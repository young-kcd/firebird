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
#include "../../common/common.h"
#include "../../common/classes/auto.h"
#include "../../common/utils_proto.h"
#include "../../jrd/trace/TraceManager.h"
#include "../../jrd/trace/TraceLog.h"
#include "../../jrd/trace/TraceObjects.h"
#include "../../common/isc_proto.h"
#include "../../common/isc_s_proto.h"
#include "../../jrd/jrd.h"
#include "../../jrd/tra.h"
#include "../../jrd/DataTypeUtil.h"
#include "../../dsql/ExprNodes.h"
#include "../../dsql/StmtNodes.h"
#include "../../jrd/evl_proto.h"
#include "../../jrd/intl_proto.h"
#include "../../jrd/mov_proto.h"
#include "../../jrd/opt_proto.h"
#include "../../jrd/pag_proto.h"
#include "../../common/os/path_utils.h"
#include "../../common/config/os/config_root.h"
#include "../../dsql/dsql_proto.h"
#include "../../common/prett_proto.h"

#ifdef WIN_NT
#include <process.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

using namespace Firebird;

namespace Jrd {

/// TraceConnectionImpl

int TraceConnectionImpl::getConnectionID()
{
	//return m_att->att_attachment_id;
	return PAG_attachment_id(JRD_get_thread_data());
}

int TraceConnectionImpl::getProcessID()
{
	return getpid();
}

const char* TraceConnectionImpl::getDatabaseName()
{
	return m_att->att_filename.c_str();
}

const char* TraceConnectionImpl::getUserName()
{
	const UserId* user = m_att->att_user;
	return user ? user->usr_user_name.c_str() : NULL;
}

const char* TraceConnectionImpl::getRoleName()
{
	const UserId* user = m_att->att_user;
	return user ? user->usr_sql_role_name.c_str() : NULL;
}

const char* TraceConnectionImpl::getCharSet()
{
	CharSet* cs = INTL_charset_lookup(JRD_get_thread_data(), m_att->att_charset);
	return cs ? cs->getName() : NULL;
}

const char* TraceConnectionImpl::getRemoteProtocol()
{
	return m_att->att_network_protocol.c_str();
}

const char* TraceConnectionImpl::getRemoteAddress()
{
	return m_att->att_remote_address.c_str();
}

int TraceConnectionImpl::getRemoteProcessID()
{
	return m_att->att_remote_pid;
}

const char* TraceConnectionImpl::getRemoteProcessName()
{
	return m_att->att_remote_process.c_str();
}


/// TraceTransactionImpl

int TraceTransactionImpl::getTransactionID()
{
	return m_tran->tra_number;
}

bool TraceTransactionImpl::getReadOnly()
{
	return (m_tran->tra_flags & TRA_readonly);
}

int TraceTransactionImpl::getWait()
{
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


/// BLRPrinter

const char* BLRPrinter::getText()
{
	if (m_text.empty() && getDataLength())
		fb_print_blr(getData(), (ULONG) getDataLength(), print_blr, this, 0);
	return m_text.c_str();
}

void BLRPrinter::print_blr(void* arg, SSHORT offset, const char* line)
{
	BLRPrinter* blr = (BLRPrinter*) arg;

	string temp;
	temp.printf("%4d %s\n", offset, line);
	blr->m_text.append(temp);
}


/// TraceSQLStatementImpl

int TraceSQLStatementImpl::getStmtID()
{
	if (m_stmt->req_request)
		return m_stmt->req_request->req_id;

	return 0;
}

const char* TraceSQLStatementImpl::getText()
{
	return m_stmt->getStatement()->getSqlText()->c_str();
}

// Returns false if conversion is not needed.
static bool convertToUTF8(const string& src, string& dst)
{
	thread_db* tdbb = JRD_get_thread_data();
	const CHARSET_ID charset = tdbb->getAttachment()->att_charset;

	if (charset == CS_UTF8 || charset == CS_UNICODE_FSS)
		return false;

	if (charset == CS_NONE)
	{
		const size_t length = src.length();

		const char* s = src.c_str();
		char* p = dst.getBuffer(length);

		for (const char* end = src.end(); s < end; ++p, ++s)
			*p = (*s < 0 ? '?' : *s);
	}
	else // charset != CS_UTF8
	{
		DataTypeUtil dtUtil(tdbb);
		ULONG length = dtUtil.convertLength(src.length(), charset, CS_UTF8);

		length = INTL_convert_bytes(tdbb,
			CS_UTF8, (UCHAR*) dst.getBuffer(length), length,
			charset, (const BYTE*) src.begin(), src.length(),
			ERR_post);

		dst.resize(length);
	}

	return true;
}

const char* TraceSQLStatementImpl::getTextUTF8()
{
	const string* stmtText = m_stmt->getStatement()->getSqlText();

	if (m_textUTF8.isEmpty() && !stmtText->isEmpty())
	{
		if (!convertToUTF8(*stmtText, m_textUTF8))
			return stmtText->c_str();
	}

	return m_textUTF8.c_str();
}

const char* TraceSQLStatementImpl::getPlan()
{
	if (m_plan.isEmpty())
	{
		m_plan = OPT_get_plan(JRD_get_thread_data(), m_stmt->req_request, false);
	}

	return m_plan.c_str();
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
	for (size_t i = 0 ; i < m_params->getCount(); ++i)
	{
		dsql_par* parameter = (*m_params)[i];

		if (parameter->par_index)
		{
			if (!first_index)
				first_index = parameter->par_index;

			// Use descriptor for nulls signaling
			USHORT null_flag = 0;
			if (parameter->par_null &&
				parameter->par_null->par_desc.dsc_address &&
				*((SSHORT*) parameter->par_null->par_desc.dsc_address))
			{
				null_flag = DSC_null;
			}

			if (first_index > parameter->par_index)
			{
				m_descs.insert(0, parameter->par_desc);
				m_descs.front().dsc_flags |= null_flag;
			}
			else
			{
				m_descs.add(parameter->par_desc);
				m_descs.back().dsc_flags |= null_flag;
			}
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


/// TraceFailedSQLStatement

const char* TraceFailedSQLStatement::getTextUTF8()
{
	if (m_textUTF8.isEmpty() && !m_text.isEmpty())
	{
		if (!convertToUTF8(m_text, m_textUTF8))
			return m_text.c_str();
	}

	return m_textUTF8.c_str();
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

	thread_db* tdbb = JRD_get_thread_data();

	const NestConst<ValueExprNode>* ptr = m_params->args.begin();
	const NestConst<ValueExprNode>* const end = m_params->args.end();

	for (; ptr != end; ++ptr)
	{
		const dsc* from_desc = NULL;
		dsc desc;

		const NestConst<ValueExprNode> prm = *ptr;
		const ParameterNode* param;
		const VariableNode* var;
		const LiteralNode* literal;

		if ((param = prm->as<ParameterNode>()))
		{
			//const impure_value* impure = m_request->getImpure<impure_value>(param->impureOffset)
			const MessageNode* message = param->message;
			const Format* format = message->format;
			const int arg_number = param->argNumber;

			desc = format->fmt_desc[arg_number];
			from_desc = &desc;
			desc.dsc_address = m_request->getImpure<UCHAR>(
				message->impureOffset + (IPTR) desc.dsc_address);

			// handle null flag if present
			if (param->argFlag)
			{
				const dsc* flag = EVL_expr(tdbb, m_request, param->argFlag);
				if (MOV_get_long(flag, 0))
					desc.dsc_flags |= DSC_null;
			}
		}
		else if ((var = prm->as<VariableNode>()))
		{
			impure_value* impure = m_request->getImpure<impure_value>(var->impureOffset);
			from_desc = &impure->vlu_desc;
		}
		else if ((literal = prm->as<LiteralNode>()))
			from_desc = &literal->litDesc;
		else if (prm->is<NullNode>())
		{
			desc.clear();
			desc.setNull();
			from_desc = &desc;
		}

		if (from_desc)
			m_descs.add(*from_desc);
	}
}


/// TraceTriggerImpl

const char* TraceTriggerImpl::getTriggerName()
{
	return m_trig->getStatement()->triggerName.c_str();
}

const char* TraceTriggerImpl::getRelationName()
{
	const jrd_rel* rel = m_trig->req_rpb[0].rpb_relation;
	return rel ? rel->rel_name.c_str() : NULL;
}


/// TraceLogWriterImpl

class TraceLogWriterImpl : public RefCntIface<TraceLogWriter, FB_TRACE_LOG_WRITER_VERSION>
{
public:
	TraceLogWriterImpl(const TraceSession& session) :
		m_log(getPool(), session.ses_logfile, false),
		m_sesId(session.ses_id)
	{
		m_maxSize = Config::getMaxUserTraceLogSize();
	}

	// TraceLogWriter implementation
	virtual size_t FB_CARG write(const void* buf, size_t size);

	virtual int FB_CARG release()
	{
		if (--refCounter == 0)
		{
			delete this;
			return 0;
		}
		return 1;
	}

private:
	TraceLog m_log;
	ULONG m_sesId;
	size_t m_maxSize;
};

size_t TraceLogWriterImpl::write(const void* buf, size_t size)
{
	// comparison is in MB
	if (m_log.getApproxLogSize() <= m_maxSize)
		return m_log.write(buf, size);

	ConfigStorage* storage = TraceManager::getStorage();
	StorageGuard guard(storage);

	TraceSession session(*getDefaultMemoryPool());
	storage->restart();
	while (storage->getNextSession(session))
	{
		if (session.ses_id == m_sesId)
		{
			if (!(session.ses_flags & trs_log_full))
			{
				// suspend session
				session.ses_flags |= trs_log_full;
				storage->updateSession(session);

				string s;
				s.printf("\n--- Session %d is suspended as its log is full ---\n", m_sesId);
				m_log.write(s.c_str(), s.length());
			}
			break;
		}
	}

	// report successful write
	return size;
}


/// TraceInitInfoImpl

const char* TraceInitInfoImpl::getFirebirdRootDirectory()
{
	return Config::getRootDirectory();
}

TraceLogWriter* TraceInitInfoImpl::getLogWriter()
{
	if (!m_logWriter && !m_session.ses_logfile.empty())
	{
		m_logWriter = new TraceLogWriterImpl(m_session);
	}
	m_logWriter->addRef();
	return m_logWriter;
}


/// TraceServiceImpl

ntrace_service_t TraceServiceImpl::getServiceID()
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

TraceRuntimeStats::TraceRuntimeStats(Attachment* att, RuntimeStatistics* baseline, RuntimeStatistics* stats,
	SINT64 clock, SINT64 records_fetched)
{
	m_info.pin_time = clock * 1000 / fb_utils::query_performance_frequency();
	m_info.pin_records_fetched = records_fetched;

	if (baseline)
		baseline->computeDifference(att, *stats, m_info, m_counts);
	else
	{
		// Report all zero counts for the moment.
		memset(&m_info, 0, sizeof(m_info));
		m_info.pin_counters = m_dummy_counts;
	}
}

SINT64 TraceRuntimeStats::m_dummy_counts[DBB_max_dbb_count] = {0};

} // namespace Jrd
