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
 *  The Original Code was created by John Bellardo
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2000 John Bellardo <bellardo@users.sourceforge.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../common/common.h"
#include "../jrd/jrd.h"
#include "../jrd/req.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/mov_proto.h"

#include "RecordSource.h"

using namespace Firebird;
using namespace Jrd;

// -------------------------------
// Data access: skip N rows filter
// -------------------------------

SkipRowsStream::SkipRowsStream(CompilerScratch* csb, RecordSource* next, jrd_nod* value)
	: m_next(next), m_value(value)
{
	fb_assert(m_next && m_value);

	m_impure = CMP_impure(csb, sizeof(Impure));
}

void SkipRowsStream::open(thread_db* tdbb) const
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = request->getImpure<Impure>(m_impure);

	impure->irsb_flags = irsb_open;

	const dsc* desc = EVL_expr(tdbb, m_value);
	const SINT64 value = (desc && !(request->req_flags & req_null)) ? MOV_get_int64(desc, 0) : 0;

    if (value < 0)
	{
		status_exception::raise(Arg::Gds(isc_bad_skip_param));
	}

	impure->irsb_count = value + 1;

	m_next->open(tdbb);
}

void SkipRowsStream::close(thread_db* tdbb) const
{
	jrd_req* const request = tdbb->getRequest();

	invalidateRecords(request);

	Impure* const impure = request->getImpure<Impure>(m_impure);

	if (impure->irsb_flags & irsb_open)
	{
		impure->irsb_flags &= ~irsb_open;

		m_next->close(tdbb);
	}
}

bool SkipRowsStream::getRecord(thread_db* tdbb) const
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = request->getImpure<Impure>(m_impure);

	if (!(impure->irsb_flags & irsb_open))
	{
		return false;
	}

	while (impure->irsb_count > 1)
	{
		impure->irsb_count--;

		if (!m_next->getRecord(tdbb))
		{
			return false;
		}
	}

	impure->irsb_count--;

	return m_next->getRecord(tdbb);
}

bool SkipRowsStream::refetchRecord(thread_db* tdbb) const
{
	return m_next->refetchRecord(tdbb);
}

bool SkipRowsStream::lockRecord(thread_db* tdbb) const
{
	return m_next->lockRecord(tdbb);
}

void SkipRowsStream::dump(thread_db* tdbb, UCharBuffer& buffer) const
{
	buffer.add(isc_info_rsb_begin);

	buffer.add(isc_info_rsb_type);
	buffer.add(isc_info_rsb_skip);

	m_next->dump(tdbb, buffer);

	buffer.add(isc_info_rsb_end);
}

void SkipRowsStream::markRecursive()
{
	m_next->markRecursive();
}

void SkipRowsStream::findUsedStreams(StreamsArray& streams) const
{
	m_next->findUsedStreams(streams);
}

void SkipRowsStream::invalidateRecords(jrd_req* request) const
{
	m_next->invalidateRecords(request);
}

void SkipRowsStream::nullRecords(thread_db* tdbb) const
{
	m_next->nullRecords(tdbb);
}

void SkipRowsStream::saveRecords(thread_db* tdbb) const
{
	m_next->saveRecords(tdbb);
}

void SkipRowsStream::restoreRecords(thread_db* tdbb) const
{
	m_next->restoreRecords(tdbb);
}
