/*
 *      PROGRAM:        JRD Lock Manager
 *      MODULE:         lock.h
 *      DESCRIPTION:    Lock manager structure definitions
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
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "EPSON" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "DELTA" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "IMP" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "NCR3000" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "M88K" port
 *
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "UNIXWARE" port
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "Ultrix" port
 *
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "DGUX" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "MPEXL" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "DecOSF" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "SGI" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 * 2003.03.24 Nickolay Samofatov - clean up defines, improve deadlock detection logic
 *
 */

#ifndef ISC_LOCK_LOCK_H
#define ISC_LOCK_LOCK_H

#include <stdio.h>
#include <sys/types.h>

#if !defined(WIN_NT)
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#endif

#include "../jrd/common.h"
#include "../jrd/file_params.h"
#include "../jrd/isc_signal.h"
#include "../jrd/que.h"

#ifdef WIN_NT
const int DEFAULT_SIZE		= 32768;
#else
const int DEFAULT_SIZE		= 98304;
#endif

#define EXTEND_SIZE     32768

// not used
//#ifdef SCO_UNIX
//#define SEMAPHORES      25
//#endif
//
//#ifdef SINIXZ
//#define SEMAPHORES      25
//#endif
//
//#ifndef SEMAPHORES
//#define SEMAPHORES      32
//#endif

/* Maximum lock series for gathering statistics
   and querying data */

const int LCK_MAX_SERIES	= 7;

/* Lock query data aggregates */

const int LCK_MIN		= 1;
const int LCK_MAX		= 2;
const int LCK_CNT		= 3;
const int LCK_SUM		= 4;
const int LCK_AVG		= 5;
const int LCK_ANY		= 6;

/* Lock states */
// in LCK_convert the type of level is USHORT instead of UCHAR
const UCHAR LCK_none	= 0;
const UCHAR LCK_null	= 1;
const UCHAR LCK_SR		= 2;		// Shared Read
const UCHAR LCK_PR		= 3;		// Protected Read
const UCHAR LCK_SW		= 4;		// Shared Write
const UCHAR LCK_PW		= 5;		// Protected Write
const UCHAR LCK_EX		= 6;		// Exclusive
const UCHAR LCK_max		= 7;

const UCHAR LCK_read	= LCK_PR;
const UCHAR LCK_write	= LCK_EX;

const SSHORT LCK_WAIT		= 1;
const SSHORT LCK_NO_WAIT	= 0;

/* Lock block types */

const UCHAR type_null	= 0;
const UCHAR type_lhb	= 1;
const UCHAR type_prb	= 2;
const UCHAR type_lrq	= 3;
const UCHAR type_lbl	= 4;
const UCHAR type_his	= 5;
const UCHAR type_smbx	= 6;
const UCHAR type_shb	= 7;
const UCHAR type_own	= 8;

const UCHAR type_MAX		= type_own;

#if SIZEOF_VOID_P == 8
const int PLATFORM_LHB_VERSION	= 200;	// 64-bit target
#else
const int PLATFORM_LHB_VERSION	= 0;	// 32-bit target
#endif

const int CLASSIC_LHB_VERSION	= PLATFORM_LHB_VERSION + 16; // Firebird 2.0
const int SS_LHB_VERSION		= CLASSIC_LHB_VERSION + 100;

#ifdef SUPERSERVER
const int LHB_VERSION			= SS_LHB_VERSION;
#else
const int LHB_VERSION			= CLASSIC_LHB_VERSION;
#endif

const SLONG LHB_PATTERN			= 123454321;

//
// Define methods that lock manager uses to handle events
// USE_WAKEUP_EVENTS - use UNIX events to handle owner wakeup.
//    makes no sense on Windows because signals are simulated VIA events
//    otherwise signals are used to wakeup owners
#ifdef UNIX
#define USE_WAKEUP_EVENTS
#endif

// STATIC_SEMAPHORES - preallocate semaphores for UNIX events
// if undefined UNIX events allocate semaphores dynamically
#if defined UNIX && (!defined SUPERSERVER && !defined MULTI_THREAD)
#define USE_STATIC_SEMAPHORES
#endif

// USE_BLOCKING_THREAD - use thread to handle blocking ASTs
//   real UNIX guys use signals instead  - but their CS is not MT
#if (defined WIN_NT || defined SOLARIS_MT) && !defined SUPERSERVER
#define USE_BLOCKING_THREAD
#endif

// USE_BLOCKING_SIGNALS - use UNIX signals to deliver blocking ASTs
#if !defined WIN_NT && !defined SOLARIS_MT && !defined SUPERSERVER
#define USE_BLOCKING_SIGNALS
#endif

/* Lock header block -- one per lock file, lives up front */

typedef struct lhb {
	UCHAR lhb_type;				/* memory tag - always type_lbh */
	UCHAR lhb_version;			/* Version of lock table */
	SRQ_PTR lhb_secondary;			/* Secondary lock header block */
	SRQ_PTR lhb_active_owner;		/* Active owner, if any */
	srq lhb_owners;				/* Que of active owners */
	srq lhb_free_owners;		/* Free owners blocks */
	srq lhb_free_locks;			/* Free lock blocks */
	srq lhb_free_requests;		/* Free lock requests */
	SLONG lhb_length;			/* Size of lock table */
	SLONG lhb_used;				/* Bytes of lock table in use */
	USHORT lhb_hash_slots;		/* Number of hash slots allocated */
	USHORT lhb_flags;			/* Miscellaneous info */
	MTX_T lhb_mutex[1];			/* Mutex controlling access */
	SRQ_PTR lhb_manager;			/* Lock manager owner block */
	SRQ_PTR lhb_history;
	ULONG lhb_process_count;	/* To give a unique id to each process attachment to the lock table */
	SRQ_PTR lhb_mask;				/* Semaphore mask block */
	ULONG lhb_scan_interval;	/* Deadlock scan interval (secs) */
	ULONG lhb_acquire_spins;
	UINT64 lhb_acquires;
	UINT64 lhb_acquire_blocks;
	UINT64 lhb_acquire_retries;
	UINT64 lhb_retry_success;
	UINT64 lhb_enqs;
	UINT64 lhb_converts;
	UINT64 lhb_downgrades;
	UINT64 lhb_deqs;
	UINT64 lhb_read_data;
	UINT64 lhb_write_data;
	UINT64 lhb_query_data;
	UINT64 lhb_operations[LCK_MAX_SERIES];
	UINT64 lhb_waits;
	UINT64 lhb_denies;
	UINT64 lhb_timeouts;
	UINT64 lhb_blocks;
	UINT64 lhb_direct_sigs;
	UINT64 lhb_indirect_sigs;
	UINT64 lhb_wakeups;
	UINT64 lhb_scans;
	UINT64 lhb_deadlocks;
	ULONG lhb_wait_time;
	UINT64 lhb_reserved[2];		/* For future use */
	srq lhb_data[LCK_MAX_SERIES];
	srq lhb_hash[1];			/* Hash table */
} *LHB;

// lhb_flags
const USHORT LHB_lock_ordering		= 1;	/* Lock ordering is enabled */
const USHORT LHB_shut_manager		= 2;	/* Lock manager shutdown flag */

/* Secondary header block -- exists only in V3.3 and later lock
   managers.  It is pointed to by the word in the lhb that used to contain
   a pattern. */

typedef struct shb {
	UCHAR shb_type;				/* memory tag - always type_shb */
	UCHAR shb_flags;
	SRQ_PTR shb_history;
	SRQ_PTR shb_remove_node;		/* Node removing itself */
	SRQ_PTR shb_insert_que;			/* Queue inserting into */
	SRQ_PTR shb_insert_prior;		/* Prior of inserting queue */
	SLONG shb_misc[10];			/* Unused space */
} *SHB;


/* Lock block */

typedef struct lbl
{
	UCHAR lbl_type;				/* mem tag: type_lbl=in use, type_null=free */
	UCHAR lbl_state;			/* High state granted */
	UCHAR lbl_size;				/* Key bytes allocated */
	UCHAR lbl_length;			/* Key bytes used */
	srq lbl_requests;			/* Requests granted */
	srq lbl_lhb_hash;			/* Collision que for hash table */
	srq lbl_lhb_data;			/* Lock data que by series */
	SLONG lbl_data;				/* user data */
	SRQ_PTR lbl_parent;				/* Parent */
	UCHAR lbl_series;			/* Lock series */
	UCHAR lbl_flags;			/* Misc flags */
	USHORT lbl_pending_lrq_count;	/* count of lbl_requests with LRQ_pending */
	USHORT lbl_counts[LCK_max];	/* Counts of granted locks */
	UCHAR lbl_key[1];			/* Key value */
} *LBL;

/* No flags are defined for LBL at this time */

/* Lock requests */

typedef struct lrq {
	UCHAR lrq_type;				/* mem tag: type_lrq=in use, type_null=free */
	UCHAR lrq_requested;		/* Level requested  */
	UCHAR lrq_state;			/* State of lock request */
	USHORT lrq_flags;			/* Misc crud */
	SRQ_PTR lrq_owner;				/* Owner making request */
	SRQ_PTR lrq_lock;				/* Lock requested */
	SLONG lrq_data;				/* Lock data requested */
	srq lrq_own_requests;		/* Locks granted for owner */
	srq lrq_lbl_requests;		/* Que of requests (active, pending) */
	srq lrq_own_blocks;			/* Owner block que */
	lock_ast_t lrq_ast_routine;	/* Block ast routine */
	void* lrq_ast_argument;		/* Ast argument */
} *LRQ;

// lrw_flags
const USHORT LRQ_blocking		= 1;		/* Request is blocking */
const USHORT LRQ_pending		= 2;		/* Request is pending */
const USHORT LRQ_converting		= 4;		/* Request is pending conversion */
const USHORT LRQ_rejected		= 8;		/* Request is rejected */
const USHORT LRQ_timed_out		= 16;		/* Wait timed out */
const USHORT LRQ_deadlock		= 32;		/* Request has been seen by the deadlock-walk */
const USHORT LRQ_repost			= 64;		/* Request block used for repost */
const USHORT LRQ_scanned		= 128;		/* Request already scanned for deadlock */
const USHORT LRQ_blocking_seen	= 256;		/* Blocking notification received by owner */

/* Owner block */

typedef struct own
{
	UCHAR own_type;				/* memory tag - always type_own */
	UCHAR own_owner_type;		/* type of owner */
	SSHORT own_count;			/* init count for the owner */
	LOCK_OWNER_T own_owner_id;	/* Owner ID */
	UATOM own_ast_flags;		/* flags shared by main and ast codes */
	UATOM own_ast_hung_flags;	/* unprotected - OWN_hung flag */
	srq own_lhb_owners;			/* Owner que */
	srq own_requests;			/* Lock requests granted */
	srq own_blocks;				/* Lock requests blocking */
	SRQ_PTR own_pending_request;	/* Request we're waiting on */
	int own_process_id;			/* Owner's process ID */
	int own_process_uid;		/* Owner's process UID */
	UINT64 own_acquire_time;		/* lhb_acquires when owner last tried acquire() */
	ULONG own_acquire_realtime;	/* GET_TIME when owner last tried acquire() */
#ifdef WIN_NT
	void *own_wakeup_hndl;		/* Handle of wakeup event */
#ifndef SUPERSERVER
	void *own_blocking_hndl;	/* Handle of blocking event */
#endif
#endif							/* WIN_NT */
#ifdef SOLARIS_MT
	event_t own_blocking[1];	/* Blocking event block */
	event_t own_stall[1];		/* Owner is stalling for other owner */
#endif
#if !(defined WIN_NT) || (defined WIN_NT && !defined SUPERSERVER)
	event_t own_wakeup[1];		/* Wakeup event block */
#endif
	USHORT own_semaphore;		/* Owner semaphore -- see note below */
	USHORT own_flags;			/* Misc stuff */
} *OWN;

/* Flags in own_flags */
const USHORT OWN_blocking	= 1;		// Owner is blocking
const USHORT OWN_scanned	= 2;		// Owner has been deadlock scanned
const USHORT OWN_manager	= 4;		// Owner is privileged manager
const USHORT OWN_signal		= 8;		// Owner needs signal delivered
const USHORT OWN_wakeup		= 32;		// Owner has been awoken
const USHORT OWN_starved	= 128;		// This thread may be starved

/* Flags in own_ast_flags */
const UATOM OWN_signaled	= 16;		/* Signal is thought to be delivered */

/* Flags in own_semaphore */
const USHORT OWN_semavail	= 0x8000;	/* Process semaphore is available */

/* Flags in own_ast_hung_flag */
const UATOM OWN_hung		= 64;		/* Owner may be hung by OS-level bug */

/* NOTE: own_semaphore, when USE_WAKEUP_EVENTS is set, is used to indicate when a 
   owner is waiting inside wait_for_request().  post_wakeup() will only
   attempt to wakeup owners that are waiting.  The reason for this is
   likely historical - a flag bit could be used for this instead. */

/* Semaphore mask block */
// How can this thing use type_smb if this is unrelated?
// There was a clash between this smb and rse's smb (Sort Map Block).

struct semaphore_mask {
	UCHAR smb_type;				/* memory tag - always type_smb */
	ULONG smb_mask[1];			/* Mask of available semaphores */
};

/* Lock manager history block */

typedef struct his {
	UCHAR his_type;				/* memory tag - always type_his */
	UCHAR his_operation;		/* operation that occured */
	SRQ_PTR his_next;				/* SRQ_PTR to next item in history list */
	SRQ_PTR his_process;			/* owner to record for this operation */
	SRQ_PTR his_lock;				/* lock to record for operation */
	SRQ_PTR his_request;			/* request to record for operation */
} *HIS;

/* his_operation definitions */
// should be UCHAR according to his_operation but is USHORT in lock.cpp:post_operation 
const UCHAR his_enq			= 1;
const UCHAR his_deq			= 2;
const UCHAR his_convert		= 3;
const UCHAR his_signal		= 4;
const UCHAR his_post_ast	= 5;
const UCHAR his_wait		= 6;
const UCHAR his_del_process	= 7;
const UCHAR his_del_lock	= 8;
const UCHAR his_del_request	= 9;
const UCHAR his_deny		= 10;
const UCHAR his_grant		= 11;
const UCHAR his_leave_ast	= 12;
const UCHAR his_scan		= 13;
const UCHAR his_dead		= 14;
const UCHAR his_enter		= 15;
const UCHAR his_bug			= 16;
const UCHAR his_active		= 17;
const UCHAR his_cleanup		= 18;
const UCHAR his_del_owner	= 19;
const UCHAR his_MAX			= his_del_owner;

#endif // ISC_LOCK_LOCK_H

