/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		lc_iso8859_1.cpp
 *	DESCRIPTION:	Language Drivers in the is8859_1 family.
 *			(full International collation)
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
#include "ld_proto.h"

static ULONG fam2_str_to_upper(TEXTTYPE obj, ULONG iLen, const BYTE* pStr, ULONG iOutLen, BYTE *pOutStr);
static ULONG fam2_str_to_lower(TEXTTYPE obj, ULONG iLen, const BYTE* pStr, ULONG iOutLen, BYTE *pOutStr);

#include "lc_narrow.h"
#include "lc_dos.h"

static inline bool FAMILY2(TEXTTYPE cache,
							SSHORT country,
							USHORT flags,
							const SortOrderTblEntry* NoCaseOrderTbl,
							const BYTE* ToUpperConversionTbl,
							const BYTE* ToLowerConversionTbl,
							const CompressPair* CompressTbl,
							const ExpandChar* ExpansionTbl,
							const ASCII* POSIX,
							USHORT attributes,
							const UCHAR* specific_attributes,
							ULONG specific_attributes_length)
//#define FAMILY2(id_number, name, charset, country)
{
	if ((attributes & ~TEXTTYPE_ATTR_PAD_SPACE) || specific_attributes_length)
		return false;

	cache->texttype_version			= TEXTTYPE_VERSION_1;
	cache->texttype_name			= POSIX;
	cache->texttype_country			= country;
	cache->texttype_pad_option		= (attributes & TEXTTYPE_ATTR_PAD_SPACE) ? true : false;
	cache->texttype_fn_key_length	= LC_NARROW_key_length;
	cache->texttype_fn_string_to_key= LC_NARROW_string_to_key;
	cache->texttype_fn_compare		= LC_NARROW_compare;
	cache->texttype_fn_str_to_upper = fam2_str_to_upper;
	cache->texttype_fn_str_to_lower = fam2_str_to_lower;
	cache->texttype_fn_destroy		= LC_NARROW_destroy;
	cache->texttype_impl			= new TextTypeImpl;
	cache->texttype_impl->texttype_collation_table	= (const BYTE*) NoCaseOrderTbl;
	cache->texttype_impl->texttype_toupper_table	= ToUpperConversionTbl;
	cache->texttype_impl->texttype_tolower_table	= ToLowerConversionTbl;
	cache->texttype_impl->texttype_compress_table	= (const BYTE*) CompressTbl;
	cache->texttype_impl->texttype_expand_table		= (const BYTE*) ExpansionTbl;
	cache->texttype_impl->texttype_flags			= ((flags) & REVERSE) ? TEXTTYPE_reverse_secondary : 0;
	cache->texttype_impl->texttype_bytes_per_key	= 0;

	return true;
}


static inline bool FAMILY3(TEXTTYPE cache,
							SSHORT country,
							USHORT flags,
							const SortOrderTblEntry* NoCaseOrderTbl,
							const BYTE* ToUpperConversionTbl,
							const BYTE* ToLowerConversionTbl,
							const CompressPair* CompressTbl,
							const ExpandChar* ExpansionTbl,
							const ASCII* POSIX,
							USHORT attributes,
							const UCHAR* specific_attributes,
							ULONG specific_attributes_length)
{
	bool multiLevel = false;

	if (specific_attributes_length == 13)
	{
		if (memcmp(specific_attributes, "MULTI-LEVEL=0", 13) == 0)
		{
			multiLevel = false;
			specific_attributes_length = 0;
		}
		else if (memcmp(specific_attributes, "MULTI-LEVEL=1", 13) == 0)
		{
			multiLevel = true;
			specific_attributes_length = 0;
		}
	}

	if (FAMILY2(cache, country, flags, NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
				CompressTbl, ExpansionTbl, POSIX,
				attributes & ~(TEXTTYPE_ATTR_CASE_INSENSITIVE | TEXTTYPE_ATTR_ACCENT_INSENSITIVE),
				specific_attributes, specific_attributes_length))
	{
		cache->texttype_impl->texttype_flags |= TEXTTYPE_ignore_specials;

		if (!multiLevel)
			cache->texttype_impl->texttype_flags |= TEXTTYPE_non_multi_level;

		if (attributes & (TEXTTYPE_ATTR_CASE_INSENSITIVE | TEXTTYPE_ATTR_ACCENT_INSENSITIVE))
		{
			if (multiLevel)
			{
				if (attributes & (TEXTTYPE_ATTR_CASE_INSENSITIVE | TEXTTYPE_ATTR_ACCENT_INSENSITIVE))
					cache->texttype_flags |= TEXTTYPE_SEPARATE_UNIQUE;

				if ((attributes & (TEXTTYPE_ATTR_CASE_INSENSITIVE | TEXTTYPE_ATTR_ACCENT_INSENSITIVE)) ==
					TEXTTYPE_ATTR_ACCENT_INSENSITIVE)
				{
					cache->texttype_flags |= TEXTTYPE_UNSORTED_UNIQUE;
				}
			}

			if ((attributes & (TEXTTYPE_ATTR_CASE_INSENSITIVE | TEXTTYPE_ATTR_ACCENT_INSENSITIVE)) ==
				(TEXTTYPE_ATTR_CASE_INSENSITIVE | TEXTTYPE_ATTR_ACCENT_INSENSITIVE))
			{
				cache->texttype_canonical_width = 1;
			}
			else
				cache->texttype_canonical_width = 2;

			cache->texttype_fn_canonical = LC_NARROW_canonical;

			if (attributes & TEXTTYPE_ATTR_ACCENT_INSENSITIVE)
				cache->texttype_impl->texttype_flags |= TEXTTYPE_secondary_insensitive;

			if (attributes & TEXTTYPE_ATTR_CASE_INSENSITIVE)
				cache->texttype_impl->texttype_flags |= TEXTTYPE_tertiary_insensitive;
		}

		return true;
	}
	else
		return false;
}


TEXTTYPE_ENTRY(ISO88591_39_init)
{
	static const ASCII POSIX[] = "da_DA.ISO8859_1";

#include "../intl/collations/bl88591da0.h"

	return FAMILY2(cache, CC_DENMARK, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(ISO88591_40_init)
{
	static const ASCII POSIX[] = "du_NL.ISO8859_1";

#include "../intl/collations/bl88591nl0.h"

	return FAMILY2(cache, CC_NEDERLANDS, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(ISO88591_41_init)
{
	static const ASCII POSIX[] = "fi_FI.ISO8859_1";

#include "../intl/collations/bl88591fi0.h"

	return FAMILY2(cache, CC_FINLAND, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(ISO88591_42_init)
{
	static const ASCII POSIX[] = "fr_FR.ISO8859_1";

#include "../intl/collations/bl88591fr0.h"

	return FAMILY2(cache, CC_FRANCE, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(ISO88591_43_init)
{
	static const ASCII POSIX[] = "fr_CA.ISO8859_1";

#include "../intl/collations/bl88591ca0.h"

	return FAMILY2(cache, CC_FRENCHCAN, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(ISO88591_44_init)
{
	static const ASCII POSIX[] = "de_DE.ISO8859_1";

#include "../intl/collations/bl88591de0.h"

	return FAMILY2(cache, CC_GERMANY, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(ISO88591_45_init)
{
	static const ASCII POSIX[] = "is_IS.ISO8859_1";

#include "../intl/collations/bl88591is0.h"

	return FAMILY2(cache, CC_ICELAND, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(ISO88591_46_init)
{
	static const ASCII POSIX[] = "it_IT.ISO8859_1";

#include "../intl/collations/bl88591it0.h"

	return FAMILY2(cache, CC_ITALY, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}



TEXTTYPE_ENTRY(ISO88591_48_init)
{
	static const ASCII POSIX[] = "no_NO.ISO8859_1";

#include "../intl/collations/bl88591no0.h"

	return FAMILY2(cache, CC_NORWAY, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(ISO88591_49_init)
{
	static const ASCII POSIX[] = "es_ES.ISO8859_1";

#include "../intl/collations/bl88591es0.h"

	return FAMILY2(cache, CC_SPAIN, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(ISO88591_51_init)
{
	static const ASCII POSIX[] = "sv_SV.ISO8859_1";

#include "../intl/collations/bl88591sv0.h"

	return FAMILY2(cache, CC_SWEDEN, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(ISO88591_52_init)
{
	static const ASCII POSIX[] = "en_UK.ISO8859_1";

#include "../intl/collations/bl88591uk0.h"

	return FAMILY2(cache, CC_UK, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(ISO88591_53_init)
{
	static const ASCII POSIX[] = "en_US.ISO8859_1";

#include "../intl/collations/bl88591us0.h"

	return FAMILY2(cache, CC_US, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(ISO88591_54_init)
{
	static const ASCII POSIX[] = "pt_PT.ISO8859_1";

#include "../intl/collations/bl88591pt0.h"

	return FAMILY2(cache, CC_PORTUGAL, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(ISO88591_55_init)
{
	static const ASCII POSIX[] = "pt_BR.ISO8859_1";

#include "../intl/collations/bl88591ptbr0.h"

	return FAMILY3(cache, CC_BRAZIL, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1250_c1_init)
{
	static const ASCII POSIX[] = "PXW_CSY.WIN1250";

#include "../intl/collations/pw1250czech.h"

	return FAMILY2(cache, CC_CZECH, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1250_c2_init)
{
	static const ASCII POSIX[] = "PXW_HUNDC.WIN1250";

#include "../intl/collations/pw1250hundc.h"

	return FAMILY2(cache, CC_HUNGARY, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1250_c3_init)
{
	static const ASCII POSIX[] = "PXW_PLK.WIN1250";

#include "../intl/collations/pw1250polish.h"

	return FAMILY2(cache, CC_POLAND, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1250_c4_init)
{
	static const ASCII POSIX[] = "PXW_SLOV.WIN1250";

#include "../intl/collations/pw1250slov.h"

	return FAMILY2(cache, CC_YUGOSLAVIA, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY (WIN1250_c5_init)
{
	static const ASCII	  POSIX[] = "PXW_HUN.WIN1250";
	
#include "../intl/collations/pw1250hun.h"
	
	return FAMILY2(cache, CC_HUNGARY, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}						   


TEXTTYPE_ENTRY(WIN1251_c1_init)
{
	static const ASCII POSIX[] = "PXW_CYRL.WIN1251";

#include "../intl/collations/pw1251cyrr.h"

	return FAMILY2(cache, CC_RUSSIA, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1251_c2_init)
{
	static const ASCII POSIX[] = "WIN1251_UA.WIN1251";

#include "../intl/collations/xx1251_ua.h"

	return FAMILY2(cache, CC_RUSSIA, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1252_c1_init)
{
	static const ASCII POSIX[] = "PXW_INTL.WIN1252";

#include "../intl/collations/pw1252intl.h"

	return FAMILY2(cache, CC_INTL, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1252_c2_init)
{
	static const ASCII POSIX[] = "PXW_INTL850.WIN1252";

#include "../intl/collations/pw1252i850.h"

	return FAMILY2(cache, CC_INTL, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1252_c3_init)
{
	static const ASCII POSIX[] = "PXW_NORDAN4.WIN1252";

#include "../intl/collations/pw1252nor4.h"

	return FAMILY2(cache, CC_NORDAN, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1252_c4_init)
{
	static const ASCII POSIX[] = "PXW_SPAN.WIN1252";

#include "../intl/collations/pw1252span.h"

	return FAMILY2(cache, CC_SPAIN, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1252_c5_init)
{
	static const ASCII POSIX[] = "PXW_SWEDFIN.WIN1252";

#include "../intl/collations/pw1252swfn.h"

	return FAMILY2(cache, CC_SWEDFIN, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1252_c6_init)
{
	static const ASCII POSIX[] = "WIN_PTBR.WIN1252";

#include "../intl/collations/pw1252ptbr.h"

	return FAMILY3(cache, CC_BRAZIL, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1252_c7_init)
{
	static const ASCII POSIX[] = "PT_BR.WIN1252";

#include "../intl/collations/pw1252ptbr.h"

	return FAMILY3(cache, CC_BRAZIL, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1253_c1_init)
{
	static const ASCII POSIX[] = "PXW_GREEK.WIN1253";

#include "../intl/collations/pw1253greek1.h"

	return FAMILY2(cache, CC_GREECE, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1254_c1_init)
{
	static const ASCII POSIX[] = "PXW_TURK.WIN1254";

#include "../intl/collations/pw1254turk.h"

	return FAMILY2(cache, CC_TURKEY, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(NEXT_c1_init)
{
	static const ASCII POSIX[] = "NXT_US.NEXT";

#include "../intl/collations/blNEXTus0.h"

	return FAMILY2(cache, CC_US, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(NEXT_c2_init)
{
	static const ASCII POSIX[] = "NXT_DEU.NEXT";

#include "../intl/collations/blNEXTde0.h"

	return FAMILY2(cache, CC_GERMANY, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(NEXT_c3_init)
{
	static const ASCII POSIX[] = "NXT_FRA.NEXT";

#include "../intl/collations/blNEXTfr0.h"

	return FAMILY2(cache, CC_FRANCE, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(NEXT_c4_init)
{
	static const ASCII POSIX[] = "NXT_ITA.NEXT";

#include "../intl/collations/blNEXTit0.h"

	return FAMILY2(cache, CC_ITALY, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX, attributes, specific_attributes, specific_attributes_length);
}


/*
 * Generic base for InterBase 4.0 Language Driver
 */

#define	LOCALE_UPPER(ch)	(obj->texttype_impl->texttype_toupper_table[ (unsigned) (ch) ])
#define	LOCALE_LOWER(ch)	(obj->texttype_impl->texttype_tolower_table[ (unsigned) (ch) ])




/*
 *	Returns INTL_BAD_STR_LENGTH if output buffer was too small
 */
static ULONG fam2_str_to_upper(TEXTTYPE obj, ULONG iLen, const BYTE* pStr, ULONG iOutLen, BYTE *pOutStr)
{
	fb_assert(pStr != NULL);
	fb_assert(pOutStr != NULL);
	fb_assert(iOutLen >= iLen);
	const BYTE* const p = pOutStr;
	while (iLen && iOutLen) {
		*pOutStr++ = LOCALE_UPPER(*pStr);
		pStr++;
		iLen--;
		iOutLen--;
	}
	if (iLen != 0)
		return (INTL_BAD_STR_LENGTH);
	return (pOutStr - p);
}


/*
 *	Returns INTL_BAD_STR_LENGTH if output buffer was too small
 */
static ULONG fam2_str_to_lower(TEXTTYPE obj, ULONG iLen, const BYTE* pStr, ULONG iOutLen, BYTE *pOutStr)
{
	fb_assert(pStr != NULL);
	fb_assert(pOutStr != NULL);
	fb_assert(iOutLen >= iLen);
	const BYTE* const p = pOutStr;
	while (iLen && iOutLen) {
		*pOutStr++ = LOCALE_LOWER(*pStr);
		pStr++;
		iLen--;
		iOutLen--;
	}
	if (iLen != 0)
		return (INTL_BAD_STR_LENGTH);
	return (pOutStr - p);
}
