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


//// TODO: Make StatusType customizable.
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
	void execute(::Firebird::ThrowStatusWrapper* status, ::Firebird::IExternalContext* context, \
		void* in, void* out)	\
	{	\
		internalExecute(status, context, (InMessage::Type*) in, (OutMessage::Type*) out);	\
	}	\
	\
	void internalExecute(::Firebird::ThrowStatusWrapper* status, ::Firebird::IExternalContext* context, \
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
	::Firebird::IExternalResultSet* open(::Firebird::ThrowStatusWrapper* status, \
		::Firebird::IExternalContext* context, void* in, void* out)	\
	{	\
		return new ResultSet(status, context, this, (InMessage::Type*) in, (OutMessage::Type*) out);	\
	}	\
	\
	class ResultSet : public ::Firebird::Udr::ResultSet<ResultSet, Impl, InMessage, OutMessage>	\
	{	\
	public:	\
		ResultSet(::Firebird::ThrowStatusWrapper* status, ::Firebird::IExternalContext* context,	\
				Impl* const procedure, InMessage::Type* const in, OutMessage::Type* const out)	\
			: ::Firebird::Udr::ResultSet<ResultSet, Impl, InMessage, OutMessage>(	\
					context, procedure, in, out)

#define FB_UDR_FETCH_PROCEDURE	\
	FB_BOOLEAN fetch(::Firebird::ThrowStatusWrapper* status)	\
	{	\
		return (FB_BOOLEAN) internalFetch(status);	\
	}	\
	\
	bool internalFetch(::Firebird::ThrowStatusWrapper* status)


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
	void execute(::Firebird::ThrowStatusWrapper* status, ::Firebird::IExternalContext* context,	\
		unsigned int action, void* oldFields, void* newFields)	\
	{	\
		internalExecute(status, context, action,	\
			(FieldsMessage::Type*) oldFields, (FieldsMessage::Type*) newFields);	\
	}	\
	\
	void internalExecute(::Firebird::ThrowStatusWrapper* status, ::Firebird::IExternalContext* context,	\
		unsigned int action, \
		FieldsMessage::Type* oldFields, FieldsMessage::Type* newFields)


#define FB_UDR_CONSTRUCTOR	\
	Impl(::Firebird::ThrowStatusWrapper* const status, ::Firebird::IExternalContext* const context,	\
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
		static void setup(::Firebird::ThrowStatusWrapper*, ::Firebird::IMetadataBuilder*) {}	\
	}


template <typename T> class Procedure;


class Helper
{
public:
	template <typename StatusType>
	static isc_db_handle getIscDbHandle(StatusType* status, IExternalContext* context)
	{
		IAttachment* attachment = context->getAttachment(status);

		if (!attachment)
			return 0;

		ISC_STATUS_ARRAY statusVector = {0};
		isc_db_handle handle = 0;

		fb_get_database_handle(statusVector, &handle, attachment);

		if (!handle)
		{
			status->setErrors(statusVector);
			StatusType::checkException(status);
		}

		return handle;
	}

	template <typename StatusType>
	static isc_tr_handle getIscTrHandle(StatusType* status, IExternalContext* context)
	{
		ITransaction* transaction = context->getTransaction(status);

		if (!transaction)
			return 0;

		ISC_STATUS_ARRAY statusVector = {0};
		isc_tr_handle handle = 0;

		fb_get_transaction_handle(statusVector, &handle, transaction);

		if (!handle)
		{
			status->setErrors(statusVector);
			StatusType::checkException(status);
		}

		return handle;
	}
};


template <typename This, typename Procedure, typename InMessage, typename OutMessage>
class ResultSet : public IExternalResultSetImpl<This, ThrowStatusWrapper>, public Helper
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
class Function : public IExternalFunctionImpl<This, ThrowStatusWrapper>, public Helper
{
public:
	FB__UDR_COMMON_TYPE(InMessage);
	FB__UDR_COMMON_TYPE(OutMessage);

	void dispose()
	{
		delete static_cast<This*>(this);
	}

	void getCharSet(ThrowStatusWrapper* /*status*/, IExternalContext* /*context*/,
		char* /*name*/, unsigned /*nameSize*/)
	{
	}
};


template <typename This>
class Procedure : public IExternalProcedureImpl<This, ThrowStatusWrapper>, public Helper
{
public:
	FB__UDR_COMMON_TYPE(InMessage);
	FB__UDR_COMMON_TYPE(OutMessage);

	void dispose()
	{
		delete static_cast<This*>(this);
	}

	void getCharSet(ThrowStatusWrapper* /*status*/, IExternalContext* /*context*/,
		char* /*name*/, unsigned /*nameSize*/)
	{
	}
};


template <typename This>
class Trigger : public IExternalTriggerImpl<This, ThrowStatusWrapper>, public Helper
{
public:
	FB__UDR_COMMON_TYPE(FieldsMessage);

	void dispose()
	{
		delete static_cast<This*>(this);
	}

	void getCharSet(ThrowStatusWrapper* /*status*/, IExternalContext* /*context*/,
		char* /*name*/, unsigned /*nameSize*/)
	{
	}
};


template <typename T> class FunctionFactoryImpl :
	public IUdrFunctionFactoryImpl<FunctionFactoryImpl<T>, ThrowStatusWrapper>
{
public:
	explicit FunctionFactoryImpl(const char* name)
	{
		fbUdrRegFunction(name, this);
	}

	void setup(ThrowStatusWrapper* status, IExternalContext* /*context*/,
		IRoutineMetadata* /*metadata*/, IMetadataBuilder* in, IMetadataBuilder* out)
	{
		T::InMessage::setup(status, in);
		T::OutMessage::setup(status, out);
	}

	IExternalFunction* newItem(ThrowStatusWrapper* status, IExternalContext* context,
		IRoutineMetadata* metadata)
	{
		return new T(status, context, metadata);
	}
};


template <typename T> class ProcedureFactoryImpl :
	public IUdrProcedureFactoryImpl<ProcedureFactoryImpl<T>, ThrowStatusWrapper>
{
public:
	explicit ProcedureFactoryImpl(const char* name)
	{
		fbUdrRegProcedure(name, this);
	}

	void setup(ThrowStatusWrapper* status, IExternalContext* /*context*/,
		IRoutineMetadata* /*metadata*/, IMetadataBuilder* in, IMetadataBuilder* out)
	{
		T::InMessage::setup(status, in);
		T::OutMessage::setup(status, out);
	}

	IExternalProcedure* newItem(ThrowStatusWrapper* status, IExternalContext* context,
		IRoutineMetadata* metadata)
	{
		return new T(status, context, metadata);
	}
};


template <typename T> class TriggerFactoryImpl :
	public IUdrTriggerFactoryImpl<TriggerFactoryImpl<T>, ThrowStatusWrapper>
{
public:
	explicit TriggerFactoryImpl(const char* name)
	{
		fbUdrRegTrigger(name, this);
	}

	void setup(ThrowStatusWrapper* status, IExternalContext* /*context*/,
		IRoutineMetadata* /*metadata*/, IMetadataBuilder* fields)
	{
		T::FieldsMessage::setup(status, fields);
	}

	IExternalTrigger* newItem(ThrowStatusWrapper* status, IExternalContext* context,
		IRoutineMetadata* metadata)
	{
		return new T(status, context, metadata);
	}
};


//------------------------------------------------------------------------------
	}	// namespace Udr
}	// namespace Firebird

#endif	// FIREBIRD_UDR_CPP_ENGINE
