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
#include "../jrd/lck.h"
#include "../jrd/cch.h"
#include "../jrd/blr.h"
#include "../jrd/btr_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/dbg_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/nav_proto.h"
#include "../jrd/rse_proto.h"
#include "../jrd/vio_proto.h"

using namespace Jrd;

//#define MOVE_BYTE(x_from, x_to)	*x_to++ = *x_from++;

static int compare_keys(const index_desc*, const UCHAR*, USHORT,
						const temporary_key*, USHORT);

static btree_exp* find_current(exp_index_buf*, Ods::btree_page*, const UCHAR*);
static bool find_saved_node(thread_db* tdbb, RecordSource*, IRSB_NAV, WIN*, UCHAR**);
static UCHAR* get_position(thread_db*, RecordSource*, IRSB_NAV, WIN *, btree_exp**);
static bool get_record(thread_db* tdbb, RecordSource*, IRSB_NAV, record_param*, temporary_key*, bool);
static void init_fetch(IRSB_NAV);
static UCHAR* nav_open(thread_db* tdbb, RecordSource*, IRSB_NAV, WIN *);
static void set_page(IRSB_NAV impure, WIN* window);
static void set_position(IRSB_NAV, record_param*, WIN*, const UCHAR*, btree_exp*, const UCHAR*, USHORT);
static bool setup_bitmaps(thread_db* tdbb, RecordSource*, IRSB_NAV);


bool NAV_get_record(thread_db* tdbb, RecordSource* rsb, IRSB_NAV impure, record_param* rpb)
{
/**************************************
 *
 *	N A V _ g e t _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Get a record from a stream.
 *
 **************************************/
	SET_TDBB(tdbb);

	init_fetch(impure);
	index_desc* idx = (index_desc*) ((SCHAR*) impure + (IPTR) rsb->rsb_arg[RSB_NAV_idx_offset]);

	// find the last fetched position from the index
	const USHORT pageSpaceID = rpb->rpb_relation->getPages(tdbb)->rel_pg_space_id;
	WIN window(pageSpaceID, impure->irsb_nav_page);

	btree_exp* expanded_next = NULL;
	UCHAR* nextPointer = get_position(tdbb, rsb, impure, &window, &expanded_next);
	if (!nextPointer)
		return false;
	temporary_key key;
	memcpy(key.key_data, impure->irsb_nav_data, impure->irsb_nav_length);
	jrd_nod* retrieval_node = (jrd_nod*) rsb->rsb_arg[RSB_NAV_index];
	IndexRetrieval* retrieval = (IndexRetrieval*) retrieval_node->nod_arg[e_idx_retrieval];

	// set the upper (or lower) limit for navigational retrieval
	temporary_key upper;
	if (retrieval->irb_upper_count)
	{
		upper.key_length = impure->irsb_nav_upper_length;
		memcpy(upper.key_data,
				(impure->irsb_nav_data + (IPTR) rsb->rsb_arg[RSB_NAV_key_length]),
				upper.key_length);
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
	while (true)
	{
		Ods::btree_page* page = (Ods::btree_page*) window.win_buffer;
		const UCHAR flags = page->btr_header.pag_flags;

		UCHAR* pointer = nextPointer;
		btree_exp* expanded_node = expanded_next;
		if (pointer)
		{
			BTreeNode::readNode(&node, pointer, flags, true);
			number = node.recordNumber;
		}

		if (node.isEndLevel)
			break;

		if (node.isEndBucket)
		{
			page = (Ods::btree_page*) window.win_buffer;
			page = (Ods::btree_page*) CCH_HANDOFF(tdbb, &window, page->btr_sibling,
				LCK_read, pag_index);
			nextPointer = BTreeNode::getPointerFirstNode(page);
			exp_index_buf* expanded_page = window.win_expanded_buffer;
			if (expanded_page) {
				expanded_next = (btree_exp*) expanded_page->exp_nodes;
			}

			page_changed = true;
			continue;
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
		if (rsb->rsb_flags & rsb_project)
		{
			if (page_changed)
			{
				if (node.length != key.key_length || (key.key_length &&
					memcmp(key.key_data, node.data, key.key_length)))
				{
					impure->irsb_flags |= irsb_key_changed;
				}
			}
			else if (node.length) {
				impure->irsb_flags |= irsb_key_changed;
			}
		}
		page_changed = false;

		// Build the current key value from the prefix and current node data.
		if (expanded_node) {
			memcpy(key.key_data, expanded_node->btx_data, node.length + node.prefix);
		}
		else {
			memcpy(key.key_data + node.prefix, node.data, node.length);
		}
		key.key_length = node.length + node.prefix;

		// Make sure we haven't hit the upper (or lower) limit.
		if (retrieval->irb_upper_count &&
			compare_keys(idx, key.key_data, key.key_length, &upper,
						 retrieval->irb_generic & (irb_descending | irb_partial | irb_starting)) > 0)
		{
			break;
		}

		// skip this record if:
		// 1) there is an inversion tree for this index and this record
		//    is not in the bitmap for the inversion, or
		// 2) the record has already been visited, or
		// 3) this is a projection and the new node is the same as the last

		if ((rsb->rsb_arg[RSB_NAV_inversion] &&
			 (!impure->irsb_nav_bitmap ||
			 	!RecordBitmap::test(*impure->irsb_nav_bitmap, number.getValue()))) ||
			RecordBitmap::test(impure->irsb_nav_records_visited, number.getValue()) ||
			((rsb->rsb_flags & rsb_project) && !(impure->irsb_flags & irsb_key_changed)))
		{
			nextPointer = BTreeNode::nextNode(&node, pointer, flags, &expanded_node);
			expanded_next = expanded_node;
			continue;
		}

		// reset the current navigational position in the index
		rpb->rpb_number = number;
		set_position(impure, rpb, &window, pointer, expanded_node, key.key_data, key.key_length);

		CCH_RELEASE(tdbb, &window);

		if (get_record(tdbb, rsb, impure, rpb, &key, false)) {
			return true;
		}

		nextPointer = get_position(tdbb, rsb, impure, &window, &expanded_next);
		if (!nextPointer)
			return false;
	}

	CCH_RELEASE(tdbb, &window);

	// bof or eof must have been set at this point
	return false;
}


static int compare_keys(const index_desc* idx,
						const UCHAR* key_string1, USHORT length1, const temporary_key* key2,
						USHORT flags)
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
	if (l)
	{
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

	if ((flags & (irb_partial | irb_starting)) && (length1 > length2))
	{
		// figure out what segment we're on; if it's a
		// character segment we've matched the partial string
		const UCHAR* segment = 0;
		const index_desc::idx_repeat* tail;
		if (idx->idx_count > 1)
		{
			segment = key_string1 + ((length2 - 1) / (Ods::STUFF_COUNT + 1)) * (Ods::STUFF_COUNT + 1);
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

		if (idx->idx_count > 1)
		{

			// If we search for NULLs at the begin then we're done if the first
			// segment isn't the first one possible (0 for ASC, 255 for DESC).
			if (length2 == 0)
			{
				if (flags & irb_descending)
				{
					if (*segment != 255) {
						return 0;
					}
				}
				else
				{
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
			if (remainder)
			{
				for (remainder = Ods::STUFF_COUNT + 1 - remainder; remainder; remainder--)
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

	return (length1 < length2) ? -1 : 1;
}


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
	const UCHAR flags = page->btr_header.pag_flags;
	UCHAR* pointer = BTreeNode::getPointerFirstNode(page);
	const UCHAR* const endPointer = ((UCHAR*) page + page->btr_length);
	Ods::IndexNode node;
	while (pointer < endPointer)
	{
		if (pointer == current_pointer) {
			return expanded_node;
		}

		// AB: Together this looks pretty the same as BTreeNode::nextNode
		pointer = BTreeNode::readNode(&node, pointer, flags, true);
		expanded_node = (btree_exp*) ((UCHAR*) expanded_node->btx_data + node.prefix + node.length);
	}

	return NULL;
}


static bool find_saved_node(thread_db* tdbb, RecordSource* rsb, IRSB_NAV impure,
						WIN* window, UCHAR** return_pointer)
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
 *	the actual node, return true.
 *
 **************************************/
	SET_TDBB(tdbb);

	index_desc* idx = (index_desc*) ((SCHAR*) impure + (IPTR) rsb->rsb_arg[RSB_NAV_idx_offset]);
	Ods::btree_page* page = (Ods::btree_page*) CCH_FETCH(tdbb, window, LCK_read, pag_index);

	// the outer loop goes through all the sibling pages
	// looking for the node (in case the page has split);
	// the inner loop goes through the nodes on each page
	temporary_key key;
	const UCHAR flags = page->btr_header.pag_flags;
	Ods::IndexNode node;
	while (true)
	{
		UCHAR* pointer = BTreeNode::getPointerFirstNode(page);
		const UCHAR* const endPointer = ((UCHAR*) page + page->btr_length);
		while (pointer < endPointer)
		{

			pointer = BTreeNode::readNode(&node, pointer, flags, true);

			if (node.isEndLevel)
			{
				*return_pointer = node.nodePointer;
				return false;
			}
			if (node.isEndBucket)
			{
				page = (Ods::btree_page*) CCH_HANDOFF(tdbb, window, page->btr_sibling,
					LCK_read, pag_index);
				break;
			}

			// maintain the running key value and compare it with the stored value
			memcpy(key.key_data + node.prefix, node.data, node.length);
			key.key_length = node.length + node.prefix;
			const int result = compare_keys(idx, impure->irsb_nav_data,
											impure->irsb_nav_length, &key, 0);

			// if the keys are equal, return this node even if it is just a duplicate node;
			// duplicate nodes that have already been returned will be filtered out at a
			// higher level by checking the sparse bit map of records already returned, and
			// duplicate nodes that were inserted before the stored node can be
			// safely returned since their position in the index is arbitrary
			if (!result)
			{
				*return_pointer = node.nodePointer;
				return node.recordNumber == impure->irsb_nav_number;
			}

			// if the stored key is less than the current key, then the stored key
			// has been deleted from the index and we should just return the next
			// key after it
			if (result < 0)
			{
				*return_pointer = node.nodePointer;
				return false;
			}

		}
	}
}


static UCHAR* get_position(thread_db* tdbb,
						   RecordSource* rsb,
						   IRSB_NAV impure,
						   WIN* window,
						   btree_exp** expanded_node)
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

	// If this is the first time, start at the beginning
	if (!window->win_page.getPageNum())
	{
		return nav_open(tdbb, rsb, impure, window);
	}

	exp_index_buf* expanded_page = NULL;

	// Re-fetch page and get incarnation counter
	Ods::btree_page* page = (Ods::btree_page*) CCH_FETCH(tdbb, window, LCK_read, pag_index);

	UCHAR* pointer = 0;
	const UCHAR flags = page->btr_header.pag_flags;
	const SLONG incarnation = CCH_get_incarnation(window);
	Ods::IndexNode node;
	if (incarnation == impure->irsb_nav_incarnation)
	{
		pointer = ((UCHAR*) page + impure->irsb_nav_offset);
		if (!expanded_page)
		{
			// theoretically impossible
			*expanded_node = NULL;
		}
		else if (impure->irsb_nav_expanded_offset < 0)
		{
			// position unknown or invalid
			*expanded_node = find_current(expanded_page, page, pointer);
		}
		else
		{
			*expanded_node = (btree_exp*) ((UCHAR*) expanded_page +
				impure->irsb_nav_expanded_offset);
		}

		// The new way of doing things is to have the current
		// nav_offset be the last node fetched.
		return BTreeNode::nextNode(&node, pointer, flags, expanded_node);
	}

	// Page is presumably changed.  If there is a previous
	// stored position in the page, go back to it if possible.
	CCH_RELEASE(tdbb, window);
	if (!impure->irsb_nav_page) {
		return nav_open(tdbb, rsb, impure, window);
	}

	const bool found = find_saved_node(tdbb, rsb, impure, window, &pointer);
	page = (Ods::btree_page*) window->win_buffer;
	if (pointer)
	{
		*expanded_node = find_current(window->win_expanded_buffer, page, pointer);
		if (found)
		{
			// in the forward case, seek to the next node only if we found
			// the actual node on page; if we did not find it, we are already
			// at the next node (such as when the node is garbage collected)
			pointer = BTreeNode::nextNode(&node, pointer, flags, expanded_node);
		}
		return pointer;
	}

	return BTreeNode::getPointerFirstNode(page);
}


static bool get_record(thread_db* tdbb, RecordSource* rsb, IRSB_NAV impure,
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
 *
 **************************************/
	SET_TDBB(tdbb);
	jrd_req* request = tdbb->getRequest();
	index_desc* idx = (index_desc*) ((SCHAR*) impure + (IPTR) rsb->rsb_arg[RSB_NAV_idx_offset]);

	ULONG old_att_flags = 0;
	bool result = false;

	try {

		if (inhibit_cleanup)
		{
			// Inhibit garbage collection & other housekeeping -
			// to prevent a deadlock when we visit the record.
			// There are cases when we are called here and have a window locked
			// HACK for bug 7041

			old_att_flags = (tdbb->getAttachment()->att_flags & ATT_no_cleanup);
			tdbb->getAttachment()->att_flags |= ATT_no_cleanup;	// HACK
		}

		result = VIO_get(tdbb, rpb, request->req_transaction, request->req_pool);

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
			else
			{
				// mark in the navigational bitmap that we have visited this record
				RBM_SET(tdbb->getDefaultPool(), &impure->irsb_nav_records_visited,
					rpb->rpb_number.getValue());
			}
		}

		if (inhibit_cleanup)
		{
			tdbb->getAttachment()->att_flags &= ~ATT_no_cleanup;
			tdbb->getAttachment()->att_flags |= old_att_flags;
		}

	}	// try
	catch (const Firebird::Exception&)
	{
		// Cleanup the HACK should there be an error
		tdbb->getAttachment()->att_flags &= ~ATT_no_cleanup;
		tdbb->getAttachment()->att_flags |= old_att_flags;
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
	if (impure->irsb_flags & irsb_first)
	{
		impure->irsb_flags &= ~irsb_first;
		set_page(impure, NULL);
	}
}


static UCHAR* nav_open(thread_db* tdbb,
					   RecordSource* rsb,
					   IRSB_NAV impure,
					   WIN* window)
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
	if (!setup_bitmaps(tdbb, rsb, impure))
		return NULL;

	set_page(impure, NULL);
	impure->irsb_nav_length = 0;

	// Find the starting leaf page
	jrd_nod* retrieval_node = (jrd_nod*) rsb->rsb_arg[RSB_NAV_index];
	IndexRetrieval* retrieval = (IndexRetrieval*) retrieval_node->nod_arg[e_idx_retrieval];
	index_desc* idx = (index_desc*) ((SCHAR *) impure + (IPTR) rsb->rsb_arg[RSB_NAV_idx_offset]);
	temporary_key lower, upper;
	Ods::btree_page* page = BTR_find_page(tdbb, retrieval, window, idx, &lower, &upper);
	set_page(impure, window);

	// find the upper limit for the search
	temporary_key* limit_ptr = NULL;
	if (retrieval->irb_upper_count)
	{
		impure->irsb_nav_upper_length = upper.key_length;
		memcpy((impure->irsb_nav_data + (IPTR) rsb->rsb_arg[RSB_NAV_key_length]),
				upper.key_data,
				upper.key_length);
	}
	if (retrieval->irb_lower_count) {
		limit_ptr = &lower;
	}

	// If there is a starting descriptor, search down index to starting position.
	// This may involve sibling buckets if splits are in progress.  If there
	// isn't a starting descriptor, walk down the left side of the index.

	UCHAR* pointer = NULL;
	if (limit_ptr)
	{

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
	}
	else
	{
		pointer = BTreeNode::getPointerFirstNode(page);
	}

	return pointer;
}


static void set_page(IRSB_NAV impure, WIN* window)
{
/**************************************
 *
 *	s e t _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Take a LCK_btr_dont_gc lock on walked index page to prevent its
 *	removal from index while page lock is not taken
 *
 **************************************/
	const SLONG newPage = window ? window->win_page.getPageNum() : 0;

	if (impure->irsb_nav_page != newPage)
	{
		thread_db *tdbb = JRD_get_thread_data();

		if (impure->irsb_nav_page)
			impure->irsb_nav_btr_gc_lock->enablePageGC(tdbb);

		if (newPage)
		{
			if (!impure->irsb_nav_btr_gc_lock)
				impure->irsb_nav_btr_gc_lock = FB_NEW_RPT(*tdbb->getDefaultPool(), 0) BtrPageGCLock(tdbb);
			impure->irsb_nav_btr_gc_lock->disablePageGC(tdbb, window->win_page);
		}
		impure->irsb_nav_page = newPage;
	}
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
	set_page(impure, window);
	impure->irsb_nav_number = rpb->rpb_number;

	// save the current key value if it hasn't already been saved
	if (key_data)
	{
		impure->irsb_nav_length = length;
		memcpy(impure->irsb_nav_data, key_data, length);
	}

	// setup the offsets into the current index page and expanded index buffer
	impure->irsb_nav_offset = pointer - (UCHAR*) window->win_buffer;
	if (window->win_expanded_buffer)
	{
		impure->irsb_nav_expanded_offset =
			(UCHAR*) expanded_node - (UCHAR*) window->win_expanded_buffer;
	}
	else {
		impure->irsb_nav_expanded_offset = -1;
	}
}


static bool setup_bitmaps(thread_db* tdbb, RecordSource* rsb, IRSB_NAV impure)
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
	SET_TDBB(tdbb);

	// Start a bitmap which tells us we have already visited
	// this record; this is to handle the case where there is more
	// than one leaf node reference to the same record number; the
	// bitmap allows us to filter out the multiple references.
	RecordBitmap::reset(impure->irsb_nav_records_visited);

	// the first time we open the stream, compute a bitmap
	// for the inversion tree--this may cause problems for
	// read-committed transactions since they will get one
	// view of the database when the stream is opened
	if (rsb->rsb_arg[RSB_NAV_inversion])
	{
		// There is no need to reset or release the bitmap, it is
		// done in EVL_bitmap ().
		impure->irsb_nav_bitmap = EVL_bitmap(tdbb,
				reinterpret_cast<jrd_nod*>(rsb->rsb_arg[RSB_NAV_inversion]), NULL);
		return (*impure->irsb_nav_bitmap != NULL);
	}

	return true;
}

