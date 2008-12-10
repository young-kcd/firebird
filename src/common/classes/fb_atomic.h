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
 *
 *  $Id: fb_atomic.h,v 1.4 2004-06-30 01:26:06 skidder Exp $
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
	
	AtomicCounter(counter_type val = 0) : counter(val) {}
	~AtomicCounter() {}
			
	counter_type exchangeAdd(counter_type val) {
		return InterlockedExchangeAdd(&counter, val);
	}
	
	counter_type operator +=(counter_type val) {
		return exchangeAdd(val) + val;
	}
	
	counter_type operator -=(counter_type val) {
		return exchangeAdd(-val) - val;
	}
	
	counter_type operator ++() {
		return InterlockedIncrement(&counter);
	}
	
	counter_type operator --() {
		return InterlockedDecrement(&counter);
	}
	
	counter_type value() const { return counter; }
	
private:
# if (defined(_MSC_VER) && (_MSC_VER <= 1200)) || defined(MINGW)
	counter_type counter;
# else
	volatile counter_type counter;
# endif
};

} // namespace Firebird

#elif defined(__GNUC__) && (defined(i386) || defined(I386) || defined(_M_IX86) || defined(AMD64))

// Assembler version for x86 and AMD64. Note it uses xaddl thus it requires i486
class AtomicCounter
{
public:
	typedef int counter_type;
	
	AtomicCounter(counter_type value = 0) : counter(value) {}
	~AtomicCounter() {}
			
	counter_type exchangeAdd(counter_type value) {
		register counter_type result;
		__asm __volatile (
			"lock; xaddl %0, %1"
			 : "=r" (result), "=m" (counter)
			 : "0" (value), "m" (counter));
		return result;
	}
	
	counter_type operator +=(counter_type value) {
		return exchangeAdd(value) + value;
	}
	
	counter_type operator -=(counter_type value) {
		return exchangeAdd(-value) - value;
	}
	
	counter_type operator ++() {
		return exchangeAdd(1) + 1;
	}
	
	counter_type operator --() {
		return exchangeAdd(-1) - 1;
	}
	
	counter_type value() const { return counter; }
	
private:
	volatile counter_type counter;
};

#else

# include "../common/classes/locks.h"

namespace Firebird {

// Highly inefficient, but safe and portable implementation
class AtomicCounter
{
public:
	typedef int counter_type;
	
	AtomicCounter(counter_type value = 0) : counter(value) {}
	~AtomicCounter() {}
			
	counter_type exchangeAdd(counter_type value) {
		lock.enter();
		counter_type temp = counter;
		counter += value;
		lock.leave();
		return temp;		
	}
	
	counter_type operator +=(counter_type value) {
		lock.enter();
		counter_type temp = counter += value;
		lock.leave();
		return temp;
	}
	
	counter_type operator -=(counter_type value) {
		lock.enter();
		counter_type temp = counter -= value;
		lock.leave();
		return temp;
	}
	
	counter_type operator ++() {
		lock.enter();
		counter_type temp = counter++;
		lock.leave();
		return temp;
	}
	
	counter_type operator --() {
		lock.enter();
		counter_type temp = counter--;
		lock.leave();
		return temp;
	}
	
	counter_type value() const { return counter; }
	
private:
	volatile counter_type counter;
	Mutex lock;
};

} // namespace Firebird

#endif

#endif // CLASSES_FB_ATOMIC_H
