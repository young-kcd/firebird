/*
 *	PROGRAM:		Client/Server Common Code
 *	MODULE:			rwlock.h
 *	DESCRIPTION:	Read/write multi-state locks
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
 *  $Id: rwlock.h,v 1.12 2004-03-11 05:30:07 skidder Exp $
 *
 */

#ifndef RWLOCK_H
#define RWLOCK_H

#ifdef WIN_NT

#include <windows.h>
#include <limits.h>

#define LOCK_WRITER_OFFSET 50000

namespace Firebird
{

// This class works on Windows 98/NT4 or later. Win95 is not supported
// Should work pretty fast.
class RWLock {
private:
	volatile LONG lock; // This is the actual lock
	           // -50000 - writer is active
			   // 0 - noone owns the lock
			   // positive value - number of concurrent readers
	volatile LONG blockedReaders;
	volatile LONG blockedWriters;
	HANDLE writers_event, readers_semaphore;

	//
	// Those inlines are needed due to the different argument taken by msvc6 and msvc7
	// funcions
	//
#if (defined(_MSC_VER) && (_MSC_VER <= 1200)) || defined(MINGW)
	inline LONG InterlockedIncrement_uni(volatile LONG* lock_p)
	{
		return InterlockedIncrement(const_cast<LONG*>(lock_p));
	}
	inline LONG InterlockedDecrement_uni(volatile LONG* lock_p)
	{
		return InterlockedDecrement(const_cast<LONG*>(lock_p));
	}
	inline LONG InterlockedExchangeAdd_uni(volatile LONG* lock_p, LONG value)
	{
		return InterlockedExchangeAdd(const_cast<LONG*>(lock_p), value);
	}
#else
	inline LONG InterlockedIncrement_uni(volatile LONG* lock_p)
	{
		return InterlockedIncrement(lock_p);
	}
	inline LONG InterlockedDecrement_uni(volatile LONG* lock_p)
	{
		return InterlockedDecrement(lock_p);
	}
	inline LONG InterlockedExchangeAdd_uni(volatile LONG* lock_p, LONG value)
	{
		return InterlockedExchangeAdd(lock_p, value);
	}
#endif

public:
	RWLock() : lock(0), blockedReaders(0), blockedWriters(0)
	{ 
		readers_semaphore = CreateSemaphore(NULL, 0 /*initial count*/, 
			INT_MAX, NULL); 
		if (readers_semaphore == NULL)
			system_call_failed::raise("CreateSemaphore");
		writers_event = CreateEvent(NULL, FALSE/*auto-reset*/, FALSE, NULL);
		if (writers_event == NULL)
			system_call_failed::raise("CreateEvent");
	}
	~RWLock()
	{
		if (readers_semaphore && !CloseHandle(readers_semaphore))
			system_call_failed::raise("CloseHandle");		
		if (writers_event && !CloseHandle(writers_event))
			system_call_failed::raise("CloseHandle");
	}
	// Returns negative value if writer is active.
	// Otherwise returns a number of readers
	LONG getState() const
	{
		return lock;
	}
	void unblockWaiting()
	{
		if (blockedWriters) {
			if (!SetEvent(writers_event))
				system_call_failed::raise("SetEvent");
		}
		else
			if (blockedReaders) {
				if (!ReleaseSemaphore(readers_semaphore, blockedReaders, NULL))
					system_call_failed::raise("ReleaseSemaphore");
			}
	}
	bool tryBeginRead()
	{
		if (lock < 0) return false;
		if (InterlockedIncrement_uni(&lock) > 0) return true;
		// We stepped on writer's toes. Fix our mistake 
		if (InterlockedDecrement_uni(&lock) == 0)
			unblockWaiting();
		return false;
	}
	bool tryBeginWrite()
	{
		if (lock) return false;
		if (InterlockedExchangeAdd_uni(&lock, -LOCK_WRITER_OFFSET) == 0) return true;
		// We stepped on somebody's toes. Fix our mistake
		if (InterlockedExchangeAdd_uni(&lock, LOCK_WRITER_OFFSET) == -LOCK_WRITER_OFFSET)
			unblockWaiting();
		return false;
	}
	void beginRead()
	{
		if (!tryBeginRead()) {
			InterlockedIncrement_uni(&blockedReaders);
			while (!tryBeginRead())
				if (WaitForSingleObject(readers_semaphore, INFINITE) != WAIT_OBJECT_0)
					system_call_failed::raise("WaitForSingleObject");
			InterlockedDecrement_uni(&blockedReaders); 
		}
	}
	void beginWrite()
	{
		if (!tryBeginWrite()) {
			InterlockedIncrement_uni(&blockedWriters);
			while (!tryBeginWrite())
				if (WaitForSingleObject(writers_event, INFINITE) != WAIT_OBJECT_0)
					system_call_failed::raise("WaitForSingleObject");
			InterlockedDecrement_uni(&blockedWriters);
		}
	}
	void endRead()
	{
		if (InterlockedDecrement_uni(&lock) == 0)
			unblockWaiting();
	}
	void endWrite()
	{
		if (InterlockedExchangeAdd_uni(&lock, LOCK_WRITER_OFFSET) == -LOCK_WRITER_OFFSET)
			unblockWaiting();
	}
};


}

#else

#ifdef MULTI_THREAD

#ifdef SOLARIS

#include <thread.h>
#include <synch.h>
#include <errno.h>

namespace Firebird
{

class RWLock
{
private:
	rwlock_t lock;
public:
	RWLock()
	{		
		if (rwlock_init(&lock, USYNC_PROCESS, NULL))
		{
			system_call_failed::raise("rwlock_init");
		}
	}
	~RWLock()
	{
		if (rwlock_destroy(&lock))
			system_call_failed::raise("rwlock_destroy");
	}
	void beginRead()
	{
		if (rw_rdlock(&lock))	
			system_call_failed::raise("rw_rdlock");
	}
	bool tryBeginRead()
	{
		const int code = rw_tryrdlock(&lock);
		if (code == EBUSY)
			return false;
		if (code)
			system_call_failed::raise("rw_tryrdlock");
		return true;
	}
	void endRead()
	{
		if (rw_unlock(&lock))	
			system_call_failed::raise("rw_unlock");
	}
	bool tryBeginWrite()
	{
		const int code = rw_trywrlock(&lock);
		if (code == EBUSY)
			return false;
		if (code)
			system_call_failed::raise("rw_trywrlock");
		return true;
	}
	void beginWrite()
	{
		if (rw_wrlock(&lock))	
			system_call_failed::raise("rw_wrlock");
	}
	void endWrite()
	{
		if (rw_unlock(&lock))	
			system_call_failed::raise("rw_unlock");
	}
};

} // namespace Firebird




#else

#include <pthread.h>
#include <errno.h>

namespace Firebird
{

class RWLock
{
private:
	pthread_rwlock_t lock;
public:
	RWLock() {		
#ifdef LINUX
		pthread_rwlockattr_t attr;
		if (pthread_rwlockattr_init(&attr))
			system_call_failed::raise("pthread_rwlockattr_init");
		if (pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP))
			system_call_failed::raise("pthread_rwlockattr_setkind_np");			
		if (pthread_rwlock_init(&lock, NULL))
			system_call_failed::raise("pthread_rwlock_init");
		if (pthread_rwlockattr_destroy(&attr))
			system_call_failed::raise("pthread_rwlockattr_destroy");
#else
		if (pthread_rwlock_init(&lock, NULL))
			system_call_failed::raise("pthread_rwlock_init");
#endif
	}
	~RWLock()
	{
		if (pthread_rwlock_destroy(&lock))
			system_call_failed::raise("pthread_rwlock_destroy");
	}
	void beginRead()
	{
		if (pthread_rwlock_rdlock(&lock))	
			system_call_failed::raise("pthread_rwlock_rdlock");
	}
	bool tryBeginRead()
	{
		const int code = pthread_rwlock_tryrdlock(&lock);
		if (code == EBUSY)
			return false;
		if (code)
			system_call_failed::raise("pthread_rwlock_tryrdlock");
		return true;
	}
	void endRead()
	{
		if (pthread_rwlock_unlock(&lock))	
			system_call_failed::raise("pthread_rwlock_unlock");
	}
	bool tryBeginWrite()
	{
		const int code = pthread_rwlock_trywrlock(&lock);
		if (code == EBUSY)
			return false;
		if (code)
			system_call_failed::raise("pthread_rwlock_trywrlock");
		return true;
	}
	void beginWrite()
	{
		if (pthread_rwlock_wrlock(&lock))	
			system_call_failed::raise("pthread_rwlock_wrlock");
	}
	void endWrite()
	{
		if (pthread_rwlock_unlock(&lock))	
			system_call_failed::raise("pthread_rwlock_unlock");
	}
};

} // namespace Firebird

#endif /*solaris*/
#endif /*MULTI_THREAD*/

#endif /*!WIN_NT*/

#endif // #ifndef RWLOCK_H

