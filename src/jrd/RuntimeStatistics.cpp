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
 *  Copyright (c) 2006 Dmitry Yemanov <dimitr@users.sf.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../jrd/gdsassert.h"
#include "../jrd/jrd.h"
#include "../jrd/req.h"
#include "../jrd/tra.h"

#include "../jrd/RuntimeStatistics.h"

using namespace Firebird;
using namespace Jrd;

void RuntimeStatistics::bumpValue(thread_db* tdbb, stat_t index)
{
	fb_assert(tdbb);

	RuntimeStatistics* statistics = NULL;

	if (tdbb->tdbb_request) {
		statistics = &tdbb->tdbb_request->req_stats;
	}
	else if (tdbb->tdbb_transaction) {
		statistics = &tdbb->tdbb_transaction->tra_stats;
	}
	else if (tdbb->tdbb_attachment) {
		statistics = &tdbb->tdbb_attachment->att_stats;
	}
	else if (tdbb->tdbb_database) {
		statistics = &tdbb->tdbb_database->dbb_stats;
	}

	fb_assert(statistics);

	for (; statistics; statistics = statistics->parent) {
		statistics->values[index]++;
	}
}
