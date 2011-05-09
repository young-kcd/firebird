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
#include "../jrd/ValueImpl.h"
#include "../jrd/ErrorImpl.h"
#include "../jrd/intl_classes.h"
#include "../jrd/DataTypeUtil.h"
#include "../jrd/intl_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/Attachment.h"

using namespace Firebird;
using Firebird::uint;

namespace Jrd {


USHORT ValueMover::getClientCharSet() const
{
	thread_db* tdbb = getThreadData();
	return tdbb->getAttachment()->att_charset;
}


void ValueMover::getClientCharSetName(Firebird::MetaName& name) const
{
	thread_db* tdbb = getThreadData();
	Attachment::SyncGuard attGuard(tdbb->getAttachment());

	CharSet* cs = INTL_charset_lookup(tdbb, tdbb->getAttachment()->att_charset);
	name = cs->getName();
}


int ValueMover::getMaxBytesPerChar(USHORT charSet) const
{
	thread_db* tdbb = getThreadData();
	Attachment::SyncGuard attGuard(tdbb->getAttachment());

	return DataTypeUtil(tdbb).maxBytesPerChar(charSet);
}


const char* ValueMover::getString(const ValueImpl* value, const dsc* desc, MoveBuffer& buffer,
	uint* strLength, bool* isNull) const
{
	if (value->isNull())
	{
		if (isNull)
			*isNull = true;

		return NULL;
	}

	if (isNull)
		*isNull = false;

	thread_db* tdbb = getThreadData();
	Attachment::SyncGuard attGuard(tdbb->getAttachment());
	USHORT charSet = tdbb->getAttachment()->att_charset;

	UCHAR* temp = NULL;
	int len = MOV_make_string2(tdbb, desc, charSet, &temp, buffer, false);

	if (temp == buffer.begin())
		buffer.resize(len + 1);
	else
		memcpy(buffer.getBuffer(len + 1), temp, len);

	buffer[len] = '\0';

	if (strLength)
		*strLength = len;

	return (const char*) buffer.begin();
}


void ValueMover::getValue(const ValueImpl* value, const dsc* desc, dsc* target, bool* isNull) const
{
	if (value->isNull())
	{
		if (isNull)
			*isNull = true;
		else
			status_exception::raise(Arg::Gds(isc_unexpected_null));
	}
	else
	{
		if (isNull)
			*isNull = false;

		thread_db* tdbb = getThreadData();
		Attachment::SyncGuard attGuard(tdbb->getAttachment());

		MOV_move(tdbb, const_cast<dsc*>(desc), target);
	}
}


void ValueMover::setValue(dsc* desc, USHORT* nullFlag, dsc* from) const
{
	thread_db* tdbb = getThreadData();
	Attachment::SyncGuard attGuard(tdbb->getAttachment());

	MOV_move(tdbb, from, desc);
	*nullFlag &= ~DSC_null;
}


//-------------------------------------


ValueImpl::ValueImpl(MemoryPool& p, const dsc* aDesc, const Firebird::MetaName& aName, bool aNullable)
	: PermanentStorage(p),
	  name(p),
	  fieldNumber(0),
	  buffer(p),
	  charSetName(p)
{
	make(aDesc, aName, aNullable);
}


ValueImpl::ValueImpl(MemoryPool& p, const Format* format, unsigned index, UCHAR* msg,
		const Firebird::MetaName& aName, bool aNullable)
	: PermanentStorage(p),
	  name(p),
	  fieldNumber(0),
	  buffer(p),
	  charSetName(p)
{
	make(format, index, msg, aName, aNullable);
}


ValueImpl::ValueImpl(MemoryPool& p)
	: PermanentStorage(p),
	  nullFlag(NULL),
	  name(p),
	  buffer(p)
{
	desc.clear();
}


void ValueImpl::make(const dsc* aDesc, const Firebird::MetaName& aName, bool aNullable,
	USHORT aFieldNumber)
{
	desc = *aDesc;
	nullFlag = &desc.dsc_flags;

	name = aName;
	nullable = aNullable;
	fieldNumber = aFieldNumber;
}


void ValueImpl::make(const Format* format, unsigned index, UCHAR* msg,
	const Firebird::MetaName& aName, bool aNullable, USHORT aFieldNumber)
{
	fb_assert(index * 2 < format->fmt_count);

	desc = format->fmt_desc[index * 2];
	desc.dsc_address = msg + (IPTR) desc.dsc_address;

	const dsc* nullDesc = &format->fmt_desc[index * 2 + 1];
	fb_assert(nullDesc->dsc_dtype == dtype_short);
	nullFlag = (USHORT*) (msg + (IPTR) nullDesc->dsc_address);

	name = aName;
	nullable = aNullable;
	fieldNumber = aFieldNumber;
}


const char* FB_CALL ValueImpl::getName(Error* /*error*/) const
{
	return name.c_str();
}


Value::Type FB_CALL ValueImpl::getType(Error* error) const
{
	switch (desc.dsc_dtype)
	{
		case dtype_byte:
		case dtype_short:
			return TYPE_SMALLINT;

		case dtype_long:
			return TYPE_INTEGER;

		case dtype_int64:
			return TYPE_BIGINT;

		case dtype_real:
		case dtype_double:
		case dtype_d_float:
			return TYPE_DOUBLE;

		case dtype_text:
			return TYPE_CHAR;

		case dtype_cstring:
		case dtype_varying:
			return TYPE_VARCHAR;

		case dtype_blob:
		case dtype_quad:
			return TYPE_BLOB;

		case dtype_sql_date:
			return TYPE_DATE;

		case dtype_sql_time:
			return TYPE_TIME;

		case dtype_timestamp:
			return TYPE_TIMESTAMP;

		default:
			ISC_STATUS status[] = {isc_arg_gds, isc_wish_list, 0};
			ErrorImpl::statusVectorToError(status, error);
			return (Type) 0;
	}
}


const char* FB_CALL ValueImpl::getCharSet(Error* /*error*/) const
{
	if ((desc.isText() || desc.isBlob()) && charSetName.isEmpty())
		mover.getClientCharSetName(charSetName);

	return charSetName.c_str();
}


int FB_CALL ValueImpl::getSubType(Error* /*error*/) const
{
	if (desc.isBlob())
		return desc.getBlobSubType();
	return 0;
}


int FB_CALL ValueImpl::getPrecision(Error* error) const
{
	try
	{
		if (desc.isText())
			return desc.getStringLength() / mover.getMaxBytesPerChar(desc.getCharSet());

		return 0;


	}
	catch (const Exception& e)
	{
		ErrorImpl::exceptionToError(e, error);
	}

	return 0;
}


int FB_CALL ValueImpl::getScale(Error* /*error*/) const
{
	return desc.dsc_scale;
}


bool FB_CALL ValueImpl::isNullable(Error* /*error*/) const
{
	return nullable;
}


void FB_CALL ValueImpl::setNull(Error* error)
{
	if (isNullable(error))
		setNull();
	else
	{
		string text;
		text.printf("parameter %s", name.c_str());

		ISC_STATUS status[] = {
			isc_arg_gds, isc_not_valid_for,
			isc_arg_string, (ISC_STATUS) text.c_str(),
			isc_arg_string, (ISC_STATUS) NULL_STRING_MARK,
			0};
		ErrorImpl::statusVectorToError(status, error);
	}
}


void FB_CALL ValueImpl::copyFrom(Error* error, const Value* from)
{
	if (from->isNull())
		setNull(error);
	else
	{
		switch (from->getType(error))
		{
			case TYPE_SMALLINT:
			case TYPE_INTEGER:
			case TYPE_BIGINT:
			{
				DelegateError err(error);
				int scale = from->getScale(error);
				int64 value = from->getBigInt(&err, scale);
				if (!err.isRaised())
					setBigInt(error, value, scale);
				break;
			}

			case TYPE_DOUBLE:
			{
				DelegateError err(error);
				double value = from->getDouble(&err);
				if (!err.isRaised())
					setDouble(error, value);
				break;
			}

			case TYPE_BLOB:
			{
				DelegateError err(error);
				int64 id = from->getBlobId(&err);
				if (!err.isRaised())
					setBlobId(error, id);
				break;
			}

			case TYPE_DATE:
			{
				DelegateError err(error);
				Date value;
				from->getDate(&err, &value);
				if (!err.isRaised())
					setDate(error, &value);
				break;
			}

			case TYPE_TIME:
			{
				DelegateError err(error);
				Time value;
				from->getTime(&err, &value);
				if (!err.isRaised())
					setTime(error, &value);
				break;
			}

			case TYPE_TIMESTAMP:
			{
				DelegateError err(error);
				DateTime value;
				from->getTimeStamp(&err, &value);
				if (!err.isRaised())
					setTimeStamp(error, &value);
				break;
			}

			case TYPE_CHAR:
			case TYPE_VARCHAR:
			default:
			{
				uint len;
				const char* p = from->getString(error, &len);
				if (p)
					setString(error, p, len);
				break;
			}
		}
	}
}


int16 FB_CALL ValueImpl::getSmallInt(Error* error, int scale, bool* isNull) const
{
	SSHORT value = 0;
	dsc target;
	target.makeShort(scale, &value);
	getValue(error, &target, isNull);
	return value;
}


void FB_CALL ValueImpl::setSmallInt(Error* error, int16 value, int scale)
{
	SSHORT val = value;
	dsc from;
	from.makeShort(scale, &val);
	setValue(error, &from);
}


int32 FB_CALL ValueImpl::getInt(Error* error, int scale, bool* isNull) const
{
	SLONG value = 0;
	dsc target;
	target.makeLong(scale, &value);
	getValue(error, &target, isNull);
	return value;
}


void FB_CALL ValueImpl::setInt(Error* error, int32 value, int scale)
{
	SLONG val = value;
	dsc from;
	from.makeLong(scale, &val);
	setValue(error, &from);
}


int64 FB_CALL ValueImpl::getBigInt(Error* error, int scale, bool* isNull) const
{
	SINT64 value = 0;
	dsc target;
	target.makeInt64(scale, &value);
	getValue(error, &target, isNull);
	return value;
}


void FB_CALL ValueImpl::setBigInt(Error* error, int64 value, int scale)
{
	dsc from;
	from.makeInt64(scale, &value);
	setValue(error, &from);
}


double FB_CALL ValueImpl::getDouble(Error* error, bool* isNull) const
{
	double value = 0;
	dsc target;
	target.makeDouble(&value);
	getValue(error, &target, isNull);
	return value;
}


void FB_CALL ValueImpl::setDouble(Error* error, double value)
{
	dsc from;
	from.makeDouble(&value);
	setValue(error, &from);
}


const char* FB_CALL ValueImpl::getString(Error* /*error*/, uint* strLength, bool* isNull) const
{
	return mover.getString(this, &desc, buffer, strLength, isNull);
}


void FB_CALL ValueImpl::setString(Error* error, const char* str, uint strLength)
{
	dsc from;
	from.makeText(strLength, mover.getClientCharSet(), (UCHAR*) str);
	setValue(error, &from);
}


Firebird::int64 FB_CALL ValueImpl::getBlobId(Error* error, bool* isNull) const
{
	if (!desc.isBlob())
	{
		Arg::Gds vector(isc_type_notcompat_blob);
		vector << desc.typeToText();
		ErrorImpl::statusVectorToError(vector.value(), error);
		return 0;
	}

	int64 blobId;

	if (this->isNull())
		blobId = 0;
	else
	{
		ISC_QUAD quad = *(ISC_QUAD*) desc.dsc_address;
		blobId = quad.gds_quad_low | ((int64) quad.gds_quad_high << 32);
	}

	if (isNull)
		*isNull = blobId == 0;

	return blobId;
}


void FB_CALL ValueImpl::setBlobId(Error* error, Firebird::int64 value)
{
	bid bid;
	bid.bid_quad.bid_quad_low = value;
	bid.bid_quad.bid_quad_high = value >> 32;

	dsc from;
	from.makeBlob(desc.getBlobSubType(), mover.getClientCharSet(), reinterpret_cast<ISC_QUAD*>(&bid));

	setValue(error, &from);
}


void FB_CALL ValueImpl::getDate(Error* error, Date* value, bool* isNull) const
{
	DelegateError err(error);

	GDS_DATE gdsDate;
	dsc target;
	target.makeDate(&gdsDate);
	getValue(&err, &target, isNull);

	if (!err.isRaised())
	{
		tm t;
		TimeStamp::decode_date(gdsDate, &t);

		value->year = t.tm_year + 1900;
		value->month = t.tm_mon + 1;
		value->day = t.tm_mday;
	}
}


void FB_CALL ValueImpl::setDate(Error* error, const Date* value)
{
	tm t;
	t.tm_year = value->year - 1900;
	t.tm_mon = value->month - 1;
	t.tm_mday = value->day;

	GDS_DATE gdsDate = TimeStamp::encode_date(&t);

	tm t2;
	TimeStamp::decode_date(gdsDate, &t2);

	if (t2.tm_year != t.tm_year || t2.tm_mon != t.tm_mon || t2.tm_mday != t.tm_mday)
	{
		Arg::Gds vector(isc_invalid_date_val);
		ErrorImpl::statusVectorToError(vector.value(), error);
		return;
	}

	dsc from;
	from.makeDate(&gdsDate);
	setValue(error, &from);
}


void FB_CALL ValueImpl::getTime(Error* error, Time* value, bool* isNull) const
{
	DelegateError err(error);

	GDS_TIME gdsTime;
	dsc target;
	target.makeTime(&gdsTime);
	getValue(&err, &target, isNull);

	if (!err.isRaised())
	{
		TimeStamp::decode_time(gdsTime, &value->hours, &value->minutes,
			&value->seconds, &value->fractions);
	}
}


void FB_CALL ValueImpl::setTime(Error* error, const Time* value)
{
	GDS_TIME gdsTime;
	gdsTime = TimeStamp::encode_time(value->hours, value->minutes, value->seconds, value->fractions);

	Time t2;
	TimeStamp::decode_time(gdsTime, &t2.hours, &t2.minutes, &t2.seconds, &t2.fractions);
	if (t2.hours != value->hours || t2.minutes != value->minutes ||
		t2.seconds != value->seconds || t2.fractions != value->fractions)
	{
		Arg::Gds vector(isc_invalid_time_val);
		ErrorImpl::statusVectorToError(vector.value(), error);
		return;
	}

	dsc from;
	from.makeTime(&gdsTime);
	setValue(error, &from);
}


void FB_CALL ValueImpl::getTimeStamp(Error* error, DateTime* value, bool* isNull) const
{
	DelegateError err(error);

	GDS_TIMESTAMP gdsTimeStamp;
	dsc target;
	target.makeTimestamp(&gdsTimeStamp);
	getValue(&err, &target, isNull);

	if (!err.isRaised())
	{
		tm t;
		TimeStamp::decode_date(gdsTimeStamp.timestamp_date, &t);
		TimeStamp::decode_time(gdsTimeStamp.timestamp_time, &value->time.hours,
			&value->time.minutes, &value->time.seconds, &value->time.fractions);

		value->date.year = t.tm_year + 1900;
		value->date.month = t.tm_mon + 1;
		value->date.day = t.tm_mday;
	}
}


void FB_CALL ValueImpl::setTimeStamp(Error* error, const DateTime* value)
{
	tm t;
	t.tm_year = value->date.year - 1900;
	t.tm_mon = value->date.month - 1;
	t.tm_mday = value->date.day;

	GDS_TIMESTAMP gdsTimeStamp;
	gdsTimeStamp.timestamp_date = TimeStamp::encode_date(&t);
	gdsTimeStamp.timestamp_time = TimeStamp::encode_time(value->time.hours,
		value->time.minutes, value->time.seconds, value->time.fractions);

	DateTime timeStamp2;
	TimeStamp::decode_time(gdsTimeStamp.timestamp_time, &timeStamp2.time.hours,
		&timeStamp2.time.minutes, &timeStamp2.time.seconds, &timeStamp2.time.fractions);

	tm t2;
	//TimeStamp(gdsTimeStamp).decode(&t2);
	TimeStamp::decode_date(gdsTimeStamp.timestamp_date, &t2);

	if (timeStamp2.time.hours != value->time.hours ||
		timeStamp2.time.minutes != value->time.minutes ||
		timeStamp2.time.seconds != value->time.seconds ||
		timeStamp2.time.fractions != value->time.fractions ||
		t2.tm_year != t.tm_year ||
		t2.tm_mon != t.tm_mon ||
		t2.tm_mday != t.tm_mday)
	{
		Arg::Gds vector(isc_invalid_timestamp_val);
		ErrorImpl::statusVectorToError(vector.value(), error);
		return;
	}

	dsc from;
	from.makeTimestamp(&gdsTimeStamp);
	setValue(error, &from);
}


}	// namespace Jrd
