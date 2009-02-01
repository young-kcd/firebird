/*
 *	PROGRAM:	Firebird Trace Services
 *	MODULE:		TraceObjects.h
 *	DESCRIPTION:	Trace API manager support
 *
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
 *  The Original Code was created by Khorsun Vladyslav 
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2009 Khorsun Vladyslav <hvlad@users.sourceforge.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 */

#include "firebird.h" 

#include "../../jrd/common.h"
#include "../../common/classes/auto.h"
#include "../../common/utils_proto.h"
#include "../../jrd/trace/TraceManager.h"
#include "../../jrd/trace/TraceLog.h"
#include "../../jrd/trace/TraceObjects.h"
#include "../../jrd/plugin_manager.h"
#include "../../jrd/isc_proto.h"
#include "../../jrd/isc_s_proto.h"
#include "../../jrd/jrd.h"
#include "../../jrd/jrd_pwd.h"
#include "../../jrd/tra.h"
#include "../../jrd/evl_proto.h"
#include "../../jrd/mov_proto.h"
#include "../../jrd/pag_proto.h"
#include "../../jrd/os/path_utils.h"
#include "../../jrd/os/config_root.h"
#include "../../dsql/dsql_proto.h"
#include "../../gpre/prett_proto.h"

#ifdef WIN_NT
#include <process.h> 
#endif

using namespace Firebird;

namespace Jrd {

/// TraceConnectionImpl 

int TraceConnectionImpl::getConnectionID() {
	//return m_att->att_attachment_id;
	return PAG_attachment_id(JRD_get_thread_data());
}

int TraceConnectionImpl::getProcessID() {
	return getpid();
}

const char* TraceConnectionImpl::getDatabaseName() {
	return m_att->att_filename.c_str();
}

const char* TraceConnectionImpl::getUserName() {
	const UserId *user = m_att->att_user;
	return user ? user->usr_user_name.c_str() : NULL;
}

const char* TraceConnectionImpl::getRoleName() {
	const UserId *user = m_att->att_user;
	return user ? user->usr_sql_role_name.c_str() : NULL;
}

const char* TraceConnectionImpl::getRemoteProtocol() {
	return m_att->att_network_protocol.c_str();
}

const char* TraceConnectionImpl::getRemoteAddress() {
	return m_att->att_remote_address.c_str();
}

int TraceConnectionImpl::getRemoteProcessID() {
	return m_att->att_remote_pid;
}

const char* TraceConnectionImpl::getRemoteProcessName() {
	return m_att->att_remote_process.c_str();
}


/// TraceTransactionImpl

int TraceTransactionImpl::getTransactionID() {
	return m_tran->tra_number;
}

bool TraceTransactionImpl::getReadOnly() {
	return (m_tran->tra_flags & TRA_readonly);
}

int TraceTransactionImpl::getWait(){
	return -m_tran->getLockWait();
}

ntrace_tra_isolation_t TraceTransactionImpl::getIsolation() 
{
	switch (m_tran->tra_flags & (TRA_read_committed | TRA_rec_version | TRA_degree3))
	{
	case TRA_degree3:
		return tra_iso_consistency;

	case TRA_read_committed:
		return tra_iso_read_committed_norecver;

	case TRA_read_committed | TRA_rec_version:
		return tra_iso_read_committed_recver;

	case 0:
		return tra_iso_concurrency;

	default:
		fb_assert(false);
		return tra_iso_concurrency;
	}
}


/// TraceDYNRequestImpl

const char* TraceDYNRequestImpl::getText()
{
	if (m_text.empty() && m_length) {
		PRETTY_print_dyn((UCHAR*) m_ddl, print_dyn, this, 0);
	}
	return m_text.c_str();
}

void TraceDYNRequestImpl::print_dyn(void* arg, SSHORT offset, const char* line)
{
	TraceDYNRequestImpl *dyn = (TraceDYNRequestImpl*) arg;

	string temp;
	temp.printf("%4d %s\n", offset, line);
	dyn->m_text.append(temp);
}


/// BLRPrinter

const char* BLRPrinter::getText()
{
	if (m_text.empty() && getDataLength()) {
		gds__print_blr((UCHAR*) getData(), print_blr, this, 0);
	}
	return m_text.c_str();
}

void BLRPrinter::print_blr(void* arg, SSHORT offset, const char* line)
{
	BLRPrinter *blr = (BLRPrinter*) arg;

	string temp;
	temp.printf("%4d %s\n", offset, line);
	blr->m_text.append(temp);
}


/// TraceSQLStatementImpl 

TraceSQLStatementImpl::~TraceSQLStatementImpl()
{
	if (m_plan)
		gds__free(m_plan);
}

int TraceSQLStatementImpl::getStmtID()
{
	if (m_stmt->req_request)
		return m_stmt->req_request->req_id;
	else
		return 0;
}

const char* TraceSQLStatementImpl::getText()
{
	return m_stmt->req_sql_text->c_str();
}

const char* TraceSQLStatementImpl::getPlan() 
{
	if (!m_plan && m_stmt->req_request)
	{
		char buff;
		m_plan = &buff;

		const int len = DSQL_get_plan_info(JRD_get_thread_data(),
			const_cast<dsql_req*> (m_stmt), sizeof(buff), &m_plan);
		
		if (len)
			m_plan[len] = 0;
		else
			m_plan = NULL;
	}

	return m_plan;
}

PerformanceInfo* TraceSQLStatementImpl::getPerf()
{
	return m_perf;
}

TraceParams* TraceSQLStatementImpl::getInputs()
{
	return &m_inputs;
}


/// TraceSQLStatementImpl::DSQLParamsImpl 

void TraceSQLStatementImpl::DSQLParamsImpl::fillParams()
{
	if (m_descs.getCount() || !m_params)
		return;

	USHORT first_index = 0;
	for (dsql_par* parameter = m_params; parameter; parameter = parameter->par_next)
	{
		if (parameter->par_index) 
		{
			if (!first_index)
				first_index = parameter->par_index;

			// Use descriptor for nulls signaling
			if (parameter->par_null && 
				*((SSHORT *) parameter->par_null->par_desc.dsc_address))
			{
				parameter->par_desc.dsc_flags |= DSC_null;
			}

			if (first_index > parameter->par_index)
				m_descs.insert(0, parameter->par_desc);
			else
				m_descs.add(parameter->par_desc);
		}
	}
}


size_t TraceSQLStatementImpl::DSQLParamsImpl::getCount()
{
	fillParams();
	return m_descs.getCount();
}

const dsc* TraceSQLStatementImpl::DSQLParamsImpl::getParam(size_t idx)
{
	fillParams();

	if (idx >= 0 && idx < m_descs.getCount())
		return &m_descs[idx];

	return NULL;
}



/// TraceProcedureImpl::JrdParamsImpl

size_t TraceProcedureImpl::JrdParamsImpl::getCount()
{
	fillParams();
	return m_descs.getCount();
}

const dsc* TraceProcedureImpl::JrdParamsImpl::getParam(size_t idx)
{
	fillParams();

	if (idx >= 0 && idx < m_descs.getCount())
		return &m_descs[idx];

	return NULL;
}

void TraceProcedureImpl::JrdParamsImpl::fillParams()
{
	if (m_descs.getCount() || !m_params)
		return;

	thread_db *tdbb = JRD_get_thread_data();

	const jrd_nod* const* ptr = m_params->nod_arg;
	const jrd_nod* const* end = ptr + m_params->nod_count;
	for (; ptr < end; ptr++)
	{
		dsc* from_desc = NULL;
		dsc desc;

		jrd_nod* const prm = (*ptr)->nod_arg[e_asgn_to];
		switch (prm->nod_type) 
		{
			case nod_argument: 
			{
				impure_value* impure = (impure_value*) ((SCHAR *) m_request + prm->nod_impure);
				jrd_nod* message = prm->nod_arg[e_arg_message];
				Format* format = (Format*) message->nod_arg[e_msg_format];
				const int arg_number = (int) (IPTR) prm->nod_arg[e_arg_number];
				
				desc = format->fmt_desc[arg_number];
				from_desc = &desc;
				from_desc->dsc_address = (UCHAR *) m_request + message->nod_impure + (IPTR) desc.dsc_address;

				// handle null flag if present
				if (prm->nod_arg[e_arg_flag])
				{
					dsc* flag = EVL_expr(tdbb, prm->nod_arg[e_arg_flag]);
					if (MOV_get_long(flag, 0)) {
						from_desc->dsc_flags |= DSC_null;
					}
				}
				break;
			}

			case nod_variable: 
			{
				impure_value* impure = (impure_value*) ((SCHAR *) m_request + prm->nod_impure);
				from_desc = &impure->vlu_desc;
				break;
			}

			case nod_null:
				from_desc = &((Literal*) prm)->lit_desc;
				from_desc->dsc_flags |= DSC_null;
				break;

			case nod_literal:
				from_desc = &((Literal*) prm)->lit_desc;
				break;

			default:
				break;
		}

		if (from_desc) 
			m_descs.add(*from_desc);
	}
}


/// TraceTriggerImpl 

const char* TraceTriggerImpl::getTriggerName()
{
	return m_trig->req_trg_name.c_str();
}

const char* TraceTriggerImpl::getRelationName()
{
	const jrd_rel *rel = m_trig->req_rpb->rpb_relation;
	return rel ? rel->rel_name.c_str() : NULL;
}


/// TraceInitInfoImpl

TraceLogWriter* TraceInitInfoImpl::getLogWriter()
{
	if (!m_logWriter && !m_session.ses_logfile.empty()) 
	{
		MemoryPool &pool = *getDefaultMemoryPool();
		m_logWriter = FB_NEW(pool) TraceLogImpl(pool, m_session.ses_logfile, false);
	}
	return m_logWriter;
}


/// TraceServiceImpl 

const ntrace_service_t TraceServiceImpl::getServiceID() 
{
	return (ntrace_service_t) m_svc;
}

const char* TraceServiceImpl::getServiceMgr()
{
	return m_svc->getServiceMgr();
}

const char* TraceServiceImpl::getServiceName()
{
	return m_svc->getServiceName();
}

int TraceServiceImpl::getProcessID()
{
	return getpid();
}

const char* TraceServiceImpl::getUserName()
{
	return m_svc->getUserName().c_str();
}

const char* TraceServiceImpl::getRoleName()
{
	return NULL;
}

const char* TraceServiceImpl::getRemoteProtocol()
{
	return m_svc->getNetworkProtocol().c_str();
}

const char* TraceServiceImpl::getRemoteAddress()
{
	return m_svc->getRemoteAddress().c_str();
}

int TraceServiceImpl::getRemoteProcessID()
{
	return m_svc->getRemotePID();
}

const char* TraceServiceImpl::getRemoteProcessName()
{
	return m_svc->getRemoteProcess().c_str();
}


/// TraceRuntimeStats

TraceRuntimeStats::TraceRuntimeStats(Database *dbb, RuntimeStatistics* baseline, RuntimeStatistics* stats, 
	SINT64 clock, SINT64 records_fetched)
{
	m_info.pin_time = clock * 1000 / fb_utils::query_performance_frequency();
	m_info.pin_records_fetched = records_fetched;

	if (baseline) 
	{
		baseline->computeDifference(dbb, *stats, m_info, m_counts);
	} 
	else 
	{
		// Report all zero counts for the moment. 
		memset(&m_info, 0, sizeof(m_info));
		m_info.pin_counters = m_dummy_counts;
	}
}

SINT64 TraceRuntimeStats::m_dummy_counts[DBB_max_dbb_count] = {0};

} // namespace Jrd 
