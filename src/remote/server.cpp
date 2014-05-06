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
#include "../common/classes/timestamp.h"
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
#include "../jrd/constants.h"
#include "../common/classes/semaphore.h"
#include "../common/classes/ClumpletWriter.h"
#include "../common/config/config.h"
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

#define CHECK_HANDLE(blk, cast, type, id, err)							\
	{																\
		if ((port->port_flags & PORT_lazy) && id == INVALID_OBJECT)	\
		{															\
			id = port->port_last_object_id;							\
		}															\
		if (!port->port_objects ||									\
			id >= port->port_object_vector->vec_count ||			\
			!(blk = (cast) port->port_objects [id]) ||				\
			((BLK) blk)->blk_type != (UCHAR) type)					\
		{															\
			status_vector [0] = isc_arg_gds;						\
			status_vector [1] = err;								\
			status_vector [2] = isc_arg_end;						\
			return port->send_response(send, 0, 0, status_vector,	\
									   false);						\
		}															\
	}

#define CHECK_HANDLE_MEMBER(blk, cast, type, id, err)					\
	{																\
		if ((this->port_flags & PORT_lazy) && id == INVALID_OBJECT)	\
		{															\
			id = this->port_last_object_id;							\
		}															\
		if (!this->port_objects ||									\
			id >= this->port_object_vector->vec_count ||			\
			!(blk = (cast) this->port_objects [id]) ||				\
			((BLK) blk)->blk_type != (UCHAR) type)					\
		{															\
			status_vector [0] = isc_arg_gds;						\
			status_vector [1] = err;								\
			status_vector [2] = isc_arg_end;						\
			return this->send_response(sendL, 0, 0, status_vector,	\
									   false);						\
		}															\
	}


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

namespace {
	// this sets of parameters help use same functions
	// for both services and databases attachments
	struct ParametersSet {
		UCHAR address_path;
	};
	const ParametersSet dpbParam = {isc_dpb_address_path};
	const ParametersSet spbParam = {isc_spb_address_path};
}

#ifdef SUPERSERVER
static void		free_request(SERVER_REQ);
static SERVER_REQ alloc_request();
static bool		link_request(SERVER_REQ, rem_port*, SERVER_REQ, const char *);
#endif

static bool		accept_connection(rem_port*, P_CNCT*, PACKET*);
static ISC_STATUS	allocate_statement(rem_port*, P_RLSE*, PACKET*);
#ifdef MULTI_THREAD
static SLONG	append_request_chain(SERVER_REQ, SERVER_REQ*);
static SLONG	append_request_next(SERVER_REQ, SERVER_REQ*);
#endif
static void		attach_database(rem_port*, P_OP, P_ATCH*, PACKET*);
static void		attach_service(rem_port*, P_ATCH*, PACKET*);
static void		attach_database2(rem_port*, P_OP, const char*, int, 
								 const UCHAR*, int, PACKET*);
static void		attach_service2(rem_port*, P_OP, const char*, int, 
								const UCHAR*, int, PACKET*);
#ifdef TRUSTED_AUTH
static void		trusted_auth(rem_port*, P_TRAU*, PACKET*);
static bool		canUseTrusted();
#endif

#ifdef NOT_USED_OR_REPLACED
static void		aux_connect(rem_port*, P_REQ*, PACKET*);
#endif
static void		aux_request(rem_port*, P_REQ*, PACKET*);
static ISC_STATUS	cancel_events(rem_port*, P_EVENT*, PACKET*);
static void		addClumplets(Firebird::ClumpletWriter&, const ParametersSet&, const rem_port*);

#ifdef CANCEL_OPERATION
static void		cancel_operation(rem_port*);
#endif

static bool		check_request(rrq*, USHORT, USHORT);
static USHORT	check_statement_type(RSR);

#ifdef SCROLLABLE_CURSORS
static REM_MSG	dump_cache(rrq::rrq_repeat*);
#endif

static bool		get_next_msg_no(rrq*, USHORT, USHORT*);
static RTR		make_transaction(RDB, FB_API_HANDLE);
static bool		process_packet2(rem_port* port,
								PACKET* sendL,
								PACKET* receive,
								rem_port** result);

static void		release_blob(RBL);
static void		release_event(RVNT);
static void		release_request(rrq*);
static void		release_statement(RSR*);
static void		release_sql_request(RSR);
static void		release_transaction(RTR);

#ifdef SCROLLABLE_CURSORS
static REM_MSG	scroll_cache(rrq::rrq_repeat*, USHORT *, ULONG *);
#endif

static void		server_ast(void*, USHORT, const UCHAR*);
static void		success(ISC_STATUS*);
#ifdef MULTI_THREAD
static THREAD_ENTRY_DECLARE loopThread(THREAD_ENTRY_PARAM);
#endif
static void		zap_packet(PACKET*, bool);

static bool		bad_port_context(ISC_STATUS*, RDB, const ISC_LONG);


inline bool bad_db(ISC_STATUS* status_vector, RDB rdb)
{
	return bad_port_context(status_vector, rdb, isc_bad_db_handle);
}

inline bool bad_service(ISC_STATUS* status_vector, RDB rdb)
{
	return bad_port_context(status_vector, rdb, isc_bad_svc_handle);
}


// static data - NOT THREAD SAFE!

static SLONG		threads_waiting		= 0;
static SLONG		threads_total		= 0;
static SLONG		extra_threads		= 0;
static SERVER_REQ	request_que			= NULL;
static SERVER_REQ	free_requests		= NULL;
static SERVER_REQ	active_requests		= NULL;
static bool			shutting_down		= false;
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
		if (!process_packet(port, &send, &receive, &port)) {
			break;
		}
	}

	THREAD_EXIT();
}


#ifdef SUPERSERVER
static void free_request(SERVER_REQ request)
{
/**************************************
 *
 *	f r e e  _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	request->req_next = free_requests;
	free_requests = request;
}


static SERVER_REQ alloc_request()
{
/**************************************
 *
 *	a l l o c _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Get request block from the free blocks list,
 *	if empty - allocate the new one.
 *
 **************************************/
	SERVER_REQ request = free_requests;
#if defined(DEV_BUILD) && defined(DEBUG)
	int request_count = 0;
#endif
	
	/* Allocate a memory block to store the request in */
	if (request)
	{
		free_requests = request->req_next;
	}
	else
	{
		/* No block on the free list - allocate some new memory */

		while (!(request = (SERVER_REQ) gds__alloc((SLONG) sizeof(struct server_req_t))))
		{
#if defined(DEV_BUILD) && defined(DEBUG)
			if (request_count++ > 4)
				Firebird::BadAlloc::raise();
#endif

			/* System is out of memory, let's delay processing this
			   request and hope another thread will free memory or
			   request blocks that we can then use. */

			THREAD_EXIT();
			THREAD_SLEEP(1 * 1000);
			THREAD_ENTER();
		}
		zap_packet(&request->req_send, true);
		zap_packet(&request->req_receive, true);
#ifdef DEBUG_REMOTE_MEMORY
		printf("alloc_request         allocate request %x\n", request);
#endif
	}
	
	request->req_next = NULL;
	request->req_chain = NULL;
	return request;
}


static bool link_request(SERVER_REQ queue, 
						 rem_port* port, 
						 SERVER_REQ request, 
						 const char *
#ifdef DEBUG_REMOTE_MEMORY
									 kind
#endif
)
{
/**************************************
 *
 *	l i n k _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Search for a port in a queue, 
 *	if found - append new request to it.
 *
 **************************************/
	for (; queue; queue = queue->req_next) 
	{
		if (queue->req_port == port) 
		{
		 	P_OP operation = request->req_receive.p_operation;

			// Don't queue a dummy keepalive packet if there is a request on this port
			if (operation == op_dummy) {
				free_request(request);
				return true;
			}

			port->port_requests_queued++;
			append_request_chain(request, &queue->req_chain);
#ifdef DEBUG_REMOTE_MEMORY
			printf
				("link_request    %s    request_queued %d\n",
				 kind, port->port_requests_queued);
			 fflush(stdout);
#endif
#ifdef CANCEL_OPERATION
			if (operation == op_exit || operation == op_disconnect)
				cancel_operation(port);
#endif
			return true;
		}
	}

	return false;
}
#endif //SUPERSERVER


void SRVR_multi_thread( rem_port* main_port, USHORT flags)
{
#ifdef SUPERSERVER
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
	SERVER_REQ request = 0;
	rem_port* port = 0;		// Was volatile PORT port = NULL;
	SLONG pending_requests;
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

			// When this loop exits, the server will no longer receive requests
			while (true)
			{
				// We have a request block - now get some information to stick into it
				const size_t MAX_PACKET_SIZE = 32767;
				UCHAR buffer[MAX_PACKET_SIZE];
				SSHORT dataSize = main_port->port_buff_size > sizeof(buffer) ? 
								  sizeof(buffer) : main_port->port_buff_size;
				const bool ok = main_port->select_multi(buffer, dataSize, &dataSize, port);
				if (!port)
				{
					gds__log("SRVR_multi_thread/RECEIVE: error on main_port, shutting down");
					THREAD_EXIT();
					REM_restore_thread_data();
					return;
				}
				if (dataSize)
				{
					memcpy(port->port_queue->add().getBuffer(dataSize), buffer, dataSize);
				}

				if (!(port->port_flags & PORT_busy) && !port->port_requests_queued || !dataSize)
				{
					// Allocate a memory block to store the request in
					request = alloc_request();

					if (dataSize) 
					{
						const rem_port::RecvQueState recvState = port->getRecvState();
						port->receive(&request->req_receive);
						if (request->req_receive.p_operation == op_partial)
						{
							port->setRecvState(recvState);

							free_request(request);
							request = 0;
							port = 0;
							continue;
						}
					}
					else
					{
						request->req_receive.p_operation = ok ? op_dummy : op_exit;
					}

					if (!port->haveRecvData())
						port->clearRecvQue();

					request->req_port = port;

					// If port has an active request, link this one in
					if ((!link_request(active_requests, port, request, "ACTIVE ")) &&
						// If port has a pending request, link this one in
						(!link_request(request_que, port, request, "PENDING")))
					{
						/* No port to assign request to, add it to the waiting queue and wake up a
						* thread to handle it 
						*/
						REMOTE_TRACE(("Enqueue request %p", request));
						pending_requests = append_request_next(request, &request_que);
						request = 0;
						port->port_requests_queued++;
#ifdef DEBUG_REMOTE_MEMORY
						printf
							("SRVR_multi_thread    APPEND_PENDING     request_queued %d\n",
							port->port_requests_queued);
						fflush(stdout);
#endif

						/* 
						* NOTE: we really *should* have something that limits how many
						* total threads we allow in the system.  As each thread will
						* eat up memory that other threads could use to complete their work
						*
						* NOTE: The setting up of extra_threads variable is done below to let waiting
						* threads know if their services may be needed for the current set
						* of requests.  Otherwise, some idle threads may leave the system 
						* freeing up valuable memory.
						*/
						extra_threads = threads_waiting - pending_requests;
						if (extra_threads < 0) {
							if (gds__thread_start(loopThread, (void*)(IPTR) flags, THREAD_medium,
												  THREAD_ast, 0) != 0)
							{
								if (!threads_total)
								{
									gds__log("Could not start first worker thread - shutdown");
									break;
								}
							}
						}

						REMOTE_TRACE(("Post event"));
						requests_semaphore.release();
					}
					request = 0;
				}

				port = 0;
			}

		}
		catch (const Firebird::Exception& e)
		{
			/* If we got as far as having a port allocated before the error, disconnect it
			 * gracefully.
			 */
			if (port != NULL)
			{
/*
#if defined(DEV_BUILD) && defined(DEBUG)
			ConsolePrintf("%%ISERVER-F-NOPORT, no port in a storm\r\n");
#endif 
*/
				gds__log("SRVR_multi_thread: forcefully disconnecting a port");

				/* To handle recursion within the error handler */
				try {
					Firebird::stuff_exception(status_vector, e);
					/* If we have a port, request really should be non-null, but just in case ... */
					if (request != NULL) {
						/* Send client a real status indication of why we disconnected them */
						/* Note that send_response() can post errors that wind up in this same handler */
/*
#if defined(DEV_BUILD) && defined(DEBUG)
						ConsolePrintf
							("%%ISERVER-F-NOMEM, virtual memory exhausted\r\n");
#endif 
*/
						port->send_response(&request->req_send, 0, 0,
								  status_vector, false);
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
				catch (const Firebird::Exception&) {
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

	}
	catch (const Firebird::Exception&) {
		/* Some kind of unhandled error occured during server setup.  In lieu
		 * of anything we CAN do, log something (and we might be so hosed
		 * we can't log anything) and give up.
		 * The likely error here is out-of-memory.
		 */
		gds__log("SRVR_multi_thread: error during startup, shutting down");
	}

	THREAD_EXIT();
	REM_restore_thread_data();
#endif //SUPERSERVER
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

	// Accept the physical connection
	send->p_operation = op_reject;
	P_ACPT* accept = &send->p_acpt;

	if (!port->accept(connect)) {
		port->send(send);
		return false;
	}


	// Select the most appropriate protocol (this will get smarter)
	P_ARCH architecture = arch_generic;
	USHORT version = 0;
	USHORT type = 0;
	bool accepted = false;
	USHORT weight = 0;
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
			 protocol->p_cnct_version == PROTOCOL_VERSION10 ||
			 protocol->p_cnct_version == PROTOCOL_VERSION11
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
			type = MIN(protocol->p_cnct_max_type, ptype_lazy_send);
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

	Firebird::string buffer;
	buffer.printf("%s/P%d", port->port_version->str_data,
							port->port_protocol & FB_PROTOCOL_MASK);
	ALLR_free(port->port_version);
	port->port_version = REMOTE_make_string(buffer.c_str());

	if (architecture == ARCHITECTURE)
		port->port_flags |= PORT_symmetric;

	if (type == ptype_rpc)
		port->port_flags |= PORT_rpc;

	if (type != ptype_out_of_band)
		port->port_flags |= PORT_no_oob;

	if (type == ptype_lazy_send)
		port->port_flags |= PORT_lazy;

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
	
	if (bad_db(status_vector, rdb))
	{
		return port->send_response(send, 0, 0, status_vector, true);
	}
	
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

	return port->send_response(send, object, 0, status_vector, true);
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


static void addClumplets(Firebird::ClumpletWriter& dpb_buffer, 
						 const ParametersSet& par, 
						 const rem_port* port)
{
/**************************************
 *
 *	a d d C l u m p l e t s
 *
 **************************************
 *
 * Functional description
 *	Insert remote endpoint data into DPB address stack
 *  If configured, insert remote_attachment into dpb
 *
 **************************************/
	Firebird::ClumpletWriter address_stack_buffer(Firebird::ClumpletReader::UnTagged, MAX_UCHAR - 2);
	if (dpb_buffer.find(par.address_path)) {
		address_stack_buffer.reset(dpb_buffer.getBytes(), dpb_buffer.getClumpLength());
		dpb_buffer.deleteClumplet();
	}

	Firebird::ClumpletWriter address_record(Firebird::ClumpletReader::UnTagged, MAX_UCHAR - 2);
	if (port->port_protocol_str)
		address_record.insertString(isc_dpb_addr_protocol, 
			port->port_protocol_str->str_data, port->port_protocol_str->str_length);
	if (port->port_address_str)
		address_record.insertString(isc_dpb_addr_endpoint, 
			port->port_address_str->str_data, port->port_address_str->str_length);

	// We always insert remote address descriptor as a first element
	// of appropriate clumplet so user cannot fake it and engine may somewhat trust it.
	fb_assert(address_stack_buffer.getCurOffset() == 0);
	address_stack_buffer.insertBytes(isc_dpb_address, 
		address_record.getBuffer(), address_record.getBufferLength());

	dpb_buffer.insertBytes(par.address_path, address_stack_buffer.getBuffer(), 
								address_stack_buffer.getBufferLength());

	// Remove all remaining isc_*pb_address_path clumplets. 	
	// This is the security feature to prevent user from faking remote address
	// by passing multiple address_path clumplets. Engine assumes that
	// dpb contains no more than one address_path clumplet and for 
	// clients coming via remote interface it can trust the first address from 
	// address stack. Clients acessing database directly can do whatever they 
	// want with the engine including faking the source address, not much we 
	// can do about it.

	while (!dpb_buffer.isEof()) {
		if (dpb_buffer.getClumpTag() == par.address_path)
			dpb_buffer.deleteClumplet();
		else
			dpb_buffer.moveNext();
	}
}


static void attach_database(rem_port* port,
							P_OP operation, 
							P_ATCH * attach, 
							PACKET* send)
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
	const char* file = reinterpret_cast<const char*>(attach->p_atch_file.cstr_address);
	const USHORT l = attach->p_atch_file.cstr_length;

	const UCHAR* dpb = attach->p_atch_dpb.cstr_address;
	USHORT dl = attach->p_atch_dpb.cstr_length;

	Firebird::ClumpletWriter dpb_buffer(Firebird::ClumpletReader::Tagged, MAX_SSHORT);

	if (dl)
		dpb_buffer.reset(dpb, dl);
	else
		dpb_buffer.reset(isc_dpb_version1);

#ifdef TRUSTED_AUTH
	// Do we need trusted authentication?
	if (dpb_buffer.find(isc_dpb_trusted_auth))
	{
		try 
		{
			// extract trusted authentication data from dpb
			AuthSspi::DataHolder data;
			memcpy(data.getBuffer(dpb_buffer.getClumpLength()), 
				dpb_buffer.getBytes(), dpb_buffer.getClumpLength());
			dpb_buffer.deleteClumplet();

			// remove extra trusted_auth if present (security measure)
			dpb_buffer.deleteWithTag(isc_dpb_trusted_auth);

			if (canUseTrusted() && port->port_protocol >= PROTOCOL_VERSION11)
			{
				port->port_trusted_auth = FB_NEW(*getDefaultMemoryPool()) 
					ServerAuth(file, l, dpb_buffer, attach_database2, operation);
				AuthSspi* authSspi = port->port_trusted_auth->authSspi;

				if (authSspi->accept(data) && authSspi->isActive())
				{
					send->p_operation = op_trusted_auth;
					cstring& s = send->p_trau.p_trau_data;
					s.cstr_allocated = 0;
					s.cstr_length = data.getCount();
					s.cstr_address = data.begin();
					port->send(send);
					return;
				}
			}
		}
		catch(const Firebird::status_exception& e)
		{
			ISC_STATUS_ARRAY status_vector;
			Firebird::stuff_exception(status_vector, e);
			delete port->port_trusted_auth;
			port->port_trusted_auth = 0;
			port->send_response(send, 0, 0, status_vector, false);
			return;
		}
	}
#endif //TRUSTED_AUTH
	
	attach_database2(port, operation, file, l, dpb_buffer.getBuffer(), 
		dpb_buffer.getBufferLength(), send);
}


static void attach_database2(rem_port* port,
							 P_OP operation,
							 const char* file, 
							 int l, 
							 const UCHAR* dpb, 
							 int dl, 
							 PACKET* send)
{
	ISC_STATUS_ARRAY status_vector;
    send->p_operation = op_accept;
	FB_API_HANDLE handle = 0;

	Firebird::ClumpletWriter dpb_buffer(Firebird::ClumpletReader::Tagged, MAX_SSHORT);
	if (dl)
		dpb_buffer.reset(dpb, dl);
	else
		dpb_buffer.reset(isc_dpb_version1);

#ifdef TRUSTED_AUTH
	// If we have trusted authentication, append it to database parameter block
	if (port->port_trusted_auth)
	{
		AuthSspi* authSspi = port->port_trusted_auth->authSspi;

		Firebird::string trustedUserName;
		if (authSspi->getLogin(trustedUserName))
		{
			dpb_buffer.insertString(isc_dpb_trusted_auth, trustedUserName);
		}
	}
#endif //TRUSTED_AUTH

	// If we have user identification, append it to database parameter block
	rem_str* string = port->port_user_name;
	if (string) {
		dpb_buffer.setCurOffset(dpb_buffer.getBufferLength());
		dpb_buffer.insertString(isc_dpb_sys_user_name, 
			string->str_data, string->str_length);
	}

	// Now insert additional clumplets into dpb
	addClumplets(dpb_buffer, dpbParam, port);
	
/* Disable remote gsec attachments */
	for (dpb_buffer.setCurOffset(1); !dpb_buffer.isEof(); ) {
		if (dpb_buffer.getClumpTag() == isc_dpb_gsec_attach)
			dpb_buffer.deleteClumplet();
		else
			dpb_buffer.moveNext();
	}

	dpb = dpb_buffer.getBuffer();
	dl = dpb_buffer.getBufferLength();

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

	if (!status_vector[1])
	{
		RDB rdb = (RDB) ALLR_block(type_rdb, 0);
		if (rdb)
		{
			port->port_context = rdb;
#ifdef DEBUG_REMOTE_MEMORY
			printf("attach_databases(server)  allocate rdb     %x\n", rdb);
#endif
			rdb->rdb_port = port;
			rdb->rdb_handle = handle;
		}
		else
		{
			status_vector[0] = isc_arg_gds;
			status_vector[1] = isc_virmemexh;
			status_vector[2] = isc_arg_end;
		}
	}

	port->send_response(send, 0, 0, status_vector, false);

#ifdef TRUSTED_AUTH
	delete port->port_trusted_auth;
	port->port_trusted_auth = 0;
#endif
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

	RDB rdb = port->port_context;
	if (bad_db(status_vector, rdb))
	{
		port->send_response(send, 0, 0, status_vector, false);
		return;
	}

	// This buffer is used by INET and WNET transports
	// to return the server identification string
	UCHAR buffer[BUFFER_TINY];
	send->p_resp.p_resp_data.cstr_address = buffer;

	rem_port* aux_port = port->request(send);

	port->send_response(send, rdb->rdb_id,
				  send->p_resp.p_resp_data.cstr_length, 
				  status_vector, false);

	if (status_vector[1]) {
		/* restore the port status vector */

		port->port_status_vector = save_status;
		return;
	}

	if (aux_port) 
	{
		ISC_STATUS* const save_status2 = aux_port->port_status_vector;
		aux_port->port_status_vector = status_vector;

		if (aux_port->connect(send, 0))
		{
			aux_port->port_context = rdb;
			aux_port->port_status_vector = save_status2;
		}
		else
		{
			gds__log_status(NULL, aux_port->port_status_vector);
			fb_assert(port->port_async == aux_port);
			port->port_async = NULL;
			aux_port->disconnect();
		}
	}

/* restore the port status vector */

	port->port_status_vector = save_status;
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
    ISC_STATUS_ARRAY status_vector;
	success(status_vector);
	
/* Which database ? */

	RDB rdb = port->port_context;
	if (bad_db(status_vector, rdb))
	{
		return port->send_response(send, 0, 0, status_vector, false);
	}

/* Find the event */

	RVNT event;
	for (event = rdb->rdb_events; event; event = event->rvnt_next)
		if (event->rvnt_rid == stuff->p_event_rid)
			break;

/* If no event found, pretend it was cancelled */

	if (!event)	{
		return port->send_response(send, 0, 0, status_vector, false);
	}

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

	return port->send_response(send, 0, 0, status_vector, false);
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
	USHORT ret = 0;
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
					ret |= STMT_BLOB;
				}
				else if (type == isc_info_sql_stmt_select ||
					type == isc_info_sql_stmt_select_for_upd)
				{
					ret |= STMT_DEFER_EXECUTE;
				}
				break;
			case isc_info_sql_batch_fetch:
				if (type == 0)
					ret |= STMT_NO_BATCH;
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
	if (bad_db(status_vector, rdb))
	{
		return this->send_response(sendL, 0, 0, status_vector, false);
	}

	FB_API_HANDLE handle = 0;
	const UCHAR* blr = compileL->p_cmpl_blr.cstr_address;
	USHORT blr_length = compileL->p_cmpl_blr.cstr_length;

	THREAD_EXIT();
	isc_compile_request(status_vector, &rdb->rdb_handle, &handle, blr_length,
						reinterpret_cast<const char*>(blr));
	THREAD_ENTER();

	if (status_vector[1])
		return this->send_response(sendL, 0, 0, status_vector, false);

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
		return this->send_response(sendL, 0, 0, status_vector, false);
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

	return this->send_response(sendL, object, 0, status_vector, false);
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
	if (bad_db(status_vector, rdb))
	{
		return this->send_response(sendL, 0, 0, status_vector, false);
	}

	const UCHAR* blr = ddlL->p_ddl_blr.cstr_address;
	const USHORT blr_length = ddlL->p_ddl_blr.cstr_length;

	THREAD_EXIT();
	isc_ddl(status_vector, &rdb->rdb_handle, &transaction->rtr_handle,
			blr_length, reinterpret_cast<const char*>(blr));
	THREAD_ENTER();

	return this->send_response(sendL, 0, 0, status_vector, false);
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
	if (this->port_async)
		this->port_async->port_context = NULL;
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
	if (bad_db(status_vector, rdb))
	{
		this->send_response(sendL, 0, 0, status_vector, false);
		return;
	}

	THREAD_EXIT();
	isc_drop_database(status_vector, &rdb->rdb_handle);
	THREAD_ENTER();

	if (status_vector[1]
		&& (status_vector[1] != isc_drdb_completed_with_errs))
	{
		this->send_response(sendL, 0, 0, status_vector, false);
		return;
	}

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

	this->send_response(sendL, 0, 0, status_vector, false);
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

	return this->send_response(sendL, 0, 0, status_vector, false);
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
	if (bad_db(status_vector, rdb))
	{
		return this->send_response(sendL, 0, 0, status_vector, false);
	}

	THREAD_EXIT();
	isc_detach_database(status_vector, &rdb->rdb_handle);
	THREAD_ENTER();

	if (status_vector[1])
		return this->send_response(sendL, 0, 0, status_vector, false);

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

	return this->send_response(sendL, 0, 0, status_vector, false);
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

	return this->send_response(sendL, 0, 0, status_vector, true);
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
		return this->send_response(sendL, 0, 0, status_vector, true);

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

	const USHORT object = (statement) ? statement->rsr_id : INVALID_OBJECT;

	return this->send_response(sendL, object, 0, status_vector, true);
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

	return this->send_response(sendL, 0, 0, status_vector, false);
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
	if (bad_db(status_vector, rdb))
	{
		return this->send_response(sendL, 0, 0, status_vector, false);
	}

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
			if (!this->port_statement->rsr_message->msg_address)
			{
				this->port_statement->rsr_message->msg_address =
					this->port_statement->rsr_message->msg_buffer;
			}
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
								status_vector, 
								false);
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
		fb_assert(statement->rsr_format == statement->rsr_bind_format);

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

	const bool defer = this->haveRecvData();

	return this->send_response(	sendL,
								(OBJCT) (transaction ? transaction->rtr_id : 0),
								0,
								status_vector, 
								defer);
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
		stmt_clear_exception(statement);
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
			fb_assert(statement->rsr_status);
			statement->rsr_flags &= ~RSR_stream_err;
			return this->send_response(sendL, 0, 0,
								 statement->rsr_status->value(),
								 false);
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
					return this->send_response(sendL, 0, 0, status_vector, false);
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

	// hvlad: message->msg_address not used in xdr_protocol because of
	// response->p_sqldata_messages set to zero above.
	// It is important to not zero message->msg_address after send because 
	// during thread context switch in send we can receive packet with
	// op_free and op_execute (lazy port feature allow this) which itself
	// set message->msg_address to some useful information
	// This fix must be re-thought when real multithreading will be implemented
	if (message) {
		message->msg_address = NULL;
	}

	this->send(sendL);

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
					stmt_save_exception(statement, status_vector, true);
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

	return this->send_response(sendL, 0, 0, status_vector, false);
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
		return (this->port_last_object_id = 
			REMOTE_set_object(this, block, (OBJCT) 1));
	}

	// Search vector for an empty slot.  If we find one, use it

	blk** p = vector->vec_object + 1;
	for (const blk* const* const end = vector->vec_object + vector->vec_count;
		p < end; p++)
	{
		if (!*p) {
			*p = block;
			this->port_last_object_id = (OBJCT) (p - vector->vec_object);
			return this->port_last_object_id;
		}
	}

/* Vector is full -- somebody will need to expand it */

	return (this->port_last_object_id = 
		REMOTE_set_object(this, block, (OBJCT) vector->vec_count));
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
			this->send_response(sendL, blob->rbl_id, length, status_vector, false);
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
								status_vector, 
								false);

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
	if (bad_db(status_vector, rdb))
	{
		return this->send_response(sendL, 0, 0, status_vector, false);
	}

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
		status = this->send_response(sendL, 0, 0, status_vector, false);
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
	if (bad_db(status_vector, rdb))
	{
		return this->send_response(sendL, 0, 0, status_vector, false);
	}

/* Make sure there is a suitable temporary blob buffer */

	UCHAR* const buffer = ALLR_alloc(stuff->p_info_buffer_length);
	memset(buffer, 0, stuff->p_info_buffer_length);
#ifdef DEBUG_REMOTE_MEMORY
	printf("info(server)              allocate buffer  %x\n", buffer);
#endif

	SCHAR info[1024], *info_buffer;
	USHORT info_len;
	UCHAR temp[1024];
	UCHAR* temp_buffer;
	temp_buffer = temp;
	if (op == op_info_database)
	{
		info_len = 0;
		info_buffer = 0;

		if (stuff->p_info_buffer_length > sizeof(temp)) 
		{
			temp_buffer = ALLR_alloc((SLONG) stuff->p_info_buffer_length);
#ifdef DEBUG_REMOTE_MEMORY
			printf("info(server)              allocate buffer  %x\n", temp_buffer);
#endif
		}
	}
	else
	{
		// stuff isc_info_length in front of info items buffer
 
		CSTRING* info_string = (op == op_service_info) ? 
			&stuff->p_info_recv_items : &stuff->p_info_items;

		info_len = 1 + info_string->cstr_length;

		if (info_len > sizeof(info))
		{
			info_buffer = (SCHAR*) ALLR_alloc((SLONG) info_len);
#ifdef DEBUG_REMOTE_MEMORY
			ib_printf("info(server)              allocate buffer  %x\n", info_buffer);
#endif
		}
		else {
			info_buffer = info;
		}

		*info_buffer = isc_info_length;
		memmove(info_buffer + 1, info_string->cstr_address, info_len - 1);
	}

	USHORT info_db_len = 0;
	switch (op) {
	case op_info_blob:
		CHECK_HANDLE_MEMBER(blob,
							RBL,
							type_rbl,
							stuff->p_info_object,
							isc_bad_segstr_handle);
		THREAD_EXIT();
		isc_blob_info(status_vector, &blob->rbl_handle,
					  info_len,
					  info_buffer,
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
			Firebird::string version;
			version.printf("%s/%s", GDS_VERSION, this->port_version->str_data);
			info_db_len = MERGE_database_info(temp_buffer /*temp*/, 
								buffer, stuff->p_info_buffer_length,
								IMPLEMENTATION, 4, 1,
								reinterpret_cast<const UCHAR*>(version.c_str()),
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
						 info_len,
						 info_buffer,
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
							 info_len,
							 info_buffer,
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
						  info_len,
						  info_buffer,
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
						  info_len,
						  info_buffer,
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

	if (info_buffer && (info_buffer != info)) {
#ifdef DEBUG_REMOTE_MEMORY
    	printf ("info(server)              free buffer      %x\n", info_buffer);
#endif
    	ALLR_free(info_buffer);
	}

/* Send a response that includes the segment. */

	USHORT response_len = 
		info_db_len ? info_db_len : stuff->p_info_buffer_length;

	SSHORT skip_len = 0;
	if (*buffer == isc_info_length)
	{
		skip_len = gds__vax_integer(buffer + 1, 2);
		SLONG val = gds__vax_integer(buffer + 3, skip_len);
		skip_len += 3;
		if (val && val <= response_len) {
			response_len = val;
		}
	}

	sendL->p_resp.p_resp_data.cstr_address = buffer + skip_len;

	const ISC_STATUS status = this->send_response(sendL, stuff->p_info_object, 
		response_len, status_vector, false);

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

	return this->send_response(sendL, 0, 0, status_vector, false);
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
	if (bad_db(status_vector, rdb))
	{
		return this->send_response(sendL, 0, 0, status_vector, false);
	}

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

	return this->send_response(sendL, object, 0, status_vector, false);
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

	return this->send_response(sendL, 0, 0, status_vector, false);
}


ISC_STATUS rem_port::prepare_statement(P_SQLST * prepareL, PACKET* sendL)
{
/*****************************************
 *
 *	p r e p a r e _ s t a t e m e n t
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
	UCHAR *info, info_buffer[1024];
	UCHAR* buffer;
	if (prepareL->p_sqlst_buffer_length > sizeof(local_buffer))
		buffer = ALLR_alloc((SLONG) prepareL->p_sqlst_buffer_length);
	else
		buffer = local_buffer;

	// stuff isc_info_length in front of info items buffer
	if (prepareL->p_sqlst_items.cstr_length + 1u > sizeof(info_buffer)) {
		info = ALLR_alloc((SLONG) prepareL->p_sqlst_items.cstr_length + 1);
	}
	else {
		info = info_buffer;
	}
	*info = isc_info_length;
	memmove(info + 1, prepareL->p_sqlst_items.cstr_address, 
		prepareL->p_sqlst_items.cstr_length);

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
					 prepareL->p_sqlst_items.cstr_length + 1,
					 reinterpret_cast<const char*> (info),
					 prepareL->p_sqlst_buffer_length,
					 reinterpret_cast<char*>(buffer));
	THREAD_ENTER();

	if (info != info_buffer) {
		ALLR_free(info);
	}

	if (status_vector[1]) {
		if (buffer != local_buffer)
			ALLR_free(buffer);
		return this->send_response(sendL, 0, 0, status_vector, false);
	}

	REMOTE_reset_statement(statement);

	statement->rsr_flags &= ~(RSR_blob | RSR_no_batch | RSR_defer_execute);
	USHORT state = check_statement_type(statement);
	if (state & STMT_BLOB) {
		statement->rsr_flags |= RSR_blob;
	}
	if (state & STMT_NO_BATCH) {
		statement->rsr_flags |= RSR_no_batch;
	}
	if (state & STMT_DEFER_EXECUTE && (port_flags & PORT_lazy)) {
		statement->rsr_flags |= RSR_defer_execute;
	}
	if (!(port_flags & PORT_lazy)) {
		state = (state & STMT_BLOB) ? 1 : 0;
	}

/* Send a response that includes the info requested. */

	USHORT response_len = prepareL->p_sqlst_buffer_length;
	SSHORT skip_len = 0;
	if (*buffer == isc_info_length)
	{
		skip_len = gds__vax_integer(buffer + 1, 2);
		SLONG val = gds__vax_integer(buffer + 3, skip_len);
		skip_len += 3;
		if (val && val <= response_len) {
			response_len = val;
		}
	}

	sendL->p_resp.p_resp_data.cstr_address = buffer + skip_len;

	const ISC_STATUS status =
		this->send_response(sendL,
							state,
							response_len,
							status_vector,
							false);

	if (buffer != local_buffer) {
		ALLR_free(buffer);
	}

	return status;
}


// Declared in serve_proto.h
bool process_packet(rem_port* port,
					PACKET* sendL,
					PACKET* receive,
					rem_port** result)
{
	port->port_flags |= PORT_busy;
	bool res;
	try {
		res = process_packet2(port, sendL, receive, result);
		if (res && result)
		{
			port = *result;
			if (port)
			{
				port->port_flags &= ~PORT_busy;
				port->port_requests_queued--;
			}
		}
		return res;
	}
	catch(...)
	{
		port->port_flags &= ~PORT_busy;
		port->port_requests_queued--;
		throw;
	}
}


static bool process_packet2(rem_port* port,
							PACKET* sendL,
							PACKET* receive,
							rem_port** result)
{
/**************************************
 *
 *	p r o c e s s _ p a c k e t 2
 *
 **************************************
 *
 * Functional description
 *	Given an packet received from client, process it a packet ready to be
 *	sent.
 *
 **************************************/
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
					gds__log("SERVER/process_packet: connection rejected for %*.*s",
							string->str_length, string->str_length,
							string->str_data);
				}
				if (port->port_server->srvr_flags & SRVR_multi_client) {
					port->port_state = state_broken;
				}
				else {
					gds__log("SERVER/process_packet: connect reject, server exiting");
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
			attach_service(port, &receive->p_atch, sendL);
			break;

		case op_trusted_auth:
#ifdef TRUSTED_AUTH
			trusted_auth(port, &receive->p_trau, sendL);
			break;
//else		
//			fall down ...
#endif
		case op_update_account_info:
		case op_authenticate_user:
			{
				ISC_STATUS_ARRAY status_vector;
				status_vector[0] = isc_arg_gds;
				status_vector[1] = isc_wish_list;
				status_vector[2] = isc_arg_end;
				port->send_response(sendL, 0, 0, status_vector, false);
			}
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
						gds__log("SERVER/process_packet: Multi-client server shutdown");
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
			gds__log("SERVER/process_packet: don't understand packet type %d",
					receive->p_operation);
			port->port_state = state_broken;
			break;
		}

		if (port && port->port_state == state_broken) {
			if (!port->port_parent) {
				gds__log("SERVER/process_packet: broken port, server exiting");
				port->disconnect(sendL, receive);
				ThreadData::restoreSpecific();
				return false;
			}
			port->disconnect(sendL, receive);
			port = NULL;
		}

		if (result)
			*result = port;

	}	// try
	catch (const Firebird::Exception& ex) {
		// NS: trdb_status_vector is usually NULL at this point.
		// This needs to be fixed, probably via eliminating trdb_status_vector
		// completely, but this work is not for now.
		ISC_STATUS* status_ptr;

		ISC_STATUS_ARRAY local_status;
		memset(local_status, 0, sizeof(local_status));

		if (tdrdb->trdb_status_vector) {
			status_ptr = tdrdb->trdb_status_vector;
		} 
		else {
			status_ptr = local_status;
		}

		Firebird::stuff_exception(status_ptr, ex);

		gds__log_status("unknown, SERVER/process_packet", status_ptr);

		/*  It would be nice to log an error to the user, instead of just terminating them!  */
		port->send_response(sendL, 0, 0, status_ptr, false);
		port->disconnect(sendL, receive);	/*  Well, how about this...  */

		ThreadData::restoreSpecific();
		return false;
	}

	ThreadData::restoreSpecific();
	return true;
}


#ifdef TRUSTED_AUTH
static void trusted_auth(rem_port* port, P_TRAU* p_trau, PACKET* send)
{
	ISC_STATUS_ARRAY status_vector;
	ServerAuth* sa = port->port_trusted_auth;
	if (! sa)
	{
		status_vector[0] = isc_arg_gds;
		status_vector[1] = isc_unavailable;
		status_vector[2] = isc_arg_end;
		port->send_response(send, 0, 0, status_vector, false);
	}
	try 
	{
		AuthSspi::DataHolder data;
		memcpy(data.getBuffer(p_trau->p_trau_data.cstr_length),
			p_trau->p_trau_data.cstr_address, p_trau->p_trau_data.cstr_length);

		AuthSspi* authSspi = sa->authSspi;

		if (authSspi->accept(data) && authSspi->isActive())
		{
			send->p_operation = op_trusted_auth;
			cstring& s = send->p_trau.p_trau_data;
			s.cstr_allocated = 0;
			s.cstr_length = data.getCount();
			s.cstr_address = data.begin();
			port->send(send);
			return;
		}
	}
	catch(const Firebird::status_exception& e)
	{
		ISC_STATUS_ARRAY status_vector;
		Firebird::stuff_exception(status_vector, e);
		port->send_response(send, 0, 0, status_vector, false);
		return;
	}

	sa->part2(port, sa->operation, sa->fileName.c_str(), sa->fileName.length(), 
		sa->clumplet.begin(), sa->clumplet.getCount(), send);
}


static bool canUseTrusted()
{
/**************************************
 *
 *	c a n U s e T r u s t e d
 *
 **************************************
 *
 * Functional description
 *	Cache config setting for trusted auth.
 *
 **************************************/
	static AmCache useTrusted = AM_UNKNOWN;

	if (useTrusted == AM_UNKNOWN)
	{
		Firebird::PathName authMethod(Config::getAuthMethod());
		useTrusted = (authMethod == AmTrusted || authMethod == AmMixed) ? AM_ENABLED : AM_DISABLED;
	}

	return useTrusted == AM_ENABLED;
}
#endif // TRUSTED_AUTH


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
		return this->send_response(sendL, 0, 0, status_vector, false);
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
			return this->send_response(sendL, 0, 0, status_vector, false);
		p += length;
	}

	return this->send_response(sendL, 0, 0, status_vector, false);
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
	if (bad_db(status_vector, rdb))
	{
		return this->send_response(sendL, 0, 0, status_vector, false);
	}

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

	return this->send_response(sendL, 0, 0, status_vector, false);
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
	ISC_STATUS_ARRAY status_vector;

	RDB rdb = this->port_context;
	if (bad_db(status_vector, rdb))
	{
		return this->send_response(sendL, 0, 0, status_vector, false);
	}

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
	isc_que_events(status_vector, &rdb->rdb_handle, &event->rvnt_id,
				   stuff->p_event_items.cstr_length,
				   stuff->p_event_items.cstr_address,
				   server_ast,
				   event);
	THREAD_ENTER();

	const SLONG id = event->rvnt_id;
	if (status_vector[1]) {
		event->rvnt_id = 0;
	}

	return this->send_response(sendL, (OBJCT) id, 0, status_vector, false);
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
		return this->send_response(sendL, 0, 0, status_vector, false);
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
	
	if (msg_number > requestL->rrq_max_msg)
	{
		status_vector[0] = isc_arg_gds;
		status_vector[1] = isc_badmsgnum;
		status_vector[2] = isc_arg_end;
		return this->send_response(sendL, 0, 0, status_vector, false);
	}
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
								  requestL->rrq_status_vector, 
								  false);
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
				return this->send_response(sendL, 0, 0, status_vector, false);

#ifdef SCROLLABLE_CURSORS
			/* set the appropriate flags according to the way we just scrolled 
			   the next layer down, and calculate the offset from the beginning 
			   of the result set */

			switch (direction) {
			case blr_forward:
				tail->rrq_flags &= ~RRQ_backward;
				tail->rrq_absolute +=
					(tail->rrq_flags & RRQ_absolute_backward) ?
						-offset : offset;
				break;

			case blr_backward:
				tail->rrq_flags |= RRQ_backward;
				tail->rrq_absolute +=
					(tail->rrq_flags & RRQ_absolute_backward) ?
						offset : -offset;
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

	stmt_release_exception(*statement);
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
			&& !(tail->rrq_flags & RRQ_absolute_backward)))
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

	sendL->p_resp.p_resp_blob_id.bid_quad_low = result;

	return this->send_response(sendL, 0, 0, status_vector, false);
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
	if (number > requestL->rrq_max_msg)
	{
		status_vector[0] = isc_arg_gds;
		status_vector[1] = isc_badmsgnum;
		status_vector[2] = isc_arg_end;
		return this->send_response(sendL, 0, 0, status_vector, false);
	}
	REM_MSG message = requestL->rrq_rpt[number].rrq_message;
	const rem_fmt* format = requestL->rrq_rpt[number].rrq_format;

	THREAD_EXIT();
	isc_send(status_vector, &requestL->rrq_handle, number, format->fmt_length,
			 message->msg_address, data->p_data_incarnation);
	THREAD_ENTER();

	message->msg_address = NULL;

	return this->send_response(sendL, 0, 0, status_vector, false);
}


ISC_STATUS rem_port::send_response(	PACKET*	sendL,
							OBJCT	object,
							USHORT	length,
							const ISC_STATUS* status_vector,
							bool defer_flag)
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
		switch (*status_vector)
		{
		case isc_arg_warning:
		case isc_arg_gds:
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
			for (;;)
			{
				switch (*status_vector)
				{
				case isc_arg_string:
				case isc_arg_number:
					*v++ = *status_vector++;
					*v++ = *status_vector++;
					continue;

				case isc_arg_cstring:
					++status_vector;
					*v++ = isc_arg_string;
					if (p < bufferEnd) // CVC: Avoid B.O.
					{
						TEXT** sp = (TEXT**) v;
						*sp++ = p;
						v = (ISC_STATUS*) sp;
						SLONG l = *status_vector++;
						const TEXT* q = (TEXT*) *status_vector++;
						while (l-- > 0 && (p < bufferEnd - 1))
							*p++ = *q++;

						*p++ = 0;
					}
					else
					{
						// See if we can honor zero length strings, else use an error msg
						TEXT** sp = (TEXT**) v;
						if (!*status_vector) // this is the length
							*sp++ = const_cast<char*>("");
						else
							*sp++ = const_cast<char*>("Not enough buffer for message");
							
						v = (ISC_STATUS*) sp;
						status_vector += 2;
					}
					continue;
				} // switch
				break;
			} // for (;;)
			continue;

		case isc_arg_interpreted:
			*v++ = *status_vector++;
			*v++ = *status_vector++;
			continue;
		}
		
		const int l = (p < bufferEnd) ? fb_interpret(p, bufferEnd - p, &status_vector) : 0;
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

	if ((this->port_flags & PORT_lazy) && defer_flag) {
		this->send_partial(sendL);
	}
	else {
		this->send(sendL);
	}

#ifndef SUPERSERVER
	// In case of CS remote listener we have a single connection system.
	// If database is shut down, it's no use running fb_inet_server any more.
	if (exit_code == isc_shutdown)
	{
		exit(0);
	}
#endif

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


static void attach_service(rem_port* port, P_ATCH* attach, PACKET* sendL)
{
	const char* service_name = reinterpret_cast<const char*>
		(attach->p_atch_file.cstr_address);
	const USHORT service_length = attach->p_atch_file.cstr_length;

	Firebird::ClumpletWriter spb(Firebird::ClumpletReader::SpbAttach, MAX_DPB_SIZE, 
		attach->p_atch_dpb.cstr_address, attach->p_atch_dpb.cstr_length, isc_spb_current_version);

#ifdef TRUSTED_AUTH
	// Do we can & need trusted authentication?
	if (spb.find(isc_spb_trusted_auth))
	{
		try 
		{
			// extract trusted authentication data from spb
			AuthSspi::DataHolder data;
			memcpy(data.getBuffer(spb.getClumpLength()), 
				spb.getBytes(), spb.getClumpLength());
			spb.deleteClumplet();

			// remove extra trusted tags if present (security measure)
			spb.deleteWithTag(isc_spb_trusted_auth);

			if (canUseTrusted() && port->port_protocol >= PROTOCOL_VERSION11)
			{
				port->port_trusted_auth = FB_NEW(*getDefaultMemoryPool()) 
					ServerAuth(service_name, service_length, spb, attach_service2, op_trusted_auth);
				AuthSspi* authSspi = port->port_trusted_auth->authSspi;

				if (authSspi->accept(data) && authSspi->isActive())
				{
					sendL->p_operation = op_trusted_auth;
					cstring& s = sendL->p_trau.p_trau_data;
					s.cstr_allocated = 0;
					s.cstr_length = data.getCount();
					s.cstr_address = data.begin();
					port->send(sendL);
					return;
				}
			}
		}
		catch(const Firebird::status_exception& e)
		{
			ISC_STATUS_ARRAY status_vector;
			Firebird::stuff_exception(status_vector, e);
			delete port->port_trusted_auth;
			port->port_trusted_auth = 0;
			port->send_response(sendL, 0, 0, status_vector, false);
			return;
		}
	}
#endif // TRUSTED_AUTH
	
	attach_service2(port, op_trusted_auth, service_name, service_length, 
		spb.getBuffer(), spb.getBufferLength(), sendL);
}


static void attach_service2(rem_port* port,
							P_OP,
							const char* service_name, 
							int service_length, 
							const UCHAR* spb, 
							int sl, 
							PACKET* sendL)
{
	Firebird::ClumpletWriter tmp(Firebird::ClumpletReader::SpbAttach, MAX_DPB_SIZE, 
			spb, sl, isc_spb_current_version);

	port->service_attach(service_name, service_length, tmp, sendL);

#ifdef TRUSTED_AUTH
	delete port->port_trusted_auth;
	port->port_trusted_auth = 0;
#endif
}


ISC_STATUS rem_port::service_attach(const char* service_name, 
									const USHORT service_length,
									Firebird::ClumpletWriter& spb, 
									PACKET* sendL)
{
/**************************************
 *
 *	s e r v i c e _ a t t a c h 
 *
 **************************************
 *
 * Functional description
 *	Connect to a Firebird service.
 *
 **************************************/
	sendL->p_operation = op_accept;
	FB_API_HANDLE handle = 0;

#ifdef TRUSTED_AUTH
	// If we have trusted authentication, append it to database parameter block
	if (port_trusted_auth)
	{
		AuthSspi* authSspi = port_trusted_auth->authSspi;

		Firebird::string trustedUserName;
		if (authSspi->getLogin(trustedUserName))
		{
			spb.insertString(isc_spb_trusted_auth, trustedUserName);
		}
	}
#endif // TRUSTED_AUTH

	// If we have user identification, append it to database parameter block
	const rem_str* string = port_user_name;
	if (string) {
		spb.setCurOffset(spb.getBufferLength());
		spb.insertString(isc_spb_sys_user_name, 
			string->str_data, string->str_length);
	}

    // Now insert additional clumplets into spb
	addClumplets(spb, spbParam, this);
	
	/* See if user has specified parameters relevent to the connection,
	   they will be stuffed in the SPB if so. */
	REMOTE_get_timeout_params(this, spb.getBuffer(), spb.getBufferLength());

	THREAD_EXIT();
	ISC_STATUS_ARRAY status_vector;
	isc_service_attach(status_vector,
					   service_length,
					   service_name,
					   &handle,
					   spb.getBufferLength(),
					   reinterpret_cast<const char*>(spb.getBuffer()));
	THREAD_ENTER();

	if (!status_vector[1]) {
		RDB rdb = (RDB) ALLR_block(type_rdb, 0);
		if (rdb)
		{
			this->port_context = rdb;
#ifdef DEBUG_REMOTE_MEMORY
			printf("attach_service(server)  allocate rdb     %x\n", rdb);
#endif
			rdb->rdb_port = this;
			rdb->rdb_handle = handle;
			rdb->rdb_flags |= RDB_service;
		}
		else
		{
			status_vector[0] = isc_arg_gds;
			status_vector[1] = isc_virmemexh;
			status_vector[2] = isc_arg_end;
		}
	}

	return this->send_response(sendL, 0, 0, status_vector, false);
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
    ISC_STATUS_ARRAY status_vector;
	
	RDB rdb = this->port_context;
	if (bad_service(status_vector, rdb))
	{
		return this->send_response(sendL, 0, 0, status_vector, false);
	}

	THREAD_EXIT();
	isc_service_detach(status_vector, &rdb->rdb_handle);
	THREAD_ENTER();

	return this->send_response(sendL, 0, 0, status_vector, false);
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
    ISC_STATUS_ARRAY status_vector;
	
	RDB rdb = this->port_context;
	if (bad_service(status_vector, rdb))
	{
		return this->send_response(sendL, 0, 0, status_vector, false);
	}
	
	THREAD_EXIT();
	SLONG* reserved = 0;		// reserved for future use
	isc_service_start(status_vector,
					  &rdb->rdb_handle,
					  reserved,
					  stuff->p_info_items.cstr_length,
					  reinterpret_cast<char*>(stuff->p_info_items.cstr_address));
	THREAD_ENTER();

	return this->send_response(sendL, 0, 0, status_vector, false);
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

	return this->send_response(sendL, 0, 0, status_vector, false);
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

	return this->send_response(sendL, 0, 0, status_vector, false);
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

	if (number > requestL->rrq_max_msg)
	{
		status_vector[0] = isc_arg_gds;
		status_vector[1] = isc_badmsgnum;
		status_vector[2] = isc_arg_end;
		return this->send_response(sendL, 0, 0, status_vector, false);
	}
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

	return this->send_response(sendL, 0, 0, status_vector, false);
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
	if (bad_db(status_vector, rdb))
	{
		return this->send_response(sendL, 0, 0, status_vector, false);
	}
	
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

	return this->send_response(sendL, object, 0, status_vector, false);
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
	++threads_total;

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

#ifdef SUPERSERVER
				// With lazy port feature enabled we can have more received and
				// not handled data in receive queue. Handle it now if it contains
				// whole request packet. If it contain partial packet don't clear
				// request queue, restore receive buffer state to state before
				// reading packet and wait until rest of data arrives
				if (port && port->haveRecvData())
				{
					SERVER_REQ new_request = alloc_request();
					
					const rem_port::RecvQueState recvState = port->getRecvState();
					port->receive(&new_request->req_receive);

					if (new_request->req_receive.p_operation == op_partial)
					{
//						gds__log("Partial");
						free_request(new_request);
						port->setRecvState(recvState);
					}
					else
					{
						if (!port->haveRecvData())
							port->clearRecvQue();

						new_request->req_port = port;

						const bool ok = 
							link_request(active_requests, port, new_request, "ACTIVE ") ||
							link_request(request_que, port, new_request, "PENDING"); 
						fb_assert(ok);
					}
				}
#endif

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
			if (shutting_down)
			{
				return 0;
			}
			THREAD_ENTER();
			--threads_waiting;
		}
	}

	--threads_total;
	THREAD_EXIT();

#ifdef WIN_NT
	if (!((SLONG) flags & SRVR_non_service))
		CNTL_remove_thread(thread);
#endif

	return 0;
}
#endif


void SRVR_shutdown()
{
/**************************************
 *
 *	S R V R _ s h u t d o w n
 *
 **************************************
 *
 * Functional description
 *	Shutdown working threads, waiting for work
 *  Function is called when shutdowm thread ENTERed,
 *	and will never EXIT
 *
 **************************************/
	shutting_down = true;

#ifdef MULTI_THREAD
	int limit = threads_waiting;
	for (int i = 0; i < limit; i++)
	{
		requests_semaphore.release();
	}
	
	// let them terminate
	THREAD_SLEEP(1 * 1000);
#endif
}


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
	if (bad_db(status_vector, rdb))
	{
		return this->send_response(sendL, 0, 0, status_vector, false);
	}
	
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
		(procedure->rpr_out_format) ? procedure->rpr_out_format->fmt_length : 0;

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
		return this->send_response(sendL, 0, 0, status_vector, false);

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

static bool bad_port_context(ISC_STATUS* status_vector, 
							 RDB rdb, 
							 const ISC_LONG error)
{
/**************************************
 *
 *	b a d _ p o r t _ c o n t e x t
 *
 **************************************
 *
 * Functional description
 *	Check rdb pointer, in case of error create status vector
 *
 **************************************/
	if (rdb)
	{
		return false;
	}
	status_vector[0] = isc_arg_gds;
	status_vector[1] = error;
	status_vector[2] = isc_arg_end;
	return true;
}
