/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		alloc.cpp
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
 *  Copyright (c) 2004 Nickolay Samofatov <nickolay@broadviewsoftware.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *  $Id: alloc.cpp,v 1.55 2004-06-30 01:26:06 skidder Exp $
 *
 */

#include "firebird.h"
#include "../common/classes/alloc.h"
#include "../common/classes/fb_tls.h"
#include "../jrd/gdsassert.h"
#ifdef HAVE_MMAP
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#endif

// Fill blocks with patterns
#define FREE_PATTERN 0xDEADBEEF
#define ALLOC_PATTERN 0xFEEDABED
#ifdef DEBUG_GDS_ALLOC
# define PATTERN_FILL(ptr, size, pattern) for (size_t _i = 0; _i < size / sizeof(unsigned int); _i++) ((unsigned int*)(ptr))[_i] = (pattern)
#else
# define PATTERN_FILL(ptr, size, pattern) ((void)0)
#endif

// TODO (in order of importance):
// 1. local pool locking +
// 2. line number debug info +
// 3. debug alloc/free pattern +
// 4. print pool contents function +
//---- Not needed for current codebase
// 5. Pool size limit
// 6. allocation source +
// 7. shared pool locking
// 8. red zones checking (not really needed because verify_pool is able to detect most corruption cases)

/****************************** Local declarations *****************************/

namespace {

using namespace Firebird;

inline static void mem_assert(bool value)
{
	if (!value) abort();
}

// Returns redirect list for given memory block
inline MemoryRedirectList* block_list_small(MemoryBlock* block)
{
    return (MemoryRedirectList*)((char*)block + MEM_ALIGN(sizeof(MemoryBlock)) + 
		block->small.mbk_length - MEM_ALIGN(sizeof(MemoryRedirectList)));
}

inline const MemoryRedirectList* block_list_small(const MemoryBlock* block)
{
    return (const MemoryRedirectList*)((const char*)block + MEM_ALIGN(sizeof(MemoryBlock)) +
		block->small.mbk_length - MEM_ALIGN(sizeof(MemoryRedirectList)));
}


inline MemoryRedirectList* block_list_large(MemoryBlock* block)
{
    return (MemoryRedirectList*)((char*)block + MEM_ALIGN(sizeof(MemoryBlock)) + 
		block->mbk_large_length - MEM_ALIGN(sizeof(MemoryRedirectList)));
}

inline const MemoryRedirectList* block_list_large(const MemoryBlock* block)
{
    return (const MemoryRedirectList*)((const char*)block + MEM_ALIGN(sizeof(MemoryBlock)) +
		block->mbk_large_length - MEM_ALIGN(sizeof(MemoryRedirectList)));
}


// Returns block header from user block pointer
inline MemoryBlock* ptr_block(void *ptr)
{
    return (MemoryBlock*)((char*)ptr - MEM_ALIGN(sizeof(MemoryBlock)));
}

// Returns previos block in extent. Doesn't check that next block exists
inline MemoryBlock* prev_block(MemoryBlock *block)
{
	return (MemoryBlock*)((char*)block - block->small.mbk_prev_length - MEM_ALIGN(sizeof(MemoryBlock)));
}

// Returns next block in extent. Doesn't check that previous block exists
inline MemoryBlock* next_block(MemoryBlock *block)
{
	return (MemoryBlock*)((char*)block + block->small.mbk_length + MEM_ALIGN(sizeof(MemoryBlock)));
}

inline const MemoryBlock* next_block(const MemoryBlock *block)
{
	return (const MemoryBlock*)((const char*)block + block->small.mbk_length + MEM_ALIGN(sizeof(MemoryBlock)));
}

inline size_t FB_MAX(size_t M, size_t N)
{
	return M > N ? M : N;
}
  
// Size in bytes, must be aligned according to ALLOC_ALIGNMENT
// It should also be a multiply of page size
const size_t EXTENT_SIZE = 65536;
// We cache this amount of extents to avoid memory mapping overhead
const int MAP_CACHE_SIZE = 16; // == 1 MB
// Size of pool to start its own mapping and stop redirecting allocations to parent
// For current implementation it should be smaller then maximum block size which can fit in extent
const size_t REDIRECT_THRESHOLD = 32768;

// Declare thread-specific variable for context memory pool
TLS_DECLARE(MemoryPool*, contextPool);

// Helper function to reduce code size, since many compilers
// generate quite a bit of code at the point of the throw.
void pool_out_of_memory()
{
	throw std::bad_alloc();
}

// Support for memory mapping facilities
#if defined(WIN_NT)
size_t get_page_size()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return info.dwPageSize;
}
#elif defined(HAVE_MMAP)
size_t get_page_size()
{
	return sysconf(_SC_PAGESIZE);
}
#endif

#if !defined(MAP_ANONYMOUS) && defined(MAP_ANON)
# define MAP_ANONYMOUS MAP_ANON
#endif

#if defined(HAVE_MMAP) && !defined(MAP_ANONYMOUS)
int dev_zero_fd = -1; /* Cached file descriptor for /dev/zero. */
#endif

#if defined(WIN_NT) || defined(HAVE_MMAP)
const size_t map_page_size = get_page_size();
Firebird::Vector<void*, MAP_CACHE_SIZE> extents_cache;
Mutex cache_mutex;
#endif

} // namespace

namespace Firebird {

/****************************** Firebird::MemoryPool ***************************/

inline void MemoryPool::increment_usage(size_t size)
{
	const size_t temp = stats->mst_usage += size;
	if (temp > stats->mst_max_usage)
		stats->mst_max_usage = temp;
	used_memory += size;
}

inline void MemoryPool::decrement_usage(size_t size)
{
	stats->mst_usage -= size;
	used_memory -= size;
}

inline void MemoryPool::increment_mapping(size_t size)
{
	const size_t temp = stats->mst_mapped += size;
	if (temp > stats->mst_max_mapped)
		stats->mst_max_mapped = temp;
	mapped_memory += size;
}

inline void MemoryPool::decrement_mapping(size_t size)
{
	stats->mst_mapped -= size;
	mapped_memory -= size;
}

MemoryPool* MemoryPool::setContextPool(MemoryPool *newPool)
{
	MemoryPool* const old = TLS_GET(contextPool);
	TLS_SET(contextPool, newPool);
	return old;
}
	
MemoryPool* MemoryPool::getContextPool()
{
	return TLS_GET(contextPool);
}

// Initialize default stats group
MemoryStats MemoryPool::default_stats_group;

// Initialize process memory pool to avoid possible race conditions
MemoryPool* MemoryPool::processMemoryPool = MemoryPool::createPool();


void MemoryPool::setStatsGroup(MemoryStats &statsL)
{
	lock.enter();
	const size_t sav_used_memory = used_memory;
	const size_t sav_mapped_memory = mapped_memory;
	decrement_mapping(sav_mapped_memory);
	decrement_usage(sav_used_memory);
	this->stats = &statsL;
	increment_mapping(sav_mapped_memory);
	increment_usage(sav_used_memory);	
	lock.leave();
}

MemoryPool::MemoryPool(MemoryPool* parentL, MemoryStats &statsL,
	void *first_extent, void *root_page) :
	parent_redirect(parentL != NULL),
	freeBlocks((InternalAllocator*)this, root_page),
	extents((MemoryExtent *)first_extent), 
	needSpare(false),
	pendingFree(NULL),
	mapped_memory(0),
	used_memory(0),
	parent(parentL),
	parent_redirected(NULL),
	os_redirected(NULL),
	redirect_amount(0),
	stats(&statsL)
{
}

void MemoryPool::updateSpare()
{
	// Pool does not maintain its own allocation mechanizms if it redirects allocations to parent
	fb_assert(!parent_redirect);
	do {
		// Try to allocate a number of pages to return tree to usable state (when we are able to add blocks safely there)
		// As a result of this operation we may get some extra blocks in pendingFree list
		while (spareLeafs.getCount() < spareLeafs.getCapacity()) {
			void* temp = internal_alloc(MEM_ALIGN(sizeof(FreeBlocksTree::ItemList)), TYPE_LEAFPAGE);
			if (!temp)
				return;
			spareLeafs.add(temp);
		}
		while (spareNodes.getCount() <= freeBlocks.level) {
			void* temp = internal_alloc(MEM_ALIGN(sizeof(FreeBlocksTree::NodeList)), TYPE_TREEPAGE);
			if (!temp)
				return;
			spareNodes.add(temp);
		}
		needSpare = false;

		// Great, if we were able to restore free blocks tree operations after critically low
		// memory condition then try to add pending free blocks to our tree
		while (pendingFree) {
			PendingFreeBlock *temp = pendingFree;
			pendingFree = temp->next;
			// Blocks added with tree_deallocate may require merging with nearby ones
			// This is why we do internal_deallocate
			internal_deallocate(temp); // Note that this method may change pendingFree!
			
			if (needSpare)
				break; // New pages were added to tree. Loop again
		}
	} while (needSpare);
}

void* MemoryPool::external_alloc(size_t &size)
{
	// This method is assumed to return NULL in case it cannot alloc
#if defined(WIN_NT) || defined(HAVE_MMAP)
	if (size == EXTENT_SIZE) {
		cache_mutex.enter();
		void *result = NULL;
		if (extents_cache.getCount()) {
			// Use recently used object object to encourage caching
			result = extents_cache[extents_cache.getCount() - 1];
			extents_cache.shrink(extents_cache.getCount() - 1);
		}
		cache_mutex.leave();
		if (result)
			return result;
	}
#endif
#if defined WIN_NT
	size = FB_ALIGN(size, map_page_size);
	return VirtualAlloc(NULL, size, MEM_COMMIT, 
						PAGE_READWRITE);
#elif defined HAVE_MMAP
	size = FB_ALIGN(size, map_page_size);
# ifdef MAP_ANONYMOUS
	return mmap(NULL, size, PROT_READ | PROT_WRITE, 
		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
# else	
	// This code is needed for Solaris 2.6, AFAIK
	if (dev_zero_fd < 0)
		dev_zero_fd = open("/dev/zero", O_RDWR);
	return mmap(NULL, size, PROT_READ | PROT_WRITE, 
		MAP_PRIVATE, dev_zero_fd, 0);
# endif
#else
	return malloc(size);
#endif
}
	
void MemoryPool::external_free(void *blk, size_t &size) {
#if defined(WIN_NT) || defined(HAVE_MMAP)
	if (size == EXTENT_SIZE) {
		cache_mutex.enter();
		if (extents_cache.getCount() < extents_cache.getCapacity()) {
			extents_cache.add(blk);
			cache_mutex.leave();
			return;
		}
		cache_mutex.leave();
	}
#endif
#if defined WIN_NT
	size = FB_ALIGN(size, map_page_size);
	if (!VirtualFree(blk, 0, MEM_RELEASE))
		system_call_failed::raise("VirtualFree");
#elif defined HAVE_MMAP
	size = FB_ALIGN(size, map_page_size);
#if (defined SOLARIS) and (defined HAVE_CADDR_T)
	if (munmap((caddr_t) blk, size))
		system_call_failed::raise("munmap");

#else 
	if (munmap(blk, size))
		system_call_failed::raise("munmap");
#endif /*Solaris*/
#else
	::free(blk);
#endif
}

void* MemoryPool::tree_alloc(size_t size) {
	if (size == sizeof(FreeBlocksTree::ItemList))
		// This condition is to handle case when nodelist and itemlist have equal size
		if (sizeof(FreeBlocksTree::ItemList) != sizeof(FreeBlocksTree::NodeList) || 
			spareLeafs.getCount()) 
		{
			if (!spareLeafs.getCount())
				pool_out_of_memory();
			void *temp = spareLeafs[spareLeafs.getCount() - 1];
			spareLeafs.shrink(spareLeafs.getCount() - 1);
			needSpare = true;
			return temp;
		}
	if (size == sizeof(FreeBlocksTree::NodeList)) {
		if (!spareNodes.getCount())
			pool_out_of_memory();
		void *temp = spareNodes[spareNodes.getCount() - 1];
		spareNodes.shrink(spareNodes.getCount() - 1);
		needSpare = true;
		return temp;
	}
	fb_assert(false);
	return NULL;
}

void MemoryPool::tree_free(void* block) {
	// This method doesn't merge nearby pages
	((PendingFreeBlock*)block)->next = pendingFree;
	ptr_block(block)->mbk_flags &= ~MBK_USED;
	pendingFree = (PendingFreeBlock*)block;
	needSpare = true;
}

void* MemoryPool::allocate_nothrow(size_t size, SSHORT type
#ifdef DEBUG_GDS_ALLOC
	, const char* file, int line
#endif
) {
	size = MEM_ALIGN(size);
	if (parent_redirect) {
		// We do not synchronize redirect_amount here. In the worst case we redirect slightly 
		// more allocations to parent than we wanted. This shouldn't cause problems
		if (redirect_amount + size < REDIRECT_THRESHOLD) {
			parent->lock.enter();
			// Allocate block from parent
			void* result = parent->internal_alloc(size + MEM_ALIGN(sizeof(MemoryRedirectList)), type
#ifdef DEBUG_GDS_ALLOC
			  , file, line
#endif
			);
			if (!result) {
				parent->lock.leave();
				return NULL;
			}
			MemoryBlock* const blk = ptr_block(result);
			blk->mbk_pool = this;
			blk->mbk_flags |= MBK_PARENT;
			// Add block to the list of redirected blocks
			block_list_small(parent_redirected)->mrl_prev = blk;
			MemoryRedirectList *list = block_list_small(blk);
			list->mrl_prev = NULL;
			list->mrl_next = parent_redirected;
			parent_redirected = blk;

			// Update usage statistics
			const size_t blk_size = blk->small.mbk_length-MEM_ALIGN(sizeof(MemoryRedirectList));
			parent->increment_usage(blk_size);
			redirect_amount += blk_size;
			parent->lock.leave();
			return result;
		} else {
			lock.enter();
			if (parent_redirect) { // It may have changed while we were taking the lock
				parent_redirect = false;
				// Do some hard manual work to initialize first extent
				
				// This is the exact initial layout of memory pool in the first extent //
				// MemoryExtent
				// MemoryBlock
				// FreeBlocksTree::ItemList
				// MemoryBlock
				// free space
				//
				// ******************************************************************* //
				size_t ext_size = EXTENT_SIZE;
				MemoryExtent *extent = (MemoryExtent*)external_alloc(ext_size);
				fb_assert(ext_size == EXTENT_SIZE); // Make sure exent size is a multiply of page size
	
				if (!extent) {
					lock.leave();
					return NULL;
				}
				extent->mxt_next = NULL;
				extent->mxt_prev = NULL;
				extents = extent;
				increment_mapping(EXTENT_SIZE);
		
				MemoryBlock* hdr = (MemoryBlock*) ((char*)extent +
					MEM_ALIGN(sizeof(MemoryExtent)));
				hdr->mbk_pool = this;
				hdr->mbk_flags = MBK_USED;
				hdr->mbk_type = TYPE_LEAFPAGE;
				hdr->small.mbk_length = MEM_ALIGN(sizeof(FreeBlocksTree::ItemList));
				hdr->small.mbk_prev_length = 0;
				spareLeafs.add((char*)hdr + MEM_ALIGN(sizeof(MemoryBlock)));
				
				MemoryBlock* const blk = (MemoryBlock *)((char*)extent +
					MEM_ALIGN(sizeof(MemoryExtent)) +
					MEM_ALIGN(sizeof(MemoryBlock)) +
					MEM_ALIGN(sizeof(FreeBlocksTree::ItemList)));
				const int blockLength = EXTENT_SIZE -
					MEM_ALIGN(sizeof(MemoryExtent)) -
					MEM_ALIGN(sizeof(MemoryBlock)) -
					MEM_ALIGN(sizeof(FreeBlocksTree::ItemList)) -
					MEM_ALIGN(sizeof(MemoryBlock));
				blk->mbk_pool = this;
				blk->mbk_flags = MBK_LAST;
				blk->mbk_type = 0;
				blk->small.mbk_length = blockLength;
				blk->small.mbk_prev_length = hdr->small.mbk_length;
				BlockInfo temp = {blk, blockLength};
				freeBlocks.add(temp);
				updateSpare();
			}
			lock.leave();
		}
	}

	lock.enter();
	// If block cannot fit into extent then allocate it from OS directly
	if (size > EXTENT_SIZE - MEM_ALIGN(sizeof(MemoryBlock)) - MEM_ALIGN(sizeof(MemoryExtent))) {
		size_t ext_size = MEM_ALIGN(sizeof(MemoryBlock)) + size + 
			MEM_ALIGN(sizeof(MemoryRedirectList));
		MemoryBlock *blk = (MemoryBlock*) external_alloc(ext_size);
		if (!blk) {
			lock.leave();
			return NULL;
		}
		increment_mapping(ext_size);
		blk->mbk_pool = this;
		blk->mbk_flags = MBK_LARGE | MBK_USED;
		blk->mbk_type = type;
		blk->mbk_large_length = size + MEM_ALIGN(sizeof(MemoryRedirectList));
#ifdef DEBUG_GDS_ALLOC
		blk->mbk_file = file;
		blk->mbk_line = line;
#endif
		// Add block to the list of redirected blocks
		if (os_redirected)
			block_list_large(os_redirected)->mrl_prev = blk;
		MemoryRedirectList *list = block_list_large(blk);
		list->mrl_prev = NULL;
		list->mrl_next = os_redirected;
		os_redirected = blk;
		
		// Update usage statistics
		increment_usage(size);
		lock.leave();
		return (char*)blk + MEM_ALIGN(sizeof(MemoryBlock));
	}
	// Otherwise use conventional allocator
	void* result = internal_alloc(size, type
#ifdef DEBUG_GDS_ALLOC
		, file, line
#endif
	);
	if (needSpare)
		updateSpare();
	// Update usage statistics
	if (result)
		increment_usage(ptr_block(result)->small.mbk_length);
	lock.leave();
	return result;
}

void* MemoryPool::allocate(size_t size, SSHORT type
#ifdef DEBUG_GDS_ALLOC
	, const char* file, int line
#endif
) {
	void* result = allocate_nothrow(size, type
#ifdef DEBUG_GDS_ALLOC
		, file, line
#endif
	);
	if (!result)
		pool_out_of_memory();
	return result;
}

bool MemoryPool::verify_pool() {
	lock.enter();
	mem_assert(!pendingFree || needSpare); // needSpare flag should be set if we are in 
										// a critically low memory condition
	size_t blk_used_memory = 0;
	size_t blk_mapped_memory = 0;
	// check each block in each segment for consistency with free blocks structure
	for (const MemoryExtent *extent = extents; extent; extent = extent->mxt_next) {
		// Verify doubly linked list
		if (extent == extents) {
			mem_assert(extent->mxt_prev == NULL);
		} else {
			mem_assert(extent->mxt_prev);
			mem_assert(extent->mxt_prev->mxt_next == extent);
		}
		blk_mapped_memory += EXTENT_SIZE;
		USHORT prev_length = 0;
		for (const MemoryBlock *blk = (const MemoryBlock *)((const char*)extent + MEM_ALIGN(sizeof(MemoryExtent)));
			; 
			blk = next_block(blk))
		{
			mem_assert(!(blk->mbk_flags & ~(MBK_USED | MBK_LAST | MBK_PARENT))); // No large blocks allowed here
			// Check pool pointer
			if (blk->mbk_flags & MBK_PARENT)
				mem_assert(blk->mbk_pool->parent == this);
			else
				mem_assert(blk->mbk_pool == this);

			// Calculate memory usage
			if ((blk->mbk_flags & MBK_USED) && (blk->mbk_type >= 0)) {
				if (blk->mbk_flags & MBK_PARENT) {
					blk_used_memory += blk->small.mbk_length - MEM_ALIGN(sizeof(MemoryRedirectList));
				} else {
					blk_used_memory += blk->small.mbk_length;
				}
			}
			
			mem_assert(blk->small.mbk_prev_length == prev_length); // Prev is correct ?
			BlockInfo temp = {const_cast<MemoryBlock*>(blk), blk->small.mbk_length};
			bool foundTree = freeBlocks.locate(temp), foundPending = false;
			for (const PendingFreeBlock *tmp = pendingFree; tmp; tmp = tmp->next)
				if (tmp == (const PendingFreeBlock *)((const char*)blk + MEM_ALIGN(sizeof(MemoryBlock)))) {
					mem_assert(!foundPending); // Block may be in pending list only one time
					foundPending = true;
				}
			mem_assert(!(foundTree && foundPending)); // Block shouldn't be present both in
												   // pending list and in tree list
			
			if (!(blk->mbk_flags & MBK_USED)) {
				mem_assert(foundTree || foundPending); // Block is free. Should be somewhere
			}
			else
				mem_assert(!foundTree && !foundPending); // Block is not free. Should not be in free lists
			prev_length = blk->small.mbk_length;
			if (blk->mbk_flags & MBK_LAST)
				break;
		}
	}
	
	// Verify large blocks
	for (const MemoryBlock *large = os_redirected; large; large = block_list_large(large)->mrl_next)
	{
		const MemoryRedirectList* list = block_list_large(large);
		// Verify doubly linked list
		if (large == os_redirected) {
			mem_assert(list->mrl_prev == NULL);
		} else {
			mem_assert(list->mrl_prev);
			mem_assert(block_list_large(list->mrl_prev)->mrl_next == large);
		}
		mem_assert(large->mbk_flags & MBK_LARGE);
		mem_assert(large->mbk_flags & MBK_USED);
		mem_assert(!(large->mbk_flags & MBK_PARENT));
		blk_used_memory += large->mbk_large_length - MEM_ALIGN(sizeof(MemoryRedirectList));
#if defined(WIN_NT) || defined(HAVE_MMAP)
		blk_mapped_memory += FB_ALIGN(large->mbk_large_length, map_page_size);
#else
		blk_mapped_memory += large->mbk_large_length;
#endif
	}
	
	// Verify memory usage accounting
	mem_assert(blk_mapped_memory == mapped_memory);
	mem_assert(blk_used_memory == used_memory);
	lock.leave();
	
	if (parent) {
		parent->lock.enter();
		// Verify redirected blocks
		size_t blk_redirected = 0;
		for (const MemoryBlock *redirected = parent_redirected; redirected; redirected = block_list_small(redirected)->mrl_next)
		{
			const MemoryRedirectList* list = block_list_small(redirected);
			// Verify doubly linked list
			if (redirected == parent_redirected) {
				mem_assert(list->mrl_prev == NULL);
			} else {
				mem_assert(list->mrl_prev);
				mem_assert(block_list_small(list->mrl_prev)->mrl_next == redirected);
			}
			// Verify flags
			mem_assert(redirected->mbk_flags & MBK_PARENT);
			mem_assert(redirected->mbk_flags & MBK_USED);
			mem_assert(!(redirected->mbk_flags & MBK_LARGE));
			if (redirected->mbk_type >= 0)
				blk_redirected += redirected->small.mbk_length - sizeof(MemoryRedirectList);
		}
		// Check accounting
		mem_assert(blk_redirected == redirect_amount);
		parent->lock.leave();
	}
	
	return true;
}

static void print_block(FILE *file, const MemoryBlock *blk, bool used_only)
{
	const void *mem = (const char*)blk + MEM_ALIGN(sizeof(MemoryBlock));
	if (((blk->mbk_flags & MBK_USED) && blk->mbk_type >= 0) || !used_only) {
		char flags[100];
		flags[0] = 0;
		if (blk->mbk_flags & MBK_USED)
			strcat(flags, " USED");
		if (blk->mbk_flags & MBK_LAST)
			strcat(flags, " LAST");
		if (blk->mbk_flags & MBK_LARGE)
			strcat(flags, " LARGE");
		if (blk->mbk_flags & MBK_PARENT)
			strcat(flags, " PARENT");
		
		const int size =
			blk->mbk_flags & MBK_LARGE ? blk->mbk_large_length : blk->small.mbk_length;
#ifdef DEBUG_GDS_ALLOC
		if (blk->mbk_flags & MBK_USED) {
			if (blk->mbk_type > 0)
				fprintf(file, "%p%s: size=%d type=%d allocated at %s:%d\n", 
					mem, flags, size, blk->mbk_type, blk->mbk_file, blk->mbk_line);
			else if (blk->mbk_type == 0)
				fprintf(file, "%p%s: size=%d allocated at %s:%d\n", 
					mem, flags, size, blk->mbk_file, blk->mbk_line);
			else
				fprintf(file, "%p%s: size=%d type=%d\n", 
					mem, flags, size, blk->mbk_type);
		}
#else
		if (blk->mbk_type && (blk->mbk_flags & MBK_USED))
			fprintf(file, "%p(%s): size=%d type=%d\n", 
				mem, flags, size, blk->mbk_type);
#endif
		else
			fprintf(file, "%p(%s): size=%d\n", 
				mem, flags, size);
	}
}

// This member function can't be const because there are calls to the mutex.
void MemoryPool::print_contents(FILE *file, bool used_only)
{
	lock.enter();
	fprintf(file, "********* Printing contents of pool %p used=%ld mapped=%ld:\n", 
		this, (long)used_memory, (long)mapped_memory);
	// Print extents
	for (const MemoryExtent *extent = extents; extent; extent = extent->mxt_next) {
		if (!used_only)
			fprintf(file, "EXTENT %p:\n", extent);
		for (const MemoryBlock *blk = (MemoryBlock *)((char*)extent + MEM_ALIGN(sizeof(MemoryExtent)));
			; 
			blk = next_block(blk))
		{
			print_block(file, blk, used_only);			
			if (blk->mbk_flags & MBK_LAST)
				break;
		}
	}
	// Print large blocks
	if (os_redirected) {
		fprintf(file, "LARGE BLOCKS:\n");
		for (const MemoryBlock *blk = os_redirected; blk; blk = block_list_large(blk)->mrl_next)
			print_block(file, blk, used_only);
	}
	lock.leave();
	// Print redirected blocks
	if (parent_redirected) {
		fprintf(file, "REDIRECTED TO PARENT %p:\n", parent);
		parent->lock.enter();
		for (const MemoryBlock *blk = parent_redirected; blk; blk = block_list_small(blk)->mrl_next)
			print_block(file, blk, used_only);
		parent->lock.leave();
	}
	fprintf(file, "********* End of output for pool %p:\n", this);
}

MemoryPool* MemoryPool::internal_create(size_t instance_size, MemoryPool* parent, MemoryStats &stats) 
{
	// If pool has a parent things are simplified
	if (parent) {
		parent->lock.enter();
		void* mem = parent->internal_alloc(instance_size + sizeof(MemoryRedirectList), TYPE_POOL);
		if (!mem) {
			parent->lock.leave();
			pool_out_of_memory();
		}
		MemoryPool* pool = new(mem) MemoryPool(parent, stats, NULL, NULL);
		pool->parent_redirect = true;
		
		MemoryBlock* const blk = ptr_block(mem);
		blk->mbk_pool = pool;
		blk->mbk_flags |= MBK_PARENT;
		// Add block to the list of redirected blocks
		MemoryRedirectList *list = block_list_small(blk);
		list->mrl_prev = NULL;
		list->mrl_next = NULL;
		pool->parent_redirected = blk;

		parent->lock.leave();
		return pool;
	}

	// This is the exact initial layout of memory pool in the first extent //
	// MemoryExtent
	// MemoryBlock
	// MemoryPool (instance_size)
	// MemoryBlock
	// FreeBlocksTree::ItemList
	// MemoryBlock
	// free space
	//
	// ******************************************************************* //

	size_t ext_size = EXTENT_SIZE;
	char* mem = (char *)external_alloc(ext_size);
	fb_assert(ext_size == EXTENT_SIZE); // Make sure exent size is a multiply of page size
	
	if (!mem)
		pool_out_of_memory();
	((MemoryExtent *)mem)->mxt_next = NULL;
	((MemoryExtent *)mem)->mxt_prev = NULL;
	MemoryPool* pool = new(mem +
		MEM_ALIGN(sizeof(MemoryExtent)) +
		MEM_ALIGN(sizeof(MemoryBlock))) 
	MemoryPool(parent, stats, mem, mem + 
		MEM_ALIGN(sizeof(MemoryExtent)) + 
		MEM_ALIGN(sizeof(MemoryBlock)) + 
		MEM_ALIGN(instance_size) + 
		MEM_ALIGN(sizeof(MemoryBlock)));
		
	pool->increment_mapping(EXTENT_SIZE);
		
	MemoryBlock *poolBlk = (MemoryBlock*) (mem+MEM_ALIGN(sizeof(MemoryExtent)));
	poolBlk->mbk_pool = pool;
	poolBlk->mbk_flags = MBK_USED;
	poolBlk->mbk_type = TYPE_POOL;
	poolBlk->small.mbk_length = MEM_ALIGN(instance_size);
	poolBlk->small.mbk_prev_length = 0;
	
	MemoryBlock* hdr = (MemoryBlock*) (mem +
		MEM_ALIGN(sizeof(MemoryExtent)) +
		MEM_ALIGN(sizeof(MemoryBlock)) +
		MEM_ALIGN(instance_size));
	hdr->mbk_pool = pool;
	hdr->mbk_flags = MBK_USED;
	hdr->mbk_type = TYPE_LEAFPAGE;
	hdr->small.mbk_length = MEM_ALIGN(sizeof(FreeBlocksTree::ItemList));
	hdr->small.mbk_prev_length = poolBlk->small.mbk_length;
	MemoryBlock* const blk = (MemoryBlock *)(mem +
		MEM_ALIGN(sizeof(MemoryExtent)) +
		MEM_ALIGN(sizeof(MemoryBlock)) +
		MEM_ALIGN(instance_size) +
		MEM_ALIGN(sizeof(MemoryBlock)) +
		MEM_ALIGN(sizeof(FreeBlocksTree::ItemList)));
	const int blockLength = EXTENT_SIZE -
		MEM_ALIGN(sizeof(MemoryExtent)) -
		MEM_ALIGN(sizeof(MemoryBlock)) -
		MEM_ALIGN(instance_size) -
		MEM_ALIGN(sizeof(MemoryBlock)) -
		MEM_ALIGN(sizeof(FreeBlocksTree::ItemList)) -
		MEM_ALIGN(sizeof(MemoryBlock));
	blk->mbk_pool = pool;
	blk->mbk_flags = MBK_LAST;
	blk->mbk_type = 0;
	blk->small.mbk_length = blockLength;
	blk->small.mbk_prev_length = hdr->small.mbk_length;
	BlockInfo temp = {blk, blockLength};
	pool->freeBlocks.add(temp);
	pool->updateSpare();
	return pool;
}

void MemoryPool::deletePool(MemoryPool* pool)
{
	// Adjust usage
	pool->decrement_usage(pool->used_memory);
	pool->decrement_mapping(pool->mapped_memory);
	
	// Free mutex
	pool->lock.~Mutex();
	
	// Order of deallocation is of significance because
	// we delete our pool in process
	
	// Deallocate all large blocks redirected to OS
	MemoryBlock *large = pool->os_redirected;
	while (large) {
		MemoryBlock *next = block_list_large(large)->mrl_next;
		size_t ext_size = large->mbk_large_length;
		external_free(large, ext_size);
		large = next;
	}
	
	MemoryPool *parent = pool->parent;
	
	// Delete all extents now
	MemoryExtent *extent = pool->extents;
	while (extent) {
		MemoryExtent *next = extent->mxt_next;
		size_t ext_size = EXTENT_SIZE;
		external_free(extent, ext_size);
		fb_assert(ext_size == EXTENT_SIZE); // Make sure exent size is a multiply of page size	
		extent = next;
	}
	
	// Deallocate blocks redirected to parent
	// IF parent is set then pool was allocated from it and is not deleted at this point yet
	if (parent) {		
		parent->lock.enter();
		parent->decrement_usage(pool->redirect_amount);
		MemoryBlock *redirected = pool->parent_redirected;
		while (redirected) {
			MemoryBlock *next = block_list_small(redirected)->mrl_next;
			redirected->mbk_pool = parent;
			redirected->mbk_flags &= ~MBK_PARENT;			
			parent->internal_deallocate((char*)redirected + MEM_ALIGN(sizeof(MemoryBlock)));
			redirected = next;
		}
		// Our pool does not exist at this point
		if (parent->needSpare)
			parent->updateSpare();
		parent->lock.leave();
	}
}

void* MemoryPool::internal_alloc(size_t size, SSHORT type
#ifdef DEBUG_GDS_ALLOC
	, const char* file, int line
#endif
) {
	// This method assumes already aligned block sizes
	fb_assert(size % ALLOC_ALIGNMENT == 0);
	// Make sure block can fit into extent
	fb_assert(size < EXTENT_SIZE - MEM_ALIGN(sizeof(MemoryBlock)) - MEM_ALIGN(sizeof(MemoryExtent)));
	
	// Lookup a block greater or equal than size in freeBlocks tree
	BlockInfo blTemp = {NULL, size};
	MemoryBlock* blk;
	if (freeBlocks.locate(locGreatEqual,blTemp)) {
		// Found large enough block
		BlockInfo* current = &freeBlocks.current();
		if (current->length - size < MEM_ALIGN(sizeof(MemoryBlock)) + ALLOC_ALIGNMENT)
		{
			blk = current->block;
			// Block is small enough to be returned AS IS
			blk->mbk_flags |= MBK_USED;
			blk->mbk_type = type;
#ifdef DEBUG_GDS_ALLOC
			blk->mbk_file = file;
			blk->mbk_line = line;
#endif
			freeBlocks.fastRemove();
		}
		else {
			// Cut a piece at the end of block in hope to avoid structural
			// modification of free blocks tree
			current->block->small.mbk_length -= MEM_ALIGN(sizeof(MemoryBlock))+size;
			blk = next_block(current->block);
			blk->mbk_pool = this;
			blk->mbk_flags = MBK_USED | (current->block->mbk_flags & MBK_LAST);
#ifdef DEBUG_GDS_ALLOC
			blk->mbk_file = file;
			blk->mbk_line = line;
#endif
			current->block->mbk_flags &= ~MBK_LAST;
			blk->mbk_type = type;
			blk->small.mbk_length = size;
			blk->small.mbk_prev_length = current->block->small.mbk_length;
			if (!(blk->mbk_flags & MBK_LAST))
				next_block(blk)->small.mbk_prev_length = blk->small.mbk_length;
			// Update tree of free blocks
			if (!freeBlocks.getPrev() || freeBlocks.current().length < current->block->small.mbk_length)
				current->length = current->block->small.mbk_length;
			else {
				// Tree needs to be modified structurally
#ifndef DEV_BUILD
				freeBlocks.getNext();
#else
				const bool res = freeBlocks.getNext();
				fb_assert(res);
				fb_assert(&freeBlocks.current() == current);
#endif
				MemoryBlock *block = current->block;
				freeBlocks.fastRemove();
				addFreeBlock(block);
			}
		}
	}
	else {
		// If we are in a critically low memory condition look up for a block in a list
		// of pending free blocks. We do not do "best fit" in this case
		PendingFreeBlock *itr = pendingFree, *prev = NULL;
		while (itr) {
			MemoryBlock *temp = ptr_block(itr);
			if (temp->small.mbk_length >= size) {
				if (temp->small.mbk_length - size < MEM_ALIGN(sizeof(MemoryBlock)) + ALLOC_ALIGNMENT)
				{
					// Block is small enough to be returned AS IS
					temp->mbk_flags |= MBK_USED;
					temp->mbk_type = type;
#ifdef DEBUG_GDS_ALLOC
					temp->mbk_file = file;
					temp->mbk_line = line;
#endif
					// Remove block from linked list
					if (prev)
						prev->next = itr->next;
					else
						pendingFree = itr->next;						
					PATTERN_FILL(itr, size, ALLOC_PATTERN);
					return itr;
				}
				else {
					// Cut a piece at the end of block
					// We don't need to modify tree of free blocks or a list of
					// pending free blocks in this case
					temp->small.mbk_length -= MEM_ALIGN(sizeof(MemoryBlock)) + size;
					blk = next_block(temp);
					blk->mbk_pool = this;
					blk->mbk_flags = MBK_USED | (temp->mbk_flags & MBK_LAST);
#ifdef DEBUG_GDS_ALLOC
					blk->mbk_file = file;
					blk->mbk_line = line;
#endif
					temp->mbk_flags &= ~MBK_LAST;
					blk->mbk_type = type;
					blk->small.mbk_length = size;
					blk->small.mbk_prev_length = temp->small.mbk_length;
					if (!(blk->mbk_flags & MBK_LAST))
						next_block(blk)->small.mbk_prev_length = blk->small.mbk_length;
					void *result = (char *)blk + MEM_ALIGN(sizeof(MemoryBlock));
					PATTERN_FILL(result, size, ALLOC_PATTERN);
					return result;
				}
			}
			prev = itr;
			itr = itr->next;
		}
		// No large enough block found. We need to extend the pool
		size_t ext_size = EXTENT_SIZE;
		MemoryExtent* extent = (MemoryExtent *)external_alloc(ext_size);
		fb_assert(ext_size == EXTENT_SIZE); // Make sure exent size is a multiply of page size
	
		if (!extent) {
			return NULL;
		}
		increment_mapping(EXTENT_SIZE);
		// Add extent to a doubly linked list
		extents->mxt_prev = extent;
		extent->mxt_next = extents;
		extent->mxt_prev = NULL;
		extents = extent;
			
		blk = (MemoryBlock *)((char*)extent + MEM_ALIGN(sizeof(MemoryExtent)));
		blk->mbk_pool = this;
		blk->mbk_flags = MBK_USED;
		blk->mbk_type = type;
#ifdef DEBUG_GDS_ALLOC
		blk->mbk_file = file;
		blk->mbk_line = line;
#endif
		blk->small.mbk_prev_length = 0;
		if (EXTENT_SIZE - size - MEM_ALIGN(sizeof(MemoryExtent)) - MEM_ALIGN(sizeof(MemoryBlock)) 
					< MEM_ALIGN(sizeof(MemoryBlock)) + ALLOC_ALIGNMENT) 
		{
			// Block is small enough to be returned AS IS
			blk->mbk_flags |= MBK_LAST;
			blk->small.mbk_length = EXTENT_SIZE - MEM_ALIGN(sizeof(MemoryExtent)) - MEM_ALIGN(sizeof(MemoryBlock));
		}
		else {
			// Cut a piece at the beginning of the block
			blk->small.mbk_length = size;
			// Put the rest to the tree of free blocks
			MemoryBlock *rest = next_block(blk);
			rest->mbk_pool = this;
			rest->mbk_flags = MBK_LAST;
			rest->small.mbk_length = EXTENT_SIZE - MEM_ALIGN(sizeof(MemoryExtent)) - 
				MEM_ALIGN(sizeof(MemoryBlock)) - size - MEM_ALIGN(sizeof(MemoryBlock));
			rest->small.mbk_prev_length = blk->small.mbk_length;
			addFreeBlock(rest);
		}
	}
	void *result = (char*)blk + MEM_ALIGN(sizeof(MemoryBlock));
	PATTERN_FILL(result, size, ALLOC_PATTERN);
	return result;
}

inline void MemoryPool::addFreeBlock(MemoryBlock *blk)
{
	BlockInfo info = {blk, blk->small.mbk_length};
	try {
		freeBlocks.add(info);
	} catch(const std::exception&) {
		// Add item to the list of pending free blocks in case of critically-low memory condition
		PendingFreeBlock* temp = (PendingFreeBlock *)((char *)blk + MEM_ALIGN(sizeof(MemoryBlock)));
		temp->next = pendingFree;
		pendingFree = temp;
	}
}
		
void MemoryPool::removeFreeBlock(MemoryBlock *blk)
{
	BlockInfo info = {blk, blk->small.mbk_length};
	if (freeBlocks.locate(info)) {
		freeBlocks.fastRemove();
	}
	else {
		// If we are in a critically-low memory condition our block could be in the
		// pending free blocks list. Remove it from there
		PendingFreeBlock *itr = pendingFree, 
			*temp = (PendingFreeBlock *)((char *)blk + MEM_ALIGN(sizeof(MemoryBlock)));
		if (itr == temp)
			pendingFree = itr->next;
		else
		{
			while ( itr ) {
				PendingFreeBlock *next = itr->next;
				if (next==temp) {
					itr->next = temp->next;
					break;
				}
				itr = next;
			}
			fb_assert(itr); // We had to find it somewhere
		}
	}
}

void MemoryPool::free_blk_extent(MemoryBlock *blk)
{
	MemoryExtent *extent = (MemoryExtent *)((char *)blk - MEM_ALIGN(sizeof(MemoryExtent)));
	
	// Delete extent from the doubly linked list
	if (extent->mxt_prev)
		extent->mxt_prev->mxt_next = extent->mxt_next;
	else
		extents = extent->mxt_next;
	if (extent->mxt_next)
		extent->mxt_next->mxt_prev = extent->mxt_prev;

	fb_assert(blk->small.mbk_length + MEM_ALIGN(sizeof(MemoryBlock)) + 
		MEM_ALIGN(sizeof(MemoryExtent)) == EXTENT_SIZE);

	size_t ext_size = EXTENT_SIZE;
	external_free(extent, ext_size);
	fb_assert(ext_size == EXTENT_SIZE); // Make sure exent size is a multiply of page size	
	decrement_mapping(EXTENT_SIZE);
}

void MemoryPool::internal_deallocate(void *block)
{
	// Note that this method is also used to add blocks from pending free list
	// These blocks are marked as unused already
	MemoryBlock *blk = ptr_block(block);
	
	//fb_assert(blk->mbk_flags & MBK_USED);
	fb_assert(blk->mbk_pool==this);

	MemoryBlock *prev;
	// Try to merge block with preceding free block
	if (blk->small.mbk_prev_length && !((prev = prev_block(blk))->mbk_flags & MBK_USED))
	{
	   	removeFreeBlock(prev);
		prev->small.mbk_length += blk->small.mbk_length + MEM_ALIGN(sizeof(MemoryBlock));

		MemoryBlock *next = NULL;
		if (blk->mbk_flags & MBK_LAST) {
			prev->mbk_flags |= MBK_LAST;
		}
		else {
			next = next_block(blk);
			if (next->mbk_flags & MBK_USED) {
				next->small.mbk_prev_length = prev->small.mbk_length;
				prev->mbk_flags &= ~MBK_LAST;
			}
			else {
				// Merge next block too
				removeFreeBlock(next);
				prev->small.mbk_length += next->small.mbk_length + MEM_ALIGN(sizeof(MemoryBlock));
				prev->mbk_flags |= next->mbk_flags & MBK_LAST;
				if (!(next->mbk_flags & MBK_LAST))
					next_block(next)->small.mbk_prev_length = prev->small.mbk_length;
			}
		}
		PATTERN_FILL((char*)prev + MEM_ALIGN(sizeof(MemoryBlock)), prev->small.mbk_length, FREE_PATTERN);
		if (!prev->small.mbk_prev_length && (prev->mbk_flags & MBK_LAST))
			free_blk_extent(prev);
		else
			addFreeBlock(prev);
	}
	else {
		MemoryBlock *next;
		// Mark block as free
		blk->mbk_flags &= ~MBK_USED;
		// Try to merge block with next free block
		if (!(blk->mbk_flags & MBK_LAST) && 
			!((next = next_block(blk))->mbk_flags & MBK_USED)) 
		{
			removeFreeBlock(next);
			blk->small.mbk_length += next->small.mbk_length + MEM_ALIGN(sizeof(MemoryBlock));
			blk->mbk_flags |= next->mbk_flags & MBK_LAST;
			if (!(next->mbk_flags & MBK_LAST))
				next_block(next)->small.mbk_prev_length = blk->small.mbk_length;
		}
		PATTERN_FILL(block, blk->small.mbk_length, FREE_PATTERN);
		if (!blk->small.mbk_prev_length && (blk->mbk_flags & MBK_LAST))
			free_blk_extent(blk);
		else
			addFreeBlock(blk);
	}
}


void MemoryPool::deallocate(void *block)
{
	if (!block)
		return;

	MemoryBlock* const blk = ptr_block(block);
	
	fb_assert(blk->mbk_flags & MBK_USED);
	fb_assert(blk->mbk_pool==this);

	if (blk->mbk_flags & MBK_PARENT) {
		parent->lock.enter();
		blk->mbk_pool = parent;
		blk->mbk_flags &= ~MBK_PARENT;
		// Delete block from list of redirected blocks
		MemoryRedirectList* const list = block_list_small(blk);
		if (list->mrl_prev)
			block_list_small(list->mrl_prev)->mrl_next = list->mrl_next;
		else
			parent_redirected = list->mrl_next;
		if (list->mrl_next)
			block_list_small(list->mrl_next)->mrl_prev = list->mrl_prev;
		// Update usage statistics
		const size_t size = blk->small.mbk_length - MEM_ALIGN(sizeof(MemoryRedirectList));
		parent->decrement_usage(size);
		redirect_amount -= size;
		// Free block from parent
		parent->internal_deallocate(block);
		if (parent->needSpare)
			parent->updateSpare();
		parent->lock.leave();
		return;
	}

	lock.enter();
	
	if (blk->mbk_flags & MBK_LARGE) {
		// Delete block from list of redirected blocks
		MemoryRedirectList *list = block_list_large(blk);
		if (list->mrl_prev)
			block_list_large(list->mrl_prev)->mrl_next = list->mrl_next;
		else
			os_redirected = list->mrl_next;
		if (list->mrl_next)
			block_list_large(list->mrl_next)->mrl_prev = list->mrl_prev;
		// Update usage statistics
		const size_t size = blk->mbk_large_length - MEM_ALIGN(sizeof(MemoryRedirectList));
		decrement_usage(size);
		// Free the block
		size_t ext_size = MEM_ALIGN(sizeof(MemoryBlock)) + size +
			MEM_ALIGN(sizeof(MemoryRedirectList));
		external_free(blk, ext_size);
		decrement_mapping(ext_size);
		lock.leave();
		return;
	}
	
	// Deallocate small block from this pool
	decrement_usage(blk->small.mbk_length);
	internal_deallocate(block);
	if (needSpare)
		updateSpare();
	
	lock.leave();
}

} // namespace Firebird

/******************************** Global functions *****************************/

void* operator new(size_t s) THROW_BAD_ALLOC
{
#if defined(DEV_BUILD)
// Do not complain here. It causes client tools to crash on Red Hat 8.0
//	fprintf(stderr, "You MUST allocate all memory from a pool.  Don't use the default global new().\n");
#endif	// DEV_BUILD
//	return getDefaultMemoryPool()->calloc(s, 0
	return getDefaultMemoryPool()->allocate(s, 0
#ifdef DEBUG_GDS_ALLOC
	  ,__FILE__,__LINE__
#endif
	);
}

void* operator new[](size_t s) THROW_BAD_ALLOC
{
#if defined(DEV_BUILD)
// Do not complain here. It causes client tools to crash on Red Hat 8.0
//	fprintf(stderr, "You MUST allocate all memory from a pool.  Don't use the default global new[]().\n");
#endif	// DEV_BUILD
//	return getDefaultMemoryPool()->->calloc(s, 0
	return getDefaultMemoryPool()->allocate(s, 0
#ifdef DEBUG_GDS_ALLOC
	  ,__FILE__,__LINE__
#endif
	);
}

void operator delete(void* mem) throw()
{
	Firebird::MemoryPool::globalFree(mem);
}

void operator delete[](void* mem) throw()
{
	Firebird::MemoryPool::globalFree(mem);
}

#if defined(DEV_BUILD)
void Firebird::AutoStorage::ProbeStack() const {
	//
	// AutoStorage() default constructor can be used only 
	// for objects on the stack. ProbeStack() uses the 
	// following assumptions to check it:
	//	1. One and only one stack is used for all kind of variables.
	//	2. Objects don't grow > 64K.
	//
	char ProbeVar = '\0';
	const char *MyStack = &ProbeVar;
	const char *ThisLocation = (const char *)this;
	int distance = ThisLocation - MyStack;
	if (distance < 0) {
		distance = -distance;
	}
	fb_assert(distance < 64 * 1024);
}
#endif

