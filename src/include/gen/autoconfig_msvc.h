/*
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "MAC" and "MAC_CP" defines
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "EPSON" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "DELTA" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "IMP" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "NCR3000" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "NeXT" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "M88K" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "Cray" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "OS/2" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "Apollo" port
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

/* Headers */
#define HAVE_ASSERT_H
#undef HAVE_UNISTD_H
#define HAVE_FCNTL_H
#undef HAVE_SYS_TYPES_H
#define HAVE_TIME_H
#undef HAVE_SYS_TIME_H
#define HAVE_SYS_STAT_H
#define HAVE_CTYPE_H
#undef HAVE_SYS_UIO_H
#undef HAVE_LIB_IO_H
#undef HAVE_WAIT_H
#undef HAVE_SYS_WAIT_H
#undef HAVE_SYS_PARAM_H
#undef HAVE_VARARGS_H
#define HAVE_STDARG_H
#define HAVE_STDLIB_H
#define HAVE_STRING_H
#define HAVE_SIGNAL_H
#undef HAVE_READLINE_READLINE_H

/* types */
#undef socklen_t


/* Is the platform big endian? */
#define WORDS_BIGENDIAN 0

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
#undef ultrix
#undef PC_PLATFORM
#undef NETWARE_386
#undef WINDOWS_ONLY
#undef VMS
#undef AIX
#define WIN_NT
#undef SCO_EV
#undef DGUX
#undef DECOSF
#undef sgi
#undef mpexl
#undef UNIXWARE

#endif