/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		btn.cpp
 *	DESCRIPTION:	B-tree management code
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Arno Brinkman
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2003 Arno Brinkman and all contributors
 *  signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */


#include "firebird.h"			// needed for get_long
#include "memory_routines.h"	// needed for get_long

#include "../jrd/common.h"		// needed for SINT64
#include "../jrd/ods.h"			
#include "../jrd/btn.h"			

namespace BTreeNode {


USHORT computePrefix(const UCHAR* prevString, USHORT prevLength, 
			const UCHAR* string, USHORT length)
{
/**************************************
 *
 *	c o m p u t e P r e f i x
 *
 **************************************
 *
 * Functional description
 *	Compute and return prefix common 
 *  to two strings.
 *
 **************************************/
	USHORT l = MIN(prevLength, length);
	if (!l) {
		return 0;
	}

	const UCHAR* p = prevString;

	while (*p == *string) {
		++p;
		++string;
		if (!--l) {
			break;
		}
	}
	return (p - prevString);
}


SLONG findPageInDuplicates(const btree_page* page, UCHAR* pointer,
			SLONG previousNumber, SLONG findRecordNumber)
{
/**************************************
 *
 *	f i n d P a g e I n D u p l i c a t e s
 *
 **************************************
 *
 * Functional description
 *	Return the first page number
 *
 **************************************/
	const bool leafPage = (page->btr_level == 0);
	const SCHAR flags = page->btr_header.pag_flags;

	IndexNode node, previousNode;
	pointer = readNode(&node, pointer, flags, leafPage);

	while (true) {
		// loop through duplicates until 
		// correct node is found.
		// If this is an end bucket marker then return
		// the previous passed page number.
		if (BTreeNode::isEndBucket(&node, leafPage)) {
			return previousNumber;
		}
		if (findRecordNumber < node.recordNumber) {
			// If first record number on page is higher
			// then record number must be at the previous
			// passed page number.
			return previousNumber;
		} 
		else if (findRecordNumber == node.recordNumber) {
			// If first record number on page is equal
			// then we are exactly on the right place :)
			return node.pageNumber;
		} 
		// Save current page number and fetch next node 
		// for comparision.
		previousNumber = node.pageNumber;
		previousNode = node;
		pointer = BTreeNode::readNode(&node, pointer, flags, leafPage);
		// We're done if end level marker is reached or this 
		// isn't a equal node anymore.
		if (BTreeNode::isEndLevel(&node, leafPage) || 
			(node.length != 0) || 
			(node.prefix != (previousNode.length + previousNode.prefix)))
		{
			return previousNumber;
		}
	}
	// We never reach this point
}


USHORT getJumpNodeSize(const IndexJumpNode* jumpNode, SCHAR flags)
{
/**************************************
 *
 *	g e t J u m p N o d e S i z e
 *
 **************************************
 *
 * Functional description
 *	Return the size needed to store 
 *  this node.
 *
 **************************************/
	USHORT result = 0;
	if (flags & btr_large_keys) {
		// Size needed for prefix
		result += sizeof(USHORT);
		// Size needed for length
		result += sizeof(USHORT);
	}
	else {
		// Size needed for prefix
		result++;
		// Size needed for length
		result++;
	}
	// Size needed for offset
	result += sizeof(USHORT);
	// Size needed for data
	result += jumpNode->length;
	return result;
}


USHORT getNodeSize(const IndexNode* indexNode, SCHAR flags, bool leafNode)
{
/**************************************
 *
 *	g e t N o d e S i z e
 *
 **************************************
 *
 * Functional description
 *	Return the size needed to store 
 *  this node.
 *
 **************************************/
	USHORT result = 0;
	if (flags & btr_large_keys) {
		// Size needed for prefix  
		if (indexNode->prefix >= 128) {
			result += 2;
		}
		else {
			result++;
		}
		// Size needed for length 
		if (indexNode->length >= 128) {
			result += 2;
		}
		else {
			result++;
		}
		// Size needed for record number 
		result +=  sizeof(SLONG);
	}
	else {
		// Size needed for prefix  
		result++;
		// Size needed for length 
		result++;
		// Size needed for page number 
		result += sizeof(SLONG);
	}

	result += indexNode->length;

	if (!leafNode && (flags & btr_all_record_number)) {
		// Size needed for record number
		result += sizeof(SLONG);
	}

	return result;
}


UCHAR* getPointerFirstNode(btree_page* page, IndexJumpInfo* jumpInfo)
{
/**************************************
 *
 *	g e t P o i n t e r F i r s t N o d e
 *
 **************************************
 *
 * Functional description
 *	Return the pointer to first data node 
 *  in the bucket. When a jumpInfo is
 *  given, then pointer of first jump
 *  node is returned.
 *
 **************************************/
	if (page->btr_header.pag_flags & btr_jump_info) {
		if (jumpInfo) {
			UCHAR* pointer = reinterpret_cast<UCHAR*>(page->btr_nodes);
			return readJumpInfo(jumpInfo, pointer);
		}
		else {
			IndexJumpInfo jumpInformation;
			UCHAR* pointer = reinterpret_cast<UCHAR*>(page->btr_nodes);
			readJumpInfo(&jumpInformation, pointer);
			return reinterpret_cast<UCHAR*>(page) + jumpInformation.firstNodeOffset;
		}
	}
	else {
		return reinterpret_cast<UCHAR*>(page->btr_nodes);
	}
}


bool isEndBucket(const IndexNode* indexNode, bool leafNode)
{
/**************************************
 *
 *	i s E n d B u c k e t
 *
 **************************************
 *
 * Functional description
 *	Check if this is a END_BUCKET
 *  marker node.
 *
 **************************************/
	if (leafNode) {
		return (indexNode->recordNumber == END_BUCKET);
	}
	else {
		return (indexNode->pageNumber == END_BUCKET);
	}
}


bool isEndLevel(const IndexNode* indexNode, bool leafNode)
{
/**************************************
 *
 *	i s E n d L e v e l
 *
 **************************************
 *
 * Functional description
 *	Check if this is a END_LEVEL
 *  marker node.
 *
 **************************************/
	if (leafNode) {
		return (indexNode->recordNumber == END_LEVEL);
	}
	else {
		return (indexNode->pageNumber == END_LEVEL);
	}
}


bool keyEquality(USHORT length, const UCHAR* data, const IndexNode* indexNode)
{
/**************************************
 *
 *	k e y E q u a l i t y
 *
 **************************************
 *
 * Functional description
 *	Check a B-tree node against a key for equality.
 *
 **************************************/

	if (length != indexNode->length + indexNode->prefix) {
		return false;
	}

	USHORT l = indexNode->length;
	if (!l) {
		return true;
	}

	const UCHAR* p = indexNode->data;
	const UCHAR* q = data + indexNode->prefix;
	while (l) {
		if (*p++ != *q++) {
			return false;
		}
		--l;
	};

	return true;
}


#ifdef SCROLLABLE_CURSORS
UCHAR* lastNode(btree_page* page, EXP expanded_page, BTX* expanded_node)
{
/**************************************
 *
 *	l a s t N o d e
 *
 **************************************
 *
 * Functional description                               
 *	Find the last node on a page.  Used when walking
 *	down the right side of an index tree.  
 *
 **************************************/

	// the last expanded node is always at the end of the page 
	// minus the size of a BTX, since there is always an extra
	// BTX node with zero-length tail at the end of the page
	BTX enode = (BTX) ((UCHAR*) expanded_page + expanded_page->exp_length - BTX_SIZE);

	// starting at the end of the page, find the
	// first node that is not an end marker
	UCHAR* pointer = ((UCHAR*) page + page->btr_length);
	const SCHAR flags = page->btr_header.pag_flags;
	IndexNode node;
	while (true) {
		pointer = previousNode(&node, pointer, flags, &enode);
		if (!isEndBucket(&node, false) && 
			!isEndLevel(&node, false)) 
		{
			if (expanded_node) {
				*expanded_node = enode;
			}
			return node.nodePointer;
		}
	}
}
#endif


UCHAR* nextNode(IndexNode* node, UCHAR* pointer, 
					SCHAR flags,  BTX* expanded_node)
{
/**************************************
 *
 *	n e x t N o d e
 *
 **************************************
 *
 * Functional description                               
 *	Find the next node on both the index page
 *	and its associated expanded buffer.
 *
 **************************************/

	pointer = readNode(node, pointer, flags, true);

	if (*expanded_node) {
		*expanded_node = (BTX) ((UCHAR*) (*expanded_node)->btx_data + 
			node->prefix + node->length);
	}

	return pointer;
}


UCHAR* previousNode(IndexNode* node, UCHAR* pointer,
					SCHAR flags,  BTX* expanded_node)
{
/**************************************
 *
 *	p r e v i o u s N o d e
 *
 **************************************
 *
 * Functional description                               
 *	Find the previous node on a page.  Used when walking
 *	an index backwards.  
 *
 **************************************/

	pointer = (pointer - (*expanded_node)->btx_btr_previous_length);

	*expanded_node = (BTX) ((UCHAR*) *expanded_node - (*expanded_node)->btx_previous_length);

	return pointer;
}


void quad_put(SLONG value, UCHAR* data)
{
/**************************************
 *
 *	q u a d _ p u t
 *
 **************************************
 *
 * Functional description
 *	Move SLONG to a four byte vector.
 *
 **************************************/

	const UCHAR* p = (UCHAR*) &value;

	data[0] = p[0];
	data[1] = p[1];
	data[2] = p[2];
	data[3] = p[3];
}


UCHAR* readJumpInfo(IndexJumpInfo* jumpInfo, UCHAR* pagePointer)
{
/**************************************
 *
 *	r e a d J u m p I n f o
 *
 **************************************
 *
 * Functional description
 *	Read jump information from the page 
 *  by the given pagePointer and the 
 *  return the remaining position after 
 *  the read.
 *
 **************************************/
	jumpInfo->firstNodeOffset = *reinterpret_cast<const USHORT*>(pagePointer);
	pagePointer += sizeof(USHORT);
	jumpInfo->jumpAreaSize = *reinterpret_cast<const USHORT*>(pagePointer);
	pagePointer += sizeof(USHORT);
	jumpInfo->jumpers = (USHORT)(*pagePointer);
	++pagePointer;
	jumpInfo->keyLength = *reinterpret_cast<const USHORT*>(pagePointer);
	pagePointer += sizeof(USHORT);
	return pagePointer;
}


UCHAR* readJumpNode(IndexJumpNode* jumpNode, UCHAR* pagePointer, 
					SCHAR flags)
{
/**************************************
 *
 *	r e a d J u m p N o d e
 *
 **************************************
 *
 * Functional description
 *	Read a jump node from the page by the
 *  given pagePointer and the return the
 *  remaining position after the read.
 *
 **************************************/
	jumpNode->nodePointer = pagePointer;
	if (flags & btr_large_keys) {
		jumpNode->prefix = *reinterpret_cast<const USHORT*>(pagePointer);
		pagePointer += sizeof(USHORT);
		jumpNode->length = *reinterpret_cast<const USHORT*>(pagePointer);
		pagePointer += sizeof(USHORT);
	}
	else {
		jumpNode->prefix = (USHORT)(UCHAR)(*pagePointer);
		++pagePointer;
		jumpNode->length = (USHORT)(UCHAR)(*pagePointer);
		++pagePointer;
	}
	jumpNode->offset = *reinterpret_cast<const USHORT*>(pagePointer);
	pagePointer += sizeof(USHORT);
	jumpNode->data = pagePointer;
	pagePointer += jumpNode->length;
	return pagePointer;
}


UCHAR* readNode(IndexNode* indexNode, UCHAR* pagePointer, SCHAR flags, bool leafNode)
{
/**************************************
 *
 *	r e a d N o d e
 *
 **************************************
 *
 * Functional description
 *	Read a leaf/page node from the page by the
 *  given pagePointer and the return the
 *  remaining position after the read.
 *
 **************************************/
	indexNode->nodePointer = pagePointer;
	if (flags & btr_large_keys) {
		indexNode->prefix = (USHORT)(*pagePointer);
		pagePointer++;
		if (indexNode->prefix & 128) {
			const USHORT prefix = (USHORT)(*pagePointer);
			indexNode->prefix = (prefix << 7) | (indexNode->prefix & 127);
			pagePointer++;
		}
		indexNode->length = (USHORT)(*pagePointer);
		pagePointer++;
		if (indexNode->length & 128) {
			const USHORT length = (USHORT)(*pagePointer);
			indexNode->length = (length << 7) | (indexNode->length & 127);
			pagePointer++;
		}
		if (leafNode) {
			indexNode->recordNumber = *reinterpret_cast<const SLONG*>(pagePointer);
		} 
		else {
			indexNode->pageNumber = *reinterpret_cast<const SLONG*>(pagePointer);
		}
		pagePointer += 4;
	}
	else {
		indexNode->prefix = (*pagePointer);
		pagePointer++;
		indexNode->length = (*pagePointer);
		pagePointer++;
		if (leafNode) {
			indexNode->recordNumber = get_long(pagePointer);
		} 
		else {
			indexNode->pageNumber = get_long(pagePointer);
		}
		pagePointer += 4;
	}

	indexNode->data = pagePointer;
	pagePointer += indexNode->length;

	if (!leafNode && (flags & btr_all_record_number)) {
		if (flags & btr_large_keys) {
			indexNode->recordNumber = *reinterpret_cast<const SLONG*>(pagePointer);
		}
		else {
			indexNode->recordNumber = get_long(pagePointer);
		}
		pagePointer += 4;
	}
	return pagePointer;
}


UCHAR* writeJumpInfo(btree_page* page, const IndexJumpInfo* jumpInfo)
{
/**************************************
 *
 *	w r i t e J u m p I n f o
 *
 **************************************
 *
 * Functional description
 *	Write jump information to the page by the
 *  given pointer.
 *
 **************************************/
	UCHAR* pointer = reinterpret_cast<UCHAR*> (page->btr_nodes);
	*reinterpret_cast<USHORT*>(pointer) = jumpInfo->firstNodeOffset;
	pointer += sizeof(USHORT);
	*reinterpret_cast<USHORT*>(pointer) = jumpInfo->jumpAreaSize;
	pointer += sizeof(USHORT);
	*pointer = (UCHAR) jumpInfo->jumpers;
	pointer++;
	*reinterpret_cast<USHORT*>(pointer) = jumpInfo->keyLength;
	pointer += sizeof(USHORT);
	return pointer;
}


UCHAR* writeJumpNode(IndexJumpNode* jumpNode, UCHAR* pagePointer, 
						SCHAR flags)
{
/**************************************
 *
 *	w r i t e J u m p N o d e
 *
 **************************************
 *
 * Functional description
 *	Write jump information to the page by the
 *  given pointer.
 *
 **************************************/
	jumpNode->nodePointer = pagePointer;
	if (flags & btr_large_keys) {
		*reinterpret_cast<USHORT*>(pagePointer) = jumpNode->prefix;
		pagePointer += sizeof(USHORT);
		*reinterpret_cast<USHORT*>(pagePointer) = jumpNode->length;
		pagePointer += sizeof(USHORT);
	}
	else {
		*pagePointer = (UCHAR) jumpNode->prefix;
		pagePointer++;
		*pagePointer = (UCHAR) jumpNode->length;
		pagePointer++;
	}
	*reinterpret_cast<USHORT*>(pagePointer) = jumpNode->offset;
	pagePointer += sizeof(USHORT);
	USHORT size = jumpNode->length;
	UCHAR* ptr = jumpNode->data;
	while (size) {
		*pagePointer++ = *ptr++;
		size--;
	}
	return pagePointer;
}


UCHAR* writeNode(IndexNode* indexNode, UCHAR* pagePointer, SCHAR flags, 
	bool leafNode, bool withData)
{
/**************************************
 *
 *	w r i t e N o d e
 *
 **************************************
 *
 * Functional description
 *	Write a leaf/page node to the page by the
 *  given page_pointer.
 *
 **************************************/
	indexNode->nodePointer = pagePointer;
	if (flags & btr_large_keys) {
		// Write prefix  
		if (indexNode->prefix >= 128) {
			*pagePointer = (UCHAR)(128 | (127 & indexNode->prefix));
			pagePointer++;
			*pagePointer = (UCHAR)(255 & (indexNode->prefix >> 7));
			pagePointer++;
		}
		else {
			*pagePointer = (UCHAR)indexNode->prefix;
			pagePointer++;
		}
		// Write length 
		if (indexNode->length >= 128) {
			*pagePointer = (UCHAR)(128 | (127 & indexNode->length));
			pagePointer++;
			*pagePointer = (UCHAR)(255 & (indexNode->length >> 7));
			pagePointer++;
		}
		else {
			*pagePointer = (UCHAR)indexNode->length;
			pagePointer++;
		}
		if (leafNode) {
			// Write record number 
			*reinterpret_cast<SLONG*>(pagePointer) = indexNode->recordNumber;
		}
		else {
			// Write page number 
			*reinterpret_cast<SLONG*>(pagePointer) = indexNode->pageNumber;
		}
		pagePointer += sizeof(SLONG);
	}
	else {
		// Write prefix  
		*pagePointer = (UCHAR)indexNode->prefix;
		pagePointer++;
		// Write length 
		*pagePointer = (UCHAR)indexNode->length;
		pagePointer++;
		if (leafNode) {
			// Write record number 
			quad_put(indexNode->recordNumber, pagePointer);
		}
		else {
			// Write page number 
			quad_put(indexNode->pageNumber, pagePointer);
		}
		pagePointer += sizeof(SLONG);
	}

	if (withData) {
		memcpy(pagePointer, indexNode->data, indexNode->length);
		pagePointer += indexNode->length;
	} 
	else {
		memmove(pagePointer, indexNode->data, indexNode->length);
		pagePointer += indexNode->length;
	}


	if (!leafNode && (flags & btr_all_record_number)) {
		// Write record number 
		if (flags & btr_large_keys) {
			*reinterpret_cast<SLONG*>(pagePointer) = indexNode->recordNumber;
		}
		else {
			quad_put(indexNode->recordNumber, pagePointer);
		}
		pagePointer += sizeof(SLONG);
	}

	return pagePointer;
}


void setEndBucket(IndexNode* indexNode, bool leafNode)
{
/**************************************
 *
 *	s e t E n d B u c k e t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	if (leafNode) {
		indexNode->recordNumber = END_BUCKET;
	}
	else {
		indexNode->pageNumber = END_BUCKET;
	}
}


void setEndLevel(IndexNode* indexNode, bool leafNode)
{
/**************************************
 *
 *	s e t E n d L e v e l
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	indexNode->prefix = 0;
	indexNode->length = 0;
	if (leafNode) {
		indexNode->recordNumber = END_LEVEL;
	}
	else {
		indexNode->pageNumber = END_LEVEL;
	}
}


} // namespace BTreeNode
