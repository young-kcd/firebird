/*
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
 *  for the Firebird Open Source RDBMS project, based on previous work done
 *  by Eugeney Putilin <evgeneyputilin at mail.ru>,
 *  Vlad Khorsun <hvlad at users.sourceforge.net> and
 *  Roman Rokytskyy <roman at rokytskyy.de>.
 *
 *  Copyright (c) 2008 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *  Eugeney Putilin <evgeneyputilin at mail.ru>
 *  Vlad Khorsun <hvlad at users.sourceforge.net>
 *  Roman Rokytskyy <roman at rokytskyy.de>
 */

#include "firebird.h"
#include "../jrd/ErrorImpl.h"
#include "../jrd/err_proto.h"
#include "../common/classes/alloc.h"

using namespace Firebird;
using Firebird::uint;

namespace Jrd {


ErrorImpl::~ErrorImpl()
{
	for (ISC_STATUS* p = status; p <= next; p++)
	{
		if (*p == isc_arg_cstring)
		{
			++p;
			char* s = (char*) (*++p);
			delete[] s;
		}
	}
}


void ErrorImpl::statusVectorToError(const ISC_STATUS* vector, Error* error)
{
	while (*vector != isc_arg_end)
	{
		switch (*vector)
		{
			case isc_arg_warning:
			case isc_arg_gds:
			case isc_arg_number:
			case isc_arg_interpreted:
			case isc_arg_vms:
			case isc_arg_unix:
			case isc_arg_win32:
				error->addCode(*vector++);
				error->addCode(*vector++);
				break;

			case isc_arg_string:
				error->addString((const char*) vector[1], strlen((const char*) vector[1]));
				vector += 2;
				break;

			case isc_arg_cstring:
				error->addString((const char*) vector[2], vector[1]);
				vector += 3;
				break;

			default:
				fb_assert(false);
				return;
		}
	}
}


void ErrorImpl::exceptionToError(const Firebird::Exception& ex, Error* error)
{
	ISC_STATUS_ARRAY statusVector;
	ex.stuff_exception(statusVector);
	statusVectorToError(statusVector, error);
}


bool FB_CALL ErrorImpl::addCode(int32 code)
{
	if (next - status >= ISC_STATUS_LENGTH - 1)
		return false;

	*next++ = code;
	*next = isc_arg_end;

	return true;
}


bool FB_CALL ErrorImpl::addString(const char* str, uint strLength)
{
	//// TODO: transliteration

	if (next - status < ISC_STATUS_LENGTH - 3)
	{
		*next++ = isc_arg_cstring;

		MemoryPool* pool = getDefaultMemoryPool();
		char* s = FB_NEW(*pool) char[strLength + 1];

		memcpy(s, str, strLength);
		s[strLength] = 0;

		*next++ = (ISC_STATUS) strLength;
		*next++ = (ISC_STATUS)(IPTR) s;
		*next = isc_arg_end;
	}

	return true;
}


//---------------------


RaiseError::~RaiseError()
{
	if (next == status)
		return;

	Arg::StatusVector newStatusVector;

	if (*status != isc_arg_gds)
		newStatusVector << Arg::Gds(isc_random);

	newStatusVector.append(Arg::StatusVector(status));

	Firebird::status_exception::raise(newStatusVector);
}


//---------------------


LogError::~LogError()
{
	if (next == status)
		return;

	char msg[1024];
	const ISC_STATUS* p = status;

	if (fb_interpret(msg, sizeof(msg), &p) != 0)
		gds__log("%s", msg);
}


}	// namespace Jrd
