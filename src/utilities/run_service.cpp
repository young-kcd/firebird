/*
 *	PROGRAM:	Service manager
 *	MODULE:		run_service.c
 *	DESCRIPTION:	Run a utility as an Interbase service
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
#include <string.h>
#include "../jrd/common.h"
#include "../jrd/gds.h"
#include "../jrd/gds_proto.h"

static SCHAR recv_items[] = { gds__info_svc_to_eof };
static SCHAR send_timeout[] = { gds__info_svc_timeout, 1, 0, 30 };


int CLIB_ROUTINE main( int argc, char *argv[])
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Initialize lock manager for process.
 *
 **************************************/
	SCHAR *service_path;
	SCHAR *spb, spb_buffer[2048], *p, *send_items, send_buffer[2048],
		buffer[2048], item;
	SLONG *handle;
	SSHORT send_item_length, len;

	if (argc < 2) {
		ib_printf("usage: run_service service_path [args]\n");
		exit(FINI_ERROR);
	}

	service_path = argv[1];

	spb = spb_buffer;
	if (argc > 2) {
		*spb++ = gds__spb_version1;
		*spb++ = gds__spb_command_line;
		spb++;
		for (argv += 2, argc -= 2; argc--;) {
			for (p = *argv++; *spb = *p++; spb++);
			*spb++ = ' ';
		}
		*--spb = 0;
		spb_buffer[2] = strlen(spb_buffer + 3);
	}

	handle = NULL;
	isc_service_attach((SLONG *) 0, 0, service_path, &handle,
					   (SSHORT) (spb - spb_buffer), spb_buffer);

	if (strstr(service_path, "start_cache")) {
		send_items = send_timeout;
		send_item_length = sizeof(send_timeout);
	}
	else {
		send_items = send_buffer;
		send_item_length = 0;
	}

	if (send_item_length)
		ib_printf
			("It will take about 30 seconds to confirm that the cache manager\nis running.  Please wait...\n");

	item = gds__info_end;
	do {
		isc_service_query((SLONG *) 0, &handle, send_item_length, send_items,
						  sizeof(recv_items), recv_items, sizeof(buffer),
						  buffer);
		if (send_items == send_buffer)
			send_item_length = 0;
		p = buffer;
		while (p < buffer + sizeof(buffer) &&
			   (item = *p) != gds__info_end &&
			   item != gds__info_truncated && item != gds__info_svc_timeout) {
			len = gds__vax_integer(p + 1, 2);
			p += 2;
			while (len--) {
				p++;
				if (*p != '\001')
					ib_putchar(*p);
			}
			if (*p++ == '\001') {
				send_buffer[0] = gds__info_svc_line;
				ib_fgets(send_buffer + 3, sizeof(send_buffer) - 3, ib_stdin);
				len = strlen(send_buffer + 3);
				send_buffer[1] = len;
				send_buffer[2] = len >> 8;
				send_item_length = len + 3;
			}
		}
	} while (item == gds__info_truncated
			 || (send_items == send_buffer && send_item_length));

	isc_service_detach((SLONG *) 0, &handle);

	exit(FINI_OK);
}
