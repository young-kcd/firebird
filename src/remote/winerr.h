/*
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)errno.h	7.1 (Berkeley) 6/4/86
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
 * Error codes
 */

ERRNOCODE(EPERM,	1,	"Not owner")
ERRNOCODE(ENOENT,	2,	"No such file or directory")
ERRNOCODE(ESRCH,	3,	"No such process")
ERRNOCODE(EINTR,	4,	"Interrupted system call")
ERRNOCODE(EIO,	5,	"I/O error")
ERRNOCODE(ENXIO,	6,	"No such device or address")
ERRNOCODE(E2BIG,	7,	"Arg list too long")
ERRNOCODE(ENOEXEC,	8,	"Exec format error")
ERRNOCODE(EBADF,	9,	"Bad file number")
ERRNOCODE(ECHILD,	10,	"No children")
ERRNOCODE(EAGAIN,	11,	"No more processes")
ERRNOCODE(ENOMEM,	12,	"Not enough core")
ERRNOCODE(EACCES,	13,	"Permission denied")
ERRNOCODE(EFAULT,	14,	"Bad address")
ERRNOCODE(ENOTBLK,	15,	"Block device required")
ERRNOCODE(EBUSY,	16,	"Mount device busy")
ERRNOCODE(EEXIST,	17,	"File exists")
ERRNOCODE(EXDEV,	18,	"Cross-device link")
ERRNOCODE(ENODEV,	19,	"No such device")
ERRNOCODE(ENOTDIR,	20,	"Not a directory")
ERRNOCODE(EISDIR,	21,	"Is a directory")
ERRNOCODE(EINVAL,	22,	"Invalid argument")
ERRNOCODE(ENFILE,	23,	"File table overflow")
ERRNOCODE(EMFILE,	24,	"Too many open files")
ERRNOCODE(ENOTTY,	25,	"Not a typewriter")
ERRNOCODE(ETXTBSY,	26,	"Text file busy")
ERRNOCODE(EFBIG,	27,	"File too large")
ERRNOCODE(ENOSPC,	28,	"No space left on device")
ERRNOCODE(ESPIPE,	29,	"Illegal seek")
ERRNOCODE(EROFS,	30,	"Read-only file system")
ERRNOCODE(EMLINK,	31,	"Too many links")
ERRNOCODE(EPIPE,	32,	"Broken pipe")

/* math software */
ERRNOCODE(EDOM,	33,	"Argument too large")
ERRNOCODE(ERANGE,	34,	"Result too large")

/* non-blocking and interrupt i/o */
ERRNOCODE(EWOULDBLOCK,	35,	"Operation would block")
ERRNOCODE(EINPROGRESS,	36,	"Operation now in progress")
ERRNOCODE(EALREADY,	37,	"Operation already in progress")

/* ipc/network software */

	/* argument errors */
ERRNOCODE(ENOTSOCK,	38,	"Socket operation on non-socket")
ERRNOCODE(EDESTADDRREQ,	39,	"Destination address required")
ERRNOCODE(EMSGSIZE,	40,	"Message too long")
ERRNOCODE(EPROTOTYPE,	41,	"Protocol wrong type for socket")
ERRNOCODE(ENOPROTOOPT,	42,	"Protocol not available")
ERRNOCODE(EPROTONOSUPPORT,	43,	"Protocol not supported")
ERRNOCODE(ESOCKTNOSUPPORT,	44,	"Socket type not supported")
ERRNOCODE(EOPNOTSUPP,	45,	"Operation not supported on socket")
ERRNOCODE(EPFNOSUPPORT,	46,	"Protocol family not supported")
ERRNOCODE(EAFNOSUPPORT,	47,	"Address family not supported by protocol family")
ERRNOCODE(EADDRINUSE,	48,	"Address already in use")
ERRNOCODE(EADDRNOTAVAIL,	49,	"Can't assign requested address")

	/* operational errors */
ERRNOCODE(ENETDOWN,	50,	"Network is down")
ERRNOCODE(ENETUNREACH,	51,	"Network is unreachable")
ERRNOCODE(ENETRESET,	52,	"Network dropped connection on reset")
ERRNOCODE(ECONNABORTED,	53,	"Software caused connection abort")
ERRNOCODE(ECONNRESET,	54,	"Connection reset by peer")
ERRNOCODE(ENOBUFS,	55,	"No buffer space available")
ERRNOCODE(EISCONN,	56,	"Socket is already connected")
ERRNOCODE(ENOTCONN,	57,	"Socket is not connected")
ERRNOCODE(ESHUTDOWN,	58,	"Can't send after socket shutdown")
ERRNOCODE(ETOOMANYREFS,	59,	"Too many references: can't splice")
ERRNOCODE(ETIMEDOUT,	60,	"Connection timed out")
ERRNOCODE(ECONNREFUSED,	61,	"Connection refused")

	/* */
ERRNOCODE(ELOOP,	62,	"Too many levels of symbolic links")
ERRNOCODE(ENAMETOOULONG,	63,	"File name too long")

/* should be rearranged */
ERRNOCODE(EHOSTDOWN,	64,	"Host is down")
ERRNOCODE(EHOSTUNREACH,	65,	"No route to host")
ERRNOCODE(ENOTEMPTY,	66,	"Directory not empty")

/* quotas & mush */
ERRNOCODE(EPROCLIM,	67,	"Too many processes")
ERRNOCODE(EUSERS,	68,	"Too many users")
ERRNOCODE(EDQUOT,	69,	"Disc quota exceeded")
/* Network File System */
ERRNOCODE(ESTALE,	70,	"Stale NFS file handle")
ERRNOCODE(EREMOTE,	71,	"Too many levels of remote in path")

#define WIN_NERR	72
