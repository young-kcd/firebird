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
#include "../common/classes/array.h"
#include "../common/classes/init.h"
#include "../common/classes/tree.h"
#include "../jrd/ntrace.h"

namespace Jrd {

class Database;


// Performance counters for individual table 
struct RelationCounts 
{
	SLONG rlc_relation_id;	// Relation ID 
	SINT64 rlc_counter[DBB_max_rel_count];

	inline static const SLONG& generate(const void* sender, const RelationCounts& item) 
		{ return item.rlc_relation_id; }
};

typedef Firebird::BePlusTree<RelationCounts, SLONG, Firebird::MemoryPool, RelationCounts> RelationCountsTree;

 typedef Firebird::HalfStaticArray<TraceCounts, 5> TraceCountsArray;

// Runtime statistics class

class RuntimeStatistics
{
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

	RuntimeStatistics(MemoryPool& pool) : rel_counts(&pool)
	{
		reset();
	}

	RuntimeStatistics(MemoryPool& pool, const RuntimeStatistics& other);
	
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
	void bumpValue(StatType index, SLONG relation_id);

	// Calculate difference between counts stored in this object and current 
	// counts of given request. Counts stored in object are destroyed.
	PerformanceInfo *computeDifference(Database *dbb, const RuntimeStatistics& new_stat, 
		PerformanceInfo &dest, TraceCountsArray& temp);

//	bool operator==(const RuntimeStatistics& other) const;
//	bool operator!=(const RuntimeStatistics& other) const;

	RuntimeStatistics& operator+=(const RuntimeStatistics& other)
	{
		for (size_t i = 0; i < TOTAL_ITEMS; ++i)
			values[i] += other.values[i];

		addRelCounts(other.rel_counts, true);
		return *this;
	}

	RuntimeStatistics& operator-=(const RuntimeStatistics& other)
	{
		for (size_t i = 0; i < TOTAL_ITEMS; ++i)
			values[i] -= other.values[i];

		addRelCounts(other.rel_counts, false);
		return *this;
	}

	static RuntimeStatistics* getDummy()
	{
		return &dummy;
	}

private:
	void addRelCounts(const RelationCountsTree &other, bool add);

	SINT64 values[TOTAL_ITEMS];
	RelationCountsTree rel_counts;

	// This dummy RuntimeStatistics is used instead of missing elements in tdbb,
	// helping us avoid conditional checks in time-critical places of code.
	// Values of it contain actually garbage - don't be surprised when debugging.
	static Firebird::GlobalPtr<RuntimeStatistics> dummy;
};

} // namespace

#endif // JRD_RUNTIME_STATISTICS_H
