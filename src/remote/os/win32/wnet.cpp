/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		wnet.cpp
 *	DESCRIPTION:	Windows Net Communications module.
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

#ifdef DEBUG
/* define WNET_trace to 0 (zero) for no packet debugging */
#define WNET_trace
#endif

#include "firebird.h"
#include <stdio.h>
#include <string.h>
#include "../remote/remote.h"
#include "../jrd/ibase.h"
#include "../jrd/thd.h"
#include "../jrd/iberr.h"

#include "../utilities/install/install_nt.h"

#include "../remote/proto_proto.h"
#include "../remote/remot_proto.h"
#include "../remote/os/win32/wnet_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_f_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thread_proto.h"
#include "../common/config/config.h"

#include <stdarg.h>

const int MAX_DATA		= 2048;
const int BUFFER_SIZE	= MAX_DATA;
const int MAX_SEQUENCE	= 256;

const char* PIPE_PREFIX			= "pipe"; // win32-specific
const char* SERVER_PIPE_SUFFIX	= "server";
const char* EVENT_PIPE_SUFFIX	= "event";

int xdrmem_create();

static int		accept_connection(rem_port*, P_CNCT *);
static rem_port*		alloc_port(rem_port*);
static rem_port*		aux_connect(rem_port*, PACKET*, t_event_ast);
static rem_port*		aux_request(rem_port*, PACKET*);
static void		cleanup_port(rem_port*);
static void		disconnect(rem_port*);
static void		exit_handler(rem_port*);
static rem_str*		make_pipe_name(const TEXT*, const TEXT*, const TEXT*);
static rem_port*		receive(rem_port*, PACKET *);
static int		send_full(rem_port*, PACKET *);
static int		send_partial(rem_port*, PACKET *);
static int		xdrwnet_create(XDR *, rem_port*, UCHAR *, USHORT, enum xdr_op);
static bool_t	xdrwnet_endofrecord(XDR *, int);
static int		wnet_destroy(XDR *);
static int		wnet_error(rem_port*, const TEXT*, ISC_STATUS, int);
static void		wnet_gen_error(rem_port*, ISC_STATUS, ...);
static bool_t	wnet_getbytes(XDR *, SCHAR *, u_int);
static bool_t	wnet_getlong(XDR *, SLONG *);
static u_int	wnet_getpostn(XDR *);
static caddr_t	wnet_inline(XDR *, u_int);
static bool_t	wnet_putlong(XDR *, const SLONG*);
static bool_t	wnet_putbytes(XDR*, const SCHAR*, u_int);
static bool_t	wnet_read(XDR *);
static bool_t	wnet_setpostn(XDR *, u_int);
static bool_t	wnet_write(XDR *, int);
#ifdef DEBUG
static void		packet_print(const TEXT*, const UCHAR*, const int);
#endif
static int		packet_receive(rem_port*, UCHAR *, SSHORT, SSHORT *);
static int		packet_send(rem_port*, const SCHAR*, SSHORT);
static void		wnet_copy(const SCHAR*, SCHAR*, int);
static void		wnet_make_file_name(TEXT *, DWORD);

static xdr_t::xdr_ops wnet_ops =
{
	wnet_getlong,
	wnet_putlong,
	wnet_getbytes,
	wnet_putbytes,
	wnet_getpostn,
	wnet_setpostn,
	wnet_inline,
	wnet_destroy
};

const USHORT MAX_PTYPE	= ptype_out_of_band;


rem_port* WNET_analyze(Firebird::PathName& file_name,
					ISC_STATUS*	status_vector,
					const TEXT*	node_name,
					const TEXT*	user_string,
					bool	uv_flag)
{
/**************************************
 *
 *	W N E T _ a n a l y z e
 *
 **************************************
 *
 * Functional description
 *	Determine whether the file name has a "\\nodename".
 *	If so, establish an external connection to the node.
 *
 *	If a connection is established, return a port block, otherwise
 *	return NULL.
 *
 **************************************/

/* We need to establish a connection to a remote server.  Allocate the necessary
   blocks and get ready to go. */

	RDB rdb = (RDB) ALLR_block(type_rdb, 0);
	PACKET* packet = &rdb->rdb_packet;

/* Pick up some user identification information */
	UCHAR user_id[200];
	user_id[0] = CNCT_user;
	UCHAR* p = user_id + 2;
	ISC_get_user(reinterpret_cast<SCHAR*>(p), 0, 0, 0, 0, 0, user_string);
	user_id[1] = (UCHAR) strlen((SCHAR *) p);

	for (; *p; p++)
		if (*p >= 'A' && *p <= 'Z')
			*p = *p - 'A' + 'a';

	*p++ = CNCT_host;
	p++;
	ISC_get_host(reinterpret_cast<SCHAR*>(p), 64);
	p[-1] = (UCHAR) strlen((const char*) p);

	for (; *p; p++)
		if (*p >= 'A' && *p <= 'Z')
			*p = *p - 'A' + 'a';

	if (uv_flag) {
		*p++ = CNCT_user_verification;
		*p++ = 0;
	}

	const SSHORT user_length = p - user_id;

/* Establish connection to server */

	P_CNCT* cnct = &packet->p_cnct;
	packet->p_operation = op_connect;
	cnct->p_cnct_operation = op_attach;
	cnct->p_cnct_cversion = CONNECT_VERSION2;
	cnct->p_cnct_client = ARCHITECTURE;
	cnct->p_cnct_file.cstr_length = file_name.length();
	cnct->p_cnct_file.cstr_address = 
			reinterpret_cast<UCHAR*>(file_name.begin());

/* Note: prior to V3.1E a receivers could not in truth handle more
   then 5 protocol descriptions; however, this restriction does not 
   apply to Windows since it was created in 4.0 */

/* If we want user verification, we can't speak anything less than version 7 */

	cnct->p_cnct_user_id.cstr_length = user_length;
	cnct->p_cnct_user_id.cstr_address = user_id;

	static const p_cnct::p_cnct_repeat protocols_to_try1[] =
	{
		REMOTE_PROTOCOL(PROTOCOL_VERSION7, ptype_rpc, MAX_PTYPE, 1),
		REMOTE_PROTOCOL(PROTOCOL_VERSION8, ptype_rpc, MAX_PTYPE, 2),
		REMOTE_PROTOCOL(PROTOCOL_VERSION10, ptype_rpc, MAX_PTYPE, 3)
#ifdef SCROLLABLE_CURSORS
		,
		REMOTE_PROTOCOL(PROTOCOL_SCROLLABLE_CURSORS, ptype_rpc, MAX_PTYPE, 4)
#endif
	};
	cnct->p_cnct_count = FB_NELEM(protocols_to_try1);

	for (size_t i = 0; i < cnct->p_cnct_count; i++) {
		cnct->p_cnct_versions[i] = protocols_to_try1[i];
	}

/* If we can't talk to a server, punt. Let somebody else generate an error. */

	rem_port* port = WNET_connect(node_name, packet, status_vector, 0);
	if (!port) {
		ALLR_release(rdb);
		return NULL;
	}

/* Get response packet from server. */

	rdb->rdb_port = port;
	port->port_context = rdb;
	port->receive(packet);

	if (packet->p_operation == op_reject && !uv_flag) {
		disconnect(port);
		packet->p_operation = op_connect;
		cnct->p_cnct_operation = op_attach;
		cnct->p_cnct_cversion = CONNECT_VERSION2;
		cnct->p_cnct_client = ARCHITECTURE;
		cnct->p_cnct_file.cstr_length = file_name.length();
		cnct->p_cnct_file.cstr_address = (UCHAR *) file_name.c_str();

		/* try again with next set of known protocols */

		cnct->p_cnct_user_id.cstr_length = user_length;
		cnct->p_cnct_user_id.cstr_address = user_id;

		static const p_cnct::p_cnct_repeat protocols_to_try2[] =
		{
			REMOTE_PROTOCOL(PROTOCOL_VERSION4, ptype_rpc, ptype_batch_send, 1),
			REMOTE_PROTOCOL(PROTOCOL_VERSION6, ptype_rpc, ptype_batch_send, 2),
		};
		cnct->p_cnct_count = FB_NELEM(protocols_to_try2);

		for (size_t i = 0; i < cnct->p_cnct_count; i++) {
			cnct->p_cnct_versions[i] = protocols_to_try2[i];
		}

		port = WNET_connect(node_name, packet, status_vector, 0);
		if (!port) {
			ALLR_release(rdb);
			return NULL;
		}

		/* Get response packet from server. */

		rdb->rdb_port = port;
		port->port_context = rdb;
		port->receive(packet);
	}

	if (packet->p_operation == op_reject && !uv_flag) {
		disconnect(port);
		packet->p_operation = op_connect;
		cnct->p_cnct_operation = op_attach;
		cnct->p_cnct_cversion = CONNECT_VERSION2;
		cnct->p_cnct_client = ARCHITECTURE;
		cnct->p_cnct_file.cstr_length = file_name.length();
		cnct->p_cnct_file.cstr_address = (UCHAR *) file_name.c_str();

		/* try again with next set of known protocols */

		cnct->p_cnct_user_id.cstr_length = user_length;
		cnct->p_cnct_user_id.cstr_address = user_id;

		static const p_cnct::p_cnct_repeat protocols_to_try3[] =
		{
			REMOTE_PROTOCOL(PROTOCOL_VERSION3, ptype_rpc, ptype_batch_send, 1)
		};
		cnct->p_cnct_count = FB_NELEM(protocols_to_try3);

		for (size_t i = 0; i < cnct->p_cnct_count; i++) {
			cnct->p_cnct_versions[i] = protocols_to_try3[i];
		}

		port = WNET_connect(node_name, packet, status_vector, 0);
		if (!port) {
			ALLR_release(rdb);
			return NULL;
		}

		/* Get response packet from server. */

		rdb->rdb_port = port;
		port->port_context = rdb;
		port->receive(packet);
	}

	if (packet->p_operation != op_accept) {
		*status_vector++ = isc_arg_gds;
		*status_vector++ = isc_connect_reject;
		*status_vector++ = 0;
		disconnect(port);
		return NULL;
	}

	port->port_protocol = packet->p_acpt.p_acpt_version;

/* once we've decided on a protocol, concatenate the version 
   string to reflect it...  */

	TEXT buffer[BUFFER_TINY];
	sprintf(buffer, "%s/P%d", port->port_version->str_data,
			port->port_protocol);
	ALLR_free(port->port_version);
	port->port_version = REMOTE_make_string(buffer);

	if (packet->p_acpt.p_acpt_architecture == ARCHITECTURE)
		port->port_flags |= PORT_symmetric;

	if (packet->p_acpt.p_acpt_type == ptype_rpc)
		port->port_flags |= PORT_rpc;

	if (packet->p_acpt.p_acpt_type != ptype_out_of_band)
		port->port_flags |= PORT_no_oob;

	return port;
}


rem_port* WNET_connect(const TEXT*		name,
				  PACKET*	packet,
				  ISC_STATUS*	status_vector,
				  USHORT	flag)
{
/**************************************
 *
 *	W N E T _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Establish half of a communication link.  If a connect packet is given,
 *	the connection is on behalf of a remote interface.  Otherwise the
 *	connect is for a server process.
 *
 **************************************/
	rem_port* port = alloc_port(0);
	port->port_status_vector = status_vector;
	status_vector[0] = isc_arg_gds;
	status_vector[1] = 0;
	status_vector[2] = isc_arg_end;

	if (port->port_connection) {
		ALLR_free(port->port_connection);
	}
	port->port_connection = make_pipe_name(name, SERVER_PIPE_SUFFIX, 0);

/* If we're a host, just make the connection */

	if (packet)
	{
		THREAD_EXIT();
		while (true) {
			port->port_handle = CreateFile(port->port_connection->str_data,
										   GENERIC_WRITE | GENERIC_READ,
										   0, NULL, OPEN_EXISTING, 0, NULL);
			if (port->port_handle != INVALID_HANDLE_VALUE) {
				break;
			}
			const ISC_STATUS status = GetLastError();
			if (status != ERROR_PIPE_BUSY) {
				THREAD_ENTER();
				wnet_error(port, "CreateFile", isc_net_connect_err, status);
				disconnect(port);
				return NULL;
			}
			WaitNamedPipe(port->port_connection->str_data, 3000L);
		}
		THREAD_ENTER();
		send_full(port, packet);
		return port;
	}

#ifndef REQUESTER
/* We're a server, so wait for a host to show up */

	LPSECURITY_ATTRIBUTES security_attr;
	security_attr = ISC_get_security_desc();
	THREAD_EXIT();
	TEXT command_line[MAXPATHLEN + 32];
	command_line[0] = 0;
	TEXT* p = 0;

	while (true)
	{
		port->port_handle =
			CreateNamedPipe(port->port_connection->str_data,
							PIPE_ACCESS_DUPLEX,
							PIPE_WAIT | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
							PIPE_UNLIMITED_INSTANCES,
							MAX_DATA,
							MAX_DATA,
							0,
							security_attr);
		if (port->port_handle == INVALID_HANDLE_VALUE ||
			GetLastError() == ERROR_CALL_NOT_IMPLEMENTED)
		{
			// TMN: The check for GetLastError() is redundant.
			// This code should NEVER be called if not running on NT,
			// since Win9x does not support the server side of named pipes!
			THREAD_ENTER();
			wnet_error(port, "CreateNamedPipe", isc_net_connect_listen_err,
					   ERRNO);
			disconnect(port);
			return NULL;
		}

		if (!ConnectNamedPipe(port->port_handle, 0) &&
			GetLastError() != ERROR_PIPE_CONNECTED)
		{
			THREAD_ENTER();
			wnet_error(port, "ConnectNamedPipe", isc_net_connect_err, ERRNO);
			disconnect(port);
			return NULL;
		}

		if (flag & (SRVR_debug | SRVR_multi_client))
		{
			THREAD_ENTER();
			port->port_server_flags |= SRVR_server;
			if (flag & SRVR_multi_client)
			{
				port->port_server_flags |= SRVR_multi_client;
			}
			gds__register_cleanup(reinterpret_cast <
								  void (*)(void *) >(exit_handler), port);
			return port;
		}

		if (!command_line[0])
		{

#ifdef CMDLINE_VIA_SERVICE_MANAGER

			SC_HANDLE manager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
			SC_HANDLE service = 0;
			if (manager) {
				service = OpenService(	manager,
										REMOTE_SERVICE,
										SERVICE_QUERY_CONFIG);
			}

			if (manager && service)
			{
				LPQUERY_SERVICE_CONFIG config;
				SCHAR buffer[1024];
				DWORD config_len;

				config = (LPQUERY_SERVICE_CONFIG) buffer;
				if (!QueryServiceConfig
					(service, config, sizeof(buffer), &config_len)) {
					THREAD_ENTER();
					config = (LPQUERY_SERVICE_CONFIG) ALLR_alloc(config_len);
					/* NOMEM: handled by ALLR_alloc, FREE: in this block */
					QueryServiceConfig(service, config, config_len,
									   &config_len);
				}
				sprintf(command_line, "%s -s", config->lpBinaryPathName);
				if ((SCHAR *) config != buffer) {
					ALLR_free(config);
					THREAD_EXIT();
				}
				CloseServiceHandle(service);
			}
			else
			{
				strcpy(command_line, GetCommandLine());
			}
			CloseServiceHandle(manager);
#else
			strcpy(command_line, GetCommandLine());
#endif
			p = command_line + strlen(command_line);
		}

		sprintf(p, " -s -w -h %"SLONGFORMAT, (SLONG) port->port_handle);
		STARTUPINFO           start_crud;
		PROCESS_INFORMATION   pi;
		start_crud.cb = sizeof(STARTUPINFO);
		start_crud.lpReserved = NULL;
		start_crud.lpReserved2 = NULL;
		start_crud.cbReserved2 = 0;
		start_crud.lpDesktop = NULL;
		start_crud.lpTitle = NULL;
		start_crud.dwFlags = STARTF_FORCEOFFFEEDBACK;
		const USHORT ret = CreateProcess(NULL,
							command_line,
							NULL,
							NULL,
							TRUE,
							(flag & SRVR_high_priority ?
							 HIGH_PRIORITY_CLASS | DETACHED_PROCESS :
							 NORMAL_PRIORITY_CLASS | DETACHED_PROCESS),
							NULL, NULL, &start_crud, &pi);
		if (ret) {
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
		CloseHandle(port->port_handle);
	}
#endif /* REQUESTER */
}


rem_port* WNET_reconnect(HANDLE handle, ISC_STATUS* status_vector)
{
/**************************************
 *
 *	W N E T _ r e c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	A communications link has been established by another
 *	process.  We have inheritted the handle.  Set up
 *	a port block.
 *
 **************************************/
	rem_port* port = alloc_port(0);
	port->port_status_vector = status_vector;
	status_vector[0] = isc_arg_gds;
	status_vector[1] = 0;
	status_vector[2] = isc_arg_end;

	if (port->port_connection)
		ALLR_free(port->port_connection);
	port->port_connection = make_pipe_name(NULL, SERVER_PIPE_SUFFIX, 0);

	port->port_handle = handle;
	port->port_server_flags |= SRVR_server;

	return port;
}


rem_port* WNET_server(void *handle)
{
/**************************************
 *
 *	W N E T _ s e r v e r
 *
 **************************************
 *
 * Functional description
 *	We have been spawned by a master server with a connection
 *	established.  Set up port block with the appropriate socket.
 *
 **************************************/
	rem_port* port = alloc_port(0);
	port->port_server_flags |= SRVR_server;
	port->port_handle = (HANDLE) handle;

	return port;
}


static int accept_connection( rem_port* port, P_CNCT * cnct)
{
/**************************************
 *
 *	a c c e p t _ c o n n e c t i o n
 *
 **************************************
 *
 * Functional description
 *	Accept an incoming request for connection.  This is purely a lower
 *	level handshaking function, and does not constitute the server
 *	response for protocol selection.
 *
 **************************************/
	TEXT name[BUFFER_TINY], password[BUFFER_TINY];

/* Default account to "guest" (in theory all packets contain a name) */

	strcpy(name, "guest");
	password[0] = 0;

/* Pick up account and password, if given */

	const TEXT* id = (TEXT *) cnct->p_cnct_user_id.cstr_address;
	const TEXT* const end = id + cnct->p_cnct_user_id.cstr_length;

	while (id < end)
		switch (*id++) {
		case CNCT_user:
			{
				const int length = *id++;
				rem_str* string= (rem_str*) ALLR_block(type_str, length);
				port->port_user_name = string;
				string->str_length = length;
				if (length) {
					TEXT* p = (TEXT *) string->str_data;
					int l = length;
					do {
						*p++ = *id++;
					} while (--l);
				}
				strncpy(name, string->str_data, length);
				name[length] = (TEXT) 0;
				break;
			}

		case CNCT_passwd:
			{
				TEXT* p = password;
				int length = *id++;
				if (length != 0) {
					do {
						*p++ = *id++;
					} while (--length);
				}
				*p = 0;
				break;
			}

		case CNCT_user_verification:
			id++;
			break;

		default:
			id += *id + 1;
		}

	return TRUE;
}


static rem_port* alloc_port( rem_port* parent)
{
/**************************************
 *
 *	a l l o c _ p o r t
 *
 **************************************
 *
 * Functional description
 *	Allocate a port block, link it in to parent (if there is a parent),
 *	and initialize input and output XDR streams.
 *
 **************************************/
	rem_port* port = (rem_port*) ALLR_block(type_port, BUFFER_SIZE * 2);
	port->port_type = port_pipe;
	port->port_state = state_pending;

	TEXT buffer[BUFFER_TINY];
	ISC_get_host(buffer, sizeof(buffer));
	port->port_host = REMOTE_make_string(buffer);
	port->port_connection = REMOTE_make_string(buffer);
	sprintf(buffer, "WNet (%s)", port->port_host->str_data);
	port->port_version = REMOTE_make_string(buffer);

	if (parent) {
		port->port_parent = parent;
		port->port_next = parent->port_clients;
		parent->port_clients = parent->port_next = port;
		port->port_handle = parent->port_handle;
		port->port_server = parent->port_server;
		port->port_server_flags = parent->port_server_flags;
		if (port->port_connection)
			ALLR_free(port->port_connection);
		port->port_connection =
			REMOTE_make_string(parent->port_connection->str_data);
	}

	port->port_accept = accept_connection;
	port->port_disconnect = disconnect;
	port->port_receive_packet = receive;
	port->port_send_packet = send_full;
	port->port_send_partial = send_partial;
	port->port_connect = aux_connect;
	port->port_request = aux_request;
	port->port_buff_size = BUFFER_SIZE;

	xdrwnet_create(&port->port_send, port,
				   &port->port_buffer[BUFFER_SIZE], BUFFER_SIZE, XDR_ENCODE);

	xdrwnet_create(&port->port_receive, port, port->port_buffer, 0,
				   XDR_DECODE);

	return port;
}


// Third param "ast" is unused.
static rem_port* aux_connect( rem_port* port, PACKET* packet, t_event_ast ast)
{
/**************************************
 *
 *	a u x _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Try to establish an alternative connection.  Somebody has already
 *	done a successfull connect request ("packet" contains the response).
 *
 **************************************/
#ifndef REQUESTER
/* If this is a server, we're got an auxiliary connection.  Accept it */

	if (port->port_server_flags) {
		if (!ConnectNamedPipe(port->port_handle, 0) &&
			GetLastError() != ERROR_PIPE_CONNECTED)
		{
			wnet_error(port, "ConnectNamedPipe", isc_net_event_connect_err,
					   ERRNO);
			disconnect(port);
			return NULL;
		}

		port->port_flags |= PORT_async;
		return port;
	}
#endif /* REQUESTER */

/* The server will be sending its process id in the packet to
 * create a unique pipe name.
 */

	P_RESP* response = &packet->p_resp;

	TEXT str_pid[32];
	TEXT* p = 0;
	if (response->p_resp_data.cstr_length) {
		wnet_copy(reinterpret_cast<const char*>(response->p_resp_data.cstr_address),
				  str_pid,
				  response->p_resp_data.cstr_length);
		str_pid[response->p_resp_data.cstr_length] = 0;
		p = str_pid;
	}

	rem_port* new_port = alloc_port(port->port_parent);
	port->port_async = new_port;
	new_port->port_flags |= PORT_async;
	new_port->port_connection =
		make_pipe_name(port->port_connection->str_data, EVENT_PIPE_SUFFIX, p);

	THREAD_EXIT();
	while (true) {
		new_port->port_handle =
			CreateFile(new_port->port_connection->str_data, GENERIC_READ, 0,
					   NULL, OPEN_EXISTING, 0, NULL);
		if (new_port->port_handle != INVALID_HANDLE_VALUE)
			break;
		const ISC_STATUS status = GetLastError();
		if (status != ERROR_PIPE_BUSY) {
			THREAD_ENTER();
			return (rem_port*) wnet_error(new_port, "CreateFile",
									 isc_net_event_connect_err, status);
		}
		WaitNamedPipe(new_port->port_connection->str_data, 3000L);
	}

	THREAD_ENTER();

	new_port->port_flags = port->port_flags & PORT_no_oob;

	return new_port;
}


static rem_port* aux_request( rem_port* vport, PACKET* packet)
{
/**************************************
 *
 *	a u x _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	A remote interface has requested the server prepare an auxiliary 
 *	connection; the server calls aux_request to set up the connection.
 *	Send the servers process id on the packet.  If at a later time
 *	a multi client server is used, there may be a need to
 *	generate a unique id based on connection.
 *
 **************************************/
	rem_port* new_port = NULL;  // If this is the client, we will return NULL

#ifndef REQUESTER
	const DWORD server_pid = GetCurrentProcessId();
	vport->port_async = new_port = alloc_port(vport->port_parent);
	new_port->port_server_flags = vport->port_server_flags;
	new_port->port_flags = vport->port_flags & PORT_no_oob;

	TEXT str_pid[32];
	wnet_make_file_name(str_pid, server_pid);
	new_port->port_connection =
		make_pipe_name(vport->port_connection->str_data, EVENT_PIPE_SUFFIX, str_pid);

	LPSECURITY_ATTRIBUTES security_attr = ISC_get_security_desc();
	THREAD_EXIT();
	new_port->port_handle =
		CreateNamedPipe(new_port->port_connection->str_data,
						PIPE_ACCESS_DUPLEX,
						PIPE_WAIT | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
						PIPE_UNLIMITED_INSTANCES,
						MAX_DATA,
						MAX_DATA,
						0,
						security_attr);
	THREAD_ENTER();
	if (new_port->port_handle == INVALID_HANDLE_VALUE) {
		wnet_error(new_port, "CreateNamedPipe", isc_net_event_listen_err,
				   ERRNO);
		disconnect(new_port);
		return NULL;
	}

	P_RESP* response = &packet->p_resp;
	response->p_resp_data.cstr_length = strlen(str_pid);
	wnet_copy(str_pid,
			  reinterpret_cast<char*>(response->p_resp_data.cstr_address),
			  response->p_resp_data.cstr_length);

#endif /* REQUESTER */

	return new_port;
}


static void disconnect(rem_port* port)
{
/**************************************
 *
 *	d i s c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Break a remote connection.
 *
 **************************************/

/* If this is a sub-port, unlink it from it's parent */

	rem_port* parent = port->port_parent;
	if (parent)
	{
		if (port->port_async)
		{
			disconnect(port->port_async);
			port->port_async = NULL;
		}
		for (rem_port** ptr = &parent->port_clients; *ptr; ptr = &(*ptr)->port_next)
		{
			if (*ptr == port)
			{
				*ptr = port->port_next;
				if (ptr == &parent->port_clients) {
					parent->port_next = *ptr;
				}
				break;
			}
		}
	}
	else if (port->port_async)
	{
/* If we're MULTI_THREAD then we cannot free the port because another
 * thread might be using it.  If we're SUPERSERVER we must free the
 * port to avoid a memory leak.  What we really need to know is if we
 * have multi-threaded events, but this is transport specific.
 */
#if     (defined (MULTI_THREAD) && !defined (SUPERSERVER))
		port->port_async->port_flags |= PORT_disconnect;
#else
		disconnect(port->port_async);
		port->port_async = NULL;
#endif
	}

#ifndef REQUESTER
	if (port->port_server_flags & SRVR_server)
	{
		FlushFileBuffers(port->port_handle);
		DisconnectNamedPipe(port->port_handle);
		if (port->port_flags & PORT_impersonate)
		{
			RevertToSelf();
			port->port_flags &= ~PORT_impersonate;
		}
	}
#endif /* REQUESTER */
	if (port->port_handle) {
		CloseHandle(port->port_handle);
		port->port_handle = 0;
	}
	gds__unregister_cleanup(reinterpret_cast<void (*)(void*)>(exit_handler),
	                        port);
	cleanup_port(port);
}


static void cleanup_port( rem_port* port)
{
/**************************************
 *
 *      c l e a n u p _ p o r t
 *
 **************************************
 *
 * Functional description
 *      Walk through the port structure freeing
 *      allocated memory and then free the port.
 *
 **************************************/

	if (port->port_version)
		ALLR_free((UCHAR *) port->port_version);

	if (port->port_connection)
		ALLR_free((UCHAR *) port->port_connection);

	if (port->port_user_name)
		ALLR_free((UCHAR *) port->port_user_name);

	if (port->port_host)
		ALLR_free((UCHAR *) port->port_host);

	if (port->port_object_vector)
		ALLR_free((UCHAR *) port->port_object_vector);

#ifdef DEBUG_XDR_MEMORY
	if (port->port_packet_vector)
		ALLR_free((UCHAR *) port->port_packet_vector);
#endif

	ALLR_release((UCHAR *) port);
	return;
}


static void exit_handler( rem_port* main_port)
{
/**************************************
 *
 *	e x i t _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Shutdown all active connections
 *	to allow restart.
 *
 **************************************/
	for (rem_port* vport = main_port; vport; vport = vport->port_next)
		CloseHandle(vport->port_handle);
}


static rem_str* make_pipe_name(
						  const TEXT* connect_name,
						  const TEXT* suffix_name,
						  const TEXT* str_pid)
{
/**************************************
 *
 *	m a k e _ p i p e _ n a m e
 *
 **************************************
 *
 * Functional description
 *	Construct a name for the pipe connection.
 *	Figure out whether we need a remote node name,
 *	and construct the pipe name accordingly.
 *	If a server pid != 0, append it to pipe name  as <>/<pid>
 *
 **************************************/
	TEXT buffer[BUFFER_TINY];

	const TEXT* p = connect_name;
	TEXT* q = buffer;

	if (!p || *p++ != '\\' || *p++ != '\\')
		p = ".";

	*q++ = '\\';
	*q++ = '\\';
	while (*p && *p != '\\' && *p != '@')
		*q++ = *p++;

	const TEXT* protocol = 0;
	if (!*p)
		protocol = Config::getRemoteServiceName();
	else if (*p == '@')
		protocol = p + 1;
	else {
		while (*p)
			if (*p++ == '\\')
				protocol = p;
	}

	*q++ = '\\';
	strcpy(q, PIPE_PREFIX);
	q += strlen(PIPE_PREFIX);
	*q++ = '\\';
	const char *pipe_name = Config::getRemotePipeName();
	strcpy(q, pipe_name);
	q += strlen(pipe_name);
	*q++ = '\\';
	strcpy(q, suffix_name);
	q += strlen(suffix_name);
	*q++ = '\\';
	strcpy(q, protocol);

	if (str_pid) {
		TEXT pid[32];
		sprintf(pid, "\\%s", str_pid);
		strcat(buffer, pid);
	}

	return REMOTE_make_string(buffer);
}


static rem_port* receive( rem_port* main_port, PACKET * packet)
{
/**************************************
 *
 *	r e c e i v e
 *
 **************************************
 *
 * Functional description
 *	Receive a message from a port or clients of a port.  If the process
 *	is a server and a connection request comes in, generate a new port
 *	block for the client.
 *
 **************************************/

	if (!xdr_protocol(&main_port->port_receive, packet))
		packet->p_operation = op_exit;

	return main_port;
}


static int send_full( rem_port* port, PACKET * packet)
{
/**************************************
 *
 *	s e n d _ f u l l
 *
 **************************************
 *
 * Functional description
 *	Send a packet across a port to another process.
 *
 **************************************/

	if (!xdr_protocol(&port->port_send, packet))
		return FALSE;

	return xdrwnet_endofrecord(&port->port_send, TRUE);
}


static int send_partial( rem_port* port, PACKET * packet)
{
/**************************************
 *
 *	s e n d _ p a r t i a l
 *
 **************************************
 *
 * Functional description
 *	Send a packet across a port to another process.
 *
 **************************************/

	return xdr_protocol(&port->port_send, packet);
}


static int xdrwnet_create(
						  XDR * xdrs,
						  rem_port* port,
						  UCHAR * buffer, USHORT length, enum xdr_op x_op)
{
/**************************************
 *
 *	x d r w n e t _ c r e a t e
 *
 **************************************
 *
 * Functional description
 *	Initialize an XDR stream for Apollo mailboxes.
 *
 **************************************/

	xdrs->x_public = (caddr_t) port;
	xdrs->x_base = xdrs->x_private = (SCHAR *) buffer;
	xdrs->x_handy = length;
	xdrs->x_ops = &wnet_ops;
	xdrs->x_op = x_op;

	return TRUE;
}


static bool_t xdrwnet_endofrecord( XDR * xdrs, bool_t flushnow)
{
/**************************************
 *
 *	x d r w n e t _ e n d o f r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Write out the rest of a record.
 *
 **************************************/

	return wnet_write(xdrs, flushnow);
}


static int wnet_destroy( XDR * xdrs)
{
/**************************************
 *
 *	w n e t _ d e s t r o y
 *
 **************************************
 *
 * Functional description
 *	Destroy a stream.  A no-op.
 *
 **************************************/

	return 0;
}


static int wnet_error(
					  rem_port* port,
					  const TEXT* function, ISC_STATUS operation, int status)
{
/**************************************
 *
 *	w n e t _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	An I/O error has occurred.  If a status vector is present,
 *	generate an error return.  In any case, return NULL, which
 *	is used to indicate and error.
 *
 **************************************/
	TEXT node_name[MAXPATHLEN];

	strcpy(node_name, ((SCHAR *) port->port_connection->str_data) + 2);
	TEXT* p = strchr(node_name, '\\');
	if (p != NULL)
		*p = '\0';

	if (status) {
		wnet_gen_error(port, isc_network_error,
					   isc_arg_string, (ISC_STATUS) node_name,
					   isc_arg_gds, operation,
					   SYS_ERR, status,
					   0);
		if (status != ERROR_CALL_NOT_IMPLEMENTED) {
            TEXT msg[BUFFER_TINY];
			sprintf(msg, "WNET/wnet_error: %s errno = %d", function, status);
			gds__log(msg, 0, 0, 0, 0);
		}
	}
	else {
		wnet_gen_error(port, isc_network_error,
					   isc_arg_string, (ISC_STATUS) node_name,
					   isc_arg_gds, operation, 0);
	}

	return 0;
}


static void wnet_gen_error( rem_port* port, ISC_STATUS status, ...)
{
/**************************************
 *
 *	w n e t _ g e n _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	An error has occurred.  Mark the port as broken.
 *	Format the status vector if there is one and
 *	save the status vector strings in a permanent place.
 *
 **************************************/
	port->port_flags |= PORT_broken;
	port->port_state = state_broken;

	ISC_STATUS* status_vector = NULL;
	if (port->port_context != NULL)
		status_vector = port->port_context->rdb_status_vector;
	if (status_vector == NULL)
		status_vector = port->port_status_vector;
	if (status_vector != NULL) {
		STUFF_STATUS(status_vector, status);
		REMOTE_save_status_strings(status_vector);
	}
}


static bool_t wnet_getbytes( XDR * xdrs, SCHAR * buff, u_int count)
{
/**************************************
 *
 *	w n e t _ g e t b y t e s
 *
 **************************************
 *
 * Functional description
 *	Get a bunch of bytes from a memory stream if it fits.
 *
 **************************************/
	SLONG bytecount = count;

/* Use memcpy to optimize bulk transfers. */

	while (bytecount > (SLONG) sizeof(ISC_QUAD)) {
		if (xdrs->x_handy >= bytecount) {
			memcpy(buff, xdrs->x_private, bytecount);
			xdrs->x_private += bytecount;
			xdrs->x_handy -= bytecount;
			return TRUE;
		}
		else {
			if (xdrs->x_handy > 0) {
				memcpy(buff, xdrs->x_private, xdrs->x_handy);
				xdrs->x_private += xdrs->x_handy;
				buff += xdrs->x_handy;
				bytecount -= xdrs->x_handy;
				xdrs->x_handy = 0;
			}
			if (!wnet_read(xdrs))
				return FALSE;
		}
	}

/* Scalar values and bulk transfer remainder fall thru
   to be moved byte-by-byte to avoid memcpy setup costs. */

	if (!bytecount)
		return TRUE;

	if (xdrs->x_handy >= bytecount) {
		xdrs->x_handy -= bytecount;
		do {
			*buff++ = *xdrs->x_private++;
		} while (--bytecount);
		return TRUE;
	}

	while (--bytecount >= 0) {
		if (!xdrs->x_handy && !wnet_read(xdrs))
			return FALSE;
		*buff++ = *xdrs->x_private++;
		--xdrs->x_handy;
	}

	return TRUE;
}


static bool_t wnet_getlong( XDR * xdrs, SLONG * lp)
{
/**************************************
 *
 *	w n e t _ g e t l o n g
 *
 **************************************
 *
 * Functional description
 *	Fetch a longword into a memory stream if it fits.
 *
 **************************************/
	SLONG l;

	if (!(*xdrs->x_ops->x_getbytes) (xdrs, reinterpret_cast<char*>(&l), 4))
		return FALSE;

	*lp = ntohl(l);

	return TRUE;
}


static u_int wnet_getpostn( XDR * xdrs)
{
/**************************************
 *
 *	w n e t _ g e t p o s t n
 *
 **************************************
 *
 * Functional description
 *	Get the current position (which is also current length) from stream.
 *
 **************************************/

	return (u_int) (xdrs->x_private - xdrs->x_base);
}


static caddr_t wnet_inline( XDR * xdrs, u_int bytecount)
{
/**************************************
 *
 *	w n e t _  i n l i n e
 *
 **************************************
 *
 * Functional description
 *	Return a pointer to somewhere in the buffer.
 *
 **************************************/

	if (bytecount > (u_int) xdrs->x_handy)
		return FALSE;

	return xdrs->x_base + bytecount;
}


static bool_t wnet_putbytes( XDR* xdrs, const SCHAR* buff, u_int count)
{
/**************************************
 *
 *	w n e t _ p u t b y t e s
 *
 **************************************
 *
 * Functional description
 *	Put a bunch of bytes to a memory stream if it fits.
 *
 **************************************/
	SLONG bytecount = count;

/* Use memcpy to optimize bulk transfers. */

	while (bytecount > (SLONG) sizeof(ISC_QUAD)) {
		if (xdrs->x_handy >= bytecount) {
			memcpy(xdrs->x_private, buff, bytecount);
			xdrs->x_private += bytecount;
			xdrs->x_handy -= bytecount;
			return TRUE;
		}
		else {
			if (xdrs->x_handy > 0) {
				memcpy(xdrs->x_private, buff, xdrs->x_handy);
				xdrs->x_private += xdrs->x_handy;
				buff += xdrs->x_handy;
				bytecount -= xdrs->x_handy;
				xdrs->x_handy = 0;
			}
			if (!wnet_write(xdrs, 0))
				return FALSE;
		}
	}

/* Scalar values and bulk transfer remainder fall thru
   to be moved byte-by-byte to avoid memcpy setup costs. */

	if (!bytecount)
		return TRUE;

	if (xdrs->x_handy >= bytecount) {
		xdrs->x_handy -= bytecount;
		do {
			*xdrs->x_private++ = *buff++;
		} while (--bytecount);
		return TRUE;
	}

	while (--bytecount >= 0) {
		if (xdrs->x_handy <= 0 && !wnet_write(xdrs, 0))
			return FALSE;
		--xdrs->x_handy;
		*xdrs->x_private++ = *buff++;
	}

	return TRUE;
}


static bool_t wnet_putlong( XDR * xdrs, const SLONG* lp)
{
/**************************************
 *
 *	w n e t _ p u t l o n g
 *
 **************************************
 *
 * Functional description
 *	Fetch a longword into a memory stream if it fits.
 *
 **************************************/
	const SLONG l = htonl(*lp);
	return (*xdrs->x_ops->x_putbytes) (xdrs,
									   reinterpret_cast<const char*>(AOF32L(l)),
									   4);
}


static bool_t wnet_read( XDR * xdrs)
{
/**************************************
 *
 *	w n e t _ r e a d
 *
 **************************************
 *
 * Functional description
 *	Read a buffer full of data.  If we receive a bad packet,
 *	send the moral equivalent of a NAK and retry.  ACK all
 *	partial packets.  Don't ACK the last packet -- the next
 *	message sent will handle this.
 *
 **************************************/
	rem_port* port = (rem_port*) xdrs->x_public;
	SCHAR* p = xdrs->x_base;
	const SCHAR* const end = p + BUFFER_SIZE;

/* If buffer is not completely empty, slide down what what's left */

	if (xdrs->x_handy > 0) {
		memmove(p, xdrs->x_private, xdrs->x_handy);
		p += xdrs->x_handy;
	}

/* If an ACK is pending, do an ACK.  The alternative is deadlock. */

/*
if (port->port_flags & PORT_pend_ack)
    if (!packet_send (port, 0, 0))
	return FALSE;
*/

	while (true) {
		SSHORT length = end - p;
		if (!packet_receive
			(port, reinterpret_cast<UCHAR*>(p), length, &length))
		{
			return FALSE;
	/***
	if (!packet_send (port, 0, 0))
	    return FALSE;
	continue;
	***/
		}
		if (length >= 0) {
			p += length;
			break;
		}
		p -= length;
		if (!packet_send(port, 0, 0))
			return FALSE;
	}

	port->port_flags |= PORT_pend_ack;
	xdrs->x_handy = (int) ((SCHAR *) p - xdrs->x_base);
	xdrs->x_private = xdrs->x_base;

	return TRUE;
}


static bool_t wnet_setpostn( XDR * xdrs, u_int bytecount)
{
/**************************************
 *
 *	w n e t _ s e t p o s t n
 *
 **************************************
 *
 * Functional description
 *	Set the current position (which is also current length) from stream.
 *
 **************************************/

	if (bytecount > (u_int) xdrs->x_handy)
		return FALSE;

	xdrs->x_private = xdrs->x_base + bytecount;

	return TRUE;
}


static bool_t wnet_write( XDR * xdrs, bool_t end_flag)
{
/**************************************
 *
 *	w n e t _ w r i t e
 *
 **************************************
 *
 * Functional description
 *	Write a buffer fulll of data.  If the end_flag isn't set, indicate
 *	that the buffer is a fragment, and reset the XDR for another buffer
 *	load.
 *
 **************************************/
/* Encode the data portion of the packet */

	rem_port* vport = (rem_port*) xdrs->x_public;
	const SCHAR* p = xdrs->x_base;
	SSHORT length = xdrs->x_private - p;

/* Send data in manageable hunks.  If a packet is partial, indicate
   that with a negative length.  A positive length marks the end. */

	while (length) {
		vport->port_misc1 = (vport->port_misc1 + 1) % MAX_SEQUENCE;
		const SSHORT l = MIN(length, MAX_DATA);
		length -= l;
		if (!packet_send(vport, p, (SSHORT) (length ? -l : l)))
			return FALSE;
		p += l;
	}

	xdrs->x_private = xdrs->x_base;
	xdrs->x_handy = BUFFER_SIZE;

	return TRUE;
}


#ifdef DEBUG
static void packet_print(const TEXT* string, const UCHAR* packet,
	const int length)
{
/**************************************
 *
 *	p a c k e t _ p r i n t
 *
 **************************************
 *
 * Functional description
 *	Print a summary of packet.
 *
 **************************************/
	int sum = 0;
	int l = length;

	if (l) {
		do {
			sum += *packet++;
		} while (--l);
	}

	printf("%s\t: length = %d, checksum = %d\n", string, length, sum);
}
#endif


static int packet_receive(
						  rem_port* port,
						  UCHAR * buffer,
						  SSHORT buffer_length, SSHORT * length)
{
/**************************************
 *
 *	p a c k e t _ r e c e i v e
 *
 **************************************
 *
 * Functional description
 *	Receive a packet and pass on it's goodness.  If it's good,
 *	return TRUE and the reported length of the packet, and update
 *	the receive sequence number.  If it's bad, return FALSE.  If it's
 *	a duplicate message, just ignore it.
 *
 **************************************/
	DWORD n = 0;

	THREAD_EXIT();
	const USHORT status =
		ReadFile(port->port_handle, buffer, buffer_length, &n, NULL);
	THREAD_ENTER();
	if (!status && GetLastError() != ERROR_BROKEN_PIPE)
		return wnet_error(port, "ReadFile", isc_net_read_err, ERRNO);
	if (!n)
		return wnet_error(port, "ReadFile end-of-file", isc_net_read_err,
						  ERRNO);

#if defined(DEBUG) && defined(WNET_trace)
	packet_print("receive", buffer, n);
#endif

	*length = (SSHORT) n;

	return TRUE;
}


static int packet_send( rem_port* port, const SCHAR* buffer, SSHORT buffer_length)
{
/**************************************
 *
 *	p a c k e t _ s e n d
 *
 **************************************
 *
 * Functional description
 *	Send some data on it's way.  
 *
 **************************************/
	const SCHAR* data = buffer;
	const DWORD length = buffer_length;

	THREAD_EXIT();
	DWORD n;
	const USHORT status = WriteFile(port->port_handle, data, length, &n, NULL);
	THREAD_ENTER();
	if (!status)
		return wnet_error(port, "WriteFile", isc_net_write_err, ERRNO);
	if (n != length)
		return wnet_error(port, "WriteFile truncated", isc_net_write_err,
						  ERRNO);

#if defined(DEBUG) && defined(WNET_trace)
	packet_print("send", (UCHAR*)buffer, buffer_length);
#endif

	port->port_flags &= ~PORT_pend_ack;

	return TRUE;
}


static void wnet_copy(const SCHAR* from, SCHAR* to, int length)
{
/**************************************
 *
 *      w n e t _ c o p y
 *
 **************************************
 *
 * Functional description
 *      Copy a number of bytes;
 *
 **************************************/

	if (length) {
		do {
			*to++ = *from++;
		} while ((--length) != 0);
	}
}


static void wnet_make_file_name( TEXT* name, DWORD number)
{
/**************************************
 *
 *      w n e t _ m a k e _ f i l e _ n a m e
 *
 **************************************
 *
 * Functional description
 *      Create a file name out of a number making sure
 *	the Windows <8>.<3> limitations are handled.
 *
 **************************************/
	TEXT temp[32];

	sprintf(temp, "%lu", number);

	USHORT length = strlen(temp);
	if ((length) < 8) {
		strcpy(name, temp);
		return;
	}

	TEXT* p = name;
	const TEXT* q = temp;

	while (length) {
		USHORT len = (length > 8) ? 8 : length;
		length -= len;
		do {
			*p++ = *q++;
		} while ((--len) != 0);

		if (length)
			*p++ = '\\';
	}
	*p++ = 0;
}

