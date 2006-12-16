/*
 *	PROGRAM:	JRD International support
 *	MODULE:		unicode_util.h
 *	DESCRIPTION:	Unicode functions
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
#include "../jrd/unicode_util.h"
#include "../jrd/IntlUtil.h"
#include "../jrd/gdsassert.h"
#include "unicode/ustring.h"
#include "unicode/uchar.h"
#include "unicode/ucnv.h"
#include "unicode/ucol.h"


using Firebird::IntlUtil;


namespace Jrd {


// BOCU-1
USHORT UnicodeUtil::utf16KeyLength(USHORT len)
{
	return (len / 2) * 4;
}


// BOCU-1
USHORT UnicodeUtil::utf16ToKey(USHORT srcLen, const USHORT* src, USHORT dstLen, UCHAR* dst,
							   USHORT key_type)
{
	fb_assert(srcLen % sizeof(*src) == 0);
	fb_assert(src != NULL && dst != NULL);

	if (dstLen < srcLen / sizeof(*src) * 4)
		return INTL_BAD_KEY_LENGTH;

	UErrorCode status = U_ZERO_ERROR;
	UConverter* conv = ucnv_open("BOCU-1", &status);
	fb_assert(U_SUCCESS(status));

	int32_t len = ucnv_fromUChars(conv, reinterpret_cast<char*>(dst), dstLen, 
								  reinterpret_cast<const UChar*>(src), srcLen / sizeof(*src), &status);
	fb_assert(U_SUCCESS(status));

	ucnv_close(conv);

	return len;
}


ULONG UnicodeUtil::utf16LowerCase(ULONG srcLen, const USHORT* src, ULONG dstLen, USHORT* dst)
{
	// this is more correct but we don't support completely yet
	/***
	fb_assert(srcLen % sizeof(*src) == 0);
	fb_assert(src != NULL && dst != NULL);

	UErrorCode errorCode = U_ZERO_ERROR;

	int32_t length = u_strToLower(reinterpret_cast<UChar*>(dst), dstLen / sizeof(USHORT),
								  reinterpret_cast<const UChar*>(src), srcLen / sizeof(USHORT),
								  NULL, &errorCode);

	if (errorCode > 0 || length > dstLen)
		return INTL_BAD_STR_LENGTH;
	else
		return static_cast<ULONG>(length * sizeof(USHORT));
	***/

	fb_assert(srcLen % sizeof(*src) == 0);
	fb_assert(src != NULL && dst != NULL);

	srcLen /= sizeof(*src);
	dstLen /= sizeof(*dst);

	ULONG n = 0;

	for (ULONG i = 0; i < srcLen;)
	{
		uint32_t c;
		U16_NEXT(src, i, srcLen, c);

		c = u_tolower(c);

		bool error;
		U16_APPEND(dst, n, dstLen, c, error);
	}

	return n * sizeof(*dst);
}


ULONG UnicodeUtil::utf16UpperCase(ULONG srcLen, const USHORT* src, ULONG dstLen, USHORT* dst)
{
	// this is more correct but we don't support completely yet
	/***
	fb_assert(srcLen % sizeof(*src) == 0);
	fb_assert(src != NULL && dst != NULL);

	UErrorCode errorCode = U_ZERO_ERROR;

	int32_t length = u_strToUpper(reinterpret_cast<UChar*>(dst), dstLen / sizeof(USHORT),
								  reinterpret_cast<const UChar*>(src), srcLen / sizeof(USHORT),
								  NULL, &errorCode);

	if (errorCode > 0 || length > dstLen)
		return INTL_BAD_STR_LENGTH;
	else
		return static_cast<ULONG>(length * sizeof(USHORT));
	***/

	fb_assert(srcLen % sizeof(*src) == 0);
	fb_assert(src != NULL && dst != NULL);

	srcLen /= sizeof(*src);
	dstLen /= sizeof(*dst);

	ULONG n = 0;

	for (ULONG i = 0; i < srcLen;)
	{
		uint32_t c;
		U16_NEXT(src, i, srcLen, c);

		c = u_toupper(c);

		bool error;
		U16_APPEND(dst, n, dstLen, c, error);
	}

	return n * sizeof(*dst);
}


ULONG UnicodeUtil::utf16ToUtf8(ULONG srcLen, const USHORT* src, ULONG dstLen, UCHAR* dst,
							   USHORT* err_code, ULONG* err_position)
{
	fb_assert(srcLen % sizeof(*src) == 0);
	fb_assert(src != NULL || dst == NULL);
	fb_assert(err_code != NULL);
	fb_assert(err_position != NULL);

	*err_code = 0;

	if (dst == NULL)
		return srcLen / sizeof(*src) * 4;

	srcLen /= sizeof(*src);

	const USHORT* const srcEnd = src + srcLen;
	const UCHAR* const dstStart = dst;
	const UCHAR* const dstEnd = dst + dstLen;

	for (ULONG i = 0; i < srcLen; )
	{
		if (dstEnd - dst == 0)
		{
			*err_code = CS_TRUNCATION_ERROR;
			*err_position = i * sizeof(*src);
			break;
		}

		UChar32 c = src[i++];

		if (c <= 0x7F)
			*dst++ = c;
		else
		{
			*err_position = (i - 1) * sizeof(*src);

			if (UTF_IS_SURROGATE(c))
			{
				UChar32 c2;

				if (UTF_IS_SURROGATE_FIRST(c) && src < srcEnd && UTF_IS_TRAIL(c2 = *src))
				{
					++src;
					c = UTF16_GET_PAIR_VALUE(c, c2);
				}
				else
				{
					*err_code = CS_BAD_INPUT;
					break;
				}
			}

			if (U8_LENGTH(c) <= dstEnd - dst)
			{
				int j = 0;
				U8_APPEND_UNSAFE(dst, j, c);
				dst += j;
			}
			else
			{
				*err_code = CS_TRUNCATION_ERROR;
				break;
			}
		}
	}

	return (dst - dstStart) * sizeof(*dst);
}


ULONG UnicodeUtil::utf8ToUtf16(ULONG srcLen, const UCHAR* src, ULONG dstLen, USHORT* dst,
							   USHORT* err_code, ULONG* err_position)
{
	fb_assert(src != NULL || dst == NULL);
	fb_assert(err_code != NULL);
	fb_assert(err_position != NULL);

	*err_code = 0;

	if (dst == NULL)
		return srcLen * sizeof(*dst);

	const UCHAR* const srcEnd = src + srcLen;
	const USHORT* const dstStart = dst;
	const USHORT* const dstEnd = dst + dstLen / sizeof(*dst);

	for (ULONG i = 0; i < srcLen; )
	{
		if (dstEnd - dst == 0)
		{
			*err_code = CS_TRUNCATION_ERROR;
			*err_position = i;
			break;
		}

		UChar32 c = src[i++];

		if (c <= 0x7F)
			*dst++ = c;
		else
		{
			*err_position = i - 1;

			c = utf8_nextCharSafeBody(src, reinterpret_cast<int32_t*>(&i),
					srcLen, c, -1);

			if (c < 0)
			{
				*err_code = CS_BAD_INPUT;
				break;
			}
			else if (c <= 0xFFFF)
				*dst++ = c;
			else
			{
				if (dstEnd - dst > 1)
				{
					*dst++ = UTF16_LEAD(c);
					*dst++ = UTF16_TRAIL(c);
				}
				else
				{
					*err_code = CS_TRUNCATION_ERROR;
					break;
				}
			}
		}
	}

	return (dst - dstStart) * sizeof(*dst);
}


ULONG UnicodeUtil::utf16ToUtf32(ULONG srcLen, const USHORT* src, ULONG dstLen, ULONG* dst,
								USHORT* err_code, ULONG* err_position)
{
	fb_assert(srcLen % sizeof(*src) == 0);
	fb_assert(src != NULL || dst == NULL);
	fb_assert(err_code != NULL);
	fb_assert(err_position != NULL);

	*err_code = 0;

	if (dst == NULL)
		return srcLen / sizeof(*src) * sizeof(*dst);

	// based on u_strToUTF32 from ICU
	const USHORT* const srcStart = src;
	const ULONG* const dstStart = dst;
	const USHORT* const srcEnd = src + srcLen / sizeof(*src);
	const ULONG* const dstEnd = dst + dstLen / sizeof(*dst);

	while (src < srcEnd && dst < dstEnd)
	{
		ULONG ch = *src++;
		ULONG ch2;

		if (UTF_IS_LEAD(ch))
		{
			if (src < srcEnd && UTF_IS_TRAIL(ch2 = *src))
			{
				ch = UTF16_GET_PAIR_VALUE(ch, ch2);
				++src;
			}
			else
			{
				*err_code = CS_BAD_INPUT;
				--src;
				break;
			}
		}

		*(dst++) = ch;
	}

	*err_position = (src - srcStart) * sizeof(*src);

	if (*err_code == 0 && src < srcEnd)
		*err_code = CS_TRUNCATION_ERROR;

	return (dst - dstStart) * sizeof(*dst);
}


ULONG UnicodeUtil::utf32ToUtf16(ULONG srcLen, const ULONG* src, ULONG dstLen, USHORT* dst,
								USHORT* err_code, ULONG* err_position)
{
	fb_assert(srcLen % sizeof(*src) == 0);
	fb_assert(src != NULL || dst == NULL);
	fb_assert(err_code != NULL);
	fb_assert(err_position != NULL);

	*err_code = 0;

	if (dst == NULL)
		return srcLen;

	// based on u_strFromUTF32 from ICU
	const ULONG* const srcStart = src;
	const USHORT* const dstStart = dst;
	const ULONG* const srcEnd = src + srcLen / sizeof(*src);
	const USHORT* const dstEnd = dst + dstLen / sizeof(*dst);

	while (src < srcEnd && dst < dstEnd)
	{
		ULONG ch = *src++;

		if (ch <= 0xFFFF)
			*(dst++) = ch;
		else if (ch <= 0x10FFFF)
		{
			*(dst++) = UTF16_LEAD(ch);

			if (dst < dstEnd)
				*(dst++) = UTF16_TRAIL(ch);
			else
			{
				*err_code = CS_TRUNCATION_ERROR;
				--dst;
				break;
			}
		}
		else
		{
			*err_code = CS_BAD_INPUT;
			--src;
			break;
		}
	}

	*err_position = (src - srcStart) * sizeof(*src);

	if (*err_code == 0 && src < srcEnd)
		*err_code = CS_TRUNCATION_ERROR;

	return (dst - dstStart) * sizeof(*dst);
}


SSHORT UnicodeUtil::utf16Compare(ULONG len1, const USHORT* str1, ULONG len2, const USHORT* str2,
								 INTL_BOOL* error_flag)
{
	fb_assert(len1 % sizeof(*str1) == 0);
	fb_assert(len2 % sizeof(*str2) == 0);
	fb_assert(str1 != NULL);
	fb_assert(str2 != NULL);
	fb_assert(error_flag != NULL);

	*error_flag = false;

	int32_t cmp = u_strCompare(reinterpret_cast<const UChar*>(str1), len1 / sizeof(*str1), 
							   reinterpret_cast<const UChar*>(str2), len2 / sizeof(*str2), true);
	
	return (cmp < 0 ? -1 : (cmp > 0 ? 1 : 0));
}


ULONG UnicodeUtil::utf16Length(ULONG len, const USHORT* str)
{
	fb_assert(len % sizeof(*str) == 0);
	return u_countChar32(reinterpret_cast<const UChar*>(str), len / sizeof(*str));
}


ULONG UnicodeUtil::utf16Substring(ULONG srcLen, const USHORT* src, ULONG dstLen, USHORT* dst, 
								  ULONG startPos, ULONG length)
{
	fb_assert(srcLen % sizeof(*src) == 0);
	fb_assert(src != NULL && dst != NULL);

	if (length == 0)
		return 0;

	const USHORT* const srcStart = src;
	const USHORT* const dstStart = dst;
	const USHORT* const srcEnd = src + srcLen / sizeof(*src);
	const USHORT* const dstEnd = dst + dstLen / sizeof(*dst);
	ULONG pos = 0;

	while (src < srcEnd && dst < dstEnd && pos < startPos)
	{
		ULONG ch = *src++;

		if (UTF_IS_LEAD(ch))
		{
			if (src < srcEnd && UTF_IS_TRAIL(*src))
				++src;
		}

		++pos;
	}

	while (src < srcEnd && dst < dstEnd && pos < startPos + length)
	{
		ULONG ch = *src++;
		ULONG ch2;

		*(dst++) = ch;

		if (UTF_IS_LEAD(ch))
		{
			if (src < srcEnd && UTF_IS_TRAIL(ch2 = *src))
			{
				*(dst++) = ch2;
				++src;
			}
		}

		++pos;
	}

	return (dst - dstStart) * sizeof(*dst);
}


INTL_BOOL UnicodeUtil::utf8WellFormed(ULONG len, const UCHAR* str, ULONG* offending_position)
{
	fb_assert(str != NULL);

	for (ULONG i = 0; i < len; )
	{
		UChar32 c = str[i++];

		if (c > 0x7F)
		{
			ULONG save_i = i - 1;

			c = utf8_nextCharSafeBody(str, reinterpret_cast<int32_t*>(&i), len, c, -1);

			if (c < 0)
			{
				if (offending_position)
					*offending_position = save_i;
				return false;	// malformed
			}
		}
	}

	return true;	// well-formed
}


INTL_BOOL UnicodeUtil::utf16WellFormed(ULONG len, const USHORT* str, ULONG* offending_position)
{
	fb_assert(str != NULL);
	fb_assert(len % sizeof(*str) == 0);

	len = len / sizeof(*str);

	for (ULONG i = 0; i < len;)
	{
		ULONG save_i = i;

		uint32_t c;
		U16_NEXT(str, i, len, c);

		if (!U_IS_SUPPLEMENTARY(c) && (U16_IS_LEAD(c) || U16_IS_TRAIL(c)))
		{
			if (offending_position)
				*offending_position = save_i * sizeof(*str);
			return false;	// malformed
		}
	}

	return true;	// well-formed
}


INTL_BOOL UnicodeUtil::utf32WellFormed(ULONG len, const ULONG* str, ULONG* offending_position)
{
	fb_assert(str != NULL);
	fb_assert(len % sizeof(*str) == 0);

	const ULONG* strStart = str;

	while (len)
	{
		if (!U_IS_UNICODE_CHAR(*str))
		{
			if (offending_position)
				*offending_position = (str - strStart) * sizeof(*str);
			return false;	// malformed
		}
		else
		{
			++str;
			len -= sizeof(*str);
		}
	}

	return true;	// well-formed
}


UnicodeUtil::Utf16Collation* UnicodeUtil::Utf16Collation::create(
	texttype* tt, USHORT attributes,
	Firebird::IntlUtil::SpecificAttributesMap& specificAttributes)
{
	Firebird::string locale;
	int attributeCount = 0;

	if (specificAttributes.get(IntlUtil::convertAsciiToUtf16("LOCALE"), locale))
		++attributeCount;

	bool error;
	locale = IntlUtil::convertUtf16ToAscii(locale, &error);
	if (error)
		return false;

	if ((attributes & ~(TEXTTYPE_ATTR_PAD_SPACE | TEXTTYPE_ATTR_CASE_INSENSITIVE)) ||
		(specificAttributes.count() - attributeCount) != 0)
	{
		return false;
	}

	tt->texttype_pad_option = (attributes & TEXTTYPE_ATTR_PAD_SPACE) ? true : false;

	UErrorCode status = U_ZERO_ERROR;

	if (locale.hasData())
	{
		int i = uloc_countAvailable();

		while (--i >= 0)
		{
			if (locale == uloc_getAvailable(i))
				break;
		}

		if (i < 0)
			return false;
	}

	UCollator* compareCollator = ucol_open(locale.c_str(), &status);
	if (!compareCollator)
		return NULL;

	UCollator* partialCollator = ucol_open(locale.c_str(), &status);
	if (!partialCollator)
	{
		ucol_close(compareCollator);
		return NULL;
	}

	UCollator* sortCollator = ucol_open(locale.c_str(), &status);
	if (!sortCollator)
	{
		ucol_close(compareCollator);
		ucol_close(partialCollator);
		return NULL;
	}

	ucol_setAttribute(partialCollator, UCOL_STRENGTH, UCOL_PRIMARY, &status);

	if (attributes & TEXTTYPE_ATTR_CASE_INSENSITIVE)
	{
		ucol_setAttribute(compareCollator, UCOL_STRENGTH, UCOL_SECONDARY, &status);
		tt->texttype_flags |= TEXTTYPE_SEPARATE_UNIQUE;
		tt->texttype_canonical_width = 4;	// UTF-32
	}
	else
		tt->texttype_flags = TEXTTYPE_DIRECT_MATCH;

	USet* contractions = uset_open(0, 0);
	ucol_getContractions(partialCollator, contractions, &status);

	Utf16Collation* obj = new Utf16Collation();
	obj->tt = tt;
	obj->attributes = attributes;
	obj->compareCollator = compareCollator;
	obj->partialCollator = partialCollator;
	obj->sortCollator = sortCollator;
	obj->contractions = contractions;
	obj->contractionsCount = uset_getItemCount(contractions);

	return obj;
}


UnicodeUtil::Utf16Collation::~Utf16Collation()
{
	uset_close(static_cast<USet*>(contractions));

	ucol_close((UCollator*)compareCollator);
	ucol_close((UCollator*)partialCollator);
	ucol_close((UCollator*)sortCollator);
}


USHORT UnicodeUtil::Utf16Collation::keyLength(USHORT len) const
{
	return (len / 4) * 6;
}


USHORT UnicodeUtil::Utf16Collation::stringToKey(USHORT srcLen, const USHORT* src,
												USHORT dstLen, UCHAR* dst,
												USHORT key_type) const
{
	fb_assert(src != NULL && dst != NULL);
	fb_assert(srcLen % sizeof(*src) == 0);

	if (dstLen < keyLength(srcLen))
	{
		fb_assert(false);
		return INTL_BAD_KEY_LENGTH;
	}

	srcLen /= sizeof(*src);

	if (tt->texttype_pad_option)
	{
		const USHORT* pad;

		for (pad = src + srcLen - 1; pad >= src; --pad)
		{
			if (*pad != 32)
				break;
		}

		srcLen = pad - src + 1;
	}

	void* coll;

	switch (key_type)
	{
		case INTL_KEY_PARTIAL:
		{
			coll = partialCollator;

			// Remove last bytes of key if they are start of a contraction
			// to correctly find in the index.
			for (int i = 0; i < contractionsCount; ++i)
			{
				UChar str[10];
				UErrorCode status = U_ZERO_ERROR;
				int len = uset_getItem(static_cast<USet*>(contractions),
					i, NULL, NULL, str, sizeof(str), &status);

				if (len > srcLen)
					len = srcLen;
				else
					--len;

				if (u_strCompare(str, len, reinterpret_cast<const UChar*>(src) + srcLen - len, len, true) == 0)
				{
					srcLen -= len;
					break;
				}
			}

			break;
		}

		case INTL_KEY_UNIQUE:
			coll = compareCollator;
			break;

		case INTL_KEY_SORT:
			coll = sortCollator;
			break;

		default:
			fb_assert(false);
			return INTL_BAD_KEY_LENGTH;
	}

	if (srcLen != 0)
	{
		return ucol_getSortKey(static_cast<const UCollator*>(coll),
			reinterpret_cast<const UChar*>(src), srcLen, dst, dstLen);
	}
	else
		return 0;
}


SSHORT UnicodeUtil::Utf16Collation::compare(ULONG len1, const USHORT* str1,
											ULONG len2, const USHORT* str2,
											INTL_BOOL* error_flag) const
{
	fb_assert(len1 % sizeof(*str1) == 0 && len2 % sizeof(*str2) == 0);
	fb_assert(str1 != NULL && str2 != NULL);
	fb_assert(error_flag != NULL);

	*error_flag = false;

	len1 /= sizeof(*str1);
	len2 /= sizeof(*str2);

	if (tt->texttype_pad_option)
	{
		const USHORT* pad;

		for (pad = str1 + len1 - 1; pad >= str1; --pad)
		{
			if (*pad != 32)
				break;
		}

		len1 = pad - str1 + 1;

		for (pad = str2 + len2 - 1; pad >= str2; --pad)
		{
			if (*pad != 32)
				break;
		}

		len2 = pad - str2 + 1;
	}

	return (SSHORT)ucol_strcoll(static_cast<const UCollator*>(compareCollator),
								reinterpret_cast<const UChar*>(str1), len1, 
								reinterpret_cast<const UChar*>(str2), len2);
}


ULONG UnicodeUtil::Utf16Collation::canonical(ULONG srcLen, const USHORT* src, ULONG dstLen, ULONG* dst)
{
	USHORT errCode;
	ULONG errPosition;

	Firebird::HalfStaticArray<UCHAR, BUFFER_SMALL> upperStr;

	if (attributes & TEXTTYPE_ATTR_CASE_INSENSITIVE)
	{
		srcLen = utf16UpperCase(srcLen, src,
			dstLen, reinterpret_cast<USHORT*>(upperStr.getBuffer(dstLen)));
		src = reinterpret_cast<USHORT*>(upperStr.begin());
	}

	// convert UTF-16 to UTF-32
	return utf16ToUtf32(srcLen, src, dstLen, dst, &errCode, &errPosition) / sizeof(ULONG);
}


}	// namespace Jrd
