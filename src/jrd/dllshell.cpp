/*
 *      PROGRAM:        JRD Access Method
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
 */

#include <windows.h>
#include <signal.h>

static void fpe_handler(int);
static int fpe_count;

/* define a global instance handle */
HINSTANCE hDLLInstance;

#if !defined(__WIN32__) && !defined(WIN32) && !defined(_WIN32) && !defined(WIN_NT)
#pragma argsused
int FAR PASCAL LibMain(HANDLE hInstance, WORD wDataSegment,
					   WORD wHeapSize, LPSTR lpszCmdLine)
{
	/* initialize the global instance handle */
	hDLLInstance = hInstance;

	/* The startup code for the DLL initializes the local heap (if there is one)
	   with a call to LocalInit which locks the data segment. */
	if (wHeapSize != 0)
		UnlockData(0);

	/* init the floating point exception count and set the handler */

	fpe_count = 0;
	signal(SIGFPE, fpe_handler);

	return 1;
}

#pragma argsused
int FAR PASCAL WEP(int bSystemExit)
{
	LOCAL_wep();
	return 1;
}

static void fpe_handler(int signal_number)
{
	signal(SIGFPE, fpe_handler);	/* reset handler */
	fpe_count++;				/* count the event */
}

#endif /* !__WIN32__ */
