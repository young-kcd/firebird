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

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <ctype.h>
#include <string.h>
#include "memory_routines.h"

#include "../jrd/jrd.h"
#include "../jrd/lck.h"
#include "../jrd/ods.h"
#include "../jrd/cch.h"
#include "../jrd/pag.h"
#include "../jrd/val.h"
#include "../jrd/btr.h"
#include "../jrd/jrd_time.h"
#include "../jrd/tra.h"
#include "../jrd/cch_proto.h"
#include "../jrd/dmp_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/sqz_proto.h"
#include "../jrd/thd_proto.h"


extern "C" {


extern int (*dbg_block) ();

void (*dmp_active) (void) = DMP_active,
	(*dmp_dirty) (void) = DMP_dirty, (*dmp_page) (SLONG, USHORT) = DMP_page;

extern IB_FILE *dbg_file;

static void btc_printer(SLONG, BDB, SCHAR *);
static void btc_printer_errors(SLONG, BDB, SCHAR *);
static void complement_key(UCHAR *, int);
static double decompress(SCHAR *);
static void dmp_blob(BLP);
static void dmp_data(DPG);
static void dmp_header(HDR);
static void dmp_index(BTR, USHORT);
static void dmp_pip(PIP, ULONG);
static void dmp_pointer(PPG);
static void dmp_root(IRT);
static void dmp_transactions(TIP, ULONG);

static int dmp_descending = 0;

static const SCHAR months[][4] = {
	"Jan", "Feb", "Mar",
	"Apr", "May", "Jun",
	"Jul", "Aug", "Sep",
	"Oct", "Nov", "Dec"
};


#define TRANS_SHIFT(number)	(((number) & TRA_MASK) << 1)
#define TRANS_OFFSET(number)	((number) >> TRA_SHIFT)


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
	DBB dbb;
	BCB bcb;
	BDB bdb;
	USHORT i;

	dbb = GET_DBB;

	bcb = dbb->dbb_bcb;
	for (i = 0; i < bcb->bcb_count; i++)
	{
		bdb = bcb->bcb_rpt[i].bcb_bdb;
		if (bdb->bdb_use_count)
		{
			if (*dbg_block != NULL)
			{
				reinterpret_cast<void (*)(BDB)>(*dbg_block)(bdb);
			}
			DMP_page(bdb->bdb_page, 0);
		}
	}
}


void DMP_btc(void)
{
/**************************************
 *
 *	D M P _ b t c
 *
 **************************************
 *
 * Functional description
 *	Dump the dirty page b-tree.
 *
 **************************************/
	DBB dbb;
	BDB bdb;
	SLONG level;
	SCHAR buffer[250];

	dbb = GET_DBB;

	level = 0;
	bdb = dbb->dbb_bcb->bcb_btree;

	memset(buffer, ' ', sizeof(buffer));
	buffer[249] = 0;

	if (bdb)
		btc_printer(level, bdb, buffer);
	ib_fprintf(dbg_file, "%s\n", buffer);
}


void DMP_btc_errors(void)
{
/**************************************
 *
 *	D M P _ b t c _ e r r o r s
 *
 **************************************
 *
 * Functional description
 *	Dump the dirty page b-tree.
 *
 **************************************/
	DBB dbb;
	BDB bdb;
	SLONG level;
	SCHAR buffer[250];

	dbb = GET_DBB;

	level = 0;
	bdb = dbb->dbb_bcb->bcb_btree;
	if (bdb)
		btc_printer_errors(level, bdb, buffer);
}


void DMP_btc_ordered(void)
{
/**************************************
 *
 *	D M P _ b t c _ o r d e r e d
 *
 **************************************
 *
 * Functional description
 *	Dump the dirty page b-tree.
 *
 **************************************/
	DBB dbb;
	BDB bdb, next;
	int i;
	SLONG max_seen;

	dbb = GET_DBB;

/* Pick starting place at leftmost node */

	ib_fprintf(dbg_file,
			   "\nDirty Page Binary Tree -- Page (Transaction) { Dirty | Clean }\n");

	max_seen = -3;

	for (next = dbb->dbb_bcb->bcb_btree; next && next->bdb_left;
		 next = next->bdb_left);

	i = 0;

	while (bdb = next) {
		if (!bdb->bdb_parent && bdb != dbb->dbb_bcb->bcb_btree) {
			for (bdb = dbb->dbb_bcb->bcb_btree; bdb;)
				if (bdb->bdb_left && max_seen < bdb->bdb_page)
					bdb = bdb->bdb_left;
				else if (bdb->bdb_right && max_seen > bdb->bdb_page)
					bdb = bdb->bdb_right;
				else
					break;
			if (!bdb)
				break;
		}

		/* Decide where to go next.  The options are (right, then down to the left)
		   or up */
		i++;
		if (bdb->bdb_right && max_seen < bdb->bdb_right->bdb_page)
			for (next = bdb->bdb_right; next->bdb_left;
				 next = next->bdb_left);
		else
			next = bdb->bdb_parent;

		if (max_seen >= bdb->bdb_page)
			continue;

		max_seen = bdb->bdb_page;

		ib_fprintf(dbg_file, "\t%ld (%ld) %s%s", bdb->bdb_page,
				   bdb->bdb_transactions,
				   (bdb->bdb_flags & BDB_dirty) ? "D" : "C",
				   (i % 10) ? "," : "\n");

	}
	ib_fprintf(dbg_file, "\n");
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
	DBB dbb;
	BCB bcb;
	BDB bdb;
	USHORT i;

	dbb = GET_DBB;

	bcb = dbb->dbb_bcb;
	for (i = 0; i < bcb->bcb_count; i++)
	{
		bdb = bcb->bcb_rpt[i].bcb_bdb;
		if (bdb->bdb_flags & BDB_dirty)
		{
			if (*dbg_block != NULL)
			{
				reinterpret_cast<void (*)(BDB)>(*dbg_block)(bdb);
			}
			DMP_page(bdb->bdb_page, 0);
		}
	}
}


void DMP_fetched_page(PAG page,
					  ULONG		number,
					  ULONG		sequence,
					  USHORT page_size)
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

	switch (page->pag_type)
	{
	case pag_header:
		dmp_header((HDR) page);
		break;

	case pag_pages:
		dmp_pip((PIP) page, sequence);
		break;

	case pag_transactions:
		dmp_transactions((TIP) page, sequence);
		break;

	case pag_pointer:
		dmp_pointer((PPG) page);
		break;

	case pag_data:
		dmp_data((DPG) page);
		break;

	case pag_root:
		dmp_root((IRT) page);
		break;

	case pag_index:
		dmp_index((BTR) page, page_size);
		break;

	case pag_blob:
		dmp_blob((BLP) page);
		break;

	case pag_ids:
		ib_fprintf(dbg_file, "GEN-IDS PAGE\n");
		break;

	case pag_log:
		ib_fprintf(dbg_file, "WRITE-AHEAD LOG INFO PAGE\n");
		break;

	default:
		ib_fprintf(dbg_file, "*** Page %ld (type %d) is undefined ***",
				   number, page->pag_type);
	}

	ib_fprintf(dbg_file, "\n");
}


void DMP_page(SLONG number, USHORT page_size)
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
	PAG page;

	window.win_page = number;
	window.win_flags = 0;
	page = CCH_FETCH(NULL, &window, LCK_read, 0);
	DMP_fetched_page(page, number, 0, page_size);
	CCH_RELEASE(NULL, &window);
}


static void btc_printer(SLONG level, BDB bdb, SCHAR * buffer)
{
/**************************************
 *
 * 	b t c _ p r i n t e r
 *
 **************************************
 *
 * Functional description
 *	Dump the dirty page b-tree recursively
 *
 **************************************/

	if (level >= 48) {
		ib_fprintf(dbg_file, "overflow\n");
		return;
	}

	sprintf((buffer + 5 * level + 1), "%.4ld", bdb->bdb_page);

	if (bdb->bdb_left) {
		buffer[5 * (level + 1)] = 'L';
		btc_printer(level + 1, bdb->bdb_left, buffer);
	}
	else
		ib_fprintf(dbg_file, "%s\n", buffer);

	memset(buffer, ' ', 250);
	buffer[249] = 0;

	if (bdb->bdb_right) {
		buffer[5 * (level + 1)] = 'R';
		btc_printer(level + 1, bdb->bdb_right, buffer);
	}
}


static void btc_printer_errors(SLONG level, BDB bdb, SCHAR * buffer)
{
/**************************************
 *
 * 	b t c _ p r i n t e r _ e r r o r s
 *
 **************************************
 *
 * Functional description
 *	Dump the dirty page b-tree recursively
 *
 **************************************/

	if (((bdb->bdb_left) && (bdb->bdb_left->bdb_page > bdb->bdb_page)) ||
		((bdb->bdb_right) && (bdb->bdb_right->bdb_page < bdb->bdb_page)))
		ib_fprintf(dbg_file, "Whoops! Parent %ld, Left %ld, Right %ld\n",
				   bdb->bdb_page,
				   (bdb->bdb_left) ? bdb->bdb_left->bdb_page : 0,
				   (bdb->bdb_right) ? bdb->bdb_right->bdb_page : 0);

	if (bdb->bdb_left)
		btc_printer_errors(level + 1, bdb->bdb_left, buffer);
	if (bdb->bdb_right) {
		btc_printer_errors(level + 1, bdb->bdb_right, buffer);
	}
}


static void complement_key(UCHAR * p, int length)
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


static double decompress(SCHAR * value)
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
		*p++ = static_cast < SCHAR > (*value++ ^ (1 << 7));
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


static void dmp_blob(BLP page)
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
			   "BLOB PAGE\t checksum %d\t generation %ld\n\tFlags: %x, lead page: %d, sequence: %d, length: %d\n\t",
			   ((PAG) page)->pag_checksum, ((PAG) page)->pag_generation,
			   ((PAG) page)->pag_flags, page->blp_lead_page,
			   page->blp_sequence, page->blp_length);

	if (((PAG) page)->pag_flags & blp_pointers) {
		n = page->blp_length >> SHIFTLONG;
		for (i = 0, ptr = (ULONG *) page->blp_page; i < n; i++, ptr++)
			ib_fprintf(dbg_file, "%d,", *ptr);
	}

	ib_fprintf(dbg_file, "\n");
}


static void dmp_data(DPG page)
{
/**************************************
 *
 *	d m p _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Dump a data page in a semi-readable format.
 *
 **************************************/
	SCHAR*	p;
	SCHAR*	q;
	SCHAR*	end;
	SSHORT	length;
	SSHORT	expanded_length;
	USHORT	i;
	RHD		header;
	RHDF	fragment;
	BLH		blob;
	dpg::dpg_repeat* index;
	SCHAR	buffer[8096 + 1];

	ib_fprintf(dbg_file,
			   "DATA PAGE\t checksum %d\t generation %ld\n\tRelation: %d, Sequence: %d, Count: %d, Flags: %x\n",
			   ((PAG) page)->pag_checksum,
			   ((PAG) page)->pag_generation,
			   page->dpg_relation,
			   page->dpg_sequence,
			   page->dpg_count,
			   ((PAG) page)->pag_flags);

	for (i = 0, index = page->dpg_rpt; i < page->dpg_count; i++, index++)
	{
		if (index->dpg_length == 0)
		{
			ib_fprintf(dbg_file, "\n\t%d - (empty)\n", i);
			continue;
		}
		header = (RHD) ((SCHAR *) page + index->dpg_offset);
		fragment = (RHDF) header;
		if (header->rhd_flags & rhd_blob)
		{
			blob = (BLH) header;
			ib_fprintf(dbg_file,
					   "\n\t%d - (blob) offset: %d, length: %d, flags: %x\n",
					   i, index->dpg_offset, index->dpg_length,
					   header->rhd_flags);
			ib_fprintf(dbg_file,
					   "\tlevel: %d, lead page: %d, length: %d, count %d\n",
					   blob->blh_level, blob->blh_lead_page, blob->blh_length,
					   blob->blh_count);
			ib_fprintf(dbg_file,
					   "\tmaxseq: %d, maxseg: %d, flags: %X, sub_type %d\n",
					   blob->blh_max_sequence, blob->blh_max_segment,
					   blob->blh_flags, blob->blh_sub_type);
		}
		else
		{
			expanded_length = 0;
			if (index->dpg_offset)
			{
				if (header->rhd_flags & rhd_incomplete)
				{
					length = index->dpg_length - OFFSETA(RHDF, rhdf_data);
					p = (SCHAR *) ((RHDF) header)->rhdf_data;
				}
				else
				{
					length = index->dpg_length - OFFSETA(RHD, rhd_data);
					p = (SCHAR *) header->rhd_data;
				}
				for (q = p, end = p + length; q < end;)
				{
					if (*q > 0) {
						expanded_length += *q;
						q += *q + 1;
					}
					else {
						expanded_length += -(*q);
						q += 2;
					}
				}
			}
			ib_fprintf(dbg_file,
					   "\n\t%d - offset: %d, length: %d, expanded data length: %d\n\t",
					   i, index->dpg_offset, index->dpg_length,
					   expanded_length);
			ib_fprintf(dbg_file, "trans: %d, format: %d, flags: %#x\n\t",
					   header->rhd_transaction, header->rhd_format,
					   header->rhd_flags);
			if (header->rhd_b_page)
			{
				ib_fprintf(dbg_file, "back page: %d, line: %d\n\t",
						   header->rhd_b_page, header->rhd_b_line);
			}
			if (header->rhd_flags & rhd_incomplete)
			{
				ib_fprintf(dbg_file, "frag page: %d, line: %d\n\t",
						   fragment->rhdf_f_page, fragment->rhdf_f_line);
			}
			if (index->dpg_offset)
			{
				if (length < 0)
				{
					ib_fprintf(dbg_file, "*** invalid record length ***");
				}
				else if (length)
				{
					SCHAR *p_save = p;
					int length_save = length;
					int compress_value = 0;
					int cnt;
					cnt = 0;
					ib_fprintf(dbg_file,
							   "Raw Compressed format: (length %d)\n\t",
							   length);
					do {
						if (cnt++ >= 16) {
							ib_fprintf(dbg_file, "\n\t");
							cnt = 1;
						}

						ib_fprintf(dbg_file, "%3d ", *p++);
					} while (--length);

					ib_fprintf(dbg_file, "\n\t");

					buffer[0] = 0;
					end = SQZ_decompress(p_save, length_save, &buffer[1],
										 &buffer[sizeof(buffer)]);
					cnt = 0;
					p = &buffer[1];
					ib_fprintf(dbg_file,
							   "Decompressed format: (length %d)\n\t",
							   end - p);
					do {
						if (cnt++ >= 20) {
							ib_fprintf(dbg_file, "\n\t");
							cnt = 1;
						}

						if (isprint(*p) &&
							(isprint(*(p + 1)) || isprint(*(p - 1))))
						{
							ib_fprintf(dbg_file, "%2c ", *p++);
						}
						else
						{
							ib_fprintf(dbg_file, "%02x ", (UCHAR) *p++);
						}
					} while (p < end);

					ib_fprintf(dbg_file, "\n");
				}
			}
		}
	}
}


static void dmp_header(HDR page)
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
	USHORT minor_version;

	minor_version = page->hdr_ods_minor;

	ib_fprintf(dbg_file,
			   "HEADER PAGE\t checksum %d\t generation %ld\n\tPage size: %d, version: %d.%d(%d), pages: %ld\n",
			   ((PAG) page)->pag_checksum, ((PAG) page)->pag_generation,
			   page->hdr_page_size, page->hdr_ods_version, minor_version,
			   page->hdr_ods_minor_original, page->hdr_PAGES);

	isc_decode_timestamp((GDS_TIMESTAMP *) page->hdr_creation_date, &time);
	ib_fprintf(dbg_file, "\tCreation date:\t%s %d, %d %d:%02d:%02d\n",
			   months[time.tm_mon], time.tm_mday, time.tm_year + 1900,
			   time.tm_hour, time.tm_min, time.tm_sec);

	ib_fprintf(dbg_file,
			   "\tOldest trans %ld, oldest_active %ld, oldest_snapshot %ld, next trans %ld, bumped trans %ld\n",
			   page->hdr_oldest_transaction, page->hdr_oldest_active,
			   page->hdr_oldest_snapshot, page->hdr_next_transaction,
			   page->hdr_bumped_transaction);

	ib_fprintf(dbg_file,
			   "\tfile sequence # %d, flags %d, attachment %ld\n",
			   page->hdr_sequence, page->hdr_flags, page->hdr_attachment_id);


	ib_fprintf(dbg_file,
			   "\timplementation %ld, shadow count %ld\n",
			   page->hdr_implementation, page->hdr_shadow_count);


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
			memcpy(&number, p + 2, sizeof(number));
			ib_printf("\tLast logical page: %ld\n", number);
			break;

		case HDR_unlicensed:
			memcpy(&number, p + 2, sizeof(number));
			ib_printf("\tUnlicensed accesses: %ld\n", number);
			break;

		case HDR_sweep_interval:
			memcpy(&number, p + 2, sizeof(number));
			ib_printf("\tSweep interval: %ld\n", number);
			break;

		case HDR_log_name:
			ib_printf("\tLog file name: %*s\n", p[1], p + 2);
			break;

		case HDR_journal_file:
			ib_printf("\tJournal file: %*s\n", p[1], p + 2);
			break;

		case HDR_password_file_key:
			ib_printf("\tPassword file key: (can't print)\n");
			break;

		case HDR_backup_info:
			ib_printf("\tBackup info: (can't print)\n");
			break;

		case HDR_cache_file:
			ib_printf("\tShared cache file: %*s\n", p[1], p + 2);
			break;

		default:
			ib_printf("\tUnrecognized option %d, length %d\n", p[0], p[1]);
		}

	ib_printf("\t*END*\n");
}


static void dmp_index(BTR page, USHORT page_size)
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
	UCHAR	c;
	UCHAR	value[256];
	UCHAR	print[256];
	UCHAR*	p;
	UCHAR*	q;
	SSHORT	l;
	double	n;

	ib_fprintf(dbg_file,
			   "B-TREE PAGE\t checksum %d\t generation %ld\n"
			   "\tRelation: %d, Sibling: %ld, Backward Sibling: %ld, Level = %d, Length = %d, Flags = %d\n",
			   ((PAG) page)->pag_checksum,
			   ((PAG) page)->pag_generation,
			   page->btr_relation,
			   page->btr_sibling,
			   page->btr_left_sibling,
			   page->btr_level,
			   page->btr_length,
			   ((PAG) page)->pag_flags);
/* Compute the number of data pages per pointer page.  Each data page
   requires a 32 bit pointer and a 2 bit control field. */
	const USHORT dp_per_pp =
			(USHORT)((ULONG) ((page_size - OFFSETA(PPG, ppg_page)) * 8) /
						(BITS_PER_LONG + 2));
	const USHORT max_records = (page_size - sizeof(struct dpg)) /
								(sizeof(dpg::dpg_repeat) + OFFSETA(RHD, rhd_data));

	BTN end  = (BTN) ((UCHAR *) page + page->btr_length);
	BTN node = (BTN) page->btr_nodes;

	while (node < end)
	{
		const ULONG number = get_long(BTN_NUMBER(node));

		/* compute running value */

		p = value + BTN_PREFIX(node);
		q = BTN_DATA(node);
		l = BTN_LENGTH(node);
		if (l)
		{
			do {
				*p++ = *q++;
			} while (--l);
		}
		while (p < &value[8])
		{
			*p++ = 0;
		}

		/* format value as number */

		if (dmp_descending || (page->btr_header.pag_flags & btr_descending))
			complement_key(value, BTN_PREFIX(node) + BTN_LENGTH(node));

		if ((BTN_PREFIX(node) == 0 && BTN_LENGTH(node) == 0) ||
			BTN_PREFIX(node) + BTN_LENGTH(node) > 8)
		{
			n = 0;
		}
		else
		{
			n = decompress((SCHAR *) value);
		}

		/* format value as string for printing */

		p = print;
		q = value;
		if (l = BTN_PREFIX(node))
		{
			do {
				c = *q++;
				*p++ = (c >= ' ' && c <= '~') ? c : '.';
			} while (--l);
		}
		*p++ = '|';
		if (l = BTN_LENGTH(node))
		{
			do {
				c = *q++;
				*p++ = (c >= ' ' && c <= '~') ? c : '.';
			} while (--l);
		}
		*p = 0;

		/* print formatted node */

		ib_fprintf(dbg_file, "\t+%x Prefix: %d, length: %d, ",
				   (SCHAR *) node - (SCHAR *) page, BTN_PREFIX(node),
				   BTN_LENGTH(node));
		if (page->btr_level)
			ib_fprintf(dbg_file, "page number: %ld", number);
		else
			ib_fprintf(dbg_file, "number: %ld", number);
		if (page_size && !page->btr_level) {
			int pp, slot, line;
			line = number % max_records;
			slot = (number / max_records) % dp_per_pp;
			pp = (number / max_records) / dp_per_pp;
			ib_fprintf(dbg_file, " (pp=%d,slot=%d,line=%d)", pp, slot, line);
		}
		ib_fprintf(dbg_file, ",\t(%s) [%g]\n", print, n);

		if (dmp_descending || (page->btr_header.pag_flags & btr_descending))
			complement_key(value, BTN_PREFIX(node) + BTN_LENGTH(node));

		node = NEXT_NODE(node);
	}
}


static void dmp_pip(PIP page, ULONG sequence)
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
	DBB dbb;
	PGC control;
	int n;

	dbb = GET_DBB;

	control = dbb->dbb_pcontrol;
	ib_fprintf(dbg_file,
			   "PAGE INVENTORY PAGE\t checksum %d\t generation %ld\n\tMin page: %ld\n\tFree pages:\n\t",
			   ((PAG) page)->pag_checksum, ((PAG) page)->pag_generation,
			   page->pip_min);

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


static void dmp_pointer(PPG page)
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
	DBB dbb;
	USHORT i;
	UCHAR *bytes;

	dbb = GET_DBB;

	ib_fprintf(dbg_file,
			   "POINTER PAGE\t checksum %d\t generation %ld\n\tRelation: %d, Flags: %x, Sequence: %ld, Next: %ld, Count: %d\n",
			   ((PAG) page)->pag_checksum, ((PAG) page)->pag_generation,
			   page->ppg_relation, ((PAG) page)->pag_flags,
			   page->ppg_sequence, page->ppg_next, page->ppg_count);
	ib_fprintf(dbg_file, "\tMin space slot: %d, Max space slot: %d\n",
			   page->ppg_min_space, page->ppg_max_space);

	bytes = (UCHAR *) & page->ppg_page[dbb->dbb_dp_per_pp];

	for (i = 0; i < page->ppg_count; i++) {
		if (i % 20 == 0)
			/* ib_fprintf (dbg_file, "\n\t%d: ", bytes [i / 4]); */
			ib_fprintf(dbg_file, "\n\t");
		ib_fprintf(dbg_file, "%ld ", page->ppg_page[i]);
	}

	ib_fprintf(dbg_file, "\n");
}


static void dmp_root(IRT page)
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
	USHORT i, j;

	ib_fprintf(dbg_file,
			   "INDEX ROOT PAGE\t checksum %d\t generation %ld\n\tRelation: %d, Count: %d\n",
			   ((PAG) page)->pag_checksum, ((PAG) page)->pag_generation,
			   page->irt_relation, page->irt_count);
	for (i = 0, desc = page->irt_rpt; i < page->irt_count; i++, desc++) {
		ib_fprintf(dbg_file,
				   "\t%d -- root: %ld, number of keys: %d, flags: %x\n", i,
				   desc->irt_root, desc->irt_keys, desc->irt_flags);
		ib_fprintf(dbg_file, "\t     keys (field, type): ");
		stuff = (struct irtd *) ((SCHAR *) page + desc->irt_desc);
		for (j = 0; j < desc->irt_keys; j++, stuff++)
			ib_fprintf(dbg_file, "(%d, %d),", stuff->irtd_field,
					   stuff->irtd_itype);
		ib_fprintf(dbg_file, "\n");
	}
}


static void dmp_transactions(TIP page, ULONG sequence)
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
	TDBB tdbb;
	DBB dbb;
	UCHAR *byte, s[101], *p, *end;
	ULONG transactions_per_tip, number, trans_offset;
	USHORT shift, state, hundreds;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;

	transactions_per_tip = dbb->dbb_pcontrol->pgc_tpt;

	ib_fprintf(dbg_file,
			   "Transaction Inventory Page\t checksum %d\t generation %ld\n",
			   ((PAG) page)->pag_checksum, ((PAG) page)->pag_generation);
	if (tdbb->tdbb_transaction)
		ib_fprintf(dbg_file, "\tCurrent transaction %d",
				   tdbb->tdbb_transaction->tra_number);
	else
		ib_fprintf(dbg_file, "\tCurrent transaction (NULL)");
	if (sequence)
		ib_fprintf(dbg_file, "\t first transaction on page %ld",
				   transactions_per_tip * (sequence - 1));
	else
		ib_fprintf(dbg_file, "\t Transactions per TIP %ld",
				   transactions_per_tip);
	ib_fprintf(dbg_file, "\tnext TIP page %d\n\n", page->tip_next);
	ib_fprintf(dbg_file,
			   "\t          1         2         3         4         5         6         7         8         9\n");
	ib_fprintf(dbg_file,
			   "\t0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\n\n");
	end = s + sizeof(s) - 1;
	p = s;

	for (hundreds = number = 0; number < transactions_per_tip; number++)
	{
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


} // extern "C"
