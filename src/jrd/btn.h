/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		btn.h
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

#ifndef JRD_BTN_H_
#define JRD_BTN_H_

#include "firebird.h"			// needed for get_long
#include "memory_routines.h"	// needed for get_long

#include "../jrd/common.h"
#include "../jrd/ods.h"
#include "../include/fb_vector.h"
#include "../common/classes/array.h"

// format of expanded index node, used for backwards navigation
typedef struct btx {
	UCHAR btx_previous_length;		// AB: total size for previous node --length of data for previous node
	UCHAR btx_btr_previous_length;	// length of data for previous node on btree page
	UCHAR btx_data[1];				// expanded data element
} *BTX;

#define BTX_SIZE	2

// format of expanded index buffer
typedef struct jrd_exp {
	USHORT exp_length;
	ULONG exp_incarnation;
	btx exp_nodes[1];
} *EXP;

#define EXP_SIZE	OFFSETA (EXP, exp_nodes)

struct dynKey {
	USHORT keyLength;
	UCHAR* keyData;
};

typedef Firebird::vector<dynKey*> keyList;
typedef Firebird::HalfStaticArray<IndexJumpNode, 32> jumpNodeList;

namespace BTreeNode {

	USHORT computePrefix(const UCHAR* prevString, USHORT prevLength, 
				const UCHAR* string, USHORT length);

	SLONG findPageInDuplicates(const btr* page, UCHAR* pointer, 
				SLONG previousNumber, SLONG findRecordNumber);

	USHORT getJumpNodeSize(const IndexJumpNode* jumpNode, SCHAR flags);
	USHORT getNodeSize(const IndexNode* indexNode, SCHAR flags, bool leafNode = true);
	UCHAR* getPointerFirstNode(btr* page, IndexJumpInfo* jumpInfo = NULL);

	bool isEndBucket(const IndexNode* indexNode, bool leafNode = true);
	bool isEndLevel(const IndexNode* indexNode, bool leafNode = true);

	bool keyEquality(USHORT length, const UCHAR* data, const IndexNode* indexNode);

#ifdef SCROLLABLE_CURSORS
	UCHAR* lastNode(btr* page, EXP expanded_page, BTX* expanded_node);
#endif

	UCHAR* nextNode(IndexNode* node, UCHAR* pointer, 
				SCHAR flags,  BTX* expanded_node);
	UCHAR* previousNode(IndexNode* node, UCHAR* pointer,
				SCHAR flags,  BTX* expanded_node);

	void quad_put(SLONG value, UCHAR *data);

	UCHAR* readJumpInfo(IndexJumpInfo* jumpInfo, UCHAR* pagePointer);
	UCHAR* readJumpNode(IndexJumpNode* jumpNode, UCHAR* pagePointer, SCHAR flags);
	UCHAR* readNode(IndexNode* indexNode, UCHAR* pagePointer, SCHAR flags, 
		bool leafNode);

	UCHAR* writeJumpInfo(btr* page, const IndexJumpInfo* jumpInfo);
	UCHAR* writeJumpNode(IndexJumpNode* jumpNode, UCHAR* pagePointer, SCHAR flags);
	UCHAR* writeNode(IndexNode* indexNode, UCHAR* pagePointer, SCHAR flags, 
		bool leafNode, bool withData = true);

	void setEndBucket(IndexNode* indexNode, bool leafNode = true);
	void setEndLevel(IndexNode* indexNode, bool leafNode = true);

} // namespace BTreeNode

#endif // JRD_BTN_H_
