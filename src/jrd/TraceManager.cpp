/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		TraceManager.cpp
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
 *  $Id: TraceManager.cpp,v 1.1 2004-12-09 07:20:26 skidder Exp $
 *
 */

#include "firebird.h"

#include "../jrd/TraceManager.h"
#include "../jrd/plugin_manager.h"
#include "../jrd/jrd.h"
#include "../jrd/tra.h"
#include "../jrd/os/path_utils.h"


namespace {

static const char* NTRACE_PREFIX = "fbtrace";
static const char* NTRACE_ATTACH = "trace_attach";

}

namespace Jrd {

bool TraceManager::check_result(const TracePlugin *plugin, const char *module, const char* function, bool result) {
	if (result)
		return true;

	if (!plugin) {
		gds__log("Trace plugin %s returned error on call %s, "
			"did not create plugin and provided no additional details on reasons of failure", module, function);
		return false;
	}

	const char* errorStr = plugin->tpl_get_error(plugin);

	if (!errorStr)
	{
		gds__log("Trace plugin %s returned error on call %s, "
			"but provided no additional details on reasons of failure", module, function);
		return false;
	}

	gds__log("Trace plugin %s returned error on call %s. Error details: %s", 
		module, function, errorStr);
	return false;
}

TraceManager::TraceManager(Database *dbb) : trace_plugins(*dbb->dbb_permanent) {
	this->dbb = dbb;

	// Initialize all trace needs to false
	memset(&trace_needs, 0, sizeof(trace_needs));

	for (PluginManager::iterator i = PluginManager::enginePluginManager().begin(); *i; ++i) {
		PluginManager::Plugin p = *i;
		const Firebird::PathName *name = p.name();
		if (!name) 
			continue;

		Firebird::PathName pathComponent, modName;			
		PathUtils::splitLastComponent(pathComponent, modName, *name);

		if (modName.substr(0, strlen(NTRACE_PREFIX)) != NTRACE_PREFIX)
			continue;

		ntrace_attach_t ntrace_attach_func = 
			static_cast<ntrace_attach_t>((*i).lookupSymbol(NTRACE_ATTACH));

		if (!ntrace_attach_func)
			continue;

		const TracePlugin *plugin = NULL;
		if (!check_result(plugin, modName.c_str(), NTRACE_ATTACH, 
			ntrace_attach_func(dbb->dbb_database_name.c_str(), &plugin))) 
		{
			// This was a skeletal plugin to return an error
			if (plugin && plugin->tpl_shutdown)
				plugin->tpl_shutdown(plugin);
		}

		if (plugin)
		{
			PluginInfo info;
			info.plugin = plugin;
			strncpy(info.module, modName.c_str(), sizeof(info.module));
			info.module[sizeof(info.module)-1] = 0;
			trace_plugins.add(info);

			if (plugin->tpl_event_attach)
				trace_needs.event_attach = true;
			if (plugin->tpl_event_detach)
				trace_needs.event_detach = true;
			if (plugin->tpl_event_dsql_execute)
				trace_needs.event_dsql_execute = true;
			if (plugin->tpl_event_dsql_fetch)
				trace_needs.event_dsql_fetch = true;
			if (plugin->tpl_event_dsql_free)
				trace_needs.event_dsql_free = true;
			if (plugin->tpl_event_dsql_prepare)
				trace_needs.event_dsql_prepare = true;
			if (plugin->tpl_event_transaction_end)
				trace_needs.event_transaction_end = true;
			if (plugin->tpl_event_transaction_start)
				trace_needs.event_transaction_start = true;
			if (plugin->tpl_event_set_context)
				trace_needs.event_set_context = true;

			// This is pointless to install plugin and not install any hooks, right?
			// So let's enforce generation of ID for attachment as each hook requires attachment_id
			trace_needs.need_connection_id = true;
		}
	}
}

TraceManager::~TraceManager() {
	// Destroy all plugins
	for (PluginInfo* info = trace_plugins.begin(); info < trace_plugins.end(); info++)
	{
		check_result(NULL, info->module, "tpl_shutdown", 
			info->plugin->tpl_shutdown(info->plugin));
	}
}

bool TraceManager::need_dsql_prepare(Attachment *att) {
	return att->att_database->dbb_trace_manager->needs().event_dsql_prepare;
}

bool TraceManager::need_dsql_execute(Attachment *att) {
	return att->att_database->dbb_trace_manager->needs().event_dsql_execute;
}

bool TraceManager::need_dsql_fetch(Attachment *att) {
	return att->att_database->dbb_trace_manager->needs().event_dsql_fetch;
}

bool TraceManager::need_dsql_free(Attachment *att) {
	return att->att_database->dbb_trace_manager->needs().event_dsql_free;
}

void TraceManager::event_dsql_prepare(Attachment *att, jrd_tra* transaction,
		ntrace_statement_t statement, ntrace_size_t sql_length, const char* sql, 
		ntrace_counter_t time_millis, 
		ntrace_size_t access_path_length, const ntrace_byte_t* access_path)
{
	att->att_database->dbb_trace_manager->event_dsql_prepare(
		att->att_attachment_id, transaction->tra_number, statement,
		sql_length, sql, time_millis, access_path_length, access_path);
}

void TraceManager::event_dsql_execute(Attachment *att, jrd_tra* transaction,
		ntrace_statement_t statement, ntrace_size_t paramcount, 
		struct dsc* parameters, PerformanceInfo *info)
{
	att->att_database->dbb_trace_manager->event_dsql_execute(
		att->att_attachment_id, transaction->tra_number, statement, 
		paramcount, parameters, info);
}

void TraceManager::event_dsql_fetch(Attachment *att, jrd_tra* transaction,
		ntrace_statement_t statement, PerformanceInfo *info, ntrace_counter_t records_fetched)
{
	att->att_database->dbb_trace_manager->event_dsql_fetch(
		att->att_attachment_id, transaction->tra_number, statement, 
		info, records_fetched);
}

void TraceManager::event_dsql_free(Attachment *att,	ntrace_statement_t statement, unsigned short option)
{
	att->att_database->dbb_trace_manager->event_dsql_free(
		att->att_attachment_id, statement, option);
}


#define EXECUTE_HOOKS(METHOD, PARAMS) \
	size_t i = 0; \
	while(i < trace_plugins.getCount()) \
	{ \
		PluginInfo* plug_info = &trace_plugins[i]; \
		if (!plug_info->plugin->METHOD || \
			check_result(plug_info->plugin, plug_info->module, #METHOD, \
			plug_info->plugin->METHOD PARAMS)) \
		{ \
			i++; /* Move to next plugin */ \
		} \
		else { \
			trace_plugins.remove(i); /* Remove broken plugin from the list */ \
		} \
	}



void TraceManager::event_attach(ntrace_connection_t connection,
		bool create_db, ntrace_size_t dpb_length, const ntrace_byte_t *dpb)
{
	EXECUTE_HOOKS(tpl_event_attach, (plug_info->plugin, connection, create_db, dpb_length, dpb));
}

void TraceManager::event_detach(ntrace_connection_t connection, bool drop_db)
{
	EXECUTE_HOOKS(tpl_event_detach, (plug_info->plugin, connection, drop_db));
}

void TraceManager::event_transaction_start(ntrace_connection_t connection, 
		ntrace_transaction_t transaction, ntrace_size_t tpb_length, 
		const ntrace_byte_t* tpb)
{
	EXECUTE_HOOKS(tpl_event_transaction_start, 
		(plug_info->plugin, connection, transaction, tpb_length, tpb));
}

void TraceManager::event_transaction_end(ntrace_connection_t connection, 
		ntrace_transaction_t transaction, bool commit, bool retain_context)
{
	EXECUTE_HOOKS(tpl_event_transaction_end, 
		(plug_info->plugin, connection, transaction, commit, retain_context));
}

void TraceManager::event_set_context(ntrace_connection_t connection, 
	ntrace_transaction_t transaction, ntrace_size_t ns_len, const char* ns,
	ntrace_size_t name_len, const char* name, ntrace_size_t value_len, const char* value)
{
	EXECUTE_HOOKS(tpl_event_set_context, 
		(plug_info->plugin, connection, transaction, ns_len, ns, name_len, name, value_len, value));
}

void TraceManager::event_dsql_prepare(ntrace_connection_t connection, 
		ntrace_transaction_t transaction, ntrace_statement_t statement, 
		ntrace_size_t sql_length, const char* sql, ntrace_counter_t time_millis, 
		ntrace_size_t access_path_length, const ntrace_byte_t* access_path)
{
	EXECUTE_HOOKS(tpl_event_dsql_prepare, 
		(plug_info->plugin, connection, transaction, statement,
		 sql_length, sql, time_millis, access_path_length, access_path));
}

void TraceManager::event_dsql_execute(ntrace_connection_t connection, 
		ntrace_transaction_t transaction, ntrace_statement_t statement, 
		ntrace_size_t paramcount, struct dsc* parameters, PerformanceInfo *info)
{
	EXECUTE_HOOKS(tpl_event_dsql_execute, 
		(plug_info->plugin, connection, transaction, statement, paramcount, (struct paramdsc*) parameters, info));
}

void TraceManager::event_dsql_fetch(ntrace_connection_t connection, ntrace_transaction_t transaction,
		ntrace_statement_t statement, PerformanceInfo *info, ntrace_counter_t records_fetched)
{
	EXECUTE_HOOKS(tpl_event_dsql_fetch, 
		(plug_info->plugin, connection, transaction, statement, info, records_fetched));
}

void TraceManager::event_dsql_free(ntrace_connection_t connection,
								   ntrace_statement_t statement, unsigned short option)
{
	EXECUTE_HOOKS(tpl_event_dsql_free, 
		(plug_info->plugin, connection, statement, option));
}

}
