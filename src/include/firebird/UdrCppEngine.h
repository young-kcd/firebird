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
	public:

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
	virtual void FB_CALL execute(::Firebird::Error* error, ::Firebird::ExternalContext* context, \
		void* inMsg, void* outMsg)	\
	{	\
		try	\
		{	\
			internalExecute(error, context, (InMessage*) inMsg, (OutMessage*) outMsg);	\
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
	void FB_CALL internalExecute(::Firebird::Error* error, ::Firebird::ExternalContext* context, \
		InMessage* in, OutMessage* out)


#define FB_UDR_BEGIN_PROCEDURE(name)	\
	class FB_UDR_PROCEDURE(name);	\
	\
	::Firebird::Udr::ProcedureFactoryImpl<FB_UDR_PROCEDURE(name)> ProcFactory##name(#name);	\
	\
	class FB_UDR_PROCEDURE(name) : public ::Firebird::Udr::Procedure<FB_UDR_PROCEDURE(name)>	\
	{	\
	public:	\
		typedef FB_UDR_PROCEDURE(name) This;

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
	virtual ::Firebird::ExternalResultSet* FB_CALL open(::Firebird::Error* error, \
		::Firebird::ExternalContext* context, void* inMsg, void* outMsg)	\
	{	\
		try	\
		{	\
			return new ResultSet(error, context, this, (InMessage*) inMsg, (OutMessage*) outMsg);	\
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
		\
		return 0;	\
	}	\
	\
	class ResultSet : public ::Firebird::Udr::ResultSet<ResultSet, This, InMessage, OutMessage>	\
	{	\
	public:	\
		ResultSet(::Firebird::Error* error, ::Firebird::ExternalContext* context,	\
				This* procedure, InMessage* inMsg, OutMessage* outMsg)	\
			: ::Firebird::Udr::ResultSet<ResultSet, This, InMessage, OutMessage>(	\
					context, procedure, inMsg, outMsg)

#define FB_UDR_FETCH_PROCEDURE	\
	virtual bool FB_CALL fetch(::Firebird::Error* error)	\
	{	\
		try	\
		{	\
			return internalFetch(error);	\
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
		\
		return 0;	\
	}	\
	\
	bool FB_CALL internalFetch(::Firebird::Error* error)


#define FB_UDR_BEGIN_TRIGGER(name)	\
	class FB_UDR_TRIGGER(name);	\
	\
	::Firebird::Udr::TriggerFactoryImpl<FB_UDR_TRIGGER(name)> TrigFactory##name(#name);	\
	\
	class FB_UDR_TRIGGER(name) : public ::Firebird::Udr::Trigger<FB_UDR_TRIGGER(name)>	\
	{	\
	public:

#define FB_UDR_END_TRIGGER	\
	};

#define FB_UDR_EXECUTE_DYNAMIC_TRIGGER	\
	FB_UDR_EXECUTE__TRIGGER

#define FB_UDR_EXECUTE__TRIGGER	\
	virtual void FB_CALL execute(::Firebird::Error* error, ::Firebird::ExternalContext* context,	\
		::Firebird::ExternalTrigger::Action action, void* oldMsg, void* newMsg)	\
	{	\
		try	\
		{	\
			internalExecute(error, context, action, oldMsg, newMsg);	\
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
	void FB_CALL internalExecute(::Firebird::Error* error, ::Firebird::ExternalContext* context,	\
		::Firebird::ExternalTrigger::Action action, void* oldMsg, void* newMsg)


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


template <typename T> class Procedure;


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
};


template <typename This, typename Procedure, typename InMessage, typename OutMessage>
class ResultSet : public ExternalResultSet, public Helper
{
public:
	ResultSet(Firebird::ExternalContext* aContext, Procedure* aProcedure,
				InMessage* aIn, OutMessage* aOut)
		: context(aContext),
		  procedure(aProcedure),
		  in(aIn),
		  out(aOut)
	{
	}

public:
	virtual void FB_CALL dispose(Firebird::Error* /*error*/)
	{
		delete static_cast<This*>(this);
	}

protected:
	Firebird::ExternalContext* context;
	Procedure* procedure;
	InMessage* in;
	OutMessage* out;
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
	virtual void FB_CALL dispose(Error* /*error*/)
	{
		delete static_cast<This*>(this);
	}

	virtual void FB_CALL getCharSet(Error* /*error*/, ExternalContext* /*context*/,
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
	virtual void FB_CALL dispose(Error* /*error*/)
	{
		delete static_cast<This*>(this);
	}

	virtual void FB_CALL getCharSet(Error* /*error*/, ExternalContext* /*context*/,
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
	virtual void FB_CALL dispose(Error* /*error*/)
	{
		delete static_cast<This*>(this);
	}

	virtual void FB_CALL getCharSet(Error* /*error*/, ExternalContext* /*context*/,
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

	virtual void setup(const IRoutineMetadata* /*metadata*/, BlrMessage* inBlr, BlrMessage* outBlr)
	{
		setBlr(inBlr, (typename T::InMessage*) 0);
		setBlr(outBlr, (typename T::OutMessage*) 0);
	}

	virtual ExternalFunction* FB_CALL newItem(const IRoutineMetadata* metadata)
	{
		return new(metadata) Routine<T>;
	}

private:
	template <typename MessageType> void setBlr(BlrMessage* blrMessage, MessageType*)
	{
		blrMessage->blr = MessageType::getBlr(&blrMessage->blrLength);
		blrMessage->bufferLength = MessageType::getSize();
	}

	void setBlr(BlrMessage* blrMessage, void**)
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

	virtual void setup(const IRoutineMetadata* /*metadata*/, BlrMessage* inBlr, BlrMessage* outBlr)
	{
		setBlr(inBlr, (typename T::InMessage*) 0);
		setBlr(outBlr, (typename T::OutMessage*) 0);
	}

	virtual ExternalProcedure* FB_CALL newItem(const IRoutineMetadata* metadata)
	{
		return new(metadata) Routine<T>;
	}

private:
	template <typename MessageType> void setBlr(BlrMessage* blrMessage, MessageType*)
	{
		blrMessage->blr = MessageType::getBlr(&blrMessage->blrLength);
		blrMessage->bufferLength = MessageType::getSize();
	}

	void setBlr(BlrMessage* blrMessage, void**)
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

	virtual void setup(const IRoutineMetadata* /*metadata*/)
	{
	}

	virtual ExternalTrigger* FB_CALL newItem(const IRoutineMetadata* metadata)
	{
		return new(metadata) Routine<T>;
	}
};


//------------------------------------------------------------------------------
	}	// namespace Udr
}	// namespace Firebird

#endif	// FIREBIRD_UDR_CPP_ENGINE
