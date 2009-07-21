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
#include "../jrd/err_proto.h"
#include "../common/classes/array.h"

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

	// walk through all the pools in the database
	Firebird::Array<JrdMemoryPool*>::iterator itr;
	for (itr = dbb->dbb_pools.begin(); itr < dbb->dbb_pools.end(); ++itr)
	{
		JrdMemoryPool* pool = *itr;
		if (pool) {
			// walk through all the hunks in the pool
			pool->verify_pool();
		}
	}
}
#endif /* DEV_BUILD */


JrdMemoryPool *JrdMemoryPool::createDbPool(Firebird::MemoryStats &stats) {
	JrdMemoryPool* result = (JrdMemoryPool *)internal_create(
		sizeof(JrdMemoryPool), NULL, stats);
	result->plb_dccs = NULL;
	return result;
}

JrdMemoryPool *JrdMemoryPool::createPool() {
    Database* dbb = GET_DBB();
	fb_assert(dbb);

#ifdef SUPERSERVER
	JrdMemoryPool* result = (JrdMemoryPool *)internal_create(sizeof(JrdMemoryPool),
		dbb->dbb_permanent, dbb->dbb_memory_stats);
#else
	JrdMemoryPool *result = (JrdMemoryPool *)internal_create(sizeof(JrdMemoryPool), dbb->dbb_permanent);
#endif
	result->plb_dccs = NULL;
	dbb->dbb_pools.push(result);
	return result;
}

JrdMemoryPool** JrdMemoryPool::deletePool(JrdMemoryPool* pool) {
	Database* dbb = GET_DBB();
	JrdMemoryPool** rc = 0;
	for (size_t n = 0; n < dbb->dbb_pools.getCount(); ++n)
	{
		if (dbb->dbb_pools[n] == pool)
		{
			rc = dbb->dbb_pools.remove(n);
			break;
		}
	}
	fb_assert(rc);
	MemoryPool::deletePool(pool);
	return rc;
}

void JrdMemoryPool::noDbbDeletePool(JrdMemoryPool* pool) {
	MemoryPool::deletePool(pool);
}


void ALL_print_memory_pool_info(FILE* fptr, Database* databases)
{
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
	Attachment* att;
	int i, j, k;

	/*fprintf(fptr, "\n\tALL_xx block types\n");
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
	fprintf(fptr, "\n");*/

	// TMN: Note. Evil code.
	for (i = 0, dbb = databases; dbb; dbb = dbb->dbb_next, ++i);
	fprintf(fptr, "\tNo of dbbs = %d\n", i);

	for (k = 1, dbb = databases; dbb; dbb = dbb->dbb_next, ++k)
	{
		fprintf(fptr, "\n\t dbb #%d -> %s\n", k, dbb->dbb_filename.c_str());
		j = 0;
		
		size_t itr;
		for (itr = 0; itr < dbb->dbb_pools.getCount(); ++itr)
		{
			if (dbb->dbb_pools[itr]) 
			{
				++j;
			}
		}
		fprintf(fptr, "\t    %s has %d pools", dbb->dbb_filename.c_str(), j);
		for (j = 0, att = dbb->dbb_attachments; att; att = att->att_next)
		{
			j++;
		}
		fprintf(fptr, " and %d attachment(s)\n\n", j);
		for (itr = 0; itr < dbb->dbb_pools.getCount(); ++itr)
		{
			JrdMemoryPool *myPool = dbb->dbb_pools[itr];
			if (myPool) 
			{
				myPool->print_contents(fptr, true);
			}
		}
	}
}

