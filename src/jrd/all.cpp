/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		all.cpp
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
#include "../jrd/common.h"
#include <stdio.h>

#include "gen/iberror.h"
#include "../jrd/jrd.h"
#include "../jrd/all.h"
#include "../jrd/tra.h"
#include "../jrd/all_proto.h"
#include "../jrd/err_proto.h"

#include <algorithm>

using namespace Jrd;

const int PERM_EXTEND_SIZE		= 16 * 1024;
const int CACH_EXTEND_SIZE		= 16 * 1024;


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
	Database* dbb = GET_DBB();

#ifdef V4_THREADING
	V4_RW_LOCK_LOCK(dbb->dbb_rw_locks + DBB_WLCK_pools, WLCK_read);
#endif

	// walk through all the pools in the database
	Firebird::vector<JrdMemoryPool*>::iterator itr;
	for (itr = dbb->dbb_pools.begin(); itr < dbb->dbb_pools.end(); ++itr)
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


JrdMemoryPool *JrdMemoryPool::createDbPool(Firebird::MemoryStats &stats) {
	JrdMemoryPool* result = (JrdMemoryPool *)internal_create(
		sizeof(JrdMemoryPool), NULL, stats);
	result->plb_buckets = NULL;
	result->plb_segments = NULL;
	result->plb_dccs = NULL;
	return result;
}

JrdMemoryPool *JrdMemoryPool::createPool() {
    Database* dbb = GET_DBB();
	fb_assert(dbb);
		
#ifdef SUPERSERVER
	JrdMemoryPool* result = (JrdMemoryPool *)internal_create(sizeof(JrdMemoryPool),
		dbb->dbb_permanent,	dbb->dbb_memory_stats);
#else
	JrdMemoryPool *result = (JrdMemoryPool *)internal_create(sizeof(JrdMemoryPool), dbb->dbb_permanent);
#endif
	result->plb_buckets = NULL;
	result->plb_segments = NULL;
	result->plb_dccs = NULL;
	dbb->dbb_pools.push_back(result);
	return result;
}

void JrdMemoryPool::deletePool(JrdMemoryPool* pool) {
	Database* dbb = GET_DBB();
	Database::pool_vec_type::iterator itr =
		std::find(dbb->dbb_pools.begin(), dbb->dbb_pools.end(), pool);
	if (itr != dbb->dbb_pools.end()) dbb->dbb_pools.erase(itr);
	MemoryPool::deletePool(pool);
}

void JrdMemoryPool::noDbbDeletePool(JrdMemoryPool* pool) {
	MemoryPool::deletePool(pool);
}

TEXT* ALL_cstring(const Firebird::string& in_string)
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
	thread_db* tdbb = JRD_get_thread_data();

	JrdMemoryPool* pool = tdbb->tdbb_default;
	if (!pool) {
		if (tdbb->tdbb_transaction)
			pool = tdbb->tdbb_transaction->tra_pool;
		else if (tdbb->tdbb_request)
			pool = tdbb->tdbb_request->req_pool;

		/* theoretically this shouldn't happen, but just in case */

		if (!pool)
			return NULL;
	}

	TEXT* p = FB_NEW(*pool) TEXT[in_string.length()];
	strcpy(p, in_string.c_str());
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
 *	because it references the Database block which gets
 *	released at the top of this routine.
 *
 **************************************/
	Database* dbb = GET_DBB();

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
	thread_db* tdbb = JRD_get_thread_data();
	Database* dbb = tdbb->tdbb_database;

	JrdMemoryPool* pool = tdbb->tdbb_default = dbb->dbb_permanent;
//	dbb->dbb_permanent->setExtendSize(PERM_EXTEND_SIZE);
	dbb->dbb_pools[0] = pool;
	dbb->dbb_bufferpool = JrdMemoryPool::createPool();
//	FB_NEW(*pool) JrdMemoryPool(CACH_EXTEND_SIZE);
}


#ifdef SUPERSERVER
void ALL_print_memory_pool_info(FILE* fptr, Database* databases)
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
 *	Walk the Database's to print out pool info of every database
 *
 **************************************/
	Database* dbb;
	STR string;
	VEC vector;
	HNK hnk;
	Attachment* att;
	int i, j, k, col;

	fprintf(fptr, "\n\tALL_xx block types\n");
	fprintf(fptr, "\t------------------");
	for (i = 0, col = 0; i < types->type_MAX; i++) {
		if (types->all_block_type_count[i]) {
			if (col % 5 == 0)
				fprintf(fptr, "\n\t");
			fprintf(fptr, "%s = %d  ", types->ALL_types[i],
					   types->all_block_type_count[i]);
			++col;
		}
	}
	fprintf(fptr, "\n");

	// TMN: Note. Evil code.
	for (i = 0, dbb = databases; dbb; dbb = dbb->dbb_next, ++i);
	fprintf(fptr, "\tNo of dbbs = %d\n", i);

	for (k = 1, dbb = databases; dbb; dbb = dbb->dbb_next, ++k)
	{
		string = dbb->dbb_filename;
		fprintf(fptr, "\n\t dbb%d -> %s\n", k, string->str_data);
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
		fprintf(fptr, "\t    %s has %d pools", string->str_data, j);
		for (j = 0, att = dbb->dbb_attachments; att; att = att->att_next)
		{
			j++;
		}
		fprintf(fptr, " and %d attachment(s)", j);
		for (i = 0; i < (int) vector->vec_count; i++)
		{
			PLB myPool = (PLB) vector->vec_object[i];
			if (!myPool) {
				continue;
			}
			fprintf(fptr, "\n\t    Pool %d", myPool->plb_pool_id);

			// Count # of hunks
			for (j = 0, hnk = myPool->plb_hunks; hnk; hnk = hnk->hnk_next) {
				++j;
			}
			if (j) {
				fprintf(fptr, " has %d hunks", j);
			}
			j = 0;

			// Count # of "huge" hunks
			for (hnk = myPool->plb_huge_hunks; hnk; hnk = hnk->hnk_next)
			{
				++j;
			}
			if (j) {
				fprintf(fptr, " and %d huge_hunks", j);
			}
			fprintf(fptr, " Extend size is %d", myPool->plb_extend_size);
			for (j = 0, col = 0; j < types->type_MAX; j++)
			{
				if (myPool->plb_blk_type_count[j])
				{
					if (col % 5 == 0)
					{
						fprintf(fptr, "\n\t    ");
					}
					fprintf(fptr, "%s = %d  ", types->ALL_types[j],
							   myPool->plb_blk_type_count[j]);
					++col;
				}
			}
		}
	}
#endif	// 0
}
#endif	// SUPERSERVER

