/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		timestamp.h
 *	DESCRIPTION:	Date/time handling class
 *
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
 *  The Original Code was created by Dmitry Yemanov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Dmitry Yemanov <dimitr@users.sourceforge.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef CLASSES_TIMESTAMP_H
#define CLASSES_TIMESTAMP_H

#if defined(TIME_WITH_SYS_TIME)
#include <sys/time.h>
#include <time.h>
#else
#if defined(HAVE_SYS_TIME_H)
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif

#ifdef HAVE_SYS_TIMES_H
#include <sys/times.h>
#endif
#ifdef HAVE_SYS_TIMEB_H
#include <sys/timeb.h>
#endif

namespace Firebird {

class TimeStamp
{
public:

	TimeStamp()
	{
		generate();
	}

	void invalidate()
	{
		seconds = fractions = 0;
	}

	void validate()
	{
		if (!seconds)
		{
			generate();
		}
	}

	bool encode(tm* times) const
	{
		fb_assert(seconds > 0);
		const tm* t = localtime(&seconds);
		if (!t)
		{
			return false;
		}
		*times = *t;
		return true;
	}

	bool encode(ISC_TIMESTAMP* ts) const
	{
		tm times;
		if (!encode(&times))
		{
			return false;
		}
		isc_encode_timestamp(&times, ts);
		ts->timestamp_time += (fractions * 10) % ISC_TIME_SECONDS_PRECISION;
		return true;
	}

private:

	time_t seconds;
	time_t fractions;

	void generate()
	{
		// Only millisecond precision is supported currently
#ifdef HAVE_GETTIMEOFDAY
		struct timeval tp;
		GETTIMEOFDAY(&tp);
		seconds = tp.tv_sec;
		fractions = tp.tv_usec / 1000;
#else
		struct timeb time_buffer;
		ftime(&time_buffer);
		seconds = time_buffer.time;
		fractions = time_buffer.millitm;
#endif
		fb_assert(seconds > 0);
	}

};

}	// namespace Firebird

#endif CLASSES_TIMESTAMP_H
