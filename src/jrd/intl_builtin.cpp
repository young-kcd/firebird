#include "../jrd/common.h"
#include "intl_classes.h"
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include "../common/classes/alloc.h"
#include "../jrd/intl.h"
#include "../intl/country_codes.h"
#include "../jrd/gdsassert.h"
#include "../jrd/jrd.h"
#include "../jrd/err_proto.h"
#include "../intl/charsets.h"

using Jrd::UnicodeUtil;
 

static USHORT internal_keylength(TEXTTYPE, USHORT);
static USHORT internal_string_to_key(TEXTTYPE, USHORT, const UCHAR*, USHORT, UCHAR*, USHORT);
static SSHORT internal_compare(TEXTTYPE, ULONG, const UCHAR*, ULONG, const UCHAR*, INTL_BOOL*);
static ULONG internal_str_to_upper(TEXTTYPE, ULONG, const UCHAR*, ULONG, UCHAR*);
static ULONG internal_str_to_lower(TEXTTYPE, ULONG, const UCHAR*, ULONG, UCHAR*);
static void internal_destroy(TEXTTYPE);
static void unicode_destroy(TEXTTYPE obj);


struct TextTypeImpl
{
	BYTE texttype_pad_char;
};


static inline bool FAMILY_INTERNAL(TEXTTYPE cache,
								   SSHORT country,
								   const ASCII* POSIX,
								   USHORT attributes,
								   const UCHAR* specific_attributes,
								   ULONG specific_attributes_length)
//#define FAMILY_INTERNAL(name, country)
{
	if ((attributes & ~TEXTTYPE_ATTR_PAD_SPACE) || specific_attributes_length)
		return false;

	cache->texttype_version			= TEXTTYPE_VERSION_1;
	cache->texttype_name			= POSIX;
	cache->texttype_country			= (country);
	cache->texttype_pad_option		= (attributes & TEXTTYPE_ATTR_PAD_SPACE) ? true : false;
	cache->texttype_fn_key_length	= internal_keylength;
	cache->texttype_fn_string_to_key= internal_string_to_key;
	cache->texttype_fn_compare		= internal_compare;
	cache->texttype_fn_str_to_upper	= internal_str_to_upper;
	cache->texttype_fn_str_to_lower	= internal_str_to_lower;
	cache->texttype_fn_destroy		= internal_destroy;
	cache->texttype_impl			= new TextTypeImpl;
	cache->texttype_impl->texttype_pad_char = ' ';

	return true;
}


static inline bool FAMILY_INTERNAL_UTF(TEXTTYPE cache,
									   const ASCII* POSIX,
									   USHORT attributes,
									   const UCHAR* specific_attributes,
									   ULONG specific_attributes_length)
{
	if ((attributes & ~TEXTTYPE_ATTR_PAD_SPACE) || specific_attributes_length)
		return false;

	cache->texttype_version			= TEXTTYPE_VERSION_1;
	cache->texttype_name			= POSIX;
	cache->texttype_country			= CC_INTL;
	cache->texttype_flags			= TEXTTYPE_DIRECT_MATCH;
	cache->texttype_pad_option		= (attributes & TEXTTYPE_ATTR_PAD_SPACE) ? true : false;

	return true;
}


static inline bool FAMILY_INTERNAL_UNICODE(TEXTTYPE cache,
										   const ASCII* POSIX,
										   USHORT attributes,
										   const UCHAR* specific_attributes,
										   ULONG specific_attributes_length)
{
	if (FAMILY_INTERNAL_UTF(cache, POSIX, attributes, specific_attributes, specific_attributes_length))
	{
		//// TODO: locale
		cache->reserved_for_driver[0] = UnicodeUtil::Utf16Collation::create("");
		cache->texttype_fn_destroy = unicode_destroy;

		return cache->reserved_for_driver[0] != NULL;
	}

	return true;
}


typedef unsigned char FILECHAR;
typedef USHORT UNICODE;

typedef USHORT fss_wchar_t;
typedef int fss_size_t;

struct Byte_Mask_Table
{
	int cmask;
	int cval;
	int shift;
	SLONG lmask;
	SLONG lval;
};

static const Byte_Mask_Table tab[] = {
	{ 0x80, 0x00, 0 * 6, 0x7F, 0 },	/* 1 byte sequence */
	{ 0xE0, 0xC0, 1 * 6, 0x7FF, 0x80 },	/* 2 byte sequence */
	{ 0xF0, 0xE0, 2 * 6, 0xFFFF, 0x800 },	/* 3 byte sequence */
	{ 0xF8, 0xF0, 3 * 6, 0x1FFFFF, 0x10000 },	/* 4 byte sequence */
	{ 0xFC, 0xF8, 4 * 6, 0x3FFFFFF, 0x200000 },	/* 5 byte sequence */
	{ 0xFE, 0xFC, 5 * 6, 0x7FFFFFFF, 0x4000000 },	/* 6 byte sequence */
	{ 0, 0, 0, 0, 0 } 				/* end of table    */
};

static fss_size_t fss_mbtowc(fss_wchar_t* p, const UCHAR* s, fss_size_t n)
{
	if (s == 0)
		return 0;

	int nc = 0;
	if (n <= nc)
		return -1;
	const int c0 = *s & 0xff;
	SLONG l = c0;
	for (const Byte_Mask_Table* t = tab; t->cmask; t++) {
		nc++;
		if ((c0 & t->cmask) == t->cval) {
			l &= t->lmask;
			if (l < t->lval)
				return -1;
			*p = l;
			return nc;
		}
		if (n <= nc)
			return -1;
		s++;
		const int c = (*s ^ 0x80) & 0xFF;
		if (c & 0xC0)
			return -1;
		l = (l << 6) | c;
	}
	return -1;
}

static fss_size_t fss_wctomb(UCHAR * s, fss_wchar_t wc)
{
	if (s == 0)
		return 0;

	SLONG l = wc;
	int nc = 0;
	for (const Byte_Mask_Table* t = tab; t->cmask; t++) {
		nc++;
		if (l <= t->lmask) {
			int c = t->shift;
			*s = t->cval | (l >> c);
			while (c > 0) {
				c -= 6;
				s++;
				*s = 0x80 | ((l >> c) & 0x3F);
			}
			return nc;
		}
	}
	return -1;
}

static SSHORT internal_fss_mbtowc(TEXTTYPE obj,
						   USHORT* wc, const UCHAR* p, USHORT n)
{
/**************************************
 *
 *      I N T L _ f s s _ m b t o w c
 *
 **************************************
 *
 * Functional description
 *      InterBase interface to mbtowc function for Unicode
 *      text in FSS bytestream format.
 *
 * Return:      (common to all mbtowc routines)
 *      -1      Error in parsing next character
 *      <n>     Count of characters consumed.
 *      *wc     Next character from byte stream (if wc <> NULL)
 *
 * Note: This routine has a cousin in intl/cv_utffss.c
 *
 **************************************/
	fb_assert(obj);
	fb_assert(wc);
	fb_assert(p);

	return fss_mbtowc(wc, p, n);
}

static ULONG internal_fss_to_unicode(TEXTTYPE obj,
									 ULONG src_len,
									 const UCHAR* src_ptr,
									 ULONG dest_len,
									 UNICODE* dest_ptr,
									 USHORT* err_code,
									 ULONG* err_position)
{
	fb_assert(src_ptr != NULL || dest_ptr == NULL);
	fb_assert(err_code != NULL);
	fb_assert(err_position != NULL);
	fb_assert(obj != NULL);

	*err_code = 0;

/* See if we're only after a length estimate */
	if (dest_ptr == NULL)
		return (src_len * 2);	/* All single byte narrow characters */

	const UNICODE* const start = dest_ptr;
	const ULONG src_start = src_len;
	while ((src_len) && (dest_len >= sizeof(*dest_ptr))) {
		const fss_size_t res = fss_mbtowc(dest_ptr, src_ptr, src_len);
		if (res == -1) {
			*err_code = CS_BAD_INPUT;
			break;
		}
		fb_assert(res <= src_len);
		dest_ptr++;
		dest_len -= sizeof(*dest_ptr);
		src_ptr += res;
		src_len -= res;
	}
	if (src_len && !*err_code) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = src_start - src_len;
	return ((dest_ptr - start) * sizeof(*dest_ptr));
}

ULONG internal_unicode_to_fss(csconvert* obj,
							  ULONG unicode_len,	/* BYTE count */
							  const UNICODE* unicode_str,
							  ULONG fss_len,
							  UCHAR* fss_str,
							  USHORT* err_code,
							  ULONG* err_position)
{
	const ULONG src_start = unicode_len;
	UCHAR tmp_buffer[6];

	fb_assert(unicode_str != NULL || fss_str == NULL);
	fb_assert(err_code != NULL);
	fb_assert(err_position != NULL);
	fb_assert(obj != NULL);
	fb_assert(obj->csconvert_fn_convert == (pfn_INTL_convert) internal_unicode_to_fss);

	*err_code = 0;

/* See if we're only after a length estimate */
	if (fss_str == NULL)
		return ((unicode_len + 1) / 2 * 3);	/* worst case - all han character input */

	const UCHAR* const start = fss_str;
	while ((fss_len) && (unicode_len >= sizeof(*unicode_str))) {
		/* Convert the wide character into temp buffer */
		fss_size_t res = fss_wctomb(tmp_buffer, *unicode_str);
		if (res == -1) {
			*err_code = CS_BAD_INPUT;
			break;
		}
		/* will the mb sequence fit into space left? */
		if (res > fss_len) {
			*err_code = CS_TRUNCATION_ERROR;
			break;
		}
		/* copy the converted bytes into the destination */
		const UCHAR* p = tmp_buffer;
		for (; res; res--, fss_len--)
			*fss_str++ = *p++;
		unicode_len -= sizeof(*unicode_str);
		unicode_str++;
	}
	if (unicode_len && !*err_code) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = src_start - unicode_len;
	return ((fss_str - start) * sizeof(*fss_str));
}

static ULONG internal_fss_length(charset* obj, ULONG srcLen, const UCHAR* src)
{
/**************************************
 *
 *      i n t e r n a l _ f s s _ l e n g t h
 *
 **************************************
 *
 * Functional description
 *  Return character length of a string.
 *  If the string is malformed, count number
 *  of bytes after the offending character.
 *
 **************************************/
	ULONG charLength = 0;

	while (srcLen)
	{
		USHORT c;
		const fss_size_t res = fss_mbtowc(&c, src, srcLen);

		if (res == -1)
			break;

		fb_assert(res <= srcLen);

		src += res;
		srcLen -= res;
		++charLength;
	}

	return charLength + srcLen;
}

static ULONG internal_fss_substring(charset* obj, ULONG srcLen, const UCHAR* src,
									ULONG dstLen, UCHAR* dst, ULONG startPos, ULONG length)
{
/**************************************
 *
 *      i n t e r n a l _ f s s _ s u b s t r i n g
 *
 **************************************
 *
 * Functional description
 *  Return substring of a string.
 *  If the string is malformed, consider
 *  only bytes after the offending character.
 *
 **************************************/
	fb_assert(src != NULL && dst != NULL);

	if (length == 0)
		return 0;

	const UCHAR* srcStart = src;
	const UCHAR* dstStart = dst;
	const UCHAR* const srcEnd = src + srcLen;
	const UCHAR* const dstEnd = dst + dstLen;
	ULONG pos = 0;
	bool wellFormed = true;

	while (src < srcEnd && dst < dstEnd && pos < startPos)
	{
		USHORT c;
		fss_size_t res;

		if (wellFormed)
		{
			res = fss_mbtowc(&c, src, srcLen);

			if (res == -1)
			{
				wellFormed = false;
				continue;
			}
		}
		else
		{
			c = *src;
			res = 1;
		}

		fb_assert(res <= srcLen);

		src += res;
		srcLen -= res;
		++pos;
	}

	while (src < srcEnd && dst < dstEnd && pos < startPos + length)
	{
		USHORT c;
		fss_size_t res;

		if (wellFormed)
		{
			res = fss_mbtowc(&c, src, srcLen);

			if (res == -1)
			{
				wellFormed = false;
				continue;
			}
		}
		else
		{
			c = *src;
			res = 1;
		}

		fb_assert(res <= srcLen);

		src += res;
		srcLen -= res;
		++pos;

		if (wellFormed)
			res = fss_wctomb(dst, c);
		else
			*dst = c;

		dst += res;
	}

	return dst - dstStart;
}

static ULONG internal_str_copy(
								TEXTTYPE obj,
								ULONG inLen,
								const UCHAR* src, ULONG outLen, UCHAR* dest)
{
/**************************************
 *
 *      i n t e r n a l _ s t r _ c o p y
 *
 **************************************
 *
 * Functional description
 *      Note: dest may equal src.
 *
 **************************************/
	const UCHAR* const pStart = dest;
	while (inLen-- && outLen--) {
		*dest++ = *src++;
	}

	return (dest - pStart);
}

static USHORT internal_keylength(TEXTTYPE obj, USHORT iLength)
{
/**************************************
 *
 *      i n t e r n a l _ k e y l e n g t h
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return (iLength);
}

static USHORT internal_string_to_key(
									 TEXTTYPE obj,
									 USHORT inLen,
									 const UCHAR* src,
									 USHORT outLen,
									UCHAR* dest,
									USHORT key_type) // unused
{
/**************************************
 *
 *      i n t e r n a l _ s t r i n g _ t o _ k e y
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	const UCHAR* const pStart = dest;
	const UCHAR pad_char = obj->texttype_impl->texttype_pad_char;
	while (inLen-- && outLen--)
		*dest++ = *src++;

	if (obj->texttype_pad_option)
	{
		/* strip off ending pad characters */
		while (dest > pStart)
			if (*(dest - 1) == pad_char)
				dest--;
			else
				break;
	}

	return (dest - pStart);
}

static SSHORT internal_compare(
							   TEXTTYPE obj,
							   ULONG length1,
							   const UCHAR* p1, ULONG length2, const UCHAR* p2, INTL_BOOL* error_flag)
{
/**************************************
 *
 *      i n t e r n a l _ c o m p a r e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	const UCHAR pad = obj->texttype_impl->texttype_pad_char;
	SLONG fill = length1 - length2;
	if (length1 >= length2) {
		if (length2)
			do {
				if (*p1++ != *p2++)
					if (p1[-1] > p2[-1])
						return 1;
					else
						return -1;
			} while (--length2);
		if (fill > 0)
			do {
				if (!obj->texttype_pad_option || *p1++ != pad)
					if (p1[-1] > pad)
						return 1;
					else
						return -1;
			} while (--fill);
		return 0;
	}

	if (length1)
		do {
			if (*p1++ != *p2++)
				if (p1[-1] > p2[-1])
					return 1;
				else
					return -1;
		} while (--length1);

	do {
		if (!obj->texttype_pad_option || *p2++ != pad)
			if (pad > p2[-1])
				return 1;
			else
				return -1;
	} while (++fill);

	return 0;
}


static ULONG internal_str_to_upper(
									TEXTTYPE obj,
									ULONG inLen,
									const UCHAR* src, ULONG outLen, UCHAR* dest)
{
/**************************************
 *
 *      i n t e r n a l _ s t r _ t o _ u p p e r
 *
 **************************************
 *
 * Functional description
 *      Note: dest may equal src.
 *
 **************************************/
	const UCHAR* const pStart = dest;
	while (inLen-- && outLen--) {
		*dest++ = UPPER7(*src);
		src++;
	}

	return (dest - pStart);
}


static ULONG internal_str_to_lower(
									TEXTTYPE obj,
									ULONG inLen,
									const UCHAR* src, ULONG outLen, UCHAR* dest)
{
/**************************************
 *
 *      i n t e r n a l _ s t r _ t o _ l o w e r
 *
 **************************************
 *
 * Functional description
 *      Note: dest may equal src.
 *
 **************************************/
	const UCHAR* const pStart = dest;
	while (inLen-- && outLen--) {
		*dest++ = LOWWER7(*src);
		src++;
	}

	return (dest - pStart);
}


static void internal_destroy(TEXTTYPE obj)
{
/**************************************
 *
 *      i n t e r n a l _ d e s t r o y
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	delete obj->texttype_impl;
}


static void unicode_destroy(TEXTTYPE obj)
{
/**************************************
 *
 *      u n i c o d e _ d e s t r o y
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	fb_assert(obj != NULL);
	delete (UnicodeUtil::Utf16Collation*)obj->reserved_for_driver[0];
}


static USHORT unicode8_keylength(TEXTTYPE obj, USHORT len)
{
/**************************************
 *
 *      u n i c o d e 8 _ k e y l e n g t h
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	fb_assert(obj != NULL);
	return ((UnicodeUtil::Utf16Collation*)obj->reserved_for_driver[0])->keyLength(len);
}


static USHORT unicode8_string_to_key(
									 TEXTTYPE obj,
									 USHORT srcLen,
									 const UCHAR* src,
									 USHORT dstLen,
									 UCHAR* dst,
									 USHORT key_type)
{
/**************************************
 *
 *      u n i c o d e 8 _ s t r i n g _ t o _ k e y
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	fb_assert(obj != NULL);

	USHORT err_code;
	ULONG err_position;

	Firebird::HalfStaticArray<UCHAR, BUFFER_SMALL> utf16Str;
	ULONG utf16Len = UnicodeUtil::utf8ToUtf16(srcLen, src, 0, NULL,
		&err_code, &err_position);

	utf16Len = UnicodeUtil::utf8ToUtf16(srcLen, src,
		utf16Len, reinterpret_cast<USHORT*>(utf16Str.getBuffer(utf16Len)),
		&err_code, &err_position);

	if (obj->texttype_pad_option)
	{
		const UCHAR* pad;

		for (pad = utf16Str.begin() + utf16Len - sizeof(USHORT); pad >= utf16Str.begin(); pad -= sizeof(USHORT))
		{
			if (*reinterpret_cast<const USHORT*>(pad) != 32)
				break;
		}

		utf16Len = pad - utf16Str.begin() + sizeof(USHORT);
	}

	return ((UnicodeUtil::Utf16Collation*)obj->reserved_for_driver[0])->stringToKey(
		utf16Len, (USHORT*)utf16Str.begin(), dstLen, dst, key_type);
}


static SSHORT unicode8_compare(TEXTTYPE obj, ULONG length1, const UCHAR* p1,
							   ULONG length2, const UCHAR* p2, INTL_BOOL* error_flag)
{
/**************************************
 *
 *      u n i c o d e 8 _ c o m p a r e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	fb_assert(obj != NULL);

	USHORT err_code;
	ULONG err_position;

	Firebird::HalfStaticArray<UCHAR, BUFFER_SMALL> utf16Str1;
	Firebird::HalfStaticArray<UCHAR, BUFFER_SMALL> utf16Str2;
	ULONG utf16Len1 = UnicodeUtil::utf8ToUtf16(length1, p1, 0, NULL,
		&err_code, &err_position);
	ULONG utf16Len2 = UnicodeUtil::utf8ToUtf16(length2, p2, 0, NULL,
		&err_code, &err_position);

	utf16Len1 = UnicodeUtil::utf8ToUtf16(length1, p1,
		utf16Len1, reinterpret_cast<USHORT*>(utf16Str1.getBuffer(utf16Len1)),
		&err_code, &err_position);

	utf16Len2 = UnicodeUtil::utf8ToUtf16(length2, p2,
		utf16Len2, reinterpret_cast<USHORT*>(utf16Str2.getBuffer(utf16Len2)),
		&err_code, &err_position);

	if (obj->texttype_pad_option)
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

	return ((UnicodeUtil::Utf16Collation*)obj->reserved_for_driver[0])->compare(
		utf16Len1, (USHORT*)utf16Str1.begin(),
		utf16Len2, (USHORT*)utf16Str2.begin(), error_flag);
}


static USHORT utf16_keylength(TEXTTYPE obj, USHORT len)
{
/**************************************
 *
 *      u t f 1 6 _ k e y l e n g t h
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	return UnicodeUtil::utf16KeyLength(len);
}

static USHORT utf16_string_to_key(
								  TEXTTYPE obj,
								  USHORT srcLen,
								  const UCHAR* src,
								  USHORT dstLen,
								  UCHAR* dst,
								  USHORT key_type)
{
/**************************************
 *
 *      u t f 1 6 _ s t r i n g _ t o _ k e y
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	fb_assert(obj != NULL);
	fb_assert(srcLen % 2 == 0);

	if (obj->texttype_pad_option)
	{
		const UCHAR* pad;

		for (pad = src + srcLen - sizeof(USHORT); pad >= src; pad -= sizeof(USHORT))
		{
			if (*reinterpret_cast<const USHORT*>(pad) != 32)
				break;
		}

		srcLen = pad - src + sizeof(USHORT);
	}

	return UnicodeUtil::utf16ToKey(srcLen, reinterpret_cast<const USHORT*>(src), dstLen, dst, key_type);
}

static SSHORT utf16_compare(
							TEXTTYPE obj,
							ULONG len1, 
							const UCHAR* str1, 
							ULONG len2, 
							const UCHAR* str2,
							INTL_BOOL* error_flag)
{
/**************************************
 *
 *      u t f 1 6 _ c o m p a r e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	fb_assert(obj != NULL);
	fb_assert(len1 % 2 == 0 && len2 % 2 == 0);
	fb_assert(str1 != NULL && str2 != NULL);

	if (obj->texttype_pad_option)
	{
		const UCHAR* pad;

		for (pad = str1 + len1 - sizeof(USHORT); pad >= str1; pad -= sizeof(USHORT))
		{
			if (*reinterpret_cast<const USHORT*>(pad) != 32)
				break;
		}

		len1 = pad - str1 + sizeof(USHORT);

		for (pad = str2 + len2 - sizeof(USHORT); pad >= str2; pad -= sizeof(USHORT))
		{
			if (*reinterpret_cast<const USHORT*>(pad) != 32)
				break;
		}

		len2 = pad - str2 + sizeof(USHORT);
	}

	return UnicodeUtil::utf16Compare(len1, reinterpret_cast<const USHORT*>(str1),
									 len2, reinterpret_cast<const USHORT*>(str2), error_flag);
}

static ULONG utf16_upper(
						 TEXTTYPE obj,
						 ULONG srcLen, 
						 const UCHAR* src, 
						 ULONG dstLen, 
						 UCHAR* dst)
{
/**************************************
 *
 *      u t f 1 6 _ u p p e r
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	fb_assert(obj != NULL);
	fb_assert(srcLen % 2 == 0);
	fb_assert(src != NULL && dst != NULL);

	return UnicodeUtil::utf16UpperCase(srcLen, reinterpret_cast<const USHORT*>(src),
									   dstLen, reinterpret_cast<USHORT*>(dst));
}

static ULONG utf16_lower(
						 TEXTTYPE obj,
						 ULONG srcLen, 
						 const UCHAR* src, 
						 ULONG dstLen, 
						 UCHAR* dst)
{
/**************************************
 *
 *      u t f 1 6 _ l o w e r
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	fb_assert(obj != NULL);
	fb_assert(srcLen % 2 == 0);
	fb_assert(src != NULL && dst != NULL);

	return UnicodeUtil::utf16LowerCase(srcLen, reinterpret_cast<const USHORT*>(src),
									   dstLen, reinterpret_cast<USHORT*>(dst));
}


static USHORT utf32_keylength(TEXTTYPE obj, USHORT len)
{
/**************************************
 *
 *      u t f 3 2 _ k e y l e n g t h
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	return len;
}

static USHORT utf32_string_to_key(
								  TEXTTYPE obj,
								  USHORT srcLen,
								  const UCHAR* src,
								  USHORT dstLen,
								  UCHAR* dst,
								  USHORT key_type)
{
/**************************************
 *
 *      u t f 3 2 _ s t r i n g _ t o _ k e y
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	fb_assert(obj != NULL);
	fb_assert(srcLen % 4 == 0);

	USHORT err_code;
	ULONG err_position;

	Firebird::HalfStaticArray<UCHAR, BUFFER_SMALL> utf16Str;
	srcLen = UnicodeUtil::utf32ToUtf16(srcLen, reinterpret_cast<const ULONG*>(src),
		dstLen, reinterpret_cast<USHORT*>(utf16Str.getBuffer(dstLen)), &err_code, &err_position);
	src = utf16Str.begin();

	if (obj->texttype_pad_option)
	{
		const UCHAR* pad;

		for (pad = src + srcLen - sizeof(USHORT); pad >= src; pad -= sizeof(USHORT))
		{
			if (*reinterpret_cast<const USHORT*>(pad) != 32)
				break;
		}

		srcLen = pad - src + sizeof(USHORT);
	}

	return UnicodeUtil::utf16ToKey(srcLen, reinterpret_cast<const USHORT*>(src), dstLen, dst, key_type);
}


static ULONG wc_to_nc(csconvert* obj, ULONG nSrc, const USHORT* pSrc,
					  ULONG nDest, UCHAR* pDest,
					  USHORT* err_code, ULONG* err_position)
{
/**************************************
 *
 *      w c _ t o _ n c
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	fb_assert(obj != NULL);
	fb_assert((pSrc != NULL) || (pDest == NULL));
	fb_assert(err_code != NULL);
	fb_assert(err_position != NULL);

	*err_code = 0;
	if (pDest == NULL)			/* length estimate needed? */
		return ((nSrc + 1) / 2);

	const UCHAR* const pStart = pDest;
	const USHORT* const pStart_src = pSrc;

	while (nDest && nSrc >= sizeof(*pSrc)) {
		if (*pSrc >= 256) {
			*err_code = CS_CONVERT_ERROR;
			break;
		}
		*pDest++ = *pSrc++;
		nDest -= sizeof(*pDest);
		nSrc -= sizeof(*pSrc);
	}
	if (!*err_code && nSrc) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = (pSrc - pStart_src) * sizeof(*pSrc);

	return ((pDest - pStart) * sizeof(*pDest));
}


static ULONG mb_to_wc(csconvert* obj, ULONG nSrc, const UCHAR* pSrc,
					  ULONG nDest, USHORT* pDest,
					  USHORT* err_code, ULONG* err_position)
{
/**************************************
 *
 *      m b _ t o _ w c
 *
 **************************************
 *
 * Functional description
 *      Convert a wc string from network form - high-endian
 *      byte stream.
 *
 *************************************/
	fb_assert(obj != NULL);
	fb_assert((pSrc != NULL) || (pDest == NULL));
	fb_assert(err_code != NULL);
	fb_assert(err_position != NULL);

	*err_code = 0;
	if (pDest == NULL)			/* length estimate needed? */
		return (nSrc);

	const USHORT* const pStart = pDest;
	const UCHAR* const pStart_src = pSrc;
	while (nDest > 1 && nSrc > 1) {
		*pDest++ = *pSrc * 256 + *(pSrc + 1);
		pSrc += 2;
		nDest -= 2;
		nSrc -= 2;
	}
	if (!*err_code && nSrc) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = (pSrc - pStart_src) * sizeof(*pSrc);

	return ((pDest - pStart) * sizeof(*pDest));
}


static ULONG wc_to_mb(csconvert* obj, ULONG nSrc, const USHORT* pSrc,
					  ULONG nDest, UCHAR* pDest,
					  USHORT* err_code, ULONG* err_position)
{
/**************************************
 *
 *      w c _ t o _ m b 
 *
 **************************************
 *
 * Functional description
 *      Convert a wc string to network form - high-endian
 *      byte stream.
 *
 *************************************/
	fb_assert(obj != NULL);
	fb_assert((pSrc != NULL) || (pDest == NULL));
	fb_assert(err_code != NULL);
	fb_assert(err_position != NULL);

	*err_code = 0;
	if (pDest == NULL)			/* length estimate needed? */
		return (nSrc);

	const UCHAR* const pStart = pDest;
	const USHORT* const pStart_src = pSrc;
	while (nDest > 1 && nSrc > 1) {
		*pDest++ = *pSrc / 256;
		*pDest++ = *pSrc++ % 256;
		nDest -= 2;
		nSrc -= 2;
	}
	if (!*err_code && nSrc) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = (pSrc - pStart_src) * sizeof(*pSrc);

	return ((pDest - pStart) * sizeof(*pDest));
}

static INTL_BOOL ttype_ascii_init(TEXTTYPE cache, const ASCII* texttype_name, const ASCII* charset_name,
								  USHORT attributes, const UCHAR* specific_attributes,
								  ULONG specific_attributes_length)
{
/**************************************
 *
 *      t t y p e _ a s c i i _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/
	static const ASCII POSIX[] = "C.ASCII";

	return FAMILY_INTERNAL(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


static INTL_BOOL ttype_none_init(TEXTTYPE cache, const ASCII* texttype_name, const ASCII* charset_name,
								 USHORT attributes, const UCHAR* specific_attributes,
								 ULONG specific_attributes_length)
{
/**************************************
 *
 *      t t y p e _ n o n e _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/
	static const ASCII POSIX[] = "C";

	return FAMILY_INTERNAL(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length);
}


static INTL_BOOL ttype_unicode_fss_init(TEXTTYPE cache, const ASCII* texttype_name, const ASCII* charset_name,
										USHORT attributes, const UCHAR* specific_attributes,
										ULONG specific_attributes_length)
{
/**************************************
 *
 *      t t y p e _ u n i c o d e _ f s s _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/
	static const ASCII POSIX[] = "C.UNICODE_FSS";

	if (FAMILY_INTERNAL(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length))
	{
		cache->texttype_flags |= TEXTTYPE_DIRECT_MATCH;
		cache->texttype_fn_str_to_upper	= NULL;		// use default implementation
		cache->texttype_fn_str_to_lower	= NULL;		// use default implementation
		return true;
	}

	return false;
}


static INTL_BOOL ttype_binary_init(TEXTTYPE cache, const ASCII* texttype_name, const ASCII* charset_name,
								   USHORT attributes, const UCHAR* specific_attributes,
								   ULONG specific_attributes_length)
{
/**************************************
 *
 *      t t y p e _ b i n a r y _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/
	static const ASCII POSIX[] = "C.OCTETS";

	if (FAMILY_INTERNAL(cache, CC_C, POSIX, attributes, specific_attributes, specific_attributes_length))
	{
		cache->texttype_fn_str_to_upper = internal_str_copy;
		cache->texttype_fn_str_to_lower = internal_str_copy;
		cache->texttype_impl->texttype_pad_char = '\0';
		return true;
	}

	return false;
}


static INTL_BOOL ttype_utf8_init(TEXTTYPE cache, const ASCII* texttype_name, const ASCII* charset_name,
								 USHORT attributes, const UCHAR* specific_attributes,
								 ULONG specific_attributes_length)
{
/**************************************
 *
 *      t t y p e _ u t f 8 _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/
	static const ASCII POSIX[] = "C.UTF8";

	return FAMILY_INTERNAL_UTF(cache, POSIX, attributes, specific_attributes, specific_attributes_length);
}


static INTL_BOOL ttype_unicode8_init(TEXTTYPE cache, const ASCII* texttype_name, const ASCII* charset_name,
									 USHORT attributes, const UCHAR* specific_attributes,
									 ULONG specific_attributes_length)
{
/**************************************
 *
 *      t t y p e _ u n i c o d e 8 _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/
	static const ASCII POSIX[] = "C.UTF8.UNICODE";

	if (FAMILY_INTERNAL_UNICODE(cache, POSIX, attributes, specific_attributes, specific_attributes_length))
	{
		cache->texttype_fn_key_length = unicode8_keylength;
		cache->texttype_fn_string_to_key = unicode8_string_to_key;
		cache->texttype_fn_compare = unicode8_compare;

		return true;
	}

	return false;
}


static INTL_BOOL ttype_utf16_init(TEXTTYPE cache, const ASCII* texttype_name, const ASCII* charset_name,
								  USHORT attributes, const UCHAR* specific_attributes,
								  ULONG specific_attributes_length)
{
/**************************************
 *
 *      t t y p e _ u t f 1 6 _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/
	static const ASCII POSIX[] = "C.UTF16";

	if (FAMILY_INTERNAL_UTF(cache, POSIX, attributes, specific_attributes, specific_attributes_length))
	{
		cache->texttype_fn_key_length = utf16_keylength;
		cache->texttype_fn_string_to_key = utf16_string_to_key;
		cache->texttype_fn_compare = utf16_compare;
		cache->texttype_fn_str_to_upper = utf16_upper;
		cache->texttype_fn_str_to_lower = utf16_lower;
		return true;
	}

	return false;
}


static INTL_BOOL ttype_utf32_init(TEXTTYPE cache, const ASCII* texttype_name, const ASCII* charset_name,
								  USHORT attributes, const UCHAR* specific_attributes,
								  ULONG specific_attributes_length)
{
/**************************************
 *
 *      t t y p e _ u t f 3 2 _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/
	static const ASCII POSIX[] = "C.UTF32";

	if (FAMILY_INTERNAL_UTF(cache, POSIX, attributes, specific_attributes, specific_attributes_length))
	{
		cache->texttype_fn_key_length = utf32_keylength;
		cache->texttype_fn_string_to_key = utf32_string_to_key;
		return true;
	}

	return false;
}


/*
 *      Start of Character set definitions 
 */

static void common_8bit_init(
							 charset* csptr,
							 const ASCII* name,
							 const USHORT* to_unicode_tbl,
							 const UCHAR* from_unicode_tbl1,
							 const USHORT* from_unicode_tbl2)
{
/**************************************
 *
 *      c o m m o n _ 8 b i t _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/

	csptr->charset_version = CHARSET_VERSION_1;
	csptr->charset_name = name;
	csptr->charset_flags |= CHARSET_ASCII_BASED;
	csptr->charset_min_bytes_per_char = 1;
	csptr->charset_max_bytes_per_char = 1;
	csptr->charset_space_length = 1;
	csptr->charset_space_character = (const BYTE*) " ";
	csptr->charset_fn_well_formed = NULL;
}


static INTL_BOOL cs_utf8_well_formed(charset* cs, 
									 ULONG len,
									 const UCHAR* str,
									 ULONG* offending_position)
{
/**************************************
 *
 *      c s _ u t f 8 _ w e l l _ f o r m e d
 *
 **************************************
 *
 * Functional description
 *      Check if UTF-8 string is weel-formed
 *
 *************************************/
	fb_assert(cs != NULL);

	return UnicodeUtil::utf8WellFormed(len, str, offending_position);
}


static INTL_BOOL cs_utf16_well_formed(charset* cs, 
									  ULONG len,
									  const UCHAR* str,
									  ULONG* offending_position)
{
/**************************************
 *
 *      c s _ u t f 1 6 _ w e l l _ f o r m e d
 *
 **************************************
 *
 * Functional description
 *      Check if UTF-16 string is weel-formed
 *
 *************************************/
	fb_assert(cs != NULL);

	return UnicodeUtil::utf16WellFormed(len, reinterpret_cast<const USHORT*>(str), offending_position);
}


static ULONG cs_utf16_length(charset* cs, 
							 ULONG srcLen,
							 const UCHAR* src)
{
/**************************************
 *
 *      c s _ u t f 1 6 _ l e n g t h
 *
 **************************************
 *
 * Functional description
 *      Length of UTF-16 string
 *
 *************************************/
	fb_assert(cs != NULL);
	return UnicodeUtil::utf16Length(srcLen, reinterpret_cast<const USHORT*>(src));
}


static ULONG cs_utf16_substring(charset* cs, 
								ULONG srcLen,
								const UCHAR* src,
								ULONG dstLen,
								UCHAR* dst,
								ULONG startPos,
								ULONG length)
{
/**************************************
 *
 *      c s _ u t f 1 6 _ s u b s t r i n g
 *
 **************************************
 *
 * Functional description
 *      Substring of UTF-16 string
 *
 *************************************/
	fb_assert(cs != NULL);

	return UnicodeUtil::utf16Substring(srcLen, reinterpret_cast<const USHORT*>(src),
		dstLen, reinterpret_cast<USHORT*>(dst), startPos, length);
}


static INTL_BOOL cs_utf32_well_formed(charset* cs, 
									  ULONG len,
									  const UCHAR* str,
									  ULONG* offending_position)
{
/**************************************
 *
 *      c s _ u t f 3 2 _ w e l l _ f o r m e d
 *
 **************************************
 *
 * Functional description
 *      Check if UTF-32 string is weel-formed
 *
 *************************************/
	fb_assert(cs != NULL);

	return UnicodeUtil::utf32WellFormed(len, reinterpret_cast<const ULONG*>(str), offending_position);
}


static void common_convert_init(
								csconvert* csptr,
								pfn_INTL_convert cvt_fn)
{
/**************************************
 *
 *      c o m m o n _ c o n v e r t _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	csptr->csconvert_version = CSCONVERT_VERSION_1;
	csptr->csconvert_name = (const ASCII*) "DIRECT";
	csptr->csconvert_fn_convert = cvt_fn;
}


static ULONG cvt_ascii_to_unicode(csconvert* obj, ULONG nSrc, const UCHAR* pSrc,
								  ULONG nDest, USHORT* pDest,
								  USHORT* err_code, ULONG* err_position)
{
/**************************************
 *
 *      c v t _ a s c i i _ t o _ u n i c o d e
 *
 **************************************
 *
 * Functional description
 *      Convert CHARACTER SET NONE to UNICODE (wide char).
 *      Byte values below 128 treated as ASCII.
 *      Byte values >= 128 create BAD_INPUT
 *
 *************************************/
	fb_assert(obj != NULL);
	fb_assert((pSrc != NULL) || (pDest == NULL));
	fb_assert(err_code != NULL);

	*err_code = 0;
	if (pDest == NULL)			/* length estimate needed? */
		return (2 * nSrc);

	const USHORT* const pStart = pDest;
	const UCHAR* const pStart_src = pSrc;
	while (nDest >= sizeof(*pDest) && nSrc >= sizeof(*pSrc)) {
		if (*pSrc > 127) {
			*err_code = CS_BAD_INPUT;
			break;
		}
		*pDest++ = *pSrc++;
		nDest -= sizeof(*pDest);
		nSrc -= sizeof(*pSrc);
	}
	if (!*err_code && nSrc) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = (pSrc - pStart_src) * sizeof(*pSrc);

	return ((pDest - pStart) * sizeof(*pDest));
}


static ULONG cvt_unicode_to_ascii(csconvert* obj, ULONG nSrc, const USHORT* pSrc,
								  ULONG nDest, UCHAR* pDest,
								  USHORT* err_code, ULONG* err_position)
{
/**************************************
 *
 *      c v t _ u n i c o d e _ t o _ a s c i i 
 *
 **************************************
 *
 * Functional description
 *      Convert UNICODE to CHARACTER SET ASCII (wide char).
 *      Byte values below 128 treated as ASCII.
 *      Byte values >= 128 create CONVERT_ERROR
 *
 *************************************/
	fb_assert(obj != NULL);
	fb_assert((pSrc != NULL) || (pDest == NULL));
	fb_assert(err_code != NULL);

	*err_code = 0;
	if (pDest == NULL)			/* length estimate needed? */
		return (nSrc / 2);

	const UCHAR* const pStart = pDest;
	const USHORT* const pStart_src = pSrc;
	while (nDest >= sizeof(*pDest) && nSrc >= sizeof(*pSrc)) {
		if (*pSrc > 127) {
			*err_code = CS_CONVERT_ERROR;
			break;
		}
		*pDest++ = *pSrc++;
		nDest -= sizeof(*pDest);
		nSrc -= sizeof(*pSrc);
	}
	if (!*err_code && nSrc) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = (pSrc - pStart_src) * sizeof(*pSrc);

	return ((pDest - pStart) * sizeof(*pDest));
}


static ULONG cvt_none_to_unicode(csconvert* obj, ULONG nSrc, const UCHAR* pSrc,
								 ULONG nDest, USHORT* pDest,
								 USHORT* err_code, ULONG* err_position)
{
/**************************************
 *
 *      c v t _ n o n e _ t o _ u n i c o d e
 *
 **************************************
 *
 * Functional description
 *      Convert CHARACTER SET NONE to UNICODE (wide char).
 *      Byte values below 128 treated as ASCII.
 *      Byte values >= 128 create CONVERT ERROR
 *
 *************************************/
	fb_assert(obj != NULL);
	fb_assert((pSrc != NULL) || (pDest == NULL));
	fb_assert(err_code != NULL);

	*err_code = 0;
	if (pDest == NULL)			/* length estimate needed? */
		return (2 * nSrc);

	const USHORT* const pStart = pDest;
	const UCHAR* const pStart_src = pSrc;
	while (nDest >= sizeof(*pDest) && nSrc >= sizeof(*pSrc)) {
		if (*pSrc > 127) {
			*err_code = CS_CONVERT_ERROR;
			break;
		}
		*pDest++ = *pSrc++;
		nDest -= sizeof(*pDest);
		nSrc -= sizeof(*pSrc);
	}
	if (!*err_code && nSrc) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = (pSrc - pStart_src) * sizeof(*pSrc);

	return ((pDest - pStart) * sizeof(*pDest));
}


static ULONG cvt_unicode_to_unicode(csconvert* obj, ULONG nSrc, const USHORT* pSrc,
									ULONG nDest, USHORT* pDest,
									USHORT* err_code, ULONG* err_position)
{
/**************************************
 *
 *      c v t _ u n i c o d e _ t o _ u n i c o d e
 *
 **************************************
 *
 * Functional description
 *
 *************************************/
	fb_assert(obj != NULL);
	fb_assert((pSrc != NULL) || (pDest == NULL));
	fb_assert(err_code != NULL);

	*err_code = 0;
	if (pDest == NULL)			/* length estimate needed? */
		return nSrc;

	const USHORT* const pStart = pDest;
	const USHORT* const pStart_src = pSrc;
	while (nDest >= sizeof(*pDest) && nSrc >= sizeof(*pSrc)) {
		*pDest++ = *pSrc++;
		nDest -= sizeof(*pDest);
		nSrc -= sizeof(*pSrc);
	}
	if (!*err_code && nSrc) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = (pSrc - pStart_src) * sizeof(*pSrc);

	return ((pDest - pStart) * sizeof(*pDest));
}


static ULONG cvt_utffss_to_ascii(csconvert* obj, ULONG nSrc, const UCHAR* pSrc,
								 ULONG nDest, UCHAR* pDest,
								 USHORT* err_code, ULONG* err_position)
{
/**************************************
 *
 *      c v t _ u t f f s s _ t o _ a s c i i 
 * also
 *      c v t _ a s c i i _ t o _ u t f f s s 
 * also
 *      c v t _ n o n e _ t o _ u t f f s s 
 *
 **************************************
 *
 * Functional description
 *      Perform a pass-through transformation of ASCII to Unicode
 *      in FSS format.  Note that any byte values greater than 127
 *      cannot be converted in either direction, so the same
 *      routine does double duty.
 *
 *************************************/
	fb_assert(obj != NULL);
	fb_assert((pSrc != NULL) || (pDest == NULL));
	fb_assert(err_code != NULL);

	*err_code = 0;
	if (pDest == NULL)			/* length estimate needed? */
		return (nSrc);

	const UCHAR* const pStart = pDest;
	const UCHAR* const pStart_src = pSrc;
	while (nDest >= sizeof(*pDest) && nSrc >= sizeof(*pSrc)) {
		if (*pSrc > 127) {
			/* In the cvt_ascii_to_utffss case this should be CS_BAD_INPUT */
			/* but not in cvt_none_to_utffss or cvt_utffss_to_ascii */
			*err_code = CS_CONVERT_ERROR;
			break;
		}
		*pDest++ = *pSrc++;
		nDest -= sizeof(*pDest);
		nSrc -= sizeof(*pSrc);
	}
	if (!*err_code && nSrc) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = (pSrc - pStart_src) * sizeof(*pSrc);

	return ((pDest - pStart) * sizeof(*pDest));
}


static ULONG cvt_unicode_to_utf8(csconvert* obj,
								 ULONG unicode_len,
								 const USHORT* unicode_str,
								 ULONG utf8_len,
								 UCHAR* utf8_str,
								 USHORT* err_code,
								 ULONG* err_position)
{
	fb_assert(obj != NULL);
	fb_assert(obj->csconvert_fn_convert == (pfn_INTL_convert) cvt_unicode_to_utf8);
	return UnicodeUtil::utf16ToUtf8(unicode_len, unicode_str, utf8_len, utf8_str, err_code, err_position);
}


static ULONG cvt_utf8_to_unicode(csconvert* obj,
								 ULONG utf8_len,
								 const UCHAR* utf8_str,
								 ULONG unicode_len,
								 USHORT* unicode_str,
								 USHORT* err_code,
								 ULONG* err_position)
{
	fb_assert(obj != NULL);
	fb_assert(obj->csconvert_fn_convert == (pfn_INTL_convert) cvt_utf8_to_unicode);
	return UnicodeUtil::utf8ToUtf16(utf8_len, utf8_str, unicode_len, unicode_str, err_code, err_position);
}


static ULONG cvt_unicode_to_utf32(csconvert* obj,
								  ULONG unicode_len,
								  const USHORT* unicode_str,
								  ULONG utf32_len,
								  ULONG* utf32_str,
								  USHORT* err_code,
								  ULONG* err_position)
{
	fb_assert(obj != NULL);
	fb_assert(obj->csconvert_fn_convert == (pfn_INTL_convert) cvt_unicode_to_utf32);
	return UnicodeUtil::utf16ToUtf32(unicode_len, unicode_str, utf32_len, utf32_str, err_code, err_position);
}


static ULONG cvt_utf32_to_unicode(csconvert* obj,
								  ULONG utf32_len,
								  const ULONG* utf32_str,
								  ULONG unicode_len,
								  USHORT* unicode_str,
								  USHORT* err_code,
								  ULONG* err_position)
{
	fb_assert(obj != NULL);
	fb_assert(obj->csconvert_fn_convert == (pfn_INTL_convert) cvt_utf32_to_unicode);
	return UnicodeUtil::utf32ToUtf16(utf32_len, utf32_str, unicode_len, unicode_str, err_code, err_position);
}


static INTL_BOOL cs_ascii_init(charset* csptr, const ASCII* charset_name)
{
/**************************************
 *
 *      c s _ a s c i i _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/

	common_8bit_init(csptr, (const ASCII*) "ASCII", NULL, NULL, NULL);
	common_convert_init(&csptr->charset_to_unicode,
						reinterpret_cast<pfn_INTL_convert>(cvt_ascii_to_unicode));
	common_convert_init(&csptr->charset_from_unicode,
						reinterpret_cast<pfn_INTL_convert>(cvt_unicode_to_ascii));
	return true;
}


static INTL_BOOL cs_none_init(charset* csptr, const ASCII* charset_name)
{
/**************************************
 *
 *      c s _ n o n e _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/

	common_8bit_init(csptr, (const ASCII*) "NONE", NULL, NULL, NULL);
/*
common_convert_init (&csptr->charset_to_unicode, CS_UTF16, id,
	nc_to_wc, to_unicode_tbl, NULL);
*/
	common_convert_init(&csptr->charset_to_unicode,
						reinterpret_cast<pfn_INTL_convert>(cvt_none_to_unicode));
	common_convert_init(&csptr->charset_from_unicode,
						reinterpret_cast<pfn_INTL_convert>(wc_to_nc));
	return true;
}


static INTL_BOOL cs_unicode_fss_init(charset* csptr, const ASCII* charset_name)
{
/**************************************
 *
 *      c s _ u n i c o d e _ f s s _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/

	common_8bit_init(csptr, (const ASCII*) "UNICODE_FSS", NULL,
					 NULL, NULL);
	csptr->charset_max_bytes_per_char = 3;
	csptr->charset_flags |= CHARSET_LEGACY_SEMANTICS;

	common_convert_init(&csptr->charset_to_unicode,
						reinterpret_cast<pfn_INTL_convert>(internal_fss_to_unicode));
	common_convert_init(&csptr->charset_from_unicode,
						reinterpret_cast<pfn_INTL_convert>(internal_unicode_to_fss));
	csptr->charset_fn_length = internal_fss_length;
	csptr->charset_fn_substring = internal_fss_substring;

	return true;
}


static INTL_BOOL cs_unicode_ucs2_init(charset* csptr, const ASCII* charset_name)
{
/**************************************
 *
 *      c s _ u n i c o d e _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/
	static const USHORT space = 0x0020;

	csptr->charset_version = CHARSET_VERSION_1;
	csptr->charset_name = "UNICODE_UCS2";
	csptr->charset_flags |= CHARSET_ASCII_BASED;
	csptr->charset_min_bytes_per_char = 2;
	csptr->charset_max_bytes_per_char = 2;
	csptr->charset_space_length = 2;
	csptr->charset_space_character = (const BYTE*) & space;	/* 0x0020 */
	csptr->charset_fn_well_formed = NULL;

	common_convert_init(&csptr->charset_to_unicode,
						reinterpret_cast<pfn_INTL_convert>(cvt_unicode_to_unicode));
	common_convert_init(&csptr->charset_from_unicode,
						reinterpret_cast<pfn_INTL_convert>(cvt_unicode_to_unicode));

	return true;
}


static INTL_BOOL cs_binary_init(charset* csptr, const ASCII* charset_name)
{
/**************************************
 *
 *      c s _ b i n a r y _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/

	common_8bit_init(csptr, (const ASCII*) "BINARY", NULL, NULL, NULL);
	csptr->charset_space_character = (const BYTE*) "\0";
	common_convert_init(&csptr->charset_to_unicode,
						reinterpret_cast<pfn_INTL_convert>(mb_to_wc));
	common_convert_init(&csptr->charset_from_unicode,
						reinterpret_cast<pfn_INTL_convert>(wc_to_mb));
	return true;
}


static INTL_BOOL cs_utf8_init(charset* csptr, const ASCII* charset_name)
{
/**************************************
 *
 *      c s _ u t f 8 _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/

	static UCHAR space = 32;

	csptr->charset_version = CHARSET_VERSION_1;
	csptr->charset_name = "UTF8";
	csptr->charset_flags |= CHARSET_ASCII_BASED;
	csptr->charset_min_bytes_per_char = 1;
	csptr->charset_max_bytes_per_char = 4;
	csptr->charset_space_length = 1;
	csptr->charset_space_character = (const BYTE*)&space;
	csptr->charset_fn_well_formed = cs_utf8_well_formed;

	common_convert_init(&csptr->charset_to_unicode,
						reinterpret_cast<pfn_INTL_convert>(cvt_utf8_to_unicode));
	common_convert_init(&csptr->charset_from_unicode,
						reinterpret_cast<pfn_INTL_convert>(cvt_unicode_to_utf8));
	return true;
}


static INTL_BOOL cs_utf16_init(charset* csptr, const ASCII* charset_name)
{
/**************************************
 *
 *      c s _ u t f 1 6 _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/

	static USHORT space = 32;

	csptr->charset_version = CHARSET_VERSION_1;
	csptr->charset_name = "UTF16";
	csptr->charset_flags |= CHARSET_ASCII_BASED;
	csptr->charset_min_bytes_per_char = 2;
	csptr->charset_max_bytes_per_char = 4;
	csptr->charset_space_length = 2;
	csptr->charset_space_character = (const BYTE*)&space;
	csptr->charset_fn_well_formed = cs_utf16_well_formed;
	csptr->charset_fn_length = cs_utf16_length;
	csptr->charset_fn_substring = cs_utf16_substring;

	common_convert_init(&csptr->charset_to_unicode,
						reinterpret_cast<pfn_INTL_convert>(cvt_unicode_to_unicode));
	common_convert_init(&csptr->charset_from_unicode,
						reinterpret_cast<pfn_INTL_convert>(cvt_unicode_to_unicode));
	return true;
}


static INTL_BOOL cs_utf32_init(charset* csptr, const ASCII* charset_name)
{
/**************************************
 *
 *      c s _ u t f 3 2 _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/

	static ULONG space = 32;

	csptr->charset_version = CHARSET_VERSION_1;
	csptr->charset_name = "UTF32";
	csptr->charset_flags |= CHARSET_ASCII_BASED;
	csptr->charset_min_bytes_per_char = 4;
	csptr->charset_max_bytes_per_char = 4;
	csptr->charset_space_length = 4;
	csptr->charset_space_character = (const BYTE*)&space;
	csptr->charset_fn_well_formed = cs_utf32_well_formed;

	common_convert_init(&csptr->charset_to_unicode,
						reinterpret_cast<pfn_INTL_convert>(cvt_utf32_to_unicode));
	common_convert_init(&csptr->charset_from_unicode,
						reinterpret_cast<pfn_INTL_convert>(cvt_unicode_to_utf32));
	return true;
}


/*
 *      Start of Conversion entries
 */

static USHORT cvt_ascii_utf_init(csconvert* csptr)
{
/**************************************
 *
 *      c v t _ a s c i i _ u t f _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/

	common_convert_init(csptr, cvt_utffss_to_ascii);
	return true;
}


INTL_BOOL INTL_builtin_lookup_charset(charset* cs, const ASCII* charset_name)
{
	if (strcmp(charset_name, "NONE") == 0)
		return cs_none_init(cs, charset_name);
	if (strcmp(charset_name, "ASCII") == 0 ||
		strcmp(charset_name, "USASCII") == 0 ||
		strcmp(charset_name, "ASCII7") == 0)
	{
		return cs_ascii_init(cs, charset_name);
	}
	if (strcmp(charset_name, "UNICODE_FSS") == 0 ||
		strcmp(charset_name, "UTF_FSS") == 0 ||
		strcmp(charset_name, "SQL_TEXT") == 0)
	{
		return cs_unicode_fss_init(cs, charset_name);
	}
	if (strcmp(charset_name, "UNICODE_UCS2") == 0)
		return cs_unicode_ucs2_init(cs, charset_name);
	if (strcmp(charset_name, "OCTETS") == 0 ||
		strcmp(charset_name, "BINARY") == 0)
	{
		return cs_binary_init(cs, charset_name);
	}
	if (strcmp(charset_name, "UTF8") == 0 ||
		strcmp(charset_name, "UTF-8") == 0)
	{
		return cs_utf8_init(cs, charset_name);
	}
	if (strcmp(charset_name, "UTF16") == 0 ||
		strcmp(charset_name, "UTF-16") == 0)
	{
		return cs_utf16_init(cs, charset_name);
	}
	if (strcmp(charset_name, "UTF32") == 0 ||
		strcmp(charset_name, "UTF-32") == 0)
	{
		return cs_utf32_init(cs, charset_name);
	}

	return false;
}


INTL_BOOL INTL_builtin_lookup_texttype(texttype* tt, const ASCII* texttype_name, const ASCII* charset_name,
									   USHORT attributes, const UCHAR* specific_attributes,
									   ULONG specific_attributes_length, INTL_BOOL ignore_attributes)
{
	if (ignore_attributes)
	{
		attributes = TEXTTYPE_ATTR_PAD_SPACE;
		specific_attributes = NULL;
		specific_attributes_length = 0;
	}

	if (strcmp(texttype_name, "NONE") == 0)
	{
		return ttype_none_init(tt, texttype_name, charset_name, attributes,
							   specific_attributes, specific_attributes_length);
	}
	if (strcmp(texttype_name, "ASCII") == 0)
	{
		return ttype_ascii_init(tt, texttype_name, charset_name, attributes,
								specific_attributes, specific_attributes_length);
	}
	if (strcmp(texttype_name, "UNICODE_FSS") == 0)
	{
		return ttype_unicode_fss_init(tt, texttype_name, charset_name, attributes,
									  specific_attributes, specific_attributes_length);
	}
	if (strcmp(texttype_name, "OCTETS") == 0)
	{
		return ttype_binary_init(tt, texttype_name, charset_name, attributes,
								 specific_attributes, specific_attributes_length);
	}
	if (strcmp(texttype_name, "UTF8") == 0 ||
		(strcmp(charset_name, "UTF8") == 0 && strcmp(texttype_name, "UCS_BASIC") == 0))
	{
		return ttype_utf8_init(tt, texttype_name, charset_name, attributes,
							   specific_attributes, specific_attributes_length);
	}
	if ((strcmp(charset_name, "UTF8") == 0 && strcmp(texttype_name, "UNICODE") == 0))
	{
		return ttype_unicode8_init(tt, texttype_name, charset_name, attributes,
								   specific_attributes, specific_attributes_length);
	}
	if (strcmp(texttype_name, "UTF16") == 0 ||
		(strcmp(charset_name, "UTF16") == 0 && strcmp(texttype_name, "UCS_BASIC") == 0))
	{
		return ttype_utf16_init(tt, texttype_name, charset_name, attributes,
								specific_attributes, specific_attributes_length);
	}
	if (strcmp(texttype_name, "UTF32") == 0 ||
		(strcmp(charset_name, "UTF32") == 0 && strcmp(texttype_name, "UCS_BASIC") == 0))
	{
		return ttype_utf32_init(tt, texttype_name, charset_name, attributes,
								specific_attributes, specific_attributes_length);
	}

	return false;
}
