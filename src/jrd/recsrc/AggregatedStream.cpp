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
#include "../common/common.h"
#include "../jrd/jrd.h"
#include "../dsql/Nodes.h"
#include "../dsql/ExprNodes.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/exe_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/vio_proto.h"
#include "../jrd/Attachment.h"

#include "RecordSource.h"

using namespace Firebird;
using namespace Jrd;

// ------------------------
// Data access: aggregation
// ------------------------

// Note that we can have NULL order here, in case of window function with shouldCallWinPass
// returning true, with partition, and without order. Example: ROW_NUMBER() OVER (PARTITION BY N).
AggregatedStream::AggregatedStream(CompilerScratch* csb, UCHAR stream, const LegacyNodeArray* group,
			const MapNode* map, BaseBufferedStream* next, const LegacyNodeArray* order)
	: RecordStream(csb, stream),
	  m_bufferedStream(next),
	  m_next(m_bufferedStream),
	  m_group(group),
	  m_map(map),
	  m_order(order),
	  m_winPassMap(csb->csb_pool)
{
	init(csb);
}

AggregatedStream::AggregatedStream(CompilerScratch* csb, UCHAR stream, const LegacyNodeArray* group,
			const MapNode* map, RecordSource* next)
	: RecordStream(csb, stream),
	  m_bufferedStream(NULL),
	  m_next(next),
	  m_group(group),
	  m_map(map),
	  m_order(NULL),
	  m_winPassMap(csb->csb_pool)
{
	init(csb);
}

void AggregatedStream::open(thread_db* tdbb) const
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = request->getImpure<Impure>(m_impure);

	impure->irsb_flags = irsb_open;

	impure->state = STATE_GROUPING;
	impure->pending = 0;
	VIO_record(tdbb, &request->req_rpb[m_stream], m_format, tdbb->getDefaultPool());

	m_next->open(tdbb);
}

void AggregatedStream::close(thread_db* tdbb) const
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

bool AggregatedStream::getRecord(thread_db* tdbb) const
{
	jrd_req* const request = tdbb->getRequest();
	record_param* const rpb = &request->req_rpb[m_stream];
	Impure* const impure = request->getImpure<Impure>(m_impure);

	if (!(impure->irsb_flags & irsb_open))
	{
		rpb->rpb_number.setValid(false);
		return false;
	}

	if (m_bufferedStream)	// Is that a window stream?
	{
		FB_UINT64 position = m_bufferedStream->getPosition(request);

		if (impure->pending == 0)
		{
			if (impure->state == STATE_PENDING)
				m_bufferedStream->getRecord(tdbb);

			impure->state = evaluateGroup(tdbb, impure->state);

			if (impure->state == STATE_PROCESS_EOF)
			{
				rpb->rpb_number.setValid(false);
				return false;
			}

			impure->pending = m_bufferedStream->getPosition(request) - position -
				(impure->state == STATE_EOF_FOUND ? 0 : 1);
			m_bufferedStream->locate(tdbb, position);
		}

		if (m_winPassMap.hasData())
		{
			SlidingWindow window(tdbb, m_bufferedStream, m_group, request);
			dsc* desc;

			for (const jrd_nod* const* ptr = m_winPassMap.begin(); ptr != m_winPassMap.end(); ++ptr)
			{
				const jrd_nod* from = (*ptr)->nod_arg[e_asgn_from];
				fb_assert(from->nod_type == nod_class_exprnode_jrd);
				const AggNode* aggNode = reinterpret_cast<const AggNode*>(from->nod_arg[0]);

				const FieldNode* field = ExprNode::as<FieldNode>((*ptr)->nod_arg[e_asgn_to]);
				const USHORT id = field->fieldId;
				Record* record = request->req_rpb[field->fieldStream].rpb_record;

				desc = aggNode->winPass(tdbb, request, &window);

				if (!desc)
					SET_NULL(record, id);
				else
				{
					MOV_move(tdbb, desc, EVL_assign_to(tdbb, (*ptr)->nod_arg[e_asgn_to]));
					CLEAR_NULL(record, id);
				}
			}
		}

		if (impure->pending > 0)
			--impure->pending;

		m_bufferedStream->getRecord(tdbb);

		// If there is no group, we should reassign the map items.
		if (!m_group)
		{
			const NestConst<jrd_nod>* const end = m_map->items.end();

			for (const NestConst<jrd_nod>* ptr = m_map->items.begin(); ptr != end; ++ptr)
			{
				const jrd_nod* from = (*ptr)->nod_arg[e_asgn_from];
				const AggNode* aggNode = ExprNode::as<AggNode>(from);

				if (!aggNode)
					EXE_assignment(tdbb, *ptr);
			}
		}
	}
	else
	{
		impure->state = evaluateGroup(tdbb, impure->state);

		if (impure->state == STATE_PROCESS_EOF)
		{
			rpb->rpb_number.setValid(false);
			return false;
		}
	}

	rpb->rpb_number.setValid(true);
	return true;
}

bool AggregatedStream::refetchRecord(thread_db* tdbb) const
{
	return m_next->refetchRecord(tdbb);
}

bool AggregatedStream::lockRecord(thread_db* /*tdbb*/) const
{
	status_exception::raise(Arg::Gds(isc_record_lock_not_supp));
	return false; // compiler silencer
}

void AggregatedStream::dump(thread_db* tdbb, UCharBuffer& buffer) const
{
	buffer.add(isc_info_rsb_begin);

	buffer.add(isc_info_rsb_type);
	buffer.add(isc_info_rsb_aggregate);

	m_next->dump(tdbb, buffer);

	buffer.add(isc_info_rsb_end);
}

void AggregatedStream::markRecursive()
{
	m_next->markRecursive();
}

void AggregatedStream::invalidateRecords(jrd_req* request) const
{
	m_next->invalidateRecords(request);
}

void AggregatedStream::findUsedStreams(StreamsArray& streams) const
{
	RecordStream::findUsedStreams(streams);
	if (m_bufferedStream)
		m_bufferedStream->findUsedStreams(streams);
}

void AggregatedStream::init(CompilerScratch* csb)
{
	fb_assert(m_map && m_next);
	m_impure = CMP_impure(csb, sizeof(Impure));

	// Separate nodes that requires the winPass call.

	const NestConst<jrd_nod>* const end = m_map->items.end();

	for (const NestConst<jrd_nod>* ptr = m_map->items.begin(); ptr != end; ++ptr)
	{
		const jrd_nod* from = (*ptr)->nod_arg[e_asgn_from];
		const AggNode* aggNode = ExprNode::as<AggNode>(from);

		if (aggNode && aggNode->shouldCallWinPass())
			m_winPassMap.add(*ptr);
	}
}

// Compute the next aggregated record of a value group. evlGroup is driven by, and returns, a state
// variable.
AggregatedStream::State AggregatedStream::evaluateGroup(thread_db* tdbb, AggregatedStream::State state) const
{
	jrd_req* const request = tdbb->getRequest();

	if (--tdbb->tdbb_quantum < 0)
		JRD_reschedule(tdbb, 0, true);

	impure_value vtemp;
	vtemp.vlu_string = NULL;

	// if we found the last record last time, we're all done

	if (state == STATE_EOF_FOUND)
		return STATE_PROCESS_EOF;

	try
	{
		const NestConst<jrd_nod>* ptr, *end;

		// If there isn't a record pending, open the stream and get one

		if (!m_order || state == STATE_PROCESS_EOF || state == STATE_GROUPING)
		{
			// Initialize the aggregate record

			for (ptr = m_map->items.begin(), end = m_map->items.end(); ptr != end; ++ptr)
			{
				const jrd_nod* from = (*ptr)->nod_arg[e_asgn_from];

				switch (from->nod_type)
				{
					case nod_class_exprnode_jrd:
					{
						const AggNode* aggNode = ExprNode::as<AggNode>(from);
						if (aggNode)
							aggNode->aggInit(tdbb, request);
						else if (ExprNode::is<LiteralNode>(from))
							EXE_assignment(tdbb, *ptr);
						break;
					}

					default:    // Shut up some compiler warnings
						break;
				}
			}
		}

		if (state == STATE_PROCESS_EOF || state == STATE_GROUPING)
		{
			if (!m_next->getRecord(tdbb))
			{
				if (m_group)
				{
					finiDistinct(tdbb, request);
					return STATE_PROCESS_EOF;
				}

				state = STATE_EOF_FOUND;
			}
		}

		dsc* desc;

		if (m_group)
		{
			for (ptr = m_group->begin(), end = m_group->end(); ptr != end; ++ptr)
			{
				const jrd_nod* from = *ptr;
				impure_value* impure = request->getImpure<impure_value>(from->nod_impure);

				desc = EVL_expr(tdbb, from);

				if (request->req_flags & req_null)
					impure->vlu_desc.dsc_address = NULL;
				else
					EVL_make_value(tdbb, desc, impure);
			}
		}

		if (m_order)
		{
			for (ptr = m_order->begin(), end = m_order->end(); ptr != end; ++ptr)
			{
				const jrd_nod* from = *ptr;
				impure_value* impure = request->getImpure<impure_value>(from->nod_impure);

				desc = EVL_expr(tdbb, from);

				if (request->req_flags & req_null)
					impure->vlu_desc.dsc_address = NULL;
				else
					EVL_make_value(tdbb, desc, impure);
			}
		}

		// Loop thru records until either a value change or EOF

		bool first = true;

		while (state != STATE_EOF_FOUND)
		{
			state = STATE_PENDING;

			if (first)
				first = false;
			else
			{
				// In the case of a group by, look for a change in value of any of
				// the columns; if we find one, stop aggregating and return what we have.

				if (m_group)
				{
					for (ptr = m_group->begin(), end = m_group->end(); ptr != end; ++ptr)
					{
						const jrd_nod* from = *ptr;
						impure_value* impure = request->getImpure<impure_value>(from->nod_impure);

						if (impure->vlu_desc.dsc_address)
							EVL_make_value(tdbb, &impure->vlu_desc, &vtemp);
						else
							vtemp.vlu_desc.dsc_address = NULL;

						desc = EVL_expr(tdbb, from);

						if (request->req_flags & req_null)
						{
							impure->vlu_desc.dsc_address = NULL;
							if (vtemp.vlu_desc.dsc_address)
							{
								if (m_order)
									state = STATE_GROUPING;
								goto break_out;
							}
						}
						else
						{
							EVL_make_value(tdbb, desc, impure);
							if (!vtemp.vlu_desc.dsc_address || MOV_compare(&vtemp.vlu_desc, desc))
							{
								if (m_order)
									state = STATE_GROUPING;
								goto break_out;
							}
						}
					}
				}

				if (m_order)
				{
					for (ptr = m_order->begin(), end = m_order->end(); ptr != end; ++ptr)
					{
						const jrd_nod* from = *ptr;
						impure_value* impure = request->getImpure<impure_value>(from->nod_impure);

						if (impure->vlu_desc.dsc_address)
							EVL_make_value(tdbb, &impure->vlu_desc, &vtemp);
						else
							vtemp.vlu_desc.dsc_address = NULL;

						desc = EVL_expr(tdbb, from);

						if (request->req_flags & req_null)
						{
							impure->vlu_desc.dsc_address = NULL;
							if (vtemp.vlu_desc.dsc_address)
								goto break_out;
						}
						else
						{
							EVL_make_value(tdbb, desc, impure);
							if (!vtemp.vlu_desc.dsc_address || MOV_compare(&vtemp.vlu_desc, desc))
								goto break_out;
						}
					}
				}
			}

			// go through and compute all the aggregates on this record

			for (ptr = m_map->items.begin(), end = m_map->items.end(); ptr != end; ++ptr)
			{
				const jrd_nod* from = (*ptr)->nod_arg[e_asgn_from];
				const AggNode* aggNode = ExprNode::as<AggNode>(from);

				if (aggNode)
				{
					aggNode->aggPass(tdbb, request);

					// If a max or min has been mapped to an index, then the first record is the EOF.
					if (aggNode->indexed)
						state = STATE_EOF_FOUND;
				}
				else
					EXE_assignment(tdbb, *ptr);
			}

			if (state == STATE_EOF_FOUND)
				break;

			if (!m_next->getRecord(tdbb))
				state = STATE_EOF_FOUND;
		}

		break_out:

		// Finish up any residual computations and get out

		delete vtemp.vlu_string;

		for (ptr = m_map->items.begin(), end = m_map->items.end(); ptr != end; ++ptr)
		{
			const jrd_nod* from = (*ptr)->nod_arg[e_asgn_from];
			const AggNode* aggNode = ExprNode::as<AggNode>(from);

			if (aggNode)
			{
				const FieldNode* field = ExprNode::as<FieldNode>((*ptr)->nod_arg[e_asgn_to]);
				const USHORT id = field->fieldId;
				Record* record = request->req_rpb[field->fieldStream].rpb_record;

				desc = aggNode->execute(tdbb, request);
				if (!desc || !desc->dsc_dtype)
					SET_NULL(record, id);
				else
				{
					MOV_move(tdbb, desc, EVL_assign_to(tdbb, (*ptr)->nod_arg[e_asgn_to]));
					CLEAR_NULL(record, id);
				}
			}
		}
	}
	catch (const Exception&)
	{
		finiDistinct(tdbb, request);
		throw;
	}

	return state;
}

// Finalize a sort for distinct aggregate
void AggregatedStream::finiDistinct(thread_db* tdbb, jrd_req* request) const
{
	const NestConst<jrd_nod>* ptr, *end;

	for (ptr = m_map->items.begin(), end = m_map->items.end(); ptr != end; ++ptr)
	{
		const jrd_nod* const from = (*ptr)->nod_arg[e_asgn_from];

		const AggNode* aggNode = ExprNode::as<AggNode>(from);
		if (aggNode)
			aggNode->aggFinish(tdbb, request);
	}
}


SlidingWindow::SlidingWindow(thread_db* aTdbb, const BaseBufferedStream* aStream,
			const LegacyNodeArray* aGroup, jrd_req* aRequest)
	: tdbb(aTdbb),	// Note: instanciate the class only as local variable
	  stream(aStream),
	  group(aGroup),
	  request(aRequest)
{
	savedPosition = stream->getPosition(request);
}

SlidingWindow::~SlidingWindow()
{
	if (!moved)
		return;

	for (impure_value* impure = partitionKeys.begin(); impure != partitionKeys.end(); ++impure)
		delete impure->vlu_string;

	// Position the stream where we received it.
	stream->locate(tdbb, savedPosition);
}

// Move in the window without pass partition boundaries.
bool SlidingWindow::move(SINT64 delta)
{
	const SINT64 newPosition = SINT64(savedPosition) + delta;

	// If we try to go out of bounds, no need to check the partition.
	if (newPosition < 0 || newPosition >= (SINT64) stream->getCount(request))
		return false;

	if (!group)
	{
		// No partition, we may go everywhere.

		moved = true;

		stream->locate(tdbb, newPosition);

		if (!stream->getRecord(tdbb))
		{
			fb_assert(false);
			return false;
		}

		return true;
	}

	if (!moved)
	{
		// This is our first move. We should cache the partition values, so subsequente moves didn't
		// need to evaluate them again.

		if (!stream->getRecord(tdbb))
		{
			fb_assert(false);
			return false;
		}

		try
		{
			impure_value* impure = partitionKeys.getBuffer(group->getCount());
			memset(impure, 0, sizeof(impure_value) * group->getCount());

			const NestConst<jrd_nod>* const end = group->end();
			dsc* desc;

			for (const NestConst<jrd_nod>* ptr = group->begin(); ptr < end; ++ptr, ++impure)
			{
				const jrd_nod* from = *ptr;
				desc = EVL_expr(tdbb, from);

				if (request->req_flags & req_null)
					impure->vlu_desc.dsc_address = NULL;
				else
					EVL_make_value(tdbb, desc, impure);
			}
		}
		catch (const Exception&)
		{
			stream->locate(tdbb, savedPosition);	// Reposition for a new try.
			throw;
		}

		moved = true;
	}

	stream->locate(tdbb, newPosition);

	if (!stream->getRecord(tdbb))
	{
		fb_assert(false);
		return false;
	}

	// Verify if we're still inside the same partition.

	impure_value* impure = partitionKeys.begin();
	dsc* desc;
	const NestConst<jrd_nod>* const end = group->end();

	for (const NestConst<jrd_nod>* ptr = group->begin(); ptr != end; ++ptr, ++impure)
	{
		const jrd_nod* from = *ptr;
		desc = EVL_expr(tdbb, from);

		if (request->req_flags & req_null)
		{
			if (impure->vlu_desc.dsc_address)
				return false;
		}
		else
		{
			if (!impure->vlu_desc.dsc_address || MOV_compare(&impure->vlu_desc, desc) != 0)
				return false;
		}
	}

	return true;
}
