/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		license.h
 *	DESCRIPTION:	Internal licensing parameters
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
 * $Id: license.h,v 1.2 2001-07-12 05:46:05 bellardo Exp $
 * Revision 1.5  2000/12/08 16:18:21  fsg
 * Preliminary changes to get IB_BUILD_NO automatically
 * increased on commits.
 *
 * setup_dirs will create 'jrd/build_no.h' by a call to
 * a slightly modified 'builds_win32/original/build_no.ksh'
 * that gets IB_BUILD_NO from 'this_build', that hopefully
 * will be increased automatically in the near future :-)
 *
 * I have changed 'jrd/iblicense.h' to use IB_BUILD_TYPE
 * from 'jrd/build_no.h'.
 * So all changes to version numbers, build types etc. can
 * now be done in 'builds_win32/original/build_no.ksh'.
 *
 */

#ifndef _JRD_LICENSE_H_
#define _JRD_LICENSE_H_

#include "../jrd/build_no.h"

#ifdef hpux
#ifdef hp9000s300
#ifdef HP300
#define FB_PLATFORM	"H3"
#endif
#ifdef HM300
#define FB_PLATFORM	"HM"
#endif
#else
#ifdef HP700
#define FB_PLATFORM	"HP"
#endif
#ifdef HP800
#define FB_PLATFORM	"HO"
#endif
#ifdef HP10
#define FB_PLATFORM	"HU"
#endif /* HP10 */
#endif
#endif

#ifdef sun
#ifdef sparc
#ifdef SOLARIS
#define FB_PLATFORM	"SO"
#else
#define FB_PLATFORM	"S4"
#endif
#endif
#ifdef i386
#define FB_PLATFORM     "SI"
#endif
#ifdef SUN3_3
#define FB_PLATFORM	"SU"
#endif
#ifndef FB_PLATFORM
#define FB_PLATFORM	"S3"
#endif
#endif

#ifdef ultrix
#ifdef mips
#define FB_PLATFORM	"MU"
#else
#define FB_PLATFORM	"UL"
#endif
#endif

#ifdef VMS
#ifdef __ALPHA
#define FB_PLATFORM     "AV"
#else
#define FB_PLATFORM	"VM"
#endif
#endif

#ifdef MAC
#define FB_PLATFORM	"MA"
#endif

#ifdef PC_PLATFORM
#undef NODE_CHECK
#define NODE_CHECK(val,resp)
#endif

#ifdef NETWARE_386
#define FB_PLATFORM     "NW"
#endif

#ifdef AIX
#define FB_PLATFORM	"IA"
#endif

#ifdef AIX_PPC
#define FB_PLATFORM	"PA"
#endif

#ifdef IMP
#define FB_PLATFORM	"IM"
#endif

#ifdef DELTA
#define FB_PLATFORM	"DL"
#endif

#ifdef XENIX
#ifdef SCO_UNIX
#define FB_PLATFORM	"SI"		/* 5.5 SCO Port */
#else
#define FB_PLATFORM	"XN"
#endif
#endif

#ifdef sgi
#define FB_PLATFORM	"SG"
#endif

#ifdef DGUX
#ifdef DG_X86
#define FB_PLATFORM     "DI"	/* DG INTEL */
#else
#define FB_PLATFORM	"DA"		/* DG AViiON */
#define M88K_DEFINED
#endif /* DG_X86 */
#endif /* DGUX */

#ifdef WIN_NT
#ifdef i386
#if (defined SUPERCLIENT || defined SUPERSERVER)
#if (defined WIN95)
#define FB_PLATFORM	"WI"
#else
#define FB_PLATFORM	"NIS"
#endif /* WIN95 */
#else
#define FB_PLATFORM	"NI"
#endif
#else
#ifdef alpha
#define FB_PLATFORM	"NA"
#else
#ifdef mips
#define FB_PLATFORM	"NM"
#else /* PowerPC */
#define FB_PLATFORM	"NP"
#endif
#endif
#endif
#endif

#ifdef EPSON
#define FB_PLATFORM	"EP"		/* epson */
#endif

#ifdef ALPHA_NT
#define	FB_PLATFORM	"AN"		/* Alpha NT */
#endif

#ifdef DECOSF
#define	FB_PLATFORM	"AO"		/* Alpha OSF-1 */
#endif

#ifdef M88K
#define	FB_PLATFORM	"M8"		/* Motorola 88k */
#endif

#ifdef UNIXWARE
#define	FB_PLATFORM	"UW"		/* Unixware */
#endif

#ifdef NCR3000
#define	FB_PLATFORM	"NC"		/* NCR3000 */
#endif

#ifdef LINUX
#define FB_PLATFORM     "LI"	/* Linux on Intel */
#endif

#ifdef FREEBSD
#define FB_PLATFORM     "FB"	/* FreeBSD/i386 */
#endif

#ifdef NETBSD
#define FB_PLATFORM     "NB"	/* NetBSD */
#endif

#ifdef DARWIN
#define FB_PLATFORM	"UP"	/* Darwin/PowerPC */
#endif


#ifndef GDS_VERSION
#define GDS_VERSION	FB_PLATFORM "-" FB_BUILD_TYPE FB_MAJOR_VER "." FB_MINOR_VER "." FB_REV_NO "." FB_BUILD_NO " " FB_BUILD_SUFFIX
#endif

#endif /* _JRD_LICENSE_H_ */
