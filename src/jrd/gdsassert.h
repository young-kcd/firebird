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
 */
#ifndef JRD_GDSASSERT_H
#define JRD_GDSASSERT_H


#ifdef DEV_BUILD

#include <stdlib.h>				// abort()

#include "../jrd/ib_stdio.h"
#include "../jrd/gds_proto.h"


/* assert() has been made into a generic version that works across
 * gds components.  Previously, the assert() defined here was only
 * usable within the engine.
 * 1996-Feb-09 David Schnepper 
 */

#define FB_GDS_ASSERT_FAILURE_STRING	"GDS Assertion failure: %s %"LINEFORMAT"\n"

#ifdef SUPERSERVER

#define fb_assert(ex)	{if (!(ex)){(void) gds__log (FB_GDS_ASSERT_FAILURE_STRING, __FILE__, __LINE__); abort();}}

#else	// !SUPERSERVER

#define fb_assert(ex)	{if (!(ex)){(void) ib_fprintf (ib_stderr, FB_GDS_ASSERT_FAILURE_STRING, __FILE__, __LINE__); abort();}}

#endif	// SUPERSERVER

#else	// DEV_BUILD

#define fb_assert(ex)				// nothing 

#endif // DEV_BUILD 

// It's a bit poor, since assert is a standard macro but this was the way it
// was done.  It is preferable to use the gds_assert(x) function, but I've left
// the following for back compatibility since I don't want to wade through that
// much code at the moment.

#ifndef assert
#define assert(ex)     fb_assert(ex)
#endif // assert

#endif // JRD_GDSASSERT_H 
