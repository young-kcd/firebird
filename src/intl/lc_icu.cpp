/*
 *	PROGRAM:	Firebird International support
 *	MODULE:		lc_icu.cpp
 *	DESCRIPTION:	Collations for ICU character sets
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Adriano dos Santos Fernandes
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "ldcommon.h"
#include "ld_proto.h"
#include "lc_icu.h"
#include "cs_icu.h"
#include "../../common/classes/array.h"
#include "../../include/fb_exception.h"
#include "unicode/ustring.h"
#include "unicode/ucol.h"


namespace
{
	struct TextTypeImpl
	{
		TextTypeImpl()
			: collator(NULL),
			  partialCollator(NULL)
		{
			memset(&cs, 0, sizeof(cs));
		}

		~TextTypeImpl()
		{
			if (cs.charset_fn_destroy)
				cs.charset_fn_destroy(&cs);

			if (collator)
				ucol_close(collator);

			if (partialCollator)
				ucol_close(partialCollator);
		}

		charset cs;
		UCollator* collator;
		UCollator* partialCollator;
	};
}


static USHORT unicode_keylength(
	texttype* tt,
	USHORT len)
{
	return len / tt->texttype_impl->cs.charset_min_bytes_per_char * 4;
}


static USHORT unicode_str2key(
	texttype* tt, 
	USHORT srcLen, 
	const UCHAR* src, 
	USHORT dstLen, 
	UCHAR* dst, 
	USHORT key_type)
{
	try
	{
		charset* cs = &tt->texttype_impl->cs;

		Firebird::HalfStaticArray<UCHAR, BUFFER_SMALL> utf16Str;
		USHORT errorCode;
		ULONG offendingPos;

		utf16Str.getBuffer(
			cs->charset_to_unicode.csconvert_fn_convert(
				&cs->charset_to_unicode,
				srcLen,
				src,
				0,
				NULL,
				&errorCode,
				&offendingPos));

		ULONG utf16Len = cs->charset_to_unicode.csconvert_fn_convert(
			&cs->charset_to_unicode,
			srcLen,
			src,
			utf16Str.getCapacity(),
			utf16Str.begin(),
			&errorCode,
			&offendingPos);

		if (dstLen < unicode_keylength(tt, srcLen))
		{
			fb_assert(false);
			return INTL_BAD_KEY_LENGTH;
		}

		return ucol_getSortKey(
			(key_type == INTL_KEY_PARTIAL ? tt->texttype_impl->partialCollator : tt->texttype_impl->collator),
			reinterpret_cast<const UChar*>(utf16Str.begin()), utf16Len / sizeof(UChar), dst, dstLen);
	}
	catch (Firebird::BadAlloc)
	{
		fb_assert(false);
		return INTL_BAD_KEY_LENGTH;
	}
}


static SSHORT unicode_compare(
	texttype* tt, 
	ULONG len1, 
	const UCHAR* str1, 
	ULONG len2, 
	const UCHAR* str2,
	INTL_BOOL* error_flag)
{
	try
	{
		*error_flag = false;

		charset* cs = &tt->texttype_impl->cs;

		Firebird::HalfStaticArray<UCHAR, BUFFER_SMALL> utf16Str1;
		Firebird::HalfStaticArray<UCHAR, BUFFER_SMALL> utf16Str2;
		USHORT errorCode;
		ULONG offendingPos;

		utf16Str1.getBuffer(
			cs->charset_to_unicode.csconvert_fn_convert(
				&cs->charset_to_unicode,
				len1,
				str1,
				0,
				NULL,
				&errorCode,
				&offendingPos));

		ULONG utf16Len1 = cs->charset_to_unicode.csconvert_fn_convert(
			&cs->charset_to_unicode,
			len1,
			str1,
			utf16Str1.getCapacity(),
			utf16Str1.begin(),
			&errorCode,
			&offendingPos);

		utf16Str2.getBuffer(
			cs->charset_to_unicode.csconvert_fn_convert(
				&cs->charset_to_unicode,
				len2,
				str2,
				0,
				NULL,
				&errorCode,
				&offendingPos));

		ULONG utf16Len2 = cs->charset_to_unicode.csconvert_fn_convert(
			&cs->charset_to_unicode,
			len2,
			str2,
			utf16Str2.getCapacity(),
			utf16Str2.begin(),
			&errorCode,
			&offendingPos);

		if (tt->texttype_pad_option)
		{
			const UCHAR* pad;

			for (pad = utf16Str1.begin() + utf16Len1 - sizeof(USHORT); pad >= utf16Str1.begin(); pad -= sizeof(USHORT))
			{
				if (*reinterpret_cast<const USHORT*>(pad) != 32)
					break;
			}

			utf16Len1 = pad - utf16Str1.begin() + sizeof(USHORT);

			for (pad = utf16Str2.begin() + utf16Len2 - sizeof(USHORT); pad >= utf16Str2.begin(); pad -= sizeof(USHORT))
			{
				if (*reinterpret_cast<const USHORT*>(pad) != 32)
					break;
			}

			utf16Len2 = pad - utf16Str2.begin() + sizeof(USHORT);
		}

		int32_t cmp = u_strCompare(
			reinterpret_cast<const UChar*>(utf16Str1.begin()), utf16Len1 / sizeof(UChar),
            reinterpret_cast<const UChar*>(utf16Str2.begin()), utf16Len2 / sizeof(UChar), true);

		return (cmp < 0 ? -1 : (cmp > 0 ? 1 : 0));
	}
	catch (Firebird::BadAlloc)
	{
		fb_assert(false);
		return 0;
	}
}


static void texttype_destroy(texttype* tt)
{
	delete [] const_cast<ASCII*>(tt->texttype_name);
	delete tt->texttype_impl;
}


static bool texttype_default_init(texttype* tt,
								  const ASCII* name,
								  const ASCII* charSetName,
								  USHORT attributes,
								  const UCHAR* specificAttributes,
								  ULONG specificAttributesLength)
{
	charset cs;
	memset(&cs, 0, sizeof(cs));

	// test if that ICU charset exist
	if (CSICU_charset_init(&cs, charSetName))
	{
		if (cs.charset_fn_destroy)
			cs.charset_fn_destroy(&cs);
	}
	else
		return false;

	if ((attributes & ~TEXTTYPE_ATTR_PAD_SPACE) ||
		// disabled TEXTTYPE_ATTR_PAD_SPACE isn't allowed
		// for our ICU collations yet
		!(attributes & TEXTTYPE_ATTR_PAD_SPACE) ||
		specificAttributesLength)
	{
		return false;
	}

	// name comes from stack. Copy it.
	tt->texttype_name = new ASCII[strlen(name) + 1];
	strcpy(const_cast<ASCII*>(tt->texttype_name), name);

	tt->texttype_version = TEXTTYPE_VERSION_1;
	tt->texttype_country = CC_INTL;
	tt->texttype_pad_option = (attributes & TEXTTYPE_ATTR_PAD_SPACE) ? true : false;
	tt->texttype_fn_destroy = texttype_destroy;

	return true;
}


static bool texttype_unicode_init(texttype* tt,
								  const ASCII* name,
								  const ASCII* charSetName,
								  USHORT attributes,
								  const UCHAR* specificAttributes,
								  ULONG specificAttributesLength)
{
	tt->texttype_impl = new TextTypeImpl();

	// test if that charset exist
	if (!LD_lookup_charset(&tt->texttype_impl->cs, charSetName))
	{
		delete tt->texttype_impl;
		return false;
	}

	if ((attributes & ~TEXTTYPE_ATTR_PAD_SPACE) ||
		// disabled TEXTTYPE_ATTR_PAD_SPACE isn't allowed
		// for our ICU collations yet
		!(attributes & TEXTTYPE_ATTR_PAD_SPACE) ||
		specificAttributesLength)
	{
		delete tt->texttype_impl;
		return false;
	}

	const char* locale = "";
	UErrorCode status = U_ZERO_ERROR;

	tt->texttype_impl->collator = ucol_open(locale, &status);
	tt->texttype_impl->partialCollator = ucol_open(locale, &status);

	if (!tt->texttype_impl->collator || !tt->texttype_impl->partialCollator)
	{
		delete tt->texttype_impl;
		return false;
	}

	ucol_setAttribute(tt->texttype_impl->partialCollator, UCOL_STRENGTH, UCOL_PRIMARY, &status);

	// name comes from stack. Copy it.
	tt->texttype_name = new ASCII[strlen(name) + 1];
	strcpy(const_cast<ASCII*>(tt->texttype_name), name);

	tt->texttype_version = TEXTTYPE_VERSION_1;
	tt->texttype_country = CC_INTL;
	tt->texttype_pad_option = (attributes & TEXTTYPE_ATTR_PAD_SPACE) ? true : false;
	tt->texttype_fn_destroy = texttype_destroy;
	tt->texttype_fn_compare = unicode_compare;
	tt->texttype_fn_key_length = unicode_keylength;
	tt->texttype_fn_string_to_key = unicode_str2key;

	return true;
}


bool LCICU_texttype_init(texttype* tt,
						 const ASCII* name,
						 const ASCII* charSetName,
						 USHORT attributes,
						 const UCHAR* specificAttributes,
						 ULONG specificAttributesLength)
{
	int len = strlen(name);

	if (strcmp(name, charSetName) == 0)
	{
		return texttype_default_init(
			tt, name, charSetName, attributes,
			specificAttributes, specificAttributesLength);
	}
	else if (len > 8 && strcmp(name + len - 8, "_UNICODE") == 0)
	{
		return texttype_unicode_init(
			tt, name, charSetName, attributes,
			specificAttributes, specificAttributesLength);
	}
	else
		return false;
}
