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
#include "../../include/firebird.h"
#include "../jrd/common.h"
#include "../jrd/ib_stdio.h"
#include "tree.h"
#include "locks.h"

#define MAX_TREE_DEPTH 4
// Must be a power of 2
#define ALLOC_ALIGNMENT 4

#define MEM_ALIGN(X) FB_ALIGN(X,ALLOC_ALIGNMENT)

namespace Firebird {

struct MemoryBlock /* 16 bytes of block header is not too much I think */ {
	class MemoryPool *pool;
	bool used;
	bool last;
	SSHORT type;
	size_t length; /* Includes only actual block size, header not included */
	struct MemoryBlock *prev;
#ifdef DEBUG_GDS_ALLOC
	char *file;
	int line;
#endif
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

// We are going to have two target architectures:
// 1. Multi-process server with customizable lock manager
// 2. Multi-threaded server with single process (SUPERSERVER)
class MemoryPool {
private:
	class InternalAllocator {
	public:
		void* alloc(size_t size) {
			return ((MemoryPool*)this)->internal_alloc(size);
		}
		void free(void* block) {
			((MemoryPool*)this)->internal_free(block);
		}
	};
	typedef BePlusTree<BlockInfo, BlockInfo, InternalAllocator, 
		DefaultKeyValue<BlockInfo>, BlockInfo> FreeBlocksTree;
	FreeBlocksTree freeBlocks; // B+ tree ordered by (length,address)
	MemoryExtent *extents; // Linked list of all memory extents

	Vector<void*,2> spareLeafs;
	Vector<void*,MAX_TREE_DEPTH+1> spareNodes;
	bool needSpare;
	PendingFreeBlock *pendingFree;
#ifdef SUPERSERVER
	Spinlock lock;
#else
	SharedSpinlock lock;
#endif
	
	// Do not allow to create and destroy pool directly from outside
	MemoryPool(void *first_extent, void *root_page) : 
		freeBlocks((InternalAllocator*)this, root_page),
		extents((MemoryExtent *)first_extent), 
		needSpare(false),
		pendingFree(NULL)
	{
	}

	// This should never be called
	~MemoryPool() {
	}
	
	/* Returns NULL in case it cannot allocate requested chunk */
	static void* external_alloc(size_t size);

	static void external_free(void *blk);
	
	void* internal_alloc(size_t size);

	void internal_free(void* block);

	void updateSpare();
	
	void addFreeBlock(MemoryBlock *blk);
		
	void removeFreeBlock(MemoryBlock *blk);
	
	// int_XXX functions do all the stuff except locking and exceptions
	void* int_alloc(size_t size, SSHORT type = 0
#ifdef DEBUG_GDS_ALLOC
		, char *file = NULL, int line = 0
#endif
	);

public:
	static MemoryPool* createPool();
	
	static MemoryPool* getProcessPool();

	static void deletePool(MemoryPool* pool);

	void* alloc(size_t size, SSHORT type = 0
#ifdef DEBUG_GDS_ALLOC
		, char *file = NULL, int line = 0
#endif
	);

	void free(void *block);
	
	void verify_pool();

	void print_pool(IB_FILE *, bool = false);
	
	static void globalFree(void *block) {
		((MemoryBlock*)((char*)block-MEM_ALIGN(sizeof(MemoryBlock))))->pool->free(block);
	}
	
	void* calloc(size_t size, SSHORT type = 0
#ifdef DEBUG_GDS_ALLOC
		, char *file = NULL, int line = 0
#endif
	) {
		void* result = alloc(size, type
#ifdef DEBUG_GDS_ALLOC
			, file, line
#endif
		);
		memset(result,size,0);
		return result;	
	}

	friend class InternalAllocator;
};

}; // namespace Firebird

#ifndef TESTING_ONLY

extern "C" {
#ifdef DEBUG_GDS_ALLOC
void* API_ROUTINE gds__alloc_debug(SLONG size_request,
                                   TEXT* filename,
                                   ULONG lineno);
#else
void* API_ROUTINE gds__alloc(SLONG size_request);
#endif
ULONG API_ROUTINE gds__free(void* blk);
};

// Global versions of operator new() for compatibility with crappy libraries
void* operator new(size_t);
void* operator new[](size_t);

#ifdef DEBUG_GDS_ALLOC
inline void* operator new(size_t s, Firebird::MemoryPool* pool, char* file, int line) {
	return pool->alloc(s, 0, file, line);
}
inline void* operator new[](size_t s, Firebird::MemoryPool* pool, char* file, int line) {
	return pool->alloc(s, 0, file, line);
}
#define FB_NEW(pool) new(pool,__FILE__,__LINE__)
#define FB_NEW_RPT(pool,count) new(pool,count,__FILE__,__LINE__)
#else
inline void* operator new(size_t s, Firebird::MemoryPool* pool) {
	return pool->alloc(s);
}
inline void* operator new[](size_t s, Firebird::MemoryPool* pool) {
	return pool->alloc(s);
}
#define FB_NEW(pool) new(pool)
#define FB_NEW_RPT(pool,count) new(pool,count)
#endif

inline void operator delete(void* mem) {
	Firebird::MemoryPool::globalFree(mem);
}
inline void operator delete[](void* mem) {
	Firebird::MemoryPool::globalFree(mem);
}

#endif

#endif
