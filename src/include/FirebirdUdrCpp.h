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
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2008 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef FIREBIRD_PLUGIN_UDR_CPP
#define FIREBIRD_PLUGIN_UDR_CPP

#ifdef __GNUC__
#	pragma GCC system_header	// disable warning about non-existent virtual destructor
#endif

#include "FirebirdApi.h"
#include "FirebirdExternalApi.h"
#include "FirebirdUdr.h"
#ifndef JRD_IBASE_H
#include "ibase.h"
#include "iberror.h"
#endif
#include <string.h>


namespace Firebird
{
	namespace Udr
	{
//------------------------------------------------------------------------------


#define FB_UDR_FUNCTION(name)	Func##name
#define FB_UDR_PROCEDURE(name)	Proc##name
#define FB_UDR_TRIGGER(name)	Trig##name


#define FB_UDR_BEGIN_DECLARE_FUNCTION(name)	\
	class FB_UDR_FUNCTION(name) : public ::Firebird::Udr::Function	\
	{	\
	public:	\
		virtual void FB_CALL execute(::Firebird::Error* error, ::Firebird::ExternalContext* context, \
			::Firebird::Values* params, ::Firebird::Value* result);	\
	private:

#define FB_UDR_END_DECLARE_FUNCTION(name)	\
	};

#define FB_UDR_DECLARE_FUNCTION(name)	\
	FB_UDR_BEGIN_DECLARE_FUNCTION(name)	\
	FB_UDR_END_DECLARE_FUNCTION(name)

#define FB_UDR_BEGIN_FUNCTION(name)	\
	void FB_CALL FB_UDR_FUNCTION(name)::execute(::Firebird::Error* error, \
		::Firebird::ExternalContext* context, ::Firebird::Values* params, ::Firebird::Value* result)	\
	{	\
		try	\
		{

#define FB_UDR_END_FUNCTION(name)	\
		}	\
		catch (const ::Firebird::Udr::ThrowError::Exception& e)	\
		{	\
			e.stuff(error);	\
		}	\
		catch (...)	\
		{	\
			error->addCode(isc_arg_gds);	\
			error->addCode(isc_random);	\
			error->addString(	\
				FB_UDR_UNRECOGNIZED_EXCEPTION,	\
				strlen(FB_UDR_UNRECOGNIZED_EXCEPTION));	\
		}	\
	}	\
	::Firebird::Udr::FunctionFactoryImpl<FB_UDR_FUNCTION(name)> FuncFactory##name(#name);


#define FB_UDR_BEGIN_DECLARE_PROCEDURE(name)	\
	class FB_UDR_PROCEDURE(name) : public ::Firebird::Udr::Procedure	\
	{	\
	public:	\
		virtual ::Firebird::ExternalResultSet* FB_CALL open(::Firebird::Error* error, \
			::Firebird::ExternalContext* context, ::Firebird::Values* params, \
			::Firebird::Values* results);	\

#define FB_UDR_END_DECLARE_PROCEDURE(name)	\
	};

#define FB_UDR_DECLARE_PROCEDURE(name)	\
	FB_UDR_BEGIN_DECLARE_PROCEDURE(name)	\
	FB_UDR_END_DECLARE_PROCEDURE(name)

#define FB_UDR_BEGIN_DECLARE_FETCH_PROCEDURE(name)	\
	class ResultSet##name : public ::Firebird::Udr::ResultSet	\
	{	\
	public:	\
		ResultSet##name(::Firebird::Error* error, ::Firebird::ExternalContext* context, \
			::Firebird::Values* params, ::Firebird::Values* results);	\
	\
	public:	\
		virtual bool FB_CALL fetch(::Firebird::Error* error);	\
	\
	private:

#define FB_UDR_END_DECLARE_FETCH_PROCEDURE(name)	\
	};

#define FB_UDR_DECLARE_FETCH_PROCEDURE(name)	\
	FB_UDR_BEGIN_DECLARE_FETCH_PROCEDURE(name)	\
	FB_UDR_END_DECLARE_FETCH_PROCEDURE(name)

#define FB_UDR_DECLARE_PROCEDURE(name)	\
	FB_UDR_BEGIN_DECLARE_PROCEDURE(name)	\
	FB_UDR_END_DECLARE_PROCEDURE(name)

#define FB_UDR_BEGIN_PROCEDURE(name)	\
	::Firebird::ExternalResultSet* FB_CALL Proc##name::open(::Firebird::Error* error, \
		::Firebird::ExternalContext* context, ::Firebird::Values* params, \
		::Firebird::Values* results)	\
	{	\
		return new ResultSet##name(error, context, params, results);	\
	}	\
	\
	ResultSet##name::ResultSet##name(::Firebird::Error* error, ::Firebird::ExternalContext* context, \
			::Firebird::Values* params, ::Firebird::Values* results)	\
		: ResultSet(context, params, results)	\
	{	\
		try	\
		{

#define FB_UDR_FETCH_PROCEDURE(name)	\
		}	\
		catch (const ::Firebird::Udr::ThrowError::Exception& e)	\
		{	\
			e.stuff(error);	\
		}	\
		catch (...)	\
		{	\
			error->addCode(isc_arg_gds);	\
			error->addCode(isc_random);	\
			error->addString(	\
				FB_UDR_UNRECOGNIZED_EXCEPTION,	\
				strlen(FB_UDR_UNRECOGNIZED_EXCEPTION));	\
		}	\
	}	\
	\
	bool FB_CALL ResultSet##name::fetch(::Firebird::Error* error)	\
	{	\
		try	\
		{

#define FB_UDR_END_PROCEDURE(name)	\
		}	\
		catch (const ::Firebird::Udr::ThrowError::Exception& e)	\
		{	\
			e.stuff(error);	\
		}	\
		catch (...)	\
		{	\
			error->addCode(isc_arg_gds);	\
			error->addCode(isc_random);	\
			error->addString(	\
				FB_UDR_UNRECOGNIZED_EXCEPTION,	\
				strlen(FB_UDR_UNRECOGNIZED_EXCEPTION));	\
		}	\
		return false;	\
	}	\
	::Firebird::Udr::ProcedureFactoryImpl<FB_UDR_PROCEDURE(name)> ProcFactory##name(#name);


#define FB_UDR_BEGIN_DECLARE_TRIGGER(name)	\
	class FB_UDR_TRIGGER(name) : public ::Firebird::Udr::Trigger	\
	{	\
	public:	\
		virtual void FB_CALL execute(::Firebird::Error* error, ::Firebird::ExternalContext* context, \
			::Firebird::ExternalTrigger::Action action, const ::Firebird::Values* oldValues,	\
			::Firebird::Values* newValues);	\
	private:

#define FB_UDR_END_DECLARE_TRIGGER(name)	\
	};

#define FB_UDR_DECLARE_TRIGGER(name)	\
	FB_UDR_BEGIN_DECLARE_TRIGGER(name)	\
	FB_UDR_END_DECLARE_TRIGGER(name)

#define FB_UDR_BEGIN_TRIGGER(name)	\
	void FB_CALL FB_UDR_TRIGGER(name)::execute(::Firebird::Error* error,	\
		::Firebird::ExternalContext* context, ::Firebird::ExternalTrigger::Action action, \
		const ::Firebird::Values* oldValues, ::Firebird::Values* newValues)	\
	{	\
		try	\
		{

#define FB_UDR_END_TRIGGER(name)	\
		}	\
		catch (const ::Firebird::Udr::ThrowError::Exception& e)	\
		{	\
			e.stuff(error);	\
		}	\
		catch (...)	\
		{	\
			error->addCode(isc_arg_gds);	\
			error->addCode(isc_random);	\
			error->addString(	\
				FB_UDR_UNRECOGNIZED_EXCEPTION,	\
				strlen(FB_UDR_UNRECOGNIZED_EXCEPTION));	\
		}	\
	}	\
	::Firebird::Udr::TriggerFactoryImpl<FB_UDR_TRIGGER(name)> TrigFactory##name(#name);


#define FB_UDR_UNRECOGNIZED_EXCEPTION	"Unrecognized C++ exception"


class ThrowError : public Error
{
private:
	struct Info
	{
		Info()
			: next(FB_NULL),
			  str(FB_NULL)
		{
		}

		~Info()
		{
			if (str)
				delete [] str;
		}

		static void free(Info* info)
		{
			while (info)
			{
				Info* p = info;
				info = info->next;
				delete p;
			}
		}

		enum { TYPE_CODE, TYPE_STR } type;

		Info* next;
		int32 code;
		char* str;
		int strLength;
	};

public:
	class Exception
	{
	public:
		explicit Exception(Info* aInfo)
			: info(aInfo)
		{
		}

		Exception(const Exception& e)
			: info(FB_NULL)
		{
			Info* end = FB_NULL;

			for (const Info* p = e.info; p; p = p->next)
			{
				Info* newInfo = new Info;
				newInfo->type = p->type;
				newInfo->code = p->code;

				if (p->str)
				{
					newInfo->str = new char[p->strLength];
					memcpy(newInfo->str, p->str, p->strLength);
					newInfo->strLength = p->strLength;
				}

				if (end)
					end->next = newInfo;

				end = newInfo;

				if (!info)
					info = newInfo;
			}
		}

		~Exception()
		{
			Info::free(info);
		}

	public:
		void stuff(Error* error) const
		{
			for (const Info* p = info; p; p = p->next)
			{
				if (p->type == Info::TYPE_CODE)
					error->addCode(p->code);
				else if (p->type == Info::TYPE_STR)
					error->addString(p->str, p->strLength);
			}
		}

	private:
		Info* info;
	};

public:
	ThrowError()
		: start(FB_NULL),
		  end(FB_NULL)
	{
	}

	virtual ~ThrowError()
	{
		raise();
		Info::free(start);
	}

public:
	static void check(ISC_STATUS status, const ISC_STATUS* vector)
	{
		if (status == 0)
			return;

		ThrowError error;

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
					error.addCode(*vector++);
					error.addCode(*vector++);
					break;

				case isc_arg_string:
					error.addString((const char*) vector[1], strlen((const char*) vector[1]));
					vector += 2;
					break;

				case isc_arg_cstring:
					error.addString((const char*) vector[2], vector[1]);
					vector += 3;
					break;

				default:
					return;
			}
		}
	}

	static void check(const ISC_STATUS* vector)
	{
		check(vector[1], vector);
	}

public:
	inline operator Firebird::Error* ()
	{
		return this;
	}

public:
	virtual bool FB_CALL addCode(Firebird::int32 code)
	{
		Info* info = new Info;
		info->type = Info::TYPE_CODE;
		info->code = code;

		if (end)
			end->next = info;

		end = info;

		if (!start)
			start = info;

		return true;
	}

	virtual bool FB_CALL addString(const char* str, uint strLength)
	{
		Info* info = new Info;
		info->type = Info::TYPE_STR;
		info->str = new char[strLength];
		memcpy(info->str, str, strLength);
		info->strLength = strLength;

		if (end)
			end->next = info;

		end = info;

		if (!start)
			start = info;

		return true;
	}

private:
	void raise()
	{
		if (start)
			throw Exception(start);
	}

protected:
	Info* start;
	Info* end;
};


class Helper
{
public:
	static isc_db_handle getIscDbHandle(ExternalContext* context)
	{
		ISC_STATUS_ARRAY status = {0};
		isc_db_handle handle = 0;
		fb_get_database_handle(status, &handle, context->getAttachment(ThrowError()));
		ThrowError::check(status);
		return handle;
	}

	static isc_tr_handle getIscTrHandle(ExternalContext* context)
	{
		ISC_STATUS_ARRAY status = {0};
		isc_tr_handle handle = 0;
		fb_get_transaction_handle(status, &handle, context->getTransaction(ThrowError()));
		ThrowError::check(status);
		return handle;
	}

	static void* getEntryPoint(ExternalContext* /*context*/, const char* entryPoint)
	{
		return fbUdrGetFunction(entryPoint);
	}
};


class ResultSet : public ExternalResultSet, public Helper
{
public:
	ResultSet(Firebird::ExternalContext* aContext, Firebird::Values* aParams,
				Firebird::Values* aResults)
		: context(aContext),
		  params(aParams),
		  results(aResults)
	{
	}

	virtual ~ResultSet()
	{
	}

public:
	virtual void FB_CALL dispose(Firebird::Error* /*error*/)
	{
		delete this;
	}

protected:
	Firebird::ExternalContext* context;
	Firebird::Values* params;
	Firebird::Values* results;
};


class Function : public ExternalFunction, public Helper
{
public:
	virtual ~Function()
	{
	}

public:
	virtual void FB_CALL dispose(Error* /*error*/)
	{
		delete this;
	}

	void FB_CALL getCharSet(Error* /*error*/, ExternalContext* /*context*/, Utf8* /*name*/, uint /*nameSize*/)
	{
	}

public:
	MetaInfo* metaInfo;
};


class Procedure : public ExternalProcedure, public Helper
{
public:
	virtual ~Procedure()
	{
	}

public:
	virtual void FB_CALL dispose(Error* /*error*/)
	{
		delete this;
	}

	void FB_CALL getCharSet(Error* /*error*/, ExternalContext* /*context*/, Utf8* /*name*/, uint /*nameSize*/)
	{
	}

public:
	MetaInfo* metaInfo;
};


class Trigger : public ExternalTrigger, public Helper
{
public:
	virtual ~Trigger()
	{
	}

public:
	virtual void FB_CALL dispose(Error* /*error*/)
	{
		delete this;
	}

	void FB_CALL getCharSet(Error* /*error*/, ExternalContext* /*context*/, Utf8* /*name*/, uint /*nameSize*/)
	{
	}

public:
	TriggerMetaInfo* metaInfo;
};


template <typename T> class FunctionFactoryImpl : public FunctionFactory
{
public:
	explicit FunctionFactoryImpl(const char* aName)
		: name(aName)
	{
		fbUdrRegFunction(this);
	}

public:
	virtual const char* FB_CALL getName()
	{
		return name;
	}

	virtual ExternalFunction* FB_CALL newItem(MetaInfo* metaInfo)
	{
		Function* function = new T();
		function->metaInfo = metaInfo;
		return function;
	}

private:
	const char* name;
};


template <typename T> class ProcedureFactoryImpl : public ProcedureFactory
{
public:
	explicit ProcedureFactoryImpl(const char* aName)
		: name(aName)
	{
		fbUdrRegProcedure(this);
	}

public:
	virtual const char* FB_CALL getName()
	{
		return name;
	}

	virtual ExternalProcedure* FB_CALL newItem(MetaInfo* metaInfo)
	{
		Procedure* procedure = new T();
		procedure->metaInfo = metaInfo;
		return procedure;
	}

private:
	const char* name;
};


template <typename T> class TriggerFactoryImpl : public TriggerFactory
{
public:
	explicit TriggerFactoryImpl(const char* aName)
		: name(aName)
	{
		fbUdrRegTrigger(this);
	}

public:
	virtual const char* FB_CALL getName()
	{
		return name;
	}

	virtual ExternalTrigger* FB_CALL newItem(TriggerMetaInfo* metaInfo)
	{
		Trigger* trigger = new T();
		trigger->metaInfo = metaInfo;
		return trigger;
	}

private:
	const char* name;
};


//------------------------------------------------------------------------------
	}	// namespace Udr
}	// namespace Firebird

#endif	// FIREBIRD_PLUGIN_UDR_CPP
