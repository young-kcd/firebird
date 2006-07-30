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
 *  Copyright (c) 2004 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
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


Firebird::string IntlUtil::generateSpecificAttributes(
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

		s += attribute->first;

		const USHORT equalChar = '=';
		size = cs->charset_from_unicode.csconvert_fn_convert(
			&cs->charset_from_unicode,
			sizeof(equalChar), (const UCHAR*)&equalChar,
			sizeof(c), c, &errCode, &errPos);

		s += string((const char*)&c, size);

		s += attribute->second;

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
	const UCHAR* end = s + len;
	ULONG size = 0;

	readChar(cs, &p, end, &size);

	while (p < end)
	{
		while (p < end && size == cs->charset_space_length &&
			   memcmp(p, cs->charset_space_character, cs->charset_space_length) == 0)
		{
			if (!readChar(cs, &p, end, &size))
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
			else if (uSize == 2 &&
					 ((*(USHORT*)uc >= 'A' && *(USHORT*)uc <= 'Z') ||
					  (*(USHORT*)uc >= 'a' && *(USHORT*)uc <= 'z') ||
					  *(USHORT*)uc == '-' || *(USHORT*)uc == '_'))
			{
				if (!readChar(cs, &p, end, &size))
					return false;
			}
			else
				break;
		}

		if (p - start == 0)
			return false;

		string name = string((const char*)start, p - start);
		name.trim();

		while (p < end && size == cs->charset_space_length &&
			   memcmp(p, cs->charset_space_character, cs->charset_space_length) == 0)
		{
			if (!readChar(cs, &p, end, &size))
				return false;
		}

		uSize = cs->charset_to_unicode.csconvert_fn_convert(
			&cs->charset_to_unicode, size, p, sizeof(uc), uc, &errCode, &errPos);

		if (uSize == INTL_BAD_STR_LENGTH ||
			uSize != 2 ||
			*(USHORT*)uc != '=' ||
			!readChar(cs, &p, end, &size))
		{
			return false;
		}

		start = p;

		while (p < end)
		{
			uSize = cs->charset_to_unicode.csconvert_fn_convert(
				&cs->charset_to_unicode, size, p, sizeof(uc), uc, &errCode, &errPos);

			if (uSize == INTL_BAD_STR_LENGTH)
				return false;
			else if (uSize != 2 || *(USHORT*)uc != ';')
			{
				if (!readChar(cs, &p, end, &size))
					break;
			}
			else
				break;
		}

		string value = string((const char*)start, p - start);
		value.trim();

		if (p < end)
			readChar(cs, &p, end, &size);	// skip the semicolon

		map->put(name, value);
	}

	return true;
}


bool IntlUtil::readChar(charset* cs, const UCHAR** s, const UCHAR* end, ULONG* size)
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
