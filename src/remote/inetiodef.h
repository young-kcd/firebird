/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		remote.h
 *	DESCRIPTION:	Internet I/O Definitions!
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

#ifndef IO$S_FCODE
#define IO$S_FCODE	6
#endif	IO$S_FCODE

#ifndef IO$_IOCTL
#define IO$_SEND	(IO$_WRITEVBLK)
#define IO$_RECEIVE	(IO$_READVBLK)
#define IO$_SOCKET	(IO$_ACCESS | (0 << IO$S_FCODE))
#define IO$_BIND	(IO$_ACCESS | (1 << IO$S_FCODE))
#define IO$_LISTEN	(IO$_ACCESS | (2 << IO$S_FCODE))
#define IO$_ACCEPT	(IO$_ACCESS | (3 << IO$S_FCODE))
#define IO$_CONNECT	(IO$_ACCESS | (4 << IO$S_FCODE))
#define IO$_SETSOCKOPT	(IO$_ACCESS | (5 << IO$S_FCODE))
#define IO$_GETSOCKOPT	(IO$_ACCESS | (6 << IO$S_FCODE))
#define IO$_IOCTL	(IO$_ACCESS | (8 << IO$S_FCODE))
#define IO$_ACCEPT_WAIT (IO$_ACCESS | (10 << IO$S_FCODE))
#define IO$_SHUTDOWN	(IO$_ACCESS | (12 << IO$S_FCODE))
#define IO$_GETSOCKNAME (IO$_ACCESS | (13 << IO$S_FCODE))
#define IO$_GETPEERNAME (IO$_ACCESS | (15 << IO$S_FCODE))
#endif	IO$_IOCTL
