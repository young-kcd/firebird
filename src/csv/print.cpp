/*
 *	PROGRAM:	Central Server
 *	MODULE:		print.c
 *	DESCRIPTION:	Global region print utility
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
#include "../csv/csi.h"
#include "../csv/css_proto.h"
#include "../jrd/gds_proto.h"

static void prt_que(UCHAR *, SRQ *);

static CSH CSS_header;


void main( int argc, char **argv)
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
	SRQ *que;
	HDR *block;
	PRB process;
	MSG message;
	FRB free;
	CNCT connection;
	SLONG offset;
	STATUS status_vector[20];

	if (!(CSS_header = CSS_init(status_vector, TRUE))) {
		printf("Can't access global region\n");
		gds__print_status(status_vector);
		exit(FINI_ERROR);
	}

	printf("%.5d GLOBAL REGION HEADER\n", 0);
	printf("\tLength: %ld, version: %d, free: %ld\n", CSS_header->csh_length,
		   CSS_header->csh_version, CSS_header->csh_free);
	printf("\tSemid: %ld, current process: %ld\n",
		   CSS_header->csh_semid, CSS_header->csh_current_process);
	prt_que("\tProcesses", &CSS_header->csh_processes);

	for (offset = sizeof(struct csh); offset < CSS_header->csh_length;
		 offset += block->hdr_length) {
		printf("\n%.5ld ", offset);
		block = (HDR *) ABS_PTR(offset);
		switch (block->hdr_type) {
		case type_prb:
			printf("PROCESS_BLOCK (%ld)\n", block->hdr_length);
			process = (PRB) block;
			printf("\tProcess_id: %ld, flags: %d, protocol: %d\n",
				   process->prb_process_id, process->prb_flags,
				   process->prb_protocol_version);
			printf("\tSemaphore: %d, group_id: %ld, number: %ld\n",
				   process->prb_semaphore, process->prb_group_id,
				   process->prb_process_number);
			prt_que("\tProcesses", &process->prb_processes);
			prt_que("\tConnections", &process->prb_connections);
			prt_que("\tMessages", &process->prb_messages);
			break;

		case type_frb:
			printf("FREE BLOCK (%ld)\n", block->hdr_length);
			free = (FRB) block;
			printf("\tNext: %ld\n", free->frb_next);
			break;

		case type_msg:
			printf("MSG (%ld)\n", block->hdr_length);
			message = (MSG) block;
			printf("\tType: %d, connection: %ld\n", message->msg_type,
				   message->msg_connection);
			prt_que("\tMsg que", &message->msg_que);
			break;

		case type_cnct:
			printf("CONNECTION (%ld)\n", block->hdr_length);
			connection = (CNCT) block;
			printf("\tParent: %ld, partner: %ld, mirror: %ld\n",
				   connection->cnct_parent, connection->cnct_partner,
				   connection->cnct_mirror);
			prt_que("\tConnections", &connection->cnct_connections);
			break;

		default:
			printf("*** UNKNOWN *** (%ld)\n", block->hdr_length);
			break;
		}
		if (!block->hdr_length)
			break;
	}

	exit(FINI_OK);
}


static void prt_que( UCHAR * string, SRQ * que)
{
/**************************************
 *
 *	p r t _ q u e
 *
 **************************************
 *
 * Functional description
 *	Print the contents of a self-relative que.
 *
 **************************************/
	SLONG offset;

	offset = REL_PTR(que);

	if (offset == que->srq_forward && offset == que->srq_backward)
		printf("%s: *empty*\n", string);
	else
		printf("%s: forward: %d, backward: %d\n",
			   string, que->srq_forward, que->srq_backward);
}
