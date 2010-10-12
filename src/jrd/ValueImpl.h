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

#ifndef JRD_VALUE_IMPL_H
#define JRD_VALUE_IMPL_H

#include "FirebirdApi.h"
#include "FirebirdExternalApi.h"
#include "../jrd/common.h"
#include "../common/dsc.h"
#include "../jrd/jrd.h"
#include "../jrd/ErrorImpl.h"

namespace Jrd {

class ValueImpl;


class ValueMover
{
public:
	USHORT getClientCharSet() const;
	void getClientCharSetName(Firebird::MetaName& name) const;
	int getMaxBytesPerChar(USHORT charSet) const;

	const char* getString(const ValueImpl* value, const dsc* desc, MoveBuffer& buffer,
		Firebird::uint* strLength, bool* isNull) const;

	void getValue(const ValueImpl* value, const dsc* desc, dsc* target, bool* isNull) const;
	void setValue(dsc* desc, USHORT* nullFlag, dsc* from) const;

	static inline thread_db* getThreadData()
	{
		return JRD_get_thread_data();
	}
};


class ValueImpl : public Firebird::Value, Firebird::PermanentStorage
{
public:
	ValueImpl(Firebird::MemoryPool& p, const dsc* aDesc,
		const Firebird::MetaName& aName, bool aNullable);
	ValueImpl(Firebird::MemoryPool& p, const Format* format, unsigned index, UCHAR* msg,
		const Firebird::MetaName& aName, bool aNullable);
	ValueImpl(Firebird::MemoryPool& p);

	virtual ~ValueImpl()
	{
	}

private:
	// copying is prohibited
	ValueImpl(const ValueImpl&);
	ValueImpl& operator =(const ValueImpl&);

public:
	// used in triggers code
	USHORT getFieldNumber() const
	{
		return fieldNumber;
	}

	dsc& getDesc()
	{
		return desc;
	}

	void setNull()
	{
		*nullFlag |= DSC_null;
	}

	void make(const dsc* aDesc, const Firebird::MetaName& aName, bool aNullable,
		USHORT aFieldNumber = 0);
	void make(const Format* format, unsigned index, UCHAR* msg, const Firebird::MetaName& aName,
		bool aNullable, USHORT aFieldNumber = 0);

public:
	virtual const char* FB_CALL getName(Firebird::Error* error) const;
	virtual Type FB_CALL getType(Firebird::Error* error) const;
	virtual const char* FB_CALL getCharSet(Firebird::Error* error) const;
	virtual int FB_CALL getSubType(Firebird::Error* error) const;
	virtual int FB_CALL getPrecision(Firebird::Error* error) const;
	virtual int FB_CALL getScale(Firebird::Error* error) const;
	virtual bool FB_CALL isNullable(Firebird::Error* error) const;

	virtual bool FB_CALL isNull() const
	{
		return *nullFlag & DSC_null;
	}

	virtual void FB_CALL setNull(Firebird::Error* error);

	virtual void FB_CALL copyFrom(Firebird::Error* error, const Firebird::Value* from);

	virtual Firebird::int16 FB_CALL getSmallInt(Firebird::Error* error, int scale = 0,
		bool* isNull = FB_NULL) const;
	virtual void FB_CALL setSmallInt(Firebird::Error* error, Firebird::int16 value, int scale = 0);

	virtual Firebird::int32 FB_CALL getInt(Firebird::Error* error, int scale = 0,
		bool* isNull = FB_NULL) const;
	virtual void FB_CALL setInt(Firebird::Error* error, Firebird::int32 value, int scale = 0);

	virtual Firebird::int64 FB_CALL getBigInt(Firebird::Error* error, int scale = 0,
		bool* isNull = FB_NULL) const;
	virtual void FB_CALL setBigInt(Firebird::Error* error, Firebird::int64 value, int scale = 0);

	virtual double FB_CALL getDouble(Firebird::Error* error, bool* isNull = FB_NULL) const;
	virtual void FB_CALL setDouble(Firebird::Error* error, double value);

	virtual const char* FB_CALL getString(Firebird::Error* error,
		Firebird::uint* strLength = FB_NULL, bool* isNull = FB_NULL) const;
	virtual void FB_CALL setString(Firebird::Error* error, const char* str,
		Firebird::uint strLength);

	virtual Firebird::int64 FB_CALL getBlobId(Firebird::Error* error,
		bool* isNull = FB_NULL) const;
	virtual void FB_CALL setBlobId(Firebird::Error* error, Firebird::int64 value);

	virtual void FB_CALL getDate(Firebird::Error* error, Firebird::Date* value,
		bool* isNull = FB_NULL) const;
	virtual void FB_CALL setDate(Firebird::Error* error, const Firebird::Date* value);

	virtual void FB_CALL getTime(Firebird::Error* error, Firebird::Time* value,
		bool* isNull = FB_NULL) const;
	virtual void FB_CALL setTime(Firebird::Error* error, const Firebird::Time* value);

	virtual void FB_CALL getTimeStamp(Firebird::Error* error, Firebird::DateTime* value,
		bool* isNull = FB_NULL) const;
	virtual void FB_CALL setTimeStamp(Firebird::Error* error, const Firebird::DateTime* value);

private:
	void getValue(Firebird::Error* error, dsc* target, bool* isNull) const
	{
		try
		{
			mover.getValue(this, &desc, target, isNull);
		}
		catch (const Firebird::Exception& ex)
		{
			ErrorImpl::exceptionToError(ex, error);
		}
	}

	void setValue(Firebird::Error* error, dsc* from)
	{
		try
		{
			mover.setValue(&desc, nullFlag, from);
		}
		catch (const Firebird::Exception& ex)
		{
			ErrorImpl::exceptionToError(ex, error);
		}
	}

private:
	dsc desc;
	USHORT* nullFlag;
	Firebird::MetaName name;
	bool nullable;
	USHORT fieldNumber;
	mutable MoveBuffer buffer;
	mutable Firebird::MetaName charSetName;
	ValueMover mover;
};


}	// namespace Jrd

#endif	// JRD_VALUE_IMPL_H
