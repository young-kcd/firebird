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

#include "../jrd/intlobj.h"
#include "../jrd/constants.h"
#include "../intl/charsets.h"
#include "../intl/country_codes.h"
#include "../intl/ld.h"

#undef DEBUG

/* texttype version */
#define IB_LANGDRV_VERSION	1

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



#define	TEXTTYPE_ENTRY(name)	USHORT name (TEXTTYPE cache, USHORT parm1, USHORT dummy)

#define	TEXTTYPE_RETURN	return (0)




#define CONVERT_ENTRY(cs1, cs2, name)	USHORT	name (csconvert* csptr, SSHORT dest_cs, SSHORT source_cs)

#define	CONVERT_RETURN	return (0)




#define CHARSET_ENTRY(name)	USHORT	name (charset* csptr, SSHORT cs_id, SSHORT dummy)

#define CHARSET_RETURN	return (0)

void CV_convert_init(csconvert*, SSHORT, SSHORT, pfn_INTL_convert, const void*, const void*);

#endif /* INTL_LDCOMMON_H */

