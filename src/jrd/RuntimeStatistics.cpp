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
 *  The Original Code was created by Dmitry Yemanov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2006 Dmitry Yemanov <dimitr@users.sf.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"

#include "../jrd/RuntimeStatistics.h"

using namespace Firebird;

namespace Jrd {

RuntimeStatistics RuntimeStatistics::dummy;

void RuntimeStatistics::reset()
{
	memset(values, 0, sizeof values);
}

bool RuntimeStatistics::operator==(const RuntimeStatistics& other) const
{
	return !memcmp(values, other.values, sizeof(values));
}

bool RuntimeStatistics::operator!=(const RuntimeStatistics& other) const
{
	return !(*this == other);
}

RuntimeStatistics& RuntimeStatistics::operator+=(const RuntimeStatistics& other)
{
	for (size_t i = 0; i < TOTAL_ITEMS; ++i)
		values[i] += other.values[i];

	return *this;
}

RuntimeStatistics& RuntimeStatistics::operator-=(const RuntimeStatistics& other)
{
	for (size_t i = 0; i < TOTAL_ITEMS; ++i)
		values[i] -= other.values[i];

	return *this;
}

} // namespace
