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
#include "../jrd/gdsassert.h"
#include "../jrd/req.h"

#include "../jrd/RuntimeStatistics.h"

using namespace Firebird;

namespace Jrd {

GlobalPtr<RuntimeStatistics> RuntimeStatistics::dummy;

RuntimeStatistics::RuntimeStatistics(MemoryPool& pool, const RuntimeStatistics& other)
	: rel_counts(&pool, other.rel_counts)
{
	memcpy(values, other.values, sizeof(values));
}

void RuntimeStatistics::bumpValue(StatType index, SLONG relation_id)
{
	fb_assert(index >= 0);
	
	RelationCountsTree::Accessor accessor(&rel_counts);
	if (accessor.locate(relation_id))
		accessor.current().rlc_counter[index]++;
	else
	{
		RelationCounts counts;
		memset(&counts, 0, sizeof(counts));
		counts.rlc_relation_id = relation_id;
		counts.rlc_counter[index]++;
		rel_counts.add(counts);
	}
}

void RuntimeStatistics::addRelCounts(const RelationCountsTree& other, bool add)
{
	RelationCountsTree::Accessor first(&rel_counts);
	RelationCountsTree::ConstAccessor second(&other);

	if (second.getFirst())
	{
		do 
		{
			const RelationCounts& src = second.current();

			if (!first.locate(src.rlc_relation_id))
			{
				RelationCounts counts;
				memset(&counts, 0, sizeof(counts));
				counts.rlc_relation_id = src.rlc_relation_id;
				first.add(counts);

				first.locate(src.rlc_relation_id);
			}

			RelationCounts& dst = first.current();
			fb_assert(src.rlc_relation_id == dst.rlc_relation_id);

			for (int index = 0; index < FB_NELEM(src.rlc_counter); index++)
			{
				if (add)
					dst.rlc_counter[index] += src.rlc_counter[index];
				else
					dst.rlc_counter[index] -= src.rlc_counter[index];
			}
		} while(second.getNext());
	}
}

void RuntimeStatistics::reset()
{
	memset(values, 0, sizeof values);
	rel_counts.clear();
}


PerformanceInfo* RuntimeStatistics::computeDifference(
	Database* dbb, const RuntimeStatistics& new_stat, PerformanceInfo& dest, TraceCountsArray& temp)
{
	// NOTE: we do not initialize dest.pin_time. This must be done by the caller

	// Calculate database-level statistics
	for (int i = 0; i < TOTAL_ITEMS; i++)
		values[i] = new_stat.values[i] - values[i];
 
	dest.pin_counters = values;

	// Calculate relation-level statistics
	temp.clear();

	RelationCountsTree::ConstAccessor new_acc(&new_stat.rel_counts);
	RelationCountsTree::Accessor base_acc(&rel_counts);

	// This loop assumes that base array is smaller than new one
	bool base_found = base_acc.getFirst();

	if (new_acc.getFirst())
	{	
		do
		{
			const RelationCounts* counts = &new_acc.current();
			if (base_found && base_acc.current().rlc_relation_id == counts->rlc_relation_id) 
			{
				RelationCounts* base_counts = &base_acc.current();
				bool all_zeros = true;
				for (int i = 0; i < DBB_max_rel_count; i++)
				{
					if ((base_counts->rlc_counter[i] = counts->rlc_counter[i] - base_counts->rlc_counter[i]))
						all_zeros = false;
				}

				// Point TraceCounts to counts array from baseline object
				if (!all_zeros) 
				{
					jrd_rel* relation = counts->rlc_relation_id < dbb->dbb_relations->count() ?
						(*dbb->dbb_relations)[counts->rlc_relation_id] : NULL;
					TraceCounts traceCounts;
					traceCounts.trc_relation_id = counts->rlc_relation_id;
					traceCounts.trc_counters = base_counts->rlc_counter;
					traceCounts.trc_relation_name = relation ? relation->rel_name.c_str() : NULL;
					temp.add(traceCounts);
				}

				base_found = base_acc.getNext();
			}
			else 
			{
				jrd_rel* relation = counts->rlc_relation_id < dbb->dbb_relations->count() ?
					(*dbb->dbb_relations)[counts->rlc_relation_id] : NULL;

				// Point TraceCounts to counts array from object with updated counters
				TraceCounts traceCounts;
				traceCounts.trc_relation_id = counts->rlc_relation_id;
				traceCounts.trc_counters = counts->rlc_counter;
				traceCounts.trc_relation_name = relation ? relation->rel_name.c_str() : NULL;
				temp.add(traceCounts);
			}
		} while (new_acc.getNext());
	}

	dest.pin_count = temp.getCount();
	dest.pin_tables = temp.begin();

	return &dest;
}


/* hvlad: I've commented out this two operators as they're not used and not take 
	  in account relation's counters
bool RuntimeStatistics::operator==(const RuntimeStatistics& other) const
{
	return !memcmp(values, other.values, sizeof(values));
}

bool RuntimeStatistics::operator!=(const RuntimeStatistics& other) const
{
	return !(*this == other);
}
*/

} // namespace
