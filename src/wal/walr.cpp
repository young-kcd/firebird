/*
 *        PROGRAM:        JRD Write Ahead Log Reader
 *        MODULE:         walr.cpp
 *        DESCRIPTION:    Write Ahead Log Reader routines
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
#include <string.h>
#include "../wal/wal.h"
#include "gen/codes.h"
#include "../jrd/llio.h"
#include "../wal/walf_proto.h"
#include "../wal/walr_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/iberr_proto.h"
#include "../jrd/llio_proto.h"

static void dispose_walr_handle(WALR);
static SSHORT get_next_logname(WALRS, SCHAR **, SLONG *);
static SSHORT log_close(WALR);
static SSHORT log_open(ISC_STATUS *, WALRS, SCHAR *, SLONG, SLONG);
static SSHORT read_next_block(ISC_STATUS *, WALRS);


SSHORT WALR_close( ISC_STATUS * status_vector, WALRS * WALRS_handle)
{
/**************************************
 *
 *        W A L R _ c l o s e
 *
 **************************************
 *
 * Functional description
 *        Close the current log file, deallocate all the memory associated
 *        with the WAL reading handle.
 *    
 **************************************/

	log_close((*WALRS_handle)->walrs_walr);
	gds__free((SLONG *) (*WALRS_handle)->walrs_walr);
	gds__free((SLONG *) (*WALRS_handle)->walrs_dbname);
	gds__free((SLONG *) (*WALRS_handle));
	*WALRS_handle = NULL;

	return FB_SUCCESS;
}


SSHORT WALR_fixup_log_header(ISC_STATUS * status_vector, WALRS WALRS_handle)
{
/**************************************
 *
 *        W A L R _ f i x u p _ l o g _ h e a d e r
 *
 **************************************
 *
 * Functional description
 *        Called after the last valid block of the current log file has
 *        been read for short-term recovery.  So we are already positioned
 *        at the last valid block and we can update the log file header 
 *        information. 
 *    
 **************************************/
	WALR WALR_handle;

	WALR_handle = WALRS_handle->walrs_walr;
	if (!(WALR_handle->walr_log_header->walfh_flags & WALFH_OPEN))
		return FB_SUCCESS;			/* Everything should be fine anyway. */

	WALR_handle->walr_log_header->walfh_length = WALR_handle->walr_offset +
		BLKHDR->walblk_hdr_blklen;
	WALR_handle->walr_log_header->walfh_hibsn =
		WALR_handle->walr_next_bsn - 1;
	WALR_handle->walr_log_header->walfh_flags &= ~WALFH_OPEN;

	return WALF_update_log_header(status_vector, WALR_handle->walr_logname,
								  WALR_handle->walr_log_header->
								  walfh_log_partition_offset,
								  WALR_handle->walr_log_header,
								  WALR_handle->walr_fd);
}


SSHORT WALR_get(ISC_STATUS * status_vector,
				WALRS WALRS_handle,
				UCHAR * logrec,
				USHORT * len, SLONG * log_seqno, SLONG * log_offset)
{
/**************************************
 *
 *        W A L R _ g e t
 *
 **************************************
 *
 * Functional description
 *        Get the next log record from the log file.  It automatically
 *        handles getting the next block (which in turn may require
 *        rolling over to next log file) and extracting the log 
 *        record from it.   

 *        Returns FB_SUCCESS, if the next log record is found.  It sets the
 *        parameters log_seqno and log_offset which were returned by the
 *        WAL subsystem when this record was originally put in the WAL.
 *
 *        It returns -1 when end of log record stream is reached.
 *        This could logically and optionally happen when 
 *        reading a block with higher timestamp value than the one set
 *        by user through WALR_open() call.
 *
 **************************************/
	WALR WALR_handle;
	WALREC_HDR header;
	int reclen, remaining_block_bytes, remaining_logrec_bytes;
	UCHAR *p, *q;
	SSHORT ret;

	WALR_handle = WALRS_handle->walrs_walr;
	remaining_block_bytes = BLKHDR->walblk_hdr_len - REC_OFFSET;
	if (remaining_block_bytes < (int) REC_HDROVHD) {
		/* Go to the next block */

		if ((ret = read_next_block(status_vector, WALRS_handle)) != FB_SUCCESS)
			return ret;
		WALR_handle = WALRS_handle->walrs_walr;
		remaining_block_bytes = BLKHDR->walblk_hdr_len - REC_OFFSET;
	}

	memcpy((SCHAR *) & header, &WALR_handle->walr_buffer[REC_OFFSET],
		   REC_HDROVHD);
	reclen = header.walrec_len;

	if (reclen == 0) {			/* 0-length checkpoint record */
		/* skip over it */

		REC_OFFSET += REC_HDROVHD;
		return WALR_get(status_vector, WALRS_handle, logrec, len,
						log_seqno, log_offset);
	}

	*log_seqno = WALR_handle->walr_log_header->walfh_seqno;
	*log_offset = WALR_handle->walr_offset + REC_OFFSET;
	*len = reclen;
	p = &WALR_handle->walr_buffer[REC_OFFSET + REC_HDROVHD];
	q = logrec;

	remaining_logrec_bytes = remaining_block_bytes - REC_HDROVHD;
	if (reclen > remaining_logrec_bytes) {
		/* The record is incomplete */

		WAL_FORMAT_ERROR(status_vector, gds_logr_incomplete,
						 WALR_handle->walr_logname, *log_offset);
		return FB_FAILURE;
	}
	else {
		memcpy(q, p, reclen);
		REC_OFFSET += REC_HDROVHD + reclen;
	}

	return FB_SUCCESS;
}


SSHORT WALR_get_blk_timestamp(WALRS WALRS_handle, SLONG * timestamp)
{
/**************************************
 *
 *        W A L R _ g e t _ b l k _ t i m e s t a m p
 *
 **************************************
 *
 * Functional description
 *        Returns the timestamp of the current block being read.
 *
 **************************************/

	if (WALRS_handle == NULL || WALRS_handle->walrs_walr == NULL
		|| BLKHDR == NULL) return FB_FAILURE;
	timestamp[0] = BLKHDR->walblk_hdr_timestamp[0];
	timestamp[1] = BLKHDR->walblk_hdr_timestamp[1];

	return FB_SUCCESS;
}


SSHORT WALR_open(ISC_STATUS* status_vector,
				 WALRS* WALRS_handle,
				 const SCHAR* dbname,
				 int numlogs,
				 SCHAR** lognames,
				 SLONG* log_partitions_offsets,
				 SLONG first_log_offset,
				 SLONG* timestamp,
				 bool dont_scan_tail_logs)
{
/**************************************
 *
 *        W A L R _ o p e n
 *
 **************************************
 *
 * Functional description
 *        Open a stream of log records on the set of given 
 *        log files and all subsequently created log files.  
 *        If 'dont_scan_tail_logs' parameter is set, stop 
 *        scan after the end of the last log file in the set.
 *        Also, set the scan upto passed timestamp, if any.
 *        Initialize and return the WALRS_handle.  Used for 
 *        full or incremental recovery.  
 *
 *        Returns FB_SUCCESS or FB_FAILURE.
 *        Returns -1 if end-of-log encounterd.
 *
 **************************************/
	struct walrs walrset;
	SSHORT ret;

	walrset.walrs_dbname = (SCHAR *) gds__alloc(strlen(dbname) + 1);
/* NOMEM: error return, FREE: error returns & WALR_close()  */
	if (!walrset.walrs_dbname)
		return FB_FAILURE;
	strcpy(walrset.walrs_dbname, dbname);
	walrset.walrs_max_logs = numlogs;
	walrset.walrs_cur_log = 0;
	walrset.walrs_lognames = lognames;
	walrset.walrs_log_partitions_offsets = log_partitions_offsets;
	walrset.walrs_flags = 0;
	if (timestamp != NULL) {
		walrset.walrs_timestamp[0] = timestamp[0];
		walrset.walrs_timestamp[1] = timestamp[1];
		walrset.walrs_flags |= WALRS_TIMESTAMP;
	}
	if (dont_scan_tail_logs)
		walrset.walrs_flags |= WALRS_DONT_SCAN_TAIL_LOGS;

	ret = log_open(status_vector, &walrset, lognames[0],
				   log_partitions_offsets[0], first_log_offset);
	if (ret == FB_SUCCESS) {
		walrset.walrs_cur_log++;
		*WALRS_handle = (WALRS) gds__alloc(sizeof(struct walrs));
		/* NOMEM: error return, FREE: WALR_close() */
		if (!*WALRS_handle)
			ret = FB_FAILURE;
		else
			**WALRS_handle = walrset;
	}
	else
	 if (ret != FB_SUCCESS)
		gds__free((SLONG *) walrset.walrs_dbname);

	return ret;
}


static void dispose_walr_handle( WALR WALR_handle)
{
/**************************************
 *
 *        d i s p o s e _ w a l r _ h a n d l e
 *
 **************************************
 *
 * Functional description
 *        Deallocate memory associated with the WALR_handle.
 *
 **************************************/

	gds__free((SLONG *) WALR_handle->walr_logname);
	gds__free((SLONG *) WALR_handle->walr_read_ahead_buffer);
	gds__free((SLONG *) WALR_handle->walr_buffer);
	WALF_dispose_log_header(WALR_handle->walr_log_header);
}


static SSHORT get_next_logname(
							   WALRS WALRS_handle,
							   SCHAR ** logname, SLONG * log_partition_offset)
{
/**************************************
 *
 *        g e t _ n e x t _ l o g n a m e
 *
 **************************************
 *
 * Functional description
 *        Get the next logical logname and offset.  The logname may come
 *        from either the list of lognames supplied at the time of 
 *        WALR_open() or from the next logically linked log file.
 *
 *        Returns FB_FAILURE if no further logname can be found else
 *        returns FB_SUCCESS.
 *
 **************************************/
	WALR WALR_handle;

	if (WALRS_handle->walrs_cur_log >= WALRS_handle->walrs_max_logs) {
		if (WALRS_handle->walrs_flags & WALRS_DONT_SCAN_TAIL_LOGS)
			return FB_FAILURE;		/* No need to scan the linked log files */

		/* Look for the next logically linked file */

		WALR_handle = WALRS_handle->walrs_walr;
		if (strlen(WALR_handle->walr_log_header->walfh_next_logname) > 1) {
			*logname = WALR_handle->walr_log_header->walfh_next_logname;
			*log_partition_offset =
				WALR_handle->walr_log_header->walfh_next_log_partition_offset;
			return FB_SUCCESS;
		}
		else
			return FB_FAILURE;
	}
	else {
		/* Get the logname from the user-specified lognames list */

		*logname = WALRS_handle->walrs_lognames[WALRS_handle->walrs_cur_log];
		*log_partition_offset = WALRS_handle->walrs_log_partitions_offsets
			[WALRS_handle->walrs_cur_log];
		WALRS_handle->walrs_cur_log++;	/* for the next time */
		return FB_SUCCESS;
	}
}


static SSHORT log_close( WALR WALR_handle)
{
/**************************************
 *
 *        l o g _ c l o s e
 *
 **************************************
 *
 * Functional description
 *        Close the current log file, deallocate all the memory associated
 *        with the WAL reading handle.
 *    
 **************************************/

	LLIO_close(0, WALR_handle->walr_fd);
	dispose_walr_handle(WALR_handle);

	return FB_SUCCESS;
}


static SSHORT log_open(
					   ISC_STATUS * status_vector,
					   WALRS WALRS_handle,
					   SCHAR * logname,
					   SLONG log_partition_offset, SLONG offset)
{
/**************************************
 *
 *        l o g _ o p e n
 *
 **************************************
 *
 * Functional description
 *        Open a WAL file to read.  Seek to the desired offset.
 *        Set the scan upto passed timestamp, if any.
 *        Initialize and return the WALR_handle.  Used for recovery.  
 *
 *        Returns FB_SUCCESS or FB_FAILURE.
 *        Returns -1 if end-of-log encounterd.
 *
 **************************************/
	SLONG log_fd, read_len;
	WALFH log_header;
	WALR walr;
	SLONG blk_offset;
	USHORT rec_offset;
	WALBLK_HDR blkhdr;
	WALREC_HDR rechdr;
	SSHORT ret;
	SLONG sector_offset;
	UCHAR sector_bytes[LOG_SECTOR_SIZE * 2];
	USHORT read_ahead_bytes;
	WAL_TERMINATOR(log_terminator_block);

#define IO_ERR_RETURN	{WALF_dispose_log_header(log_header); return FB_FAILURE;}
#define FORMAT_ERR_RETURN(code, offset) {WALF_dispose_log_header(log_header);\
                   WAL_FORMAT_ERROR (status_vector, code, logname, offset);\
                   return FB_FAILURE;}

	log_header = (WALFH) gds__alloc(WALFH_LENGTH);
/* NOMEM: return error, 
   FREE:  error returns (dispose_log_header) & dispose_wal_handle */
	if (!log_header)
		return FB_FAILURE;

	ret =
		WALF_open_log_file(status_vector, WALRS_handle->walrs_dbname, logname,
						   log_partition_offset, log_header, &log_fd);
	if (ret != FB_SUCCESS) {
		gds__free((SLONG *) log_header);
		return FB_FAILURE;
	}

	WAL_TERMINATOR_SET(log_terminator_block);
/* Now position at the block containing the log record at the given offset */
	if (offset == 0L)
		blk_offset = log_header->walfh_offset;
	else {
		/* The passed 'offset' is an absolute offset of a WAL record in 
		   the log file.  Read the record header and then get the starting
		   position of its block */

		/* First read the whole sector containing the record header.  To 
		   support raw devices, read from a sector boundary */

		sector_offset = offset & ~((SLONG) (LOG_SECTOR_SIZE - 1));

		if (LLIO_read(status_vector, log_fd, logname,
					  log_partition_offset + sector_offset, LLIO_SEEK_BEGIN,
					  sector_bytes, sizeof(sector_bytes), &read_len))
			IO_ERR_RETURN;

		if (read_len < (offset - sector_offset + (SLONG) REC_HDROVHD))
			FORMAT_ERR_RETURN(gds_logr_header_small, offset)

				if (memcmp(log_terminator_block, sector_bytes, REC_HDROVHD) ==
					0) {
				/* end of log */
				WALF_dispose_log_header(log_header);
				return -1;
			}
		/* Now extract the actual record header */
		memcpy((SCHAR *) & rechdr, &sector_bytes[offset % LOG_SECTOR_SIZE],
			   REC_HDROVHD);
		rec_offset = rechdr.walrec_offset;	/* offset of record within the block */
		blk_offset = offset - rec_offset;	/* offset of block within the file */
	}

/* Now position and read the header of the starting block */

	if (log_header->walfh_flags & WALFH_RAW)
		read_ahead_bytes = LOG_SECTOR_SIZE;
	else
		read_ahead_bytes = BLK_HDROVHD;

	if (LLIO_read(status_vector, log_fd, logname,
				  log_partition_offset + blk_offset, LLIO_SEEK_BEGIN,
				  sector_bytes, read_ahead_bytes, &read_len))
		IO_ERR_RETURN;

	if (read_len == 0) {
		/* end of log */
		WALF_dispose_log_header(log_header);
		return -1;
	}

	if (read_len < (SLONG) BLK_HDROVHD)
		FORMAT_ERR_RETURN(gds_logb_small, blk_offset)

			memcpy((SCHAR *) & blkhdr, sector_bytes, BLK_HDROVHD);

	if (memcmp
		((SCHAR *) log_terminator_block, (SCHAR *) & blkhdr,
		 BLK_HDROVHD) == 0) {
		/* end of log */
		WALF_dispose_log_header(log_header);
		return -1;
	}

/* Now allocate and initialize the log reading handle. */

	walr = (WALR) gds__alloc(sizeof(struct walr));
/* NOMEM: return error, FREE: error return or handle close */
	if (!walr) {
		WALF_dispose_log_header(log_header);
		return FB_FAILURE;
	}
	walr->walr_logname = (SCHAR *) gds__alloc(strlen(logname) + 1);
/* NOMEM: return error, FREE: error return or log_close() */
	if (!walr->walr_logname) {
		gds__free(walr);
		WALF_dispose_log_header(log_header);
		return FB_FAILURE;
	}
	strcpy(walr->walr_logname, logname);
	walr->walr_log_partition_offset = log_partition_offset;
	walr->walr_fd = log_fd;
	walr->walr_next_bsn = blkhdr.walblk_hdr_bsn + 1;
	walr->walr_flags = 0;
	walr->walr_read_ahead_bytes = read_ahead_bytes;
	walr->walr_log_header = log_header;
	walr->walr_offset = blk_offset;
	walr->walr_read_ahead_buffer = (UCHAR *) gds__alloc(read_ahead_bytes);
/* NOMEM: return error, FREE: error return or log_close() */
	if (!walr->walr_read_ahead_buffer) {
		gds__free(walr->walr_logname);
		gds__free(walr);
		WALF_dispose_log_header(log_header);
		return FB_FAILURE;
	}

	walr->walr_next_blkhdr = (WALBLK_HDR *) walr->walr_read_ahead_buffer;
	memcpy(walr->walr_next_blkhdr, sector_bytes, read_ahead_bytes);
	walr->walr_buffer =
		(UCHAR *) gds__alloc(MAX_WALBUFLEN + read_ahead_bytes);
/* NOMEM: return error, FREE: error return or log_close() */
	if (!walr->walr_buffer) {
		gds__free(walr->walr_read_ahead_buffer);
		gds__free(walr->walr_logname);
		gds__free(walr);
		WALF_dispose_log_header(log_header);
		return FB_FAILURE;
	}

	walr->walr_blkhdr = (WALBLK_HDR *) walr->walr_buffer;	/* fixed for all subsequent blocks */
	walr->walr_blkhdr->walblk_hdr_blklen = 0;	/* Initialization for the subsequent read */

/* Now that the walr structure has been set up, set the handle to it */

	WALRS_handle->walrs_walr = walr;

/* Read ahead the first block in anticipation of subsequent WALR_get() calls. */
	if ((ret = read_next_block(status_vector, WALRS_handle)) != FB_SUCCESS) {
		/* log_close() free's all dangling allocations off of walr */
		log_close(walr);
		gds__free((SLONG *) walr);
		WALRS_handle->walrs_walr = NULL;
		return ret;
	}

	if (offset > 0) {
		walr->walr_rec_offset = rec_offset;
	}

	return FB_SUCCESS;
}


static SSHORT read_next_block( ISC_STATUS * status_vector, WALRS WALRS_handle)
{
/**************************************
 *
 *        r e a d _ n e x t _ b l o c k
 *
 **************************************
 *
 * Functional description
 *        Read a block from the current log file.  The block header 
 *        has already been read into 
 *        WALRS_handle->walrs_walr->walr_next_blkhdr.  
 *        Rolls over the reading to the next log file in sequence after the
 *        current one finishes.     
 *
 *        Returns -1 if eof (end of all logs or timestamp limit) reached.
 *
 **************************************/
	SSHORT ret;
	SLONG len, read_len;
	UCHAR *ptr;
	USHORT last_block_len, trailing_length;
	WALR WALR_handle;
	SCHAR *next_logname;
	SLONG next_log_partition_offset;
	SLONG *timestamp, *blk_timestamp;

	if (WALRS_handle->walrs_flags & WALRS_EOF_SCAN)
		return -1;				/* end of log records */

	WALR_handle = WALRS_handle->walrs_walr;
	if (WALR_handle->walr_flags & WALR_EOF) {
		/* Try to rollover to the next log file, if any. */

		if (get_next_logname(WALRS_handle, &next_logname,
							 &next_log_partition_offset) != FB_SUCCESS)
			return -1;			/* end of log records */

		ret = log_open(status_vector, WALRS_handle, next_logname,
					   next_log_partition_offset, 0L);
		if (ret == FB_SUCCESS) {
			LLIO_close(0, WALR_handle->walr_fd);
			dispose_walr_handle(WALR_handle);
			return FB_SUCCESS;
		}
		else
			return -1;			/* end of log records */
	}

	last_block_len = BLKHDR->walblk_hdr_blklen;

/* First get the next block header in the beginning of the buffer. */

	memcpy(WALR_handle->walr_buffer, WALR_handle->walr_read_ahead_buffer,
		   WALR_handle->walr_read_ahead_bytes);

/* Now read the remaining part of the log block, including the next 
   block header, if any */

	len = (int) BLKHDR->walblk_hdr_blklen;
	ptr = &WALR_handle->walr_buffer[WALR_handle->walr_read_ahead_bytes];
	if (LLIO_read(0, WALR_handle->walr_fd, 0, 0L, LLIO_SEEK_NONE,
				  ptr, len, &read_len) ||
		(read_len < (len - WALR_handle->walr_read_ahead_bytes))) {
		/* log block too small, we must have reached end-of-log at the time 
		   of crash. */

		return -1;				/* end of log records */
	}
	else {
		/* First check the consistency in terms of the trailing block length. */

		ptr = &WALR_handle->walr_buffer[len - BLK_TAILOVHD];
		memcpy((SCHAR *) & trailing_length, ptr, BLK_TAILOVHD);
		if ((int) trailing_length != len)
			return -1;			/* We have found an inconsistent block.  Assume this 
								   to be end of log because there might have been a crash 
								   after the previous block was properly written */

		if (read_len == (len - WALR_handle->walr_read_ahead_bytes)) {
			/* This may be the last block of this log file. */

			if (BLKHDR->walblk_hdr_bsn <
				WALR_handle->walr_log_header->walfh_hibsn) {
				/* unexpected end of log file. */

				WAL_FORMAT_ERROR(status_vector, gds_logf_unexpected_eof,
								 WALR_handle->walr_logname,
								 WALR_handle->walr_offset);
				return FB_FAILURE;
			}
			else
				WALR_handle->walr_flags |= WALR_EOF;	/* For the next time */
		}
		else if (read_len < len) {
			/* The next block header did not seem to have been written properly. */

			WALR_handle->walr_flags |= WALR_EOF;	/* For the next time */
		}
		else {
			/* We have read the the current block as well as the header of 
			   the next block.  Make sure that the next header looks okay. */

			memcpy((SCHAR *) WALR_handle->walr_read_ahead_buffer,
				   (SCHAR *) & WALR_handle->walr_buffer[len],
				   WALR_handle->walr_read_ahead_bytes);

			if (WALR_handle->walr_next_blkhdr->walblk_hdr_bsn !=
				WALR_handle->walr_next_bsn) WALR_handle->walr_flags |= WALR_EOF;	/* For the next time */
			WALR_handle->walr_next_bsn++;	/* expected seqno for the next block */

			/* Check the time stamp of the next block, if necessary */

			if (WALRS_handle->walrs_flags & WALRS_TIMESTAMP) {
				timestamp = WALRS_handle->walrs_timestamp;
				blk_timestamp =
					WALR_handle->walr_next_blkhdr->walblk_hdr_timestamp;
				if ((blk_timestamp[0] > timestamp[0])
					|| ((blk_timestamp[0] == timestamp[0])
						&& (blk_timestamp[1] > timestamp[1])))
					WALRS_handle->walrs_flags |= WALRS_EOF_SCAN;	/* For the next time */
			}
		}
	}

/* Now set the access fields */

	WALR_handle->walr_offset += last_block_len;
	REC_OFFSET = BLK_HDROVHD;

	return FB_SUCCESS;
}

