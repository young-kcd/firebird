/*
 *	PROGRAM:	JRD Remote Interface
 *	MODULE:		interface.cpp
 *	DESCRIPTION:	User visible entrypoints remote interface
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
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "Ultrix" port
 *
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "MPEXL" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "DecOSF" port
 *
 * 2002.10.29 Sean Leyne - Removed support for obsolete IPX/SPX Protocol
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../remote/remote.h"
#include "../jrd/gdsassert.h"
#include "../jrd/jrd_proto.h"
#include <stdarg.h>

#ifndef NO_NFS
#ifndef VMS
#include <sys/param.h>
#endif
#endif

#include "../jrd/ibase.h"
#include "../jrd/thd.h"
#include "../jrd/license.h"
#include "../jrd/fil.h"
#include "../jrd/sdl.h"
#include "../jrd/jrd_pwd.h"
#include "../remote/inet_proto.h"
#include "../remote/inter_proto.h"
#include "../remote/merge_proto.h"
#include "../remote/parse_proto.h"
#include "../remote/remot_proto.h"
#include "../remote/proto_proto.h"
#include "../jrd/cvt_proto.h"
#include "../jrd/enc_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_f_proto.h"
#include "../jrd/sdl_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thread_proto.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#if defined(WIN_NT)
#define XNET
#include "../jrd/isc_proto.h"
#include "../remote/os/win32/wnet_proto.h"
#include "../remote/xnet_proto.h"
#endif

#ifdef VMS
#include "../remote/decne_proto.h"
#endif

#ifdef WIN_NT
#define sleep(seconds)		Sleep ((seconds) * 1000)

#include <direct.h>				// getcwd

#if defined(SUPERCLIENT)
static USHORT ostype = 0;
const USHORT OSTYPE_NT		= 1;
const USHORT OSTYPE_WIN_95	= 2;
#endif
#endif // WIN_NT

const char* ISC_USER		= "ISC_USER";
const char* ISC_PASSWORD	= "ISC_PASSWORD";
const int MAX_USER_LENGTH	= 33;
#define MAX_OTHER_PARAMS	(1 + 1 + sizeof(port->port_dummy_packet_interval))

static RVNT add_event(rem_port*);
static void add_other_params(rem_port*, UCHAR*, USHORT*);
static void add_working_directory(UCHAR*, USHORT*, const Firebird::PathName&);
static rem_port* analyze(Firebird::PathName&, ISC_STATUS*, const TEXT*,
					bool, const SCHAR*, SSHORT, Firebird::PathName&);
static rem_port* analyze_service(Firebird::PathName&, ISC_STATUS*, const TEXT*, 
					bool, const SCHAR*, SSHORT);
static bool batch_gds_receive(trdb *, rem_port*, struct rmtque *,
								 ISC_STATUS *, USHORT);
static bool batch_dsql_fetch(trdb *, rem_port*, struct rmtque *,
								ISC_STATUS *, USHORT);
static bool check_response(RDB, PACKET *);
static bool clear_queue(rem_port*, ISC_STATUS *);
static void disconnect(rem_port*);
#ifdef SCROLLABLE_CURSORS
static REM_MSG dump_cache(rem_port*, ISC_STATUS *, rrq::rrq_repeat *);
#endif
static void enqueue_receive(rem_port*,
							t_rmtque_fn,
							RDB, void*, rrq::rrq_repeat*);
static void dequeue_receive(rem_port*);
static ISC_STATUS error(ISC_STATUS *);
static ISC_STATUS error(ISC_STATUS* user_status, const std::exception& ex);
#ifndef MULTI_THREAD
static void event_handler(rem_port*);
#else
static THREAD_ENTRY_DECLARE event_thread(THREAD_ENTRY_PARAM);
#endif
static ISC_STATUS fetch_blob(ISC_STATUS*, RSR, USHORT, const UCHAR*, USHORT,
						USHORT, UCHAR*);
static RVNT find_event(rem_port*, SLONG);
static bool get_new_dpb(const UCHAR*, SSHORT, bool, UCHAR*, USHORT*, TEXT*);
#ifdef UNIX
static bool get_single_user(USHORT, const SCHAR*);
#endif
static ISC_STATUS handle_error(ISC_STATUS *, ISC_STATUS);
static ISC_STATUS info(ISC_STATUS*, RDB, P_OP, USHORT, USHORT, USHORT,
					const SCHAR*, USHORT, const SCHAR*, USHORT, SCHAR*);
static bool init(ISC_STATUS *, rem_port*, P_OP, const Firebird::PathName&, UCHAR *, USHORT);
static RTR make_transaction(RDB, USHORT);
static ISC_STATUS mov_dsql_message(const UCHAR*, const rem_fmt*, UCHAR*, const rem_fmt*);
static void move_error(ISC_STATUS, ...);
static void receive_after_start(rrq*, USHORT);
static bool receive_packet(rem_port*, PACKET *, ISC_STATUS *);
static bool receive_packet_noqueue(rem_port*, PACKET *, ISC_STATUS *);
static bool receive_queued_packet(trdb *, rem_port*, ISC_STATUS *, USHORT);
static bool receive_response(RDB, PACKET *);
static void release_blob(RBL);
static void release_event(RVNT);
static bool release_object(RDB, P_OP, USHORT);
static void release_request(rrq*);
static void release_statement(RSR *);
static void release_sql_request(RSR);
static void release_transaction(RTR);
static ISC_STATUS return_success(RDB);
#ifdef SCROLLABLE_CURSORS
static REM_MSG scroll_cache(ISC_STATUS *, trdb *, rrq*, rem_port*, rrq::rrq_repeat *,
						USHORT *, ULONG *);
#endif
static ISC_STATUS send_and_receive(RDB, PACKET *, ISC_STATUS *);
static ISC_STATUS send_blob(ISC_STATUS*, RBL, USHORT, const UCHAR*);
static void send_cancel_event(RVNT);
static bool send_packet(rem_port*, PACKET *, ISC_STATUS *);
#ifdef NOT_USED_OR_REPLACED
static bool send_partial_packet(rem_port*, PACKET *, ISC_STATUS *);
#endif
#ifdef MULTI_THREAD
static void server_death(rem_port*);
#endif
static void stuff_vax_integer(UCHAR*, SLONG, USHORT);
static ISC_STATUS svcstart(ISC_STATUS*, RDB, P_OP, USHORT, USHORT, USHORT, const SCHAR*);
static ISC_STATUS unsupported(ISC_STATUS*);
static void zap_packet(PACKET *);

static void mov_faster(const SLONG*, SLONG*, USHORT);

static ULONG remote_event_id = 0;

#define CHECK_HANDLE(blk,type,error) if (!blk || ((BLK) blk)->blk_type != (UCHAR) type) \
				return handle_error (user_status, (ISC_STATUS) error)

#define NULL_CHECK(ptr,code)	if (*ptr) return handle_error (user_status, (ISC_STATUS) code)

#define SET_OBJECT(rdb,object,id) REMOTE_set_object (rdb->rdb_port, (struct blk *) object, id)

#define GDS_ATTACH_DATABASE	REM_attach_database
#define GDS_BLOB_INFO		REM_blob_info
#define GDS_CANCEL_BLOB		REM_cancel_blob
#define GDS_CLOSE_BLOB		REM_close_blob
#define GDS_COMMIT		REM_commit_transaction
#define GDS_COMMIT_RETAINING	REM_commit_retaining
#define GDS_COMPILE		REM_compile_request
#define GDS_CREATE_BLOB2	REM_create_blob2
#define GDS_CREATE_DATABASE	REM_create_database
#define GDS_CANCEL_EVENTS	REM_cancel_events
#define GDS_DATABASE_INFO	REM_database_info
#define GDS_DDL			REM_ddl
#define GDS_DETACH		REM_detach_database
#define GDS_DROP_DATABASE	REM_drop_database
#define GDS_GET_SEGMENT		REM_get_segment
#define GDS_GET_SLICE		REM_get_slice
#define GDS_OPEN_BLOB2		REM_open_blob2
#define GDS_PREPARE		REM_prepare_transaction
#define GDS_PUT_SEGMENT		REM_put_segment
#define GDS_PUT_SLICE		REM_put_slice
#define GDS_QUE_EVENTS		REM_que_events
#define GDS_RECEIVE		REM_receive
#define GDS_RECONNECT		REM_reconnect_transaction
#define GDS_RELEASE_REQUEST	REM_release_request
#define GDS_REQUEST_INFO	REM_request_info
#define GDS_ROLLBACK_RETAINING	REM_rollback_retaining
#define GDS_ROLLBACK		REM_rollback_transaction
#define GDS_SEEK_BLOB		REM_seek_blob
#define GDS_SEND		REM_send
#define GDS_SERVICE_ATTACH	REM_service_attach
#define GDS_SERVICE_DETACH	REM_service_detach
#define GDS_SERVICE_QUERY	REM_service_query
#define GDS_SERVICE_START	REM_service_start
#define GDS_START_AND_SEND	REM_start_and_send
#define GDS_START		REM_start_request
#define GDS_START_TRANSACTION	REM_start_transaction
#define GDS_TRANSACT_REQUEST	REM_transact_request
#define GDS_TRANSACTION_INFO	REM_transaction_info
#define GDS_UNWIND		REM_unwind_request

/* DSQL definitions */

#define GDS_DSQL_ALLOCATE	REM_allocate_statement
#define GDS_DSQL_EXECUTE	REM_execute
#define GDS_DSQL_EXECUTE2	REM_execute2
#define GDS_DSQL_EXECUTE_IMMED	REM_execute_immediate
#define GDS_DSQL_EXECUTE_IMMED2	REM_execute_immediate2
#define GDS_DSQL_FETCH		REM_fetch
#define GDS_DSQL_FREE		REM_free_statement
#define GDS_DSQL_INSERT		REM_insert
#define GDS_DSQL_PREPARE	REM_prepare
#define GDS_DSQL_SET_CURSOR	REM_set_cursor_name
#define GDS_DSQL_SQL_INFO	REM_sql_info


ISC_STATUS GDS_ATTACH_DATABASE(ISC_STATUS*	user_status,
						   SSHORT	file_length,
						   const SCHAR*	file_name,
						   RDB*		handle,
						   SSHORT	dpb_length,
						   const SCHAR*	dpb,
						   const UCHAR*	expanded_filename)
{
/**************************************
 *
 *	g d s _ a t t a c h _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Connect to an old, grungy database, corrupted by user data.
 *
 **************************************/
	trdb	thd_context(user_status);
	trdb*	tdrdb;

	ISC_STATUS* v = user_status;

	*v++ = isc_arg_gds;
	*v++ = isc_unavailable;
	*v = isc_arg_end;

#ifdef UNIX
	// If single user, return
	if (get_single_user(dpb_length, dpb))
	{
		return isc_unavailable;
	}
#endif

	REM_set_thread_data(tdrdb, &thd_context);

	NULL_CHECK(handle, isc_bad_db_handle);

	UCHAR	new_dpb[MAXPATHLEN];
	UCHAR* new_dpb_ptr = new_dpb;
	rem_port* port = 0; // MAX_OTHER_PARAMS uses port's port_dummy_packet_interval

	if ((dpb_length + MAX_USER_LENGTH + MAX_PASSWORD_ENC_LENGTH +
		 MAX_OTHER_PARAMS) > sizeof(new_dpb))
	{
		new_dpb_ptr =
			(UCHAR*)gds__alloc(dpb_length + MAX_USER_LENGTH +
							   MAX_PASSWORD_ENC_LENGTH + MAX_OTHER_PARAMS);

		/* FREE: by return(s) from this procedure */

		if (!new_dpb_ptr)
		{		/* NOMEM: return error to client */
			user_status[1] = isc_virmemexh;
			return error(user_status);
		}
	}

	TEXT user_string[256];
	USHORT new_dpb_length;
	const bool user_verification = get_new_dpb(reinterpret_cast<const UCHAR*>(dpb),
					dpb_length, true, new_dpb_ptr,
					&new_dpb_length, user_string);

	const TEXT* us = (user_string[0]) ? user_string : 0;

	Firebird::PathName expanded_name(expanded_filename);
	Firebird::PathName node_name;
	port = analyze(expanded_name, user_status, us,
					user_verification, dpb, dpb_length, node_name);
	if (!port)
	{
		if (new_dpb_ptr != new_dpb) 
		{
			gds__free(new_dpb_ptr);
		}
		return error(user_status);
	}

	RDB rdb = port->port_context;

	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;
	try
	{
		/* The client may have set a parameter for dummy_packet_interval.  Add that to the
		   the DPB so the server can pay attention to it.  Note: allocation code must
		   ensure sufficient space has been added. */

		add_other_params(port, new_dpb_ptr, &new_dpb_length);
		add_working_directory(new_dpb_ptr, &new_dpb_length, node_name);

		const bool result = init(user_status, port, op_attach, expanded_name,
						new_dpb_ptr, new_dpb_length);

		if (new_dpb_ptr != new_dpb) {
			gds__free(new_dpb_ptr);
		}
		if (!result) {
			return error(user_status);
		}

		*handle = rdb;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_BLOB_INFO(ISC_STATUS*	user_status,
					 RBL*		blob_handle,
					 SSHORT		item_length,
					 const SCHAR*		items,
					 SSHORT		buffer_length,
					 SCHAR*		buffer)
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
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	RBL blob = *blob_handle;
	CHECK_HANDLE(blob, type_rbl, isc_bad_segstr_handle);
	RDB rdb = blob->rbl_rdb;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);

	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	ISC_STATUS status;
	try
	{
		status = info(user_status, rdb, op_info_blob, blob->rbl_id, 0,
					  item_length, items, 0, 0, buffer_length, buffer);
		REM_restore_thread_data();
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return status;
}


ISC_STATUS GDS_CANCEL_BLOB(ISC_STATUS * user_status, RBL * blob_handle)
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
	trdb thd_context(user_status);
	trdb* tdrdb;

	RBL blob = *blob_handle;
	if (!blob) {
		if (user_status) {
			*user_status++ = isc_arg_gds;
			*user_status++ = FB_SUCCESS;
			*user_status = isc_arg_end;
		}
		return FB_SUCCESS;
	}

	REM_set_thread_data(tdrdb, &thd_context);

	CHECK_HANDLE(blob, type_rbl, isc_bad_segstr_handle);
	RDB rdb = blob->rbl_rdb;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);

	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;
	try
	{
		if (!release_object(rdb, op_cancel_blob, blob->rbl_id)) {
			return error(user_status);
		}

		release_blob(blob);
		*blob_handle = NULL;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_CANCEL_EVENTS(ISC_STATUS * user_status, RDB * handle, SLONG * id)
{
/**************************************
 *
 *	g d s _ $ c a n c e l _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *	Cancel an outstanding event.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	RDB rdb = *handle;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);

	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;
	rem_port* port = rdb->rdb_port;

	try
	{
		/* Make sure protocol supports action */

		if (port->port_protocol < PROTOCOL_VERSION6) {
			return unsupported(user_status);
		}

		/* If the event exists, tell the remote server to cancel it,
	   		and delete it from the list */

		RVNT event = find_event(port, *id);
		if (event) {
			send_cancel_event(event);
		}
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_CLOSE_BLOB(ISC_STATUS * user_status, RBL * blob_handle)
{
/**************************************
 *
 *	g d s _ c l o s e _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Close a completed blob.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	RBL blob = *blob_handle;
	CHECK_HANDLE(blob, type_rbl, isc_bad_segstr_handle);
	RDB rdb = blob->rbl_rdb;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;
	rem_port* port = rdb->rdb_port;

	try
	{
		if (!(port->port_flags & PORT_rpc) &&
			(blob->rbl_flags & RBL_create) && blob->rbl_ptr != blob->rbl_buffer)
		{
			if (send_blob(user_status, blob, 0, NULL)) {
				return error(user_status);
			}
		}

		if (!release_object(rdb, op_close_blob, blob->rbl_id)) {
			return error(user_status);
		}

		release_blob(blob);
		*blob_handle = NULL;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_COMMIT(ISC_STATUS * user_status, RTR * rtr_handle)
{
/**************************************
 *
 *	g d s _ c o m m i t
 *
 **************************************
 *
 * Functional description
 *	Commit a transaction.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	RTR transaction = *rtr_handle;
	CHECK_HANDLE(transaction, type_rtr, isc_bad_trans_handle);
	RDB rdb = (*rtr_handle)->rtr_rdb;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);

	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		if (!release_object(rdb, op_commit, transaction->rtr_id)) {
			return error(user_status);
		}

		REMOTE_cleanup_transaction(transaction);
		release_transaction(transaction);
		*rtr_handle = NULL;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}
	
	return return_success(rdb);
}


ISC_STATUS GDS_COMMIT_RETAINING(ISC_STATUS * user_status, RTR * rtr_handle)
{
/**************************************
 *
 *	g d s _ c o m m i t _ r e t a i n i n g
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	RTR transaction = *rtr_handle;
	CHECK_HANDLE(transaction, type_rtr, isc_bad_trans_handle);
	RDB rdb = (*rtr_handle)->rtr_rdb;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);

	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		/* Make sure protocol support action */

		if (rdb->rdb_port->port_protocol < PROTOCOL_VERSION4) {
			return unsupported(user_status);
		}

		if (!release_object(rdb, op_commit_retaining, transaction->rtr_id)) {
			return error(user_status);
		}
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_COMPILE(ISC_STATUS* user_status,
				   RDB* db_handle,
				   rrq** req_handle, USHORT blr_length, const UCHAR* blr)
{
/**************************************
 *
 *	g d s _ c o m p i l e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

/* Check and validate handles, etc. */

	NULL_CHECK(req_handle, isc_bad_req_handle);
	RDB rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);

	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		/* Parse the request in case blr_d_float must be converted to blr_double */

		const UCHAR* new_blr;
		if (rdb->rdb_port->port_protocol < PROTOCOL_VERSION5) {
			new_blr = PARSE_prepare_messages(blr, blr_length);
		}
		else {
			new_blr = blr;
		}

		/* Make up a packet for the remote guy */

		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = op_compile;
		P_CMPL* compile = &packet->p_cmpl;
		compile->p_cmpl_database = rdb->rdb_id;
		compile->p_cmpl_blr.cstr_length = blr_length;
		compile->p_cmpl_blr.cstr_address = const_cast<UCHAR*>(new_blr); // safe cast, I hope

		send_and_receive(rdb, packet, user_status);
		if (new_blr != blr) {
			ALLR_free((void*) new_blr);
		}
		if (user_status[1]) {
			return error(user_status);
		}

		/* Parse the request to find the messages */

		REM_MSG next;

		REM_MSG message = PARSE_messages(blr, blr_length);
		USHORT max_msg = 0;
		for (next = message; next; next = next->msg_next) {
			max_msg = MAX(max_msg, next->msg_number);
		}

		/* Allocate request block */
		rrq* request = (rrq*) ALLR_block(type_rrq, max_msg + 1);
		*req_handle = request;
		request->rrq_rdb = rdb;
		request->rrq_id = packet->p_resp.p_resp_object;
		request->rrq_max_msg = max_msg;
		SET_OBJECT(rdb, request, request->rrq_id);
		request->rrq_next = rdb->rdb_requests;
		rdb->rdb_requests = request;

		/* when the messages are parsed, they are linked together; we need 
		   to place the messages in the tail of the request block and create 
		   a queue of length 1 for each message number */

		for (; message; message = next)
		{
			next = message->msg_next;

			message->msg_next = message;
#ifdef SCROLLABLE_CURSORS
			message->msg_prior = message;
#endif

			rrq::rrq_repeat * tail = request->rrq_rpt + message->msg_number;
			tail->rrq_message = message;
			tail->rrq_xdr = message;
#ifdef SCROLLABLE_CURSORS
			tail->rrq_last = NULL;
#endif
			tail->rrq_format = (rem_fmt*) message->msg_address;

			message->msg_address = NULL;
		}
	}
	catch (const std::exception& ex)
	{
	    // deallocate new_blr here???
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_CREATE_BLOB2(ISC_STATUS* user_status,
						RDB* db_handle,
						RTR* rtr_handle,
						RBL* blob_handle,
						BID blob_id, USHORT bpb_length, const UCHAR* bpb)
{
/**************************************
 *
 *	g d s _ c r e a t e _ b l o b 2
 *
 **************************************
 *
 * Functional description
 *	Open an existing blob.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	NULL_CHECK(blob_handle, isc_bad_segstr_handle);
	RDB rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	CHECK_HANDLE((*rtr_handle), type_rtr, isc_bad_trans_handle);
	RTR transaction = *rtr_handle;

	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = op_create_blob;
		P_BLOB* p_blob = &packet->p_blob;
		p_blob->p_blob_transaction = transaction->rtr_id;

		if (rdb->rdb_port->port_protocol >= PROTOCOL_VERSION4) {
			packet->p_operation = op_create_blob2;
			p_blob->p_blob_bpb.cstr_length = bpb_length;
			fb_assert(!p_blob->p_blob_bpb.cstr_allocated ||
				p_blob->p_blob_bpb.cstr_allocated < p_blob->p_blob_bpb.cstr_length);
			// CVC: Should we ensure here that cstr_allocated < bpb_length???
			// Otherwise, xdr_cstring() calling alloc_string() to decode would
			// cause memory problems on the client side for SS, as the client
			// would try to write to the application's provided R/O buffer.
			p_blob->p_blob_bpb.cstr_address = bpb;
		}

		send_and_receive(rdb, packet, user_status);
		p_blob->p_blob_bpb.cstr_length = 0;
		p_blob->p_blob_bpb.cstr_address = NULL;

		if (user_status[1])
			return error(user_status);

		RBL blob = (RBL) ALLR_block(type_rbl, BLOB_LENGTH);
		*blob_handle = blob;
		*blob_id = packet->p_resp.p_resp_blob_id;
		blob->rbl_buffer_length = BLOB_LENGTH;
		blob->rbl_rdb = rdb;
		blob->rbl_rtr = transaction;
		blob->rbl_id = packet->p_resp.p_resp_object;
		blob->rbl_ptr = blob->rbl_buffer = blob->rbl_data;
		blob->rbl_flags |= RBL_create;
		SET_OBJECT(rdb, blob, blob->rbl_id);
		blob->rbl_next = transaction->rtr_blobs;
		transaction->rtr_blobs = blob;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_CREATE_DATABASE(ISC_STATUS* user_status,
						   SSHORT file_length,
						   const SCHAR* file_name,
						   RDB* handle,
						   SSHORT dpb_length,
						   const SCHAR* dpb,
						   SSHORT db_type,
						   const UCHAR* expanded_filename)
{
/**************************************
 *
 *	g d s _ c r e a t e _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Create a nice, squeeky clean database, uncorrupted by user data.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	ISC_STATUS* v = user_status;
	*v++ = isc_arg_gds;
	*v++ = isc_unavailable;
	*v = isc_arg_end;

#ifdef UNIX
/* If single user, return */

	if (get_single_user(dpb_length, dpb))
		return isc_unavailable;
#endif

	REM_set_thread_data(tdrdb, &thd_context);

	NULL_CHECK(handle, isc_bad_db_handle);

	rem_port* port;
	UCHAR new_dpb[MAXPATHLEN];
	UCHAR* new_dpb_ptr = new_dpb;

	if ((dpb_length + MAX_USER_LENGTH + MAX_PASSWORD_ENC_LENGTH +
		 MAX_OTHER_PARAMS) > sizeof(new_dpb))
	{
		new_dpb_ptr =
			(UCHAR*)gds__alloc(dpb_length + MAX_USER_LENGTH +
							   MAX_PASSWORD_ENC_LENGTH + MAX_OTHER_PARAMS);

		/* FREE: by return(s) in this routine */

		if (!new_dpb_ptr)
		{		/* NOMEM: return error to client */
			user_status[1] = isc_virmemexh;
			return error(user_status);
		}
	}

	USHORT new_dpb_length;
	TEXT user_string[256];
	const bool user_verification =
		get_new_dpb(reinterpret_cast<const UCHAR*>(dpb),
					dpb_length, true, new_dpb_ptr,
					&new_dpb_length, user_string);

	const TEXT* us = (user_string[0]) ? user_string : 0;

	Firebird::PathName expanded_name(expanded_filename);
	Firebird::PathName node_name;
	port = analyze(expanded_name, user_status, us,
				   user_verification, dpb, dpb_length, node_name);
	if (!port) {
		if (new_dpb_ptr != new_dpb)
			gds__free(new_dpb_ptr);
		return error(user_status);
	}

	RDB rdb = port->port_context;
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
	/* The client may have set a parameter for dummy_packet_interval.  Add that to the
	   the DPB so the server can pay attention to it.  Note: allocation code must
	   ensure sufficient space has been added. */

		add_other_params(port, new_dpb_ptr, &new_dpb_length);
		add_working_directory(new_dpb_ptr, &new_dpb_length, node_name);

		const bool result = 
			init(user_status, port, op_create, expanded_name,
					  new_dpb_ptr, new_dpb_length);
		if (new_dpb_ptr != new_dpb) {
			gds__free(new_dpb_ptr);
		}
		if (!result) {
			return error(user_status);
		}

		*handle = rdb;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_DATABASE_INFO(ISC_STATUS*	user_status,
						 RDB*		handle,
						 SSHORT		item_length,
						 const SCHAR*		items,
						 SSHORT		buffer_length,
						 SCHAR*		buffer)
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
	ISC_STATUS	status;
	UCHAR	temp[1024];
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	RDB rdb = *handle;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);

	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		UCHAR* temp_buffer = temp;
		if (buffer_length > (SLONG) sizeof(temp)) {
			temp_buffer = ALLR_alloc((SLONG) buffer_length);
		}
		/* NOMEM: ALLR_alloc handled */
		/* FREE:  Normal case later in this procedure, what about error to ERROR_INIT? */

		status = info(user_status, rdb, op_info_database, rdb->rdb_id, 0,
					  item_length, items, 0, 0, buffer_length,
					  (SCHAR *) temp_buffer);

		if (!status)
		{
			rem_port* port = rdb->rdb_port;

			/* two bytes too much allocated, better safe than sorry */
			const size_t nLen = strlen(GDS_VERSION) +
								strlen(port->port_version->str_data) + 4;
			char* version = (char*)ALLR_alloc(nLen);

			sprintf(version, "%s/%s", GDS_VERSION, port->port_version->str_data);

			MERGE_database_info(temp_buffer, (UCHAR *) buffer, buffer_length,
								IMPLEMENTATION, 3, 1, (UCHAR*)version,
								(UCHAR *) port->port_host->str_data, 0);
			ALLR_free(version);
		}

		if (temp_buffer != temp) {
			ALLR_free(temp_buffer);
		}

		REM_restore_thread_data();
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return status;
}


ISC_STATUS GDS_DDL(ISC_STATUS*	user_status,
			   RDB*		db_handle,
			   RTR*		rtr_handle,
			   USHORT	blr_length,
			   const UCHAR*	blr)
{
/**************************************
 *
 *	g d s _ d d l
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	ISC_STATUS status;
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

/* Check and validate handles, etc. */

	RDB rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	CHECK_HANDLE((*rtr_handle), type_rtr, isc_bad_trans_handle);
	RTR transaction = *rtr_handle;

	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		if (rdb->rdb_port->port_protocol < PROTOCOL_VERSION4) {
			return unsupported(user_status);
		}

		/* Make up a packet for the remote guy */

		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = op_ddl;
		P_DDL* ddl = &packet->p_ddl;
		ddl->p_ddl_database = rdb->rdb_id;
		ddl->p_ddl_transaction = transaction->rtr_id;
		ddl->p_ddl_blr.cstr_length = blr_length;
		ddl->p_ddl_blr.cstr_address = blr;

		status = send_and_receive(rdb, packet, user_status);
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	REM_restore_thread_data();

	return status;
}


ISC_STATUS GDS_DETACH(ISC_STATUS* user_status, RDB* handle)
{
/**************************************
 *
 *	g d s _ d e t a c h
 *
 **************************************
 *
 * Functional description
 *	Close down a database.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	RDB rdb = *handle;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);

	rem_port* port = rdb->rdb_port;
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		release_object(rdb, op_detach, rdb->rdb_id);

		/* If something other than a network error occurred, just return.  Otherwise
		   we need to free up the associated structures, close the socket and
		   scram.  By the way, we should probably create an entry in the log
		   telling the user that an unrecoverable network error occurred and that
		   if there was any uncommitted work, its gone......  Oh well.... */

		if (user_status[1] && user_status[1] != isc_network_error)
		{
			return (error(user_status));
		}

		while (rdb->rdb_events)
			release_event(rdb->rdb_events);

		while (rdb->rdb_requests)
			release_request(rdb->rdb_requests);

		while (rdb->rdb_sql_requests)
			release_sql_request(rdb->rdb_sql_requests);

		while (rdb->rdb_transactions)
			release_transaction(rdb->rdb_transactions);

		if (port->port_statement)
			release_statement(&port->port_statement);

	/* If there is a network error, don't try to send another packet, just
	   free the packet and disconnect the port. Put something into firebird.log
	   informing the user of the following. */

		if (user_status[1])
		{
			gds__log("REMOTE INTERFACE/gds__detach: Unsuccesful detach from "
					"database. \n\tUncommitted work may have been lost", 0);
		}

		disconnect(port);
		*handle = NULL;

		/* Can't return_success(rdb) here as we've already torn down memory */
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	REM_restore_thread_data();

	*user_status++ = isc_arg_gds;
	*user_status++ = FB_SUCCESS;
	*user_status = isc_arg_end;

	return FB_SUCCESS;
}


ISC_STATUS GDS_DROP_DATABASE(ISC_STATUS* user_status, RDB* handle)
{
/**************************************
 *
 *	i s c _ d r o p _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Close down and purge a database.
 *
 **************************************/
	ISC_STATUS_ARRAY local_status;
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	RDB rdb = *handle;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);

	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;
	rem_port* port = rdb->rdb_port;

	try
	{
		/* Make sure protocol supports the action */

		if (port->port_protocol < PROTOCOL_VERSION8)
			return unsupported(user_status);

		if (!release_object(rdb, op_drop_database, rdb->rdb_id))
			if (user_status[1] != isc_drdb_completed_with_errs)
				return error(user_status);

		while (rdb->rdb_events)
			release_event(rdb->rdb_events);

		while (rdb->rdb_requests)
			release_request(rdb->rdb_requests);

		while (rdb->rdb_sql_requests)
			release_sql_request(rdb->rdb_sql_requests);

		while (rdb->rdb_transactions)
			release_transaction(rdb->rdb_transactions);

		if (port->port_statement)
			release_statement(&port->port_statement);

		rdb->rdb_status_vector = local_status;

		disconnect(port);
		*handle = NULL;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	REM_restore_thread_data();

	return user_status[1];
}


ISC_STATUS GDS_DSQL_ALLOCATE(ISC_STATUS*	user_status,
						 RDB*		db_handle,
						 RSR*		stmt_handle)
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
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	NULL_CHECK(stmt_handle, isc_bad_req_handle);
	RDB rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);

	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		/* Make sure protocol support action */

		if (rdb->rdb_port->port_protocol < PROTOCOL_VERSION7)
			return unsupported(user_status);

		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = op_allocate_statement;
		packet->p_rlse.p_rlse_object = rdb->rdb_id;

		if (send_and_receive(rdb, packet, user_status))
			return error(user_status);

		/* Allocate SQL request block */

		RSR statement = (RSR) ALLR_block(type_rsr, 0);
		*stmt_handle = statement;
		statement->rsr_rdb = rdb;
		statement->rsr_id = packet->p_resp.p_resp_object;
		statement->rsr_next = rdb->rdb_sql_requests;

		rdb->rdb_sql_requests = statement;

		/* register the object */

		SET_OBJECT(rdb, statement, statement->rsr_id);
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_DSQL_EXECUTE(ISC_STATUS*	user_status,
						RTR*	rtr_handle,
						RSR*	stmt_handle,
						USHORT	blr_length,
						UCHAR*	blr,
						USHORT	msg_type,
						USHORT	msg_length,
						UCHAR*	msg)
{
/**************************************
 *
 *	d s q l _ e x e c u t e
 *
 **************************************
 *
 * Functional description
 *	Execute a non-SELECT dynamic SQL statement.
 *
 **************************************/

	return GDS_DSQL_EXECUTE2(user_status, rtr_handle, stmt_handle,
							 blr_length, blr, msg_type, msg_length, msg, 0,
							 NULL, 0, 0, NULL);
}


ISC_STATUS GDS_DSQL_EXECUTE2(ISC_STATUS*	user_status,
						 RTR*		rtr_handle,
						 RSR*		stmt_handle,
						 USHORT		in_blr_length,
						 UCHAR*		in_blr,
						 USHORT		in_msg_type,
						 USHORT		in_msg_length,
						 UCHAR*		in_msg,
						 USHORT		out_blr_length,
						 UCHAR*		out_blr,
						 USHORT		out_msg_type,
						 USHORT		out_msg_length,
						 UCHAR*		out_msg)
{
/**************************************
 *
 *	d s q l _ e x e c u t e 2
 *
 **************************************
 *
 * Functional description
 *	Execute a non-SELECT dynamic SQL statement.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

/* Check and validate handles, etc. */

	RSR statement = *stmt_handle;
	CHECK_HANDLE(statement, type_rsr, isc_bad_req_handle);
	RDB rdb = statement->rsr_rdb;
	RTR transaction = *rtr_handle;
	if (transaction) {
		CHECK_HANDLE(transaction, type_rtr, isc_bad_trans_handle);
	}

	rem_port* port = rdb->rdb_port;
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		/* bag it if the protocol doesn't support it... */

		if (port->port_protocol < PROTOCOL_VERSION7 ||
			(out_msg_length && port->port_protocol < PROTOCOL_VERSION8))
		{
			return unsupported(user_status);
		}

		// 24-Mar-2004 Nickolay Samofatov
		// Unconditionally deallocate existing formats that are left from 
		// previous executions (possibly with different statement if 
		// isc_dsql_prepare is called multiple times). 
		// This should cure SF#919246
		if (statement->rsr_bind_format) {
			ALLR_release(statement->rsr_bind_format);
			statement->rsr_bind_format = NULL;
		}
		if (port->port_statement && port->port_statement->rsr_select_format) {
			ALLR_release(port->port_statement->rsr_select_format);
			port->port_statement->rsr_select_format = NULL;
		}

		/* Parse the blr describing the message, if there is any. */

		if (in_blr_length) {
			REM_MSG message = PARSE_messages(in_blr, in_blr_length);
			if (message != (REM_MSG) - 1) {
				statement->rsr_bind_format = (rem_fmt*) message->msg_address;
				ALLR_release(message);
			}
		}

		/* Parse the blr describing the output message.  This is not the fetch
		   message!  That comes later. */

		if (out_blr_length) {
			if (!port->port_statement)
				port->port_statement = (RSR) ALLR_block(type_rsr, 0);

			REM_MSG message = PARSE_messages(out_blr, out_blr_length);
			if (message != (REM_MSG) - 1) {
				port->port_statement->rsr_select_format =
					(rem_fmt*) message->msg_address;
				ALLR_release(message);
			}

			if (!port->port_statement->rsr_buffer) {
				REM_MSG message2 = (REM_MSG) ALLR_block(type_msg, 0);
				port->port_statement->rsr_buffer = message2;
				port->port_statement->rsr_message = message2;
				message2->msg_next = message2;
#ifdef SCROLLABLE_CURSORS
				message2->msg_prior = message2;
#endif
				port->port_statement->rsr_fmt_length = 0;
			}
		}

		REM_MSG message = 0;
		if (!statement->rsr_buffer) {
			statement->rsr_buffer = message = (REM_MSG) ALLR_block(type_msg, 0);
			statement->rsr_message = message;

			message->msg_next = message;
#ifdef SCROLLABLE_CURSORS
			message->msg_prior = message;
#endif

			statement->rsr_fmt_length = 0;
		}
		else {
			message = statement->rsr_message = statement->rsr_buffer;
		}

		message->msg_address = in_msg;
		statement->rsr_flags &= ~RSR_fetched;
		statement->rsr_format = statement->rsr_bind_format;

		/* set up the packet for the other guy... */

		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = (out_msg_length) ? op_execute2 : op_execute;
		P_SQLDATA* sqldata = &packet->p_sqldata;
		sqldata->p_sqldata_statement = statement->rsr_id;
		sqldata->p_sqldata_transaction = (transaction) ? transaction->rtr_id : 0;
		sqldata->p_sqldata_blr.cstr_length = in_blr_length;
		sqldata->p_sqldata_blr.cstr_address = in_blr;
		sqldata->p_sqldata_message_number = in_msg_type;
		sqldata->p_sqldata_messages = (statement->rsr_bind_format) ? 1 : 0;
		sqldata->p_sqldata_out_blr.cstr_length = out_blr_length;
		sqldata->p_sqldata_out_blr.cstr_address = out_blr;
		sqldata->p_sqldata_out_message_number = out_msg_type;

		if (!send_packet(port, packet, user_status))
			return error(user_status);

		/* Set up the response packet.  We may receive an SQL response followed
		   by a normal response packet or simply a response packet. */

		message->msg_address = NULL;
		if (out_msg_length)
			port->port_statement->rsr_message->msg_address = out_msg;

		packet->p_resp.p_resp_status_vector = rdb->rdb_status_vector;

		if (!receive_packet(port, packet, user_status))
			return error(user_status);

		if (packet->p_operation != op_sql_response)
			check_response(rdb, packet);
		else {
			port->port_statement->rsr_message->msg_address = NULL;
			receive_response(rdb, packet);
		}

		if (user_status[1])
			return error(user_status);

		if (transaction && !packet->p_resp.p_resp_object) {
			REMOTE_cleanup_transaction(transaction);
			release_transaction(transaction);
			*rtr_handle = NULL;
		}
		else if (!transaction && packet->p_resp.p_resp_object)
			*rtr_handle = make_transaction(rdb, packet->p_resp.p_resp_object);

		statement->rsr_rtr = *rtr_handle;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_DSQL_EXECUTE_IMMED(ISC_STATUS* user_status,
							  RDB* db_handle,
							  RTR* rtr_handle,
							  USHORT length,
							  const TEXT* string,
							  USHORT dialect,
							  USHORT blr_length,
							  const UCHAR* blr,
							  USHORT msg_type, USHORT msg_length, UCHAR* msg)
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

	return GDS_DSQL_EXECUTE_IMMED2(user_status, db_handle, rtr_handle,
								   length, string, dialect,
								   blr_length, blr, msg_type, msg_length, msg,
								   0, NULL, 0, 0, NULL);
}


ISC_STATUS GDS_DSQL_EXECUTE_IMMED2(ISC_STATUS* user_status,
							   RDB* db_handle,
							   RTR* rtr_handle,
							   USHORT length,
							   const TEXT* string,
							   USHORT dialect,
							   USHORT in_blr_length,
							   const UCHAR* in_blr,
							   USHORT in_msg_type,
							   USHORT in_msg_length,
							   UCHAR* in_msg,
							   USHORT out_blr_length,
							   UCHAR* out_blr,
							   USHORT out_msg_type,
							   USHORT out_msg_length, UCHAR* out_msg)
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
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

/* Check and validate handles, etc. */

	RDB rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	RTR transaction = *rtr_handle;
	if (transaction) {
		CHECK_HANDLE(transaction, type_rtr, isc_bad_trans_handle);
	}

	rem_port* port = rdb->rdb_port;
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	if (dialect > 10)
	{
		// dimitr: adjust dialect received after
		//		   a multi-hop transmission to be
		//		   redirected in its original value.
		dialect /= 10;
	}

	try
	{
		/* bag it if the protocol doesn't support it... */

		if (port->port_protocol < PROTOCOL_VERSION7 ||
			((in_msg_length || out_msg_length)
		 	&& port->port_protocol < PROTOCOL_VERSION8))
		{
		 	return unsupported(user_status);
		}

		/* If the server is pre-6.0, do not send anything if the client dialect is 3 and
		   there is a SQLDA.  This will cause the older server to crash */
		if (port->port_protocol < PROTOCOL_VERSION10 &&
			(in_msg_length || out_msg_length) && dialect > SQL_DIALECT_V5)
		{
			return unsupported(user_status);
		}

		RSR statement = port->port_statement;
		if (!statement) {
			statement = port->port_statement = (RSR) ALLR_block(type_rsr, 0);
		}

		/* reset statement buffers */

		if (!clear_queue(rdb->rdb_port, user_status))
			return error(user_status);

		REMOTE_reset_statement(statement);

		if (statement->rsr_bind_format) {
			ALLR_release(statement->rsr_bind_format);
			statement->rsr_bind_format = NULL;
		}

		if (statement->rsr_select_format) {
			ALLR_release(statement->rsr_select_format);
			statement->rsr_select_format = NULL;
		}

		if (in_msg_length || out_msg_length)
		{
			if (in_blr_length)
			{
				REM_MSG message = PARSE_messages(in_blr, in_blr_length);
				if ((message) != (REM_MSG) - 1) {
					statement->rsr_bind_format = (rem_fmt*) message->msg_address;
					ALLR_release(message);
				}
			}
			if (out_blr_length)
			{
				REM_MSG message = PARSE_messages(out_blr, out_blr_length);
				if ((message) != (REM_MSG) - 1) {
					statement->rsr_select_format = (rem_fmt*) message->msg_address;
					ALLR_release(message);
				}
			}
		}

		REM_MSG message = 0;
		if (!statement->rsr_buffer)
		{
			statement->rsr_buffer = message = (REM_MSG) ALLR_block(type_msg, 0);
			statement->rsr_message = message;
			message->msg_next = message;
#ifdef SCROLLABLE_CURSORS
			message->msg_prior = message;
#endif
			statement->rsr_fmt_length = 0;
		}
		else {
			message = statement->rsr_message = statement->rsr_buffer;
		}

		message->msg_address = in_msg;

		/* set up the packet for the other guy... */

		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = (in_msg_length || out_msg_length) ?
			op_exec_immediate2 : op_exec_immediate;
		P_SQLST* ex_now = &packet->p_sqlst;
		ex_now->p_sqlst_transaction = (transaction) ? transaction->rtr_id : 0;
		ex_now->p_sqlst_SQL_dialect = dialect;
		ex_now->p_sqlst_SQL_str.cstr_length =
			length ? length : strlen(string);
		ex_now->p_sqlst_SQL_str.cstr_address = (UCHAR *) string;
		ex_now->p_sqlst_items.cstr_length = 0;
		ex_now->p_sqlst_buffer_length = 0;
		ex_now->p_sqlst_blr.cstr_length = in_blr_length;
		ex_now->p_sqlst_blr.cstr_address = const_cast<UCHAR*>(in_blr);
		ex_now->p_sqlst_message_number = in_msg_type;
		ex_now->p_sqlst_messages = (in_msg_length
									&& statement->rsr_bind_format) ? 1 : 0;
		ex_now->p_sqlst_out_blr.cstr_length = out_blr_length;
		ex_now->p_sqlst_out_blr.cstr_address = out_blr;
		ex_now->p_sqlst_out_message_number = out_msg_type;

		if (!send_packet(port, packet, user_status)) {
			return error(user_status);
		}

		/* SEND could have changed the message */

		message = statement->rsr_message;

		/* Set up the response packet.  We may receive an SQL response followed
		   by a normal response packet or simply a response packet. */

		if (in_msg_length || out_msg_length)
			port->port_statement->rsr_message->msg_address = out_msg;

		packet->p_resp.p_resp_status_vector = rdb->rdb_status_vector;
		if (!receive_packet(rdb->rdb_port, packet, user_status))
			return error(user_status);

		if (packet->p_operation != op_sql_response)
			check_response(rdb, packet);
		else {
			message->msg_address = NULL;
			receive_response(rdb, packet);
		}

		if (user_status[1])
			return error(user_status);

		if (transaction && !packet->p_resp.p_resp_object) {
			REMOTE_cleanup_transaction(transaction);
			release_transaction(transaction);
			*rtr_handle = NULL;
		}
		else if (!transaction && packet->p_resp.p_resp_object)
			*rtr_handle = make_transaction(rdb, packet->p_resp.p_resp_object);
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_DSQL_FETCH(ISC_STATUS* user_status,
					  RSR* stmt_handle,
					  USHORT blr_length,
					  const UCHAR* blr,
					  USHORT msg_type, USHORT msg_length, UCHAR* msg)
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
	ISC_STATUS status;
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

/* Check and validate handles, etc. */

	RSR statement = *stmt_handle;
	CHECK_HANDLE(statement, type_rsr, isc_bad_req_handle);
	RDB rdb = statement->rsr_rdb;
	rem_port* port = rdb->rdb_port;
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		/* make sure the protocol supports it */

		if (rdb->rdb_port->port_protocol < PROTOCOL_VERSION7) {
			return unsupported(user_status);
		}

		/* On first fetch, clear the end-of-stream flag & reset the message buffers */

		if (!(statement->rsr_flags & RSR_fetched))
		{
			statement->rsr_flags &= ~(RSR_eof | RSR_stream_err);
			statement->rsr_rows_pending = 0;
			memset(statement->rsr_status_vector, 0,
				   sizeof(statement->rsr_status_vector));
			REM_MSG message = statement->rsr_message;
			if (message)
			{
				statement->rsr_buffer = message;
				while (true)
				{
					message->msg_address = NULL;
					message = message->msg_next;
					if (message == statement->rsr_message) {
						break;
					}
				}
			}
		}

		/* Parse the blr describing the message, if there is any. */

		if (blr_length) {
			if (statement->rsr_user_select_format &&
				statement->rsr_user_select_format != statement->rsr_select_format)
				ALLR_release(statement->rsr_user_select_format);
			REM_MSG message = PARSE_messages(blr, blr_length);
			if (message != (REM_MSG) - 1) {
				statement->rsr_user_select_format = (rem_fmt*) message->msg_address;
				ALLR_release(message);
			}
			else
				statement->rsr_user_select_format = NULL;
			if (statement->rsr_flags & RSR_fetched)
				blr_length = 0;
			else {
				if (statement->rsr_select_format)
					ALLR_release(statement->rsr_select_format);
				statement->rsr_select_format = statement->rsr_user_select_format;
			}
		}

		if (statement->rsr_flags & RSR_blob) {
			status = fetch_blob(user_status, statement, blr_length, blr,
								msg_type, msg_length, msg);
			REM_restore_thread_data();
			return status;
		}


		if (!statement->rsr_buffer) {
			statement->rsr_buffer = (REM_MSG) ALLR_block(type_msg, 0);
			statement->rsr_message = statement->rsr_buffer;
			statement->rsr_message->msg_next = statement->rsr_message;
#ifdef SCROLLABLE_CURSORS
			statement->rsr_message->msg_prior = statement->rsr_message;
#endif
			statement->rsr_fmt_length = 0;
		}

		REM_MSG message = statement->rsr_message;

#ifdef DEBUG
		fprintf(stdout, "Rows Pending in REM_fetch=%lu\n",
				   statement->rsr_rows_pending);
#endif

		/* Check to see if data is waiting.  If not, solicite data. */

		if ((!(statement->rsr_flags & (RSR_eof | RSR_stream_err)) &&
			 (!statement->rsr_message->msg_address) &&
			 (statement->rsr_rows_pending == 0))
			|| (					/* Low in inventory */
				   (statement->rsr_rows_pending <= statement->rsr_reorder_level) &&
				   (statement->rsr_msgs_waiting <= statement->rsr_reorder_level)
				   &&
				   /* doing Batch, not RPC */
				   !(port->port_flags & PORT_rpc) &&
				   /* not using named pipe on NT */
				   /* Pipelining causes both server & client to 
				      write at the same time. In named pipes, writes
				      block for the other end to read -  and so when both
				      attempt to write simultaenously, they end up
				      waiting indefinetly for the other end to read */
				   (port->port_type != port_pipe) &&
#ifdef XNET
				   (port->port_type != port_xnet) &&
#endif
				   /* We've reached eof or there was an error */
				   !(statement->rsr_flags & (RSR_eof | RSR_stream_err)) &&
				   /* No error pending */
				   (!statement->rsr_status_vector[1])))
		{
			/* set up the packet for the other guy... */

			PACKET* packet = &rdb->rdb_packet;
			packet->p_operation = op_fetch;
			P_SQLDATA* sqldata = &packet->p_sqldata;
			sqldata->p_sqldata_statement = statement->rsr_id;
			sqldata->p_sqldata_blr.cstr_length = blr_length;
			sqldata->p_sqldata_blr.cstr_address = const_cast<UCHAR*>(blr);
			sqldata->p_sqldata_message_number = msg_type;
			if (sqldata->p_sqldata_messages =
				(statement->rsr_select_format) ? 1 : 0)
			{
				if (!(port->port_flags &PORT_rpc))
				{
				sqldata->p_sqldata_messages =
					static_cast<USHORT>(REMOTE_compute_batch_size(port,
						0, op_fetch_response, statement->rsr_select_format));
				sqldata->p_sqldata_messages *= 4;

				/** Reorder data when the local buffer is half empty **/

					statement->rsr_reorder_level =
						sqldata->p_sqldata_messages / 2;
#ifdef DEBUG
				fprintf(stdout,
						   "Recalculating Rows Pending in REM_fetch=%lu\n",
						   statement->rsr_rows_pending);
#endif
				}
			}
			statement->rsr_rows_pending += sqldata->p_sqldata_messages;

			/* Make the batch request - and force the packet over the wire */

			if (!send_packet(rdb->rdb_port, packet, user_status)) {
				return error(user_status);
			}

			statement->rsr_batch_count++;

			/* Queue up receipt of the pending data */

			enqueue_receive(port, batch_dsql_fetch, rdb, statement, NULL);

			fb_assert(statement->rsr_rows_pending > 0
				   || (!statement->rsr_select_format));
		}

		/* Receive queued responses until we have some data for this cursor
		   or an error status has been received. */

		/* We've either got data, or some is on the way, or we have an error, or we have EOF */

		fb_assert(statement->rsr_msgs_waiting || (statement->rsr_rows_pending > 0)
			   || statement->rsr_status_vector[1]
			   || statement->rsr_flags & (RSR_eof));

		while (!(statement->rsr_status_vector[1])	/* received a database error */
			   &&!(statement->rsr_flags & (RSR_eof))	/* reached end of cursor */
			   &&!(statement->rsr_msgs_waiting >= 2)	/* Have looked ahead for end of batch */
			   &&!(statement->rsr_rows_pending == 0))
		{	/* Hit end of batch */
			if (!receive_queued_packet(tdrdb, port, user_status,
										statement->rsr_id))
			{
				return error(user_status);
			}
		}

		if (!statement->rsr_msgs_waiting)
		{
			if (statement->rsr_flags & RSR_eof)
			{
				statement->rsr_flags &= ~RSR_eof;

				/* Set up status vector and REM_restore_thread_data in common return_success */

				return_success(rdb);
				return 100;
			}

			if (statement->rsr_flags & RSR_stream_err) {

				/* The previous batch of receives ended with an error status.
				   We're all done returning data in the local queue.
				   Return that error status vector to the user. */

				/* Stuff in the error result to the user's vector */

				statement->rsr_flags &= ~RSR_stream_err;

				memcpy(user_status, statement->rsr_status_vector,
					   sizeof(statement->rsr_status_vector));
				memset(statement->rsr_status_vector, 0,
					   sizeof(statement->rsr_status_vector));

				return error(user_status);
			}
		}
		statement->rsr_msgs_waiting--;

		message = statement->rsr_message;
		statement->rsr_message = message->msg_next;

		if (statement->rsr_user_select_format == statement->rsr_select_format) {
			if ((U_IPTR) msg & (ALIGNMENT - 1))
				memcpy(msg, message->msg_address, msg_length);
			else
				mov_faster((SLONG *) message->msg_address, (SLONG *) msg,
						   msg_length);
		}
		else {
			if (mov_dsql_message
				(message->msg_address, statement->rsr_select_format, msg,
				 statement->rsr_user_select_format))
			{
				return error(user_status);
			}
		}

		message->msg_address = NULL;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_DSQL_FREE(ISC_STATUS * user_status, RSR * stmt_handle, USHORT option)
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
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

/* Check and validate handles, etc. */

	RSR statement = *stmt_handle;
	CHECK_HANDLE(statement, type_rsr, isc_bad_req_handle);
	RDB rdb = statement->rsr_rdb;

	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		/* make sure the protocol supports it */

		if (rdb->rdb_port->port_protocol < PROTOCOL_VERSION7) {
			return unsupported(user_status);
		}

		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = op_free_statement;
		P_SQLFREE* free_stmt = &packet->p_sqlfree;
		free_stmt->p_sqlfree_statement = statement->rsr_id;
		free_stmt->p_sqlfree_option = option;

		if (send_and_receive(rdb, packet, user_status)) {
			return error(user_status);
		}

		statement->rsr_handle = (FB_API_HANDLE) packet->p_resp.p_resp_object;
		if (packet->p_resp.p_resp_object == 0xFFFF) {
			release_sql_request(statement);
			*stmt_handle = NULL;
		}
		else {
			statement->rsr_flags &= ~RSR_fetched;
			statement->rsr_rtr = NULL;

			if (!clear_queue(rdb->rdb_port, user_status))
				return error(user_status);

			REMOTE_reset_statement(statement);
		}
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_DSQL_INSERT(ISC_STATUS * user_status,
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
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	/* Check and validate handles, etc. */

	RSR statement = *stmt_handle;
	CHECK_HANDLE(statement, type_rsr, isc_bad_req_handle);
	RDB rdb = statement->rsr_rdb;
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		/* make sure the protocol supports it */

		if (rdb->rdb_port->port_protocol < PROTOCOL_VERSION8) {
			return unsupported(user_status);
		}

		// Free existing format unconditionally. 
		// This is also related to SF#919246
		if (statement->rsr_bind_format) {
			ALLR_release(statement->rsr_bind_format);
			statement->rsr_bind_format = NULL;
		}

		/* Parse the blr describing the message, if there is any. */

		if (blr_length) {
			REM_MSG message = PARSE_messages(blr, blr_length);
			if (message != (REM_MSG) - 1) {
				statement->rsr_bind_format = (rem_fmt*) message->msg_address;
				ALLR_release(message);
			}
		}

		REM_MSG message = 0;
		if (!statement->rsr_buffer) {
			statement->rsr_buffer = message = (REM_MSG) ALLR_block(type_msg, 0);
			statement->rsr_message = message;
			message->msg_next = message;
#ifdef SCROLLABLE_CURSORS
			message->msg_prior = message;
#endif
			statement->rsr_fmt_length = 0;
		}
		else {
			message = statement->rsr_message;
		}

		message->msg_address = msg;
		statement->rsr_format = statement->rsr_bind_format;

		/* set up the packet for the other guy... */

		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = op_insert;
		P_SQLDATA* sqldata = &packet->p_sqldata;
		sqldata->p_sqldata_statement = statement->rsr_id;
		sqldata->p_sqldata_blr.cstr_length = blr_length;
		sqldata->p_sqldata_blr.cstr_address = blr;
		sqldata->p_sqldata_message_number = msg_type;
		sqldata->p_sqldata_messages = (statement->rsr_bind_format) ? 1 : 0;

		if (!send_packet(rdb->rdb_port, packet, user_status)) {
			return error(user_status);
		}

		message->msg_address = NULL;

		if (!receive_response(rdb, packet)) {
			return error(user_status);
		}
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_DSQL_PREPARE(ISC_STATUS * user_status, RTR * rtr_handle, RSR * stmt_handle,	/* a remote statement block */
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
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

/* Check and validate handles, etc. */

	RSR statement = *stmt_handle;
	CHECK_HANDLE(statement, type_rsr, isc_bad_req_handle);
	RDB rdb = statement->rsr_rdb;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	RTR transaction = *rtr_handle;
	if (transaction) {
		CHECK_HANDLE(transaction, type_rtr, isc_bad_trans_handle);
	}
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	if (dialect > 10)
	{
		// dimitr: adjust dialect received after
		//		   a multi-hop transmission to be
		//		   redirected in its original value.
		dialect /= 10;
	}

	try
	{
		/* reset current statement */

		if (!clear_queue(rdb->rdb_port, user_status))
			return error(user_status);

		REMOTE_reset_statement(statement);

		/* if we're less than protocol 7, the remote server doesn't support 
	 	* DSQL, so we're done... */

		if (rdb->rdb_port->port_protocol < PROTOCOL_VERSION7) {
			return unsupported(user_status);
		}

		/* set up the packet for the other guy... */

		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = op_prepare_statement;
		P_SQLST* prepare = &packet->p_sqlst;
		prepare->p_sqlst_transaction = (transaction) ? transaction->rtr_id : 0;
		prepare->p_sqlst_statement = statement->rsr_id;
		prepare->p_sqlst_SQL_dialect = dialect;
		prepare->p_sqlst_SQL_str.cstr_length =
			length ? length : strlen(string);
		prepare->p_sqlst_SQL_str.cstr_address = (UCHAR *) string;
		prepare->p_sqlst_items.cstr_length = item_length;
		prepare->p_sqlst_items.cstr_address = (UCHAR *) items;
		prepare->p_sqlst_buffer_length = buffer_length;

		if (!send_packet(rdb->rdb_port, packet, user_status))
			return error(user_status);

		statement->rsr_flags &= ~RSR_blob;

		/* Set up for the response packet. */

		P_RESP* response = &packet->p_resp;
		CSTRING temp = response->p_resp_data;
		response->p_resp_data.cstr_allocated = buffer_length;
		response->p_resp_data.cstr_address = (UCHAR *) buffer;

		bool status = receive_response(rdb, packet);

		if (response->p_resp_object)
			statement->rsr_flags |= RSR_blob;
		response->p_resp_data = temp;
		if (!status) {
			return error(user_status);
		}
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_DSQL_SET_CURSOR(ISC_STATUS* user_status,
						   RSR* stmt_handle, const TEXT* cursor, USHORT type)
{
/*****************************************
 *
 *	d s q l _ s e t _ c u r s o r
 *
 *****************************************
 *
 * Functional Description
 *	Declare a cursor for a dynamic request.
 *
 *	Note:  prior to version 6.0, this function terminated the
 *	cursor name at the first blank.  With delimited cursor
 *	name support that is no longer sufficient.  We now pass
 *	the entire NULL-Terminated cursor name to the server, and let
 *	the server deal with blank termination or not.
 *	NOTE:  THIS NOW MEANS THAT IF CURSOR is NOT null terminated
 *	we will have inconsistant results with version 5.x.  The only
 *	"normal" way this happens is if this API is called from a
 *	non-C host language.   If that results in a later problem we
 *	must provide a new API that takes a "cursor_name_length"
 *	parameter.
 *
 *****************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	/* Check and validate handles, etc. */

	RSR statement = *stmt_handle;
	CHECK_HANDLE(statement, type_rsr, isc_bad_req_handle);
	RDB rdb = statement->rsr_rdb;
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		/* make sure the protocol supports it */

		if (rdb->rdb_port->port_protocol < PROTOCOL_VERSION7) {
			return unsupported(user_status);
		}

		/* set up the packet for the other guy... */

		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = op_set_cursor;
		P_SQLCUR* sqlcur = &packet->p_sqlcur;
		sqlcur->p_sqlcur_statement = statement->rsr_id;

		if (!cursor)
		{
			/** Return CURSOR unknown error **/
			user_status[1] = isc_dsql_cursor_err;
			return error(user_status);
		}

		const USHORT name_l = strlen(cursor);
		sqlcur->p_sqlcur_cursor_name.cstr_length = name_l + 1;
		sqlcur->p_sqlcur_cursor_name.cstr_address = (UCHAR *) cursor; // const cast
		sqlcur->p_sqlcur_type = type;

		if (send_and_receive(rdb, packet, user_status)) {
			return error(user_status);
		}
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_DSQL_SQL_INFO(ISC_STATUS* user_status,
						 RSR* stmt_handle,
						 SSHORT item_length,
						 const SCHAR* items,
						 SSHORT buffer_length, SCHAR* buffer)
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
	ISC_STATUS status;
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

/* Check and validate handles, etc. */

	RSR statement = *stmt_handle;
	CHECK_HANDLE(statement, type_rsr, isc_bad_req_handle);
	RDB rdb = statement->rsr_rdb;
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		/* make sure the protocol supports it */

		if (rdb->rdb_port->port_protocol < PROTOCOL_VERSION7) {
			return unsupported(user_status);
		}

		status = info(user_status, rdb, op_info_sql, statement->rsr_id, 0,
					  item_length, items, 0, 0, buffer_length, buffer);
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	REM_restore_thread_data();

	return status;
}


ISC_STATUS GDS_GET_SEGMENT(ISC_STATUS * user_status,
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
 *	Buffer segments of a blob and pass
 *	them one by one to the caller.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

/* Sniff out handles, etc, and find the various blocks. */

	CHECK_HANDLE((*blob_handle), type_rbl, isc_bad_segstr_handle);
	RBL blob = *blob_handle;
	RDB rdb = blob->rbl_rdb;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;
	rem_port* port = rdb->rdb_port;

	try
	{
		/* Build the primary packet to get the operation started. */

		PACKET* packet = &rdb->rdb_packet;
		P_SGMT* segment = &packet->p_sgmt;
		P_RESP* response = &packet->p_resp;
		CSTRING temp = response->p_resp_data;

		/* Handle old protocol.  Also handle new protocol on a blob that has
		   been created rather than opened.   (This should yield an error.) */

		if ((port->port_flags & PORT_rpc) || (blob->rbl_flags & RBL_create))
		{
			packet->p_operation = op_get_segment;
			segment->p_sgmt_length = buffer_length;
			segment->p_sgmt_blob = blob->rbl_id;
			segment->p_sgmt_segment.cstr_length = 0;
			if (!send_packet(rdb->rdb_port, packet, user_status)) {
				return error(user_status);
			}
			response->p_resp_data.cstr_allocated = buffer_length;
			response->p_resp_data.cstr_address = buffer;

			if (!receive_response(rdb, packet)) {
				response->p_resp_data = temp;
				return error(user_status);
			}

			*length = response->p_resp_data.cstr_length;
			response->p_resp_data = temp;
			REM_restore_thread_data();
			return user_status[1];
		}

		/* New protocol -- ask for a 1K chunk of blob and 
		   fill segment requests from it until its time to
		   get the next section.  In other words, get a bunch,
		   pass it out piece by piece, then when there isn't 
		   enough left, ask for more. */

		/* set up the status vector for the calls we're going to fake */

		ISC_STATUS* v = user_status;
		*v++ = isc_arg_gds;
		v[0] = FB_SUCCESS;
		v[1] = isc_arg_end;
		*length = 0;

		/* if we're already done, stop now */

		if (blob->rbl_flags & RBL_eof) {
			*v++ = isc_segstr_eof;
			REM_restore_thread_data();
			return user_status[1];
		}

		/* Here's the loop, passing out data from our basket & refilling it.
		   Our buffer (described by the structure blob) is counted strings 
		   <count word> <string> <count word> <string>... */

		while (true) {
			/* If there's data to be given away, give some away (p points to the
			   local data) */

			if (blob->rbl_length) {
				UCHAR* p = blob->rbl_ptr;

				/* If there was a fragment left over last time use it */

				USHORT l = blob->rbl_fragment_length;
				if (l) {
					blob->rbl_fragment_length = 0;

				/* otherwise pick up the count word as the length, & decrement the
				   local length */
				}
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
					*v = isc_segment;
				}

				/* and, just for yucks, see if we're exactly using up the fragment
				   part of a previous incomplete read - if so mark this as an
				   incomplete read */

				if (l == buffer_length &&
					l == blob->rbl_length && (blob->rbl_flags & RBL_segment))
				{
					*v = isc_segment;
				}

				/* finally set up the return length, decrement the current length,
				   copy the data, and indicate where to start next time. */

				*length += l;
				blob->rbl_length -= l;
				blob->rbl_offset += l;
				buffer_length -= l;

				if (l) {
					if (((U_IPTR) buffer & (ALIGNMENT - 1))
						|| ((U_IPTR) p & (ALIGNMENT - 1)))
						memcpy(buffer, p, l);
					else
						mov_faster((SLONG *) p, (SLONG *) buffer, l);
				}

				buffer += l;
				p += l;
				blob->rbl_ptr = p;

				/* return if we've filled up the caller's buffer, or completed a
				   segment */

				if (!buffer_length ||
					blob->rbl_length || !(blob->rbl_flags & RBL_segment)) break;
			}

			/* We're done with buffer.  If this was the last, we're done */

			if (blob->rbl_flags & RBL_eof_pending) {
				blob->rbl_flags |= RBL_eof;
				*v = isc_segstr_eof;
				break;
			}

			/* Preparatory to asking for more data, use input buffer length
			   to cue more efficient blob buffering. */

			/* Allocate 2 extra bytes to handle the special case where the
			   segment size of blob in the database is equal to the buffer
			   size that the user has passed.

			   Do not go into this loop if we already have a buffer 
			   of size 65535 or 65534. */

			if (buffer_length > blob->rbl_buffer_length - sizeof(USHORT) &&
				blob->rbl_buffer_length <= MAX_USHORT - sizeof(USHORT))
			{
				ULONG new_size = buffer_length + sizeof(USHORT);

				if (new_size > MAX_USHORT)	/* Check if we've overflown */
					new_size = buffer_length;
				if (blob->rbl_buffer != blob->rbl_data)
					ALLR_release(blob->rbl_buffer);
				blob->rbl_ptr = blob->rbl_buffer = ALLR_alloc((SLONG) new_size);
				/* NOMEM: ALLR_alloc handled */
				/* FREE:  in release_blob()  */
				blob->rbl_buffer_length = (USHORT) new_size;
			}

			/* We need more data.  Ask for it politely */

			packet->p_operation = op_get_segment;
			segment->p_sgmt_length = blob->rbl_buffer_length;
			segment->p_sgmt_blob = blob->rbl_id;
			segment->p_sgmt_segment.cstr_length = 0;
			if (!send_packet(rdb->rdb_port, packet, user_status))
				return error(user_status);

			response->p_resp_data.cstr_allocated = blob->rbl_buffer_length;
			response->p_resp_data.cstr_address = blob->rbl_buffer;

			if (!receive_response(rdb, packet)) {
				response->p_resp_data = temp;
				return error(user_status);
			}

			blob->rbl_length = response->p_resp_data.cstr_length;
			blob->rbl_ptr = blob->rbl_buffer;
			blob->rbl_flags &= ~RBL_segment;
			if (response->p_resp_object == 1)
				blob->rbl_flags |= RBL_segment;
			else if (response->p_resp_object == 2)
				blob->rbl_flags |= RBL_eof_pending;
		}

		response->p_resp_data = temp;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	REM_restore_thread_data();

	return user_status[1];
}


ISC_STATUS GDS_GET_SLICE(ISC_STATUS* user_status,
					 RDB* db_handle,
					 RTR* tra_handle,
					 BID array_id,
					 USHORT sdl_length,
					 const UCHAR* sdl,
					 USHORT param_length,
					 const UCHAR* param,
					 SLONG slice_length, UCHAR* slice, SLONG* return_length)
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
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	RDB rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	CHECK_HANDLE((*tra_handle), type_rtr, isc_bad_trans_handle);
	RTR transaction = *tra_handle;
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		if (rdb->rdb_port->port_protocol < PROTOCOL_VERSION4) {
			return unsupported(user_status);
		}
		/* Parse the sdl in case blr_d_float must be converted to blr_double */

		const UCHAR* new_sdl;
		if (rdb->rdb_port->port_protocol < PROTOCOL_VERSION6) {
			new_sdl = SDL_prepare_slice(sdl, sdl_length);
		}
		else {
			new_sdl = sdl;
		}

		// CVC: Modified this horrible idea: don't touch input parameters!
		/* The modified (perhaps) sdl is send to the remote connection.  The
		   original sdl is used to process the slice data when it is received.
		   (This is why both 'new_sdl' and 'sdl' are saved in the packet.) */
		UCHAR sdl_buffer[128];
		UCHAR* old_sdl = SDL_clone_sdl(sdl, sdl_length, sdl_buffer, sizeof(sdl_buffer));

		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = op_get_slice;
		P_SLC* data = &packet->p_slc;
		data->p_slc_transaction = transaction->rtr_id;
		data->p_slc_id = *array_id;
		data->p_slc_length = slice_length;
		data->p_slc_sdl.cstr_length = sdl_length;
		data->p_slc_sdl.cstr_address = const_cast<UCHAR*>(new_sdl);
		data->p_slc_parameters.cstr_length = param_length;
		data->p_slc_parameters.cstr_address = const_cast<UCHAR*>(param);

		data->p_slc_slice.lstr_length = 0;
		data->p_slc_slice.lstr_address = slice;

		P_SLR* response = &packet->p_slr;
		response->p_slr_sdl = old_sdl; //const_cast<UCHAR*>(sdl);
		response->p_slr_sdl_length = sdl_length;
		response->p_slr_slice.lstr_address = slice;
		response->p_slr_slice.lstr_length = slice_length;

		bool err_flag = false;
		if (!send_packet(rdb->rdb_port, packet, user_status))
			err_flag = true;
		else {
			packet->p_resp.p_resp_status_vector = rdb->rdb_status_vector;
			if (!receive_packet(rdb->rdb_port, packet, user_status))
				err_flag = true;
		}
		if (new_sdl != sdl) {
			gds__free((void*) new_sdl);
		}
		if (old_sdl != sdl_buffer) {
		    gds__free(old_sdl);
		}
		if (err_flag)
			return error(user_status);

		if (packet->p_operation != op_slice) {
			check_response(rdb, packet);
			return error(user_status);
		}

		if (return_length)
			*return_length = response->p_slr_length;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_OPEN_BLOB2(ISC_STATUS* user_status,
					  RDB* db_handle,
					  RTR* rtr_handle,
					  RBL* blob_handle,
					  BID blob_id, USHORT bpb_length, const UCHAR* bpb)
{
/**************************************
 *
 *	g d s _ o p e n _ b l o b 2
 *
 **************************************
 *
 * Functional description
 *	Open an existing blob.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	NULL_CHECK(blob_handle, isc_bad_segstr_handle);
	RDB rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	CHECK_HANDLE((*rtr_handle), type_rtr, isc_bad_trans_handle);
	RTR transaction = *rtr_handle;
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = op_open_blob;
		P_BLOB* p_blob = &packet->p_blob;
		p_blob->p_blob_transaction = transaction->rtr_id;
		p_blob->p_blob_id = *blob_id;

		if (rdb->rdb_port->port_protocol >= PROTOCOL_VERSION4) {
			packet->p_operation = op_open_blob2;
			p_blob->p_blob_bpb.cstr_length = bpb_length;
			fb_assert(!p_blob->p_blob_bpb.cstr_allocated ||
				p_blob->p_blob_bpb.cstr_allocated < p_blob->p_blob_bpb.cstr_length);
			// CVC: Should we ensure here that cstr_allocated < bpb_length???
			// Otherwise, xdr_cstring() calling alloc_string() to decode would
			// cause memory problems on the client side for SS, as the client
			// would try to write to the application's provided R/O buffer.
			p_blob->p_blob_bpb.cstr_address = bpb;
		}

		if (send_and_receive(rdb, packet, user_status)) {
			return error(user_status);
		}
		// CVC: It's not evident to me why these two lines that I've copied
		// here as comments are only found in create_blob calls.
		// I think they should be enabled to avoid whatever buffer corruption.
		//p_blob->p_blob_bpb.cstr_length = 0;
		//p_blob->p_blob_bpb.cstr_address = NULL;

		RBL blob = (RBL) ALLR_block(type_rbl, BLOB_LENGTH);
		*blob_handle = blob;
		blob->rbl_rdb = rdb;
		blob->rbl_rtr = transaction;
		blob->rbl_id = packet->p_resp.p_resp_object;
		blob->rbl_buffer_length = BLOB_LENGTH;
		SET_OBJECT(rdb, blob, blob->rbl_id);
		blob->rbl_next = transaction->rtr_blobs;
		blob->rbl_ptr = blob->rbl_buffer = blob->rbl_data;
		transaction->rtr_blobs = blob;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_PREPARE(ISC_STATUS * user_status,
				   RTR * rtr_handle, USHORT msg_length, UCHAR * msg)
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
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	RTR transaction = *rtr_handle;
	CHECK_HANDLE(transaction, type_rtr, isc_bad_trans_handle);
	RDB rdb = (*rtr_handle)->rtr_rdb;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		/* Handle historical version */

		if (rdb->rdb_port->port_protocol < PROTOCOL_VERSION4) {
			if (msg_length) {
				return unsupported(user_status);
			}
			if (!release_object(rdb, op_prepare, transaction->rtr_id)) {
				return error(user_status);
			}
			return return_success(rdb);
		}

		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = op_prepare2;
		packet->p_prep.p_prep_transaction = transaction->rtr_id;
		packet->p_prep.p_prep_data.cstr_length = msg_length;
		packet->p_prep.p_prep_data.cstr_address = msg;

		if (!send_packet(rdb->rdb_port, packet, user_status) ||
			!receive_response(rdb, packet))
		{
			return error(user_status);
		}
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_PUT_SEGMENT(ISC_STATUS* user_status,
					   RBL* blob_handle,
					   USHORT segment_length, const UCHAR* segment)
{
/**************************************
 *
 *	g d s _ p u t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Emit a blob segment.  If the protocol allows,
 *	the segment is buffered locally for a later
 *	batch put.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	/* Sniff out handles, etc, and find the various blocks. */

	CHECK_HANDLE((*blob_handle), type_rbl, isc_bad_segstr_handle);
	RBL blob = *blob_handle;
	RDB rdb = blob->rbl_rdb;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;
	rem_port* port = rdb->rdb_port;

	try
	{
		/* If this is an ancient protocol, just send the segment.
		   Also handle the new protocol on a blob that has been
		   opened rather than created.   (This should yield an error.) */

		if ((port->port_flags & PORT_rpc) || !(blob->rbl_flags & RBL_create))
		{
			send_blob(user_status, blob, segment_length, segment);
			REM_restore_thread_data();
			return user_status[1];
		}

		/* If the buffer can't hold the complete incoming segment, flush out the
		   buffer.  If the incoming segment is too large to fit into the blob
		   buffer, just send it as a single segment. */

		UCHAR* p = blob->rbl_ptr;
		const USHORT l = blob->rbl_buffer_length - (p - blob->rbl_buffer);

		if ((ULONG) segment_length + 2 > l) {
			if (blob->rbl_ptr > blob->rbl_buffer) {
				if (send_blob(user_status, blob, 0, NULL)) {
					REM_restore_thread_data();
					return user_status[1];
				}
			}
			if ((ULONG) segment_length + 2 > blob->rbl_buffer_length) {
				send_blob(user_status, blob, segment_length, segment);
				REM_restore_thread_data();
				return user_status[1];
			}
			p = blob->rbl_buffer;
		}

		/* Move segment length and data into blob buffer */

		*p++ = (UCHAR) segment_length;
		*p++ = segment_length >> 8;

		if (segment_length) {
			if (((U_IPTR) segment & (ALIGNMENT - 1))
				|| ((U_IPTR) p & (ALIGNMENT - 1)))
			{
				memcpy(p, segment, segment_length);
			}
			else {
				mov_faster(reinterpret_cast<const SLONG*>(segment), (SLONG*) p,
					segment_length);
			}
		}

		blob->rbl_ptr = p + segment_length;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_PUT_SLICE(ISC_STATUS* user_status,
					 RDB* db_handle,
					 RTR* tra_handle,
					 BID array_id,
					 USHORT sdl_length,
					 const UCHAR* sdl,
					 USHORT param_length,
					 const UCHAR* param, SLONG slice_length, UCHAR* slice)
{
/**************************************
 *
 *	g d s _ p u t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *	Store a slice of an array.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	RDB rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	CHECK_HANDLE((*tra_handle), type_rtr, isc_bad_trans_handle);
	RTR transaction = *tra_handle;
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		if (rdb->rdb_port->port_protocol < PROTOCOL_VERSION4) {
			return unsupported(user_status);
		}

		/* Parse the sdl in case blr_d_float must be converted to blr_double */

		const UCHAR* new_sdl;
		if (rdb->rdb_port->port_protocol < PROTOCOL_VERSION6) {
			new_sdl = SDL_prepare_slice(sdl, sdl_length);
		} 
		else {
			new_sdl = sdl;
		}

		// CVC: Modified this horrible idea: don't touch input parameters!
		/* The modified (perhaps) sdl is send to the remote connection.  The
		   original sdl is used to process the slice data before it is sent.
		   (This is why both 'new_sdl' and 'sdl' are saved in the packet.) */
		UCHAR sdl_buffer[128];
		UCHAR* old_sdl = SDL_clone_sdl(sdl, sdl_length, sdl_buffer, sizeof(sdl_buffer));

		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = op_put_slice;
		P_SLC* data = &packet->p_slc;
		data->p_slc_transaction = transaction->rtr_id;
		data->p_slc_id = *array_id;
		data->p_slc_length = slice_length;
		data->p_slc_sdl.cstr_length = sdl_length;
		data->p_slc_sdl.cstr_address = const_cast<UCHAR*>(new_sdl);
		data->p_slc_parameters.cstr_length = param_length;
		data->p_slc_parameters.cstr_address = const_cast<UCHAR*>(param); // safe
		data->p_slc_slice.lstr_length = slice_length;
		data->p_slc_slice.lstr_address = slice;

		P_SLR* response = &packet->p_slr;
		response->p_slr_sdl = old_sdl; //const_cast<UCHAR*>(sdl);
		response->p_slr_sdl_length = sdl_length;
		response->p_slr_slice.lstr_address = slice;
		response->p_slr_slice.lstr_length = slice_length;

		send_and_receive(rdb, packet, user_status);
		if (new_sdl != sdl) {
			gds__free((void*) new_sdl);
		}
		if (old_sdl != sdl_buffer) {
		    gds__free(old_sdl);
		}

		if (user_status[1]) {
			return error(user_status);
		}

		*array_id = packet->p_resp.p_resp_blob_id;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_QUE_EVENTS(ISC_STATUS* user_status,
					  RDB* handle,
					  SLONG* id,
					  SSHORT length,
					  const UCHAR* items,
					  FPTR_EVENT_CALLBACK ast, void* arg)
{
/**************************************
 *
 *	g d s _ $ q u e _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *	Queue a request for event notification.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	RDB rdb = *handle;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;
	rem_port* port = rdb->rdb_port;
	PACKET* packet = &rdb->rdb_packet;

	try
	{
		/* Make sure protocol support action */

		if (rdb->rdb_port->port_protocol < PROTOCOL_VERSION4) {
			return unsupported(user_status);
		}

		/* If there isn't a auxiliary asynchronous port, make one now */

		if (!port->port_async)
		{
			packet->p_operation = op_connect_request;
			P_REQ* request = &packet->p_req;
			request->p_req_object = rdb->rdb_id;
			request->p_req_type = P_REQ_async;
			if (!send_packet(port, packet, user_status)
				|| !receive_response(rdb, packet))
			{
				return error(user_status);
			}
#ifdef MULTI_THREAD
			if (!port->connect(packet, 0)) {
				return error(user_status);
			}

			gds__thread_start(event_thread, port->port_async, 
				THREAD_high, THREAD_ast, 0);
#else
			if (!port->connect(packet, event_handler)) {
				return error(user_status);
			}
#endif
			port->port_async->port_context = rdb;
		}

		/* Add event block to port's list of active remote events */

		RVNT rem_event = add_event(port);

		rem_event->rvnt_ast = ast;
		rem_event->rvnt_arg = arg;
		rem_event->rvnt_port = port->port_async;
		rem_event->rvnt_items = items;
		rem_event->rvnt_length = length;
		rem_event->rvnt_rdb = rdb;

		/* Update id value */

		*id = rem_event->rvnt_id;

		/* Build the primary packet to get the operation started. */

		packet = &rdb->rdb_packet;
		packet->p_operation = op_que_events;

		P_EVENT* event = &packet->p_event;
		event->p_event_database = rdb->rdb_id;
		event->p_event_items.cstr_length = length;
		event->p_event_items.cstr_address = const_cast<UCHAR*>(items);
		event->p_event_ast = ast;
		// Nickolay Samofatov: We pass this value to the server (as 32-bit value)
		// then it returns it to us and we do not use it. Maybe pass zero here
		// to avoid client-side security risks?
		event->p_event_arg = (SLONG) (IPTR) arg;
		event->p_event_rid = rem_event->rvnt_id;

		if (!send_packet(port, packet, user_status) ||
			!receive_response(rdb, packet))
		{
			return error(user_status);
		}
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_RECEIVE(ISC_STATUS * user_status,
				   rrq** req_handle,
				   USHORT msg_type,
				   USHORT msg_length, UCHAR * msg, SSHORT level
#ifdef SCROLLABLE_CURSORS
				   , USHORT direction, ULONG offset
#endif
	)
{
/**************************************
 *
 *	g d s _ r e c e i v e
 *
 **************************************
 *
 * Functional description
 *	Give a client program a record.  Ask the
 *	Remote server to send it to us if necessary.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

/* Check handles and environment, then set up error handling */

	CHECK_HANDLE((*req_handle), type_rrq, isc_bad_req_handle);
	rrq* request = REMOTE_find_request(*req_handle, level);
	RDB rdb = request->rrq_rdb;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		rem_port* port = rdb->rdb_port;
		rrq::rrq_repeat* tail = &request->rrq_rpt[msg_type];

		REM_MSG message = tail->rrq_message;
#ifdef SCROLLABLE_CURSORS
		if (port->port_protocol >= PROTOCOL_SCROLLABLE_CURSORS)
		{
			message = scroll_cache(user_status,
									tdrdb,
									request,
									port,
									tail,
									&direction,
									&offset);
			if (!message) {
				return error(user_status);
			}
		}
#endif
			

#ifdef DEBUG
		fprintf(stdout, "Rows Pending in REM_receive=%d\n",
				   tail->rrq_rows_pending);
#endif

		/* Check to see if data is waiting.  If not, solicit data. 
		   Solicit data either when we've run out, or there's a low
		   inventory of messages in local buffers & no shipments on the
		   ether being sent to us. */

		if (!request->rrq_status_vector[1] &&	/* No error pending */
			((!message->msg_address && tail->rrq_rows_pending == 0) ||	/* No message waiting */
			 (tail->rrq_rows_pending <= tail->rrq_reorder_level &&	/* Low in inventory */
			  tail->rrq_msgs_waiting <= tail->rrq_reorder_level &&
			  !(port->port_flags & PORT_rpc) &&	/* doing Batch, not RPC */
			  /* Pipelining causes both server & client to 
				 write at the same time. In named pipes, writes
				 block for the other end to read -  and so when both
				 attempt to write simultaenously, they end up
				 waiting indefinetly for the other end to read */
			  (port->port_type != port_pipe) &&	/* not named pipe on NT */
#ifdef XNET
			  (port->port_type != port_xnet) &&	/* not named pipe on NT */
#endif
			  request->rrq_max_msg <= 1))) 
		{	/* there's only one message type */

#ifdef DEBUG
			fprintf(stderr, "Rows Pending %d\n", tail->rrq_rows_pending);
			if (!message->msg_address)
				fprintf(stderr, "Out of data - reordering\n");
			else
				fprintf(stderr, "Low on inventory - reordering\n");
#endif

			/* Format a request for data */

			PACKET *packet = &rdb->rdb_packet;
			packet->p_operation = op_receive;
			P_DATA* data = &packet->p_data;
			data->p_data_request = request->rrq_id;
			data->p_data_message_number = msg_type;
			data->p_data_incarnation = level;
#ifdef SCROLLABLE_CURSORS
			/* if the protocol can handle it, tell the server to scroll before returning records */

			if (port->port_protocol >= PROTOCOL_SCROLLABLE_CURSORS) {
				data->p_data_direction = direction;
				data->p_data_offset = offset;

				/* set the appropriate flags according to the way we're about to scroll 
				   the next layer down, and calculate the offset from the beginning 
				   of the result set */

				switch (direction) {
				case blr_forward:
					tail->rrq_flags &= ~RRQ_backward;
					tail->rrq_absolute +=
						(tail->
						 rrq_flags & RRQ_absolute_backward) ? -offset : offset;
					break;

				case blr_backward:
					tail->rrq_flags |= RRQ_backward;
					tail->rrq_absolute +=
						(tail->
						 rrq_flags & RRQ_absolute_backward) ? offset : -offset;
					break;

				case blr_bof_forward:
					tail->rrq_flags &= ~RRQ_backward;
					tail->rrq_flags &= ~RRQ_absolute_backward;
					tail->rrq_absolute = offset;
					direction = blr_forward;
					break;

				case blr_eof_backward:
					tail->rrq_flags |= RRQ_backward;
					tail->rrq_flags |= RRQ_absolute_backward;
					tail->rrq_absolute = offset;
					direction = blr_backward;
					break;
				}
			}
#endif

			/* Compute how many to send in a batch.  While this calculation
			   is the same for each batch (June 1996), perhaps in the future it
			   could dynamically adjust batching sizes based on fetch patterns */

			if (port->port_flags & PORT_rpc)

				/* This is an RPC (remote procedure call) port - we just do
				   one at a time processing as that's how RPC works. */

				data->p_data_messages = 1;
			else {
				data->p_data_messages =
					static_cast < USHORT >
					(REMOTE_compute_batch_size
					 (port, 0, op_send, tail->rrq_format));
				tail->rrq_reorder_level = 2 * data->p_data_messages;
				data->p_data_messages *= 4;
				tail->rrq_rows_pending += data->p_data_messages;

#ifdef DEBUG
				fprintf(stdout,
						   "Recalculating Rows Pending in REM_receive=%d\n",
						   tail->rrq_rows_pending);
#endif
			}

#ifdef DEBUG
			fprintf(stderr, "port_flags %d max_msg %d\n", port->port_flags,
					   request->rrq_max_msg);
			fprintf(stderr, "Fetch: Req One batch of %d messages\n",
					   data->p_data_messages);
#endif

			if (!send_packet(rdb->rdb_port, packet, user_status))
				return error(user_status);
			tail->rrq_batch_count++;

#ifdef DEBUG
			fprintf(stderr, "Rows Pending %d\n", tail->rrq_rows_pending);
#endif

			/* Queue up receipt of the pending data */

			enqueue_receive(port, batch_gds_receive, rdb, request, tail);
		}

		/* Receive queued responses until we have some data for this cursor
		   or an error status has been received. */

		/* We've either got data, or some is on the way, or we have an error */

		fb_assert(message->msg_address || (tail->rrq_rows_pending > 0)
			   || request->rrq_status_vector[1]);

		while (!message->msg_address && !request->rrq_status_vector[1])
			if (!receive_queued_packet(tdrdb, port, user_status, request->rrq_id))
				return error(user_status);

		if (!message->msg_address && request->rrq_status_vector[1]) {
			/* The previous batch of receives ended with an error status.
			   We're all done returning data in the local queue.
			   Return that error status vector to the user. */

			/* Stuff in the error result to the user's vector */

			memcpy(user_status, request->rrq_status_vector,
				   sizeof(request->rrq_status_vector));
			memset(request->rrq_status_vector, 0,
				   sizeof(request->rrq_status_vector));

			REM_restore_thread_data();
			return user_status[1];
		}

		/* Copy data from the message buffer to the client buffer */

		message = tail->rrq_message;
		if ((U_IPTR) msg & (ALIGNMENT - 1))
			memcpy(msg, message->msg_address, msg_length);
		else
			mov_faster((SLONG *) message->msg_address, (SLONG *) msg, msg_length);

#ifdef SCROLLABLE_CURSORS
		tail->rrq_last = message;
#else
		/* Move the head-of-full-buffer-queue pointer forward */

		tail->rrq_message = message->msg_next;

		/* Mark the buffer the message came from as available for reuse */

		message->msg_address = NULL;
#endif
		tail->rrq_msgs_waiting--;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_RECONNECT(ISC_STATUS* user_status,
					 RDB* db_handle,
					 RTR* rtr_handle, USHORT length, const UCHAR* id)
{
/**************************************
 *
 *	g d s _ r e c o n n e c t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	NULL_CHECK(rtr_handle, isc_bad_trans_handle);
	RDB rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = op_reconnect;
		P_STTR* trans = &packet->p_sttr;
		trans->p_sttr_database = rdb->rdb_id;
		trans->p_sttr_tpb.cstr_length = length;
		trans->p_sttr_tpb.cstr_address = const_cast<UCHAR*>(id);

		if (send_and_receive(rdb, packet, user_status)) {
			return error(user_status);
		}

		*rtr_handle = make_transaction(rdb, packet->p_resp.p_resp_object);
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_RELEASE_REQUEST(ISC_STATUS * user_status, rrq** req_handle)
{
/**************************************
 *
 *	g d s _ r e l e a s e _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Release a request.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	rrq* request = *req_handle;
	CHECK_HANDLE(request, type_rrq, isc_bad_req_handle);
	RDB rdb = request->rrq_rdb;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		if (!release_object(rdb, op_release, request->rrq_id)) {
			return error(user_status);
		}

		release_request(request);
		*req_handle = NULL;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_REQUEST_INFO(ISC_STATUS* user_status,
						rrq** req_handle,
						SSHORT level,
						SSHORT item_length,
						const UCHAR* items, SSHORT buffer_length, UCHAR* buffer)
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
	ISC_STATUS status;
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	rrq* request = REMOTE_find_request(*req_handle, level);
	CHECK_HANDLE(request, type_rrq, isc_bad_req_handle);
	RDB rdb = request->rrq_rdb;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		/* Check for buffered message.  If there is, report on it locally. */
		const rrq::rrq_repeat* tail= request->rrq_rpt;
		for (const rrq::rrq_repeat* const end = tail + request->rrq_max_msg;
			 tail <= end; tail++)
		{
			REM_MSG msg = tail->rrq_message;
			if (!msg || !msg->msg_address) {
				continue;
			}

			/* We've got a pending message, respond locally */

			const rem_fmt* format = tail->rrq_format;
			UCHAR* out = buffer;
			const UCHAR* info_items = items;
			const UCHAR* const end_items = info_items + item_length;

			while (info_items < end_items) {
				USHORT data;
				const UCHAR item = *info_items++;
				switch (item) {
				case isc_info_end:
					break;

				case isc_info_state:
					data = isc_info_req_send;
					break;

				case isc_info_message_number:
					data = msg->msg_number;
					break;

				case isc_info_message_size:
					data = format->fmt_length;
					break;

				default:
					goto punt;
				}

				*out++ = item;
				if (item == isc_info_end)
					break;

				*out++ = 2;
				*out++ = 2 >> 8;
				*out++ = (UCHAR) data;
				*out++ = data >> 8;
			}

			return return_success(rdb);
		}

		/* No message pending, request status from other end */

punt:

		status = info(user_status, rdb, op_info_request, request->rrq_id, level,
					  item_length, (const SCHAR*) items, 0, 0, buffer_length,
					  (SCHAR *) buffer);
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	REM_restore_thread_data();

	return status;
}


ISC_STATUS GDS_ROLLBACK_RETAINING(ISC_STATUS * user_status, RTR * rtr_handle)
{
/**************************************
 *
 *	i s c _ r o l l b a c k _ r e t a i n i n g
 *
 **************************************
 *
 * Functional description
 *	Abort a transaction but keep its environment valid
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	RTR transaction = *rtr_handle;
	CHECK_HANDLE(transaction, type_rtr, isc_bad_trans_handle);
	RDB rdb = (*rtr_handle)->rtr_rdb;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		/* Make sure protocol support action */

		if (rdb->rdb_port->port_protocol < PROTOCOL_VERSION10) {
			return unsupported(user_status);
		}

		if (!release_object(rdb, op_rollback_retaining, transaction->rtr_id)) {
			return error(user_status);
		}
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_ROLLBACK(ISC_STATUS * user_status, RTR * rtr_handle)
{
/**************************************
 *
 *	g d s _ r o l l b a c k
 *
 **************************************
 *
 * Functional description
 *	Abort a transaction.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	RTR transaction = *rtr_handle;
	CHECK_HANDLE(transaction, type_rtr, isc_bad_trans_handle);
	RDB rdb = (*rtr_handle)->rtr_rdb;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		if (!release_object(rdb, op_rollback, transaction->rtr_id)) {
			return error(user_status);
		}

		REMOTE_cleanup_transaction(transaction);
		release_transaction(transaction);
		*rtr_handle = NULL;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_SEEK_BLOB(ISC_STATUS * user_status,
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
 *	Seek into a blob.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	RBL blob = *blob_handle;
	CHECK_HANDLE(blob, type_rbl, isc_bad_segstr_handle);
	RDB rdb = blob->rbl_rdb;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		if (rdb->rdb_port->port_protocol < PROTOCOL_VERSION6) {
			return unsupported(user_status);
		}

		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = op_seek_blob;
		P_SEEK* seek = &packet->p_seek;
		seek->p_seek_blob = blob->rbl_id;
		seek->p_seek_mode = mode;
		seek->p_seek_offset = offset;

		if (mode == 1) {
			seek->p_seek_mode = 0;
			seek->p_seek_offset = blob->rbl_offset + offset;
		}

		if (send_and_receive(rdb, packet, user_status)) {
			return error(user_status);
		}

		blob->rbl_offset = *result = packet->p_resp.p_resp_blob_id.bid_number;
		blob->rbl_length = 0;
		blob->rbl_fragment_length = 0;
		blob->rbl_flags &= ~(RBL_eof | RBL_eof_pending | RBL_segment);
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_SEND(ISC_STATUS * user_status,
				rrq** req_handle,
				USHORT msg_type, USHORT msg_length, UCHAR * msg, SSHORT level)
{
/**************************************
 *
 *	g d s _ s e n d
 *
 **************************************
 *
 * Functional description
 *	Send a message to the server.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	CHECK_HANDLE((*req_handle), type_rrq, isc_bad_req_handle);
	rrq* request = REMOTE_find_request(*req_handle, level);
	RDB rdb = request->rrq_rdb;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	if (msg_type > request->rrq_max_msg)
		return handle_error(user_status, isc_badmsgnum);

	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		REM_MSG message = request->rrq_rpt[msg_type].rrq_message;
		message->msg_address = msg;

		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = op_send;
		P_DATA* data = &packet->p_data;
		data->p_data_request = request->rrq_id;
		data->p_data_message_number = msg_type;
		data->p_data_incarnation = level;

		if (!send_packet(rdb->rdb_port, packet, user_status)) {
			return error(user_status);
		}

		/* Bump up the message pointer to resync with rrq_xdr (rrq_xdr 
		   was incremented by xdr_request in the SEND call).  */

		message->msg_address = NULL;
		request->rrq_rpt[msg_type].rrq_message = message->msg_next;

		if (!receive_response(rdb, packet)) {
			return error(user_status);
		}
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_SERVICE_ATTACH(ISC_STATUS* user_status,
						  USHORT service_length,
						  const TEXT* service_name,
						  RDB* handle, USHORT spb_length, const SCHAR* spb)
{
/**************************************
 *
 *	g d s _ s e r v i c e _ a t t a c h
 *
 **************************************
 *
 * Functional description
 *	Connect to an Interbase service.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	NULL_CHECK(handle, isc_bad_svc_handle);

	Firebird::PathName expanded_name;
	if (service_length) {
		expanded_name.assign(service_name, service_length);
	}
	else {
		expanded_name.assign(service_name);
	}

	ISC_STATUS* v = user_status;
	*v++ = isc_arg_gds;
	*v++ = isc_unavailable;
	*v = isc_arg_end;

	rem_port* port;
	UCHAR new_spb[MAXPATHLEN];
	UCHAR* new_spb_ptr = new_spb;
	if ((spb_length + MAX_USER_LENGTH + MAX_PASSWORD_ENC_LENGTH +
		 MAX_OTHER_PARAMS) > sizeof(new_spb))
	{
		new_spb_ptr =
			(UCHAR*)gds__alloc(spb_length + MAX_USER_LENGTH +
					   MAX_PASSWORD_ENC_LENGTH + MAX_OTHER_PARAMS);

		/* FREE: by return(s) in this routine */

		if (!new_spb_ptr)
		{		/* NOMEM: return error to client */
			user_status[1] = isc_virmemexh;
			return error(user_status);
		}
	}
	USHORT new_spb_length;
	TEXT user_string[256];
	const bool user_verification =
		get_new_dpb(reinterpret_cast<const UCHAR*>(spb),
					spb_length, false, new_spb_ptr,
					&new_spb_length, user_string);

	const TEXT* us = (user_string[0]) ? user_string : 0;

	port = analyze_service(expanded_name, user_status, us,
						 user_verification, spb, spb_length);
	if (!port) {
		if (new_spb_ptr != new_spb)
			gds__free(new_spb_ptr);
		return error(user_status);
	}

	RDB rdb = port->port_context;
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		/* make sure the protocol supports it */

		if (port->port_protocol < PROTOCOL_VERSION8) {
			if (new_spb_ptr != new_spb) {
				gds__free(new_spb_ptr);
			}
			disconnect(port);
			return unsupported(user_status);
		}

		/* The client may have set a parameter for dummy_packet_interval.  Add that to the
		   the SPB so the server can pay attention to it.  Note: allocation code must
		   ensure sufficient space has been added. */

		add_other_params(port, new_spb_ptr, &new_spb_length);

		const bool result =
			init(user_status, port, op_service_attach, expanded_name,
				 new_spb_ptr, new_spb_length);
		if (new_spb_ptr != new_spb) {
			gds__free(new_spb_ptr);
		}
		if (!result) {
			return error(user_status);
		}

		*handle = rdb;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_SERVICE_DETACH(ISC_STATUS * user_status, RDB * handle)
{
/**************************************
 *
 *	g d s _ s e r v i c e _ d e t a c h
 *
 **************************************
 *
 * Functional description
 *	Close down a connection to an Interbase service.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	/* Check and validate handles, etc. */

	RDB rdb = *handle;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_svc_handle);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;
	rem_port* port = rdb->rdb_port;

	try
	{
		/* make sure the protocol supports it */

		if (port->port_protocol < PROTOCOL_VERSION8) {
			return unsupported(user_status);
		}

		if (!release_object(rdb, op_service_detach, rdb->rdb_id)) {
			return error(user_status);
		}

		disconnect(port);
		*handle = NULL;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	/* Note: Can't return_success(rdb) here as we've torn down memory already */

	REM_restore_thread_data();

	*user_status++ = isc_arg_gds;
	*user_status++ = FB_SUCCESS;
	*user_status = isc_arg_end;

	return FB_SUCCESS;
}


ISC_STATUS GDS_SERVICE_QUERY(ISC_STATUS* user_status,
						 RDB* svc_handle,
						 ULONG* reserved,
						 USHORT item_length,
						 const SCHAR* items,
						 USHORT recv_item_length,
						 const SCHAR* recv_items,
						 USHORT buffer_length, SCHAR* buffer)
{
/**************************************
 *
 *	g d s _ s e r v i c e _ q u e r y
 *
 **************************************
 *
 * Functional description
 *	Provide information on service object.
 * 
 *	NOTE: The parameter RESERVED must not be used
 *	for any purpose as there are networking issues
 *	involved (as with any handle that goes over the
 *	network).  This parameter will be implemented at 
 *	a later date.
 **************************************/
	ISC_STATUS status;
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	/* Check and validate handles, etc. */

	RDB rdb = *svc_handle;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_svc_handle);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		/* make sure the protocol supports it */

		if (rdb->rdb_port->port_protocol < PROTOCOL_VERSION8) {
			return unsupported(user_status);
		}

		status = info(user_status, rdb, op_service_info, rdb->rdb_id, 0,
					  item_length, items, recv_item_length, recv_items,
					  buffer_length, buffer);
	}
	catch (const std::exception& ex)
	{
		status = error(user_status, ex);
	}

	REM_restore_thread_data();

	return status;
}


ISC_STATUS GDS_SERVICE_START(ISC_STATUS * user_status,
						 RDB* svc_handle,
						 ULONG* reserved, USHORT item_length, const SCHAR* items)
{
/**************************************
 *
 *	g d s _ s e r v i c e _ s t a r t
 *
 **************************************
 *
 * Functional description
 *	Start an InterBase service
 *
 * 	NOTE: The parameter RESERVED must not be used
 *	for any purpose as there are networking issues
 *	involved (as with any handle that goes over the
 *	network).  This parameter will be implemented at 
 *	a later date.
 **************************************/
	ISC_STATUS status;
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	/* Check and validate handles, etc. */

	RDB rdb = *svc_handle;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_svc_handle);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		/* make sure the protocol supports it */

		if (rdb->rdb_port->port_protocol < PROTOCOL_VERSION8) {
			return unsupported(user_status);
		}

		status =
			svcstart(user_status, rdb, op_service_start, rdb->rdb_id, 0,
					 item_length, items);
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	REM_restore_thread_data();

	return status;
}


ISC_STATUS GDS_START_AND_SEND(ISC_STATUS * user_status,
						  rrq** req_handle,
						  RTR* rtr_handle,
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
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	CHECK_HANDLE((*req_handle), type_rrq, isc_bad_req_handle);
	CHECK_HANDLE((*rtr_handle), type_rtr, isc_bad_trans_handle);
	rrq* request = REMOTE_find_request(*req_handle, level);
	RTR transaction = *rtr_handle;
	RDB rdb = request->rrq_rdb;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	if (msg_type > request->rrq_max_msg)
		return handle_error(user_status, isc_badmsgnum);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		if ((*rtr_handle)->rtr_rdb != rdb) {
			user_status[0] = isc_arg_gds;
			user_status[1] = isc_trareqmis;
			user_status[2] = isc_arg_end;
			return error(user_status);
		}

		if (!clear_queue(rdb->rdb_port, user_status)) {
			return error(user_status);
		}

		REMOTE_reset_request(request, 0);
		REM_MSG message = request->rrq_rpt[msg_type].rrq_message;
		message->msg_address = msg;

		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = (rdb->rdb_port->port_protocol < PROTOCOL_VERSION8) ?
			op_start_and_send : op_start_send_and_receive;
		P_DATA* data = &packet->p_data;
		data->p_data_request = request->rrq_id;
		data->p_data_transaction = transaction->rtr_id;
		data->p_data_message_number = msg_type;
		data->p_data_incarnation = level;

		if (!send_packet(rdb->rdb_port, packet, user_status))
			return error(user_status);

		/* Bump up the message pointer to resync with rrq_xdr (rrq_xdr 
		   was incremented by xdr_request in the SEND call).  */

		message->msg_address = NULL;
		request->rrq_rpt[msg_type].rrq_message = message->msg_next;

		if (!receive_response(rdb, packet))
			return error(user_status);

		/* Save the request's transaction. */

		request->rrq_rtr = transaction;

		if (rdb->rdb_port->port_protocol >= PROTOCOL_VERSION8 &&
			packet->p_operation == op_response_piggyback)
		{
			receive_after_start(request, packet->p_resp.p_resp_object);
		}
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_START(ISC_STATUS * user_status,
				 rrq** req_handle,
				 RTR* rtr_handle, USHORT level)
{
/**************************************
 *
 *	g d s _ s t a r t
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	CHECK_HANDLE((*req_handle), type_rrq, isc_bad_req_handle);
	CHECK_HANDLE((*rtr_handle), type_rtr, isc_bad_trans_handle);
	rrq* request = REMOTE_find_request(*req_handle, level);
	RTR transaction = *rtr_handle;
	RDB rdb = request->rrq_rdb;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		if ((*rtr_handle)->rtr_rdb != rdb) {
			user_status[0] = isc_arg_gds;
			user_status[1] = isc_trareqmis;
			user_status[2] = isc_arg_end;
			return error(user_status);
		}

		if (!clear_queue(rdb->rdb_port, user_status)) {
			return error(user_status);
		}

		REMOTE_reset_request(request, 0);
		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = (rdb->rdb_port->port_protocol < PROTOCOL_VERSION8) ?
			op_start : op_start_and_receive;
		P_DATA* data = &packet->p_data;
		data->p_data_request = request->rrq_id;
		data->p_data_transaction = transaction->rtr_id;
		data->p_data_message_number = 0;
		data->p_data_incarnation = level;

		if (send_and_receive(rdb, packet, user_status))
			return error(user_status);

		/* Save the request's transaction. */

		request->rrq_rtr = transaction;

		if (rdb->rdb_port->port_protocol >= PROTOCOL_VERSION8 &&
			packet->p_operation == op_response_piggyback)
		{
			receive_after_start(request, packet->p_resp.p_resp_object);
		}
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_START_TRANSACTION(ISC_STATUS * user_status,
							 RTR * rtr_handle,
							 SSHORT count,
							 RDB * db_handle, SSHORT tpb_length, UCHAR * tpb)
{
/**************************************
 *
 *	g d s _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Start a transaction.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	NULL_CHECK(rtr_handle, isc_bad_trans_handle);
	RDB rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = op_transaction;
		P_STTR* trans = &packet->p_sttr;
		trans->p_sttr_database = rdb->rdb_id;
		trans->p_sttr_tpb.cstr_length = tpb_length;
		trans->p_sttr_tpb.cstr_address = tpb;

		if (send_and_receive(rdb, packet, user_status)) {
			return error(user_status);
		}

		*rtr_handle = make_transaction(rdb, packet->p_resp.p_resp_object);
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_TRANSACT_REQUEST(ISC_STATUS* user_status,
							RDB* db_handle,
							RTR* rtr_handle,
							USHORT blr_length,
							const UCHAR* blr,
							USHORT in_msg_length,
							UCHAR* in_msg,
							USHORT out_msg_length, UCHAR* out_msg)
{
/**************************************
 *
 *	i s c _ t r a n s a c t _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Execute a procedure on remote host.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	RDB rdb = *db_handle;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	RTR transaction = *rtr_handle;
	CHECK_HANDLE(transaction, type_rtr, isc_bad_trans_handle);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;
	rem_port* port = rdb->rdb_port;

	try
	{
		/* bag it if the protocol doesn't support it... */
	
		if (port->port_protocol < PROTOCOL_VERSION8) {
			return unsupported(user_status);
		}

		RPR procedure = port->port_rpr;
		if (!procedure) {
			procedure = port->port_rpr = (RPR) ALLR_block(type_rpr, 0);
		}

		if ((*rtr_handle)->rtr_rdb != rdb) {
			user_status[0] = isc_arg_gds;
			user_status[1] = isc_trareqmis;
			user_status[2] = isc_arg_end;
			return error(user_status);
		}

		/* Parse the blr describing the messages */

		if (procedure->rpr_in_msg) {
			ALLR_release(procedure->rpr_in_msg);
			procedure->rpr_in_msg = NULL;
		}
		if (procedure->rpr_in_format) {
			ALLR_release(procedure->rpr_in_format);
			procedure->rpr_in_format = NULL;
		}
		if (procedure->rpr_out_msg) {
			ALLR_release(procedure->rpr_out_msg);
			procedure->rpr_out_msg = NULL;
		}
		if (procedure->rpr_out_format) {
			ALLR_release(procedure->rpr_out_format);
			procedure->rpr_out_format = NULL;
		}

		REM_MSG message = PARSE_messages(blr, blr_length);
		if (message != (REM_MSG) - 1) {
			while (message) {
				if (message->msg_number == 0) {
					procedure->rpr_in_msg = message;
					procedure->rpr_in_format = (rem_fmt*) message->msg_address;
					message->msg_address = in_msg;
					message = message->msg_next;
					procedure->rpr_in_msg->msg_next = NULL;
				}
				else if (message->msg_number == 1) {
					procedure->rpr_out_msg = message;
					procedure->rpr_out_format = (rem_fmt*) message->msg_address;
					message->msg_address = out_msg;
					message = message->msg_next;
					procedure->rpr_out_msg->msg_next = NULL;
				}
				else {
					REM_MSG temp = message;
					message = message->msg_next;
					ALLR_release(temp);
				}
			}
		}
		/*
		else
			error
		*/

		PACKET* packet = &rdb->rdb_packet;
		packet->p_operation = op_transact;
		P_TRRQ* trrq = &packet->p_trrq;
		trrq->p_trrq_database = rdb->rdb_id;
		trrq->p_trrq_transaction = transaction->rtr_id;
		trrq->p_trrq_blr.cstr_length = blr_length;
		trrq->p_trrq_blr.cstr_address = const_cast<UCHAR*>(blr);
		trrq->p_trrq_messages = (in_msg_length) ? 1 : 0;

		if (!send_packet(rdb->rdb_port, packet, user_status))
			return error(user_status);

		/* Two types of responses are possible, op_transact_response or 
		   op_response.  When there is an error op_response packet is returned
		   and it modifies the status vector to indicate the error which occured.
		   But when success occurs a packet with op_transact_response comes back
		   which does not change the status vector.
		 */

		packet->p_resp.p_resp_status_vector = rdb->rdb_status_vector;
		if (!receive_packet(port, packet, user_status)) {
			return error(user_status);
		}

		if (packet->p_operation != op_transact_response) {
			if (!check_response(rdb, packet)) {
				return error(user_status);
			}
		}
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


ISC_STATUS GDS_TRANSACTION_INFO(ISC_STATUS* user_status,
							RTR* tra_handle,
							SSHORT item_length,
							const UCHAR* items,
							SSHORT buffer_length, UCHAR* buffer)
{
/**************************************
 *
 *	g d s _ t r a n s a c t i o n _ i n f o
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	RTR transaction = *tra_handle;
	CHECK_HANDLE(transaction, type_rtr, isc_bad_trans_handle);
	RDB rdb = transaction->rtr_rdb;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	ISC_STATUS status;
	try
	{
		status =
			info(user_status, rdb, op_info_transaction, transaction->rtr_id, 0,
				 item_length, reinterpret_cast<const SCHAR*>(items), 0, 0,
				 buffer_length, (SCHAR *) buffer);
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	REM_restore_thread_data();

	return status;
}


ISC_STATUS GDS_UNWIND(ISC_STATUS* user_status, rrq** req_handle, USHORT level)
{
/**************************************
 *
 *	g d s _ u n w i n d
 *
 **************************************
 *
 * Functional description
 *	Unwind a running request.
 *
 **************************************/
	trdb thd_context(user_status);
	trdb* tdrdb;

	REM_set_thread_data(tdrdb, &thd_context);

	rrq* request = REMOTE_find_request(*req_handle, level);
	CHECK_HANDLE(request, type_rrq, isc_bad_req_handle);
	RDB rdb = request->rrq_rdb;
	CHECK_HANDLE(rdb, type_rdb, isc_bad_db_handle);
	rdb->rdb_status_vector = user_status;
	tdrdb->trdb_database = rdb;

	try
	{
		// EXE_unwind (*req_handle);
	}
	catch(const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(rdb);
}


static RVNT add_event( rem_port* port)
{
/*************************************
 * 
 * 	a d d _ e v e n t
 *
 **************************************
 *
 * Functional description
 *	Add remote event block to active chain.
 *
 **************************************/
	RDB rdb = port->port_context;

/* Find unused event block or, if necessary, a new one */

	RVNT event;
	for (event = rdb->rdb_events; event; event = event->rvnt_next) {
		if (!event->rvnt_id)
			break;
	}

	if (!event) {
		event = (RVNT) ALLR_block(type_rvnt, 0);
		event->rvnt_next = rdb->rdb_events;
		rdb->rdb_events = event;
	}

	event->rvnt_id = ++remote_event_id;

	return event;
}


static void add_other_params( rem_port* port, UCHAR* dpb_or_spb, USHORT* length)
{
/**************************************
 *
 *	a d d _ o t h e r _ p a r a m s
 *
 **************************************
 *
 * Functional description
 *	Add parameters to a dpb or spb to describe client-side
 *	settings that the server should know about.  
 *	Currently only dummy_packet_interval.
 *	Note: caller must ensure enough spare space is available at the end of 
 *	the passed in dpb or spb.
 *
 **************************************/
	fb_assert(isc_dpb_dummy_packet_interval == isc_spb_dummy_packet_interval);
	fb_assert(isc_dpb_version1 == isc_spb_version1);

	if (port->port_flags & PORT_dummy_pckt_set) {
		if (*length == 0)
			dpb_or_spb[(*length)++] = isc_dpb_version1;
		dpb_or_spb[(*length)++] = isc_dpb_dummy_packet_interval;
		dpb_or_spb[(*length)++] = sizeof(port->port_dummy_packet_interval);
		stuff_vax_integer(&dpb_or_spb[*length],
						  port->port_dummy_packet_interval,
						  sizeof(port->port_dummy_packet_interval));
		*length += sizeof(port->port_dummy_packet_interval);
	}
}


static void add_working_directory(UCHAR*	dpb_or_spb,
								  USHORT*	length,
								  const Firebird::PathName&	node_name)
{
/************************************************
 *
 *      a d d _ w o r k i n g _ d i r e c t o r y
 *
 ************************************************
 *
 * Functional description
 *      Add parameters to a dpb or spb to describe client-side
 *      settings that the server should know about.
 *
 ************************************************/
	Firebird::PathName cwd;

	// for WNet local node_name should be compared with "\\\\." ?
	if (node_name == "localhost")
	{
		fb_getcwd(cwd);
	}
	if (*length == 0) {
		dpb_or_spb[(*length)++] = isc_dpb_version1;
	}
	dpb_or_spb[(*length)++] = isc_dpb_working_directory;
	dpb_or_spb[(*length)++] = cwd.length();
	memcpy(&(dpb_or_spb[(*length)]), cwd.c_str(), cwd.length());
	*length += cwd.length();
}


static rem_port* analyze(Firebird::PathName&	file_name,
					ISC_STATUS*					status_vector,
					const TEXT*					user_string,
					bool						uv_flag,
					const SCHAR*				dpb,
					SSHORT						dpb_length,
					Firebird::PathName&			node_name)
{
/**************************************
 *
 *	a n a l y z e
 *
 **************************************
 *
 * Functional description
 *	Analyze a file specification and determine whether
 *	a remote server is required, and if so, what protocol
 *	to use.  If the database can be accessed locally,
 *	return a NULL connection block.  If a "full context"
 *	server is to be used, return the address of a port block
 *	with which to communicate with the server.
 *
 *	NOTE: The file name must have been expanded prior to this call.
 *
 **************************************/
#if defined(WIN_NT)
	ISC_expand_share(file_name);
#endif

	rem_port* port = NULL;

/* Analyze the file name to see if a remote connection is required.  If not,
   quietly (sic) return. */

#ifdef VMS
	port = DECNET_analyze(file_name.c_str(), file_name.length(), status_vector, uv_flag);
#endif

#if defined(WIN_NT)
	if (ISC_analyze_pclan(file_name, node_name)) {
		return WNET_analyze(file_name, status_vector,
							node_name.c_str(), user_string, uv_flag);
	}
#endif

	if (!port)
	{
		if (ISC_analyze_tcp(file_name, node_name))
		{
			port = INET_analyze(file_name, status_vector,
								node_name.c_str(), user_string, uv_flag, dpb,
								dpb_length);

			if (!port)
			{
				/* retry in case multiclient inet server not forked yet */
				sleep(2);
				port = INET_analyze(file_name, status_vector,
									node_name.c_str(), user_string, uv_flag, dpb,
									dpb_length);
			}
		}
		else
		{
#ifndef NO_NFS
			if (!port)
			{
				if (ISC_analyze_nfs(file_name, node_name))
				{
					port = INET_analyze(file_name, status_vector,
										node_name.c_str(), user_string, uv_flag, dpb,
										dpb_length);
					if (!port)
					{
						/* retry in case multiclient inet server not forked yet */

						sleep(2);
						port =
							INET_analyze(file_name, 
										 status_vector, node_name.c_str(),
										 user_string, uv_flag, dpb,
										 dpb_length);
					}
				}
			}
#endif
		}
	}

#if defined(XNET)

/* all remote attempts have failed, so access locally through the
   interprocess server */

	if (!port)
	{
		return XNET_analyze(file_name, 
							status_vector,
							node_name.c_str(),
							user_string,
							uv_flag);
	}

#endif /* XNET */

#if defined(SUPERCLIENT) && !defined(EMBEDDED)
/* Coerce host connections to loopback to SUPERSERVER. */

#ifdef WIN_NT
	if (!ostype)
	{
		if (ISC_is_WinNT())
		{
			ostype = OSTYPE_NT;
		}
		else
		{
			ostype = OSTYPE_WIN_95;
		}
	}

	if (ostype == OSTYPE_NT && !port)
	{
		file_name.insert(0, "\\\\.\\");
		if (ISC_analyze_pclan(file_name, node_name))
			return WNET_analyze(file_name, status_vector,
								node_name.c_str(), user_string, uv_flag);
	}
#endif /* WIN_NT */

#ifdef UNIX

	if (!port && node_name.isEmpty())
	{
		file_name.insert(0, "localhost:");
		if (ISC_analyze_tcp(file_name, node_name))
		{
			return INET_analyze(file_name, 
								status_vector,
								node_name.c_str(),
								user_string,
								uv_flag,
								dpb,
								dpb_length);
		}
	}

#endif /* UNIX */

#endif /* SUPERCLIENT */

	if (port || status_vector[1])
	{
		return port;
	}

	return NULL;
}


static rem_port* analyze_service(Firebird::PathName& service_name,
							ISC_STATUS* status_vector,
							const TEXT* user_string,
							bool uv_flag,
							const SCHAR* dpb,
							SSHORT dpb_length)
{
/**************************************
 *
 *	a n a l y z e _ s e r v i c e
 *
 **************************************
 *
 * Functional description
 *	Analyze a service specification and determine whether a remote
 *	server is required, and if so, what protocol to use.  If the
 *	database can be accessed locally, return the value FALSE with a
 *	NULL connection block.  If a server is to be used, return TRUE
 *	and the address of a port block with which to communicate
 *	with the server.
 *
 **************************************/
	rem_port* port = NULL;
	Firebird::PathName node_name;

/* Analyze the service name to see if a remote connection is required.  If not,
   quietly (sic) return. */

#ifdef VMS
	port =
		DECNET_analyze(service_name, service_length, status_vector, uv_flag);
#endif

#if defined(WIN_NT)
	if (ISC_analyze_pclan(service_name, node_name)) {
		return WNET_analyze(service_name, status_vector,
							node_name.c_str(), user_string, uv_flag);
	}
#endif
	if (!port) {
		if (ISC_analyze_tcp(service_name, node_name))
			port = INET_analyze(service_name, status_vector,
								node_name.c_str(), user_string, uv_flag, dpb,
								dpb_length);
	}

#if defined(XNET)

/* all remote attempts have failed, so access locally through the
   interprocess server */

	if (!port) {
		port = XNET_analyze(service_name, status_vector,
							node_name.c_str(), user_string, uv_flag);
	}
#endif

#ifdef SUPERCLIENT
#ifdef UNIX

	if (!port && node_name.isEmpty()) {
		service_name.insert(0, "localhost:");
		if (ISC_analyze_tcp(service_name, node_name))
		{
			return INET_analyze(service_name, 
								status_vector,
								node_name.c_str(), 
								user_string,
								uv_flag,
								dpb,
								dpb_length);
		}
	}
#endif /* UNIX */
#endif /* SUPERCLIENT */


	return port;
}


static bool batch_dsql_fetch(trdb*	tdrdb,
							 rem_port*	port,
							 rmtque*	que_inst,
							 ISC_STATUS*	user_status,
							 USHORT		id)
{
/**************************************
 *
 *	b a t c h _ d s q l _ f e t c h
 *
 **************************************
 *
 * Functional description
 *	Receive a batch of messages that were queued
 *	on the wire.
 *
 *	This function will be invoked whenever we need to wait
 *	for something to come over on the wire, and there are
 *	items in the queue for receipt.
 *
 *	Note on error handing:  Actual networking errors
 *	need to be reported to user_status - which is bubbled
 *	upwards to the API call which initiated this receive.
 *	A status vector being returned as part of the cursor
 *	fetch needs to be stored away for later return to the
 *	client in the proper place in the stream.
 *
 **************************************/

	fb_assert(port);
	fb_assert(que_inst);
	fb_assert(user_status);

	fb_assert(que_inst->rmtque_function == batch_dsql_fetch);

	RDB     rdb       = que_inst->rmtque_rdb;
	RSR     statement = static_cast<rsr*>(que_inst->rmtque_parm);
	PACKET* packet    = &rdb->rdb_packet;

	fb_assert(port == rdb->rdb_port);

/* Queue errors within the batched request */

	ISC_STATUS_ARRAY tmp_status;
	ISC_STATUS* save_status = packet->p_resp.p_resp_status_vector;
	packet->p_resp.p_resp_status_vector = tmp_status;

/* Setup the packet structures so it knows what statement we
   are trying to receive at this point in time */

	packet->p_sqldata.p_sqldata_statement = statement->rsr_id;

/* We'll either receive the whole batch, until end-of-batch is seen,
   or we'll just fetch one.  We'll fetch one when we've run out of
   local data to return to the client, so we grab one "hot off the wire"
   to handoff to them.  We'll grab the whole batch when we need to
   receive a response for a DIFFERENT network request on the wire,
   so we have to clear the wire before the response can be received */
/* In addtion to the above we grab all the records in case of XNET as 
 * we need to clear the queue */
	bool clear_queue = false;
	if (id != statement->rsr_id || port->port_type == port_xnet) {
		clear_queue = true;
	}

	statement->rsr_flags |= RSR_fetched;
	while (true)
	{
		/* Swallow up data. If a buffer isn't available, allocate another. */

		REM_MSG message = statement->rsr_buffer;
		if (message->msg_address)
		{
			REM_MSG new_msg = (REM_MSG) ALLR_block(type_msg, statement->rsr_fmt_length);
			statement->rsr_buffer = new_msg;
				
			new_msg->msg_next = message;

#ifdef SCROLLABLE_CURSORS
			/* link the new message in a doubly linked list to make it 
			   easier to scroll back and forth through the records */

			REM_MSG prior = message->msg_prior;
			message->msg_prior = new_msg;
			prior->msg_next = new_msg;
			new_msg->msg_prior = prior;
#else
			while (message->msg_next != new_msg->msg_next) {
				message = message->msg_next;
			}
			message->msg_next = new_msg;
#endif
		}

		if (!receive_packet_noqueue(port, packet, tmp_status)) {
			/* Must be a network error */

			memcpy(user_status, tmp_status, sizeof(tmp_status));
			packet->p_resp.p_resp_status_vector = save_status;
			statement->rsr_rows_pending = 0;
			--statement->rsr_batch_count;
			dequeue_receive(port);
			Firebird::status_exception::raise(user_status);
		}

		if (packet->p_operation != op_fetch_response) {
			statement->rsr_flags |= RSR_stream_err;
			check_response(rdb, packet);

			/* save the status vector in a safe place */

			if (!statement->rsr_status_vector[1])
				memcpy(statement->rsr_status_vector, tmp_status,
					   sizeof(statement->rsr_status_vector));
			statement->rsr_rows_pending = 0;
			--statement->rsr_batch_count;
			dequeue_receive(port);
			break;
		}

		/* See if we're at end of the batch */

		if (packet->p_sqldata.p_sqldata_status ||
			!packet->p_sqldata.p_sqldata_messages ||
			(port->port_flags & PORT_rpc))
		{
			if (packet->p_sqldata.p_sqldata_status == 100)
			{
				statement->rsr_flags |= RSR_eof;
				statement->rsr_rows_pending = 0;
#ifdef DEBUG
				fprintf(stdout,
						   "Resetting Rows Pending in batch_dsql_fetch=%lu\n",
						   statement->rsr_rows_pending);
#endif
			}
			--statement->rsr_batch_count;
			if (statement->rsr_batch_count == 0) {
				statement->rsr_rows_pending = 0;
			}
			dequeue_receive(port);
			break;
		}
		statement->rsr_msgs_waiting++;
		statement->rsr_rows_pending--;
#ifdef DEBUG
		fprintf(stdout,
				   "Decrementing Rows Pending in batch_dsql_fetch=%lu\n",
				   statement->rsr_rows_pending);
#endif
		if (clear_queue == false) {
			break;
		}
	}
	packet->p_resp.p_resp_status_vector = save_status;
	return true;
}


static bool batch_gds_receive(trdb*		tdrdb,
							  rem_port*		port,
							  rmtque*	que_inst,
							  ISC_STATUS*	user_status,
							  USHORT		id)
{
/**************************************
 *
 *	b a t c h _ g d s _ r e c e i v e
 *
 **************************************
 *
 * Functional description
 *	Receive a batch of messages that were queued
 *	on the wire.
 *
 *	This function will be invoked whenever we need to wait
 *	for something to come over on the wire, and there are
 *	items in the queue for receipt.
 *
 *	Note on error handing:  Actual networking errors
 *	need to be reported to user_status - which is bubbled
 *	upwards to the API call which initiated this receive.
 *	A status vector being returned as part of the cursor
 *	fetch needs to be stored away for later return to the
 *	client in the proper place in the stream.
 *
 **************************************/

	fb_assert(port);
	fb_assert(que_inst);
	fb_assert(user_status);
	fb_assert(que_inst->rmtque_function == batch_gds_receive);

	RDB rdb = que_inst->rmtque_rdb;
	rrq* request = static_cast<rrq*>(que_inst->rmtque_parm);
	rrq::rrq_repeat* tail = que_inst->rmtque_message;
	PACKET *packet = &rdb->rdb_packet;

	fb_assert(port == rdb->rdb_port);
	
	// Queue errors within the batched request

	ISC_STATUS_ARRAY tmp_status;
	ISC_STATUS* save_status = packet->p_resp.p_resp_status_vector;
	packet->p_resp.p_resp_status_vector = tmp_status;

	bool clear_queue = false;
	// indicates whether queue is just being emptied, not retrieved

	// always clear the complete queue for XNET, as we might
	// have incomplete packets
	if (id != request->rrq_id || port->port_type == port_xnet) {
		clear_queue = true;
	}

	// Receive the whole batch of records, until end-of-batch is seen

	while (true)
	{
		REM_MSG message = tail->rrq_xdr;	/* First free buffer */

		/* If the buffer queue is full, allocate a new message and 
		   place it in the queue--if we are clearing the queue, don't 
		   read records into messages linked list so that we don't 
		   mess up the record cache for scrolling purposes. */

		if (message->msg_address)
		{
			const rem_fmt* format = tail->rrq_format;
			REM_MSG new_msg = (REM_MSG) ALLR_block(type_msg, format->fmt_length);
			tail->rrq_xdr = new_msg;
			new_msg->msg_next = message;
			new_msg->msg_number = message->msg_number;

#ifdef SCROLLABLE_CURSORS
			/* link the new message in a doubly linked list to make it 
			   easier to scroll back and forth through the records */

			REM_MSG prior = message->msg_prior;
			message->msg_prior = new_msg;
			prior->msg_next = new_msg;
			new_msg->msg_prior = prior;
#else
			/* Walk the que until we find the predecessor of message */

			while (message->msg_next != new_msg->msg_next) {
				message = message->msg_next;
			}
			message->msg_next = new_msg;
#endif
		}

		/* Note: not receive_packet */

		if (!receive_packet_noqueue(rdb->rdb_port, packet, tmp_status))
		{
			/* Must be a network error */

			memcpy(user_status, tmp_status, sizeof(tmp_status));
			packet->p_resp.p_resp_status_vector = save_status;
			tail->rrq_rows_pending = 0;
			--tail->rrq_batch_count;
			dequeue_receive(port);
			Firebird::status_exception::raise(user_status);
		}

		if (packet->p_operation != op_send) {
			tail->rrq_rows_pending = 0;
			--tail->rrq_batch_count;
			check_response(rdb, packet);
#ifdef DEBUG
			fprintf(stderr, "End of batch. rows pending = %d\n",
					   tail->rrq_rows_pending);
			fprintf(stderr, "Got batch error %ld Max message = %d\n",
					   tmp_status[1], request->rrq_max_msg);
#endif
			if (!request->rrq_status_vector[1]) {
				memcpy(request->rrq_status_vector, tmp_status,
					   sizeof(tmp_status));
			}
			dequeue_receive(port);
			break;
		}

#ifdef SCROLLABLE_CURSORS
		/* at this point we've received a row into the message, so mark the message 
		   with the absolute offset */
		const bool bIsBackward    = (tail->rrq_flags & RRQ_backward) != 0;
		const bool bIsAbsBackward = (tail->rrq_flags & RRQ_absolute_backward) != 0;

		if (bIsBackward == bIsAbsBackward) {
				tail->rrq_absolute++;
		}
		else {
			tail->rrq_absolute--;
		}
		message->msg_absolute = tail->rrq_absolute;
#endif

		tail->rrq_msgs_waiting++;
		tail->rrq_rows_pending--;
#ifdef DEBUG
		fprintf(stdout,
				   "Decrementing Rows Pending in batch_gds_receive=%d\n",
				   tail->rrq_rows_pending);
#endif

		/* See if we're at end of the batch */

		if (!packet->p_data.p_data_messages || (port->port_flags & PORT_rpc)) {
			if (!(--tail->rrq_batch_count))
				tail->rrq_rows_pending = 0;
#ifdef DEBUG
			fprintf(stderr, "End of batch waiting %d\n",
					   tail->rrq_rows_pending);
#endif
			dequeue_receive(port);
			break;
		}

		/* one packet is enough unless we are trying to clear the queue */

		if (!clear_queue)
			break;
#ifdef SCROLLABLE_CURSORS
		else {
			/* if we are just trying to clear the queue, then NULL out the message 
			   address so we don't get a record out of order--it would mess up 
			   scrolling through the cache */

			message->msg_address = NULL;
		}
#endif
	}

	packet->p_resp.p_resp_status_vector = save_status;
	return true;
}


static bool check_response(RDB rdb,
						   PACKET * packet)
{
/**************************************
 *
 *	c h e c k _ r e s p o n s e
 *
 **************************************
 *
 * Functional description
 *	Check response to a remote call.
 *
 **************************************/
	rem_port* port = rdb->rdb_port;
	ISC_STATUS* vector = packet->p_resp.p_resp_status_vector;

/* Translate any gds codes into local operating specific codes */

	while (*vector != isc_arg_end) {
		const ISC_STATUS vec = *vector++;
		switch ((USHORT) vec) {
		case isc_arg_warning:
		case isc_arg_gds:
			if (port->port_protocol < PROTOCOL_VERSION10) {
				fb_assert(vec == isc_arg_gds);
				*vector = gds__encode(*vector, 0);
			}
			else
				*vector = *vector;
			vector++;
			break;

		case isc_arg_cstring:
			vector += 2;
			break;

		default:
			vector++;
			break;
		}
	}

	if ((packet->p_operation == op_response ||
		 packet->p_operation == op_response_piggyback) &&
		!rdb->rdb_status_vector[1])
	{
		return true;
	}

	return false;
}


static bool clear_queue(rem_port* port,
						ISC_STATUS * user_status)
{
/**************************************
 *
 *	c l e a r _ q u e u e
 *
 **************************************
 *
 * Functional description
 *	Clear the queue of batched packets - in preparation
 *	for waiting for a specific response, or when we are
 *	about to reuse an internal request.
 * Return codes:
 *	TRUE  - no errors.
 *	FALSE - Network error occured, error code in user_status 
 **************************************/

	if (port->port_receive_rmtque) {
		trdb* tdrdb = REM_get_thread_data();
		while (port->port_receive_rmtque)
			if (!receive_queued_packet(tdrdb, port, user_status, (USHORT) - 1))
				return false;
	}

	return true;
}


static void disconnect( rem_port* port)
{
/**************************************
 *
 *	d i s c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Disconnect a port and free its memory.
 *
 **************************************/

/* Send a disconnect to the server so that it
   gracefully terminates. */

	RDB rdb = port->port_context;
	if (rdb) {
		/* BAND-AID:
		   It seems as if we are disconnecting the port
		   on both the server and client side.  For now
		   let the server handle this for named pipes

		   8-Aug-1997  M.  Duquette
		   R.  Kumar
		   M.  Romanini

		 */
		/* Win95 SPX does not like the server closing the connection, just as the client
		   is attempting to send data. Such a scenario is caused by both sides trying to 
		   perform a disconnect. So, disable client side disconnect for spx. Though, 
		   this is really  not a NT issue, let me leave the same behaviour for NT & 95 
		   RaviKumar Jan 3, 98
		 */

		PACKET* packet = &rdb->rdb_packet;
		if (port->port_type != port_pipe) {
			packet->p_operation = op_disconnect;
			port->send(packet);
		}
		REMOTE_free_packet(port, packet);
	}

/* Perform physical network disconnect and release
   memory for remote database context. */

	port->disconnect();
	if (rdb) {
		ALLR_release(rdb);
	}
}


#ifdef SCROLLABLE_CURSORS
static REM_MSG dump_cache(
					  rem_port* port, ISC_STATUS * user_status, rrq::rrq_repeat * tail)
{
/**************************************
 *
 *	d u m p _ c a c h e
 *
 **************************************
 *
 * Functional description
 *	We have encountered a situation where what's in 
 *	cache is not useful, so clear any pending requests 
 *	and empty the cache in preparation for refilling it. 
 *
 **************************************/
	if (!clear_queue(port, user_status))
		return NULL;

	REM_MSG message = tail->rrq_message;
	while (true) {
		message->msg_address = NULL;
		message = message->msg_next;
		if (message == tail->rrq_message)
			break;
	}

	tail->rrq_xdr = message;
	tail->rrq_last = NULL;
	tail->rrq_rows_pending = 0;

	return message;
}
#endif


static ISC_STATUS error( ISC_STATUS * user_status)
{
/**************************************
 *
 *	e r r o r
 *
 **************************************
 *
 * Functional description
 *	An error returned has been trapped.  Return a status code.
 *
 **************************************/

	REM_restore_thread_data();

	return user_status[1];
}

static ISC_STATUS error(ISC_STATUS* user_status, const std::exception& ex)
{
/**************************************
 *
 *	e r r o r
 *
 **************************************
 *
 * Functional description
 *	An error returned has been trapped.  Return a status code.
 *
 **************************************/

	REM_restore_thread_data();
	
	return Firebird::stuff_exception(user_status, ex);
}


#ifndef MULTI_THREAD
static void event_handler( rem_port* port)
{
/**************************************
 *
 *	e v e n t _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Wait on auxiliary mailbox for event notification.
 *	If we are single threaded, run this routine, blocking until
 *	an event message arrives, and handling accordingly.
 *
 **************************************/
/* zero packet */
	PACKET packet;
	zap_packet(&packet);

/* Read what should be an event message. If it's not, return. */

	if (port->receive(&packet)) {
		/* If we received an event packet */

		if (packet.p_operation == op_event) {
			/* Find the event, if any, that matches the packet's event */
			P_EVENT* pevent = &packet.p_event;
			RVNT event = find_event(port, pevent->p_event_rid);
			if (event) {
				/* Call the asynchronous trap function associated with the event. */

				(*event->rvnt_ast) (event->rvnt_arg,
									pevent->p_event_items.cstr_length,
									pevent->p_event_items.cstr_address);

				event->rvnt_id = 0;
			}
		}
	}

/* free up anything allocated */

	REMOTE_free_packet(port, &packet);
}


#else /* MULTI_THREAD  */
static THREAD_ENTRY_DECLARE event_thread(THREAD_ENTRY_PARAM arg)
{
/**************************************
 *
 *	e v e n t _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *	Wait on auxilary mailbox for event notification.
 *
 **************************************/
	rem_port* port = (rem_port*)arg;
	PACKET packet;

	for (;;) {
		/* zero packet */

		zap_packet(&packet);

		/* read what should be an event message */

		THREAD_ENTER();
		rem_port* stuff = port->receive(&packet);
		THREAD_EXIT();

		const P_OP operation = packet.p_operation;

		if (!stuff || operation == op_exit || operation == op_disconnect) {
			/* Actually, the remote server doing the watching died.
			   Clean up and leave. */

			REMOTE_free_packet(port, &packet);
			server_death(port);
			break;
		}

		/* If the packet was an event, we handle it */

		if (operation == op_event) {
			P_EVENT* pevent = &packet.p_event;

			THREAD_ENTER();
			RVNT event = find_event(port, pevent->p_event_rid);
			THREAD_EXIT();

			if (event) {
				/* Call the asynchronous event routine associated
				   with this event */

				// CVC: Will try to review this function signature later.
				(*event->rvnt_ast) (event->rvnt_arg,
									pevent->p_event_items.cstr_length,
									pevent->p_event_items.cstr_address);

				event->rvnt_id = 0;
			}

		}						/* end of event handling for op_event */

		REMOTE_free_packet(port, &packet);
	}							/* end of infinite for loop */
	// to make compilers happy
	return 0;
}
#endif /* MULTI_THREAD  */


static ISC_STATUS fetch_blob(
						 ISC_STATUS* user_status,
						 RSR statement,
						 USHORT blr_length,
						 const UCHAR* blr,
						 USHORT msg_type,
						 USHORT msg_length,
						 UCHAR* msg)
{
/**************************************
 *
 *	f e t c h _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Fetch next record from a dynamic SQL cursor.
 *
 **************************************/
	RDB rdb = statement->rsr_rdb;

	rem_port* port = rdb->rdb_port;
	PACKET* packet = &rdb->rdb_packet;
	packet->p_operation = op_fetch;
	P_SQLDATA* sqldata = &packet->p_sqldata;
	sqldata->p_sqldata_statement = statement->rsr_id;
	sqldata->p_sqldata_blr.cstr_length = blr_length;
	sqldata->p_sqldata_blr.cstr_address = const_cast<UCHAR*>(blr);
	sqldata->p_sqldata_message_number = msg_type;
	sqldata->p_sqldata_messages = (statement->rsr_select_format) ? 1 : 0;

	if (!send_packet(port, packet, user_status))
		return user_status[1];

/* set up the response packet. */

	packet->p_resp.p_resp_status_vector = rdb->rdb_status_vector;

/* Swallow up data. */

	REM_MSG message = statement->rsr_buffer;
	message->msg_address = msg;
	if (!receive_packet(port, packet, user_status)) {
		message->msg_address = NULL;
		return user_status[1];
	}
	message->msg_address = NULL;

	if (packet->p_operation == op_fetch_response)
		receive_response(rdb, packet);
	else {
		check_response(rdb, packet);
		return user_status[1];
	}

	return packet->p_sqldata.p_sqldata_status;
}


static RVNT find_event( rem_port* port, SLONG id)
{
/*************************************
 * 
 * 	f i n d _ e v e n t
 *
 **************************************
 *
 * Functional description
 *	Find event with specified event_id.
 *
 **************************************/
	RDB rdb = port->port_context;

	for (RVNT event = rdb->rdb_events; event; event = event->rvnt_next) {
		if (event->rvnt_id == id)
			return event;
	}

	return NULL;
}


static bool get_new_dpb(const UCHAR*	dpb,
						  SSHORT	dpb_length,
						  bool		dpb_vs_spb,
						  UCHAR*	new_dpb,
						  USHORT*	new_dpb_length,
						  TEXT*		user_string)
{
/**************************************
 *
 *	g e t _ n e w _ d p b
 *
 **************************************
 *
 * Functional description
 *	Fetch user_string out of dpb.
 *	(Based on JRD get_options())
 *
 **************************************/
	UCHAR	pw_buffer[MAX_PASSWORD_ENC_LENGTH + 6];

	*user_string = 0;
	*new_dpb_length = 0;

	UCHAR	pb_version;
	UCHAR	pb_sys_user_name;
	UCHAR	pb_password;
	UCHAR	pb_user_name;
	UCHAR	pb_password_enc;

	if (dpb_vs_spb)
	{
		pb_version = isc_dpb_version1;
		pb_sys_user_name = isc_dpb_sys_user_name;
		pb_password = isc_dpb_password;
		pb_user_name = isc_dpb_user_name;
		pb_password_enc = isc_dpb_password_enc;
	}
	else
	{
		if (dpb_length)
		{
			if (*dpb == isc_spb_version) {
				pb_version = dpb[1];
			}
			else {
				pb_version = *dpb;
			}
		}
		else
		{
			pb_version = isc_spb_current_version;
		}

		pb_sys_user_name = isc_spb_sys_user_name;
		pb_password = isc_spb_password;
		pb_user_name = isc_spb_user_name;
		pb_password_enc = isc_spb_password_enc;
	}
	const UCHAR* p = dpb;
	UCHAR* s = new_dpb;
	const UCHAR* const end_dpb = p + dpb_length;

	if ((dpb_length > 0) && (*p != pb_version))
	{
		if (dpb_vs_spb) {
			gds__log("REMOTE INTERFACE: wrong dpb version", 0);
		}
		else {
			gds__log("REMOTE INTERFACE: wrong spb version", 0);
		}
	}

	if (dpb_length == 0)
	{
		*s++ = pb_version;
	}
	else
	{
		/* for all spb_versions > 1 (meaning usc_spb_version was specified)
		 * the actual version of the spb is stored in the second byte so
		 * move the first byte (isc_spb_version) into the new spb so that
		 * it can be saved off
		 */
		if (*p == isc_spb_version) {
			*s++ = *p++;
		}
		*s++ = *p++;
	}

	bool result = false;
	SSHORT password_length = 0;
	const UCHAR* password = 0;
	bool moved_some = false;
	while (p < end_dpb)
	{
		const UCHAR c = *p++;
		*s++ = c;
		if (c == pb_sys_user_name)
		{
			s--;
			UCHAR* q = (UCHAR *) user_string;
			SSHORT l = *p++;
			if (l)
			{
				do {
					*q++ = *p++;
				} while (--l);
			}
			*q = 0;
		}
		else if (c == pb_password)
		{
			moved_some = true;
			s--;
			password_length = *p++;
			password = p;
			p += password_length;
		}
		else
		{
			if (c == pb_user_name) {
				result = true;
			}
			moved_some = true;
			SSHORT l = *p++;
			if (*s++ = static_cast<UCHAR>(l))
			{
				do {
					*s++ = *p++;
				} while (--l);
			}

		}
	}

#ifdef NO_PASSWORD_ENCRYPTION
	if (password)
	{
		moved_some = true;
		*s++ = pb_password;
		*s++ = password_length;
		do {
			*s++ = *password++;
		} while (--password_length);
	}
#else
	if (password)
	{
		moved_some = true;
		*s++ = pb_password_enc;
		const SSHORT l = MIN(password_length, MAX_PASSWORD_ENC_LENGTH);
		strncpy((char*) pw_buffer, (const char*) password, l);
		pw_buffer[l] = 0;
		TEXT pwt[MAX_PASSWORD_LENGTH + 2];
		ENC_crypt(pwt, sizeof pwt, reinterpret_cast<char*>(pw_buffer), PASSWORD_SALT);
		const char* pp = pwt + 2;
		*s++ = strlen(pp);
		while (*pp) {
			*s++ = *pp++;
		}
	}
#endif

	if (moved_some || ((s - new_dpb) > 1)) {
		*new_dpb_length = s - new_dpb;
	}
	else {
		*new_dpb_length = 0;
	}

	return result;
}

#ifdef UNIX
static bool get_single_user(USHORT dpb_length,
							const SCHAR* dpb)
{
/******************************************
 *
 *	g e t _ s i n g l e _ u s e r
 *
 ******************************************
 *
 * Functional description
 *	Get the dpb and return true if the
 *	dpb_single_user flag is set, false
 *	otherwise.
 *
 ******************************************/
	if (!dpb)
		return false;

	const SCHAR* const end_dpb = dpb + dpb_length;

	if (dpb < end_dpb && *dpb++ != isc_dpb_version1)
		return false;

	USHORT l;
	while (dpb < end_dpb)
		switch (*dpb++) {
		case isc_dpb_reserved:
			l = *dpb++;
			if (l == 3 && !strncmp(dpb, "YES", 3))
				return true;
			return false;

		default:
			l = *dpb++;
			dpb += l;
		}

	return false;
}
#endif

static ISC_STATUS handle_error( ISC_STATUS * user_status, ISC_STATUS code)
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

	REM_restore_thread_data();

	*user_status++ = isc_arg_gds;
	*user_status++ = code;
	*user_status = isc_arg_end;

	return code;
}


static ISC_STATUS info(
				   ISC_STATUS* user_status,
				   RDB rdb,
				   P_OP operation,
				   USHORT object,
				   USHORT incarnation,
				   USHORT item_length,
				   const SCHAR* items,
				   USHORT recv_item_length,
				   const SCHAR* recv_items,
				   USHORT buffer_length,
				   SCHAR* buffer)
{
/**************************************
 *
 *	i n f o
 *
 **************************************
 *
 * Functional description
 *	Solicit and receive information.
 *
 **************************************/

/* Build the primary packet to get the operation started. */

	PACKET* packet = &rdb->rdb_packet;
	packet->p_operation = operation;
	P_INFO* information = &packet->p_info;
	information->p_info_object = object;
	information->p_info_incarnation = incarnation;
	information->p_info_items.cstr_length = item_length;
	information->p_info_items.cstr_address = (UCHAR *) items;
	if (operation == op_service_info) {
		information->p_info_recv_items.cstr_length = recv_item_length;
		information->p_info_recv_items.cstr_address = (UCHAR *) recv_items;
	}
	information->p_info_buffer_length = buffer_length;

/* Assume the result will be successful */

	fb_assert(user_status == rdb->rdb_status_vector);
	user_status[0] = isc_arg_gds;
	user_status[1] = FB_SUCCESS;
	user_status[2] = isc_arg_end;

	if (!send_packet(rdb->rdb_port, packet, user_status))
		return user_status[1];

/* Set up for the response packet. */

	P_RESP* response = &packet->p_resp;
	CSTRING temp = response->p_resp_data;
	response->p_resp_data.cstr_allocated = buffer_length;
	response->p_resp_data.cstr_address = (UCHAR *) buffer;

	if (!receive_response(rdb, packet)) {
		response->p_resp_data = temp;
		return user_status[1];
	}

	response->p_resp_data = temp;

	return rdb->rdb_status_vector[1];
}


static bool init(ISC_STATUS* user_status,
				 rem_port* port,
				 P_OP op,
				 const Firebird::PathName& file_name,
				 UCHAR* dpb,
				 USHORT dpb_length)
{
/**************************************
 *
 *	i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize for database access.  First call from both CREATE and
 *	OPEN.
 *
 **************************************/
	RDB rdb = port->port_context;
	PACKET* packet = &rdb->rdb_packet;

/* Make attach packet */

	P_ATCH* attach = &packet->p_atch;
	packet->p_operation = op;
	attach->p_atch_file.cstr_length = file_name.length();
	attach->p_atch_file.cstr_address = (UCHAR *) file_name.c_str();
	attach->p_atch_dpb.cstr_length = dpb_length;
	attach->p_atch_dpb.cstr_address = dpb;

	if (!send_packet(rdb->rdb_port, packet, user_status)) {
		disconnect(port);
		return false;
	}

/* Get response */

	if (!receive_response(rdb, packet)) {
		REMOTE_save_status_strings(user_status);
		disconnect(port);
		return false;
	}

	rdb->rdb_id = packet->p_resp.p_resp_object;

	return true;
}


static RTR make_transaction( RDB rdb, USHORT id)
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
	RTR transaction = (RTR) ALLR_block(type_rtr, 0);
	transaction->rtr_rdb = rdb;
	transaction->rtr_id = id;
	transaction->rtr_next = rdb->rdb_transactions;
	rdb->rdb_transactions = transaction;
	SET_OBJECT(rdb, transaction, id);

	return transaction;
}


static ISC_STATUS mov_dsql_message(const UCHAR*	from_msg,
								const rem_fmt*		from_fmt,
								UCHAR*	to_msg,
								const rem_fmt*		to_fmt)
{
/**************************************
 *
 *	m o v _ d s q l _ m e s s a g e
 *
 **************************************
 *
 * Functional description
 *	Move data using formats.
 *
 **************************************/

/* Set up in case we get a conversion error.
   NOTE: The code below is not amenable to multi-threading. */

	TRDB tdrdb = REM_get_thread_data();

	try {

		if (!from_fmt || !to_fmt || from_fmt->fmt_count != to_fmt->fmt_count) {
			move_error(isc_dsql_sqlda_err, isc_arg_end);
			/* Msg 263 SQLDA missing or wrong number of variables */
		}

		const dsc* from_desc = from_fmt->fmt_desc;
		const dsc* to_desc = to_fmt->fmt_desc;
		const dsc* const end_desc = to_desc + to_fmt->fmt_count;
		for (; to_desc < end_desc; from_desc++, to_desc++) {
			dsc from = *from_desc;
			dsc to = *to_desc;
			// Safe const cast, we are going to move from it to anywhere.
			from.dsc_address = const_cast<UCHAR*>(from_msg) + (IPTR) from.dsc_address;
			to.dsc_address = to_msg + (IPTR) to.dsc_address;
			CVT_move(&from, &to, move_error);
		}

	}	// try
	catch (const std::exception& ex) {
		Firebird::stuff_exception(tdrdb->trdb_status_vector, ex);
		return FB_FAILURE;
	}

	return FB_SUCCESS;
}


static void mov_faster( const SLONG* from, SLONG* to, USHORT length)
{
/**************************************
 *
 *	m o v _ f a s t e r
 *
 **************************************
 *
 * Functional description
 *	Do a long move already aligned as quickly as possible.
 *
 **************************************/
	USHORT l;

	if (l = length >> 5) {
		do {
			*to++ = *from++;
			*to++ = *from++;
			*to++ = *from++;
			*to++ = *from++;
			*to++ = *from++;
			*to++ = *from++;
			*to++ = *from++;
			*to++ = *from++;
		} while (--l);
		length &= 31;
	}

	if (l = length >> 2)
		do {
			*to++ = *from++;
		} while (--l);

	if (l = length & 3) {
		UCHAR* p = (UCHAR *) to;
		const UCHAR* q = (UCHAR *) from;
		do {
			*p++ = *q++;
		} while (--l);
	}
}


static void move_error( ISC_STATUS status, ...)
{
/**************************************
 *
 *	m o v e _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	A conversion error occurred.  Complain.
 *
 **************************************/
	va_list ap;
	ISC_STATUS *p_args, *end_args;

/* copy into an array any other arguments which may 
   have been handed to us, then post the error.
   N.B., one of the supplied errors should be a 'isc_arg_end' */

	va_start(ap, status);

	TRDB tdrdb = REM_get_thread_data();
	p_args = tdrdb->trdb_status_vector;
	end_args = p_args + ISC_STATUS_LENGTH;

	*p_args++ = isc_arg_gds;
	*p_args++ = isc_random;
	*p_args++ = isc_arg_string;
	*p_args++ = (ISC_STATUS) "Dynamic SQL Error";
	*p_args++ = isc_arg_gds;
	*p_args++ = isc_sqlerr;
	*p_args++ = isc_arg_number;
	*p_args++ = -303;
	*p_args++ = isc_arg_gds;
	*p_args++ = status;

/* NOTE: This loop could potentially set up a bad status vector */

	while ((*p_args++ = (ISC_STATUS) va_arg(ap, ISC_STATUS)) && p_args < end_args);
	
	if (p_args >= end_args)
		end_args[-1] = isc_arg_end;
		
	va_end(ap);

	Firebird::status_exception::raise(tdrdb->trdb_status_vector);
}


static void receive_after_start( rrq* request, USHORT msg_type)
{
/*****************************************
 *
 *	r e c e i v e _ a f t e r _ s t a r t
 *
 *****************************************
 *
 * Functional Description
 *	Some opcodes, such as "start_and_send" automatically start the
 *	cursor being started, under protcol 8 we then receive the first
 *	batch of records without having to ask for them.
 *
 *	Note: if a network error occurs during this receive, we do not
 *	recognize it in the "gds_start" API call that initiated this
 *	action.  It will be stored with the queue of records for the
 *	cursor that is being fetched.  This is not ideal - but compabile
 *	with how the code worked prior to pipelining work done
 *	1996-Jul-15 David Schnepper 
 *
 *****************************************/

/* Check to see if any data is waiting to happen */

	RDB rdb = request->rrq_rdb;
	rem_port* port = rdb->rdb_port;
	PACKET* packet = &rdb->rdb_packet;
	rrq::rrq_repeat* tail = &request->rrq_rpt[msg_type];
	// CVC: I commented this line because it's overwritten immediately in the loop.
	// REM_MSG message = tail->rrq_message;
	const rem_fmt* format = tail->rrq_format;

/* save the status vector in the request block, as the API call
   which started this function already has a status (the result of
   the isc_start or isc_start_and_receive) */
	ISC_STATUS_ARRAY tmp_status;
	packet->p_resp.p_resp_status_vector = tmp_status;

/* Swallow up data.  If a buffer isn't available, allocate another */

	while (true) {
		REM_MSG message = tail->rrq_xdr;
		if (message->msg_address) {
			REM_MSG new_msg = (REM_MSG) ALLR_block(type_msg, format->fmt_length);
			tail->rrq_xdr = new_msg;
			new_msg->msg_next = message;
			new_msg->msg_number = message->msg_number;

#ifdef SCROLLABLE_CURSORS
			/* link the new message in a doubly linked list to make it 
			   easier to scroll back and forth through the records */

			REM_MSG prior = message->msg_prior;
			message->msg_prior = new_msg;
			prior->msg_next = new_msg;
			new_msg->msg_prior = prior;
#else
			while (message->msg_next != new_msg->msg_next)
				message = message->msg_next;
			message->msg_next = new_msg;
#endif
		}

		/* Note: not receive_packet */
		if (!receive_packet_noqueue(rdb->rdb_port, packet, tmp_status)) {
			memcpy(request->rrq_status_vector, tmp_status,
				   sizeof(request->rrq_status_vector));
			return;
		}

		/* Did an error response come back ? */
		if (packet->p_operation != op_send) {
			check_response(rdb, packet);
			memcpy(request->rrq_status_vector, tmp_status,
				   sizeof(request->rrq_status_vector));
			return;
		}

		tail->rrq_msgs_waiting++;

		/* Reached end of batch */

		if (!packet->p_data.p_data_messages || (port->port_flags & PORT_rpc)) {
			break;
		}
	}
}


static bool receive_packet(rem_port* port,
						   PACKET * packet,
						   ISC_STATUS * user_status)
{
/**************************************
 *
 *	r e c e i v e _ p a c k e t
 *
 **************************************
 *
 * Functional description
 *	Clear the queue of any pending receives, then receive the
 *	response to a sent request, blocking if necessary until
 *	the response is present.
 *
 * Return codes:
 *	TRUE  - no errors.
 *	FALSE - Network error occured, error code in user_status 
 *
 **************************************/

/* Must clear the wire of any queued receives before fetching
   the desired packet */

	if (!clear_queue(port, user_status))
		return false;

	return receive_packet_noqueue(port, packet, user_status);
}


static bool receive_packet_noqueue(rem_port* port,
								   PACKET * packet,
								   ISC_STATUS * user_status)
{
/**************************************
 *
 *	r e c e i v e _ p a c k e t _ n o q u e u e
 *
 **************************************
 *
 * Functional description
 *	Receive a packet and check for a network
 *	error on the receive.  
 *	Note: SOME of the network lower level protocols
 *	will set up a status vector when errors
 *	occur, but other ones won't.
 *	So this routine sets up an error result
 *	for the vector prior to going into the
 *	network layer.  Note that we can't
 *	RESET the status vector as one thing
 *	that can be received is a new status vector
 *
 *	See also cousin routine: send_packet, send_partial_packet
 *
 *	NOTE:  Error handling, specifically the difference between
 *	user_status, rdb_status_vector, and p_resp_status_vector
 *	is very hazy, muddled, confused, and much too repeatitive in
 *	most cases.  A prime candidate for fixing up.
 *	Basically, the reason receive_packet must have a status
 *	vector passed in is that there ARE some cases where we will
 *	receive multiple packets for a response (for instance in
 *	batch mode of gds_receive).  So we throw away
 *	intermediate status vectors.
 *
 * Return codes:
 *	TRUE  - no errors.
 *	FALSE - Network error occured, error code in user_status 
 *
 **************************************/

	user_status[0] = isc_arg_gds;
	user_status[1] = isc_net_read_err;
	user_status[2] = isc_arg_end;

	return (port->receive(packet));
}


static bool receive_queued_packet(trdb*	tdrdb,
								  rem_port*		port,
								  ISC_STATUS*	user_status,
								  USHORT id)
{
/**************************************
 *
 *	r e c e i v e _ q u e u e d_ p a c k e t
 *
 **************************************
 *
 * Functional description
 *	We're marked as having pending receives on the
 *	wire.  Grab the first pending receive and return.
 * Return codes:
 *	TRUE  - no errors.
 *	FALSE - Network error occured, error code in user_status 
 *
 **************************************/
/* Trivial case, nothing pending on the wire */

	if (!port->port_receive_rmtque)
		return true;

/* Grab first queue entry */

	RMTQUE que_inst = port->port_receive_rmtque;

/* Receive the data */

	bool result = (que_inst->rmtque_function) (tdrdb, port, que_inst, user_status, id);

/* Note: it is the rmtque_function's responsibility to dequeue the request */

	return result;
}


static void enqueue_receive(rem_port* port,
							t_rmtque_fn fn,
							RDB rdb,
							void* parm,
							rrq::rrq_repeat* parm1)
{
/**************************************
 *
 *	e n q u e u e _ r e c e i v e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	RMTQUE que_inst = (RMTQUE) ALLR_block(type_rmtque, 0);

/* Prepare a queue entry */

	que_inst->rmtque_next = NULL;
	que_inst->rmtque_function = fn;
	que_inst->rmtque_parm = parm;
	que_inst->rmtque_message = parm1;
	que_inst->rmtque_rdb = rdb;

/* Walk to the end of the current queue */
	RMTQUE* queptr;
	for (queptr = &port->port_receive_rmtque;
		 *queptr; queptr = &(*queptr)->rmtque_next)
		/* do nothing */ ;

/* Add the new entry to the end of the queue */

	*queptr = que_inst;
}


static void dequeue_receive( rem_port* port)
{
/**************************************
 *
 *	d e q u e u e _ r e c e i v e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

/* Grab first queue entry & de-queue it*/

	RMTQUE que_inst = port->port_receive_rmtque;
	port->port_receive_rmtque = que_inst->rmtque_next;
	que_inst->rmtque_next = NULL;

/* Add queue entry onto free queue */

	ALLR_free(que_inst);
}


static bool receive_response(RDB rdb,
							 PACKET * packet)
{
/**************************************
 *
 *	r e c e i v e _ r e s p o n s e
 *
 **************************************
 *
 * Functional description
 *	Check response to a remote call.
 *
 **************************************/

	ISC_STATUS* status = packet->p_resp.p_resp_status_vector = rdb->rdb_status_vector;

	if (!receive_packet(rdb->rdb_port, packet, status))
		return false;

	return check_response(rdb, packet);
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
	RTR transaction = blob->rbl_rtr;
	RDB rdb = blob->rbl_rdb;
	SET_OBJECT(rdb, NULL, blob->rbl_id);

	for (RBL* p = &transaction->rtr_blobs; *p; p = &(*p)->rbl_next)
		if (*p == blob) {
			*p = blob->rbl_next;
			break;
		}

	if (blob->rbl_buffer != blob->rbl_data)
		ALLR_release(blob->rbl_buffer);

	ALLR_release(blob);
}


static void release_event( RVNT event)
{
/**************************************
 *
 *	r e l e a s e _ e v e n t
 *
 **************************************
 *
 * Functional description
 *	Release an event block.
 *
 **************************************/
	RDB rdb = event->rvnt_rdb;

	for (RVNT* p = &rdb->rdb_events; *p; p = &(*p)->rvnt_next)
		if (*p == event) {
			*p = event->rvnt_next;
			break;
		}

	ALLR_release(event);
}


static bool release_object(RDB rdb,
						   P_OP op,
						   USHORT id)
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
	PACKET* packet = &rdb->rdb_packet;
	packet->p_operation = op;
	packet->p_rlse.p_rlse_object = id;

	if (!send_packet(rdb->rdb_port, packet, rdb->rdb_status_vector))
		return false;

	return receive_response(rdb, packet);
}


static void release_request( rrq* request)
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
	RDB rdb = request->rrq_rdb;
	SET_OBJECT(rdb, NULL, request->rrq_id);
	REMOTE_release_request(request);
}


static void release_statement( RSR* statement)
{
/**************************************
 *
 *	r e l e a s e _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *	Release a GDML or SQL statement block ?
 *
 **************************************/

	if ((*statement)->rsr_bind_format) {
		ALLR_release((*statement)->rsr_bind_format);
	}
	if ((*statement)->rsr_user_select_format &&
		(*statement)->rsr_user_select_format !=
		(*statement)->rsr_select_format)
	{
		ALLR_release((*statement)->rsr_user_select_format);
	}
	if ((*statement)->rsr_select_format) {
		ALLR_release((*statement)->rsr_select_format);
	}

	REMOTE_release_messages((*statement)->rsr_message);
	ALLR_release((*statement));
	(*statement) = NULL;
}


static void release_sql_request( RSR statement)
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
	RDB rdb = statement->rsr_rdb;
	SET_OBJECT(rdb, NULL, statement->rsr_id);

	for (RSR* p = &rdb->rdb_sql_requests; *p; p = &(*p)->rsr_next)
		if (*p == statement) {
			*p = statement->rsr_next;
			break;
		}

	release_statement(&statement);
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
	RDB rdb = transaction->rtr_rdb;
	SET_OBJECT(rdb, NULL, transaction->rtr_id);

	for (RTR* p = &rdb->rdb_transactions; *p; p = &(*p)->rtr_next)
		if (*p == transaction) {
			*p = transaction->rtr_next;
			break;
		}

	ALLR_release(transaction);
}


static ISC_STATUS return_success( RDB rdb)
{
/**************************************
 *
 *	r e t u r n _ s u c c e s s
 *
 **************************************
 *
 * Functional description
 *	Set up status vector to reflect successful execution.
 *
 **************************************/
	REM_restore_thread_data();

	ISC_STATUS* p = rdb->rdb_status_vector;

/* If the status vector has not been initialized, then 
   initilalize the status vector to indicate success.  
   Else pass the status vector along at it stands.  */

	if (p[0] != isc_arg_gds || p[1] != FB_SUCCESS
		|| (p[2] != isc_arg_end && p[2] != isc_arg_gds
			&& p[2] != isc_arg_warning)) 
	{
		*p++ = isc_arg_gds;
		*p++ = FB_SUCCESS;
		*p = isc_arg_end;
	}

	return FB_SUCCESS;
}


#ifdef SCROLLABLE_CURSORS
static REM_MSG scroll_cache(
						ISC_STATUS * user_status,
						trdb *tdrdb,
						rrq* request,
						rem_port* port,
						rrq::rrq_repeat * tail,
						USHORT * direction, ULONG * offset)
{
/**************************************
 *
 *	s c r o l l _ c a c h e
 *
 **************************************
 *
 * Functional description
 *	
 * Try to fetch the requested record from cache, if possible.  This algorithm depends 
 * on all scrollable cursors being INSENSITIVE to database changes, so that absolute 
 * record numbers within the result set will remain constant. 
 *
 *  1.  BOF Forward or EOF Backward:  Retain the record number of the offset from the 
 *      beginning or end of the result set.  If we can figure out the relative offset 
 *      from the absolute, then scroll to it.  If it's in cache, great, otherwise dump 
 *      the cache and have the server scroll the correct number of records. 
 *
 *  2.  Forward or Backward:  Try to scroll the desired offset in cache.  If we  
 *      scroll off the end of the cache, dump the cache and ask the server for a 
 *      packetful of records.  
 *
 *  In the forward direction, assume X is the number of records cached. 
 *  If offset <= X, scroll forward offset records.  If offset > X, 
 *  dump the cache and send a message to the server to scroll forward (offset - X) 
 *  records.  However, if the server last scrolled in the backward direction, 
 *  ask the server to scroll forward (offset - X + C) records, where C is the 
 *  total number of records in cache. 
 *
 *  In the backward direction, do the same thing but in reverse.
 *
 **************************************/

/* if we are to continue in the current direction, set direction to 
   the last direction scrolled; then depending on the direction asked 
   for, save the last direction asked for by the next layer above */

	if (*direction == blr_continue) {
		if (tail->rrq_flags & RRQ_last_backward)
			*direction = blr_backward;
		else
			*direction = blr_forward;
	}

	if (*direction == blr_forward || *direction == blr_bof_forward)
		tail->rrq_flags &= ~RRQ_last_backward;
	else
		tail->rrq_flags |= RRQ_last_backward;

/* set to the last message returned to the higher level; 
   if none, set to the first message in cache */
	REM_MSG message = tail->rrq_last;
	if (!message) {
		message = tail->rrq_message;

		/* if the first record hasn't been returned yet and we are doing a relative seek 
		   forward (or backward when caching backwards), we effectively have just seeked 
		   forward one by positioning to the first record, so decrement the offset by one */

		if (*offset &&
			((*direction == blr_forward) && !(tail->rrq_flags & RRQ_backward))
			|| ((*direction == blr_backward)
				&& (tail->rrq_flags & RRQ_backward)))
		{
			(*offset)--;
		}
	}

/* if we are scrolling from BOF and the cache was started from EOF 
   (or vice versa), the cache is unusable. */

	if (
		(*direction == blr_bof_forward
		 && (tail->rrq_flags & RRQ_absolute_backward))
		|| (*direction == blr_eof_backward
			&& !(tail->
				 rrq_flags & RRQ_absolute_backward)))
	{
		return dump_cache(port, user_status, tail);
	}

/* if we are going to an absolute position, see if we can find that position 
   in cache, otherwise change to a relative seek from our former position */

	if (*direction == blr_bof_forward || *direction == blr_eof_backward) {
		/* if offset is before our current position, scroll backwards  
		   through the cache to see if we can find it */

		if (*offset < message->msg_absolute)
			for (;;) {
				if (message == tail->rrq_xdr || !message->msg_address) {
					/* if the cache was formed in the backward direction, see if 
					   there are any packets pending which might contain the record */

					if ((tail->rrq_flags & RRQ_backward)
						&& (tail->rrq_rows_pending > 0)) 
					{
						tail->rrq_message = message;
						while (!message->msg_address
							   && !request->rrq_status_vector[1])
						{
							if (!receive_queued_packet
								(tdrdb, port, user_status, request->rrq_id))
							{
								return NULL;
							}
						}
					}

					if ((message == tail->rrq_xdr) || !message->msg_address) {
						return dump_cache(port, user_status, tail);
					}
				}
				else {
					message = message->msg_prior;
				}

				if (*offset == message->msg_absolute)
					return message;
			}

		/* convert the absolute to relative, and prepare to scroll forward or 
		   back to look for the record */

		*offset -= message->msg_absolute;
		if (*direction == blr_bof_forward)
			*direction = blr_forward;
		else
			*direction = blr_backward;
	}

	for (; *offset; (*offset)--) {
		/* if the record was not found, see if there are any packets pending 
		   which might contain the record; otherwise dump the cache */

		if (!message->msg_address || message == tail->rrq_xdr)
		{
			if (tail->rrq_rows_pending > 0)
			{
				if (((*direction == blr_forward) &&
					!(tail->rrq_flags & RRQ_backward)) ||
					((*direction == blr_backward) &&
					(tail->rrq_flags & RRQ_backward)))
				{
					tail->rrq_message = message;
					while (!message->msg_address &&
							!request->rrq_status_vector[1])
					{
						if (!receive_queued_packet(tdrdb,
													port,
													user_status,
													request->rrq_id))
						{
							return NULL;
						}
					}
				}
			}

			if ((message == tail->rrq_xdr) || !message->msg_address)
			{
				return dump_cache(port, user_status, tail);
			}
		}

		/* step one record forward or back, depending on whether the cache was 
		   initially formed in the forward or backward direction */

		if (((*direction == blr_forward) &&
			!(tail->rrq_flags & RRQ_backward)) ||
			((*direction == blr_backward) &&
			(tail->rrq_flags & RRQ_backward)))
		{
			message = message->msg_next;
		}
		else
		{
			message = message->msg_prior;
		}
	}

	return message;
}
#endif


static ISC_STATUS send_and_receive(RDB rdb, PACKET* packet, ISC_STATUS* user_status)
{
/**************************************
 *
 *	s e n d _ a n d _ r e c e i v e
 *
 **************************************
 *
 * Functional description
 *	Send a packet, check status, receive a packet, and check status.
 *
 **************************************/

	if (!send_packet(rdb->rdb_port, packet, user_status))
	{
		return user_status[1];
	}

	if (!receive_response(rdb, packet))
	{
		return user_status[1];
	}

	return FB_SUCCESS;
}


static ISC_STATUS send_blob(ISC_STATUS*	user_status,
						RBL		blob,
						USHORT	buffer_length,
						const UCHAR*	buffer)
{
/**************************************
 *
 *	s e n d _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Actually send blob data (which might be buffered)
 *
 **************************************/
	RDB rdb = blob->rbl_rdb;
	PACKET* packet = &rdb->rdb_packet;
	packet->p_operation = op_put_segment;

/* If we aren't passed a buffer address, this is a batch send.  Pick up the
   address and length from the blob buffer and blast away */

	if (!buffer)
	{
		buffer = blob->rbl_buffer;
		buffer_length = blob->rbl_ptr - buffer;
		blob->rbl_ptr = 0;// <==> buffer;
		packet->p_operation = op_batch_segments;
	}

	P_SGMT* segment = &packet->p_sgmt;
	CSTRING_CONST temp = segment->p_sgmt_segment;
	segment->p_sgmt_blob = blob->rbl_id;
	segment->p_sgmt_segment.cstr_length = buffer_length;
	segment->p_sgmt_segment.cstr_address = buffer;
	segment->p_sgmt_length = buffer_length;

	if (!send_packet(rdb->rdb_port, packet, user_status))
	{
		return user_status[1];
	}

     // restore the string; "buffer" is not referenced anymore, hence no
     // possibility to overwrite it accidentally.
	segment->p_sgmt_segment = temp;

/* Set up for the response packet. */

	if (!receive_response(rdb, packet))
	{
		return user_status[1];
	}

	return FB_SUCCESS;
}


static void send_cancel_event(RVNT event)
{
/**************************************
 *
 *	s e n d _ c a n c e l _ e v e n t
 *
 **************************************
 *
 * Functional description
 *	Send a cancel event opcode to a remote
 *	server.
 *
 **************************************/

/* Look up the event's database, port and packet */

	RDB		rdb = event->rvnt_rdb;
	PACKET*	packet = &rdb->rdb_packet;

/* 
 Set the various parameters for the packet:
 remote operation to perform, which database,
 and which event.
*/

	packet->p_operation = op_cancel_events;
	packet->p_event.p_event_database = rdb->rdb_id;
	packet->p_event.p_event_rid = event->rvnt_id;

/* Send the packet, and if that worked, get a response */

	if (send_packet(rdb->rdb_port, packet, rdb->rdb_status_vector))
	{
		receive_response(rdb, packet);
	}

/*
 If the event has never been fired, fire it off with a length of 0.
 Note: it is job of person being notified to check that counts
 actually changed and that they were not woken up because of
 server death.
*/

	if (event->rvnt_id)
	{
		THREAD_EXIT();
		(*event->rvnt_ast)(event->rvnt_arg, (USHORT) 0, NULL);
		THREAD_ENTER();
		event->rvnt_id = 0;
	}
}


static bool send_packet(rem_port* port,
						PACKET* packet,
						ISC_STATUS* user_status)
{
/**************************************
 *
 *	s e n d _ p a c k e t
 *
 **************************************
 *
 * Functional description
 *	Send a packet and check for a network error
 *	on the send.
 *	Make up a status vector for any error.
 *	Note: SOME of the network lower level protocols
 *	will set up a status vector when errors
 *	occur, but other ones won't.
 *	So this routine sets up an error result
 *	for the vector and resets it to FB_SUCCESS
 *	if the packet send occured.
 *
 *	See also cousin routine: receive_packet
 *
 **************************************/

	user_status[0] = isc_arg_gds;
	user_status[1] = isc_net_write_err;
	user_status[2] = isc_arg_end;

	return (port->send(packet));
}

#ifdef NOT_USED_OR_REPLACED
static bool send_partial_packet(rem_port*		port,
								PACKET*	packet,
								ISC_STATUS*	user_status)
{
/**************************************
 *
 *	s e n d _ p a r t i a l _ p a c k e t
 *
 **************************************
 *
 * Functional description
 *	Send a packet and check for a network error
 *	on the send.
 *	Make up a status vector for any error.
 *	Note: SOME of the network lower level protocols
 *	will set up a status vector when errors
 *	occur, but other ones won't.
 *	So this routine sets up an error result
 *	for the vector and resets it to FB_SUCCESS
 *	if the packet send occured.
 *
 *	See also cousin routine: receive_packet, send_packet
 *
 **************************************/

	user_status[0] = isc_arg_gds;
	user_status[1] = isc_net_write_err;
	user_status[2] = isc_arg_end;

	if (!port->send_partial(packet)) {
		return false;
	}

	return true;
}
#endif

#ifdef MULTI_THREAD
static void server_death(rem_port* port)
{
/**************************************
 *
 *	s e r v e r _ d e a t h
 *
 **************************************
 *
 * Functional description
 *	Received "EOF" from remote server
 *	Cleanup events.
 *
 **************************************/
	THREAD_ENTER();
	RDB rdb = port->port_context;

	if (!(port->port_flags & PORT_disconnect))
	{
		for (RVNT event = rdb->rdb_events; event; event = event->rvnt_next)
		{
			if (event->rvnt_id)
			{
				THREAD_EXIT();
				(*event->rvnt_ast) (event->rvnt_arg, (USHORT) 0, NULL);
				THREAD_ENTER();
				event->rvnt_id = 0;
			}
		}
	}

	port->disconnect();
	THREAD_EXIT();
}
#endif


static void stuff_vax_integer(UCHAR* ptr, SLONG value, USHORT length)
{
/**************************************
 *
 *	s t u f f _ v a x _ i n t e g e r
 *
 **************************************
 *
 * Functional description
 *	Move an integer value into network format.
 *
 **************************************/

	while (length--) {
		*ptr++ = value % 256;
		value = value >> 8;
	}
}


static ISC_STATUS svcstart(ISC_STATUS*	user_status,
					   RDB		rdb,
					   P_OP		operation,
					   USHORT	object,
					   USHORT	incarnation,
					   USHORT	item_length,
					   const SCHAR*	items)
 {
/**************************************
 *
 *	s v c s t a r t
 *
 **************************************
 *
 * Functional description
 *	Instruct the server to start a service
 *
 **************************************/

/* Build the primary packet to get the operation started. */

	PACKET* packet = &rdb->rdb_packet;
	packet->p_operation = operation;
	P_INFO* information = &packet->p_info;
	information->p_info_object = object;
	information->p_info_incarnation = incarnation;
	information->p_info_items.cstr_length = item_length;
	information->p_info_items.cstr_address = (UCHAR *) items; // const_cast
	information->p_info_buffer_length = item_length;

/* Assume the result will be successful */

	fb_assert(user_status == rdb->rdb_status_vector);
	user_status[0] = isc_arg_gds;
	user_status[1] = FB_SUCCESS;
	user_status[2] = isc_arg_end;

	if (!send_packet(rdb->rdb_port, packet, user_status))
		return user_status[1];

/* Set up for the response packet. */

	P_RESP* response = &packet->p_resp;
	CSTRING temp = response->p_resp_data;

	if (!receive_response(rdb, packet)) {
		response->p_resp_data = temp;
		return user_status[1];
	}

	response->p_resp_data = temp;

	return rdb->rdb_status_vector[1];
}


static ISC_STATUS unsupported(ISC_STATUS* user_status)
{
/**************************************
 *
 *	u n s u p p o r t e d
 *
 **************************************
 *
 * Functional description
 *	No_entrypoint is called if there is not entrypoint for a given routine.
 *
 **************************************/

	REM_restore_thread_data();

	*user_status++ = isc_arg_gds;
	*user_status++ = isc_wish_list;
	*user_status = isc_arg_end;

	return isc_unavailable;
}


static void zap_packet(PACKET* packet)
{
/**************************************
 *
 *	z a p _ p a c k e t
 *
 **************************************
 *
 * Functional description
 *	Zero out a packet block.
 *
 **************************************/

	memset(packet, 0, sizeof(struct packet));
}

