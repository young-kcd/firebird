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

#ifndef JRD_ERROR_IMPL_H
#define JRD_ERROR_IMPL_H

#include "FirebirdApi.h"
#include "firebird/ExternalEngine.h"
#include "iberror.h"

namespace Jrd {


class ErrorImpl : public Firebird::Error
{
public:
	inline ErrorImpl()
		: next(status)
	{
		*next = isc_arg_end;
	}

	virtual ~ErrorImpl();

	static void statusVectorToError(const ISC_STATUS* vector, Error* error);
	static void exceptionToError(const Firebird::Exception& ex, Error* error);

	bool errorOccurred() const
	{
		return next != status;
	}

	inline operator Error* ()
	{
		return this;
	}

	virtual bool FB_CALL addCode(Firebird::int32 code);
	virtual bool FB_CALL addString(const char* str, Firebird::uint strLength);

protected:
	ISC_STATUS_ARRAY status;
	ISC_STATUS* next;
};


class RaiseError : public ErrorImpl
{
public:
	virtual ~RaiseError();
};


class LogError : public ErrorImpl
{
public:
	virtual ~LogError();
};


class DelegateError : public Firebird::Error
{
public:
	explicit DelegateError(Firebird::Error* aError)
		: error(aError),
		  raised(false)
	{
	}

	virtual bool FB_CALL addCode(Firebird::int32 code)
	{
		raised = true;
		return error->addCode(code);
	}

	virtual bool FB_CALL addString(const char* str, Firebird::uint strLength)
	{
		raised = true;
		return error->addString(str, strLength);
	}

	bool isRaised() const
	{
		return raised;
	}

private:
	Firebird::Error* error;
	bool raised;
};


}	// namespace Jrd

#endif	// JRD_ERROR_IMPL_H
