/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		lc_ascii.cpp
 *	DESCRIPTION:	Language Drivers in the binary collation family.
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
#include "lc_ascii.h"

static inline bool FAMILY_ASCII(TEXTTYPE cache,
								SSHORT country,
								const ASCII* POSIX,
								USHORT attributes,
								const UCHAR* specific_attributes,
								ULONG specific_attributes_length)
{
	if ((attributes & ~TEXTTYPE_ATTR_PAD_SPACE) || specific_attributes_length)
		return false;

	cache->texttype_version			= TEXTTYPE_VERSION_1;
	cache->texttype_name			= POSIX;
	cache->texttype_country			= country;
	cache->texttype_pad_option		= (attributes & TEXTTYPE_ATTR_PAD_SPACE) ? true : false;
	cache->texttype_fn_key_length	= famasc_key_length;
	cache->texttype_fn_string_to_key= famasc_string_to_key;
	cache->texttype_fn_compare		= famasc_compare;
	//cache->texttype_fn_str_to_upper	= famasc_str_to_upper;
	//cache->texttype_fn_str_to_lower	= famasc_str_to_lower;

	return true;
}



TEXTTYPE_ENTRY(DOS101_init)
{
	static const ASCII POSIX[] = "C.DOS437";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(DOS107_init)
{
	static const ASCII POSIX[] = "C.DOS865";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(DOS160_init)
{
	static const ASCII POSIX[] = "C.DOS850";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(ISO88591_cp_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY (ISO88592_cp_init)
{
	static const ASCII	POSIX[] = "C.ISO8859_2";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY (ISO88593_cp_init)
{
	static const ASCII	POSIX[] = "C.ISO8859_3";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY (ISO88594_cp_init)
{
	static const ASCII	POSIX[] = "C.ISO8859_4";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY (ISO88595_cp_init)
{
	static const ASCII	POSIX[] = "C.ISO8859_5";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY (ISO88596_cp_init)
{
	static const ASCII	POSIX[] = "C.ISO8859_6";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length); 
}


TEXTTYPE_ENTRY (ISO88597_cp_init)
{
	static const ASCII	POSIX[] = "C.ISO8859_7";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY (ISO88598_cp_init)
{
	static const ASCII	POSIX[] = "C.ISO8859_8";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length); 
}


TEXTTYPE_ENTRY (ISO88599_cp_init)
{
	static const ASCII	POSIX[] = "C.ISO8859_9";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY (ISO885913_cp_init)
{
	static const ASCII	POSIX[] = "C.ISO8859_13";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length); 
}


TEXTTYPE_ENTRY(DOS852_c0_init)
{
	static const ASCII POSIX[] = "C.DOS852";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(DOS857_c0_init)
{
	static const ASCII POSIX[] = "C.DOS857";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(DOS860_c0_init)
{
	static const ASCII POSIX[] = "C.DOS860";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(DOS861_c0_init)
{
	static const ASCII POSIX[] = "C.DOS861";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(DOS863_c0_init)
{
	static const ASCII POSIX[] = "C.DOS863";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(DOS737_c0_init)
{
	static const ASCII POSIX[] = "C.DOS737";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(DOS775_c0_init)
{
	static const ASCII POSIX[] = "C.DOS775";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(DOS858_c0_init)
{
	static const ASCII POSIX[] = "C.DOS858";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(DOS862_c0_init)
{
	static const ASCII POSIX[] = "C.DOS862";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(DOS864_c0_init)
{
	static const ASCII POSIX[] = "C.DOS864";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(DOS866_c0_init)
{
	static const ASCII POSIX[] = "C.DOS866";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(DOS869_c0_init)
{
	static const ASCII POSIX[] = "C.DOS869";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(CYRL_c0_init)
{
	static const ASCII POSIX[] = "C.CYRL";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1250_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1251_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	if (FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length))
	{
		cache->texttype_fn_str_to_upper =	cp1251_str_to_upper;
		cache->texttype_fn_str_to_lower =	cp1251_str_to_lower;
		return true;
	}
	else
		return false;
}


TEXTTYPE_ENTRY(WIN1252_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1253_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1254_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1255_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_5";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1256_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(WIN1257_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


TEXTTYPE_ENTRY(NEXT_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


/*
 * Generic base for InterBase 4.0 Language Driver - ASCII family (binary
 * 8 bit sorting)
 */

const USHORT LANGASCII_MAX_KEY	= MAX_KEY;
const BYTE ASCII_SPACE			= 32;			// ASCII code for space

#define	ASCII7_UPPER(ch) \
	((((UCHAR) (ch) >= (UCHAR) ASCII_LOWER_A) && ((UCHAR) (ch) <= (UCHAR) ASCII_LOWER_Z)) \
		? (UCHAR) ((ch)-ASCII_LOWER_A+ASCII_UPPER_A) \
		: (UCHAR) (ch))
#define	ASCII7_LOWER(ch) \
	((((UCHAR) (ch) >= (UCHAR) ASCII_UPPER_A) && ((UCHAR) (ch) <= (UCHAR) ASCII_UPPER_Z)) \
		? (UCHAR) ((ch)-ASCII_UPPER_A+ASCII_LOWER_A) \
		: (UCHAR) (ch))

const UCHAR CP1251_UPPER_A		= 0xC0;
const UCHAR CP1251_LOWER_A		= 0xE0;
const UCHAR CP1251_UPPER_YA		= 0xDF;
const UCHAR CP1251_LOWER_YA		= 0xFF;

const UCHAR CP1251_UPPER_EX0	= 0xA8;	// ¨
const UCHAR CP1251_LOWER_EX0	= 0xB8;	// ¸
const UCHAR CP1251_UPPER_EX1	= 0xA5;	// ¥
const UCHAR CP1251_LOWER_EX1	= 0xB4;	// ´
const UCHAR CP1251_UPPER_EX2	= 0xAA;	// ª
const UCHAR CP1251_LOWER_EX2	= 0xBA;	// º
const UCHAR CP1251_UPPER_EX3	= 0xAF;	// ¯
const UCHAR CP1251_LOWER_EX3	= 0xBF;	// ¿
const UCHAR CP1251_UPPER_EX4	= 0xB2;	// ²
const UCHAR CP1251_LOWER_EX4	= 0xB3;	// ³

static inline UCHAR CP1251_UPPER(UCHAR ch)
{
	UCHAR res;

	if (ch >= ASCII_LOWER_A && ch <= ASCII_LOWER_Z)
	{
		res = ch - ASCII_LOWER_A + ASCII_UPPER_A;
	}
	else if (ch >= CP1251_LOWER_A && ch <= CP1251_LOWER_YA)
	{
		res = ch - CP1251_LOWER_A + CP1251_UPPER_A;
	}
	else
	{
		switch (ch)
		{
		case CP1251_LOWER_EX0:
			res = CP1251_UPPER_EX0;
			break;
		case CP1251_LOWER_EX1:
			res = CP1251_UPPER_EX1;
			break;
		case CP1251_LOWER_EX2:
			res = CP1251_UPPER_EX2;
			break;
		case CP1251_LOWER_EX3:
			res = CP1251_UPPER_EX3;
			break;
		case CP1251_LOWER_EX4:
			res = CP1251_UPPER_EX4;
			break;
		default:
			res = ch;
		}
	}

	return res;
}

static inline UCHAR CP1251_LOWER(UCHAR ch)
{
	UCHAR res;

	if (ch >= ASCII_UPPER_A && ch <= ASCII_UPPER_Z)
	{
		res = ch - ASCII_UPPER_A + ASCII_LOWER_A;
	}
	else if (ch >= CP1251_UPPER_A && ch <= CP1251_UPPER_YA)
	{
		res = ch - CP1251_UPPER_A + CP1251_LOWER_A;
	}
	else
	{
		switch (ch)
		{
		case CP1251_UPPER_EX0:
			res = CP1251_LOWER_EX0;
			break;
		case CP1251_UPPER_EX1:
			res = CP1251_LOWER_EX1;
			break;
		case CP1251_UPPER_EX2:
			res = CP1251_LOWER_EX2;
			break;
		case CP1251_UPPER_EX3:
			res = CP1251_LOWER_EX3;
			break;
		case CP1251_UPPER_EX4:
			res = CP1251_LOWER_EX4;
			break;
		default:
			res = ch;
		}
	}

	return res;
}


/*
 * key_length (in_len)
 *
 * For an input string of (in_len) bytes, return the maximum
 * key buffer length.
 *
 * This is used for index buffer allocation within the
 * Engine.
 */
USHORT famasc_key_length(TEXTTYPE obj, USHORT inLen)
{
/* fb_assert (inLen <= LANGASCII_MAX_KEY); - possible upper logic error if true */
	return (MIN(inLen, LANGASCII_MAX_KEY));
}


/*
 *
 *  Convert a user string to a sequence that will collate bytewise.
 *
 *  For ASCII type collation (codepoint collation) this mearly
 *  involves stripping the space character off the key.
 * 
 * RETURN:
 *		Length, in bytes, of returned key
 */
USHORT famasc_string_to_key(TEXTTYPE obj, USHORT iInLen, const BYTE* pInChar, USHORT iOutLen, BYTE *pOutChar,
	USHORT key_type) // unused
{
	fb_assert(pOutChar != NULL);
	fb_assert(pInChar != NULL);
	fb_assert(iInLen <= LANGASCII_MAX_KEY);
	fb_assert(iOutLen <= LANGASCII_MAX_KEY);
	fb_assert(iOutLen >= famasc_key_length(obj, iInLen));

/* point inbuff at last character */
	const BYTE* inbuff = pInChar + iInLen - 1;

	if (obj->texttype_pad_option)
	{
		/* skip backwards over all spaces & reset input length */
		while ((inbuff >= pInChar) && (*inbuff == ASCII_SPACE))
			inbuff--;
	}

	iInLen = (inbuff - pInChar + 1);

	BYTE* outbuff = pOutChar;
	while (iInLen-- && iOutLen--) {
		*outbuff++ = *pInChar++;
	}
	return (outbuff - pOutChar);
}


static bool all_spaces(const BYTE* s, SLONG len)
{
	fb_assert(s != NULL);

	while (len-- > 0) {
		if (*s++ != ASCII_SPACE)
			return false;
	}
	return true;
}


SSHORT famasc_compare(TEXTTYPE obj, ULONG l1, const BYTE* s1, ULONG l2, const BYTE* s2,
	INTL_BOOL* error_flag)
{
	fb_assert(obj != NULL);
	fb_assert(s1 != NULL);
	fb_assert(s2 != NULL);
	fb_assert(error_flag != NULL);

	*error_flag = false;

	const ULONG len = MIN(l1, l2);
	for (ULONG i = 0; i < len; i++) {
		if (s1[i] == s2[i])
			continue;
		else if (all_spaces(&s1[i], (SLONG) (l1 - i)))
			return -1;
		else if (all_spaces(&s2[i], (SLONG) (l2 - i)))
			return 1;
		else if (s1[i] < s2[i])
			return -1;
		else
			return 1;
	}

	if (l1 > len) {
		if (obj->texttype_pad_option && all_spaces(&s1[len], (SLONG) (l1 - len)))
			return 0;
		return 1;
	}
	if (l2 > len) {
		if (obj->texttype_pad_option && all_spaces(&s2[len], (SLONG) (l2 - len)))
			return 0;
		return -1;
	}
	return (0);
}


ULONG famasc_str_to_upper(TEXTTYPE obj, ULONG iLen, const BYTE* pStr, ULONG iOutLen, BYTE *pOutStr)
{
	fb_assert(pStr != NULL);
	fb_assert(pOutStr != NULL);
	fb_assert(iOutLen >= iLen);
	const BYTE* const p = pOutStr;
	while (iLen && iOutLen) {
		*pOutStr++ = ASCII7_UPPER(*pStr);
		pStr++;
		iLen--;
		iOutLen--;
	}
	if (iLen != 0)
		return INTL_BAD_STR_LENGTH;
	return (pOutStr - p);
}


ULONG famasc_str_to_lower(TEXTTYPE obj, ULONG iLen, const BYTE* pStr, ULONG iOutLen, BYTE *pOutStr)
{
	fb_assert(pStr != NULL);
	fb_assert(pOutStr != NULL);
	fb_assert(iOutLen >= iLen);
	const BYTE* const p = pOutStr;
	while (iLen && iOutLen) {
		*pOutStr++ = ASCII7_LOWER(*pStr);
		pStr++;
		iLen--;
		iOutLen--;
	}
	if (iLen != 0)
		return INTL_BAD_STR_LENGTH;
	return (pOutStr - p);
}


ULONG cp1251_str_to_upper(TEXTTYPE obj, ULONG iLen, const BYTE* pStr, ULONG iOutLen, BYTE *pOutStr)
{
	fb_assert(pStr != NULL);
	fb_assert(pOutStr != NULL);
	fb_assert(iOutLen >= iLen);
	const BYTE* const p = pOutStr;
	while (iLen && iOutLen) {
		*pOutStr++ = CP1251_UPPER(*pStr);
		pStr++;
		iLen--;
		iOutLen--;
	}
	if (iLen != 0)
		return INTL_BAD_STR_LENGTH;
	return (pOutStr - p);
}


ULONG cp1251_str_to_lower(TEXTTYPE obj, ULONG iLen, const BYTE* pStr, ULONG iOutLen, BYTE *pOutStr)
{
	fb_assert(pStr != NULL);
	fb_assert(pOutStr != NULL);
	fb_assert(iOutLen >= iLen);
	const BYTE* const p = pOutStr;
	while (iLen && iOutLen) {
		*pOutStr++ = CP1251_LOWER(*pStr);
		pStr++;
		iLen--;
		iOutLen--;
	}
	if (iLen != 0)
		return INTL_BAD_STR_LENGTH;
	return (pOutStr - p);
}
