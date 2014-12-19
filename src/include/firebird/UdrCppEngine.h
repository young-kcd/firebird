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

#ifndef FIREBIRD_UDR_CPP_ENGINE
#define FIREBIRD_UDR_CPP_ENGINE

#include "./UdrEngine.h"
#include "./Message.h"
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


#define FB_UDR_BEGIN_FUNCTION(name)	\
	namespace Func##name	\
	{	\
		class Impl;	\
		\
		static ::Firebird::Udr::FunctionFactoryImpl<Impl> factory(#name);	\
		\
		class Impl : public ::Firebird::Udr::Function<Impl>	\
		{	\
		public:	\
			FB__UDR_COMMON_IMPL

#define FB_UDR_END_FUNCTION	\
		};	\
	}

#define FB_UDR_EXECUTE_FUNCTION	\
	void execute(::Firebird::IStatus* status, ::Firebird::IExternalContext* context, \
		void* in, void* out)	\
	{	\
		try	\
		{	\
			internalExecute(status, context, (InMessage::Type*) in, (OutMessage::Type*) out);	\
		}	\
		FB__UDR_CATCH	\
	}	\
	\
	void internalExecute(::Firebird::IStatus* status, ::Firebird::IExternalContext* context, \
		InMessage::Type* in, OutMessage::Type* out)


#define FB_UDR_BEGIN_PROCEDURE(name)	\
	namespace Proc##name	\
	{	\
		class Impl;	\
		\
		static ::Firebird::Udr::ProcedureFactoryImpl<Impl> factory(#name);	\
		\
		class Impl : public ::Firebird::Udr::Procedure<Impl>	\
		{	\
		public:	\
			FB__UDR_COMMON_IMPL

#define FB_UDR_END_PROCEDURE	\
			};	\
		};	\
	}

#define FB_UDR_EXECUTE_PROCEDURE	\
	::Firebird::IExternalResultSet* open(::Firebird::IStatus* status, \
		::Firebird::IExternalContext* context, void* in, void* out)	\
	{	\
		try	\
		{	\
			return new ResultSet(status, context, this, (InMessage::Type*) in, (OutMessage::Type*) out);	\
		}	\
		FB__UDR_CATCH	\
		\
		return NULL;	\
	}	\
	\
	class ResultSet : public ::Firebird::Udr::ResultSet<ResultSet, Impl, InMessage, OutMessage>	\
	{	\
	public:	\
		ResultSet(::Firebird::IStatus* status, ::Firebird::IExternalContext* context,	\
				Impl* const procedure, InMessage::Type* const in, OutMessage::Type* const out)	\
			: ::Firebird::Udr::ResultSet<ResultSet, Impl, InMessage, OutMessage>(	\
					context, procedure, in, out)

#define FB_UDR_FETCH_PROCEDURE	\
	FB_BOOLEAN fetch(::Firebird::IStatus* status)	\
	{	\
		try	\
		{	\
			return (FB_BOOLEAN) internalFetch(status);	\
		}	\
		FB__UDR_CATCH	\
		\
		return FB_FALSE;	\
	}	\
	\
	bool internalFetch(::Firebird::IStatus* status)


#define FB_UDR_BEGIN_TRIGGER(name)	\
	namespace Trig##name	\
	{	\
		class Impl;	\
		\
		static ::Firebird::Udr::TriggerFactoryImpl<Impl> factory(#name);	\
		\
		class Impl : public ::Firebird::Udr::Trigger<Impl>	\
		{	\
		public:	\
			FB__UDR_COMMON_IMPL

#define FB_UDR_END_TRIGGER	\
		};	\
	}

#define FB_UDR_EXECUTE_TRIGGER	\
	void execute(::Firebird::IStatus* status, ::Firebird::IExternalContext* context,	\
		unsigned int action, void* oldFields, void* newFields)	\
	{	\
		try	\
		{	\
			internalExecute(status, context, action,	\
				(FieldsMessage::Type*) oldFields, (FieldsMessage::Type*) newFields);	\
		}	\
		FB__UDR_CATCH	\
	}	\
	\
	void internalExecute(::Firebird::IStatus* status, ::Firebird::IExternalContext* context,	\
		unsigned int action, \
		FieldsMessage::Type* oldFields, FieldsMessage::Type* newFields)


#define FB_UDR_CONSTRUCTOR	\
	Impl(::Firebird::IStatus* const status, ::Firebird::IExternalContext* const context,	\
			::Firebird::IRoutineMetadata* const metadata__)	\
		: master(context->getMaster()),	\
		  metadata(metadata__)

#define FB_UDR_DESTRUCTOR	\
	~Impl()


#define FB__UDR_COMMON_IMPL	\
	Impl(const void* const, ::Firebird::IExternalContext* const context,	\
			::Firebird::IRoutineMetadata* const aMetadata)	\
		: master(context->getMaster()),	\
		  metadata(aMetadata)	\
	{	\
	}	\
	\
	::Firebird::IMaster* master;	\
	::Firebird::IRoutineMetadata* metadata;

#define FB__UDR_COMMON_TYPE(name)	\
	struct name	\
	{	\
		typedef unsigned char Type;	\
		static void setup(::Firebird::IStatus*, ::Firebird::IMetadataBuilder*) {}	\
	}

#define FB__UDR_CATCH	\
	catch (const ::Firebird::Udr::StatusException& e)	\
	{	\
		e.stuff(status);	\
	}	\
	catch (...)	\
	{	\
		ISC_STATUS statusVector[] = {	\
			isc_arg_gds, isc_random, isc_arg_string, (ISC_STATUS) "Unrecognized C++ exception",	\
			isc_arg_end};	\
		status->setErrors(statusVector);	\
	}


class StatusException
{
public:
	StatusException(const ISC_STATUS* vector)
	{
		ISC_STATUS* p = statusVector;

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
					*p++ = *vector++;
					*p++ = *vector++;
					break;

				case isc_arg_string:
					*p++ = *vector++;
					*p++ = *vector++;
					break;

				case isc_arg_cstring:
					*p++ = *vector++;
					*p++ = *vector++;
					*p++ = *vector++;
					break;

				default:
					return;
			}
		}

		*p = isc_arg_end;
	}

public:
	static void check(const ISC_STATUS* vector)
	{
		if (vector[1])
			throw StatusException(vector);
	}

	static void checkStatus(ISC_STATUS status, const ISC_STATUS* vector)
	{
		if (status == 0)
			return;

		check(vector);
	}

	template <typename T>
	static T check(IStatus* status, T value)
	{
		if (status->getStatus() & Firebird::IStatus::FB_HAS_ERRORS)
			StatusException::check(status->getErrors());		// !!! loose warnings!
		return value;
	}

public:
	const ISC_STATUS* getStatusVector() const
	{
		return statusVector;
	}

	void stuff(IStatus* status) const
	{
		status->setErrors(statusVector);
	}

private:
	ISC_STATUS_ARRAY statusVector;
};

class StatusImpl : public Api::IStatusImpl<StatusImpl>
{
public:
	StatusImpl(IMaster* master)
		: delegate(master->getStatus()),
		  state(0)
	{
	}

	int getVersion()
	{
		return IStatus::VERSION;
	}

	IPluginModule* getModule()
	{
		return NULL;
	}

	void dispose()
	{
		delegate->dispose();
		delete this;
	}

	void setErrors(const ISC_STATUS* value)
	{
		delegate->setErrors(value);
		state = delegate->getStatus();
	}

	void setWarnings(const ISC_STATUS* value)
	{
		delegate->setWarnings(value);
		state = delegate->getStatus();
	}

	void setErrors2(unsigned length, const ISC_STATUS* value)
	{
		delegate->setErrors2(length, value);
		state = delegate->getStatus();
	}

	void setWarnings2(unsigned length, const ISC_STATUS* value)
	{
		delegate->setWarnings2(length, value);
		state = delegate->getStatus();
	}

	void init()
	{
		delegate->init();
		state = 0;
	}

	const ISC_STATUS* getErrors() const
	{
		return delegate->getErrors();
	}

	const ISC_STATUS* getWarnings() const
	{
		return delegate->getWarnings();
	}

	unsigned getStatus() const
	{
		return state;
	}

public:
	void check()
	{
		if (state & IStatus::FB_HAS_ERRORS)
			StatusException::check(delegate->getErrors());
	}

private:
	IStatus* delegate;
	unsigned state;
};


template <typename T> class Procedure;


class Helper
{
public:
	static isc_db_handle getIscDbHandle(IExternalContext* context)
	{
		StatusImpl status(context->getMaster());

		IAttachment* attachment = context->getAttachment(&status);
		status.check();

		ISC_STATUS_ARRAY statusVector = {0};
		isc_db_handle handle = 0;

		fb_get_database_handle(statusVector, &handle, attachment);
		StatusException::check(statusVector);

		return handle;
	}

	static isc_tr_handle getIscTrHandle(IExternalContext* context)
	{
		StatusImpl status(context->getMaster());

		ITransaction* transaction = context->getTransaction(&status);
		status.check();

		ISC_STATUS_ARRAY statusVector = {0};
		isc_tr_handle handle = 0;

		fb_get_transaction_handle(statusVector, &handle, transaction);
		StatusException::check(statusVector);

		return handle;
	}
};


template <typename This, typename Procedure, typename InMessage, typename OutMessage>
class ResultSet : public Api::IExternalResultSetImpl<This>, public Helper
{
public:
	ResultSet(IExternalContext* aContext, Procedure* aProcedure,
				typename InMessage::Type* aIn, typename OutMessage::Type* aOut)
		: context(aContext),
		  procedure(aProcedure),
		  in(aIn),
		  out(aOut)
	{
	}

public:
	int getVersion()
	{
		return IExternalResultSet::VERSION;
	}

	IPluginModule* getModule()
	{
		return NULL;
	}

	void dispose()
	{
		delete static_cast<This*>(this);
	}

protected:
	IExternalContext* const context;
	Procedure* const procedure;
	typename InMessage::Type* const in;
	typename OutMessage::Type* const out;
};


template <typename This>
class Function : public Api::IExternalFunctionImpl<This>, public Helper
{
public:
	FB__UDR_COMMON_TYPE(InMessage);
	FB__UDR_COMMON_TYPE(OutMessage);

	IPluginModule* getModule()
	{
		return NULL;
	}

	void dispose()
	{
		delete static_cast<This*>(this);
	}

	void getCharSet(IStatus* /*status*/, IExternalContext* /*context*/,
		char* /*name*/, unsigned /*nameSize*/)
	{
	}
};


template <typename This>
class Procedure : public Api::IExternalProcedureImpl<This>, public Helper
{
public:
	FB__UDR_COMMON_TYPE(InMessage);
	FB__UDR_COMMON_TYPE(OutMessage);

	IPluginModule* getModule()
	{
		return NULL;
	}

	void dispose()
	{
		delete static_cast<This*>(this);
	}

	void getCharSet(IStatus* /*status*/, IExternalContext* /*context*/,
		char* /*name*/, unsigned /*nameSize*/)
	{
	}
};


template <typename This>
class Trigger : public Api::IExternalTriggerImpl<This>, public Helper
{
public:
	FB__UDR_COMMON_TYPE(FieldsMessage);

	IPluginModule* getModule()
	{
		return NULL;
	}

	void dispose()
	{
		delete static_cast<This*>(this);
	}

	void getCharSet(IStatus* /*status*/, IExternalContext* /*context*/,
		char* /*name*/, unsigned /*nameSize*/)
	{
	}
};


template <typename T> class FunctionFactoryImpl : public FunctionFactory
{
public:
	explicit FunctionFactoryImpl(const char* name)
	{
		fbUdrRegFunction(name, this);
	}

	void setup(IStatus* status, IExternalContext* /*context*/,
		IRoutineMetadata* /*metadata*/, IMetadataBuilder* in, IMetadataBuilder* out)
	{
		T::InMessage::setup(status, in);
		T::OutMessage::setup(status, out);
	}

	IExternalFunction* newItem(IStatus* status, IExternalContext* context,
		IRoutineMetadata* metadata)
	{
		try
		{
			return new T(status, context, metadata);
		}
		FB__UDR_CATCH

		return NULL;
	}
};


template <typename T> class ProcedureFactoryImpl : public ProcedureFactory
{
public:
	explicit ProcedureFactoryImpl(const char* name)
	{
		fbUdrRegProcedure(name, this);
	}

	void setup(IStatus* status, IExternalContext* /*context*/,
		IRoutineMetadata* /*metadata*/, IMetadataBuilder* in, IMetadataBuilder* out)
	{
		T::InMessage::setup(status, in);
		T::OutMessage::setup(status, out);
	}

	IExternalProcedure* newItem(IStatus* status, IExternalContext* context,
		IRoutineMetadata* metadata)
	{
		try
		{
			return new T(status, context, metadata);
		}
		FB__UDR_CATCH

		return NULL;
	}
};


template <typename T> class TriggerFactoryImpl : public TriggerFactory
{
public:
	explicit TriggerFactoryImpl(const char* name)
	{
		fbUdrRegTrigger(name, this);
	}

	void setup(IStatus* status, IExternalContext* /*context*/,
		IRoutineMetadata* /*metadata*/, IMetadataBuilder* fields)
	{
		T::FieldsMessage::setup(status, fields);
	}

	IExternalTrigger* newItem(IStatus* status, IExternalContext* context,
		IRoutineMetadata* metadata)
	{
		try
		{
			return new T(status, context, metadata);
		}
		FB__UDR_CATCH

		return NULL;
	}
};


//------------------------------------------------------------------------------
	}	// namespace Udr
}	// namespace Firebird

#endif	// FIREBIRD_UDR_CPP_ENGINE
