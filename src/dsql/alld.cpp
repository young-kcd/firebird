/*
 *	PROGRAM:	Dynamic SQL runtime support
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


/***************************************************
   THIS MODULE HAS SEVERAL KISSING COUSINS; IF YOU
   SHOULD CHANGE ONE OF THE MODULES IN THE FOLLOWING
   LIST, PLEASE BE SURE TO CHECK THE OTHERS FOR
   SIMILAR CHANGES:

        /gds/maint/pyxis/all.c
                  /dsql/all.c
                  /jrd/all.c
                  /pipe/allp.c
                  /qli/all.c
                  /remote/allr.c
                  /gpre/msc.c

   - THANK YOU
***************************************************/


/**************************************************************
V4 Multi-threading changes.

-- direct calls to gds__ () & isc_ () entry points

	THREAD_EXIT;
	    gds__ () or isc_ () call.
	THREAD_ENTER;

-- calls through embedded GDML.

the following protocol will be used.  Care should be taken if
nested FOR loops are added.

    THREAD_EXIT;                // last statment before FOR loop 

    FOR ...............

	THREAD_ENTER;           // First statment in FOR loop
	.....some C code....
	.....some C code....
	THREAD_EXIT;            // last statment in FOR loop 

    END_FOR;

    THREAD_ENTER;               // First statment after FOR loop
***************************************************************/

#include <string.h>
#include "../jrd/ib_stdio.h"
#include "../dsql/dsql.h"
#include "../dsql/node.h"
#include "../dsql/sym.h"
#include "../jrd/codes.h"
#include "../dsql/alld_proto.h"
#include "../dsql/errd_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/thd_proto.h"
#include "../jrd/gdsassert.h"

ASSERT_FILENAME					/* Define things dsql/assert needed */
extern "C" {
#define MIN_ALLOCATION	1024
#if (defined PC_PLATFORM && !defined NETWARE_386)
#define MAX_BLOCK	65520
#else
#define MAX_BLOCK	(262144 - MIN_ALLOCATION - sizeof (struct hnk) - 8)
#endif
#define SHIFT			SHIFTLONG
#define BLOCK_FACTOR		(1 << SHIFT)
#define	BLOCKS_TO_SIZE(blocks)	((blocks) << SHIFT)
#define	SIZE_TO_BLOCKS(size)	((size) >> SHIFT)
#define	BLOCK_ROUNDUP(size)	ROUNDUP (size, MAX(BLOCK_FACTOR,ALIGNMENT))
#define EXPAND_BLOCKSIZE(b)	(BLOCKS_TO_SIZE ((b)->blk_length))

#define BLKDEF(type, root, tail)	sizeof (struct root), tail,
	static CONST struct {
	USHORT typ_root_length;
	USHORT typ_tail_length;
} block_sizes[] = {
	0, 0,
#include "../dsql/blk.h"
0};


#undef BLKDEF

#ifdef SUPERSERVER
extern SLONG trace_pools;
SLONG alld_delta_alloc = 0;

#define BLKDEF(type, root, tail)  0,

SLONG alld_block_type_count[] = {
#include "../dsql/blk.h"
	0
};
#undef BLKDEF
#define BLKDEF(type, root, tail)  #type,
char ALLD_types[][24] = {
	"type_MIN",
#include "../dsql/blk.h"
	"type_MAX"
};
#undef BLKDEF
int alld_type_MAX = type_MAX;
#endif

static void extend_pool(PLB, ULONG);
static PLB find_pool(BLK);
static void release(FRB, PLB);

static USHORT init_flag = FALSE;
static VEC pools = NULL;


BLK ALLD_alloc( PLB pool, UCHAR type, ULONG count)
{
/**************************************
 *
 *	A L L D _ a l l o c
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
	register ULONG size;
	ULONG l;
	SLONG best_tail, tail;
	ULONG needed_blocks;

	assert(pool != NULL);
	DEV_BLKCHK(pool, type_plb);

	if (type <= (SCHAR) type_MIN || type >= (SCHAR) type_MAX)
		BUGCHECK("bad block type");

/* Compute block length */

	size = block_sizes[type].typ_root_length;

	if ((tail = block_sizes[type].typ_tail_length) && count >= 1)
		size += (count - 1) * tail;

#ifdef DEV_BUILD
	if (size <= sizeof(struct blk) || size >= MAX_BLOCK)
		BUGCHECK("bad block size");
#endif

	needed_blocks = SIZE_TO_BLOCKS(BLOCK_ROUNDUP(size));

	assert(BLOCKS_TO_SIZE(needed_blocks) >= size);

/* Find best fit.  Best fit is defined to be the free block of shortest
   tail.  If there isn't a fit, extend the pool and try, try again. */

	while (TRUE) {
		best = NULL;
		best_tail = MAX_BLOCK;
		for (ptr = &pool->plb_free; ((free = *ptr) != NULL);
			 ptr =
			 &free->frb_next) if ((SCHAR HUGE_PTR *) free ==
								  (SCHAR HUGE_PTR *) free->frb_next) {
				BUGCHECK("corrupt pool");
			}
		/* Is this block big enough?  
		 * And have less leftover than the best one found? */
			else
				if (
					((tail
					  =
					  ((ULONG) free->frb_header.blk_length -
					   needed_blocks)) >= 0) && (tail < best_tail)) {
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
	if (best_tail > SIZE_TO_BLOCKS(BLOCK_ROUNDUP(sizeof(struct frb)))) {
		/* Carve off the needed size from the bottom of the free block */
		l = free->frb_header.blk_length - needed_blocks;

		block = (BLK) ((UCHAR *) free + BLOCKS_TO_SIZE(l));

		/* Reset the length of the free block */
		assert(l <= MAX_USHORT);
		free->frb_header.blk_length = (USHORT) l;
	}
	else {
		/* There isn't left over in the free block to save in the free list */
		/* So give the client the whole free block */

		/* Unhook the free block from the free chain */
		*best = free->frb_next;

		/* Client gets the whole block */
		needed_blocks = free->frb_header.blk_length;
		block = (BLK) free;
	}

/* Zero the whole allocated structure */
	memset(block, 0, BLOCKS_TO_SIZE(needed_blocks));

/* Now set the block header (yeah, we just zero'ed it, sue me) */
	block->blk_type = type;
	assert(pool->plb_pool_id <= MAX_UCHAR);
	block->blk_pool_id = (UCHAR) pool->plb_pool_id;
	assert(needed_blocks <= MAX_USHORT);
	block->blk_length = (USHORT) needed_blocks;

#ifdef SUPERSERVER
	if (trace_pools) {
		++alld_block_type_count[type];
		++pool->plb_blk_type_count[type];
	}
#endif

	return block;
}


BLK ALLD_extend(BLK * pointer, ULONG size)
{
/**************************************
 *
 *	A L L D _ e x t e n d
 *
 **************************************
 *
 * Functional description
 *	Extend a repeating block, copying the constant part.
 *
 **************************************/
	BLK block, new_;
	PLB pool;
	register ULONG length, l;
	register SLONG *p1, *p2;
	register SCHAR *c1, *c2;

	assert(pointer != NULL);
	assert(*pointer != NULL);

	block = *pointer;
	pool = find_pool(block);
	new_ = ALLD_alloc(pool, block->blk_type, size);
	length = MIN(EXPAND_BLOCKSIZE(block), EXPAND_BLOCKSIZE(new_))
		- sizeof(struct blk);
	p1 = (SLONG *) ((UCHAR *) new_ + sizeof(struct blk));
	p2 = (SLONG *) ((UCHAR *) block + sizeof(struct blk));

/* Copy the bytes a longword at a time */
	if ((l = length >> SHIFTLONG) != 0)
		do
			*p1++ = *p2++;
		while (--l);

/* Copy any remaining bytes */
	if (length &= 3) {
		c1 = (SCHAR *) p1;
		c2 = (SCHAR *) p2;
		do
			*c1++ = *c2++;
		while (--length);
	}

	release(reinterpret_cast<FRB>(block), pool);

	if (new_->blk_type == (SCHAR) type_vec)
		((VEC) new_)->vec_count = size;
	else if (new_->blk_type == (SCHAR) type_vcl)
		((VCL) new_)->vcl_count = size;

	*pointer = new_;

	return new_;
}


void ALLD_fini(void)
{
/**************************************
 *
 *	A L L D _ f i n i
 *
 **************************************
 *
 * Functional description
 *	Get rid of everything.
 *
 **************************************/
	BLK *vector, *until;
	PLB pool;

	assert(init_flag);			/* Must _init before _fini */

/* if there are no pools, we've already finished. */

	if (!pools)
		return;

/* vec_object [0] is the memory pool which contains pools, so release
 * objects in reverse order, and be careful not to refer to pools
 * during the process.
 */
	for (vector = pools->vec_object + pools->vec_count, until =
		 pools->vec_object; --vector >= until;)
		if ((pool = (PLB) * vector) != NULL)
			ALLD_rlpool(pool);

	pools = NULL;
	init_flag = FALSE;
}


void ALLD_free( SCHAR * memory)
{
/**************************************
 *
 *	A L L D _ f r e e
 *
 **************************************
 *
 * Functional description
 *	Give space back to system.
 *
 **************************************/

#ifdef SUPERSERVER
	alld_delta_alloc -= gds__free(memory);
#else
	gds__free(memory);
#endif
}


USHORT ALLD_init(void)
{
/**************************************
 *
 *	A L L D _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize the pool system.  Return
 *	TRUE if initialization took place, FALSE if
 * 	we had been previous initialized.
 *
 **************************************/
	SLONG temp_vector[20];
	PLB pool;
	USHORT init;
	TSQL tdsql;

	tdsql = GET_THREAD_DATA;

	init = (init_flag == FALSE);
	if (!init_flag) {
		init_flag = TRUE;

		pools = (VEC) temp_vector;
		pools->vec_count = 1;
		pools->vec_object[0] = NULL;

		tdsql->tsql_default = DSQL_permanent_pool = pool = ALLD_pool();
		pools = (VEC) ALLD_alloc(pool, type_vec, 10);
		pools->vec_count = 10;

		/* Note: ALLD_fini() assumes the this master pool is in vec_object [0] */
		pools->vec_object[0] = (BLK) pool;
	}

	return init;
}


UCHAR *ALLD_malloc(ULONG size)
{
/**************************************
 *
 *	A L L D _ m a l l o c
 *
 **************************************
 *
 * Functional description
 *	Get memory from system.
 *
 **************************************/
	register UCHAR *memory;
	TSQL tdsql;

	tdsql = GET_THREAD_DATA;

	if ((memory = gds__alloc((SLONG) size)) != NULL)
#ifdef SUPERSERVER
	{
		alld_delta_alloc += size;
		return memory;
	}
#else
		return memory;
#endif
/* FREE: by ALLD_free, called during DSQL cleanup */
/* NOMEM: post a user level error - if we can */

	if (tdsql && tdsql->tsql_setjmp)
		ERRD_post(gds__sys_request, gds_arg_string, "gds__alloc", gds_arg_gds,
				  gds__virmemexh, gds_arg_end);

/* Commentary:  This expands out to a call to ERRD_error - which
 * promply depends on tdsql being non-NULL.  Knock, knock, anyone home?
 */
	IBERROR(-1, "out of memory");
	return ((UCHAR *) NULL);	/* Added to remove warnings */
}


PLB ALLD_pool(void)
{
/**************************************
 *
 *	A L L D _ p o o l
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
	register USHORT pool_id;

/* Start by assigning a pool id */

	for (pool_id = 0; pool_id < pools->vec_count; pool_id++)
		if (!(pools->vec_object[pool_id]))
			break;

	if (pool_id >= pools->vec_count)
		ALLD_extend((BLK *) & pools, (ULONG) (pool_id + 10));

	memset((UCHAR *) & temp_pool, 0, sizeof(temp_pool));
	temp_pool.plb_header.blk_type = type_plb;
	pools->vec_object[pool_id] = (BLK) & temp_pool;
	temp_pool.plb_free = NULL;
	temp_pool.plb_hunks = NULL;
	temp_pool.plb_pool_id = pool_id;
	temp_pool.plb_blk_type_count = NULL;
#ifdef SUPERSERVER
	if (trace_pools)
	{
		temp_pool.plb_blk_type_count =
			reinterpret_cast <long*>(gds__alloc(sizeof(alld_block_type_count)));
		if (!temp_pool.plb_blk_type_count) {
			trace_pools = 0;	/* No memory!! stop tracing pool info */
		} else {
			memset(temp_pool.plb_blk_type_count, 0, sizeof(alld_block_type_count));
		}
	}
#endif

	if (pool_id == 0)
		DSQL_permanent_pool = &temp_pool;

	pool = (PLB) ALLD_alloc(&temp_pool, type_plb, 0);
	pool->plb_pool_id = pool_id;
	pool->plb_free = temp_pool.plb_free;
	pool->plb_hunks = temp_pool.plb_hunks;
#ifdef SUPERSERVER
	pool->plb_blk_type_count = temp_pool.plb_blk_type_count;
#endif
	pools->vec_object[pool_id] = (BLK) pool;

	if (pool_id == 0)
		DSQL_permanent_pool = pool;

	return pool;
}


#ifdef SUPERSERVER
void ALLD_print_memory_pool_info( IB_FILE * fptr)
{
/*************************************************************
 *
 *	A L L D _ p r i n t _ m e m o r y _ p o o l _ i n f o
 *
 *************************************************************
 *
 * Functional description
 *	Print the various block types allocated with in the pools
 *
 *************************************************************/
	VEC vector;
	PLB myPool;
	HNK hnk;
	int i, j, col;

	if (!trace_pools)
		return;

	ib_fprintf(fptr, "\n\tALLD_xx block types\n");
	ib_fprintf(fptr, "\t--------------------");
	for (i = 0, col = 0; i < type_MAX; i++)
		if (alld_block_type_count[i]) {
			if (col % 5 == 0)
				ib_fprintf(fptr, "\n\t");
			ib_fprintf(fptr, "%s = %d  ", ALLD_types[i],
					   alld_block_type_count[i]);
			++col;
		}
	ib_fprintf(fptr, "\n");

	if (!pools) {
		ib_fprintf(fptr, "\t    No pools allocated");
		return;
	}
	vector = pools;
	for (j = 0, i = 0; i < (int) vector->vec_count; i++) {
		myPool = (PLB) vector->vec_object[i];
		if (myPool)
			++j;
	}
	ib_fprintf(fptr, "\t    There are %d pools", j);
	for (i = 0; i < (int) vector->vec_count; i++) {
		myPool = (PLB) vector->vec_object[i];
		if (!myPool)
			continue;
		ib_fprintf(fptr, "\n\t    Pool %d", myPool->plb_pool_id);
		for (j = 0, hnk = myPool->plb_hunks; hnk; hnk = hnk->hnk_next)
			j++;
		if (j)
			ib_fprintf(fptr, " has %d hunks", j);
		for (j = 0, col = 0; j < type_MAX; j++)
			if (myPool->plb_blk_type_count[j]) {
				if (col % 5 == 0)
					ib_fprintf(fptr, "\n\t    ");
				ib_fprintf(fptr, "%s = %d  ", ALLD_types[j],
						   myPool->plb_blk_type_count[j]);
				++col;
			}
	}
}
#endif


void ALLD_push( BLK object, register LLS * stack)
{
/**************************************
 *
 *	A L L D _ p u s h
 *
 **************************************
 *
 * Functional description
 *	Push an object on an LLS stack.
 *
 **************************************/
	register LLS node;
	PLB pool;
	TSQL tdsql;

	tdsql = GET_THREAD_DATA;

	pool = tdsql->tsql_default;

	if ((node = pool->plb_lls) != NULL) {
		/* Pull a stack block off the list of free stack blocks */
		pool->plb_lls = node->lls_next;
	}
	else {
		/* No free stack blocks - allocate a new one */
		node = (LLS) ALLD_alloc(pool, type_lls, 0);
	}

	DEV_BLKCHK(node, type_lls);
	DEV_BLKCHK(*stack, type_lls);

	node->lls_object = object;
	node->lls_next = *stack;
	*stack = node;
}


BLK ALLD_pop(register LLS * stack)
{
/**************************************
 *
 *	A L L D _ p o p
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
	BLK block;

	DEV_BLKCHK(*stack, type_lls);

	node = *stack;

	DEV_BLKCHK(node, type_lls);



/* what we are doing below is .... block = &(node->lls_header) */
	block = (BLK) node;
	pool = find_pool(block);

/* Pop item off the stack */
	*stack = node->lls_next;

/* Add the popped stack lls block to the list of free stack blocks */
	node->lls_next = pool->plb_lls;
	pool->plb_lls = node;

	return node->lls_object;
}


void ALLD_release( register FRB block)
{
/**************************************
 *
 *	A L L D _ r e l e a s e
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

	release(block, find_pool(&block->frb_header));
}


void ALLD_rlpool( PLB pool)
{
/**************************************
 *
 *	A L L D _ r l p o o l
 *
 **************************************
 *
 * Functional description
 *	Release a storage pool.  This involves nothing more than returning
 *	hunks to the free hunk list.
 *
 **************************************/
	register HNK hunk, hunks;

/* if there are no pools, there's no point in releasing anything... */

	if (!pools)
		return;

	DEV_BLKCHK(pool, type_plb);

	pools->vec_object[pool->plb_pool_id] = NULL;

/* Have to release hunks carefully, as one of the hunks being released 
 * contains pool itself
 */
#ifdef SUPERSERVER
	if (trace_pools && pool->plb_blk_type_count) {
		int i;
		for (i = 0; i < type_MAX; i++)
			alld_block_type_count[i] -= pool->plb_blk_type_count[i];
		gds__free(pool->plb_blk_type_count);
	}
#endif

	for (hunks = pool->plb_hunks; hunk = hunks;) {
		hunks = hunk->hnk_next;
		ALLD_free(reinterpret_cast < char *>(hunk->hnk_address));
	}
}


static void extend_pool( PLB pool, ULONG size)
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
	register HNK hunk;
	register BLK block;

	DEV_BLKCHK(pool, type_plb);

	size =
		(size + sizeof(struct hnk) + MIN_ALLOCATION -
		 1) & ~((ULONG) MIN_ALLOCATION - 1);
	block = (BLK) ALLD_malloc(size);
	assert(SIZE_TO_BLOCKS(size) <= MAX_USHORT);
	block->blk_length = (USHORT) SIZE_TO_BLOCKS(size);
	block->blk_type = (SCHAR) type_frb;
	assert(pool->plb_pool_id <= MAX_UCHAR);
	block->blk_pool_id = (UCHAR) pool->plb_pool_id;
#ifdef SUPERSERVER
	if (trace_pools) {
		++alld_block_type_count[block->blk_type];
		++pool->plb_blk_type_count[block->blk_type];
	}
#endif
	release(reinterpret_cast<FRB>(block), pool);

	hunk = (HNK) ALLD_alloc(pool, type_hnk, 0);
	hunk->hnk_address = (UCHAR *) block;
	hunk->hnk_length = size;
	hunk->hnk_next = pool->plb_hunks;
	pool->plb_hunks = hunk;
}


static PLB find_pool( BLK block)
{
/**************************************
 *
 *	f i n d _ p o o l
 *
 **************************************
 *
 * Functional description
 *	Find pool associate with block.
 *
 **************************************/
	PLB pool;
	HNK hunk;
	USHORT pool_id;

	if (pools->vec_count < 256)
		if ((pool_id = block->blk_pool_id) < pools->vec_count &&
			(pool = (PLB) pools->vec_object[pool_id]))
			return pool;
		else
			BUGCHECK("bad pool id");

	for (pool_id = block->blk_pool_id; pool_id < pools->vec_count;
		 pool_id += 256) if (pool = (PLB) pools->vec_object[pool_id]) {
			hunk = pool->plb_hunks;
			for (; hunk; hunk = hunk->hnk_next)
				if ((SCHAR HUGE_PTR *) block >=
					(SCHAR HUGE_PTR *) hunk->hnk_address
					&& (SCHAR HUGE_PTR *) block <
					(SCHAR HUGE_PTR *) hunk->hnk_address + hunk->hnk_length)
					return pool;
		}
	BUGCHECK("bad pool id");
	return (PLB) NULL;			/* Added to remove warnings */
}


static void release( FRB block, PLB pool)
{
/**************************************
 *
 *	r e l e a s e
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
#ifdef SUPERSERVER
	UCHAR blk_header_type;
#endif

	DEV_BLKCHK(pool, type_plb);

#ifdef SUPERSERVER
	blk_header_type = block->frb_header.blk_type;
#endif
	block->frb_header.blk_type = (SCHAR) type_frb;
	prior = NULL;

	for (ptr = &pool->plb_free; (free = *ptr) != NULL;
		 prior = free, ptr =
		 &free->frb_next) if ((SCHAR HUGE_PTR *) block <=
							  (SCHAR HUGE_PTR *) free) break;

#ifdef DEBUG_GDS_ALLOC
/* Debugging code to erase memory locations after a release - 
 * this will assist in catching dangling memory references to
 * freed locations.
 * Note that the header parts of the freed block may still be used,
 * for the free chain, so we don't zap those locations.
 */

#define ALLD_RELEASED_PATTERN	0xEE
	{
		ULONG size;
		size = BLOCKS_TO_SIZE(block->frb_header.blk_length);
		assert(size >= sizeof(struct blk));
		size -= sizeof(struct blk);
		if (size)
			memset((SCHAR *) block + sizeof(struct blk),
				   ALLD_RELEASED_PATTERN, size);
	};
#endif	/* DEBUG_GDS_ALLOC */

	if ((SCHAR HUGE_PTR *) block == (SCHAR HUGE_PTR *) free) {
		BUGCHECK("block released twice");
	}

/* Merge block into list first, then try to combine blocks */

	block->frb_next = free;
	*ptr = block;

/* Try to merge the free block with the next one down. */

	if (free) {
		if ((SCHAR HUGE_PTR *) block + EXPAND_BLOCKSIZE(&block->frb_header) ==
			(SCHAR HUGE_PTR *) free) {
			block->frb_header.blk_length += free->frb_header.blk_length;
			block->frb_next = free->frb_next;
#ifdef DEBUG_GDS_ALLOC
			/* Paint the freed header of the merged-away block */
			memset((UCHAR *) free, ALLD_RELEASED_PATTERN, sizeof(*free));
#endif	/* DEBUG_GDS_ALLOC */
		}
		else if ((SCHAR HUGE_PTR *) block +
				 EXPAND_BLOCKSIZE(&block->frb_header) >
				 (SCHAR HUGE_PTR *) free) {
			BUGCHECK("released block overlaps following free block");
		}
	}

/* Try and merge the block with the prior free block */

	if (prior) {
		if ((SCHAR HUGE_PTR *) prior + EXPAND_BLOCKSIZE(&prior->frb_header) ==
			(SCHAR HUGE_PTR *) block) {
			prior->frb_header.blk_length += block->frb_header.blk_length;
			prior->frb_next = block->frb_next;
#ifdef DEBUG_GDS_ALLOC
			/* Paint the freed header of the merged-away block */
			memset((UCHAR *) block, ALLD_RELEASED_PATTERN, sizeof(*block));
#endif	/* DEBUG_GDS_ALLOC */
		}
		else if ((SCHAR HUGE_PTR *) prior +
				 EXPAND_BLOCKSIZE(&prior->frb_header) >
				 (SCHAR HUGE_PTR *) block) {
			BUGCHECK("released block overlaps prior free block");
		}
	}
#ifdef SUPERSERVER
	if (trace_pools) {
		--alld_block_type_count[blk_header_type];
		--pool->plb_blk_type_count[blk_header_type];
	}
#endif
}


} // extern "C"
