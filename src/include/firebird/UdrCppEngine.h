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

#include "./ExternalEngine.h"
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


#define FB_UDR_FUNCTION(name)	Func##name
#define FB_UDR_PROCEDURE(name)	Proc##name
#define FB_UDR_TRIGGER(name)	Trig##name


#define FB_UDR_BEGIN_FUNCTION(name)	\
	class FB_UDR_FUNCTION(name);	\
	\
	::Firebird::Udr::FunctionFactoryImpl<FB_UDR_FUNCTION(name)> FuncFactory##name(#name);	\
	\
	class FB_UDR_FUNCTION(name) : public ::Firebird::Udr::Function<FB_UDR_FUNCTION(name)>	\
	{	\
	public:	\
		void initialize(::Firebird::IStatus* /*status*/, void*)	\
		{	\
		}

#define FB_UDR_END_FUNCTION	\
	};

#define FB_UDR_EXECUTE_DYNAMIC_FUNCTION	\
	typedef void* InMessage;	\
	typedef void* OutMessage;	\
	\
	FB_UDR_EXECUTE__FUNCTION

#define FB_UDR_EXECUTE_MESSAGE_FUNCTION(inputs, output)	\
	FB_MESSAGE(InMessage,	\
		inputs	\
	);	\
	FB_MESSAGE(OutMessage,	\
		output	\
	);	\
	\
	FB_UDR_EXECUTE__FUNCTION

#define FB_UDR_EXECUTE_MESSAGE_FUNCTION_OUT(outputs)	\
	typedef void* InMessage;	\
	FB_MESSAGE(OutMessage,	\
		outputs	\
	);	\
	\
	FB_UDR_EXECUTE__FUNCTION

#define FB_UDR_EXECUTE__FUNCTION	\
	virtual void FB_CALL execute(::Firebird::IStatus* status, ::Firebird::ExternalContext* context, \
		void* in, void* out)	\
	{	\
		try	\
		{	\
			internalExecute(status, context, (InMessage*) in, (OutMessage*) out);	\
		}	\
		FB_UDR__CATCH	\
	}	\
	\
	void internalExecute(::Firebird::IStatus* status, ::Firebird::ExternalContext* context, \
		InMessage* in, OutMessage* out)


#define FB_UDR_BEGIN_PROCEDURE(name)	\
	class FB_UDR_PROCEDURE(name);	\
	\
	::Firebird::Udr::ProcedureFactoryImpl<FB_UDR_PROCEDURE(name)> ProcFactory##name(#name);	\
	\
	class FB_UDR_PROCEDURE(name) : public ::Firebird::Udr::Procedure<FB_UDR_PROCEDURE(name)>	\
	{	\
	public:	\
		typedef FB_UDR_PROCEDURE(name) This;	\
		\
		void initialize(::Firebird::IStatus* /*status*/, void*)	\
		{	\
		}

#define FB_UDR_END_PROCEDURE	\
		};	\
	};

#define FB_UDR_EXECUTE_DYNAMIC_PROCEDURE	\
	typedef void* InMessage;	\
	typedef void* OutMessage;	\
	\
	FB_UDR_EXECUTE__PROCEDURE

#define FB_UDR_EXECUTE_MESSAGE_PROCEDURE(inputs, outputs)	\
	FB_MESSAGE(InMessage,	\
		inputs	\
	);	\
	FB_MESSAGE(OutMessage,	\
		outputs	\
	);	\
	\
	FB_UDR_EXECUTE__PROCEDURE

#define FB_UDR_EXECUTE_MESSAGE_PROCEDURE_IN(inputs)	\
	FB_MESSAGE(InMessage,	\
		inputs	\
	);	\
	typedef void* OutMessage;	\
	\
	FB_UDR_EXECUTE__PROCEDURE

#define FB_UDR_EXECUTE_MESSAGE_PROCEDURE_OUT(outputs)	\
	typedef void* InMessage;	\
	FB_MESSAGE(OutMessage,	\
		outputs	\
	);	\
	\
	FB_UDR_EXECUTE__PROCEDURE

#define FB_UDR_EXECUTE__PROCEDURE	\
	virtual ::Firebird::ExternalResultSet* FB_CALL open(::Firebird::IStatus* status, \
		::Firebird::ExternalContext* context, void* in, void* out)	\
	{	\
		try	\
		{	\
			return new ResultSet(status, context, this, (InMessage*) in, (OutMessage*) out);	\
		}	\
		FB_UDR__CATCH	\
		\
		return 0;	\
	}	\
	\
	class ResultSet : public ::Firebird::Udr::ResultSet<ResultSet, This, InMessage, OutMessage>	\
	{	\
	public:	\
		ResultSet(::Firebird::IStatus* status, ::Firebird::ExternalContext* context,	\
				This* const procedure, InMessage* const in, OutMessage* const out)	\
			: ::Firebird::Udr::ResultSet<ResultSet, This, InMessage, OutMessage>(	\
					context, procedure, in, out)

#define FB_UDR_FETCH_PROCEDURE	\
	virtual bool FB_CALL fetch(::Firebird::IStatus* status)	\
	{	\
		try	\
		{	\
			return internalFetch(status);	\
		}	\
		FB_UDR__CATCH	\
		\
		return 0;	\
	}	\
	\
	bool internalFetch(::Firebird::IStatus* status)


#define FB_UDR_BEGIN_TRIGGER(name)	\
	class FB_UDR_TRIGGER(name);	\
	\
	::Firebird::Udr::TriggerFactoryImpl<FB_UDR_TRIGGER(name)> TrigFactory##name(#name);	\
	\
	class FB_UDR_TRIGGER(name) : public ::Firebird::Udr::Trigger<FB_UDR_TRIGGER(name)>	\
	{	\
	public:	\
		\
		void initialize(::Firebird::IStatus* /*status*/, void*)	\
		{	\
		}

#define FB_UDR_END_TRIGGER	\
	};

#define FB_UDR_EXECUTE_DYNAMIC_TRIGGER	\
	typedef void* FieldsMessage;	\
	\
	FB_UDR_EXECUTE__TRIGGER

#define FB_UDR_EXECUTE_MESSAGE_TRIGGER(fields)	\
	FB_TRIGGER_MESSAGE(FieldsMessage,	\
		fields	\
	);	\
	\
	FB_UDR_EXECUTE__TRIGGER

#define FB_UDR_EXECUTE__TRIGGER	\
	virtual void FB_CALL execute(::Firebird::IStatus* status, ::Firebird::ExternalContext* context,	\
		::Firebird::ExternalTrigger::Action action, void* oldFields, void* newFields)	\
	{	\
		try	\
		{	\
			internalExecute(status, context, action, (FieldsMessage*) oldFields, (FieldsMessage*) newFields);	\
		}	\
		FB_UDR__CATCH	\
	}	\
	\
	void internalExecute(::Firebird::IStatus* status, ::Firebird::ExternalContext* context,	\
		::Firebird::ExternalTrigger::Action action, FieldsMessage* oldFields, FieldsMessage* newFields)


#define FB_UDR_INITIALIZE	\
	void initialize(::Firebird::IStatus* status, ExternalContext* context)	\
	{	\
		try	\
		{	\
			internalInitialize(status, context);	\
		}	\
		FB_UDR__CATCH	\
	}	\
	\
	void internalInitialize(::Firebird::IStatus* status, ::Firebird::ExternalContext* context)


#define FB_UDR__CATCH	\
	catch (const ::Firebird::Udr::StatusException& e)	\
	{	\
		e.stuff(status);	\
	}	\
	catch (...)	\
	{	\
		ISC_STATUS statusVector[] = {	\
			isc_arg_gds, isc_random, isc_arg_string, (ISC_STATUS) "Unrecognized C++ exception",	\
			isc_arg_end};	\
		status->set(statusVector);	\
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

	static void check(ISC_STATUS status, const ISC_STATUS* vector)
	{
		if (status == 0)
			return;

		check(vector);
	}

public:
	const ISC_STATUS* getStatusVector() const
	{
		return statusVector;
	}

	void stuff(IStatus* status) const
	{
		status->set(statusVector);
	}

private:
	ISC_STATUS_ARRAY statusVector;
};

class StatusImpl : public IStatus
{
public:
	StatusImpl(IMaster* master)
		: delegate(master->getStatus()),
		  success(true)
	{
	}

	virtual int FB_CARG getVersion()
	{
		return FB_STATUS_VERSION;
	}

	virtual IPluginModule* FB_CARG getModule()
	{
		return NULL;
	}

	virtual void FB_CARG dispose()
	{
		delegate->dispose();
		delete this;
	}

	virtual void FB_CARG set(unsigned int length, const ISC_STATUS* value)
	{
		delegate->set(length, value);
		success = delegate->isSuccess();
	}

	virtual void FB_CARG set(const ISC_STATUS* value)
	{
		delegate->set(value);
		success = delegate->isSuccess();
	}

	virtual void FB_CARG init()
	{
		delegate->init();
		success = true;
	}

	virtual const ISC_STATUS* FB_CARG get() const
	{
		return delegate->get();
	}

	virtual int FB_CARG isSuccess() const
	{
		return success;
	}

public:
	void check()
	{
		if (!success)
			StatusException::check(delegate->get());
	}

private:
	IStatus* delegate;
	bool success;
};


template <typename T> class Procedure;


class Helper
{
public:
	static isc_db_handle getIscDbHandle(ExternalContext* context)
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

	static isc_tr_handle getIscTrHandle(ExternalContext* context)
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
class ResultSet : public ExternalResultSet, public Helper
{
public:
	ResultSet(ExternalContext* aContext, Procedure* aProcedure,
				InMessage* aIn, OutMessage* aOut)
		: context(aContext),
		  procedure(aProcedure),
		  in(aIn),
		  out(aOut)
	{
	}

public:
	virtual void FB_CALL dispose()
	{
		delete static_cast<This*>(this);
	}

protected:
	ExternalContext* const context;
	Procedure* const procedure;
	InMessage* const in;
	OutMessage* const out;
};


// This class is used to fix an apparent bug with clang, where the object is wrongly initialized
// and overwrites the members set in the operator new.
template <typename T>
class Routine : public T
{
public:
	Routine()
	{
	}
};


template <typename This>
class Function : public ExternalFunction, public Helper
{
public:
	virtual void FB_CALL dispose()
	{
		delete static_cast<This*>(this);
	}

	virtual void FB_CALL getCharSet(IStatus* /*status*/, ExternalContext* /*context*/,
		Utf8* /*name*/, uint /*nameSize*/)
	{
	}

	void* operator new(size_t size, const IRoutineMetadata* metadata)
	{
		Function* p = reinterpret_cast<Function*>(::new char[size]);
		p->metadata = metadata;
		return p;
	}

	void operator delete(void* p)
	{
		::delete [] static_cast<char*>(p);
	}

public:
	const IRoutineMetadata* metadata;
};


template <typename This>
class Procedure : public ExternalProcedure, public Helper
{
public:
	virtual void FB_CALL dispose()
	{
		delete static_cast<This*>(this);
	}

	virtual void FB_CALL getCharSet(IStatus* /*status*/, ExternalContext* /*context*/,
		Utf8* /*name*/, uint /*nameSize*/)
	{
	}

	void* operator new(size_t size, const IRoutineMetadata* metadata)
	{
		Procedure* p = reinterpret_cast<Procedure*>(::new char[size]);
		p->metadata = metadata;
		return p;
	}

	void operator delete(void* p)
	{
		::delete [] static_cast<char*>(p);
	}

public:
	const IRoutineMetadata* metadata;
};


template <typename This>
class Trigger : public ExternalTrigger, public Helper
{
public:
	virtual void FB_CALL dispose()
	{
		delete static_cast<This*>(this);
	}

	virtual void FB_CALL getCharSet(IStatus* /*status*/, ExternalContext* /*context*/,
		Utf8* /*name*/, uint /*nameSize*/)
	{
	}

	void* operator new(size_t size, const IRoutineMetadata* metadata)
	{
		Trigger* p = reinterpret_cast<Trigger*>(::new char[size]);
		p->metadata = metadata;
		return p;
	}

	void operator delete(void* p)
	{
		::delete [] static_cast<char*>(p);
	}

public:
	const IRoutineMetadata* metadata;
};


template <typename T> class FunctionFactoryImpl : public FunctionFactory
{
public:
	explicit FunctionFactoryImpl(const char* name)
	{
		fbUdrRegFunction(name, this);
	}

	virtual void setup(IStatus* /*status*/, ExternalContext* /*context*/,
		const IRoutineMetadata* /*metadata*/, IRoutineMessage* in, IRoutineMessage* out)
	{
		setBlr(in, (typename T::InMessage*) 0);
		setBlr(out, (typename T::OutMessage*) 0);
	}

	virtual ExternalFunction* FB_CALL newItem(IStatus* status, ExternalContext* context,
		const IRoutineMetadata* metadata)
	{
		T* obj = new(metadata) Routine<T>;
		obj->initialize(status, context);
		return obj;
	}

private:
	template <typename MessageType> void setBlr(IRoutineMessage* blrMessage, MessageType*)
	{
		unsigned blrLength;
		const unsigned char* blr = MessageType::getBlr(&blrLength);
		blrMessage->set(blr, blrLength, MessageType::getSize());
	}

	void setBlr(IRoutineMessage* /*blrMessage*/, void**)
	{
	}
};


template <typename T> class ProcedureFactoryImpl : public ProcedureFactory
{
public:
	explicit ProcedureFactoryImpl(const char* name)
	{
		fbUdrRegProcedure(name, this);
	}

	virtual void setup(IStatus* /*status*/, ExternalContext* /*context*/,
		const IRoutineMetadata* /*metadata*/, IRoutineMessage* in, IRoutineMessage* out)
	{
		setBlr(in, (typename T::InMessage*) 0);
		setBlr(out, (typename T::OutMessage*) 0);
	}

	virtual ExternalProcedure* FB_CALL newItem(IStatus* status, ExternalContext* context,
		const IRoutineMetadata* metadata)
	{
		T* obj = new(metadata) Routine<T>;
		obj->initialize(status, context);
		return obj;
	}

private:
	template <typename MessageType> void setBlr(IRoutineMessage* blrMessage, MessageType*)
	{
		unsigned blrLength;
		const unsigned char* blr = MessageType::getBlr(&blrLength);
		blrMessage->set(blr, blrLength, MessageType::getSize());
	}

	void setBlr(IRoutineMessage* /*blrMessage*/, void**)
	{
	}
};


template <typename T> class TriggerFactoryImpl : public TriggerFactory
{
public:
	explicit TriggerFactoryImpl(const char* name)
	{
		fbUdrRegTrigger(name, this);
	}

	virtual void setup(IStatus* /*status*/, ExternalContext* /*context*/,
		const IRoutineMetadata* /*metadata*/, ITriggerMessage* fields)
	{
		setBlr(fields, (typename T::FieldsMessage*) 0);
	}

	virtual ExternalTrigger* FB_CALL newItem(IStatus* status, ExternalContext* context,
		const IRoutineMetadata* metadata)
	{
		T* obj = new(metadata) Routine<T>;
		obj->initialize(status, context);
		return obj;
	}

private:
	template <typename MessageType> void setBlr(ITriggerMessage* blrMessage, MessageType*)
	{
		unsigned blrLength, namesCount;
		const unsigned char* blr = MessageType::getBlr(&blrLength);
		const char** names = MessageType::getNames(&namesCount);

		blrMessage->set(blr, blrLength, MessageType::getSize(), names, namesCount);
	}

	void setBlr(ITriggerMessage* /*blrMessage*/, void**)
	{
	}
};


//------------------------------------------------------------------------------
	}	// namespace Udr
}	// namespace Firebird

#endif	// FIREBIRD_UDR_CPP_ENGINE
