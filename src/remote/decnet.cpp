/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		decnet.c
 *	DESCRIPTION:	DECnet Communications module for VMS
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
#include descrip
#include iodef
#include nfbdef
#include msgdef

#include "../jrd/ib_stdio.h"
#include "../remote/remote.h"
#include "../jrd/gds.h"
#include "../jrd/thd.h"
#include "../remote/decne_proto.h"
#include "../remote/remot_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/sch_proto.h"

#define OBJECT_NUMBER	128
#define OBJECT_NAME	""
#define SERVER_NAME	"ISC_SERVER"

#define EVENT_FLAG	15
#define MAX_DATA	1450
#define BUFFER_SIZE	MAX_DATA
#define MAX_SEQUENCE	256

typedef struct mbxmsg {
	SSHORT msgtype;
	SSHORT unit;
	SCHAR name_count;
	SCHAR name[1];
} *MBXMSG;

static int accept_connection(PORT, P_CNCT *);
static int accept_link(PORT, MBXMSG);
static PORT active_port(PORT);
static PORT alloc_port(PORT);
static PORT aux_connect(PORT, PACKET *, XDR_INT(*)());
static PORT aux_request(PORT, PACKET *);
static void cleanup_port(PORT);
static int dnet_create(XDR *, PORT, UCHAR *, USHORT, enum xdr_op);
static XDR_INT dnet_destroy(XDR *);
static int dnet_endofrecord(XDR *, bool_t);
static int dnet_error(PORT, UCHAR *, int);
static bool_t dnet_getbytes(XDR *, SCHAR *, int);
static bool_t dnet_getlong(XDR *, SLONG *);
static u_int dnet_getpostn(XDR *);
static caddr_t dnet_inline(XDR *, u_int);
static bool_t dnet_putbytes(XDR *, SCHAR *, int);
static bool_t dnet_putlong(XDR *, SLONG *);
static bool_t dnet_read(XDR *);
static bool_t dnet_setpostn(XDR *, u_int);
static bool_t dnet_write(XDR *, bool_t);
static void disconnect(PORT);
static int packet_receive(PORT, UCHAR *, SSHORT, SSHORT *);
static int packet_send(PORT, UCHAR *, SSHORT);
static int process_connect(PORT);
static PORT receive(PORT, PACKET *);
static int send_full(PORT, PACKET *);
static int send_partial(PORT, PACKET *);
static int start_read(PORT, UCHAR *, SSHORT);
static void wait(USHORT *);
static int wait_and_test(PORT, TEXT *, int);

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

static struct {
	SCHAR nfb_type;
	SLONG nfb_value;
} null_nfb = {
NFB$C_DECLNAME, 0}, nfb = {
NFB$C_DECLOBJ, OBJECT_NUMBER};

static SLONG maxmsg = 128, bufquo = 128;
static request_count;


PORT DECNET_analyze(
					UCHAR * file_name;
					SSHORT * file_length,
					STATUS * status_vector; SSHORT uv_flag; {
/**************************************
 *
 *	D E C N E T _ a n a l y z e
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
PACKET * packet;
P_CNCT * connect;
P_ATCH * attach;
SSHORT length, user_length;
UCHAR node_name[128], *p, *q, user_id[200], c;
TEXT buffer[64]; int local_status[20]; p_cnct::p_cnct_repeat * protocol;
/* Analyze the file name to see if a remote connection is required.  If not,
   quietly (sic) return. */
/* Pick up node if, if present */
p = node_name;
q = file_name; length = *file_length; while (q[0] != ':' || q[1] != ':') {
if (--length < 0 || p >= node_name + sizeof(node_name) - 1)
return NULL; c = *q++; *p++ = UPPER(c);}

q += 2; *p = 0; length -= 2; *file_length = length; if (length)
do
*file_name++ = *q++; while (--length); *file_name = 0;
/* We need to establish a connection to a remote server.  Allocate the necessary
   blocks and get ready to go. */
rdb = ALLOC(type_rdb); packet = &rdb->rdb_packet;
/* Pick up some user identification information */
user_id[0] = CNCT_user;
p = user_id + 2;
ISC_get_user(p, 0, 0, 0, 0, 0, 0);
user_id[1] = (UCHAR) strlen(p); for (; *p; p++)
if (*p >= 'A' && *p <= 'Z')
* p = *p - 'A' + 'a';
*p++ = CNCT_host;
p++; ISC_get_host(p, 64); p[-1] = (UCHAR) strlen((SCHAR *) p); for (; *p; p++)
if (*p >= 'A' && *p <= 'Z')
* p = *p - 'A' + 'a'; if (uv_flag) {
*p++ = CNCT_user_verification; *p++ = 0;}

user_length = p - user_id;
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
/* If we want user verification, we can't speak anything less that version 7 */
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
protocol->p_cnct_max_type = ptype_batch_send;
protocol->p_cnct_weight = 2;
++protocol;
protocol->p_cnct_version = PROTOCOL_VERSION8;
protocol->p_cnct_architecture = ARCHITECTURE;
protocol->p_cnct_min_type = ptype_rpc;
protocol->p_cnct_max_type = ptype_batch_send; protocol->p_cnct_weight = 3;
#ifdef SCROLLABLE_CURSORS
++protocol;
protocol->p_cnct_version = PROTOCOL_SCROLLABLE_CURSORS;
protocol->p_cnct_architecture = arch_generic;
protocol->p_cnct_min_type = ptype_rpc;
protocol->p_cnct_max_type = ptype_batch_send;
protocol->p_cnct_weight = 4;
++protocol;
protocol->p_cnct_version = PROTOCOL_SCROLLABLE_CURSORS;
protocol->p_cnct_architecture = ARCHITECTURE;
protocol->p_cnct_min_type = ptype_rpc;
protocol->p_cnct_max_type = ptype_batch_send; protocol->p_cnct_weight = 5;
#endif
/* If we can't talk to a server, punt.  Let somebody else generate
   an error. */
if (!(port = DECNET_connect(node_name, packet, status_vector, 0))) {
ALLR_release(rdb); return NULL;}

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
connect->p_cnct_file.cstr_address = file_name;
/* try again with next set of known protocols */
connect->p_cnct_count = 5;
connect->p_cnct_user_id.cstr_length = user_length;
connect->p_cnct_user_id.cstr_address = user_id;
protocol = connect->p_cnct_versions;
protocol->p_cnct_version = PROTOCOL_VERSION5;
protocol->p_cnct_architecture = ARCHITECTURE;
protocol->p_cnct_min_type = ptype_rpc;
protocol->p_cnct_max_type = ptype_out_of_band;
protocol->p_cnct_weight = 2;
++protocol;
protocol->p_cnct_version = PROTOCOL_VERSION6;
protocol->p_cnct_architecture = arch_generic;
protocol->p_cnct_min_type = ptype_rpc;
protocol->p_cnct_max_type = ptype_batch_send;
protocol->p_cnct_weight = 3;
++protocol;
protocol->p_cnct_version = PROTOCOL_VERSION6;
protocol->p_cnct_architecture = ARCHITECTURE;
protocol->p_cnct_min_type = ptype_rpc;
protocol->p_cnct_max_type = ptype_batch_send;
protocol->p_cnct_weight = 4;
++protocol;
protocol->p_cnct_version = PROTOCOL_VERSION7;
protocol->p_cnct_architecture = arch_generic;
protocol->p_cnct_min_type = ptype_rpc;
protocol->p_cnct_max_type = ptype_batch_send;
protocol->p_cnct_weight = 5;
++protocol;
protocol->p_cnct_version = PROTOCOL_VERSION7;
protocol->p_cnct_architecture = ARCHITECTURE;
protocol->p_cnct_min_type = ptype_rpc;
protocol->p_cnct_max_type = ptype_batch_send;
protocol->p_cnct_weight = 6;
if (!(port = DECNET_connect(node_name, packet, status_vector, 0))) {
ALLR_release(rdb); return NULL;}

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
if (!(port = DECNET_connect(node_name, packet, status_vector, 0))) {
ALLR_release(rdb); return NULL;}

/* Get response packet from server. */

rdb->rdb_port = port;
port->port_context = rdb;
port->receive(packet);
}

if (packet->p_operation != op_accept) {
*status_vector++ = gds_arg_gds;
*status_vector++ = gds__connect_reject;
*status_vector++ = 0; disconnect(port); return NULL;}

port->port_protocol = packet->p_acpt.p_acpt_version;
/* once we've decided on a protocol, concatenate the version
   string to reflect it...  */
sprintf(buffer, "%s/P%d", port->port_version->str_data, port->port_protocol);
ALLR_free(port->port_version);
port->port_version = REMOTE_make_string(buffer);
if (packet->p_acpt.p_acpt_architecture == ARCHITECTURE)
port->port_flags |= PORT_symmetric;
if (packet->p_acpt.p_acpt_type == ptype_rpc)
port->port_flags |= PORT_rpc;
if (packet->p_acpt.p_acpt_type != ptype_out_of_band)
port->port_flags |= PORT_no_oob; return port;}


PORT DECNET_connect(UCHAR * name,
					PACKET * packet, STATUS * status_vector, int flags) {
/**************************************
 *
 *	D E C N E T _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Establish half of a communication link.  If a connect packet is given,
 *	the connection is on behalf of a remote interface.  Other the connect
 *	is for a server process.
 *
 **************************************/
TEXT * p, *object_name, host[128], string[128];
int *s, status, n, l, object_number;
USHORT channel, mbx_channel, iosb[4];
PORT port;
struct dsc$descriptor_s desc, nfb_desc;
port = alloc_port(NULL);
port->port_status_vector = status_vector;
s = status_vector; *s++ = gds_arg_gds; *s = 0;
/* Handle debug protocol - if one is specified, we go by name else by OBJECT_NUMBER */
if (name) {
strcpy(host, name); for (p = host; *p;)
if (*p++ == '/') {
p[-1] = 0; name = host; break;}
object_name = p; object_number = (*object_name) ? 0 : OBJECT_NUMBER;}

/* Handle outgoing connections */

if (packet) {
sprintf(string, "%s::\"%d=%s\"", name, object_number, object_name);
ISC_make_desc(string, &desc, 0);
status = sys$assign(&desc, &channel, NULL, NULL); if (!(status & 1)) {
dnet_error(port, "sys$assign", status); return NULL;}
port->port_handle = channel; send_full(port, packet); return port;}

/* Now handle incoming connections.  Handle single client case first */

if (!(flags & SRVR_multi_client)) {
ISC_make_desc("SYS$NET", &desc, 0);
status = sys$assign(&desc, &channel, NULL, NULL); if (!(status & 1)) {
dnet_error(port, "sys$assign", status); return NULL;}
port->port_server_flags |= SRVR_server;
port->port_handle = channel; return port;}

/* We're supposed to be a multi-client server -- take our best shot */

ISC_make_desc("_NET:", &desc, 0);
status = lib$asn_wth_mbx(&desc, &maxmsg, &bufquo, &channel, &mbx_channel);
if (!(status & 1)) {
dnet_error(port, "lib$asn_wth_mbx", status); return NULL;}

ISC_make_desc(&nfb, &nfb_desc, sizeof(nfb)); status = sys$qio(EVENT_FLAG,	/* Event flag */
															  channel,	/* Channel */
															  IO$_ACPCONTROL,	/* Function */
															  port->port_iosb,	/* IO status block */
															  gds__completion_ast,	/* AST address */
															  iosb,	/* AST parameter */
															  &nfb_desc,	/* P1 (buffer) */
															  NULL,	/* P2 (length) */
															  NULL,	/* P3 (virtual block) */
															  NULL, NULL,
															  NULL);
if (!wait_and_test(port, "qio/acpcontrol", status))
return NULL; port->port_server_flags |= SRVR_server | SRVR_multi_client;
port->port_handle = mbx_channel; port->port_channel = channel;
if (!start_read(port, port->port_buffer, maxmsg))
return NULL; return port;}


static int accept_connection(PORT port, P_CNCT * connect) {
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

return TRUE;}


static int accept_link(PORT port, MBXMSG msg) {
/**************************************
 *
 *	a c c e p t _ l i n k
 *
 **************************************
 *
 * Functional description
 *	Accept a new logical link.  Return new channel number
 *	or NULL.
 *
 **************************************/
TEXT * p, stuff[32];
PORT new_port;
int status;
USHORT channel, iosb[4];
struct dsc$descriptor_s desc;
ISC_make_desc("_NET:", &desc, 0);
status = sys$assign(&desc, &channel, NULL, NULL); if (!(status & 1))
return dnet_error(port, "sys$assign", status);
p = msg->name + msg->name_count;
desc.dsc$w_length = *p; desc.dsc$a_pointer = p + 1;
status = sys$qio(EVENT_FLAG,	/* Event flag */
				 channel,	/* Channel */
				 IO$_ACCESS,	/* Function */
				 iosb,	/* IO status block */
				 gds__completion_ast,	/* AST address */
				 NULL,	/* AST parameter */
				 NULL,	/* P1 (buffer) */
				 &desc,	/* P2 (length) */
				 NULL,	/* P3 (virtual block) */
				 NULL,
				 NULL,
				 NULL);
if (status & 1) {
wait(iosb); status = iosb[0];}

if (!(status & 1))
return dnet_error(port, "sys$assign", status); return channel;}


static PORT active_port(PORT port) {
/**************************************
 *
 *	a c t i v e _ p o r t
 *
 **************************************
 *
 * Functional description
 *	Return the address of an active port (i.e. read complete)
 *	or NULL (nothing complete).
 *
 **************************************/

if (port->port_iosb[0])
return port; for (port = port->port_clients; port; port = port->port_next)
if (port->port_iosb[0])
return port; return NULL;}


static PORT alloc_port(PORT parent) {
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
UCHAR * message;
PORT port;
USHORT l;
UCHAR buffer[256];
l = (parent && (parent->port_server_flags & SRVR_multi_client)) ?
BUFFER_SIZE * 3 : BUFFER_SIZE * 2;
port = ALLOCV(type_port, l);
port->port_type = port_decnet;
port->port_state = state_pending;
ISC_get_host(buffer, sizeof(buffer));
port->port_host = REMOTE_make_string(buffer);
sprintf(buffer, "DECnet (%s)", port->port_host->str_data);
port->port_version = REMOTE_make_string(buffer); if (parent) {
port->port_parent = parent;
port->port_next = parent->port_clients;
parent->port_clients = port;
port->port_handle = parent->port_handle;
port->port_server = parent->port_server;
port->port_server_flags = parent->port_server_flags;}

message = port->port_buffer;
port->port_accept = &accept_connection;
port->port_disconnect = &disconnect;
port->port_receive_packet = &receive;
port->port_send_packet = &send_full;
port->port_send_partial = &send_partial;
port->port_connect = aux_connect;
port->port_request = aux_request;
port->port_buff_size = BUFFER_SIZE;
dnet_create(&port->port_send, port,
			&port->port_buffer[BUFFER_SIZE], BUFFER_SIZE,
			XDR_ENCODE);
dnet_create(&port->port_receive, port, port->port_buffer, 0,
			XDR_DECODE); return port;}


static PORT aux_connect(PORT port, PACKET * packet, XDR_INT(*ast) ()) {
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
P_RESP * response;
P_REQ * request;
MBXMSG msg;
int n, l, arg, status;
USHORT channel;
PORT new_port; UCHAR buffer[128], *p; struct dsc$descriptor_s desc;
/* If this is a server, we're got an auxiliary connection.  Accept it */
if (port->port_server_flags) {
if (!start_read(port, buffer, sizeof(buffer)))
return NULL; if (!wait_and_test(port, "qio/readvblk", 1))
return NULL;
channel = accept_link(port, buffer);
sys$dassgn(port->port_channel);
if (!channel)
return NULL;
port->port_handle = channel;
port->port_flags |= PORT_async; port->port_iosb[0] = 0; return port;}

port->port_async = new_port = alloc_port(port->port_parent);
new_port->port_flags |= PORT_async;
response = &packet->p_resp;
request = &packet->p_req;
ISC_make_desc(response->p_resp_data.cstr_address, &desc,
			  response->p_resp_data.cstr_length);
status = sys$assign(&desc, &channel, NULL, NULL); if (!(status & 1)) {
dnet_error(port, "sys$assign", status); return NULL;}

new_port->port_handle = channel;
new_port->port_flags = port->port_flags & PORT_no_oob; return new_port;}


static PORT aux_request(PORT port, PACKET * packet) {
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
PORT parent, new_port;
P_RESP * response;
int n, length, status, pid;
USHORT channel, mbx_channel;
TEXT task_name[32], *ncb;
struct dsc$descriptor_s desc, nfb_desc;
ISC_make_desc("_NET:", &desc, 0);
status = lib$asn_wth_mbx(&desc, &maxmsg, &bufquo, &channel, &mbx_channel);
if (!(status & 1)) {
dnet_error(port, "sys$assign", status); return NULL;}

port->port_async = new_port = alloc_port(port->port_parent);
new_port->port_server_flags = port->port_server_flags;
new_port->port_handle = mbx_channel;
new_port->port_channel = channel;
new_port->port_flags = port->port_flags & PORT_no_oob;
pid = getpid();
ncb = new_port->port_buffer;
sprintf(task_name, "ISC_%X_%d", pid, ++request_count);
sprintf(ncb, "%s::\"TASK=%s\"", port->port_host->str_data, task_name);
ISC_make_desc(task_name, &desc, 0);
ISC_make_desc(&null_nfb, &nfb_desc, sizeof(null_nfb)); status = sys$qio(EVENT_FLAG,	/* Event flag */
																		channel,	/* Channel */
																		IO$_ACPCONTROL,	/* Function */
																		new_port->port_iosb,	/* IO status block */
																		gds__completion_ast,	/* AST address */
																		NULL,	/* AST parameter */
																		&nfb_desc,	/* P1 (buffer) */
																		&desc,	/* P2 (length) */
																		NULL,	/* P3 (virtual block) */
																		NULL,
																		NULL,
																		NULL);
if (!wait_and_test(new_port, "qio/acpcontrol", status))
return NULL; response = &packet->p_resp;
response->p_resp_data.cstr_address = ncb;
response->p_resp_data.cstr_length = strlen(ncb); return new_port;}


static dnet_create(XDR * xdrs,
				   PORT port,
				   UCHAR * buffer, USHORT length, enum xdr_op x_op) {
/**************************************
 *
 *	d n e t _ c r e a t e
 *
 **************************************
 *
 * Functional description
 *	Initialize an XDR stream for Apollo mailboxes.
 *
 **************************************/

xdrs->x_public = port;
xdrs->x_base = xdrs->x_private = buffer;
xdrs->x_handy = length;
xdrs->x_ops = &dnet_ops; xdrs->x_op = x_op; return TRUE;}


static XDR_INT dnet_destroy(XDR * xdrs) {
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


static int dnet_endofrecord(XDR * xdrs, bool_t flushnow) {
/**************************************
 *
 *	d n e t _ e n d o f r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Write out the rest of a record.
 *
 **************************************/

return dnet_write(xdrs, flushnow);}


static int dnet_error(PORT port, UCHAR * operation, int status) {
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
int *status_vector;
port->port_flags |= PORT_broken;
port->port_state = state_broken;
if (rdb = port->port_context) status_vector = rdb->rdb_status_vector;
else
status_vector = port->port_status_vector; if (status_vector) {
*status_vector++ = gds_arg_gds;
*status_vector++ = gds__io_error;
*status_vector++ = gds_arg_string;
*status_vector++ = operation;
*status_vector++ = gds_arg_string;
*status_vector++ = "DECnet connection"; if (status) {
*status_vector++ = gds_arg_vms; *status_vector++ = status;}
*status_vector++ = 0;}

return NULL;}


static bool_t dnet_getbytes(XDR * xdrs, SCHAR * buff, int count) {
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
xdrs->x_private += bytecount; xdrs->x_handy -= bytecount; return TRUE;}
else
{
if (xdrs->x_handy > 0) {
memcpy(buff, xdrs->x_private, xdrs->x_handy);
xdrs->x_private += xdrs->x_handy;
buff += xdrs->x_handy; bytecount -= xdrs->x_handy; xdrs->x_handy = 0;}
if (!dnet_read(xdrs))
return FALSE;}
}

/* Scalar values and bulk transfer remainder fall thru
   to be moved byte-by-byte to avoid memcpy setup costs. */

if (!bytecount)
return TRUE; if (bytecount && xdrs->x_handy >= bytecount) {
xdrs->x_handy -= bytecount;
do
*buff++ = *xdrs->x_private++; while (--bytecount); return TRUE;}

while (--bytecount >= 0) {
if (!xdrs->x_handy && !dnet_read(xdrs))
return FALSE; *buff++ = *xdrs->x_private++; --xdrs->x_handy;}

return TRUE;}


static bool_t dnet_getlong(XDR * xdrs, SLONG * lp) {
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
SLONG l; if (!(*xdrs->x_ops->x_getbytes) (xdrs, &l, 4))
return FALSE; *lp = ntohl(l); return TRUE;}


static u_int dnet_getpostn(XDR * xdrs) {
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

return xdrs->x_private - xdrs->x_base;}


static caddr_t dnet_inline(XDR * xdrs, u_int bytecount) {
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
return FALSE; return xdrs->x_base + bytecount;}


static bool_t dnet_putbytes(XDR * xdrs, SCHAR * buff, int count) {
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
xdrs->x_private += bytecount; xdrs->x_handy -= bytecount; return TRUE;}
else
{
if (xdrs->x_handy > 0) {
memcpy(xdrs->x_private, buff, xdrs->x_handy);
xdrs->x_private += xdrs->x_handy;
buff += xdrs->x_handy; bytecount -= xdrs->x_handy; xdrs->x_handy = 0;}
if (!dnet_write(xdrs))
return FALSE;}
}

/* Scalar values and bulk transfer remainder fall thru
   to be moved byte-by-byte to avoid memcpy setup costs. */

if (!bytecount)
return TRUE; if (bytecount && xdrs->x_handy >= bytecount) {
xdrs->x_handy -= bytecount;
do
*xdrs->x_private++ = *buff++; while (--bytecount); return TRUE;}

while (--bytecount >= 0) {
if (xdrs->x_handy <= 0 && !dnet_write(xdrs))
return FALSE; --xdrs->x_handy; *xdrs->x_private++ = *buff++;}

return TRUE;}


static bool_t dnet_putlong(XDR * xdrs, SLONG * lp) {
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
SLONG l; l = htonl(*lp); return (*xdrs->x_ops->x_putbytes) (xdrs, &l, 4);}


static bool_t dnet_read(XDR * xdrs) {
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
UCHAR * p, *end;
port = xdrs->x_public; p = xdrs->x_base; end = p + BUFFER_SIZE;
/* If buffer is not completely empty, slide down what what's left */
if (xdrs->x_handy > 0) {
memmove(p, xdrs->x_private, xdrs->x_handy); p += xdrs->x_handy;}

/* If an ACK is pending, do an ACK.  The alternative is deadlock. */

/*
if (port->port_flags & PORT_pend_ack)
    if (!packet_send (port, NULL, 0))
	return FALSE;
*/

while (TRUE) {
length = end - p; if (!packet_receive(port, p, length, &length)) {
return FALSE; if (!packet_send(port, NULL, 0))
return FALSE; continue;}
if (length >= 0) {
p += length; break;}
p -= length; if (!packet_send(port, NULL, 0))
return FALSE;}

port->port_flags |= PORT_pend_ack;
xdrs->x_handy = p - xdrs->x_base; xdrs->x_private = xdrs->x_base; return TRUE;}


static bool_t dnet_setpostn(XDR * xdrs, u_int bytecount) {
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
return FALSE; xdrs->x_private = xdrs->x_base + bytecount; return TRUE;}


static bool_t dnet_write(XDR * xdrs, bool_t end_flag) {
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
UCHAR aux_buffer[BUFFER_SIZE], *p;
UCHAR buffer[BUFFER_SIZE]; PORT port; SSHORT l, length, l2;
/* Encode the data portion of the packet */
port = xdrs->x_public; p = xdrs->x_base; length = xdrs->x_private - p;
/* Send data in manageable hunks.  If a packet is partial, indicate
   that with a negative length.  A positive length marks the end. */
p = xdrs->x_base; while (length) {
port->port_misc1 = (port->port_misc1 + 1) % MAX_SEQUENCE;
l = MIN(length, MAX_DATA); length -= l; while (TRUE) {
if (!packet_send(port, p, (length) ? -l : l))
return FALSE;
/*
   if (packet_receive (port, aux_buffer, sizeof (aux_buffer), &l2))
 */
break;}
p += l;}

xdrs->x_private = xdrs->x_base; xdrs->x_handy = BUFFER_SIZE; return TRUE;
/* If the other end has not piggy-backed the next packet, we're done. */
if (!l2)
return TRUE;
/* We've got a piggy-backed response.  If the packet is partial,
   send an ACK for part we did receive. */
p = aux_buffer; while (l2 < 0) {
if (!packet_send(port, NULL, 0))
return FALSE;
p -= l2;
length = aux_buffer + sizeof(aux_buffer) - p;
if (!packet_receive(port, p, length, &l2)) {
p += l2; continue;}
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
p += xdrs->x_handy; xdrs->x_private = xdrs->x_base;
/*
xdrs->x_handy += JAP_decode (aux_buffer, length, p);
*/
port->port_flags |= PORT_pend_ack; return TRUE;}


static void disconnect(PORT port) {
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
/* If this is a sub-port, unlink it from it's parent */
if (parent = port->port_parent) {
if (port->port_async) {
disconnect(port->port_async); port->port_async = NULL;}
for (ptr = &parent->port_clients; *ptr; ptr = &(*ptr)->port_next)
if (*ptr == port) {
*ptr = port->port_next; if (ptr == &parent->port_clients)
parent->port_next = *ptr; break;}
}
else
if (port->port_async) {
#ifdef MULTI_THREAD
port->port_async->port_flags |= PORT_disconnect;
#else
disconnect(port->port_async); port->port_async = NULL;
#endif
}

sys$dassgn(port->port_handle); cleanup_port(port);}
^L static void cleanup_port(PORT port) {
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
ALLR_free((UCHAR *) port->port_version); if (port->port_host)
ALLR_free((UCHAR *) port->port_host);
#ifdef DEBUG_XDR_MEMORY
if (port->port_packet_vector)
ALLR_free((UCHAR *) port->port_packet_vector);
#endif
ALLR_release((UCHAR *) port); return;}


static int packet_receive(PORT port,
						  UCHAR * buffer,
						  SSHORT buffer_length, SSHORT * length) {
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
UCHAR c, aux_buffer[1024], *p; USHORT l, n; int status, flags;
/* If this is a multi-client case, as the message has already been
   received, there is nothing to do */
flags = port->port_flags; if (port->port_server_flags & SRVR_multi_client) {
while (!(status = port->port_iosb[0]))
wait(port->port_iosb); if (status & 1) {
l = port->port_iosb[1];
p = port->port_buffer + 2 * BUFFER_SIZE;
memcpy(buffer, p, l);
*length = l; start_read(port, p, BUFFER_SIZE); return TRUE;}
}
else
{
if (!start_read(port, buffer, buffer_length))
return NULL; wait(port->port_iosb); status = port->port_iosb[0];}

if (!(status & 1)) {
if (flags & PORT_async)
return FALSE; return dnet_error(port, "sys$qio/io$_readvblk", status);}

*length = port->port_iosb[1]; return TRUE;}


static int packet_send(PORT port, UCHAR * data, SSHORT length) {
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
int status; SSHORT iosb[4]; status = sys$qio(EVENT_FLAG,	/* Event flag */
											 port->port_handle,	/* Channel */
											 IO$_WRITEVBLK,	/* Function */
											 iosb,	/* IO status block */
											 gds__completion_ast,	/* AST address */
											 NULL,	/* AST parameter */
											 data,	/* P1 (buffer) */
											 length,	/* P2 (length) */
											 NULL,	/* P3 (virtual block) */
											 NULL, NULL, NULL);
if (status & 1) {
wait(iosb); status = iosb[0];}

if (!(status & 1))
return dnet_error(port, "sys$qio/io$_writevblk", status);
port->port_flags &= ~PORT_pend_ack; return TRUE;}


static int process_connect(PORT port) {
/**************************************
 *
 *	p r o c e s s _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	We've got a message on the mailbox.  Find out
 *	what's up and doing something appropriate.
 *
 **************************************/
MBXMSG msg;
PORT new_port;
int status;
USHORT channel, iosb[4];
msg = (MSG) port->port_buffer; switch (msg->msgtype) {
case MSG$_CONNECT:
if (!(channel = accept_link(port, msg)))
return NULL;
new_port = alloc_port(port);
new_port->port_handle = channel;
start_read(new_port, new_port->port_buffer + 2 * BUFFER_SIZE, BUFFER_SIZE);
break;}

start_read(port, port->port_buffer, maxmsg);}


static PORT receive(PORT port, PACKET * packet) {
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
PORT active; int status; USHORT iosb[4];
/* Single client case is MUCH easier */
if (!(port->port_server_flags & SRVR_multi_client)) {
if (!xdr_protocol(&port->port_receive, packet))
return NULL; return port;}

/* Worry about multi-client case */

for (;;) {
if (!(active = active_port(port))) {
THREAD_EXIT; gds__thread_wait(active_port, port); THREAD_ENTER; continue;}
if (active != port) {
if (!xdr_protocol(&active->port_receive, packet))
packet->p_operation = op_exit; return active;}
process_connect(port);}

return NULL;}


static int send_full(PORT port, PACKET * packet) {
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
return FALSE; return dnet_endofrecord(&port->port_send, TRUE);}


static int send_partial(PORT port, PACKET * packet) {
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

return xdr_protocol(&port->port_send, packet);}


static int start_read(PORT port, UCHAR * buffer, SSHORT buffer_length) {
/**************************************
 *
 *	s t a r t _ r e a d
 *
 **************************************
 *
 * Functional description
 *	Start an asynchronous read on a port.
 *
 **************************************/
int status; status = sys$qio(EVENT_FLAG,	/* Event flag */
							 port->port_handle,	/* Channel */
							 IO$_READVBLK,	/* Function */
							 port->port_iosb,	/* IO status block */
							 gds__completion_ast,	/* AST address */
							 NULL,	/* AST parameter */
							 buffer,	/* P1 (buffer) */
							 buffer_length,	/* P2 (length) */
							 NULL,	/* P3 (virtual block) */
							 NULL, NULL, NULL); if (!(status & 1)) {
if (port->port_flags & PORT_async)
return FALSE; return dnet_error(port, "sys$qio/io$_readvblk", status);}

return TRUE;}


static void wait(USHORT * iosb) {
/**************************************
 *
 *	w a i t
 *
 **************************************
 *
 * Functional description
 *	Wait for something to happen.
 *
 **************************************/

if (iosb[0])
return; THREAD_EXIT; ISC_wait(iosb, EVENT_FLAG); THREAD_ENTER;}


static int wait_and_test(PORT port, TEXT * string, int status) {
/**************************************
 *
 *	w a i t _ a n d _ t e s t
 *
 **************************************
 *
 * Functional description
 *	Do status checking and waiting associated with a QIO.  If
 *	anything fails, issue error and return FALSE/NULL.
 *
 **************************************/

if (status & 1) {
wait(port->port_iosb); status = port->port_iosb[0];}

if (!(status & 1))
return dnet_error(port, string, status); return TRUE;}
