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

#include <stdlib.h>		// abort()

#include "../jrd/ib_stdio.h"
#include "../jrd/gds_proto.h"


/* fb_assert() has been made into a generic version that works across
 * gds components.  Previously, the fb_assert() defined here was only
 * usable within the engine.
 * 1996-Feb-09 David Schnepper 
 */

#define FB_GDS_ASSERT_FAILURE_STRING	"GDS Assertion failure: %s %"LINEFORMAT"\n"

#ifdef SUPERSERVER

#define fb_assert(ex)	{if (!(ex)){gds__log (FB_GDS_ASSERT_FAILURE_STRING, __FILE__, __LINE__); abort();}}
#define fb_assert_continue(ex)	{if (!(ex)){gds__log (FB_GDS_ASSERT_FAILURE_STRING, __FILE__, __LINE__);}}

#else	// !SUPERSERVER

#define fb_assert(ex)	{if (!(ex)){ib_fprintf (ib_stderr, FB_GDS_ASSERT_FAILURE_STRING, __FILE__, __LINE__); abort();}}
#define fb_assert_continue(ex)	{if (!(ex)){ib_fprintf (ib_stderr, FB_GDS_ASSERT_FAILURE_STRING, __FILE__, __LINE__);}}

#endif	// SUPERSERVER

#else	// DEV_BUILD

#define fb_assert(ex)				// nothing 
#define fb_assert_continue(ex)		// nothing 

#endif // DEV_BUILD 

#endif // JRD_GDSASSERT_H 
