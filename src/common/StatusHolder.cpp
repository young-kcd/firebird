/*
 *	PROGRAM:		Firebird exceptions classes
 *	MODULE:			StatusHolder.cpp
 *	DESCRIPTION:	Firebird's exception classes
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
 *  The Original Code was created by Vlad Khorsun
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2007 Vlad Khorsun <hvlad at users.sourceforge.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#include "firebird.h"
#include "StatusHolder.h"
#include "gen/iberror.h"
#include "classes/alloc.h"

namespace Firebird {

ISC_STATUS DynamicStatusVector::merge(const IStatus* status)
{
	SimpleStatusVector<> tmp;
	unsigned length = fb_utils::statusLength(status->getErrors());
	length += fb_utils::statusLength(status->getWarnings());
	ISC_STATUS* s = tmp.getBuffer(length + 1);
	fb_utils::mergeStatus(s, length + 1, status);
	return save(s);
}

ISC_STATUS StatusHolder::save(IStatus* status)
{
	fb_assert(isSuccess() || m_raised);
	if (m_raised)
	{
		clear();
	}

	setErrors(status->getErrors());
	setWarnings(status->getWarnings());
	return getErrors()[1];
}

void StatusHolder::clear()
{
	BaseStatus<StatusHolder>::clear();
	m_raised = false;
}

void StatusHolder::raise()
{
	if (getError())
	{
		Arg::StatusVector tmp(getErrors());
		tmp << Arg::StatusVector(getWarnings());
		m_raised = true;
		tmp.raise();
	}
}

unsigned makeDynamicStrings(unsigned length, ISC_STATUS* const dst, const ISC_STATUS* const src)
{
	const ISC_STATUS* end = &src[length];

	// allocate space for strings
	size_t len = 0;
	for (const ISC_STATUS* from = src; from < end; ++from)
	{
		const ISC_STATUS type = *from++;
		if (from >= end || type == isc_arg_end)
		{
			end = from - 1;
			break;
		}

		switch (type)
		{
		case isc_arg_cstring:
			if (from + 1 >= end)
			{
				end = from - 1;
				break;
			}
			len += *from++;
			len++;
			break;

		case isc_arg_string:
		case isc_arg_interpreted:
		case isc_arg_sql_state:
			len += strlen(reinterpret_cast<const char*>(*from));
			len++;
			break;
		}
	}

	char* string = len ? FB_NEW(*getDefaultMemoryPool()) char[len] : NULL;
	ISC_STATUS* to = dst;

	// copy status vector saving strings in local buffer
	for (const ISC_STATUS* from = src; from < end; ++from)
	{
		const ISC_STATUS type = *from++;
		*to++ = type == isc_arg_cstring ? isc_arg_string : type;

		switch (type)
		{
		case isc_arg_cstring:
			fb_assert(string);
			*to++ = (ISC_STATUS)(IPTR) string;
			memcpy(string, reinterpret_cast<const char*>(from[1]), from[0]);
			string += *from++;
			*string++ = 0;
			break;

		case isc_arg_string:
		case isc_arg_interpreted:
		case isc_arg_sql_state:
			fb_assert(string);
			*to++ = (ISC_STATUS)(IPTR) string;
			strcpy(string, reinterpret_cast<const char*>(*from));
			string += strlen(string);
			string++;
			break;

		default:
			*to++ = *from;
			break;
		}
	}

	*to++ = isc_arg_end;
	return (to - dst) - 1;
}

void freeDynamicStrings(unsigned length, ISC_STATUS* ptr)
{
	while (length--)
	{
		const ISC_STATUS type = *ptr++;
		if (type == isc_arg_end)
			return;

		switch (type)
		{
		case isc_arg_cstring:
			fb_assert(false); // CVC: according to the new logic, this case cannot happen
			ptr++;

		case isc_arg_string:
		case isc_arg_interpreted:
		case isc_arg_sql_state:
			delete[] reinterpret_cast<char*>(*ptr++);
			return;

		default:
			ptr++;
			break;
		}
	}
}


} // namespace Firebird
