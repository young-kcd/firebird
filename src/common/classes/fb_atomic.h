/*
 *	PROGRAM:		Client/Server Common Code
 *	MODULE:			fb_atomic.h
 *	DESCRIPTION:	Atomic counters
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * You may obtain a copy of the Licence at
 * http://www.gnu.org/licences/lgpl.html
 * 
 * As a special exception this file can also be included in modules
 * with other source code as long as that source code has been 
 * released under an Open Source Initiative certificed licence.  
 * More information about OSI certification can be found at: 
 * http://www.opensource.org 
 * 
 * This module is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public Licence for more details.
 * 
 * This module was created by members of the firebird development 
 * team.  All individual contributions remain the Copyright (C) of 
 * those individuals and all rights are reserved.  Contributors to 
 * this file are either listed below or can be obtained from a CVS 
 * history command.
 *
 *  Created by: Nickolay Samofatov <skidder@bssys.com>
 *
 *  Contributor(s):
 * 
 *
 *  $Id: fb_atomic.h,v 1.1 2004-03-25 23:12:39 skidder Exp $
 *
 */
 
#ifndef FB_ATOMIC_H
#define FB_ATOMIC_H

#if defined(WIN_NT)

#include <windows.h>

namespace Firebird {

// Win95 is not supported unless compiled conditionally and
// redirected to generic version below
class AtomicCounter
{
public:
	typedef LONG counter_type;
	
	AtomicCounter(counter_type value = 0) : counter(value) {}
	~AtomicCounter() {}
			
	counter_type exchangeAdd(counter_type value) {
		return InterlockedExchangeAdd(&counter, value);
	}
	
	counter_type operator +=(counter_type value) {
		return exchangeAdd(value) + value;
	}
	
	counter_type operator -=(counter_type value) {
		return exchangeAdd(-value) - value;
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

#endif
