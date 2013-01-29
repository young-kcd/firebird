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
 *  Copyright (c) 2011 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef FIREBIRD_MESSAGE_H
#define FIREBIRD_MESSAGE_H

#include "ibase.h"
#include "firebird/impl/boost/preprocessor/seq/for_each_i.hpp"
#include <assert.h>
#include <time.h>
#include <string.h>

#define FB_MESSAGE(name, fields)	\
	struct name	\
	{	\
		FB_MESSAGE_I(name, 2, FB_BOOST_PP_CAT(FB_MESSAGE_X fields, 0))	\
	}

#define FB_MESSAGE_X(x, y) ((x, y)) FB_MESSAGE_Y
#define FB_MESSAGE_Y(x, y) ((x, y)) FB_MESSAGE_X
#define FB_MESSAGE_X0
#define FB_MESSAGE_Y0

#define FB_TRIGGER_MESSAGE(name, fields)	\
	struct name	\
	{	\
		FB_MESSAGE_I(name, 3, FB_BOOST_PP_CAT(FB_TRIGGER_MESSAGE_X fields, 0))	\
		FB_TRIGGER_MESSAGE_NAMES_I(name, 3, FB_BOOST_PP_CAT(FB_TRIGGER_MESSAGE_NAMES_X fields, 0))	\
	}

#define FB_TRIGGER_MESSAGE_X(x, y, z) ((x, y, z)) FB_TRIGGER_MESSAGE_Y
#define FB_TRIGGER_MESSAGE_Y(x, y, z) ((x, y, z)) FB_TRIGGER_MESSAGE_X
#define FB_TRIGGER_MESSAGE_X0
#define FB_TRIGGER_MESSAGE_Y0

#define FB_MESSAGE_I(name, size, fields)	\
	static const unsigned char* getBlr(unsigned* length)	\
	{	\
		static const unsigned char blr[] = {	\
			blr_version5,	\
			blr_begin,	\
			blr_message, 0,	\
				(2 * (FB_BOOST_PP_SEQ_SIZE(fields))) & 0xFF,	\
				(2 * (FB_BOOST_PP_SEQ_SIZE(fields))) >> 8,	\
			FB_BOOST_PP_SEQ_FOR_EACH_I(FB_MESSAGE_BLR, size, fields)	\
			blr_end,	\
			blr_eoc	\
		};	\
		*length = sizeof(blr);	\
		return blr;	\
	}	\
	\
	static unsigned getSize()	\
	{	\
		return (unsigned)(size_t) (&((name*) 0)->FB_BOOST_PP_CAT(	\
			FB_BOOST_PP_TUPLE_ELEM(size, 1,	\
				FB_BOOST_PP_SEQ_ELEM(FB_BOOST_PP_DEC(FB_BOOST_PP_SEQ_SIZE(fields)), fields)),	\
			Null) - 0) + sizeof(ISC_SHORT);	\
	}	\
	\
	void clear()	\
	{	\
		memset(this, 0, sizeof(*this));	\
	}	\
	\
	FB_BOOST_PP_SEQ_FOR_EACH_I(FB_MESSAGE_FIELD, size, fields)

#define FB_MESSAGE_FIELD(r, _, i, xy)	\
	FB_BOOST_PP_CAT(FB_TYPE_, FB_BOOST_PP_TUPLE_ELEM(_, 0, xy)) FB_BOOST_PP_TUPLE_ELEM(_, 1, xy);	\
	ISC_SHORT FB_BOOST_PP_CAT(FB_BOOST_PP_TUPLE_ELEM(_, 1, xy), Null);

#define FB_MESSAGE_BLR(r, _, i, xy)	\
	FB_BOOST_PP_CAT(FB_BLR_, FB_BOOST_PP_TUPLE_ELEM(_, 0, xy)),	\
	FB_BLR_FB_SMALLINT,

#define FB_BLR_FB_SCALED_SMALLINT(scale)	blr_short, (scale)
#define FB_BLR_FB_SCALED_INTEGER(scale)		blr_long, (scale)
#define FB_BLR_FB_SCALED_BIGINT(scale)		blr_int64, (scale)
#define FB_BLR_FB_SMALLINT					FB_BLR_FB_SCALED_SMALLINT(0)
#define FB_BLR_FB_INTEGER					FB_BLR_FB_SCALED_INTEGER(0)
#define FB_BLR_FB_BIGINT					FB_BLR_FB_SCALED_BIGINT(0)
#define FB_BLR_FB_FLOAT						blr_float
#define FB_BLR_FB_DOUBLE					blr_double
#define FB_BLR_FB_BLOB						blr_blob2, 0, 0, 0, 0
#define FB_BLR_FB_BOOLEAN					blr_bool
#define FB_BLR_FB_DATE						blr_sql_date
#define FB_BLR_FB_TIME						blr_sql_time
#define FB_BLR_FB_TIMESTAMP					blr_timestamp
#define FB_BLR_FB_CHAR(len)					blr_text2, 0, 0, (len) & 0xFF, (len) >> 8
#define FB_BLR_FB_VARCHAR(len)				blr_varying2, 0, 0, (len) & 0xFF, (len) >> 8

#define FB_TYPE_FB_SCALED_SMALLINT(x)	ISC_SHORT
#define FB_TYPE_FB_SCALED_INTEGER(x)	ISC_LONG
#define FB_TYPE_FB_SCALED_BIGINT(x)		ISC_INT64
#define FB_TYPE_FB_SMALLINT				ISC_SHORT
#define FB_TYPE_FB_INTEGER				ISC_LONG
#define FB_TYPE_FB_BIGINT				ISC_INT64
#define FB_TYPE_FB_FLOAT				float
#define FB_TYPE_FB_DOUBLE				double
#define FB_TYPE_FB_BLOB					ISC_QUAD
#define FB_TYPE_FB_BOOLEAN				ISC_UCHAR
#define FB_TYPE_FB_DATE					::Firebird::FbDate
#define FB_TYPE_FB_TIME					::Firebird::FbTime
#define FB_TYPE_FB_TIMESTAMP			::Firebird::FbTimestamp
#define FB_TYPE_FB_CHAR(len)			::Firebird::FbChar<(len)>
#define FB_TYPE_FB_VARCHAR(len)			::Firebird::FbVarChar<(len)>

#define FB_MESSAGE_DESC(name, fields)	\
	FB_MESSAGE(name, fields);	\
	struct name##Desc : public name	\
	{	\
		::Firebird::FbMessage desc;	\
		\
		name##Desc()	\
		{	\
			desc.blr = getBlr(&desc.blrLength);	\
			desc.buffer = (unsigned char*) this;	\
			desc.bufferLength = getSize();	\
		}	\
	}

#define FB_TRIGGER_MESSAGE_DESC(name, fields)	\
	FB_TRIGGER_MESSAGE(name, fields);	\
	struct name##Desc : public name	\
	{	\
		::Firebird::FbMessage desc;	\
		\
		name##Desc()	\
		{	\
			desc.blr = getBlr(&desc.blrLength);	\
			desc.buffer = (unsigned char*) this;	\
			desc.bufferLength = getSize();	\
		}	\
	}

#define FB_TRIGGER_MESSAGE_NAMES(name, fields)	\
	FB_TRIGGER_MESSAGE_NAMES_I(name, 3, FB_BOOST_PP_CAT(FB_TRIGGER_MESSAGE_NAMES_X fields, 0))

#define FB_TRIGGER_MESSAGE_NAMES_X(x, y, z) ((x, y, z)) FB_TRIGGER_MESSAGE_NAMES_Y
#define FB_TRIGGER_MESSAGE_NAMES_Y(x, y, z) ((x, y, z)) FB_TRIGGER_MESSAGE_NAMES_X
#define FB_TRIGGER_MESSAGE_NAMES_X0
#define FB_TRIGGER_MESSAGE_NAMES_Y0

#define FB_TRIGGER_MESSAGE_NAMES_I(name, size, fields)	\
	static const char** getNames(unsigned* count)	\
	{	\
		*count = FB_BOOST_PP_SEQ_SIZE(fields);	\
		\
		static const char* names[] = {	\
			FB_BOOST_PP_SEQ_FOR_EACH_I(FB_TRIGGER_MESSAGE_NAME, size, fields)	\
			NULL	\
		};	\
		\
		return names;	\
	}

#define FB_TRIGGER_MESSAGE_NAME(r, _, i, xy)	\
	FB_BOOST_PP_TUPLE_ELEM(_, 2, xy),


namespace Firebird {


template <unsigned N>
struct FbChar
{
	char str[N];
};

template <unsigned N>
struct FbVarChar
{
	ISC_USHORT length;
	char str[N];

	void set(const char* s)
	{
		length = strlen(s);
		assert(length <= N);
		memcpy(str, s, length);
	}
};

// This class has memory layout identical to ISC_DATE.
class FbDate
{
public:
	void decode(unsigned* year, unsigned* month, unsigned* day) const
	{
		tm times;
		isc_decode_sql_date(&value, &times);

		if (year)
			*year = times.tm_year + 1900;
		if (month)
			*month = times.tm_mon + 1;
		if (day)
			*day = times.tm_mday;
	}

	unsigned getYear() const
	{
		unsigned year;
		decode(&year, NULL, NULL);
		return year;
	}

	unsigned getMonth() const
	{
		unsigned month;
		decode(NULL, &month, NULL);
		return month;
	}

	unsigned getDay() const
	{
		unsigned day;
		decode(NULL, NULL, &day);
		return day;
	}

	void encode(unsigned year, unsigned month, unsigned day)
	{
		tm times;
		times.tm_year = year - 1900;
		times.tm_mon = month - 1;
		times.tm_mday = day;

		isc_encode_sql_date(&times, &value);
	}

public:
	ISC_DATE value;
};

// This class has memory layout identical to ISC_TIME.
class FbTime
{
public:
	void decode(unsigned* hours, unsigned* minutes, unsigned* seconds, unsigned* fractions) const
	{
		tm times;
		isc_decode_sql_time(&value, &times);

		if (hours)
			*hours = times.tm_hour;
		if (minutes)
			*minutes = times.tm_min;
		if (seconds)
			*seconds = times.tm_sec;
		if (fractions)
			*fractions = value % ISC_TIME_SECONDS_PRECISION;
	}

	unsigned getHours() const
	{
		unsigned hours;
		decode(&hours, NULL, NULL, NULL);
		return hours;
	}

	unsigned getMinutes() const
	{
		unsigned minutes;
		decode(NULL, &minutes, NULL, NULL);
		return minutes;
	}

	unsigned getSeconds() const
	{
		unsigned seconds;
		decode(NULL, NULL, &seconds, NULL);
		return seconds;
	}

	unsigned getFractions() const
	{
		unsigned fractions;
		decode(NULL, NULL, NULL, &fractions);
		return fractions;
	}

	void encode(unsigned hours, unsigned minutes, unsigned seconds, unsigned fractions)
	{
		tm times;
		times.tm_hour = hours;
		times.tm_min = minutes;
		times.tm_sec = seconds;

		isc_encode_sql_time(&times, &value);
		value += fractions;
	}

public:
	ISC_TIME value;
};

// This class has memory layout identical to ISC_TIMESTAMP.
class FbTimestamp
{
public:
	FbDate date;
	FbTime time;
};


}	// namespace Firebird

#endif	// FIREBIRD_MESSAGE_H
