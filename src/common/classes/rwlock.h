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
 *  $Id: rwlock.h,v 1.18 2004-05-02 23:03:47 skidder Exp $
 *
 */

#ifndef CLASSES_RWLOCK_H
#define CLASSES_RWLOCK_H

#ifdef WIN_NT

#include <windows.h>
#include <limits.h>

#include "../common/classes/fb_atomic.h"


namespace Firebird
{

const int LOCK_WRITER_OFFSET = 50000;

// Should work pretty fast if atomic operations are native.
// This is not the case for Win95
class RWLock {
private:
	AtomicCounter lock; // This is the actual lock
	           // -50000 - writer is active
			   // 0 - noone owns the lock
			   // positive value - number of concurrent readers
	AtomicCounter blockedReaders;
	AtomicCounter blockedWriters;
	HANDLE writers_event, readers_semaphore;

	// Forbid copy constructor
	RWLock(RWLock& source);
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
		return lock.value();
	}
	void unblockWaiting()
	{
		if (blockedWriters.value()) {
			if (!SetEvent(writers_event))
				system_call_failed::raise("SetEvent");
		}
		else
			if (blockedReaders.value()) {
				if (!ReleaseSemaphore(readers_semaphore, blockedReaders.value(), NULL))
					system_call_failed::raise("ReleaseSemaphore");
			}
	}
	bool tryBeginRead()
	{
		if (lock.value() < 0)
			return false;
		if (++lock > 0)
			return true;
		// We stepped on writer's toes. Fix our mistake 
		if (--lock == 0)
			unblockWaiting();
		return false;
	}
	bool tryBeginWrite()
	{
		if (lock.value())
			return false;
		if (lock.exchangeAdd(-LOCK_WRITER_OFFSET) == 0)
			return true;
		// We stepped on somebody's toes. Fix our mistake
		if (lock.exchangeAdd(LOCK_WRITER_OFFSET) == -LOCK_WRITER_OFFSET)
			unblockWaiting();
		return false;
	}
	void beginRead()
	{
		if (!tryBeginRead()) {
			++blockedReaders;
			while (!tryBeginRead())
				if (WaitForSingleObject(readers_semaphore, INFINITE) != WAIT_OBJECT_0)
					system_call_failed::raise("WaitForSingleObject");
			--blockedReaders; 
		}
	}
	void beginWrite()
	{
		if (!tryBeginWrite()) {
			++blockedWriters;
			while (!tryBeginWrite())
				if (WaitForSingleObject(writers_event, INFINITE) != WAIT_OBJECT_0)
					system_call_failed::raise("WaitForSingleObject");
			--blockedWriters;
		}
	}
	void endRead()
	{
		if (--lock == 0)
			unblockWaiting();
	}
	void endWrite()
	{
		if (lock.exchangeAdd(LOCK_WRITER_OFFSET) == -LOCK_WRITER_OFFSET)
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
	// Forbid copy constructor
	RWLock(const RWLock& source);
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
	// Forbid copy constructor
	RWLock(const RWLock& source);
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

#else

namespace Firebird {

// Non-threaded version
class RWLock
{
private:
	// Forbid copy constructor
	RWLock(const RWLock& source);
public:
	RWLock() {		
	}
	~RWLock() {	}
	void beginRead() { }
	bool tryBeginRead() { return true; }
	void endRead() { }
	bool tryBeginWrite() { return true; }
	void beginWrite() {	}
	void endWrite() { }
};

}

#endif /*MULTI_THREAD*/

#endif /*!WIN_NT*/

namespace Firebird {

// RAII holder of read lock
class ReadLockGuard {
public:
	ReadLockGuard(RWLock &alock) : lock(&alock) { lock->beginRead(); }
	~ReadLockGuard() { lock->endRead(); };
private:
	// Forbid copy constructor
	ReadLockGuard(const ReadLockGuard& source);
	RWLock *lock;
};

// RAII holder of write lock
class WriteLockGuard {
public:
	WriteLockGuard(RWLock &alock) : lock(&alock) { lock->beginWrite(); }
	~WriteLockGuard() { lock->endWrite(); };
private:
	// Forbid copy constructor
	WriteLockGuard(const WriteLockGuard& source);
	RWLock *lock;
};

}

#endif // #ifndef CLASSES_RWLOCK_H

