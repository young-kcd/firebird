/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		PerformanceView.cpp
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
 *  $Id: PerformanceView.cpp,v 1.1 2004-12-09 07:20:26 skidder Exp $
 *
 */

#include "firebird.h"

#include "../jrd/PerformanceView.h"
#include "../jrd/TraceManager.h"
#include "../jrd/jrd.h"
#include "../jrd/req.h"

namespace Jrd {

// Create view
PerformanceView::PerformanceView(Firebird::MemoryPool* pool) : 
	relation_counts(pool)
{
	memset(database_counts, 0, sizeof(database_counts));
}

// Create view cloning counters of given request
PerformanceView::PerformanceView(Firebird::MemoryPool* pool, jrd_req* request) :
	relation_counts(pool, request->req_total_counts.relation_counts)
{
	memcpy(database_counts, request->req_total_counts.database_counts, sizeof(database_counts));
}

// Calculate difference between baseline and current view state (to call Trace API nicely)
PerformanceInfo *PerformanceView::compute_difference(
	jrd_req* request, PerformanceInfo &dest, TraceCountsArray& temp)
{
	// NOTE: we do not initialize dest.pin_time. This must be done by the caller

	// Calculate database-level statistics
	for(int i = 0; i < DBB_max_dbb_count; i++)
		database_counts[i] = request->req_total_counts.database_counts[i] - database_counts[i];

	dest.pin_counters = database_counts;

	// Calculate relation-level statistics
	temp.clear();

	RelationCountsTree::Accessor new_acc(&request->req_total_counts.relation_counts), base_acc(&relation_counts);

	// This loop assumes that new array is smaller than old one
	bool base_found = base_acc.getFirst();
	if (new_acc.getFirst())	do {
		RelationCounts *counts = &new_acc.current();
		if (base_found && base_acc.current().rlc_relation_id == counts->rlc_relation_id) {
			RelationCounts *base_counts = &base_acc.current();
			bool all_zeros = true;
			for (int i = 0; i < DBB_max_rel_count; i++) {
				if ((base_counts->rlc_counter[i] = counts->rlc_counter[i] - base_counts->rlc_counter[i]))
					all_zeros = false;
			}
			// Point TraceCounts to counts array from baseline object
			if (!all_zeros) {
				jrd_rel *relation = 
					counts->rlc_relation_id < request->req_attachment->att_database->dbb_relations->count() ?
					(jrd_rel*)(*request->req_attachment->att_database->dbb_relations)[counts->rlc_relation_id] :
					NULL;
				TraceCounts traceCounts;
				traceCounts.trc_relation_id = counts->rlc_relation_id;
				traceCounts.trc_counters = base_counts->rlc_counter;
				traceCounts.trc_relation_name = relation ? relation->rel_name : NULL;
				temp.add(traceCounts);
			}
			base_found = base_acc.getNext();
		} else {
			jrd_rel *relation = 
				counts->rlc_relation_id < request->req_attachment->att_database->dbb_relations->count() ?
				(jrd_rel*)(*request->req_attachment->att_database->dbb_relations)[counts->rlc_relation_id] :
				NULL;

			// Point TraceCounts to counts array from object with updated counters
			TraceCounts traceCounts;
			traceCounts.trc_relation_id = counts->rlc_relation_id;
			traceCounts.trc_counters = counts->rlc_counter;
			traceCounts.trc_relation_name = relation ? relation->rel_name : NULL;
			temp.add(traceCounts);
		}
	} while (new_acc.getNext());

	dest.pin_count = temp.getCount();
	dest.pin_tables = temp.begin();
	return &dest;
}

void PerformanceView::bump_count(int counter_id, SLONG relation_id, int change)
{
	fb_assert(counter_id >= 0 && counter_id < DBB_max_rel_count);

	RelationCountsTree::Accessor accessor(&relation_counts);
	if (accessor.locate(relation_id)) {
		accessor.current().rlc_counter[counter_id] += change;
	} else {
		RelationCounts counts;
		memset(&counts, 0, sizeof(counts));
		counts.rlc_relation_id = relation_id;
		counts.rlc_counter[counter_id] += change;
		relation_counts.add(counts);
	}
}

void PerformanceView::bump_count(thread_db *tdbb, int counter_id) {
	fb_assert(counter_id >= 0 && counter_id < DBB_max_dbb_count);

	// Bump attachment-level statistics
	Attachment *att = tdbb->tdbb_attachment;
	if (att)
		att->att_total_counts.database_counts[counter_id]++;

	Database* dbb = tdbb->tdbb_database;
    if (!dbb)
		return;

	// If we are collecting statement-level statistics then bump counter for top-level statement
	if (dbb->dbb_trace_manager->needs().event_dsql_execute || 
		dbb->dbb_trace_manager->needs().event_dsql_fetch)
	{
		jrd_req* request = tdbb->tdbb_request;
		if (!request) 
			return;

		while (request->req_caller)
			request = request->req_caller;

		request->req_total_counts.database_counts[counter_id]++;
	}
}

void PerformanceView::bump_count(
	thread_db *tdbb, int counter_id, SLONG relation_id, int change)
{
	// Bump attachment-level statistics
	Attachment *att = tdbb->tdbb_attachment;
	if (att)
		att->att_total_counts.bump_count(counter_id, relation_id, change);

	Database* dbb = tdbb->tdbb_database;
    if (!dbb)
		return;

	// If we are collecting statement-level statistics then bump counter for top-level statement
	if (dbb->dbb_trace_manager->needs().event_dsql_execute || 
		dbb->dbb_trace_manager->needs().event_dsql_fetch)
	{
		jrd_req* request = tdbb->tdbb_request;
		if (!request) 
			return;

		while (request->req_caller)
			request = request->req_caller;

		request->req_total_counts.bump_count(counter_id, relation_id, change);
	}
}

}
