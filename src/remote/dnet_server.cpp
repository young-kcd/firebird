/*
 *	PROGRAM:	JRD Remote Server
 *	MODULE:		dnet_server.c
 *	DESCRIPTION:	DECnet remote server.
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
#include "../jrd/common.h"
#include "../jrd/license.h"
#include "../remote/remote.h"
#include "../remote/dnet_proto.h"
#include "../remote/serve_proto.h"
#include "../jrd/gds_proto.h"


void main( int argc, char *argv[])
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Run the server with DECnet.
 *
 **************************************/
	PORT port;
	int debug, flag, standalone;
	STATUS status_vector[20];
	TEXT *p, **end, c, protocol[128];

	end = argc + argv;
	argv++;
	debug = standalone = flag = FALSE;
	protocol[0] = 0;

	while (argv < end) {
		p = *argv++;
		if (*p++ == '-')
			while (c = *p++)
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
					sprintf(protocol, "/%s", *argv++);
					break;

				case 'Z':
					ib_printf("Interbase DECnet server version %s\n",
							  GDS_VERSION);
					exit(FINI_OK);
				}
	}

	if (standalone) {
		port = DNET_connect(protocol, 0, status_vector, flag);
		if (!port) {
			gds__print_status(status_vector);
			exit(FINI_ERROR);
		}
	}
	else
		port = DNET_server(1);

	SRVR_main(port, flag);

	exit(FINI_OK);
}
