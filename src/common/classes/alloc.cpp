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

#include "alloc.h"
#include <new>

// Size in bytes, must be aligned according to ALLOC_ALIGNMENT
#define MIN_EXTENT_SIZE  100
// Must be a power of 2
#define ALLOC_ALIGNMENT 4

#define ALIGN(X) ((X+ALLOC_ALIGNMENT-1) & ~(ALLOC_ALIGNMENT-1))

#define FB_MAX(M,N) ((M)>(N)?(M):(N))

namespace Firebird {

// Helper function to reduce code size, since many compilers
// generate quite a bit of code at the point of the throw.
static void pool_out_of_memory()
{
	// FIXME: this is a temporary solution until we switch out of using STL exceptions
	throw std::bad_alloc();
}

void* MemoryPool::external_alloc(size_t size) {
	// This method is assumed to throw exceptions in case it cannot alloc
	return malloc(size);
}
	
void MemoryPool::external_free(void *blk) {
	::free(blk);
}

MemoryPool* MemoryPool::createPool() {
	size_t alloc_size = FB_MAX(
		// This is the exact initial layout of memory pool in the first extent //
		ALIGN(sizeof(MemoryExtent))+
		ALIGN(sizeof(MemoryPool))+
		ALIGN(sizeof(MemoryBlock))+
		ALIGN(sizeof(FreeBlocksTree::ItemList))+
		ALIGN(sizeof(MemoryBlock))+
		ALLOC_ALIGNMENT,
		// ******************************************************************* //
		MIN_EXTENT_SIZE);
			
	char* mem = (char *)external_alloc(alloc_size);
	if (!mem) pool_out_of_memory();
	((MemoryExtent *)mem)->next = NULL;
	MemoryPool* pool = new(mem+ALIGN(sizeof(MemoryExtent))) MemoryPool(mem, mem + 
	  ALIGN(sizeof(MemoryExtent)) + ALIGN(sizeof(MemoryPool)) + ALIGN(sizeof(MemoryBlock)));
	
	MemoryBlock *hdr = (MemoryBlock*) (mem+ALIGN(sizeof(MemoryExtent))+ALIGN(sizeof(MemoryPool)));
	hdr->pool = pool;
	hdr->used = true;
	hdr->last = false;
	hdr->type = TYPE_LEAFPAGE;
	hdr->length = ALIGN(sizeof(MemoryBlock))+ALIGN(sizeof(FreeBlocksTree::ItemList));
	hdr->prev = NULL;
	MemoryBlock *blk = (MemoryBlock *)(mem+ALIGN(sizeof(MemoryExtent))+
		ALIGN(sizeof(MemoryPool))+ALIGN(sizeof(MemoryBlock))+
		ALIGN(sizeof(FreeBlocksTree::ItemList)));
	int blockLength = alloc_size - (ALIGN(sizeof(MemoryExtent))+
		ALIGN(sizeof(MemoryPool))+ALIGN(sizeof(MemoryBlock))+ALIGN(sizeof(FreeBlocksTree::ItemList))+
		ALIGN(sizeof(MemoryBlock)));
	blk->pool = pool;
	blk->used = false;
	blk->last = true;
	blk->type = 0;
	blk->length = blockLength;
	blk->prev = hdr;
	BlockInfo temp = {blk, blockLength};
	pool->freeBlocks.add(temp);
	// This code may not work if tree factor is 2 (but if MIN_EXTENT_SIZE is large enough it will)
	pool->spareLeaf = pool->alloc(sizeof(FreeBlocksTree::ItemList));
	pool->spareNodes.add(pool->alloc(sizeof(FreeBlocksTree::NodeList)));
	return pool;
}

void MemoryPool::deletePool(MemoryPool* pool) {
	// Delete all extents now
	MemoryExtent *temp = pool->extents;
	while (temp) {
		MemoryExtent *next = temp->next;
		external_free(temp);
		temp = next;
	}
}

void* MemoryPool::alloc(size_t size, SSHORT type) {
	if (internalAlloc) {
		if (size == sizeof(FreeBlocksTree::ItemList))
			// This condition is to handle case when nodelist and itemlist have equal size
			if (sizeof(FreeBlocksTree::ItemList)!=sizeof(FreeBlocksTree::ItemList) || spareLeaf) {
				void *temp = spareLeaf;
				spareLeaf = NULL;	
				needSpare = true;
				if (!temp) pool_out_of_memory();
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
	}
	// Lookup a block greater or equal than size in freeBlocks tree
	size = ALIGN(size);
	BlockInfo temp = {NULL, size};
	void *result;
	if (freeBlocks.locate(locGreatEqual,temp)) {
		// Found large enough block
		BlockInfo* current = &freeBlocks.current();
		if (current->length-size < sizeof(MemoryBlock)+ALLOC_ALIGNMENT) {
			// Block is small enough to be returned AS IS
			current->block->used = true;
			current->block->type = type;
			result = current->block+1;
			freeBlocks.fastRemove();
		} else {
			// Cut a piece at the end of block in hope to avoid structural
			// modification of free blocks tree
			current->block->length -= sizeof(MemoryBlock)+size;
			MemoryBlock *blk = (MemoryBlock *)((char*)(current->block+1) + current->block->length);
			blk->pool = this;
			blk->used = true;
			blk->last = current->block->last;
			current->block->last = false;
			blk->type = type;
			blk->length = size;
			blk->prev = current->block;
			if (!blk->last)
				((MemoryBlock *)((char*)(blk+1) + blk->length))->prev = blk;
			// Update tree of free blocks
			if (!freeBlocks.getPrev() || freeBlocks.current().length <= current->block->length)
				current->length = current->block->length;
			else {
				// Tree needs to be modified structurally
				bool res = freeBlocks.getNext();
				assert(res);
				BlockInfo temp = {current->block, current->block->length};
				freeBlocks.fastRemove();
				internalAlloc = true;
				try {
					freeBlocks.add(temp);
				} catch(...) {
					// Add item to the list of pending free blocks in case of critically-low memory condition
					PendingFreeBlock* temp = (PendingFreeBlock *)(freeBlocks.getAddErrorValue().block+1);
					temp->next = pendingFree;
					pendingFree = temp;
				}
				internalAlloc = false;
			}
			result = blk+1;
		}
	} else {
		// If we are in a critically low memory condition look up for a block in a list
		// of pending free blocks. We do not do "best fit" in this case
		PendingFreeBlock *itr = pendingFree, *prev = NULL;
		while (itr) {
			MemoryBlock *temp = (MemoryBlock *)itr-1;
			if (temp->length >= size) {
				if (temp->length-size < sizeof(MemoryBlock)+ALLOC_ALIGNMENT) {
					// Block is small enough to be returned AS IS
					temp->used = true;
					temp->type = type;
					// Remove block from linked list
					if (prev)
						prev->next = itr->next;
					else
						pendingFree = itr->next;
					return itr;
				} else {
					// Cut a piece at the end of block
					// We don't need to modify tree of free blocks or a list of
					// pending free blocks in this case
					temp->length -= sizeof(MemoryBlock)+size;
					MemoryBlock *blk = (MemoryBlock *)((char*)(temp+1) + temp->length);
					blk->pool = this;
					blk->used = true;
					blk->last = temp->last;
					temp->last = false;
					blk->type = type;
					blk->length = size;
					blk->prev = temp;
					if (!blk->last)
						((MemoryBlock *)((char*)(blk+1) + blk->length))->prev = blk;
					return blk+1;
				}
			}
			prev = itr;
			itr = itr->next;
		}
		// No large enough block found. We need to extend the pool
		size_t alloc_size = FB_MAX(sizeof(MemoryExtent)+sizeof(MemoryBlock)+size, MIN_EXTENT_SIZE);
		MemoryExtent* extent = (MemoryExtent *)external_alloc(alloc_size);
		if (!extent) pool_out_of_memory();
		extent->next = extents;
		extents = extent;
			
		MemoryBlock *blk = (MemoryBlock *)(extent+1);
		blk->pool = this;
		blk->used = true;
		blk->type = type;
		blk->prev = NULL;
		if (alloc_size-size < sizeof(MemoryBlock)+ALLOC_ALIGNMENT) {
			// Block is small enough to be returned AS IS
			blk->last = true;
			blk->length = alloc_size - sizeof(MemoryExtent) - sizeof(MemoryBlock);
		} else {
			// Cut a piece at the beginning of the block
			MemoryBlock *blk = (MemoryBlock *)(extent+1);
			blk->last = false;
			blk->length = size;
			// Put the rest to the tree of free blocks
			MemoryBlock *rest = (MemoryBlock *)((char *)blk + size);
			rest->pool = this;
			rest->used = false;
			rest->last = true;
			rest->length = alloc_size - sizeof(MemoryExtent) - 
				sizeof(MemoryBlock) - size - sizeof(MemoryBlock);
			rest->prev = blk;
			BlockInfo temp = {rest, rest->length};
			internalAlloc = true;
			try {
				freeBlocks.add(temp);
			} catch(...) {
				// Add item to the list of pending free blocks in case of critically-low memory condition
				PendingFreeBlock* temp = (PendingFreeBlock *)(freeBlocks.getAddErrorValue().block+1);
				temp->next = pendingFree;
				pendingFree = temp;
			}
			internalAlloc = false;
		}
		result = blk+1;
	}
	// Grow spare blocks pool if necessary
	if (needSpare) {
		try {
			if (!spareLeaf)
				spareLeaf = alloc(sizeof(FreeBlocksTree::ItemList), TYPE_LEAFPAGE);
			while (spareNodes.getCount() <= freeBlocks.level)
				spareNodes.add(alloc(sizeof(FreeBlocksTree::NodeList), TYPE_TREEPAGE));
			needSpare = false;
			// We do not try to add pending blocks here because it is REALLY unlikely
			// that we'll be able to recover after critically low memory condition
			// during alloc()
		} catch(...) {
			// We can recover after this
		}
	}
	return result;
}

void MemoryPool::free(void *block) {
	MemoryBlock *blk = (MemoryBlock *)block - 1, *prev;
	// Try to merge block with preceding free block
	if ((prev = blk->prev) && !prev->used) {
		BlockInfo temp = {prev, prev->length};
		if (freeBlocks.locate(temp)) {
			freeBlocks.fastRemove();
		} else {
			// If we are in a critically-low memory condition our block could be in the
			// pending free blocks list. Remove it from there
			PendingFreeBlock *itr = pendingFree, *temp = (PendingFreeBlock *)(prev+1);
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
			}
			assert(itr); // We had to find it somewhere
		}
		prev->length += blk->length + sizeof(MemoryBlock);
		prev->last = blk->last;
		if (!blk->last)
			((MemoryBlock *)((char *)(blk+1)+blk->length))->prev = prev;
		temp.length = prev->length;
		internalAlloc = true;
		try {
			freeBlocks.add(temp);
		} catch(...) {
			// Add item to the list of pending free blocks in case of critically-low memory condition
			PendingFreeBlock* temp = (PendingFreeBlock *)(freeBlocks.getAddErrorValue().block+1);
			temp->next = pendingFree;
			pendingFree = temp;
		}
		internalAlloc = false;
	} else {	
		// Try to merge block with next free block
		if (!blk->last) {
			MemoryBlock *next = (MemoryBlock *)((char*)(blk+1)+blk->length);
			if (!next->used) {
				blk->length+=next->length+sizeof(MemoryBlock);
				blk->last = next->last;
				if (!next->last)
					((MemoryBlock *)((char *)(next+1)+next->length))->prev = prev;
				BlockInfo temp = {next, next->length};
				if (freeBlocks.locate(temp)) {
					freeBlocks.fastRemove();
				} else {
					// If we are in a critically-low memory condition our block could be in the
					// pending free blocks list. Remove it from there
		  			PendingFreeBlock *itr = pendingFree, *temp = (PendingFreeBlock *)(prev+1);
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
					}
					assert(itr); // We had to find it somewhere
				}
			}
		}
		// Mark block as free and add it to freeBlocks array
		blk->used = false;
		BlockInfo temp = {blk, blk->length};
		internalAlloc = true;
		try {
			freeBlocks.add(temp);
		} catch(...) {
			// Add item to the list of pending free blocks in case of critically-low memory condition
			PendingFreeBlock* temp = (PendingFreeBlock *)(freeBlocks.getAddErrorValue().block+1);
			temp->next = pendingFree;
			pendingFree = temp;
		}
		internalAlloc = false;
	}
	// Grow spare blocks pool if necessary
	if (needSpare) {
		updateSpareBlocks: {
			try {
				if (!spareLeaf)
					spareLeaf = alloc(sizeof(FreeBlocksTree::ItemList), TYPE_LEAFPAGE);
				while (spareNodes.getCount() <= freeBlocks.level)
					spareNodes.add(alloc(sizeof(FreeBlocksTree::NodeList), TYPE_TREEPAGE));
				needSpare = false;
				// Great, if we were able to restore free blocks tree operations after critically low
				// memory condition then try to add pending free blocks to our tree
				while (pendingFree) {
					PendingFreeBlock *temp = pendingFree;
					pendingFree = temp->next;
					BlockInfo info = {(MemoryBlock*)temp-1, ((MemoryBlock*)temp)->length};
					internalAlloc = true;
					freeBlocks.add(info); // We should be able to do this because we had spare blocks
					internalAlloc = false;
					if (needSpare)
						goto updateSpareBlocks;
				}
			} catch(...) {
				// We can recover after this
			}
		}
	}
}

} /* namespace Firebird */
