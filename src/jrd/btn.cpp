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
		USHORT number = jumpNode->prefix;
		result++;
		number >>= 7;
		if (number > 0) {
			result++;
		}
		// Size needed for length
		number = jumpNode->length;
		result++;
		number >>= 7;
		if (number > 0) {
			result++;
		}
	}
	else {
		// Size needed for prefix
		result++;
		// Size needed for length
		result++;
	}
	// Size needed for offset
	// NOTE! offset can be unknown when this function is called,
	// therefor we can't use a compression method.
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

		// Determine flags 
		SLONG number = indexNode->recordNumber;
		if (!leafNode) {
			number = indexNode->pageNumber;
		}
		UCHAR internalFlags = 0;
		if (number == END_BUCKET) {
			internalFlags = 2;
		}
		if (number == END_LEVEL) {
			internalFlags = 1;
		}
		if ((internalFlags == 0) && 
			(indexNode->length == 0) && (indexNode->prefix == 0))
		{
			internalFlags = 3;
		}

		// Store internal flags + 6 bits from number
		if (!leafNode) {
			number = indexNode->recordNumber;
		}
		if (number < 0) {
			number = 0;
		}
		result++;
		number >>= 6;

		// Get size for storing remaining bits for number
		UCHAR tmp = (number & 0x7F);
		number >>= 7; //13
		if (number > 0) {
			tmp |= 0x80;
		}
		result++;
		if (tmp & 0x80) {
			tmp = (number & 0x7F);
			number >>= 7; //20
			if (number > 0) {
				tmp |= 0x80;
			}
			result++;
			if (tmp & 0x80) {
				tmp = (number & 0x7F);
				number >>= 7; //27
				if (number > 0) {
					tmp |= 0x80;
				}
				result++;
				if (tmp & 0x80) {
					tmp = (number & 0x7F);
					number >>= 7; //34
					if (number > 0) {
						tmp |= 0x80;
					}
					result++;
/*
	Change number to 64-bit type and enable this for 64-bit support

					if (tmp & 0x80) {
						tmp = (number & 0x7F);
						number >>= 7; //41
						if (number > 0) {
							tmp |= 0x80;
						}
						result++;
						if (tmp & 0x80) {
							tmp = (number & 0x7F);
							number >>= 7; //48
							if (number > 0) {
								tmp |= 0x80;
							}
							result++;
							if (tmp & 0x80) {
								tmp = (number & 0x7F);
								number >>= 7; //55
								if (number > 0) {
									tmp |= 0x80;
								}
								result++; //62 bits written 
							}
						}
					}*/
				}
			}
		}

		if (!leafNode) {
			// Size needed for page number
			number = indexNode->pageNumber;
			if (number < 0) {
				number = 0;
			}

			tmp = (number & 0x7F);
			number >>= 7;
			if (number > 0) {
				tmp |= 0x80;
			}
			result++; // 7
			if (tmp & 0x80) {
				tmp = (number & 0x7F);
				number >>= 7;
				if (number > 0) {
					tmp |= 0x80;
				}
				result++; // 14
				if (tmp & 0x80) {
					tmp = (number & 0x7F);
					number >>= 7;
					if (number > 0) {
						tmp |= 0x80;
					}
					result++; // 21
					if (tmp & 0x80) {
						tmp = (number & 0x7F);
						number >>= 7; 
						if (number > 0) {
							tmp |= 0x80;
						}
						result++; // 28
						if (tmp & 0x80) {
							tmp = (number & 0x7F);
							number >>= 7;
							if (number > 0) {
								tmp |= 0x80;
							}
							result++; // 35
/*
	Change number to 64-bit type and enable this for 64-bit support

							if (tmp & 0x80) {
								tmp = (number & 0x7F);
								number >>= 7;
								if (number > 0) {
									tmp |= 0x80;
								}
								result++; // 42
								if (tmp & 0x80) {
									tmp = (number & 0x7F);
									number >>= 7;
									if (number > 0) {
										tmp |= 0x80;
									}
									result++; // 49 
									if (tmp & 0x80) {
										tmp = (number & 0x7F);
										number >>= 7;
										if (number > 0) {
											tmp |= 0x80;
										}
										result++; // 56 
										if (tmp & 0x80) {
											tmp = (number & 0x7F);
											number >>= 7;
											if (number > 0) {
												tmp |= 0x80;
											}
											result++; // 62 bits written 
										}
									}
								}
							}*/
						}
					}
				}
			}
		}

		if (internalFlags != 3) {
			// Size needed for prefix  
			number = indexNode->prefix;
			tmp = (number & 0x7F);
			number >>= 7;
			if (number > 0) {
				tmp |= 0x80;
			}
			result++; // 7
			if (tmp & 0x80) {
				tmp = (number & 0x7F);
				number >>= 7;
				if (number > 0) {
					tmp |= 0x80;
				}
				result++; // 14
			}

			// Size needed for length 
			number = indexNode->length;
			tmp = (number & 0x7F);
			number >>= 7;
			if (number > 0) {
				tmp |= 0x80;
			}
			result++; // 7
			if (tmp & 0x80) {
				tmp = (number & 0x7F);
				number >>= 7;
				if (number > 0) {
					tmp |= 0x80;
				}
				result++; // 14
			}
		}

		result += indexNode->length;
	}
	else {
		// Size needed for prefix  
		result++;
		// Size needed for length 
		result++;
		// Size needed for page number 
		result += sizeof(SLONG);

		result += indexNode->length;

		if (!leafNode && (flags & btr_all_record_number)) {
			// Size needed for record number
			result += sizeof(SLONG);
		}
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
UCHAR* lastNode(btree_page* page, jrd_exp* expanded_page, BTX* expanded_node)
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
		// Get prefix
		UCHAR tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
		pagePointer++;
		jumpNode->prefix = (tmp & 0x7F);
		if (tmp & 0x80) {
			tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
			pagePointer++;
			jumpNode->prefix |= (tmp & 0x7F) << 7; // We get 14 bits at this point
		}
		// Get length
		tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
		pagePointer++;
		jumpNode->length = (tmp & 0x7F);
		if (tmp & 0x80) {
			tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
			pagePointer++;
			jumpNode->length |= (tmp & 0x7F) << 7; // We get 14 bits at this point
		}
	}
	else {
		jumpNode->prefix = (USHORT)(UCHAR)(*pagePointer);
		pagePointer++;
		jumpNode->length = (USHORT)(UCHAR)(*pagePointer);
		pagePointer++;
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
		// Initialize
		indexNode->prefix = 0;
		indexNode->length = 0;
		bool duplicate = false;

		// Get first byte that contains internal flags and 6 bits from number
		UCHAR internalFlags = *reinterpret_cast<const UCHAR*>(pagePointer);
		SLONG number = (internalFlags & 0x3F);
		internalFlags = ((internalFlags & 0xC0) >> 6);
		pagePointer++;

		if (internalFlags == BTN_DUPLICATE_FLAG) {
			duplicate = true;
		}

		// Get remaining bits for number
		SLONG tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
		pagePointer++;
		number |= (tmp & 0x7F) << 6;
		if (tmp & 0x80) {
			tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
			pagePointer++;
			number |= (tmp & 0x7F) << 13;
			if (tmp & 0x80) {
				tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
				pagePointer++;
				number |= (tmp & 0x7F) << 20;
				if (tmp & 0x80) {
					tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
					pagePointer++;
					number |= (tmp & 0x7F) << 27;
					if (tmp & 0x80) {
						tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
						pagePointer++;
						number |= (tmp & 0x7F) << 34;
/*
	Change number to 64-bit type and enable this for 64-bit support

						if (tmp & 0x80) {
							tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
							pagePointer++;
							number |= (tmp & 0x7F) << 41;
							if (tmp & 0x80) {
								tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
								pagePointer++;
								number |= (tmp & 0x7F) << 48;
								if (tmp & 0x80) {
									tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
									pagePointer++;
									number |= (tmp & 0x7F) << 55; // We get 62 bits at this point!
								}
							}
						}*/
					}
				}
			}
		}
		
		if (leafNode) {
			if ((internalFlags == BTN_END_LEVEL_FLAG) || 
				(internalFlags == BTN_END_BUCKET_FLAG)) 
			{
				if (number == BTN_DUPLICATE_MARKER) {
					duplicate = true;
				}
			}
			if (internalFlags == BTN_END_LEVEL_FLAG) {
				number = END_LEVEL;
			}
			if (internalFlags == BTN_END_BUCKET_FLAG) {
				number = END_BUCKET;
			}
		}
		indexNode->recordNumber = number;

		if (!leafNode) {
			// Get page number for non-leaf pages
			tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
			pagePointer++;
			number = (tmp & 0x7F);
			if (tmp & 0x80) {
				tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
				pagePointer++;
				number |= (tmp & 0x7F) << 7;
				if (tmp & 0x80) {
					tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
					pagePointer++;
					number |= (tmp & 0x7F) << 14;
					if (tmp & 0x80) {
						tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
						pagePointer++;
						number |= (tmp & 0x7F) << 21;
						if (tmp & 0x80) {
							tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
							pagePointer++;
							number |= (tmp & 0x7F) << 28;
							if (tmp & 0x80) {
								tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
								pagePointer++;
								number |= (tmp & 0x7F) << 35;
/*
	Change number to 64-bit type and enable this for 64-bit support

								if (tmp & 0x80) {
									tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
									pagePointer++;
									number |= (tmp & 0x7F) << 42;
									if (tmp & 0x80) {
										tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
										pagePointer++;
										number |= (tmp & 0x7F) << 49;
										if (tmp & 0x80) {
											tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
											pagePointer++;
											number |= (tmp & 0x7F) << 56; // We get 63 bits at this point!
										}
									}
								}*/
							}
						}
					}
				}
			}
			indexNode->pageNumber = number;

			if ((internalFlags == BTN_END_LEVEL_FLAG) || 
				(internalFlags == BTN_END_BUCKET_FLAG)) 
			{
				if (number == BTN_DUPLICATE_MARKER) {
					duplicate = true;
				}
			}

			if (internalFlags == BTN_END_LEVEL_FLAG) {
				indexNode->pageNumber = END_LEVEL;
			}
			if (internalFlags == BTN_END_BUCKET_FLAG) {
				indexNode->pageNumber = END_BUCKET;
			}
		}

		// Get prefix and length if it isn't a duplicate
		if (!duplicate) {
			// Get prefix
			tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
			pagePointer++;
			indexNode->prefix = (tmp & 0x7F);
			if (tmp & 0x80) {
				tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
				pagePointer++;
				indexNode->prefix |= (tmp & 0x7F) << 7; // We get 14 bits at this point
			}
			// Get length
			tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
			pagePointer++;
			indexNode->length = (tmp & 0x7F);
			if (tmp & 0x80) {
				tmp = *reinterpret_cast<const UCHAR*>(pagePointer);
				pagePointer++;
				indexNode->length |= (tmp & 0x7F) << 7; // We get 14 bits at this point
			}
		}

		// Get pointer where data starts
		indexNode->data = pagePointer;
		pagePointer += indexNode->length;
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

		indexNode->data = pagePointer;
		pagePointer += indexNode->length;

		if (!leafNode && (flags & btr_all_record_number)) {
			indexNode->recordNumber = get_long(pagePointer);
			pagePointer += 4;
		}
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
		// Write prefix, maximum 14 bits
		USHORT number = jumpNode->prefix;
		UCHAR tmp = (number & 0x7F);
		number >>= 7;
		if (number > 0) {
			tmp |= 0x80;
		}
		*pagePointer = tmp;
		pagePointer++;
		if (tmp & 0x80) {
			tmp = (number & 0x7F);
			*pagePointer = tmp;
			pagePointer++;
		}

		// Write length, maximum 14 bits
		number = jumpNode->length;
		tmp = (number & 0x7F);
		number >>= 7;
		if (number > 0) {
			tmp |= 0x80;
		}
		*pagePointer = tmp;
		pagePointer++;
		if (tmp & 0x80) {
			tmp = (number & 0x7F);
			*pagePointer = tmp;
			pagePointer++;
		}
	}
	else {
		*pagePointer = (UCHAR) jumpNode->prefix;
		pagePointer++;
		*pagePointer = (UCHAR) jumpNode->length;
		pagePointer++;
	}
	*reinterpret_cast<USHORT*>(pagePointer) = jumpNode->offset;
	pagePointer += sizeof(USHORT);
	memmove(pagePointer, jumpNode->data, jumpNode->length);
	pagePointer += jumpNode->length;
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

		// AB: 2004-02-22
		// To allow as much as compression possible we
		// store numbers per 7 bit and the 8-th bit tell us
		// if we need to go on reading or we're done.
		// Also for duplicate node entries (length and prefix 
		// are zero) we don't store the length and prefix
		// information. This will save at least 2 bytes per node.

		bool duplicate = false;
		SLONG number = indexNode->recordNumber;
		if (!leafNode) {
			number = indexNode->pageNumber;
		}
		// Internal flags
		UCHAR internalFlags = 0;
		if (number == END_LEVEL) {
			internalFlags = BTN_END_LEVEL_FLAG;
		}
		else if (number == END_BUCKET) {
			internalFlags = BTN_END_BUCKET_FLAG;
		}
		if (!leafNode) {
			number = indexNode->recordNumber;
		}
		if ((indexNode->length == 0) && (indexNode->prefix == 0)) {
			duplicate = true;
			if (internalFlags == 0) {
				// Duplicate value or ASC NULL state
				internalFlags = BTN_DUPLICATE_FLAG;
			}
			else if (leafNode) {
				// if we have a END_BUCKET or END_LEVEL marker we
				// use the number for the duplicate information.
				number = BTN_DUPLICATE_MARKER;
			}
		}
		if (number < 0) {
			number = 0;
		}
		// Store internal flags + 6 bits from number
		UCHAR tmp = internalFlags;
		*pagePointer = ((tmp << 6) | (number & 0x3F));
		pagePointer++;

		// Store remaining bits from number
		number >>= 6;
		tmp = (number & 0x7F);
		number >>= 7; //13
		if (number > 0) {
			tmp |= 0x80;
		}
		*pagePointer = tmp;
		pagePointer++;
		if (tmp & 0x80) {
			tmp = (number & 0x7F);
			number >>= 7; //20
			if (number > 0) {
				tmp |= 0x80;
			}
			*pagePointer = tmp;
			pagePointer++;
			if (tmp & 0x80) {
				tmp = (number & 0x7F);
				number >>= 7; //27
				if (number > 0) {
					tmp |= 0x80;
				}
				*pagePointer = tmp;
				pagePointer++;
				if (tmp & 0x80) {
					tmp = (number & 0x7F);
					number >>= 7; //34
					if (number > 0) {
						tmp |= 0x80;
					}
					*pagePointer = tmp;
					pagePointer++;
/*
	Change number to 64-bit type and enable this for 64-bit support

					if (tmp & 0x80) {
						tmp = (number & 0x7F);
						number >>= 7; //41
						if (number > 0) {
							tmp |= 0x80;
						}
						*pagePointer = tmp;
						pagePointer++;
						if (tmp & 0x80) {
							tmp = (number & 0x7F);
							number >>= 7; //48
							if (number > 0) {
								tmp |= 0x80;
							}
							*pagePointer = tmp;
							pagePointer++;
							if (tmp & 0x80) {
								tmp = (number & 0x7F);
								number >>= 7; //55
								if (number > 0) {
									tmp |= 0x80;
								}
								*pagePointer = tmp;
								pagePointer++;
								if (tmp & 0x80) {
									tmp = (number & 0x7F);
									*pagePointer = tmp;
									pagePointer++;
								}
							}
						}
					}*/
				}
			}
		}

		if (!leafNode) {
			// Store page number for non-leaf pages
			number = indexNode->pageNumber;
			if (number < 0) {
				if (duplicate) {
					number = 1;
				}
				else {
					number = 0;
				}
			}

			tmp = (number & 0x7F);
			number >>= 7;
			if (number > 0) {
				tmp |= 0x80;
			}
			*pagePointer = tmp;
			pagePointer++;
			if (tmp & 0x80) {
				tmp = (number & 0x7F);
				number >>= 7; //14
				if (number > 0) {
					tmp |= 0x80;
				}
				*pagePointer = tmp;
				pagePointer++;
				if (tmp & 0x80) {
					tmp = (number & 0x7F);
					number >>= 7; //21
					if (number > 0) {
						tmp |= 0x80;
					}
					*pagePointer = tmp;
					pagePointer++;
					if (tmp & 0x80) {
						tmp = (number & 0x7F);
						number >>= 7; //28
						if (number > 0) {
							tmp |= 0x80;
						}
						*pagePointer = tmp;
						pagePointer++;
						if (tmp & 0x80) {
							tmp = (number & 0x7F);
							number >>= 7; //35
							if (number > 0) {
								tmp |= 0x80;
							}
							*pagePointer = tmp;
							pagePointer++;
/*
	Change number to 64-bit type and enable this for 64-bit support

							if (tmp & 0x80) {
								tmp = (number & 0x7F);
								number >>= 7; //42
								if (number > 0) {
									tmp |= 0x80;
								}
								*pagePointer = tmp;
								pagePointer++;
								if (tmp & 0x80) {
									tmp = (number & 0x7F);
									number >>= 7; //49
									if (number > 0) {
										tmp |= 0x80;
									}
									*pagePointer = tmp;
									pagePointer++;
									if (tmp & 0x80) {
										tmp = (number & 0x7F);
										number >>= 7; //56
										if (number > 0) {
											tmp |= 0x80;
										}
										*pagePointer = tmp;
										pagePointer++;
										if (tmp & 0x80) {
											tmp = (number & 0x7F);
											*pagePointer = tmp;
											pagePointer++;
										}
									}
								}
							} */
						}
					}
				}
			}
		}

		// Store prefix and length when it isn't a duplicate
		if (!duplicate) {
			// Write prefix, maximum 14 bits
			number = indexNode->prefix;
			tmp = (number & 0x7F);
			number >>= 7;
			if (number > 0) {
				tmp |= 0x80;
			}
			*pagePointer = tmp;
			pagePointer++;
			if (tmp & 0x80) {
				tmp = (number & 0x7F);
				*pagePointer = tmp;
				pagePointer++;
			}

			// Write length, maximum 14 bits 
			number = indexNode->length;
			tmp = (number & 0x7F);
			number >>= 7;
			if (number > 0) {
				tmp |= 0x80;
			}
			*pagePointer = tmp;
			pagePointer++;
			if (tmp & 0x80) {
				tmp = (number & 0x7F);
				*pagePointer = tmp;
				pagePointer++;
			}
		}

		// Store data
		if (withData) {
			memcpy(pagePointer, indexNode->data, indexNode->length);
		} 
		else {
			memmove(pagePointer, indexNode->data, indexNode->length);
		}
		pagePointer += indexNode->length;
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

		if (withData) {
			USHORT size = indexNode->length;
			const UCHAR* ptr = indexNode->data;
			while (size) {
				*pagePointer++ = *ptr++;
				size--;
			}
		} 
		else {
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
		indexNode->recordNumber = 0;
	}
}


} // namespace BTreeNode
