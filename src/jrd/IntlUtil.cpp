/*
 *	PROGRAM:	JRD International support
 *	MODULE:		IntlUtil.cpp
 *	DESCRIPTION:	INTL Utility functions
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
 *  Copyright (c) 2006 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../jrd/IntlUtil.h"
#include "../jrd/intlobj_new.h"
#include "../jrd/unicode_util.h"


namespace Firebird {


string IntlUtil::generateSpecificAttributes(
	charset* cs, SpecificAttributesMap& map)
{
	bool found = map.getFirst();
	string s;

	while (found)
	{
		UCHAR c[sizeof(ULONG)];
		USHORT errCode;
		ULONG errPos;
		ULONG size;

		SpecificAttribute* attribute = map.current();

		s += escapeAttribute(cs, attribute->first);

		const USHORT equalChar = '=';
		size = cs->charset_from_unicode.csconvert_fn_convert(
			&cs->charset_from_unicode,
			sizeof(equalChar), (const UCHAR*)&equalChar,
			sizeof(c), c, &errCode, &errPos);

		s += string((const char*)&c, size);

		s += escapeAttribute(cs, attribute->second);

		found = map.getNext();

		if (found)
		{
			const USHORT semiColonChar = ';';
			size = cs->charset_from_unicode.csconvert_fn_convert(
				&cs->charset_from_unicode,
				sizeof(semiColonChar), (const UCHAR*)&semiColonChar, sizeof(c),
				c, &errCode, &errPos);

			s += string((const char*)&c, size);
		}
	}

	return s;
}


bool IntlUtil::parseSpecificAttributes(
	charset* cs, ULONG len, const UCHAR* s, SpecificAttributesMap* map)
{
	// Note that the map isn't cleared.
	// Old attributes will be combined with the new ones.

	const UCHAR* p = s;
	const UCHAR* const end = s + len;
	ULONG size = 0;

	readAttributeChar(cs, &p, end, &size, true);

	while (p < end)
	{
		while (p < end && size == cs->charset_space_length &&
			   memcmp(p, cs->charset_space_character, cs->charset_space_length) == 0)
		{
			if (!readAttributeChar(cs, &p, end, &size, true))
				return true;
		}

		const UCHAR* start = p;

		UCHAR uc[sizeof(ULONG)];
		USHORT errCode;
		ULONG errPos;
		ULONG uSize;

		while (p < end)
		{
			uSize = cs->charset_to_unicode.csconvert_fn_convert(
				&cs->charset_to_unicode, size, p, sizeof(uc), uc, &errCode, &errPos);

			if (uSize == INTL_BAD_STR_LENGTH)
				return false;
				
			if (uSize == 2 &&
					 ((*(USHORT*)uc >= 'A' && *(USHORT*)uc <= 'Z') ||
					  (*(USHORT*)uc >= 'a' && *(USHORT*)uc <= 'z') ||
					  *(USHORT*)uc == '-' || *(USHORT*)uc == '_'))
			{
				if (!readAttributeChar(cs, &p, end, &size, true))
					return false;
			}
			else
				break;
		}

		if (p - start == 0)
			return false;

		string name = string((const char*)start, p - start);
		name = unescapeAttribute(cs, name);

		while (p < end && size == cs->charset_space_length &&
			   memcmp(p, cs->charset_space_character, cs->charset_space_length) == 0)
		{
			if (!readAttributeChar(cs, &p, end, &size, true))
				return false;
		}

		uSize = cs->charset_to_unicode.csconvert_fn_convert(
			&cs->charset_to_unicode, size, p, sizeof(uc), uc, &errCode, &errPos);

		if (uSize == INTL_BAD_STR_LENGTH ||
			uSize != 2 ||
			*(USHORT*)uc != '=')
		{
			return false;
		}

		string value;

		if (readAttributeChar(cs, &p, end, &size, true))
		{
			while (p < end && size == cs->charset_space_length &&
				   memcmp(p, cs->charset_space_character, cs->charset_space_length) == 0)
			{
				if (!readAttributeChar(cs, &p, end, &size, true))
					return false;
			}

			const UCHAR* endNoSpace = start = p;

			while (p < end)
			{
				uSize = cs->charset_to_unicode.csconvert_fn_convert(
					&cs->charset_to_unicode, size, p, sizeof(uc), uc, &errCode, &errPos);

				if (uSize == INTL_BAD_STR_LENGTH)
					return false;
					
				if (uSize != 2 || *(USHORT*)uc != ';')
				{
					if (!(size == cs->charset_space_length &&
						  memcmp(p, cs->charset_space_character, cs->charset_space_length) == 0))
					{
						endNoSpace = p + size;
					}

					if (!readAttributeChar(cs, &p, end, &size, true))
						break;
				}
				else
					break;
			}

			value = unescapeAttribute(cs,
				string((const char*)start, endNoSpace - start));

			if (p < end)
				readAttributeChar(cs, &p, end, &size, true);	// skip the semicolon
		}

		if (value.isEmpty())
			map->remove(name);
		else
			map->put(name, value);
	}

	return true;
}


string IntlUtil::escapeAttribute(charset* cs, const string& s)
{
	string ret;
	const UCHAR* p = (const UCHAR*)s.begin();
	const UCHAR* end = (const UCHAR*)s.end();
	ULONG size = 0;

	while (readOneChar(cs, &p, end, &size))
	{
		UCHAR uc[sizeof(ULONG)];
		USHORT errCode;
		ULONG errPos;

		ULONG uSize = cs->charset_to_unicode.csconvert_fn_convert(
			&cs->charset_to_unicode, size, p, sizeof(uc), uc, &errCode, &errPos);

		if (uSize == 2)
		{
			if (*(USHORT*)uc == '\\' || *(USHORT*)uc == '=' || *(USHORT*)uc == ';')
			{
				*(USHORT*)uc = '\\';
				UCHAR bytes[sizeof(ULONG)];
				
				ULONG bytesSize = cs->charset_from_unicode.csconvert_fn_convert(
					&cs->charset_from_unicode, sizeof(USHORT), uc,
					sizeof(bytes), bytes, &errCode, &errPos);

				ret.append(string((const char*)bytes, bytesSize));
			}
		}

		ret.append(string((const char*)p, size));
	}

	return ret;
}


string IntlUtil::unescapeAttribute(charset* cs, const string& s)
{
	string ret;
	const UCHAR* p = (const UCHAR*)s.begin();
	const UCHAR* end = (const UCHAR*)s.end();
	ULONG size = 0;

	while (readAttributeChar(cs, &p, end, &size, false))
		ret.append(string((const char*)p, size));

	return ret;
}


bool IntlUtil::isAttributeEscape(charset* cs, const UCHAR* s, ULONG size)
{
	UCHAR uc[sizeof(ULONG)];
	USHORT errCode;
	ULONG errPos;

	ULONG uSize = cs->charset_to_unicode.csconvert_fn_convert(
		&cs->charset_to_unicode, size, s, sizeof(uc), uc, &errCode, &errPos);

	if (uSize == 2 && *(USHORT*)uc == '\\')
		return true;

	return false;
}


bool IntlUtil::readAttributeChar(charset* cs, const UCHAR** s, const UCHAR* end, ULONG* size, bool returnEscape)
{
	if (readOneChar(cs, s, end, size))
	{
		if (isAttributeEscape(cs, *s, *size))
		{
			const UCHAR* p = *s;
			ULONG firstSize = *size;

			if (readOneChar(cs, s, end, size))
			{
				if (returnEscape)
				{
					*s = p;
					*size += firstSize;
				}
			}
			else
				return false;
		}

		return true;
	}

	return false;
}


bool IntlUtil::readOneChar(charset* cs, const UCHAR** s, const UCHAR* end, ULONG* size)
{
	(*s) += *size;

	if (*s >= end)
	{
		(*s) = end;
		*size = 0;
		return false;
	}

	UCHAR c[sizeof(ULONG)];

	if (cs->charset_fn_substring)
		*size = cs->charset_fn_substring(cs, end - *s, *s, sizeof(c), c, 0, 1);
	else if (cs->charset_min_bytes_per_char == cs->charset_max_bytes_per_char)
		*size = cs->charset_min_bytes_per_char;
	else
	{
		UCHAR uc[sizeof(ULONG)];
		USHORT errCode;
		ULONG errPos;
		ULONG uSize;

		ULONG n = cs->charset_min_bytes_per_char;

		while ((uSize = cs->charset_to_unicode.csconvert_fn_convert(
			&cs->charset_to_unicode, n, *s, sizeof(uc), uc, &errCode, &errPos)) == 0)
		{
			++n;
			if (n > cs->charset_max_bytes_per_char)
				return false;
		}

		*size = n;
	}

	return true;
}


}	// namespace Firebird
