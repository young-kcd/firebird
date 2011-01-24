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

#ifndef JRD_TRACE_OBJECTS_H
#define JRD_TRACE_OBJECTS_H

#include <time.h>
#include "../../common/classes/array.h"
#include "../../common/classes/fb_string.h"
#include "../../dsql/dsql.h"
#include "../../jrd/ntrace.h"
#include "../../common/dsc.h"
#include "../../common/isc_s_proto.h"
#include "../../jrd/req.h"
#include "../../jrd/svc.h"
#include "../../jrd/tra.h"
#include "../../jrd/RuntimeStatistics.h"
#include "../../jrd/trace/TraceSession.h"
#include "../../common/classes/ImplementHelper.h"

//// TODO: DDL triggers, packages and external procedures and functions support
namespace Jrd {

class Database;
class Attachment;
class jrd_tra;

class TraceConnectionImpl : public Firebird::StackIface<TraceConnection, FB_TRACE_CONNECTION_VERSION>
{
public:
	TraceConnectionImpl(const Attachment* att) :
		m_att(att)
	{}

	virtual int FB_CARG getConnectionID();
	virtual int FB_CARG getProcessID();
	virtual const char* FB_CARG getDatabaseName();
	virtual const char* FB_CARG getUserName();
	virtual const char* FB_CARG getRoleName();
	virtual const char* FB_CARG getCharSet();
	virtual const char* FB_CARG getRemoteProtocol();
	virtual const char* FB_CARG getRemoteAddress();
	virtual int FB_CARG getRemoteProcessID();
	virtual const char* FB_CARG getRemoteProcessName();

private:
	const Attachment* const m_att;
};


class TraceTransactionImpl : public Firebird::StackIface<TraceTransaction, FB_TRACE_TRANSACTION_VERSION>
{
public:
	TraceTransactionImpl(const jrd_tra* tran, PerformanceInfo* perf = NULL) :
		m_tran(tran),
		m_perf(perf)
	{}

	virtual int FB_CARG getTransactionID();
	virtual bool FB_CARG getReadOnly();
	virtual int FB_CARG getWait();
	virtual ntrace_tra_isolation_t FB_CARG getIsolation();
	virtual PerformanceInfo* FB_CARG getPerf()	{ return m_perf; }

private:
	const jrd_tra* const m_tran;
	PerformanceInfo* const m_perf;
};


class BLRPrinter : public Firebird::StackIface<TraceBLRStatement, FB_TRACE_BLR_VERSION>
{
public:
	BLRPrinter(const unsigned char* blr, size_t length) :
		m_blr(blr),
		m_length(length),
		m_text(*getDefaultMemoryPool())
	{}

	virtual const unsigned char* FB_CARG getData()	{ return m_blr; }
	virtual size_t FB_CARG getDataLength()	{ return m_length; }
	virtual const char* FB_CARG getText();

private:
	static void print_blr(void* arg, SSHORT offset, const char* line);

	const unsigned char* const m_blr;
	const size_t m_length;
	Firebird::string m_text;
};


class TraceBLRStatementImpl : public BLRPrinter
{
public:
	TraceBLRStatementImpl(const jrd_req* stmt, PerformanceInfo* perf) :
		BLRPrinter(stmt->getStatement()->blr.begin(), stmt->getStatement()->blr.getCount()),
		m_stmt(stmt),
		m_perf(perf)
	{}

	virtual int FB_CARG getStmtID()				{ return m_stmt->req_id; }
	virtual PerformanceInfo* FB_CARG getPerf()	{ return m_perf; }

private:
	const jrd_req* const m_stmt;
	PerformanceInfo* const m_perf;
};


class TraceFailedBLRStatement : public BLRPrinter
{
public:
	TraceFailedBLRStatement(const unsigned char* blr, size_t length) :
		BLRPrinter(blr, length)
	{}

	virtual int FB_CARG getStmtID()				{ return 0; }
	virtual PerformanceInfo* FB_CARG getPerf()	{ return NULL; }
};


class TraceSQLStatementImpl : public Firebird::StackIface<TraceSQLStatement, FB_TRACE_SQL_VERSION>
{
public:
	TraceSQLStatementImpl(const dsql_req* stmt, PerformanceInfo* perf) :
		m_stmt(stmt),
		m_perf(perf),
		m_plan(NULL),
		m_inputs(*getDefaultMemoryPool(), m_stmt->getStatement()->getSendMsg() ?
			&m_stmt->getStatement()->getSendMsg()->msg_parameters : NULL)
	{}

	~TraceSQLStatementImpl();

	virtual int FB_CARG getStmtID();
	virtual PerformanceInfo* FB_CARG getPerf();
	virtual TraceParams* FB_CARG getInputs();
	virtual const char* FB_CARG getText();
	virtual const char* FB_CARG getPlan();
	virtual const char* FB_CARG getTextUTF8();

private:
	class DSQLParamsImpl : public Firebird::StackIface<TraceParams, FB_TRACE_PARAMS_VERSION>
	{
	public:
		DSQLParamsImpl(Firebird::MemoryPool &pool, const Firebird::Array<dsql_par*>* params) :
			m_params(params),
			m_descs(pool)
		{}

		virtual size_t FB_CARG getCount();
		virtual const dsc* FB_CARG getParam(size_t idx);

	private:
		void fillParams();

		const Firebird::Array<dsql_par*>* m_params;
		Firebird::HalfStaticArray<dsc, 16> m_descs;
	};

	const dsql_req* const m_stmt;
	PerformanceInfo* const m_perf;
	char* m_plan;
	DSQLParamsImpl m_inputs;
	Firebird::string m_textUTF8;
};


class TraceFailedSQLStatement : public Firebird::StackIface<TraceSQLStatement, FB_TRACE_SQL_VERSION>
{
public:
	TraceFailedSQLStatement(Firebird::string &text) :
		m_text(text)
	{}

	virtual int FB_CARG getStmtID()				{ return 0; }
	virtual PerformanceInfo* FB_CARG getPerf()	{ return NULL; }
	virtual TraceParams* FB_CARG getInputs()	{ return NULL; }
	virtual const char* FB_CARG getText()		{ return m_text.c_str(); }
	virtual const char* FB_CARG getPlan()		{ return ""; }
	virtual const char* FB_CARG getTextUTF8();

private:
	Firebird::string& m_text;
	Firebird::string m_textUTF8;
};


class TraceContextVarImpl : public Firebird::StackIface<TraceContextVariable, FB_TRACE_CONTEXT_VARIABLE_VERSION>
{
public:
	TraceContextVarImpl(const char* ns, const char* name, const char* value) :
		m_namespace(ns),
		m_name(name),
		m_value(value)
	{}

	virtual const char* FB_CARG getNameSpace()	{ return m_namespace; }
	virtual const char* FB_CARG getVarName()	{ return m_name; }
	virtual const char* FB_CARG getVarValue()	{ return m_value; }

private:
	const char* const m_namespace;
	const char* const m_name;
	const char* const m_value;
};

class TraceProcedureImpl : public Firebird::StackIface<TraceProcedure, FB_TRACE_PROCEDURE_VERSION>
{
public:
	TraceProcedureImpl(jrd_req* request, PerformanceInfo* perf) :
		m_request(request),
		m_perf(perf),
		m_inputs(*getDefaultMemoryPool(), request->req_proc_caller, request->req_proc_inputs)
	{}

	virtual const char* FB_CARG getProcName()
	{
		return m_request->getStatement()->procedure->getName().identifier.c_str();
	}

	virtual TraceParams* FB_CARG getInputs()	{ return &m_inputs; }
	virtual PerformanceInfo* FB_CARG getPerf()	{ return m_perf; };

private:
	class JrdParamsImpl : public Firebird::StackIface<TraceParams, FB_TRACE_PARAMS_VERSION>
	{
	public:
		JrdParamsImpl(Firebird::MemoryPool& pool, jrd_req* request, const ValueListNode* params) :
			m_request(request),
			m_params(params),
			m_descs(pool)
		{}

		virtual size_t FB_CARG getCount();
		virtual const dsc* FB_CARG getParam(size_t idx);

	private:
		void fillParams();

		jrd_req* m_request;
		const ValueListNode* m_params;
		Firebird::HalfStaticArray<dsc, 16> m_descs;
	};

	jrd_req* const m_request;
	PerformanceInfo* const m_perf;
	JrdParamsImpl m_inputs;
};


class TraceTriggerImpl : public Firebird::StackIface<TraceTrigger, FB_TRACE_TRIGGER_VERSION>
{
public:
	TraceTriggerImpl(const jrd_req* trig, SSHORT which, PerformanceInfo* perf) :
	  m_trig(trig),
	  m_which(which),
	  m_perf(perf)
	{}

	virtual const char* FB_CARG getTriggerName();
	virtual const char* FB_CARG getRelationName();
	virtual int FB_CARG getAction()				{ return m_trig->req_trigger_action; }
	virtual int FB_CARG getWhich()				{ return m_which; }
	virtual PerformanceInfo* FB_CARG getPerf()	{ return m_perf; }

private:
	const jrd_req* const m_trig;
	const SSHORT m_which;
	PerformanceInfo* const m_perf;
};


class TraceServiceImpl : public Firebird::StackIface<TraceService, FB_TRACE_SERVICE_VERSION>
{
public:
	TraceServiceImpl(const Service* svc) :
		m_svc(svc)
	{}

	virtual ntrace_service_t FB_CARG getServiceID();
	virtual const char* FB_CARG getServiceMgr();
	virtual const char* FB_CARG getServiceName();
	virtual const char* FB_CARG getUserName();
	virtual const char* FB_CARG getRoleName();
	virtual int FB_CARG getProcessID();
	virtual const char* FB_CARG getRemoteProtocol();
	virtual const char* FB_CARG getRemoteAddress();
	virtual int FB_CARG getRemoteProcessID();
	virtual const char* FB_CARG getRemoteProcessName();

private:
	const Service* const m_svc;
};


class TraceRuntimeStats
{
public:
	TraceRuntimeStats(Database* dbb, RuntimeStatistics* baseline, RuntimeStatistics* stats,
		SINT64 clock, SINT64 records_fetched);

	PerformanceInfo* FB_CARG getPerf()	{ return &m_info; }

private:
	PerformanceInfo m_info;
	TraceCountsArray m_counts;
	static SINT64 m_dummy_counts[DBB_max_dbb_count];	// Zero-initialized array with zero counts
};


class TraceInitInfoImpl : public Firebird::StackIface<TraceInitInfo, FB_TRACE_INIT_INFO_VERSION>
{
public:
	TraceInitInfoImpl(const Firebird::TraceSession &session, const Attachment* att,
					const char* filename) :
		m_session(session),
		m_trace_conn(att),
		m_filename(filename),
		m_attachment(att)
	{
		if (m_attachment && !m_attachment->att_filename.empty()) {
			m_filename = m_attachment->att_filename.c_str();
		}
		m_logWriter = NULL;
	}

	virtual const char* FB_CARG getConfigText()			{ return m_session.ses_config.c_str(); }
	virtual int FB_CARG getTraceSessionID()				{ return m_session.ses_id; }
	virtual const char* FB_CARG getTraceSessionName()	{ return m_session.ses_name.c_str(); }

	virtual const char* FB_CARG getFirebirdRootDirectory();
	virtual const char* FB_CARG getDatabaseName()		{ return m_filename; }

	virtual TraceConnection* FB_CARG getConnection()
	{
		if (m_attachment)
			return &m_trace_conn;

		return NULL;
	}

	virtual TraceLogWriter* FB_CARG getLogWriter();

private:
	const Firebird::TraceSession& m_session;
	TraceLogWriter* m_logWriter;
	TraceConnectionImpl m_trace_conn;
	const char* m_filename;
	const Attachment* const m_attachment;
};


} // namespace Jrd

#endif // JRD_TRACE_OBJECTS_H
