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

const int EVENT_DEFAULT_SIZE	= 32768;
const int EVENT_EXTEND_SIZE		= 32768;
const int EVENT_VERSION		= 2;
const int EVENT_SEMAPHORES	= 1;

#ifdef NOHOSTNAME
static const char* EVENT_FILE	= "isc_event1";
static const char* LOCK_FILE	= "isc_lock1.gbl";
static const char* LOCK_HEADER	= "isc_config";
static const char* INIT_FILE	= "isc_init1";
static const char* GUARD_FILE	= "isc_guard1";
#elif defined(SMALL_FILE_NAMES)
static const char* EVENT_FILE	= "isce1.%s";
static const char* LOCK_FILE	= "iscl1.%s";
static const char* LOCK_HEADER	= "isc_config";
static const char* INIT_FILE	= "isci1.%s";
static const char* GUARD_FILE	= "iscg1.%s";
#elif defined(VMS)
static const char* EVENT_FILE	= "[000000]isc_event1.%s";
static const char* LOCK_FILE	= "[000000]isc_lock1.%s";
static const char* LOCK_HEADER	= "[000000]isc_config";
static const char* INIT_FILE	= "[000000]isc_init1.%s";
static const char* GUARD_FILE	= "[000000]isc_guard1.%s";
#elif defined(WIN_NT)
static const char* EVENT_FILE	= "%s.evn";
static const char* LOCK_FILE	= "%s.lck";
static const char* LOCK_HEADER	= "ibconfig";
static const char* INIT_FILE	= "%s.int";
static const char* GUARD_FILE	= "%s.grd";
#else
static const char* EVENT_FILE	= "isc_event1.%s";
static const char* LOCK_FILE	= "isc_lock1.%s";
static const char* LOCK_HEADER	= "isc_config";
static const char* INIT_FILE	= "isc_init1.%s";
static const char* GUARD_FILE	= "isc_guard1.%s";
#endif

#ifdef sun
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

#ifdef DARWIN
#undef FB_PREFIX
#define FB_PREFIX		"/all/files/are/in/framework/resources"
#define DARWIN_GEN_DIR		"var"
#define DARWIN_FRAMEWORK_ID	"com.firebirdsql.Firebird"
#endif

/* keep MSG_FILE_LANG in sync with build_file.epp */
#ifdef UNIX
static const char* WORKFILE		= "/tmp/";
static const char* MSG_FILE		= "firebird.msg";
static const char MSG_FILE_LANG[]= "intl/%.10s.msg";
const int LOCALE_MAX	= 10;
static const char* LOGFILE		= "firebird.log";
#define TEMP_PATTERN	"_XXXXXX"
#elif defined(WIN_NT)
static const char* WORKFILE		= "c:\\temp\\";
static const char* MSG_FILE		= "firebird.msg";
static const char MSG_FILE_LANG[]= "intl/%.8s.msg";
const int LOCALE_MAX	= 6;
static const char* LOGFILE		= "firebird.log";
#define TEMP_PATTERN	"XXXXXX"
#elif defined(VMS)
static const char* WORKFILE		= "SYS$SCRATCH:";
static const char* MSG_FILE		= "[sysmsg]firebird_msg.dat";
static const char MSG_FILE_LANG[]= "[sysmsg]firebird_%.10s.dat";
const int LOCALE_MAX	= 10;
static const char* LOGFILE		= "[syserr]firebird.log";
#define TEMP_PATTERN	"_XXXXXX"

static const char ISC_LOGICAL[]	= "interbase:";
static const char ISC_LOGICAL_LOCK[]	= "interbase_lock:";

struct isc_vms_prefix
{
	const TEXT*	isc_prefix;
	const TEXT*	vms_prefix;
};

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
#else
static const char* WORKFILE		= "/tmp/";
static const char* MSG_FILE		= "firebird.msg";
static const char MSG_FILE_LANG[]= "intl/%.10s.msg";
const int LOCALE_MAX	= 10;
static const char* LOGFILE		= "firebird.log";
#define TEMP_PATTERN	"_XXXXXX"
#endif

#endif /* JRD_FILE_PARAMS_H */
