/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		shrinit.c
 *	DESCRIPTION:	Imported symbol initialization for shared library
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
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "IMP" port
 *
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "SGI" port
 *
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <rpc/types.h>
#include <sys/times.h>
#include <sys/types.h>
#ifndef SCO
#include <mntent.h>
#else
#include <netinet/in.h>
#endif

#define LOCAL_SHLIB_DEFS
#include "../jrd/common.h"


extern "C" {


/* IMPORTANT: Before making any changes, see the WARNING at the end
	      of this file. */

int (*_libgds_setjmp) () = 0;
int (*_libgds_sprintf) () = 0;
int (*_libgds_strlen) () = 0;
int (*_libgds_strcmp) () = 0;
SCHAR *(*_libgds_strcpy) () = 0;
void (*_libgds_exit) () = 0;
void (*_libgds_longjmp) () = 0;
IB_FILE(*_libgds__iob) = 0;
int (*_libgds_abort) () = 0;
void *(*_libgds_memset) () = 0;
pid_t(*_libgds_getpid) () = 0;
void *(*_libgds_memcpy) () = 0;
int (*_libgds_shmdt) () = 0;
int (*_libgds_memcmp) () = 0;
int (*_libgds_fprintf) () = 0;
int (*_libgds_printf) () = 0;
int (*_libgds_errno) = 0;
IB_FILE *(*_libgds_fopen) () = 0;
int (*_libgds_fclose) () = 0;
int (*_libgds__filbuf) () = 0;
void (*_libgds_rewind) () = 0;
int (*_libgds_fseek) () = 0;
int (*_libgds__flsbuf) () = 0;
SCHAR(**_libgds_sys_errlist) = 0;
int *_libgds_sys_nerr = 0;
void *(*_libgds_malloc) () = 0;
int (*_libgds_gettimeofday) () = 0;
SCHAR *(*_libgds_ctime) () = 0;
SCHAR *(*_libgds_getenv) () = 0;
off_t(*_libgds_lseek) () = 0;
int (*_libgds_read) () = 0;
int (*_libgds_open) () = 0;
SCHAR *(*_libgds_strcat) () = 0;
int (*_libgds_fputs) () = 0;
int (*_libgds_fputc) () = 0;
SCHAR *(*_libgds_mktemp) () = 0;
int (*_libgds_unlink) () = 0;
#ifndef SCO
int (*_libgds_sigvec) () = 0;
#endif
int (*_libgds_semctl) () = 0;
int (*_libgds_semop) () = 0;
int (*_libgds_select) () = 0;
SCHAR *(*_libgds_strncpy) () = 0;
SCHAR *(*_libgds_strchr) () = 0;
struct passwd *(*_libgds_getpwnam) () = 0;
uid_t(*_libgds_geteuid) () = 0;
struct passwd *(*_libgds_getpwuid) () = 0;
#ifndef SCO
SCHAR *(*_libgds_getwd) () = 0;
#else
SCHAR *(*_libgds_getcwd) () = 0;
#endif
int (*_libgds_readlink) () = 0;
void (*_libgds_gethostname) () = 0;
void (*_libgds_endpwent) () = 0;
gid_t(*_libgds_getegid) () = 0;
int (*_libgds_kill) () = 0;
int (*_libgds_pipe) () = 0;
pid_t(*_libgds_fork) () = 0;
int (*_libgds_execl) () = 0;
int (*_libgds_write) () = 0;
mode_t(*_libgds_umask) () = 0;
int (*_libgds_lockf) () = 0;
int (*_libgds_shmget) () = 0;
void *(*_libgds_shmat) () = 0;
int (*_libgds_shmctl) () = 0;
key_t(*_libgds_ftok) () = 0;
int (*_libgds_close) () = 0;
int (*_libgds_fscanf) () = 0;
int (*_libgds_semget) () = 0;
int (*_libgds_strncmp) () = 0;
int (*_libgds_send) () = 0;
int (*_libgds_socket) () = 0;
int (*_libgds_connect) () = 0;
int (*_libgds_recv) () = 0;
int (*_libgds_fflush) () = 0;
time_t(*_libgds_time) () = 0;
struct tm *(*_libgds_localtime) () = 0;
uint(*_libgds_sleep) () = 0;
#ifndef SCO
int (*_libgds_gethostid) () = 0;
#endif
clock_t(*_libgds_times) () = 0;
int (*_libgds_stat) () = 0;
int (*_libgds_system) () = 0;
int (*_libgds_fgetc) () = 0;
int (*_libgds_fstat) () = 0;
uid_t(*_libgds_getuid) () = 0;
SCHAR *(*_libgds_fgets) () = 0;
int (*_libgds_atoi) () = 0;
uint(*_libgds_alarm) () = 0;
int *(*_libgds_signal) () = 0;
struct servent *(*_libgds_getservbyname) () = 0;
struct hostent *(*_libgds_gethostbyname) () = 0;
int (*_libgds_setsockopt) () = 0;
int (*_libgds_bind) () = 0;
int (*_libgds_listen) () = 0;
int (*_libgds_accept) () = 0;
int (*_libgds_setreuid) () = 0;
int (*_libgds_setregid) () = 0;
int (*_libgds_chdir) () = 0;
int (*_libgds_ioctl) () = 0;
int (*_libgds_getsockname) () = 0;
int (*_libgds_getpeername) () = 0;
struct hostent *(*_libgds_gethostbyaddr) () = 0;
int (*_libgds_sscanf) () = 0;
void (*_libgds_xdrmem_create) () = 0;
int (*_libgds_innetgr) () = 0;
bool_t(*_libgds_xdr_enum) () = 0;
bool_t(*_libgds_xdr_short) () = 0;
bool_t(*_libgds_xdr_u_short) () = 0;
bool_t(*_libgds_xdr_long) () = 0;
bool_t(*_libgds_xdr_opaque) () = 0;
bool_t(*_libgds_xdr_string) () = 0;
int *(*_libgds_FUNCTIONS_entrypoint) () = 0;
int *(*_libgds_fcntl) () = 0;
int *(*_libgds_fwrite) () = 0;
int *(*_libgds_getppid) () = 0;
#ifndef SCO
int (*_libgds_initgroups) () = 0;
#endif
int (*_libgds_setitimer) () = 0;
void ((*_libgds_sigset) ()) = 0;
SLONG(*_libgds_ftell) () = 0;
int (*_libgds_getgroups) () = 0;
int (*_libgds_bzero) () = 0;
int (*_libgds_shutdown) () = 0;
SCHAR *(*_libgds_crypt) () = 0;
#ifndef SCO
IB_FILE *(*_libgds_setmntent) () = 0;
struct mntent *(*_libgds_getmntent) () = 0;
int (*_libgds_endmntent) () = 0;
#endif
int (*_libgds_pause) () = 0;
SCHAR **(*_libgds_environ) () = 0;
IB_FILE *(*_libgds_fdopen) () = 0;
int (*_libgds_dup) () = 0;
int (*_libgds_execle) () = 0;
#ifdef SCO
bool_t(*_libgds_xdr_float) () = 0;
bool_t(*_libgds_xdr_double) () = 0;
SCHAR *(*_libgds_sbrk) () = 0;
#endif
SCHAR *(*_libgds_strrchr) () = 0;
#ifndef SCO
int (*_libgds_fsync) () = 0;
#endif
int (*_libgds_waitpid) () = 0;
void (*_libgds__exit) () = 0;
#ifdef SCO
IB_FILE *(*_libgds_popen) () = 0;
IB_FILE *(*_libgds_pclose) () = 0;
ulong(*_libgds_htonl) () = 0;
ulong(*_libgds_ntohl) () = 0;
int (*_libgds_atexit) () = 0;
#endif
int (*_libgds_atol) () = 0;
int (*_libgds_execvp) () = 0;
int (*_libgds_nice) () = 0;
int (*_libgds_putenv) () = 0;
int (*_libgds_xdr_free) () = 0;
int (*_libgds_xdr_wrapstring) () = 0;
int (*_libgds_vsprintf) () = 0;
int (*_libgds_h_errno) = 0;
int (*_libgds_chmod) () = 0;
int (*_libgds_ungetc) () = 0;
int (*_libgds_vfprintf) () = 0;
int (*_libgds__assert) () = 0;
int (*_libgds_access) () = 0;
#ifdef SCO
int (*_libgds_fsync) () = 0;
#endif
int (*_libgds_sigsuspend) () = 0;
int (*_libgds_sigprocmask) () = 0;
int (*_libgds_sigaddset) () = 0;

/* WARNING: The definition and initialization of new imported symbol pointers
            should be added at the end of the list ABOVE.  At the same time,
            the size of the padding array, below, should be decremented by
            the number of pointers that are added.  If the size of the padding
            array reaches zero, then new pointers will have to be added after
            the InterBase global data.  At that time, move this comment and
            modify it accordingly.  All new InterBase global data must be
            added at the end of the list BELOW. */

#ifdef SCO
int _libgds_padding[466] = { 0 };
#endif

int*	dbb = 0;
int		debug = 0;
int*	DSQL_default_pool = 0;
int*	DSQL_permanent_pool = 0;
int*	DSQL_error = 0;
unsigned DSQL_debug = 0;
int*	DSQL_parse = 0;
int		DSQL_yyerrflag = 0;
int		DSQL_yychar = 0;
int*	DSQL_hash_table = 0;
void*	gdbb = 0;
int*	internal_db_handles = 0;
#if defined(DEV_BUILD) && defined(WIN_NT) && defined(SUPERSERVER)
FILE	*redirected_output = 0;
#endif

}	// extern "C"
