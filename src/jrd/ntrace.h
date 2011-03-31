/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		ntrace.h
 *	DESCRIPTION:	Trace API header
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
 *  The Original Code was created by Nickolay Samofatov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Nickolay Samofatov <nickolay@broadviewsoftware.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *  2008 Khorsun Vladyslav
 *
 */

#ifndef FIREBIRD_NTRACE_H
#define FIREBIRD_NTRACE_H

#include "FirebirdPluginApi.h"
#include "../common/common.h"		// to define SINT64

// Database objects

struct PerformanceInfo;

class TraceConnection : public Firebird::IDisposable
{
public:
	virtual int FB_CARG getConnectionID() = 0;
	virtual const char* FB_CARG getDatabaseName() = 0;

	virtual int FB_CARG getProcessID() = 0;
	virtual const char* FB_CARG getUserName() = 0;
	virtual const char* FB_CARG getRoleName() = 0;
	virtual const char* FB_CARG getCharSet() = 0;
	virtual const char* FB_CARG getRemoteProtocol() = 0;
	virtual const char* FB_CARG getRemoteAddress() = 0;
	virtual int FB_CARG getRemoteProcessID() = 0;
	virtual const char* FB_CARG getRemoteProcessName() = 0;
};

enum ntrace_tra_isolation_t
{
	tra_iso_consistency = 1,
	tra_iso_concurrency,
	tra_iso_read_committed_recver,
	tra_iso_read_committed_norecver
};

class TraceTransaction : public Firebird::IDisposable
{
public:
	virtual int FB_CARG getTransactionID() = 0;
	virtual bool FB_CARG getReadOnly() = 0;
	virtual int FB_CARG getWait() = 0;
	virtual ntrace_tra_isolation_t FB_CARG getIsolation() = 0;
	virtual PerformanceInfo* FB_CARG getPerf() = 0;
};

typedef int ntrace_relation_t;

class TraceParams : public Firebird::IDisposable
{
public:
	virtual size_t FB_CARG getCount() = 0;
	virtual const struct dsc* FB_CARG getParam(size_t idx) = 0;
};

class TraceStatement : public Firebird::IDisposable
{
public:
	virtual int FB_CARG getStmtID() = 0;
	virtual PerformanceInfo* FB_CARG getPerf() = 0;
};

class TraceSQLStatement : public TraceStatement
{
public:
	virtual const char* FB_CARG getText() = 0;
	virtual const char* FB_CARG getPlan() = 0;
	virtual TraceParams* FB_CARG getInputs() = 0;
	virtual const char* FB_CARG getTextUTF8() = 0;
};

class TraceBLRStatement : public TraceStatement
{
public:
	virtual const unsigned char* FB_CARG getData() = 0;
	virtual size_t FB_CARG getDataLength() = 0;
	virtual const char* FB_CARG getText() = 0;
};

class TraceDYNRequest : public Firebird::IDisposable
{
public:
	virtual const unsigned char* FB_CARG getData() = 0;
	virtual size_t FB_CARG getDataLength() = 0;
	virtual const char* FB_CARG getText() = 0;
};

class TraceContextVariable : public Firebird::IDisposable
{
public:
	virtual const char* FB_CARG getNameSpace() = 0;
	virtual const char* FB_CARG getVarName() = 0;
	virtual const char* FB_CARG getVarValue() = 0;
};

class TraceProcedure : public Firebird::IDisposable
{
public:
	virtual const char* FB_CARG getProcName() = 0;
	virtual TraceParams* FB_CARG getInputs() = 0;
	virtual PerformanceInfo* FB_CARG getPerf() = 0;
};

class TraceTrigger : public Firebird::IDisposable
{
public:
	virtual const char* FB_CARG getTriggerName() = 0;
	virtual const char* FB_CARG getRelationName() = 0;
	virtual int FB_CARG getAction() = 0;
	virtual int FB_CARG getWhich() = 0;
	virtual PerformanceInfo* FB_CARG getPerf() = 0;
};

typedef void* ntrace_service_t;

class TraceService : public Firebird::IDisposable
{
public:
	virtual ntrace_service_t FB_CARG getServiceID() = 0;
	virtual const char* FB_CARG getServiceMgr() = 0;
	virtual const char* FB_CARG getServiceName() = 0;

	virtual int FB_CARG getProcessID() = 0;
	virtual const char* FB_CARG getUserName() = 0;
	virtual const char* FB_CARG getRoleName() = 0;
	virtual const char* FB_CARG getRemoteProtocol() = 0;
	virtual const char* FB_CARG getRemoteAddress() = 0;
	virtual int FB_CARG getRemoteProcessID() = 0;
	virtual const char* FB_CARG getRemoteProcessName() = 0;
};


// Plugin-specific argument. Passed by the engine to each hook
typedef void* ntrace_object_t;

// Structure version
typedef int ntrace_version_t;

// Boolean type
typedef int ntrace_boolean_t;

// Performance counter
typedef SINT64 ntrace_counter_t;
typedef FB_UINT64 ntrace_mask_t;

// Used for arrays with binary data
typedef unsigned char ntrace_byte_t;


// Event completion: 0 - successful, 1 - unsuccessful, 2 - unauthorized access
enum ntrace_result_t
{
	res_successful		= 0,
	res_failed			= 1,
	res_unauthorized	= 2
};

enum ntrace_trigger_type_t
{
	trg_all				= 0,
	trg_before			= 1,
	trg_after			= 2
};

const int DBB_max_rel_count = 8; // must be the same as DBB_max_count from jrd.h

// Performance counters for entire database

enum {
	DBB_fetches_count = 0,
	DBB_reads_count,
	DBB_marks_count,
	DBB_writes_count,
	DBB_max_dbb_count
};

// Performance counters for individual table
struct TraceCounts
{
	ntrace_relation_t	trc_relation_id;	// Relation ID
	const char*			trc_relation_name;	// Relation name
	const ntrace_counter_t*	trc_counters;	// Pointer to allow easy addition of new counters
};

// Performance statistics for operation
struct PerformanceInfo
{
	ntrace_counter_t pin_time;		// Total operation time in milliseconds
	ntrace_counter_t* pin_counters;	// Pointer to allow easy addition of new counters

	size_t pin_count;				// Number of relations involved in analysis
	struct TraceCounts* pin_tables; // Pointer to array with table stats

	ntrace_counter_t pin_records_fetched;	// records fetched from statement/procedure
};

class TraceLogWriter : public Firebird::Interface
{
public:
	virtual size_t FB_CARG write(const void* buf, size_t size) = 0;
};
#define FB_TRACE_LOG_WRITER_VERSION (FB_INTERFACE_VERSION + 1)

class TraceInitInfo : public Firebird::IDisposable
{
public:
	virtual const char* FB_CARG getConfigText() = 0;
	virtual int FB_CARG getTraceSessionID() = 0;
	virtual const char* FB_CARG getTraceSessionName() = 0;
	virtual const char* FB_CARG getFirebirdRootDirectory() = 0;
	virtual const char* FB_CARG getDatabaseName() = 0;
	virtual TraceConnection* FB_CARG getConnection() = 0;
	virtual TraceLogWriter* FB_CARG getLogWriter() = 0;
};


// API of trace plugin. Used to deliver notifications for each database
class TracePlugin : public Firebird::Interface
{
public:
	// Function to return error string for hook failure
	virtual const char* FB_CARG trace_get_error() = 0;

	// Events supported:

	// Create/close attachment
	virtual int FB_CARG trace_attach(TraceConnection* connection, ntrace_boolean_t create_db, ntrace_result_t att_result) = 0;
	virtual int FB_CARG trace_detach(TraceConnection* connection, ntrace_boolean_t drop_db) = 0;

	// Start/end transaction
	virtual int FB_CARG trace_transaction_start(TraceConnection* connection, TraceTransaction* transaction,
			size_t tpb_length, const ntrace_byte_t* tpb, ntrace_result_t tra_result) = 0;
	virtual int FB_CARG trace_transaction_end(TraceConnection* connection, TraceTransaction* transaction,
			ntrace_boolean_t commit, ntrace_boolean_t retain_context, ntrace_result_t tra_result) = 0;

	// Stored procedure and triggers executing
	virtual int FB_CARG trace_proc_execute (TraceConnection* connection, TraceTransaction* transaction, TraceProcedure* procedure,
			bool started, ntrace_result_t proc_result) = 0;
	virtual int FB_CARG trace_trigger_execute(TraceConnection* connection, TraceTransaction* transaction, TraceTrigger* trigger,
			bool started, ntrace_result_t trig_result) = 0;

	// Assignment to context variables
	virtual int FB_CARG trace_set_context(TraceConnection* connection, TraceTransaction* transaction, TraceContextVariable* variable) = 0;

	// DSQL statement lifecycle
	virtual int FB_CARG trace_dsql_prepare(TraceConnection* connection, TraceTransaction* transaction,
			TraceSQLStatement* statement, ntrace_counter_t time_millis, ntrace_result_t req_result) = 0;
	virtual int FB_CARG trace_dsql_free(TraceConnection* connection, TraceSQLStatement* statement, unsigned short option) = 0;
	virtual int FB_CARG trace_dsql_execute(TraceConnection* connection, TraceTransaction* transaction, TraceSQLStatement* statement,
			bool started, ntrace_result_t req_result) = 0;

	// BLR requests
	virtual int FB_CARG trace_blr_compile(TraceConnection* connection, TraceTransaction* transaction,
			TraceBLRStatement* statement, ntrace_counter_t time_millis, ntrace_result_t req_result) = 0;
	virtual int FB_CARG trace_blr_execute(TraceConnection* connection, TraceTransaction* transaction,
			TraceBLRStatement* statement, ntrace_result_t req_result) = 0;

	// DYN requests
	virtual int FB_CARG trace_dyn_execute(TraceConnection* connection, TraceTransaction* transaction,
			TraceDYNRequest* request, ntrace_counter_t time_millis, ntrace_result_t req_result) = 0;

	// Using the services
	virtual int FB_CARG trace_service_attach(TraceService* service, ntrace_result_t att_result) = 0;
	virtual int FB_CARG trace_service_start(TraceService* service, size_t switches_length, const char* switches,
			ntrace_result_t start_result) = 0;
	virtual int FB_CARG trace_service_query(TraceService* service, size_t send_item_length,
			const ntrace_byte_t* send_items, size_t recv_item_length,
			const ntrace_byte_t* recv_items, ntrace_result_t query_result) = 0;
	virtual int FB_CARG trace_service_detach(TraceService* service, ntrace_result_t detach_result) = 0;
};
#define FB_TRACE_PLUGIN_VERSION (FB_INTERFACE_VERSION + 18)

// Trace plugin second level factory (this is what is known to PluginManager as "trace plugin")
class TraceFactory : public Firebird::Plugin
{
public:
	// What notifications does plugin need
	virtual ntrace_mask_t FB_CARG trace_needs() = 0;

	// Create plugin
	virtual TracePlugin* FB_CARG trace_create(Firebird::Status* status, TraceInitInfo* init_info) = 0;
};
#define FB_TRACE_FACTORY_VERSION (FB_PLUGIN_VERSION + 2)

enum TraceEvent
{
	// Bit is set if event is tracked
	TRACE_EVENT_ATTACH,
	TRACE_EVENT_DETACH,
	TRACE_EVENT_TRANSACTION_START,
	TRACE_EVENT_TRANSACTION_END,
	TRACE_EVENT_SET_CONTEXT,
	TRACE_EVENT_PROC_EXECUTE,
	TRACE_EVENT_TRIGGER_EXECUTE,
	TRACE_EVENT_DSQL_PREPARE,
	TRACE_EVENT_DSQL_FREE,
	TRACE_EVENT_DSQL_EXECUTE,
	TRACE_EVENT_BLR_COMPILE,
	TRACE_EVENT_BLR_EXECUTE,
	TRACE_EVENT_DYN_EXECUTE,
	TRACE_EVENT_SERVICE_ATTACH,
	TRACE_EVENT_SERVICE_START,
	TRACE_EVENT_SERVICE_QUERY,
	TRACE_EVENT_SERVICE_DETACH,
	TRACE_EVENT_MAX					// keep it last
};

#endif	// FIREBIRD_NTRACE_H
