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

#ifndef JRD_RUNTIME_STATISTICS_H
#define JRD_RUNTIME_STATISTICS_H

#include "../common/classes/alloc.h"

namespace Jrd {

class thread_db;

// Runtime statistics class

class RuntimeStatistics {
public:
	enum StatType {
		PAGE_FETCHES = 0,
		PAGE_READS,
		PAGE_MARKS,
		PAGE_WRITES,
		FLUSHES,
		RECORD_SEQ_READS,
		RECORD_IDX_READS,
		RECORD_INSERTS,
		RECORD_UPDATES,
		RECORD_DELETES,
		RECORD_BACKOUTS,
		RECORD_PURGES,
		RECORD_EXPUNGES,
		SORTS,
		SORT_GETS,
		SORT_PUTS,
		STMT_PREPARES,
		STMT_EXECUTES,
		TOTAL_ITEMS		// last
	};

	RuntimeStatistics()
	{
		reset();
	}

	~RuntimeStatistics() {}

	SINT64 getValue(const StatType index) const
	{
		return values[index];
	}

	void reset();

	void bumpValue(const StatType index)
	{
		++values[index];
	}

	bool operator==(const RuntimeStatistics& other) const;
	bool operator!=(const RuntimeStatistics& other) const;

	RuntimeStatistics& operator+=(const RuntimeStatistics& other);
	RuntimeStatistics& operator-=(const RuntimeStatistics& other);

	static RuntimeStatistics* getDummy()
	{
		return &dummy;
	}

private:
	SINT64 values[TOTAL_ITEMS];

	// This dummy RuntimeStatistics is used instead of missing elements in tdbb,
	// helping us avoid conditional checks in time-critical places of code.
	// Values of it contain actually garbage - don't be surprised when debugging.
	static RuntimeStatistics dummy;
};

} // namespace

#endif // JRD_RUNTIME_STATISTICS_H
