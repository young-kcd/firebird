/*
 *	PROGRAM:	JRD Lock Manager
 *	MODULE:		lockv3.h
 *	DESCRIPTION:	Version 3 lock manager structure definitions
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
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "XENIX" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "DELTA" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "IMP" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "M88K" port
 *
 */

#include <stdio.h>

#include <sys/types.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "../jrd/common.h"
#include "../lock/fparamv3.h"

#if (defined linux || defined FREEBSD || defined NETBSD || defined DARWIN )
#define SEMUN
#endif

#ifdef sun
#ifndef SOLARIS
#define SEMUN
#endif
#endif

#ifdef sgi
#define SEMUN
#endif

#ifdef SEMUN
#undef SEMUN
#else
union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
};
#endif

#define DEFAULT_SIZE	32768
#define BLOCKING_SIGNAL	SIGUSR1

#ifndef EXTEND_SIZE
#define EXTEND_SIZE	32768
#endif

#ifdef UNIXWARE
#define SEMAPHORES      10
#endif

#ifdef sgi
#define SEMAPHORES	25
#endif

#ifdef ultrix
#define SEMAPHORES	25
#endif

#ifdef DGUX
#define SEMAPHORES	25
#endif

#ifdef SINIXZ
#define SEMAPHORES	25
#endif

#ifdef SOLARIS
#define SEMAPHORES	25
#endif

#ifndef SEMAPHORES
#define SEMAPHORES	32
#endif

#undef BASE
#define BASE 			((UCHAR*) LOCK_header)
#define REL_PTR(item)		((UCHAR*) item - BASE)
#define ABS_PTR(item)		(BASE + item)

#define QUE_INIT(que)	{que.srq_forward = que.srq_backward = REL_PTR (&que);}
#define QUE_EMPTY(que)	(que.srq_forward == REL_PTR (&que))
#define QUE_NEXT(que)	(SRQ) ABS_PTR (que.srq_forward)

#define QUE_LOOP(header,que)	for (que = QUE_NEXT (header);\
	que != &header; que = QUE_NEXT ((*que)))

#define PTR	long

/* Lock states */

#define LCK_none	0
#define LCK_null	1
#define LCK_SR		2			/* Shared Read */
#define LCK_PR		3			/* Protected Read */
#define LCK_SW		4			/* Shared Write */
#define LCK_PW		5			/* Protected Write */
#define LCK_EX		6			/* Exclusive */
#define LCK_max		7

/* Lock block types */

#define type_null	0
#define type_lhb	1
#define type_prb	2
#define type_lrq	3
#define type_lbl	4
#define type_his	5
#define type_smb	6
#define type_shb	7


/* Self-relative que block.  Offsets are from the block itself. */

typedef struct srq {
	PTR srq_forward;			/* Forward offset */
	PTR srq_backward;			/* Backward offset */
} *SRQ;


#define LHB_VERSION5	5
#define LHB_VERSION6	6
#define LHB_VERSION7	7
#define LHB_VERSION	7
#define LHB_PATTERN	123454321

/* Lock header block -- one per lock file, lives up front */

typedef struct lhb {
	UCHAR lhb_type;
	UCHAR lhb_version;			/* Version of lock table */
	PTR lhb_secondary;			/* Secondary lock header block */
	PTR lhb_active_process;		/* Active process, if any */
	struct srq lhb_processes;	/* Que of active processess */
	struct srq lhb_free_processes;	/* Free process blocks */
	struct srq lhb_free_locks;	/* Free lock blocks */
	struct srq lhb_free_requests;	/* Free lock requests */
	long lhb_length;			/* Size of lock table */
	long lhb_used;				/* Bytes of lock table in use */
	USHORT lhb_hash_slots;		/* Number of hash slots allocated */
	long lhb_mutex[2];			/* Mutex controlling access */
	PTR lhb_history;
	int lhb_enqs;
	int lhb_converts;
	int lhb_denies;
	int lhb_deadlocks;
	int lhb_scans;
	int lhb_blocks;
	ULONG lhb_mask;				/* Mask of available semaphores */
	struct srq lhb_hash[1];		/* Hash table */
} *LHB;

/* Secondary header block -- exists only in V3.3 and later lock
   managers.  It is pointed to by the word in the lhb that used to contain
   a pattern. */

typedef struct shb {
	UCHAR shb_type;
	UCHAR shb_flags;
	PTR shb_history;
	PTR shb_remove_node;		/* Node removing itself */
	PTR shb_insert_que;			/* Queue inserting into */
	PTR shb_insert_prior;		/* Prior of inserting queue */
	long shb_misc[10];			/* Unused space */
} *SHB;


/* Lock block */

typedef struct lbl
{
	UCHAR lbl_type;
	UCHAR lbl_state;			/* High state granted */
	UCHAR lbl_size;				/* Key bytes allocated */
	UCHAR lbl_length;			/* Key bytes used */
	struct srq lbl_requests;	/* Requests granted */
	struct srq lbl_lhb_hash;	/* Collision que for hash table */
	long lbl_data;				/* user data */
	PTR lbl_parent;				/* Parent */
	UCHAR lbl_series;			/* Lock series */
	UCHAR lbl_flags;			/* Misc flags */
	UCHAR lbl_counts5[LCK_max];	/* Counts of granted locks */
	UCHAR lbl_key5[1];			/* Key value */
	USHORT lbl_counts7[LCK_max];	/* Counts of granted locks */
	UCHAR lbl_key7[1];			/* Key value */
} *LBL;

#define LBL_scanned	1			/* Lock as been scanned */

/* Lock requests */

typedef struct lrq {
	UCHAR lrq_type;
	UCHAR lrq_flags;			/* Misc crud */
	UCHAR lrq_requested;		/* Level requested  */
	UCHAR lrq_state;			/* State of lock request */
	PTR lrq_process;			/* Process making request */
	PTR lrq_lock;				/* Lock requested */
	struct srq lrq_prb_requests;	/* Locks granted for process */
	struct srq lrq_lbl_requests;	/* Que of requests (active, pending) */
	struct srq lrq_prb_blocks;	/* Process block que */
	int (*lrq_ast_routine) ();	/* Block ast routine */
	int *lrq_ast_argument;		/* Ast argument */
} *LRQ;

#define LRQ_blocking	1		/* Request is blocking */
#define LRQ_pending	2			/* Request is pending */
#define LRQ_converting	4		/* Request is pending conversion */
#define LRQ_rejected	8		/* Request is rejected */
#define LRQ_timed_out	16		/* Wait timed out */

/* Process block */

typedef struct prb
{
	UCHAR prb_type;
	UCHAR prb_flags;			/* Misc stuff */
	struct srq prb_lhb_processes;	/* Processes que */
	struct srq prb_requests;	/* Lock requests granted */
	struct srq prb_blocks;		/* Lock requests blocking */
	PTR prb_pending_request;	/* Request we're waiting on */
	int prb_process_id;			/* Process ID */
	int prb_process_uid;		/* Process UID */
	UCHAR prb_semaphore5;		/* Process semaphore */
	USHORT prb_semaphore7;		/* Process semaphore */
} *PRB;

#define PRB_blocking	1		/* Process is blocking */
#define PRB_scanned	2			/* Process has been deadlock scanned */
#define PRB_manager	4			/* Process is privileged manager */
#define PRB_signal	8			/* Process needs signal delivered */
#define PRB_signaled	16		/* Signal is thought to be delivered */
#define PRB_wakeup	32			/* Process has been awoken */

/* Flag in prb_semaphore */

#define PRB_semavail5	0x80	/* Process semaphore is available */
#define PRB_semavail7	0x8000	/* Process semaphore is available */

/* Semaphore mask block */

#define SMB_SEMAPHORES6	128

typedef struct smb {
	UCHAR smb_type;
	ULONG smb_mask[1];			/* Mask of available semaphores */
} *SMB;

/* Lock manager history block */

typedef struct his {
	UCHAR his_type;
	UCHAR his_operation;
	PTR his_next;
	PTR his_process;
	PTR his_lock;
	PTR his_request;
} *HIS;

#define his_enq		1
#define his_deq		2
#define his_convert	3
#define his_signal	4
#define his_post_ast	5
#define his_wait	6
#define his_del_process	7
#define his_del_lock	8
#define his_del_request	9
#define his_deny	10
#define his_grant	11
#define his_leave_ast	12
#define his_scan	13
#define his_dead	14
#define his_enter	15
#define his_bug		16
#define his_active	17
