/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		all.h
 *	DESCRIPTION:	Allocator prototypes
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

#ifndef DSQL_ALL_H
#define DSQL_ALL_H

#include "../common/classes/alloc.h"
#include "../jrd/block_cache.h"

class DsqlMemoryPool : public MemoryPool
{
protected:
	// Dummy constructor and destructor. Should never be called
	DsqlMemoryPool() : MemoryPool(NULL, NULL), lls_cache(*this) {}
	~DsqlMemoryPool() {}	
public:
	static DsqlMemoryPool *createPool();
	
	static void deletePool(DsqlMemoryPool* pool);

	static class blk* ALLD_pop(class dsql_lls**);
	static void ALLD_push(class blk*, class dsql_lls**);

private:
	BlockCache<class dsql_lls> lls_cache;  /* Was plb_lls */
};

extern DsqlMemoryPool *DSQL_permanent_pool;

#endif	// DSQL_ALL_H
