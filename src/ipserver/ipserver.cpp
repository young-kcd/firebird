/*
 *      PROGRAM:        Interprocess Interface server
 *      MODULE:         ipserver.c
 *      DESCRIPTION:    interprocess interface server
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
#include "../jrd/ib_stdio.h"
#include <signal.h>
#include <errno.h>
#include <windows.h>

#include "../ipserver/ipc.h"
#include "../jrd/license.h"
#include "../jrd/gds.h"
#include "../jrd/gds_proto.h"
#include "../ipserver/alli_proto.h"
#include "../jrd/thd_proto.h"
#include "../ipserver/ipsrv_proto.h"
#include "../ipserver/ips.h"
#include "../jrd/isc_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/ibsetjmp.h"
#include "../jrd/why_proto.h"
#include "../common/config/config.h"
#include "../jrd/gdsassert.h"

static void allocate_statement(ICC);
static void attach_database(ICC, P_OP);
static void cancel_events(ICC);
static USHORT check_statement_type(IPSERVER_ISR);
static void compile(ICC);
static void ddl(ICC);
static void drop_database(ICC);
static void end_blob(ICC, P_OP);
static void end_database(ICC);
static void end_request(ICC);
static void end_statement(ICC);
static void end_transaction(ICC, P_OP);
static void event_ast(IVNT, USHORT, UCHAR *);
static void execute_immediate(ICC, P_OP);
static void execute_statement(ICC, P_OP);
static void fetch(ICC);
static UCHAR *get_buffer(IPS, USHORT, USHORT);
static void get_segment(ICC);
static void get_slice(ICC);
static void info(ICC, P_OP);
static void insert(ICC);
static void ipi_end_thread(ICC);
static void ipi_server(ICC);
static IPM make_map(USHORT);
static ITR make_transaction(IDB, FRBRD *);
static void open_blob(ICC, P_OP);
static void prepare_statement(ICC);
static void put_segment(ICC);
static void put_slice(ICC);
static void que_events(ICC);
static void receive_msg(ICC);
static void reconnect(ICC);
static void release_blob(IBL);
static void release_event(IVNT);
static void release_request(IRQ);
static void release_sql_request(IPSERVER_ISR);
static void release_transaction(ITR);
static void seek_blob(ICC);
static BOOL send_and_wait(ICC);
static void send_msg(ICC);
static void send_no_wait(ICC);
static void send_response(ICC, ISC_STATUS *);
static void set_cursor(ICC);
static void service_attach(ICC);
static void service_end(ICC);
static void service_start(ICC);
static void shutdown_attachments(ICC);
static void start(ICC);
static void start_and_send(ICC);
static void start_transaction(ICC);
static void transact_request(ICC);
static BOOL transfer_buffers(ICC, ips_comm_area *);
static void unwind(ICC);
static BOOL wait_no_send(ICC);

static HWND IPSVR_window = NULL;
static UCHAR sql_info[] = { isc_info_sql_batch_fetch };

/* Macro to check a given handle for validity.  While generally
 * we can depend on the client-side remote library to do this
 * for us, there were bugs due to not checking on our side.
 * Also, since these handles are passed though to
 * the why-value routines and checked there, strictly speaking
 * we only have to check ones that we dereference before
 * passing along.  For safety's side, we check them all.
 */

/* h = handle to check
 * t = type of handle to expect
 * c = error code to return
 */
#define CHECK_HANDLE(h,t,c)     if ( !(h) || (((BLK) (h))->blk_type !=( t))) \
				    { \
				    status_vector[0] = isc_arg_gds;\
				    status_vector[1] = (c);\
				    status_vector[2] = isc_arg_end;\
				    send_response( icc, status_vector); \
				    return; \
				    }

#define NOT_NULL(p,s)           if ( !(p) && (s)) \
				    { \
				    status_vector[0] = isc_arg_gds;\
				    status_vector[1] = isc_virmemexh;\
				    status_vector[2] = isc_arg_end;\
				    send_response( icc, status_vector); \
				    return; \
				    }

#define GET_COMM_OBJECT { comm = (ips_comm_area*)icc->icc_mapped_addr;\
			  comm_ptr = (TEXT*)comm->ips_data;}

#define IPS_SERVER(c,n,a,s)     { c->ips_buffers[n].ips_sv_size = s;\
				  c->ips_buffers[n].ips_sv_addr = a;\
				  c->ips_buffers[n].ips_sv_copied = 0;}



#define GDS_ATTACH_DATABASE     isc_attach_database
#define GDS_BLOB_INFO           isc_blob_info
#define GDS_CANCEL_BLOB         isc_cancel_blob
#define GDS_CANCEL_EVENTS       isc_cancel_events
#define GDS_CLOSE_BLOB          isc_close_blob
#define GDS_COMMIT              isc_commit_transaction
#define GDS_COMMIT_RETAINING    isc_commit_retaining
#define GDS_COMPILE             isc_compile_request
#define GDS_CREATE_BLOB         isc_create_blob
#define GDS_CREATE_BLOB2        isc_create_blob2
#define GDS_CREATE_DATABASE     isc_create_database
#define GDS_DATABASE_INFO       isc_database_info
#define GDS_DDL                 isc_ddl
#define GDS_DETACH              isc_detach_database
#define GDS_DROP_DATABASE       isc_drop_database
#define GDS_GET_SEGMENT         isc_get_segment
#define GDS_GET_SLICE           isc_get_slice
#define GDS_OPEN_BLOB           isc_open_blob
#define GDS_OPEN_BLOB2          isc_open_blob2
#define GDS_PREPARE             isc_prepare_transaction
#define GDS_PREPARE2            isc_prepare_transaction2
#define GDS_PUT_SEGMENT         isc_put_segment
#define GDS_PUT_SLICE           isc_put_slice
#define GDS_QUE_EVENTS          isc_que_events
#define GDS_RECONNECT           isc_reconnect_transaction

#ifdef SCROLLABLE_CURSORS
#define GDS_RECEIVE             isc_receive2
#else
#define GDS_RECEIVE             isc_receive
#endif

#define GDS_RELEASE_REQUEST     isc_release_request
#define GDS_REQUEST_INFO        isc_request_info
#define GDS_ROLLBACK            isc_rollback_transaction
#define GDS_ROLLBACK_RETAINING  isc_rollback_retaining
#define GDS_SEEK_BLOB           isc_seek_blob
#define GDS_SEND                isc_send
#define GDS_SERVICE_ATTACH      isc_service_attach
#define GDS_SERVICE_DETACH      isc_service_detach
#define GDS_SERVICE_QUERY       isc_service_query
#define GDS_SERVICE_START       isc_service_start
#define GDS_START_AND_SEND      isc_start_and_send
#define GDS_START               isc_start_request
#define GDS_START_MULTIPLE      isc_start_multiple
#define GDS_START_TRANSACTION   isc_start_transaction
#define GDS_TRANSACT_REQUEST    isc_transact_request
#define GDS_TRANSACTION_INFO    isc_transaction_info
#define GDS_UNWIND              isc_unwind_request

#define GDS_DSQL_ALLOCATE       isc_dsql_allocate_statement
#define GDS_DSQL_EXECUTE        isc_dsql_execute2_m
#define GDS_DSQL_EXECUTE_IMMED  isc_dsql_exec_immed3_m
#define GDS_DSQL_FETCH          isc_dsql_fetch_m
#define GDS_DSQL_FREE           isc_dsql_free_statement
#define GDS_DSQL_INSERT         isc_dsql_insert_m
#define GDS_DSQL_PREPARE        isc_dsql_prepare_m
#define GDS_DSQL_SET_CURSOR     isc_dsql_set_cursor_name
#define GDS_DSQL_SQL_INFO       isc_dsql_sql_info

//
// TMN:
//
// Static data ahead.
//
// But the multithread-wizard said:
// - Let there be light. Move static data into specific instances.
//   Let these instances be one per thread, and there will be rejoicing
//   among your followers again... Currently we don't use thread-specific
//   data here, but putting it in a struct is a step.
//
struct ipserver_private_data_t
{
	IPM		first_ipm;
	IPM		ipms;
	MUTX_T	ipics;
	USHORT	pages_per_user;
	USHORT	users_per_map;
	// TMN: num_maps was never used, why I patched it with comments.
	// If anyone can shed some light as to why this was added in the
	// first place it would be appreciated.
//	USHORT	num_maps;
};

static ipserver_private_data_t ipserver_private_data =
{
	0,						// first_ipm
	0,						// ipms
	{ 0 },					// ipics - DON'T REMOVE THE BRACES!
	IPS_DEF_PAGES_PER_CLI,	// pages_per_user
	IPS_DEF_NUM_CLI			// users_per_map
};


#ifdef  DEBUG_IP_TRACE
static const char* op_strings[] =
{
	"op_attach",
	"op_create",
	"op_detach",
	"op_compile",
	"op_start",
	"op_start_and_send",
	"op_send",
	"op_receive",
	"op_unwind",
	"op_release",
	"op_transaction",
	"op_commit",
	"op_rollback",
	"op_reconnect",
	"op_create_blob",
	"op_open_blob",
	"op_get_segment",
	"op_put_segment",
	"op_cancel_blob",
	"op_close_blob",
	"op_info_database",
	"op_info_request",
	"op_info_transaction",
	"op_info_blob",
	"op_prepare2",
	"op_commit_retaining",
	"op_rollback_retaining",
	"op_open_blob2",
	"op_create_blob2",
	"op_ddl",
	"op_que_events",
	"op_cancel_events",
	"op_put_slice",
	"op_get_slice",
	"op_seek_blob",
	"op_allocate_stmt",
	"op_execute",
	"op_exec_immediate",
	"op_fetch",
	"op_free_stmt",
	"op_prepare_stmt",
	"op_set_cursor",
	"op_info_sql",
	"op_insert",
	"op_execute2",
	"op_exec_immediate2",
	"op_service_attach",
	"op_service_detach",
	"op_service_info",
	"op_service_start",
	"op_transact_request",
	"op_drop_database",
	"op_ping",
	"op_disconnect"
};
#endif


#if defined(SUPERSERVER) && defined(WIN_NT)
extern "C" {
	static void atexit_close_handles()
	{
		IPM pFirstIpm = ipserver_private_data.first_ipm;
		for (IPM pIpm = pFirstIpm; pIpm; pIpm = pIpm->ipm_next)
		{
			if (pIpm->ipm_address) {
				UnmapViewOfFile(pIpm->ipm_address);
				pIpm->ipm_address = 0;
			}
			if (pIpm->ipm_handle) {
				CloseHandle(pIpm->ipm_handle);
				pIpm->ipm_handle = 0;
			}
		}
	}
}
#endif	// SUPERSERVER && WIN_NT


USHORT IPS_init(HWND	hwnd,
				USHORT	usrs_pr_mp,
				USHORT	pgs_pr_usr,
				USHORT	mx_mps)
{
/**************************************
 *
 *      I P S _ i n i t
 *
 **************************************
 *
 * Functional description
 *      Create a critical section structure and init
 *      the mapped file stuff.   Returns 1 if successful
 *      and 0 otherwise.
 *
 **************************************/

	/* init the limits */

	if (usrs_pr_mp &&
		usrs_pr_mp >= IPS_MIN_NUM_CLI &&
		usrs_pr_mp <= IPS_MAX_NUM_CLI)
	{
		ipserver_private_data.users_per_map = usrs_pr_mp;
	}
	if (pgs_pr_usr &&
		pgs_pr_usr >= IPS_MIN_PAGES_PER_CLI &&
		pgs_pr_usr <= IPS_MAX_PAGES_PER_CLI)
	{
		ipserver_private_data.pages_per_user = pgs_pr_usr;
	}

// TMN: Disabled code. See definition of the struct above for comments.
//	if (mx_mps && mx_mps >= IPS_MIN_NUM_MAPS && mx_mps <= IPS_MAX_NUM_MAPS) {
//		ipserver_private_data.num_maps = mx_mps;
//	}

	/* init mapped chain and critical regions */

	ipserver_private_data.ipms = NULL;
	THD_mutex_init(&ipserver_private_data.ipics);
	IPSVR_window = hwnd;

	/* create the first map (failure means it's already there */

	IPM ipm = make_map(0);

	if (ipm && (ipm != (IPM) - 1))
	{
		fb_assert(!ipserver_private_data.first_ipm);
		ipserver_private_data.first_ipm = ipm;
#if defined(SUPERSERVER) && defined(WIN_NT)
		// TMN: 2003-03-11: Close the handles at server shutdown
		// Possibly this is also needed for CS, but since I can't test that
		// I decided to only do it for SS.
		atexit(&atexit_close_handles);
#endif
		return (USHORT) 1;
	}

	return (USHORT)(ULONG) ipm;
}


ULONG IPS_start_thread(ULONG client_pid)
{
/**************************************
 *
 *      I P S _ s t a r t _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *      Start an interprocess thread.   This allocates
 *      the next available chunk of the mapped file and
 *      tells the client where it is.
 *
 **************************************/
	ICC icc;
	USHORT mapped_area, mapped_position;
	TEXT name_buffer[128];
	IPM ipm;
	USHORT i, j;
	UCHAR *p;
	HANDLE server_proc;
	ips_comm_area *comm;


	/* go through list of maps */

	THD_mutex_lock(&ipserver_private_data.ipics);
	j = 0;
	for (ipm = ipserver_private_data.ipms; ipm; ipm = ipm->ipm_next)
	{
		/* find an available unused comm area */

		for (i = 0; i < ipserver_private_data.users_per_map; i++)
		{
			if (!ipm->ipm_ids[i])
			{
				break;
			}
		}
		if (i < ipserver_private_data.users_per_map)
		{
			ipm->ipm_count++;
			ipm->ipm_ids[i]++;
			j = ipm->ipm_number;
			break;
		}
		j++;
	}

	/* if the mapped file structure has not yet been initialized,
	   make one now */

	if (!ipm)
	{

		/* allocate new map file and first slot */

		ipm = make_map(j);

		/* check for errors in creation of mapped file */

		if (!ipm || ipm == (IPM) - 1)
		{
			THD_mutex_unlock(&ipserver_private_data.ipics);
			return (ULONG) - 1;
		}
		i = 0;
		ipm->ipm_ids[0]++;
		ipm->ipm_count++;
	}
	mapped_area = j;
	mapped_position = i;

	/* allocate a communications control structure and fill it in */

	icc = (ICC) ALLOC(type_icc);
	if (!icc)
	{
		THD_mutex_unlock(&ipserver_private_data.ipics);
		return (ULONG) - 1;
	}
	p = (UCHAR *) ipm->ipm_address;
	p += IPS_MAPPED_FOR_CLI(ipserver_private_data.pages_per_user, mapped_position);
	memset(p, (char) 0, IPS_MAPPED_PER_CLI(ipserver_private_data.pages_per_user));
	icc->icc_mapped_addr = reinterpret_cast<UCHAR*>(p);
	icc->icc_ipm = ipm;
	icc->icc_slot = mapped_position;
	icc->icc_flags = 0;
	icc->icc_client_id = (DWORD) client_pid;
	icc->icc_client_proc =
		OpenProcess(PROCESS_ALL_ACCESS, TRUE, icc->icc_client_id);
	comm = (ips_comm_area *) icc->icc_mapped_addr;
	comm->ips_client_id = icc->icc_client_id;
	comm->ips_client_proc = icc->icc_client_proc;
	comm->ips_server_id = GetCurrentProcessId();

	/* make sure client knows what this server speaks */

	comm->ips_server_protocol = 1L;
	comm->ips_client_protocol = 0L;

	/* Duplicate the server process handle for client synchronization */
	if (!DuplicateHandle(GetCurrentProcess(),
						 GetCurrentProcess(),
						 GetCurrentProcess(),
						 &server_proc, PROCESS_ALL_ACCESS, FALSE, 0))
	{
		CHAR szBuf[80];
		DWORD dw = GetLastError();

		sprintf(szBuf, "%s:%s failed: GetLastError returned %lu\n",
				"ipserver.c", "DuplicateHandle(server_proc)", dw);
		gds__log(szBuf);
	}
	if (!DuplicateHandle(server_proc,
						 server_proc,
						 comm->ips_client_proc,
						 &comm->ips_server_proc, SYNCHRONIZE, FALSE, 0))
	{
		CHAR szBuf[80];
		DWORD dw = GetLastError();

		sprintf(szBuf, "%s:%s failed: GetLastError returned %lu\n",
				"ipserver.c", "DuplicateHandle(ips_server_proc)", dw);
		gds__log(szBuf);
		comm->ips_server_proc = 0;
	}

	CloseHandle(server_proc);

	/* create the semaphores and put the handles into the icc */

	sprintf(name_buffer, IPI_CLIENT_SEM_NAME, Config::getIpcName(),
		mapped_area, mapped_position);
	icc->icc_client_sem =
		CreateSemaphore(ISC_get_security_desc(), 0L, 1L, name_buffer);
	sprintf(name_buffer, IPI_SERVER_SEM_NAME, Config::getIpcName(),
		mapped_area, mapped_position);
	icc->icc_server_sem =
		CreateSemaphore(ISC_get_security_desc(), 0L, 1L, name_buffer);

	/* make sure semaphores are "empty" */

	while (WaitForSingleObject(icc->icc_client_sem, (DWORD) 1) ==
		   WAIT_OBJECT_0);
	while (WaitForSingleObject(icc->icc_server_sem, (DWORD) 1) ==
		   WAIT_OBJECT_0);

	/* initialize wait for multiple objects array */

	icc->icc_waits[0] = icc->icc_server_sem;
	icc->icc_waits[1] = icc->icc_client_proc;

	/* finally, start the thread for this client */

	gds__thread_start(reinterpret_cast < FPTR_INT_VOID_PTR > (ipi_server),
					  icc, THREAD_medium, 0, 0);

	/* return combined mapped area and number */

#ifdef  DEBUG_IP_TRACE
	gds__log("ipserver icc %8lX, comm %8lX", icc, comm);
#endif

	THD_mutex_unlock(&ipserver_private_data.ipics);
	return IPS_PACK_PARAMS(ipserver_private_data.users_per_map, ipserver_private_data.pages_per_user, mapped_area,
						   mapped_position);
}


static void ipi_end_thread( ICC icc)
{
/**************************************
 *
 *      i p i _ e n d _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *      End a thread's existence, close its handles,
 *      free its slot and whatnot.
 *
 **************************************/

	/* free up this thread's slot */

	IPM ipm = icc->icc_ipm;
	THD_mutex_lock(&ipserver_private_data.ipics);
	ipm->ipm_ids[icc->icc_slot] = 0;
	ipm->ipm_count--;

	/* if last slot was emptied, unmap and free structure, except
	   for the first map, which is always retained */

	if (!ipm->ipm_count && ipm != ipserver_private_data.first_ipm)
	{
		if (ipm->ipm_address) {
			UnmapViewOfFile(ipm->ipm_address);
			ipm->ipm_address = 0;
		}
		if (ipm->ipm_handle) {
			CloseHandle(ipm->ipm_handle);
			ipm->ipm_handle = 0;
		}

		if (ipserver_private_data.ipms == ipm){
			ipserver_private_data.ipms = ipm->ipm_next;
		} else {
			for (IPM pipm = ipserver_private_data.ipms;
				 pipm->ipm_next;
				 pipm = pipm->ipm_next)
			{
				if (pipm->ipm_next == ipm)
				{
					pipm->ipm_next = ipm->ipm_next;
					break;
				}
			}
		}
		ALLI_free((UCHAR *) ipm);
	}

	/* close handles for semaphores */

	CloseHandle(icc->icc_client_sem);
	CloseHandle(icc->icc_server_sem);
	CloseHandle(icc->icc_client_proc);
	ALLI_release((BLK) icc);
	THD_mutex_unlock(&ipserver_private_data.ipics);
}


static void ipi_server( ICC icc)
{
/**************************************
 *
 *      i p i _ s e r v e r
 *
 **************************************
 *
 * Functional description
 *      Main entrypoint of server.
 *
 **************************************/
	ips_comm_area *comm;
	TEXT *comm_ptr;

	// start this thread

	gds__thread_enable(-1);

	/* error handler */

	try {

	/* request processing loop */

	while (!(icc->icc_flags & ICCF_SHUTDOWN))
	{
		if (!wait_no_send(icc))
			break;
		GET_COMM_OBJECT;
		comm->ips_ops_count++;
#ifdef  DEBUG_IP_TRACE
		{
			char line[200];
			sprintf(line, "%8lX %8lX %8lX %d ipserver %s",
					(long) GetCurrentThreadId(), (long) icc,
					comm, comm->ips_operation, op_strings[comm->ips_operation]);
			gds__log(line);
		}
#endif
		switch (comm->ips_operation)
		{
		case op_attach:
		case op_create:
			attach_database(icc, static_cast<P_OP>(comm->ips_operation));
			break;

		case op_cancel_events:
			cancel_events(icc);
			break;

		case op_compile:
			compile(icc);
			break;

		case op_ddl:
			ddl(icc);
			break;

		case op_receive:
			receive_msg(icc);
			break;

		case op_seek_blob:
			seek_blob(icc);
			break;

		case op_send:
			send_msg(icc);
			break;

		case op_start:
			start(icc);
			break;

		case op_start_and_send:
			start_and_send(icc);
			break;

		case op_transact_request:
			transact_request(icc);
			break;

		case op_transaction:
			start_transaction(icc);
			break;

		case op_rollback:
		case op_commit:
		case op_commit_retaining:
		case op_rollback_retaining:
		case op_prepare2:
			end_transaction(icc, static_cast < P_OP > (comm->ips_operation));
			break;

		case op_detach:
			end_database(icc);
			break;

		case op_drop_database:
			drop_database(icc);
			break;

		case op_create_blob:
		case op_open_blob:
		case op_create_blob2:
		case op_open_blob2:
			open_blob(icc, static_cast < P_OP > (comm->ips_operation));
			break;

		case op_put_segment:
			put_segment(icc);
			break;

		case op_get_segment:
			get_segment(icc);
			break;

		case op_cancel_blob:
		case op_close_blob:
			end_blob(icc, static_cast < P_OP > (comm->ips_operation));
			break;

		case op_release:
			end_request(icc);
			break;

		case op_que_events:
			que_events(icc);
			break;

		case op_unwind:
			unwind(icc);
			break;

		case op_reconnect:
			reconnect(icc);
			break;

		case op_info_blob:
		case op_info_request:
		case op_info_database:
		case op_info_transaction:
		case op_info_sql:
		case op_service_info:
			info(icc, static_cast < P_OP > (comm->ips_operation));
			break;

		case op_get_slice:
			get_slice(icc);
			break;

		case op_put_slice:
			put_slice(icc);
			break;

		case op_allocate_stmt:
			allocate_statement(icc);
			break;

		case op_execute:
		case op_execute2:
			execute_statement(icc,
							  static_cast < P_OP > (comm->ips_operation));
			break;

		case op_exec_immediate:
		case op_exec_immediate2:
			execute_immediate(icc,
							  static_cast < P_OP > (comm->ips_operation));
			break;

		case op_fetch:
			fetch(icc);
			break;

		case op_free_stmt:
			end_statement(icc);
			break;

		case op_insert:
			insert(icc);
			break;

		case op_prepare_stmt:
			prepare_statement(icc);
			break;

		case op_set_cursor:
			set_cursor(icc);
			break;

		case op_service_attach:
			service_attach(icc);
			break;

		case op_service_detach:
			service_end(icc);
			break;

		case op_service_start:
			service_start(icc);
			break;

		case op_disconnect:
			icc->icc_flags |= ICCF_SHUTDOWN;
			break;

		default:
			gds__log("Interprocess Server op code %d unknown\n",
					 (int *)(ULONG) comm->ips_operation);
			icc->icc_flags |= ICCF_SHUTDOWN;
			break;
		}
	}

	/* left loop, so shut down */

	shutdown_attachments(icc);
	ipi_end_thread(icc);

	}	// try
	catch (const std::exception&) {
		gds__log("ipi_server: error during startup, shutting down");
	}
}


static void allocate_statement( ICC icc)
{
/**************************************
 *
 *      a l l o c a t e _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *      Allocate a statement handle.
 *
 **************************************/
	IDB idb;
	IPSERVER_ISR statement;
	FRBRD* handle;
	ISC_STATUS_ARRAY status_vector;
	ISC_STATUS result;
	ips_dsql *ips;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_dsql;
	idb = (IDB) ips->ips_db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	handle = NULL;

	/* allocate the statement and create a structure */

	result = GDS_DSQL_ALLOCATE(status_vector, &idb->idb_handle, &handle);
	if (!result)
	{
		statement = (IPSERVER_ISR) ALLOC(type_ipserver_isr);
		ips->ips_st_handle = (UCHAR *) statement;
		NOT_NULL(statement, TRUE);
		statement->isr_handle = handle;
		statement->isr_idb = idb;
		statement->isr_next = idb->idb_sql_requests;
		statement->isr_batch_flag = 0;
		statement->isr_max_recs = 0;
		statement->isr_packed = NULL;
		idb->idb_sql_requests = statement;
	}
	else {
		ips->ips_st_handle = NULL;
	}
	send_response(icc, status_vector);
}


static void attach_database( ICC icc, P_OP operation)
{
/**************************************
 *
 *      a t t a c h _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *      Process an attach or create packet.
 *
 **************************************/
	USHORT file_length, dpb_length, expanded_length;
	UCHAR *file_name, file_buf[256], *dpb, dpb_buf[256];
	UCHAR *expanded, expanded_buf[256];
	FRBRD* handle;
	ISC_STATUS_ARRAY status_vector;
	ISC_STATUS result;
	IDB idb;
	ips_object *ips;
	ips_string *ips_name;
	ips_string *ips_dpb;
	ips_string *ips_expanded;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_object;

	/* figure out file name buffer */

	ips_name = &comm->ips_buffers[IPS_ATTACH_NAME];
	file_length = (USHORT) ips_name->ips_cl_size;
	if (file_length <= sizeof(file_buf))
		file_name = file_buf;
	else
		file_name = get_buffer(comm, file_length, IPS_ATTACH_NAME);
	NOT_NULL(file_name, TRUE);
	IPS_SERVER(comm, IPS_ATTACH_NAME, file_name, file_length);

	/* figure out dpb buffer */

	ips_dpb = &comm->ips_buffers[IPS_ATTACH_DPB];
	dpb_length = (USHORT) ips_dpb->ips_cl_size;
	if (dpb_length <= sizeof(dpb_buf))
		dpb = dpb_buf;
	else
		dpb = get_buffer(comm, dpb_length, IPS_ATTACH_DPB);
	NOT_NULL(dpb, dpb_length);
	IPS_SERVER(comm, IPS_ATTACH_DPB, dpb, dpb_length);

	/* figure out expanded file name buffer */

	ips_expanded = &comm->ips_buffers[IPS_ATTACH_EXPANDED];
	expanded_length = (USHORT) ips_expanded->ips_cl_size;
	if (expanded_length <= sizeof(expanded_buf))
		expanded = expanded_buf;
	else
		expanded = get_buffer(comm, expanded_length, IPS_ATTACH_EXPANDED);
	NOT_NULL(expanded, expanded_length);
	IPS_SERVER(comm, IPS_ATTACH_EXPANDED, expanded, expanded_length);
	if (!transfer_buffers(icc, comm))
		return;

	/* get handle */

	handle = NULL;
	if (operation == op_attach)
		result = GDS_ATTACH_DATABASE(status_vector,
									 file_length,
									 reinterpret_cast < char *>(file_name),
									 GDS_REF(handle),
									 dpb_length,
									 reinterpret_cast < char *>(dpb));
	else
		result = GDS_CREATE_DATABASE(status_vector,
									 file_length,
									 reinterpret_cast < char *>(file_name),
									 GDS_REF(handle),
									 dpb_length,
									 reinterpret_cast < char *>(dpb), 0);

	/* allocate structure and return handle */

	if (!result)
	{
		idb = (IDB) ALLOC(type_idb);
		ips->ips_handle = (UCHAR *) idb;
		NOT_NULL(idb, TRUE);
		idb->idb_flags = IDBF_DATABASE_ATTACHMENT;
		idb->idb_handle = handle;
		idb->idb_next = icc->icc_databases;
		icc->icc_databases = idb;
	}
	else
		ips->ips_handle = NULL;
	send_response(icc, status_vector);
}


static void cancel_events( ICC icc)
{
/**************************************
 *
 *      c a n c e l _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *      Cancel an outstanding event call.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	IDB idb;
	IVNT event;
	SLONG id;
	ips_object *ips;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_object;
	idb = (IDB) ips->ips_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	id = ips->ips_parameter;

	/* perform operation, return response and remove event */

	GDS_CANCEL_EVENTS(status_vector, GDS_REF(idb->idb_handle), GDS_REF(id));
	for (event = idb->idb_events; event; event = event->ivnt_next)
	{
		if (event->ivnt_ast && event->ivnt_id == id)
		{
			event->ivnt_ast = NULL;
			event->ivnt_id = 0;
			break;
		}
	}
	send_response(icc, status_vector);
}


static USHORT check_statement_type( IPSERVER_ISR statement)
{
/**************************************
 *
 *      c h e c k _ s t a t e m e n t _ t y p e
 *
 **************************************
 *
 * Functional description
 *      Return the type of SQL statement.
 *
 **************************************/
	UCHAR buffer[16], *info;
	USHORT l, type;
	ISC_STATUS_ARRAY local_status;
	ISC_STATUS result;
	USHORT ret;
	BOOLEAN done;


	/* init to no batch allowed */

	ret = 0;
	done = FALSE;
	result = GDS_DSQL_SQL_INFO(local_status,
							   &statement->isr_handle,
							   sizeof(sql_info),
							   reinterpret_cast < char *>(sql_info),
							   sizeof(buffer),
							   reinterpret_cast < char *>(buffer));
	if (!result)
	{
		/* check the buffer for a batch fetch flag */

		for (info = buffer; (*info != isc_info_end) && !done;)
		{
			l = (USHORT)isc_vax_integer(reinterpret_cast<char*>(info + 1),
										 2);
			type = (USHORT)isc_vax_integer(reinterpret_cast<char*>(info + 3),
										 l);
			switch (*info)
			{
			case isc_info_sql_batch_fetch:
				ret = type;
				done = TRUE;
				break;

			case isc_info_error:
			case isc_info_truncated:
				done = TRUE;
				break;
			}
			info += 3 + l;
		}
	}
	return ret;
}


static void compile( ICC icc)
{
/**************************************
 *
 *      c o m p i l e
 *
 **************************************
 *
 * Functional description
 *      Compile a request.
 *
 **************************************/
	IDB idb;
	IRQ request;
	USHORT blr_length;
	UCHAR *blr;
	FRBRD* handle;
	ISC_STATUS_ARRAY status_vector;
	ISC_STATUS result;
	ips_compile_req *ips;
	ips_string *ips_blr;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_compile;
	idb = (IDB) ips->ips_db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* set up blr buffer */

	ips_blr = &comm->ips_buffers[IPS_COMPILE_REQ];
	blr_length = (USHORT) ips_blr->ips_cl_size;
	blr = get_buffer(comm, blr_length, IPS_COMPILE_REQ);
	NOT_NULL(blr, blr_length);
	IPS_SERVER(comm, IPS_COMPILE_REQ, blr, blr_length);
	if (!transfer_buffers(icc, comm))
		return;

	/* compile and get handle */

	handle = NULL;
	result = GDS_COMPILE(status_vector,
						 GDS_REF(idb->idb_handle),
						 GDS_REF(handle),
						 blr_length,
						 reinterpret_cast < char *>(GDS_VAL(blr)));

	/* create data structure and return handle */

	if (!result)
	{
		request = (IRQ) ALLOC(type_irq);
		ips->ips_rq_handle = (UCHAR *) request;
		NOT_NULL(request, TRUE);
		request->irq_handle = handle;
		request->irq_idb = idb;
		request->irq_next = idb->idb_requests;
		idb->idb_requests = request;
	}
	else
		ips->ips_rq_handle = NULL;
	send_response(icc, status_vector);
}


static void ddl( ICC icc)
{
/**************************************
 *
 *      d d l
 *
 **************************************
 *
 * Functional description
 *      Pass thru a DDL call.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	ITR transaction;
	IDB idb;
	UCHAR *buffer;
	USHORT length;
	ips_ddl *ips;
	ips_string *ips_ddl_string;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_ddl;
	idb = (IDB) ips->ips_db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	transaction = (ITR) ips->ips_tr_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);

	/* set up ddl buffer */

	ips_ddl_string = &comm->ips_buffers[IPS_DDL_DDL];
	length = (USHORT) ips_ddl_string->ips_cl_size;
	buffer = get_buffer(comm, length, IPS_DDL_DDL);
	NOT_NULL(buffer, length);
	IPS_SERVER(comm, IPS_DDL_DDL, buffer, length);
	if (!transfer_buffers(icc, comm))
		return;

	/* perform operation and return response */

	GDS_DDL(status_vector,
			GDS_REF(idb->idb_handle),
			GDS_REF(transaction->itr_handle),
			length, reinterpret_cast < char *>(GDS_VAL(buffer)));
	send_response(icc, status_vector);
}


static void drop_database( ICC icc)
{
/**************************************
 *
 *      d r o p _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *      End a request.
 *
 **************************************/
	IDB idb, *ptr;
	ISC_STATUS_ARRAY status_vector;
	ISC_STATUS code;
	ips_object *ips;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_object;
	idb = (IDB) ips->ips_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* drop database, release resources and send response */

	code = GDS_DROP_DATABASE(status_vector, &idb->idb_handle);
	send_response(icc, status_vector);
	if (!code || (code == isc_drdb_completed_with_errs))
	{
		while (idb->idb_events)
			release_event(idb->idb_events);
		while (idb->idb_requests)
			release_request(idb->idb_requests);
		while (idb->idb_sql_requests)
			release_sql_request(idb->idb_sql_requests);
		while (idb->idb_transactions)
			release_transaction(idb->idb_transactions);
		for (ptr = &icc->icc_databases; *ptr; ptr = &(*ptr)->idb_next)
		{
			if (*ptr == idb)
			{
				*ptr = idb->idb_next;
				break;
			}
		}
		ALLI_release((BLK) idb);
	}
}


static void end_blob( ICC icc, P_OP operation)
{
/**************************************
 *
 *      e n d _ b l o b
 *
 **************************************
 *
 * Functional description
 *      End a blob.
 *
 **************************************/
	IBL blob;
	ISC_STATUS_ARRAY status_vector;
	UCHAR *buffer;
	USHORT length;
	ips_object *ips;
	ips_string *ips_seg;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_object;
	blob = (IBL) ips->ips_handle;
	CHECK_HANDLE(blob, type_ibl, isc_bad_segstr_handle);

	/* set up buffer for last segment, if any */

	ips_seg = &comm->ips_buffers[IPS_CLOSE_BLOB_REM];
	length = (USHORT) ips_seg->ips_cl_size;
	if (length)
	{
		buffer = get_buffer(comm, length, IPS_CLOSE_BLOB_REM);
		NOT_NULL(buffer, length);
		IPS_SERVER(comm, IPS_CLOSE_BLOB_REM, buffer, length);
		if (!transfer_buffers(icc, comm))
			return;
	}

	/* perform operation, send response and free resources */

	if (operation == op_close_blob)
		GDS_CLOSE_BLOB(status_vector, GDS_REF(blob->ibl_handle));
	else
		GDS_CANCEL_BLOB(status_vector, GDS_REF(blob->ibl_handle));
	send_response(icc, status_vector);
	release_blob(blob);
}


static void end_database( ICC icc)
{
/**************************************
 *
 *      e n d _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *      End a request.
 *
 **************************************/
	IDB idb, *ptr;
	ISC_STATUS_ARRAY status_vector;
	ISC_STATUS result;
	ips_object *ips;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_object;
	idb = (IDB) ips->ips_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* detach database, send response and free resources */

	result = GDS_DETACH(status_vector, GDS_REF(idb->idb_handle));
	send_response(icc, status_vector);
	if (!result)
	{
		while (idb->idb_events)
			release_event(idb->idb_events);
		while (idb->idb_requests)
			release_request(idb->idb_requests);
		while (idb->idb_sql_requests)
			release_sql_request(idb->idb_sql_requests);
		while (idb->idb_transactions)
			release_transaction(idb->idb_transactions);
		for (ptr = &icc->icc_databases; *ptr; ptr = &(*ptr)->idb_next)
		{
			if (*ptr == idb)
			{
				*ptr = idb->idb_next;
				break;
			}
		}
		ALLI_release((BLK) idb);
	}
}


static void end_request( ICC icc)
{
/**************************************
 *
 *      e n d _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *      End a request.
 *
 **************************************/
	IRQ request;
	ISC_STATUS_ARRAY status_vector;
	ips_object *ips;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_object;
	request = (IRQ) ips->ips_handle;
	CHECK_HANDLE(request, type_irq, isc_bad_req_handle);

	/* process call, respond and free resources */

	GDS_RELEASE_REQUEST(status_vector, GDS_REF(request->irq_handle));
	send_response(icc, status_vector);
	release_request(request);
}


static void end_statement( ICC icc)
{
/**************************************
 *
 *      e n d _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *      Free a dynamic statement.
 *
 **************************************/
	IPSERVER_ISR statement;
	USHORT option;
	ISC_STATUS_ARRAY status_vector;
	ips_object *ips;
	ips_comm_area *comm;
	TEXT *comm_ptr;

/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_object;
	statement = (IPSERVER_ISR) ips->ips_handle;
	CHECK_HANDLE(statement, type_ipserver_isr, isc_bad_stmt_handle);
	option = (USHORT) ips->ips_parameter;

/* free statement, return response and free resources */

	GDS_DSQL_FREE(status_vector, &statement->isr_handle, option);

	if (statement->isr_handle)
	{
		ips->ips_handle = (UCHAR *) statement;
		send_response(icc, status_vector);
	}
	else
	{
		ips->ips_handle = NULL;
		send_response(icc, status_vector);
		release_sql_request(statement);
	}
}


static void end_transaction( ICC icc, P_OP operation)
{
/**************************************
 *
 *      e n d _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *      End a transaction.
 *
 **************************************/
	ITR transaction;
	UCHAR *msg;
	USHORT msg_length;
	ISC_STATUS_ARRAY status_vector;
	ISC_STATUS result;
	ips_object *ips;
	ips_string *ips_prep;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_object;
	transaction = (ITR) ips->ips_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);

	/* set up buffer for prepare2 */

	if (operation == op_prepare2)
	{
		ips_prep = &comm->ips_buffers[IPS_PREPARE_TRANS];
		msg_length = (USHORT) ips_prep->ips_cl_size;
		msg = get_buffer(comm, msg_length, IPS_PREPARE_TRANS);
		NOT_NULL(msg, msg_length);
		IPS_SERVER(comm, IPS_PREPARE_TRANS, msg, msg_length);
		if (!transfer_buffers(icc, comm))
			return;
	}

	/* perform operation as required */

	switch (operation)
	{
	case op_commit:
		result = GDS_COMMIT(status_vector, GDS_REF(transaction->itr_handle));
		if (!result)
			release_transaction(transaction);
		break;

	case op_rollback:
		result = GDS_ROLLBACK(status_vector,
							  GDS_REF(transaction->itr_handle));
		if (!result)
			release_transaction(transaction);
		break;

	case op_commit_retaining:
		result = GDS_COMMIT_RETAINING(status_vector,
									  GDS_REF(transaction->itr_handle));
		break;

	case op_rollback_retaining:
		result = GDS_ROLLBACK_RETAINING(status_vector,
										GDS_REF(transaction->itr_handle));
		break;

	case op_prepare2:
		result = GDS_PREPARE2(status_vector,
							  GDS_REF(transaction->itr_handle),
							  msg_length,
							  reinterpret_cast<UCHAR*>(GDS_VAL(msg)));
		if (!result)
			transaction->itr_flags |= ITR_limbo;
		break;
	}
	send_response(icc, status_vector);
}


static void event_ast( IVNT event, USHORT length, UCHAR * data)
{
/**************************************
 *
 *      e v e n t _ a s t
 *
 **************************************
 *
 * Functional description
 *      Send off event stuff.
 *
 **************************************/
	COPYDATASTRUCT cpdata;
	ULONG result;


	if (!event->ivnt_ast)
		return;
	cpdata.dwData = (DWORD) event->ivnt_id;
	cpdata.cbData = (DWORD) length;
	cpdata.lpData = (LPVOID) data;
	result =
		SendMessage(event->ivnt_window, WM_COPYDATA, (WPARAM) IPSVR_window,
					(LPARAM) & cpdata);
	event->ivnt_ast = NULL;
}


static void execute_immediate( ICC icc, P_OP operation)
{
/**************************************
 *
 *      e x e c u t e _ i m m e d i a t e
 *
 **************************************
 *
 * Functional description
 *      Prepare and execute a statement.
 *
 **************************************/
	IDB idb;
	ITR transaction;
	FRBRD* handle;
	USHORT length, dialect, in_blr_length, in_msg_type, in_msg_length,
		out_blr_length, out_msg_type, out_msg_length, parser_version;
	UCHAR *string, *in_blr, *in_msg, *out_blr, *out_msg;
	ISC_STATUS_ARRAY status_vector;
	ips_dsql *ips;
	ips_string *ips_blr;
	ips_string *ips_msg;
	ips_string *ips_sql;
	ips_string *ips_blr_out;
	ips_string *ips_msg_out;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_dsql;
	idb = (IDB) ips->ips_db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	transaction = (ITR) ips->ips_tr_handle;
	if (transaction)
	{
		CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
		handle = transaction->itr_handle;
	}
	else
		handle = NULL;
	in_msg_type = ips->ips_msg_type;
	dialect = (USHORT) ips->ips_parameter;

	/* set up sql */

	ips_sql = &comm->ips_buffers[IPS_DSQL_EXEC_IMMED_SQL];
	length = (USHORT) ips_sql->ips_cl_size;
	string = get_buffer(comm, length, IPS_DSQL_EXEC_IMMED_SQL);
	NOT_NULL(string, length);
	IPS_SERVER(comm, IPS_DSQL_EXEC_IMMED_SQL, string, length);

	/* set up blr */

	ips_blr = &comm->ips_buffers[IPS_DSQL_EXEC_IMMED_BLR];
	in_blr_length = (USHORT) ips_blr->ips_cl_size;
	in_blr = get_buffer(comm, in_blr_length, IPS_DSQL_EXEC_IMMED_BLR);
	NOT_NULL(in_blr, in_blr_length);
	IPS_SERVER(comm, IPS_DSQL_EXEC_IMMED_BLR, in_blr, in_blr_length);

	/* set up message */

	ips_msg = &comm->ips_buffers[IPS_DSQL_EXEC_IMMED_MSG];
	in_msg_length = (USHORT) ips_msg->ips_cl_size;
	in_msg = get_buffer(comm, in_msg_length, IPS_DSQL_EXEC_IMMED_MSG);
	NOT_NULL(in_msg, in_msg_length);
	IPS_SERVER(comm, IPS_DSQL_EXEC_IMMED_MSG, in_msg, in_msg_length);

	/* set up output message and blr if necessary */

	if (operation == op_exec_immediate2)
	{
		out_msg_type = ips->ips_msg_out;
		ips_blr_out = &comm->ips_buffers[IPS_DSQL_EXEC_IMMED2_BLR_OUT];
		out_blr_length = (USHORT) ips_blr_out->ips_cl_size;
		out_blr =
			get_buffer(comm, out_blr_length, IPS_DSQL_EXEC_IMMED2_BLR_OUT);
		NOT_NULL(out_blr, out_blr_length);
		IPS_SERVER(comm, IPS_DSQL_EXEC_IMMED2_BLR_OUT, out_blr,
				   out_blr_length);
		ips_msg_out = &comm->ips_buffers[IPS_DSQL_EXEC_IMMED2_MSG_OUT];
		out_msg_length = (USHORT) ips_msg_out->ips_cl_size;
		out_msg =
			get_buffer(comm, out_msg_length, IPS_DSQL_EXEC_IMMED2_MSG_OUT);
		NOT_NULL(out_msg, out_msg_length);
		IPS_SERVER(comm, IPS_DSQL_EXEC_IMMED2_MSG_OUT, out_msg,
				   out_msg_length);
	}
	else
	{
		out_blr_length = out_msg_type = out_msg_length = 0;
		out_blr = out_msg = NULL;
	}
	if (!transfer_buffers(icc, comm))
		return;

	/* NT Local access:  Client and server are always the same version */
	/* Since the API to GDS_DSQL_EXECUTE_IMMED is public and can not be changed, there needs to
	 * be a way to send the parser version to DSQL so that the parser can compare the keyword
	 * version to the parser version.  To accomplish this, the parser version is combined with
	 * the client dialect and sent across that way.  In dsql8_execute_immediate, the parser version
	 * and client dialect are separated and passed on to their final desintations.  The information
	 * is combined as follows:
	 *     Dialect * 10 + parser_version
	 *
	 * and is extracted in dsql8_execute_immediate as follows:
	 *      parser_version = ((dialect *10)+parser_version)%10
	 *      client_dialect = ((dialect *10)+parser_version)/10
	 *
	 * For example, parser_version = 1 and client dialect = 1
	 *
	 *  combined = (1 * 10) + 1 == 11
	 *
	 *  parser = (combined) %10 == 1
	 *  dialect = (combined) / 19 == 1
	 */

	parser_version = 2;

	/* execute the statement, respond and clean up */

	GDS_DSQL_EXECUTE_IMMED(status_vector,
						   &idb->idb_handle,
						   &handle,
						   length,
						   reinterpret_cast < char *>(string),
						   (USHORT) ((dialect * 10) + parser_version),
						   in_blr_length,
						   reinterpret_cast < char *>(in_blr),
						   in_msg_type,
						   in_msg_length,
						   reinterpret_cast < char *>(in_msg),
						   out_blr_length,
						   reinterpret_cast < char *>(out_blr),
						   out_msg_type,
						   out_msg_length,
						   reinterpret_cast < char *>(out_msg));
	if (transaction && !handle)
	{
		release_transaction(transaction);
		transaction = NULL;
	}
	else if (!transaction && handle)
	{
		transaction = make_transaction(idb, handle);
		NOT_NULL(transaction, TRUE);
	}
	ips->ips_tr_handle = (UCHAR *) transaction;
	send_response(icc, status_vector);
}


static void execute_statement( ICC icc, P_OP operation)
{
/**************************************
 *
 *      e x e c u t e _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *      Execute a non-SELECT dynamic SQL statement.
 *
 **************************************/
	ITR transaction;
	IPSERVER_ISR statement;
	FRBRD* handle;
	USHORT in_blr_length, in_msg_type, in_msg_length,
		out_blr_length, out_msg_type, out_msg_length;
	UCHAR *in_blr, *in_msg, *out_blr, *out_msg;
	ISC_STATUS_ARRAY status_vector;
	ips_dsql *ips;
	ips_string *ips_blr;
	ips_string *ips_msg;
	ips_string *ips_blr_out;
	ips_string *ips_msg_out;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_dsql;
	transaction = (ITR) ips->ips_tr_handle;
	if (transaction)
	{
		CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
		handle = transaction->itr_handle;
	}
	else
		handle = NULL;
	statement = (IPSERVER_ISR) ips->ips_st_handle;
	CHECK_HANDLE(statement, type_ipserver_isr, isc_bad_stmt_handle);
	in_msg_type = ips->ips_msg_type;

	/* set up blr */

	ips_blr = &comm->ips_buffers[IPS_DSQL_EXEC_BLR];
	in_blr_length = (USHORT) ips_blr->ips_cl_size;
	in_blr = get_buffer(comm, in_blr_length, IPS_DSQL_EXEC_BLR);
	NOT_NULL(in_blr, in_blr_length);
	IPS_SERVER(comm, IPS_DSQL_EXEC_BLR, in_blr, in_blr_length);

	/* set up message */

	ips_msg = &comm->ips_buffers[IPS_DSQL_EXEC_MSG];
	in_msg_length = (USHORT) ips_msg->ips_cl_size;
	in_msg = get_buffer(comm, in_msg_length, IPS_DSQL_EXEC_MSG);
	NOT_NULL(in_msg, in_msg_length);
	IPS_SERVER(comm, IPS_DSQL_EXEC_MSG, in_msg, in_msg_length);

	/* set up output message and blr if necessary */

	if (operation == op_execute2)
	{
		out_msg_type = ips->ips_msg_out;
		ips_blr_out = &comm->ips_buffers[IPS_DSQL_EXEC2_BLR_OUT];
		out_blr_length = (USHORT) ips_blr_out->ips_cl_size;
		out_blr = get_buffer(comm, out_blr_length, IPS_DSQL_EXEC2_BLR_OUT);
		NOT_NULL(out_blr, out_blr_length);
		IPS_SERVER(comm, IPS_DSQL_EXEC2_BLR_OUT, out_blr, out_blr_length);
		ips_msg_out = &comm->ips_buffers[IPS_DSQL_EXEC2_MSG_OUT];
		out_msg_length = (USHORT) ips_msg_out->ips_cl_size;
		out_msg = get_buffer(comm, out_msg_length, IPS_DSQL_EXEC2_MSG_OUT);
		NOT_NULL(out_msg, out_msg_length);
		IPS_SERVER(comm, IPS_DSQL_EXEC2_MSG_OUT, out_msg, out_msg_length);
	}
	else
	{
		out_blr_length = out_msg_type = out_msg_length = 0;
		out_blr = out_msg = NULL;
	}
	if (!transfer_buffers(icc, comm))
		return;

	/* execute, get results and clean up */

	GDS_DSQL_EXECUTE(status_vector,
					 &handle,
					 &statement->isr_handle,
					 in_blr_length,
					 reinterpret_cast < char *>(in_blr),
					 in_msg_type,
					 in_msg_length,
					 reinterpret_cast < char *>(in_msg),
					 out_blr_length,
					 reinterpret_cast < char *>(out_blr),
					 out_msg_type,
					 out_msg_length, reinterpret_cast < char *>(out_msg));
	if (transaction && !handle)
	{
		release_transaction(transaction);
		transaction = NULL;
	}
	else if (!transaction && handle)
	{
		transaction = make_transaction(statement->isr_idb, handle);
		NOT_NULL(transaction, TRUE);
	}
	ips->ips_tr_handle = (UCHAR *) transaction;
	send_response(icc, status_vector);
}


static void fetch( ICC icc)
{
/**************************************
 *
 *      f e t c h
 *
 **************************************
 *
 * Functional description
 *      Fetch next record from a dynamic SQL cursor.
 *
 **************************************/
	IPSERVER_ISR statement;
	USHORT records, total_length, blr_length, msg_type, msg_length;
	UCHAR *blr, *msg;
	ISC_STATUS s;
	ISC_STATUS_ARRAY status_vector;
	ips_dsql *ips;
	ips_string *ips_blr;
	ips_string *ips_msg;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_dsql;
	statement = (IPSERVER_ISR) ips->ips_st_handle;
	CHECK_HANDLE(statement, type_ipserver_isr, isc_bad_stmt_handle);
	msg_type = ips->ips_msg_type;

	/* set up blr */

	ips_blr = &comm->ips_buffers[IPS_DSQL_FETCH_BLR];
	blr_length = (USHORT) ips_blr->ips_cl_size;
	blr = get_buffer(comm, blr_length, IPS_DSQL_FETCH_BLR);
	NOT_NULL(blr, blr_length);
	IPS_SERVER(comm, IPS_DSQL_FETCH_BLR, blr, blr_length);

	/* set up message */

	ips_msg = &comm->ips_buffers[IPS_DSQL_FETCH_MSG];
	total_length = (USHORT) ips_msg->ips_cl_size;
	msg_length = total_length / ips->ips_rec_count;
	msg = get_buffer(comm, total_length, IPS_DSQL_FETCH_MSG);
	NOT_NULL(msg, msg_length);
	IPS_SERVER(comm, IPS_DSQL_FETCH_MSG, msg, total_length);
	if (!transfer_buffers(icc, comm))
		return;

	/* pack in records, keeping final result of operation for eof */

	for (records = 0; records < ips->ips_rec_count; records++)
	{
		s = GDS_DSQL_FETCH(status_vector,
						   &statement->isr_handle,
						   blr_length,
						   reinterpret_cast < char *>(blr),
						   msg_type,
						   msg_length, reinterpret_cast < char *>(msg));

		/* point to next packed record */

		msg += msg_length;
		if (s)
			break;
	}
	ips->ips_rec_count = records;
	ips->ips_parameter = s;
	send_response(icc, status_vector);
}


static UCHAR *get_buffer( IPS comm, USHORT length, USHORT n)
{
/**************************************
 *
 *      g e t _ b u f f e r
 *
 **************************************
 *
 * Functional description
 *      Get a buffer "big enough".
 *
 **************************************/

	if (comm->ips_buffers[n].ips_sv_alloced < length)
	{
		if (comm->ips_buffers[n].ips_sv_buffer)
			gds__free(comm->ips_buffers[n].ips_sv_buffer);
		comm->ips_buffers[n].ips_sv_alloced = length;
		comm->ips_buffers[n].ips_sv_buffer = (UCHAR*)gds__alloc((SLONG) length);
	}

	/* Make sure that the buffer returned is empty */
	if (comm->ips_buffers[n].ips_sv_buffer)
		memset(comm->ips_buffers[n].ips_sv_buffer, 0,
			   comm->ips_buffers[n].ips_sv_alloced);

	return comm->ips_buffers[n].ips_sv_buffer;
}


static void get_segment( ICC icc)
{
/**************************************
 *
 *      g e t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *      Get a single blob segment.
 *
 **************************************/
	IBL blob;
	USHORT buffer_length, length;
	UCHAR *buffer;
	ISC_STATUS_ARRAY status_vector;
	ips_segment *ips;
	ips_string *ips_seg;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_segment;
	blob = (IBL) ips->ips_bl_handle;
	CHECK_HANDLE(blob, type_ibl, isc_bad_segstr_handle);

	/* set up segment buffer */

	ips_seg = &comm->ips_buffers[IPS_BLOB_SEGMENT];
	buffer_length = (USHORT) ips_seg->ips_cl_size;
	buffer = get_buffer(comm, buffer_length, IPS_BLOB_SEGMENT);
	NOT_NULL(buffer, buffer_length);
	IPS_SERVER(comm, IPS_BLOB_SEGMENT, buffer, buffer_length);

	/* get the segment data - return it regardless of result
	 * as that is what the client side expects
	 * isc_segment can be returned in the status vector,
	 * but that the data is still valid.
	 */

	length = 0;
	GDS_GET_SEGMENT(status_vector,
					GDS_REF(blob->ibl_handle),
					GDS_REF(length),
					buffer_length,
					reinterpret_cast < char *>(GDS_VAL(buffer)));
	if (!transfer_buffers(icc, comm))
		return;
	ips->ips_length = length;
	send_response(icc, status_vector);
}


static void get_slice( ICC icc)
{
/**************************************
 *
 *      g e t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *      Get slice of array;
 *
 **************************************/
	IDB idb;
	ITR transaction;
	ISC_STATUS_ARRAY status_vector;
	SSHORT sdl_length, param_length;
	ULONG slice_length, return_length, *params, parameters[32];
	UCHAR *sdl, sdl_buf[256], *slice;
	struct bid array_id;
	ips_slice *ips;
	ips_string *ips_sdl;
	ips_string *ips_parms;
	ips_string *ips_data;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_slice;
	idb = (IDB) ips->ips_db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	transaction = (ITR) ips->ips_tr_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
	array_id.bid_relation_id = ips->ips_rel_id;
	array_id.bid_number = ips->ips_number;

	/* set up sdl */

	ips_sdl = &comm->ips_buffers[IPS_SLICE_SDL];
	sdl_length = (SSHORT) ips_sdl->ips_cl_size;
	if (sdl_length <= (SLONG) sizeof(sdl_buf))
		sdl = sdl_buf;
	else
		sdl = get_buffer(comm, sdl_length, IPS_SLICE_SDL);
	NOT_NULL(sdl, sdl_length);
	IPS_SERVER(comm, IPS_SLICE_SDL, sdl, sdl_length);

	/* set up parameters */

	ips_parms = &comm->ips_buffers[IPS_SLICE_PARAM];
	param_length = (SSHORT) ips_parms->ips_cl_size;
	if (param_length <= (SLONG) sizeof(parameters))
		params = parameters;
	else
		params = (ULONG *) get_buffer(comm, param_length, IPS_SLICE_PARAM);
	NOT_NULL(params, param_length);
	IPS_SERVER(comm, IPS_SLICE_PARAM, (UCHAR *) params, param_length);

	/* set up slice data buffer */

	ips_data = &comm->ips_buffers[IPS_SLICE_BUFFER];
	slice_length = ips_data->ips_cl_size;
	if (slice_length)
	{
		slice = (UCHAR*)gds__alloc(slice_length);
		NOT_NULL(slice, TRUE);
		memset(slice, 0, slice_length);
	}
	else
		slice = NULL;
	IPS_SERVER(comm, IPS_SLICE_BUFFER, slice, slice_length);
	if (!transfer_buffers(icc, comm))
	{
		if (slice)
		{
			gds__free(slice);
		}
		return;
	}

	/* get and return slice */

	GDS_GET_SLICE(status_vector,
				  GDS_REF(idb->idb_handle),
				  GDS_REF(transaction->itr_handle),
				  (GDS_QUAD *) & array_id,
				  sdl_length,
				  reinterpret_cast < char *>(sdl),
				  param_length,
				  reinterpret_cast < long *>(params),
				  slice_length,
				  slice, reinterpret_cast < long *>(GDS_REF(return_length)));
	ips->ips_length = return_length;
	send_response(icc, status_vector);
	if (slice)
	{
		gds__free(slice);
	}
}


static void info( ICC icc, P_OP operation)
{
/**************************************
 *
 *      i n f o
 *
 **************************************
 *
 * Functional description
 *      Issue information.
 *
 **************************************/
	FRBRD* handle;
	ISC_STATUS_ARRAY status_vector;
	USHORT item_length, recv_item_length, buffer_length, incarnation;
	UCHAR *buffer, *items, *recv_items;
	UCHAR items_buf[128], recv_items_buf[128];
	IDB idb;
	ITR transaction;
	IBL blob;
	IRQ request;
	IPSERVER_ISR statement;
	ips_object *ips;
	ips_string *ips_items;
	ips_string *ips_data;
	ips_string *ips_rcv_items;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	comm->ips_operation = op_info_blob;
	ips = &comm->ips_operations.ips_op_object;
	handle = (FRBRD *)ips->ips_handle;
	if (operation == op_info_request)
		incarnation = (USHORT) ips->ips_parameter;

	/* set up items buffer */

	ips_items = &comm->ips_buffers[IPS_INFO_ITEMS];
	item_length = (USHORT) ips_items->ips_cl_size;
	if (item_length <= sizeof(items_buf))
		items = items_buf;
	else
		items = get_buffer(comm, item_length, IPS_INFO_ITEMS);
	NOT_NULL(items, item_length);
	IPS_SERVER(comm, IPS_INFO_ITEMS, items, item_length);

	/* set up data buffer */

	ips_data = &comm->ips_buffers[IPS_INFO_DATA];
	buffer_length = (USHORT) ips_data->ips_cl_size;
	buffer = get_buffer(comm, buffer_length, IPS_INFO_DATA);
	NOT_NULL(buffer, buffer_length);
	IPS_SERVER(comm, IPS_INFO_DATA, buffer, buffer_length);

	/* set up receive items buffer if necessary */

	if (operation == op_service_info)
	{
		ips_rcv_items = &comm->ips_buffers[IPS_QUERY_RECV_ITEMS];
		recv_item_length = (USHORT) ips_rcv_items->ips_cl_size;
		if (recv_item_length <= sizeof(recv_items_buf))
			recv_items = recv_items_buf;
		else
			recv_items =
				get_buffer(comm, recv_item_length, IPS_QUERY_RECV_ITEMS);
		NOT_NULL(recv_items, recv_item_length);
		IPS_SERVER(comm, IPS_QUERY_RECV_ITEMS, recv_items, recv_item_length);
	}
	if (!transfer_buffers(icc, comm))
		return;

	/* perform operation */

	switch (operation)
	{
	case op_info_database:
		idb = (IDB) handle;
		CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
		GDS_DATABASE_INFO(status_vector,
						  GDS_REF(idb->idb_handle),
						  item_length,
						  reinterpret_cast < char *>(items),
						  buffer_length,
						  reinterpret_cast < char *>(GDS_VAL(buffer)));
		break;

	case op_info_blob:
		blob = (IBL) handle;
		CHECK_HANDLE(blob, type_ibl, isc_bad_segstr_handle);
		GDS_BLOB_INFO(status_vector,
					  GDS_REF(blob->ibl_handle),
					  item_length,
					  reinterpret_cast < char *>(items),
					  buffer_length,
					  reinterpret_cast < char *>(GDS_VAL(buffer)));
		break;

	case op_info_request:
		request = (IRQ) handle;
		CHECK_HANDLE(request, type_irq, isc_bad_req_handle);
		GDS_REQUEST_INFO(status_vector,
						 GDS_REF(request->irq_handle),
						 incarnation,
						 item_length,
						 reinterpret_cast < char *>(items),
						 buffer_length,
						 reinterpret_cast < char *>(GDS_VAL(buffer)));
		break;

	case op_info_transaction:
		transaction = (ITR) handle;
		CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
		GDS_TRANSACTION_INFO(status_vector,
							 GDS_REF(transaction->itr_handle),
							 item_length,
							 reinterpret_cast < char *>(items),
							 buffer_length,
							 reinterpret_cast < char *>(GDS_VAL(buffer)));
		break;

	case op_service_info:
		idb = (IDB) handle;
		CHECK_HANDLE(idb, type_idb, isc_bad_svc_handle);
		GDS_SERVICE_QUERY(status_vector, &idb->idb_handle, 0,	/* reserved */
						  item_length,
						  reinterpret_cast < char *>(items),
						  recv_item_length,
						  reinterpret_cast < char *>(recv_items),
						  buffer_length, reinterpret_cast < char *>(buffer));
		break;

	case op_info_sql:
		statement = (IPSERVER_ISR) handle;
		CHECK_HANDLE(statement, type_ipserver_isr, isc_bad_stmt_handle);
		GDS_DSQL_SQL_INFO(status_vector,
						  &statement->isr_handle,
						  item_length,
						  reinterpret_cast < char *>(items),
						  buffer_length, reinterpret_cast < char *>(buffer));
		break;
	}

	/* return results */

	send_response(icc, status_vector);
}


static void insert( ICC icc)
{
/**************************************
 *
 *      i n s e r t
 *
 **************************************
 *
 * Functional description
 *      Insert next record into a dynamic SQL cursor.
 *
 **************************************/
	IPSERVER_ISR statement;
	USHORT blr_length, msg_type, msg_length;
	UCHAR *blr, *msg;
	ISC_STATUS_ARRAY status_vector;
	ips_dsql *ips;
	ips_string *ips_blr;
	ips_string *ips_msg;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_dsql;
	statement = (IPSERVER_ISR) ips->ips_st_handle;
	CHECK_HANDLE(statement, type_ipserver_isr, isc_bad_stmt_handle);
	msg_type = ips->ips_msg_type;

	/* set up blr */

	ips_blr = &comm->ips_buffers[IPS_DSQL_INSERT_BLR];
	blr_length = (USHORT) ips_blr->ips_cl_size;
	blr = get_buffer(comm, blr_length, IPS_DSQL_INSERT_BLR);
	NOT_NULL(blr, blr_length);
	IPS_SERVER(comm, IPS_DSQL_INSERT_BLR, blr, blr_length);

	/* set up message */

	ips_msg = &comm->ips_buffers[IPS_DSQL_INSERT_MSG];
	msg_length = (USHORT) ips_msg->ips_cl_size;
	msg = get_buffer(comm, msg_length, IPS_DSQL_INSERT_MSG);
	NOT_NULL(msg, msg_length);
	IPS_SERVER(comm, IPS_DSQL_INSERT_MSG, msg, msg_length);
	if (!transfer_buffers(icc, comm))
		return;

	/* insert the record and return */

	GDS_DSQL_INSERT(status_vector,
					&statement->isr_handle,
					blr_length,
					reinterpret_cast < char *>(blr),
					msg_type, msg_length, reinterpret_cast < char *>(msg));
	send_response(icc, status_vector);
}


static IPM make_map(USHORT map_number)
{
/**************************************
 *
 *      m a k e _ m a p
 *
 **************************************
 *
 * Functional description
 *      Create a mapped file.   This
 *      returns -
 *          0 - mapped file already exists
 *         -1 - mapping  or creation error
 *      other - pointer to new structure
 *
 **************************************/

	HANDLE map_handle;
	LPVOID map_address;
	// Here we better be 100% SURE this won't overflow!
	TEXT name_buffer[128];

	/* create the mapped file name and try to open it */

	sprintf(name_buffer, IPI_MAPPED_FILE_NAME, Config::getIpcName(), map_number);
	map_handle = CreateFileMapping((HANDLE) 0xFFFFFFFF,
								   ISC_get_security_desc(),
								   PAGE_READWRITE,
								   0L,
								   IPS_MAPPED_SIZE(ipserver_private_data.users_per_map,
												   ipserver_private_data.pages_per_user),
								   name_buffer);
	if (map_handle && GetLastError() == ERROR_ALREADY_EXISTS) {
		return NULL;
	}
	if (!map_handle) {
		return (IPM) -1;
	}
	map_address = MapViewOfFile(map_handle, FILE_MAP_WRITE, 0L, 0L,
								IPS_MAPPED_SIZE(ipserver_private_data.users_per_map,
												ipserver_private_data.pages_per_user));
	if (!map_address) {
		CloseHandle(map_handle);
		return (IPM) -1;
	}

	/* allocate a structure and initialize it */

	IPM ipm = (IPM) ALLI_alloc(sizeof(struct ipm));
	if (!ipm) {
		UnmapViewOfFile(map_address);
		CloseHandle(map_handle);
		return (IPM) -1;
	}
	ipm->ipm_handle = map_handle;
	ipm->ipm_address = map_address;
	ipm->ipm_number = map_number;
	ipm->ipm_count = 0;
	for (USHORT i = 0; i < ipserver_private_data.users_per_map; i++) {
		ipm->ipm_ids[i] = 0;
	}
	ipm->ipm_next = ipserver_private_data.ipms;
	ipserver_private_data.ipms = ipm;
	return ipm;
}


static ITR make_transaction( IDB idb, FRBRD * handle)
{
/**************************************
 *
 *      m a k e _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *      Create a local transaction handle.
 *
 **************************************/
	ITR transaction;


	transaction = (ITR) ALLOC(type_itr);
	if (transaction)
	{
		transaction->itr_idb = idb;
		transaction->itr_handle = handle;
		transaction->itr_next = idb->idb_transactions;
		idb->idb_transactions = transaction;
	}
	return transaction;
}


static void open_blob( ICC icc, P_OP op)
{
/**************************************
 *
 *      o p e n _ b l o b
 *
 **************************************
 *
 * Functional description
 *      Open or create a new blob.
 *
 **************************************/
	IDB idb;
	IBL blob;
	ITR transaction;
	FRBRD * handle;
	ISC_STATUS_ARRAY status_vector;
	ISC_STATUS result;
	USHORT bpb_length;
	UCHAR *bpb, bpb_buf[128];
	struct bid blob_id;
	ips_blob *ips;
	ips_string *ips_bpb;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_blob;
	idb = (IDB) ips->ips_db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	transaction = (ITR) ips->ips_tr_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
	handle = NULL;
	bpb_length = 0;

	/* set up bpb if needed */

	if (op == op_open_blob2 || op == op_create_blob2)
	{
		ips_bpb = &comm->ips_buffers[IPS_BLOB_BPB];
		bpb_length = (USHORT) ips_bpb->ips_cl_size;
		if (bpb_length <= sizeof(bpb_buf))
			bpb = bpb_buf;
		else
			bpb = get_buffer(comm, bpb_length, IPS_BLOB_BPB);
		NOT_NULL(bpb, bpb_length);
		IPS_SERVER(comm, IPS_BLOB_BPB, bpb, bpb_length);
	}
	if (!transfer_buffers(icc, comm))
		return;

	/* perform operation and return handle */

	switch (op)
	{
	case op_open_blob:
		blob_id.bid_relation_id = ips->ips_rel_id;
		blob_id.bid_number = ips->ips_bid_number;
		result = GDS_OPEN_BLOB(status_vector,
							   GDS_REF(idb->idb_handle),
							   GDS_REF(transaction->itr_handle),
							   GDS_REF(handle), (GDS_QUAD *) & blob_id);
		break;

	case op_create_blob:
		result = GDS_CREATE_BLOB(status_vector,
								 GDS_REF(idb->idb_handle),
								 GDS_REF(transaction->itr_handle),
								 GDS_REF(handle), (GDS_QUAD *) & blob_id);
		ips->ips_rel_id = blob_id.bid_relation_id;
		ips->ips_bid_number = blob_id.bid_number;
		break;

	case op_open_blob2:
		blob_id.bid_relation_id = ips->ips_rel_id;
		blob_id.bid_number = ips->ips_bid_number;
		result = GDS_OPEN_BLOB2(status_vector,
								GDS_REF(idb->idb_handle),
								GDS_REF(transaction->itr_handle),
								GDS_REF(handle),
								(GDS_QUAD *) & blob_id,
								bpb_length, reinterpret_cast<UCHAR*>(bpb));
		break;

	case op_create_blob2:
		result = GDS_CREATE_BLOB2(status_vector,
								  GDS_REF(idb->idb_handle),
								  GDS_REF(transaction->itr_handle),
								  GDS_REF(handle),
								  (GDS_QUAD *) & blob_id,
								  bpb_length,
								  reinterpret_cast < char *>(bpb));
		ips->ips_rel_id = blob_id.bid_relation_id;
		ips->ips_bid_number = blob_id.bid_number;
		break;
	}
	if (!result)
	{
		blob = (IBL) ALLOC(type_ibl);
		ips->ips_bl_handle = (UCHAR *) blob;
		NOT_NULL(blob, TRUE);
		blob->ibl_handle = handle;
		blob->ibl_idb = idb;
		blob->ibl_itr = transaction;
		blob->ibl_next = transaction->itr_blobs;
		transaction->itr_blobs = blob;
	}
	else
		ips->ips_bl_handle = NULL;
	send_response(icc, status_vector);
}


static void prepare_statement( ICC icc)
{
/**************************************
 *
 *      p r e p a r e _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *      Prepare a dynamic SQL statement for execution.
 *
 **************************************/
	ITR transaction;
	IPSERVER_ISR statement;
	USHORT length, item_length, buffer_length, dialect, parser_version;
	UCHAR *string, *items, items_buf[128], *buffer;
	ISC_STATUS_ARRAY status_vector;
	FRBRD * handle;
	ips_dsql *ips;
	ips_string *ips_prep_string;
	ips_string *ips_prep_items;
	ips_string *ips_prep_buffer;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_dsql;
	transaction = (ITR) ips->ips_tr_handle;
	if (transaction)
	{
		CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
		handle = transaction->itr_handle;
	}
	else
		handle = NULL;
	statement = (IPSERVER_ISR) ips->ips_st_handle;
	CHECK_HANDLE(statement, type_ipserver_isr, isc_bad_stmt_handle);
	dialect = (USHORT) ips->ips_parameter;

	/* set up string */

	ips_prep_string = &comm->ips_buffers[IPS_DSQL_PREP_STRING];
	length = (USHORT) ips_prep_string->ips_cl_size;
	string = get_buffer(comm, length, IPS_DSQL_PREP_STRING);
	NOT_NULL(string, length);
	IPS_SERVER(comm, IPS_DSQL_PREP_STRING, string, length);

	/* set up items */

	ips_prep_items = &comm->ips_buffers[IPS_DSQL_PREP_ITEMS];
	item_length = (USHORT) ips_prep_items->ips_cl_size;
	if (item_length <= sizeof(items_buf))
		items = items_buf;
	else
		items = get_buffer(comm, item_length, IPS_DSQL_PREP_ITEMS);
	NOT_NULL(items, item_length);
	IPS_SERVER(comm, IPS_DSQL_PREP_ITEMS, items, item_length);

	/* set up buffer */

	ips_prep_buffer = &comm->ips_buffers[IPS_DSQL_PREP_BUFFER];
	buffer_length = (USHORT) ips_prep_buffer->ips_cl_size;
	buffer = get_buffer(comm, buffer_length, IPS_DSQL_PREP_BUFFER);
	NOT_NULL(buffer, buffer_length);
	IPS_SERVER(comm, IPS_DSQL_PREP_BUFFER, buffer, buffer_length);
	if (!transfer_buffers(icc, comm))
		return;

	/* NT Local access:  Client and server are always the same version */
	/* Since the API to GDS_DSQL_PREPARE is public and can not be changed, there needs to
	 * be a way to send the parser version to DSQL so that the parser can compare the keyword
	 * version to the parser version.  To accomplish this, the parser version is combined with
	 * the client dialect and sent across that way.  In dsql8_prepare_statement, the parser version
	 * and client dialect are separated and passed on to their final desintations.  The information
	 * is combined as follows:
	 *     Dialect * 10 + parser_version
	 *
	 * and is extracted in dsql8_prepare_statement as follows:
	 *      parser_version = ((dialect *10)+parser_version)%10
	 *      client_dialect = ((dialect *10)+parser_version)/10
	 *
	 * For example, parser_version = 1 and client dialect = 1
	 *
	 *  combined = (1 * 10) + 1 == 11
	 *
	 *  parser = (combined) %10 == 1
	 *  dialect = (combined) / 19 == 1
	 */

	parser_version = 2;

	/* prepare statement and return data */

	GDS_DSQL_PREPARE(status_vector,
					 &handle,
					 &statement->isr_handle,
					 length,
					 reinterpret_cast < char *>(string),
					 (USHORT) ((dialect * 10) + parser_version),
					 item_length,
					 reinterpret_cast < char *>(items),
					 buffer_length, reinterpret_cast < char *>(buffer));
	statement->isr_batch_flag = check_statement_type(statement);
	ips->ips_parameter = statement->isr_batch_flag;
	send_response(icc, status_vector);
}


static void put_segment( ICC icc)
{
/**************************************
 *
 *      p u t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *      Write a single blob segment.
 *
 **************************************/
	IBL blob;
	USHORT buffer_length;
	UCHAR *buffer;
	ISC_STATUS_ARRAY status_vector;
	ips_segment *ips;
	ips_string *ips_seg;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_segment;
	blob = (IBL) ips->ips_bl_handle;
	CHECK_HANDLE(blob, type_ibl, isc_bad_segstr_handle);

	/* set up segment buffer */

	ips_seg = &comm->ips_buffers[IPS_BLOB_SEGMENT];
	buffer_length = (USHORT) ips_seg->ips_cl_size;
	buffer = get_buffer(comm, buffer_length, IPS_BLOB_SEGMENT);
	NOT_NULL(buffer, buffer_length);
	IPS_SERVER(comm, IPS_BLOB_SEGMENT, buffer, buffer_length);
	if (!transfer_buffers(icc, comm))
		return;

	/* put the segment into the database */

	GDS_PUT_SEGMENT(status_vector,
					GDS_REF(blob->ibl_handle),
					buffer_length,
					reinterpret_cast < char *>(GDS_VAL(buffer)));
	send_response(icc, status_vector);
}


static void put_slice( ICC icc)
{
/**************************************
 *
 *      p u t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *      Put slice of array;
 *
 **************************************/
	IDB idb;
	ITR transaction;
	ISC_STATUS_ARRAY status_vector;
	USHORT sdl_length, param_length;
	ULONG slice_length, *params, parameters[32];
	UCHAR *sdl, sdl_buf[256], *slice;
	struct bid array_id;
	ips_slice *ips;
	ips_string *ips_sdl;
	ips_string *ips_parms;
	ips_string *ips_data;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_slice;
	idb = (IDB) ips->ips_db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	transaction = (ITR) ips->ips_tr_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
	array_id.bid_relation_id = ips->ips_rel_id;
	array_id.bid_number = ips->ips_number;

	/* set up sdl */

	ips_sdl = &comm->ips_buffers[IPS_SLICE_SDL];
	sdl_length = (USHORT) ips_sdl->ips_cl_size;
	if (sdl_length <= sizeof(sdl_buf))
		sdl = sdl_buf;
	else
		sdl = get_buffer(comm, sdl_length, IPS_SLICE_SDL);
	NOT_NULL(sdl, sdl_length);
	IPS_SERVER(comm, IPS_SLICE_SDL, sdl, sdl_length);

	/* set up parameters */

	ips_parms = &comm->ips_buffers[IPS_SLICE_PARAM];
	param_length = (USHORT) ips_parms->ips_cl_size;
	if (param_length <= sizeof(parameters))
		params = parameters;
	else
		params = (ULONG *) get_buffer(comm, param_length, IPS_SLICE_PARAM);
	NOT_NULL(params, param_length);
	IPS_SERVER(comm, IPS_SLICE_PARAM, (UCHAR *) params, param_length);

	/* set up slice data buffer */

	ips_data = &comm->ips_buffers[IPS_SLICE_BUFFER];
	slice_length = ips_data->ips_cl_size;
	if (slice_length)
	{
		slice = (UCHAR*)gds__alloc(slice_length);
		NOT_NULL(slice, TRUE);
		memset(slice, 0, slice_length);
	}
	else
		slice = NULL;
	IPS_SERVER(comm, IPS_SLICE_BUFFER, slice, slice_length);
	if (!transfer_buffers(icc, comm))
	{
		if (slice)
		{
			gds__free(slice);
		}
		return;
	}

	/* put the slice into the database */

	GDS_PUT_SLICE(status_vector,
				  GDS_REF(idb->idb_handle),
				  GDS_REF(transaction->itr_handle),
				  (GDS_QUAD *) & array_id,
				  sdl_length,
				  reinterpret_cast < char *>(sdl),
				  param_length,
				  reinterpret_cast < long *>(params),
				  slice_length, GDS_VAL(slice));
	ips->ips_rel_id = array_id.bid_relation_id;
	ips->ips_number = array_id.bid_number;
	send_response(icc, status_vector);
	if (slice)
	{
		gds__free(slice);
	}
}


static void que_events( ICC icc)
{
/**************************************
 *
 *      q u e _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *      Que events for asynchronous notification.
 *
 **************************************/
	IDB idb;
	USHORT length;
	UCHAR *events;
	ISC_STATUS_ARRAY status_vector;
	ISC_STATUS result;
	IVNT event;
	ips_que_events *ips;
	ips_string *ips_event;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_que_evnt;
	idb = (IDB) ips->ips_db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* set up event string */

	ips_event = &comm->ips_buffers[IPS_QUEUE_EVENT];
	length = (USHORT) ips_event->ips_cl_size;
	events = get_buffer(comm, length, IPS_QUEUE_EVENT);
	NOT_NULL(events, length);
	IPS_SERVER(comm, IPS_QUEUE_EVENT, events, length);

	/* find or allocate event block */

	for (event = idb->idb_events; event; event = event->ivnt_next)
		if (!event->ivnt_ast)
			break;
	if (!event)
	{
		event = (IVNT) ALLOC(type_ivnt);
		NOT_NULL(event, TRUE);
		event->ivnt_next = idb->idb_events;
		event->ivnt_idb = idb;
		idb->idb_events = event;
	}

	/* Get data, etc */

	event->ivnt_window = ips->ips_event_hwnd;
	event->ivnt_id = ips->ips_event_id;
	event->ivnt_ast = (void (*)()) ips->ips_ast;
	event->ivnt_arg = (void *) ips->ips_arg;
	if (!transfer_buffers(icc, comm))
		return;

	/* queue the event and return id */

	result = GDS_QUE_EVENTS(status_vector,
							GDS_REF(idb->idb_handle),
							GDS_REF(event->ivnt_handle),
							length,
							reinterpret_cast<char*>(GDS_VAL(events)),
							reinterpret_cast < void (*)() > (event_ast),
							GDS_VAL(event));
	if (!result)
		ips->ips_event_id = event->ivnt_handle;
	else
		event->ivnt_ast = NULL;
	send_response(icc, status_vector);
}


static void receive_msg( ICC icc)
{
/**************************************
 *
 *      r e c e i v e _ m s g
 *
 **************************************
 *
 * Functional description
 *      Receive a message.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	IRQ request;
	USHORT length, number, level;
#ifdef SCROLLABLE_CURSORS
	USHORT direction;
	ULONG offset;
#endif
	UCHAR *buffer;
	ips_request *ips;
	ips_string *ips_message;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_request;
	request = (IRQ) ips->ips_rq_handle;
	CHECK_HANDLE(request, type_irq, isc_bad_req_handle);

	/* set up message */

	ips_message = &comm->ips_buffers[IPS_RECEIVE_MESSAGE];
	length = (USHORT) ips_message->ips_cl_size;
	buffer = get_buffer(comm, length, IPS_RECEIVE_MESSAGE);
	NOT_NULL(buffer, length);
	IPS_SERVER(comm, IPS_RECEIVE_MESSAGE, buffer, length);
	number = ips->ips_msg_type;
	level = ips->ips_req_level;
#ifdef SCROLLABLE_CURSORS
	direction = ips->ips_direction;
	offset = ips->ips_offset;
#endif

	/* receive message and send response */

	GDS_RECEIVE(status_vector,
				GDS_REF(request->irq_handle),
				number, length, GDS_VAL(buffer), level
#ifdef SCROLLABLE_CURSORS
				, direction, offset
#endif
		);

	send_response(icc, status_vector);
}


static void reconnect( ICC icc)
{
/**************************************
 *
 *      r e c o n n e c t
 *
 **************************************
 *
 * Functional description
 *      Reconnect to a transaction in limbo.
 *
 **************************************/
	IDB idb;
	ITR transaction;
	USHORT length;
	UCHAR *buffer;
	FRBRD * handle;
	ISC_STATUS_ARRAY status_vector;
	ips_reconnect *ips;
	ips_string *ips_recon;
	ips_comm_area *comm;
	TEXT *comm_ptr;

/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_recon_trans;
	idb = (IDB) ips->ips_db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

/* set up string */

	ips_recon = &comm->ips_buffers[IPS_RECONNECT_ID];
	length = (USHORT) ips_recon->ips_cl_size;
	buffer = get_buffer(comm, length, IPS_RECONNECT_ID);
	NOT_NULL(buffer, length);
	IPS_SERVER(comm, IPS_RECONNECT_ID, buffer, length);
	handle = NULL;
	if (!transfer_buffers(icc, comm))
		return;

/* reconnect transaction and return id */

	if (!GDS_RECONNECT(status_vector,
					   GDS_REF(idb->idb_handle),
					   GDS_REF(handle),
					   length, reinterpret_cast < char *>(GDS_VAL(buffer))))
	{
		if (transaction = make_transaction(idb, handle))
		{
			ips->ips_tr_handle = (UCHAR *) transaction;
			transaction->itr_flags |= ITR_limbo;
		}
		else
		{
			gds__handle_cleanup(status_vector,
								(FRBRD **) GDS_REF(handle));
			NOT_NULL(transaction, TRUE);
		}
	}

	send_response(icc, status_vector);
}


static void release_blob( IBL blob)
{
/**************************************
 *
 *      r e l e a s e _ b l o b
 *
 **************************************
 *
 * Functional description
 *      Release a blob block and friends.
 *
 **************************************/
	IDB idb;
	ITR transaction;
	IBL *p;


	idb = blob->ibl_idb;
	transaction = blob->ibl_itr;

	for (p = &transaction->itr_blobs; *p; p = &(*p)->ibl_next)
	{
		if (*p == blob)
		{
			*p = blob->ibl_next;
			break;
		}
	}
	ALLI_release((BLK) blob);
}


static void release_event( IVNT event)
{
/**************************************
 *
 *      r e l e a s e _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *      Release a event block and friends.
 *
 **************************************/
	IDB idb;
	IVNT *p;


	idb = event->ivnt_idb;
	for (p = &idb->idb_events; *p; p = &(*p)->ivnt_next)
	{
		if (*p == event)
		{
			*p = event->ivnt_next;
			break;
		}
	}
	ALLI_release((BLK) event);
}


static void release_request( IRQ request)
{
/**************************************
 *
 *      r e l e a s e _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *      Release a request block and friends.
 *
 **************************************/
	IDB idb;
	IRQ *p;


	idb = request->irq_idb;
	for (p = &idb->idb_requests; *p; p = &(*p)->irq_next)
		if (*p == request)
		{
			*p = request->irq_next;
			break;
		}
	ALLI_release((BLK) request);
}


static void release_sql_request( IPSERVER_ISR stmt)
{
/**************************************
 *
 *      r e l e a s e _ s q l _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *      Release an SQL request block.
 *
 **************************************/
	IDB idb;
	IPSERVER_ISR *p;


	idb = stmt->isr_idb;
	for (p = &idb->idb_sql_requests; *p; p = &(*p)->isr_next)
		if (*p == stmt)
		{
			*p = stmt->isr_next;
			break;
		}
	ALLI_release((BLK) stmt);
}


static void release_transaction( ITR transaction)
{
/**************************************
 *
 *      r e l e a s e _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *      Release a transaction block and friends.
 *
 **************************************/
	IDB idb;
	ITR *p;


	idb = transaction->itr_idb;
	while (transaction->itr_blobs)
		release_blob(transaction->itr_blobs);
	for (p = &idb->idb_transactions; *p; p = &(*p)->itr_next)
		if (*p == transaction)
		{
			*p = transaction->itr_next;
			break;
		}
	ALLI_release((BLK) transaction);
}


static void seek_blob( ICC icc)
{
/**************************************
 *
 *      s e e k _ b l o b
 *
 **************************************
 *
 * Functional description
 *      Perform a blob seek.
 *
 **************************************/
	IBL blob;
	SSHORT mode;
	SLONG offset, result;
	ISC_STATUS_ARRAY status_vector;
	ips_seek_blob *ips;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_sk_blob;
	blob = (IBL) ips->ips_bl_handle;
	CHECK_HANDLE(blob, type_ibl, isc_bad_segstr_handle);
	offset = ips->ips_offset;
	mode = ips->ips_mode;

	/* seek blob and return result */

	GDS_SEEK_BLOB(status_vector,
				  GDS_REF(blob->ibl_handle), mode, offset, GDS_REF(result));
	ips->ips_result = result;
	send_response(icc, status_vector);
}


static BOOL send_and_wait( ICC icc)
{
/**************************************
 *
 *      s e n d _ a n d _ w a i t
 *
 **************************************
 *
 * Functional description
 *      This function responds to the client by releasing
 *      the client's semaphore, and then waits for more
 *      work by waiting on the server's semaphore.
 *
 **************************************/
	DWORD result;


	/* already in shutdown mode */

	if (icc->icc_flags & ICCF_SHUTDOWN)
		return FALSE;

	/* first, signal the client that the communications
	   area in mapped memory is filed and ready */

	if (!ReleaseSemaphore(icc->icc_client_sem, 1L, NULL))
	{
		icc->icc_flags |= ICCF_SHUTDOWN;
#ifdef  DEBUG_IP_TRACE
		gds__log("ipserver send of send and wait failed %lX %ld",
				 (long) icc, (long) GetLastError());
#endif
		return FALSE;
	}

	/* next, wait for the client to signal us back */

	result =
		WaitForMultipleObjects((DWORD) 2, icc->icc_waits, FALSE, INFINITE);
	if (result != WAIT_OBJECT_0)
	{
		icc->icc_flags |= ICCF_SHUTDOWN;
#ifdef  DEBUG_IP_TRACE
		gds__log("ipserver wait of send and wait failed %lX %ld",
				 (long) icc, (long) GetLastError());
#endif
		return FALSE;
	}
	else
		return TRUE;
}


static void send_msg( ICC icc)
{
/**************************************
 *
 *      s e n d _ m s g
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	IRQ request;
	UCHAR *buffer;
	USHORT length, number, level;
	ips_request *ips;
	ips_string *ips_message;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_request;
	request = (IRQ) ips->ips_rq_handle;
	CHECK_HANDLE(request, type_irq, isc_bad_req_handle);

	/* set up message */

	ips_message = &comm->ips_buffers[IPS_SEND_MESSAGE];
	length = (USHORT) ips_message->ips_cl_size;
	buffer = get_buffer(comm, length, IPS_SEND_MESSAGE);
	NOT_NULL(buffer, length);
	IPS_SERVER(comm, IPS_SEND_MESSAGE, buffer, length);
	number = ips->ips_msg_type;
	level = ips->ips_req_level;
	if (!transfer_buffers(icc, comm))
		return;

	/* send message */

	GDS_SEND(status_vector,
			 GDS_REF(request->irq_handle),
			 number, length, GDS_VAL(buffer), level);
	send_response(icc, status_vector);
}


static void send_no_wait( ICC icc)
{
/**************************************
 *
 *      s e n d _ n o _ w a i t
 *
 **************************************
 *
 * Functional description
 *      This function sends the client the server's
 *      last filled in communications area, indicating
 *      that the server is done.   No wait is done here.
 *
 **************************************/


	/* already in shutdown mode */

	if (icc->icc_flags & ICCF_SHUTDOWN)
		return;

	/* signal the client that the communications
	   area in mapped memory is filed and ready */

	if (!ReleaseSemaphore(icc->icc_client_sem, 1L, NULL))
	{
#ifdef  DEBUG_IP_TRACE
		gds__log("ipserver send no wait failed %lX %ld",
				 (long) icc, (long) GetLastError());
#endif
		icc->icc_flags |= ICCF_SHUTDOWN;
	}
}


static void send_response( ICC icc, ISC_STATUS * status_vector)
{
/**************************************
 *
 *      s e n d _ r e s p o n s e
 *
 **************************************
 *
 * Functional description
 *      Send a response packet.
 *
 **************************************/
	ISC_STATUS *comm_status;
	USHORT i, length;
	ULONG to_copy, size_left;
	TEXT *p, buffer[1024];
	TEXT *acursor, *scursor, *abase;
	ips_comm_area *comm;
	TEXT *comm_ptr;
	ips_string *string;
	ULONG l, *lp;


	/* first, initialize the status vector in the comm area */

	comm = (ips_comm_area *) icc->icc_mapped_addr;
	comm_ptr = (TEXT *) comm->ips_data;
	comm_status = reinterpret_cast < ISC_STATUS * >(comm->ips_status);
	abase = comm_ptr;

	/* if there's no error or warning, just put in a clean status vector */

	if (!status_vector[1] && !status_vector[2])
	{
		*comm_status++ = *status_vector++;
		*comm_status++ = *status_vector++;
	}
	else
	{
		/* transfer the status vector, putting strings in the comm area's
		   data area */

		while (*status_vector)
		{
			switch (*status_vector)
			{
			case isc_arg_interpreted:
			case isc_arg_string:

				/* for strings, put the length, followed by the
				   string in the comm data area */

				*comm_status++ = *status_vector++;
				p = (TEXT *) * status_vector++;
				if (p)
					length = strlen(p);
				else
					length = 0;
				lp = (ULONG *) comm_ptr;
				*lp = length;
				comm_ptr += sizeof(ULONG);
				for (i = 0; i < length; i++)
					*comm_ptr++ = *p++;
				l = length % sizeof(ULONG);
				if (l)
					comm_ptr += (sizeof(ULONG) - l);
				break;

			case isc_arg_cstring:
				*comm_status++ = isc_arg_string;
				status_vector++;
				length = (USHORT) * status_vector++;
				lp = (ULONG *) comm_ptr;
				*lp = length;
				comm_ptr += sizeof(ULONG);
				p = (TEXT *) * status_vector++;
				for (i = 0; i < length; i++)
					*comm_ptr++ = *p++;
				l = length % sizeof(ULONG);
				if (l)
					comm_ptr += (sizeof(ULONG) - l);
				break;

			case isc_arg_warning:
			case isc_arg_number:
			case isc_arg_gds:
				*comm_status++ = *status_vector++;
				*comm_status++ = *status_vector++;
				break;

			default:
				length = (USHORT) isc_interprete(buffer, &status_vector);
				if (!length)
				{
					*status_vector = 0;
					break;
				}
				*comm_status++ = isc_arg_interpreted;
				lp = (ULONG *) comm_ptr;
				*lp = length;
				comm_ptr += sizeof(ULONG);
				for (i = 0; i < length; i++)
					*comm_ptr++ = buffer[i];
				l = length % sizeof(ULONG);
				if (l)
					comm_ptr += (sizeof(ULONG) - l);
				break;
			}
		}
	}
	*comm_status++ = isc_arg_end;

	/* point to the current first available byte and figure space available */

	acursor = comm_ptr;
	size_left = (ULONG) (IPS_USEFUL_SPACE(ipserver_private_data.pages_per_user)) -
		(abase - reinterpret_cast < TEXT * >(comm->ips_data));
	for (i = 0; i < MAX_IPS_STRINGS; i++)
	{
		string = &comm->ips_buffers[i];

		/* see if this buffer needs sending */

		if ((string->ips_flags & IPS_OUTPUT_BUFFER) && string->ips_sv_size)
		{
			string->ips_sv_copied = 0;
			scursor = reinterpret_cast < char *>(string->ips_sv_addr);

			/* keep packing it in */

			while (string->ips_sv_copied < string->ips_sv_size)
			{

				/* put in the whole thing, or whatever will fit */

				to_copy = string->ips_sv_size - string->ips_sv_copied;
				if (size_left <= to_copy)
					to_copy = size_left;
				string->ips_com_offset = acursor - abase;
				string->ips_com_size = to_copy;
				memcpy(acursor, scursor, (USHORT) to_copy);
				acursor += to_copy;
				scursor += to_copy;
				size_left -= to_copy;
				string->ips_sv_copied += to_copy;

				/* if all the space has been used up, send it and wait */

				if (!size_left && string->ips_sv_copied < string->ips_sv_size)
				{
					comm->ips_sequence++;
					if (!send_and_wait(icc))
						return;
					size_left = (ULONG) (IPS_USEFUL_SPACE(ipserver_private_data.pages_per_user));
					acursor = reinterpret_cast < char *>(comm->ips_data);
				}
			}
		}
	}

	/* sending the last buffer, all done */

	comm->ips_sequence++;
	send_no_wait(icc);
}


static void service_attach( ICC icc)
{
/**************************************
 *
 *      s e r v i c e _ a t t a c h 
 *
 **************************************
 *
 * Functional description
 *      Connect to an Interbase service.
 *
 **************************************/
	USHORT service_length, spb_length;
	UCHAR service_name[256], *spb, spb_buf[256];
	FRBRD * handle;
	ISC_STATUS_ARRAY status_vector;
	ISC_STATUS result;
	IDB idb;
	ips_object *ips;
	ips_string *ips_name;
	ips_string *ips_spb;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_object;

	/* figure out service name buffer */

	ips_name = &comm->ips_buffers[IPS_ATTACH_SVC_NAME];
	service_length = (USHORT) ips_name->ips_cl_size;
	IPS_SERVER(comm, IPS_ATTACH_SVC_NAME, service_name, service_length);

	/* figure out spb buffer */

	ips_spb = &comm->ips_buffers[IPS_ATTACH_SVC_SPB];
	spb_length = (USHORT) ips_spb->ips_cl_size;
	if (spb_length <= sizeof(spb_buf))
		spb = spb_buf;
	else
		spb = get_buffer(comm, spb_length, IPS_ATTACH_SVC_SPB);
	NOT_NULL(spb, spb_length);
	IPS_SERVER(comm, IPS_ATTACH_SVC_SPB, spb, spb_length);
	if (!transfer_buffers(icc, comm))
		return;

	/* get handle */

	handle = NULL;
	result = GDS_SERVICE_ATTACH(status_vector,
								service_length,
								reinterpret_cast < char *>(service_name),
								&handle,
								spb_length, reinterpret_cast < char *>(spb));

	/* allocate structure and return handle */

	if (!result)
	{
		idb = (IDB) ALLOC(type_idb);
		ips->ips_handle = (UCHAR *) idb;
		NOT_NULL(idb, TRUE);
		idb->idb_flags = IDBF_SERVICE_ATTACHMENT;
		idb->idb_handle = handle;
		idb->idb_next = icc->icc_databases;
		icc->icc_databases = idb;
	}
	else
		ips->ips_handle = NULL;
	send_response(icc, status_vector);
}


static void service_end( ICC icc)
{
/**************************************
 *
 *      s e r v i c e _ e n d
 *
 **************************************
 *
 * Functional description
 *      Close down a service.
 *
 **************************************/
	IDB idb, *ptr;
	ISC_STATUS_ARRAY status_vector;
	ISC_STATUS result;
	ips_object *ips;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_object;
	idb = (IDB) ips->ips_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* detach request, return response and free resources */

	result = GDS_SERVICE_DETACH(status_vector, &idb->idb_handle);
	send_response(icc, status_vector);
	if (!result)
	{
		for (ptr = &icc->icc_databases; *ptr; ptr = &(*ptr)->idb_next)
			if (*ptr == idb)
			{
				*ptr = idb->idb_next;
				break;
			}
		ALLI_release((BLK) idb);
	}
}


static void service_start( ICC icc)
{
/**************************************
 *
 *      s e r v i c e _ s t a r t
 *
 **************************************
 *
 * Functional description
 *      Start a service.
 *
 **************************************/
	IDB idb;
	ISC_STATUS_ARRAY status_vector;
	ISC_STATUS result;
	ips_object *ips;
	ips_comm_area *comm;
	ips_string *ips_spb;
	TEXT *comm_ptr;
	USHORT spb_length;
	UCHAR *spb, spb_buf[256];
	HANDLE handle;
	ULONG *reserved = 0;

/* set up communications area */
	GET_COMM_OBJECT;
	comm->ips_operation = op_service_start;
	ips = &comm->ips_operations.ips_op_object;
	handle = (HANDLE) ips->ips_handle;

/* figure out spb buffer */
	ips_spb = &comm->ips_buffers[IPS_START_SVC_SPB];
	spb_length = (USHORT) ips_spb->ips_cl_size;
	if (spb_length <= sizeof(spb_buf))
		spb = spb_buf;
	else
		spb = get_buffer(comm, spb_length, IPS_START_SVC_SPB);
	NOT_NULL(spb, spb_length);

	IPS_SERVER(comm, IPS_START_SVC_SPB, spb, spb_length);

	if (!transfer_buffers(icc, comm))
		return;

	idb = (IDB) handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_svc_handle);
	result = GDS_SERVICE_START(status_vector,
							   &idb->idb_handle,
							   reinterpret_cast < long *>(reserved),
							   spb_length, reinterpret_cast < char *>(spb));

	send_response(icc, status_vector);
}


static void set_cursor( ICC icc)
{
/**************************************
 *
 *      s e t _ c u r s o r
 *
 **************************************
 *
 * Functional description
 *      Set a cursor name for a dynamic request.
 *
 **************************************/
	IPSERVER_ISR statement;
	USHORT length, type;
	UCHAR *cursor;
	ISC_STATUS_ARRAY status_vector;
	ips_dsql *ips;
	ips_string *ips_name;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_dsql;
	statement = (IPSERVER_ISR) ips->ips_st_handle;
	CHECK_HANDLE(statement, type_ipserver_isr, isc_bad_stmt_handle);
	type = (USHORT) ips->ips_parameter;

	/* set up cursor name */

	ips_name = &comm->ips_buffers[IPS_DSQL_SET_CURSOR];
	length = (USHORT) ips_name->ips_cl_size;
	cursor = get_buffer(comm, length, IPS_DSQL_SET_CURSOR);
	NOT_NULL(cursor, length);
	IPS_SERVER(comm, IPS_DSQL_SET_CURSOR, cursor, length);
	if (!transfer_buffers(icc, comm))
		return;

	/* set cursor name */

	GDS_DSQL_SET_CURSOR(status_vector,
						&statement->isr_handle,
						reinterpret_cast < char *>(cursor), type);
	send_response(icc, status_vector);
}


static void shutdown_attachments( ICC icc)
{
/**************************************
 *
 *      s h u t d o w n _ a t t a c h m e n t s
 *
 **************************************
 *
 * Functional description
 *      About to exit, detach any remaining database
 *      and service attachments.
 *
 **************************************/
	IDB idb, nextidb;
	ISC_STATUS_ARRAY status_vector;
	ips_comm_area *comm;
	USHORT i;

	for (idb = icc->icc_databases; idb; idb = nextidb)
	{
		/* save next pointer here because the structure's getting freed */

		nextidb = idb->idb_next;
		if (idb->idb_flags | IDBF_DATABASE_ATTACHMENT)
		{
			while (idb->idb_transactions)
			{
				if (!(idb->idb_transactions->itr_flags & ITR_limbo))
					GDS_ROLLBACK(status_vector,
								 GDS_REF(idb->idb_transactions->itr_handle));
				else
					gds__handle_cleanup(status_vector,
										(FRBRD **) GDS_REF(idb->
																 idb_transactions->
																 itr_handle));

				release_transaction(idb->idb_transactions);
			}

			GDS_DETACH(status_vector, GDS_REF(idb->idb_handle));
			while (idb->idb_events)
				release_event(idb->idb_events);

			while (idb->idb_requests)
				release_request(idb->idb_requests);

			while (idb->idb_sql_requests)
				release_sql_request(idb->idb_sql_requests);
		}
		else
		{
			GDS_SERVICE_DETACH(status_vector, &idb->idb_handle);
		}

		ALLI_release((BLK) idb);
	}

/* free any allocated server buffers */

	comm = (ips_comm_area *) icc->icc_mapped_addr;
	for (i = 0; i < MAX_IPS_STRINGS; i++)
		if (comm->ips_buffers[i].ips_sv_buffer)
			gds__free(comm->ips_buffers[i].ips_sv_buffer);
}


static void start( ICC icc)
{
/**************************************
 *
 *      s t a r t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	IRQ request;
	ITR transaction;
	USHORT level;
	ips_request *ips;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_request;
	request = (IRQ) ips->ips_rq_handle;
	CHECK_HANDLE(request, type_irq, isc_bad_req_handle);
	transaction = (ITR) ips->ips_tr_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
	level = ips->ips_req_level;

	/* start the request */

	GDS_START(status_vector,
			  GDS_REF(request->irq_handle),
			  GDS_REF(transaction->itr_handle), level);
	send_response(icc, status_vector);
}


static void start_and_send( ICC icc)
{
/**************************************
 *
 *      s t a r t _ a n d _ s e n d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	IRQ request;
	ITR transaction;
	UCHAR *buffer;
	USHORT length, number, level;
	ips_request *ips;
	ips_string *ips_message;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_request;
	request = (IRQ) ips->ips_rq_handle;
	CHECK_HANDLE(request, type_irq, isc_bad_req_handle);
	transaction = (ITR) ips->ips_tr_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
	number = ips->ips_msg_type;
	level = ips->ips_req_level;

	/* set up message */

	ips_message = &comm->ips_buffers[IPS_SEND_MESSAGE];
	length = (USHORT) ips_message->ips_cl_size;
	buffer = get_buffer(comm, length, IPS_SEND_MESSAGE);
	NOT_NULL(buffer, length);
	IPS_SERVER(comm, IPS_SEND_MESSAGE, buffer, length);
	number = ips->ips_msg_type;
	level = ips->ips_req_level;
	if (!transfer_buffers(icc, comm))
		return;

	/* start the request */

	GDS_START_AND_SEND(status_vector,
					   GDS_REF(request->irq_handle),
					   GDS_REF(transaction->itr_handle),
					   number, length, GDS_VAL(buffer), level);
	send_response(icc, status_vector);
}


static void start_transaction( ICC icc)
{
/**************************************
 *
 *      s t a r t _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *      Start a transaction.
 *
 **************************************/
	IDB idb;
	ITR transaction;
	USHORT count, c;
	UCHAR *buffer;
	FRBRD * handle;
	ULONG **v, *vector[3 * 16];
	ISC_STATUS_ARRAY status_vector;
	ips_start_trans *ips;
	ips_comm_area *comm;
	TEXT *comm_ptr;
	ULONG length, l, *lp;


	/* set up communications area */

	GET_COMM_OBJECT;
	lp = (ULONG *) comm_ptr;
	ips = &comm->ips_operations.ips_op_strt_trans;
	v = vector;
	count = ips->ips_db_count;
	buffer = get_buffer(comm, 1024, 0);
	NOT_NULL(buffer, TRUE);

	/* set up all the handles */

	for (c = 0; c < count; c++)
	{
		idb = (IDB) * lp++;
		CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
		*v++ = reinterpret_cast < ULONG * >(&idb->idb_handle);
		length = *lp++;
		*v++ = reinterpret_cast < ULONG * >(length);
		*v++ = reinterpret_cast < ULONG * >(buffer);
		comm_ptr = (TEXT *) lp;
		for (l = 0; l < length; l++)
			*buffer++ = *comm_ptr++;
		lp = lp + ((length + (sizeof(ULONG) - 1)) / sizeof(ULONG));
	}
	handle = NULL;

	/* start the transaction and return the handle */

	GDS_START_MULTIPLE(status_vector,
					   GDS_REF(handle), count, (UCHAR *) vector);
	transaction = make_transaction(idb, handle);
	ips->ips_tr_handle = (UCHAR *) transaction;
	NOT_NULL(transaction, TRUE);
	send_response(icc, status_vector);
}


static void transact_request( ICC icc)
{
/**************************************
 *
 *      t r a n s a c t _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	IDB idb;
	ITR transaction;
	USHORT blr_length, in_msg_length, out_msg_length;
	UCHAR *blr, *in_buffer, *out_buffer;
	ISC_STATUS_ARRAY status_vector;
	ips_transact_request *ips;
	ips_string *ips_blr;
	ips_string *ips_in_msg;
	ips_string *ips_out_msg;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_trans_req;
	idb = (IDB) ips->ips_db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	transaction = (ITR) ips->ips_tr_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);

	/* set up blr */

	ips_blr = &comm->ips_buffers[IPS_TRANS_REQ_BLR];
	blr_length = (USHORT) ips_blr->ips_cl_size;
	blr = get_buffer(comm, blr_length, IPS_TRANS_REQ_BLR);
	NOT_NULL(blr, blr_length);
	IPS_SERVER(comm, IPS_TRANS_REQ_BLR, blr, blr_length);

	/* set up input message */

	ips_in_msg = &comm->ips_buffers[IPS_TRANS_REQ_IN_MSG];
	in_msg_length = (USHORT) ips_in_msg->ips_cl_size;
	in_buffer = get_buffer(comm, in_msg_length, IPS_TRANS_REQ_IN_MSG);
	NOT_NULL(in_buffer, in_msg_length);
	IPS_SERVER(comm, IPS_TRANS_REQ_IN_MSG, in_buffer, in_msg_length);

	/* set up blr */

	ips_out_msg = &comm->ips_buffers[IPS_TRANS_REQ_OUT_MSG];
	out_msg_length = (USHORT) ips_out_msg->ips_cl_size;
	out_buffer = get_buffer(comm, out_msg_length, IPS_TRANS_REQ_OUT_MSG);
	NOT_NULL(out_buffer, out_msg_length);
	IPS_SERVER(comm, IPS_TRANS_REQ_OUT_MSG, out_buffer, out_msg_length);
	if (!transfer_buffers(icc, comm))
		return;

	/* transact request and return output */

	GDS_TRANSACT_REQUEST(status_vector,
						 &idb->idb_handle,
						 &transaction->itr_handle,
						 blr_length,
						 reinterpret_cast < char *>(blr),
						 in_msg_length,
						 reinterpret_cast < char *>(in_buffer),
						 out_msg_length,
						 reinterpret_cast < char *>(out_buffer));
	send_response(icc, status_vector);
}


static BOOL transfer_buffers( ICC icc, ips_comm_area * comm)
{
/**************************************
 *
 *      t r a n s f e r _ b u f f e r s
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	TEXT *acursor, *scursor;
	ULONG to_copy;
	USHORT i;
	ips_string *string;


	/* for each buffer, see if the client is sending stuff, and if so,
	   put the data in the server's buffers */

	for (i = 0; i < MAX_IPS_STRINGS; i++)
	{
		string = &comm->ips_buffers[i];

		/* see if this buffer needs extraction */

		if ((string->ips_flags & IPS_INPUT_BUFFER) && string->ips_sv_size)
		{
			string->ips_sv_copied = 0;
			scursor = reinterpret_cast < char *>(string->ips_sv_addr);

			/* start extraction */

			while (string->ips_sv_copied < string->ips_sv_size)
			{

				/* extract all or whatever's available */

				to_copy = string->ips_sv_size - string->ips_sv_copied;
				if (string->ips_com_size <= to_copy)
					to_copy = string->ips_com_size;
				acursor = (TEXT *) comm->ips_data + string->ips_com_offset;
				memcpy(scursor, acursor, (USHORT) to_copy);
				scursor += to_copy;
				string->ips_sv_copied += to_copy;

				/* if there is more to copy, wait for it */

				if (string->ips_sv_copied < string->ips_sv_size)
				{
					comm->ips_sequence++;
					if (!send_and_wait(icc))
						return FALSE;
				}
			}
		}
	}
	return TRUE;
}


static void unwind( ICC icc)
{
/**************************************
 *
 *      u n w i n d
 *
 **************************************
 *
 * Functional description
 *      Unwind a request.
 *
 **************************************/
	IRQ request;
	USHORT level;
	ISC_STATUS_ARRAY status_vector;
	ips_object *ips;
	ips_comm_area *comm;
	TEXT *comm_ptr;


	/* set up communications area */

	GET_COMM_OBJECT;
	ips = &comm->ips_operations.ips_op_object;
	request = (IRQ) ips->ips_handle;
	CHECK_HANDLE(request, type_irq, isc_bad_req_handle);
	level = (USHORT) ips->ips_parameter;

	/* unwind the request */

	GDS_UNWIND(status_vector, GDS_REF(request->irq_handle), level);
	send_response(icc, status_vector);
}


static BOOL wait_no_send( ICC icc)
{
/**************************************
 *
 *      w a i t _ n o _ s e n d
 *
 **************************************
 *
 * Functional description
 *      This function waits for the client's next
 *      request by waiting on the server's semaphore.
 *
 **************************************/
	DWORD result1;
	DWORD result2;
	DWORD error1;
	DWORD error2;
	USHORT ret;


	/* already in shutdown mode */

	if (icc->icc_flags & ICCF_SHUTDOWN)
		return FALSE;

	/* wait for the server to signal us back */

	ret = TRUE;
	while (ret)
	{

		/* wait for semaphore for a minute */

		result1 = WaitForSingleObject(icc->icc_waits[0], 10000L);
		error1 = GetLastError();

		/* check for success */

		if (result1 == WAIT_OBJECT_0)
			break;

		/* a non-timeout result is an error */

		if (result1 != WAIT_TIMEOUT)
		{
#ifdef  DEBUG_IP_TRACE
			gds__log("ipserver wait no send failed %lX %ld %ld",
					 (long) icc, result1, error1);
			gds__log("ipserver ids %lX %lX",
					 icc->icc_waits[0], icc->icc_waits[1]);
#endif
			ret = 0;
			break;
		}

		/* see if the client is still alive */

		result2 = WaitForSingleObject(icc->icc_waits[1], 1L);
		error2 = GetLastError();

		/* if process is gone, or there's some error, bag it */

		if (result2 != WAIT_TIMEOUT)
		{
#ifdef  DEBUG_IP_TRACE
			gds__log("ip server wait no send failed again %lX %ld %ld",
					 (long) icc, result2, error2);
#endif
			ret = 0;
			break;
		}
	}
	if (!ret)
		icc->icc_flags |= ICCF_SHUTDOWN;
	return ret;
}
