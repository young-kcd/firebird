/*
 *      PROGRAM:        JRD Lock Manager
 *      MODULE:         print.c
 *      DESCRIPTION:    Lock Table printer
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
 * 2002.10.27 Sean Leyne - Completed removal of obsolete "DELTA" port
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "Ultrix" port
 *
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "SGI" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jrd/common.h"
#include "../jrd/file_params.h"
#include "../jrd/jrd.h"
#include "../jrd/lck.h"
#include "../jrd/isc.h"
#include "../jrd/jrd_time.h"
#include "../lock/lock.h"
#include "../jrd/gdsassert.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_s_proto.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <sys/stat.h>

#ifdef HAVE_SYS_PARAM_H
#  include <sys/param.h>
#endif

#ifdef WIN_NT
#include <io.h>
#endif

#ifndef FPRINTF
#define FPRINTF         fprintf
#endif

typedef FILE* OUTFILE;

const USHORT SW_I_ACQUIRE	= 1;
const USHORT SW_I_OPERATION	= 2;
const USHORT SW_I_TYPE		= 4;
const USHORT SW_I_WAIT		= 8;

#define SRQ_BASE                    ((UCHAR*) LOCK_header)

struct waitque {
	USHORT waitque_depth;
	SRQ_PTR waitque_entry[30];
};

static void prt_lock_activity(OUTFILE, const lhb*, USHORT, USHORT, USHORT);
static void prt_lock_init(void*, sh_mem*, bool);
static void prt_history(OUTFILE, const lhb*, SRQ_PTR, const SCHAR*);
static void prt_lock(OUTFILE, const lhb*, LBL, USHORT);
static void prt_owner(OUTFILE, const lhb*, const own*, bool, bool);
static void prt_owner_wait_cycle(OUTFILE, const lhb*, const own*, USHORT, waitque*);
static void prt_request(OUTFILE, const lhb*, const lrq*);
static void prt_que(OUTFILE, const lhb*, const SCHAR*, const srq*, USHORT);
static void prt_que2(OUTFILE, const lhb*, const SCHAR*, const srq*, USHORT);

static const TEXT history_names[][10] = {
	"n/a", "ENQ", "DEQ", "CONVERT", "SIGNAL", "POST", "WAIT",
	"DEL_PROC", "DEL_LOCK", "DEL_REQ", "DENY", "GRANT", "LEAVE",
	"SCAN", "DEAD", "ENTER", "BUG", "ACTIVE", "CLEANUP", "DEL_OWNER"
};

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



int CLIB_ROUTINE main( int argc, char *argv[])
{
/**************************************
 *
 *      m a i n
 *
 **************************************
 *
 * Functional description
 *	Check switches passed in and prepare to dump the lock table
 *	to stdout.
 *
 **************************************/
	OUTFILE outfile = stdout;

/* Perform some special handling when run as an Interbase service.  The
   first switch can be "-svc" (lower case!) or it can be "-svc_re" followed
   by 3 file descriptors to use in re-directing stdin, stdout, and stderr. */

	if (argc > 1 && !strcmp(argv[1], "-svc")) {
		argv++;
		argc--;
	}
	else if (argc > 4 && !strcmp(argv[1], "-svc_re")) {
		long redir_in = atol(argv[2]);
		long redir_out = atol(argv[3]);
		long redir_err = atol(argv[4]);
#ifdef WIN_NT
		redir_in = _open_osfhandle(redir_in, 0);
		redir_out = _open_osfhandle(redir_out, 0);
		redir_err = _open_osfhandle(redir_err, 0);
#endif
		if (redir_in != 0)
			if (dup2((int) redir_in, 0))
				close((int) redir_in);
		if (redir_out != 1)
			if (dup2((int) redir_out, 1))
				close((int) redir_out);
		if (redir_err != 2)
			if (dup2((int) redir_err, 2))
				close((int) redir_err);
		argv += 4;
		argc -= 4;
	}
	//const int orig_argc = argc;
	//SCHAR** orig_argv = argv;

/* Handle switches, etc. */

	argv++;
	bool sw_consistency = false;
	bool sw_waitlist = false;
	bool sw_file = false;
	bool sw_requests = false;
	bool sw_locks = false;
	bool sw_history = false;
	bool sw_nobridge = false;
	bool sw_owners = true;
	
	USHORT sw_interactive;
	// Those variables should be signed to accept negative values from atoi
	SSHORT sw_series;
	SSHORT sw_intervals;
	SSHORT sw_seconds;
	sw_series = sw_interactive = sw_intervals = sw_seconds = 0;
	TEXT* lock_file = NULL;

	while (--argc) {
		SCHAR* p = *argv++;
		if (*p++ != '-') {
			FPRINTF(outfile,
					"Valid switches are: -o, -p, -l, -r, -a, -h, -n, -s <n>, -c, -i <n> <n>\n");
			exit(FINI_OK);
		}
		SCHAR c;
		while (c = *p++)
			switch (c) {
			case 'o':
			case 'p':
				sw_owners = true;
				break;

			case 'c':
				sw_nobridge = true;
				sw_consistency = true;
				break;

			case 'l':
				sw_locks = true;
				break;

			case 'r':
				sw_requests = true;
				break;

			case 'a':
				sw_locks = true;
				sw_owners = true;
				sw_requests = true;
				sw_history = true;
				break;

			case 'h':
				sw_history = true;
				break;

			case 's':
				if (argc > 1)
					sw_series = atoi(*argv++);
				if (sw_series <= 0) {
					FPRINTF(outfile,
							"Please specify a positive value following option -s\n");
					exit(FINI_OK);
				}
				--argc;
				break;

			case 'n':
				sw_nobridge = true;
				break;

			case 'i':
				while (c = *p++)
					switch (c) {
					case 'a':
						sw_interactive |= SW_I_ACQUIRE;
						break;

					case 'o':
						sw_interactive |= SW_I_OPERATION;
						break;

					case 't':
						sw_interactive |= SW_I_TYPE;
						break;

					case 'w':
						sw_interactive |= SW_I_WAIT;
						break;

					default:
						FPRINTF(outfile,
								"Valid interactive switches are: a, o, t, w\n");
						exit(FINI_OK);
						break;
					}
				if (!sw_interactive)
					sw_interactive =
						(SW_I_ACQUIRE | SW_I_OPERATION | SW_I_TYPE |
						 SW_I_WAIT);
				sw_nobridge = true;
				sw_seconds = sw_intervals = 1;
				if (argc > 1) {
					sw_seconds = atoi(*argv++);
					--argc;
					if (argc > 1) {
						sw_intervals = atoi(*argv++);
						--argc;
					}
					if (!(sw_seconds > 0) || (sw_intervals < 0)) {
						FPRINTF(outfile,
								"Please specify 2 positive values for option -i\n");
						exit(FINI_OK);
					}
				}
				--p;
				break;

			case 'w':
				sw_nobridge = true;
				sw_waitlist = true;
				break;

			case 'f':
				sw_nobridge = true;
				sw_file = true;
				if (argc > 1) {
					lock_file = *argv++;
					--argc;
				}
				else {
					FPRINTF(outfile, "Usage: -f <filename>\n");
					exit(FINI_OK);
				}
				break;

			default:
				FPRINTF(outfile,
						"Valid switches are: -o, -p, -l, -r, -a, -h, -n, -s <n>, -c, -i <n> <n>\n");
				exit(FINI_OK);
				break;
			}
	}

	TEXT buffer[MAXPATHLEN];
	if (!sw_file) {
		gds__prefix_lock(buffer, LOCK_FILE);
		lock_file = buffer;
	}

	SH_MEM_T shmem_data;
#ifdef UNIX
	shmem_data.sh_mem_semaphores = 0;
#endif

	SLONG LOCK_size_mapped = DEFAULT_SIZE;

#ifdef UNIX
	LOCK_size_mapped = 0;		/* Use length of existing segment */
#else
	LOCK_size_mapped = DEFAULT_SIZE;	/* length == 0 not supported by all non-UNIX */
#endif

	ISC_STATUS_ARRAY status_vector;
	
	lhb* LOCK_header = (lhb*) ISC_map_file(status_vector,
							lock_file,
							prt_lock_init,
							0,
							-LOCK_size_mapped,	/* Negative to NOT truncate file */
							&shmem_data);

	TEXT expanded_lock_filename[MAXPATHLEN];
	TEXT hostname[64];
	sprintf(expanded_lock_filename, lock_file,
			ISC_get_host(hostname, sizeof(hostname)));

/* Make sure the lock file is valid - if it's a zero length file we 
 * can't look at the header without causing a BUS error by going
 * off the end of the mapped region.
 */

	if (LOCK_header && shmem_data.sh_mem_length_mapped < (SLONG) sizeof(lhb)) {
		/* Mapped file is obviously too small to really be a lock file */
		FPRINTF(outfile,
				"Unable to access lock table - file too small.\n%s\n",
				expanded_lock_filename);
		exit(FINI_OK);
	}

	if (LOCK_header
		&& LOCK_header->lhb_length > shmem_data.sh_mem_length_mapped)
	{
#if (!(defined UNIX) || (defined HAVE_MMAP))
		SLONG length = LOCK_header->lhb_length;
		LOCK_header = (lhb*) ISC_remap_file(status_vector, &shmem_data, length,
										   FALSE);
#endif
	}

	if (!LOCK_header) {
		FPRINTF(outfile, "Unable to access lock table.\n%s\n",
				expanded_lock_filename);
		gds__print_status(status_vector);
		exit(FINI_OK);
	}

	LOCK_size_mapped = shmem_data.sh_mem_length_mapped;

/* if we can't read this version - admit there's nothing to say and return. */

	if ((LOCK_header->lhb_version != SS_LHB_VERSION) &&
		(LOCK_header->lhb_version != CLASSIC_LHB_VERSION))
	{
		FPRINTF(outfile, "\tUnable to read lock table version %d.\n",
				LOCK_header->lhb_version);
		exit(FINI_OK);
	}

/* Print lock activity report */

	if (sw_interactive) {
		prt_lock_activity(outfile, LOCK_header, sw_interactive, (USHORT) sw_seconds,
						  (USHORT) sw_intervals);
		exit(FINI_OK);
	}

	lhb* header = NULL;
	
	if (sw_consistency) {
		/* To avoid changes in the lock file while we are dumping it - make
		 * a local buffer, lock the lock file, copy it, then unlock the
		 * lock file to let processing continue.  Printing of the lock file
		 * will continue from the in-memory copy.
		 */
		header = (lhb*) gds__alloc(LOCK_header->lhb_length);
		if (!header) {
			FPRINTF(outfile,
					"Insufficient memory for consistent lock statistics.\n");
			FPRINTF(outfile, "Try omitting the -c switch.\n");
			exit(FINI_OK);
		}

		ISC_mutex_lock(LOCK_header->lhb_mutex);
		memcpy(header, LOCK_header, LOCK_header->lhb_length);
		ISC_mutex_unlock(LOCK_header->lhb_mutex);
		LOCK_header = header;
	}

/* Print lock header block */

	FPRINTF(outfile, "LOCK_HEADER BLOCK\n");
	FPRINTF(outfile,
			"\tVersion: %d, Active owner: %6"SLONGFORMAT", Length: %6"SLONGFORMAT
			", Used: %6"SLONGFORMAT"\n",
			LOCK_header->lhb_version, LOCK_header->lhb_active_owner,
			LOCK_header->lhb_length, LOCK_header->lhb_used);

#ifdef MANAGER_PROCESS
	int manager_pid = 0;
	if (LOCK_header->lhb_manager) {
		OWN manager = (OWN) SRQ_ABS_PTR(LOCK_header->lhb_manager);
		manager_pid = manager->own_process_id;
	}

	FPRINTF(outfile, "\tLock manager pid: %6d\n", manager_pid);
#endif

	FPRINTF(outfile, "\tSemmask: 0x%"XLONGFORMAT", Flags: 0x%04X\n",
			LOCK_header->lhb_mask, LOCK_header->lhb_flags);

	FPRINTF(outfile,
			"\tEnqs: %6"UQUADFORMAT", Converts: %6"UQUADFORMAT
			", Rejects: %6"UQUADFORMAT", Blocks: %6"UQUADFORMAT"\n",
			LOCK_header->lhb_enqs, LOCK_header->lhb_converts,
			LOCK_header->lhb_denies, LOCK_header->lhb_blocks);

	FPRINTF(outfile,
			"\tDeadlock scans: %6"UQUADFORMAT", Deadlocks: %6"UQUADFORMAT
			", Scan interval: %3"ULONGFORMAT"\n",
			LOCK_header->lhb_scans, LOCK_header->lhb_deadlocks,
			LOCK_header->lhb_scan_interval);

	FPRINTF(outfile,
			"\tAcquires: %6"UQUADFORMAT", Acquire blocks: %6"UQUADFORMAT
			", Spin count: %3"ULONGFORMAT"\n",
			LOCK_header->lhb_acquires, LOCK_header->lhb_acquire_blocks,
			LOCK_header->lhb_acquire_spins);

	if (LOCK_header->lhb_acquire_blocks) {
		// CVC: MSVC up to v6 couldn't convert UINT64 to double.
		const float bottleneck =
			(float) ((100. * (SINT64) LOCK_header->lhb_acquire_blocks) /
					 (SINT64) LOCK_header->lhb_acquires);
		FPRINTF(outfile, "\tMutex wait: %3.1f%%\n", bottleneck);
	}
	else
		FPRINTF(outfile, "\tMutex wait: 0.0%%\n");

	SLONG hash_total_count = 0;
	SLONG hash_max_count = 0;
	SLONG hash_min_count = 10000000;
	USHORT i = 0;
	for (const srq* slot = LOCK_header->lhb_hash; i < LOCK_header->lhb_hash_slots;
		 slot++, i++)
	{
		SLONG hash_lock_count = 0;
		for (const srq* que_inst = (SRQ) SRQ_ABS_PTR(slot->srq_forward); que_inst != slot;
			 que_inst = (SRQ) SRQ_ABS_PTR(que_inst->srq_forward))
		{
			++hash_total_count;
			++hash_lock_count;
		}
		if (hash_lock_count < hash_min_count)
			hash_min_count = hash_lock_count;
		if (hash_lock_count > hash_max_count)
			hash_max_count = hash_lock_count;
	}

	FPRINTF(outfile, "\tHash slots: %4d, ", LOCK_header->lhb_hash_slots);

	FPRINTF(outfile, "Hash lengths (min/avg/max): %4"SLONGFORMAT"/%4"SLONGFORMAT
			"/%4"SLONGFORMAT"\n",
			hash_min_count, (hash_total_count / LOCK_header->lhb_hash_slots),
			hash_max_count);

	const shb* a_shb = NULL;
	if (LOCK_header->lhb_secondary != LHB_PATTERN) {
		a_shb = (shb*) SRQ_ABS_PTR(LOCK_header->lhb_secondary);
		FPRINTF(outfile,
				"\tRemove node: %6"SLONGFORMAT", Insert queue: %6"SLONGFORMAT
				", Insert prior: %6"SLONGFORMAT"\n",
				a_shb->shb_remove_node, a_shb->shb_insert_que,
				a_shb->shb_insert_prior);
	}

	prt_que(outfile, LOCK_header, "\tOwners", &LOCK_header->lhb_owners,
			OFFSET(OWN, own_lhb_owners));
	prt_que(outfile, LOCK_header, "\tFree owners",
			&LOCK_header->lhb_free_owners, OFFSET(OWN, own_lhb_owners));
	prt_que(outfile, LOCK_header, "\tFree locks",
			&LOCK_header->lhb_free_locks, OFFSET(LBL, lbl_lhb_hash));
	prt_que(outfile, LOCK_header, "\tFree requests",
			&LOCK_header->lhb_free_requests, OFFSET(LRQ, lrq_lbl_requests));

/* Print lock ordering option */

	FPRINTF(outfile, "\tLock Ordering: %s\n",
			(LOCK_header->lhb_flags & LHB_lock_ordering) ?
				"Enabled" : "Disabled");
	FPRINTF(outfile, "\n");

/* Print known owners */

	if (sw_owners) {
		const srq* que_inst;
		
#ifdef SOLARIS_MT
		/* The Lock Starvation recovery code on Solaris rotates the owner
		   queue once per acquire.  This makes it difficult to read the
		   printouts when multiple runs are made.  So we scan the list
		   of owners once to find the lowest owner_id, and start the printout
		   from there. */
		SRQ_PTR least_owner_id = 0x7FFFFFFF;
		const srq* least_owner_ptr = &LOCK_header->lhb_owners;
		SRQ_LOOP(LOCK_header->lhb_owners, que_inst) {
			OWN this_owner = (OWN) ((UCHAR*) que_inst - OFFSET(OWN, own_lhb_owners));
			if (SRQ_REL_PTR(this_owner) < least_owner_id) {
				least_owner_id = SRQ_REL_PTR(this_owner);
				least_owner_ptr = que_inst;
			}
		}
		que_inst = least_owner_ptr;
		do {
			if (que_inst != &LOCK_header->lhb_owners)
				prt_owner(outfile, LOCK_header,
						  (OWN) ((UCHAR*) que_inst - OFFSET(OWN, own_lhb_owners)),
						  sw_requests, sw_waitlist);
			que_inst = SRQ_NEXT((*que_inst));
		} while (que_inst != least_owner_ptr);
#else
		SRQ_LOOP(LOCK_header->lhb_owners, que_inst) {
			prt_owner(outfile, LOCK_header,
					  (OWN) ((UCHAR*) que_inst - OFFSET(OWN, own_lhb_owners)),
					  sw_requests, sw_waitlist);
		}
#endif /* SOLARIS_MT */
	}

/* Print known locks */

	if (sw_locks || sw_series) {
		USHORT i2 = 0;
		for (const srq* slot = LOCK_header->lhb_hash;
			 i2 < LOCK_header->lhb_hash_slots; slot++, i2++)
		{
			for (const srq* que_inst = (SRQ) SRQ_ABS_PTR(slot->srq_forward); que_inst != slot;
				 que_inst = (SRQ) SRQ_ABS_PTR(que_inst->srq_forward))
			{
				prt_lock(outfile, LOCK_header,
						 (LBL) ((UCHAR *) que_inst - OFFSET(LBL, lbl_lhb_hash)),
						 sw_series);
			}
		}
	}

	if (sw_history)
		prt_history(outfile, LOCK_header, LOCK_header->lhb_history,
					"History");

	if (LOCK_header->lhb_secondary != LHB_PATTERN)
		prt_history(outfile, LOCK_header, a_shb->shb_history, "Event log");

	if (header)
		gds__free(header);
	exit(FINI_OK);
	return (FINI_OK); // make compiler happy
}


static void prt_lock_activity(
							  OUTFILE outfile,
							  const lhb* header,
							  USHORT flag, USHORT seconds, USHORT intervals)
{
/**************************************
 *
 *	p r t _ l o c k _ a c t i v i t y
 *
 **************************************
 *
 * Functional description
 *	Print a time-series lock activity report 
 *
 **************************************/
	ULONG i;

	time_t clock = time(NULL);
	tm d = *localtime(&clock);

	FPRINTF(outfile, "%02d:%02d:%02d ", d.tm_hour, d.tm_min, d.tm_sec);

	if (flag & SW_I_ACQUIRE)
		FPRINTF(outfile,
				"acquire/s acqwait/s  %%acqwait acqrtry/s rtrysuc/s ");

	if (flag & SW_I_OPERATION)
		FPRINTF(outfile,
				"enqueue/s convert/s downgrd/s dequeue/s readata/s wrtdata/s qrydata/s ");

	if (flag & SW_I_TYPE)
		FPRINTF(outfile,
				"  dblop/s  rellop/s pagelop/s tranlop/s relxlop/s idxxlop/s misclop/s ");

	if (flag & SW_I_WAIT) {
		FPRINTF(outfile,
				"   wait/s  reject/s timeout/s blckast/s  dirsig/s  indsig/s ");
		FPRINTF(outfile, " wakeup/s dlkscan/s deadlck/s avlckwait(msec)");
	}

	FPRINTF(outfile, "\n");

	lhb base = *header;
	lhb prior = *header;
	if (intervals == 0)
		memset(&base, 0, sizeof(base));

	for (i = 0; i < intervals; i++) {
#ifdef WIN_NT
		Sleep((DWORD) seconds * 1000);
#else
		sleep(seconds);
#endif
		clock = time(NULL);
		d = *localtime(&clock);

		FPRINTF(outfile, "%02d:%02d:%02d ", d.tm_hour, d.tm_min, d.tm_sec);

		if (flag & SW_I_ACQUIRE) {
			FPRINTF(outfile, "%9"UQUADFORMAT" %9"UQUADFORMAT" %9"UQUADFORMAT
					" %9"UQUADFORMAT" %9"UQUADFORMAT" ",
					(header->lhb_acquires - prior.lhb_acquires) / seconds,
					(header->lhb_acquire_blocks -
					 prior.lhb_acquire_blocks) / seconds,
					(header->lhb_acquires -
					 prior.lhb_acquires) ? (100 *
											(header->lhb_acquire_blocks -
											 prior.lhb_acquire_blocks)) /
					(header->lhb_acquires - prior.lhb_acquires) : 0,
					(header->lhb_acquire_retries -
					 prior.lhb_acquire_retries) / seconds,
					(header->lhb_retry_success -
					 prior.lhb_retry_success) / seconds);

			prior.lhb_acquires = header->lhb_acquires;
			prior.lhb_acquire_blocks = header->lhb_acquire_blocks;
			prior.lhb_acquire_retries = header->lhb_acquire_retries;
			prior.lhb_retry_success = header->lhb_retry_success;
		}

		if (flag & SW_I_OPERATION) {
			FPRINTF(outfile, "%9"UQUADFORMAT" %9"UQUADFORMAT" %9"UQUADFORMAT
					" %9"UQUADFORMAT" %9"UQUADFORMAT" %9"UQUADFORMAT
					" %9"UQUADFORMAT" ",
					(header->lhb_enqs - prior.lhb_enqs) / seconds,
					(header->lhb_converts - prior.lhb_converts) / seconds,
					(header->lhb_downgrades - prior.lhb_downgrades) / seconds,
					(header->lhb_deqs - prior.lhb_deqs) / seconds,
					(header->lhb_read_data - prior.lhb_read_data) / seconds,
					(header->lhb_write_data - prior.lhb_write_data) / seconds,
					(header->lhb_query_data -
					 prior.lhb_query_data) / seconds);

			prior.lhb_enqs = header->lhb_enqs;
			prior.lhb_converts = header->lhb_converts;
			prior.lhb_downgrades = header->lhb_downgrades;
			prior.lhb_deqs = header->lhb_deqs;
			prior.lhb_read_data = header->lhb_read_data;
			prior.lhb_write_data = header->lhb_write_data;
			prior.lhb_query_data = header->lhb_query_data;
		}

		if (flag & SW_I_TYPE) {
			FPRINTF(outfile, "%9"UQUADFORMAT" %9"UQUADFORMAT" %9"UQUADFORMAT
					" %9"UQUADFORMAT" %9"UQUADFORMAT" %9"UQUADFORMAT
					" %9"UQUADFORMAT" ",
					(header->lhb_operations[Jrd::LCK_database] -
					 prior.lhb_operations[Jrd::LCK_database]) / seconds,
					(header->lhb_operations[Jrd::LCK_relation] -
					 prior.lhb_operations[Jrd::LCK_relation]) / seconds,
					(header->lhb_operations[Jrd::LCK_bdb] -
					 prior.lhb_operations[Jrd::LCK_bdb]) / seconds,
					(header->lhb_operations[Jrd::LCK_tra] -
					 prior.lhb_operations[Jrd::LCK_tra]) / seconds,
					(header->lhb_operations[Jrd::LCK_rel_exist] -
					 prior.lhb_operations[Jrd::LCK_rel_exist]) / seconds,
					(header->lhb_operations[Jrd::LCK_idx_exist] -
					 prior.lhb_operations[Jrd::LCK_idx_exist]) / seconds,
					(header->lhb_operations[0] -
					 prior.lhb_operations[0]) / seconds);

			prior.lhb_operations[Jrd::LCK_database] =
				header->lhb_operations[Jrd::LCK_database];
			prior.lhb_operations[Jrd::LCK_relation] =
				header->lhb_operations[Jrd::LCK_relation];
			prior.lhb_operations[Jrd::LCK_bdb] = header->lhb_operations[Jrd::LCK_bdb];
			prior.lhb_operations[Jrd::LCK_tra] = header->lhb_operations[Jrd::LCK_tra];
			prior.lhb_operations[Jrd::LCK_rel_exist] =
				header->lhb_operations[Jrd::LCK_rel_exist];
			prior.lhb_operations[Jrd::LCK_idx_exist] =
				header->lhb_operations[Jrd::LCK_idx_exist];
			prior.lhb_operations[0] = header->lhb_operations[0];
		}

		if (flag & SW_I_WAIT) {
			FPRINTF(outfile, "%9"UQUADFORMAT" %9"UQUADFORMAT" %9"UQUADFORMAT
					" %9"UQUADFORMAT" %9"UQUADFORMAT" %9"UQUADFORMAT
					" %9"UQUADFORMAT" %9"UQUADFORMAT" %9"UQUADFORMAT
					" %9"UQUADFORMAT" ",
					(header->lhb_waits - prior.lhb_waits) / seconds,
					(header->lhb_denies - prior.lhb_denies) / seconds,
					(header->lhb_timeouts - prior.lhb_timeouts) / seconds,
					(header->lhb_blocks - prior.lhb_blocks) / seconds,
					(header->lhb_direct_sigs -
					 prior.lhb_direct_sigs) / seconds,
					(header->lhb_indirect_sigs -
					 prior.lhb_indirect_sigs) / seconds,
					(header->lhb_wakeups - prior.lhb_wakeups) / seconds,
					(header->lhb_scans - prior.lhb_scans) / seconds,
					(header->lhb_deadlocks - prior.lhb_deadlocks) / seconds,
					(header->lhb_waits -
					 prior.lhb_waits) ? (header->lhb_wait_time -
										 prior.lhb_wait_time) /
					(header->lhb_waits - prior.lhb_waits) : 0);

			prior.lhb_waits = header->lhb_waits;
			prior.lhb_denies = header->lhb_denies;
			prior.lhb_timeouts = header->lhb_timeouts;
			prior.lhb_blocks = header->lhb_blocks;
			prior.lhb_direct_sigs = header->lhb_direct_sigs;
			prior.lhb_indirect_sigs = header->lhb_indirect_sigs;
			prior.lhb_wakeups = header->lhb_wakeups;
			prior.lhb_scans = header->lhb_scans;
			prior.lhb_deadlocks = header->lhb_deadlocks;
			prior.lhb_wait_time = header->lhb_wait_time;
		}

		FPRINTF(outfile, "\n");
	}

	ULONG factor = seconds * intervals;
	if (factor < 1)
		factor = 1;

	FPRINTF(outfile, "\nAverage: ");
	if (flag & SW_I_ACQUIRE) {
		FPRINTF(outfile, "%9"UQUADFORMAT" %9"UQUADFORMAT" %9"UQUADFORMAT
				" %9"UQUADFORMAT" %9"UQUADFORMAT" ",
				(header->lhb_acquires - base.lhb_acquires) / (factor),
				(header->lhb_acquire_blocks -
				 base.lhb_acquire_blocks) / (factor),
				(header->lhb_acquires -
				 base.lhb_acquires) ? (100 * (header->lhb_acquire_blocks -
											  base.lhb_acquire_blocks)) /
				(header->lhb_acquires - base.lhb_acquires) : 0,
				(header->lhb_acquire_retries -
				 base.lhb_acquire_retries) / (factor),
				(header->lhb_retry_success -
				 base.lhb_retry_success) / (factor));
	}

	if (flag & SW_I_OPERATION) {
		FPRINTF(outfile, "%9"UQUADFORMAT" %9"UQUADFORMAT" %9"UQUADFORMAT
				" %9"UQUADFORMAT" %9"UQUADFORMAT" %9"UQUADFORMAT" %9"
				UQUADFORMAT" ",
				(header->lhb_enqs - base.lhb_enqs) / (factor),
				(header->lhb_converts - base.lhb_converts) / (factor),
				(header->lhb_downgrades - base.lhb_downgrades) / (factor),
				(header->lhb_deqs - base.lhb_deqs) / (factor),
				(header->lhb_read_data - base.lhb_read_data) / (factor),
				(header->lhb_write_data - base.lhb_write_data) / (factor),
				(header->lhb_query_data - base.lhb_query_data) / (factor));
	}

	if (flag & SW_I_TYPE) {
		FPRINTF(outfile, "%9"UQUADFORMAT" %9"UQUADFORMAT" %9"UQUADFORMAT
				" %9"UQUADFORMAT" %9"UQUADFORMAT" %9"UQUADFORMAT
				" %9"UQUADFORMAT" ",
				(header->lhb_operations[Jrd::LCK_database] -
				 base.lhb_operations[Jrd::LCK_database]) / (factor),
				(header->lhb_operations[Jrd::LCK_relation] -
				 base.lhb_operations[Jrd::LCK_relation]) / (factor),
				(header->lhb_operations[Jrd::LCK_bdb] -
				 base.lhb_operations[Jrd::LCK_bdb]) / (factor),
				(header->lhb_operations[Jrd::LCK_tra] -
				 base.lhb_operations[Jrd::LCK_tra]) / (factor),
				(header->lhb_operations[Jrd::LCK_rel_exist] -
				 base.lhb_operations[Jrd::LCK_rel_exist]) / (factor),
				(header->lhb_operations[Jrd::LCK_idx_exist] -
				 base.lhb_operations[Jrd::LCK_idx_exist]) / (factor),
				(header->lhb_operations[0] -
				 base.lhb_operations[0]) / (factor));
	}

	if (flag & SW_I_WAIT) {
		FPRINTF(outfile, "%9"UQUADFORMAT" %9"UQUADFORMAT" %9"UQUADFORMAT
				" %9"UQUADFORMAT" %9"UQUADFORMAT" %9"UQUADFORMAT
				" %9"UQUADFORMAT" %9"UQUADFORMAT" %9"UQUADFORMAT
				" %9"UQUADFORMAT" ",
				(header->lhb_waits - base.lhb_waits) / (factor),
				(header->lhb_denies - base.lhb_denies) / (factor),
				(header->lhb_timeouts - base.lhb_timeouts) / (factor),
				(header->lhb_blocks - base.lhb_blocks) / (factor),
				(header->lhb_direct_sigs - base.lhb_direct_sigs) / (factor),
				(header->lhb_indirect_sigs -
				 base.lhb_indirect_sigs) / (factor),
				(header->lhb_wakeups - base.lhb_wakeups) / (factor),
				(header->lhb_scans - base.lhb_scans) / (factor),
				(header->lhb_deadlocks - base.lhb_deadlocks) / (factor),
				(header->lhb_waits -
				 base.lhb_waits) ? (header->lhb_wait_time -
									base.lhb_wait_time) / (header->lhb_waits -
														   base.
														   lhb_waits) : 0);
	}

	FPRINTF(outfile, "\n");
}


static void prt_lock_init(void*, sh_mem*, bool)
{
/**************************************
 *
 *      l o c k _ i n i t
 *
 **************************************
 *
 * Functional description
 *      Initialize a lock table to looking -- i.e. don't do
 *      nuthin.
 *
 **************************************/
}


static void prt_history(
						OUTFILE outfile,
						const lhb* LOCK_header,
						SRQ_PTR history_header,
						const SCHAR* title)
{
/**************************************
 *
 *      p r t _ h i s t o r y
 *
 **************************************
 *
 * Functional description
 *      Print history list of lock table.
 *
 **************************************/
	FPRINTF(outfile, "%s:\n", title);

	for (const his* history = (HIS) SRQ_ABS_PTR(history_header); true;
		 history = (HIS) SRQ_ABS_PTR(history->his_next))
	{
		if (history->his_operation)
			FPRINTF(outfile,
					"    %s:\towner = %6"ULONGFORMAT", lock = %6"ULONGFORMAT
					", request = %6"ULONGFORMAT"\n",
					history_names[history->his_operation],
					history->his_process, history->his_lock,
					history->his_request);
		if (history->his_next == history_header)
			break;
	}
}


static void prt_lock(
					 OUTFILE outfile,
					 const lhb* LOCK_header, LBL lock, USHORT sw_series)
{
/**************************************
 *
 *      p r t _ l o c k
 *
 **************************************
 *
 * Functional description
 *      Print a formatted lock block 
 *
 **************************************/
	if (sw_series && lock->lbl_series != sw_series)
		return;

	FPRINTF(outfile, "LOCK BLOCK %6"SLONGFORMAT"\n", SRQ_REL_PTR(lock));
	FPRINTF(outfile,
			"\tSeries: %d, Parent: %6"ULONGFORMAT
			", State: %d, size: %d length: %d data: %"ULONGFORMAT"\n",
			lock->lbl_series, lock->lbl_parent, lock->lbl_state,
			lock->lbl_size, lock->lbl_length, lock->lbl_data);

	if (lock->lbl_length == 4) {
		SLONG key;
		UCHAR* p = (UCHAR *) &key;
		const UCHAR* q = lock->lbl_key;
		for (const UCHAR* const end = q + 4; q < end; q++)
			*p++ = *q;
		FPRINTF(outfile, "\tKey: %06"SLONGFORMAT",", key);
	}
	else {
		UCHAR temp[512];
		UCHAR* p = temp;
  		const UCHAR* q = lock->lbl_key;
  		const UCHAR* const end = q + lock->lbl_length;
		for (; q < end; q++) {
			const UCHAR c = *q;
			if ((c >= 'a' && c <= 'z') ||
				(c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '/')
			{
				*p++ = c;
			}
			else {
				sprintf((char*) p, "<%d>", c);
				while (*p)
					p++;
			}
		}
		*p = 0;
		FPRINTF(outfile, "\tKey: %s,", temp);
	}

	FPRINTF(outfile, " Flags: 0x%02X, Pending request count: %6d\n",
			lock->lbl_flags, lock->lbl_pending_lrq_count);

	prt_que(outfile, LOCK_header, "\tHash que", &lock->lbl_lhb_hash,
			OFFSET(LBL, lbl_lhb_hash));

	prt_que(outfile, LOCK_header, "\tRequests", &lock->lbl_requests,
			OFFSET(LRQ, lrq_lbl_requests));

	const srq* que_inst;
	SRQ_LOOP(lock->lbl_requests, que_inst) {
		const lrq* request = (lrq*) ((UCHAR*) que_inst - OFFSET(LRQ, lrq_lbl_requests));
		FPRINTF(outfile,
				"\t\tRequest %6"SLONGFORMAT", Owner: %6"SLONGFORMAT
				", State: %d (%d), Flags: 0x%02X\n",
				SRQ_REL_PTR(request), request->lrq_owner, request->lrq_state,
				request->lrq_requested, request->lrq_flags);
	}

	FPRINTF(outfile, "\n");
}


static void prt_owner(OUTFILE outfile,
					  const lhb* LOCK_header,
					  const own* owner,
					  bool sw_requests,
					  bool sw_waitlist)
{
/**************************************
 *
 *      p r t _ o w n e r
 *
 **************************************
 *
 * Functional description
 *      Print a formatted owner block.
 *
 **************************************/
	FPRINTF(outfile, "OWNER BLOCK %6"SLONGFORMAT"\n", SRQ_REL_PTR(owner));
	FPRINTF(outfile, "\tOwner id: %6"ULONGFORMAT
			", type: %1d, flags: 0x%02X, pending: %6"SLONGFORMAT", semid: %6d ",
			owner->own_owner_id, owner->own_owner_type,
			(owner->own_flags | (UCHAR) owner->own_ast_flags),
			owner->own_pending_request, owner->own_semaphore & ~OWN_semavail);
	if (owner->own_semaphore & OWN_semavail)
		FPRINTF(outfile, "(available)\n");
	else
		FPRINTF(outfile, "\n");

	FPRINTF(outfile, "\tProcess id: %6d, UID: 0x%X  %s\n",
			owner->own_process_id,
			owner->own_process_uid,
			ISC_check_process_existence(owner->own_process_id,
										owner->own_process_uid,
										FALSE) ? "Alive" : "Dead");
#ifdef SOLARIS_MT
	FPRINTF(outfile, "\tLast Acquire: %6"UQUADFORMAT" (%6"UQUADFORMAT" ago)",
			owner->own_acquire_time,
			LOCK_header->lhb_acquires - owner->own_acquire_time);
#ifdef DEV_BUILD
	FPRINTF(outfile, " sec %9"ULONGFORMAT" (%3"ULONGFORMAT" sec ago)",
			owner->own_acquire_realtime,
			time(NULL) - owner->own_acquire_realtime);
#endif /* DEV_BUILD */
	FPRINTF(outfile, "\n");
#endif /* SOLARIS_MT */
	{
		const UCHAR tmp = (owner->own_flags | (UCHAR) owner->own_ast_flags
			   | (UCHAR) owner->own_ast_hung_flags);
		FPRINTF(outfile, "\tFlags: 0x%02X ", tmp);
		FPRINTF(outfile, " %s", (tmp & OWN_hung) ? "hung" : "    ");
		FPRINTF(outfile, " %s", (tmp & OWN_blocking) ? "blkg" : "    ");
		FPRINTF(outfile, " %s", (tmp & OWN_starved) ? "STRV" : "    ");
		FPRINTF(outfile, " %s", (tmp & OWN_signal) ? "sgnl" : "    ");
		FPRINTF(outfile, " %s", (tmp & OWN_wakeup) ? "wake" : "    ");
		FPRINTF(outfile, " %s", (tmp & OWN_scanned) ? "scan" : "    ");
		FPRINTF(outfile, "\n");
	}

	prt_que(outfile, LOCK_header, "\tRequests", &owner->own_requests,
			OFFSET(LRQ, lrq_own_requests));
	prt_que(outfile, LOCK_header, "\tBlocks", &owner->own_blocks,
			OFFSET(LRQ, lrq_own_blocks));

	if (sw_waitlist) {
		waitque owner_list;
		owner_list.waitque_depth = 0;
		prt_owner_wait_cycle(outfile, LOCK_header, owner, 8, &owner_list);
	}

	FPRINTF(outfile, "\n");

	if (sw_requests) {
		const srq* que_inst;
		SRQ_LOOP(owner->own_requests, que_inst)
			prt_request(outfile, LOCK_header,
						(LRQ) ((UCHAR *) que_inst -
							   OFFSET(LRQ, lrq_own_requests)));
	}
}


static void prt_owner_wait_cycle(
								 OUTFILE outfile,
								 const lhb* LOCK_header,
								 const own* owner,
								 USHORT indent, waitque *waiters)
{
/**************************************
 *
 *      p r t _ o w n e r _ w a i t _ c y c l e
 *
 **************************************
 *
 * Functional description
 *	For the given owner, print out the list of owners
 *	being waited on.  The printout is recursive, up to
 *	a limit.  It is recommended this be used with 
 *	the -c consistency mode.
 *
 **************************************/
	USHORT i;

	for (i = indent; i; i--)
		FPRINTF(outfile, " ");

/* Check to see if we're in a cycle of owners - this might be
   a deadlock, or might not, if the owners haven't processed
   their blocking queues */

	for (i = 0; i < waiters->waitque_depth; i++)
		if (SRQ_REL_PTR(owner) == waiters->waitque_entry[i]) {
			FPRINTF(outfile, "%6"SLONGFORMAT" (potential deadlock).\n",
					SRQ_REL_PTR(owner));
			return;
		}

	FPRINTF(outfile, "%6"SLONGFORMAT" waits on ", SRQ_REL_PTR(owner));

	if (!owner->own_pending_request)
		FPRINTF(outfile, "nothing.\n");
	else {
		if (waiters->waitque_depth > FB_NELEM(waiters->waitque_entry)) {
			FPRINTF(outfile, "Dependency too deep\n");
			return;
		}

		waiters->waitque_entry[waiters->waitque_depth++] = SRQ_REL_PTR(owner);

		FPRINTF(outfile, "\n");
		const lrq* owner_request = (LRQ) SRQ_ABS_PTR(owner->own_pending_request);
		fb_assert(owner_request->lrq_type == type_lrq);
		const bool owner_conversion = (owner_request->lrq_state > LCK_null);

		const lbl* lock = (LBL) SRQ_ABS_PTR(owner_request->lrq_lock);
		fb_assert(lock->lbl_type == type_lbl);

		int counter = 0;
		const srq* que_inst;
		SRQ_LOOP(lock->lbl_requests, que_inst) {

			if (counter++ > 50) {
				for (i = indent + 6; i; i--)
					FPRINTF(outfile, " ");
				FPRINTF(outfile, "printout stopped after %d owners\n",
						counter - 1);
				break;
			}

			const lrq* lock_request =
				(LRQ) ((UCHAR *) que_inst - OFFSET(LRQ, lrq_lbl_requests));
			fb_assert(lock_request->lrq_type == type_lrq);


			if (LOCK_header->lhb_flags & LHB_lock_ordering && !owner_conversion) {

				/* Requests AFTER our request can't block us */
				if (owner_request == lock_request)
					break;

				if (COMPATIBLE(owner_request->lrq_requested, MAX(lock_request->lrq_state,
								   lock_request->lrq_requested)))
				{
					continue;
				}
			}
			else {

				/* Requests AFTER our request CAN block us */
				if (lock_request == owner_request)
					continue;

				if (COMPATIBLE(owner_request->lrq_requested, lock_request->lrq_state)) 
					continue;
			}
			const own* lock_owner = (OWN) SRQ_ABS_PTR(lock_request->lrq_owner);
			prt_owner_wait_cycle(outfile, LOCK_header, lock_owner, indent + 4,
								 waiters);
		}
		waiters->waitque_depth--;
	}
}


static void prt_request(OUTFILE outfile, const lhb* LOCK_header, const lrq* request)
{
/**************************************
 *
 *      p r t _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *      Print a format request block.
 *
 **************************************/

	FPRINTF(outfile, "REQUEST BLOCK %6"SLONGFORMAT"\n", SRQ_REL_PTR(request));
	FPRINTF(outfile, "\tOwner: %6"SLONGFORMAT", Lock: %6"SLONGFORMAT
			", State: %d, Mode: %d, Flags: 0x%02X\n",
			request->lrq_owner, request->lrq_lock, request->lrq_state,
			request->lrq_requested, request->lrq_flags);
	FPRINTF(outfile, "\tAST: 0x%p, argument: 0x%p\n",
			request->lrq_ast_routine, request->lrq_ast_argument);
	prt_que2(outfile, LOCK_header, "\tlrq_own_requests",
			 &request->lrq_own_requests, OFFSET(LRQ, lrq_own_requests));
	prt_que2(outfile, LOCK_header, "\tlrq_lbl_requests",
			 &request->lrq_lbl_requests, OFFSET(LRQ, lrq_lbl_requests));
	prt_que2(outfile, LOCK_header, "\tlrq_own_blocks  ",
			 &request->lrq_own_blocks, OFFSET(LRQ, lrq_own_blocks));
	FPRINTF(outfile, "\n");
}


static void prt_que(
					OUTFILE outfile,
					const lhb* LOCK_header,
					const SCHAR* string, const srq* que_inst, USHORT que_offset)
{
/**************************************
 *
 *      p r t _ q u e
 *
 **************************************
 *
 * Functional description
 *      Print the contents of a self-relative que.
 *
 **************************************/
	const SLONG offset = SRQ_REL_PTR(que_inst);

	if (offset == que_inst->srq_forward && offset == que_inst->srq_backward) {
		FPRINTF(outfile, "%s: *empty*\n", string);
		return;
	}

	SLONG count = 0;
	const srq* next;
	SRQ_LOOP((*que_inst), next)
		++count;

	FPRINTF(outfile, "%s (%ld):\tforward: %6"SLONGFORMAT
			", backward: %6"SLONGFORMAT"\n", string, count,
			que_inst->srq_forward - que_offset, que_inst->srq_backward - que_offset);
}


static void prt_que2(
					 OUTFILE outfile,
					 const lhb* LOCK_header,
					 const SCHAR* string, const srq* que_inst, USHORT que_offset)
{
/**************************************
 *
 *      p r t _ q u e 2
 *
 **************************************
 *
 * Functional description
 *      Print the contents of a self-relative que.
 *      But don't try to count the entries, as they might be invalid
 *
 **************************************/
	const SLONG offset = SRQ_REL_PTR(que_inst);

	if (offset == que_inst->srq_forward && offset == que_inst->srq_backward) {
		FPRINTF(outfile, "%s: *empty*\n", string);
		return;
	}

	FPRINTF(outfile, "%s:\tforward: %6"SLONGFORMAT
			", backward: %6"SLONGFORMAT"\n", string,
			que_inst->srq_forward - que_offset, que_inst->srq_backward - que_offset);
}

