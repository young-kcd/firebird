/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		dmp.c
 *	DESCRIPTION:	Logical page dumper
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

#include "../jrd/jrd.h"
#include "../jrd/cch.h"
#include "../jrd/pag.h"
#include "../jrd/ods.h"
#include "../jrd/btr.h"
#include "../jrd/time.h"
#include "../jrd/tra.h"
#include "../jrd/cch_proto.h"
#include "../jrd/dmp_proto.h"

void (*dbg_block) ();
void (*dmp_active) () = DMP_active,
	(*dmp_dirty) () = DMP_dirty, (*dmp_page) () = DMP_page;

extern *dbg_file;

static void complement_key(UCHAR *, int);
static double decompress(SCHAR *);
static void dmp_blob(register BLP);
static void dmp_header(register HDR);
static void dmp_pip(register PIP, ULONG);
static void dmp_transactions(register TIP, ULONG);
static void dmp_pointer(register PPG);
static void dmp_data(register DPG);
static void dmp_root(register IRT);
static void dmp_index(BTR);
static void move(SCHAR *, SCHAR *, SSHORT);

static int DMP_descending;

static TEXT *months[] = {
	"Jan", "Feb", "Mar",
	"Apr", "May", "Jun",
	"Jul", "Aug", "Sep",
	"Oct", "Nov", "Dec"
};


#define TRANS_SHIFT(number)	(((number) & TRA_MASK) << 1)
#define TRANS_OFFSET(number)	((number) >> TRA_SHIFT)
#define SHIFTULONG	2

#define BTN_PREFIX(node)	node->btn_prefix
#define BTN_LENGTH(node)	node->btn_length
#define BTN_NUMBER(node)	node->btn_number
#define BTN_DATA(node)		node->btn_data

#define QUAD_MOVE(a,b)	quad_move(a, b)

#define QE(a,b,n)	(((UCHAR*)b)[n] == ((UCHAR*)a)[n])
#define QUAD_EQUAL(a,b)	(QE(a,b,0) && QE(a,b,1) && QE(a,b,2) && QE(a,b,3))
#define NEXT_NODE(xxx)	(BTN) (BTN_DATA (xxx) + BTN_LENGTH (xxx))
#define CTO32L(p) ((((SCHAR*)(p))[0] << 24) | (((UCHAR*)(p))[1] << 16) | (((UCHAR*)(p))[2] << 8) | (((UCHAR*)(p)) [3]))


void DMP_active(void)
{
/**************************************
 *
 *	D M P _ a c t i v e
 *
 **************************************
 *
 * Functional description
 *	Dump all buffers that are active.
 *
 **************************************/
	register BCB bcb;
	register BDB bdb;
	register int i;

	bcb = dbb->dbb_bcb;
	for (i = 0; i < bcb->bcb_count; i++) {
		bdb = bcb->bcb_rpt[i].bcb_bdb;
		if (bdb->bdb_use_count) {
			if (*dbg_block)
				(*dbg_block) (bdb);
			DMP_page(bdb->bdb_page);
		}
	}
}


void DMP_dirty(void)
{
/**************************************
 *
 *	D M P _ d i r t y
 *
 **************************************
 *
 * Functional description
 *	Dump all buffers that are dirty.
 *
 **************************************/
	register BCB bcb;
	register BDB bdb;
	register int i;

	bcb = dbb->dbb_bcb;
	for (i = 0; i < bcb->bcb_count; i++) {
		bdb = bcb->bcb_rpt[i].bcb_bdb;
		if (bdb->bdb_flags & bdb_dirty) {
			if (*dbg_block)
				(*dbg_block) (bdb);
			DMP_page(bdb->bdb_page);
		}
	}
}


void DMP_page( SLONG number)
{
/**************************************
 *
 *	D M P _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Dump a database page.  Actually, just case on type
 *	and disptach.
 *
 **************************************/
	WIN window;
	register PAG page;

	window.win_page = number;
	page = FETCH(&window, LCK_read, 0);
	DMP_fetched_page(page, number, 0);
	RELEASE(&window);
}


void DMP_fetched_page( register PAG page, ULONG number, ULONG sequence)
{
/**************************************
 *
 *	D M P _ f e t c h e d _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Dump a database page.  Actually, just case on type
 *	and disptach.  The sequence number is provided by
 *	the rebuild utility and passed as zero by the standard
 *	dump code.
 *
 **************************************/

	ib_fprintf(dbg_file, "\n%ld\t", number);

	switch (page->pag_type) {
	case pag_header:
		dmp_header(page);
		break;

	case pag_pages:
		dmp_pip(page, sequence);
		break;

	case pag_transactions:
		dmp_transactions(page, sequence);
		break;

	case pag_pointer:
		dmp_pointer(page, sequence);
		break;

	case pag_data:
		dmp_data(page, sequence);
		break;

	case pag_root:
		dmp_root(page);
		break;

	case pag_index:
		dmp_index(page);
		break;

	case pag_blob:
		dmp_blob(page);
		break;

	default:
		ib_fprintf(dbg_file, "*** Page %ld (type %d) is undefined ***",
				   number, page->pag_type);
	}

	ib_fprintf(dbg_file, "\n");
}


static void complement_key( UCHAR * p, int length)
{
/**************************************
 *
 *	c o m p l e m e n t _ k e y
 *
 **************************************
 *
 * Functional description
 *	Negate a key for descending index.
 *
 **************************************/
	UCHAR *end;

	for (end = p + length; p < end; p++)
		*p ^= -1;
}


static double decompress( SCHAR * value)
{
/**************************************
 *
 *	d e c o m p r e s s
 *
 **************************************
 *
 * Functional description
 *	Re-form a double precision number out of a compressed
 *	value string (assume string has been null padded to 8
 *	bytes).
 *
 **************************************/
	double dbl;
	SSHORT l;
	SCHAR *p;

	p = (SCHAR *) & dbl;

	if (*value & (1 << 7)) {
		*p++ = *value++ ^ (1 << 7);
		l = 7;
		do
			*p++ = *value++;
		while (--l);
	}
	else {
		l = 8;
		do
			*p++ = -*value++ - 1;
		while (--l);
	}

	return dbl;
}


static void dmp_blob( register BLP page)
{
/**************************************
 *
 *	d m p _ b l o b
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	USHORT i, n;
	ULONG *ptr;

	ib_fprintf(dbg_file,
			   "BLOB PAGE\n\tFlags: %x, lead page: %d, sequence: %d, length: %d\n\t",
			   ((PAG) page)->pag_flags, page->blp_lead_page,
			   page->blp_sequence, page->blp_length);

	if (((PAG) page)->pag_flags & blp_pointers) {
		n = page->blp_length >> SHIFTULONG;
		for (i = 0, ptr = (ULONG *) page->blp_page; i < n; i++, ptr++)
			ib_fprintf(dbg_file, "%d,", *ptr);
	}

	ib_fprintf(dbg_file, "\n");
}


static void dmp_header( register HDR page)
{
/**************************************
 *
 *	d m p _ h e a d e r
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	struct tm time;
	SCHAR *p, *end;
	SLONG number;

	ib_fprintf(dbg_file,
			   "HEADER PAGE\n\tPage size: %d, version: %d, pages: %ld\n",
			   page->hdr_page_size, page->hdr_ods_version, page->hdr_PAGES);

	gds__decode_date(page->hdr_creation_date, &time);
	ib_fprintf(dbg_file, "\tCreation date:\t%s %d, %d %d:%02d:%02d\n",
			   months[time.tm_mon], time.tm_mday, time.tm_year + 1900,
			   time.tm_hour, time.tm_min, time.tm_sec);




	ib_fprintf(dbg_file,
			   "\tOldest trans %ld, oldest_active %ld, next trans %ld\n",
			   page->hdr_oldest_transaction, page->hdr_oldest_active,
			   page->hdr_next_transaction);

	ib_fprintf(dbg_file,
			   "\tfile sequence # %d, flags %d, attachment %ld\n",
			   page->hdr_sequence, page->hdr_flags, 0);


/*ib_fprintf (dbg_file,
    "\timplementation %ld, shadow count %ld\n",
	0, page->hdr_shadow_count);
*/

	ib_fprintf(dbg_file, "\n    Variable header data:\n");

	for (p = (SCHAR *) page->hdr_data, end = p + page->hdr_page_size;
		 p < end && *p != HDR_end; p += 2 + p[1])
		switch (*p) {
		case HDR_root_file_name:
			ib_printf("\tRoot file name: %*s\n", p[1], p + 2);
			break;

		case HDR_journal_server:
			ib_printf("\tJournal server: %*s\n", p[1], p + 2);
			break;

		case HDR_file:
			ib_printf("\tContinuation file: %*s\n", p[1], p + 2);
			break;

		case HDR_last_page:
			move(p + 2, &number, sizeof(number));
			ib_printf("\tLast logical page: %ld\n", number);
			break;

		case HDR_unlicensed:
			move(p + 2, &number, sizeof(number));
			ib_printf("\tUnlicensed accesses: %ld\n", number);
			break;

/*	case HDR_sweep_interval:
	    move (p + 2, &number, sizeof (number));
	    ib_printf ("\tSweep interval: %ld\n", number);
	    break;

 	case HDR_log_name:
	    ib_printf ("\tLog file name: %*s\n", p[1], p + 2);
	    break;
*/
		default:
			ib_printf("\tUnrecognized option %d, length %d\n", p[0], p[1]);
		}

	ib_printf("\t*END*\n");
}


static void dmp_pip( register PIP page, ULONG sequence)
{
/**************************************
 *
 *	d m p _ p i p
 *
 **************************************
 *
 * Functional description
 *	Print a page inventory page.
 *
 **************************************/
	register PGC control;
	register int n;

	control = dbb->dbb_pcontrol;
	ib_fprintf(dbg_file,
			   "PAGE INVENTORY PAGE\n\tMin page: %ld\n\t", page->pip_min);

#define BIT(n) (page->pip_bits [n >> 3] & (1 << (n & 7)))

	for (n = 0; n < control->pgc_ppp;) {
		while (n < control->pgc_ppp)
			if (BIT(n))
				break;
			else
				n++;
		ib_fprintf(dbg_file, "%d - ", n);
		while (n < control->pgc_ppp)
			if (!BIT(n))
				break;
			else
				n++;
		ib_fprintf(dbg_file, "%d, ", n - 1);
	}

	ib_fprintf(dbg_file, "\n");
}


static void dmp_transactions( register TIP page, ULONG sequence)
{
/**************************************
 *
 *	d m p _ t r a n s a c t i o n 
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	UCHAR *byte, s[101], *p, *end;
	ULONG transactions_per_tip, number, trans_offset;
	USHORT shift, state, hundreds;

	transactions_per_tip = dbb->dbb_pcontrol->pgc_tpt;

	ib_fprintf(dbg_file, "Transaction Inventory Page");
/*
ib_fprintf (dbg_file, "\tCurrent transaction %d", dbb->dbb_transaction->tra_number);
*/
	if (sequence)
		ib_fprintf(dbg_file, "\t first transaction on page %ld",
				   transactions_per_tip * (sequence - 1));
	ib_fprintf(dbg_file, "\tnext TIP page %d\n\n", page->tip_next);
	ib_fprintf(dbg_file,
			   "\t          1         2         3         4         5         6         7         8         9\n");
	ib_fprintf(dbg_file,
			   "\t0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\n\n");
	end = s + sizeof(s) - 1;
	p = s;

	for (hundreds = number = 0; number < transactions_per_tip; number++) {
		trans_offset = TRANS_OFFSET(number);
		byte = page->tip_transactions + trans_offset;
		shift = TRANS_SHIFT(number);
		state = (*byte >> shift) & TRA_MASK;
		*p++ = (state == tra_active) ? 'A' :
			(state == tra_limbo) ? 'L' : (state == tra_dead) ? 'D' : 'C';
		if (p >= end) {
			*p = 0;
			ib_fprintf(dbg_file, "  %3d\t%s\n", hundreds++, s);
			p = s;
		}
	}
	while (p < end)
		*p++ = ' ';
	*p = 0;
	ib_fprintf(dbg_file, "  %3d\t%s\n", hundreds, s);
}


static void dmp_pointer( register PPG page)
{
/**************************************
 *
 *	d m p _ p o i n t e r
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	USHORT i;
	UCHAR *bytes;

	ib_fprintf(dbg_file,
			   "POINTER PAGE\n\tRelation: %d, Flags: %x, Next: %ld, Count: %d\n",
			   page->ppg_relation, ((PAG) page)->pag_flags,
			   page->ppg_next, page->ppg_count);
	ib_fprintf(dbg_file,
			   "\tMin space slot: %d, Max space slot: %d\n",
			   page->ppg_min_space, page->ppg_max_space);

	bytes = (UCHAR *) & page->ppg_page[dbb->dbb_dp_per_pp];

	for (i = 0; i < page->ppg_count; i++) {
		if (i % 20 == 0)
			ib_fprintf(dbg_file, "\n\t%d: ", bytes[i / 4]);
		ib_fprintf(dbg_file, "%ld ", page->ppg_page[i]);
	}

	ib_fprintf(dbg_file, "\n");
}


static void dmp_data( register DPG page)
{
/**************************************
 *
 *	d m p _ d a t a
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	SCHAR c, *p, *q, *end;
	SSHORT i, length, expanded_length;
	RHD header;
	RHDF fragment;
	BLH blob;
	dpg::dpg_repeat * index;

	ib_fprintf(dbg_file,
			   "DATA PAGE\n\tRelation: %d, Sequence: %d, Count: %d, Flags: %x\n",
			   page->dpg_relation, page->dpg_sequence, page->dpg_count,
			   ((PAG) page)->pag_flags);

	for (i = 0, index = page->dpg_rpt; i < page->dpg_count; i++, index++) {
		if (index->dpg_length == 0)
			continue;
		header = (RHD) ((SCHAR *) page + index->dpg_offset);
		fragment = (RHDF) header;
		if (header->rhd_flags & rhd_blob) {
			blob = (BLH) header;
			ib_fprintf(dbg_file,
					   "\n\t%d - (blob) offset: %d, length: %d, flags: %x\n",
					   i, index->dpg_offset, index->dpg_length,
					   header->rhd_flags);
			ib_fprintf(dbg_file,
					   "\tlevel: %d, lead page: %d, length: %d, count %d\n",
					   blob->blh_level, blob->blh_lead_page, blob->blh_length,
					   blob->blh_count);
		}
		else {
			expanded_length = 0;
			if (index->dpg_offset) {
				if (header->rhd_flags & rhd_incomplete) {
					length = index->dpg_length - OFFSETA(RHDF, rhdf_data);
					p = (SCHAR *) ((RHDF) header)->rhdf_data;
				}
				else {
					length = index->dpg_length - OFFSETA(RHD, rhd_data);
					p = (SCHAR *) header->rhd_data;
				}
				for (q = p, end = p + length; q < end;)
					if (*q > 0) {
						expanded_length += *q;
						q += *q + 1;
					}
					else {
						expanded_length -= *q;
						q += 2;
					}
			}
			ib_fprintf(dbg_file,
					   "\n\t%d - offset: %d, length: %d (%d), trans: %d, format: %d, flags: %x\n\t",
					   i, index->dpg_offset, index->dpg_length,
					   expanded_length, header->rhd_transaction,
					   header->rhd_format, header->rhd_flags);
			if (header->rhd_b_page)
				ib_fprintf(dbg_file, "back page: %d, line: %d\n\t",
						   header->rhd_b_page, header->rhd_b_line);
			if (header->rhd_flags & rhd_incomplete)
				ib_fprintf(dbg_file, "frag page: %d, line: %d\n\t",
						   fragment->rhdf_f_page, fragment->rhdf_f_line);
			if (index->dpg_offset) {
				if (length < 0)
					ib_printf(dbg_file, "*** invalid record length ***");
				else if (length)
					do {
						c = *p++;
						ib_fprintf(dbg_file, (c == ' ' ||
											  c >= 'a' && c <= 'z' ||
											  c >= 'A' && c <= 'Z' ||
											  c >= '0' && c <= '9' ||
											  c == '$'
											  || c == '_') ? "%c" : "(%d)",
								   (unsigned) c);
					}
					while (--length);
				ib_fprintf(dbg_file, "\n");
			}
		}
	}
}


static void dmp_root( register IRT page)
{
/**************************************
 *
 *	d m p _ r o o t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	irt::irt_repeat * desc;
	struct irtd *stuff;
	SSHORT i, j;

	ib_fprintf(dbg_file, "ROOT PAGE\n\tRelation: %d, Count: %d\n",
			   page->irt_relation, page->irt_count);
	for (i = 0, desc = page->irt_rpt; i < page->irt_count; i++, desc++) {
		ib_fprintf(dbg_file, "\t%d -- root: %ld, cnt: %d, flags: %x, keys: ",
				   i, desc->irt_root, desc->irt_keys, desc->irt_flags);
		stuff = (struct irtd *) ((SCHAR *) page + desc->irt_desc);
		for (j = 0; j < desc->irt_keys; j++, stuff++)
			ib_fprintf(dbg_file, "(%d, %d),", stuff->irtd_field,
					   stuff->irtd_itype);
		ib_fprintf(dbg_file, "\n");
	}
}


static void dmp_index( BTR page)
{
/**************************************
 *
 *	d m p _ i n d e x
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	BTN node, end;
	UCHAR c, value[256], print[256], *p, *q;
	SSHORT l;
	SLONG number;
	double n;

	ib_fprintf(dbg_file,
			   "B-TREE PAGE\n\tRelation: %d, Sibling: %ld, Level = %d, Length = %d, Flags = %d\n",
			   page->btr_relation, page->btr_sibling, page->btr_level,
			   page->btr_length, ((PAG) page)->pag_flags);

	end = (BTN) ((UCHAR *) page + page->btr_length);

	for (node = (BTN) page->btr_nodes; node < end;) {
		number = CTO32L(BTN_NUMBER(node));

		/* compute running value */

		p = value + BTN_PREFIX(node);
		q = BTN_DATA(node);
		if (l = BTN_LENGTH(node))
			do
				*p++ = *q++;
			while (--l);
		while (p < &value[8])
			*p++ = 0;

		/* format value as number */

		if (DMP_descending)
			complement_key(value, BTN_PREFIX(node) + BTN_LENGTH(node));

		if ((BTN_PREFIX(node) == 0 && BTN_LENGTH(node) == 0) ||
			BTN_PREFIX(node) + BTN_LENGTH(node) > 8)
			n = 0;
		else
			n = decompress(value);

		/* format value as string for printing */

		p = print;
		q = value;
		if (l = BTN_PREFIX(node))
			do {
				c = *q++;
				*p++ = (c >= ' ' && c <= '~') ? c : '.';
			} while (--l);
		*p++ = '|';
		if (l = BTN_LENGTH(node))
			do {
				c = *q++;
				*p++ = (c >= ' ' && c <= '~') ? c : '.';
			} while (--l);
		*p = 0;

		/* print formatted node */

		ib_fprintf(dbg_file,
				   "\t+%x Prefix: %d, length: %d, number: %ld, (%s) [%g]\n",
				   (SCHAR *) node - (SCHAR *) page, BTN_PREFIX(node),
				   BTN_LENGTH(node), number, print, n);

		if (DMP_descending)
			complement_key(value, BTN_PREFIX(node) + BTN_LENGTH(node));

		node = NEXT_NODE(node);
	}
}


static void move( SCHAR * from, SCHAR * to, SSHORT length)
{
/**************************************
 *
 *	m o v e
 *
 **************************************
 *
 * Functional description
 *	Move some stuff.
 *
 **************************************/

	do
		*to++ = *from++;
	while (--length);
}
