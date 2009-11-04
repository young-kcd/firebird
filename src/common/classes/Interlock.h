/* $Id: Interlock.h,v 1.2 2009-11-04 10:51:27 robocop Exp $ */
#ifndef __INTERLOCK_H
#define __INTERLOCK_H

#if defined(SOLARIS) || defined(hpux)
extern "C"
{
extern int cas (volatile int *state, int compare, int exchange);
extern void* casx (volatile void **state, void* compare, void* exchange);
extern void membar_flush(void);
extern void membar_wait(void);
}
#define COMPARE_EXCHANGE(target,compare,exchange)\
	(cas(target, compare, exchange) == compare)
#define COMPARE_EXCHANGE_POINTER(target,compare,exchange)\
	(casx((volatile void**)target, (void*)compare, (void*)exchange) == compare)
#endif

#ifdef SOLARIS
#define WAIT_FOR_FLUSH_CACHE membar_wait();
#define FLUSH_CACHE membar_flush();
#else
/* I64 hpux currently has mf in the cas asm routines */
/* (h6icas.s). They should probably be moved here instead */
#endif


#ifdef AIX_PPC
#include <sys/atomic_op.h>
extern "C"
{
static inline int COMPARE_EXCHANGE(volatile int *target, int compare, int exchange)
{
	return compare_and_swap((int *)target, &compare, exchange);
}
static inline int COMPARE_EXCHANGE_POINTER(volatile void *target, volatile void * compare, void * exchange)
{
	return compare_and_swaplp((long *)target, (long*)&compare, (long)exchange);
}

void asm_isync(void);
   #pragma  mc_func  asm_isync  {       \
      "4c00012c" /* isync */        \
   }
void asm_sync(void);
   #pragma  mc_func  asm_sync  {       \
      "7c0004ac" /* sync */        \
   }

#define WAIT_FOR_FLUSH_CACHE asm_isync();
#define FLUSH_CACHE asm_sync();
}
#endif

#ifdef DARWIN
#define __DEBUGGING__
#include <CoreServices/CoreServices.h>
extern "C"
{
static inline int COMPARE_EXCHANGE(volatile int* target, int compare, int exchange)
{
	return CompareAndSwap((UInt32)compare, (UInt32) exchange, (UInt32 *)(target));
}

static inline int COMPARE_EXCHANGE_POINTER(void* target, volatile void* compare, void* exchange)
{
	return CompareAndSwap( (UInt32) compare, (UInt32) exchange, (UInt32 *) target);
}

}
#endif


#ifdef MVS
extern "C" {
static inline int cas (volatile int *state, int compare, int exchange)
{
	__cs((cs_t*)(&compare), (cs_t*)state, (cs_t)exchange);
	return compare;
}
static inline void* casx (volatile void *state, volatile void *compare, void *exchange)
{
	__cs((cs_t*)(&compare), (cs_t*)state, (cs_t)exchange);
	return (void*)compare;
}
}
#define COMPARE_EXCHANGE(target,compare,exchange)\
	(cas(target, compare, exchange) == compare)

#define COMPARE_EXCHANGE_POINTER(target,compare,exchange)\
	(casx(target, compare, exchange) == compare)

#endif

#ifdef __VMS
#include <builtins.h>
extern "C"
{
    static inline int COMPARE_EXCHANGE(volatile int *target, int compare, int exchange)
    {
	return __CMP_STORE_LONG(&compare, compare, exchange, target);
    }
    static inline int COMPARE_EXCHANGE_POINTER(volatile void *target, volatile void * compare, void * exchange)
    {
	return __CMP_STORE_QUAD(compare, (__int64)compare, (__int64)exchange, &target);
    }
}

#endif

#ifdef _WIN32
#include <windows.h>
#define INTERLOCKED_INCREMENT(variable)	InterlockedIncrement (&variable)
#define INTERLOCKED_DECREMENT(variable)	InterlockedDecrement (&variable)

//#define INTERLOCK_TYPE	LONG
#define COMPARE_EXCHANGE(target,compare,exchange)\
	(InterlockedCompareExchange(target, exchange, compare) == compare)
#define COMPARE_EXCHANGE_POINTER(target,compare,exchange)\
	(InterlockedCompareExchangePointer((volatile PVOID*) target, (PVOID)exchange, (PVOID)compare) == compare)
#ifndef InterlockedIncrement
#define InterlockedIncrement		_InterlockedIncrement
#define InterlockedDecrement		_InterlockedDecrement
//#define InterlockedCompareExchange	_InterlockedCompareExchange

extern "C"
	{
	INTERLOCK_TYPE  InterlockedIncrement(volatile INTERLOCK_TYPE* lpAddend);
	INTERLOCK_TYPE  InterlockedDecrement(volatile INTERLOCK_TYPE* lpAddend);
	//PVOID InterlockedCompareExchange(volatile PVOID *target, PVOID exchange, PVOID compare);
	}
#pragma intrinsic(_InterlockedIncrement)
#pragma intrinsic(_InterlockedDecrement)
//#pragma intrinsic(_InterlockedCompareExchange)
#endif

#endif

#ifndef INTERLOCKED_INCREMENT
#define INTERLOCKED_INCREMENT(variable)	interlockedIncrement (&variable)
#define INTERLOCKED_DECREMENT(variable)	interlockedDecrement (&variable)
#endif

#if defined (__i386) || (__x86_64__)
#define COMPARE_EXCHANGE(target,compare,exchange)\
	(inline_cas(target, compare, exchange))
#define COMPARE_EXCHANGE_POINTER(target,compare,exchange)\
	(inline_cas_pointer((volatile void**)target, (void*)compare, (void*)exchange))
#endif

#ifndef INTERLOCK_TYPE
#define INTERLOCK_TYPE	int
#endif

inline INTERLOCK_TYPE interlockedIncrement(volatile INTERLOCK_TYPE *ptr)
{
#ifdef _WIN32
	return InterlockedIncrement ((INTERLOCK_TYPE*) ptr);
#elif defined DARWIN
	return IncrementAtomic ((SInt32 *) ptr);
#elif defined (__i386) || (__x86_64__)
	INTERLOCK_TYPE ret;
	asm (
		"mov $1,%%eax\n\t"
		"lock\n\t" "xaddl %%eax,%1\n\t"
		"incl %%eax\n\t"
		"movl %%eax,%0\n\t"
		: "=m" (ret)
		: "m" (*ptr)
		: "%eax"
		);
	return ret;
#else
	for (;;)
		{
		int oldValue = *ptr;
		int newValue = oldValue + 1;
		if (COMPARE_EXCHANGE(ptr, oldValue, newValue))
			return newValue;
		}
#endif
}

inline INTERLOCK_TYPE interlockedDecrement(volatile INTERLOCK_TYPE *ptr)
{
#ifdef _WIN32
	return InterlockedDecrement ((INTERLOCK_TYPE*) ptr);
#elif defined DARWIN
	return DecrementAtomic ((SInt32 *) ptr);
#elif defined (__i386) || (__x86_64__)
	INTERLOCK_TYPE ret;
	asm (
		"mov $-1,%%eax\n\t"
		"lock\n\t" "xaddl %%eax,%1\n\t"
		"decl %%eax\n\t"
		"movl %%eax,%0\n\t"
		: "=m" (ret)
		: "m" (*ptr)
		: "%eax"
		);
	return ret;
#else
	for (;;)
		{
		int oldValue = *ptr;
		int newValue = oldValue - 1;
		if (COMPARE_EXCHANGE(ptr, oldValue, newValue))
			return newValue;
		}
#endif
}

inline int inline_cas (volatile int *target, int compare, int exchange)
{
#if defined(__i386) || (__x86_64__)
    char ret;

    __asm __volatile ("lock; cmpxchg %2, %1 ; sete %0"
            : "=q" (ret), "+m" (*(target))
            : "r" (exchange), "a" (compare)
            : "cc", "memory");

    return ret;
#else
	return -2;
#endif
}

inline char inline_cas_pointer (volatile void **target, void *compare, void *exchange)
{
#if defined(__i386) || defined(__x86_64__)
    char ret;

    __asm __volatile ("lock; cmpxchg %2, %1 ; sete %0"
            : "=q" (ret), "+m" (*(target))
            : "r" (exchange), "a" (compare)
            : "cc", "memory");

    return ret;

#else
	return NULL;
#endif
}


/* On some machines, a "wait for flush complete" type instruction must be */
/* executed to insure cache coherency across processors after the atomic */
/* lock has been granted */
#ifndef WAIT_FOR_FLUSH_CACHE
#define WAIT_FOR_FLUSH_CACHE
#endif

/* On some machines, the cache must be flushed before an atomic lock is */
/* released. */
#ifndef FLUSH_CACHE
#define FLUSH_CACHE
#endif

#endif //  __INTERLOCK_H
