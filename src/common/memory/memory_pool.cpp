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

#ifdef _MSC_VER
// The following warning disabler must come before any includes.
#pragma warning(disable: 4786)	// debug identifier truncated
// Microsoft... sigh...
#define for if (0) {} else for
#endif

#include "../../include/fb_exception.h"
#include "../../jrd/smp_impl.h"
#include "../../common/memory/memory_pool.h"
#include "gen/codes.h"

#include "../jrd/err_proto.h"
#include "../jrd/smp_impl.h"
#include "../jrd/gdsassert.h"
#include "../jrd/gds.h"

#ifdef UNIX
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#endif

#ifdef WIN_NT
#include <windows.h>
#endif

#include <map>
#include <new>


// This next variable enables _very frequent_ pool integrity checks.  These checks
// include red zone checks if red zones have been enabled.
//
// This is a debug option ONLY and SHOULD NEVER be enabled in a production build
// because of its significant performance penality.
static const bool ENABLE_FREQUENT_VERIFICATION = false;



// These next variables control the Red Zone debugging offered by the memory pool
static const bool ENABLE_RED_ZONES     = false;	// set to false to disable the red zones
static const int  RED_ZONE_FILL        = 0xFD;
static const int  BEFORE_RED_ZONE_SIZE = 3;		// The size of the red zone BEFORE the memory,
												//  in ALLOCATION UNITS! Does NOT need to be set to
												//  0 if red zones are disabled.
static const int AFTER_RED_ZONE_SIZE = 3;		// The size of the red zone AFTER the memory,
												//  in ALLOCATION UNITS!  Does NOT need to be set to
												//  0 if red zones are disabled.

// Helper function to reduce code size, since many compilers
// generate quite a bit of code at the point of the throw.
static void pool_out_of_memory()
{
	throw std::bad_alloc();
}


/* globalAllocLock is a static global instead of a static member of the
 * class because it is an implementation detail of the system allocation
 * routines.  Making it a member of the class requires the inclusion of
 * our implementation dependant synchronization header files in the
 * otherwise generic memory_pool.h file.
 */
static V4Mutex globalAllocLock;


/**
	This is the "private" implementation used by the MemoryPool class.
	This class does all the real work.  Providing it separate from the
	MemoryPool class increases compile time and adds a level of abstraction
	between the users of the memory management code and those who actually
	worry about writing it.  This part of the documentation is targeted at
	the later group, those who actually have to maintain the memory pool code.
	Note that all the functions in FBMemoryPool are private.  That is because
	no one should be calling them.  All access should go through MemoryPool.

	A pool is composed of zero of more segments.  Segments are kept in a linked
	link in no particular order.  Each segment is the memory returned by a
	single request for more memory (consecutive segments DO NOT get merged).
	Each segment has a header.  The header is FBMemoryPool::Segment.

	Within a segment there is a free list.  The free list nodes are stored 
	directly in the unused pool memory.  The free list nodes are
	FBMemoryPool::FreeBlock.  It is assumed the free list node is at the
	beginning of every free block of memory.  The node contains the total
	length of the free block and a pointer to the next free block.  All
	lengths stored in the memory pool are in allocation units, not in bytes.
	The enum ALLOC_ALIGN_SHIFT allows the use of the bit shift operator to
	convert between allocation units and actual bytes.

	When an object is allocated from the pool a header is added before
	the object.  The minimum size of the header is ALLOC_ALIGNMENT, or
	currently 8 bytes.  In this header is a 4 byte pointer to the segment
	the object is in (for quick deallocation), a 2 byte signed int type,
	and a 2 byte signed int length (in allocation units, remember?).
	If the length is negative it means the object is too long to represent
	in 2 signed bytes.  In that case an additional header is placed
	BEFORE the first header.  The additional header has to be at least
	ALLOC_ALIGNMENT bytes (to preserve the alignment of the allocated object).
	It contains a 4 byte unsigned int that is the true length of the object
	(in allocation units).  This allows us to store objects as large as
	32GB in the pool, while keeping the header overhead to a minimum.

	FBMemoryPool::SmallHeader is the header present on all objects, and
	FBMemoryPool::ExtendedHeader is the header that is added if the
	object size excedes what will fit in 2 signed bytes.  An optimization
	has been made to allow the full header (4 byte length, 4 byte pointer,
	2 byte type) if that option takes no more allocation units than the
	small and extended header options.  Helper functions have been provided
	to make the SmallHeader/ExtendedHeader/FullHeader difference transparent.

	A FBMemoryPool object can be allocated on the stack or from the heap.
	The stack case is boring, but if a FBMemoryPool object is allocated
	from the heap, it actually gets allocated out of its own pool.
	To handle this the segment containing the pool object gets special
	treatment to ensure it isn't freed with the rest of the pool

	There are a number of debug options provided on the pool.  One of them
	is red zones around the allocated objects.  A number of constants at
	the beginning of this file control the behavior of the red zones.
	Red zones should not be enabled in a release version of Firebird.
	There is also a function that does pool validation.  It has the ability
	to walk through each segment and identify the free blocks and allocated
	objects based on the headers.  If Firebird is have memory corruption
	or other such phantom memory problems this is a useful tool to have.
**/
class FBMemoryPool
{
private:
	class pool_locker
	{
	public:
		explicit pool_locker(SmpLock& l) : lock(l)
			{ l.aquire(); }
		~pool_locker()
			{ lock.release(); }
	private:
		pool_locker(const pool_locker&);	// no impl.
		void operator=(const pool_locker&);	// no impl.
		SmpLock& lock;
	};

	struct Segment
	{
		Segment()
		:	pool(0),
			next(0),
			start(0),
			end(0),
			overhead(0),
			allocated(0),
			firstFree(0)
		{
		}

		// The memory management structure that tracks free blocks in
		// the pool.
		struct FreeBlock
		{
			size_t		length;
			FreeBlock*	next;
		};

		FBMemoryPool*	pool;
		Segment*	next;
		void*		start;
		void*		end;
		size_t		overhead;
		size_t		allocated;
		FreeBlock*	firstFree;
	};

	// The base class for all memory management blocks in the
	// pool.  This class also tracks allocated blocks in the
	// memory pool.
	struct SmallHeader
	{
		Segment*	segment;
		SSHORT		type;
		SSHORT		length;
	};
	struct ExtendedHeader
	{
		size_t		length;
	};
	struct FullHeader
	{
		Segment*	segment;
		SSHORT		type;
		size_t		length;
	};

	friend class MemoryPool;
	FBMemoryPool(MemoryPool*, Segment*, size_t, MemoryPool*);
	virtual ~FBMemoryPool();

	void* operator new(size_t, MemoryPool*, Segment*&, size_t);
	void operator delete(void*, MemoryPool*, Segment*&, size_t);
	void operator delete(void*);

	void release_pool(void);
	void* allocate(size_t, SSHORT);
	int deallocate(void*);

	void verify_pool(bool);

	size_t extendSize();
	void setExtendSize(size_t s);

	static SSHORT blk_type(const void* mem);
	static MemoryPool* blk_pool(const void* mem);
	static int blk_length(const void* mem);


	void LOCK_POOL() { lock.aquire(); }
	void UNLOCK_POOL() { lock.release(); }

	static void print_pool_callback(void * mem, int type, void *cook);
	void print_pool(IB_FILE *, const char *(*)(int) = 0,
				void (*)(int, void*, IB_FILE*, const char*) = 0);
	void walk_pool(void (*)(void *, int, void*) = 0, void *cookie = 0);

	void extend_pool(size_t);

	/// Allocate memory from the system using malloc
	static void* malloc_alloc(size_t);
	/// Deallocate memory allocated using ALL_malloc_alloc
	static SLONG malloc_free(void*);
	/// Allocate memory by mapping in unused address space
	static void* mmap_alloc(size_t);
	/// Deallocate memory allocated using ALL_mmap_alloc
	static SLONG mmap_free(void*);

	void check_red_zone(void* obj);


	static void read_header(const void*, FullHeader*);
	static void write_header(void*, const FullHeader&);
	static size_t compute_header_units(size_t);
	int determine_header_size(const void* , Segment*);
	static Segment* findSegment(const void*);

	// butt ugly, but unfortunately forced by the inferior Microsoft VC++
	enum {
		ALLOC_ALIGNMENT = 8,
		ALLOC_ALIGN_SHIFT = 3,
		SMALL_HEADER_ALLOC_UNITS =
			(sizeof(SmallHeader) + ALLOC_ALIGNMENT - 1) / ALLOC_ALIGNMENT,
		EXTENDED_HEADER_ALLOC_UNITS =
			(sizeof(ExtendedHeader) + ALLOC_ALIGNMENT - 1)/ ALLOC_ALIGNMENT,
		FULL_HEADER_ALLOC_UNITS =
			(sizeof(FullHeader) + ALLOC_ALIGNMENT - 1) / ALLOC_ALIGNMENT,
		SEGMENT_ALLOC_UNITS =
			(sizeof(Segment) + ALLOC_ALIGNMENT - 1) / ALLOC_ALIGNMENT,
		MIN_ALLOCATION	= 1024 / ALLOC_ALIGNMENT
                             /* Minimum allocation from operating system */
	};

	Segment* firstSegment;		// The segment list
	Segment* poolSegment;		// The segment containing the pool
	MemoryPool* parentPool;
	MemoryPool *pimplParent;
	V4Mutex lock;
	// to keep track of block types
	typedef std::map<int, int> pool_map_t;
	pool_map_t*	blk_type_count;
	int extend_increment;
};


//////////////////////////////////////////////////////////////////
// TMN: FIXFIX
// This is currently NEEDED, since the BUGCHECK macro evaluates
// to ERR_Bugcheck, and that's a server only function!

#pragma FB_COMPILER_MESSAGE("TMN: FIXFIX?")
#if 0
#if defined(REQUESTER) || defined(SUPERCLIENT)
extern "C" {
static void ERR_bugcheck(int number)
{
	// What to do?
	Firebird::status_exception::raise(number);
}
}
#endif
#endif	// 0

// currently unconditional override
#undef BUGCHECK
#define BUGCHECK(n) private_ERR_bugcheck((n))

static void private_ERR_bugcheck(int number)
{
	Firebird::status_exception::raise(number);
}

// TMN: FIXFIX
//////////////////////////////////////////////////////////////////




/* ***********************************************
 *
 * We start the show with the functions to dispatch
 *  the MemoryPool functions to FBMemoryPool.
 *  This is a pretty basic pimpl wrapper with a bunch of
 *  doxygen comments.
 *
 ************************************************/

/** Constructor for MemoryPool.  There are two arguments available.
	The first argument is the minimum allocation size every time the
	pool needs to expand.  The second argument is a pointer to the
	parent pool for this pool.  If no parent is provided all memory is
	allocated from the host operating system.  Refer to the detailed
	description of a MemoryPool for more information.
 **/
MemoryPool::MemoryPool(size_t extS, MemoryPool *par)
	:
		pimpl(0),
		parent(par),
		extend_inc(extS)
{
}

/** The destructor releases all resources associated with the pool
	(including all allocated objects in the pool) when the MemoryPool object
	is deleted.
**/
MemoryPool::~MemoryPool()
{
	if (pimpl)
	{
		delete pimpl;
		pimpl = 0;
	}
}

/** Free all memory from the pool.  This causes the memory for all objects
	in the pool to be freed.  This function does NOT call the destructor
	for objects that have them, so all objects that need to have their
	destructor called must first be explicitly deleted.  Freeing all the
	memory from the pool at once does have performance advantages over
	freeing the one at a time.  This function leaves the pool in a
	state that allows the pool to be used further.
**/
void MemoryPool::release_pool(void)
{
	if (pimpl)
	{
		pimpl->release_pool();
	}
}

/**	Allocate the given amount of memory from the MemoryPool.  This function
	returns a pointer to the allocated memory.  The memory is guarenteed
	to be at least as big as the size parameter.  Negative sizes are invalid.
	Additionally the allocated memory is tagged with the given type, if any.
	The type can later be reterived with the MemoryPool:blk_type function.
	All negitive types are reserved for internal pool use and will be converted
	to 0 if they are passed into allocate.  The type 0 is used to indicate
	an unknown type.  Other than that they type can be any value that fits in
	a signed short.
**/
void *MemoryPool::allocate(size_t size, SSHORT type)
{
	if (type < 0)
		type = 0;
	if (!pimpl)
	{
		FBMemoryPool::Segment *seg;
		pimpl = new(parent, seg, extend_inc)
				FBMemoryPool(parent, seg, extend_inc, this);
	}
	return pimpl->allocate(size, type);
}

void *MemoryPool::allocate_int(size_t size, SSHORT type)
{
	if (!pimpl)
	{
		FBMemoryPool::Segment *seg;
		pimpl = new(parent, seg, extend_inc)
				FBMemoryPool(parent, seg, extend_inc, this);
	}
	return pimpl->allocate(size, type);
}

/**	Deallocate memory that has been allocated from any MemoryPool.  This
	function deallocates the memory, but it doesn't call the destructor
	for the object.  It returns the size of the object that was freed.
	This size doesn't include any internal allocation overhead necessary
	for the object.
**/
int MemoryPool::deallocate(void *mem)
{
	MemoryPool* pool = FBMemoryPool::blk_pool(mem);
	assert(pool);
	if (!pool || !pool->pimpl) {
		Firebird::memory_corrupt::raise();
	}
	return pool->pimpl->deallocate(mem);
}

/**	Allocate memory directly from the host operating system.  Memory
	allocated by this function will NOT be deallocated automatically
	when a pool gets released.  It MUST be deallocated with the
	function MemoryPool::free_from_system.
**/
void *MemoryPool::malloc_from_system(size_t size)
{
	/* I don't know why we us different allocation functions depending
	 * on a SS/CS build, but that was the way in FB1 so it is preserved
	 * here for the time being  -- JMB
	 */
#if 0	// TMN: I disabled this to use the same allocation for everything
#ifdef SUPERSERVER
		return FBMemoryPool::mmap_alloc(size);
#else
		return FBMemoryPool::malloc_alloc(size);
#endif
#else	// 0
		return FBMemoryPool::malloc_alloc(size);
#endif	// 0
}

/**	Deallocate memory from the host operating system.  This function
	MUST be called to deallocate any memory obtained from a call to
	MemoryPool::malloc_from_system.  It returns the number of bytes
	of memory returned to the host operating system.
**/
SLONG MemoryPool::free_from_system(void *mem)
{
	// See comment in malloc_from_system for a clue:
#if 0	// TMN: Me too
#ifdef SUPERSERVER 
		return FBMemoryPool::mmap_free(mem);
#else
		return FBMemoryPool::malloc_free(mem);
#endif
#else	// 0
		return FBMemoryPool::malloc_free(mem);
#endif	// 0
}

/**	This is a debugging aid function.  It walks the memory pool, in its
	entirety, and prints out each object in the pool.  In addition it
	prints out some summary statistics about the pool.  The print_contents
	function is type aware.  The first parameter to the function is the
	IB_FILE* to print the contents to.  The IB_FILE* for standard out
	on most systems is "1".

	It prints out the type number for each object.
	All of the pools internal types (negitive values) are translated into
	a string description of the type so you will never see those values.
	Additionally the value 0 is translated to the string "Unknown".
	The second parameter is a pointer to a function that maps type numbers
	to string.  This allows you to provide the output strings for engine
	objects.  Currently the mapping function is defined in jrd/dbt.cpp.
	If the mapping function doesn't return a string or the function doesn't
	exist the type number fill be printed.

	The third parameter is a pointer to a function that can print out the
	contents of a given memory type.  As there are many many objects allocated
	in Firebird this option is of little value unless you need to do some
	very nasty debugging.  The arguments that are passed to the function are:
	the type, the memory pointer, the IB_FILE* to print to, and a string
	to print before any line of output as an indentation.
**/
void MemoryPool::print_contents(IB_FILE *outp,
			const char* (*typeMappingFunc)(int),
            void (*contentFunc)(int, void*, IB_FILE*, const char*))
{
	if (!pimpl)
		return;
	pimpl->print_pool(outp, typeMappingFunc, contentFunc);
}

/**	This is another memory debugging aid.  It verifies the structural
	integrity of the pool's internal data structures.  There are two types
	of verification, fast and complete.  As its name implies the fast check
	doesn't take very long, but it only checks the pools data structures.
	The full check can take significantly longer, but will verify that each
	object in the pool still appears valid.  Additionally if the red zone
	memory debugging has been enabled the full check will verify the red zone
	of all objects in the pool.  It makes sense to do a fast check every
	once in a while for a debug build, and use full checks to aid in debugging
	a particular problem but don't leave the full checks in the main line
	debug builds.  For performance reasons no checks should be done in
	production builds.

	The parameter to the function determines the type of verification.  Pass
	in 'true' for a full verification and 'false' for a fast one.  The
	argument value defaults to 'false' if you don't specify it.
	
	The function will return 'true' if the pool verifies.  Otherwise it will throw an
	exception.
**/
bool MemoryPool::verify_pool(bool full)
{
	if (pimpl)
		pimpl->verify_pool(full);
	return true;
}

/**	An accessor method for the minimum grow size of the pool.  This value
	can be set in the constructor to MemoryPool and can be varied dynamically
	at runtime as needed by the setExtendSize function.
**/
size_t MemoryPool::extendSize()
{
	return extend_inc;
}

/**	This function allows the dynamic changing of the minimum grow size for
	the pool.
**/
void MemoryPool::setExtendSize(size_t s)
{
	extend_inc = s;
	if (pimpl)
		pimpl->setExtendSize(s);
}

/// Returns the type associated with the allocated memory.
SSHORT MemoryPool::blk_type(const void *mem)
{
	return FBMemoryPool::blk_type(mem);
}

/// Returns the pool the memory was allocated from.
MemoryPool *MemoryPool::blk_pool(const void* mem)
{
	return FBMemoryPool::blk_pool(mem);
}


//#######################################################################
//#######################################################################
//#######################################################################

/* ***********************************************
 *
 * Now we get into the guts of the FBMemoryPool implementation.
 *
 ************************************************/

/**	Our bland constructor.  One thing to note, though, is that it takes
	a pointer to the Segment allocated in operator new() that has the
	pool memory already allocated out of it.
**/
FBMemoryPool::FBMemoryPool(MemoryPool* p, Segment* seg, size_t extInc,
					MemoryPool *pP)
	:	firstSegment(seg),
		poolSegment(seg),
		parentPool(p),
		pimplParent(pP),
		blk_type_count(0),
		extend_increment( (extInc + ALLOC_ALIGNMENT - 1) << ALLOC_ALIGN_SHIFT )
{
	if (extend_increment < MIN_ALLOCATION)
		extend_increment = MIN_ALLOCATION;
}

/**	Deallocate all the memory associated with this pool, with the exception
	of the pool object, of course!  The pool segment gets released in
	the operator delete.
**/
FBMemoryPool::~FBMemoryPool()
{
	release_pool();
}


/**	The actual code that allocated a block of memory out of the pool.
	The allocation policy is best fit, which usually requires searching
	all the unused memory in the pool.  A different policy might result in
	more fragmentation, but would see a speed improvement.  See the description
	of the FBMemoryPool class for a discussion on the different data structures
	used to manage the pool.  This function is thread safe.
**/
void* FBMemoryPool::allocate(size_t size, SSHORT type)
{
	Segment::FreeBlock** best;
	Segment::FreeBlock** ptr;
	size_t		best_tail;
	size_t		tail;
	size_t		units;
	size_t		headerUnits;
	void*		obj;
	Segment*	bestSeg;

	// Do we need to verify the pool now?
	if (ENABLE_FREQUENT_VERIFICATION)
	{
		verify_pool(true);
	}

	// Compute the allocation units needed, taking into account alignment.
	units       = ( (size + ALLOC_ALIGNMENT - 1) >> ALLOC_ALIGN_SHIFT);
	headerUnits = compute_header_units(units);
	units += headerUnits;
	
	if (ENABLE_RED_ZONES)
	{
		units += BEFORE_RED_ZONE_SIZE + AFTER_RED_ZONE_SIZE;
	}

/* Find best fit.  Best fit is defined to be the free block of shortest
   tail.  If there isn't a fit, extend the pool and try, try again. */
	pool_locker lock_holder(lock);

	while (true)
	{
		best = NULL;
		best_tail = MAX_SLONG;
		for(Segment* seg = firstSegment; seg; seg = seg->next)
		{
			assert(seg != (void*)1);
			for (ptr = &seg->firstFree; *ptr; ptr = &(*ptr)->next)
			{
				assert(*ptr != (void*)1);
		        if ((char*)(*ptr) >= (char*)(*ptr)->next && (*ptr)->next)
				{
					assert((*ptr)->next != (void*)1);
					BUGCHECK(152);
					/* msg 152 memory pool free list is invalid */
				}
				if ((tail = (*ptr)->length - units) >= 0 &&
                                       tail < best_tail)
				{
					best = ptr;
					bestSeg = seg;
					best_tail = tail;
					if (tail == 0) {
						break;
					}
				}
			}
			if (best && tail == 0) {
				break;
			}
		}
		if (best) {
			break;
		}

		// We don't have enough free memory, lets extend the pool
		extend_pool(units);
	}

/* We've got our free block.  If there's enough left of the free block
   after taking out our block, chop out out block.  If not, allocate
   the entire free block as our block (a little extra won't hurt). */
	
	if (best_tail >= 1 + compute_header_units(1)) {
		obj = ((UCHAR*)*best) +
                             ( (best_tail + headerUnits) << ALLOC_ALIGN_SHIFT);
		(*best)->length = best_tail;
	}
	else {
                obj = ((UCHAR*)*best) + (headerUnits << ALLOC_ALIGN_SHIFT);
		*best = (*best)->next;
		units += best_tail;
		assert(*best != (void*)1);
	}

	// Write out the block header
	FullHeader	header;
	header.length	= units - headerUnits;
	header.type		= type;
	header.segment	= bestSeg;
	bestSeg->overhead += headerUnits;
	bestSeg->allocated += units - headerUnits;
	if (ENABLE_RED_ZONES)
	{
		header.length -= BEFORE_RED_ZONE_SIZE + AFTER_RED_ZONE_SIZE;
		bestSeg->overhead += BEFORE_RED_ZONE_SIZE + AFTER_RED_ZONE_SIZE;
		bestSeg->allocated -= BEFORE_RED_ZONE_SIZE + AFTER_RED_ZONE_SIZE;
		memset((UCHAR*)obj - (headerUnits << ALLOC_ALIGN_SHIFT), RED_ZONE_FILL,
					units << ALLOC_ALIGN_SHIFT);
		obj = (UCHAR*)obj + (BEFORE_RED_ZONE_SIZE << ALLOC_ALIGN_SHIFT);
	}
	write_header(obj, header);

	if (blk_type_count) {
		++(*blk_type_count)[type];
	}

	memset((SLONG*)obj, 0, header.length << ALLOC_ALIGN_SHIFT);
	
	return obj;
}

/**	The implementation of operator new() for FBMemoryPool.  The function
	allocates the first segment of the pool.  It stashes the pointer to the
	parent pool before the segment to allow the segment to be deallocated
	correctly in the operator delete.  It initializes the segment header
	and puts the pointer to that header into the out parameter segRet.
	It then allocates s bytes from the segment (presumably for the
	FBMemoryPool, since this is the operator new in the FBMemoryPool
	class) taking into account any red zones.  This memory is taken
	immediately following the segment header.  This is important as some of
	the other pool code assumes this.  After everything is initialized
	a pointer to the newly allocated FBMemoryPool is returned.
**/
void* FBMemoryPool::operator new(size_t		s,
								MemoryPool*	p,
								Segment*&		segRet,
								size_t			extInc)
{
	const size_t s_units = (s + ALLOC_ALIGNMENT - 1) >> ALLOC_ALIGN_SHIFT;

	size_t size = 1 + s_units + compute_header_units(s_units)
								+ SEGMENT_ALLOC_UNITS;
	// We assume there will be free space left in the segment, so we need to
	// add it now.  1 unit is a waste because the object header takes that much
	// room.  So we need to add at least 2 units to be useful.
	size += 2;
	
	// Make sure we put a red zone around the pool.  We aren't very worried about the pool
	// code, but it prevents us from adding a bunch of special cases to handle mixed
	// red zone / non red zone objects in the pool.
	if (ENABLE_RED_ZONES)
	{
		size += BEFORE_RED_ZONE_SIZE + AFTER_RED_ZONE_SIZE;
	}

	/* Keep minimum allocation roundup for blocks larger than pool
		extend size. */
	if (size < extInc) {
		size = extInc;
	}
	if (size < MIN_ALLOCATION) {
		size = MIN_ALLOCATION;
	}

	size_t alloc_size = size << ALLOC_ALIGN_SHIFT;

	void* mem = p ?	p->allocate_int(alloc_size, -2) :
				MemoryPool::malloc_from_system(alloc_size);
	if (!mem)
	{
		pool_out_of_memory();
	}
	*((MemoryPool**)mem) = p;

	Segment* seg = (Segment*)(((UCHAR*)mem) + (1 << ALLOC_ALIGN_SHIFT) );
	segRet = seg;

	seg->start = ( ((UCHAR*)seg) + (SEGMENT_ALLOC_UNITS << ALLOC_ALIGN_SHIFT) );
	if (ENABLE_RED_ZONES)
	{
		memset(seg->start, RED_ZONE_FILL, (s_units + compute_header_units(s_units) +
							BEFORE_RED_ZONE_SIZE + AFTER_RED_ZONE_SIZE) << ALLOC_ALIGN_SHIFT);
	}
	void* result = ((UCHAR*)seg->start) +  ((compute_header_units(s_units) +
						(ENABLE_RED_ZONES ? BEFORE_RED_ZONE_SIZE : 0) ) << ALLOC_ALIGN_SHIFT);
	seg->firstFree = (Segment::FreeBlock*)( ((UCHAR*)result) +
		((s_units + (ENABLE_RED_ZONES ? AFTER_RED_ZONE_SIZE : 0) ) << ALLOC_ALIGN_SHIFT) );
	seg->firstFree->next = 0;
	seg->firstFree->length = size - SEGMENT_ALLOC_UNITS - 1 - s_units -
			compute_header_units(s_units);
	if (ENABLE_RED_ZONES)
	{
		seg->firstFree->length -= BEFORE_RED_ZONE_SIZE + AFTER_RED_ZONE_SIZE;
	}
	seg->end = (UCHAR*)seg->firstFree +
		(seg->firstFree->length << ALLOC_ALIGN_SHIFT);
	seg->allocated = s_units;
	seg->overhead = SEGMENT_ALLOC_UNITS + 1 + compute_header_units(s_units);
	if (ENABLE_RED_ZONES)
	{
		seg->overhead += BEFORE_RED_ZONE_SIZE + AFTER_RED_ZONE_SIZE;
	}
	seg->pool = (FBMemoryPool*)result;
	seg->next = 0;

	FullHeader header;
	header.type    = -1;
	header.length  = s_units;
	header.segment = seg;
	write_header(result, header);
	return result;
}

/** The matching operator delete for our operator new.  This gets called if
	and only if there is an exception raised during the construction of
	the object.  After the object has been constructed the standard operator
	delete is called for deletion. 
**/
void FBMemoryPool::operator delete(void* mem, MemoryPool* p, Segment*& segRet, size_t)
{
	if (mem) {
		operator delete(mem);
	}
}

/**	This is called when our FBMemoryPool object's memory needs to be destroyed.
	At this point all other memory from the pool has been released courtesy of
	the destructor.  All we have to worry about is the FBPool itself.
	But the pool is still contained in that very first segment we allocated
	in the operator new(), which has our parent pool stashed above it.
	So we account for all those structures and then delete the segment.
**/
void FBMemoryPool::operator delete(void* gone)
{
	Segment* seg = findSegment(gone);
	void*    mem = ((UCHAR*)seg) - (1 << ALLOC_ALIGN_SHIFT);
	MemoryPool* parent = *((MemoryPool**) mem);

	if (!parent) {
		MemoryPool::free_from_system(mem);
	} else {
		parent->deallocate(mem);
	}
}

/** Walks the memory pool and calls the given function for each object
	in the pool.  Used to implement print_pool and the full validation.
**/
void FBMemoryPool::walk_pool(void (*callback)(void*, int, void*), void* cookie)
{
	pool_locker lock_holder(lock);

	if (callback)
		(*callback)(this, 1, cookie);

	for (Segment *seg = firstSegment; seg; seg = seg->next)
	{
		assert(seg != (void*)1);
		if (callback)
			(*callback)(seg, 2, cookie);

		Segment::FreeBlock* nextFree = seg->firstFree;
		UCHAR* curr = (UCHAR*)seg->start;

		while(curr != (UCHAR*)seg->end)
		{
			if ((Segment::FreeBlock*)curr == nextFree)
			{
				if (callback)
					(*callback)(curr, 3, cookie);
				curr += nextFree->length << ALLOC_ALIGN_SHIFT;
				nextFree = nextFree->next;
				assert(nextFree != (void*)1);
			}
			else
			{
				FullHeader header;
				curr += determine_header_size(curr, seg) << ALLOC_ALIGN_SHIFT;
				read_header(curr, &header);
				if (ENABLE_RED_ZONES)
				{
					check_red_zone(curr);
				}
				if (callback)
					(*callback)(curr, 4, cookie);
				curr += (header.length + (ENABLE_RED_ZONES ? AFTER_RED_ZONE_SIZE : 0) ) << ALLOC_ALIGN_SHIFT;
			}
			assert(curr <= (UCHAR*)seg->end);
		}
	}
	//if (callback)
	//	(*callback)(0, 5, cookie);
}

struct print_cookie
{
	IB_FILE *out;
	const char *(*name_func)(int);
	void (*detail_func)(int, void*, IB_FILE*, const char*);
};

/**	Prints out the contents of the memory pool using the walk_pool
	function.
**/
void FBMemoryPool::print_pool(IB_FILE *out,
		const char *(*name_func)(int),
		void (*detail_func)(int, void*, IB_FILE*, const char*))
{
	print_cookie cookie;

	cookie.out = out;
	cookie.name_func = name_func;
	cookie.detail_func = detail_func;

	walk_pool(print_pool_callback, (void*)&cookie);
}

/** The callback function, suitable for use in walk_pool, that prints the
	contents of the memory pool.
**/
void FBMemoryPool::print_pool_callback(void * mem, int type, void *cook)
{
	print_cookie *cookie = (print_cookie*)cook;
	Segment *seg = (Segment*)mem;
	Segment::FreeBlock *frb = (Segment::FreeBlock*)mem;

	switch(type)
	{
		case 1:
			ib_fprintf(cookie->out, "Pool: %p\n", mem);
			break;

		case 2:
			ib_fprintf(cookie->out, "\tSegment %p: %d bytes, %ld used, "
									"%2.2f%% overhead\n", seg,
				((UCHAR*)seg->end - (UCHAR*)seg->start) +
					(SEGMENT_ALLOC_UNITS << ALLOC_ALIGN_SHIFT),
					(seg->overhead + seg->allocated) << ALLOC_ALIGN_SHIFT,
					((double)seg->overhead)*100.0 / ((double)seg->allocated) );
			break;

		case 3:
			ib_fprintf(cookie->out, "\t\t%9p FreeBlock: %ld bytes\n",
						frb, frb->length << ALLOC_ALIGN_SHIFT);
			break;

		case 5:
			ib_fprintf(cookie->out, "\n");
			break;

		case 4:
				FullHeader header;
				read_header(mem, &header);
				ib_fprintf(cookie->out, "\t\t%9p ", mem);
				switch(header.type)
				{
					case 0:
						ib_fprintf(cookie->out, "Untyped: ");
						break;
					case -1:
						ib_fprintf(cookie->out, "MemoryPool: ");
						break;
					case -2:
						ib_fprintf(cookie->out, "MemoryPool::Segment: ");
						break;
					default:
						if ( header.type > 0 && cookie->name_func
								&& (*cookie->name_func)(header.type) )
						{
							ib_fprintf(cookie->out, "%s: ",
								(*cookie->name_func)(header.type));
							break;
						}
						ib_fprintf(cookie->out, "Object type %d: ",
								header.type);
				}
				ib_fprintf(cookie->out, "%ld bytes\n",
								header.length << ALLOC_ALIGN_SHIFT);
				if (header.type > 0 && cookie->detail_func) {
						(*cookie->detail_func)(header.type, mem,
									cookie->out, "\t\t\t");
			}
			break;
	}
}

/**	A helper function used when walking the memory pool structures.
	Given a pointer to the _beginning_ of an object header it returns
	the size of the header so you can find the start of the object.
**/
int FBMemoryPool::determine_header_size(const void* mem, Segment *seg)
{
	int redSize = (ENABLE_RED_ZONES ? BEFORE_RED_ZONE_SIZE : 0);
	
    if (SMALL_HEADER_ALLOC_UNITS <= 1 && FULL_HEADER_ALLOC_UNITS > 1)
    {
        const SmallHeader* smHeader = reinterpret_cast<const SmallHeader*>(mem);
        
        if (smHeader->segment == seg && smHeader->length >= 0) {
            return SMALL_HEADER_ALLOC_UNITS + redSize;
		}
		const void* p = reinterpret_cast<const UCHAR*>(mem) +
						(EXTENDED_HEADER_ALLOC_UNITS << ALLOC_ALIGN_SHIFT);
        smHeader = reinterpret_cast<const SmallHeader*>(p);
        if (smHeader->segment == seg && smHeader->length < 0) {
            return SMALL_HEADER_ALLOC_UNITS + EXTENDED_HEADER_ALLOC_UNITS + redSize;
		}
		assert(0);
    }
    else
    {
        return FULL_HEADER_ALLOC_UNITS + redSize;
    }
}

/** Releases all the memory associated with a pool.  Doesn't release the
	segment that contains the FBMemoryPool object itself.
**/
void FBMemoryPool::release_pool()
{
	if (ENABLE_FREQUENT_VERIFICATION)
	{
		verify_pool(true);
	}
		
	// Delete all the segments in the current pool, except
	// the segment that holds the pool object itself!
	Segment* next;
	for (Segment* curr = firstSegment; curr; curr = next)
	{
		next = curr->next;
		assert(next != (void*)1);

		if (curr == poolSegment) {
			continue;
		}
		if (parentPool) {
			parentPool->deallocate(curr);
		} else {
			MemoryPool::free_from_system(curr);
		}
	}
	if (poolSegment) {
		poolSegment->next = 0;
	}

	firstSegment = poolSegment;
    
    /* Now delete all the objects besides the pool object in the
     * segment that holds the pool object.  Note that this code
     * ASSUMES the pool is the first object in the segment.  If
     * that is not the case VERY BAD things will happen.
     */
	if (poolSegment)
	{
		FullHeader header;
		read_header(this, &header);
		void* checkPool = ((UCHAR*)poolSegment->start +
			((compute_header_units(header.length) +
				(ENABLE_RED_ZONES ? BEFORE_RED_ZONE_SIZE : 0) ) << ALLOC_ALIGN_SHIFT) );
		assert(header.type == -1);
		assert(header.segment == poolSegment);
		assert(this == checkPool);

		Segment::FreeBlock *newFree = (Segment::FreeBlock*)
			((UCHAR*)poolSegment->start +
			 ( (compute_header_units(header.length) + header.length +
					(ENABLE_RED_ZONES ? BEFORE_RED_ZONE_SIZE + AFTER_RED_ZONE_SIZE : 0))
			    << ALLOC_ALIGN_SHIFT) );
		newFree->next = 0;
		newFree->length =((UCHAR*)poolSegment->end - (UCHAR*)poolSegment->start)
			>> ALLOC_ALIGN_SHIFT;
		newFree->length -= header.length;
		newFree->length -= compute_header_units(header.length);
		if (ENABLE_RED_ZONES)
		{
			newFree->length -= BEFORE_RED_ZONE_SIZE + AFTER_RED_ZONE_SIZE;
		}
		poolSegment->firstFree = newFree;
	}
}

/** Adds a segment to the pool that is large enough to old an object of
	the size past in.  Observes the minimum pool extend size.
**/
void FBMemoryPool::extend_pool(size_t size)
{
	size_t minimum_size = size + SEGMENT_ALLOC_UNITS;
	size_t extend_size  = minimum_size < extend_increment ? extend_increment : minimum_size;
	size_t alloc_size   = extend_size << ALLOC_ALIGN_SHIFT;

	Segment* seg =
		(Segment*) (parentPool ?
					parentPool->allocate_int(alloc_size, -2) :
					MemoryPool::malloc_from_system(alloc_size));
	if (!seg)
	{
		pool_out_of_memory();
	}

	seg->pool = this;
	seg->firstFree = (Segment::FreeBlock*)( ((UCHAR*)seg) +
					(SEGMENT_ALLOC_UNITS << ALLOC_ALIGN_SHIFT) );
	seg->firstFree->next = 0;
	seg->firstFree->length = extend_size - SEGMENT_ALLOC_UNITS;
	seg->start = (void*)seg->firstFree;
	seg->end = (UCHAR*)seg->start +
				(seg->firstFree->length << ALLOC_ALIGN_SHIFT);
	seg->allocated = 0;
	seg->overhead = SEGMENT_ALLOC_UNITS;

	pool_locker lock_holder(lock);

	seg->next = firstSegment;
	assert(seg->next != (void*)1);
	firstSegment = seg;
}

/**	Deallocate an object allocated from the pool.  It also checks for
	red zone violations as requested.
**/
int FBMemoryPool::deallocate(void* obj)
{
	Segment::FreeBlock **ptr, **prior, *newFree, *tmp;
	FullHeader header;
	void* freeMem;
	size_t length;

	if (ENABLE_FREQUENT_VERIFICATION)
	{
		verify_pool(true);
	}
	if (ENABLE_RED_ZONES)
	{
		check_red_zone(obj);
	}
	
	read_header(obj, &header);

	size_t hUnits = compute_header_units(header.length);
	freeMem = (UCHAR*)obj - ((hUnits + (ENABLE_RED_ZONES ? BEFORE_RED_ZONE_SIZE : 0)) << ALLOC_ALIGN_SHIFT);
	newFree = (Segment::FreeBlock*)freeMem;

	pool_locker lock_holder(lock);


#ifdef DEBUG_GDS_ALLOC
/* Debugging code to erase memory locations after a release - 
 * this will assist in catching dangling memory references to
 * freed locations.
 */

/* Pattern to set "released" memory to */
#define ALL_RELEASED_FREE_PATTERN	0xBB
	memset(freeMem, ALL_RELEASED_FREE_PATTERN,
			((header.length + hUnits) << ALLOC_ALIGN_SHIFT) );
#endif /* DEBUG_GDS_ALLOC */

	for (prior = NULL, ptr = &header.segment->firstFree;
		 (*ptr); prior = ptr, ptr = &(*ptr)->next)
	{
		assert((*ptr) != (void*)1);
		if ((SCHAR HUGE_PTR*) freeMem <= (SCHAR HUGE_PTR*) (*ptr)) {
			break;
		}
	}

	if ((SCHAR HUGE_PTR *) freeMem == (SCHAR HUGE_PTR*) (*ptr))
	{
		BUGCHECK(154);	   /* msg 154 attempt to release free block */
	}

/* Merge block into list first, then try to combine blocks */

	newFree->length = header.length + hUnits;
	if (ENABLE_RED_ZONES)
	{
		newFree->length += BEFORE_RED_ZONE_SIZE + AFTER_RED_ZONE_SIZE;
		header.segment->overhead -= BEFORE_RED_ZONE_SIZE + AFTER_RED_ZONE_SIZE;
	}
	newFree->next = *ptr;
	assert(newFree->next != (void*)1);
	*ptr = newFree;
	header.segment->overhead -= hUnits;
	header.segment->allocated -= header.length;

/* Try to merge the free block with the next one down. */

	if ( (((UCHAR*)newFree) + (newFree->length << ALLOC_ALIGN_SHIFT)) ==
                    ((UCHAR*)newFree->next) )
	{
		assert(newFree->next != (void*)1);
		length = newFree->next->length;
		tmp = newFree->next;
		newFree->next = tmp->next;
		assert(newFree->next != (void*)1);
		newFree->length += length;
#ifdef DEBUG_GDS_ALLOC
	    /* We've joined the blocks, so paint the header of the merged away 
	     * block as freed.
	     */
	    memset((void*)tmp, ALL_RELEASED_FREE_PATTERN,
                                  length << ALLOC_ALIGN_SHIFT);
#endif
	}
	else if ( (((UCHAR*)newFree) + (newFree->length << ALLOC_ALIGN_SHIFT))
                    > ((void*)newFree->next) && newFree->next )
	{
		assert(newFree->next != (void*)1);
		BUGCHECK(155);		/* msg 155 attempt to release block overlapping following free block */
	}


/* Try and merge the block with the prior free block */
	if (prior && (((UCHAR*)(*prior)) +
                       ((*prior)->length << ALLOC_ALIGN_SHIFT)) ==
                           (UCHAR*)newFree)
        {
            tmp = (*prior)->next;
			assert(tmp != (void*)1);
            length = tmp->length;
			assert(tmp->next != (void*)1);
            (*prior)->next = tmp->next;
            (*prior)->length += length;
#ifdef DEBUG_GDS_ALLOC
	    /* We've joined the blocks, so paint the header of the merged away 
	     * block as freed.
	     */
	    memset( (void*)tmp, ALL_RELEASED_FREE_PATTERN,
				   length << ALLOC_ALIGN_SHIFT);
#endif
	}
	else if (prior && (((UCHAR*)(*prior)) +
                  ((*prior)->length << ALLOC_ALIGN_SHIFT)) > (UCHAR*)newFree )
	{
		BUGCHECK(156);		/* msg 156 attempt to release block overlapping prior free block */
	}

	if (blk_type_count) {
		--(*blk_type_count)[header.type];
	}

	return (header.length + hUnits) << ALLOC_ALIGN_SHIFT;
}

/**	Implements the full and partial verification of the FBMemoryPool.
**/
void FBMemoryPool::verify_pool(bool full)
{
	if ( (ENABLE_FREQUENT_VERIFICATION && !full) || full)
	{
		walk_pool();
	}
	else
	{
		pool_locker lock_holder(lock);
    
		for (Segment* seg = firstSegment; seg; seg = seg->next) {
			assert(seg != (void*)1);
		}
	}
}

/**	An internal helper function that reads the contents of a block header
	and returns them in a FullHeader object.  It accounts for the different
	header sizes and the presence of red zone(s).
**/
void FBMemoryPool::read_header(const void* obj, FullHeader* header)
{
	// Make sure we read our headers from before any red zones
	if (ENABLE_RED_ZONES)
	{
		obj = (UCHAR*)obj - (BEFORE_RED_ZONE_SIZE << ALLOC_ALIGN_SHIFT);
	}
		
    // If our small header is one or fewer allocation units and
    // our full header is more than 1, use the small and extended headers,
    // otherwise use the full header.
    if (SMALL_HEADER_ALLOC_UNITS <= 1 && FULL_HEADER_ALLOC_UNITS > 1)
    {
        SmallHeader* smHeader;
        ExtendedHeader* exHeader;

        smHeader = (SmallHeader*)
            ((UCHAR*)obj - (SMALL_HEADER_ALLOC_UNITS << ALLOC_ALIGN_SHIFT));
        header->segment = smHeader->segment;
        header->type = smHeader->type;
        if (smHeader->length >= 0)
            header->length = smHeader->length;
        else
        {
            exHeader = (ExtendedHeader*)((UCHAR*)obj -
                  ((SMALL_HEADER_ALLOC_UNITS + EXTENDED_HEADER_ALLOC_UNITS)
                  << ALLOC_ALIGN_SHIFT));
            header->length = exHeader->length;
        }
    }
    else
    {
		*header = *((FullHeader*)((UCHAR*)obj - (FULL_HEADER_ALLOC_UNITS << ALLOC_ALIGN_SHIFT)));
    }
}

/**	An internal helper function that takes a FullHeader and writes it to the
	objects header in the pool.  It correctly converts the full header to
	a small header as needed.  It also accounts for any red zone(s).
**/
void FBMemoryPool::write_header(void* obj, const FullHeader& header)
{
	// Make sure we put our headers _before_ any red zones
	if (ENABLE_RED_ZONES)
	{
		obj = (UCHAR*)obj - (BEFORE_RED_ZONE_SIZE << ALLOC_ALIGN_SHIFT);
	}
	
    // If our small header is one or fewer allocation units and
    // our full header is more than 1, use the small and extended headers,
    // otherwise use the full header.
    if (SMALL_HEADER_ALLOC_UNITS <= 1 && FULL_HEADER_ALLOC_UNITS > 1)
    {
        SmallHeader* smHeader;
        ExtendedHeader* exHeader;

        smHeader = (SmallHeader*)
            ((UCHAR*)obj - (SMALL_HEADER_ALLOC_UNITS << ALLOC_ALIGN_SHIFT));
        smHeader->segment = header.segment;
        smHeader->type = header.type;
        if (header.length <= MAX_SSHORT)
            smHeader->length = header.length;
        else
        {
            smHeader->length = -1;
            exHeader = (ExtendedHeader*)((UCHAR*)obj -
                  ((SMALL_HEADER_ALLOC_UNITS + EXTENDED_HEADER_ALLOC_UNITS)
                  << ALLOC_ALIGN_SHIFT));
            exHeader->length = header.length;
        }
    }
    else
    {
		*((FullHeader*)((UCHAR*)obj - (FULL_HEADER_ALLOC_UNITS << ALLOC_ALIGN_SHIFT))) = header;
    }
}

/**	Given an object this function verifies the red zones are intact.
	If they are not intact it throws an exception.
**/
inline void FBMemoryPool::check_red_zone(void *obj)
{
	if (ENABLE_RED_ZONES)
	{
		FullHeader header;
		read_header(obj, &header);
		
		UCHAR* curr = (UCHAR*)obj;
		curr -= BEFORE_RED_ZONE_SIZE << ALLOC_ALIGN_SHIFT;
		for(int i = 0; i < (BEFORE_RED_ZONE_SIZE << ALLOC_ALIGN_SHIFT); i++)
		{
			if (curr[i] != RED_ZONE_FILL)
			{
				Firebird::red_zone_error::raise();
			}
		}
		
		curr = (UCHAR*)obj;
		curr += header.length << ALLOC_ALIGN_SHIFT;
		for(int i = 0; i < (AFTER_RED_ZONE_SIZE << ALLOC_ALIGN_SHIFT); i++)
		{
			if (curr[i] != RED_ZONE_FILL)
			{
				Firebird::red_zone_error::raise();
			}
		}
	}
}

/**	Given the length of an object this function returns the number of allocation
	units needed to hold the object head.
**/
inline size_t FBMemoryPool::compute_header_units(size_t len)
{
    // If our small header is one or fewer allocation units and
    // our full header is more than 1, use the small and extended headers,
    // otherwise use the full header.
    if (SMALL_HEADER_ALLOC_UNITS <= 1 && FULL_HEADER_ALLOC_UNITS > 1)
    {
        if (len <= MAX_SSHORT)
            return SMALL_HEADER_ALLOC_UNITS;
        else
            return SMALL_HEADER_ALLOC_UNITS + EXTENDED_HEADER_ALLOC_UNITS;
    }
    else
        return FULL_HEADER_ALLOC_UNITS;
}

/// Return the type associated with the object.
SSHORT FBMemoryPool::blk_type(const void* mem)
{
	FullHeader block;
	read_header(mem, &block);
	return block.type;
}

/// Return the pool this object was allocated out of.
MemoryPool* FBMemoryPool::blk_pool(const void* mem)
{
	FullHeader block;
	read_header(mem, &block);
	return block.segment->pool->pimplParent;
}

/// Return the length of the allocated object in allocation units
int FBMemoryPool::blk_length(const void* mem)
{
	FullHeader block;
	read_header(mem, &block);
	return block.length;
}

/// Return the segment, within a pool, that an object is allocated out of.
FBMemoryPool::Segment* FBMemoryPool::findSegment(const void* mem)
{
	FullHeader block;
	read_header(mem, &block);
	return block.segment;
}

/// Accessor for the minimum pool extend size.
size_t FBMemoryPool::extendSize()
{
	return extend_increment << ALLOC_ALIGN_SHIFT;
}

/// Sets the minimum pool extend size.
void FBMemoryPool::setExtendSize(size_t s)
{
	extend_increment = ( s + ALLOC_ALIGNMENT - 1) >> ALLOC_ALIGN_SHIFT;
	if (extend_increment < MIN_ALLOCATION)
		extend_increment = MIN_ALLOCATION;
}

/// Allocate from the host operating system using the malloc call if available.
void* FBMemoryPool::malloc_alloc(size_t size)
{
    void* block;

    size += ALLOC_ALIGNMENT;

    globalAllocLock.aquire();

#ifdef VMS
	SLONG status = lib$get_vm(&size, &block);
	if (!(status & 1))
	{
		globalAllocLock.release();
		pool_out_of_memory();
	}
#define MEMORY_ALLOCATED
#endif

#ifndef MEMORY_ALLOCATED
	if ((block = malloc(size)) == NULL)
	{
		globalAllocLock.release();
		pool_out_of_memory();
	}
#endif
#undef MEMORY_ALLOCATED

/*
#ifdef DEV_BUILD
        gds_max_alloc += tail;
#endif

#ifdef DEV_BUILD
        gds_delta_alloc += tail;
#endif
*/
#ifdef DEV_BUILD
	memset((UCHAR *) block, 0xCD, size);
#endif
	
    globalAllocLock.release();
    *((ULONG *) block) = size;
    return (UCHAR*)block + ALLOC_ALIGNMENT;
}

/// Return memory allocated with malloc_alloc to the host os.  Returns the
/// number of bytes freed.
SLONG FBMemoryPool::malloc_free(void* mem)
{
    ULONG* block = (ULONG*) ((UCHAR*) mem - ALLOC_ALIGNMENT);
    mem = (void*) block;
    ULONG length = *block;

    globalAllocLock.aquire();

#ifdef VMS
        SLONG status = lib$free_vm(&length, &mem);
        if (!(status & 1))
		{
    		globalAllocLock.release();
			pool_out_of_memory();
		}
#define MEMORY_DEALLOCATED
#endif  // VMS


#ifndef MEMORY_DEALLOCATED
    free(mem);
#endif
#undef MEMORY_DEALLOCATED
    globalAllocLock.release();
    return length - ALLOC_ALIGNMENT;
}

/** Allocate memory from the host operating systems using the platform specific
	virtual memory functions.
**/
void* FBMemoryPool::mmap_alloc(size_t size)
{
    UCHAR* memory;
    ULONG* block;

    size += ALLOC_ALIGNMENT;

    globalAllocLock.aquire();
#ifdef NETWARE
        REPLACE THIS ILLEGAL COMMENT WITH NETWARE IMPLEMENTATION
                Malloc / free may be good enough as long as free() returns
                the memory back to a system - wide heap.
#define SYS_ALLOC_DEFINED
#endif
#ifdef UNIX
#ifdef MAP_ANONYMOUS
	memory = (UCHAR*)mmap(NULL, size, (PROT_READ | PROT_WRITE),
#if (defined(MAP_ANONYMOUS) && !defined(DARWIN))
                                (MAP_ANONYMOUS |
#else
                                (MAP_ANON |
#endif
#if (!defined(LINUX) && !defined(DARWIN))
/* In LINUX and Darwin there is no such thing as MAP_VARIABLE. Hence, it gives
   compilation error. The equivalent functionality is default,
   if you do not specify MAP_FIXED */
                                 MAP_VARIABLE |
#endif                                                  /* LINUX */
                                 MAP_PRIVATE), -1, 0);
	if (memory == (UCHAR*) - 1)
	{
		    globalAllocLock.release();
			pool_out_of_memory();
	}
#define SYS_ALLOC_DEFINED
#else
#ifdef SOLARIS
    if (!(memory = mmap_anon(size)))        /* Jump thru hoops for Solaris. */
	{
		globalAllocLock.release();
        return NULL;
	}
#define SYS_ALLOC_DEFINED
#endif
#endif /* MAP_ANONYMOUS */
#endif /* UNIX */

#ifdef VMS
	{
		SLONG status;

		status = lib$get_vm(&size, &memory);
		if (!(status & 1))
		{
			globalAllocLock.release();
			pool_out_of_memory();
		}
	}
#define SYS_ALLOC_DEFINED
#endif

#ifdef WIN_NT
	memory = reinterpret_cast<UCHAR*>(VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE));
	if (!memory)
	{
		globalAllocLock.release();
		pool_out_of_memory();
/***
		DWORD error = GetLastError();

		if (error == ERROR_NOT_ENOUGH_MEMORY)
			return NULL;

		ERR_post(gds_sys_request, gds_arg_string, "VirtualAlloc",
				gds_arg_win32, error, 0);
***/
	}
#define SYS_ALLOC_DEFINED
#endif

	globalAllocLock.release();
#ifdef SYS_ALLOC_DEFINED
	block = (ULONG*) memory;
	*block = size;
	return memory + ALLOC_ALIGNMENT;
#else
	pool_out_of_memory();
    return NULL;
#endif
}

SLONG FBMemoryPool::mmap_free(void* mem)
{
	ULONG* block = (ULONG*) ((UCHAR*) mem - ALLOC_ALIGNMENT);
	mem = (void*) block;
	ULONG length = *block;

	globalAllocLock.aquire();
#ifdef NETWARE
#error NETWARE is unimplemented
//      REPLACE THIS ILLEGAL COMMENT WITH NETWARE IMPLEMENTATION
//              Malloc / free may be good enough as long as free() returns
//              the memory back to a system - wide heap.
#define SYS_FREE_DEFINED
#endif  // NETWARE

#ifdef UNIX
	if (munmap(mem, length) == -1) {
		globalAllocLock.release();
		pool_out_of_memory();
	}
#define SYS_FREE_DEFINED
#endif  // UNIX

#ifdef VMS
	{
		SLONG status;

		status = lib$free_vm(&length, &mem);
		if (!(status & 1))
		{
			globalAllocLock.release();
			pool_out_of_memory();
		}
	}
#define SYS_FREE_DEFINED
#endif  // VMS

#ifdef WIN_NT
	if (!VirtualFree((LPVOID) mem, (DWORD) 0, MEM_RELEASE))
	{
		globalAllocLock.release();
		const DWORD dwErr = GetLastError();
		pool_out_of_memory();
	}
#define SYS_FREE_DEFINED
#endif  // WIN_NT

	globalAllocLock.release();
#ifdef SYS_FREE_DEFINED
	return length - ALLOC_ALIGNMENT;
#else
	return 0L;
#endif
}
