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

// struct tm declaration
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

namespace Firebird {

// Wrapper class for ISC_TIMESTAMP supposed to implement date/time conversions
// and arithmetic. Small and not platform-specific methods are implemented 
// inline. Usage of this class normally should involve zero overhead.
//
// Note: default "shallow-copy" constructor and assignment operators
// are just fine for our purposes
//
// TODO: migrate fbudf to this class.
class TimeStamp
{
public:
	// Number of the first day of UNIX epoch in GDS counting
	enum { GDS_EPOCH_START = 40617 };

	// Default constructor. Fills timestamp with current date/time
	TimeStamp() { generate(); }

	// Altername constructor, may create empty timestamp object
	explicit TimeStamp(bool zero) 
	{
		if (zero)
			invalidate();
		else
			generate();
	}

	// Construct wrapper around pre-existing timestamp
	TimeStamp(const ISC_TIMESTAMP& from) : mValue(from) { }

	// See if timestamp contains non-zero value
	bool isEmpty() const {
		return mValue.timestamp_date == 0 && mValue.timestamp_time == 0; 
	}

	// Set value of timestamp to zero
	void invalidate() {
		mValue.timestamp_date = 0;		
		mValue.timestamp_time = 0;
	}

	// Assign value of timestamp to current date/time if it is zero
	void validate() {
		if (isEmpty()) generate();
	}

	// Encode timestamp from UNIX datetime structure
	void encode(const struct tm* times);

	// Decode timestamp into UNIX datetime structure
	void decode(struct tm* times) const;

	// Write access to timestamp structure we wrap
	ISC_TIMESTAMP& value() { return mValue; }

	// Read access to timestamp structure we wrap
	const ISC_TIMESTAMP& value() const { return mValue; }

	// ISC date/time helper routines. Both functions are signal-safe.
	static void decode_date(ISC_DATE nday, struct tm* times);
	static ISC_DATE encode_date(const struct tm* times);
private:
	ISC_TIMESTAMP mValue;

	static int yday(const struct tm* times);

	// Assign value of timestamp to current date/time
	void generate();
};

}	// namespace Firebird

#endif // CLASSES_TIMESTAMP_H
