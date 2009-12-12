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
#include "../jrd/btr.h"
#include "../jrd/intl.h"
#include "../jrd/req.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/sort_proto.h"
#include "../jrd/vio_proto.h"

#include "RecordSource.h"

using namespace Firebird;
using namespace Jrd;

// -----------------------------
// Data access: external sorting
// -----------------------------

SortedStream::SortedStream(CompilerScratch* csb, RecordSource* next, SortMap* map)
	: m_next(next), m_map(map)
{
	fb_assert(m_next && m_map);

	m_impure = CMP_impure(csb, sizeof(Impure));
}

void SortedStream::open(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	impure->irsb_flags = irsb_open;

	// get rid of the old sort areas if this request has been used already
	SORT_fini(impure->irsb_sort_handle);

	impure->irsb_sort_handle = init(tdbb);
}

void SortedStream::close(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();

	invalidateRecords(request);

	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	if (impure->irsb_flags & irsb_open)
	{
		impure->irsb_flags &= ~irsb_open;

		SORT_fini(impure->irsb_sort_handle);
		impure->irsb_sort_handle = NULL;

		m_next->close(tdbb);
	}
}

bool SortedStream::getRecord(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	if (!(impure->irsb_flags & irsb_open))
	{
		return false;
	}

	UCHAR* const data = getData(tdbb);

	if (!data)
	{
		return false;
	}

	mapData(tdbb, request, reinterpret_cast<UCHAR*>(data));

	return true;
}

bool SortedStream::refetchRecord(thread_db* tdbb)
{
	return m_next->refetchRecord(tdbb);
}

bool SortedStream::lockRecord(thread_db* tdbb)
{
	return m_next->lockRecord(tdbb);
}

void SortedStream::dump(thread_db* tdbb, UCharBuffer& buffer)
{
	buffer.add(isc_info_rsb_begin);

	buffer.add(isc_info_rsb_type);
	buffer.add(isc_info_rsb_sort);

	m_next->dump(tdbb, buffer);

	buffer.add(isc_info_rsb_end);
}

void SortedStream::markRecursive()
{
	m_next->markRecursive();
}

void SortedStream::findUsedStreams(StreamsArray& streams)
{
	m_next->findUsedStreams(streams);
}

void SortedStream::invalidateRecords(jrd_req* request)
{
	m_next->invalidateRecords(request);
}

void SortedStream::nullRecords(thread_db* tdbb)
{
	m_next->nullRecords(tdbb);
}

void SortedStream::saveRecords(thread_db* tdbb)
{
	m_next->saveRecords(tdbb);
}

void SortedStream::restoreRecords(thread_db* tdbb)
{
	m_next->restoreRecords(tdbb);
}

sort_context* SortedStream::init(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();

	m_next->open(tdbb);
	ULONG records = 0;

	// Initialize for sort. If this is really a project operation,
	// establish a callback routine to reject duplicate records.

	sort_context* handle =
		SORT_init(tdbb->getDatabase(), &request->req_sorts,
				  m_map->smb_length, m_map->smb_keys, m_map->smb_keys, m_map->smb_key_desc,
				  ((m_map->smb_flags & SMB_project) ? rejectDuplicate : NULL), 0);

	try
	{
		// Pump the input stream dry while pushing records into sort. For
		// each record, map all fields into the sort record. The reverse
		// mapping is done in get_sort().

		dsc to, temp;

		while (m_next->getRecord(tdbb))
		{
			records++;

			// "Put" a record to sort. Actually, get the address of a place
			// to build a record.

			UCHAR* data = NULL;
			SORT_put(tdbb, handle, reinterpret_cast<ULONG**>(&data));

			// Zero out the sort key. This solve a multitude of problems.

			memset(data, 0, m_map->smb_length);

			// Loop thru all field (keys and hangers on) involved in the sort.
			// Be careful to null field all unused bytes in the sort key.

			const smb_repeat* const end_item = m_map->smb_rpt + m_map->smb_count;
			for (smb_repeat* item = m_map->smb_rpt; item < end_item; item++)
			{
				to = item->smb_desc;
				to.dsc_address = data + (IPTR) to.dsc_address;
				bool flag = false;
				dsc* from = 0;

				if (item->smb_node)
				{
					from = EVL_expr(tdbb, item->smb_node);
					if (request->req_flags & req_null)
						flag = true;
				}
				else
				{
					from = &temp;

					record_param* const rpb = &request->req_rpb[item->smb_stream];

					if (item->smb_field_id < 0)
					{
						switch (item->smb_field_id)
						{
						case SMB_TRANS_ID:
							*reinterpret_cast<SLONG*>(to.dsc_address) = rpb->rpb_transaction_nr;
							break;
						case SMB_DBKEY:
							*reinterpret_cast<SINT64*>(to.dsc_address) = rpb->rpb_number.getValue();
							break;
						case SMB_DBKEY_VALID:
							*to.dsc_address = (UCHAR) rpb->rpb_number.isValid();
							break;
						default:
							fb_assert(false);
						}
						continue;
					}

					if (!EVL_field(rpb->rpb_relation, rpb->rpb_record, item->smb_field_id, from))
					{
						flag = true;
					}
				}

				*(data + item->smb_flag_offset) = flag ? TRUE : FALSE;

				if (!flag)
				{
					// If moving a TEXT item into the key portion of the sort record,
					// then want to sort by language dependent order.

					if (IS_INTL_DATA(&item->smb_desc) &&
						(USHORT)(IPTR) item->smb_desc.dsc_address < m_map->smb_key_length * sizeof(ULONG))
					{
						INTL_string_to_key(tdbb, INTL_INDEX_TYPE(&item->smb_desc), from, &to,
							(m_map->smb_flags & SMB_unique_sort ? INTL_KEY_UNIQUE : INTL_KEY_SORT));
					}
					else
					{
						MOV_move(tdbb, from, &to);
					}
				}
			}
		}

		SORT_sort(tdbb, handle);
	}
	catch (const Firebird::Exception&)
	{
		SORT_fini(handle);
		throw;
	}

	// For the sake of prudence, set all record parameter blocks to contain
	// the most recent format. This is will guarentee that all fields mapped
	// back to records by get_sort() have homes in the target record.

	if (records)
	{
		SSHORT stream = -1;

		const smb_repeat* const end_item = m_map->smb_rpt + m_map->smb_count;
		for (smb_repeat* item = m_map->smb_rpt; item < end_item; item++)
		{
			if (item->smb_node && item->smb_node->nod_type != nod_field)
				continue;

			if (item->smb_stream == stream)
				continue;

			stream = item->smb_stream;
			record_param* const rpb = &request->req_rpb[stream];

			if (rpb->rpb_relation)
			{
				VIO_record(tdbb, rpb, MET_current(tdbb, rpb->rpb_relation), tdbb->getDefaultPool());
			}
		}
	}

	return handle;
}

UCHAR* SortedStream::getData(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	ULONG* data = NULL;
	SORT_get(tdbb, impure->irsb_sort_handle, &data);

	return reinterpret_cast<UCHAR*>(data);
}

void SortedStream::mapData(thread_db* tdbb, jrd_req* request, UCHAR* data)
{
	dsc from, to;

	const smb_repeat* const end_item = m_map->smb_rpt + m_map->smb_count;

	for (smb_repeat* item = m_map->smb_rpt; item < end_item; item++)
	{
		const UCHAR flag = *(data + item->smb_flag_offset);
		from = item->smb_desc;
		from.dsc_address = data + (IPTR) from.dsc_address;
		jrd_nod* const node = item->smb_node;

		if (node && node->nod_type != nod_field)
			continue;

		// if moving a TEXT item into the key portion of the sort record,
		// then want to sort by language dependent order

		// in the case below a nod_field is being converted to
		// a sort key, there is a later nod_field in the item
		// list that contains the data to send back

		if (IS_INTL_DATA(&item->smb_desc) &&
			(USHORT)(IPTR) item->smb_desc.dsc_address < m_map->smb_key_length * sizeof(ULONG))
		{
			continue;
		}

		record_param* const rpb = &request->req_rpb[item->smb_stream];

		const SSHORT id = item->smb_field_id;

		if (id < 0)
		{
			switch (id)
			{
			case SMB_TRANS_ID:
				rpb->rpb_transaction_nr = *reinterpret_cast<SLONG*>(from.dsc_address);
				break;
			case SMB_DBKEY:
				rpb->rpb_number.setValue(*reinterpret_cast<SINT64*>(from.dsc_address));
				break;
			case SMB_DBKEY_VALID:
				rpb->rpb_number.setValid(*from.dsc_address != 0);
				break;
			default:
				fb_assert(false);
			}
			rpb->rpb_stream_flags |= RPB_s_refetch;
			continue;
		}

		Record* const record = rpb->rpb_record;

		if (record && !flag && !record->rec_format)
		{
			fb_assert(record->rec_fmt_bk);
			record->rec_format = record->rec_fmt_bk;
		}

		EVL_field(NULL, record, id, &to);

		if (flag)
		{
			SET_NULL(record, id);
		}
		else
		{
			MOV_move(tdbb, &from, &to);
			CLEAR_NULL(record, id);
		}
	}
}
