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

#ifndef FIREBIRD_API_H
#define FIREBIRD_API_H

#ifdef __GNUC__
#	pragma GCC system_header	// disable warning about non-existent virtual destructor
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#	define FB_CALL __stdcall
#else
#	define FB_CALL
#endif

#define FB_NULL	0L


namespace Firebird {


typedef unsigned int uint;
typedef short int16;
typedef int int32;

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) && !defined(__GNUC__)
typedef __int64 int64;
#else
typedef long long int int64;
#endif

typedef char Utf8;		// Utf8* used as nul-terminated string

// ISC compatible handle
#if defined(_LP64) || defined(__LP64__) || defined(__arch64__) || defined(_WIN64)
typedef unsigned int Handle;
#else
typedef void* Handle;
#endif


struct Date
{
	int year;
	int month;
	int day;
};


struct Time
{
	int hours;
	int minutes;
	int seconds;
	int fractions;
};


struct DateTime	//// FIXME: rename to TimeStamp
{
	Date date;
	Time time;
};


class Error
{
public:
	virtual bool FB_CALL addCode(int32 code) = 0;
	virtual bool FB_CALL addString(const char* str, uint strLength) = 0;
};


// Interface used in cases which an instance is created by one layer and released by another one.
class Disposable
{
public:
	// Disposes the object.
	virtual void FB_CALL dispose(Error* error) = 0;
};


// Represents a parameter or column.
class Value
{
public:
	// data types
	enum Type
	{
		TYPE_SMALLINT = 1,
		TYPE_INTEGER,
		TYPE_BIGINT,
		TYPE_DOUBLE,
		TYPE_CHAR,
		TYPE_VARCHAR,
		TYPE_BLOB,
		TYPE_DATE,
		TYPE_TIME,
		TYPE_TIMESTAMP
	};

public:
	// Get parameter or column name.
	virtual const char* FB_CALL getName(Error* error) const = 0;

	virtual Type FB_CALL getType(Error* error) const = 0;
	virtual const char* FB_CALL getCharSet(Error* error) const = 0;

	// Get BLOB sub-type.
	virtual int FB_CALL getSubType(Error* error) const = 0;

	// Get numeric precision or maximum string length.
	virtual int FB_CALL getPrecision(Error* error) const = 0;

	virtual int FB_CALL getScale(Error* error) const = 0;
	virtual bool FB_CALL isNullable(Error* error) const = 0;

	virtual bool FB_CALL isNull() const = 0;
	virtual void FB_CALL setNull(Error* error) = 0;

	virtual void FB_CALL copyFrom(Error* error, const Value* from) = 0;

	virtual int16 FB_CALL getSmallInt(Error* error, int scale = 0, bool* isNull = FB_NULL) const = 0;
	virtual void FB_CALL setSmallInt(Error* error, int16 value, int scale = 0) = 0;

	virtual int32 FB_CALL getInt(Error* error, int scale = 0, bool* isNull = FB_NULL) const = 0;
	virtual void FB_CALL setInt(Error* error, int32 value, int scale = 0) = 0;

	virtual int64 FB_CALL getBigInt(Error* error, int scale = 0, bool* isNull = FB_NULL) const = 0;
	virtual void FB_CALL setBigInt(Error* error, int64 value, int scale = 0) = 0;

	virtual double FB_CALL getDouble(Error* error, bool* isNull = FB_NULL) const = 0;
	virtual void FB_CALL setDouble(Error* error, double value) = 0;

	virtual const char* FB_CALL getString(Error* error, uint* strLength = FB_NULL,
		bool* isNull = FB_NULL) const = 0;
	virtual void FB_CALL setString(Error* error, const char* str, uint strLength) = 0;

	virtual int64 FB_CALL getBlobId(Error* error, bool* isNull = FB_NULL) const = 0;
	virtual void FB_CALL setBlobId(Error* error, int64 value) = 0;

	virtual void FB_CALL getDate(Error* error, Date* value, bool* isNull = FB_NULL) const = 0;
	virtual void FB_CALL setDate(Error* error, const Date* value) = 0;

	virtual void FB_CALL getTime(Error* error, Time* value, bool* isNull = FB_NULL) const = 0;
	virtual void FB_CALL setTime(Error* error, const Time* value) = 0;

	virtual void FB_CALL getTimeStamp(Error* error, DateTime* value, bool* isNull = FB_NULL) const = 0;
	virtual void FB_CALL setTimeStamp(Error* error, const DateTime* value) = 0;
};


// A queue associated with a Values. Could be used for batching processing.
class ValuesQueue : public Disposable
{
public:
	virtual void FB_CALL enqueue(Error* error) = 0;	// Enqueue the current Values.
	virtual bool FB_CALL dequeue(Error* error) = 0;	// Dequeue in Values.
};


// Represents a group of parameters or columns.
class Values
{
public:
	virtual uint FB_CALL getCount() const = 0;
	virtual uint FB_CALL getIndexByName(Error* error, const char* name) const = 0;

	// Get a given value. The first value is at index 1.
	virtual Value* FB_CALL getValue(Error* error, uint index) const = 0;

	virtual Value* FB_CALL getValueByName(Error* error, const char* name) const = 0;

	// Creates a queue associated with this Values.
	virtual ValuesQueue* FB_CALL createQueue(Error* error) = 0;
};


}	// namespace Firebird


#endif	// FIREBIRD_API_H
