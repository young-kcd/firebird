/*
 *      PROGRAM:        JRD access method
 *      MODULE:         common.h
 *      DESCRIPTION:    Common descriptions for all GDS programs
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
 * Added TCP_NO_DELAY option for superserver on Linux
 * FSG 16.03.2001
 *
 * 2001.07.06 Sean Leyne - Code Cleanup, removed "#ifdef READONLY_DATABASE"
 *                         conditionals, as the engine now fully supports
 *                         readonly databases.
 *
 * 2001.11.20  Ann Harrison - make 64bitio.h conditional on not windows.
 *
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete ports:
 *                          - MAC ("MAC", "MAC_AUX" and "MAC_CP" defines)
 *                          - EPSON, XENIX, DELTA, IMP, NCR3000, M88K
 *                          - NT Power PC and HP9000 s300
 *
 * 2002.04.16  Paul Beach - HP10 and unistd.h
 *
 * 2002.10.27 Sean Leyne - Completed removal of obsolete "DG_X86" port
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "UNIXWARE" port
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "Ultrix" port
 *
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "DGUX" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "DecOSF" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "SGI" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */
/*
$Id: common.h,v 1.73.2.3 2003-10-27 18:30:37 skidder Exp $
*/

#ifndef JRD_COMMON_H
#define JRD_COMMON_H

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#ifndef INCLUDE_FB_MACROS_H
#include "../include/fb_macros.h"
#endif

#ifndef INCLUDE_FB_TYPES_H
#include "../include/fb_types.h"
#endif


/*
  do not use links in source code to maintain platform neutraility
*/

#ifdef PROD_BUILD
#ifdef DEV_BUILD
#undef DEV_BUILD
#endif
#endif

#ifdef SUPERSERVER
#define GOVERNOR 1
#define CANCEL_OPERATION
#define FB_ARCHITECTURE isc_info_db_class_server_access
#else
#define FB_ARCHITECTURE isc_info_db_class_classic_access
#endif


/*****************************************************
* Linux for Intel platforms 
*****************************************************/
#ifdef LINUX
#define QUADFORMAT "ll"
#define QUADCONST(n) (n##LL)
#define SLONGFORMAT "ld"
#define ULONGFORMAT "lu"
#define XLONGFORMAT "lX"
#define xLONGFORMAT "lx"


#define __LINE__IS_INT

#ifdef SUPERSERVER
#define SET_TCP_NO_DELAY
#endif

#define KILLER_SIGNALS

#define VA_START(list,parmN)    va_start (list, parmN)
#define UNIX    1
#define IEEE    1

#ifdef i386
#define I386    1
#define IMPLEMENTATION  isc_info_db_impl_i386 /* 60  next higher unique number, See you later  */
#endif /* i386 */

#ifdef sparc
#define IMPLEMENTATION  isc_info_db_impl_linux_sparc /* 65  */
#endif /* sparc */

#define MEMMOVE(from,to,length)		memmove ((void *)to, (void *)from, (size_t) length)
#define MOVE_FAST(from,to,length)       memcpy (to, from, (int) (length))
#define MOVE_FASTER(from,to,length)     memcpy (to, from, (int) (length))
#define MOVE_CLEAR(to,length)           memset (to, 0, (int) (length))

typedef RETSIGTYPE (*SIG_FPTR) (int);
#endif /* LINUX */

/*****************************************************
* SINIX-Z 5.42
*****************************************************/
#ifdef SINIXZ
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* These prototypes are missing in the system header files :-( */
int gettimeofday (struct timeval *tp);
int munmap(void * addr, size_t len);
int gethostname(char *name, size_t len);
int socket(int domain, int type, int protocol);
int connect(int s, struct sockaddr *name, int namelen);
int send(int s, void *msg, int len, int flags);
int recv(int s, void *buf, int len, int flags);
int strcasecmp(const char *s1, const char *s2);
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *execptfds, struct timeval *timeout);
int getsockopt(int s, int level, int optname, char *optval, int *optlen);
int setsockopt(int s, int level, int optname, char *optval, int optlen);
int bind(int s, struct sockaddr *name, int namelen);
int listen(int s, int backlog);
int accept(int s, struct sockaddr *ddr, int *addrlen);
int getsockname(int s, struct sockaddr *name, int *namelen);
int setsockname(int s, struct sockaddr *name, int *namelen);
int getpeername(int s, struct sockaddr *name, int *namelen);
int shutdown(int s, int how);
int syslog(int pri, char *fmt, ...);

#ifdef __cplusplus
    }
#endif

#include <dlfcn.h>
#define dlopen(a,b)		dlopen((char *)(a),(b))
#define dlsym(a,b)		dlsym((a), (char *)(b))

#define QUADFORMAT "ll"
#define QUADCONST(n) (n##LL)

/*#define ALIGNMENT	4*/
/*#define DOUBLE_ALIGN	8*/

#ifdef SUPERSERVER
#define SET_TCP_NO_DELAY
#endif

#define KILLER_SIGNALS

#define VA_START(list,parmN)    va_start (list, parmN)
#define UNIX    1
#define IEEE    1

#ifdef i386
#define I386    1
/* Change version string into SINIXZ */
#define IMPLEMENTATION  isc_info_db_impl_sinixz  /* 64 */
#endif /* i386 */

#define setreuid(ruid,euid)     setuid(euid)
#define setregid(rgid,egid)     setgid(egid)

#define MEMMOVE(from,to,length)		memmove ((void *)to, (void *)from, (size_t) length)
#define MOVE_FAST(from,to,length)       memcpy (to, from, (int) (length))
#define MOVE_FASTER(from,to,length)     memcpy (to, from, (int) (length))
#define MOVE_CLEAR(to,length)           memset (to, 0, (int) (length))

typedef RETSIGTYPE (*SIG_FPTR) ();

#define __LINE__IS_INT
#define SLONGFORMAT	"ld"
#define ULONGFORMAT "lu"
#define XLONGFORMAT "lX"
#define xLONGFORMAT "lx"
#endif /* SINIXZ */

/*****************************************************
* Darwin Platforms 
*****************************************************/
#ifdef DARWIN
/* EKU: obsolete, replaced by _FILE_OFFSET_BITS
#ifndef UNIX_64_BIT_IO
#define UNIX_64_BIT_IO
#endif
*/
#define __LINE__IS_INT
#define SLONGFORMAT	"ld"
#define ULONGFORMAT "lu"
#define XLONGFORMAT "lX"
#define xLONGFORMAT "lx"

/*#define ALIGNMENT       4*/
/*#define DOUBLE_ALIGN    4*/
#define BSD_UNIX        1
#define UNIX            1
#define IMPLEMENTATION  63
#define IEEE
#define QUADCONST(n) (n##LL)
#define QUADFORMAT "q"
#define MAP_ANONYMOUS
#define MAP_ANNON
#define LSEEK_OFFSET_CAST (off_t)

#define MEMMOVE(from,to,length)		memmove ((void *)to, (void *)from, (size_t)length)
#define MOVE_FAST(from,to,length)	memcpy (to, from, (int) (length))
#define MOVE_FASTER(from,to,length)	memcpy (to, from, (int) (length))
#define MOVE_CLEAR(to,length)		memset (to, 0, (int) (length))

typedef RETSIGTYPE (*SIG_FPTR) (int);
#endif /* Darwin Platforms */


/*****************************************************
* FreeBSD for Intel platforms 
*****************************************************/
#ifdef FREEBSD

/* EKU: obsolete, replaced by _FILE_OFFSET_BITS
#ifndef UNIX_64_BIT_IO
#define UNIX_64_BIT_IO
#endif
*/

/*#define ALIGNMENT     4*/
/*#define DOUBLE_ALIGN  4*/

#define UNIX  1
#define IEEE  1
#define I386  1
#define IMPLEMENTATION    isc_info_db_impl_freebsd   /* 61 */

#define QUADFORMAT "ll"
#define QUADCONST(n) (n##LL)
#define KILLER_SIGNALS
#define NO_NFS					/* no MTAB_OPEN or MTAB_CLOSE in isc_file.c */

#define MEMMOVE(from,to,length)     memmove ((void *)to, (void *)from, (size_t) length)
#define MOVE_FAST(from,to,length)       memcpy (to, from, (int) (length))
#define MOVE_FASTER(from,to,length)     memcpy (to, from, (int) (length))
#define MOVE_CLEAR(to,length)           memset (to, 0, (int) (length))

typedef RETSIGTYPE (*SIG_FPTR) (int);
#endif /* FREEBSD */

/*****************************************************
* NetBSD 
*****************************************************/
#ifdef NETBSD

#if defined(__i386__)
/*#define ALIGNMENT     4*/
/*#define DOUBLE_ALIGN  4*/

#define IEEE  1
#define I386  1
#define IMPLEMENTATION        isc_info_db_impl_netbsd /* 62 */

#define QUADFORMAT "ll"
#define QUADCONST(n) (n##LL)
#else /* !__i386__ */
#error Please add support for other ports
#endif

#define UNIX  1

#define KILLER_SIGNALS
#define NO_NFS					/* no MTAB_OPEN or MTAB_CLOSE in isc_file.c */

#define MEMMOVE(from,to,length)     memmove ((void *)to, (void *)from, (size_t) length)
#define MOVE_FAST(from,to,length)       memcpy (to, from, (int) (length))
#define MOVE_FASTER(from,to,length)     memcpy (to, from, (int) (length))
#define MOVE_CLEAR(to,length)           memset (to, 0, (int) (length))

typedef RETSIGTYPE (*SIG_FPTR) ();
#endif /* NETBSD */



/*****************************************************
 * SUN platforms--the 386i is obsolete 
*****************************************************/

#ifdef sun
/* Defined KILLER_SIGNALS for Sun - as we were getting lots of lockups
 * using pipe server.
 * 1995-February-24 David Schnepper
 */
#define KILLER_SIGNALS

#ifdef SOLARIS

/* This is NOT defined when building the special shared-pipe library
 * which customers can use to avoid the problems with signals & threads
 * in Solaris
 */
#define SOLARIS_MT	1

/*  Define the following only on platforms whose standard I/O
 *  implementation is so weak that we wouldn't be able to fopen
 *  a file whose underlying file descriptor would be > 255.
 *  Hey, we're not running on PDP-11's any more: would it kill you
 *  to use a short instead of a char to hold the fileno?  :-(
 */
 
/* Why we (solarises) need to rewrite old BSD stdio
   so many times I suggest stdIO from 
   http://www.research.att.com/sw/tools/sfio/ 
*/       
/* 	Need to use full sfio not just stdio emulation to fix
	file descriptor number limit. nmcc Dec2002
*/
#if (!defined(SFIO) && defined(SUPERSERVER))
#error "need to use SFIO"
#endif

#ifdef SOLX86
#define LSEEK_OFFSET_CAST (off_t)
#endif

#define MEMMOVE(from,to,length)       memmove ((void *)to, (void *)from, (size_t) length)
/*********   Reason for introducing MEMMOVE macro.

  void *memcpy( void *s1, const void *s2, size_t n);
  void *memmove( void *s1, const void *s2, size_t n);

  The memcpy() function copies n characters from the string pointed to by the
  s2 parameter into the location pointed to by the s1 parameter.  When copy-
  ing overlapping strings, the behavior of this function is unreliable.

  The memmove() function copies n characters from the string at the location
  pointed to by the s2 parameter to the string at the location pointed to by
  the s1 parameter.  Copying takes place as though the n number of characters
  from string s2 are first copied into a temporary location having n bytes
  that do not overlap either of the strings pointed to by s1 and s2. Then, n
  number of characters from the temporary location are copied to the string
  pointed to by s1. Consequently, this operation is nondestructive and
  proceeds from left to right.
  The above text is taken from the Digital UNIX man pages.

     For maximum portability, memmove should be used when the memory areas
     indicated by s1 and s2 may overlap, and memcpy used for faster copying
     between non-overlapping areas.

**********/

/* The following define is the prefix to go in front of a "d" or "u"
   format item in a ib_printf() format string, to indicate that the argument
   is an SINT64 or UINT64. */
#define QUADFORMAT "ll"
/* The following macro creates a quad-sized constant, possibly one
   which is too large to fit in a long int. */
#define QUADCONST(n) (n##LL)

#else /* SOLARIS */

#define BSD_UNIX        1

#endif /* SOLARIS */

#define UNIX            1
#define                 IEEE

#ifdef sparc
/*#define ALIGNMENT       4*/
/*#define DOUBLE_ALIGN    8*/
#define IMPLEMENTATION  isc_info_db_impl_isc_sun4 /* 30 */
#else /* sparc */

#ifdef i386
#define I386            1
#define IMPLEMENTATION  isc_info_db_impl_isc_sun_386i  /* 32 */
#else /* i386 */
#define IMPLEMENTATION  isc_info_db_impl_isc_sun_68k /* 28 */
#endif /* i386 */

#endif /* sparc */

#define MOVE_FAST(from,to,length)       memcpy (to, from, (int) (length))
#define MOVE_FASTER(from,to,length)     memcpy (to, from, (int) (length))
#define MOVE_CLEAR(to,length)           memset (to, 0, (int) (length))

typedef RETSIGTYPE (*SIG_FPTR) (int);
#endif /* sun */


/*****************************************************
* HP/UX platforms 
*****************************************************/

#if (defined(hpux) || defined(HPUX))

#ifndef hpux
#define hpux
#endif

#define KILLER_SIGNALS
#define UNIX            1
#define CURSES_KEYPAD   1

/*#define ALIGNMENT       8*/
/*#define DOUBLE_ALIGN    8*/
#define IMPLEMENTATION  isc_info_db_impl_isc_hp_ux /* 31 */

#define                 IEEE
#pragma OPT_LEVEL 1
// 16-Apr-2002 HP10 in unistd.h Paul Beach
//#define setreuid(ruid,euid)     setresuid (ruid, euid, -1)
//#define setregid(rgid,egid)     setresgid (rgid, egid, -1)

/* The following define is the prefix to go in front of a "d" or "u"
   format item in a ib_printf() format string, to indicate that the argument
   is an SINT64 or UINT64. */
#define QUADFORMAT "ll"
/* The following macro creates a quad-sized constant, possibly one
   which is too large to fit in a long int. */
#define QUADCONST(n) (n##LL)

#define MEMMOVE(from,to,length)       memmove ((void *)to, (void *)from, (size_t) length)
#define MOVE_FAST(from,to,length)       memcpy (to, from, (int) (length))
#define MOVE_FASTER(from,to,length)     memcpy (to, from, (int) (length))
#define MOVE_CLEAR(to,length)           memset (to, 0, (int) (length))

typedef RETSIGTYPE (*SIG_FPTR) ();
#endif /* hpux */


/*****************************************************
* DEC VAX/VMS and AlphaVMS 
*****************************************************/
#ifdef VMS
#define VAX_FLOAT       1
/*#define ALIGNMENT       4*/
#define NO_NFS
#define CTO32L(p)       (*(long*)p)
#define NO_CHECKSUM	1
#define ISC_EXT_LIB_PATH_ENV	"interbase_ext_lib_path:"
#define SYS_ARG		isc_arg_vms

#if __ALPHA
#define IMPLEMENTATION  isc_info_db_impl_alpha_vms /* 53 */
#include <ints.h>
#define ATOM_DEFINED
typedef int64 SATOM;			/* 64 bit */
typedef unsigned int64 UATOM;
#else
#define IMPLEMENTATION  isc_info_db_impl_isc_vms /* 27 */
#endif /* __ALPHA */

#define FINI_OK         1
#define FINI_ERROR      44
#define STARTUP_ERROR   46		/* this is also used in iscguard.h, make sure these match */

typedef RETSIGTYPE (*SIG_FPTR) ();
#endif /* VMS */



/*****************************************************
* IBM AIX RS/6000 and IBM AIX PowerPC 
*****************************************************/

#ifdef _AIX						/* IBM AIX */
#ifndef _POWER					/* IBM RS/6000 */
#define AIX
#define KILLER_SIGNALS
#define UNIX            1
#define CURSES_KEYPAD   1
/*#define ALIGNMENT       4*/
#define IMPLEMENTATION  isc_info_db_impl_isc_rt_aix /* 35 */
#define                 IEEE
#define MEMMOVE(from,to,length)       memmove ((void *)to, (void *)from, (size_t) length)
#define MOVE_FAST(from,to,length)       memcpy (to, from, (int) (length))
#define MOVE_FASTER(from,to,length)     memcpy (to, from, (int) (length))
#define MOVE_CLEAR(to,length)           memset (to, 0, (int) (length))
#define SYSCALL_INTERRUPTED(err)        (((err) == EINTR) || ((err) == ERESTART))	/* pjpg 20001102 */
#else /* AIX PowerPC */
#define AIX_PPC
#define KILLER_SIGNALS
#define UNIX            1
#define CURSES_KEYPAD   1
/*#define ALIGNMENT       4*/
#define IMPLEMENTATION  isc_info_db_impl_isc_rt_aix /* 35 */
#define                 IEEE
#define MEMMOVE(from,to,length)       memmove ((void *)to, (void *)from, (size_t) length)
#define MOVE_FAST(from,to,length)       memcpy (to, from, (int) (length))
#define MOVE_FASTER(from,to,length)     memcpy (to, from, (int) (length))
#define MOVE_CLEAR(to,length)           memset (to, 0, (int) (length))
#define SYSCALL_INTERRUPTED(err)        (((err) == EINTR) || ((err) == ERESTART))	/* pjpg 20001102 */

#define VA_START(list,parmN)    va_start (list, parmN)	/* TMC 081700 */
#define QUADFORMAT "ll"			/* TMC 081700 */
#define QUADCONST(n) (n##LL)	/* TMC 081700 */

#endif /* IBM PowerPC */

typedef RETSIGTYPE (*SIG_FPTR) ();
#endif /* IBM AIX */



/*****************************************************
* Windows NT 
*****************************************************/

#ifdef WIN_NT
#define NO_NFS

#define MOVE_FAST(from,to,length)       memcpy (to, from, (int) (length))
#define MOVE_FASTER(from,to,length)     memcpy (to, from, (int) (length))
#define MOVE_CLEAR(to,length)           memset (to, 0, (int) (length))
#define MEMMOVE(from,to,length)         memmove ((void *)to, (void *)from, (size_t) length)

#define SYS_ARG		isc_arg_win32
#define SLONGFORMAT	"ld"
#define ULONGFORMAT	"lu"
#define XLONGFORMAT "lX"
#define xLONGFORMAT "lx"
#define __LINE__IS_INT

typedef __int64 SINT64;
typedef unsigned __int64 UINT64;
#define INT64_DEFINED

/* The following define is the prefix to go in front of a "d" or "u"
   format item in a ib_printf() format string, to indicate that the argument
   is an SINT64 or UINT64. */
#define QUADFORMAT "I64"
/* The following macro creates a quad-sized constant, possibly one
   which is too large to fit in a long int.  The Microsoft compiler does
   not permit the LL suffix which some other platforms require, but it
   handles numbers up to the largest 64-bit integer correctly without such
   a suffix, so the macro definition is trivial. */
#define QUADCONST(n) (n)

#ifdef _X86_
#ifndef I386
#define I386
#endif
#define IMPLEMENTATION  isc_info_db_impl_isc_winnt_x86 /* 50 */
#endif

#define                 IEEE
#define VA_START(list,parmN)    va_start (list, parmN)
#define API_ROUTINE     __stdcall
#define API_ROUTINE_VARARG      __cdecl
#define CLIB_ROUTINE    __cdecl
#define THREAD_ROUTINE  __stdcall
#define INTERNAL_API_ROUTINE	API_ROUTINE

#define SYNC_WRITE_DEFAULT      1

#ifndef DLL_EXPORT
#define DLL_EXPORT
#endif

//#define BOOLEAN_DEFINED
//typedef unsigned char BOOLEAN;

#ifndef MAXPATHLEN
#ifdef MAX_PATH
#define MAXPATHLEN MAX_PATH
#else
#define MAXPATHLEN 260
#endif
#endif

typedef RETSIGTYPE (CLIB_ROUTINE * SIG_FPTR) (int);
#endif /* WIN_NT */

// 23 Sep 2002, skidder, ALLOC_LIB_MEMORY moved here,
// DEBUG_GDS_ALLOC definition removed because allocators 
// do not (and can not) include this file,
// but use DEBUG_GDS_ALLOC. Hence DEBUG_GDS_ALLOC should be defined
// globally by now and moved to autoconf-generated header later
#ifdef DEBUG_GDS_ALLOC
#define ALLOC_LIB_MEMORY(size)   gds__alloc_debug ((size),(TEXT *)__FILE__,(ULONG)__LINE__)
#endif

/*****************************************************
* SCO 
*****************************************************/
#ifdef SCO_EV

#define QUADFORMAT "ll"
#define QUADCONST(n) (n##LL)
#define I386            1
#define UNIX            1
#define SCO_UNIX        1
#define                 IEEE
/*
#define KILLER_SIGNALS
*/
#define IMPLEMENTATION  isc_info_db_impl_sco_ev /* 59 */
#define MEMMOVE(from,to,length)       memmove ((void *)to, (void *)from, (size_t) length)
#define MOVE_FAST(from,to,length)    memcpy (to, from, (unsigned int) (length))
#define MOVE_FASTER(from,to,length)  memcpy (to, from, (unsigned int) (length))
#define MOVE_CLEAR(to,length)        memset (to, 0, (unsigned int) (length))

/*  These functions are supported so we don't need the defines
#define setreuid(ruid,euid)     setuid(euid)
#define setregid(rgid,egid)     setgid(egid)
*/

typedef RETSIGTYPE (*SIG_FPTR) ();
#endif /* SCO_EV */

/*****************************************************
 * UNIX
*****************************************************/
#ifdef UNIX
#define NO_CHECKSUM     1
#define SYS_ARG		isc_arg_unix
#endif /* UNIX */


/* various declaration modifiers */

#ifndef API_ROUTINE
#define API_ROUTINE
#define API_ROUTINE_VARARG
#define INTERNAL_API_ROUTINE	API_ROUTINE
#endif

#ifndef CLIB_ROUTINE
#define CLIB_ROUTINE
#endif

#ifndef THREAD_ROUTINE
#define THREAD_ROUTINE
#endif



/* alignment macros */

#ifndef OLD_ALIGNMENT
#ifdef I386
/* Using internal alignment optimal for 386 processor and above
 */
/*#define ALIGNMENT       4*/
/*#define DOUBLE_ALIGN    8*/
#endif
#endif

#ifndef ALIGNMENT
/*#define ALIGNMENT       2*/
#error must define ALIGNMENT for your system
#endif

#ifndef SHIFTLONG
/* Number of shifts needed to convert between char and LONG */
#define SHIFTLONG       2
#define BITS_PER_LONG   32
#define LOG2_BITS_PER_LONG      5
#endif

#ifndef DOUBLE_ALIGN
/*#define DOUBLE_ALIGN    4*/
#error must define DOUBLE_ALIGN for your system
#endif



/* common return values */

#ifndef FINI_OK
#define FINI_OK         0
#define FINI_ERROR      1
#define STARTUP_ERROR   2		/* this is also used in iscguard.h, make sure these match */
#endif

#ifndef NULL
#define NULL            0L
#endif

#define NULL_PTR        ((void*) 0)
#ifndef TRUE
#define TRUE            1
#endif
#ifndef FALSE
#define FALSE           0
#endif
#define FB_SUCCESS         0
#define FB_FAILURE         1

/* sys/paramh.h : compatibility purposes */
#ifndef NOFILE
#ifdef VMS
#define NOFILE      32
#else
#define NOFILE      20
#endif
#endif

/* data type definitions */


#ifndef INT64_DEFINED			/* 64 bit */
typedef long long int SINT64;
typedef unsigned long long int UINT64;
#else
#undef INT64_DEFINED
#endif


#ifndef ATOM_DEFINED			/* 32 or 64 bit */
typedef long SATOM;
typedef unsigned long UATOM;
#else
#undef ATOM_DEFINED
#endif

/*
#ifndef BOOLEAN_DEFINED
typedef USHORT BOOLEAN;
#else
#undef BOOLEAN_DEFINED
#endif
*/

#ifndef ISC_TIMESTAMP_DEFINED
typedef SLONG ISC_DATE;
typedef ULONG ISC_TIME;
typedef struct
{
	ISC_DATE timestamp_date;
	ISC_TIME timestamp_time;
} ISC_TIMESTAMP;
#define ISC_TIMESTAMP_DEFINED
#endif	/* ISC_TIMESTAMP_DEFINED */

#define GDS_DATE	ISC_DATE
#define GDS_TIME	ISC_TIME
#define GDS_TIMESTAMP	ISC_TIMESTAMP


#ifndef ENUM
#define ENUM            enum
#endif

#ifndef BLOB_PTR
#define BLOB_PTR        UCHAR
#endif


#ifndef SLONGFORMAT
#define SLONGFORMAT	"d"
#define ULONGFORMAT	"u"
#define XLONGFORMAT "X"
#define xLONGFORMAT "x"
#endif



/* variable argument definitions */

#define VA_START(list,parmN)    va_start (list, parmN)

/* conditional compilation macros */



/* MAX and MIN for datatypes */

#define MAX_UCHAR		((UCHAR)0xFF)
#define MIN_UCHAR		0x00

#define MAX_SCHAR		0x7F
#define MIN_SCHAR		(-MAX_SCHAR-1)

#define MAX_USHORT		((USHORT)0xFFFF)
#define MIN_USHORT		0x0000

#define MAX_SSHORT		0x7FFF
#define MIN_SSHORT		(-MAX_SSHORT-1)

#define MAX_ULONG		((ULONG)0xFFFFFFFF)
#define MIN_ULONG		0x00000000

#define MAX_SLONG		0x7FFFFFFF
#define MIN_SLONG		(-MAX_SLONG-1)

#define MAX_UINT64              ((UINT64) QUADCONST(0xFFFFFFFFFFFFFFFF))
#define MIN_UINT64              QUADCONST(0x0000000000000000)

#define MAX_SINT64              QUADCONST(0x7FFFFFFFFFFFFFFF)
#define MIN_SINT64              (-MAX_SINT64-1)



/* commonly used macros */

#ifndef MAX
#define MAX(a,b)                (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b)                (((a) < (b)) ? (a) : (b))
#endif

#define UPPER(c)                (((c) >= 'a' && (c)<= 'z') ? (c) - 'a' + 'A' : (c))
#define LOWWER(c)               (((c) >= 'A' && (c)<= 'Z') ? (c) - 'A' + 'a' : (c))

/* UPPER7 & LOWWER7 are to be used when the data is known to
   be 7-bit ASCII, eg: internal data, OS data.  NOT user data.  */

#define UPPER7(c)               (((c) >= 'a' && (c)<= 'z') ? (c) - 'a' + 'A' : (c))
#define LOWWER7(c)              (((c) >= 'A' && (c)<= 'Z') ? (c) - 'A' + 'a' : (c))

#ifndef ROUNDUP
#define ROUNDUP(n,b)            (((n) + (b) - 1) & ~((b) - 1))
#endif
#define ROUNDUP_LONG(len)       ROUNDUP (len, sizeof (SLONG))

#define JRD_BUGCHK 15			/* facility code for bugcheck messages */
#ifndef OFFSET
#define OFFSET(struct,fld)      ((int) &((struct) 0)->fld)
#define OFFSETA(struct,fld)     ((int) ((struct) 0)->fld)
#endif

#ifndef ODS_ALIGNMENT
#define ODS_ALIGNMENT           4
#endif

#ifndef SYSCALL_INTERRUPTED
#define SYSCALL_INTERRUPTED(err)        ((err) == EINTR)
#endif



/* data conversion macros */

#ifndef CTO32L
#ifndef WORDS_BIGENDIAN
#define CTO32L(p) ((((SCHAR*)(p))[3] << 24) | (((UCHAR*)(p))[2] << 16) | (((UCHAR*)(p))[1] << 8) | (((UCHAR*)(p)) [0]))
#else
#define CTO32L(p) ((((SCHAR*)(p))[0] << 24) | (((UCHAR*)(p))[1] << 16) | (((UCHAR*)(p))[2] << 8) | (((UCHAR*)(p)) [3]))
#endif
#endif

#ifndef AOF32L
#define AOF32L(l)               &l
#endif



/* data movement and allocation macros */

#ifndef MOVE_FAST
#define MOVE_FAST(from,to,length)       MOV_fast (from, to, (ULONG) (length))
#endif

#ifndef MOVE_FASTER
#define MOVE_FASTER(from,to,length)     MOV_faster (from, to, (ULONG) (length))
#endif

#ifndef MEMMOVE
/* Use character by character copy function */
#define MEMMOVE(from,to,length)       MOV_fast (from, to, (ULONG) (length))
#endif

#ifndef MOVE_CLEAR
#define MOVE_CLEAR(to,length)           MOV_fill (to, (ULONG) (length))
#endif

#ifndef ALLOC_LIB_MEMORY
#define ALLOC_LIB_MEMORY(size)          gds__alloc (size)
#endif

#ifndef FREE_LIB_MEMORY
#define FREE_LIB_MEMORY(block)          gds__free (block)
#endif



#ifdef DEV_BUILD

/* Define any debugging symbols and macros here.  This
   ifdef will be executed during development builds. */

#ifdef WIN_NT
#define TRACE(msg)              gds__log (msg)
#define DEV_REPORT(msg)         gds__log (msg)
#endif

#ifndef TRACE
#define TRACE(msg)              ib_fprintf (ib_stderr, (msg))
#endif

#ifndef DEV_REPORT
#define DEV_REPORT(msg)         ib_fprintf (ib_stderr, (msg))
#endif

#ifndef BREAKPOINT
#define BREAKPOINT(x)           GDS_breakpoint(x)
// fwd. decl. the function itself
#ifdef __cplusplus
extern "C" {
#endif
void GDS_breakpoint(int);
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif

#endif /* DEV_BUILD */

#ifndef DEV_BUILD
#define DEV_REPORT(msg)         gds__log (msg)
#define BREAKPOINT(x)			/* nothing */
#define TRACE(msg)				/* nothing */
#endif



/* shared library definitions */

#ifdef SHLIB_DEFS
#ifndef LOCAL_SHLIB_DEFS
#include "../jrd/shdef.h"
#endif
#endif

#ifndef DLL_EXPORT
#define DLL_EXPORT
#endif



/* commonly used buffer sizes for dynamic buffer allocation */

#define BUFFER_XLARGE   2048
#define BUFFER_LARGE    1024
#define BUFFER_MEDIUM   512
#define BUFFER_SMALL    256
#define BUFFER_TINY     128

/* The default lseek offset type.  Changed from nothing to (off_t) to correctly support 64 bit IO */
#ifndef LSEEK_OFFSET_CAST
#define LSEEK_OFFSET_CAST (off_t)
#endif

#ifndef DOUBLE_MULTIPLY
#define DOUBLE_MULTIPLY(a,b)    (((double) (a)) * ((double) (b)))
#endif

#ifndef DOUBLE_DIVIDE
#define DOUBLE_DIVIDE(a,b)      (((double) (a)) / ((double) (b)))
#endif

#ifndef ISC_EXT_LIB_PATH_ENV
#define ISC_EXT_LIB_PATH_ENV	"INTERBASE_EXT_LIB_PATH"
#endif

/* switch name and state table.  This structure should be used in all
 * command line tools to facilitate parsing options.*/
typedef struct in_sw_tab_t {
	int in_sw;
	int in_spb_sw;
	TEXT *in_sw_name;
	ULONG in_sw_value;			/* alice specific field */
	ULONG in_sw_requires;		/* alice specific field */
	ULONG in_sw_incompatibilities;	/* alice specific field */
	USHORT in_sw_state;
	USHORT in_sw_msg;
	USHORT in_sw_min_length;
	TEXT *in_sw_text;

} *IN_SW_TAB;

#ifndef HAVE_WORKING_VFORK
#define vfork fork
#endif

#endif /* JRD_COMMON_H */
