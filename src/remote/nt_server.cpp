/*
 *	PROGRAM:	JRD Remote Server
 *	MODULE:		nt_server.c
 *	DESCRIPTION:	Windows NT remote server.
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
#include <stdlib.h>
#include "../remote/remote.h"
#include "gen/codes.h"
#include "../jrd/license.h"
#include "../jrd/thd.h"
#include "../jrd/thd_proto.h"
#include "../utilities/install_nt.h"
#include "../remote/cntl_proto.h"
#include "../remote/inet_proto.h"
#include "../remote/serve_proto.h"
#include "../remote/wnet_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thd_proto.h"

#include <windows.h>
#define TEXT		SCHAR

static void THREAD_ROUTINE process_connection_thread(PORT);
static void THREAD_ROUTINE inet_connect_wait_thread(void *);
static void service_connection(PORT);
static void THREAD_ROUTINE start_connections_thread(int);
static void THREAD_ROUTINE wnet_connect_wait_thread(void *);

static TEXT protocol_inet[128];
static TEXT protocol_wnet[128];
static USHORT server_flag;
static SERVICE_TABLE_ENTRY service_table[] = {
	REMOTE_SERVICE, (LPSERVICE_MAIN_FUNCTION) CNTL_main_thread,
	NULL, NULL
};


int CLIB_ROUTINE main( int argc, char *argv[])
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Run the server with NT named
 *	pipes and/or TCP/IP sockets.
 *
 **************************************/
	STATUS status_vector[ISC_STATUS_LENGTH];
	PORT port;
	BOOLEAN debug;
	TEXT *p, c;
	HANDLE connection_handle;

	argv++;
	debug = FALSE;
#ifdef SUPERSERVER
	server_flag = SRVR_multi_client;
#else
	server_flag = 0;
#endif
	protocol_inet[0] = 0;
	protocol_wnet[0] = 0;
	connection_handle = INVALID_HANDLE_VALUE;

	while (--argc) {
		p = *argv++;
		if (*p++ = '-')
			while (c = *p++)
				switch (UPPER(c)) {
				case 'B':
					server_flag |= SRVR_high_priority;
					break;

				case 'D':
					debug = TRUE;
					server_flag |= SRVR_debug | SRVR_non_service;
					break;

				case 'H':
					if (--argc)
						connection_handle = (HANDLE) atol(*argv++);
					break;

				case 'I':
					server_flag |= SRVR_inet;
					break;

				case 'N':
					server_flag |= SRVR_pipe;
					break;

				case 'P':
					if (--argc) {
						sprintf(protocol_inet, "/%s", *argv);
						sprintf(protocol_wnet, "\\\\.@%s", *argv);
						argv++;
					}
					break;

				case 'R':
					server_flag &= ~SRVR_high_priority;
					break;

				case 'Z':
					ib_printf("Firebird remote server version %s\n",
							  GDS_VERSION);
					exit(FINI_OK);
				}
	}

	if (connection_handle != INVALID_HANDLE_VALUE) {
		THREAD_ENTER;
		if (server_flag & SRVR_inet)
			port = INET_reconnect(connection_handle, 0, status_vector);
		else
			port = WNET_reconnect(connection_handle, 0, status_vector);
		THREAD_EXIT;
		service_connection(port);
	}
	else if (!debug) {
		CNTL_init((FPTR_VOID) start_connections_thread, REMOTE_SERVICE);
		if (!StartServiceCtrlDispatcher(service_table))
			if (GetLastError() != ERROR_CALL_NOT_IMPLEMENTED)
				CNTL_shutdown_service("StartServiceCtrlDispatcher failed");
			else {
				start_connections_thread(server_flag);
				Sleep(INFINITE);
			}
	}
	else {
		if ((server_flag & SRVR_multi_client) ||
			!(server_flag & (SRVR_pipe | SRVR_inet)) ||
			(server_flag & (SRVR_pipe | SRVR_inet)) ==
			(SRVR_pipe | SRVR_inet)) {
			start_connections_thread(server_flag);
			Sleep(INFINITE);
		}
		else {
			THREAD_ENTER;
			if (server_flag & SRVR_inet)
				port = INET_connect(protocol_inet, 0, status_vector,
									server_flag, 0, 0);
			else
				port =
					WNET_connect(protocol_wnet, 0, status_vector,
								 server_flag);
			THREAD_EXIT;
			if (!port) {
				gds__print_status(status_vector);
				exit(FINI_ERROR);
			}
			service_connection(port);
		}
	}

	exit(FINI_OK);
}


static void THREAD_ROUTINE process_connection_thread( PORT port)
{
/**************************************
 *
 *	p r o c e s s _ c o n n e c t i o n _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	void *thread;

	if (!(server_flag & SRVR_non_service))
		thread = CNTL_insert_thread();
	service_connection(port);
	if (!(server_flag & SRVR_non_service))
		CNTL_remove_thread(thread);
}


static void THREAD_ROUTINE inet_connect_wait_thread( void *dummy)
{
/**************************************
 *
 *	i n e t _ c o n n e c t _ w a i t _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	void *thread;
	STATUS status_vector[ISC_STATUS_LENGTH];
	PORT port;

	if (!(server_flag & SRVR_non_service))
		thread = CNTL_insert_thread();

	THREAD_ENTER;
	port = INET_connect(protocol_inet, 0, status_vector, server_flag, 0, 0);
	THREAD_EXIT;
	if (port)
		SRVR_multi_thread(port, server_flag);
	else
		gds__log_status(0, status_vector);

	if (!(server_flag & SRVR_non_service))
		CNTL_remove_thread(thread);
}


static void service_connection( PORT port)
{
/**************************************
 *
 *	s e r v i c e _ c o n n e c t i o n
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	SRVR_main(port, server_flag & ~SRVR_multi_client);
}


static void THREAD_ROUTINE start_connections_thread( int flag)
{
/**************************************
 *
 *	s t a r t _ c o n n e c t i o n s _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	server_flag = flag;
	if (!(server_flag & (SRVR_pipe | SRVR_inet)))
		server_flag |= SRVR_pipe | SRVR_inet;
	if ((server_flag & SRVR_multi_client) ||
		(server_flag & (SRVR_pipe | SRVR_inet)) == (SRVR_pipe | SRVR_inet))
		gds__thread_enable(-1);

	if (server_flag & SRVR_inet)
		gds__thread_start(reinterpret_cast<FPTR_INT_VOID_PTR>(inet_connect_wait_thread),
			0, THREAD_high, 0, 0);
	if (server_flag & SRVR_pipe)
		gds__thread_start(reinterpret_cast<FPTR_INT_VOID_PTR>(wnet_connect_wait_thread),
			0, THREAD_high, 0, 0);
}


static void THREAD_ROUTINE wnet_connect_wait_thread( void *dummy)
{
/**************************************
 *
 *	w n e t _ c o n n e c t _ w a i t _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	void *thread;
	STATUS status_vector[ISC_STATUS_LENGTH];
	PORT port;

	if (!(server_flag & SRVR_non_service))
		thread = CNTL_insert_thread();

	while (TRUE) {
		THREAD_ENTER;
		port = WNET_connect(protocol_wnet, 0, status_vector, server_flag);
		THREAD_EXIT;
		if (!port) {
			if (status_vector[1] != gds_io_error ||
				status_vector[6] != gds_arg_win32 ||
				status_vector[7] != ERROR_CALL_NOT_IMPLEMENTED)
					gds__log_status(0, status_vector);
			break;
		}
		gds__thread_start(reinterpret_cast<FPTR_INT_VOID_PTR>(process_connection_thread),
			port, 0, 0, 0);
	}

	if (!(server_flag & SRVR_non_service))
		CNTL_remove_thread(thread);
}
