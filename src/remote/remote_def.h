/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		remote_def.h
 *	DESCRIPTION:	Common descriptions
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
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete ports:
 *                          - EPSON, XENIX, DELTA, IMP, NCR3000, M88K
 *                          - NT Power PC and HP9000 s300
 *
 * 2002.10.27 Sean Leyne - Completed removal of obsolete "DG_X86" port
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "Ultrix" port
 *
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "DGUX" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "MPEXL" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "DecOSF" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "SGI" port
 *
 * 2002.10.29 Sean Leyne - Removed support for obsolete IPX/SPX Protocol
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#ifndef REMOTE_REMOTE_DEF_H
#define REMOTE_REMOTE_DEF_H

#include "../remote/protocol.h"

#ifdef VMS
const P_ARCH ARCHITECTURE	= arch_vms;
#endif

#ifdef sun
#ifdef sparc
const P_ARCH ARCHITECTURE	= arch_sun4;
#elif defined(i386)
const P_ARCH ARCHITECTURE	= arch_sun386;
#else
const P_ARCH ARCHITECTURE	= arch_sun;
#endif
#endif

#ifdef hpux
const P_ARCH ARCHITECTURE	= arch_hpux;
#endif

#if (defined AIX || defined AIX_PPC)
const P_ARCH ARCHITECTURE	= arch_rt;
#endif

#ifdef SCO_EV
const P_ARCH ARCHITECTURE	= arch_sco_ev;
#endif

#ifdef LINUX
const P_ARCH ARCHITECTURE	= arch_linux;
#endif

#ifdef FREEBSD
const P_ARCH ARCHITECTURE	= arch_freebsd;
#endif

#ifdef NETBSD
const P_ARCH ARCHITECTURE	= arch_netbsd;
#endif

#ifdef DARWIN
const P_ARCH ARCHITECTURE	= arch_darwin_ppc;
#endif

#ifdef I386
const P_ARCH ARCHITECTURE	= arch_intel_32;
#endif


// port_server_flags
const USHORT SRVR_server		= 1;	/* 0x0001 server */
const USHORT SRVR_multi_client	= 2;	/* 0x0002 multi-client server */
const USHORT SRVR_debug			= 4;	/* 0x0004 */
const USHORT SRVR_inet			= 8;	/* 0x0008 Inet protocol */
const USHORT SRVR_wnet			= 16;	/* 0x0010 Wnet (named pipe) protocol (WinNT) */
const USHORT SRVR_ipc			= 64;	/* 0x0040 IPC protocol (Win32) */
const USHORT SRVR_non_service	= 128;	/* 0x0080 Not running as an NT service */
const USHORT SRVR_high_priority	= 256;	/* 0x0100 fork off server at high priority */
const USHORT SRVR_auto_unload	= 512;	/* 0x0200 Unload server after last disconnect */
const USHORT SRVR_xnet			= 1024; /* 0x0400 IPC protocol via server */
const USHORT SRVR_thread_per_port	= 2048;	/* 0x0800 Bind thread to a port */
const USHORT SRVR_no_icon		= 4096;	/* 0x1000 Tell the server not to show the icon */
#endif /* REMOTE_REMOTE_DEF_H */

