/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		mgr.c
 *	DESCRIPTION:	Remote Manager
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

#include "../jrd/y_ref.h"
#include "../jrd/ibase.h"
#include "../remote/remote.h"
#include "../jrd/license.h"
#include "../remote/ambx_proto.h"


void main( int argc, char *argv[])
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	PORT port;
	UCHAR mailbox[256];

	AMBX_mailbox("//frank", mailbox);
	port = init_connection(mailbox);
}


static PORT init_connection( UCHAR * node)
{
/**************************************
 *
 *	i n i t _ c o n n e c t i o n
 *
 **************************************
 *
 * Functional description
 *	Establish connection to remote server.
 *
 **************************************/
	PORT port;
	PACKET packet;
	P_CNCT *connect;
	ISC_STATUS_ARRAY status_vector;
	p_cnct::p_cnct_repeat * protocol;

/* Establish connection to server.  Start by telling the server about
   the protocols that we know.  The server will then pick at most one. */

	connect = &packet.p_cnct;
	packet.p_operation = op_connect;
	connect->p_cnct_operation = op_attach;
	connect->p_cnct_cversion = CONNECT_VERSION2;
	connect->p_cnct_client = ARCHITECTURE;
	connect->p_cnct_file.cstr_length = 0;
	connect->p_cnct_file.cstr_address = 0;
	connect->p_cnct_user_id.cstr_length = 0;
	connect->p_cnct_user_id.cstr_address = NULL;
	protocol = connect->p_cnct_versions;
	connect->p_cnct_count = 1;

	protocol->p_cnct_version = PROTOCOL_VERSION4;
	protocol->p_cnct_architecture = arch_generic;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = ptype_batch_send;
	protocol->p_cnct_weight = 2;

/* If we can't talk to a server, punt.  Let somebody else generate
   an error. */

	if (!(port = AMBX_connect(node, strlen(node), &packet, status_vector)))
		return NULL;

/* Get response packet from server. */

	port->receive(&packet);

	if (packet.p_operation != op_accept) {
		ib_fprintf(ib_stderr, "remote server refused connection\n");
		return NULL;
	}

/* The server has picked a protocol.  Find out which. */

	port->port_protocol = packet.p_acpt.p_acpt_version;

	if (packet.p_acpt.p_acpt_architecture == ARCHITECTURE)
		port->port_flags |= PORT_symmetric;

	if (packet.p_acpt.p_acpt_type == ptype_rpc)
		port->port_flags |= PORT_rpc;

	return port;
}
