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

#include "firebird.h"
#include "../common/gdsassert.h"
#include "fb_exception.h"

#include "SyncObject.h"
#include "Synchronize.h"

namespace Firebird {


void SyncObject::lock(Sync* sync, SyncType type, const char* from)
{
	ThreadSync* thread = NULL;

	if (type == SYNC_SHARED)
	{
		// In Vulcan SyncObject locking is not fair. Shared locks have priority
		// before Exclusive locks. If we'll need to restore this behavior we
		// should replace loop condition below by:
		// while (true)
		while (waiters == 0)
		{
			const AtomicCounter::counter_type oldState = lockState;
			if (oldState < 0)
				break;

			const AtomicCounter::counter_type newState = oldState + 1;
			if (lockState.compareExchange(oldState, newState))
			{
				WaitForFlushCache();
#ifdef DEV_BUILD
				MutexLockGuard g(mutex, FB_FUNCTION);
				reason(from);
#endif
				return;
			}
		}

		mutex.enter(FB_FUNCTION);
		++waiters;

		//while (true)
		while (!waitingThreads)
		{
			const AtomicCounter::counter_type oldState = lockState;
			if (oldState < 0)
				break;

			const AtomicCounter::counter_type newState = oldState + 1;
			if (lockState.compareExchange(oldState, newState))
			{
				--waiters;
				reason(from);
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
			reason(from);
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
				WaitForFlushCache();
				reason(from);
				return;
			}
		}

		mutex.enter(FB_FUNCTION);
		++waiters;

		while (!waitingThreads)
		{
			const AtomicCounter::counter_type oldState = lockState;
			if (oldState != 0)
				break;

			if (lockState.compareExchange(oldState, -1))
			{
				exclusiveThread = thread;
				--waiters;
				mutex.leave();
				reason(from);
				return;
			}
		}
	}

	wait(type, thread, sync);
#ifdef DEV_BUILD
	MutexLockGuard g(mutex, FB_FUNCTION);
	reason(from);
#endif
}

bool SyncObject::lockConditional(SyncType type, const char* from)
{
	if (waitingThreads)
		return false;

	if (type == SYNC_SHARED)
	{
		while (true)
		{
			const AtomicCounter::counter_type oldState = lockState;
			if (oldState < 0)
				break;

			const AtomicCounter::counter_type newState = oldState + 1;
			if (lockState.compareExchange(oldState, newState))
			{
				WaitForFlushCache();
#ifdef DEV_BUILD
				MutexLockGuard g(mutex, FB_FUNCTION);
				reason(from);
#endif
				return true;
			}
		}
	}
	else
	{
		ThreadSync* thread = ThreadSync::findThread();
		fb_assert(thread);

		if (thread == exclusiveThread)
		{
			++monitorCount;
			reason(from);
			return true;
		}

		while (waiters == 0)
		{
			const AtomicCounter::counter_type oldState = lockState;
			if (oldState != 0)
				break;

			if (lockState.compareExchange(oldState, -1))
			{
				WaitForFlushCache();
				exclusiveThread = thread;
				reason(from);
				return true;
			}
		}

	}

	return false;
}

void SyncObject::unlock(Sync* /*sync*/, SyncType type)
{
	fb_assert((type == SYNC_SHARED && lockState > 0) ||
			  (type == SYNC_EXCLUSIVE && lockState == -1));

	if (monitorCount)
	{
		fb_assert(monitorCount > 0);
		--monitorCount;
		return;
	}

	while (true)
	{
		const AtomicCounter::counter_type oldState = lockState;
		const AtomicCounter::counter_type newState = (type == SYNC_SHARED) ? oldState - 1 : 0;
		exclusiveThread = NULL;

		FlushCache();

		if (lockState.compareExchange(oldState, newState))
		{
			if (waiters)
				grantLocks();

			return;
		}
	}
}

void SyncObject::unlock()
{
	if (lockState > 0)
		unlock(NULL, SYNC_SHARED);
	else if (lockState == -1)
		unlock(NULL, SYNC_EXCLUSIVE);
	else
	{
		fb_assert(false);
	}
}

void SyncObject::downgrade(SyncType type)
{
	fb_assert(monitorCount == 0);
	fb_assert(type == SYNC_SHARED);
	fb_assert(lockState == -1);
	fb_assert(exclusiveThread);
	fb_assert(exclusiveThread == ThreadSync::findThread());

	FlushCache();

	while (true)
	{
		if (lockState.compareExchange(-1, 1))
		{
			if (waiters)
				grantLocks();

			return;
		}
	}
}

void SyncObject::wait(SyncType type, ThreadSync* thread, Sync* sync)
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

ThreadSync* SyncObject::grantThread(ThreadSync* thread)
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
	MutexLockGuard guard(mutex, "SyncObject::grantLocks");
	fb_assert((waiters && waitingThreads) || (!waiters && !waitingThreads));

	ThreadSync* thread = waitingThreads;

	while (thread)
	{
		bool granted = false;

		if (thread->lockType == SYNC_SHARED)
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

void SyncObject::validate(SyncType lockType) const
{
	switch (lockType)
	{
	case SYNC_NONE:
		fb_assert(lockState == 0);
		break;

	case SYNC_SHARED:
		fb_assert(lockState > 0);
		break;

	case SYNC_EXCLUSIVE:
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
