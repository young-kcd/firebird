/*
 *	PROGRAM:	JRD Remote Interface/ Windows Client
 *	MODULE:		spxnet32.c
 *	DESCRIPTION:	SPX 32 bit Windows Client Communications module.
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
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "../jrd/common.h"
#include <stdarg.h>

#include "../jrd/time.h"

#ifdef SUPERSERVER
#define FD_SETSIZE 256
#endif /* SUPERSERVER */

#define GETHOSTBYNAME gethostbyname
#define GETHOSTNAME gethostname

#include "../remote/remote.h"
#include "../jrd/gds.h"
#include "../jrd/iberr.h"
#include "../jrd/thd.h"
#include "../remote/spxnet32_proto.h"
#include "../remote/proto_proto.h"
#include "../remote/remot_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"

#include "../jrd/isc_i_proto.h"
#include "../jrd/sch_proto.h"

#include <wsipx.h>
#include <wsnwlink.h>



#define FILESERVER_SAP_NUMBER		0x0004
#define SERVICE_QUERY_PACKET_TYPE	4

typedef struct nodeaddr
{
	char name[20];
	SOCKADDR_IPX addr;
	SOCKET SAPSocket;
} NODEADDR;

/* Packet structure for broadcast SAP packets. Refer to Novell docs. */
typedef struct saphdr
{
	WORD operation;
	WORD servicetype;
	char name[48];
	char network[4];
	char node[6];
	char socket[2];
} SAPHDR;

/* Packet structure for broadcast Service SERVICE_QUERY packets. Refer to Novell docs. */
typedef struct servicequery {
	WORD operation;
	WORD servicetype;
} SERVICE_QUERY;


#define ERRNO		WSAGetLastError()
#define H_ERRNO		WSAGetLastError()
#define SOCLOSE		closesocket
#define SYS_ERR		gds_arg_win32

#ifndef ENOBUFS
#define ENOBUFS	0
#endif

/* 576 - (spx header + 4) match server. 
   This is really required only for Win 95 as it appears to be 
   either a limitation or a bug in the SPX stack.
   Though NT works fine for a larger size, let's use this accross the board */
#define MAX_DATA	530
#define BUFFER_SIZE	MAX_DATA
#define MAX_SEQUENCE	256

#ifndef MAXPATHLEN
#define MAXPATHLEN	1024
#endif

#ifndef MAXHOSTLEN
#define MAXHOSTLEN	64
#endif

static PORT alloc_port(PORT);
static PORT aux_connect(PORT, PACKET *, XDR_INT(*)(void));
static unsigned char btoh(char);
static void cleanup_port(PORT);
static void disconnect(PORT);
static void exit_handler(void *);
static void spxnet32_copy(SCHAR *, SCHAR *, int);
static int spxnet32_destroy(XDR *);
static void spxnet32_gen_error(PORT, STATUS, ...);
static bool_t spxnet32_getbytes(XDR *, SCHAR *, u_int);
static bool_t spxnet32_getlong(XDR *, SLONG *);
static u_int spxnet32_getpostn(XDR *);
static caddr_t spxnet32_inline(XDR *, u_int);
static int spxnet32_error(PORT, CONST TEXT *, STATUS, int);
static bool_t spxnet32_putlong(XDR *, SLONG *);
static bool_t spxnet32_putbytes(XDR *, SCHAR *, u_int);
static bool_t spxnet32_read(XDR *);
static bool_t spxnet32_setpostn(XDR *, u_int);
static PORT spxnet32_try_connect(PACKET *,
								 RDB,
								 USHORT,
								 TEXT *, TEXT *, STATUS *, SCHAR *, SSHORT);
static bool_t spxnet32_write(XDR *, int);
static void spxnet32_zero(SCHAR *, int);
static bool_t packet_receive(PORT, UCHAR *, SSHORT, SSHORT *);
static bool_t packet_send(PORT, SCHAR *, SSHORT);
static PORT receive(PORT, PACKET *);
static int send_full(PORT, PACKET *);
static int send_partial(PORT, PACKET *);
void WINAPI SAP_handler_thread(LPVOID);
static void unhook_port(PORT, PORT);
static int xdrspxnet32_create(XDR *, PORT, UCHAR *, USHORT, enum xdr_op);
static bool_t xdrspxnet32_endofrecord(XDR *, int);
static BOOL get_ipx_network_address(NODEADDR * nodeAddr);


static const xdr_t::xdr_ops spxnet32_ops = {
	spxnet32_getlong,
	spxnet32_putlong,
	spxnet32_getbytes,
	spxnet32_putbytes,
	spxnet32_getpostn,
	spxnet32_setpostn,
	spxnet32_inline,
	spxnet32_destroy
};

#ifndef MAX_PTYPE
#define MAX_PTYPE	ptype_batch_send
#endif

static int SPXNET32_initialized = FALSE;
static WSADATA SPXNET32_wsadata;

#define	INTERRUPT_ERROR(x)	(SYSCALL_INTERRUPTED(x) || (x) == WSAEINTR)

#ifdef  START_PORT_CRITICAL
#undef  START_PORT_CRITICAL
#undef  STOP_PORT_CRITICAL
#undef  DEFER_PORT_CLEANUP
#endif

#ifdef  SUPERSERVER
#include "../jrd/thd_proto.h"

static MUTX_T port_mutex;
static BOOL port_mutex_inited = 0;

#define DEFER_PORT_CLEANUP

#define START_PORT_CRITICAL     if (!port_mutex_inited)                \
                                    {                                  \
                                    port_mutex_inited = 1;             \
                                    THD_mutex_init (&port_mutex);      \
                                    }                                  \
                                THREAD_EXIT;                           \
                                THD_mutex_lock (&port_mutex);          \
                                THREAD_ENTER

#define STOP_PORT_CRITICAL      THREAD_EXIT;                           \
                                THD_mutex_unlock (&port_mutex);        \
                                THREAD_ENTER

#endif /* SUPERSERVER */

#ifndef START_PORT_CRITICAL
#define START_PORT_CRITICAL
#define STOP_PORT_CRITICAL
#endif





PORT SPXNET32_analyze(
					  TEXT * file_name,
					  USHORT * file_length,
					  STATUS * status_vector,
					  TEXT * node_name,
TEXT * user_string, USHORT uv_flag, SCHAR * dpb, SSHORT dpb_length)
{
/**************************************
 *
 *	S P X N E T 3 2 _ a n a l y z e
 *
 **************************************
 *
 * Functional description
 *	File_name is on node_name.
 *	Establish an external connection to node_name.
 *
 *	If a connection is established, return a port block, otherwise
 *	return NULL.
 *
 **************************************/
	RDB rdb;
	PORT port;
	PACKET *packet;
	P_CNCT *cnct;
	int eff_gid, eff_uid;
	SSHORT user_length;
	UCHAR *p, user_id[200];
	TEXT buffer[64];
	p_cnct::p_cnct_repeat * protocol;

	*file_length = strlen(file_name);

/* We need to establish a connection to a remote server.  Allocate the necessary
   blocks and get ready to go. */

	rdb = (RDB) ALLOC(type_rdb);
	packet = &rdb->rdb_packet;

/* Pick up some user identification information */
	user_id[0] = CNCT_user;
	p = user_id + 2;
	ISC_get_user(reinterpret_cast < char *>(p), &eff_uid, &eff_gid, 0, 0, 0,
				 user_string);
	user_id[1] = (UCHAR) strlen((SCHAR *) p);
	p = p + user_id[1];

	*p++ = CNCT_host;
	p++;
	ISC_get_host(reinterpret_cast < char *>(p), 64);
	p[-1] = (UCHAR) strlen((SCHAR *) p);

	for (; *p; p++)
		if (*p >= 'A' && *p <= 'Z')
			*p = *p - 'A' + 'a';

	if ((eff_uid == -1) || uv_flag) {
		*p++ = CNCT_user_verification;
		*p++ = 0;
	}
	else {
		/* Communicate group id info to server, as user maybe running under group
		   id other than default specified in /etc/passwd. */

		*p++ = CNCT_group;
		*p++ = sizeof(SLONG);
		eff_gid = htonl(eff_gid);
		memcpy(p, AOF32L(eff_gid), sizeof(SLONG));
		p += sizeof(SLONG);
	}

	user_length = (SSHORT) (p - user_id);

/* Establish connection to server */

	cnct = &packet->p_cnct;
	cnct->p_cnct_count = 4;
	cnct->p_cnct_user_id.cstr_length = user_length;
	cnct->p_cnct_user_id.cstr_address = user_id;

	protocol = cnct->p_cnct_versions;

	protocol->p_cnct_version = PROTOCOL_VERSION9;
	protocol->p_cnct_architecture = arch_generic;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = MAX_PTYPE;
	protocol->p_cnct_weight = 2;

	++protocol;

	protocol->p_cnct_version = PROTOCOL_VERSION9;
	protocol->p_cnct_architecture = ARCHITECTURE;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = MAX_PTYPE;
	protocol->p_cnct_weight = 3;

	++protocol;

	protocol->p_cnct_version = PROTOCOL_VERSION10;
	protocol->p_cnct_architecture = arch_generic;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = MAX_PTYPE;
	protocol->p_cnct_weight = 4;

	++protocol;

	protocol->p_cnct_version = PROTOCOL_VERSION10;
	protocol->p_cnct_architecture = ARCHITECTURE;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = MAX_PTYPE;
	protocol->p_cnct_weight = 5;

	port = spxnet32_try_connect(packet, rdb, *file_length, file_name,
								node_name, status_vector, dpb, dpb_length);
	if (!port)
		return NULL;

	if (packet->p_operation != op_accept) {
		*status_vector++ = gds_arg_gds;
		*status_vector++ = gds_connect_reject;
		*status_vector++ = 0;
		disconnect(port);
		return NULL;
	}

	port->port_protocol = packet->p_acpt.p_acpt_version;

#ifdef DEBUG
	ib_printf("port proto version = %d\n", port->port_protocol);
#endif

/* once we've decided on a protocol, concatenate the version 
   string to reflect it...  */

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


PORT DLL_EXPORT SPXNET32_connect(TEXT * name,
								 PACKET * packet,
								 STATUS * status_vector,
								 USHORT flag, SCHAR * dpb, SSHORT dpb_length)
{
/**************************************
 *
 *	S P X N E T 3 2 _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Establish half of a communication link.  If a connect packet is given,
 *	the connection is on behalf of a remote interface.  Otherwise the connect
 *	is for a server process.
 *
 **************************************/
	int n;
	PORT port;
	TEXT temp[128], *p;
	NODEADDR nodeAddr;

	port = alloc_port(0);
	port->port_status_vector = status_vector;
	REMOTE_get_timeout_params(port, reinterpret_cast < UCHAR * >(dpb),
							  dpb_length);
	status_vector[0] = gds_arg_gds;
	status_vector[1] = 0;
	status_vector[2] = gds_arg_end;

	if (name) {
		strcpy(temp, name);
		for (p = temp; *p;)
			if (*p++ == '/') {
				p[-1] = 0;
				name = temp;
				break;
			}
	}

	if (name && *name) {
		if (port->port_connection)
			ALLR_free(port->port_connection);
		port->port_connection = REMOTE_make_string(name);
	}
	else
		name = port->port_host->str_data;


/* Allocate a port block and initialize a socket for communications */

	port->port_handle = (HANDLE) socket(AF_IPX, SOCK_STREAM, NSPROTO_SPX);
	if ((SOCKET) port->port_handle == INVALID_SOCKET) {
		spxnet32_error(port, "socket", isc_net_connect_err, ERRNO);
		return NULL;
	}
	strcpy(nodeAddr.name, name);


	if (packet) {
#ifdef  DEBUG
		ib_printf("Get IPX net address & node number\n");
#endif
		if (get_ipx_network_address(&nodeAddr) == NULL) {
#ifdef  DEBUG
			ib_printf("Get IPX address failed\n");
#endif
			spxnet32_error(port, "connect addr", isc_net_connect_err, ERRNO);
			return NULL;
		}
	}

/* If we're a host, just make the connection */

	if (packet) {
		THREAD_EXIT;
		nodeAddr.addr.sa_family = AF_IPX;
		memcpy(&(nodeAddr.addr.sa_socket), "\x94\x87", 2);
#ifdef DEBUG
		ib_printf("connect to node (sock %d) \n", port->port_handle);
		ib_printf("net = %x,%x,%x,%x node = %x,%x,%x,%x,%x,%x, sock %d\n",
				  (unsigned char) nodeAddr.addr.sa_netnum[0],
				  (unsigned char) nodeAddr.addr.sa_netnum[1],
				  (unsigned char) nodeAddr.addr.sa_netnum[2],
				  (unsigned char) nodeAddr.addr.sa_netnum[3],
				  (unsigned char) nodeAddr.addr.sa_nodenum[0],
				  (unsigned char) nodeAddr.addr.sa_nodenum[1],
				  (unsigned char) nodeAddr.addr.sa_nodenum[2],
				  (unsigned char) nodeAddr.addr.sa_nodenum[3],
				  (unsigned char) nodeAddr.addr.sa_nodenum[4],
				  (unsigned char) nodeAddr.addr.sa_nodenum[5],
				  nodeAddr.addr.sa_socket);
#endif

		n =
			connect((SOCKET) port->port_handle,
					(SOCKADDR *) & (nodeAddr.addr), 14);
		THREAD_ENTER;
		if (n != SOCKET_ERROR && send_full(port, packet)) {
#ifdef DEBUG
			ib_printf("connected to node\n");
#endif
			return port;
		}
		else {
			spxnet32_error(port, "connect", isc_net_connect_err, ERRNO);
			disconnect(port);
			return NULL;
		}
	}

	return NULL;
}


PORT SPXNET32_reconnect(HANDLE handle, TEXT * name, STATUS * status_vector)
{
/**************************************
 *
 *	S P X N E T 3 2 _ r e c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	A communications link has been established by another
 *	process.  We have inheritted the handle.  Set up
 *	a port block.
 *
 **************************************/
	PORT port;

	port = alloc_port(0);
	port->port_status_vector = status_vector;
	status_vector[0] = gds_arg_gds;
	status_vector[1] = 0;
	status_vector[2] = gds_arg_end;

	port->port_handle = handle;
	port->port_server_flags |= SRVR_server;

	return port;
}


static PORT alloc_port( PORT parent)
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
	PORT port;
	TEXT buffer[64];

	if (!SPXNET32_initialized) {
		WORD version;

		version = MAKEWORD(2, 0);
		if (WSAStartup(version, &SPXNET32_wsadata)) {
			if (parent)
				spxnet32_error(parent, "WSAStartup", isc_net_init_error,
							   ERRNO);
			else {
				sprintf(buffer,
						"SPXNET32/alloc_port: WSAStartup failed, error code = %d",
						ERRNO);
				gds__log(buffer, 0);
			}
			return NULL;
		}
		gds__register_cleanup(exit_handler, 0);
		SPXNET32_initialized = TRUE;
	}
	port = (PORT) ALLOCV(type_port, BUFFER_SIZE * 2);
	port->port_type = port_spx;
	port->port_state = state_pending;
	REMOTE_get_timeout_params(port, 0, 0);

	GETHOSTNAME(buffer, sizeof(buffer));

	port->port_host = REMOTE_make_string(buffer);
	port->port_connection = REMOTE_make_string(buffer);
	sprintf(buffer, "spx (%s)", port->port_host->str_data);
	port->port_version = REMOTE_make_string(buffer);

	START_PORT_CRITICAL;
	if (parent && !(parent->port_server_flags & SRVR_thread_per_port)) {
		port->port_parent = parent;
		port->port_next = parent->port_clients;
		parent->port_clients = parent->port_next = port;
		port->port_handle = parent->port_handle;
		port->port_server = parent->port_server;
		port->port_server_flags = parent->port_server_flags;
	}
	STOP_PORT_CRITICAL;

	port->port_receive_packet = receive;
	port->port_disconnect = disconnect;
	port->port_send_packet = send_full;
	port->port_send_partial = send_partial;
	port->port_connect =
		reinterpret_cast < PORT(*)(PORT, PACKET *, void (*)()) >(aux_connect);
	port->port_buff_size = BUFFER_SIZE;

	xdrspxnet32_create(&port->port_send, port,
					   &port->port_buffer[BUFFER_SIZE], BUFFER_SIZE,
					   XDR_ENCODE);

	xdrspxnet32_create(&port->port_receive, port, port->port_buffer, 0,
					   XDR_DECODE);

	return port;
}


static PORT aux_connect( PORT port, PACKET * packet, XDR_INT(*ast) (void))
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
	P_RESP *response;
	SOCKET n;
	int status;
	PORT new_port;
	SOCKADDR_IPX address;

#ifdef DEBUG
	ib_printf("aux_connect from client\n");
#endif
	port->port_async = new_port = alloc_port(port->port_parent);

	new_port->port_dummy_packet_interval = port->port_dummy_packet_interval;

	new_port->port_flags |= PORT_async;
	response = &packet->p_resp;

/* Set up new socket */

	n = (SOCKET) socket(AF_IPX, SOCK_STREAM, NSPROTO_SPX);
	if (n == INVALID_SOCKET) {
		spxnet32_error(port, "socket", isc_net_event_connect_err, ERRNO);
		return NULL;
	}

#ifdef DEBUG
	ib_printf("aux_connect from client: sock created\n");
#endif
	spxnet32_zero((SCHAR *) & address, sizeof(address));
	address.sa_family = AF_IPX;
	spxnet32_copy(reinterpret_cast <
				  char *>(response->p_resp_data.cstr_address),
				  &(address.sa_netnum[0]), response->p_resp_data.cstr_length);
	THREAD_EXIT;
	status = connect(n, (SOCKADDR *) & address, 14);
	THREAD_ENTER;

	if (status < 0) {
		spxnet32_error(port, "connect", isc_net_event_connect_err, ERRNO);
		SOCLOSE(n);
		return NULL;
	}

	new_port->port_handle = (HANDLE) n;
	new_port->port_flags |= port->port_flags & PORT_no_oob;

#ifdef DEBUG
	ib_printf("aux_con: port = %x, port_flags = %x\n",
			  new_port, new_port->port_flags);
#endif

	return new_port;
}


static void cleanup_port( PORT port)
{
/**************************************
 *
 *	c l e a n u p _ p o r t 
 *
 **************************************
 *
 * Functional description
 *	Walk through the port structure freeing
 * 	allocated memory and then free the port.
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


static BOOL decode_sap( NODEADDR * nodeAddr, SAPHDR * lpSap, int cbBufSize)
{
	int count = cbBufSize / sizeof(SAPHDR);
	int i;

	for (i = 0; i < count; i++, lpSap++) {
#ifdef DEBUG
		ib_printf("lpsap oper = %x\n", lpSap->operation);
#endif
		if (lpSap->servicetype != htons(FILESERVER_SAP_NUMBER))
			continue;
#ifdef DEBUG
		ib_printf("sap parsing(iter %d): %s\n", i, lpSap->name);
#endif
		CharLowerBuff(lpSap->name, strlen(lpSap->name));
		CharLowerBuff(nodeAddr->name, strlen(nodeAddr->name));
		if (strcmp(nodeAddr->name, lpSap->name) != 0)
			continue;
		memcpy(&nodeAddr->addr.sa_netnum, &lpSap->network, 12);
		return TRUE;
	}
	return FALSE;
}


static void disconnect( PORT port)
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
	PORT parent;
#ifdef	DEFER_PORT_CLEANUP
	SSHORT defer_cleanup = 0;
#endif


	shutdown((int) port->port_handle, 2);

/* If this is a sub-port, unlink it from it's parent */

#ifdef  DEFER_PORT_CLEANUP
	port->port_state = state_disconnected;
#endif
	if ((parent = port->port_parent) != NULL) {
		if (port->port_async) {
			disconnect(port->port_async);
			port->port_async = NULL;
		}
#ifdef	DEFER_PORT_CLEANUP
		defer_cleanup = 1;
#else
		unhook_port(port, parent);
#endif
	}
	else if (port->port_async) {
#ifdef MULTI_THREAD
		port->port_async->port_flags |= PORT_disconnect;
#else
		disconnect(port->port_async);
		port->port_async = NULL;
#endif
	}

	SOCLOSE((SOCKET) port->port_handle);

	gds__unregister_cleanup(exit_handler, (void *) port);

#ifdef DEFER_PORT_CLEANUP
	if (!defer_cleanup)
#endif
		cleanup_port(port);
	return;
}


static void exit_handler( void *arg)
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
	PORT port, main_port;

	main_port = (PORT) arg;

	if (!main_port) {
		WSACleanup();
		return;
	}

	for (port = main_port; port; port = port->port_next) {
		shutdown((int) port->port_handle, 2);
		SOCLOSE((SOCKET) port->port_handle);
	}
}


static BOOL get_ipx_network_address( NODEADDR * nodeAddr)
{
	HANDLE hSAPhdlrThread;
	DWORD SAPhdlrThreadID;
	SOCKADDR_IPX SAPSockAddr;
	DWORD dwRet;
	int bcaststat;
	int reuseaddr;
	SERVICE_QUERY query;
	SOCKADDR_IPX queryAddr;
	DWORD serviceQueryPacketType = SERVICE_QUERY_PACKET_TYPE;
	SOCKET s;

	if ((nodeAddr->SAPSocket = socket(AF_IPX,
									  SOCK_DGRAM,
									  NSPROTO_IPX + 4)) != INVALID_SOCKET) {
		bcaststat = TRUE;
		reuseaddr = TRUE;
		setsockopt(nodeAddr->SAPSocket, SOL_SOCKET, SO_BROADCAST,
				   (char *) &bcaststat, sizeof(bcaststat));
		setsockopt(nodeAddr->SAPSocket, SOL_SOCKET, SO_REUSEADDR,
				   (char *) &reuseaddr, sizeof(reuseaddr));
		spxnet32_zero((SCHAR *) & SAPSockAddr, sizeof(SAPSockAddr));
		SAPSockAddr.sa_family = AF_IPX;
		memcpy(&(SAPSockAddr.sa_socket), "\x04\x52", 2);

		if (bind
			(nodeAddr->SAPSocket, (PSOCKADDR) & SAPSockAddr,
			 sizeof(SOCKADDR_IPX)) == SOCKET_ERROR) {
#ifdef DEBUG
			ib_printf("SAP socket bind failed %d.\n", WSAGetLastError());
#endif
			return (NULL);
		}

#ifdef DEBUG
		ib_printf("SAPhdlr Thread create call %d\n", nodeAddr->SAPSocket);
#endif
		hSAPhdlrThread = CreateThread(NULL, 0L, (LPTHREAD_START_ROUTINE) SAP_handler_thread, nodeAddr,	/* node name, addr place holder */
									  0, &SAPhdlrThreadID);

		if (hSAPhdlrThread == NULL) {
			return (NULL);
		}

#ifdef DEBUG
		ib_printf("SAPhdlr Thread created\n");
#endif
		if ((s = socket(AF_IPX, SOCK_DGRAM, NSPROTO_IPX + 4)) ==
			INVALID_SOCKET) {
#ifdef DEBUG
			ib_printf(" socket creation failed %d.\n", WSAGetLastError());
#endif
			TerminateThread(hSAPhdlrThread, 0);
			closesocket(nodeAddr->SAPSocket);
			return NULL;
		}
		dwRet =
			setsockopt(s, NSPROTO_IPX, IPX_PTYPE,
					   (const char *) &serviceQueryPacketType, 4);
		if (dwRet == SOCKET_ERROR) {
#ifdef DEBUG
			ib_printf("SERVICE query set packet type error : %d\n",
					  WSAGetLastError());
#endif
			TerminateThread(hSAPhdlrThread, 0);
			closesocket(s);
			closesocket(nodeAddr->SAPSocket);
			return NULL;
		}
		spxnet32_zero((SCHAR *) & queryAddr, sizeof(queryAddr));
		queryAddr.sa_family = AF_IPX;
		memcpy(&(queryAddr.sa_netnum), "\x00\x00\x00\x00", 4);
		memcpy(&(queryAddr.sa_nodenum), "\xFF\xFF\xFF\xFF\xFF\xFF", 6);
		memcpy(&(queryAddr.sa_socket), "\x04\x52", 2);

		query.operation = htons(0x0001);
		query.servicetype = htons(FILESERVER_SAP_NUMBER);

		dwRet = sendto(nodeAddr->SAPSocket,
					   (LPSTR) & query,
					   sizeof(query),
					   0, (const struct sockaddr *) &queryAddr, 16);
		if (dwRet != SOCKET_ERROR) {
#ifdef DEBUG
			ib_printf("SERVICE query packet sent\n");
#endif
		}
		else {
#ifdef DEBUG
			ib_printf("SERVICE query send Err: %d\n", WSAGetLastError());
#endif
			TerminateThread(hSAPhdlrThread, 0);
			closesocket(s);
			closesocket(nodeAddr->SAPSocket);
			return NULL;
		}

		if (WaitForSingleObject(hSAPhdlrThread, 90 * 1000) == WAIT_OBJECT_0) {
#ifdef DEBUG
			ib_printf("SAPhdlr thread completes address resolution\n");
#endif
			closesocket(s);
			closesocket(nodeAddr->SAPSocket);
			return TRUE;
		}
		else {
#ifdef DEBUG
			ib_printf("SAPhdlr thread timed out\n");
#endif
			TerminateThread(hSAPhdlrThread, 0);
			closesocket(s);
			closesocket(nodeAddr->SAPSocket);
			return NULL;
		}
	}
#ifdef DEBUG
	ib_printf("SAP socket creation failed %d.\n", WSAGetLastError());
#endif
	return NULL;
}


static void spxnet32_copy( SCHAR * from, SCHAR * to, int length)
{
/**************************************
 *
 *	s p x n e t 3 2 _ c o p y
 *
 **************************************
 *
 * Functional description
 *	Copy a number of bytes;
 *
 **************************************/

	if (length)
		do
			*to++ = *from++;
		while ((--length) != 0);
}


static void spxnet32_zero( SCHAR * address, int length)
{
/**************************************
 *
 *	s p x n e t 3 2 _  z e r o
 *
 **************************************
 *
 * Functional description
 *	Zero a block of memory.
 *
 **************************************/

	if (length)
		do
			*address++ = 0;
		while ((--length) != 0);
}


static PORT receive( PORT main_port, PACKET * packet)
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

/* If this isn't a multi-client server, just do the operation and get it
   over with */

	if (!(main_port->port_server_flags & SRVR_multi_client)) {
		/* loop as long as we are receiving dummy packets, just
		   throwing them away--note that if we are a server we won't 
		   be receiving them, but it is better to check for them at 
		   this level rather than try to catch them in all places where 
		   this routine is called */

		do {
#ifdef DEBUG
			ib_printf("receive calls xdr_protocol\n");
#endif
			if (!xdr_protocol(&main_port->port_receive, packet))
				return (PORT) 0;
		}
		while (packet->p_operation == op_dummy);

		return main_port;
	}
	return (PORT) 0;
}


static int send_full( PORT port, PACKET * packet)
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

#ifdef DEBUG
	ib_printf("send_full calls xdr_protocol\n");
#endif
	if (!xdr_protocol(&port->port_send, packet))
		return FALSE;

	return xdrspxnet32_endofrecord(&port->port_send, TRUE);
}


static int send_partial( PORT port, PACKET * packet)
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

#ifdef DEBUG
	ib_printf("send_partial calls xdr_protocol\n");
#endif
	return xdr_protocol(&port->port_send, packet);
}



static int xdrspxnet32_create(
							  XDR * xdrs,
							  PORT port,
							  UCHAR * buffer, USHORT length, enum xdr_op x_op)
{
/**************************************
 *
 *	x d r s p x n e t 3 2 _ c r e a t e
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
	xdrs->x_ops = &spxnet32_ops;
	xdrs->x_op = x_op;

	return TRUE;
}


static bool_t xdrspxnet32_endofrecord( XDR * xdrs, bool_t flushnow)
{
/**************************************
 *
 *	x d r s p x n e t 3 2 _ e n d o f r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Write out the rest of a record.
 *
 **************************************/

	return spxnet32_write(xdrs, flushnow);
}


static XDR_INT spxnet32_destroy( XDR * xdrs)
{
/**************************************
 *
 *	s p x n e t 3 2 _ d e s t r o y
 *
 **************************************
 *
 * Functional description
 *	Destroy a stream.  A no-op.
 *
 **************************************/

	return NULL;
}


static void spxnet32_gen_error( PORT port, STATUS status, ...)
{
/**************************************
 *
 *	s p x n e t 3 2 _ g e n _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	An error has occurred.  Mark the port as broken.
 *	Format the status vector if there is one and
 *	save the status vector strings in a permanent place.
 *
 **************************************/
	STATUS *status_vector;

	port->port_flags |= PORT_broken;
	port->port_state = state_broken;

	status_vector = NULL;
	if (port->port_context != NULL)
		status_vector = port->port_context->rdb_status_vector;
	if (status_vector == NULL)
		status_vector = port->port_status_vector;
	if (status_vector != NULL) {
		STUFF_STATUS(status_vector, status);
		REMOTE_save_status_strings(status_vector);
	}
}


static bool_t spxnet32_getbytes( XDR * xdrs, SCHAR * buff, u_int count)
{
/**************************************
 *
 *	s p x n e t 3 2 _ g e t b y t e s
 *
 **************************************
 *
 * Functional description
 *	Get a bunch of bytes from a memory stream if it fits.
 *
 **************************************/
	SLONG bytecount = count;

/* Use memcpy to optimize bulk transfers. */

	while (bytecount > sizeof(GDS_QUAD)) {
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
			if (!spxnet32_read(xdrs))
				return FALSE;
		}
	}

/* Scalar values and bulk transfer remainder fall thru
   to be moved byte-by-byte to avoid memcpy setup costs. */

	if (!bytecount)
		return TRUE;

	if (xdrs->x_handy >= bytecount) {
		xdrs->x_handy -= bytecount;
		do
			*buff++ = *xdrs->x_private++;
		while (--bytecount);
		return TRUE;
	}

	while (--bytecount >= 0) {
		if (!xdrs->x_handy && !spxnet32_read(xdrs))
			return FALSE;
		*buff++ = *xdrs->x_private++;
		--xdrs->x_handy;
	}

	return TRUE;
}


static bool_t spxnet32_getlong( XDR * xdrs, SLONG * lp)
{
/**************************************
 *
 *	s p x n e t 3 2 _ g e t l o n g
 *
 **************************************
 *
 * Functional description
 *	Fetch a longword into a memory stream if it fits.
 *
 **************************************/

	SLONG l;

	if (!(*xdrs->x_ops->x_getbytes) (xdrs, reinterpret_cast < char *>(&l), 4))
		return FALSE;

	*lp = ntohl(l);

	return TRUE;
}


static u_int spxnet32_getpostn( XDR * xdrs)
{
/**************************************
 *
 *	s p x n e t 3 2 _ g e t p o s t n
 *
 **************************************
 *
 * Functional description
 *	Get the current position (which is also current length) from stream.
 *
 **************************************/

	return (u_int) (xdrs->x_private - xdrs->x_base);
}


static caddr_t spxnet32_inline( XDR * xdrs, u_int bytecount)
{
/**************************************
 *
 *	s p x n e t 3 2 _  i n l i n e
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


static int spxnet32_error(
						  PORT port,
						  CONST TEXT * function, STATUS operation, int status)
{
/**************************************
 *
 *	s p x n e t 3 2 _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	An I/O error has occurred.  Call
 * 	spxnet32_gen_error with the appropriate args
 *	to format the status vector if any.
 *	In any case, return NULL, which
 *	is used to indicate an error.
 *
 **************************************/
	TEXT msg[64];

	if (status) {
		spxnet32_gen_error(port, isc_network_error,
						   gds_arg_string,
						   (STATUS) port->port_connection->str_data,
						   isc_arg_gds, operation, SYS_ERR, status, 0);

		sprintf(msg, "SPXNET32/spxnet32_error: %s errno = %d", function,
				status);
		gds__log(msg, 0);
	}
	else {
		/* No status value, just format the basic arguments. */

		spxnet32_gen_error(port, isc_network_error,
						   gds_arg_string,
						   (STATUS) port->port_connection->str_data,
						   isc_arg_gds, operation, 0);
	}

	return (int) NULL;
}


static bool_t spxnet32_putbytes( XDR * xdrs, SCHAR * buff, u_int count)
{
/**************************************
 *
 *	s p x n e t 3 2 _ p u t b y t e s
 *
 **************************************
 *
 * Functional description
 *	Put a bunch of bytes to a memory stream if it fits.
 *
 **************************************/
	SLONG bytecount = count;

/* Use memcpy to optimize bulk transfers. */

	while (bytecount > sizeof(GDS_QUAD)) {
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
			if (!spxnet32_write(xdrs, 0))
				return FALSE;
		}
	}

/* Scalar values and bulk transfer remainder fall thru
   to be moved byte-by-byte to avoid memcpy setup costs. */

	if (!bytecount)
		return TRUE;

	if (xdrs->x_handy >= bytecount) {
		xdrs->x_handy -= bytecount;
		do
			*xdrs->x_private++ = *buff++;
		while (--bytecount);
		return TRUE;
	}

	while (--bytecount >= 0) {
		if (xdrs->x_handy <= 0 && !spxnet32_write(xdrs, 0))
			return FALSE;
		--xdrs->x_handy;
		*xdrs->x_private++ = *buff++;
	}

	return TRUE;
}


static bool_t spxnet32_putlong( XDR * xdrs, SLONG * lp)
{
/**************************************
 *
 *	s p x n e t 3 2 _ p u t l o n g
 *
 **************************************
 *
 * Functional description
 *	Fetch a longword into a memory stream if it fits.
 *
 **************************************/
	SLONG l;

	l = htonl(*lp);
	return (*xdrs->x_ops->x_putbytes) (xdrs,
									   reinterpret_cast < char *>(AOF32L(l)),
									   4);
}


static bool_t spxnet32_read( XDR * xdrs)
{
/**************************************
 *
 *	s p x n e t 3 2 _ r e a d
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
	PORT port;
	SSHORT length;
	SCHAR *p, *end;

	port = (PORT) xdrs->x_public;
	p = xdrs->x_base;
	end = p + BUFFER_SIZE;

/* If buffer is not completely empty, slide down what's left */

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

	while (TRUE) {
		length = end - p;
		if (!packet_receive
			(port, reinterpret_cast < UCHAR * >(p), length, &length)) {
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
#ifdef DEBUG
		ib_printf("********************************************SEND ACK \n");
#endif
		if (!packet_send(port, 0, 0))
			return FALSE;
	}

	port->port_flags |= PORT_pend_ack;
	xdrs->x_handy = (int) ((SCHAR *) p - xdrs->x_base);
	xdrs->x_private = xdrs->x_base;


	return TRUE;
}


static bool_t spxnet32_setpostn( XDR * xdrs, u_int bytecount)
{
/**************************************
 *
 *	s p x n e t 3 2 _ s e t p o s t n
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


static PORT spxnet32_try_connect(
								 PACKET * packet,
								 RDB rdb,
								 USHORT file_length,
								 TEXT * file_name,
TEXT * node_name, STATUS * status_vector, SCHAR * dpb, SSHORT dpb_length)
{
/**************************************
 *
 *	S P X N E T 3 2 _ t r y _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Given a packet with formatted protocol infomation,
 *	set header information and try the connection.
 *
 *	If a connection is established, return a port block, otherwise
 *	return NULL.
 *
 **************************************/
	PORT port;
	P_CNCT *cnct;

	cnct = &packet->p_cnct;
	packet->p_operation = op_connect;
	cnct->p_cnct_operation = op_attach;
	cnct->p_cnct_cversion = CONNECT_VERSION2;
	cnct->p_cnct_client = ARCHITECTURE;
	cnct->p_cnct_file.cstr_length = file_length;
	cnct->p_cnct_file.cstr_address = (UCHAR *) file_name;

/* If we can't talk to a server, punt.  Let somebody else generate
   an error.  status_vector will have the network error info. */

	port =
		SPXNET32_connect(node_name, packet, status_vector, FALSE, dpb,
						 dpb_length);
	if (!port) {
		ALLR_release((BLK) rdb);
		return NULL;
	}

/* Get response packet from server. */

	rdb->rdb_port = port;
	port->port_context = rdb;
	if (!port->receive(packet)) {
		spxnet32_error(port, "receive in try_connect", isc_net_connect_err,
					   ERRNO);
		disconnect(port);
		ALLR_release((BLK) rdb);
		return NULL;
	}

	return port;

}


static bool_t spxnet32_write( XDR * xdrs, bool_t end_flag)
{
/**************************************
 *
 *	s p x n e t 3 2 _ w r i t e
 *
 **************************************
 *
 * Functional description
 *	Write a buffer full of data.  If the end_flag isn't set, indicate
 *	that the buffer is a fragment, and reset the XDR for another buffer
 *	load.
 *
 **************************************/
	SCHAR *p;
	PORT port;
	SSHORT l, length;

/* Encode the data portion of the packet */

	port = (PORT) xdrs->x_public;
	p = xdrs->x_base;
	length = xdrs->x_private - p;

#ifdef DEBUG
	ib_printf("spxnet32_write: length = %d\n", length);
#endif
/* Send data in manageable hunks.  If a packet is partial, indicate
   that with a negative length.  A positive length marks the end. */

	p = xdrs->x_base;

	while (length) {
		port->port_misc1 = (port->port_misc1 + 1) % MAX_SEQUENCE;
		l = MIN(length, MAX_DATA);
		length -= l;
		if (!packet_send(port, p, (SSHORT) (length ? -l : l)))
			return FALSE;
		p += l;
	}

	xdrs->x_private = xdrs->x_base;
	xdrs->x_handy = BUFFER_SIZE;

	return TRUE;

}


static int packet_receive(
						  PORT port,
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
	int n;
	struct timeval timeout, *time_ptr;
	int ph;
	fd_set slct_fdset;
	int slct_count;
	PACKET packet;

/* set the time interval for sending dummy packets to the client */

	timeout.tv_sec = port->port_dummy_packet_interval;

	if (port->port_protocol >= PROTOCOL_VERSION8 &&
		port->port_dummy_packet_interval > 0) {
#ifdef DEBUG
		ib_printf("ver 8\n");
#endif
		time_ptr = &timeout;
	}
	else
		time_ptr = NULL;


/* If the protocol is 0 we are still in the process of establishing
   a connection. Add a time out to the wait */
	if (port->port_protocol == 0) {
		timeout.tv_sec = port->port_connect_timeout;
		time_ptr = &timeout;
	}

	ph = (int) port->port_handle;

	for (;;) {
#ifdef DEBUG
		ib_printf("packet rece for loop \n");
#endif

		/* Implement an error-detection protocol to ensure that the client
		   is still there.  Use the select() call with a timeout to wait on 
		   the connection for an incoming packet.  If none comes within a
		   suitable time interval, write a dummy packet on the connection.
		   If the client is not there, an error will be returned on the write.
		   If the client is there, the dummy packet will be ignored by all 
		   InterBase clients V4 or greater.  This protocol will detect when 
		   clients are lost abnormally through reboot or network disconnect. */

		/* Don't send op_dummy packets on aux port; the server won't
		   read them because it only writes to aux ports. */

		if (port->port_flags & PORT_async);
		else {
			FD_ZERO(&slct_fdset);
			FD_SET(ph, &slct_fdset);

			THREAD_EXIT;
			for (;;) {
				slct_count = select(FD_SETSIZE, &slct_fdset,
									(fd_set *) NULL, (fd_set *) NULL,
									time_ptr);
				if (slct_count != -1 || !INTERRUPT_ERROR(ERRNO))
					break;
			}
			THREAD_ENTER;

			if (slct_count == -1)
				return spxnet32_error(port, "select in packet_receive",
									  isc_net_read_err, ERRNO);

			if (!slct_count && port->port_protocol >= PROTOCOL_VERSION8) {
				packet.p_operation = op_dummy;
				if (!send_full(port, &packet))
					return FALSE;
				continue;
			}

			if (!slct_count && port->port_protocol == 0)
				return FALSE;
		}

		THREAD_EXIT;
		n =
			recv((SOCKET) port->port_handle,
				 reinterpret_cast < char *>(buffer), buffer_length, 0);
		THREAD_ENTER;
		if (n != -1 || !INTERRUPT_ERROR(ERRNO))
			break;
	}
#ifdef DEBUG
	ib_printf("recv length %d\n", n);
#endif

	if (n <= 0 && (port->port_flags & PORT_async))
		return FALSE;

	if (n == -1)
		return spxnet32_error(port, "read", isc_net_read_err, ERRNO);

	if (!n)
		return spxnet32_error(port, "read end_of_file", isc_net_read_err, 0);

	*length = n;

	return TRUE;
}


static bool_t packet_send( PORT port, SCHAR * buffer, SSHORT buffer_length)
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
	SSHORT n, length, count;
	SCHAR *data;

	data = buffer;
	length = buffer_length;

	while (length) {
#ifdef DEBUG
		ib_printf("calling send: %d\n", length);
#endif
		THREAD_EXIT;
		n = send((SOCKET) port->port_handle, data, length, 0);
		THREAD_ENTER;
		if (n == length)
			break;
		else if (n == -1) {
			if (INTERRUPT_ERROR(ERRNO))
				continue;
			else
				return spxnet32_error(port, "send", isc_net_write_err, ERRNO);
		}
		data += n;
		length -= n;
	}


	if ((port->port_flags & PORT_async) && !(port->port_flags & PORT_no_oob)) {
		THREAD_EXIT;
		count = 0;
		while ((n = send((SOCKET) port->port_handle, buffer, 1, MSG_OOB)) ==
			   -1 && (ERRNO == ENOBUFS || INTERRUPT_ERROR(ERRNO))) {
			if (count++ > 20)
				break;
			SleepEx(50, TRUE);
		}
		THREAD_ENTER;
		if (n == -1)
			return spxnet32_error(port, "send/oob", isc_net_write_err, ERRNO);
	}

	port->port_flags &= ~PORT_pend_ack;

	return TRUE;
}


void WINAPI SAP_handler_thread( LPVOID param)
{
	SAPHDR sapData[20];
	SOCKADDR_IPX RequestAddr;
	int iRet, iAddr;
	NODEADDR *nodeAddr = (NODEADDR *) param;

#ifdef DEBUG
	ib_printf("Start Thread to parse SAP broadcasts (sock %d)\n",
			  nodeAddr->SAPSocket);
#endif
	for (;;) {
		iAddr = sizeof(SOCKADDR);
		iRet = recvfrom(nodeAddr->SAPSocket,
						(LPSTR) & sapData,
						sizeof(sapData),
						0, (LPSOCKADDR) & RequestAddr, (LPINT) & iAddr);

		if (iRet != SOCKET_ERROR) {
#ifdef DEBUG
			ib_printf("decode_sap packets\n");
#endif
			if (decode_sap((NODEADDR *) nodeAddr, sapData, iRet) == TRUE)
				ExitThread(0);
		}
#ifdef DEBUG
		else
			ib_printf("RecvFrom Err: %d\n", WSAGetLastError());
#endif
	}
}


static void unhook_port( PORT port, PORT parent)
{
/**************************************
 *
 *      u n h o o k _ p a r e n t
 *
 **************************************
 *
 * Functional description
 *      Disconnect a port from its parent
 *      This must be done under
 *      START_PORT_CRITICAL/STOP_PORT_CRITICAL control.
 *
 **************************************/
	PORT *ptr;

	for (ptr = &parent->port_clients; *ptr; ptr = &(*ptr)->port_next)
		if (*ptr == port) {
			*ptr = port->port_next;
			if (ptr == &parent->port_clients)
				parent->port_next = *ptr;
			break;
		}
}
