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
 *  $Id: rwlock.h,v 1.8 2003-09-13 09:25:57 brodsom Exp $
 *
 */

#ifndef RWLOCK_H
#define RWLOCK_H

#ifdef WIN_NT

#include <windows.h>
#include <limits.h>

#define LOCK_WRITER_OFFSET 50000

namespace Firebird {

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
	inline LONG InterlockedIncrement_uni(volatile LONG* lock_p){
		return InterlockedIncrement(const_cast<LONG*>(lock_p));
	}
	inline LONG InterlockedDecrement_uni(volatile LONG* lock_p){
		return InterlockedDecrement(const_cast<LONG*>(lock_p));
	}
	inline LONG InterlockedExchangeAdd_uni(volatile LONG* lock_p, LONG value){
		return InterlockedExchangeAdd(const_cast<LONG*>(lock_p), value);
	}
#else
	inline LONG InterlockedIncrement_uni(volatile LONG* lock_p){
		return InterlockedIncrement(lock_p);
	}
	inline LONG InterlockedDecrement_uni(volatile LONG* lock_p){
		return InterlockedDecrement(lock_p);
	}
	inline LONG InterlockedExchangeAdd_uni(volatile LONG* lock_p, LONG value){
		return InterlockedExchangeAdd(lock_p, value);
	}
#endif

public:
	RWLock() : lock(0), blockedReaders(0), blockedWriters(0) { 
		readers_semaphore = CreateSemaphore(NULL, 0 /*initial count*/, 
			INT_MAX, NULL); 
		writers_event = CreateEvent(NULL, FALSE/*auto-reset*/, FALSE, NULL);
	}
	~RWLock() { CloseHandle(readers_semaphore); CloseHandle(writers_event); }
	// Returns negative value if writer is active.
	// Otherwise returns a number of readers
	LONG getState() {
		return lock;
	}
	void unblockWaiting() {
		if (blockedWriters) 
			SetEvent(writers_event);
		else
			if (blockedReaders)
				ReleaseSemaphore(readers_semaphore, blockedReaders, NULL);
	}
	bool tryBeginRead() {
		if (lock < 0) return false;
		if (InterlockedIncrement_uni(&lock) > 0) return true;
		// We stepped on writer's toes. Fix our mistake 
		if (InterlockedDecrement_uni(&lock) == 0)
			unblockWaiting();
		return false;
	}
	bool tryBeginWrite() {
		if (lock) return false;
		if (InterlockedExchangeAdd_uni(&lock, -LOCK_WRITER_OFFSET) == 0) return true;
		// We stepped on somebody's toes. Fix our mistake
		if (InterlockedExchangeAdd_uni(&lock, LOCK_WRITER_OFFSET) == -LOCK_WRITER_OFFSET)
			unblockWaiting();
		return false;
	}
	void beginRead() {
		if (!tryBeginRead()) {
			InterlockedIncrement_uni(&blockedReaders);
			while (!tryBeginRead())
				WaitForSingleObject(readers_semaphore, INFINITE);
			InterlockedDecrement_uni(&blockedReaders); 
		}
	}
	void beginWrite() {
		if (!tryBeginWrite()) {
			InterlockedIncrement_uni(&blockedWriters);
			while (!tryBeginWrite())
				WaitForSingleObject(writers_event, INFINITE);
			InterlockedDecrement_uni(&blockedWriters);
		}
	}
	void endRead() {
		if (InterlockedDecrement_uni(&lock) == 0)
			unblockWaiting();
	}
	void endWrite() {
		if (InterlockedExchangeAdd_uni(&lock, LOCK_WRITER_OFFSET) == -LOCK_WRITER_OFFSET)
			unblockWaiting();
	}
};


}

#else

#ifdef MULTI_THREAD

#include <pthread.h>
#include <errno.h>

namespace Firebird {

class RWLock {
private:
	pthread_rwlock_t lock;
public:
	RWLock() {		
#ifdef LINUX
		pthread_rwlockattr_t attr;
		if (pthread_rwlockattr_init(&attr) ||
			pthread_rwlockattr_setkind_np(&attr, 
				PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP) ||
			pthread_rwlock_init(&lock, NULL) ||
			pthread_rwlockattr_destroy(&attr) )
		{
			system_call_failed::raise();
		}
#else
		if (pthread_rwlock_init(&lock, NULL))
		{
			system_call_failed::raise();
		}
#endif
	}
	~RWLock() {
		if (pthread_rwlock_destroy(&lock))
			system_call_failed::raise();
	}
	void beginRead() {
		if (pthread_rwlock_rdlock(&lock))	
			system_call_failed::raise();
	}
	bool tryBeginRead() {
		int code = pthread_rwlock_tryrdlock(&lock);
		if (code == EBUSY) return false;
		if (code) system_call_failed::raise();
		return true;
	}
	void endRead() {
		if (pthread_rwlock_unlock(&lock))	
			system_call_failed::raise();
	}
	bool tryBeginWrite() {
		int code = pthread_rwlock_trywrlock(&lock);
		if (code == EBUSY) return false;
		if (code) system_call_failed::raise();
		return true;
	}
	void beginWrite() {
		if (pthread_rwlock_wrlock(&lock))	
			system_call_failed::raise();
	}
	void endWrite() {
		if (pthread_rwlock_unlock(&lock))	
			system_call_failed::raise();
	}
};

}

#endif /*MULTI_THREAD*/

#endif /*!WIN_NT*/

#endif
