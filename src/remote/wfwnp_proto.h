/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		wfwnp_pr.h
 *	DESCRIPTION:	Prototpe header file for wfwnp.c
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
#ifndef _WFWNP_PR_H
#define _WFWNP_PR_H

int         initWNP (void);
HANDLE      CreateFile (LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD,
               DWORD, HANDLE);
BOOL        ReadFile (HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);
BOOL        WriteFile (HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
BOOL        FlushFileBuffers (HANDLE);
BOOL        CloseHandle (HANDLE);
DWORD       GetLastError ();
void        SetLastError (DWORD);
BOOL			WNPListenForResponse(PORT);
FARPROC     WNPSetBlockingHook (FARPROC);

#endif /* _WFWNP_PR_H */
