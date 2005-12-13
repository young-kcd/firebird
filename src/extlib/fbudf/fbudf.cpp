/*
 *
 *     The contents of this file are subject to the Initial
 *     Developer's Public License Version 1.0 (the "License");
 *     you may not use this file except in compliance with the
 *     License. You may obtain a copy of the License at
 *     http://www.ibphoenix.com/idpl.html.
 *
 *     Software distributed under the License is distributed on
 *     an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either
 *     express or implied.  See the License for the specific
 *     language governing rights and limitations under the License.
 *
 *
 *  The Original Code was created by Claudio Valderama C. for IBPhoenix.
 *  The development of the Original Code was sponsored by Craig Leonardi.
 *
 *  Copyright (c) 2001 IBPhoenix
 *  All Rights Reserved.
 *
 * 2002.01.07 Claudio Valderrama: change the impolite way truncate and round work,
 *	make null handling more consistent and add dpower(x,y).
 *	Beware the SQL declaration for those functions has changed.
 * 2002.01.20 Claudio Valderrama: addMonth should work with negative values, too.
 */


// fbudf.cpp : Defines the entry point for the DLL application.
//

/*
CVC: The MS way of doing things puts the includes in stdafx. I expect
that you can continue this trend without problems on other compilers
or platforms. Since I conditioned the Windows-related includes, it should
be easy to add needed headers to stdafx.h after a makefile is built.
*/


#include "stdafx.h"

#ifndef FBUDF_EXPORTS
#define FBUDF_EXPORTS
#endif

#include "fbudf.h"


//Original code for this library was written by Claudio Valderrama
// on July 2001 for IBPhoenix.

// Define this symbol if you want truncate and round to be symmetric wr to zero.
//#define SYMMETRIC_MATH

#if defined (_WIN32)
/*
BOOL APIENTRY DllMain( HANDLE ,//hModule,
					  DWORD  ul_reason_for_call, 
					  LPVOID //lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
*/
#endif



const long seconds_in_day = 86400;
const long tenthmsec_in_day = seconds_in_day * ISC_TIME_SECONDS_PRECISION;
const short varchar_indicator = sizeof(unsigned short);


#ifdef DEV_BUILD
// This function shouldn't be defined in production.
FBUDF_API paramdsc* testreflect(paramdsc* rc)
{
	rc->dsc_address = 0;
	rc->dsc_flags = 1 | 4; //DSC_null | DSC_nullable;
	return rc;
}
#endif

namespace internal
{
	// This definition comes from jrd\val.h and is equivalent to helper
	// functions {get/set}_varchar_len defined below.
	typedef struct vvary {
		unsigned short	vary_length;
		unsigned char	vary_string [1];
	} VVARY;

	/*
	inline short get_varchar_len(char* vchar) 
	{
		return  static_cast<short>((static_cast<short>(vchar[1]) << 8) + vchar[0]);
	}
	*/

	inline short get_varchar_len(unsigned char* vchar) 
	{
		return  static_cast<short>((static_cast<short>(vchar[1]) << 8) + vchar[0]);
	}

	inline void set_varchar_len(char* vchar, short len)
	{
		vchar[1] = static_cast<char>(len >> 8);
		vchar[0] = static_cast<char>(len);
	}
	
	inline void set_varchar_len(unsigned char* vchar, short len)
	{
		vchar[1] = static_cast<unsigned char>(len >> 8);
		vchar[0] = static_cast<unsigned char>(len);
	}

	short get_int_type(const paramdsc* v, ISC_INT64& rc)
	{
		short s = -1;
		switch(v->dsc_dtype)
		{
		case dtype_short:
			rc = *reinterpret_cast<short*>(v->dsc_address);
			s = sizeof(short);
			break;
		case dtype_long:
			rc = *reinterpret_cast<ISC_LONG*>(v->dsc_address);
			s = sizeof(ISC_LONG);
			break;
		case dtype_int64:
			rc = *reinterpret_cast<ISC_INT64*>(v->dsc_address);
			s = sizeof(ISC_INT64);
			break;
		default:
			break;
		}
		return s;
	}

	void set_int_type(paramdsc* v, const ISC_INT64 iv)
	{
		switch(v->dsc_dtype)
		{
		case dtype_short:
			*reinterpret_cast<short*>(v->dsc_address) = static_cast<short>(iv);
			break;
		case dtype_long:
			*reinterpret_cast<ISC_LONG*>(v->dsc_address) = static_cast<ISC_LONG>(iv);
			break;
		case dtype_int64:
			*reinterpret_cast<ISC_INT64*>(v->dsc_address) = iv;
			break;
		}
	}

	short get_double_type(const paramdsc* v, double& rc)
	{
		short s = -1;
		switch(v->dsc_dtype)
		{
		case dtype_real:
			rc = static_cast<double>(*reinterpret_cast<float*>(v->dsc_address));
			s = sizeof(float);
			break;
		case dtype_double:
			rc = *reinterpret_cast<double*>(v->dsc_address);
			s = sizeof(double);
			break;
		default:
			break;
		}
		return s;
	}

	void set_double_type(paramdsc* v, const double iv)
	{
		switch(v->dsc_dtype)
		{
		case dtype_real:
			*reinterpret_cast<float*>(v->dsc_address) = static_cast<float>(iv);
			break;
		case dtype_double:
			*reinterpret_cast<double*>(v->dsc_address) = iv;
			break;
		}
	}

	short get_string_type(const paramdsc* v, unsigned char*& text)
	{
		short len = v->dsc_length;
		switch(v->dsc_dtype)
		{
		case dtype_text:
			text = v->dsc_address;
			break;
		case dtype_cstring:
			text = v->dsc_address;
			if (len && text)
			{
				unsigned char* p = text; //strlen(v->dsc_address);
				while (*p) ++p; // couldn't use strlen!
				if (p - text < len)
					len = static_cast<short>(p - text);
			}
			break;
		case dtype_varying:
			len -= varchar_indicator;
			text = reinterpret_cast<vvary*>(v->dsc_address)->vary_string;
			{
				short x = get_varchar_len(v->dsc_address);
				if (x < len)
					len = x;
			}
			break;
		default:
			len = -1;
			break;
		}
		return len;
	}

	void set_string_type(paramdsc* v, const short len, unsigned char* text = 0)
	{
		switch(v->dsc_dtype)
		{
		case dtype_text:
			v->dsc_length = len;
			if (text)
				memcpy(v->dsc_address, text, len);
			break;
		case dtype_cstring:
			v->dsc_length = len;
			if (text)
				memcpy(v->dsc_address, text, len);
			v->dsc_address[len] = 0;
			break;
		case dtype_varying:
			v->dsc_length = static_cast<short>(len + varchar_indicator);
			set_varchar_len(v->dsc_address, len);
			if (text)
				memcpy(v->dsc_address + varchar_indicator, text, len);
			break;
		}
	}

	bool isnull(const paramdsc* v)
	{
		return !v || !v->dsc_address || (v->dsc_flags & DSC_null);
	}

	paramdsc* setnull(paramdsc* v)
	{
		if (v)
			v->dsc_flags |= DSC_null;
		return v;
	}

	short get_scaled_double(const paramdsc* v, double& rc)
	{
		ISC_INT64 iv;
		short rct = get_int_type(v, iv);
		if (rct < 0)
			rct = get_double_type(v, rc);
		else
		{
			rc = static_cast<double>(iv);
			signed char scale = v->dsc_scale;
			for (; scale < 0; ++scale)
				rc /= 10;
			for (; scale > 0; --scale)
				rc *= 10;
		}
		return rct;
	}
} // namespace internal


FBUDF_API paramdsc* idNvl(paramdsc* v, paramdsc* v2)
{
	if (!internal::isnull(v))
		return v;
	return v2;
}

FBUDF_API paramdsc* sNvl(paramdsc* v, paramdsc* v2, paramdsc* rc)
{
	if (!internal::isnull(v))
	{
		unsigned char *sv = 0;
		short len = internal::get_string_type(v, sv);
		internal::set_string_type(rc, len, sv);
		return rc;
	}
	if (!internal::isnull(v2))
	{
		unsigned char *sv2 = 0;
		short len = internal::get_string_type(v2, sv2);
		internal::set_string_type(rc, len, sv2);
		return rc;
	}
	return internal::setnull(rc);
}


FBUDF_API paramdsc* iNullIf(paramdsc* v, paramdsc* v2)
{
	if (internal::isnull(v) || internal::isnull(v2))
		return 0;
	ISC_INT64 iv, iv2;
	short rc = internal::get_int_type(v, iv);
	short rc2 = internal::get_int_type(v2, iv2);
	if (rc < 0 || rc2 < 0)
		return v;
	if (iv == iv2 && v->dsc_scale == v2->dsc_scale)
		return 0;
	return v;
}

FBUDF_API paramdsc* dNullIf(paramdsc* v, paramdsc* v2)
{
	if (internal::isnull(v) || internal::isnull(v2))
		return 0;
	double iv, iv2;
	short rc = internal::get_double_type(v, iv);
	short rc2 = internal::get_double_type(v2, iv2);
	if (rc < 0 || rc2 < 0)
		return v;
	if (iv == iv2) // && v->dsc_scale == v2->dsc_scale) double w/o scale
		return 0;
	return v;
}

FBUDF_API paramdsc* sNullIf(paramdsc* v, paramdsc* v2, paramdsc* rc)
{
	if (internal::isnull(v) || internal::isnull(v2))
		return internal::setnull(rc);
	unsigned char *sv, *sv2;
	short len = internal::get_string_type(v, sv);
	short len2 = internal::get_string_type(v2, sv2);
	if (len < 0 || len2 < 0) // good luck with the result, we can't do more.
		return v;
	if (len == len2 && (!len || !memcmp(sv, sv2, len)) &&
		(v->dsc_ttype == v2->dsc_ttype ||
		!v->dsc_ttype || !v2->dsc_ttype))
		return internal::setnull(rc);
	internal::set_string_type(rc, len, sv);
	return rc;
}

namespace internal
{
	enum day_format {day_short, day_long};
	const size_t day_len[] = {4, 14};
	const char* day_fmtstr[] = {"%a", "%A"};

	char* get_DOW(ISC_TIMESTAMP* v, char* rc, const day_format df)
	{
		tm times;
		//ISC_TIMESTAMP timestamp;
		//timestamp.timestamp_date = *v;
		//timestamp.timestamp_time = 0;
		//isc_decode_timestamp(&timestamp, &times);
		isc_decode_timestamp(v, &times);
		//isc_decode_sql_date(v, &times);
		int dow = times.tm_wday;
		if (dow >= 0 && dow <= 6)
		{
			size_t name_len = day_len[df];
			const char* name_fmt = day_fmtstr[df];
			if (!strcmp(setlocale(LC_TIME, NULL), "C"))
				setlocale(LC_ALL, "");
			name_len = strftime(rc + varchar_indicator, name_len, name_fmt, &times);
			if (name_len)
			{
				// There's no clarity in the docs whether '\0' is counted or not; be safe.
				char *p = rc + varchar_indicator + name_len - 1;
				if (!*p)
					--name_len;
				set_varchar_len(rc, static_cast<short>(name_len));
				return rc;
			}
		}
		set_varchar_len(rc, 5);
		memcpy(rc + varchar_indicator, "ERROR", 5);
		return rc;
	}
} // namespace internal

FBUDF_API char* DOW(ISC_TIMESTAMP* v, char* rc)
{
	return internal::get_DOW(v, rc, internal::day_long);
}

FBUDF_API char* SDOW(ISC_TIMESTAMP* v, char* rc)
{
	return internal::get_DOW(v, rc, internal::day_short);
}

FBUDF_API paramdsc* right(paramdsc* v, short* rl, paramdsc* rc)
{
	if (internal::isnull(v))
		return internal::setnull(rc);
	unsigned char* text = 0;
	short len = internal::get_string_type(v, text), diff = static_cast<short>(len - *rl);
	if (*rl < len)
		len = *rl;
	if (len < 0)
		return internal::setnull(rc);
	if (diff > 0)
		text += diff;
	internal::set_string_type(rc, len, text);
	return rc;
}

FBUDF_API ISC_TIMESTAMP* addDay(ISC_TIMESTAMP* v, int& ndays)
{
	//tm times;
	v->timestamp_date += ndays;
	return v;
}

FBUDF_API ISC_TIMESTAMP* addWeek(ISC_TIMESTAMP* v, int& nweeks)
{
	//tm times;
	v->timestamp_date += nweeks * 7;
	return v;
}

FBUDF_API ISC_TIMESTAMP* addMonth(ISC_TIMESTAMP* v, int& nmonths)
{
	tm times;
	isc_decode_timestamp(v, &times);
	int y = nmonths / 12, m = nmonths % 12;
	times.tm_year += y;
	if ((times.tm_mon += m) > 11)
	{
		times.tm_year++;
		times.tm_mon -= 12;
	}
	else if (times.tm_mon < 0)
	{
		times.tm_year--;
		times.tm_mon += 12;
	}
	int md[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int ly = times.tm_year + 1900;
	if (ly % 4 == 0 && ly % 100 != 0 || ly % 400 == 0)
		md[1]++;
	if (times.tm_mday > md[times.tm_mon])
		times.tm_mday = md[times.tm_mon];
	isc_encode_timestamp(&times, v);
	return v;
}

FBUDF_API ISC_TIMESTAMP* addYear(ISC_TIMESTAMP* v, int& nyears)
{
	tm times;
	isc_decode_timestamp(v, &times);
	times.tm_year += nyears;
	isc_encode_timestamp(&times, v);
	return v;
}

namespace internal
{
	ISC_TIMESTAMP* addTenthMSec(ISC_TIMESTAMP* v, int tenthmilliseconds, int multiplier)
	{
		long full = tenthmilliseconds * multiplier;
		long days = full / tenthmsec_in_day, secs = full % tenthmsec_in_day;
		v->timestamp_date += days;
		 // Time portion is unsigned, so we avoid unsigned rolling over negative values
		// that only produce a new unsigned number with the wrong result.
		if (secs < 0 && static_cast<unsigned long>(-secs) > v->timestamp_time)
		{
			v->timestamp_date--;
			v->timestamp_time += tenthmsec_in_day + secs;
		}
		else if ((v->timestamp_time += secs) >= tenthmsec_in_day)
		{
			v->timestamp_date++;
			v->timestamp_time -= tenthmsec_in_day;
		}
		return v;
	}
}

FBUDF_API ISC_TIMESTAMP* addMilliSecond(ISC_TIMESTAMP* v, int& nmseconds)
{
	return internal::addTenthMSec(v, nmseconds, ISC_TIME_SECONDS_PRECISION / 1000);
}

FBUDF_API ISC_TIMESTAMP* addSecond(ISC_TIMESTAMP* v, int& nseconds)
{
	return internal::addTenthMSec(v, nseconds, ISC_TIME_SECONDS_PRECISION);
}

FBUDF_API ISC_TIMESTAMP* addMinute(ISC_TIMESTAMP* v, int& nminutes)
{
	return internal::addTenthMSec(v, nminutes, 60 * ISC_TIME_SECONDS_PRECISION);
}

FBUDF_API ISC_TIMESTAMP* addHour(ISC_TIMESTAMP* v, int& nhours)
{
	return internal::addTenthMSec(v, nhours, 3600 * ISC_TIME_SECONDS_PRECISION);
}

FBUDF_API ISC_TIMESTAMP* getExactTimestamp(ISC_TIMESTAMP* rc)
{
	//time_t now;
	//time(&now);
#if defined(_WIN32)
	_timeb timebuffer;
	_ftime(&timebuffer);
	// localtime uses thread local storage in NT, no need to lock threads.
	// Of course, this facility is only available in multithreaded builds.
	tm times = *localtime(&timebuffer.time);
	isc_encode_timestamp(&times, rc);
	rc->timestamp_time += timebuffer.millitm * 10;
#else

#ifdef HAVE_PTHREAD_H
    pthread_mutex_t loctimelock =  PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&loctimelock);  // ctime critical section start
#endif

	timeval tv;
#ifdef GETTIMEOFDAY_RETURNS_TIMEZONE
	(void)gettimeofday(&tv, (struct timezone *)0);
#else
	(void)gettimeofday(&tv);
#endif

	tm times = *localtime(reinterpret_cast<const time_t*>(&tv.tv_sec));

#ifdef HAVE_PTHREAD_H
	pthread_mutex_unlock(&loctimelock);  // ctime critical section end
#endif

	isc_encode_timestamp(&times, rc);
	rc->timestamp_time += tv.tv_usec / 100;
#endif
	return rc;
}

FBUDF_API paramdsc* fbtruncate(paramdsc* v, paramdsc* rc)
{
	if (internal::isnull(v))
		return internal::setnull(rc);
	ISC_INT64 iv;
	short rct = internal::get_int_type(v, iv);
	if (rct < 0 || v->dsc_scale > 0)
		return internal::setnull(rc);
	if (!v->dsc_scale /*|| !v->dsc_sub_type*/) //second test won't work with ODS9
	{
		internal::set_int_type(rc, iv);
		rc->dsc_scale = 0;
		return rc;
	}

	// truncate(0.9)  =>  0
	// truncate(-0.9) => -1
	// truncate(-0.9) =>  0 ### SYMMETRIC_MATH defined.
	signed char scale = v->dsc_scale;
#if defined(SYMMETRIC_MATH)
	while (scale++ < 0)
		iv /= 10;
#else
	bool gt = false;
	while (scale++ < 0)
	{
		if (iv % 10)
			gt = true;
		iv /= 10;
	}
	if (gt)
	{
		if (iv < 0)
			--iv;
	}
#endif
	internal::set_int_type(rc, iv);
	rc->dsc_scale = 0;
	return rc;
}

FBUDF_API paramdsc* fbround(paramdsc* v, paramdsc* rc)
{
	if (internal::isnull(v))
		return internal::setnull(rc);
	ISC_INT64 iv;
	short rct = internal::get_int_type(v, iv);
	if (rct < 0 || v->dsc_scale > 0)
		return internal::setnull(rc);
	if (!v->dsc_scale /*|| !v->dsc_sub_type*/) //second test won't work with ODS9
	{
		internal::set_int_type(rc, iv);
		rc->dsc_scale = 0;
		return rc;
	}
	
	// round(0.3)  => 0 ### round(0.5)  =>  1
	// round(-0.3) => 0 ### round(-0.5) =>  0
	// round(-0.3) => 0 ### round(-0.5) => -1 ### SYMMETRIC_MATH defined.
	bool gt = false;
	signed char scale = v->dsc_scale;
	while(scale++ < 0)
	{
		if (!scale)
		{
			short dig = static_cast<short>(iv % 10);
#if defined(SYMMETRIC_MATH)
			if (dig >= 5 || dig <= -5)
				gt = true;
#else
			if (dig >= 5 || dig < -5)
				gt = true;
#endif
		}
		iv /= 10;
	}
	if (gt)
	{
		if (iv < 0)
			--iv;
		else ++iv;
	}
	internal::set_int_type(rc, iv);
	rc->dsc_scale = 0;
	return rc;
}

FBUDF_API paramdsc* power(paramdsc* v, paramdsc* v2, paramdsc* rc)
{
	if (internal::isnull(v) || internal::isnull(v2))
		return internal::setnull(rc);
	double d, d2;
	short rct = internal::get_scaled_double(v, d);
	short rct2 = internal::get_scaled_double(v2, d2);

	// If we cause a div by zero, SS shutdowns in response.
	// The doc I read says 0^0 will produce 1, so it's not tested below.
	if (rct < 0 || rct2 < 0 || !d && d2 < 0)
		return internal::setnull(rc);

	internal::set_double_type(rc, pow(d, d2));
	rc->dsc_scale = 0;
	return rc;
}

FBUDF_API blobcallback* string2blob(paramdsc* v, blobcallback* outblob)
{
	if (internal::isnull(v))
		return 0;
	unsigned char* text = 0;
	short len = internal::get_string_type(v, text);
	if (len < 0)
		return 0;
	if (!outblob || !outblob->blob_handle)
		return 0;
	outblob->blob_put_segment(outblob->blob_handle, text, len);
	return outblob;
}

