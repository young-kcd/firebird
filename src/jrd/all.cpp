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

#include "firebird.h"
#include <string.h>
#include "../jrd/ib_stdio.h"

#include "gen/iberror.h"
//#include "../jrd/common.h"
//#include "../jrd/isc.h"
#include "../jrd/jrd.h"
//#include "../jrd/lck.h"
//#include "../jrd/ods.h"
//#include "../jrd/cch.h"
#include "../jrd/all.h"
//#include "../jrd/os/pio.h"
//#include "../jrd/pag.h"
//#include "../jrd/val.h"
//#include "../jrd/exe.h"
//#include "../jrd/req.h"
//#include "../jrd/lls.h"
//#include "../jrd/rse.h"
//#include "../jrd/sbm.h"
#include "../jrd/tra.h"
//#include "../jrd/sqz.h"
//#include "../jrd/blb.h"
//#include "../jrd/btr.h"
//#include "../jrd/jrn.h"
//#include "../jrd/scl.h"
//#include "../jrd/ext.h"
//#include "../jrd/met.h"
//#include "../jrd/sdw.h"
//#include "../jrd/log.h"
//#include "../jrd/intl.h"
//#include "../jrd/intl_classes.h"
//#include "../jrd/fil.h"
//#include "../jrd/tpc.h"
//#include "../jrd/rng.h"
//#include "../jrd/svc.h"
//#include "../jrd/blob_filter.h"


#include "../jrd/all_proto.h"
#include "../jrd/err_proto.h"
//#include "../jrd/gds_proto.h"
//#include "../jrd/mov_proto.h"
#include "../jrd/thd_proto.h"

#include <algorithm>

#define PERM_EXTEND_SIZE        (16 * 1024)
#define CACH_EXTEND_SIZE        (16 * 1024)


#ifdef DEV_BUILD
void ALL_check_memory()
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

	Firebird::vector<JrdMemoryPool*>::iterator itr;

	DBB Dbb = GET_DBB;

#ifdef V4_THREADING
	V4_RW_LOCK_LOCK(dbb->dbb_rw_locks + DBB_WLCK_pools, WLCK_read);
#endif

	// walk through all the pools in the database
	for (itr = Dbb->dbb_pools.begin(); itr < Dbb->dbb_pools.end(); ++itr)
	{
		JrdMemoryPool* pool = *itr;
		if (pool) {
			// walk through all the hunks in the pool
			pool->verify_pool();
		}
	}

#ifdef V4_THREADING
	V4_RW_LOCK_UNLOCK(dbb->dbb_rw_locks + DBB_WLCK_pools);
#endif
}
#endif /* DEV_BUILD */


JrdMemoryPool *JrdMemoryPool::createPool(int *cur_mem, int *max_mem) {
	DBB dbb = GET_DBB;
	JrdMemoryPool *result = (JrdMemoryPool *)internal_create(sizeof(JrdMemoryPool),
		cur_mem, max_mem);
	result->plb_buckets = NULL;
	result->plb_segments = NULL;
	result->plb_dccs = NULL;
	new (&result->lls_cache) BlockCache<lls> (*result);
	if (dbb) dbb->dbb_pools.push_back(result);
	return result;
}

JrdMemoryPool *JrdMemoryPool::createPool() {
    DBB dbb = GET_DBB;
#ifdef SUPERSERVER
	JrdMemoryPool *result = (JrdMemoryPool *)internal_create(sizeof(JrdMemoryPool),
		(int*)&dbb->dbb_current_memory, (int*)&dbb->dbb_max_memory);
#else
	JrdMemoryPool *result = (JrdMemoryPool *)internal_create(sizeof(JrdMemoryPool));
#endif
	result->plb_buckets = NULL;
	result->plb_segments = NULL;
	result->plb_dccs = NULL;
	new (&result->lls_cache) BlockCache<lls> (*result);
	if (dbb) dbb->dbb_pools.push_back(result);
	return result;
}

void JrdMemoryPool::deletePool(JrdMemoryPool* pool) {
	DBB dbb = GET_DBB;
	dbb::pool_vec_type::iterator itr =
		std::find(dbb->dbb_pools.begin(), dbb->dbb_pools.end(), pool);
	if (itr != dbb->dbb_pools.end()) dbb->dbb_pools.erase(itr);
	pool->lls_cache.~BlockCache<lls>();
	MemoryPool::deletePool(pool);
}

TEXT* ALL_cstring(TEXT* in_string)
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
	JrdMemoryPool *pool;
	TEXT *p;
	size_t length;

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
	p = FB_NEW(*pool) TEXT[length+1];
	strcpy(p,in_string);	
	return p;
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

	dbb = GET_DBB;

	/* Don't know if we even need to do this, so it is commented out */
	//delete dbb;
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
	JrdMemoryPool* pool;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;

	pool = tdbb->tdbb_default = dbb->dbb_permanent;
//	dbb->dbb_permanent->setExtendSize(PERM_EXTEND_SIZE);
	dbb->dbb_pools[0] = pool;
	dbb->dbb_bufferpool = JrdMemoryPool::createPool();
//	FB_NEW(*pool) JrdMemoryPool(CACH_EXTEND_SIZE);
}

void JrdMemoryPool::ALL_push(BLK object, LLS * stack)
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
	JrdMemoryPool* pool;

	tdbb = GET_THREAD_DATA;

	pool = tdbb->tdbb_default;
	node = pool->lls_cache.newBlock();
	node->lls_object = object;
	node->lls_next = *stack;
	*stack = node;
}


BLK JrdMemoryPool::ALL_pop(LLS *stack)
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
	JrdMemoryPool* pool;
	BLK object;

	node = *stack;
	*stack = node->lls_next;
	object = node->lls_object;

	pool = (JrdMemoryPool*)MemoryPool::blk_pool(node);
	pool->lls_cache.returnBlock(node);

	return object;
}


#ifdef SUPERSERVER
void ALL_print_memory_pool_info(IB_FILE* fptr, DBB databases)
{
#ifdef NOT_USED_OR_REPLACED
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
	HNK hnk;
	ATT att;
	int i, j, k, col;

	ib_fprintf(fptr, "\n\tALL_xx block types\n");
	ib_fprintf(fptr, "\t------------------");
	for (i = 0, col = 0; i < types->type_MAX; i++) {
		if (types->all_block_type_count[i]) {
			if (col % 5 == 0)
				ib_fprintf(fptr, "\n\t");
			ib_fprintf(fptr, "%s = %d  ", types->ALL_types[i],
					   types->all_block_type_count[i]);
			++col;
		}
	}
	ib_fprintf(fptr, "\n");

	// TMN: Note. Evil code.
	for (i = 0, dbb = databases; dbb; dbb = dbb->dbb_next, ++i);
	ib_fprintf(fptr, "\tNo of dbbs = %d\n", i);

	for (k = 1, dbb = databases; dbb; dbb = dbb->dbb_next, ++k)
	{
		string = dbb->dbb_filename;
		ib_fprintf(fptr, "\n\t dbb%d -> %s\n", k, string->str_data);
		vector = (VEC) dbb->dbb_pools;
		j = 0;
		for (pool_vec_type::iterator itr = dbb->dbb_pools.begin();
			itr != dbb->dbb_pools.end(); ++itr)
		{
			PLB myPool = *itr;
			if (myPool) {
				++j;
			}
		}
		ib_fprintf(fptr, "\t    %s has %d pools", string->str_data, j);
		for (j = 0, att = dbb->dbb_attachments; att; att = att->att_next)
		{
			j++;
		}
		ib_fprintf(fptr, " and %d attachment(s)", j);
		for (i = 0; i < (int) vector->vec_count; i++)
		{
			PLB myPool = (PLB) vector->vec_object[i];
			if (!myPool) {
				continue;
			}
			ib_fprintf(fptr, "\n\t    Pool %d", myPool->plb_pool_id);

			// Count # of hunks
			for (j = 0, hnk = myPool->plb_hunks; hnk; hnk = hnk->hnk_next) {
				++j;
			}
			if (j) {
				ib_fprintf(fptr, " has %d hunks", j);
			}
			j = 0;

			// Count # of "huge" hunks
			for (hnk = myPool->plb_huge_hunks; hnk; hnk = hnk->hnk_next)
			{
				++j;
			}
			if (j) {
				ib_fprintf(fptr, " and %d huge_hunks", j);
			}
			ib_fprintf(fptr, " Extend size is %d", myPool->plb_extend_size);
			for (j = 0, col = 0; j < types->type_MAX; j++)
			{
				if (myPool->plb_blk_type_count[j])
				{
					if (col % 5 == 0)
					{
						ib_fprintf(fptr, "\n\t    ");
					}
					ib_fprintf(fptr, "%s = %d  ", types->ALL_types[j],
							   myPool->plb_blk_type_count[j]);
					++col;
				}
			}
		}
	}
#endif	// 0
}
#endif	// SUPERSERVER

