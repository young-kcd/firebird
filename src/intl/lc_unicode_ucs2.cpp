/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		lc_unicode_ucs2.c
 *	DESCRIPTION:	Language Drivers in the Unicode family.
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

#include "firebird.h"
#include "../intl/ldcommon.h"
#include "lc_ascii.h"
#include "cv_unicode_fss.h"

static SSHORT wc_mbtowc(TEXTTYPE *obj, UCS2_CHAR *wc, NCHAR *p, USHORT n);

#define FAMILY_UNICODE_WIDE_BIN(id_number, name, charset, country) \
	cache->texttype_version =		IB_LANGDRV_VERSION; \
	cache->texttype_type =			(id_number); \
	cache->texttype_character_set =		(charset); \
	cache->texttype_country =		(country); \
	cache->texttype_bytes_per_char =	2; \
	cache->texttype_fn_init =		(FPTR_SHORT) (name); \
	cache->texttype_fn_key_length =		(FPTR_SHORT) famasc_key_length; \
	cache->texttype_fn_string_to_key =	(FPTR_SHORT) famasc_string_to_key; \
	cache->texttype_fn_compare =		(FPTR_short) famasc_compare; \
	cache->texttype_fn_to_upper =		(FPTR_SHORT) famasc_to_upper; \
	cache->texttype_fn_to_lower =		(FPTR_SHORT) famasc_to_lower; \
	cache->texttype_fn_str_to_upper =	(FPTR_short) famasc_str_to_upper; \
	cache->texttype_collation_table =	(BYTE *) NULL; \
	cache->texttype_toupper_table =		(BYTE *) NULL; \
	cache->texttype_tolower_table =		(BYTE *) NULL; \
	cache->texttype_compress_table =	(BYTE *) NULL; \
	cache->texttype_expand_table =		(BYTE *) NULL; \
	cache->texttype_name =			POSIX;

#define FAMILY_UNICODE_MB_BIN(id_number, name, charset, country) \
	cache->texttype_version =		IB_LANGDRV_VERSION; \
	cache->texttype_type =			(id_number); \
	cache->texttype_character_set =		(charset); \
	cache->texttype_country =		(country); \
	cache->texttype_bytes_per_char =	3; \
	cache->texttype_fn_init =		(FPTR_SHORT) (name); \
	cache->texttype_fn_key_length =		(FPTR_SHORT) famasc_key_length; \
	cache->texttype_fn_string_to_key =	(FPTR_SHORT) famasc_string_to_key; \
	cache->texttype_fn_compare =		(FPTR_short) famasc_compare; \
	cache->texttype_fn_to_upper =		(FPTR_SHORT) famasc_to_upper; \
	cache->texttype_fn_to_lower =		(FPTR_SHORT) famasc_to_lower; \
	cache->texttype_fn_str_to_upper =	(FPTR_short) famasc_str_to_upper; \
	cache->texttype_collation_table =	(BYTE *) NULL; \
	cache->texttype_toupper_table =		(BYTE *) NULL; \
	cache->texttype_tolower_table =		(BYTE *) NULL; \
	cache->texttype_compress_table =	(BYTE *) NULL; \
	cache->texttype_expand_table =		(BYTE *) NULL; \
	cache->texttype_name =			POSIX;



TEXTTYPE_ENTRY(UNI200_init)
{
	static ASCII POSIX[] = "C.UNICODE";

	FAMILY_UNICODE_WIDE_BIN(200, UNI200_init, CS_UNICODE_UCS2, CC_C);
	cache->texttype_fn_mbtowc = (FPTR_short) wc_mbtowc;

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(UNI201_init)
{
	static ASCII POSIX[] = "C.UNICODE_FSS";

	FAMILY_UNICODE_MB_BIN(201, UNI201_init, CS_UNICODE_FSS, CC_C);
	cache->texttype_fn_to_wc = (FPTR_SHORT) CS_UTFFSS_fss_to_unicode;
	cache->texttype_fn_mbtowc = (FPTR_short) CS_UTFFSS_fss_mbtowc;

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


static SSHORT wc_mbtowc(TEXTTYPE *obj, UCS2_CHAR *wc, NCHAR *p, USHORT n)
{
	assert(obj);
	assert(wc);
	assert(p);

	if (n < sizeof(UCS2_CHAR))
		return -1;
	*wc = *(UCS2_CHAR *) p;
	return sizeof(UCS2_CHAR);
}


#undef FAMILY_UNICODE_MB_BIN
#undef FAMILY_UNICODE_WIDE_BIN
