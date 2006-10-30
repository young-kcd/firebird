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
 *  The Original Code was created by Vlad Horsun
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2006 Vlad Horsun <hvlad@users.sourceforge.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "../jrd/DebugInterface.h"
#include "../jrd/blb_proto.h"

using namespace Jrd;
using namespace Firebird;

void DBG_parse_debug_info(thread_db* tdbb, bid *blob_id, MapBlrToSrc& Map)
{
	Database* dbb = tdbb->tdbb_database;
	blb* blob = BLB_open(tdbb, dbb->dbb_sys_trans, blob_id);
	const SLONG length = blob->blb_length;
	Firebird::HalfStaticArray<UCHAR, 128> tmp;

	UCHAR* temp = tmp.getBuffer(length);
	BLB_get_data(tdbb, blob, temp, length);

	const UCHAR* end = temp + length;
	bool bad_format = false;

	if ((*temp++ != fb_dbg_version) ||
		(end[-1] != fb_dbg_end)		||
		(*temp++ != 1))
	{
		bad_format = true;
	}

	while (!bad_format && (temp < end)) 
	{
		switch (*temp++)
		{
		case fb_dbg_map_src2blr:
		{
			if (temp + 6 > end) {
				bad_format = true;
				break;
			}

			MapBlrToSrcItem i;
			i.mbs_src_line = *temp++;
			i.mbs_src_line |= *temp++ << 8;

			i.mbs_src_col = *temp++;
			i.mbs_src_col |= *temp++ << 8;

			i.mbs_offset = *temp++;
			i.mbs_offset |= *temp++ << 8;

			Map.add(i);
		}
		break;

		case fb_dbg_map_varname:
		{
			if (temp + 3 > end) {
				bad_format = true;
				break;
			}

			USHORT val;
			// variable number
			val = *temp++;
			val |= *temp++;

			// variable name string length
			val = *temp++;

			if (temp + val > end) {
				bad_format = true;
				break;
			}
			// variable name string
			temp += val;
		}
		break;

		case fb_dbg_end:
			if (temp != end)
				bad_format = true;
		break;

		default:
			bad_format = true;
		}
	}

	if (!bad_format && (temp != end))
		bad_format = true;

	if (bad_format) 
	{
		Map.clear();
		ERR_post_warning(isc_random, isc_arg_string, "Bad debug info format", 0);
	}
}
