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
 *  Copyright (c) 2009 Dmitry Yemanov <dimitr@firebirdsql.org>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../jrd/common.h"
#include "../jrd/align.h"
#include "../jrd/jrd.h"
#include "../jrd/req.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/mov_proto.h"

#include "RecordSource.h"

using namespace Firebird;
using namespace Jrd;

// --------------------------
// Data access: record buffer
// --------------------------

BufferedStream::BufferedStream(CompilerScratch* csb, RecordSource* next)
	: m_next(next), m_map(csb->csb_pool)
{
	fb_assert(m_next);

	m_impure = CMP_impure(csb, sizeof(Impure));

	UCHAR streams[MAX_STREAMS];
	streams[0] = 0;
	m_next->findUsedStreams(streams);

	Array<dsc> fields;

	for (size_t i = 1; i <= streams[0]; i++)
	{
		const UCHAR stream = streams[i];
		CompilerScratch::csb_repeat* const tail = &csb->csb_rpt[stream];

		UInt32Bitmap::Accessor accessor(csb->csb_rpt[stream].csb_fields);

		if (accessor.getFirst())
		{
			do {
				const USHORT id = (USHORT) accessor.current();
				const Format* const format = tail->csb_format; // CMP_format(tdbb, csb, stream);
				const dsc* const desc = &format->fmt_desc[id];
				m_map.add(FieldMap(stream, id));
				fields.add(*desc);
			} while (accessor.getNext());
		}
	}

	const size_t count = fields.getCount();
	m_format = Format::newFormat(csb->csb_pool, count);
	ULONG offset = FLAG_BYTES(count);

	for (size_t i = 0; i < count; i++)
	{
		dsc& desc = m_format->fmt_desc[i] = fields[i];
		offset = FB_ALIGN(offset, type_alignments[desc.dsc_dtype]);
		desc.dsc_address = (UCHAR *)(IPTR) offset;
		offset += desc.dsc_length;
	}

	m_format->fmt_length = offset;
}

void BufferedStream::open(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	impure->irsb_flags = irsb_open;

	m_next->open(tdbb);

	delete impure->irsb_buffer;
	MemoryPool& pool = *tdbb->getDefaultPool();
	impure->irsb_buffer = FB_NEW(pool) RecordBuffer(pool, m_format);

	Record* const buffer_record = impure->irsb_buffer->getTempRecord();

	// Place all the underlying records into the buffer
	while (m_next->getRecord(tdbb))
	{
		// Assign the non-null fields
		for (size_t i = 0; i < m_map.getCount(); i++)
		{
			const FieldMap map = m_map[i];

			record_param* const rpb = &request->req_rpb[map.map_stream];
			Record* const record = rpb->rpb_record;

			dsc from;
			if (EVL_field(NULL, record, map.map_id, &from))
			{
				dsc to;
				EVL_field(NULL, buffer_record, (USHORT) i, &to);
				MOV_move(tdbb, &from, &to);
				CLEAR_NULL(buffer_record, i);
			}
			else
			{
				SET_NULL(buffer_record, i);
			}
		}

		// Put the record into the buffer
		impure->irsb_buffer->store(buffer_record);
	}

	impure->irsb_position = 0;
}

void BufferedStream::close(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();

	invalidateRecords(request);

	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	if (impure->irsb_flags & irsb_open)
	{
		impure->irsb_flags &= ~irsb_open;

		delete impure->irsb_buffer;
		impure->irsb_buffer = NULL;

		m_next->close(tdbb);
	}
}

bool BufferedStream::getRecord(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	if (!(impure->irsb_flags & irsb_open))
	{
		return false;
	}

	Record* const buffer_record = impure->irsb_buffer->getTempRecord();
	if (!impure->irsb_buffer->fetch(impure->irsb_position, buffer_record))
	{
		return false;
	}

	// Assign fields back to their original streams
	for (size_t i = 0; i < m_map.getCount(); i++)
	{
		const FieldMap map = m_map[i];

		record_param* const rpb = &request->req_rpb[map.map_stream];
		Record* const record = rpb->rpb_record;

		if (record && !record->rec_format)
		{
			fb_assert(record->rec_fmt_bk);
			record->rec_format = record->rec_fmt_bk;
		}

		dsc from;
		if (EVL_field(NULL, buffer_record, (USHORT) i, &from))
		{
			dsc to;
			EVL_field(NULL, record, map.map_id, &to);
			MOV_move(tdbb, &from, &to);
			CLEAR_NULL(record, map.map_id);
		}
		else
		{
			SET_NULL(record, map.map_id);
		}
	}

	impure->irsb_position++;
	return true;
}

bool BufferedStream::refetchRecord(thread_db* tdbb)
{
	return m_next->refetchRecord(tdbb);
}

bool BufferedStream::lockRecord(thread_db* tdbb)
{
	return m_next->lockRecord(tdbb);
}

void BufferedStream::dump(thread_db* tdbb, UCharBuffer& buffer)
{
	buffer.add(isc_info_rsb_begin);

	buffer.add(isc_info_rsb_type);
	buffer.add(isc_info_rsb_buffer);

	m_next->dump(tdbb, buffer);

	buffer.add(isc_info_rsb_end);
}

void BufferedStream::markRecursive()
{
	m_next->markRecursive();
}

void BufferedStream::findUsedStreams(UCHAR* streams)
{
	m_next->findUsedStreams(streams);
}

void BufferedStream::invalidateRecords(jrd_req* request)
{
	m_next->invalidateRecords(request);
}

void BufferedStream::nullRecords(thread_db* tdbb)
{
	m_next->nullRecords(tdbb);
}

void BufferedStream::saveRecords(thread_db* tdbb)
{
	m_next->saveRecords(tdbb);
}

void BufferedStream::restoreRecords(thread_db* tdbb)
{
	m_next->restoreRecords(tdbb);
}

void BufferedStream::locate(jrd_req* request, FB_UINT64 position)
{
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);
	impure->irsb_position = position;
}

FB_UINT64 BufferedStream::getCount(jrd_req* request) const
{
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);
	return impure->irsb_buffer ? impure->irsb_buffer->getCount() : 0;
}
