/*
 *	PROGRAM:	JRD International support
 *	MODULE:		intl_classes.h
 *	DESCRIPTION:	International text handling definitions
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
 *  The Original Code was created by Nickolay Samofatov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Nickolay Samofatov <nickolay@broadviewsoftware.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 */

#ifndef JRD_INTL_CLASSES_H
#define JRD_INTL_CLASSES_H

#include "firebird.h"
#include "../jrd/jrd.h"
#include "../jrd/intlobj_new.h"
#include "../jrd/constants.h"
#include "../jrd/unicode_util.h"

typedef SSHORT CHARSET_ID;
typedef SSHORT COLLATE_ID;
typedef USHORT TTYPE_ID;

namespace Jrd {

class LikeObject {
public:
	virtual void reset() = 0;
	virtual bool process(thread_db*, TextType*, const UCHAR*, SLONG) = 0;
	virtual bool result() = 0;
	virtual ~LikeObject() {}
};

class ContainsObject {
public:
	virtual void reset() = 0;
	virtual bool process(Jrd::thread_db*, Jrd::TextType*, const UCHAR*, SLONG) = 0;
	virtual bool result() = 0;
	virtual ~ContainsObject() {}
};

class CsConvert
{ 
public:
	CsConvert(csconvert* _cnvt) : cnvt(_cnvt) {}
	CsConvert(const CsConvert& obj) : cnvt(obj.cnvt) {}

	// CVC: Beware of this can of worms: csconvert_convert gets assigned
	// different functions that not necessarily take the same argument. Typically,
	// the src pointer and the dest pointer use different types.
	// How does this work without crashing is a miracle of IT.

	// To be used with getConvFromUnicode method of CharSet class
	ULONG convert(ULONG a,
				  const USHORT* b,
				  ULONG c,
				  UCHAR* d,
				  USHORT* e,
				  ULONG* f)
	{
		fb_assert(cnvt != NULL);
		return (*cnvt->csconvert_fn_convert)(cnvt, a, reinterpret_cast<const UCHAR*>(b), c, d, e, f);
	}

	// To be used with getConvToUnicode method of CharSet class
	ULONG convert(ULONG a,
				  const UCHAR* b,
				  ULONG c,
				  USHORT* d,
				  USHORT* e,
				  ULONG* f)
	{
		fb_assert(cnvt != NULL);
		return (*cnvt->csconvert_fn_convert)(cnvt, a, b, c, reinterpret_cast<UCHAR*>(d), e, f);
	}

	// To be used for arbitrary conversions
	ULONG convert(ULONG a,
				  const UCHAR* b,
				  ULONG c,
				  UCHAR* d,
				  USHORT* e,
				  ULONG* f)
	{
		fb_assert(cnvt != NULL);
		return (*cnvt->csconvert_fn_convert)(cnvt, a, b, c, d, e, f);
	}

	// To be used for measure length of conversion
	ULONG convertLength(ULONG srcLen)
	{
		fb_assert(cnvt != NULL);

		USHORT errCode;
		ULONG errPos;
		return (*cnvt->csconvert_fn_convert)(cnvt, srcLen, NULL, 0, NULL, &errCode, &errPos);
	}

	const char* getName() const { fb_assert(cnvt); return cnvt->csconvert_name; }

	csconvert* getStruct() const { return cnvt; }

private:
	csconvert* cnvt;
};

static inline bool operator ==(const CsConvert& cv1, const CsConvert& cv2)
{
	return cv1.getStruct() == cv2.getStruct();
}

static inline bool operator !=(const CsConvert& cv1, const CsConvert& cv2)
{
	return cv1.getStruct() != cv2.getStruct();
}

class CharSet
{
public:
	CharSet(CHARSET_ID _id, charset* _cs)
		: id(_id), cs(_cs)
	{
		USHORT err_code;
		ULONG err_pos;

		sqlMatchAnyLength = getConvFromUnicode().convert(sizeof(SQL_MATCH_ANY_CHARS), &SQL_MATCH_ANY_CHARS, sizeof(sqlMatchAny), sqlMatchAny, &err_code, &err_pos);
		sqlMatchOneLength = getConvFromUnicode().convert(sizeof(SQL_MATCH_1_CHAR), &SQL_MATCH_1_CHAR, sizeof(sqlMatchOne), sqlMatchOne, &err_code, &err_pos);
	}

private:
	CharSet(const CharSet&) {}

public:
	virtual ~CharSet() {}

	CHARSET_ID getId() const { fb_assert(cs); return id; }
	const char* getName() const { fb_assert(cs); return cs->charset_name; }
	UCHAR minBytesPerChar() const { fb_assert(cs); return cs->charset_min_bytes_per_char; }
	UCHAR maxBytesPerChar() const { fb_assert(cs); return cs->charset_max_bytes_per_char; }
	UCHAR getSpaceLength() const { fb_assert(cs); return cs->charset_space_length; }
	const UCHAR* getSpace() const { fb_assert(cs); return cs->charset_space_character; }
	USHORT getFlags() const { fb_assert(cs); return cs->charset_flags; }

	bool isMultiByte() const
	{
		fb_assert(cs);
		return cs->charset_min_bytes_per_char != cs->charset_max_bytes_per_char;
	}

	bool wellFormed(ULONG len, const UCHAR* str, ULONG* offendingPos = NULL) const
	{
		ULONG offendingPos2;

		if (offendingPos == NULL)
			offendingPos = &offendingPos2;

		fb_assert(cs);
		if (cs->charset_fn_well_formed)
			return cs->charset_fn_well_formed(cs, len, str, offendingPos);
		else
			return true;
	}

	CsConvert getConvToUnicode() const { fb_assert(cs); return &cs->charset_to_unicode; }
	CsConvert getConvFromUnicode() const { fb_assert(cs); return &cs->charset_from_unicode; }

	void destroy()
	{
		fb_assert(cs);

		if (cs->charset_fn_destroy)
			cs->charset_fn_destroy(cs);
	}

	const UCHAR* getSqlMatchAny() const { return sqlMatchAny; }
	const UCHAR* getSqlMatchOne() const { return sqlMatchOne; }
	const BYTE getSqlMatchAnyLength() const { return sqlMatchAnyLength; }
	const BYTE getSqlMatchOneLength() const { return sqlMatchOneLength; }

	charset* getStruct() const { return cs; }

	ULONG removeTrailingSpaces(ULONG srcLen, const UCHAR* src) const
	{
		const UCHAR* p = src + srcLen - getSpaceLength();

		while (p >= src && memcmp(p, getSpace(), getSpaceLength()) == 0)
			p -= getSpaceLength();

		p += getSpaceLength();

		return p - src;
	}

	virtual ULONG length(thread_db* tdbb, ULONG srcLen, const UCHAR* src, bool countTrailingSpaces) const = 0;
	virtual ULONG substring(thread_db* tdbb, ULONG srcLen, const UCHAR* src, ULONG dstLen, UCHAR* dst, ULONG startPos, ULONG length) const = 0;

private:
	CHARSET_ID id;
	UCHAR sqlMatchAny[sizeof(ULONG)];
	UCHAR sqlMatchOne[sizeof(ULONG)];
	BYTE sqlMatchAnyLength;
	BYTE sqlMatchOneLength;
	charset* cs;
};

class TextType
{
public:
	TextType(TTYPE_ID _type, texttype *_tt, CharSet* _cs)
		: type(_type), tt(_tt), cs(_cs)
	{
		canonical(cs->getSqlMatchAnyLength(), cs->getSqlMatchAny(), sizeof(sqlMatchAnyCanonic), sqlMatchAnyCanonic);
		canonical(cs->getSqlMatchOneLength(), cs->getSqlMatchOne(), sizeof(sqlMatchOneCanonic), sqlMatchOneCanonic);

		struct Conversion
		{
			USHORT ch;
			UCHAR* ptr;
		};

		Conversion conversions[] =
			{
				{GDML_MATCH_ONE, gdmlMatchOneCanonic},
				{GDML_MATCH_ANY, gdmlMatchAnyCanonic},
				{GDML_QUOTE, gdmlQuoteCanonic},
				{GDML_NOT, gdmlNotCanonic},
				{GDML_RANGE, gdmlRangeCanonic},
				{GDML_CLASS_START, gdmlClassStartCanonic},
				{GDML_CLASS_END, gdmlClassEndCanonic},
				{GDML_SUBSTITUTE, gdmlSubstituteCanonic},
				{GDML_FLAG_SET, gdmlFlagSetCanonic},
				{GDML_FLAG_CLEAR, gdmlFlagClearCanonic},
				{GDML_COMMA, gdmlCommaCanonic},
				{GDML_LPAREN, gdmlLParenCanonic},
				{GDML_RPAREN, gdmlRParenCanonic},
				{'S', gdmlUpperSCanonic},
				{'s', gdmlLowerSCanonic}
			};

		for (int i = 0; i < FB_NELEM(conversions); i++)
		{
			USHORT err_code;
			ULONG err_position;
			UCHAR temp[sizeof(ULONG)];

			ULONG length = getCharSet()->getConvFromUnicode().convert(sizeof(USHORT), &conversions[i].ch, sizeof(temp), temp, &err_code, &err_position);
			canonical(length, temp, sizeof(ULONG), conversions[i].ptr);
		}
	}

private:
	TextType(const TextType&) {}

public:
	virtual ~TextType() {}

	USHORT key_length(USHORT len) {
		fb_assert(tt);
		if (tt->texttype_fn_key_length)
			return (*tt->texttype_fn_key_length)(tt, len);
		else
		{
			if (getCharSet()->isMultiByte())
				return UnicodeUtil::utf16KeyLength(len);
			else
				return len;
		}
	}

	USHORT string_to_key(USHORT srcLen,
						 const UCHAR* src,
						 USHORT dstLen,
						 UCHAR* dst,
						 USHORT key_type)
	{
		fb_assert(tt);
		if (tt->texttype_fn_string_to_key)
			return (*tt->texttype_fn_string_to_key)(tt, srcLen, src, dstLen, dst, key_type);
		else
		{
			const UCHAR* space = getCharSet()->getSpace();
			BYTE spaceLength = getCharSet()->getSpaceLength();
			Firebird::HalfStaticArray<UCHAR, BUFFER_SMALL> utf16Str;
			UCHAR utf16Space[sizeof(ULONG)];

			if (getCharSet()->isMultiByte())
			{
				USHORT err_code;
				ULONG err_position;

				// convert src to UTF-16
				ULONG utf16Length = getCharSet()->getConvToUnicode().convertLength(srcLen);

				srcLen = getCharSet()->getConvToUnicode().convert(srcLen, src,
							utf16Length, utf16Str.getBuffer(utf16Length), &err_code, &err_position);
				src = utf16Str.begin();

				// convert charset space to UTF-16
				spaceLength = getCharSet()->getConvToUnicode().convert(spaceLength, space,
							sizeof(utf16Space), utf16Space, &err_code, &err_position);
				fb_assert(spaceLength == 2);	// space character can't be surrogate for default string_to_key
				space = utf16Space;
			}

			if (tt->texttype_pad_option)
			{
				const UCHAR* pad;

				for (pad = src + srcLen - spaceLength; pad >= src; pad -= spaceLength)
				{
					if (memcmp(pad, space, spaceLength) != 0)
						break;
				}

				srcLen = pad - src + spaceLength;
			}

			if (getCharSet()->isMultiByte())
			{
				dstLen = UnicodeUtil::utf16ToKey(srcLen, reinterpret_cast<const USHORT*>(src), 
												 dstLen, dst, key_type);
			}
			else
			{
				if (dstLen >= srcLen)
				{
					memcpy(dst, src, srcLen);
					dstLen = srcLen;
				}
				else
					dstLen = INTL_BAD_KEY_LENGTH;
			}

			return dstLen;
		}
	}

	SSHORT compare(ULONG len1,
				   const UCHAR* str1,
				   ULONG len2,
				   const UCHAR* str2)
	{
		INTL_BOOL error = false;
		fb_assert(tt);
		if (tt->texttype_fn_compare)
			return (*tt->texttype_fn_compare)(tt, len1, str1, len2, str2, &error);
		else
		{
			const UCHAR* space = getCharSet()->getSpace();
			BYTE spaceLength = getCharSet()->getSpaceLength();
			Firebird::HalfStaticArray<UCHAR, BUFFER_SMALL> utf16Str1;
			Firebird::HalfStaticArray<UCHAR, BUFFER_SMALL> utf16Str2;
			UCHAR utf16Space[sizeof(ULONG)];

			if (getCharSet()->isMultiByte())
			{
				USHORT err_code;
				ULONG err_position;

				// convert str1 to UTF-16
				ULONG utf16Length = getCharSet()->getConvToUnicode().convertLength(len1);

				len1 = getCharSet()->getConvToUnicode().convert(len1, str1,
							utf16Length, utf16Str1.getBuffer(utf16Length), &err_code, &err_position);
				str1 = utf16Str1.begin();

				// convert str2 to UTF-16
				utf16Length = getCharSet()->getConvToUnicode().convertLength(len2);

				len2 = getCharSet()->getConvToUnicode().convert(len2, str2,
							utf16Length, utf16Str2.getBuffer(utf16Length), &err_code, &err_position);
				str2 = utf16Str2.begin();

				// convert charset space to UTF-16
				spaceLength = getCharSet()->getConvToUnicode().convert(spaceLength, space,
							sizeof(utf16Space), utf16Space, &err_code, &err_position);
				fb_assert(spaceLength == 2);	// space character can't be surrogate for default compare
				space = utf16Space;
			}

			if (tt->texttype_pad_option)
			{
				const UCHAR* pad;

				for (pad = str1 + len1 - spaceLength; pad >= str1; pad -= spaceLength)
				{
					if (memcmp(pad, space, spaceLength) != 0)
						break;
				}

				len1 = pad - str1 + spaceLength;

				for (pad = str2 + len2 - spaceLength; pad >= str2; pad -= spaceLength)
				{
					if (memcmp(pad, space, spaceLength) != 0)
						break;
				}

				len2 = pad - str2 + spaceLength;
			}

			if (getCharSet()->isMultiByte())
			{
				INTL_BOOL error_flag;
				return UnicodeUtil::utf16Compare(len1, reinterpret_cast<const USHORT*>(str1), 
												 len2, reinterpret_cast<const USHORT*>(str2), &error_flag);
			}
			else
			{
				SSHORT cmp = memcmp(str1, str2, MIN(len1, len2));

				if (cmp == 0)
					cmp = (len1 < len2 ? -1 : (len1 > len2 ? 1 : 0));

				return cmp;
			}
		}
	}

	ULONG str_to_upper(ULONG srcLen,
					   const UCHAR* src,
					   ULONG dstLen,
					   UCHAR* dst)
	{
		fb_assert(tt);
		if (tt->texttype_fn_str_to_upper)
			return (*tt->texttype_fn_str_to_upper)(tt, srcLen, src, dstLen, dst);
		else
		{
			USHORT err_code;
			ULONG err_position;

			ULONG utf16_length = getCharSet()->getConvToUnicode().convertLength(srcLen);
			Firebird::HalfStaticArray<UCHAR, BUFFER_SMALL> utf16_str;
			UCHAR* utf16_ptr;

			if (dstLen >= utf16_length)	// if dst buffer is sufficient large, use it as intermediate
				utf16_ptr = dst;
			else
				utf16_ptr = utf16_str.getBuffer(utf16_length);

			// convert to UTF-16
			srcLen = getCharSet()->getConvToUnicode().convert(srcLen, src,
						utf16_length, utf16_ptr, &err_code, &err_position);

			// convert to uppercase
			Firebird::HalfStaticArray<UCHAR, BUFFER_SMALL> upper_str;
			srcLen = UnicodeUtil::utf16UpperCase(srcLen, reinterpret_cast<USHORT*>(utf16_ptr),
						utf16_length, reinterpret_cast<USHORT*>(upper_str.getBuffer(utf16_length)));

			// convert to original character set
			return getCharSet()->getConvFromUnicode().convert(srcLen, upper_str.begin(),
						dstLen, dst, &err_code, &err_position);
		}
	}

	ULONG str_to_lower(ULONG srcLen,
					   const UCHAR* src,
					   ULONG dstLen,
					   UCHAR* dst)
	{
		fb_assert(tt);
		if (tt->texttype_fn_str_to_lower)
			return (*tt->texttype_fn_str_to_lower)(tt, srcLen, src, dstLen, dst);
		else
		{
			USHORT err_code;
			ULONG err_position;

			ULONG utf16_length = getCharSet()->getConvToUnicode().convertLength(srcLen);
			Firebird::HalfStaticArray<UCHAR, BUFFER_SMALL> utf16_str;
			UCHAR* utf16_ptr;

			if (dstLen >= utf16_length)	// if dst buffer is sufficient large, use it as intermediate
				utf16_ptr = dst;
			else
				utf16_ptr = utf16_str.getBuffer(utf16_length);

			// convert to UTF-16
			srcLen = getCharSet()->getConvToUnicode().convert(srcLen, src,
						utf16_length, utf16_ptr, &err_code, &err_position);

			// convert to lowercase
			Firebird::HalfStaticArray<UCHAR, BUFFER_SMALL> lower_str;
			srcLen = UnicodeUtil::utf16LowerCase(srcLen, reinterpret_cast<USHORT*>(utf16_ptr),
						utf16_length, reinterpret_cast<USHORT*>(lower_str.getBuffer(utf16_length)));

			// convert to original character set
			return getCharSet()->getConvFromUnicode().convert(srcLen, lower_str.begin(),
						dstLen, dst, &err_code, &err_position);
		}
	}

	ULONG canonical(ULONG srcLen,
					const UCHAR* src,
					ULONG dstLen,
					UCHAR* dst)
	{
		fb_assert(tt);

		if (tt->texttype_fn_canonical)
			return (*tt->texttype_fn_canonical)(tt, srcLen, src, dstLen, dst);
		else if (getCharSet()->isMultiByte())
		{
			fb_assert(tt->texttype_canonical_width == sizeof(ULONG));

			USHORT err_code;
			ULONG err_position;
			Firebird::HalfStaticArray<UCHAR, BUFFER_SMALL> utf16_str;

			ULONG utf16_len = getCharSet()->getConvToUnicode().convertLength(srcLen);

			// convert to UTF-16
			utf16_len = getCharSet()->getConvToUnicode().convert(srcLen, src,
				utf16_len, utf16_str.getBuffer(utf16_len), &err_code, &err_position);

			// convert UTF-16 to UTF-32
			return UnicodeUtil::utf16ToUtf32(utf16_len, reinterpret_cast<const USHORT*>(utf16_str.begin()),
				dstLen, reinterpret_cast<ULONG*>(dst), &err_code, &err_position) / sizeof(ULONG);
		}
		else
		{
			fb_assert(tt->texttype_canonical_width == getCharSet()->minBytesPerChar());
			fb_assert(dstLen >= srcLen);

			memcpy(dst, src, srcLen);

			return srcLen / getCharSet()->minBytesPerChar();
		}
	}

	const UCHAR* getSqlMatchAnyCanonic() const { return sqlMatchAnyCanonic; }
	const UCHAR* getSqlMatchOneCanonic() const { return sqlMatchOneCanonic; }
	const UCHAR* getGdmlMatchOneCanonic() const { return gdmlMatchOneCanonic; }
	const UCHAR* getGdmlMatchAnyCanonic() const { return gdmlMatchAnyCanonic; }
	const UCHAR* getGdmlQuoteCanonic() const { return gdmlQuoteCanonic; }
	const UCHAR* getGdmlNotCanonic() const { return gdmlNotCanonic; }
	const UCHAR* getGdmlRangeCanonic() const { return gdmlRangeCanonic; }
	const UCHAR* getGdmlClassStartCanonic() const { return gdmlClassStartCanonic; }
	const UCHAR* getGdmlClassEndCanonic() const { return gdmlClassEndCanonic; }
	const UCHAR* getGdmlSubstituteCanonic() const { return gdmlSubstituteCanonic; }
	const UCHAR* getGdmlFlagSetCanonic() const { return gdmlFlagSetCanonic; }
	const UCHAR* getGdmlFlagClearCanonic() const { return gdmlFlagClearCanonic; }
	const UCHAR* getGdmlCommaCanonic() const { return gdmlCommaCanonic; }
	const UCHAR* getGdmlLParenCanonic() const { return gdmlLParenCanonic; }
	const UCHAR* getGdmlRParenCanonic() const { return gdmlRParenCanonic; }
	const UCHAR* getGdmlUpperSCanonic() const { return gdmlUpperSCanonic; }
	const UCHAR* getGdmlLowerSCanonic() const { return gdmlLowerSCanonic; }

	USHORT getType() const
	{
		return type;
	}

	CharSet* TextType::getCharSet() const
	{
		return cs;
	}

	BYTE getCanonicalWidth() const
	{ 
		fb_assert(tt);
		return tt->texttype_canonical_width;
	}

	USHORT getFlags() const
	{ 
		fb_assert(tt);
		return tt->texttype_flags;
	}

	void destroy()
	{
		fb_assert(tt);
		if (tt->texttype_fn_destroy)
			tt->texttype_fn_destroy(tt);
	}

	virtual bool matches(thread_db* tdbb, const UCHAR* a, SLONG b, const UCHAR* c, SLONG d) = 0;

	virtual bool sleuth_check(thread_db* tdbb, USHORT a, const UCHAR* b, SLONG c, const UCHAR* d, SLONG e) = 0;
	virtual ULONG sleuth_merge(thread_db* tdbb, const UCHAR* a, SLONG b, const UCHAR* c, SLONG d, UCHAR* e, SLONG f) = 0;

	virtual bool like(thread_db* tdbb, const UCHAR* s, SLONG sl, const UCHAR* p, SLONG pl, const UCHAR* escape, SLONG escape_length) = 0;
	virtual LikeObject *like_create(thread_db* tdbb, const UCHAR* p, SLONG pl, const UCHAR* escape, SLONG escape_length) = 0;

	virtual bool contains(thread_db* tdbb, const UCHAR* s, SLONG sl, const UCHAR* p, SLONG pl) = 0;
	virtual ContainsObject *contains_create(thread_db* tdbb, const UCHAR* p, SLONG pl) = 0;

private:
	TTYPE_ID type;
	texttype* tt;
	CharSet* cs;
	UCHAR sqlMatchAnyCanonic[sizeof(ULONG)];
	UCHAR sqlMatchOneCanonic[sizeof(ULONG)];
	UCHAR gdmlMatchOneCanonic[sizeof(ULONG)];
	UCHAR gdmlMatchAnyCanonic[sizeof(ULONG)];
	UCHAR gdmlQuoteCanonic[sizeof(ULONG)];
	UCHAR gdmlNotCanonic[sizeof(ULONG)];
	UCHAR gdmlRangeCanonic[sizeof(ULONG)];
	UCHAR gdmlClassStartCanonic[sizeof(ULONG)];
	UCHAR gdmlClassEndCanonic[sizeof(ULONG)];
	UCHAR gdmlSubstituteCanonic[sizeof(ULONG)];
	UCHAR gdmlFlagSetCanonic[sizeof(ULONG)];
	UCHAR gdmlFlagClearCanonic[sizeof(ULONG)];
	UCHAR gdmlCommaCanonic[sizeof(ULONG)];
	UCHAR gdmlLParenCanonic[sizeof(ULONG)];
	UCHAR gdmlRParenCanonic[sizeof(ULONG)];
	UCHAR gdmlUpperSCanonic[sizeof(ULONG)];
	UCHAR gdmlLowerSCanonic[sizeof(ULONG)];
};

} //namespace Jrd

#endif /* JRD_INTL_CLASSES_H */
