/*
 *	PROGRAM:		Client/Server Common Code
 *	MODULE:			rwlock.h
 *	DESCRIPTION:	Read/write multi-state locks
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
 * Created by: Nickolay Samofatov <skidder@bssys.com>
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#ifndef RWLOCK_H
#define RWLOCK_H

#ifdef WIN_NT

#include <windows.h>

#define LOCK_WRITER_OFFSET 50000

namespace Firebird {

// This class works on Windows 98/NT4 or later. Win95 is not supported
// Should work pretty fast.
class RWLock {
private:
	LONG lock; // This is the actual lock
	           // -50000 - writer is active
			   // 0 - noone owns the lock
			   // positive value - number of concurrent readers
	LONG blockedReaders;
	LONG blockedWriters;
	HANDLE writers_event, readers_semaphore;
public:
	RWLock() : lock(0) { 
		readers_semaphore = CreateSemaphore(NULL, 0 /*initial count*/, 
			1<<(sizeof(LONG)*8-1)-1 /* maximum count = MAX_LONG */, NULL); 
		writers_event = CreateEvent(NULL, FALSE/*auto-reset*/, FALSE, NULL);
	}
	~RWLock() { CloseHandle(readers_event); CloseHandle(writers_event); }
	void unblockWaiting() {
		if (blockedWriters) 
			SetEvent(writers_event);
		else
			if (blockedReaders)
				ReleaseSemaphore(readers_semaphore, blockedReaders, NULL);
	}
	bool tryBeginRead() {
		if (lock < 0) return false;
		if (InterlockedIncrement(&lock) > 0) return true;
		// We stepped on writer's toes. Fix our mistake 
		if (InterlockedDecrement(&lock) == 0)
			unblockWaiting();
		return false;
	}
	bool tryBeginWrite() {
		if (lock) return false;
		if (InterlockedExchangeAdd(&lock, -LOCK_WRITER_OFFSET) == 0) return true;
		// We stepped on somebody's toes. Fix our mistake
		if (InterlockedExchangeAdd(&lock, LOCK_WRITER_OFFSET) == -LOCK_WRITER_OFFSET)
			unblockWaiting();
		return false;
	}
	void beginRead() {
		if (!tryBeginRead()) {
			InterlockedIncrement(&blockedReaders);
			while (!tryBeginRead())
				WaitForSingleObject(readers_semaphore, INFINITE);
			InterlockedDecrement(&blockedReaders); 
		}
	}
	void beginWrite() {
		if (!tryBeginWrite()) {
			InterlockedIncrement(&blockedWriters);
			while (!tryBeginWrite())
				WaitForSingleObject(writers_event, INFINITE);
			InterlockedDecrement(&blockedWriters);
		}
	}
	void endRead() {
		if (InterlockedDecrement(&lock) == 0)
			unblockWaiting();
	}
	void endWrite() {
		if (InterlockedExchangeAdd(&lock, LOCK_WRITER_OFFSET) == -LOCK_WRITER_OFFSET)
			unblockWaiting();
	}
};

}

#else

#ifdef SUPERSERVER

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

#endif /*SUPERSERVER*/

#endif /*!WIN_NT*/

#endif
