/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		TraceManager.h
 *	DESCRIPTION:	Trace API manager
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
 *
 *  $Id: TraceManager.h,v 1.1 2004-12-09 07:20:26 skidder Exp $
 *
 */

#ifndef JRD_TRACEMANAGER_H
#define JRD_TRACEMANAGER_H

#include <time.h>

#include "../jrd/ntrace.h"
#include "../jrd/dsc.h"
#include "../common/classes/array.h"
#include "fb_string.h"

namespace Jrd {

class Database;
class Attachment;
class jrd_tra;

class TraceManager {
public:
    /* Initializes plugins. */
	TraceManager(Database *dbb);

	/* Finalize plugins. Called when database is closed by the engine */
	~TraceManager();

	void event_attach(ntrace_connection_t connection, bool create_db,
		ntrace_size_t dpb_length, const ntrace_byte_t *dpb);

	void event_detach(ntrace_connection_t connection, bool drop_db);

	/* Start/end transaction */
	void event_transaction_start(ntrace_connection_t connection, 
		ntrace_transaction_t transaction, ntrace_size_t tpb_length, 
		const ntrace_byte_t* tpb);

	void event_transaction_end(ntrace_connection_t connection, 
		ntrace_transaction_t transaction, bool commit, bool retain_context);

	void event_set_context(ntrace_connection_t connection, 
		ntrace_transaction_t transaction, ntrace_size_t ns_len, const char* ns,
		ntrace_size_t name_len, const char* name, ntrace_size_t value_len, const char* value);

	struct NotificationNeeds {
		// Whether we need to assign connection id for attachment
		bool need_connection_id;

		// Set if event is tracked
		bool event_attach;
		bool event_detach;
		bool event_transaction_start;
		bool event_transaction_end;
		bool event_dsql_prepare;
		bool event_dsql_execute;
		bool event_dsql_fetch;
		bool event_dsql_free;
		bool event_set_context;
	};

	inline const NotificationNeeds& needs() {
		return trace_needs;
	}

	/* DSQL-friendly routines to call Trace API hooks.
       Needed because DSQL cannot include JRD for the current engine */
	static bool need_dsql_prepare(Attachment *att);
	static bool need_dsql_execute(Attachment *att);
	static bool need_dsql_fetch(Attachment *att);
	static bool need_dsql_free(Attachment *att);

	static void event_dsql_prepare(Attachment *att, jrd_tra* transaction,
		ntrace_statement_t statement, ntrace_size_t sql_length, const char* sql, 
		ntrace_counter_t time_millis, ntrace_size_t access_path_length, 
		const ntrace_byte_t* access_path);

	static void event_dsql_execute(Attachment *att, jrd_tra* transaction,
		ntrace_statement_t statement, ntrace_size_t paramcount, 
		dsc* parameters, PerformanceInfo *info);

	static void event_dsql_fetch(Attachment *att, jrd_tra* transaction,
		ntrace_statement_t statement, PerformanceInfo *info, 
		ntrace_counter_t records_fetched);

	static void event_dsql_free(Attachment *att, 
		ntrace_statement_t statement, unsigned short option);
private:
	Database* dbb;
	NotificationNeeds trace_needs;

	// This structure should be POD-like to be stored in Array
	struct PluginInfo {
		const TracePlugin* plugin;
		char module[64];
	};

	Firebird::Array<PluginInfo> trace_plugins;

	bool check_result(const TracePlugin *plugin, const char *module, const char* function, bool result);

	/* DSQL statement lifecycle. To be moved to public and used directly when DSQL becomes a part of JRD */
	void event_dsql_prepare(ntrace_connection_t connection, 
		ntrace_transaction_t transaction, ntrace_statement_t statement, 
		ntrace_size_t sql_length, const char* sql, ntrace_counter_t time_millis, 
		ntrace_size_t access_path_length, const ntrace_byte_t* access_path);

	void event_dsql_execute(ntrace_connection_t connection, 
		ntrace_transaction_t transaction, ntrace_statement_t statement, 
		ntrace_size_t paramcount, dsc* parameters, PerformanceInfo *info);

	void event_dsql_fetch(ntrace_connection_t connection, ntrace_transaction_t transaction,
		ntrace_statement_t statement, PerformanceInfo *info, ntrace_counter_t records_fetched);

	void event_dsql_free(ntrace_connection_t connection,
		ntrace_statement_t statement, unsigned short option);

};

}

#endif
