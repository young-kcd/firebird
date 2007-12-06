/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		shdef.h
 *	DESCRIPTION:	Imported symbol definitions
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
 */

#ifndef _JRD_SHDEF_H_
#define _JRD_SHDEF_H_

#include "../jrd/ib_stdio.h"
#include <sys/times.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define setjmp		(*_libgds_setjmp)
#define sprintf		(*_libgds_sprintf)
#define strlen		(*_libgds_strlen)
#define strcmp		(*_libgds_strcmp)
#define strcpy		(*_libgds_strcpy)
#define exit		(*_libgds_exit)
#define longjmp		(*_libgds_longjmp)
#define _iob		(*_libgds__iob)
#define abort		(*_libgds_abort)
#define memset		(*_libgds_memset)
#define getpid		(*_libgds_getpid)
#define memcpy		(*_libgds_memcpy)
#define shmdt		(*_libgds_shmdt)
#define memcmp		(*_libgds_memcmp)
#define ib_fprintf		(*_libgds_fprintf)
#define ib_printf		(*_libgds_printf)
#define errno		(*_libgds_errno)
#define ib_fopen		(*_libgds_fopen)
#define ib_fclose		(*_libgds_fclose)
#define _filbuf		(*_libgds__filbuf)
#define ib_rewind		(*_libgds_rewind)
#define ib_fseek		(*_libgds_fseek)
#define ib_ftell		(*_libgds_ftell)
#define _flsbuf		(*_libgds__flsbuf)
#define gettimeofday	(*_libgds_gettimeofday)
#define getenv		(*_libgds_getenv)
#define lseek		(*_libgds_lseek)
#define read		(*_libgds_read)
#define open		(*_libgds_open)
#define strcat		(*_libgds_strcat)
#define unlink		(*_libgds_unlink)
#define semctl		(*_libgds_semctl)
#define semop		(*_libgds_semop)
#define strncpy		(*_libgds_strncpy)
#define getpwnam	(*_libgds_getpwnam)
#define kill		(*_libgds_kill)
#define fork		(*_libgds_fork)
#define write		(*_libgds_write)
#define umask		(*_libgds_umask)
#define close		(*_libgds_close)
#define ib_fscanf		(*_libgds_fscanf)
#define strncmp		(*_libgds_strncmp)
#define send		(*_libgds_send)
#define socket		(*_libgds_socket)
#define recv		(*_libgds_recv)
#define ib_fflush		(*_libgds_fflush)
#define time		(*_libgds_time)
#define localtime	(*_libgds_localtime)
#define sleep		(*_libgds_sleep)
#ifndef SCO
#define gethostid	(*_libgds_gethostid)
#endif
#define getuid		(*_libgds_getuid)
#define ib_fgets		(*_libgds_fgets)
#define atoi		(*_libgds_atoi)
#define alarm		(*_libgds_alarm)
#define signal		(*_libgds_signal)
#define ioctl		(*_libgds_ioctl)
#define sscanf		(*_libgds_sscanf)
#define ib_fwrite		(*_libgds_fwrite)
#define crypt		(*_libgds_crypt)
#define _semputc	(*_libgds__semputc)
#define _semgetc	(*_libgds__semgetc)
#define _sproced	(*_libgds__sproced)
#define _us_rsthread_stdio      (*_libgds__us_rsthread_stdio)
#define sbrk            (*_libgds_sbrk)
#define ctime		(*_libgds_ctime)
#define atol		(*_libgds_atol)

extern int sprintf();
extern int strlen();
extern int strcmp();
extern SCHAR *strcpy();
extern void exit();
extern IB_FILE _iob[];
extern int abort();
extern void *memset();
extern pid_t getpid();
extern void *memcpy();
extern int shmdt();
extern int memcmp();
extern int ib_fprintf();
extern int ib_printf();
extern int errno;
extern IB_FILE *ib_fopen();
extern int ib_fclose();
extern int _filbuf();
extern void ib_rewind();
extern int ib_fseek();
extern SLONG ib_ftell();
extern int _flsbuf();
extern int gettimeofday();
extern SCHAR *getenv();
extern off_t lseek();
extern int read();
extern int open();
extern SCHAR *strcat();
extern int unlink();
extern int semctl();
extern int semop();
extern SCHAR *strncpy();
extern struct passwd *getpwnam();
extern int kill();
extern pid_t fork();
extern int write();
extern mode_t umask();
extern int close();
extern int ib_fscanf();
extern int strncmp();
extern int send();
extern int socket();
extern int recv();
extern int ib_fflush();
extern time_t time();
extern struct tm *localtime();
extern uint sleep();
extern int gethostid();
extern uid_t getuid();
extern SCHAR *ib_fgets();
extern int atoi();
extern uint alarm();
extern void (*signal()) ();
extern int ioctl();
extern int sscanf();
extern int ib_fwrite();
extern SCHAR *crypt();
extern void *_sproced;
extern int _semgetc();
extern int _semputc();
extern int _us_rsthread_stdio;
extern int sbrk();
extern SCHAR *ctime();
extern SLONG atol();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _JRD_SHDEF_H */