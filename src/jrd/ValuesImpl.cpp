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

#include "firebird.h"
#include "../jrd/ValuesImpl.h"
#include "../jrd/ErrorImpl.h"
#include "../jrd/mov_proto.h"
#include "../jrd/align.h"

using namespace Firebird;
using Firebird::uint;

namespace Jrd {


ValuesImpl::IndividualQueue::IndividualQueue(MemoryPool& p, Error* /*error*/, ValuesImpl* aValues)
	: PermanentStorage(p),
	  values(aValues),
	  valueCount(values->getCount()),
	  recordSize(0),
	  records(p),
	  enqueuePos(0),
	  recordNumber(0)
{
	for (uint i = 1; i <= valueCount; ++i)
	{
		ValueImpl* value = values->getValue(i);
		const dsc& desc = value->getDesc();

		USHORT align = type_alignments[desc.dsc_dtype];
		if (align)
			recordSize = FB_ALIGN(recordSize, align);

		recordSize += desc.dsc_length;
	}

	nullsStart = recordSize;
	recordSize += valueCount / 8 + 1;
}


ValuesImpl::IndividualQueue::~IndividualQueue()
{
	for (Array<UCHAR*>::iterator end = records.end(), i = records.begin(); i != end; ++i)
		delete [] *i;
}


void FB_CALL ValuesImpl::IndividualQueue::enqueue(Error* /*error*/)
{
	/*thread_db* tdbb = */JRD_get_thread_data();
	const size_t recordCount = records.getCount();
	UCHAR* const buffer = (enqueuePos < recordCount ?
		records[enqueuePos] : FB_NEW(getPool()) UCHAR[recordSize]);
	UCHAR* nullsBuffer = buffer + nullsStart;
	uint pos = 0;

	for (uint i = 1; i <= valueCount; ++i)
	{
		ValueImpl* value = values->getValue(i);
		const dsc& desc = value->getDesc();

		USHORT align = type_alignments[desc.dsc_dtype];
		if (align)
			pos = FB_ALIGN(pos, align);

		if (value->isNull())
			nullsBuffer[(i - 1) >> 3] |= 1 << ((i - 1) & 7);
		else
		{
			nullsBuffer[(i - 1) >> 3] &= ~(1 << ((i - 1) & 7));
			memcpy(buffer + pos, desc.dsc_address, desc.dsc_length);
		}

		pos += desc.dsc_length;
	}

	if (enqueuePos++ >= recordCount)
		records.push(buffer);
}


bool FB_CALL ValuesImpl::IndividualQueue::dequeue(Error* /*error*/)
{
	if (recordNumber == enqueuePos)
		return false;

	const UCHAR* buffer = records[recordNumber++];
	const UCHAR* nullsBuffer = buffer + nullsStart;
	uint pos = 0;

	for (uint i = 1; i <= valueCount; ++i)
	{
		ValueImpl* value = values->getValue(i);
		dsc& desc = value->getDesc();

		USHORT align = type_alignments[desc.dsc_dtype];
		if (align)
			pos = FB_ALIGN(pos, align);

		if (nullsBuffer[(i - 1) >> 3] & (1 << ((i - 1) & 7)))
			desc.dsc_flags |= DSC_null;
		else
		{
			memcpy(desc.dsc_address, buffer + pos, desc.dsc_length);
			desc.dsc_flags &= ~DSC_null;
		}

		pos += desc.dsc_length;
	}

	if (recordNumber == enqueuePos)
	{
		recordNumber = 0;
		enqueuePos = 0;
	}

	return true;
}


//---------------------


ValuesImpl::MsgQueue::MsgQueue(MemoryPool& p, Error* /*error*/, UCHAR* aMsg, unsigned aMsgLength)
	: PermanentStorage(p),
	  msg(aMsg),
	  msgLength(aMsgLength),
	  records(p),
	  enqueuePos(0),
	  recordNumber(0)
{
}


ValuesImpl::MsgQueue::~MsgQueue()
{
	for (Array<UCHAR*>::iterator end = records.end(), i = records.begin(); i != end; ++i)
		delete [] *i;
}


void FB_CALL ValuesImpl::MsgQueue::enqueue(Error* /*error*/)
{
	/*thread_db* tdbb = */JRD_get_thread_data();
	const size_t recordCount = records.getCount();
	UCHAR* buffer = (enqueuePos < recordCount ?
		records[enqueuePos] : FB_NEW(getPool()) UCHAR[msgLength]);

	memcpy(buffer, msg, msgLength);

	if (enqueuePos++ >= recordCount)
		records.push(buffer);
}


bool FB_CALL ValuesImpl::MsgQueue::dequeue(Error* /*error*/)
{
	if (recordNumber == enqueuePos)
		return false;

	const UCHAR* buffer = records[recordNumber++];
	memcpy(msg, buffer, msgLength);

	if (recordNumber == enqueuePos)
	{
		recordNumber = 0;
		enqueuePos = 0;
	}

	return true;
}


//---------------------


Firebird::uint FB_CALL ValuesImpl::getIndexByName(Error* error, const char* name) const
{
	const string nameStr(name);

	for (unsigned i = 0; i < count; ++i)
	{
		const char* valName = (*values)[i].getName(error);

		if (valName)
		{
			if (nameStr == valName)
				return i + 1;
		}
		else
			break;
	}

	return 0;
}


Value* FB_CALL ValuesImpl::getValueByName(Error* error, const char* name) const
{
	int index = getIndexByName(error, name);

	if (index > 0)
		return getValue(error, index);

	return NULL;
}


ValuesQueue* FB_CALL ValuesImpl::createQueue(Error* error)
{
	if (msg)
		return new MsgQueue(getPool(), error, msg, msgLength);

	return new IndividualQueue(getPool(), error, this);
}


}	// namespace Jrd
