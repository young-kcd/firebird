/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		GlobalRWLock.h
 *	DESCRIPTION:	Cached Object Synchronizer
 *
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
 *  The Original Code was created by Nickolay Samofatov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2006 Nickolay Samofatov
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#ifndef GLOBALRWLOCK_H
#define GLOBALRWLOCK_H

#include "../common/classes/alloc.h"
#include "../jrd/jrd.h"
#include "../jrd/lck.h"
#include "../jrd/lck_proto.h"
#include "../include/fb_types.h"
#include "../jrd/isc.h"
#include "os/pio.h"

//#define COS_DEBUG

#ifdef COS_DEBUG
DEFINE_TRACE_ROUTINE(cos_trace);
#define COS_TRACE(args) cos_trace args
#define COS_TRACE_AST(message) gds__trace(message)
#else
#define COS_TRACE(args) /* nothing */
#define COS_TRACE_AST(message) /* nothing */
#endif


namespace Jrd {

typedef USHORT locktype_t;

struct ObjectOwnerData {
	SLONG owner_handle;
	ULONG entry_count;
	static const SLONG& generate(const void* sender, const ObjectOwnerData& value) {
		return value.owner_handle;
	}
	ObjectOwnerData() {
		owner_handle = 0;
		entry_count = 0;
	}
};

/*
 * Architecture goals for the class
 * - Lock to protect intra-process cached resources with object-oriented interface:
 *   invalidate()/fetch()
 * - Two lock modes: LCK_read (LCK_PR) and LCK_write (LCK_EX)
 * - Support for lock recursion (multiple acquires of a lock by a given owner)
 * - Flexible execution environment
 * - Multiple threads
 * - Multiple processes
 * - Signals
 * - Locks belong to logical owners (typically type ATTACHMENT or DATABASE, but
 *   potentially also THREAD and PROCESS)
 * - Logical ownership of a lock may change during the object access lifecycle
 *   (somewhat special case, happens if cached resource needs to be passed from
 *   one worker thread to another without releasing the lock)
 *
 * Implementation constraints
 * - Avoid calling lock manager for synchronization in non-contention case
 *   (for performance reasons, especially in DLM environments)
 * - All contention to be handled via Lock manager to ensure reliable deadlock
 *   detection and to be monitored and debuggable via standard means
 */
class GlobalRWLock : public Firebird::PermanentStorage {
public:
	GlobalRWLock(thread_db* tdbb, MemoryPool& p, locktype_t lckType, 
					   size_t lockLen, const UCHAR* lockStr,
					   lck_owner_t physical_lock_owner = LCK_OWNER_database,
					   lck_owner_t default_logical_lock_owner = LCK_OWNER_attachment,
					   bool lock_caching = true);

	virtual ~GlobalRWLock();

	// As usual, 
	// wait = 0 - try to lock a thing instantly (doesn't send ASTs)
	// wait < 0 - timeout in seconds (doesn't deadlock)
	// wait > 0 - infinite wait (may deadlock)
	//
	// This function returns false if it cannot take the lock
	bool lock(thread_db* tdbb, locklevel_t level, SSHORT wait, SLONG owner_handle);
	bool lock(thread_db* tdbb, locklevel_t level, SSHORT wait) {
		return lock(tdbb, level, wait, LCK_get_owner_handle_by_type(tdbb, defaultLogicalLockOwner));
	}

	// NOTE: unlock method must be signal safe
	// This function may be called in AST. The function doesn't wait.
	void unlock(thread_db* tdbb, locklevel_t level, SLONG owner_handle);
	void unlock(thread_db* tdbb, locklevel_t level) {
		unlock(tdbb, level, LCK_get_owner_handle_by_type(tdbb, defaultLogicalLockOwner));
	}

	// Change the lock owner. The function doesn't wait.
	void changeLockOwner(thread_db* tdbb, locklevel_t level, SLONG old_owner_handle, SLONG new_owner_handle);

	SLONG getLockData() const {
		return cached_lock->lck_data;
	}
	void setLockData(SLONG lck_data);

	// Release phisical lock if possible. Use to force refetch
	// Returns true if lock was released
	bool tryReleaseLock(thread_db* tdbb);
protected:
	Lock* cached_lock;
	// Flag to indicate that somebody is waiting via lock manager.
	// If somebody uses lock manager, all concurrent requests should also
	// go via lock manager to prevent starvation.
	int internal_blocking;
	bool external_blocking; // Unprocessed AST pending

	// Load the object from shared location.
	virtual void fetch(thread_db* tdbb) {}

	// May be called under AST. Should not throw exceptions.
	virtual void invalidate(thread_db* tdbb, bool ast_handler) {}

	virtual void blockingAstHandler(thread_db* tdbb);
private:
	Firebird::Mutex lockMutex;	// Protects status of logical lock, counters and blocking flag
	lck_owner_t		physicalLockOwner;	// Holds cached lock
	lck_owner_t		defaultLogicalLockOwner;	// Requests new lock to replace cached
	
	// true - unlock keep cached lock and release by AST. 
	// false - unlock releases cached lock if possible
	bool	lockCaching;

	Firebird::SortedArray<ObjectOwnerData, Firebird::EmptyStorage<ObjectOwnerData>, 
		SLONG, ObjectOwnerData, Firebird::DefaultComparator<SLONG> > readers;
	ObjectOwnerData writer;

	// In current implementation, threads are not used along with signals
	// Anyways, if we own mutex only with signals disabled this code
	// becomes signal-safe even in presense of threads.
	//
	class EngineMutexLockGuard {
	public:
		explicit EngineMutexLockGuard(Firebird::Mutex& alock) 
			: lock(&alock) 
		{ 
			ThreadExit te;
			lock->enter(); 
		}
		~EngineMutexLockGuard() { lock->leave(); }
	private:
		// Forbid copy constructor
		EngineMutexLockGuard(const EngineMutexLockGuard& source);
		Firebird::Mutex* lock;
	};

	class CountersLockHolder : public AstInhibit, public EngineMutexLockGuard
	{
	public:
		explicit CountersLockHolder(Firebird::Mutex& mtx)
			: AstInhibit(), EngineMutexLockGuard(mtx) { }
	};

	static int blocking_ast_cached_lock(void* ast_object);
};

}

#endif //  GLOBALRWLOCK_H
