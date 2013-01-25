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
 * Adriano dos Santos Fernandes - refactored from others modules.
 */

#include "firebird.h"
#include "consts_pub.h"
#include "dyn_consts.h"
#include "gen/iberror.h"
#include "../jrd/jrd.h"
#include "../jrd/exe.h"
#include "../dsql/BlrWriter.h"
#include "../dsql/StmtNodes.h"
#include "../dsql/dsql.h"
#include "../jrd/blr.h"
#include "../dsql/errd_proto.h"

using namespace Firebird;

namespace Jrd {


// Write out a string valued attribute.
void BlrWriter::appendString(UCHAR verb, const char* string, USHORT length)
{
	// TMN: Doesn't this look pretty awkward? If we are given
	// a verb, the length is a ushort, else it's uchar.
	if (verb)
	{
		appendUChar(verb);
		appendUShort(length);
	}
	else
	{
		fb_assert(length <= MAX_UCHAR);
		appendUChar(length);
	}

	if (string)
		appendBytes(reinterpret_cast<const UCHAR*>(string), length);
}

void BlrWriter::beginDebug()
{
	fb_assert(debugData.isEmpty());

	// Version 2 replaces 16-bit values inside the
	// fb_dbg_map_src2blr tag with 32-bit ones.
	// Also, it introduces some new tags.
	const UCHAR CURRENT_DEBUG_INFO_VERSION = 2;

	debugData.add(fb_dbg_version);
	debugData.add(CURRENT_DEBUG_INFO_VERSION);
}

void BlrWriter::endDebug()
{
	debugData.add(fb_dbg_end);
}

void BlrWriter::putDebugSrcInfo(ULONG line, ULONG col)
{
	debugData.add(fb_dbg_map_src2blr);

	debugData.add(line);
	debugData.add(line >> 8);
	debugData.add(line >> 16);
	debugData.add(line >> 24);

	debugData.add(col);
	debugData.add(col >> 8);
	debugData.add(col >> 16);
	debugData.add(col >> 24);

	const ULONG offset = (blrData.getCount() - baseOffset);
	debugData.add(offset);
	debugData.add(offset >> 8);
	debugData.add(offset >> 16);
	debugData.add(offset >> 24);
}

void BlrWriter::putDebugVariable(USHORT number, const MetaName& name)
{
	debugData.add(fb_dbg_map_varname);

	debugData.add(number);
	debugData.add(number >> 8);

	USHORT len = MIN(name.length(), MAX_UCHAR);
	debugData.add(len);

	debugData.add(reinterpret_cast<const UCHAR*>(name.c_str()), len);
}

void BlrWriter::putDebugArgument(UCHAR type, USHORT number, const TEXT* name)
{
	fb_assert(name);

	debugData.add(fb_dbg_map_argument);

	debugData.add(type);
	debugData.add(number);
	debugData.add(number >> 8);

	USHORT len = strlen(name);
	if (len > MAX_UCHAR)
		len = MAX_UCHAR;
	debugData.add(len);

	debugData.add(reinterpret_cast<const UCHAR*>(name), len);
}

void BlrWriter::putDebugSubFunction(DeclareSubFuncNode* subFuncNode)
{
	debugData.add(fb_dbg_subfunc);

	dsql_udf* subFunc = subFuncNode->dsqlFunction;
	const MetaName& name = subFunc->udf_name.identifier;
	USHORT len = MIN(name.length(), MAX_UCHAR);

	debugData.add(len);
	debugData.add(reinterpret_cast<const UCHAR*>(name.c_str()), len);

	HalfStaticArray<UCHAR, 128>& subDebugData = subFuncNode->blockScratch->debugData;
	const ULONG count = ULONG(subDebugData.getCount());
	debugData.add(UCHAR(count));
	debugData.add(UCHAR(count >> 8));
	debugData.add(UCHAR(count >> 16));
	debugData.add(UCHAR(count >> 24));
	debugData.add(subDebugData.begin(), count);
}

void BlrWriter::putDebugSubProcedure(DeclareSubProcNode* subProcNode)
{
	debugData.add(fb_dbg_subproc);

	dsql_prc* subProc = subProcNode->dsqlProcedure;
	const MetaName& name = subProc->prc_name.identifier;
	USHORT len = MIN(name.length(), MAX_UCHAR);

	debugData.add(len);
	debugData.add(reinterpret_cast<const UCHAR*>(name.c_str()), len);

	HalfStaticArray<UCHAR, 128>& subDebugData = subProcNode->blockScratch->debugData;
	const ULONG count = ULONG(subDebugData.getCount());
	debugData.add(UCHAR(count));
	debugData.add(UCHAR(count >> 8));
	debugData.add(UCHAR(count >> 16));
	debugData.add(UCHAR(count >> 24));
	debugData.add(subDebugData.begin(), count);
}

}	// namespace Jrd
