/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		blb.cpp
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
$Id: blb.cpp,v 1.94 2005-06-12 06:27:11 dimitr Exp $
*/

#include "firebird.h"
#include <string.h>
#include "../jrd/common.h"
#include "../jrd/ibase.h"

#include "../jrd/jrd.h"
#include "../jrd/tra.h"
#include "../jrd/val.h"
#include "../jrd/exe.h"
#include "../jrd/req.h"
#include "../jrd/blb.h"
#include "../jrd/ods.h"
#include "../jrd/lls.h"
#include "gen/iberror.h"
#include "../jrd/blob_filter.h"
#include "../jrd/sdl.h"
#include "../jrd/intl.h"
#include "../jrd/cch.h"
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
#include "../jrd/thd.h"
#include "../jrd/dsc_proto.h"

using namespace Jrd;
typedef Ods::blob_page blob_page;

inline bool SEGMENTED(const blb* blob)
{
	return !(blob->blb_flags & BLB_stream);
}

static ArrayField* alloc_array(jrd_tra*, internal_array_desc*);
static blb* allocate_blob(thread_db*, jrd_tra*);
static ISC_STATUS blob_filter(USHORT, BlobControl*, SSHORT, SLONG);
static blb* copy_blob(thread_db*, const bid*, bid*, USHORT, const UCHAR*);
static void delete_blob(thread_db*, blb*, ULONG);
static void delete_blob_id(thread_db*, const bid*, SLONG, jrd_rel*);
static ArrayField* find_array(jrd_tra*, const bid*);
static BlobFilter* find_filter(thread_db*, SSHORT, SSHORT);
static blob_page* get_next_page(thread_db*, blb*, WIN *);
#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
static void get_replay_blob(thread_db*, const bid*);
#endif
static void insert_page(thread_db*, blb*);
static void release_blob(blb*, const bool);
static void slice_callback(array_slice*, ULONG, dsc*);
static blb* store_array(thread_db*, jrd_tra*, bid*);


void BLB_cancel(thread_db* tdbb, blb* blob)
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

	release_blob(blob, true);
}


void BLB_close(thread_db* tdbb, class blb* blob)
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
		release_blob(blob, true);
		return;
	}

	if (blob->blb_level >= 1
		&& blob->blb_space_remaining < blob->blb_clump_size)
	{
		insert_page(tdbb, blob);
	}
}


blb* BLB_create(thread_db* tdbb, jrd_tra* transaction, bid* blob_id)
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
	return BLB_create2(tdbb, transaction, blob_id, 0, NULL);
}


blb* BLB_create2(thread_db* tdbb,
				jrd_tra* transaction, bid* blob_id,
				USHORT bpb_length, const UCHAR* bpb)
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
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	// FIXME! Temporary BLOBs are not supported in read only databases
	if (dbb->dbb_flags & DBB_read_only)
		ERR_post(isc_read_only_database, 0);

/* Create a blob large enough to hold a single data page */
	SSHORT from, to;
	SSHORT from_charset, to_charset;
	const SSHORT type = gds__parse_bpb2(bpb_length,
						   bpb,
						   &from,
						   &to,
						   reinterpret_cast<USHORT*>(&from_charset),
						   reinterpret_cast<USHORT*>(&to_charset));
	blb* blob = allocate_blob(tdbb, transaction);

	if (type)
		blob->blb_flags |= BLB_stream;

	blob->blb_source_interp = from_charset;
	blob->blb_target_interp = to_charset;
	blob->blb_sub_type = to;

	bool filter_required = false;
	BlobFilter* filter = NULL;
	if (to && from != to) {
		filter = find_filter(tdbb, from, to);
		filter_required = true;
	}
	else if (to == isc_blob_text && (from_charset != to_charset)) {
		if (from_charset == CS_dynamic)
			from_charset = tdbb->tdbb_attachment->att_charset;
		if (to_charset == CS_dynamic)
			to_charset = tdbb->tdbb_attachment->att_charset;
		if ((to_charset != CS_NONE) && (from_charset != to_charset)) {
			filter = FB_NEW(*dbb->dbb_permanent) BlobFilter(*dbb->dbb_permanent);
			filter->blf_filter = filter_transliterate_text;
			filter_required = true;
		}
	}

	if (filter_required) {
		if (BLF_create_blob(tdbb,
							transaction,
							&blob->blb_filter,
							blob_id,
							bpb_length,
							bpb,
							// CVC: This cast is very suspicious to me.
							// We have to research if seek really gets params
							// from the control struct instead. Maybe y-valve has a special case.
							// Otherwise, blob_filter's sig would be like any filter.
							reinterpret_cast<FPTR_BFILTER_CALLBACK>(blob_filter),
							filter))
		{
			ERR_punt();
		}
		blob->blb_flags |= BLB_temporary;
		return blob;
	}

	blob->blb_space_remaining = blob->blb_clump_size;
	blob->blb_flags |= BLB_temporary;

/* Set up for a "small" blob -- a blob that fits on an ordinary data page */

	blob_page* page = (blob_page*) blob->blb_data;
	page->blp_header.pag_type = pag_blob;
	blob->blb_segment = (UCHAR *) page->blp_page;

/* Format blob id and return blob handle */

	blob_id->set_temporary(blob->blb_temp_id);

	return blob;
}


//
// This function makes linear stacks lookup. Therefore
// in case of big stacks garbage collection speed may become
// real problem. Stacks should be sorted before run?
//
void BLB_garbage_collect(
						 thread_db* tdbb,
						 RecordStack& going,
						 RecordStack& staying,
						 SLONG prior_page, jrd_rel* relation)
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
	DSC desc1, desc2;

	SET_TDBB(tdbb);

/* Loop thru records on the way out looking for blobs to garbage collect */

	for (RecordStack::iterator stack1(going); stack1.hasData(); ++stack1) {
		Record* rec1 = stack1.object();
		if (!rec1)
			continue;
		const Format* format = rec1->rec_format;

		/* Look for active blob records */

		for (USHORT id = 0; id < format->fmt_count; id++) {
			if (!DTYPE_IS_BLOB(format->fmt_desc[id].dsc_dtype)
				|| !EVL_field(0, rec1, id, &desc1))
			{
				continue;
			}
			const bid* blob = (bid*) desc1.dsc_address;

			/* Got active blob, cancel it out of any remaining records on the way out */

			for (RecordStack::iterator stack2(stack1); stack2.hasData(); ++stack2) {
				Record* rec2 = stack2.object();
				if (!EVL_field(0, rec2, id, &desc2))
					continue;
				const bid* blob2 = (bid*) desc2.dsc_address;
				if (*blob == *blob2)
				{
					SET_NULL(rec2, id);
				}
			}

			/* Make sure the blob doesn't stack in any record remaining */

			RecordStack::iterator stack3(staying);
			for (; stack3.hasData(); ++stack3) {
				Record* rec3 = stack3.object();
				if (!EVL_field(0, rec3, id, &desc2))
					continue;
				const bid* blob3 = (bid*) desc2.dsc_address;
				if (*blob == *blob3)
				{
					break;
				}
			}
			if (stack3.hasData())
				continue;

			/* Get rid of blob */

			delete_blob_id(tdbb, blob, prior_page, relation);
		}
	}
}


blb* BLB_get_array(thread_db* tdbb, jrd_tra* transaction, const bid* blob_id,
				   internal_array_desc* desc)
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
	SET_TDBB(tdbb);

	blb* blob = BLB_open2(tdbb, transaction, blob_id, 0, 0);

	if (blob->blb_length < sizeof(internal_array_desc)) {
		BLB_close(tdbb, blob);
		IBERROR(193);			/* msg 193 null or invalid array */
	}

	BLB_get_segment(tdbb, blob, reinterpret_cast<UCHAR*>(desc), sizeof(internal_array_desc));

	const USHORT n = desc->iad_length - sizeof(internal_array_desc);
	if (n) {
		BLB_get_segment(tdbb, blob, reinterpret_cast<UCHAR*>(desc) + sizeof(internal_array_desc), n);
	}

	return blob;
}


SLONG BLB_get_data(thread_db* tdbb, blb* blob, UCHAR* buffer, SLONG length, bool close)
{
/**************************************
 *
 *      B L B _ g e t _ d a t a
 *
 **************************************
 *
 * Functional description
 *      Get a large hunk of data from a blob, which can then be
 *      closed (if close == true).  Return total number of bytes read.
 *      Don't worry about segment boundaries.
 *
 **************************************/
	SET_TDBB(tdbb);
	BLOB_PTR* p = buffer;

	while (length > 0) {
		/* I have no idea why this limit is 32768 instead of 32767
		 * 1994-August-12 David Schnepper
		 */
		USHORT n = (USHORT) MIN(length, (SLONG) 32768);
		n = BLB_get_segment(tdbb, blob, p, n);
		p += n;
		length -= n;
		if (blob->blb_flags & BLB_eof)
			break;
	}

	if (close)
		BLB_close(tdbb, blob);

	return (SLONG) (p - buffer);
}


USHORT BLB_get_segment(thread_db* tdbb,
					   blb* blob, UCHAR* segment, USHORT buffer_length)
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
	ISC_STATUS status;
	USHORT l;

	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;

#ifdef SUPERSERVER

	if (--tdbb->tdbb_quantum < 0)
		JRD_reschedule(tdbb, 0, true);

#endif

/* If we reached end of file, we're still there */

	if (blob->blb_flags & BLB_eof)
		return 0;

	if (blob->blb_filter) {
		blob->blb_fragment_size = 0;
		USHORT tmp_len = 0;
		if ( (status =
			BLF_get_segment(tdbb, &blob->blb_filter, &tmp_len, buffer_length,
							segment)) )
		{
			if (status == isc_segstr_eof)
				blob->blb_flags |= BLB_eof;
			else if (status == isc_segment)
				blob->blb_fragment_size = 1;
			else
				ERR_punt();
		}

		return tmp_len;
	}

/* If there is a seek pending, handle it here */

	USHORT seek = 0;

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

	BLOB_PTR* to = segment;
	const BLOB_PTR* from = blob->blb_segment;
	USHORT length = blob->blb_space_remaining;
	bool active_page = false;
	WIN window(-1); // there was no initialization of win_page here.
	if (blob->blb_flags & BLB_large_scan) {
		window.win_flags = WIN_large_scan;
		window.win_scans = 1;
	}

	while (true) {

		/* If the blob is segmented, and this isn't a fragment, pick up
		   the length of the next segment. */

		if (SEGMENTED(blob) && !blob->blb_fragment_size) {
			while (length < 2) {
				if (active_page) {
					if (window.win_flags & WIN_large_scan)
						CCH_RELEASE_TAIL(tdbb, &window);
					else
						CCH_RELEASE(tdbb, &window);
				}
				const blob_page* page = get_next_page(tdbb, blob, &window);
				if (!page) {
					blob->blb_flags |= BLB_eof;
					return 0;
				}
				from = (const UCHAR*) page->blp_page;
				length = page->blp_length;
				active_page = true;
			}

			UCHAR* p = (UCHAR *) & blob->blb_fragment_size;
			*p++ = *from++;
			*p++ = *from++;
			length -= 2;
		}

		/* Figure out how much data can be moved.  Then account for the
		   space, and move the data */

		l = MIN(buffer_length, length);

		if (SEGMENTED(blob)) {
			l = MIN(l, blob->blb_fragment_size);
			blob->blb_fragment_size -= l;
		}

		length -= l;
		buffer_length -= l;
		if (((U_IPTR) from & (ALIGNMENT - 1))
			|| ((U_IPTR) to & (ALIGNMENT - 1)))
		{
			MOVE_FAST(from, to, l);
		}
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
			const blob_page* page = get_next_page(tdbb, blob, &window);
			if (!page) {
				active_page = false;
				break;
			}
			from = reinterpret_cast<const UCHAR*>(page->blp_page) + seek;
			length = page->blp_length - seek;
			seek = 0;
			active_page = true;
		}

		/* If either the buffer or the fragment is exhausted, we're
		   done. */

		if (!buffer_length || (SEGMENTED(blob) && !blob->blb_fragment_size))
			break;
	}

	if (active_page) {
		if (((U_IPTR) from & (ALIGNMENT - 1))
			|| ((U_IPTR) blob->blb_data & (ALIGNMENT - 1)))
		{
			MOVE_FAST(from, blob->blb_data, length);
		}
		else
			MOVE_FASTER(from, blob->blb_data, length);
		from = blob->blb_data;
		if (window.win_flags & WIN_large_scan)
			CCH_RELEASE_TAIL(tdbb, &window);
		else
			CCH_RELEASE(tdbb, &window);
	}

	blob->blb_segment = const_cast<BLOB_PTR*>(from); // safe cast
	blob->blb_space_remaining = length;
	length = to - segment;
	blob->blb_seek += length;

/* If this is a stream blob, fake fragment unless we're at the end */

	if (!SEGMENTED(blob)) { // stream blob
		blob->blb_fragment_size =
			(blob->blb_seek == blob->blb_length) ? 0 : 1;
	}

	return length;
}


SLONG BLB_get_slice(thread_db* tdbb,
					jrd_tra* transaction,
					const bid* blob_id,
					const UCHAR* sdl,
					USHORT param_length,
					const SLONG* param, SLONG slice_length, UCHAR* slice_addr)
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
	ISC_STATUS status;

	SET_TDBB(tdbb);
    Database* database = GET_DBB();
	Jrd::ContextPoolHolder context(tdbb, transaction->tra_pool);

/* Checkout slice description language */
	SLONG variables[64];
	sdl_info info;
	MOVE_FAST(param, variables, MIN(sizeof(variables), param_length));

	if (SDL_info(tdbb->tdbb_status_vector, sdl, &info, variables)) {
		ERR_punt();
	}

	SLONG stuff[IAD_LEN(16) / 4];
	internal_array_desc* desc = (internal_array_desc*) stuff;
	blb* blob = BLB_get_array(tdbb, transaction, blob_id, desc);
	SLONG length = desc->iad_total_length;

/* Get someplace to put data */

	UCHAR* data = (UCHAR*) database->dbb_permanent->allocate(desc->iad_total_length, 0
#ifdef DEBUG_GDS_ALLOC
	  ,__FILE__, __LINE__
#endif
	);

/* zero out memory, so that it does not have to be done for
   each element */

	memset(data, 0, desc->iad_total_length);

	SLONG offset = 0;

	array_slice arg;

/* Trap any potential errors */
	try {

/* If we know something about the subscript bounds, prepare
   to fetch only stuff we really care about */

	if (info.sdl_info_dimensions) {
		const SLONG from =
			SDL_compute_subscript(tdbb->tdbb_status_vector, desc,
								  info.sdl_info_dimensions,
								  info.sdl_info_lower);
		const SLONG to =
			SDL_compute_subscript(tdbb->tdbb_status_vector, desc,
								  info.sdl_info_dimensions,
								  info.sdl_info_upper);
		if (from != -1 && to != -1) {
			if (from) {
				offset = from * desc->iad_element_length;
				BLB_lseek(blob, 0, offset + (SLONG) desc->iad_length);
			}
			length = (to - from + 1) * desc->iad_element_length;
		}
	}

	length = BLB_get_data(tdbb, blob, data + offset, length) + offset;

/* Walk array */
	arg.slice_desc = info.sdl_info_element;
	arg.slice_desc.dsc_address = slice_addr;
	arg.slice_end = slice_addr + slice_length;
	arg.slice_count = 0;
	arg.slice_element_length = info.sdl_info_element.dsc_length;
	arg.slice_direction = FALSE;	/* fetching from array */
	arg.slice_high_water = data + length;
	arg.slice_base = data + offset;

	status = SDL_walk(tdbb->tdbb_status_vector,
					  sdl,
					  true,
					  data,
					  desc,
					  variables,
					  slice_callback,
					  &arg);

	database->dbb_permanent->deallocate(data);

	if (status) {
		ERR_punt();
	}

	}	// try
	catch (const std::exception&) {
		database->dbb_permanent->deallocate(data);
		throw;
	}

	return (SLONG) (arg.slice_count * arg.slice_element_length);
}


SLONG BLB_lseek(blb* blob, USHORT mode, SLONG offset)
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
		ERR_post(isc_bad_segstr_type, 0);

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

void BLB_map_blobs(thread_db* tdbb, blb* old_blob, blb* new_blob)
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
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	blb_map* new_map = FB_NEW(*dbb->dbb_permanent) blb_map();
	new_map->map_old_blob = old_blob;
	new_map->map_new_blob = new_blob;

	new_map->map_next = dbb->dbb_blob_map;
	dbb->dbb_blob_map = new_map;
}

#endif	// REPLAY_OSRI_API_CALLS_SUBSYSTEM


// This function can't take from_desc as const because it may call store_array,
// which in turn calls BLB_create2 that writes in the blob id. Although the
// compiler allows to modify from_desc->dsc_address' contents when from_desc is
// constant, this is misleading so I didn't make the source descriptor constant.
void BLB_move(thread_db* tdbb, dsc* from_desc, dsc* to_desc, jrd_nod* field)
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
	SET_TDBB(tdbb);

	if (field->nod_type != nod_field)
		BUGCHECK(199);			/* msg 199 expected field node */

	if (from_desc->dsc_dtype != dtype_quad
		&& from_desc->dsc_dtype != dtype_blob)
	{
		ERR_post(isc_convert_error, isc_arg_string, "BLOB", 0);
	}

	jrd_req* request = tdbb->tdbb_request;
	bid* source = (bid*) from_desc->dsc_address;
	bid* destination = (bid*) to_desc->dsc_address;
	const USHORT id = (USHORT) (IPTR) field->nod_arg[e_fld_id];
	record_param* rpb = &request->req_rpb[(IPTR)field->nod_arg[e_fld_stream]];
	jrd_rel* relation = rpb->rpb_relation;
	Record* record = rpb->rpb_record;

/* If nothing changed, do nothing.  If it isn't broken,
   don't fix it. */

	if (*source == *destination)
	{
		return;
	}

/* If either the source value is null or the blob id itself is null (all
   zeros, then the blob is null. */

	if ((request->req_flags & req_null) || source->isEmpty())
	{
		SET_NULL(record, id);
		destination->clear();
		return;
	}

	CLEAR_NULL(record, id);
	jrd_tra* transaction = request->req_transaction;

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

	ArrayField* array = NULL;
	BlobIndex* blobIndex;
	blb* blob = NULL;
	bool materialized_blob; // Set if we materialized temporary blob in this routine

	do {
		materialized_blob = false;
		blobIndex = NULL;
		if (source->bid_internal.bid_relation_id)
		{
			UCHAR bpb[] = {isc_bpb_version1,
						   isc_bpb_source_type, 1, isc_blob_text, isc_bpb_source_interp, 1, 0,
						   isc_bpb_target_type, 1, isc_blob_text, isc_bpb_target_interp, 1, 0};
			USHORT bpb_length = 0;

			if (from_desc->dsc_sub_type == isc_blob_text && to_desc->dsc_sub_type == isc_blob_text)
			{
				bpb[6] = from_desc->dsc_scale;	// source charset
				bpb[12] = to_desc->dsc_scale;	// destination charset
				bpb_length = sizeof(bpb);
			}

			blob = copy_blob(tdbb, source, destination, bpb_length, bpb);
		}
		else if ((to_desc->dsc_dtype == dtype_array) &&
				 (array = find_array(transaction, source)) &&
				 (blob = store_array(tdbb, transaction, source)))
		{
			materialized_blob = true;
		}
		else {
			if (transaction->tra_blobs.locate(source->bid_temp_id())) {
				blobIndex = &transaction->tra_blobs.current();
				if (blobIndex->bli_materialized) {
					if (blobIndex->bli_request) {
						// Walk through call stack looking if our BLOB is
						// owned by somebody from our call chain
						jrd_req* temp_req = request;
						do {
							if (blobIndex->bli_request == temp_req)
								break;
							temp_req = temp_req->req_caller;
						} while (temp_req);
						if (!temp_req) {
							// Trying to use temporary id of materialized blob from another request
							ERR_post(isc_bad_segstr_id, 0);
						}
					}
					source = &blobIndex->bli_blob_id;
					continue;
				}
				else {
					materialized_blob = true;
					blob = blobIndex->bli_blob_object;
				}
			}
		}

		if (!blob || !(blob->blb_flags & BLB_closed))
		{
			ERR_post(isc_bad_segstr_id, 0);
		}

		break;
	} while (true);

	blob->blb_relation = relation;
	destination->set_permanent(relation->rel_id, DPM_store_blob(tdbb, blob, record));
	// This is the only place in the engine where blobs are materialized
	// If new places appear code below should transform to common sub-routine
	if (materialized_blob) {
		if (!blobIndex) {
			// In case of normal blobs tra_blobs is already positioned on item we need
			if (transaction->tra_blobs.locate(blob->blb_temp_id)) {
				blobIndex = &transaction->tra_blobs.current();
			}
		}

		// If we didn't find materialized blob in transaction blob index it
		// means memory structures are inconsistent and crash is appropriate
		blobIndex->bli_materialized = true;
		blobIndex->bli_blob_id = *destination;
		// Assign temporary BLOB ownership to top-level request if it is not assigned yet
		jrd_req* own_request;
		if (blobIndex->bli_request) {
			own_request = blobIndex->bli_request;
		} else {
			own_request = request;
			while (own_request->req_caller)
				own_request = own_request->req_caller;
			blobIndex->bli_request = own_request;
			own_request->req_blobs.add(blob->blb_temp_id);
		}
		// Not sure that this ownership is entirely correct for arrays, but
		// even if I make mistake here widening array lifetime this should not hurt much
		if (array)
			array->arr_request = own_request;
	}
	release_blob(blob, !materialized_blob);
}


void BLB_move_from_string(thread_db* tdbb, const dsc* from_desc, dsc* to_desc, jrd_nod* field)
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
	    ERR_post(isc_convert_error, isc_arg_string,
		DSC_dtype_tostring(from_desc->dsc_dtype), 0);
	else
	{
		USHORT ttype = 0;
		blb* blob = 0;
		UCHAR *fromstr = 0;
		bid temp_bid;
		DSC blob_desc;
		temp_bid.clear();
		MOVE_CLEAR(&blob_desc, sizeof(blob_desc));

		int length = MOV_get_string_ptr(from_desc, &ttype, &fromstr, 0, 0);

		UCHAR bpb[] = {isc_bpb_version1,
					   isc_bpb_source_type, 1, isc_blob_text, isc_bpb_source_interp, 1, 0,
					   isc_bpb_target_type, 1, isc_blob_text, isc_bpb_target_interp, 1, 0};
		USHORT bpb_length = 0;

		if (to_desc->dsc_sub_type == isc_blob_text)
		{
			bpb[6] = ttype;					// from charset
			bpb[12] = to_desc->dsc_scale;	// to charset
			bpb_length = sizeof(bpb);
		}

		blob = BLB_create2(tdbb, tdbb->tdbb_request->req_transaction, &temp_bid, bpb_length, bpb);

		blob_desc.dsc_scale = to_desc->dsc_scale;	// blob charset
		blob_desc.dsc_flags = (blob_desc.dsc_flags & 0xFF) | (to_desc->dsc_flags & 0xFF00);	// blob collation
		blob_desc.dsc_sub_type = to_desc->dsc_sub_type;
		blob_desc.dsc_dtype = dtype_blob;
		blob_desc.dsc_address = reinterpret_cast<UCHAR*>(&temp_bid);
		BLB_put_segment(tdbb, blob, fromstr, length);
		BLB_close(tdbb, blob);
		ULONG blob_temp_id = blob->blb_temp_id;
		BLB_move(tdbb, &blob_desc, to_desc, field);
		// 14-June-2004. Nickolay Samofatov
		// The code below saves a lot of memory when bunches of records are
		// converted to blobs from strings. If BLB_move is materialized blob we
		// can discard it without consequences since we know there are no other
		// descriptors using temporary ID of blob we just created. If blob is
		// still temporary we cannot free it as it may now be used inside
		// trigger for updatable view. In theory we could make this decision
		// solely via checking that destination field belongs to updatable
		// view, but direct check that blob is fully materialized should be
		// more future proof.
		jrd_tra* transaction = tdbb->tdbb_request->req_transaction;
		if (transaction->tra_blobs.locate(blob_temp_id)) {
			BlobIndex* current = &transaction->tra_blobs.current();
			if (current->bli_materialized) {
				// Delete BLOB from request owned blob list
				jrd_req* blob_request = current->bli_request;
				if (blob_request) {
					if (blob_request->req_blobs.locate(blob_temp_id)) {
						blob_request->req_blobs.fastRemove();
					} else {
						// We should never get here because when bli_request is assigned
						// item should be added to req_blobs array
						fb_assert(false);
					}
				}

				// Free materialized blob handle
				transaction->tra_blobs.fastRemove();
			}
			else {
				// But even in bad case when we cannot free blob immediately
				// we may still bind lifetime of blob to current request.
				if (!current->bli_request) {
					current->bli_request = tdbb->tdbb_request;
					current->bli_request->req_blobs.add(blob_temp_id);
				}
			}
		}
	}
}


blb* BLB_open(thread_db* tdbb, jrd_tra* transaction, const bid* blob_id)
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


blb* BLB_open2(thread_db* tdbb,
			  jrd_tra* transaction, const bid* blob_id,
			  USHORT bpb_length, const UCHAR* bpb)
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
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;

/* Handle filter case */
	SSHORT from, to;
	SSHORT from_charset, to_charset;
	gds__parse_bpb2(bpb_length,
					bpb,
					&from,
					&to,
					reinterpret_cast<USHORT*>(&from_charset),
					reinterpret_cast<USHORT*>(&to_charset));

	blb* blob = allocate_blob(tdbb, transaction);

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
/* for REPLAY, map blob id's from the original session */

	get_replay_blob(tdbb, blob_id);
#endif

	blob->blb_target_interp = to_charset;
	blob->blb_source_interp = from_charset;

	BlobFilter* filter = NULL;
	bool filter_required = false;
	if (to && from != to) {
		filter = find_filter(tdbb, from, to);
		filter_required = true;
	}
	else if (to == isc_blob_text && (from_charset != to_charset)) {
		if (from_charset == CS_dynamic)
			from_charset = tdbb->tdbb_attachment->att_charset;
		if (to_charset == CS_dynamic)
			to_charset = tdbb->tdbb_attachment->att_charset;
		if ((to_charset != CS_NONE) && (from_charset != to_charset)) {
			filter = FB_NEW(*dbb->dbb_permanent) BlobFilter(*dbb->dbb_permanent);
			filter->blf_filter = filter_transliterate_text;
			filter_required = true;
		}
	}

	if (filter_required) {
		BlobControl* control = 0;
		if (BLF_open_blob(tdbb,
						  transaction,
						  &control,
						  blob_id,
						  bpb_length,
						  bpb,
						  reinterpret_cast<FPTR_BFILTER_CALLBACK>(blob_filter),
						  filter))
		{
			ERR_punt();
		}
		blob->blb_filter = control;
		blob->blb_max_segment = control->ctl_max_segment;
		blob->blb_count = control->ctl_number_segments;
		blob->blb_length = control->ctl_total_length;
		return blob;
	}

	if (!blob_id->bid_internal.bid_relation_id)
		if (blob_id->isEmpty())
		{
			blob->blb_flags |= BLB_eof;
			return blob;
		}
		else {
			/* Note: Prior to 1991, we would immediately report bad_segstr_id here,
			 * but then we decided to allow a newly created blob to be opened,
			 * leaving the possibility of receiving a garbage blob ID from
			 * the application.
			 * The following does some checks to try and product ourselves
			 * better.  94-Jan-07 Daves.
			 */

			/* Search the index of transaction blobs for a match */
			const blb* new_blob = NULL;
			if (transaction->tra_blobs.locate(blob_id->bid_temp_id())) {
				BlobIndex *current = &transaction->tra_blobs.current();
				if (!current->bli_materialized)
					new_blob = current->bli_blob_object;
			}

			if (!new_blob || !(new_blob->blb_flags & BLB_temporary)) {
				ERR_post(isc_bad_segstr_id, 0);
			}

			blob->blb_lead_page = new_blob->blb_lead_page;
			blob->blb_max_sequence = new_blob->blb_max_sequence;
			blob->blb_count = new_blob->blb_count;
			blob->blb_length = new_blob->blb_length;
			blob->blb_max_segment = new_blob->blb_max_segment;
			blob->blb_level = new_blob->blb_level;
			blob->blb_flags = new_blob->blb_flags & BLB_stream;
			const vcl* pages = new_blob->blb_pages;
			if (pages) {
				vcl* new_pages = vcl::newVector(*transaction->tra_pool, *pages);
				blob->blb_pages = new_pages;
			}
			if (blob->blb_level == 0) {
				blob->blb_space_remaining =
					new_blob->blb_clump_size - new_blob->blb_space_remaining;
				blob->blb_segment =
					(UCHAR *) ((blob_page*) new_blob->blb_data)->blp_page;
			}
			return blob;
		}

/* Ordinarily, we would call MET_relation to get the relation id.
   However, since the blob id must be consider suspect, this is
   not a good idea.  On the other hand, if we don't already
   know about the relation, the blob id has got to be invalid
   anyway. */

	vec* vector = dbb->dbb_relations;

	if (blob_id->bid_internal.bid_relation_id >= vector->count() ||
		!(blob->blb_relation =
		  static_cast<jrd_rel*>( (*vector)[blob_id->bid_internal.bid_relation_id]) ) )
	{
			ERR_post(isc_bad_segstr_id, 0);
	}

	DPM_get_blob(tdbb, blob, blob_id->get_permanent_number(), false, (SLONG) 0);

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


void BLB_put_segment(thread_db* tdbb, blb* blob, const UCHAR* seg, USHORT segment_length)
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
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	const BLOB_PTR* segment = seg;

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

	ULONG length;				// length of segment + overhead
	bool length_flag;
	if (SEGMENTED(blob)) {
		length = segment_length + 2;
		length_flag = true;
	}
	else {
		length = segment_length;
		length_flag = false;
	}

/* Case 0: Transition from small blob to medium size blob.  This really
   just does a form transformation and drops into the next case. */

	if (blob->blb_level == 0 && length > (ULONG) blob->blb_space_remaining) {
		jrd_tra* transaction = blob->blb_transaction;
		blob->blb_pages = vcl::newVector(*transaction->tra_pool, 0);
		const USHORT l = dbb->dbb_page_size - BLP_SIZE;
		blob->blb_space_remaining += l - blob->blb_clump_size;
		blob->blb_clump_size = l;
		blob->blb_level = 1;
	}

/* Case 1: The segment fits.  In what is immaterial.  Just move the segment
   and get out! */

	BLOB_PTR* p = blob->blb_segment;

	if (length_flag && blob->blb_space_remaining >= 2) {
		const BLOB_PTR* q = (UCHAR*) &segment_length;
		*p++ = *q++;
		*p++ = *q++;
		blob->blb_space_remaining -= 2;
		length_flag = false;
	}

	if (!length_flag && segment_length <= blob->blb_space_remaining) {
		blob->blb_space_remaining -= segment_length;
		if (((U_IPTR) segment & (ALIGNMENT - 1))
			|| ((U_IPTR) p & (ALIGNMENT - 1)))
		{
			MOVE_FAST(segment, p, segment_length);
		}
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

		const USHORT l = MIN(segment_length, blob->blb_space_remaining);

		if (!length_flag && l) {
			segment_length -= l;
			blob->blb_space_remaining -= l;
			if (((U_IPTR) segment & (ALIGNMENT - 1))
				|| ((U_IPTR) p & (ALIGNMENT - 1)))
			{
				MOVE_FAST(segment, p, l);
			}
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

		blob_page* page = (blob_page*) blob->blb_data;
		p = blob->blb_segment = (UCHAR *) page->blp_page;
		blob->blb_space_remaining = blob->blb_clump_size;

		/* If there's still a length waiting to be moved, move it already! */

		if (length_flag) {
			const BLOB_PTR* q = (UCHAR*) &segment_length;
			*p++ = *q++;
			*p++ = *q++;
			blob->blb_space_remaining -= 2;
			length_flag = false;
			blob->blb_segment = p;
		}
	}

}


void BLB_put_slice(	thread_db*	tdbb,
					jrd_tra*		transaction,
					bid*		blob_id,
					const UCHAR*	sdl,
					USHORT	param_length,
					const SLONG*	param,
					SLONG	slice_length,
					UCHAR*	slice_addr)
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
	SET_TDBB(tdbb);
	Jrd::ContextPoolHolder context(tdbb, transaction->tra_pool);

/* Do initial parse of slice description to get relation and field identification */
	sdl_info info;
	if (SDL_info(tdbb->tdbb_status_vector, sdl, &info, 0))
		ERR_punt();

	jrd_rel* relation;
	if (info.sdl_info_relation.length()) {
		relation = MET_lookup_relation(tdbb, info.sdl_info_relation);
	}
	else {
		relation = MET_relation(tdbb, info.sdl_info_rid);
	}

	if (!relation) {
		IBERROR(196);			/* msg 196 relation for array not known */
	}

	SSHORT	n;
	if (info.sdl_info_field.length()) {
	    n = MET_lookup_field(tdbb, relation, info.sdl_info_field, 0);
	}
	else {
		n = info.sdl_info_fid;
	}

/* Make sure relation is scanned */
	MET_scan_relation(tdbb, relation);

	jrd_fld* field;
	if (n < 0 || !(field = MET_get_field(relation, n))) {
		IBERROR(197);			/* msg 197 field for array not known */
	}

	ArrayField* array_desc = field->fld_array;
	if (!array_desc)
	{
		ERR_post(isc_invalid_dimension, isc_arg_number, (SLONG) 0,
				 isc_arg_number, (SLONG) 1, 0);
	}

/* Find and/or allocate array block.  There are three distinct cases:

	1.  Array is totally new.
	2.  Array is still in "temporary" state.
	3.  Array exists and is being updated.
*/
	ArrayField* array = 0;
	array_slice arg;
	if (blob_id->bid_internal.bid_relation_id)
	{
		for (array = transaction->tra_arrays; array; array = array->arr_next)
		{
			if (array->arr_blob &&
				array->arr_blob->blb_blob_id == *blob_id)
			{
				break;
			}
		}
		if (array)
		{
			arg.slice_high_water = array->arr_data + array->arr_effective_length;
		}
		else
		{
			// CVC: maybe char temp[IAD_LEN(16)]; may work but it won't be aligned.
			SLONG temp[IAD_LEN(16) / 4];
			internal_array_desc* p_ads = reinterpret_cast<internal_array_desc*>(temp);
			blb* blob = BLB_get_array(tdbb, transaction, blob_id, p_ads);
			array =	alloc_array(transaction, p_ads);
			array->arr_effective_length =
				blob->blb_length - array->arr_desc.iad_length;
			BLB_get_data(tdbb, blob, array->arr_data,
						 array->arr_desc.iad_total_length);
			arg.slice_high_water = array->arr_data + array->arr_effective_length;
			array->arr_blob = allocate_blob(tdbb, transaction);
			(array->arr_blob)->blb_blob_id = *blob_id;
		}
	}
	else if (blob_id->bid_temp_id())
	{
		array = find_array(transaction, blob_id);
		if (!array) {
			ERR_post(isc_invalid_array_id, 0);
		}

		arg.slice_high_water = array->arr_data + array->arr_effective_length;
	}
	else {
		array = alloc_array(transaction, &array_desc->arr_desc);
		arg.slice_high_water = array->arr_data;
	}

/* Walk array */

	arg.slice_desc = info.sdl_info_element;
	arg.slice_desc.dsc_address = slice_addr;
	arg.slice_end = slice_addr + slice_length;
	arg.slice_count = 0;
	arg.slice_element_length = info.sdl_info_element.dsc_length;
	arg.slice_direction = TRUE;	/* storing INTO array */
	arg.slice_base = array->arr_data;

	SLONG variables[64];
	MOVE_FAST(param, variables, MIN(sizeof(variables), param_length));

	if (SDL_walk(tdbb->tdbb_status_vector,
				 sdl,
				 true,
				 array->arr_data,
				 &array_desc->arr_desc,
				 variables,
				 slice_callback,
				 &arg))
	{
		ERR_punt();
	}

	const SLONG length = arg.slice_high_water - array->arr_data;

	if (length > array->arr_effective_length) {
		array->arr_effective_length = length;
	}

	blob_id->set_temporary(array->arr_temp_id);
}


void BLB_release_array(ArrayField* array)
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

	jrd_tra* transaction = array->arr_transaction;
	if (transaction)
	{
		for (ArrayField** ptr = &transaction->tra_arrays; *ptr;
			ptr = &(*ptr)->arr_next)
		{
			if (*ptr == array) {
				*ptr = array->arr_next;
				break;
			}
		}
	}

	delete array;
}


void BLB_scalar(thread_db*		tdbb,
				jrd_tra*		transaction,
				const bid*		blob_id,
				USHORT			count,
				SLONG*			subscripts,
				impure_value*	value)
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

	SLONG stuff[IAD_LEN(16) / 4];

	SET_TDBB(tdbb);

	internal_array_desc* array_desc = (internal_array_desc*) stuff;
	blb* blob = BLB_get_array(tdbb, transaction, blob_id, array_desc);

// Get someplace to put data.
// We need DOUBLE_ALIGNed buffer, that's why some tricks
	Firebird::HalfStaticArray<double, 64> temp;
	dsc desc = array_desc->iad_rpt[0].iad_desc;
	desc.dsc_address = reinterpret_cast<UCHAR*>
		(temp.getBuffer((desc.dsc_length / sizeof(double)) +
			(desc.dsc_length % sizeof(double) ? 1 : 0)));

	const SLONG number =
		SDL_compute_subscript(tdbb->tdbb_status_vector, array_desc, count,
							  subscripts);
	if (number < 0) {
		BLB_close(tdbb, blob);
		ERR_punt();
	}

	const SLONG offset = number * array_desc->iad_element_length;
	BLB_lseek(blob, 0, offset + (SLONG) array_desc->iad_length);
	BLB_get_segment(tdbb, blob, desc.dsc_address, desc.dsc_length);

/* If we have run out of data, then clear the data buffer. */

	if (blob->blb_flags & BLB_eof) {
		memset(desc.dsc_address, 0, (int) desc.dsc_length);
	}
	EVL_make_value(tdbb, &desc, value);
	BLB_close(tdbb, blob);
}


static ArrayField* alloc_array(jrd_tra* transaction, internal_array_desc* proto_desc)
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

	Database* dbb = GET_DBB();

	// Compute size and allocate block

	const USHORT n = MAX(proto_desc->iad_struct_count, proto_desc->iad_dimensions);
	ArrayField* array = FB_NEW_RPT(*transaction->tra_pool, n) ArrayField();

	// Copy prototype descriptor

	MOVE_FAST(proto_desc, &array->arr_desc, proto_desc->iad_length);

	// Link into transaction block

	array->arr_next = transaction->tra_arrays;
	transaction->tra_arrays = array;
	array->arr_transaction = transaction;

	// Allocate large block to hold array

	array->arr_data =
		(UCHAR*)dbb->dbb_permanent->allocate(array->arr_desc.iad_total_length, 0
#ifdef DEBUG_GDS_ALLOC
		  ,__FILE__, __LINE__
#endif
		);
	array->arr_temp_id = ++transaction->tra_next_blob_id;

	return array;
}


static blb* allocate_blob(thread_db* tdbb, jrd_tra* transaction)
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
	Database* dbb = tdbb->tdbb_database;

/* Create a blob large enough to hold a single data page */

	blb* blob = FB_NEW_RPT(*transaction->tra_pool, dbb->dbb_page_size) blb();
	blob->blb_attachment = tdbb->tdbb_attachment;
	blob->blb_transaction = transaction;

/* Compute some parameters governing various maximum sizes based on
   database page size. */

	blob->blb_clump_size = dbb->dbb_page_size -
							sizeof(Ods::data_page) -
							sizeof(Ods::data_page::dpg_repeat) -
							sizeof(Ods::blh);
	blob->blb_max_pages = blob->blb_clump_size >> SHIFTLONG;
	blob->blb_pointers = (dbb->dbb_page_size - BLP_SIZE) >> SHIFTLONG;
	// This code is to handle huge number of blob updates done in one transaction.
	// Blob index counter may wrap in this case
	do {
		transaction->tra_next_blob_id++;
		// Do not generate null blob ID
		if (!transaction->tra_next_blob_id)
			transaction->tra_next_blob_id++;
	} while (!transaction->tra_blobs.add(BlobIndex(transaction->tra_next_blob_id, blob)));
	blob->blb_temp_id = transaction->tra_next_blob_id;

	return blob;
}


static ISC_STATUS blob_filter(	USHORT	action,
							BlobControl*	control,
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

/* Note: Cannot remove this JRD_get_thread_data without API change to
   blob filter routines */

	thread_db* tdbb = JRD_get_thread_data();

	jrd_tra* transaction = (jrd_tra*) control->ctl_internal[1];
	bid* blob_id = reinterpret_cast<bid*>(control->ctl_internal[2]);

#ifdef DEV_BUILD
	if (transaction) {
		BLKCHK(transaction, type_tra);
	}
#endif

	blb* blob = 0;

	switch (action) {
	case isc_blob_filter_open:
		blob = BLB_open2(tdbb, transaction, blob_id, 0, 0);
		control->source_handle = blob;
		control->ctl_total_length = blob->blb_length;
		control->ctl_max_segment = blob->blb_max_segment;
		control->ctl_number_segments = blob->blb_count;
		return FB_SUCCESS;

	case isc_blob_filter_get_segment:
		blob = control->source_handle;
		control->ctl_segment_length =
			BLB_get_segment(tdbb, blob, control->ctl_buffer,
							control->ctl_buffer_length);
		if (blob->blb_flags & BLB_eof) {
			return isc_segstr_eof;
		}
		if (blob->blb_fragment_size) {
			return isc_segment;
		}
		return FB_SUCCESS;

	case isc_blob_filter_create:
		control->source_handle =
			BLB_create2(tdbb, transaction, blob_id, 0, NULL);
		return FB_SUCCESS;

	case isc_blob_filter_put_segment:
		blob = control->source_handle;
		BLB_put_segment(tdbb, blob, control->ctl_buffer,
						control->ctl_buffer_length);
		return FB_SUCCESS;

	case isc_blob_filter_close:
		BLB_close(tdbb, control->source_handle);
		return FB_SUCCESS;

	case isc_blob_filter_alloc:
	    // pointer to ISC_STATUS!!!
		return (ISC_STATUS) FB_NEW(*transaction->tra_pool) BlobControl(*transaction->tra_pool);

	case isc_blob_filter_free:
		delete control;
		return FB_SUCCESS;

	case isc_blob_filter_seek:
		return BLB_lseek(control->source_handle, mode, offset);

	default:
		ERR_post(isc_uns_ext, 0);
		return FB_SUCCESS;
	}
}


static blb* copy_blob(thread_db* tdbb, const bid* source, bid* destination, USHORT bpb_length, const UCHAR* bpb)
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
	SET_TDBB(tdbb);

	jrd_req* request = tdbb->tdbb_request;
	blb* input = BLB_open2(tdbb, request->req_transaction, source, bpb_length, bpb);
	blb* output = BLB_create(tdbb, request->req_transaction, destination);
	output->blb_sub_type = input->blb_sub_type;

	if (input->blb_flags & BLB_stream) {
		output->blb_flags |= BLB_stream;
	}

	Firebird::HalfStaticArray<UCHAR, 2048> buffer;
	UCHAR* buff = buffer.getBuffer(input->blb_max_segment);

	while (true) {
		const USHORT length =
			BLB_get_segment(tdbb, input, buff, input->blb_max_segment);
		if (input->blb_flags & BLB_eof) {
			break;
		}
		BLB_put_segment(tdbb, output, buff, length);
	}

	BLB_close(tdbb, input);
	BLB_close(tdbb, output);

	return output;
}


static void delete_blob(thread_db* tdbb, blb* blob, ULONG prior_page)
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
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	if (dbb->dbb_flags & DBB_read_only)
		ERR_post(isc_read_only_database, 0);

/* Level 0 blobs don't need cleanup */

	if (blob->blb_level == 0)
		return;

/* Level 1 blobs just need the root page level released */

	vcl* vector = blob->blb_pages;
	vcl::iterator ptr = vector->begin();
	const vcl::iterator end = vector->end();

	if (blob->blb_level == 1) {
		for (; ptr < end; ptr++) {
			if (*ptr) {
				PAG_release_page(*ptr, prior_page);
			}
		}
		return;
	}

/* Level 2 blobs need a little more work to keep the page precedence
   in order.  The basic problem is that the pointer page has to be
   released before the data pages that it points to.  Sigh. */

	WIN window(-1);
	window.win_flags = WIN_large_scan;
	window.win_scans = 1;

	for (; ptr < end; ptr++)
		if ( (window.win_page = *ptr) ) {
			blob_page* page = (blob_page*) CCH_FETCH(tdbb, &window, LCK_read, pag_blob);
			MOVE_FASTER(page, blob->blb_data, dbb->dbb_page_size);
			CCH_RELEASE_TAIL(tdbb, &window);
			PAG_release_page(*ptr, prior_page);
			page = (blob_page*) blob->blb_data;
			SLONG* ptr2 = page->blp_page;
			for (const SLONG* const end2 = ptr2 + blob->blb_pointers;
				 ptr2 < end2; ptr2++)
			{
				if (*ptr2) {
					PAG_release_page(*ptr2, *ptr);
				}
			}
		}
}


static void delete_blob_id(
						   thread_db* tdbb,
						   const bid* blob_id, SLONG prior_page, jrd_rel* relation)
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
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* If the blob is null, don't bother to delete it.  Reasonable? */

	if (blob_id->isEmpty())
		return;

	if (blob_id->bid_internal.bid_relation_id != relation->rel_id)
		CORRUPT(200);			/* msg 200 invalid blob id */

/* Fetch blob */

	blb* blob = allocate_blob(tdbb, dbb->dbb_sys_trans);
	blob->blb_relation = relation;
	prior_page =
		DPM_get_blob(tdbb, blob, blob_id->get_permanent_number(), true,
					 prior_page);

	if (!(blob->blb_flags & BLB_damaged))
		delete_blob(tdbb, blob, prior_page);

	release_blob(blob, true);
}


static ArrayField* find_array(jrd_tra* transaction, const bid* blob_id)
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
	ArrayField* array = transaction->tra_arrays;

	for (; array; array = array->arr_next) {
		if (array->arr_temp_id == blob_id->bid_temp_id()) {
			break;
		}
	}

	return array;
}


static BlobFilter* find_filter(thread_db* tdbb, SSHORT from, SSHORT to)
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
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	BlobFilter* cache = dbb->dbb_blob_filters;
	for (; cache; cache = cache->blf_next) {
		if (cache->blf_from == from && cache->blf_to == to)
			return cache;
	}

	cache = BLF_lookup_internal_filter(tdbb, from, to);
	if (!cache) {
		cache = MET_lookup_filter(tdbb, from, to);
	}

	if (cache) {
		cache->blf_next = dbb->dbb_blob_filters;
		dbb->dbb_blob_filters = cache;
	}

	return cache;
}


static blob_page* get_next_page(thread_db* tdbb, blb* blob, WIN * window)
{
/**************************************
 *
 *      g e t _ n e x t _ p a g e
 *
 **************************************
 *
 * Functional description
 *      Read a blob page and copy it into the blob data area.  Return
 *      the next page. if there's no next page, return NULL.
 *
 **************************************/
	if (blob->blb_level == 0 || blob->blb_sequence > blob->blb_max_sequence) {
		blob->blb_space_remaining = 0;
		return NULL;
	}

	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	vcl* vector = blob->blb_pages;

#ifdef SUPERSERVER_V2
	SLONG pages[PREFETCH_MAX_PAGES];
#endif


	blob_page* page = 0;
/* Level 1 blobs are much easier -- page number is in vector. */
	if (blob->blb_level == 1) {
#ifdef SUPERSERVER_V2
		/* Perform prefetch of blob level 1 data pages. */

		if (!(blob->blb_sequence % dbb->dbb_prefetch_sequence)) {
			USHORT sequence = blob->blb_sequence;
			USHORT i = 0;
			while (i < dbb->dbb_prefetch_pages &&
				sequence <= blob->blb_max_sequence)
			{
				 pages[i++] =
					(*vector)[sequence++];
			}

			CCH_PREFETCH(tdbb, pages, i);
		}
#endif
		window->win_page = (*vector)[blob->blb_sequence];
		page = (blob_page*) CCH_FETCH(tdbb, window, LCK_read, pag_blob);
	}
	else {
		window->win_page =
			(*vector)[blob->blb_sequence / blob->blb_pointers];
		page = (blob_page*) CCH_FETCH(tdbb, window, LCK_read, pag_blob);
#ifdef SUPERSERVER_V2
		/* Perform prefetch of blob level 2 data pages. */

		USHORT sequence = blob->blb_sequence % blob->blb_pointers;
		if (!(sequence % dbb->dbb_prefetch_sequence))
		{
			ULONG abs_sequence = blob->blb_sequence;
			USHORT i = 0;
			while (i < dbb->dbb_prefetch_pages &&
						sequence < blob->blb_pointers &&
						abs_sequence <= blob->blb_max_sequence)
			{
				pages[i++] = page->blp_page[sequence++];
				abs_sequence++;
			}

			CCH_PREFETCH(tdbb, pages, i);
		}
#endif
		page = (blob_page*)CCH_HANDOFF(tdbb,
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
static void get_replay_blob(thread_db* tdbb, bid* blob_id)
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
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* we're only interested in newly created blobs */

	if (blob_id->bid_internal.bid_relation_id != 0)
		return;

/* search the linked list for the old blob id */

	for (map* map_ptr = dbb->dbb_blob_map; map_ptr; map_ptr = map_ptr->map_next)
	{
		if (blob_id->bid_temp_id() == map_ptr->map_old_blob)
		{
			blob_id->bid_temp_id() = map_ptr->map_new_blob;
			break;
		}
	}
}
#endif


static void insert_page(thread_db* tdbb, blb* blob)
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
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	const USHORT length = dbb->dbb_page_size - blob->blb_space_remaining;
	vcl* vector = blob->blb_pages;
	blob->blb_max_sequence = blob->blb_sequence;

/* Allocate a page for the now full blob data page.  Move the page
   image to the buffer, and release the page.  */

	WIN window(-1);
	blob_page* page = (blob_page*) DPM_allocate(tdbb, &window);
	const ULONG page_number = window.win_page;

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
		page = (blob_page*) DPM_allocate(tdbb, &window);
		page->blp_header.pag_flags = Ods::blp_pointers;
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

	USHORT l = blob->blb_sequence / blob->blb_pointers;

	if (l < vector->count()) {
		window.win_page = (*vector)[l];
		window.win_flags = 0;
		page = (blob_page*) CCH_FETCH(tdbb, &window, LCK_write, pag_blob);
	}
	else {
		page = (blob_page*) DPM_allocate(tdbb, &window);
		page->blp_header.pag_flags = Ods::blp_pointers;
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


static void release_blob(blb* blob, const bool purge_flag)
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
 *      is false, then only release the associated blocks.
 *
 **************************************/
	jrd_tra* transaction = blob->blb_transaction;

/* Disconnect blob from transaction block. */

	if (purge_flag) {
		if (transaction->tra_blobs.locate(blob->blb_temp_id)) {
			jrd_req* blob_request = transaction->tra_blobs.current().bli_request;
			if (blob_request) {
				if (blob_request->req_blobs.locate(blob->blb_temp_id)) {
					blob_request->req_blobs.fastRemove();
				} else {
					// We should never get here because when bli_request is assigned
					// item should be added to req_blobs array
					fb_assert(false);
				}
			}
			transaction->tra_blobs.fastRemove();
		} else {
			// We should never get here because allocate_blob stores each blob object
			// in tra_blobs
			fb_assert(false);
		}
	}

	if (blob->blb_pages) {
		delete blob->blb_pages;
		blob->blb_pages = NULL;
	}

	delete blob;
}


static void slice_callback(array_slice* arg, ULONG count, DSC* descriptors)
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
	dsc* array_desc = descriptors;
	dsc* slice_desc = &arg->slice_desc;
	BLOB_PTR* const next =
		slice_desc->dsc_address + arg->slice_element_length;

	if (next > arg->slice_end)
		ERR_post(isc_out_of_bounds, 0);

	if (array_desc->dsc_address < arg->slice_base)
		ERR_error(198);			/* msg 198 array subscript computation error */

	if (arg->slice_direction) {

		/* Storing INTO array */
		/* FROM slice_desc TO array_desc */

		/* If storing beyond the high-water mark, ensure elements
		 * from the high-water mark to current position are zeroed
		 */

		// Since we are only initializing, it makes sense to throw away
		// the constness of arg->slice_high_water.
		const SLONG l = array_desc->dsc_address - arg->slice_high_water;
		if (l > 0)
			memset(const_cast<BLOB_PTR*>(arg->slice_high_water), 0, l);

		/* The individual elements of a varying string array may not be aligned
		   correctly.  If they aren't, some RISC machines may break.  In those
		   cases, calculate the actual length and then move the length and
		   text manually. */

		if (array_desc->dsc_dtype == dtype_varying &&
			(U_IPTR) array_desc->dsc_address !=
			FB_ALIGN((U_IPTR) array_desc->dsc_address,
					 (MIN(sizeof(USHORT), ALIGNMENT))))
		{
			/* Note: cannot remove this JRD_get_thread_data without api change
			   to slice callback routines */
			thread_db* tdbb = JRD_get_thread_data();

			Firebird::HalfStaticArray<char, 1024> tmp_buffer;
			const USHORT tmp_len = array_desc->dsc_length;
			const char* p;
			const USHORT len = MOV_make_string(slice_desc,
								  INTL_TEXT_TYPE(*array_desc),
								  &p,
								  reinterpret_cast<vary*>(tmp_buffer.getBuffer(tmp_len)),
								  tmp_len);
			MOVE_FAST(&len, array_desc->dsc_address, sizeof(USHORT));
			MOVE_FAST(p, array_desc->dsc_address + sizeof(USHORT), (int) len);
		}
		else
		{
			MOV_move(slice_desc, array_desc);
		}
		const BLOB_PTR* const end =
			array_desc->dsc_address + array_desc->dsc_length;
		if (end > arg->slice_high_water)
			arg->slice_high_water = end;
	}
	else {

		/* Fetching FROM array */
		/* FROM array_desc TO slice_desc */

		/* If the element is under the high-water mark, fetch it,
		 * otherwise just zero it
		 */
		if (array_desc->dsc_address < arg->slice_high_water) {
			/* If a varying string isn't aligned correctly, calculate the actual
			   length and then treat the string as if it had type text. */

			if (array_desc->dsc_dtype == dtype_varying &&
				(U_IPTR) array_desc->dsc_address !=
				FB_ALIGN((U_IPTR) array_desc->dsc_address,
						 (MIN(sizeof(USHORT), ALIGNMENT))))
			{
			    // temp_desc will vanish at the end of the block, but it's used
			    // only as a way to transfer blocks of memory.
				dsc temp_desc;
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
		else {
		    const SLONG l = slice_desc->dsc_length;
			if (l)
				memset(slice_desc->dsc_address, 0, l);
		}
	}

	slice_desc->dsc_address = next;
}


static blb* store_array(thread_db* tdbb, jrd_tra* transaction, bid* blob_id)
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
	SET_TDBB(tdbb);

/* Validate array */

	ArrayField* array = find_array(transaction, blob_id);
	if (!array)
		return NULL;

/* Create blob for array */

	blb* blob = BLB_create2(tdbb, transaction, blob_id, 0, NULL);
	blob->blb_flags |= BLB_stream;

/* Write out array descriptor */

	BLB_put_segment(tdbb, blob,
					reinterpret_cast<const UCHAR*>(&array->arr_desc),
					array->arr_desc.iad_length);

/* Write out actual array */
	const USHORT seg_limit = 32768;
	const BLOB_PTR* p = array->arr_data;
	SLONG length = array->arr_effective_length;
	while (length > seg_limit) {
		BLB_put_segment(tdbb, blob, p, seg_limit);
		length -= seg_limit;
		p += seg_limit;
	}

	if (length)
		BLB_put_segment(tdbb, blob, p, (USHORT) length);

	BLB_close(tdbb, blob);

	return blob;
}

