/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		format.c
 *	DESCRIPTION:	Print planner and formatter
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <string.h>
#include "../jrd/jrd_time.h"
#include "../qli/dtr.h"
#include "../qli/exe.h"
#include "../jrd/gds.h"
#include "../qli/compile.h"
#include "../qli/report.h"
#include "../qli/format.h"
#include "../qli/all_proto.h"
#include "../qli/err_proto.h"
#include "../qli/eval_proto.h"
#include "../qli/exe_proto.h"
#include "../qli/form_proto.h"
#include "../qli/forma_proto.h"
#include "../qli/mov_proto.h"
#include "../qli/picst_proto.h"

extern USHORT QLI_columns, QLI_lines;
#ifdef PYXIS
extern USHORT sw_forms;
#endif
#ifdef DEV_BUILD
extern USHORT QLI_hex_output;

#define PRINTABLE(x) ((((x) >= ' ') && ((x) <= 127)) ||\
		     ((x) == '\n')||\
		     ((x) == '\t')||\
		     ((x) == '\r')||\
		     ((x) == '\f'))
#endif

static int decompose_header(SCHAR *, SCHAR **, SSHORT *);
static void format_index(ITM, QLI_NOD, USHORT);
static TEXT *format_report(VEC, USHORT, USHORT *);
static void format_value(ITM, int);
static TEXT *get_buffer(STR *, TEXT *, USHORT);
static int match_expr(QLI_NOD, QLI_NOD);
static void print_blobs(PRT, ITM *, ITM *);
static int print_line(ITM, TEXT **);
static void put_line(PRT, TEXT **, TEXT *, TEXT);
static void report_break(BRK, VEC *, int);
static void report_item(ITM, VEC *, USHORT *);
static void report_line(QLI_NOD, VEC *);

static STR fmt_buffer, blob_buffer;

#define BOTTOM_INIT			get_buffer (&fmt_buffer, NULL, 1024)
#define BOTTOM_CHECK(ptr, length)	ptr = get_buffer (&fmt_buffer, ptr, length)
#define BOTTOM_LINE			fmt_buffer->str_data

#define BUFFER_INIT			get_buffer (&fmt_buffer, NULL, 1024)
#define BUFFER_CHECK(ptr, length)	ptr = get_buffer (&fmt_buffer, ptr, length)
#define BUFFER_BEGINNING		fmt_buffer->str_data
#define BUFFER_REMAINING(ptr)		(fmt_buffer->str_length - (ptr - fmt_buffer->str_data))

int FMT_expression( QLI_NOD node)
{
/**************************************
 *
 *	F M T _ e x p r e s s i o n
 *
 **************************************
 *
 * Functional description
 *	Handle formatting for FORMAT expression.  Return editted
 *	length.
 *
 **************************************/
	PICS picture;
	QLI_NOD sub;
	QLI_FLD field;

	sub = node->nod_arg[e_fmt_value];
	picture = PIC_analyze((TEXT*) node->nod_arg[e_fmt_edit], &sub->nod_desc);
	node->nod_arg[e_fmt_picture] = (QLI_NOD) picture;

	if (node->nod_type == nod_reference)
		node = node->nod_arg[0];

	if (!(picture->pic_missing) && (node->nod_type == nod_field) &&
		(field = (QLI_FLD) node->nod_arg[e_fld_field]) && field->fld_missing)
		PIC_missing(field->fld_missing, picture);

	return picture->pic_length;
}


TEXT *FMT_format(LLS stack)
{
/**************************************
 *
 *	F M T _ f o r m a t
 *
 **************************************
 *
 * Functional description
 *	Format a print line.  Figure out spacing, print formats, and
 *	headers.  Return a pointer to the header string.
 *
 **************************************/
	ITM item, item2;
	STR header;
	QLI_NOD value;
	SSHORT segment;
	USHORT j, l, offset, max_offset, number_segments, n, lengths[10], *ptr,
		flag;
	LLS temp, temp2;
	TEXT *line, *p, *q, *segments[10], *bottom;
	ULONG size;

/* Start by inverting the item stack into an item que */

	temp = stack;
	stack = NULL;

	if (fmt_buffer) {
		ALL_release((FRB) fmt_buffer);
		fmt_buffer = NULL;
	}

	while (temp) {
		item = (ITM) LLS_POP(&temp);
		LLS_PUSH(item, &stack);
	}

/* Make a pass thru print items computing print lengths and header
   lengths, and the number of header segments. */

	number_segments = offset = max_offset = 0;
	bottom = BOTTOM_INIT;

	for (temp = stack; temp; temp = temp->lls_next) {
		item = (ITM) temp->lls_object;
		switch (item->itm_type) {
		case item_column:
			offset = (item->itm_count) ? item->itm_count - 1 : 0;
			continue;

		case item_skip:
		case item_new_page:
			offset = 0;
			continue;

		case item_space:
			offset += item->itm_count;
			continue;

		case item_tab:
			offset = (offset & ~7) + item->itm_count * 8;
			continue;
		}

		if (item->itm_type == item_value && (value = item->itm_value)) {
			if (value->nod_type == nod_reference)
				value = value->nod_arg[0];
			format_index(item, value, TRUE);
		}

		if (item->itm_query_header)
			if (*item->itm_query_header == '-')
				item->itm_query_header = NULL;
			else {
				n =
					decompose_header(item->itm_query_header, segments,
									 (SSHORT*) lengths);
				number_segments = MAX(n, number_segments);
				for (j = 0, ptr = lengths; j < n; j++, ptr++)
					item->itm_header_length =
						MAX(item->itm_header_length, *ptr);
			}

		format_value(item, 0);

		/* If the item would overflow the line, reset to beginning of line */

		if (offset + MAX(item->itm_print_length, item->itm_header_length) >
			QLI_columns) offset = 0;

		/* Before we blindly format the header, make sure there already isn't
		   header information in the same location */

		if (item->itm_query_header) {
			n = MAX(item->itm_print_length, item->itm_header_length);
			BOTTOM_CHECK(bottom, offset);
			q = BOTTOM_LINE + offset;
			while (bottom < q)
				*bottom++ = ' ';
			flag = TRUE;
			if (offset && q[-1] != ' ')
				flag = FALSE;
			else if (l = MIN(n, bottom - q)) {
				p = bottom;
				while (--l)
					if (*p++ != ' ') {
						flag = FALSE;
						break;
					}
				if (flag && p < bottom && *p != ' ')
					flag = FALSE;
			}
			if (flag && (l = n)) {
				BOTTOM_CHECK(bottom, bottom - BOTTOM_LINE + n);
				do
					*bottom++ = '=';
				while (--l);
			}
			else {
				item->itm_query_header = NULL;
				item->itm_header_length = 0;
			}
		}

		/* Now that have settled the issue of header, decide where to put
		   the field and header */

		n = MAX(item->itm_print_length, item->itm_header_length);
		item->itm_print_offset = offset + (n - item->itm_print_length) / 2;
		item->itm_header_offset = offset + n / 2;
		offset += n + 1;
		max_offset = MAX(max_offset, offset);
	}

/* Make another pass checking for overlapping fields */

	for (temp = stack; temp; temp = temp->lls_next) {
		item = (ITM) temp->lls_object;
		if (item->itm_type != item_value)
			continue;
		for (temp2 = temp->lls_next; temp2; temp2 = temp2->lls_next) {
			item2 = (ITM) temp2->lls_object;
			if (item2->itm_type != item_value)
				continue;
			if (item2->itm_print_offset <
				item->itm_print_offset + item->itm_print_length) {
				item->itm_flags |= ITM_overlapped;
				break;
			}
		}
	}

	if (number_segments == 0)
		return NULL;

/* Allocate a string block big enough to hold all lines of the print header */

	size = (max_offset + 1) * (number_segments + 1) + 2;

	if (size >= 60000)
		ERRQ_print_error(482, (TEXT *) max_offset,
						 (TEXT *) (number_segments + 1), NULL, NULL, NULL);

	header = (STR) ALLOCDV(type_str, size);
	p = header->str_data;

/* Generate the various lines of the header line at a time. */

	for (j = 0; j < number_segments; j++) {
		*p++ = '\n';
		line = p;
		for (temp = stack; temp; temp = temp->lls_next) {
			item = (ITM) temp->lls_object;
			if (item->itm_type != item_value)
				continue;
			n = decompose_header(item->itm_query_header, segments, (SSHORT*) lengths);
			segment = j - (number_segments - n);
			if (segment < 0)
				continue;
			l = lengths[segment];
			q = line + item->itm_header_offset - l / 2;

			while (p < q)
				*p++ = ' ';
			q = segments[segment];
			if (l)
				do
					*p++ = *q++;
				while (--l);
		}
	}

/* Make one last pass to put in underlining of headers */

	if (l = bottom - BOTTOM_LINE) {
		*p++ = '\n';
		bottom = BOTTOM_LINE;
		do
			*p++ = *bottom++;
		while (--l);
	}

	*p++ = '\n';
	*p++ = '\n';
	*p = 0;

	return header->str_data;
}


QLI_NOD FMT_list(QLI_NOD list)
{
/**************************************
 *
 *	F M T _ l i s t
 *
 **************************************
 *
 * Functional description
 *	Rebuild and format a list of stuff for vertical formatting.
 *
 **************************************/
	ITM new_item, *item, *end, *new_ptr;
	SYM name;
	QLI_FLD field;
	QLI_NOD value, new_nod;
	USHORT column, expression;
	DSC *desc;
	STR header;
	TEXT *p, *q, c;

	new_nod = (QLI_NOD) ALLOCDV(type_nod, list->nod_count * 2 + 1);
	new_nod->nod_type = nod_list;
	new_ptr = (ITM *) new_nod->nod_arg;
	column = 0;

	for (item = (ITM *) list->nod_arg, end = item + list->nod_count;
		 item < end; item++) {
		if ((*item)->itm_type != item_value || !(value = (*item)->itm_value))
			continue;
		(*item)->itm_flags |= ITM_overlapped;
		format_value(*item, PIC_suppress_blanks);
		if (value->nod_type == nod_reference)
			value = value->nod_arg[0];
		if (value->nod_type == nod_field ||
			value->nod_type == nod_variable ||
			value->nod_type == nod_function) {
			expression = FALSE;
			if (value->nod_type != nod_function) {
				field = (QLI_FLD) value->nod_arg[e_fld_field];
				name = field->fld_name;
				format_index(*item, value, FALSE);
			}
			else
				name = ((FUN) value->nod_arg[e_fun_function])->fun_symbol;
		}
		else
			expression = TRUE;
		*new_ptr++ = new_item = (ITM) ALLOCD(type_itm);
		new_item->itm_type = item_value;
		new_item->itm_value = value = (QLI_NOD) ALLOCDV(type_nod, 0);
		value->nod_type = nod_constant;
		value->nod_flags |= NOD_local;
		value->nod_desc.dsc_dtype = dtype_text;
		if (!expression && (!(q = (*item)->itm_query_header) || *q != '-')) {
			if (q) {
				if (*q != '"' && *q != '\'')
					value->nod_desc.dsc_address = (UCHAR *) q;
				else {
					header = (STR) ALLOCDV(type_str, strlen(q));
					p = header->str_data;
					value->nod_desc.dsc_address = (UCHAR *) p;
					while (c = *q++) {
						while (*q != c)
							*p++ = *q++;
						*p++ = ' ';
						q++;
					}
					p[-1] = 0;
				}
				value->nod_desc.dsc_length =
					strlen((char*) value->nod_desc.dsc_address);
			}
			else {
				value->nod_desc.dsc_length = name->sym_length;
				value->nod_desc.dsc_address = (UCHAR *) name->sym_string;
			}
			column = MAX(column, value->nod_desc.dsc_length);
			new_item->itm_picture = PIC_analyze(0, &value->nod_desc);
		}
		else {
			desc = EVAL_value(value);
			new_item->itm_picture = PIC_analyze(0, desc);
		}

		if (!new_item->itm_picture->pic_missing &&
			value->nod_type == nod_field && field->fld_missing)
			PIC_missing(field->fld_missing, new_item->itm_picture);

		*new_ptr++ = *item;
	}

	*new_ptr++ = new_item = (ITM) ALLOCD(type_itm);
	new_item->itm_type = item_skip;
	new_item->itm_count = 1;
	column += 2;

	for (item = (ITM *) list->nod_arg, end = item + list->nod_count;
		 item < end; item++) {
		if ((*item)->itm_type != item_value || !(value = (*item)->itm_value))
			continue;
		if (value->nod_type == nod_reference)
			value = value->nod_arg[0];
		(*item)->itm_print_offset = column;
	}

	new_nod->nod_count = new_ptr - (ITM *) new_nod->nod_arg;

	return new_nod;
}


void FMT_print( QLI_NOD list, PRT print)
{
/**************************************
 *
 *	F M T _ p r i n t
 *
 **************************************
 *
 * Functional description
 *	Format a print line.  Return the number of lines printed.
 *
 **************************************/
	ITM item;
	USHORT l;
	DSC *desc;
	TEXT *p, *q, *buffer;
	QLI_NOD *ptr, *end;
	RPT report;
	STATUS status_vector[ISC_STATUS_LENGTH];

/* Now go thru and make up the first line */

	if (!list)
		return;
	p = BUFFER_INIT;
	end = list->nod_arg + list->nod_count;

	for (ptr = list->nod_arg; ptr < end; ptr++) {
		item = (ITM) * ptr;

		/* Handle formating directives.  Most have been translated into
		   column assignments and are no-ops. */

		buffer = BUFFER_BEGINNING;

		switch (item->itm_type) {
		case item_value:
			break;

		case item_new_page:
			if (print->prt_new_page) {
				put_line(print, &p, buffer, '\n');
				(*(void(*)(PRT, int))print->prt_new_page) (print, FALSE);
			}
			else {
				put_line(print, &p, buffer, '\f');
				QLI_skip_line = FALSE;
			}
			continue;

		case item_skip:
			put_line(print, &p, buffer, '\n');
			print_blobs(print, (ITM*) list->nod_arg, (ITM*) ptr);
			for (l = item->itm_count - 1; l > 0; --l)
				put_line(print, &p, buffer, '\n');
			QLI_skip_line = FALSE;
			continue;

		case item_column_header:
			if ((report = print->prt_report) && report->rpt_column_header)
				FMT_put(report->rpt_column_header, print);
			continue;

		case item_report_header:
			if ((report = print->prt_report) && report->rpt_header)
				FMT_put(report->rpt_header, print);
			continue;

		case item_column:
		case item_tab:
		case item_space:
		default:
			continue;
		}

		/* Handle print items.  Start by by spacing out to the correct column,
		   forcing a new line if required. */

		BUFFER_CHECK(p, item->itm_print_offset + item->itm_print_length + 2);
		buffer = BUFFER_BEGINNING;
		q = buffer + item->itm_print_offset;
		if (p > q) {
			put_line(print, &p, buffer, '\n');
			print_blobs(print, (ITM*) list->nod_arg, (ITM*) ptr);
		}
		while (p < q)
			*p++ = ' ';

		/* Next, handle simple formated values */

		if (item->itm_dtype != dtype_blob) {
			desc = EVAL_value(item->itm_value);
			if (!(desc->dsc_missing & DSC_missing))
				PIC_edit(desc, item->itm_picture, &p, BUFFER_REMAINING(p));
			else if (item->itm_picture->pic_missing)
				PIC_edit(desc, item->itm_picture->pic_missing, &p,
						 BUFFER_REMAINING(p));
			continue;
		}

		/* Finally, handle blobs */

		if (!(item->itm_stream = EXEC_open_blob(item->itm_value)))
			continue;

		if (print_line(item, &p) != EOF)
			if (item->itm_flags & ITM_overlapped)
				for (;;) {
					put_line(print, &p, buffer, '\n');
					while (p < q)
						*p++ = ' ';
					if (print_line(item, &p) == EOF)
						break;
				}
	}

	put_line(print, &p, buffer, '\n');

/* Now go back until all blobs have been fetched */

	print_blobs(print, (ITM*) list->nod_arg, (ITM*) end);

/* Finish by closing all blobs */

	for (ptr = list->nod_arg; ptr < end; ptr++) {
		item = (ITM) * ptr;
		if (item->itm_dtype == dtype_blob && item->itm_stream)
			gds__close_blob(status_vector, GDS_REF(item->itm_stream));
	}
}


void FMT_put( TEXT * line, PRT print)
{
/**************************************
 *
 *	F M T _ p u t
 *
 **************************************
 *
 * Functional description
 *	Write out an output file.   Write
 *	fewer than 256 characters at at time
 *	to avoid annoying VMS.
 *
 **************************************/
	TEXT *p, *q, *end, buffer[256];

	for (p = line; *p; p++)
		if (*p == '\n' || *p == '\f')
			--print->prt_lines_remaining;

	end = buffer + sizeof(buffer) - 1;
	q = line;

	if (print && print->prt_file)
		while (*q) {
			for (p = buffer; p < end && *q;)
				*p++ = *q++;
			*p = 0;
			ib_fprintf((IB_FILE *) print->prt_file, "%s", buffer);
		}
	else {
#ifdef PYXIS
		if (sw_forms)
			FORM_reset();
#endif
		while (*q) {
			for (p = buffer; p < end && *q;)
				*p++ = *q++;
			*p = 0;
#ifdef DEV_BUILD
			if (QLI_hex_output) {
				/* Hex mode output to assist debugging of multicharset work */

				for (p = buffer; p < end && *p; p++)
					if (PRINTABLE(*p))
						ib_fprintf(ib_stdout, "%c", *p);
					else
						ib_fprintf(ib_stdout, "[%02.2X]", *(UCHAR *) p);
			}
			else
#endif
				ib_fprintf(ib_stdout, "%s", buffer);
		}
		QLI_skip_line = TRUE;
	}
}


void FMT_report( RPT report)
{
/**************************************
 *
 *	F M T _ r e p o r t
 *
 **************************************
 *
 * Functional description
 *	Format a report.
 *
 **************************************/
	QLI_NOD list;
	USHORT lengths[16], n, i, column, width;
	TEXT *segments[16], *p, *q, *end;
	STR string;
	VEC columns_vec;

	if (fmt_buffer) {
		ALL_release((FRB) fmt_buffer);
		fmt_buffer = NULL;
	}

	width = report->rpt_columns;
	columns_vec = (VEC) ALLOCDV(type_vec, 256);
	columns_vec->vec_count = 256;
	columns_vec->vec_object[0] = NULL;

	report_break(report->rpt_top_rpt, &columns_vec, FALSE);
	report_break(report->rpt_top_page, &columns_vec, FALSE);
	report_break(report->rpt_top_breaks, &columns_vec, FALSE);

	if (list = report->rpt_detail_line)
		report_line(list, &columns_vec);

	report_break(report->rpt_bottom_breaks, &columns_vec, TRUE);
	report_break(report->rpt_bottom_page, &columns_vec, TRUE);
	report_break(report->rpt_bottom_rpt, &columns_vec, TRUE);

	report->rpt_column_header = format_report(columns_vec, width, &width);

/* Handle report name, if any */

	if (report->rpt_name) {
		n = decompose_header(report->rpt_name, segments, (SSHORT*) lengths);
		for (i = 0; i < n; i++)
			width = MAX(width, lengths[i] + 15);
		string = (STR) ALLOCDV(type_str, width * n);
		report->rpt_header = p = string->str_data;
		for (i = 0; i < n; i++) {
			column = (width - lengths[i]) / 2;
			if (column > 0)
				do
					*p++ = ' ';
				while (--column);
			q = segments[i];
			end = q + lengths[i];
			while (q < end)
				*p++ = *q++;
			*p++ = '\n';
		}
	}
}


static int decompose_header(
							SCHAR * string,
							SCHAR ** segments, SSHORT * lengths)
{
/**************************************
 *
 *	d e c o m p o s e _ h e a d e r
 *
 **************************************
 *
 * Functional description
 *	Decompose a header string (aka field name) into segments.
 *	Return the address of and length of each segment (in arrays)
 *	and the number of segments.
 *
 **************************************/
	TEXT c;
	SSHORT n;

	if (!string)
		return 0;

	n = 0;

/* Handle simple name first */

	if (*string != '"' && *string != '\'')
		while (*string) {
			*segments = string;
			while (*string && *string != '_')
				string++;
			*lengths++ = string - *segments++;
			n++;
			if (*string == '_')
				string++;
		}
	else
		while (c = *string++) {
			*segments = string;
			while (*string++ != c);
			*lengths++ = string - *segments++ - 1;
			n++;
		}

	return n;
}


static void format_index( ITM item, QLI_NOD field, USHORT print_flag)
{
/**************************************
 *
 *	f o r m a t _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Format the label of a subscripted item.
 *
 **************************************/
	QLI_NOD args, *ptr, *end, subscript;
	USHORT l, length;
	TEXT s[32], *p, *q, *r;
	STR str;

/* Don't bother with anything except non-indexed fields.  Also
   ignore subscripted fields with user specified query headers. */

	if (field->nod_type != nod_field ||
		!(args = field->nod_arg[e_fld_subs]) ||
		((p = item->itm_query_header) && (*p == '"' || *p == '\'')))
		return;

/* Start the label with the current query header, if any */

	if (item->itm_query_header) {
		q = item->itm_query_header;
		l = strlen(item->itm_query_header);
	}
	else {
		q = ((QLI_FLD) field->nod_arg[e_fld_field])->fld_name->sym_string;
		l = ((QLI_FLD) field->nod_arg[e_fld_field])->fld_name->sym_length;
	}

	length = l + 2;
	str = NULL;
	p = get_buffer(&str, NULL, length + 32);
	while (l--)
		*p++ = *q++;

/* Loop through the subscripts, adding to the label */

	if (print_flag) {
		r = "_[";
		length++;
	}
	else
		r = "[";
	for (ptr = args->nod_arg, end = ptr + args->nod_count; ptr < end; ptr++) {
		subscript = *ptr;
		switch (subscript->nod_type) {
		case nod_constant:
			sprintf(s, "%ld", MOVQ_get_long(&subscript->nod_desc, 0));
			q = s;
			l = strlen(s);
			break;

		case nod_variable:
		case nod_field:
			q = ((QLI_FLD) subscript->nod_arg[e_fld_field])->fld_name->sym_string;
			l = ((QLI_FLD) subscript->nod_arg[e_fld_field])->fld_name->sym_length;
			break;

		default:
			/* Punt on anything but constants, fields, and variables */

			ALL_release((FRB) str);
			return;
		}

		length += l + 1;
		p = get_buffer(&str, p, length);
		while (*r)
			*p++ = *r++;
		while (l--)
			*p++ = *q++;
		r = ",";
	}

	if (*r == ',')
		*p++ = ']';
	*p = 0;
	item->itm_query_header = str->str_data;
}


static TEXT *format_report( VEC columns_vec, USHORT width, USHORT * max_width)
{
/**************************************
 *
 *	f o r m a t _ r e p o r t
 *
 **************************************
 *
 * Functional description
 *	Format a report.  Figure out spacing, print formats, and
 *	headers.  Return a pointer to the header string.
 *
 **************************************/
	ITM item;
	STR header;
	QLI_NOD node;
	SSHORT segment;
	USHORT j, l, offset, max_offset, number_segments, n, lengths[10],
		*ptr, flag, column_width, max_print_width, right_adjust, right_offset;
	LLS *col, *col_end, temp;
	TEXT *line, *p, *q, *segments[10], *bottom;

/* Make a pass thru print items computing print lengths and header
   lengths, and the number of header segments. */

	number_segments = offset = max_offset = 0;
	bottom = BOTTOM_INIT;

	col = (LLS *) columns_vec->vec_object;
	for (col_end = col + columns_vec->vec_count; col < col_end && *col; col++) {
		column_width = max_print_width = 0;
		right_adjust = FALSE;
		for (temp = *col; temp; temp = temp->lls_next) {
			item = (ITM) temp->lls_object;
			switch (item->itm_type) {
			case item_column:
				offset = (item->itm_count) ? item->itm_count - 1 : 0;
				continue;

			case item_skip:
			case item_new_page:
			case item_report_header:
			case item_column_header:
				offset = 0;
				continue;

			case item_space:
				offset += item->itm_count;
				continue;

			case item_tab:
				offset = (offset & ~7) + item->itm_count * 8;
				continue;

			case item_value:
				max_print_width =
					MAX(max_print_width, item->itm_print_length);
				node = item->itm_value;
				if (node->nod_desc.dsc_dtype >= dtype_short &&
					node->nod_desc.dsc_dtype <= dtype_double)
					right_adjust = TRUE;
			}

			if (item->itm_query_header) {
				n =
					decompose_header(item->itm_query_header, segments,
									 (SSHORT*) lengths);
				number_segments = MAX(n, number_segments);
				for (j = 0, ptr = lengths; j < n; j++, ptr++)
					item->itm_header_length =
						MAX(item->itm_header_length, *ptr);
			}

			format_value(item, 0);
			n = MAX(item->itm_print_length, item->itm_header_length);
			column_width = MAX(column_width, n);
		}

		if (offset + column_width > width)
			offset = 0;

		right_offset = column_width - max_print_width / 2;

		for (temp = *col; temp; temp = temp->lls_next) {
			item = (ITM) temp->lls_object;
			if (item->itm_type != item_value)
				continue;

			if (right_adjust)
				item->itm_print_offset =
					offset + right_offset - item->itm_print_length;
			else
				item->itm_print_offset =
					offset + (column_width - item->itm_print_length) / 2;

			item->itm_header_offset = offset + column_width / 2;

			/* Before we blindly format the header, make sure there already isn't
			   header information in the same location */

			if (item->itm_query_header) {
				BOTTOM_CHECK(bottom, offset);
				q = BOTTOM_LINE + offset;
				while (bottom < q)
					*bottom++ = ' ';
				flag = TRUE;
				if (offset && q[-1] != ' ')
					flag = FALSE;
				else if (l = MIN(column_width, bottom - q)) {
					p = bottom;
					while (--l)
						if (*p++ != ' ') {
							flag = FALSE;
							break;
						}
					if (flag && p < bottom && *p != ' ')
						flag = FALSE;
				}
				if (flag) {
					BOTTOM_CHECK(bottom, offset + column_width);
					for (q = BOTTOM_LINE + offset + column_width; bottom < q;)
						*bottom++ = '=';
				}
				else
					item->itm_query_header = NULL;
			}
		}
		offset += column_width + 1;
		max_offset = MAX(max_offset, offset);
	}

	*max_width = max_offset;

	if (number_segments == 0)
		return NULL;

/* Allocate a string block big enough to hold all lines of the print header */

	l = bottom - BOTTOM_LINE;
	header =
		(STR) ALLOCDV(type_str,
					  (max_offset + 1) * (number_segments + 1) + 2 + l);
	p = header->str_data;

/* Generate the various lines of the header line at a time. */

	for (j = 0; j < number_segments; j++) {
		*p++ = '\n';
		line = p;
		col = (LLS *) columns_vec->vec_object;
		for (col_end = col + columns_vec->vec_count; col < col_end && *col;
			 col++)
			for (temp = *col; temp; temp = temp->lls_next) {
				item = (ITM) temp->lls_object;
				if (item->itm_type != item_value)
					continue;
				n =
					decompose_header(item->itm_query_header, segments,
									 (SSHORT*) lengths);
				segment = j - (number_segments - n);
				if (segment < 0)
					continue;
				l = lengths[segment];
				q = line + item->itm_header_offset - l / 2;
				while (p < q)
					*p++ = ' ';
				q = segments[segment];
				if (l)
					do
						*p++ = *q++;
					while (--l);
			}
	}

/* Make one last pass to put in underlining of headers */

	if (l = bottom - BOTTOM_LINE) {
		*p++ = '\n';
		bottom = BOTTOM_LINE;
		do
			*p++ = *bottom++;
		while (--l);
	}

	*p++ = '\n';
	*p++ = '\n';
	*p = 0;

	return header->str_data;
}


static void format_value( ITM item, int flags)
{
/**************************************
 *
 *	f o r m a t _ v a l u e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	QLI_NOD node;
	DSC *desc;
	PICS picture;
	QLI_FLD field;

	node = item->itm_value;
	desc = &node->nod_desc;
	item->itm_dtype = desc->dsc_dtype;
	item->itm_sub_type = desc->dsc_sub_type;

	if (desc->dsc_dtype == dtype_blob) {
		item->itm_print_length = 40;
		if (node->nod_type == nod_reference)
			node = node->nod_arg[0];
		if (node->nod_type == nod_field) {
			field = (QLI_FLD) node->nod_arg[e_fld_field];
			if (field->fld_segment_length)
				item->itm_print_length = field->fld_segment_length;
		}
	}
	else {
		item->itm_picture = picture =
			PIC_analyze(item->itm_edit_string, desc);

		if (node->nod_type == nod_reference)
			node = node->nod_arg[0];

		if (node->nod_type == nod_field) {
			field = (QLI_FLD) node->nod_arg[e_fld_field];
			if ((field->fld_flags & FLD_array) && !node->nod_arg[e_fld_subs])
				ERRQ_print_error(480, field->fld_name->sym_string, NULL, NULL,
								 NULL, NULL);	/* msg 480 can not format unsubscripted array %s */
		}

		if (!(item->itm_picture->pic_missing) &&
			(node->nod_type == nod_field) &&
			(field = (QLI_FLD) node->nod_arg[e_fld_field]) && field->fld_missing)
			PIC_missing(field->fld_missing, picture);
		item->itm_print_length = picture->pic_length;
		picture->pic_flags |= flags;
	}
}


static TEXT *get_buffer( STR * str, TEXT * ptr, USHORT length)
{
/**************************************
 *
 *	g e t _ b u f f e r
 *
 **************************************
 *
 * Functional description
 *	Make sure we have a large enough buffer.
 *	If the current one is too short, copy the
 *	current buffer to the new one.
 *
 **************************************/
	STR temp_str;
	TEXT *p, *q;
	USHORT l;

	if (!*str) {
		*str = (STR) ALLOCPV(type_str, length);
		(*str)->str_length = length;
		return (*str)->str_data;
	}

	if (length <= (*str)->str_length)
		return (ptr) ? ptr : (*str)->str_data;

	temp_str = (STR) ALLOCPV(type_str, length);
	temp_str->str_length = length;
	p = temp_str->str_data;
	q = (*str)->str_data;

	if (ptr && (l = ptr - q))
		do
			*p++ = *q++;
		while (--l);

	ALL_release((FRB) *str);
	*str = temp_str;

	return p;
}


static int match_expr( QLI_NOD node1, QLI_NOD node2)
{
/**************************************
 *
 *	m a t c h _ e x p r
 *
 **************************************
 *
 * Functional description
 *	Compare two nodes for equality of value.
 *
 **************************************/
	QLI_NOD *ptr1, *ptr2, *end;

/* If either is missing, they can't match. */

	if (!node1 || !node2)
		return FALSE;

	if (node1->nod_type == nod_reference)
		node1 = node1->nod_arg[0];

	if (node2->nod_type == nod_reference)
		node2 = node2->nod_arg[0];

/* A constant more or less matches anything */

	if (node1->nod_type == nod_constant)
		return TRUE;

/* Hasn't matched yet.  Check for statistical expression */

	switch (node1->nod_type) {
	case nod_average:
	case nod_max:
	case nod_min:
	case nod_total:

	case nod_rpt_average:
	case nod_rpt_max:
	case nod_rpt_min:
	case nod_rpt_total:
	case nod_running_total:

	case nod_agg_average:
	case nod_agg_max:
	case nod_agg_min:
	case nod_agg_total:
		return match_expr(node1->nod_arg[e_stt_value], node2);
	}

	switch (node2->nod_type) {
	case nod_average:
	case nod_max:
	case nod_min:
	case nod_total:

	case nod_rpt_average:
	case nod_rpt_max:
	case nod_rpt_min:
	case nod_rpt_total:
	case nod_running_total:

	case nod_agg_average:
	case nod_agg_max:
	case nod_agg_min:
	case nod_agg_total:
		return match_expr(node1, node2->nod_arg[e_stt_value]);
	}

	if (node1->nod_type == node2->nod_type) {
		if (node1->nod_type == nod_field) {
			if (node1->nod_arg[e_fld_field] != node2->nod_arg[e_fld_field] ||
				node1->nod_arg[e_fld_context] !=
				node2->nod_arg[e_fld_context]) return FALSE;
			return TRUE;
		}
		ptr1 = node1->nod_arg;
		ptr2 = node2->nod_arg;
		for (end = ptr1 + node1->nod_count; ptr1 < end; ptr1++, ptr2++)
			if (!match_expr(*ptr1, *ptr2))
				return FALSE;
		return TRUE;
	}

	return FALSE;
}


static void print_blobs( PRT print, ITM * first, ITM * last)
{
/**************************************
 *
 *	p r i n t _ b l o b s
 *
 **************************************
 *
 * Functional description
 *	Print any blobs still active in item list.
 *
 **************************************/
	ITM *ptr, item;
	USHORT blob_active, do_line, length;
	TEXT *p, *q, *buffer, *pp;
	int c;

	if (QLI_abort)
		return;

	length = 0;
	for (ptr = first; ptr < last; ptr++) {
		item = *ptr;
		if (item->itm_dtype == dtype_blob && item->itm_stream)
			length =
				MAX(length,
					item->itm_print_offset + item->itm_print_length + 2);
	}


	buffer = get_buffer(&blob_buffer, NULL, length);

	while (!QLI_abort) {
		blob_active = FALSE;
		p = buffer;
		do_line = FALSE;
		for (ptr = first; ptr < last; ptr++) {
			item = *ptr;
			if (item->itm_dtype != dtype_blob || !item->itm_stream)
				continue;
			q = buffer + item->itm_print_offset;
			while (p < q)
				*p++ = ' ';
			pp = p;
			c = print_line(item, &p);
			if (c != EOF)
				blob_active = TRUE;
			if (pp != p || c == '\n')
				do_line = TRUE;
		}
		if (do_line)
			put_line(print, &p, buffer, '\n');
		if (!blob_active)
			break;
	}
}


static int print_line( ITM item, TEXT ** ptr)
{
/**************************************
 *
 *	p r i n t _ l i n e
 *
 **************************************
 *
 * Functional description
 *	Print a line of a blob or scratch file.  The
 *	last thing printed.
 *
 **************************************/
	USHORT l, length;
	TEXT *p;
	STATUS status_vector[ISC_STATUS_LENGTH], status;

	EXEC_poll_abort();

/* If we're already at end of stream, there's nothing to do */

	if (!item->itm_stream)
		return EOF;

	p = *ptr;
	l = item->itm_print_length;


	if ((status = gds__get_segment(status_vector,
								   GDS_REF(item->itm_stream),
								   GDS_REF(length),
								   l, GDS_VAL(p))) && status != gds_segment) {
		long *null_status = 0;
		gds__close_blob(null_status, GDS_REF(item->itm_stream));
		if (status != gds_segstr_eof)
			ERRQ_database_error(0, status_vector);
		return EOF;
	}

/* If this is not a partial segment and the last character
   is a newline, throw away the newline */

	if (!status && length && p[length - 1] == '\n')
		if (length > 1)
			--length;
		else
			p[0] = ' ';

	*ptr = p + length;

/* Return the last character in the segment.
   If the segment is null, return a newline. */

	return (length) ? p[length - 1] : '\n';
}


static void put_line( PRT print, TEXT ** ptr, TEXT * buffer, TEXT terminator)
{
/**************************************
 *
 *	p u t _ l i n e
 *
 **************************************
 *
 * Functional description
 *	Given a file descriptor, a running output pointer, and the address
 *	of the original buffer, write out the current line and reset the
 *	pointer.
 *
 **************************************/

	*(*ptr)++ = terminator;
	**ptr = 0;
	FMT_put(buffer, print);
	*ptr = buffer;
}


static void report_break( BRK control, VEC * columns_vec, int bottom_flag)
{
/**************************************
 *
 *	r e p o r t _ b r e a k
 *
 **************************************
 *
 * Functional description
 *	Handle formatting for a chain of control breaks.
 *
 **************************************/

	if (!control)
		return;

	if (bottom_flag) {
		if (control->brk_next)
			report_break(control->brk_next, columns_vec, bottom_flag);
		if (control->brk_line)
			report_line((QLI_NOD) control->brk_line, columns_vec);
		return;
	}

	for (; control; control = control->brk_next)
		if (control->brk_line)
			report_line((QLI_NOD) control->brk_line, columns_vec);
}


static void report_item( ITM item, VEC * columns_vec, USHORT * col_ndx)
{
/**************************************
 *
 *	r e p o r t _ i t e m
 *
 **************************************
 *
 * Functional description
 *	Insert a report item into a logical column.  It it fits
 *	someplace reasonable, stick it there.
 *
 **************************************/
	ITM item2;
	VEC columns;
	LLS *col, *col_end, temp;
	QLI_NOD node;
	USHORT new_index;

	if (item->itm_query_header && *item->itm_query_header == '-')
		item->itm_query_header = NULL;

/* If it's a constant, dump it in the next logical column */

	columns = *columns_vec;
	if (columns->vec_object[*col_ndx] &&
		(node = item->itm_value) && node->nod_type == nod_constant) {
		LLS_PUSH(item, (LLS*) (columns->vec_object + *col_ndx));
		return;
	}

/* Loop thru remaining logical columns looking for an equivalent
   expression.  If we find one, the item beSLONGs in that column;
   otherwise, someplace else. */

	col = (LLS *) (columns->vec_object + *col_ndx);
	col_end = (LLS *) (columns->vec_object + columns->vec_count);
	for (; col < col_end && *col; col++)
		for (temp = *col; temp; temp = temp->lls_next) {
			item2 = (ITM) temp->lls_object;
			if (match_expr(item->itm_value, item2->itm_value)) {
				LLS_PUSH(item, col);
				*col_ndx = col - (LLS *) columns->vec_object;
				return;
			}
		}

/* Didn't fit -- make a new logical column */

	*col_ndx = new_index = col - (LLS *) columns->vec_object;
	if (new_index >= columns->vec_count) {
		ALLQ_extend((BLK*) columns_vec, new_index + 16);
		(*columns_vec)->vec_count = new_index + 16;
	}

	LLS_PUSH(item, (LLS*) ((*columns_vec)->vec_object + new_index));
}


static void report_line( QLI_NOD list, VEC * columns_vec)
{
/**************************************
 *
 *	r e p o r t _ l i n e
 *
 **************************************
 *
 * Functional description
 *	Process a report line.
 *
 **************************************/
	ITM item, *ptr, *end;
	USHORT col_ndx;

	col_ndx = 0;
	for (ptr = (ITM *) list->nod_arg, end = ptr + list->nod_count; ptr < end;
		 ptr++) {
		item = *ptr;
		report_item(item, columns_vec, &col_ndx);
		switch (item->itm_type) {
		case item_skip:
		case item_new_page:
			col_ndx = 0;
			break;
		}
	}
}
