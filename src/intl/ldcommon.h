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

#ifndef _INTL_LDCOMMON_H_
#define _INTL_LDCOMMON_H_

#ifdef DEBUG_LD
#include <stdio.h>
#include <assert.h>
#else
#include "../jrd/gdsassert.h"
#endif

#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../jrd/intl.h"
#include "../jrd/intlobj.h"
#include "../intl/langdrv.h"
#include "../intl/charsets.h"
#include "../intl/country_codes.h"
#include "../intl/ld.h"
#include "../jrd/kanji.h"
#include "../jrd/ods.h"
#include "../jrd/btr.h"

#undef DEBUG

#define STATIC	static

typedef USHORT UNICODE;

/* Redirect the assertion code defined by gdsassert.h to a local routine */
#ifdef assert
#undef assert
#endif
#ifndef DEV_BUILD
#define ERR_assert				/* nothing */
#define assert(ex)				/* nothing */
#else
#define ERR_assert	LD_assert
#define assert(ex)	{if (!(ex)){(void) LD_assert (__FILE__, __LINE__); abort();}}
#endif




#define	TEXTTYPE_ENTRY(name)	USHORT name (cache, parm1, dummy) \
TEXTTYPE	cache; \
SSHORT		parm1; \
SSHORT		dummy;

#define	TEXTTYPE_RETURN	return (0)




#define CONVERT_ENTRY(cs1, cs2, name)	USHORT	name (csptr, dest_cs, source_cs)\
CSCONVERT	csptr; \
SSHORT		dest_cs; \
SSHORT		source_cs;

#define	CONVERT_RETURN	return (0)




#define CHARSET_ENTRY(name)	USHORT	name (csptr, cs_id, dummy) \
CHARSET	csptr;\
SSHORT	cs_id;\
SSHORT	dummy;

#define CHARSET_RETURN	return (0)

#endif /* _INTL_LDCOMMON_H_ */
