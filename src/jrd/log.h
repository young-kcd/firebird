/*
 *	PROGRAM:	REPLAY Debugging Utility
 *	MODULE:		log.h
 *	DESCRIPTION:	Definitions for logging OSRI calls
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

#ifndef JRD_LOG_H
#define JRD_LOG_H

#define LOG_VERSION1	1		/* version to correspond with v2.5 access method */
#define LOG_VERSION2	2		/* version to correspond with v3.0 access method */
#define LOG_VERSION3	3		/* version to correspond with v4.0 access method */

/* the log buffer length should be long enough to fit
   any OSRI call atomically to prevent multiuser bugs */

#define LOG_BUFFER_LENGTH	1024

/* replay log block */

class fblog : public pool_alloc<type_log>
{
    public:
	struct str *log_string;		/* string block allocated to hold log buffer */
	UCHAR *log_buffer;			/* points to beginning of string data */
	UCHAR *log_ptr;				/* current end of buffer for logging */
	void *log_file;				/* db-specific file to write */
};
typedef fblog *LOG;



/* this table is the possible log file entries */

enum log_t {
	log_attach = 0,				/* V2 OSRI calls */
	log_blob_info,
	log_cancel_blob,
	log_close_blob,
	log_commit,
	log_compile,
	log_create_blob,
	log_create_database,
	log_database_info,
	log_detach,
	log_get_segment,
	log_open_blob,
	log_prepare,
	log_put_segment,
	log_receive,
	log_reconnect,
	log_release_request,
	log_request_info,
	log_rollback,
	log_send,
	log_start_and_send,
	log_start,
	log_start_multiple,
	log_transaction_info,
	log_unwind,
	log_handle_returned,		/* handle returned from a call */
	log_statistics,				/* stats for sanity checking */
	log_error,					/* error from logging system */

	/* the following are V3 OSRI calls -- note that calls which
	   are even slightly changed are given a new number even if
	   the name of the call didn't change, such as attach.  This
	   is to achieve upward compatibility of the log files. */

	log_attach2,
	log_cancel_events,
	log_commit_retaining,
	log_create_blob2,
	log_create_database2,
	log_get_slice,
	log_open_blob2,
	log_prepare2,
	log_put_slice,
	log_que_events,
	log_blob_seek,

	/* the following are the same as the above calls but they stop recording
	   buffers that are supposed to be returned by the access method */

	log_get_segment2,
	log_get_slice2,
	log_receive2,
	log_blob_info2,
	log_database_info2,
	log_request_info2,
	log_transaction_info2,

	log_ddl,
	log_transact_request,
	log_drop_database,

	log_max
};

#endif /* JRD_LOG_H */
