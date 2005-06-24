/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		rse.cpp
 *	DESCRIPTION:	Run time record fetching
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
 *
 * $Id: rse.cpp,v 1.89 2005-06-24 12:56:34 dimitr Exp $
 *
 * 2001.07.28: John Bellardo: Implemented rse_skip and made rse_first work with
 *                              seekable streams.
 * 2002.02.22 Claudio Valderrama: Fix SF Bugs #225283, #518279, #514186 & #221925.
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#include "firebird.h"
#include <errno.h>
#include "../jrd/common.h"
#include <stdio.h>
#include <string.h>
#include "../jrd/jrd.h"
#include "../jrd/req.h"
#include "../jrd/val.h"
#include "../jrd/exe.h"
#include "../jrd/rse.h"
#include "../jrd/sbm.h"
#include "../jrd/lls.h"
#include "../jrd/sort.h"
#include "../jrd/intl.h"
#include "../jrd/ods.h"
#include "../jrd/btr.h"
#include "../jrd/lck.h"
#include "../jrd/cch.h"
#include "../jrd/tra.h"
#include "../jrd/sort_mem.h"
#include "gen/iberror.h"
#include "../jrd/gdsassert.h"
#include "../jrd/all_proto.h"
#include "../jrd/bookmark.h"
#include "../jrd/dpm_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/exe_proto.h"
#include "../jrd/ext_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/jrd_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/nav_proto.h"
#include "../jrd/rlck_proto.h"
#include "../jrd/rse_proto.h"
#include "../jrd/sort_proto.h"
#include "../jrd/thd.h"
#include "../jrd/vio_proto.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#if defined(WIN_NT)
#include <io.h> // close
#endif

using namespace Jrd;

static void close_merge(thread_db*, RecordSource*, IRSB_MRG);
static void close_procedure(thread_db*, RecordSource*);
static SSHORT compare(thread_db*, jrd_nod*, jrd_nod*);
static SSHORT compare_longs(const SLONG*, const SLONG*, USHORT);
#ifdef SCROLLABLE_CURSORS
static bool fetch_record(thread_db*, RecordSource*, SSHORT, RSE_GET_MODE);
static bool get_merge_join(thread_db*, RecordSource*, IRSB_MRG, RSE_GET_MODE);
static bool get_merge_fetch(thread_db*, RecordSource*, SSHORT, RSE_GET_MODE);
static SLONG get_merge_record(thread_db*, RecordSource*, irsb_mrg::irsb_mrg_repeat *,
							  RSE_GET_MODE);
static UCHAR *get_sort(thread_db*, RecordSource*, RSE_GET_MODE);
static void resynch_merge(thread_db*, RecordSource*, IRSB_MRG, RSE_GET_MODE);
static void unget_sort(thread_db*, RecordSource*, UCHAR *);
#else
static bool fetch_record(thread_db*, RecordSource*, SSHORT);
static bool get_merge_join(thread_db*, RecordSource*, IRSB_MRG);
static bool get_merge_fetch(thread_db*, RecordSource*, SSHORT);
static SLONG get_merge_record(thread_db*, RecordSource*, irsb_mrg::irsb_mrg_repeat *);
static UCHAR *get_sort(thread_db*, RecordSource*);
#endif
#ifdef SCROLLABLE_CURSORS
static bool fetch_left(thread_db*, RecordSource*, IRSB, RSE_GET_MODE);
#else
static bool fetch_left(thread_db*, RecordSource*, IRSB);
#endif
static UCHAR* get_merge_data(thread_db*, merge_file*, SLONG);
static bool get_procedure(thread_db*, RecordSource*, IRSB_PROCEDURE, record_param*);
static bool get_record(thread_db*, RecordSource*, RecordSource*, RSE_GET_MODE);
static bool get_union(thread_db*, RecordSource*, IRSB);
static void join_to_nulls(thread_db*, RecordSource*, StreamStack*);
static void map_sort_data(jrd_req*, SortMap*, UCHAR *);
static void open_merge(thread_db*, RecordSource*, IRSB_MRG);
static void open_procedure(thread_db*, RecordSource*, IRSB_PROCEDURE);
static void open_sort(thread_db*, RecordSource*, IRSB_SORT, UINT64);
static void proc_assignment(const dsc*, const dsc*, UCHAR*, dsc*, SSHORT, Record*);
static void pop_rpbs(jrd_req*, RecordSource*);
static void push_rpbs(thread_db*, jrd_req*, RecordSource*);
static ULONG read_merge_block(thread_db*, merge_file*, ULONG);
static bool reject(const UCHAR*, const UCHAR*, void*);
static void restore_record(record_param*);
static void save_record(thread_db*, record_param*);
static void write_merge_block(thread_db*, merge_file*, ULONG);

#ifdef SMALL_FILE_NAMES
static const char* SCRATCH		= "fb_m";
#else
static const char* SCRATCH		= "fb_merge_";
#endif



void RSE_close(thread_db* tdbb, RecordSource* rsb)
{
/**************************************
 *
 *	R S E _ c l o s e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	SET_TDBB(tdbb);

	while (true) {
		IRSB_SORT impure = (IRSB_SORT) ((UCHAR *) tdbb->tdbb_request + rsb->rsb_impure);
		if (!(impure->irsb_flags & irsb_open))
			return;

		impure->irsb_flags &= ~irsb_open;

		switch (rsb->rsb_type) {
		case rsb_indexed:
		case rsb_navigate:
			return;

		case rsb_sequential:
			{
				jrd_req* request = tdbb->tdbb_request;
				record_param* rpb = &request->req_rpb[rsb->rsb_stream];
				if (rpb->rpb_window.win_flags & WIN_large_scan &&
					rpb->rpb_relation->rel_scan_count)
						--rpb->rpb_relation->rel_scan_count;
				return;
			}

		case rsb_first:
        case rsb_skip:
		case rsb_boolean:
		case rsb_aggregate:
			rsb = rsb->rsb_next;
			break;

		case rsb_cross:
			{
				RecordSource** ptr = rsb->rsb_arg;
				for (const RecordSource* const* const end = ptr + rsb->rsb_count;
					 ptr < end; ptr++)
				{
					RSE_close(tdbb, *ptr);
				}
				return;
			}

		case rsb_left_cross:
			RSE_close(tdbb, rsb->rsb_arg[RSB_LEFT_outer]);
			RSE_close(tdbb, rsb->rsb_arg[RSB_LEFT_inner]);
			return;

		case rsb_procedure:
			close_procedure(tdbb, rsb);
			return;

		case rsb_merge:
			close_merge(tdbb, rsb, (IRSB_MRG) impure);
			return;

		case rsb_sort:
			SORT_fini(impure->irsb_sort_handle, tdbb->tdbb_attachment);
			impure->irsb_sort_handle = NULL;
			rsb = rsb->rsb_next;
			break;

		case rsb_union:
			{
				const USHORT i = ((IRSB) impure)->irsb_count;
				if (i >= rsb->rsb_count)
					return;
				rsb = rsb->rsb_arg[i];
			}
			break;

		case rsb_ext_sequential:
		case rsb_ext_indexed:
		case rsb_ext_dbkey:
			EXT_close(rsb);
			return;

		default:
			BUGCHECK(166);		/* msg 166 invalid rsb type */
		}
	}
}


#ifdef PC_ENGINE
bool RSE_find_dbkey(thread_db* tdbb, RecordSource* rsb, jrd_nod* find_key, jrd_nod* record_version)
{
/**************************************
 *
 *	R S E _ f i n d _ d b k e y
 *
 **************************************
 *
 * Functional description
 *	Find the passed dbkey in the stream
 *	with equivalent key values.  Optionally,
 *	check for a record version also.
 *
 **************************************/

	SET_TDBB(tdbb);
	jrd_req* request = tdbb->tdbb_request;

/* get the record number from the passed dbkey */

	const dsc* desc = EVL_expr(tdbb, find_key);

	ULONG dbkey[2];
	dsc desc2;
	desc2.dsc_address = (UCHAR *) dbkey;
	desc2.dsc_length = sizeof(dbkey);
	desc2.dsc_dtype = dtype_text;
	desc2.dsc_scale = 0;
	desc2.dsc_sub_type = ttype_binary;
	desc2.dsc_flags = 0;

	MOV_move(desc, &desc2);

/* now get the record version to use in comparing
   against the tid of the record */

	ULONG version_number;
	if (record_version) {
		desc = EVL_expr(tdbb, record_version);

		desc2.dsc_address = (UCHAR *) & version_number;
		desc2.dsc_length = sizeof(version_number);
		desc2.dsc_dtype = dtype_text;
		desc2.dsc_scale = 0;
		desc2.dsc_sub_type = ttype_binary;
		desc2.dsc_flags = 0;

		MOV_move(desc, &desc2);
	}

	record_param* rpb;
	IRSB_INDEX impure;
	RecordBitmap* bitmap;

	switch (rsb->rsb_type) {
	case rsb_boolean:
		if (!RSE_find_dbkey(tdbb, rsb->rsb_next, find_key, record_version))
			return false;

		if ((rsb->rsb_arg[0]) && (!EVL_boolean(tdbb, rsb->rsb_arg[0]))) {
			RSE_MARK_CRACK(tdbb, rsb, irsb_crack);
			return false;
		}

		return true;

	case rsb_navigate:
		rpb = request->req_rpb + rsb->rsb_stream;
		rpb->rpb_number = dbkey[1] - 1;

		/* first, fetch the indicated record */

		if (!VIO_get
			(tdbb, rpb, rsb, request->req_transaction,
			 request->req_pool))
		{
			return false;
		}
		if (record_version && version_number != rpb->rpb_transaction_nr)
			return false;

		/* next, set the stream position to that of the fetched record */

		RSE_MARK_CRACK(tdbb, rsb, 0);
		if (NAV_reset_position(rsb, rpb))
			return true;
		RSE_MARK_CRACK(tdbb, rsb, irsb_crack);
		return false;

	case rsb_sequential:
		rpb = request->req_rpb + rsb->rsb_stream;
		rpb->rpb_number = dbkey[1] - 1;

		RSE_MARK_CRACK(tdbb, rsb, 0);
		if (VIO_get
			(tdbb, rpb, rsb, request->req_transaction, request->req_pool)
			&& (!record_version || version_number == rpb->rpb_transaction_nr))
		{
			return true;
		}
		RSE_MARK_CRACK(tdbb, rsb, irsb_crack);
		return false;

	case rsb_indexed:
		rpb = request->req_rpb + rsb->rsb_stream;
		rpb->rpb_number = dbkey[1] - 1;
		impure = (IRSB_INDEX) ((UCHAR *) request + rsb->rsb_impure);

		RSE_MARK_CRACK(tdbb, rsb, 0);
		if ((bitmap = impure->irsb_bitmap) &&
			SBM_next(*bitmap, &rpb->rpb_number, RSE_get_current) &&
			VIO_get(tdbb, rpb, rsb, request->req_transaction,
					request->req_pool) &&
			(!record_version || version_number == rpb->rpb_transaction_nr))
		{
				return true;
		}
		RSE_MARK_CRACK(tdbb, rsb, irsb_crack);
		return false;

	default:
		BUGCHECK(166);			/* msg 166 invalid rsb type */
		return false;			/* Added to remove compiler warnings */
	}
}
#endif


#ifdef PC_ENGINE
bool RSE_find_record(thread_db* tdbb,
						RecordSource* rsb,
						USHORT blr_operator, USHORT direction, jrd_nod* find_key)
{
/**************************************
 *
 *	R S E _ f i n d _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Find the record with the passed key
 *	value, using the passed operator to
 *	compare records with the key value.
 *	Search forwards or backwards.
 *
 **************************************/

	SET_TDBB(tdbb);

	switch (rsb->rsb_type) {
	case rsb_indexed:
		return RSE_find_record(tdbb, rsb->rsb_next, blr_operator, direction,
							   find_key);

	case rsb_boolean:
		if (!RSE_find_record
			(tdbb, rsb->rsb_next, blr_operator, direction, find_key))
		{
			return false;
		}

		if ((rsb->rsb_arg[0]) && (!EVL_boolean(tdbb, rsb->rsb_arg[0]))) {
			RSE_MARK_CRACK(tdbb, rsb, irsb_crack);
			return false;
		}

		return true;

	case rsb_navigate:
		return NAV_find_record(rsb, blr_operator, direction, find_key);

	default:
		BUGCHECK(166);			/* msg 166 invalid rsb type */
		return false;			/* Added to remove compiler warnings */
	}
}
#endif


bool RSE_get_record(thread_db* tdbb, RecordSource* rsb, RSE_GET_MODE mode)
{
/**************************************
 *
 *	R S E _ g e t _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *	External entrypoint for get_record().
 *	Handle the "stickiness" of directionality.
 *	Check whether we need to count records
 *	retrieved at the top level of the rsb tree.
 *
 **************************************/
	SET_TDBB(tdbb);
	jrd_req* request = tdbb->tdbb_request;
	IRSB impure = (IRSB) ((UCHAR *) request + rsb->rsb_impure);

#ifdef SCROLLABLE_CURSORS
/* The mode RSE_get_next is a generic mode which requests that
   we continue on in the last direction we were going.  Oblige
   by converting the mode to the appropriate direction. */

	if (mode == RSE_get_next)
	{
		mode =
			(impure->irsb_flags & irsb_last_backwards) ? RSE_get_backward :
			RSE_get_forward;
	}
	request->req_flags &= ~req_fetch_required;
#endif

/* Turn off the flag so that records at a
   lower level will not be counted. */

	const bool count = (request->req_flags & req_count_records) != 0;
	request->req_flags &= ~req_count_records;

	bool result;
	while ( (result = get_record(tdbb, rsb, NULL, mode)) )
	{
		if (rsb->rsb_flags & rsb_writelock)
		{
			// Lock record if we were asked for it
			jrd_tra* transaction = request->req_transaction;

			RecordSource* table_rsb = rsb;

			// Skip nodes without streams
			while (table_rsb->rsb_type == rsb_boolean ||
				    table_rsb->rsb_type == rsb_first ||
					table_rsb->rsb_type == rsb_skip)
			{
				table_rsb = table_rsb->rsb_next;
			}

			record_param* org_rpb = request->req_rpb + table_rsb->rsb_stream;
			jrd_rel* relation = org_rpb->rpb_relation;

			// Raise error if we cannot lock this kind of stream
			if (!relation || relation->rel_view_rse || relation->rel_file) {
				ERR_post(isc_random, isc_arg_string, "Unsupported RSE construct for blr_writelock operation", 0);
			}

			RLCK_reserve_relation(tdbb, transaction, relation, true, true);

			// Fetch next record if current was deleted before being locked
			if (!VIO_writelock(tdbb, org_rpb, rsb, transaction)) {
				continue;
			}
		}

		if (count) {
			request->req_records_selected++;
			request->req_records_affected.bumpFetched();
		}
		break;
	}

/* reset the flag to whatever it was */

	if (count)
		request->req_flags |= req_count_records;

	return result;
}


#ifdef PC_ENGINE
Bookmark* RSE_get_bookmark(thread_db* tdbb, RecordSource* rsb)
{
/**************************************
 *
 *	R S E _ g e t _ b o o k m a r k
 *
 **************************************
 *
 * Functional description
 *	Return a descriptor whose value is a pointer
 *	to a bookmark describing the location of
 *	the current record in a navigational stream.
 *
 **************************************/
	SET_TDBB(tdbb);
	jrd_req* request = tdbb->tdbb_request;
	if (request->req_flags & req_abort)
		return FALSE;

	switch (rsb->rsb_type) {
	case rsb_boolean:
		return RSE_get_bookmark(rsb->rsb_next);

	case rsb_navigate:
		{
			IRSB_NAV impure = (IRSB_NAV) ((UCHAR *) request + rsb->rsb_impure);
			Bookmark* bookmark = BKM_allocate(rsb, impure->irsb_nav_length);
			NAV_get_bookmark(rsb, impure, bookmark);
			return bookmark;
		}

	case rsb_indexed:
	case rsb_sequential:
		{
			record_param* rpb = request->req_rpb + rsb->rsb_stream;
			Bookmark* bookmark = BKM_allocate(rsb, (USHORT) 0);
			bookmark->bkm_number = rpb->rpb_number;
			bookmark->bkm_key_desc.dsc_dtype = dtype_long;
			bookmark->bkm_key_desc.dsc_length = sizeof(bookmark->bkm_number);
			bookmark->bkm_key_desc.dsc_address = (UCHAR *) & bookmark->bkm_number;
			return bookmark;
		}

	default:
		BUGCHECK(166);			/* msg 166 invalid rsb type */
		return FALSE;			/* Added to remove compiler warnings */
	}
}
#endif


#ifdef PC_ENGINE
void RSE_mark_crack(thread_db* tdbb, RecordSource* rsb, USHORT flags)
{
/**************************************
 *
 *	R S E _ m a r k _ c r a c k
 *
 **************************************
 *
 * Functional description
 *	Position stream on a crack.
 *
 **************************************/
	SET_TDBB(tdbb);
	jrd_req* request = tdbb->tdbb_request;

/* correct boolean rsbs to point to the "real" rsb */

	if (rsb->rsb_type == rsb_boolean)
		rsb = rsb->rsb_next;

/* clear all the flag bits first to make sure
   no conflicting bits are set */

	irsb* impure = (IRSB) ((UCHAR *) request + rsb->rsb_impure);
	impure->irsb_flags &=
		~(irsb_bof | irsb_eof | irsb_crack | irsb_forced_crack);

	impure->irsb_flags |= flags;

/* release the current record to make sure
   that no one erroneously tries to retrieve it */

	if (flags) {
		record_param* rpb = &request->req_rpb[rsb->rsb_stream];
		if (rpb->rpb_record) {
			delete rpb->rpb_record;
			rpb->rpb_record = NULL;
		}
		if (rpb->rpb_copy) {
			delete rpb->rpb_copy;
			rpb->rpb_copy = NULL;
		}
	}
}
#endif


void RSE_open(thread_db* tdbb, RecordSource* rsb)
{
/**************************************
 *
 *	R S E _ o p e n
 *
 **************************************
 *
 * Functional description
 *	Open a stream, as represented by the
 *	record source block (rsb).
 *
 **************************************/
	SINT64 first_records = -1, skip_records = 0;

	SET_TDBB(tdbb);

	jrd_req* request = tdbb->tdbb_request;

	// Initialize dependent invariants if any
	if (rsb->rsb_invariants) {
		SLONG *ptr, *end;
		for (ptr = rsb->rsb_invariants->begin(), end = rsb->rsb_invariants->end();
			ptr < end; ptr++)
		{
			reinterpret_cast<impure_value*>((SCHAR *) request + *ptr)->vlu_flags = 0;
		}
	}

	while (true) {
		IRSB_INDEX impure = (IRSB_INDEX) ((SCHAR *) request + rsb->rsb_impure);
		impure->irsb_flags |= irsb_first | irsb_open;
		impure->irsb_flags &=
			~(irsb_singular_processed | irsb_checking_singular);
		record_param* rpb = &request->req_rpb[rsb->rsb_stream];
		rpb->rpb_window.win_flags = 0;

		switch (rsb->rsb_type) {
		case rsb_indexed:
			impure->irsb_bitmap = EVL_bitmap(tdbb, (jrd_nod*) rsb->rsb_arg[0]);
			impure->irsb_prefetch_number = -1;

		case rsb_navigate:
		case rsb_sequential:
#ifdef SCROLLABLE_CURSORS
			if (rsb->rsb_type == rsb_navigate) {
				impure->irsb_flags |= irsb_bof;
				impure->irsb_flags &= ~irsb_eof;
			}
#endif
			if (rsb->rsb_type == rsb_sequential) {
				Database* dbb = tdbb->tdbb_database;
				BufferControl* bcb = dbb->dbb_bcb;

				/* Unless this is the only attachment, limit the cache flushing
				   effect of large sequential scans on the page working sets of
				   other attachments. */

				Attachment* attachment = tdbb->tdbb_attachment;
				if (attachment &&
					(attachment != dbb->dbb_attachments
					 || attachment->att_next))
				{
					/* If the relation has more data pages than the number of
					   pages in the buffer cache then mark the input window
					   block as a large scan so that a data page is released
					   to the LRU tail after its last record is fetched.

					   A database backup treats everything as a large scan
					   because the cumulative effect of scanning all relations
					   is equal to that of a single large relation. */

					if (attachment->att_flags & ATT_gbak_attachment ||
						DPM_data_pages(tdbb,
									   rpb->rpb_relation) > (SLONG) bcb->bcb_count)
					{
						rpb->rpb_window.win_flags = WIN_large_scan;
						rpb->rpb_org_scans =
							rpb->rpb_relation->rel_scan_count++;
					}
				}
			}

			RLCK_reserve_relation(tdbb, request->req_transaction,
								  rpb->rpb_relation, false, true);

			rpb->rpb_number.setValue(BOF_NUMBER);
			return;

		case rsb_cross:
			return;

		case rsb_sort:
#ifdef SCROLLABLE_CURSORS
			impure->irsb_flags |= irsb_bof;
			impure->irsb_flags &= ~irsb_eof;
#endif
			// dimitr:	we can avoid reading and sorting the entire
			//			record set, if there's actually nothing to return
			if (first_records) {
				open_sort(tdbb, rsb, (IRSB_SORT) impure,
					(first_records < 0) ? 0 : (UINT64) first_records + skip_records);
			}
			else {
				((IRSB_SORT) impure)->irsb_sort_handle = NULL;
			}
			return;

		case rsb_procedure:
			open_procedure(tdbb, rsb, (IRSB_PROCEDURE) impure);
			return;

        case rsb_first:
			{
				const dsc* desc = EVL_expr(tdbb, (jrd_nod*) rsb->rsb_arg[0]);
				first_records = (desc && !(request->req_flags & req_null)) ?
					MOV_get_int64(desc, 0) : 0;

	            if (first_records < 0)
		            ERR_post(isc_bad_limit_param, 0);

				((IRSB_FIRST) impure)->irsb_count = first_records;
				rsb = rsb->rsb_next;
			}
            break;

        case rsb_skip:
			{
				const dsc* desc = EVL_expr(tdbb, (jrd_nod*) rsb->rsb_arg[0]);
				skip_records = (desc && !(request->req_flags & req_null)) ?
					MOV_get_int64(desc, 0) : 0;

	            if (skip_records < 0)
		            ERR_post(isc_bad_skip_param, 0);

				((IRSB_SKIP) impure)->irsb_count = skip_records + 1;
				rsb = rsb->rsb_next;
			}
            break;

		case rsb_boolean:
			rsb = rsb->rsb_next;
			break;

		case rsb_union:
			{
				((IRSB) impure)->irsb_count = 0;
				VIO_record(tdbb, rpb, rsb->rsb_format, tdbb->getDefaultPool());

				/* Initialize the record number of each stream in the union */

				RecordSource** ptr = &rsb->rsb_arg[rsb->rsb_count];
				for (const RecordSource* const* const end = ptr + (USHORT)(IPTR) *ptr;
					++ptr <= end;)
				{
					request->req_rpb[(USHORT)(IPTR) *ptr].rpb_number.setValue(BOF_NUMBER);
				}

				rsb = rsb->rsb_arg[0];
			}
			break;

		case rsb_aggregate:
			((IRSB) impure)->irsb_count = 3;
			VIO_record(tdbb, rpb, rsb->rsb_format, tdbb->getDefaultPool());
			return;

		case rsb_merge:
			open_merge(tdbb, rsb, (IRSB_MRG) impure);
			return;

		case rsb_ext_sequential:
		case rsb_ext_indexed:
		case rsb_ext_dbkey:
			EXT_open(rsb);
			return;

		case rsb_left_cross:
			{
				RSE_open(tdbb, rsb->rsb_arg[RSB_LEFT_outer]);
				impure->irsb_flags &=
					~(irsb_first | irsb_in_opened | irsb_join_full);
				impure->irsb_flags |= irsb_mustread;

				/* Allocate a record block for each union/aggregate/procedure
				   stream in the right sub-stream.  The block will be needed
				   if we join to nulls before opening the rsbs */

				for (RsbStack::iterator stack(*(rsb->rsb_left_rsbs));
					stack.hasData(); ++stack)
				{
					VIO_record(tdbb,
							   &request->req_rpb[stack.object()->rsb_stream],
							   stack.object()->rsb_format, tdbb->getDefaultPool());
				}
				return;
			}

		default:
			BUGCHECK(166);		/* msg 166 invalid rsb type */
		}
	}
}


#ifdef PC_ENGINE
bool RSE_reset_position(thread_db* tdbb, RecordSource* rsb, record_param* new_rpb)
{
/**************************************
 *
 *	R S E _ r e s e t _ p o s i t i o n
 *
 **************************************
 *
 * Functional description
 *	Reset the position of a navigational stream to
 *	the position indicated by the passed record.
 *
 **************************************/
	record_param* rpb;
	IRSB_INDEX impure;
	RecordBitmap* bitmap;

	SET_TDBB(tdbb);
	jrd_req* request = tdbb->tdbb_request;
	if (request->req_flags & req_abort)
		return false;

	switch (rsb->rsb_type) {
	case rsb_boolean:
		return RSE_reset_position(rsb->rsb_next, new_rpb);

	case rsb_navigate:
		RSE_MARK_CRACK(rsb, 0);
		if (!(NAV_reset_position(rsb, new_rpb))) {
			RSE_MARK_CRACK(rsb, irsb_crack);
			return false;
		}
		return true;

	case rsb_sequential:
		RSE_MARK_CRACK(rsb, 0);
		rpb = request->req_rpb + rsb->rsb_stream;
		rpb->rpb_number = new_rpb->rpb_number;

		if (!
			(VIO_get
			 (tdbb, rpb, rsb, request->req_transaction, request->req_pool)))
		{
			RSE_MARK_CRACK(rsb, irsb_crack);
			return false;
		}
		return true;

	case rsb_indexed:
		RSE_MARK_CRACK(rsb, 0);
		rpb = request->req_rpb + rsb->rsb_stream;
		rpb->rpb_number = new_rpb->rpb_number;

		impure = (IRSB_INDEX) ((UCHAR *) request + rsb->rsb_impure);
		if ((bitmap = impure->irsb_bitmap) &&
			SBM_next(*bitmap, &rpb->rpb_number, RSE_get_current) &&
			VIO_get(tdbb, rpb, rsb, request->req_transaction,
					request->req_pool))
		{
			return true;
		}

		RSE_MARK_CRACK(rsb, irsb_crack);
		return false;

	default:
		BUGCHECK(166);			/* msg 166 invalid rsb type */
		return false;			/* Added to remove compiler warnings */
	}
}
#endif


#ifdef PC_ENGINE
bool RSE_set_bookmark(thread_db* tdbb, RecordSource* rsb, record_param* rpb, Bookmark* bookmark)
{
/**************************************
 *
 *	R S E _ s e t _ b o o k m a r k
 *
 **************************************
 *
 * Functional description
 *	Set the location of a stream to the location
 *	specified by the given bookmark.
 *
 **************************************/
	SET_TDBB(tdbb);
	jrd_req* request = tdbb->tdbb_request;
	if (request->req_flags & req_abort)
		return false;

	IRSB impure = (IRSB) ((UCHAR *) request + rsb->rsb_impure);

	switch (rsb->rsb_type) {
	case rsb_boolean:
		return RSE_set_bookmark(rsb->rsb_next, rpb, bookmark);

	case rsb_navigate:
		return NAV_set_bookmark(rsb, impure, rpb, bookmark);

	case rsb_sequential:
	case rsb_indexed:
		rpb->rpb_number = bookmark->bkm_number;

		if (impure->irsb_flags & (irsb_bof | irsb_eof | irsb_crack))
			return false;

		if (!(get_record(tdbb, rsb, NULL, RSE_get_current))) {
			RSE_MARK_CRACK(rsb, irsb_crack);
			return false;
		}
		return true;

	default:
		BUGCHECK(166);			/* msg 166 invalid rsb type */
		return false;			/* Added to remove compiler warnings */
	}
}
#endif


static void close_merge(thread_db* tdbb, RecordSource* rsb, IRSB_MRG impure)
{
/**************************************
 *
 *	c l o s e _ m e r g e
 *
 **************************************
 *
 * Functional description
 *	Close a merge stream by closing all substreams
 *	and cleaning up after any prior sort-merge retrieval.
 *
 **************************************/
	SET_TDBB(tdbb);

/* do two simultaneous but unrelated things in one loop */
	irsb_mrg::irsb_mrg_repeat* tail = impure->irsb_mrg_rpt;
	RecordSource** ptr = rsb->rsb_arg;
	for (const RecordSource* const* const end = ptr + rsb->rsb_count * 2;
		ptr < end; ptr += 2, tail++)
	{
		/* close all the substreams for the sort-merge */

		RSE_close(tdbb, *ptr);

		/* Release memory associated with the merge file block
		   and the sort file block. Also delete the merge file
		   if one exists. */

		merge_file* mfb = &tail->irsb_mrg_file;
		sort_work_file* sfb = mfb->mfb_sfb;
		if (sfb) {
			if (sfb->sfb_file_name) {
				close(sfb->sfb_file);
				unlink(sfb->sfb_file_name);
				delete[] sfb->sfb_file_name;
			}
			delete sfb->sfb_mem;
			delete sfb;
			mfb->mfb_sfb = 0;
		}
		if (mfb->mfb_block_data) {
			delete[] mfb->mfb_block_data;
			mfb->mfb_block_data = 0;
		}
	}
}


static void close_procedure(thread_db* tdbb, RecordSource* rsb)
{
/**************************************
 *
 *	c l o s e _ p r o c e d u r e
 *
 **************************************
 *
 * Functional description
 *	Shut down a procedural view.
 *
 **************************************/
	SET_TDBB(tdbb);

	jrd_req* request = tdbb->tdbb_request;
	IRSB_PROCEDURE impure = (IRSB_PROCEDURE) ((UCHAR *) request + rsb->rsb_impure);
	jrd_req* proc_request = impure->irsb_req_handle;
	if (proc_request) {
		EXE_unwind(tdbb, proc_request);
		proc_request->req_flags &= ~req_in_use;
		impure->irsb_req_handle = 0;
		proc_request->req_attachment = NULL;
	}

	if (impure->irsb_message) {
		delete impure->irsb_message;
		impure->irsb_message = NULL;
	}
}


// It seems strange this function doesn't take two const nodes,
// but EVL_expr can change node1 and node2' args.
static SSHORT compare(thread_db* tdbb, jrd_nod* node1, jrd_nod* node2)
{
/**************************************
 *
 *	c o m p a r e
 *
 **************************************
 *
 * Functional description
 *	Compare values of two sorts lists, return result.  This is used for
 *	sort merge join.
 *
 **************************************/
	SET_TDBB(tdbb);

	jrd_req* request = tdbb->tdbb_request;
	jrd_nod* const* ptr1 = node1->nod_arg;
	jrd_nod* const* ptr2 = node2->nod_arg;
	for (const jrd_nod* const* const end = ptr1 + node1->nod_count;
		ptr1 < end; ptr1++, ptr2++)
	{
		const dsc* desc1 = EVL_expr(tdbb, *ptr1);
		const ULONG flags = request->req_flags;
		const dsc* desc2 = EVL_expr(tdbb, *ptr2);
		if (flags & req_null) {
			if (!(request->req_flags & req_null)) {
				return -1;
			}
			else {
				// AB: When both expression evaluated NULL then
				// we return 0 ( (NULL = NULL) = true).
				//
				// Currently this (0 and higher) isn't used by the
				// MERGE procedure, but when we allow MERGE to
				// handle outer-joins we must not forget this one !!!
				return 0;
			}
		}
		else if (request->req_flags & req_null) {
			return 1;
		}
		// AB: MOV_compare can't handle NULL parameters
		// therefore check before passing all null flags.
		const SSHORT result = MOV_compare(desc1, desc2);
		if (result != 0) {
			return result;
		}
	}

	return 0;
}


static SSHORT compare_longs(const SLONG* p, const SLONG* q, USHORT count)
{
/**************************************
 *
 *	c o m p a r e _ l o n g s
 *
 **************************************
 *
 * Functional description
 *	Compare two strings of SLONG words.
 *
 **************************************/
	for (; count; p++, q++, --count)
		if (*p > *q)
			return 1;
		else if (*p < *q)
			return -1;

	return 0;
}


static bool fetch_record(thread_db* tdbb, RecordSource* rsb, SSHORT n
#ifdef SCROLLABLE_CURSORS
							, RSE_GET_MODE mode
#endif
	)
{
/**************************************
 *
 *	f e t c h _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Get a record for a sub-stream of a cross.  If the
 *	sub-stream is dry, close it and recurse to get the
 *	next record from the previous stream.  If there isn't
 *	a previous stream, then the cross is finished, so
 *	return false.
 *
 **************************************/
	SET_TDBB(tdbb);

	RecordSource* sub_rsb = rsb->rsb_arg[n];

	if (get_record(tdbb, sub_rsb, NULL
#ifdef SCROLLABLE_CURSORS
				   , mode
#else
				   , RSE_get_forward
#endif
		))
	{
		return true;
	}

/* we have exhausted this stream, so close it; if there is
   another candidate record from the n-1 streams to the left,
   then reopen the stream and start again from the beginning */

	while (true)
	{
		RSE_close(tdbb, sub_rsb);
		if (n == 0 || !fetch_record(tdbb, rsb, n - 1
#ifdef SCROLLABLE_CURSORS
									, mode
#endif
			))
		{
			return false;
		}
		RSE_open(tdbb, sub_rsb);

		if (get_record(tdbb, sub_rsb, NULL
#ifdef SCROLLABLE_CURSORS
					   , mode
#else
					   , RSE_get_forward
#endif
			))
		{
			return true;
		}
	}
}


#ifdef SCROLLABLE_CURSORS
static bool fetch_left(thread_db* tdbb, RecordSource* rsb, IRSB impure, RSE_GET_MODE mode)
{
/**************************************
 *
 *	f e t c h _ l e f t ( I B _ V 4 _ 1 )
 *
 **************************************
 *
 * Functional description
 *	Get records for a left outer join.  Records are read
 *	from the left sub-stream when the right sub-stream is
 *	dry or when it is not yet open.  When the left
 *	sub-stream's boolean is true, open the right sub-stream
 *	and read a record.  When the right sub-stream becomes dry,
 *	close it, and if nothing has been joined to the left
 *	sub-stream's current record, join a null valued right
 *	sub-stream record.  When the left sub-stream is dry,
 *	the outer join is finished, so return false.
 *
 **************************************/
	SET_TDBB(tdbb);

/* loop through the outer join in either the forward or the backward direction;
   the various modes indicate what state of the join we are in */

	while (true)
	{
		if (!(impure->irsb_flags & irsb_join_full))
		{
			/* mustread indicates to get the next record from the outer stream */

			if (impure->irsb_flags & irsb_mustread)
			{
				if (!get_record(tdbb, rsb->rsb_arg[RSB_LEFT_outer], NULL, mode))
				{
					if (mode == RSE_get_backward)
						return false;
					else
					{
						if (!rsb->rsb_arg[RSB_LEFT_inner_streams])
							return false;
					}

					/* We have a full outer join.  Open up the inner stream
					   one more time. */

					RSE_close(tdbb, rsb->rsb_arg[RSB_LEFT_outer]);
					impure->irsb_flags |= irsb_join_full;
					RSE_open(tdbb, rsb->rsb_arg[RSB_LEFT_inner]);
					continue;
				}

				/* check if the outer record qualifies for the boolean */

				if (rsb->rsb_arg[RSB_LEFT_boolean] &&
					!EVL_boolean(tdbb, rsb->rsb_arg[RSB_LEFT_boolean]))
				{
					/* The boolean pertaining to the left sub-stream is false
					   so just join sub-stream to a null valued right sub-stream */
					join_to_nulls(tdbb, rsb, rsb->rsb_left_streams);
					return true;
				}

				impure->irsb_flags &= ~(irsb_mustread | irsb_joined);
				impure->irsb_flags |= irsb_in_opened;
				RSE_open(tdbb, rsb->rsb_arg[RSB_LEFT_inner]);
			}

			/* fetch records from the inner stream until exhausted */

			while (get_record(tdbb, rsb->rsb_arg[RSB_LEFT_inner], NULL, mode))
			{
				if (!rsb->rsb_arg[RSB_LEFT_inner_boolean] ||
					EVL_boolean(tdbb, rsb->rsb_arg[RSB_LEFT_inner_boolean]))
				{
					impure->irsb_flags |= irsb_joined;
					return true;
				}
			}

			/* out of inner records, go back to reading the next outer record */

			RSE_close(tdbb, rsb->rsb_arg[RSB_LEFT_inner]);
			impure->irsb_flags |= irsb_mustread;

			/* The right stream did not have any matching records.  Join
			   the left stream to a null valued right sub-stream */

			if (!(impure->irsb_flags & irsb_joined)) {
				join_to_nulls(tdbb, rsb, rsb->rsb_left_streams);
				return true;
			}
		}
		else
		{
			/* Continue with a full outer join. */

			RecordSource* full = rsb->rsb_arg[RSB_LEFT_inner];
			full = (full->rsb_type == rsb_boolean) ? full->rsb_next : full;

			if (impure->irsb_flags & irsb_in_opened)
			{
				/* The inner stream was opened at some point.  If it doesn't have a
				   boolean, then all of its records have been returned.  Otherwise,
				   find the records that haven't been. */
				bool found;
				do {
					if (!get_record(tdbb, full, NULL, mode)) {
						if (mode == RSE_get_forward)
							return false;
						else
							goto return_to_outer;
					}

					RSE_open(tdbb, rsb->rsb_arg[RSB_LEFT_outer]);
					while (found =
						   get_record(tdbb, rsb->rsb_arg[RSB_LEFT_outer],
									  NULL, mode))
					{
							if (
								(!rsb->rsb_arg[RSB_LEFT_boolean]
								 || EVL_boolean(tdbb,
												rsb->
												rsb_arg[RSB_LEFT_boolean]))
								&& (!rsb->rsb_arg[RSB_LEFT_inner_boolean]
									|| EVL_boolean(tdbb,
												   rsb->rsb_arg
												   [RSB_LEFT_inner_boolean]))
								&& (full == rsb->rsb_arg[RSB_LEFT_inner]
									|| EVL_boolean(tdbb,
												   rsb->
												   rsb_arg
												   [RSB_LEFT_inner]->rsb_arg
												   [0])))
							{
								break;
							}
					}
					RSE_close(tdbb, rsb->rsb_arg[RSB_LEFT_outer]);
				} while (found);
			}
			else if (!get_record(tdbb, full, NULL, mode))
			{
				if (mode == RSE_get_forward)
					return false;
				else
					goto return_to_outer;
			}

			join_to_nulls(tdbb, rsb, rsb->rsb_left_inner_streams);
			return true;

return_to_outer:
			impure->irsb_flags &= ~(irsb_join_full | irsb_in_opened);
			impure->irsb_flags |= irsb_mustread;
			RSE_close(tdbb, rsb->rsb_arg[RSB_LEFT_inner]);
			RSE_close(tdbb, rsb->rsb_arg[RSB_LEFT_outer]);
			RSE_open(tdbb, rsb->rsb_arg[RSB_LEFT_outer]);
		}
	}

/* NOTREACHED */
	return true;
}
#else


static bool fetch_left(thread_db* tdbb, RecordSource* rsb, IRSB impure)
{
/**************************************
 *
 *	f e t c h _ l e f t
 *
 **************************************
 *
 * Functional description
 *	Get records for a left outer join.  Records are read
 *	from the left sub-stream when the right sub-stream is
 *	dry or when it is not yet open.  When the left
 *	sub-stream's boolean is true, open the right sub-stream
 *	and read a record.  When the right sub-stream becomes dry,
 *	close it, and if nothing has been joined to the left
 *	sub-stream's current record, join a null valued right
 *	sub-stream record.  When the left sub-stream is dry,
 *	the outer join is finished, so return false.
 *
 **************************************/
	SET_TDBB(tdbb);

	if (!(impure->irsb_flags & irsb_join_full))
	{
		while (true)
		{
			if (impure->irsb_flags & irsb_mustread)
			{
				if (!get_record
					(tdbb, rsb->rsb_arg[RSB_LEFT_outer], NULL,
					 RSE_get_forward))
				{
					if (rsb->rsb_left_inner_streams->isEmpty())
						return false;

					/* We have a full outer join.  Open up the inner stream
					   one more time. */

					RSE_close(tdbb, rsb->rsb_arg[RSB_LEFT_outer]);
					impure->irsb_flags |= irsb_join_full;
					RSE_open(tdbb, rsb->rsb_arg[RSB_LEFT_inner]);
					break;
				}
				if (rsb->rsb_arg[RSB_LEFT_boolean] &&
					!EVL_boolean(tdbb, (jrd_nod*) rsb->rsb_arg[RSB_LEFT_boolean]))
				{
					/* The boolean pertaining to the left sub-stream is false
					   so just join sub-stream to a null valued right sub-stream */
					join_to_nulls(tdbb, rsb, rsb->rsb_left_streams);
					return true;
				}
				impure->irsb_flags &= ~(irsb_mustread | irsb_joined);
				impure->irsb_flags |= irsb_in_opened;
				RSE_open(tdbb, rsb->rsb_arg[RSB_LEFT_inner]);
			}

			while (get_record(tdbb, rsb->rsb_arg[RSB_LEFT_inner], NULL,
					RSE_get_forward))
			{
				if (!rsb->rsb_arg[RSB_LEFT_inner_boolean]
						|| EVL_boolean(tdbb,
									   (jrd_nod*)
									   rsb->rsb_arg[RSB_LEFT_inner_boolean]))
				{
					impure->irsb_flags |= irsb_joined;
					return true;
				}
			}

			RSE_close(tdbb, rsb->rsb_arg[RSB_LEFT_inner]);
			impure->irsb_flags |= irsb_mustread;
			if (!(impure->irsb_flags & irsb_joined))
			{
				/* The current left sub-stream record has not been joined
				   to anything.  Join it to a null valued right sub-stream */
				join_to_nulls(tdbb, rsb, rsb->rsb_left_streams);
				return true;
			}
		}
	}

/* Continue with a full outer join. */

	RecordSource* full = rsb->rsb_arg[RSB_LEFT_inner];
	full = (full->rsb_type == rsb_boolean) ? full->rsb_next : full;

	if (impure->irsb_flags & irsb_in_opened)
	{
		/* The inner stream was opened at some point.  If it doesn't have a
		   boolean, then all of its records have been returned.  Otherwise,
		   find the records that haven't been. */
		bool found;
		do {
			if (!get_record(tdbb, full, NULL, RSE_get_forward))
				return false;
			RSE_open(tdbb, rsb->rsb_arg[RSB_LEFT_outer]);
			while ( (found =
				   get_record(tdbb, rsb->rsb_arg[RSB_LEFT_outer], NULL,
							  RSE_get_forward)) )
			{
				if (
					(!rsb->rsb_arg[RSB_LEFT_boolean]
					 || EVL_boolean(tdbb,
									(jrd_nod*) rsb->rsb_arg[RSB_LEFT_boolean]))
					&& (!rsb->rsb_arg[RSB_LEFT_inner_boolean]
						|| EVL_boolean(tdbb,
									   (jrd_nod*)
									   rsb->rsb_arg
									   [RSB_LEFT_inner_boolean]))
					&& (full == rsb->rsb_arg[RSB_LEFT_inner]
						|| EVL_boolean(tdbb,
									   (jrd_nod*)
									   rsb->rsb_arg
									   [RSB_LEFT_inner]->rsb_arg[0])))
				{
					break;
				}
			}
			RSE_close(tdbb, rsb->rsb_arg[RSB_LEFT_outer]);
		} while (found);
	}
	else if (!get_record(tdbb, full, NULL, RSE_get_forward))
		return false;

	join_to_nulls(tdbb, rsb, rsb->rsb_left_inner_streams);

	return true;
}
#endif


static UCHAR *get_merge_data(thread_db* tdbb, merge_file* mfb, SLONG record)
{
/**************************************
 *
 *	g e t _ m e r g e _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Get merge data for record from merge
 *	equivalence file.
 *
 **************************************/
	fb_assert(record >= 0 && record < (SLONG) mfb->mfb_equal_records);

	const ULONG merge_block = record / mfb->mfb_blocking_factor;
	if (merge_block != mfb->mfb_current_block) {
		mfb->mfb_current_block = read_merge_block(tdbb, mfb, merge_block);
	}

	const ULONG merge_offset = (record % mfb->mfb_blocking_factor) * mfb->mfb_record_size;
	return (mfb->mfb_block_data + merge_offset);
}


#ifdef SCROLLABLE_CURSORS
static bool get_merge_fetch(
							   thread_db* tdbb,
							   RecordSource* rsb, SSHORT stream, RSE_GET_MODE mode)
{
/**************************************
 *
 *	g e t _ m e r g e _ f e t c h ( I B _ V 4 _ 1 )
 *
 **************************************
 *
 * Functional description
 *	Cycle through all the records of equal key
 *	in all the substreams of a sort-merge.
 *
 **************************************/
	SET_TDBB(tdbb);

	irsb_mrg* impure = (IRSB_MRG) ((UCHAR *) tdbb->tdbb_request + rsb->rsb_impure);
	irsb_mrg::irsb_mrg_repeat* tail = impure->irsb_mrg_rpt + stream;
	const SSHORT m = tail->irsb_mrg_order;
	tail = impure->irsb_mrg_rpt + m;
	RecordSource* sub_rsb = rsb->rsb_arg[2 * m];

/* go forward or backward to the next record */

	SLONG record = tail->irsb_mrg_equal_current;

	if (((mode == RSE_get_backward) && !(impure->irsb_flags & irsb_backwards))
		|| ((mode == RSE_get_forward)
			&& (impure->irsb_flags & irsb_backwards)))
	{
		--record;
	}
	else
		++record;
//} CVC: Misplaced ending brace

/* if there is a record waiting for us, use it;
   otherwise proceed recursively from right to left
   through the substreams, getting the next record
   in the equality group */

	if (record < tail->irsb_mrg_equal || record > tail->irsb_mrg_equal_end) {
		/* if we get to the first stream and there is no next record,
		   we have exhausted the equality group, so return */

		if (stream == 0) {
			/* in the case where we are stepping off the end of the equivalence
			   group in a direction other than the direction in which we entered
			   it, we have a problem; the sort streams are positioned at the other
			   end of the equivalence group, so we need to resynch them */

			if (((mode == RSE_get_backward)
				 && !(impure->irsb_flags & irsb_backwards))
				|| ((mode == RSE_get_forward)
					&& (impure->irsb_flags & irsb_backwards)))
			{
				resynch_merge(tdbb, rsb, impure, mode);
			}

			return false;
		}

		/* we have exhausted the equivalent records in this stream,
		   so get the next record in the next leftmost stream */

		if (!get_merge_fetch(tdbb, rsb, stream - 1, mode))
			return false;

		/* we are incrementing (or decrementing) the next leftmost stream,
		   so we need to start at the beginning (or end) of this stream
		   (and recursively all the streams to the right of this one) */

		if (((mode == RSE_get_backward) && !(impure->irsb_flags & irsb_backwards))
			|| ((mode == RSE_get_forward)
				&& (impure->irsb_flags & irsb_backwards)))
		{
			record = tail->irsb_mrg_equal_end;
		}
		else
			record = tail->irsb_mrg_equal;
	}

	/* indicate the last record fetched */

	tail->irsb_mrg_equal_current = record;

	/* copy out the data from this sort merge block to the
	current record in the record parameter block */

	merge_file* mfb = &tail->irsb_mrg_file;
	map_sort_data(tdbb->tdbb_request, (SortMap*) sub_rsb->rsb_arg[0],
			  get_merge_data(tdbb, mfb, record));

	return true;
}
#else
static bool get_merge_fetch(thread_db* tdbb, RecordSource* rsb, SSHORT stream)
{
/**************************************
 *
 *	g e t _ m e r g e _ f e t c h
 *
 **************************************
 *
 * Functional description
 *	Get the next record from a stream within a sort/merge
 *	record group.
 *
 **************************************/
	SET_TDBB(tdbb);

	IRSB_MRG impure = (IRSB_MRG) ((UCHAR *) tdbb->tdbb_request + rsb->rsb_impure);
	irsb_mrg::irsb_mrg_repeat* tail = impure->irsb_mrg_rpt + stream;
	const SSHORT m = tail->irsb_mrg_order;
	tail = impure->irsb_mrg_rpt + m;
	RecordSource* sub_rsb = rsb->rsb_arg[m + m];

	SLONG record = tail->irsb_mrg_equal_current;
	++record;

	if (record > tail->irsb_mrg_equal_end) {
		if (stream == 0 || !get_merge_fetch(tdbb, rsb, stream - 1))
			return false;
		record = tail->irsb_mrg_equal;
	}

	tail->irsb_mrg_equal_current = record;

	merge_file* mfb = &tail->irsb_mrg_file;
	map_sort_data(tdbb->tdbb_request, (SortMap*) sub_rsb->rsb_arg[0],
				  get_merge_data(tdbb, mfb, record));

	return true;
}
#endif

#ifdef SCROLLABLE_CURSORS
static bool get_merge_join(
							  thread_db* tdbb,
							  RecordSource* rsb, IRSB_MRG impure, RSE_GET_MODE mode)
{
/**************************************
 *
 *	g e t _ m e r g e _ j o i n ( I B _ V 4 _ 1 )
 *
 **************************************
 *
 * Functional description
 *	Get the next tuple from a sort/merge join.
 *
 **************************************/
	SET_TDBB(tdbb);

	jrd_req* request = tdbb->tdbb_request;
	const RecordSource* const* const end = rsb->rsb_arg + rsb->rsb_count * 2;

/* If there is a group of equivalent records already formed,
   fetch the next record from it */

	if (get_merge_fetch(tdbb, rsb, rsb->rsb_count - 1, mode))
		return true;

/* We are done with the current equivalence group,
   so we need to generate a new one.  If backwards
   is specified, we will fetch all the records in
   the backward direction, so the backwards flag indicates
   that all the equivalent records were fetched in the
   backward direction.  This will effectively reverse the
   direction we traverse the equivalent records in response
   to a scroll request. */

	if (mode == RSE_get_backward)
		impure->irsb_flags |= irsb_backwards;
	else
		impure->irsb_flags &= ~irsb_backwards;

/* Increment (or decrement) each stream one record.  If any comes
   up dry, we are at the end. */

	RecordSource** ptr;
	RecordSource** highest_ptr = rsb->rsb_arg;
	irsb_mrg::irsb_mrg_repeat* tail;
	for (ptr = rsb->rsb_arg, tail = impure->irsb_mrg_rpt; ptr < end;
		 ptr += 2, tail++)
	{
		RecordSource* sort_rsb = *ptr;
		SortMap* map = (SortMap*) sort_rsb->rsb_arg[0];
		merge_file* mfb = &tail->irsb_mrg_file;

		/* reset equality group record positions */

		tail->irsb_mrg_equal = 0;
		tail->irsb_mrg_equal_current = 0;
		tail->irsb_mrg_equal_end = 0;

		/* If there is a record waiting, use it.  Otherwise get another */

		SLONG record = tail->irsb_mrg_last_fetched;
		if (record >= 0)
		{
			tail->irsb_mrg_last_fetched = -1;
			const UCHAR* last_data = get_merge_data(tdbb, mfb, record);
			mfb->mfb_current_block = 0;
			UCHAR* first_data = get_merge_data(tdbb, mfb, 0);
			if (first_data != last_data)
				MOVE_FASTER(last_data, first_data, map->smb_length);
			mfb->mfb_equal_records = 1;
			record = 0;
		}
		else
		{
			mfb->mfb_current_block = 0;
			mfb->mfb_equal_records = 0;
			if ((record = get_merge_record(tdbb, sort_rsb, tail, mode)) < 0)
				return false;
		}

		/* Map data into target records and do comparison */

		map_sort_data(request, map, get_merge_data(tdbb, mfb, record));
		const int result = compare(tdbb, highest_ptr[1], ptr[1]);
		if (ptr != highest_ptr)
		{
			if (((result > 0) && (impure->irsb_flags & irsb_backwards)) ||
				((result < 0) && ~(impure->irsb_flags & irsb_backwards)))
			{
				highest_ptr = ptr;
			}
		}
	}

/* Loop thru the streams advancing each up to (or down to) the target value.
   If any exceeds the target value, there is no match so start over. */

	for (;;)
	{
		for (ptr = rsb->rsb_arg, tail = impure->irsb_mrg_rpt;
			 ptr < end; ptr += 2, tail++)
		{
			if (highest_ptr != ptr)
			{
				int result;
				while (result = compare(tdbb, highest_ptr[1], ptr[1]))
				{
					if (((result > 0)
						 && (impure->irsb_flags & irsb_backwards))
						|| ((result < 0)
							&& ~(impure->irsb_flags & irsb_backwards)))
					{
						highest_ptr = ptr;
						goto recycle;
					}
					RecordSource* sort_rsb = *ptr;
					merge_file* mfb = &tail->irsb_mrg_file;
					mfb->mfb_current_block = 0;
					mfb->mfb_equal_records = 0;

					/* get the new record, which is both the beginning and end of
					   the equal queue for the moment */

					const SLONG record =
						get_merge_record(tdbb, sort_rsb, tail, mode);
					if (record < 0)
						return false;
					map_sort_data(request, (SortMap*) sort_rsb->rsb_arg[0],
								  get_merge_data(tdbb, mfb, record));
				}
			}
		}

		break;
	  recycle:;
	}

/* Finally compute equality group for each stream in sort/merge */

	for (ptr = rsb->rsb_arg, tail = impure->irsb_mrg_rpt; ptr < end;
		 ptr += 2, tail++)
	{
		ULONG key[32];

		RecordSource* sort_rsb = *ptr;
		SortMap* map = (SortMap*) sort_rsb->rsb_arg[0];
		merge_file* mfb = &tail->irsb_mrg_file;
		const ULONG key_length = map->smb_key_length * sizeof(ULONG);
		UCHAR* first_data;
		if (key_length > sizeof(key))
			first_data = FB_NEW(*tdbb->getDefaultPool()) UCHAR[key_length];
		else
			first_data = (UCHAR *) key;
		MOVE_FASTER(get_merge_data(tdbb, mfb, 0), first_data, key_length);

		SLONG record;
		while ((record = get_merge_record(tdbb, sort_rsb, tail, mode)) >= 0)
		{
			if (compare_longs((const SLONG*) first_data,
							  (const SLONG*) get_merge_data(tdbb, mfb, record),
							  map->smb_key_length))
			{
				tail->irsb_mrg_last_fetched = record;
				break;
			}

			tail->irsb_mrg_equal_end = record;
		}

		if (first_data != (UCHAR *) key)
			delete[] first_data;
		if (mfb->mfb_current_block)
			write_merge_block(tdbb, mfb, mfb->mfb_current_block);
	}

/* Optimize cross product of equivalence groups by ordering the streams
   from left (outermost) to right (innermost) by descending cardinality
   of merge blocks. This ordering will vary for each set of equivalence
   groups and cannot be statically assigned by the optimizer. */

	lls* best_tails = 0;

	tail = impure->irsb_mrg_rpt;
	for (const irsb_mrg::irsb_mrg_repeat* const tail_end = tail + rsb->rsb_count;
		 tail < tail_end; tail++)
	{
		ULONG most_blocks = 0;
		irsb_mrg::irsb_mrg_repeat* best_tail = 0;
		for (irsb_mrg::irsb_mrg_repeat* tail2 = impure->irsb_mrg_rpt;
			tail2 < tail_end; tail2++)
		{
			lls* stack;
			for (stack = best_tails; stack; stack = stack->lls_next)
			{
				if (stack->lls_object == (BLK) tail2)
				{
					break;
				}
			}
			if (stack) {
				continue;
			}
			merge_file* mfb = &tail2->irsb_mrg_file;
			ULONG blocks = mfb->mfb_equal_records / mfb->mfb_blocking_factor;
			if (++blocks > most_blocks) {
				most_blocks = blocks;
				best_tail = tail2;
			}
		}

		LLS_PUSH(best_tail, &best_tails);
		tail->irsb_mrg_order = best_tail - impure->irsb_mrg_rpt;
	}

	while (best_tails) {
		LLS_POP(&best_tails);
	}

	return true;
}
#else
static bool get_merge_join(thread_db* tdbb, RecordSource* rsb, IRSB_MRG impure)
{
/**************************************
 *
 *	g e t _ m e r g e _ j o i n
 *
 **************************************
 *
 * Functional description
 *	Get the next tuple from a sort/merge join.
 *
 **************************************/
	SET_TDBB(tdbb);

	jrd_req* request = tdbb->tdbb_request;
	const RecordSource* const* const end = rsb->rsb_arg + rsb->rsb_count * 2;

/* If there is a record group already formed, fetch the next combination */

	if (get_merge_fetch(tdbb, rsb, rsb->rsb_count - 1))
		return true;

	RecordSource** ptr;
	irsb_mrg::irsb_mrg_repeat* tail;

/* Assuming we are done with the current value group, advance each
   stream one record.  If any comes up dry, we're done. */
	RecordSource** highest_ptr = rsb->rsb_arg;

	for (ptr = rsb->rsb_arg, tail = impure->irsb_mrg_rpt; ptr < end;
		 ptr += 2, tail++)
	{
		RecordSource* sort_rsb = *ptr;
		SortMap* map = (SortMap*) sort_rsb->rsb_arg[0];
		merge_file* mfb = &tail->irsb_mrg_file;

		/* reset equality group record positions */

		tail->irsb_mrg_equal = 0;
		tail->irsb_mrg_equal_current = 0;
		tail->irsb_mrg_equal_end = 0;

		/* If there is a record waiting, use it.  Otherwise get another */

		SLONG record = tail->irsb_mrg_last_fetched;
		if (record >= 0) {
			tail->irsb_mrg_last_fetched = -1;
			const UCHAR* last_data = get_merge_data(tdbb, mfb, record);
			mfb->mfb_current_block = 0;
			UCHAR* first_data = get_merge_data(tdbb, mfb, 0);
			if (first_data != last_data)
				MOVE_FASTER(last_data, first_data, map->smb_length);
			mfb->mfb_equal_records = 1;
			record = 0;
		}
		else {
			mfb->mfb_current_block = 0;
			mfb->mfb_equal_records = 0;
			if ((record = get_merge_record(tdbb, sort_rsb, tail)) < 0)
				return false;
		}

		/* Map data into target records and do comparison */

		map_sort_data(request, map, get_merge_data(tdbb, mfb, record));
		if (ptr != highest_ptr &&
			compare(tdbb, (jrd_nod*) highest_ptr[1], (jrd_nod*) ptr[1]) < 0)
		{
			highest_ptr = ptr;
		}
	}

/* Loop thru the streams advancing each up to the target value.  If any
   exceeds the target value, start over */

	for (;;)
	{
		for (ptr = rsb->rsb_arg, tail = impure->irsb_mrg_rpt;
			 ptr < end; ptr += 2, tail++)
		{
			if (highest_ptr != ptr)
			{
				int result;
				while ( (result =
					   compare(tdbb, (jrd_nod*) highest_ptr[1], (jrd_nod*) ptr[1])) )
				{
					if (result < 0) {
						highest_ptr = ptr;
						goto recycle;
					}
					RecordSource* sort_rsb = *ptr;
					merge_file* mfb = &tail->irsb_mrg_file;
					mfb->mfb_current_block = 0;
					mfb->mfb_equal_records = 0;

					const SLONG record = get_merge_record(tdbb, sort_rsb, tail);
					if (record < 0)
						return false;
					map_sort_data(request, (SortMap*) sort_rsb->rsb_arg[0],
								  get_merge_data(tdbb, mfb, record));
				}
			}
		}
		break;
	  recycle:;
	}

/* Finally compute equality group for each stream in sort/merge */

	for (ptr = rsb->rsb_arg, tail = impure->irsb_mrg_rpt; ptr < end;
		 ptr += 2, tail++)
	{
		ULONG key[64];

		RecordSource* sort_rsb = *ptr;
		SortMap* map = (SortMap*) sort_rsb->rsb_arg[0];
		merge_file* mfb = &tail->irsb_mrg_file;
		const ULONG key_length = map->smb_key_length * sizeof(ULONG);
		UCHAR* first_data;
		if (key_length > sizeof(key))
			first_data = FB_NEW(*tdbb->getDefaultPool()) UCHAR[key_length];
		else
			first_data = (UCHAR *) key;
		MOVE_FASTER(get_merge_data(tdbb, mfb, 0), first_data, key_length);

		SLONG record;
		while ((record = get_merge_record(tdbb, sort_rsb, tail)) >= 0)
		{
			if (compare_longs((const SLONG*) first_data,
							  (const SLONG*) get_merge_data(tdbb, mfb, record),
							  map->smb_key_length))
			{
				tail->irsb_mrg_last_fetched = record;
				break;
			}

			tail->irsb_mrg_equal_end = record;
		}

		if (first_data != (UCHAR *) key)
			delete[] first_data;
		if (mfb->mfb_current_block)
			write_merge_block(tdbb, mfb, mfb->mfb_current_block);
	}

/* Optimize cross product of equivalence groups by ordering the streams
   from left (outermost) to right (innermost) by descending cardinality
   of merge blocks. This ordering will vary for each set of equivalence
   groups and cannot be statically assigned by the optimizer. */

	typedef Firebird::Stack<irsb_mrg::irsb_mrg_repeat*> ImrStack;
	ImrStack best_tails;

	tail = impure->irsb_mrg_rpt;
	for (const irsb_mrg::irsb_mrg_repeat* const tail_end = tail + rsb->rsb_count;
		 tail < tail_end; tail++)
	{
		irsb_mrg::irsb_mrg_repeat* best_tail = 0;

		ULONG most_blocks = 0;
		for (irsb_mrg::irsb_mrg_repeat* tail2 = impure->irsb_mrg_rpt;
			tail2 < tail_end; tail2++)
		{
			ImrStack::iterator stack(best_tails);
			for (; stack.hasData(); ++stack)
			{
				if (stack.object() == tail2)
				{
					break;
				}
			}
			if (stack.hasData()) {
				continue;
			}
			merge_file* mfb = &tail2->irsb_mrg_file;
			ULONG blocks = mfb->mfb_equal_records / mfb->mfb_blocking_factor;
			if (++blocks > most_blocks) {
				most_blocks = blocks;
				best_tail = tail2;
			}
		}

		best_tails.push(best_tail);
		tail->irsb_mrg_order = best_tail - impure->irsb_mrg_rpt;
	}

/*	while (best_tails) {
		LLS_POP(&best_tails);
	} */

	return true;
}
#endif


static SLONG get_merge_record(
							  thread_db* tdbb,
							  RecordSource* rsb, irsb_mrg::irsb_mrg_repeat * impure
#ifdef SCROLLABLE_CURSORS
, RSE_GET_MODE mode
#endif
	)
{
/**************************************
 *
 *	g e t _ m e r g e _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Get the next record from sort.  If there is one, allocate
 *	a sort/merge record block for it.
 *
 **************************************/
	SET_TDBB(tdbb);

	UCHAR* sort_data = get_sort(tdbb, rsb
#ifdef SCROLLABLE_CURSORS
							   , mode
#endif
		  );
	if (!sort_data)
		return -1;

	SortMap* map = (SortMap*) rsb->rsb_arg[0];
	merge_file* mfb = &impure->irsb_mrg_file;
	const SLONG record = mfb->mfb_equal_records;

	const ULONG merge_block = record / mfb->mfb_blocking_factor;
	if (merge_block != mfb->mfb_current_block)
	{
		write_merge_block(tdbb, mfb, mfb->mfb_current_block);
		mfb->mfb_current_block = merge_block;
	}

	const ULONG merge_offset =
		(record % mfb->mfb_blocking_factor) * mfb->mfb_record_size;
	UCHAR* merge_data = mfb->mfb_block_data + merge_offset;

	MOVE_FASTER(sort_data, merge_data, map->smb_length);
	++mfb->mfb_equal_records;

#ifdef SCROLLABLE_CURSORS
/* fix up the sort data in case we need to retrieve it again */

	unget_sort(tdbb, rsb, sort_data);
#endif

	return record;
}


static bool get_procedure(thread_db*			tdbb,
							 RecordSource*		rsb,
							 IRSB_PROCEDURE		impure,
							 record_param*		rpb)
{
/**************************************
 *
 *	g e t _ p r o c e d u r e
 *
 **************************************
 *
 * Functional description
 *	Get the next record from a procedural view.
 *
 **************************************/
	SET_TDBB(tdbb);

	jrd_prc* procedure = rsb->rsb_procedure;
	jrd_req* request = tdbb->tdbb_request;
	impure = (IRSB_PROCEDURE) ((UCHAR *) request + rsb->rsb_impure);
	jrd_req* proc_request = impure->irsb_req_handle;
	Format* rec_format = procedure->prc_format;

	const Format* msg_format = (Format*) procedure->prc_output_msg->nod_arg[e_msg_format];
	if (!impure->irsb_message)
	{
		const SLONG size = msg_format->fmt_length + ALIGNMENT;
		impure->irsb_message = FB_NEW_RPT(*tdbb->getDefaultPool(), size) VaryingString();
		impure->irsb_message->str_length = size;
	}
	UCHAR* om =
		(UCHAR *) FB_ALIGN((U_IPTR) impure->irsb_message->str_data,
						   ALIGNMENT);
	USHORT oml = impure->irsb_message->str_length - ALIGNMENT;

	Record* record;
	if (!rpb->rpb_record) {
		record = rpb->rpb_record =
			FB_NEW_RPT(*tdbb->getDefaultPool(), rec_format->fmt_length) Record(*tdbb->getDefaultPool());
		record->rec_format = rec_format;
		record->rec_length = rec_format->fmt_length;
	}
	else
		record = rpb->rpb_record;

	try {
		EXE_receive(tdbb, proc_request, 1, oml, om);
	}
	catch (const std::exception&) {
		close_procedure(tdbb, rsb);
		throw;
	}

	dsc desc = msg_format->fmt_desc[msg_format->fmt_count - 1];
	desc.dsc_address = (UCHAR *) (om + (IPTR) desc.dsc_address);
	USHORT eos;
	dsc eos_desc;
	eos_desc.dsc_dtype = dtype_short;
	eos_desc.dsc_scale = 0;
	eos_desc.dsc_length = sizeof(SSHORT);
	eos_desc.dsc_sub_type = 0;
	eos_desc.dsc_flags = 0;
	eos_desc.dsc_address = (UCHAR *) & eos;
	MOV_move(&desc, &eos_desc);
	if (!eos)
		return false;

	for (int i = 0; i < rec_format->fmt_count; i++)
	{
		proc_assignment(&msg_format->fmt_desc[2 * i],
						&msg_format->fmt_desc[2 * i + 1],
						om,
						&rec_format->fmt_desc[i],
						i,
						record);
	}

	return true;
}


static bool get_record(thread_db*	tdbb,
						  RecordSource*	rsb,
						  RecordSource*	parent_rsb,
						  RSE_GET_MODE	mode)
{
/**************************************
 *
 *	g e t _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Get a record from an open record stream.
 *	The mode parameter specifies whether to
 *	go forward one record, go backward one
 *	record, or fetch the current record.
 *
 **************************************/
	SET_TDBB(tdbb);

#ifdef SUPERSERVER

	if (--tdbb->tdbb_quantum < 0)
	{
		JRD_reschedule(tdbb, 0, true);
	}

#endif

/* check request flags for special processing */

	jrd_req* request = tdbb->tdbb_request;
	if (request->req_flags & req_abort)
	{
		return false;
	}
	if (!request->req_transaction)
	{
		return false;
	}
	IRSB impure = (IRSB) ((UCHAR *) request + rsb->rsb_impure);
	if (impure->irsb_flags & irsb_singular_processed)
	{
		return false;
	}

	record_param* rpb = request->req_rpb + rsb->rsb_stream;

#ifdef SCROLLABLE_CURSORS
/* do bof and eof handling for streams which may be navigated */

	if (rsb->rsb_type == rsb_sequential ||
		rsb->rsb_type == rsb_navigate || rsb->rsb_type == rsb_sort)
	{
		if (((mode == RSE_get_forward) && (impure->irsb_flags & irsb_eof)) ||
			((mode == RSE_get_backward) && (impure->irsb_flags & irsb_bof)))
#ifdef PC_ENGINE
			||((mode == RSE_get_current)
			   && (impure->irsb_flags & (irsb_bof | irsb_eof)))
#endif
		{
				return false;
		}
	}
#endif

	switch (rsb->rsb_type)
	{
	case rsb_sequential:
		if (impure->irsb_flags & irsb_bof)
		{
			rpb->rpb_number.setValue(BOF_NUMBER);
		}

#ifdef PC_ENGINE
		if (mode == RSE_get_current)
		{
			if (!VIO_get(tdbb,
						rpb,
						rsb,
						request->req_transaction,
						request->req_pool))
			{
				return false;
			}
		}
		else
#endif
			if (!VIO_next_record(tdbb,
								rpb,
								rsb,
								request->req_transaction,
								request->req_pool,
								(mode == RSE_get_backward),
								false))
			{
				 return false;
			}
		break;

	case rsb_indexed:
		{
			RecordBitmap **pbitmap = ((IRSB_INDEX) impure)->irsb_bitmap, *bitmap;
			if (!pbitmap || !(bitmap = *pbitmap))
				return false;

			bool result = false;

			// NS: Original code was the following:
			// while (SBM_next(*bitmap, &rpb->rpb_number, mode))
			// We assume mode = RSE_get_forward because we do not support
			// scrollable cursors at the moment.
			if (rpb->rpb_number.isBof() ? bitmap->getFirst() : bitmap->getNext()) do {
				rpb->rpb_number.setValue(bitmap->current());
#ifdef SUPERSERVER_V2
				/* Prefetch next set of data pages from bitmap. */

				if (rpb->rpb_number >
					((IRSB_INDEX) impure)->irsb_prefetch_number &&
					(mode == RSE_get_forward))
				{
					((IRSB_INDEX) impure)->irsb_prefetch_number =
						DPM_prefetch_bitmap(tdbb, rpb->rpb_relation,
											*bitmap, rpb->rpb_number);
				}
#endif
				if (VIO_get(tdbb, rpb, rsb, request->req_transaction,
							request->req_pool))
				{
					result = true;
					break;
				}
			} while (bitmap->getNext());

			if (result)
				break;
			return false;
		}

	case rsb_navigate:
#ifdef SCROLLABLE_CURSORS
		if (impure->irsb_flags & irsb_bof)
		{
			rpb->rpb_number = -1;
		}
#endif
		if (!NAV_get_record(tdbb, rsb, (IRSB_NAV) impure, rpb, mode))
		{
			return false;
		}
		break;

	case rsb_boolean:
		{
			bool result;

			/* For ANY and ALL clauses (ALL is handled as a negated ANY),
			   we must first detect them, and then make sure that the returned
			   results are correct.   This mainly entails making sure that
			   there are in fact records in the source stream to test against.
			   If there were none, the response must be FALSE.
			   Also, if the result of the column comparison is always
			   NULL, this must also be returned as NULL.   (Note that normally,
			   an AND of a NULL and a FALSE would be FALSE, not NULL).

			   This all depends on evl.c putting the unoptimized expression
			   in the rsb.   The unoptimized expression always has the
			   select expression on the left, and the column comparison
			   on the right. */

			jrd_nod* select_node;	// ANY/ALL select node pointer
            // ANY/ALL column node pointer
			jrd_nod* column_node = (jrd_nod*) rsb->rsb_any_boolean;
			if (column_node &&
				(request->req_flags & (req_ansi_all | req_ansi_any)))
			{
				/* see if there's a select node to work with */

				if (column_node->nod_type == nod_and)
				{
					select_node = column_node->nod_arg[0];
					column_node = column_node->nod_arg[1];
				}
				else
				{
					select_node = NULL;
				}
			}
			if (column_node && (request->req_flags & req_ansi_any))
			{
				bool any_null;	// some records null for ANY/ALL
				bool any_true;	// some records true for ANY/ALL

				request->req_flags &= ~req_ansi_any;
				if (request->req_flags & req_ansi_not)
				{
					request->req_flags &= ~req_ansi_not;

					/* do NOT ANY */
					/* if the subquery was the empty set
					   (numTrue + numFalse + numUnknown = 0)
					   or if all were false
					   (numTrue + numUnknown = 0),
					   NOT ANY is true */

					any_null = false;
					any_true = false;
					while (get_record(tdbb, rsb->rsb_next, rsb, mode))
					{
						if (EVL_boolean(tdbb, (jrd_nod*) rsb->rsb_arg[0]))
						{
							/* found a TRUE value */

							any_true = true;
							break;
						}
#ifdef PC_ENGINE
						else if (mode == RSE_get_current)
							break;
#endif

						/* check for select stream and nulls */

						if (!select_node)
						{
							if (request->req_flags & req_null)
							{
								any_null = true;
								break;
							}
						}
						else
						{
							request->req_flags &= ~req_null;
							// select for ANY/ALL processing
							const bool select_value = EVL_boolean(tdbb, select_node);

							/* see if any record in select stream */

							if (select_value)
							{
								/* see if any nulls */

								request->req_flags &= ~req_null;
								EVL_boolean(tdbb, column_node);

								/* see if any record is null */

								if (request->req_flags & req_null) {
									any_null = true;
									break;
								}
							}
						}
					}
					request->req_flags &= ~req_null;
					if (any_null || any_true)
						result = true;
					else
						return false;
					break;
				}
				else
				{

					/* do ANY */
					/* if the subquery was true for any comparison,
					   ANY is true */

					result = false;
					while (get_record(tdbb, rsb->rsb_next, rsb, mode))
					{
						if (EVL_boolean(tdbb, (jrd_nod*) rsb->rsb_arg[0])) {
							result = true;
							break;
						}
#ifdef PC_ENGINE
						else if (mode == RSE_get_current)
							break;
#endif
					}
					request->req_flags &= ~req_null;
					if (result)
						break;
					return false;
				}
			}
			else if (column_node && (request->req_flags & req_ansi_all))
			{
				bool any_false;	// some records false for ANY/ALL

				request->req_flags &= ~req_ansi_all;
				if (request->req_flags & req_ansi_not)
				{
					request->req_flags &= ~req_ansi_not;

					/* do NOT ALL */
					/* if the subquery was false for any comparison,
					   NOT ALL is true */

					any_false = false;
					while (get_record(tdbb, rsb->rsb_next, rsb, mode))
					{
						request->req_flags &= ~req_null;

						/* look for a FALSE (and not null either) */

						if (!EVL_boolean(tdbb, (jrd_nod*) rsb->rsb_arg[0]) &&
							!(request->req_flags & req_null))
						{

							/* make sure it wasn't FALSE because there's
							   no select stream record */

							if (select_node) {
								request->req_flags &= ~req_null;
								// select for ANY/ALL processing
								const bool select_value = EVL_boolean(tdbb, select_node);
								if (select_value) {
									any_false = true;
									break;
								}
							}
							else {
								any_false = true;
								break;
							}
						}
#ifdef PC_ENGINE
						else if (mode == RSE_get_current)
							break;
#endif
					}
					request->req_flags &= ~req_null;
					if (any_false)
						return false;
					result = true;
					break;
				}
				else
				{

					/* do ALL */
					/* if the subquery was the empty set
					   (numTrue + numFalse + numUnknown = 0)
					   or if all were true
					   (numFalse + numUnknown = 0),
					   ALL is true */

					any_false = false;
					while (get_record(tdbb, rsb->rsb_next, rsb, mode))
					{
						request->req_flags &= ~req_null;

						/* look for a FALSE or null */

						if (!EVL_boolean(tdbb, (jrd_nod*) rsb->rsb_arg[0]))
						{
							/* make sure it wasn't FALSE because there's
							   no select stream record */

							if (select_node) {
								request->req_flags &= ~req_null;
								// select for ANY/ALL processing
								const bool select_value = EVL_boolean(tdbb, select_node);
								if (select_value) {
									any_false = true;
									break;
								}
							}
							else {
								any_false = true;
								break;
							}
						}
#ifdef PC_ENGINE
						else if (mode == RSE_get_current)
							break;
#endif
					}
					request->req_flags &= ~req_null;
					if (any_false)
						return false;
					result = true;
					break;
				}
			}
			else
			{
				bool flag = false;

				result = false;
				while (get_record(tdbb, rsb->rsb_next, rsb, mode))
				{
					if (EVL_boolean(tdbb, (jrd_nod*) rsb->rsb_arg[0])) {
						result = true;
						break;
					}
#ifdef PC_ENGINE
					/* If we are trying to "RSE_get_current" and there is a
					   * "where" clause which is not true, someone must have
					   * modified it after we positioned on the record
					 */
					else if (mode == RSE_get_current)
						break;
#endif

					if (request->req_flags & req_null)
						flag = true;
				}

				if (flag)
					request->req_flags |= req_null;
				if (result)
					break;
				return false;
			}
		}

        /******
         ***     IMPORTANT!!
         *
         *   If the RecordSource list contains both a rsb_first node and a rsb_skip node
         *     the rsb_skip node MUST be after the rsb_first node in the list.
         *     The reason is the rsb_skip calls get_record in a loop to skip
         *     over the first n records in the stream.  If the rsb_first node
         *     was down stream the counter associated with rsb_first would
         *     be decremented by the calls to get_record that never return a
         *     record to the user.  Possible symptoms of this are erroneous
         *     empty result sets (when skip >= first) and too small result sets
         *     (when first > skip, first - skip records will be returned).
         *******/

	case rsb_first:
		switch (mode) {
		case RSE_get_forward:
			if (((IRSB_FIRST) impure)->irsb_count <= 0)
				return false;
			((IRSB_FIRST) impure)->irsb_count--;
			if (!get_record(tdbb, rsb->rsb_next, NULL, mode))
				return false;
			break;

		case RSE_get_current:
			if (((IRSB_FIRST) impure)->irsb_count <= 0)
				return false;
			if (!get_record(tdbb, rsb->rsb_next, NULL, mode))
				return false;
			break;

		case RSE_get_backward:
			((IRSB_FIRST) impure)->irsb_count++;
			if (!get_record(tdbb, rsb->rsb_next, NULL, mode))
				return false;
			break;
		}
		break;

	case rsb_skip:
		switch (mode) {
		case RSE_get_backward:
			if (((IRSB_SKIP) impure)->irsb_count > 0)
				return false;
			if (((IRSB_SKIP) impure)->irsb_count == 0) {
				((IRSB_SKIP) impure)->irsb_count++;
				get_record(tdbb, rsb->rsb_next, NULL, mode);
				return false;
			}
			((IRSB_SKIP) impure)->irsb_count++;
			if (!get_record(tdbb, rsb->rsb_next, NULL, mode))
				return false;
			break;

		case RSE_get_forward:
			while (((IRSB_SKIP) impure)->irsb_count > 1) {
				((IRSB_SKIP) impure)->irsb_count--;
				if (!get_record(tdbb, rsb->rsb_next, NULL, mode))
					return false;
			}
			((IRSB_SKIP) impure)->irsb_count--;
			if (!get_record(tdbb, rsb->rsb_next, NULL, mode))
				return false;
			break;

		case RSE_get_current:
			if (((IRSB_SKIP) impure)->irsb_count >= 1)
				return false;
			else if (!get_record(tdbb, rsb->rsb_next, NULL, mode))
				return false;
		}
		break;

	case rsb_merge:
		if (!get_merge_join(tdbb, rsb, (IRSB_MRG) impure
#ifdef SCROLLABLE_CURSORS
							, mode
#endif
			))
			return false;
		break;

	case rsb_procedure:
		if (!get_procedure(tdbb, rsb, (IRSB_PROCEDURE) impure, rpb))
			return false;
		break;

	case rsb_sort:
		{
			UCHAR* data;

#ifdef SCROLLABLE_CURSORS
			/* any attempt to get a record takes us off of bof or eof */

			impure->irsb_flags &= ~(irsb_bof | irsb_eof);
#endif

			if (!(data = get_sort(tdbb, rsb
#ifdef SCROLLABLE_CURSORS
								  , mode
#endif
				  )))
			{
#ifdef SCROLLABLE_CURSORS
				if (mode == RSE_get_forward)
					impure->irsb_flags |= irsb_eof;
				else
					impure->irsb_flags |= irsb_bof;
#endif
				return false;
			}

			map_sort_data(request, (SortMap*) rsb->rsb_arg[0], data);

#ifdef SCROLLABLE_CURSORS
			/* fix up the sort data in case we need to retrieve it again */

			unget_sort(tdbb, rsb, data);
#endif
		}
		break;

	case rsb_cross:
		if (impure->irsb_flags & irsb_first) {
			SSHORT i;

			for (i = 0; i < (SSHORT) rsb->rsb_count; i++) {
				RSE_open(tdbb, rsb->rsb_arg[i]);
				if (!fetch_record(tdbb, rsb, i
#ifdef SCROLLABLE_CURSORS
								  , mode
#endif
					))
					return false;
			}
			impure->irsb_flags &= ~irsb_first;
			break;
		}

		/* in the case of a project which has been mapped to an index,
		   we need to make sure that we only return a single record for
		   each of the leftmost records in the join */

		if (rsb->rsb_flags & rsb_project) {
			if (!fetch_record(tdbb, rsb, 0
#ifdef SCROLLABLE_CURSORS
							  , mode
#endif
				))
				return false;
		}

		else if (!fetch_record(tdbb, rsb, rsb->rsb_count - 1
#ifdef SCROLLABLE_CURSORS
							   , mode
#endif
				 ))
			return false;
		break;

	case rsb_union:
		if (!get_union(tdbb, rsb, impure))
			return false;
		break;

	case rsb_aggregate:
		if ( (impure->irsb_count = EVL_group(tdbb, rsb->rsb_next,
										   (jrd_nod*) rsb->rsb_arg[0],
										   impure->irsb_count)) ) break;
		return false;

	case rsb_ext_sequential:
	case rsb_ext_indexed:
	case rsb_ext_dbkey:
		if (!EXT_get(rsb))
			return false;
		break;

	case rsb_left_cross:
		if (!fetch_left(tdbb, rsb, impure
#ifdef SCROLLABLE_CURSORS
						, mode
#endif
			))
			return false;
		break;

	default:
		BUGCHECK(166);			/* msg 166 invalid rsb type */
	}

/* Check to see if we need to update the record_count. This record
   count is used in NAV_get_record and needs to be updated before
   checking for singularity. Note that in our check for singularity
   we call get_record which calls NAV_get_record where this count
   is used. Bug # 8415, 8416 */

	if (rsb->rsb_type == rsb_boolean)
		rsb->rsb_next->rsb_record_count++;
	else if (!parent_rsb || parent_rsb->rsb_type != rsb_boolean)
		rsb->rsb_record_count++;

	if (rsb->rsb_flags & rsb_singular &&
		!(impure->irsb_flags & irsb_checking_singular)) {
		push_rpbs(tdbb, request, rsb);
		impure->irsb_flags |= irsb_checking_singular;
		if (get_record(tdbb, rsb, parent_rsb, mode)) {
			impure->irsb_flags &= ~irsb_checking_singular;
			ERR_post(isc_sing_select_err, 0);
		}
		pop_rpbs(request, rsb);
		impure->irsb_flags &= ~irsb_checking_singular;
		impure->irsb_flags |= irsb_singular_processed;
	}

	return true;
}


static UCHAR *get_sort(thread_db* tdbb, RecordSource* rsb
#ifdef SCROLLABLE_CURSORS
					   , RSE_GET_MODE mode
#endif
	)
{
/**************************************
 *
 *	g e t _ s o r t
 *
 **************************************
 *
 * Functional description
 *	Get the next raw record from a sort stream.  The caller has the
 *	responsibility for mapping the raw data in the target records.
 *
 **************************************/
	SET_TDBB(tdbb);

	jrd_req* request = tdbb->tdbb_request;
	irsb_sort* impure = (IRSB_SORT) ((UCHAR *) request + rsb->rsb_impure);

/* Get address of record from sort.  If the address if null, we
   ran out of records.  This is known in the trade as "end of file." */

	ULONG* data = 0;
#ifdef SCROLLABLE_CURSORS
	SORT_get(tdbb->tdbb_status_vector, impure->irsb_sort_handle,
			 &data, mode);
#else
	SORT_get(tdbb->tdbb_status_vector, impure->irsb_sort_handle,
			 &data);
#endif

	return reinterpret_cast<UCHAR*>(data);
}


static bool get_union(thread_db* tdbb, RecordSource* rsb, IRSB impure)
{
/**************************************
 *
 *	g e t _ u n i o n
 *
 **************************************
 *
 * Functional description
 *	Get next record in a union.
 *
 **************************************/
	SET_TDBB(tdbb);
	RecordSource** rsb_ptr = rsb->rsb_arg + impure->irsb_count;

/* March thru the sub-streams (tributaries?) looking for a record */

	while (!get_record(tdbb, *rsb_ptr, NULL, RSE_get_forward)) {
		RSE_close(tdbb, *rsb_ptr);
		impure->irsb_count += 2;
		if (impure->irsb_count >= rsb->rsb_count)
			return false;
		rsb_ptr += 2;
		RSE_open(tdbb, *rsb_ptr);
	}

/* We've got a record, map it into the target record */

	jrd_nod* map = (jrd_nod*) rsb_ptr[1];

	jrd_nod** ptr = map->nod_arg;
	for (const jrd_nod* const* const end = ptr + map->nod_count; ptr < end; ptr++)
		EXE_assignment(tdbb, *ptr);

	return true;
}


static void join_to_nulls(thread_db* tdbb, RecordSource* rsb, StreamStack* stream)
{
/**************************************
 *
 *	j o i n _ t o _ n u l l s
 *
 **************************************
 *
 * Functional description
 *	Loop through the streams on the right side of a left
 *	outer join, and make them all indicate a null record.
 *
 **************************************/
	SET_TDBB(tdbb);

	jrd_req* request = tdbb->tdbb_request;
	for (StreamStack::iterator stack(*stream);
		stack.hasData(); ++stack)
	{
		record_param* rpb = &request->req_rpb[stack.object()];

		/* Make sure a record block has been allocated.  If there isn't
		   one, first find the format, then allocate the record block */

		Record* record = rpb->rpb_record;
		if (!record) {
			const Format* format = rpb->rpb_relation->rel_current_format;
			if (!format) {
				format =
					MET_format(tdbb, rpb->rpb_relation,
							   rpb->rpb_format_number);
			}
			record = VIO_record(tdbb, rpb, format, tdbb->getDefaultPool());
		}

        record->rec_fmt_bk = record->rec_format;
		record->rec_format = NULL;
	}
}


static void map_sort_data(jrd_req* request, SortMap* map, UCHAR * data)
{
/**************************************
 *
 *	m a p _ s o r t _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Given a sort record, a sort map, and a request, map data from
 *	the raw sort record to civilized record blocks.
 *
 **************************************/
	DSC from, to;

	const smb_repeat* const end_item = map->smb_rpt + map->smb_count;

	for (smb_repeat* item = map->smb_rpt; item < end_item; item++) {
		const UCHAR flag = *(data + item->smb_flag_offset);
		from = item->smb_desc;
		from.dsc_address = data + (IPTR) from.dsc_address;
		jrd_nod* node = item->smb_node;
		if (node && node->nod_type != nod_field)
			continue;

		/* if moving a TEXT item into the key portion of
		   the sort record, then want to sort by
		   language dependant order */

		/* in the case below a nod_field is being converted to
		   a sort key, there is a later nod_field in the item
		   list that contains the data to send back
		 */
		if (IS_INTL_DATA(&item->smb_desc) &&
			(USHORT)(IPTR) item->smb_desc.dsc_address <
			map->smb_key_length * sizeof(ULONG))
		{
			continue;
		}

		record_param* rpb = &request->req_rpb[item->smb_stream];
		const SSHORT id = item->smb_field_id;
		if (id < 0) {
			if (id == SMB_TRANS_ID)
				rpb->rpb_transaction_nr = *(SLONG *) (from.dsc_address);
			else
				rpb->rpb_number.setValue(*(SINT64 *) (from.dsc_address));
			rpb->rpb_stream_flags |= RPB_s_refetch;
			continue;
		}
		Record* record = rpb->rpb_record;

        if (record && !flag && !record->rec_format && record->rec_fmt_bk) {
            record->rec_format = record->rec_fmt_bk; // restore the format
        }

		EVL_field(0, record, id, &to);

		if (flag)
			SET_NULL(record, id);
		else {
			MOV_move(&from, &to);
			CLEAR_NULL(record, id);
		}
	}
}


static void open_merge(thread_db* tdbb, RecordSource* rsb, IRSB_MRG impure)
{
/**************************************
 *
 *	o p e n _ m e r g e
 *
 **************************************
 *
 * Functional description
 *	Initialize a merge stream by opening all substreams
 *	and cleaning up after any prior sort-merge retrieval.
 *
 **************************************/

	SET_TDBB(tdbb);

/* do two simultaneous but unrelated things in one loop */

	RecordSource** ptr = rsb->rsb_arg;
	const RecordSource* const* end = ptr + rsb->rsb_count * 2;
	for (irsb_mrg::irsb_mrg_repeat* tail = impure->irsb_mrg_rpt; ptr < end;
		ptr += 2, tail++)
	{
		/* open all the substreams for the sort-merge */

		RSE_open(tdbb, *ptr);

		RecordSource* sort_rsb = *ptr;
		SortMap* map = (SortMap*) sort_rsb->rsb_arg[0];

		/* Reset equality group record positions */

		tail->irsb_mrg_equal = -1;
		tail->irsb_mrg_equal_end = -1;
		tail->irsb_mrg_equal_current = -1;
		tail->irsb_mrg_last_fetched = -1;
		tail->irsb_mrg_order = tail - impure->irsb_mrg_rpt;

		merge_file* mfb = &tail->irsb_mrg_file;
		mfb->mfb_equal_records = 0;
		mfb->mfb_current_block = 0;
		mfb->mfb_record_size = ROUNDUP_LONG(map->smb_length);
		mfb->mfb_block_size = MAX(mfb->mfb_record_size, MERGE_BLOCK_SIZE);
		mfb->mfb_blocking_factor = mfb->mfb_block_size / mfb->mfb_record_size;
		if (!mfb->mfb_block_data)
			mfb->mfb_block_data =
				FB_NEW(*tdbb->tdbb_request->req_pool) UCHAR[mfb->mfb_block_size];
	}
}


static void open_procedure(thread_db* tdbb, RecordSource* rsb, IRSB_PROCEDURE impure)
{
/**************************************
 *
 *	o p e n _ p r o c e d u r e
 *
 **************************************
 *
 * Functional description
 *	Initialize a procedural view.
 *
 **************************************/
	SET_TDBB(tdbb);

	jrd_nod* inputs = (jrd_nod*) rsb->rsb_arg[RSB_PRC_inputs];
	jrd_prc* procedure = rsb->rsb_procedure;
	jrd_req* request = tdbb->tdbb_request;

/* get rid of any lingering record */

	record_param* rpb = request->req_rpb + rsb->rsb_stream;
	if (rpb->rpb_record) {
		delete rpb->rpb_record;
		rpb->rpb_record = NULL;
	}

	USHORT iml;
	UCHAR* im;

	jrd_req* proc_request = EXE_find_request(tdbb, procedure->prc_request, FALSE);
	impure->irsb_req_handle = proc_request;
	if (inputs) {
		enum jrd_req::req_s saved_state = request->req_operation;

		jrd_nod** ptr = inputs->nod_arg;
		for (const jrd_nod* const* const end = ptr + inputs->nod_count;
			ptr < end; ptr++)
		{
			EXE_assignment(tdbb, *ptr);
		}

		request->req_operation = saved_state;
		jrd_nod* in_message = (jrd_nod*) rsb->rsb_arg[RSB_PRC_in_msg];
		const Format* format = (Format*) in_message->nod_arg[e_msg_format];
		iml = format->fmt_length;
		im = (UCHAR *) request + in_message->nod_impure;
	}
	else {
		iml = 0;
		im = NULL;
	}

/* req_proc_fetch flag used only when fetching rows, so
   is set at end of open_procedure (). */

	proc_request->req_flags &= ~req_proc_fetch;

	try {
		EXE_start(tdbb, proc_request, request->req_transaction);
		if (iml) {
			EXE_send(tdbb, proc_request, 0, iml, im);
		}
	}
	catch (const std::exception&) {
		close_procedure(tdbb, rsb);
		throw;
	}

	proc_request->req_flags |= req_proc_fetch;
}


static void open_sort(thread_db* tdbb, RecordSource* rsb, IRSB_SORT impure, UINT64 max_records)
{
/**************************************
 *
 *	o p e n _ s o r t
 *
 **************************************
 *
 * Functional description
 *	Initialize sort, open the sub-stream, pump all records
 *	into sort, etc.  This gets things going.
 *
 **************************************/
	SET_TDBB(tdbb);
	jrd_req* request = tdbb->tdbb_request;

	RSE_open(tdbb, rsb->rsb_next);
	SortMap* map = (SortMap*) rsb->rsb_arg[0];
	ULONG records = 0;

	// Get rid of the old sort areas if this request has been used already

	if (impure->irsb_sort_handle &&
		impure->irsb_sort_handle->scb_impure == impure)
	{
		SORT_fini(impure->irsb_sort_handle, tdbb->tdbb_attachment);
	}

	// Initialize for sort. If this is really a project operation,
	// establish a callback routine to reject duplicate records.

	sort_context* handle = SORT_init(tdbb->tdbb_status_vector,
						   map->smb_length,
						   map->smb_keys,
						   map->smb_keys,
						   map->smb_key_desc,
         				   ((map->smb_flags & SMB_project) ? reject : NULL), 0,
						   tdbb->tdbb_attachment, max_records);

	if (!(impure->irsb_sort_handle = handle))
		ERR_punt();

	// Mark sort_context with the impure area pointer

	handle->scb_impure = impure;

	// Pump the input stream dry while pushing records into sort. For
	// each record, map all fields into the sort record. The reverse
	// mapping is done in get_sort().

	dsc to, temp;

	while (get_record(tdbb, rsb->rsb_next, NULL, RSE_get_forward)) {
		records++;

		// "Put" a record to sort. Actually, get the address of a place
		// to build a record.

		UCHAR* data = 0;
		SORT_put(tdbb->tdbb_status_vector, impure->irsb_sort_handle,
				(ULONG **) &data);

		// Zero out the sort key. This solve a multitude of problems.

		MOVE_CLEAR(data, (SLONG) map->smb_length);

		// Loop thru all field (keys and hangers on) involved in the sort.
		// Be careful to null field all unused bytes in the sort key.

		const smb_repeat* const end_item = map->smb_rpt + map->smb_count;
		for (smb_repeat* item = map->smb_rpt; item < end_item; item++) {
			to = item->smb_desc;
			to.dsc_address = data + (IPTR) to.dsc_address;
			bool flag = false;
			dsc* from = 0;
			if (item->smb_node) {
				from = EVL_expr(tdbb, item->smb_node);
				if (request->req_flags & req_null)
					flag = true;
			}
			else {
				from = &temp;
				record_param* rpb = &request->req_rpb[item->smb_stream];
				if (item->smb_field_id < 0) {
					if (item->smb_field_id == SMB_TRANS_ID)
						*(SLONG *) (to.dsc_address) = rpb->rpb_transaction_nr;
					else
						*(SINT64 *) (to.dsc_address) = rpb->rpb_number.getValue();
					continue;
				}
				if (!EVL_field
					(rpb->rpb_relation, rpb->rpb_record, item->smb_field_id,
					 from))
				{
					flag = true;
				}
			}
			*(data + item->smb_flag_offset) = flag ? TRUE : FALSE;
			if (!flag) {
				// If moving a TEXT item into the key portion of the sort record,
				// then want to sort by language dependent order.

				if (IS_INTL_DATA(&item->smb_desc) &&
					(USHORT)(IPTR) item->smb_desc.dsc_address <
					map->smb_key_length * sizeof(ULONG))
				{
					INTL_string_to_key(tdbb, INTL_INDEX_TYPE(&item->smb_desc),
									   from, &to, INTL_KEY_SORT);
				}
				else
					MOV_move(from, &to);
			}
		}
	}

	if (!SORT_sort(tdbb->tdbb_status_vector, impure->irsb_sort_handle))
		ERR_punt();

	// For the sake of prudence, set all record parameter blocks to contain
	// the most recent format. This is will guarentee that all fields mapped
	// back to records by get_sort() have homes in the target record.

	if (!records)
		return;

	SSHORT stream = -1;

	const smb_repeat* const end_item = map->smb_rpt + map->smb_count;
	for (smb_repeat* item = map->smb_rpt; item < end_item; item++) {
		if (item->smb_node && item->smb_node->nod_type != nod_field)
			continue;
		if (item->smb_stream == stream)
			continue;
		stream = item->smb_stream;
		record_param* rpb = &request->req_rpb[stream];
		if (rpb->rpb_relation)
			VIO_record(tdbb, rpb, MET_current(tdbb, rpb->rpb_relation),
					   tdbb->getDefaultPool());
	}
}


static void proc_assignment(
							const dsc* from_desc,
							const dsc* flag_desc,
							UCHAR* msg, // this param is logically const
							dsc* to_desc, SSHORT to_id, Record* record)
{
/**************************************
 *
 *	p r o c _ a s s i g n m e n t
 *
 **************************************
 *
 * Functional description
 *	Perform an assignment from a message to a record format.
 *
 **************************************/
	SSHORT indicator;
	dsc desc2;
	desc2.dsc_dtype = dtype_short;
	desc2.dsc_scale = 0;
	desc2.dsc_length = sizeof(SSHORT);
	desc2.dsc_sub_type = 0;
	desc2.dsc_flags = 0;
	desc2.dsc_address = (UCHAR *) & indicator;

	dsc desc1;
	desc1 = *flag_desc;
	desc1.dsc_address = msg + (IPTR) flag_desc->dsc_address;
	MOV_move(&desc1, &desc2);
	if (indicator) {
		SET_NULL(record, to_id);
		SSHORT l = to_desc->dsc_length; // it seems safer to use USHORT
		fb_assert(l); // l == 0 would produce undesirable results here
		UCHAR* p = record->rec_data + (IPTR) to_desc->dsc_address;
		switch (to_desc->dsc_dtype) {
		case dtype_text:
			/* YYY - not necessarily the right thing to do */
			/* YYY for text formats that don't have trailing spaces */
			if (l) {
				do {
					*p++ = ' ';
				} while (--l);
			}
			break;

		case dtype_cstring:
			*p = 0;
			break;

		case dtype_varying:
			*(SSHORT *) p = 0;
			break;

		default:
			do {
				*p++ = 0;
			} while (--l);
			break;
		}
		to_desc->dsc_flags |= DSC_null;
	}
	else {
		CLEAR_NULL(record, to_id);
		desc1 = *from_desc;
		desc1.dsc_address = msg + (IPTR) desc1.dsc_address;
		desc2 = *to_desc;
		desc2.dsc_address = record->rec_data + (IPTR) desc2.dsc_address;
		if (!DSC_EQUIV((&desc1), (&desc2)))
			MOV_move(&desc1, &desc2);

		else if (desc1.dsc_dtype == dtype_short)
			*((SSHORT *) desc2.dsc_address) = *((SSHORT *) desc1.dsc_address);

		else if (desc1.dsc_dtype == dtype_long)
			*((SLONG *) desc2.dsc_address) = *((SLONG *) desc1.dsc_address);

		else if (desc1.dsc_dtype == dtype_int64)
			*((SINT64 *) desc2.dsc_address) = *((SINT64 *) desc1.dsc_address);

		else if (((U_IPTR) desc1.dsc_address & (ALIGNMENT - 1)) ||
				 ((U_IPTR) desc2.dsc_address & (ALIGNMENT - 1)))
		{
			MOVE_FAST(desc1.dsc_address, desc2.dsc_address, desc1.dsc_length);
		}
		else
			MOVE_FASTER(desc1.dsc_address, desc2.dsc_address,
						desc1.dsc_length);
	}
}


static void pop_rpbs(jrd_req* request, RecordSource* rsb)
{
/**************************************
 *
 *	p o p _ r p b s
 *
 **************************************
 *
 * Functional description
 *	Restore record state to saved copy.
 *
 **************************************/
	// temporary sparse bitmap of the used streams
	Firebird::HalfStaticArray<UCHAR, OPT_STATIC_ITEMS> streams(*request->req_pool);

	switch (rsb->rsb_type) {
	case rsb_indexed:
	case rsb_sequential:
	case rsb_procedure:
	case rsb_ext_sequential:
	case rsb_ext_indexed:
	case rsb_ext_dbkey:
	case rsb_navigate:
	case rsb_union:
	case rsb_aggregate:
		{
			record_param* rpb = request->req_rpb + rsb->rsb_stream;
			restore_record(rpb);
			break;
		}

	case rsb_sort:
		{
			streams.grow(request->req_count);
			memset(streams.begin(), 0, request->req_count * sizeof(UCHAR));
			const SortMap* map = (SortMap*) rsb->rsb_arg[0];
			const smb_repeat* const end_item = map->smb_rpt + map->smb_count;
			for (const smb_repeat* item = map->smb_rpt; item < end_item; item++)
			{
				streams[item->smb_stream] = 1;
			}
			for (int i = 0; i < request->req_count; i++)
			{
				if (streams[i])
				{
					record_param* rpb = request->req_rpb + i;
					restore_record(rpb);
				}
			}
		}
		break;

	case rsb_merge:
		{
			streams.grow(request->req_count);
			memset(streams.begin(), 0, request->req_count * sizeof(UCHAR));
			const RecordSource* const* const end = rsb->rsb_arg + rsb->rsb_count * 2;
			// irsb_mrg* impure = (irsb_mrg*) ((UCHAR *) request + rsb->rsb_impure);
			for (const RecordSource* const* ptr = rsb->rsb_arg; ptr < end; ptr += 2)
			{
				const RecordSource* sort_rsb = *ptr;
				const SortMap* map = (SortMap*) sort_rsb->rsb_arg[0];
				const smb_repeat* const end_item = map->smb_rpt + map->smb_count;
				for (const smb_repeat* item = map->smb_rpt; item < end_item; item += 2)
				{
					streams[item->smb_stream] = 1;
				}
			}
			for (int i = 0; i < request->req_count; i++)
			{
				if (streams[i])
				{
					record_param* rpb = request->req_rpb + i;
					restore_record(rpb);
				}
			}
		}
		break;

	case rsb_first:
    case rsb_skip:
	case rsb_boolean:
		pop_rpbs(request, rsb->rsb_next);
		break;

	case rsb_cross:
		{
			/* Bug # 72369: singleton-SELECT in Stored Procedure gives wrong
			 * results when there are more than 2 streams in the cross.
			 * rsb_cross can have more than 2 rsb_arg's. Go through each one
			 */
			RecordSource** ptr = rsb->rsb_arg;
			for (const RecordSource* const* const end = ptr + rsb->rsb_count; ptr < end;
				 ptr++)
			{
				pop_rpbs(request, *ptr);
			}
		}
		break;

	case rsb_left_cross:
		pop_rpbs(request, rsb->rsb_arg[RSB_LEFT_outer]);
		pop_rpbs(request, rsb->rsb_arg[RSB_LEFT_inner]);
		break;

	default:
		BUGCHECK(166);	/* msg 166 invalid rsb type */
	}
}


static void push_rpbs(thread_db* tdbb, jrd_req* request, RecordSource* rsb)
{
/**************************************
 *
 *	p u s h _ r p b s
 *
 **************************************
 *
 * Functional description
 *	Save data state for current rsb
 *
 **************************************/
	SET_TDBB(tdbb);

	// temporary sparse bitmap of the used streams
	Firebird::HalfStaticArray<UCHAR, OPT_STATIC_ITEMS> streams(*request->req_pool);

	switch (rsb->rsb_type) {
	case rsb_indexed:
	case rsb_sequential:
	case rsb_procedure:
	case rsb_ext_sequential:
	case rsb_ext_indexed:
	case rsb_ext_dbkey:
	case rsb_navigate:
	case rsb_union:
	case rsb_aggregate:
		{
			record_param* rpb = request->req_rpb + rsb->rsb_stream;
			save_record(tdbb, rpb);
			break;
		}

	case rsb_sort:
		{
			streams.grow(request->req_count);
			memset(streams.begin(), 0, request->req_count * sizeof(UCHAR));
			const SortMap* map = (SortMap*) rsb->rsb_arg[0];
			const smb_repeat* const end_item = map->smb_rpt + map->smb_count;
			for (const smb_repeat* item = map->smb_rpt; item < end_item; item++)
			{
				streams[item->smb_stream] = 1;
			}
			for (int i = 0; i < request->req_count; i++)
			{
				if (streams[i])
				{
					record_param* rpb = request->req_rpb + i;
					save_record(tdbb, rpb);
				}
			}
		}
		break;

	case rsb_merge:
		{
			streams.grow(request->req_count);
			memset(streams.begin(), 0, request->req_count * sizeof(UCHAR));
			const RecordSource* const* const end = rsb->rsb_arg + rsb->rsb_count * 2;
			// irsb_mrg* impure = (irsb_mrg*) ((UCHAR *) request + rsb->rsb_impure);
			for (const RecordSource* const* ptr = rsb->rsb_arg; ptr < end; ptr += 2)
			{
				const RecordSource* sort_rsb = *ptr;
				const SortMap* map = (SortMap*) sort_rsb->rsb_arg[0];
				const smb_repeat* const end_item = map->smb_rpt + map->smb_count;
				for (const smb_repeat* item = map->smb_rpt; item < end_item; item++)
				{
					streams[item->smb_stream] = 1;
				}
			}
			for (int i = 0; i < request->req_count; i++)
			{
				if (streams[i])
				{
					record_param* rpb = request->req_rpb + i;
					save_record(tdbb, rpb);
				}
			}
		}
		break;

	case rsb_first:
    case rsb_skip:
	case rsb_boolean:
		push_rpbs(tdbb, request, rsb->rsb_next);
		break;

	case rsb_cross:
		{
			/* Bug # 72369: singleton-SELECT in Stored Procedure gives wrong
			 * results when there are more than 2 streams in the cross.
			 * rsb_cross can have more than 2 rsb_arg's. Go through each one
			 */
			RecordSource** ptr = rsb->rsb_arg;
			for (const RecordSource* const* const end = ptr + rsb->rsb_count; ptr < end;
				 ptr++)
			{
				push_rpbs(tdbb, request, *ptr);
			}
		}
		break;

		/* BUG #8637: left outer join gives internal gds software consistency
		   check. Added case for rsb_left_cross. */
	case rsb_left_cross:
		push_rpbs(tdbb, request, rsb->rsb_arg[RSB_LEFT_outer]);
		push_rpbs(tdbb, request, rsb->rsb_arg[RSB_LEFT_inner]);
		break;

	default:
		BUGCHECK(166);	/* msg 166 invalid rsb type */
	}
}


static ULONG read_merge_block(thread_db* tdbb, merge_file* mfb, ULONG block)
{
/**************************************
 *
 *	r e a d _ m e r g e _ b l o c k
 *
 **************************************
 *
 * Functional description
 *	Read a block from the merge equivalence file.
 *
 **************************************/

	fb_assert(mfb->mfb_sfb && mfb->mfb_sfb->sfb_file_name);

	mfb->mfb_sfb->sfb_mem->read(tdbb->tdbb_status_vector,
								mfb->mfb_block_size * block,
								reinterpret_cast<char*>(mfb->mfb_block_data),
								mfb->mfb_block_size);

	return block;
}


static bool reject(const UCHAR* record_a, const UCHAR* record_b, void* user_arg)
{
/**************************************
 *
 *	r e j e c t
 *
 **************************************
 *
 * Functional description
 *	Callback routine used by project to reject duplicate records.
 *	Particularly dumb routine -- always returns true;
 *
 **************************************/
	return true;
}


static void restore_record(record_param* rpb)
{
/**************************************
 *
 *	r e s t o r e _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Restore record to prior state
 *
 **************************************/
	Record* rec_copy;
	SaveRecordParam* rpb_copy = rpb->rpb_copy;
	if (rpb_copy && (rec_copy = rpb_copy->srpb_rpb->rpb_record)) {
		Record* record = rpb->rpb_record;
		const USHORT size = rec_copy->rec_length;
		if (!record || size > record->rec_length)
			/* msg 284 cannot restore singleton select data */
			BUGCHECK(284);
		record->rec_format = rec_copy->rec_format;
		record->rec_number = rec_copy->rec_number;
		MOVE_FAST(rec_copy->rec_data, record->rec_data, size);

		MOVE_FAST(rpb_copy->srpb_rpb, rpb, sizeof(record_param));
		rpb->rpb_record = record;

		delete rec_copy;
	}
	if (rpb_copy)
		delete rpb_copy;

	rpb->rpb_copy = NULL;
}


#ifdef SCROLLABLE_CURSORS
static void resynch_merge(
						  thread_db* tdbb,
						  RecordSource* rsb, IRSB_MRG impure, RSE_GET_MODE mode)
{
/**************************************
 *
 *	r e s y n c h _ m e r g e
 *
 **************************************
 *
 * Functional description
 *	We are in trouble because we stepped off
 *	an equivalence group in a direction different
 *	than that which we entered it.  We need to
 *	"back up" each substream by an amount equal
 * 	to the number of records in the grouping.
 *
 **************************************/
	SET_TDBB(tdbb);
	jrd_req* request = tdbb->tdbb_request;

	const RecordSource* const* const end = rsb->rsb_arg + rsb->rsb_count * 2;
	RecordSource** ptr = rsb->rsb_arg;
	for (irsb_mrg::irsb_mrg_repeat* tail = impure->irsb_mrg_rpt; ptr < end;
		 ptr += 2, tail++)
	{
		RecordSource* sort_rsb = *ptr;
		merge_file* mfb = &tail->irsb_mrg_file;

		/* increment (or decrement) past each record in the substream
		   which has been stored in the merge equivalence group; assume
		   the actual count is one less, unless an additional record
		   was stored in the "last_fetched" field */

		SLONG records = mfb->mfb_equal_records;
		if (records)
			--records;

		for (; records; --records) {
			/* failure of this operation should never happen,
			   but if it does give an error */

			UCHAR* data = get_sort(tdbb, sort_rsb, mode);
			if (!data)
				ERR_post(isc_stream_eof, 0);

			/* do an "unget" to fix up the sort record for re-retrieval */

			unget_sort(tdbb, sort_rsb, data);
		}

		/* null out the "last fetched" record, since this is not the
		   next record we want to retrieve anymore--we are headed in
		   the opposite direction */

		tail->irsb_mrg_last_fetched = -1;
	}
}
#endif


static void save_record(thread_db* tdbb, record_param* rpb)
{
/**************************************
 *
 *	s a v e _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Save current record
 *
 **************************************/
	SET_TDBB(tdbb);

	Record* record = rpb->rpb_record;
	if (record) {
		const SLONG size = record->rec_length;
		SaveRecordParam* rpb_copy = rpb->rpb_copy;
		if (rpb_copy) {
			Record* rec_copy = rpb_copy->srpb_rpb->rpb_record;
			if (rec_copy)
				delete rec_copy;
		}
		else
			rpb->rpb_copy = rpb_copy = FB_NEW(*tdbb->getDefaultPool()) SaveRecordParam();

		MOVE_FAST(rpb, rpb_copy->srpb_rpb, sizeof(record_param));
		Record* rec_copy = FB_NEW_RPT(*tdbb->getDefaultPool(), size) Record(*tdbb->getDefaultPool());
		rpb_copy->srpb_rpb->rpb_record = rec_copy;

		rec_copy->rec_length = size;
		rec_copy->rec_format = record->rec_format;
		rec_copy->rec_number = record->rec_number;
		MOVE_FAST(record->rec_data, rec_copy->rec_data, size);
	}
}


#ifdef SCROLLABLE_CURSORS
static void unget_sort(thread_db* tdbb, RecordSource* rsb, UCHAR* data)
{
/**************************************
 *
 *	u n g e t _ s o r t
 *
 **************************************
 *
 * Functional description
 *	Fix up the sort data in case we need to re-retrieve
 *	it.  This was initiated because of the need to navigate
 *	through the sort, and the fact that retrieving the sort
 *	data is destructive to the sort data (diddles it).
 *
 **************************************/
	SET_TDBB(tdbb);
	jrd_req* request = tdbb->tdbb_request;

	IRSB_SORT impure = (IRSB_SORT) ((UCHAR *) request + rsb->rsb_impure);

	SORT_diddle_key(data, impure->irsb_sort_handle, true);
}
#endif


static void write_merge_block(thread_db* tdbb, merge_file* mfb, ULONG block)
{
/**************************************
 *
 *	w r i t e _ m e r g e _ b l o c k
 *
 **************************************
 *
 * Functional description
 *	Write a block to the merge equivalence file. If
 *	the file doesn't exist, by all means, create one.
 *
 **************************************/
	sort_work_file* sfb = mfb->mfb_sfb;
	if (!sfb) {
		sfb = mfb->mfb_sfb = FB_NEW(*getDefaultMemoryPool()) sort_work_file;
		memset(sfb, 0, sizeof(sort_work_file));
	}
	if (!sfb->sfb_file_name) {
		TEXT file_name[MAXPATHLEN];

		// Cast is ok because stdio_flag is false
		sfb->sfb_file = (int) (IPTR) gds__temp_file(FALSE, SCRATCH, file_name);
		if (sfb->sfb_file == -1)
			SORT_error(tdbb->tdbb_status_vector, sfb, "open", isc_io_error,
					   errno);
		sfb->sfb_file_name =
			FB_NEW(*getDefaultMemoryPool()) char[strlen(file_name) + 1];
		strcpy(sfb->sfb_file_name, file_name);

		sfb->sfb_mem = FB_NEW (*getDefaultMemoryPool()) SortMem(sfb, mfb->mfb_block_size);
	}

	sfb->sfb_mem->write(tdbb->tdbb_status_vector,
						 mfb->mfb_block_size * block,
						 reinterpret_cast<char*>(mfb->mfb_block_data),
						 mfb->mfb_block_size);
}
