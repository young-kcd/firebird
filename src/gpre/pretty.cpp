//____________________________________________________________
//  
//		PROGRAM:	BLR Pretty Printer
//		MODULE:		pretty.cpp
//		DESCRIPTION:	BLR Pretty Printer
//  
//  The contents of this file are subject to the Interbase Public
//  License Version 1.0 (the "License"); you may not use this file
//  except in compliance with the License. You may obtain a copy
//  of the License at http://www.Inprise.com/IPL.html
//  
//  Software distributed under the License is distributed on an
//  "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
//  or implied. See the License for the specific language governing
//  rights and limitations under the License.
//  
//  The Original Code was created by Inprise Corporation
//  and its predecessors. Portions created by Inprise Corporation are
//  Copyright (C) Inprise Corporation.
//  
//  All Rights Reserved.
//  Contributor(s): ______________________________________.
//  TMN (Mike Nordell) 11.APR.2001 - Reduce compiler warnings
//  
//
//____________________________________________________________
//
//	$Id: pretty.cpp,v 1.13 2003-09-05 10:14:08 aafemt Exp $
//

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include "../jrd/common.h"
#include <stdarg.h>
#include "../jrd/gds.h"
#include "../gpre/prett_proto.h"
#include "../jrd/gds_proto.h"

#ifdef sun
#ifndef SOLARIS
extern int ib_printf();
#endif
#endif

#define ADVANCE_PTR(ptr) while (*ptr) ptr++;

#define PRINT_VERB 	if (print_verb (control, level)) return -1
#define PRINT_DYN_VERB 	if (print_dyn_verb (control, level)) return -1
#define PRINT_SDL_VERB 	if (print_sdl_verb (control, level)) return -1
#define PRINT_LINE	print_line (control, (SSHORT)offset)
#define PRINT_BYTE	print_byte (control, (SSHORT)offset)
#define PRINT_CHAR	print_char (control, (SSHORT)offset)
#define PRINT_WORD	print_word (control, (SSHORT)offset)
#define PRINT_LONG	print_long (control, (SSHORT)offset)
#define PRINT_STRING	print_string (control, (SSHORT)offset)
#define BLR_BYTE	*(control->ctl_blr)++
#define PUT_BYTE(byte)	*(control->ctl_ptr)++ = byte
#define NEXT_BYTE	*(control->ctl_blr)
#define CHECK_BUFFER	if (control->ctl_ptr > control->ctl_buffer + sizeof (control->ctl_buffer) - 20) PRINT_LINE


typedef int (*pfn_ctl_routine) (SCHAR *, SSHORT, TEXT *);

typedef struct ctl {
	SCHAR *ctl_blr;				/* Running blr string */
	SCHAR *ctl_blr_start;		/* Original start of blr string */
	pfn_ctl_routine ctl_routine;	/* Call back */
	SCHAR *ctl_user_arg;		/* User argument */
	TEXT *ctl_ptr;
	SSHORT ctl_language;
	SSHORT ctl_level;
	TEXT ctl_buffer[256];
} *CTL;

static int blr_format(CTL, const char *, ...);
static int error(CTL, int, TEXT *, int);
static int indent(CTL, SSHORT);
static int print_blr_dtype(CTL, BOOLEAN);
static int print_blr_line(CTL, USHORT, UCHAR *);
static int print_byte(CTL, SSHORT);
static int print_char(CTL, SSHORT);
static int print_dyn_verb(CTL, SSHORT);
static int print_line(CTL, SSHORT);
static SLONG print_long(CTL, SSHORT);
static int print_sdl_verb(CTL, SSHORT);
static int print_string(CTL, SSHORT);
static int print_word(CTL, SSHORT);

const char *dyn_table[] = {
#include "../gpre/dyntable.h"
	NULL
};

const char *cdb_table[] = {
#include "../gpre/cdbtable.h"
	NULL
};

const char *sdl_table[] = {
#include "../gpre/sdltable.h"
	NULL
};

const char *map_strings[] = {
	"FIELD2",
	"FIELD1",
	"MESSAGE",
	"TERMINATOR",
	"TERMINATING_FIELD",
	"OPAQUE",
	"TRANSPARENT",
	"TAG",
	"SUB_FORM",
	"ITEM_INDEX",
	"SUB_FIELD"
};

const char *menu_strings[] = {
	"LABEL",
	"ENTREE",
	"OPAQUE",
	"TRANSPARENT",
	"HORIZONTAL",
	"VERTICAL"
};

//____________________________________________________________
//  
//		Pretty print create database parameter buffer thru callback routine.
//  

int PRETTY_print_cdb( SCHAR * blr,
					 int (*routine) (), SCHAR * user_arg, SSHORT language)
{

	ctl ctl_buffer;
	ctl* control = &ctl_buffer;
	SCHAR temp[32];
	SSHORT parameter;
	SSHORT level = 0;
	SSHORT length;
	SSHORT i;
	int offset = 0;


	if (!routine) {
		routine = (int (*)()) ib_printf;
		user_arg = "%.4d %s\n";
	}

	control->ctl_routine = reinterpret_cast < pfn_ctl_routine > (routine);
	control->ctl_user_arg = user_arg;
	control->ctl_blr = control->ctl_blr_start = blr;
	control->ctl_ptr = control->ctl_buffer;
	control->ctl_language = language;

	indent(control, level);
	i = BLR_BYTE;
	if (*control->ctl_blr)
		sprintf((SCHAR *) temp, "gds__dpb_version%d, ", i);
	else
		sprintf((SCHAR *) temp, "gds__dpb_version%d", i);
	blr_format(control, temp);
	PRINT_LINE;

	while (parameter = BLR_BYTE) {
		const char *p;
		if (parameter > FB_NELEM(cdb_table) ||
			!(p = cdb_table[parameter])) {
			return error(control, 0,
						 "*** cdb parameter %d is undefined ***\n",
						 parameter);
		}
		indent(control, level);
		blr_format(control, p);
		PUT_BYTE(',');
		if (length = PRINT_BYTE) {
			do {
				PRINT_CHAR;
			} while (--length);
		}
		PRINT_LINE;
	}

	return 0;
}


int PRETTY_print_dyn(
					 SCHAR * blr,
//____________________________________________________________
//  
//		Pretty print dynamic DDL thru callback routine.
//  

					 int (*routine) (), SCHAR * user_arg, SSHORT language)
{
	ctl ctl_buffer;
	ctl* control = &ctl_buffer;
	int offset;
	SSHORT version, level;

	offset = level = 0;

	if (!routine) {
		routine = (int (*)()) ib_printf;
		user_arg = "%.4d %s\n";
	}

	control->ctl_routine = reinterpret_cast < pfn_ctl_routine > (routine);
	control->ctl_user_arg = user_arg;
	control->ctl_blr = control->ctl_blr_start = blr;
	control->ctl_ptr = control->ctl_buffer;
	control->ctl_language = language;

	version = BLR_BYTE;

	if (version != gds_dyn_version_1)
		return error(control, offset,
					 "*** dyn version %d is not supported ***\n",
					 version);

	blr_format(control, "gds__dyn_version_1, ");
	PRINT_LINE;
	level++;
	PRINT_DYN_VERB;

	if (BLR_BYTE != (SCHAR) gds_dyn_eoc)
		return error(control, offset,
					 "*** expected dyn end-of-command  ***\n", 0);

	blr_format(control, "gds__dyn_eoc");
	PRINT_LINE;

	return 0;
}

#ifdef PYXIS
int
PRETTY_print_form_map(SCHAR * blr,
//____________________________________________________________
//  
//		Pretty print a form map.
//  

					  int (*routine) (), SCHAR * user_arg, SSHORT language)
{
	ctl ctl_buffer;
	ctl* control = &ctl_buffer;
	SCHAR c;
	int offset, n;
	SSHORT version, level;

	offset = level = 0;

	if (!routine) {
		routine = (int (*)()) ib_printf;
		user_arg = "%.4d %s\n";
	}

	control->ctl_routine = reinterpret_cast < pfn_ctl_routine > (routine);
	control->ctl_user_arg = user_arg;
	control->ctl_blr = control->ctl_blr_start = blr;
	control->ctl_ptr = control->ctl_buffer;
	control->ctl_language = language;

	version = BLR_BYTE;

	if (version != PYXIS_MAP_VERSION1)
		return error(control, offset,
					 "*** dyn version %d is not supported ***\n",
					 version);

	blr_format(control, "PYXIS_MAP_VERSION1,");
	PRINT_LINE;
	level++;

	while ((c = BLR_BYTE) != PYXIS_MAP_END) {
		indent(control, level);
		if (c >= PYXIS_MAP_FIELD2 && c <= PYXIS_MAP_SUB_FIELD)
			blr_format(control, "PYXIS_MAP_%s, ",
					   map_strings[c - PYXIS_MAP_FIELD2]);
		switch (c) {
		case PYXIS_MAP_MESSAGE:
			PRINT_BYTE;
			for (n = PRINT_WORD; n; --n) {
				PRINT_LINE;
				indent(control, (SSHORT) (level + 1));
				print_blr_dtype(control, TRUE);
			}
			break;

		case PYXIS_MAP_SUB_FORM:
			PRINT_STRING;
			break;

		case PYXIS_MAP_SUB_FIELD:
			PRINT_STRING;

		case PYXIS_MAP_FIELD1:
		case PYXIS_MAP_FIELD2:
			PRINT_STRING;
			PRINT_LINE;
			indent(control, (SSHORT) (level + 1));
			PRINT_WORD;
			if (c != PYXIS_MAP_FIELD1)
				PRINT_WORD;
			break;

		case PYXIS_MAP_TERMINATOR:
		case PYXIS_MAP_TERMINATING_FIELD:
		case PYXIS_MAP_ITEM_INDEX:
			PRINT_WORD;
			break;

		case PYXIS_MAP_OPAQUE:
		case PYXIS_MAP_TRANSPARENT:
		case PYXIS_MAP_TAG:
			break;

		default:
			return error(control, offset, "*** invalid form map ***\n", 0);
		}
		PRINT_LINE;
	}

	blr_format(control, "PYXIS_MAP_END");
	PRINT_LINE;

	return 0;
}
#endif

int PRETTY_print_mblr(
					  SCHAR * blr,
//____________________________________________________________
//  
//		Pretend we're printing MBLR, but print dynamic
//		DDL instead.
//  

					  int (*routine) (), SCHAR * user_arg, SSHORT language)
{

	return PRETTY_print_dyn(blr, routine, user_arg, language);
}

#ifdef PYXIS
int
PRETTY_print_menu(SCHAR * blr,
//____________________________________________________________
//  
//		Pretty print a menu definition.
//  

				  int (*routine) (), SCHAR * user_arg, SSHORT language)
{
	ctl ctl_buffer;
	ctl* control = &ctl_buffer;
	SCHAR c;
	int offset;
	SSHORT version, level;

	offset = level = 0;

	if (!routine) {
		routine = (int (*)()) ib_printf;
		user_arg = "%.4d %s\n";
	}

	control->ctl_routine = reinterpret_cast < pfn_ctl_routine > (routine);
	control->ctl_user_arg = user_arg;
	control->ctl_blr = control->ctl_blr_start = blr;
	control->ctl_ptr = control->ctl_buffer;
	control->ctl_language = language;

	version = BLR_BYTE;

	if (version != PYXIS_MENU_VERSION1)
		return error(control, offset,
					 "*** menu version %d is not supported ***\n",
					 version);

	blr_format(control, "PYXIS_MENU_VERSION1,");
	PRINT_LINE;
	level++;

	while ((c = BLR_BYTE) != PYXIS_MENU_END) {
		indent(control, level);
		if (c >= PYXIS_MENU_LABEL && c <= PYXIS_MENU_VERTICAL)
			blr_format(control, "PYXIS_MENU_%s, ",
					   menu_strings[c - PYXIS_MENU_LABEL]);
		switch (c) {
		case PYXIS_MENU_ENTREE:
			PRINT_BYTE;

		case PYXIS_MENU_LABEL:
			PRINT_STRING;
			break;

		case PYXIS_MENU_HORIZONTAL:
		case PYXIS_MENU_VERTICAL:
		case PYXIS_MENU_OPAQUE:
		case PYXIS_MENU_TRANSPARENT:
			break;

		default:
			return error(control, offset, "*** invalid MENU ***\n", 0);
		}
		PRINT_LINE;
	}

	blr_format(control, "PYXIS_MENU_END");
	PRINT_LINE;

	return 0;
}
#endif

int
PRETTY_print_sdl(SCHAR * blr,
//____________________________________________________________
//  
//		Pretty print slice description language.
//  

				 int (*routine) (), SCHAR * user_arg, SSHORT language)
{
	ctl ctl_buffer;
	ctl* control = &ctl_buffer;
	int offset;
	SSHORT version, level;

	offset = level = 0;

	if (!routine) {
		routine = (int (*)()) ib_printf;
		user_arg = "%.4d %s\n";
	}

	control->ctl_routine = reinterpret_cast < pfn_ctl_routine > (routine);
	control->ctl_user_arg = user_arg;
	control->ctl_blr = control->ctl_blr_start = blr;
	control->ctl_ptr = control->ctl_buffer;
	control->ctl_language = language;

	version = BLR_BYTE;

	if (version != gds_sdl_version1)
		return error(control, offset,
					 "*** sdl version %d is not supported ***\n",
					 version);

	blr_format(control, "gds__sdl_version1, ");
	PRINT_LINE;
	level++;

	while (NEXT_BYTE != (SCHAR) gds_sdl_eoc)
		PRINT_SDL_VERB;

	offset = control->ctl_blr - control->ctl_blr_start;
	blr_format(control, "gds__sdl_eoc");
	PRINT_LINE;

	return 0;
}


//____________________________________________________________
//  
//		Format an utterance.
//  

static int blr_format(CTL control, const char *string, ...)
{
	va_list ptr;

	VA_START(ptr, string);
	vsprintf(control->ctl_ptr, string, ptr);
	while (*control->ctl_ptr)
		control->ctl_ptr++;
	return 0;
}


//____________________________________________________________
//  
//		Put out an error msg and punt.
//  

static int error( CTL control, int offset, TEXT * string, int arg)
{

	PRINT_LINE;
	sprintf(control->ctl_ptr, string, arg);
	ib_fprintf(ib_stderr, control->ctl_ptr);
	ADVANCE_PTR(control->ctl_ptr);
	PRINT_LINE;

	return -1;
}


//____________________________________________________________
//  
//		Indent for pretty printing.
//  

static int indent( CTL control, SSHORT level)
{

	level *= 3;
	while (--level >= 0)
		PUT_BYTE(' ');
	return 0;
}


//____________________________________________________________
//  
//		Print a datatype sequence and return the length of the
//		data described.
//  

static int print_blr_dtype( CTL control, BOOLEAN print_object)
{
	unsigned short dtype;
	SCHAR *string;
	SSHORT length;
//  TMN: FIX FIX Note that offset is not initialized to anything useful
//  for e.g. PRINT_WORD. I assume it's better to initialize it to zero
//  than letting it be random.
//  
	SSHORT offset = 0;

	dtype = BLR_BYTE;

//  Special case blob (261) to keep down the size of the
//  jump table 

	switch (dtype) {
	case blr_short:
		string = "short";
		length = 2;
		break;

	case blr_long:
		string = "long";
		length = 4;
		break;

	case blr_quad:
		string = "quad";
		length = 8;
		break;

// ** Begin date/time/timestamp *
	case blr_sql_date:
		string = "sql_date";
		length = sizeof(ISC_DATE);
		break;

	case blr_sql_time:
		string = "sql_time";
		length = sizeof(ISC_TIME);
		break;

	case blr_timestamp:
		string = "timestamp";
		length = sizeof(ISC_TIMESTAMP);
		break;
// ** End date/time/timestamp *

	case blr_int64:
		string = "int64";
		length = sizeof(ISC_INT64);
		break;

	case blr_float:
		string = "float";
		length = 4;
		break;

	case blr_double:
		string = "double";
		length = 8;
		break;

	case blr_d_float:
		string = "d_float";
		length = 8;
		break;

	case blr_text:
		string = "text";
		break;

	case blr_cstring:
		string = "cstring";
		break;

	case blr_varying:
		string = "varying";
		break;

	case blr_text2:
		string = "text2";
		break;

	case blr_cstring2:
		string = "cstring2";
		break;

	case blr_varying2:
		string = "varying2";
		break;

	case blr_blob_id:
		string = "blob_id";
		length = 8;
		break;

	default:
		error(control, 0, "*** invalid data type ***", 0);
	}

	blr_format(control, "blr_%s, ", string);

	if (!print_object)
		return length;

	switch (dtype) {
	case blr_text:
		length = PRINT_WORD;
		break;

	case blr_varying:
		length = PRINT_WORD + 2;
		break;

	case blr_text2:
		PRINT_WORD;
		length = PRINT_WORD;
		break;

	case blr_varying2:
		PRINT_WORD;
		length = PRINT_WORD + 2;
		break;

	case blr_short:
	case blr_long:
	case blr_int64:
	case blr_quad:
		PRINT_BYTE;
		break;

	case blr_blob_id:
		PRINT_WORD;
		break;

	default:
		if (dtype == blr_cstring)
			length = PRINT_WORD;
		if (dtype == blr_cstring2) {
			PRINT_WORD;
			length = PRINT_WORD;
		}
		break;
	}

	return length;
}



//____________________________________________________________
//  
//		Print a line of pretty-printed BLR.
//  

static int print_blr_line( CTL control, USHORT offset, UCHAR * line)
{
	USHORT comma;
	UCHAR c;

	indent(control, control->ctl_level);
	comma = FALSE;

	while (c = *line++) {
		PUT_BYTE(c);
		if (c == ',')
			comma = TRUE;
		else if (c != ' ')
			comma = FALSE;
	}

	if (!comma)
		PUT_BYTE(',');

	PRINT_LINE;
	return 0;
}


//____________________________________________________________
//  
//		Print a byte as a numeric value and return same.
//  

static int print_byte( CTL control, SSHORT offset)
{
	UCHAR v;

	v = BLR_BYTE;
	sprintf(control->ctl_ptr, (control->ctl_language) ? "chr(%d), " : "%d, ",
			v);
	ADVANCE_PTR(control->ctl_ptr);

	return v;
}


//____________________________________________________________
//  
//		Print a byte as a numeric value and return same.
//  

static int print_char( CTL control, SSHORT offset)
{
	UCHAR c;
	SSHORT printable;

	c = BLR_BYTE;
	printable = (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		(c >= '0' && c <= '9' || c == '$' || c == '_');

	sprintf(control->ctl_ptr, (printable) ?
			"'%c'," : (control->ctl_language) ? "chr(%d)," : "%d,", c);
	ADVANCE_PTR(control->ctl_ptr);

	CHECK_BUFFER;

	return c;
}


//____________________________________________________________
//  
//		Primary recursive routine to print dynamic DDL.
//  

static int print_dyn_verb( CTL control, SSHORT level)
{
	int offset, length, size;
	UCHAR operator_;
	const char *p;

	offset = control->ctl_blr - control->ctl_blr_start;
	operator_ = BLR_BYTE;

	size = FB_NELEM(dyn_table);
	if (operator_ > size || operator_ <= 0 || !(p = dyn_table[operator_]))
		return error(control, offset,
					 "*** dyn operator %d is undefined ***\n",
					 (int) operator_);

	indent(control, level);
	blr_format(control, p);
	PUT_BYTE(',');
	PUT_BYTE(' ');
	++level;

	switch (operator_) {
	case gds_dyn_begin:
	case gds_dyn_mod_database:
		PRINT_LINE;
		while (NEXT_BYTE != gds_dyn_end)
			PRINT_DYN_VERB;
		PRINT_DYN_VERB;
		return 0;

	case gds_dyn_view_blr:
	case gds_dyn_fld_validation_blr:
	case gds_dyn_fld_computed_blr:
	case gds_dyn_trg_blr:
	case gds_dyn_fld_missing_value:
	case gds_dyn_prc_blr:
	case gds_dyn_fld_default_value:
		length = PRINT_WORD;
		PRINT_LINE;
		if (length) {
			control->ctl_level = level;
			gds__print_blr((UCHAR *) control->ctl_blr,
						   (FPTR_VOID) print_blr_line, (SCHAR *) control,
						   control->ctl_language);
			control->ctl_blr += length;
		}
		return 0;

	case gds_dyn_scl_acl:
	case gds_dyn_log_check_point_length:
	case gds_dyn_log_num_of_buffers:
	case gds_dyn_log_buffer_size:
	case gds_dyn_log_group_commit_wait:
	case gds_dyn_idx_inactive:
		length = PRINT_WORD;
		while (length--)
			PRINT_BYTE;
		PRINT_LINE;
		return 0;

	case gds_dyn_view_source:
	case gds_dyn_fld_validation_source:
	case gds_dyn_fld_computed_source:
	case gds_dyn_description:
	case gds_dyn_prc_source:
	case gds_dyn_fld_default_source:
		length = PRINT_WORD;
		while (length--)
			PRINT_CHAR;
		PRINT_LINE;
		return 0;

	case gds_dyn_del_exception:
		if (length = PRINT_WORD)
			do
				PRINT_CHAR;
			while (--length);
		return 0;

	case gds_dyn_fld_not_null:
	case gds_dyn_sql_object:
	case gds_dyn_drop_log:
	case gds_dyn_drop_cache:
	case gds_dyn_def_default_log:
	case gds_dyn_log_file_serial:
	case gds_dyn_log_file_raw:
	case gds_dyn_log_file_overflow:
	case gds_dyn_single_validation:
	case gds_dyn_del_default:
	case gds_dyn_del_validation:
	case gds_dyn_idx_statistic:
	case gds_dyn_foreign_key_delete:
	case gds_dyn_foreign_key_update:
	case gds_dyn_foreign_key_cascade:
	case gds_dyn_foreign_key_default:
	case gds_dyn_foreign_key_null:
	case gds_dyn_foreign_key_none:

		PRINT_LINE;
		return 0;

	case gds_dyn_end:
		PRINT_LINE;
		return 0;
	}

	if (length = PRINT_WORD)
		do
			PRINT_CHAR;
		while (--length);

	PRINT_LINE;

	switch (operator_) {
	case gds_dyn_def_database:
	case gds_dyn_def_dimension:
	case gds_dyn_def_exception:
	case gds_dyn_def_file:
	case gds_dyn_def_log_file:
	case gds_dyn_def_cache_file:
	case gds_dyn_def_filter:
	case gds_dyn_def_function:
	case gds_dyn_def_function_arg:
	case gds_dyn_def_generator:
	case gds_dyn_def_global_fld:
	case gds_dyn_def_idx:
	case gds_dyn_def_local_fld:
	case gds_dyn_def_rel:
	case gds_dyn_def_procedure:
	case gds_dyn_def_parameter:
	case gds_dyn_def_security_class:
	case gds_dyn_def_shadow:
	case gds_dyn_def_sql_fld:
	case gds_dyn_def_trigger:
	case gds_dyn_def_trigger_msg:
	case gds_dyn_def_view:
	case gds_dyn_delete_database:
	case gds_dyn_delete_dimensions:
	case gds_dyn_delete_filter:
	case gds_dyn_delete_function:
	case gds_dyn_delete_global_fld:
	case gds_dyn_delete_idx:
	case gds_dyn_delete_local_fld:
	case gds_dyn_delete_rel:
	case gds_dyn_delete_procedure:
	case gds_dyn_delete_parameter:
	case gds_dyn_delete_security_class:
	case gds_dyn_delete_trigger:
	case gds_dyn_delete_trigger_msg:
	case gds_dyn_delete_shadow:
	case gds_dyn_mod_exception:
	case gds_dyn_mod_global_fld:
	case gds_dyn_mod_idx:
	case gds_dyn_mod_local_fld:
	case gds_dyn_mod_procedure:
	case gds_dyn_mod_rel:
	case gds_dyn_mod_security_class:
	case gds_dyn_mod_trigger:
	case gds_dyn_mod_trigger_msg:
	case gds_dyn_rel_constraint:
	case gds_dyn_mod_view:
	case gds_dyn_grant:
	case gds_dyn_revoke:
	case gds_dyn_view_relation:
		while (NEXT_BYTE != gds_dyn_end)
			PRINT_DYN_VERB;
		PRINT_DYN_VERB;
		return 0;
	}

	return 0;
}


//____________________________________________________________
//  
//		Invoke callback routine to print (or do something with) a line.
//  

static int print_line( CTL control, SSHORT offset)
{

	*control->ctl_ptr = 0;
	(*control->ctl_routine) (control->ctl_user_arg, offset, control->ctl_buffer);
	control->ctl_ptr = control->ctl_buffer;
	return 0;
}


//____________________________________________________________
//  
//		Print a VAX word as a numeric value an return same.
//  

static SLONG print_long( CTL control, SSHORT offset)
{
	UCHAR v1, v2, v3, v4;

	v1 = BLR_BYTE;
	v2 = BLR_BYTE;
	v3 = BLR_BYTE;
	v4 = BLR_BYTE;
	sprintf(control->ctl_ptr,
			(control->
			 ctl_language) ? "chr(%d),chr(%d),chr(%d),chr(%d) " :
			"%d,%d,%d,%d, ", v1, v2, v3, v4);
	ADVANCE_PTR(control->ctl_ptr);

	return v1 | (v2 << 8) | (v3 << 16) | (v4 << 24);
}


//____________________________________________________________
//  
//		Primary recursive routine to print slice description language.
//  

static int print_sdl_verb( CTL control, SSHORT level)
{
	int offset, n;
	const char *p;
	SCHAR operator_;

	offset = control->ctl_blr - control->ctl_blr_start;
	operator_ = BLR_BYTE;

	if (operator_ > FB_NELEM(sdl_table) ||
		operator_ <= 0 || !(p = sdl_table[operator_]))
		return error(control, offset,
					 "*** SDL operator %d is undefined ***\n",
					 (int) operator_);

	indent(control, level);
	blr_format(control, p);
	PUT_BYTE(',');
	PUT_BYTE(' ');
	++level;
	n = 0;

	switch (operator_) {
	case gds_sdl_begin:
		PRINT_LINE;
		while (NEXT_BYTE != gds_sdl_end)
			PRINT_SDL_VERB;
		PRINT_SDL_VERB;
		return 0;

	case gds_sdl_struct:
		n = PRINT_BYTE;
		while (n--) {
			PRINT_LINE;
			indent(control, (SSHORT) (level + 1));
			offset = control->ctl_blr - control->ctl_blr_start;
			print_blr_dtype(control, TRUE);
		}
		break;

	case gds_sdl_scalar:
		PRINT_BYTE;

	case gds_sdl_element:
		n = PRINT_BYTE;
		PRINT_LINE;
		while (n--)
			PRINT_SDL_VERB;
		return 0;

	case gds_sdl_field:
	case gds_sdl_relation:
		PRINT_STRING;
		break;

	case gds_sdl_fid:
	case gds_sdl_rid:
	case gds_sdl_short_integer:
		PRINT_WORD;
		break;

	case gds_sdl_variable:
	case gds_sdl_tiny_integer:
		PRINT_BYTE;
		break;

	case gds_sdl_long_integer:
		PRINT_LONG;
		break;

	case gds_sdl_add:
	case gds_sdl_subtract:
	case gds_sdl_multiply:
	case gds_sdl_divide:
		PRINT_LINE;
		PRINT_SDL_VERB;
		PRINT_SDL_VERB;
		return 0;

	case gds_sdl_negate:
		PRINT_LINE;
		PRINT_SDL_VERB;
		return 0;

	case gds_sdl_do3:
		n++;
	case gds_sdl_do2:
		n++;
	case gds_sdl_do1:
		n += 2;
		PRINT_BYTE;
		PRINT_LINE;
		while (n--)
			PRINT_SDL_VERB;
		return 0;
	}

	PRINT_LINE;

	return 0;
}


//____________________________________________________________
//  
//		Print a byte-counted string.
//  

static int print_string( CTL control, SSHORT offset)
{
	SSHORT n;

	n = PRINT_BYTE;
	while (--n >= 0)
		PRINT_CHAR;

	PUT_BYTE(' ');
	return 0;
}


//____________________________________________________________
//  
//		Print a VAX word as a numeric value an return same.
//  

static int print_word( CTL control, SSHORT offset)
{
	UCHAR v1, v2;

	v1 = BLR_BYTE;
	v2 = BLR_BYTE;
	sprintf(control->ctl_ptr,
			(control->ctl_language) ? "chr(%d),chr(%d), " : "%d,%d, ", v1,
			v2);
	ADVANCE_PTR(control->ctl_ptr);

	return (v2 << 8) | v1;
}
