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

#ifndef JRD_VALUES_IMPL_H
#define JRD_VALUES_IMPL_H

#include "FirebirdApi.h"
#include "FirebirdExternalApi.h"
#include "../jrd/ValueImpl.h"
#include "../common/classes/auto.h"

namespace Jrd {


class ValuesImpl : public Firebird::Values, public Firebird::PermanentStorage
{
private:
	// ValuesQueue used for Values composed by independent fields. This is slower.
	class IndividualQueue : public Firebird::ValuesQueue, public Firebird::PermanentStorage
	{
	public:
		IndividualQueue(Firebird::MemoryPool& p, Firebird::Error* error, ValuesImpl* aValues);
		virtual ~IndividualQueue();

	public:
		virtual void FB_CALL dispose(Firebird::Error* /*error*/)
		{
			delete this;
		}

		virtual void FB_CALL enqueue(Firebird::Error* error);
		virtual bool FB_CALL dequeue(Firebird::Error* error);

	private:
		ValuesImpl* values;
		Firebird::uint valueCount;
		Firebird::uint recordSize;
		Firebird::Array<UCHAR*> records;
		Firebird::uint enqueuePos;
		Firebird::uint recordNumber;
		Firebird::uint nullsStart;
	};

	// ValuesQueue used for Values composed by fields of a message. This is faster.
	class MsgQueue : public Firebird::ValuesQueue, public Firebird::PermanentStorage
	{
	public:
		MsgQueue(Firebird::MemoryPool& p, Firebird::Error* error, UCHAR* aMsg, unsigned aMsgLength);
		virtual ~MsgQueue();

	public:
		virtual void FB_CALL dispose(Firebird::Error* /*error*/)
		{
			delete this;
		}

		virtual void FB_CALL enqueue(Firebird::Error* error);
		virtual bool FB_CALL dequeue(Firebird::Error* error);

	private:
		UCHAR* msg;
		unsigned msgLength;
		Firebird::Array<UCHAR*> records;
		Firebird::uint enqueuePos;
		Firebird::uint recordNumber;
	};

public:
	ValuesImpl(Firebird::MemoryPool& p, Firebird::uint aCount)
		: PermanentStorage(p),
		  msg(NULL),
		  msgLength(0),
		  valuesArray(p),
		  count(aCount)
	{
		while (aCount-- > 0)
			valuesArray.add();

		values = &valuesArray;
	}

	ValuesImpl(Firebird::MemoryPool& p, const Format* format, UCHAR* aMsg,
			const Firebird::Array<NestConst<Parameter> >& parameters)
		: PermanentStorage(p),
		  msg(aMsg),
		  msgLength(format->fmt_length),
		  valuesArray(p),
		  count(format->fmt_count / 2)
	{
		for (unsigned i = 0; i < count; ++i)
		{
			const Parameter* parameter = parameters[i];
			valuesArray.add().make(format, i, msg, parameter->prm_name, parameter->prm_nullable);
		}

		values = &valuesArray;
	}

	virtual ~ValuesImpl()
	{
	}

private:
	// copying is prohibited
	ValuesImpl(const ValuesImpl&);
	ValuesImpl& operator =(const ValuesImpl&);

public:
	Jrd::ValueImpl* getValue(Firebird::uint index) const
	{
		fb_assert(index >= 1 && index <= count);
		return values->getPointer(index - 1);
	}

	void setNull()
	{
		for (unsigned i = 0; i < count; ++i)
			(*values)[i].setNull();
	}

public:
	virtual Firebird::uint FB_CALL getCount() const
	{
		return count;
	}

	virtual Firebird::uint FB_CALL getIndexByName(Firebird::Error* error, const char* name) const;

	virtual Jrd::ValueImpl* FB_CALL getValue(Firebird::Error* error, Firebird::uint index) const
	{
		if (index >= 1 && index <= count)
			return getValue(index);

		error->addCode(isc_invalid_index_val);
		return NULL;
	}

	virtual Firebird::Value* FB_CALL getValueByName(Firebird::Error* error,
		const char* name) const;

	virtual Firebird::ValuesQueue* FB_CALL createQueue(Firebird::Error* error);

private:
	UCHAR* msg;
	unsigned msgLength;
	Firebird::ObjectsArray<ValueImpl> valuesArray;
	Firebird::ObjectsArray<ValueImpl>* values;
	Firebird::uint count;
};


}	// namespace Jrd

#endif	// JRD_VALUES_IMPL_H
