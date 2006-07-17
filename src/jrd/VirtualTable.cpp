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
#include "../jrd/dsc.h"
#include "../jrd/exe.h"
#include "../jrd/req.h"
#include "../jrd/rse.h"
#include "../jrd/val.h"
#include "../jrd/err_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/vio_proto.h"

#include "../jrd/DatabaseSnapshot.h"
#include "../jrd/RecordBuffer.h"
#include "../jrd/VirtualTable.h"

using namespace Jrd;

void VirtualTable::close(thread_db* tdbb, RecordSource* rsb)
{
	SET_TDBB(tdbb);

	irsb_virtual* impure =
		(irsb_virtual*) ((UCHAR *) tdbb->tdbb_request + rsb->rsb_impure);

	impure->irsb_record_buffer = NULL;
}

void VirtualTable::erase(record_param* rpb)
{
	ERR_post(isc_read_only, 0);
}

bool VirtualTable::get(thread_db* tdbb, RecordSource* rsb)
{
	SET_TDBB(tdbb);

	jrd_req* request = tdbb->tdbb_request;

	record_param* const rpb = &request->req_rpb[rsb->rsb_stream];
	irsb_virtual* const impure =
		(irsb_virtual*) ((UCHAR *) request + rsb->rsb_impure);

	if (!impure->irsb_record_buffer)
		return false;

	return impure->irsb_record_buffer->fetch(rpb->rpb_record);
}

void VirtualTable::modify(record_param* org_rpb, record_param* new_rpb)
{
	ERR_post(isc_read_only, 0);
}

void VirtualTable::open(thread_db* tdbb, RecordSource* rsb)
{
	SET_TDBB(tdbb);

	jrd_req* request = tdbb->tdbb_request;

	jrd_rel* const relation = rsb->rsb_relation;
	record_param* const rpb = &request->req_rpb[rsb->rsb_stream];
	irsb_virtual* const impure =
		(irsb_virtual*) ((UCHAR *) request + rsb->rsb_impure);

	const Record* const record = rpb->rpb_record;
	const Format* format = NULL;
	if (!record || !record->rec_format) {
		format = MET_current(tdbb, relation);
		VIO_record(tdbb, rpb, format, request->req_pool);
	}
	else {
		format = record->rec_format;
	}

	DatabaseSnapshot* snapshot = DatabaseSnapshot::create(tdbb);
	impure->irsb_record_buffer = snapshot->getData(relation);
}

Jrd::RecordSource* VirtualTable::optimize(thread_db* tdbb, OptimizerBlk* opt, SSHORT stream)
{
	SET_TDBB(tdbb);

	CompilerScratch* csb = opt->opt_csb;
	CompilerScratch::csb_repeat* csb_tail = &csb->csb_rpt[stream];

	RecordSource* rsb = FB_NEW_RPT(*tdbb->getDefaultPool(), 0) RecordSource;
	rsb->rsb_type = rsb_virt_sequential;
	rsb->rsb_stream = stream;
	rsb->rsb_relation = csb_tail->csb_relation;
	rsb->rsb_impure = csb->csb_impure;
	csb->csb_impure += sizeof(irsb);

	return rsb;
}

void VirtualTable::store(record_param* rpb)
{
	ERR_post(isc_read_only, 0);
}
