/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		rec.c
 *	DESCRIPTION:	
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
#include <string.h>
#include "../jrd/gds.h"
#include "../jrd/jrd.h"
#include "../jrd/ods.h"
#include "../jrd/btr.h"
#include "../jrd/jrn.h"
#include "../jrd/sbm.h"
#include "../jrd/lck.h"
#include "../jrd/cch.h"
#include "../wal/wal.h"
#include "../jrd/rse.h"
#include "../jrd/all_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/dpm_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/misc_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/pag_proto.h"
#include "../jrd/rec_proto.h"
#include "../jrd/sbm_proto.h"
#include "../jrd/thd_proto.h"
#include "../wal/walr_proto.h"

#define MOVE_BYTE(x_from, x_to)	*x_to++ = *x_from++

#define HIGH_WATER(x)		((SSHORT) sizeof (dpg) + (SSHORT) sizeof (dpg::dpg_repeat) * (x - 1))

static void apply_data(DPG, JRND *);
static void apply_header(HDR, JRND *);
static void apply_ids(PPG, JRND *);
static void apply_index(BTR, JRND *);
static void apply_log(LIP, JRND *);
static void apply_pip(PIP, JRND *);
static void apply_pointer(PPG, JRND *);
static void apply_root(IRT, JRND *);
static void apply_transaction(TIP, JRND *);
static void disable(void);
static JRNP *next_clump(JRND *, JRNP *);
static void process_page(TDBB, JRND *, SLONG, SLONG, PAG, SBM *, USHORT);
static void quad_move(UCHAR *, UCHAR *);
static void rec_process_record(TDBB, JRNH *, USHORT, ULONG,
							   ULONG, PAG, SBM *, USHORT);
static void scan_and_apply_logs(TEXT *, TEXT *, CP *, SBM *, USHORT, SLONG *,
								SLONG, PAG);


void REC_recover(
				 TEXT * dbname,
				 TEXT * walname,
				 CP * cp1, SBM * sbm_rec, USHORT activate_shadow)
{
/**************************************
 *
 *	R E C _ r e c o v e r
 *
 **************************************
 *
 * Functional description
 *	Do short term recovery.
 *	Fixup log header at end.
 *
 **************************************/

	gds__log("Short term recovery for database \"%s\". Start log file \"%s\"",
			 dbname, walname);

	scan_and_apply_logs(dbname, walname, cp1, sbm_rec,
						activate_shadow, NULL, -1, NULL);
	REC_update_next_transid();

/* Should do the equivalent of 2 control point flush to write
 * all pages to disk, so that the effects of recovery are not
 * lost.

 * The control points should not be done for 2 cases -
 * - if shadows are present, doing a control point will "recover" the
 *   database.  So a crash before the shadows are recovered will leave
 *   the shadows out of sync.
 * - if there is no shared cache, there will be no one to complete
 *   the control point.

 * The control points are called in CCH_recover_shadow () after the
 * shadows are recovered.
 */
}


void REC_recover_page(
					  TEXT * dbname,
					  TEXT * walname, CP * cp1, SLONG page_no, PAG page)
{
/**************************************
 *
 *	R E C _ r e c o v e r _ p a g e
 *
 **************************************
 *
 * Functional description
 *	recover a single page from a point in time to end of log.
 *
 **************************************/
	SLONG date[2];

/* get current time stamp */

	MOV_time_stamp(reinterpret_cast < ISC_TIMESTAMP * >(date));

	scan_and_apply_logs(dbname, walname, cp1, NULL,
						0, date, page_no, page);
}


void REC_update_next_transid(void)
{
/**************************************
 *
 *	R E C _ u p d a t e _ n e x t _ t r a n s i d
 *
 **************************************
 *
 * Functional description
 *	Used to set the next transaction id from the
 *      bumped transaction id.
 *
 **************************************/
	WIN window;
	HDR hdr;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;

	window.win_page = HEADER_PAGE;
	window.win_flags = 0;
	hdr = (HDR) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
	CCH_MARK_MUST_WRITE(tdbb, &window);
	hdr->hdr_next_transaction = hdr->hdr_bumped_transaction;
	CCH_RELEASE(tdbb, &window);

}


static void apply_data(DPG page, JRND * record)
{
/**************************************
 *
 *	a p p l y _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Apply incremental changes to a data page.
 *
 **************************************/
	DBB dbb;
	JRNP temp, *clump;
	SSHORT space, l, top, used;
	UCHAR *p, *q;
	dpg::dpg_repeat * index, *end;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* Process clumps */

	for (clump = NULL; (clump = next_clump(record, clump));) {
		MOVE_FAST((SCHAR *) clump, (SCHAR *) & temp, JRNP_SIZE);

		if (temp.jrnp_type != JRNP_DATA_SEGMENT)
			BUGCHECK(270);		/* msg 270 error in recovery! wrong data page record */

		/* Handle segment deletion */

		if (!temp.jrnp_length) {
			index = page->dpg_rpt + temp.jrnp_index;
			index->dpg_offset = 0;
			index->dpg_length = 0;
		}

		/* Re-compute page high water mark */

		index = page->dpg_rpt;
		end = index + page->dpg_count;
		page->dpg_count = 0;
		space = dbb->dbb_page_size;

		for (l = 1, used = 0; index < end; index++, l++)
			if (index->dpg_length) {
				page->dpg_count = l;
				space = MIN((int) space, (int) index->dpg_offset);
				used += ROUNDUP(index->dpg_length, ODS_ALIGNMENT);
			}

		/* delete record */

		if (!temp.jrnp_length)
			continue;

		/* Handle segment addition */

		index = page->dpg_rpt + temp.jrnp_index;
		q = clump->jrnp_data;
		l = temp.jrnp_length;

		/* If space exists, just all record */

		if (((SCHAR *) index < (SCHAR *) end)
			&& ((USHORT) l <= index->dpg_length)) {
			index->dpg_length = l;
			p = (UCHAR *) page + index->dpg_offset;
			do
				*p++ = *q++;
			while (--l);
			continue;
		}

		page->dpg_count =
			MAX((int) page->dpg_count, (int) (temp.jrnp_index + 1));
		top = HIGH_WATER(page->dpg_count);
		l = ROUNDUP(l, ODS_ALIGNMENT);
		space -= l;

		/* Compress the page and make space */

		if (space < top) {
			index->dpg_length = 0;
			space = DPM_compress(tdbb, page);
			space -= l;
			if (space < top)
				BUGCHECK(271);	/* msg 271 error in recovery! no space on data page */
		}

		if ((space + l) > dbb->dbb_page_size)
			BUGCHECK(269);		/* msg 269 error in recovery! database corrupted */

		index->dpg_offset = space;
		index->dpg_length = temp.jrnp_length;
		p = (UCHAR *) page + space;
		do
			*p++ = *q++;
		while (--l);
	}
}


static void apply_header(HDR page, JRND * record)
{
/**************************************
 *
 *	a p p l y _ h e a d e r
 *
 **************************************
 *
 * Functional description
 *	Apply changes to database header page 
 *
 **************************************/
	JRNDH temp, *clump;
	JRNDA temp1;

	for (clump = NULL;
		 (clump =
		 (JRNDH *) next_clump(record, reinterpret_cast < jrnp * >(clump)));) {
		MOVE_FAST((SCHAR *) clump, (SCHAR *) & temp, JRNDH_SIZE);

		if (temp.jrndh_type == JRNP_DB_HEADER) {
			page->hdr_bumped_transaction = temp.jrndh_nti;
			page->hdr_oldest_transaction = temp.jrndh_oit;
			page->hdr_oldest_active = temp.jrndh_oat;
		}
		else {
			MOVE_FAST((SCHAR *) clump, (SCHAR *) & temp1, JRNDA_SIZE);

			switch (temp1.jrnda_type) {
			case JRNP_DB_ATTACHMENT:
				page->hdr_attachment_id = temp1.jrnda_data;
				break;

			case JRNP_DB_HDR_PAGES:
				page->hdr_PAGES = temp1.jrnda_data;
				break;

			case JRNP_DB_HDR_FLAGS:
				page->hdr_flags = (USHORT) temp1.jrnda_data;
				break;

			case JRNP_DB_HDR_SDW_COUNT:
				page->hdr_shadow_count = temp1.jrnda_data;
				break;

			default:
				BUGCHECK(272);	/* msg 272 error in recovery! wrong header page record */
			}
		}
	}
}


static void apply_ids(PPG page, JRND * record)
{
/**************************************
 *
 *	a p p l y _ i d s
 *
 **************************************
 *
 * Functional description
 *	Apply changes to generator page (gen ids) page
 *
 **************************************/
	JRNG temp, *clump;
	SLONG *ptr;

	for (clump = NULL;
		 (clump =
		 (JRNG *) next_clump(record, reinterpret_cast < jrnp * >(clump)));) {
		MOVE_FAST((SCHAR *) clump, (SCHAR *) & temp, JRNG_SIZE);

		if (temp.jrng_type != JRNP_GENERATOR)
			BUGCHECK(273);		/* msg 273 error in recovery! wrong generator page record */

		ptr = page->ppg_page + temp.jrng_offset;
		*ptr = temp.jrng_genval;
	}
}


static void apply_index(BTR page, JRND * record)
{
/**************************************
 *
 *	a p p l y _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Apply changes to b-tree pages
 *
 **************************************/
	JRNB temp, *clump;
	UCHAR *p, *q;
	SLONG l;
	SLONG delta;
	BTN node, next;

	for (clump = NULL;
		 (clump =
		 (JRNB *) next_clump(record, reinterpret_cast < jrnp * >(clump)));) {
		MOVE_FAST((SCHAR *) clump, (SCHAR *) & temp, JRNB_SIZE);

		switch (temp.jrnb_type) {
		case JRNP_BTREE_NODE:

			/* 
			 * slide down upper part by delta
			 * add node and increment btr_length 
			 */

			delta = temp.jrnb_delta;
			p = (UCHAR *) page + page->btr_length;
			q = p + delta;
			if ( (l = page->btr_length - temp.jrnb_offset) )
				do
					*--q = *--p;
				while (--l);

			/* move in node , then BTN */

			p = (UCHAR *) page + temp.jrnb_offset;
			q = clump->jrnb_data;

			if ( (l = temp.jrnb_length) )
				MOVE_FAST(q, p, l);

			page->btr_length += delta;
			page->btr_prefix_total = temp.jrnb_prefix_total;

			break;

		case JRNP_BTREE_SEGMENT:

			/* apply change directly */

			p = (UCHAR *) page;
			q = (UCHAR *) clump->jrnb_data;

			if ( (l = temp.jrnb_length) )
				MOVE_FAST(q, p, l);
			break;

		case JRNP_BTREE_DELETE:

			/* delete a node entry */

			node = (BTN) ((UCHAR *) page + temp.jrnb_offset);
			next = (BTN) (BTN_DATA(node) + BTN_LENGTH(node));
			QUAD_MOVE(BTN_NUMBER(next), BTN_NUMBER(node));
			p = BTN_DATA(node);
			q = BTN_DATA(next);
			l = BTN_LENGTH(next);
			if (BTN_PREFIX(node) < BTN_PREFIX(next)) {
				BTN_LENGTH(node) = BTN_LENGTH(next) + BTN_PREFIX(next)
					- BTN_PREFIX(node);
				p += BTN_PREFIX(next) - BTN_PREFIX(node);
			}
			else {
				BTN_LENGTH(node) = l;
				BTN_PREFIX(node) = BTN_PREFIX(next);
			}

			if (l)
				do
					*p++ = *q++;
				while (--l);

			/* Compute length of rest of bucket and move it down. */

			if ( (l = page->btr_length - (q - (UCHAR *) page)) )
				do
					*p++ = *q++;
				while (--l);

			page->btr_length = p - (UCHAR *) page;
			page->btr_prefix_total = temp.jrnb_prefix_total;

			/* Error Check */

			if ((BTN_PREFIX(node) != temp.jrnb_delta) ||
				(page->btr_length != temp.jrnb_length)) BUGCHECK(274);	/* msg 274 error in recovery! wrong b-tree page record */

			break;

		default:
			BUGCHECK(274);		/* msg 274 error in recovery! wrong b-tree page record */
		}
	}
}


static void apply_log(LIP page, JRND * record)
{
/**************************************
 *
 *	a p p l y _ l o g
 *
 **************************************
 *
 * Functional description
 *	Apply changes to database log page (= 2)
 *
 **************************************/
	JRNL temp, *clump;

	for (clump = NULL;
		 (clump =
		 (JRNL *) next_clump(record, reinterpret_cast < jrnp * >(clump)));) {
		MOVE_FAST((SCHAR *) clump, (SCHAR *) & temp, JRNL_SIZE);

		page->log_flags = temp.jrnl_flags;
		page->log_mod_tid = temp.jrnl_tid;
		page->log_mod_tip = temp.jrnl_tip;
	}
	page->log_flags |= log_rec_in_progress;
}


static void apply_pip(PIP page, JRND * record)
{
/**************************************
 *
 *	a p p l y _ p i p
 *
 **************************************
 *
 * Functional description
 *	Apply changes to page inventory page.
 *
 **************************************/
	JRNA temp, *clump;
	UCHAR bit;
	USHORT byte;

	for (clump = NULL;
		 (clump =
		 (JRNA *) next_clump(record, reinterpret_cast < jrnp * >(clump)));) {
		MOVE_FAST((SCHAR *) clump, (SCHAR *) & temp, sizeof(struct jrna));

		if (temp.jrna_type != JRNP_PIP)
			BUGCHECK(275);		/* msg 275 error in recovery! wrong page inventory page record */

		byte = temp.jrna_slot >> 3;
		bit = 1 << (temp.jrna_slot & 7);
		if (temp.jrna_allocate)
			page->pip_bits[byte] &= ~bit;
		else {
			page->pip_bits[byte] |= bit;
			page->pip_min = MIN(page->pip_min, (SLONG) temp.jrna_slot);
		}
	}
}


static void apply_pointer(PPG page, JRND * record)
{
/**************************************
 *
 *	a p p l y _ p o i n t e r
 *
 **************************************
 *
 * Functional description
 *	Apply incremental changes to a pointer page.
 *
 **************************************/
	JRNP temp, *clump;
	SLONG longword;

	for (clump = NULL; (clump = next_clump(record, clump));) {
		MOVE_FAST((SCHAR *) clump, (SCHAR *) & temp, JRNP_SIZE);

		if (temp.jrnp_type != JRNP_POINTER_SLOT)
			BUGCHECK(276);		/* msg 276 error in recovery! wrong pointer page record */
		if (temp.jrnp_length) {
			MOVE_FAST(clump->jrnp_data, &longword, sizeof(SLONG));
			page->ppg_page[temp.jrnp_index] = longword;
			page->ppg_count = MAX(page->ppg_count, temp.jrnp_index + 1);
			page->ppg_min_space = MIN(page->ppg_min_space, temp.jrnp_index);
			page->ppg_max_space = MAX(page->ppg_min_space, temp.jrnp_index);
		}
		else
			page->ppg_page[temp.jrnp_index] = 0;
	}
}


static void apply_root(IRT page, JRND * record)
{
/**************************************
 *
 *	a p p l y _ r o o t
 *
 **************************************
 *
 * Functional description
 *	Apply changes to index root page
 *
 **************************************/
	JRNRP temp, *clump;

	for (clump = NULL;
		 (clump =
		 (JRNRP *) next_clump(record, reinterpret_cast < jrnp * >(clump)));) {
		MOVE_FAST((SCHAR *) clump, (SCHAR *) & temp, JRNRP_SIZE);

		if (temp.jrnrp_type != JRNP_ROOT_PAGE)
			BUGCHECK(277);		/* msg 277 error in recovery! wrong index root page record */

		page->irt_rpt[temp.jrnrp_id].irt_root = temp.jrnrp_page;
	}
}


static void apply_transaction(TIP page, JRND * record)
{
/**************************************
 *
 *	a p p l y _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Apply incremental changes to transaction inventory page
 *
 **************************************/
	JRNI *clump, *clump_end;
	JRNI temp;
	JRND rec;

	MOVE_FAST((SCHAR *) record, (SCHAR *) & rec, JRND_SIZE);
	clump = (JRNI *) record->jrnd_data;
	clump_end = (JRNI *) (record->jrnd_data + rec.jrnd_length);

/* Process clumps */

	for (; clump < clump_end; clump++) {
		MOVE_FAST((SCHAR *) clump, (SCHAR *) & temp, JRNI_SIZE);

		if (temp.jrni_type == JRNP_TRANSACTION)
			page->tip_transactions[temp.jrni_position] = temp.jrni_states;
		else if (temp.jrni_type == JRNP_NEXT_TIP)
			page->tip_next = temp.jrni_transaction;
		else
			BUGCHECK(278);		/* msg 278 error in recovery! wrong transaction page record */
	}
}


static void disable(void)
{
/**************************************
 *
 *	d i s a b l e
 *
 **************************************
 *
 * Functional description
 *	We encountered a database disable record.
 *	Clean up database header page.
 *
 **************************************/
/*
 * Delete journal related entries
 */

	PAG_delete_clump_entry(HEADER_PAGE, HDR_journal_server);
	PAG_delete_clump_entry(HEADER_PAGE, HDR_backup_info);
}


static JRNP *next_clump(JRND * record, JRNP * prior)
{
/**************************************
 *
 *	n e x t _ c l u m p
 *
 **************************************
 *
 * Functional description
 *	Given a prior clump, compute the address of the next
 *	clump on a data page.  If the prior clump is null,
 *	compute the address of the first clump.  If we run
 *	off the record, return NULL.
 *
 **************************************/
	JRNB temp1;
	USHORT offset, l;
	JRNP temp;

/* If the prior pointer is null, just return the data area */

	if (!prior)
		return (JRNP *) record->jrnd_data;

/* Compute the offset and length of prior clump */

	offset = (SCHAR *) prior - (SCHAR *) record;

	MOVE_FAST((SCHAR *) prior, (SCHAR *) & temp, JRNP_SIZE);

	switch (temp.jrnp_type) {
	case JRNP_DATA_SEGMENT:
	case JRNP_POINTER_SLOT:
		l = JRNP_SIZE + temp.jrnp_length;
		break;

	case JRNP_BTREE_SEGMENT:
	case JRNP_BTREE_NODE:
		MOVE_FAST((SCHAR *) prior, (SCHAR *) & temp1, JRNB_SIZE);

		l = JRNB_SIZE + temp1.jrnb_length;
		break;

		/* 
		 * currently DELETE node contains some debug info in jrnb_length 
		 * but data field is not used.
		 */
	case JRNP_BTREE_DELETE:
		l = JRNB_SIZE;
		break;

	case JRNP_PIP:
		l = sizeof(struct jrna);
		break;

	case JRNP_DB_HEADER:
		l = JRNDH_SIZE;
		break;

	case JRNP_LOG_PAGE:
		l = JRNL_SIZE;
		break;

	case JRNP_DB_ATTACHMENT:
	case JRNP_DB_HDR_PAGES:
	case JRNP_DB_HDR_FLAGS:
	case JRNP_DB_HDR_SDW_COUNT:
		l = JRNDA_SIZE;
		break;

	case JRNP_GENERATOR:
		l = JRNG_SIZE;
		break;

	case JRNP_ROOT_PAGE:
		l = JRNRP_SIZE;
		break;

	default:
		BUGCHECK(269);			/* msg 269 error in recovery! database corrupted */
	}

	offset += l;

	if (offset & 1)
		++offset;

	if (offset >= (JRND_SIZE + record->jrnd_length))
		return 0;

	return (JRNP *) ((SCHAR *) record + offset);
}


static void process_page(
						 TDBB tdbb,
						 JRND * record,
						 SLONG seqno,
						 SLONG offset,
						 PAG rec_page, SBM * sbm_rec, USHORT activate_shadow)
{
/**************************************
 *
 *	p r o c e s s _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Process a WAL record associated with a database page
 *	If rec_page is set, a corrupt page is being recovered.
 *	This page may have been read with a read lock, so do not
 *	mark it or read it again.
 *
 **************************************/
	DBB dbb;
	PAG page;
	JRNP *clump;
	UCHAR *p, *q;
	WIN window;
	LIP logp;
	JRND rec;
	HDR hdr;
	USHORT hdr_end;

	SET_TDBB(tdbb);

	dbb = tdbb->tdbb_database;
	window.win_flags = 0;

	clump = (JRNP *) record->jrnd_data;
	q = (UCHAR *) clump;

/* If record is a full replacement page, handle it */

	MOVE_FAST((SCHAR *) record, (SCHAR *) & rec, JRND_SIZE);

/* handle zero length data records.  This is an offshoot of PAG_release_page
 * where the free page is left in the shared cache with some possible 
 * journal records in it.
 */

	if (rec.jrnd_length == 0)
		return;

/* set the page number in the sparse bit map during short term recovery
 * but not during single page recovery.
 */

	if (!rec_page)
		SBM_set(tdbb, sbm_rec, rec.jrnd_page);

	if (rec_page)
		page = rec_page;
	else {
		window.win_page = rec.jrnd_page;

		/* If this is a newly allocated page, read will fail at EOF */

		if ((rec.jrnd_header.jrnh_prev_seqno == 0)
			&& (clump->jrnp_type <= pag_max))
			page = CCH_fake(tdbb, &window, 1);
		else
			page = CCH_FETCH(tdbb, &window, LCK_write, pag_undefined);
	}

/* If there is no need to apply record, just return */

	if (((SLONG) page->pag_seqno > seqno) ||
		(((SLONG) page->pag_seqno == seqno) && ((SLONG) page->pag_offset >= offset))) {
		if (!rec_page)
			CCH_RELEASE(tdbb, &window);
		return;
	}

/*
 * Error check to see if records are being skipped
 * Do not check for error if it is a full page, or clump type
 * is JRNP_BTREE_SEGMENT and the previous seqno & offset are 0
 */

	if (!((rec.jrnd_header.jrnh_prev_seqno == 0) &&
		  (rec.jrnd_header.jrnh_prev_offset == 0) &&
		  ((clump->jrnp_type <= pag_max) ||
		   (clump->jrnp_type == JRNP_BTREE_SEGMENT)))) {
		if (((SLONG) page->pag_seqno != rec.jrnd_header.jrnh_prev_seqno) ||
			((SLONG) page->pag_offset != rec.jrnd_header.jrnh_prev_offset)) {
			if (!rec_page)
				CCH_RELEASE(tdbb, &window);
			BUGCHECK(279);		/* msg 279 error in recovery! out of sequence log record encountered */
		}
	}

/* changes need to be applied */

	if (!rec_page)
		CCH_MARK(tdbb, &window);

	if (clump->jrnp_type <= pag_max) {
		p = (UCHAR *) page;

		/* 
		 * if header page is being replaced, and we are activating shadow
		 * copy only relevent part.  i.e. non clumplet part.
		 */

		if ((activate_shadow) && (rec.jrnd_page == HEADER_PAGE)) {
			hdr = (HDR) page;
			hdr_end = hdr->hdr_end;
			MOVE_FAST(q, p, HDR_SIZE);
			hdr->hdr_end = hdr_end;
			hdr->hdr_flags |= hdr_active_shadow;
			hdr->hdr_next_page = 0;
		}
		else
			MOVE_FAST(q, p, dbb->dbb_page_size);

		page->pag_seqno = seqno;
		page->pag_offset = offset;

		/* If LOG_page is being replaced, set the flag log_rec_in_progress */

		if (rec.jrnd_page == LOG_PAGE) {
			logp = (LIP) page;
			logp->log_flags |= log_rec_in_progress;
		}
		if (!rec_page)
			CCH_RELEASE(tdbb, &window);
		return;
	}

	switch (page->pag_type) {
	case pag_pointer:
		apply_pointer(reinterpret_cast < PPG > (page), record);
		break;

	case pag_data:
		apply_data(reinterpret_cast < dpg * >(page), record);
		break;

	case pag_transactions:
		apply_transaction(reinterpret_cast < tip * >(page), record);
		break;

	case pag_pages:
		apply_pip(reinterpret_cast < pip * >(page), record);
		break;
	case pag_header:
		apply_header(reinterpret_cast < HDR > (page), record);
		break;
	case pag_root:
		apply_root(reinterpret_cast < IRT > (page), record);
		break;
	case pag_index:
		apply_index(reinterpret_cast < btr * >(page), record);
		break;
	case pag_ids:
		apply_ids(reinterpret_cast < PPG > (page), record);
		break;
	case pag_log:
		apply_log(reinterpret_cast < LIP > (page), record);
		break;

	default:
		BUGCHECK(280);			/* msg 280 error in recovery! unknown page type */
	}

	page->pag_seqno = seqno;
	page->pag_offset = offset;
	if (!rec_page)
		CCH_RELEASE(tdbb, &window);
}


static void quad_move(UCHAR * a, UCHAR * b)
{
/**************************************
 *
 *      q u a d _ m o v e
 *
 **************************************
 *
 * Functional description
 *      Move an unaligned longword ( 4 bytes).
 *
 **************************************/

	MOVE_BYTE(a, b);
	MOVE_BYTE(a, b);
	MOVE_BYTE(a, b);
	MOVE_BYTE(a, b);
}


static void rec_process_record(
							   TDBB tdbb,
							   JRNH * record,
							   USHORT length,
							   ULONG seqno,
							   ULONG offset,
PAG page, SBM * sbm_rec, USHORT activate_shadow)
{
/**************************************
 *
 *	r e c _ p r o c e s s _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Process a single WAL record.  
 *
 **************************************/

	SET_TDBB(tdbb);

/* Handle particular record type */

	switch (record->jrnh_type) {
	case JRN_PAGE:
		process_page(tdbb, reinterpret_cast < JRND * >(record), seqno, offset,
					 page, sbm_rec, activate_shadow);
		break;

	case JRN_DISABLE:
		disable();
		break;

	case JRN_NEW_FILE:
		/* 
		 * The whole page is journalled if it is the primary page
		 * The page is already written out, so NO
		 * problem!! (Or should PAG_add_file () be called?)
		 */
		break;

	case JRN_COMMIT:
		/*
		 * the commit is journalled in the tip pages 
		 */
		break;

	case JRN_ENABLE:
	case JRN_SYNC:
	case JRN_ONLINE_DMP_FILE:
	case JRN_START_ONLINE_DMP:
	case JRN_END_ONLINE_DMP:
	case JRN_CNTRL_PT:
	case JRN_WAL_NAME:
		/*
		 * No action needed
		 */
		break;

	default:
		BUGCHECK(281);			/* msg 281 error in recovery! unknown record type */
	}
}


static void scan_and_apply_logs(
								TEXT * dbname,
								TEXT * start_logname,
								CP * cp1,
								SBM * sbm_rec,
USHORT activate_shadow, SLONG * timestamp, SLONG page_no, PAG page)
{
/**************************************
 *
 *	s c a n _ a n d _ a p p l y _ l o g
 *
 **************************************
 *
 * Functional description
 *	Do short-term or page recovery.
 *      If a page is passed, recover only that page.
 *	Fixup log header at end in case of short-term recovery.
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;
	SLONG ret_val;
	SSHORT len;
	ULONG seqno;
	ULONG offset;
	WALR WALRS_handle;
	UCHAR *wal_buff;
	STR string;
	JRND rec;
#ifdef DEV_BUILD
	JRND rec1;
	SLONG checksum;
#endif

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;

/* Open wal files */

	WALRS_handle = 0;
	ret_val =
		WALR_open(tdbb->tdbb_status_vector,
				  reinterpret_cast < walrs ** >(&WALRS_handle), dbname, 1,
				  &start_logname, &cp1->cp_p_offset, cp1->cp_offset,
				  timestamp, FALSE);
	if (ret_val == -1)
		return;					/* reached EOF */
	else if (ret_val != FB_SUCCESS)
		ERR_punt();
	else if (!WALRS_handle)
		ERR_post(gds_wal_failure, 0);

	string = FB_NEW_RPT(*dbb->dbb_permanent, MAX_WALBUFLEN) str();
	wal_buff = (UCHAR *) string->str_data;

	while (TRUE) {
		ret_val =
			WALR_get(tdbb->tdbb_status_vector,
					 reinterpret_cast < walrs * >(WALRS_handle), wal_buff,
					 reinterpret_cast < USHORT * >(&len),
					 reinterpret_cast < long *>(&seqno),
					 reinterpret_cast < long *>(&offset));
		if (ret_val == -1)		/* done processing */
			break;
		else if (ret_val != FB_SUCCESS) {
			WALR_close(tdbb->tdbb_status_vector,
					   reinterpret_cast < walrs ** >(&WALRS_handle));
			delete string;
			ERR_punt();
		}

#ifdef DEV_BUILD
		/* take care of word alignment problems */

		MOVE_FAST((SCHAR *) wal_buff, (SCHAR *) & rec1, JRND_SIZE);
		checksum = rec1.jrnd_header.jrnh_series;
		rec1.jrnd_header.jrnh_series = 0;
		MOVE_FAST((SCHAR *) & rec1, (SCHAR *) wal_buff, JRND_SIZE);

		if (checksum != (SLONG) MISC_checksum_log_rec(wal_buff, len, 0, 0))
			BUGCHECK(283);

		rec1.jrnd_header.jrnh_series = checksum;
		MOVE_FAST((SCHAR *) & rec1, (SCHAR *) wal_buff, JRND_SIZE);
#endif

		if (page) {
			/* Recovery is needed for one page only */

			MOVE_FAST((SCHAR *) wal_buff, (SCHAR *) & rec, JRND_SIZE);
			if (rec.jrnd_page != page_no)
				continue;
		}

		rec_process_record(tdbb, (JRNH *) wal_buff, len, seqno, offset, page,
						   sbm_rec, activate_shadow);
	}

	if (!page)					/* i.e. regular short-term recovery */
		WALR_fixup_log_header(tdbb->tdbb_status_vector,
							  reinterpret_cast < walrs * >(WALRS_handle));

	WALR_close(tdbb->tdbb_status_vector,
			   reinterpret_cast < walrs ** >(&WALRS_handle));

	delete string;
}
