/*
 *      PROGRAM:        Client/Server Common Code
 *      MODULE:         memory_pool.h
 *      DESCRIPTION:    Memory Pool Manager
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
 * 2001.11.29 John Bellardo: Reworked all routines to create the MemoryPool
 *   class as part of the C++ conversion.  Additionally the class now handles
 *   generic memory allocations instead of typed-only allocations.
 */

#ifndef COMMON_ALLOCATORS_H
#define COMMON_ALLOCATORS_H

#include "../common/memory/memory_pool.h"
#include "../jrd/gds_proto.h"
#include <vector>

extern "C" {
#ifdef DEBUG_GDS_ALLOC
void* API_ROUTINE gds__alloc_debug(SLONG size_request,
                                   TEXT* filename,
                                   ULONG lineno);
#else
void* API_ROUTINE gds__alloc(SLONG size_request);
#endif
extern ULONG API_ROUTINE gds__free(void* blk);
};

void* operator new(size_t);
void* operator new[](size_t);

FB_DLL_EXPORT void* operator new(size_t, MemoryPool&);
FB_DLL_EXPORT void  operator delete(void* mem, MemoryPool&);
FB_DLL_EXPORT void* operator new[](size_t s, MemoryPool&);
FB_DLL_EXPORT void  operator delete[](void* mem, MemoryPool&);

FB_DLL_EXPORT void* operator new(size_t, MemoryPool*);
FB_DLL_EXPORT void* operator new[](size_t s, MemoryPool*);

void operator delete(void* mem);
void operator delete[](void* mem);

/**
	This is the allocator template provided to be used with the STL.
	Since the STL is the client of this class look to its documentation
	to determine what the individual functions and typedefs do.

	In order to use the allocator class you need to instansiate the
	C++ container template with the allocator.  For example if you
	want to use a std::vector<int> the declaration would be:

	std::vector<int, MemoryPool::allocator<int> >

	The allocator, by default, allocates all memory from the process
	wide pool FB_MemoryPool.  Typically this is NOT the behavior you
	want.  Selection of the correct pool to allocate your memory from is
	important.  If you select a pool to far down in (a statement pool,
	for example) you memory may be freed before you are done with it.
	On the other hand if you always use the global pool you will
	either leak memory or have to make sure you always delete the objects
	you create.

	If you decide to allocate the memory from a pool other than the global
	pool you need to pass an allocator object to the constructor for
	the STL object.  For example:

	std::vector<int, MemoryPool::allocator<int> > vec(MemoryPool::allocator<int>(poolRef, type));
	The type is an optional parameter that defaults to 0.
**/
namespace Firebird
{
	template <class T>
	class allocator
	{
		public:
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef T			value_type;
	
		allocator(MemoryPool& p, SSHORT t = 0) : pool(&p), type(t) {}
		allocator(MemoryPool *p = getDefaultMemoryPool(), SSHORT t = 0) : pool(p), type(t) {}
	
		template <class DST>
		allocator(const allocator<DST> &alloc)
		: pool(alloc.getPool()), type(alloc.getType()) { }

		pointer allocate(size_type s, const void * = 0)
			{ return (pointer) (pool ? pool->allocate(sizeof(T) * s) : gds__alloc(sizeof(T)*s)); }
		void deallocate(pointer p, size_type s)
			{ if (pool) MemoryPool::deallocate(p); else gds__free(p); }
		void construct(pointer p, const T& v) { new(p) T(v); }
		void destroy(pointer p) { p->~T(); }
	
		size_type max_size() const { return (size_type)-1 / sizeof(T); }
	
		pointer address(reference X) const {return &X; }
		const_pointer address(const_reference X) const {return &X; }
	
		template <class _Tp1> struct rebind {
			typedef Firebird::allocator<_Tp1> other;
		};


		bool operator==(const allocator<T>& rhs) const
		{
			return pool == rhs.pool && type == rhs.type;
		}

		MemoryPool *getPool() const { return pool; }
		SSHORT getType() const { return type; }
	
	private:
		MemoryPool *pool;
		SSHORT type;
	};

};

#endif	// COMMON_ALLOCATORS_H
