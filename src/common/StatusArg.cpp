/*
 *	PROGRAM:		Firebird exceptions classes
 *	MODULE:			StatusArg.cpp
 *	DESCRIPTION:	Build status vector with variable number of elements
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
 *  The Original Code was created by Alex Peshkov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2008 Alex Peshkov <peshkoff at mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#include "firebird.h"
#include "../common/StatusArg.h"

#include "../common/classes/fb_string.h"
#include "../common/classes/MetaName.h"
#include "fb_exception.h"
#include "gen/iberror.h"

namespace Firebird {

namespace Arg {


void StatusVector::clear() throw()
{
	m_length = 0;
	m_warning = 0;
	m_status_vector[0] = isc_arg_end;
}

StatusVector::StatusVector(const ISC_STATUS* s) throw() : Base(0, 0)
{
	clear();
	// special case - empty initialized status vector, no warnings
	if (s[0] != isc_arg_gds || s[1] != 0 || s[2] != 0)
	{
		append(s, FB_NELEM(m_status_vector) - 1);
	}
}

void StatusVector::append(const StatusVector& v) throw()
{
	StatusVector newVector;

	if (newVector.appendErrors(this))
	{
		if (newVector.appendErrors(&v))
		{
			if (newVector.appendWarnings(this))
				newVector.appendWarnings(&v);
		}
	}
	
	*this = newVector;
}

bool StatusVector::appendErrors(const StatusVector* v) throw()
{
	return append(v->m_status_vector, v->m_warning ? v->m_warning : v->m_length);
}	

bool StatusVector::appendWarnings(const StatusVector* v) throw()
{
	if (! v->m_warning)
		return true;
	return append(v->m_status_vector + v->m_warning, v->m_length - v->m_warning);
}	

bool StatusVector::append(const ISC_STATUS* from, const int count) throw()
{
	int copied = 0;

	for (int i = 0; i < count; )
	{
		if (from[i] == isc_arg_end)
		{
			break;
		}
		i += (from[i] == isc_arg_cstring ? 3 : 2);
		if (m_length + i > FB_NELEM(m_status_vector) - 1)
		{
			break;
		}
		copied = i;
	}

	memcpy(&m_status_vector[m_length], from, copied * sizeof(m_status_vector[0]));
	m_length += copied;
	m_status_vector[m_length] = isc_arg_end;

	return copied == count;
}

StatusVector& StatusVector::operator<<(const Base& arg) throw()
{
	if (m_length < FB_NELEM(m_status_vector) - 2)
	{
		m_status_vector[m_length++] = arg.kind;
		m_status_vector[m_length++] = arg.value;
		m_status_vector[m_length] = isc_arg_end;
	}
	return *this;
}

StatusVector& StatusVector::operator<<(const Warning& arg) throw()
{
	const int cur = m_warning ? 0 : m_length;
	operator<<(*static_cast<const Base*>(&arg));
	if (cur && m_status_vector[cur] == isc_arg_warning)
		m_warning = cur;
	return *this;
}

StatusVector& StatusVector::operator<<(const char* text) throw()
{
	operator<<(Str(text));
	return *this;
}

StatusVector& StatusVector::operator<<(const AbstractString& text) throw()
{
	operator<<(Str(text));
	return *this;
}

StatusVector& StatusVector::operator<<(const MetaName& text) throw()
{
	operator<<(Str(text));
	return *this;
}

void StatusVector::raise() const
{
	if (hasData())
	{
		status_exception::raise(*this);
	}
	status_exception::raise(Gds(isc_random) <<
		Str("Attempt to raise empty exception"));
}

ISC_STATUS StatusVector::copyTo(ISC_STATUS* dest) const throw()
{
	if (hasData())
	{
		memcpy(dest, value(), (length() + 1) * sizeof(ISC_STATUS));
	}
	else {
		dest[0] = isc_arg_gds;
		dest[1] = FB_SUCCESS;
		dest[2] = isc_arg_end;
	}
	return dest[1];
}

Gds::Gds(ISC_STATUS s) throw() : 
	StatusVector(isc_arg_gds, s) { }

Num::Num(ISC_STATUS s) throw() : 
	Base(isc_arg_number, s) { }

Interpreted::Interpreted(const char* text) throw() : 
	StatusVector(isc_arg_interpreted, (ISC_STATUS)(IPTR) text) { }

Interpreted::Interpreted(const AbstractString& text) throw() : 
	StatusVector(isc_arg_interpreted, (ISC_STATUS)(IPTR) text.c_str()) { }

Unix::Unix(ISC_STATUS s) throw() : 
	Base(isc_arg_unix, s) { }

Windows::Windows(ISC_STATUS s) throw() : 
	Base(isc_arg_win32, s) { }

Warning::Warning(ISC_STATUS s) throw() : 
	StatusVector(isc_arg_warning, s) { }

Str::Str(const char* text) throw() : 
	Base(isc_arg_string, (ISC_STATUS)(IPTR) text) { }

Str::Str(const AbstractString& text) throw() : 
	Base(isc_arg_string, (ISC_STATUS)(IPTR) text.c_str()) { }

Str::Str(const MetaName& text) throw() : 
	Base(isc_arg_string, (ISC_STATUS)(IPTR) text.c_str()) { }

SqlState::SqlState(const char* text) throw() : 
	Base(isc_arg_sql_state, (ISC_STATUS)(IPTR) text) { }

SqlState::SqlState(const AbstractString& text) throw() : 
	Base(isc_arg_sql_state, (ISC_STATUS)(IPTR) text.c_str()) { }


} // namespace Arg

} // namespace Firebird
