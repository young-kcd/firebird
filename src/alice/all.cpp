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
//	$Id: all.cpp,v 1.27 2004-08-16 12:28:13 alexpeshkoff Exp $
//

#include "firebird.h"
#include "../jrd/common.h"
#include "../alice/all.h"
#include "../alice/alice.h"
#include "../jrd/thd.h"
#include "../common/classes/alloc.h"


//____________________________________________________________
//  
//		Get rid of everything.
//  

void ALLA_fini(void)
{
	AliceGlobals* tdgbl = AliceGlobals::getSpecific();

	for (AliceGlobals::pool_vec_t::iterator curr = tdgbl->pools.begin();
					curr != tdgbl->pools.end(); ++curr)
	{
		AliceMemoryPool::deletePool(*curr);
		*curr = 0;
	}
	tdgbl->pools.clear();

	tdgbl->setDefaultPool(0);
	tdgbl->ALICE_permanent_pool = 0;
}


//____________________________________________________________
//  
//		Initialize the pool system.
//  

void ALLA_init(void)
{
	AliceGlobals* tdgbl = AliceGlobals::getSpecific();
#ifdef NOT_USED_OR_REPLACED
	tdgbl->ALICE_default_pool = tdgbl->ALICE_permanent_pool =
		AliceMemoryPool::create_new_pool();
#else
	// TMN: John, what pool to use here?
	tdgbl->ALICE_permanent_pool = AliceMemoryPool::createPool();
	tdgbl->setDefaultPool(tdgbl->ALICE_permanent_pool);
#endif
}



#ifdef NOT_USED_OR_REPLACED
void AliceMemoryPool::ALLA_push(blk* object, alice_lls** stack)
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
	AliceGlobals* tdgbl = AliceGlobals::getSpecific();
	AliceMemoryPool* pool = tdgbl->ALICE_default_pool;

	alice_lls* node = pool->lls_cache.newBlock();
	node->lls_object = object;
	node->lls_next = *stack;
	*stack = node;
}


BLK AliceMemoryPool::ALLA_pop(alice_lls** stack)
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
	alice_lls* node = *stack;
	*stack = node->lls_next;
	BLK object = node->lls_object;

	AliceMemoryPool* pool = (AliceMemoryPool*)MemoryPool::blk _pool(node);
	pool->lls_cache.returnBlock(node);

	return object;
}

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

	AliceGlobals* tdgbl = AliceGlobals::getSpecific();

	// TMN: John, is this correct?
    AliceMemoryPool* pool = new(0, parent) AliceMemoryPool(parent);
	AliceGlobals::pool_vec_t::iterator curr;

	for (curr = tdgbl->pools.begin(); curr != tdgbl->pools.end(); ++curr)
	{
		if (!*curr)
		{
			*curr = pool;
			return pool;
		}
	}

	tdgbl->pools.resize(tdgbl->pools.size() + 10);
	for (curr = tdgbl->pools.begin(); curr != tdgbl->pools.end(); ++curr)
	{
		if (!*curr)
		{
			*curr = pool;
			return pool;
		}
	}

	//fb_assert(0);
	//BUGCHECK ("ALLA_fini - finishing before starting");
    return 0;//pool;	// Never reached, but makes the compiler happy.
}
#endif

void AliceMemoryPool::deletePool(AliceMemoryPool* pool) 
{
	AliceGlobals* tdgbl = AliceGlobals::getSpecific();

	AliceGlobals::pool_vec_t::iterator curr;
	for (curr = tdgbl->pools.begin(); curr != tdgbl->pools.end(); ++curr)
	{
		if (*curr == pool)
		{
			*curr = 0;
			break;
		}
	}
//	pool->lls_cache.~BlockCache<alice_lls>();
	MemoryPool::deletePool(pool);
}

