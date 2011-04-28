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

#define LOG_DEBUG

enum LockType {
	None,
	Exclusive,
	Shared,
	Invalid
};

class Sync;
class ThreadSync;

class SynchronizationObject
{
public:
	virtual void lock(Sync *sync, LockType type) = 0;
	virtual void unlock(Sync *sync, LockType type) = 0;
	virtual void downgrade(LockType type) = 0;
};

class SyncObject : public SynchronizationObject
{
public:
	SyncObject()
	{
		waiters = 0;
		monitorCount = 0;
		exclusiveThread = NULL;
		waitingThreads = NULL;
	}

	virtual ~SyncObject()
	{}

	virtual void lock(Sync *sync, LockType type);
	bool lockConditional(LockType type);

	virtual void unlock(Sync *sync, LockType type);
	void unlock();

	virtual void downgrade(LockType type);

	LockType getState() const
	{
		if (lockState.value() == 0)
			return None;

		if (lockState.value() < 0)
			return Exclusive;

		return Shared;
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

	void sysServiceFailed(const char* service, int code);
	void assertionFailed();

protected:
	void wait(LockType type, ThreadSync *thread, Sync *sync);
	ThreadSync* grantThread(ThreadSync *thread);
	void grantLocks();
	void validate(LockType lockType);

	AtomicCounter	lockState;
	AtomicCounter	waiters;
	//int			waiters;
	int				monitorCount;
	Mutex			mutex;
	ThreadSync		*volatile exclusiveThread;
	ThreadSync		*volatile waitingThreads;
};


class Sync  
{
public:
	Sync(SynchronizationObject *obj, const char *fromWhere)
	{
		fb_assert(obj);
		syncObject = obj;
		prior = NULL;
		where = fromWhere;
		state = request = None;
	}

	~Sync()
	{
		fb_assert(state != Invalid);

		if (syncObject && state != None)
		{
			syncObject->unlock(this, state);
			state = Invalid;
		}
	}

	void lock(LockType type)
	{
		request = type;
		syncObject->lock(this, type);
		state = type;
	}

	void lock(LockType type, const char *fromWhere)
	{
		where = fromWhere;
		lock(type);
	}

	void unlock()
	{
		fb_assert(state != None);
		syncObject->unlock(this, state);
		state = None;
	}

	void downgrade(LockType type)
	{
		fb_assert(state == Exclusive);
		syncObject->downgrade(type);
		state = Shared;
	}

	void setObject(SynchronizationObject *obj)
	{
		if (syncObject && state != None)
			syncObject->unlock(this, state);

		state = None;
		syncObject = obj;
	}


protected:
	LockType	state;
	LockType	request;
	SynchronizationObject	*syncObject;
	Sync		*prior;	// not used
	const char	*where;

friend class ThreadSync;
};


class SyncLockGuard : public Sync
{
public:
	SyncLockGuard(SynchronizationObject *obj, LockType type, const char *fromWhere) :
	  Sync(obj, fromWhere)
	{
		lock(type);
	}

	~SyncLockGuard()
	{
		//fb_assert(state != None);
		if (state != None)
			unlock();
	}
};

class SyncUnlockGuard : public Sync
{
public:
	SyncUnlockGuard(SynchronizationObject *obj, const char *fromWhere) :
	  Sync(obj, fromWhere)
	{
		oldState = state;

		fb_assert(oldState != None);
		if (oldState != None)
			unlock();
	}

	~SyncUnlockGuard()
	{
		if (oldState != None)
			lock(oldState);
	}

private:
	LockType oldState;
};

} // namespace Firebird

#endif // CLASSES_SYNCOBJECT_H

