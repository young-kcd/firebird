/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		nav.cpp
 *	DESCRIPTION:	Navigational index walking
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
#include "memory_routines.h"
#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../jrd/ods.h"
#include "../jrd/val.h"
#include "../jrd/btr.h"
#include "../jrd/btn.h"
#include "../jrd/req.h"
#include "../jrd/exe.h"
#include "../jrd/sbm.h"
#include "../jrd/rse.h"
#include "gen/iberror.h"
#include "../jrd/rng.h"
#include "../jrd/lck.h"
#include "../jrd/cch.h"
#include "../jrd/blr.h"
#include "../jrd/all_proto.h"
#include "../jrd/btr_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/dbg_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/nav_proto.h"
#include "../jrd/rng_proto.h"
#include "../jrd/rse_proto.h"
#include "../jrd/thd.h"
#include "../jrd/vio_proto.h"

using namespace Jrd;

#define MOVE_BYTE(x_from, x_to)	*x_to++ = *x_from++;

static int compare_keys(const index_desc*, const UCHAR*, USHORT, 
						const temporary_key*, USHORT);

#ifdef SCROLLABLE_CURSORS
static void expand_index(WIN *);
#endif
#ifdef PC_ENGINE
static bool find_dbkey(RecordSource*, ULONG);
static bool find_record(RecordSource*, RSE_GET_MODE, temporary_key*, USHORT, USHORT);
#endif
static btree_exp* find_current(exp_index_buf*, Ods::btree_page*, const UCHAR*);
static bool find_saved_node(RecordSource*, IRSB_NAV, WIN *, UCHAR **);
static UCHAR* get_position(thread_db*, RecordSource*, IRSB_NAV, WIN *, RSE_GET_MODE, btree_exp**);
static bool get_record(RecordSource*, IRSB_NAV, record_param*, temporary_key*, bool);
static void init_fetch(IRSB_NAV);
static UCHAR* nav_open(thread_db*, RecordSource*, IRSB_NAV, WIN *, RSE_GET_MODE, btree_exp**);
static void set_position(IRSB_NAV, record_param*, WIN*, const UCHAR*, btree_exp*, const UCHAR*, USHORT);
static void setup_bitmaps(RecordSource*, IRSB_NAV);


#ifdef SCROLLABLE_CURSORS
exp_index_buf* NAV_expand_index(WIN * window, IRSB_NAV impure)
{
/**************************************
 *
 *	N A V _ e x p a n d _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Given a window with a btree leaf page loaded into it, 
 *	expand the index page into a form that facilitates walking
 *	backward through it.  Each node's data is decompressed.  
 *	Since the prefix field is not needed, it contains an offset to 
 *	the prior node.
 *
 *	If an impure pointer is passed, set the current node on the 
 *	expanded page to match that on the corresponding btree page.
 *
 **************************************/

	// allocate the expanded page to allow room for all nodes fully expanded,
	// plus leave room for an extra node with zero-length tail
	// (note that the difference in size of a btree_exp node versus a btree_nod node could
	// be deducted, but there is no easy way to get the no. of nodes on page) */

	// if the right version of expanded page is available, there 
	// is no work to be done 
	exp_index_buf* expanded_page = window->win_expanded_buffer;
	if (expanded_page &&
		(expanded_page->exp_incarnation == CCH_get_incarnation(window)))
	{
		return expanded_page;
	}

	// If different incarnation, reallocate the page
	if (expanded_page) {
		ALL_free(expanded_page);
	}

	btree_page* page = (btree_page*) window->win_buffer;

	expanded_page = (exp_index_buf*) ALL_malloc(EXP_SIZE + page->btr_prefix_total +
		(SLONG) page->btr_length + BTX_SIZE, ERR_jmp);
	window->win_expanded_buffer = expanded_page;
	expanded_page->exp_incarnation = -1;

	expand_index(window);

	if (!impure) {
		return expanded_page;
	}

	// go through the nodes on the original page and reposition
	UCHAR* pointer = BTreeNode::getPointerFirstNode(page);
	const UCHAR* const endPointer = ((UCHAR*) page + page->btr_length);
	btree_exp* expanded_node = (btree_exp*) expanded_page->exp_nodes;
	const UCHAR* current_pointer = ((UCHAR*) page + impure->irsb_nav_offset);

	impure->irsb_nav_expanded_offset = -1;

	IndexNode node;
	while (pointer < endPointer) {
		if (pointer == current_pointer) {
			impure->irsb_nav_expanded_offset =
				(UCHAR*) expanded_node - (UCHAR*) expanded_page;
		}
		pointer = BTreeNode::nextNode(pointer, &expanded_node);
	}

	return expanded_page;
}
#endif


#ifdef PC_ENGINE
bool NAV_find_record(RecordSource* rsb,
						USHORT blr_operator, USHORT direction, jrd_nod* find_key)
{
/**************************************
 *
 *	N A V _ f i n d _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Find a record with the specified key value
 *	in a stream opened on a navigational index.
 *	This routine must set BOF, EOF, or CRACK.
 *
 **************************************/
	const bool backwards = (direction == blr_backward
				 || direction == blr_backward_starting);

	USHORT search_flags;
	if (direction == blr_forward_starting || direction == blr_backward_starting)
		search_flags = irb_starting;
	else
		search_flags = 0;


	thread_db* tdbb = JRD_get_thread_data();
	jrd_req* request = tdbb->tdbb_request;
	irsb_nav* impure = (IRSB_NAV) ((UCHAR *) request + rsb->rsb_impure);
	WIN window(-1);

	btree_nod* expanded_node;
	init_fetch(impure);
	if (!impure->irsb_nav_page) {
		nav_open(tdbb, rsb, impure, &window, RSE_get_forward, &expanded_node);
		CCH_RELEASE(tdbb, &window);
	}

	// finding a record invalidates the visited records
	SBM_reset(&impure->irsb_nav_records_visited);

	index_desc* idx =
		(index_desc*) ((SCHAR *) impure + (SLONG) rsb->rsb_arg[RSB_NAV_idx_offset]);

	if ((idx == NULL) || (find_key->nod_count == 0) ||
		(find_key->nod_count > idx->idx_count))
	{
		ERR_post(gds__invalid_key, 0);
	}

	if (find_key->nod_count < idx->idx_count)
		search_flags |= irb_partial;

	temporary_key key_value;
	BTR_make_key(tdbb, find_key->nod_count, &find_key->nod_arg[0], idx,
				 &key_value, (search_flags & irb_starting) != 0);

	// save the key value 
	impure->irsb_nav_length = key_value.key_length;
	MOVE_FAST(key_value.key_data, impure->irsb_nav_data,
			  key_value.key_length);

	// for descending indices, reverse the sense of the comparison operator 
	// to simplify the following switch statement
	if (idx->idx_flags & idx_descending) {
		switch (blr_operator)
		{
		case blr_lss:
			blr_operator = blr_gtr;
			break;
		case blr_leq:
			blr_operator = blr_geq;
			break;
		case blr_gtr:
			blr_operator = blr_lss;
			break;
		case blr_geq:
			blr_operator = blr_leq;
			break;
		default:    // silence compiler
			break;
		}
	}

	// find the proper record by comparing the passed key with the records in
	// the record stream, optimizing the fetch as best as possible by using the
	// index to get as close to the proper record as possible
	switch (blr_operator) {
	case blr_lss:
		if (backwards)
			// find the first record before the given key value
		{
			find_record(rsb, RSE_get_first, &key_value, find_key->nod_count,
						search_flags);
			return NAV_get_record(rsb, impure,
								  request->req_rpb + rsb->rsb_stream,
								  RSE_get_backward);
		}
		else
			// find the first record in the stream, assuming its key value is < the passed key
		{
			nav_open(tdbb, rsb, impure, &window, RSE_get_forward,
					 &expanded_node);
			CCH_RELEASE(tdbb, &window);
			if (!NAV_get_record
				(rsb, impure, request->req_rpb + rsb->rsb_stream,
				 RSE_get_forward))
			{
				return false;
			}

			if (compare_keys
				(idx, impure->irsb_nav_data, impure->irsb_nav_length,
				 &key_value, search_flags) < 0)
			{
				return true;
			}
			else {
				RSE_MARK_CRACK(tdbb, rsb, irsb_crack);
				return false;
			}
		}
	//	break;  unreachable

	case blr_leq:
		if (backwards)
			// find the last record with the matching key; barring that, find the first less than 
		{
			if (find_record
				(rsb, RSE_get_last, &key_value, find_key->nod_count,
				 search_flags))
			{
				return true;
			}
			return NAV_get_record(rsb, impure,
								  request->req_rpb + rsb->rsb_stream,
								  RSE_get_backward);
		}
		else
			// find the first record in the stream, assuming its key value is <= the passed key
		{
			nav_open(tdbb, rsb, impure, &window, RSE_get_forward,
					 &expanded_node);
			CCH_RELEASE(tdbb, &window);
			if (!NAV_get_record
				(rsb, impure, request->req_rpb + rsb->rsb_stream,
				 RSE_get_forward))
			{
				return false;
			}

			if (compare_keys
				(idx, impure->irsb_nav_data, impure->irsb_nav_length,
				 &key_value, search_flags) <= 0)
			{
				return true;
			}
			else {
				RSE_MARK_CRACK(tdbb, rsb, irsb_crack);
				return false;
			}
		}
	//	break;  unreachable

	case blr_equiv:
	case blr_eql:
		if (find_record(rsb, backwards ? RSE_get_last : RSE_get_first,
						&key_value, find_key->nod_count, search_flags))
		{
			return true;
		}
		else {
			// We need to override the crack semantics of find_record, because even 
			// if we go to EOF or BOF as a result of the find, this is defined as a
			// crack.  On the other hand, we do need to retain a forced crack if 
			// one was set.  bug #7024

			if (impure->irsb_flags & irsb_forced_crack)
				RSE_MARK_CRACK(tdbb, rsb, irsb_crack | irsb_forced_crack);
			else
				RSE_MARK_CRACK(tdbb, rsb, irsb_crack);
			return false;
		}
	//	break;  unreachable

	case blr_gtr:
		if (backwards)
			// find the last record in the stream, assuming its key value is > the passed key
		{
			nav_open(tdbb, rsb, impure, &window, RSE_get_backward,
					 &expanded_node);
			CCH_RELEASE(tdbb, &window);
			if (!NAV_get_record
				(rsb, impure, request->req_rpb + rsb->rsb_stream,
				 RSE_get_backward))
			{
				return false;
			}

			if (compare_keys
				(idx, impure->irsb_nav_data, impure->irsb_nav_length,
				 &key_value, search_flags) > 0)
			{
				return true;
			}
			else {
				RSE_MARK_CRACK(tdbb, rsb, irsb_crack);
				return false;
			}
		}
		else
			// find the first record after the last matching record
		{
			find_record(rsb, RSE_get_last, &key_value, find_key->nod_count,
						search_flags);

			// special case when the key is greater than the last record in the file;
			// stream is defined to be on EOF; see bug #6151

			if (impure->irsb_flags & irsb_eof)
				return false;

			return NAV_get_record(rsb, impure,
								  request->req_rpb + rsb->rsb_stream,
								  RSE_get_forward);
		}
	//	break;  unreachable

	case blr_geq:
		if (backwards)
			// find the last record in the stream, assuming its key value is >= the passed key
		{
			nav_open(tdbb, rsb, impure, &window, RSE_get_backward,
					 &expanded_node);
			CCH_RELEASE(tdbb, &window);
			if (!NAV_get_record
				(rsb, impure, request->req_rpb + rsb->rsb_stream,
				 RSE_get_backward))
			{
				return false;
			}

			if (compare_keys
				(idx, impure->irsb_nav_data, impure->irsb_nav_length,
				 &key_value, search_flags) >= 0)
			{
				return true;
			}
			else {
				RSE_MARK_CRACK(tdbb, rsb, irsb_crack);
				return false;
			}
		}
		else
			// find the first record of matching key; barring that, find the first greater
		{
			if (find_record
				(rsb, RSE_get_first, &key_value, find_key->nod_count,
				 search_flags))
			{
				return true;
			}

			// special case when the key is greater than the last record in the file;
			// stream is defined to be on EOF; see bug #6151

			if (impure->irsb_flags & irsb_eof)
				return false;

			return NAV_get_record(rsb, impure,
								  request->req_rpb + rsb->rsb_stream,
								  RSE_get_forward);
		}
	//	break;  unreachable
	}
}
#endif


#ifdef PC_ENGINE
void NAV_get_bookmark(RecordSource* rsb, IRSB_NAV impure, Bookmark* bookmark)
{
/**************************************
 *
 *	N A V _ g e t _ b o o k m a r k
 *
 **************************************
 *
 * Functional description
 *	Return a descriptor containing a pointer
 * 	to a bookmark data structure which describes
 *	the current location of a navigational stream.
 *
 **************************************/

	// store the info necessary to return to this location in the index
	bookmark->bkm_number = impure->irsb_nav_number;
	bookmark->bkm_page = impure->irsb_nav_page;
	bookmark->bkm_incarnation = impure->irsb_nav_incarnation;
	bookmark->bkm_offset = impure->irsb_nav_offset;
	bookmark->bkm_expanded_offset = impure->irsb_nav_expanded_offset;

	// store the current key value, setting up the key descriptor to point at it
	bookmark->bkm_key_desc.dsc_dtype = dtype_text;
	bookmark->bkm_key_desc.dsc_length = impure->irsb_nav_length;
	bookmark->bkm_key_desc.dsc_address = bookmark->bkm_key_data;
	MOVE_FAST(impure->irsb_nav_data, bookmark->bkm_key_data,
			  impure->irsb_nav_length);
}
#endif


bool NAV_get_record(thread_db* tdbb,
					   RecordSource* rsb,
					   IRSB_NAV impure, record_param* rpb, RSE_GET_MODE direction)
{
/**************************************
 *
 *	N A V _ g e t _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Get a record from a stream, either in
 *	the forward or backward direction, or the
 *	current record.  This routine must set 
 *	BOF, EOF, or CRACK properly.
 *
 **************************************/
	SET_TDBB(tdbb);

#ifdef SCROLLABLE_CURSORS
	// before we do anything, check for the case where an ascending index 
	// was used to optimize a descending sort, or a descending index was 
	// used to optimize an ascending sort--in either case, we need to flip 
	// the nominal direction of navigation
	if (rsb->rsb_flags & rsb_descending) {
		if (direction == RSE_get_forward) {
			direction = RSE_get_backward;
		}
		else if (direction == RSE_get_backward) {
			direction = RSE_get_forward;
		}
	}
#endif

	init_fetch(impure);
	index_desc* idx = (index_desc*) ((SCHAR*) impure + (IPTR) rsb->rsb_arg[RSB_NAV_idx_offset]);

	// The bitmap is only valid when we are continuing on in one 
	// direction.  It is of no help when we change direction,
	// and so we need to reset in that case.
	if (((impure->irsb_flags & irsb_backwards)
		 && direction != RSE_get_backward)
		|| (!(impure->irsb_flags & irsb_backwards)
			&& direction != RSE_get_forward))
	{
		RecordBitmap::reset(impure->irsb_nav_records_visited);
	}

	if (direction == RSE_get_forward) {
		impure->irsb_flags &= ~irsb_backwards;
	}
	else if (direction == RSE_get_backward) {
		impure->irsb_flags |= irsb_backwards;
	}

	// find the last fetched position from the index
	WIN window(impure->irsb_nav_page);

	btree_exp* expanded_next = NULL;
	UCHAR* nextPointer = get_position(tdbb, rsb, impure, &window, 
		direction, &expanded_next);
	temporary_key key;
	MOVE_FAST(impure->irsb_nav_data, key.key_data, impure->irsb_nav_length);
	jrd_nod* retrieval_node = (jrd_nod*) rsb->rsb_arg[RSB_NAV_index];
	IndexRetrieval* retrieval = 
		(IndexRetrieval*) retrieval_node->nod_arg[e_idx_retrieval];

	// set the upper (or lower) limit for navigational retrieval
	temporary_key upper, lower;
	if ((direction == RSE_get_forward) && retrieval->irb_upper_count) {
		upper.key_length = impure->irsb_nav_upper_length;
#ifdef SCROLLABLE_CURSORS
		MOVE_FAST(
				  (impure->irsb_nav_data +
				   (2 * (SLONG) rsb->rsb_arg[RSB_NAV_key_length])),
				  upper.key_data, upper.key_length);
#else
		MOVE_FAST(
				  (impure->irsb_nav_data +
				   (IPTR) rsb->rsb_arg[RSB_NAV_key_length]), upper.key_data,
				  upper.key_length);
#endif
	}
	else if ((direction == RSE_get_backward) && retrieval->irb_lower_count) {
		lower.key_length = impure->irsb_nav_lower_length;
		MOVE_FAST(
				  (impure->irsb_nav_data +
				   (IPTR) rsb->rsb_arg[RSB_NAV_key_length]), lower.key_data,
				  lower.key_length);
	}

	// In the case of a DISTINCT, we must detect whether the key changed since the last 
	// time a record was returned from the rsb.  It is not good enough to know whether the 
	// key changed since the last node in the index, because the record represented 
	// by the last index node may not have satisfied some boolean expression on the rsb.  
	// So to find out whether the key has changed since the last time a record was actually 
	// returned from the rsb, we save the record count each time we return a candidate record, 
	// and see if the count is updated the next time we get here.

	// If the count has been updated, we know the record has been returned from the rsb, and 
	// the index key must change before we can return another DISTINCT record candidate.  If 
	// the last candidate did not qualify, we know we should return another record no matter 
	// what.

	if (rsb->rsb_record_count != impure->irsb_nav_count) {
		impure->irsb_flags &= ~irsb_key_changed;
		impure->irsb_nav_count = rsb->rsb_record_count;
	}
	else {
		impure->irsb_flags |= irsb_key_changed;
	}

	// Find the next interesting node.  If necessary, skip to the next page
	bool page_changed = false;
	RecordNumber number;
	Ods::IndexNode node;
	while (true) {
		Ods::btree_page* page = (Ods::btree_page*) window.win_buffer;
		const SCHAR flags = page->btr_header.pag_flags;

		UCHAR* pointer = nextPointer;
		btree_exp* expanded_node = expanded_next;
		if (pointer) {
			BTreeNode::readNode(&node, pointer, flags, true);
			number = node.recordNumber;
		}

#ifdef SCROLLABLE_CURSORS
		// in the backwards case, check to make sure we haven't hit the 
		// beginning of a page, and if so fetch the left sibling page.
		if (direction == RSE_get_backward) {
			if (pointer < BTreeNode::getPointerFirstNode(page)) {
				exp_index_buf* expanded_page = window.win_expanded_buffer;

				if (!page->btr_left_sibling) {
					impure->irsb_flags |= irsb_bof;
					break;
				}

				page = BTR_left_handoff(tdbb, &window, page, LCK_read);

#ifdef PC_ENGINE
				RNG_add_page(window.win_page);
#endif
				expanded_page = NAV_expand_index(&window, 0);
				nextPointer = BTR_last_node(page, expanded_page, &expanded_next);

				page_changed = true;
				continue;
			}
		}
		else
			// In the forwards case, check for end of page.  If we find
			// it, do a simple handoff to the right sibling page.
#endif
		{
			if (node.isEndLevel) {
				impure->irsb_flags |= irsb_eof;
				break;
			}
			if (node.isEndBucket) {
				page = (Ods::btree_page*) window.win_buffer;
				page = (Ods::btree_page*) CCH_HANDOFF(tdbb, &window, page->btr_sibling,
					LCK_read, pag_index);
#ifdef PC_ENGINE
				RNG_add_page(window.win_page);
#endif
				nextPointer = BTreeNode::getPointerFirstNode(page);
				exp_index_buf* expanded_page = window.win_expanded_buffer;
				if (expanded_page) {
					expanded_next = (btree_exp*) expanded_page->exp_nodes;
				}

				page_changed = true;
				continue;
			}

		}

		// In the project (DISTINCT) case, we need to determine if the key has changed
		// since the last record that was returned from the rsb.  That means that we must 
		// know what the key was for that record


		// in the case where we are on the same page, 
		// we simply check to see if there is any data in the node;  if we are on a new 
		// page, we must compare this node with the last key we saw 
		// NOTE: the optimizer only uses indices which have the same number of fields as 
		// those in the DISTINCT clause; there's no reason we couldn't use an index which 
		// has extra fields to the right, we just need to add some code to check 
		// when only the first n segment(s) of the key has changed, rather than the whole key
		if (rsb->rsb_flags & rsb_project) {
			if (page_changed) {
				if (node.length != key.key_length) {
					impure->irsb_flags |= irsb_key_changed;
				}
				else {
					UCHAR* p = key.key_data;
					const UCHAR* q = node.data;
					USHORT l = key.key_length;
					for (; l; l--) {
						if (*p++ != *q++) {
							break;
						}
					}
					if (l) {
						impure->irsb_flags |= irsb_key_changed;
					}
				}
			}
			else if (node.length) {
				impure->irsb_flags |= irsb_key_changed;
			}
		}
		page_changed = false;

		// Build the current key value from the prefix and current node data.
		if (expanded_node) {
			USHORT l = node.length + node.prefix;
			if (l) {
				UCHAR* p = key.key_data;
				const UCHAR* q = expanded_node->btx_data;
				do {
					*p++ = *q++;
				} while (--l);
			}

		}
		else {
			USHORT l = node.length;
			if (l) {
				UCHAR* p = key.key_data + node.prefix;
				const UCHAR* q = node.data;
				do {
					*p++ = *q++;
				} while (--l);
			}
		}
		key.key_length = node.length + node.prefix;

		// Make sure we haven't hit the upper (or lower) limit.
		if ((direction == RSE_get_forward) && retrieval->irb_upper_count &&
			compare_keys(idx, key.key_data, key.key_length, &upper,
						 retrieval->irb_generic & (irb_descending |
												   irb_partial |
												   irb_starting)) > 0) 
		{
			RSE_MARK_CRACK(tdbb, rsb, irsb_crack);
			break;
		}

		if ((direction == RSE_get_backward) && retrieval->irb_lower_count &&
			compare_keys(idx, key.key_data, key.key_length, &lower,
						 retrieval->irb_generic & (irb_descending |
												   irb_partial |
												   irb_starting)) < 0) 
		{
			RSE_MARK_CRACK(tdbb, rsb, irsb_crack);
			break;
		}

		// skip this record if:
		// 1) there is an inversion tree for this index and this record
		//    is not in the bitmap for the inversion, or
		// 2) the record has already been visited, or
		// 3) this is a projection and the new node is the same as the last

		if ((rsb->rsb_arg[RSB_NAV_inversion] &&
			 (!impure->irsb_nav_bitmap
			  || !RecordBitmap::test(*impure->irsb_nav_bitmap, number.getValue())))
			|| RecordBitmap::test(impure->irsb_nav_records_visited, number.getValue())
			|| ((rsb->rsb_flags & rsb_project)
				&& !(impure->irsb_flags & irsb_key_changed))) 
		{
			if (direction == RSE_get_backward) {
				nextPointer = BTreeNode::previousNode(&node, pointer, flags, &expanded_node);
				expanded_next = expanded_node;
				continue;
			}
			else if (direction == RSE_get_forward
					 && !(impure->irsb_flags & irsb_forced_crack)) 
			{
				nextPointer = BTreeNode::nextNode(&node, pointer, flags, &expanded_node);
				expanded_next = expanded_node;
				continue;
			}
		}

		// reset the current navigational position in the index
		rpb->rpb_number = number;
#ifdef PC_ENGINE
		if (direction != RSE_get_current)
#endif
			set_position(impure, rpb, &window, pointer, expanded_node,
						 key.key_data, key.key_length);

		CCH_RELEASE(tdbb, &window);

		if (get_record(rsb, impure, rpb, &key, false)) {
#ifdef PC_ENGINE
			if (impure->irsb_flags & irsb_refresh)
				RNG_add_record(rpb);
#endif
			return true;
		}

#ifdef PC_ENGINE
		// if we're here, we didn't find the record at the current position, 
		// so it must have been deleted out from under us; by definition we
		// are on a crack (already marked by get_record())
		if (direction == RSE_get_current) {
			return false;
		}
#endif

		nextPointer = get_position(tdbb, rsb, impure, &window, direction, &expanded_next);
	}

	CCH_RELEASE(tdbb, &window);

	// crack, bof, or eof must have been set at this point
	return false;
}


#ifdef PC_ENGINE
bool NAV_reset_position(RecordSource* rsb, record_param* new_rpb)
{
/**************************************
 *
 *	N A V _ r e s e t _ p o s i t i o n
 *
 **************************************
 *
 * Functional description
 *	Reset the stream represented by the passed 
 *	rsb to the record indicated by the passed rpb.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	jrd_req* request = tdbb->tdbb_request;
	irsb_nav* impure = (IRSB_NAV) ((UCHAR *) request + rsb->rsb_impure);
	WIN window(-1);

	btree_nod* expanded_node;
	init_fetch(impure);
	if (!impure->irsb_nav_page) {
		nav_open(tdbb, rsb, impure, &window, RSE_get_current, &expanded_node);
		CCH_RELEASE(tdbb, &window);
	}

	// resetting the stream invalidates the visited records
	SBM_reset(&impure->irsb_nav_records_visited);

	index_desc* idx = (index_desc*) ((SCHAR*) impure + (SLONG) rsb->rsb_arg[RSB_NAV_idx_offset]);

	// save the record number, in case the passed new_rpb is 
	// the same as the one on the rpb, in which case it will 
	// be updated by find_record()--bug #7426

	const RecordNumber record_number = new_rpb->rpb_number;

	// find the key value of the new position, and set the stream to it
	temporary_key key_value;
	BTR_key(tdbb, new_rpb->rpb_relation, new_rpb->rpb_record, idx,
			&key_value, 0);
	if (!find_record(rsb, RSE_get_first, &key_value, idx->idx_count, 0)) // XXX
		return false;

	// now find the dbkey of the new record within the 
	// duplicates of this key value
	return find_dbkey(rsb, record_number);
}
#endif


#ifdef PC_ENGINE
bool NAV_set_bookmark(RecordSource* rsb, IRSB_NAV impure, record_param* rpb, Bookmark* bookmark)
{
/**************************************
 *
 *	N A V _ s e t _ b o o k m a r k
 *
 **************************************
 *
 * Functional description
 *	Set up the impure area so that the current 
 *	position of the stream is that of the 
 *	stored bookmark.
 *
 **************************************/

	// setting to bookmark invalidates the visited records
	SBM_reset(&impure->irsb_nav_records_visited);

	// save the bookmark state in the impure area for the stream
	impure->irsb_nav_number = bookmark->bkm_number;
	impure->irsb_nav_incarnation = bookmark->bkm_incarnation;
	impure->irsb_nav_offset = bookmark->bkm_offset;
	impure->irsb_nav_expanded_offset = bookmark->bkm_expanded_offset;

	// assuming we had already fetched a page when 
	// we set the bookmark, make sure the "first" flag
	// is not set so that we don't reopen the stream--bug 5709
	if (impure->irsb_nav_page = bookmark->bkm_page) {
		impure->irsb_flags &= ~irsb_first;
	}

	// store the current key value, setting up the key descriptor to point at it
	impure->irsb_nav_length = bookmark->bkm_key_desc.dsc_length;
	MOVE_FAST(bookmark->bkm_key_data, impure->irsb_nav_data,
			  bookmark->bkm_key_desc.dsc_length);

	if (impure->irsb_flags & (irsb_bof | irsb_eof | irsb_crack)) {
		return false;
	}

	// if we didn't find the record, it must have been deleted 
	// from under us; the desired semantics is to be on a crack 
	// at the previous position of the record
	return NAV_get_record(rsb, impure, rpb, RSE_get_current);
}
#endif


static int compare_keys(
						   const index_desc* idx,
						   const UCHAR* key_string1,
						   USHORT length1, const temporary_key* key2, USHORT flags)
{
/**************************************
 *      
 *	c o m p a r e _ k e y s
 *
 **************************************
 *
 * Functional description
 *	Compare two index keys. 
 *	If a partial key comparison is specified, 
 *	ensure that the shorter key (the second 
 *	one) matches segment-by-segment with the 
 *	index key.
 *
 **************************************/
	const UCHAR* string1 = key_string1;
	const UCHAR* string2 = key2->key_data;
	const USHORT length2 = key2->key_length;

	USHORT l = MIN(length1, length2);
	if (l) {
		do {
			if (*string1++ != *string2++) {
				return (string1[-1] < string2[-1]) ? -1 : 1;
			}
		} while (--l);
	}

	// if the keys are identical, return 0
	if (length1 == length2) {
		return 0;
	}

	// do a partial key search; if the index is a compound key,
	// check to see if the segments are matching--for character
	// fields, do partial matches within a segment if irb_starting
	// is specified, for other types do only matches on the entire segment

	if ((flags & (irb_partial | irb_starting)) && (length1 > length2)) {
		// figure out what segment we're on; if it's a 
		// character segment we've matched the partial string
		const UCHAR* segment = 0;
		const index_desc::idx_repeat* tail;
		if (idx->idx_count > 1) {
			segment = key_string1 +
				((length2 - 1) / (Ods::STUFF_COUNT + 1)) * (Ods::STUFF_COUNT + 1);
			tail = idx->idx_rpt + (idx->idx_count - *segment);
		}
		else {
			tail = &idx->idx_rpt[0];
		}

		// If it's a string type key, and we're allowing "starting with" fuzzy
		// type matching, we're done
		if ((flags & irb_starting) &&
			(tail->idx_itype == idx_string ||
			 tail->idx_itype == idx_byte_array ||
			 tail->idx_itype == idx_metadata ||
			 tail->idx_itype >= idx_first_intl_string)) 
		{
			return 0;
		}

		if (idx->idx_count > 1) {

			// If we search for NULLs at the begin then we're done if the first 
			// segment isn't the first one possible (0 for ASC, 255 for DESC).
			if (length2 == 0) {
				if (flags & irb_descending) {
					if (*segment != 255) {
						return 0;
					}
				}
				else {
					if (*segment != 0) {
						return 0;
					}
				}
			}

			// if we've exhausted the segment, we've found a match
			USHORT remainder = length2 % (Ods::STUFF_COUNT + 1);
			if (!remainder && (*string1 != *segment)) 
			{
				return 0;
			}

			// if the rest of the key segment is 0's, we've found a match
			if (remainder) {
				for (remainder = Ods::STUFF_COUNT + 1 - remainder; remainder;
					remainder--) 
				{
					if (*string1++) {
						break;
					}
				}
				if (!remainder) {
					return 0;
				}
			}
		}
	}

	if (flags & irb_descending) {
		return (length1 < length2) ? 1 : -1;
	}
	else {
		return (length1 < length2) ? -1 : 1;
	}
}


#ifdef SCROLLABLE_CURSORS
static void expand_index(WIN * window)
{
/**************************************
 *
 *	e x p a n d _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Given a window with a btree leaf page loaded into it, 
 *	expand the index page into a form that facilitates walking
 *	backward through it.  Each node's data is decompressed.  
 *	Since the prefix field is not needed, it contains an offset to 
 *	the prior node.
 *
 **************************************/
	btree_page* page = (btree_page*) window->win_buffer;
	exp_index_buf* expanded_page = window->win_expanded_buffer;
	expanded_page->exp_incarnation = CCH_get_incarnation(window);

	// go through the nodes on the original page and expand them
	temporary_key key;
	btree_exp* expanded_node = (btree_exp*) expanded_page->exp_nodes;
	bool priorPointer = false;
	UCHAR* pointer = BTreeNode::getPointerFirstNode(page);
	const UCHAR* const endPointer = ((UCHAR*) page + page->btr_length);

	const SCHAR flags = page->pag_flags;
	IndexNode node, priorNode;

	while (pointer < endPointer) {

		if (!priorPointer) {
			pointer = BTreeNode::readNode(&node, pointer, flags);
		}

		UCHAR* p = key.key_data + node.prefix;
		const UCHAR* q = node.data;
		USHORT l;
		for (l = node.length; l; l--) {
			*p++ = *q++;
		}		
		p = expanded_node->btx_data;
		q = key.key_data;
		for (l = node.length + node.prefix; l; l--) {
			*p++ = *q++;
		}

		// point back to the prior nodes on the expanded page and the btree page
		expanded_node->btx_btr_previous_length =
			priorPointer ? BTreeNode::getLeafNodeSize(&priorNode, flags) : 0;
		expanded_node->btx_previous_length =
			priorPointer ? priorNode.length + priorNode.prefix : 0;

		priorPointer = true;
		priorNode = node;
		pointer = BTreeNode::nextNode(&node, pointer, flags, &expanded_node);
	}

	// insert one additional expanded node to keep track of the length of the
	// last node on page--this is necessary because the end of bucket marker
	// contains the length and key of the first node on the next page
	expanded_node->btx_btr_previous_length =
		priorPointer ? BTreeNode::getLeafNodeSize(&priorNode, flags) : 0;
	expanded_node->btx_previous_length =
		priorPointer ? priorNode.length + priorNode.prefix : 0;

	expanded_page->exp_length =
		(UCHAR*) expanded_node - (UCHAR*) expanded_page + BTX_SIZE;
}
#endif


#ifdef PC_ENGINE
static bool find_dbkey(RecordSource* rsb, RecordNumber record_number)
{
/**************************************
 *
 *	f i n d _ d b k e y
 *
 **************************************
 *
 * Functional description
 *	Look for the record with the passed dbkey in the
 *	set of records with key value equal to the current 
 *	record's key.  This is used to find a particular
 *	record within a set of equivalent keys.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	jrd_req* request = tdbb->tdbb_request;
	irsb_nav* impure = (IRSB_NAV) ((UCHAR *) request + rsb->rsb_impure);
	record_param* rpb = request->req_rpb + rsb->rsb_stream;

	init_fetch(impure);

	// if we're on a crack, this isn't going very far
	if (impure->irsb_flags & (irsb_bof | irsb_eof | irsb_crack)) {
		return false;
	}

	// find the last fetched position from the index,
	// and get the current key value
	btree_exp* expanded_node = NULL;
	WIN window(impure->irsb_nav_page);
	btree_nod* node =
		get_position(tdbb, rsb, impure, &window, RSE_get_current,
					 &expanded_node);
	temporary_key key;
	MOVE_FAST(impure->irsb_nav_data, key.key_data, impure->irsb_nav_length);
	key.key_length = impure->irsb_nav_length;

	// In case of an error, we still need to release the window we hold
	// during the parse.  See HACKs for bug 7041

	try {

	// loop through the equivalent values of the given key, looking for a
	// record which matches the passed dbkey 
	for (;;) {
		rpb->rpb_number = get_long(node->btn_number);

		// if we find an index entry with the proper dbkey, try to fetch the record 
		if (rpb->rpb_number == record_number) {
			if (get_record(rsb, impure, rpb, &key, true)) {
				set_position(impure, rpb, &window, node, expanded_node,
							 key.key_data, key.key_length);
				CCH_RELEASE(tdbb, &window);
				return true;
			}

			CCH_RELEASE(tdbb, &window);
			return false;
		}

		// go to the next node; if we find a non-equivalent node, give up
		node = BTR_next_node(node, &expanded_node);
		if (node->btn_length) {
			CCH_RELEASE(tdbb, &window);
			return false;
		}
	}

	}	// try
	catch (const std::exception&) {
		CCH_RELEASE(tdbb, &window);
		throw;
	}
}
#endif


#ifdef PC_ENGINE
static bool find_record(
						   RecordSource* rsb,
						   RSE_GET_MODE mode,
						   temporary_key* find_key,
						   USHORT find_count, USHORT search_flags)
{
/**************************************
 *
 *	f i n d _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Find a record with the specified key value
 *	in a stream opened on a navigational index.
 *	Depending on the mode, get the first or 
 *	last value with the given key.  This routine
 *	must handle CRACK semantics.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	jrd_req* request = tdbb->tdbb_request;
	irsb_nav* impure = (IRSB_NAV) ((UCHAR *) request + rsb->rsb_impure);
	record_param* rpb = request->req_rpb + rsb->rsb_stream;
	WIN window(-1);

	jrd_nod* retrieval_node = (jrd_nod*) rsb->rsb_arg[RSB_NAV_index];
	IndexRetrieval* retrieval = 
		(IndexRetrieval*) retrieval_node->nod_arg[e_idx_retrieval];

	// save the current equality retrieval key
	temporary_key* const tmp = retrieval->irb_key;
	const USHORT lower_count = retrieval->irb_lower_count;
	const USHORT upper_count = retrieval->irb_upper_count;

	// find the page that the key value should be on
	retrieval->irb_key = find_key;
	retrieval->irb_upper_count = retrieval->irb_lower_count = find_count;

	index_desc* idx =
		(index_desc*) ((SCHAR *) impure + (SLONG) rsb->rsb_arg[RSB_NAV_idx_offset]);
	temporary_key lower, upper;
	btree_page* page =
		BTR_find_page(tdbb, retrieval, &window, idx, &lower, &upper, false);

	// restore the saved equality retrieval key
	retrieval->irb_key = tmp;
	retrieval->irb_lower_count = lower_count;
	retrieval->irb_upper_count = upper_count;

	// find the appropriate leaf node
	btree_nod* node;
	while (!(node = BTR_find_leaf(page, find_key, impure->irsb_nav_data,
								  0, (idx->idx_flags & idx_descending) != 0, true)))
	{
		page =
			(btree_page*) CCH_HANDOFF(tdbb, &window, page->btr_sibling, LCK_read,
							  pag_index);
	}

	btree_exp* expanded_node;
	exp_index_buf* expanded_page = window.win_expanded_buffer;
	if (expanded_page) {
		expanded_node = find_current(expanded_page, page, node);
	}
	else {
		expanded_node = NULL;
	}

	// seed the key value with the prefix seen up to the current key 
	temporary_key value;
	MOVE_FAST(impure->irsb_nav_data, value.key_data, node->btn_prefix);

	// In case of an error, we still need to release the window we hold
	// during the parse.  See HACKs for bug 7041

	try {
		bool result = false;
		bool position_set = false;

	// loop through the equivalent values of the given key, finding
	// a valid record if possible; for RSE_get_last, save the last
	// valid record and keep looking for more

	// NOTE: this whole process could be made more efficient if we 
	// implemented a means of finding the last index key via 
	// BTR_find_page() and BTR_find_leaf(), then proceeding backwards
	// to find a valid record -deej

	for (;;)
	{
		rpb->rpb_number = get_long(node->btn_number);

		// if we have gone past the search key value, stop looking
		if (rpb->rpb_number == END_LEVEL) {
			CCH_RELEASE(tdbb, &window);
			RSE_MARK_CRACK(tdbb, rsb, irsb_eof);
			return false;
		}

		if (rpb->rpb_number == END_BUCKET) {
			page =
				(btree_page*) CCH_HANDOFF(tdbb, &window, page->btr_sibling, LCK_read,
								  pag_index);
			node = (btree_nod*) page->btr_nodes;

			if (expanded_page = window.win_expanded_buffer)
				expanded_node = (btree_exp*) expanded_page->exp_nodes;
			continue;
		}

		// update the current stored key value
		value.key_length = node->btn_length + node->btn_prefix;
		UCHAR* p = value.key_data + node->btn_prefix;
		const UCHAR* q = node->btn_data;
		for (USHORT l = node->btn_length; l--;)
			*p++ = *q++;

		// if the index key is greater than the search key, we didn't find the key 
		if (compare_keys
			(idx, value.key_data, value.key_length, find_key,
			 search_flags) > 0)
		{
			// if we never saw a valid record, mark as a forced crack so that
			// we will be at a position before the current record

			if (!position_set) {
				set_position(impure, rpb, &window, node, expanded_node,
							 value.key_data, value.key_length);
				RSE_MARK_CRACK(tdbb, rsb, irsb_crack | irsb_forced_crack);
			}

			CCH_RELEASE(tdbb, &window);
			return result;
		}

		// anytime we successfully retrieve a record, set position 
		// to it in case we go past a viable record in looking for
		// the last record of equivalent key
		if (result = get_record(rsb, impure, rpb, &value, true)) {
			set_position(impure, rpb, &window, node, expanded_node,
						 value.key_data, value.key_length);
			position_set = true;
		}

		// if we're looking for the first record, we're done
		if (result && mode == RSE_get_first) {
			CCH_RELEASE(tdbb, &window);
			return result;
		}

		node = BTR_next_node(node, &expanded_node);
	}

	}	// try
	catch (const std::exception&) {
		CCH_RELEASE(tdbb, &window);
		throw;
	}
}
#endif


static btree_exp* find_current(exp_index_buf* expanded_page, Ods::btree_page* page,
	const UCHAR* current_pointer)
{
/**************************************
 *
 *	f i n d _ c u r r e n t
 *
 **************************************
 *
 * Functional description
 *	Find the current location in the expanded
 *	index buffer according to its corresponding
 *	btree page.
 *
 **************************************/
	if (!expanded_page) {
		return NULL;
	}

	btree_exp* expanded_node = expanded_page->exp_nodes;
	const SCHAR flags = page->btr_header.pag_flags;
	UCHAR* pointer = BTreeNode::getPointerFirstNode(page);
	const UCHAR* const endPointer = ((UCHAR*) page + page->btr_length);
	Ods::IndexNode node;
	while (pointer < endPointer) {
		if (pointer == current_pointer) {
			return expanded_node;
		}

		// AB: Together this looks pretty the same as BTreeNode::nextNode
		pointer = BTreeNode::readNode(&node, pointer, flags, true);
		expanded_node = (btree_exp*) ((UCHAR*) expanded_node->btx_data +
			node.prefix + node.length);
	}

	return NULL;
}


static bool find_saved_node(RecordSource* rsb, IRSB_NAV impure,
						WIN * window, UCHAR ** return_pointer)
{
/**************************************
 *
 *	f i n d _ s a v e d _ n o d e
 *
 **************************************
 *
 * Functional description
 *	A page has changed.  Find the new position of the 
 *	previously stored node.  The node could even be on 
 *	a sibling page if the page has split.  If we find
 *	the actual node, return TRUE.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	index_desc* idx = (index_desc*) ((SCHAR*) impure + (IPTR) rsb->rsb_arg[RSB_NAV_idx_offset]);
	Ods::btree_page* page = (Ods::btree_page*) CCH_FETCH(tdbb, window, LCK_read, pag_index);

	// the outer loop goes through all the sibling pages
	// looking for the node (in case the page has split);
	// the inner loop goes through the nodes on each page
	temporary_key key;
	const SCHAR flags = page->btr_header.pag_flags;
	Ods::IndexNode node;
	while (true) {
		UCHAR* pointer = BTreeNode::getPointerFirstNode(page);
		const UCHAR* const endPointer = ((UCHAR*) page + page->btr_length);
		while (pointer < endPointer) {

			pointer = BTreeNode::readNode(&node, pointer, flags, true);

			if (node.isEndLevel) {
				*return_pointer = node.nodePointer;
				return false;
			}
			if (node.isEndBucket) {
				page = (Ods::btree_page*) CCH_HANDOFF(tdbb, window, page->btr_sibling,
					LCK_read, pag_index);
				break;
			}

			// maintain the running key value and compare it with the stored value
			USHORT l = node.length;
			if (l) {
				UCHAR* p = key.key_data + node.prefix;
				const UCHAR* q = node.data;
				do {
					*p++ = *q++;
				} while (--l);
			}
			key.key_length = node.length + node.prefix;
			const int result = compare_keys(idx, impure->irsb_nav_data,
						impure->irsb_nav_length, &key, 0);

			// if the keys are equal, return this node even if it is just a duplicate node;
			// duplicate nodes that have already been returned will be filtered out at a 
			// higher level by checking the sparse bit map of records already returned, and
			// duplicate nodes that were inserted before the stored node can be
			// safely returned since their position in the index is arbitrary
			if (!result) {
				*return_pointer = node.nodePointer;
				if (node.recordNumber == impure->irsb_nav_number) {
					return true;
				}
				else {
					return false;
				}
			}

			// if the stored key is less than the current key, then the stored key
			// has been deleted from the index and we should just return the next 
			// key after it
			if (result < 0) {
				*return_pointer = node.nodePointer;
				return false;
			}

		}
	}
}


static UCHAR* get_position(
						thread_db* tdbb,
						RecordSource* rsb,
						IRSB_NAV impure,
						WIN * window,
						RSE_GET_MODE direction, btree_exp** expanded_node)
{
/**************************************
 *
 *	g e t _ p o s i t i o n
 *
 **************************************
 *
 * Functional description
 *	Re-fetch B-tree page and reposition the node pointer to where
 *	we left off.  If the page has been sitting in cache unchanged
 *	since we were there last, this is easy and cheap.  If there's
 *	any chance the page has changed, we need to resynch.
 *
 **************************************/
	SET_TDBB(tdbb);

	// If this is the first time, start at the beginning (or the end)
	if (!window->win_page || impure->irsb_flags & (irsb_bof | irsb_eof)) {
		return nav_open(tdbb, rsb, impure, window, direction, expanded_node);
	}

	exp_index_buf* expanded_page = NULL;

#ifdef PC_ENGINE
	// if we are on a forced crack, don't really get the next node in 
	// the forward direction, since we are really supposed to be on a
	// position before the current node
	if ((impure->irsb_flags & irsb_forced_crack)
		&& (direction == RSE_get_forward))
	{
		direction = RSE_get_current;
	}
#endif

	// Re-fetch page and get incarnation counter
	Ods::btree_page* page = (Ods::btree_page*) CCH_FETCH(tdbb, window, LCK_read, pag_index);

#ifdef SCROLLABLE_CURSORS
	// we must ensure that if we are going backwards, we always 
	// have an expanded buffer (and a valid position on that page)
	if (direction == RSE_get_backward) {
		NAV_expand_index(window, impure);
	}
	expanded_page = window->win_expanded_buffer;
#endif

	UCHAR* pointer = 0;
	const SCHAR flags = page->btr_header.pag_flags;
	const SLONG incarnation = CCH_get_incarnation(window);
	Ods::IndexNode node;
	if (incarnation == impure->irsb_nav_incarnation) {
		pointer = ((UCHAR*) page + impure->irsb_nav_offset);
		if (!expanded_page) {		
			// theoretically impossible
			*expanded_node = NULL;
		}
		else if (impure->irsb_nav_expanded_offset < 0) {
			// position unknown or invalid 
			*expanded_node = find_current(expanded_page, page, pointer);
		}
		else {
			*expanded_node = (btree_exp*) ((UCHAR*) expanded_page +
				impure->irsb_nav_expanded_offset);
		}

		// The new way of doing things is to have the current
		// nav_offset be the last node fetched.  Go forward or
		// backward from that point accordingly.
		if (direction == RSE_get_backward) {
			pointer = BTreeNode::previousNode(&node, pointer, flags, expanded_node);
			//node = (btree_nod*) BTR_previous_node( (UCHAR*)node, expanded_node);
		}
		else if (direction == RSE_get_forward) {
			pointer = BTreeNode::nextNode(&node, pointer, flags, expanded_node);
		}
		return pointer;
	}

	// Page is presumably changed.  If there is a previous
	// stored position in the page, go back to it if possible.
	CCH_RELEASE(tdbb, window);
	if (!impure->irsb_nav_page) {
		return nav_open(tdbb, rsb, impure, window, direction, expanded_node);
	}

	const bool found = find_saved_node(rsb, impure, window, &pointer);
	page = (Ods::btree_page*) window->win_buffer;
	if (pointer) {
		*expanded_node = find_current(window->win_expanded_buffer, page, pointer);
		if (direction == RSE_get_backward) {
			pointer = BTreeNode::previousNode(&node, pointer, flags, expanded_node);
			//node = (btree_nod*) BTR_previous_node((UCHAR*) node, expanded_node);
		}
		else if (direction == RSE_get_forward && found) {
			// in the forward case, seek to the next node only if we found
			// the actual node on page; if we did not find it, we are already
			// at the next node (such as when the node is garbage collected)
			pointer = BTreeNode::nextNode(&node, pointer, flags, expanded_node);
		}
		return pointer;
	}

#ifdef SCROLLABLE_CURSORS
	// As a last resort, return the first (or last if backwards) 
	// node on the page.  The sparse bitmap will prevent us from 
	// seeing records we have already visited before.
	if (direction == RSE_get_backward) {
		expanded_page = NAV_expand_index(window, 0);
		return BTR_last_node(page, expanded_page, expanded_node);
	}

	if (expanded_page = window->win_expanded_buffer) {
		*expanded_node = (btree_exp*) expanded_page->exp_nodes;
	}
#endif

	return BTreeNode::getPointerFirstNode(page);
}


static bool get_record(
						  RecordSource* rsb,
						  IRSB_NAV impure,
						  record_param* rpb, temporary_key* key, bool inhibit_cleanup)
{
/**************************************
 *
 *	g e t _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Get the record indicated by the passed rpb.
 *	This routine must set or clear the CRACK flag.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	jrd_req* request = tdbb->tdbb_request;
	index_desc* idx = (index_desc*) ((SCHAR*) impure + (IPTR) rsb->rsb_arg[RSB_NAV_idx_offset]);

	USHORT old_att_flags = 0;
	bool result = false;

	try {

	if (inhibit_cleanup) {
		// Inhibit garbage collection & other housekeeping - 
		// to prevent a deadlock when we visit the record.
		// There are cases when we are called here and have a window locked 
		// HACK for bug 7041

		old_att_flags = 
			static_cast<USHORT>(tdbb->tdbb_attachment->att_flags & ATT_no_cleanup);
		tdbb->tdbb_attachment->att_flags |= ATT_no_cleanup;	// HACK
	}

	// the attempt to get a record, whether successful or not, takes
	// us off bof or eof (otherwise we will keep trying to retrieve
	// the first record)

	impure->irsb_flags &= ~(irsb_bof | irsb_eof);

	result =
		VIO_get(tdbb,
				rpb,
				rsb,
				request->req_transaction,
				request->req_pool);

	temporary_key value;
	if (result)
	{
		BTR_key(tdbb, rpb->rpb_relation, rpb->rpb_record,
				reinterpret_cast<index_desc*>((SCHAR*) impure +
					(IPTR) rsb->rsb_arg[RSB_NAV_idx_offset]),
				&value,	0, false);
		if (compare_keys(idx, key->key_data, key->key_length, &value, 0)) {
			result = false;
		} 
		else {
			// the successful retrieval of a record
			// means we are no longer on a crack

			RSE_MARK_CRACK(tdbb, rsb, 0);

			// mark in the navigational bitmap that we have visited this record
			RBM_SET(tdbb->getDefaultPool(), &impure->irsb_nav_records_visited, rpb->rpb_number.getValue());
		}
	}

	if (!result) {
		RSE_MARK_CRACK(tdbb, rsb, irsb_crack);
	}

	if (inhibit_cleanup) {
		tdbb->tdbb_attachment->att_flags &= ~ATT_no_cleanup;
		tdbb->tdbb_attachment->att_flags |= (old_att_flags & ATT_no_cleanup);
	}

	}	// try
	catch (const std::exception&) {
		// Cleanup the HACK should there be an error
		tdbb->tdbb_attachment->att_flags &= ~ATT_no_cleanup;
		tdbb->tdbb_attachment->att_flags |=
			(old_att_flags & ATT_no_cleanup);
		throw;
	}

	return result;
}


static void init_fetch(IRSB_NAV impure)
{
/**************************************
 *
 *	i n i t _ f e t c h
 *
 **************************************
 *
 * Functional description
 *	Initialize for a fetch operation.
 *
 **************************************/
	if (impure->irsb_flags & irsb_first) {
		impure->irsb_flags &= ~irsb_first;
		impure->irsb_nav_page = 0;
	}
}


static UCHAR* nav_open(
					thread_db* tdbb,
					RecordSource* rsb,
					IRSB_NAV impure,
					WIN * window, RSE_GET_MODE direction, btree_exp** expanded_node)
{
/**************************************
 *
 *	n a v _ o p e n
 *
 **************************************
 *
 * Functional description
 *	Open a stream to walk an index.
 *
 **************************************/
	SET_TDBB(tdbb);

	// initialize for a retrieval
	setup_bitmaps(rsb, impure);
	impure->irsb_nav_page = 0;
	impure->irsb_nav_length = 0;

	if (direction == RSE_get_forward) {
		impure->irsb_flags |= irsb_bof;
	}
	else if (direction == RSE_get_backward) {
		impure->irsb_flags |= irsb_eof;
	}

	// Find the starting leaf page
	jrd_nod* retrieval_node = (jrd_nod*) rsb->rsb_arg[RSB_NAV_index];
	IndexRetrieval* retrieval =
		(IndexRetrieval*) retrieval_node->nod_arg[e_idx_retrieval];
	index_desc* idx = (index_desc*) ((SCHAR *) impure + (IPTR) rsb->rsb_arg[RSB_NAV_idx_offset]);
	temporary_key lower, upper;
	Ods::btree_page* page = BTR_find_page(tdbb, retrieval, window, idx, &lower,
		&upper, (direction == RSE_get_backward));
	impure->irsb_nav_page = window->win_page;


#ifdef SCROLLABLE_CURSORS
	// store the upper and lower bounds for the search in the impure area for the rsb
	if (retrieval->irb_lower_count) {
		impure->irsb_nav_lower_length = lower.key_length;
		MOVE_FAST(lower.key_data,
				  (impure->irsb_nav_data +
				   (SLONG) rsb->rsb_arg[RSB_NAV_key_length]),
				  lower.key_length);
	}

	if (retrieval->irb_upper_count) {
		impure->irsb_nav_upper_length = upper.key_length;
		MOVE_FAST(upper.key_data,
				  (impure->irsb_nav_data +
				   (2 * (SLONG) rsb->rsb_arg[RSB_NAV_key_length])),
				  upper.key_length);
	}

	// find the limit the search needs to begin with, if any
	temporary_key* limit_ptr = NULL;
	if (direction == RSE_get_forward) {
		if (retrieval->irb_lower_count)
			limit_ptr = &lower;
	}
	else {
		if (retrieval->irb_upper_count)
			limit_ptr = &upper;
	}
#else

	// find the upper limit for the search (or lower for backwards)
	temporary_key* limit_ptr = NULL;
	if (direction == RSE_get_forward) {
		if (retrieval->irb_upper_count) {
			impure->irsb_nav_upper_length = upper.key_length;
			MOVE_FAST(upper.key_data, (impure->irsb_nav_data +
				(IPTR) rsb->rsb_arg[RSB_NAV_key_length]),
				upper.key_length);
		}
		if (retrieval->irb_lower_count) {
			limit_ptr = &lower;
		}
	}
	else {
		if (retrieval->irb_lower_count) {
			impure->irsb_nav_lower_length = lower.key_length;
			MOVE_FAST(lower.key_data, (impure->irsb_nav_data +
				(IPTR) rsb->rsb_arg[RSB_NAV_key_length]),
				lower.key_length);
		}
		if (retrieval->irb_upper_count) {
			limit_ptr = &upper;
		}
	}
#endif

	// If there is a starting descriptor, search down index to starting position.
	// This may involve sibling buckets if splits are in progress.  If there 
	// isn't a starting descriptor, walk down the left side of the index (or the
	// right side if backwards).

	UCHAR* pointer = NULL;
	if (limit_ptr) {
		
		// If END_BUCKET is reached BTR_find_leaf will return NULL
		while (!(pointer = BTR_find_leaf(page, limit_ptr, impure->irsb_nav_data,
									  0, (idx->idx_flags & idx_descending) != 0, true)))
		{
			  page = (Ods::btree_page*) CCH_HANDOFF(tdbb, window, page->btr_sibling,
				  LCK_read, pag_index);
		}

		Ods::IndexNode node;
		BTreeNode::readNode(&node, pointer, page->btr_header.pag_flags, true);

		impure->irsb_nav_length = node.prefix + node.length;

#ifdef SCROLLABLE_CURSORS
		// ensure that if we are going backward, there is an expanded page
		if (direction == RSE_get_backward && !window->win_expanded_buffer)
			NAV_expand_index(window, 0);

		// now find our current location on the expanded page
		if (expanded_node && (expanded_page = window->win_expanded_buffer)) {
			*expanded_node = find_current(expanded_page, page, pointer);
		}
#endif
	}
#ifdef SCROLLABLE_CURSORS
	else if (direction == RSE_get_backward) {
		expanded_page = NAV_expand_index(window, 0);
		pointer = BTR_last_node(page, expanded_page, expanded_node);
	}
#endif
	else {

		pointer = BTreeNode::getPointerFirstNode(page);

#ifdef SCROLLABLE_CURSORS
		if (expanded_node && (expanded_page = window->win_expanded_buffer)) {
			*expanded_node = (btree_exp*) expanded_page->exp_nodes;
		}
		else {
			*expanded_node = NULL;
		}
#endif
	}

	return pointer;
}


static void set_position(IRSB_NAV impure, record_param* rpb, WIN* window,
				const UCHAR* pointer, btree_exp* expanded_node,
				const UCHAR* key_data, USHORT length)
{
/**************************************
 *
 *	s e t _ p o s i t i o n
 *
 **************************************
 *
 * Functional description
 *	Setup the information about the current position 
 *	in the navigational index.  Store all required data in the
 *	impure area for the request.
 *
 **************************************/

	// We can actually set position without having a data page
	// fetched; if not, just set the incarnation to the lowest possible
	impure->irsb_nav_incarnation = CCH_get_incarnation(window);
	impure->irsb_nav_page = window->win_page;
	impure->irsb_nav_number = rpb->rpb_number;

	// save the current key value if it hasn't already been saved 
	if (key_data) {
		impure->irsb_nav_length = length;
		MOVE_FAST(key_data, impure->irsb_nav_data, length);
	}

	// setup the offsets into the current index page and expanded index buffer
	impure->irsb_nav_offset = pointer - (UCHAR*) window->win_buffer;
	if (window->win_expanded_buffer) {
		impure->irsb_nav_expanded_offset =
			(UCHAR*) expanded_node - (UCHAR*) window->win_expanded_buffer;
	} 
	else {
		impure->irsb_nav_expanded_offset = -1;
	}
}


static void setup_bitmaps(RecordSource* rsb, IRSB_NAV impure)
{
/**************************************
 *
 *	s e t u p _ b i t m a p s
 *
 **************************************
 *
 * Functional description
 *	Setup the various bitmaps associated 
 *	with a stream.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	// Start a bitmap which tells us we have already visited
	// this record; this is to handle the case where there is more
	// than one leaf node reference to the same record number; the
	// bitmap allows us to filter out the multiple references.
	RecordBitmap::reset(impure->irsb_nav_records_visited);

	// the first time we open the stream, compute a bitmap
	// for the inversion tree--this may cause problems for
	// read-committed transactions since they will get one
	// view of the database when the stream is opened
	if (rsb->rsb_arg[RSB_NAV_inversion]) {
		// There is no need to reset or release the bitmap, it is
		// done in EVL_bitmap ().
		impure->irsb_nav_bitmap = EVL_bitmap(tdbb,
				reinterpret_cast<jrd_nod *>(rsb->rsb_arg[RSB_NAV_inversion]));
	}
}

