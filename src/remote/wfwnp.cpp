/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		wfwnp.c
 *	DESCRIPTION:	Windows for Workgroups Named Pipe Support module.
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

/*
 *  Please note that the following implementation of Windows NT named pipe
 *  support functions is only a partial implementation of these functions.
 *  This is only a wrapping of these functions to enable existing NT code
 *  to function properly.  This code should be studied and modified if
 *  necessary when any new calls to these functions are made.  In other 
 *  words, use these functions within Windows 3.11 at your own risk.
 *  In addition, unlike Windows NT, the error code is reset after each
 *  function call to avoid instancing problems.  This means GetLastError()
 *  returns NO_ERROR after a successful function call even if the preceding
 *  function resulted in an error.
 *  
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <nmpipe.h>
#define INCL_ERRORS
#include <bseerr.h>
#include "../jrd/ibase.h"
#include "../remote/remote.h"
#include "../remote/wfwnp.h"
#include "../remote/wfwnp_pr.h"
#include "../jrd/gds_proto.h"

#define IO_READ     0
#define IO_WRITE    1

/* 
** The windows netapi library has to be dynamically loaded because the
** library may not be installed if the user does not have name pipe support.
** they are using SPX).  The following defines the netapi functions to
** reference function pointers which will be set using the windows
** GetProcAddress function at startup.
*/
#define DosReadAsyncNmPipe      DosReadAsyncNmPipeProcPtr
#define DosWriteAsyncNmPipe     DosWriteAsyncNmPipeProcPtr

#define GET_WIN_ENTRYPOINT(funcp,funcname) \
			    funcp = GetProcAddress (hlib, funcname); \
			    if (funcp == NULL) {FreeLibrary (hlib); \
						hlib = 0; return FB_FAILURE; }

typedef struct tag_wnpport {
	HANDLE hFile;
	LPVOID lpBuffer;
	BOOL bFinished;
	USHORT usAsyncErr;
	USHORT usNumberOfBytes;
	char acBuffer[ASYNC_READ_BYTES];
	PORT port;
	BOOL bWaiting;
	FARPROC pfnBlockingHook;
	struct tag_wnpport *next;
} *WNPPORT;

static HINSTANCE hlib = 0;

/* necessary static variables */
static FARPROC tpfnBlockingHook;
static DWORD tdwErrCode;
static WNPPORT head_of_port_list = NULL;

static BOOL wfwnp_pipe_io(WORD, HANDLE, LPVOID, USHORT, LPDWORD);
static WNPPORT wfwnp_add(void);
static BOOL wfwnp_remove(WNPPORT wnpremove);
static WNPPORT wfwnp_find_handle(HANDLE hFile);
static WNPPORT wfwnp_find_buffer(LPVOID lpBuffer);

void wfwnp_cleanup(void *);
void _far _pascal wfwnp_async_callback(LPVOID);
static int _far _pascal blocking_stub(void);

static PIPE_FUNCTION(*DosReadAsyncNmPipeProcPtr) (unsigned short,
												  API_CALLBACK,
												  unsigned short far *,
												  unsigned char far *,
												  unsigned short,
												  unsigned short far *);
static PIPE_FUNCTION(*DosWriteAsyncNmPipeProcPtr) (unsigned short,
												   API_CALLBACK,
												   unsigned short far *,
												   unsigned char far *,
												   unsigned short,
												   unsigned short far *);


int initWNP(void)
{
/**************************************
 *
 *	i n i t W N P
 *
 **************************************
 *
 * Functional description:
 *	Initializes the netapi library.
 *
 **************************************/
	extern HINSTANCE LoadDll(UCHAR *);

	if (hlib == 0) {
		hlib = LoadDll("netapi.dll");

		if (hlib) {
			GET_WIN_ENTRYPOINT(DosReadAsyncNmPipeProcPtr,
							   "DosReadAsyncNmPipe");
			GET_WIN_ENTRYPOINT(DosWriteAsyncNmPipeProcPtr,
							   "DosWriteAsyncNmPipe");

			gds__register_cleanup(wfwnp_cleanup, (void *) hlib);
		}
		else {
			return FB_FAILURE;
		}
	}

	WNPSetBlockingHook(blocking_stub);
	SetLastError(NO_ERROR);

	return FB_SUCCESS;
}


HANDLE CreateFile(LPCSTR lpFileName,
				  DWORD dwDesiredAccess,
				  DWORD dwShareMode,
				  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				  DWORD dwCreationDisposition,
				  DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
/**************************************
 *
 *	C r e a t e F i l e
 *
 **************************************
 *
 * Functional description
 *	Very limited support of Windows NT CreateFile function for named pipes.
 *	Sets GetLastError() to ERROR_BAD_PIPE if unsuccessful for any reason.
 *  NO_ERROR if successful.
 *
 **************************************/
	HANDLE hFile;
	DWORD dwTimeBeg;
	WNPPORT wnpport;


/* Add a new internal buffer for this connection */
	wnpport = wfwnp_add();

	if (wnpport == NULL) {
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);

		return INVALID_HANDLE_VALUE;
	}

	dwTimeBeg = GetTickCount();

/* Wait for successful open on pipe */
	do {
		hFile = _lopen(lpFileName, READ_WRITE);
		if (hFile != HFILE_ERROR) {
			/* Save handle in internal buffer */
			wnpport->hFile = hFile;

			/* Successfully opened pipe */
			SetLastError(NO_ERROR);

			return hFile;
		}

		/* Call blocking hook to yield if necessary */
		while (wnpport->pfnBlockingHook());
	}
	while ((GetTickCount() - dwTimeBeg) < CF_TIMEOUT);

/* Remove internal buffer for this connection */
	wfwnp_remove(wnpport);

/* Unable to open pipe */
	SetLastError(ERROR_FILE_NOT_FOUND);

	return INVALID_HANDLE_VALUE;
}


BOOL ReadFile(HANDLE hFile,
			  LPVOID lpBuffer,
			  DWORD dwNumberOfBytesToRead,
			  LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
/**************************************
 *
 *	R e a d F i l e
 *
 **************************************
 *
 * Functional description
 *	Very limited support of Windows NT ReadFile function for named pipes.
 *	Note that even though dwNumberOfBytes is DWORD, this call is limited
 *  to MAX_PIPE_IO_BYTES (65535) for length of data to read.
 *
 **************************************/

/* Check if parameter exceeds limits */
	if (dwNumberOfBytesToRead > MAX_PIPE_IO_BYTES) {
		SetLastError(ERROR_BAD_LENGTH);

		return FALSE;
	}

	return wfwnp_pipe_io(IO_READ, hFile, lpBuffer,
						 (USHORT) dwNumberOfBytesToRead, lpNumberOfBytesRead);
}


BOOL WriteFile(HANDLE hFile,
			   LPCVOID lpBuffer,
			   DWORD dwNumberOfBytesToWrite,
			   LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
/**************************************
 *
 *	W r i t e F i l e
 *
 **************************************
 *
 * Functional description
 *	Very limited support of Windows NT WriteFile function for named pipes.
 *	Note that even though dwNumberOfBytes is DWORD, this call is limited
 *  to MAX_PIPE_IO_BYTES (65535) for length of data to write.
 *
 **************************************/

/* Check if parameter exceeds limits */
	if (dwNumberOfBytesToWrite > MAX_PIPE_IO_BYTES) {
		SetLastError(ERROR_BAD_LENGTH);

		return FALSE;
	}

	return wfwnp_pipe_io(IO_WRITE, hFile, lpBuffer,
						 (USHORT) dwNumberOfBytesToWrite,
						 lpNumberOfBytesWritten);
}


BOOL CloseHandle(HANDLE hFile)
{
/**************************************
 *
 *	C l o s e H a n d l e
 *
 **************************************
 *
 * Functional description
 *	Support of Windows NT CloseHandle function for named pipes.
 *
 **************************************/
	WNPPORT wnpport;

	wnpport = wfwnp_find_handle(hFile);

	if (_lclose((HFILE) hFile) == HFILE_ERROR || wnpport == NULL) {
		SetLastError(ERROR_INVALID_HANDLE);

		return FALSE;
	}

/* Wait for close to finish on async read */
	if (wnpport->bWaiting) {
		DWORD dwTimeBeg;

		dwTimeBeg = GetTickCount();

		while (wnpport->bWaiting && (GetTickCount() - dwTimeBeg) < CH_TIMEOUT) {
			/* Call blocking hook to yield if necessary */
			while (wnpport->pfnBlockingHook());
		}

		/* Error if close never finished */
		if (wnpport->bWaiting) {
			wnpport->hFile = INVALID_HANDLE_VALUE;

			SetLastError(ERROR_UNEXP_NET_ERR);

			return FALSE;
		}
	}

/* Remove internal buffer for this connection */
	wfwnp_remove(wnpport);

	SetLastError(NO_ERROR);

	return TRUE;
}


DWORD GetLastError()
{
/**************************************
 *
 *	G e t L a s t E r r o r
 *
 **************************************
 *
 * Functional description
 *	Very limited support of Windows NT GetLastError function for named pipes.
 *	Note that the value returned by GetLastError() is shared between multiple
 *  application instances using this DLL.  It is important to get this
 *  value only immediately after making API calls.
 *
 **************************************/
	extern DWORD tdwErrCode;

	return tdwErrCode;
}


void SetLastError( DWORD dwErrCode)
{
/**************************************
 *
 *	S e t L a s t E r r o r
 *
 **************************************
 *
 * Functional description
 *	Very limited support of Windows NT SetLastError function for named pipes.
 *	Note that the value set by SetLastError() is shared between multiple
 *  application instances using this DLL.
 *
 **************************************/
	extern DWORD tdwErrCode;

	tdwErrCode = dwErrCode;

	return;
}


BOOL WNPListenForResponse(PORT port)
{
/**************************************
 *
 *	W N P L i s t e n F o r R e s p o n s e
 *
 **************************************
 *
 * Functional description
 *	Sets up for an async read on port.
 *
 **************************************/
	API_CALLBACK pfn;
	USHORT usQuickErr;
	WNPPORT wnpport;

/* Setup flag and buffer address in internal buffer for callback */
	wnpport = wfwnp_find_handle(port->port_handle);
	wnpport->bFinished = FALSE;
	wnpport->lpBuffer = wnpport->acBuffer;

	if (wnpport == NULL)
		return FALSE;

	wnpport->port = port;
	wnpport->bWaiting = TRUE;

/* The following line of code only works within a DLL.  If this code is
/* moved, MakeProcInstance() may need to be called instead. */
	pfn = (API_CALLBACK) wfwnp_async_callback;

/* Do async read */
	usQuickErr = DosReadAsyncNmPipe((USHORT) port->port_handle, pfn,
									&wnpport->usAsyncErr, wnpport->acBuffer,
									(USHORT) ASYNC_READ_BYTES,
									&wnpport->usNumberOfBytes);

	return (usQuickErr == 0);
}


FARPROC WNPSetBlockingHook(FARPROC pfnBlockingHook)
{
/**************************************
 *
 *	W N P S e t B l o c k i n g H o o k
 *
 **************************************
 *
 * Functional description
 *	Sets the blocking hook function to overide the default implementation.
 *
 **************************************/

	FARPROC pfnOldBlockingHook = tpfnBlockingHook;

	tpfnBlockingHook = pfnBlockingHook;

	return pfnOldBlockingHook;
}


static BOOL wfwnp_pipe_io(
						  WORD wType,
						  HANDLE hFile,
						  LPVOID lpBuffer,
						  USHORT usNumberOfBytes, LPDWORD lpNumberOfBytes)
{
/**************************************
 *
 *	w f w n p _ p i p e _ i o
 *
 **************************************
 *
 * Functional description
 *	Implements the ReadFile and WriteFile functions.
 *	Buffer size, usNumberOfBytes, is limited to MAX_PIPE_IO_BYTES (65535).
 *
 **************************************/
	extern HINSTANCE hInstance;

	API_CALLBACK pfn;
	USHORT usQuickErr;
	WNPPORT wnpport;

/* Setup flag and buffer address in internal buffer for callback */
	wnpport = wfwnp_find_handle(hFile);
	wnpport->bFinished = FALSE;
	wnpport->lpBuffer = lpBuffer;

	if (wnpport == NULL) {
		SetLastError(ERROR_INVALID_HANDLE);

		return FALSE;
	}

/* The following line of code only works within a DLL.  If this code is
/* moved, MakeProcInstance() may need to be called instead. */
	pfn = (API_CALLBACK) wfwnp_async_callback;

	if (wType == IO_READ) {
		/* Do async read */
		usQuickErr = DosReadAsyncNmPipe((USHORT) hFile, pfn,
										&wnpport->usAsyncErr, lpBuffer,
										usNumberOfBytes,
										&wnpport->usNumberOfBytes);
	}
	else if (wType == IO_WRITE) {
		/* Do async write */
		usQuickErr = DosWriteAsyncNmPipe((USHORT) hFile, pfn,
										 &wnpport->usAsyncErr, lpBuffer,
										 usNumberOfBytes,
										 &wnpport->usNumberOfBytes);
	}
	else
		usQuickErr = ERROR_INVALID_PARAMETER;

	if (usQuickErr != 0) {
		SetLastError(usQuickErr);

		return FALSE;
	}

/* Wait for read/write to complete */
	while (TRUE) {
		while (wnpport->pfnBlockingHook());

		if (wnpport->bFinished)
			break;
	}

	*lpNumberOfBytes = wnpport->usNumberOfBytes;

	if (wnpport->usAsyncErr != 0) {
		SetLastError(wnpport->usAsyncErr);

		return FALSE;
	}

	SetLastError(NO_ERROR);

	return TRUE;
}


static WNPPORT wfwnp_add(void)
{
/**************************************
 *
 *	w f w n p _ a d d
 *
 **************************************
 *
 * Functional description
 *	Adds a new internal buffer for tracking this connection.
 *
 **************************************/
	WNPPORT wnpport;

	wnpport = (WNPPORT) ALLOC_LIB_MEMORY((SLONG) sizeof(*wnpport));
/* FREE: static memory freed when DLL unloaded? */

	if (wnpport == NULL)		/* NOMEM: */
		return NULL;

	wnpport->hFile = INVALID_HANDLE_VALUE;
	wnpport->lpBuffer = NULL;
	wnpport->bFinished = FALSE;
	wnpport->usAsyncErr = NO_ERROR;
	wnpport->usNumberOfBytes = 0;
	wnpport->port = NULL;
	wnpport->bWaiting = FALSE;
	wnpport->pfnBlockingHook = tpfnBlockingHook;
	wnpport->next = NULL;

	if (head_of_port_list == 0)
		head_of_port_list = wnpport;
	else {
		wnpport->next = head_of_port_list;
		head_of_port_list = wnpport;
	}

	return wnpport;
}


static BOOL wfwnp_remove( WNPPORT wnpremove)
{
/**************************************
 *
 *	w f w n p _ r e m o v e
 *
 **************************************
 *
 * Functional description
 *	Removes the internal buffer for tracking this connection.
 *
 **************************************/
	WNPPORT wnpport;

	if (head_of_port_list == NULL || wnpremove == NULL)
		return FALSE;

	if (head_of_port_list == wnpremove) {
		head_of_port_list = head_of_port_list->next;
		FREE_LIB_MEMORY(wnpremove);

		return TRUE;
	}

	for (wnpport = head_of_port_list; wnpport != NULL;
		 wnpport = wnpport->next) {
		if (wnpport->next == wnpremove) {
			wnpport->next = wnpport->next->next;
			FREE_LIB_MEMORY(wnpremove);

			return TRUE;
		}
	}

	return FALSE;
}


static WNPPORT wfwnp_find_handle( HANDLE hFile)
{
/**************************************
 *
 *	w f w n p _ f i n d _ h a n d le
 *
 **************************************
 *
 * Functional description
 *	Searched and returns an internal buffer by file handle.
 *
 **************************************/
	WNPPORT wnpport;

	for (wnpport = head_of_port_list; wnpport != NULL &&
		 wnpport->hFile != hFile; wnpport = wnpport->next);

	return wnpport;
}


static WNPPORT wfwnp_find_buffer( LPVOID lpBuffer)
{
/**************************************
 *
 *	w f w n p _ f i n d _ b u f f e r
 *
 **************************************
 *
 * Functional description
 *	Searched and returns an internal buffer by file buffer.
 *
 **************************************/
	WNPPORT wnpport;

	for (wnpport = head_of_port_list; wnpport != NULL;
		 wnpport = wnpport->next) {
		if (wnpport->hFile != INVALID_HANDLE_VALUE
			&& wnpport->lpBuffer == lpBuffer) break;
	}

	return wnpport;
}


void wfwnp_cleanup( void *arg)
{
/**************************************
 *
 *	w f w n p _ c l e a n u p
 *
 **************************************
 *
 * Functional description:
 *	If an arg is passed in, it is the handle to a dynamically loaded
 *	library, so free it.
 *
 **************************************/

	if (arg != NULL) {
		/* Free netapi.dll which was loaded and saved in the global hlib var */
		FreeLibrary((HINSTANCE) arg);
	}

	return;
}


void _far _pascal wfwnp_async_callback( LPVOID lpBuffer)
{
/**************************************
 *
 *	w f w n p _ a s y n c _ c a l l b a c k
 *
 **************************************
 *
 * Functional description
 *	Callback function for both read and write operations when finished.
 *
 **************************************/
	WNPPORT wnpport;

/* Get the internal buffer for this I/O call */
	wnpport = wfwnp_find_buffer(lpBuffer);

	if (wnpport != NULL && !wnpport->bFinished) {
		PORT port;

		/* Mark flag as finished */
		wnpport->bFinished = TRUE;
		wnpport->lpBuffer = NULL;

		port = wnpport->port;

		/* Check if async read */
		if (port != NULL && wnpport->bWaiting) {
			wnpport->bWaiting = FALSE;
			memcpy(port->port_receive.x_private, lpBuffer,
				   wnpport->usNumberOfBytes);
			port->port_receive.x_handy += wnpport->usNumberOfBytes;

			/* Post message to trigger remaining read on pipe */
			if (port->port_msg_handle != NULL) {
				PostMessage(port->port_msg_handle, WM_USER + 1,
							wnpport->usAsyncErr, 0);
			}
		}
	}

	return;
}


static int _far _pascal blocking_stub(void)
{
/**************************************
 *
 *	b l o c k i n g _ s t u b
 *
 **************************************
 *
 * Functional description:
 *	Defines an empty blocking stub so that by default we do not
 *	yield the CPU when waiting on name pipe I/O.
 *	The user must register a blocking function with our DLL
 *	before we will allow any yields (protects ODAPI from re-entrancy).
 *
 **************************************/
/*
 *   This implementation could be changed to the following code for
 *   background message processing by default.
 *
 *   MSG     msg;
 *   BOOL    bMessage;
 *
 *   bMessage = (BOOL) PeekMessage (&msg, NULL, 0, 0, PM_REMOVE | PM_NOYIELD);
 *
 *   if (bMessage)
 *       {
 *       TranslateMessage (&msg);
 *       DispatchMessage (&msg);
 *       }
 *
 *   return bMessage;
 *
 **************************************/

	return FALSE;
}
