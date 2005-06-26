/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		lck.cpp
 *	DESCRIPTION:	Lock handler for JRD (not lock manager!)
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
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#include "firebird.h"
#include "../jrd/common.h"
#include <stdio.h>
#include "../jrd/jrd.h"
#include "../jrd/lck.h"
#include "gen/iberror.h"
#include "../jrd/iberr.h"
#include "../jrd/all_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/jrd_proto.h"

#include "../jrd/lck_proto.h"
#ifndef VMS
#include "../lock/lock_proto.h"
#else
#include "../jrd/vmslo_proto.h"
#endif
#include "../jrd/sch_proto.h"
#include "../jrd/thd.h"
#include "../jrd/gdsassert.h"

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef WIN_NT
#include <process.h>
#endif


using namespace Jrd;

static void bug_lck(TEXT*);
#ifdef MULTI_THREAD
static void check_lock(Lock*, USHORT);
#endif
static bool compatible(Lock*, Lock*, USHORT);
static void enqueue(thread_db*, Lock*, USHORT, SSHORT);
static int external_ast(void*);
#ifdef MULTI_THREAD
static Lock* find_block(Lock*, USHORT);
#endif
static USHORT hash_func(const UCHAR*, USHORT);
static void hash_allocate(Lock*);
static Lock* hash_get_lock(Lock*, USHORT *, Lock***);
static void hash_insert_lock(Lock*);
static bool hash_remove_lock(Lock*, Lock**);
static void internal_ast(Lock*);
static bool internal_compatible(Lock*, Lock*, USHORT);
static void internal_dequeue(thread_db*, Lock*);
static USHORT internal_downgrade(thread_db*, Lock*);
static bool internal_enqueue(thread_db*, Lock*, USHORT, SSHORT, bool);

static void set_lock_attachment(Lock*, Attachment*);


/* globals and macros */

static SLONG process_lck_owner_handle = 0;

#ifdef SUPERSERVER

inline LOCK_OWNER_T LCK_OWNER_ID_PROCESS() {
	return getpid();
}
inline LOCK_OWNER_T LCK_OWNER_ID_DBB(Database* dbb) {
	return (LOCK_OWNER_T) dbb;
}
inline LOCK_OWNER_T LCK_OWNER_ID_ATT(Attachment* attachment) {
	return (LOCK_OWNER_T) attachment;
}

const lck_owner_t LCK_OWNER_TYPE_PROCESS	= LCK_OWNER_process;
const lck_owner_t LCK_OWNER_TYPE_DBB		= LCK_OWNER_database;
const lck_owner_t LCK_OWNER_TYPE_ATT		= LCK_OWNER_attachment;

inline SLONG* LCK_OWNER_HANDLE_PROCESS() {
	return &process_lck_owner_handle;
}
inline SLONG* LCK_OWNER_HANDLE_DBB(Database* dbb) {
	return &dbb->dbb_lock_owner_handle;
}
inline SLONG* LCK_OWNER_HANDLE_ATT(Attachment* attachment) {
	return &attachment->att_lock_owner_handle;
}

#else	/* SUPERSERVER */

/* This is not a SUPERSERVER build */

inline LOCK_OWNER_T LCK_OWNER_ID_PROCESS() {
	return getpid();
}
inline LOCK_OWNER_T LCK_OWNER_ID_DBB(Database* dbb) {
	return getpid();
}
inline LOCK_OWNER_T LCK_OWNER_ID_ATT(Attachment* attachment) {
	return getpid();
}

const lck_owner_t LCK_OWNER_TYPE_PROCESS	= LCK_OWNER_process;
const lck_owner_t LCK_OWNER_TYPE_DBB		= LCK_OWNER_process;
const lck_owner_t LCK_OWNER_TYPE_ATT		= LCK_OWNER_process;

inline SLONG* LCK_OWNER_HANDLE_PROCESS() {
	return &process_lck_owner_handle;
}
inline SLONG* LCK_OWNER_HANDLE_DBB(Database* dbb) {
	return &process_lck_owner_handle;
}
inline SLONG* LCK_OWNER_HANDLE_ATT(Attachment* attachment) {
	return &process_lck_owner_handle;
}

#endif	/* SUPERSERVER */


static const UCHAR compatibility[] = {

/*				Shared	Prot	Shared	Prot
		none	null	 Read	Read	Write	Write	Exclusive */

/* none */ 1, 1, 1, 1, 1, 1, 1,
/* null */ 1, 1, 1, 1, 1, 1, 1,
/* SR */ 1, 1, 1, 1, 1, 1, 0,
/* PR */ 1, 1, 1, 1, 0, 0, 0,
/* SW */ 1, 1, 1, 0, 1, 0, 0,
/* PW */ 1, 1, 1, 0, 0, 0, 0,
/* EX */ 1, 1, 0, 0, 0, 0, 0
};

#define COMPATIBLE(st1, st2)	compatibility [st1 * LCK_max + st2]
const int LOCK_HASH_SIZE	= 19;

inline void ENQUEUE(thread_db* tdbb, Lock* lock, USHORT level, SSHORT wait)
{
	if (lock->lck_compatible)
		internal_enqueue (tdbb, lock, level, wait, false);
	else
		enqueue (tdbb, lock, level, wait);
}

inline bool CONVERT(thread_db* tdbb, Lock* lock, USHORT level, SSHORT wait, ISC_STATUS* status)
{
	return (lock->lck_compatible) ?
		internal_enqueue (tdbb, lock, level, wait, true) :
		LOCK_convert (lock->lck_id, level, wait, lock->lck_ast, lock->lck_object, status);
}

inline void DEQUEUE(thread_db* tdbb, Lock* lock)
{
	if (lock->lck_compatible)
		internal_dequeue (tdbb, lock);
	else
		LOCK_deq (lock->lck_id);
}

inline USHORT DOWNGRADE(thread_db* tdbb, Lock* lock, ISC_STATUS* status)
{
	return (lock->lck_compatible) ?
		internal_downgrade (tdbb, lock) :
		LOCK_downgrade (lock->lck_id, status);
}

#ifdef DEV_BUILD
/* Valid locks are not NULL, 
	of the right memory block type, 
	are attached to a dbb.
   	If we have the dbb in non-exclusive mode, 
	    then we must have a physical lock of at least the same level
	    as the logical lock.
   	If we don't have a lock ID, 
	    then we better not have a physical lock at any level.

JMB: As part of the c++ conversion I removed the check for Lock block type.
 There is no more blk_type field in the Lock structure, and some stack allocated
 Lock's are passed into lock functions, so we can't do the check.
 Here is the line I removed from the macro:
				 (l->blk_type == type_lck) && \
 */
#define LCK_CHECK_LOCK(l)	(((l) != NULL) && \
				 (l->lck_dbb != NULL) && \
				 (l->lck_test_field == 666) && \
				 (l->lck_id || (l->lck_physical == LCK_none)))

/* The following check should be part of LCK_CHECK_LOCK, but it fails
   when the exclusive attachment to a database is changed to a shared
   attachment.  When that occurs we fb_assert all our internal locks, but
   while we are in the process of asserting DBB_assert_locks is set, but
   we haven't gotten physical locks yet.
 
			         (!(l->lck_dbb->dbb_ast_flags & DBB_assert_locks) \
				    || (l->lck_physical >= l->lck_logical)) && \
*/
#endif

#ifndef LCK_CHECK_LOCK
#define LCK_CHECK_LOCK(x)		(TRUE)	/* nothing */
#endif

void LCK_ast_inhibit() {
	LOCK_ast_inhibit();
}

void LCK_ast_enable() {
	LOCK_ast_enable();
}

void LCK_assert(thread_db* tdbb, Lock* lock)
{
/**************************************
 *
 *	L C K _ a s s e r t
 *
 **************************************
 *
 * Functional description
 *	Assert a logical lock.
 *
 **************************************/

	SET_TDBB(tdbb);
	fb_assert(LCK_CHECK_LOCK(lock));

	if (lock->lck_logical == lock->lck_physical ||
		lock->lck_logical == LCK_none)
	{
		return;
	}

	if (!LCK_lock(tdbb, lock, lock->lck_logical, LCK_WAIT))
		BUGCHECK(159);			/* msg 159 cannot fb_assert logical lock */

	fb_assert(LCK_CHECK_LOCK(lock));
}


bool LCK_convert(thread_db* tdbb, Lock* lock, USHORT level, SSHORT wait)
{
/**************************************
 *
 *	L C K _ c o n v e r t
 *
 **************************************
 *
 * Functional description
 *	Convert an existing lock to a new level.
 *
 **************************************/
	fb_assert(LCK_CHECK_LOCK(lock));

	SET_TDBB(tdbb);
	Database* dbb = lock->lck_dbb;
	ISC_STATUS* status = tdbb->tdbb_status_vector;

	Attachment *old_attachment = lock->lck_attachment;
	set_lock_attachment(lock, tdbb->tdbb_attachment);

	const bool result = CONVERT(tdbb, lock, level, wait, status);

	if (!result) {
	    set_lock_attachment(lock, old_attachment);

		if (status[1] == isc_deadlock ||
			status[1] == isc_lock_conflict || status[1] == isc_lock_timeout)
		{
			return false;
		}
		if (status[1] == isc_lockmanerr)
			dbb->dbb_flags |= DBB_bugcheck;
		ERR_punt();
	}

	if (!lock->lck_compatible)
		lock->lck_physical = lock->lck_logical = level;

	fb_assert(LCK_CHECK_LOCK(lock));
	return true;
}


#ifdef MULTI_THREAD
int LCK_convert_non_blocking(thread_db* tdbb, Lock* lock, USHORT level, SSHORT wait)
{
/**************************************
 *
 *	L C K _ c o n v e r t _ n o n _ b l o c k i n g		( m u l t i _ t h r e a d )
 *
 **************************************
 *
 * Functional description
 *	Convert an existing lock.
 *
 **************************************/
	fb_assert(LCK_CHECK_LOCK(lock));
	SET_TDBB(tdbb);

	Database* dbb = lock->lck_dbb;

	if (!wait || !gds__thread_enable(FALSE))
		return LCK_convert(tdbb, lock, level, wait);

	Attachment* old_attachment = lock->lck_attachment;
	set_lock_attachment(lock, tdbb->tdbb_attachment);

/* Save context and checkout from the scheduler */

	check_lock(lock, level);
	ISC_STATUS* status = tdbb->tdbb_status_vector;
	AST_DISABLE();

/* SuperServer: Do Not release engine here, it creates a race
   condition - more than one thread RUNNING in the engine.
   We check out of the scheduler later - in wait_for_request()
   in lock/lock.cpp - when we are going to wait on wakeup event.
*/
#ifndef SUPERSERVER
	SCH_exit();
#endif

	const bool result = CONVERT(tdbb, lock, level, wait, status);

/* Check back in with the scheduler and restore context */

#ifndef SUPERSERVER
	SCH_enter();
#endif

	AST_ENABLE();

	if (!result) {
		set_lock_attachment(lock, old_attachment);

		if (status[1] == isc_deadlock ||
			status[1] == isc_lock_conflict || status[1] == isc_lock_timeout)
		{
			return FALSE;
		}
		if (status[1] == isc_lockmanerr)
			dbb->dbb_flags |= DBB_bugcheck;
		ERR_punt();
	}

	if (!lock->lck_compatible)
		lock->lck_physical = lock->lck_logical = level;

	fb_assert(LCK_CHECK_LOCK(lock));
	return TRUE;
}


#else
int LCK_convert_non_blocking(thread_db* tdbb, Lock* lock, USHORT level, SSHORT wait)
{
/**************************************
 *
 *	L C K _ c o n v e r t _ n o n _ b l o c k i n g		( s i n g l e _
 *								  t h r e a d )
 *
 **************************************
 *
 * Functional description
 *	Convert and existing lock.  If successful, return 0, otherwise
 *	return error code.
 *
 **************************************/

	fb_assert(LCK_CHECK_LOCK(lock));
	SET_TDBB(tdbb);
	return LCK_convert(tdbb, lock, level, wait);
}
#endif


int LCK_convert_opt(thread_db* tdbb, Lock* lock, USHORT level)
{
/**************************************
 *
 *	L C K _ c o n v e r t _ o p t
 *
 **************************************
 *
 * Functional description
 *	Assert a lock if the parent is not locked in exclusive mode.
 *
 **************************************/
	SET_TDBB(tdbb);
	fb_assert(LCK_CHECK_LOCK(lock));
	const USHORT old_level = lock->lck_logical;
	lock->lck_logical = level;
	Database* dbb = lock->lck_dbb;

	if (dbb->dbb_ast_flags & DBB_assert_locks) {
		lock->lck_logical = old_level;
		return LCK_convert(tdbb, lock, level, LCK_NO_WAIT);
	}

	fb_assert(LCK_CHECK_LOCK(lock));
	return TRUE;
}


#ifndef VMS
int LCK_downgrade(thread_db* tdbb, Lock* lock)
{
/**************************************
 *
 *	L C K _ d o w n g r a d e
 *
 **************************************
 *
 * Functional description
 *	Downgrade a lock.
 *
 **************************************/
	fb_assert(LCK_CHECK_LOCK(lock));
	SET_TDBB(tdbb);

	ISC_STATUS* status = tdbb->tdbb_status_vector;

	fb_assert(LCK_CHECK_LOCK(lock));

	if (lock->lck_id && lock->lck_physical != LCK_none) {
		const USHORT level = DOWNGRADE(tdbb, lock, status);
		if (!lock->lck_compatible)
			lock->lck_physical = lock->lck_logical = level;
	}

	if (lock->lck_physical == LCK_none) {
		lock->lck_id = lock->lck_data = 0;
	    set_lock_attachment(lock, NULL);
	}

	fb_assert(LCK_CHECK_LOCK(lock));
	return TRUE;
}
#endif


void LCK_fini(thread_db* tdbb, enum lck_owner_t owner_type)
{
/**************************************
 *
 *	L C K _ f i n i
 *
 **************************************
 *
 * Functional description
 *	Check out with lock manager.
 *
 **************************************/
	SLONG* owner_handle_ptr = 0;

	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	Attachment* attachment = tdbb->tdbb_attachment;

	switch (owner_type) {
	case LCK_OWNER_process:
		owner_handle_ptr = LCK_OWNER_HANDLE_PROCESS();
		break;

	case LCK_OWNER_database:
		owner_handle_ptr = LCK_OWNER_HANDLE_DBB(dbb);
		break;

	case LCK_OWNER_attachment:
		owner_handle_ptr = LCK_OWNER_HANDLE_ATT(attachment);
		break;

	default:
		bug_lck("Invalid lock owner type in LCK_fini ()");
		break;
	}

	LOCK_fini(tdbb->tdbb_status_vector, owner_handle_ptr);
}


SLONG LCK_get_owner_handle(thread_db* tdbb, enum lck_t lock_type)
{
/**************************************
 *
 *	L C K _ g e t _ l o c k _ o w n e r
 *
 **************************************
 *
 * Functional description
 *	return the right kind of lock owner given a lock type.
 *
 **************************************/

	SET_TDBB(tdbb);
#ifdef SUPERSERVER
	Database* dbb = tdbb->tdbb_database;
	Attachment* attachment = tdbb->tdbb_attachment;
#else
	Database* dbb = NULL;
	Attachment* attachment = NULL;
#endif
	switch (lock_type) {
	case LCK_database:
	case LCK_bdb:
	case LCK_rel_exist:
	case LCK_rel_partners:
	case LCK_idx_exist:
	case LCK_shadow:
	case LCK_retaining:
	case LCK_expression:
	case LCK_record_locking:
	case LCK_prc_exist:
	case LCK_range_relation:
	case LCK_backup_state:
	case LCK_backup_alloc:
	case LCK_backup_database:
		return *LCK_OWNER_HANDLE_DBB(dbb);
	case LCK_attachment:
	case LCK_relation:
	case LCK_file_extend:
	case LCK_tra:
	case LCK_sweep:
	case LCK_record:
	case LCK_update_shadow:
		return *LCK_OWNER_HANDLE_ATT(attachment);
	default:
		bug_lck("Invalid lock type in LCK_get_owner_handle ()");
		/* Not Reached - bug_lck calls ERR_post */
		return 0;
	}
}


void LCK_init(thread_db* tdbb, enum lck_owner_t owner_type)
{
/**************************************
 *
 *	L C K _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize the locking stuff for the given owner.
 *
 **************************************/
	LOCK_OWNER_T owner_id;
	SLONG* owner_handle_ptr = 0;

	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	Attachment* attachment = tdbb->tdbb_attachment;

	switch (owner_type) {
	case LCK_OWNER_process:
		owner_id = LCK_OWNER_ID_PROCESS();
		owner_handle_ptr = LCK_OWNER_HANDLE_PROCESS();
		break;

	case LCK_OWNER_database:
		owner_id = LCK_OWNER_ID_DBB(dbb);
		owner_handle_ptr = LCK_OWNER_HANDLE_DBB(dbb);
		break;

	case LCK_OWNER_attachment:
		owner_id = LCK_OWNER_ID_ATT(attachment);
		owner_handle_ptr = LCK_OWNER_HANDLE_ATT(attachment);
		break;

	default:
		bug_lck("Invalid lock owner type in LCK_init ()");
		break;
	}

	if (LOCK_init(tdbb->tdbb_status_vector, true,
				  owner_id, owner_type, owner_handle_ptr))
	{
		if (tdbb->tdbb_status_vector[1] == isc_lockmanerr)
			dbb->dbb_flags |= DBB_bugcheck;
		ERR_punt();
	}
}


int LCK_lock(thread_db* tdbb, Lock* lock, USHORT level, SSHORT wait)
{
/**************************************
 *
 *	L C K _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Lock a block.  There had better not have been a lock there.
 *
 **************************************/
	fb_assert(LCK_CHECK_LOCK(lock));
	SET_TDBB(tdbb);

	Database* dbb = lock->lck_dbb;
	ISC_STATUS* status = tdbb->tdbb_status_vector;
	lock->lck_blocked_threads = NULL;
    set_lock_attachment(lock, tdbb->tdbb_attachment);

	ENQUEUE(tdbb, lock, level, wait);
	fb_assert(LCK_CHECK_LOCK(lock));
	if (!lock->lck_id)
	{
    	set_lock_attachment(lock, NULL);
		if (!wait ||
			status[1] == isc_deadlock ||
			status[1] == isc_lock_conflict || status[1] == isc_lock_timeout)
		{
			return FALSE;
		}
		else {
			if (status[1] == isc_lockmanerr)
				dbb->dbb_flags |= DBB_bugcheck;
			ERR_punt();
		}
	}

	if (!lock->lck_compatible)
		lock->lck_physical = lock->lck_logical = level;

	fb_assert(LCK_CHECK_LOCK(lock));
	return TRUE;
}


int LCK_lock_non_blocking(thread_db* tdbb, Lock* lock, USHORT level, SSHORT wait)
{
/**************************************
 *
 *	L C K _ l o c k _ n o n _ b l o c k i n g
 *
 **************************************
 *
 * Functional description
 *	Lock an object in a manner that allows other 
 *	threads to be scheduled.
 *
 **************************************/
#ifdef MULTI_THREAD
	fb_assert(LCK_CHECK_LOCK(lock));
	SET_TDBB(tdbb);

	Database* dbb = lock->lck_dbb;

/* Don't bother for the non-wait or non-multi-threading case */

	if (!wait || !gds__thread_enable(FALSE))
		return LCK_lock(tdbb, lock, level, wait);

	set_lock_attachment(lock, tdbb->tdbb_attachment);

/* Make sure we're not about to wait for ourselves */

	lock->lck_blocked_threads = NULL;
	check_lock(lock, level);
	ISC_STATUS* status = tdbb->tdbb_status_vector;

/* If we can get the lock without waiting, save a great
   deal of grief */

	ENQUEUE(tdbb, lock, level, LCK_NO_WAIT);
	fb_assert(LCK_CHECK_LOCK(lock));
	if (!lock->lck_id) {
		/* Save context and checkout from the scheduler */

		AST_DISABLE();

/* SuperServer: Do Not release engine here, it creates a race
   condition - more than one thread RUNNING in the engine.
   We check out of the scheduler later - in wait_for_request()
   in lock/lock.cpp - when we are going to wait on wakeup event.
*/
#ifndef SUPERSERVER
		SCH_exit();
#endif

		INIT_STATUS(status);
		ENQUEUE(tdbb, lock, level, wait);

		/* Check back in with the scheduler and restore context */

#ifndef SUPERSERVER
		SCH_enter();
#endif
		AST_ENABLE();

		fb_assert(LCK_CHECK_LOCK(lock));

		/* If lock was rejected, there's trouble */

		if (!lock->lck_id) {
			set_lock_attachment(lock, NULL);

			if (status[1] == isc_deadlock ||
				status[1] == isc_lock_conflict ||
				status[1] == isc_lock_timeout)
			{
				return FALSE;
			}
			else {
				if (status[1] == isc_lockmanerr)
					dbb->dbb_flags |= DBB_bugcheck;
				ERR_punt();
			}
		}
	}

	if (!lock->lck_compatible)
		lock->lck_physical = lock->lck_logical = level;

	fb_assert(LCK_CHECK_LOCK(lock));
	return TRUE;

#else

	SET_TDBB(tdbb);
	fb_assert(LCK_CHECK_LOCK(lock));

	return LCK_lock(tdbb, lock, level, wait);
#endif
}


int LCK_lock_opt(thread_db* tdbb, Lock* lock, USHORT level, SSHORT wait)
{
/**************************************
 *
 *	L C K _ l o c k _ o p t
 *
 **************************************
 *
 * Functional description
 *	Assert a lock if the parent is not locked in exclusive mode.
 *
 **************************************/

	SET_TDBB(tdbb);
	fb_assert(LCK_CHECK_LOCK(lock));
	lock->lck_logical = level;
	Database* dbb = lock->lck_dbb;

	if (dbb->dbb_ast_flags & DBB_assert_locks) {
		lock->lck_logical = LCK_none;
		return LCK_lock(tdbb, lock, level, wait);
	}

	fb_assert(LCK_CHECK_LOCK(lock));
	return TRUE;
}


#ifndef VMS
SLONG LCK_query_data(Lock* parent, enum lck_t lock_type, USHORT aggregate)
{
/**************************************
 *
 *	L C K _ q u e r y _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Perform aggregate operations on data associated
 *	with a lock series for a lock hierarchy rooted
 *	at a parent lock.
 *
 **************************************/

	fb_assert(LCK_CHECK_LOCK(parent));
	return LOCK_query_data(parent->lck_id, lock_type, aggregate);
}
#endif


SLONG LCK_read_data(Lock* lock)
{
/**************************************
 *
 *	L C K _ r e a d _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Read the data associated with a lock.
 *
 **************************************/
	fb_assert(LCK_CHECK_LOCK(lock));
#ifdef VMS
	if (!LCK_lock(NULL, lock, LCK_null, LCK_NO_WAIT))
		return 0;

	const SLONG data = LOCK_read_data(lock->lck_id);
	LCK_release(lock);
#else
	Lock* parent = lock->lck_parent;
	const SLONG data = LOCK_read_data2(
						   parent ? parent->lck_id : 0,
						   lock->lck_type,
						   (UCHAR *) & lock->lck_key,
						   lock->lck_length, lock->lck_owner_handle);
#endif

	fb_assert(LCK_CHECK_LOCK(lock));
	return data;
}


void LCK_release(thread_db* tdbb, Lock* lock)
{
/**************************************
 *
 *	L C K _ r e l e a s e
 *
 **************************************
 *
 * Functional description
 *	Release an existing lock.
 *
 **************************************/

	SET_TDBB(tdbb);
	fb_assert(LCK_CHECK_LOCK(lock));

	if (lock->lck_physical != LCK_none) {
		DEQUEUE(tdbb, lock);
	}

	lock->lck_physical = lock->lck_logical = LCK_none;
	lock->lck_id = lock->lck_data = 0;
	set_lock_attachment(lock, NULL);

#ifdef MULTI_THREAD
	if (lock->lck_blocked_threads)
		JRD_unblock(&lock->lck_blocked_threads);
#endif

	fb_assert(LCK_CHECK_LOCK(lock));
}


void LCK_re_post(Lock* lock)
{
/**************************************
 *
 *	L C K _ r e _ p o s t
 *
 **************************************
 *
 * Functional description
 *	Re-post an ast when the original 
 *	deliver resulted in blockage.
 *
 **************************************/

	fb_assert(LCK_CHECK_LOCK(lock));
	if (lock->lck_compatible) {
		if (lock->lck_ast) {
			(*lock->lck_ast)(lock->lck_object);
		}
		return;
	}

	LOCK_re_post(lock->lck_ast, lock->lck_object, lock->lck_owner_handle);
	fb_assert(LCK_CHECK_LOCK(lock));
}


void LCK_write_data(Lock* lock, SLONG data)
{
/**************************************
 *
 *	L C K _ w r i t e _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Write a longword into an existing lock.
 *
 **************************************/

	fb_assert(LCK_CHECK_LOCK(lock));
	LOCK_write_data(lock->lck_id, data);
	lock->lck_data = data;
	fb_assert(LCK_CHECK_LOCK(lock));
}


static void bug_lck(TEXT* string)
{
/**************************************
 *
 *	b u g _ l c k
 *
 **************************************
 *
 * Functional description
 *	Log the bug message, initialize the status vector
 *	and get out.
 *
 **************************************/
	TEXT s[128];

	sprintf(s, "Fatal lock interface error: %s", string);
	gds__log(s);
	ERR_post(isc_db_corrupt, isc_arg_string, string, 0);
}


#ifdef MULTI_THREAD
static void check_lock(Lock* lock, USHORT level)
{
/**************************************
 *
 *	c h e c k _ l o c k
 *
 **************************************
 *
 * Functional description
 *	If a lock is already held by this process, 
 *	wait for it to be released before asking 
 *	the lock manager for it.
 *
 **************************************/
	Lock* next;

	fb_assert(LCK_CHECK_LOCK(lock));
	while (next = find_block(lock, level))
		JRD_blocked(next->lck_attachment, &next->lck_blocked_threads);
}
#endif


static bool compatible(Lock* lock1, Lock* lock2, USHORT level2)
{
/**************************************
 *
 *	c o m p a t i b l e
 *
 **************************************
 *
 * Functional description
 *	Given two locks, and a desired level for the
 *	second lock, determine whether the two locks
 *	would be compatible.
 *
 **************************************/

	fb_assert(LCK_CHECK_LOCK(lock1));
	fb_assert(LCK_CHECK_LOCK(lock2));

/* if the locks have the same compatibility block,
   they are always compatible regardless of level */

	if (lock1->lck_compatible &&
		lock2->lck_compatible &&
		lock1->lck_compatible == lock2->lck_compatible)
	{
		/* check for a second level of compatibility as well:
		   if a second level was specified, the locks must 
		   also be compatible at the second level */

		if (!lock1->lck_compatible2 ||
			!lock2->lck_compatible2 ||
			lock1->lck_compatible2 == lock2->lck_compatible2)
		{
			return true;
		}
	}

	if (COMPATIBLE(lock1->lck_logical, level2))
		return true;

	return false;
}


static void enqueue(thread_db* tdbb, Lock* lock, USHORT level, SSHORT wait)
{
/**************************************
 *
 *	e n q u e u e
 *
 **************************************
 *
 * Functional description
 *	Submit a lock to the lock manager.
 *
 **************************************/
	SET_TDBB(tdbb);

	fb_assert(LCK_CHECK_LOCK(lock));

	Lock* parent = lock->lck_parent;
	lock->lck_id = LOCK_enq(lock->lck_id,
							parent ? parent->lck_id : 0,
							lock->lck_type,
							(const UCHAR*) & lock->lck_key,
							lock->lck_length,
							level,
							lock->lck_ast,
							lock->lck_object, lock->lck_data, wait,
							tdbb->tdbb_status_vector, lock->lck_owner_handle);
	if (!lock->lck_id)
		lock->lck_physical = lock->lck_logical = LCK_none;
	fb_assert(LCK_CHECK_LOCK(lock));
}


static int external_ast(void* lock_void)
{
/**************************************
 *
 *	e x t e r n a l _ a s t
 *
 **************************************
 *
 * Functional description
 *	Deliver blocking asts to all locks identical to 
 *	the passed lock.  This routine is called when 
 *	we are blocking a lock from another process.
 *
 **************************************/
	Lock* lock = static_cast<Lock*>(lock_void);
	fb_assert(LCK_CHECK_LOCK(lock));

/* go through the list, saving the next lock in the list
   in case the current one gets deleted in the ast */

	Lock* next;
	for (Lock* match = hash_get_lock(lock, 0, 0); match; match = next) {
		next = match->lck_identical;
		if (match->lck_ast) {
			(*match->lck_ast)(match->lck_object);
		}
	}
	return 0; // make the compiler happy
}



#ifdef MULTI_THREAD
static Lock* find_block(Lock* lock, USHORT level)
{
/**************************************
 *
 *	f i n d _ b l o c k
 *
 **************************************
 *
 * Functional description
 *	Given a prospective lock, see if the database already holds
 *	the lock for a different attachment.  If so, return the
 *	locking lock, otherwise return NULL.
 *
 **************************************/
	fb_assert(LCK_CHECK_LOCK(lock));

	Database* dbb = lock->lck_dbb;

	Attachment* attachment = lock->lck_attachment;
	if (!attachment)
		return NULL;

	const char* const end = lock->lck_key.lck_string + lock->lck_length;

	for (Lock* next = attachment->att_long_locks; next; next = next->lck_next)
		if (lock->lck_type == next->lck_type &&
			lock->lck_parent && next->lck_parent &&
			(lock->lck_parent->lck_id == next->lck_parent->lck_id) &&
			lock->lck_length == next->lck_length &&
			lock->lck_attachment != next->lck_attachment &&
			!compatible(next, lock, level))
		{
			const char* p1 = lock->lck_key.lck_string;
			const char* p2 = next->lck_key.lck_string;
			while (p1 < end && *p1++ == *p2++); // empty body
			if (p1[-1] == p2[-1])
				return next;
		}

	return NULL;
}
#endif


static USHORT hash_func(const UCHAR* value, USHORT length)
{
/**************************************
 *
 *	h a s h
 *
 **************************************
 *
 * Functional description
 *	Provide a repeatable hash value based
 *	on the passed key.
 *
 **************************************/

/* Hash the value, preserving its distribution 
   as much as possible */

	ULONG hash_value = 0;
	UCHAR* p = 0;
	const UCHAR* q = value;

	for (USHORT l = 0; l < length; l++) {
		if (!(l & 3))
			p = (UCHAR *) & hash_value;
		*p++ = *q++;
	}

	return (USHORT) (hash_value % LOCK_HASH_SIZE);
}


static void hash_allocate(Lock* lock)
{
/**************************************
 *
 *	h a s h _ a l l o c a t e
 *
 **************************************
 *
 * Functional description
 *	Allocate the hash table for handling
 *	compatible locks.
 *
 **************************************/
	fb_assert(LCK_CHECK_LOCK(lock));

	Database* dbb = lock->lck_dbb;

	Attachment* attachment = lock->lck_attachment;
	if (attachment) {
		attachment->att_compatibility_table =
			vec::newVector(*dbb->dbb_permanent, LOCK_HASH_SIZE);
	}
}


static Lock* hash_get_lock(Lock* lock, USHORT * hash_slot, Lock*** prior)
{
/**************************************
 *
 *	h a s h _ g e t _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Return the first matching identical 
 *	lock to the passed lock.  To minimize
 *	code for searching through the hash 
 *	table, return hash_slot or prior lock
 *	if requested.
 *
 **************************************/
	fb_assert(LCK_CHECK_LOCK(lock));

	Attachment* att = lock->lck_attachment;
	if (!att)
		return NULL;

	if (!att->att_compatibility_table)
		hash_allocate(lock);

	const USHORT hash_value =
		hash_func((UCHAR *) & lock->lck_key, lock->lck_length);
		
	if (hash_slot)
		*hash_slot = hash_value;

/* if no collisions found, we're done */

	Lock* match = (Lock*) (*att->att_compatibility_table)[hash_value];
	if (!match)
		return NULL;
	if (prior)
		*prior =
			(Lock**) & (*att->att_compatibility_table)[hash_value];

/* look for an identical lock */

	fb_assert(LCK_CHECK_LOCK(match));
	for (Lock* collision = match; collision; collision = collision->lck_collision)
	{
		fb_assert(LCK_CHECK_LOCK(collision));
		if (collision->lck_parent && lock->lck_parent &&
			collision->lck_parent->lck_id == lock->lck_parent->lck_id &&
			collision->lck_type == lock->lck_type &&
			collision->lck_length == lock->lck_length)
		{
			/* check that the keys are the same */

			const char* p = lock->lck_key.lck_string;
			const char* q = collision->lck_key.lck_string;
			SSHORT l;
			for (l = lock->lck_length; l; l--)
				if (*p++ != *q++)
					break;
			if (!l)
				return collision;
		}

		if (prior)
			*prior = &collision->lck_collision;
	}

	return NULL;
}


static void hash_insert_lock(Lock* lock)
{
/**************************************
 *
 *	h a s h _ i n s e r t _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Insert the provided lock into the 
 *	compatibility lock table.
 *
 **************************************/
	fb_assert(LCK_CHECK_LOCK(lock));

	Attachment* att = lock->lck_attachment;
	if (!att)
		return;

/* if no identical is returned, place it in the collision list */

	USHORT hash_slot;
	Lock* identical = hash_get_lock(lock, &hash_slot, 0);
	if (!identical) {
		lock->lck_collision =
			(Lock*) (*att->att_compatibility_table)[hash_slot];
		(*att->att_compatibility_table)[hash_slot] = (BLK) lock;
		return;
	}

/* place it second in the list, out of pure laziness */

	lock->lck_identical = identical->lck_identical;
	identical->lck_identical = lock;
}


static bool hash_remove_lock(Lock* lock, Lock** match)
{
/**************************************
 *
 *	h a s h _ r e m o v e _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Remove the passed lock from the hash table.
 *	Return true if this is the last such identical
 *	lock removed.   Also return the first matching 
 *	locking found.
 *
 **************************************/
	fb_assert(LCK_CHECK_LOCK(lock));

	Lock** prior;
	Lock* next = hash_get_lock(lock, 0, &prior);
	if (!next) {
		/* set lck_compatible to NULL to make sure we don't
		   try to release the lock again in bugchecking */

		lock->lck_compatible = NULL;
		BUGCHECK(285);			/* lock not found in internal lock manager */
	}

	if (match)
		*match = next;

/* special case if our lock is the first one in the identical list */

	if (next == lock)
		if (lock->lck_identical) {
			lock->lck_identical->lck_collision = lock->lck_collision;
			*prior = lock->lck_identical;
			return false;
		}
		else {
			*prior = lock->lck_collision;
			return true;
		}

	Lock* last = 0;
	for (; next; last = next, next = next->lck_identical)
		if (next == lock)
			break;

	if (!next) {
		lock->lck_compatible = NULL;
		BUGCHECK(285);			/* lock not found in internal lock manager */
	}

	last->lck_identical = next->lck_identical;
	return false;
}


static void internal_ast(Lock* lock)
{
/**************************************
 *
 *	i n t e r n a l _ a s t
 *
 **************************************
 *
 * Functional description
 *	Deliver blocking asts to all locks identical to 
 *	the passed lock.  This routine is called to downgrade
 *	all other locks in the same process which do not have
 *	the lck_compatible field set.  
 *	Note that if this field were set, the internal lock manager 
 *	should not be able to generate a lock request which blocks 
 *	on our own process.
 *
 **************************************/
	fb_assert(LCK_CHECK_LOCK(lock));

/* go through the list, saving the next lock in the list
   in case the current one gets deleted in the ast */

	Lock* next;
	for (Lock* match = hash_get_lock(lock, 0, 0); match; match = next) {
		next = match->lck_identical;

		/* don't deliver the ast to any locks which are already compatible */

		if ((match != lock) &&
			!compatible(match, lock, lock->lck_logical) &&
			match->lck_ast)
		{
			(*match->lck_ast)(match->lck_object);
		}
	}
}



static bool internal_compatible(Lock* match, Lock* lock, USHORT level)
{
/**************************************
 *
 *	i n t e r n a l _ c o m p a t i b l e
 *
 **************************************
 *
 * Functional description
 *	See if there are any incompatible locks
 *	in the list of locks held by this process.
 *	If there are none, return true to indicate
 *	that the lock is compatible.
 *
 **************************************/
	fb_assert(LCK_CHECK_LOCK(match));
	fb_assert(LCK_CHECK_LOCK(lock));

	Lock* next;
	
/* first check if there are any locks which are 
   incompatible which do not have blocking asts;
   if so, there is no chance of getting a compatible 
   lock */

	for (next = match; next; next = next->lck_identical)
		if (!next->lck_ast && !compatible(next, lock, level))
			return false;

/* now deliver the blocking asts, attempting to gain
   compatibility by getting everybody to downgrade */

	internal_ast(match);

/* make one more pass to see if all locks were downgraded */

	for (next = match; next; next = next->lck_identical)
		if (!compatible(next, match, level))
			return false;

	return true;
}


static void internal_dequeue(thread_db* tdbb, Lock* lock)
{
/**************************************
 *
 *	i n t e r n a l _ d e q u e u e
 *
 **************************************
 *
 * Functional description
 *	Dequeue a lock.  If there are identical 
 *	compatible locks, check to see whether 
 *	the lock needs to be downgraded.
 *
 **************************************/
	SET_TDBB(tdbb);
	fb_assert(LCK_CHECK_LOCK(lock));
	fb_assert(lock->lck_compatible);

/* if this is the last identical lock in the hash table, release it */

	Lock* match;
	if (hash_remove_lock(lock, &match)) {
		if (!LOCK_deq(lock->lck_id))
			bug_lck("LOCK_deq() failed in Lock:internal_dequeue");
		lock->lck_id = 0;
		lock->lck_physical = lock->lck_logical = LCK_none;
		return;
	}

/* check for a potential downgrade */

	internal_downgrade(tdbb, match);
}


static USHORT internal_downgrade(thread_db* tdbb, Lock* first)
{
/**************************************
 *
 *	i n t e r n a l _ d o w n g r a d e 
 *
 **************************************
 *
 * Functional description
 *	Set the physical lock value of all locks identical
 *	to the passed lock.  It should be the same as the
 *	highest logical level.
 *
 **************************************/
	SET_TDBB(tdbb);

	fb_assert(LCK_CHECK_LOCK(first));
	fb_assert(first->lck_compatible);

	Lock* lock;
	
/* find the highest required lock level */

	USHORT level = LCK_none;
	for (lock = first; lock; lock = lock->lck_identical)
		level = MAX(level, lock->lck_logical);

/* if we can convert to that level, set all identical 
   locks as having that level */

	if ((level < first->lck_physical) &&
		LOCK_convert(first->lck_id,
					 level,
					 LCK_NO_WAIT,
					 external_ast,
					 first, tdbb->tdbb_status_vector))
	{
		for (lock = first; lock; lock = lock->lck_identical)
			lock->lck_physical = level;
		return level;
	}

	return first->lck_physical;
}


static bool internal_enqueue(
								thread_db* tdbb,
								Lock* lock,
								USHORT level,
								SSHORT wait, bool convert_flg)
{
/**************************************
 *
 *	i n t e r n a l _ e n q u e u e
 *
 **************************************
 *
 * Functional description
 *	See if there is a compatible lock already held
 *	by this process; if not, go ahead and submit the
 *	lock to the real lock manager.
 *	NOTE: This routine handles both enqueueing
 *	and converting existing locks, since the convert
 *	will find itself in the hash table and convert 
 *	itself upward.
 *
 **************************************/
	fb_assert(LCK_CHECK_LOCK(lock));
	fb_assert(lock->lck_compatible);

	SET_TDBB(tdbb);
	ISC_STATUS* status = tdbb->tdbb_status_vector;

/* look for an identical lock */

	Lock* match = hash_get_lock(lock, 0, 0);
	if (match) {
		/* if there are incompatible locks for which
		   there are no blocking asts defined, give up */

		if (!internal_compatible(match, lock, level)) {
			/* for now return a lock conflict; it would be better if we were to 
			   do a wait on the other lock by setting some flag bit or some such */

			status[0] = isc_arg_gds;
			status[1] = isc_lock_conflict;
			status[2] = isc_arg_end;
			return false;
		}

		/* if there is still an identical lock,
		   convert the lock, otherwise fall
		   through and enqueue a new one */

		if ( (match = hash_get_lock(lock, 0, 0)) ) {
			/* if a conversion is necessary, update all identical 
			   locks to reflect the new physical lock level */

			if (level > match->lck_physical) {
				if (!LOCK_convert(match->lck_id,
								  level,
								  wait,
								  external_ast,
								  lock, status))
				{
					  return false;
				}
				for (Lock* update = match; update; update = update->lck_identical)
					update->lck_physical = level;
			}

			lock->lck_id = match->lck_id;
			lock->lck_logical = level;
			lock->lck_physical = match->lck_physical;

			/* When converting a lock (from the callers point of view), 
			   then no new lock needs to be inserted. */

			if (!convert_flg)
				hash_insert_lock(lock);

			return true;
		}
	}

/* enqueue the lock, but swap out the ast and the ast argument
   with the local ast handler, passing it the lock block itself */

	lock->lck_id = LOCK_enq(lock->lck_id,
							lock->lck_parent ? lock->lck_parent->lck_id : 0,
							lock->lck_type,
							(const UCHAR*) & lock->lck_key,
							lock->lck_length,
							level,
							external_ast,
							lock,
							lock->lck_data,
							wait, status, lock->lck_owner_handle);

/* If the lock exchange failed, set the lock levels appropriately */
	if (lock->lck_id == 0)
		lock->lck_physical = lock->lck_logical = LCK_none;

	fb_assert(LCK_CHECK_LOCK(lock));

	if (lock->lck_id) {
		hash_insert_lock(lock);
		lock->lck_logical = lock->lck_physical = level;
	}

	fb_assert(LCK_CHECK_LOCK(lock));
	return lock->lck_id ? true : false;
}


static void set_lock_attachment(Lock* lock, Attachment* attachment)
{
	if (lock->lck_attachment == attachment)
		return;

	// Disable delivery of ASTs for the moment while queue of locks is in flux
	LCK_ast_inhibit();

	// If lock has an attachment it must not be a part of linked list
	fb_assert(!lock->lck_attachment ? !lock->lck_prior && !lock->lck_next : true);

	// Delist in old attachment
	if (lock->lck_attachment) {
		// Check that attachment seems to be valid, check works only when DEBUG_GDS_ALLOC is defined
		fb_assert(lock->lck_attachment->att_flags != 0xDEADBEEF);

		Lock* next = lock->lck_next;
		Lock* prior = lock->lck_prior;

		if (prior) {
			fb_assert(prior->lck_next == lock);
			prior->lck_next = next;
		}
		else {
			fb_assert(lock->lck_attachment->att_long_locks == lock);
			lock->lck_attachment->att_long_locks = next;
		}

		if (next) {
			fb_assert(next->lck_prior == lock);
			next->lck_prior = prior;
		}

		lock->lck_next = NULL;
		lock->lck_prior = NULL;
	}
	

	// Enlist in new attachment
	if (attachment) {
		// Check that attachment seems to be valid, check works only when DEBUG_GDS_ALLOC is defined
		fb_assert(attachment->att_flags != 0xDEADBEEF);

		lock->lck_next = attachment->att_long_locks;
		lock->lck_prior = NULL;
		attachment->att_long_locks = lock;

		Lock* next = lock->lck_next;
		if (next)
			next->lck_prior = lock;
	}

	LCK_ast_enable();

	lock->lck_attachment = attachment;
}

