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

#ifndef IPSERVER_IPC_H
#define IPSERVER_IPC_H

#include <stdio.h>
#include "../jrd/common.h"
#include "../jrd/thd.h"

#define ALLOC(type)		ALLI_block (type, 0)
#define ALLOCV(type, count)	ALLI_block (type, count)

#ifdef BUFSIZ
const USHORT BLOB_LENGTH		= BUFSIZ - 20;
#else
const USHORT BLOB_LENGTH		= 1024 - 20;
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

const USHORT IPS_DEF_NUM_CLI		= 10;	/* default clients per mapped file */
// const USHORT IPS_DEF_NUM_MAPS	= 10;	/* default maps available */
const USHORT IPS_DEF_PAGES_PER_CLI	= 8;	/* default 1k pages space per client */

const USHORT IPS_MIN_NUM_CLI		= 1;	/* min clients per mapped file */
// const USHORT IPS_MIN_NUM_MAPS	= 1;	/* min maps available */
const USHORT IPS_MIN_PAGES_PER_CLI	= 1;	/* min 1k pages space per client */

const USHORT IPS_MAX_NUM_CLI		= 64;	/* max clients per mapped file */
// const USHORT IPS_MAX_NUM_MAPS	= 64;	/* max maps available */
const USHORT IPS_MAX_PAGES_PER_CLI	= 16;	/* max 1k pages space per client */

#define	IPS_PACK_PARAMS(x,p,m,u) (((ULONG)(x) << 24) + ((ULONG)(p) << 16) + ((m) << 8) + (u) + IP_BIAS)
#define	IPS_UNPACK_MAX_USERS(n)	(((ULONG)(n - IP_BIAS) >> 24) & 0xFF)
#define	IPS_UNPACK_PAGES(n)	(((ULONG)(n - IP_BIAS) >> 16) & 0xFF)
#define	IPS_UNPACK_MAPNUM(n)	(((ULONG)(n - IP_BIAS) >> 8) & 0xFF)
#define	IPS_UNPACK_USERNUM(n)	((n - IP_BIAS) & 0xFF)

struct bid {
	ULONG bid_relation_id;		/* Relation id (or null) */
	ULONG bid_number;			/* Record number */
};
typedef bid* BID;

/* Block types */

struct blk;

#ifndef INCLUDE_FB_BLK
#include "../include/old_fb_blk.h"
#endif

/* Block types */

struct cnct {
	int *cnct_read_pipe;		/* this is really a FILE pointer */
	int *cnct_write_pipe;		/* this is really a FILE pointer */
};
typedef cnct* CNCT;

/* mapped file structure */

struct ipm {
	struct ipm *ipm_next;		/* pointer to next one */
	USHORT ipm_count;			/* slots in use */
	USHORT ipm_number;			/* mapped area number */
	HANDLE ipm_handle;			/* handle of mapped memory */
	USHORT ipm_flags;			/* flag word */
	LPVOID ipm_address;			/* address of mapped memory */
	UCHAR ipm_ids[IPS_MAX_NUM_CLI];
	/* ids */
};
typedef ipm* IPM;

/* mapped structure flags */
const USHORT IPMF_SERVER_SHUTDOWN	= 1;	/* server has shut down */

/* thread connection control block */

struct icc
{
	blk			icc_header;
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
};
typedef icc* ICC;

/* icc structure flags */
const USHORT ICCF_SHUTDOWN			= 1;	/* shutdown in progress */
const USHORT ICCF_SERVER_SHUTDOWN	= 2;	/* server has shut down */
const USHORT ICCF_UNMAP_CLIENT		= 4;	/* client maps must be shut down */

/* database block */

struct idb
{
	blk						idb_header;
	struct icc*				idb_thread;			/* back pointer to thread */
	FB_API_HANDLE			idb_handle;			/* database handle */
	struct itr*				idb_transactions;	/* linked list of transactions */
	struct irq*				idb_requests;		/* compiled requests */
	struct ipserver_isr*	idb_sql_requests;	/* SQL requests */
	struct ivnt*			idb_events;			/* events */
	struct idb*				idb_next;			/* pointer to next db */
	int*					idb_status_vector;	/* pointer to status vector */
	USHORT					idb_flags;			/* flags (see below) */
	SCHAR*					idb_setjmp;
};
typedef idb* IDB;

const USHORT IDBF_DATABASE_ATTACHMENT	= 1;
const USHORT IDBF_SERVICE_ATTACHMENT	= 2;
const USHORT IDBF_SERVER_SHUTDOWN		= 4;

/* transaction block */

struct itr
{
	blk			itr_header;
	struct idb*	itr_idb;
	struct itr*	itr_next;
	struct ibl*	itr_blobs;
	FB_API_HANDLE itr_handle;
	USHORT		itr_flags;
};
typedef itr* ITR;

const USHORT ITR_limbo	= 1;

/* blob control block */

struct ibl
{
	blk			ibl_header;
	struct idb*	ibl_idb;
	struct itr*	ibl_itr;
	struct ibl*	ibl_next;
	FB_API_HANDLE ibl_handle;
	SLONG		ibl_offset;			/* Apparent (to user) offset in blob */
	USHORT		ibl_flags;
	UCHAR*		ibl_ptr;
	USHORT		ibl_length;
	USHORT		ibl_fragment_length;
	USHORT		ibl_buffer_length;
	UCHAR		ibl_buffer[1];
};
typedef ibl* IBL;

//const USHORT IBL_eof			= 1;
//const USHORT IBL_segment		= 2;
//const USHORT IBL_eof_pending	= 4;
const USHORT IBL_create			= 8;

/* request block */

struct irq {
	blk			irq_header;
	struct idb *irq_idb;
	struct tra *irq_itr;
	struct irq *irq_next;
	struct irq **irq_user_handle;
	FB_API_HANDLE irq_handle;
};
typedef irq* IRQ;

/* event structure */

struct ivnt {
	blk			ivnt_header;
	struct ivnt *ivnt_next;
	struct idb *ivnt_idb;
	FPTR_EVENT_CALLBACK ivnt_ast;
	void* ivnt_arg;
	HWND ivnt_window;
	SLONG ivnt_id;
	SLONG ivnt_handle;
};
typedef ivnt* IVNT;

/* remote SQL request */

struct ipserver_isr {
	blk		isr_header;
	struct ipserver_isr *isr_next;
	struct idb *isr_idb;
	struct itr *isr_itr;
	FB_API_HANDLE isr_handle;
	USHORT isr_rec_count;
	USHORT isr_max_recs;
	USHORT isr_eof_flag;
	USHORT isr_batch_flag;
	ISC_STATUS_ARRAY isr_status;
	UCHAR *isr_cursor;
	UCHAR *isr_packed;
};
typedef ipserver_isr* IPSERVER_ISR;

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

struct ips_string {
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
	const UCHAR* ips_sv_addr;			/* address of server buffer */
	UCHAR *ips_sv_buffer;		/* allocated local buffer */
};

const ULONG IPS_INPUT_BUFFER	= 1;	/* used for input from client */
const ULONG IPS_OUTPUT_BUFFER	= 2;	/* used for output to client */


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

struct ips_object {
	UCHAR *ips_handle;			/* handle of object */
	ULONG ips_parameter;		/* a parameter, used if needed */
};

const USHORT IPS_ATTACH_NAME	= 0;	/* use controller 0 for db name */
const USHORT IPS_ATTACH_DPB		= 1;	/* use controller 1 for dpb */
const USHORT IPS_ATTACH_EXPANDED= 2;	/* use controller 2 for exp name */

const USHORT IPS_CREATE_NAME	= 0;	/* use controller 0 for db name */
const USHORT IPS_CREATE_DPB		= 1;	/* use controller 1 for dpb */
const USHORT IPS_CREATE_EXPANDED= 2;	/* use controller 2 for exp name */

const USHORT IPS_ATTACH_SVC_NAME= 0;	/* use controller 0 for service name */
const USHORT IPS_ATTACH_SVC_SPB	= 1;	/* use controller 1 for spb */
const USHORT IPS_START_SVC_SPB	= 2;	/* use controller 2 for start spb */

const USHORT IPS_CLOSE_BLOB_REM	= 0;	/* use controller 0 for remainders */

const USHORT IPS_INFO_ITEMS		= 0;	/* use controller 0 for items */
const USHORT IPS_INFO_DATA		= 1;	/* use controller 1 for data */

const USHORT IPS_QUERY_RECV_ITEMS	= 2;/* use controller 2 for recv items */

const USHORT IPS_PREPARE_TRANS	= 0;	/* use controller 0 for string */


/* structure used to compile requests */

struct ips_compile_req {
	UCHAR *ips_db_handle;		/* database handle */
	UCHAR *ips_rq_handle;		/* returned request handle */
};

const USHORT IPS_COMPILE_REQ	= 0;	/* use controller 0 for blr */


/* structure used by create/open blob (and create/open blob2) */

struct ips_blob {
	UCHAR *ips_db_handle;		/* database handle */
	UCHAR *ips_tr_handle;		/* transaction handle */
	UCHAR *ips_bl_handle;		/* returned blob handle */
	ULONG ips_rel_id;			/* returned relation ID */
	ULONG ips_bid_number;		/* returned blob ID number */
};

const USHORT IPS_BLOB_BPB		= 0;	/* use contorller 0 for bpb */


/* structure used for DDL operations */

struct ips_ddl {
	UCHAR *ips_db_handle;		/* database handle */
	UCHAR *ips_tr_handle;		/* transaction handle */
};

const USHORT IPS_DDL_DDL		= 0;	/* use controller 0 for ddl */


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

struct ips_dsql {
	UCHAR *ips_db_handle;		/* database handle */
	UCHAR *ips_tr_handle;		/* transaction handle (in/out) */
	UCHAR *ips_st_handle;		/* statement handle */
	ULONG ips_parameter;		/* sql dialect/fetch status/set cursor */
	USHORT ips_msg_type;		/* message type */
	USHORT ips_msg_out;			/* output message type */
	USHORT ips_rec_count;		/* packed records count */
};

const USHORT IPS_DSQL_EXEC_BLR	= 0;	/* use controller 0 for blr */
const USHORT IPS_DSQL_EXEC_MSG	= 1;	/* use controller 1 for message */

const USHORT IPS_DSQL_EXEC2_BLR_IN		= IPS_DSQL_EXEC_BLR;
const USHORT IPS_DSQL_EXEC2_MSG_IN		= IPS_DSQL_EXEC_MSG;
const USHORT IPS_DSQL_EXEC2_BLR_OUT		= 2;	/* use controller 2 for output blr */
const USHORT IPS_DSQL_EXEC2_MSG_OUT		= 3;	/* use controller 3 for output msg */

const USHORT IPS_DSQL_EXEC_IMMED_SQL	= 0;	/* use controller 0 for sql */
const USHORT IPS_DSQL_EXEC_IMMED_BLR	= 1;	/* use controller 1 for blr */
const USHORT IPS_DSQL_EXEC_IMMED_MSG	= 2;	/* use controller 2 for message */

const USHORT IPS_DSQL_EXEC_IMMED2_SQL	= IPS_DSQL_EXEC_IMMED_SQL;
const USHORT IPS_DSQL_EXEC_IMMED2_BLR_IN	= IPS_DSQL_EXEC_IMMED_BLR;
const USHORT IPS_DSQL_EXEC_IMMED2_MSG_IN	= IPS_DSQL_EXEC_IMMED_MSG;
const USHORT IPS_DSQL_EXEC_IMMED2_BLR_OUT	= 3;	/* use controller 3 for output blr */
const USHORT IPS_DSQL_EXEC_IMMED2_MSG_OUT	= 4;	/* use controller 4 for output msg */

const USHORT IPS_DSQL_FETCH_BLR		= 0;	/* use controller 0 for blr */
const USHORT IPS_DSQL_FETCH_MSG		= 1;	/* use controller 1 for message */

const USHORT IPS_DSQL_INSERT_BLR	= 0;	/* use controller 0 for blr */
const USHORT IPS_DSQL_INSERT_MSG	= 1;	/* use controller 1 for message */

const USHORT IPS_DSQL_PREP_STRING	= 0;	/* use controller 0 for string */
const USHORT IPS_DSQL_PREP_ITEMS	= 1;	/* use controller 1 for items */
const USHORT IPS_DSQL_PREP_BUFFER	= 2;	/* use controller 2 for buffer */

const USHORT IPS_DSQL_SET_CURSOR	= 0;	/* use controller 0 for name */


/* structure used to get/put blob segments */

struct ips_segment {
	UCHAR *ips_bl_handle;		/* blob handle */
	USHORT ips_length;			/* returned actual length */
};

const USHORT IPS_BLOB_SEGMENT	= 0;	/* use controller 0 for segment */


/* structure used to get/put array slices */

struct ips_slice {
	UCHAR *ips_db_handle;		/* database handle */
	UCHAR *ips_tr_handle;		/* transaction handle */
	ULONG ips_rel_id;			/* array rel id */
	ULONG ips_number;			/* array number */
	ULONG ips_length;			/* returned actual length */
};

const USHORT IPS_SLICE_SDL		= 0;	/* use controller 0 for sdl */
const USHORT IPS_SLICE_PARAM	= 1;	/* use controller 1 for parameters */
const USHORT IPS_SLICE_BUFFER	= 2;	/* use controller 2 for slice */


/* structure for queueing events */

struct ips_que_events {
	UCHAR *ips_db_handle;		/* database handle */
	ULONG ips_event_id;			/* returned event id */
	HWND ips_event_hwnd;		/* window handle to return to */
	FPTR_EVENT_CALLBACK ips_ast;	/* ast address */
	UCHAR* ips_arg;				/* ast arg */
};

const USHORT IPS_QUEUE_EVENT	= 0;	/* use controller 0 for event */


/* structure for send/receive message */

struct ips_request {
	UCHAR *ips_tr_handle;		/* transaction handle */
	UCHAR *ips_rq_handle;		/* request handle */
	USHORT ips_msg_type;		/* message type */
	USHORT ips_req_level;		/* request level */
#ifdef SCROLLABLE_CURSORS
	USHORT ips_direction;		/* direction */
	ULONG ips_offset;			/* offset */
#endif
};

const USHORT IPS_SEND_MESSAGE		= 0;	/* use controller 0 for message */
const USHORT IPS_RECEIVE_MESSAGE	= 0;	/* use controller 0 for message */


/* structure used to reconnect transaction */

struct ips_reconnect {
	UCHAR *ips_db_handle;		/* transaction handle */
	UCHAR *ips_tr_handle;		/* transaction handle */
};

const USHORT IPS_RECONNECT_ID	= 0;	/* use controller 0 for id */


/* structure used to seek into a blob */

struct ips_seek_blob {
	UCHAR *ips_bl_handle;		/* blob handle */
	ULONG ips_offset;			/* offset into blob */
	ULONG ips_result;			/* returned result */
	USHORT ips_mode;			/* seek mode */
};


/* structure used to start transactions */

struct ips_start_trans {
	UCHAR *ips_tr_handle;		/* returned transaction handle */
	USHORT ips_db_count;		/* number of db's in transaction */
};


/* structure used for transact request */

struct ips_transact_request {
	UCHAR *ips_db_handle;		/* database handle */
	UCHAR *ips_tr_handle;		/* transaction handle (in/out) */
};

const USHORT IPS_TRANS_REQ_BLR		= 0;	/* use controller 0 for blr */
const USHORT IPS_TRANS_REQ_IN_MSG	= 1;	/* use controller 1 for input msg */
const USHORT IPS_TRANS_REQ_OUT_MSG	= 2;	/* use controller 2 for output msg */


/*
    This structure is mapped to the start of the allocated
    communications area between the client and server.
*/

const USHORT MAX_IPS_STRINGS		= 5;

struct ips_comm_area {
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
};
typedef ips_comm_area* IPS;

/* size of mapped memory for a given client */

//#define	CLIENT_SERVER_AREA_SIZE	8192

/* size available for packing stuff in the mapped area */

//#define	CLIENT_SERVER_AVAIL (CLIENT_SERVER_AREA_SIZE - sizeof(ips_comm_area))

/* Windows names used to identify various named objects */

static const char* IPI_MAPPED_FILE_NAME	= "%sMappedArea%d";
static const char* IPI_CLIENT_SEM_NAME		= "%sClientSem%d_%d";
static const char* IPI_SERVER_SEM_NAME		= "%sServerSem%d_%d";
static const char* IPI_EVENT_NAME			= "%sEvent%d";
static const char* IPI_EVENT_CLASS			= "%sEventClass";
static const char* IPI_EVENT_THREAD		= "%sEventThread%d_%d";

/* local event queue structure */

struct eventq {
	struct eventq *evq_next;	/* next event */
	SLONG evq_id;				/* event ID */
	USHORT evq_length;			/* event length */
	UCHAR evq_string[1];			/* event string */
};
typedef eventq* EVENTQ;

/* interprocess database thread structure */

class tidb : public thdd 
{
public:
	struct rdb *tidb_database;
	ISC_STATUS *tidb_status_vector;
	UCHAR *tidb_setjmp;
};

typedef tidb* TIDB;


/* bias for connect messages */

const ULONG IP_BIAS		= 3333;

#endif /* IPSERVER_IPC_H */
