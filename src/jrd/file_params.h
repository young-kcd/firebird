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
 */

#ifndef _JRD_FILE_PARAMS_H_
#define _JRD_FILE_PARAMS_H_

#define EVENT_DEFAULT_SIZE	32768
#define EVENT_EXTEND_SIZE	32768
#define EVENT_VERSION		2
#define EVENT_SEMAPHORES	1

#ifdef NOHOSTNAME
#define EVENT_FILE		"isc_event1"
#define LOCK_FILE		"isc_lock1.gbl"
#define CSI_FILE		"isc_csv1"
#define INIT_FILE		"isc_init1"
#define GUARD_FILE		"isc_guard1"
#else
#ifdef SMALL_FILE_NAMES
#define EVENT_FILE		"isce1.%s"
#define LOCK_FILE		"iscl1.%s"
#define CSI_FILE		"iscc1.%s"
#define INIT_FILE		"isci1.%s"
#define GUARD_FILE		"iscg1.%s"
#endif
#endif


#ifdef NETWARE_386

#define LOCK_HEADER		"isc_conf"

#endif


#ifdef VMS
#define EVENT_FILE		"[000000]isc_event1.%s"
#define INIT_FILE		"[000000]isc_init1.%s"
#define GUARD_FILE		"[000000]isc_guard1.%s"
#endif

#if defined WIN_NT
#define EVENT_FILE		"%s.evn"
#define LOCK_FILE		"%s.lck"
#define CSI_FILE		"%s.csv"
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

#ifndef CSI_FILE
#define CSI_FILE		"isc_csv1.%s"
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
#ifndef SUN3_3
#include <sys/types.h>
#include <sys/ipc.h>
#define SUNOS4
#endif
#endif

#ifdef EPSON
#include <sys/types.h>
#include <sys/ipc.h>
#endif

#ifdef LINUX
#include <sys/types.h>
#include <sys/ipc.h>
#endif

#if defined FREEBSD || defined NETBSD
#include <sys/types.h>
#include <sys/ipc.h>
#endif

#ifdef CSV
#define CSI_DEFAULT_SIZE	262144
#define CSI_EXTEND_SIZE		32768
#define MAX_PROCESSES		16
#endif

#ifdef UNIX
#define WORKFILE	"/tmp/"
#ifdef LINUX
#define ISC_PREFIX	"/usr/local/interbase/"
#else
#ifdef DARWIN
#define ISC_PREFIX		"/all/files/are/in/framework/resources"
#define DARWIN_GEN_DIR		"var"
#define DARWIN_FRAMEWORK_ID	"com.firebird.Firebird2"
#else
#define ISC_PREFIX	"/usr/interbase/"
#endif
#endif
#endif


/* keep MSG_FILE_LANG in sync with build_file.e */

#ifdef PC_PLATFORM
#ifdef NETWARE_386
#define WORKFILE	"sys:\\tmp\\"
#define ISC_PREFIX	"sys:\\interbas\\"
#define TEMP_PATTERN	""
#else
#define WORKFILE	""
#define ISC_PREFIX	"c:/interbas/"
#define TEMP_PATTERN	"XXXXXX"
#endif
#define MSG_FILE	"interbas.msg"
#define MSG_FILE_LANG	"%.8s.msg"
#define LOCALE_MAX	5
#define LOGFILE		"interbas.log"
#endif

#ifdef WIN_NT
#define WORKFILE	"c:\\temp\\"
#define ISC_PREFIX	"c:\\Program Files\\Firebird\\FBServer/"
#define ISC_PREFIX_SUBPATH "\\Firebird\\FBServer/"
#define MSG_FILE	"interbase.msg"
#define MSG_FILE_LANG	"intl/%.8s.msg"
#define LOCALE_MAX	6
#define LOGFILE		"interbase.log"
#define TEMP_PATTERN	"XXXXXX"
#endif

#ifdef VMS
#define MSG_FILE	"[sysmsg]interbase_msg.dat"
#define MSG_FILE_LANG	"[sysmsg]interbase_%.10s.dat"
#define LOCALE_MAX	10
#define LOGFILE		"[syserr]interbase.log"
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
#define MSG_FILE	"interbase.msg"
#define MSG_FILE_LANG	"intl/%.10s.msg"
#define LOCALE_MAX	10
#endif

#ifndef LOGFILE
#define LOGFILE		"interbase.log"
#endif

#ifndef TEMP_PATTERN
#define TEMP_PATTERN	"_XXXXXX"
#endif

#endif /* _JRD_FILE_PARAMS_H_ */
