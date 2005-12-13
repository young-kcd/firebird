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

#ifndef _REMOTE_REMOTE_DEF_H_
#define _REMOTE_REMOTE_DEF_H_

#ifdef VMS
#define ARCHITECTURE		arch_vms
#endif

#ifdef sun
#ifdef sparc
#define ARCHITECTURE		arch_sun4
#endif
#ifdef i386
#define ARCHITECTURE		arch_sun386
#endif
#ifndef ARCHITECTURE
#define ARCHITECTURE		arch_sun
#endif
#endif

#ifdef hpux
#define ARCHITECTURE		arch_hpux
#endif

#if (defined AIX || defined AIX_PPC)
#define ARCHITECTURE		arch_rt
#endif

#ifdef SCO_EV
#define ARCHITECTURE		arch_sco_ev
#endif

#ifdef LINUX
#define ARCHITECTURE		arch_linux
#endif

#ifdef FREEBSD
#define ARCHITECTURE		arch_freebsd
#endif

#ifdef NETBSD
#define ARCHITECTURE		arch_netbsd
#endif

#ifdef DARWIN
#define	ARCHITECTURE		arch_darwin_ppc
#endif

#ifdef I386
#ifndef ARCHITECTURE
#define ARCHITECTURE		arch_intel_32
#endif
#endif



#define SRVR_server		1	/* 0x0001 server */
#define SRVR_multi_client	2	/* 0x0002 multi-client server */
#define SRVR_debug		4	/* 0x0004 */
#define SRVR_inet		8	/* 0x0008 Inet protocol */
#define SRVR_wnet		16	/* 0x0010 Wnet (named pipe) protocol (WinNT) */
#define SRVR_ipc		64	/* 0x0040 IPC protocol (Win32) */
#define SRVR_non_service	128	/* 0x0080 Not running as an NT service */
#define SRVR_high_priority	256	/* 0x0100 fork off server at high priority */
#define SRVR_auto_unload	512	/* 0x0200 Unload server after last disconnect */
#define SRVR_xnet			1024 /* 0x0400 IPC protocol via server */
#define SRVR_thread_per_port	2048	/* 0x0800 Bind thread to a port */
#define SRVR_no_icon		4096	/* 0x1000 Tell the server not to show the icon */
#endif /* _REMOTE_REMOTE_DEF_H_ */

