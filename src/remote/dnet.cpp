/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		dnet.c
 *	DESCRIPTION:	DECnet Communications module for ULTRIX
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

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdnet/dn.h>
#include <netdnet/dnetdb.h>
#include "../jrd/ib_stdio.h"
#include <signal.h>
#include "../include/jrd/gds.h"
#include <errno.h>
#include <pwd.h>
#include "../remote/remote.h"
#include "../remote/dnet_proto.h"
#include "../remote/remot_proto.h"
#include "../jrd/isc_proto.h"

#define OBJECT_NUMBER	128
#define OBJECT_NAME	"#128"

#define MAX_DATA	1450
#define BUFFER_SIZE	MAX_DATA
#define MAX_SEQUENCE	256

extern int errno;

static int accept_connection(PORT, P_CNCT *);
static PORT alloc_port(PORT);
static PORT aux_connect(PORT, PACKET *, XDR_INT(*)());
static PORT aux_request(PORT, PACKET *);
static void cleanup_port(PORT);
static int check_host(PORT);
static void disconnect(PORT);
static XDR_INT dnet_destroy(XDR *);
static int dnet_error(PORT, TEXT *, int);
static bool_t dnet_getbytes(XDR *, SCHAR *, int);
static bool_t dnet_getlong(XDR *, SLONG *);
static u_int dnet_getpostn(XDR *);
static void dnet_handler(PORT);
static caddr_t dnet_inline(XDR *, u_int);
static bool_t dnet_putbytes(XDR *, SCHAR *, int);
static bool_t dnet_putlong(XDR *, SLONG *);
static bool_t dnet_read(XDR *);
static bool_t dnet_setpostn(XDR *, u_int);
static bool_t dnet_write(XDR *, bool_t);
static int packet_receive(PORT, UCHAR *, SSHORT, SSHORT *);
static int packet_send(PORT, UCHAR *, SSHORT);
static PORT receive(PORT, PACKET *);
static int send_full(PORT, PACKET *);
static int send_partial(PORT, PACKET *);
static int xdrdnet_create(XDR *, PORT, UCHAR *, USHORT, enum xdr_op);
static int xdrdnet_endofrecord(XDR *, bool_t);

static xdr_t::xdr_ops dnet_ops = {
	dnet_getlong,
	dnet_putlong,
	dnet_getbytes,
	dnet_putbytes,
	dnet_getpostn,
	dnet_setpostn,
	dnet_inline,
	dnet_destroy
};

static int request_count;


PORT DNET_analyze(
				  UCHAR * file_name,
				  SSHORT * file_length,
				  STATUS * status_vector, SSHORT uv_flag)
{
/**************************************
 *
 *	D N E T _ a n a l y z e
 *
 **************************************
 *
 * Functional description
 *	Determine whether the file name has a "external!name".  If so,
 *	establish an external connection to the node.
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
	struct passwd *pw;
	SSHORT uid, length, user_length;
	UCHAR node_name[ND_MAXNODE + 1 + OB_MAXNAME], *p, *q, user_id[128];
	STATUS local_status[20];
	p_cnct::p_cnct_repeat * protocol;

/* Analyze the file name to see if a remote connection is required.  If not,
   quietly (sic) return. */

/* Pick up node if, if present */

	p = node_name;
	q = file_name;
	length = *file_length;

	while (q[0] != ':' || q[1] != ':') {
		if (--length < 0 || p >= node_name + sizeof(node_name) - 1)
			return NULL;
		*p++ = *q++;
	}

	q += 2;
	*p = 0;
	length -= 2;
	*file_length = length;

	if (length)
		do
			*file_name++ = *q++;
		while (--length);

	*file_name = 0;

/* We need to establish a connection to a remote server.  Allocate the necessary
   blocks and get ready to go. */

	rdb = (RDB) ALLOC(type_rdb);
	packet = &rdb->rdb_packet;

/* Pick up some user identification information */

	user_id[0] = CNCT_user;
	p = user_id + 2;
	ISC_get_user(p, 0, 0, 0, 0, 0, 0);
	user_id[1] = user_length = strlen(p);
	user_length += 2;

	for (; *p; p++)
		if (*p >= 'A' && *p <= 'Z')
			*p = *p - 'A' + 'a';

/* Establish connection to server */

	connect = &packet->p_cnct;
	packet->p_operation = op_connect;
	connect->p_cnct_operation = op_attach;
	connect->p_cnct_cversion = CONNECT_VERSION2;
	connect->p_cnct_client = ARCHITECTURE;
	connect->p_cnct_file.cstr_length = *file_length;
	connect->p_cnct_file.cstr_address = file_name;

/* Note: prior to V3.1E a recievers could not in truth handle more
   then 5 protocol descriptions, so we try them in chunks of 5 or less */

#ifdef SCROLLABLE_CURSORS
	connect->p_cnct_count = 4;
#else
	connect->p_cnct_count = 2;
#endif
	connect->p_cnct_user_id.cstr_length = user_length;
	connect->p_cnct_user_id.cstr_address = user_id;

	protocol = connect->p_cnct_versions;

	protocol->p_cnct_version = PROTOCOL_VERSION8;
	protocol->p_cnct_architecture = arch_generic;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = ptype_out_of_band;
	protocol->p_cnct_weight = 2;

	++protocol;

	protocol->p_cnct_version = PROTOCOL_VERSION8;
	protocol->p_cnct_architecture = ARCHITECTURE;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = ptype_out_of_band;
	protocol->p_cnct_weight = 3;

#ifdef SCROLLABLE_CURSORS
	++protocol;

	protocol->p_cnct_version = PROTOCOL_SCROLLABLE_CURSORS;
	protocol->p_cnct_architecture = arch_generic;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = ptype_out_of_band;
	protocol->p_cnct_weight = 4;

	++protocol;

	protocol->p_cnct_version = PROTOCOL_SCROLLABLE_CURSORS;
	protocol->p_cnct_architecture = ARCHITECTURE;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = ptype_out_of_band;
	protocol->p_cnct_weight = 5;
#endif

/* If we can't talk to a server, punt.  Let somebody else generate
   an error. */

	if (!(port = DNET_connect(node_name, packet, status_vector, FALSE))) {
		ALLR_release(rdb);
		return NULL;
	}

/* Get response packet from server. */

	rdb->rdb_port = port;
	port->port_context = rdb;
	port->receive(packet);

	if (packet->p_operation == op_reject) {
		disconnect(port);
		packet->p_operation = op_connect;
		connect->p_cnct_operation = op_attach;
		connect->p_cnct_cversion = CONNECT_VERSION2;
		connect->p_cnct_client = ARCHITECTURE;
		connect->p_cnct_file.cstr_length = *file_length;
		connect->p_cnct_file.cstr_address = file_name;

		/* try again with next set of known protocols */

		connect->p_cnct_count = 4;
		connect->p_cnct_user_id.cstr_length = user_length;
		connect->p_cnct_user_id.cstr_address = user_id;

		protocol = connect->p_cnct_versions;

		protocol->p_cnct_version = PROTOCOL_VERSION6;
		protocol->p_cnct_architecture = arch_generic;
		protocol->p_cnct_min_type = ptype_rpc;
		protocol->p_cnct_max_type = ptype_out_of_band;
		protocol->p_cnct_weight = 2;

		++protocol;

		protocol->p_cnct_version = PROTOCOL_VERSION6;
		protocol->p_cnct_architecture = ARCHITECTURE;
		protocol->p_cnct_min_type = ptype_rpc;
		protocol->p_cnct_max_type = ptype_out_of_band;
		protocol->p_cnct_weight = 3;

		++protocol;

		protocol->p_cnct_version = PROTOCOL_VERSION7;
		protocol->p_cnct_architecture = arch_generic;
		protocol->p_cnct_min_type = ptype_rpc;
		protocol->p_cnct_max_type = ptype_out_of_band;
		protocol->p_cnct_weight = 4;

		++protocol;

		protocol->p_cnct_version = PROTOCOL_VERSION7;
		protocol->p_cnct_architecture = ARCHITECTURE;
		protocol->p_cnct_min_type = ptype_rpc;
		protocol->p_cnct_max_type = ptype_out_of_band;
		protocol->p_cnct_weight = 5;

		if (!(port = DNET_connect(node_name, packet, status_vector, FALSE))) {
			ALLR_release(rdb);
			return NULL;
		}

		/* Get response packet from server. */

		rdb->rdb_port = port;
		port->port_context = rdb;
		port->receive(packet);
	}

	if (packet->p_operation == op_reject) {
		disconnect(port);
		packet->p_operation = op_connect;
		connect->p_cnct_operation = op_attach;
		connect->p_cnct_cversion = CONNECT_VERSION2;
		connect->p_cnct_client = ARCHITECTURE;
		connect->p_cnct_file.cstr_length = *file_length;
		connect->p_cnct_file.cstr_address = file_name;

		/* try again with next set of known protocols */

		connect->p_cnct_count = 4;
		connect->p_cnct_user_id.cstr_length = user_length;
		connect->p_cnct_user_id.cstr_address = user_id;

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

		if (!(port = DNET_connect(node_name, packet, status_vector, FALSE))) {
			ALLR_release(rdb);
			return NULL;
		}

		/* Get response packet from server. */

		rdb->rdb_port = port;
		port->port_context = rdb;
		port->receive(packet);
	}

	if (packet->p_operation != op_accept) {
		ib_fprintf(ib_stderr, "remote server refused connection\n");
		return NULL;
	}

	port->port_protocol = packet->p_acpt.p_acpt_version;

	if (packet->p_acpt.p_acpt_architecture == ARCHITECTURE)
		port->port_flags |= PORT_symmetric;

	if (packet->p_acpt.p_acpt_type == ptype_rpc)
		port->port_flags |= PORT_rpc;

	if (packet->p_acpt.p_acpt_type != ptype_out_of_band)
		port->port_flags |= PORT_no_oob;

	return port;
}


PORT DNET_connect(UCHAR * name,
				  PACKET * packet, STATUS * status_vector, int flag)
{
/**************************************
 *
 *	D N E T _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Establish half of a communication link.  If a connect packet is given,
 *	the connection is on behalf of a remote interface.  Other the connect
 *	is is for a server process.
 *
 **************************************/
	XDR xdr;
	int n, l, object_number;
	PORT port;
	struct sockaddr_dn address;
	struct nodeent *node;
	TEXT *p, *object_name, node_name[ND_MAXNODE + 1 + OB_MAXNAME];

/* If given a debug protocol, use it */

	if (name) {
		strcpy(node_name, name);
		for (p = node_name; *p;)
			if (*p++ == '/') {
				p[-1] = 0;
				name = (UCHAR *) node_name;
				break;
			}
		object_name = (*p) ? p : OBJECT_NAME;
	}

/* Allocate a port block */

	port = alloc_port(NULL);
	port->port_status_vector = status_vector;
	status_vector[0] = gds_arg_gds;
	status_vector[1] = 0;

	if (name && *name) {
		if (port->port_connection)
			ALLR_free(port->port_connection);
		port->port_connection = REMOTE_make_string(name);
	}
	else
		name = (UCHAR *) port->port_host->str_data;

/* Client and server connections diverge here */

	if (packet) {
		port->port_handle = dnet_conn(name, object_name, SOCK_SEQPACKET,
									  NULL, NULL, NULL, NULL);
		if (port->port_handle == -1) {
			dnet_error(port, "dnet_conn", errno);
			return NULL;
		}
		send_full(port, packet);
		return port;
	}

/* Server stuff */

	port->port_handle = socket(AF_DECnet, SOCK_STREAM, 0);
	if (port->port_handle == -1) {
		dnet_error(port, "socket", errno);
		return NULL;
	}

	node = getnodebyname(name);
	if (!node) {
		dnet_error(port, "getnodebyname", errno);
		return NULL;
	}

	bzero((SCHAR *) & address, sizeof(address));
	bcopy(node->n_addr, address.sdn_add.a_addr, node->n_length);
	address.sdn_add.a_len = node->n_length;
	address.sdn_family = node->n_addrtype;
	address.sdn_objnum = object_number;
	strcpy(address.sdn_objname, object_name);
	address.sdn_objnamel = strlen(address.sdn_objname);

	if (bind(port->port_handle, &address, sizeof(address)) < 0) {
		dnet_error(port, "bind", errno);
		return NULL;
	}

	if (listen(port->port_handle, 2) < 0) {
		dnet_error(port, "listen", errno);
		return NULL;
	}

/* We're a server, so wait for a host to show up */

	while (TRUE) {
		l = sizeof(address);
		n = accept(port->port_handle, &address, &l);
		if (n == -1) {
			dnet_error(port, "accept", errno);
			return NULL;
		}

		if (flag || !fork()) {
			if (port->port_handle != (HANDLE) - 1)
				close(port->port_handle);
			port->port_handle = n;
			port->port_server_flags = TRUE;
			return port;
		}
		close(n);
	}
}


PORT DNET_server(int socket)
{
/**************************************
 *
 *	D N E T _ s e r v e r
 *
 **************************************
 *
 * Functional description
 *	We have been spawned by a master server with a connection
 *	established.  Set up port block with the appropriate socket.
 *
 **************************************/
	PORT port;

	port = alloc_port(NULL);
	port->port_server_flags |= SRVR_server;
	port->port_handle = socket;

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
	UCHAR name[32], password[32], *id, *end, *p, *q;
	SLONG length;
	struct passwd *passwd;

/* Default account to "guest" */

	strcpy(name, "guest");
	password[0] = 0;

/* Pick up account and password, if given */

	id = connect->p_cnct_user_id.cstr_address;
	end = id + connect->p_cnct_user_id.cstr_length;

	while (id < end)
		switch (*id++) {
		case CNCT_user:
			p = name;
			if (length = *id++)
				do
					*p++ = *id++;
				while (--length);
			*p = 0;
			break;

		case CNCT_passwd:
			p = password;
			if (length = *id++)
				do
					*p++ = *id++;
				while (--length);
			*p = 0;
			break;

		default:
			p += *p + 1;
		}

/* See if user exists.  If not, reject connection */

	passwd = getpwnam(name);
	if (!check_host(port))
		return FALSE;

/* Change effective user id and default directory */

	setreuid(-1, passwd->pw_uid);
	chdir(passwd->pw_dir);

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
 *
 **************************************/
	SCHAR buffer[256], *p;
	UCHAR *message;
	PORT port;

	port = (PORT) ALLOCV(type_port, BUFFER_SIZE * 2);
	port->port_type = port_decnet;
	port->port_state = state_pending;
	port->port_version = REMOTE_make_string("DECnet");

/* Init file descriptors so we can tell when they are used */

	port->port_handle = (HANDLE) - 1;
	port->port_channel = (int) -1;

	if (!(p = getnodename()))
		p = "?";
	port->port_host = REMOTE_make_string(p);
	port->port_connection = REMOTE_make_string(p);
	sprintf(buffer, "DECnet (%s)", port->port_host->str_data);
	if (port->port_version)
		ALLR_free(port->port_version);
	port->port_version = REMOTE_make_string(buffer);

	if (parent) {
		port->port_parent = parent;
		port->port_next = parent->port_clients;
		parent->port_clients = port;
		port->port_handle = parent->port_handle;
		port->port_server = parent->port_server;
		port->port_server_flags = parent->port_server_flags;
	}

	message = port->port_buffer;
	port->port_accept = accept_connection;
	port->port_disconnect = disconnect;
	port->port_receive_packet = receive;
	port->port_send_packet = send_full;
	port->port_send_partial = send_partial;
	port->port_connect = aux_connect;
	port->port_request = aux_request;
	port->port_buff_size = BUFFER_SIZE;

	xdrdnet_create(&port->port_send, port,
				   &port->port_buffer[BUFFER_SIZE], BUFFER_SIZE, XDR_ENCODE);

	xdrdnet_create(&port->port_receive, port, port->port_buffer, 0,
				   XDR_DECODE);

	return port;
}


static PORT aux_connect(PORT port, PACKET* packet, XDR_INT (*ast)())
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
	int n, l, pid;
	TEXT *p, *end, object_name[OB_MAXNAME];
	PORT new_port;
	struct sockaddr_dn address;

/* If this is a server, we're got an auxiliary connection.  Accept it */

	if (port->port_server_flags) {
		l = sizeof(address);
		n = accept(port->port_channel, &address, &l);
		if (n == -1) {
			dnet_error(port, "accept", errno);
			if (port->port_channel != (int) -1) {
				close(port->port_channel);
				port->port_channel = (int) -1;
			}
			return NULL;
		}
		if (port->port_channel != (int) -1) {
			close(port->port_channel);
			port->port_channel = (int) -1;
		}
		port->port_handle = (HANDLE) n;
		port->port_flags |= PORT_async;
		return port;
	}

/* client side */

	response = &packet->p_resp;
	if (response->p_resp_data.cstr_length > OB_MAXNAME) {
		dnet_error(port, "dnet_conn", EINVAL);
		return NULL;
	}
	else
		bcopy(response->p_resp_data.cstr_address, object_name,
			  response->p_resp_data.cstr_length);

	n =
		dnet_conn(port->port_connection->str_data, object_name,
				  SOCK_SEQPACKET, NULL, NULL, NULL, NULL);

	if (n < 0) {
		dnet_error(port, "dnet_conn", errno);
		return NULL;
	}

	new_port = alloc_port(port->port_parent);
	new_port->port_handle = (HANDLE) n;
	new_port->port_flags |= PORT_async;
	new_port->port_flags = port->port_flags & PORT_no_oob;

	if (ast) {
		pid = getpid();
		if (ioctl(new_port->port_handle, SIOCSPGRP, &pid) < 0) {
			disconnect(new_port);
			dnet_error(port, "ioctl/SIOCSPGRP", errno);
			return NULL;
		}
		ISC_signal(SIGURG, dnet_handler, new_port);
		new_port->port_ast = ast;
	}

	port->port_async = new_port;
	return new_port;
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
	PORT new_port;
	P_RESP *response;
	int n, pid;
	struct sockaddr_dn address;

/* Set up new socket */

	if ((n = socket(AF_DECnet, SOCK_STREAM, 0)) < 0) {
		dnet_error(port, "socket", errno);
		return NULL;
	}

	bzero((SCHAR *) & address, sizeof(address));
	address.sdn_family = AF_DECnet;
	address.sdn_objnum = 0;
	pid = getpid();
	sprintf(address.sdn_objname, "ISC_%X_%d", pid, request_count++);
	address.sdn_objnamel = strlen(address.sdn_objname);

	if ((bind(n, &address, sizeof(address)) < 0) || (listen(n, 1) < 0)) {
		close(n);
		dnet_error(port, "bind", errno);
		return NULL;
	}

	new_port = alloc_port(port->port_parent);
	new_port->port_server_flags = port->port_server_flags;
	new_port->port_channel = n;
	new_port->port_flags = port->port_flags & PORT_no_oob;
	strcpy(new_port->port_buffer, address.sdn_objname);

	response = &packet->p_resp;
	response->p_resp_data.cstr_address = new_port->port_buffer;
	response->p_resp_data.cstr_length = strlen(new_port->port_buffer);

	port->port_async = new_port;
	return new_port;
}


static int check_host( PORT port)
{
/**************************************
 *
 *	c h e c k _ h o s t
 *
 **************************************
 *
 * Functional description
 *	Check the host on the other end of the socket to see it
 *	it's an equivalent host.
 *
 **************************************/
	IB_FILE *equiv;
	UCHAR host_name[32];
	USHORT result;
	int length;
	struct sockaddr_dn address;
	struct nodeent *node;

	return TRUE;

/* comment out remaining code since it is not executed
length = sizeof (address);
if (getpeername (port->port_handle, &address, &length) == -1)
    return FALSE;

node = getnodebyaddr (&address.sdn_add, sizeof (address.sdn_add),
	address.sdn_family);
if (!node)
    return FALSE;

equiv = ib_fopen ("/etc/hosts.equiv", "r");
if (!equiv)
    return FALSE;

result = FALSE;

while (ib_fscanf (equiv, "%s", host_name) != -1)
    if (strcmp (host_name, node->n_name) == 0)
	{
	result = TRUE;
	break;
	}

ib_fclose (equiv);

return result;
end unxecuted code*/
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
	PORT parent, *ptr;

	if (port->port_ast)
		ISC_signal_cancel(SIGURG, dnet_handler, port);

/* If this is a sub-port, unlink it from it's parent */

	if (parent = port->port_parent) {
		if (port->port_async) {
			disconnect(port->port_async);
			port->port_async = NULL;
		}
		for (ptr = &parent->port_clients; *ptr; ptr = &(*ptr)->port_next)
			if (*ptr == port) {
				*ptr = port->port_next;
				if (ptr == &parent->port_clients)
					parent->port_next = *ptr;
				break;
			}
	}
	else if (port->port_async) {
#ifdef MULTI_THREAD
		port->port_async->port_flags |= PORT_disconnect;
#else
		disconnect(port->port_async);
		port->port_async = NULL;
#endif
	}

	if (port->port_channel != (int) -1) {
		close(port->port_channel);
		port->port_channel = (int) -1;
	}

	if (port->port_handle != (HANDLE) - 1) {
		close(port->port_handle);
		port->port_handle = (HANDLE) - 1;
	}

	cleanup_port(port);
}


static void cleanup_port( PORT port)
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


static XDR_INT dnet_destroy( XDR * xdrs)
{
/**************************************
 *
 *	d n e t _ d e s t r o y
 *
 **************************************
 *
 * Functional description
 *	Destroy a stream.  A no-op.
 *
 **************************************/
}


static int dnet_error( PORT port, TEXT * operation, int status)
{
/**************************************
 *
 *	d n e t _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	An I/O error has occurred.  If a status vector is present,
 *	generate an error return.  In any case, return NULL, which
 *	is used to indicate and error.
 *
 **************************************/
	RDB rdb;
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
		*status_vector++ = (STATUS) "DECnet connection";
		if (status) {
			*status_vector++ = gds_arg_unix;
			*status_vector++ = status;
		}
		*status_vector++ = 0;
	}

	return 0;
}


static bool_t dnet_getbytes( XDR * xdrs, SCHAR * buff, int count)
{
/**************************************
 *
 *	d n e t _ g e t b y t e s
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
			if (!dnet_read(xdrs))
				return FALSE;
		}
	}

/* Scalar values and bulk transfer remainder fall thru
   to be moved byte-by-byte to avoid memcpy setup costs. */

	if (!bytecount)
		return TRUE;

	if (bytecount && xdrs->x_handy >= bytecount) {
		xdrs->x_handy -= bytecount;
		do
			*buff++ = *xdrs->x_private++;
		while (--bytecount);
		return TRUE;
	}

	while (--bytecount >= 0) {
		if (!xdrs->x_handy && !dnet_read(xdrs))
			return FALSE;
		*buff++ = *xdrs->x_private++;
		--xdrs->x_handy;
	}

	return TRUE;
}


static bool_t dnet_getlong( XDR * xdrs, SLONG * lp)
{
/**************************************
 *
 *	d n e t _ g e t l o n g
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


static u_int dnet_getpostn( XDR * xdrs)
{
/**************************************
 *
 *	d n e t _ g e t p o s t n
 *
 **************************************
 *
 * Functional description
 *	Get the current position (which is also current length) from stream.
 *
 **************************************/

	return xdrs->x_private - xdrs->x_base;
}


static void dnet_handler( PORT port)
{
/**************************************
 *
 *	d n e t _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Dnet_handler is the signal handler on receipt of
 *	a SIGURG indicating out-of-band data.  Since SIGURG
 *	may be noisy, check to see if any IO is pending on the channel.
 *	If not, ignore the signal.  If so, call the port specific 
 *	handler to do something appropriate.
 *
 **************************************/
	int n;
	UCHAR junk;

/* If there isn't any out of band data, this signal isn't for us */

	if ((n = recv(port->port_handle, &junk, 1, MSG_OOB)) < 0)
		return;

	(*port->port_ast) (port);
}


static caddr_t dnet_inline( XDR * xdrs, u_int bytecount)
{
/**************************************
 *
 *	d n e t _  i n l i n e
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


static bool_t dnet_putbytes( XDR * xdrs, SCHAR * buff, int count)
{
/**************************************
 *
 *	d n e t _ p u t b y t e s
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
			if (!dnet_write(xdrs, FALSE))
				return FALSE;
		}
	}

/* Scalar values and bulk transfer remainder fall thru
   to be moved byte-by-byte to avoid memcpy setup costs. */

	if (!bytecount)
		return TRUE;

	if (bytecount && xdrs->x_handy >= bytecount) {
		xdrs->x_handy -= bytecount;
		do
			*xdrs->x_private++ = *buff++;
		while (--bytecount);
		return TRUE;
	}

	while (--bytecount >= 0) {
		if (xdrs->x_handy <= 0 && !dnet_write(xdrs, FALSE))
			return FALSE;
		--xdrs->x_handy;
		*xdrs->x_private++ = *buff++;
	}

	return TRUE;
}


static bool_t dnet_putlong( XDR * xdrs, SLONG * lp)
{
/**************************************
 *
 *	d n e t _ p u t l o n g
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


static bool_t dnet_read( XDR * xdrs)
{
/**************************************
 *
 *	d n e t _ r e a d
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
	UCHAR *p, *end;

	port = (PORT) xdrs->x_public;
	p = (UCHAR *) xdrs->x_base;
	end = p + BUFFER_SIZE;

/* If buffer is not completely empty, slide down what what's left */

	if (xdrs->x_handy > 0) {
		memmove(p, xdrs->x_private, xdrs->x_handy);
		p += xdrs->x_handy;
	}

/* If an ACK is pending, do an ACK.  The alternative is deadlock. */

/*
if (port->port_flags & PORT_pend_ack)
    if (!packet_send (port, NULL, 0))
	return FALSE;
*/

	while (TRUE) {
		length = end - p;
		if (!packet_receive(port, p, length, &length))
			return FALSE;
		if (length >= 0) {
			p += length;
			break;
		}
		p -= length;
		if (!packet_send(port, NULL, 0))
			return FALSE;
	}

	port->port_flags |= PORT_pend_ack;
	xdrs->x_handy = p - (UCHAR *) xdrs->x_base;
	xdrs->x_private = xdrs->x_base;

	return TRUE;
}


static bool_t dnet_setpostn( XDR * xdrs, u_int bytecount)
{
/**************************************
 *
 *	d n e t _ s e t p o s t n
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


static bool_t dnet_write( XDR * xdrs, bool_t end_flag)
{
/**************************************
 *
 *	d n e t _ w r i t e
 *
 **************************************
 *
 * Functional description
 *	Write a buffer fulll of data.  If the end_flag isn't set, indicate
 *	that the buffer is a fragment, and reset the XDR for another buffer
 *	load.
 *
 **************************************/
	UCHAR aux_buffer[BUFFER_SIZE], *p, *q;
	UCHAR buffer[BUFFER_SIZE];
	PORT port;
	SSHORT l, length, l2;

/* Encode the data portion of the packet */

	port = (PORT) xdrs->x_public;
	p = (UCHAR *) xdrs->x_base;
	length = (UCHAR *) xdrs->x_private - p;

/* Send data in manageable hunks.  If a packet is partial, indicate
   that with a negative length.  A positive length marks the end. */

	p = (UCHAR *) xdrs->x_base;

	while (length) {
		port->port_misc1 = (port->port_misc1 + 1) % MAX_SEQUENCE;
		l = MIN(length, MAX_DATA);
		length -= l;
		while (TRUE) {
			if (!packet_send(port, p, (length) ? -l : l))
				return FALSE;
			/*
			   if (packet_receive (port, aux_buffer, sizeof (aux_buffer), &l2))
			 */
			break;
		}
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
	UCHAR c, aux_buffer[1024], *p;
	USHORT l, n, checksum;

	do
		n = read(port->port_handle, buffer, buffer_length);
	while (n == -1 && SYSCALL_INTERRUPTED(errno));

	if (n == -1)
		return dnet_error(port, "read", errno);

	if (!n)
		return dnet_error(port, "read end-of_file", 0);

	*length = n;

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
	SSHORT checksum, n;

	do
		n = write(port->port_handle, data, length);
	while (n == -1 && SYSCALL_INTERRUPTED(errno));

	if (n == -1)
		return dnet_error(port, "write", errno);

	if ((port->port_flags & PORT_async) && !(port->port_flags & PORT_no_oob))
		n = send(port->port_handle, data, 1, MSG_OOB);

	if (n == -1)
		return dnet_error(port, "send/oob", errno);

	port->port_flags &= ~PORT_pend_ack;

	return TRUE;
}


static PORT receive( PORT port, PACKET * packet)
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

	if (!xdr_protocol(&port->port_receive, packet))
		return NULL;

	return port;
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

	if (!xdr_protocol(&port->port_send, packet))
		return FALSE;

	return xdrdnet_endofrecord(&port->port_send, TRUE);
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

	return xdr_protocol(&port->port_send, packet);
}




static xdrdnet_create(
					  XDR * xdrs,
					  PORT port,
					  UCHAR * buffer, USHORT length, enum xdr_op x_op)
{
/**************************************
 *
 *	x d r d n e t _ c r e a t e
 *
 **************************************
 *
 * Functional description
 *	Initialize an XDR stream for Apollo mailboxes.
 *
 **************************************/

	xdrs->x_public = (caddr_t) port;
	xdrs->x_base = xdrs->x_private = (caddr_t) buffer;
	xdrs->x_handy = length;
	xdrs->x_ops = &dnet_ops;
	xdrs->x_op = x_op;

	return TRUE;
}


static int xdrdnet_endofrecord( XDR * xdrs, bool_t flushnow)
{
/**************************************
 *
 *	x d r d n e t _ e n d o f r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Write out the rest of a record.
 *
 **************************************/

	return dnet_write(xdrs, flushnow);
}
