/*
 *	PROGRAM:	JRD Lock Manager
 *	MODULE:		lock.cpp
 *	DESCRIPTION:	Generic ISC Lock Manager
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
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "IMP" port
 *
 * 2002.10.27 Sean Leyne - Completed removal of obsolete "DELTA" port
 * 2002.10.27 Sean Leyne - Completed removal of obsolete "IMP" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 * 2003.03.24 Nickolay Samofatov
 *	- cleanup #define's,
 *  - shutdown blocking thread cleanly on Windows CS
 *  - fix Windows CS lock-ups (make wakeup event manual-reset)
 *  - detect deadlocks instantly in most cases (if blocking owner 
 *     dies during AST processing deadlock scan timeout still applies)
 * 2003.04.29 Nickolay Samofatov - fix broken lock table resizing code in CS builds
 * 2003.08.11 Nickolay Samofatov - finally and correctly fix Windows CS lock-ups. 
 *            Roll back earlier workarounds on this subject.
 *
 */

/*
$Id: lock.cpp,v 1.110 2004-12-08 06:03:05 robocop Exp $
*/

#include "firebird.h"
#include "../jrd/jrd_time.h"
#include <stdio.h>
#include "../jrd/common.h"
#include "../jrd/thd.h"
#include "../jrd/isc.h"
#include "../lock/lock.h"
#include "../lock/lock_proto.h"
#include "gen/iberror.h"
#include "../jrd/gds_proto.h"
#include "../jrd/gdsassert.h"
#include "../jrd/isc_proto.h"
#include "../jrd/os/isc_i_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thread_proto.h"
#include "../common/config/config.h"
#include <errno.h>

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_VFORK_H
#include <vfork.h>
#endif

#ifdef WIN_NT
#include <process.h>
#define MUTEX		lock_manager_mutex
#else
#define MUTEX		LOCK_header->lhb_mutex
#endif

#ifdef MANAGER_PROCESS
#include <sys/stat.h>
#include <sys/file.h>
#define statistics	stat
static bool LOCK_post_manager;
#endif

#ifdef DEV_BUILD
#define ASSERT_ACQUIRED current_is_active_owner (true,  __LINE__)
#define ASSERT_RELEASED current_is_active_owner (false, __LINE__)
#define VALIDATE_LOCK_TABLE
#if ((defined HAVE_MMAP) && !(defined SUPERSERVER))
#define LOCK_DEBUG_ACQUIRE
#define DEBUG_ACQUIRE_INTERVAL 5000
static ULONG debug_acquire_count = 0;
#endif
#define CHECK(x)	{ if (!(x)) bug_assert ("consistency check", __LINE__); }
//#define DEBUG_TRACE
#else // DEV_BUILD
#define	ASSERT_ACQUIRED			/* nothing */
#define	ASSERT_RELEASED			/* nothing */
#define CHECK(x)				/* nothing */
#endif // DEV_BUILD

#ifdef DEBUG
#define DEBUG_MANAGER "manager"
#define DEBUG_TRACE
#endif

#ifdef DEBUG_TRACE
#define LOCK_TRACE(x)	{ time_t t; time(&t); printf("%s", ctime(&t) ); printf x; fflush (stdout); gds__log x;}
#else
#define LOCK_TRACE(x)			/* nothing */
#endif

#ifdef DEBUG
SSHORT LOCK_debug_level = 0;
#define DEBUG_MSG(l, x)	if ((l) <= LOCK_debug_level) { time_t t; time(&t); printf("%s", ctime(&t) ); printf x; fflush (stdout); gds__log x; }
#else
#define DEBUG_MSG(l, x)			/* nothing */
#endif

/* Debug delay is used to create nice big windows for signals or other
 * events to occur in - eg: slow down the code to try and make
 * timing race conditions show up
 */
#ifdef DEBUG
#define	DEBUG_DELAY	debug_delay (__LINE__)
#else
#define DEBUG_DELAY				/* nothing */
#endif

const SRQ_PTR DUMMY_OWNER_CREATE	= -1;
const SRQ_PTR DUMMY_OWNER_DELETE	= -2;
const SRQ_PTR DUMMY_OWNER_SHUTDOWN	= -3;

static void acquire(SRQ_PTR);
static UCHAR *alloc(SSHORT, ISC_STATUS *);
static LBL alloc_lock(USHORT, ISC_STATUS *);
#ifdef USE_STATIC_SEMAPHORES
static USHORT alloc_semaphore(OWN, ISC_STATUS *);
#endif
#ifndef SUPERSERVER
// This is either signal handler of called from blocking_thread
// only SuperServer does direct calls to blocking_action2
static void blocking_action(void *_owner_offset);
#endif
static void blocking_action2(SRQ_PTR, SRQ_PTR);
#ifdef USE_BLOCKING_THREAD
static THREAD_ENTRY_DECLARE blocking_action_thread(THREAD_ENTRY_PARAM);
#endif
static void bug(ISC_STATUS *, const TEXT *);
#ifdef DEV_BUILD
static void bug_assert(const TEXT *, ULONG);
#endif
static bool convert(SRQ_PTR, UCHAR, SSHORT, lock_ast_t, void*, ISC_STATUS*);
static USHORT create_owner(ISC_STATUS *, LOCK_OWNER_T, UCHAR, SLONG *);
#ifdef DEV_BUILD
static void current_is_active_owner(bool, ULONG);
#endif
static void deadlock_clear(void);
static LRQ deadlock_scan(OWN, LRQ);
static LRQ deadlock_walk(LRQ, bool *);
static void dequeue(SRQ_PTR);
#ifdef DEBUG
static void debug_delay(ULONG);
#endif
static void exit_handler(void *);
static LBL find_lock(SRQ_PTR, USHORT, const UCHAR*, USHORT, USHORT*);
#ifdef MANAGER_PROCESS
static bool fork_lock_manager(ISC_STATUS *);
static OWN get_manager(bool);
#endif
static LRQ get_request(SRQ_PTR);
static void grant(LRQ, LBL);
static SRQ_PTR grant_or_que(LRQ, LBL, SSHORT);
static ISC_STATUS init_lock_table(ISC_STATUS *);
static void init_owner_block(OWN, UCHAR, LOCK_OWNER_T, USHORT);
#ifdef USE_WAKEUP_EVENTS
static void lock_alarm_handler(void *event);
#endif
static void lock_initialize(void*, SH_MEM, bool);
static void insert_data_que(LBL);
static void insert_tail(SRQ, SRQ);
static USHORT lock_state(LBL);
//static void port_waker(SRQ_PTR *);
static void post_blockage(LRQ, LBL, bool);
static void post_history(USHORT, SRQ_PTR, SRQ_PTR, SRQ_PTR, bool);
static void post_pending(LBL);
static void post_wakeup(OWN);
#ifndef SUPERSERVER
static bool probe_owners(SRQ_PTR);
#endif
static void purge_owner(SRQ_PTR, OWN);
static void remove_que(SRQ);
static void release(SRQ_PTR);
static void release_mutex(void);
static void release_request(LRQ);
#ifdef USE_STATIC_SEMAPHORES
static void release_semaphore(OWN);
#endif
#ifdef USE_BLOCKING_THREAD
static void shutdown_blocking_thread(ISC_STATUS *);
#endif
static int signal_owner(OWN, SRQ_PTR);
#ifdef VALIDATE_LOCK_TABLE
static void validate_lhb(LHB);
static void validate_shb(SRQ_PTR);
static void validate_owner(SRQ_PTR, USHORT);
static void validate_lock(SRQ_PTR, USHORT, SRQ_PTR);
static void validate_request(SRQ_PTR, USHORT, USHORT);
//static void validate_block(SRQ_PTR);
#endif

static USHORT wait_for_request(LRQ, SSHORT, ISC_STATUS *);
//static void wakeup_action(SRQ_PTR *);

static own LOCK_process_owner;	/* Place holder */
static SSHORT LOCK_bugcheck = 0;
static LHB volatile LOCK_header = NULL;
static SRQ_PTR LOCK_owner_offset = 0;
static OWN LOCK_owner = 0;
#if defined(USE_BLOCKING_SIGNALS) || defined(DEBUG)
static SSHORT volatile LOCK_asts = -1;
#endif
static int LOCK_pid = 0, LOCK_version = 0;
static SLONG LOCK_shm_size, LOCK_sem_count;

#if (defined SOLARIS_MT && !defined SUPERSERVER)
static SLONG LOCK_solaris_stall;
#endif

static SLONG LOCK_block_signal;

#ifdef WAKEUP_SIGNAL
static SLONG LOCK_wakeup_signal = 0;
#endif

static SLONG LOCK_ordering;
static SLONG LOCK_hash_slots;
static SLONG LOCK_scan_interval;
static SLONG LOCK_acquire_spins;
static SH_MEM_T LOCK_data;
static bool start_manager = false;
static TEXT LOCK_bug_buffer[128];


#ifdef WIN_NT
static MTX_T	lock_manager_mutex[1];
#ifndef SUPERSERVER
static HANDLE	blocking_event[1];
static HANDLE	wakeup_event[1];
#endif
#endif

#define GET_TIME	time (NULL)

const SLONG HASH_MIN_SLOTS	= 101;
const SLONG HASH_MAX_SLOTS	= 2048;
const USHORT HISTORY_BLOCKS	= 256;
const int LOCKMANTIMEOUT	= 300;

#if (defined SOLARIS_MT && !defined SUPERSERVER)
const int STARVATION_THRESHHOLD	= 500;	/* acquires of lock table */
const SLONG SOLARIS_MIN_STALL	= 0;
const SLONG SOLARIS_MAX_STALL	= 200;	/* Seconds */
#endif

const USHORT OWN_BLOCK_new		= 1;
const USHORT OWN_BLOCK_reused	= 2;
const USHORT OWN_BLOCK_dummy	= 3;

const char* LOCK_MANAGER	= "bin/fb_lock_mgr";

#if defined WIN_NT && defined USE_BLOCKING_THREAD
static HANDLE blocking_action_thread_handle;
#endif

#define SRQ_BASE                    ((UCHAR*) LOCK_header)

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

void LOCK_ast_inhibit() {
#ifdef MULTI_THREAD
	AST_DISABLE();
#else
	ISC_inhibit();
#endif
}

void LOCK_ast_enable() {
#ifdef MULTI_THREAD
	AST_ENABLE();
#else
	ISC_enable();
#endif
}

bool LOCK_convert(SRQ_PTR		request_offset,
				 UCHAR		type,
				 SSHORT		lck_wait,
				 lock_ast_t ast_routine,
				 void*		ast_argument,
				 ISC_STATUS*	status_vector)
{
/**************************************
 *
 *	L O C K _ c o n v e r t
 *
 **************************************
 *
 * Functional description
 *	Perform a lock conversion, if possible.
 *
 **************************************/
	LOCK_TRACE(("LOCK_convert (%d, %d)\n", type, lck_wait));

	lrq* request = get_request(request_offset);
	own* owner = (OWN) SRQ_ABS_PTR(request->lrq_owner);
	if (!owner->own_count)
		return false;

	acquire(request->lrq_owner);
	owner = NULL;				/* remap */
	++LOCK_header->lhb_converts;
	request = (LRQ) SRQ_ABS_PTR(request_offset);	/* remap */
	lbl* lock = (LBL) SRQ_ABS_PTR(request->lrq_lock);
	if (lock->lbl_series < LCK_MAX_SERIES)
		++LOCK_header->lhb_operations[lock->lbl_series];
	else
		++LOCK_header->lhb_operations[0];

	if (convert(request_offset, type, lck_wait, ast_routine, ast_argument,
				   status_vector))
	{
		return true;
	}
	else
		return false;
}


int LOCK_deq( SRQ_PTR request_offset)
{
/**************************************
 *
 *	L O C K _ d e q
 *
 **************************************
 *
 * Functional description
 *	Release an outstanding lock.
 *
 **************************************/
	LOCK_TRACE(("LOCK_deq (%ld)\n", request_offset));

	lrq* request = get_request(request_offset);
	SRQ_PTR owner_offset = request->lrq_owner;
	own* owner = (OWN) SRQ_ABS_PTR(owner_offset);
	if (!owner->own_count)
		return FALSE;

	acquire(owner_offset);
	owner = NULL;				/* remap */
	++LOCK_header->lhb_deqs;
	request = (LRQ) SRQ_ABS_PTR(request_offset);	/* remap */
	lbl* lock = (LBL) SRQ_ABS_PTR(request->lrq_lock);
	if (lock->lbl_series < LCK_MAX_SERIES)
		++LOCK_header->lhb_operations[lock->lbl_series];
	else
		++LOCK_header->lhb_operations[0];

	dequeue(request_offset);
	release(owner_offset);
	return TRUE;
}


UCHAR LOCK_downgrade(SRQ_PTR request_offset, ISC_STATUS * status_vector)
{
/**************************************
 *
 *	L O C K _ d o w n g r a d e
 *
 **************************************
 *
 * Functional description
 *	Downgrade an existing lock returning
 *	its new state.
 *
 **************************************/
	LOCK_TRACE(("LOCK_downgrade (%ld)\n", request_offset));

	lrq* request = get_request(request_offset);
	SRQ_PTR owner_offset = request->lrq_owner;
	own* owner = (OWN) SRQ_ABS_PTR(owner_offset);
	if (!owner->own_count)
		return FALSE;

	acquire(owner_offset);
	owner = NULL;				/* remap */
	++LOCK_header->lhb_downgrades;

	request = (LRQ) SRQ_ABS_PTR(request_offset);	/* Re-init after a potential remap */
	lbl* lock = (LBL) SRQ_ABS_PTR(request->lrq_lock);
	UCHAR pending_state = LCK_none;

/* Loop thru requests looking for pending conversions
   and find the highest requested state */

	srq* lock_srq;
	SRQ_LOOP(lock->lbl_requests, lock_srq) {
		const lrq* pending = (LRQ) ((UCHAR *) lock_srq - OFFSET(LRQ, lrq_lbl_requests));
		if (pending->lrq_flags & LRQ_pending && pending != request) {
			pending_state = MAX(pending->lrq_requested, pending_state);
			if (pending_state == LCK_EX)
				break;
		}
	}

	UCHAR state = request->lrq_state;
	while (state > LCK_none && !COMPATIBLE(pending_state, state))
		--state;

	if (state == LCK_none || state == LCK_null) {
		dequeue(request_offset);
		release(owner_offset);
		state = LCK_none;
	}
	else {
		convert(request_offset, state, FALSE,
				request->lrq_ast_routine, request->lrq_ast_argument,
				status_vector);
	}

	return state;
}


SLONG LOCK_enq(	SRQ_PTR		prior_request,
				SRQ_PTR		parent_request,
				USHORT	series,
				const UCHAR*	value,
				USHORT	length,
				UCHAR	type,
				lock_ast_t ast_routine,
				void*	ast_argument,
				SLONG	data,
				SSHORT	lck_wait,
				ISC_STATUS*	status_vector,
				SRQ_PTR		owner_offset)
{
/**************************************
 *
 *	L O C K _ e n q
 *
 **************************************
 *
 * Functional description
 *	Enque on a lock.  If the lock can't be granted immediately,
 *	return an event count on which to wait.  If the lock can't
 *	be granted because of deadlock, return NULL.
 *
 **************************************/
	LOCK_TRACE(("LOCK_enq (%ld)\n", parent_request));

	own* owner = (OWN) SRQ_ABS_PTR(owner_offset);
	if (!owner_offset || !owner->own_count)
		return 0;

	acquire(owner_offset);
	owner = NULL;				/* remap */

	ASSERT_ACQUIRED;
	++LOCK_header->lhb_enqs;

#ifdef VALIDATE_LOCK_TABLE
	if ((LOCK_header->lhb_enqs % 50) == 0)
		validate_lhb(LOCK_header);
#endif

	if (prior_request)
		dequeue(prior_request);

	lrq* request = 0;
	SRQ_PTR parent;
	if (parent_request) {
		request = get_request(parent_request);
		parent = request->lrq_lock;
	}
	else
		parent = 0;

/* Allocate or reuse a lock request block */

	ASSERT_ACQUIRED;
	if (SRQ_EMPTY(LOCK_header->lhb_free_requests)) {
		if (!(request = (LRQ) alloc(sizeof(lrq), status_vector))) {
			release(owner_offset);
			return 0;
		}
	}
	else {
		ASSERT_ACQUIRED;
		request = (LRQ) ((UCHAR *) SRQ_NEXT(LOCK_header->lhb_free_requests) -
						 OFFSET(LRQ, lrq_lbl_requests));
		remove_que(&request->lrq_lbl_requests);
	}

	owner = (OWN) SRQ_ABS_PTR(owner_offset);	/* Re-init after a potential remap */
	post_history(his_enq, owner_offset, (SRQ_PTR)0, SRQ_REL_PTR(request), true);

	request->lrq_type = type_lrq;
	request->lrq_flags = 0;
	request->lrq_requested = type;
	request->lrq_state = LCK_none;
	request->lrq_data = 0;
	request->lrq_owner = owner_offset;
	request->lrq_ast_routine = ast_routine;
	request->lrq_ast_argument = ast_argument;
	insert_tail(&owner->own_requests, &request->lrq_own_requests);
	SRQ_INIT(request->lrq_own_blocks);

/* See if the lock already exits */

	USHORT hash_slot;
	lbl* lock = find_lock(parent, series, value, length, &hash_slot);
	if (lock)
	{
		if (series < LCK_MAX_SERIES) {
			++LOCK_header->lhb_operations[series];
		}
		else {
			++LOCK_header->lhb_operations[0];
		}

		insert_tail(&lock->lbl_requests, &request->lrq_lbl_requests);
		request->lrq_data = data;
		const SLONG lock_id = grant_or_que(request, lock, lck_wait);
		if (!lock_id) {
			*status_vector++ = isc_arg_gds;
			*status_vector++ = (lck_wait > 0) ? isc_deadlock :
				((lck_wait < 0) ? isc_lock_timeout : isc_lock_conflict);
			*status_vector++ = isc_arg_end;
		}
		ASSERT_RELEASED;
		return lock_id;
	}

/* Lock doesn't exist.  Allocate lock block and set it up. */

	SRQ_PTR request_offset = SRQ_REL_PTR(request);

	if (!(lock = alloc_lock(length, status_vector))) {
		/* lock table is exhausted */
		/* release request gracefully */
		remove_que(&request->lrq_own_requests);
		request->lrq_type = type_null;
		insert_tail(&LOCK_header->lhb_free_requests,
					&request->lrq_lbl_requests);
		release(owner_offset);
		return 0;
	}
	lock->lbl_state = type;
	lock->lbl_parent = parent;
	fb_assert(series <= MAX_UCHAR);
	lock->lbl_series = (UCHAR)series;

/* Maintain lock series data queue */

	SRQ_INIT(lock->lbl_lhb_data);
	if (lock->lbl_data = data)
		insert_data_que(lock);

	if (series < LCK_MAX_SERIES)
		++LOCK_header->lhb_operations[series];
	else
		++LOCK_header->lhb_operations[0];

	lock->lbl_flags = 0;
	lock->lbl_pending_lrq_count = 0;

	{ // scope
	SSHORT l = LCK_max;
	USHORT* ps = lock->lbl_counts;
	while (l--)
		*ps++ = 0;
	} // scope

	if (lock->lbl_length = length)
	{
		UCHAR* p = lock->lbl_key;
		do {
			*p++ = *value++;
		} while (--length);
	}

	request = (LRQ) SRQ_ABS_PTR(request_offset);

	SRQ_INIT(lock->lbl_requests);
	ASSERT_ACQUIRED;
	insert_tail(&LOCK_header->lhb_hash[hash_slot], &lock->lbl_lhb_hash);
	insert_tail(&lock->lbl_requests, &request->lrq_lbl_requests);
	request->lrq_lock = SRQ_REL_PTR(lock);
	grant(request, lock);
	const SLONG lock_id = SRQ_REL_PTR(request);
	release(request->lrq_owner);

	return lock_id;
}


void LOCK_fini( ISC_STATUS* status_vector, SRQ_PTR * owner_offset)
{
/**************************************
 *
 *	L O C K _ f i n i
 *
 **************************************
 *
 * Functional description
 *	Release the owner block and any outstanding locks.
 *	The exit handler will unmap the shared memory.
 *
 **************************************/
	LOCK_TRACE(("LOCK_fini(%ld)\n", *owner_offset));

	SRQ_PTR offset = *owner_offset;
	own* owner = (OWN) SRQ_ABS_PTR(offset);
	if (!offset || !owner->own_count)
		return;

	if (--owner->own_count > 0 || !LOCK_header)
		return;

#ifdef USE_BLOCKING_THREAD
	shutdown_blocking_thread(status_vector);
#endif

#if !defined SUPERSERVER && defined HAVE_MMAP
	if (LOCK_owner) {
		ISC_unmap_object(status_vector, &LOCK_data, (UCHAR**)&LOCK_owner,
						 sizeof(own));
		LOCK_owner_offset = 0;
	}
#endif

#ifndef SUPERSERVER
	LOCK_owner = 0;
#endif

	if (LOCK_header->lhb_active_owner != offset)
	{
		acquire(offset);
		owner = (OWN) SRQ_ABS_PTR(offset);	/* Re-init after a potential remap */
	}

	if (LOCK_pid == owner->own_process_id)
		purge_owner(offset, owner);

	release_mutex();

#ifdef USE_BLOCKING_SIGNALS
	ISC_signal_cancel(LOCK_block_signal, blocking_action,  (void *)(IPTR)offset);
#endif

	*owner_offset = (SRQ_PTR)0;
}


int LOCK_init(
			  ISC_STATUS* status_vector,
			  bool owner_flag,
			  LOCK_OWNER_T owner_id, UCHAR owner_type, SLONG * owner_handle)
{
/**************************************
 *
 *	L O C K _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize lock manager for the given owner, if not already done.
 *
 *	Initialize an owner block in the lock manager, if not already
 *	initialized.
 *
 *	Return the offset of the owner block through owner_handle.
 *
 *	Return FB_SUCCESS or FB_FAILURE.
 *
 **************************************/
	LOCK_TRACE(("LOCK_init (ownerid=%ld)\n", owner_id));

/* If everything is already initialized, just bump the use count. */
	own* owner = 0;
	if (*owner_handle) {
		owner = (OWN) SRQ_ABS_PTR(*owner_handle);
		owner->own_count++;
		return FB_SUCCESS;
	}

	if (!LOCK_header) {
		/* We haven't yet mapped the shared region.  Do so now. */

		start_manager = false;
		if (init_lock_table(status_vector) != FB_SUCCESS) {
			return FB_FAILURE;
		}
	}
	if (owner_flag &&
		create_owner(status_vector, owner_id, owner_type,
					 owner_handle) != FB_SUCCESS)
	{
		return FB_FAILURE;
	}

#ifndef SUPERSERVER
	if (LOCK_owner_offset = *owner_handle)
		LOCK_owner = (OWN) SRQ_ABS_PTR(*owner_handle);
#endif

#ifdef USE_BLOCKING_SIGNALS
	if (LOCK_owner_offset)
		ISC_signal(LOCK_block_signal, blocking_action,
				   (void *)(IPTR) LOCK_owner_offset);
#endif

/* Initialize process level stuffs for different platforms.
   This should be done after the call to create_owner() that 
   initializes owner_handle. */

#if !defined SUPERSERVER && defined WIN_NT
	owner = (OWN) SRQ_ABS_PTR(*owner_handle);
	wakeup_event[0] = owner->own_wakeup_hndl;
	blocking_event[0] = ISC_make_signal(TRUE, FALSE, LOCK_pid, LOCK_block_signal);
	owner->own_blocking_hndl = blocking_event[0];
	AST_ALLOC();
	if (gds__thread_start(blocking_action_thread, 
						  &LOCK_owner_offset, 
						  THREAD_critical, 
						  0, 
						  &blocking_action_thread_handle))
	{
		*status_vector++ = isc_arg_gds;
		*status_vector++ = isc_lockmanerr;
		*status_vector++ = isc_arg_gds;
		*status_vector++ = isc_sys_request;
		*status_vector++ = isc_arg_string;
		*status_vector++ = (ISC_STATUS) "CreateThread";
		*status_vector++ = isc_arg_win32;
		*status_vector++ = GetLastError();
		*status_vector++ = isc_arg_end;
		CloseHandle(blocking_event[0]);
		CloseHandle(wakeup_event[0]);
		return FB_FAILURE;
	}
#endif

#if !defined SUPERSERVER && defined HAVE_MMAP
/* Map the owner block separately so that threads waiting
   on synchronization variables embedded in the owner block
   don't have to coordinate during lock table unmapping. */
	if (LOCK_owner_offset &&
		!(LOCK_owner = (OWN) ISC_map_object(status_vector, &LOCK_data,
											LOCK_owner_offset,
											sizeof(own)))) 
	{
		return FB_FAILURE;
	}
#endif

#if !defined SUPERSERVER && defined SOLARIS_MT
	AST_ALLOC();
	const ULONG status = gds__thread_start(blocking_action_thread, 
					&LOCK_owner_offset, THREAD_high, 0, 0);
	if (status) {
		*status_vector++ = isc_arg_gds;
		*status_vector++ = isc_lockmanerr;
		*status_vector++ = isc_arg_gds;
		*status_vector++ = isc_sys_request;
		*status_vector++ = isc_arg_string;
		*status_vector++ = (ISC_STATUS) "thr_create";
		*status_vector++ = isc_arg_unix;
		*status_vector++ = status;
		*status_vector++ = isc_arg_end;
		return FB_FAILURE;
	}
#endif

#ifdef MANAGER_PROCESS
	if (start_manager) {
		start_manager = false;
		if (!fork_lock_manager(status_vector))
			return FB_FAILURE;
	}
#endif

	return FB_SUCCESS;
}


#ifdef MANAGER_PROCESS
void LOCK_manager( SRQ_PTR manager_owner_offset)
{
/**************************************
 *
 *	L O C K _ m a n a g e r
 *
 **************************************
 *
 * Functional description
 *	LOCK_manager is used only in the privileged lock manager
 *	process.  The routine simply waits on its semaphore.  It
 *	the semaphore is poked, it wakes up, looks for blocking
 *	signals to be delivered, delivers them, and goes back to
 *	sleep.
 *
 **************************************/
	SRQ lock_srq;
	ISC_STATUS_ARRAY local_status;
	SLONG value;
	event_t* event_ptr;

	LOCK_TRACE(("LOCK_manager\n"));

	acquire(manager_owner_offset);

#ifdef VALIDATE_LOCK_TABLE
	validate_lhb(LOCK_header);
#endif

/* If there already is a lock manager running, quietly return */

	own* owner;
	while (owner = get_manager(false)) {
		if (signal_owner(owner, (SRQ_PTR) NULL))
			purge_owner(manager_owner_offset, owner);
		else {
			DEBUG_MSG(0,
					  ("LOCK_manager, pid %ld quiting, pid %ld is already manager\n",
					   LOCK_pid, owner->own_process_id));
			release(manager_owner_offset);
			return;
		}
	}

/* Declare ourselves to be lock manager process */

	DEBUG_MSG(0, ("LOCK_manager, pid %ld becoming manager\n", LOCK_pid));

	own* manager_owner = (OWN) SRQ_ABS_PTR(manager_owner_offset);
	manager_owner->own_flags |= OWN_manager;
	LOCK_process_owner.own_flags |= OWN_manager;
#ifdef USE_STATIC_SEMAPHORES
	USHORT semaphore = alloc_semaphore(manager_owner, NULL);
#else
	manager_owner->own_semaphore = 1;
#endif
	ASSERT_ACQUIRED;
	LOCK_header->lhb_manager = manager_owner_offset;
	LOCK_header->lhb_flags &= ~LHB_shut_manager;
	release(manager_owner_offset);

/* Loop, waiting for something to happen */

	chmod(LOCK_HEADER, 0444);
	
#ifdef VALIDATE_LOCK_TABLE
	ULONG manager_counter = 0;
#endif
#ifdef DEBUG
	ULONG signal_counter = 0;
#endif

	for (;;) {
		acquire(manager_owner_offset);
#ifdef VALIDATE_LOCK_TABLE
		if ((manager_counter++ % 100) == 0)
			validate_lhb(LOCK_header);
#endif
		manager_owner = (OWN) SRQ_ABS_PTR(manager_owner_offset);
		if (LOCK_header->lhb_flags & LHB_shut_manager) {
			purge_owner(manager_owner_offset, manager_owner);
			release_mutex();
			break;
		}
		ASSERT_ACQUIRED;
		SRQ_LOOP(LOCK_header->lhb_owners, lock_srq) {
			owner = (OWN) ((UCHAR *) lock_srq - OFFSET(OWN, own_lhb_owners));
			if (owner->own_flags & OWN_signal) {
				if (signal_owner(owner, (SRQ_PTR) NULL)) {
					lock_srq = (SRQ) SRQ_ABS_PTR(lock_srq->srq_backward);
					purge_owner(manager_owner_offset, owner);
				}
				else {
					owner->own_flags &= ~OWN_signal;
#ifdef DEBUG
					if ((++signal_counter % 1000) == 0)
						DEBUG_MSG(1,
								  ("LOCK_manager: delivered %ld signals\n",
								   signal_counter));
#endif
				}
			}
		}
		event_ptr = manager_owner->own_wakeup;
		value = ISC_event_clear(manager_owner->own_wakeup);
		release(manager_owner_offset);

		/* Prepare to wait for a timeout or a wakeup from somebody else.  Start
		   by setting an alarm clock. */

		const int ret = ISC_event_wait(1, &event_ptr, &value,
							 LOCKMANTIMEOUT * 1000000, lock_alarm_handler,
							 event_ptr);

#ifdef DEBUG
		if (ret == FB_FAILURE)
			DEBUG_MSG(1, ("LOCK_manager timer wakeup\n"));
#endif

#ifdef TERMINATE_IDLE_LOCK_MANAGER
		/* The code to terminate an idle lock manager
		 * was disabled around Dec 1993 as part of the Solaris port.
		 * This was due to customer complaints about running InterBase and
		 * Oracle on the same machine.  When Oracle starts up it grabs all
		 * the system resources, leaving none for InterBase.  And if we
		 * release our resources we won't be able to restart.
		 * 1995-March-20 David Schnepper 
		 */

		/* If we're the only process mapping the lock table when the alarm 
		   expires, exit. */

		if (ret == FALSE)
			if (!flock((int) LOCK_data.sh_mem_handle, LOCK_EX | LOCK_NB))
				break;
			else
				flock((int) LOCK_data.sh_mem_handle, LOCK_SH);
#endif
	}

/* Release system resources for semaphores. */

	LOCK_header = NULL;
#ifdef TERMINATE_IDLE_LOCK_MANAGER
	ISC_unmap_file(local_status, &LOCK_data, ISC_SEM_REMOVE);
	chmod(LOCK_HEADER, 0664);
#else
	ISC_unmap_file(local_status, &LOCK_data, 0);
#endif

}
#endif


SLONG LOCK_query_data(SRQ_PTR parent_request, USHORT series, USHORT aggregate)
{
/**************************************
 *
 *	L O C K _ q u e r y _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Query lock series data with respect to a rooted
 *	lock hierarchy calculating aggregates as we go.
 *
 **************************************/
	LBL lock;
	LRQ parent;
	SLONG data, count;
	SRQ lock_srq;

/* Get root of lock hierarchy */

	if (parent_request && series < LCK_MAX_SERIES)
		parent = get_request(parent_request);
	else {
		CHECK(false);
		return 0;
	}

	acquire(parent->lrq_owner);
	parent = (LRQ) SRQ_ABS_PTR(parent_request);	/* remap */

	++LOCK_header->lhb_query_data;
	srq* data_header = &LOCK_header->lhb_data[series];
	data = count = 0;

/* Simply walk the lock series data queue forward for the minimum
   and backward for the maximum -- it's maintained in sorted order. */

	switch (aggregate) {
	case LCK_MIN:
	case LCK_CNT:
	case LCK_AVG:
	case LCK_SUM:
	case LCK_ANY:
		for (lock_srq = (SRQ) SRQ_ABS_PTR(data_header->srq_forward);
			 lock_srq != data_header; lock_srq = (SRQ) SRQ_ABS_PTR(lock_srq->srq_forward))
		{
			lock = (LBL) ((UCHAR *) lock_srq - OFFSET(LBL, lbl_lhb_data));
			CHECK(lock->lbl_series == series);
			if (lock->lbl_parent != parent->lrq_lock)
				continue;

			switch (aggregate) {
			case LCK_MIN:
				data = lock->lbl_data;
				break;

			case LCK_ANY:
			case LCK_CNT:
				++count;
				break;

			case LCK_AVG:
				++count;

			case LCK_SUM:
				data += lock->lbl_data;
				break;
			}

			if (aggregate == LCK_MIN || aggregate == LCK_ANY)
				break;
		}

		if (aggregate == LCK_CNT || aggregate == LCK_ANY)
			data = count;
		else if (aggregate == LCK_AVG)
			data = (count) ? data / count : 0;
		break;

	case LCK_MAX:
		for (lock_srq = (SRQ) SRQ_ABS_PTR(data_header->srq_backward);
			 lock_srq != data_header; lock_srq = (SRQ) SRQ_ABS_PTR(lock_srq->srq_backward))
		{
			lock = (LBL) ((UCHAR *) lock_srq - OFFSET(LBL, lbl_lhb_data));
			CHECK(lock->lbl_series == series);
			if (lock->lbl_parent != parent->lrq_lock)
				continue;

			data = lock->lbl_data;
			break;
		}
		break;

	default:
		CHECK(false);
	}

	release(parent->lrq_owner);
	return data;
}


SLONG LOCK_read_data(SRQ_PTR request_offset)
{
/**************************************
 *
 *	L O C K _ r e a d _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Read data associated with a lock.
 *
 **************************************/
	LOCK_TRACE(("LOCK_read_data(%ld)\n", request_offset));

	lrq* request = get_request(request_offset);
	acquire(request->lrq_owner);
	++LOCK_header->lhb_read_data;
	request = (LRQ) SRQ_ABS_PTR(request_offset);	/* Re-init after a potential remap */
	lbl* lock = (LBL) SRQ_ABS_PTR(request->lrq_lock);
	const SLONG data = lock->lbl_data;
	if (lock->lbl_series < LCK_MAX_SERIES)
		++LOCK_header->lhb_operations[lock->lbl_series];
	else
		++LOCK_header->lhb_operations[0];

	release(request->lrq_owner);

	return data;
}


SLONG LOCK_read_data2(SRQ_PTR parent_request,
					  USHORT series,
					  const UCHAR* value, USHORT length, SRQ_PTR owner_offset)
{
/**************************************
 *
 *	L O C K _ r e a d _ d a t a 2
 *
 **************************************
 *
 * Functional description
 *	Read data associated with transient locks.
 *
 **************************************/
	SRQ_PTR parent;
	LRQ request;
	LBL lock;
	SLONG data;
	USHORT junk;

	LOCK_TRACE(("LOCK_read_data2(%ld)\n", parent_request));

	acquire(owner_offset);
	++LOCK_header->lhb_read_data;
	if (series < LCK_MAX_SERIES)
		++LOCK_header->lhb_operations[series];
	else
		++LOCK_header->lhb_operations[0];

	if (parent_request) {
		request = get_request(parent_request);
		parent = request->lrq_lock;
	}
	else
		parent = 0;

	if (lock = find_lock(parent, series, value, length, &junk))
		data = lock->lbl_data;
	else
		data = 0;

	release(owner_offset);

	return data;
}


void LOCK_re_post( lock_ast_t ast, void* arg, SRQ_PTR owner_offset)
{
/**************************************
 *
 *	L O C K _ r e _ p o s t
 *
 **************************************
 *
 * Functional description
 *	Re-post an AST that was previously blocked.
 *	It is assumed that the routines that look
 *	at the re-post list only test the ast element.
 *
 **************************************/
	LRQ request;

	LOCK_TRACE(("LOCK_re_post(%ld)\n", owner_offset));

	acquire(owner_offset);

/* Allocate or reuse a lock request block */

	ASSERT_ACQUIRED;
	if (SRQ_EMPTY(LOCK_header->lhb_free_requests)) {
		if (!(request = (LRQ) alloc(sizeof(lrq), NULL))) {
			release(owner_offset);
			return;
		}
	}
	else {
		ASSERT_ACQUIRED;
		request = (LRQ) ((UCHAR *) SRQ_NEXT(LOCK_header->lhb_free_requests) -
						 OFFSET(LRQ, lrq_lbl_requests));
		remove_que(&request->lrq_lbl_requests);
	}

	OWN owner = (OWN) SRQ_ABS_PTR(owner_offset);
	request->lrq_type = type_lrq;
	request->lrq_flags = LRQ_repost;
	request->lrq_ast_routine = ast;
	request->lrq_ast_argument = arg;
	request->lrq_requested = LCK_none;
	request->lrq_state = LCK_none;
	request->lrq_owner = owner_offset;
	request->lrq_lock = (SRQ_PTR) 0;
	insert_tail(&owner->own_blocks, &request->lrq_own_blocks);

	DEBUG_DELAY;

#ifdef USE_BLOCKING_THREAD
	signal_owner((OWN) SRQ_ABS_PTR(owner_offset), (SRQ_PTR) NULL);
#else
/* The deadlock detection looks at the OWN_signaled bit to decide
 * whether processes have things to look at - as we're putting
 * a repost item on the blocking queue, we DO have additional work
 * to do, so set the flag to indicate so.
 */
	owner->own_flags &= ~OWN_signal;
	owner->own_ast_flags |= OWN_signaled;
	DEBUG_DELAY;
	ISC_inhibit();
	DEBUG_DELAY;
	blocking_action2(owner_offset, (SRQ_PTR) NULL);
	DEBUG_DELAY;
	ISC_enable();
	DEBUG_DELAY;
#endif
	release(owner_offset);
}


bool LOCK_shut_manager(void)
{
/**************************************
 *
 *	L O C K _ s h u t _ m a n a g e r
 *
 **************************************
 *
 * Functional description
 *	Set a shutdown flag and post the lock
 *	manager process to exit.
 *
 **************************************/
#ifdef MANAGER_PROCESS
	OWN manager;

	acquire(DUMMY_OWNER_SHUTDOWN);
	if (manager = get_manager(false)) {
		LOCK_header->lhb_flags |= LHB_shut_manager;
		post_wakeup(manager);
		release_mutex();
		sleep(5);
		acquire(DUMMY_OWNER_SHUTDOWN);
		LOCK_header->lhb_flags &= ~LHB_shut_manager;
		manager = get_manager(false);
	}

	release_mutex();
	return (!manager);
#else
	return true;
#endif
}


SLONG LOCK_write_data(SRQ_PTR request_offset, SLONG data)
{
/**************************************
 *
 *	L O C K _ w r i t e _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Write a longword into the lock block.
 *
 **************************************/
	LOCK_TRACE(("LOCK_write_data (%ld)\n", request_offset));

	lrq* request = get_request(request_offset);
	acquire(request->lrq_owner);
	++LOCK_header->lhb_write_data;
	request = (LRQ) SRQ_ABS_PTR(request_offset);	/* Re-init after a potential remap */
	lbl* lock = (LBL) SRQ_ABS_PTR(request->lrq_lock);
	remove_que(&lock->lbl_lhb_data);
	if (lock->lbl_data = data)
		insert_data_que(lock);

	if (lock->lbl_series < LCK_MAX_SERIES)
		++LOCK_header->lhb_operations[lock->lbl_series];
	else
		++LOCK_header->lhb_operations[0];

	release(request->lrq_owner);

	return data;
}


static void acquire( SRQ_PTR owner_offset)
{
/**************************************
 *
 *	a c q u i r e
 *
 **************************************
 *
 * Functional description
 *	Acquire the lock file.  It it's busy, wait for it.
 *
 **************************************/
	SRQ_PTR prior_active;
	SLONG length, spins, status;
	LHB header;

#if (defined SOLARIS_MT && !defined SUPERSERVER)
  acquire_retry:
#endif

/* Check that we aren't trying to acquire when we already own it! */
/* ASSERT_RELEASED; This will not work, when the current active owner
   of the lock table falls in the remapped portion of the map
   file, which we are yet to expand (remap) to */

#ifdef USE_BLOCKING_SIGNALS
	++LOCK_asts;
	DEBUG_DELAY;
#endif

/* Measure the impact of the lock table resource as an overall
   system bottleneck. This will be useful metric for lock
   improvements and as a limiting factor for SMP. A conditional
   mutex would probably be more accurate but isn't worth the
   effort. */

	prior_active = LOCK_header->lhb_active_owner;

#ifndef SUPERSERVER
	if (LOCK_owner) {
		/* Record a "timestamp" of when this owner requested the lock table */
		LOCK_owner->own_acquire_time = LOCK_header->lhb_acquires;
#ifdef DEV_BUILD
		/* Due to the system overhead of acquiring the actual time-of-day,
		   we only get the realtime for DEV_BUILD */
		LOCK_owner->own_acquire_realtime = GET_TIME;
#endif
		LOCK_owner->own_ast_hung_flags |= OWN_hung;
	}
#endif

/* Perform a spin wait on the lock table mutex. This should only
   be used on SMP machines; it doesn't make much sense otherwise. */

	status = FB_FAILURE;
#if (defined UNIX || defined WIN_NT)
	for (spins = 0; spins < LOCK_acquire_spins; ++spins) {
		if ((status = ISC_mutex_lock_cond(MUTEX)) == FB_SUCCESS) {
			break;
		}
	}
#endif

/* If the spin wait didn't succeed then wait forever. */

	if (status != FB_SUCCESS) {
		if (ISC_mutex_lock(MUTEX)) {
			bug(NULL, "semop failed (acquire)");
		}
	}

	++LOCK_header->lhb_acquires;
	if (prior_active) {
		++LOCK_header->lhb_acquire_blocks;
	}

#if (defined UNIX || defined WIN_NT)
	if (spins) {
		++LOCK_header->lhb_acquire_retries;
		if (spins < LOCK_acquire_spins) {
			++LOCK_header->lhb_retry_success;
		}
	}
#endif

	prior_active = LOCK_header->lhb_active_owner;
	LOCK_header->lhb_active_owner = owner_offset;

#ifdef MANAGER_PROCESS
	LOCK_post_manager = false;
#endif

#ifndef SUPERSERVER
	if (LOCK_owner) {
		LOCK_owner->own_ast_hung_flags &= ~OWN_hung;	/* Can't be hung by OS if we got here */
	}
#endif

	if (LOCK_header->lhb_length > LOCK_data.sh_mem_length_mapped
#ifdef LOCK_DEBUG_ACQUIRE
/* If we're debugging remaps occuring during acquire, force 
   a remap every-so-often. */
		|| ((debug_acquire_count++ % DEBUG_ACQUIRE_INTERVAL) == 0)
#endif
		) {

		length = LOCK_header->lhb_length;
/* We do not do Lock table remapping here for SuperServer because 
  we have only one address space and we do not need to adjust our
  mapping because another process has changed size of the lock table.
*/
#if !defined SUPERSERVER && (defined HAVE_MMAP || defined WIN_NT)
		ISC_STATUS_ARRAY status_vector;
		header =
			(LHB) ISC_remap_file(status_vector, &LOCK_data, length, FALSE);
		if (!header)
#endif
		{
			bug(NULL, "remap failed");
			return;
		}
		LOCK_header = header;
#ifdef WIN_NT
		// Other platforms use directly mapped objects
		LOCK_owner = (OWN)SRQ_ABS_PTR(LOCK_owner_offset);
#endif
	}

/* If we were able to acquire the MUTEX, but there is an prior owner marked
 * in the the lock table, it means that someone died while owning
 * the lock mutex.  In that event, lets see if there is any unfinished work
 * left around that we need to finish up.
 */
	if (prior_active) {
		SHB recover;
		SRQ lock_srq;

		post_history(his_active, owner_offset, prior_active, (SRQ_PTR) 0, false);
		recover = (SHB) SRQ_ABS_PTR(LOCK_header->lhb_secondary);
		if (recover->shb_remove_node) {
			/* There was a remove_que operation in progress when the prior_owner died */
			DEBUG_MSG(0, ("Got to the funky shb_remove_node code\n"));
			remove_que((SRQ) SRQ_ABS_PTR(recover->shb_remove_node));
		}
		else if (recover->shb_insert_que && recover->shb_insert_prior) {
			/* There was a insert_que operation in progress when the prior_owner died */
			DEBUG_MSG(0, ("Got to the funky shb_insert_que code\n"));

			lock_srq = (SRQ) SRQ_ABS_PTR(recover->shb_insert_que);
			lock_srq->srq_backward = recover->shb_insert_prior;
			lock_srq = (SRQ) SRQ_ABS_PTR(recover->shb_insert_prior);
			lock_srq->srq_forward = recover->shb_insert_que;
			recover->shb_insert_que = 0;
			recover->shb_insert_prior = 0;
		}
	}
#if !defined SUPERSERVER && defined SOLARIS_MT
	if (LOCK_solaris_stall) {
		if (owner_offset > 0) {
			event_t* event_ptr;
			SLONG value;

			// Can't be hung by OS if we got here
			OWN owner = (OWN)SRQ_ABS_PTR(owner_offset);

			owner->own_ast_hung_flags &= ~OWN_hung;
			own* first_owner = (OWN) ((UCHAR *) SRQ_NEXT(LOCK_header->lhb_owners) -
								 OFFSET(OWN, own_lhb_owners));
			if (first_owner->own_ast_hung_flags & OWN_hung &&
				((LOCK_header->lhb_acquires - first_owner->own_acquire_time)
				 > STARVATION_THRESHHOLD))
			{
				first_owner->own_flags |= OWN_starved;
				if (owner->own_flags & OWN_blocking) {
					probe_owners(owner_offset);
					owner->own_flags &= ~OWN_blocking;
					release_mutex();
				}
				else {
					owner->own_flags |= OWN_blocking;
					owner->own_flags &= ~OWN_wakeup;
					owner->own_semaphore = 1;
					event_ptr = owner->own_stall;
					value = ISC_event_clear(event_ptr);
					release_mutex();
					const SLONG ret = ISC_event_wait(1, &event_ptr, &value,
								LOCK_solaris_stall * 1000000,
								lock_alarm_handler, event_ptr);
#ifdef DEV_BUILD
					if (ret != FB_SUCCESS)
						gds__log
							("LOCK: owner %d timed out while stalling for benefit of owner %d",
							 owner_offset, SRQ_REL_PTR(first_owner));
#endif
				}

				goto acquire_retry;
			}

			owner->own_flags &= ~OWN_blocking;
		}
	}
#endif

}


static UCHAR* alloc( SSHORT size, ISC_STATUS * status_vector)
{
/**************************************
 *
 *	a l l o c
 *
 **************************************
 *
 * Functional description
 *	Allocate a block of given size.
 *
 **************************************/
	size = FB_ALIGN(size, ALIGNMENT);
	ASSERT_ACQUIRED;
	const ULONG block = LOCK_header->lhb_used;
	LOCK_header->lhb_used += size;

/* Make sure we haven't overflowed the lock table.  If so, bump the size of
   the table */

	if (LOCK_header->lhb_used > LOCK_header->lhb_length) {
		LOCK_header->lhb_used -= size;
/* We do not do Lock table remapping for SuperServer on non-Windows platforms
  mainly because it is not tested and is not really needed as long SS builds
  do not use lock manager for page locks. On all other platforms we grow
  lock table automatically.
  Do not remap file for Win32 CS because we use mapped objects for 
  synchronization and there is no (documented) way to have multiple 
  coherent memory mappings of different size on Windows to apply
  ISC_map_object approach
*/
#if defined WIN_NT || (!defined SUPERSERVER && defined HAVE_MMAP)
		const ULONG length = LOCK_data.sh_mem_length_mapped + EXTEND_SIZE;
		lhb* header = (LHB) ISC_remap_file(status_vector, &LOCK_data, length, TRUE);
		if (header) {
			LOCK_header = header;
#ifdef WIN_NT
			// Other platforms use directly mapped objects
			LOCK_owner = (OWN)SRQ_ABS_PTR(LOCK_owner_offset);
#endif
			ASSERT_ACQUIRED;
			LOCK_header->lhb_length = LOCK_data.sh_mem_length_mapped;
			LOCK_header->lhb_used += size;
		}
		else
#endif
		{
			/* Do not do abort in case if there is not enough room -- just 
			   return an error */

			if (status_vector) {
				*status_vector++ = isc_arg_gds;
				*status_vector++ = isc_random;
				*status_vector++ = isc_arg_string;
				*status_vector++ = (ISC_STATUS) "lock manager out of room";
				*status_vector++ = isc_arg_end;
			}

			return NULL;
		}
	}

#ifdef DEV_BUILD
/* This version of alloc() doesn't initialize memory.  To shake out
   any bugs, in DEV_BUILD we initialize it to a "funny" pattern */
	memset((void*)SRQ_ABS_PTR(block), 0xFD, size);
#endif

	return (UCHAR*) SRQ_ABS_PTR(block);
}


static LBL alloc_lock( USHORT length, ISC_STATUS * status_vector)
{
/**************************************
 *
 *	a l l o c _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Allocate a lock for a key of a given length.  Look first to see
 *	if a spare of the right size is sitting around.  If not, allocate
 *	one.
 *
 **************************************/
	length = (length + 3) & ~3;

	ASSERT_ACQUIRED;
	srq* lock_srq;
	SRQ_LOOP(LOCK_header->lhb_free_locks, lock_srq) {
		lbl* lock = (LBL) ((UCHAR *) lock_srq - OFFSET(LBL, lbl_lhb_hash));
		if (lock->lbl_size == length) {
			remove_que(&lock->lbl_lhb_hash);
			lock->lbl_type = type_lbl;
			return lock;
		}
	}

	lbl* lock = (LBL) alloc(sizeof(lbl) + length, status_vector);
	if (lock) {
		lock->lbl_size = length;
		lock->lbl_type = type_lbl;
	}

/* NOTE: if the above alloc() fails do not release mutex here but rather
         release it in LOCK_enq() (as of now it is the only function that
         calls alloc_lock()). We need to hold mutex to be able
         to release a lock request block */


	return lock;
}


#ifdef USE_STATIC_SEMAPHORES
static USHORT alloc_semaphore( OWN owner, ISC_STATUS * status_vector)
{
/**************************************
 *
 *	a l l o c _ s e m a p h o r e
 *
 **************************************
 *
 * Functional description
 *	Allocate an unused semaphore.
 *
 **************************************/
	for (USHORT h = 0; h < 2; h++) {
		ASSERT_ACQUIRED;
		semaphore_mask* semaphores = (semaphore_mask*) SRQ_ABS_PTR(LOCK_header->lhb_mask);
		for (USHORT i = 1; i < (USHORT) LOCK_sem_count; i++) {
			if (semaphores->
				smb_mask[i / BITS_PER_LONG] & (1L << (i % BITS_PER_LONG)))
			{
				semaphores->smb_mask[i / BITS_PER_LONG] &=
					~(1L << (i % BITS_PER_LONG));
				owner->own_semaphore = i;
				owner->own_wakeup[0].event_semnum = i;
				return i;
			}
		}

		/* Loop thru owners to see if a semaphore is available */

		ASSERT_ACQUIRED;
		srq* lock_srq;
		SRQ_LOOP(LOCK_header->lhb_owners, lock_srq) {
			own* other = (OWN) ((UCHAR *) lock_srq - OFFSET(OWN, own_lhb_owners));
			if (other->own_semaphore & OWN_semavail)
				release_semaphore(other);
		}
	}

	bug(status_vector, "semaphores are exhausted");

	return 0;
}
#endif


#ifndef SUPERSERVER
static void blocking_action( void* _owner_offset)
{
/**************************************
 *
 *	b l o c k i n g _ a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Fault hander for a blocking signal.  A blocking signal
 *	is an indication (albeit a strong one) that a blocking
 *	AST is pending for the owner.  Check in with the data
 *	structure for details.
 *	The re-post code in this routine assumes that no more
 *	than one thread of execution can be running in this
 *	routine at any time.
 *
 *      NOTE: This is a wrapper for calling blocking_action2() where 
 *		   the real action is.  This routine would be called 
 *		   from a signal_handler or blocking_action_thread()
 *		   or LOCK_re_post() where acquire() would not have 
 *		   been done.
 *
 **************************************/
	SRQ_PTR owner_offset = (SRQ_PTR)(IPTR)_owner_offset;

/* Ignore signals that occur when executing in lock manager
   or when there is no owner block set up */

#ifdef USE_BLOCKING_SIGNALS
	if (++LOCK_asts || !owner_offset) {
		DEBUG_DELAY;
		if (owner_offset) {
			OWN owner;
			/* This operation is done WITHOUT acquiring the lock table.
			 * As this is done at the signal level, we might be resignalling
			 * ourself and already have the lock table acquired at this
			 * point.  If we already have it we'll hang when trying to acquire
			 * it.  The use of LOCK_asts indicates, somewhat imprecisely,
			 * when we are resignalling ourselves when we have the table
			 * acquired already.
			 */
			owner = (OWN) SRQ_ABS_PTR(owner_offset);
			owner->own_ast_flags &= ~OWN_signaled;
		}
		DEBUG_DELAY;
		--LOCK_asts;
		DEBUG_DELAY;
		return;
	}
#else
	if (!owner_offset)
		return;
#endif

	DEBUG_DELAY;

	acquire(owner_offset);
	blocking_action2(owner_offset, (SRQ_PTR) NULL);
	release(owner_offset);

#ifdef USE_BLOCKING_SIGNALS
	DEBUG_DELAY;
	--LOCK_asts;
	DEBUG_DELAY;
#endif
}
#endif

static void blocking_action2(
							 SRQ_PTR blocking_owner_offset,
							 SRQ_PTR blocked_owner_offset)
{
/**************************************
 *
 *	b l o c k i n g _ a c t i o n 2
 *
 **************************************
 *
 * Functional description
 *	Fault hander for a blocking signal.  A blocking signal
 *	is an indication (albeit a strong one) that a blocking
 *	AST is pending for the owner.  Check in with the data
 *	structure for details.
 *	The re-post code in this routine assumes that no more
 *	than one thread of execution can be running in this
 *	routine at any time.
 *
 *      IMPORTANT: Before calling this routine, acquire() should
 *	           have already been done.
 *
 *      Note that both a blocking owner offset and blocked owner
 *      offset are passed to this function.   This is for those
 *      cases where the owners are not the same.   If they are
 *      the same, then the blocked owner offset will be NULL.
 *
 **************************************/
	ASSERT_ACQUIRED;
	own* owner = (OWN) SRQ_ABS_PTR(blocking_owner_offset);

	if (!blocked_owner_offset)
		blocked_owner_offset = blocking_owner_offset;

	while (owner->own_count) {
		srq* lock_srq = SRQ_NEXT(owner->own_blocks);
		if (lock_srq == &owner->own_blocks) {
			/* We've processed the own_blocks queue, reset the "we've been
			 * signaled" flag and start winding out of here
			 */
			owner->own_ast_flags &= ~OWN_signaled;
			/*post_history (his_leave_ast, blocking_owner_offset, 0, 0, true); */
			break;
		}
		lrq* request = (LRQ) ((UCHAR *) lock_srq - OFFSET(LRQ, lrq_own_blocks));
		lock_ast_t routine = request->lrq_ast_routine;
		void* arg = request->lrq_ast_argument;
		remove_que(&request->lrq_own_blocks);
		if (request->lrq_flags & LRQ_blocking) {
			request->lrq_flags &= ~LRQ_blocking;
			request->lrq_flags |= LRQ_blocking_seen;
			++LOCK_header->lhb_blocks;
			post_history(his_post_ast, blocking_owner_offset,
						 request->lrq_lock, SRQ_REL_PTR(request), true);
		}
		else if (request->lrq_flags & LRQ_repost) {
			request->lrq_type = type_null;
			insert_tail(&LOCK_header->lhb_free_requests,
						&request->lrq_lbl_requests);
		}

		if (routine) {
			release(blocked_owner_offset);
			(*routine)(arg);
			acquire(blocked_owner_offset);
			owner = (OWN) SRQ_ABS_PTR(blocking_owner_offset);
		}
	}
}


#if !defined SUPERSERVER && defined WIN_NT
static THREAD_ENTRY_DECLARE blocking_action_thread(THREAD_ENTRY_PARAM arg)
{
/**************************************
 *
 *	b l o c k i n g _ a c t i o n _ t h r e a d	( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Thread to handle blocking signals that a process
 *	will send to itself.
 *
 **************************************/
	SRQ_PTR* owner_offset_ptr = (SRQ_PTR*)arg;
	AST_INIT();					/* Check into scheduler as AST thread */

	while (true) {
		const SLONG ret = WaitForSingleObject(blocking_event[0], INFINITE);
		AST_ENTER();
		own* owner = (OWN) SRQ_ABS_PTR(*owner_offset_ptr);
		if ((ret != WAIT_OBJECT_0 && ret != WAIT_ABANDONED) || !*owner_offset_ptr ||
			owner->own_process_id != LOCK_pid || owner->own_owner_id == 0)
		{
			break;
		}
		blocking_action((void*)(IPTR)*owner_offset_ptr);
		AST_EXIT();
	}

	AST_EXIT();
	AST_FINI();					/* Check out of scheduler as AST thread */

	return 0;
}
#endif


#if (defined SOLARIS_MT && !defined SUPERSERVER)
static THREAD_ENTRY_DECLARE blocking_action_thread(THREAD_ENTRY_PARAM arg)
{
/**************************************
 *
 *	b l o c k i n g _ a c t i o n _ t h r e a d	( S O L A R I S _ M T )
 *
 **************************************
 *
 * Functional description
 *	Thread to handle blocking signals.
 *
 **************************************/
	SRQ_PTR* owner_offset_ptr = (SRQ_PTR*)arg;
	AST_INIT();					/* Check into scheduler as AST thread */

	while (true) {
		AST_ENTER();

		/* See if main thread has requested us to go away */
		if (!*owner_offset_ptr ||
			LOCK_owner->own_process_id != LOCK_pid ||
			!LOCK_owner->own_owner_id)
		{
			break;
		}

		SLONG value = ISC_event_clear(LOCK_owner->own_blocking);
		blocking_action((void*)(IPTR)*owner_offset_ptr);
		AST_EXIT();
		event_t* event_ptr = LOCK_owner->own_blocking;
		ISC_event_wait(1, &event_ptr, &value, 0, NULL, 0);
	}

/* Main thread asked us to go away, do our cleanup, then tell
 * main thread we're done (see shutdown_blocking_action()).
 */

	AST_EXIT();
	AST_FINI();					/* Check out of scheduler as AST thread */

/* Wakeup the main thread waiting for our exit. */
/* Main thread won't wait forever, so check LOCK_owner is still mapped */

	if (LOCK_owner)
		ISC_event_post(LOCK_owner->own_wakeup);
}
#endif


#ifdef DEV_BUILD
static void bug_assert( const TEXT* string, ULONG line)
{
/**************************************
 *
 *      b u g _ a s s e r t
 *
 **************************************
 *
 * Functional description
 *	Disasterous lock manager bug.  Issue message and abort process.
 *
 **************************************/
	TEXT buffer[MAXPATHLEN + 100];
	lhb LOCK_header_copy;

	sprintf((char *) buffer, "%s %ld: lock assertion failure: %.60s\n",
			__FILE__, line, string);

/* Copy the shared memory so we can examine its state when we crashed */
	LOCK_header_copy = *LOCK_header;

	bug(NULL, buffer);			/* Never returns */
}
#endif


static void bug( ISC_STATUS * status_vector, const TEXT* string)
{
/**************************************
 *
 *	b u g
 *
 **************************************
 *
 * Functional description
 *	Disasterous lock manager bug.  Issue message and abort process.
 *
 **************************************/
	TEXT s[2 * MAXPATHLEN];

#ifdef WIN_NT
	sprintf(s, "Fatal lock manager error: %s, errno: %ld", string, ERRNO);
#else
	sprintf(s, "Fatal lock manager error: %s, errno: %d", string, ERRNO);
#endif
	gds__log(s);
	fprintf(stderr, "%s\n", s);

#if !(defined WIN_NT)
	/* The  strerror()  function  returns  the appropriate description string,
	   or an unknown error message if the error code is unknown. */
	fprintf(stderr, "--%s\n", strerror(errno));
#endif

	if (!LOCK_bugcheck++) {

#ifdef DEV_BUILD
#if !defined(WIN_NT)
		/* The lock file has some problem - copy it for later analysis */
		{
			TEXT buffer[2 * MAXPATHLEN];
			TEXT buffer2[2 * MAXPATHLEN];
			TEXT hostname[64];
			gds__prefix_lock(buffer, LOCK_FILE);
			const TEXT* lock_file = buffer;
			sprintf(buffer2, lock_file,
					ISC_get_host(hostname, sizeof(hostname)));
			sprintf(buffer, "cp %s isc_lock1.%d", buffer2, getpid());
			system(buffer);
		}
#endif /* WIN_NT */
#endif /* DEV_BUILD */

		/* If the current mutex acquirer is in the same process, 
		   release the mutex */

		if (LOCK_header && (LOCK_header->lhb_active_owner > 0)) {
			const own* owner = (OWN) SRQ_ABS_PTR(LOCK_header->lhb_active_owner);
			if (owner->own_process_id == LOCK_pid)
				release(LOCK_header->lhb_active_owner);
		}

		if (status_vector) {
			*status_vector++ = isc_arg_gds;
			*status_vector++ = isc_lockmanerr;
			*status_vector++ = isc_arg_gds;
			*status_vector++ = isc_random;
			*status_vector++ = isc_arg_string;
			*status_vector++ = (ISC_STATUS) string;
			*status_vector++ = isc_arg_end;
			return;
		}
	}


#ifdef DEV_BUILD
/* Make a core drop - we want to LOOK at this failure! */
	abort();
#endif

	exit(FINI_ERROR);
}


static bool convert(SRQ_PTR		request_offset,
					UCHAR	type,
					SSHORT	lck_wait,
					lock_ast_t ast_routine,
					void*		ast_argument,
					ISC_STATUS*	status_vector)
{
/**************************************
 *
 *	c o n v e r t
 *
 **************************************
 *
 * Functional description
 *	Perform a lock conversion, if possible.  If the lock cannot be
 *	granted immediately, either return immediately or wait depending
 *	on a wait flag.  If the lock is granted return true, otherwise
 *	return false.  Note: if the conversion would cause a deadlock,
 *	FALSE is returned even if wait was requested.
 *
 **************************************/
	ASSERT_ACQUIRED;
	lrq* request = get_request(request_offset);
	lbl* lock = (LBL) SRQ_ABS_PTR(request->lrq_lock);
	SRQ_PTR owner_offset = request->lrq_owner;
	post_history(his_convert, owner_offset, request->lrq_lock, request_offset,
				 true);
	request->lrq_requested = type;
	request->lrq_flags &= ~LRQ_blocking_seen;

/* Compute the state of the lock without the request. */

	--lock->lbl_counts[request->lrq_state];
	const UCHAR temp = lock_state(lock);

/* If the requested lock level is compatible with the current state
   of the lock, just grant the request.  Easy enough. */

	if (COMPATIBLE(type, temp))
	{
		request->lrq_ast_routine = ast_routine;
		request->lrq_ast_argument = ast_argument;
		grant(request, lock);
		post_pending(lock);
		release(owner_offset);
		return true;
	}

	++lock->lbl_counts[request->lrq_state];

/* If we weren't requested to wait, just forget about the whole thing. 
   Otherwise wait for the request to be granted or rejected */

	if (lck_wait) {
		bool new_ast;
		if (request->lrq_ast_routine != ast_routine ||
			request->lrq_ast_argument != ast_argument)
		{
			new_ast = true;
		}
		else
			new_ast = false;

		if (wait_for_request(request, lck_wait, status_vector)) {
			ASSERT_RELEASED;
			return false;
		}
		request = (LRQ) SRQ_ABS_PTR(request_offset);
		if (!(request->lrq_flags & LRQ_rejected)) {
			if (new_ast) {
				acquire(owner_offset);
				request = (LRQ) SRQ_ABS_PTR(request_offset);
				request->lrq_ast_routine = ast_routine;
				request->lrq_ast_argument = ast_argument;
				release(owner_offset);
			}
			ASSERT_RELEASED;
			return true;
		}
		acquire(owner_offset);
		request = get_request(request_offset);
		lock = (LBL) SRQ_ABS_PTR(request->lrq_lock);
		post_pending(lock);
	}

	request = (LRQ) SRQ_ABS_PTR(request_offset);
	request->lrq_requested = request->lrq_state;
	ASSERT_ACQUIRED;
	++LOCK_header->lhb_denies;
	if (lck_wait < 0)
		++LOCK_header->lhb_timeouts;
	release(owner_offset);
	*status_vector++ = isc_arg_gds;
	*status_vector++ = (lck_wait > 0) ? isc_deadlock :
		((lck_wait < 0) ? isc_lock_timeout : isc_lock_conflict);
	*status_vector++ = isc_arg_end;

	return false;
}


static USHORT create_owner(ISC_STATUS*	status_vector,
						   LOCK_OWNER_T	owner_id,
						   UCHAR	owner_type,
						   SLONG*	owner_handle)
{
/**************************************
 *
 *	c r e a t e _ o w n e r 
 *
 **************************************
 *
 * Functional description
 *	Create an owner block.
 *
 **************************************/
	LOCK_version = LOCK_header->lhb_version;
	if (LOCK_version != LHB_VERSION)
	{
		sprintf(LOCK_bug_buffer,
				"inconsistent lock table version number; found %d, expected %d",
				LOCK_version, LHB_VERSION);
		bug(status_vector, LOCK_bug_buffer);
		return FB_FAILURE;
	}

	acquire(DUMMY_OWNER_CREATE);	/* acquiring owner is being created */

/* Look for a previous instance of owner.  If we find one, get rid of it. */
	srq* lock_srq;
	SRQ_LOOP(LOCK_header->lhb_owners, lock_srq)
	{
		own* owner = (OWN) ((UCHAR *) lock_srq - OFFSET(OWN, own_lhb_owners));
		if (owner->own_owner_id == owner_id &&
			(UCHAR)owner->own_owner_type == owner_type)
		{
			purge_owner(DUMMY_OWNER_CREATE, owner);	/* purging owner_offset has not been set yet */
			break;
		}
	}

/* Allocate an owner block */

	own* owner = 0;
	USHORT new_block;
	if (SRQ_EMPTY(LOCK_header->lhb_free_owners))
	{
		if (!(owner = (OWN) alloc(sizeof(own), status_vector)))
		{
			release_mutex();
			return FB_FAILURE;
		}
		new_block = OWN_BLOCK_new;
	}
	else
	{
		owner = (OWN) ((UCHAR *) SRQ_NEXT(LOCK_header->lhb_free_owners) -
					   OFFSET(OWN, own_lhb_owners));
		remove_que(&owner->own_lhb_owners);
		new_block = OWN_BLOCK_reused;
	}

	init_owner_block(owner, owner_type, owner_id, new_block);

/* cannot ASSERT_ACQUIRED; here - owner not setup */
	insert_tail(&LOCK_header->lhb_owners, &owner->own_lhb_owners);

#ifndef SUPERSERVER
	probe_owners(SRQ_REL_PTR(owner));
#endif

	*owner_handle = SRQ_REL_PTR(owner);
	LOCK_header->lhb_active_owner = *owner_handle;

#ifdef VALIDATE_LOCK_TABLE
	validate_lhb(LOCK_header);
#endif

	release(*owner_handle);

	return FB_SUCCESS;
}


#ifdef DEV_BUILD
static void current_is_active_owner(bool expect_acquired, ULONG line)
{
/**************************************
 *
 *	c u r r e n t _ i s _ a c t i v e _ o w n e r
 *
 **************************************
 *
 * Functional description
 *	Decide if the current process is the active owner
 *	for the lock table.  Used in assertion checks.
 *
 **************************************/

/* Do not ASSERT_ACQUIRED in this routine */

/* If there's no header, we must be setting up in init somewhere */
	if (!LOCK_header)
		return;

/* Use a local copy of lhb_active_owner.  We're viewing the lock table
   without being acquired in the "expect_acquired false" case, so it
   can change out from under us.  We don't care that it changes, but
   if it gets set to DUMMY_OWNER_SHUTDOWN, DUMMY_OWNER_CREATE or 
   DUMMY_OWNER_DELETE it can lead to a core drop when we try to map 
   the owner pointer */

	SRQ_PTR owner_ptr = LOCK_header->lhb_active_owner;

/* If no active owner, then we certainly aren't the active owner */
	if (!owner_ptr) {
		if (!expect_acquired)
			return;
		bug_assert("not acquired", line);
	}

/* When creating or deleting an owner the owner offset is set such that
 * we can't be sure if WE have the lock table acquired, or someone else 
 * has it, and they just happen to be doing the same operation.  So, we 
 * don't try to report any problems when the lock table is in that state
 */

/* If active owner is DUMMY_OWNER_SHUTDOWN, then we're in process of shutting
   down the lock system. */
	if (owner_ptr == DUMMY_OWNER_SHUTDOWN)
		return;

/* If active owner is DUMMY_OWNER_CREATE, then we're creating a new owner */
	if (owner_ptr == DUMMY_OWNER_CREATE)
		return;

/* If active owner is DUMMY_OWNER_DELETE, then we're deleting an owner */
	if (owner_ptr == DUMMY_OWNER_DELETE)
		return;

/* Find the active owner, and see if it is us */
	own* owner = (OWN) SRQ_ABS_PTR(owner_ptr);


/* SUPERSERVER uses the same pid for all threads, so the tests
   below are of limited utility and can cause bogus errors */

#ifndef SUPERSERVER

	own owner_copy;

	if (owner->own_process_id == LOCK_pid) {
		if (expect_acquired)
			return;
		/* Save a copy for the debugger before we abort */
		memcpy(&owner_copy, owner, sizeof(owner_copy));
		bug_assert("not acquired", line);
	}
	else {
		if (!expect_acquired)
			return;
		/* Save a copy for the debugger before we abort */
		memcpy(&owner_copy, owner, sizeof(owner_copy));
		bug_assert("not released", line);
	}
#endif // SUPERSERVER
}

#endif // DEV_BUILD


static void deadlock_clear(void)
{
/**************************************
 *
 *	d e a d l o c k _ c l e a r
 *
 **************************************
 *
 * Functional description
 *	Clear deadlock and scanned bits for pending requests
 *	in preparation for a deadlock scan.
 *
 **************************************/
	ASSERT_ACQUIRED;
	srq* lock_srq;
	SRQ_LOOP(LOCK_header->lhb_owners, lock_srq) {
		own* owner = (OWN) ((UCHAR *) lock_srq - OFFSET(OWN, own_lhb_owners));
		SRQ_PTR pending_offset = owner->own_pending_request;
		if (!pending_offset)
			continue;
		lrq* pending = (LRQ) SRQ_ABS_PTR(pending_offset);
		pending->lrq_flags &= ~(LRQ_deadlock | LRQ_scanned);
	}
}


static LRQ deadlock_scan(OWN owner,
						 LRQ request)
{
/**************************************
 *
 *	d e a d l o c k _ s c a n
 *
 **************************************
 *
 * Functional description
 *	Given an owner block that has been stalled for some time, find
 *	a deadlock cycle if there is one.  If a deadlock is found, return
 *	the address of a pending lock request in the deadlock request.
 *	If no deadlock is found, return null.
 *
 **************************************/
	LOCK_TRACE(
			   ("deadlock_scan: owner %ld request %ld\n", SRQ_REL_PTR(owner),
				SRQ_REL_PTR(request)));

	ASSERT_ACQUIRED;
	++LOCK_header->lhb_scans;
	post_history(his_scan, request->lrq_owner, request->lrq_lock,
				 SRQ_REL_PTR(request), true);
	deadlock_clear();

#ifdef VALIDATE_LOCK_TABLE
	validate_lhb(LOCK_header);
#endif

	bool maybe_deadlock = false;
	lrq* victim = deadlock_walk(request, &maybe_deadlock);

/* Only when it is certain that this request is not part of a deadlock do we
   mark this request as 'scanned' so that we will not check this request again. 
   Note that this request might be part of multiple deadlocks. */

	if (!victim && !maybe_deadlock)
		owner->own_flags |= OWN_scanned;
#ifdef DEBUG
	else if (!victim && maybe_deadlock)
		DEBUG_MSG(0, ("deadlock_scan: not marking due to maybe_deadlock\n"));
#endif

	return victim;
}


static LRQ deadlock_walk(LRQ request,
						 bool * maybe_deadlock)
{
/**************************************
 *
 *	d e a d l o c k _ w a l k
 *
 **************************************
 *
 * Functional description
 *	Given a request that is waiting, determine whether a deadlock has
 *	occured.
 *
 **************************************/

/* If this request was scanned for deadlock earlier than don't
   visit it again. */

	if (request->lrq_flags & LRQ_scanned)
		return NULL;

/* If this request has been seen already during this deadlock-walk, then we
   detected a circle in the wait-for graph.  Return "deadlock". */

	if (request->lrq_flags & LRQ_deadlock)
		return request;

/* Remember that this request is part of the wait-for graph. */

	request->lrq_flags |= LRQ_deadlock;

/* Check if this is a conversion request. */

	const bool conversion = (request->lrq_state > LCK_null);

/* Find the parent lock of the request */

	lbl* lock = (LBL) SRQ_ABS_PTR(request->lrq_lock);

/* Loop thru the requests granted against the lock.  If any granted request is
   blocking the request we're handling, recurse to find what's blocking him. */
	srq* lock_srq;
	SRQ_LOOP(lock->lbl_requests, lock_srq) {
		lrq* block = (LRQ) ((UCHAR *) lock_srq - OFFSET(LRQ, lrq_lbl_requests));

		/* Note that the default for LOCK_ordering is 1, and the default can be
		   changed with the isc_config modifier 'V4_LOCK_GRANT_ORDER'. */

		if (!LOCK_ordering || conversion) {
			/* Don't pursue our own lock-request again. */

			if (request == block)
				continue;

			/* Since lock conversions can't follow the fairness rules (to avoid
			   deadlocks), only granted lock requests need to be examined. */
			/* If lock-ordering is turned off (opening the door for starvation),
			   only granted requests can block our request. */

			if (COMPATIBLE(request->lrq_requested, block->lrq_state))
				continue;
		}
		else {
			/* Don't pursue our own lock-request again.  In addition, don't look
			   at requests that arrived after our request because lock-ordering
			   is in effect. */

			if (request == block)
				break;

			/* Since lock ordering is in effect, granted locks and waiting
			   requests that arrived before our request could block us. */

			if (COMPATIBLE
				(request->lrq_requested,
				 MAX(block->lrq_state, block->lrq_requested)))
			{
				continue;
			}
		}

		/* Don't pursue lock owners that are not blocked themselves 
		   (they can't cause a deadlock). */

		own* owner = (OWN) SRQ_ABS_PTR(block->lrq_owner);

		/* Don't pursue lock owners that still have to finish processing their AST.
		   If the blocking queue is not empty, then the owner still has some
		   AST's to process (or lock reposts).
		   Remember this fact because they still might be part of a deadlock. */

		if (owner->own_ast_flags & OWN_signaled ||
			!SRQ_EMPTY((owner->own_blocks)))
		{
			*maybe_deadlock = true;
			continue;
		}

		/* YYY: Note: can the below code be moved to the
		   start of this block?  Before the OWN_signaled check?
		 */

		/* Get pointer to the waiting request whose owner also owns a lock
		   that blocks the input request. */

		const SRQ_PTR pending_offset = owner->own_pending_request;
		if (!pending_offset)
			continue;

		lrq* target = (LRQ) SRQ_ABS_PTR(pending_offset);

		/* If this waiting request is not pending anymore, then things are changing
		   and this request cannot be part of a deadlock. */

		if (!(target->lrq_flags & LRQ_pending))
			continue;

		/* Check who is blocking the request whose owner is blocking the input
		   request. */

		if (target = deadlock_walk(target, maybe_deadlock))
			return target;
	}

/* This branch of the wait-for graph is exhausted, the current waiting
   request is not part of a deadlock. */

	request->lrq_flags &= ~LRQ_deadlock;
	request->lrq_flags |= LRQ_scanned;
	return NULL;
}




static void dequeue( SRQ_PTR request_offset)
{
/**************************************
 *
 *	d e q u e u e
 *
 **************************************
 *
 * Functional description
 *	Release an outstanding lock.
 *
 **************************************/

/* Acquire the data structure, and compute addresses of both lock
   request and lock */

	lrq* request = get_request(request_offset);
	post_history(his_deq, request->lrq_owner, request->lrq_lock,
				 request_offset, true);
	request->lrq_ast_routine = NULL;
	release_request(request);
}


#ifdef DEBUG

static ULONG delay_count = 0;
static ULONG last_signal_line = 0;
static ULONG last_delay_line = 0;

static void debug_delay( ULONG lineno)
{
/**************************************
 *
 *	d e b u g _ d e l a y
 *
 **************************************
 *
 * Functional description
 *	This is a debugging routine, the purpose of which is to slow
 *	down operations in order to expose windows of critical 
 *	sections.
 *
 **************************************/

	ULONG i;

/* Delay for a while */

	last_delay_line = lineno;
	for (i = 0; i < 10000; i++)
		/* Nothing */ ;


/* Occasionally post a signal to ourselves, provided we aren't
 * in the signal handler already and we've gotten past the initialization code.
 */
#if !defined WIN_NT
	if (LOCK_asts < 1) {
		if (((delay_count++ % 20) == 0) && LOCK_block_signal
			&& LOCK_owner_offset)
		{
			last_signal_line = lineno;
			kill(getpid(), LOCK_block_signal);
		}
	}
#endif

/* Occasionally crash for robustness testing */
/*
if ((delay_count % 500) == 0)
    exit (-1);
*/

	for (i = 0; i < 10000; i++)
		/* Nothing */ ;

}
#endif


static void exit_handler( void *arg)
{
/**************************************
 *
 *	e x i t _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Release the process block, any outstanding locks,
 *	and unmap the lock manager.  This is usually called
 *	by the cleanup handler.
 *
 **************************************/
	ISC_STATUS_ARRAY local_status;

	if (!LOCK_header) {
		return;
	}

#ifndef SUPERSERVER
/* For a superserver (e.g. Netware), since the server is going away, 
   the semaphores cleanups are also happening; things are in a flux; 
   all the threads are going away -- so don't get into any trouble 
   by doing purge_owner() below. */

/* Get rid of all the owners belonging to the current process */

	SRQ_PTR owner_offset = LOCK_owner_offset;
	if (owner_offset) {
#ifdef USE_BLOCKING_THREAD
		shutdown_blocking_thread(local_status);
#else
#ifdef HAVE_MMAP
		if (LOCK_owner) {
			ISC_unmap_object(local_status, &LOCK_data,
				(UCHAR**)&LOCK_owner, sizeof(own));
		}
#endif
#endif
		if (owner_offset != LOCK_header->lhb_active_owner)
			acquire(DUMMY_OWNER_DELETE);

		srq* lock_srq;
		SRQ_LOOP(LOCK_header->lhb_owners, lock_srq) {
			OWN owner = (OWN) ((UCHAR *) lock_srq - OFFSET(OWN, own_lhb_owners));
			if (owner->own_process_id == LOCK_pid) {
				lock_srq = (SRQ) SRQ_ABS_PTR(lock_srq->srq_backward);
				purge_owner(SRQ_REL_PTR(owner), owner);
				break;
			}
		}
		release_mutex();
		LOCK_owner_offset = 0;
	}

#endif

	ISC_unmap_file(local_status, &LOCK_data, 0);
}


static LBL find_lock(
					 SRQ_PTR parent,
					 USHORT series,
					 const UCHAR* value, USHORT length, USHORT* slot)
{
/**************************************
 *
 *	f i n d _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Find a lock block given a resource
 *	name. If it doesn't exist, the hash
 *	slot will be useful for enqueing a
 *	lock.
 *
 **************************************/

/* Hash the value preserving its distribution as much as possible */

	ULONG hash_value = 0;
	{ // scope
	UCHAR* p = NULL; // silence uninitialized warning
	const UCHAR* q = value;
	for (USHORT l = 0; l < length; l++) {
		if (!(l & 3))
			p = (UCHAR *) &hash_value;
		*p++ = *q++;
	}
	} // scope

/* See if the lock already exists */

	const USHORT hash_slot = *slot = (USHORT) (hash_value % LOCK_header->lhb_hash_slots);
	ASSERT_ACQUIRED;
	srq* hash_header = &LOCK_header->lhb_hash[hash_slot];

	for (srq* lock_srq = (SRQ) SRQ_ABS_PTR(hash_header->srq_forward);
		 lock_srq != hash_header; lock_srq = (SRQ) SRQ_ABS_PTR(lock_srq->srq_forward)) 
	{
		lbl* lock = (LBL) ((UCHAR *) lock_srq - OFFSET(LBL, lbl_lhb_hash));
		if (lock->lbl_series != series ||
			lock->lbl_length != length || lock->lbl_parent != parent)
		{
			continue;
		}
		USHORT l = length;
		if (l) {
			const UCHAR* p = value;
			const UCHAR* q = lock->lbl_key;
			do {
				if (*p++ != *q++)
					break;
			} while (--l);
		}
		if (!l)
			return lock;
	}

	return NULL;
}


#ifdef MANAGER_PROCESS
static bool fork_lock_manager( ISC_STATUS * status_vector)
{
/**************************************
 *
 *	f o r k _ l o c k _ m a n a g e r
 *
 **************************************
 *
 * Functional description
 *	Fork lock manager process.
 *
 **************************************/
	TEXT string[MAXPATHLEN];
	struct stat stat_buf;
	int pid;

/* Probe the lock manager executable to see if it plausibly exists. */

#ifdef DEBUG_MANAGER
	gds__prefix(string, DEBUG_MANAGER);
#else
	gds__prefix(string, LOCK_MANAGER);
#endif
	if (statistics(string, &stat_buf) == -1) {
		// bug() fills the vector with this address, it must be static.
		static TEXT errorstring[MAXPATHLEN + 100];
		sprintf (errorstring, "can't start lock manager: %s", string);
		bug(status_vector, errorstring);
		return false;
	}

	if (!(pid = fork())) {
		if (!vfork()) {
			execl(string, string, NULL);
			_exit(FINI_ERROR);
		}
		_exit(FINI_OK);
	}

	if (pid == -1) {
		bug(status_vector, "can't start lock manager");
		return false;
	}

	while (waitpid(pid, NULL, 0) == -1 && SYSCALL_INTERRUPTED(errno))
		/* do nothing */ ;

	return true;
}
#endif


#ifdef MANAGER_PROCESS
static OWN get_manager(bool flag)
{
/**************************************
 *
 *	g e t _ m a n a g e r
 *
 **************************************
 *
 * Functional description
 *	Find lock manager process.  If we can't find one, start
 *	one.
 *
 **************************************/

/* Look for lock manager */

	ASSERT_ACQUIRED;
/* Let's perform a quick search if possible */

	if (LOCK_header->lhb_manager) {
		own* owner = (OWN) SRQ_ABS_PTR(LOCK_header->lhb_manager);
		if (owner->own_flags & OWN_manager)
			return owner;
	}

/* Perform a long, hard search thru the owner blocks. */

	srq* lock_srq;
	SRQ_LOOP(LOCK_header->lhb_owners, lock_srq) {
		own* owner = (OWN) ((UCHAR *) lock_srq - OFFSET(OWN, own_lhb_owners));
		if (owner->own_flags & OWN_manager) {
			LOCK_header->lhb_manager = SRQ_REL_PTR(owner);
			return owner;
		}
	}

/* Oops -- no lock manager.  */

	if (flag)
		fork_lock_manager(NULL);

	return NULL;
}
#endif


static LRQ get_request( SRQ_PTR offset)
{
/**************************************
 *
 *	g e t _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Locate and validate user supplied request offset.
 *
 **************************************/
	TEXT s[32];

	lrq* request = (LRQ) SRQ_ABS_PTR(offset);
	if ((SLONG) offset == -1 || request->lrq_type != type_lrq) {
		sprintf(s, "invalid lock id (%"SLONGFORMAT")", offset);
		bug(NULL, s);
	}

	const lbl* lock = (LBL) SRQ_ABS_PTR(request->lrq_lock);
	if (lock->lbl_type != type_lbl) {
		sprintf(s, "invalid lock (%"SLONGFORMAT")", offset);
		bug(NULL, s);
	}

	return request;
}


static void grant( LRQ request, LBL lock)
{
/**************************************
 *
 *	g r a n t
 *
 **************************************
 *
 * Functional description
 *	Grant a lock request.  If the lock is a conversion, assume the caller
 *	has already decremented the former lock type count in the lock block.
 *
 **************************************/

/* Request must be for THIS lock */
	CHECK(SRQ_REL_PTR(lock) == request->lrq_lock);

	post_history(his_grant, request->lrq_owner, request->lrq_lock,
				 SRQ_REL_PTR(request), true);

	++lock->lbl_counts[request->lrq_requested];
	request->lrq_state = request->lrq_requested;
	if (request->lrq_data) {
		remove_que(&lock->lbl_lhb_data);
		if (lock->lbl_data = request->lrq_data)
			insert_data_que(lock);
		request->lrq_data = 0;
	}

	lock->lbl_state = lock_state(lock);
	if (request->lrq_flags & LRQ_pending)
	{
		request->lrq_flags &= ~LRQ_pending;
		lock->lbl_pending_lrq_count--;
	}
	post_wakeup((OWN) SRQ_ABS_PTR(request->lrq_owner));
}


static SRQ_PTR grant_or_que( LRQ request, LBL lock, SSHORT lck_wait)
{
/**************************************
 *
 *	g r a n t _ o r _ q u e
 *
 **************************************
 *
 * Functional description
 *	There is a request against an existing lock.  If the request
 *	is compatible with the lock, grant it.  Otherwise lock_srq it.
 *	If the lock is lock_srq-ed, set up the machinery to do a deadlock
 *	scan in awhile.
 *
 **************************************/
	const SRQ_PTR request_offset = SRQ_REL_PTR(request);
	request->lrq_lock = SRQ_REL_PTR(lock);

/* Compatible requests are easy to satify.  Just post the request
   to the lock, update the lock state, release the data structure,
   and we're done. */

	if (COMPATIBLE(request->lrq_requested, lock->lbl_state)) {
		if (!LOCK_ordering ||
			request->lrq_requested == LCK_null ||
			(lock->lbl_pending_lrq_count == 0)) 
		{
			grant(request, lock);
			post_pending(lock);
			release(request->lrq_owner);
			return request_offset;
		}
	}

/* The request isn't compatible with the current state of the lock.
 * If we haven't be asked to wait for the lock, return now. 
 */

	if (lck_wait)
	{
		wait_for_request(request, lck_wait, NULL);

		/* For performance reasons, we're going to look at the 
		 * request's status without re-acquiring the lock table.
		 * This is safe as no-one can take away the request, once
		 * granted, and we're doing a read-only access
		 */
		request = (LRQ) SRQ_ABS_PTR(request_offset);

		/* Request HAS been resolved */
		CHECK(!(request->lrq_flags & LRQ_pending));

		if (!(request->lrq_flags & LRQ_rejected))
			return request_offset;
		acquire(request->lrq_owner);
	}

	request = (LRQ) SRQ_ABS_PTR(request_offset);
	post_history(his_deny, request->lrq_owner, request->lrq_lock,
				 SRQ_REL_PTR(request), true);
	ASSERT_ACQUIRED;
	++LOCK_header->lhb_denies;
	if (lck_wait < 0)
		++LOCK_header->lhb_timeouts;
	const SRQ_PTR owner_offset = request->lrq_owner;
	release_request(request);
	release(owner_offset);

	return (SRQ_PTR)0;
}


static ISC_STATUS init_lock_table( ISC_STATUS * status_vector)
{
/**************************************
 *
 *	i n i t _ l o c k _ t a b l e
 *
 **************************************
 *
 * Functional description
 *	Initialize the global lock table for the first time.
 *	Read the config file, map the shared region, etc. 
 *
 **************************************/
	TEXT *lock_file;
	TEXT buffer[MAXPATHLEN];

	LOCK_shm_size = Config::getLockMemSize();
	LOCK_sem_count = Config::getLockSemCount();
	LOCK_block_signal = Config::getLockSignal();
#ifdef WAKEUP_SIGNAL
	LOCK_wakeup_signal = WAKEUP_SIGNAL;
#endif
	LOCK_hash_slots = Config::getLockHashSlots();
	LOCK_scan_interval = Config::getDeadlockTimeout();
	LOCK_acquire_spins = Config::getLockAcquireSpins();

/* LOCK_ordering is TRUE (ON) by default.  It may be changed 
   by the V4_LOCK_GRANT_ORDER option in the configuration file.
   A value of 0 for that option turns lock ordering off */

	LOCK_ordering = Config::getLockGrantOrder();

	if (LOCK_hash_slots < HASH_MIN_SLOTS)
		LOCK_hash_slots = HASH_MIN_SLOTS;
	if (LOCK_hash_slots > HASH_MAX_SLOTS)
		LOCK_hash_slots = HASH_MAX_SLOTS;

#if (defined SOLARIS_MT && !defined SUPERSERVER)
	LOCK_solaris_stall = Config::getSolarisStallValue();

	if (LOCK_solaris_stall < SOLARIS_MIN_STALL)
		LOCK_solaris_stall = SOLARIS_MIN_STALL;
	if (LOCK_solaris_stall > SOLARIS_MAX_STALL)
		LOCK_solaris_stall = SOLARIS_MAX_STALL;
#endif

	LOCK_pid = getpid();

	gds__prefix_lock(buffer, LOCK_FILE);
	lock_file = buffer;
#ifdef UNIX
	LOCK_data.sh_mem_semaphores = LOCK_sem_count;
#endif
	if (!(LOCK_header = (LHB) ISC_map_file(status_vector, lock_file,
										   lock_initialize, 0,
										   LOCK_shm_size, &LOCK_data)))
	{

/* In Superserver lock table init happens only once and 
   if error occurs it could be considered a startup error.
   There is almost nothing the server could do without
   the lock table, let's exit
*/
#if (defined SUPERSERVER && defined UNIX)
		gds__log_status(NULL, status_vector);
		exit(STARTUP_ERROR);
#endif
		return status_vector[1];
	}

/* Now get the globally consistent value of LOCK_ordering from the 
   shared lock table. */

	LOCK_ordering = (LOCK_header->lhb_flags & LHB_lock_ordering) ? TRUE : FALSE;

	gds__register_cleanup(exit_handler, 0);

/* Define a place holder process owner block */

	init_owner_block(&LOCK_process_owner, 255 /*LCK_OWNER_dummy_process */ ,
					 LOCK_header->lhb_process_count++, OWN_BLOCK_dummy);

	return FB_SUCCESS;
}


static void init_owner_block(
							 OWN owner,
							 UCHAR owner_type,
							 LOCK_OWNER_T owner_id, USHORT new_block)
{
/**************************************
 *
 *	i n i t _ o w n e r _ b l o c k
 *
 **************************************
 *
 * Functional description
 *	Initialize the passed owner block nice and new. 
 *
 **************************************/

	owner->own_type = type_own;
	owner->own_owner_type = owner_type;
	owner->own_flags = 0;
	owner->own_ast_flags = 0;
	owner->own_ast_hung_flags = 0;
	owner->own_count = 1;
	owner->own_owner_id = owner_id;
	SRQ_INIT(owner->own_lhb_owners);
	SRQ_INIT(owner->own_requests);
	SRQ_INIT(owner->own_blocks);
	owner->own_pending_request = 0;

	owner->own_process_id = LOCK_pid;
#ifdef UNIX
	owner->own_process_uid = getuid();
#endif
#if defined(WIN_NT)
	owner->own_process_uid = 0;
#endif

	owner->own_acquire_time = 0;
	owner->own_acquire_realtime = 0;
	owner->own_semaphore = 0;

#if defined(WIN_NT) && !defined(SUPERSERVER)
	// Skidder: This Win32 event is deleted when our process is closing
	if (new_block != OWN_BLOCK_dummy)
		owner->own_wakeup_hndl = ISC_make_signal(TRUE, TRUE, LOCK_pid,
												 LOCK_wakeup_signal);
#endif
	if (new_block == OWN_BLOCK_new)
	{
#if defined WIN_NT && defined SUPERSERVER
		// TMN: This Win32 event is never deleted!
		// Skidder: But it may be reused along with the owner block
		owner->own_wakeup_hndl = ISC_make_signal(TRUE, TRUE, LOCK_pid, 0);
#endif
#ifdef USE_WAKEUP_EVENTS
#if (defined WIN_NT && defined SUPERSERVER)
		ISC_event_init(owner->own_wakeup, 0, 0);
#endif
#ifdef SOLARIS
		ISC_event_init(owner->own_wakeup, 0, 1);
#endif
#ifdef SOLARIS_MT
		ISC_event_init(owner->own_blocking, 0, 1);
		ISC_event_init(owner->own_stall, 0, 1);
#endif
#if (defined USE_POSIX_THREADS && defined SUPERSERVER)
		ISC_event_init(owner->own_wakeup, 0, 0);
#endif

#ifdef UNIX
#if !(defined SOLARIS || defined USE_POSIX_THREADS)

		owner->own_wakeup[0].event_semid = LOCK_data.sh_mem_mutex_arg;
		owner->own_wakeup[0].event_semnum = 0;
		owner->own_wakeup[0].event_count = 0;
#endif
#endif
#endif
	}
}


#ifdef USE_WAKEUP_EVENTS
static void lock_alarm_handler(void* event)
{
/**************************************
 *
 *      l o c k _ a l a r m _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *      This handler would just post the passed event.
 *      This is to take care of the situation when an alarm handler
 *      expires before the wait on the samaphore starts.  This way, we can
 *      avoid an indefinite wait when there is nobody to poke the
 *      semaphore and the timer also expired becuase of some asynchronous
 *      event (signal) handling.
 *
 **************************************/

	ISC_event_post(static_cast<event_t*>(event));
}
#endif


static void lock_initialize(void* arg, SH_MEM shmem_data, bool initialize)
{
/**************************************
 *
 *	l o c k _ i n i t i a l i z e
 *
 **************************************
 *
 * Functional description
 *	Initialize the lock header block.  The caller is assumed
 *	to have an exclusive lock on the lock file.
 *
 **************************************/
	SSHORT length;
	USHORT i, j;
	SRQ lock_srq;
	HIS history;
	SRQ_PTR *prior;

#ifdef WIN_NT
	char buffer[MAXPATHLEN];
	gds__prefix_lock(buffer, LOCK_FILE);
	if (ISC_mutex_init(MUTEX, buffer)) {
		bug(NULL, "mutex init failed");
	}
#endif

	LOCK_header = (LHB) shmem_data->sh_mem_address;
	// Reflect real number of semaphores available
#ifdef UNIX
	LOCK_sem_count = shmem_data->sh_mem_semaphores;
#endif

	if (!initialize) {
		return;
	}

	start_manager = true;

	memset(LOCK_header, 0, sizeof(lhb));
	LOCK_header->lhb_type = type_lhb;
	LOCK_header->lhb_version = LHB_VERSION;

/* Mark ourselves as active owner to prevent fb_assert() checks */
	LOCK_header->lhb_active_owner = DUMMY_OWNER_CREATE;	/* In init of lock system */

	SRQ_INIT(LOCK_header->lhb_owners);
	SRQ_INIT(LOCK_header->lhb_free_owners);
	SRQ_INIT(LOCK_header->lhb_free_locks);
	SRQ_INIT(LOCK_header->lhb_free_requests);

#ifndef WIN_NT
	if (ISC_mutex_init(MUTEX, shmem_data->sh_mem_mutex_arg)) {
		bug(NULL, "mutex init failed");
	}
#endif

	LOCK_header->lhb_hash_slots = (USHORT) LOCK_hash_slots;
	LOCK_header->lhb_scan_interval = LOCK_scan_interval;
	LOCK_header->lhb_acquire_spins = LOCK_acquire_spins;

/* Initialize lock series data queues and lock hash chains. */

	for (i = 0, lock_srq = LOCK_header->lhb_data; i < LCK_MAX_SERIES; i++, lock_srq++)
	{
		SRQ_INIT((*lock_srq));
	}
	for (i = 0, lock_srq = LOCK_header->lhb_hash; i < LOCK_header->lhb_hash_slots;
		 i++, lock_srq++)
	{
		SRQ_INIT((*lock_srq));
	}

/* Set lock_ordering flag for the first time */

	if (LOCK_ordering)
		LOCK_header->lhb_flags |= LHB_lock_ordering;

	length =
		sizeof(lhb) +
		(LOCK_header->lhb_hash_slots * sizeof(LOCK_header->lhb_hash[0]));
	LOCK_header->lhb_length = shmem_data->sh_mem_length_mapped;
	LOCK_header->lhb_used = FB_ALIGN(length, ALIGNMENT);

	SHB secondary_header;

	if (!(secondary_header = (SHB) alloc(sizeof(shb), NULL)))
	{
		gds__log("Fatal lock manager error: lock manager out of room");
		exit(STARTUP_ERROR);
	}
	
	LOCK_header->lhb_secondary = SRQ_REL_PTR(secondary_header);
	secondary_header->shb_type = type_shb;
	secondary_header->shb_flags = 0;
	secondary_header->shb_remove_node = 0;
	secondary_header->shb_insert_que = 0;
	secondary_header->shb_insert_prior = 0;
	
	for (i = FB_NELEM(secondary_header->shb_misc); i--;)
	{
		secondary_header->shb_misc[i] = 0;
	}

/* Allocate a sufficiency of history blocks */

	for (j = 0; j < 2; j++)
	{
		prior = (j == 0) ? &LOCK_header->lhb_history : &secondary_header->shb_history;

		for (i = 0; i < HISTORY_BLOCKS; i++)
		{
			if (!(history = (HIS) alloc(sizeof(his), NULL)))
			{
				gds__log("Fatal lock manager error: lock manager out of room");
				exit(STARTUP_ERROR);
			}
			*prior = SRQ_REL_PTR(history);
			history->his_type = type_his;
			history->his_operation = 0;
			prior = &history->his_next;
		}

		history->his_next =
			(j == 0) ? LOCK_header->lhb_history : secondary_header->shb_history;
	}

#ifdef USE_STATIC_SEMAPHORES
/* Initialize the semaphores. Allocate semaphore block. */

	semaphore_mask* semaphores = (semaphore_mask*) alloc(sizeof(semaphore_mask) +
								   (LOCK_sem_count / BITS_PER_LONG) *
								   sizeof(ULONG), NULL);
	if (!semaphores)
	{
		gds__log("Fatal lock manager error: lock manager out of room");
		exit(STARTUP_ERROR);
	}
	LOCK_header->lhb_mask = SRQ_REL_PTR(semaphores);
	semaphores->smb_type = type_smbx;
	for (i = LOCK_sem_count / BITS_PER_LONG + 1; i--;)
	{
		semaphores->smb_mask[i] = 0;
	}
	for (i = 1; i < (USHORT) LOCK_sem_count; i++)
	{
		event_t local_event;

		ISC_event_init(&local_event, shmem_data->sh_mem_mutex_arg, i);
		semaphores->smb_mask[i / BITS_PER_LONG] |= 1L << (i % BITS_PER_LONG);
	}
#endif

/* Done initializing, unmark owner information */
	LOCK_header->lhb_active_owner = 0;
}


static void insert_data_que( LBL lock)
{
/**************************************
 *
 *	i n s e r t _ d a t a _ q u e
 *
 **************************************
 *
 * Functional description
 *	Insert a node in the lock series data queue
 *	in sorted (ascending) order by lock data.
 *
 **************************************/
	LBL lock2;
	SRQ data_header, lock_srq;

	if (lock->lbl_series < LCK_MAX_SERIES && lock->lbl_parent
		&& lock->lbl_data)
	{
		data_header = &LOCK_header->lhb_data[lock->lbl_series];

		for (lock_srq = (SRQ) SRQ_ABS_PTR(data_header->srq_forward);
			 lock_srq != data_header; lock_srq = (SRQ) SRQ_ABS_PTR(lock_srq->srq_forward))
		{
			lock2 = (LBL) ((UCHAR *) lock_srq - OFFSET(LBL, lbl_lhb_data));
			CHECK(lock2->lbl_series == lock->lbl_series);
			if (lock2->lbl_parent != lock->lbl_parent)
				continue;

			if (lock->lbl_data <= lock2->lbl_data)
				break;
		}

		insert_tail(lock_srq, &lock->lbl_lhb_data);
	}
}


static void insert_tail( SRQ lock_srq, SRQ node)
{
/**************************************
 *
 *	i n s e r t _ t a i l
 *
 **************************************
 *
 * Functional description
 *	Insert a node at the tail of a lock_srq.
 *
 *	To handle the event of the process terminating during
 *	the insertion of the node, we set values in the shb to
 *	indicate the node being inserted.
 *	Then, should we be unable to complete
 *	the node insert, the next process into the lock table
 *	will notice the uncompleted work and undo it,
 *	eg: it will put the queue back to the state
 *	prior to the insertion being started.
 *
 **************************************/
	SRQ prior;
	SHB recover;

	ASSERT_ACQUIRED;
	recover = (SHB) SRQ_ABS_PTR(LOCK_header->lhb_secondary);
	DEBUG_DELAY;
	recover->shb_insert_que = SRQ_REL_PTR(lock_srq);
	DEBUG_DELAY;
	recover->shb_insert_prior = lock_srq->srq_backward;
	DEBUG_DELAY;

	node->srq_forward = SRQ_REL_PTR(lock_srq);
	DEBUG_DELAY;
	node->srq_backward = lock_srq->srq_backward;

	DEBUG_DELAY;
	prior = (SRQ) SRQ_ABS_PTR(lock_srq->srq_backward);
	DEBUG_DELAY;
	prior->srq_forward = SRQ_REL_PTR(node);
	DEBUG_DELAY;
	lock_srq->srq_backward = SRQ_REL_PTR(node);
	DEBUG_DELAY;

	recover->shb_insert_que = 0;
	DEBUG_DELAY;
	recover->shb_insert_prior = 0;
	DEBUG_DELAY;
}


static USHORT lock_state( LBL lock)
{
/**************************************
 *
 *	l o c k _ s t a t e
 *
 **************************************
 *
 * Functional description
 *	Compute the current state of a lock.
 *
 **************************************/

	if (lock->lbl_counts[LCK_EX])
		return LCK_EX;
	else if (lock->lbl_counts[LCK_PW])
		return LCK_PW;
	else if (lock->lbl_counts[LCK_SW])
		return LCK_SW;
	else if (lock->lbl_counts[LCK_PR])
		return LCK_PR;
	else if (lock->lbl_counts[LCK_SR])
		return LCK_SR;
	else if (lock->lbl_counts[LCK_null])
		return LCK_null;

	return LCK_none;
}


static void post_blockage(LRQ request,
						  LBL lock,
						  bool force)
{
/**************************************
 *
 *	p o s t _ b l o c k a g e
 *
 **************************************
 *
 * Functional description
 *	The current request is blocked.  Post blocking notices to
 *	any process blocking the request.
 *
 **************************************/
	SRQ lock_srq, next_que;
	SRQ_PTR next_que_offset;
	LRQ block;

	own* owner = (OWN) SRQ_ABS_PTR(request->lrq_owner);

	ASSERT_ACQUIRED;
	CHECK(owner->own_pending_request == SRQ_REL_PTR(request));
	CHECK(request->lrq_flags & LRQ_pending);

	for (lock_srq = SRQ_NEXT(lock->lbl_requests); lock_srq != &lock->lbl_requests;
		 lock_srq = (SRQ) SRQ_ABS_PTR(next_que_offset)) 
	{
		next_que = SRQ_NEXT((*lock_srq));
		next_que_offset = SRQ_REL_PTR(next_que);
		block = (LRQ) ((UCHAR *) lock_srq - OFFSET(LRQ, lrq_lbl_requests));

		/* Figure out if this lock request is blocking our own lock request.
		   Of course, our own request cannot block ourselves.  Compatible
		   requests don't block us, and if there is no AST routine for the
		   request the block doesn't matter as we can't notify anyone.
		   If the owner has marked the request with "LRQ_blocking_seen 
		   then the blocking AST has been delivered and the owner promises
		   to release the lock as soon as possible (so don't bug the owner) */
		if (block == request ||
			COMPATIBLE(request->lrq_requested, block->lrq_state) ||
			!block->lrq_ast_routine ||
			((block->lrq_flags & LRQ_blocking_seen) && !force))
		{
			continue;
		}

		own* blocking_owner = (OWN) SRQ_ABS_PTR(block->lrq_owner);

		/* Add the blocking request to the list of blocks if it's not
		   there already (LRQ_blocking) */

		if (!(block->lrq_flags & LRQ_blocking)) {
			insert_tail(&blocking_owner->own_blocks, &block->lrq_own_blocks);
			block->lrq_flags |= LRQ_blocking;
			block->lrq_flags &= ~LRQ_blocking_seen;
		}

		if (force) {
			blocking_owner->own_ast_flags &= ~OWN_signaled;
		}
		if (blocking_owner != owner &&
			signal_owner(blocking_owner, SRQ_REL_PTR(owner)))
		{
			/* We can't signal the blocking_owner, assume he has died
			   and purge him out */

			lock_srq = (SRQ) SRQ_ABS_PTR(lock_srq->srq_backward);
			purge_owner(SRQ_REL_PTR(owner), blocking_owner);
		}
		if (block->lrq_state == LCK_EX)
			break;
	}
}


static void post_history(USHORT operation,
						 SRQ_PTR process,
						 SRQ_PTR lock,
						 SRQ_PTR request,
						 bool old_version)
{
/**************************************
 *
 *	p o s t _ h i s t o r y
 *
 **************************************
 *
 * Functional description
 *	Post a history item.
 *
 **************************************/
	HIS history;

	if (old_version) {
		history = (HIS) SRQ_ABS_PTR(LOCK_header->lhb_history);
		ASSERT_ACQUIRED;
		LOCK_header->lhb_history = history->his_next;
	}
	else {
		SHB shb;

		ASSERT_ACQUIRED;
		shb = (SHB) SRQ_ABS_PTR(LOCK_header->lhb_secondary);
		history = (HIS) SRQ_ABS_PTR(shb->shb_history);
		shb->shb_history = history->his_next;
	}

	history->his_operation = operation;
	history->his_process = process;
	history->his_lock = lock;
	history->his_request = request;
}


static void post_pending( LBL lock)
{
/**************************************
 *
 *	p o s t _ p e n d i n g
 *
 **************************************
 *
 * Functional description
 *	There has been a change in state of a lock.  Check pending
 *	requests to see if something can be granted.  If so, do it.
 *
 **************************************/
	LRQ request;
	SRQ lock_srq;
	UCHAR temp_state;
	OWN owner;
#ifdef DEV_BUILD
	USHORT pending_counter = 0;
#endif

	if (lock->lbl_pending_lrq_count == 0)
		return;

/* Loop thru granted requests looking for pending conversions.  If one
   is found, check to see if it can be granted.  Even if a request cannot
   be granted for compatibility reason, post_wakeup () that owner so that
   it can post_blockage() to the newly granted owner of the lock. */

	SRQ_LOOP(lock->lbl_requests, lock_srq) {
		request = (LRQ) ((UCHAR *) lock_srq - OFFSET(LRQ, lrq_lbl_requests));
		if (!(request->lrq_flags & LRQ_pending))
			continue;
		if (request->lrq_state)
		{
			--lock->lbl_counts[request->lrq_state];
			temp_state = lock_state(lock);
			if (COMPATIBLE(request->lrq_requested, temp_state))
				grant(request, lock);
			else {
#ifdef DEV_BUILD
				pending_counter++;
#endif
				++lock->lbl_counts[request->lrq_state];
				owner = (OWN) SRQ_ABS_PTR(request->lrq_owner);
				post_wakeup(owner);
				if (LOCK_ordering) {
					CHECK(lock->lbl_pending_lrq_count >= pending_counter);
					return;
				}
			}
		}
		else if (COMPATIBLE(request->lrq_requested, lock->lbl_state))
			grant(request, lock);
		else {
#ifdef DEV_BUILD
			pending_counter++;
#endif
			owner = (OWN) SRQ_ABS_PTR(request->lrq_owner);
			post_wakeup(owner);
			if (LOCK_ordering) {
				CHECK(lock->lbl_pending_lrq_count >= pending_counter);
				return;
			}
		}
	}

	CHECK(lock->lbl_pending_lrq_count == pending_counter);
}


#ifdef USE_WAKEUP_EVENTS
static void post_wakeup( OWN owner)
{
/**************************************
 *
 *	p o s t _ w a k e u p		( U S E _ E V E N T S )
 *
 **************************************
 *
 * Functional description
 *	Wakeup whoever is waiting on a lock.
 *
 **************************************/

/* Note: own_semaphore is set to 1 if we are in wait_for_request() */

	const USHORT semaphore = owner->own_semaphore;
	if (!semaphore || (semaphore & OWN_semavail))
		return;

	++LOCK_header->lhb_wakeups;
	owner->own_flags |= OWN_wakeup;
	ISC_event_post(owner->own_wakeup);
}
#endif


#ifndef USE_WAKEUP_EVENTS
static void post_wakeup( OWN owner)
{
/**************************************
 *
 *	p o s t _ w a k e u p		( n o n - U S E _ E V E N T S )
 *
 **************************************
 *
 * Functional description
 *	Wakeup whoever is waiting on a lock.
 *
 **************************************/

	if (!owner->own_semaphore)
		return;

	if (owner->own_flags & OWN_wakeup)
		return;

	++LOCK_header->lhb_wakeups;
	owner->own_flags |= OWN_wakeup;

#ifdef WIN_NT
	ISC_kill(owner->own_process_id, LOCK_wakeup_signal,
			 owner->own_wakeup_hndl);
#else
	ISC_kill(owner->own_process_id, LOCK_wakeup_signal);
#endif
}
#endif


#ifndef SUPERSERVER
static bool probe_owners( SRQ_PTR probing_owner_offset)
{
/**************************************
 *
 *	p r o b e _ o w n e r s
 *
 **************************************
 *
 * Functional description
 *	Probe an owner to see if it still exists.  If it doesn't, get
 *	rid of it.
 *
 **************************************/
	SRQ lock_srq;
	OWN owner;
	bool purged;

	purged = false;

	ASSERT_ACQUIRED;
	SRQ_LOOP(LOCK_header->lhb_owners, lock_srq) {
		owner = (OWN) ((UCHAR *) lock_srq - OFFSET(OWN, own_lhb_owners));
		if (owner->own_flags & OWN_signal)
			signal_owner(owner, (SRQ_PTR) NULL);
		if (owner->own_process_id != LOCK_pid &&
			!ISC_check_process_existence(owner->own_process_id,
										 owner->own_process_uid, FALSE))
		{
			lock_srq = (SRQ) SRQ_ABS_PTR(lock_srq->srq_backward);
			purge_owner(probing_owner_offset, owner);
			purged = true;
		}
	}

	return purged;
}
#endif /* SUPERSERVER */


static void purge_owner(SRQ_PTR purging_owner_offset, OWN owner)
{
/**************************************
 *
 *	p u r g e _ o w n e r    
 *
 **************************************
 *
 * Functional description
 *	Purge an owner and all of its associated locks.
 *
 **************************************/
	SRQ lock_srq;
	LRQ request;

	LOCK_TRACE(("purge_owner (%ld)\n", purging_owner_offset));

	post_history(his_del_owner, purging_owner_offset, SRQ_REL_PTR(owner), 0,
				 false);

#ifdef USE_STATIC_SEMAPHORES
	release_semaphore(owner);
#endif

/* Release any locks that are active. */

	while ((lock_srq = SRQ_NEXT(owner->own_requests)) != &owner->own_requests) {
		request = (LRQ) ((UCHAR *) lock_srq - OFFSET(LRQ, lrq_own_requests));
		release_request(request);
	}

/* Release any repost requests left dangling on blocking queue. */

	while ((lock_srq = SRQ_NEXT(owner->own_blocks)) != &owner->own_blocks) {
		request = (LRQ) ((UCHAR *) lock_srq - OFFSET(LRQ, lrq_own_blocks));
		remove_que(&request->lrq_own_blocks);
		request->lrq_type = type_null;
		insert_tail(&LOCK_header->lhb_free_requests,
					&request->lrq_lbl_requests);
	}

#ifdef MANAGER_PROCESS
	if (owner->own_flags & OWN_manager) {
		LOCK_header->lhb_manager = 0;
	}
#endif

/* Release owner block */

	remove_que(&owner->own_lhb_owners);
	insert_tail(&LOCK_header->lhb_free_owners, &owner->own_lhb_owners);

	owner->own_owner_type = 0;
	owner->own_owner_id = 0;
	owner->own_process_id = 0;
	owner->own_flags = 0;
}


static void remove_que( SRQ node)
{
/**************************************
 *
 *	r e m o v e _ q u e
 *
 **************************************
 *
 * Functional description
 *	Remove a node from a self-relative lock_srq.
 *
 *	To handle the event of the process terminating during
 *	the removal of the node, we set shb_remove_node to the
 *	node to be removed.  Then, should we be unsuccessful
 *	in the node removal, the next process into the lock table
 *	will notice the uncompleted work and complete it.
 *
 *	Note that the work is completed by again calling this routine,
 *	specifing the node to be removed.  As the prior and following
 *	nodes may have changed prior to the crash, we need to redo the
 *	work only based on what is in <node>.  
 *
 **************************************/
	SRQ lock_srq;
	SHB recover;

	ASSERT_ACQUIRED;
	recover = (SHB) SRQ_ABS_PTR(LOCK_header->lhb_secondary);
	DEBUG_DELAY;
	recover->shb_remove_node = SRQ_REL_PTR(node);
	DEBUG_DELAY;

	lock_srq = (SRQ) SRQ_ABS_PTR(node->srq_forward);

/* The next link might point back to us, or our prior link should
 * the backlink change occur before a crash
 */
	CHECK(lock_srq->srq_backward == SRQ_REL_PTR(node) ||
		  lock_srq->srq_backward == node->srq_backward);
	DEBUG_DELAY;
	lock_srq->srq_backward = node->srq_backward;

	DEBUG_DELAY;
	lock_srq = (SRQ) SRQ_ABS_PTR(node->srq_backward);

/* The prior link might point forward to us, or our following link should
 * the change occur before a crash
 */
	CHECK(lock_srq->srq_forward == SRQ_REL_PTR(node) ||
		  lock_srq->srq_forward == node->srq_forward);
	DEBUG_DELAY;
	lock_srq->srq_forward = node->srq_forward;

	DEBUG_DELAY;
	recover->shb_remove_node = 0;
	DEBUG_DELAY;

/* To prevent trying to remove this entry a second time, which could occur
 * for instance, when we're removing an owner, and crash while removing
 * the owner's blocking requests, reset the lock_srq entry in this node.
 * Note that if we get here, shb_remove_node has been cleared, so we
 * no longer need the queue information.
 */

	SRQ_INIT((*node));
}


static void release( SRQ_PTR owner_offset)
{
/**************************************
 *
 *	r e l e a s e
 *
 **************************************
 *
 * Functional description
 *	Release the mapped lock file.  Advance the event count to wake up
 *	anyone waiting for it.  If there appear to be blocking items
 *	posted.
 *
 **************************************/

#if defined USE_BLOCKING_SIGNALS || \
	defined(MANAGER_PROCESS) || \
	(defined SOLARIS_MT && !defined SUPERSERVER)

	OWN owner;
#endif

#ifdef USE_BLOCKING_SIGNALS
	if (owner_offset) {
		owner = (OWN) SRQ_ABS_PTR(owner_offset);
		if (!SRQ_EMPTY(owner->own_blocks))
			signal_owner(owner, (SRQ_PTR) NULL);
	}
#endif

	if (owner_offset && LOCK_header->lhb_active_owner != owner_offset)
		bug(NULL, "release when not owner");

#ifdef MANAGER_PROCESS
	if (LOCK_post_manager && (owner = get_manager(true)))
		post_wakeup(owner);
#endif

#if (defined SOLARIS_MT && !defined SUPERSERVER)

	if (LOCK_solaris_stall) {
		OWN first_owner;

		if (owner_offset)
			owner = (OWN) SRQ_ABS_PTR(owner_offset);

		/* Rotate first owner to tail of active owners' queue
		   in search of mutex-starved owners. */

		first_owner = (OWN) ((UCHAR *) SRQ_NEXT(LOCK_header->lhb_owners) -
							 OFFSET(OWN, own_lhb_owners));
		remove_que(&first_owner->own_lhb_owners);
		insert_tail(&LOCK_header->lhb_owners, &first_owner->own_lhb_owners);

		/* If others stepped aside to let us run then wake them up now. */

		if (owner_offset && owner->own_flags & OWN_starved) {
			SRQ lock_srq;

			owner->own_flags &= ~(OWN_starved | OWN_blocking);
			SRQ_LOOP(LOCK_header->lhb_owners, lock_srq) {
				owner = (OWN) ((UCHAR *) lock_srq - OFFSET(OWN, own_lhb_owners));
				if (owner->own_flags & OWN_blocking) {
					owner->own_flags &= ~OWN_blocking;
					ISC_event_post(owner->own_stall);
				}
			}
		}

	}
#endif /* SOLARIS_MT */

#ifdef VALIDATE_LOCK_TABLE
/* Validate the lock table occasionally (every 500 releases) */
	if ((LOCK_header->lhb_acquires % (HISTORY_BLOCKS / 2)) == 0)
		validate_lhb(LOCK_header);
#endif

	release_mutex();
}


static void release_mutex(void)
{
/**************************************
 *
 *	r e l e a s e _ m u t e x
 *
 **************************************
 *
 * Functional description
 *	Release the mapped lock file.  Advance the event count to wake up
 *	anyone waiting for it.  If there appear to be blocking items
 *	posted.
 *
 **************************************/

	DEBUG_DELAY;
	ISC_inhibit();
	DEBUG_DELAY;

	if (!LOCK_header->lhb_active_owner)
		bug(NULL, "release when not active");

	LOCK_header->lhb_active_owner = 0;

	if (ISC_mutex_unlock(MUTEX))
		bug(NULL, "semop failed (release)");

#ifdef USE_BLOCKING_SIGNALS
	DEBUG_DELAY;
	--LOCK_asts;
#endif

	DEBUG_DELAY;
	ISC_enable();
	DEBUG_DELAY;
}


static void release_request( LRQ request)
{
/**************************************
 *
 *	r e l e a s e _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Release a request.  This is called both by release lock
 *	and by the cleanup handler.
 *
 **************************************/
	LBL lock;

	ASSERT_ACQUIRED;

/* Start by disconnecting request from both lock and process */

	remove_que(&request->lrq_lbl_requests);
	remove_que(&request->lrq_own_requests);

	request->lrq_type = type_null;
	insert_tail(&LOCK_header->lhb_free_requests, &request->lrq_lbl_requests);
	lock = (LBL) SRQ_ABS_PTR(request->lrq_lock);

/* If the request is marked as blocking, clean it up. */

	if (request->lrq_flags & LRQ_blocking)
	{
		remove_que(&request->lrq_own_blocks);
		request->lrq_flags &= ~LRQ_blocking;
	}

	request->lrq_flags &= ~LRQ_blocking_seen;

/* Update counts if we are cleaning up something we're waiting on!
   This should only happen if we are purging out an owner that died */
	if (request->lrq_flags & LRQ_pending) {
		request->lrq_flags &= ~LRQ_pending;
		lock->lbl_pending_lrq_count--;
	}

/* If there are no outstanding requests, release the lock */

	if (SRQ_EMPTY(lock->lbl_requests))
	{
		CHECK(lock->lbl_pending_lrq_count == 0);
		remove_que(&lock->lbl_lhb_hash);
		remove_que(&lock->lbl_lhb_data);
		lock->lbl_type = type_null;
		insert_tail(&LOCK_header->lhb_free_locks, &lock->lbl_lhb_hash);
		return;
	}

/* Re-compute the state of the lock and post any compatible pending
   requests. */

	if ((request->lrq_state != LCK_none) &&
		!(--lock->lbl_counts[request->lrq_state]))
	{
		lock->lbl_state = lock_state(lock);
		if (request->lrq_state != LCK_null)
		{
			post_pending(lock);
			return;
		}
	}

/* If a lock enqueue failed because of a deadlock or timeout, the pending
   request may be holding up compatible, pending lock requests queued
   behind it. 
   Or, even if this request had been granted, it's now being released,
   so we might be holding up requests or conversions queued up behind. */

	post_pending(lock);
}


#ifdef USE_STATIC_SEMAPHORES
static void release_semaphore( OWN owner)
{
/**************************************
 *
 *	r e l e a s e _ s e m a p h o r e
 *
 **************************************
 *
 * Functional description
 *	Release an un-used and unloved semaphore.
 *
 **************************************/
	USHORT semaphore = owner->own_semaphore;
	if (semaphore) {
		semaphore &= ~OWN_semavail;
		semaphore_mask* semaphores = (semaphore_mask*) SRQ_ABS_PTR(LOCK_header->lhb_mask);
		semaphores->smb_mask[semaphore / BITS_PER_LONG] |=
			1L << (semaphore % BITS_PER_LONG);
		owner->own_semaphore = 0;
		owner->own_wakeup[0].event_semnum = 0;
	}
}
#endif


#ifdef USE_BLOCKING_THREAD
static void shutdown_blocking_thread( ISC_STATUS * status_vector)
{
/**************************************
 *
 *	s h u t d o w n _ b l o c k i n g _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *	Perform a controlled shutdown of blocking thread
 *	to avoid tragic misunderstandings when unmapping
 *	memory.
 *
 **************************************/

/* Zeroing the owner offset implicitly flags the
   blocking thread to exit. */

	LOCK_owner_offset = 0;

#ifdef WIN_NT
	SetEvent(blocking_event[0]);
	AST_ENABLE();
	WaitForSingleObject(blocking_action_thread_handle, 10*1000 /* Give it 10 seconds for clean shutdown */);
	CloseHandle(blocking_action_thread_handle);
	CloseHandle(blocking_event[0]);
#endif

#ifdef SOLARIS_MT
/* Poke the blocking action thread to wakeup and notice
   the exit flag. In turn, wait on an event ourselves
   which the blocking action thread should post on
   the way out. If it doesn't post, wakeup after a
   timeout and cleanup anyway. */

	if (LOCK_owner) {
		event_t* event_ptr;
		SLONG value;

		/* Set a marker for the AST thread to know it's time to cleanup */
		LOCK_owner->own_semaphore = 1;

		event_ptr = LOCK_owner->own_wakeup;
		value = ISC_event_clear(event_ptr);

		/* Wakeup the AST thread - it might be blocking or stalled */
		ISC_event_post(LOCK_owner->own_blocking);
		ISC_event_post(LOCK_owner->own_stall);

		/* Tell the scheduler to allow AST's to run */
		AST_ENABLE();

		/* Wait for the AST thread to finish cleanup or for 10 seconds */
		ISC_event_wait(1, &event_ptr, &value, 10 * 1000000,
					 lock_alarm_handler, event_ptr);

		/* Either AST thread terminated, or our timeout expired */

		/* Finish our cleanup */
		ISC_unmap_object(status_vector,  &LOCK_data, (UCHAR**) &LOCK_owner,
						 sizeof(own));
	}
#endif
}
#endif

static int signal_owner( OWN blocking_owner, SRQ_PTR blocked_owner_offset)
{
/**************************************
 *
 *	s i g n a l _ o w n e r
 *
 **************************************
 *
 * Functional description
 *	Send a signal to a process.
 *
 *      The second parameter is a possible offset to the
 *      blocked owner (or NULL), which is passed on to
 *      blocking_action2().
 *
 **************************************/

/*post_history (his_signal, LOCK_header->lhb_iactive_owner, SRQ_REL_PTR (blocking_owner), 0, true);*/

	ASSERT_ACQUIRED;

/* If a process, other than ourselves, hasn't yet seen a signal
   that was sent, don't bother to send another one. */

	DEBUG_DELAY;

#ifdef USE_BLOCKING_SIGNALS
	if ((blocking_owner->own_ast_flags & OWN_signaled) ||
		(blocking_owner->own_ast_hung_flags & OWN_hung))
#else
	if (blocking_owner->own_ast_flags & OWN_signaled)
#endif
	{
		DEBUG_MSG(1,
				  ("signal_owner (%ld) - skipped OWN_signaled\n",
				   blocking_owner->own_process_id));
		return FB_SUCCESS;
	}

	blocking_owner->own_ast_flags |= OWN_signaled;
	DEBUG_DELAY;
	blocking_owner->own_flags &= ~OWN_signal;
#ifndef USE_BLOCKING_THREAD
	if (blocking_owner->own_process_id == LOCK_pid) {
		DEBUG_DELAY;
		ISC_inhibit();
		DEBUG_DELAY;
		blocking_action2(SRQ_REL_PTR(blocking_owner), blocked_owner_offset);
		DEBUG_DELAY;
		ISC_enable();
		DEBUG_DELAY;
		return FB_SUCCESS;
	}
#endif

#ifdef MANAGER_PROCESS
/* If we can deliver the signal directly, do so.  */

	if ((LOCK_process_owner.own_flags & OWN_manager) ||
		blocking_owner->own_process_uid == LOCK_process_owner.own_process_uid)
	{
		if (LOCK_process_owner.own_flags & OWN_manager)
			++LOCK_header->lhb_indirect_sigs;
		else
			++LOCK_header->lhb_direct_sigs;

		/* Following cases are included */
#endif

		DEBUG_DELAY;

#if !(defined WIN_NT || defined SOLARIS_MT)
		if (ISC_kill(blocking_owner->own_process_id, LOCK_block_signal) != -1)
			return FB_SUCCESS;
#endif

#if (defined WIN_NT && !defined SUPERSERVER)
		if (ISC_kill
			(blocking_owner->own_process_id, LOCK_block_signal,
			 blocking_owner->own_blocking_hndl) != -1)
			return FB_SUCCESS;
#endif

#if (defined SOLARIS_MT && !defined SUPERSERVER)
		if (!(errno = ISC_event_post(blocking_owner->own_blocking)))
			return FB_SUCCESS;
#endif

#ifdef MANAGER_PROCESS
		/* End block started above */
	}
#endif

	DEBUG_MSG(1, ("signal_owner - direct delivery failed\n"));

	blocking_owner->own_ast_flags &= ~OWN_signaled;
	DEBUG_DELAY;
	blocking_owner->own_flags |= OWN_signal;

#ifdef MANAGER_PROCESS
/* If we can't deliver the signal, direct it toward the lock manager
   process for delivery. */

	if (!(LOCK_process_owner.own_flags & OWN_manager)) {
		LOCK_post_manager = true;
		return FB_SUCCESS;
	}
#endif

/* Conclude that the owning process is dead */
	return FB_FAILURE;
}


#ifdef VALIDATE_LOCK_TABLE


const USHORT EXPECT_inuse	= 0;
const USHORT EXPECT_freed	= 1;

const USHORT RECURSE_yes	= 0;
const USHORT RECURSE_not	= 1;

static void validate_history( SRQ_PTR history_header)
{
/**************************************
 *
 *	v a l i d a t e _ h i s t o r y
 *
 **************************************
 *
 * Functional description
 *	Validate a circular list of history blocks.
 *
 **************************************/
	ULONG count = 0;

	LOCK_TRACE(("validate_history: %ld\n", history_header));

	for (const his* history = (HIS) SRQ_ABS_PTR(history_header); true;
		 history = (HIS) SRQ_ABS_PTR(history->his_next)) 
	{
		count++;
		CHECK(history->his_type == type_his);
// The following condition is always true because UCHAR >= 0
//		CHECK(history->his_operation >= 0);
		CHECK(history->his_operation <= his_MAX);
		if (history->his_next == history_header)
			break;
		CHECK(count <= HISTORY_BLOCKS);
	}
}
#endif


#ifdef VALIDATE_LOCK_TABLE
static void validate_lhb( LHB lhb)
{
/**************************************
 *
 *	v a l i d a t e _ l h b
 *
 **************************************
 *
 * Functional description
 *	Validate the LHB structure and everything that hangs off of it.
 *
 **************************************/

	SRQ lock_srq, que_next;
	OWN owner;
	LBL lock;
	LRQ request;

	LOCK_TRACE(("validate_lhb:\n"));

/* Prevent recursive reporting of validation errors */
	if (LOCK_bugcheck)
		return;

	ISC_inhibit();

	CHECK(lhb != NULL);

	CHECK(lhb->lhb_type == type_lhb);
	CHECK(lhb->lhb_version == LHB_VERSION);

	validate_shb(lhb->lhb_secondary);
	if (lhb->lhb_active_owner > 0)
		validate_owner(lhb->lhb_active_owner, EXPECT_inuse);

	SRQ_LOOP(lhb->lhb_owners, lock_srq) {
		/* Validate that the next backpointer points back to us */
		que_next = SRQ_NEXT((*lock_srq));
		CHECK(que_next->srq_backward == SRQ_REL_PTR(lock_srq));

		owner = (OWN) ((UCHAR *) lock_srq - OFFSET(OWN, own_lhb_owners));
		validate_owner(SRQ_REL_PTR(owner), EXPECT_inuse);
	}

	SRQ_LOOP(lhb->lhb_free_owners, lock_srq) {
		/* Validate that the next backpointer points back to us */
		que_next = SRQ_NEXT((*lock_srq));
		CHECK(que_next->srq_backward == SRQ_REL_PTR(lock_srq));

		owner = (OWN) ((UCHAR *) lock_srq - OFFSET(OWN, own_lhb_owners));
		validate_owner(SRQ_REL_PTR(owner), EXPECT_freed);
	}

	SRQ_LOOP(lhb->lhb_free_locks, lock_srq) {
		/* Validate that the next backpointer points back to us */
		que_next = SRQ_NEXT((*lock_srq));
		CHECK(que_next->srq_backward == SRQ_REL_PTR(lock_srq));

		lock = (LBL) ((UCHAR *) lock_srq - OFFSET(LBL, lbl_lhb_hash));
		validate_lock(SRQ_REL_PTR(lock), EXPECT_freed, (SRQ_PTR) 0);
	}

	SRQ_LOOP(lhb->lhb_free_requests, lock_srq) {
		/* Validate that the next backpointer points back to us */
		que_next = SRQ_NEXT((*lock_srq));
		CHECK(que_next->srq_backward == SRQ_REL_PTR(lock_srq));

		request = (LRQ) ((UCHAR *) lock_srq - OFFSET(LRQ, lrq_lbl_requests));
		validate_request(SRQ_REL_PTR(request), EXPECT_freed, RECURSE_not);
	}

	CHECK(lhb->lhb_used <= lhb->lhb_length);

#if defined UNIX && !defined SUPERSERVER && !defined USE_BLOCKING_THREAD
	CHECK(lhb->lhb_mutex[0].mtx_semid == LOCK_data.sh_mem_mutex_arg);
#endif

	validate_history(lhb->lhb_history);
/* validate_semaphore_mask (lhb->lhb_mask); */

	CHECK(lhb->lhb_reserved[0] == 0);
	CHECK(lhb->lhb_reserved[1] == 0);

	DEBUG_MSG(0, ("validate_lhb completed:\n"));

	ISC_enable();

}
#endif


#ifdef VALIDATE_LOCK_TABLE
static void validate_lock( SRQ_PTR lock_ptr, USHORT freed, SRQ_PTR lrq_ptr)
{
/**************************************
 *
 *	v a l i d a t e _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Validate the lock structure and everything that hangs off of it.
 *
 **************************************/
	LBL lock;
	SRQ lock_srq, que_next;
	LRQ request;
	USHORT found;
	UCHAR highest_request;
	USHORT found_pending;
	USHORT i;
	USHORT direct_counts[LCK_max];
	lbl lock_copy;

	LOCK_TRACE(("validate_lock: %ld\n", lock_ptr));

	lock = (LBL) SRQ_ABS_PTR(lock_ptr);
	lock_copy = *lock;

	if (freed == EXPECT_freed)
		CHECK(lock->lbl_type == type_null)
			else
		CHECK(lock->lbl_type == type_lbl);

// The following condition is always true because UCHAR >= 0
//	CHECK(lock->lbl_state >= LCK_none);
	CHECK(lock->lbl_state < LCK_max);

	CHECK(lock->lbl_length <= lock->lbl_size);

/* The lbl_count's should never roll over to be negative */
	for (i = 0; i < FB_NELEM(lock->lbl_counts); i++)
		CHECK(!(lock->lbl_counts[i] & 0x8000))

/* The count of pending locks should never roll over to be negative */
			CHECK(!(lock->lbl_pending_lrq_count & 0x8000));

	memset(direct_counts, 0, sizeof(direct_counts));

	found = 0;
	found_pending = 0;
	highest_request = LCK_none;
	SRQ_LOOP(lock->lbl_requests, lock_srq) {
		/* Validate that the next backpointer points back to us */
		que_next = SRQ_NEXT((*lock_srq));
		CHECK(que_next->srq_backward == SRQ_REL_PTR(lock_srq));

		/* Any requests of a freed lock should also be free */
		CHECK(freed == EXPECT_inuse);

		request = (LRQ) ((UCHAR *) lock_srq - OFFSET(LRQ, lrq_lbl_requests));
		/* Note: Don't try to validate_request here, it leads to recursion */

		if (SRQ_REL_PTR(request) == lrq_ptr)
			found++;

		CHECK(found <= 1);		/* check for a loop in the queue */

		/* Request must be for this lock */
		CHECK(request->lrq_lock == lock_ptr);

		if (request->lrq_requested > highest_request)
			highest_request = request->lrq_requested;

		/* If the request is pending, then it must be incompatible with current
		   state of the lock - OR lock_ordering is enabled and there is at 
		   least one pending request in the queue (before this request 
		   but not including it). */

		if (request->lrq_flags & LRQ_pending) {
			CHECK(!COMPATIBLE(request->lrq_requested, lock->lbl_state) ||
				  (LOCK_ordering && found_pending));

			/* The above condition would probably be more clear if we 
			   wrote it as the following:

			   CHECK (!COMPATIBLE (request->lrq_requested, lock->lbl_state) ||
			   (LOCK_ordering && found_pending &&
			   COMPATIBLE (request->lrq_requested, lock->lbl_state)));

			   but that would be redundant
			 */

			found_pending++;
		}

		/* If the request is NOT pending, then it must be rejected or 
		   compatible with the current state of the lock */
		if (!(request->lrq_flags & LRQ_pending)) {
			CHECK((request->lrq_flags & LRQ_rejected) ||
				  (request->lrq_requested == lock->lbl_state) ||
				  COMPATIBLE(request->lrq_requested, lock->lbl_state));

		}

		direct_counts[request->lrq_state]++;
	}

	if ((freed == EXPECT_inuse) && (lrq_ptr != 0))
		CHECK(found == 1);		/* request is in lock's queue */


	if (freed == EXPECT_inuse) {
		CHECK(found_pending == lock->lbl_pending_lrq_count);

		/* The counter in the lock header should match the actual counts
		   lock->lbl_counts [LCK_null] isn't maintained, so don't check it */
		for (i = LCK_null; i < LCK_max; i++)
			CHECK(direct_counts[i] == lock->lbl_counts[i]);
	}

	if (lock->lbl_parent && (freed == EXPECT_inuse))
		validate_lock(lock->lbl_parent, EXPECT_inuse, (SRQ_PTR) 0);

}
#endif


#ifdef VALIDATE_LOCK_TABLE
static void validate_owner( SRQ_PTR own_ptr, USHORT freed)
{
/**************************************
 *
 *	v a l i d a t e _ o w n e r
 *
 **************************************
 *
 * Functional description
 *	Validate the owner structure and everything that hangs off of it.
 *
 **************************************/
	OWN owner;
	SRQ lock_srq, que_next;
	SRQ que2, que2_next;
	LRQ request;
	LRQ request2;
	USHORT found;
	SRQ_PTR owner_own_pending_request;
	own owner_copy;

	LOCK_TRACE(("validate_owner: %ld\n", own_ptr));

	owner = (OWN) SRQ_ABS_PTR(own_ptr);
	owner_copy = *owner;

/* Note that owner->own_pending_request can be reset without the lock
 * table being acquired - eg: by another process.  That being the case,
 * we need to stash away a copy of it for validation.
 */
	owner_own_pending_request = owner->own_pending_request;

	CHECK(owner->own_type == type_own);
	if (freed == EXPECT_freed)
		CHECK(owner->own_owner_type == 0)
	else {
		CHECK(owner->own_owner_type > 0);
		CHECK(owner->own_owner_type <= 4	/* LCK_OWNER_transaction */
			  || owner->own_owner_type == 255);	/* LCK_OWNER_dummy_process */
	}

	CHECK(owner->own_acquire_time <= LOCK_header->lhb_acquires);
	CHECK(owner->own_acquire_realtime == 0 ||
		  static_cast<time_t>(owner->own_acquire_realtime) <= GET_TIME);

/* Check that no invalid flag bit is set */
	CHECK(!
		  (owner->
		   own_flags & ~(OWN_blocking | OWN_scanned | OWN_manager | OWN_signal
						 | OWN_wakeup | OWN_starved)));

/* Check that no invalid flag bit is set */
	CHECK(!(owner->own_ast_flags & ~(OWN_signaled)));

/* Check that no invalid flag bit is set */
	CHECK(!(owner->own_ast_hung_flags & ~(OWN_hung)));

/* Can't both be signal & signaled */
	if (owner->own_flags & OWN_signal)
		CHECK(!(owner->own_ast_flags & OWN_signaled));

	if (owner->own_ast_flags & OWN_signaled)
		CHECK(!(owner->own_flags & OWN_signal));

	SRQ_LOOP(owner->own_requests, lock_srq) {
		/* Validate that the next backpointer points back to us */
		que_next = SRQ_NEXT((*lock_srq));
		CHECK(que_next->srq_backward == SRQ_REL_PTR(lock_srq));

		CHECK(freed == EXPECT_inuse);	/* should not be in loop for freed owner */

		request = (LRQ) ((UCHAR *) lock_srq - OFFSET(LRQ, lrq_own_requests));
		validate_request(SRQ_REL_PTR(request), EXPECT_inuse, RECURSE_not);
		CHECK(request->lrq_owner == own_ptr);

		/* Make sure that request marked as blocking also exists in the blocking list */

		if (request->lrq_flags & LRQ_blocking) {
			found = 0;
			SRQ_LOOP(owner->own_blocks, que2) {
				/* Validate that the next backpointer points back to us */
				que2_next = SRQ_NEXT((*que2));
				CHECK(que2_next->srq_backward == SRQ_REL_PTR(que2));

				request2 =
					(LRQ) ((UCHAR *) que2 - OFFSET(LRQ, lrq_own_blocks));
				CHECK(request2->lrq_owner == own_ptr);

				if (SRQ_REL_PTR(request2) == SRQ_REL_PTR(request))
					found++;

				CHECK(found <= 1);	/* watch for loops in queue */
			}
			CHECK(found == 1);	/* request marked as blocking must be in blocking queue */
		}
	}

/* Check each item in the blocking queue */

	SRQ_LOOP(owner->own_blocks, lock_srq) {
		/* Validate that the next backpointer points back to us */
		que_next = SRQ_NEXT((*lock_srq));
		CHECK(que_next->srq_backward == SRQ_REL_PTR(lock_srq));

		CHECK(freed == EXPECT_inuse);	/* should not be in loop for freed owner */

		request = (LRQ) ((UCHAR *) lock_srq - OFFSET(LRQ, lrq_own_blocks));
		validate_request(SRQ_REL_PTR(request), EXPECT_inuse, RECURSE_not);

		LOCK_TRACE(("Validate own_block: %ld\n", SRQ_REL_PTR(request)));

		CHECK(request->lrq_owner == own_ptr);

		/* A repost won't be in the request list */

		if (request->lrq_flags & LRQ_repost)
			continue;

		/* Make sure that each block also exists in the request list */

		found = 0;
		SRQ_LOOP(owner->own_requests, que2) {
			/* Validate that the next backpointer points back to us */
			que2_next = SRQ_NEXT((*que2));
			CHECK(que2_next->srq_backward == SRQ_REL_PTR(que2));

			request2 = (LRQ) ((UCHAR *) que2 - OFFSET(LRQ, lrq_own_requests));
			CHECK(request2->lrq_owner == own_ptr);

			if (SRQ_REL_PTR(request2) == SRQ_REL_PTR(request))
				found++;

			CHECK(found <= 1);	/* watch for loops in queue */
		}
		CHECK(found == 1);		/* blocking request must be in request queue */
	}

/* If there is a pending request, make sure it is valid, that it
 * exists in the queue for the lock.
 */
	if (owner_own_pending_request && (freed == EXPECT_inuse)) {
		bool found_pending;
		LRQ request3;
		LBL lock;
		LRQ pending;
		SRQ que_of_lbl_requests;

		/* Make sure pending request is valid, and we own it */
		request3 = (LRQ) SRQ_ABS_PTR(owner_own_pending_request);
		validate_request(SRQ_REL_PTR(request3), EXPECT_inuse, RECURSE_not);
		CHECK(request3->lrq_owner == own_ptr);

		/* Make sure the lock the request is for is valid */
		lock = (LBL) SRQ_ABS_PTR(request3->lrq_lock);
		validate_lock(SRQ_REL_PTR(lock), EXPECT_inuse, (SRQ_PTR) 0);

		/* Make sure the pending request is on the list of requests for the lock */

		found_pending = false;
		SRQ_LOOP(lock->lbl_requests, que_of_lbl_requests) {
			pending =
				(LRQ) ((UCHAR *) que_of_lbl_requests -
					   OFFSET(LRQ, lrq_lbl_requests));
			if (SRQ_REL_PTR(pending) == owner_own_pending_request) {
				found_pending = true;
				break;
			}
		}

		/* pending request must exist in the lock's request queue */
		CHECK(found_pending);

		/* Either the pending request is the same as what we stashed away, or it's
		 * been cleared by another process without the lock table acquired.  */
		CHECK((owner_own_pending_request == owner->own_pending_request) ||
			  !owner->own_pending_request);
	}
#ifdef USE_STATIC_SEMAPHORES
	{
		const USHORT semaphore = owner->own_semaphore;
		if (semaphore && (freed == EXPECT_inuse)) {
			if (!(semaphore & OWN_semavail)) {
				/* Check that the semaphore allocated is flagged as in use */
				semaphore_mask* semaphores =
					(semaphore_mask*) SRQ_ABS_PTR(LOCK_header->lhb_mask);
				CHECK(!
					  (semaphores->
					   smb_mask[semaphore /
								BITS_PER_LONG] & (1L << (semaphore %
														 BITS_PER_LONG))));
			}
		}
	}
#endif
}
#endif


#ifdef VALIDATE_LOCK_TABLE
static void validate_request( SRQ_PTR lrq_ptr, USHORT freed, USHORT recurse)
{
/**************************************
 *
 *	v a l i d a t e _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Validate the request structure and everything that hangs off of it.
 *
 **************************************/
	LRQ request;
	lrq request_copy;

	LOCK_TRACE(("validate_request: %ld\n", lrq_ptr));

	request = (LRQ) SRQ_ABS_PTR(lrq_ptr);
	request_copy = *request;

	if (freed == EXPECT_freed)
		CHECK(request->lrq_type == type_null)
			else
		CHECK(request->lrq_type == type_lrq);

/* Check that no invalid flag bit is set */
	CHECK(!
		  (request->
		   lrq_flags & ~(LRQ_blocking | LRQ_pending | LRQ_converting |
						 LRQ_rejected | LRQ_timed_out | LRQ_deadlock |
						 LRQ_repost | LRQ_scanned | LRQ_blocking_seen)));

/* LRQ_converting & LRQ_timed_out are defined, but never actually used */
	CHECK(!(request->lrq_flags & (LRQ_converting | LRQ_timed_out)));

/* Once a request is rejected, it CAN'T be pending any longer */
	if (request->lrq_flags & LRQ_rejected)
		CHECK(!(request->lrq_flags & LRQ_pending));

/* Can't both be scanned & marked for deadlock walk */
	CHECK((request->lrq_flags & (LRQ_deadlock | LRQ_scanned)) !=
		  (LRQ_deadlock | LRQ_scanned));

	CHECK(request->lrq_requested < LCK_max);
	CHECK(request->lrq_state < LCK_max);

	if (freed == EXPECT_inuse) {
		if (recurse == RECURSE_yes)
			validate_owner(request->lrq_owner, EXPECT_inuse);

		/* Reposted request are pseudo requests, not attached to any real lock */
		if (!(request->lrq_flags & LRQ_repost))
			validate_lock(request->lrq_lock, EXPECT_inuse, SRQ_REL_PTR(request));
	}
}
#endif


#ifdef VALIDATE_LOCK_TABLE
static void validate_shb( SRQ_PTR shb_ptr)
{
/**************************************
 *
 *	v a l i d a t e _ s h b
 *
 **************************************
 *
 * Functional description
 *	Validate the SHB structure and everything that hangs off of
 *	it.
 *	Of course, it would have been a VERY good thing if someone
 *	had moved this into lhb when we made a unique v4 lock
 *	manager....
 *	1995-April-13 David Schnepper 
 *
 **************************************/
	SHB secondary_header;
	USHORT i;

	LOCK_TRACE(("validate_shb: %ld\n", shb_ptr));

	secondary_header = (SHB) SRQ_ABS_PTR(shb_ptr);

	CHECK(secondary_header->shb_type == type_shb);

	validate_history(secondary_header->shb_history);

	for (i = 0; i < FB_NELEM(secondary_header->shb_misc); i++)
		CHECK(secondary_header->shb_misc[i] == 0);
}
#endif


static USHORT wait_for_request(
							   LRQ request,
							   SSHORT lck_wait, ISC_STATUS * status_vector)
{
/**************************************
 *
 *	w a i t _ f o r _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	There is a request that needs satisfaction, but is waiting for
 *	somebody else.  Mark the request as pending and go to sleep until
 *	the lock gets poked.  When we wake up, see if somebody else has
 *	cleared the pending flag.  If not, go back to sleep.
 * Returns
 *	FB_SUCCESS	- we waited until the request was granted or rejected
 * 	FB_FAILURE - Insufficient resouces to wait (eg: no semaphores)
 *
 **************************************/
	LRQ blocking_request;
	int ret;
	SLONG timeout, scan_interval, lock_timeout;

	ASSERT_ACQUIRED;

	++LOCK_header->lhb_waits;
	scan_interval = LOCK_header->lhb_scan_interval;

/* lrq_count will be off if we wait for a pending request */
	CHECK(!(request->lrq_flags & LRQ_pending));

	request->lrq_flags &= ~LRQ_rejected;
	request->lrq_flags |= LRQ_pending;
	SRQ_PTR owner_offset = request->lrq_owner;
	SRQ_PTR lock_offset = request->lrq_lock;
	lbl* lock = (LBL) SRQ_ABS_PTR(lock_offset);
	lock->lbl_pending_lrq_count++;
	if (LOCK_ordering) {
		if (!request->lrq_state) {
			/* If ordering is in effect, and this is a conversion of
			   an existing lock in LCK_none state - put the lock to the
			   end of the list so it's not taking cuts in the lineup */
			remove_que(&request->lrq_lbl_requests);
			insert_tail(&lock->lbl_requests, &request->lrq_lbl_requests);
		}
	}

	own* owner = (OWN) SRQ_ABS_PTR(owner_offset);
	SRQ_PTR request_offset = SRQ_REL_PTR(request);
	owner->own_pending_request = request_offset;
	owner->own_flags &= ~(OWN_scanned | OWN_wakeup);

#ifdef USE_STATIC_SEMAPHORES
/* If semaphore hasn't been allocated for lock, allocate it now. */

	USHORT semaphore = owner->own_semaphore &= ~OWN_semavail;

	if (!semaphore)
		if (!(semaphore = alloc_semaphore(owner, status_vector)))
			return FB_FAILURE;
#else
/* On non-static semaphore systems, set a flag to indicate that we're
   in wait_for_request.  Post_wakeup will check this flag. */

	owner->own_semaphore = 1;
#endif /* STATIC_SEMAPHORES */

#ifdef WIN_NT
#ifdef SUPERSERVER
	ResetEvent(owner->own_wakeup_hndl);
#else
	ResetEvent(wakeup_event[0]);
#endif
#endif

/* Post blockage.  If the blocking owner has disappeared, the blockage
   may clear spontaneously. */

	post_blockage(request, lock, false);
	post_history(his_wait, owner_offset, lock_offset, SRQ_REL_PTR(request), true);
	release(owner_offset);

	SLONG current_time = GET_TIME;

/* If a lock timeout was requested (wait < 0) then figure
   out the time when the lock request will timeout */

	if (lck_wait < 0) {
		lock_timeout = current_time + (-lck_wait);
	}
	else {
		lock_timeout = 0; // silence uninitialized warning
	}
	SLONG deadlock_timeout = current_time + scan_interval;

/* Wait in a loop until the lock becomes available */

#ifdef DEV_BUILD
	ULONG repost_counter = 0;
#endif

	while (true) {
		/* NOTE: Many operations in this loop are done without having
		 *       the lock table acquired - for performance reasons
		 */

		/* Before starting to wait - look to see if someone resolved
		   the request for us - if so we're out easy! */

		ISC_inhibit();
		request = (LRQ) SRQ_ABS_PTR(request_offset);
		if (!(request->lrq_flags & LRQ_pending)) {
			ISC_enable();
			break;
		}
		ISC_enable();

		/* recalculate when we next want to wake up, the lesser of a
		   deadlock scan interval or when the lock request wanted a timeout */

		timeout = deadlock_timeout;
		if (lck_wait < 0 && lock_timeout < deadlock_timeout)
			timeout = lock_timeout;

		/* Prepare to wait for a timeout or a wakeup from somebody else.  */

#ifdef USE_WAKEUP_EVENTS
		SLONG value;
		event_t* event_ptr;

		owner = (OWN) SRQ_ABS_PTR(owner_offset);
#if (defined HAVE_MMAP && !defined SUPERSERVER)
		if (!(LOCK_owner->own_flags & OWN_wakeup))
#else
		if (!(owner->own_flags & OWN_wakeup))
#endif
		{
			/* Re-initialize value each time thru the loop to make sure that the
			   semaphore looks 'un-poked'. */

			/* YYY: NOTE - couldn't there be "missing events" here? */
			/* We don't own the lock_table at this point, but we modify it! */

#if (defined HAVE_MMAP && !defined SUPERSERVER)
			event_ptr = LOCK_owner->own_wakeup;
			value = ISC_event_clear(event_ptr);
			event_ptr = LOCK_owner->own_wakeup;
#else
			event_ptr = owner->own_wakeup;
			value = ISC_event_clear(event_ptr);
			owner = (OWN) SRQ_ABS_PTR(owner_offset);
			event_ptr = owner->own_wakeup;
#endif
#if (defined HAVE_MMAP && !defined SUPERSERVER)
			if (!(LOCK_owner->own_flags & OWN_wakeup))
#else
			if (!(owner->own_flags & OWN_wakeup))
#endif
			{
/* Until here we've been the only thread in the engine (We no longer
   release engine in LCK.C module to avoid problems with more than
   one thread running in out not-yet-MT-safe engine). We already
   tried to execute AST routine provided (sometimes) by blocking
   owner hoping our lock request would be satisfied. Did not help!
   The only thing we could do now is to wait. But let's do it without
   monopolizing the engine
*/
#ifdef SUPERSERVER
				THREAD_EXIT();
#endif
				AST_ENABLE();
				ret = ISC_event_wait(1, &event_ptr, &value,
									 (timeout - current_time) * 1000000,
									 lock_alarm_handler, event_ptr);
				AST_DISABLE();
#ifdef SUPERSERVER
				THREAD_ENTER();
#endif
			}
		}

#else

#ifdef WIN_NT
		AST_ENABLE();
#ifdef SUPERSERVER

/* Until here weve been the only thread in the engine (We no longer
   release engine in LCK.C module to avoid problems with more than
   one thread running in out not-yet-MT-safe engine). We already
   tried to execute AST routine provided (sometimes) by blocking
   owner hoping our lock request would be satisfied. Did not help!
   The only thing we could do now is to wait. But let's do it without
   monopolizing the engine
*/
		THREAD_EXIT();		
		ret = WaitForSingleObject(owner->own_wakeup_hndl,
								(timeout - current_time) * 1000);
		THREAD_ENTER();
#else
		ret = WaitForSingleObject(wakeup_event[0], (timeout - current_time) * 1000);
#endif
		AST_DISABLE();
		if (ret == WAIT_OBJECT_0 || ret == WAIT_ABANDONED)
			ret = FB_SUCCESS;
		else /* if (ret == WAIT_TIMEOUT) */
			ret = FB_FAILURE;
#endif

#endif

		/* We've worken up from the wait - now look around and see
		   why we wokeup */

		/* If somebody else has resolved the lock, we're done */

		ISC_inhibit();
		request = (LRQ) SRQ_ABS_PTR(request_offset);
		if (!(request->lrq_flags & LRQ_pending)) {
			ISC_enable();
			break;
		}
		ISC_enable();

		/* See if we wokeup due to another owner deliberately waking us up
		   ret==FB_SUCCESS --> we were deliberately worken up
		   ret==FB_FAILURE --> we still don't know why we work up */

		/* Only if we came out of the ISC_event_wait() because of a post_wakeup()
		   by another owner is OWN_wakeup set.  This is the only FB_SUCCESS case. */

#if (defined HAVE_MMAP && !defined SUPERSERVER)
		if (LOCK_owner->own_flags & OWN_wakeup)
#else
		owner = (OWN) SRQ_ABS_PTR(owner_offset);
		if (owner->own_flags & OWN_wakeup)
#endif
			ret = FB_SUCCESS;
#ifdef USE_WAKEUP_EVENTS
		else
			ret = FB_FAILURE;
#endif

		current_time = GET_TIME;

		/* See if we workup for a bogus reason - if so 
		   go right back to sleep.  We wokeup bogus unless
		   - we weren't deliberatly woken up
		   - it's not time for a deadlock scan.
		   - it's not time for the lock timeout to expire.
		   Bogus reasons for wakeups include signal reception on some
		   platforms (eg: SUN4)
		   Note: we allow a 1 second leaway on declaring a bogus
		   wakeup due to timing differences (we use seconds here,
		   ISC_event_wait() uses finer granularity) */

		if ((ret != FB_SUCCESS) && (current_time + 1 < timeout)) {
			continue;
		}

		/* We apparently woke up for some real reason.  
		   Make sure everyone is still with us.  Then see if we're still
		   blocked. */

		acquire(owner_offset);
		request = (LRQ) SRQ_ABS_PTR(request_offset);	/* Re-init after potential remap */
		lock = (LBL) SRQ_ABS_PTR(lock_offset);
		owner = (OWN) SRQ_ABS_PTR(owner_offset);
		owner->own_flags &= ~OWN_wakeup;

		/* Now that we own the lock table, see if the request was resolved
		   while we were waiting for the lock table */

		if (!(request->lrq_flags & LRQ_pending)) {
			release(owner_offset);
			break;
		}

		current_time = GET_TIME;	/* refetch due to wait in acquire() */

		/* See if we've waited beyond the lock timeout - if so we
		   mark our own request as rejected */

		if (lck_wait < 0 && lock_timeout <= current_time)
		{
			/* We're going to reject our lock - it's the callers responsibility
			   to do cleanup and make sure post_pending() is called to wakeup
			   other owners we might be blocking */
			request->lrq_flags |= LRQ_rejected;
			request->lrq_flags &= ~LRQ_pending;
			lock->lbl_pending_lrq_count--;
			release(owner_offset);
			break;
		}

		/* We're going to do some real work - reset when we next want to
		   do a deadlock scan */
		deadlock_timeout = current_time + scan_interval;


		/* Handle lock event first */
		if (ret == FB_SUCCESS)
		{
#ifdef WIN_NT
#ifdef SUPERSERVER
			ResetEvent(owner->own_wakeup_hndl);
#else
			ResetEvent(wakeup_event[0]);
#endif
#endif
			/* Someone posted our wakeup event, but DIDN'T grant our request.
			   Re-post what we're blocking on and continue to wait.
			   This could happen if the lock was granted to a different request,
			   we have to tell the new owner of the lock that they are blocking us. */

			post_blockage(request, lock, false);
			release(owner_offset);
			continue;
		}

#ifndef SUPERSERVER
		/* See if all the other owners are still alive.  Dead ones will be purged,
		   purging one might resolve our lock request. */
		/* Do not do rescan of owners if we received notification that
		   blocking ASTs have completed - will do it next time if needed */
		else if (probe_owners(owner_offset) &&
				 !(request->lrq_flags & LRQ_pending))
		{
			release(owner_offset);
			break;
		}
#endif /* SUPERSERVER */

		/* If we've not previously been scanned for a deadlock, go do a
		   deadlock scan */

		else if (!(owner->own_flags & OWN_scanned) &&
				 (blocking_request = deadlock_scan(owner, request)))
		{
			/* Something has been selected for rejection to prevent a
			   deadlock.  Clean things up and go on.  We still have to
			   wait for our request to be resolved. */

			DEBUG_MSG(0, ("wait_for_request: selecting something for deadlock kill\n"));

			++LOCK_header->lhb_deadlocks;
			blocking_request->lrq_flags |= LRQ_rejected;
			blocking_request->lrq_flags &= ~LRQ_pending;
			lbl* blocking_lock = (LBL) SRQ_ABS_PTR(blocking_request->lrq_lock);
			blocking_lock->lbl_pending_lrq_count--;

			own* blocking_owner = (OWN) SRQ_ABS_PTR(blocking_request->lrq_owner);
			blocking_owner->own_pending_request = 0;
			blocking_owner->own_flags &= ~OWN_scanned;
			if (blocking_request != request)
				post_wakeup(blocking_owner);
			/* else
			   We rejected our own request to avoid a deadlock.
			   When we get back to the top of the master loop we
			   fall out and start cleaning up */
		}
		else
		{
			/* Our request is not resolved, all the owners are alive, there's
			   no deadlock -- there's nothing else to do.  Let's
			   make sure our request hasn't been forgotten by reminding
			   all the owners we're waiting - some plaforms under CLASSIC
			   architecture had problems with "missing signals" - which is
			   another reason to repost the blockage.
			   Also, the ownership of the lock could have changed, and we
			   weren't woken up because we weren't next in line for the lock.
			   We need to inform the new owner. */

			DEBUG_MSG(0, ("wait_for_request: forcing a resignal of blockers\n"));
			post_blockage(request, lock, false);
#ifdef DEV_BUILD
			repost_counter++;
			if (repost_counter % 50 == 0) {
				gds__log("wait_for_request: owner %d reposted %ld times for lock %d",
						owner_offset,
						repost_counter,
						lock_offset);
				DEBUG_MSG(0,
						  ("wait_for_request: reposted %ld times for this lock!\n",
						   repost_counter));
			}
#endif /* DEV_BUILD */
		}
		release(owner_offset);
	}

/* NOTE: lock table is not acquired at this point */

#ifdef DEV_BUILD
	request = (LRQ) SRQ_ABS_PTR(request_offset);
	CHECK(!(request->lrq_flags & LRQ_pending));
#endif /* DEV_BUILD */

	owner = (OWN) SRQ_ABS_PTR(owner_offset);
	owner->own_pending_request = 0;

#ifdef USE_STATIC_SEMAPHORES
	owner->own_semaphore |= OWN_semavail;
#else
	owner->own_semaphore = 0;
#endif

	return FB_SUCCESS;
}

