/*
 *      PROGRAM:        JRD Remote Interface/Server
 *      MODULE:         dllshell.c
 *      DESCRIPTION:    Win16 DLL entry and exit functions
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
 *
 * 2002.10.29 Sean Leyne - Removed support for obsolete IPX/SPX Protocol
 *
 */

#include "firebird.h"
#include "../jrd/common.h"
#include <windows.h>
#include <winsock2.h>
#include <signal.h>
#include "../jrd/gds_proto.h"


/* The DLL's instance handle.  This is used to create windows later. */
HINSTANCE hInstance;

/* floating point exception counter */

static int fpe_count;

/* static function prototypes */

static void fpe_handler(int);
LRESULT FAR PASCAL inet_wndproc(HWND hwnd, UINT message, WPARAM wParam,
								LPARAM lParam);
void wfwnp_cleanup(void *arg);
void inet_cleanup(void *arg);

// Every DLL has an entry point LibMain and an exit point WEP.
int FAR PASCAL LibMain(HINSTANCE hInst, WORD wDataSegment,
					   WORD wHeapSize, LPSTR lpszCmdLine)
{
	WNDCLASS wndclass_tcp;
	hInstance = hInst;

	// The startup code for the DLL initializes the local heap (if there is one)
	// with a call to LocalInit which locks the data segment.
	if (wHeapSize != 0)
		UnlockData(0);

	/* Define an invisible window class used for receiving winsock messages */
	wndclass_tcp.style = CS_HREDRAW;
	wndclass_tcp.lpfnWndProc = inet_wndproc;
	wndclass_tcp.cbClsExtra = 0;
	wndclass_tcp.cbWndExtra = sizeof(void FAR *);
	wndclass_tcp.hInstance = hInstance;
	wndclass_tcp.hIcon = NULL;
	wndclass_tcp.hCursor = NULL;
	wndclass_tcp.hbrBackground = NULL;
	wndclass_tcp.lpszMenuName = NULL;
	wndclass_tcp.lpszClassName = "RemoteMsgClass";

	/* ***CM - Add error handling here! */
	RegisterClass(&wndclass_tcp);

	/* init the floating point exception count and set the handler */

	fpe_count = 0;
	signal(SIGFPE, fpe_handler);

	return 1;
}

int FAR PASCAL WEP(int bSystemExit)
{
/* 
** Until the gds__cleanup processing is fixed,
** or Windows will crash when we exit.
** Same for named pipes under Windows.
** For TCP/IP we must unload Winsock.dll.
*/
	TRACE("Called remote\\dllshell.c WEP()...\n");
	TRACE("remote\\dllshell.c WEP:  calling wfwnp_cleanup()\n");
	wfwnp_cleanup(NULL);
	TRACE("remote\\dllshell.c WEP:  calling inet_cleanup()\n");
	inet_cleanup(NULL);

	TRACE("Returning from remote\\dllshell.c WEP()...\n");
	return 1;
}

HINSTANCE LoadDll(UCHAR * name)
{
	HINSTANCE hlib;
	UINT fuErrorMode;

	fuErrorMode = SetErrorMode(SEM_NOOPENFILEERRORBOX);

	hlib = LoadLibrary(name);

	(void) SetErrorMode(fuErrorMode);

	if (!hlib) {
		gds__log("Windows error %d while attempting to load %s",
				 GetLastError(), name);
		return 0;
	}
	else
		return hlib;

}

static void fpe_handler(int signal_number)
{
	signal(SIGFPE, fpe_handler);	/* reset handler */
	fpe_count++;				/* count the event */
}
