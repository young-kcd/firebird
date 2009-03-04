/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		GlobalRWLock.cpp
 *	DESCRIPTION:	GlobalRWLock
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

#include "firebird.h"
#include "GlobalRWLock.h"
#include "../lock/lock.h"
#include "../lock/lock_proto.h"
#include "iberr_proto.h"
#include "isc_proto.h"
#include "jrd.h"
#include "lck_proto.h"
#include "err_proto.h"

#ifdef COS_DEBUG
IMPLEMENT_TRACE_ROUTINE(cos_trace, "COS")
#endif

namespace Jrd {

int GlobalRWLock::blocking_ast_cached_lock(void* ast_object)
{
	Jrd::GlobalRWLock *GlobalRWLock = 
		static_cast<Jrd::GlobalRWLock*>(ast_object);

	ISC_ast_enter();

	/* Since this routine will be called asynchronously, we must establish
		a thread context. */
	Jrd::thread_db thd_context, *tdbb;
	JRD_set_thread_data(tdbb, thd_context);

	ISC_STATUS_ARRAY ast_status;
	Jrd::Database* dbb = GlobalRWLock->cached_lock->lck_dbb;

	tdbb->setDatabase(dbb);
	tdbb->setAttachment(NULL);
	tdbb->tdbb_quantum = QUANTUM;
	tdbb->setRequest(NULL);
	tdbb->setTransaction(NULL);
	tdbb->tdbb_status_vector = ast_status;

	GlobalRWLock->blockingAstHandler(tdbb);

	/* Restore the prior thread context */

	JRD_restore_thread_data();

	ISC_ast_exit();

	return 0;	
}

GlobalRWLock::GlobalRWLock(thread_db* tdbb, MemoryPool& p, locktype_t lckType, 
						   size_t lockLen, const UCHAR* lockStr, lck_owner_t physical_lock_owner,
						   lck_owner_t default_logical_lock_owner, bool lock_caching)
	: PermanentStorage(p), internal_blocking(0), external_blocking(false), 
	  physicalLockOwner(physical_lock_owner), defaultLogicalLockOwner(default_logical_lock_owner), 
	  lockCaching(lock_caching), readers(p)
{
	SET_TDBB(tdbb);

	Database* dbb = tdbb->getDatabase();

	cached_lock = FB_NEW_RPT(getPool(), lockLen) Lock();
	cached_lock->lck_type = static_cast<Jrd::lck_t>(lckType);
	cached_lock->lck_owner_handle = 0;
	cached_lock->lck_length = lockLen;

	cached_lock->lck_dbb = dbb;
	cached_lock->lck_parent = dbb->dbb_lock;
	cached_lock->lck_object = reinterpret_cast<blk*>(this);
	cached_lock->lck_ast = lockCaching ? blocking_ast_cached_lock : NULL;
	memcpy(&cached_lock->lck_key, lockStr, lockLen);
	
	writer.owner_handle = 0;
	writer.entry_count = 0;
}

GlobalRWLock::~GlobalRWLock()
{
	thread_db* tdbb = JRD_get_thread_data();
	LCK_release(tdbb, cached_lock);
	delete cached_lock;
}

bool GlobalRWLock::lock(thread_db* tdbb, locklevel_t level, SSHORT wait, SLONG owner_handle)
{
	SET_TDBB(tdbb);
	fb_assert(owner_handle);

	{ // this is a first scope for a code where counters are locked
		CountersLockHolder lockHolder(lockMutex);

		COS_TRACE(("lock type=%i, level=%i, readerscount=%i, owner=%i", cached_lock->lck_type, level, readers.getCount(), owner_handle));
		// Check if this is a recursion case
		size_t n;
		if (level == LCK_read) {
			if (readers.find(owner_handle, n)) {
				readers[n].entry_count++;
				return true;
			}
		} 
		else {
			if (writer.owner_handle == owner_handle) {
				writer.entry_count++;
				return true;
			}
		}

		const bool all_compatible = !writer.entry_count && (level == LCK_read || readers.getCount() == 0);

		// We own the lock and all present requests are compatible with us
		// In case of any congestion we force all requests through the lock
		// manager to ensure lock ordering.
		if (cached_lock->lck_physical >= level && all_compatible && 
			!internal_blocking && !external_blocking) 
		{
			if (level == LCK_read) {
				ObjectOwnerData ownerData;
				ownerData.owner_handle = owner_handle;
				ownerData.entry_count++;
				readers.insert(n, ownerData);
			}
			else
			{
				writer.owner_handle = owner_handle;
				writer.entry_count++;
			}

			return true;
		}

		// We need to release lock to get new level lock
		if ( (cached_lock->lck_physical > 0) && (writer.entry_count == 0) && (readers.getCount() == 0) )
		{
			LCK_release(tdbb, cached_lock);
			invalidate(tdbb, false);
			external_blocking = false;
			COS_TRACE(("release our lock to get new level lock, type=%i, level=%i", cached_lock->lck_type, cached_lock->lck_physical));
		}

		internal_blocking++;
	}

	// There is some congestion. Need to use the lock manager.
	// Request new lock at the new level. Several concurrent lock requests may 
	// wait here in the same process in parallel.
	Lock* newLock = FB_NEW_RPT(getPool(), cached_lock->lck_length) Lock;
	newLock->lck_type = cached_lock->lck_type;
	newLock->lck_owner_handle = owner_handle;
	newLock->lck_length = cached_lock->lck_length;

	newLock->lck_dbb = cached_lock->lck_dbb;
	newLock->lck_parent = cached_lock->lck_parent;
	newLock->lck_object = cached_lock->lck_object;
	newLock->lck_ast = cached_lock->lck_ast;
	memcpy(&newLock->lck_key, &cached_lock->lck_key, cached_lock->lck_length);

	COS_TRACE(("request new lock, type=%i, level=%i", cached_lock->lck_type, level));
	if (!LCK_lock(tdbb, newLock, level, wait)) {
		COS_TRACE(("Can't get a lock"));
		delete newLock;
		return false;
	}
	COS_TRACE(("Lock is got, type=%i", cached_lock->lck_type));

	{ // this is a second scope for a code where counters are locked
		CountersLockHolder lockHolder(lockMutex);

		fb_assert(internal_blocking > 0);
		internal_blocking--;

		// Here old lock is not protecting shared object. We must refresh state by fetch.
		if (newLock->lck_physical >= LCK_read) {
			try {
				fetch(tdbb);
			}
			catch(const Firebird::Exception&) {
				LCK_release(tdbb, newLock);
				delete newLock;
				return false;
			}
		}

		if (level == LCK_read) {
			ObjectOwnerData ownerData;
			ownerData.entry_count++;
			ownerData.owner_handle = owner_handle;
			readers.add(ownerData);
		}
		else
		{
			writer.owner_handle = owner_handle;
			writer.entry_count++;
		}

		// Replace cached lock with the new lock if needed
		COS_TRACE(("Replace lock, type=%i", cached_lock->lck_type));
		if (newLock->lck_physical > cached_lock->lck_physical) {
			LCK_release(tdbb, cached_lock);
			delete cached_lock;
			cached_lock = newLock;
			if (!LCK_set_owner_handle(tdbb, cached_lock, LCK_get_owner_handle_by_type(tdbb, physicalLockOwner))) {
				COS_TRACE(("Error: set owner handle for captured lock, type=%i", cached_lock->lck_type));
				LCK_release(tdbb, cached_lock);
				return false;
			}
		}
		else {
			LCK_release(tdbb, newLock);
			delete newLock;
		}
	}

	return true;
}

// NOTE: unlock method must be signal safe
// This function may be called in AST. The function doesn't wait.
void GlobalRWLock::unlock(thread_db* tdbb, locklevel_t level, SLONG owner_handle)
{
	SET_TDBB(tdbb);

	CountersLockHolder lockHolder(lockMutex);

	COS_TRACE(("unlock level=%i", level));

	// Check if this is a recursion case
	if (level == LCK_read) {
		size_t n;
		if (!readers.find(owner_handle, n)) {
			ERR_bugcheck_msg("Attempt to call GlobalRWLock::unlock() while not holding a valid lock for logical owner");
		}
		fb_assert(readers[n].entry_count > 0);
		readers[n].entry_count--;
		if (readers[n].entry_count == 0)
			readers.remove(n);
	}
	else {
		fb_assert(writer.owner_handle == owner_handle);
		fb_assert(writer.entry_count == 1);
		fb_assert(cached_lock->lck_physical == LCK_write);
		
		writer.entry_count = 0;
		writer.owner_handle = 0;

		// Optimize non-contention case - downgrade to PR and re-use the lock
		if (!internal_blocking && !external_blocking && lockCaching)
		{
			if (!LCK_convert(tdbb, cached_lock, LCK_read, 0))
				ERR_bugcheck_msg("LCK_convert call failed in GlobalRWLock::unlock()");
			return;
		}
	}

	if ( (readers.getCount() == 0) && (writer.entry_count == 0) ) {
		COS_TRACE(("check for release a lock, type=%i", cached_lock->lck_type));
		if (internal_blocking || !lockCaching) {
			LCK_release(tdbb, cached_lock);
			invalidate(tdbb, false);
			external_blocking = false;
		} 
		else if (external_blocking) {
			LCK_downgrade(tdbb, cached_lock);
			if (cached_lock->lck_physical < LCK_read)
				invalidate(tdbb, false);
			external_blocking = false;
		}
	}

	COS_TRACE(("unlock type=%i, level=%i, readerscount=%i, owner=%i", cached_lock->lck_type, level, readers.getCount(), owner_handle));
}

void GlobalRWLock::blockingAstHandler(thread_db* tdbb)
{
	SET_TDBB(tdbb);

	CountersLockHolder lockHolder(lockMutex);

	COS_TRACE_AST("bloackingAstHandler");
	// When we request a new lock counters are not updated until we get it.
	// As such, we need to check internal_blocking flag that is set during such situation.
	if ( !internal_blocking && (readers.getCount() == 0) && (writer.entry_count == 0) ) {
		COS_TRACE_AST("downgrade");
		LCK_downgrade(tdbb, cached_lock);
		if (cached_lock->lck_physical < LCK_read) {
			invalidate(tdbb, true);
			external_blocking = false;
		}
	}
	else
		external_blocking = true;
}

void GlobalRWLock::setLockData(SLONG lck_data)
{
	LCK_write_data(cached_lock, lck_data);
} 

void GlobalRWLock::changeLockOwner(thread_db* tdbb, locklevel_t level, SLONG old_owner_handle, SLONG new_owner_handle)
{
	SET_TDBB(tdbb);

	if (old_owner_handle == new_owner_handle)
		return;

	CountersLockHolder lockHolder(lockMutex);

	if (level == LCK_read) {
		size_t n;
		if (readers.find(old_owner_handle, n)) {
			fb_assert(readers[n].entry_count > 0);
			readers[n].entry_count--;
			if (readers[n].entry_count == 0)
				readers.remove(n);

			if (readers.find(new_owner_handle, n))
				readers[n].entry_count++;
			else {
				ObjectOwnerData ownerData;
				ownerData.entry_count++;
				ownerData.owner_handle = new_owner_handle;
				readers.insert(n, ownerData);
			}
		}
		else {
			ERR_bugcheck_msg("Attempt to perform GlobalRWLock::change_lock_owner() while not holding a valid lock for logical owner");
		}
	}
	else {
		fb_assert(writer.entry_count == 1);
		writer.owner_handle = new_owner_handle;
	}
}

bool GlobalRWLock::tryReleaseLock(thread_db* tdbb)
{
	CountersLockHolder lockHolder(lockMutex);
	if (!writer.entry_count && !readers.getCount())
	{
		LCK_release(tdbb, cached_lock);
		invalidate(tdbb, false);
		return true;
	}
	return false;
}

} // namespace Jrd
