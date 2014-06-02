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
	  GlobalRWLock(tdbb, p, lckType, lockLen, lockStr),
	  m_use_count(0),
	  m_chgKey(NULL)
	{
		QUE_INIT(m_lru);
	}


	const UCHAR* getLockKey() const 
	{
		fb_assert(cached_lock);
		return reinterpret_cast<UCHAR*> (&cached_lock->lck_key);
	}

	bool setLockKey(thread_db *tdbb, const UCHAR* key) 
	{
		m_chgKey = key;
		if (!tryReleaseLock(tdbb))
		{
			m_chgKey = NULL;
			return false;
		}

		memcpy(&cached_lock->lck_key, key, cached_lock->lck_length);
		m_chgKey = NULL;
		return true;
	}

	void addRef()
	{
		m_use_count++;
	}

	void release()
	{
		m_use_count--;
	}

	bool isUsed()
	{
		return m_use_count;
	}

	static const KeyHolder generate(const void*, const CachedLock* lock) { 
		return KeyHolder(lock->getLockKey(), lock->cached_lock->lck_length); 
	}

	static bool greaterThan(const KeyHolder &i1, const KeyHolder &i2) {
		return KeyHolder::greaterThan(i1, i2);
	}

	int m_use_count;
	que m_lru;
	const void* m_chgKey;
};


// LockClass must be descendant of CachedLock
template <class LockClass = CachedLock>
class LocksCache
{
public:
	LocksCache(thread_db *tdbb, Jrd::lck_t lockType, size_t lockLen, size_t maxCapacity);
	~LocksCache();

	CachedLock* get(thread_db *tdbb, const UCHAR* key);

private:
	Firebird::MemoryPool &m_pool;
	que		m_lru;
	que		m_changing;
	lck_t	m_lockType;
	size_t	m_lockLen;
	size_t	m_capacity;
	size_t	m_allocated;

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
	QUE_INIT(m_changing);
	m_lockType = lockType;
	m_lockLen  = lockLen;
	m_capacity = maxCapacity;
	m_allocated = 0;
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
CachedLock* LocksCache<LockClass>::get(thread_db *tdbb, const UCHAR* key)
{
	int tries = MIN(m_capacity / 2, 16);
	while (true)
	{
		LockClass* lock = NULL;
		size_t pos;
		if (m_sortedLocks.find(KeyHolder(key, m_lockLen), pos))
		{
			lock = m_sortedLocks[pos];
			if (lock->m_chgKey)
			{
				ThreadExit te;
				THREAD_YIELD();
				continue;
			}
			QUE_DELETE(lock->m_lru);
			QUE_INSERT(m_lru, lock->m_lru);
			return lock;
		}

		bool changing = false;
		QUE que_inst = m_changing.que_forward;
		for (; que_inst != &m_changing; que_inst = que_inst->que_forward)
		{
			LockClass* chgLock = (LockClass*) ((SCHAR*) que_inst - OFFSET (LockClass*, m_lru));
			fb_assert(chgLock->m_chgKey);

			if (memcmp(chgLock->m_chgKey, key, m_lockLen) == 0)
			{
				changing = true;
				{
					ThreadExit te;
					THREAD_YIELD();
				}
				que_inst = m_changing.que_forward;
				continue;
			}
		}
		if (changing)
			continue;

		if (m_allocated < m_capacity || !tries) 
		{
			lock = FB_NEW (m_pool) LockClass(tdbb, m_pool, m_lockType, m_lockLen, key);

			m_allocated++;
			if (!tries) {
				m_capacity++;
			}

			QUE_INSERT(m_lru, lock->m_lru);
			m_sortedLocks.insert(pos, lock);
			return lock;
		}

		// We going to change key of the least recently used lock.
		// To prevent someone from acquire this lock while we not in 
		// sheduler (inside setLockKey() below) we should remove 
		// our lock from internal structures first and only then try 
		// to change its key

		if (QUE_EMPTY(m_lru))
		{
			tries--;
			ThreadExit te;
			THREAD_YIELD();
			continue;
		}

		que_inst = m_lru.que_backward;
		while (tries && que_inst != &m_lru)
		{
			lock = (LockClass*) ((SCHAR*) que_inst - OFFSET (LockClass*, m_lru));
			if (!lock->isUsed())
				break;

			lock = NULL;
			que_inst = que_inst->que_backward;
			tries--;
		}
		if (!lock)
			continue;

		bool found = (m_sortedLocks.find(KeyHolder(lock->getLockKey(), m_lockLen), pos));
#ifdef _DEBUG
		if (!found) {
			DebugBreak();
		}
#endif
		fb_assert(found);

		QUE_DELETE(lock->m_lru);
		m_sortedLocks.remove(pos);

		QUE_INSERT(m_changing, lock->m_lru);

		if (lock->setLockKey(tdbb, key)) 
		{
			found = (m_sortedLocks.find(KeyHolder(lock->getLockKey(), m_lockLen), pos));
#ifdef _DEBUG
			if (found) {
				DebugBreak();
			}
#endif
			fb_assert(!found);

			// remove from changing que
			QUE_DELETE(lock->m_lru);

			QUE_INSERT(m_lru, lock->m_lru);
			m_sortedLocks.insert(pos, lock);
			return lock;
		}

		tries--;

		found = (m_sortedLocks.find(KeyHolder(lock->getLockKey(), m_lockLen), pos));
		if (found)
		{
#ifdef _DEBUG
			DebugBreak();
#endif
		}
		else
		{
			// remove from changing que
			QUE_DELETE(lock->m_lru);

			// move busy lock to the head of LRU queue
			QUE_INSERT(m_lru, lock->m_lru);

			// and put it back to the sorted array
			m_sortedLocks.insert(pos, lock);
		}
		fb_assert(!found);
	}
}

}; // namespace Jrd


#endif // LOCKSCACHE_H
