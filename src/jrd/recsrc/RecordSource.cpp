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
#include "../jrd/jrd.h"
#include "../jrd/btr.h"
#include "../jrd/intl.h"
#include "../jrd/req.h"
#include "../jrd/rse.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/dpm_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/rlck_proto.h"
#include "../jrd/vio_proto.h"
#include "../jrd/DataTypeUtil.h"

#include "RecordSource.h"

using namespace Firebird;
using namespace Jrd;


// Record source class
// -------------------

string RecordSource::printName(thread_db* tdbb, const string& name)
{
	ULONG nameLength = (ULONG) name.length();
	const UCHAR* namePtr = (UCHAR*) name.c_str();

	MoveBuffer nameBuffer;

	const CHARSET_ID charset = tdbb->getCharSet();
	if (charset != CS_METADATA && charset != CS_NONE)
	{
		const ULONG bufferLength = INTL_convert_bytes(tdbb, charset, NULL, 0,
													  CS_METADATA, namePtr, nameLength, ERR_post);
		nameBuffer.getBuffer(bufferLength);
		nameLength = INTL_convert_bytes(tdbb, charset, nameBuffer.begin(), bufferLength,
										CS_METADATA, namePtr, nameLength, ERR_post);

		namePtr = nameBuffer.begin();
	}

	return string(namePtr, nameLength);
}

string RecordSource::printIndent(unsigned level)
{
	fb_assert(level);

	const string indent(level * 4, ' ');
	return string("\n" + indent + "-> ");
}

void RecordSource::printInversion(thread_db* tdbb, const InversionNode* inversion,
								  string& plan, bool detailed, unsigned level, bool navigation)
{
	if (detailed)
		plan += printIndent(++level);

	switch (inversion->type)
	{
	case InversionNode::TYPE_AND:
		if (detailed)
			plan += "Bitmap And";
		printInversion(tdbb, inversion->node1, plan, detailed, level);
		printInversion(tdbb, inversion->node2, plan, detailed, level);
		break;

	case InversionNode::TYPE_OR:
	case InversionNode::TYPE_IN:
		if (detailed)
			plan += "Bitmap Or";
		printInversion(tdbb, inversion->node1, plan, detailed, level);
		printInversion(tdbb, inversion->node2, plan, detailed, level);
		break;

	case InversionNode::TYPE_DBKEY:
		if (detailed)
			plan += "DBKEY";
		break;

	case InversionNode::TYPE_INDEX:
		{
			MetaName indexName;
			MET_lookup_index(tdbb, indexName, inversion->retrieval->irb_relation->rel_name,
							 (USHORT) (inversion->retrieval->irb_index + 1));

			if (detailed)
			{
				if (!navigation)
					plan += "Bitmap" + printIndent(++level);

				plan += "Index \"" + printName(tdbb, indexName.c_str()) + "\" Scan";
			}
			else
			{
				plan += (plan.hasData() ? ", " : "") + printName(tdbb, indexName.c_str());
			}
		}
		break;

	default:
		fb_assert(false);
	}
}

RecordSource::~RecordSource()
{
}


// RecordStream class
// ------------------

RecordStream::RecordStream(CompilerScratch* csb, StreamType stream, const Format* format)
	: m_stream(stream), m_format(format ? format : csb->csb_rpt[stream].csb_format)
{
	fb_assert(m_format);
}

bool RecordStream::refetchRecord(thread_db* tdbb) const
{
	jrd_req* const request = tdbb->getRequest();
	jrd_tra* const transaction = request->req_transaction;

	record_param* const org_rpb = &request->req_rpb[m_stream];

	if (org_rpb->rpb_stream_flags & RPB_s_refetch)
	{
		if (!DPM_get(tdbb, org_rpb, LCK_read) ||
			!VIO_chase_record_version(tdbb, org_rpb, transaction, tdbb->getDefaultPool(), true))
		{
			return false;
		}

		VIO_data(tdbb, org_rpb, tdbb->getDefaultPool());

		org_rpb->rpb_stream_flags &= ~RPB_s_refetch;
	}

	return true;
}

bool RecordStream::lockRecord(thread_db* tdbb) const
{
	jrd_req* const request = tdbb->getRequest();
	jrd_tra* const transaction = request->req_transaction;

	record_param* const org_rpb = &request->req_rpb[m_stream];
	jrd_rel* const relation = org_rpb->rpb_relation;

	fb_assert(relation && !relation->rel_view_rse);

	RLCK_reserve_relation(tdbb, transaction, relation, true);

	return VIO_writelock(tdbb, org_rpb, transaction);
}

void RecordStream::markRecursive()
{
	m_recursive = true;
}

void RecordStream::findUsedStreams(StreamList& streams, bool /*expandAll*/) const
{
	if (!streams.exist(m_stream))
		streams.add(m_stream);
}

void RecordStream::invalidateRecords(jrd_req* request) const
{
	record_param* const rpb = &request->req_rpb[m_stream];
	rpb->rpb_number.setValid(false);
}

void RecordStream::nullRecords(thread_db* tdbb) const
{
	jrd_req* const request = tdbb->getRequest();
	record_param* const rpb = &request->req_rpb[m_stream];

	rpb->rpb_number.setValid(false);

	// Make sure a record block has been allocated. If there isn't
	// one, first find the format, then allocate the record block.

	Record* record = rpb->rpb_record;

	if (!record)
		record = VIO_record(tdbb, rpb, m_format, tdbb->getDefaultPool());

	if (record->rec_format)
		record->rec_fmt_bk = record->rec_format;

	record->rec_format = NULL;
}
