//____________________________________________________________
//  
//		PROGRAM:	Alice (All Else) Utility
//		MODULE:		all.cpp
//		DESCRIPTION:	Block allocator
//  
//  The contents of this file are subject to the Interbase Public
//  License Version 1.0 (the "License"); you may not use this file
//  except in compliance with the License. You may obtain a copy
//  of the License at http://www.Inprise.com/IPL.html
//  
//  Software distributed under the License is distributed on an
//  "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
//  or implied. See the License for the specific language governing
//  rights and limitations under the License.
//  
//  The Original Code was created by Inprise Corporation
//  and its predecessors. Portions created by Inprise Corporation are
//  Copyright (C) Inprise Corporation.
//  
//  All Rights Reserved.
//  Contributor(s): ______________________________________.
//  
//
//____________________________________________________________
//
//	$Id: all.cpp,v 1.1.1.1 2001-05-23 13:25:33 tamlin Exp $
//

#include "../jrd/ib_stdio.h"
#include "../jrd/common.h"
#include "../jrd/ibsetjmp.h"
#include "../alice/alice.h"
#include "../alice/all.h"
#include "../alice/alloc.h"
#include "../alice/lls.h"
#include "../alice/all_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/thd_proto.h"

#define BLKDEF(type, root, tail) sizeof (struct root), tail,
static struct {
	SSHORT typ_root_length;
	SSHORT typ_tail_length;
} block_sizes[] = {
	0, 0,
#include "../alice/blk.h"
0};

#undef BLKDEF

static void extend_pool(PLB, SLONG);
static PLB find_pool(BLK);
static void release(FRB, PLB);



//____________________________________________________________
//  
//		Allocate a block from a given pool and initialize the block.
//		This is the primary block allocation routine.
//  

BLK ALLA_alloc(PLB pool, UCHAR type, int count)
{
	register BLK block;
	FRB free, *best, *ptr;
	SLONG best_tail, tail;
	USHORT units;
	register SLONG size;
	register USHORT *p;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

	if (type <= (UCHAR) type_MIN || type >= (SCHAR) type_MAX)
		ALICE_error(62, 0, 0, 0, 0, 0);	/* msg 62: bad block type */

//  Compute block length 

	size = block_sizes[type].typ_root_length;

	if ((tail = block_sizes[type].typ_tail_length) && count >= FUDGE)
		size += (count - FUDGE) * tail;

	size = (size + MIN_ALLOC - 1) & ~((ULONG) MIN_ALLOC - 1);
//  TMN: Here we should probably have the following assert 
//  assert((size >> SHIFT) <= MAX_USHORT); 
	units = (USHORT) (size >> SHIFT);

	if (size >= MAX_BLOCK)
		ALICE_error(63, 0, 0, 0, 0, 0);	/* msg 63:internal block exceeds maximum size */

//  Find best fit.  Best fit is defined to be the free block of shortest
//  tail.  If there isn't a fit, extend the pool and try, try again. 

	while (TRUE) {
		best = NULL;
		best_tail = MAX_BLOCK;
		for (ptr = &pool->plb_free; (free = *ptr); ptr = &free->frb_next) {
			if (free == free->frb_next)
				ALICE_error(64, 0, 0, 0, 0, 0);	/* msg 64: corrupt pool */
			if ((tail = (int) free->frb_header.blk_length - (int) units) >= 0
				&& tail < best_tail) {
				best = ptr;
				best_tail = tail;
				if (tail == 0)
					break;
			}
		}
		if (best)
			break;
		extend_pool(pool, size);
	}

//  We've got our free block.  If there's enough left of the free block
//  after taking out our block, chop out out block.  If not, allocate
//  the entire free block as our block (a little extra won't hurt). 

	best_tail <<= SHIFT;
	free = *best;

	if (best_tail > sizeof(struct frb)) {
		block =
			(BLK) ((SCHAR *) free + (free->frb_header.blk_length << SHIFT));
		block = (BLK) ((SCHAR *) block - size);
		free->frb_header.blk_length -= units;
	}
	else {
		*best = free->frb_next;
		units = free->frb_header.blk_length;
		size = units << SHIFT;
		block = (BLK) free;
	}

//  zero out the allocated memory 

	size >>= 1;
	p = (USHORT *) block;
	do
		*p++ = 0;
	while (--size);

	block->blk_type = type;
//  TMN: Here we should probably have the following assert 
//  assert(pool->plb_pool_id <= MAX_UCHAR); 
	block->blk_pool_id = (UCHAR) pool->plb_pool_id;
	block->blk_length = units;

	return block;
}


//____________________________________________________________
//  
//		Extend a repeating block, copying the constant part.
//  

BLK ALLA_extend(BLK * pointer, int size)
{
	BLK block;
	PLB pool;
	SLONG old_length, new_length;
	register SCHAR *from, *to;
	register SSHORT length;

	block = *pointer;
	pool = find_pool(block);
	BLK new_ = ALLA_alloc(pool, block->blk_type, size);

	old_length = block->blk_length << SHIFT;
	new_length = new_->blk_length << SHIFT;

	from = (SCHAR *) block + sizeof(struct blk);
	to = (SCHAR *) new_ + sizeof(struct blk);
	length = MIN(old_length, new_length) - sizeof(struct blk);
	if (length)
		do
			*to++ = *from++;
		while (--length);

	release(reinterpret_cast < frb * >(block), pool);

	if (new_->blk_type == (UCHAR) type_vec)
		((VEC) new_)->vec_count = size;
	else if (new_->blk_type == (UCHAR) type_vcl)
		((VCL) new_)->vcl_count = size;

	*pointer = new_;

	return new_;
}


//____________________________________________________________
//  
//		Get rid of everything.
//  

void ALLA_fini(void)
{
	register PLB pool, *vector, *until;
	register HNK hunks, hunk;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

	if (tdgbl->pools) {
		for (vector =
			 (PLB *) tdgbl->pools->vec_object + tdgbl->pools->vec_count,
			 until = (PLB *) tdgbl->pools->vec_object; --vector >= until;)
			if (pool = *vector)
				for (hunks = pool->plb_hunks; hunk = hunks;) {
					hunks = hunk->hnk_next;
					ALLA_free(hunk->hnk_address);
				}
	}

	tdgbl->pools = NULL;
}


//____________________________________________________________
//  
//		Give space back to system.
//  

void ALLA_free(SCHAR * memory)
{

	gds__free(memory);
}


//____________________________________________________________
//  
//		Initialize the pool system.
//  

void ALLA_init(void)
{
	SLONG temp_vector[20];
	VEC vector;
	PLB pool;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

	tdgbl->pools = vector = (VEC) temp_vector;
	vector->vec_count = 1;
	vector->vec_object[0] = NULL;

	tdgbl->ALICE_default_pool = tdgbl->ALICE_permanent_pool = pool =
		ALLA_pool();
	tdgbl->pools = vector = (VEC) ALLA_alloc(pool, type_vec, 10);
	vector->vec_count = 10;
	vector->vec_object[0] = (BLK) pool;
}


//____________________________________________________________
//  
//		Get memory from system.
//  

SCHAR *ALLA_malloc(SLONG size)
{
	register SCHAR *memory;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

	if (memory = (SCHAR *) gds__alloc(size))
		return memory;

	ALICE_error(65, 0, 0, 0, 0, 0);	/* msg 65: virtual memory exhausted */
	return (SCHAR *) 0;			/* Keep compilers happy. ALICE_error() never returns. */
}


//____________________________________________________________
//  
//		Allocate a new pool.  This is done by creating a tempory
//		pool block on the stack, then allocating a real pool block.
//		In USHORT, by mirrors.
//  

PLB ALLA_pool(void)
{
	struct plb temp_pool;
	register VEC vector;
	register PLB pool;
	register int pool_id;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;


//  Start by assigning a pool id 

	vector = tdgbl->pools;

	for (pool_id = 0; pool_id < (int) vector->vec_count; pool_id++)
		if (!(vector->vec_object[pool_id]))
			break;

	if (pool_id >= (int) vector->vec_count)
		vector =
			(VEC) ALLA_extend(reinterpret_cast < blk ** >(&(tdgbl->pools)),
							  pool_id + 10);

	vector->vec_object[pool_id] = (BLK) & temp_pool;
	temp_pool.plb_free = NULL;
	temp_pool.plb_hunks = NULL;
	temp_pool.plb_pool_id = pool_id;
	if (pool_id == 0)
		tdgbl->ALICE_permanent_pool = &temp_pool;

	pool = (PLB) ALLA_alloc(&temp_pool, type_plb, 0);
	pool->plb_pool_id = pool_id;
	pool->plb_free = temp_pool.plb_free;
	pool->plb_hunks = temp_pool.plb_hunks;
	vector->vec_object[pool_id] = (BLK) pool;

	if (pool_id == 0)
		tdgbl->ALICE_permanent_pool = pool;

	return pool;
}


//____________________________________________________________
//  
//		Push an object on an LLS stack.
//  

void ALLA_push(BLK object, register LLS * stack)
{
	register LLS node;
	PLB pool;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

	pool = tdgbl->ALICE_default_pool;

	if (node = pool->plb_lls)
		pool->plb_lls = node->lls_next;
	else
		node = (LLS) ALLA_alloc(pool, type_lls, 0);

	node->lls_object = object;
	node->lls_next = *stack;
	*stack = node;
}


//____________________________________________________________
//  
//		Pop an object off a linked list stack.  Save the node for
//		further use.
//  

BLK ALLA_pop(register LLS * stack)
{
	register LLS node;
	register PLB pool;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;


	node = *stack;
	pool = (PLB) tdgbl->pools->vec_object[node->lls_header.blk_pool_id];
	*stack = node->lls_next;
	node->lls_next = pool->plb_lls;
	pool->plb_lls = node;

	return node->lls_object;
}


//____________________________________________________________
//  
//		Release a block to its pool.  If it is contiguous to
//		another free block, combine them.  Otherwise link it
//		into the free block linked list (kept in ascending order
//		of addresses).
//  

void ALLA_release(register FRB block)
{

	release(block, find_pool(block ? &block->frb_header : (BLK) 0));
}


//____________________________________________________________
//  
//		Release a storage pool.  This involves nothing more than returning
//		hunks to the free hunk list.
//  

void ALLA_rlpool(PLB pool)
{
	register HNK hunk, hunks;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;


	tdgbl->pools->vec_object[pool->plb_pool_id] = NULL;

	for (hunks = pool->plb_hunks; hunk = hunks;) {
		hunks = hunk->hnk_next;
		ALLA_free(hunk->hnk_address);
	}
}


//____________________________________________________________
//  
//		Allocate, extend, or no-op a vector making sure it is "long enough".
//  

VEC ALLA_vector(PLB pool, VEC * ptr, USHORT count)
{
	VEC vector;

	++count;

	if (!(vector = *ptr)) {
		vector = *ptr = (VEC) ALLA_alloc(pool, type_vec, count);
		vector->vec_count = count;
		return vector;
	}

	if (vector->vec_count >= count)
		return vector;

	return (VEC) ALLA_extend(reinterpret_cast < blk ** >(ptr), count);
}


//____________________________________________________________
//  
//		Extend a pool by at least enough to accomodate a block
//		of given size.
//  

static void extend_pool(PLB pool, SLONG size)
{
	register HNK hunk;
	register BLK block;

	size =
		(size + sizeof(struct hnk) + MIN_ALLOCATION -
		 1) & ~((ULONG) MIN_ALLOCATION - 1);
	block = (BLK) ALLA_malloc((SLONG) size);
//  TMN: Here we should probably have the following assert 
//  assert((size >> SHIFT) <= MAX_USHORT); 
	block->blk_length = (USHORT) size >> SHIFT;
	block->blk_type = (UCHAR) type_frb;
//  TMN: Here we should probably have the following assert 
//  assert(pool->plb_pool_id <= MAX_UCHAR); 
	block->blk_pool_id = (UCHAR) pool->plb_pool_id;
	release(reinterpret_cast < frb * >(block), pool);

	hunk = (HNK) ALLA_alloc(pool, type_hnk, 0);
	hunk->hnk_address = (SCHAR *) block;
	hunk->hnk_length = size;
	hunk->hnk_next = pool->plb_hunks;
	pool->plb_hunks = hunk;
}


//____________________________________________________________
//  
//		Find pool associate with block.
//  

static PLB find_pool(BLK block)
{
	PLB pool;
	HNK hunk;
	USHORT pool_id;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;


	for (pool_id = block->blk_pool_id;
		 pool_id < tdgbl->pools->vec_count
		 && (pool = (PLB) tdgbl->pools->vec_object[pool_id]); pool_id += 256)
		for (hunk = pool->plb_hunks; hunk; hunk = hunk->hnk_next)
			if ((SCHAR *) block >= hunk->hnk_address
				&& (SCHAR *) block < hunk->hnk_address + hunk->hnk_length)
				return pool;

	ALICE_error(66, 0, 0, 0, 0, 0);	/* msg 66: bad pool id */
	return (PLB) 0;				/* Keep compilers happy. ALICE_error() never returns. */
}


//____________________________________________________________
//  
//		Release a block to its pool.  If it is contiguous to
//		another free block, combine them.  Otherwise link it
//		into the free block linked list (kept in ascending order
//		of addresses).
//  

static void release(FRB block, PLB pool)
{
	register FRB prior, free;
	FRB *ptr;
	SLONG length;

	block->frb_header.blk_type = (UCHAR) type_frb;
	prior = NULL;

	for (ptr = &pool->plb_free; free = *ptr;
		 prior = free, ptr = &free->frb_next) if (block < free)
			break;

//  Merge block into list first, then try to combine blocks 

	block->frb_next = free;
	*ptr = block;

//  Try to merge the free block with the next one down. 

	length = block->frb_header.blk_length << SHIFT;

	if (free && (SCHAR *) block + length == (SCHAR *) free) {
		block->frb_header.blk_length += free->frb_header.blk_length;
		block->frb_next = free->frb_next;
	}

//  Try and merge the block with the prior free block 

	if (prior &&
		(length = prior->frb_header.blk_length << SHIFT) &&
		(SCHAR *) prior + length == (SCHAR *) block) {
		prior->frb_header.blk_length += block->frb_header.blk_length;
		prior->frb_next = block->frb_next;
	}
}
