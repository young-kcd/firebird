#ifndef INCLUDE_Firebird
#define INCLUDE_Firebird

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
 *  $Id: firebird.h,v 1.15 2003-10-27 22:27:29 skidder Exp $
 *
 */


#include "gen/autoconfig.h"

#if defined(WIN_NT)
#define FB_DLL_EXPORT __declspec(dllexport)
#else
#define FB_DLL_EXPORT
#endif
#if defined(SOLX86)
#define __PRAGMA_REDEFINE_EXTNAME 
#define __EXTENSIONS__
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
#define SERVER_SHUTDOWN		1
#endif

// Check if we need thread synchronization
#if defined(SUPERSERVER) || defined(SUPERCLIENT) || \
	defined(WIN_NT) || defined(SOLARIS_MT) || defined (VMS)
#define MULTI_THREAD 1
#endif

#ifndef NULL
#define NULL            0L
#endif

#if defined __cplusplus && defined __GNUC__
#include "../common/classes/alloc.h"
#endif

#endif /* INCLUDE_Firebird */
