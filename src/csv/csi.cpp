/*
 *	PROGRAM:	Central Server
 *	MODULE:		csi.c
 *	DESCRIPTION:	Central Server Interface
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
#include "../jrd/common.h"
#include <stdarg.h>
#include "../csv/csi.h"
#include "../jrd/license.h"
#include "gen/codes.h"
#include "../jrd/inf.h"
#include "../jrd/thd.h"
#include "../csv/csi_proto.h"
#include "../csv/css_proto.h"
#include "../jrd/gds_proto.h"
#include "../remote/merge_proto.h"

#ifdef PIPE_SERVER
#ifdef VMS
#include descrip
#include libdef
#include ssdef
#include jpidef
#include pqldef
#include "../jrd/lnmdef.h"

#ifndef ORACLE_ALT
#define GDS_PIPE	"[sysexe]gds_pipe_orcl.exe"
#else
#define GDS_PIPE	"[sysexe]gds_pipe_orcl_alt.exe"
#endif
#endif
#endif	// PIPE_SERVER

#define ALLOC(type, length)	CSS_alloc_local (type, length)
#define FREE(block)		CSS_free_local (block)

#ifndef MOVE
#define MOVE(from, to, length)	move (from, to, length)
#endif

static RDB CSI_databases, CSI_free_servers;
static TEXT error_buffer[1024];

#ifdef PIPE_SERVER
#ifdef VMS
/* Define logical names that sub-process should inherit */

static SCHAR *inherit_logicals[] = {
	"SYS$LOGIN",
	"SYS$SCRATCH",
	"SYS$NODE",
	"SYS$INTERBASE",
	0
};

typedef struct itm {
	SSHORT itm_length;
	SSHORT itm_code;
	SCHAR *itm_buffer;
	SSHORT *itm_return_length;
} ITM;

typedef struct itmq {
	SCHAR itmq_code;
	SLONG itmq_value;
} ITMQ;
#endif

#endif

#ifndef MULTI_THREAD
static void event_handler(PTR);
#else
static void event_thread(PTR);
#endif

static STATUS get_response(RDB, MSG, STATUS *, HANDLE *, USHORT, UCHAR *,
						   USHORT *);
static STATUS handle_error(STATUS *, STATUS);
static STATUS info(RDB, STATUS *, HANDLE, MSG_T, USHORT, USHORT, UCHAR *,
				   USHORT, UCHAR *);
#ifndef GATEWAY
static RDB init(STATUS *, MSG_T, UCHAR *, USHORT, UCHAR *, USHORT, TEXT *,
				USHORT, USHORT, TEXT *, USHORT *);
#else
static RDB init(STATUS *, MSG_T, UCHAR *, USHORT, UCHAR *, USHORT, TEXT *);
#endif
static RTR make_transaction(RDB, HANDLE);
static void move(UCHAR *, UCHAR *, USHORT);
static USHORT name_length(TEXT *);
static STATUS process_response(MSG_RESP, STATUS *, HANDLE *, USHORT, UCHAR *,
							   USHORT *);
static void release_blob(RBL);
static STATUS release_object(RDB, MSG_T, HANDLE, STATUS *);
static void release_request(RRQ);
static void release_sql_request(RSR);
static void release_transaction(RTR);
static STATUS send_blob(STATUS *, RBL, USHORT, UCHAR *);
#ifdef PIPE_SERVER
#ifdef VMS
static void setup_creprc_info(SLONG **, ITMQ **, SLONG *);
static int spawn(STATUS *, UCHAR *, PTR *);
static void trans_logicals(void);
#endif
#endif

#define CHECK_HANDLE(blk, type, error) if (!blk || ((BLK) blk)->blk_type != (SCHAR) type) \
	return handle_error (user_status, error)

#define NULL_CHECK(ptr, code) if (*ptr) return handle_error (user_status, code)

/* Transaction element block */

typedef struct teb {
	RDB *teb_database;
	int teb_tpb_length;
	UCHAR *teb_tpb;
} TEB;


#ifndef PIPE_SERVER
#define GDS_ATTACH_DATABASE	CSI_attach_database
#define GDS_BLOB_INFO		CSI_blob_info
#define GDS_CANCEL_BLOB		CSI_cancel_blob
#define GDS_CLOSE_BLOB		CSI_close_blob
#define GDS_COMMIT		CSI_commit_transaction
#define GDS_COMMIT_RETAINING	CSI_commit_retaining
#define GDS_COMPILE		CSI_compile_request
#define GDS_CREATE_BLOB2	CSI_create_blob2
#define GDS_CREATE_DATABASE	CSI_create_database
#define GDS_DATABASE_INFO	CSI_database_info
#define GDS_DDL			CSI_ddl
#define GDS_DETACH		CSI_detach_database
#define GDS_DROP_DATABASE	CSI_drop_database
#define GDS_GET_SEGMENT		CSI_get_segment
#define GDS_GET_SLICE		CSI_get_slice
#define GDS_OPEN_BLOB2		CSI_open_blob2
#define GDS_PREPARE		CSI_prepare_transaction
#define GDS_PUT_SEGMENT		CSI_put_segment
#define GDS_PUT_SLICE		CSI_put_slice
#define GDS_RECEIVE		CSI_receive
#define GDS_RECONNECT		CSI_reconnect_transaction
#define GDS_RELEASE_REQUEST	CSI_release_request
#define GDS_REQUEST_INFO	CSI_request_info
#define GDS_ROLLBACK		CSI_rollback_transaction
#define GDS_SEEK_BLOB		CSI_seek_blob
#define GDS_SEND		CSI_send
#define GDS_START_AND_SEND	CSI_start_and_send
#define GDS_START		CSI_start_request
#define GDS_START_MULTIPLE	CSI_start_multiple
#define GDS_START_TRANSACTION	CSI_start_transaction
#define GDS_TRANSACT_REQUEST	CSI_transact_request
#define GDS_TRANSACTION_INFO	CSI_transaction_info
#define GDS_UNWIND		CSI_unwind_request
#define GDS_QUE_EVENTS		CSI_que_events
#define GDS_CANCEL_EVENTS	CSI_cancel_events

#define GDS_DSQL_ALLOCATE	CSI_allocate_statement
#define GDS_DSQL_EXECUTE	CSI_execute
#define GDS_DSQL_EXECUTE2	CSI_execute2
#define GDS_DSQL_EXECUTE_IMMED	CSI_execute_immediate
#define GDS_DSQL_EXECUTE_IMMED2	CSI_execute_immediate2
#define GDS_DSQL_FETCH		CSI_fetch
#define GDS_DSQL_FREE		CSI_free_statement
#define GDS_DSQL_INSERT		CSI_insert
#define GDS_DSQL_PREPARE	CSI_prepare
#define GDS_DSQL_SET_CURSOR	CSI_set_cursor_name
#define GDS_DSQL_SQL_INFO	CSI_sql_info
#else
#define GDS_ATTACH_DATABASE	PSI_attach_database
#define GDS_BLOB_INFO		PSI_blob_info
#define GDS_CANCEL_BLOB		PSI_cancel_blob
#define GDS_CLOSE_BLOB		PSI_close_blob
#define GDS_COMMIT		PSI_commit_transaction
#define GDS_COMMIT_RETAINING	PSI_commit_retaining
#define GDS_COMPILE		PSI_compile_request
#define GDS_CREATE_BLOB		PSI_create_blob
#define GDS_CREATE_BLOB2	PSI_create_blob2
#define GDS_CREATE_DATABASE	PSI_create_database
#define GDS_DATABASE_INFO	PSI_database_info
#define GDS_DDL			PSI_ddl
#define GDS_DETACH		PSI_detach_database
#define GDS_DROP_DATABASE	PSI_drop_database
#define GDS_GET_SEGMENT		PSI_get_segment
#define GDS_GET_SLICE		PSI_get_slice
#define GDS_OPEN_BLOB		PSI_open_blob
#define GDS_OPEN_BLOB2		PSI_open_blob2
#define GDS_PREPARE		PSI_prepare_transaction
#define GDS_PUT_SEGMENT		PSI_put_segment
#define GDS_PUT_SLICE		PSI_put_slice
#define GDS_RECEIVE		PSI_receive
#define GDS_RECONNECT		PSI_reconnect_transaction
#define GDS_RELEASE_REQUEST	PSI_release_request
#define GDS_REQUEST_INFO	PSI_request_info
#define GDS_ROLLBACK		PSI_rollback_transaction
#define GDS_SEND		PSI_send
#define GDS_SEEK_BLOB		PSI_seek_blob
#define GDS_START_AND_SEND	PSI_start_and_send
#define GDS_START		PSI_start_request
#define GDS_START_MULTIPLE	PSI_start_multiple
#define GDS_START_TRANSACTION	PSI_start_transaction
#define GDS_TRANSACT_REQUEST	PSI_transact_request
#define GDS_TRANSACTION_INFO	PSI_transaction_info
#define GDS_UNWIND		PSI_unwind_request
#define GDS_QUE_EVENTS		PSI_que_events
#define GDS_CANCEL_EVENTS	PSI_cancel_events

#define GDS_DSQL_ALLOCATE	PSI_allocate_statement
#define GDS_DSQL_EXECUTE	PSI_execute
#define GDS_DSQL_EXECUTE2	PSI_execute2
#define GDS_DSQL_EXECUTE_IMMED	PSI_execute_immediate
#define GDS_DSQL_EXECUTE_IMMED2	PSI_execute_immediate2
#define GDS_DSQL_FETCH		PSI_fetch
#define GDS_DSQL_FREE		PSI_free_statement
#define GDS_DSQL_INSERT		PSI_insert
#define GDS_DSQL_PREPARE	PSI_prepare
#define GDS_DSQL_SET_CURSOR	PSI_set_cursor_name
#define GDS_DSQL_SQL_INFO	PSI_sql_info
#endif


#ifndef GATEWAY
STATUS GDS_ATTACH_DATABASE(
						   STATUS * user_status,
						   SSHORT file_length,
						   SCHAR * file_name,
						   RDB * handle,
SSHORT dpb_length, SCHAR * dpb, TEXT * expanded_filename)
{
/**************************************
 *
 *	g d s _ A T T A C H _ D A T A B A S E	( n o n - G a t e w a y )
 *
 **************************************
 *
 * Functional description
 *	Create a nice, squeeky clean database, uncorrupted by user data.
 *
 **************************************/
	RDB rdb;

	NULL_CHECK(handle, gds__bad_db_handle);

	if (rdb = init(user_status, MSG_attach_database,
				   file_name, file_length, dpb, dpb_length, expanded_filename,
				   0, 0, 0, 0))
		*handle = rdb;

	return user_status[1];
}
#endif


#ifdef GATEWAY
#ifdef PIPE_SERVER
STATUS GDS_ATTACH_DATABASE(STATUS * user_status,
						   SSHORT file_length,
						   SCHAR * file_name,
						   RDB * handle,
						   SSHORT dpb_length,
						   SCHAR * dpb, TEXT * expanded_filename)
{
/**************************************
 *
 *	g d s _ A T T A C H _ D A T A B A S E	( G a t e w a y _ s e r v e r )
 *
 **************************************
 *
 * Functional description
 *	Create a nice, squeeky clean database, uncorrupted by user data.
 *
 **************************************/
	RDB rdb;

	NULL_CHECK(handle, gds__bad_db_handle);

	if (rdb = init(user_status, MSG_attach_database,
				   file_name, file_length, dpb, dpb_length,
				   expanded_filename)) *handle = rdb;

	return user_status[1];
}
#endif
#endif


STATUS GDS_BLOB_INFO(STATUS * user_status,
					 RBL * blob,
					 SSHORT item_length,
					 UCHAR * items, SSHORT buffer_length, UCHAR * buffer)
{
/**************************************
 *
 *	g d s _ b l o b _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Provide information on blob object.
 *
 **************************************/
	RDB rdb;

	CHECK_HANDLE((*blob), type_rbl, gds__bad_segstr_handle);
	rdb = (*blob)->rbl_rdb;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	return info(rdb, user_status, (*blob)->rbl_handle, MSG_blob_info, 0,
				item_length, items, buffer_length, buffer);
}


STATUS GDS_CANCEL_BLOB(STATUS * user_status, RBL * blob_handle)
{
/**************************************
 *
 *	g d s _ c a n c e l _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Abort a partially completed blob.
 *
 **************************************/
	RDB rdb;
	RBL blob;

	if (!(blob = *blob_handle)) {
		if (user_status) {
			*user_status++ = gds_arg_gds;
			*user_status++ = SUCCESS;
			*user_status = 0;
		}
		return SUCCESS;
	}

	CHECK_HANDLE(blob, type_rbl, gds__bad_segstr_handle);
	rdb = blob->rbl_rdb;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	if (release_object(rdb, MSG_cancel_blob, blob->rbl_handle, user_status))
		return user_status[1];

	release_blob(blob);
	*blob_handle = NULL;

	return user_status[1];
}


STATUS GDS_CANCEL_EVENTS(STATUS * user_status, RDB * handle, SLONG * id)
{
/**************************************
 *
 *	g d s _ C A N C E L _ E V E N T S
 *
 **************************************
 *
 * Functional description
 *	Cancel a remove event.
 *
 **************************************/
	RDB rdb;
	MSG_OP message;

	rdb = *handle;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	message = (MSG_OP) CSS_alloc_message(type_msg, sizeof(struct msg_op));
	message->msg_op_header.msg_type = MSG_cancel_events;
	message->msg_op_handle = rdb->rdb_handle;
	message->msg_op_level = *id;

	return get_response(rdb, message, user_status, 0, 0, 0, 0);
}


STATUS GDS_CLOSE_BLOB(STATUS * user_status, RBL * blob_handle)
{
/**************************************
 *
 *	g d s _ c l o s e _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Abort a partially completed blob.
 *
 **************************************/
	RDB rdb;
	RBL blob;

	blob = *blob_handle;
	CHECK_HANDLE(blob, type_rbl, gds__bad_segstr_handle);
	rdb = blob->rbl_rdb;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

/* If a portion of the blob hasn't been sent to the pipe yet,
   do so now. */

	if ((blob->rbl_flags & RBL_create) && blob->rbl_ptr != blob->rbl_buffer)
		send_blob(user_status, blob, 0, NULL);

	if (release_object(rdb, MSG_close_blob, blob->rbl_handle, user_status))
		return user_status[1];

	release_blob(blob);
	*blob_handle = NULL;

	return user_status[1];
}


STATUS GDS_COMMIT(STATUS * user_status, RTR * rtr_handle)
{
/**************************************
 *
 *	g d s _ C O M M I T
 *
 **************************************
 *
 * Functional description
 *	Commit a transaction.
 *
 **************************************/
	RDB rdb;
	RTR transaction;

	transaction = *rtr_handle;
	CHECK_HANDLE(transaction, type_rtr, gds__bad_trans_handle);
	rdb = transaction->rtr_rdb;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	if (release_object(rdb, MSG_commit, transaction->rtr_handle, user_status))
		return user_status[1];

	release_transaction(transaction);
	*rtr_handle = NULL;

	return user_status[1];
}


STATUS GDS_COMMIT_RETAINING(STATUS * user_status, RTR * rtr_handle)
{
/**************************************
 *
 *	g d s _ c o m m i t _ r e t a i n i n g
 *
 **************************************
 *
 * Functional description
 *	Prepare a transaction for commit.  First phase of a two
 *	phase commit.
 *
 **************************************/
	RDB rdb;
	RTR transaction;

	transaction = *rtr_handle;
	CHECK_HANDLE(transaction, type_rtr, gds__bad_trans_handle);
	rdb = transaction->rtr_rdb;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	if (release_object
		(rdb, MSG_commit_retaining, transaction->rtr_handle,
		 user_status)) return user_status[1];

	return user_status[1];
}


STATUS GDS_COMPILE(STATUS * user_status,
				   RDB * db_handle,
				   RRQ * req_handle, USHORT blr_length, UCHAR * blr)
{
/**************************************
 *
 *	g d s _ C O M P I L E
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	RDB rdb;
	RRQ request;
	MSG_OP message;
	HANDLE handle;

/* Check and validate handles, etc. */

	NULL_CHECK(req_handle, gds__bad_req_handle);
	rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	message =
		(MSG_OP) CSS_alloc_message(type_msg,
								   (int) (sizeof(struct msg_op) +
										  blr_length));
	message->msg_op_header.msg_type = MSG_compile;
	message->msg_op_handle = rdb->rdb_handle;
	message->msg_op_length = blr_length;
	MOVE(blr, message->msg_op_data, blr_length);

	if (!get_response(rdb, message, user_status, &handle, 0, 0, 0)) {
		*req_handle = request = (RRQ) ALLOC(type_rrq, sizeof(struct rrq));
		request->rrq_handle = handle;
		request->rrq_rdb = rdb;
		request->rrq_next = rdb->rdb_requests;
		rdb->rdb_requests = request;
	}

	return user_status[1];
}


STATUS GDS_CREATE_BLOB2(STATUS * user_status,
						RDB * db_handle,
						RTR * rtr_handle,
						RBL * blob_handle,
						BID blob_id, USHORT bpb_length, UCHAR * bpb)
{
/**************************************
 *
 *	g d s _ c r e a t e _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Open an existing blob.
 *
 **************************************/
	RDB rdb;
	RTR transaction;
	RBL blob;
	HANDLE handle;
	MSG_BLOB message;

	NULL_CHECK(blob_handle, gds__bad_segstr_handle);
	rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);
	transaction = *rtr_handle;
	CHECK_HANDLE(transaction, type_rtr, gds__bad_trans_handle);

	message =
		(MSG_BLOB) CSS_alloc_message(type_msg,
									 (int) (sizeof(struct msg_blob) +
											bpb_length - 1));
	message->msg_blob_header.msg_type = MSG_create_blob;
	message->msg_blob_database = rdb->rdb_handle;
	message->msg_blob_transaction = transaction->rtr_handle;
	message->msg_blob_bpb_length = bpb_length;
	MOVE(bpb, message->msg_blob_bpb, bpb_length);

	if (!get_response(rdb, message, user_status, &handle, 8, blob_id, 0)) {
		*blob_handle = blob =
			(RBL) ALLOC(type_rbl, sizeof(struct rbl) + BLOB_LENGTH);
		blob->rbl_handle = handle;
		blob->rbl_buffer_length = BLOB_LENGTH;
		blob->rbl_rdb = rdb;
		blob->rbl_rtr = transaction;
		blob->rbl_ptr = blob->rbl_buffer;
		blob->rbl_flags |= RBL_create;
		blob->rbl_next = transaction->rtr_blobs;
		transaction->rtr_blobs = blob;
	}

	return user_status[1];
}


#ifdef PIPE_SERVER
STATUS GDS_CREATE_BLOB(STATUS * user_status,
					   RDB * db_handle,
					   RTR * rtr_handle, RBL * blob_handle, BID blob_id)
{
/**************************************
 *
 *	g d s _ c r e a t e _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Open an existing blob.
 *
 **************************************/

	return GDS_CREATE_BLOB2(user_status, db_handle, rtr_handle, blob_handle,
							blob_id, 0, NULL);
}
#endif


#ifndef GATEWAY
STATUS GDS_CREATE_DATABASE(STATUS * user_status,
						   SSHORT file_length,
						   SCHAR * file_name,
						   RDB * handle,
						   SSHORT dpb_length,
						   SCHAR * dpb,
						   SSHORT db_type, TEXT * expanded_filename)
{
/**************************************
 *
 *	g d s _ C R E A T E _ D A T A B A S E
 *
 **************************************
 *
 * Functional description
 *	Create a nice, squeeky clean database, uncorrupted by user data.
 *
 **************************************/
	RDB rdb;

	NULL_CHECK(handle, gds__bad_db_handle);

	if (rdb = init(user_status, MSG_create_database,
				   file_name, file_length, dpb, dpb_length, expanded_filename,
				   0, 0, 0, 0))
		*handle = rdb;

	return user_status[1];
}
#endif


STATUS GDS_DATABASE_INFO(STATUS * user_status,
						 RDB * handle,
						 SSHORT item_length,
						 UCHAR * items, SSHORT buffer_length, UCHAR * buffer)
{
/**************************************
 *
 *	g d s _ d a t a b a s e _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Provide information on database object.
 *
 **************************************/
	RDB rdb;
	UCHAR temp[1024];

	rdb = *handle;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	if (info
		(rdb, user_status, rdb->rdb_handle, MSG_database_info, 0, item_length,
		 items, sizeof(temp), temp))
		return user_status[1];

	MERGE_database_info(temp, buffer, buffer_length,
#ifndef PIPE_SERVER
						IMPLEMENTATION, 9, 1, GDS_VERSION, "", 0);
#else
						IMPLEMENTATION, 7, 1, GDS_VERSION, "", 0);
#endif

	return user_status[1];
}


STATUS GDS_DDL(STATUS * user_status,
			   RDB * db_handle,
			   RTR * rtr_handle, USHORT ddl_length, UCHAR * ddl)
{
/**************************************
 *
 *	g d s _ d d l
 *
 **************************************
 *
 * Functional description
 *	Do meta-data update.
 *
 **************************************/
	RDB rdb;
	RTR transaction;
	MSG_DDL message;

/* Check and validate handles, etc. */

	transaction = *rtr_handle;
	CHECK_HANDLE(transaction, type_rtr, gds__bad_trans_handle);
	rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	message =
		(MSG_DDL) CSS_alloc_message(type_msg,
									(int) (sizeof(struct msg_ddl) +
										   ddl_length));
	message->msg_ddl_header.msg_type = MSG_ddl;
	message->msg_ddl_database = rdb->rdb_handle;
	message->msg_ddl_transaction = transaction->rtr_handle;
	message->msg_ddl_length = ddl_length;
	MOVE(ddl, message->msg_ddl_data, ddl_length);

	return get_response(rdb, message, user_status, 0, 0, 0, 0);
}


STATUS GDS_DETACH(STATUS * user_status, RDB * handle)
{
/**************************************
 *
 *	g d s _ D E T A C H
 *
 **************************************
 *
 * Functional description
 *	Close down a database.
 *
 **************************************/
	RDB rdb, *ptr;
	RRQ rrq;

	rdb = *handle;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	if (release_object(rdb, MSG_detach, rdb->rdb_handle, user_status))
		return user_status[1];

	while (rrq = rdb->rdb_requests) {
		if (rrq->rrq_user_handle)
			*(rrq->rrq_user_handle) = NULL;
		release_request(rrq);
	}

	while (rdb->rdb_sql_requests)
		release_sql_request(rdb->rdb_sql_requests);

	while (rdb->rdb_transactions)
		release_transaction(rdb->rdb_transactions);

	if (rdb->rdb_connection2) {
#ifndef GATEWAY
#ifndef MULTI_THREAD
		ISC_signal_cancel(EVENT_SIGNAL, event_handler, rdb->rdb_connection2);
#endif
#endif
		CSS_disconnect(rdb->rdb_connection2);
	}

#ifdef GATEWAY
#ifdef PIPE_SERVER
#define GWAY_PIPE
	rdb->rdb_connection2 = NULL;
	rdb->rdb_handle = NULL;

/* Unhook the rdb block and then add it to the free server list */

	for (ptr = &CSI_databases; *ptr; ptr = &(*ptr)->rdb_next)
		if (*ptr == rdb) {
			*ptr = rdb->rdb_next;
			break;
		}

	rdb->rdb_next = CSI_free_servers;
	CSI_free_servers = rdb;
#endif
#endif

#ifndef GWAY_PIPE
	CSS_disconnect(rdb->rdb_connection);

	FREE(rdb);
#endif

	*handle = NULL;

	return user_status[1];
}


STATUS GDS_DROP_DATABASE(STATUS * user_status, RDB * handle)
{
/**************************************
 *
 *	i s c _ d r o p _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Close down and then purge a database.
 *
 **************************************/
	RDB rdb, *ptr;
	RRQ rrq;

	rdb = *handle;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	if (release_object(rdb, MSG_drop_database, rdb->rdb_handle, user_status))
		return user_status[1];

	while (rrq = rdb->rdb_requests) {
		if (rrq->rrq_user_handle)
			*(rrq->rrq_user_handle) = NULL;
		release_request(rrq);
	}

	while (rdb->rdb_sql_requests)
		release_sql_request(rdb->rdb_sql_requests);

	while (rdb->rdb_transactions)
		release_transaction(rdb->rdb_transactions);

	if (rdb->rdb_connection2) {
#ifndef GATEWAY
#ifndef MULTI_THREAD
		ISC_signal_cancel(EVENT_SIGNAL, event_handler, rdb->rdb_connection2);
#endif
#endif
		CSS_disconnect(rdb->rdb_connection2);
	}

#ifdef GATEWAY
#ifdef PIPE_SERVER
#define GWAY_PIPE
	rdb->rdb_connection2 = NULL;
	rdb->rdb_handle = NULL;

/* Unhook the rdb block and then add it to the free server list */

	for (ptr = &CSI_databases; *ptr; ptr = &(*ptr)->rdb_next)
		if (*ptr == rdb) {
			*ptr = rdb->rdb_next;
			break;
		}

	rdb->rdb_next = CSI_free_servers;
	CSI_free_servers = rdb;
#endif
#endif

#ifndef GWAY_PIPE
	CSS_disconnect(rdb->rdb_connection);

	FREE(rdb);
#endif

	*handle = NULL;

	return user_status[1];
}


STATUS GDS_DSQL_ALLOCATE(STATUS * user_status,
						 RDB * db_handle, RSR * stmt_handle)
{
/**************************************
 *
 *	d s q l _ a l l o c a t e _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *	Allocate a statement handle.
 *
 **************************************/
	RDB rdb;
	RSR statement;
	MSG_OP message;
	HANDLE handle;

/* Check and validate handles, etc. */

	NULL_CHECK(stmt_handle, gds__bad_req_handle);
	rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

/* set up the packet for the other guy... */

	message = (MSG_OP) CSS_alloc_message(type_msg, sizeof(struct msg_op));
	message->msg_op_header.msg_type = MSG_allocate_stmt;
	message->msg_op_handle = rdb->rdb_handle;

	if (!get_response(rdb, message, user_status, &handle, 0, 0, 0)) {
		*stmt_handle = statement = (RSR) ALLOC(type_rsr, sizeof(struct rsr));
		statement->rsr_handle = handle;
		statement->rsr_rdb = rdb;
		statement->rsr_next = rdb->rdb_sql_requests;
		rdb->rdb_sql_requests = statement;
	}

	return user_status[1];
}


STATUS GDS_DSQL_EXECUTE(STATUS * user_status,
						RTR * rtr_handle,
						RSR * stmt_handle,
						USHORT blr_length,
						UCHAR * blr,
						USHORT msg_type, USHORT msg_length, UCHAR * msg)
{
/**************************************
 *
 *	d s q l _ e x e c u t e
 *
 **************************************
 *
 * Functional description
 *	Execute a dynamic SQL statement.
 *
 **************************************/
	RTR transaction;
	RSR statement;
	HANDLE handle;
	MSG_SQLMSG message;

/* Check and validate handles, etc. */

	statement = *stmt_handle;
	CHECK_HANDLE(statement, type_rsr, gds__bad_req_handle);
	if (transaction = *rtr_handle)
		CHECK_HANDLE(transaction, type_rtr, gds__bad_trans_handle);

/* set up the packet for the other guy... */

	message =
		(MSG_SQLMSG) CSS_alloc_message(type_msg,
									   (int) (sizeof(struct msg_sqlmsg) +
											  blr_length + msg_length));
	message->msg_sqlmsg_header.msg_type = MSG_execute;
	message->msg_sqlmsg_transaction =
		(transaction) ? transaction->rtr_handle : NULL;
	message->msg_sqlmsg_statement = statement->rsr_handle;
	message->msg_sqlmsg_in_blr_length = blr_length;
	MOVE(blr, message->msg_sqlmsg_data + msg_length, blr_length);
	message->msg_sqlmsg_in_msg_type = msg_type;
	message->msg_sqlmsg_in_msg_length = msg_length;
	MOVE(msg, message->msg_sqlmsg_data, msg_length);

	if (!get_response
		(statement->rsr_rdb, message, user_status, &handle, 0, 0,
		 0)) if (transaction && !handle) {
			release_transaction(transaction);
			*rtr_handle = NULL;
		}
		else if (!transaction && handle)
			*rtr_handle = make_transaction(statement->rsr_rdb, handle);

	return user_status[1];
}


STATUS GDS_DSQL_EXECUTE2(STATUS * user_status,
						 RTR * rtr_handle,
						 RSR * stmt_handle,
						 USHORT in_blr_length,
						 UCHAR * in_blr,
						 USHORT in_msg_type,
						 USHORT in_msg_length,
						 UCHAR * in_msg,
						 USHORT out_blr_length,
						 UCHAR * out_blr,
						 USHORT out_msg_type,
						 USHORT out_msg_length, UCHAR * out_msg)
{
/**************************************
 *
 *	d s q l _ e x e c u t e 2
 *
 **************************************
 *
 * Functional description
 *	Execute a dynamic SQL statement.
 *
 **************************************/
	RTR transaction;
	RSR statement;
	HANDLE handle;
	MSG_SQLMSG message;

/* Check and validate handles, etc. */

	statement = *stmt_handle;
	CHECK_HANDLE(statement, type_rsr, gds__bad_req_handle);
	if (transaction = *rtr_handle)
		CHECK_HANDLE(transaction, type_rtr, gds__bad_trans_handle);

/* set up the packet for the other guy... */

	message =
		(MSG_SQLMSG) CSS_alloc_message(type_msg,
									   (int) (sizeof(struct msg_sqlmsg) +
											  in_blr_length + in_msg_length +
											  out_blr_length));
	message->msg_sqlmsg_header.msg_type = MSG_execute2;
	message->msg_sqlmsg_transaction =
		(transaction) ? transaction->rtr_handle : NULL;
	message->msg_sqlmsg_statement = statement->rsr_handle;
	message->msg_sqlmsg_in_blr_length = in_blr_length;
	MOVE(in_blr, message->msg_sqlmsg_data + in_msg_length, in_blr_length);
	message->msg_sqlmsg_in_msg_type = in_msg_type;
	message->msg_sqlmsg_in_msg_length = in_msg_length;
	MOVE(in_msg, message->msg_sqlmsg_data, in_msg_length);
	message->msg_sqlmsg_out_blr_length = out_blr_length;
	MOVE(out_blr, message->msg_sqlmsg_data + in_msg_length + in_blr_length,
		 out_blr_length);
	message->msg_sqlmsg_out_msg_type = out_msg_type;
	message->msg_sqlmsg_out_msg_length = out_msg_length;

	if (!get_response
		(statement->rsr_rdb, message, user_status, &handle, out_msg_length,
		 out_msg, 0))
		if (transaction && !handle) {
			release_transaction(transaction);
			*rtr_handle = NULL;
		}
		else if (!transaction && handle)
			*rtr_handle = make_transaction(statement->rsr_rdb, handle);

	return user_status[1];
}


STATUS GDS_DSQL_EXECUTE_IMMED(STATUS * user_status,
							  RDB * db_handle,
							  RTR * rtr_handle,
							  USHORT length,
							  TEXT * string,
							  USHORT dialect,
							  USHORT blr_length,
							  UCHAR * blr,
							  USHORT msg_type, USHORT msg_length, UCHAR * msg)
{
/**************************************
 *
 *	d s q l _ e x e c u t e _ i m m e d i a t e 
 *
 **************************************
 *
 * Functional description
 *	Prepare and execute a statement.
 *
 **************************************/
	RDB rdb;
	RTR transaction;
	HANDLE handle;
	MSG_EXNOW message;

/* Check and validate handles, etc. */

	rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);
	if (transaction = *rtr_handle)
		CHECK_HANDLE(transaction, type_rtr, gds__bad_trans_handle);

	if (!length)
		length = strlen(string);

/* set up the packet for the other guy... */

	message = (MSG_EXNOW) CSS_alloc_message(type_msg,
											(int) (sizeof(struct msg_exnow) +
												   length + blr_length +
												   msg_length));
	message->msg_exnow_header.msg_type = MSG_execute_immediate;
	message->msg_exnow_database = rdb->rdb_handle;
	message->msg_exnow_transaction =
		(transaction) ? transaction->rtr_handle : NULL;
	message->msg_exnow_SQL_length = length;
	MOVE(string, message->msg_exnow_data + length + blr_length, length);
	message->msg_exnow_SQL_dialect = dialect;
	message->msg_exnow_in_blr_length = blr_length;
	MOVE(blr, message->msg_exnow_data + length, blr_length);
	message->msg_exnow_in_msg_type = msg_type;
	message->msg_exnow_in_msg_length = msg_length;
	MOVE(msg, message->msg_exnow_data, msg_length);

	if (!get_response(rdb, message, user_status, &handle, 0, 0, 0))
		if (transaction && !handle) {
			release_transaction(transaction);
			*rtr_handle = NULL;
		}
		else if (!transaction && handle)
			*rtr_handle = make_transaction(rdb, handle);

	return user_status[1];
}


STATUS GDS_DSQL_EXECUTE_IMMED2(STATUS * user_status,
							   RDB * db_handle,
							   RTR * rtr_handle,
							   USHORT length,
							   TEXT * string,
							   USHORT dialect,
							   USHORT in_blr_length,
							   UCHAR * in_blr,
							   USHORT in_msg_type,
							   USHORT in_msg_length,
							   UCHAR * in_msg,
							   USHORT out_blr_length,
							   UCHAR * out_blr,
							   USHORT out_msg_type,
							   USHORT out_msg_length, UCHAR * out_msg)
{
/**************************************
 *
 *	d s q l _ e x e c u t e _ i m m e d i a t e 2
 *
 **************************************
 *
 * Functional description
 *	Prepare and execute a statement.
 *
 **************************************/
	RDB rdb;
	RTR transaction;
	HANDLE handle;
	MSG_EXNOW message;

/* Check and validate handles, etc. */

	rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);
	if (transaction = *rtr_handle)
		CHECK_HANDLE(transaction, type_rtr, gds__bad_trans_handle);

	if (!length)
		length = strlen(string);

/* set up the packet for the other guy... */

	message = (MSG_EXNOW) CSS_alloc_message(type_msg,
											(int) (sizeof(struct msg_exnow) +
												   length + in_blr_length +
												   in_msg_length +
												   out_blr_length));
	message->msg_exnow_header.msg_type = MSG_execute_immediate2;
	message->msg_exnow_database = rdb->rdb_handle;
	message->msg_exnow_transaction =
		(transaction) ? transaction->rtr_handle : NULL;
	message->msg_exnow_SQL_length = length;
	MOVE(string, message->msg_exnow_data + length + in_blr_length, length);
	message->msg_exnow_SQL_dialect = dialect;
	message->msg_exnow_in_blr_length = in_blr_length;
	MOVE(in_blr, message->msg_exnow_data + length, in_blr_length);
	message->msg_exnow_in_msg_type = in_msg_type;
	message->msg_exnow_in_msg_length = in_msg_length;
	MOVE(in_msg, message->msg_exnow_data, in_msg_length);
	message->msg_exnow_out_blr_length = out_blr_length;
	MOVE(out_blr,
		 message->msg_exnow_data + length + in_blr_length + in_msg_length,
		 out_blr_length);
	message->msg_exnow_out_msg_type = out_msg_type;
	message->msg_exnow_out_msg_length = out_msg_length;

	if (!get_response
		(rdb, message, user_status, &handle, out_msg_length, out_msg, 0))
		if (transaction && !handle) {
			release_transaction(transaction);
			*rtr_handle = NULL;
		}
		else if (!transaction && handle)
			*rtr_handle = make_transaction(rdb, handle);

	return user_status[1];
}


STATUS GDS_DSQL_FETCH(STATUS * user_status,
					  RSR * stmt_handle,
					  USHORT blr_length,
					  UCHAR * blr,
					  USHORT msg_type, USHORT msg_length, UCHAR * msg)
{
/**************************************
 *                  
 *	d s q l _ f e t c h
 *
 **************************************
 *
 * Functional description
 *	Fetch next record from a dynamic SQL cursor.
 *
 **************************************/
	RSR statement;
	MSG_SQLMSG message;
	SLONG status;

/* Check and validate handles, etc. */

	statement = *stmt_handle;
	CHECK_HANDLE(statement, type_rsr, gds__bad_req_handle);

/* set up the packet for the other guy... */

	message =
		(MSG_SQLMSG) CSS_alloc_message(type_msg,
									   (int) (sizeof(struct msg_sqlmsg) +
											  blr_length));
	message->msg_sqlmsg_header.msg_type = MSG_fetch;
	message->msg_sqlmsg_statement = statement->rsr_handle;
	message->msg_sqlmsg_in_blr_length = blr_length;
	MOVE(blr, message->msg_sqlmsg_data, blr_length);
	message->msg_sqlmsg_in_msg_type = msg_type;
	message->msg_sqlmsg_in_msg_length = msg_length;

	get_response(statement->rsr_rdb, message, user_status, &status,
				 msg_length, msg, 0);

	if (status == 100)
		return 100;

	return user_status[1];
}


STATUS GDS_DSQL_FREE(STATUS * user_status, RSR * stmt_handle, USHORT option)
{
/**************************************
 *
 *	d s q l _ f r e e _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *	Release request for a Dynamic SQL statement
 *
 **************************************/
	RDB rdb;
	RSR statement;
	MSG_OP message;
	HANDLE handle;

/* Check and validate handles, etc. */

	statement = *stmt_handle;
	CHECK_HANDLE(statement, type_rsr, gds__bad_req_handle);
	rdb = statement->rsr_rdb;

	message = (MSG_OP) CSS_alloc_message(type_msg, sizeof(struct msg_op));
	message->msg_op_header.msg_type = MSG_free_stmt;
	message->msg_op_handle = statement->rsr_handle;
	message->msg_op_level = option;

	if (get_response(rdb, message, user_status, &handle, 0, 0, 0))
		return user_status[1];

	if (!(statement->rsr_handle = handle)) {
		release_sql_request(statement);
		*stmt_handle = NULL;
	}

	return user_status[1];
}


STATUS GDS_DSQL_INSERT(STATUS * user_status,
					   RSR * stmt_handle,
					   USHORT blr_length,
					   UCHAR * blr,
					   USHORT msg_type, USHORT msg_length, UCHAR * msg)
{
/**************************************
 *                  
 *	d s q l _ i n s e r t
 *
 **************************************
 *
 * Functional description
 *	Insert next record into a dynamic SQL cursor.
 *
 **************************************/
	RSR statement;
	MSG_SQLMSG message;
	SLONG status;

/* Check and validate handles, etc. */

	statement = *stmt_handle;
	CHECK_HANDLE(statement, type_rsr, gds__bad_req_handle);

/* set up the packet for the other guy... */

	message =
		(MSG_SQLMSG) CSS_alloc_message(type_msg,
									   (int) (sizeof(struct msg_sqlmsg) +
											  blr_length + msg_length));
	message->msg_sqlmsg_header.msg_type = MSG_insert;
	message->msg_sqlmsg_statement = statement->rsr_handle;
	message->msg_sqlmsg_in_blr_length = blr_length;
	MOVE(blr, message->msg_sqlmsg_data + msg_length, blr_length);
	message->msg_sqlmsg_in_msg_type = msg_type;
	message->msg_sqlmsg_in_msg_length = msg_length;
	MOVE(msg, message->msg_sqlmsg_data, msg_length);

	return get_response(statement->rsr_rdb, message, user_status, 0, 0, 0, 0);
}


STATUS GDS_DSQL_PREPARE(STATUS * user_status,
						RTR * rtr_handle,
						RSR * stmt_handle,
						USHORT length,
						TEXT * string,
						USHORT dialect,
						USHORT item_length,
						SCHAR * items, USHORT buffer_length, SCHAR * buffer)
{
/**************************************
 *
 *	d s q l _ p r e p a r e
 *
 **************************************
 *
 * Functional description
 *	Prepare a dynamic SQL statement for execution.
 *
 **************************************/
	RDB rdb;
	RTR transaction;
	RSR statement;
	MSG_PSTMT message;

/* Check and validate handles, etc. */

	statement = *stmt_handle;
	CHECK_HANDLE(statement, type_rsr, gds__bad_req_handle);
	rdb = statement->rsr_rdb;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);
	if (transaction = *rtr_handle)
		CHECK_HANDLE(transaction, type_rtr, gds__bad_trans_handle);

	if (!length)
		length = strlen(string);

/* set up the packet for the other guy... */

	message =
		(MSG_PSTMT) CSS_alloc_message(type_msg,
									  (int) (sizeof(struct msg_pstmt) +
											 length + item_length));
	message->msg_pstmt_header.msg_type = MSG_prepare_stmt;
	message->msg_pstmt_transaction =
		((transaction) ? transaction->rtr_handle : NULL);
	message->msg_pstmt_statement = statement->rsr_handle;
	message->msg_pstmt_SQL_length = length;
	MOVE(string, message->msg_pstmt_data, length);
	message->msg_pstmt_SQL_dialect = dialect;
	message->msg_pstmt_item_length = item_length;
	MOVE(items, message->msg_pstmt_data + length, item_length);
	message->msg_pstmt_buffer_length = buffer_length;

	return get_response(rdb, message, user_status, 0, buffer_length, buffer,
						0);
}


STATUS GDS_DSQL_SET_CURSOR(STATUS * user_status,
						   RSR * stmt_handle, TEXT * cursor, USHORT type)
{
/*****************************************
 *
 *	d s q l _ s e t _ c u r s o r
 *
 *****************************************
 *
 * Functional Description
 *	Set a cursor name for a dynamic request.
 *
 *****************************************/
	RSR statement;
	USHORT length;
	MSG_SETCUR message;

/* Check and validate handles, etc. */

	statement = *stmt_handle;
	CHECK_HANDLE(statement, type_rsr, gds__bad_req_handle);

	length = name_length(cursor) + 1;

/* set up the packet for the other guy... */

	message =
		(MSG_SETCUR) CSS_alloc_message(type_msg,
									   (int) (sizeof(struct msg_setcur) +
											  length - 1));
	message->msg_setcur_header.msg_type = MSG_set_cursor;
	message->msg_setcur_statement = statement->rsr_handle;
	MOVE(cursor, message->msg_setcur_cursor, length);
	message->msg_setcur_type = type;

	return get_response(statement->rsr_rdb, message, user_status, 0, 0, 0, 0);
}


STATUS GDS_DSQL_SQL_INFO(STATUS * user_status,
						 RSR * stmt_handle,
						 SSHORT item_length,
						 SCHAR * items, SSHORT buffer_length, SCHAR * buffer)
{
/**************************************
 *
 *	d s q l _ s q l _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Provide information on sql object.
 *
 **************************************/
	RDB rdb;
	RSR statement;

/* Check and validate handles, etc. */

	statement = *stmt_handle;
	CHECK_HANDLE(statement, type_rsr, gds__bad_req_handle);
	rdb = statement->rsr_rdb;

	return info(rdb, user_status, statement->rsr_handle, MSG_sql_info, 0,
				item_length, items, buffer_length, buffer);
}


STATUS GDS_GET_SEGMENT(STATUS * user_status,
					   RBL * blob_handle,
					   USHORT * length, USHORT buffer_length, UCHAR * buffer)
{
/**************************************
 *
 *	g d s _ g e t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Abort a partially completed blob.
 *
 **************************************/
	RDB rdb;
	RBL blob;
	USHORT l;
	UCHAR *p;
	STATUS *v;
	HANDLE state;
	MSG_SEG message;

/* Sniff out handles, etc, and find the various blocks. */

	blob = *blob_handle;
	CHECK_HANDLE(blob, type_rbl, gds__bad_segstr_handle);
	rdb = blob->rbl_rdb;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

/* Ask for a 1K chunk of blob and fill segment requests from it until its time to
   get the next section.  In other words, get a bunch, pass it out piece by piece,
   then when there isn't enough left, ask for more. */

/* set up the status vector for the calls we're going to fake */

	v = user_status;
	*v++ = gds_arg_gds;
	v[0] = 0;
	v[1] = 0;
	*length = 0;

/* if we're already done, stop now */

	if (blob->rbl_flags & RBL_eof) {
		*v = gds__segstr_eof;
		return user_status[1];
	}

/* Here's the loop, passing out data from our basket & refilling it.
   Our buffer (described by the structure blob) is counted strings 
   <count word> <string> <count word> <string>... */

	while (TRUE) {
		/* If there's data to be given away, give some away (p points to the local data) */

		if (blob->rbl_length) {
			p = blob->rbl_ptr;

			/* If there was a fragment left over last time use it */

			if (l = blob->rbl_fragment_length)
				blob->rbl_fragment_length = 0;

			/* otherwise pick up the count word as the length, & decrement the local length */

			else {
				l = *p++;
				l += *p++ << 8;
				blob->rbl_length -= 2;
			}

			/* Now check that what we've got fits. 
			   If not, set up the fragment pointer and set the status vector */

			if (l > buffer_length) {
				blob->rbl_fragment_length = l - buffer_length;
				l = buffer_length;
				*v = gds__segment;
			}

			/* and, just for yucks, see if we're exactly using up the fragment
			   part of a previous incomplete read - if so mark this as an incomplete read */

			if (l == buffer_length &&
				l == blob->rbl_length && (blob->rbl_flags & RBL_segment))
				*v = gds__segment;

			/* finally set up the return length, decrement the current length,
			   copy the data, and indicate where to start next time. */

			*length += l;
			blob->rbl_length -= l;
			buffer_length -= l;

			if (l)
				do
					*buffer++ = *p++;
				while (--l);

			blob->rbl_ptr = p;

			/* return if we've filled up the caller's buffer, or completed a segment */

			if (!buffer_length ||
				blob->rbl_length || !(blob->rbl_flags & RBL_segment)) break;
		}

		/* We're done with buffer.  If this was the last, we're done */

		if (blob->rbl_flags & RBL_eof_pending) {
			blob->rbl_flags |= RBL_eof;
			*v = gds__segstr_eof;
			break;
		}

		/* We need more data.  Ask for it politely */

		message =
			(MSG_SEG) CSS_alloc_message(type_msg, sizeof(struct msg_seg));
		message->msg_seg_header.msg_type = MSG_get_segment_buf;
		message->msg_seg_handle = blob->rbl_handle;
		message->msg_seg_buffer_length = blob->rbl_buffer_length;

		if (get_response
			(rdb, message, user_status, &state, blob->rbl_buffer_length,
			 blob->rbl_buffer, &blob->rbl_length))
			return user_status[1];

		blob->rbl_ptr = blob->rbl_buffer;
		blob->rbl_flags &= ~RBL_segment;
		if ((SLONG) state == 1)
			blob->rbl_flags |= RBL_segment;
		else if ((SLONG) state == 2)
			blob->rbl_flags |= RBL_eof_pending;
	}

	return user_status[1];
}


STATUS GDS_GET_SLICE(STATUS * user_status,
					 RDB * db_handle,
					 RTR * rtr_handle,
					 BID array_id,
					 USHORT sdl_length,
					 UCHAR * sdl,
					 USHORT param_length,
					 UCHAR * param,
					 SLONG slice_length, UCHAR * slice, SLONG * return_length)
{
/**************************************
 *
 *	g d s _ g e t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *	Snatch a slice of an array.
 *
 **************************************/
	RDB rdb;
	RTR transaction;
	MSG_SLICE message;
	HANDLE handle;
	USHORT max_length, actual_length;

	rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);
	transaction = *rtr_handle;
	CHECK_HANDLE(transaction, type_rtr, gds__bad_trans_handle);

	if (!array_id->bid_relation_id && !array_id->bid_number) {
		while (slice_length--)
			*slice++ = 0;
		*return_length = 0;
		return SUCCESS;
	}

/* Since arrays can be more than 64K bytes and we limit central
   server messages to that length, we must be able to retrieve
   an array in pieces.  The fact of multiple messages will be
   transparent to the user. */

	handle = NULL;
	*return_length = 0;

	do {
		message = (MSG_SLICE) CSS_alloc_message(type_msg,
												(int) (sizeof
													   (struct msg_slice) +
													   sdl_length +
													   param_length - 1));
		message->msg_slice_header.msg_type = MSG_get_slice;
		message->msg_slice_database = rdb->rdb_handle;
		message->msg_slice_transaction = transaction->rtr_handle;
		message->msg_slice_sdl_length = sdl_length;
		message->msg_slice_param_length = param_length;
		message->msg_slice_slice_length = slice_length;
		MOVE(array_id, message->msg_slice_id, 8);
		message->msg_slice_handle = handle;
		MOVE(sdl, message->msg_slice_data, sdl_length);
		MOVE(param, message->msg_slice_data + sdl_length, param_length);

		max_length = MIN(slice_length, MAX_ARRAY_MESSAGE);
		if (!get_response
			(rdb, message, user_status, &handle, max_length, slice,
			 &actual_length)) {
			slice_length -= (SLONG) actual_length;
			slice += actual_length;
			*return_length += actual_length;
		}

		/* Once one message has been sent, don't bother to send
		   the sdl or param arguments. */

		sdl_length = param_length = 0;

	} while (!user_status[1] && handle);

	return user_status[1];
}


#ifdef GATEWAY
#ifndef PIPE_SERVER
STATUS CSI_mdi_attach_db(STATUS * user_status,
						 SSHORT file_length, SCHAR * file_name, RDB * handle)
{
/**************************************
 *
 *	C S I _ m d i _ a t t a c h _ d b
 *
 **************************************
 *
 * Functional description
 *	Create a nice, squeeky clean database, uncorrupted by user data.
 *
 **************************************/
	RDB rdb;
	MSG_OP message;

/* Check and validate handles, etc. */

	rdb = *handle;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	message =
		(MSG_OP) CSS_alloc_message(type_msg,
								   (int) (sizeof(struct msg_op) +
										  file_length));
	message->msg_op_header.msg_type = MSG_mdi_attach_db;
	message->msg_op_handle = rdb->rdb_handle;
	message->msg_op_length = file_length;
	MOVE(file_name, message->msg_op_data, file_length);

	return get_response(rdb, message, user_status, 0, 0, 0, 0);
}
#endif
#endif


STATUS GDS_OPEN_BLOB2(STATUS * user_status,
					  RDB * db_handle,
					  RTR * rtr_handle,
					  RBL * blob_handle,
					  BID blob_id, USHORT bpb_length, UCHAR * bpb)
{
/**************************************
 *
 *	g d s _ o p e n _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Open an existing blob.
 *
 **************************************/
	RDB rdb;
	RTR transaction;
	RBL blob;
	HANDLE handle;
	MSG_BLOB message;

	NULL_CHECK(blob_handle, gds__bad_segstr_handle);
	rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);
	transaction = *rtr_handle;
	CHECK_HANDLE(transaction, type_rtr, gds__bad_trans_handle);

	message =
		(MSG_BLOB) CSS_alloc_message(type_msg,
									 (int) (sizeof(struct msg_blob) +
											bpb_length - 1));
	message->msg_blob_header.msg_type = MSG_open_blob;
	message->msg_blob_database = rdb->rdb_handle;
	message->msg_blob_transaction = transaction->rtr_handle;
	message->msg_blob_bpb_length = bpb_length;
	MOVE(bpb, message->msg_blob_bpb, bpb_length);
	MOVE(blob_id, message->msg_blob_id, 8);

	if (!get_response(rdb, message, user_status, &handle, 8, blob_id, 0)) {
		*blob_handle = blob =
			(RBL) ALLOC(type_rbl, sizeof(struct rbl) + BLOB_LENGTH);
		blob->rbl_handle = handle;
		blob->rbl_buffer_length = BLOB_LENGTH;
		blob->rbl_rdb = rdb;
		blob->rbl_rtr = transaction;
		blob->rbl_ptr = blob->rbl_buffer;
		blob->rbl_next = transaction->rtr_blobs;
		transaction->rtr_blobs = blob;
	}

	return user_status[1];
}


#ifdef PIPE_SERVER
STATUS GDS_OPEN_BLOB(STATUS * user_status,
					 RDB * db_handle,
					 RTR * rtr_handle, RBL * blob_handle, BID blob_id)
{
/**************************************
 *
 *	g d s _ o p e n _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Open an existing blob.
 *
 **************************************/

	return GDS_OPEN_BLOB2(user_status, db_handle, rtr_handle, blob_handle,
						  blob_id, 0, NULL);
}
#endif


STATUS GDS_PREPARE(STATUS * user_status,
				   RTR * rtr_handle, USHORT buffer_length, UCHAR * buffer)
{
/**************************************
 *
 *	g d s _ p r e p a r e
 *
 **************************************
 *
 * Functional description
 *	Prepare a transaction for commit.  First phase of a two
 *	phase commit.
 *
 **************************************/
	RDB rdb;
	RTR transaction;
	MSG_OP message;

	transaction = *rtr_handle;
	CHECK_HANDLE(transaction, type_rtr, gds__bad_trans_handle);
	rdb = transaction->rtr_rdb;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	message =
		(MSG_OP) CSS_alloc_message(type_msg,
								   (int) (sizeof(struct msg_op) +
										  buffer_length));
	message->msg_op_header.msg_type = MSG_prepare;
	message->msg_op_handle = transaction->rtr_handle;
	message->msg_op_length = buffer_length;
	MOVE(buffer, message->msg_op_data, buffer_length);

	return get_response(rdb, message, user_status, 0, 0, 0, 0);
}


STATUS GDS_PUT_SEGMENT(STATUS * user_status,
					   RBL * blob_handle,
					   USHORT buffer_length, UCHAR * buffer)
{
/**************************************
 *
 *	g d s _ p u t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Abort a partially completed blob.
 *
 **************************************/
	RDB rdb;
	RBL blob;
	USHORT l;
	UCHAR *p;

/* Sniff out handles, etc, and find the various blocks. */

	blob = *blob_handle;
	CHECK_HANDLE(blob, type_rbl, gds__bad_segstr_handle);
	rdb = blob->rbl_rdb;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

/* set up the status vector for the calls we're going to fake */

	user_status[0] = gds_arg_gds;
	user_status[1] = 0;
	user_status[2] = gds_arg_end;

/* If the buffer is too full to even hold the length of the incoming
   segment, flush out the old buffer.  If the segment is too large
   to fit into the blob buffer, just send it as a single segment. */

	p = blob->rbl_ptr;
	l = blob->rbl_buffer_length - (p - blob->rbl_buffer);

	if (buffer_length + 2 > l) {
		if (blob->rbl_ptr > blob->rbl_buffer)
			send_blob(user_status, blob, 0, NULL);
		if (buffer_length + 2 > blob->rbl_buffer_length)
			return send_blob(user_status, blob, buffer_length, buffer);
		p = blob->rbl_buffer;
	}

/* Move segment length and data into blob buffer */

	*p++ = buffer_length;
	*p++ = buffer_length >> 8;

	if (buffer_length)
		do
			*p++ = *buffer++;
		while (--buffer_length);

	blob->rbl_ptr = p;

	return user_status[1];
}


STATUS GDS_PUT_SLICE(STATUS * user_status,
					 RDB * db_handle,
					 RTR * rtr_handle,
					 BID array_id,
					 USHORT sdl_length,
					 UCHAR * sdl,
					 USHORT param_length,
					 UCHAR * param, SLONG slice_length, UCHAR * slice)
{
/**************************************
 *
 *	g d s _ p u t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *	Snatch a slice of an array.
 *
 **************************************/
	RDB rdb;
	RTR transaction;
	MSG_SLICE message;
	HANDLE handle;
	USHORT length, use_sdl_length, use_param_length;

	rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);
	transaction = *rtr_handle;
	CHECK_HANDLE(transaction, type_rtr, gds__bad_trans_handle);

/* Since arrays can be more than 64K bytes and we limit central
   server messages to that length, we must be able to send
   an array in pieces.  The fact of multiple messages will be
   transparent to the user. */

	handle = NULL;

	do {
		if ((length = MIN(slice_length, MAX_ARRAY_MESSAGE)) < slice_length)
			use_sdl_length = use_param_length = 0;
		else {
			use_sdl_length = sdl_length;
			use_param_length = param_length;
		}

		message = (MSG_SLICE) CSS_alloc_message(type_msg,
												(int) (sizeof
													   (struct msg_slice) +
													   use_sdl_length +
													   use_param_length +
													   length - 1));
		message->msg_slice_header.msg_type = MSG_put_slice;
		message->msg_slice_database = rdb->rdb_handle;
		message->msg_slice_transaction = transaction->rtr_handle;
		message->msg_slice_sdl_length = use_sdl_length;
		message->msg_slice_param_length = use_param_length;
		message->msg_slice_slice_length = slice_length;
		MOVE(array_id, message->msg_slice_id, 8);
		message->msg_slice_handle = handle;
		MOVE(sdl, message->msg_slice_data, use_sdl_length);
		MOVE(param, message->msg_slice_data + use_sdl_length,
			 use_param_length);

		MOVE(slice,
			 message->msg_slice_data + use_sdl_length + use_param_length,
			 length);

		if (!get_response(rdb, message, user_status, &handle, 8, array_id, 0)) {
			slice_length -= (SLONG) length;
			slice += length;
		}

	} while (!user_status[1] && handle);

	return user_status[1];
}


STATUS GDS_QUE_EVENTS(STATUS * user_status,
					  RDB * handle,
					  SLONG * id,
					  SSHORT length, UCHAR * items, void (*ast) (), void *arg)
{
/**************************************
 *
 *	g d s _ $ q u e _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *	Que a request for event notification.
 *
 **************************************/
	MSG_EVENT message;
	MSG_OP connect;
	MSG_RESP response;
	RDB rdb;

	rdb = *handle;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	if (!rdb->rdb_connection2) {
		rdb->rdb_connection2 = CSS_connect(rdb->rdb_server);
		connect =
			(MSG_OP) CSS_alloc_message(type_msg,
									   (int) (sizeof(struct msg_op) +
											  length));
		connect->msg_op_header.msg_type = MSG_alt_connection;
		connect->msg_op_handle = rdb->rdb_handle;
		CSS_put_message(rdb->rdb_connection2, connect, 0);
		response = CSS_get_message(rdb->rdb_connection2, 0, 0);
		CSS_free_global(response);
#ifndef GATEWAY
#ifdef MULTI_THREAD
		gds__thread_start(event_thread, rdb->rdb_connection2);
#else
		ISC_signal(EVENT_SIGNAL, event_handler, rdb->rdb_connection2);
#endif
#endif
	}

	message =
		(MSG_EVENT) CSS_alloc_message(type_msg,
									  (int) (sizeof(struct msg_event) +
											 length));
	message->msg_event_header.msg_type = MSG_que_events;
	message->msg_event_database = rdb->rdb_handle;
	message->msg_event_ast = ast;
	message->msg_event_arg = arg;
	message->msg_event_length = length;
	MOVE(items, message->msg_event_data, length);

	return get_response(rdb, message, user_status, id, 0, 0, 0);
}


#ifdef GATEWAY
#ifndef PIPE_SERVER
STATUS CSI_query_connect(STATUS * user_status,
						 SSHORT file_length,
						 SCHAR * file_name,
						 RDB * handle,
						 TEXT * connect_method, SSHORT * connect_length)
{
/**************************************
 *
 *	C S I _ q u e r y _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Attach to a central server and return the "connection method".
 *
 **************************************/
	RDB rdb;

	NULL_CHECK(handle, gds__bad_db_handle);

	if (rdb = init(user_status, MSG_query_connect,
				   file_name, file_length, 0, 0, file_name,
				   PRB_server_t1, PRB_client_t1, connect_method,
				   connect_length)) *handle = rdb;

	return user_status[1];
}
#endif
#endif


STATUS GDS_RECEIVE(STATUS * user_status,
				   RRQ * req_handle,
				   USHORT msg_type,
				   USHORT msg_length, UCHAR * msg, SSHORT level)
{
/**************************************
 *
 *	g d s _ R E C E I V E
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
	RRQ request;
	RTR transaction;
	RDB rdb;
	MSG_MSG message;

	request = *req_handle;
	CHECK_HANDLE(request, type_rrq, gds__bad_req_handle);
	rdb = request->rrq_rdb;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	message = (MSG_MSG) CSS_alloc_message(type_msg, sizeof(struct msg_msg));
	message->msg_msg_header.msg_type = MSG_receive;
	message->msg_msg_request = request->rrq_handle;
	message->msg_msg_length = msg_length;
	message->msg_msg_level = level;
	message->msg_msg_type = msg_type;

	return get_response(rdb, message, user_status, 0, msg_length, msg, 0);
}


STATUS GDS_RECONNECT(STATUS * user_status,
					 RDB * db_handle,
					 RTR * rtr_handle, SSHORT length, UCHAR * id)
{
/**************************************
 *
 *	G D S _ R E C O N N E C T
 *
 **************************************
 *
 * Functional description
 *	Connect to a transaction in limbo.
 *
 **************************************/
	RDB rdb;
	RTR transaction;
	HANDLE handle;
	MSG_OP message;

	NULL_CHECK(rtr_handle, gds__bad_trans_handle);
	rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	message =
		(MSG_OP) CSS_alloc_message(type_msg,
								   (int) (sizeof(struct msg_op) + length));
	message->msg_op_header.msg_type = MSG_reconnect;
	message->msg_op_handle = rdb->rdb_handle;
	message->msg_op_length = length;
	MOVE(id, message->msg_op_data, length);

	if (!get_response(rdb, message, user_status, &handle, 0, 0, 0))
		*rtr_handle = make_transaction(rdb, handle);

	return user_status[1];
}


STATUS GDS_RELEASE_REQUEST(STATUS * user_status, RRQ * req_handle)
{
/**************************************
 *
 *	g d s _ R E L E A S E _ R E Q U E S T
 *
 **************************************
 *
 * Functional description
 *	Release a request.
 *
 **************************************/
	RDB rdb;
	RRQ request;

	request = *req_handle;
	CHECK_HANDLE(request, type_rrq, gds__bad_req_handle);
	rdb = request->rrq_rdb;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	if (!release_object(rdb, MSG_release, request->rrq_handle, user_status)) {
		release_request(request);
		*req_handle = NULL;
	}

	return user_status[1];
}


STATUS GDS_REQUEST_INFO(STATUS * user_status,
						RRQ * request,
						USHORT level,
						SSHORT item_length,
						UCHAR * items, SSHORT buffer_length, UCHAR * buffer)
{
/**************************************
 *
 *	g d s _ r e q u e s t _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Provide information on request object.
 *
 **************************************/
	RDB rdb;

	CHECK_HANDLE((*request), type_rrq, gds__bad_req_handle);
	rdb = (*request)->rrq_rdb;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	return info(rdb, user_status, (*request)->rrq_handle, MSG_request_info,
				level, item_length, items, buffer_length, buffer);
}


STATUS GDS_ROLLBACK(STATUS * user_status, RTR * rtr_handle)
{
/**************************************
 *
 *	g d s _ R O L L B A C K
 *
 **************************************
 *
 * Functional description
 *	Abort a transaction.
 *
 **************************************/
	RDB rdb;
	RTR transaction;

	transaction = *rtr_handle;
	CHECK_HANDLE(transaction, type_rtr, gds__bad_trans_handle);
	rdb = transaction->rtr_rdb;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	if (!release_object
		(rdb, MSG_rollback, transaction->rtr_handle, user_status)) {
		release_transaction(transaction);
		*rtr_handle = NULL;
	}

	return user_status[1];
}


STATUS GDS_SEND(STATUS * user_status,
				RRQ * req_handle,
				USHORT msg_type, USHORT msg_length, UCHAR * msg, SSHORT level)
{
/**************************************
 *
 *	g d s _ s e n d
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
	RRQ request;
	RTR transaction;
	RDB rdb;
	MSG_MSG message;

	request = *req_handle;
	CHECK_HANDLE(request, type_rrq, gds__bad_req_handle);
	rdb = request->rrq_rdb;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	message =
		(MSG_MSG) CSS_alloc_message(type_msg,
									(int) (sizeof(struct msg_msg) +
										   msg_length));
	message->msg_msg_header.msg_type = MSG_send;
	message->msg_msg_request = request->rrq_handle;
	message->msg_msg_length = msg_length;
	message->msg_msg_level = level;
	message->msg_msg_type = msg_type;
	MOVE(msg, message->msg_msg_data, msg_length);

	return get_response(rdb, message, user_status, 0, 0, 0, 0);
}


STATUS GDS_SEEK_BLOB(STATUS * user_status,
					 RBL * blob_handle,
					 SSHORT mode, SLONG offset, SLONG * result)
{
/**************************************
 *
 *	g d s _ s e e k _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Abort a partially completed blob.
 *
 **************************************/
	RDB rdb;
	RBL blob;
	MSG_SEEK message;

/* Sniff out handles, etc, and find the various blocks. */

	blob = *blob_handle;
	CHECK_HANDLE(blob, type_rbl, gds__bad_segstr_handle);
	rdb = blob->rbl_rdb;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	message = (MSG_SEEK) CSS_alloc_message(type_msg, sizeof(struct msg_seek));
	message->msg_seek_header.msg_type = MSG_seek_blob;
	message->msg_seek_handle = blob->rbl_handle;
	message->msg_seek_mode = mode;
	message->msg_seek_offset = offset;

	return get_response(rdb, message, user_status, 0, sizeof(*result), result,
						0);
}



STATUS GDS_START_AND_SEND(STATUS * user_status,
						  RRQ * req_handle,
						  RTR * rtr_handle,
						  USHORT msg_type,
						  USHORT msg_length, UCHAR * msg, SSHORT level)
{
/**************************************
 *
 *	g d s _ s t a r t _ a n d _ s e n d
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
	RRQ request;
	RTR transaction;
	RDB rdb;
	MSG_MSG message;

	request = *req_handle;
	CHECK_HANDLE(request, type_rrq, gds__bad_req_handle);
	transaction = *rtr_handle;
	CHECK_HANDLE(transaction, type_rtr, gds__bad_trans_handle);
	rdb = request->rrq_rdb;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	message =
		(MSG_MSG) CSS_alloc_message(type_msg,
									(int) (sizeof(struct msg_msg) +
										   msg_length));
	message->msg_msg_header.msg_type = MSG_start_and_send;
	message->msg_msg_request = request->rrq_handle;
	message->msg_msg_transaction = transaction->rtr_handle;
	message->msg_msg_length = msg_length;
	message->msg_msg_level = level;
	message->msg_msg_type = msg_type;
	MOVE(msg, message->msg_msg_data, msg_length);

	return get_response(rdb, message, user_status, 0, 0, 0, 0);
}


STATUS GDS_START(STATUS * user_status,
				 register RRQ * req_handle,
				 register RTR * rtr_handle, SSHORT level)
{
/**************************************
 *
 *	g d s _ S T A R T
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
	RRQ request;
	RTR transaction;
	RDB rdb;
	MSG_MSG message;

	request = *req_handle;
	CHECK_HANDLE(request, type_rrq, gds__bad_req_handle);
	transaction = *rtr_handle;
	CHECK_HANDLE(transaction, type_rtr, gds__bad_trans_handle);
	rdb = request->rrq_rdb;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	message = (MSG_MSG) CSS_alloc_message(type_msg, sizeof(struct msg_msg));
	message->msg_msg_header.msg_type = MSG_start_request;
	message->msg_msg_request = request->rrq_handle;
	message->msg_msg_transaction = transaction->rtr_handle;
	message->msg_msg_level = level;

	return get_response(rdb, message, user_status, 0, 0, 0, 0);
}


STATUS GDS_START_TRANSACTION(STATUS * user_status,
							 RTR * tra_handle, SSHORT count, ...)
{
/**************************************
 *
 *	g d s _ $ s t a r t _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Start a transaction.
 *
 **************************************/
	TEB tebs[16], *teb, *end;
	va_list ptr;

	teb = tebs;
	end = teb + count;
	VA_START(ptr, count);

	for (; teb < end; teb++) {
		teb->teb_database = va_arg(ptr, RDB *);
		teb->teb_tpb_length = va_arg(ptr, int);
		teb->teb_tpb = va_arg(ptr, UCHAR *);
	}

	return GDS_START_MULTIPLE(user_status, tra_handle, count, tebs);
}


STATUS GDS_TRANSACT_REQUEST(STATUS * user_status,
							RDB * db_handle,
							RTR * tra_handle,
							USHORT blr_length,
							UCHAR * blr,
							USHORT in_msg_length,
							UCHAR * in_msg,
							USHORT out_msg_length, UCHAR * out_msg)
{
/**************************************
 *
 *	i s c _ t r a n s a c t _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Execute a stored procedure.
 *
 **************************************/
	RTR transaction;
	RDB rdb;
	MSG_TRRQ message;
	HANDLE handle;

/* Check and validate handles, etc. */

	rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);
	if (transaction = *tra_handle)
		CHECK_HANDLE(transaction, type_rtr, gds__bad_trans_handle);

/* set up the packet for the other guy... */

	message =
		(MSG_TRRQ) CSS_alloc_message(type_msg, blr_length + in_msg_length);
	message->msg_trrq_header.msg_type = MSG_transact_request;
	message->msg_trrq_database = rdb->rdb_handle;
	message->msg_trrq_transaction =
		(transaction) ? transaction->rtr_handle : NULL;
	message->msg_trrq_blr_length = blr_length;
	MOVE(blr, message->msg_trrq_data, blr_length);
	message->msg_trrq_in_msg_length = in_msg_length;
	MOVE(in_msg, message->msg_trrq_data + blr_length, in_msg_length);
	message->msg_trrq_out_msg_length = out_msg_length;

	if (!get_response
		(rdb, message, user_status, &handle, out_msg_length, out_msg, 0))
		if (transaction && !handle) {
			release_transaction(transaction);
			*tra_handle = NULL;
		}
		else if (!transaction && handle)
			*tra_handle = make_transaction(rdb, handle);

	return user_status[1];
}


STATUS GDS_TRANSACTION_INFO(STATUS * user_status,
							RTR * transaction,
							SSHORT item_length,
							UCHAR * items,
							SSHORT buffer_length, UCHAR * buffer)
{
/**************************************
 *
 *	g d s _ t r a n s a c t i o n _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Provide information on transaction object.
 *
 **************************************/
	RDB rdb;

	CHECK_HANDLE((*transaction), type_rtr, gds__bad_trans_handle);
	rdb = (*transaction)->rtr_rdb;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	return info(rdb, user_status, (*transaction)->rtr_handle,
				MSG_transaction_info, 0, item_length, items, buffer_length,
				buffer);
}


STATUS GDS_UNWIND(STATUS * user_status, RRQ * req_handle, SSHORT level)
{
/**************************************
 *
 *	g d s _ U N W I N D
 *
 **************************************
 *
 * Functional description
 *	Unwind a running request.
 *
 **************************************/
	RDB rdb;
	RRQ request;
	MSG_OP message;

	request = *req_handle;
	CHECK_HANDLE(request, type_rrq, gds__bad_req_handle);
	rdb = request->rrq_rdb;
	CHECK_HANDLE(rdb, type_rdb, gds__bad_db_handle);

	user_status[0] = gds_arg_gds;
	user_status[1] = 0;
	user_status[2] = 0;

#ifdef GATEWAY
#ifndef PIPE_SERVER
	message = (MSG_OP) CSS_alloc_message(type_msg, sizeof(struct msg_op));
	message->msg_op_header.msg_type = MSG_unwind;
	message->msg_op_handle = request->rrq_handle;
	message->msg_op_level = level;

	get_response(rdb, message, user_status, 0, 0, 0, 0);
#endif
#endif

	return user_status[1];
}


#ifndef MULTI_THREAD
static void event_handler( PTR connection)
{
/**************************************
 *
 *	e v e n t _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Wait for something to happen.
 *
 **************************************/
	MSG_EVENT message;

	if (!(message = (MSG_EVENT) CSS_get_message(connection, 0, -1)) ||
		message->msg_event_header.msg_type != MSG_event)
		return;

	(*message->msg_event_ast) (message->msg_event_arg,
							   message->msg_event_length,
							   message->msg_event_data);

	CSS_free_global(message);
}
#endif


#ifdef MULTI_THREAD
static void event_thread( PTR connection)
{
/**************************************
 *
 *	e v e n t _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *	Wait for something to happen.
 *
 **************************************/
	MSG_EVENT message;

	THREAD_ENTER;

	for (;;) {
		if (!(message = (MSG_EVENT) CSS_get_message(connection, 0, 0)) ||
			message->msg_event_header.msg_type != MSG_event)
			break;
		(*message->msg_event_ast) (message->msg_event_arg,
								   message->msg_event_length,
								   message->msg_event_data);
		CSS_free_global(message);
	}
}
#endif


static STATUS get_response(
						   RDB rdb,
						   MSG message,
						   STATUS * user_status,
						   HANDLE * handle,
USHORT buffer_length, UCHAR * buffer, USHORT * return_length)
{
/**************************************
 *
 *	g e t _ r e s p o n s e
 *
 **************************************
 *
 * Functional description
 *	Send message to server, await and process reply.
 *
 **************************************/
	MSG_RESP response;

	if (CSS_put_message(rdb->rdb_connection, message, 0))
		response = CSS_get_message(rdb->rdb_connection, 0, 0);
	else
		response = NULL;

	return process_response(response, user_status, handle, buffer_length,
							buffer, return_length);
}


static STATUS handle_error( STATUS * user_status, STATUS code)
{
/**************************************
 *
 *	h a n d l e _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	An invalid handle has been passed in.  If there is a user status
 *	vector, make it reflect the error.  If not, emulate the routine
 *	"error" and abort.
 *
 **************************************/
	STATUS local_status[20];
	STATUS *vector;

	vector = (user_status) ? user_status : local_status;
	vector[0] = 1;
	vector[1] = code;
	vector[2] = 0;

	return vector[1];
}


static STATUS info(
				   RDB rdb,
				   STATUS * user_status,
				   HANDLE handle,
				   MSG_T type,
				   USHORT level,
USHORT item_length, UCHAR * items, USHORT buffer_length, UCHAR * buffer)
{
/**************************************
 *
 *	i n f o
 *
 **************************************
 *
 * Functional description
 *	Generic information call.
 *
 **************************************/
	MSG_INFO message;

	message =
		(MSG_INFO) CSS_alloc_message(type_msg,
									 (int) (sizeof(struct msg_info) +
											item_length));
	message->msg_info_header.msg_type = type;
	message->msg_info_handle = handle;
	message->msg_info_level = level;
	message->msg_info_length = item_length;
	message->msg_info_buffer_length = buffer_length;
	MOVE(items, message->msg_info_data, item_length);

	return get_response(rdb, message, user_status, 0, buffer_length, buffer,
						0);
}


#ifndef PIPE_SERVER
static RDB init(
				STATUS * user_status,
				MSG_T type,
				UCHAR * file_name,
				USHORT file_length,
				UCHAR * dpb,
USHORT dpb_length,
TEXT * expanded_filename,
USHORT server_flags,
USHORT client_flags, TEXT * buffer, USHORT * buffer_length)
{
/**************************************
 *
 *	i n i t		( c e n t r a l _ s e r v e r )
 *
 **************************************
 *
 * Functional description
 *	Initialize for database access.  First call from both CREATE and
 *	OPEN.
 *
 **************************************/
	MSG_ATT message;
	MSG_RESP response;
	RDB rdb;
	SRQ *que;
	PRB process;
	PTR connection, server, client;
	CSH CSS_header;
	HANDLE handle;
	USHORT expanded_length, length;
	SLONG process_numbers[256], *ptr, *end;

	if (!CSS_init(user_status, FALSE)) {
		*user_status++ = gds_arg_gds;
		*user_status++ = gds__unavailable;
		*user_status = 0;
		return NULL;
	}

	if (!file_length)
		file_length = strlen(file_name);

	expanded_length = strlen(expanded_filename);

	client_flags |= PRB_client;
	server_flags |= PRB_server;

	client = CSS_create_process(client_flags);

/* Search for potential cooperative server processes.  Keep track of their
   server numbers until later. */

	CSS_header = ACQUIRE;
	CSS_probe_processes();
	end = process_numbers;

	QUE_LOOP(CSS_header->csh_processes, que) {
		process = (PRB) ((UCHAR *) que - OFFSET(PRB, prb_processes));
		if (client != REL_PTR(process) &&
			(process->prb_flags & server_flags) == server_flags &&
			process->prb_protocol_version == CSI_PROTOCOL_VERSION)
			*end++ = process->prb_process_number;
	}

	RELEASE;

/* We've got the candidate servers numbers.  Now go back and talk to them */

	for (ptr = process_numbers; ptr < end; ptr++) {
		if (server = CSS_find_process(*ptr)) {
			connection = CSS_connect(server);
			length =
				sizeof(struct msg_att) + file_length + expanded_length +
				dpb_length;
			message = (MSG_ATT) CSS_alloc_message(type_msg, (int) length);
			message->msg_att_header.msg_type = type;
			message->msg_att_dpb_length = dpb_length;
			message->msg_att_file_length = file_length;
			message->msg_att_expanded_length = expanded_length;
			MOVE(file_name, message->msg_att_data, file_length);
			MOVE(expanded_filename, message->msg_att_data + file_length,
				 expanded_length);
			MOVE(dpb, message->msg_att_data + file_length + expanded_length,
				 dpb_length);
			if (CSS_put_message(connection, message, 0)
				&& (response = CSS_get_message(connection, 0, 0)) &&
#ifdef GATEWAY
				!process_response(response, user_status, &handle,
								  *buffer_length, buffer, buffer_length))
#else
				!process_response(response, user_status, &handle, 0, 0, 0))
#endif
			{
				rdb = (RDB) ALLOC(type_rdb, sizeof(struct rdb));
				rdb->rdb_handle = handle;
				rdb->rdb_connection = connection;
				rdb->rdb_server = server;
				return rdb;
			}
			CSS_disconnect(connection);
		}
	}

	return NULL;
}
#endif


#ifdef PIPE_SERVER
#ifndef GATEWAY
static RDB init(
				STATUS * user_status,
				MSG_T type,
				UCHAR * file_name,
				USHORT file_length,
				UCHAR * dpb,
USHORT dpb_length,
TEXT * expanded_filename,
USHORT server_flags,
USHORT client_flags, TEXT * buffer, USHORT * buffer_length)
{
/**************************************
 *
 *	i n i t		( p i p e _ s e r v e r )
 *
 **************************************
 *
 * Functional description
 *	Initialize for database access.  First call from both CREATE and
 *	OPEN.
 *
 **************************************/
	MSG_ATT message;
	MSG_RESP response;
	RDB rdb;
	SRQ *que;
	PRB process;
	PTR connection, client;
	CSH CSS_header;
	HANDLE handle;
	USHORT expanded_length, length;
	TEXT local_expanded[256], pipe_xl[64], version[16], var_name[16];
	SLONG status, len, itemnums[4], items[4], isc_level, var_type;
	STATUS local_status[20];

/* Assume that we don't want a PIPE. */

	user_status[0] = gds_arg_gds;
	user_status[1] = gds__unavailable;
	user_status[2] = 0;

/* If the user has explicitly asked for a local attachment, skip the pipe */

	if (!
		(length =
		 strncmp(expanded_filename, AM_SENTINEL, sizeof(AM_SENTINEL) - 1))
|| !CSS_init(local_status, FALSE, 0)) {
		if (!length) {
			/* Re-expand the filename, minus the sentinel */

			strcpy(local_expanded,
				   expanded_filename + sizeof(AM_SENTINEL) - 1);
			ISC_expand_filename(local_expanded, strlen(local_expanded),
								expanded_filename);
		}
		return NULL;
	}

/* See if the user explicitly requested a pipe attachment! */

	if (!strncmp(expanded_filename, PIPE_SENTINEL, sizeof(PIPE_SENTINEL) - 1)) {
		/* Re-expand the filename, minus the sentinel */

		expanded_filename += sizeof(PIPE_SENTINEL) - 1;
		ISC_expand_filename(expanded_filename, strlen(expanded_filename),
							local_expanded);
		expanded_filename = local_expanded;
	}
	else {
		/* Don't use a pipe for non-interactive attachments */

		HPCIGETVAR("HPINTERACTIVE", &status, VAR_BOOL_VALUE, &isc_level,
				   VAR_TYPE, &var_type);
		if (!status && var_type == VAR_BOOL_VALUE && isc_level == 0)
			return NULL;

		/* Also don't use a pipe when variable ISC_LOCAL[ISC_UDCLVL] is true */

		HPCIGETVAR("ISC_UDCLVL", &status, VAR_INT_VALUE, &isc_level, VAR_TYPE,
				   &var_type);
		if (!status && var_type == VAR_INT_VALUE) {
			sprintf(var_name, "ISC_LOCAL%d", isc_level);
			HPCIGETVAR(var_name, &status, VAR_BOOL_VALUE, &isc_level,
					   VAR_TYPE, &var_type);
			if (!status && var_type == VAR_BOOL_VALUE && isc_level == 1)
				return NULL;
		}
	}

	if (!file_length)
		file_length = strlen(file_name);

	expanded_length = strlen(expanded_filename);
	client_flags |= PRB_client;
	client = CSS_create_process(client_flags);

/* Check to see if child process has already been created.
   If so, make sure it is still alive. */

	if (child_pin) {
		GETPROCINFO(child_pin);
		if (ccode() != CCE)
			child_pin = 0;
	}

/* Create a child process if one doesn't yet exist. */

	if (child_pin)
		connection = CSS_connect(pipe_server);
	else {
		/* Create a program fid and an XL fid for the CREATEPROCESS call */

		version[0] = 0;
		HPCIGETVAR("ISC_UDCLVL", &status, VAR_INT_VALUE, &isc_level, VAR_TYPE,
				   &var_type);
		if (!status && var_type == VAR_INT_VALUE) {
			sprintf(var_name, "ISC_VERS%d", isc_level);
			HPCIGETVAR(var_name, &status, VAR_STR_VALUE, version, VAR_LENGTH,
					   &len);
			if (!status && len)
				version[len] = 0;
		}

		sprintf(pipe_program, GDS_PIPE, version);

		HPMYFILE(pipe_xl, &status, &len);
		if (!status && len
			&& !strncmp(pipe_xl, " GDSLIB.", sizeof(" GDSLIB.") - 1))
			pipe_xl[len - 1] = 0;
		else
			pipe_xl[0] = pipe_xl[1] = 0;

		/* Create an item list for CREATEPROCESS.  The XL items must be at the
		   end because if we can't lookup the XL name we don't pass one in. */

		itemnums[0] = CP_ACT;
		items[0] = 0;
		itemnums[1] = pipe_xl[0] ? CP_XL : ITM_END;
		items[1] = (SLONG) & pipe_xl[1];
		itemnums[2] = CP_XLLEN;
		items[2] = strlen(items[1]);
		itemnums[3] = ITM_END;

		CREATEPROCESS(&status, &child_pin, pipe_program, itemnums, items);
		if (ccode() == CCL) {
			*user_status++ = gds_arg_gds;
			*user_status++ = gds__sys_request;
			*user_status++ = gds_arg_string;
			*user_status++ = (STATUS) "CREATEPROCESS";
			*user_status++ = gds_arg_string;
			*user_status++ = (STATUS) pipe_program;
			*user_status = gds_arg_end;
			return NULL;
		}

		gds__register_cleanup(exit_handler, 0);

		/* Wait for a message from our dear child. */

		while (!(response = CSS_get_message((SLONG) 0, 0, 10))) {
			GETPROCINFO(child_pin);
			if (ccode() != CCE) {
				child_pin = 0;
				return NULL;
			}
		}

		connection = response->msg_resp_header.msg_connection;
		CSS_free_global(response);

		/* Search for the child process. */

		CSS_header = ACQUIRE;
		pipe_server = 0;
		pipe_util_cnct = 0;

		QUE_LOOP(CSS_header->csh_processes, que) {
			process = (PRB) ((UCHAR *) que - OFFSET(PRB, prb_processes));
			if (client != REL_PTR(process) &&
				process->prb_protocol_version == CSI_PROTOCOL_VERSION &&
				process->prb_process_id == child_pin) {
				pipe_server = REL_PTR(process);
				break;
			}
		}

		RELEASE;
	}

/* We've got a candidate server.  Now go back and talk to it. */

	if (pipe_server) {
		length =
			sizeof(struct msg_att) + file_length + expanded_length +
			dpb_length;
		message = (MSG_ATT) CSS_alloc_message(type_msg, (int) length);
		message->msg_att_header.msg_type = type;
		message->msg_att_dpb_length = dpb_length;
		message->msg_att_file_length = file_length;
		message->msg_att_expanded_length = expanded_length;
		MOVE(file_name, message->msg_att_data, file_length);
		MOVE(expanded_filename, message->msg_att_data + file_length,
			 expanded_length);
		MOVE(dpb, message->msg_att_data + file_length + expanded_length,
			 dpb_length);
		if (CSS_put_message(connection, message, 0)
			&& (response = CSS_get_message(connection, 0, 0))
			&& !process_response(response, user_status, &handle, 0, 0, 0)) {
			rdb = (RDB) ALLOC(type_rdb, sizeof(struct rdb));
			rdb->rdb_handle = handle;
			rdb->rdb_connection = connection;
			rdb->rdb_server = pipe_server;

			/* At all times, keep a utility connection open to the pipe server.
			   This is for the sake of efficiency.  It will force the port to
			   remain open. */

			if (!pipe_util_cnct) {
				pipe_util_cnct = CSS_connect(pipe_server);
				message =
					(MSG_ATT) CSS_alloc_message(type_msg,
												sizeof(struct msg_util));
				((MSG_UTIL) message)->msg_util_header.msg_type = MSG_util_cmd;
				((MSG_UTIL) message)->msg_util_cmd = UTIL_noop;
				if (CSS_put_message(pipe_util_cnct, message, 0) &&
					(response = CSS_get_message(pipe_util_cnct, 0, 0)))
					CSS_free_global(response);
				else {
					CSS_disconnect(pipe_util_cnct);
					pipe_util_cnct = 0;
				}
			}

			return rdb;
		}
		CSS_disconnect(connection);
	}

	return NULL;
}
#endif


#ifdef GATEWAY
static RDB init(
				STATUS * user_status,
				MSG_T type,
				UCHAR * file_name,
				USHORT file_length,
				UCHAR * dpb, USHORT dpb_length, TEXT * expanded_filename)
{
/**************************************
 *
 *	i n i t		( g a t e w a y _ p i p e _ s e r v e r )
 *
 **************************************
 *
 * Functional description
 *	Initialize for database access.  First call from both CREATE and
 *	OPEN.
 *
 **************************************/
	MSG_ATT message;
	MSG_RESP response;
	RDB rdb;
	SRQ *que;
	PRB process;
	PTR connection, server, client;
	CSH CSS_header;
	HANDLE handle;
	UCHAR gbl_file[16], csi_file[256], *p, *q;
	USHORT expanded_length, length;
	int pid;

	sprintf(gbl_file, "ISC_%08lx", getpid());
	q = csi_file - 1;
	if (ISC_expand_logical_once
		("SYS$LOGIN", sizeof("SYS$LOGIN") - 1, csi_file)) {
		for (p = csi_file; *p; p++)
			if (*p == ':' || *p == ']')
				q = p;
		if (q < csi_file)
			*(q = p) = ':';
	}
	strcpy(++q, gbl_file);
#ifdef ORACLE_ALT
	strcat(q, "_ALT");
#endif

	if (!CSS_init(user_status, TRUE, csi_file)) {
		*user_status++ = gds_arg_gds;
		*user_status++ = gds__unavailable;
		*user_status = 0;
		return NULL;
	}

	if (!file_length)
		file_length = strlen(file_name);

	expanded_length = strlen(expanded_filename);

	client = CSS_create_process(PRB_server);
	CSS_header = ACQUIRE;

/* Try to find a user on the free list whose connection is
   still around.  Delete any that aren't. */

	CSS_probe_processes();
	while (CSI_free_servers) {
		QUE_LOOP(CSS_header->csh_processes, que)
			if (CSI_free_servers->rdb_server ==
				REL_PTR((PRB) ((UCHAR *) que - OFFSET(PRB, prb_processes))))
			break;

		if (que != &CSS_header->csh_processes)
			break;
		else {
			rdb = CSI_free_servers;
			CSI_free_servers = rdb->rdb_next;
			FREE(rdb);
		}
	}

	RELEASE;

/* If there aren't any available users, create one and then find it */

	if (!CSI_free_servers) {
		if ((pid = spawn(user_status, gbl_file, &connection)) > 0) {
			ACQUIRE;

			QUE_LOOP(CSS_header->csh_processes, que) {
				process = (PRB) ((UCHAR *) que - OFFSET(PRB, prb_processes));
				if (client != REL_PTR(process) &&
					process->prb_protocol_version == CSI_PROTOCOL_VERSION &&
					process->prb_process_id == pid) {
					rdb = (RDB) ALLOC(type_rdb, sizeof(struct rdb));
					rdb->rdb_next = CSI_free_servers;
					CSI_free_servers = rdb;
					rdb->rdb_connection = connection;
					rdb->rdb_server = REL_PTR(process);
					break;
				}
			}

			RELEASE;
		}
	}

	if (CSI_free_servers) {
		connection = CSI_free_servers->rdb_connection;

		length =
			sizeof(struct msg_att) + file_length + expanded_length +
			dpb_length;
		message = (MSG_ATT) CSS_alloc_message(type_msg, (int) length);
		message->msg_att_header.msg_type = type;
		message->msg_att_dpb_length = dpb_length;
		message->msg_att_file_length = file_length;
		message->msg_att_expanded_length = expanded_length;
		MOVE(file_name, message->msg_att_data, file_length);
		MOVE(expanded_filename, message->msg_att_data + file_length,
			 expanded_length);
		MOVE(dpb, message->msg_att_data + file_length + expanded_length,
			 dpb_length);
		if (CSS_put_message(connection, message, 0)
			&& (response = CSS_get_message(connection, 0, 0))
			&& !process_response(response, user_status, &handle, 0, 0, 0)) {
			rdb = CSI_free_servers;
			CSI_free_servers = rdb->rdb_next;

			rdb->rdb_handle = handle;
			rdb->rdb_next = CSI_databases;
			CSI_databases = rdb;
			return rdb;
		}
	}

	return NULL;
}
#endif
#endif


static RTR make_transaction( RDB rdb, HANDLE handle)
{
/**************************************
 *
 *	m a k e _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Create a local transaction handle.
 *
 **************************************/
	RTR transaction;

	transaction = (RTR) ALLOC(type_rtr, sizeof(struct rtr));
	transaction->rtr_rdb = rdb;
	transaction->rtr_handle = handle;
	transaction->rtr_next = rdb->rdb_transactions;
	rdb->rdb_transactions = transaction;

	return transaction;
}


static void move( UCHAR * from, UCHAR * to, USHORT length)
{
/**************************************
 *
 *	m o v e
 *
 **************************************
 *
 * Functional description
 *	Move some bytes.
 *
 **************************************/

	if (length)
		do
			*to++ = *from++;
		while (--length);
}


static USHORT name_length( TEXT * name)
{
/*****************************************
 *
 *	n a m e _ l e n g t h
 *
 *****************************************
 *
 * Functional Description
 *	Compute length of user-supplied (blank-padded) or
 * 	NULL-terminated buffer
 *
 *****************************************/
	TEXT *p;

	for (p = name; *p && *p != ' '; p++);

	return (USHORT) (p - name);
}


static STATUS process_response(
							   MSG_RESP message,
							   STATUS * user_status,
							   HANDLE * handle,
USHORT buffer_length, UCHAR * buffer, USHORT * return_length)
{
/**************************************
 *
 *	p r o c e s s _ r e s p o n s e
 *
 **************************************
 *
 * Functional description
 *	Process a response packet from server.
 *
 **************************************/
	TEXT *p, *q;
	STATUS *status, *stuff, *end;
	USHORT l;

/* If there isn't a message, the server has disappeared */

	if (!message || message->msg_resp_header.msg_type != MSG_response) {
		status = user_status;
		*status++ = gds_arg_gds;
		*status++ = gds__random;
		*status++ = gds_arg_string;
#ifndef PIPE_SERVER
		*status++ = (STATUS) "connection lost to central server";
#else
		*status++ = (STATUS) "connection lost to pipe server";
#endif
		*status = gds_arg_end;
		return user_status[1];
	}

/* Well, at least we got something -- eat it up */

	p = error_buffer;

	for (status = user_status, end = status + 20, stuff =
		 message->msg_resp_status; (*status = *stuff++) && status < end;)
		switch (*status++) {
		case gds_arg_interpreted:
		case gds_arg_string:
			q = (TEXT *) message + *stuff++;
			*status++ = (STATUS) p;
			while (*p++ = *q++);
			break;

		case gds_arg_cstring:
			l = *status++ = *stuff++;
			q = (TEXT *) message + *stuff++;
			*status++ = (STATUS) p;
			if (l)
				do
					*p++ = *q++;
				while (--l);
			break;

		default:
			*status++ = *stuff++;
		}

	if (!user_status[1] || user_status[1] == gds__segment) {
		if (handle)
			*handle = message->msg_resp_handle;
		if (buffer && (l = MIN(buffer_length, message->msg_resp_length)))
			MOVE(message->msg_resp_data, buffer, l);
		if (return_length)
			*return_length = message->msg_resp_length;
	}

	CSS_free_global(message);

	return user_status[1];
}


static void release_blob( RBL blob)
{
/**************************************
 *
 *	r e l e a s e _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Release a blob block and friends.
 *
 **************************************/
	RTR transaction;
	RBL *p;

	transaction = blob->rbl_rtr;

	for (p = &transaction->rtr_blobs; *p; p = &(*p)->rbl_next)
		if (*p == blob) {
			*p = blob->rbl_next;
			break;
		}

	FREE(blob);
}


static STATUS release_object(
							 RDB rdb,
							 MSG_T type, HANDLE handle, STATUS * user_status)
{
/**************************************
 *
 *	r e l e a s e _ o b j e c t
 *
 **************************************
 *
 * Functional description
 *	Tell the server to zap an object.  This doesn't not necessary
 *	release the object, but usually does.
 *
 **************************************/
	MSG_OP message;

	message = (MSG_OP) CSS_alloc_message(type_msg, sizeof(struct msg_op));
	message->msg_op_header.msg_type = type;
	message->msg_op_handle = handle;

	return get_response(rdb, message, user_status, 0, 0, 0, 0);
}


static void release_request( RRQ request)
{
/**************************************
 *
 *	r e l e a s e _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Release a request block and friends.
 *
 **************************************/
	RDB rdb;
	RRQ *p;

	rdb = request->rrq_rdb;

	for (p = &rdb->rdb_requests; *p; p = &(*p)->rrq_next)
		if (*p == request) {
			*p = request->rrq_next;
			break;
		}

	FREE(request);
}


static void release_sql_request( RSR stmt)
{
/**************************************
 *
 *	r e l e a s e _ s q l _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Release an SQL request block.
 *
 **************************************/
	RDB rdb;
	RSR *p;

	rdb = stmt->rsr_rdb;

	for (p = &rdb->rdb_sql_requests; *p; p = &(*p)->rsr_next)
		if (*p == stmt) {
			*p = stmt->rsr_next;
			break;
		}

	FREE(stmt);
}


static void release_transaction( RTR transaction)
{
/**************************************
 *
 *	r e l e a s e _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Release a transaction block and friends.
 *
 **************************************/
	RDB rdb;
	RTR *p;

	rdb = transaction->rtr_rdb;

	for (p = &rdb->rdb_transactions; *p; p = &(*p)->rtr_next)
		if (*p == transaction) {
			*p = transaction->rtr_next;
			break;
		}

	FREE(transaction);
}


static STATUS send_blob(
						STATUS * user_status,
						RBL blob, USHORT buffer_length, UCHAR * buffer)
{
/**************************************
 *
 *	s e n d _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Send a partially completed blob.
 *
 **************************************/
	MSG_T op;
	MSG_SEG message;

/* If there is no buffer address, this is a batch send.  Pick up the
   address and length from the blob buffer and blast away */

	if (!buffer) {
		buffer = blob->rbl_buffer;
		buffer_length = blob->rbl_ptr - buffer;
		blob->rbl_ptr = buffer;
		op = MSG_put_segment_buf;
	}
	else
		op = MSG_put_segment;

	message =
		(MSG_SEG) CSS_alloc_message(type_msg,
									(int) (sizeof(struct msg_seg) +
										   buffer_length));
	message->msg_seg_header.msg_type = op;
	message->msg_seg_handle = blob->rbl_handle;
	message->msg_seg_length = buffer_length;
	MOVE(buffer, message->msg_seg_data, buffer_length);

	return get_response(blob->rbl_rdb, message, user_status, 0, 0, 0, 0);
}


#ifdef PIPE_SERVER
#ifdef VMS
static void setup_creprc_info(
							  SLONG ** privileges,
							  ITMQ ** quotas, SLONG * priority)
{
/**************************************
 *
 *	s e t u p _ c r e p r c _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Get the information relating to process quotas and
 *	privileges that is useful when creating a sub-process.
 *
 **************************************/
	ITM items[16];
	SLONG values[16];
	SSHORT i, lengths[16];
	ITMQ *quota;

	for (i = sizeof(items) / sizeof(ITM); i--;) {
		items[i].itm_length = sizeof(SLONG);
		items[i].itm_buffer = &values[i];
		items[i].itm_return_length = &lengths[i];
	}
	items[10].itm_buffer = priority;
	items[11].itm_length = sizeof(SLONG) * 2;
	items[11].itm_buffer = *privileges;

	items[0].itm_code = JPI$_ASTLM;
	items[1].itm_code = JPI$_BIOLM;
	items[2].itm_code = JPI$_BYTLM;
	items[3].itm_code = JPI$_CPULIM;
	items[4].itm_code = JPI$_DIOLM;
	items[5].itm_code = JPI$_ENQLM;
	items[6].itm_code = JPI$_DFWSCNT;
	items[7].itm_code = JPI$_FILLM;
	items[8].itm_code = JPI$_PGFLQUOTA;
	items[9].itm_code = JPI$_PRCLM;
	items[10].itm_code = JPI$_PRIB;
	items[11].itm_code = JPI$_PROCPRIV;
	items[12].itm_code = JPI$_TQLM;
	items[13].itm_code = JPI$_WSQUOTA;
	items[14].itm_code = JPI$_WSAUTHEXT;

	items[15].itm_code = 0;
	items[15].itm_length = 0;

	if (sys$getjpiw(NULL, NULL, NULL, items, NULL, NULL, NULL) & 1) {
		for (quota = *quotas, i = sizeof(items) / sizeof(ITM); i--;)
			if (lengths[i]) {
				switch (items[i].itm_code) {
				case JPI$_ASTLM:
					quota->itmq_code = PQL$_ASTLM;
					break;

				case JPI$_BIOLM:
					quota->itmq_code = PQL$_BIOLM;
					break;

				case JPI$_BYTLM:
					quota->itmq_code = PQL$_BYTLM;
					break;

				case JPI$_CPULIM:
					quota->itmq_code = PQL$_CPULM;
					break;

				case JPI$_DIOLM:
					quota->itmq_code = PQL$_DIOLM;
					break;

				case JPI$_ENQLM:
					quota->itmq_code = PQL$_ENQLM;
					break;

				case JPI$_DFWSCNT:
					quota->itmq_code = PQL$_FILLM;
					break;

				case JPI$_FILLM:
					quota->itmq_code = PQL$_PGFLQUOTA;
					break;

				case JPI$_PGFLQUOTA:
					quota->itmq_code = PQL$_PRCLM;
					break;

				case JPI$_PRCLM:
					quota->itmq_code = PQL$_TQELM;
					break;

				case JPI$_TQLM:
					quota->itmq_code = PQL$_WSDEFAULT;
					break;

				case JPI$_WSQUOTA:
					quota->itmq_code = PQL$_WSEXTENT;
					break;

				case JPI$_WSAUTHEXT:
					quota->itmq_code = PQL$_WSQUOTA;
					break;

				default:
					continue;
				}
				
					(quota++)->itmq_value =
					(lengths[i] == 4) ? values[i] : (SSHORT) (values[i]);
			}
			else {
				if (items[i].itm_code == JPI$_PRIB)
					*priority = 4;
				else if (items[i].itm_code == JPI$_PROCPRIV)
					*privileges = NULL;
			}

		quota->itmq_code = PQL$_LISTEND;
	}
	else {
		*privileges = NULL;
		*quotas = NULL;
		*priority = 4;
	}
}
#endif
#endif


#ifdef PIPE_SERVER
#ifdef VMS
static int spawn( STATUS * user_status, UCHAR * gbl_file, PTR * connection)
{
/**************************************
 *
 *	s p a w n
 *
 **************************************
 *
 * Functional description
 *	Spawn a sub-process using SYS$CREPRC.  Return > 0 if a
 *	process was created, -1 if create process failed.  Also
 *	return a pointer to the created process's connection block.
 *
 **************************************/
	UCHAR output[128], error[128], *p, *q, process_name[16],
		pipe_temp[256], pipe_file[256];
	USHORT i, len;
	ULONG status, pid, flags, item;
	SLONG *privileges, procpriv[2], priority;
	ITMQ quota_list[14], *quotas;
	MSG_RESP response;
	struct dsc$descriptor_s desc1, desc2, desc3, desc4;

	privileges = procpriv;
	quotas = quota_list;
	setup_creprc_info(&privileges, &quotas, &priority);

/* Tell the sub-process about the values of some important logical names. */

	trans_logicals();

	gds__prefix(pipe_temp, GDS_PIPE);
	for (p = pipe_temp, q = p - 1; *p; p++)
		if (*p == ':')
			q = p;
	if (q < pipe_temp)
		strcpy(pipe_file, pipe_temp);
	else {
		len = ISC_expand_logical(pipe_temp, q - pipe_temp, pipe_file);
		if (pipe_file[len - 1] != ']')
			pipe_file[len++] = ':';
		strcpy(&pipe_file[len], q + 1);
	}

	q = error - 1;
	if (ISC_expand_logical_once("SYS$LOGIN", sizeof("SYS$LOGIN") - 1, error)) {
		for (p = error; *p; p++)
			if (*p == ':' || *p == ']')
				q = p;
		if (q < error)
			*(q = p) = ':';
	}
	for (p = "GDS_PIPE.ERR"; *++q = *p++;);
	len = q - error;

	strcpy(output, error);
	strcpy(&output[len - 3], "OUT");

	ISC_make_desc(pipe_file, &desc1, 0);
	ISC_make_desc(output, &desc2, len);
	ISC_make_desc(error, &desc3, len);
	i = 0;
	do {
		/* Loop until the sub-process is given a unique process name */

		sprintf(process_name, "%s_%02x", gbl_file, i++);
		ISC_make_desc(process_name, &desc4, 0);
		status = sys$creprc(&pid,	/* pidadr */
							&desc1,	/* image */
							NULL,	/* input */
							&desc2,	/* output */
							&desc3,	/* error */
							privileges,	/* prvadr */
							quotas,	/* quota */
							&desc4,	/* prcnam */
							priority,	/* baspri */
							NULL, NULL, NULL);
	} while (status == SS$_DUPLNAM);

	if (!(status & 1)) {
		/* We failed.  Fill in the status vector. */

		*user_status++ = gds_arg_gds;
		*user_status++ = gds__sys_request;
		*user_status++ = gds_arg_string;
		*user_status++ = (STATUS) "sys$creprc";
		*user_status++ = gds_arg_vms;
		*user_status++ = status;
		*user_status = gds_arg_end;
		return -1;
	}

	while (!(response = CSS_get_message((SLONG) 0, 0, 10))) {
		item = JPI$_PID;
		if (lib$getjpi(&item, &pid, NULL, NULL, NULL, NULL) != SS$_NORMAL)
			return -1;
	}

	*connection = response->msg_resp_header.msg_connection;
	CSS_free_global(response);

	return pid;
}
#endif
#endif


#ifdef PIPE_SERVER
#ifdef VMS
static void trans_logicals(void)
{
/**************************************
 *
 *	t r a n s _ l o g i c a l s
 *
 **************************************
 *
 * Functional description
 *	Translate some logicals and write their values
 *	to the job logical table.
 *
 **************************************/
	UCHAR **logicals, value[256], job_logical[32], buffer[256];
	int attr;
	SSHORT len;
	ITM items[2];
	struct dsc$descriptor_s tab_desc, log_desc;

	for (logicals = inherit_logicals; *logicals; logicals++) {
		items[0].itm_code = LNM$_STRING;
		items[0].itm_buffer = value;
		items[0].itm_length = sizeof(value);
		items[0].itm_return_length = &len;
		items[1].itm_length = 0;
		items[1].itm_code = 0;

		attr = LNM$M_CASE_BLIND;

		ISC_make_desc(*logicals, &log_desc, 0);
		ISC_make_desc("LNM$PROCESS", &tab_desc, sizeof("LNM$PROCESS") - 1);
		if (!(sys$trnlnm(&attr, &tab_desc, &log_desc, NULL, items) & 1))
			continue;

		/* We have a value to set.  Do so in the JOB table. */

		items[0].itm_length = len;

		sprintf(job_logical, "GDS_PIPE_%s", *logicals);
		ISC_make_desc(job_logical, &log_desc, 0);
		ISC_make_desc("LNM$JOB", &tab_desc, sizeof("LNM$JOB") - 1);

		sys$crelnm(NULL, &tab_desc, &log_desc, NULL, items);
	}
}
#endif
#endif
