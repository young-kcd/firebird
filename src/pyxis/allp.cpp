/*
 *	PROGRAM:	PYXIS Form Package
 *	MODULE:		all.c
 *	DESCRIPTION:	Internal block allocator
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
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */
/*
$Id: allp.cpp,v 1.1.1.1 2001-05-23 13:26:33 tamlin Exp $
*/

#include "../pyxis/everything.h"


extern UCHAR *gds__alloc();

PLB PYXIS_pool();
UCHAR *PYXIS_malloc();

static VEC pools;

#define BLKDEF(type, root, tail) sizeof (struct root), tail,

static struct {
	SSHORT typ_root_length;
	SSHORT typ_tail_length;
} block_sizes[] = {
	0, 0,
#include "../pyxis/blk.h"
0};

#undef BLKDEF


BLK PYXIS_alloc(pool, type, count)
	 PLB pool;
	 UCHAR type;
	 int count;
{
/**************************************
 *
 *	P Y X I S _ a l l o c
 *
 **************************************
 *
 * Functional description
 *	Allocate a block from a given pool and initialize the block.
 *	This is the primary block allocation routine.
 *
 **************************************/
	register BLK block;
	FRB free, *best, *ptr;
	HNK hunk;
	register USHORT size, *p;
	USHORT best_tail, tail;

	if (type <= (SCHAR) type_MIN || type >= (SCHAR) type_MAX)
		BUGCHECK("bad block type");

/* Compute block length */

	size = block_sizes[type].typ_root_length;

	if ((tail = block_sizes[type].typ_tail_length) && count >= 1)
		size += (count - 1) * tail;

#ifndef VMS
	size = FB_ALIGN(size, ALIGNMENT);
#else
	size = ((size + ALIGNMENT - 1) & ~(ALIGNMENT - 1));
#endif

	if (size <= 4 || size > 32768)
		BUGCHECK("bad block size");

/* Find best fit.  Best fit is defined to be the free block of shortest
   tail.  If there isn't a fit, extend the pool and try, try again. */

	while (TRUE) {
		best = NULL;
		best_tail = 32767;
		for (ptr = &pool->plb_free; (free = *ptr); ptr = &free->frb_next)
			if ((SCHAR HUGE_PTR *) free == (SCHAR HUGE_PTR *) free->frb_next)
				BUGCHECK("corrupt pool");
			else if ((tail = free->frb_header.blk_length - size) >= 0
					 && tail < best_tail) {
				best = ptr;
				best_tail = tail;
				if (tail == 0)
					break;
			}
		if (best)
			break;
		extend_pool(pool, size);
	}

/* We've got our free block.  If there's enough left of the free block
   after taking out our block, chop out our block.  If not, allocate
   the entire free block as our block (a little extra won't hurt). */

	free = *best;
	if (best_tail > sizeof(struct frb)) {
		block = (BLK) ((SCHAR *) free + free->frb_header.blk_length - size);
		free->frb_header.blk_length -= size;
	}
	else {
		*best = free->frb_next;
		size += best_tail;
		block = (BLK) free;
	}

	block->blk_type = type;
	block->blk_pool_id = pool->plb_pool_id;
	block->blk_length = size;

	if (size -= sizeof(struct blk))
		memset((SCHAR *) block + sizeof(struct blk), 0, size);

	return block;
}


BLK PYXIS_extend(pointer, size)
	 BLK *pointer;
	 int size;
{
/**************************************
 *
 *	P Y X I S _ e x t e n d
 *
 **************************************
 *
 * Functional description
 *	Extend a repeating block, copying the constant part.
 *
 **************************************/
	BLK block, new;
	register int length, l;
	register SLONG *p1, *p2;
	register SCHAR *c1, *c2;

	block = *pointer;
	new = PYXIS_alloc(pools->vec_object[block->blk_pool_id],
					  block->blk_type, size);
	length = MIN(block->blk_length, new->blk_length) - sizeof(struct blk);
	p1 = (SLONG *) ((SCHAR *) new + sizeof(struct blk));
	p2 = (SLONG *) ((SCHAR *) block + sizeof(struct blk));

	if (l = length >> SHIFTLONG)
		do
			*p1++ = *p2++;
		while (--l);

	if (length &= 3) {
		c1 = (SCHAR *) p1;
		c2 = (SCHAR *) p2;
		do
			*c1++ = *c2++;
		while (--length);
	}

	PYXIS_release(block);

	if (new->blk_type == (SCHAR) type_vec)
		((VEC) new)->vec_count = size;

	*pointer = new;
	return new;
}


PYXIS_alloc_fini()
{
/**************************************
 *
 *	P Y X I S _ a l l o c _ f i n i
 *
 **************************************
 *
 * Functional description
 *	Get rid of everything.
 *
 **************************************/
	BLK *vector, *until;
	register PLB pool;
	register HNK hunks, hunk;

	for (vector = pools->vec_object + pools->vec_count,
		 until = pools->vec_object; --vector >= until;)
		if (pool = (PLB) * vector)
			for (hunks = pool->plb_hunks; hunk = hunks;) {
				hunks = hunk->hnk_next;
				PYXIS_free(hunk->hnk_address);
			}
}


PYXIS_free(memory)
	 SCHAR *memory;
{
/**************************************
 *
 *	P Y X I S _ f r e e
 *
 **************************************
 *
 * Functional description
 *	Give space back to system.
 *
 **************************************/

	gds__free(memory);
}


PYXIS_alloc_init()
{
/**************************************
 *
 *	P Y X I S _ a l l o c _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize the pool system.
 *
 **************************************/
	SLONG temp_vector[20];
	PLB pool;

	pools = (VEC) temp_vector;
	pools->vec_count = 1;
	pools->vec_object[0] = NULL;

	PYXIS_default_pool = PYXIS_permanent_pool = pool = PYXIS_pool();
	pools = (VEC) PYXIS_alloc(pool, type_vec, 10);
	pools->vec_count = 10;
	pools->vec_object[0] = (BLK) pool;
}


UCHAR *PYXIS_malloc(size)
	 SLONG size;
{
/**************************************
 *
 *	P Y X I S _ m a l l o c
 *
 **************************************
 *
 * Functional description
 *	Get memory from system.
 *
 **************************************/
	register UCHAR *memory;

	if (memory = (UCHAR *) gds__alloc(size))
		return memory;

	IBERROR("memory gonzo");
}


PLB PYXIS_pool()
{
/**************************************
 *
 *	P Y X I S _ p o o l
 *
 **************************************
 *
 * Functional description
 *	Allocate a new pool.  This is done by creating a tempory
 *	pool block on the stack, then allocating a real pool block.
 *	In short, by mirrors.
 *
 **************************************/
	struct plb temp_pool;
	register PLB pool;
	register int pool_id;

/* Start by assigning a pool id */

	for (pool_id = 0; pool_id < pools->vec_count; pool_id++)
		if (!(pools->vec_object[pool_id]))
			break;

	if (pool_id >= pools->vec_count)
		PYXIS_extend(&pools, pool_id + 10);

	pools->vec_object[pool_id] = (BLK) & temp_pool;
	temp_pool.plb_free = NULL;
	temp_pool.plb_hunks = NULL;
	temp_pool.plb_pool_id = pool_id;
	if (pool_id == 0)
		PYXIS_permanent_pool = &temp_pool;

	pool = (PLB) PYXIS_alloc(&temp_pool, type_plb);
	pool->plb_pool_id = pool_id;
	pool->plb_free = temp_pool.plb_free;
	pool->plb_hunks = temp_pool.plb_hunks;
	pools->vec_object[pool_id] = (BLK) pool;

	if (pool_id == 0)
		PYXIS_permanent_pool = pool;

	return pool;
}


PYXIS_push(object, stack)
	 BLK object;
	 register LLS *stack;
{
/**************************************
 *
 *	P Y X I S _ p u s h
 *
 **************************************
 *
 * Functional description
 *	Push an object on an LLS stack.
 *
 **************************************/
	register LLS node;
	PLB pool;

	pool = PYXIS_default_pool;

	if (node = pool->plb_lls)
		pool->plb_lls = node->lls_next;
	else
		node = (LLS) PYXIS_alloc(pool, type_lls);

	node->lls_object = object;
	node->lls_next = *stack;
	*stack = node;
}


BLK PYXIS_pop(stack)
	 register LLS *stack;
{
/**************************************
 *
 *	P Y X I S _ p o p
 *
 **************************************
 *
 * Functional description
 *	Pop an object off a linked list stack.  Save the node for
 *	further use.
 *
 **************************************/
	register LLS node;
	register PLB pool;

	node = *stack;
	pool = (PLB) pools->vec_object[node->lls_header.blk_pool_id];
	*stack = node->lls_next;
	node->lls_next = pool->plb_lls;
	pool->plb_lls = node;

	return node->lls_object;
}


PYXIS_release(block)
	 register FRB block;
{
/**************************************
 *
 *	P Y X I S _ r e l e a s e
 *
 **************************************
 *
 * Functional description
 *	Release a block to its pool.  If it is contiguous to
 *	another free block, combine them.  Otherwise link it
 *	into the free block linked list (kept in ascending order
 *	of addresses).
 *
 **************************************/
	register FRB prior, free;
	FRB *ptr;
	PLB pool;
	int pool_id;

	block->frb_header.blk_type = (SCHAR) type_frb;
	pool_id = block->frb_header.blk_pool_id;

	if (pool_id >= pools->vec_count ||
		!(pool = (PLB) pools->vec_object[pool_id])) BUGCHECK("bad pool id");

	prior = NULL;
	for (ptr = &pool->plb_free; free = *ptr;
		 prior = free, ptr =
		 &free->frb_next) if ((SCHAR HUGE_PTR *) block <=
							  (SCHAR HUGE_PTR *) free) break;

	if ((SCHAR HUGE_PTR *) block == (SCHAR HUGE_PTR *) free)
		BUGCHECK("block released twice");

/* Merge block into list first, then try to combine blocks */

	block->frb_next = free;
	*ptr = block;

/* Try to merge the free block with the next one down. */

	if (free) {
		if ((SCHAR HUGE_PTR *) block + block->frb_header.blk_length ==
			(SCHAR HUGE_PTR *) free) {
			block->frb_header.blk_length += free->frb_header.blk_length;
			block->frb_next = free->frb_next;
		}
		else if ((SCHAR HUGE_PTR *) block + block->frb_header.blk_length >
				 (SCHAR HUGE_PTR *) free)
			BUGCHECK("released block overlaps following free block");
	}

/* Try to merge the block with the prior free block */

	if (prior) {
		if ((SCHAR HUGE_PTR *) prior + prior->frb_header.blk_length ==
			(SCHAR HUGE_PTR *) block) {
			prior->frb_header.blk_length += block->frb_header.blk_length;
			prior->frb_next = block->frb_next;
		}
		else if ((SCHAR HUGE_PTR *) prior + prior->frb_header.blk_length ==
				 (SCHAR HUGE_PTR *) block)
			BUGCHECK("released block overlaps prior free block");
	}

}


PYXIS_rlpool(pool)
	 PLB pool;
{
/**************************************
 *
 *	P Y X I S _ r l p o o l
 *
 **************************************
 *
 * Functional description
 *	Release a storage pool.  This involves nothing more than returning
 *	hunks to the free hunk list.
 *
 **************************************/
	register HNK hunk, hunks;

	pools->vec_object[pool->plb_pool_id] = NULL;

	for (hunks = pool->plb_hunks; hunk = hunks;) {
		hunks = hunk->hnk_next;
		gds__free(hunk->hnk_address);
	}
}


static extend_pool(pool, size)
	 PLB pool;
	 int size;
{
/**************************************
 *
 *	e x t e n d _ p o o l
 *
 **************************************
 *
 * Functional description
 *	Extend a pool by at least enough to accomodate a block
 *	of given size.
 *
 **************************************/
	register HNK hunk, *ptr;
	register BLK block;

	size =
		(size + sizeof(struct hnk) + MIN_ALLOCATION - 1) & ~(MIN_ALLOCATION -
															 1);
	block = (BLK) PYXIS_malloc((SLONG) size);
	block->blk_length = size;
	block->blk_type = (SCHAR) type_frb;
	block->blk_pool_id = pool->plb_pool_id;
	PYXIS_release(block);

	hunk = (HNK) PYXIS_alloc(pool, type_hnk);
	hunk->hnk_address = (SCHAR *) block;
	hunk->hnk_length = size;
	hunk->hnk_next = pool->plb_hunks;
	pool->plb_hunks = hunk;
}
