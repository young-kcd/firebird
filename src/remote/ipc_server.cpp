/*
 *	PROGRAM:	JRD Remote Server
 *	MODULE:		ipc_server.c
 *	DESCRIPTION:	MPE/XL remote server
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

#include "../jrd/ib_stdio.h"

#include "../jrd/common.h"
#include "../jrd/license.h"
#include "../remote/remote.h"
#include "../remote/ipc.h"
#include "../remote/ipc_proto.h"
#include "../remote/serve_proto.h"
#include "../jrd/gds_proto.h"

static void enable_signals(void);
static void excp_handler(void);


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
	int flag, standalone;
	STATUS status_vector[20];
	TEXT *p, **end, c, protocol[128], *server_arg;

	end = argc + argv;
	debug = standalone = FALSE;
	protocol[0] = 0;
	server_arg = NULL;
	flag = SRVR_mpexl;

	while (++argv < end) {
		p = *argv;
		if (*p != '-')
			server_arg = p;
		else
			while (c = *++p)
				switch (UPPER(c)) {
				case 'D':
					flag |= SRVR_debug;
					debug = standalone = TRUE;
					break;

				case 'S':
					standalone = TRUE;
					break;

				case 'I':
					standalone = FALSE;
					break;

				case 'P':
					sprintf(protocol, "/%s", *++argv);
					break;

				case 'Z':
					ib_printf("Interbase TCP/NETipc server version %s\n",
							  GDS_VERSION);
					exit(FINI_OK);
				}
	}

	if (standalone || debug) {
		port = IPC_connect(protocol, 0, status_vector, flag);
		if (!port) {
			gds__print_status(status_vector);
			exit(FINI_ERROR);
		}
	}
	else if (!(port = IPC_server(server_arg)))
		exit(FINI_ERROR);

/* Establish signal handlers so the access method won't have to */

	enable_signals();

	SRVR_main(port, flag);

	exit(FINI_OK);
}


static void enable_signals(void)
{
/**************************************
 *
 *	e n a b l e _ s i g n a l s
 *
 **************************************
 *
 * Functional description
 *	Enable signals.
 *
 **************************************/
	SLONG mask;
	void (*prev_handler) ();

/* Set up to catch floating overflow, underflow, and divide traps */

	HPENBLTRAP(0, &mask);
	mask |=
		(1 << 16) | (1 << 5) | (1 << 3) | (1 << 15) | (1 << 6) | (1 << 2) | (1
																			 <<
																			 17)
		| (1 << 7) | (1 << 0);
	HPENBLTRAP(mask, &mask);
	mask |=
		(1 << 16) | (1 << 5) | (1 << 3) | (1 << 15) | (1 << 6) | (1 << 2) | (1
																			 <<
																			 17)
		| (1 << 7) | (1 << 0);
	XARITRAP(mask, (int) excp_handler, &mask, (int *) &prev_handler);
}


static void excp_handler(void)
{
/**************************************
 *
 *	e x c p _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Catch arithmetic exception.
 *
 **************************************/
}
