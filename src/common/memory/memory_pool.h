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

#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include "../../include/firebird.h"
#include "../../include/fb_types.h"
#include "../jrd/ib_stdio.h"


FB_DLL_EXPORT class MemoryPool* getDefaultMemoryPool();

/**
	For various reasons Firebird has chosen to do its own heap management.
	The MemoryPool class is the result of that decision.  Basically a memory
	pool object is the same as a single heap.  Objects can be allocated
	out of the heap and returned to the heap.  Just like all requests
	for dynamic memory must reference a heap, all such requests in Firebird
	must reference a memory pool.

	Memory pools are implemented in such a way they never return their memory
	back to the host operating system.  Instead they keep track of that
	memory internally and use it to satisify future allocation requests.

	Every memory pool has no initial pool space.  It grows dynamically as
	more allocation requests are made.  Every time the pool needs more space
	it goes to the operating system and requests another chunk of memory.
	That chunk is usually more than what is needed to satisify the pending
	allocation request.  The minimum requested size can vary from memory
	pool to memory pool and is call the extend size.

	The option also exists to have a pool contact another pool if it needs
	more memory instead of going to the operating system.  This allows the
	creation of pool hierarchies.

	All the memory in the pool can be freed at once which is a speed savings,
	but at the current time the destructors for any objects in the pool
	will NOT get called.  So they need to be explicitly deleted BEFORE the
	pool is released.

	To make it easier to allocate memory from the memory pools a number
	of implementations of the operator new are provided in the file
	common/memory/allocators.h.  Basically, to allocate memory from a pool
	use the following code:

	obj = new(poolReference) Obj;

	If you use the STL you must also use allocators to ensure all the memory
	allocated by the STL ends up in a heap.  The template
	MemoryPool::allocator<T> has been written to be used with the STL objects.
	Sometimes the template types can get out of control when using allocators.
	So there are some convience templates provided in the Firebird namespace
	that mimic the std classes, but use Firebird allocators.

	The details of how the memory is managed (not just how you get the memory
	you need) can be found in the documentation for the FBMemoryPool class.

	If any memory operation fail an exception is raised.
**/
class FB_DLL_EXPORT MemoryPool
{
public:
	MemoryPool(size_t = 0, MemoryPool* = 0);
	virtual ~MemoryPool();

	/// Free all memory from the pool, but leave the pool in a state to
	///  allocate more memory.
	void release_pool(void);

	/// Allocates at least the given number of bytes from the pool and
	///  returns a pointer to the memory.
	void* allocate(size_t, SSHORT = 0);

	/// Deallocates memory that has been allocated from ANY MemoryPool.
	static int deallocate(void*);

	/// Allocate memory directly from the OS
	static void* malloc_from_system(size_t);
	/// Deallocate memory allocated directly from the OS
	static SLONG free_from_system(void* mem);

	/// Pool debugging calls
	void print_contents(IB_FILE*, const char* (*)(int) = 0,
			void (*)(int, void*, IB_FILE*, const char*) = 0);
	/// Verify the structural integrity of the pool
	bool verify_pool(bool = false);

	/// Get the minimum extend size for the pool.
	size_t extendSize();
	/// Set the minimum extend size for the pool
	void setExtendSize(size_t);

	/// Returns the type associated with the allocated memory.
	static SSHORT blk_type(const void* mem);
	/// Returns the pool the memory was allocated from.
	static MemoryPool* blk_pool(const void* mem);

private:
	class FBMemoryPool	*pimpl;
	class MemoryPool	*parent;
	size_t				extend_inc;

	void* allocate_int(size_t, SSHORT);
	friend class FBMemoryPool;
};

#endif	// JRD_MEMORY_POOL_H
