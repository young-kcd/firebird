/*
 *	PROGRAM:	JRD Lock Manager
 *	MODULE:		printv3s4.c
 *	DESCRIPTION:	Version 3 Sun 4 lock Table printer
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
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include "../lock/lockv3s4.h"
#include "../lock/prtv3_proto.h"

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 64
#endif

#include <sys/param.h>
#include <sys/stat.h>

static USHORT sw_requests, sw_processes, sw_locks, sw_series, sw_history,
	sw_mutex;

static LHB LOCK_header;
static long LOCK_size_mapped;
static LOCK_fd;

#define default_size	8192
#define hash_slots	101

static TEXT *history_names[] = {
	"n/a", "ENQ", "DEQ", "CONVERT", "SIGNAL", "POST", "WAIT",
	"DEL_PROC", "DEL_LOCK", "DEL_REQ", "DENY", "GRANT", "LEAVE",
	"SCAN", "DEAD", "ENTER", "CLEANUP", "BUG", "ACTIVE"
};



static UCHAR compatibility[] = {

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


V3_lock_print(argc, argv)
	 USHORT argc;
	 UCHAR *argv[];
{
/**************************************
 *
 *	V 3 _ l o c k _ p r i n t
 *
 **************************************
 *
 * Functional description
 *	Print v3 lock table structure
 *
 **************************************/
	USHORT name_length, i;
	long start, length;
	SHB shb;
	SRQ que, slot;
	PRB process;
	LBL lock;
	UCHAR *p, c;
	char lock_file[MAXPATHLEN];
	char host_name[MAXHOSTNAMELEN];
	struct stat stat_buf;

/* Handle switches, etc. */

	argv++;
	sw_processes = TRUE;

	while (--argc) {
		p = *argv++;
		while (c = *p++)
			switch (c) {
			case 'o':			/* Owners: compatible with v4 */
			case 'p':
				sw_processes = TRUE;
				break;

			case 'l':
				sw_locks = TRUE;
				break;

			case 'r':
				sw_requests = TRUE;
				break;

			case 'a':
				sw_locks = TRUE;
				sw_processes = TRUE;
				sw_requests = TRUE;
				sw_history = TRUE;
				break;

			case 'h':
				sw_history = TRUE;
				break;

			case 's':
				sw_series = atoi(*argv++);
				--argc;
				break;

			case '-':
				break;

			default:
				/* Silently ignore invalid options - 
				 *  the V4 option parser reported any appropriate error */
				break;
			}
	}

/* Construct a node-specific filename of the form "gds.lockfile.<hostname>" */

	if (gethostname(host_name, sizeof(host_name))) {
		ib_printf("gethostname failed\n");
		return FAILURE;
	}

	sprintf(lock_file, LOCK_FILE, host_name);

	LOCK_fd = open(lock_file, O_RDWR, 0660);
	if (LOCK_fd == -1) {
		/* Silently return - there doesn't have to be a bridge lock file */
		return FAILURE;
	}

	if (stat(lock_file, &stat_buf)) {
		ib_printf("stat failed for bridge lock table %s\n", lock_file);
		return FAILURE;
	}

	if (stat_buf.st_size < sizeof(struct lhb)) {
		/* File is obviously too small to really be a lock file */
		ib_printf("Bridge lock file too small: %s\n", lock_file);
		return FAILURE;
	}

	LOCK_size_mapped = DEFAULT_SIZE;
	LOCK_header =
		(LHB) mmap(NULL, LOCK_size_mapped, PROT_READ | PROT_WRITE, MAP_SHARED,
				   LOCK_fd, 0);

	if ((int) LOCK_header == -1) {
		ib_printf("mmap failed\n");
		return FAILURE;
	}

/* If the table is larger than usual, remap it */

	if (LOCK_header->lhb_length > LOCK_size_mapped) {
		length = LOCK_header->lhb_length;
		LOCK_header =
			(LHB) mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED,
					   LOCK_fd, 0);
	}

/* Print lock header block */

	ib_printf("LOCK_HEADER BLOCK\n");
	ib_printf("\tVersion: %d, Active process: %d, Length: %d, Used: %d\n",
			  LOCK_header->lhb_version, LOCK_header->lhb_active_process,
			  LOCK_header->lhb_length, LOCK_header->lhb_used);

	ib_printf("\tEnqs: %d, Converts: %d, Rejects: %d, Blocks: %d\n",
			  LOCK_header->lhb_enqs, LOCK_header->lhb_converts,
			  LOCK_header->lhb_denies, LOCK_header->lhb_blocks);

	ib_printf("\tDeadlock scans: %d, Deadlocks: %d\n",
			  LOCK_header->lhb_scans, LOCK_header->lhb_deadlocks);

	if (LOCK_header->lhb_secondary != LHB_PATTERN) {
		shb = (SHB) ABS_PTR(LOCK_header->lhb_secondary);
		ib_printf("\tRemove node: %d, Insert queue: %d, Insert prior: %d\n",
				  shb->shb_remove_node, shb->shb_insert_que,
				  shb->shb_insert_prior);
	}

	prt_que("\tProcesses", &LOCK_header->lhb_processes);
	prt_que("\tFree processes", &LOCK_header->lhb_free_processes);
	prt_que("\tFree locks", &LOCK_header->lhb_free_locks);
	prt_que("\tFree requests", &LOCK_header->lhb_free_requests);
	ib_printf("\n");

/* Print known processes */

	if (sw_processes)
		QUE_LOOP(LOCK_header->lhb_processes, que)
			prt_process((UCHAR *) que - OFFSET(PRB, prb_lhb_processes));

/* Print known locks */

	if (sw_locks || sw_series)
		for (slot = LOCK_header->lhb_hash, i = 0;
			 i < LOCK_header->lhb_hash_slots; slot++, i++)
			for (que = (SRQ) ABS_PTR(slot->srq_forward); que != slot;
				 que = (SRQ) ABS_PTR(que->srq_forward))
				prt_lock((UCHAR *) que - OFFSET(LBL, lbl_lhb_hash));

	if (sw_history)
		prt_history(LOCK_header->lhb_history, "History");

	if (LOCK_header->lhb_secondary != LHB_PATTERN)
		prt_history(shb->shb_history, "Event log");

	return FBOK;
}


static void prt_history(history_header, title)
	 PTR history_header;
	 char *title;
{
/**************************************
 *
 *	p r t _ h i s t o r y
 *
 **************************************
 *
 * Functional description
 *	Print history list of lock table.
 *
 **************************************/
	HIS history;

	ib_printf("%s:\n", title);

	for (history = (HIS) ABS_PTR(history_header); TRUE;
		 history = (HIS) ABS_PTR(history->his_next)) {
		if (history->his_operation)
			ib_printf("    %s:\tprocess = %d, lock = %d, request = %d\n",
					  history_names[history->his_operation],
					  history->his_process, history->his_lock,
					  history->his_request);
		if (history->his_next == history_header)
			break;
	}
}


static void prt_lock(lock)
	 LBL lock;
{
/**************************************
 *
 *	p r t _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Print a formatted lock block 
 *
 **************************************/
	long key;
	UCHAR *p, *q, *end;
	SRQ que;
	LRQ request;

	if (sw_series && lock->lbl_series != sw_series)
		return;

	ib_printf("LOCK BLOCK %d\n", REL_PTR(lock));
	ib_printf
		("\tSeries: %d, Parent: %d, State: %d, size: %d length: %d data: %d\n",
		 lock->lbl_series, lock->lbl_parent, lock->lbl_state, lock->lbl_size,
		 lock->lbl_length, lock->lbl_data);

	if (LOCK_header->lhb_version >= LHB_VERSION5)
		q = lock->lbl_key5;
	else
		q = lock->lbl_key3;
	if (lock->lbl_length == 4) {
		p = (UCHAR *) & key;
		for (end = q + 4; q < end; q++)
			*p++ = *q;
		ib_printf("\tKey: %d\n", key);
	}
	else
		ib_printf("\tKey: %.*s\n", lock->lbl_length, q);

	prt_que("\tHash que", &lock->lbl_lhb_hash);
	prt_que("\tRequests", &lock->lbl_requests);

	QUE_LOOP(lock->lbl_requests, que) {
		request = (LRQ) ((UCHAR *) que - OFFSET(LRQ, lrq_lbl_requests));
		ib_printf("\t\tRequest %d, Process: %d, State: %d (%d), Flags: %d\n",
				  REL_PTR(request), request->lrq_process, request->lrq_state,
				  request->lrq_requested, request->lrq_flags);
	}

	ib_printf("\n");
}


static void prt_process(process)
	 PRB process;
{
/**************************************
 *
 *	p r t _ p r o c e s s
 *
 **************************************
 *
 * Functional description
 *	Print a formatted process block.
 *
 **************************************/
	SRQ que;

	ib_printf("PROCESS BLOCK %d\n", REL_PTR(process));
	ib_printf("\tProcess id: %d, flags: %d, pending: %d\n",
			  process->prb_process_id, process->prb_flags,
			  process->prb_pending_request);

	prt_que("\tRequests", &process->prb_requests);
	prt_que("\tBlocks", &process->prb_blocks);
	ib_printf("\n");

	if (sw_requests)
		QUE_LOOP(process->prb_requests, que)
			prt_request((UCHAR *) que - OFFSET(LRQ, lrq_prb_requests));
}


static void prt_request(request)
	 LRQ request;
{
/**************************************
 *
 *	p r t _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Print a format request block.
 *
 **************************************/

	ib_printf("REQUEST BLOCK %d\n", REL_PTR(request));
	ib_printf("\tProcess: %d, Lock: %d, State: %d, Mode: %d, Flags: %x\n",
			  request->lrq_process, request->lrq_lock, request->lrq_state,
			  request->lrq_requested, request->lrq_flags);
	ib_printf("\tAST: %x, argument: %x\n", request->lrq_ast_routine,
			  request->lrq_ast_argument);
	ib_printf("\n");
}


static void prt_que(string, que)
	 UCHAR *string;
	 SRQ que;
{
/**************************************
 *
 *	p r t _ q u e
 *
 **************************************
 *
 * Functional description
 *	Print the contents of a self-relative que.
 *
 **************************************/
	long count, offset;
	SRQ next;

	offset = REL_PTR(que);

	if (offset == que->srq_forward && offset == que->srq_backward) {
		ib_printf("%s: *empty*\n", string);
		return;
	}

	count = 0;

	QUE_LOOP((*que), next)
		++ count;

	ib_printf("%s (%ld):\tforward: %d, backward: %d\n",
			  string, count, que->srq_forward, que->srq_backward);
}
