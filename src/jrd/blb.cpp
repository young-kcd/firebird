/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		blb.c
 *	DESCRIPTION:	Blob handler
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
 * 2001.6.23 Claudio Valderrama: BLB_move_from_string to accept assignments
 * from string to blob field. First use was to allow inserting a literal string
 * in a blob field without requiring an UDF.
 *
 * 2001.07.06 Sean Leyne - Code Cleanup, removed "#ifdef READONLY_DATABASE"
 *                         conditionals, as the engine now fully supports
 *                         readonly databases.
 *
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "MPEXL" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "DecOSF" port
 *
 */
/*
$Id: blb.cpp,v 1.30 2003-04-10 06:49:10 aafemt Exp $
*/

#include "firebird.h"
#include <string.h>
#include "../jrd/jrd.h"
#include "../jrd/tra.h"
#include "../jrd/val.h"
#include "../jrd/exe.h"
#include "../jrd/req.h"
#include "../jrd/blb.h"
#include "../jrd/ods.h"
#include "../jrd/lls.h"
#include "gen/codes.h"
#include "../jrd/blob_filter.h"
#include "../jrd/sdl.h"
#include "../jrd/intl.h"
#include "../jrd/cch.h"
#include "../jrd/common.h"
#include "../jrd/constants.h"
#include "../jrd/gdsassert.h"
#include "../jrd/all_proto.h"
#include "../jrd/blb_proto.h"
#include "../jrd/blf_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/dpm_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/filte_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/jrd_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/pag_proto.h"
#include "../jrd/sdl_proto.h"
#include "../jrd/thd_proto.h"
#include "../jrd/dsc_proto.h"


#define STREAM          (blob->blb_flags & BLB_stream)
#define SEGMENTED       !STREAM

static ARR alloc_array(JRD_TRA, ADS);
static BLB allocate_blob(TDBB, JRD_TRA);
static ISC_STATUS blob_filter(USHORT, CTL, SSHORT, SLONG);
static void check_BID_validity(BLB, TDBB);
static BLB copy_blob(TDBB, BID, JRD_REL, BID);
static void delete_blob(TDBB, BLB, ULONG);
static void delete_blob_id(TDBB, BID, ULONG, JRD_REL);
static ARR find_array(JRD_TRA, BID);
static BLF find_filter(TDBB, SSHORT, SSHORT);
static BLP get_next_page(TDBB, BLB, WIN *);
#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
static void get_replay_blob(TDBB, BID);
#endif
static void insert_page(TDBB, BLB);
static void release_blob(BLB, USHORT);
static void slice_callback(SLICE, ULONG, DSC *);
static BLB store_array(TDBB, JRD_TRA, BID);


void BLB_cancel(TDBB tdbb, BLB blob)
{
/**************************************
 *
 *      B L B _ c a n c e l
 *
 **************************************
 *
 * Functional description
 *      Abort a blob operation.  If the blob is a partially created
 *      temporary blob, free up any allocated pages.  In any case,
 *      get rid of the blob block.
 *
 **************************************/

	SET_TDBB(tdbb);

/* Release filter control resources */

	if (blob->blb_flags & BLB_temporary)
		delete_blob(tdbb, blob, 0);

	release_blob(blob, TRUE);
}


void BLB_close(TDBB tdbb, class blb* blob)
{
/**************************************
 *
 *      B L B _ c l o s e
 *
 **************************************
 *
 * Functional description
 *      Close a blob.  If the blob is open for retrieval, release the
 *      blob block.  If it's a temporary blob, flush out the last page
 *      (if necessary) in preparation for materialization.
 *
 **************************************/

	SET_TDBB(tdbb);

/* Release filter control resources */

	if (blob->blb_filter)
		BLF_close_blob(tdbb, &blob->blb_filter);

	blob->blb_flags |= BLB_closed;

	if (!(blob->blb_flags & BLB_temporary)) {
		release_blob(blob, TRUE);
		return;
	}

	if (blob->blb_level >= 1
		&& blob->blb_space_remaining < blob->blb_clump_size) insert_page(tdbb,
																		 blob);
}


BLB BLB_create(TDBB tdbb, JRD_TRA transaction, BID blob_id)
{
/**************************************
 *
 *      B L B _ c r e a t e
 *
 **************************************
 *
 * Functional description
 *      Create a shiney, new, empty blob.
 *
 **************************************/

	SET_TDBB(tdbb);
	return BLB_create2(tdbb, transaction, blob_id, 0, (UCHAR *) 0);
}


BLB BLB_create2(TDBB tdbb,
				JRD_TRA transaction, BID blob_id, USHORT bpb_length, UCHAR * bpb)
{
/**************************************
 *
 *      B L B _ c r e a t e 2
 *
 **************************************
 *
 * Functional description
 *      Create a shiney, new, empty blob.
 *      Basically BLB_create() with BPB structure.
 *
 **************************************/
	DBB dbb;
	BLB blob;
	BLP page;
	SSHORT from, to, type;
	SSHORT from_charset, to_charset;
	BLF filter;
	USHORT filter_required = FALSE;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	if (dbb->dbb_flags & DBB_read_only)
		ERR_post(isc_read_only_database, 0);

/* Create a blob large enough to hold a single data page */

	type = gds__parse_bpb2(bpb_length,
						   bpb,
						   reinterpret_cast < SSHORT * >(&from),
						   reinterpret_cast < SSHORT * >(&to),
						   reinterpret_cast < USHORT * >(&from_charset),
						   reinterpret_cast < USHORT * >(&to_charset));
	blob = allocate_blob(tdbb, transaction);

	if (type)
		blob->blb_flags |= BLB_stream;

	blob->blb_source_interp = from_charset;
	blob->blb_target_interp = to_charset;
	blob->blb_sub_type = to;

	filter = NULL;
	if (to && from != to) {
		filter = find_filter(tdbb, from, to);
		filter_required = TRUE;
	}
	else if (to == BLOB_text && (from_charset != to_charset)) {
		if (from_charset == CS_dynamic)
			from_charset = tdbb->tdbb_attachment->att_charset;
		if (to_charset == CS_dynamic)
			to_charset = tdbb->tdbb_attachment->att_charset;
		if ((to_charset != CS_NONE) && (from_charset != to_charset)) {
			filter = FB_NEW(*dbb->dbb_permanent) blf();
			filter->blf_filter =
				reinterpret_cast<ISC_STATUS (*)(USHORT, CTL)>(filter_transliterate_text);
			filter_required = TRUE;
		}
	}

	if (filter_required) {
		if (BLF_create_blob(tdbb,
							transaction,
							&blob->blb_filter,
							reinterpret_cast < long *>(blob_id),
							bpb_length,
							bpb,
							reinterpret_cast < long (*)() > (blob_filter),
							filter)) ERR_punt();
		blob->blb_flags |= BLB_temporary;
		return blob;
	}

	blob->blb_space_remaining = blob->blb_clump_size;
	blob->blb_flags |= BLB_temporary;

/* Set up for a "small" blob -- a blob that fits on an ordinary data page */

	page = (BLP) blob->blb_data;
	page->blp_header.pag_type = pag_blob;
	blob->blb_segment = (UCHAR *) page->blp_data;

/* Format blob id and return blob handle */

	blob_id->bid_stuff.bid_blob = blob;
	blob_id->bid_relation_id = 0;

	return blob;
}


void BLB_garbage_collect(
						 TDBB tdbb,
						 LLS going,
						 LLS staying, SLONG prior_page, JRD_REL relation)
{
/**************************************
 *
 *      B L B _ g a r b a g e _ c o l l e c t
 *
 **************************************
 *
 * Functional description
 *      Garbage collect indices and blobs.  Garbage_collect is passed a
 *      stack of record blocks of records that are being deleted and a stack
 *      of records blocks of records that are staying.  Garbage_collect
 *      can be called from four operations:
 *
 *              VIO_backout     -- removing top record
 *              expunge         -- removing all versions of a record
 *              purge           -- removing all but top version of a record
 *              update_in_place -- replace the top level record.
 *
 *
 **************************************/
	LLS stack1, stack2;
	REC rec1, rec2;
	DSC desc1, desc2;
	FMT format;
	BID blob, blob2;
	USHORT id;

	SET_TDBB(tdbb);

/* Loop thru records on the way out looking for blobs to garbage collect */

	for (stack1 = going; stack1; stack1 = stack1->lls_next) {
		if (!(rec1 = (REC) stack1->lls_object))
			continue;
		format = (FMT) rec1->rec_format;

		/* Look for active blob records */

		for (id = 0; id < format->fmt_count;
			 id++) {
			if (!DTYPE_IS_BLOB(format->fmt_desc[id].dsc_dtype)
				|| !EVL_field(0, rec1, id, &desc1))
				continue;
			blob = (BID) desc1.dsc_address;

			/* Got active blob, cancel it out of any remaining records on the way out */

			for (stack2 = stack1->lls_next; stack2; stack2 = stack2->lls_next) {
				rec2 = (REC) stack2->lls_object;
				if (!EVL_field(0, rec2, id, &desc2))
					continue;
				blob2 = (BID) desc2.dsc_address;
				if (blob->bid_relation_id == blob2->bid_relation_id &&
					blob->bid_stuff.bid_number == blob2->bid_stuff.bid_number)
					SET_NULL(rec2, id);
			}

			/* Make sure the blob doesn't stack in any record remaining */

			for (stack2 = staying; stack2; stack2 = stack2->lls_next) {
				rec2 = (REC) stack2->lls_object;
				if (!EVL_field(0, rec2, id, &desc2))
					continue;
				blob2 = (BID) desc2.dsc_address;
				if (blob->bid_relation_id == blob2->bid_relation_id &&
					blob->bid_stuff.bid_number == blob2->bid_stuff.bid_number)
					break;
			}
			if (stack2)
				continue;

			/* Get rid of blob */

			delete_blob_id(tdbb, blob, prior_page, relation);
		}
	}
}


BLB BLB_get_array(TDBB tdbb, JRD_TRA transaction, BID blob_id, ADS desc)
{
/**************************************
 *
 *      B L B _ g e t _ a r r a y
 *
 **************************************
 *
 * Functional description
 *      Get array blob and array descriptor.
 *
 **************************************/
	BLB blob;
	USHORT n;

	SET_TDBB(tdbb);

	blob = BLB_open2(tdbb, transaction, blob_id, 0, 0);

	if (blob->blb_length < sizeof(struct ads)) {
		BLB_close(tdbb, blob);
		IBERROR(193);			/* msg 193 null or invalid array */
	}

	BLB_get_segment(tdbb, blob, reinterpret_cast < UCHAR * >(desc),
					sizeof(struct ads));

	if ( (n = desc->ads_length - sizeof(struct ads)) )
		BLB_get_segment(tdbb, blob, (UCHAR *) desc + sizeof(struct ads), n);

	return blob;
}


SLONG BLB_get_data(TDBB tdbb, BLB blob, UCHAR* buffer, SLONG length)
{
/**************************************
 *
 *      B L B _ g e t _ d a t a
 *
 **************************************
 *
 * Functional description
 *      Get a large hunk of data from a blob, which can then be
 *      closed.  Return total number of bytes read.  Don't worry
 *      about segment boundaries.
 *
 **************************************/
	BLOB_PTR *p;
	USHORT n;

	SET_TDBB(tdbb);

	p = (BLOB_PTR *) buffer;

	while (length > 0) {
		/* I have no idea why this limit is 32768 instead of 32767
		 * 1994-August-12 David Schnepper
		 */
		n = (USHORT) MIN(length, (SLONG) 32768);
		n = BLB_get_segment(tdbb, blob, p, n);
		p += n;
		length -= n;
		if (blob->blb_flags & BLB_eof)
			break;
	}

	BLB_close(tdbb, blob);

	return (SLONG) ((BLOB_PTR *) p - (BLOB_PTR *) buffer);
}


USHORT BLB_get_segment(TDBB tdbb,
					   BLB blob, UCHAR * segment, USHORT buffer_length)
{
/**************************************
 *
 *      B L B _ g e t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *      Get next segment or fragment from a blob.  Return the number
 *      of bytes returned.
 *
 **************************************/
	DBB dbb;
	BLOB_PTR *from;
	BLOB_PTR *to;
	UCHAR *p;
	WIN window;
	BLP page;
	ISC_STATUS status;
	USHORT length, l, active_page, seek;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;

#ifdef SUPERSERVER

	if (--tdbb->tdbb_quantum < 0 && !tdbb->tdbb_inhibit)
		(void) JRD_reschedule(tdbb, 0, TRUE);

#endif

/* If we reached end of file, we're still there */

	if (blob->blb_flags & BLB_eof)
		return 0;

	if (blob->blb_filter) {
		blob->blb_fragment_size = 0;
		if ( (status =
			BLF_get_segment(tdbb, &blob->blb_filter, &length, buffer_length,
							segment)) ) {
			if (status == gds_segstr_eof)
				blob->blb_flags |= BLB_eof;
			else if (status == gds_segment)
				blob->blb_fragment_size = 1;
			else
				ERR_punt();
		}

		return length;
	}

/* If there is a seek pending, handle it here */

	seek = 0;

	if (blob->blb_flags & BLB_seek) {
		if (blob->blb_seek >= blob->blb_length) {
			blob->blb_flags |= BLB_eof;
			return 0;
		}
		l = dbb->dbb_page_size - BLP_SIZE;
		blob->blb_sequence = blob->blb_seek / l;
		seek = (USHORT)(blob->blb_seek % l);	// safe cast
		blob->blb_flags &= ~BLB_seek;
		blob->blb_fragment_size = 0;
		if (blob->blb_level) {
			blob->blb_space_remaining = 0;
			blob->blb_segment = NULL;
		}
		else {
			blob->blb_space_remaining = blob->blb_length - seek;
			blob->blb_segment = blob->blb_data + seek;
		}
	}

	if (!blob->blb_space_remaining && blob->blb_segment) {
		blob->blb_flags |= BLB_eof;
		return 0;
	}

/* Figure out how much data is to be moved, move it, and if necessary,
   advance to the next page.  The length is a function of segment
   size (or fragment size), buffer size, and amount of data left
   in the blob. */

	to = segment;
	from = blob->blb_segment;
	length = blob->blb_space_remaining;
	active_page = FALSE;
	window.win_flags = 0;
	if (blob->blb_flags & BLB_large_scan) {
		window.win_flags = WIN_large_scan;
		window.win_scans = 1;
	}

	while (TRUE) {

		/* If the blob is segmented, and this isn't a fragment, pick up
		   the length of the next segment. */

		if (SEGMENTED && !blob->blb_fragment_size) {
			while (length < 2) {
				if (active_page) {
					if (window.win_flags & WIN_large_scan)
						CCH_RELEASE_TAIL(tdbb, &window);
					else
						CCH_RELEASE(tdbb, &window);
				}
				if (!(page = get_next_page(tdbb, blob, &window))) {
					blob->blb_flags |= BLB_eof;
					return 0;
				}
				from = (UCHAR *) page->blp_data;
				length = page->blp_length;
				active_page = TRUE;
			}

			p = (UCHAR *) & blob->blb_fragment_size;
			*p++ = *from++;
			*p++ = *from++;
			length -= 2;
		}

		/* Figure out how much data can be moved.  Then account for the
		   space, and move the data */

		l = MIN(buffer_length, length);

		if (SEGMENTED) {
			l = MIN(l, blob->blb_fragment_size);
			blob->blb_fragment_size -= l;
		}

		length -= l;
		buffer_length -= l;
		if (((U_IPTR) from & (ALIGNMENT - 1))
			|| ((U_IPTR) to & (ALIGNMENT - 1)))
			MOVE_FAST(from, to, l);
		else
			MOVE_FASTER(from, to, l);
		to += l;
		from += l;

		/* If we ran out of space in the data clump, and there is a next
		   clump, get it. */

		if (!length) {
			if (active_page) {
				if (window.win_flags & WIN_large_scan)
					CCH_RELEASE_TAIL(tdbb, &window);
				else
					CCH_RELEASE(tdbb, &window);
			}
			if (!(page = get_next_page(tdbb, blob, &window))) {
				active_page = FALSE;
				break;
			}
			from = (UCHAR *) page->blp_data + seek;
			length = page->blp_length - seek;
			seek = 0;
			active_page = TRUE;
		}

		/* If either the buffer or the fragment is exhausted, we're
		   done. */

		if (!buffer_length || (SEGMENTED && !blob->blb_fragment_size))
			break;
	}

	if (active_page) {
		if (((U_IPTR) from & (ALIGNMENT - 1))
			|| ((U_IPTR) blob->blb_data & (ALIGNMENT - 1)))
			MOVE_FAST(from, blob->blb_data, length);
		else
			MOVE_FASTER(from, blob->blb_data, length);
		from = blob->blb_data;
		if (window.win_flags & WIN_large_scan)
			CCH_RELEASE_TAIL(tdbb, &window);
		else
			CCH_RELEASE(tdbb, &window);
	}

	blob->blb_segment = from;
	blob->blb_space_remaining = length;
	length = to - segment;
	blob->blb_seek += length;

/* If this is a stream blob, fake fragment unless we're at the end */

	if (STREAM)
		blob->blb_fragment_size =
			(blob->blb_seek == blob->blb_length) ? 0 : 1;

	return length;
}


SLONG BLB_get_slice(TDBB tdbb,
					JRD_TRA transaction,
					BID blob_id,
					UCHAR * sdl,
					USHORT param_length,
					SLONG * param, SLONG slice_length, UCHAR * slice)
{
/**************************************
 *
 *      B L B _ g e t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *      Fetch a slice of an array.
 *
 **************************************/
	BLB blob;
	ADS desc;
	ISC_STATUS status;
	UCHAR *data;
	SLONG offset, length, variables[64], stuff[ADS_LEN(16) / 4], from, to;
	struct sdl_info info;
	struct slice arg;
        DBB dbb;

	SET_TDBB(tdbb);
        dbb = GET_DBB;
	tdbb->tdbb_default = transaction->tra_pool;

/* Checkout slice description language */

	MOVE_FAST(param, variables, MIN(sizeof(variables), param_length));

	if (SDL_info(tdbb->tdbb_status_vector, sdl, &info, variables))
		ERR_punt();

	desc = (ADS) stuff;
	blob = BLB_get_array(tdbb, transaction, blob_id, desc);
	length = desc->ads_total_length;

/* Get someplace to put data */

	data = (UCHAR*) dbb->dbb_permanent->allocate(desc->ads_total_length, 0
#ifdef DEBUG_GDS_ALLOC
	  ,__FILE__, __LINE__
#endif
	);

/* zero out memory, so that it does not have to be done for
   each element */

	memset(data, 0, desc->ads_total_length);

	offset = 0;

/* Trap any potential errors */

	try {

/* If we know something about the subscript bounds, prepare
   to fetch only stuff we really care about */

	if (info.sdl_info_dimensions) {
		from =
			SDL_compute_subscript(tdbb->tdbb_status_vector, desc,
								  info.sdl_info_dimensions,
								  info.sdl_info_lower);
		to =
			SDL_compute_subscript(tdbb->tdbb_status_vector, desc,
								  info.sdl_info_dimensions,
								  info.sdl_info_upper);
		if (from != -1 && to != -1) {
			if (from) {
				offset = from * desc->ads_element_length;
				BLB_lseek(blob, 0, offset + (SLONG) desc->ads_length);
			}
			length = (to - from + 1) * desc->ads_element_length;
		}
	}

	length = BLB_get_data(tdbb, blob, data + offset, length) + offset;

/* Walk array */

	arg.slice_desc = info.sdl_info_element;
	arg.slice_desc.dsc_address = slice;
	arg.slice_end = (BLOB_PTR *) slice + slice_length;
	arg.slice_count = 0;
	arg.slice_element_length = info.sdl_info_element.dsc_length;
	arg.slice_direction = FALSE;	/* fetching from array */
	arg.slice_high_water = (BLOB_PTR *) data + length;
	arg.slice_base = (BLOB_PTR *) data + offset;

	status = SDL_walk(tdbb->tdbb_status_vector,
					  sdl,
					  TRUE,
					  data,
					  desc,
					  variables,
					  reinterpret_cast < void (*)() > (slice_callback),
					  reinterpret_cast < struct slice *>(&arg));

	dbb->dbb_permanent->deallocate(data);

	if (status) {
		ERR_punt();
	}

	}	// try
	catch (const std::exception&) {
		dbb->dbb_permanent->deallocate(data);
		ERR_punt();
	}

	return (SLONG) (arg.slice_count * arg.slice_element_length);
}


SLONG BLB_lseek(BLB blob, USHORT mode, SLONG offset)
{
/**************************************
 *
 *      B L B _ l s e e k
 *
 **************************************
 *
 * Functional description
 *      Position a blob for direct access.  Seek is only defined for stream
 *      type blobs.
 *
 **************************************/

	if (!(blob->blb_flags & BLB_stream))
		ERR_post(gds_bad_segstr_type, 0);

	if (mode == 1)
		offset += blob->blb_seek;
	else if (mode == 2)
		offset = blob->blb_length + offset;

	if (offset < 0)
		offset = 0;

	if (offset > (SLONG) blob->blb_length)
		offset = blob->blb_length;

	blob->blb_seek = offset;
	blob->blb_flags |= BLB_seek;
	blob->blb_flags &= ~BLB_eof;

	return offset;
}


#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM

extern "C" {

void DLL_EXPORT BLB_map_blobs(TDBB tdbb, BLB old_blob, BLB new_blob)
{
/**************************************
 *
 *      B L B _ m a p _ b l o b s
 *
 **************************************
 *
 * Functional description
 *      Form a mapping between two blobs.
 *      Since the blobs have been newly created
 *      in this session, only the second part of
 *      the blob id is significant.  At the moment
 *      this is intended solely for REPLAY, when
 *      replaying a log.
 *
 **************************************/
	DBB dbb;
	MAP new_map;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	new_map = FB_NEW(*dbb->dbb_permanent) map();
	new_map->map_old_blob = old_blob;
	new_map->map_new_blob = new_blob;

	new_map->map_next = dbb->dbb_blob_map;
	dbb->dbb_blob_map = new_map;
}

}	// extern "C"

#endif	// REPLAY_OSRI_API_CALLS_SUBSYSTEM


void BLB_move(TDBB tdbb, DSC * from_desc, DSC * to_desc, JRD_NOD field)
{
/**************************************
 *
 *      B L B _ m o v e
 *
 **************************************
 *
 * Functional description
 *      Perform an assignment to a blob field.  Unless the blob is null,
 *      this requires that either a temporary blob be materialized or that
 *      a permanent blob be copied.  Note: it is illegal to have a blob
 *      field in a message.
 *
 **************************************/
	BLB blob;
	ARR array;
	BID source, destination;
	USHORT id, materialized_blob, refetch_flag;
	JRD_REQ request;
	RPB *rpb;
	REC record;
	JRD_REL relation;
	JRD_TRA transaction;

	SET_TDBB(tdbb);

	if (field->nod_type != nod_field)
		BUGCHECK(199);			/* msg 199 expected field node */

	if (from_desc->dsc_dtype != dtype_quad
		&& from_desc->dsc_dtype != dtype_blob) ERR_post(gds_convert_error,
														gds_arg_string,
														"BLOB", 0);

	request = tdbb->tdbb_request;
	source = (BID) from_desc->dsc_address;
	destination = (BID) to_desc->dsc_address;
	id = (USHORT) field->nod_arg[e_fld_id];
	rpb = &request->req_rpb[(int) field->nod_arg[e_fld_stream]];
	relation = rpb->rpb_relation;
	record = rpb->rpb_record;

/* If nothing changed, don't do nothing.  If it isn't broken,
   don't fix it. */

	if (source->bid_relation_id == destination->bid_relation_id &&
		source->bid_stuff.bid_number == destination->bid_stuff.bid_number)
		return;

/* If either the source value is null or the blob id itself is null (all
   zeros, then the blob is null. */

	if ((request->req_flags & req_null) || (!source->bid_relation_id &&
											!source->bid_stuff.bid_blob)) {
		SET_NULL(record, id);
		destination->bid_relation_id = 0;
		destination->bid_stuff.bid_number = 0;
		return;
	}

	CLEAR_NULL(record, id);
	transaction = request->req_transaction;

/* If the target is a view, this must be from a view update trigger.
   Just pass the blob id thru */

	if (relation->rel_view_rse) {
		*destination = *source;
		return;
	}

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
/* for REPLAY, map blob id's from the original session */

	get_replay_blob(tdbb, source);
#endif

/* If the source is a permanent blob, then the blob must be copied.
   Otherwise find the temporary blob referenced.  */

	array = 0;

	do {
		materialized_blob = refetch_flag = FALSE;
		if (source->bid_relation_id)
			blob = copy_blob(tdbb, source, relation, destination);
		else if ((to_desc->dsc_dtype == dtype_array) &&
				 (array = find_array(transaction, source)) &&
				 (blob = store_array(tdbb, transaction, source)))
			materialized_blob = TRUE;
		else
			for (blob = transaction->tra_blobs; blob; blob = blob->blb_next)
				if (blob == source->bid_stuff.bid_blob)
				{
					materialized_blob = TRUE;
					break;
				}

		if (!blob ||
			MemoryPool::blk_type(blob) != type_blb ||
			blob->blb_attachment != tdbb->tdbb_attachment ||
			!(blob->blb_flags & BLB_closed) ||
			(blob->blb_request && blob->blb_request != request))
			ERR_post(gds_bad_segstr_id, 0);

		if (materialized_blob && !(blob->blb_flags & BLB_temporary)) {
			refetch_flag = TRUE;
			source = &blob->blb_blob_id;
		}
	} while (refetch_flag);

	blob->blb_relation = relation;
	destination->bid_relation_id = relation->rel_id;
	destination->bid_stuff.bid_number = DPM_store_blob(tdbb, blob, record);
	if (materialized_blob) {
		blob->blb_flags &= ~BLB_temporary;
		blob->blb_blob_id = *destination;
		blob->blb_request = request;
		if (array)
			array->arr_request = request;
	}
	release_blob(blob, (materialized_blob) ? FALSE : TRUE);
}


void BLB_move_from_string(TDBB tdbb, DSC * from_desc, DSC * to_desc, JRD_NOD field)
{
/**************************************
 *
 *      B L B _ m o v e _ f r o m _ s t r i n g
 *
 **************************************
 *
 * Functional description
 *      Perform an assignment to a blob field.  It's capable of handling
 *      strings by doing an internal conversion to blob and then calling
 *      BLB_move with that new blob.
 *
 **************************************/
	SET_TDBB (tdbb);

	if (from_desc->dsc_dtype > dtype_varying)
	    ERR_post(gds_convert_error, gds_arg_string,
		DSC_dtype_tostring(from_desc->dsc_dtype), 0);
	else
	{
		USHORT ttype = 0;
		BLB blob = 0;
		UCHAR *fromstr = 0;
		struct bid temp_bid;
		DSC blob_desc;
		MOVE_CLEAR(&temp_bid, sizeof(temp_bid));
		MOVE_CLEAR(&blob_desc, sizeof(blob_desc));
		blob = BLB_create(tdbb, tdbb->tdbb_request->req_transaction, &temp_bid);
		blob_desc.dsc_length = MOV_get_string_ptr(from_desc, &ttype, &fromstr, 0, 0);
		if (from_desc->dsc_sub_type == BLOB_text)
		{
		/* I have doubts on the merits of this charset assignment since BLB_create2
		calculates charset internally and assigns it to fields inside blb struct.
		I really need to call BLB_create2 and provide more parameters.
		This macro is useless here as it doesn't cater for blob fields because
		desc.dsc_ttype is desc.dsc_sub_type but blobs use dsc_scale for the charset
		and dsc_sub_type for blob sub_types, IE text.
		INTL_ASSIGN_TTYPE (&blob_desc, ttype);
		*/
			blob_desc.dsc_scale = ttype;
		}
		else
		{
			blob_desc.dsc_scale = ttype_none;
		}
		blob_desc.dsc_dtype = dtype_blob;
		blob_desc.dsc_address = reinterpret_cast<UCHAR*>(&temp_bid);
		BLB_put_segment(tdbb, blob, fromstr, blob_desc.dsc_length);
		BLB_close(tdbb, blob);
		BLB_move(tdbb, &blob_desc, to_desc, field);
	}
}


BLB BLB_open(TDBB tdbb, JRD_TRA transaction, BID blob_id)
{
/**************************************
 *
 *      B L B _ o p e n
 *
 **************************************
 *
 * Functional description
 *      Open an existing blob.
 *
 **************************************/

	SET_TDBB(tdbb);
	return BLB_open2(tdbb, transaction, blob_id, 0, 0);
}


BLB BLB_open2(TDBB tdbb,
			  JRD_TRA transaction, BID blob_id, USHORT bpb_length, UCHAR * bpb)
{
/**************************************
 *
 *      B L B _ o p e n 2
 *
 **************************************
 *
 * Functional description
 *      Open an existing blob.
 *      Basically BLB_open() with BPB structure.
 *
 **************************************/
	DBB dbb;
	BLB blob, new_;
	VCL new_pages, pages;
	CTL control;
	SSHORT from, to;
	SSHORT from_charset, to_charset;
	BLF filter;
	USHORT filter_required = FALSE;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;

/* Handle filter case */

	gds__parse_bpb2(bpb_length,
					bpb,
					reinterpret_cast < SSHORT * >(&from),
					reinterpret_cast < SSHORT * >(&to),
					reinterpret_cast < USHORT * >(&from_charset),
					reinterpret_cast < USHORT * >(&to_charset));

	blob = allocate_blob(tdbb, transaction);

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
/* for REPLAY, map blob id's from the original session */

	get_replay_blob(tdbb, blob_id);
#endif

	blob->blb_target_interp = to_charset;
	blob->blb_source_interp = from_charset;

	filter = NULL;
	if (to && from != to) {
		filter = find_filter(tdbb, from, to);
		filter_required = TRUE;
	}
	else if (to == BLOB_text && (from_charset != to_charset)) {
		if (from_charset == CS_dynamic)
			from_charset = tdbb->tdbb_attachment->att_charset;
		if (to_charset == CS_dynamic)
			to_charset = tdbb->tdbb_attachment->att_charset;
		if ((to_charset != CS_NONE) && (from_charset != to_charset)) {
			filter = FB_NEW(*dbb->dbb_permanent) blf();
			filter->blf_filter =
				reinterpret_cast < ISC_STATUS (*) (USHORT, CTL) > (filter_transliterate_text);
			filter_required = TRUE;
		}
	}

	if (filter_required) {
		if (BLF_open_blob(tdbb,
						  transaction,
						  &control,
						  reinterpret_cast < long *>(blob_id),
						  bpb_length,
						  bpb,
						  reinterpret_cast < long (*)() > (blob_filter),
						  filter)) ERR_punt();
		blob->blb_filter = control;
		blob->blb_max_segment = control->ctl_max_segment;
		blob->blb_count = control->ctl_number_segments;
		blob->blb_length = control->ctl_total_length;
		return blob;
	}

	if (!blob_id->bid_relation_id)
		if (!blob_id->bid_stuff.bid_number)
		{
			blob->blb_flags |= BLB_eof;
			return blob;
		}
		else {
			/* Note: Prior to 1991, we would immediately report bad_segstr_id here,
			 * but then we decided to allow a newly created blob to be openned,
			 * leaving the possibility of receiving a garbage blob ID from
			 * the application.
			 * The following does some checks to try and product ourselves
			 * better.  94-Jan-07 Daves.
			 */

			/* Search the list of transaction blobs for a match */
			for (new_ = transaction->tra_blobs; new_; new_ = new_->blb_next)
				if (new_ == blob_id->bid_stuff.bid_blob)
					break;

			check_BID_validity(new_, tdbb);

			blob->blb_lead_page = new_->blb_lead_page;
			blob->blb_max_sequence = new_->blb_max_sequence;
			blob->blb_count = new_->blb_count;
			blob->blb_length = new_->blb_length;
			blob->blb_max_segment = new_->blb_max_segment;
			blob->blb_level = new_->blb_level;
			blob->blb_flags = new_->blb_flags & BLB_stream;
			pages = new_->blb_pages;
			if (pages) {
				new_pages = vcl::newVector(*transaction->tra_pool, *pages);
				blob->blb_pages = new_pages;
			}
			if (blob->blb_level == 0) {
				blob->blb_space_remaining =
					new_->blb_clump_size - new_->blb_space_remaining;
				blob->blb_segment =
					(UCHAR *) ((BLP) new_->blb_data)->blp_data;
			}
			return blob;
		}

/* Ordinarily, we would call MET_relation to get the relation id.
   However, since the blob id must be consider suspect, this is
   not a good idea.  On the other hand, if we don't already
   know about the relation, the blob id has got to be invalid
   anyway. */

	VEC vector = dbb->dbb_relations;

	if (blob_id->bid_relation_id >= vector->count() ||
		!(blob->blb_relation =
		  reinterpret_cast<jrd_rel*>( (*vector)[blob_id->bid_relation_id]) ) )
	{
			ERR_post(gds_bad_segstr_id, 0);
	}

	DPM_get_blob(tdbb, blob, blob_id->bid_stuff.bid_number, FALSE, (SLONG) 0);

/* If the blob is known to be damaged, ignore it. */

	if (blob->blb_flags & BLB_damaged) {
		if (!(dbb->dbb_flags & DBB_damaged))
			IBERROR(194);		/* msg 194 blob not found */
		blob->blb_flags |= BLB_eof;
		return blob;
	}

/* Get first data page in anticipation of reading. */

	if (blob->blb_level == 0)
		blob->blb_segment = blob->blb_data;

	return blob;
}


void BLB_put_segment(TDBB tdbb, BLB blob, UCHAR* seg, USHORT segment_length)
{
/**************************************
 *
 *      B L B _ p u t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *      Add a segment to a blob.
 *
 **************************************/
	DBB dbb;
	BLOB_PTR *segment;
	BLOB_PTR *p;
	BLOB_PTR *q;
	UCHAR length_flag;
	ULONG length;				/* length of segment + overhead */
	USHORT l;
	BLP page;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	segment = (BLOB_PTR *) seg;

/* Make sure blob is a temporary blob.  If not, complain bitterly. */

	if (!(blob->blb_flags & BLB_temporary))
		IBERROR(195);			/* msg 195 cannot update old blob */

	if (blob->blb_filter) {
		if (BLF_put_segment(tdbb, &blob->blb_filter, segment_length, segment))
			ERR_punt();

		return;
	}

/* Account for new segment */

	blob->blb_count++;
	blob->blb_length += segment_length;

	if (segment_length > blob->blb_max_segment)
		blob->blb_max_segment = segment_length;

/* Compute the effective length of the segment (counts length unless
   the blob is a stream blob). */

	if (SEGMENTED) {
		length = segment_length + 2;
		length_flag = TRUE;
	}
	else {
		length = segment_length;
		length_flag = FALSE;
	}

/* Case 0: Transition from small blob to medium size blob.  This really
   just does a form transformation and drops into the next case. */

	if (blob->blb_level == 0 && length > (ULONG) blob->blb_space_remaining) {
		JRD_TRA transaction;

		transaction = blob->blb_transaction;
		blob->blb_pages = vcl::newVector(*transaction->tra_pool, 0);
		l = dbb->dbb_page_size - BLP_SIZE;
		blob->blb_space_remaining += l - blob->blb_clump_size;
		blob->blb_clump_size = l;
		blob->blb_level = 1;
	}

/* Case 1: The segment fits.  In what is immaterial.  Just move the segment
   and get out! */

	p = blob->blb_segment;

	if (length_flag && blob->blb_space_remaining >= 2) {
		q = (UCHAR *) & segment_length;
		*p++ = *q++;
		*p++ = *q++;
		blob->blb_space_remaining -= 2;
		length_flag = FALSE;
	}

	if (!length_flag && segment_length <= blob->blb_space_remaining) {
		blob->blb_space_remaining -= segment_length;
		if (((U_IPTR) segment & (ALIGNMENT - 1))
			|| ((U_IPTR) p & (ALIGNMENT - 1)))
			MOVE_FAST(segment, p, segment_length);
		else
			MOVE_FASTER(segment, p, segment_length);
		blob->blb_segment = p + segment_length;
		return;
	}

/* The segment cannot be contained in the current clump.  What does
   bit is copied to the current page image.  Then allocate a page to
   hold the current page, copy the page image to the buffer, and release
   the page.  Since a segment can be much larger than a page, this whole
   mess is done in a loop. */

	while (length_flag || segment_length) {

		/* Move what fits.  At this point, the length is known not to fit. */

		l = MIN(segment_length, blob->blb_space_remaining);

		if (!length_flag && l) {
			segment_length -= l;
			blob->blb_space_remaining -= l;
			if (((U_IPTR) segment & (ALIGNMENT - 1))
				|| ((U_IPTR) p & (ALIGNMENT - 1)))
				MOVE_FAST(segment, p, l);
			else
				MOVE_FASTER(segment, p, l);
			p += l;
			segment += l;
			if (segment_length == 0) {
				blob->blb_segment = p;
				return;
			}
		}

		/* Data page is full.  Add the page to the blob data structure. */

		insert_page(tdbb, blob);
		blob->blb_sequence++;

		/* Get ready to start filling the next page. */

		page = (BLP) blob->blb_data;
		p = blob->blb_segment = (UCHAR *) page->blp_data;
		blob->blb_space_remaining = blob->blb_clump_size;

		/* If there's still a length waiting to be moved, move it already! */

		if (length_flag) {
			q = (UCHAR *) & segment_length;
			*p++ = *q++;
			*p++ = *q++;
			blob->blb_space_remaining -= 2;
			length_flag = FALSE;
			blob->blb_segment = p;
		}
	}

}


void BLB_put_slice(	TDBB	tdbb,
					JRD_TRA		transaction,
					BID		blob_id,
					UCHAR*	sdl,
					USHORT	param_length,
					SLONG*	param,
					SLONG	slice_length,
					UCHAR*	slice)
{
/**************************************
 *
 *      B L B _ p u t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *      Put a slice of an array.
 *
 **************************************/
	JRD_REL		relation;
	ARR		array;
	SLONG	variables[64];
	SLONG	temp[ADS_LEN(16) / 4];
	SSHORT	n;
	struct	sdl_info info;
	struct	slice arg;

	SET_TDBB(tdbb);
	tdbb->tdbb_default = transaction->tra_pool;

/* Do initial parse of slice description to get relation and field identification */

	if (SDL_info(tdbb->tdbb_status_vector, sdl, &info, 0))
		ERR_punt();

	if (info.sdl_info_relation[0]) {
		relation = MET_lookup_relation(tdbb, info.sdl_info_relation);
	} else {
		relation = MET_relation(tdbb, info.sdl_info_rid);
	}

	if (!relation) {
		IBERROR(196);			/* msg 196 relation for array not known */
	}

	if (info.sdl_info_field[0]) {
	    n = MET_lookup_field(tdbb, relation, info.sdl_info_field, 0);
	} else {
		n = info.sdl_info_fid;
	}

/* Make sure relation is scanned */
	MET_scan_relation(tdbb, relation);

	JRD_FLD field;
	if (n < 0 || !(field = MET_get_field(relation, n))) {
		IBERROR(197);			/* msg 197 field for array not known */
	}

	ARR array_desc = field->fld_array;
	if (!array_desc)
	{
		ERR_post(gds_invalid_dimension, gds_arg_number, (SLONG) 0,
				 gds_arg_number, (SLONG) 1, 0);
	}

/* Find and/or allocate array block.  There are three distinct cases:

	1.  Array is totally new.
	2.  Array is still in "temporary" state.
	3.  Array exists and is being updated.
*/

	if (blob_id->bid_relation_id)
	{
		for (array = transaction->tra_arrays; array; array = array->arr_next)
		{
			if (array->arr_blob &&
				array->arr_blob->blb_blob_id.bid_relation_id ==
				blob_id->bid_relation_id &&
				array->arr_blob->blb_blob_id.bid_stuff.bid_number ==
				blob_id->bid_stuff.bid_number)
			{
				break;
			}
		}
		if (array)
		{
			arg.slice_high_water =
				(BLOB_PTR*) array->arr_data + array->arr_effective_length;
		}
		else
		{
			BLB blob =
				BLB_get_array(	tdbb,
								transaction,
								blob_id,
								reinterpret_cast<ads*>(temp));
			array =
				alloc_array(transaction, reinterpret_cast<ads*>(temp));
			array->arr_effective_length =
				blob->blb_length - array->arr_desc.ads_length;
			BLB_get_data(tdbb, blob, array->arr_data,
						 array->arr_desc.ads_total_length);
			arg.slice_high_water =
				(BLOB_PTR*) array->arr_data + array->arr_effective_length;
			array->arr_blob = allocate_blob(tdbb, transaction);
			(array->arr_blob)->blb_blob_id = *blob_id;
		}
	}
	else if (blob_id->bid_stuff.bid_blob)
	{
		array = find_array(transaction, blob_id);
		if (!array) {
			ERR_post(isc_invalid_array_id, 0);
		}

		arg.slice_high_water =
			(BLOB_PTR *) array->arr_data + array->arr_effective_length;
	}
	else {
		array = alloc_array(transaction, &array_desc->arr_desc);
		arg.slice_high_water = (BLOB_PTR *) array->arr_data;
	}

/* Walk array */

	arg.slice_desc = info.sdl_info_element;
	arg.slice_desc.dsc_address = slice;
	arg.slice_end = (BLOB_PTR *) slice + slice_length;
	arg.slice_count = 0;
	arg.slice_element_length = info.sdl_info_element.dsc_length;
	arg.slice_direction = TRUE;	/* storing INTO array */
	arg.slice_base = (BLOB_PTR *) array->arr_data;
	MOVE_FAST(param, variables, MIN(sizeof(variables), param_length));

	if (SDL_walk(tdbb->tdbb_status_vector,
				 sdl,
				 TRUE,
				 array->arr_data,
				 &array_desc->arr_desc,
				 variables,
				 reinterpret_cast<void (*)()>(slice_callback),
				 &arg))
	{
		ERR_punt();
	}

	SLONG length = (BLOB_PTR*)arg.slice_high_water - (BLOB_PTR*)array->arr_data;

	if (length > array->arr_effective_length) {
		array->arr_effective_length = length;
	}

	blob_id->bid_stuff.bid_blob = (BLB) array;
	blob_id->bid_relation_id = 0;
}


void BLB_release_array(ARR array)
{
/**************************************
 *
 *      B L B _ r e l e a s e _ a r r a y
 *
 **************************************
 *
 * Functional description
 *      Release an array block and friends and relations.
 *
 **************************************/

	if (array->arr_data) {
		MemoryPool::globalFree(array->arr_data); // But know that it comes from permanent pool
	}

	JRD_TRA transaction  = array->arr_transaction;
	if (transaction)
	{
		ARR* ptr;
		for (ptr = &transaction->tra_arrays; *ptr; ptr = &(*ptr)->arr_next) {
			if (*ptr == array) {
				*ptr = array->arr_next;
				break;
			}
		}
	}

	delete array;
}


void BLB_scalar(TDBB	tdbb,
				JRD_TRA		transaction,
				BID		blob_id,
				USHORT	count,
				SLONG*	subscripts,
				VLU		value)
{
/**************************************
 *
 *      B L B _ s c a l a r
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	SLONG stuff[ADS_LEN(16) / 4];
	double temp[64];
	STR temp_str;

	SET_TDBB(tdbb);

	ADS array_desc = (ADS) stuff;
	BLB blob = BLB_get_array(tdbb, transaction, blob_id, array_desc);

/* Get someplace to put data.  If the local buffer isn't large enough,
   allocate one that is. */

	DSC desc = array_desc->ads_rpt[0].ads_desc;
	if (desc.dsc_length <= sizeof(temp)) {
		desc.dsc_address = (UCHAR*) temp;
	} else {
		temp_str =
			FB_NEW_RPT(*tdbb->tdbb_default, desc.dsc_length + DOUBLE_ALIGN - 1) str;
		desc.dsc_address =
			(UCHAR *) FB_ALIGN((U_IPTR) temp_str->str_data, DOUBLE_ALIGN);
	}

	SLONG number =
		SDL_compute_subscript(tdbb->tdbb_status_vector, array_desc, count,
							  subscripts);

	if (number < 0) {
		BLB_close(tdbb, blob);
		if (desc.dsc_address != (UCHAR *) temp) {
			delete temp_str;
		}
		ERR_punt();
	}

	SLONG offset = number * array_desc->ads_element_length;
	BLB_lseek(blob, 0, offset + (SLONG) array_desc->ads_length);
	BLB_get_segment(tdbb, blob, reinterpret_cast < UCHAR * >(temp),
					desc.dsc_length);

/* If we have run out of data, then clear the data buffer. */

	if (blob->blb_flags & BLB_eof) {
		memset(desc.dsc_address, 0, (int) desc.dsc_length);
	}
	EVL_make_value(tdbb, &desc, value);
	BLB_close(tdbb, blob);
	if (desc.dsc_address != (UCHAR *) temp) {
		delete temp_str;
	}
}


static ARR alloc_array(JRD_TRA transaction, ADS proto_desc)
{
/**************************************
 *
 *      a l l o c _ a r r a y
 *
 **************************************
 *
 * Functional description
 *      Allocate an array block based on a prototype array descriptor.
 *
 **************************************/

	DBB dbb = GET_DBB;

	// Compute size and allocate block

	USHORT n = MAX(proto_desc->ads_struct_count, proto_desc->ads_dimensions);
	ARR array = FB_NEW_RPT(*transaction->tra_pool, n) arr();

	// Copy prototype descriptor

	MOVE_FAST(proto_desc, &array->arr_desc, proto_desc->ads_length);

	// Link into transaction block

	array->arr_next = transaction->tra_arrays;
	transaction->tra_arrays = array;
	array->arr_transaction = transaction;

	// Allocate large block to hold array

	array->arr_data =
		(UCHAR*)dbb->dbb_permanent->allocate(array->arr_desc.ads_total_length, 0
#ifdef DEBUG_GDS_ALLOC
		  ,__FILE__, __LINE__
#endif
		);

	return array;
}


static BLB allocate_blob(TDBB tdbb, JRD_TRA transaction)
{
/**************************************
 *
 *      a l l o c a t e _ b l o b
 *
 **************************************
 *
 * Functional description
 *      Create a shiney, new, empty blob.
 *
 **************************************/

	SET_TDBB(tdbb);
	DBB dbb = tdbb->tdbb_database;

/* Create a blob large enough to hold a single data page */

	BLB blob = FB_NEW_RPT(*transaction->tra_pool, dbb->dbb_page_size) blb();
	blob->blb_attachment = tdbb->tdbb_attachment;
	blob->blb_next = transaction->tra_blobs;
	transaction->tra_blobs = blob;
	blob->blb_transaction = transaction;

/* Compute some parameters governing various maximum sizes based on
   database page size. */

	blob->blb_clump_size = dbb->dbb_page_size -
							sizeof(struct dpg) -
							sizeof(dpg::dpg_repeat) -
							sizeof(struct blh);
	blob->blb_max_pages = blob->blb_clump_size >> SHIFTLONG;
	blob->blb_pointers = (dbb->dbb_page_size - BLP_SIZE) >> SHIFTLONG;

	return blob;
}


static ISC_STATUS blob_filter(	USHORT	action,
							CTL		control,
							SSHORT	mode,
							SLONG	offset)
{
/**************************************
 *
 *      b l o b _ f i l t e r
 *
 **************************************
 *
 * Functional description
 *      Filter of last resort for filtered blob access handled by Y-valve.
 *
 **************************************/
	BLB blob;

/* Note: Cannot remove this GET_THREAD_DATA without API change to
   blob filter routines */

	TDBB tdbb = GET_THREAD_DATA;

	JRD_TRA transaction = (JRD_TRA) control->ctl_internal[1];
	SLONG* blob_id = (SLONG *) control->ctl_internal[2];

#ifdef DEV_BUILD
	if (transaction) {
		BLKCHK(transaction, type_tra);
	}
#endif

	switch (action) {
	case ACTION_open:
		blob =
			BLB_open2(	tdbb,
						transaction,
						reinterpret_cast<bid*>(blob_id),
						0,
						0);
		control->ctl_source_handle = (CTL) blob;
		control->ctl_total_length = blob->blb_length;
		control->ctl_max_segment = blob->blb_max_segment;
		control->ctl_number_segments = blob->blb_count;
		return FB_SUCCESS;

	case ACTION_get_segment:
		blob = (BLB) control->ctl_source_handle;
		control->ctl_segment_length =
			BLB_get_segment(tdbb, blob, control->ctl_buffer,
							control->ctl_buffer_length);
		if (blob->blb_flags & BLB_eof) {
			return gds_segstr_eof;
		}
		if (blob->blb_fragment_size) {
			return gds_segment;
		}
		return FB_SUCCESS;

	case ACTION_create:
		control->ctl_source_handle =
			(CTL) BLB_create2(tdbb, transaction,
							  reinterpret_cast<bid*>(blob_id), 0,
							  (UCHAR *) 0);
		return FB_SUCCESS;

	case ACTION_put_segment:
		blob = (BLB) control->ctl_source_handle;
		BLB_put_segment(tdbb, blob, control->ctl_buffer,
						control->ctl_buffer_length);
		return FB_SUCCESS;

	case ACTION_close:
		BLB_close(tdbb,
				  reinterpret_cast<blb*>(control->ctl_source_handle));
		return FB_SUCCESS;

	case ACTION_alloc:
		return (ISC_STATUS) FB_NEW(*transaction->tra_pool) ctl();

	case ACTION_free:
		delete control;
		return FB_SUCCESS;

	case ACTION_seek:
		return BLB_lseek((BLB) control->ctl_source_handle, mode, offset);

	default:
		ERR_post(gds_uns_ext, 0);
		return FB_SUCCESS;
	}
}


static void check_BID_validity(BLB blob, TDBB tdbb)
{
/**************************************
 *
 *      c h e c k _ B I D _ v a l i d i t y
 *
 **************************************
 *
 * Functional description
 *      There are times when an application passes the engine
 *      a BID, which we then assume points to a valid BLB structure.
 *      Specifically, this can occur when an application is trying
 *      to open a newly created blob (that doesn't have a relation
 *      ID assigned).
 *
 *      However, it is quite possible that garbage BID is passed in
 *      from an application; resulting in core dumps within the engine
 *      due to trying to make use of the information.
 *
 *      This function takes a BID's pointer to a blb, and performs
 *      some validity checks on it.  It can't catch all possible
 *      garbage inputs from an application, but should catch
 *      many of them.
 *
 *      Note that we can't BUGCHECK or BLKCHK here, this is an
 *      application level error.
 *
 *      94-Jan-07 Daves
 *
 **************************************/

	if (!blob ||
		MemoryPool::blk_type(blob) != type_blb ||
		blob->blb_attachment != tdbb->tdbb_attachment ||
		blob->blb_level > 2 || !(blob->blb_flags & BLB_temporary))
	{
		ERR_post(gds_bad_segstr_id, 0);
	}
}


static BLB copy_blob(TDBB tdbb, BID source, JRD_REL relation, BID destination)
{
/**************************************
 *
 *      c o p y _ b l o b
 *
 **************************************
 *
 * Functional description
 *      Make a copy of an existing blob.
 *
 **************************************/

	UCHAR buffer[2000];
	UCHAR* buff;
	STR string;

	SET_TDBB(tdbb);

	JRD_REQ request = tdbb->tdbb_request;
	BLB input = BLB_open(tdbb, request->req_transaction, source);
	BLB output = BLB_create(tdbb, request->req_transaction, destination);
	output->blb_sub_type = input->blb_sub_type;

	if (input->blb_flags & BLB_stream) {
		output->blb_flags |= BLB_stream;
	}

	if (input->blb_max_segment > sizeof(buffer))

	{
		string = FB_NEW_RPT(*tdbb->tdbb_default, input->blb_max_segment) str();
		buff = (UCHAR *) string->str_data;
	}
	else {
		string = NULL;
		buff = buffer;
	}

	while (TRUE) {
		USHORT length = BLB_get_segment(tdbb, input, buff, input->blb_max_segment);
		if (input->blb_flags & BLB_eof) {
			break;
		}
		BLB_put_segment(tdbb, output, buff, length);
	}

	delete string;

	BLB_close(tdbb, input);
	BLB_close(tdbb, output);

	return output;
}


static void delete_blob(TDBB tdbb, BLB blob, ULONG prior_page)
{
/**************************************
 *
 *      d e l e t e _ b l o b
 *
 **************************************
 *
 * Functional description
 *      Delete all disk storage associated with blob.  This can be used
 *      to either abort a temporary blob or get rid of an unwanted and
 *      unloved permanent blob.  The routine deletes only blob page --
 *      somebody else will have to worry about the blob root.
 *
 **************************************/
	DBB dbb;
	VCL vector;
	WIN window;
	BLP page;
	vcl::iterator ptr, end;
	SLONG *ptr2, *end2;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	if (dbb->dbb_flags & DBB_read_only)
		ERR_post(isc_read_only_database, 0);

/* Level 0 blobs don't need cleanup */

	if (blob->blb_level == 0)
		return;

/* Level 1 blobs just need the root page level released */

	vector = blob->blb_pages;
	ptr = vector->begin();
	end = vector->end();

	if (blob->blb_level == 1) {
		for (; ptr < end; ptr++)
			if (*ptr)
				PAG_release_page(*ptr, prior_page);
		return;
	}

/* Level 2 blobs need a little more work to keep the page precedence
   in order.  The basic problem is that the pointer page has to be
   released before the data pages that it points to.  Sigh. */

	window.win_flags = WIN_large_scan;
	window.win_scans = 1;

	for (; ptr < end; ptr++)
		if ( (window.win_page = *ptr) ) {
			page = (BLP) CCH_FETCH(tdbb, &window, LCK_read, pag_blob);
			MOVE_FASTER(page, blob->blb_data, dbb->dbb_page_size);
			CCH_RELEASE_TAIL(tdbb, &window);
			PAG_release_page(*ptr, prior_page);
			page = (BLP) blob->blb_data;
			for (ptr2 = page->blp_page, end2 = ptr2 + blob->blb_pointers;
				 ptr2 < end2; ptr2++)
				if (*ptr2)
					PAG_release_page(*ptr2, *ptr);
		}
}


static void delete_blob_id(
						   TDBB tdbb,
						   BID blob_id, ULONG prior_page, JRD_REL relation)
{
/**************************************
 *
 *      d e l e t e _ b l o b _ i d
 *
 **************************************
 *
 * Functional description
 *      Delete an existing blob for purposed of garbage collection.
 *
 **************************************/
	DBB dbb;
	BLB blob;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* If the blob is null, don't both to delete it.  Reasonable? */

	if (!blob_id->bid_stuff.bid_number && !blob_id->bid_relation_id)
		return;

	if (blob_id->bid_relation_id != relation->rel_id)
		CORRUPT(200);			/* msg 200 invalid blob id */

/* Fetch blob */

	blob = allocate_blob(tdbb, dbb->dbb_sys_trans);
	blob->blb_relation = relation;
	prior_page =
		DPM_get_blob(tdbb, blob, blob_id->bid_stuff.bid_number, TRUE,
					 prior_page);

	if (!(blob->blb_flags & BLB_damaged))
		delete_blob(tdbb, blob, prior_page);

	release_blob(blob, TRUE);
}


static ARR find_array(JRD_TRA transaction, BID blob_id)
{
/**************************************
 *
 *      f i n d _ a r r a y
 *
 **************************************
 *
 * Functional description
 *      Find array from temporary blob id.
 *
 **************************************/
	ARR array;

	for (array = transaction->tra_arrays; array; array = array->arr_next)
		if (array == (ARR) blob_id->bid_stuff.bid_blob)
			break;

	return array;
}


static BLF find_filter(TDBB tdbb, SSHORT from, SSHORT to)
{
/**************************************
 *
 *      f i n d _ f i l t e r
 *
 **************************************
 *
 * Functional description
 *      Find blob filter.
 *
 **************************************/
	DBB dbb;
	BLF cache;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	for (cache = dbb->dbb_blob_filters; cache; cache = cache->blf_next)
		if (cache->blf_from == from && cache->blf_to == to)
			return cache;

	if (!(cache = BLF_lookup_internal_filter(tdbb, from, to)))
		cache = MET_lookup_filter(tdbb, from, to);

	if (cache) {
		cache->blf_next = dbb->dbb_blob_filters;
		dbb->dbb_blob_filters = cache;
	}

	return cache;
}


static BLP get_next_page(TDBB tdbb, BLB blob, WIN * window)
{
/**************************************
 *
 *      g e t _ n e x t _ p a g e
 *
 **************************************
 *
 * Functional description
 *      Read a blob page and copy it into the blob data area.  Return
 *      TRUE is there is a next page.
 *
 **************************************/
	VCL vector;
	BLP page;
	DBB dbb;
#ifdef SUPERSERVER_V2
	USHORT sequence, i;
	ULONG abs_sequence;
	SLONG pages[PREFETCH_MAX_PAGES];
#endif

	if (blob->blb_level == 0 || blob->blb_sequence > blob->blb_max_sequence) {
		blob->blb_space_remaining = 0;
		return NULL;
	}

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	vector = blob->blb_pages;

/* Level 1 blobs are much easier -- page number is in vector. */

	if (blob->blb_level == 1) {
#ifdef SUPERSERVER_V2
		/* Perform prefetch of blob level 1 data pages. */

		if (!(blob->blb_sequence % dbb->dbb_prefetch_sequence)) {
			sequence = blob->blb_sequence;
			for (i = 0;
				 i < dbb->dbb_prefetch_pages
				 && sequence <= blob->blb_max_sequence;) pages[i++] =
					(*vector)[sequence++];

			CCH_PREFETCH(tdbb, pages, i);
		}
#endif
		window->win_page = (*vector)[blob->blb_sequence];
		page = (BLP) CCH_FETCH(tdbb, window, LCK_read, pag_blob);
	}
	else {
		window->win_page =
			(*vector)[blob->blb_sequence / blob->blb_pointers];
		page = (BLP) CCH_FETCH(tdbb, window, LCK_read, pag_blob);
#ifdef SUPERSERVER_V2
		/* Perform prefetch of blob level 2 data pages. */

		sequence = blob->blb_sequence % blob->blb_pointers;
		if (!(sequence % dbb->dbb_prefetch_sequence))
		{
			abs_sequence = blob->blb_sequence;
			for (i = 0; i < dbb->dbb_prefetch_pages &&
						sequence < blob->blb_pointers &&
						abs_sequence <= blob->blb_max_sequence;)
			{
				pages[i++] = page->blp_page[sequence++];
				abs_sequence++;
			}

			CCH_PREFETCH(tdbb, pages, i);
		}
#endif
		page = (BLP)CCH_HANDOFF(tdbb,
								window,
								 page->blp_page[blob->blb_sequence %
												blob->blb_pointers],
								LCK_read,
								 pag_blob);
	}

	if (page->blp_sequence != (SLONG) blob->blb_sequence)
		CORRUPT(201);			/* msg 201 cannot find blob page */

	blob->blb_sequence++;

	return page;
}


#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
static void get_replay_blob(TDBB tdbb, BID blob_id)
{
/**************************************
 *
 *      g e t _ r e p l a y _ b l o b
 *
 **************************************
 *
 * Functional description
 *      Replace the blob id passed with the
 *      blob id used in the original session.
 *
 **************************************/
	DBB dbb;
	MAP map_ptr;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* we're only interested in newly created blobs */

	if (blob_id->bid_relation_id != 0)
		return;

/* search the linked list for the old blob id */

	for (map_ptr = dbb->dbb_blob_map; map_ptr; map_ptr = map_ptr->map_next) {
		if (blob_id->bid_stuff.bid_blob == map_ptr->map_old_blob)
		{
			blob_id->bid_stuff.bid_blob = map_ptr->map_new_blob;
			break;
		}
	}
}
#endif


static void insert_page(TDBB tdbb, BLB blob)
{
/**************************************
 *
 *      i n s e r t _ p a g e
 *
 **************************************
 *
 * Functional description
 *      A data page has been formatted.  Allocate a physical page,
 *      move the data page to the buffer, and insert the page number
 *      of the new page into the blob data structure.
 *
 **************************************/
	DBB dbb;
	USHORT length, l;
	BLP page;
	WIN window;
	ULONG page_number;
	VCL vector;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	length = dbb->dbb_page_size - blob->blb_space_remaining;
	vector = blob->blb_pages;
	blob->blb_max_sequence = blob->blb_sequence;

/* Allocate a page for the now full blob data page.  Move the page
   image to the buffer, and release the page.  */

	page = (BLP) DPM_allocate(tdbb, &window);
	page_number = window.win_page;

	if (blob->blb_sequence == 0)
		blob->blb_lead_page = page_number;

	MOVE_FASTER(blob->blb_data, page, length);
	page->blp_sequence = blob->blb_sequence;
	page->blp_lead_page = blob->blb_lead_page;
	page->blp_length = length - BLP_SIZE;
	CCH_RELEASE(tdbb, &window);

/* If the blob is at level 1, there are two cases.  First, and easiest,
   is that there is still room in the page vector to hold the pointer.
   The second case is that the vector is full, and the blob must be
   transformed into a level 2 blob. */

	if (blob->blb_level == 1) {
		/*  See if there is room in the page vector.  If so, just update
		   the vector. */

		if (blob->blb_sequence < blob->blb_max_pages) {
			if (blob->blb_sequence >= vector->count()) {
				vector->resize(blob->blb_sequence + 1);
			}
			(*vector)[blob->blb_sequence] = page_number;
			return;
		}

		/* The vector just overflowed.  Sigh.  Transform blob to level 2. */

		blob->blb_level = 2;
		page = (BLP) DPM_allocate(tdbb, &window);
		page->blp_header.pag_flags = blp_pointers;
		page->blp_header.pag_type = pag_blob;
		page->blp_lead_page = blob->blb_lead_page;
		page->blp_length = vector->count() << SHIFTLONG;
		MOVE_FASTER(vector->memPtr(), page->blp_page, page->blp_length);
		vector->resize(1);
		(*vector)[0] = window.win_page;
		CCH_RELEASE(tdbb, &window);
	}

/* The blob must be level 2.  Find the appropriate pointer page (creating
   it if need be, and stick the pointer in the appropriate slot. */

	l = blob->blb_sequence / blob->blb_pointers;

	if (l < vector->count()) {
		window.win_page = (*vector)[l];
		window.win_flags = 0;
		page = (BLP) CCH_FETCH(tdbb, &window, LCK_write, pag_blob);
	}
	else {
		page = (BLP) DPM_allocate(tdbb, &window);
		page->blp_header.pag_flags = blp_pointers;
		page->blp_header.pag_type = pag_blob;
		page->blp_lead_page = blob->blb_lead_page;
		vector->resize(l + 1);
		(*vector)[l] = window.win_page;
	}

	CCH_precedence(tdbb, &window, page_number);
	CCH_MARK(tdbb, &window);
	l = blob->blb_sequence % blob->blb_pointers;
	page->blp_page[l] = page_number;
	page->blp_length = (l + 1) << SHIFTLONG;
	CCH_RELEASE(tdbb, &window);
}


static void release_blob(BLB blob, USHORT purge_flag)
{
/**************************************
 *
 *      r e l e a s e _ b l o b
 *
 **************************************
 *
 * Functional description
 *      Release a blob and associated blocks.  Among other things,
 *      disconnect it from the transaction.  However, if purge_flag
 *      is FALSE, then only release the associated blocks.
 *
 **************************************/
	JRD_TRA transaction;
	BLB *ptr;

	transaction = blob->blb_transaction;

/* Disconnect blob from transaction block. */

	if (purge_flag)
		for (ptr = &transaction->tra_blobs; *ptr; ptr = &(*ptr)->blb_next)
			if (*ptr == blob) {
				*ptr = blob->blb_next;
				break;
			}

	if (blob->blb_pages) {
		delete blob->blb_pages;
		blob->blb_pages = NULL;
	}

	if (purge_flag)
		delete blob;
}


static void slice_callback(SLICE arg, ULONG count, DSC * descriptors)
{
/**************************************
 *
 *      s l i c e _ c a l l b a c k
 *
 **************************************
 *
 * Functional description
 *      Perform slice assignment.
 *
 **************************************/
	DSC *array_desc, *slice_desc, temp_desc;
	BLOB_PTR *next;
	BLOB_PTR *end;
	SLONG l;
	USHORT len;

	array_desc = descriptors;
	slice_desc = &arg->slice_desc;
	next = (BLOB_PTR *) slice_desc->dsc_address + arg->slice_element_length;

	if ((BLOB_PTR *) next > (BLOB_PTR *) arg->slice_end)
		ERR_post(gds_out_of_bounds, 0);

	if ((BLOB_PTR *) array_desc->dsc_address < (BLOB_PTR *) arg->slice_base)
		ERR_error(198);			/* msg 198 array subscript computation error */

	if (arg->slice_direction) {

		/* Storing INTO array */
		/* FROM slice_desc TO array_desc */

		/* If storing beyond the high-water mark, ensure elements
		 * from the high-water mark to current position are zeroed
		 */
		l =
			(BLOB_PTR *) array_desc->dsc_address -
			(BLOB_PTR *) arg->slice_high_water;
		if (l > 0)
			memset(arg->slice_high_water, 0, l);

		/* The individual elements of a varying string array may not be aligned
		   correctly.  If they aren't, some RISC machines may break.  In those
		   cases, calculate the actual length and then move the length and
		   text manually. */

		if (array_desc->dsc_dtype == dtype_varying &&
			(U_IPTR) array_desc->dsc_address !=
			FB_ALIGN((U_IPTR) array_desc->dsc_address,
					 (MIN(sizeof(USHORT), ALIGNMENT))))
		{
			STR tmp_buffer;
			USHORT tmp_len;
			TDBB tdbb;
			const char* p;

			/* Note: cannot remove this GET_THREAD_DATA without api change
			   to slice callback routines */
			tdbb = GET_THREAD_DATA;

			tmp_len = array_desc->dsc_length;
			tmp_buffer = FB_NEW_RPT(*tdbb->tdbb_default, tmp_len) str();
			len = MOV_make_string(slice_desc,
								  INTL_TEXT_TYPE(*array_desc),
								  &p,
								  reinterpret_cast<vary*>(tmp_buffer->str_data),
								  tmp_len);
			MOVE_FAST(&len, array_desc->dsc_address, sizeof(USHORT));
			MOVE_FAST(p, array_desc->dsc_address + sizeof(USHORT), (int) len);
			delete tmp_buffer;
		}
		else
		{
			MOV_move(slice_desc, array_desc);
		}
		end = (BLOB_PTR *) array_desc->dsc_address + array_desc->dsc_length;
		if ((BLOB_PTR *) end > (BLOB_PTR *) arg->slice_high_water)
			arg->slice_high_water = (BLOB_PTR *) end;
	}
	else {

		/* Fetching FROM array */
		/* FROM array_desc TO slice_desc */

		/* If the element is under the high-water mark, fetch it,
		 * otherwise just zero it
		 */
		if ((BLOB_PTR *) array_desc->dsc_address <
			(BLOB_PTR *) arg->slice_high_water) {
			/* If a varying string isn't aligned correctly, calculate the actual
			   length and then treat the string as if it had type text. */

			if (array_desc->dsc_dtype == dtype_varying &&
				(U_IPTR) array_desc->dsc_address !=
				FB_ALIGN((U_IPTR) array_desc->dsc_address,
						 (MIN(sizeof(USHORT), ALIGNMENT)))) {
				temp_desc.dsc_dtype = dtype_text;
				temp_desc.dsc_sub_type = array_desc->dsc_sub_type;
				temp_desc.dsc_scale = array_desc->dsc_scale;
				temp_desc.dsc_flags = array_desc->dsc_flags;
				MOVE_FAST(array_desc->dsc_address, &temp_desc.dsc_length,
						  sizeof(USHORT));
				temp_desc.dsc_address =
					array_desc->dsc_address + sizeof(USHORT);
				MOV_move(&temp_desc, slice_desc);
			}
			else
				MOV_move(array_desc, slice_desc);
			++arg->slice_count;
		}
		else if ( (l = slice_desc->dsc_length) )
			memset(slice_desc->dsc_address, 0, l);
	}

	slice_desc->dsc_address = next;
}


static BLB store_array(TDBB tdbb, JRD_TRA transaction, BID blob_id)
{
/**************************************
 *
 *      s t o r e _ a r r a y
 *
 **************************************
 *
 * Functional description
 *      Actually store an array.  Oh boy!
 *
 **************************************/
	ARR array;
	BLB blob;
	BLOB_PTR *p;
	SLONG length;

	SET_TDBB(tdbb);

/* Validate array */

	if (!(array = find_array(transaction, blob_id)))
		return NULL;

/* Create blob for array */

	blob = BLB_create2(tdbb, transaction, blob_id, 0, (UCHAR *) 0);
	blob->blb_flags |= BLB_stream;

/* Write out array descriptor */

	BLB_put_segment(tdbb, blob,
					reinterpret_cast < UCHAR * >(&array->arr_desc),
					array->arr_desc.ads_length);

/* Write out actual array */

	for (length = array->arr_effective_length, p =
		 (BLOB_PTR *) array->arr_data; length > 32768;
		 length -= 32768, p +=
		 32768) BLB_put_segment(tdbb, blob, p, (USHORT) 32768);

	if (length)
		BLB_put_segment(tdbb, blob, p, (USHORT) length);

	BLB_close(tdbb, blob);

	return blob;
}

