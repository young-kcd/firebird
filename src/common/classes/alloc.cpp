/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		alloc.cpp
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

#include "../../include/firebird.h"
#include "alloc.h"
#include <new>

// Size in bytes, must be aligned according to ALLOC_ALIGNMENT
#define MIN_EXTENT_SIZE  16384

#define FB_MAX(M,N) ((M)>(N)?(M):(N))

#define FREE_PATTERN 0xDEADBEEF
#define ALLOC_PATTERN 0xFEEDABED
//#define ALLOC_PATTERN 0x0
#ifdef DEBUG_GDS_ALLOC
#define PATTERN_FILL(ptr,size,pattern) for (size_t _i=0;_i< size>>2;_i++) ((unsigned int*)(ptr))[_i]=(pattern)
#else
#define PATTERN_FILL(ptr,size,pattern) ((void)0)
#endif

// TODO (in order of importance):
// 1. local pool locking +
// 2. line number debug info +
// 3. debug alloc/free pattern +
// 4. print pool contents function +
//---- Not needed for current codebase
// 5. Pool size limit
// 6. allocation source
// 7. shared pool locking
// 8. red zones checking (not really needed because verify_pool is able to detect most corruption cases)

namespace Firebird {

int MemoryPool::process_max_memory = 0;
int MemoryPool::process_current_memory = 0;

// Helper function to reduce code size, since many compilers
// generate quite a bit of code at the point of the throw.
static void pool_out_of_memory()
{
	throw std::bad_alloc();
}

static MemoryPool* processMemoryPool;

MemoryPool::MemoryPool(void *first_extent, void *root_page, int* cur_mem, int* max_mem) : 
	freeBlocks((InternalAllocator*)this, root_page),
	extents((MemoryExtent *)first_extent), 
	needSpare(false),
	pendingFree(NULL),
	/*extents_memory(0), - Initialized in internal_create() */
	used_memory(0),
	cur_memory(cur_mem),
	max_memory(max_mem)
{
}

void MemoryPool::updateSpare() {
	do {
		do {
			needSpare = false;
			while (spareLeafs.getCount() < spareLeafs.getCapacity()) {
				void* temp = internal_alloc(sizeof(FreeBlocksTree::ItemList), TYPE_LEAFPAGE);
				if (!temp) return;
				spareLeafs.add(temp);
			}
			while (spareNodes.getCount() <= freeBlocks.level) {
				void* temp = internal_alloc(sizeof(FreeBlocksTree::NodeList), TYPE_TREEPAGE);
				if (!temp) return;
				spareNodes.add(temp);
			}
			break;
		} while (needSpare);
		// Great, if we were able to restore free blocks tree operations after critically low
		// memory condition then try to add pending free blocks to our tree
		while (pendingFree) {
			PendingFreeBlock *temp = pendingFree;
			pendingFree = temp->next;
			MemoryBlock *blk = (MemoryBlock*)((char*)temp-MEM_ALIGN(sizeof(MemoryBlock)));
			BlockInfo info = {
				blk,
				blk->length
			};
			freeBlocks.add(info); // We should be able to do this because we had all needed spare blocks
		}
	} while (needSpare);
}

void* MemoryPool::external_alloc(size_t size) {
	// This method is assumed to return NULL in case it cannot alloc
	return malloc(size);
}
	
void MemoryPool::external_free(void *blk) {
	::free(blk);
}

void* MemoryPool::tree_alloc(size_t size) {
	if (size == sizeof(FreeBlocksTree::ItemList))
		// This condition is to handle case when nodelist and itemlist have equal size
		if (sizeof(FreeBlocksTree::ItemList)!=sizeof(FreeBlocksTree::NodeList) || 
			spareLeafs.getCount()) 
		{
			if (!spareLeafs.getCount()) pool_out_of_memory();
			void *temp = spareLeafs[spareLeafs.getCount()-1];
			spareLeafs.shrink(spareLeafs.getCount()-1);
			needSpare = true;
			return temp;
		}
	if (size == sizeof(FreeBlocksTree::NodeList)) {
		if (!spareNodes.getCount()) pool_out_of_memory();
		void *temp = spareNodes[spareNodes.getCount()-1];
		spareNodes.shrink(spareNodes.getCount()-1);
		needSpare = true;
		return temp;
	}
	assert(false);
	return 0;
}

void MemoryPool::tree_free(void* block) {
	((PendingFreeBlock*)block)->next = pendingFree;
	((MemoryBlock*)((char*)block-MEM_ALIGN(sizeof(MemoryBlock))))->used = false;
	pendingFree = (PendingFreeBlock*)block;
	needSpare = true;
	return;
}

void* MemoryPool::allocate(size_t size, SSHORT type
#ifdef DEBUG_GDS_ALLOC
	, char* file, int line
#endif
) {
	lock.enter();
	void* result = internal_alloc(size, type
#ifdef DEBUG_GDS_ALLOC
		, file, line
#endif
	);
	if (needSpare) updateSpare();
	lock.leave();
	if (!result) pool_out_of_memory();
	// test with older behavior
	// memset(result,0,size);
	return result;
}

bool MemoryPool::verify_pool() {
#ifdef TESTING_ONLY
	lock.enter();
	assert (!pendingFree || needSpare); // needSpare flag should be set if we are in 
										// a critically low memory condition
	// check each block in each segment for consistency with free blocks structure
	for (MemoryExtent *extent = extents; extent; extent=extent->next) {
		MemoryBlock *prev = NULL;
		for (MemoryBlock *blk = (MemoryBlock *)((char*)extent+MEM_ALIGN(sizeof(MemoryExtent))); 
			; 
			blk = (MemoryBlock *)((char*)blk+MEM_ALIGN(sizeof(MemoryBlock))+blk->length))
		{
#ifndef NDEBUG
			assert(blk->pool == this); // Pool is correct ?
			assert(blk->prev == prev); // Prev is correct ?
			BlockInfo temp = {blk, blk->length};
			bool foundTree = freeBlocks.locate(temp), foundPending = false;
			for (PendingFreeBlock *tmp = pendingFree; tmp; tmp = tmp->next)
				if (tmp == (PendingFreeBlock *)((char*)blk+MEM_ALIGN(sizeof(MemoryBlock)))) {
					assert(!foundPending); // Block may be in pending list only one time
					foundPending = true;
				}
			assert(! (foundTree && foundPending)); // Block shouldn't be present both in
												   // pending list and in tree list
			
			if (!blk->used) {
				assert(foundTree || foundPending); // Block is free. Should be somewhere
			} else
				assert(!foundTree && !foundPending); // Block is not free. Should not be in free lists
#endif
			prev = blk;
			if (blk->last) break;
		}
	}
	lock.leave();
#endif
	return true;
}

void MemoryPool::print_contents(IB_FILE *file, bool used_only) {
	lock.enter();
	for (MemoryExtent *extent = extents; extent; extent=extent->next) {
		if (!used_only)
			ib_fprintf(file, "EXTENT %p:\n", extent);
		for (MemoryBlock *blk = (MemoryBlock *)((char*)extent+MEM_ALIGN(sizeof(MemoryExtent))); 
			; 
			blk = (MemoryBlock *)((char*)blk+MEM_ALIGN(sizeof(MemoryBlock))+blk->length))
		{
			void *mem = (char*)blk + MEM_ALIGN(sizeof(MemoryBlock));
			if (blk->used && (blk->type>0 || !used_only)) {
#ifdef DEBUG_GDS_ALLOC
				if (blk->type > 0)
					ib_fprintf(file, "Block %p: size=%d allocated at %s:%d\n", 
						mem, blk->length, blk->file, blk->line);
				else if (blk->type == 0)
					ib_fprintf(file, "Typed block %p: type=%d size=%d allocated at %s:%d\n", 
						mem, blk->type, blk->length, blk->file, blk->line);
				else
					ib_fprintf(file, "Typed block %p: type=%d size=%d\n", 
						mem, blk->type, blk->length);
#else
				if (blk->type)
					ib_fprintf(file, "Typed block %p: type=%d size=%d\n", mem, blk->type, blk->length);
				else
					ib_fprintf(file, "Block %p: size=%d\n", mem, blk->length);
#endif
			} else {
				if (!used_only) 
					ib_fprintf(file, "Free block %p: size=%d\n", mem, blk->length);
			}
			if (blk->last) break;
		}
	}
	lock.leave();
}

MemoryPool* MemoryPool::internal_create(size_t instance_size, int *cur_mem, int *max_mem) {
	if (!cur_mem) cur_mem = &process_current_memory;
	if (!max_mem) max_mem = &process_max_memory;

	size_t alloc_size = FB_MAX(
		// This is the exact initial layout of memory pool in the first extent //
		MEM_ALIGN(sizeof(MemoryExtent)) +
		MEM_ALIGN(sizeof(MemoryBlock)) +
		MEM_ALIGN(instance_size) +
		MEM_ALIGN(sizeof(MemoryBlock)) +
		MEM_ALIGN(sizeof(FreeBlocksTree::ItemList)) +
		MEM_ALIGN(sizeof(MemoryBlock)) +
		ALLOC_ALIGNMENT,
		// ******************************************************************* //
		MIN_EXTENT_SIZE);

	char* mem = (char *)external_alloc(alloc_size);
	if (!mem) pool_out_of_memory();
	((MemoryExtent *)mem)->next = NULL;
	MemoryPool* pool = new(mem +
		MEM_ALIGN(sizeof(MemoryExtent)) +
		MEM_ALIGN(sizeof(MemoryBlock))) 
	MemoryPool(mem, mem + 
		MEM_ALIGN(sizeof(MemoryExtent)) + 
		MEM_ALIGN(sizeof(MemoryBlock)) + 
		MEM_ALIGN(instance_size) + 
		MEM_ALIGN(sizeof(MemoryBlock)),
		cur_mem, max_mem);
		
	pool->extents_memory = alloc_size - MEM_ALIGN(sizeof(MemoryExtent));
		
	MemoryBlock *poolBlk = (MemoryBlock*) (mem+MEM_ALIGN(sizeof(MemoryExtent)));
	poolBlk->pool = pool;
	poolBlk->used = true;
	poolBlk->last = false;
	poolBlk->type = TYPE_POOL;
	poolBlk->length = MEM_ALIGN(instance_size);
	poolBlk->prev = NULL;
	
	MemoryBlock *hdr = (MemoryBlock*) (mem +
		MEM_ALIGN(sizeof(MemoryExtent)) +
		MEM_ALIGN(sizeof(MemoryBlock)) +
		MEM_ALIGN(instance_size));
	hdr->pool = pool;
	hdr->used = true;
	hdr->last = false;
	hdr->type = TYPE_LEAFPAGE;
	hdr->length = MEM_ALIGN(sizeof(FreeBlocksTree::ItemList));
	hdr->prev = poolBlk;
	MemoryBlock *blk = (MemoryBlock *)(mem +
		MEM_ALIGN(sizeof(MemoryExtent)) +
		MEM_ALIGN(sizeof(MemoryBlock)) +
		MEM_ALIGN(instance_size) +
		MEM_ALIGN(sizeof(MemoryBlock)) +
		MEM_ALIGN(sizeof(FreeBlocksTree::ItemList)));
	int blockLength = alloc_size -
		MEM_ALIGN(sizeof(MemoryExtent)) -
		MEM_ALIGN(sizeof(MemoryBlock)) -
		MEM_ALIGN(instance_size) -
		MEM_ALIGN(sizeof(MemoryBlock)) -
		MEM_ALIGN(sizeof(FreeBlocksTree::ItemList)) -
		MEM_ALIGN(sizeof(MemoryBlock));
	blk->pool = pool;
	blk->used = false;
	blk->last = true;
	blk->type = 0;
	blk->length = blockLength;
	blk->prev = hdr;
	BlockInfo temp = {blk, blockLength};
	pool->freeBlocks.add(temp);
	pool->updateSpare();
	return pool;
}

void MemoryPool::deletePool(MemoryPool* pool) {
	/* dimitr: I think we need an abstract base class or a global macro
			   in locks.h to avoid these architecture checks. */
#ifdef MULTI_THREAD
	pool->lock.~Spinlock();
#else
	pool->lock.~SharedSpinlock();
#endif
	if (pool->cur_memory) *pool->cur_memory -= pool->used_memory;
	// Delete all extents now
	MemoryExtent *temp = pool->extents;
	while (temp) {
		MemoryExtent *next = temp->next;
		external_free(temp);
		temp = next;
	}
}

void* MemoryPool::internal_alloc(size_t size, SSHORT type
#ifdef DEBUG_GDS_ALLOC
	, char* file, int line
#endif
) {
	// Lookup a block greater or equal than size in freeBlocks tree
	size = MEM_ALIGN(size);
	BlockInfo blTemp = {NULL, size};
	MemoryBlock* blk;
	if (freeBlocks.locate(locGreatEqual,blTemp)) {
		// Found large enough block
		BlockInfo* current = &freeBlocks.current();
		if (current->length-size < MEM_ALIGN(sizeof(MemoryBlock))+ALLOC_ALIGNMENT) {
			blk = current->block;
			// Block is small enough to be returned AS IS
			blk->used = true;
			blk->type = type;
#ifdef DEBUG_GDS_ALLOC
			blk->file = file;
			blk->line = line;
#endif
			freeBlocks.fastRemove();
		} else {
			// Cut a piece at the end of block in hope to avoid structural
			// modification of free blocks tree
			current->block->length -= MEM_ALIGN(sizeof(MemoryBlock))+size;
			blk = (MemoryBlock *)((char*)current->block + 
				MEM_ALIGN(sizeof(MemoryBlock)) + current->block->length);
			blk->pool = this;
			blk->used = true;
#ifdef DEBUG_GDS_ALLOC
			blk->file = file;
			blk->line = line;
#endif
			blk->last = current->block->last;
			current->block->last = false;
			blk->type = type;
			blk->length = size;
			blk->prev = current->block;
			if (!blk->last)
				((MemoryBlock *)((char*)blk + MEM_ALIGN(sizeof(MemoryBlock)) + blk->length))->prev = blk;
			// Update tree of free blocks
			if (!freeBlocks.getPrev() || freeBlocks.current().length < current->block->length)
				current->length = current->block->length;
			else {
				// Tree needs to be modified structurally
#ifdef NDEBUG
				freeBlocks.getNext();
#else
				bool res = freeBlocks.getNext();
				assert(res);
				assert(&freeBlocks.current()==current);
#endif
				MemoryBlock *block = current->block;
				freeBlocks.fastRemove();
				addFreeBlock(block);
			}
		}
	} else {
		// If we are in a critically low memory condition look up for a block in a list
		// of pending free blocks. We do not do "best fit" in this case
		PendingFreeBlock *itr = pendingFree, *prev = NULL;
		while (itr) {
			MemoryBlock *temp = (MemoryBlock *)((char*)itr-MEM_ALIGN(sizeof(MemoryBlock)));
			if (temp->length >= size) {
				if (temp->length-size < MEM_ALIGN(sizeof(MemoryBlock))+ALLOC_ALIGNMENT) {
					// Block is small enough to be returned AS IS
					temp->used = true;
					temp->type = type;
#ifdef DEBUG_GDS_ALLOC
					temp->file = file;
					temp->line = line;
#endif
					// Remove block from linked list
					if (prev)
						prev->next = itr->next;
					else
						pendingFree = itr->next;						
					// We can do this w/o any locking because 
					// (1) -= integer operation is atomic on all current platforms 
					// (2) nobody will die if max_memory will be a little inprecise
					used_memory += temp->length + MEM_ALIGN(sizeof(MemoryBlock));
					if (cur_memory) {
						*cur_memory += temp->length + MEM_ALIGN(sizeof(MemoryBlock));
						if (max_memory && *max_memory < *cur_memory) *max_memory = *cur_memory;
					}
					PATTERN_FILL(itr,size,ALLOC_PATTERN);
					return itr;
				} else {
					// Cut a piece at the end of block
					// We don't need to modify tree of free blocks or a list of
					// pending free blocks in this case
					temp->length -= MEM_ALIGN(sizeof(MemoryBlock))+size;
					blk = (MemoryBlock *)((char*)temp + 
						MEM_ALIGN(sizeof(MemoryBlock)) + temp->length);
					blk->pool = this;
					blk->used = true;
#ifdef DEBUG_GDS_ALLOC
					blk->file = file;
					blk->line = line;
#endif
					blk->last = temp->last;
					temp->last = false;
					blk->type = type;
					blk->length = size;
					blk->prev = temp;
					if (!blk->last)
						((MemoryBlock *)((char*)blk + MEM_ALIGN(sizeof(MemoryBlock)) + blk->length))->prev = blk;
					used_memory += blk->length + MEM_ALIGN(sizeof(MemoryBlock));
					if (cur_memory) {
						*cur_memory += blk->length + MEM_ALIGN(sizeof(MemoryBlock));
						if (max_memory && *max_memory < *cur_memory) *max_memory = *cur_memory;
					}
					void *result = (char *)blk + MEM_ALIGN(sizeof(MemoryBlock));
					PATTERN_FILL(result,size,ALLOC_PATTERN);
					return result;
				}
			}
			prev = itr;
			itr = itr->next;
		}
		// No large enough block found. We need to extend the pool
		size_t alloc_size = FB_MAX(MEM_ALIGN(sizeof(MemoryExtent))+MEM_ALIGN(sizeof(MemoryBlock))+size, MIN_EXTENT_SIZE);
		MemoryExtent* extent = (MemoryExtent *)external_alloc(alloc_size);
		if (!extent) {
			return NULL;
		}
		extents_memory += alloc_size - MEM_ALIGN(sizeof(MemoryExtent));
		extent->next = extents;
		extents = extent;
			
		blk = (MemoryBlock *)((char*)extent+MEM_ALIGN(sizeof(MemoryExtent)));
		blk->pool = this;
		blk->used = true;
		blk->type = type;
#ifdef DEBUG_GDS_ALLOC
		blk->file = file;
		blk->line = line;
#endif
		blk->prev = NULL;
		if (alloc_size-size-MEM_ALIGN(sizeof(MemoryExtent))-MEM_ALIGN(sizeof(MemoryBlock)) < MEM_ALIGN(sizeof(MemoryBlock))+ALLOC_ALIGNMENT) {
			// Block is small enough to be returned AS IS
			blk->last = true;
			blk->length = alloc_size - MEM_ALIGN(sizeof(MemoryExtent)) - MEM_ALIGN(sizeof(MemoryBlock));
		} else {
			// Cut a piece at the beginning of the block
			blk->last = false;
			blk->length = size;
			// Put the rest to the tree of free blocks
			MemoryBlock *rest = (MemoryBlock *)((char *)blk + MEM_ALIGN(sizeof(MemoryBlock)) + size);
			rest->pool = this;
			rest->used = false;
			rest->last = true;
			rest->length = alloc_size - MEM_ALIGN(sizeof(MemoryExtent)) - 
				MEM_ALIGN(sizeof(MemoryBlock)) - size - MEM_ALIGN(sizeof(MemoryBlock));
			rest->prev = blk;
			addFreeBlock(rest);
		}
	}
	used_memory += blk->length + MEM_ALIGN(sizeof(MemoryBlock));
	if (cur_memory) {
		*cur_memory += blk->length + MEM_ALIGN(sizeof(MemoryBlock));
		if (max_memory && *max_memory < *cur_memory) *max_memory = *cur_memory;
	}
	void *result = (char*)blk+MEM_ALIGN(sizeof(MemoryBlock));
	// Grow spare blocks pool if necessary
	PATTERN_FILL(result,size,ALLOC_PATTERN);
	return result;
}

void MemoryPool::addFreeBlock(MemoryBlock *blk) {
	BlockInfo info = {blk, blk->length};
	try {
		freeBlocks.add(info);
	} catch(const std::exception&) {
		// Add item to the list of pending free blocks in case of critically-low memory condition
		PendingFreeBlock* temp = (PendingFreeBlock *)((char *)freeBlocks.getAddErrorValue().block+
			MEM_ALIGN(sizeof(MemoryBlock)));
		temp->next = pendingFree;
		pendingFree = temp;
	}
}
		
void MemoryPool::removeFreeBlock(MemoryBlock *blk) {
	BlockInfo info = {blk, blk->length};
	if (freeBlocks.locate(info)) {
		freeBlocks.fastRemove();
	} else {
		// If we are in a critically-low memory condition our block could be in the
		// pending free blocks list. Remove it from there
		PendingFreeBlock *itr = pendingFree, 
			*temp = (PendingFreeBlock *)((char *)blk+MEM_ALIGN(sizeof(MemoryBlock)));
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
			assert(itr); // We had to find it somewhere
		}
	}
}

void MemoryPool::free_blk_extent(MemoryBlock *blk) {
	MemoryExtent *extent = (MemoryExtent *)((char *)blk-MEM_ALIGN(sizeof(MemoryExtent)));
	MemoryExtent *itr = extents;
	if (extents == extent)
		extents = extents->next;
	else
	{
  		while ( itr ) {
			MemoryExtent *next = itr->next;
			if (next==extent) {
				itr->next = extent->next;
				break;
			}
			itr = next;
		}
		assert(itr); // We had to find it somewhere
	}
	extents_memory -= blk->length + MEM_ALIGN(sizeof(MemoryBlock));
	external_free(extent);
}

void MemoryPool::deallocate(void *block) {
	if (!block) return;
	lock.enter();
	MemoryBlock *blk = (MemoryBlock *)((char*)block - MEM_ALIGN(sizeof(MemoryBlock))), *prev;
	assert(blk->used);
	assert(blk->pool==this);
	used_memory -= blk->length + MEM_ALIGN(sizeof(MemoryBlock));
	if (cur_memory)	*cur_memory -= blk->length + MEM_ALIGN(sizeof(MemoryBlock));
	// Try to merge block with preceding free block
	if ((prev = blk->prev) && !prev->used) {
		removeFreeBlock(prev);
		prev->length += blk->length + MEM_ALIGN(sizeof(MemoryBlock));

		MemoryBlock *next = NULL;
		if (blk->last) {
			prev->last = true;
		} else {
			next = (MemoryBlock *)((char *)blk+MEM_ALIGN(sizeof(MemoryBlock))+blk->length);
			if (next->used) {
				next->prev = prev;
				prev->last = false;
			} else {
				// Merge next block too
				removeFreeBlock(next);
				prev->length += next->length + MEM_ALIGN(sizeof(MemoryBlock));
				prev->last = next->last;
				if (!next->last)
					((MemoryBlock *)((char *)next+MEM_ALIGN(sizeof(MemoryBlock))+next->length))->prev = prev;
			}
		}
		PATTERN_FILL((char*)prev+MEM_ALIGN(sizeof(MemoryBlock)),prev->length,FREE_PATTERN);
		if (!prev->prev && prev->last)
			free_blk_extent(prev);
		else
			addFreeBlock(prev);
	} else {	
		MemoryBlock *next;
		// Mark block as free
		blk->used = false;
		// Try to merge block with next free block
		if (!blk->last && 
			!(next = (MemoryBlock *)((char*)blk+MEM_ALIGN(sizeof(MemoryBlock))+blk->length))->used) 
		{
			removeFreeBlock(next);
			blk->length += next->length + MEM_ALIGN(sizeof(MemoryBlock));
			blk->last = next->last;
			if (!next->last)
				((MemoryBlock *)((char *)next+MEM_ALIGN(sizeof(MemoryBlock))+next->length))->prev = blk;
		}
		PATTERN_FILL(block,blk->length,FREE_PATTERN);
		if (!blk->prev && blk->last)
			free_blk_extent(blk);
		else
			addFreeBlock(blk);
	}
	if (needSpare) updateSpare();
	lock.leave();
}

} /* namespace Firebird */

#ifndef TESTING_ONLY

Firebird::MemoryPool* getDefaultMemoryPool() {
	if (!Firebird::processMemoryPool) Firebird::processMemoryPool = MemoryPool::createPool();
	return Firebird::processMemoryPool;
}

void* operator new(size_t s) THROW_BAD_ALLOC {
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

void* operator new[](size_t s) THROW_BAD_ALLOC {
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

void operator delete(void* mem) throw() {
	Firebird::MemoryPool::globalFree(mem);
}

void operator delete[](void* mem) throw() {
	Firebird::MemoryPool::globalFree(mem);
}

#endif
