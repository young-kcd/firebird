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
 * Adriano dos Santos Fernandes
 */

#include "firebird.h"
#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/exe_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/vio_proto.h"
#include "../jrd/Attachment.h"

#include "RecordSource.h"

using namespace Firebird;
using namespace Jrd;

// -----------------------------
// Data access: window functions
// -----------------------------

OrderedWindowStream::OrderedWindowStream(CompilerScratch* csb, UCHAR stream,
			jrd_nod* group, jrd_nod* order, jrd_nod* const map, RecordSource* next)
	: RecordStream(csb, stream), m_next(FB_NEW(csb->csb_pool) BufferedStream(csb, next)),
	  m_group(group), m_order(order), m_map(map)
{
	fb_assert(m_map && m_next);
	fb_assert(m_order);

	m_impure = CMP_impure(csb, sizeof(Impure));
}

void OrderedWindowStream::open(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	impure->irsb_flags = irsb_open;

	impure->state = STATE_GROUPING;
	impure->pending = 0;
	VIO_record(tdbb, &request->req_rpb[m_stream], m_format, tdbb->getDefaultPool());

	m_next->open(tdbb);
}

void OrderedWindowStream::close(thread_db* tdbb)
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

bool OrderedWindowStream::getRecord(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();
	record_param* const rpb = &request->req_rpb[m_stream];
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	if (!(impure->irsb_flags & irsb_open))
	{
		rpb->rpb_number.setValid(false);
		return false;
	}

	if (impure->pending == 0)
	{
		FB_UINT64 position = m_next->getPosition(request);

		if (impure->state == STATE_PENDING)
			m_next->getRecord(tdbb);

		impure->state = evaluateGroup(tdbb, impure->state);

		if (impure->state == STATE_PROCESS_EOF)
		{
			rpb->rpb_number.setValid(false);
			return false;
		}

		impure->pending = m_next->getPosition(request) - position -
			(impure->state == STATE_EOF_FOUND ? 0 : 1);
		m_next->locate(tdbb, position);
	}

	if (impure->pending > 0)
		--impure->pending;
	m_next->getRecord(tdbb);

	rpb->rpb_number.setValid(true);
	return true;
}

bool OrderedWindowStream::refetchRecord(thread_db* tdbb)
{
	return m_next->refetchRecord(tdbb);
}

bool OrderedWindowStream::lockRecord(thread_db* tdbb)
{
	status_exception::raise(Arg::Gds(isc_record_lock_not_supp));
	return false; // compiler silencer
}

void OrderedWindowStream::dump(thread_db* tdbb, UCharBuffer& buffer)
{
	buffer.add(isc_info_rsb_begin);

	buffer.add(isc_info_rsb_type);
	buffer.add(isc_info_rsb_aggregate);

	m_next->dump(tdbb, buffer);

	buffer.add(isc_info_rsb_end);
}

void OrderedWindowStream::markRecursive()
{
	m_next->markRecursive();
}

void OrderedWindowStream::invalidateRecords(jrd_req* request)
{
	m_next->invalidateRecords(request);
}

void OrderedWindowStream::findUsedStreams(StreamsArray& streams)
{
	RecordStream::findUsedStreams(streams);
	m_next->findUsedStreams(streams);
}

// Compute the next aggregated record of a value group. evlGroup is driven by, and returns, a state
// variable.
OrderedWindowStream::State OrderedWindowStream::evaluateGroup(thread_db* tdbb,
	OrderedWindowStream::State state)
{
	jrd_req* const request = tdbb->getRequest();

	if (--tdbb->tdbb_quantum < 0)
		JRD_reschedule(tdbb, 0, true);

	impure_value vtemp;
	vtemp.vlu_string = NULL;

	jrd_nod** ptr;
	const jrd_nod* const* end;

	// if we found the last record last time, we're all done

	if (state == STATE_EOF_FOUND)
		return STATE_PROCESS_EOF;

	// If there isn't a record pending, open the stream and get one

	if (state == STATE_PROCESS_EOF || state == STATE_GROUPING)
	{
		// Initialize the aggregate record

		for (ptr = m_map->nod_arg, end = ptr + m_map->nod_count; ptr < end; ptr++)
		{
			const jrd_nod* from = (*ptr)->nod_arg[e_asgn_from];
			impure_value_ex* impure = (impure_value_ex*) ((SCHAR*) request + from->nod_impure);
			impure->vlux_count = 0;

			switch (from->nod_type)
			{
			case nod_agg_average:
				impure->vlu_desc.dsc_dtype = DEFAULT_DOUBLE;
				impure->vlu_desc.dsc_length = sizeof(double);
				impure->vlu_desc.dsc_sub_type = 0;
				impure->vlu_desc.dsc_scale = 0;
				impure->vlu_desc.dsc_address = (UCHAR*) &impure->vlu_misc.vlu_double;
				impure->vlu_misc.vlu_double = 0;
				break;

			case nod_agg_average2:
				// Initialize the result area as an int64.  If the field being
				// averaged is approximate numeric, the first call to add2 will
				// convert the descriptor to double.
				impure->make_int64(0, from->nod_scale);
				break;

			case nod_agg_total:
				impure->make_long(0);
				break;

			case nod_agg_total2:
				// Initialize the result area as an int64.  If the field being
				// averaged is approximate numeric, the first call to add2 will
				// convert the descriptor to double.
				impure->make_int64(0, from->nod_scale);
				break;

			case nod_agg_min:
			case nod_agg_min_indexed:
			case nod_agg_max:
			case nod_agg_max_indexed:
				impure->vlu_desc.dsc_dtype = 0;
				break;

			case nod_agg_count:
			case nod_agg_count2:
				impure->make_long(0);
				break;

			case nod_literal:	// pjpg 20001124
				EXE_assignment(tdbb, *ptr);
				break;

			default:    // Shut up some compiler warnings
				break;
			}
		}

		if (!m_next->getRecord(tdbb))
		{
			if (m_group)
				return STATE_PROCESS_EOF;

			state = STATE_EOF_FOUND;
		}
	}

	dsc* desc;

	if (m_group)
	{
		for (ptr = m_group->nod_arg, end = ptr + m_group->nod_count; ptr < end; ptr++)
		{
			jrd_nod* from = *ptr;
			impure_value_ex* impure = (impure_value_ex*) ((SCHAR*) request + from->nod_impure);
			desc = EVL_expr(tdbb, from);
			if (request->req_flags & req_null)
				impure->vlu_desc.dsc_address = NULL;
			else
				EVL_make_value(tdbb, desc, impure);
		}
	}

	for (ptr = m_order->nod_arg, end = ptr + m_order->nod_count; ptr < end; ptr++)
	{
		jrd_nod* from = *ptr;
		impure_value_ex* impure = (impure_value_ex*) ((SCHAR*) request + from->nod_impure);
		desc = EVL_expr(tdbb, from);
		if (request->req_flags & req_null)
			impure->vlu_desc.dsc_address = NULL;
		else
			EVL_make_value(tdbb, desc, impure);
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
				for (ptr = m_group->nod_arg, end = ptr + m_group->nod_count; ptr < end; ptr++)
				{
					jrd_nod* from = *ptr;
					impure_value_ex* impure = (impure_value_ex*) ((SCHAR*) request + from->nod_impure);

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
							state = STATE_GROUPING;
							goto break_out;
						}
					}
					else
					{
						EVL_make_value(tdbb, desc, impure);
						if (!vtemp.vlu_desc.dsc_address || MOV_compare(&vtemp.vlu_desc, desc))
						{
							state = STATE_GROUPING;
							goto break_out;
						}
					}
				}
			}

			for (ptr = m_order->nod_arg, end = ptr + m_order->nod_count; ptr < end; ptr++)
			{
				jrd_nod* from = *ptr;
				impure_value_ex* impure = (impure_value_ex*) ((SCHAR*) request + from->nod_impure);

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

		// go through and compute all the aggregates on this record

		for (ptr = m_map->nod_arg, end = ptr + m_map->nod_count; ptr < end; ptr++)
		{
			jrd_nod* from = (*ptr)->nod_arg[e_asgn_from];
			impure_value_ex* impure = (impure_value_ex*) ((SCHAR*) request + from->nod_impure);

			switch (from->nod_type)
			{
			case nod_agg_min:
			case nod_agg_min_indexed:
			case nod_agg_max:
			case nod_agg_max_indexed:
				{
					desc = EVL_expr(tdbb, from->nod_arg[0]);
					if (request->req_flags & req_null)
						break;

					// if a max or min has been mapped to an index,
					// then the first record is the EOF

					if (from->nod_type == nod_agg_max_indexed || from->nod_type == nod_agg_min_indexed)
						state = STATE_EOF_FOUND;

					++impure->vlux_count;
					if (!impure->vlu_desc.dsc_dtype)
					{
						EVL_make_value(tdbb, desc, impure);
						break;
					}

					const SLONG result = MOV_compare(desc, &impure->vlu_desc);

					if ((result > 0 &&
							(from->nod_type == nod_agg_max || from->nod_type == nod_agg_max_indexed)) ||
						(result < 0 &&
							(from->nod_type == nod_agg_min || from->nod_type == nod_agg_min_indexed)))
					{
						EVL_make_value(tdbb, desc, impure);
					}

					break;
				}

			case nod_agg_total:
			case nod_agg_average:
				desc = EVL_expr(tdbb, from->nod_arg[0]);
				if (request->req_flags & req_null)
					break;
				++impure->vlux_count;
				EVL_add(desc, from, impure);
				break;

			case nod_agg_total2:
			case nod_agg_average2:
				desc = EVL_expr(tdbb, from->nod_arg[0]);
				if (request->req_flags & req_null)
					break;
				++impure->vlux_count;
				EVL_add2(desc, from, impure);
				break;

			case nod_agg_count2:
				++impure->vlux_count;
				desc = EVL_expr(tdbb, from->nod_arg[0]);
				if (request->req_flags & req_null)
					break;

			case nod_agg_count:
				++impure->vlux_count;
				++impure->vlu_misc.vlu_long;
				break;

			default:
				EXE_assignment(tdbb, *ptr);
			}
		}

		if (state == STATE_EOF_FOUND)
			break;

		if (!m_next->getRecord(tdbb))
			state = STATE_EOF_FOUND;
	}

	break_out:

	// Finish up any residual computations and get out

	delete vtemp.vlu_string;

	dsc temp;
	double d;
	SINT64 i;

	for (ptr = m_map->nod_arg, end = ptr + m_map->nod_count; ptr < end; ptr++)
	{
		jrd_nod* from = (*ptr)->nod_arg[e_asgn_from];
		jrd_nod* field = (*ptr)->nod_arg[e_asgn_to];
		const USHORT id = (USHORT)(IPTR) field->nod_arg[e_fld_id];
		Record* record = request->req_rpb[(int) (IPTR) field->nod_arg[e_fld_stream]].rpb_record;
		impure_value_ex* impure = (impure_value_ex*) ((SCHAR*) request + from->nod_impure);

		switch (from->nod_type)
		{
		case nod_agg_min:
		case nod_agg_min_indexed:
		case nod_agg_max:
		case nod_agg_max_indexed:
		case nod_agg_total:
		case nod_agg_total2:
			if (!impure->vlux_count)
			{
				SET_NULL(record, id);
				break;
			}
			// If vlux_count is non-zero, we need to fall through.

		case nod_agg_count:
		case nod_agg_count2:
			if (!impure->vlu_desc.dsc_dtype)
			{
				SET_NULL(record, id);
			}
			else
			{
				MOV_move(tdbb, &impure->vlu_desc, EVL_assign_to(tdbb, field));
				CLEAR_NULL(record, id);
			}
			break;

		case nod_agg_average:
			if (!impure->vlux_count)
			{
				SET_NULL(record, id);
				break;
			}
			CLEAR_NULL(record, id);
			temp.dsc_dtype = DEFAULT_DOUBLE;
			temp.dsc_length = sizeof(double);
			temp.dsc_scale = 0;
			temp.dsc_sub_type = 0;
			temp.dsc_address = (UCHAR*) &d;
			d = MOV_get_double(&impure->vlu_desc) / impure->vlux_count;
			MOV_move(tdbb, &temp, EVL_assign_to(tdbb, field));
			break;

		case nod_agg_average2:
			if (!impure->vlux_count)
			{
				SET_NULL(record, id);
				break;
			}

			CLEAR_NULL(record, id);
			temp.dsc_sub_type = 0;

			if (dtype_int64 == impure->vlu_desc.dsc_dtype)
			{
				temp.dsc_dtype = dtype_int64;
				temp.dsc_length = sizeof(SINT64);
				temp.dsc_scale = impure->vlu_desc.dsc_scale;
				temp.dsc_address = (UCHAR*) &i;
				i = *((SINT64*) impure->vlu_desc.dsc_address) / impure->vlux_count;
			}
			else
			{
				temp.dsc_dtype = DEFAULT_DOUBLE;
				temp.dsc_length = sizeof(double);
				temp.dsc_scale = 0;
				temp.dsc_address = (UCHAR*) &d;
				d = MOV_get_double(&impure->vlu_desc) / impure->vlux_count;
			}

			MOV_move(tdbb, &temp, EVL_assign_to(tdbb, field));
			break;

		default:	// Shut up some compiler warnings
			break;
		}
	}

	return state;
}
