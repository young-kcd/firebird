/*
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
#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../jrd/exe.h"
#include "../jrd/btr.h"
#include "../jrd/req.h"
#include "../jrd/rse.h"
#include "../jrd/btr_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/vio_proto.h"
#include "../jrd/rlck_proto.h"

#include "RecordSource.h"

using namespace Firebird;
using namespace Jrd;

// ------------------------------------
// Data access: index driven table scan
// ------------------------------------

IndexTableScan::IndexTableScan(CompilerScratch* csb, const string& name, UCHAR stream,
			jrd_nod* index, USHORT length)
	: RecordStream(csb, stream), m_name(csb->csb_pool, name), m_index(index), m_inversion(NULL),
	  m_length(length), m_offset(0)
{
	fb_assert(m_index);

	USHORT size = sizeof(Impure) + 2 * m_length;
	size = FB_ALIGN(size, FB_ALIGNMENT);
	m_offset = size;
	size += sizeof(index_desc);

	m_impure = CMP_impure(csb, size);
}

void IndexTableScan::open(thread_db* tdbb)
{
	Database* const dbb = tdbb->getDatabase();
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	impure->irsb_flags = irsb_first | irsb_open;

	record_param* const rpb = &request->req_rpb[m_stream];
	RLCK_reserve_relation(tdbb, request->req_transaction, rpb->rpb_relation, false);

	rpb->rpb_number.setValue(BOF_NUMBER);
}

void IndexTableScan::close(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();

	invalidateRecords(request);

	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	if (impure->irsb_flags & irsb_open)
	{
		impure->irsb_flags &= ~irsb_open;

		if (m_recursive)
		{
			if (impure->irsb_nav_bitmap)
			{
				delete *impure->irsb_nav_bitmap;
				*impure->irsb_nav_bitmap = NULL;
			}

			delete impure->irsb_nav_records_visited;
			impure->irsb_nav_records_visited = NULL;
		}

		if (impure->irsb_nav_page)
		{
			impure->irsb_nav_btr_gc_lock->enablePageGC(tdbb);
			impure->irsb_nav_page = 0;
		}
	}
}

bool IndexTableScan::getRecord(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();
	record_param* const rpb = &request->req_rpb[m_stream];
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	if (!(impure->irsb_flags & irsb_open))
	{
		rpb->rpb_number.setValid(false);
		return false;
	}

	if (impure->irsb_flags & irsb_first)
	{
		impure->irsb_flags &= ~irsb_first;
		setPage(tdbb, NULL);
	}

	index_desc* const idx = (index_desc*) ((SCHAR*) impure + m_offset);

	// find the last fetched position from the index
	const USHORT pageSpaceID = rpb->rpb_relation->getPages(tdbb)->rel_pg_space_id;
	win window(pageSpaceID, impure->irsb_nav_page);

	btree_exp* expanded_next = NULL;
	UCHAR* nextPointer = getPosition(tdbb, &window, &expanded_next);
	if (!nextPointer)
	{
		rpb->rpb_number.setValid(false);
		return false;
	}

	temporary_key key;
	memcpy(key.key_data, impure->irsb_nav_data, impure->irsb_nav_length);
	IndexRetrieval* const retrieval = (IndexRetrieval*) m_index->nod_arg[e_idx_retrieval];

	// set the upper (or lower) limit for navigational retrieval
	temporary_key upper;
	if (retrieval->irb_upper_count)
	{
		upper.key_length = impure->irsb_nav_upper_length;
		memcpy(upper.key_data, impure->irsb_nav_data + m_length, upper.key_length);
	}

	// Find the next interesting node. If necessary, skip to the next page.
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

			continue;
		}

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
			compareKeys(idx, key.key_data, key.key_length, &upper,
						 retrieval->irb_generic & (irb_descending | irb_partial | irb_starting)) > 0)
		{
			break;
		}

		// skip this record if:
		// 1) there is an inversion tree for this index and this record
		//    is not in the bitmap for the inversion, or
		// 2) the record has already been visited, or
		// 3) this is a projection and the new node is the same as the last

		if ((m_inversion &&
			 (!impure->irsb_nav_bitmap ||
			 	!RecordBitmap::test(*impure->irsb_nav_bitmap, number.getValue()))) ||
			RecordBitmap::test(impure->irsb_nav_records_visited, number.getValue()))
		{
			nextPointer = BTreeNode::nextNode(&node, pointer, flags, &expanded_node);
			expanded_next = expanded_node;
			continue;
		}

		// reset the current navigational position in the index
		rpb->rpb_number = number;
		setPosition(tdbb, rpb, &window, pointer, expanded_node, key.key_data, key.key_length);

		CCH_RELEASE(tdbb, &window);

		if (VIO_get(tdbb, rpb, request->req_transaction, request->req_pool))
		{
			temporary_key value;
			BTR_key(tdbb, rpb->rpb_relation, rpb->rpb_record, idx, &value,	0, false);

			if (!compareKeys(idx, key.key_data, key.key_length, &value, 0))
			{
				// mark in the navigational bitmap that we have visited this record
				RBM_SET(tdbb->getDefaultPool(), &impure->irsb_nav_records_visited,
						rpb->rpb_number.getValue());

				rpb->rpb_number.setValid(true);
				return true;
			}
		}

		nextPointer = getPosition(tdbb, &window, &expanded_next);
		if (!nextPointer)
		{
			rpb->rpb_number.setValid(false);
			return false;
		}
	}

	CCH_RELEASE(tdbb, &window);

	// bof or eof must have been set at this point
	rpb->rpb_number.setValid(false);
	return false;
}

void IndexTableScan::dump(thread_db* tdbb, UCharBuffer& buffer)
{
	buffer.add(isc_info_rsb_begin);

	buffer.add(isc_info_rsb_relation);
	dumpName(tdbb, m_name, buffer);

	buffer.add(isc_info_rsb_type);
	buffer.add(isc_info_rsb_navigate);
	dumpInversion(tdbb, m_index, buffer);

	if (m_inversion)
	{
		buffer.add(isc_info_rsb_type);
		buffer.add(isc_info_rsb_indexed);
		dumpInversion(tdbb, m_inversion, buffer);
	}

	buffer.add(isc_info_rsb_end);
}

int IndexTableScan::compareKeys(const index_desc* idx,
								  const UCHAR* key_string1,
								  USHORT length1,
								  const temporary_key* key2,
								  USHORT flags)
{
	const UCHAR* string1 = key_string1;
	const UCHAR* string2 = key2->key_data;
	const USHORT length2 = key2->key_length;

	USHORT l = MIN(length1, length2);
	if (l)
	{
		do
		{
			if (*string1++ != *string2++)
			{
				return (string1[-1] < string2[-1]) ? -1 : 1;
			}
		} while (--l);
	}

	// if the keys are identical, return 0
	if (length1 == length2)
	{
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
		const UCHAR* segment = NULL;
		const index_desc::idx_repeat* tail;
		if (idx->idx_count > 1)
		{
			segment = key_string1 + ((length2 - 1) / (Ods::STUFF_COUNT + 1)) * (Ods::STUFF_COUNT + 1);
			tail = idx->idx_rpt + (idx->idx_count - *segment);
		}
		else
		{
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
					if (*segment != 255)
					{
						return 0;
					}
				}
				else
				{
					if (*segment != 0)
					{
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
					if (*string1++)
					{
						break;
					}
				}
				if (!remainder)
				{
					return 0;
				}
			}
		}
	}

	if (flags & irb_descending)
	{
		return (length1 < length2) ? 1 : -1;
	}

	return (length1 < length2) ? -1 : 1;
}

btree_exp* IndexTableScan::findCurrent(exp_index_buf* expanded_page,
										 Ods::btree_page* page,
										 const UCHAR* current_pointer)
{
	if (expanded_page)
	{
		btree_exp* expanded_node = expanded_page->exp_nodes;
		const UCHAR flags = page->btr_header.pag_flags;
		UCHAR* pointer = BTreeNode::getPointerFirstNode(page);
		const UCHAR* const endPointer = (UCHAR*) page + page->btr_length;
		while (pointer < endPointer)
		{
			if (pointer == current_pointer)
			{
				return expanded_node;
			}

			Ods::IndexNode node;
			pointer = BTreeNode::nextNode(&node, pointer, flags, &expanded_node);
		}
	}

	return NULL;
}

bool IndexTableScan::findSavedNode(thread_db* tdbb, win* window, UCHAR** return_pointer)
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);
	index_desc* const idx = (index_desc*) ((SCHAR*) impure + m_offset);
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
			const int result =
				compareKeys(idx, impure->irsb_nav_data, impure->irsb_nav_length, &key, 0);

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

UCHAR* IndexTableScan::getPosition(thread_db* tdbb, win* window, btree_exp** expanded_node)
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	// If this is the first time, start at the beginning
	if (!window->win_page.getPageNum())
	{
		return openStream(tdbb, window);
	}

	exp_index_buf* expanded_page = NULL;

	// Re-fetch page and get incarnation counter
	Ods::btree_page* page = (Ods::btree_page*) CCH_FETCH(tdbb, window, LCK_read, pag_index);

	UCHAR* pointer = NULL;
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
			*expanded_node = findCurrent(expanded_page, page, pointer);
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
	if (!impure->irsb_nav_page)
	{
		return openStream(tdbb, window);
	}

	const bool found = findSavedNode(tdbb, window, &pointer);
	page = (Ods::btree_page*) window->win_buffer;
	if (pointer)
	{
		*expanded_node = findCurrent(window->win_expanded_buffer, page, pointer);

		// seek to the next node only if we found the actual node on page;
		// if we did not find it, we are already at the next node
		// (such as when the node is garbage collected)
		return found ? BTreeNode::nextNode(&node, pointer, flags, expanded_node) : pointer;
	}

	return BTreeNode::getPointerFirstNode(page);
}

UCHAR* IndexTableScan::openStream(thread_db* tdbb, win* window)
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	// initialize for a retrieval
	if (!setupBitmaps(tdbb))
	{
		return NULL;
	}

	setPage(tdbb, NULL);
	impure->irsb_nav_length = 0;

	// Find the starting leaf page
	IndexRetrieval* const retrieval = (IndexRetrieval*) m_index->nod_arg[e_idx_retrieval];
	index_desc* const idx = (index_desc*) ((SCHAR*) impure + m_offset);
	temporary_key lower, upper;
	Ods::btree_page* page = BTR_find_page(tdbb, retrieval, window, idx, &lower, &upper);
	setPage(tdbb, window);

	// find the upper limit for the search
	temporary_key* limit_ptr = NULL;
	if (retrieval->irb_upper_count)
	{
		impure->irsb_nav_upper_length = upper.key_length;
		memcpy(impure->irsb_nav_data + m_length, upper.key_data, upper.key_length);
	}

	if (retrieval->irb_lower_count)
	{
		limit_ptr = &lower;
	}

	// If there is a starting descriptor, search down index to starting position.
	// This may involve sibling buckets if splits are in progress.  If there
	// isn't a starting descriptor, walk down the left side of the index.

	if (limit_ptr)
	{
		UCHAR* pointer = NULL;
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
		return pointer;
	}

	return BTreeNode::getPointerFirstNode(page);
}

void IndexTableScan::setPage(thread_db* tdbb, win* window)
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	const SLONG newPage = window ? window->win_page.getPageNum() : 0;

	if (impure->irsb_nav_page != newPage)
	{
		if (impure->irsb_nav_page)
		{
			impure->irsb_nav_btr_gc_lock->enablePageGC(tdbb);
		}

		if (newPage)
		{
			if (!impure->irsb_nav_btr_gc_lock)
			{
				impure->irsb_nav_btr_gc_lock =
					FB_NEW_RPT(*tdbb->getDefaultPool(), 0) BtrPageGCLock(tdbb);
			}

			impure->irsb_nav_btr_gc_lock->disablePageGC(tdbb, window->win_page);
		}

		impure->irsb_nav_page = newPage;
	}
}

void IndexTableScan::setPosition(thread_db* tdbb,
								   record_param* rpb,
								   win* window,
								   const UCHAR* pointer,
								   btree_exp* expanded_node,
								   const UCHAR* key_data,
								   USHORT length)
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	// We can actually set position without having a data page
	// fetched; if not, just set the incarnation to the lowest possible
	impure->irsb_nav_incarnation = CCH_get_incarnation(window);
	setPage(tdbb, window);
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
	else
	{
		impure->irsb_nav_expanded_offset = -1;
	}
}

bool IndexTableScan::setupBitmaps(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	// Start a bitmap which tells us we have already visited
	// this record; this is to handle the case where there is more
	// than one leaf node reference to the same record number; the
	// bitmap allows us to filter out the multiple references.
	RecordBitmap::reset(impure->irsb_nav_records_visited);

	// the first time we open the stream, compute a bitmap
	// for the inversion tree -- this may cause problems for
	// read-committed transactions since they will get one
	// view of the database when the stream is opened
	if (m_inversion)
	{
		// There is no need to reset or release the bitmap, it is
		// done in EVL_bitmap()
		impure->irsb_nav_bitmap = EVL_bitmap(tdbb, m_inversion, NULL);
		return (*impure->irsb_nav_bitmap != NULL);
	}

	return true;
}
