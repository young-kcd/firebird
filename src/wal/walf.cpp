/*
 *        PROGRAM:        JRD Write Ahead Log File utilitles
 *        MODULE:         walf.c
 *        DESCRIPTION:    Write Ahead Log File utilities
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
#include "../jrd/gds.h"
#include "../wal/wal.h"
#include "../jrd/dsc.h"
#include "../jrd/llio.h"
#include "../wal/walf_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/iberr_proto.h"
#include "../jrd/llio_proto.h"
#include "../jrd/misc_proto.h"


SSHORT WALF_delink_log(ISC_STATUS * status_vector,
					   SCHAR * dbname,
					   SCHAR * logname, SLONG log_partition_offset)
{
/**************************************
 *
 *        W A L F _ d e l i n k _ l o g
 *
 **************************************
 *
 * Functional description
 *        Delink the given log from the chain of log files by 
 *        making the previous log name for the next log empty,
 *        i.e., after this call, the header of the next log has a
 *        'null' previous pointer. 
 *        If there is any error, return FB_FAILURE else return FB_SUCCESS.
 *        In case of an error, status_vector would be updated.
 *
 **************************************/
	SLONG log_fd, next_log_fd;
	SSHORT ret;
	WALFH log_header, next_log_header;
	SCHAR next_logname[MAXPATHLEN];
	SLONG next_log_partition_offset;

/* First open the current log to get its header */

	log_header = (WALFH) gds__alloc(WALFH_LENGTH);
/* NOMEM: error return, FREE: by error returns & WALF_dispose_log_header() */
	if (!log_header)
		return FB_FAILURE;
	ret = WALF_open_log_file(status_vector, dbname, logname,
						   log_partition_offset, log_header, &log_fd);
	if (ret != FB_SUCCESS) {
		gds__free((SLONG *) log_header);
		return FB_FAILURE;
	}

	if ((log_header->walfh_next_logname != NULL) &&
		(strlen(log_header->walfh_next_logname) > 0)) {
		/* Get the info about the next log file */

		strcpy(next_logname, log_header->walfh_next_logname);
		next_log_partition_offset =
			log_header->walfh_next_log_partition_offset;
		WALF_dispose_log_header(log_header);
		LLIO_close(0, log_fd);

		/* Open and update the next log file's previous log pointer */

		next_log_header = (WALFH) gds__alloc(WALFH_LENGTH);
		/* NOMEM: error return, FREE: by error returns & WALF_dispose_log_header() */
		if (!next_log_header)
			return FB_FAILURE;
		ret = WALF_open_log_file(status_vector, dbname, next_logname,
								 next_log_partition_offset, next_log_header,
								 &next_log_fd);
		if (ret != FB_SUCCESS) {
			gds__free((SLONG *) next_log_header);
			return FB_FAILURE;
		}
		*next_log_header->walfh_prev_logname = 0;
		WALF_update_log_header(status_vector, next_logname,
							   next_log_partition_offset, next_log_header,
							   next_log_fd);
		WALF_dispose_log_header(next_log_header);
		LLIO_close(0, next_log_fd);
	}
	else {
		WALF_dispose_log_header(log_header);
		LLIO_close(0, log_fd);
	}

	return FB_SUCCESS;
}


SSHORT WALF_delink_prev_log(ISC_STATUS * status_vector,
							SCHAR * dbname,
							SCHAR * logname, SLONG log_partition_offset)
{
/**************************************
 *
 *        W A L F _ d e l i n k _ p r e v _ l o g
 *
 **************************************
 *
 * Functional description
 *        Make the previous log name for the given logname empty,
 *        i.e., after this call, the header of the logname has a
 *        'null' previous pointer. 
 *        If there is any error, return FB_FAILURE else return FB_SUCCESS.
 *        In case of an error, status_vector would be updated.
 *
 **************************************/
	SLONG log_fd;
	SSHORT ret;
	WALFH log_header;

/* First open the log to get its header */

	log_header = (WALFH) gds__alloc(WALFH_LENGTH);
/* NOMEM: error return, FREE: by error returns & WALF_dispose_log_header() */
	if (!log_header)
		return FB_FAILURE;
	ret = WALF_open_log_file(status_vector, dbname, logname,
						   log_partition_offset, log_header, &log_fd);
	if (ret != FB_SUCCESS) {
		gds__free((SLONG *) log_header);
		return FB_FAILURE;
	}

	if ((log_header->walfh_prev_logname != NULL) &&
		(strlen(log_header->walfh_prev_logname) > 0)) {
		gds__free((SLONG *) log_header->walfh_prev_logname);
		log_header->walfh_prev_logname = NULL;
		WALF_update_log_header(status_vector, logname, log_partition_offset,
							   log_header, log_fd);
	}
	WALF_dispose_log_header(log_header);
	LLIO_close(0, log_fd);

	return ret;
}


void WALF_dispose_log_header( WALFH log_header)
{
/**************************************
 *
 *        W A L F _ d i s p o s e _ l o g _ h e a d e r
 *
 **************************************
 *
 * Functional description
 *        Free up memory taken up by the pointers in the 
 *        passed log_header.  Free up the log_header memory also.
 *
 **************************************/

	if (log_header->walfh_dbname)
		gds__free((SLONG *) log_header->walfh_dbname);
	if (log_header->walfh_prev_logname)
		gds__free((SLONG *) log_header->walfh_prev_logname);
	if (log_header->walfh_next_logname)
		gds__free((SLONG *) log_header->walfh_next_logname);
	gds__free((SLONG *) log_header);
}


bool WALF_get_linked_logs_info(ISC_STATUS * status_vector,
							   SCHAR * dbname,
							   SCHAR * starting_logname,
							   SLONG starting_log_partition_offset,
							   int *prev_logs_count,
							   SCHAR * last_logname,
							   SLONG * last_log_partition_offset,
							   SLONG * last_log_flags,
							   bool * any_log_to_be_archived)
{
/**************************************
 *
 *        W A L F _ g e t _ l i n k e d _ l o g s _ i n f o
 *
 **************************************
 *
 * Functional description
 *        Get the number of all the previous logfiles
 *        linked to the given starting_logname.  Also pass the information 
 *        about the last linked log file, if any. 
 *
 *        If any log file in the chain (including the starting logname)
 *        needs to be archived (i.e. its WALFH_KEEP_FOR_LONG_TERM_RECV 
 *        is set), the parameter any_log_to_be_archived would be set to 
 *        true else it would be set to false.
 *
 *        If there is any error, return FB_FAILURE else return FB_SUCCESS. 
 *        In case of error, status_vector would be updated.
 *
 **************************************/
	SCHAR *curr_name;
	SLONG curr_log_partition_offset;
	SCHAR *prev_name;
	SLONG prev_log_partition_offset;
	SLONG log_flags;
	SCHAR log_name1[MAXPATHLEN];
	SCHAR log_name2[MAXPATHLEN];
	SCHAR *temp_name;
	SLONG log_seqno;
	SLONG log_length;
	int log_count;
	SSHORT ret;

	log_flags = 0;
	if ((ret = WALF_get_log_info(status_vector, dbname, starting_logname,
								 starting_log_partition_offset,
								 &log_seqno, &log_length,
								 &log_flags)) != FB_SUCCESS) 
	{
		return false;
	}
	*any_log_to_be_archived = (log_flags & WALFH_KEEP_FOR_LONG_TERM_RECV);
	strcpy(log_name1, starting_logname);
	curr_name = log_name1;
	curr_log_partition_offset = starting_log_partition_offset;
	prev_name = log_name2;

	log_count = 0;
	for (;;) {
		if (WALF_get_next_log_info
			(status_vector, dbname, curr_name, curr_log_partition_offset,
			 prev_name, &prev_log_partition_offset, &log_seqno, &log_length,
			 &log_flags, -1) != FB_SUCCESS)
		{
			break;
		}
		log_count++;
		temp_name = prev_name;
		prev_name = curr_name;
		curr_name = temp_name;
		curr_log_partition_offset = prev_log_partition_offset;
		if (!(*any_log_to_be_archived) &&
			(log_flags & WALFH_KEEP_FOR_LONG_TERM_RECV))
		{
			*any_log_to_be_archived = true;
		}
	}

/* Now initialize the passed parameters with the oldest log info. */

	*prev_logs_count = log_count;
	strcpy(last_logname, curr_name);
	*last_log_partition_offset = curr_log_partition_offset;
	*last_log_flags = log_flags;

	return true;
}


SSHORT WALF_get_log_info(ISC_STATUS * status_vector,
						 SCHAR * dbname,
						 SCHAR * logname,
						 SLONG log_partition_offset,
						 SLONG * log_seqno,
						 SLONG * log_length,
						 SLONG * log_flag)
{
/**************************************
 *
 *        W A L F _ g e t _ l o g _ i n f o
 *
 **************************************
 *
 * Functional description
 *       Get the seqno, length and the header flag of the given log file.
 *
 *       If there is any error, return false else return true.
 *       In case of error, status_vector would be updated.
 *
 **************************************/
	SLONG log_fd;
	SSHORT ret;
	WALFH log_header;

	log_header = (WALFH) gds__alloc(WALFH_LENGTH);
/* NOMEM: error return, FREE: by error returns & WALF_dispose_log_header() */
	if (!log_header)
		return false;
	ret = WALF_open_log_file(status_vector, dbname, logname,
							 log_partition_offset, log_header, &log_fd);
	if (ret != FB_SUCCESS) {
		gds__free((SLONG *) log_header);
		return false;
	}

	*log_seqno = log_header->walfh_seqno;
	*log_length = log_header->walfh_length;
	*log_flag = log_header->walfh_flags;

	WALF_dispose_log_header(log_header);
	LLIO_close(0, log_fd);

	return ret;
}


SSHORT WALF_get_next_log_info(ISC_STATUS * status_vector,
							  SCHAR * dbname,
							  SCHAR * logname,
							  SLONG log_partition_offset,
							  SCHAR * next_logname,
							  SLONG * next_log_partition_offset,
							  SLONG * next_log_seqno,
							  SLONG * next_log_length,
							  SLONG * next_log_flags,
							  SSHORT direction)
{
/**************************************
 *
 *        W A L F _ g e t _ n e x t _ l o g _ i n f o 
 *
 **************************************
 *
 * Functional description
 *        Get the name, offset and flag of the next logfile.  
 *        If the direction is 1, the next logfile is chronologically
 *        next log file.  If the direction is -1, the next logfile is
 *        chronologically previous log file.
 *
 *        If the 'next' logfile name is empty, return -1.  If there
 *        is any error, return FB_FAILURE else return FB_SUCCESS.  In case 
 *        of error, status_vector would be updated.
 *
 **************************************/
	SLONG log_fd;
	int ret;
	WALFH log_header;

/* First open the current log to get the 'next' logname and offset */

	log_header = (WALFH) gds__alloc(WALFH_LENGTH);
/* NOMEM: error return, FREE: by error returns & WALF_dispose_log_header() */
	if (!log_header)
		return FB_FAILURE;

	ret = WALF_open_log_file(status_vector, dbname, logname,
							 log_partition_offset, log_header, &log_fd);
	if (ret != FB_SUCCESS) {
		gds__free((SLONG *) log_header);
		return FB_FAILURE;
	}

	if ((direction == 1) &&
		(log_header->walfh_next_logname != NULL) &&
		(strlen(log_header->walfh_next_logname) > 0)) {
		strcpy(next_logname, log_header->walfh_next_logname);
		*next_log_partition_offset =
			log_header->walfh_next_log_partition_offset;
	}
	else if ((direction == -1) &&
			 (log_header->walfh_prev_logname != NULL) &&
			 (strlen(log_header->walfh_prev_logname) > 0)) {
		strcpy(next_logname, log_header->walfh_prev_logname);
		*next_log_partition_offset =
			log_header->walfh_prev_log_partition_offset;
	}
	else
		ret = -1;

/* Now get the information about the next log file */

	if (ret != -1)
		ret = WALF_get_log_info(status_vector, dbname, next_logname,
								*next_log_partition_offset,
								next_log_seqno, next_log_length,
								next_log_flags);

	WALF_dispose_log_header(log_header);
	LLIO_close(0, log_fd);

	return ret;
}


SSHORT WALF_get_all_next_logs_info(ISC_STATUS * status_vector,
								   SCHAR * dbname,
								   SCHAR * starting_logname,
								   SLONG starting_log_partition_offset,
								   int max_logs_count,
								   SCHAR * lognames_buffer,
								   int *next_logs_count,
								   SCHAR ** next_logs_names,
								   SLONG * next_log_partitions_offsets,
								   SLONG * next_logs_seqnos,
								   SLONG * next_logs_lengths,
								   SLONG * next_logs_flags, SSHORT direction)
{
/**************************************
 *
 *        W A L F _ g e t _ a l l _ n e x t _ l o g s _ i n f o
 *
 **************************************
 *
 * Functional description
 *        Get the names, offsets and flags of all the logfiles
 *        linked to the given starting_logname. 
 *
 *        If the direction is 1, the next logfiles are chronologically
 *        next log files.  If the direction is -1, the next logfiles are
 *        chronologically previous log files.
 *
 *        Pass the information  back through the parameters which are 
 *        arrays, each element of which would hold info about one 
 *        'next' log file.  The parameter next_logs_count returns the 
 *        total number of linked log files.  The information about log 
 *        files is returned in the 'oldest log file first' order.
 *
 *        If the total number of next log files exceeds
 *        max_logs_count then the search stops there.  In that case,
 *        the user may call this routine again with the last
 *        logname and with new or extended buffers.
 *
 *        If there is any error, return FB_FAILURE else return FB_SUCCESS. 
 *        In case of error, status_vector would be updated.
 *
 **************************************/
	STK stack;
	SCHAR *curr_name;
	SLONG curr_log_partition_offset;
	SCHAR *next_name;
	SLONG next_log_partition_offset;
	SLONG next_log_seqno;
	SLONG next_log_length;
	SLONG next_log_flags;
	int log_count;
	struct log_info_stru {
		SCHAR *log_name;
		SLONG log_partition_offset;
		SLONG log_seqno;
		SLONG log_length;
		SLONG log_flags;
	} *log_info;

	curr_name = starting_logname;
	curr_log_partition_offset = starting_log_partition_offset;

/* Now collect info about all the 'next' log names. */

	stack = NULL;
	log_count = 0;
	next_name = lognames_buffer;
	for (;;) {
		/* Set the buffer address to get the next file name */

		if (WALF_get_next_log_info(status_vector, dbname, curr_name,
								   curr_log_partition_offset, next_name,
								   &next_log_partition_offset,
								   &next_log_seqno, &next_log_length,
								   &next_log_flags, direction) != FB_SUCCESS)
			break;

		if (direction == 1) {
			/* We are getting the names in the correct chronological order */

			*next_logs_names++ = next_name;
			*next_log_partitions_offsets++ = next_log_partition_offset;
			*next_logs_seqnos++ = next_log_seqno;
			*next_logs_lengths++ = next_log_length;
			*next_logs_flags++ = next_log_flags;
		}
		else {
			/* We are getting the log names in the reverse chronological order,
			   so put them on stack for later retrieval in the correct order */

			log_info =
				(struct log_info_stru *)
				gds__alloc(sizeof(struct log_info_stru));
			/* NOMEM: free stack & error return, FREE: by popping the stack below */
			if (!log_info) {
				while (log_info =
					   reinterpret_cast < log_info_stru * >(MISC_pop(&stack)))
					gds__free(log_info);
				return FB_FAILURE;
			}
			log_info->log_name = next_name;
			log_info->log_partition_offset = next_log_partition_offset;
			log_info->log_seqno = next_log_seqno;
			log_info->log_length = next_log_length;
			log_info->log_flags = next_log_flags;

			/* MISC_push() returns NULL on NOMEM:, FREE: by MISC_pop */
			if (!MISC_push((UCHAR *) log_info, &stack)) {
				gds__free(log_info);
				while (log_info =
					   reinterpret_cast < log_info_stru * >(MISC_pop(&stack)))
					gds__free(log_info);
				return FB_FAILURE;
			}
		}

		log_count++;
		if (log_count == max_logs_count)
			break;

		curr_name = next_name;
		curr_log_partition_offset = next_log_partition_offset;
		next_name = curr_name + strlen(curr_name) + 1;	/* slide in the passed buffer */
	}

	*next_logs_count = log_count;

	if (direction == 1)
		return FB_SUCCESS;			/* We are all set */

	while (stack) {
		/* By creating the correct ordering of information from the stack,    
		   initialize the passed parameters starting from the oldest log. */

		log_info = (struct log_info_stru *) MISC_pop(&stack);
		*next_logs_names++ = log_info->log_name;
		*next_log_partitions_offsets++ = log_info->log_partition_offset;
		*next_logs_seqnos++ = log_info->log_seqno;
		*next_logs_lengths++ = log_info->log_length;
		*next_logs_flags++ = log_info->log_flags;
		gds__free((SLONG *) log_info);
	}

	return FB_SUCCESS;
}


SSHORT WALF_init_p_log(ISC_STATUS * status_vector,
					   SCHAR * dbname,
					   SCHAR * logname, SLONG logsize, SSHORT num_partitions)
{
/**************************************
 *
 *        W A L F _ i n i t _ p _ l o g
 *
 **************************************
 *
 * Functional description
 *        Create and initialize a logname file with given number of 
 *        equal log partitions.  The space (length logsize bytes) for 
 *        the file is also allocated.
 *
 *        If num_partitions = 0, just allocate logsize bytes for the 
 *        given logfile and return.
 *
 *        If there is any error, return FB_FAILURE else return FB_SUCCESS.
 *        In case of error, status_vector would be updated.
 *
 **************************************/
	SLONG p_log_fd;
	int i;
	P_LOGFH p_log_header;

/* Since we allow partitions for the RAW devices only, it is
   okay to overwrite the file in that case. */

	if (LLIO_allocate_file_space(status_vector, logname, logsize,
								 (UCHAR) WAL_TERMINATOR_CHAR,
								 (num_partitions) ? TRUE : FALSE) != FB_SUCCESS)
		return FB_FAILURE;

	if (num_partitions <= 0)
		return FB_SUCCESS;

	if (LLIO_open(status_vector, logname, LLIO_OPEN_RW, FALSE, &p_log_fd))
		return FB_FAILURE;

	p_log_header = (P_LOGFH) gds__alloc(P_LOGFH_LENGTH);
/* NOMEM: error return, FREE: end of routine */
	if (!p_log_header)
		return FB_FAILURE;

	p_log_header->p_logfh_version1 = P_LOGFH_VERSION;
	p_log_header->p_logfh_version2 = 0;
	p_log_header->p_logfh_length = logsize;
	p_log_header->p_logfh_maxp = num_partitions;
	p_log_header->p_logfh_firstp_offset = P_LOGFH_LENGTH;
	p_log_header->p_logfh_sizep = PARTITION_SIZE(logsize, num_partitions);
	p_log_header->p_logfh_curp = -1;
	p_log_header->p_logfh_flags = 0L;
	strcpy(p_log_header->p_logfh_dbname, dbname);

	for (i = 0; i < 10; i++)
		p_log_header->p_logfh_reserved[i] = 0L;

	if (LLIO_write(status_vector, p_log_fd, logname, 0L, LLIO_SEEK_NONE,
				   (UCHAR *) p_log_header, P_LOGFH_LENGTH, 0)) {
		LLIO_close(0, p_log_fd);
		gds__free((SLONG *) p_log_header);
		return FB_FAILURE;
	}

	LLIO_close(0, p_log_fd);
	gds__free((SLONG *) p_log_header);

	return FB_SUCCESS;
}


SSHORT WALF_open_partitioned_log_file(ISC_STATUS * status_vector,
									  SCHAR * dbname,
									  SCHAR * logname,
									  P_LOGFH p_log_header, SLONG * p_log_fd)
{
/**************************************
 *
 *        W A L F _ o p e n _ p a r t i t i o n e d _ l o g _ f i l e
 *
 **************************************
 *
 * Functional description
 *        Open and read the header of the given partioiotned log file.
 *        Return the file handle through the parameter p_log_fd.
 *        If there is any error, return FB_FAILURE else return FB_SUCCESS.
 *        In case of error, status_vector would be updated.
 *
 **************************************/
	SLONG fd, read_len;
#define ERROR_RETURN(error) {if (fd>0) LLIO_close(0,fd); \
                      WAL_ERROR (status_vector, error, logname);\
                      return FB_FAILURE;}
#define IO_ERROR_RETURN	{if (fd>0) LLIO_close(0,fd); return FB_FAILURE;}

/* Open and position at the beginning of the partitioned log file. */

	if (LLIO_open(status_vector, logname, LLIO_OPEN_EXISTING_RW, FALSE, &fd))
		return FB_FAILURE;

	*p_log_fd = fd;

/* Now try to read the partitioned logfile header and take appropriate steps 
   afterwards. */

	if (LLIO_read(status_vector,
				  fd,
				  logname,
				  0L,
				  LLIO_SEEK_NONE,
				  reinterpret_cast < UCHAR * >(p_log_header),
				  P_LOGFH_LENGTH, &read_len)) IO_ERROR_RETURN;

	if (read_len < (SLONG) sizeof(struct p_logfh))
		ERROR_RETURN(gds_logh_small);
	if (p_log_header->p_logfh_version1 != P_LOGFH_VERSION)
		ERROR_RETURN(gds_logh_inv_version);

/* Make sure that the database name matches with the passed database name. */

	if (strcmp(dbname, p_log_header->p_logfh_dbname) != 0)
		ERROR_RETURN(gds_logh_diff_dbname);

	return FB_SUCCESS;
}


SSHORT WALF_open_log_file(ISC_STATUS * status_vector,
						  SCHAR * dbname,
						  SCHAR * logname,
						  SLONG log_partition_offset,
						  WALFH log_header, SLONG * log_fd)
{
/**************************************
 *
 *        W A L F _ o p e n _ l o g _ f i l e
 *
 **************************************
 *
 * Functional description
 *        Open and read the header of the given log file.  Return the 
 *        file handle through the parameter log_fd.
 *        If there is any error, return FB_FAILURE else return FB_SUCCESS.
 *        In case of error, status_vector would be updated.
 *
 **************************************/
	SLONG fd, read_len;
#define ERROR_RETURN(error) {if (fd>0) LLIO_close(0,fd); \
                      WAL_ERROR (status_vector, error, logname);\
                      return FB_FAILURE;}
#define IO_ERROR_RETURN	{if (fd>0) LLIO_close(0,fd); return FB_FAILURE;}

	if (LLIO_open(0, logname, LLIO_OPEN_EXISTING_RW, FALSE, &fd))
		return FB_FAILURE;

	*log_fd = fd;

/* Position at the beginning of the logical log file and read the header. */

	if (LLIO_read(status_vector, fd, logname,
				  log_partition_offset, LLIO_SEEK_BEGIN,
				  (UCHAR *) log_header, WALFH_LENGTH, &read_len))
		IO_ERROR_RETURN;

	if (read_len < (SLONG) sizeof(struct walfh))
		ERROR_RETURN(gds_logh_small);
	if (log_header->walfh_version != WALFH_VERSION)
		ERROR_RETURN(gds_logh_inv_version);

/* Extract the database name from the log_header and make sure that 
   it matches with the passed database name. */

	log_header->walfh_dbname = NULL;
	log_header->walfh_prev_logname = NULL;
	log_header->walfh_next_logname = NULL;
	WALF_upd_log_hdr_frm_walfh_data(log_header, log_header->walfh_data);
	if (strcmp(dbname, log_header->walfh_dbname) != 0) {
		if (log_header->walfh_dbname)
			gds__free((SLONG *) log_header->walfh_dbname);
		if (log_header->walfh_prev_logname)
			gds__free((SLONG *) log_header->walfh_prev_logname);
		if (log_header->walfh_next_logname)
			gds__free((SLONG *) log_header->walfh_next_logname);
		ERROR_RETURN(gds_logh_diff_dbname);
	}

	return FB_SUCCESS;
}


SSHORT WALF_set_log_header_flag(ISC_STATUS * status_vector,
								SCHAR * dbname,
								SCHAR * logname,
								SLONG log_partition_offset,
								SLONG flag,
								bool set)
{
/**************************************
 *
 *        W A L F _ s e t _ l o g _ h e a d e r _ f l a g
 *
 **************************************
 *
 * Functional description
 *        Update the walfh_flags field of the log header in the log 
 *        file by the passed flag setting(s).  If the parameter 'set'
 *        is true, the flag bit(s) would be set else they would be reset.
 *        If there is any error, return FB_FAILURE else return FB_SUCCESS.
 *        In case of error, status_vector would be updated.
 *
 **************************************/
	SLONG log_fd;
	SSHORT ret;
	WALFH log_header;

/* First open the log and get its header */

	log_header = (WALFH) gds__alloc(WALFH_LENGTH);
/* NOMEM: error return, FREE: error return or WALF_dispose_log_header() */
	if (!log_header)
		return FB_FAILURE;

	ret =
		WALF_open_log_file(status_vector, dbname, logname,
						   log_partition_offset, log_header, &log_fd);
	if (ret != FB_SUCCESS) {
		gds__free((SLONG *) log_header);
		return FB_FAILURE;
	}

/* Now set or reset the bits */

	if (set)
		log_header->walfh_flags |= flag;
	else
		log_header->walfh_flags &= ~(flag);

	ret = WALF_update_log_header(status_vector, logname, log_partition_offset,
								 log_header, log_fd);
	WALF_dispose_log_header(log_header);
	LLIO_close(0, log_fd);

	return ret;
}


SSHORT WALF_update_log_header(ISC_STATUS * status_vector,
							  SCHAR * logname,
							  SLONG log_partition_offset,
							  WALFH log_header, SLONG log_fd)
{
/**************************************
 *
 *        W A L F _ u p d a t e _ l o g _ h e a d e r
 *
 **************************************
 *
 * Functional description
 *        Rewrite the log header in the log file.
 *        If there is any error, return FB_FAILURE else return FB_SUCCESS.
 *        In case of error, status_vector would be updated.
 *
 **************************************/

	log_header->walfh_log_partition_offset = log_partition_offset;
	log_header->walfh_data_len =
		MISC_build_parameters_block(log_header->walfh_data,
									PARAM_BYTE(WALFH_dbname),
									PARAM_STRING(log_header->walfh_dbname),
									PARAM_BYTE(WALFH_prev_logname),
									PARAM_STRING(log_header->
												 walfh_prev_logname),
									PARAM_BYTE(WALFH_next_logname),
									PARAM_STRING(log_header->
												 walfh_next_logname),
									PARAM_BYTE(WALFH_end), 0);

/* Position at the beginning of the logical log file. */

	if (LLIO_write(status_vector, log_fd, logname,
				   log_header->walfh_log_partition_offset, LLIO_SEEK_BEGIN,
				   (UCHAR *) log_header, WALFH_LENGTH, 0))
		return FB_FAILURE;

	return FB_SUCCESS;
}


SSHORT WALF_update_partitioned_log_hdr(ISC_STATUS * status_vector,
									   SCHAR * logname,
									   P_LOGFH p_log_header, SLONG p_log_fd)
{
/**************************************
 *
 *        W A L F _ u p d a t e _ p a r t i t i o n e d _ l o g _ h d r
 *
 **************************************
 *
 * Functional description
 *        Rewrite the header of the log file containing partitions.
 *        If there is any error, return FB_FAILURE else return FB_SUCCESS.
 *        In case of error, status_vector would be updated.
 *
 **************************************/

/* Position at the beginning of the partitioned log file. */

	if (LLIO_write(status_vector, p_log_fd, logname, 0L, LLIO_SEEK_BEGIN,
				   (UCHAR *) p_log_header, (ULONG) P_LOGFH_LENGTH, 0))
		return FB_FAILURE;

	return FB_SUCCESS;
}


void WALF_upd_log_hdr_frm_walfh_data( WALFH log_header, UCHAR * walfh_data)
{
/**************************************
 *
 *         W A L F _ u p d _ l o g _ h d r _ f r m _ w a l f h _ d a t a 
 *
 **************************************
 *
 * Functional description
 *        Decode walfh_data and update the fields of log_header structure.
 *
 **************************************/
	UCHAR *p, *q;
	SCHAR filename[MAXPATHLEN];

	p = walfh_data;

	for (;;) {
		switch (*p++) {
		case WALFH_dbname:
			q = (UCHAR *) filename;
			while (*q++ = *p++);
			if (log_header->walfh_dbname != NULL)
				gds__free((SLONG *) log_header->walfh_dbname);
			STRING_DUP(log_header->walfh_dbname, filename);
			/* NOMEM: walfh_dbname is set to NULL, FREE: unknown */
			break;

		case WALFH_prev_logname:
			q = (UCHAR *) filename;
			while (*q++ = *p++);
			if (log_header->walfh_prev_logname != NULL)
				gds__free((SLONG *) log_header->walfh_prev_logname);
			STRING_DUP(log_header->walfh_prev_logname, filename);
			/* NOMEM: walfh_prev_logname is set to NULL, FREE: unknown */
			break;

		case WALFH_next_logname:
			q = (UCHAR *) filename;
			while (*q++ = *p++);
			if (log_header->walfh_next_logname != NULL)
				gds__free((SLONG *) log_header->walfh_next_logname);
			STRING_DUP(log_header->walfh_next_logname, filename);
			/* NOMEM: walfh_next_logname is set to NULL, FREE: unknown */
			break;

		case WALFH_end:
			return;
		}
	}
}
