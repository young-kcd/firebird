/*
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete ports:
 *                          - MAC (MAC, MAC_AUX and "MAC_CP" defines)
 *                          - EPSON, DELTA, IMP, NCR3000, NeXT, M88K, Cray
 *                          - OS/2, Apollo
 *
 * 2002-02-23 Sean Leyne - Code Cleanup, removed old Win3.1 port (Windows_Only)
 *
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "UNIXWARE" port
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "Ultrix" port
 *
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "DGUX" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "MPEXL" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "SGI" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#ifndef AUTOCONFIG_H
#define AUTOCONFIG_H

#ifdef _MSC_VER
// turn off some warnings from the inferior MS compiler
#pragma warning(disable: 4099)	// class/struct mixups
#pragma warning(disable: 4291)	// no matching op. delete (there are)
#pragma warning(disable: 4355)	// 'this' used in base member initializer list
#endif

#ifndef _X86_
#define _X86_
#endif

#ifdef DEV_BUILD
#define DEBUG_GDS_ALLOC
#endif

/* Headers */
#define HAVE_ASSERT_H
#define HAVE_CTYPE_H
#undef HAVE_UNISTD_H
#define HAVE_FCNTL_H
#undef HAVE_GRP_H
#undef HAVE_PWD_H
#undef HAVE_LIBIO_H
#define HAVE_LOCALE_H
#define HAVE_MATH_H
#undef HAVE_PTHREAD_H
#undef HAVE_SYS_TYPES_H
#define HAVE_SYS_STAT_H
#undef HAVE_SYS_UIO_H
#undef HAVE_SYS_WAIT_H
#undef TIME_WITH_SYS_TIME
#define HAVE_TIME_H
#undef HAVE_SYS_TIME_H
#define HAVE_SYS_TIMEB_H
#undef HAVE_SYS_PARAM_H
#undef HAVE_SYS_IOCTL_H
#define HAVE_SETJMP_H
#define HAVE_STDARG_H
#define HAVE_STDLIB_H
#define HAVE_STRING_H
#define HAVE_SIGNAL_H
#undef HAVE_READLINE_READLINE_H
#undef HAVE_EDITLINE_H
#undef HAVE_UNISTD_H
#undef HAVE_VARARGS_H
#undef HAVE_VFORK_H

/* Functions */
#undef HAVE_STRLCPY
#undef HAVE_STRLCAT
#undef HAVE_GETTIMEOFDAY
#undef HAVE_TIME
#undef HAVE_TIMES
#undef HAVE_STRCASECMP
#define HAVE_STRICMP
#define HAVE_DIRNAME
#undef HAVE_SIGACTION
#define HAVE_SNPRINTF
#undef HAVE_MMAP
#undef HAVE_ATEXIT
#undef HAVE_ON_EXIT
#undef HAVE_WORKING_VFORK
#undef HAVE_SETPGRP 
#undef HAVE_SETPGID
#undef SETPGRP_VOID
#undef HAVE_FLOCK
#undef HAVE_INITGROUPS
#undef HAVE_GETPAGESIZE
#undef HAVE_XDR_HYPER

/* Types */
#undef socklen_t

/* gettimeofday accepts second (timezone) argument */
#undef GETTIMEOFDAY_RETURNS_TIMEZONE

/* Are we doing 64bit IO on unix */
#undef _FILE_OFFSET_BITS
#undef _LARGE_FILES

/* sizeof(void *) = 8 ==> HAS_64BIT_POINTERS */
#define SIZEOF_VOID_P 4

/* alignment of long */
#define ALIGNMENT 4

/* alignment of double */
#define DOUBLE_ALIGN 8

/* Functions */
#define snprintf _snprintf

/* Is the platform big endian? */
#undef WORDS_BIGENDIAN

/* Return type of signal handler */
#define RETSIGTYPE void

/* Are paths case sensitive? */
#define CASE_SENSITIVITY false

/* Do we build (need) the lock manager process? */
#undef MANAGER_PROCESS

/* Support databases on Raw Devices (UNIX only) */
#undef SUPPORT_RAW_DEVICES

/* CPU types */
#undef PowerPC
#undef sparc
#define i386
#undef mips
#undef alpha

/* various OS efforts */
#undef DARWIN
#undef LINUX
#undef FREEBSD
#undef NETBSD
#undef sun
#undef SOLARIS
#undef hpux
#undef VMS
#undef AIX
#define WIN_NT
#undef SCO_EV
#undef SINIXZ

#endif
