/*
 *	PROGRAM:	JRD Remote Server
 *	MODULE:		nwserv.c
 *	DESCRIPTION:	Netware remote server.
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

#include <signal.h>
#include <process.h>
#include "../remote/remote.h"
#include "../jrd/license.h"
#include "../jrd/thd.h"
#include "../remote/inet_proto.h"
#include "../remote/serve_proto.h"
#include "../remote/spxne_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/sch_proto.h"

static void inet_connect_wait_thread(void);
static void nlm_terminate(int);
static void spx_connect_wait_thread(void);
static void spx_process_connect_thread(PORT);

int regular_malloc = 0;

extern int FAUX_check();
int NWSERV_terminating = FALSE;
int NWSERV_spx_done = FALSE;

static TEXT protocol[128];
static USHORT SERVER_flag;


int CLIB_ROUTINE main( int argc, char **argv)
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Run the server
 *
 **************************************/
	PORT port;
	STATUS status_vector[20];
	TEXT *p, c;

	SERVER_flag |= SRVR_spx;
	SERVER_flag |= SRVR_inet;
	SERVER_flag |= SRVR_multi_client;
	protocol[0] = 0;

	argv++;
	while (--argc) {
		p = *argv++;
		if (*p++ == '-')
			while (c = *p++)
				switch (UPPER(c)) {
				case 'T':
					SERVER_flag |= SRVR_inet;
					break;

				case 'M':
					regular_malloc = 1;
					break;

				case 'Z':
					ib_printf("Interbase remote server version %s\n",
							  GDS_VERSION);
					exit(FINI_OK);
				}
	}

	if (SPXNET_init()) {
		SPXNET_shutdown();
		exit(FINI_OK);
	}

#ifdef NETWARE_386
	if (FAUX_check() == -1)
		SERVER_flag ^= SRVR_inet;
#endif

	gds__thread_enable(-1);
	ISC_sync_init();

	if (SERVER_flag & SRVR_inet)
		gds__thread_start(inet_connect_wait_thread, 0, 0, 0, 0);

	if (SERVER_flag & SRVR_spx)
		gds__thread_start(spx_connect_wait_thread, 0, 0, 0, 0);

	DestroyScreen(GetCurrentScreen());
	signal(SIGTERM, nlm_terminate);
	ExitThread(TSR_THREAD, 0);
	ExitThread(EXIT_NLM, 1);

	exit(FINI_OK);
}


static void inet_connect_wait_thread(void)
{
/**************************************
 *
 *	i n e t _ c o n n e c t _ w a i t _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	STATUS status_vector[20];
	PORT port;

	THREAD_ENTER;
	port = INET_connect(protocol, 0, status_vector, SERVER_flag, 0, 0);
	THREAD_EXIT;
	if (port)
		SRVR_multi_thread(port, SERVER_flag);
	else
		gds__log_status(0, status_vector);
}


static void nlm_terminate( int sig)
{
/**************************************
 *
 *	n l m _ t e r m i n a t e
 *
 **************************************
 *
 * Functional description
 *
 * Inform the world that we are going
 * away, shut down SPX, then wait to
 * make sure that SPX is really shut
 * down, then let everything die.
 **************************************/
	int i;

	NWSERV_terminating = TRUE;
	SPXNET_shutdown();
	for (i = 10; !NWSERV_spx_done && i; i--)
		delay(500);
}


static void spx_connect_wait_thread(void)
{
/**************************************
 *
 *	s p x _ c o n n e c t _ w a i t _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	STATUS status_vector[20];
	PORT port;

	while (!NWSERV_terminating) {
		THREAD_ENTER;
		port = SPXNET_listen(protocol, 0, status_vector, SERVER_flag);
		THREAD_EXIT;
		if (port)
			gds__thread_start(spx_process_connect_thread, port, 0, 0, 0);
		else {
			EnterCritSec();
			gds__log_status(0, status_vector);
			ExitCritSec();
		}
	}
	NWSERV_spx_done = TRUE;
}


static void spx_process_connect_thread( PORT port)
{
/**************************************
 *
 *	s p x _ p r o c e s s _ c o n n e c t _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	SRVR_main(port, SERVER_flag & ~SRVR_multi_client);
}
