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
 *  Copyright (c) 2009 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../jrd/jrd.h"
#include "../jrd/ini.h"
#include "../jrd/rse.h"
#include "../jrd/exe_proto.h"
#include "../jrd/opt_proto.h"
#include "../jrd/rse_proto.h"
#include "../jrd/WindowRsb.h"

using namespace Jrd;
using namespace Firebird;


WindowRsb::WindowRsb(RecordSource* aRsb)
	: rsb(aRsb),
	  next(rsb->rsb_next)
{
}


RecordSource* WindowRsb::create(thread_db* tdbb, OptimizerBlk* opt, RecordSource* next)
{
	SET_TDBB(tdbb);

	CompilerScratch* csb = opt->opt_csb;

	RecordSource* rsb = FB_NEW_RPT(*tdbb->getDefaultPool(), 0) RecordSource;
	rsb->rsb_type = rsb_record_stream;
	rsb->rsb_next = next;
	rsb->rsb_stream = next->rsb_stream;
	rsb->rsb_record_stream = FB_NEW(*tdbb->getDefaultPool()) WindowRsb(rsb);

	return rsb;
}


unsigned WindowRsb::dump(UCHAR* buffer, unsigned bufferLen)
{
	UCHAR* bufferStart = buffer;

	if (bufferLen > 0)
		*buffer++ = isc_info_rsb_window;

	return buffer - bufferStart;
}


void WindowRsb::open(thread_db* tdbb)
{
	SET_TDBB(tdbb);

	RSE_open(tdbb, next);
	RSE_internal_get_record(tdbb, next, NULL, RSE_get_forward);
	RSE_close(tdbb, next);

	RSE_open(tdbb, next->rsb_next);
}


void WindowRsb::close(thread_db* tdbb)
{
	SET_TDBB(tdbb);

	RSE_close(tdbb, next->rsb_next);
}


bool WindowRsb::get(thread_db* tdbb)
{
	SET_TDBB(tdbb);

	if (!RSE_internal_get_record(tdbb, next->rsb_next, NULL, RSE_get_forward))
		return false;

	jrd_req* request = tdbb->getRequest();
	jrd_nod* node = (jrd_nod*) next->rsb_arg[0];
	jrd_nod* map = node->nod_arg[e_agg_map];

	jrd_nod* const* ptr = map->nod_arg;
	for (const jrd_nod* const* end = ptr + map->nod_count; ptr < end; ptr++)
	{
		jrd_nod* from = (*ptr)->nod_arg[e_asgn_from];
		impure_value_ex* impure = (impure_value_ex*) ((SCHAR*) request + from->nod_impure);

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


void WindowRsb::markRecursive()
{
	OPT_mark_rsb_recursive(rsb->rsb_next);
}
