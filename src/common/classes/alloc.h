/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		alloc.h
 *	DESCRIPTION:	Memory Pool Manager (based on B+ tree)
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
 *  The Original Code was created by Nickolay Samofatov
 *  for the Firebird Open Source RDBMS project.
 *
 *  STL allocator is based on one by Mike Nordell and John Bellardo
 *
 *  Copyright (c) 2004 Nickolay Samofatov <nickolay@broadviewsoftware.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *
 *  Contributor(s):
 * 
 *		Alex Peshkoff <peshkoff@mail.ru>
 *				added PermanentStorage and AutoStorage classes.
 *
 *  $Id: alloc.h,v 1.53 2004-10-25 05:14:05 skidder Exp $
 *
 */

#ifndef CLASSES_ALLOC_H
#define CLASSES_ALLOC_H

#include <cstddef>

#include "../../include/fb_types.h"
#include "../../include/firebird.h"
#include <stdio.h>
#include "../jrd/common.h"
#include "../common/classes/fb_atomic.h"
#include "../common/classes/tree.h"
#include "../common/classes/locks.h"
#ifdef HAVE_STDLIB_H
#include <stdlib.h> /* XPG: prototypes for malloc/free have to be in
					   stdlib.h (EKU) */
#endif

#ifdef _MSC_VER
#define THROW_BAD_ALLOC
#else
#define THROW_BAD_ALLOC throw (std::bad_alloc)
#endif

#ifdef USE_VALGRIND

// Size of Valgrind red zone applied before and after memory block allocated for user
#define VALGRIND_REDZONE 8

// When memory block is deallocated by user from the pool it must pass queue of this 
// length before it is actually deallocated and access protection from it removed.
#define DELAYED_FREE_COUNT 1024

// When memory extent is deallocated when pool is destroying it must pass through
// queue of this length before it is actually returned to system
#define DELAYED_EXTENT_COUNT 32

#endif

namespace Firebird {

// Maximum number of B+ tree pages kept spare for tree allocation
// Since we store only unique fragment lengths in our tree there 
// shouldn't be more than 16K elements in it. This is why MAX_TREE_DEPTH 
// equal to 4 is more than enough
const int MAX_TREE_DEPTH = 4;

// Alignment for all memory blocks. Sizes of memory blocks in headers are measured in this units
const size_t ALLOC_ALIGNMENT = ALIGNMENT;

static inline size_t MEM_ALIGN(size_t value) {
	return FB_ALIGN(value, ALLOC_ALIGNMENT);
}

// Flags for memory block
const USHORT MBK_LARGE = 1; // Block is large, allocated from OS directly
const USHORT MBK_PARENT = 2; // Block is allocated from parent pool
const USHORT MBK_USED = 4; // Block is used
const USHORT MBK_LAST = 8; // Block is last in the extent
const USHORT MBK_DELAYED = 16; // Block is pending in the delayed free queue

struct FreeMemoryBlock {
	FreeMemoryBlock* fbk_next_fragment;
};

// Block header.
// Has size of 12 bytes for 32-bit targets and 16 bytes on 64-bit ones
struct MemoryBlock {
	USHORT mbk_flags;
	SSHORT mbk_type;
	union {
		struct {
		  // Length and offset are measured in bytes thus memory extent size is limited to 64k
		  // Larger extents are not needed now, but this may be icreased later via using allocation units 
		  USHORT mbk_length; // Actual block size: header not included, redirection list is included if applicable
		  USHORT mbk_prev_length;
		} small;
		// Measured in bytes
		ULONG mbk_large_length;
	};
#ifdef DEBUG_GDS_ALLOC
	const char* mbk_file;
	int mbk_line;
#endif
	union {
		class MemoryPool* mbk_pool;
		FreeMemoryBlock* mbk_prev_fragment;
	};
#if defined(USE_VALGRIND) && (VALGRIND_REDZONE != 0)
	const char mbk_valgrind_redzone[VALGRIND_REDZONE];
#endif
};


// This structure is appended to the end of block redirected to parent pool or operating system
// It is a doubly-linked list which we are going to use when our pool is going to be deleted
struct MemoryRedirectList {
	MemoryBlock* mrl_prev;
	MemoryBlock* mrl_next;
};

const SSHORT TYPE_POOL = -1;
const SSHORT TYPE_EXTENT = -2;
const SSHORT TYPE_LEAFPAGE = -3;
const SSHORT TYPE_TREEPAGE = -4;

// We store BlkInfo structures instead of BlkHeader pointers to get benefits from 
// processor cache-hit optimizations
struct BlockInfo {
	size_t bli_length;
	FreeMemoryBlock* bli_fragments;
	inline static const size_t& generate(const void* sender, const BlockInfo& i) {
		return i.bli_length;
	}
};

struct MemoryExtent {
	MemoryExtent *mxt_next;
	MemoryExtent *mxt_prev;
};

struct PendingFreeBlock {
	PendingFreeBlock *next;
};

class MemoryStats {
public:
	MemoryStats() : mst_usage(0), mst_mapped(0), mst_max_usage(0), mst_max_mapped(0) {}
	~MemoryStats() {}
	size_t get_current_usage() const { return mst_usage.value(); }
	size_t get_maximum_usage() const { return mst_max_usage; }
	size_t get_current_mapping() const { return mst_mapped.value(); }
	size_t get_maximum_mapping() const { return mst_max_mapped; }
private:
	// Forbid copy constructor
	MemoryStats(const MemoryStats& object) {}

	// Currently allocated memory (without allocator overhead)
	// Useful for monitoring engine memory leaks
	AtomicCounter mst_usage;
	// Amount of memory mapped (including all overheads)
	// Useful for monitoring OS memory consumption
	AtomicCounter mst_mapped;
	
	// We don't particularily care about extreme precision of these max values,
	// this is why we don't synchronize them on Windows
	size_t mst_max_usage;
	size_t mst_max_mapped;
	
	friend class MemoryPool;	
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
	typedef BePlusTree<BlockInfo, size_t, InternalAllocator, BlockInfo> FreeBlocksTree;
	
	// We keep most of our structures uninitialized as long we redirect 
	// our allocations to parent pool
	bool parent_redirect;

	// B+ tree ordered by length 
	FreeBlocksTree freeBlocks;

	MemoryExtent *extents; // Linked list of all memory extents

	Vector<void*, 2> spareLeafs;
	Vector<void*, MAX_TREE_DEPTH + 1> spareNodes;
	bool needSpare;
	PendingFreeBlock *pendingFree;

    // Synchronization of this object is a little bit tricky. Allocations 
	// redirected to parent pool are not protected with our mutex and not 
	// accounted locally, i.e. redirect_amount and parent_redirected linked list
	// are synchronized with parent pool mutex only. All other pool members are 
	// synchronized with this mutex.
	Mutex lock;
	
	// Current usage counters for pool. Used to move pool to different statistics group
	AtomicCounter used_memory;

	size_t mapped_memory;

	MemoryPool *parent; // Parent pool. Used to redirect small allocations there
	MemoryBlock *parent_redirected, *os_redirected;
	size_t redirect_amount; // Amount of memory redirected to parent
							// It is protected by parent pool mutex along with redirect list
	// Statistics group for the pool
	MemoryStats *stats;
	
#ifdef USE_VALGRIND
	// Circular FIFO buffer of read/write protected blocks pending free operation
	void* delayedFree[DELAYED_FREE_COUNT];
	int delayedFreeHandles[DELAYED_FREE_COUNT];
	size_t delayedFreeCount;
	size_t delayedFreePos;
#endif

	/* Returns NULL in case it cannot allocate requested chunk */
	static void* external_alloc(size_t &size);

	static void external_free(void* blk, size_t &size, bool pool_destroying);
	
	void* tree_alloc(size_t size);

	void tree_free(void* block);

	void updateSpare();
	
	inline void addFreeBlock(MemoryBlock* blk);
		
	void removeFreeBlock(MemoryBlock* blk);
	
	void free_blk_extent(MemoryBlock* blk);
	
	// Allocates small block from this pool. Pool must be locked during call
	void* internal_alloc(size_t size, SSHORT type = 0
#ifdef DEBUG_GDS_ALLOC
		, const char* file = NULL, int line = 0
#endif
	);

	// Deallocates small block from this pool. Pool must be locked during this call
	void internal_deallocate(void* block);
	
	// Forbid copy constructor, should never be called
	MemoryPool(const MemoryPool& pool) : freeBlocks((InternalAllocator*)this) { }
	
	// Used by pools to track memory usage. 

	// These 2 methods are thread-safe due to usage of atomic counters only
	inline void increment_usage(size_t size);
	inline void decrement_usage(size_t size);

	inline void increment_mapping(size_t size);
	inline void decrement_mapping(size_t size);
	
protected:
	// Do not allow to create and destroy pool directly from outside
	MemoryPool(MemoryPool* _parent, MemoryStats &_stats, void* first_extent, void* root_page);

	// This should never be called
	~MemoryPool() {
	}
	
	// Used to create MemoryPool descendants
	static MemoryPool* internal_create(size_t instance_size, 
		MemoryPool* parent = NULL, MemoryStats &stats = default_stats_group);
	
public:
	// Default statistics group for process
	static MemoryStats default_stats_group;

	// Pool created for process
	static MemoryPool* processMemoryPool;
	
	// Create memory pool instance
	static MemoryPool* createPool(MemoryPool* parent = NULL, MemoryStats &stats = default_stats_group) {
		return internal_create(sizeof(MemoryPool), parent, stats);
	}
	
	// Set context pool for current thread of execution
	static MemoryPool* setContextPool(MemoryPool *newPool);
	
	// Get context pool for current thread of execution
	static MemoryPool* getContextPool();
	
	// Set statistics group for pool. Usage counters will be decremented from 
	// previously set group and added to new
	void setStatsGroup(MemoryStats &stats);

	// Deallocate pool and all its contents
	static void deletePool(MemoryPool* pool);

	// Allocate memory block. Result is not zero-initialized.
	// It case of problems this method throws std::bad_alloc
	void* allocate(size_t size, SSHORT type = 0
#ifdef DEBUG_GDS_ALLOC
		, const char* file = NULL, int line = 0
#endif
	);

	// Allocate memory block. In case of problems this method returns NULL
	void* allocate_nothrow(size_t size, SSHORT type = 0
#ifdef DEBUG_GDS_ALLOC
		, const char* file = NULL, int line = 0
#endif
	);
	
	void deallocate(void* block);
	
	// Check pool for internal consistent. When enabled, call is very expensive
	bool verify_pool(bool fast_checks_only = false);

	// Print out pool contents. This is debugging routine
	void print_contents(FILE*, bool = false);
	
	// The same routine, but more easily callable from the debugger
	void print_contents(const char* filename, bool = false);
	
	// Deallocate memory block. Pool is derived from block header
	static void globalFree(void* block) {
	    if (block)
		  ((MemoryBlock*)((char*)block - MEM_ALIGN(sizeof(MemoryBlock))))->mbk_pool->deallocate(block);
	}
	
	// Allocate zero-initialized block of memory
	void* calloc(size_t size, SSHORT type = 0
#ifdef DEBUG_GDS_ALLOC
		, const char* file = NULL, int line = 0
#endif
	) {
		void* result = allocate(size, type
#ifdef DEBUG_GDS_ALLOC
			, file, line
#endif
		);
		memset(result, 0, size);
		return result;	
	}

	/// Returns the type associated with the allocated memory.
	static SSHORT blk_type(const void* mem) {
		return ((MemoryBlock*)((char *)mem - MEM_ALIGN(sizeof(MemoryBlock))))->mbk_type;
	}
	
	/// Returns the pool the memory was allocated from.
	//static MemoryPool* blk_pool(const void* mem) {
	//	return ((MemoryBlock*)((char *)mem - MEM_ALIGN(sizeof(MemoryBlock))))->mbk_pool;
	//}
	
	friend class InternalAllocator;
};

// Class intended to manage execution context pool stack
// Declare instance of this class when you need to set new context pool and it 
// will be restored automatically as soon holder variable gets out of scope
class ContextPoolHolder {
public:
	ContextPoolHolder(MemoryPool* newPool) {
		savedPool = MemoryPool::setContextPool(newPool);
	}
	~ContextPoolHolder() {
		MemoryPool::setContextPool(savedPool);
	}
private:
	MemoryPool* savedPool;	
};

// template enabling common use of old and new pools control code
// to be dropped when old-style code goes away
template <typename SubsystemThreadData, typename SubsystemPool>
class SubsystemContextPoolHolder
: public ContextPoolHolder
{
public:
	SubsystemContextPoolHolder <SubsystemThreadData, SubsystemPool> 
	(
		SubsystemThreadData* subThreadData, 
		SubsystemPool* newPool
	) 
		: ContextPoolHolder(newPool), 
		savedThreadData(subThreadData),
		savedPool(savedThreadData->getDefaultPool()) 
	{
		savedThreadData->setDefaultPool(newPool);
	}
	~SubsystemContextPoolHolder() {
		savedThreadData->setDefaultPool(savedPool);
	}
private:
	SubsystemThreadData* savedThreadData;
	SubsystemPool* savedPool;
};

} // namespace Firebird

using Firebird::MemoryPool;

inline static MemoryPool* getDefaultMemoryPool() { return Firebird::MemoryPool::processMemoryPool; }

MemoryPool* getContextMemoryPool();

// Global versions of operator new()
// Implemented in alloc.cpp
void* operator new(size_t) THROW_BAD_ALLOC;
void* operator new[](size_t) THROW_BAD_ALLOC;

// We cannot use inline versions because we have to replace STL delete defined in <new> header
void operator delete(void* mem) throw();
void operator delete[](void* mem) throw();

#ifdef DEBUG_GDS_ALLOC
static inline void* operator new(size_t s, Firebird::MemoryPool& pool, const char* file, int line) {
	return pool.allocate(s, 0, file, line);
}
static inline void* operator new[](size_t s, Firebird::MemoryPool& pool, const char* file, int line) {
	return pool.allocate(s, 0, file, line);
}
#define FB_NEW(pool) new(pool,__FILE__,__LINE__)
#define FB_NEW_RPT(pool,count) new(pool,count,__FILE__,__LINE__)
#else
static inline void* operator new(size_t s, Firebird::MemoryPool& pool) {
	return pool.allocate(s);
}
static inline void* operator new[](size_t s, Firebird::MemoryPool& pool) {
	return pool.allocate(s);
}
#define FB_NEW(pool) new(pool)
#define FB_NEW_RPT(pool,count) new(pool,count)
#endif


/**
	This is the allocator template provided to be used with the STL.
	Since the STL is the client of this class look to its documentation
	to determine what the individual functions and typedefs do.

	In order to use the allocator class you need to instanciate the
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

#ifndef TESTING_ONLY

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
		allocator(MemoryPool* p = getDefaultMemoryPool(), SSHORT t = 0) : pool(p), type(t) {}
	
		template <class DST>
		allocator(const allocator<DST> &alloc)
			: pool(alloc.getPool()), type(alloc.getType()) { }

#ifdef DEBUG_GDS_ALLOC
		pointer allocate(size_type s, const void* = 0)
			{ return (pointer) pool->allocate(sizeof(T) * s, 0, __FILE__, __LINE__); }
		char* _Charalloc(size_type n)
			{ return (char*) pool->allocate(n, 0, __FILE__, __LINE__); }
#else
		pointer allocate(size_type s, const void* = 0)
			{ return (pointer) pool->allocate(sizeof(T) * s, 0); }
		char* _Charalloc(size_type n)
			{ return (char*) pool->allocate(n, 0); }
#endif
			
		void deallocate(pointer p, size_type s)	{ pool->deallocate(p); }
		void deallocate(void* p, size_type s) { pool->deallocate(p); }
		void construct(pointer p, const T& v) { new(p) T(v); }
		void destroy(pointer p) { p->~T(); }
	
		size_type max_size() const { return (size_type) - 1 / sizeof(T); }
	
		pointer address(reference X) const { return &X; }
		const_pointer address(const_reference X) const { return &X; }
	
		template <class _Tp1> struct rebind {
			typedef Firebird::allocator<_Tp1> other;
		};

		bool operator==(const allocator<T>& rhs) const
		{
			return pool == rhs.pool && type == rhs.type;
		}

		MemoryPool* getPool() const { return pool; }
		SSHORT getType() const { return type; }

	private:
		MemoryPool* pool;
		SSHORT type;
	};

#endif /*TESTING_ONLY*/

	// Permanent storage is used as base class for all objects,
	// performing memory allocation in methods other than 
	// constructors of this objects. Permanent means that pool,
	// which will be later used for such allocations, must
	// be explicitly passed in all constructors of such object.
	class PermanentStorage {
	private:
		MemoryPool& pool;
	protected:
		explicit PermanentStorage(MemoryPool& p) : pool(p) { }
		MemoryPool& getPool() const { return pool; }
	};

	// Automatic storage is used as base class for objects,
	// that may have constructors without explicit MemoryPool
	// parameter. In this case AutoStorage sends AutoMemoryPool
	// to PermanentStorage. To ensure this operation to be safe
	// such trick possible only for local (on stack) variables.
	class AutoStorage : public PermanentStorage {
	private:
#if defined(DEV_BUILD)
		void ProbeStack() const;
#endif
	public:
		static MemoryPool& getAutoMemoryPool() { return *getDefaultMemoryPool(); }
	protected:
		AutoStorage() : PermanentStorage(getAutoMemoryPool()) {
#if defined(DEV_BUILD)
			ProbeStack();
#endif
		}
		explicit AutoStorage(MemoryPool& p) : PermanentStorage(p) { }
	};
	
} // namespace Firebird


#endif // CLASSES_ALLOC_H

