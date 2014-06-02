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
#include "../common/classes/alloc.h"
#include "../intl/ldcommon.h"
#include "../jrd/CharSet.h"
#include "../jrd/IntlUtil.h"
#include "ld_proto.h"
#include "lc_ascii.h"


static const ULONG UPPER_EXCEPTIONS_B5[] = {
	0x00B5,	// #MICRO SIGN
	0
};
static const ULONG UPPER_EXCEPTIONS_FF[] = {
	0x00FF,	// #LATIN SMALL LETTER Y WITH DIAERESIS
	0
};
static const ULONG UPPER_EXCEPTIONS_B5_FF[] = {
	0x00B5,	// #MICRO SIGN
	0x00FF,	// #LATIN SMALL LETTER Y WITH DIAERESIS
	0
};


namespace {

struct TextTypeImpl
{
	Jrd::CharSet* charSet;
	charset cs;
	const ULONG* lower_exceptions;
	const ULONG* upper_exceptions;
};

} // namespace


static void famasc_destroy(texttype* obj)
{
	TextTypeImpl* impl = obj->texttype_impl;

	if (impl)
	{
		if (impl->cs.charset_fn_destroy)
			impl->cs.charset_fn_destroy(&impl->cs);

		delete impl->charSet;
		delete impl;
	}
}


static ULONG famasc_str_to_lower(texttype* obj, ULONG iLen, const BYTE* pStr, ULONG iOutLen, BYTE *pOutStr)
{
	try
	{
		return Firebird::IntlUtil::toLower(obj->texttype_impl->charSet, iLen, pStr, iOutLen, pOutStr,
			obj->texttype_impl->lower_exceptions);
	}
	catch (const Firebird::Exception&)
	{
		return INTL_BAD_STR_LENGTH;
	}
}


static ULONG famasc_str_to_upper(texttype* obj, ULONG iLen, const BYTE* pStr, ULONG iOutLen, BYTE *pOutStr)
{
	try
	{
		return Firebird::IntlUtil::toUpper(obj->texttype_impl->charSet, iLen, pStr, iOutLen, pOutStr,
			obj->texttype_impl->upper_exceptions);
	}
	catch (const Firebird::Exception&)
	{
		return INTL_BAD_STR_LENGTH;
	}
}


static inline bool FAMILY_ASCII(texttype* cache,
								SSHORT country,
								const ASCII* POSIX,
								USHORT attributes,
								const UCHAR* specific_attributes,
								ULONG specific_attributes_length,
								const ASCII* cs_name,
								const ASCII* config_info,
								const ULONG* lower_exceptions,
								const ULONG* upper_exceptions)
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

	if (lower_exceptions || upper_exceptions)
	{
		cache->texttype_fn_destroy		= famasc_destroy;
		cache->texttype_fn_str_to_upper	= famasc_str_to_upper;
		cache->texttype_fn_str_to_lower	= famasc_str_to_lower;

		TextTypeImpl* impl = FB_NEW(*getDefaultMemoryPool()) TextTypeImpl;
		cache->texttype_impl = impl;
		
		memset(&impl->cs, 0, sizeof(impl->cs));
		LD_lookup_charset(&impl->cs, cs_name, config_info);

		impl->charSet = Jrd::CharSet::createInstance(*getDefaultMemoryPool(), 0, &impl->cs);

		impl->lower_exceptions = lower_exceptions;
		impl->upper_exceptions = upper_exceptions;
	}

	return true;
}



TEXTTYPE_ENTRY(DOS101_init)
{
	static const ASCII POSIX[] = "C.DOS437";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(DOS107_init)
{
	static const ASCII POSIX[] = "C.DOS865";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(DOS160_init)
{
	static const ASCII POSIX[] = "C.DOS850";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(ISO88591_cp_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY (ISO88592_cp_init)
{
	static const ASCII	POSIX[] = "C.ISO8859_2";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY (ISO88593_cp_init)
{
	static const ASCII	POSIX[] = "C.ISO8859_3";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY (ISO88594_cp_init)
{
	static const ASCII	POSIX[] = "C.ISO8859_4";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY (ISO88595_cp_init)
{
	static const ASCII	POSIX[] = "C.ISO8859_5";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY (ISO88596_cp_init)
{
	static const ASCII	POSIX[] = "C.ISO8859_6";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY (ISO88597_cp_init)
{
	static const ASCII	POSIX[] = "C.ISO8859_7";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY (ISO88598_cp_init)
{
	static const ASCII	POSIX[] = "C.ISO8859_8";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY (ISO88599_cp_init)
{
	static const ASCII	POSIX[] = "C.ISO8859_9";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY (ISO885913_cp_init)
{
	static const ASCII	POSIX[] = "C.ISO8859_13";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, NULL);
}


TEXTTYPE_ENTRY(DOS852_c0_init)
{
	static const ASCII POSIX[] = "C.DOS852";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(DOS857_c0_init)
{
	static const ASCII POSIX[] = "C.DOS857";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(DOS860_c0_init)
{
	static const ASCII POSIX[] = "C.DOS860";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(DOS861_c0_init)
{
	static const ASCII POSIX[] = "C.DOS861";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(DOS863_c0_init)
{
	static const ASCII POSIX[] = "C.DOS863";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(DOS737_c0_init)
{
	static const ASCII POSIX[] = "C.DOS737";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(DOS775_c0_init)
{
	static const ASCII POSIX[] = "C.DOS775";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(DOS858_c0_init)
{
	static const ASCII POSIX[] = "C.DOS858";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(DOS862_c0_init)
{
	static const ASCII POSIX[] = "C.DOS862";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(DOS864_c0_init)
{
	static const ASCII POSIX[] = "C.DOS864";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(DOS866_c0_init)
{
	static const ASCII POSIX[] = "C.DOS866";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(DOS869_c0_init)
{
	static const ASCII POSIX[] = "C.DOS869";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(CYRL_c0_init)
{
	static const ASCII POSIX[] = "C.CYRL";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(WIN1250_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(WIN1251_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(WIN1252_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5);
}


TEXTTYPE_ENTRY(WIN1253_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_FF);
}


TEXTTYPE_ENTRY(WIN1254_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5);
}


TEXTTYPE_ENTRY(WIN1255_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_5";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(WIN1256_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(WIN1257_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(NEXT_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(KOI8R_c0_init)
{
	static const ASCII POSIX[] = "C.KOI8R";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(KOI8U_c0_init)
{
	static const ASCII POSIX[] = "C.KOI8U";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5_FF);
}


TEXTTYPE_ENTRY(WIN1258_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	return FAMILY_ASCII(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length,
		cs_name, config_info, NULL, UPPER_EXCEPTIONS_B5);
}


const USHORT LANGASCII_MAX_KEY	= MAX_KEY;
const BYTE ASCII_SPACE			= 32;			// ASCII code for space

/*
 * key_length (in_len)
 *
 * For an input string of (in_len) bytes, return the maximum
 * key buffer length.
 *
 * This is used for index buffer allocation within the
 * Engine.
 */
USHORT famasc_key_length(texttype* obj, USHORT inLen)
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
USHORT famasc_string_to_key(texttype* obj, USHORT iInLen, const BYTE* pInChar, USHORT iOutLen, BYTE *pOutChar,
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


SSHORT famasc_compare(texttype* obj, ULONG l1, const BYTE* s1, ULONG l2, const BYTE* s2,
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
