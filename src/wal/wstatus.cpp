/*
 *	PROGRAM:	JRD Write Ahead Log APIs
 *	MODULE:		wstatus.c
 *	DESCRIPTION:	Displays shared WAL info
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
#include "../wal/wal.h"
#include "../wal/walc_proto.h"
#include "../wal/wstat_proto.h"

#ifndef FPRINTF
#define FPRINTF		ib_fprintf
#endif


void WSTATUS_dump_wal_segment( WAL WAL_handle, int acquire_flag,
							  IB_FILE * outfile)
{
/**************************************
 *
 *	W S T A T U S _ d u m p _ w a l _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Print the shared WAL segment info in a 
 *	formatted way.
 *
 *	these messages were considered for translation but left as is for now
 *	since they are likely only to be seen by DBA's and technical types
 *    
 **************************************/
	WALS WAL_segment;
	WALBLK *wblk;
	LOGF *logf;
	int i;

	if (acquire_flag)
		WALC_acquire(WAL_handle, &WAL_segment);
	else
		WAL_segment = WAL_handle->wal_segment;

	FPRINTF(outfile,
			"---------------------------------------------------------\n");
	FPRINTF(outfile,
			"WAL segment flags=%ld, flags2=%ld, Last bug=%ld, Last error=%ld\n",
			WAL_segment->wals_flags, WAL_segment->wals_flags2,
			WAL_segment->wals_last_bug, WAL_segment->wals_last_err);
	FPRINTF(outfile, "Database page size=%d, owner id=%"SLONGFORMAT
			", group id=%"SLONGFORMAT"\n",
			WAL_segment->wals_dbpage_size, WAL_segment->wals_owner_id,
			WAL_segment->wals_group_id);
	FPRINTF(outfile, "Num attaches=%d, Num detaches=%d\n",
			WAL_segment->wals_num_attaches, WAL_segment->wals_num_detaches);
	FPRINTF(outfile,
			"---------------------------------------------------------\n");
	FPRINTF(outfile,
			"WAL length=%"SLONGFORMAT
			", buffer size=%d, total buffers=%d, original buffers=%d\n",
			WAL_segment->wals_length, WAL_segment->wals_bufsize,
			WAL_segment->wals_maxbufs, WAL_segment->wals_initial_maxbufs);
	FPRINTF(outfile,
			"current buffer=%d, last flushed buffer=%d, max buf waiting count=%d\n",
			WAL_segment->wals_curbuf, WAL_segment->wals_last_flushed_buf,
			WAL_segment->wals_buf_waiting_count);
	FPRINTF(outfile, "Number \tCurrent offset \tFlags\n");
	for (i = 0; i < WAL_segment->wals_maxbufs; i++) {
		wblk = WAL_BLOCK(i);
		FPRINTF(outfile, "%d \t\t%d \t%ud\n",
				wblk->walblk_number, wblk->walblk_cur_offset,
				wblk->walblk_flags);
	}
	FPRINTF(outfile,
			"---------------------------------------------------------\n");
	FPRINTF(outfile, "current log seqno=%ld, logfile=%s\n",
			WAL_segment->wals_log_seqno, WAL_segment->wals_logname);
	FPRINTF(outfile, "log partition offset=%ld, current offset=%ld\n",
			WAL_segment->wals_log_partition_offset,
			WAL_segment->wals_buf_offset);
	FPRINTF(outfile,
			"Flushed log seqno=%ld, Flushed offset=%ld, Rollover length=%ld\n",
			WAL_segment->wals_flushed_log_seqno,
			WAL_segment->wals_flushed_offset,
			WAL_segment->wals_rollover_threshold);
	FPRINTF(outfile,
			"---------------------------------------------------------\n");
	FPRINTF(outfile, "Max ckpt interval=%ld, Current ckpt interval=%ld\n",
			WAL_segment->wals_max_ckpt_intrvl,
			WAL_segment->wals_cur_ckpt_intrvl);
	FPRINTF(outfile, "curr ckpt seqno=%ld, ckpt logname=%s\n",
			WAL_segment->wals_ckpted_log_seqno,
			WAL_segment->wals_ckpt_logname);
	FPRINTF(outfile, "ckpt log partition offset=%ld, ckpt offset=%ld\n",
			WAL_segment->wals_ckpt_log_p_offset,
			WAL_segment->wals_ckpted_offset);
	FPRINTF(outfile, "prev ckpt seqno=%ld, ckpt logname=%s\n",
			WAL_segment->wals_prev_ckpted_log_seqno,
			WAL_segment->wals_prev_ckpt_logname);
	FPRINTF(outfile, "ckpt log partition offset=%ld, ckpt offset=%ld\n",
			WAL_segment->wals_prev_ckpt_log_p_offset,
			WAL_segment->wals_prev_ckpted_offset);
	FPRINTF(outfile,
			"---------------------------------------------------------\n");
	FPRINTF(outfile,
			"WAL writer pid=%"SLONGFORMAT
			", pid of the last process acquring the WAL=%"SLONGFORMAT"\n",
			WAL_segment->wals_writer_pid, WAL_segment->wals_last_pid);
	FPRINTF(outfile,
			"---------------------------------------------------------\n");
	FPRINTF(outfile,
			"Total WAL IOs=%ld, Avg IO size=%d, next block seqno=%ld\n",
			WAL_segment->wals_IO_count,
			WAL_segment->wals_total_IO_bytes /
			(WAL_segment->wals_IO_count ? WAL_segment->wals_IO_count : 1),
			WAL_segment->wals_blkseqno);
	FPRINTF(outfile,
			"---------------------------------------------------------\n");
	FPRINTF(outfile,
			"Total commits=%"SLONGFORMAT", Number of group-commits=%"SLONGFORMAT
			", Avg grpc size=%f\n",
			WAL_segment->wals_commit_count, WAL_segment->wals_grpc_count,
			((WAL_segment->wals_commit_count * 1.0) /
			 (WAL_segment->wals_grpc_count ? WAL_segment->
			  wals_grpc_count : 1)));
	FPRINTF(outfile, "grpc wait micro-seconds =%"SLONGFORMAT"\n",
			WAL_segment->wals_grpc_wait_usecs);
	FPRINTF(outfile, "Current grpc block=%d\n",
			WAL_segment->wals_cur_grpc_blknum + 1);
	FPRINTF(outfile,
			"Group-commit block   Coordinator pid       Group size\n");
	for (i = 0; i < MAX_GRP_COMMITTERS; i++)
		FPRINTF(outfile,
				"        %d                  %"SLONGFORMAT"                     %d\n",
				i + 1, WAL_segment->wals_grpc_blks[i].grp_commit_coordinator,
				WAL_segment->wals_grpc_blks[i].grp_commit_size);
	FPRINTF(outfile,
			"---------------------------------------------------------\n");
	FPRINTF(outfile,
			"WAL segment acquire count=%"SLONGFORMAT", number of wal_put2() calls=%"
			SLONGFORMAT"\n",
			WAL_segment->wals_acquire_count, WAL_segment->wals_put_count);
	FPRINTF(outfile,
			"---------------------------------------------------------\n");
	FPRINTF(outfile,
			"Number of preallocated log files=%d, Current log file=%d\n",
			WAL_segment->wals_max_logfiles,
			WAL_segment->wals_cur_logfile + 1);
	if (WAL_segment->wals_max_logfiles > 0) {
		FPRINTF(outfile,
				"Number \tName \t\tPartitions \tPartition size \tBase offset \tFlags\n");
		for (i = 0; i < WAL_segment->wals_max_logfiles; i++) {
			logf = LOGF_INFO(i);
			FPRINTF(outfile, "%d \t%s \t\t%d \t%ld \t%ld \t%ud\n",
					i + 1, LOGF_NAME(logf), logf->logf_partitions,
					logf->logf_max_size, logf->logf_base_offset,
					logf->logf_flags);
		}
	}
	logf = &WAL_segment->wals_log_serial_file_info;
	if (logf->logf_name_offset > 0)
		FPRINTF(outfile,
				"Serial log file name=%s, size=%ld, fname seqno=%ld\n",
				LOGF_NAME(logf), logf->logf_max_size, logf->logf_fname_seqno);
	logf = &WAL_segment->wals_log_ovflow_file_info;
	if (logf->logf_name_offset > 0)
		FPRINTF(outfile,
				"Overflow log file name=%s, size=%ld, fname seqno=%ld\n",
				LOGF_NAME(logf), logf->logf_max_size, logf->logf_fname_seqno);
	FPRINTF(outfile,
			"---------------------------------------------------------\n");

	if (acquire_flag)
		WALC_release(WAL_handle);
}
