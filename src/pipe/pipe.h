/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		pipe.h
 *	DESCRIPTION:	Common descriptions
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

#ifndef _PIPE_PIPE_H_
#define _PIPE_PIPE_H_

#include <stdio.h>
#include "../jrd/common.h"

#define ALLOC(type)		ALLP_block (type, 0)
#define ALLOCV(type, count)	ALLP_block (type, count)

#ifdef BUFSIZ
#define BLOB_LENGTH		(BUFSIZ - 20)
#else
#define BLOB_LENGTH		(1024 - 20)
#endif

typedef enum {
	op_attach,					/* Attach database */
	op_create,					/* Create database */
	op_detach,					/* Detach database */

	op_compile,					/* Request based operations */
	op_start,
	op_start_and_send,
	op_send,
	op_receive,
	op_unwind,
	op_release,

	op_transaction,				/* Transaction operations */
	op_commit,
	op_rollback,
	op_prepare,
	op_reconnect,

	op_create_blob,				/* Blob operations */
	op_open_blob,
	op_get_segment,
	op_put_segment,
	op_cancel_blob,
	op_close_blob,

	op_info_database,			/* Information services */
	op_info_request,
	op_info_transaction,
	op_info_blob,

	op_prepare2,
	op_commit_retaining,
	op_open_blob2,
	op_create_blob2,
	op_ddl,
	op_que_events,
	op_cancel_events,
	op_put_slice,
	op_get_slice,
	op_seek_blob,
	op_get_segment_buf,			/* a buffered version of get_segment */
	op_put_segment_buf,			/* a buffered version of put_segment */

	op_allocate_stmt,			/* DSQL operations */
	op_execute,
	op_exec_immediate,
	op_fetch,
	op_free_stmt,
	op_prepare_stmt,
	op_set_cursor,
	op_info_sql,				/* Last opcode in V3.3 */

	op_insert,
	op_execute2,
	op_exec_immediate2,

	op_attach_service,
	op_detach_service,
	op_info_service,

	op_transact_request,
	op_drop_database,
	op_max,
	op_gateway_sync = 127
} P_OP;

typedef struct bid {
	ULONG bid_relation_id;		/* Relation id (or null) */
	ULONG bid_number;			/* Record number */
} *BID;

typedef int *HANDLE;

/* Block types */

#ifndef INCLUDE_FB_BLK
#include "../include/old_fb_blk.h"
#endif

/* Block types */

typedef struct cnct {
	int *cnct_read_pipe;		/* this is really a FILE pointer */
	int *cnct_write_pipe;		/* this is really a FILE pointer */
#ifdef VMS
	int cnct_misc_pipe_fds[2];
#endif
} *CNCT;

typedef struct rdb {
	struct blk rdb_header;
	HANDLE rdb_handle;			/* database handle */
	struct rtr *rdb_transactions;	/* linked list of transactions */
	struct rrq *rdb_requests;	/* compiled requests */
	struct rsr *rdb_sql_requests;	/* SQL requests */
	struct rdb *rdb_next;
	int *rdb_status_vector;
	SCHAR *rdb_setjmp;
#ifdef GATEWAY
	struct cnct rdb_pipes;		/* pipe connection */
#endif
} *RDB;

typedef struct rtr {
	struct blk rtr_header;
	struct rdb *rtr_rdb;
	struct rtr *rtr_next;
	struct rbl *rtr_blobs;
	HANDLE rtr_handle;
	USHORT rtr_flags;
} *RTR;

#define RTR_limbo	1

typedef struct rbl {
	struct blk rbl_header;
	struct rdb *rbl_rdb;
	struct rtr *rbl_rtr;
	struct rbl *rbl_next;
	HANDLE rbl_handle;
	SLONG rbl_offset;			/* Apparent (to user) offset in blob */
	USHORT rbl_flags;
	UCHAR *rbl_ptr;
	USHORT rbl_length;
	USHORT rbl_fragment_length;
	USHORT rbl_buffer_length;
	UCHAR rbl_buffer[1];
} *RBL;

#define RBL_eof		1
#define RBL_segment	2
#define RBL_eof_pending	4
#define RBL_create	8

typedef struct rrq {
	struct blk rrq_header;
	struct rdb *rrq_rdb;
	struct tra *rrq_rtr;
	struct rrq *rrq_next;
	struct rrq **rrq_user_handle;
	HANDLE rrq_handle;
} *RRQ;

typedef struct rvnt {
	struct blk rvnt_header;
	struct rvnt *rvnt_next;
	void (*rvnt_ast) ();
	void *rvnt_arg;
	SLONG rvnt_handle;
} *RVNT;

/* remote SQL request */

typedef struct rsr {
	struct blk rsr_header;
	struct rsr *rsr_next;
	struct rdb *rsr_rdb;
	HANDLE rsr_handle;
} *RSR;

#define BLKDEF(type, root, tail) type,
enum blk_t {
	type_MIN = 0,
#include "../pipe/blk.h"
	type_MAX
};
#undef BLKDEF

#endif /* _PIPE_PIPE_H_ */
