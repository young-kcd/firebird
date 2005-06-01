/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		ldcommon.h
 *	DESCRIPTION:	Common declarations for InterBase language drivers
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
 */

#ifndef INTL_LDCOMMON_H
#define INTL_LDCOMMON_H

/* #include "../jrd/gdsassert.h" */
/* Put the assert in here */

#include "../jrd/intlobj_new.h"
#include "../jrd/constants.h"
#include "../intl/charsets.h"
#include "../intl/country_codes.h"
#include "../intl/ld.h"

#undef DEBUG

typedef USHORT UNICODE;

/* Redirect the assertion code defined by gdsassert.h to a local routine */
#ifdef fb_assert
#undef fb_assert
#endif
#ifndef DEV_BUILD
#define ERR_assert				/* nothing */
#define fb_assert(ex)				/* nothing */
#else
#include <stdlib.h> /* prototype for abort() */
#define ERR_assert	LD_assert
#define fb_assert(ex)	{if (!(ex)) {LD_assert (__FILE__, __LINE__); abort();}}

#endif

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif
#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif



#define	TEXTTYPE_ENTRY(name)	INTL_BOOL name (TEXTTYPE cache, const ASCII* tt_name, const ASCII* cs_name, \
												USHORT attributes, const UCHAR* specific_attributes, \
												ULONG specific_attributes_length)



#ifdef NOT_USED_OR_REPLACED
#define CONVERT_ENTRY(cs1, cs2, name)	INTL_BOOL	name (csconvert* csptr, const ASCII* dest_cs, const ASCII* source_cs)

#define	CONVERT_RETURN	return (true)
#endif



#define CHARSET_ENTRY(name)	INTL_BOOL	name (charset* csptr, const ASCII* cs_name)

#define CHARSET_RETURN	return (true)

void CV_convert_init(csconvert*, pfn_INTL_convert, const void*, const void*);

#endif /* INTL_LDCOMMON_H */

