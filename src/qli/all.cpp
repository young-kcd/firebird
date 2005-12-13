/*
 *	PROGRAM:	JRD Command Oriented Query Language
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
$Id: all.cpp,v 1.14 2003-04-16 10:18:15 aafemt Exp $
*/

/***************************************************
   THIS MODULE HAS SEVERAL KISSING COUSINS; IF YOU
   SHOULD CHANGE ONE OF THE MODULES IN THE FOLLOWING
   LIST, PLEASE BE SURE TO CHECK THE OTHERS FOR
   SIMILAR CHANGES:

                  /dsql/all.c
                  /jrd/all.c
                  /pipe/allp.c
                  /qli/all.c
                  /remote/allr.c
                  /gpre/msc.c

   - THANK YOU
***************************************************/

#include "firebird.h"
#include <string.h>
#include "../qli/everything.h"
#include "../qli/all_proto.h"
#include "../qli/err_proto.h"
#include "../qli/mov_proto.h"
#include "../jrd/gds_proto.h"


#define BLKDEF(type, root, tail) { sizeof (struct root), tail },

static struct {
	SSHORT typ_root_length;
	SSHORT typ_tail_length;
} block_sizes[] = {
	{0, 0},
#include "../qli/blk.h"
0};

#undef BLKDEF

static void extend_pool(PLB, USHORT);

static VEC pools;

#define MIN_ALLOCATION	1024


BLK ALLQ_alloc( PLB pool, UCHAR type, int count)
{
/**************************************
 *
 *	A L L Q _ a l l o c
 *
 **************************************
 *
 * Functional description
 *	Allocate a block from a given pool and initialize the block.
 *	This is the primary block allocation routine.
 *
 **************************************/
	FRB block;
	FRB free, *best, *ptr;
	USHORT l;
	USHORT size;
	SLONG best_tail, tail;

	if (type <= (SCHAR) type_MIN || type >= (SCHAR) type_MAX)
		BUGCHECK(1);			/* Msg1 bad block type */

/* Compute block length */

	size = block_sizes[type].typ_root_length;

	if (tail = block_sizes[type].typ_tail_length)
		size += count * tail;

#ifndef VMS
	size = FB_ALIGN(size, ALIGNMENT);
#else
	size = ((size + ALIGNMENT - 1) & ~(ALIGNMENT - 1));
#endif

	if (size <= 4 || size > 65535)
		BUGCHECK(2);			/* Msg2 bad block size */

/* Find best fit.  Best fit is defined to be the free block of SHORTest
   tail.  If there isn't a fit, extend the pool and try, try again. */

	while (TRUE) {
		best = NULL;
		best_tail = 32767;
		for (ptr = &pool->plb_free; (free = *ptr); ptr = &free->frb_next)
			if (free->frb_next
				&& (SCHAR *) free >=
				(SCHAR *) free->frb_next) BUGCHECK(434);	/* memory pool free list is incorrect */
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
   after taking out our block, chop out out block.  If not, allocate
   the entire free block as our block (a little extra won't hurt). */

	free = *best;
	if (best_tail > sizeof(struct frb)) {
		l = free->frb_header.blk_length - size;
		block = (FRB) ((SCHAR *) free + l);
		free->frb_header.blk_length -= size;
	}
	else {
		*best = free->frb_next;
		size += best_tail;
		block = free;
	}

	block->frb_header.blk_type = type;
	block->frb_header.blk_pool_id = pool->plb_pool_id;
	block->frb_header.blk_length = size;

	if (size -= sizeof(struct blk))
		memset((SCHAR *) block + sizeof(struct blk), 0, size);

	return (BLK) block;
}


BLK ALLQ_extend(BLK * pointer, int size)
{
/**************************************
 *
 *	A L L Q _ e x t e n d
 *
 **************************************
 *
 * Functional description
 *	Extend a repeating block, copying the constant part.
 *
 **************************************/
	BLK block, new_blk;
	int length;

	block = *pointer;
	new_blk = (BLK) ALLQ_alloc((PLB) pools->vec_object[block->blk_pool_id],
						   block->blk_type, size);
	length = MIN(block->blk_length, new_blk->blk_length) - sizeof(struct blk);
	MOVQ_fast((SCHAR *) block + sizeof(struct blk),
			  (SCHAR *) new_blk + sizeof(struct blk), length);
	ALLQ_release((FRB) block);

	if (new_blk->blk_type == (SCHAR) type_vec)
		((VEC) new_blk)->vec_count = size;
	else if (new_blk->blk_type == (SCHAR) type_vcl)
		((VCL) new_blk)->vcl_count = size;

	*pointer = new_blk;

	return new_blk;
}


void ALLQ_fini(void)
{
/**************************************
 *
 *	A L L Q _ f i n i
 *
 **************************************
 *
 * Functional description
 *	Get rid of everything.
 *
 **************************************/
	PLB pool, *vector, *until;
	HNK hunks, hunk;

	for (vector = (PLB *) pools->vec_object + pools->vec_count,
		 until = (PLB *) pools->vec_object; --vector >= until;)
		if (pool = *vector)
			for (hunks = pool->plb_hunks; hunk = hunks;) {
				hunks = hunk->hnk_next;
				ALLQ_free(hunk->hnk_address);
			}
}


void ALLQ_free( SCHAR * memory)
{
/**************************************
 *
 *	A L L Q _ f r e e
 *
 **************************************
 *
 * Functional description
 *	Give space back to system.
 *
 **************************************/

	gds__free(memory);
}


void ALLQ_init(void)
{
/**************************************
 *
 *	A L L Q _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize the pool system.
 *
 **************************************/
	ISC_STATUS_ARRAY temp_vector;
	PLB pool;

	pools = (VEC) temp_vector;
	pools->vec_count = 1;
	pools->vec_object[0] = NULL;

	QLI_default_pool = QLI_permanent_pool = pool = ALLQ_pool();
	pools = (VEC) ALLQ_alloc(pool, type_vec, 10);
	pools->vec_count = 10;
	pools->vec_object[0] = (BLK) pool;
}


SCHAR *ALLQ_malloc(SLONG size)
{
/**************************************
 *
 *	A L L Q _ m a l l o c
 *
 **************************************
 *
 * Functional description
 *	Get memory from system.
 *
 **************************************/
	SCHAR *memory;

	if (memory = (SCHAR *) gds__alloc(size)) {
#ifdef DEBUG_GDS_ALLOC
		gds_alloc_flag_unfreed((void *) memory);	/* Don't care about QLI leaks */
#endif
		return memory;
	}

	IBERROR(5);					/* Msg5 "memory gonzo" */
	return 0;
}


PLB ALLQ_pool(void)
{
/**************************************
 *
 *	A L L Q _ p o o l
 *
 **************************************
 *
 * Functional description
 *	Allocate a new pool.  This is done by creating a tempory
 *	pool block on the stack, then allocating a real pool block.
 *	In SHORT, by mirrors.
 *
 **************************************/
	struct plb temp_pool;
	PLB pool;
	int pool_id;

/* Start by assigning a pool id */

	for (pool_id = 0; pool_id < pools->vec_count; pool_id++)
		if (!(pools->vec_object[pool_id]))
			break;

	if (pool_id >= pools->vec_count)
		ALLQ_extend((BLK*) &pools, pool_id + 10);

	pools->vec_object[pool_id] = (BLK) & temp_pool;
	temp_pool.plb_free = NULL;
	temp_pool.plb_hunks = NULL;
	temp_pool.plb_pool_id = pool_id;
	if (pool_id == 0)
		QLI_permanent_pool = &temp_pool;

	pool = (PLB) ALLQ_alloc(&temp_pool, type_plb, 0);
	pool->plb_pool_id = pool_id;
	pool->plb_free = temp_pool.plb_free;
	pool->plb_hunks = temp_pool.plb_hunks;
	pools->vec_object[pool_id] = (BLK) pool;

	if (pool_id == 0)
		QLI_permanent_pool = pool;

	return pool;
}


void ALLQ_push( BLK object, LLS * stack)
{
/**************************************
 *
 *	A L L Q _ p u s h
 *
 **************************************
 *
 * Functional description
 *	Push an object on an LLS stack.
 *
 **************************************/
	LLS node;
	PLB pool;

	pool = QLI_default_pool;

	if (node = pool->plb_lls)
		pool->plb_lls = node->lls_next;
	else
		node = (LLS) ALLQ_alloc(pool, type_lls, 0);

	node->lls_object = object;
	node->lls_next = *stack;
	*stack = node;
}


BLK ALLQ_pop(LLS * stack)
{
/**************************************
 *
 *	A L L Q _ p o p
 *
 **************************************
 *
 * Functional description
 *	Pop an object off a linked list stack.  Save the node for
 *	further use.
 *
 **************************************/
	LLS node;
	PLB pool;

	node = *stack;
	pool = (PLB) pools->vec_object[node->lls_header.blk_pool_id];
	*stack = node->lls_next;
	node->lls_next = pool->plb_lls;
	pool->plb_lls = node;

	return node->lls_object;
}


void ALLQ_release( FRB block)
{
/**************************************
 *
 *	A L L Q _ r e l e a s e
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
	FRB prior, free;
	FRB *ptr;
	PLB pool;
	int pool_id;

	block->frb_header.blk_type = (SCHAR) type_frb;
	pool_id = block->frb_header.blk_pool_id;

	if (pool_id >= pools->vec_count ||
		!(pool = (PLB) pools->vec_object[pool_id])) BUGCHECK(4);	/* Msg4 bad pool id */

	prior = NULL;
	for (ptr = &pool->plb_free; free = *ptr;
		 prior = free, ptr =
		 &free->frb_next) if ((SCHAR *) block <=
							  (SCHAR *) free) break;

	if ((SCHAR *) block == (SCHAR *) free)
		BUGCHECK(435);			/* block released twice */

/* Merge block into list first, then try to combine blocks */

	block->frb_next = free;
	*ptr = block;

/* Try to merge the free block with the next one down. */

	if (free) {
		if ((SCHAR *) block + block->frb_header.blk_length ==
			(SCHAR *) free) {
			block->frb_header.blk_length += free->frb_header.blk_length;
			block->frb_next = free->frb_next;
		}
		else if ((SCHAR *) block + block->frb_header.blk_length >
				 (SCHAR *) free)
			BUGCHECK(436);		/* released block overlaps following free block */
	}

/* Try and merge the block with the prior free block */

	if (prior) {
		if ((SCHAR *) prior + prior->frb_header.blk_length ==
			(SCHAR *) block) {
			prior->frb_header.blk_length += block->frb_header.blk_length;
			prior->frb_next = block->frb_next;
		}
		else if ((SCHAR *) prior + prior->frb_header.blk_length >
				 (SCHAR *) block)
			BUGCHECK(437);		/* released block overlaps prior free block */
	}
}


void ALLQ_rlpool( PLB pool)
{
/**************************************
 *
 *	A L L Q _ r l p o o l
 *
 **************************************
 *
 * Functional description
 *	Release a storage pool.  This involves nothing more than returning
 *	hunks to the free hunk list.
 *
 **************************************/
	HNK hunk, hunks;

	pools->vec_object[pool->plb_pool_id] = NULL;

	for (hunks = pool->plb_hunks; hunk = hunks;) {
		hunks = hunk->hnk_next;
		gds__free(hunk->hnk_address);
	}
}


static void extend_pool( PLB pool, USHORT count)
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
	HNK hunk;
	BLK block;
	SLONG size;

	size =
		(count + sizeof(struct hnk) + MIN_ALLOCATION - 1) & ~(MIN_ALLOCATION -
															  1);

	if ((USHORT) size < count)
		IBERROR(481);			/* msg 481 unsuccessful attempt to extend pool beyond 64KB */

	block = (BLK) ALLQ_malloc(size);
	block->blk_length = size;
	block->blk_type = (SCHAR) type_frb;
	block->blk_pool_id = pool->plb_pool_id;
	ALLQ_release((FRB) block);

	hunk = (HNK) ALLQ_alloc(pool, type_hnk, 0);
	hunk->hnk_address = (SCHAR *) block;
	hunk->hnk_length = size;
	hunk->hnk_next = pool->plb_hunks;
	pool->plb_hunks = hunk;
}
