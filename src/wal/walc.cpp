/*
 *        PROGRAM:        JRD Write Ahead Log Coordinator
 *        MODULE:         walc.c
 *        DESCRIPTION:    Write Ahead Log low level routines
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
 */

#include "firebird.h"
#include <string.h>
#include "../jrd/jrd_time.h"
#include "../jrd/isc.h"
#include "../jrd/file_params.h"
#include "../wal/wal.h"
#include "../jrd/jrn.h"
#include "../jrd/flags.h"
#include "../jrd/isc_signal.h"
#include "gen/codes.h"
#include "../jrd/llio.h"
#include "../wal/walc_proto.h"
#include "../wal/walf_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/iberr_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/os/isc_i_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/llio_proto.h"

#ifdef UNIX
#include <sys/types.h>
#include <sys/stat.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef WIN_NT
#include <process.h>
#include <windows.h>
#ifdef TEXT
#undef TEXT
#endif
#define TEXT		SCHAR

#define WAL_MUTEX	WAL_handle->wal_mutex
#endif

#ifdef SUPERSERVER
#ifdef WIN_NT
#define getpid		GetCurrentThreadId
#endif
#endif

#ifndef WAL_MUTEX
#define WAL_MUTEX	WAL_segment->wals_mutex
#endif

/* Define a structure to hold WAL configuration variables */

typedef struct walc {
	TEXT *walc_dbname;
	USHORT walc_db_page_len;
	TEXT *walc_logname;
	SLONG walc_log_partition_offset;
	bool walc_first_time_log;
	SLONG walc_new_log_seqno;	// used when first_time_log is true

	SSHORT walc_maxbufs;
	USHORT walc_bufsize;
	USHORT walc_blksize;
	USHORT walc_blks_offset;
	int walc_segment_length;

	SLONG walc_max_ckpt_intrvl;

	LGFILE *walc_log_serial_file_info;
	SSHORT walc_log_names_count;
	LGFILE **walc_log_rr_files_info;
	LGFILE *walc_log_ovflow_file_info;
	USHORT walc_log_names_buf_len;
	USHORT walc_log_names_offset;
	USHORT walc_logf_size;
	USHORT walc_logf_offset;

	TEXT *walc_jrn_dirname;
	USHORT walc_jrn_data_len;
	UCHAR *walc_jrn_data;

	SLONG walc_grpc_wait_usecs;

	struct wal *walc_wal;
	TEXT *walc_mapfile;
} *WALC;

static SSHORT check_base_name(ISC_STATUS *, TEXT *);
static void cleanup(void *);
static void init_group_commit_blocks(GRP_COMMIT *);
static SSHORT setup_wal_params(ISC_STATUS *, TEXT *, USHORT, WALC, SSHORT,
							   UCHAR *);
static void wals_initialize(WALC, SH_MEM, int);

/* these statics define a round-robin data area for storing
   textual error messages returned to the user */

static TEXT *wal_failures = NULL, *wal_failures_ptr;

#define WAL_FAILURE_SPACE	2048

#define statistics	stat

#ifdef SHLIB_DEFS
#define statistics	(*_libgds_stat)

extern int statistics();
#endif


void WALC_acquire( WAL WAL_handle, WALS * address)
{
/**************************************
 *
 *	W A L C _ a c q u i r e
 *
 **************************************
 *
 * Functional description
 *	Acquire the WAL segment.  If it's busy, wait for it.
 *
 **************************************/
	WALS WAL_segment;
	SLONG length;
	ISC_STATUS_ARRAY local_status;

	*address = WAL_segment = WAL_handle->wal_segment;
	ISC_inhibit();
	if (ISC_mutex_lock(WAL_MUTEX))
		WALC_bug(NULL, WAL_handle->wal_dbname, "WAL mutex lock failed");

	if (WAL_handle->wal_shmem_data.sh_mem_length_mapped <
		WAL_segment->wals_length) {
		/* Somebody extended the WAL segment since we last accessed it. */

		length = WAL_segment->wals_length;
		WAL_segment = (WALS) ISC_remap_file(local_status,
											&WAL_handle->wal_shmem_data,
											length, TRUE);
		if (WAL_segment == NULL) {
			WALC_release(WAL_handle);
			WALC_bug(NULL, WAL_handle->wal_dbname, "WAL remap failed");
		}
		WAL_handle->wal_segment = *address = WAL_segment;
		WAL_handle->wal_length = length;
	}

	WAL_segment->wals_last_pid = WAL_handle->wal_pid;
	WAL_segment->wals_acquire_count++;
}


void WALC_alarm_handler(void* _event)
{
/**************************************
 *
 *	W A L C _ a l a r m _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	This handler would just poke the semaphore WAL_GCOMMIT_STALL_SEM.
 *	This is to take care of the situation when an alarm handler
 *	expires before the wait on the samaphore starts.  This way, we can
 *	avoid an indefinite wait when there is nobody to poke the
 *	semaphore and the timer also expired becuase of some asynchronous
 *	event (signal) handling.
 *
 **************************************/

	ISC_event_post(reinterpret_cast<EVENT>(_event));
}


SSHORT WALC_bug( ISC_STATUS * status_vector, TEXT * dbname, TEXT * string)
{
/**************************************
 *
 *	W A L C _ b u g
 *
 **************************************
 *
 * Functional description
 *	Disasterous WAL bug.  Issue message and abort process.
 *
 **************************************/
	ISC_STATUS_ARRAY local_status;

	IBERR_build_status(local_status, gds_wal_bugcheck,
					   gds_arg_string, dbname,
					   gds_arg_number, (SLONG) getpid(),
					   gds_arg_string, string, 0);
	gds__log_status(dbname, local_status);
	gds__print_status(local_status);

	if (status_vector) {
		IBERR_build_status(status_vector, gds_bug_check,
						   gds_arg_string, string, 0);
		return FB_FAILURE;
	}

	abort();
}


void WALC_build_dbg_filename( TEXT * dbname, TEXT * dbg_filename)
{
/**************************************
 *
 *      W A L C _ b u i l d _ d b g _ f i l e n a m e
 *
 **************************************
 *
 * Functional description
 *      Generate the debug file name.
 *
 **************************************/

	strcpy(dbg_filename, dbname);
	strcat(dbg_filename, ".ww.dbg");
}


void WALC_build_logname( TEXT * logname, TEXT * basename, SLONG seqno)
{
/**************************************
 *
 *	W A L C _ b u i l d _ l o g n a m e
 *
 **************************************
 *
 * Functional description
 *	Generate a log file name.  Try to keep the file extension
 *	to three characters.
 *
 **************************************/
	static TEXT letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	TEXT extension[11];

	if (seqno < 1000 || seqno > 18575)
		sprintf(extension, "%"SLONGFORMAT, seqno);
	else {
		seqno -= 1000;
		extension[0] = letters[seqno % 26];
		extension[1] = letters[seqno / 26 % 26];
		extension[2] = letters[seqno / 26 / 26];
		extension[3] = 0;
	}

	sprintf(logname, "%s%s.%s", basename, WAL_FNAME_SUBSCRIPT, extension);
}


SSHORT WALC_check_writer(WAL WAL_handle)
{
/**************************************
 *
 *	W A L C _ c h e c k _ w r i t e r 
 *
 **************************************
 *
 * Functional description
 *	Find WAL writer process (thread).  If it is not there, return 
 *	FB_FAILURE, else return FB_SUCCESS.
 *
 **************************************/
	WALS WAL_segment;
	SLONG writer_pid;

/* Look for the WAL writer */

	WALC_acquire(WAL_handle, &WAL_segment);
	writer_pid = WAL_segment->wals_writer_pid;
	WALC_release(WAL_handle);

	if (!writer_pid)
		return FB_FAILURE;

	return (ISC_check_process_existence(writer_pid, 0, FALSE) ? FB_SUCCESS :
			FB_FAILURE);
}


void WALC_fini( ISC_STATUS * status_vector, WAL * WAL_handle)
{
/**************************************
 *
 *	W A L C _ f i n i
 *
 **************************************
 *
 * Functional description
 *	Detach the WAL segment.  If this process (thread) did a 
 *	WAL shutdown earlier, get rid of the WAL segment and 
 *	the corresponding semaphores.
 *
 **************************************/
	int flag;
	TEXT wal_file[MAXPATHLEN];

	WAL wal = *WAL_handle;
	if ((wal == NULL) || ((--wal->wal_count) > 0))
		return;

	wal->wal_count = 0;
	WALS WAL_segment = wal->wal_segment;

	if (WAL_segment) {
		WAL_segment->wals_num_attaches--;
		flag = (wal->wal_flags & WAL_SHUTDOWN_HANDLE) ? ISC_SEM_REMOVE : 0;
#ifdef HAVE_MMAP
		if (wal->wal_flags & WAL_CLEANUP_HANDLE)
			flag |= ISC_MEM_REMOVE;
#else
		if (wal->wal_flags & WAL_SHUTDOWN_HANDLE)
			flag |= ISC_MEM_REMOVE;
#endif
		ISC_unmap_file(status_vector, &wal->wal_shmem_data, flag);
		if (wal->wal_flags & WAL_CLEANUP_HANDLE) {
			WAL_BUILD_MAPFNAME(wal_file, wal->wal_dbname);
			unlink(wal_file);
			WALC_build_dbg_filename(wal->wal_dbname, wal_file);
			unlink(wal_file);
		}
	}

	gds__free((SLONG *) * WAL_handle);
	*WAL_handle = NULL;
}


SSHORT WALC_init(ISC_STATUS * status_vector,
				 WAL * WAL_handle,
				 TEXT * dbname,
				 USHORT db_page_len,
				 TEXT * logname,
				 SLONG log_partition_offset,
				 bool first_time_log,
				 SLONG new_log_seqno,
				 SSHORT wpb_length,
				 UCHAR * wpb,
				 bool first_attach)
{
/**************************************
 *
 *	W A L C _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize Write Ahead Log segment for the database.
 *	Initialize WAL_handle for the process (thread).
 *
 *	If first_time_log is true then use the new_log_seqno
 *	as the starting sequence number for the set of new log
 *	files.
 *
 *	Return FB_SUCCESS if initialization succeeds else return FB_FAILURE.
 *
 **************************************/
	TEXT wal_mapfile[MAXPATHLEN];
	struct walc wal_args;
	SLONG length;
	WALS WAL_segment;
	void (*wal_init_routine) ();

	if (*WAL_handle != NULL) {
		/* We are already initialized.  Just increment the use count. */

		(*WAL_handle)->wal_count++;
		return FB_SUCCESS;
	}

/* Construct a wal filename of the form  "<database_name>.walfile" */

	WAL_BUILD_MAPFNAME(wal_mapfile, dbname);

	wal_args.walc_dbname = dbname;
	wal_args.walc_db_page_len = db_page_len;
	wal_args.walc_logname = logname;
	wal_args.walc_log_partition_offset = log_partition_offset;
	wal_args.walc_first_time_log = first_time_log;
	wal_args.walc_new_log_seqno = new_log_seqno;
	wal_args.walc_mapfile = wal_mapfile;

	if (first_attach) {
		/* Now parse WAL paramater block (wpb) */

		if (setup_wal_params(status_vector, dbname, db_page_len, &wal_args,
							 wpb_length, wpb) != FB_SUCCESS) {
			WALC_save_status_strings(status_vector);
			return FB_FAILURE;
		}
		length = wal_args.walc_segment_length;
		wal_init_routine = reinterpret_cast < void (*) () > (wals_initialize);
	}
	else {
		length = 0;
		wal_init_routine = NULL;
	}

	WAL wal = wal_args.walc_wal = (WAL) gds__alloc(sizeof(struct wal));
/* NOMEM: return error status, FREE: error returns & WAL_fini() */
	if (!wal) {
		status_vector[0] = gds_arg_gds;
		status_vector[1] = gds_virmemexh;
		status_vector[2] = gds_arg_end;
		return FB_FAILURE;
	}

#if (defined UNIX)
	wal->wal_shmem_data.sh_mem_semaphores = MAX_WALSEMS;
#endif
	if ((WAL_segment = (WALS) ISC_map_file(status_vector, wal_mapfile,
										   (void(*)(void *, sh_mem *, int))wal_init_routine,
										   (void *) &wal_args, length,
										   &wal->wal_shmem_data)) == NULL) {
		WAL_ERROR_APPEND(status_vector, gds_wal_illegal_attach, dbname);
		WALC_save_status_strings(status_vector);
		gds__free((SLONG *) wal);
		return FB_FAILURE;
	}

	if (WAL_segment->wals_version != WALS_VERSION) {
		WALC_bug(status_vector, dbname,
				 "Inconsistent WAL segment version number");
		WALC_save_status_strings(status_vector);
		gds__free((SLONG *) wal);
		return FB_FAILURE;
	}

#if (defined WIN_NT)
/* For Windows/NT and OS/2, mutex_init needs to be done for every
   process; for UNIX, it happens once in wals_initialize(). */

	if (ISC_mutex_init(wal->wal_mutex, wal_mapfile))
		WALC_bug(NULL, dbname, "WAL mutex init failed");

	if (!wal_init_routine) {
		/* If there wasn't an initialize routine then events haven't
		   been initialized. */
		EVENT event, shared_event;
		SSHORT i;

		event = wal->wal_events + 1;
		shared_event = WAL_segment->wals_events + 1;
		for (i = 1; i < MAX_WALSEMS; event++, shared_event++, i++)
			ISC_event_init_shared(event, WAL_SIGNALS + i, wal_mapfile,
								  shared_event, FALSE);
	}
#endif

	WAL_segment->wals_num_attaches++;

	wal->wal_count = 1;
	wal->wal_pid = getpid();
	wal->wal_id = (SLONG) wal->wal_shmem_data.sh_mem_handle;
	wal->wal_grpc_wait_id = -1;
	wal->wal_flags = 0;
	wal->wal_local_info_ptr = NULL;
	wal->wal_segment = WAL_segment;
	strcpy(wal->wal_dbname, dbname);

	*WAL_handle = wal;

	return FB_SUCCESS;
}


void WALC_release( WAL WAL_handle)
{
/**************************************
 *
 *	W A L C _ r e l e a s e
 *
 **************************************
 *
 * Functional description
 *	Release the mapped WAL lock file. 
 *
 **************************************/
	WALS WAL_segment = WAL_handle->wal_segment;
	WAL_segment->wals_last_pid = 0;
	if (ISC_mutex_unlock(WAL_MUTEX)) {
		WALC_bug(NULL, WAL_handle->wal_dbname, "WAL mutex unlock failed");
	}
	ISC_enable();
}


void WALC_save_status_strings( ISC_STATUS * vector)
{
/**************************************
 *
 *	W A L C _ s a v e _ s t a t u s _ s t r i n g s
 *
 **************************************
 *
 * Functional description
 *	Strings in status vectors may be stored in stack variables
 *	or memory pools that are transient.  To perserve the information,
 *	copy any included strings to a special buffer.
 *
 **************************************/
	TEXT *p;
	ISC_STATUS status;
	USHORT l;

/* allocate space for failure strings if it hasn't already been allocated */

	if (!wal_failures) {
		wal_failures_ptr = wal_failures =
			(TEXT *) gds__alloc((SLONG) WAL_FAILURE_SPACE);
		/* NOMEM: don't try to copy the status strings, FREE: by cleanup() handler */
		if (!wal_failures)
			return;
		gds__register_cleanup(cleanup, 0);
	}

	while (*vector) {
		switch (status = *vector++) {
		case gds_arg_cstring:
			l = *vector++;

		case gds_arg_interpreted:
		case gds_arg_string:
			p = (TEXT *) * vector;
			if (status != gds_arg_cstring)
				l = strlen(p) + 1;

			/* If there isn't any more room in the buffer,
			   start at the beginning again */

			if (wal_failures_ptr + l > wal_failures + WAL_FAILURE_SPACE)
				wal_failures_ptr = wal_failures;
			*vector++ = (ISC_STATUS) wal_failures_ptr;
			while (l--)
				*wal_failures_ptr++ = *p++;
			break;

		default:
			++vector;
			break;
		}
	}
}


void WALC_setup_buffer_block(WALS WAL_segment,
							 WALBLK * wblk,
							 bool ckpt)
{
/**************************************
 *
 *       W A L C _ s e t u p _ b u f f e r _ b l o c k
 *
 **************************************
 *
 * Functional description
 *      Mark the passed buffer as ready to be written.
 *      If 'ckpt' flag is true then this buffer finishes a checkpoint.
 *      Assumes that acquire() has been done, before calling this routine.
 *
 ***************************************/
	USHORT padding_bytes;

/* Add extra bytes for the trailing length word.  The block header
   and trailing block length will be initialized by the WAL writer.
   Add padding bytes to round-up the block size to the next block
   boundry, if applicable. */

	wblk->walblk_roundup_offset = wblk->walblk_cur_offset + BLK_TAILOVHD;
	if (WAL_segment->wals_roundup_size) {
		padding_bytes = WAL_segment->wals_roundup_size -
			((int) wblk->walblk_roundup_offset %
			 (int) WAL_segment->wals_roundup_size);

		wblk->walblk_roundup_offset +=
			(USHORT) ((int) padding_bytes %
					  (int) WAL_segment->wals_roundup_size);
	}

	WAL_segment->wals_buf_offset +=
		(wblk->walblk_roundup_offset - wblk->walblk_cur_offset);
	wblk->walblk_flags |= WALBLK_to_be_written;
	if (ckpt)
		wblk->walblk_flags |= WALBLK_checkpoint;
}


static SSHORT check_base_name( ISC_STATUS * status_vector, TEXT * base_name)
{
/**************************************
 *
 *	c h e c k _ b a s e _ n a m e
 *
 **************************************
 *
 * Functional description
 *	Try to create or open a file with the 
 *	given base name.  Return FB_SUCCESS or FB_FAILURE.
 *
 **************************************/
	TEXT dummy_fname[MAXPATHLEN];
	SLONG fd;

/* First create a dummy log name with the given base name */

	WALC_build_logname(dummy_fname, base_name, (SLONG) 1);

/* Try to create a new file.  Delete it if we're successful. */

	if (LLIO_open(status_vector, dummy_fname, LLIO_OPEN_NEW_RW, TRUE, &fd) ==
		FB_SUCCESS) {
		LLIO_close(status_vector, fd);
		unlink(dummy_fname);
		return FB_SUCCESS;
	}

/* Try to open an existing file */

	if (LLIO_open
		(status_vector, dummy_fname, LLIO_OPEN_EXISTING_RW, TRUE,
		 &fd) == FB_SUCCESS) {
		LLIO_close(status_vector, fd);
		return FB_SUCCESS;
	}

	return FB_FAILURE;
}


static void cleanup( void *arg)
{
/**************************************
 *
 *	c l e a n u p
 *
 **************************************
 *
 * Functional description
 *	Exit handler to cleanup dynamically allocated memory.
 *
 **************************************/

	if (wal_failures)
		gds__free((SLONG *) wal_failures);

	wal_failures = NULL;
}


static void init_group_commit_blocks( GRP_COMMIT * grpc_blks)
{
/**************************************
 *
 *	i n i t _ g r o u p _ c o m m i t _ b l o c k s
 *
 **************************************
 *
 * Functional description
 *	Initialize the group commit blocks of the WAL segment.
 *
 **************************************/
	USHORT i;
	int snum;

	for (i = 0, snum = WAL_GCOMMIT_WAIT1_SEM; i < MAX_GRP_COMMITTERS;
		 i++, snum++) {
		grpc_blks[i].grp_commit_coordinator = 0L;
		grpc_blks[i].grp_commit_size = 0;
		grpc_blks[i].grp_commit_event_num = snum;
	}
}


static SSHORT setup_wal_params(
							   ISC_STATUS * status_vector,
							   TEXT * dbname,
							   USHORT db_page_len,
							   WALC wal_args, SSHORT wpb_length, UCHAR * wpb)
 {
/**************************************
 *
 *	s e t u p _ w a l _ p a r a m s
 *
 **************************************
 *
 * Functional description
 *	Parse the WAL parameter block and setup the WAL 
 *	attributes like size and number of WAL buffers.
 *
 **************************************/
	UCHAR *p, *q;
	int i, j;
	LGFILE *log_file;
	TEXT err_buffer[16];

/* First setup the default values */

	wal_args->walc_maxbufs = MIN_WALBLKS;
	wal_args->walc_bufsize =
		(USHORT) MIN(MAX_WALBUFLEN, 4 * (int) db_page_len);

	wal_args->walc_max_ckpt_intrvl = DEFAULT_CKPT_INTRVL * OneK;

	wal_args->walc_log_names_count = 0;
	wal_args->walc_log_serial_file_info = NULL;
	wal_args->walc_log_rr_files_info = NULL;
	wal_args->walc_log_ovflow_file_info = NULL;
	wal_args->walc_log_names_buf_len = 0;

	wal_args->walc_jrn_dirname = NULL;
	wal_args->walc_jrn_data_len = 0;

	wal_args->walc_grpc_wait_usecs = GRPC_WAIT_USECS;

	SSHORT buf_count = 0;
	USHORT buf_size = 0;
	SLONG ckpt_intrvl = 0L;


/* Now see if wal parameter block specifies any parameters */

	bool done = false;
	if ((p = wpb) == NULL)
		done = true; // Nothing to parse from the wal paramater block

	q = p + wpb_length;
	while (!done && (p < q)) {
		switch (*p++) {
		case WAL_PARAM_BUF_COUNT:
			memcpy((SCHAR *) & buf_count, p, sizeof(USHORT));
			p += sizeof(USHORT);
			break;

		case WAL_PARAM_BUF_LEN:
			memcpy((SCHAR *) & buf_size, p, sizeof(USHORT));
			p += sizeof(USHORT);
			break;

		case WAL_PARAM_CKPT_INTRVL:
			memcpy((SCHAR *) & ckpt_intrvl, p, sizeof(SLONG));
			p += sizeof(SLONG);
			break;

		case WAL_PARAM_SERIAL_LOG:
			memcpy((UCHAR *) & wal_args->walc_log_serial_file_info, p,
				   sizeof(UCHAR *));
			p += sizeof(UCHAR *);
			break;

		case WAL_PARAM_RR_LOGS_COUNT:
			memcpy((SCHAR *) & wal_args->walc_log_names_count, p,
				   sizeof(USHORT));
			p += sizeof(USHORT);
			break;

		case WAL_PARAM_RR_LOGS:
			memcpy((UCHAR *) & wal_args->walc_log_rr_files_info, p,
				   sizeof(UCHAR *));
			p += sizeof(UCHAR *);
			break;

		case WAL_PARAM_OVFLOW_LOG:
			memcpy((UCHAR *) & wal_args->walc_log_ovflow_file_info, p,
				   sizeof(UCHAR *));
			p += sizeof(UCHAR *);
			break;

		case WAL_PARAM_GRPC_WAIT_USECS:
			memcpy((SCHAR *) & wal_args->walc_grpc_wait_usecs, p,
				   sizeof(wal_args->walc_grpc_wait_usecs));
			p += sizeof(wal_args->walc_grpc_wait_usecs);
			break;

		case WAL_PARAM_JRN_DIRNAME:
			wal_args->walc_jrn_dirname = (TEXT *) p;
			p += (strlen(wal_args->walc_jrn_dirname) + 1);
			break;

		case WAL_PARAM_JRN_DATA_LEN:
			memcpy((SCHAR *) & wal_args->walc_jrn_data_len, p,
				   sizeof(USHORT));
			p += sizeof(USHORT);
			break;

		case WAL_PARAM_JRN_DATA:
			memcpy((SCHAR *) & wal_args->walc_jrn_data_len, p,
				   sizeof(USHORT));
			p += sizeof(USHORT);
			wal_args->walc_jrn_data = p;
			p += wal_args->walc_jrn_data_len;
			break;

		case WAL_PARAM_END:
			done = true;
			break;

		default:
			sprintf(err_buffer, "%d", (int) *(p - 1));
			WAL_ERROR(status_vector, gds_wal_invalid_wpb, err_buffer);
			WALC_save_status_strings(status_vector);
			return FB_FAILURE;
		}
	}

/* Now update the default values by the supplied parameter values, if any. */

	if (buf_count)
		wal_args->walc_maxbufs =
			MIN(MAX_WALBLKS, MAX(MIN_WALBLKS, buf_count));
	if (buf_size)
		/* Should be adjusted for roundup in case of using raw device -- Damodar */
		wal_args->walc_bufsize =
			(USHORT) MIN(MAX_WALBUFLEN,
						 MAX(wal_args->walc_bufsize, buf_size));

	if (ckpt_intrvl)
		wal_args->walc_max_ckpt_intrvl =
			MAX(ckpt_intrvl * OneK, MIN_CKPT_INTRVL * OneK);

/* Process serial log specification.  */
	if (log_file = wal_args->walc_log_serial_file_info) {
		/* Make sure that base path name is valid */

		if (wal_args->walc_first_time_log &&
			(check_base_name(status_vector, log_file->lg_name) != FB_SUCCESS)) {
			WALC_save_status_strings(status_vector);
			return FB_FAILURE;
		}

		wal_args->walc_log_names_buf_len += strlen(log_file->lg_name) + 1;

		/* User specifies log size in units of 1K bytes. */

		log_file->lg_size =
			MAX(log_file->lg_size * OneK, MIN_LOG_LENGTH * OneK);
	}

/* Process overflow log specification. */

	if (log_file = wal_args->walc_log_ovflow_file_info) {
		/* Make sure that base path name is valid */

		if (wal_args->walc_first_time_log &&
			(check_base_name(status_vector, log_file->lg_name) != FB_SUCCESS)) {
			WALC_save_status_strings(status_vector);
			return FB_FAILURE;
		}

		wal_args->walc_log_names_buf_len += strlen(log_file->lg_name) + 1;

		/* User specifies log size in units of 1K bytes. */

		log_file->lg_size =
			MAX(log_file->lg_size * OneK, MIN_LOG_LENGTH * OneK);
	}

/* Process round-robin logs specifications. */

	if (wal_args->walc_log_names_count && wal_args->walc_log_rr_files_info) {
		/* First make sure that an overflow log file specification is provided */
		if (!wal_args->walc_log_ovflow_file_info) {
			IBERR_build_status(status_vector, gds_wal_ovflow_log_required,
							   0);
			return FB_FAILURE;
		}

		for (i = 0; i < wal_args->walc_log_names_count; i++) {
			log_file = wal_args->walc_log_rr_files_info[i];
			wal_args->walc_log_names_buf_len += strlen(log_file->lg_name) + 1;

			/* User specifies log size in units of 1K bytes. */

			log_file->lg_size =
				MAX(log_file->lg_size * OneK, MIN_LOG_LENGTH * OneK);
			if (wal_args->walc_first_time_log) {
				/* Pre-allocate a non-raw log file */
				if (log_file->lg_flags & LOG_raw)
					continue;
				if (WALF_init_p_log(status_vector, dbname, log_file->lg_name,
									log_file->lg_size,
									log_file->lg_partitions) != FB_SUCCESS) {
					WALC_save_status_strings(status_vector);

					/* Remove the allocated files so far */

					for (j = 0; j < i; j++) {
						log_file = wal_args->walc_log_rr_files_info[j];
						if (!(log_file->lg_flags & LOG_raw))
							unlink(log_file->lg_name);
					}
					return FB_FAILURE;
				}
			}
		}
	}

	wal_args->walc_log_names_offset = ROUNDUP_LONG(sizeof(struct wals));

	wal_args->walc_logf_size = ROUNDUP_LONG(sizeof(struct logf));
	wal_args->walc_logf_offset = wal_args->walc_log_names_offset +
		ROUNDUP_LONG(wal_args->walc_log_names_buf_len);

	wal_args->walc_blksize = ROUNDUP_LONG(sizeof(WALBLK) +
										  wal_args->walc_bufsize);
	wal_args->walc_blks_offset = wal_args->walc_logf_offset +
		(wal_args->walc_logf_size * wal_args->walc_log_names_count);

	wal_args->walc_segment_length = wal_args->walc_blks_offset +
		wal_args->walc_maxbufs * wal_args->walc_blksize;

	return FB_SUCCESS;
}


static void wals_initialize( WALC wal_args, SH_MEM shmem_data, int initialize)
{
/**************************************
 *
 *	w a l s _ i n i t i a l i z e
 *
 **************************************
 *
 * Functional description
 *	Initialize the WAL segment.  The caller is assumed
 *	to have an exclusive lock on the WAL file.
 *
 **************************************/
	SSHORT i;
	UCHAR *p, *q;
#ifdef UNIX
	struct stat buf;
#endif

	WALS WAL_segment = (WALS) shmem_data->sh_mem_address;

#if (defined WIN_NT)
	EVENT event = wal_args->walc_wal->wal_events + 1;
	EVENT shared_event = WAL_segment->wals_events + 1;
	for (i = 1; i < MAX_WALSEMS; event++, shared_event++, i++)
		ISC_event_init_shared(event, WAL_SIGNALS + i, wal_args->walc_mapfile,
							  shared_event, (initialize) ? TRUE : FALSE);
#endif

	if (!initialize)
		return;

	if (shmem_data->sh_mem_length_mapped < wal_args->walc_segment_length)
		WALC_bug(NULL, wal_args->walc_dbname,
				 "Insufficient WAL segment length");

	WAL_segment->wals_version = WALS_VERSION;
	WAL_segment->wals_dbpage_size = wal_args->walc_db_page_len;
	WAL_segment->wals_length = shmem_data->sh_mem_length_mapped;
	WAL_segment->wals_id = time(NULL);	/* Time when this structure was initialized */

#ifdef UNIX
	if (statistics(wal_args->walc_dbname, &buf) == -1)
		WALC_bug(NULL, wal_args->walc_dbname, "stat() call failed");
	WAL_segment->wals_owner_id = buf.st_uid;
	WAL_segment->wals_group_id = buf.st_gid;
#else
	WAL_segment->wals_owner_id = 0;
	WAL_segment->wals_group_id = 0;
#endif

	WAL_segment->wals_writer_pid = 0;

#if !(defined WIN_NT)
	if (ISC_mutex_init(WAL_MUTEX, shmem_data->sh_mem_mutex_arg))
		WALC_bug(NULL, wal_args->walc_dbname, "WAL mutex init failed");
#endif

	WAL_segment->wals_last_pid = getpid();
	WAL_segment->wals_num_attaches = 0;
	WAL_segment->wals_num_detaches = 0;
	WAL_segment->wals_last_err = 0;
	WAL_segment->wals_last_bug = 0;

	strcpy(WAL_segment->wals_dbname, wal_args->walc_dbname);
	WAL_segment->wals_dbpage_size = wal_args->walc_db_page_len;
	strcpy(WAL_segment->wals_logname, wal_args->walc_logname);
	WAL_segment->wals_cur_log_base_offset = 0L;
	WAL_segment->wals_log_partition_offset =
		wal_args->walc_log_partition_offset;
	WAL_segment->wals_max_log_length = DEFAULT_LOG_LENGTH * OneK;
	WAL_segment->wals_rollover_threshold = WAL_segment->wals_max_log_length;
	WAL_segment->wals_log_seqno = wal_args->walc_new_log_seqno - 1;
	/* will be properly used and inititalized by the WAL writer */

/* Now setup the checkpointing info */

	WAL_segment->wals_max_ckpt_intrvl = wal_args->walc_max_ckpt_intrvl;
	WAL_segment->wals_thrshold_intrvl = 
		static_cast<SLONG>(WAL_segment->wals_max_ckpt_intrvl * 0.8);
	WAL_segment->wals_cur_ckpt_intrvl = 0L;
	WAL_segment->wals_ckpted_log_seqno = 0L;
	WAL_segment->wals_ckpt_log_p_offset = 0L;
	strcpy(WAL_segment->wals_ckpt_logname, "");
	WAL_segment->wals_ckpted_offset = 0L;
	WAL_segment->wals_saved_ckpted_offset = 0L;

	WAL_segment->wals_buf_waiters = 0;
	WAL_segment->wals_buf_waiting_count = 0;
	WAL_segment->wals_buf_offset = WAL_segment->wals_flushed_offset = 0L;
	WAL_segment->wals_flushed_log_seqno = 0L;

/* Now setup group-commit info */

	WAL_segment->wals_grpc_wait_id = 1;
	WAL_segment->wals_grpc_wait_usecs = wal_args->walc_grpc_wait_usecs;
	WAL_segment->wals_cur_grpc_blknum = 0;
	WAL_segment->wals_dummy1 = 0;
#if !(defined WIN_NT)
	for (i = 1; i < MAX_WALSEMS; i++)
		ISC_event_init(&WAL_segment->wals_events[i],
					   shmem_data->sh_mem_mutex_arg, i);
#endif
	init_group_commit_blocks(WAL_segment->wals_grpc_blks);
	WAL_segment->wals_grpc_count = 0;

	WAL_segment->wals_max_logfiles = wal_args->walc_log_names_count;
	WAL_segment->wals_cur_logfile = -1;
	q = (UCHAR *) WAL_segment + wal_args->walc_log_names_offset;
	WAL_segment->wals_logf_offset = wal_args->walc_logf_offset;
	WAL_segment->wals_logf_size = wal_args->walc_logf_size;

/* Now setup the serial log file info */

	LOGF* logf = &WAL_segment->wals_log_serial_file_info;
	LGFILE* log_file = wal_args->walc_log_serial_file_info;
	if (log_file) {
		p = (UCHAR *) log_file->lg_name;
		strcpy(reinterpret_cast < char *>(q),
			   reinterpret_cast < const char *>(p));
		logf->logf_name_offset = q - (UCHAR *) WAL_segment;
		logf->logf_max_size = log_file->lg_size;
		logf->logf_roundup_size = 0;
		logf->logf_flags = 0;
		logf->logf_base_offset = 0L;
		logf->logf_fname_seqno = 0L;
		if (log_file->lg_flags & LOG_raw) {
			logf->logf_roundup_size = 512;
			logf->logf_flags |= LOGF_RAW;
		}
		q += strlen(reinterpret_cast < const char *>(p)) + 1;
	}
	else {
		/* Set some default values */

		logf->logf_name_offset = 0;
		logf->logf_max_size = WAL_segment->wals_max_log_length;
		logf->logf_roundup_size = 0;
		logf->logf_flags = 0;
		logf->logf_base_offset = 0L;
		logf->logf_fname_seqno = 0L;
	}

/* Now setup the pre-allocated round-robin log files info */

	if (wal_args->walc_log_names_count) {
		for (i = 0; i < wal_args->walc_log_names_count; i++) {
			log_file = wal_args->walc_log_rr_files_info[i];
			p = (UCHAR *) log_file->lg_name;
			strcpy(reinterpret_cast < char *>(q),
				   reinterpret_cast < const char *>(p));
			logf = LOGF_INFO(i);
			logf->logf_name_offset = q - (UCHAR *) WAL_segment;
			logf->logf_partitions = log_file->lg_partitions;
			logf->logf_max_size = log_file->lg_size;
			logf->logf_roundup_size = 0;
			logf->logf_flags = 0;
			logf->logf_base_offset = 0L;
			logf->logf_fname_seqno = 0L;
			if (log_file->lg_partitions > 1) {
				logf->logf_max_size = PARTITION_SIZE(logf->logf_max_size,
													 log_file->lg_partitions);
				logf->logf_flags |= LOGF_PARTITIONED;
			}
			if (log_file->lg_flags & LOG_raw) {
				logf->logf_roundup_size = 512;
				logf->logf_flags |= LOGF_RAW;
				logf->logf_max_size = (logf->logf_max_size & ~((SLONG) 511));
			}
			q += strlen(reinterpret_cast < const char *>(p)) + 1;
		}
	}

/* Now setup the overflow log file info */

	logf = &WAL_segment->wals_log_ovflow_file_info;
	if (log_file = wal_args->walc_log_ovflow_file_info) {
		p = (UCHAR *) log_file->lg_name;
		strcpy(reinterpret_cast < char *>(q),
			   reinterpret_cast < const char *>(p));
		logf->logf_name_offset = q - (UCHAR *) WAL_segment;
		logf->logf_max_size = log_file->lg_size;
		logf->logf_roundup_size = 0;
		logf->logf_flags = 0;
		logf->logf_base_offset = 0L;
		logf->logf_fname_seqno = 0L;
		if (log_file->lg_flags & LOG_raw) {
			logf->logf_roundup_size = 512;
			logf->logf_flags |= LOGF_RAW;
		}
	}
	else {
		/* Set some default values */

		logf->logf_name_offset = 0;
		logf->logf_max_size = WAL_segment->wals_max_log_length;
		logf->logf_roundup_size = 0;
		logf->logf_flags = 0;
		logf->logf_base_offset = 0L;
		logf->logf_fname_seqno = 0L;
	}

/* Now setup the WAL buffers info */

	SSHORT maxbufs;
	WAL_segment->wals_initial_maxbufs =
		WAL_segment->wals_maxbufs = maxbufs = wal_args->walc_maxbufs;
	WAL_segment->wals_curbuf = 0;	/* CUR_BUF */
	WAL_segment->wals_last_flushed_buf = -1;	/* No buffer flushed so far */
	WAL_segment->wals_bufsize = wal_args->walc_bufsize;
	WAL_segment->wals_roundup_size = 0;	/* Default - no roundup */

/* Set the maximum allowable size of a log record.  It must be fully 
   acccomdable in one WAL buffer.  Set here to avoid repeated 
   recalculations later on. */

	WAL_segment->wals_max_recsize =
		wal_args->walc_bufsize - BLK_OVHD - REC_HDROVHD;

/* Now setup the long-term journalling info */

	if (wal_args->walc_jrn_dirname == NULL) {
		strcpy(WAL_segment->wals_jrn_dirname, "");
		WAL_segment->wals_jrn_init_data_len = 0;
	}
	else {
		strcpy(WAL_segment->wals_jrn_dirname, wal_args->walc_jrn_dirname);
		WAL_segment->wals_jrn_init_data_len = wal_args->walc_jrn_data_len;
		memcpy(WAL_segment->wals_jrn_init_data, wal_args->walc_jrn_data,
			   WAL_segment->wals_jrn_init_data_len);
	}

	WAL_segment->wals_total_IO_bytes = 0L;
	WAL_segment->wals_IO_count = 0L;

	WAL_segment->wals_acquire_count = 0L;
	WAL_segment->wals_put_count = 0L;
	WAL_segment->wals_commit_count = 0L;

	WAL_segment->wals_blksize = wal_args->walc_blksize;
	WAL_segment->wals_dummy2 = 0;
	WAL_segment->wals_blkseqno = 1L;
	WAL_segment->wals_blks_offset = wal_args->walc_blks_offset;
	WAL_segment->wals_flags = 0L;
	WAL_segment->wals_flags2 = 0L;
	if (wal_args->walc_first_time_log)
		WAL_segment->wals_flags |= WALS_FIRST_TIME_LOG;

	for (i = 0; i < maxbufs; i++) {
		WALBLK* wblk = WAL_BLOCK(i);
		wblk->walblk_number = i;
		wblk->walblk_flags = 0;
		wblk->walblk_roundup_offset = 0;
		wblk->walblk_cur_offset = 0;
	}

	return;
}
