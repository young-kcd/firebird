/*
 *  
 *     The contents of this file are subject to the Initial 
 *     Developer's Public License Version 1.0 (the "License"); 
 *     you may not use this file except in compliance with the 
 *     License. You may obtain a copy of the License at 
 *     http://www.ibphoenix.com/idpl.html. 
 *
 *     Software distributed under the License is distributed on 
 *     an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either 
 *     express or implied.  See the License for the specific 
 *     language governing rights and limitations under the License.
 *
 *     The contents of this file or any work derived from this file
 *     may not be distributed under any other license whatsoever 
 *     without the express prior written permission of the original 
 *     author.
 *
 *
 *  The Original Code was created by James A. Starkey for IBPhoenix.
 *
 *  Copyright (c) 1997 - 2000, 2001, 2003 James A. Starkey
 *  Copyright (c) 1997 - 2000, 2001, 2003 Netfrastructure, Inc.
 *  All Rights Reserved.
 *
 *  The Code was ported into Firebird Open Source RDBMS project by
 *  Vladyslav Khorsun at 2010
 *
 *  Contributor(s):
 */

#include "../../common/common.h"
#include "../../common/gdsassert.h"
#include "fb_exception.h"

#include "SyncObject.h"
#include "Synchronize.h"
#include "Interlock.h"

namespace Firebird {


void SyncObject::lock(Sync *sync, LockType type)
{
	ThreadSync *thread = NULL;

	if (type == Shared)
	{
		// In Vulcan SyncObject locking is not fair. Shared locks have priority
		// before Exclusive locks. If we'll need to restore this behavior we
		// should replace loop condition below by :
		// while (true)
		while (waiters == 0)
		{
			const AtomicCounter::counter_type oldState = lockState;
			if (oldState < 0)
				break;

			const AtomicCounter::counter_type newState = oldState + 1;
			if (lockState.compareExchange(oldState, newState))
			{
				WAIT_FOR_FLUSH_CACHE
				return;
			}
		}

		mutex.enter();
		++waiters;

		//while (true)
		while (waitingThreads == NULL)
		{
			const AtomicCounter::counter_type oldState = lockState;
			if (oldState < 0)
				break;

			const AtomicCounter::counter_type newState = oldState + 1;
			if (lockState.compareExchange(oldState, newState))
			{
				--waiters;
				mutex.leave();
				return;
			}
		}

		thread = ThreadSync::findThread();
		fb_assert(thread);
	}
	else
	{
		thread = ThreadSync::findThread();
		fb_assert(thread);

		if (thread == exclusiveThread)
		{
			++monitorCount;
			return;
		}

		while (waiters == 0)
		{
			const AtomicCounter::counter_type oldState = lockState;
			if (oldState != 0)
				break;

			if (lockState.compareExchange(oldState, -1))
			{
				exclusiveThread = thread;
				WAIT_FOR_FLUSH_CACHE
				return; 
			}
		}

		mutex.enter();
		++waiters;

		while (waitingThreads == NULL)
		{
			const AtomicCounter::counter_type oldState = lockState;
			if (oldState != 0)
				break;

			if (lockState.compareExchange(oldState, -1))
			{
				exclusiveThread = thread;
				--waiters;
				mutex.leave();
				return;
			}
		}
	}

	wait(type, thread, sync);
}

bool SyncObject::lockConditional(LockType type)
{
	if (waitingThreads)
		return false;

	if (type == Shared)
	{
		while (true)
		{
			const AtomicCounter::counter_type oldState = lockState;
			if (oldState < 0)
				break;

			const AtomicCounter::counter_type newState = oldState + 1;
			if (lockState.compareExchange(oldState, newState))
			{
				WAIT_FOR_FLUSH_CACHE
				return true;
			}
		}

		return false;	
	}
	else
	{
		ThreadSync *thread = ThreadSync::findThread();
		fb_assert(thread);

		if (thread == exclusiveThread)
		{
			++monitorCount;
			return true;
		}

		while (waiters == 0)
		{
			const AtomicCounter::counter_type oldState = lockState;
			if (oldState != 0)
				break;

			if (lockState.compareExchange(oldState, -1))
			{
				WAIT_FOR_FLUSH_CACHE
				exclusiveThread = thread;
				return true; 
			}
		}

		return false;
	}
}

void SyncObject::unlock(Sync *sync, LockType type)
{
	fb_assert((type == Shared && lockState > 0) || 
			(type == Exclusive && lockState == -1));

	if (monitorCount)
	{
		fb_assert(monitorCount > 0);
		--monitorCount;
		return;
	}

	while (true)
	{
		const AtomicCounter::counter_type oldState = lockState;
		const AtomicCounter::counter_type newState = (type == Shared) ? oldState - 1 : 0;
		exclusiveThread = NULL;

		FLUSH_CACHE

		if (lockState.compareExchange(oldState, newState))
		{
			if (waiters) {
				grantLocks();
			}
			return;
		}
	}
}

void SyncObject::unlock()
{
	if (lockState > 0) {
		unlock(NULL, Shared);
	}
	else if (lockState == -1) {
		unlock(NULL, Exclusive);
	}
	else {
		fb_assert(false);
	}
}

void SyncObject::downgrade(LockType type)
{
	fb_assert(monitorCount == 0);
	fb_assert(type == Shared);
	fb_assert(lockState == -1);
	fb_assert(exclusiveThread);
	fb_assert(exclusiveThread == ThreadSync::findThread());

	FLUSH_CACHE

	while (true) 
	{
		if (lockState.compareExchange(-1, 1))
		{
			if (waiters) {
				grantLocks();
			}
			return;
		}
	}
}

void SyncObject::wait(LockType type, ThreadSync *thread, Sync *sync)
{
	if (thread->nextWaiting)
	{
		mutex.leave();
		fatal_exception::raise("single thread deadlock");
	}

	if (waitingThreads)
	{
		thread->prevWaiting = waitingThreads->prevWaiting;
		thread->nextWaiting = waitingThreads;

		waitingThreads->prevWaiting->nextWaiting = thread;
		waitingThreads->prevWaiting = thread;
	}
	else
	{
		thread->prevWaiting = thread->nextWaiting = thread;
		waitingThreads = thread;
	}

	thread->lockType = type;
	thread->lockGranted = false;
	thread->lockPending = sync;
	mutex.leave();

	while (!thread->lockGranted)
	{
		const bool wakeup = thread->sleep(10000);
		if (thread->lockGranted)
			break;

		if (!wakeup)
		{
			// stalled(thread);
			break;
		}
	}

	while (!thread->lockGranted)
		thread->sleep();
}

ThreadSync* SyncObject::grantThread(ThreadSync *thread)
{
	ThreadSync* next = NULL;
	if (thread == thread->nextWaiting)
	{
		thread->nextWaiting = thread->prevWaiting = NULL;
		waitingThreads = NULL;
	}
	else
	{
		next = thread->nextWaiting;

		thread->prevWaiting->nextWaiting = next;
		next->prevWaiting = thread->prevWaiting;

		thread->nextWaiting = thread->prevWaiting = NULL;
		if (waitingThreads == thread) 
			waitingThreads = next;
	}

	--waiters;
	thread->grantLock(this);
	return next;
}

void SyncObject::grantLocks()
{
	MutexLockGuard guard(mutex);
	fb_assert((waiters && waitingThreads) || (!waiters && !waitingThreads));

	ThreadSync *thread = waitingThreads;
	while (thread)
	{
		bool granted = false;

		if (thread->lockType == Shared)
		{
			AtomicCounter::counter_type oldState = lockState;
			while (oldState >= 0)
			{
				const AtomicCounter::counter_type newState = oldState + 1;
				if (lockState.compareExchange(oldState, newState))
				{
					granted = true;
					thread = grantThread(thread);
					break;
				}
				oldState = lockState;
			}
		}
		else
		{
			while (lockState == 0)
			{
				if (lockState.compareExchange(0, -1))
				{
					granted = true;
					exclusiveThread = thread;
					thread = grantThread(thread);
					break;
				}
			}
		}

		if (!granted) {
			break;
		}
	}
}


void SyncObject::validate(LockType lockType)
{
	switch (lockType)
	{
	case None:
		fb_assert(lockState == 0);
		break;

	case Shared:
		fb_assert(lockState > 0);
		break;

	case Exclusive:
		fb_assert(lockState == -1);
		break;
	}
}


bool SyncObject::ourExclusiveLock() const
{
	if (lockState != -1)
		return false;

	return (exclusiveThread == ThreadSync::findThread());
}

} // namespace Firebird
