/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		ipc.c
 *	DESCRIPTION:	TCP/NETipc Communications module
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
#include <mpe.h>
#include "../remote/remote.h"
#include "../remote/ipc.h"
#include "../jrd/gds.h"
#include "../jrd/thd.h"
#include "../remote/ipc_proto.h"
#include "../remote/remot_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"

#define RECVTIME	3000
#define INFINITETIME	0

#define RPM_IGNORE	31			/* Ignore prior DSLINE command, bit 0 */
#define RPM_1SESSION	1		/* Accommodate multiple sessions, bit 30 */
#define RPM_NOPRIV	19			/* Load program in non-priv mode, bit 12 */

#define PROTOCOL_NAME	"gds_db"
#define PROXY_FILE	"proxy.pub"
#define SERVER_NAME	" gdsrserv.pub.starbase"

#define MAX_PTYPE	ptype_batch_send
#define MAX_DATA	1400
#define BUFFER_SIZE	MAX_DATA
#define MAX_SEQUENCE	256
#define MAX_PACKETLEN	2048
#define MAXHOSTLEN	64
#define MAXLOGINLEN	27
#define RPM_STRLEN	6

#define OPT_SIZE(entries,datalength)	(4+8*entries+datalength+1)/2

#ifdef PRIVMODE_CODE
#define GET_PRIVMODE	if (privmode_flag < 0) privmode_flag = ISC_check_privmode();\
			if (privmode_flag > 0) GETPRIVMODE();
#define GET_USERMODE	if (privmode_flag > 1) GETUSERMODE();
#else
#define GET_PRIVMODE
#define GET_USERMODE
#endif


static int accept_connection(PORT, P_CNCT *);
static PORT alloc_port(PORT);
static void atoh(SCHAR *, SSHORT, SCHAR *);
static PORT aux_connect(PORT, PACKET *, XDR_INT(*)());
static PORT aux_request(PORT, PACKET *);
static void bcopy(SCHAR *, SCHAR *, int);
static void bzero(SCHAR *, int);
static int check_proxy(PORT, TEXT *, TEXT *, TEXT *);
static void cleanup_port(PORT);
static PORT client_connect(TEXT *, USHORT, SSHORT, PORT, PACKET *);
static PORT connect(PORT, PORT);
static void disconnect(PORT);
static void htoa(SCHAR *, SCHAR *, SSHORT);
static XDR_INT ipc_destroy(XDR *);
static int ipc_error(PORT, TEXT *, int);
static bool_t ipc_getbytes(XDR *, SCHAR *, int);
static bool_t ipc_getlong(XDR *, SLONG *);
static u_int ipc_getpostn(XDR *);
static int ipc_handler(SSHORT, void *);
static caddr_t ipc_inline(XDR *, u_int);
static bool_t ipc_putbytes(XDR *, SCHAR *, int);
static bool_t ipc_putlong(XDR *, SLONG *);
static bool_t ipc_read(XDR *);
static bool_t ipc_setpostn(XDR *, u_int);
static bool_t ipc_write(XDR *, bool_t);
static void packet_print(TEXT *, UCHAR *, int);
static int packet_receive(PORT, UCHAR *, SSHORT, SSHORT *);
static int packet_send(PORT, UCHAR *, SSHORT);
static PORT receive(PORT, PACKET *);
static int send(PORT, PACKET *);
static PORT server_connect(USHORT, PORT, int);
static int xdripc_create(XDR *, PORT, UCHAR *, USHORT, enum xdr_op);
static int xdripc_endofrecord(XDR *, bool_t);
static void zap_packet(PACKET *);

static xdr_t::xdr_ops ipc_ops = {
	ipc_getlong,
	ipc_putlong,
	ipc_getbytes,
	ipc_putbytes,
	ipc_getpostn,
	ipc_setpostn,
	ipc_inline,
	ipc_destroy
};

static PORT async_list;
static SSHORT privmode_flag = -1;
static SCHAR hex[] = "0123456789ABCDEF";


PORT IPC_analyze(
				 TEXT * file_name,
				 SSHORT * file_length,
				 STATUS * status_vector, TEXT * node_name, SSHORT uv_flag)
{
/**************************************
 *
 *	I P C _ a n a l y z e
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
	P_CNCT *connect;
	P_ATCH *attach;
	SSHORT user_length;
	TEXT *p, user_id[128], buffer[64];
	p_cnct::p_cnct_repeat * protocol;

	*file_length = strlen(file_name);

/* We need to establish a connection to a remote server.  Allocate the necessary
   blocks and get ready to go. */

	rdb = (RDB) ALLOC(type_rdb);
	packet = &rdb->rdb_packet;

/* Pick up some user identification information */

	user_id[0] = CNCT_user;
	p = user_id + 2;
	ISC_get_user(p, 0, 0, 0, 0, 0, 0);
	user_id[1] = strlen(p);

	for (; *p; p++)
		if (*p >= 'A' && *p <= 'Z')
			*p = *p - 'A' + 'a';

	*p++ = CNCT_host;
	p++;
	ISC_get_host(p, user_id + sizeof(user_id) - p);
	p[-1] = strlen(p);

	for (; *p; p++)
		if (*p >= 'A' && *p <= 'Z')
			*p = *p - 'A' + 'a';

	if (uv_flag) {
		*p++ = CNCT_user_verification;
		*p++ = 0;
	}

	user_length = p - user_id;

/* Establish connection to server */

	connect = &packet->p_cnct;
	packet->p_operation = op_connect;
	connect->p_cnct_operation = op_attach;
	connect->p_cnct_cversion = CONNECT_VERSION2;
	connect->p_cnct_client = ARCHITECTURE;
	connect->p_cnct_file.cstr_length = *file_length;
	connect->p_cnct_file.cstr_address = (UCHAR *) file_name;

/* Note: prior to V3.1E a recievers could not in truth handle more
   then 5 protocol descriptions, so we try them in chunks of 5 or less */

/* If we want user verification, we can't speak anything less than version 7 */

#ifdef SCROLLABLE_CURSORS
	connect->p_cnct_count = 6;
#else
	connect->p_cnct_count = 4;
#endif
	connect->p_cnct_user_id.cstr_length = user_length;
	connect->p_cnct_user_id.cstr_address = (UCHAR *) user_id;

	protocol = connect->p_cnct_versions;

	protocol->p_cnct_version = PROTOCOL_VERSION8;
	protocol->p_cnct_architecture = arch_generic;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = MAX_PTYPE;
	protocol->p_cnct_weight = 2;

	++protocol;

	protocol->p_cnct_version = PROTOCOL_VERSION8;
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

#ifdef SCROLLABLE_CURSORS
	++protocol;

	protocol->p_cnct_version = PROTOCOL_SCROLLABLE_CURSORS;
	protocol->p_cnct_architecture = arch_generic;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = MAX_PTYPE;
	protocol->p_cnct_weight = 6;

	++protocol;

	protocol->p_cnct_version = PROTOCOL_SCROLLABLE_CURSORS;
	protocol->p_cnct_architecture = ARCHITECTURE;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = MAX_PTYPE;
	protocol->p_cnct_weight = 7;
#endif

/* If we can't talk to a server, punt.  Let somebody else generate
   an error. */

	if (!(port = IPC_connect(node_name, packet, status_vector, FALSE, 0))) {
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
		connect->p_cnct_operation = op_attach;
		connect->p_cnct_cversion = CONNECT_VERSION2;
		connect->p_cnct_client = ARCHITECTURE;
		connect->p_cnct_file.cstr_length = *file_length;
		connect->p_cnct_file.cstr_address = (UCHAR *) file_name;

		/* try again with next set of known protocols */

		connect->p_cnct_count = 4;
		connect->p_cnct_user_id.cstr_length = user_length;
		connect->p_cnct_user_id.cstr_address = (UCHAR *) user_id;

		protocol = connect->p_cnct_versions;

		protocol->p_cnct_version = PROTOCOL_VERSION6;
		protocol->p_cnct_architecture = arch_generic;
		protocol->p_cnct_min_type = ptype_rpc;
		protocol->p_cnct_max_type = MAX_PTYPE;
		protocol->p_cnct_weight = 2;

		++protocol;

		protocol->p_cnct_version = PROTOCOL_VERSION6;
		protocol->p_cnct_architecture = ARCHITECTURE;
		protocol->p_cnct_min_type = ptype_rpc;
		protocol->p_cnct_max_type = MAX_PTYPE;
		protocol->p_cnct_weight = 3;

		++protocol;

		protocol->p_cnct_version = PROTOCOL_VERSION7;
		protocol->p_cnct_architecture = arch_generic;
		protocol->p_cnct_min_type = ptype_rpc;
		protocol->p_cnct_max_type = MAX_PTYPE;
		protocol->p_cnct_weight = 4;

		++protocol;

		protocol->p_cnct_version = PROTOCOL_VERSION7;
		protocol->p_cnct_architecture = ARCHITECTURE;
		protocol->p_cnct_min_type = ptype_rpc;
		protocol->p_cnct_max_type = MAX_PTYPE;
		protocol->p_cnct_weight = 5;

		if (!(port = IPC_connect(node_name, packet, status_vector, FALSE, 0))) {
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
		connect->p_cnct_operation = op_attach;
		connect->p_cnct_cversion = CONNECT_VERSION2;
		connect->p_cnct_client = ARCHITECTURE;
		connect->p_cnct_file.cstr_length = *file_length;
		connect->p_cnct_file.cstr_address = (UCHAR *) file_name;

		/* try again with next set of known protocols */

		connect->p_cnct_count = 4;
		connect->p_cnct_user_id.cstr_length = user_length;
		connect->p_cnct_user_id.cstr_address = (UCHAR *) user_id;

		protocol = connect->p_cnct_versions;

		protocol->p_cnct_version = PROTOCOL_VERSION3;
		protocol->p_cnct_architecture = arch_generic;
		protocol->p_cnct_min_type = ptype_rpc;
		protocol->p_cnct_max_type = ptype_batch_send;
		protocol->p_cnct_weight = 2;

		++protocol;

		protocol->p_cnct_version = PROTOCOL_VERSION3;
		protocol->p_cnct_architecture = ARCHITECTURE;
		protocol->p_cnct_min_type = ptype_rpc;
		protocol->p_cnct_max_type = ptype_batch_send;
		protocol->p_cnct_weight = 3;

		++protocol;

		protocol->p_cnct_version = PROTOCOL_VERSION4;
		protocol->p_cnct_architecture = arch_generic;
		protocol->p_cnct_min_type = ptype_rpc;
		protocol->p_cnct_max_type = ptype_batch_send;
		protocol->p_cnct_weight = 4;

		++protocol;

		protocol->p_cnct_version = PROTOCOL_VERSION4;
		protocol->p_cnct_architecture = ARCHITECTURE;
		protocol->p_cnct_min_type = ptype_rpc;
		protocol->p_cnct_max_type = ptype_batch_send;
		protocol->p_cnct_weight = 5;

		if (!(port = IPC_connect(node_name, packet, status_vector, FALSE, 0))) {
			ALLR_release(rdb);
			return NULL;
		}

		/* Get response packet from server. */

		rdb->rdb_port = port;
		port->port_context = rdb;
		port->receive(packet);
	}

	if (packet->p_operation != op_accept) {
		*status_vector++ = gds_arg_gds;
		*status_vector++ = gds__connect_reject;
		*status_vector++ = 0;
		disconnect(port);
		return NULL;
	}

	port->port_protocol = packet->p_acpt.p_acpt_version;

/* once we've decided on a protocol, concatenate the version
   string to reflect it... */

	sprintf(buffer, "%sP%d", port->port_version->str_data,
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


PORT IPC_connect(TEXT * name,
				 PACKET * packet, STATUS * status_vector, int flag)
{
/**************************************
 *
 *	I P C _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Establish half of a communication link.  If a connect packet is given,
 *	the connection is on behalf of a remote interface.  Otherwise it
 *	is for a server process.
 *
 **************************************/
	PORT port;
	TEXT *p;
	SLONG flags, ipcresult;
	SLONG itemnums[6], items[6];
	USHORT gds_socket;

/* Check for enviroment variables first */

	HPCIGETVAR("GDS_DB", &ipcresult, 1, items, 13, &flags);
	if (!ipcresult && flags == 1)
		gds_socket = items[0];
	else
		gds_socket = GDS_SOCKET;

	port = alloc_port(0);
	port->port_status_vector = status_vector;
	status_vector[0] = gds_arg_gds;
	status_vector[1] = 0;
	status_vector[2] = gds_arg_end;

/* Store host name */

	if (name && *name) {
		if (port->port_connection)
			ALLR_free(port->port_connection);
		port->port_connection = REMOTE_make_string(name);
	}
	else
		name = port->port_host->str_data;

/* If we're a host, just make the connection.  Otherwise
   when we're a server, set up our half and return */

	if (packet)
		return client_connect(name, gds_socket, RECVTIME, port, packet);
	else
		return server_connect(gds_socket, port, flag);
}


PORT IPC_server(SCHAR * infostr)
{
/**************************************
 *
 *	I P C _ s e r v e r
 *
 **************************************
 *
 * Functional description
 *	We have been spawned by a master server with a connection
 *	established.  Set up port block with the appropriate socket.
 *
 **************************************/
	PORT port;
	TEXT rpmstring[RPM_STRLEN], vcname[IPC_GIVELEN];
	SLONG vcdesc, ipcresult, ipcflags, length;

	ipcflags = 0;
	htoa(infostr, vcname, sizeof(vcname));
	IPCGET(vcname, IPC_GIVELEN, &ipcflags, &vcdesc, &ipcresult);
	if (ipcresult) {
		gds__log("Remote server call to IPCGET failed with result %ld",
				 ipcresult);
		return 0;
	}

	port = alloc_port(0);
	port->port_server_flags |= SRVR_server;
	port->port_handle = (HANDLE) vcdesc;
	port->port_state = state_active;

	length = RPM_STRLEN;
	RPMGETSTRING(rpmstring, &length, &ipcresult);
	port->port_protocol = atoi(rpmstring);

	length = RPM_STRLEN;
	RPMGETSTRING(rpmstring, &length, &ipcresult);
	port->port_flags = atoi(rpmstring);

	return port;
}


static int accept_connection( PORT port, P_CNCT * connect)
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
	PACKET ack, *packet;
	P_ACPT *accept;
	P_ARCH architecture;
	USHORT weight, version, type, create_flags;
	TEXT name[MAXLOGINLEN], password[MAXLOGINLEN], node[MAXHOSTLEN],
		formal_prog[MAXPATHLEN], vcname[IPC_GIVELEN], pd[16], *id, *end, *p,
		infostr[2 * IPC_GIVELEN], portinfo[RPM_STRLEN];
	STR string;
	SLONG ipcresult, length, l;
	SSHORT infolen, status, user_verification,
		rpmopt[OPT_SIZE
			   (5, 2 * IPC_GIVELEN + 2 * sizeof(SSHORT) + 2 * RPM_STRLEN)];
	ULONG flags;
	p_cnct::p_cnct_repeat * protocol, *endp;

/* Default account to "guest" */

	strcpy(name, "guest");
	password[0] = 0;

/* Pick up account, password, and node if given */

	id = (TEXT *) connect->p_cnct_user_id.cstr_address;
	end = id + connect->p_cnct_user_id.cstr_length;

	user_verification = 0;
	while (id < end)
		switch (*id++) {
		case CNCT_user:
			if (port->port_user_name)
				ALLR_free(port->port_user_name);
			length = l = *id++;
			port->port_user_name = string =
				(STR) ALLOCV(type_str, (int) length);
			string->str_length = length;
			if (length) {
				p = (TEXT *) string->str_data;
				do
					*p++ = *id++;
				while (--l);
			}
			strncpy(name, string->str_data, length);
			name[length] = 0;
			break;

		case CNCT_passwd:
			p = password;
			if (length = *id++)
				do
					*p++ = *id++;
				while (--length);
			*p = 0;
			break;

		case CNCT_host:
			p = node;
			if (length = *id++)
				do
					*p++ = *id++;
				while (--length);
			*p = 0;
			break;

		case CNCT_user_verification:
			user_verification = 1;
			id++;
			break;

		default:
			id += *id + 1;
		}

	if (port->port_server_flags & SRVR_debug)
		return TRUE;

/* Taken from server.c - main XL server must accept connection for child 
   and then pass connection info during RPMCREATE. */

	zap_packet(&ack);
	packet = &ack;
	packet->p_operation = op_reject;
	accept = &packet->p_acpt;
	weight = 0;

/* Select the most appropriate protocol (this will get smarter) */

	protocol = connect->p_cnct_versions;

	for (endp = protocol + connect->p_cnct_count; protocol < endp; protocol++)
		if ((protocol->p_cnct_version == PROTOCOL_VERSION3 ||
			 protocol->p_cnct_version == PROTOCOL_VERSION4 ||
			 protocol->p_cnct_version == PROTOCOL_VERSION5 ||
			 protocol->p_cnct_version == PROTOCOL_VERSION6 ||
			 protocol->p_cnct_version == PROTOCOL_VERSION7 ||
			 protocol->p_cnct_version == PROTOCOL_VERSION8) &&
			(protocol->p_cnct_architecture == arch_generic ||
			 protocol->p_cnct_architecture == ARCHITECTURE) &&
			protocol->p_cnct_weight >= weight) {
			weight = protocol->p_cnct_weight;
			version = protocol->p_cnct_version;
			architecture = protocol->p_cnct_architecture;
			type = MIN(protocol->p_cnct_max_type, ptype_out_of_band);
			packet->p_operation = op_accept;
		}

/*  send the news and, if accepting, hand off to a child. */

	accept->p_acpt_version = port->port_protocol = version;
	accept->p_acpt_architecture = architecture;
	accept->p_acpt_type = type;

	if (architecture == ARCHITECTURE)
		port->port_flags |= PORT_symmetric;

	if (type == ptype_rpc)
		port->port_flags |= PORT_rpc;

	if (type != ptype_out_of_band)
		port->port_flags |= PORT_no_oob;

	if (!check_proxy(port, node, name, password))
		packet->p_operation = op_reject;

	port->send(packet);

	if (packet->p_operation != op_accept) {
		IPCSHUTDOWN(port->port_handle);
		port->port_handle = 0;
		port->port_state = state_pending;
		return FALSE;
	}

/* Put the connection to client on hold - child will pick up with IPCGET. */

	flags = 0;
	IPCGIVE(port->port_handle, vcname, 0, &flags, &ipcresult);
	port->port_handle = 0;
	port->port_state = state_pending;
	if (ipcresult) {
		gds__log("IPCGIVE failed with error %ld", ipcresult);
		IPCSHUTDOWN(port->port_handle);
		return FALSE;
	}

	atoh(vcname, sizeof(vcname), infostr);

	HPMYPROGRAM(formal_prog, &ipcresult, &length);
	if (ipcresult || !length)
		strcpy(formal_prog, SERVER_NAME);
	else
		formal_prog[--length] = 0;

/* Create an option array for RPMCREATE.  Passing connection name,
   port protocol and flags to child. */

	INITOPT(rpmopt, 5, &status);
	ADDOPT(rpmopt, 0, RPM_INFOSTR, sizeof(infostr), infostr, &status);
	infolen = sizeof(infostr);
	ADDOPT(rpmopt, 1, RPM_INFOLEN, sizeof(SSHORT), &infolen, &status);
	sprintf(portinfo, "%d", port->port_protocol);
	ADDOPT(rpmopt, 2, RPM_STROPT, RPM_STRLEN, portinfo, &status);
	sprintf(portinfo, "%d", port->port_flags);
	ADDOPT(rpmopt, 3, RPM_STROPT, RPM_STRLEN, portinfo, &status);
	create_flags = 1 << RPM_NOPRIV;
	ADDOPT(rpmopt, 4, RPM_CREATE, sizeof(SSHORT), &create_flags, &status);

	flags = (1 << RPM_IGNORE) | (1 << RPM_1SESSION);

	RPMCREATE(&formal_prog[1], strlen(&formal_prog[1]),
			  port->port_host->str_data, strlen(port->port_host->str_data),
			  name, strlen(name), password, strlen(password),
			  &flags, rpmopt, pd, &ipcresult);

	if (ipcresult) {
		gds__log("RPMCREATE failed with error %ld", ipcresult);
		flags = 0;
		IPCGET(vcname, IPC_GIVELEN, &flags, &port->port_handle, &ipcresult);
		IPCSHUTDOWN(port->port_handle);
		port->port_handle = 0;
		return FALSE;
	}

	return TRUE;
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
 *	On XL, neither multi-threaded nor multi-client. Parent is merely
 *	the main port of a connection when alloc'ing an async port.
 *
 **************************************/
	PORT port;
	UCHAR buffer[MAXHOSTLEN];

	port = (PORT) ALLOCV(type_port, BUFFER_SIZE * 2);
	port->port_type = port_ipc;
	port->port_state = state_pending;

	if (parent) {
		port->port_parent = parent;
		port->port_host = REMOTE_make_string(parent->port_host->str_data);
		port->port_connection =
			REMOTE_make_string(parent->port_host->str_data);
		port->port_version =
			REMOTE_make_string(parent->port_version->str_data);
		port->port_server = parent->port_server;
	}
	else {
		ISC_get_host(buffer, sizeof(buffer));
		port->port_host = REMOTE_make_string(buffer);
		port->port_connection = REMOTE_make_string(buffer);
		sprintf(buffer, "tcp (%s)", port->port_host->str_data);
		port->port_version = REMOTE_make_string(buffer);
	}

	port->port_accept = accept_connection;
	port->port_disconnect = disconnect;
	port->port_receive_packet = receive;
	port->port_send_packet = send;
	port->port_send_partial = send;
	port->port_connect = aux_connect;
	port->port_request = aux_request;
	port->port_buff_size = BUFFER_SIZE;

	xdripc_create(&port->port_send, port,
				  &port->port_buffer[BUFFER_SIZE], BUFFER_SIZE, XDR_ENCODE);

	xdripc_create(&port->port_receive, port, port->port_buffer, 0,
				  XDR_DECODE);

	return port;
}


static void atoh( SCHAR * vcname, SSHORT length, SCHAR * convert)
{
/**************************************
 *
 *	a t o h 
 *
 **************************************
 *
 * Functional description
 *	Convert ascii string to hexadecimal string
 *
 **************************************/
	SCHAR *p, *q, *end;
	UCHAR x;

	for (p = vcname, end = p + length, q = convert; p < end;) {
		x = *p++;
		*q++ = hex[x >> 4];
		*q++ = hex[x & 15];
	}
}


static PORT aux_connect( PORT port, PACKET * packet, XDR_INT(*ast) ())
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
	PORT async_port;
	P_RESP *response;
	USHORT event_socket;
	struct sockaddr_in address;

/* If this is a server, we've got an auxiliary connection.  Accept it */

	if (port->port_server_flags)
		return connect(port->port_async ? port->port_async : port, port);

/* client side */

	async_port = alloc_port(port);
	async_port->port_flags |= PORT_async;
	response = &packet->p_resp;

/* Set up a new socket */

	bzero((SCHAR *) & address, sizeof(address));
	bcopy(response->p_resp_data.cstr_address, &address,
		  response->p_resp_data.cstr_length);

/* get socket name as protocol */

	event_socket = address.sin_port;

	if (!client_connect
		(port->port_connection->str_data, event_socket, INFINITETIME,
		 async_port, 0))
		return NULL;

	if (!async_list) {
		if (!ISC_comm_init(port->port_status_vector, FALSE)) {
			disconnect(async_port);
			return NULL;
		}
		ISC_signal(0, ipc_handler, (void *) TRUE);
	}

	async_port->port_ast = ast;
	async_port->port_next = async_list;
	port->port_async = async_list = async_port;

	return async_port;
}


static PORT aux_request( PORT port, PACKET * packet)
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
 *
 **************************************/
	PORT parent, async_port;
	P_RESP *response;
	SLONG l, calldesc, ipcresult;
	SSHORT timeout;
	TEXT *p;
	SCHAR socket_name[16];
	struct sockaddr_in address;

/* Set up a new socket */

	async_port = alloc_port(port);
	async_port->port_flags |= PORT_async;
	async_port->port_server_flags = port->port_server_flags;
	response = &packet->p_resp;

	IPCCREATE(IPC_CALL_SOCKET, IPC_TCP,,, &calldesc, &ipcresult);
	p = "IPCCREATE";
	if (!ipcresult) {
		timeout = RECVTIME;
		IPCCONTROL(calldesc, IPCCTL_TIMEOUT, &timeout, sizeof(timeout),,,,
				   &ipcresult);
		p = "IPCCONTROL";
	}

	if (!ipcresult) {
		async_port->port_channel = calldesc;
		bzero((SCHAR *) & address, sizeof(address));
		l = sizeof(address.sin_port);

		/* get our port number so they can call us back */

		GET_PRIVMODE
			IPCCONTROL(calldesc, IPCCTL_ADDRESS,,, &address.sin_port, &l,,
					   &ipcresult);
	GET_USERMODE}

	if (ipcresult) {
		disconnect(async_port);
		ipc_error(port, p, ipcresult);
		return NULL;
	}

	response->p_resp_data.cstr_address = (UCHAR *) & response->p_resp_blob_id;
	response->p_resp_data.cstr_length = sizeof(response->p_resp_blob_id);
	bcopy(&address, response->p_resp_data.cstr_address,
		  response->p_resp_data.cstr_length);

	port->port_async = async_port;
	return async_port;
}


static void bcopy( SCHAR * from, SCHAR * to, int length)
{
/**************************************
 *
 *	b c o p y
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
		while (--length);
}


static void bzero( SCHAR * address, int length)
{
/**************************************
 *
 *	b z e r o
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
		while (--length);
}


static int check_proxy(
					   PORT port,
					   TEXT * host_name, TEXT * user_name, TEXT * user_passwd)
{
/**************************************
 *
 *	c h e c k _ p r o x y
 *
 **************************************
 *
 * Functional description
 *	Lookup <host_name, user_name> in proxy file.  If found,
 *	change user_name and record passwd.
 *
 * NB. Format of proxy file is source_node:source_user target_user target_passwd
 *     (source=remote, target=local)
 *     
 **************************************/
	IB_FILE *proxy;
	TEXT *p, source_host[MAXHOSTLEN], proxy_file[MAXPATHLEN], line[128];
	TEXT source_user[MAXLOGINLEN], target_user[MAXLOGINLEN],
		target_passwd[MAXLOGINLEN];
	int result, c;
	SLONG length;
	STR string;

	if (!(proxy = ib_fopen(PROXY_FILE, "r Tm")))
		return FALSE;

/* Read lines, scan, and compare */

	result = FALSE;

	for (;;) {
		for (p = line; (c = ib_getc(proxy)) && c != EOF && c != '\n';)
			*p++ = c;
		*p = 0;
		if (sscanf(line, " %[^:]:%s%s%s", source_host, source_user,
				   target_user, target_passwd) >= 4)
			if ((!strcmp(source_host, host_name) ||
				 !strcmp(source_host, "*")) &&
				(!strcmp(source_user, user_name) ||
				 !strcmp(source_user, "*"))) {
				ALLR_free(port->port_user_name);
				length = strlen(target_user);
				port->port_user_name = string =
					(STR) ALLOCV(type_str, (int) length);
				string->str_length = length;
				strncpy(string->str_data, target_user, length);
				strcpy(user_name, target_user);
				strcpy(user_passwd, target_passwd);
				result = TRUE;
				break;
			}
		if (c == EOF)
			break;
	}

	ib_fclose(proxy);

	return result;
}


static PORT client_connect(
						   TEXT * name,
						   USHORT gds_socket,
						   SSHORT timeout, PORT port, PACKET * packet)
{
/**************************************
 *
 *	c l i e n t _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	We're a client in search of a server. If we're successful, return port.
 *
 **************************************/
	SLONG servprot, servtype, vcdesc, destdesc, ipcresult;
	SSHORT packetlen, status;
	SSHORT maxopt[OPT_SIZE(2, 2 * sizeof(SSHORT))];
	TEXT *p;

	packetlen = MAX_PACKETLEN;
	INITOPT(maxopt, 2, &status);
	ADDOPT(maxopt, 0, IPCOPT_MAX_SEND_SIZE, sizeof(packetlen), &packetlen,
		   &status);
	ADDOPT(maxopt, 1, IPCOPT_MAX_RECV_SIZE, sizeof(packetlen), &packetlen,
		   &status);

	GET_PRIVMODE
		IPCDEST(IPC_CALL_SOCKET, name, strlen(name), IPC_TCP, &gds_socket,
				sizeof(gds_socket),,, &destdesc, &ipcresult);
	GET_USERMODE p = "IPCDEST";

	if (!ipcresult) {
		GET_PRIVMODE IPCCONNECT(, destdesc,, maxopt, &vcdesc, &ipcresult);
		GET_USERMODE p = "IPCCONNECT";
	}

	if (!ipcresult) {
		port->port_handle = (HANDLE) vcdesc;
		IPCCONTROL(vcdesc, IPCCTL_TIMEOUT, &timeout, sizeof(timeout),,,,
				   &ipcresult);
		p = "IPCCONTROL";
	}

/* Complete connection sequence */

	if (!ipcresult) {
		IPCRECV(vcdesc,,,,, &ipcresult);
		p = "IPCRECV";
	}

/* Activate no wait input so we don't block asynchronous signals */

	if (!ipcresult) {
		port->port_state = state_active;
		IPCCONTROL(vcdesc, IPCCTL_NOWAIT_RECV,,,,,, &ipcresult);
		p = "IPCCONTROL";
	}

	if (ipcresult) {
		ipc_error(port, p, ipcresult);
		disconnect(port);
		return NULL;
	}

	if (packet)
		send(port, packet);

	return port;
}


static PORT connect( PORT port, PORT parent)
{
/**************************************
 *                                                            
 *	c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Establish a remote connection.
 *
 **************************************/
	SLONG vcdesc, ipcresult;
	SSHORT status, timeout, packetlen,
		maxopt[OPT_SIZE(2, 2 * sizeof(SSHORT))];
	TEXT *p;

	packetlen = MAX_PACKETLEN;
	INITOPT(maxopt, 2, &status);
	ADDOPT(maxopt, 0, IPCOPT_MAX_SEND_SIZE, sizeof(packetlen), &packetlen,
		   &status);
	ADDOPT(maxopt, 1, IPCOPT_MAX_RECV_SIZE, sizeof(packetlen), &packetlen,
		   &status);

	IPCRECVCN(port->port_channel, &vcdesc,, maxopt, &ipcresult);
	p = "IPCRECVCN";
	if (!ipcresult) {
		timeout = INFINITETIME;
		IPCCONTROL(vcdesc, IPCCTL_TIMEOUT, &timeout, sizeof(timeout),,,,
				   &ipcresult);
		if (!ipcresult)
			IPCCONTROL(vcdesc, IPCCTL_NOWAIT_RECV,,,,,, &ipcresult);
		p = "IPCCONTROL";
	}

	if (ipcresult) {
		if (parent) {
			disconnect(port);
			parent->port_async = NULL;
			ipc_error(parent, p, ipcresult);
		}
		else
			ipc_error(port, p, ipcresult);
		return NULL;
	}

	port->port_handle = (HANDLE) vcdesc;
	port->port_state = state_active;
	port->port_flags = 0;
	port->port_protocol = 0;

	if (parent) {
		port->port_flags |= PORT_async;
		IPCSHUTDOWN(port->port_channel);
		port->port_channel = 0;
		strcpy(port->port_version->str_data, parent->port_version->str_data);
	}
	else
		sprintf(port->port_version->str_data, "tcp (%s)",
				port->port_host->str_data);

	return parent ? parent : port;
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
 *	Break a remote connection. If there's an
 *	auxiliary port (for async i/o), break that one
 *	first. If this is an auxiliary	port, and it's
 *	the last one out - cancel the async i/o handler.
 *
 **************************************/
	SLONG ipcresult;
	PORT *ports;

	if (port->port_async)
		disconnect(port->port_async);

	if (port->port_flags & PORT_async) {
		for (ports = &async_list; *ports; ports = &(*ports)->port_next)
			if (*ports == port) {
				*ports = port->port_next;
				break;
			}
		if (!async_list)
			ISC_signal_cancel(0, ipc_handler, (void *) TRUE);
	}

	if (port->port_handle)
		IPCSHUTDOWN(port->port_handle,,, &ipcresult);

	if (port->port_channel)
		IPCSHUTDOWN(port->port_channel,,, &ipcresult);

	cleanup_port(port);
	return;
}
^L static void cleanup_port( PORT port)
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

	if (port->port_object_vector)
		ALLR_free((UCHAR *) port->port_object_vector);

#ifdef DEBUG_XDR_MEMORY
	if (port->port_packet_vector)
		ALLR_free((UCHAR *) port->port_packet_vector);
#endif

	if (port->port_host)
		ALLR_free((UCHAR *) port->port_host);

	ALLR_release((UCHAR *) port);
	return;
}


static void htoa( SCHAR * convert, SCHAR * vcname, SSHORT length)
{
/**************************************
 *
 *	h t o a 
 *
 **************************************
 *
 * Functional description
 *	Convert hexadecimal string to ascii string
 *
 **************************************/
	SCHAR *p, *q, *end;

	for (p = vcname, end = p + length, q = convert; p < end;) {
		*p = ((*q >= '0' && *q <= '9') ? *q++ - '0' : *q++ - 'A' + 10) << 4;
		*p++ |= (*q >= '0' && *q <= '9') ? *q++ - '0' : *q++ - 'A' + 10;
	}
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

	if (!xdr_protocol(&main_port->port_receive, packet))
		return NULL;

	return main_port;
}


static int send( PORT port, PACKET * packet)
{
/**************************************
 *
 *	s e n d 
 *
 **************************************
 *
 * Functional description
 *	Send a packet across a port to another process.
 *
 **************************************/

/* if it's the odd case of responding to a connection request,
   just return TRUE. */

	if (!port->port_handle && (packet->p_operation == op_accept ||
							   packet->p_operation == op_reject))
		return TRUE;

	if (!xdr_protocol(&port->port_send, packet))
		return FALSE;

	return ipc_write(&port->port_send);
}


static PORT server_connect( USHORT gds_socket, PORT port, int flag)
{
/**************************************
 *
 *	s e r v e r _ c o n n e c t 
 *
 **************************************
 *
 * Functional description
 *	Set up a known port to receive service requests. In the
 *	XL-land of NetIPC, this means a call descriptor at an
 *	INET address (for both XL-XL and INET-XL requests).
 *
 **************************************/
	SLONG calldesc, ipcresult;
	SSHORT status, timeout, addressopt[OPT_SIZE(1, sizeof(SSHORT))];
	TEXT *p;

/* Set flags for server port. Clear handle so we know in receive that we haven't
   finished establishing the NetIPC connection. */

	port->port_server_flags |= SRVR_server | flag;
	port->port_handle = 0;

	INITOPT(addressopt, 1, &status);
	ADDOPT(addressopt, 0, IPCOPT_ADDRESS, sizeof(gds_socket), &gds_socket,
		   &status);

	GET_PRIVMODE
		IPCCREATE(IPC_CALL_SOCKET, IPC_TCP,, addressopt, &calldesc,
				  &ipcresult);
	GET_USERMODE p = "IPCCREATE";

	if (!ipcresult) {
		port->port_channel = (HANDLE) calldesc;
		timeout = INFINITETIME;
		IPCCONTROL(calldesc, IPCCTL_TIMEOUT, &timeout, sizeof(timeout),,,,
				   &ipcresult);
		p = "IPCCONTROL";
	}

	if (ipcresult) {
		ipc_error(port, p, ipcresult);
		disconnect(port);
		return NULL;
	}

	return port;
}


static int xdripc_create(
						 XDR * xdrs,
						 PORT port,
						 UCHAR * buffer, USHORT length, enum xdr_op x_op)
{
/**************************************
 *
 *	x d r i p c _ c r e a t e
 *
 **************************************
 *
 * Functional description
 *	Initialize an XDR stream.
 *
 **************************************/

	xdrs->x_public = (caddr_t) port;
	xdrs->x_base = xdrs->x_private = (SCHAR *) buffer;
	xdrs->x_handy = length;
	xdrs->x_ops = &ipc_ops;
	xdrs->x_op = x_op;

	return TRUE;
}


static XDR_INT ipc_destroy( XDR * xdrs)
{
/**************************************
 *
 *	i p c _ d e s t r o y
 *
 **************************************
 *
 * Functional description
 *	Destroy a stream.  A no-op.
 *
 **************************************/
}


static int ipc_error( PORT port, TEXT * operation, int status)
{
/**************************************
 *
 *	i p c _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	An I/O error has occurred.  If a status vector is present,
 *	generate an error return.  In any case, return NULL, which
 *	is used to indicate an error.
 *
 **************************************/
	RDB rdb;
	TEXT **tp;
	STATUS *status_vector;

	port->port_flags |= PORT_broken;
	port->port_state = state_broken;

	if (rdb = port->port_context)
		status_vector = rdb->rdb_status_vector;
	else
		status_vector = port->port_status_vector;

	if (status_vector) {
		*status_vector++ = gds_arg_gds;
		*status_vector++ = gds__io_error;
		*status_vector++ = gds_arg_string;
		*status_vector++ = (STATUS) operation;
		*status_vector++ = gds_arg_string;
		tp = (TEXT **) status_vector;
		*tp++ = port->port_connection->str_data;
		status_vector = (STATUS *) tp;
		if (status) {
			*status_vector++ = gds_arg_mpexl_ipc;
			*status_vector++ = (STATUS) status;
		}
		*status_vector++ = 0;
	}

	return 0;
}


static bool_t ipc_getbytes( XDR * xdrs, SCHAR * buff, int count)
{
/**************************************
 *
 *	i p c _ g e t b y t e s
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
			if (!ipc_read(xdrs))
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
		if (!xdrs->x_handy && !ipc_read(xdrs))
			return FALSE;
		*buff++ = *xdrs->x_private++;
		--xdrs->x_handy;
	}

	return TRUE;
}


static bool_t ipc_getlong( XDR * xdrs, SLONG * lp)
{
/**************************************
 *
 *	i p c _ g e t l o n g
 *
 **************************************
 *
 * Functional description
 *	Fetch a longword into a memory stream if it fits.
 *
 **************************************/
	SLONG l;

	if (!(*xdrs->x_ops->x_getbytes) (xdrs, &l, 4))
		return FALSE;

	*lp = ntohl(l);

	return TRUE;
}


static u_int ipc_getpostn( XDR * xdrs)
{
/**************************************
 *
 *	i p c _ g e t p o s t n
 *
 **************************************
 *
 * Functional description
 *	Get the current position (which is also current length) from stream.
 *
 **************************************/

	return xdrs->x_private - xdrs->x_base;
}


static int ipc_handler( SSHORT fnum, void *async_flag)
{
/**************************************
 *
 *	i p c _  h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Async i/o signal went off - if it's what
 *	we've been waiting for, go for it!
 *
 **************************************/
	PORT port;

	for (port = async_list; port; port = port->port_next)
		if (fnum == port->port_handle) {
			(*port->port_ast) (port);
			return TRUE;
		}

	return FALSE;
}


static caddr_t ipc_inline( XDR * xdrs, u_int bytecount)
{
/**************************************
 *
 *	i p c _  i n l i n e
 *
 **************************************
 *
 * Functional description
 *	Return a pointer to somewhere in the buffer.
 *
 **************************************/

	if (bytecount > xdrs->x_handy)
		return FALSE;

	return xdrs->x_base + bytecount;
}


static bool_t ipc_putbytes( XDR * xdrs, SCHAR * buff, int count)
{
/**************************************
 *
 *	i p c _ p u t b y t e s
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
			if (!ipc_write(xdrs))
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
		if (xdrs->x_handy <= 0 && !ipc_write(xdrs))
			return FALSE;
		--xdrs->x_handy;
		*xdrs->x_private++ = *buff++;
	}

	return TRUE;
}


static bool_t ipc_putlong( XDR * xdrs, SLONG * lp)
{
/**************************************
 *
 *	i p c _ p u t l o n g
 *
 **************************************
 *
 * Functional description
 *	Fetch a longword into a memory stream if it fits.
 *
 **************************************/
	SLONG l;

	l = htonl(*lp);
	return (*xdrs->x_ops->x_putbytes) (xdrs, &l, 4);
}


static bool_t ipc_read( XDR * xdrs)
{
/**************************************
 *
 *	i p c _ r e a d
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
		if (!packet_receive(port, p, length, &length)) {
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
	xdrs->x_handy = (SCHAR *) p - xdrs->x_base;
	xdrs->x_private = xdrs->x_base;

	return TRUE;
}


static bool_t ipc_setpostn( XDR * xdrs, u_int bytecount)
{
/**************************************
 *
 *	i p c _ s e t p o s t n
 *
 **************************************
 *
 * Functional description
 *	Set the current position (which is also current length) from stream.
 *
 **************************************/

	if (bytecount > xdrs->x_handy)
		return FALSE;

	xdrs->x_private = xdrs->x_base + bytecount;

	return TRUE;
}


static bool_t ipc_write( XDR * xdrs, bool_t end_flag)
{
/**************************************
 *
 *	i p c _ w r i t e
 *
 **************************************
 *
 * Functional description
 *	Write a buffer fulll of data.  If the end_flag isn't set, indicate
 *	that the buffer is a fragment, and reset the XDR for another buffer
 *	load.
 *
 **************************************/
	SCHAR aux_buffer[BUFFER_SIZE], *p;
	SCHAR buffer[BUFFER_SIZE];
	PORT port;
	SSHORT l, length, l2;

/* Encode the data portion of the packet */

	port = (PORT) xdrs->x_public;
	p = xdrs->x_base;
	length = xdrs->x_private - p;

/* Send data in manageable hunks.  If a packet is partial, indicate
   that with a negative length.  A positive length marks the end. */

	p = xdrs->x_base;

	while (length) {
		port->port_misc1 = (port->port_misc1 + 1) % MAX_SEQUENCE;
		l = MIN(length, MAX_DATA);
		length -= l;
		if (!packet_send(port, p, (length) ? -l : l))
			return FALSE;
		p += l;
	}

	xdrs->x_private = xdrs->x_base;
	xdrs->x_handy = BUFFER_SIZE;

	return TRUE;

#ifdef PIGGYBACK
/* If the other end has not piggy-backed the next packet, we're done. */

	if (!l2)
		return TRUE;

/* We've got a piggy-backed response.  If the packet is partial,
   send an ACK for part we did receive. */

	p = aux_buffer;

	while (l2 < 0) {
		if (!packet_send(port, 0, 0))
			return FALSE;
		p -= l2;
		length = aux_buffer + sizeof(aux_buffer) - p;
		if (!packet_receive(port, p, length, &l2)) {
			p += l2;
			continue;
		}
	}

	length = p - aux_buffer + l2;

/* Now we're got a encode glump ready to stuff into the read buffer.
   Unfortunately, if we just add it to the read buffer, we will shortly
   overflow the buffer.  To avoid this, "scrumpf down" the active bits
   in the read buffer, then add out stuff at the end. */

	xdrs = &port->port_receive;
	p = xdrs->x_base;

	if (xdrs->x_handy && p != xdrs->x_private)
		memmove(p, xdrs->x_private, xdrs->x_handy);

	p += xdrs->x_handy;

	xdrs->x_private = xdrs->x_base;
/*
xdrs->x_handy += JAP_decode (aux_buffer, length, p);
*/
	port->port_flags |= PORT_pend_ack;

	return TRUE;
#endif
}


static void packet_print( TEXT * string, UCHAR * packet, int length)
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
	int sum, l;

	sum = 0;
	if (l = length)
		do {
			ib_printf("%d", *packet);
			sum += *packet++;
		}
		while (--l);

	ib_printf("\n%s\t: length = %d, checksum = %d\n", string, length, sum);
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
	SLONG iodesc, buff_len, ipcresult, flags;
	SSHORT packetlen;
	PORT async_port;

/* if this is the main mpexl server, we have to finish the NetIPC connection.
   (In the debug case, handle already loaded - just continue with receive) */

	if (!port->port_handle)
		if (!connect(port, NULL))
			return NULL;

/* If there's an event port, and we haven't posted a preview yet, do so now. */

	async_port = port->port_async;
	if (async_port && !(async_port->port_flags & PORT_pend_rec)) {
		flags = IPC_PREVIEWFLAG | IPC_LOCALFLAGS;
		buff_len = BUFFER_SIZE;
		IPCRECV(async_port->port_handle, (SCHAR ^) async_port->port_buffer,
				&buff_len, &flags,, &ipcresult);
		if (ipcresult)
			return ipc_error(port, "IPCRECV", ipcresult);
		async_port->port_flags |= PORT_pend_rec;
	}

/* onto the real business at hand - post a nowait receive and wait it out */
/* At this point, port may be either a main or async one. If it's a main
   port but there are async ports, wait on everything. Otherwise, we only
   need to complete i/o on the port we were called with. */

	buff_len = buffer_length;
	flags = IPC_LOCALFLAGS;
	IPCRECV(port->port_handle, (SCHAR ^) buffer, &buff_len, &flags,,
			&ipcresult);
	if (ipcresult)
		return ipc_error(port, "IPCRECV", ipcresult);
	port->port_flags |= PORT_pend_rec;

	iodesc = 0;
	while (iodesc != port->port_handle) {
		if (!(port->port_flags & PORT_async) && async_list)
			iodesc = IOWAIT(0,, &packetlen,);
		else
			iodesc = IOWAIT(port->port_handle,, &packetlen,);
		if (!iodesc)
			return ipc_error(port, "IOWAIT", 0);
		IPCCHECK(iodesc, &ipcresult);
		if (ipcresult ||
			(iodesc != port->port_handle
			 && !ipc_handler(iodesc, (void *) FALSE))) return ipc_error(port,
																		"IOWAIT",
																		ipcresult);
	}

	port->port_flags &= ~PORT_pend_rec;

	if (packetlen <= 0)
		return ipc_error(port, "read end_of_file", 0);

	*length = packetlen;

#ifdef DEBUG
	if (INET_trace)
		packet_print("recv", buffer, buff_len);
#endif

	if (port->port_flags & PORT_async) {
		flags = IPC_PREVIEWFLAG | IPC_LOCALFLAGS;
		buff_len = BUFFER_SIZE;
		IPCRECV(port->port_handle, (SCHAR ^) port->port_buffer, &buff_len,
				&flags,, &ipcresult);
		if (ipcresult)
			return ipc_error(port, "IPCRECV", ipcresult);

		port->port_flags |= PORT_pend_rec;
	}

	return TRUE;
}


static int packet_send( PORT port, UCHAR * data, SSHORT length)
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
	SLONG urgentflag, ipcresult;
	SSHORT status, sendopt[OPT_SIZE(1, sizeof(SLONG))];

	if (port->port_flags & PORT_async) {
		urgentflag = IPCSND_URGENTFLAG;
		INITOPT(sendopt, 1, &status);
		ADDOPT(sendopt, 0, IPCOPT_PROTOCOL, sizeof(urgentflag), &urgentflag,
			   &status);
		IPCSEND(port->port_handle, (SCHAR ^) data, length,, sendopt,
				&ipcresult);
	}
	else
		IPCSEND(port->port_handle, (SCHAR ^) data, length,,, &ipcresult);

	if (ipcresult)
		return ipc_error(port, "IPCSEND", ipcresult);

#ifdef DEBUG
	if (INET_trace)
		packet_print("send", data, length);
#endif

	port->port_flags &= ~PORT_pend_ack;

	return TRUE;
}


static void zap_packet( PACKET * packet)
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
	SLONG *p;
	UCHAR *pc;
	USHORT l;

	p = (SLONG *) packet;
	l = sizeof(PACKET) >> 2;
	do
		*p++ = 0;
	while (--l);

	if (l = sizeof(PACKET) & 3) {
		pc = (UCHAR *) p;
		do
			*pc++ = 0;
		while (--l);
	}
}
