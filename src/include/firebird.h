/*
 *	PROGRAM:		Firebird RDBMS definitions
 *	MODULE:			firebird.h
 *	DESCRIPTION:	Main Firebird header.
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Mark O'Donohue, Mike Nordell and John Bellardo
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2001
 *       Mark O'Donohue <mark.odonohue@ludwig.edu.au>
 *       Mike Nordell   <tamlin@algonet.se>
 *       John Bellardo  <bellardo@cs.ucsd.edu>
  *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 */

#ifndef INCLUDE_Firebird_H
#define INCLUDE_Firebird_H

#include "gen/autoconfig.h"

// Vulcan definitions
#ifdef NAMESPACE
namespace NAMESPACE{}		// declare namespace before use
using namespace NAMESPACE;
#define START_NAMESPACE		namespace NAMESPACE {
#define CLASS(cls)			namespace NAMESPACE { class cls; }
#define END_NAMESPACE		}
#else
#define START_NAMESPACE
#define CLASS(cls)			class cls;
#define END_NAMESPACE
#endif

// Using our debugging code is pointless when we may use Valgrind features
#if defined(DEV_BUILD) && !defined(USE_VALGRIND)
#define DEBUG_GDS_ALLOC
#endif

#if defined(WIN_NT)
#define FB_DLL_EXPORT __declspec(dllexport)
#else
#define FB_DLL_EXPORT
#endif
//#if defined(SOLX86)
// this pragmas is used only with gcc 2.95!
//#define __PRAGMA_REDEFINE_EXTNAME 
//#define __EXTENSIONS__
//
//#endif

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

#if defined(WIN_NT) && defined(SUPERSERVER) && !defined(EMBEDDED)
// Comment this definition to build without priority scheduler 
//	OR:
// Uncomment this definition to build with priority scheduler
#define THREAD_PSCHED
#endif

#if defined(MULTI_THREAD) && !defined(SUPERCLIENT)
#define SERVICE_THREAD
#endif

#endif /* INCLUDE_Firebird_H */
