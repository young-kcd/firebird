/*
 *	PROGRAM:	Alice
 *	MODULE:		all.h
 *	DESCRIPTION:	Data allocation declarations
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

#ifndef _ALICE_ALL_H_
#define _ALICE_ALL_H_

#include "../common/memory/memory_pool.h"
#include "../jrd/block_cache.h"
#include "../alice/lls.h"

void		ALLA_init();				/* initialize pool system */
void		ALLA_fini();				/* get rid of everything */

class AliceMemoryPool : public MemoryPool
{
public:
//	static AliceMemoryPool *create_new_pool(MemoryPool* = 0);
//	AliceMemoryPool(MemoryPool* p = 0)
//	:	MemoryPool(0, p),
//		lls_cache(*this)
//	{}
	AliceMemoryPool(int extSize = 0, MemoryPool* p = getDefaultMemoryPool())
	:	MemoryPool(extSize, p),
		lls_cache(*this)
	{
	}


	~AliceMemoryPool();

	static class blk* ALLA_pop(class lls**);
	static void ALLA_push(class blk*, class lls**);

private:
	BlockCache<class lls> lls_cache;  /* Was plb_lls */
};

#endif /* _ALICE_ALL_H_ */