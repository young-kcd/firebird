/*
 *	PROGRAM:		Client/Server Common Code
 *	MODULE:			fb_atomic.h
 *	DESCRIPTION:	Atomic counters
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Nickolay Samofatov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Nickolay Samofatov <nickolay@broadviewsoftware.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 */

#ifndef CLASSES_FB_ATOMIC_H
#define CLASSES_FB_ATOMIC_H

#if defined(WIN_NT)

#include <windows.h>

namespace Firebird {

// Win95 is not supported unless compiled conditionally and
// redirected to generic version below
class AtomicCounter
{
public:
	typedef LONG counter_type;

	explicit AtomicCounter(counter_type val = 0) : counter(val) {}
	~AtomicCounter() {}

	counter_type exchangeAdd(counter_type val)
	{
		return InterlockedExchangeAdd(&counter, val);
	}

	counter_type operator +=(counter_type val)
	{
		return exchangeAdd(val) + val;
	}

	counter_type operator -=(counter_type val)
	{
		return exchangeAdd(-val) - val;
	}

	counter_type operator ++()
	{
		return InterlockedIncrement(&counter);
	}

	counter_type operator --()
	{
		return InterlockedDecrement(&counter);
	}

	counter_type value() const { return counter; }

	counter_type setValue(counter_type val)
	{
		return InterlockedExchange(&counter, val);
	}

private:
# if defined(MINGW)
	counter_type counter;
# else
	volatile counter_type counter;
# endif
};

} // namespace Firebird

#elif defined(__GNUC__) && (defined(i386) || defined(I386) || defined(_M_IX86) || defined(AMD64) || defined(__x86_64__))

namespace Firebird {

// Assembler version for x86 and AMD64. Note it uses xaddl thus it requires i486
class AtomicCounter
{
public:
	typedef int counter_type;

	explicit AtomicCounter(counter_type value = 0) : counter(value) {}
	~AtomicCounter() {}

	counter_type exchangeAdd(counter_type value)
	{
		register counter_type result;
		__asm __volatile (
			"lock; xaddl %0, %1"
			 : "=r" (result), "=m" (counter)
			 : "0" (value), "m" (counter));
		return result;
	}

	counter_type operator +=(counter_type value)
	{
		return exchangeAdd(value) + value;
	}

	counter_type operator -=(counter_type value)
	{
		return exchangeAdd(-value) - value;
	}

	counter_type operator ++()
	{
		return exchangeAdd(1) + 1;
	}

	counter_type operator --()
	{
		return exchangeAdd(-1) - 1;
	}

	counter_type value() const { return counter; }

	counter_type setValue(counter_type val)
	{
		register counter_type result;
		__asm __volatile (
			"lock; xchg %0, %1"
			 : "=r" (result), "=m" (counter)
			 : "0" (val), "m" (counter));
		return result;
	}

private:
	volatile counter_type counter;
};

} // namespace Firebird

#elif defined(AIX)

#include <sys/atomic_op.h>

namespace Firebird {

// AIX version - uses AIX atomic API
class AtomicCounter
{
public:
	typedef int counter_type;

	explicit AtomicCounter(counter_type value = 0) : counter(value) {}
	~AtomicCounter() {}

	counter_type exchangeAdd(counter_type value)
	{
		return fetch_and_add(&counter, value);
	}

	counter_type operator +=(counter_type value)
	{
		return exchangeAdd(value) + value;
	}

	counter_type operator -=(counter_type value)
	{
		return exchangeAdd(-value) - value;
	}

	counter_type operator ++()
	{
		return exchangeAdd(1) + 1;
	}

	counter_type operator --()
	{
		return exchangeAdd(-1) - 1;
	}

	counter_type value() const { return counter; }

	counter_type setValue(counter_type val)
	{
		counter_type old;
		do
		{
			old = counter;
		} while (!compare_and_swap(&counter, &old, val));
		return old;
	}

private:
	counter_type counter;
};

} // namespace Firebird

#elif defined(HPUX)

#include <atomic.h>

namespace Firebird {

// HPUX version - uses atomic API library
class AtomicCounter
{
public:
	typedef uint32_t counter_type;

	explicit AtomicCounter(counter_type value = 0) : counter(value) {}
	~AtomicCounter() {}

	counter_type exchangeAdd(counter_type value)
	{
		counter_type old = counter;
		do
		{
			old = counter;
			errno = 0;
		} while (atomic_cas_32(&counter, old, old + value) != old);
		return old;
	}

	counter_type operator +=(counter_type value)
	{
		return exchangeAdd(value) + value;
	}

	counter_type operator -=(counter_type value)
	{
		return exchangeAdd(-value) - value;
	}

	counter_type operator ++()
	{
		return atomic_inc_32(&counter) + 1;
	}

	counter_type operator --()
	{
		return atomic_dec_32(&counter) - 1;
	}

	counter_type value() const { return counter; }

	counter_type setValue(counter_type val)
	{
		return atomic_swap_32(&counter, val);
	}

private:
	volatile counter_type counter;
};

} // namespace Firebird

#elif defined(SOLARIS) && defined(HAVE_ATOMIC_H)

#include <atomic.h>

namespace Firebird {

// Solaris version - uses Solaris atomic_ops
class AtomicCounter
{
public:
	typedef uint_t counter_type;
	typedef int delta_type;

	explicit AtomicCounter(counter_type value = 0) : counter(value) {}
	~AtomicCounter() {}

	counter_type exchangeAdd(delta_type value)
	{
		return atomic_add_int_nv(&counter, value);
	}

	counter_type operator +=(delta_type value)
	{
		return exchangeAdd(value) + value;
	}

	counter_type operator -=(delta_type value)
	{
		return exchangeAdd(-value) - value;
	}

	counter_type operator ++()
	{
		return atomic_inc_uint_nv(&counter);
	}

	counter_type operator --()
	{
		return atomic_dec_uint_nv(&counter);
	}

	counter_type value() const { return counter; }

	counter_type setValue(delta_type value)
	{
		return atomic_swap_uint(&counter, value);
	}

private:
	counter_type counter;
};

} // namespace Firebird


#elif defined(__SUNPRO_CC) && !defined(HAVE_ATOMIC_H) && defined(__sparc)
// Solaris 9 does not have nice <atomic.h> header file, so we provide
// an assembly language, Sun Studio Pro only, implementation.

// assembly versions of fetch_and_add_il, compare_and_swap_il,
// are in fb_atomic_*.il

// No GNU CC implementation on Solaris 9 is planned!
extern "C"
{
extern int fetch_and_add_il(volatile unsigned int *word_addr, int value);
extern boolean_t compare_and_swap_il(volatile unsigned int *word_addr, unsigned int *old_val_addr, unsigned int new_val);
}
namespace Firebird {

class AtomicCounter
{
public:
	typedef uint_t counter_type;
	typedef int delta_type;

	explicit AtomicCounter(counter_type value = 0) : counter(value) {}
	~AtomicCounter() {}

	counter_type exchangeAdd(delta_type value)
	{
		return fetch_and_add_il(&counter, value);
	}

	counter_type operator +=(delta_type value)
	{
		return exchangeAdd(value) + value;
	}

	counter_type operator -=(delta_type value)
	{
		return exchangeAdd(-value) - value;
	}

	counter_type operator ++()
	{
		return exchangeAdd(1) + 1;
	}

	counter_type operator --()
	{
		return exchangeAdd(-1) - 1;
	}

	counter_type value() const { return counter; }

	counter_type setValue(delta_type value)
	{
		counter_type old;
		do
		{
			old = counter;
		} while (!compare_and_swap_il(&counter, &old, value));
		return old;
	}

private:
	counter_type counter;
};

} // namespace Firebird

#else

#error AtomicCounter: implement appropriate code for your platform!

#endif

#endif // CLASSES_FB_ATOMIC_H
