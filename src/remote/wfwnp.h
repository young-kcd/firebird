/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		wfwnp.h
 *	DESCRIPTION:	Windows for Workgroups named pipe support definitions
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
#ifndef _WFWNP_H
#define _WFWNP_H


#define CF_TIMEOUT   10000L   /* Timeout used for waiting for successful */
                              /* CreateFile to a named pipe. */

#define CH_TIMEOUT   10000L   /* Timeout used for waiting for successful */
                              /* CloseHandle on a named pipe. */

#define ASYNC_READ_BYTES    1          /* Bytes to wait for on async read */
#define MAX_PIPE_IO_BYTES   0xFFFF     /* Maximum bytes read/write in call */

#define GENERIC_READ                     (0x80000000L)
#define GENERIC_WRITE                    (0x40000000L)

#define FILE_SHARE_READ                 0x00000001
#define FILE_SHARE_WRITE                0x00000002

#define CREATE_NEW          1
#define CREATE_ALWAYS       2
#define OPEN_EXISTING       3
#define OPEN_ALWAYS         4
#define TRUNCATE_EXISTING   5

#define INVALID_HANDLE_VALUE (HANDLE)-1

typedef const void far      *LPCVOID;
typedef void *LPSECURITY_ATTRIBUTES;
typedef void *LPOVERLAPPED;

#endif /* _WFWNP_H */
