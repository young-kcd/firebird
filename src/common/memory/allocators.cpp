/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		memory_pool.cpp
 *	DESCRIPTION:	Memory Pool Manager
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
 *
 * 2001.11.29 John Bellardo: Reworked all routines to create the MemoryPool
 *   class as part of the C++ conversion.  Additionally the class now handles 
 *   generic memory allocations instead of typed-only allocations.
 */

#include "../../common/memory/allocators.h"
#include "../../include/fb_exception.h"
#include "../../jrd/gds.h"
#include "../../jrd/gdsassert.h"
#include <new>


static MemoryPool* FB_MemoryPool = 0;

MemoryPool* getDefaultMemoryPool()
{
	return FB_MemoryPool;
}


class InitMemoryPool
{
public:
	InitMemoryPool() { loadPool(); }
	static void loadPool()
		{
			if (!FB_MemoryPool)
				FB_MemoryPool = new BootstrapMemoryPool;
		}
private:
	class BootstrapMemoryPool : public MemoryPool
	{
	public:
		BootstrapMemoryPool() : MemoryPool(102400) {}
		void* operator new(size_t s)
		{
			void* mem = malloc(s);
			if (!mem) {
				throw std::bad_alloc();
			}
			return mem;
		}
		void operator delete(void *mem) { free(mem); }
	};
};


static InitMemoryPool poolLoader;
static int badNewCount = 0;

extern "C" {

#ifdef DEBUG_GDS_ALLOC

void* API_ROUTINE gds__alloc_debug(SLONG size_request,
                                   TEXT* filename,
                                   ULONG lineno)
#else

void* API_ROUTINE gds__alloc(SLONG size_request)
#endif
{
	try
	{
		poolLoader.loadPool();
		return FB_MemoryPool->allocate(size_request);
	} catch(...) {}
	
	return 0;
}


ULONG API_ROUTINE gds__free(void* blk)
{
	if (blk)
	{
		try
		{
			poolLoader.loadPool();
			return FB_MemoryPool->deallocate(blk);
		} catch(...) {}
	}
	
	return 0;
}

}	// extern "C"


// Our implementations for the global operators new/delete.

/** operator new[] implementation to trap all calls to the default operator
	new.  You MUST allocate all memory from a pool.  Since there is no way
	to specifiy a pool to the default operator new it just throws an assert.
	If you are using std::* classes you need to use allocators.
**/
void* operator new(size_t s)
{
	// We MUST have a pool to allocate from, otherwise there is
	// an error.
	// TMN: No it's not. The Dinkum C++ library as shipped with
	// MSVC (5 & 6) allocates a few small objects from the global heap.
#if defined(DEV_BUILD)
	if (++badNewCount > 1)
	{
		printf("You MUST allocate all memory from a pool.  Don't use the default global new().\n");
#if !defined(_MSC_VER)
        //		throw std::bad_alloc();
#endif
	}
#endif	// DEV_BUILD
	poolLoader.loadPool();
	return FB_MemoryPool->allocate(s);
}

/** operator new[] implementation to trap all calls to the default operator
	new.  You MUST allocate all memory from a pool.  Since there is no way
	to specifiy a pool to the default operator new it just throws an assert.
	If you are using std::* classes you need to use allocators.
**/
void* operator new[](size_t s)
{
	// We MUST have a pool to allocate from, otherwise there is
	// an error
#ifdef DEV_BUILD
	if (++badNewCount > 1)
	{
		printf("You MUST allocate all memory from a pool.  Don't use the default global new[]().\n");
        //		throw std::bad_alloc();
	}
#endif
	poolLoader.loadPool();
	return FB_MemoryPool->allocate(s);
}

/**	Generic operator new to allocate memory from a given pool.  Works with
	all objects that don't define their own operator new.
**/
void* operator new(size_t s, MemoryPool& p)
{
	return p.allocate(s, 0);
}

/**	operator delete to handle exceptions thrown while contructing object with
	our custom operator new.
**/
void operator delete(void* mem, MemoryPool& p)
{
	if (mem) {
		MemoryPool::deallocate(mem);
	}
}

/** standard operator delete called to free object without their own
	implementation of operator delete.
**/
void operator delete(void* mem)
{
	if (mem) {
		MemoryPool::deallocate(mem);
	}
}

// no implmentation of these two functions in dev build to force link
// errors if they are used.
#ifndef DEV_BUILD
/**	This operator new traps the standard libraries placement new.  It simply
	pops an assert to prevent inadvertant use.  Use the reference versions
	of operator new() instead.
**/
void* operator new[](size_t s, MemoryPool* p)
{
	assert(0);
	return 0;
}

/**	This operator new traps the standard libraries placement new.  It simply
	pops an assert to prevent inadvertant use.  Use the reference versions
	of operator new() instead.
**/
void* operator new(size_t s, MemoryPool* p)
{
	assert(0);
	return 0;
}
#endif

/**	Generic operator new to allocate memory from a given pool.  Works with
	all objects that don't define their own operator new.
**/
void* operator new[](size_t s, MemoryPool& p)
{
	return p.allocate(s, 0);
}

/**	operator delete[] to handle exceptions thrown while contructing object with
	our custom operator new.
**/
void operator delete[](void* mem, MemoryPool& p)
{
	if (mem) {
		MemoryPool::deallocate(mem);
	}
}

/** standard operator delete[] called to free object without their own
	implementation of operator delete.
**/
void operator delete[](void* mem)
{
	if (mem) {
		MemoryPool::deallocate(mem);
	}
}


