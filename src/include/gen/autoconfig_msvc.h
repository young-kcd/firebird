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

//#pragma warning(disable:4099)	// class/struct mixups
#pragma warning(disable:4251)	// needs to have dll-interface
#pragma warning(disable:4291)	// no matching op. delete (there are)
#pragma warning(disable:4355)	// 'this' used in base member initializer list
#pragma warning(disable:4786)	// debug identifiers are truncated
#pragma warning(disable:4800)	// forcing value to bool 'true' or 'false' (performance warning)

// New warnings at level W4

#pragma warning(disable:4018)  // signed/unsigned mismatch
#pragma warning(disable:4100)  // unreferenced formal parameter
#pragma warning(disable:4127)  // conditional expression is constant
//#pragma warning(disable:4131)  // uses old-style declarator
#pragma warning(disable:4146)  // unary minus operator applied to unsigned type, result still unsigned
#pragma warning(disable:4189)  // local variable is initialized but not referenced
#pragma warning(disable:4211)  // nonstandard extension used : redefined extern to static
#pragma warning(disable:4214)  // nonstandard extension used : bit field types other than int
#pragma warning(disable:4244)  // conversion from '.......' to '......', possible loss of data
#pragma warning(disable:4245)  // conversion from '.......' to '......', signed/unsigned mismatch
#pragma warning(disable:4291)  // no matching operator delete found; memory will not be freed if initialization throws an exception
#pragma warning(disable:4309)  // truncation of constant value
#pragma warning(disable:4310)  // cast truncates constant value
#pragma warning(disable:4355)  // '....' used in base member initializer list
#pragma warning(disable:4505)  // unreferenced local function has been removed
#pragma warning(disable:4511)  // copy constructor could not be generated
#pragma warning(disable:4512)  // assignment operator could not be generated
#pragma warning(disable:4514)  // unreferenced inline function has been removed
#pragma warning(disable:4663)  // to explicitly specialize class template '.....' use the following syntax
#pragma warning(disable:4701)  // local variable '......' may be used without having been initialized
#pragma warning(disable:4702)  // unreachable code
#pragma warning(disable:4706)  // assignment within conditional expression
#pragma warning(disable:4709)  // comma operator within array index expression
#pragma warning(disable:4710)  // function '.....' not inlined
#pragma warning(disable:4711)  // function '.....' selected for automatic inline expansion

// Warning from Microsoft Visual Studio\VC98\include\fstream

#pragma warning(disable:4097)  // typedef-name '......' used as synonym for class-name '.....'

#ifndef _X86_
#define _X86_
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

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
#undef HAVE_SYS_SIGNAL_H
#undef HAVE_SYS_SIGINFO_H
#undef HAVE_EDITLINE_H
#undef HAVE_TERMIO_H
#undef HAVE_TERMIOS_H
#undef HAVE_DIRENT_H 
#undef HAVE_SYS_NDIR_H 
#undef HAVE_SYS_DIR_H 
#undef HAVE_NDIR_H 
#undef HAVE_UNISTD_H
#undef HAVE_VARARGS_H
#undef HAVE_VFORK_H
#undef HAVE_CRYPT_H 
#undef HAVE_NETCONFIG_H
#undef HAVE_NETINET_IN_H
#undef HAVE_RPC_RPC_H
#undef HAVE_RPC_XDR_H
#undef HAVE_AIO_H
#undef HAVE_MNTENT_H
#undef HAVE_MNTTAB_H
#undef HAVE_SYS_MNTENT_H
#undef HAVE_SYS_MNTTAB_H
#undef HAVE_SYS_IPC_H
#undef HAVE_SYS_FILE_H
#undef HAVE_SOCKET_H
#undef HAVE_SYS_SOCKET_H
#undef HAVE_SYS_SOCKIO_H
#undef HAVE_WINSOCK2_H


/* Functions */
#undef HAVE_GETTIMEOFDAY
#undef HAVE_TIME
#undef HAVE_TIMES
#undef HAVE_STRCASECMP
#define HAVE_STRICMP
#define HAVE_DIRNAME
#undef HAVE_SIGACTION
#undef HAVE_SETITIMER
#define HAVE_SNPRINTF
#define vsnprintf _vsnprintf
#define HAVE_VSNPRINTF
#define HAVE_SWAB
#define HAVE__SWAB
#undef HAVE_MMAP
#undef HAVE_WORKING_VFORK
#undef HAVE_SETPGRP 
#undef HAVE_SETPGID
#undef HAVE_GETPGRP
#undef SETPGRP_VOID
#undef HAVE_FLOCK
#undef HAVE_INITGROUPS
#define HAVE_GETPAGESIZE
#undef HAVE_CRYPT
#undef HAVE_XDR_HYPER
#undef HAVE_PREAD
#undef HAVE_PWRITE
#define HAVE_GETCWD
#undef HAVE_GETWD
#undef HAVE_SETMNTENT
#undef HAVE_GETMNTENT
#undef GETMNTENT_TAKES_TWO_ARGUMENTS
#undef HAVE_TCGETATTR
#define HAVE_STRDUP
#undef HAVE_MKSTEMP
#undef HAVE_LLRINT
#undef HAVE_LOCALTIME_R


/* Types */
#undef HAVE_SOCKLEN_T

/* gettimeofday accepts second (timezone) argument */
#undef GETTIMEOFDAY_RETURNS_TIMEZONE

/* Are we doing 64bit IO on unix */
#undef _FILE_OFFSET_BITS
#undef _LARGE_FILES

/* sizeof(void *) = 8 ==> HAS_64BIT_POINTERS */
#define SIZEOF_VOID_P 4
#define SIZEOF_LONG 4

/* Is union semun defined? */
#undef HAVE_SEMUN

/* Is struct xdr_ops defined? */
#undef HAVE_STRUCT_XDR_OPS

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

/* Support for ISO syntax for thread-local data */
#undef HAVE___THREAD

/* Inet service name and port */
#define FB_SERVICE_NAME "gds_db"
#define FB_SERVICE_PORT 3050

/* Wnet pipe name */
#define FB_PIPE_NAME "interbas"

/* Xnet objects name */
#define FB_IPC_NAME "FIREBIRD"

/* Maximum allowed pathname length */
#define MAXPATHLEN 260 // should correspond to MAX_PATH define in windef.h

/* Windows platforms support threads */
#define HAVE_MULTI_THREAD 1

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

#define FB_PREFIX "c:\\Program Files\\Firebird\\"

#ifndef HAVE_SOCKLEN_T
typedef int socklen_t;
#endif

#endif
