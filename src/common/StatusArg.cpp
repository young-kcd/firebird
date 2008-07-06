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
	m_status_vector[0] = isc_arg_end;
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

void StatusVector::raise() const
{
	if (hasData())
	{
		status_exception::raise(*this);
	}
	status_exception::raise(Gds(isc_random) <<
		Str("Attempt to raise empty exception"));
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
