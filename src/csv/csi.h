/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		csi.h
 *	DESCRIPTION:	Central Server Interface definitions
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
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#ifndef _CSV_CSI_H_
#define _CSV_CSI_H_

#include "../jrd/common.h"
#include "../jrd/thd.h"
#include "../jrd/isc.h"

#ifdef VMS
#define CSI_FILE		"[000000]isc_csv_gbl.%s"
#if !(defined MULTI_THREAD) || defined (__ALPHA)
#define CSV_SIGNAL              33
#define EVENT_SIGNAL            3
#endif
#endif

#ifdef UNIX
#include <signal.h>
#define EVENT_SIGNAL		SIGUSR2
#define CSI_FILE		"/usr/interbase/isc_csv.%s"
#endif

#ifdef WIN_NT
#define CSI_FILE		"/interbas/%s.csv"
#endif

#ifndef CSI_DEFAULT_SIZE
#define CSI_DEFAULT_SIZE	262144
#endif

#define CSI_EXTEND_SIZE		32768

#ifndef MAX_ARRAY_MESSAGE
#define MAX_ARRAY_MESSAGE	64000
#endif

#ifndef BLOB_LENGTH
#define BLOB_LENGTH		4096
#endif

#define PTR			SLONG
#define BASE 			((UCHAR*) CSS_header)
#define REL_PTR(item)		((UCHAR*) item - BASE)
#define ABS_PTR(item)		(BASE + item)
#define ACQUIRE			CSS_acquire()
#define RELEASE			CSS_release()

#define QUE_INIT(que)	{que.srq_forward = que.srq_backward = REL_PTR (&que);}
#define QUE_EMPTY(que)	(que.srq_forward == REL_PTR (&que))
#define QUE_NEXT(que)	ABS_PTR (que.srq_forward)

#define QUE_LOOP(header,que)	for (que = (SRQ*) QUE_NEXT (header);\
	que != &header; que = (SRQ*) QUE_NEXT ((*que)))


#define SIZE_SHIFT	2
#define FAST_ALLOC	16

/* Self-relative que block.  Offsets are from the block itself. */

typedef struct srq {
	PTR srq_forward;			/* Forward offset */
	PTR srq_backward;			/* Backward offset */
} SRQ;

/* Global section header */

#define CSI_VERSION		3
#define CSI_PROTOCOL_VERSION	1

typedef struct csh {
	SLONG csh_length;			/* Current length of global section */
	UCHAR csh_version;			/* Version number of global section */
	SRQ csh_processes;			/* Known processes */
	PTR csh_free;				/* Free blocks */
	PTR csh_current_process;	/* Process "acquiring" section */
	SLONG csh_mutex[2];			/* Mutex controlling access */
	SLONG csh_semid;			/* Semaphore id for UNIX */
	SLONG csh_process_number;	/* Assigned sequentially */
	UCHAR csh_semaphores[16];	/* Free semaphores */
	PTR csh_free_vector[FAST_ALLOC];
} *CSH;

/* Common block header */

typedef struct hdr {
	SLONG hdr_length;			/* Length of block */
	UCHAR hdr_type;				/* Type of block */
} HDR;

#define type_hdr	1
#define type_frb	2
#define type_prb	3
#define type_cnct	4
#define type_msg	5

/* Free blocks */

typedef struct frb {
	HDR frb_header;
	SLONG frb_next;				/* Next block */
} *FRB;

/* Process blocks */

typedef struct prb {
	HDR prb_header;
	USHORT prb_flags;			/* Misc */
	USHORT prb_protocol_version;	/* Protocol version */
	ULONG prb_group_id;			/* Group id for VMS */
	USHORT prb_semaphore;		/* Semaphore used for event on UNIX */
	SRQ prb_processes;			/* Process que ownerd by header */
	SRQ prb_connections;		/* Connections to server process */
	SRQ prb_messages;			/* Messages to be processed */
	SLONG prb_process_id;		/* Process id */
	SLONG prb_process_number;	/* Relative process number */
	EVENT_T prb_event[1];		/* Event on which to wait */
} *PRB;

#define PRB_client	0			/* Process is client */
#define PRB_server	1			/* Process is server (otherwise client) */
#define PRB_disconnect	2		/* Some connection was broken */
#define PRB_client_t1	4		/* Process is client type 1 */
#define PRB_server_t1	8		/* Process is server type 1 */
#define PRB_timeout	32			/* Process has waited too long for a message */
#define PRB_wait	64			/* Process is awaiting acquisition */
#define PRB_signal_me	128		/* Process is waiting for a signal */

/* Connection block */

typedef struct cnct {
	HDR cnct_header;
	PTR cnct_parent;			/* Client process block */
	PTR cnct_partner;			/* Server process lock */
	PTR cnct_mirror;			/* Mirror of connection */
	SRQ cnct_connections;		/* Que of parents connections */
} *CNCT;

/* Message block */

typedef enum {
	MSG_attach_database,
	MSG_blob_info,
	MSG_cancel_blob,
	MSG_close_blob,
	MSG_commit,
	MSG_commit_retaining,
	MSG_compile,
	MSG_create_blob,
	MSG_create_database,
	MSG_database_info,
	MSG_detach,
	MSG_get_segment,
	MSG_open_blob,
	MSG_prepare,
	MSG_put_segment,
	MSG_reconnect,
	MSG_receive,
	MSG_release,
	MSG_request_info,
	MSG_rollback,
	MSG_send,
	MSG_start_and_send,
	MSG_start_request,
	MSG_start_transaction,
	MSG_transaction_info,
	MSG_unwind,
	MSG_disconnect,
	MSG_response,
	MSG_que_events,
	MSG_cancel_events,
	MSG_event,
	MSG_alt_connection,
	MSG_util_cmd,
	MSG_query_connect,
	MSG_mdi_attach_db,
	MSG_ping,
	MSG_get_slice,
	MSG_put_slice,
	MSG_ddl,
	MSG_seek_blob,
	MSG_get_segment_buf,
	MSG_put_segment_buf,
	MSG_allocate_stmt,
	MSG_execute,
	MSG_execute_immediate,
	MSG_fetch,
	MSG_free_stmt,
	MSG_prepare_stmt,
	MSG_set_cursor,
	MSG_sql_info,
	MSG_insert,
	MSG_execute2,
	MSG_execute_immediate2,
	MSG_transact_request,
	MSG_drop_database
} MSG_T;

typedef struct csv_msg {
	HDR msg_header;
	SRQ msg_que;
	MSG_T msg_type;				/* Specific message type */
	PTR msg_connection;			/* Process sending message */
} *CSV_MSG;

/* Miscellaneous message formats */

typedef struct msg_att {
	struct msg msg_att_header;	/* Common message header */
	USHORT msg_att_file_length;	/* Length of file name */
	USHORT msg_att_dpb_length;	/* Length of database parameter block */
	USHORT msg_att_expanded_length;	/* Length of expanded file name */
	USHORT msg_att_type;		/* Create type */
	UCHAR msg_att_data[1];		/* File name and dpb */
} *MSG_ATT;

typedef struct msg_info {
	struct msg msg_info_header;
	SLONG msg_info_handle;
	USHORT msg_info_level;
	USHORT msg_info_length;		/* Item length */
	USHORT msg_info_buffer_length;	/* Length of return buffer */
	UCHAR msg_info_data[1];
} *MSG_INFO;

typedef struct msg_op {
	struct msg msg_op_header;
	SLONG msg_op_handle;
	SLONG msg_op_level;
	USHORT msg_op_length;
	USHORT msg_op_data[1];
} *MSG_OP;

typedef struct msg_blob {
	struct msg msg_blob_header;
	SLONG msg_blob_database;
	SLONG msg_blob_transaction;
	USHORT msg_blob_bpb_length;
	SLONG msg_blob_id[2];
	UCHAR msg_blob_bpb[1];
} *MSG_BLOB;

typedef struct msg_seg {
	struct msg msg_seg_header;
	SLONG msg_seg_handle;
	USHORT msg_seg_length;
	USHORT msg_seg_buffer_length;
	UCHAR msg_seg_data[1];
} *MSG_SEG;

typedef struct msg_msg {
	struct msg msg_msg_header;
	SLONG msg_msg_request;
	SLONG msg_msg_transaction;
	USHORT msg_msg_level;
	USHORT msg_msg_type;
	USHORT msg_msg_length;
	UCHAR msg_msg_data[1];
} *MSG_MSG;

typedef struct tdb {
	SLONG tdb_database;
	USHORT tdb_tpb_length;
	PTR tdb_tpb;
} TDB;

typedef struct msg_trans {
	struct msg msg_trans_header;
	USHORT msg_trans_count;
	TDB msg_trans_tdb[1];
} *MSG_TRANS;

typedef struct msg_resp {
	struct msg msg_resp_header;
	SLONG msg_resp_handle;
	STATUS msg_resp_status[ISC_STATUS_LENGTH];
	USHORT msg_resp_length;
	UCHAR msg_resp_data[1];
} *MSG_RESP;

typedef struct msg_event {
	struct msg msg_event_header;
	SLONG msg_event_database;
	void (*msg_event_ast) ();
	void *msg_event_arg;
	USHORT msg_event_length;
	UCHAR msg_event_data[1];
} *MSG_EVENT;

typedef struct msg_slice {
	struct msg msg_slice_header;
	SLONG msg_slice_database;
	SLONG msg_slice_transaction;
	USHORT msg_slice_sdl_length;
	USHORT msg_slice_param_length;
	SLONG msg_slice_slice_length;
	SLONG msg_slice_id[2];
	SLONG msg_slice_handle;
	UCHAR msg_slice_data[1];	/* sdl, param, and slice */
} *MSG_SLICE;

typedef struct msg_ddl {
	struct msg msg_ddl_header;
	SLONG msg_ddl_database;
	SLONG msg_ddl_transaction;
	USHORT msg_ddl_length;
	USHORT msg_ddl_data[1];
} *MSG_DDL;

typedef struct msg_util {
	struct msg msg_util_header;
	USHORT msg_util_cmd;
	USHORT msg_util_csn_len;
	USHORT msg_util_dbn_len;
	TEXT msg_util_data[2];
} *MSG_UTIL;

typedef struct msg_seek {
	struct msg msg_seek_header;
	SLONG msg_seek_handle;
	SSHORT msg_seek_mode;
	SLONG msg_seek_offset;
} *MSG_SEEK;

typedef struct msg_exnow {
	struct msg msg_exnow_header;
	SLONG msg_exnow_database;
	SLONG msg_exnow_transaction;
	USHORT msg_exnow_SQL_length;
	USHORT msg_exnow_SQL_dialect;
	USHORT msg_exnow_in_blr_length;
	USHORT msg_exnow_in_msg_type;
	USHORT msg_exnow_in_msg_length;
	USHORT msg_exnow_out_blr_length;
	USHORT msg_exnow_out_msg_type;
	USHORT msg_exnow_out_msg_length;
	UCHAR msg_exnow_data[1];
} *MSG_EXNOW;

typedef struct msg_pstmt {
	struct msg msg_pstmt_header;
	SLONG msg_pstmt_transaction;
	SLONG msg_pstmt_statement;
	USHORT msg_pstmt_SQL_length;
	USHORT msg_pstmt_SQL_dialect;
	USHORT msg_pstmt_item_length;
	USHORT msg_pstmt_buffer_length;
	UCHAR msg_pstmt_data[1];
} *MSG_PSTMT;

typedef struct msg_setcur {
	struct msg msg_setcur_header;
	SLONG msg_setcur_statement;
	USHORT msg_setcur_type;
	TEXT msg_setcur_cursor[1];
} *MSG_SETCUR;

typedef struct msg_sqlmsg {
	struct msg msg_sqlmsg_header;
	SLONG msg_sqlmsg_transaction;
	SLONG msg_sqlmsg_statement;
	USHORT msg_sqlmsg_in_blr_length;
	USHORT msg_sqlmsg_in_msg_type;
	USHORT msg_sqlmsg_in_msg_length;
	USHORT msg_sqlmsg_out_blr_length;
	USHORT msg_sqlmsg_out_msg_type;
	USHORT msg_sqlmsg_out_msg_length;
	UCHAR msg_sqlmsg_data[1];
} *MSG_SQLMSG;

typedef struct msg_trrq {
	struct msg msg_trrq_header;
	SLONG msg_trrq_database;
	SLONG msg_trrq_transaction;
	USHORT msg_trrq_blr_length;
	USHORT msg_trrq_in_msg_length;
	USHORT msg_trrq_out_msg_length;
	UCHAR msg_trrq_data[1];
} *MSG_TRRQ;

#define UTIL_noop	0
#define UTIL_list	1
#define UTIL_disable	2
#define UTIL_kill	3
#define UTIL_reenable	4
#define UTIL_enable	5

/* Local blocks */

typedef struct bid {
	ULONG bid_relation_id;		/* Relation id (or null) */
	ULONG bid_number;			/* Record number */
} *BID;

typedef SLONG HANDLE;

/* Block types */

#ifndef INCLUDE_FB_BLK
#include "../include/old_fb_blk.h"
#endif

#define type_rdb	10
#define type_rbl	11
#define type_rrq	12
#define type_rtr	13
#define type_evnt	14
#define type_dbn	15
#define type_csn	16
#define type_array	17
#define type_rsr	18

/* Block types */

typedef struct rdb {
	struct blk rdb_header;
	PTR rdb_connection;			/* Connection to partner */
	PTR rdb_connection2;		/* Connection to partner for async events */
	PTR rdb_server;				/* Server process */
	HANDLE rdb_handle;			/* database handle */
	struct rtr *rdb_transactions;	/* linked list of transactions */
	struct rrq *rdb_requests;	/* compiled requests */
	struct rsr *rdb_sql_requests;	/* SQL requests */
	struct rdb *rdb_next;		/* next database in system */
	struct evnt *rdb_events;	/* list of allocated event blocks */
	struct dbn *rdb_dbn;		/* database name */
	int *rdb_status_vector;
	SCHAR *rdb_setjmp;
} *RDB;

typedef struct rtr {
	struct blk rtr_header;
	struct rdb *rtr_rdb;
	struct rtr *rtr_next;
	struct rbl *rtr_blobs;
	struct array *rtr_arrays;
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
	struct rrq *rrq_next;
	struct rrq **rrq_user_handle;
	HANDLE rrq_handle;
} *RRQ;

typedef struct evnt {
	struct blk evnt_header;
	struct evnt *evnt_next;
	struct rdb *evnt_rdb;
	SLONG evnt_id;
	void (*evnt_ast) ();
	void *evnt_arg;
} *EVNT;

typedef struct array {
	struct blk array_header;
	struct rdb *array_rdb;
	struct rtr *array_rtr;
	struct array *array_next;
	SLONG array_length;
	UCHAR *array_slice;
	UCHAR array_data[1];
} *ARRAY;

typedef struct rsr {
	struct blk rsr_header;
	struct rdb *rsr_rdb;
	struct rsr *rsr_next;
	HANDLE rsr_handle;
} *RSR;

typedef struct dbn {
	struct blk dbn_header;
	USHORT dbn_attaches;		/* number of attachments */
	USHORT dbn_max_attaches;	/* maximum number of allowable attachments */
	USHORT dbn_flags;
	struct rdb *dbn_server;		/* central server attachment */
	struct dbn *dbn_next;
	USHORT dbn_length;			/* length of name string */
	TEXT dbn_name[1];			/* name string */
} *DBN;

#define DBN_disable	1			/* disable new attachments */
#define DBN_kill	2			/* kill all attachments */
#define DBN_server_att	4		/* server has its own attachment to database */
#define DBN_cmd_process	8		/* used by central server utility */
#define DBN_reenable	16		/* re-enable new attachments */
#define DBN_enable	32			/* enable a new database */

typedef struct csn {
	struct blk csn_header;
	USHORT csn_flags;
	struct csn *csn_next;
	USHORT csn_cs_len;			/* length of server name string */
	USHORT csn_db_len;			/* length of database name string */
	TEXT *csn_cs_name;			/* server name string */
	TEXT *csn_db_name;			/* database name string */
	TEXT csn_data[2];
} *CSN;

/* Central server utility list command data */

typedef struct csu_list {
	USHORT csu_list_attaches;
	USHORT csu_list_flags;
	USHORT csu_list_length;
	TEXT csu_list_name[1];
} *CSU_LIST;

#endif /* _CSV_CSI_H_ */
