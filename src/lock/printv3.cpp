/*
 *	PROGRAM:	JRD Lock Manager
 *	MODULE:		printv3.c
 *	DESCRIPTION:	Version 3 lock Table printer
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
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <sys/param.h>
#include <sys/stat.h>

#include "../jrd/common.h"
#include "../jrd/isc.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/gds_proto.h"
#include "../lock/lockv3.h"
#include "../lock/prtv3_proto.h"

#include <stdlib.h>

#ifndef FOPEN_READ_TYPE
#define FOPEN_READ_TYPE	"r"
#endif

#ifndef MAXPATHLEN
#define MAXPATHLEN	256
#endif

//extern LHB ISC_map_file();

static USHORT sw_requests, sw_processes, sw_locks, sw_series, sw_history,
	sw_mutex;

static LHB LOCK_header;
static int LOCK_shmid, LOCK_semid, LOCK_pid;
static LRQ LOCK_wait_request;
static long LOCK_shm_size = DEFAULT_SIZE;

static struct {
	char *tptr;
	long *vptr;
} LOCK_hdrtbl[] = {
"SHMSIZE", &LOCK_shm_size, NULL, NULL};

static void prt_lock_init();
static void get_lock_header();
static void prt_history(PTR, char *);
static void prt_lock(LBL );
static void prt_process(PRB );
static void prt_request(LRQ );
static void prt_que(UCHAR *, SRQ );

static TEXT *history_names[] = {
	"n/a", "ENQ", "DEQ", "CONVERT", "SIGNAL", "POST", "WAIT",
	"DEL_PROC", "DEL_LOCK", "DEL_REQ", "DENY", "GRANT", "LEAVE",
	"SCAN", "DEAD", "ENTER", "BUG", "ACTIVE"
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


int V3_lock_print(USHORT argc, UCHAR **argv)
{
/**************************************
 *
 *	V 3 _ l o c k _ p r i n t
 *
 **************************************
 *
 * Functional description
 *	Print a v3 lock table for v4
 *
 **************************************/
	USHORT name_length, i;
	long start, length;
	int fd;
	SHB shb;
	SRQ que, slot;
	PRB process;
	LBL lock;
	UCHAR *p, c;
	STATUS status_vector[20];
	SH_MEM_T shmem_data;
	TEXT expanded_filename[512], hostname[64];
	struct stat stat_buf;

/* Handle switches, etc. */

	argv++;
	sw_processes = TRUE;

	while (--argc) {
		p = (UCHAR*) *argv++;
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
				sw_series = atoi((char*) *argv++);
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

/* first, make sure there's a v3 lock table */

#ifdef NOHOSTNAME
	strcpy(expanded_filename, LOCK_FILE);
#else
	sprintf(expanded_filename, LOCK_FILE,
			ISC_get_host(hostname, sizeof(hostname)));
#endif
	if (stat(expanded_filename, &stat_buf)) {
		/* Silently return - there doesn't have to be a bridge file */
		return FAILURE;
	}

	get_lock_header();

#ifdef UNIX
	shmem_data.sh_mem_semaphores = 0;
#endif
	LOCK_header = (LHB) ISC_map_file(status_vector, LOCK_FILE, (void (*)(void *, sh_mem *, int)) prt_lock_init, 0, 0,	/* Map entire length of segment */
									 &shmem_data);

/* if there's nothing mapped, there's nothing to print */

	if (!shmem_data.sh_mem_length_mapped)
		LOCK_header = (LHB) NULL;

	if (LOCK_header && shmem_data.sh_mem_length_mapped < sizeof(struct lhb)) {
		/* File is obviously too small to really be a lock file */
		ib_printf("Bridge lock file too small.\n%s\n", expanded_filename);
		return FAILURE;
	}

	if (LOCK_header && LOCK_header->lhb_length > LOCK_shm_size) {
		length = LOCK_header->lhb_length;
		LOCK_header =
			(LHB) ISC_remap_file(status_vector, &shmem_data, length, FALSE);
		LOCK_shm_size = length;
	}

	if (!LOCK_header) {
		ib_printf("Unable to access bridge lock table\n");
		gds__print_status(status_vector);
		return FAILURE;
	}

	LOCK_shm_size = shmem_data.sh_mem_length_mapped;

/* if there's no version number - admit there's nothing to say and return. */

	if (!LOCK_header->lhb_version) {
		ib_printf("LOCK_HEADER BLOCK\n");
		ib_printf("\tNo bridge lock table present.\n");
		return FAILURE;
	}

/* Print lock header block */

	ib_printf("LOCK_HEADER BLOCK\n");
	ib_printf("\tVersion: %d, Active process: %d, Length: %d, Used: %d\n",
			  LOCK_header->lhb_version, LOCK_header->lhb_active_process,
			  LOCK_header->lhb_length, LOCK_header->lhb_used);

	ib_printf("\tSemmask: %x\n", LOCK_header->lhb_mask);

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

	prt_que((UCHAR*) "\tProcesses", &LOCK_header->lhb_processes);
	prt_que((UCHAR*) "\tFree processes", &LOCK_header->lhb_free_processes);
	prt_que((UCHAR*) "\tFree locks", &LOCK_header->lhb_free_locks);
	prt_que((UCHAR*) "\tFree requests", &LOCK_header->lhb_free_requests);
	ib_printf("\n");

/* Print known processes */

	if (sw_processes)
		QUE_LOOP(LOCK_header->lhb_processes, que)
			prt_process((PRB)(UCHAR *) que - OFFSET(PRB, prb_lhb_processes));

/* Print known locks */

	if (sw_locks || sw_series)
		for (slot = LOCK_header->lhb_hash, i = 0;
			 i < LOCK_header->lhb_hash_slots; slot++, i++)
			for (que = (SRQ) ABS_PTR(slot->srq_forward); que != slot;
				 que = (SRQ) ABS_PTR(que->srq_forward))
				prt_lock((LBL)(UCHAR *) que - OFFSET(LBL, lbl_lhb_hash));

	if (sw_history)
		prt_history(LOCK_header->lhb_history, "History");

	if (LOCK_header->lhb_secondary != LHB_PATTERN)
		prt_history(shb->shb_history, "Event log");

	return SUCCESS;
}


static void get_lock_header()
{
/*************************************
 *
 *	g e t _ l o c k _ h e a d e r
 *
 *************************************
 *
 * Functional description
 *	Read user-defined values for shared memory
 *		and semaphores from LOCK_HEADER.
 *
 ************************************/
	IB_FILE *fd;
	char *p, *q, buf[40];
	int i;

	if (fd = ib_fopen(LOCK_HEADER, FOPEN_READ_TYPE)) {
		while (p = ib_fgets(buf, 40, fd))
			for (i = 0; q = LOCK_hdrtbl[i].tptr; i++) {
				p = buf;
				while (*p++ == *q++);
				if (!*--q) {
					*LOCK_hdrtbl[i].vptr = atoi(p);
					break;
				}
			}
		ib_fclose(fd);
	}
}


static void prt_lock_init()
{
/**************************************
 *
 *	l o c k _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize a lock table to looking -- i.e. don't do
 *	nuthin.
 *
 **************************************/
}


static void prt_history(PTR history_header, char *title)
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


static void prt_lock(LBL lock)
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
	UCHAR c, *p, *q, *end, temp[512];
	SRQ que;
	LRQ request;

	if (sw_series && lock->lbl_series != sw_series)
		return;

	if (LOCK_header->lhb_version >= LHB_VERSION7)
		q = lock->lbl_key7;
	else
		q = lock->lbl_key5;
	for (p = temp, end = q + lock->lbl_length; q < end; q++) {
		c = *q;
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '/')
			*p++ = c;
		else {
			sprintf((char*) p, "<%d>", c);
			while (*p)
				p++;
		}
	}

	*p = 0;

	ib_printf("LOCK BLOCK %d\n", REL_PTR(lock));
	ib_printf
		("\tSeries: %d, Parent: %d, State: %d, size: %d length: %d data: %d\n",
		 lock->lbl_series, lock->lbl_parent, lock->lbl_state, lock->lbl_size,
		 lock->lbl_length, lock->lbl_data);

	if (lock->lbl_length == 4) {
		p = (UCHAR *) & key;
		if (LOCK_header->lhb_version >= LHB_VERSION7)
			q = lock->lbl_key7;
		else
			q = lock->lbl_key5;
		for (end = q + 4; q < end; q++)
			*p++ = *q;
		ib_printf("\tKey: %d\n", key);
	}
	else
		ib_printf("\tKey: %s\n", temp);

	prt_que((UCHAR*) "\tHash que", &lock->lbl_lhb_hash);

	prt_que((UCHAR*) "\tRequests", &lock->lbl_requests);
	QUE_LOOP(lock->lbl_requests, que) {
		request = (LRQ) ((UCHAR *) que - OFFSET(LRQ, lrq_lbl_requests));
		ib_printf("\t\tRequest %d, Process: %d, State: %d (%d), Flags: %d\n",
				  REL_PTR(request), request->lrq_process, request->lrq_state,
				  request->lrq_requested, request->lrq_flags);
	}

	ib_printf("\n");
}


static void prt_process(PRB process)
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
	ib_printf
		("\tProcess id: %d, flags: %d, pending: %d, UID: %x, semid: %d\n",
		 process->prb_process_id, process->prb_flags,
		 process->prb_pending_request, process->prb_process_uid,
		 (LOCK_header->lhb_version >=
		  LHB_VERSION7) ? process->prb_semaphore7 : process->prb_semaphore5);

	prt_que((UCHAR*) "\tRequests", &process->prb_requests);
	prt_que((UCHAR*) "\tBlocks", &process->prb_blocks);
	ib_printf("\n");

	if (sw_requests)
		QUE_LOOP(process->prb_requests, que)
			prt_request((LRQ) (UCHAR *) que - OFFSET(LRQ, lrq_prb_requests));
}


static void prt_request(LRQ request)
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


static void prt_que(UCHAR *string, SRQ que)
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
