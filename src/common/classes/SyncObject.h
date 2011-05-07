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

#ifndef CLASSES_SYNCOBJECT_H
#define CLASSES_SYNCOBJECT_H

#include "../../common/classes/fb_atomic.h"
#include "../../common/classes/locks.h"

namespace Firebird {

#define SYNC_LOG_DEBUG

enum SyncType {
	SYNC_NONE,
	SYNC_EXCLUSIVE,
	SYNC_SHARED,
	SYNC_INVALID
};

class Sync;
class ThreadSync;

class SyncObject
{
public:
	SyncObject()
		: waiters(0),
		  monitorCount(0),
		  exclusiveThread(NULL),
		  waitingThreads(NULL)
	{
	}

	~SyncObject()
	{
	}

	void lock(Sync* sync, SyncType type);
	bool lockConditional(SyncType type);

	void unlock(Sync* sync, SyncType type);
	void unlock();

	void downgrade(SyncType type);

	SyncType getState() const
	{
		if (lockState.value() == 0)
			return SYNC_NONE;

		if (lockState.value() < 0)
			return SYNC_EXCLUSIVE;

		return SYNC_SHARED;
	}

	bool isLocked() const
	{
		return lockState.value() != 0;
	}

	bool hasContention() const
	{
		return (waiters.value() > 0);
	}

	bool ourExclusiveLock() const;

protected:
	void wait(SyncType type, ThreadSync* thread, Sync* sync);
	ThreadSync* grantThread(ThreadSync* thread);
	void grantLocks();
	void validate(SyncType lockType);

	AtomicCounter lockState;
	AtomicCounter waiters;
	//int waiters;
	int monitorCount;
	Mutex mutex;
	ThreadSync* volatile exclusiveThread;
	ThreadSync* volatile waitingThreads;
};


class Sync
{
friend class ThreadSync;

public:
	Sync(SyncObject* obj, const char* fromWhere)
		: state(SYNC_NONE),
		  request(SYNC_NONE),
		  syncObject(obj),
		  prior(NULL),
		  where(fromWhere)
	{
		fb_assert(obj);
	}

	~Sync()
	{
		fb_assert(state != SYNC_INVALID);

		if (syncObject && state != SYNC_NONE)
		{
			syncObject->unlock(this, state);
			state = SYNC_INVALID;
		}
	}

	void lock(SyncType type)
	{
		request = type;
		syncObject->lock(this, type);
		state = type;
	}

	void lock(SyncType type, const char* fromWhere)
	{
		where = fromWhere;
		lock(type);
	}

	void unlock()
	{
		fb_assert(state != SYNC_NONE);
		syncObject->unlock(this, state);
		state = SYNC_NONE;
	}

	void downgrade(SyncType type)
	{
		fb_assert(state == SYNC_EXCLUSIVE);
		syncObject->downgrade(type);
		state = SYNC_SHARED;
	}

	void setObject(SyncObject* obj)
	{
		if (syncObject && state != SYNC_NONE)
			syncObject->unlock(this, state);

		state = SYNC_NONE;
		syncObject = obj;
	}

protected:
	SyncType state;
	SyncType request;
	SyncObject* syncObject;
	Sync* prior;	// not used
	const char* where;
};


class SyncLockGuard : public Sync
{
public:
	SyncLockGuard(SyncObject* obj, SyncType type, const char* fromWhere)
		: Sync(obj, fromWhere)
	{
		lock(type);
	}

	~SyncLockGuard()
	{
		//fb_assert(state != SYNC_NONE);
		if (state != SYNC_NONE)
			unlock();
	}
};

class SyncUnlockGuard : public Sync
{
public:
	SyncUnlockGuard(SyncObject* obj, const char* fromWhere)
		: Sync(obj, fromWhere)
	{
		oldState = state;

		fb_assert(oldState != SYNC_NONE);
		if (oldState != SYNC_NONE)
			unlock();
	}

	~SyncUnlockGuard()
	{
		if (oldState != SYNC_NONE)
			lock(oldState);
	}

private:
	SyncType oldState;
};

} // namespace Firebird

#endif // CLASSES_SYNCOBJECT_H
