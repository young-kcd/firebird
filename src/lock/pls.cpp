/*
 *	PROGRAM:	JRD Lock Manager
 *	MODULE:		pls.c
 *	DESCRIPTION:	Page lock server interface
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
#include "lock.h"
#include "plserver.h"

#include "/sys/ins/streams.ins.c"
#include "/sys/ins/ms.ins.c"
#include "/sys/ins/name.ins.c"
#include "/sys/ins/proc2.ins.c"
#include "/sys/ins/ipc.ins.c"

#define ALLOC		pls_alloc
#define DATAGRAM_RETRY	10

/* Connection block for page/lock server */

typedef struct plc {
	struct blk plc_header;
	struct pld *plc_next;		/* Next connection */
	int plc_node;				/* Node id of connection */
	int plc_send_sequence;		/* Next available packet sequence number */
	int plc_recv_sequence;		/* Sequence number of last packet received */
	UCHAR plc_socket[20];		/* Apollo socket handle */
	UCHAR plc_socket2[20];		/* Our socket handle */
	int plc_connection_handle;
	int plc_file_handle;
	USHORT plc_length;			/* Length of file name */
	UCHAR plc_file[1];			/* Socket file name */
} *PLC;

static int connect(int, ipc_$socket_handle_t *);
static int error(UCHAR *, UCHAR *, status_$t);
static UCHAR *pls_alloc(USHORT);
static void pls_release(UCHAR *);
static void move(register SCHAR *, register SCHAR *, register SSHORT);

static PLC connections;

#define NODE_MASK	0xFFFFF


/* Asknode Stuff */

std_$call void asknode_$info();

typedef SSHORT enum {
	ask_who, who_r,
	ask_time, time_r,
	ask_node_root, node_root_r
} asknode_$kind_t;

#define asknode_$version	2

typedef struct {
	pinteger version;
	asknode_$kind_t kind;
	status_$t status;
	uid_$t uid;
	SCHAR stuff[256];			/* Random and various */
} asknode_$reply_t;

/* name_$ stuff */

std_$call void name_$gpath_dir();

SSHORT enum name_$namtyp_t {
	name_$junk_flag,
	name_$wdir_flag,
	name_$ndir_flag,
	name_$node_flag,
	name_$root_flag,
	name_$node_data_flag
};


PLC PLS_connection(int node, USHORT server_flag)
{
/**************************************
 *
 *	P L S _ c o n n e c t i o n
 *
 **************************************
 *
 * Functional description
 *	Establish a connection to a page/lock server on a given node.
 *
 **************************************/
	PLC connection;
	PHD header, reply;
	UCHAR socket_name[32];
	USHORT length;
	uid_$t uid;
	status_$t status;
	ipc_$socket_handle_t handle, local_handle;
	asknode_$reply_t node_info;

	if (connect(node, &handle))
		return NULL;

	if (connect(NULL, &local_handle))
		return NULL;

/* We seem to have a socket -- time to make a connection block */

	connection = ALLOC(sizeof(struct plc) + length);
	connection->plc_length = length;
	connection->plc_node = node;
	strcpy(connection->plc_file, socket_name);
	move(&handle, connection->plc_socket, sizeof(handle));
	move(&local_handle, connection->plc_socket2, sizeof(handle));

/* We at least have a socket handle.  See if anybody is really there */

	header.phd_type = PL_CONNECT;
	proc2_$who_am_i(uid);
	header.phd_misc = uid.low & NODE_MASK;
	header.phd_flags = (server_flag) ? PHD_SERVER : 0;
	status.all =
		PLS_request(connection, &header, sizeof(header), &reply,
					sizeof(reply), NULL, 0, 0);

	if (status.all) {
		PLS_disconnect(connection);
		return NULL;
	}

	connection->plc_connection_handle = reply.phd_handle;

	return connection;
}


PLC PLS_find_connection(int node, USHORT server_flag)
{
/**************************************
 *
 *	P L S _ f i n d _ c o n n e c t i o n
 *
 **************************************
 *
 * Functional description
 *	Find a connection if it exists, otherwise create it.
 *	Note: This is used only by the page/lock server to
 *	avoid problems of dirty statics in Apollo installed images.
 *
 **************************************/
	PLC connection;

/* See if connection exits */

	for (connection = connections; connection;
		 connection = connection->plc_next) if (connection->plc_node == node)
			return connection;

/* Connection doesn't exist -- try to make it */

	if (!(connection = PLS_connection(node, server_flag)))
		return NULL;

	connection->plc_next = connections;
	connections = connection;

	return connection;
}


int PLS_request(
				PLC connection,
				PHD * header,
				USHORT header_len,
				PHD * reply,
				USHORT reply_len,
UCHAR * data, USHORT send_len, USHORT receive_len)
{
/**************************************
 *
 *	P L S _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Send a page/lock request and get response.
 *
 **************************************/
	USHORT send, wait, count, receive_length, header_length, next;
	status_$t status;
	ipc_$socket_handle_t handle;

/* Send the request until we get something back */

	header->phd_connection = connection->plc_connection_handle;
	BUMP_SEQUENCE(connection->plc_send_sequence);
	header->phd_sequence = connection->plc_send_sequence;
	send = TRUE;
	wait = 10 * 4;

	for (count = 0; count < DATAGRAM_RETRY; count++) {
		if (send) {
			header->phd_ack = connection->plc_recv_sequence;
			ipc_$send(connection->plc_socket,
					  connection->plc_socket2,
					  *header, header_len, *data, send_len, status);
			if (status.all)
				return status.all;
		}

		/* Wait for and read reply */

		ipc_$wait(connection->plc_socket2, wait, status);
		if (status.all == ipc_$timeout)
			continue;
		if (status.all)
			return status.all;
		ipc_$rcv(connection->plc_socket2, header_len,	/* out-hdr-buflen */
				 receive_len,	/* out-data-buflen */
				 handle, *reply,	/* out-hdr-buf */
				 reply_len,		/* out-hdr-length */
				 *data,			/* out-data-buf */
				 receive_length,	/* out-data-length */
				 status);		/* status */
		if (status.all)
			return status.all;

		connection->plc_recv_sequence = reply->phd_sequence;

		/* If the packet acknowleges something other than the one sent, re-transmit */

		if (reply->phd_ack != connection->plc_send_sequence) {
			send = TRUE;
			continue;
		}

		/* If this is something other than an acknowlegement, be patient */

		if (reply->phd_type != PL_ACK)
			return FB_SUCCESS;

		/* If we got an acknowlegement, the other guy has at least received the packet.  
		   We should be more patient, but still probe looking for death */

		wait = 30 * 4;
		send = FALSE;
	}
}


void PLS_disconnect( PLC connection)
{
/**************************************
 *
 *	P L S _ d i s c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Break a remote connection.
 *
 **************************************/
	status_$t status;

	ipc_$close(connection->plc_file, connection->plc_length, status);
	if (status.all)
		error_$print(&status);

	ipc_$delete(connection->plc_file, connection->plc_length, status);
	if (status.all)
		error_$print(&status);

/* Close a client channel */

	pls_release(connection);
}


static int connect( int node, ipc_$socket_handle_t * handle)
{
/**************************************
 *
 *	c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Try to establish a physical connection to the page
 *	lock server on a given node.
 *
 **************************************/
	PHD header, reply;
	UCHAR socket_name[32], root_name[256], *p;
	USHORT length;
	status_$t status;
	asknode_$reply_t node_info;

/* If no node id was given, create a connection for ourselves */

	if (!node) {
		strcpy(socket_name, "gds.pl_server_XXXXXX");
		mktemp(socket_name);
		length = strlen(socket_name);
		ipc_$create(socket_name, length, status);
		if (status.all)
			return error("ipc_$create", socket_name, status);
		ipc_$open(socket_name, length, 4, *handle, status);
		if (status.all)
			return error("ipc_$open", socket_name, status);
		unlink(socket_name);
		return FB_SUCCESS;
	}

/* File is open by another node.  Sigh.  Make up a nice filename */

	asknode_$info(ask_node_root, node, 0, node_info, status);
	if (status.all)
		return error("asknode_$info", " ", status);

	if (node_info.status.all)
		return error("asknode_$info", "remote site", node_info.status);

	name_$gpath_dir(node_info.uid, name_$root_flag, root_name, length,
					status);
	if (status.all)
		return error("name_$gpath_dir", "", status);

	root_name[length] = 0;
	sprintf(socket_name, "%s/sys/gds.plserver_%x", root_name, node);
	length = strlen(socket_name);

/* See if we can get a socket for the server.  If not, don't bother
   continuing this non-sense */

	ipc_$resolve(socket_name, length, *handle, status);
	if (status.all) {
		for (p = socket_name + 2; *p && *p != '/'; p++)
			if (*p >= 'A' && *p <= 'Z')
				*p = *p - 'A' + 'a';
		ipc_$resolve(socket_name, length, *handle, status);
	}

	if (status.all)
		return error("ipc_$resolve", socket_name, status);

	return FB_SUCCESS;
}


static int error( UCHAR * operation, UCHAR * object, status_$t status)
{
/**************************************
 *
 *	e r r o r
 *
 **************************************
 *
 * Functional description
 *	A system service error has occurred.
 *
 **************************************/

	ib_printf("Page/lock service error occurred during %s on \"%s\"\n",
			  operation, object);
	error_$print(status);

	return status.all;
}


static UCHAR *pls_alloc( USHORT size)
{
/**************************************
 *
 *	p l s _ a l l o c
 *
 **************************************
 *
 * Functional description
 *	Allocate a block.
 *
 **************************************/
	UCHAR *blk, *p, *end;

	blk = p = gds__alloc(size);
	end = p + size;

	while (p < end)
		*p++ = 0;

	return blk;
}


static void pls_release( UCHAR * blk)
{
/**************************************
 *
 *	p l s _ r e l e a s e
 *
 **************************************
 *
 * Functional description
 *	Release a block.
 *
 **************************************/

	gds__free(blk);
}


static void move(
				 register SCHAR * from,
				 register SCHAR * to, register SSHORT length)
{
/**************************************
 *
 *	m o v e
 *
 **************************************
 *
 * Functional description
 *	Move a byte string as fast as possible.
 *
 **************************************/

	if (length)
		do
			*to++ = *from++;
		while (--length);
}
