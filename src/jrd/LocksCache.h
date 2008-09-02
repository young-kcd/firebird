/*
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
 *  The Original Code was created by Vlad Khorsun
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2008 Vlad Khorsun
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef LOCKSCACHE_H
#define LOCKSCACHE_H

#include "firebird.h"
#include "fb_types.h"
#include "../common/classes/alloc.h"

#include "../jrd/GlobalRWLock.h"
#include "../jrd/lck.h"
#include "../jrd/que.h"

namespace Jrd {

struct KeyHolder
{
	KeyHolder(const void* key, size_t size)
	{
		m_key = key;
		m_size = size;
	}

	static bool greaterThan(const KeyHolder &i1, const KeyHolder &i2) {
		fb_assert(i1.m_size == i2.m_size);
		return memcmp(i1.m_key, i2.m_key, i1.m_size) > 0;
	}

	const void*	m_key;
	size_t	m_size;
};

class CachedLock : public GlobalRWLock
{
public:
	CachedLock(thread_db* tdbb, Firebird::MemoryPool& p, locktype_t lckType, 
		size_t lockLen, const UCHAR* lockStr) :
	  GlobalRWLock(tdbb, p, lckType, lockLen, lockStr)
	{
		QUE_INIT(m_lru);
	}


	const UCHAR* getLockKey() const 
	{
		fb_assert(cached_lock);
		return reinterpret_cast<UCHAR*> (&cached_lock->lck_key);
	}

	void setLockKey(thread_db *tdbb, const UCHAR* key) 
	{
		while (!tryReleaseLock(tdbb))
		{
			lock(tdbb, LCK_write, LCK_WAIT);
			unlock(tdbb, LCK_write);
		}
		
		memcpy(&cached_lock->lck_key, key, cached_lock->lck_length);
	}

	static const KeyHolder generate(const void*, const CachedLock* lock) { 
		return KeyHolder(lock->getLockKey(), lock->cached_lock->lck_length); 
	}

	static bool greaterThan(const KeyHolder &i1, const KeyHolder &i2) {
		return KeyHolder::greaterThan(i1, i2);
	}

	que m_lru;
};


// LockClass must be descendant of CachedLock
template <class LockClass = CachedLock>
class LocksCache
{
public:
	LocksCache(Jrd::thread_db *tdbb, Jrd::lck_t lockType, size_t lockLen, size_t maxCapacity);
	~LocksCache();

	GlobalRWLock* get(thread_db *tdbb, const UCHAR* key);

private:
	Firebird::MemoryPool &m_pool;
	que		m_lru;
	lck_t	m_lockType;
	size_t	m_lockLen;
	size_t	m_capacity;

	Firebird::SortedArray<LockClass*, Firebird::EmptyStorage<LockClass*>, 
		const KeyHolder, LockClass, LockClass> m_sortedLocks;
};

// implementation

template <class LockClass>
LocksCache<LockClass>::LocksCache(thread_db *tdbb, lck_t lockType, size_t lockLen, size_t maxCapacity) :
	m_pool(*tdbb->getDatabase()->dbb_permanent),
	m_sortedLocks(m_pool, maxCapacity)
{
	QUE_INIT(m_lru);
	m_lockType = lockType;
	m_lockLen  = lockLen;
	m_capacity = maxCapacity;
}

template <class LockClass>
LocksCache<LockClass>::~LocksCache()
{
	while (QUE_NOT_EMPTY(m_lru))
	{
		QUE que_inst = m_lru.que_forward;
		QUE_DELETE((*que_inst));
		LockClass *lock = (LockClass*) ((SCHAR*) que_inst - OFFSET (LockClass*, m_lru));
		delete lock;
	}
}

template <class LockClass>
GlobalRWLock* LocksCache<LockClass>::get(thread_db *tdbb, const UCHAR* key)
{
	LockClass* lock = NULL;
	size_t pos;
	if (m_sortedLocks.find(KeyHolder(key, m_lockLen), pos))
	{
		lock = m_sortedLocks[pos];
		QUE_DELETE(lock->m_lru);
	}
	else
	{
		if (m_sortedLocks.getCount() < m_capacity) {
			lock = FB_NEW (m_pool) LockClass(tdbb, m_pool, m_lockType, m_lockLen, key);
		}
		else
		{
			QUE que_inst = m_lru.que_backward;
			QUE_DELETE((*que_inst));
			lock = (LockClass*) ((SCHAR*) que_inst - OFFSET (LockClass*, m_lru));

			bool found = (m_sortedLocks.find(KeyHolder(lock->getLockKey(), m_lockLen), pos));
			fb_assert(found);

			m_sortedLocks.remove(pos);
			lock->setLockKey(tdbb, key);

			found = (m_sortedLocks.find(KeyHolder(key, m_lockLen), pos));
			fb_assert(!found);
		}
		m_sortedLocks.insert(pos, lock);
	}
	QUE_INSERT(m_lru, lock->m_lru);

	return lock;
}

}; // namespace Jrd


#endif // LOCKSCACHE_H
