/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		DECNET_server.c
 *	DESCRIPTION:	DECNET server for VMS
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
#include "../jrd/thd.h"
#include "../remote/remote.h"
#include "../remote/decne_proto.h"
#include "../remote/serve_proto.h"
#include "../jrd/gds_proto.h"


int main( int argc, char **argv)
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Run an asyncronous server.
 *
 **************************************/
	int *port;
	int debug, flag, channel, standalone, multi_threaded;
	STATUS status_vector[20];
	TEXT c, *p, **end, protocol[128];
	SLONG debug_value;

	argc = VMS_parse(&argv, argc);
	end = argc + argv;
	argv++;
	debug = standalone = flag = FALSE;
	channel = 0;
	protocol[0] = 0;
	multi_threaded = FALSE;

	while (argv < end) {
		p = *argv++;
		if (*p++ = '-')
			while (c = *p++)
				switch (UPPER(c)) {
				case 'D':
					flag |= SRVR_debug;
					debug = standalone = TRUE;
					break;

				case 'M':
					flag |= SRVR_multi_client;
					standalone = TRUE;
					break;

				case 'S':
					standalone = TRUE;
					break;

				case 'I':
					standalone = FALSE;
					break;

				case 'T':
					multi_threaded = TRUE;
					break;

				case 'U':
					multi_threaded = FALSE;
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

	THREAD_ENTER;
	port = DECNET_connect(protocol, NULL, status_vector, flag);
	THREAD_EXIT;

	if (!port) {
		gds__print_status(status_vector);
		exit(FINI_ERROR);
	}

	if (multi_threaded)
		SRVR_multi_thread(port, flag);
	else
		SRVR_main(port, flag);

	exit(FINI_OK);
}
