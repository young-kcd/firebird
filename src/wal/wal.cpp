/*
 *	PROGRAM:	JRD Write Ahead Log APIs
 *	MODULE:		wal.c
 *	DESCRIPTION:	Write Ahead Log subsystem interface
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
 *
 */

#include "firebird.h"
#include <string.h>
#include "../jrd/jrd_time.h"
#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../wal/wal.h"
#include "../jrd/jrn.h"
#include "gen/codes.h"
#include "../wal/wal_proto.h"
#include "../wal/walc_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/iberr_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/thd_proto.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#if HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif
#ifndef WEXITSTATUS
# define WEXITSTATUS(stat_val) ((unsigned)(stat_val) >> 8)
#endif
#ifndef WIFEXITED
# define WIFEXITED(stat_val) (((stat_val) & 255) == 0)
#endif

#ifndef WIN_NT
#include <errno.h>
#endif

#ifdef VMS
#define waitpid(x,y,z)	wait (y)
#endif

#ifdef WIN_NT
#include <process.h>
#include <windows.h>
#ifdef TEXT
#undef TEXT
#endif
#define TEXT		SCHAR

#define sleep(seconds)	Sleep ((seconds) * 1000)
#endif


extern "C" {


#ifdef SUPERSERVER
extern void main_walw(SCHAR **);
#endif

static SLONG copy_buffer(WALS, WALBLK *, UCHAR *, USHORT, UCHAR *, USHORT);
static SSHORT fork_writer(ISC_STATUS *, WAL);
static SSHORT grpc_do_group_commit(ISC_STATUS *, WAL, SSHORT);
static void grpc_finish_group_commit(WAL, SSHORT);
static SSHORT grpc_wait_for_grouping(ISC_STATUS *, WAL, SSHORT);
static SSHORT grpc_wait_for_group_commit_finish(ISC_STATUS *, WAL, SSHORT,
												GRP_COMMIT *);
static void inform_wal_writer(WAL);
static SSHORT next_buffer_available(WALS);
static void setup_buffer_for_writing(WAL, WALS, SSHORT);
static SSHORT shutdown_writer(ISC_STATUS *, WAL, SSHORT);
static SSHORT sync_with_wal_writer(ISC_STATUS *, WAL);
static SSHORT wait_for_writer(ISC_STATUS *, WAL);
static SSHORT wal_put2(ISC_STATUS *, WAL, UCHAR *, USHORT, UCHAR *, USHORT,
					   SLONG *, SLONG *, SSHORT);

#ifdef WIN_NT
#define WAL_WRITER	"bin/ibwalwrt"
#endif

#ifndef WAL_WRITER
#define WAL_WRITER	"bin/gds_wal_writer"
#endif

#define WAIT_TIME	3000000
#define MAX_WAITERS	20
#define WALBLK_EFFECTIVE_OFFSET(wblk) (wblk->walblk_cur_offset?wblk->walblk_cur_offset:BLK_HDROVHD)

#ifdef SHLIB_DEFS
#define execl		(*_libgds_execl)
#define waitpid		(*_libgds_waitpid)
#define _exit		(*_libgds__exit)

extern int execl();
extern int waitpid();
extern void _exit();
#endif


SSHORT WAL_attach( ISC_STATUS * status_vector, WAL * WAL_handle, SCHAR * dbname)
{
/**************************************
 *
 *	W A L _ a t t a c h
 *
 **************************************
 *
 * Functional description
 *	Attach to an already intitalized WAL segment for the given
 *	database.
 *	Return FB_SUCCESS if attachment succeeds else return FB_FAILURE.
 *
 **************************************/
	int ret;
	ISC_STATUS_ARRAY local_status;

	ret = WALC_init(status_vector, WAL_handle, dbname, 0,
					NULL, 0L, FALSE, 1L, 0, NULL, FALSE);
	if (ret == FB_SUCCESS) {
		if ((ret = WALC_check_writer(*WAL_handle)) != FB_SUCCESS)
			ret = fork_writer(status_vector, *WAL_handle);
		else
			ret = sync_with_wal_writer(status_vector, *WAL_handle);
		if (ret != FB_SUCCESS)
			WALC_fini(local_status, WAL_handle);
	}

	return ret;
}


SSHORT WAL_checkpoint_finish(ISC_STATUS * status_vector,
							 WAL WAL_handle,
							 SLONG * log_seqno,
							 SCHAR * logname,
							 SLONG * log_partition_offset, SLONG * log_offset)
{
/**************************************
 *
 *	W A L _ c h e c k p o i n t _ f i n i s h
 *
 **************************************
 *
 * Functional description
 *	To inform the WAL writer that the caller (Asynchronous Buffer
 *	Writer) has done its part of checkpointing.
 *	This routine ib_puts a checkpoint (null) record in the log file.
 *	It returns the log file sequence number, its name, its position
 *	and the offset within it where this checkpointing finished.
 *
 ***************************************/
	WALS WAL_segment;
	UCHAR chkpt_rec[100];
	SLONG dummy_seqno;
	SLONG dummy_offset;

/* Do some sanity check.  We may remove this code once we are
 * convinced that this call will never be made improperly, i.e.
 * without the WALS_CKPT_START flag having been set. */

	WALC_acquire(WAL_handle, &WAL_segment);
	WAL_CHECK_BUG(WAL_handle, WAL_segment);
	if (!(WAL_segment->wals_flags & WALS_CKPT_START)) {
		/* Cannot finish checkpointing, if not already started. */

		WALC_release(WAL_handle);
		return FB_SUCCESS;
	}
	WALC_release(WAL_handle);

/* end of sanity check */

/* Checkpoint record is a logical concept.  The higher level routines
 * do not need to 'see' this record for recovery.   We are putting a
 * zero length record for this purpose. */

	wal_put2(status_vector, WAL_handle, chkpt_rec, 0,
			 (UCHAR *) 0, 0, log_seqno, log_offset, TRUE);

/* Now save the checkpoint record offset to be used by WAL writer later after
   it flushes the block containing the checkpoint record.  We need to do this
   now because our block may be blank-padded for raw-device support and then it
   would be difficult for the WAL writer to know the location of the checkpoint
   record in the block. */

	WALC_acquire(WAL_handle, &WAL_segment);
	WAL_segment->wals_saved_ckpted_offset = *log_offset;
	WALC_release(WAL_handle);

	WAL_flush(status_vector, WAL_handle, &dummy_seqno, &dummy_offset, FALSE);
	WALC_acquire(WAL_handle, &WAL_segment);
	*log_seqno = WAL_segment->wals_ckpted_log_seqno;
	strcpy(logname, WAL_segment->wals_ckpt_logname);
	*log_partition_offset = WAL_segment->wals_ckpt_log_p_offset;
	*log_offset = WAL_segment->wals_ckpted_offset;
	WALC_release(WAL_handle);

	return FB_SUCCESS;
}


SSHORT WAL_checkpoint_force(ISC_STATUS * status_vector,
							WAL WAL_handle,
							SLONG * log_seqno,
							SCHAR * logname,
							SLONG * log_partition_offset, SLONG * log_offset)
{
/**************************************
 *
 *	W A L _ c h e c k p o i n t _ f o r c e
 *
 **************************************
 *
 * Functional description
 *	This procedure forces a checkpoint to happen.  It starts as
 *	well as finishes the checkpoint from WALs point of view.
 *	The caller should subsequently make a call to
 *	WAL_checkpoint_recorded.
 *
 ***************************************/
	WALS WAL_segment;

	WALC_acquire(WAL_handle, &WAL_segment);
	WAL_CHECK_BUG(WAL_handle, WAL_segment);
	WAL_segment->wals_flags |= WALS_CKPT_START;
	WALC_release(WAL_handle);

	return WAL_checkpoint_finish(status_vector, WAL_handle, log_seqno,
								 logname, log_partition_offset, log_offset);
}


SSHORT WAL_checkpoint_start(ISC_STATUS * status_vector,
							WAL WAL_handle, SSHORT * ckpt_start)
{
/**************************************
 *
 *	W A L _ c h e c k p o i n t _ s t a r t ?
 *
 **************************************
 *
 * Functional description
 *	To inform the caller if checkpoint needs to be started.
 *	Returns TRUE or FASLE through ckpt_start parameter.
 *
 *	Returns FB_SUCCESS or FB_FAILURE.
 *
 ***************************************/
	WALS WAL_segment;

	WALC_acquire(WAL_handle, &WAL_segment);
	WAL_CHECK_BUG(WAL_handle, WAL_segment);
	*ckpt_start = FALSE;
	if ((WAL_segment->wals_flags & WALS_CKPT_START) &&
		!(WAL_segment->wals_flags & WALS_CKPT_RECORDED))
		*ckpt_start = TRUE;
	WALC_release(WAL_handle);

	return FB_SUCCESS;
}


SSHORT WAL_checkpoint_recorded(ISC_STATUS * status_vector, WAL WAL_handle)
{
/**************************************
 *
 *	W A L _ c h e c k p o i n t _ r e c o r d e d
 *
 **************************************
 *
 * Functional description
 *	To inform the WAL writer that the last checkpoint info has
 *	been recorded in the stable storage and so it can reuse
 *	earlier log files if possible.
 *
 *	Returns FB_SUCCESS or FB_FAILURE.
 *
 ***************************************/
	WALS WAL_segment;

	WALC_acquire(WAL_handle, &WAL_segment);
	WAL_CHECK_BUG(WAL_handle, WAL_segment);
	if (!(WAL_segment->wals_flags & WALS_CKPT_START)) {
		/* Cannot record checkpointing, if not already started. */

		WALC_release(WAL_handle);
		return FB_SUCCESS;
	}
	WAL_segment->wals_flags |= WALS_CKPT_RECORDED;
	WALC_release(WAL_handle);

	return FB_SUCCESS;
}


SSHORT WAL_commit(ISC_STATUS * status_vector,
				  WAL WAL_handle,
				  UCHAR * commit_logrec,
				  USHORT len, SLONG * log_seqno, SLONG * log_offset)
{
/**************************************
 *
 *	W A L _ c o m m i t
 *
 **************************************
 *
 * Functional description
 *	Put the commit log record in the WAL buffer and flush the
 *	buffer to log file before returning.
 *	If all the buffers are full and/or being flushed to disk,
 *	wait for one to become available.
 *	Implement group-commit protocol.
 *	Return the sequence number of the log file in the log file
 *	series and the offset of this commit record in that file.
 *
 *	Return FB_SUCCESS or FB_FAILURE.
 *
 **************************************/
	WALS WAL_segment;
	SSHORT grpc_blknum;
	GRP_COMMIT *grpc;
	SSHORT ret;
	SLONG dummy_seqno;
	SLONG dummy_offset;

	if (len && wal_put2(status_vector, WAL_handle, commit_logrec, len,
						(UCHAR *) 0, 0, log_seqno, log_offset, 0) != FB_SUCCESS)
		return FB_FAILURE;

	ret = FB_SUCCESS;

	WALC_acquire(WAL_handle, &WAL_segment);
	if (!len) {
		/* We did not put any commit record physically.  Just return our
		   current position in the log file after flushing. */

		*log_seqno = WAL_segment->wals_log_seqno;
		*log_offset = WAL_segment->wals_buf_offset;
	}

/* Do group-commit stuff. */

/* Get the group-commit waiting times from the shared segment.  They
   might have changed since we last did a commit. */

	if (WAL_handle->wal_grpc_wait_id != WAL_segment->wals_grpc_wait_id) {
		WAL_handle->wal_grpc_wait_id = WAL_segment->wals_grpc_wait_id;
		WAL_handle->wal_grpc_wait_usecs = WAL_segment->wals_grpc_wait_usecs;
		WAL_handle->wal_grpc_wait_coord_usecs =
			GRPC_COORD_FACTOR * WAL_handle->wal_grpc_wait_usecs;
		WAL_handle->wal_grpc_wait_other_coord_usecs =
			GRPC_OTHER_COORD_FACTOR * WAL_handle->wal_grpc_wait_usecs;
	}

	WAL_segment->wals_commit_count++;

	if (WAL_handle->wal_grpc_wait_usecs == 0L) {	/* No group-commit needs to be done */
		WALC_release(WAL_handle);
		return WAL_flush(status_vector, WAL_handle,
						 &dummy_seqno, &dummy_offset, FALSE);
	}

	grpc_blknum = WAL_segment->wals_cur_grpc_blknum;
	grpc = &WAL_segment->wals_grpc_blks[grpc_blknum];
	grpc->grp_commit_size++;

	if (grpc->grp_commit_coordinator == 0) {
		/* Become the group-leader (coordinator) */

		grpc->grp_commit_coordinator = WAL_handle->wal_pid;
		WAL_segment->wals_flags |= WALS_GRP_COMMIT_WAITING;
		ret = grpc_do_group_commit(status_vector, WAL_handle, grpc_blknum);
	}
	else {
		ret = grpc_wait_for_group_commit_finish(status_vector, WAL_handle,
												grpc_blknum, grpc);
	}

	return ret;
}


void WAL_fini( ISC_STATUS * status_vector, WAL * WAL_handle)
{
/**************************************
 *
 *	W A L _ f i n i
 *
 **************************************
 *
 * Functional description
 *	Unmap the WAL segment.
 *
 **************************************/

	WALC_fini(status_vector, WAL_handle);
}


SSHORT WAL_flush(
				 ISC_STATUS * status_vector,
				 WAL WAL_handle,
				 SLONG * log_seqno, SLONG * log_offset, BOOLEAN conditional)
{
/**************************************
 *
 *	W A L _ f l u s h
 *
 **************************************
 *
 * Functional description
 *	Flush all currently non-empty WAL buffers to log file.
 *	Usually called at the time of transaction commit and checkpoiniting.
 *
 *	If <log_seqno, log_offset> pair is passed then conditionally
 *	flush the WAL buffers based on this input.
 *
 *	Update private cached notion of the most recently flushed log seqno
 *	and offset.
 *
 **************************************/
	SLONG cur_log_seqno;
	SLONG cur_buf_offset;
	WALS WAL_segment;

/* Conditionally flush WAL buffers based on local notion of most recently
   flushed WAL buffers. */

	if (conditional)
		if (*log_seqno < WAL_handle->wal_flushed_seqno ||
			(*log_seqno == WAL_handle->wal_flushed_seqno &&
			 *log_offset <= WAL_handle->wal_flushed_offset)) return FB_SUCCESS;

	WALC_acquire(WAL_handle, &WAL_segment);
	WAL_CHECK_BUG(WAL_handle, WAL_segment);

/* Conditionally flush WAL buffers based on global notion of most recently
   flushed WAL buffers. */

	if (conditional)
		if (*log_seqno < WAL_segment->wals_flushed_log_seqno ||
			(*log_seqno == WAL_segment->wals_flushed_log_seqno &&
			 *log_offset <= WAL_segment->wals_flushed_offset))
			goto flush_exit;

/* Get the current status of the WAL buffers and then make sure that
   they are flushed at least upto that point. */

	cur_log_seqno = WAL_segment->wals_log_seqno;
	cur_buf_offset = WAL_segment->wals_buf_offset;
	if (cur_log_seqno == WAL_segment->wals_flushed_log_seqno &&
		cur_buf_offset == WAL_segment->wals_flushed_offset)
		goto flush_exit;

/* Current buffer is non-empty, set it up for writing.  The earlier
   buffers, if any, would already have been set for writing when we
   switched to this buffer.  */

	if (CUR_BUF != -1 && (WAL_BLOCK(CUR_BUF))->walblk_cur_offset > 0)
		setup_buffer_for_writing(WAL_handle, WAL_segment, 0);

	WAL_segment->wals_buf_waiters++;
	inform_wal_writer(WAL_handle);

/* Wait for the buffers to be flushed */

	do {
		wait_for_writer(status_vector, WAL_handle);
		WAL_segment = WAL_handle->wal_segment;
		WAL_CHECK_BUG_ERROR(WAL_handle, WAL_segment);
	} while (cur_log_seqno == WAL_segment->wals_flushed_log_seqno &&
			 cur_buf_offset > WAL_segment->wals_flushed_offset);

  flush_exit:

	WAL_handle->wal_flushed_seqno = WAL_segment->wals_flushed_log_seqno;
	WAL_handle->wal_flushed_offset = WAL_segment->wals_flushed_offset;

	WALC_release(WAL_handle);
	if (!conditional) {
		*log_seqno = WAL_handle->wal_flushed_seqno;
		*log_offset = WAL_handle->wal_flushed_offset;
	}

	return FB_SUCCESS;
}


SSHORT WAL_init(ISC_STATUS * status_vector,
				WAL * WAL_handle,
				SCHAR * dbname,
				USHORT db_page_len,
				SCHAR * logname,
				SLONG log_partition_offset,
				SSHORT first_time_log,
				SLONG new_log_seqno, SSHORT wpb_length, SCHAR * wpb)
{
/**************************************
 *
 *	W A L _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize Write Ahead Log segment for the database and
 *	attach to it.
 *
 *	Initialize the WAL_handle.
 *
 *	If first_time_log is TRUE then use the new_log_seqno
 *	as the starting sequence number for the set of new log
 *	files.
 *
 *	Start the WAL writer.
 *	Return FB_SUCCESS if initialization succeeds else return FB_FAILURE.
 *
 **************************************/
	SSHORT ret;
	ISC_STATUS_ARRAY local_status;

	ret = WALC_init(status_vector,
					WAL_handle,
					dbname,
					db_page_len,
					logname,
					log_partition_offset,
					first_time_log,
					new_log_seqno,
					wpb_length, reinterpret_cast < UCHAR * >(wpb), TRUE);
	if (ret == FB_SUCCESS) {
		if ((ret = fork_writer(status_vector, *WAL_handle)) != FB_SUCCESS)
			WALC_fini(local_status, WAL_handle);
	}

	return ret;
}


SSHORT WAL_journal_disable(ISC_STATUS * status_vector, WAL WAL_handle)
{
/**************************************
 *
 *	W A L _ j o u r n a l _ d i s a b l e
 *
 **************************************
 *
 * Functional description
 *	To inform the WAL writer that journalling should be disabled.
 *	Before returning from this procedure make sure that the WAL
 *	writer has severed its ties with the journal server.
 *
 *	Returns FB_SUCCESS or FB_FAILURE.
 *
 ***************************************/
	WALS WAL_segment;

	WALC_acquire(WAL_handle, &WAL_segment);
	WAL_CHECK_BUG(WAL_handle, WAL_segment);
	if (!(WAL_segment->wals_flags & WALS_JOURNAL_ENABLED)) {
		/* Journalling is already disabled. */

		WALC_release(WAL_handle);
		return FB_SUCCESS;
	}
	WAL_segment->wals_flags |= WALS_DISABLE_JOURNAL;
	inform_wal_writer(WAL_handle);

	while (WAL_segment->wals_flags & WALS_JOURNAL_ENABLED) {
		/* Wait for the WAL writer to severe its connection with the journal
		   server.  Should we get out with an error after a certain number
		   of retries ? -- Damodar */

		wait_for_writer(status_vector, WAL_handle);
		WAL_segment = WAL_handle->wal_segment;
		WAL_CHECK_BUG_ERROR(WAL_handle, WAL_segment);
	}

	WALC_release(WAL_handle);
	return FB_SUCCESS;
}


SSHORT WAL_journal_enable(ISC_STATUS * status_vector,
						  WAL WAL_handle,
						  SCHAR * jrn_dirname,
						  USHORT jrn_data_len, SCHAR * jrn_data)
{
/**************************************
 *
 *	W A L _ j o u r n a l _ e n a b l e
 *
 **************************************
 *
 * Functional description
 *	To inform the WAL writer that journalling has been enabled.
 *	Before returning from this procedure make sure that the WAL
 *	writer has established a connection with the journal server.
 *
 *	Returns FB_SUCCESS or FB_FAILURE.
 *
 ***************************************/
	WALS WAL_segment;

	WALC_acquire(WAL_handle, &WAL_segment);
	WAL_CHECK_BUG(WAL_handle, WAL_segment);
	if (WAL_segment->wals_flags & WALS_JOURNAL_ENABLED) {
		/* Journalling is already enabled. */

		WALC_release(WAL_handle);
		return FB_SUCCESS;
	}

	strcpy(WAL_segment->wals_jrn_dirname, jrn_dirname);
	WAL_segment->wals_jrn_init_data_len = jrn_data_len;
	memcpy(WAL_segment->wals_jrn_init_data, jrn_data, jrn_data_len);
	WAL_segment->wals_flags |= WALS_ENABLE_JOURNAL;
	inform_wal_writer(WAL_handle);

	while (!(WAL_segment->wals_flags & WALS_JOURNAL_ENABLED)) {
		/* Wait for the WAL writer to establish connection with the journal
		   server.  Should we get out with an error after a certain number
		   of retries ? -- Damodar */

		wait_for_writer(status_vector, WAL_handle);
		WAL_segment = WAL_handle->wal_segment;
		WAL_CHECK_BUG_ERROR(WAL_handle, WAL_segment);
	}

	WALC_release(WAL_handle);

	return FB_SUCCESS;
}


SSHORT WAL_put(ISC_STATUS * status_vector,
			   WAL WAL_handle,
			   UCHAR * logrec1,
			   USHORT len1,
			   UCHAR * logrec2,
			   USHORT len2, SLONG * log_seqno, SLONG * log_offset)
{
/**************************************
 *
 *	W A L _ p u t
 *
 **************************************
 *
 * Functional description
 *	Put the given log record in a shared log buffer.
 *	If all the buffers are full and or being flushed to disk,
 *	wait for one to become available.
 *
 *	If putting this buffer will make the current log file go
 *	beyond its rollover length then ask the WAL writer
 *	to rollover to the next log file and wait for that to happen
 *	before proceeding.
 *
 *	This routine is NOT supposed to flush the log record (buffer)
 *	to disk.
 *
 *	The log record is put in (header+logrec) fashion.
 *	Return the sequence number of the log file in the log file
 *	series and the offset of this logrec in that file where
 *	this log record would eventually be written.
 *
 *	Returns FB_SUCCESS or FB_FAILURE.
 *
 **************************************/

	return wal_put2(status_vector, WAL_handle,
					logrec1, len1, logrec2, len2, log_seqno, log_offset, 0);
}


bool WAL_rollover_happened(ISC_STATUS * status_vector,
							  WAL WAL_handle,
							  SLONG * new_seqno,
							  TEXT * new_logname,
							  SLONG * new_log_partition_offset)
{
/**************************************
 *
 *	W A L _ r o l l o v e r _ h a p p e n e d  ?
 *
 **************************************
 *
 * Functional description
 *	To inform the caller if rollover to a new log file has happened.
 *	Returns TRUE or FASLE.   If TRUE then new_logname,
 *	new_log_partition_offset and new_seqno parameters are initialized
 *	with the new information.  The caller should invoke
 *	WAL_rollover_recorded() after recording the rollover
 *	information at a safe place (e.g. the header page).
 *
 ***************************************/
	WALS WAL_segment;
	bool happened;

	happened = false;

	WALC_acquire(WAL_handle, &WAL_segment);
	WAL_CHECK_BUG(WAL_handle, WAL_segment);
	if (WAL_segment->wals_flags & WALS_ROLLOVER_HAPPENED) {
		happened = true;
		*new_seqno = WAL_segment->wals_log_seqno;
		strcpy(new_logname, WAL_segment->wals_logname);
		*new_log_partition_offset = WAL_segment->wals_log_partition_offset;
	}

	WALC_release(WAL_handle);

	return happened;
}


void WAL_rollover_recorded( WAL WAL_handle)
{
/**************************************
 *
 *	W A L _ r o l l o v e r _ r e c o r d e d
 *
 **************************************
 *
 * Functional description
 *	To inform the WAL writer that the last rollover information
 *	has been recorded.   After this, the WAL writer can rollover
 *	to a new log file if needed.
 *
 ***************************************/
	WALS WAL_segment;

	WALC_acquire(WAL_handle, &WAL_segment);
	WAL_segment->wals_flags &= ~WALS_ROLLOVER_HAPPENED;
	WALC_release(WAL_handle);
}


SSHORT WAL_set_checkpoint_length(
								 ISC_STATUS * status_vector,
								 WAL WAL_handle, SLONG ckpt_length)
{
/**************************************
 *
 *	W A L _ s e t _ c h e c k p o i n t _ l e n g t h
 *
 **************************************
 *
 * Functional description
 *	Sets the checkpoint length to the passed value
 *	(in Kbytes).
 *
 **************************************/
	WALS WAL_segment;

	if (ckpt_length < MIN_CKPT_INTRVL)
		return FB_FAILURE;			/* Should set the status vector also */
	WALC_acquire(WAL_handle, &WAL_segment);
	WAL_CHECK_BUG(WAL_handle, WAL_segment);

	WAL_segment->wals_max_ckpt_intrvl = OneK * ckpt_length;
	WAL_segment->wals_thrshold_intrvl =
		(SLONG)(WAL_segment->wals_max_ckpt_intrvl * 0.8);

	WALC_release(WAL_handle);

	return FB_SUCCESS;
}


void WAL_set_cleanup_flag( WAL WAL_handle)
{
/**************************************
 *
 *	W A L _ s e t _ c l e a n u p _ f l a g
 *
 **************************************
 *
 * Functional description
 *	Sets the cleanup flag in the WAL_handle so that
 *	shared resource may be cleaned up during WALC_fini().
 *
 **************************************/

	WAL_handle->wal_flags |= WAL_CLEANUP_HANDLE;
	WAL_handle->wal_flags |= WAL_SHUTDOWN_HANDLE;
}


SSHORT WAL_set_grpc_wait_time(
							  ISC_STATUS * status_vector,
							  WAL WAL_handle, SLONG wait_usecs)
{
/**************************************
 *
 *	W A L _ s e t _ g r p c _ w a i t _ t i m e
 *
 **************************************
 *
 * Functional description
 *	Sets the group commit wait time to the passed value
 *	(in microseconds).
 *
 **************************************/
	WALS WAL_segment;

	if (wait_usecs < 0)
		return FB_FAILURE;			/* Should set the status vector also */
	WALC_acquire(WAL_handle, &WAL_segment);
	WAL_CHECK_BUG(WAL_handle, WAL_segment);

	WAL_segment->wals_grpc_wait_id++;	/* For other users to notice the new value */
	WAL_segment->wals_grpc_wait_usecs = wait_usecs;
	WALC_release(WAL_handle);

	return FB_SUCCESS;
}


SSHORT WAL_set_rollover_log(ISC_STATUS * status_vector,
							WAL WAL_handle, LGFILE * rollover_log_info)
{
/**************************************
 *
 *	W A L _ s e t _ r o l l o v e r _ l o g
 *
 **************************************
 *
 * Functional description
 *	Sets the base file name to be used for rolling over to the next
 *	log file.
 *
 **************************************/
	WALS WAL_segment;
	LOGF *logf;

	WALC_acquire(WAL_handle, &WAL_segment);
	WAL_CHECK_BUG(WAL_handle, WAL_segment);

	if (WAL_segment->wals_max_logfiles > 0)
		logf = &WAL_segment->wals_log_ovflow_file_info;
	else
		logf = &WAL_segment->wals_log_serial_file_info;

	strcpy(WAL_segment->wals_rollover_logname_base,
		   rollover_log_info->lg_name);
	logf->logf_name_offset =
		(UCHAR *) WAL_segment->wals_rollover_logname_base -
		(UCHAR *) WAL_segment;
	logf->logf_max_size = MAX(rollover_log_info->lg_size, MIN_LOG_LENGTH);
	logf->logf_roundup_size = 0;
	logf->logf_flags = rollover_log_info->lg_flags;

	WALC_release(WAL_handle);

	return FB_SUCCESS;
}


SSHORT WAL_shutdown(ISC_STATUS * status_vector,
					WAL WAL_handle,
					SLONG * log_seqno,
					SCHAR * logname,
					SLONG * log_partition_offset,
					SLONG * shutdown_offset, SSHORT inform_close_to_jserver)
{
/**************************************
 *
 *	W A L _ s h u t d o w n
 *
 **************************************
 *
 * Functional description
 *	Informs the WAL writer to shut down its operations.
 *	This procedure should be called when no other process (thread)
 *	is attached to the database.
 *
 *	Returns the information about the last log file in use.
 *
 **************************************/

/* First force a checkpoint */

	WAL_checkpoint_force(status_vector, WAL_handle, log_seqno, logname,
						 log_partition_offset, shutdown_offset);

/* Now shutdown the WAL writer */

	return shutdown_writer(status_vector, WAL_handle,
						   inform_close_to_jserver);
}


SSHORT WAL_shutdown_old_writer(ISC_STATUS * status_vector, SCHAR * dbname)
{
/**************************************
 *
 *	W A L _ s h u t d o w n _ o l d _ w r i t e r
 *
 **************************************
 *
 * Functional description
 *	Informs an existing WAL writer (if any) for the passed
 *	database to shut down its operations.  It's not an error
 *	if no WAL writer for the given database is running.
 *
 *	A WAL Writer may be hanging around if the 'last' process
 *	attached to the database did not or could not detach from
 *	the database.  So, this routine is used to shutdown that
 *	WAL writer before starting recovery or in preparation for
 *	starting a fresh WAL writer.
 *
 ************************************/
	WAL WAL_handle;
	ISC_STATUS_ARRAY local_status;

	WAL_handle = NULL;

	if (WAL_attach(local_status, &WAL_handle, dbname) != FB_SUCCESS)
		return FB_SUCCESS;			/* Nobody is attached to the shared WAL segment */

	if (WALC_check_writer(WAL_handle) == FB_SUCCESS)
		shutdown_writer(status_vector, WAL_handle, (SSHORT) 0);

	WAL_fini(status_vector, &WAL_handle);

	return FB_SUCCESS;
}


SSHORT WAL_status(ISC_STATUS * status_vector,
				  WAL WAL_handle,
				  SLONG * log_seqno,
				  SCHAR * logname,
				  SLONG * log_partition_offset,
				  SLONG * flushed_offset,
				  SLONG * ckpt_seqno,
				  SCHAR * ckpt_logname,
				  SLONG * ckpt_log_partition_offset, SLONG * ckpt_offset)
 {
/**************************************
 *
 *	W A L _ s t a t u s
 *
 **************************************
 *
 * Functional description
 *	Returns current status of the WAL activites through passed
 *	parameters.
 *
 **************************************/
	WALS WAL_segment;

	WALC_acquire(WAL_handle, &WAL_segment);
	WAL_CHECK_BUG_ERROR(WAL_handle, WAL_segment);
	if (log_seqno)
		*log_seqno = WAL_segment->wals_log_seqno;
	if (logname)
		strcpy(logname, WAL_segment->wals_logname);
	if (log_partition_offset)
		*log_partition_offset = WAL_segment->wals_log_partition_offset;
	if (flushed_offset)
		*flushed_offset = WAL_segment->wals_flushed_offset;
	if (ckpt_seqno)
		*ckpt_seqno = WAL_segment->wals_ckpted_log_seqno;
	if (ckpt_logname)
		strcpy(ckpt_logname, WAL_segment->wals_ckpt_logname);
	if (ckpt_log_partition_offset)
		*ckpt_log_partition_offset = WAL_segment->wals_ckpt_log_p_offset;
	if (ckpt_offset)
		*ckpt_offset = WAL_segment->wals_ckpted_offset;
	WALC_release(WAL_handle);

	return FB_SUCCESS;
}


static SLONG copy_buffer(
						 WALS WAL_segment,
						 WALBLK * wblk,
						 UCHAR * source1,
						 USHORT count1, UCHAR * source2, USHORT count2)
{
/**************************************
 *
 *	c o p y _ b u f f e r
 *
 **************************************
 *
 * Functional description
 *	Copies 'count' bytes from source to the specified WAL buffer.
 *	Puts a header as prefix.
 *	The sufficiency of space in wblk has already been ascertained
 *	beforehand.
 *	Returns the offset in the WAL file where these bytes would endup.
 *
 **************************************/
	UCHAR *p, *q;
	WALREC_HDR header;
	SLONG offset;
	USHORT total_count;

	if (wblk->walblk_cur_offset == 0) {
		/* This buffer is being written from the beginning.  Account for
		   the block header bytes. */

		wblk->walblk_cur_offset = BLK_HDROVHD;
		WAL_segment->wals_buf_offset += BLK_HDROVHD;
	}

	offset = WAL_segment->wals_buf_offset;

/* First build the record header */

	total_count = count1 + count2;
	header.walrec_len = total_count;
	header.walrec_offset = wblk->walblk_cur_offset;
	p = (UCHAR *) & header;
	q = &wblk->walblk_buf[wblk->walblk_cur_offset];
	memcpy(q, p, REC_HDROVHD);
	q += REC_HDROVHD;

/* Now put 'count' bytes from source */

	memcpy(q, source1, count1);	/* Use some fast copy technique. Damodar */
	if (count2) {
		q += count1;
		memcpy(q, source2, count2);
	}

	wblk->walblk_cur_offset += (total_count + REC_HDROVHD);
	WAL_segment->wals_buf_offset += (total_count + REC_HDROVHD);

	return offset;
}


#ifdef SUPERSERVER
static SSHORT fork_writer( ISC_STATUS * status_vector, WAL WAL_handle)
{
/**************************************
 *
 *	f o r k _ w r i t e r 	( S U P E R S E R V E R )
 *
 **************************************
 *
 * Functional description
 *	Fork the WAL writer process (thread) and give it enough time
 *	to initialize itself.
 *
 *	Returns FB_SUCCESS if successful in forking the WAL writer.
 *
 **************************************/
	WALS WAL_segment;
	TEXT *argv[20];

	WALC_acquire(WAL_handle, &WAL_segment);
	WAL_segment->wals_flags &= ~WALS_WRITER_INITIALIZED;

	argv[0] = (TEXT *) 2;
	argv[1] = WAL_segment->wals_dbname;
	if (gds__thread_start
		(reinterpret_cast < FPTR_INT_VOID_PTR > (main_walw), argv, 0, 0, 0)) {
		WALC_release(WAL_handle);
		IBERR_build_status(status_vector, gds_bug_check,
						   gds_arg_string, "cannot start thread", 0);
		return FB_FAILURE;
	}

	WALC_release(WAL_handle);

	return sync_with_wal_writer(status_vector, WAL_handle);
}
#endif


#ifndef SUPERSERVER
static SSHORT fork_writer( ISC_STATUS * status_vector, WAL WAL_handle)
{
/**************************************
 *
 *	f o r k _ w r i t e r 	( G E N E R I C )
 *
 **************************************
 *
 * Functional description
 *	Fork the WAL writer process (thread) and give it enough time
 *	to initialize itself.
 *
 *	Returns FB_SUCCESS if successful in forking the WAL writer.
 *
 **************************************/
	WALS WAL_segment;
	TEXT image_name[MAXPATHLEN];
	int pid;

	gds__prefix(image_name, WAL_WRITER);

/* Use WALS_WRITER_INITIALIZED flag to synchronize with the WAL_writer. */

	WALC_acquire(WAL_handle, &WAL_segment);
	WAL_segment->wals_flags &= ~WALS_WRITER_INITIALIZED;

#if (defined WIN_NT)
	pid =
		spawnl(P_DETACH, image_name, WAL_WRITER, WAL_segment->wals_dbname,
			   NULL);
#else
/* We are doing vfork() twice below to start the WAL writer process.
   This is to get around the problem of a <defunt> process not responding
   to kill(0) signal properly on HP-UX systems.  On HP-UX, if a child
   (WAL writer) dies but the parent still exists then kill(0) to
   the pid of the dead process (WAL writer) does not return an error
   of ESRCH and hence we cannot restart it.   Doing vfork() twice
   makes sure that the parent of WAL writer 'dies' so that WAL writer
   would never get into a <defunt> state in case it terminates
   prematurely. */

	if (!(pid = vfork())) {
		if (!vfork()) {
			execl(image_name, image_name, WAL_segment->wals_dbname, NULL);
			_exit(FINI_OK);
		}
		_exit(FINI_OK);			/* Parent of the newly started WAL writer dies here */
	}
#endif

	WALC_release(WAL_handle);
#if (defined WIN_NT)
	if (pid == -1)
#else
	if (pid == -1
		|| (waitpid(pid, NULL, 0) == -1 && !SYSCALL_INTERRUPTED(errno)))
#endif
	{
		WALC_bug(status_vector, WAL_handle->wal_dbname,
				 "Can't start WAL writer");
		return FB_FAILURE;
	}

	return sync_with_wal_writer(status_vector, WAL_handle);
}
#endif


static SSHORT grpc_do_group_commit(
								   ISC_STATUS * status_vector,
								   WAL WAL_handle, SSHORT grpc_blknum)
{
/**************************************
 *
 *	g r p c _ d o _ g r o u p _ c o m m i t
 *
 **************************************
 *
 * Functional description
 *	This process (thread) has been chosen to be the group
 *	coordinator.  The group-commit block to be used is passed as
 *	a parameter.
 *	NOTE: It is assume that upon entry to this routine, shared memory
 *	is acquired.
 *
 **************************************/
	SLONG dummy_seqno;
	SLONG dummy_offset;
	SSHORT ret;

/* Wait for more grouping and for the other group-commit_block
   to be available. */

	ret = grpc_wait_for_grouping(status_vector, WAL_handle, grpc_blknum);
	if (ret != FB_SUCCESS)
		return ret;

	ret =
		WAL_flush(status_vector, WAL_handle, &dummy_seqno, &dummy_offset,
				  FALSE);
	if (ret != FB_SUCCESS)
		return ret;

	grpc_finish_group_commit(WAL_handle, grpc_blknum);

	return FB_SUCCESS;
}


static void grpc_finish_group_commit( WAL WAL_handle, SSHORT grpc_blknum)
{
/**************************************
 *
 *	g r p c _ f i n i s h _ g r o u p _ c o m m i t
 *
 **************************************
 *
 * Functional description
 *	Clears the group coordinator block and informs the other
 *	participants.  Also alerts the other coordinator, if any.
 *
 **************************************/
	WALS WAL_segment;
	GRP_COMMIT *grpc;

	WALC_acquire(WAL_handle, &WAL_segment);
	WAL_segment->wals_grpc_count++;
	grpc = &WAL_segment->wals_grpc_blks[grpc_blknum];
	grpc->grp_commit_coordinator = 0;

	if (grpc->grp_commit_size > 1)	/* Inform other participants */
		ISC_event_post(WAL_EVENTS + grpc->grp_commit_event_num);

	grpc->grp_commit_size = 0;
	WAL_segment->wals_flags &= ~WALS_GRP_COMMIT_IN_PROGRESS;
	if (WAL_segment->wals_flags & WALS_GRP_COMMIT_WAITING) {
		/* Other committer is waiting for this group commit block to be cleared. */

		ISC_event_post(WAL_EVENTS + WAL_GCOMMIT_STALL_SEM);
	}
	WALC_release(WAL_handle);
}


static SSHORT grpc_wait_for_grouping(
									 ISC_STATUS * status_vector,
									 WAL WAL_handle, SSHORT grpc_blknum)
{
/**************************************
 *
 *	g r p c _ w a i t _ f o r _ g r o u p i n g
 *
 **************************************
 *
 * Functional description
 *	This function first waits for increasing the group size.  It also
 *	makes sure that the other group-commit block is available before
 *	this group-commit coordinator closes its door.
 *	NOTE: It is assume that upon entry to this routine, shared memory
 *	is acquired.
 *
 **************************************/
	EVENT ptr;
	SLONG value;
	WALS WAL_segment;
	SSHORT wait_count, other_grpc_blknum;
	GRP_COMMIT *other_grpc;

/* Previously, the semaphore was being set only when called from WAL_commit. */

	WAL_segment = WAL_handle->wal_segment;
	ptr = &WAL_EVENTS[WAL_GCOMMIT_STALL_SEM];
	value = ISC_event_clear(ptr);
	WALC_release(WAL_handle);

	ISC_event_wait(1,
				   &ptr,
				   &value,
				   WAL_handle->wal_grpc_wait_usecs,
				   reinterpret_cast < void (*)() > (WALC_alarm_handler), ptr);

/* Now make sure that the other group-commit block is available */

	other_grpc_blknum = (grpc_blknum + 1) % MAX_GRP_COMMITTERS;
	wait_count = 0;
	WALC_acquire(WAL_handle, &WAL_segment);
	while (WAL_segment->wals_flags & WALS_GRP_COMMIT_IN_PROGRESS) {
		value = ISC_event_clear(ptr);
		WALC_release(WAL_handle);
		ptr = &WAL_EVENTS[WAL_GCOMMIT_STALL_SEM];
		ISC_event_wait(1, &ptr, &value,
					   WAL_handle->wal_grpc_wait_other_coord_usecs,
					   reinterpret_cast < void (*)() > (WALC_alarm_handler),
					   ptr);
		WALC_acquire(WAL_handle, &WAL_segment);
		WAL_CHECK_BUG(WAL_handle, WAL_segment);
		if ((WAL_segment->wals_flags & WALS_GRP_COMMIT_IN_PROGRESS) &&
			((wait_count++) > 5)) {
			/* Time to check the other coordinator, what's going on ? */

			other_grpc = &WAL_segment->wals_grpc_blks[other_grpc_blknum];
			if (other_grpc->grp_commit_size == 1 &&
				!ISC_check_process_existence(other_grpc->
											 grp_commit_coordinator, 0,
											 FALSE)) {
				/* Other coordinator is dead and there is no participant.
				   So, clean up. */

				WALC_release(WAL_handle);
				grpc_finish_group_commit(WAL_handle, other_grpc_blknum);
				WALC_acquire(WAL_handle, &WAL_segment);
			}
			wait_count = 0;
		}
	}
	WAL_segment->wals_flags &= ~WALS_GRP_COMMIT_WAITING;
	WAL_segment->wals_flags |= WALS_GRP_COMMIT_IN_PROGRESS;
	WAL_segment->wals_cur_grpc_blknum =
		(grpc_blknum + 1) % MAX_GRP_COMMITTERS;
	WALC_release(WAL_handle);

	return FB_SUCCESS;
}


static SSHORT grpc_wait_for_group_commit_finish(
												ISC_STATUS * status_vector,
												WAL WAL_handle,
												SSHORT grpc_blknum,
GRP_COMMIT * grpc)
{
/**************************************
 *
 *	g r p c _ w a i t _ f o r _ g r o u p _ co m m i t _ f i n i s h
 *
 **************************************
 *
 * Functional description
 *	This function waits for the group commit on the given block to
 *	finish.  All the participants call this routine.
 *	The parameter wait_id is the semaphore number to wait on.
 *	If needed, become the group_coordinator.
 *	NOTE: It is assume that upon entry to this routine, shared memory
 *	is acquired.
 *
 **************************************/
	SLONG value;
	EVENT ptr;
	WALS WAL_segment;

/* First participant sets the notification semaphore */

/* Previously, the semaphore was being set only when the commit size was 2.
   Now it is set regardless. */

	WAL_segment = WAL_handle->wal_segment;
	ptr = &WAL_EVENTS[grpc->grp_commit_event_num];
	value = ISC_event_clear(ptr);
	WALC_release(WAL_handle);
	while (ISC_event_wait
		   (1, &ptr, &value, WAL_handle->wal_grpc_wait_coord_usecs,
			reinterpret_cast < void (*)() > (WALC_alarm_handler),
			ptr) != FB_SUCCESS) {
		/* Check to make sure that the coordinator is still alive. */

		WALC_acquire(WAL_handle, &WAL_segment);
		WAL_CHECK_BUG(WAL_handle, WAL_segment);
		grpc = &WAL_segment->wals_grpc_blks[grpc_blknum];
		if (!ISC_check_process_existence
			(grpc->grp_commit_coordinator, 0, FALSE)) {
			/* Coordinator is dead.  Let's take over. */

			grpc->grp_commit_size--;
			grpc->grp_commit_coordinator = WAL_handle->wal_pid;
			grpc_do_group_commit(status_vector, WAL_handle, grpc_blknum);
			return FB_SUCCESS;
		}
		WALC_release(WAL_handle);
	}

	return FB_SUCCESS;
}


static void inform_wal_writer( WAL WAL_handle)
{
/**************************************
 *
 *	i n f o r m _ w a l _ w r i t e r
 *
 **************************************
 *
 * Functional description
 *	Inform WAL writer that there is some work to do.
 *
 ***************************************/
	WALS WAL_segment;

	WAL_segment = WAL_handle->wal_segment;

	if (!(WAL_segment->wals_flags & WALS_WRITER_INFORMED)) {
		ISC_event_post(WAL_EVENTS + WAL_WRITER_WORK_SEM);
		WAL_segment->wals_flags |= WALS_WRITER_INFORMED;
	}
}


static SSHORT next_buffer_available( WALS WAL_segment)
{
/**************************************
 *
 *	n e x t _ b u f f e r _ a v a i l a b l e
 *
 **************************************
 *
 * Functional description
 *	Return the next (to current) available buffer number modulo
 *	maximum allocated buffers for this WAL.
 *	If the next buffer is not available, return -1.
 *
 ***************************************/
	int next_buf;
	WALBLK *wblk;

	next_buf = (CUR_BUF + 1) % WAL_segment->wals_maxbufs;

	wblk = WAL_BLOCK(next_buf);
	if (wblk->walblk_flags & WALBLK_to_be_written)	/* Implies all buffers are full */
		return -1;

	return next_buf;
}


static void setup_buffer_for_writing(
									 WAL WAL_handle,
									 WALS WAL_segment, SSHORT ckpt)
{
/**************************************
 *
 *	s e t u p _ b u f f e r _ f o r _ w r i t i n g
 *
 **************************************
 *
 * Functional description
 *	Prepare the cuurent buffer block for writing.
 *	Inform the WAL writer that the current buffer is ready
 *	to be flushed to disk.  Assumes that acquire() has
 *	been done, before calling this routine.
 *	If 'ckpt' flag is TRUE then this buffer finishes a checkpoint.
 ***************************************/
	WALBLK *wblk;

	wblk = WAL_BLOCK(CUR_BUF);
	if (wblk->walblk_cur_offset <= BLK_HDROVHD) {
		WALC_release(WAL_handle);
		WALC_bug(NULL, WAL_handle->wal_dbname,
				 "An empty WAL buffer submitted for writing");
	}

	WALC_setup_buffer_block(WAL_segment, wblk, ckpt);
	CUR_BUF = next_buffer_available(WAL_segment);
	inform_wal_writer(WAL_handle);
}


static SSHORT shutdown_writer(
							  ISC_STATUS * status_vector,
							  WAL WAL_handle, SSHORT inform_close_to_jserver)
{
/**************************************
 *
 *	s h u t d o w n _ w r i t e r
 *
 **************************************
 *
 * Functional description
 *	Informs the WAL writer to shut down its operations.
 *	This procedure should be called when no other process (thread)
 *	is attached to the database.
 *
 **************************************/
	WALS WAL_segment;

	WALC_acquire(WAL_handle, &WAL_segment);
	WAL_segment->wals_flags |= WALS_SHUTDOWN_WRITER;
	if (inform_close_to_jserver)
		WAL_segment->wals_flags |= WALS_INFORM_CLOSE_TO_JOURNAL;
	inform_wal_writer(WAL_handle);
	WALC_release(WAL_handle);

/* Now wait for the WAL writer to shut itself down */

	sleep(2);

	WALC_acquire(WAL_handle, &WAL_segment);
	while (!(WAL_segment->wals_flags & WALS_WRITER_DONE)) {
		wait_for_writer(status_vector, WAL_handle);
		WAL_segment = WAL_handle->wal_segment;
		WAL_CHECK_BUG_ERROR(WAL_handle, WAL_segment);
	}
	WALC_release(WAL_handle);
	WAL_handle->wal_flags |= WAL_SHUTDOWN_HANDLE;

	return FB_SUCCESS;
}


static SSHORT sync_with_wal_writer( ISC_STATUS * status_vector, WAL WAL_handle)
{
/**************************************
 *
 *	s y n c _ w i t h _ W A L _ w r i t e r
 *
 **************************************
 *
 * Functional description
 *	Make sure that WAL writer has initialized itself (and parts of
 *	the WAL segment).  This is to avoid some race conditions when
 *	other processes may use the info regarding the log file in
 *	the WAL segment before the WAL writer got a chance to initialize
 *	that.  Returns FB_FAILURE after some number (10) of retries.
 *
 **************************************/
	SSHORT done;
	SSHORT count;
	WALS WAL_segment;

	done = FB_FAILURE;
	sleep(1);
	for (count = 0; count < 10; count++) {
		WALC_acquire(WAL_handle, &WAL_segment);
		WAL_CHECK_BUG_ERROR(WAL_handle, WAL_segment);
		if (WAL_segment->wals_flags & WALS_WRITER_INITIALIZED) {
			done = FB_SUCCESS;		/* WAL_writer has initialized itself. */
			WALC_release(WAL_handle);
			break;
		}
		WALC_release(WAL_handle);
		sleep(3);
	}

	if (done != FB_SUCCESS)
		WAL_ERROR(status_vector, gds_wal_err_ww_sync,
				  WAL_handle->wal_dbname);

	return done;
}


static SSHORT wait_for_writer( ISC_STATUS * status_vector, WAL WAL_handle)
{
/**************************************
 *
 *	w a i t _ f o r _ w r i t e r
 *
 **************************************
 *
 * Functional description
 *	We need to wait for a WAL buffer to be flushed to disk.
 *	Let's wait for the WAL writer to do the writing.
 *	The caller is supposed to have "acquire()d" the lock.
 *
 **************************************/
	EVENT ptr;
	SLONG value;
	SSHORT ret;
	WALS WAL_segment;

	WAL_segment = WAL_handle->wal_segment;
	ptr = &WAL_EVENTS[WAL_WRITER_WORK_DONE_SEM];
	value = ISC_event_clear(ptr);
	WALC_release(WAL_handle);

	ret =
		ISC_event_wait(1, &ptr, &value, WAIT_TIME,
					   reinterpret_cast < void (*)() > (WALC_alarm_handler),
					   ptr);
	if (ret == FB_FAILURE) {
		/* We got out because of timeout.  May be our condition is
		   already met.  Let the caller decide that.  In any case, make
		   sure that the WAL_writer process is alive. */

		if ((ret = WALC_check_writer(WAL_handle)) != FB_SUCCESS)
			ret = fork_writer(status_vector, WAL_handle);
	}

	WALC_acquire(WAL_handle, &WAL_segment);

	return ret;
}


static SSHORT wal_put2(
					   ISC_STATUS * status_vector,
					   WAL WAL_handle,
					   UCHAR * logrec1,
					   USHORT len1,
UCHAR * logrec2,
USHORT len2, SLONG * log_seqno, SLONG * log_offset, SSHORT ckpt)
 {
/**************************************
 *
 *	w a l _ p u t 2
 *
 **************************************
 *
 * Functional description
 *	Put the given log record in a shared log buffer.
 *	If all the buffers are full and/or being flushed to disk,
 *	wait for one to become available.
 *
 *	If putting this buffer will make the current log file go
 *	beyond its rollover length then ask the WAL writer
 *	to rollover to the next log file and wait for that to happen
 *	before proceeding.
 *
 *	This routine is NOT supposed to flush the log record (buffer)
 *	to disk.
 *
 *	The log record is put in (header+logrec) fashion.
 *	Return the sequence number of the log file in the log file
 *	series and the offset of this logrec in that file where
 *	this log record would eventually be written.
 *
 *	If 'ckpt' parameter is TRUE then steup the buffer with
 *	this logrec for writing and mark it as a checkpoint buffer.
 *	The WAL writer will handle this checkpointed buffer in a special
 *	way.
 *
 *	Returns FB_SUCCESS or FB_FAILURE.
 *
 **************************************/
	int available_bytes;
	USHORT total_len, done = FALSE;
	WALBLK *wblk;
	SLONG lsn;
	SLONG offset;
	WALS WAL_segment;
#define ROLLOVER_OVHD (REC_HDROVHD + BLK_OVHD + WAL_TERMINATOR_BLKLEN)

	WALC_acquire(WAL_handle, &WAL_segment);
	WAL_CHECK_BUG(WAL_handle, WAL_segment);
	total_len = len1 + len2;
	if (total_len > WAL_segment->wals_max_recsize) {
		WALC_release(WAL_handle);
		WALC_bug(status_vector, WAL_handle->wal_dbname,
				 "log record too long");
		return FB_FAILURE;
	}

	WAL_segment->wals_put_count++;
	while (!done) {
		while (CUR_BUF == -1
			   || (WAL_segment->wals_flags & WALS_ROLLOVER_REQUIRED)) {
			/* No buffer is available or rollover is in progress, wait */

			if ((CUR_BUF == -1)
				&& (++WAL_segment->wals_buf_waiting_count > MAX_WAITERS)) {
				WAL_segment->wals_flags2 |= WALS2_EXPAND;
				WAL_segment->wals_buf_waiting_count = 0;
			}
			WAL_segment->wals_buf_waiters++;
			wait_for_writer(status_vector, WAL_handle);
			WAL_segment = WAL_handle->wal_segment;
			WAL_CHECK_BUG(WAL_handle, WAL_segment);
		}

		wblk = WAL_BLOCK(CUR_BUF);

		/* Check for rollover requirement */

		if ((SLONG)(WAL_segment->wals_buf_offset + total_len + ROLLOVER_OVHD) >=
			WAL_segment->wals_rollover_threshold)
		{
			WAL_segment->wals_flags |= WALS_ROLLOVER_REQUIRED;
			if (wblk->walblk_cur_offset > BLK_HDROVHD)
				setup_buffer_for_writing(WAL_handle, WAL_segment, 0);
			WAL_segment->wals_buf_waiters++;
			wait_for_writer(status_vector, WAL_handle);
			WAL_segment = WAL_handle->wal_segment;
			WAL_CHECK_BUG(WAL_handle, WAL_segment);
			continue;
		}

		/* logrec will be written in header+logrec fashion to the log.  So
		   we need extra bytes of header.  Check the current buffer. */
		available_bytes = WAL_segment->wals_bufsize -
			WALBLK_EFFECTIVE_OFFSET(wblk) - REC_HDROVHD - BLK_TAILOVHD;
		if (available_bytes >= (int) total_len) {
			/* Found a large enough buffer, use it. */

			lsn = WAL_segment->wals_log_seqno;
			offset =
				copy_buffer(WAL_segment, wblk, logrec1, len1, logrec2, len2);
			done = TRUE;
		}
		else
			/* Assumption: One empty WAL buffer is long enough to fully accommodate
			   any one log record.  Let's try the next buffer. */

			setup_buffer_for_writing(WAL_handle, WAL_segment, 0);
	}

	if (ckpt)
		setup_buffer_for_writing(WAL_handle, WAL_segment, ckpt);
	WALC_release(WAL_handle);

	*log_seqno = lsn;
	*log_offset = offset;

	return FB_SUCCESS;
}


} // extern "C"

