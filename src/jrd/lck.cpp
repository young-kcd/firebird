/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		lck.c
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
#include "../jrd/ib_stdio.h"
#include "../jrd/jrd.h"
#include "../jrd/lck.h"
#include "gen/codes.h"
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
#include "../jrd/thd_proto.h"
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

extern "C" {


static void bug_lck(TEXT*);
static void check_lock(LCK, USHORT);
static BOOLEAN compatible(LCK, LCK, USHORT);
static void enqueue(TDBB, LCK, USHORT, SSHORT);
static void external_ast(LCK);
static LCK find_block(LCK, USHORT);
static USHORT hash_func(UCHAR *, USHORT);
static void hash_allocate(LCK);
static LCK hash_get_lock(LCK, USHORT *, LCK **);
static void hash_insert_lock(LCK);
static BOOLEAN hash_remove_lock(LCK, LCK *);
static void internal_ast(LCK);
static BOOLEAN internal_compatible(LCK, LCK, USHORT);
static void internal_dequeue(TDBB, LCK);
static USHORT internal_downgrade(TDBB, LCK);
static BOOLEAN internal_enqueue(TDBB, LCK, USHORT, SSHORT, BOOLEAN);


/* globals and macros */

static SLONG process_lck_owner_handle = 0;

#ifdef SUPERSERVER

#define LCK_OWNER_ID_PROCESS		(SLONG) getpid()
#define LCK_OWNER_ID_DBB		(SLONG) dbb
#define LCK_OWNER_ID_ATT		(SLONG) attachment

#define LCK_OWNER_TYPE_PROCESS		LCK_OWNER_process
#define LCK_OWNER_TYPE_DBB		LCK_OWNER_dbb
#define LCK_OWNER_TYPE_ATT		LCK_OWNER_attachment

#define	LCK_OWNER_HANDLE_PROCESS	process_lck_owner_handle
#define	LCK_OWNER_HANDLE_DBB		dbb->dbb_lock_owner_handle
#define	LCK_OWNER_HANDLE_ATT		attachment->att_lock_owner_handle

#else	/* SUPERSERVER */

/* This is not a SUPERSERVER build */

#define LCK_OWNER_ID_PROCESS		(SLONG) getpid()
#define LCK_OWNER_ID_DBB		(SLONG) getpid()
#define LCK_OWNER_ID_ATT		(SLONG) getpid()

#define LCK_OWNER_TYPE_PROCESS		LCK_OWNER_process
#define LCK_OWNER_TYPE_DBB		LCK_OWNER_process
#define LCK_OWNER_TYPE_ATT		LCK_OWNER_process

#define	LCK_OWNER_HANDLE_PROCESS	process_lck_owner_handle
#define	LCK_OWNER_HANDLE_DBB		process_lck_owner_handle
#define LCK_OWNER_HANDLE_ATT		process_lck_owner_handle

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

#define LCK_max			(LCK_EX + 1)
#define COMPATIBLE(st1, st2)	compatibility [st1 * LCK_max + st2]
#define LOCK_HASH_SIZE		19

#define	ENQUEUE(lock,level,wait) if (lock->lck_compatible) \
				     internal_enqueue (tdbb, lock, level, wait, FALSE); \
		                 else enqueue (tdbb, lock, level, wait);

#define	CONVERT(lock,level,wait) (lock->lck_compatible) ? \
				 internal_enqueue (tdbb, lock, level, wait, TRUE) : \
                                 LOCK_convert (lock->lck_id, level, wait, reinterpret_cast<int(*)(void*)>(lock->lck_ast), lock->lck_object, status)

#define DEQUEUE(lock)		 if (lock->lck_compatible) \
				     internal_dequeue (tdbb, lock); \
				 else LOCK_deq (lock->lck_id);

#define DOWNGRADE(lock,status)	 (lock->lck_compatible) ? \
				 internal_downgrade (tdbb, lock) : \
			 	 LOCK_downgrade (lock->lck_id, status);

#ifdef DEV_BUILD
/* Valid locks are not NULL, 
	of the right memory block type, 
	are attached to a dbb.
   	If we have the dbb in non-exclusive mode, 
	    then we must have a physical lock of at least the same level
	    as the logical lock.
   	If we don't have a lock ID, 
	    then we better not have a physical lock at any level.

JMB: As part of the c++ conversion I removed the check for lck block type.
 There is no more blk_type field in the lck structure, and some stack allocated
 lck's are passed into lock functions, so we can't do the check.
 Here is the line I removed from the macro:
				 (l->blk_type == type_lck) && \
 */
#define LCK_CHECK_LOCK(l)	(((l) != NULL) && \
				 (l->lck_dbb != NULL) && \
				 (l->lck_test_field == 666) && \
				 (l->lck_id || (l->lck_physical == LCK_none)))

/* The following check should be part of LCK_CHECK_LOCK, but it fails
   when the exclusive attachment to a database is changed to a shared
   attachment.  When that occurs we assert all our internal locks, but
   while we are in the process of asserting DBB_assert_locks is set, but
   we haven't gotten physical locks yet.
 
			         (!(l->lck_dbb->dbb_ast_flags & DBB_assert_locks) \
				    || (l->lck_physical >= l->lck_logical)) && \
*/
#endif

#ifndef LCK_CHECK_LOCK
#define LCK_CHECK_LOCK(x)		(TRUE)	/* nothing */
#endif


void LCK_assert(TDBB tdbb, LCK lock)
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
	assert(LCK_CHECK_LOCK(lock));

	if (lock->lck_logical == lock->lck_physical ||
		lock->lck_logical == LCK_none) return;

	if (!LCK_lock(tdbb, lock, lock->lck_logical, LCK_WAIT))
		BUGCHECK(159);			/* msg 159 cannot assert logical lock */
	assert(LCK_CHECK_LOCK(lock));
}


int LCK_convert(TDBB tdbb, LCK lock, USHORT level, SSHORT wait)
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
	STATUS *status;
	DBB dbb;
	int result;

	assert(LCK_CHECK_LOCK(lock));

	SET_TDBB(tdbb);
	dbb = lock->lck_dbb;
	status = tdbb->tdbb_status_vector;

	lock->lck_attachment = tdbb->tdbb_attachment;

	result = CONVERT(lock, level, wait);

	if (!result) {
		if (status[1] == gds_deadlock ||
			status[1] == gds_lock_conflict || status[1] == gds_lock_timeout)
			return FALSE;
		if (status[1] == gds_lockmanerr)
			dbb->dbb_flags |= DBB_bugcheck;
		ERR_punt();
	}

	if (!lock->lck_compatible)
		lock->lck_physical = lock->lck_logical = level;

	assert(LCK_CHECK_LOCK(lock));
	return TRUE;
}


#ifdef MULTI_THREAD
int LCK_convert_non_blocking(TDBB tdbb, LCK lock, USHORT level, SSHORT wait)
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
	ATT attachment;
	STATUS *status;
	int result;
	DBB dbb;

	assert(LCK_CHECK_LOCK(lock));
	SET_TDBB(tdbb);

	dbb = lock->lck_dbb;
	lock->lck_attachment = tdbb->tdbb_attachment;

	if (!wait || !gds__thread_enable(FALSE))
		return LCK_convert(tdbb, lock, level, wait);

/* Save context and checkout from the scheduler */

	check_lock(lock, level);
	status = tdbb->tdbb_status_vector;
	attachment = tdbb->tdbb_attachment;
	AST_DISABLE;

/* SuperServer: Do Not release engine here, it creates a race
   condition - more than one thread RUNNING in the engine.
   We check out of the scheduler later - in wait_for_request()
   in lock/lock.c - when we are going to wait on wakeup event.
*/
#ifndef SUPERSERVER
	SCH_exit();
#endif

	result = CONVERT(lock, level, wait);

/* Check back in with the scheduler and restore context */

#ifndef SUPERSERVER
	SCH_enter();
#endif

	AST_ENABLE;

	if (!result) {
		if (status[1] == gds_deadlock ||
			status[1] == gds_lock_conflict || status[1] == gds_lock_timeout)
			return FALSE;
		if (status[1] == gds_lockmanerr)
			dbb->dbb_flags |= DBB_bugcheck;
		ERR_punt();
	}

	if (!lock->lck_compatible)
		lock->lck_physical = lock->lck_logical = level;

	assert(LCK_CHECK_LOCK(lock));
	return TRUE;
}


#else
int LCK_convert_non_blocking(TDBB tdbb, LCK lock, USHORT level, SSHORT wait)
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

	assert(LCK_CHECK_LOCK(lock));
	SET_TDBB(tdbb);
	return LCK_convert(tdbb, lock, level, wait);
}
#endif


int LCK_convert_opt(TDBB tdbb, LCK lock, USHORT level)
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
	USHORT old_level;
	DBB dbb;

	SET_TDBB(tdbb);
	assert(LCK_CHECK_LOCK(lock));
	old_level = lock->lck_logical;
	lock->lck_logical = level;
	dbb = lock->lck_dbb;

	if (dbb->dbb_ast_flags & DBB_assert_locks) {
		lock->lck_logical = old_level;
		return LCK_convert(tdbb, lock, level, LCK_NO_WAIT);
	}

	assert(LCK_CHECK_LOCK(lock));
	return TRUE;
}


#ifndef VMS
int LCK_downgrade(TDBB tdbb, LCK lock)
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
	STATUS *status;
	USHORT level;

	assert(LCK_CHECK_LOCK(lock));
	SET_TDBB(tdbb);

	status = tdbb->tdbb_status_vector;

	assert(LCK_CHECK_LOCK(lock));

	if (lock->lck_id && lock->lck_physical != LCK_none) {
		level = DOWNGRADE(lock, status);
		if (!lock->lck_compatible)
			lock->lck_physical = lock->lck_logical = level;
	}

	if (lock->lck_physical == LCK_none)
		lock->lck_id = lock->lck_data = 0;

	assert(LCK_CHECK_LOCK(lock));
	return TRUE;
}
#endif


void LCK_fini(TDBB tdbb, enum lck_owner_t owner_type)
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
	DBB dbb;
	ATT attachment;
	SLONG *owner_handle_ptr;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	attachment = tdbb->tdbb_attachment;

	switch (owner_type) {
	case LCK_OWNER_process:
		owner_handle_ptr = &(LCK_OWNER_HANDLE_PROCESS);
		break;

	case LCK_OWNER_database:
		owner_handle_ptr = &(LCK_OWNER_HANDLE_DBB);
		break;

	case LCK_OWNER_attachment:
		owner_handle_ptr = &(LCK_OWNER_HANDLE_ATT);
		break;

	default:
		bug_lck("Invalid lock owner type in LCK_fini ()");
		break;
	}

	LOCK_fini(tdbb->tdbb_status_vector, owner_handle_ptr);
}


SLONG LCK_get_owner_handle(TDBB tdbb, enum lck_t lock_type)
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
	DBB dbb = tdbb->tdbb_database;
	ATT attachment = tdbb->tdbb_attachment;
#endif
	switch (lock_type) {
	case LCK_database:
	case LCK_bdb:
	case LCK_rel_exist:
	case LCK_idx_exist:
	case LCK_shadow:
	case LCK_retaining:
	case LCK_expression:
	case LCK_record_locking:
	case LCK_prc_exist:
	case LCK_range_relation:
		return LCK_OWNER_HANDLE_DBB;
	case LCK_attachment:
	case LCK_relation:
	case LCK_file_extend:
	case LCK_tra:
	case LCK_sweep:
	case LCK_record:
	case LCK_update_shadow:
		return LCK_OWNER_HANDLE_ATT;
	default:
		bug_lck("Invalid lock type in LCK_get_owner_handle ()");
		/* Not Reached - bug_lck calls ERR_post */
		return 0;
	}
}


void LCK_init(TDBB tdbb, enum lck_owner_t owner_type)
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
	DBB dbb;
	ATT attachment;
	SLONG owner_id;
	SLONG *owner_handle_ptr;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	attachment = tdbb->tdbb_attachment;

	switch (owner_type) {
	case LCK_OWNER_process:
		owner_id = LCK_OWNER_ID_PROCESS;
		owner_handle_ptr = &(LCK_OWNER_HANDLE_PROCESS);
		break;

	case LCK_OWNER_database:
		owner_id = LCK_OWNER_ID_DBB;
		owner_handle_ptr = &(LCK_OWNER_HANDLE_DBB);
		break;

	case LCK_OWNER_attachment:
		owner_id = LCK_OWNER_ID_ATT;
		owner_handle_ptr = &(LCK_OWNER_HANDLE_ATT);
		break;

	default:
		bug_lck("Invalid lock owner type in LCK_init ()");
		break;
	}

	if (LOCK_init(tdbb->tdbb_status_vector, TRUE,
				  owner_id, owner_type, owner_handle_ptr)) {
		if (tdbb->tdbb_status_vector[1] == gds_lockmanerr)
			dbb->dbb_flags |= DBB_bugcheck;
		ERR_punt();
	}
}


int LCK_lock(TDBB tdbb, LCK lock, USHORT level, SSHORT wait)
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
	STATUS *status;
	DBB dbb;

	assert(LCK_CHECK_LOCK(lock));
	SET_TDBB(tdbb);

	dbb = lock->lck_dbb;
	status = tdbb->tdbb_status_vector;
	lock->lck_blocked_threads = NULL;
	lock->lck_next = lock->lck_prior = NULL;
	lock->lck_attachment = tdbb->tdbb_attachment;

	ENQUEUE(lock, level, wait);
	assert(LCK_CHECK_LOCK(lock));
	if (!lock->lck_id)
		if (!wait ||
			status[1] == gds_deadlock ||
			status[1] == gds_lock_conflict || status[1] == gds_lock_timeout) {
			return FALSE;
		}
		else {
			if (status[1] == gds_lockmanerr)
				dbb->dbb_flags |= DBB_bugcheck;
			ERR_punt();
		}

	if (!lock->lck_compatible)
		lock->lck_physical = lock->lck_logical = level;

	assert(LCK_CHECK_LOCK(lock));
	return TRUE;
}


int LCK_lock_non_blocking(TDBB tdbb, LCK lock, USHORT level, SSHORT wait)
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
	ATT attachment;
	STATUS *status;
	LCK next;
	DBB dbb;

	assert(LCK_CHECK_LOCK(lock));
	SET_TDBB(tdbb);

	dbb = lock->lck_dbb;
	lock->lck_attachment = attachment = tdbb->tdbb_attachment;

/* Don't bother for the non-wait or non-multi-threading case */

	if (!wait || !gds__thread_enable(FALSE))
		return LCK_lock(tdbb, lock, level, wait);

/* Make sure we're not about to wait for ourselves */

	lock->lck_blocked_threads = NULL;
	lock->lck_next = lock->lck_prior = NULL;
	check_lock(lock, level);
	status = tdbb->tdbb_status_vector;

/* If we can get the lock without waiting, save a great
   deal of grief */

	ENQUEUE(lock, level, LCK_NO_WAIT);
	assert(LCK_CHECK_LOCK(lock));
	if (!lock->lck_id) {
		/* Save context and checkout from the scheduler */

		AST_DISABLE;

/* SuperServer: Do Not release engine here, it creates a race
   condition - more than one thread RUNNING in the engine.
   We check out of the scheduler later - in wait_for_request()
   in lock/lock.c - when we are going to wait on wakeup event.
*/
#ifndef SUPERSERVER
		SCH_exit();
#endif

		INIT_STATUS(status);
		ENQUEUE(lock, level, wait);

		/* Check back in with the scheduler and restore context */

#ifndef SUPERSERVER
		SCH_enter();
#endif
		AST_ENABLE;

		assert(LCK_CHECK_LOCK(lock));

		/* If lock was rejected, there's trouble */

		if (!lock->lck_id)
			if (status[1] == gds_deadlock ||
				status[1] == gds_lock_conflict ||
				status[1] == gds_lock_timeout) {
				return FALSE;
			}
			else {
				if (status[1] == gds_lockmanerr)
					dbb->dbb_flags |= DBB_bugcheck;
				ERR_punt();
			}
	}

	if (!lock->lck_compatible)
		lock->lck_physical = lock->lck_logical = level;

/* Link into list of two phased locks */

	lock->lck_next = attachment->att_long_locks;
	lock->lck_prior = NULL;
	attachment->att_long_locks = lock;

	if (next = lock->lck_next)
		next->lck_prior = lock;

	assert(LCK_CHECK_LOCK(lock));
	return TRUE;

#else

	SET_TDBB(tdbb);
	assert(LCK_CHECK_LOCK(lock));

	return LCK_lock(tdbb, lock, level, wait);
#endif
}


int LCK_lock_opt(TDBB tdbb, LCK lock, USHORT level, SSHORT wait)
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
	assert(LCK_CHECK_LOCK(lock));
	lock->lck_logical = level;
	DBB dbb = lock->lck_dbb;

	if (dbb->dbb_ast_flags & DBB_assert_locks) {
		lock->lck_logical = LCK_none;
		return LCK_lock(tdbb, lock, level, wait);
	}

	assert(LCK_CHECK_LOCK(lock));
	return TRUE;
}


#ifndef VMS
SLONG LCK_query_data(LCK parent, enum lck_t lock_type, USHORT aggregate)
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

	assert(LCK_CHECK_LOCK(parent));
	return LOCK_query_data(parent->lck_id, lock_type, aggregate);
}
#endif


SLONG LCK_read_data(LCK lock)
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
	SLONG data;
	LCK parent;

	assert(LCK_CHECK_LOCK(lock));
#ifdef VMS
	if (!LCK_lock(NULL_TDBB, lock, LCK_null, LCK_NO_WAIT))
		return 0;

	data = LOCK_read_data(lock->lck_id);
	LCK_release(lock);
#else
	data = LOCK_read_data2(
						   (parent = lock->lck_parent) ? parent->lck_id : 0,
						   lock->lck_type,
						   (UCHAR *) & lock->lck_key,
						   lock->lck_length, lock->lck_owner_handle);
#endif

	assert(LCK_CHECK_LOCK(lock));
	return data;
}


void LCK_release(TDBB tdbb, LCK lock)
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

#ifdef MULTI_THREAD
	ATT attachment = lock->lck_attachment;
#endif

	SET_TDBB(tdbb);
	assert(LCK_CHECK_LOCK(lock));

	if (lock->lck_physical != LCK_none) {
		DEQUEUE(lock);
	}

	lock->lck_physical = lock->lck_logical = LCK_none;
	lock->lck_id = lock->lck_data = 0;

#ifdef MULTI_THREAD

	LCK next = lock->lck_next;
	LCK prior = lock->lck_prior;

	if (prior) {
		assert(prior->lck_next == lock);
		prior->lck_next = next;
	}
	else if (attachment) {
		if (attachment->att_long_locks == lock)
			attachment->att_long_locks = lock->lck_next;
	}

	if (next) {
		assert(next->lck_prior == lock);
		next->lck_prior = prior;
	}

	lock->lck_next = NULL;
	lock->lck_prior = NULL;

	if (lock->lck_blocked_threads)
		JRD_unblock(&lock->lck_blocked_threads);
#endif

	assert(LCK_CHECK_LOCK(lock));
}


void LCK_re_post(LCK lock)
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

	assert(LCK_CHECK_LOCK(lock));
	if (lock->lck_compatible) {
		if (lock->lck_ast) {
			reinterpret_cast<int (*)(blk*)>(*lock->lck_ast)(lock->lck_object);
		}
		return;
	}

	LOCK_re_post(reinterpret_cast<int (*)(void*)>(lock->lck_ast),
				 lock->lck_object, lock->lck_owner_handle);
	assert(LCK_CHECK_LOCK(lock));
}


void LCK_write_data(LCK lock, SLONG data)
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

	assert(LCK_CHECK_LOCK(lock));
	LOCK_write_data(lock->lck_id, data);
	lock->lck_data = data;
	assert(LCK_CHECK_LOCK(lock));
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
	ERR_post(gds_db_corrupt, gds_arg_string, string, 0);
}


#ifdef MULTI_THREAD
static void check_lock(LCK lock, USHORT level)
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
	LCK next;

	assert(LCK_CHECK_LOCK(lock));
	while (next = find_block(lock, level))
		JRD_blocked(next->lck_attachment, &next->lck_blocked_threads);
}
#endif


static BOOLEAN compatible(LCK lock1, LCK lock2, USHORT level2)
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

	assert(LCK_CHECK_LOCK(lock1));
	assert(LCK_CHECK_LOCK(lock2));

/* if the locks have the same compatibility block,
   they are always compatible regardless of level */

	if (lock1->lck_compatible &&
		lock2->lck_compatible &&
		lock1->lck_compatible == lock2->lck_compatible) {
		/* check for a second level of compatibility as well:
		   if a second level was specified, the locks must 
		   also be compatible at the second level */

		if (!lock1->lck_compatible2 ||
			!lock2->lck_compatible2 ||
			lock1->lck_compatible2 == lock2->lck_compatible2) return TRUE;
	}

	if (COMPATIBLE(lock1->lck_logical, level2))
		return TRUE;

	return FALSE;
}


static void enqueue(TDBB tdbb, LCK lock, USHORT level, SSHORT wait)
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

	LCK parent;

	SET_TDBB(tdbb);

	assert(LCK_CHECK_LOCK(lock));

	lock->lck_id = LOCK_enq(lock->lck_id,
							(parent = lock->lck_parent) ? parent->lck_id : 0,
							lock->lck_type,
							(UCHAR *) & lock->lck_key,
							lock->lck_length,
							level,
							reinterpret_cast<int (*)(void*)>(lock->lck_ast),
							lock->lck_object, lock->lck_data, wait,
							tdbb->tdbb_status_vector, lock->lck_owner_handle);
	if (!lock->lck_id)
		lock->lck_physical = lock->lck_logical = LCK_none;
	assert(LCK_CHECK_LOCK(lock));
}


static void external_ast(LCK lock)
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
	LCK match, next;

	assert(LCK_CHECK_LOCK(lock));

/* go through the list, saving the next lock in the list
   in case the current one gets deleted in the ast */

	for (match = hash_get_lock(lock, 0, 0); match; match = next) {
		next = match->lck_identical;
		if (match->lck_ast)
			reinterpret_cast<int (*)(...)>(*match->lck_ast)(match->lck_object);
	}
}



#ifdef MULTI_THREAD
static LCK find_block(LCK lock, USHORT level)
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
	LCK next;
	SCHAR *p1, *p2, *end;
	DBB dbb;
	ATT attachment;

	assert(LCK_CHECK_LOCK(lock));

	dbb = lock->lck_dbb;

	if (!(attachment = lock->lck_attachment))
		return NULL;

	end = lock->lck_key.lck_string + lock->lck_length;

	for (next = attachment->att_long_locks; next; next = next->lck_next)
		if (lock->lck_type == next->lck_type &&
			lock->lck_parent && next->lck_parent &&
			(lock->lck_parent->lck_id == next->lck_parent->lck_id) &&
			lock->lck_length == next->lck_length &&
			lock->lck_attachment != next->lck_attachment &&
			!compatible(next, lock, level)) {
			for (p1 = lock->lck_key.lck_string, p2 = next->lck_key.lck_string;
				 p1 < end && *p1++ == *p2++;);
			if (p1[-1] == p2[-1])
				return next;
		}

	return NULL;
}
#endif


static USHORT hash_func(UCHAR * value, USHORT length)
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
	ULONG hash_value;
	UCHAR *p, *q;
	USHORT l;

/* Hash the value, preserving its distribution 
   as much as possible */

	hash_value = 0;
	q = value;

	for (l = 0, q = value; l < length; l++) {
		if (!(l & 3))
			p = (UCHAR *) & hash_value;
		*p++ = *q++;
	}

	return (USHORT) (hash_value % LOCK_HASH_SIZE);
}


static void hash_allocate(LCK lock)
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
	DBB dbb;
	ATT att;

	assert(LCK_CHECK_LOCK(lock));

	dbb = lock->lck_dbb;

	if ( (att = lock->lck_attachment) ) {
		att->att_compatibility_table =
			vec::newVector(*dbb->dbb_permanent, LOCK_HASH_SIZE);
	}
}


static LCK hash_get_lock(LCK lock, USHORT * hash_slot, LCK ** prior)
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
	LCK match, collision;
	SCHAR *p, *q;
	SSHORT l;
	USHORT hash_value;
	ATT att;

	assert(LCK_CHECK_LOCK(lock));

	if (!(att = lock->lck_attachment))
		return NULL;

	if (!att->att_compatibility_table)
		hash_allocate(lock);

	hash_value = hash_func((UCHAR *) & lock->lck_key, lock->lck_length);
	if (hash_slot)
		*hash_slot = hash_value;

/* if no collisions found, we're done */

	if (!(match = (LCK) (*att->att_compatibility_table)[hash_value]))
		return NULL;
	if (prior)
		*prior =
			(LCK *) & (*att->att_compatibility_table)[hash_value];

/* look for an identical lock */

	assert(LCK_CHECK_LOCK(match));
	for (collision = match; collision; collision = collision->lck_collision) {
		assert(LCK_CHECK_LOCK(collision));
		if (collision->lck_parent && lock->lck_parent &&
			collision->lck_parent->lck_id == lock->lck_parent->lck_id &&
			collision->lck_type == lock->lck_type &&
			collision->lck_length == lock->lck_length) {
			/* check that the keys are the same */

			p = lock->lck_key.lck_string;
			q = collision->lck_key.lck_string;
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


static void hash_insert_lock(LCK lock)
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
	LCK identical;
	USHORT hash_slot;
	ATT att;

	assert(LCK_CHECK_LOCK(lock));

	if (!(att = lock->lck_attachment))
		return;

/* if no identical is returned, place it in the collision list */

	if (!(identical = hash_get_lock(lock, &hash_slot, 0))) {
		lock->lck_collision =
			(LCK) (*att->att_compatibility_table)[hash_slot];
		(*att->att_compatibility_table)[hash_slot] = (BLK) lock;
		return;
	}

/* place it second in the list, out of pure laziness */

	lock->lck_identical = identical->lck_identical;
	identical->lck_identical = lock;
}


static BOOLEAN hash_remove_lock(LCK lock, LCK * match)
{
/**************************************
 *
 *	h a s h _ r e m o v e _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Remove the passed lock from the hash table.
 *	Return TRUE if this is the last such identical 
 *	lock removed.   Also return the first matching 
 *	locking found.
 *
 **************************************/
	LCK next, last, *prior;

	assert(LCK_CHECK_LOCK(lock));

	if (!(next = hash_get_lock(lock, 0, &prior))) {
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
			return FALSE;
		}
		else {
			*prior = lock->lck_collision;
			return TRUE;
		}

	for (; next; last = next, next = next->lck_identical)
		if (next == lock)
			break;

	if (!next) {
		lock->lck_compatible = NULL;
		BUGCHECK(285);			/* lock not found in internal lock manager */
	}

	last->lck_identical = next->lck_identical;
	return FALSE;
}


static void internal_ast(LCK lock)
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
	LCK match, next;

	assert(LCK_CHECK_LOCK(lock));

/* go through the list, saving the next lock in the list
   in case the current one gets deleted in the ast */

	for (match = hash_get_lock(lock, 0, 0); match; match = next) {
		next = match->lck_identical;

		/* don't deliver the ast to any locks which are already compatible */

		if ((match != lock) &&
			!compatible(match, lock, lock->lck_logical) &&
			match->lck_ast)
		{
			reinterpret_cast<int (*)(...)>(*match->lck_ast)(match->lck_object);
		}
	}
}



static BOOLEAN internal_compatible(LCK match, LCK lock, USHORT level)
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
 *	If there are none, return TRUE to indicate 
 *	that the lock is compatible.
 *
 **************************************/
	LCK next;

	assert(LCK_CHECK_LOCK(match));
	assert(LCK_CHECK_LOCK(lock));

/* first check if there are any locks which are 
   incompatible which do not have blocking asts;
   if so, there is no chance of getting a compatible 
   lock */

	for (next = match; next; next = next->lck_identical)
		if (!next->lck_ast && !compatible(next, lock, level))
			return FALSE;

/* now deliver the blocking asts, attempting to gain
   compatibility by getting everybody to downgrade */

	internal_ast(match);

/* make one more pass to see if all locks were downgraded */

	for (next = match; next; next = next->lck_identical)
		if (!compatible(next, match, level))
			return FALSE;

	return TRUE;
}


static void internal_dequeue(TDBB tdbb, LCK lock)
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
	LCK match;

	SET_TDBB(tdbb);
	assert(LCK_CHECK_LOCK(lock));
	assert(lock->lck_compatible);

/* if this is the last identical lock in the hash table, release it */

	if (hash_remove_lock(lock, &match)) {
		if (!LOCK_deq(lock->lck_id))
			bug_lck("LOCK_deq() failed in lck:internal_dequeue");
		lock->lck_id = 0;
		lock->lck_physical = lock->lck_logical = LCK_none;
		return;
	}

/* check for a potential downgrade */

	(void) internal_downgrade(tdbb, match);
}


static USHORT internal_downgrade(TDBB tdbb, LCK first)
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
	LCK lock;
	USHORT level = LCK_none;

	SET_TDBB(tdbb);

	assert(LCK_CHECK_LOCK(first));
	assert(first->lck_compatible);

/* find the highest required lock level */

	for (lock = first; lock; lock = lock->lck_identical)
		level = MAX(level, lock->lck_logical);

/* if we can convert to that level, set all identical 
   locks as having that level */

	if ((level < first->lck_physical) &&
		LOCK_convert(first->lck_id,
					 level,
					 LCK_NO_WAIT,
					 reinterpret_cast < int (*)(void *) >(external_ast),
					 (int *) first, tdbb->tdbb_status_vector)) {
		for (lock = first; lock; lock = lock->lck_identical)
			lock->lck_physical = level;
		return level;
	}

	return first->lck_physical;
}


static BOOLEAN internal_enqueue(
								TDBB tdbb,
								LCK lock,
								USHORT level,
								SSHORT wait, BOOLEAN convert_flg)
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
	LCK match, update;
	STATUS *status;

	assert(LCK_CHECK_LOCK(lock));
	assert(lock->lck_compatible);

	SET_TDBB(tdbb);
	status = tdbb->tdbb_status_vector;

/* look for an identical lock */

	if ( (match = hash_get_lock(lock, 0, 0)) ) {
		/* if there are incompatible locks for which
		   there are no blocking asts defined, give up */

		if (!internal_compatible(match, lock, level)) {
			/* for now return a lock conflict; it would be better if we were to 
			   do a wait on the other lock by setting some flag bit or some such */

			status[0] = gds_arg_gds;
			status[1] = gds_lock_conflict;
			status[2] = gds_arg_end;
			return FALSE;
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
								  reinterpret_cast <
								  int (*)(void *) >(external_ast),
								  (int *) lock, status))
					  return FALSE;
				for (update = match; update; update = update->lck_identical)
					update->lck_physical = level;
			}

			lock->lck_id = match->lck_id;
			lock->lck_logical = level;
			lock->lck_physical = match->lck_physical;

			/* When converting a lock (from the callers point of view), 
			   then no new lock needs to be inserted. */

			if (!convert_flg)
				hash_insert_lock(lock);

			return TRUE;
		}
	}

/* enqueue the lock, but swap out the ast and the ast argument
   with the local ast handler, passing it the lock block itself */

	lock->lck_id = LOCK_enq(lock->lck_id,
							lock->lck_parent ? lock->lck_parent->lck_id : 0,
							lock->lck_type,
							(UCHAR *) & lock->lck_key,
							lock->lck_length,
							level,
							reinterpret_cast < int (*)(void *)>(external_ast),
							(int *)lock,
							lock->lck_data,
							wait, status, lock->lck_owner_handle);

/* If the lock exchange failed, set the lock levels appropriately */
	if (lock->lck_id == 0)
		lock->lck_physical = lock->lck_logical = LCK_none;

	assert(LCK_CHECK_LOCK(lock));

	if (lock->lck_id) {
		hash_insert_lock(lock);
		lock->lck_logical = lock->lck_physical = level;
	}

	assert(LCK_CHECK_LOCK(lock));
	return lock->lck_id ? TRUE : FALSE;
}


} // extern "C"
