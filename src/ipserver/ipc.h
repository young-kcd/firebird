/*
 *	PROGRAM:	Interprocess Interface definitions
 *	MODULE:		ipc.h
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

#ifndef _IPC_H_
#define _IPC_H_

#include <stdio.h>
#include "../jrd/common.h"
#include "../jrd/thd.h"
#include "../jrd/y_ref.h"

#define ALLOC(type)		ALLI_block (type, 0)
#define ALLOCV(type, count)	ALLI_block (type, count)

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
	op_rollback_retaining,
	op_open_blob2,
	op_create_blob2,
	op_ddl,
	op_que_events,
	op_cancel_events,
	op_put_slice,
	op_get_slice,
	op_seek_blob,

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

	op_service_attach,
	op_service_detach,
	op_service_info,
	op_service_start,

	op_transact_request,
	op_drop_database,

	op_ping,
	op_disconnect,
	op_max,
	op_gateway_sync = 127
} P_OP;

/* mapped file parameters */

#define	IPS_MAPPED_PER_CLI(p)	((ULONG)(p) * 1024L)
#define	IPS_MAPPED_FOR_CLI(p,x) ((ULONG)(x) * IPS_MAPPED_PER_CLI(p))
#define	IPS_MAPPED_SIZE(p,c)	((ULONG)(c) * IPS_MAPPED_PER_CLI(p))
#define	IPS_USEFUL_SPACE(p)	(IPS_MAPPED_PER_CLI(p) - sizeof( ips_comm_area))

#define	IPS_DEF_NUM_CLI		10	/* default clients per mapped file */
#define	IPS_DEF_NUM_MAPS	10	/* default maps available */
#define	IPS_DEF_PAGES_PER_CLI	8	/* default 1k pages space per client */

#define	IPS_MIN_NUM_CLI		1	/* min clients per mapped file */
#define	IPS_MIN_NUM_MAPS	1	/* min maps available */
#define	IPS_MIN_PAGES_PER_CLI	1	/* min 1k pages space per client */

#define	IPS_MAX_NUM_CLI		64	/* max clients per mapped file */
#define	IPS_MAX_NUM_MAPS	64	/* max maps available */
#define	IPS_MAX_PAGES_PER_CLI	16	/* max 1k pages space per client */

#define	IPS_PACK_PARAMS(x,p,m,u) (((ULONG)(x) << 24) + ((ULONG)(p) << 16) + ((m) << 8) + (u) + IP_BIAS)
#define	IPS_UNPACK_MAX_USERS(n)	(((ULONG)(n - IP_BIAS) >> 24) & 0xFF)
#define	IPS_UNPACK_PAGES(n)	(((ULONG)(n - IP_BIAS) >> 16) & 0xFF)
#define	IPS_UNPACK_MAPNUM(n)	(((ULONG)(n - IP_BIAS) >> 8) & 0xFF)
#define	IPS_UNPACK_USERNUM(n)	((n - IP_BIAS) & 0xFF)

typedef struct bid {
	ULONG bid_relation_id;		/* Relation id (or null) */
	ULONG bid_number;			/* Record number */
} *BID;

/* Block types */

#ifndef INCLUDE_FB_BLK
#include "../include/old_fb_blk.h"
#endif

/* Block types */

typedef struct cnct {
	int *cnct_read_pipe;		/* this is really a FILE pointer */
	int *cnct_write_pipe;		/* this is really a FILE pointer */
} *CNCT;

/* mapped file structure */

typedef struct ipm {
	struct ipm *ipm_next;		/* pointer to next one */
	USHORT ipm_count;			/* slots in use */
	USHORT ipm_number;			/* mapped area number */
	HANDLE ipm_handle;			/* handle of mapped memory */
	USHORT ipm_flags;			/* flag word */
	LPVOID ipm_address;			/* address of mapped memory */
	UCHAR ipm_ids[IPS_MAX_NUM_CLI];
	/* ids */
} *IPM;

/* mapped structure flags */

#define IPMF_SERVER_SHUTDOWN    1	/* server has shut down */

/* thread connection control block */

typedef struct icc
{
	struct blk	icc_header;
	struct icc*	icc_next;			/* pointer to next thread */
	struct idb*	icc_databases;		/* linked list of attachments */
	struct ipm*	icc_ipm;			/* pointer back to ipm */
	SSHORT		icc_file;			/* this thread's mapped file number */
	SSHORT		icc_slot;			/* this thread's slot number */
	DWORD		icc_thread_handle;	/* thread's handle */
	HANDLE		icc_file_handle;	/* mapped file's handle */
	HANDLE		icc_client_sem;		/* client's semaphore handle */
	HANDLE		icc_server_sem;		/* server's semaphore handle */
	HANDLE		icc_server_proc;	/* server's process handle */
	HANDLE		icc_client_proc;	/* client's process handle */
	DWORD		icc_server_id;		/* server's process id */
	DWORD		icc_client_id;		/* client's process id */
	SSHORT		icc_flags;			/* status bits */
	HANDLE		icc_waits[2];		/* wait for these handles */
	UCHAR*		icc_mapped_addr;	/* where the thread's mapped to */
} *ICC;

/* icc structure flags */

#define	ICCF_SHUTDOWN			1	/* shutdown in progress */
#define ICCF_SERVER_SHUTDOWN    2	/* server has shut down */
#define ICCF_UNMAP_CLIENT       4	/* client maps must be shut down */

/* database block */

typedef struct idb
{
	struct blk				idb_header;
	struct icc*				idb_thread;			/* back pointer to thread */
	FRBRD*					idb_handle;			/* database handle */
	struct itr*				idb_transactions;	/* linked list of transactions */
	struct irq*				idb_requests;		/* compiled requests */
	struct ipserver_isr*	idb_sql_requests;	/* SQL requests */
	struct ivnt*			idb_events;			/* events */
	struct idb*				idb_next;			/* pointer to next db */
	int*					idb_status_vector;	/* pointer to status vector */
	USHORT					idb_flags;			/* flags (see below) */
	SCHAR*					idb_setjmp;
} *IDB;

#define IDBF_DATABASE_ATTACHMENT 1
#define IDBF_SERVICE_ATTACHMENT  2
#define IDBF_SERVER_SHUTDOWN     4

/* transaction block */

typedef struct itr
{
	struct blk	itr_header;
	struct idb*	itr_idb;
	struct itr*	itr_next;
	struct ibl*	itr_blobs;
	FRBRD*		itr_handle;
	USHORT		itr_flags;
} *ITR;

#define ITR_limbo	1

/* blob control block */

typedef struct ibl
{
	struct blk	ibl_header;
	struct idb*	ibl_idb;
	struct itr*	ibl_itr;
	struct ibl*	ibl_next;
	FRBRD*		ibl_handle;
	SLONG		ibl_offset;			/* Apparent (to user) offset in blob */
	USHORT		ibl_flags;
	UCHAR*		ibl_ptr;
	USHORT		ibl_length;
	USHORT		ibl_fragment_length;
	USHORT		ibl_buffer_length;
	UCHAR		ibl_buffer[1];
} *IBL;

#define IBL_eof			1
#define IBL_segment		2
#define IBL_eof_pending	4
#define IBL_create		8

/* request block */

typedef struct irq {
	struct blk irq_header;
	struct idb *irq_idb;
	struct tra *irq_itr;
	struct irq *irq_next;
	struct irq **irq_user_handle;
	FRBRD	   *irq_handle;
} *IRQ;

/* event structure */

typedef struct ivnt {
	struct blk ivnt_header;
	struct ivnt *ivnt_next;
	struct idb *ivnt_idb;
	void (*ivnt_ast) ();
	void *ivnt_arg;
	HWND ivnt_window;
	SLONG ivnt_id;
	SLONG ivnt_handle;
} *IVNT;

/* remote SQL request */

typedef struct ipserver_isr {
	struct blk isr_header;
	struct ipserver_isr *isr_next;
	struct idb *isr_idb;
	struct itr *isr_itr;
	FRBRD *isr_handle;
	USHORT isr_rec_count;
	USHORT isr_max_recs;
	USHORT isr_eof_flag;
	USHORT isr_batch_flag;
	ISC_STATUS_ARRAY isr_status;
	UCHAR *isr_cursor;
	UCHAR *isr_packed;
} *IPSERVER_ISR;

#define BLKDEF(type, root, tail) type,
enum blk_t {
	type_MIN = 0,
#include "../ipserver/blk.h"
	type_MAX
};
#undef BLKDEF

/*
    This structure is used to move stuff between the ipc buffer(s)
    and the client's and server's memory.

    Three buffers are tracked - a client buffer, a server buffer and
    a buffer within the communications data structure.   For the client
    and server buffers, the address within that task plus the max size
    and current size are maintained. there is a flag indicating whether
    the buffer is used for input, output, both or neither.   For the
    comm buffer, a relative offset from the start of the comm area,
    the current area mapped in the comm area and its allocated size
    in the comm area are maintained.

    When the client needs to pass stuff to the server, space will be
    allocated in the comm area for all client buffers.   If there is
    insufficient space, the comm buffer will be filled by the client
    and emptied by the server as often as necessary to pass everything.
    the same process is carried out when the server responds for any
    data passed back to the client.

    There can be up to five of these in use for a call to the server.
    The flag words are used to indicate what, if anything, to copy for
    each of the five transfer blocks.
*/

typedef struct {
	ULONG ips_cl_size;			/* client buffer size */
	ULONG ips_sv_size;			/* server buffer size */
	ULONG ips_sv_alloced;		/* server buffer size allocated */
	ULONG ips_cl_copied;		/* client buffer copied so far */
	ULONG ips_sv_copied;		/* server buffer copied so far */
	ULONG ips_com_offset;		/* offset to comm buffer */
	ULONG ips_com_size;			/* size of comm buffer */
	ULONG ips_com_curr;			/* current mapped offset of comm buffer */
	ULONG ips_flags;			/* flags */
	UCHAR *ips_cl_addr;			/* address of client buffer */
	UCHAR *ips_sv_addr;			/* address of server buffer */
	UCHAR *ips_sv_buffer;		/* allocated local buffer */
} ips_string;

#define	IPS_INPUT_BUFFER	1	/* used for input from client */
#define	IPS_OUTPUT_BUFFER	2	/* used for output to client */


/*
   strucure used for attachments
    - attach database
    - attach service
    - create database

   and for info calls
    - blob info
    - database info
    - query service info
    - request info
    - sql request info
    - transaction info

   and for simple controls of various objects
    - cancel blob
    - cancel events
    - close blob
    - commit transaction
    - commit transaction (retaining)
    - detach database
    - detach service
    - drop database
    - dsql free statement
    - prepare transaction
    - release request
    - rollback transaction
    - unwind request
*/

typedef struct {
	UCHAR *ips_handle;			/* handle of object */
	ULONG ips_parameter;		/* a parameter, used if needed */
} ips_object;

#define	IPS_ATTACH_NAME		0	/* use controller 0 for db name */
#define	IPS_ATTACH_DPB		1	/* use controller 1 for dpb */
#define	IPS_ATTACH_EXPANDED	2	/* use controller 2 for exp name */

#define	IPS_CREATE_NAME		0	/* use controller 0 for db name */
#define	IPS_CREATE_DPB		1	/* use controller 1 for dpb */
#define	IPS_CREATE_EXPANDED	2	/* use controller 2 for exp name */

#define	IPS_ATTACH_SVC_NAME	0	/* use controller 0 for service name */
#define	IPS_ATTACH_SVC_SPB	1	/* use controller 1 for spb */
#define IPS_START_SVC_SPB	2	/* use controller 2 for start spb */

#define	IPS_CLOSE_BLOB_REM	0	/* use controller 0 for remainders */

#define	IPS_INFO_ITEMS		0	/* use controller 0 for items */
#define	IPS_INFO_DATA		1	/* use controller 1 for data */

#define	IPS_QUERY_RECV_ITEMS	2	/* use controller 2 for recv items */

#define	IPS_PREPARE_TRANS	0	/* use controller 0 for string */


/* structure used to compile requests */

typedef struct {
	UCHAR *ips_db_handle;		/* database handle */
	UCHAR *ips_rq_handle;		/* returned request handle */
} ips_compile_req;

#define	IPS_COMPILE_REQ		0	/* use controller 0 for blr */


/* structure used by create/open blob (and create/open blob2) */

typedef struct {
	UCHAR *ips_db_handle;		/* database handle */
	UCHAR *ips_tr_handle;		/* transaction handle */
	UCHAR *ips_bl_handle;		/* returned blob handle */
	ULONG ips_rel_id;			/* returned relation ID */
	ULONG ips_bid_number;		/* returned blob ID number */
} ips_blob;

#define	IPS_BLOB_BPB		0	/* use contorller 0 for bpb */


/* structure used for DDL operations */

typedef struct {
	UCHAR *ips_db_handle;		/* database handle */
	UCHAR *ips_tr_handle;		/* transaction handle */
} ips_ddl;

#define	IPS_DDL_DDL		0		/* use controller 0 for ddl */


/*
   structure used for most DSQL operations
    - allocate
    - execute
    - execute2
    - execute immediate
    - execute immediate2
    - fetch
    - insert
    - prepare
    - set cursor
*/

typedef struct {
	UCHAR *ips_db_handle;		/* database handle */
	UCHAR *ips_tr_handle;		/* transaction handle (in/out) */
	UCHAR *ips_st_handle;		/* statement handle */
	ULONG ips_parameter;		/* sql dialect/fetch status/set cursor */
	USHORT ips_msg_type;		/* message type */
	USHORT ips_msg_out;			/* output message type */
	USHORT ips_rec_count;		/* packed records count */
} ips_dsql;

#define	IPS_DSQL_EXEC_BLR	0	/* use controller 0 for blr */
#define	IPS_DSQL_EXEC_MSG	1	/* use controller 1 for message */

#define	IPS_DSQL_EXEC2_BLR_IN	IPS_DSQL_EXEC_BLR
#define	IPS_DSQL_EXEC2_MSG_IN	IPS_DSQL_EXEC_MSG
#define	IPS_DSQL_EXEC2_BLR_OUT	2	/* use controller 2 for output blr */
#define	IPS_DSQL_EXEC2_MSG_OUT	3	/* use controller 3 for output msg */

#define	IPS_DSQL_EXEC_IMMED_SQL	0	/* use controller 0 for sql */
#define	IPS_DSQL_EXEC_IMMED_BLR	1	/* use controller 1 for blr */
#define	IPS_DSQL_EXEC_IMMED_MSG	2	/* use controller 2 for message */

#define	IPS_DSQL_EXEC_IMMED2_SQL	IPS_DSQL_EXEC_IMMED_SQL
#define	IPS_DSQL_EXEC_IMMED2_BLR_IN	IPS_DSQL_EXEC_IMMED_BLR
#define	IPS_DSQL_EXEC_IMMED2_MSG_IN	IPS_DSQL_EXEC_IMMED_MSG
#define	IPS_DSQL_EXEC_IMMED2_BLR_OUT	3	/* use controller 3 for output blr */
#define	IPS_DSQL_EXEC_IMMED2_MSG_OUT	4	/* use controller 4 for output msg */

#define	IPS_DSQL_FETCH_BLR	0	/* use controller 0 for blr */
#define	IPS_DSQL_FETCH_MSG	1	/* use controller 1 for message */

#define	IPS_DSQL_INSERT_BLR	0	/* use controller 0 for blr */
#define	IPS_DSQL_INSERT_MSG	1	/* use controller 1 for message */

#define	IPS_DSQL_PREP_STRING	0	/* use controller 0 for string */
#define	IPS_DSQL_PREP_ITEMS	1	/* use controller 1 for items */
#define	IPS_DSQL_PREP_BUFFER	2	/* use controller 2 for buffer */

#define	IPS_DSQL_SET_CURSOR	0	/* use controller 0 for name */


/* structure used to get/put blob segments */

typedef struct {
	UCHAR *ips_bl_handle;		/* blob handle */
	USHORT ips_length;			/* returned actual length */
} ips_segment;

#define	IPS_BLOB_SEGMENT	0	/* use controller 0 for segment */


/* structure used to get/put array slices */

typedef struct {
	UCHAR *ips_db_handle;		/* database handle */
	UCHAR *ips_tr_handle;		/* transaction handle */
	ULONG ips_rel_id;			/* array rel id */
	ULONG ips_number;			/* array number */
	ULONG ips_length;			/* returned actual length */
} ips_slice;

#define	IPS_SLICE_SDL		0	/* use controller 0 for sdl */
#define	IPS_SLICE_PARAM		1	/* use controller 1 for parameters */
#define	IPS_SLICE_BUFFER	2	/* use controller 2 for slice */


/* structure for queueing events */

typedef struct {
	UCHAR *ips_db_handle;		/* database handle */
	ULONG ips_event_id;			/* returned event id */
	HWND ips_event_hwnd;		/* window handle to return to */
	UCHAR *ips_ast;				/* ast address */
	UCHAR *ips_arg;				/* ast arg */
} ips_que_events;

#define	IPS_QUEUE_EVENT		0	/* use controller 0 for event */


/* structure for send/receive message */

typedef struct {
	UCHAR *ips_tr_handle;		/* transaction handle */
	UCHAR *ips_rq_handle;		/* request handle */
	USHORT ips_msg_type;		/* message type */
	USHORT ips_req_level;		/* request level */
#ifdef SCROLLABLE_CURSORS
	USHORT ips_direction;		/* direction */
	ULONG ips_offset;			/* offset */
#endif
} ips_request;

#define	IPS_SEND_MESSAGE	0	/* use controller 0 for message */
#define	IPS_RECEIVE_MESSAGE	0	/* use controller 0 for message */


/* structure used to reconnect transaction */

typedef struct {
	UCHAR *ips_db_handle;		/* transaction handle */
	UCHAR *ips_tr_handle;		/* transaction handle */
} ips_reconnect;

#define	IPS_RECONNECT_ID	0	/* use controller 0 for id */


/* structure used to seek into a blob */

typedef struct {
	UCHAR *ips_bl_handle;		/* blob handle */
	ULONG ips_offset;			/* offset into blob */
	ULONG ips_result;			/* returned result */
	USHORT ips_mode;			/* seek mode */
} ips_seek_blob;


/* structure used to start transactions */

typedef struct {
	UCHAR *ips_tr_handle;		/* returned transaction handle */
	USHORT ips_db_count;		/* number of db's in transaction */
} ips_start_trans;


/* structure used for transact request */

typedef struct {
	UCHAR *ips_db_handle;		/* database handle */
	UCHAR *ips_tr_handle;		/* transaction handle (in/out) */
} ips_transact_request;

#define	IPS_TRANS_REQ_BLR	0	/* use controller 0 for blr */
#define	IPS_TRANS_REQ_IN_MSG	1	/* use controller 1 for input msg */
#define	IPS_TRANS_REQ_OUT_MSG	2	/* use controller 2 for output msg */


/*
    This structure is mapped to the start of the allocated
    communications area between the client and server.
*/

#define	MAX_IPS_STRINGS		5

typedef struct {
	ULONG ips_server_protocol;	/* server's protocol level */
	ULONG ips_client_protocol;	/* client's protocol level */
	ISC_STATUS_ARRAY ips_status;	/* status vector */
	ips_string ips_buffers[MAX_IPS_STRINGS];
	/* array of buffer controllers */
	USHORT ips_ops_count;		/* operations performed */
	USHORT ips_sequence;		/* transfer sequence number */
	USHORT ips_operation;		/* operation being performed */
	HANDLE ips_server_proc;		/* server's process handle */
	HANDLE ips_client_proc;		/* client's process handle */
	DWORD ips_server_id;		/* server's process id */
	DWORD ips_client_id;		/* client's process id */
	union {
		ips_object ips_op_object;
		ips_compile_req ips_op_compile;
		ips_blob ips_op_blob;
		ips_ddl ips_op_ddl;
		ips_dsql ips_op_dsql;
		ips_segment ips_op_segment;
		ips_slice ips_op_slice;
		ips_que_events ips_op_que_evnt;
		ips_request ips_op_request;
		ips_reconnect ips_op_recon_trans;
		ips_seek_blob ips_op_sk_blob;
		ips_start_trans ips_op_strt_trans;
		ips_transact_request ips_op_trans_req;
	} ips_operations;
	ULONG ips_data[1];			/* start of data area */
} ips_comm_area, *IPS;

/* size of mapped memory for a given client */

#define	CLIENT_SERVER_AREA_SIZE	8192

/* size available for packing stuff in the mapped area */

#define	CLIENT_SERVER_AVAIL (CLIENT_SERVER_AREA_SIZE - sizeof(ips_comm_area))

/* Windows names used to identify various named objects */

#define	IPI_MAPPED_FILE_NAME	"%sMappedArea%d"
#define	IPI_CLIENT_SEM_NAME		"%sClientSem%d_%d"
#define	IPI_SERVER_SEM_NAME		"%sServerSem%d_%d"
#define	IPI_EVENT_NAME			"%sEvent%d"
#define	IPI_EVENT_CLASS			"%sEventClass"
#define	IPI_EVENT_THREAD		"%sEventThread%d_%d"

/* local event queue structure */

typedef struct eventq {
	struct eventq *evq_next;	/* next event */
	SLONG evq_id;				/* event ID */
	USHORT evq_length;			/* event length */
	TEXT evq_string[1];			/* event string */
} *EVENTQ;

/* interprocess database thread structure */

typedef struct tidb {
	struct thdd tidb_thd_data;
	struct rdb *tidb_database;
	ISC_STATUS *tidb_status_vector;
	UCHAR *tidb_setjmp;
} *TIDB;


/* bias for connect messages */

#define	IP_BIAS	3333

#endif /* _IPC_H_ */
