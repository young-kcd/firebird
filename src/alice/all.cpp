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
//	$Id: all.cpp,v 1.3 2001-12-24 02:50:47 tamlin Exp $
//

#include "../alice/all.h"
//#include "firebird.h"
//#include "../jrd/ib_stdio.h"
//#include "../jrd/common.h"
//#include "../jrd/ibsetjmp.h"
#include "../alice/alice.h"
//#include "../alice/all.h"
//#include "../alice/alloc.h"
//#include "../alice/lls.h"
//#include "../alice/all_proto.h"
//#include "../jrd/gds_proto.h"
#include "../jrd/thd_proto.h"
#include "../common/memory/allocators.h"


//____________________________________________________________
//  
//		Get rid of everything.
//  

void ALLA_fini(void)
{
	TGBL tdgbl = GET_THREAD_DATA;

	for(tgbl::pool_vec_t::iterator curr = tdgbl->pools.begin();
					curr != tdgbl->pools.end(); ++curr)
	{
		delete *curr;
		*curr = 0;
	}
	tdgbl->pools.clear();

	tdgbl->ALICE_default_pool = tdgbl->ALICE_permanent_pool = NULL;
}


//____________________________________________________________
//  
//		Initialize the pool system.
//  

void ALLA_init(void)
{
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

#if 0
	tdgbl->ALICE_default_pool = tdgbl->ALICE_permanent_pool =
		AliceMemoryPool::create_new_pool();
#else
	tdgbl->ALICE_default_pool = tdgbl->ALICE_permanent_pool =
		new(*FB_MemoryPool) AliceMemoryPool;
#endif
}



void AliceMemoryPool::ALLA_push(class blk *object, class lls** stack)
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
	class lls* node;
	AliceMemoryPool* pool;

	TGBL tdgbl = GET_THREAD_DATA;
	pool = tdgbl->ALICE_default_pool;

	node = pool->lls_cache.newBlock();
	node->lls_object = object;
	node->lls_next = *stack;
	*stack = node;
}


BLK AliceMemoryPool::ALLA_pop(register LLS *stack)
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
	register LLS node;
	AliceMemoryPool* pool;
	BLK object;

	node = *stack;
	*stack = node->lls_next;
	object = node->lls_object;

	pool = (AliceMemoryPool*)MemoryPool::blk_pool(node);
	pool->lls_cache.returnBlock(node);

	return object;
}

#if 0
AliceMemoryPool* AliceMemoryPool::create_new_pool(MemoryPool* parent)
{
/**************************************
 *
 *  A L L _ p o o l
 *
 **************************************
 *
 * Functional description
 *  Allocate a new pool.
 *
 **************************************/

	TGBL tdgbl = GET_THREAD_DATA;

	// TMN: John, is this correct?
    AliceMemoryPool* pool = new(0, parent) AliceMemoryPool(parent);
	tgbl::pool_vec_t::iterator curr;

	for(curr = tdgbl->pools.begin(); curr != tdgbl->pools.end(); ++curr)
	{
		if (!*curr)
		{
			*curr = pool;
			return pool;
		}
	}

	tdgbl->pools.resize(tdgbl->pools.size() + 10);
	for(curr = tdgbl->pools.begin(); curr != tdgbl->pools.end(); ++curr)
	{
		if (!*curr)
		{
			*curr = pool;
			return pool;
		}
	}

	//assert(0);
	//BUGCHECK ("ALLA_fini - finishing before starting");
    return 0;//pool;	// Never reached, but makes the compiler happy.
}
#endif

AliceMemoryPool::~AliceMemoryPool()
{
	TGBL tdgbl = GET_THREAD_DATA;

	tgbl::pool_vec_t::iterator curr;
	for(curr = tdgbl->pools.begin(); curr != tdgbl->pools.end(); ++curr)
	{
		if (*curr == this)
		{
			*curr = 0;
			return;
		}
	}
}

