/*
 *	PROGRAM:	JRD Access Method
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

#ifdef SHLIB_DEFS
#define LOCAL_SHLIB_DEFS
#endif

#include "firebird.h"
#include <string.h>
#include "../jrd/ib_stdio.h"

#include "gen/codes.h"

#include "../jrd/everything.h"
#include "../jrd/all_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/thd_proto.h"

#define BLKDEF(type, root, tail) { sizeof (struct root), tail },

extern "C" {


static const struct {
	SSHORT typ_root_length;
	SSHORT typ_tail_length;
} block_sizes[] = {
	{0, 0},
#include "../jrd/blk.h"
    {0, 0}
};

#undef BLKDEF

#ifdef SUPERSERVER

extern SLONG trace_pools;
SLONG all_delta_alloc = 0;

#define BLKDEF(type, root, tail)  0,

SLONG all_block_type_count[] = {
#include "../jrd/blk.h"
	0
};
#undef BLKDEF

#define BLKDEF(type, root, tail)  #type,
const char ALL_types[][24] = {
	"type_MIN",
#include "../jrd/blk.h"
	"type_MAX"
};
#undef BLKDEF

#endif // SUPERSERVER


static BLK alloc_huge_hunk(PLB, UCHAR, ULONG, ERR_T);
static BOOLEAN extend_pool(PLB, ULONG, ERR_T);
static ULONG find_block_length(BLK);
static PLB find_pool(BLK);
static void release(FRB, PLB);

#ifdef SHLIB_DEFS
#define strlen		(*_libgds_strlen)

extern int strlen();
#endif


BLK ALL_alloc(PLB pool, UCHAR type, ULONG count_argument, ERR_T err_ret)
{
/**************************************
 *
 *	A L L _ a l l o c
 *
 **************************************
 *
 * Functional description
 *	Allocate a block from a given pool and initialize the block.
 *	This is the primary block allocation routine.
 *
 **************************************/
	BLK block;
	FRB free, *best, *ptr;
	ULONG size, count;
	SLONG best_tail, tail;
	USHORT units;

/* this is a kludge to allow OS/2 to accept the
   incoming argument as an int but do calculations
   like everybody else -- this avoids changing a lot of source */

	count = count_argument;

	if (type <= (UCHAR) type_MIN || type >= (UCHAR) type_MAX)
		BUGCHECK(150);			/* msg 150 request to allocate invalid block type */

/* Compute block length */

	size = block_sizes[type].typ_root_length;

	if ((tail = block_sizes[type].typ_tail_length) && count >= FUDGE)
		size += (count - FUDGE) * tail;

	size = (size + MIN_ALLOC - 1) & ~((ULONG) MIN_ALLOC - 1);
/* TMN: Here we should really have the following assert */
/* assert((size >> SHIFT) <= MAX_USHORT); */
	units = (USHORT) (size >> SHIFT);

	if (size >= MAX_BLOCK)
		return alloc_huge_hunk(pool, type, size, err_ret);

/* Find best fit.  Best fit is defined to be the free block of shortest
   tail.  If there isn't a fit, extend the pool and try, try again. */

	while (TRUE) {
#ifdef V4_THREADING
		V4_MUTEX_LOCK(pool->plb_mutex);
#endif

		best = NULL;
		best_tail = MAX_BLOCK;
		for (ptr = &pool->plb_free; (free = *ptr); ptr = &free->frb_next) {
			if ((SCHAR *) free >= (SCHAR *) free->frb_next
				&& free->frb_next) {
#ifdef V4_THREADING
				V4_MUTEX_UNLOCK(pool->plb_mutex);
#endif
				BUGCHECK(152);	/* msg 152 memory pool free list is invalid */
			}
			if ((tail = (ULONG) free->frb_header.blk_length - (ULONG) units)
				>= 0 && tail < best_tail) {
				best = ptr;
				best_tail = tail;
				if (tail == 0)
					break;
			}
		}
		if (best)
			break;

#ifdef V4_THREADING
		V4_MUTEX_UNLOCK(pool->plb_mutex);
#endif
		if (!extend_pool(pool, size, err_ret))
			return 0;
	}

/* We've got our free block.  If there's enough left of the free block
   after taking out our block, chop out out block.  If not, allocate
   the entire free block as our block (a little extra won't hurt). */

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

#ifdef SUPERSERVER
	if (trace_pools) {
		++all_block_type_count[type];
		++pool->plb_blk_type_count[type];
	}
#endif

#ifdef V4_THREADING
	V4_MUTEX_UNLOCK(pool->plb_mutex);
#endif
	MOVE_CLEAR(block, size);
	block->blk_type = type;
/* TMN: Here we should really have the following assert */
/* assert(pool->plb_pool_id <= MAX_UCHAR); */
	block->blk_pool_id = (UCHAR) pool->plb_pool_id;
	block->blk_length = units;

	return block;
}


#ifdef DEV_BUILD
void ALL_check_memory(void)
{
/**************************************
 *
 *	A L L _ c h e c k _ m e m o r y
 *
 **************************************
 *
 * Functional description
 *	This routine goes through all allocated
 *	memory pools and checks the hunks of 
 *	memory allocated to make sure they are
 *	valid.  It is designed to be used for
 *	debugging purposes to find bad memory
 *	areas as soon as they are corrupted. 
 *	A call to this routine can be made from
 *	looper() to ensure that it will be regularly
 *	executed.
 *
 **************************************/
	DBB dbb;
	VEC vector;
	PLB pool;
	USHORT pool_id;
	HNK hunk;

	dbb = GET_DBB;

#ifdef V4_THREADING
	V4_RW_LOCK_LOCK(dbb->dbb_rw_locks + DBB_WLCK_pools, WLCK_read);
#endif

/* walk through all the pools in the database */

	vector = dbb->dbb_pools;
	for (pool_id = 0; pool_id < vector->vec_count; pool_id++) {
		pool = (PLB) vector->vec_object[pool_id];
		if (!pool)
			continue;

		/* walk through all the hunks in the pool */

#ifdef V4_THREADING
		V4_MUTEX_LOCK(pool->plb_mutex);
#endif
		for (hunk = pool->plb_hunks; hunk; hunk = hunk->hnk_next);
		for (hunk = pool->plb_huge_hunks; hunk; hunk = hunk->hnk_next);
#ifdef V4_THREADING
		V4_MUTEX_UNLOCK(pool->plb_mutex);
#endif
	}

#ifdef V4_THREADING
	V4_RW_LOCK_UNLOCK(dbb->dbb_rw_locks + DBB_WLCK_pools);
#endif
}
#endif /* DEV_BUILD */


TEXT *ALL_cstring(TEXT * in_string)
{
/**************************************
 *
 *	A L L _ c s t r i n g
 *
 **************************************
 *
 * Functional description
 *	Copy a stack local string to an allocated
 *	string so it doesn't disappear before we 
 *	return to the user or where ever.
 *
 **************************************/
	TDBB tdbb;
	struct plb *pool;
	TEXT *p, *q;
	STR string;
	ULONG length;

	tdbb = GET_THREAD_DATA;

	if (!(pool = tdbb->tdbb_default)) {
		if (tdbb->tdbb_transaction)
			pool = tdbb->tdbb_transaction->tra_pool;
		else if (tdbb->tdbb_request)
			pool = tdbb->tdbb_request->req_pool;

		/* theoretically this shouldn't happen, but just in case */

		if (!pool)
			return NULL;
	}

	length = strlen(in_string);
	string = (STR) ALL_alloc(pool, type_str, length, ERR_jmp);
/* TMN: Here we should really have the following assert */
/* assert(length <= MAX_USHORT); */
	string->str_length = (USHORT) length;

	p = (TEXT *) string->str_data;
	q = in_string;

	while (length--)
		*p++ = *q++;
	*p = 0;

	return (TEXT *) string->str_data;
}


BLK ALL_extend(BLK* pointer, ULONG size)
{
/**************************************
 *
 *	A L L _ e x t e n d
 *
 **************************************
 *
 * Functional description
 *	Extend a repeating block, copying the constant part.
 *	NOTE:  This routine does not lock any mutexes on
 *	its own behalf.  It is assumed that mutexes will
 *	have been locked before entry.
 *
 **************************************/
	BLK block, new_;
	PLB pool;
	ULONG old_length, new_length;

	block = *pointer;
	pool = find_pool(block);
	new_ = ALL_alloc(pool, block->blk_type, size, ERR_jmp);

	if (!(old_length = block->blk_length << SHIFT))
		old_length = find_block_length(block);
	if (!(new_length = new_->blk_length << SHIFT))
		new_length = find_block_length(new_);
	MOVE_FASTER((SCHAR *) block + sizeof(struct blk),
				(SCHAR *) new_ + sizeof(struct blk),
				MIN(old_length, new_length) - sizeof(struct blk));
	release(reinterpret_cast < frb * >(block), pool);

	if (new_->blk_type == (UCHAR) type_vec)
		((VEC) new_)->vec_count = size;
	else if (new_->blk_type == (UCHAR) type_vcl)
		((VCL) new_)->vcl_count = size;

	*pointer = new_;

	return new_;
}


void ALL_fini(void)
{
/**************************************
 *
 *	A L L _ f i n i
 *
 **************************************
 *
 * Functional description
 *	Get rid of everything.
 *	NOTE:  This routine does not lock any mutexes on
 *	its own behalf.  It is assumed that mutexes will
 *	have been locked before entry.
 *	Call gds__free explicitly instead of ALL_free
 *	because it references the dbb block which gets
 *	released at the top of this routine.
 *
 **************************************/
	DBB dbb;
	VEC pools;
	PLB pool, *vector, *until;
	HNK hunks, hunk;

	dbb = GET_DBB;

	pools = dbb->dbb_pools;

/* We release the database block simply to invalidate it's block type. */

	ALL_release(reinterpret_cast < frb * >(dbb));

	until = (PLB *) pools->vec_object;
	for (vector = until + pools->vec_count; --vector >= until;)
		if (pool = *vector) {
#ifdef SUPERSERVER
			if (trace_pools && pool->plb_blk_type_count) {
				int i = 0;
				for (i = 0; i < type_MAX; i++)
					all_block_type_count[i] -= pool->plb_blk_type_count[i];
				gds__free(pool->plb_blk_type_count);
			}
#endif
			for (hunks = pool->plb_huge_hunks; hunk = hunks;) {
				hunks = hunk->hnk_next;
				gds__sys_free(hunk->hnk_address);
			}
			for (hunks = pool->plb_hunks; hunk = hunks;) {
				hunks = hunk->hnk_next;
#ifdef SUPERSERVER
				all_delta_alloc -= gds__free(hunk->hnk_address);
#else
				gds__free(hunk->hnk_address);
#endif
			}
		}
}


void ALL_free(SCHAR * memory)
{
/**************************************
 *
 *	A L L _ f r e e
 *
 **************************************
 *
 * Functional description
 *	Give space back to internal memory heap.
 *	The process retains the memory for quick
 *	reallocations.
 *
 **************************************/
	DBB dbb;

	dbb = GET_DBB;

#ifdef V4_THREADING
	V4_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_statistics);
#endif

#ifdef SUPERSERVER
	{
		SLONG freed;
		freed = gds__free(memory);
		dbb->dbb_current_memory -= freed;
		all_delta_alloc -= freed;
	}
#else
	dbb->dbb_current_memory -= gds__free(memory);
#endif

#ifdef V4_THREADING
	V4_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_statistics);
#endif
}


ULONG ALL_get_free_object(PLB pool, VEC * vector_ptr, USHORT increment)
{
/**************************************
 *
 *	A L L _ g e t _ f r e e _ o b j e c t
 *
 **************************************
 *
 * Functional description
 *	Find a free slot in a vector.  If no
 *	slot is available, extend the vector.
 *	
 **************************************/
	VEC vector;
	ULONG slot;
	BLK *ptr, *end;

	if (!(vector = *vector_ptr)) {
		vector = *vector_ptr =
			(VEC) ALL_alloc(pool, type_vec, increment, ERR_jmp);
		vector->vec_count = increment;
		slot = 0;
	}
	else {
		BLKCHK(vector, type_vec);
		for (ptr = vector->vec_object, end = ptr + vector->vec_count;
			 ptr < end && *ptr; ptr++);
		if (ptr < end)
			slot = ptr - vector->vec_object;
		else {
			slot = vector->vec_count;
			ALL_extend(reinterpret_cast < blk ** >(vector_ptr),
					   slot + increment);
		}
	}

	return slot;
}


void ALL_init(void)
{
/**************************************
 *
 *	A L L _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize the pool system.
 *	NOTE:  This routine does not lock any mutexes on
 *	its own behalf.  It is assumed that mutexes will
 *	have been locked before entry.
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;
	ISC_STATUS_ARRAY temp_vector;
	VEC vector;
	PLB pool;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;

	dbb->dbb_pools = vector = (VEC) temp_vector;
	vector->vec_count = 1;
	vector->vec_object[0] = NULL;

	tdbb->tdbb_default = dbb->dbb_permanent = pool = ALL_pool();
	dbb->dbb_pools = vector = (VEC) ALL_alloc(pool, type_vec, 10, ERR_jmp);
	vector->vec_count = 10;
	vector->vec_object[0] = (BLK) pool;

	dbb->dbb_bufferpool = ALL_pool();
}


SCHAR *ALL_malloc(ULONG size, ERR_T err_ret)
{
/**************************************
 *
 *	A L L _ m a l l o c
 *
 **************************************
 *
 * Functional description
 *	Get memory from internal memory heap.
 *
 **************************************/
	DBB dbb;
	SCHAR *memory;

	if (memory = (SCHAR *) gds__alloc((SLONG) (size))) {
		/* FREE:  Done at attachment detach, process exit, and other misc times */

		dbb = GET_DBB;

#ifdef V4_THREADING
		V4_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_statistics);
#endif
#ifdef SUPERSERVER
		all_delta_alloc += size;
#endif
		dbb->dbb_current_memory += size;
		if (dbb->dbb_current_memory > dbb->dbb_max_memory)
			dbb->dbb_max_memory = dbb->dbb_current_memory;
#ifdef V4_THREADING
		V4_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_statistics);
#endif
		return memory;
	}

/* NOMEM: post user level error */
	if (err_ret == ERR_jmp)
		ERR_post(gds_sys_request, gds_arg_string, "gds__alloc", gds_arg_gds,
				 gds_virmemexh, 0);

	return NULL;
}


PLB ALL_pool(void)
{
/**************************************
 *
 *	A L L _ p o o l
 *
 **************************************
 *
 * Functional description
 *	Allocate a new pool.  This is done by creating a tempory
 *	pool block on the stack, then allocating a real pool block.
 *	In USHORT, by mirrors.
 *
 **************************************/
	DBB dbb;
	struct plb temp_pool;
	VEC vector;
	PLB pool;
	USHORT pool_id;

	dbb = GET_DBB;

#ifdef V4_THREADING
	V4_RW_LOCK_LOCK(dbb->dbb_rw_locks + DBB_WLCK_pools, WLCK_write);
#endif

/* Start by assigning a pool id */

	vector = dbb->dbb_pools;

	for (pool_id = 0; pool_id < vector->vec_count; pool_id++)
		if (!(vector->vec_object[pool_id]))
			break;

	if (pool_id >= vector->vec_count)
		vector =
			(VEC) ALL_extend(reinterpret_cast < blk ** >(&dbb->dbb_pools),
							 pool_id + 10);

	vector->vec_object[pool_id] = (BLK) & temp_pool;
	temp_pool.plb_free = NULL;
	temp_pool.plb_hunks = NULL;
	temp_pool.plb_pool_id = pool_id;
	temp_pool.plb_extend_size = MIN_ALLOCATION;
	temp_pool.plb_blk_type_count = NULL;
#ifdef SUPERSERVER
	if (trace_pools) {
		temp_pool.plb_blk_type_count =
			(SLONG *) gds__alloc(sizeof(all_block_type_count));
		if (!temp_pool.plb_blk_type_count)
			trace_pools = 0;	/* No memmory!! stop tracing pool info */
		else
			memset(temp_pool.plb_blk_type_count, 0,
				   sizeof(all_block_type_count));
	}
#endif
#ifdef V4_THREADING
	V4_MUTEX_INIT(temp_pool.plb_mutex);
#endif
	if (pool_id == 0) {
		dbb->dbb_permanent = &temp_pool;
		temp_pool.plb_extend_size = PERM_EXTEND_SIZE;
	}
	if (pool_id == 1)
		temp_pool.plb_extend_size = CACH_EXTEND_SIZE;

	pool = (PLB) ALL_alloc(&temp_pool, type_plb, 0, ERR_jmp);
	pool->plb_pool_id = pool_id;
	pool->plb_extend_size = temp_pool.plb_extend_size;
	pool->plb_free = temp_pool.plb_free;
	pool->plb_hunks = temp_pool.plb_hunks;
#ifdef SUPERSERVER
	pool->plb_blk_type_count = temp_pool.plb_blk_type_count;
#endif
	vector->vec_object[pool_id] = (BLK) pool;

#ifdef V4_THREADING
	V4_MUTEX_DESTROY(temp_pool.plb_mutex);
	V4_MUTEX_INIT(pool->plb_mutex);

	V4_RW_LOCK_UNLOCK(dbb->dbb_rw_locks + DBB_WLCK_pools);
#endif
	if (pool_id == 0)
		dbb->dbb_permanent = pool;

	return pool;
}


void ALL_push(BLK object, LLS * stack)
{
/**************************************
 *
 *	A L L _ p u s h
 *
 **************************************
 *
 * Functional description
 *	Push an object on an LLS stack.
 *
 **************************************/
	TDBB tdbb;
	LLS node;
	PLB pool;

	tdbb = GET_THREAD_DATA;

	pool = tdbb->tdbb_default;

#ifdef V4_THREADING
	V4_MUTEX_LOCK(pool->plb_mutex);
#endif
	if (node = pool->plb_lls) {
		pool->plb_lls = node->lls_next;
#ifdef V4_THREADING
		V4_MUTEX_UNLOCK(pool->plb_mutex);
#endif
	}
	else {
#ifdef V4_THREADING
		V4_MUTEX_UNLOCK(pool->plb_mutex);
#endif
		node = (LLS) ALL_alloc(pool, type_lls, 0, ERR_jmp);
	}

	node->lls_object = object;
	node->lls_next = *stack;
	*stack = node;
}


BLK ALL_pop(LLS * stack)
{
/**************************************
 *
 *	A L L _ p o p
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
	BLK object;

	node = *stack;
	*stack = node->lls_next;
	object = node->lls_object;

	pool = find_pool((BLK) node);
#ifdef V4_THREADING
	V4_MUTEX_LOCK(pool->plb_mutex);
#endif
	node->lls_next = pool->plb_lls;
	pool->plb_lls = node;
#ifdef V4_THREADING
	V4_MUTEX_UNLOCK(pool->plb_mutex);
#endif

	return object;
}


#ifdef SUPERSERVER
void ALL_print_memory_pool_info(IB_FILE * fptr, DBB databases)
{
/***********************************************************
 *
 *	A L L _ p r i n t _ m e m o r y _ p o o l _ i n f o
 *
 ***********************************************************
 *
 * Functional description
 *	Print the different block types allocated in the pool.
 *	Walk the dbb's to print out pool info of every database
 *
 **************************************/
	DBB dbb;
	STR string;
	VEC vector;
	PLB myPool;
	HNK hnk;
	ATT att;
	int i, j, k, col;

	ib_fprintf(fptr, "\n\tALL_xx block types\n");
	ib_fprintf(fptr, "\t------------------");
	for (i = 0, col = 0; i < type_MAX; i++)
		if (all_block_type_count[i]) {
			if (col % 5 == 0)
				ib_fprintf(fptr, "\n\t");
			ib_fprintf(fptr, "%s = %d  ", ALL_types[i],
					   all_block_type_count[i]);
			++col;
		}
	ib_fprintf(fptr, "\n");

	for (i = 0, dbb = databases; dbb; dbb = dbb->dbb_next, ++i);
	ib_fprintf(fptr, "\tNo of dbbs = %d\n", i);
	for (k = 1, dbb = databases; dbb; dbb = dbb->dbb_next, ++k) {
		string = dbb->dbb_filename;
		ib_fprintf(fptr, "\n\t dbb%d -> %s\n", k, string->str_data);
		vector = (VEC) dbb->dbb_pools;
		for (j = 0, i = 0; i < (int) vector->vec_count; i++) {
			myPool = (PLB) vector->vec_object[i];
			if (myPool)
				++j;
		}
		ib_fprintf(fptr, "\t    %s has %d pools", string->str_data, j);
		for (j = 0, att = dbb->dbb_attachments; att; att = att->att_next)
			j++;
		ib_fprintf(fptr, " and %d attachment(s)", j);
		for (i = 0; i < (int) vector->vec_count; i++) {
			myPool = (PLB) vector->vec_object[i];
			if (!myPool)
				continue;
			ib_fprintf(fptr, "\n\t    Pool %d", myPool->plb_pool_id);
			for (j = 0, hnk = myPool->plb_hunks; hnk; hnk = hnk->hnk_next)
				j++;
			if (j)
				ib_fprintf(fptr, " has %d hunks", j);
			for (j = 0, hnk = myPool->plb_huge_hunks; hnk;
				 hnk = hnk->hnk_next) j++;
			if (j)
				ib_fprintf(fptr, " and %d huge_hunks", j);
			ib_fprintf(fptr, " Extend size is %d", myPool->plb_extend_size);
			for (j = 0, col = 0; j < type_MAX; j++)
				if (myPool->plb_blk_type_count[j]) {
					if (col % 5 == 0)
						ib_fprintf(fptr, "\n\t    ");
					ib_fprintf(fptr, "%s = %d  ", ALL_types[j],
							   myPool->plb_blk_type_count[j]);
					++col;
				}
		}
	}
}
#endif


void ALL_release(FRB block)
{
/**************************************
 *
 *	A L L _ r e l e a s e
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

/* TMN: Here we should probably have the following assert */
/* assert(block); */
	release(block, find_pool(block ? &block->frb_header : (BLK) 0));
}


void ALL_rlpool(PLB pool)
{
/**************************************
 *
 *	A L L _ r l p o o l
 *
 **************************************
 *
 * Functional description
 *	Release a storage pool.  This involves nothing more than returning
 *	hunks to the free hunk list.
 *
 **************************************/
	DBB dbb;
	HNK hunks, hunk;

	dbb = GET_DBB;

#ifdef V4_THREADING
	V4_RW_LOCK_LOCK(dbb->dbb_rw_locks + DBB_WLCK_pools, WLCK_write);
#endif
	dbb->dbb_pools->vec_object[pool->plb_pool_id] = NULL;
#ifdef V4_THREADING
	V4_RW_LOCK_UNLOCK(dbb->dbb_rw_locks + DBB_WLCK_pools);

	V4_MUTEX_DESTROY(pool->plb_mutex);
#endif

#ifdef SUPERSERVER
	if (trace_pools && pool->plb_blk_type_count) {
		int i = 0;
		for (i = 0; i < type_MAX; i++)
			all_block_type_count[i] -= pool->plb_blk_type_count[i];
		gds__free(pool->plb_blk_type_count);
	}
#endif

	for (hunks = pool->plb_huge_hunks; hunk = hunks;) {
		hunks = hunk->hnk_next;
		ALL_sys_free(hunk->hnk_address);
	}
	for (hunks = pool->plb_hunks; hunk = hunks;) {
		hunks = hunk->hnk_next;
		ALL_free(hunk->hnk_address);
	}
}


SCHAR *ALL_sys_alloc(ULONG size, ERR_T err_ret)
{
/**************************************
 *
 *	A L L _ s y s _  a l l o c
 *
 **************************************
 *
 * Functional description
 *	Get memory from OS in such a way
 *	that it can be freed back to OS.
 *
 **************************************/
	DBB dbb;
	SCHAR *memory;

	if (memory = (SCHAR *) gds__sys_alloc((SLONG) size)) {
		/* FREE:  Done at attachment detach, process exit, and other misc times */

		dbb = GET_DBB;

#ifdef V4_THREADING
		V4_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_statistics);
#endif
		dbb->dbb_current_memory += size;
		if (dbb->dbb_current_memory > dbb->dbb_max_memory)
			dbb->dbb_max_memory = dbb->dbb_current_memory;
#ifdef V4_THREADING
		V4_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_statistics);
#endif
		return memory;
	}

/* NOMEM: post user level error */
	if (err_ret == ERR_jmp)
		ERR_post(gds_sys_request, gds_arg_string, "gds__sys_alloc",
				 gds_arg_gds, gds_virmemexh, 0);

	return NULL;
}


void ALL_sys_free(SCHAR * memory)
{
/**************************************
 *
 *	A L L _ s y s _ f r e e
 *
 **************************************
 *
 * Functional description
 *	Give space back to OS ... really!
 *
 **************************************/
	DBB dbb;

	dbb = GET_DBB;

#ifdef V4_THREADING
	V4_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_statistics);
#endif

	dbb->dbb_current_memory -= gds__sys_free(memory);

#ifdef V4_THREADING
	V4_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_statistics);
#endif
}


ULONG ALL_tail(UCHAR type)
{
/**************************************
 *
 *	A L L _ t a i l
 *
 **************************************
 *
 * Functional description
 *	Compute maximum repeating tail count.
 *
 **************************************/
	ULONG root, tail;

	if (type <= (UCHAR) type_MIN || type >= (SCHAR) type_MAX)
		BUGCHECK(150);			/* msg 150 request to allocate invalid block type */

/* Compute repeating tail count */

	root = block_sizes[type].typ_root_length;
	if (!(tail = block_sizes[type].typ_tail_length))
		return 0;

	return ((MAX_BLOCK - MIN_ALLOC - root) / tail) + FUDGE;
}


VEC ALL_vector(PLB pool, VEC* ptr, USHORT count)
{
/**************************************
 *
 *	A L L _ v e c t o r
 *
 **************************************
 *
 * Functional description
 *	Allocate, extend, or no-op a vector making sure it is "long enough".
 *	NOTE:  This routine does not lock any mutexes on
 *	its own behalf.  It is assumed that mutexes will
 *	have been locked before entry.
 *
 **************************************/
	VEC vector;

	++count;

	if (!(vector = *ptr)) {
		vector = *ptr = (VEC) ALL_alloc(pool, type_vec, count, ERR_jmp);
		vector->vec_count = count;
		return vector;
	}

	BLKCHK(vector, type_vec);

	if (vector->vec_count >= count)
		return vector;

	return (VEC) ALL_extend(reinterpret_cast < blk ** >(ptr), count);
}


static BLK alloc_huge_hunk(PLB pool, UCHAR type, ULONG size, ERR_T err_ret)
{
/**************************************
 *
 *	a l l o c _ h u g e _ h u n k
 *
 **************************************
 *
 * Functional description
 *	Allocate a hunk of memory larger than
 *	MAX_BLOCK.  Link it into a special
 *	list of huge hunks belonging to the pool.
 *
 **************************************/
	HNK hunk;
	BLK block;
	ULONG minimum_size;

	minimum_size = size + sizeof(struct hnk);
	size =
		(minimum_size + MIN_ALLOCATION - 1) & ~((ULONG) MIN_ALLOCATION - 1);

	if (!(hunk = (HNK) ALL_sys_alloc((ULONG) size, err_ret)))
		return 0;
	hunk->hnk_header.blk_length = sizeof(struct hnk) >> SHIFT;
	hunk->hnk_header.blk_type = (UCHAR) type_hnk;
/* TMN: Here we should really have the following assert */
/* assert(pool->plb_pool_id <= MAX_UCHAR); */
	hunk->hnk_header.blk_pool_id = (UCHAR) pool->plb_pool_id;
	hunk->hnk_address = (SCHAR *) hunk;
	hunk->hnk_length = size;
#ifdef V4_THREADING
	V4_MUTEX_LOCK(pool->plb_mutex);
#endif
	hunk->hnk_next = pool->plb_huge_hunks;
	pool->plb_huge_hunks = hunk;
#ifdef V4_THREADING
	V4_MUTEX_UNLOCK(pool->plb_mutex);
#endif

	block = (BLK) (hunk + 1);
	MOVE_CLEAR(block, size - sizeof(struct hnk));
	block->blk_length = 0;
	block->blk_type = type;
/* TMN: Here we should really have the following assert */
/* assert(pool->plb_pool_id <= MAX_UCHAR); */
	block->blk_pool_id = (UCHAR) pool->plb_pool_id;

	return block;
}


static BOOLEAN extend_pool(PLB pool, ULONG size, ERR_T err_ret)
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
	ULONG minimum_size, extend_size;

	minimum_size = size + sizeof(struct hnk);

/* Keep minimum allocation roundup for blocks larger than pool
   extend size. */

	extend_size = (ULONG) pool->plb_extend_size;
	if (extend_size < minimum_size)
		extend_size = MIN_ALLOCATION;

	size = (minimum_size + extend_size - 1) & ~(extend_size - 1);

	if (!(block = (BLK) ALL_malloc((ULONG) size, err_ret)))
		return FALSE;
/* TMN: Here we should really have the following assert */
/* assert((size >> SHIFT) <= MAX_USHORT); */
	block->blk_length = (USHORT) (size >> SHIFT);
	block->blk_type = (UCHAR) type_frb;
/* TMN: Here we should really have the following assert */
/* assert(pool->plb_pool_id <= MAX_UCHAR); */
	block->blk_pool_id = (UCHAR) pool->plb_pool_id;
#ifdef SUPERSERVER
	if (trace_pools) {
		++all_block_type_count[block->blk_type];
		++pool->plb_blk_type_count[block->blk_type];
	}
#endif
	release(reinterpret_cast < frb * >(block), pool);

	if (!(hunk = (HNK) ALL_alloc(pool, type_hnk, 0, err_ret)))
		return FALSE;
	hunk->hnk_address = (SCHAR *) block;
	hunk->hnk_length = size;

#ifdef V4_THREADING
	V4_MUTEX_LOCK(pool->plb_mutex);
#endif
	hunk->hnk_next = pool->plb_hunks;
	pool->plb_hunks = hunk;
#ifdef V4_THREADING
	V4_MUTEX_UNLOCK(pool->plb_mutex);
#endif
	return TRUE;
}


static ULONG find_block_length(BLK block)
{
/**************************************
 *
 *	f i n d _ b l o c k _ l e n g t h
 *
 **************************************
 *
 * Functional description
 *	The blocks that are associated with huge
 *	hunks always have a zero block length.
 *	Their length can be determined indirectly,
 *	using the length of the hunk.
 *
 **************************************/
	HNK hunk;

	hunk = (HNK) block - 1;

	return (hunk->hnk_length - sizeof(struct hnk));
}


static PLB find_pool(BLK block)
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
	DBB dbb;
	VEC pools;
	PLB pool;
	HNK hunk;
	USHORT pool_id, huge_flag;
#ifdef V4_THREADING
	USHORT dbb_pool_flag;
#endif

	dbb = GET_DBB;

#ifdef V4_THREADING
	if (!(dbb_pool_flag = (block == (BLK) dbb->dbb_pools)))
		V4_RW_LOCK_LOCK(dbb->dbb_rw_locks + DBB_WLCK_pools, WLCK_read);
#endif

	pools = dbb->dbb_pools;

/* Turn on this optimization someday */

	if (pools->vec_count < 256)
		if ((pool_id = block->blk_pool_id) < pools->vec_count &&
			(pool = (PLB) pools->vec_object[pool_id])) {
#ifdef V4_THREADING
			if (!dbb_pool_flag)
				V4_RW_LOCK_UNLOCK(dbb->dbb_rw_locks + DBB_WLCK_pools);
#endif
			return pool;
		}
		else {
#ifdef V4_THREADING
			if (!dbb_pool_flag)
				V4_RW_LOCK_UNLOCK(dbb->dbb_rw_locks + DBB_WLCK_pools);
#endif
			BUGCHECK(153);
		}

	huge_flag = (block->blk_length) ? FALSE : TRUE;

	for (pool_id = block->blk_pool_id; pool_id < pools->vec_count;
		 pool_id += 256)
		if (pool = (PLB) pools->vec_object[pool_id]) {
			hunk = (huge_flag) ? pool->plb_huge_hunks : pool->plb_hunks;
			for (; hunk; hunk = hunk->hnk_next)
				if ((SCHAR *) block >=
					(SCHAR *) hunk->hnk_address
					&& (SCHAR *) block <
					(SCHAR *) hunk->hnk_address + hunk->hnk_length) {
#ifdef V4_THREADING
					if (!dbb_pool_flag)
						V4_RW_LOCK_UNLOCK(dbb->dbb_rw_locks + DBB_WLCK_pools);
#endif
					return pool;
				}
		}

#ifdef V4_THREADING
	if (!dbb_pool_flag)
		V4_RW_LOCK_UNLOCK(dbb->dbb_rw_locks + DBB_WLCK_pools);
#endif
	BUGCHECK(153);				/* msg 153 invalid pool id encountered */
/* Harbor Software: */
/* this code should not be reached. */
	return NULL;
}


static void release(FRB block, PLB pool)
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
	FRB prior, free;
	FRB *ptr;
	ULONG length;
#ifdef SUPERSERVER
	UCHAR blk_header_type;
#endif

#ifdef V4_THREADING
	V4_MUTEX_LOCK(pool->plb_mutex);
#endif

	if (!block->frb_header.blk_length) {
		/* A block with no length indicates a HUGE hunk.  Find it and free it. */

		HNK hunk, *ptr;

		for (ptr = &pool->plb_huge_hunks; hunk = *ptr; ptr = &hunk->hnk_next)
			if ((SCHAR *) block >= (SCHAR *) hunk->hnk_address &&
				(SCHAR *) block <
				(SCHAR *) hunk->hnk_address + hunk->hnk_length) {
				*ptr = hunk->hnk_next;
#ifdef V4_THREADING
				V4_MUTEX_UNLOCK(pool->plb_mutex);
#endif
				ALL_sys_free(hunk->hnk_address);
				return;
			}
#ifdef V4_THREADING
		V4_MUTEX_UNLOCK(pool->plb_mutex);
#endif
		BUGCHECK(154);			/* msg 154 attempt to release free block */
	}

#ifdef SUPERSERVER
	blk_header_type = block->frb_header.blk_type;
#endif
	block->frb_header.blk_type = (UCHAR) type_frb;
	prior = NULL;

#ifdef DEBUG_GDS_ALLOC
/* Debugging code to erase memory locations after a release - 
 * this will assist in catching dangling memory references to
 * freed locations.
 * Note that the header parts of the freed block may still be used,
 * for the free chain, so we don't zap those locations.
 */

/* Pattern to set "released" memory to */
#define ALL_RELEASED_FREE_PATTERN	0xBB
	{
		ULONG units, size;
		units = block->frb_header.blk_length;
		size = (units << SHIFT) - sizeof(*block);
		memset(((UCHAR *) block) + sizeof(*block), ALL_RELEASED_FREE_PATTERN,
			   size);
	};
#endif /* DEBUG_GDS_ALLOC */

	for (ptr = &pool->plb_free; free = *ptr;
		 prior = free, ptr =
		 &free->frb_next) if ((SCHAR *) block <=
							  (SCHAR *) free) break;

	if ((SCHAR *) block == (SCHAR *) free) {
#ifdef V4_THREADING
		V4_MUTEX_UNLOCK(pool->plb_mutex);
#endif
		BUGCHECK(154);			/* msg 154 attempt to release free block */
	}

/* Merge block into list first, then try to combine blocks */

	block->frb_next = free;
	*ptr = block;

/* Try to merge the free block with the next one down. */

	length = block->frb_header.blk_length << SHIFT;

	if (free) {
		if ((SCHAR *) block + length == (SCHAR *) free) {
			block->frb_header.blk_length += free->frb_header.blk_length;
			block->frb_next = free->frb_next;
#ifdef DEBUG_GDS_ALLOC
			/* We've joined the blocks, so paint the header of the merged away 
			 * block as freed.
			 */
			memset(((UCHAR *) free), ALL_RELEASED_FREE_PATTERN,
				   sizeof(*free));
#endif
		}
		else if ((SCHAR *) block + length > (SCHAR *) free) {
#ifdef V4_THREADING
			V4_MUTEX_UNLOCK(pool->plb_mutex);
#endif
			BUGCHECK(155);		/* msg 155 attempt to release block overlapping following free block */
		}
	}

/* Try and merge the block with the prior free block */

	if (prior && (length = prior->frb_header.blk_length << SHIFT)) {
		if ((SCHAR *) prior + length == (SCHAR *) block) {
			prior->frb_header.blk_length += block->frb_header.blk_length;
			prior->frb_next = block->frb_next;
#ifdef DEBUG_GDS_ALLOC
			/* We've joined the blocks, so paint the header of the merged away 
			 * block as freed.
			 */
			memset(((UCHAR *) block), ALL_RELEASED_FREE_PATTERN,
				   sizeof(*block));
#endif
		}
		else if ((SCHAR *) prior + length > (SCHAR *) block) {
#ifdef V4_THREADING
			V4_MUTEX_UNLOCK(pool->plb_mutex);
#endif
			BUGCHECK(156);		/* msg 156 attempt to release block overlapping prior free block */
		}
	}

#ifdef SUPERSERVER
	if (trace_pools) {
		--all_block_type_count[blk_header_type];
		--pool->plb_blk_type_count[blk_header_type];
	}
#endif

#ifdef V4_THREADING
	V4_MUTEX_UNLOCK(pool->plb_mutex);
#endif
}


} // extern "C"
