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

namespace Firebird {

class TimeStamp
{
public:

	explicit TimeStamp(bool empty = false);

	void invalidate();
	void validate();

	bool encode(tm* times) const;
	bool encode(ISC_TIMESTAMP* ts, bool precise) const;

private:

	time_t seconds;
	time_t fractions;

	void generate();
};

}	// namespace Firebird

#endif // CLASSES_TIMESTAMP_H
