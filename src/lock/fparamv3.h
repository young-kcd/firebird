/*
 *	PROGRAM:	Lock manager
 *	MODULE:		fparamv3.h
 *	DESCRIPTION:	Version 3 file parameter definitions
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
 * 2002.10.30 Sean Leyne - Code Cleanup, removed obsolete "SUN3_3" port
 *
 */

#ifdef NOHOSTNAME
#define LOCK_FILE	FB_PREFIX "/isc_lock.gbl"
#define EVENT_FILE	FB_PREFIX "/isc_event.gbl"
#else
#ifdef SMALL_FILE_NAMES
#define LOCK_FILE	FB_PREFIX "/isclm.%s"
#define EVENT_FILE	FB_PREFIX "/iscev.%s"
#endif
#endif

#ifdef VMS
#define EVENT_FILE		"[000000]isc_event.%s"
#endif

#ifdef MSDOS
#define EVENT_FILE		"/sharemem/gds_evnt.m0"
#endif

#ifndef EVENT_FILE
#define EVENT_FILE		FB_PREFIX "/isc_event.gbl.%s"
#endif

#define EVENT_DEFAULT_SIZE	32768
#define EVENT_EXTEND_SIZE	32768
#define EVENT_VERSION		1
#define EVENT_SEMAPHORES	1

#ifndef LOCK_FILE
#define LOCK_FILE	FB_PREFIX "/gds.lock.%s"
#endif

#ifndef LOCK_HEADER
#define LOCK_HEADER	FB_PREFIX "/lock_header"
#endif

#ifdef sun
#include <sys/types.h>
#include <sys/ipc.h>
#define SUNOS4
#ifndef i386
#define CSV
#endif
#endif

#if (defined linux || defined FREEBSD || defined NETBSD)
#include <sys/types.h>
#include <sys/ipc.h>
#ifndef i386
#define CSV
#endif
#endif


#ifdef CSV
/* Central server definitions taken from source/csv/csi.h */

#define CSI_FILE                FB_PREFIX "/isc_csv.%s"
#define CSI_DEFAULT_SIZE        262144
#define MAX_PROCESSES           16
#endif
