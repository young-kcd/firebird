/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		alloc.h
 *	DESCRIPTION:	Memory Pool Manager (based on B+ tree)
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
 * Created by: Nickolay Samofatov <skidder@bssys.com>
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#ifndef ALLOC_H
#define ALLOC_H

#include <malloc.h>
#include "../../include/fb_types.h"
#include "tree.h"

#define MAX_TREE_DEPTH 4

namespace Firebird {

struct MemoryBlock /* 16 bytes of block header is not too much I think */ {
	class MemoryPool *pool;
	bool used;
	bool last;
	SSHORT type;
	size_t length; /* Includes only actual block size, header not included */
	struct MemoryBlock *prev;
/*#ifdef DEBUG_GDS_ALLOC
	char *file;
	int line;
#endif*/
};

#define TYPE_POOL     -1
#define TYPE_EXTENT   -2
#define TYPE_LEAFPAGE -3
#define TYPE_TREEPAGE -4

// We store BlkInfo structures instead of BlkHeader pointers to get benefits from 
// processor cache-hit optimizations
struct BlockInfo {
	MemoryBlock *block;
	size_t length;
	static bool compare(const BlockInfo& i1, const BlockInfo& i2) {
		return (i1.length > i2.length) || 
			(i1.length == i2.length && i1.block > i2.block);
	}
};

struct MemoryExtent {
	MemoryExtent *next;
};

struct PendingFreeBlock {
	PendingFreeBlock *next;
};

// Memory pool based on B+ tree of free memory blocks
class MemoryPool {
private:
	typedef BePlusTree<BlockInfo, BlockInfo, MemoryPool, 
		DefaultKeyValue<BlockInfo>, BlockInfo> FreeBlocksTree;
	FreeBlocksTree freeBlocks; // B+ tree ordered by (length,address)
	MemoryExtent *extents; // Linked list of all memory extents

	Vector<void*,2> spareLeafs;
	Vector<void*,MAX_TREE_DEPTH+1> spareNodes;
	bool internalAlloc;
	bool needSpare;
	bool updatingSpare;
	PendingFreeBlock *pendingFree;
	
	// Do not allow to create and destroy pool directly from outside
	MemoryPool(void *first_extent, void *root_page) : 
		freeBlocks(this, root_page),
		extents((MemoryExtent *)first_extent), 
		internalAlloc(false),
		needSpare(false),
		updatingSpare(false),
		pendingFree(NULL)
	{
	}
	
	// This should never be called
	~MemoryPool() {
	}
	
	/* Returns NULL in case it cannot allocate requested chunk */
	static void* external_alloc(size_t size);
	
	static void external_free(void *blk);
	
	void updateSpare();
	
	void addFreeBlock(MemoryBlock *blk);
		
	void removeFreeBlock(MemoryBlock *blk);
public:
	static MemoryPool* createPool();

	static void deletePool(MemoryPool* pool);

	void* alloc(size_t size, SSHORT type = 0);

	void free(void *block);
	
	void verify_pool();

/*	void* calloc(size_t size) {
		void* result = malloc(size);
		memset(result,size,0);
	}*/
};

};

#endif
