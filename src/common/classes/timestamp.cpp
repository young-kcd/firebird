/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		timestamp.cpp
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

#include "firebird.h"
#include "../jrd/common.h"
#include "../jrd/dsc.h"
#include "../jrd/gdsassert.h"

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

#include "../common/classes/timestamp.h"

namespace Firebird {

TimeStamp::TimeStamp(bool empty)
{
	if (empty)
	{
		invalidate();
	}
	else
	{
		generate();
	}
}

void TimeStamp::invalidate()
{
	seconds = fractions = 0;
}

void TimeStamp::validate()
{
	if (!seconds)
	{
		generate();
	}
}

bool TimeStamp::encode(tm* times) const
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

bool TimeStamp::encode(ISC_TIMESTAMP* ts, bool precise) const
{
	tm times;
	if (!encode(&times))
	{
		return false;
	}

	const int day = times.tm_mday;
	int month = times.tm_mon + 1;
	int year = times.tm_year + 1900;

	if (month > 2)
	{
		month -= 3;
	}
	else
	{
		month += 9;
		year -= 1;
	}

	const int c = year / 100;
	const int ya = year - 100 * c;

	ts->timestamp_date =
		(((SINT64) 146097 * c) / 4 + (1461 * ya) / 4 +
		(153 * month + 2) / 5 + day + 1721119 - 2400001);

	ts->timestamp_time =
		((times.tm_hour * 60 + times.tm_min) * 60 +
		 times.tm_sec) * ISC_TIME_SECONDS_PRECISION +
		 (precise ? fractions * 10 : 0);

	return true;
}

void TimeStamp::generate()
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

}	// namespace
