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

#ifndef JRD_COMMON_H
#define JRD_COMMON_H

#include "firebird.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#include "../include/fb_macros.h"
#include "../include/fb_types.h"

/*
  do not use links in source code to maintain platform neutrality
*/

#ifdef PROD_BUILD
#ifdef DEV_BUILD
#undef DEV_BUILD
#endif
#endif

#ifdef MULTI_THREAD
# ifdef SUPERSERVER
#  define SWEEP_THREAD
#  define GARBAGE_THREAD
# else
#  define AST_THREAD
# endif
#endif

#ifdef SUPERSERVER
#define GOVERNOR
#define CANCEL_OPERATION
#define FB_ARCHITECTURE isc_info_db_class_server_access
#else
#define FB_ARCHITECTURE isc_info_db_class_classic_access
#endif

#define EXPRESSION_INDICES

/*****************************************************
* Linux for Intel platforms 
*****************************************************/
#ifdef LINUX
#define QUADFORMAT "ll"
#define QUADCONST(n) (n##LL)

// SLONG is a 32-bit integer on 64-bit platforms
//#if SIZEOF_LONG == 4
//#define SLONGFORMAT "ld"
//#define ULONGFORMAT "lu"
//#define XLONGFORMAT "lX"
//#define xLONGFORMAT "lx"
//#endif


//format for __LINE__
#define LINEFORMAT "d"

#ifdef SUPERSERVER
#define SET_TCP_NO_DELAY
#endif

//#define KILLER_SIGNALS

#define UNIX
#define IEEE

#ifdef AMD64
#define IMPLEMENTATION  isc_info_db_impl_linux_amd64 /* 66  next higher unique number, See you later  */
#endif

#ifdef i386
#define I386
#define IMPLEMENTATION  isc_info_db_impl_i386 /* 60  next higher unique number, See you later  */
#endif /* i386 */

#ifdef sparc
#define IMPLEMENTATION  isc_info_db_impl_linux_sparc /* 65  */
#endif /* sparc */

#define MEMMOVE(from, to, length)		memmove ((void *)to, (void *)from, (size_t) length)
#define MOVE_FAST(from, to, length)       memcpy (to, from, (int) (length))
#define MOVE_FASTER(from, to, length)     memcpy (to, from, (int) (length))
#define MOVE_CLEAR(to, length)           memset (to, 0, (int) (length))

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
int strncasecmp(const char *s1, const char *s2, size_t n);
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
#define dlopen(a, b)		dlopen((char *)(a),(b))
#define dlsym(a, b)		dlsym((a), (char *)(b))

#include <signal.h>
#include <sys/siginfo.h>

struct sinixz_sigaction
  {
    int sa_flags;
    union
      {
        /* Used if SA_SIGINFO is not set.  */
        void (*sa_handler)(int);
        /* Used if SA_SIGINFO is set.  */
        void (*sa_sigaction) (int, siginfo_t *, void *);
      }
    __sigaction_handler;
#define sa_handler		__sigaction_handler.sa_handler
#define sa_sigaction		__sigaction_handler.sa_sigaction
    sigset_t sa_mask;
    int sa_resv[2];
  };

static inline int sinixz_sigaction(int sig, const struct sinixz_sigaction *act,
                                   struct sinixz_sigaction *oact)
{
  return sigaction(sig, (struct sigaction*)act, (struct sigaction*)oact);
}

// Re-define things actually
#define sigaction		sinixz_sigaction

#define QUADFORMAT "ll"
#define QUADCONST(n) (n##LL)

//#define ALIGNMENT	4
//#define DOUBLE_ALIGN	8

#ifdef SUPERSERVER
#define SET_TCP_NO_DELAY
#endif

//#define KILLER_SIGNALS

#define UNIX
#define IEEE

#ifdef i386
#define I386
/* Change version string into SINIXZ */
#define IMPLEMENTATION  isc_info_db_impl_sinixz  /* 64 */
#endif /* i386 */

#define setreuid(ruid, euid)     setuid(euid)
#define setregid(rgid, egid)     setgid(egid)

#define MEMMOVE(from, to, length)		memmove ((void *)to, (void *)from, (size_t) length)
#define MOVE_FAST(from, to, length)       memcpy (to, from, (int) (length))
#define MOVE_FASTER(from, to, length)     memcpy (to, from, (int) (length))
#define MOVE_CLEAR(to, length)           memset (to, 0, (int) (length))

//format for __LINE__
#define LINEFORMAT "d"

//#define SLONGFORMAT "ld"
//#define ULONGFORMAT "lu"
//#define XLONGFORMAT "lX"
//#define xLONGFORMAT "lx"

#define O_BINARY 0
#endif /* SINIXZ */

/*****************************************************
* Darwin Platforms 
*****************************************************/
#ifdef DARWIN
// EKU: obsolete, replaced by _FILE_OFFSET_BITS
//#ifndef UNIX_64_BIT_IO
//#define UNIX_64_BIT_IO
//#endif
//
//format for __LINE__
#define LINEFORMAT "d"

//#define SLONGFORMAT	"ld"
//#define ULONGFORMAT "lu"
//#define XLONGFORMAT "lX"
//#define xLONGFORMAT "lx"

//#define ALIGNMENT       4
//#define DOUBLE_ALIGN    4
//#define BSD_UNIX
#define UNIX
#define IMPLEMENTATION  63
#define IEEE
#define QUADCONST(n) (n##LL)
#define QUADFORMAT "q"
#define MAP_ANONYMOUS
#define MAP_ANNON

#define MEMMOVE(from, to, length)		memmove ((void *)to, (void *)from, (size_t)length)
#define MOVE_FAST(from, to, length)	memcpy (to, from, (int) (length))
#define MOVE_FASTER(from, to, length)	memcpy (to, from, (int) (length))
#define MOVE_CLEAR(to, length)		memset (to, 0, (int) (length))

#endif /* Darwin Platforms */


/*****************************************************
* FreeBSD for Intel platforms 
*****************************************************/
#ifdef FREEBSD

// EKU: obsolete, replaced by _FILE_OFFSET_BITS
//#ifndef UNIX_64_BIT_IO
//#define UNIX_64_BIT_IO
//#endif
//

//#define ALIGNMENT     4
//#define DOUBLE_ALIGN  4

#define UNIX
#define IEEE
#define I386
#define IMPLEMENTATION    isc_info_db_impl_freebsd   /* 61 */

#define QUADFORMAT "ll"
#define QUADCONST(n) (n##LL)
//#define KILLER_SIGNALS
#define NO_NFS					/* no MTAB_OPEN or MTAB_CLOSE in isc_file.c */

#define MEMMOVE(from, to, length)     memmove ((void *)to, (void *)from, (size_t) length)
#define MOVE_FAST(from, to, length)       memcpy (to, from, (int) (length))
#define MOVE_FASTER(from, to, length)     memcpy (to, from, (int) (length))
#define MOVE_CLEAR(to, length)           memset (to, 0, (int) (length))

#endif /* FREEBSD */

/*****************************************************
* NetBSD 
*****************************************************/
#ifdef NETBSD

#if defined(__i386__)
//#define ALIGNMENT     4
//#define DOUBLE_ALIGN  4

#define IEEE
#define I386
#define IMPLEMENTATION        isc_info_db_impl_netbsd /* 62 */

#define QUADFORMAT "ll"
#define QUADCONST(n) (n##LL)
#else /* !__i386__ */
#error Please add support for other ports
#endif

#define UNIX

//#define KILLER_SIGNALS
#define NO_NFS					/* no MTAB_OPEN or MTAB_CLOSE in isc_file.c */

#define MEMMOVE(from, to, length)     memmove ((void *)(to), (void *)(from), (size_t) length)
#define MOVE_FAST(from, to, length)       memcpy ((to), (from), (int) (length))
#define MOVE_FASTER(from, to, length)     memcpy ((to), (from), (int) (length))
#define MOVE_CLEAR(to, length)           memset ((to), 0, (int) (length))

#endif /* NETBSD */



/*****************************************************
 * SUN platforms--the 386i is obsolete 
*****************************************************/

#ifdef sun
/* Defined KILLER_SIGNALS for Sun - as we were getting lots of lockups
 * using pipe server.
 * 1995-February-24 David Schnepper
 */
//#define KILLER_SIGNALS

#ifdef SOLARIS

/* This is NOT defined when building the special shared-pipe library
 * which customers can use to avoid the problems with signals & threads
 * in Solaris
 */
#define SOLARIS_MT

#ifdef SOLARIS_MT
#define ANY_THREADING
#define MULTI_THREAD
#endif
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

#define MEMMOVE(from, to, length)       memmove ((void *)to, (void *)from, (size_t) length)
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
   format item in a printf() format string, to indicate that the argument
   is an SINT64 or UINT64. */
#define QUADFORMAT "ll"
/* The following macro creates a quad-sized constant, possibly one
   which is too large to fit in a long int. */
#define QUADCONST(n) (n##LL)

//#else /* SOLARIS */

//#define BSD_UNIX

#endif /* SOLARIS */

#define UNIX
#define IEEE

#ifdef sparc
//#define ALIGNMENT       4
//#define DOUBLE_ALIGN    8
#define IMPLEMENTATION  isc_info_db_impl_isc_sun4 /* 30 */
#else /* sparc */

#ifdef i386
#define I386
#define IMPLEMENTATION  isc_info_db_impl_isc_sun_386i  /* 32 */
#else /* i386 */
#define IMPLEMENTATION  isc_info_db_impl_isc_sun_68k /* 28 */
#endif /* i386 */

#endif /* sparc */

#define MOVE_FAST(from, to, length)       memcpy (to, from, (int) (length))
#define MOVE_FASTER(from, to, length)     memcpy (to, from, (int) (length))
#define MOVE_CLEAR(to, length)            memset (to, 0, (int) (length))

#endif /* sun */


/*****************************************************
* HP/UX platforms 
*****************************************************/

#if (defined(hpux) || defined(HPUX))

#ifndef hpux
#define hpux
#endif

//#define KILLER_SIGNALS
#define UNIX
//#define CURSES_KEYPAD

//#define ALIGNMENT       8
//#define DOUBLE_ALIGN    8
#define IMPLEMENTATION  isc_info_db_impl_isc_hp_ux /* 31 */

#define IEEE
#pragma OPT_LEVEL 1
// 16-Apr-2002 HP10 in unistd.h Paul Beach
//#define setreuid(ruid, euid)     setresuid (ruid, euid, -1)
//#define setregid(rgid, egid)     setresgid (rgid, egid, -1)

/* The following define is the prefix to go in front of a "d" or "u"
   format item in a printf() format string, to indicate that the argument
   is an SINT64 or UINT64. */
#define QUADFORMAT "ll"
/* The following macro creates a quad-sized constant, possibly one
   which is too large to fit in a long int. */
#define QUADCONST(n) (n##LL)

#define MEMMOVE(from, to, length)       memmove ((void *)to, (void *)from, (size_t) length)
#define MOVE_FAST(from, to, length)       memcpy (to, from, (int) (length))
#define MOVE_FASTER(from, to, length)     memcpy (to, from, (int) (length))
#define MOVE_CLEAR(to, length)           memset (to, 0, (int) (length))

#endif /* hpux */


/*****************************************************
* DEC VAX/VMS and AlphaVMS 
*****************************************************/
#ifdef VMS
#define VAX_FLOAT
//#define ALIGNMENT       4
#define NO_NFS
#define NO_CHECKSUM
#define SYS_ARG		isc_arg_vms
#define SYS_ERR		isc_arg_vms

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

#endif /* VMS */



/*****************************************************
* IBM AIX RS/6000 and IBM AIX PowerPC 
*****************************************************/

#ifdef _AIX						/* IBM AIX */
#ifndef _POWER					/* IBM RS/6000 */
#define AIX
//#define KILLER_SIGNALS
#define UNIX
//#define CURSES_KEYPAD
//*#define ALIGNMENT       4
#define IMPLEMENTATION  isc_info_db_impl_isc_rt_aix /* 35 */
#define IEEE
#define MEMMOVE(from, to, length)       memmove ((void *)to, (void *)from, (size_t) length)
#define MOVE_FAST(from, to, length)       memcpy (to, from, (int) (length))
#define MOVE_FASTER(from, to, length)     memcpy (to, from, (int) (length))
#define MOVE_CLEAR(to, length)           memset (to, 0, (int) (length))
#define SYSCALL_INTERRUPTED(err)        (((err) == EINTR) || ((err) == ERESTART))	/* pjpg 20001102 */
#else /* AIX PowerPC */
#define AIX_PPC
//#define KILLER_SIGNALS
#define UNIX
//#define CURSES_KEYPAD
//#define ALIGNMENT       4
#define IMPLEMENTATION  isc_info_db_impl_isc_rt_aix /* 35 */
#define IEEE
#define MEMMOVE(from, to, length)       memmove ((void *)to, (void *)from, (size_t) length)
#define MOVE_FAST(from, to, length)       memcpy (to, from, (int) (length))
#define MOVE_FASTER(from, to, length)     memcpy (to, from, (int) (length))
#define MOVE_CLEAR(to, length)           memset (to, 0, (int) (length))
#define SYSCALL_INTERRUPTED(err)        (((err) == EINTR) || ((err) == ERESTART))	/* pjpg 20001102 */

#define QUADFORMAT "ll"			/* TMC 081700 */
#define QUADCONST(n) (n##LL)	/* TMC 081700 */

#endif /* IBM PowerPC */

#endif /* IBM AIX */



/*****************************************************
* Windows NT 
*****************************************************/

#ifdef WIN_NT

#define NO_NFS

#define MOVE_FAST(from, to, length)       memcpy (to, from, (int) (length))
#define MOVE_FASTER(from, to, length)     memcpy (to, from, (int) (length))
#define MOVE_CLEAR(to, length)           memset (to, 0, (int) (length))
#define MEMMOVE(from, to, length)         memmove ((void *)to, (void *)from, (size_t) length)

#define SYS_ARG		isc_arg_win32
#define SYS_ERR		isc_arg_win32
//#define SLONGFORMAT	"ld"
//#define ULONGFORMAT	"lu"
//#define XLONGFORMAT "lX"
//#define xLONGFORMAT "lx"

//format for __LINE__
#define LINEFORMAT "d"

typedef __int64 SINT64;
typedef unsigned __int64 UINT64;
#define INT64_DEFINED

/* The following define is the prefix to go in front of a "d" or "u"
   format item in a printf() format string, to indicate that the argument
   is an SINT64 or UINT64. */
#define QUADFORMAT "I64"
/* The following macro creates a quad-sized constant, possibly one
   which is too large to fit in a long int.  The Microsoft compiler does
   not permit the LL suffix which some other platforms require, but it
   handles numbers up to the largest 64-bit integer correctly without such
   a suffix, so the macro definition is trivial. */
#ifdef MINGW // needed for gcc 3.3.1
#define QUADCONST(n) (n##LL)
#else
#define QUADCONST(n) (n)
#endif

#ifdef _X86_
#ifndef I386
#define I386
#endif
#define IMPLEMENTATION  isc_info_db_impl_isc_winnt_x86 /* 50 */
#endif

#define IEEE
#define API_ROUTINE     __stdcall
#define API_ROUTINE_VARARG      __cdecl
#define CLIB_ROUTINE    __cdecl
#define INTERNAL_API_ROUTINE	API_ROUTINE

#define SYNC_WRITE_DEFAULT

#ifndef MAXPATHLEN
#ifdef MAX_PATH
#define MAXPATHLEN MAX_PATH
#else
#define MAXPATHLEN 260
#endif
#endif

#define ERRNO		GetLastError()
#define INET_ERRNO	WSAGetLastError()
#define H_ERRNO		WSAGetLastError()

#endif /* WIN_NT */

// 23 Sep 2002, skidder, ALLOC_LIB_MEMORY moved here,
// DEBUG_GDS_ALLOC definition removed because allocators 
// do not (and can not) include this file,
// but use DEBUG_GDS_ALLOC. Hence DEBUG_GDS_ALLOC should be defined
// globally by now and moved to autoconf-generated header later
#ifdef DEBUG_GDS_ALLOC
#define ALLOC_LIB_MEMORY(size)   gds__alloc_debug ((size), (TEXT *)__FILE__, (ULONG)__LINE__)
#endif

/*****************************************************
* SCO 
*****************************************************/
#ifdef SCO_EV

#define QUADFORMAT "ll"
#define QUADCONST(n) (n##LL)
#define I386
#define UNIX
#define SCO_UNIX
#define IEEE
//
//#define KILLER_SIGNALS
//
#define IMPLEMENTATION  isc_info_db_impl_sco_ev /* 59 */
#define MEMMOVE(from, to, length)       memmove ((void *)to, (void *)from, (size_t) length)
#define MOVE_FAST(from, to, length)    memcpy (to, from, (unsigned int) (length))
#define MOVE_FASTER(from, to, length)  memcpy (to, from, (unsigned int) (length))
#define MOVE_CLEAR(to, length)        memset (to, 0, (unsigned int) (length))

//  These functions are supported so we don't need the defines
//#define setreuid(ruid, euid)     setuid(euid)
//#define setregid(rgid, egid)     setgid(egid)
//

#endif /* SCO_EV */

/*****************************************************
 * UNIX
*****************************************************/
#ifdef UNIX
#define NO_CHECKSUM
#define SYS_ARG		isc_arg_unix
#define SYS_ERR		isc_arg_unix
#endif /* UNIX */

#ifndef SYS_ERR
#define SYS_ERR		isc_arg_unix
#endif

#ifndef ERRNO
#define ERRNO		errno
#endif

#ifndef INET_ERRNO
#define INET_ERRNO	errno
#endif

#ifndef H_ERRNO
#define H_ERRNO		h_errno
#endif

/* various declaration modifiers */

#ifndef API_ROUTINE
#define API_ROUTINE
#define API_ROUTINE_VARARG
#define INTERNAL_API_ROUTINE	API_ROUTINE
#endif

#ifndef CLIB_ROUTINE
#define CLIB_ROUTINE
#endif



/* alignment macros */

#ifndef OLD_ALIGNMENT
#ifdef I386
/* Using internal alignment optimal for 386 processor and above
 */
//#define ALIGNMENT       4
//#define DOUBLE_ALIGN    8
#endif
#endif

#ifndef ALIGNMENT
//#define ALIGNMENT       2
#error must define ALIGNMENT for your system
#endif

#ifndef SHIFTLONG
/* Number of shifts needed to convert between char and LONG */
#define SHIFTLONG       2
#define BITS_PER_LONG   32
#define LOG2_BITS_PER_LONG      5
#endif

#ifndef DOUBLE_ALIGN
//#define DOUBLE_ALIGN    4
#error must define DOUBLE_ALIGN for your system
#endif



/* common return values */

#ifndef FINI_OK
#define FINI_OK         0
#define FINI_ERROR      1
#define STARTUP_ERROR   2		/* this is also used in iscguard.h, make sure these match */
#endif

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

#ifndef ISC_TIMESTAMP_DEFINED
typedef SLONG ISC_DATE;
typedef ULONG ISC_TIME;
struct ISC_TIMESTAMP
{
	ISC_DATE timestamp_date;
	ISC_TIME timestamp_time;
};
#define ISC_TIMESTAMP_DEFINED
#endif	/* ISC_TIMESTAMP_DEFINED */

#define GDS_DATE	ISC_DATE
#define GDS_TIME	ISC_TIME
#define GDS_TIMESTAMP	ISC_TIMESTAMP


#ifndef BLOB_PTR
#define BLOB_PTR        UCHAR
#endif


#ifndef SLONGFORMAT
#if SIZEOF_LONG == 4
#define SLONGFORMAT "ld"
#define ULONGFORMAT "lu"
#define XLONGFORMAT "lX"
#define xLONGFORMAT "lx"
#else
#define SLONGFORMAT	"d"
#define ULONGFORMAT	"u"
#define XLONGFORMAT "X"
#define xLONGFORMAT "x"
#endif
#endif

//format for __LINE__
#ifndef LINEFORMAT
#define LINEFORMAT "ld"
#endif

/* conditional compilation macros */



/* MAX and MIN for datatypes */

#define MAX_UCHAR		((UCHAR)0xFF)
#define MIN_UCHAR		0x00

#define MAX_SCHAR		0x7F
#define MIN_SCHAR		(-MAX_SCHAR - 1)

#define MAX_USHORT		((USHORT)0xFFFF)
#define MIN_USHORT		0x0000

#define MAX_SSHORT		0x7FFF
#define MIN_SSHORT		(-MAX_SSHORT - 1)

#define MAX_ULONG		((ULONG)0xFFFFFFFF)
#define MIN_ULONG		0x00000000

#define MAX_SLONG		0x7FFFFFFF
#define MIN_SLONG		(-MAX_SLONG - 1)

#define MAX_UINT64              ((UINT64) QUADCONST(0xFFFFFFFFFFFFFFFF))
#define MIN_UINT64              QUADCONST(0x0000000000000000)

#define MAX_SINT64              QUADCONST(0x7FFFFFFFFFFFFFFF)
#define MIN_SINT64              (-MAX_SINT64 - 1)



/* commonly used macros */

#ifndef MAX
#define MAX(a, b)                (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b)                (((a) < (b)) ? (a) : (b))
#endif

#define UPPER(c)                (((c) >= 'a' && (c) <= 'z') ? (c) - 'a' + 'A' : (c))
#define LOWWER(c)               (((c) >= 'A' && (c) <= 'Z') ? (c) - 'A' + 'a' : (c))

/* UPPER7 & LOWWER7 are to be used when the data is known to
   be 7-bit ASCII, eg: internal data, OS data.  NOT user data.  */

#define UPPER7(c)               (((c) >= 'a' && (c) <= 'z') ? (c) - 'a' + 'A' : (c))
#define LOWWER7(c)              (((c) >= 'A' && (c) <= 'Z') ? (c) - 'A' + 'a' : (c))

#ifndef ROUNDUP
#define ROUNDUP(n, b)            (((n) + (b) - 1) & ~((b) - 1))
#endif
#define ROUNDUP_LONG(len)       ROUNDUP (len, sizeof (SLONG))

#define JRD_BUGCHK 15			/* facility code for bugcheck messages */
#ifndef OFFSET
#define OFFSET(struct, fld)      ((size_t) &((struct) NULL)->fld)
#define OFFSETA(struct, fld)     ((size_t) ((struct) NULL)->fld)
#endif

#ifndef ODS_ALIGNMENT
#define ODS_ALIGNMENT           4
#endif

#ifndef FORMAT_ALIGNMENT
// Alignment for items in record format. Used for databases after ODS11.
// Always 64-bit to ensure ODS compatibility with 64-bit versions of the engine
#define FORMAT_ALIGNMENT           8
#endif

#ifndef SYSCALL_INTERRUPTED
#define SYSCALL_INTERRUPTED(err)        ((err) == EINTR)
#endif



/* data conversion macros */

#ifndef AOF32L
#define AOF32L(l)               &l
#endif



/* data movement and allocation macros */

#ifndef MOVE_FAST
#define MOVE_FAST(from, to, length)       MOV_fast (from, to, (ULONG) (length))
#endif

#ifndef MOVE_FASTER
#define MOVE_FASTER(from, to, length)     MOV_faster (from, to, (ULONG) (length))
#endif

#ifndef MEMMOVE
/* Use character by character copy function */
#define MEMMOVE(from, to, length)       MOV_fast (from, to, (ULONG) (length))
#endif

#ifndef MOVE_CLEAR
#define MOVE_CLEAR(to, length)           MOV_fill (to, (ULONG) (length))
#endif

#ifndef ALLOC_LIB_MEMORY
#define ALLOC_LIB_MEMORY(size)          gds__alloc (size)
#endif

#ifndef FREE_LIB_MEMORY
#define FREE_LIB_MEMORY(block)          gds__free (block)
#endif

// This macros are used to workaround shortage of standard conformance
// in Microsoft compilers. They could be replaced with normal procedure
// and generic macro if MSVC would support C99-style __VA_ARGS__
#define DEFINE_TRACE_ROUTINE(routine) void routine(const char* message, ...)

#ifdef HAVE_VSNPRINTF
#define VSNPRINTF(a, b, c, d) vsnprintf(a, b, c, d)
#else
#define VSNPRINTF(a, b, c, d) vsprintf(a, c, d)
#endif

#ifdef HAVE_SNPRINTF
#define SNPRINTF snprintf
#else
#define SNPRINTF(buffer, length, ...) sprintf(buffer, __VA_ARGS__)
#endif

#define IMPLEMENT_TRACE_ROUTINE(routine, subsystem) \
void routine(const char* message, ...) { \
	static const char name_facility[] = subsystem ","; \
	char buffer[1000]; \
	strcpy(buffer, name_facility); \
	char *ptr = buffer + sizeof(name_facility) - 1; \
	va_list params; \
	va_start(params, message); \
	VSNPRINTF(ptr, sizeof(buffer) - sizeof(name_facility), message, params); \
	va_end(params); \
	gds__trace(buffer); \
}

// Shortcuts to make it easier to convert code using SLONGFORMAT/ULONGFORMAT
#define UQUADFORMAT QUADFORMAT"u"
#define SQUADFORMAT QUADFORMAT"d"

#ifdef DEV_BUILD

/* Define any debugging symbols and macros here.  This
   ifdef will be executed during development builds. */

#define TRACE(msg)				gds__trace (msg)

#ifndef DEV_REPORT
#define DEV_REPORT(msg)         gds__log (msg)
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
#ifndef DEV_REPORT
#define DEV_REPORT(msg)         gds__log (msg)
#endif
#ifndef BREAKPOINT
#define BREAKPOINT(x)			/* nothing */
#endif
#ifndef TRACE
#define TRACE(msg)				/* nothing */
#endif
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
#define DOUBLE_MULTIPLY(a, b)    (((double) (a)) * ((double) (b)))
#endif

#ifndef DOUBLE_DIVIDE
#define DOUBLE_DIVIDE(a, b)      (((double) (a)) / ((double) (b)))
#endif

/* switch name and state table.  This structure should be used in all
 * command line tools to facilitate parsing options.*/
struct in_sw_tab_t {
	int in_sw;
	int in_spb_sw;
	const TEXT* in_sw_name;
	ULONG in_sw_value;			/* alice specific field */
	ULONG in_sw_requires;		/* alice specific field */
	ULONG in_sw_incompatibilities;	/* alice specific field */
	USHORT in_sw_state;
	USHORT in_sw_msg;
	USHORT in_sw_min_length;
	const TEXT* in_sw_text;

};

#ifndef HAVE_WORKING_VFORK
#define vfork fork
#endif


static const TEXT FB_SHORT_MONTHS[][4] =
{
	"Jan", "Feb", "Mar",
	"Apr", "May", "Jun",
	"Jul", "Aug", "Sep",
	"Oct", "Nov", "Dec",
	"\0"
};

static const TEXT* FB_LONG_MONTHS_UPPER[] =
{
	"JANUARY",
	"FEBRUARY",
	"MARCH",
	"APRIL",
	"MAY",
	"JUNE",
	"JULY",
	"AUGUST",
	"SEPTEMBER",
	"OCTOBER",
	"NOVEMBER",
	"DECEMBER",
	0
};



// ======================================
// BEGIN AUTOCONF ABSTRACTION LAYER
// CVC: It's unacceptable that we pollute all the sources with the
// #ifdef HAVE_<something> feature. While the function prototypes are
// compatible or the extra params are ignored or we can pass default values,
// we must wrap those functions in FB names.

#include <string.h>
inline int fb_stricmp(const char* a, const char* b)
{
#if defined(HAVE_STRCASECMP)
	return strcasecmp(a, b);
#elif defined(HAVE_STRICMP)
	return stricmp(a, b);
#else
#error dont know how to compare strings case insensitive on this system
#endif
}

#endif /* JRD_COMMON_H */

