#ifndef INCLUDE_Firebird_H
#define INCLUDE_Firebird_H

/*
 *  The contents of this file are subject to the Mozilla Public
 *  License Version 1.1 (the "License"); you may not use this file
 *  except in compliance with the License. You may obtain a copy of
 *  the License at http://www.mozilla.org/MPL/
 *  Alternatively, the contents of this file may be used under the
 *  terms of the GNU General Public License Version 2 or later (the
 *  "GPL"), in which case the provisions of the GPL are applicable
 *  instead of those above. You may obtain a copy of the Licence at
 *  http://www.gnu.org/copyleft/gpl.html
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Relevant for more details.
 *
 *  This file was created by members of the firebird development team.
 *  All individual contributions remain the Copyright (C) of those
 *  individuals.  Contributors to this file are either listed here or
 *  can be obtained from a CVS history command.
 *
 *  All rights reserved.
 *
 *  Contributor(s):
 *       Mark O'Donohue <mark.odonohue@ludwig.edu.au>
 *       Mike Nordell   <tamlin@algonet.se>
 *       John Bellardo  <bellardo@cs.ucsd.edu>
 *
 *
 *  $Id: firebird.h,v 1.25 2004-05-26 16:13:22 alexpeshkoff Exp $
 *
 */


#include "gen/autoconfig.h"

#if defined(WIN_NT)
#define FB_DLL_EXPORT __declspec(dllexport)
#else
#define FB_DLL_EXPORT
#endif
#if defined(SOLX86)
// this pragmas is used only with gcc 2.95!
//#define __PRAGMA_REDEFINE_EXTNAME 
//#define __EXTENSIONS__
//
#endif

//
// Macro for function attribute definition
//
#if defined(__GNUC__)
#define ATTRIBUTE_FORMAT(a,b) __attribute__ ((format(printf,a,b)))
#else
#define ATTRIBUTE_FORMAT(a,b)
#endif

#ifdef __cplusplus
#include "fb_exception.h"
#endif

// 
#if defined(SUPERSERVER) || defined(WIN_NT)
#define SERVER_SHUTDOWN
#endif

// from thd.h
#ifdef HAVE_POSIX_THREADS
#ifdef SUPERSERVER
#define USE_POSIX_THREADS
#endif
#ifdef SUPERCLIENT
#if defined(LINUX) || defined(FREEBSD)
/* The following ifdef was added to build thread safe gds shared
   library on linux platform. It seems the gdslib works now (20020220)
   with thread enabled applications. Anyway, more tests should be 
   done as I don't have deep knowledge of the interbase/firebird 
   engine and this change may imply side effect I haven't known 
   about yet. Tomas Nejedlik (tomas@nejedlik.cz) */
#define USE_POSIX_THREADS
#endif
#endif
#endif

// Check if we need thread synchronization
#if defined(HAVE_MULTI_THREAD)
# if defined(SUPERSERVER) || defined(SUPERCLIENT) || \
     defined(WIN_NT) || defined(SOLARIS_MT) || defined (VMS)
# define MULTI_THREAD
# endif
#endif

// This is needed to build client library on threaded platforms for classic server
#if defined(HAVE_POSIX_THREADS) && defined(SUPERCLIENT)
# define MULTI_THREAD
#endif

#ifdef MULTI_THREAD
#define ANY_THREADING
#endif
#ifdef V4_THREADING
#define ANY_THREADING
#endif

#ifndef NULL
#define NULL            0L
#endif

#endif /* INCLUDE_Firebird_H */

