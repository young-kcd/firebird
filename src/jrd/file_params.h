/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		file_params.h
 *	DESCRIPTION:	File parameter definitions
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
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "EPSON" define*
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 * 2002.10.30 Sean Leyne - Code Cleanup, removed obsolete "SUN3_3" port
 *
 */

#ifndef JRD_FILE_PARAMS_H
#define JRD_FILE_PARAMS_H

#define EVENT_DEFAULT_SIZE	32768
#define EVENT_EXTEND_SIZE	32768
#define EVENT_VERSION		2
#define EVENT_SEMAPHORES	1

#ifdef NOHOSTNAME
#define EVENT_FILE		"isc_event1"
#define LOCK_FILE		"isc_lock1.gbl"
#define INIT_FILE		"isc_init1"
#define GUARD_FILE		"isc_guard1"
#else
#ifdef SMALL_FILE_NAMES
#define EVENT_FILE		"isce1.%s"
#define LOCK_FILE		"iscl1.%s"
#define INIT_FILE		"isci1.%s"
#define GUARD_FILE		"iscg1.%s"
#endif
#endif


#ifdef VMS
#define EVENT_FILE		"[000000]isc_event1.%s"
#define INIT_FILE		"[000000]isc_init1.%s"
#define GUARD_FILE		"[000000]isc_guard1.%s"
#endif

#ifdef WIN_NT
#define EVENT_FILE		"%s.evn"
#define LOCK_FILE		"%s.lck"
#define LOCK_HEADER		"ibconfig"
#define INIT_FILE		"%s.int"
#define GUARD_FILE		"%s.grd"
#endif

#ifndef EVENT_FILE
#define EVENT_FILE		"isc_event1.%s"
#endif

#ifndef LOCK_FILE
#define LOCK_FILE		"isc_lock1.%s"
#endif

#ifndef LOCK_HEADER
#define LOCK_HEADER		"isc_config"
#endif

#ifndef INIT_FILE
#define INIT_FILE		"isc_init1.%s"
#endif

#ifndef GUARD_FILE
#define GUARD_FILE		"isc_guard1.%s"
#endif

#ifdef sun
#include <sys/types.h>
#include <sys/ipc.h>
#define SUNOS4
#endif

#ifdef LINUX
#include <sys/types.h>
#include <sys/ipc.h>
#endif

#if defined FREEBSD || defined NETBSD
#include <sys/types.h>
#include <sys/ipc.h>
#endif

#ifdef UNIX
#define WORKFILE	"/tmp/"
#ifdef DARWIN
#undef FB_PREFIX
#define FB_PREFIX		"/all/files/are/in/framework/resources"
#define DARWIN_GEN_DIR		"var"
#define DARWIN_FRAMEWORK_ID	"com.firebirdsql.Firebird"
#endif
#endif

/* keep MSG_FILE_LANG in sync with build_file.e */

#ifdef WIN_NT
#define WORKFILE	"c:\\temp\\"
#define MSG_FILE	"firebird.msg"
#define MSG_FILE_LANG	"intl/%.8s.msg"
#define LOCALE_MAX	6
#define LOGFILE		"firebird.log"
#define TEMP_PATTERN	"XXXXXX"
#endif

#ifdef VMS
#define MSG_FILE	"[sysmsg]firebird_msg.dat"
#define MSG_FILE_LANG	"[sysmsg]firebird_%.10s.dat"
#define LOCALE_MAX	10
#define LOGFILE		"[syserr]firebird.log"
#define WORKFILE	"SYS$SCRATCH:"
#define ISC_LOGICAL	"interbase:"
#define ISC_LOGICAL_LOCK	"interbase_lock:"

typedef struct isc_vms_prefix
{
	TEXT*	isc_prefix;
	TEXT*	vms_prefix;
} *ISC_VMS_PREFIX;

static struct isc_vms_prefix trans_prefix[] =
{
	"[SYSMSG]", "SYS$MESSAGE:",
	"[SYSEXE]", "SYS$SYSTEM:",
	"[SYSMGR]", "SYS$MANAGER:",
	"[SYSLIB]", "SYS$LIBRARY:",
	"[SYSHLP]", "SYS$HELP:",
	"[SYSERR]", "SYS$ERRORLOG:",
	"[000000]", "SYS$SYSTEM:",
	NULL, NULL
};
#endif

#ifndef MSG_FILE
#define MSG_FILE	"firebird.msg"
#define MSG_FILE_LANG	"intl/%.10s.msg"
#define LOCALE_MAX	10
#endif

#ifndef LOGFILE
#define LOGFILE		"firebird.log"
#endif

#ifndef TEMP_PATTERN
#define TEMP_PATTERN	"_XXXXXX"
#endif

#endif /* JRD_FILE_PARAMS_H */
