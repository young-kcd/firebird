/*
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
 * CVC: Do not override local fb_assert like the ones in gpre and dsql.
 */

#ifndef JRD_GDSASSERT_H
#define JRD_GDSASSERT_H

#include "../yvalve/gds_proto.h"

#ifdef DEV_BUILD

#include <stdlib.h>		// abort()
#include <stdio.h>

#ifdef WIN_NT
#include <io.h> 		// isatty()
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>		// isatty()
#endif

#define FB_ASSERT_FAILURE_STRING	"Assertion (%s) failure: %s %"LINEFORMAT"\n"

#if !defined(fb_assert)
#define fb_assert_continue(ex)	{if (!(ex)) {if (isatty(2)) fprintf(stderr, FB_ASSERT_FAILURE_STRING, #ex, __FILE__, __LINE__);\
											 else gds__log(FB_ASSERT_FAILURE_STRING, #ex, __FILE__, __LINE__);}}
#define fb_assert(ex)	{if (!(ex)) {if (isatty(2)) fprintf(stderr, FB_ASSERT_FAILURE_STRING, #ex, __FILE__, __LINE__);\
									 else gds__log(FB_ASSERT_FAILURE_STRING, #ex, __FILE__, __LINE__); abort();}}
#endif

#else	// DEV_BUILD

#define fb_assert(ex)				// nothing
#define fb_assert_continue(ex)		// nothing

#endif // DEV_BUILD

namespace DtorException {
	inline void devHalt()
	{
		// If any guard's dtor is executed during exception processing,
		// (remember - this guards live on the stack), exception
		// in leave() causes std::terminate() to be called, therefore
		// losing original exception information. Not good for us.
		// Therefore ignore in release and abort in debug.
#ifdef DEV_BUILD
		abort();
#endif
	}
}

#endif // JRD_GDSASSERT_H
