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
 *  The Original Code was created by Adriano dos Santos Fernandes
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2009 Adriano dos Santos Fernandes <asfernandes@users.sourceforge.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../jrd/req.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/exe_proto.h"

#include "RecordSource.h"

using namespace Firebird;
using namespace Jrd;

// ------------------------------
// Data access: window expression
// ------------------------------

WindowedStream::WindowedStream(CompilerScratch* csb, UCHAR stream,
							   AggregatedStream* aggregate, RecordSource* next)
	: RecordStream(csb, stream), m_aggregate(aggregate), m_next(next)
{
	fb_assert(m_aggregate && m_next);

	m_impure = CMP_impure(csb, sizeof(Impure));
}

void WindowedStream::open(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	impure->irsb_flags = irsb_open;

	m_aggregate->open(tdbb);
	m_aggregate->getRecord(tdbb);
	m_aggregate->close(tdbb);

	m_next->open(tdbb);
}

void WindowedStream::close(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();

	invalidateRecords(request);

	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	if (impure->irsb_flags & irsb_open)
	{
		impure->irsb_flags &= ~irsb_open;

		m_next->close(tdbb);
	}
}

bool WindowedStream::getRecord(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	if (!(impure->irsb_flags & irsb_open))
	{
		return false;
	}

	if (m_next->getRecord(tdbb))
	{
		jrd_nod* const node = m_aggregate->getAggregate();
		jrd_nod* const map = node->nod_arg[e_agg_map];

		jrd_nod* const* ptr = map->nod_arg;
		for (const jrd_nod* const* end = ptr + map->nod_count; ptr < end; ptr++)
		{
			jrd_nod* const from = (*ptr)->nod_arg[e_asgn_from];

			switch (from->nod_type)
			{
				case nod_agg_average:
				case nod_agg_average_distinct:
				case nod_agg_average2:
				case nod_agg_average_distinct2:
				case nod_agg_total:
				case nod_agg_total_distinct:
				case nod_agg_total2:
				case nod_agg_total_distinct2:
				case nod_agg_min:
				case nod_agg_min_indexed:
				case nod_agg_max:
				case nod_agg_max_indexed:
				case nod_agg_count:
				case nod_agg_count2:
				case nod_agg_count_distinct:
				case nod_agg_list:
				case nod_agg_list_distinct:
					break;

				default:
					EXE_assignment(tdbb, *ptr);
					break;
			}
		}

		return true;
	}

	return false;
}

bool WindowedStream::refetchRecord(thread_db* tdbb)
{
	return m_next->refetchRecord(tdbb);
}

bool WindowedStream::lockRecord(thread_db* tdbb)
{
	return m_next->lockRecord(tdbb);
}

void WindowedStream::dump(thread_db* tdbb, UCharBuffer& buffer)
{
	buffer.add(isc_info_rsb_begin);

	buffer.add(isc_info_rsb_type);
	buffer.add(isc_info_rsb_window);

	m_aggregate->dump(tdbb, buffer);

	buffer.add(isc_info_rsb_end);
}

void WindowedStream::markRecursive()
{
	m_aggregate->markRecursive();
}

void WindowedStream::invalidateRecords(jrd_req* request)
{
	m_aggregate->invalidateRecords(request);
}
