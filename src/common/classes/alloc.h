/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		alloc.h
 *	DESCRIPTION:	Memory Pool Manager (based on B+ tree)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * You may obtain a copy of the Licence at
 * http://www.gnu.org/licences/lgpl.html
 * 
 * As a special exception this file can also be included in modules
 * with other source code as long as that source code has been 
 * released under an Open Source Initiative certificed licence.  
 * More information about OSI certification can be found at: 
 * http://www.opensource.org 
 * 
 * This module is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public Licence for more details.
 * 
 * This module was created by members of the firebird development 
 * team.  All individual contributions remain the Copyright (C) of 
 * those individuals and all rights are reserved.  Contributors to 
 * this file are either listed below or can be obtained from a CVS 
 * history command.
 *
 *  Created by: Nickolay Samofatov <skidder@bssys.com>
 *             
 *  STL allocator is based on one by Mike Nordell and John Bellardo
 *
 *  Contributor(s):
 * 
 *
 *  $Id: alloc.h,v 1.25 2003-09-08 20:23:32 skidder Exp $
 *
 */

#ifndef ALLOC_H
#define ALLOC_H

#include "../../include/fb_types.h"
#include "../../include/firebird.h"
#include "../jrd/common.h"
#include "../jrd/ib_stdio.h"
#include "tree.h"
#include "locks.h"
#ifdef HAVE_STDLIB_H
#include <stdlib.h> /* XPG: prototypes for malloc/free have to be in
					   stdlib.h (EKU) */
#endif
#ifdef _MSC_VER
#define THROW_BAD_ALLOC
#else
#define THROW_BAD_ALLOC throw (std::bad_alloc)
#endif

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
	MemoryBlock *prev;
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
//
// MemoryPool inheritance looks weird because we cannot use
// any pointers to functions in shared memory. VMT usage in
// MemoryPool and its descendants is prohibited
class MemoryPool {
private:
	class InternalAllocator {
	public:
		void* allocate(size_t size) {
			return ((MemoryPool*)this)->tree_alloc(size);
		}
		void deallocate(void* block) {
			((MemoryPool*)this)->tree_free(block);
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
#ifdef MULTI_THREAD
	Spinlock lock;
#else
	SharedSpinlock lock;
#endif
	int extents_memory; // Sum of memory in allocated extents minus size of extents headers
	int used_memory; // Size of used memory blocks including block headers

	/* Returns NULL in case it cannot allocate requested chunk */
	static void* external_alloc(size_t size);

	static void external_free(void *blk);
	
	void* tree_alloc(size_t size);

	void tree_free(void* block);

	void updateSpare();
	
	void addFreeBlock(MemoryBlock *blk);
		
	void removeFreeBlock(MemoryBlock *blk);
	
	void free_blk_extent(MemoryBlock *blk);
	
	// does all the stuff except locking and exceptions
	void* internal_alloc(size_t size, SSHORT type = 0
#ifdef DEBUG_GDS_ALLOC
		, char *file = NULL, int line = 0
#endif
	);
protected:
	int *cur_memory;
	int *max_memory;
	// Do not allow to create and destroy pool directly from outside
	MemoryPool(void *first_extent, void *root_page, int* cur_mem = NULL, int* max_mem = NULL);

	// This should never be called
	~MemoryPool() {
	}
	
	static MemoryPool* internal_create(size_t instance_size, 
		int *cur_mem = NULL, int *max_mem = NULL);
public:
	static int process_max_memory;
	static int process_current_memory;

	// Move usage stats to another location
	void moveStats(int *cur_mem, int *max_mem) {
		*cur_mem = *cur_memory;
		*max_mem = *max_memory;
		cur_memory = cur_mem;
		max_memory = max_mem;
	}
	static MemoryPool* createPool() {
		return internal_create(sizeof(MemoryPool));
	}

	static void deletePool(MemoryPool* pool);

	void* allocate(size_t size, SSHORT type = 0
#ifdef DEBUG_GDS_ALLOC
		, char *file = NULL, int line = 0
#endif
	);

	void deallocate(void *block);
	
	bool verify_pool();

	void print_contents(IB_FILE *, bool = false);
	
	static void globalFree(void *block) {
	    if (block)
		  ((MemoryBlock*)((char*)block-MEM_ALIGN(sizeof(MemoryBlock))))->pool->deallocate(block);
	}
	
	void* calloc(size_t size, SSHORT type = 0
#ifdef DEBUG_GDS_ALLOC
		, char *file = NULL, int line = 0
#endif
	) {
		void* result = allocate(size, type
#ifdef DEBUG_GDS_ALLOC
			, file, line
#endif
		);
		memset(result,0,size);
		return result;	
	}

	/// Returns the type associated with the allocated memory.
	static SSHORT blk_type(const void* mem) {
		return ((MemoryBlock*)((char *)mem - MEM_ALIGN(sizeof(MemoryBlock))))->type;
	}
	
	/// Returns the pool the memory was allocated from.
	static MemoryPool* blk_pool(const void* mem) {
		return ((MemoryBlock*)((char *)mem - MEM_ALIGN(sizeof(MemoryBlock))))->pool;
	}
	
	friend class InternalAllocator;
};

}; // namespace Firebird

#ifndef TESTING_ONLY

using Firebird::MemoryPool;

MemoryPool* getDefaultMemoryPool();

// Global versions of operator new() for compatibility with crappy libraries
void* operator new(size_t) THROW_BAD_ALLOC;
void* operator new[](size_t) THROW_BAD_ALLOC;

#ifdef DEBUG_GDS_ALLOC
inline void* operator new(size_t s, Firebird::MemoryPool& pool, char* file, int line) {
	return pool.allocate(s, 0, file, line);
//	return pool.calloc(s, 0, file, line);
}
inline void* operator new[](size_t s, Firebird::MemoryPool& pool, char* file, int line) {
	return pool.allocate(s, 0, file, line);
//	return pool.calloc(s, 0, file, line);
}
#define FB_NEW(pool) new(pool,__FILE__,__LINE__)
#define FB_NEW_RPT(pool,count) new(pool,count,__FILE__,__LINE__)
#else
inline void* operator new(size_t s, Firebird::MemoryPool& pool) {
	return pool.allocate(s);
//	return pool.calloc(s);
}
inline void* operator new[](size_t s, Firebird::MemoryPool& pool) {
	return pool.allocate(s);
//	return pool.calloc(s);
}
#define FB_NEW(pool) new(pool)
#define FB_NEW_RPT(pool,count) new(pool,count)
#endif


// We cannot use inline versions because we have to replace STL delete defined in <new> header
// One more performance pain we have to take because of STL usage :((
void operator delete(void* mem) throw();

void operator delete[](void* mem) throw();

/**
	This is the allocator template provided to be used with the STL.
	Since the STL is the client of this class look to its documentation
	to determine what the individual functions and typedefs do.

	In order to use the allocator class you need to instansiate the
	C++ container template with the allocator.  For example if you
	want to use a std::vector<int> the declaration would be:

	std::vector<int, MemoryPool::allocator<int> >

	The allocator, by default, allocates all memory from the process
	wide pool FB_MemoryPool.  Typically this is NOT the behavior you
	want.  Selection of the correct pool to allocate your memory from is
	important.  If you select a pool to far down in (a statement pool,
	for example) you memory may be freed before you are done with it.
	On the other hand if you always use the global pool you will
	either leak memory or have to make sure you always delete the objects
	you create.

	If you decide to allocate the memory from a pool other than the global
	pool you need to pass an allocator object to the constructor for
	the STL object.  For example:

	std::vector<int, MemoryPool::allocator<int> > vec(MemoryPool::allocator<int>(poolRef, type));
	The type is an optional parameter that defaults to 0.
**/
namespace Firebird
{
	template <class T>
	class allocator
	{
		public:
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef T			value_type;
	
		allocator(MemoryPool& p, SSHORT t = 0) : pool(&p), type(t) {}
		allocator(MemoryPool *p = getDefaultMemoryPool(), SSHORT t = 0) : pool(p), type(t) {}
	
		template <class DST>
		allocator(const allocator<DST> &alloc)
		: pool(alloc.getPool()), type(alloc.getType()) { }

#ifdef DEBUG_GDS_ALLOC
		pointer allocate(size_type s, const void * = 0)
			{ return (pointer) pool->allocate(sizeof(T) * s, 0, __FILE__, __LINE__); }
		char *_Charalloc(size_type n)
			{ return (char*) pool->allocate(n, 0, __FILE__, __LINE__); }
#else
		pointer allocate(size_type s, const void * = 0)
			{ return (pointer) pool->allocate(sizeof(T) * s, 0); }
		char *_Charalloc(size_type n)
			{ return (char*) pool->allocate(n, 0); }
#endif
/*#ifdef DEBUG_GDS_ALLOC
		pointer allocate(size_type s, const void * = 0)
			{ return (pointer) pool->calloc(sizeof(T) * s, 0, __FILE__, __LINE__); }
		char *_Charalloc(size_type n)
			{ return (char*) pool->calloc(n, 0, __FILE__, __LINE__); }
#else
		pointer allocate(size_type s, const void * = 0)
			{ return (pointer) pool->calloc(sizeof(T) * s, 0); }
		char *_Charalloc(size_type n)
			{ return (char*) pool->calloc(n, 0); }
#endif*/
			
		void deallocate(pointer p, size_type s)	{ pool->deallocate(p); }
		void deallocate(void* p, size_type s) { pool->deallocate(p); }
		void construct(pointer p, const T& v) { new(p) T(v); }
		void destroy(pointer p) { p->~T(); }
	
		size_type max_size() const { return (size_type)-1 / sizeof(T); }
	
		pointer address(reference X) const { return &X; }
		const_pointer address(const_reference X) const { return &X; }
	
		template <class _Tp1> struct rebind {
			typedef Firebird::allocator<_Tp1> other;
		};

		bool operator==(const allocator<T>& rhs) const
		{
			return pool == rhs.pool && type == rhs.type;
		}

		MemoryPool *getPool() const { return pool; }
		SSHORT getType() const { return type; }

	private:
		MemoryPool *pool;
		SSHORT type;
	};

};

#endif /*TESTING_ONLY*/

#endif
