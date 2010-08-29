/*
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../jrd/req.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/exe_proto.h"
#include "../jrd/vio_proto.h"

#include "RecordSource.h"

using namespace Firebird;
using namespace Jrd;

// --------------------------
// Data access: regular union
// --------------------------

Union::Union(CompilerScratch* csb, UCHAR stream,
			 size_t argCount, RecordSource* const* args, NestConst<MapNode>* maps,
			 size_t streamCount, const UCHAR* streams)
	: RecordStream(csb, stream), m_args(csb->csb_pool), m_maps(csb->csb_pool),
	  m_streams(csb->csb_pool)
{
	fb_assert(argCount);

	m_impure = CMP_impure(csb, sizeof(Impure));

	m_args.resize(argCount);

	for (size_t i = 0; i < argCount; i++)
	{
		m_args[i] = args[i];
	}

	m_maps.resize(argCount);

	for (size_t i = 0; i < argCount; i++)
	{
		m_maps[i] = maps[i];
	}

	m_streams.resize(streamCount);

	for (size_t i = 0; i < streamCount; i++)
	{
		m_streams[i] = streams[i];
	}
}

void Union::open(thread_db* tdbb) const
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = request->getImpure<Impure>(m_impure);

	impure->irsb_flags = irsb_open;

	impure->irsb_count = 0;
	VIO_record(tdbb, &request->req_rpb[m_stream], m_format, tdbb->getDefaultPool());

	// Initialize the record number of each stream in the union

	for (size_t i = 0; i < m_streams.getCount(); i++)
	{
		const UCHAR stream = m_streams[i];
		request->req_rpb[stream].rpb_number.setValue(BOF_NUMBER);
	}

	m_args[impure->irsb_count]->open(tdbb);
}

void Union::close(thread_db* tdbb) const
{
	jrd_req* const request = tdbb->getRequest();

	invalidateRecords(request);

	Impure* const impure = request->getImpure<Impure>(m_impure);

	if (impure->irsb_flags & irsb_open)
	{
		impure->irsb_flags &= ~irsb_open;

		if (impure->irsb_count < m_args.getCount())
		{
			m_args[impure->irsb_count]->close(tdbb);
		}
	}
}

bool Union::getRecord(thread_db* tdbb) const
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = request->getImpure<Impure>(m_impure);

	if (!(impure->irsb_flags & irsb_open))
	{
		return false;
	}

	// March thru the sub-streams looking for a record

	while (!m_args[impure->irsb_count]->getRecord(tdbb))
	{
		m_args[impure->irsb_count]->close(tdbb);
		impure->irsb_count++;
		if (impure->irsb_count >= m_args.getCount())
		{
			return false;
		}
		m_args[impure->irsb_count]->open(tdbb);
	}

	// We've got a record, map it into the target record

	const MapNode* const map = m_maps[impure->irsb_count];
	const NestConst<jrd_nod>* ptr = map->items.begin();

	for (const NestConst<jrd_nod>* const end = map->items.end(); ptr != end; ++ptr)
		EXE_assignment(tdbb, *ptr);

	return true;
}

bool Union::refetchRecord(thread_db* tdbb) const
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = request->getImpure<Impure>(m_impure);

	if (impure->irsb_count >= m_args.getCount())
	{
		return false;
	}

	return m_args[impure->irsb_count]->refetchRecord(tdbb);
}

bool Union::lockRecord(thread_db* tdbb) const
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = request->getImpure<Impure>(m_impure);

	if (impure->irsb_count >= m_args.getCount())
	{
		return false;
	}

	return m_args[impure->irsb_count]->lockRecord(tdbb);
}

void Union::dump(thread_db* tdbb, UCharBuffer& buffer) const
{
	buffer.add(isc_info_rsb_begin);

	buffer.add(isc_info_rsb_type);
	buffer.add(isc_info_rsb_union);

	const size_t count = m_args.getCount();
	// This place must be reviewed if we allow more than 255 unions
	fb_assert(count <= USHORT(MAX_UCHAR));
	buffer.add((UCHAR) count);

	for (size_t i = 0; i < count; i++)
	{
		m_args[i]->dump(tdbb, buffer);
	}

	buffer.add(isc_info_rsb_end);
}

void Union::markRecursive()
{
	for (size_t i = 0; i < m_args.getCount(); i++)
	{
		m_args[i]->markRecursive();
	}
}

void Union::invalidateRecords(jrd_req* request) const
{
	for (size_t i = 0; i < m_args.getCount(); i++)
	{
		m_args[i]->invalidateRecords(request);
	}
}
