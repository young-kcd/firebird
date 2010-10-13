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

#include "../common/common.h"		// needed for SINT64
#include "../jrd/ods.h"
#include "../jrd/btn.h"

namespace BTreeNode {

using namespace Ods;

USHORT computePrefix(const UCHAR* prevString, USHORT prevLength, const UCHAR* string, USHORT length)
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

	while (*p == *string)
	{
		++p;
		++string;
		if (!--l) {
			break;
		}
	}
	return (p - prevString);
}


SLONG findPageInDuplicates(const btree_page* page, UCHAR* pointer,
			SLONG previousNumber, RecordNumber findRecordNumber)
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

	IndexNode node, previousNode;
	pointer = readNode(&node, pointer, leafPage);

	// Check if pointer is still valid
	//if (pointer > endPointer) {
	//	BUGCHECK(204);	// msg 204 index inconsistent
	//}

	while (true)
	{
		// loop through duplicates until
		// correct node is found.
		// If this is an end bucket marker then return
		// the previous passed page number.
		if (node.isEndBucket) {
			return previousNumber;
		}
		if (findRecordNumber <= node.recordNumber) {
			// If first record number on page is higher
			// then record number must be at the previous
			// passed page number.
			return previousNumber;
		}
		// Save current page number and fetch next node
		// for comparision.
		previousNumber = node.pageNumber;
		previousNode = node;
		pointer = readNode(&node, pointer, leafPage);

		// Check if pointer is still valid
		//if (pointer > endPointer) {
		//	BUGCHECK(204);	// msg 204 index inconsistent
		//}

		// We're done if end level marker is reached or this
		// isn't a equal node anymore.
		if ((node.isEndLevel) ||
			(node.length != 0) ||
			(node.prefix != (previousNode.length + previousNode.prefix)))
		{
			return previousNumber;
		}
	}
	// We never reach this point
}


USHORT getJumpNodeSize(const IndexJumpNode* jumpNode)
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

	// Size needed for prefix
	USHORT number = jumpNode->prefix;
	if (number & 0xC000) {
		result += 3;
	}
	else if (number & 0xFF80) {
		result += 2;
	}
	else {
		result += 1;
	}

	// Size needed for length
	number = jumpNode->length;
	if (number & 0xC000) {
		result += 3;
	}
	else if (number & 0xFF80) {
		result += 2;
	}
	else {
		result += 1;
	}

	// Size needed for offset
	// NOTE! offset can be unknown when this function is called,
	// therefor we can't use a compression method.
	result += sizeof(USHORT);
	// Size needed for data
	result += jumpNode->length;
	return result;
}


USHORT getNodeSize(const IndexNode* indexNode, bool leafNode)
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

	// Determine flags
	UCHAR internalFlags = 0;
	if (indexNode->isEndLevel) {
		internalFlags = BTN_END_LEVEL_FLAG;
	}
	else if (indexNode->isEndBucket) {
		internalFlags = BTN_END_BUCKET_FLAG;
	}
	else if (indexNode->length == 0)
	{
		if (indexNode->prefix == 0) {
			internalFlags = BTN_ZERO_PREFIX_ZERO_LENGTH_FLAG;
		}
		else {
			internalFlags = BTN_ZERO_LENGTH_FLAG;
		}
	}
	else if (indexNode->length == 1) {
		internalFlags = BTN_ONE_LENGTH_FLAG;
	}

	// Store internal flags + 5 bits from number
	SINT64 number = indexNode->recordNumber.getValue();
	if (number < 0) {
		number = 0;
	}
	result++;
	// If this is a END_LEVEL marker then we're done
	if (indexNode->isEndLevel) {
		return result;
	}

	number >>= 5;
	// Get size for storing remaining bits for number
	// 5 bytes should be enough to fit remaining 34 bits of record number
	if (number & QUADCONST(0xFFF0000000)) {
		result += 5;
	}
	else if (number & QUADCONST(0xFFFFE00000)) {
		result += 4;
	}
	else if (number & QUADCONST(0xFFFFFFC000)) {
		result += 3;
	}
	else if (number & QUADCONST(0xFFFFFFFF80)) {
		result += 2;
	}
	else {
		result += 1;
	}

	if (!leafNode)
	{
		// Size needed for page number
		number = indexNode->pageNumber;
		if (number < 0) {
			number = 0;
		}

		if (number & 0xF0000000) {
			result += 5;
		}
		else if (number & 0xFFE00000) {
			result += 4;
		}
		else if (number & 0xFFFFC000) {
			result += 3;
		}
		else if (number & 0xFFFFFF80) {
			result += 2;
		}
		else {
			result += 1;
		}
	}

	if (internalFlags != BTN_ZERO_PREFIX_ZERO_LENGTH_FLAG)
	{
		// Size needed for prefix
		number = indexNode->prefix;
		if (number & 0xFFFFC000) {
			result += 3;
		}
		else if (number & 0xFFFFFF80) {
			result += 2;
		}
		else {
			result += 1;
		}
	}

	if ((internalFlags != BTN_ZERO_PREFIX_ZERO_LENGTH_FLAG) &&
		(internalFlags != BTN_ZERO_LENGTH_FLAG) &&
		(internalFlags != BTN_ONE_LENGTH_FLAG))
	{
		// Size needed for length
		number = indexNode->length;
		if (number & 0xFFFFC000) {
			result += 3;
		}
		else if (number & 0xFFFFFF80) {
			result += 2;
		}
		else {
			result += 1;
		}
	}

	result += indexNode->length;
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
	if (page->btr_header.pag_flags & btr_jump_info)
	{
		if (jumpInfo)
		{
			UCHAR* pointer = page->btr_nodes;
			return readJumpInfo(jumpInfo, pointer);
		}

		IndexJumpInfo jumpInformation;
		UCHAR* pointer = page->btr_nodes;
		readJumpInfo(&jumpInformation, pointer);

		return reinterpret_cast<UCHAR*>(page) + jumpInformation.firstNodeOffset;
	}

	return page->btr_nodes;
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

	USHORT len = indexNode->length;
	if (!len) {
		return true;
	}

	return !memcmp(indexNode->data, data + indexNode->prefix, len);
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
	jumpInfo->firstNodeOffset = get_short(pagePointer);
	pagePointer += sizeof(USHORT);
	jumpInfo->jumpAreaSize = get_short(pagePointer);
	pagePointer += sizeof(USHORT);
	jumpInfo->jumpers = (USHORT)(*pagePointer++);
	return pagePointer;
}


UCHAR* readJumpNode(IndexJumpNode* jumpNode, UCHAR* pagePointer)
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

	// Get prefix
	UCHAR tmp = *pagePointer++;
	jumpNode->prefix = (tmp & 0x7F);
	if (tmp & 0x80)
	{
		tmp = *pagePointer++;
		jumpNode->prefix |= (tmp & 0x7F) << 7; // We get 14 bits at this point
	}

	// Get length
	tmp = *pagePointer++;
	jumpNode->length = (tmp & 0x7F);
	if (tmp & 0x80)
	{
		tmp = *pagePointer++;
		jumpNode->length |= (tmp & 0x7F) << 7; // We get 14 bits at this point
	}

	jumpNode->offset = get_short(pagePointer);
	pagePointer += sizeof(USHORT);
	jumpNode->data = pagePointer;
	pagePointer += jumpNode->length;
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
	UCHAR* pointer = page->btr_nodes;
	put_short(pointer, jumpInfo->firstNodeOffset);
	pointer += sizeof(USHORT);
	put_short(pointer, jumpInfo->jumpAreaSize);
	pointer += sizeof(USHORT);
	*pointer++ = (UCHAR) jumpInfo->jumpers;
	return pointer;
}


UCHAR* writeJumpNode(IndexJumpNode* jumpNode, UCHAR* pagePointer)
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

	// Write prefix, maximum 14 bits
	USHORT number = jumpNode->prefix;
	UCHAR tmp = (number & 0x7F);
	number >>= 7;
	if (number > 0) {
		tmp |= 0x80;
	}
	*pagePointer++ = tmp;
	if (tmp & 0x80)
	{
		tmp = (number & 0x7F);
		*pagePointer++ = tmp;
	}

	// Write length, maximum 14 bits
	number = jumpNode->length;
	tmp = (number & 0x7F);
	number >>= 7;
	if (number > 0) {
		tmp |= 0x80;
	}
	*pagePointer++ = tmp;
	if (tmp & 0x80)
	{
		tmp = (number & 0x7F);
		*pagePointer++ = tmp;
	}

	put_short(pagePointer, jumpNode->offset);
	pagePointer += sizeof(USHORT);
	memmove(pagePointer, jumpNode->data, jumpNode->length);
	pagePointer += jumpNode->length;
	return pagePointer;
}


UCHAR* writeNode(IndexNode* indexNode, UCHAR* pagePointer, bool leafNode, bool withData)
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

	// AB: 2004-02-22
	// To allow as much as compression possible we
	// store numbers per 7 bit and the 8-th bit tell us
	// if we need to go on reading or we're done.
	// Also for duplicate node entries (length and prefix
	// are zero) we don't store the length and prefix
	// information. This will save at least 2 bytes per node.

	if (!withData)
	{
		// First move data so we can't override it.
		// For older structure node was always the same, but length
		// from new structure depends on the values.
		const USHORT offset = getNodeSize(indexNode, leafNode) - indexNode->length;
		pagePointer += offset; // set pointer to right position
		memmove(pagePointer, indexNode->data, indexNode->length);
		pagePointer -= offset; // restore pointer to original position
	}

	// Internal flags
	UCHAR internalFlags = 0;
	if (indexNode->isEndLevel) {
		internalFlags = BTN_END_LEVEL_FLAG;
	}
	else if (indexNode->isEndBucket) {
		internalFlags = BTN_END_BUCKET_FLAG;
	}
	else if (indexNode->length == 0)
	{
		if (indexNode->prefix == 0) {
			internalFlags = BTN_ZERO_PREFIX_ZERO_LENGTH_FLAG;
		}
		else {
			internalFlags = BTN_ZERO_LENGTH_FLAG;
		}
	}
	else if (indexNode->length == 1) {
		internalFlags = BTN_ONE_LENGTH_FLAG;
	}

	SINT64 number = indexNode->recordNumber.getValue();
	if (number < 0) {
		number = 0;
	}
	// Store internal flags + 6 bits from number
	UCHAR tmp = internalFlags;
	*pagePointer++ = ((tmp << 5) | (number & 0x1F));

	if (indexNode->isEndLevel) {
		return pagePointer;
	}

	// Store remaining bits from number
	number >>= 5;
	tmp = (number & 0x7F);
	number >>= 7; //12
	if (number == 0) {
		*pagePointer++ = tmp;
	}
	else
	{
		*pagePointer++ = tmp | 0x80;
		tmp = (number & 0x7F);
		number >>= 7; //19
		if (number == 0) {
			*pagePointer++ = tmp;
		}
		else
		{
			*pagePointer++ = tmp | 0x80;
			tmp = (number & 0x7F);
			number >>= 7; //26
			if (number == 0) {
				*pagePointer++ = tmp;
			}
			else
			{
				*pagePointer++ = tmp | 0x80;
				tmp = (number & 0x7F);
				number >>= 7; //33
				if (number == 0) {
					*pagePointer++ = tmp;
				}
				else
				{
					*pagePointer++ = tmp | 0x80;
					tmp = (number & 0x7F);
					number >>= 7; //40
					if (number == 0) {
						*pagePointer++ = tmp;
					}
/*
			Enable this if you need more bits in record number
						else
						{
							*pagePointer++ = tmp | 0x80;
							tmp = (number & 0x7F);
							number >>= 7; //47
							if (number == 0) {
								*pagePointer++ = tmp;
							}
							else
							{
								*pagePointer++ = tmp | 0x80;
								tmp = (number & 0x7F);
								number >>= 7; //54
								if (number == 0) {
									*pagePointer++ = tmp;
								}
								else
								{
									*pagePointer++ = tmp | 0x80;
									tmp = (number & 0x7F);
									number >>= 7; //61
									if (number == 0) {
										*pagePointer++ = tmp;
									}
									else {
										// ....
									}
								}
							}
						}
*/
				}
			}
		}
	}

	if (!leafNode)
	{
		// Store page number for non-leaf pages
		number = indexNode->pageNumber;
		if (number < 0) {
			number = 0;
		}
		tmp = (number & 0x7F);
		number >>= 7;
		if (number > 0) {
			tmp |= 0x80;
		}
		*pagePointer++ = tmp;
		if (number > 0)
		{
			tmp = (number & 0x7F);
			number >>= 7; //14
			if (number > 0) {
				tmp |= 0x80;
			}
			*pagePointer++ = tmp;
			if (number > 0)
			{
				tmp = (number & 0x7F);
				number >>= 7; //21
				if (number > 0) {
					tmp |= 0x80;
				}
				*pagePointer++ = tmp;
				if (number > 0)
				{
					tmp = (number & 0x7F);
					number >>= 7; //28
					if (number > 0) {
						tmp |= 0x80;
					}
					*pagePointer++ = tmp;
					if (number > 0)
					{
						tmp = (number & 0x0F);
						number >>= 7; //35
						*pagePointer++ = tmp;
					}
				}
			}
		}
	}

	if (internalFlags != BTN_ZERO_PREFIX_ZERO_LENGTH_FLAG)
	{
		// Write prefix, maximum 14 bits
		number = indexNode->prefix;
		tmp = (number & 0x7F);
		number >>= 7;
		if (number > 0) {
			tmp |= 0x80;
		}
		*pagePointer++ = tmp;
		if (number > 0)
		{
			tmp = (number & 0x7F);
			*pagePointer++ = tmp;
		}
	}

	if ((internalFlags != BTN_ZERO_PREFIX_ZERO_LENGTH_FLAG) &&
		(internalFlags != BTN_ZERO_LENGTH_FLAG) &&
		(internalFlags != BTN_ONE_LENGTH_FLAG))
	{
		// Write length, maximum 14 bits
		number = indexNode->length;
		tmp = (number & 0x7F);
		number >>= 7;
		if (number > 0) {
			tmp |= 0x80;
		}
		*pagePointer++ = tmp;
		if (number > 0)
		{
			tmp = (number & 0x7F);
			*pagePointer++ = tmp;
		}
	}

	// Store data
	if (withData) {
		memcpy(pagePointer, indexNode->data, indexNode->length);
	}
	pagePointer += indexNode->length;

	return pagePointer;
}


void setEndBucket(IndexNode* indexNode) //, bool leafNode)
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
	indexNode->isEndBucket = true;
	indexNode->isEndLevel = false;
}


void setEndLevel(IndexNode* indexNode) //, bool leafNode)
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
	indexNode->isEndBucket = false;
	indexNode->isEndLevel = true;
	indexNode->prefix = 0;
	indexNode->length = 0;
	indexNode->pageNumber = 0;
	indexNode->recordNumber.setValue(0);
}


void setNode(IndexNode* indexNode, USHORT prefix, USHORT length,
			RecordNumber recordNumber, SLONG pageNumber,
			bool isEndBucket, bool isEndLevel)
{
/**************************************
 *
 *	s e t N o d e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	indexNode->isEndBucket = isEndBucket;
	indexNode->isEndLevel = isEndLevel;
	indexNode->prefix = prefix;
	indexNode->length = length;
	indexNode->recordNumber = recordNumber;
	indexNode->pageNumber = pageNumber;
}


} // namespace BTreeNode
