/*
 *	PROGRAM:	JRD Remote Server
 *	MODULE:		server.cpp
 *	DESCRIPTION:	Remote server
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
 * 2002.02.27 Claudio Valderrama: Fix SF Bug #509995.
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#include "firebird.h"
#include "../jrd/common.h"
#include <stdio.h>
#include <string.h>
#include "../jrd/ibase.h"
#include "../jrd/gdsassert.h"
#include "../remote/remote.h"
#include "../jrd/thd.h"
#include "../jrd/isc.h"
#include "../jrd/license.h"
#include "../jrd/jrd_time.h"
#include "../remote/merge_proto.h"
#include "../remote/parse_proto.h"
#include "../remote/remot_proto.h"
#include "../remote/serve_proto.h"
#ifdef WIN_NT
#include "../remote/os/win32/cntl_proto.h"
#include <stdlib.h>
#endif
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thread_proto.h"
#include "../jrd/why_proto.h"
#include "../common/classes/semaphore.h"
#ifdef DEBUG
#include "gen/iberror.h"
#endif
#ifdef SUPERSERVER
#include "../jrd/os/isc_i_proto.h"
#endif
#include "../remote/proto_proto.h"	// xdr_protocol_overhead()

/** CHECK_HANDLE checks -
    if the id passwd is within the vector bounds,
    that the port_object corresponding to the id is not null,
    if the port_object's type matches with the expected type
**/

#pragma FB_COMPILER_MESSAGE("What kind of crap is this?! FIX!")

#define CHECK_HANDLE(blk,cast,type,id,err)							\
	{																\
		if (id >= port->port_object_vector->vec_count ||			\
			!(blk = (cast) port->port_objects [id]) ||				\
			((BLK) blk)->blk_type != (UCHAR) type)					\
		{															\
			status_vector [0] = isc_arg_gds;						\
			status_vector [1] = err;								\
			status_vector [2] = isc_arg_end;						\
			return port->send_response(sendL, 0, 0, status_vector);	\
		}															\
	}

#define CHECK_HANDLE_MEMBER(blk,cast,type,id,err)					\
	{																\
		if (id >= this->port_object_vector->vec_count ||			\
			!(blk = (cast) this->port_objects [id]) ||				\
			((BLK) blk)->blk_type != (UCHAR) type)					\
		{															\
			status_vector [0] = isc_arg_gds;						\
			status_vector [1] = err;								\
			status_vector [2] = isc_arg_end;						\
			return this->send_response(sendL, 0, 0, status_vector);	\
		}															\
	}


const USHORT STMT_BLOB		= 1;
const USHORT STMT_NO_BATCH	= 2;
const USHORT STMT_OTHER		= 0;

typedef struct server_req_t
{
	server_req_t*	req_next;
	server_req_t*	req_chain;
	rem_port*			req_port;
	PACKET			req_send;
	PACKET			req_receive;
} *SERVER_REQ;

typedef struct srvr
{
	struct srvr*	srvr_next;
	rem_port*		srvr_parent_port;
	enum rem_port_t	srvr_port_type;
	USHORT			srvr_flags;
} *SRVR;

static bool	accept_connection(rem_port*, P_CNCT*, PACKET*);
static ISC_STATUS	allocate_statement(rem_port*, P_RLSE*, PACKET*);
#ifdef MULTI_THREAD
static SLONG	append_request_chain(SERVER_REQ, SERVER_REQ*);
static SLONG	append_request_next(SERVER_REQ, SERVER_REQ*);
#endif
static ISC_STATUS	attach_database(rem_port*, P_OP, P_ATCH*, PACKET*);
#ifdef NOT_USED_OR_REPLACED
static void		aux_connect(rem_port*, P_REQ*, PACKET*);
#endif
static void		aux_request(rem_port*, P_REQ*, PACKET*);
static ISC_STATUS	cancel_events(rem_port*, P_EVENT*, PACKET*);

#ifdef CANCEL_OPERATION
static void	cancel_operation(rem_port*);
#endif

static bool	check_request(rrq*, USHORT, USHORT);
static USHORT	check_statement_type(RSR);

#ifdef SCROLLABLE_CURSORS
static REM_MSG		dump_cache(rrq::rrq_repeat*);
#endif

static bool		get_next_msg_no(rrq*, USHORT, USHORT*);
static RTR		make_transaction(RDB, FB_API_HANDLE);

static void	release_blob(RBL);
static void	release_event(RVNT);
static void	release_request(rrq*);
static void	release_statement(RSR*);
static void	release_sql_request(RSR);
static void	release_transaction(RTR);

#ifdef SCROLLABLE_CURSORS
static REM_MSG	scroll_cache(rrq::rrq_repeat*, USHORT *, ULONG *);
#endif

static void	server_ast(void*, USHORT, const UCHAR*);
static void		success(ISC_STATUS *);
#ifdef MULTI_THREAD
static THREAD_ENTRY_DECLARE loopThread(THREAD_ENTRY_PARAM);
#endif
static void		zap_packet(PACKET*, bool);


// static data - NOT THREAD SAFE!

static SLONG		threads_waiting		= 0;
static SLONG		extra_threads		= 0;
static SERVER_REQ	request_que			= NULL;
static SERVER_REQ	free_requests		= NULL;
static SERVER_REQ	active_requests		= NULL;
static SRVR			servers;

#ifdef MULTI_THREAD
static Firebird::Semaphore requests_semaphore;
#endif


static const UCHAR request_info[] =
{
	isc_info_state,
	isc_info_message_number,
	isc_info_end
};

static const UCHAR sql_info[] =
{
	isc_info_sql_stmt_type,
	isc_info_sql_batch_fetch
};


#define GDS_DSQL_ALLOCATE	isc_dsql_allocate_statement
#define GDS_DSQL_EXECUTE	isc_dsql_execute2_m
#define GDS_DSQL_EXECUTE_IMMED	isc_dsql_exec_immed3_m
#define GDS_DSQL_FETCH		isc_dsql_fetch_m
#define GDS_DSQL_FREE		isc_dsql_free_statement
#define GDS_DSQL_INSERT		isc_dsql_insert_m
#define GDS_DSQL_PREPARE	isc_dsql_prepare_m
#define GDS_DSQL_SET_CURSOR	isc_dsql_set_cursor_name
#define GDS_DSQL_SQL_INFO	isc_dsql_sql_info


void SRVR_main(rem_port* main_port, USHORT flags)
{
/**************************************
 *
 *	S R V R _ m a i n
 *
 **************************************
 *
 * Functional description
 *	Main entrypoint of server.
 *
 **************************************/
#ifdef SUPERSERVER
	ISC_enter();				/* Setup floating point exception handler once and for all. */
#endif

	PACKET send, receive;
	zap_packet(&receive, true);
	zap_packet(&send, true);
	THREAD_ENTER();
	set_server(main_port, flags);

	while (true)
	{
		//
		// Note: The following is cloned in server other SRVR_main instances.
		//
		rem_port* port = main_port->receive(&receive);
		if (!port) {
			break;
		}
		if (!process_packet(port, &send, &receive, 0)) {
			break;
		}
	}

	THREAD_EXIT();
}


void SRVR_multi_thread( rem_port* main_port, USHORT flags)
{
#ifdef MULTI_THREAD
/**************************************
 *
 *	S R V R _ m u l t i _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *	Multi-threaded flavor of server.
 *
 **************************************/
	SERVER_REQ request = NULL, active;
	rem_port* port = NULL; // Was volatile PORT port = NULL;
	SLONG pending_requests;
	P_OP operation;
#ifdef DEV_BUILD
#ifdef DEBUG
	SSHORT request_count = 0;
#endif /* DEBUG */
#endif /* DEV_BUILD */
	ISC_STATUS_ARRAY status_vector;
	trdb thd_context(status_vector);
	trdb* tdrdb;

	gds__thread_enable(-1);

	THREAD_ENTER();

	REM_set_thread_data(tdrdb, &thd_context);

	try {

	set_server(main_port, flags);

/* We need to have this error handler as there is so much underlaying code
 * that is depending on it being there.  The expected failure
 * that can occur in the call paths from here is out-of-memory
 * and it is unknown if other failures can get us here
 * Failures can occur from set_server as well as RECEIVE
 *
 * Note that if a failure DOES occur, we reenter the loop and try to continue
 * operations.  This is important as this is the main receive loop for
 * new incoming requests, if we exit here no new requests can come to the
 * server.
 */

	try {

/* When this loop exits, the server will no longer receive requests */
	while (true)
	{
		port = NULL;

		/* Allocate a memory block to store the request in */

		if (request = free_requests)
		{
			free_requests = request->req_next;
		}
		else
		{
			/* No block on the free list - allocate some new memory */

			request = (SERVER_REQ) gds__alloc((SLONG) sizeof(struct server_req_t));
			if (request)
			{
				zap_packet(&request->req_send, true);
				zap_packet(&request->req_receive, true);
#ifdef DEBUG_REMOTE_MEMORY
				printf("SRVR_multi_thread         allocate request %x\n",
						  request);
#endif
			}
			else
			{
				/* System is out of memory, let's delay processing this
				   request and hope another thread will free memory or
				   request blocks that we can then use. */

				THREAD_EXIT();
				THREAD_SLEEP(1 * 1000);
				THREAD_ENTER();
				continue;
			}
		}

#ifdef DEV_BUILD
#ifdef DEBUG
		if ((request_count++ % 4) == 0)
			throw std::bad_alloc();
#endif /* DEBUG */
#endif /* DEV_BUILD */

		if (request) {
			request->req_next = NULL;
			request->req_chain = NULL;

			/* We have a request block - now get some information to stick into it */
			if (!(port = main_port->receive(&request->req_receive)))
			{
				gds__log("SRVR_multi_thread/RECEIVE: error on main_port, shutting down");
				THREAD_EXIT();
				REM_restore_thread_data();
				return;
			}

			request->req_port = port;
			operation = request->req_receive.p_operation;

#ifdef DEV_BUILD
#ifdef DEBUG
			if ((request_count % 5) == 0)
				throw std::bad_alloc();
#endif /* DEBUG */
#endif /* DEV_BUILD */

			/* If port has an active request, link this one in */

			for (active = active_requests; active; active = active->req_next)
				if (active->req_port == port) {
					/* Don't queue a dummy keepalive packet if there is
					   an active request running on this port. */

					if (operation == op_dummy) {
						request->req_next = free_requests;
						free_requests = request;
						goto finished;
					}
					port->port_requests_queued++;
					append_request_chain(request, &active->req_chain);
#ifdef DEBUG_REMOTE_MEMORY
					printf
						("SRVR_multi_thread    ACTIVE     request_queued %d\n",
						 port->port_requests_queued);
					fflush(stdout);
#endif
#ifdef CANCEL_OPERATION
					if (operation == op_exit || operation == op_disconnect)
						cancel_operation(port);
#endif
					goto finished;
				}

			/* If port has an pending request, link this one in */

			for (active = request_que; active; active = active->req_next)
			{
				if (active->req_port == port)
				{
					/* Don't queue a dummy keepalive packet if there is
					   a pending request against this port. */

					if (operation == op_dummy) {
						request->req_next = free_requests;
						free_requests = request;
						goto finished;
					}
					port->port_requests_queued++;
					append_request_chain(request, &active->req_chain);
#ifdef DEBUG_REMOTE_MEMORY
					printf
						("SRVR_multi_thread    PENDING     request_queued %d\n",
						 port->port_requests_queued);
					fflush(stdout);
#endif
#ifdef CANCEL_OPERATION
					if (operation == op_exit || operation == op_disconnect)
						cancel_operation(port);
#endif
					goto finished;
				}
			}

			/* No port to assign request to, add it to the waiting queue and wake up a
			 * thread to handle it 
			 */
			REMOTE_TRACE(("Enqueue request %p", request));
			pending_requests = append_request_next(request, &request_que);
			port->port_requests_queued++;
#ifdef DEBUG_REMOTE_MEMORY
			printf
				("SRVR_multi_thread    APPEND_PENDING     request_queued %d\n",
				 port->port_requests_queued);
			fflush(stdout);
#endif

			/* NOTE: we really *should* have something that limits how many
			 * total threads we allow in the system.  As each thread will
			 * eat up memory that other threads could use to complete their work
			 */
			/* NOTE: The setting up of extra_threads variable is done below to let waiting
			   threads know if their services may be needed for the current set
			   of requests.  Otherwise, some idle threads may leave the system 
			   freeing up valuable memory.
			 */
			extra_threads = threads_waiting - pending_requests;
			if (extra_threads < 0) {
				gds__thread_start(	loopThread,
									(void*)(ULONG) flags,
									THREAD_medium,
									THREAD_ast,
									0);
			}

			REMOTE_TRACE(("Post event"));
			requests_semaphore.release();
		}
	  finished:
		;
	}
/* We should never get to this point */

	THREAD_EXIT();

	}	// try
	catch (const std::exception&)
	{
		/* If we got as far as having a port allocated before the error, disconnect it
		 * gracefully.
		 */
		if (port != NULL)
		{
/*
#ifdef DEV_BUILD
#ifdef DEBUG
			ConsolePrintf("%%ISERVER-F-NOPORT, no port in a storm\r\n");
#endif 
#endif 
*/
			gds__log("SRVR_multi_thread: forcefully disconnecting a port");

			/* To handle recursion within the error handler */
			try {
				/* If we have a port, request really should be non-null, but just in case ... */
				if (request != NULL) {
					/* Send client a real status indication of why we disconnected them */
					/* Note that send_response() can post errors that wind up in this same handler */
/*
#ifdef DEV_BUILD
#ifdef DEBUG
					ConsolePrintf
						("%%ISERVER-F-NOMEM, virtual memory exhausted\r\n");
#endif 
#endif 
*/
					port->send_response(&request->req_send, 0, 0,
								  status_vector);
					port->disconnect(&request->req_send, &request->req_receive);
				}
				else {
					/* Can't tell the client much, just make 'em go away.  Their side should detect
					 * a network error
					 */
					port->disconnect(NULL, NULL);
				}
				port = NULL;

			}	// try
			catch (const std::exception&) {
				port->disconnect(NULL, NULL);
				port = NULL;
			}
		}

		/* There was an error in the processing of the request, if we have allocated
		 * a request, free it up and continue.
		 */
		if (request != NULL) {
			request->req_next = free_requests;
			free_requests = request;
			request = NULL;
		}
	}

	}	// try
	catch (const std::exception&) {
		/* Some kind of unhandled error occured during server setup.  In lieu
		 * of anything we CAN do, log something (and we might be so hosed
		 * we can't log anything) and give up.
		 * The likely error here is out-of-memory.
		 */
		gds__log("SRVR_multi_thread: error during startup, shutting down");

		REM_restore_thread_data();
		THREAD_EXIT();
		return;
	}


/* Why isn't this inside the #endif above? */
	REM_restore_thread_data();
#endif
}


static bool accept_connection(rem_port* port,
							  P_CNCT * connect,
							  PACKET* send)
{
/**************************************
 *
 *	a c c e p t _ c o n n e c t i o n
 *
 **************************************
 *
 * Functional description
 *	Process a connect packet.
 *
 **************************************/
	P_ARCH architecture = arch_generic;
	USHORT version = 0;
	USHORT type = 0;
	bool accepted = false;
/* Accept the physical connection */

	send->p_operation = op_reject;
	P_ACPT* accept = &send->p_acpt;
	USHORT weight = 0;

	if (!port->accept(connect)) {
		port->send(send);
		return false;
	}

/* Select the most appropriate protocol (this will get smarter) */

	const p_cnct::p_cnct_repeat* protocol = connect->p_cnct_versions;

	for (const p_cnct::p_cnct_repeat* const end = protocol + connect->p_cnct_count;
		protocol < end; protocol++)
	{
		if ((protocol->p_cnct_version == PROTOCOL_VERSION3 ||
			 protocol->p_cnct_version == PROTOCOL_VERSION4 ||
			 protocol->p_cnct_version == PROTOCOL_VERSION5 ||
			 protocol->p_cnct_version == PROTOCOL_VERSION6 ||
			 protocol->p_cnct_version == PROTOCOL_VERSION7 ||
			 protocol->p_cnct_version == PROTOCOL_VERSION8 ||
			 protocol->p_cnct_version == PROTOCOL_VERSION9 ||
			 protocol->p_cnct_version == PROTOCOL_VERSION10
#ifdef SCROLLABLE_CURSORS
			 || protocol->p_cnct_version == PROTOCOL_SCROLLABLE_CURSORS
#endif
			) &&
			 (protocol->p_cnct_architecture == arch_generic ||
			 protocol->p_cnct_architecture == ARCHITECTURE) &&
			protocol->p_cnct_weight >= weight)
		{
			accepted = true;
			weight = protocol->p_cnct_weight;
			version = protocol->p_cnct_version;
			architecture = protocol->p_cnct_architecture;
			type = MIN(protocol->p_cnct_max_type, ptype_out_of_band);
			send->p_operation = op_accept;
		}
	}

/* Send off out gracious acceptance or flag rejection */
	if (!accepted) {
		port->send(send);
		return false;
	}
	accept->p_acpt_version = port->port_protocol = version;
	accept->p_acpt_architecture = architecture;
	accept->p_acpt_type = type;

/* and modify the version string to reflect the chosen protocol */

	TEXT buffer[64];
	sprintf(buffer, "%s/P%d", port->port_version->str_data,
			port->port_protocol);
	ALLR_free(port->port_version);
	port->port_version = REMOTE_make_string(buffer);

	if (architecture == ARCHITECTURE)
		port->port_flags |= PORT_symmetric;

	if (type == ptype_rpc)
		port->port_flags |= PORT_rpc;

	if (type != ptype_out_of_band)
		port->port_flags |= PORT_no_oob;

	port->send(send);

	return true;
}


static ISC_STATUS allocate_statement( rem_port* port, P_RLSE * allocate, PACKET* send)
{
/**************************************
 *
 *	a l l o c a t e _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *	Allocate a statement handle.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	RDB rdb = port->port_context;
	FB_API_HANDLE handle = 0;

	THREAD_EXIT();
	GDS_DSQL_ALLOCATE(status_vector, &rdb->rdb_handle, &handle);
	THREAD_ENTER();

	OBJCT object;
	if (status_vector[1])
		object = 0;
	else {
		/* Allocate SQL request block */

		RSR statement = (RSR) ALLR_block(type_rsr, 0);
		statement->rsr_rdb = rdb;
		statement->rsr_handle = handle;
		if (statement->rsr_id = port->get_id(&statement->rsr_header))
		{
			object = statement->rsr_id;
			statement->rsr_next = rdb->rdb_sql_requests;
			rdb->rdb_sql_requests = statement;
		}
		else {
			object = 0;
			THREAD_EXIT();
			GDS_DSQL_FREE(status_vector, &statement->rsr_handle, DSQL_drop);
			THREAD_ENTER();
			ALLR_release(statement);
			status_vector[0] = isc_arg_gds;
			status_vector[1] = isc_too_many_handles;
			status_vector[2] = isc_arg_end;
		}
	}

	return port->send_response(send, object, 0, status_vector);
}


#ifdef MULTI_THREAD
static SLONG append_request_chain( SERVER_REQ request, SERVER_REQ * que_inst)
{
/**************************************
 *
 *	a p p e n d _ r e q u e s t _ c h a i n
 *
 **************************************
 *
 * Functional description
 *	Traverse using req_chain ptr and append 
 *	a request at the end of a que.
 *	Return count of pending requests.
 *
 **************************************/
	SLONG requests;

	for (requests = 1; *que_inst; que_inst = &(*que_inst)->req_chain)
		++requests;

	*que_inst = request;

	return requests;
}

static SLONG append_request_next( SERVER_REQ request, SERVER_REQ * que_inst)
{
/**************************************
 *
 *	a p p e n d _ r e q u e s t _ n e x t
 *
 **************************************
 *
 * Functional description
 *	Traverse using req_next ptr and append 
 *	a request at the end of a que.
 *	Return count of pending requests.
 *
 **************************************/
	SLONG requests;

	for (requests = 1; *que_inst; que_inst = &(*que_inst)->req_next)
		++requests;

	*que_inst = request;

	return requests;
}
#endif


static ISC_STATUS attach_database(
							  rem_port* port,
							  P_OP operation, P_ATCH * attach, PACKET* send)
{
/**************************************
 *
 *	a t t a c h _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Process an attach or create packet.
 *
 **************************************/
	UCHAR new_dpb_buffer[512];
	ISC_STATUS_ARRAY status_vector;

	send->p_operation = op_accept;
	FB_API_HANDLE handle = 0;
	const char* file = reinterpret_cast<const char*>(attach->p_atch_file.cstr_address);
	const USHORT l = attach->p_atch_file.cstr_length;
	const UCHAR* dpb = attach->p_atch_dpb.cstr_address;
	USHORT dl = attach->p_atch_dpb.cstr_length;

/* If we have user identification, append it to database parameter block */

	UCHAR* new_dpb = new_dpb_buffer;
	rem_str* string = port->port_user_name;
	if (string) {
		if ((size_t)(dl + 3 + string->str_length) > sizeof(new_dpb_buffer))
			new_dpb = ALLR_alloc((SLONG) (dl + 3 + string->str_length));
		UCHAR* p = new_dpb;
		if (dl) {
			for (const UCHAR* const end = dpb + dl; dpb < end;)
				*p++ = *dpb++;
		}
		else
			*p++ = isc_dpb_version1;
		*p++ = isc_dpb_sys_user_name;
		*p++ = (UCHAR) string->str_length;
		dpb = (UCHAR *) string->str_data;
		for (const UCHAR* const end = dpb + string->str_length; dpb < end;)
			*p++ = *dpb++;
		dpb = new_dpb;
		dl = p - new_dpb;
	}

/* See if user has specified parameters relevant to the connection,
   they will be stuffed in the DPB if so. */
	REMOTE_get_timeout_params(port, dpb, dl);

	THREAD_EXIT();
	if (operation == op_attach)
	{
		isc_attach_database(status_vector, l, file,
							&handle, dl, reinterpret_cast<const char*>(dpb));
	}
	else
	{
		isc_create_database(status_vector, l, file,
							&handle, dl, reinterpret_cast<const char*>(dpb), 0);
	}
	THREAD_ENTER();

	if (new_dpb != new_dpb_buffer) {
		ALLR_free(new_dpb);
	}

	if (!status_vector[1])
	{
		RDB rdb = (RDB) ALLR_block(type_rdb, 0);
		port->port_context = rdb;
#ifdef DEBUG_REMOTE_MEMORY
		printf("attach_databases(server)  allocate rdb     %x\n", rdb);
#endif
		rdb->rdb_port = port;
		rdb->rdb_handle = handle;
	}

	return port->send_response(send, 0, 0, status_vector);
}


#ifdef NOT_USED_OR_REPLACED
static void aux_connect( rem_port* port, P_REQ * request, PACKET* send)
{
/**************************************
 *
 *	a u x _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	We're receive a auxiliary connection on the main communications
 *	channel.  Accept connection and reply politely.
 *
 * 13-Mar-2004, Nickolay Samofatov
 *  This code is 64-bit unsafe, unused and also has a security hole, thus I disable it for now
 *
 **************************************/
	port->connect(0, 0);
	rem_port* partner = (rem_port*) request->p_req_partner;
	partner->port_async = port;
}
#endif


static void aux_request( rem_port* port, P_REQ * request, PACKET* send)
{
/**************************************
 *
 *	a u x _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Other guy wants to establish a secondary connection.
 *	Humor him.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

/* save the port status vector */

	ISC_STATUS* const save_status = port->port_status_vector;

	port->port_status_vector = status_vector;
	success(status_vector);

/* We do this silliness with buffer because the SPX protocol
   requires a 12 byte buffer to be sent back.  Other protocols
   can do what they want to with cstr_address. */

	CSTRING save_cstring = send->p_resp.p_resp_data;
	UCHAR buffer[12];
	send->p_resp.p_resp_data.cstr_address = buffer;
	rem_port* aux_port = port->request(send);
	RDB rdb = port->port_context;
	port->send_response(send, rdb->rdb_id,
				  send->p_resp.p_resp_data.cstr_length, status_vector);

	if (status_vector[1]) {
		/* restore the port status vector */

		port->port_status_vector = save_status;
		send->p_resp.p_resp_data = save_cstring;
		return;
	}

	if (aux_port) {
		aux_port->connect(send, 0);
		aux_port->port_context = rdb;
	}

/* restore the port status vector */

	port->port_status_vector = save_status;
	send->p_resp.p_resp_data = save_cstring;
}


static ISC_STATUS cancel_events( rem_port* port, P_EVENT * stuff, PACKET* send)
{
/**************************************
 *
 *	c a n c e l _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *	Cancel events.
 *
 **************************************/

/* Which database ? */

	RDB rdb = port->port_context;

/* Find the event */

	RVNT event;
	for (event = rdb->rdb_events; event; event = event->rvnt_next)
		if (event->rvnt_rid == stuff->p_event_rid)
			break;

	ISC_STATUS_ARRAY status_vector;
	
/* If no event found, pretend it was cancelled */

	if (!event)
		return port->send_response(send, 0, 0, status_vector);

/* cancel the event */

	if (event->rvnt_id) {
		THREAD_EXIT();
		isc_cancel_events(status_vector, &rdb->rdb_handle, &event->rvnt_id);
		THREAD_ENTER();
	}

/* zero event info */

	event->rvnt_id = 0L;
	event->rvnt_rid = 0L;
	event->rvnt_ast = 0;

/* return response */

	return port->send_response(send, 0, 0, status_vector);
}


#ifdef CANCEL_OPERATION
static void cancel_operation( rem_port* port)
{
/**************************************
 *
 *	c a n c e l _ o p e r a t i o n
 *
 **************************************
 *
 * Functional description
 *	Flag a running operation for cancel.
 *	Service operations are not currently
 *	able to be canceled.
 *
 **************************************/
	RDB rdb;
	if ((port->port_flags & (PORT_async | PORT_disconnect)) ||
		!(rdb = port->port_context))
	{
		return;
	}

	if (rdb->rdb_handle)
	{
		if (!(rdb->rdb_flags & RDB_service))
		{
			THREAD_EXIT();
			ISC_STATUS_ARRAY status_vector;
			gds__cancel_operation(status_vector, (FB_API_HANDLE*) &rdb->rdb_handle,
								  CANCEL_raise);
			THREAD_ENTER();
		}
	}
}
#endif


static bool check_request(rrq* request,
						  USHORT incarnation,
						  USHORT msg_number)
{
/**************************************
 *
 *	c h e c k _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Check to see if a request is ready to send us a particular
 *	message.  If so, return TRUE, otherwise FALSE.
 *
 **************************************/
	USHORT n;

	if (!get_next_msg_no(request, incarnation, &n))
		return false;

	return (msg_number == n);
}


static USHORT check_statement_type( RSR statement)
{
/**************************************
 *
 *	c h e c k _ s t a t e m e n t _ t y p e
 *
 **************************************
 *
 * Functional description
 *	Return the type of SQL statement.
 *
 **************************************/
	UCHAR buffer[16];
	ISC_STATUS_ARRAY local_status;
	USHORT ret = STMT_OTHER;
	bool done = false;

	THREAD_EXIT();
	if (!GDS_DSQL_SQL_INFO(local_status, &statement->rsr_handle,
						   sizeof(sql_info), (const SCHAR*) sql_info,
						   sizeof(buffer), reinterpret_cast<char*>(buffer)))
	{
		for (const UCHAR* info = buffer; (*info != isc_info_end) && !done;)
		{
			const USHORT l = (USHORT) gds__vax_integer(info + 1, 2);
			const USHORT type = (USHORT) gds__vax_integer(info + 3, l);
			switch (*info)
			{
			case isc_info_sql_stmt_type:
				if (type == isc_info_sql_stmt_get_segment ||
					type == isc_info_sql_stmt_put_segment)
				{
					ret = STMT_BLOB;
					done = true;
				}
				break;
			case isc_info_sql_batch_fetch:
				if (type == 0)
					ret = STMT_NO_BATCH;
				break;
			case isc_info_error:
			case isc_info_truncated:
				done = true;
				break;

			}
			info += 3 + l;
		}
	}
	THREAD_ENTER();

	return ret;
}


ISC_STATUS rem_port::compile(P_CMPL* compileL, PACKET* sendL)
{
/**************************************
 *
 *	c o m p i l e
 *
 **************************************
 *
 * Functional description
 *	Compile and request.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	RDB rdb = this->port_context;
	FB_API_HANDLE handle = 0;
	const UCHAR* blr = compileL->p_cmpl_blr.cstr_address;
	USHORT blr_length = compileL->p_cmpl_blr.cstr_length;

	THREAD_EXIT();
	isc_compile_request(status_vector, &rdb->rdb_handle, &handle, blr_length,
						reinterpret_cast<const char*>(blr));
	THREAD_ENTER();

	if (status_vector[1])
		return this->send_response(sendL, 0, 0, status_vector);

/* Parse the request to find the messages */

	REM_MSG message = PARSE_messages(blr, blr_length);
	USHORT max_msg = 0;

	REM_MSG next;
	for (next = message; next; next = next->msg_next)
		max_msg = MAX(max_msg, next->msg_number);

/* Allocate block and merge into data structures */

	rrq* requestL = (rrq*) ALLR_block(type_rrq, max_msg + 1);
#ifdef DEBUG_REMOTE_MEMORY
	printf("compile(server)           allocate request %x\n", request);
#endif
	requestL->rrq_handle = handle;
	requestL->rrq_rdb = rdb;
	requestL->rrq_max_msg = max_msg;
	OBJCT object = 0;
	
	if (requestL->rrq_id = this->get_id(&requestL->rrq_header))
	{
		object = requestL->rrq_id;
		requestL->rrq_next = rdb->rdb_requests;
		rdb->rdb_requests = requestL;
	}
	else {
		THREAD_EXIT();
		isc_release_request(status_vector, &requestL->rrq_handle);
		THREAD_ENTER();
		ALLR_release(requestL);
		status_vector[0] = isc_arg_gds;
		status_vector[1] = isc_too_many_handles;
		status_vector[2] = isc_arg_end;
		return this->send_response(sendL, 0, 0, status_vector);
	}

	while (message) {
		next = message->msg_next;
		message->msg_next = message;
#ifdef SCROLLABLE_CURSORS
		message->msg_prior = message;
#endif

		rrq::rrq_repeat* tail = requestL->rrq_rpt + message->msg_number;
		tail->rrq_message = message;
		tail->rrq_xdr = message;
		tail->rrq_format = (rem_fmt*) message->msg_address;

		message->msg_address = NULL;
		message = next;
	}

	return this->send_response(sendL, object, 0, status_vector);
}


ISC_STATUS rem_port::ddl(P_DDL* ddlL, PACKET* sendL)
{
/**************************************
 *
 *	d d l
 *
 **************************************
 *
 * Functional description
 *	Execute isc_ddl call.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	RTR transaction;

	CHECK_HANDLE_MEMBER(transaction,
						RTR,
						type_rtr,
						ddlL->p_ddl_transaction,
						isc_bad_trans_handle);

	RDB rdb = this->port_context;
	const UCHAR* blr = ddlL->p_ddl_blr.cstr_address;
	const USHORT blr_length = ddlL->p_ddl_blr.cstr_length;

	THREAD_EXIT();
	isc_ddl(status_vector, &rdb->rdb_handle, &transaction->rtr_handle,
			blr_length, reinterpret_cast<const char*>(blr));
	THREAD_ENTER();

	return this->send_response(sendL, 0, 0, status_vector);
}


void rem_port::disconnect(PACKET* sendL, PACKET* receiveL)
{
/**************************************
 *
 *	d i s c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	We've lost the connection to the parent.  Stop everything.
 *
 **************************************/
	RDB rdb = this->port_context;

	if (this->port_flags & PORT_async)
	{
		if (rdb && rdb->rdb_port && !(rdb->rdb_port->port_flags & PORT_disconnect))
		{
			PACKET *packet = &rdb->rdb_packet;
			packet->p_operation = op_dummy;
			rdb->rdb_port->send(packet);
		}
		return;
	}

	this->port_flags |= PORT_disconnect;

	if (!rdb) {
		REMOTE_free_packet(this, sendL);
		REMOTE_free_packet(this, receiveL);
		this->disconnect();
		return;
	}

	/* For WNET and XNET we should send dummy op_disconnect packet
	   to wakeup async port handling events on client side.
	   For INET it's not necessary because INET client's async port
	   wakes up while server performs shutdown(socket) call on its async port.
	   See interface.cpp - event_thread(). */

	PACKET *packet = &rdb->rdb_packet;
	if ((this->port_async) &&
		((this->port_type == port_xnet) || (this->port_type == port_pipe)))
	{
		packet->p_operation = op_disconnect;
		this->port_async->send(packet);
	}

	ISC_STATUS_ARRAY status_vector;

	if (rdb->rdb_handle)
		if (!(rdb->rdb_flags & RDB_service)) {
#ifdef CANCEL_OPERATION
			/* Prevent a pending or spurious cancel from aborting
			   a good, clean detach from the database. */

			THREAD_EXIT();
			gds__cancel_operation(status_vector, (FB_API_HANDLE*) &rdb->rdb_handle,
								  CANCEL_disable);
			THREAD_ENTER();
#endif
			while (rdb->rdb_requests)
				release_request(rdb->rdb_requests);
			while (rdb->rdb_sql_requests)
				release_sql_request(rdb->rdb_sql_requests);
			RTR transaction;
			while (transaction = rdb->rdb_transactions) {
				THREAD_EXIT();
				if (!(transaction->rtr_flags & RTR_limbo))
					isc_rollback_transaction(status_vector,
											 &transaction->rtr_handle);
#ifdef SUPERSERVER
				/* The underlying JRD subsystem will release all
				   memory resources related to a limbo transaction
				   as a side-effect of the database detach call
				   below. However, the y-valve handle must be released
				   as well since an isc_disconnect_transaction()
				   call doesn't exist. */

				else
					gds__handle_cleanup(status_vector,
										(FB_API_HANDLE*) &transaction->rtr_handle);
#endif
				THREAD_ENTER();
				release_transaction(rdb->rdb_transactions);
			}
			THREAD_EXIT();
			isc_detach_database(status_vector, &rdb->rdb_handle);
			THREAD_ENTER();
			while (rdb->rdb_events) {
				release_event(rdb->rdb_events);
			}
			if (this->port_statement) {
				release_statement(&this->port_statement);
			}
		}
		else
		{
			THREAD_EXIT();
			isc_service_detach(status_vector, &rdb->rdb_handle);
			THREAD_ENTER();
		}

	REMOTE_free_packet(this, sendL);
	REMOTE_free_packet(this, receiveL);

#ifdef DEBUG_REMOTE_MEMORY
	printf("disconnect(server)        free rdb         %x\n", rdb);
#endif
	this->port_context = NULL;
	ALLR_release(rdb);
	if (this->port_object_vector)
	{
#ifdef DEBUG_REMOTE_MEMORY
		printf("disconnect(server)        free vector      %x\n",
				  this->port_object_vector);
#endif
		ALLR_release(this->port_object_vector);
		this->port_object_vector = NULL;
	}
	if (this->port_connection)
	{
#ifdef DEBUG_REMOTE_MEMORY
		printf("disconnect(server)        free string      %x\n",
				  this->port_connection);
#endif
		ALLR_release(this->port_connection);
		this->port_connection = NULL;
	}
	if (this->port_version)
	{
#ifdef DEBUG_REMOTE_MEMORY
		printf("disconnect(server)        free string      %x\n",
				  this->port_version);
#endif
		ALLR_release(this->port_version);
		this->port_version = NULL;
	}
	if (this->port_passwd)
	{
#ifdef DEBUG_REMOTE_MEMORY
		printf("disconnect(server)        free string      %x\n",
				  this->port_passwd);
#endif
		ALLR_release(this->port_passwd);
		this->port_passwd = NULL;
	}
	if (this->port_user_name)
	{
#ifdef DEBUG_REMOTE_MEMORY
		printf("disconnect(server)        free string      %x\n",
				  this->port_user_name);
#endif
		ALLR_release(this->port_user_name);
		this->port_user_name = NULL;
	}
	if (this->port_host)
	{
#ifdef DEBUG_REMOTE_MEMORY
		printf("disconnect(server)        free string      %x\n",
				  this->port_host);
#endif
		ALLR_release(this->port_host);
		this->port_host = NULL;
	}
	this->disconnect();
}


void rem_port::drop_database(P_RLSE* release, PACKET* sendL)
{
/**************************************
 *
 *	d r o p _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	End a request.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	RDB rdb = this->port_context;

	THREAD_EXIT();
	isc_drop_database(status_vector, &rdb->rdb_handle);
	THREAD_ENTER();

	if (status_vector[1]
		&& (status_vector[1] != isc_drdb_completed_with_errs))
	{
		this->send_response(sendL, 0, 0, status_vector);
		return;
	};

	while (rdb->rdb_events)
		release_event(rdb->rdb_events);

	while (rdb->rdb_requests)
		release_request(rdb->rdb_requests);

	while (rdb->rdb_sql_requests)
		release_sql_request(rdb->rdb_sql_requests);

	while (rdb->rdb_transactions)
		release_transaction(rdb->rdb_transactions);

	if (this->port_statement)
		release_statement(&this->port_statement);

	this->send_response(sendL, 0, 0, status_vector);
}


#ifdef SCROLLABLE_CURSORS
static REM_MSG dump_cache( rrq::rrq_repeat* tail)
{
/**************************************
 *
 *	d u m p _ c a c h e
 *
 **************************************
 *
 * Functional description
 *	We have encountered a situation where what's in 
 *	cache is not useful, so empty the cache in 
 *	preparation for refilling it. 
 *
 **************************************/
	REM_MSG message = tail->rrq_xdr;
	while (true) {
		message->msg_address = NULL;
		message = message->msg_next;
		if (message == tail->rrq_xdr)
			break;
	}

	tail->rrq_message = message;

	return message;
}
#endif


ISC_STATUS rem_port::end_blob(P_OP operation, P_RLSE * release, PACKET* sendL)
{
/**************************************
 *
 *	e n d _ b l o b
 *
 **************************************
 *
 * Functional description
 *	End a blob.
 *
 **************************************/
	RBL blob;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE_MEMBER(blob,
						RBL,
						type_rbl,
						release->p_rlse_object,
						isc_bad_segstr_handle);

	THREAD_EXIT();
	if (operation == op_close_blob)
		isc_close_blob(status_vector, &blob->rbl_handle);
	else
		isc_cancel_blob(status_vector, &blob->rbl_handle);
	THREAD_ENTER();

	if (!status_vector[1]) {
		release_blob(blob);
	}

	return this->send_response(sendL, 0, 0, status_vector);
}


ISC_STATUS rem_port::end_database(P_RLSE * release, PACKET* sendL)
{
/**************************************
 *
 *	e n d _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	End a request.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	RDB rdb = this->port_context;

	THREAD_EXIT();
	isc_detach_database(status_vector,&rdb->rdb_handle);
	THREAD_ENTER();

	if (status_vector[1])
		return this->send_response(sendL, 0, 0, status_vector);

	while (rdb->rdb_events)
		release_event(rdb->rdb_events);

	while (rdb->rdb_requests)
		release_request(rdb->rdb_requests);

	while (rdb->rdb_sql_requests)
		release_sql_request(rdb->rdb_sql_requests);

	while (rdb->rdb_transactions)
		release_transaction(rdb->rdb_transactions);

	if (this->port_statement)
		release_statement(&this->port_statement);

	return this->send_response(sendL, 0, 0, status_vector);
}


ISC_STATUS rem_port::end_request(P_RLSE * release, PACKET* sendL)
{
/**************************************
 *
 *	e n d _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	End a request.
 *
 **************************************/
	rrq* requestL;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE_MEMBER(requestL,
						rrq*,
						type_rrq,
						release->p_rlse_object,
						isc_bad_req_handle);

	THREAD_EXIT();
	isc_release_request(status_vector, &requestL->rrq_handle);
	THREAD_ENTER();

	if (!status_vector[1])
		release_request(requestL);

	return this->send_response(sendL, 0, 0, status_vector);
}


ISC_STATUS rem_port::end_statement(P_SQLFREE* free_stmt, PACKET* sendL)
{
/*****************************************
 *
 *	e n d _ s t a t e m e n t
 *
 *****************************************
 *
 * Functional description
 *	Free a statement.
 *
 *****************************************/
	RSR statement;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE_MEMBER(statement,
						RSR,
						type_rsr,
						free_stmt->p_sqlfree_statement,
						isc_bad_req_handle);

	THREAD_EXIT();
	GDS_DSQL_FREE(status_vector,
				  &statement->rsr_handle,
				  free_stmt->p_sqlfree_option);
	THREAD_ENTER();

	if (status_vector[1])
		return this->send_response(sendL, 0, 0, status_vector);

	if (!statement->rsr_handle) {
		release_sql_request(statement);
		statement = NULL;
	}
	else {
		statement->rsr_flags &= ~RSR_fetched;
		statement->rsr_rtr = NULL;
		REMOTE_reset_statement(statement);
		statement->rsr_message = statement->rsr_buffer;
	}

	const USHORT object = (statement) ? statement->rsr_id : (USHORT) - 1;

	return this->send_response(sendL, object, 0, status_vector);
}


ISC_STATUS rem_port::end_transaction(P_OP operation, P_RLSE * release, PACKET* sendL)
{
/**************************************
 *
 *	e n d _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	End a transaction.
 *
 **************************************/
	RTR transaction;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE_MEMBER(transaction,
						RTR,
						type_rtr,
						release->p_rlse_object,
						isc_bad_trans_handle);

	THREAD_EXIT();
	switch (operation)
	{
	case op_commit:
		isc_commit_transaction(status_vector, &transaction->rtr_handle);
		break;

	case op_rollback:
		isc_rollback_transaction(status_vector, &transaction->rtr_handle);
		break;

	case op_rollback_retaining:
		isc_rollback_retaining(status_vector, &transaction->rtr_handle);
		break;

	case op_commit_retaining:
		isc_commit_retaining(status_vector, &transaction->rtr_handle);
		break;

	case op_prepare:
		if (!isc_prepare_transaction(status_vector, &transaction->rtr_handle))
				transaction->rtr_flags |= RTR_limbo;
		break;
	}
	THREAD_ENTER();

	if (!status_vector[1])
		if (operation == op_commit || operation == op_rollback) {
			REMOTE_cleanup_transaction(transaction);
			release_transaction(transaction);
		}

	return this->send_response(sendL, 0, 0, status_vector);
}


ISC_STATUS rem_port::execute_immediate(P_OP op, P_SQLST * exnow, PACKET* sendL)
{
/*****************************************
 *
 *	e x e c u t e _ i m m e d i a t e
 *
 *****************************************
 *
 * Functional description
 *	process an execute immediate DSQL packet
 *
 *****************************************/
	RTR transaction = NULL;
	USHORT in_blr_length, in_msg_type, parser_version,
		out_blr_length, out_msg_type;
	ISC_STATUS_ARRAY status_vector;

	RDB rdb = this->port_context;

/** Do not call CHECK_HANDLE if this is the start of a transaction **/
	if (this->port_objects && exnow->p_sqlst_transaction) {
		CHECK_HANDLE_MEMBER(transaction,
							RTR,
							type_rtr,
							exnow->p_sqlst_transaction,
							isc_bad_trans_handle);
	}

	USHORT in_msg_length = 0, out_msg_length = 0;
	UCHAR* in_msg = NULL;
	UCHAR* out_msg = NULL;
	UCHAR* in_blr;
	UCHAR* out_blr;
	if (op == op_exec_immediate2)
	{
		in_blr_length = exnow->p_sqlst_blr.cstr_length;
		in_blr = exnow->p_sqlst_blr.cstr_address;
		in_msg_type = exnow->p_sqlst_message_number;
		if (this->port_statement->rsr_bind_format)
		{
			in_msg_length = this->port_statement->rsr_bind_format->fmt_length;
			in_msg = this->port_statement->rsr_message->msg_address;
		}
		out_blr_length = exnow->p_sqlst_out_blr.cstr_length;
		out_blr = exnow->p_sqlst_out_blr.cstr_address;
		out_msg_type = exnow->p_sqlst_out_message_number;
		if (this->port_statement->rsr_select_format)
		{
			out_msg_length =
				this->port_statement->rsr_select_format->fmt_length;
			if (!this->port_statement->rsr_message->msg_address)
			{
				/* TMN: Obvious bugfix. Please look at your compilers warnings. */
				/* They are not enemies, they're friends! */
				/* port->port_statement->rsr_message->msg_address = &port->port_statement->rsr_message->msg_buffer; */
				this->port_statement->rsr_message->msg_address =
					this->port_statement->rsr_message->msg_buffer;
			}
			out_msg = this->port_statement->rsr_message->msg_address;
		}
	}
	else
	{
		in_blr_length = out_blr_length = 0;
		in_blr = out_blr = NULL;
		in_msg_type = out_msg_type = 0;
	}

	FB_API_HANDLE handle = (transaction) ? transaction->rtr_handle : 0;

/* Since the API to GDS_DSQL_EXECUTE_IMMED is public and can not be changed, there needs to
 * be a way to send the parser version to DSQL so that the parser can compare the keyword
 * version to the parser version.  To accomplish this, the parser version is combined with
 * the client dialect and sent across that way.  In dsql8_execute_immediate, the parser version
 * and client dialect are separated and passed on to their final desintations.  The information
 * is combined as follows:
 *     Dialect * 10 + parser_version
 *
 * and is extracted in dsql8_execute_immediate as follows:
 *      parser_version = ((dialect * 10) + parser_version) % 10
 *      client_dialect = ((dialect * 10) + parser_version) / 10
 *
 * For example, parser_version = 1 and client dialect = 1
 *
 *  combined = (1 * 10) + 1 == 11
 *
 *  parser = (combined) % 10 == 1
 *  dialect = (combined) / 10 == 1
 */

	parser_version = (this->port_protocol < PROTOCOL_VERSION10) ? 1 : 2;

	THREAD_EXIT();
	GDS_DSQL_EXECUTE_IMMED(status_vector,
						   &rdb->rdb_handle,
						   &handle,
						   exnow->p_sqlst_SQL_str.cstr_length,
						   reinterpret_cast<char*>(exnow->p_sqlst_SQL_str.cstr_address),
						   (USHORT) ((exnow->p_sqlst_SQL_dialect * 10) +
									 parser_version),
						   in_blr_length,
						   reinterpret_cast<char*>(in_blr),
						   in_msg_type,
						   in_msg_length,
						   reinterpret_cast<char*>(in_msg),
						   out_blr_length,
						   reinterpret_cast<char*>(out_blr),
						   out_msg_type,
						   out_msg_length,
						   reinterpret_cast<char*>(out_msg));
	THREAD_ENTER();

	if (op == op_exec_immediate2)
	{
		this->port_statement->rsr_format =
			this->port_statement->rsr_select_format;

		sendL->p_operation = op_sql_response;
		sendL->p_sqldata.p_sqldata_messages = (status_vector[1]
											  || !out_msg) ? 0 : 1;
		this->send_partial(sendL);
	}

	if (!status_vector[1]) {
		if (transaction && !handle) {
			REMOTE_cleanup_transaction(transaction);
			release_transaction(transaction);
			transaction = NULL;
		}
		else if (!transaction && handle) {
			if (!(transaction = make_transaction(rdb, handle))) {
				status_vector[0] = isc_arg_gds;
				status_vector[1] = isc_too_many_handles;
				status_vector[2] = isc_arg_end;
			}
		}
	}

	return this->send_response(	sendL,
								(OBJCT) (transaction ? transaction->rtr_id : 0),
								0,
								status_vector);
}


ISC_STATUS rem_port::execute_statement(P_OP op, P_SQLDATA* sqldata, PACKET* sendL)
{
/*****************************************
 *
 *	e x e c u t e _ s t a t e m e n t
 *
 *****************************************
 *
 * Functional description
 *	Execute a non-SELECT dynamic SQL statement.
 *
 *****************************************/
	RTR transaction = NULL;
	ISC_STATUS_ARRAY status_vector;

/** Do not call CHECK_HANDLE if this is the start of a transaction **/
	if (sqldata->p_sqldata_transaction)
	{
		CHECK_HANDLE_MEMBER(transaction,
							RTR,
							type_rtr,
							sqldata->p_sqldata_transaction,
							isc_bad_trans_handle);
	}

	RSR statement;
	CHECK_HANDLE_MEMBER(statement,
						RSR,
						type_rsr,
						sqldata->p_sqldata_statement,
						isc_bad_req_handle);

	USHORT in_msg_length = 0, out_msg_length = 0;
	UCHAR* in_msg = NULL;
	UCHAR* out_msg = NULL;
	USHORT out_msg_type, out_blr_length;
	UCHAR* out_blr;

	if (statement->rsr_format)
	{
		in_msg_length = statement->rsr_format->fmt_length;
		in_msg = statement->rsr_message->msg_address;
	}
	if (op == op_execute2)
	{
		out_blr_length = sqldata->p_sqldata_out_blr.cstr_length;
		out_blr = sqldata->p_sqldata_out_blr.cstr_address;
		out_msg_type = sqldata->p_sqldata_out_message_number;
		if (this->port_statement->rsr_select_format)
		{
			out_msg_length =
				this->port_statement->rsr_select_format->fmt_length;
			out_msg = this->port_statement->rsr_message->msg_buffer;
		}
	}
	else
	{
		out_blr_length = 0;
		out_msg_type = 0;
		out_blr = NULL;
	}
	statement->rsr_flags &= ~RSR_fetched;

	FB_API_HANDLE handle = (transaction) ? transaction->rtr_handle : 0;

	THREAD_EXIT();
	GDS_DSQL_EXECUTE(status_vector,
					 &handle,
					 &statement->rsr_handle,
					 sqldata->p_sqldata_blr.cstr_length,
					 reinterpret_cast<char*>(sqldata->p_sqldata_blr.cstr_address),
					 sqldata->p_sqldata_message_number,
					 in_msg_length,
					 reinterpret_cast<char*>(in_msg),
					 out_blr_length,
					 reinterpret_cast<char*>(out_blr),
					 out_msg_type,
					 out_msg_length,
					 reinterpret_cast<char*>(out_msg));
	THREAD_ENTER();

	if (op == op_execute2)
	{
		this->port_statement->rsr_format =
			this->port_statement->rsr_select_format;

		sendL->p_operation = op_sql_response;
		sendL->p_sqldata.p_sqldata_messages = (status_vector[1]
											  || !out_msg) ? 0 : 1;
		this->send_partial(sendL);
	}

	if (!status_vector[1]) {
		if (transaction && !handle) {
			REMOTE_cleanup_transaction(transaction);
			release_transaction(transaction);
			transaction = NULL;
		}
		else if (!transaction && handle) {
			if (!(transaction = make_transaction(statement->rsr_rdb, handle))) {
				status_vector[0] = isc_arg_gds;
				status_vector[1] = isc_too_many_handles;
				status_vector[2] = isc_arg_end;
			}
		}

		statement->rsr_rtr = transaction;
	}

	return this->send_response(	sendL,
								(OBJCT) (transaction ? transaction->rtr_id : 0),
								0,
								status_vector);
}


ISC_STATUS rem_port::fetch(P_SQLDATA * sqldata, PACKET* sendL)
{
/*****************************************
 *
 *	f e t c h
 *
 *****************************************
 *
 * Functional description
 *	Fetch next record from a dynamic SQL cursor.
 *
 *****************************************/
	RSR statement;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE_MEMBER(statement,
						RSR,
						type_rsr,
						sqldata->p_sqldata_statement,
						isc_bad_req_handle);

	if (statement->rsr_flags & RSR_blob) {
		return this->fetch_blob(sqldata, sendL);
	}

	USHORT msg_length;
	if (statement->rsr_format) {
		msg_length = statement->rsr_format->fmt_length;
	}
	else {
		msg_length = 0;
	}
	USHORT count = ((this->port_flags & PORT_rpc) ||
			(statement->rsr_flags & RSR_no_batch)) ? 1 :
			sqldata->p_sqldata_messages;
	USHORT count2 = (statement->rsr_flags & RSR_no_batch) ? 0 : count;

/* On first fetch, clear the end-of-stream flag & reset the message buffers */

	if (!(statement->rsr_flags & RSR_fetched)) {
		statement->rsr_flags &= ~(RSR_eof | RSR_stream_err);
		memset(statement->rsr_status_vector, 0,
			   sizeof(statement->rsr_status_vector));
		REM_MSG message = statement->rsr_message;
		if (message != NULL) {
			statement->rsr_buffer = message;
			while (true) {
				message->msg_address = NULL;
				message = message->msg_next;
				if (message == statement->rsr_message)
					break;
			}
		}
	}

/* Get ready to ship the data out */

	P_SQLDATA* response = &sendL->p_sqldata;
	sendL->p_operation = op_fetch_response;
	response->p_sqldata_statement = sqldata->p_sqldata_statement;
	response->p_sqldata_status = 0;
	response->p_sqldata_messages = 1;
	ISC_STATUS s = 0;
	REM_MSG message = NULL;

/* Check to see if any messages are already sitting around */

	while (true) {

		/* Have we exhausted the cache & reached cursor EOF? */
		if ((statement->rsr_flags & RSR_eof) && !statement->rsr_msgs_waiting) {
			statement->rsr_flags &= ~RSR_eof;
			s = 100;
			count2 = 0;
			break;
		}

		/* Have we exhausted the cache & have a pending error? */
		if ((statement->rsr_flags & RSR_stream_err)
			&& !statement->rsr_msgs_waiting)
		{
			statement->rsr_flags &= ~RSR_stream_err;
			return this->send_response(sendL, 0, 0,
								 statement->rsr_status_vector);
		}

		message = statement->rsr_buffer;

		/* Make sure message can be de referenced, if not then return false */
		if (message == NULL)
			return FALSE;

		/* If we don't have a message cached, get one from the
		   access method. */

		if (!message->msg_address) {
			fb_assert(statement->rsr_msgs_waiting == 0);
			THREAD_EXIT();
			s = GDS_DSQL_FETCH(status_vector,
							   &statement->rsr_handle,
							   sqldata->p_sqldata_blr.cstr_length,
							   reinterpret_cast<const char*>(sqldata->p_sqldata_blr.cstr_address),
							   sqldata->p_sqldata_message_number,
							   msg_length,
							   reinterpret_cast<char*>(message->msg_buffer));

			THREAD_ENTER();
			statement->rsr_flags |= RSR_fetched;
			if (s) {
				if (s == 100 || s == 101) {
					count2 = 0;
					break;
				}
				else {
					return this->send_response(sendL, 0, 0, status_vector);
				}
			}
			message->msg_address = message->msg_buffer;
		}
		else {
			/* Take a message from the outqoing queue */
			fb_assert(statement->rsr_msgs_waiting >= 1);
			statement->rsr_msgs_waiting--;
		}

		/* For compatibility with Protocol 7, we must break out of the
		   loop before sending the last record. */

		count--;
		if (this->port_protocol <= PROTOCOL_VERSION7 && count <= 0) {
			break;
		}

		/* There's a buffer waiting -- send it */

		if (!this->send_partial(sendL)) {
			return FALSE;
		}
		message->msg_address = NULL;

		/* If we've sent the requested amount, break out of loop */

		if (count <= 0)
			break;
	}

	response->p_sqldata_status = s;
	response->p_sqldata_messages = 0;
	this->send(sendL);
	if (message) {
		message->msg_address = NULL;
	}

/* Since we have a little time on our hands while this packet is sent
   and processed, get the next batch of records.  Start by finding the
   next free buffer. */

	message = statement->rsr_buffer;
	REM_MSG next = NULL;

	while (message->msg_address && message->msg_next != statement->rsr_buffer)
		message = message->msg_next;

	for (; count2; --count2) {
		if (message->msg_address) {
			if (!next)
				for (next = statement->rsr_buffer; next->msg_next != message;
					 next = next->msg_next);
			message = (REM_MSG) ALLR_block(type_msg, statement->rsr_fmt_length);
			message->msg_number = next->msg_number;
			message->msg_next = next->msg_next;
			next->msg_next = message;
			next = message;
		}
		THREAD_EXIT();
		s = GDS_DSQL_FETCH(status_vector,
						   &statement->rsr_handle,
						   sqldata->p_sqldata_blr.cstr_length,
						   reinterpret_cast<char*>(sqldata->p_sqldata_blr.cstr_address),
						   sqldata->p_sqldata_message_number,
						   msg_length,
						   reinterpret_cast<char*>(message->msg_buffer));
		THREAD_ENTER();
		if (s) {
			if (status_vector[1]) {
				/* If already have an error queued, don't overwrite it */
				if (!(statement->rsr_flags & RSR_stream_err)) {
					statement->rsr_flags |= RSR_stream_err;
					memcpy(statement->rsr_status_vector, status_vector,
						   sizeof(statement->rsr_status_vector));
				}
			}
			if (s == 100)
				statement->rsr_flags |= RSR_eof;
			break;
		}
		message->msg_address = message->msg_buffer;
		message = message->msg_next;
		statement->rsr_msgs_waiting++;
	}

	return TRUE;
}


ISC_STATUS rem_port::fetch_blob(P_SQLDATA * sqldata, PACKET* sendL)
{
/*****************************************
 *
 *	f e t c h _ b l o b
 *
 *****************************************
 *
 * Functional description
 *	Fetch next record from a dynamic SQL cursor.
 *
 *****************************************/
	RSR statement;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE_MEMBER(statement,
						RSR,
						type_rsr,
						sqldata->p_sqldata_statement,
						isc_bad_req_handle);

	USHORT msg_length;
	if (statement->rsr_format)
		msg_length = statement->rsr_format->fmt_length;
	else
		msg_length = 0;

	REM_MSG message = statement->rsr_message;
	if (message != NULL)
		statement->rsr_buffer = message;

/* Get ready to ship the data out */

	P_SQLDATA* response = &sendL->p_sqldata;
	sendL->p_operation = op_fetch_response;
	response->p_sqldata_statement = sqldata->p_sqldata_statement;
	response->p_sqldata_status = 0;
	response->p_sqldata_messages = 1;
	ISC_STATUS s = 0;
	message = statement->rsr_buffer;

	THREAD_EXIT();
	s = GDS_DSQL_FETCH(status_vector,
					   &statement->rsr_handle,
					   sqldata->p_sqldata_blr.cstr_length,
					   reinterpret_cast<char*>(sqldata->p_sqldata_blr.cstr_address),
					   sqldata->p_sqldata_message_number,
					   msg_length,
					   reinterpret_cast<char*>(message->msg_buffer));
	THREAD_ENTER();

	if (!status_vector[1] ||
		status_vector[1] != isc_segment || status_vector[1] != isc_segstr_eof)
	{
		message->msg_address = message->msg_buffer;
		response->p_sqldata_status = s;
		response->p_sqldata_messages =
			(status_vector[1] == isc_segstr_eof) ? 0 : 1;
		this->send_partial(sendL);
		message->msg_address = NULL;
	}

	return this->send_response(sendL, 0, 0, status_vector);
}


OBJCT rem_port::get_id(BLK block)
{
/**************************************
 *
 *	g e t _ i d
 *
 **************************************
 *
 * Functional description
 *	Allocate an object slot for an object.
 *	If the object vector cannot be expanded
 *	to accomodate the object slot then
 *	REMOTE_set_object() will return a NULL
 *	object slot.
 *
 **************************************/

/* If there isn't a vector, obvious the object goes in slot 1.
   Reserve slot 0 so we can distinguish something from nothing.
   NOTE: prior to server version 4.5.0 id==0 COULD be used - so
   only the server side can now depend on id==0 meaning "invalid id" */

	rem_vec* vector = this->port_object_vector;
	if (!vector) {
		return REMOTE_set_object(this, block, (OBJCT) 1);
	}

	// Search vector for an empty slot.  If we find one, use it

	blk** p = vector->vec_object + 1;
	for (const blk* const* const end = vector->vec_object + vector->vec_count;
		p < end; p++)
	{
		if (!*p) {
			*p = block;
			return (OBJCT) (p - vector->vec_object);
		}
	}

/* Vector is full -- somebody will need to expand it */

	return REMOTE_set_object(this, block, (OBJCT) vector->vec_count);
}


static bool get_next_msg_no(rrq* request,
							USHORT incarnation,
							USHORT * msg_number)
{
/**************************************
 *
 *	g e t _ n e x t _ m s g _ n o
 *
 **************************************
 *
 * Functional description
 *	Return the number of the next message
 *	in the request.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	UCHAR info_buffer[128];

	THREAD_EXIT();
	isc_request_info(status_vector, &request->rrq_handle, incarnation,
					 sizeof(request_info), reinterpret_cast<const SCHAR*>(request_info),
					 sizeof(info_buffer), reinterpret_cast<char*>(info_buffer));
	THREAD_ENTER();

	if (status_vector[1])
		return false;

	bool result = false;
	for (const UCHAR* info = info_buffer; *info != isc_info_end;) {
		const USHORT l = (USHORT) gds__vax_integer(info + 1, 2);
		const USHORT n = (USHORT) gds__vax_integer(info + 3, l);

		switch (*info) {
		case isc_info_state:
			if (n != isc_info_req_send)
				return false;
			break;

		case isc_info_message_number:
			*msg_number = n;
			result = true;
			break;

		default:
			return false;
		}
		info += 3 + l;
	}

	return result;
}


ISC_STATUS rem_port::get_segment(P_SGMT* segment, PACKET* sendL)
{
/**************************************
 *
 *	g e t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Get a single blob segment.
 *
 **************************************/
	RBL blob;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE_MEMBER(blob,
						RBL,
						type_rbl,
						segment->p_sgmt_blob,
						isc_bad_segstr_handle);

	UCHAR temp_buffer[BLOB_LENGTH];
	USHORT buffer_length = segment->p_sgmt_length;
	UCHAR* buffer;
	if (buffer_length <= sizeof(temp_buffer))
		buffer = temp_buffer;
	else
	{
		if (buffer_length > blob->rbl_buffer_length) {
			if (blob->rbl_buffer != blob->rbl_data) {
				ALLR_free(blob->rbl_buffer);
				blob->rbl_buffer = blob->rbl_data;
				blob->rbl_buffer_length = 1;
			}
			blob->rbl_buffer = ALLR_alloc((SLONG) buffer_length);
			blob->rbl_buffer_length = buffer_length;
		}
		buffer = blob->rbl_buffer;
	}
#ifdef DEBUG_REMOTE_MEMORY
	printf("get_segment(server)       allocate buffer  %x\n", buffer);
#endif
	sendL->p_resp.p_resp_data.cstr_address = buffer;

/* Be backwards compatible */

	USHORT length;
	if (this->port_flags & PORT_rpc)
	{
		length = 0;
		THREAD_EXIT();
		isc_get_segment(status_vector, &blob->rbl_handle, &length,
						segment->p_sgmt_length,
						reinterpret_cast<char*>(buffer));
		THREAD_ENTER();
		const ISC_STATUS status =
			this->send_response(sendL, blob->rbl_id, length, status_vector);
#ifdef DEBUG_REMOTE_MEMORY
		printf("get_segment(server)       free buffer      %x\n", buffer);
#endif
		if (status_vector[1] == isc_segstr_eof)
			if (blob->rbl_buffer != blob->rbl_data) {
				ALLR_free(blob->rbl_buffer);
				blob->rbl_buffer = blob->rbl_data;
				blob->rbl_buffer_length = 1;
			}

		return status;
	}

/* Gobble up a buffer's worth of segments */

	UCHAR* p = buffer;
	ISC_STATUS state = 0;

	while (buffer_length > 2) {
		buffer_length -= 2;
		p += 2;
		THREAD_EXIT();
		isc_get_segment(status_vector, &blob->rbl_handle, &length,
						buffer_length, reinterpret_cast<char*>(p));
		THREAD_ENTER();
		if (status_vector[1] == isc_segstr_eof)
		{
			state = 2;
			success(status_vector);
			p -= 2;
			break;
		}
		if (status_vector[1] && (status_vector[1] != isc_segment))
		{
			p -= 2;
			break;
		}
		p[-2] = (UCHAR) length;
		p[-1] = (UCHAR) (length >> 8);
		p += length;
		buffer_length -= length;
		if (status_vector[1] == isc_segment) {
			state = 1;
			success(status_vector);
			break;
		}
	}

	const ISC_STATUS status = this->send_response(sendL,
								(OBJCT)state,
								(USHORT) (p - buffer),
								status_vector);

#ifdef DEBUG_REMOTE_MEMORY
	printf("get_segment(server)       free buffer      %x\n", buffer);
#endif

	if (status_vector[1] == isc_segstr_eof)
		if (blob->rbl_buffer != blob->rbl_data) {
			ALLR_free(blob->rbl_buffer);
			blob->rbl_buffer = blob->rbl_data;
			blob->rbl_buffer_length = 1;
		}

	return status;
}


ISC_STATUS rem_port::get_slice(P_SLC * stuff, PACKET* sendL)
{
/**************************************
 *
 *	g e t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *	Get an array slice.
 *
 **************************************/
	RTR transaction;
	ISC_STATUS_ARRAY status_vector;

	RDB rdb = this->port_context;
	CHECK_HANDLE_MEMBER(transaction,
						RTR,
						type_rtr,
						stuff->p_slc_transaction,
						isc_bad_trans_handle);

	UCHAR temp_buffer[4096];
	UCHAR* slice;
	if (!stuff->p_slc_length)
		slice = 0;
	else {
		if (stuff->p_slc_length <= sizeof(temp_buffer))
			slice = temp_buffer;
		else
			slice = ALLR_alloc((SLONG) stuff->p_slc_length);
	}

	if (slice) {
		memset(slice, 0, stuff->p_slc_length);
#ifdef DEBUG_REMOTE_MEMORY
		printf("get_slice(server)         allocate buffer  %x\n", slice);
#endif
	}
	P_SLR* response = &sendL->p_slr;

	THREAD_EXIT();
	isc_get_slice(status_vector, &rdb->rdb_handle, &transaction->rtr_handle,
				  (ISC_QUAD*) &stuff->p_slc_id, stuff->p_slc_sdl.cstr_length,
				  reinterpret_cast<const char*>(stuff->p_slc_sdl.cstr_address),
				  stuff->p_slc_parameters.cstr_length,
				  (const ISC_LONG*) stuff->p_slc_parameters.cstr_address,
				  stuff->p_slc_length, slice,
				  reinterpret_cast<SLONG*>(&response->p_slr_length));
	THREAD_ENTER();

	ISC_STATUS status;
	if (status_vector[1])
		status = this->send_response(sendL, 0, 0, status_vector);
	else {
		sendL->p_operation = op_slice;
		response->p_slr_slice.lstr_address = slice;
		response->p_slr_slice.lstr_length = response->p_slr_length;
		response->p_slr_sdl = stuff->p_slc_sdl.cstr_address;
		response->p_slr_sdl_length = stuff->p_slc_sdl.cstr_length;
		this->send(sendL);
		response->p_slr_sdl = 0;
		status = FB_SUCCESS;
	}

	if (slice) {
#ifdef DEBUG_REMOTE_MEMORY
		printf("get_slice(server)         free buffer      %x\n", slice);
#endif
		if (slice != temp_buffer)
			ALLR_free(slice);
	}

	return status;
}


ISC_STATUS rem_port::info(P_OP op, P_INFO * stuff, PACKET* sendL)
{
/**************************************
 *
 *	i n f o
 *
 **************************************
 *
 * Functional description
 *	Get info for a blob, database, request, service,
 *	statement, or transaction.
 *
 **************************************/
	RBL blob;
	RTR transaction;
	RSR statement;
	ISC_STATUS_ARRAY status_vector;

	RDB rdb = this->port_context;

/* Make sure there is a suitable temporary blob buffer */

	UCHAR* const buffer = ALLR_alloc((SLONG) stuff->p_info_buffer_length);
	memset(buffer, 0, stuff->p_info_buffer_length);
#ifdef DEBUG_REMOTE_MEMORY
	printf("info(server)              allocate buffer  %x\n", buffer);
#endif

	UCHAR temp[1024];
	UCHAR* temp_buffer;
	if (op == op_info_database && stuff->p_info_buffer_length > sizeof(temp)) {
	    temp_buffer = ALLR_alloc((SLONG) stuff->p_info_buffer_length);
#ifdef DEBUG_REMOTE_MEMORY
	    printf("info(server)              allocate buffer  %x\n", temp_buffer);
#endif
	}
	else
    	temp_buffer = temp;

	switch (op) {
	case op_info_blob:
		CHECK_HANDLE_MEMBER(blob,
							RBL,
							type_rbl,
							stuff->p_info_object,
							isc_bad_segstr_handle);
		THREAD_EXIT();
		isc_blob_info(status_vector, &blob->rbl_handle,
					  stuff->p_info_items.cstr_length,
					  reinterpret_cast<char*>(stuff->p_info_items.cstr_address),
					  stuff->p_info_buffer_length,
					  reinterpret_cast<char*>(buffer));
		THREAD_ENTER();
		break;

	case op_info_database:
		THREAD_EXIT();
		isc_database_info(status_vector, &rdb->rdb_handle,
						  stuff->p_info_items.cstr_length,
						  reinterpret_cast<const char*>(stuff->p_info_items.cstr_address),
						  stuff->p_info_buffer_length /*sizeof (temp)*/,
						  reinterpret_cast<char*>(temp_buffer) /*temp*/);
		if (!status_vector[1]) {
			TEXT version[256];
			sprintf(version, "%s/%s", GDS_VERSION,
					this->port_version->str_data);
			MERGE_database_info(temp_buffer /*temp*/, buffer, stuff->p_info_buffer_length,
								IMPLEMENTATION, 4, 1,
								reinterpret_cast<const UCHAR*>(version),
								reinterpret_cast<UCHAR*>(this->port_host->str_data),
								0);
		}
		THREAD_ENTER();
		break;

	case op_info_request:
		{
		rrq* requestL;
		CHECK_HANDLE_MEMBER(requestL,
							rrq*,
							type_rrq,
							stuff->p_info_object,
							isc_bad_req_handle);
		THREAD_EXIT();
		isc_request_info(status_vector, &requestL->rrq_handle,
						 stuff->p_info_incarnation,
						 stuff->p_info_items.cstr_length,
						 reinterpret_cast<const char*>(stuff->p_info_items.cstr_address),
						 stuff->p_info_buffer_length,
						 reinterpret_cast<char*>(buffer));
		THREAD_ENTER();
		break;
		}

	case op_info_transaction:
		CHECK_HANDLE_MEMBER(transaction,
							RTR,
							type_rtr,
							stuff->p_info_object,
							isc_bad_trans_handle);
		THREAD_EXIT();
		isc_transaction_info(status_vector, &transaction->rtr_handle,
							 stuff->p_info_items.cstr_length,
							 reinterpret_cast<const char*>(stuff->p_info_items.cstr_address),
							 stuff->p_info_buffer_length,
							 reinterpret_cast < char *>(buffer));
		THREAD_ENTER();
		break;

	case op_service_info:
		THREAD_EXIT();
		isc_service_query(status_vector,
						  &rdb->rdb_handle,
						  NULL,
						  stuff->p_info_items.cstr_length,
						  reinterpret_cast<
						  const char*>(stuff->p_info_items.cstr_address),
						  stuff->p_info_recv_items.cstr_length,
						  reinterpret_cast<
						  const char*>(stuff->p_info_recv_items.cstr_address),
						  stuff->p_info_buffer_length,
						  reinterpret_cast<char*>(buffer));
		THREAD_ENTER();
		break;

	case op_info_sql:
		CHECK_HANDLE_MEMBER(statement,
							RSR,
							type_rsr,
							stuff->p_info_object,
							isc_bad_req_handle);
		THREAD_EXIT();
		GDS_DSQL_SQL_INFO(status_vector,
						  &statement->rsr_handle,
						  stuff->p_info_items.cstr_length,
						  reinterpret_cast<
						  const char*>(stuff->p_info_items.cstr_address),
						  stuff->p_info_buffer_length,
						  reinterpret_cast < char *>(buffer));
		THREAD_ENTER();
		break;
	}

	if (temp_buffer != temp) {
#ifdef DEBUG_REMOTE_MEMORY
    	printf ("info(server)              free buffer      %x\n", temp_buffer);
#endif
    	ALLR_free(temp_buffer);
	}

/* Send a response that includes the segment. */

	sendL->p_resp.p_resp_data.cstr_address = buffer;

	const ISC_STATUS status = this->send_response(sendL, stuff->p_info_object,
						   stuff->p_info_buffer_length, status_vector);
#ifdef DEBUG_REMOTE_MEMORY
	printf("info(server)              free buffer      %x\n", buffer);
#endif
	ALLR_free(buffer);

	return status;
}


ISC_STATUS rem_port::insert(P_SQLDATA * sqldata, PACKET* sendL)
{
/*****************************************
 *
 *	i n s e r t
 *
 *****************************************
 *
 * Functional description
 *	Insert next record into a dynamic SQL cursor.
 *
 *****************************************/
	RSR statement;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE_MEMBER(statement,
						RSR,
						type_rsr,
						sqldata->p_sqldata_statement,
						isc_bad_req_handle);

	USHORT msg_length;
	const UCHAR* msg;
	if (statement->rsr_format)
	{
		msg_length = statement->rsr_format->fmt_length;
		msg = statement->rsr_message->msg_address;
	}
	else
	{
		msg_length = 0;
		msg = NULL;
	}

	THREAD_EXIT();
	GDS_DSQL_INSERT(status_vector,
					&statement->rsr_handle,
					sqldata->p_sqldata_blr.cstr_length,
					reinterpret_cast<const char*>(sqldata->p_sqldata_blr.cstr_address),
					sqldata->p_sqldata_message_number, msg_length,
					reinterpret_cast<const char*>(msg));
	THREAD_ENTER();

	return this->send_response(sendL, 0, 0, status_vector);
}


static RTR make_transaction (RDB rdb, FB_API_HANDLE handle)
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
	transaction->rtr_handle = handle;
	if (transaction->rtr_id = rdb->rdb_port->get_id(&transaction->rtr_header))
	{
		transaction->rtr_next = rdb->rdb_transactions;
		rdb->rdb_transactions = transaction;
	}
	else {
		ALLR_release(transaction);
		transaction = NULL;
	}

	return transaction;
}


ISC_STATUS rem_port::open_blob(P_OP op, P_BLOB* stuff, PACKET* sendL)
{
/**************************************
 *
 *	o p e n _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Open or create a new blob.
 *
 **************************************/
	RTR transaction;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE_MEMBER(transaction,
						RTR,
						type_rtr,
						stuff->p_blob_transaction,
						isc_bad_trans_handle);

	RDB rdb = this->port_context;
	FB_API_HANDLE handle = 0;
	USHORT bpb_length = 0;
	const UCHAR* bpb = NULL;

	if (op == op_open_blob2 || op == op_create_blob2) {
		bpb_length = stuff->p_blob_bpb.cstr_length;
		bpb = stuff->p_blob_bpb.cstr_address;
	}

	THREAD_EXIT();
	if (op == op_open_blob || op == op_open_blob2)
		isc_open_blob2(status_vector, &rdb->rdb_handle, 
					   &transaction->rtr_handle, &handle,
					   (ISC_QUAD*) &stuff->p_blob_id, bpb_length, bpb);
	else
		isc_create_blob2(status_vector, &rdb->rdb_handle, &transaction->rtr_handle,
						 &handle, (ISC_QUAD*) &sendL->p_resp.p_resp_blob_id,
						 bpb_length, reinterpret_cast<const char*>(bpb));
	THREAD_ENTER();

	USHORT object;
	if (status_vector[1])
		object = 0;
	else {
		RBL blob = (RBL) ALLR_block(type_rbl, 1);
#ifdef DEBUG_REMOTE_MEMORY
		printf("open_blob(server)         allocate blob    %x\n", blob);
#endif
		blob->rbl_buffer_length = 1;
		blob->rbl_buffer = blob->rbl_data;
		blob->rbl_handle = handle;
		blob->rbl_rdb = rdb;
		if (blob->rbl_id = this->get_id(&blob->rbl_header))
		{
			object = blob->rbl_id;
			blob->rbl_rtr = transaction;
			blob->rbl_next = transaction->rtr_blobs;
			transaction->rtr_blobs = blob;
		}
		else
		{
			object = 0;
			THREAD_EXIT();
			isc_cancel_blob(status_vector, &blob->rbl_handle);
			THREAD_ENTER();
			ALLR_release(blob);
			status_vector[0] = isc_arg_gds;
			status_vector[1] = isc_too_many_handles;
			status_vector[2] = isc_arg_end;
		}
	}

	return this->send_response(sendL, object, 0, status_vector);
}


ISC_STATUS rem_port::prepare(P_PREP * stuff, PACKET* sendL)
{
/**************************************
 *
 *	p r e p a r e
 *
 **************************************
 *
 * Functional description
 *	End a transaction.
 *
 **************************************/
	RTR transaction;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE_MEMBER(transaction,
						RTR,
						type_rtr,
						stuff->p_prep_transaction,
						isc_bad_trans_handle);

	THREAD_EXIT();
	if (!isc_prepare_transaction2(status_vector, &transaction->rtr_handle,
								  stuff->p_prep_data.cstr_length,
								  stuff->p_prep_data.cstr_address))
	{
		transaction->rtr_flags |= RTR_limbo;
	}
	THREAD_ENTER();

	return this->send_response(sendL, 0, 0, status_vector);
}


ISC_STATUS rem_port::prepare_statement(P_SQLST * prepareL, PACKET* sendL)
{
/*****************************************
 *
 *	p r e p a r e _ s t a t m e n t
 *
 *****************************************
 *
 * Functional description
 *	Prepare a dynamic SQL statement for execution.
 *
 *****************************************/
	RTR transaction = NULL;
	RSR statement;
	ISC_STATUS_ARRAY status_vector;

/** Do not call CHECK_HANDLE if this is the start of a transaction **/
	if (prepareL->p_sqlst_transaction)
	{
		CHECK_HANDLE_MEMBER(transaction,
							RTR,
							type_rtr,
							prepareL->p_sqlst_transaction,
							isc_bad_trans_handle);
	}
	CHECK_HANDLE_MEMBER(statement,
						RSR,
						type_rsr,
						prepareL->p_sqlst_statement,
						isc_bad_req_handle);

	UCHAR local_buffer[1024];
	UCHAR* buffer;
	if (prepareL->p_sqlst_buffer_length > sizeof(local_buffer))
		buffer = ALLR_alloc((SLONG) prepareL->p_sqlst_buffer_length);
	else
		buffer = local_buffer;

	FB_API_HANDLE handle = (transaction) ? transaction->rtr_handle : 0;


/* Since the API to GDS_DSQL_PREPARE is public and can not be changed, there needs to
 * be a way to send the parser version to DSQL so that the parser can compare the keyword
 * version to the parser version.  To accomplish this, the parser version is combined with
 * the client dialect and sent across that way.  In dsql8_prepare_statement, the parser version
 * and client dialect are separated and passed on to their final desintations.  The information
 * is combined as follows:
 *     Dialect * 10 + parser_version
 *
 * and is extracted in dsql8_prepare_statement as follows:
 *      parser_version = ((dialect * 10) + parser_version) % 10
 *      client_dialect = ((dialect * 10) + parser_version) / 10
 *
 * For example, parser_version = 1 and client dialect = 1
 *
 *  combined = (1 * 10) + 1 == 11
 *
 *  parser = (combined) % 10 == 1
 *  dialect = (combined) / 10 == 1
 */
	const USHORT parser_version = (this->port_protocol < PROTOCOL_VERSION10) ? 1 : 2;

	THREAD_EXIT();
	GDS_DSQL_PREPARE(status_vector,
					 &handle,
					 &statement->rsr_handle,
					 prepareL->p_sqlst_SQL_str.cstr_length,
					 reinterpret_cast<const char*>(prepareL->p_sqlst_SQL_str.cstr_address),
					 (USHORT) ((prepareL->p_sqlst_SQL_dialect * 10) +
							   parser_version),
					 prepareL->p_sqlst_items.cstr_length,
					 reinterpret_cast<const char*>(prepareL->p_sqlst_items.cstr_address),
					 prepareL->p_sqlst_buffer_length,
					 reinterpret_cast<char*>(buffer));
	THREAD_ENTER();

	if (status_vector[1]) {
		if (buffer != local_buffer)
			ALLR_free(buffer);
		return this->send_response(sendL, 0, 0, status_vector);
	}

	REMOTE_reset_statement(statement);

	statement->rsr_flags &= ~(RSR_blob | RSR_no_batch);
	USHORT state = check_statement_type(statement);
	if (state == STMT_BLOB)
		statement->rsr_flags |= RSR_blob;
	else if (state == STMT_NO_BATCH)
		statement->rsr_flags |= RSR_no_batch;
	state = (state == STMT_BLOB) ? 1 : 0;

/* Send a response that includes the info requested. */

	sendL->p_resp.p_resp_data.cstr_address = buffer;

	const ISC_STATUS status =
		this->send_response(sendL,
							state,
							prepareL->p_sqlst_buffer_length,
							status_vector);

	if (buffer != local_buffer) {
		ALLR_free(buffer);
	}

	return status;
}


bool process_packet(rem_port* port,
					PACKET* sendL,
					PACKET* receive,
					rem_port** result)
{
/**************************************
 *
 *	p r o c e s s _ p a c k e t
 *
 **************************************
 *
 * Functional description
 *	Given an packet received from client, process it a packet ready to be
 *	sent.
 *
 **************************************/
	TEXT msg[128];
	trdb thd_context(port->port_status_vector);
	// BRS: This is the same as REM_set_thread_data
	trdb* tdrdb = &thd_context;
	tdrdb->putSpecific();

	try {
		P_OP op = receive->p_operation;
		switch (op)
		{
		case op_connect:
			if (!accept_connection(port, &receive->p_cnct, sendL)) {
				rem_str* string = port->port_user_name;
				if (string) {
					sprintf(msg,
							"SERVER/process_packet: connection rejected for %*.*s",
							string->str_length, string->str_length,
							string->str_data);
					gds__log(msg, 0);
				}
				if (port->port_server->srvr_flags & SRVR_multi_client) {
					port->port_state = state_broken;
				}
				else {
					gds__log
						("SERVER/process_packet: connect reject, server exiting",
						 0);
					ThreadData::restoreSpecific();
					return false;
				}
			}
			break;

		case op_compile:
			port->compile(&receive->p_cmpl, sendL);
			break;

		case op_attach:
		case op_create:
			attach_database(port, op, &receive->p_atch, sendL);
			break;

		case op_service_attach:
			port->service_attach(&receive->p_atch, sendL);
			break;

		case op_service_start:
			port->service_start(&receive->p_info, sendL);
			break;

		case op_disconnect:
		case op_exit:
			{
				SRVR server = port->port_server;
				if (!server)
					break;
				if ((server->srvr_flags & SRVR_multi_client) &&
					port != server->srvr_parent_port)
				{

					port->disconnect(sendL, receive);
					port = NULL;
					break;
				}
				else {
					if ((server->srvr_flags & SRVR_multi_client) &&
						port == server->srvr_parent_port)
					{
						gds__log("SERVER/process_packet: Multi-client server shutdown", 0);
					}
					port->disconnect(sendL, receive);
					ThreadData::restoreSpecific();
					return false;
				}
			}

		case op_receive:
			port->receive_msg(&receive->p_data, sendL);
			break;

		case op_send:
			port->send_msg(&receive->p_data, sendL);
			break;

		case op_start:
		case op_start_and_receive:
			port->start(op, &receive->p_data, sendL);
			break;

		case op_start_and_send:
		case op_start_send_and_receive:
			port->start_and_send(op, &receive->p_data, sendL);
			break;

		case op_transact:
			port->transact_request(&receive->p_trrq, sendL);
			break;

		case op_reconnect:
		case op_transaction:
			port->start_transaction(op, &receive->p_sttr, sendL);
			break;

		case op_prepare:
		case op_rollback:
		case op_rollback_retaining:
		case op_commit:
		case op_commit_retaining:
			port->end_transaction(op, &receive->p_rlse, sendL);
			break;

		case op_detach:
			port->end_database(&receive->p_rlse, sendL);
			break;

		case op_service_detach:
			port->service_end(&receive->p_rlse, sendL);
			break;

		case op_drop_database:
			port->drop_database(&receive->p_rlse, sendL);
			break;

		case op_create_blob:
		case op_open_blob:
		case op_create_blob2:
		case op_open_blob2:
			port->open_blob(op, &receive->p_blob, sendL);
			break;

		case op_batch_segments:
		case op_put_segment:
			port->put_segment(op, &receive->p_sgmt, sendL);
			break;

		case op_get_segment:
			port->get_segment(&receive->p_sgmt, sendL);
			break;

		case op_seek_blob:
			port->seek_blob(&receive->p_seek, sendL);
			break;

		case op_cancel_blob:
		case op_close_blob:
			port->end_blob(op, &receive->p_rlse, sendL);
			break;

		case op_prepare2:
			port->prepare(&receive->p_prep, sendL);
			break;

		case op_release:
			port->end_request(&receive->p_rlse, sendL);
			break;

		case op_info_blob:
		case op_info_database:
		case op_info_request:
		case op_info_transaction:
		case op_service_info:
		case op_info_sql:
			port->info(op, &receive->p_info, sendL);
			break;

		case op_que_events:
			port->que_events(&receive->p_event, sendL);
			break;

		case op_cancel_events:
			cancel_events(port, &receive->p_event, sendL);
			break;

		case op_connect_request:
			aux_request(port, &receive->p_req, sendL);
			break;

#ifdef NOT_USED_OR_REPLACED
		case op_aux_connect:
			aux_connect(port, &receive->p_req, sendL);
			break;
#endif

		case op_ddl:
			port->ddl(&receive->p_ddl, sendL);
			break;

		case op_get_slice:
			port->get_slice(&receive->p_slc, sendL);
			break;

		case op_put_slice:
			port->put_slice(&receive->p_slc, sendL);
			break;

		case op_allocate_statement:
			allocate_statement(port, &receive->p_rlse, sendL);
			break;

		case op_execute:
		case op_execute2:
			port->execute_statement(op, &receive->p_sqldata, sendL);
			break;

		case op_exec_immediate:
		case op_exec_immediate2:
			port->execute_immediate(op, &receive->p_sqlst, sendL);
			break;

		case op_fetch:
			port->fetch(&receive->p_sqldata, sendL);
			break;

		case op_free_statement:
			port->end_statement(&receive->p_sqlfree, sendL);
			break;

		case op_insert:
			port->insert(&receive->p_sqldata, sendL);
			break;

		case op_prepare_statement:
			port->prepare_statement(&receive->p_sqlst, sendL);
			break;

		case op_set_cursor:
			port->set_cursor(&receive->p_sqlcur, sendL);
			break;

		case op_dummy:
			sendL->p_operation = op_dummy;
			port->send(sendL);
			break;

		default:
			sprintf(msg, "SERVER/process_packet: don't understand packet type %d",
					receive->p_operation);
			gds__log(msg, 0);
			port->port_state = state_broken;
			break;
		}

		if (port && port->port_state == state_broken) {
			if (!port->port_parent) {
				gds__log("SERVER/process_packet: broken port, server exiting", 0);
				if (port->port_type == port_inet)
					port->disconnect();
				else
					port->disconnect(sendL, receive);
				ThreadData::restoreSpecific();
				return false;
			}
			port->disconnect(sendL, receive);
			port = NULL;
		}

		if (result)
			*result = port;

		ThreadData::restoreSpecific();
	
	}	// try
	catch (const std::exception&) {
		/* There must be something better to do here.  BUT WHAT? */

		gds__log("SERVER/process_packet: out of memory", 0);

		/*  It would be nice to log an error to the user, instead of just terminating them!  */
		port->send_response(sendL, 0, 0, tdrdb->trdb_status_vector);
		port->disconnect(sendL, receive);	/*  Well, how about this...  */

		ThreadData::restoreSpecific();
		return false;
	}

	return true;
}


ISC_STATUS rem_port::put_segment(P_OP op, P_SGMT * segment, PACKET* sendL)
{
/**************************************
 *
 *	p u t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Write a single blob segment.
 *
 **************************************/
	RBL blob;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE_MEMBER(blob,
						RBL,
						type_rbl,
						segment->p_sgmt_blob,
						isc_bad_segstr_handle);

	const UCHAR* p = segment->p_sgmt_segment.cstr_address;
	USHORT length = segment->p_sgmt_segment.cstr_length;

/* Do the signal segment version.  If it failed, just pass on the
   bad news. */

	if (op == op_put_segment) {
		THREAD_EXIT();
		isc_put_segment(status_vector, &blob->rbl_handle, length,
						reinterpret_cast<const char*>(p));
		THREAD_ENTER();
		return this->send_response(sendL, 0, 0, status_vector);
	}

/* We've got a batch of segments.  This is only slightly more complicated */

	const UCHAR* const end = p + length;

	while (p < end) {
		length = *p++;
		length += *p++ << 8;
		THREAD_EXIT();
		isc_put_segment(status_vector, &blob->rbl_handle, length,
						reinterpret_cast<const char*>(p));
		THREAD_ENTER();
		if (status_vector[1])
			return this->send_response(sendL, 0, 0, status_vector);
		p += length;
	}

	return this->send_response(sendL, 0, 0, status_vector);
}


ISC_STATUS rem_port::put_slice(P_SLC * stuff, PACKET* sendL)
{
/**************************************
 *
 *	p u t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *	Put an array slice.
 *
 **************************************/
	RTR transaction;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE_MEMBER(transaction,
						RTR,
						type_rtr,
						stuff->p_slc_transaction,
						isc_bad_trans_handle);

	RDB rdb = this->port_context;

	THREAD_EXIT();
	sendL->p_resp.p_resp_blob_id = stuff->p_slc_id;
	isc_put_slice(status_vector, &rdb->rdb_handle, &transaction->rtr_handle,
				  (ISC_QUAD*) &sendL->p_resp.p_resp_blob_id,
				  stuff->p_slc_sdl.cstr_length,
				  reinterpret_cast<char*>(stuff->p_slc_sdl.cstr_address),
				  stuff->p_slc_parameters.cstr_length,
				  (ISC_LONG *) stuff->p_slc_parameters.cstr_address,
				  stuff->p_slc_slice.lstr_length,
				  stuff->p_slc_slice.lstr_address);
	THREAD_ENTER();

	return this->send_response(sendL, 0, 0, status_vector);
}


ISC_STATUS rem_port::que_events(P_EVENT * stuff, PACKET* sendL)
{
/**************************************
 *
 *	q u e _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	RDB rdb = this->port_context;

/* Find unused event block or, if necessary, a new one */

	RVNT event;
	for (event = rdb->rdb_events; event; event = event->rvnt_next) {
		if (!event->rvnt_id) {
			break;
		}
	}

	if (!event)
	{
		event = (RVNT) ALLR_block(type_rvnt, 0);
#ifdef DEBUG_REMOTE_MEMORY
		printf("que_events(server)        allocate event   %x\n", event);
#endif
		event->rvnt_next = rdb->rdb_events;
		rdb->rdb_events = event;
	}

	event->rvnt_ast = stuff->p_event_ast;
	
	// CVC: Going from SLONG to void*, problems when sizeof(void*) > 4
	// Nickolay Samofatov. No problem actually. Event argument returned by server
	// on event firing is not used by the client. Client looks up argument via 
	// seaching client-side event table and remote event ID passed by server on
	// event registration. As a result we may use this argument value for 
	// server-side debugging only.
	event->rvnt_arg = (void*)(IPTR) stuff->p_event_arg;
	
	event->rvnt_rid = stuff->p_event_rid;
	event->rvnt_rdb = rdb;

	THREAD_EXIT();
	ISC_STATUS_ARRAY status_vector;
	isc_que_events(status_vector, &rdb->rdb_handle, &event->rvnt_id,
				   stuff->p_event_items.cstr_length,
				   reinterpret_cast<const char*>(stuff->p_event_items.cstr_address),
				   server_ast,
				   event);
	THREAD_ENTER();

	const SLONG id = event->rvnt_id;
	if (status_vector[1]) {
		event->rvnt_id = 0;
	}

	return this->send_response(sendL, (OBJCT) id, 0, status_vector);
}


ISC_STATUS rem_port::receive_after_start(	P_DATA*	data,
									PACKET*	sendL,
									ISC_STATUS*	status_vector)
{
/**************************************
 *
 *	r e c e i v e _ a f t e r _ s t a r t
 *
 **************************************
 *
 * Functional description
 *	Receive a message.
 *
 **************************************/
	rrq* requestL;

	CHECK_HANDLE_MEMBER(requestL,
						rrq*,
						type_rrq,
						data->p_data_request,
						isc_bad_req_handle);

	const USHORT level = data->p_data_incarnation;
	requestL = REMOTE_find_request(requestL, level);

/* Figure out the number of the message that we're stalled on. */

	USHORT msg_number;
	if (!get_next_msg_no(requestL, level, &msg_number)) {
		return this->send_response(sendL, 0, 0, status_vector);
	}

	sendL->p_operation = op_response_piggyback;
	P_RESP* response = &sendL->p_resp;
	response->p_resp_object = msg_number;
	response->p_resp_status_vector = status_vector;
	response->p_resp_data.cstr_length = 0;

	this->send_partial(sendL);

/* Fill in packet to fool receive into thinking that it has been
   called directly by the client. */

	const rrq::rrq_repeat* tail = requestL->rrq_rpt + msg_number;
	const rem_fmt* format = tail->rrq_format;

	data->p_data_message_number = msg_number;
	if (this->port_flags & PORT_rpc)
	{
		data->p_data_messages = 1;
	}
	else
	{
		data->p_data_messages =
			(USHORT) REMOTE_compute_batch_size(this,
											   (USHORT)
											   xdr_protocol_overhead
											   (op_response_piggyback),
											   op_send,
											   format);
	}

	return this->receive_msg(data, sendL);
}


ISC_STATUS rem_port::receive_msg(P_DATA * data, PACKET* sendL)
{
/**************************************
 *
 *	r e c e i v e _ m s g
 *
 **************************************
 *
 * Functional description
 *	Receive a message.
 *
 **************************************/

/* Find the database, request, message number, and the number of
   messages the client is willing to cope with.  Then locate the
   message control block for the request and message type. */
   
	ISC_STATUS_ARRAY status_vector;
	rrq* requestL;
	CHECK_HANDLE_MEMBER(requestL,
						rrq*,
						type_rrq,
						data->p_data_request,
						isc_bad_req_handle);

	const USHORT level = data->p_data_incarnation;
	requestL = REMOTE_find_request(requestL, level);
	const USHORT msg_number = data->p_data_message_number;
	USHORT count, count2;
	count2 = count =
		(this->port_flags & PORT_rpc) ? 1 : data->p_data_messages;
	rrq::rrq_repeat* tail = requestL->rrq_rpt + msg_number;
	const rem_fmt* format = tail->rrq_format;

/* Get ready to ship the data out */

	sendL->p_operation = op_send;
	sendL->p_data.p_data_request = data->p_data_request;
	sendL->p_data.p_data_message_number = msg_number;
	sendL->p_data.p_data_incarnation = level;
	sendL->p_data.p_data_messages = 1;

#ifdef SCROLLABLE_CURSORS
/* check the direction and offset for possible redirection; if the user 
   scrolls in a direction other than we were going or an offset other 
   than 1, then we need to resynchronize: 
   if the direction is the same as the lookahead cache, scroll forward 
   through the cache to see if we find the record; otherwise scroll the 
   next layer down backward by an amount equal to the number of records 
   in the cache, plus the amount asked for by the next level up */

	USHORT direction;
	ULONG offset;
	if (this->port_protocol < PROTOCOL_SCROLLABLE_CURSORS)
	{
		direction = blr_forward;
		offset = 1;
	}
	else
	{
		direction = data->p_data_direction;
		offset = data->p_data_offset;
		tail->rrq_xdr = scroll_cache(tail, &direction, &offset);
	}
#endif

/* Check to see if any messages are already sitting around */

	REM_MSG message = 0;

	while (true) {
		message = tail->rrq_xdr;

		/* If we don't have a message cached, get one from the next layer down. */

		if (!message->msg_address) {
			/* If we have an error queued for delivery, send it now */

			if (requestL->rrq_status_vector[1]) {
				const ISC_STATUS res =
					this->send_response(sendL, 0, 0,
								  requestL->rrq_status_vector);
				memset(requestL->rrq_status_vector, 0,
					   sizeof(requestL->rrq_status_vector));
				return res;
			}

			THREAD_EXIT();
#ifdef SCROLLABLE_CURSORS
			isc_receive2(status_vector, &requestL->rrq_handle, msg_number, 
						 format->fmt_length, message->msg_buffer, level, 
						 direction, offset);
#else
			isc_receive(status_vector, &requestL->rrq_handle, msg_number,
						format->fmt_length, message->msg_buffer, level);
#endif
			THREAD_ENTER();
			if (status_vector[1])
				return this->send_response(sendL, 0, 0, status_vector);

#ifdef SCROLLABLE_CURSORS
			/* set the appropriate flags according to the way we just scrolled 
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

			message->msg_absolute = tail->rrq_absolute;

			/* if we have already scrolled to the location indicated, 
			   then we just want to continue one by one in that direction */

			offset = 1;
#endif

			message->msg_address = message->msg_buffer;
		}

		/* If there aren't any buffers remaining, break out of loop */

		if (--count <= 0)
			break;

		/* There's a buffer waiting -- see if the request is ready to send */

		REM_MSG next = message->msg_next;

		if ((next == message || !next->msg_address) &&
			!check_request(requestL, data->p_data_incarnation, msg_number))
		{
			/* We've reached the end of the RSE - don't prefetch and flush
			   everything we've buffered so far */

			count2 = 0;
			break;
		}

		if (!this->send_partial(sendL))
			return FALSE;
		message->msg_address = NULL;
	}

	sendL->p_data.p_data_messages = 0;
	this->send(sendL);
	message->msg_address = NULL;

/* Bump up the message pointer to resync with rrq_xdr (rrq_xdr 
   was incremented by xdr_request in the SEND call).  */

	tail->rrq_message = message->msg_next;

/* Since we have a little time on our hands while this packet is sent
   and processed, get the next batch of records.  Start by finding the
   next free buffer. */

	message = tail->rrq_xdr;
	REM_MSG prior = NULL;

	while (message->msg_address && message->msg_next != tail->rrq_xdr)
		message = message->msg_next;

	for (;
		 count2
		 && check_request(requestL, data->p_data_incarnation, msg_number);
		 --count2)
	{
		if (message->msg_address) {
			if (!prior)
#ifdef SCROLLABLE_CURSORS
				prior = message->msg_prior;
#else
				for (prior = tail->rrq_xdr; prior->msg_next != message;
					 prior = prior->msg_next);
#endif

			/* allocate a new message block and put it in the cache */

			message = (REM_MSG) ALLR_block(type_msg, format->fmt_length);
#ifdef DEBUG_REMOTE_MEMORY
			printf("receive_msg(server)       allocate message %x\n",
					  message);
#endif
			message->msg_number = prior->msg_number;
			message->msg_next = prior->msg_next;
#ifdef SCROLLABLE_CURSORS
			message->msg_prior = prior;
#endif

			prior->msg_next = message;
			prior = message;
		}

		/* fetch the next record into cache; even for scrollable cursors, we are 
		   just doing a simple lookahead continuing on in the last direction specified, 
		   so there is no reason to do an isc_receive2() */

		THREAD_EXIT();
		isc_receive(status_vector, &requestL->rrq_handle, msg_number,
					format->fmt_length,
					message->msg_buffer, data->p_data_incarnation);
		THREAD_ENTER();

		/* Did we have an error?  If so, save it for later delivery */

		if (status_vector[1]) {
			/* If already have an error queued, don't overwrite it */

			if (!requestL->rrq_status_vector[1]) {
				memcpy(requestL->rrq_status_vector, status_vector,
					   sizeof(requestL->rrq_status_vector));
			}
			break;
		}

#ifdef SCROLLABLE_CURSORS
		/* if we have already scrolled to the location indicated, 
		   then we just want to continue on in that direction */

		switch (direction)
		{
		case blr_forward:
			tail->rrq_absolute +=
				(tail->rrq_flags & RRQ_absolute_backward) ? -offset : offset;
			break;

		case blr_backward:
			tail->rrq_absolute +=
				(tail->rrq_flags & RRQ_absolute_backward) ? offset : -offset;
			break;
		}

		message->msg_absolute = tail->rrq_absolute;
#endif

		message->msg_address = message->msg_buffer;
		message = message->msg_next;
	}

	return TRUE;
}


static void release_blob(RBL blob)
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
	RDB rdb = blob->rbl_rdb;
	RTR transaction = blob->rbl_rtr;

	rdb->rdb_port->port_objects[blob->rbl_id] = NULL;

	for (RBL* p = &transaction->rtr_blobs; *p; p = &(*p)->rbl_next) {
		if (*p == blob) {
			*p = blob->rbl_next;
			break;
		}
	}

#ifdef DEBUG_REMOTE_MEMORY
	printf("release_blob(server)      free blob        %x\n", blob);
#endif
	if (blob->rbl_buffer != blob->rbl_data) {
		ALLR_free(blob->rbl_buffer);
	}
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


static void release_request( rrq* request)
{
/**************************************
 *
 *	r e l e a s e _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Release a request block.
 *
 **************************************/
	RDB rdb = request->rrq_rdb;
	rdb->rdb_port->port_objects[request->rrq_id] = NULL;
	REMOTE_release_request(request);
}


static void release_statement( RSR * statement)
{
/**************************************
 *
 *	r e l e a s e _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *	Release a GDML or SQL statement?
 *
 **************************************/
	if ((*statement)->rsr_select_format)
		ALLR_release((*statement)->rsr_select_format);
	if ((*statement)->rsr_bind_format)
		ALLR_release((*statement)->rsr_bind_format);
	REMOTE_release_messages((*statement)->rsr_message);
	ALLR_release((*statement));
	*statement = NULL;
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
	rdb->rdb_port->port_objects[statement->rsr_id] = NULL;

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
	rdb->rdb_port->port_objects[transaction->rtr_id] = NULL;

	while (transaction->rtr_blobs)
		release_blob(transaction->rtr_blobs);

	for (RTR* p = &rdb->rdb_transactions; *p; p = &(*p)->rtr_next)
		if (*p == transaction) {
			*p = transaction->rtr_next;
			break;
		}

#ifdef DEBUG_REMOTE_MEMORY
	printf("release_transact(server)  free transaction %x\n", transaction);
#endif
	ALLR_release(transaction);
}


#ifdef SCROLLABLE_CURSORS
static REM_MSG scroll_cache(
						rrq::rrq_repeat* tail,
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
 *  This routine differs from the corresponding routine on the client in that 
 *  we are using only a lookahead cache.  There is no point in caching records backward, 
 *  in that the client already has them and would not request them from us. 
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

/* set to the first message; if it has no record, this means the cache is 
   empty and there is no point in trying to find the record here */

	REM_MSG message = tail->rrq_xdr;
	if (!message->msg_address)
		return message;

/* if we are scrolling from BOF and the cache was started from EOF (or vice 
   versa), the cache is unusable. */

	if (
		(*direction == blr_bof_forward
		 && (tail->rrq_flags & RRQ_absolute_backward))
		|| (*direction == blr_eof_backward
			&& !(tail->
				 rrq_flags & RRQ_absolute_backward)))
	{
		return dump_cache(tail);
	}

/* if we are going to an absolute position, see if we can find that position 
   in cache, otherwise change to a relative seek from our former position */

	if (*direction == blr_bof_forward || *direction == blr_eof_backward) {
		/* If offset is before our current position, just dump the cache because 
		   the server cache is purely a lookahead cache--we don't bother to cache 
		   back records because the client will cache those records, making it 
		   unlikely the client would be asking us for a record which is in our cache. */

		if (*offset < message->msg_absolute)
			return dump_cache(tail);

		/* convert the absolute to relative, and prepare to scroll forward to look for the record */

		*offset -= message->msg_absolute;
		if (*direction == blr_bof_forward)
			*direction = blr_forward;
		else
			*direction = blr_backward;
	}

	if ((*direction == blr_forward && (tail->rrq_flags & RRQ_backward)) ||
		(*direction == blr_backward && !(tail->rrq_flags & RRQ_backward)))
	{
		/* lookahead cache was in opposite direction from the scroll direction, 
		   so increase the scroll amount by the amount we looked ahead, then 
		   dump the cache */

		for (message = tail->rrq_xdr; message->msg_address;) {
			(*offset)++;
			message = message->msg_next;
			if (message == tail->rrq_message)
				break;
		}

		return dump_cache(tail);
	}
	else {
		/* lookahead cache is in same direction we want to scroll, so scroll 
		   forward through the cache, decrementing the offset */

		for (message = tail->rrq_xdr; message->msg_address;) {
			if (*offset == 1)
				break;
			(*offset)--;
			message = message->msg_next;
			if (message == tail->rrq_message)
				break;
		}
	}

	return message;
}
#endif


ISC_STATUS rem_port::seek_blob(P_SEEK * seek, PACKET* sendL)
{
/**************************************
 *
 *	s e e k _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Execute a blob seek operation.
 *
 **************************************/
	RBL blob;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE_MEMBER(blob,
						RBL,
						type_rbl,
						seek->p_seek_blob,
						isc_bad_segstr_handle);

	const SSHORT mode = seek->p_seek_mode;
	const SLONG offset = seek->p_seek_offset;

	THREAD_EXIT();
	SLONG result;
	isc_seek_blob(status_vector, &blob->rbl_handle, mode, offset, &result);
	THREAD_ENTER();

	sendL->p_resp.p_resp_blob_id.bid_number = result;

	return this->send_response(sendL, 0, 0, status_vector);
}


ISC_STATUS rem_port::send_msg(P_DATA * data, PACKET* sendL)
{
/**************************************
 *
 *	s e n d _ m s g
 *
 **************************************
 *
 * Functional description
 *	Handle a isc_send operation.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	rrq* requestL;
	CHECK_HANDLE_MEMBER(requestL,
						rrq*,
						type_rrq,
						data->p_data_request,
						isc_bad_req_handle);

	const USHORT number = data->p_data_message_number;
	requestL = REMOTE_find_request(requestL, data->p_data_incarnation);
	REM_MSG message = requestL->rrq_rpt[number].rrq_message;
	const rem_fmt* format = requestL->rrq_rpt[number].rrq_format;

	THREAD_EXIT();
	isc_send(status_vector, &requestL->rrq_handle, number, format->fmt_length,
			 message->msg_address, data->p_data_incarnation);
	THREAD_ENTER();

	message->msg_address = NULL;

	return this->send_response(sendL, 0, 0, status_vector);
}


ISC_STATUS rem_port::send_response(	PACKET*	sendL,
							OBJCT	object,
							USHORT	length,
							const ISC_STATUS* status_vector)
{
/**************************************
 *
 *	s e n d _ r e s p o n s e
 *
 **************************************
 *
 * Functional description
 *	Send a response packet.
 *
 **************************************/

/* Start by translating the status vector into "generic" form */

	ISC_STATUS_ARRAY new_vector;
	ISC_STATUS* v = new_vector;
	TEXT buffer[1024];
	const char* const bufferEnd = buffer + sizeof(buffer);
	TEXT* p = buffer;
	const ISC_STATUS exit_code = status_vector[1];

	for (bool sw = true; *status_vector && sw;)
	{
		switch ((USHORT) * status_vector)
		{
		case isc_arg_warning:
		case isc_arg_gds:
			{
				/* When talking with older (pre 6.0) clients, do not send
				 * warnings.
				 */

				if (*status_vector == isc_arg_warning &&
					this->port_protocol < PROTOCOL_VERSION10)
				{
					sw = false;
					continue;
				}

				*v++ = *status_vector++;

				/* The status codes are converted to their offsets so that they
				 * were compatible with the RDB implementation.  This was fine
				 * when status codes were restricted to a single facility.  Now
				 * that the facility is part of the status code we need to know
				 * this on the client side, thus when talking with 6.0 and newer
				 * clients, do not decode the status code, just send it to the
				 * client.  The same check is made in interface.cpp::check_response
				 */

				if (this->port_protocol < PROTOCOL_VERSION10) {
					USHORT fac = 0, code_class = 0;
					*v++ = gds__decode(*status_vector++, &fac, &code_class);
				}
				else {
					*v++ = *status_vector++;
				}
				for (;;) {
					switch (*status_vector) {
					case isc_arg_string:
					case isc_arg_number:
						*v++ = *status_vector++;
						*v++ = *status_vector++;
						continue;

					case isc_arg_cstring:
						{
							*v++ = isc_arg_string;
							TEXT** sp = (TEXT**) v;
							*sp++ = p;
							v = (ISC_STATUS*) sp;
							status_vector++;
							USHORT l = (USHORT) (*status_vector++);
							const TEXT* q = (TEXT*) * status_vector++;
							if (l)
								do {
									*p++ = *q++;
								} while (--l);
							*p++ = 0;
							continue;
						}
					}
					break;
				}
			}
			continue;

		case isc_arg_interpreted:
			*v++ = *status_vector++;
			*v++ = *status_vector++;
			continue;
		}
		const USHORT l = p < bufferEnd ?
			(USHORT) fb_interpret(p, bufferEnd - p, &status_vector) : 0;
		if (l == 0)
			break;

		*v++ = isc_arg_interpreted;
		TEXT** sp = (TEXT**) v;
		*sp++ = p;
		v = (ISC_STATUS *) sp;
		p += l;
		sw = false;
	}

	*v = isc_arg_end;

/* Format and send response.  Note: the blob_id and data address fields
   of the response packet may contain valid data.  Don't trash them. */

	sendL->p_operation = op_response;

	P_RESP* response = &sendL->p_resp;
	response->p_resp_object = object;
	response->p_resp_status_vector = new_vector;
	response->p_resp_data.cstr_length = length;

	this->send(sendL);

	return exit_code;
}


static void server_ast(void* event_void, USHORT length, const UCHAR* items)
{
/**************************************
 *
 *	s e r v e r _ a s t
 *
 **************************************
 *
 * Functional description
 *	Send an asynchrous event packet back to client.
 *
 **************************************/
	RVNT event = static_cast<rvnt*>(event_void);

	THREAD_ENTER();
	event->rvnt_id = 0;
	RDB rdb = event->rvnt_rdb;

	rem_port* port = rdb->rdb_port->port_async;
	if (!port) {
		THREAD_EXIT();
		return;
	}

	PACKET packet;
	packet.p_operation = op_event;
	P_EVENT* p_event = &packet.p_event;
	p_event->p_event_database = rdb->rdb_id;
	p_event->p_event_items.cstr_length = length;
	// Probalby should define this item with CSTRING_CONST instead.
	p_event->p_event_items.cstr_address = const_cast<UCHAR*>(items);
	
	// Nickolay Samofatov: We keep these values and even pass them to the client
	// (as 32-bit values) when event is fired, but client ignores them.
	p_event->p_event_ast = event->rvnt_ast;
	p_event->p_event_arg = (SLONG)(IPTR) event->rvnt_arg;
	
	p_event->p_event_rid = event->rvnt_rid;

	port->send(&packet);
	THREAD_EXIT();
}


ISC_STATUS rem_port::service_attach(P_ATCH* attach, PACKET* sendL)
{
/**************************************
 *
 *	s e r v i c e _ a t t a c h 
 *
 **************************************
 *
 * Functional description
 *	Connect to an Interbase service.
 *
 **************************************/
	sendL->p_operation = op_accept;
	FB_API_HANDLE handle = 0;
	const UCHAR* service_name = attach->p_atch_file.cstr_address;
	const USHORT service_length = attach->p_atch_file.cstr_length;
	const UCHAR* spb = attach->p_atch_dpb.cstr_address;
	USHORT spb_length = attach->p_atch_dpb.cstr_length;

/* If we have user identification, append it to database parameter block */

	UCHAR new_spb_buffer[512];
	UCHAR* new_spb = new_spb_buffer;
	const rem_str* string = this->port_user_name;
	if (string)
	{
		if ((spb_length + 3 + string->str_length) > (int)sizeof(new_spb_buffer))
			new_spb =
				ALLR_alloc((SLONG) (spb_length + 3 + string->str_length));
		UCHAR* p = new_spb;
		if (spb_length) {
			for (const UCHAR* const end = spb + spb_length; spb < end;)
			{
				*p++ = *spb++;
			}
		}
		else {
			*p++ = isc_spb_current_version;
		}

		*p++ = isc_spb_sys_user_name;
		*p++ = (UCHAR) string->str_length;
		spb = (const UCHAR*) string->str_data;
		for (const UCHAR* const end = spb + string->str_length;
			 spb < end;)
		{
			*p++ = *spb++;
		}
		spb = new_spb;
		spb_length = p - new_spb;
	}

/* See if user has specified parameters relevent to the connection,
   they will be stuffed in the SPB if so. */
	REMOTE_get_timeout_params(this, spb, spb_length);

	THREAD_EXIT();
	ISC_STATUS_ARRAY status_vector;
	isc_service_attach(status_vector,
					   service_length,
					   reinterpret_cast<const char*>(service_name),
					   &handle,
					   spb_length,
					   reinterpret_cast<const char*>(spb));
	THREAD_ENTER();

	if (new_spb != new_spb_buffer)
		ALLR_free(new_spb);

	if (!status_vector[1]) {
		RDB rdb = (RDB) ALLR_block(type_rdb, 0);
		this->port_context = rdb;
#ifdef DEBUG_REMOTE_MEMORY
		printf("attach_service(server)  allocate rdb     %x\n", rdb);
#endif
		rdb->rdb_port = this;
		rdb->rdb_handle = handle;
		rdb->rdb_flags |= RDB_service;
	}

	return this->send_response(sendL, 0, 0, status_vector);
}


ISC_STATUS rem_port::service_end(P_RLSE * release, PACKET* sendL)
{
/**************************************
 *
 *	s e r v i c e _ e n d
 *
 **************************************
 *
 * Functional description
 *	Close down a service.
 *
 **************************************/
	RDB rdb = this->port_context;

	ISC_STATUS_ARRAY status_vector;
	
	THREAD_EXIT();
	isc_service_detach(status_vector, &rdb->rdb_handle);
	THREAD_ENTER();

	return this->send_response(sendL, 0, 0, status_vector);
}


ISC_STATUS rem_port::service_start(P_INFO * stuff, PACKET* sendL)
{
/**************************************
 *
 *	s e r v i c e _ s t a r t
 *
 **************************************
 *
 * Functional description
 *	Start a service on the server
 *
 **************************************/
	RDB rdb = this->port_context;

	ISC_STATUS_ARRAY status_vector;
	
	THREAD_EXIT();
	SLONG* reserved = 0;		// reserved for future use
	isc_service_start(status_vector,
					  &rdb->rdb_handle,
					  reserved,
					  stuff->p_info_items.cstr_length,
					  reinterpret_cast<char*>(stuff->p_info_items.cstr_address));
	THREAD_ENTER();

	return this->send_response(sendL, 0, 0, status_vector);
}


ISC_STATUS rem_port::set_cursor(P_SQLCUR * sqlcur, PACKET* sendL)
{
/*****************************************
 *
 *	s e t _ c u r s o r
 *
 *****************************************
 *
 * Functional description
 *	Set a cursor name for a dynamic request.
 *
 *****************************************/
	RSR statement;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE_MEMBER(statement,
						RSR,
						type_rsr,
						sqlcur->p_sqlcur_statement,
						isc_bad_req_handle);

	THREAD_EXIT();
	GDS_DSQL_SET_CURSOR(status_vector,
						&statement->rsr_handle,
						reinterpret_cast<const char*>(sqlcur->p_sqlcur_cursor_name.cstr_address),
						sqlcur->p_sqlcur_type);
	THREAD_ENTER();

	return this->send_response(sendL, 0, 0, status_vector);
}


void set_server( rem_port* port, USHORT flags)
{
/**************************************
 *
 *	s e t _ s e r v e r
 *
 **************************************
 *
 * Functional description
 *	Look up the server for this type
 *	of port.  If one doesn't exist,
 *	create it.
 *
 **************************************/
	SRVR server;

	for (server = servers; server; server = server->srvr_next) {
		if (port->port_type == server->srvr_port_type) {
			break;
		}
	}

	if (!server) {
		server = (SRVR) ALLR_alloc((SLONG) sizeof(struct srvr));
		server->srvr_next = servers;
		servers = server;
		server->srvr_port_type = port->port_type;
		server->srvr_parent_port = port;
		server->srvr_flags = flags;
	}

	port->port_server = server;
}


ISC_STATUS rem_port::start(P_OP operation, P_DATA * data, PACKET* sendL)
{
/**************************************
 *
 *	s t a r t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	RTR transaction;

	CHECK_HANDLE_MEMBER(transaction,
						RTR,
						type_rtr,
						data->p_data_transaction,
						isc_bad_trans_handle);

	rrq* requestL;
	CHECK_HANDLE_MEMBER(requestL,
						rrq*,
						type_rrq,
						data->p_data_request,
						isc_bad_req_handle);

	requestL = REMOTE_find_request(requestL, data->p_data_incarnation);
	REMOTE_reset_request(requestL, 0);

	THREAD_EXIT();
	isc_start_request(status_vector, &requestL->rrq_handle,
					  &transaction->rtr_handle, data->p_data_incarnation);
	THREAD_ENTER();

	if (!status_vector[1]) {
		requestL->rrq_rtr = transaction;
		if (operation == op_start_and_receive)
			return this->receive_after_start(data, sendL, status_vector);
	}

	return this->send_response(sendL, 0, 0, status_vector);
}


ISC_STATUS rem_port::start_and_send(P_OP	operation,
							P_DATA*	data,
							PACKET*	sendL)
{
/**************************************
 *
 *	s t a r t _ a n d _ s e n d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
    ISC_STATUS_ARRAY status_vector;
	RTR transaction;
	CHECK_HANDLE_MEMBER(transaction,
						RTR,
						type_rtr,
						data->p_data_transaction,
						isc_bad_trans_handle);

	rrq* requestL;
	CHECK_HANDLE_MEMBER(requestL,
						rrq*,
						type_rrq,
						data->p_data_request,
						isc_bad_req_handle);

	requestL = REMOTE_find_request(requestL, data->p_data_incarnation);
	const USHORT number = data->p_data_message_number;
	REM_MSG message = requestL->rrq_rpt[number].rrq_message;
	const rem_fmt* format = requestL->rrq_rpt[number].rrq_format;
	REMOTE_reset_request(requestL, message);


	THREAD_EXIT();
	isc_start_and_send(status_vector, &requestL->rrq_handle,
					   &transaction->rtr_handle, number,
					   format->fmt_length, message->msg_address,
					   data->p_data_incarnation);
	THREAD_ENTER();

	if (!status_vector[1]) {
		requestL->rrq_rtr = transaction;
		if (operation == op_start_send_and_receive) {
			return this->receive_after_start(data, sendL, status_vector);
		}
	}

	return this->send_response(sendL, 0, 0, status_vector);
}


ISC_STATUS rem_port::start_transaction(P_OP operation, P_STTR * stuff, PACKET* sendL)
{
/**************************************
 *
 *	s t a r t _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Start a transaction.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	RDB rdb = this->port_context;
	FB_API_HANDLE handle = 0;

	THREAD_EXIT();
	if (operation == op_reconnect)
		isc_reconnect_transaction(status_vector, &rdb->rdb_handle, &handle,
								  stuff->p_sttr_tpb.cstr_length,
								  reinterpret_cast<const char*>(stuff->p_sttr_tpb.cstr_address));
	else
		isc_start_transaction(status_vector, &handle, (SSHORT) 1, &rdb->rdb_handle,
							  stuff->p_sttr_tpb.cstr_length,
							  stuff->p_sttr_tpb.cstr_address);
	THREAD_ENTER();

	OBJCT object;
	if (status_vector[1])
	{
		object = 0;
	}
	else
	{
		RTR transaction = make_transaction(rdb, handle);
		if (transaction)
		{
			object = transaction->rtr_id;
			if (operation == op_reconnect)
				transaction->rtr_flags |= RTR_limbo;
#ifdef DEBUG_REMOTE_MEMORY
			printf("start_transaction(server) allocate trans   %x\n",
					  transaction);
#endif
		}
		else {
			object = 0;
			THREAD_EXIT();
			if (operation != op_reconnect)
				isc_rollback_transaction(status_vector, &handle);
#ifdef SUPERSERVER
			/* Note that there is an underlying transaction pool
			   that won't be released until this connection is
			   detached. There is no isc_disconnect_transaction()
			   call that gets rid of a transaction reference --
			   there is only commit and rollback. It's not worth
			   introducing such a call to all subsystems. At least
			   release the y-valve handle. */

			else {
				gds__handle_cleanup(status_vector, &handle);
			}
#endif
			THREAD_ENTER();
			status_vector[0] = isc_arg_gds;
			status_vector[1] = isc_too_many_handles;
			status_vector[2] = isc_arg_end;
		}
	}

	return this->send_response(sendL, object, 0, status_vector);
}


static void success( ISC_STATUS * status_vector)
{
/**************************************
 *
 *	s u c c e s s
 *
 **************************************
 *
 * Functional description
 *	Set status vector to indicate success.
 *
 **************************************/
	status_vector[0] = isc_arg_gds;
	status_vector[1] = FB_SUCCESS;
	status_vector[2] = isc_arg_end;
}

#ifdef MULTI_THREAD
static THREAD_ENTRY_DECLARE loopThread(THREAD_ENTRY_PARAM flags)
{
/**************************************
 *
 *	t h r e a d
 *
 **************************************
 *
 * Functional description
 *	Execute requests in a happy loop.
 *
 **************************************/
#ifdef SUPERSERVER
	ISC_enter();				/* Setup floating point exception handler once and for all. */
#endif

#ifdef WIN_NT
	void* thread = NULL; // silence non initialized warning
	if (!((SLONG) flags & SRVR_non_service))
		thread = CNTL_insert_thread();
#endif

	USHORT inactive_count = 0;
	USHORT timedout_count = 0;
	THREAD_ENTER();

	rem_port* port;

	for (;;)
	{
		SERVER_REQ request = request_que;
		if (request)
		{
			inactive_count = 0;
			timedout_count = 0;
			REMOTE_TRACE(("Dequeue request %p", request_que));
			request_que = request->req_next;
			while (request)
			{
				/* Bind a thread to a port. */

				if (request->req_port->port_server_flags & SRVR_thread_per_port)
				{
					port = request->req_port;
					request->req_next = free_requests;
					free_requests = request;
					THREAD_EXIT();
					SRVR_main(port, port->port_server_flags);
					THREAD_ENTER();
					request = 0;
					continue;
				}
				/* Splice request into list of active requests, execute request,
				   and unsplice */

				request->req_next = active_requests;
				active_requests = request;

				/* Validate port.  If it looks ok, process request */

				if (request->req_port->port_state != state_disconnected)
				{
					rem_port* parent_port =
						request->req_port->port_server->srvr_parent_port;
					if (parent_port == request->req_port)
					{
						process_packet(parent_port, &request->req_send,
									   &request->req_receive, &port);
					}
					else
					{
						for (port = parent_port->port_clients; port;
							 port = port->port_next)
						{
							if (port == request->req_port)
							{
								process_packet(port, &request->req_send,
											   &request->req_receive, &port);
								break;
							}
						}
					}
				}
				else
				{
					port = NULL;
				}

				/* Take request out of list of active requests */

				for (SERVER_REQ* req_ptr = &active_requests; *req_ptr;
					 req_ptr = &(*req_ptr)->req_next)
				{
					if (*req_ptr == request) {
						*req_ptr = request->req_next;
						break;
					}
				}

				/* If this is a explicit or implicit disconnect, get rid of
				   any chained requests */

				if (!port)
				{
					SERVER_REQ next;
					while (next = request->req_chain) {
						request->req_chain = next->req_chain;
						next->req_next = free_requests;
						free_requests = next;
					}
					if (request->req_send.p_operation == op_void &&
						request->req_receive.p_operation == op_void)
					{
						gds__free(request);
						request = 0;
					}
				}
				else {
					port->port_requests_queued--;
#ifdef DEBUG_REMOTE_MEMORY
					printf("thread    ACTIVE     request_queued %d\n",
							  port->port_requests_queued);
					fflush(stdout);
#endif
				}

				/* Pick up any remaining chained request, and free current request */

				if (request) {
					SERVER_REQ next = request->req_chain;
					request->req_next = free_requests;
					free_requests = request;
					request = next;
				}
			}	// while (request)
		}
		else {
			inactive_count++;
			/* If the main server thread is not relying on me to take on a new request
			   and I have been idling for a while, it is time to quit and thereby
			   release some valuable system resources like memory */
			if ((extra_threads > 1)
				&& ((inactive_count > 20) || (timedout_count > 2)))
			{
				extra_threads--;	/* Count me out */
				break;
			}

			threads_waiting++;
			THREAD_EXIT();
			/* Wait for 1 minute (60 seconds) on a new request */
			REMOTE_TRACE(("Wait for event"));
			if (!requests_semaphore.tryEnter(60)) {
				REMOTE_TRACE(("timeout!"));
				timedout_count++;
			}
			else {
				REMOTE_TRACE(("got it"));
			}
			THREAD_ENTER();
			--threads_waiting;
		}
	}

	THREAD_EXIT();

#ifdef WIN_NT
	if (!((SLONG) flags & SRVR_non_service))
		CNTL_remove_thread(thread);
#endif

	return 0;
}
#endif


ISC_STATUS rem_port::transact_request(P_TRRQ* trrq, PACKET* sendL)
{
/**************************************
 *
 *	t r a n s a c t _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	RTR transaction;

	CHECK_HANDLE_MEMBER(transaction,
						RTR,
						type_rtr,
						trrq->p_trrq_transaction,
						isc_bad_trans_handle);

	RDB rdb = this->port_context;
	const UCHAR* blr = trrq->p_trrq_blr.cstr_address;
	const USHORT blr_length = trrq->p_trrq_blr.cstr_length;
	RPR procedure = this->port_rpr;
	UCHAR* in_msg =
		(procedure->rpr_in_msg) ? procedure->rpr_in_msg->msg_address : NULL;
	const USHORT in_msg_length =
		(procedure->rpr_in_format) ? procedure->rpr_in_format->fmt_length : 0;
	UCHAR* out_msg =
		(procedure->rpr_out_msg) ? procedure->rpr_out_msg->msg_address : NULL;
	const USHORT out_msg_length =
		(procedure->rpr_out_format) ? procedure->rpr_out_format->
		fmt_length : 0;

	THREAD_EXIT();
	isc_transact_request(status_vector,
						 &rdb->rdb_handle,
						 &transaction->rtr_handle,
						 blr_length,
						 reinterpret_cast<const char*>(blr),
						 in_msg_length,
						 reinterpret_cast<char*>(in_msg),
						 out_msg_length,
						 reinterpret_cast<char*>(out_msg));
	THREAD_ENTER();

	if (status_vector[1])
		return this->send_response(sendL, 0, 0, status_vector);

	P_DATA* data = &sendL->p_data;
	sendL->p_operation = op_transact_response;
	data->p_data_messages = 1;
	this->send(sendL);

	return FB_SUCCESS;
}


static void zap_packet(PACKET* packet,
					   bool new_packet)
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

	if (new_packet)
		memset(packet, 0, sizeof(PACKET));
	else {
#ifdef DEBUG_XDR_MEMORY
		/* Don't trash debug xdr memory allocation table of recycled packets. */

		memset(&packet->p_operation, 0,
			   sizeof(PACKET) - OFFSET(PACKET*, p_operation));
#else
		memset(packet, 0, sizeof(PACKET));
#endif
	}
}

