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
#ifndef _JRD_GDSASSERT_H_
#define _JRD_GDSASSERT_H_


#ifdef DEV_BUILD

#include <stdlib.h>				// abort()

#include "../jrd/ib_stdio.h"


/* assert() has been made into a generic version that works across
 * gds components.  Previously, the assert() defined here was only
 * usable within the engine.
 * 1996-Feb-09 David Schnepper 
 */
#ifdef SUPERSERVER
#define _assert(ex)	{if (!(ex)){(void) gds__log ("GDS Assertion failure: %s %ld\n", __FILE__, __LINE__); abort();}}
#else
#define _assert(ex)	{if (!(ex)){(void) ib_fprintf (ib_stderr, "GDS Assertion failure: %s %ld\n", __FILE__, __LINE__); abort();}}
#endif

#else

#define _assert(ex)				/* nothing */

#endif /* DEV_BUILD */

#define gds_assert(ex) _assert(ex)


/* It's a bit poor, since asset is a standard function but this was the way it
   was done.  It is preferable to use the gds_assert(x) function, but I've left
   the following for back compatibility since I don't want to wade through that
   much code at the moment.
*/

#ifndef assert
#define assert(ex)	_assert(ex)
#endif /* assert */

#endif /* _JRD_GDSASSERT_H_ */
