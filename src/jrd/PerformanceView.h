/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		PerformanceView.h
 *	DESCRIPTION:	Performance counts for tables
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
 *  The Original Code was created by Nickolay Samofatov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Nickolay Samofatov <nickolay@broadviewsoftware.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 *  $Id: PerformanceView.h,v 1.1 2004-12-09 07:20:26 skidder Exp $
 *
 */

#ifndef JRD_PERFORMANCEVIEW_H
#define JRD_PERFORMANCEVIEW_H

#include "../jrd/ntrace.h"
#include "../common/classes/array.h"
#include "../common/classes/tree.h"

namespace Jrd {

class thread_db;
class jrd_req;

/* Performance counters for individual table */
struct RelationCounts {
	SLONG rlc_relation_id; /* Relation ID */
	ntrace_counter_t rlc_counter[DBB_max_rel_count];
	inline static const SLONG& generate(const void* sender, const RelationCounts& item) 
		{ return item.rlc_relation_id; }
};

typedef Firebird::BePlusTree<RelationCounts, SLONG, Firebird::MemoryPool, RelationCounts> RelationCountsTree;

typedef Firebird::HalfStaticArray<TraceCounts, 5> TraceCountsArray;

class PerformanceView {
public:
	// Create view
	PerformanceView(Firebird::MemoryPool* pool);

	// Create view cloning counters of given request
	PerformanceView(Firebird::MemoryPool* pool, jrd_req* request);

	// Calculate difference between counts stored in this object and current 
	// counts of given request. Counts stored in object are destroyed.
	PerformanceInfo *compute_difference(jrd_req* request, PerformanceInfo &dest, TraceCountsArray& temp);

	// Bump table-specific performance counter
	static void bump_count(thread_db *tdbb, int counter_id, SLONG relation_id, int change);

	// Bump global performance counter
	static void bump_count(thread_db *tdbb, int counter_id);

	ntrace_counter_t getDbCounter(int counter_id) {
		fb_assert(counter_id >= 0 && counter_id < DBB_max_dbb_count);
		return database_counts[counter_id];
	}

	RelationCountsTree relation_counts;
private:
	// Bump particular counter in this object
	void bump_count(int counter_id, SLONG relation_id, int change);

	ntrace_counter_t database_counts[DBB_max_dbb_count];
};

}

#endif
