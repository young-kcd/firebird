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
#include "../jrd/blb_proto.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/exe_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/sort_proto.h"
#include "../jrd/vio_proto.h"
#include "../jrd/Attachment.h"

#include "RecordSource.h"

using namespace Firebird;
using namespace Jrd;

// ------------------------
// Data access: aggregation
// ------------------------

AggregatedStream::AggregatedStream(CompilerScratch* csb, UCHAR stream, jrd_nod* group,
			jrd_nod* const map, RecordSource* next)
	: RecordStream(csb, stream), m_next(next), m_group(group), m_map(map)
{
	fb_assert(m_map && m_next);

	m_impure = CMP_impure(csb, sizeof(Impure));
}

void AggregatedStream::open(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	impure->irsb_flags = irsb_open;

	impure->state = STATE_GROUPING;
	VIO_record(tdbb, &request->req_rpb[m_stream], m_format, tdbb->getDefaultPool());
}

void AggregatedStream::close(thread_db* tdbb)
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

bool AggregatedStream::getRecord(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();
	record_param* const rpb = &request->req_rpb[m_stream];
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	if (!(impure->irsb_flags & irsb_open))
	{
		rpb->rpb_number.setValid(false);
		return false;
	}

	impure->state = evaluateGroup(tdbb, impure->state);

	if (impure->state == STATE_PROCESS_EOF)
	{
		rpb->rpb_number.setValid(false);
		return false;
	}

	rpb->rpb_number.setValid(true);
	return true;
}

bool AggregatedStream::refetchRecord(thread_db* tdbb)
{
	return m_next->refetchRecord(tdbb);
}

bool AggregatedStream::lockRecord(thread_db* tdbb)
{
	status_exception::raise(Arg::Gds(isc_record_lock_not_supp));
	return false; // compiler silencer
}

void AggregatedStream::dump(thread_db* tdbb, UCharBuffer& buffer)
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

void AggregatedStream::invalidateRecords(jrd_req* request)
{
	m_next->invalidateRecords(request);
}

// Compute the next aggregated record of a value group. evlGroup is driven by, and returns, a state
// variable.
AggregatedStream::State AggregatedStream::evaluateGroup(thread_db* tdbb, AggregatedStream::State state)
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
	{
		return STATE_PROCESS_EOF;
	}

	try
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
			case nod_agg_average_distinct:
				impure->vlu_desc.dsc_dtype = DEFAULT_DOUBLE;
				impure->vlu_desc.dsc_length = sizeof(double);
				impure->vlu_desc.dsc_sub_type = 0;
				impure->vlu_desc.dsc_scale = 0;
				impure->vlu_desc.dsc_address = (UCHAR*) &impure->vlu_misc.vlu_double;
				impure->vlu_misc.vlu_double = 0;
				if (from->nod_type == nod_agg_average_distinct)
				{
					// Initialize a sort to reject duplicate values
					initDistinct(request, from);
				}
				break;

			case nod_agg_average2:
			case nod_agg_average_distinct2:
				// Initialize the result area as an int64.  If the field being
				// averaged is approximate numeric, the first call to add2 will
				// convert the descriptor to double.
				impure->make_int64(0, from->nod_scale);
				if (from->nod_type == nod_agg_average_distinct2)
				{
					// Initialize a sort to reject duplicate values
					initDistinct(request, from);
				}
				break;

			case nod_agg_total:
			case nod_agg_total_distinct:
				impure->make_long(0);
				if (from->nod_type == nod_agg_total_distinct)
				{
					// Initialize a sort to reject duplicate values
					initDistinct(request, from);
				}
				break;

			case nod_agg_total2:
			case nod_agg_total_distinct2:
				// Initialize the result area as an int64.  If the field being
				// averaged is approximate numeric, the first call to add2 will
				// convert the descriptor to double.
				impure->make_int64(0, from->nod_scale);
				if (from->nod_type == nod_agg_total_distinct2)
				{
					// Initialize a sort to reject duplicate values
					initDistinct(request, from);
				}
				break;

			case nod_agg_min:
			case nod_agg_min_indexed:
			case nod_agg_max:
			case nod_agg_max_indexed:
				impure->vlu_desc.dsc_dtype = 0;
				break;

			case nod_agg_count:
			case nod_agg_count2:
			case nod_agg_count_distinct:
				impure->make_long(0);
				if (from->nod_type == nod_agg_count_distinct)
				{
					// Initialize a sort to reject duplicate values
					initDistinct(request, from);
				}
				break;

			case nod_agg_list:
			case nod_agg_list_distinct:
				// We don't know here what should be the sub-type and text-type.
				// Defer blob creation for when first record is found.
				impure->vlu_blob = NULL;
				impure->vlu_desc.dsc_dtype = 0;

				if (from->nod_type == nod_agg_list_distinct)
				{
					// Initialize a sort to reject duplicate values
					initDistinct(request, from);
				}
				break;

			case nod_literal:	// pjpg 20001124
				EXE_assignment(tdbb, *ptr);
				break;

			default:    // Shut up some compiler warnings
				break;
			}
		}

		// If there isn't a record pending, open the stream and get one

		if (state == STATE_PROCESS_EOF || state == STATE_GROUPING)
		{
			m_next->open(tdbb);
			if (!m_next->getRecord(tdbb))
			{
				if (m_group)
				{
					finiDistinct(request);
					return STATE_PROCESS_EOF;
				}
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

				case nod_agg_list:
					{
						MoveBuffer buffer;
						UCHAR* temp;
						int len;

						desc = EVL_expr(tdbb, from->nod_arg[0]);
						if (request->req_flags & req_null)
							break;

						if (!impure->vlu_blob)
						{
							impure->vlu_blob = BLB_create(tdbb, request->req_transaction,
								&impure->vlu_misc.vlu_bid);
							impure->vlu_desc.makeBlob(desc->getBlobSubType(), desc->getTextType(),
								(ISC_QUAD*) &impure->vlu_misc.vlu_bid);
						}

						if (impure->vlux_count)
						{
							const dsc* const delimiter = EVL_expr(tdbb, from->nod_arg[1]);
							if (request->req_flags & req_null)
							{
								// mark the result as NULL
								impure->vlu_desc.dsc_dtype = 0;
								break;
							}

							len = MOV_make_string2(tdbb, delimiter, impure->vlu_desc.getTextType(),
								&temp, buffer, false);
							BLB_put_data(tdbb, impure->vlu_blob, temp, len);
						}
						++impure->vlux_count;
						len = MOV_make_string2(tdbb, desc, impure->vlu_desc.getTextType(),
							&temp, buffer, false);
						BLB_put_data(tdbb, impure->vlu_blob, temp, len);
						break;
					}

				case nod_agg_count_distinct:
				case nod_agg_total_distinct:
				case nod_agg_average_distinct:
				case nod_agg_average_distinct2:
				case nod_agg_total_distinct2:
				case nod_agg_list_distinct:
					{
						desc = EVL_expr(tdbb, from->nod_arg[0]);
						if (request->req_flags & req_null)
							break;
						// "Put" the value to sort.
						const size_t asbIndex = (from->nod_type == nod_agg_list_distinct) ? 2 : 1;
						AggregateSort* asb = (AggregateSort*) from->nod_arg[asbIndex];
						impure_agg_sort* asbImpure =
							(impure_agg_sort*) ((SCHAR*) request + asb->nod_impure);
						UCHAR* data;
						SORT_put(tdbb, asbImpure->iasb_sort_handle, reinterpret_cast<ULONG**>(&data));

						MOVE_CLEAR(data, asb->asb_length);

						if (asb->asb_intl)
						{
							// convert to an international byte array
							dsc to;
							to.dsc_dtype = dtype_text;
							to.dsc_flags = 0;
							to.dsc_sub_type = 0;
							to.dsc_scale = 0;
							to.dsc_ttype() = ttype_sort_key;
							to.dsc_length = asb->asb_key_desc[0].skd_length;
							to.dsc_address = data;
							INTL_string_to_key(tdbb, INTL_TEXT_TO_INDEX(desc->getTextType()),
								desc, &to, INTL_KEY_UNIQUE);
						}

						asb->asb_desc.dsc_address = data +
							(asb->asb_intl ? asb->asb_key_desc[1].skd_offset : 0);
						MOV_move(tdbb, desc, &asb->asb_desc);

						break;
					}

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

		for (ptr = m_map->nod_arg, end = ptr + m_map->nod_count; ptr < end; ptr++)
		{
			const jrd_nod* from = (*ptr)->nod_arg[e_asgn_from];
			impure_value_ex* impure = (impure_value_ex*) ((SCHAR*) request + from->nod_impure);

			if (from->nod_type == nod_agg_list && impure->vlu_blob)
			{
				BLB_close(tdbb, impure->vlu_blob);
				impure->vlu_blob = NULL;
			}
		}

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
			case nod_agg_total_distinct:
			case nod_agg_total2:
			case nod_agg_total_distinct2:
			case nod_agg_list:
			case nod_agg_list_distinct:
				if (from->nod_type == nod_agg_total_distinct ||
					from->nod_type == nod_agg_total_distinct2 ||
					from->nod_type == nod_agg_list_distinct)
				{
					computeDistinct(tdbb, from);
				}

				if (!impure->vlux_count)
				{
					SET_NULL(record, id);
					break;
				}
				// If vlux_count is non-zero, we need to fall through.

			case nod_agg_count:
			case nod_agg_count2:
			case nod_agg_count_distinct:
				if (from->nod_type == nod_agg_count_distinct)
				{
					computeDistinct(tdbb, from);
				}

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

			case nod_agg_average_distinct:
				computeDistinct(tdbb, from);
				// fall through

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

			case nod_agg_average_distinct2:
				computeDistinct(tdbb, from);
				// fall through

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
	}
	catch (const Exception&)
	{
		finiDistinct(request);
		throw;
	}

	return state;
}


// Initialize a sort for distinct aggregate.
void AggregatedStream::initDistinct(jrd_req* request, const jrd_nod* node)
{
	DEV_BLKCHK(node, type_nod);

	Database* database = request->req_attachment->att_database;

	const size_t asbIndex = (node->nod_type == nod_agg_list_distinct) ? 2 : 1;
	const AggregateSort* asb = (AggregateSort*) node->nod_arg[asbIndex];
	impure_agg_sort* asbImpure = (impure_agg_sort*) ((char*) request + asb->nod_impure);
	const sort_key_def* sortKey = asb->asb_key_desc;

	// Get rid of the old sort areas if this request has been used already
	SORT_fini(asbImpure->iasb_sort_handle);

	asbImpure->iasb_sort_handle = SORT_init(database, &request->req_sorts,
		ROUNDUP_LONG(asb->asb_length), (asb->asb_intl ? 2 : 1), 1, sortKey, rejectDuplicate, 0);
}

// Sort/project the values and compute the aggregate
void AggregatedStream::computeDistinct(thread_db* tdbb, jrd_nod* node)
{
	jrd_req* const request = tdbb->getRequest();

	const size_t asbIndex = (node->nod_type == nod_agg_list_distinct) ? 2 : 1;
	AggregateSort* asb = (AggregateSort*) node->nod_arg[asbIndex];
	impure_agg_sort* asbImpure = (impure_agg_sort*) ((SCHAR*) request + asb->nod_impure);
	dsc* desc = &asb->asb_desc;
	impure_value_ex* impure = (impure_value_ex*) ((SCHAR*) request + node->nod_impure);

	// Sort the values already "put" to sort

	SORT_sort(tdbb, asbImpure->iasb_sort_handle);

	// Now get the sorted/projected values and compute the aggregate

	while (true)
	{
		UCHAR* data;
		SORT_get(tdbb, asbImpure->iasb_sort_handle, reinterpret_cast<ULONG**>(&data));

		if (data == NULL)
		{
			// we are done, close the sort
			SORT_fini(asbImpure->iasb_sort_handle);
			asbImpure->iasb_sort_handle = NULL;
			break;
		}

		desc->dsc_address = data + (asb->asb_intl ? asb->asb_key_desc[1].skd_offset : 0);

		switch (node->nod_type)
		{
		case nod_agg_total_distinct:
		case nod_agg_average_distinct:
			++impure->vlux_count;
			EVL_add(desc, node, impure);
			break;

		case nod_agg_total_distinct2:
		case nod_agg_average_distinct2:
			++impure->vlux_count;
			EVL_add2(desc, node, impure);
			break;

		case nod_agg_count_distinct:
			++impure->vlux_count;
			++impure->vlu_misc.vlu_long;
			break;

		case nod_agg_list_distinct:
			{
				if (!impure->vlu_blob)
				{
					impure->vlu_blob = BLB_create(tdbb, request->req_transaction,
						&impure->vlu_misc.vlu_bid);
					impure->vlu_desc.makeBlob(desc->getBlobSubType(), desc->getTextType(),
						(ISC_QUAD*) &impure->vlu_misc.vlu_bid);
				}

				MoveBuffer buffer;
				UCHAR* temp;
				int len;

				if (impure->vlux_count)
				{
					const dsc* const delimiter = EVL_expr(tdbb, node->nod_arg[1]);
					if (request->req_flags & req_null)
					{
						// mark the result as NULL
						impure->vlu_desc.dsc_dtype = 0;
						break;
					}

					len = MOV_make_string2(tdbb, delimiter, impure->vlu_desc.getTextType(), &temp,
						buffer, false);
					BLB_put_data(tdbb, impure->vlu_blob, temp, len);
				}

				++impure->vlux_count;
				len = MOV_make_string2(tdbb, desc, impure->vlu_desc.getTextType(), &temp, buffer, false);
				BLB_put_data(tdbb, impure->vlu_blob, temp, len);
				break;
			}

		default:	// Shut up some warnings
			break;
		}
	}

	if (node->nod_type == nod_agg_list_distinct && impure->vlu_blob)
	{
		BLB_close(tdbb, impure->vlu_blob);
		impure->vlu_blob = NULL;
	}
}

// Finalize a sort for distinct aggregate
void AggregatedStream::finiDistinct(jrd_req* request)
{
	jrd_nod** ptr;
	const jrd_nod* const* end;

	for (ptr = m_map->nod_arg, end = ptr + m_map->nod_count; ptr < end; ptr++)
	{
		const jrd_nod* const from = (*ptr)->nod_arg[e_asgn_from];

		switch (from->nod_type)
		{
			case nod_agg_count_distinct:
			case nod_agg_total_distinct:
			case nod_agg_average_distinct:
			case nod_agg_average_distinct2:
			case nod_agg_total_distinct2:
			case nod_agg_list_distinct:
			{
				const size_t asbIndex = (from->nod_type == nod_agg_list_distinct) ? 2 : 1;
				const AggregateSort* const asb = (AggregateSort*) from->nod_arg[asbIndex];
				impure_agg_sort* const asbImpure = (impure_agg_sort*) ((SCHAR*) request + asb->nod_impure);
				SORT_fini(asbImpure->iasb_sort_handle);
				asbImpure->iasb_sort_handle = NULL;
			}
		}
	}
}
