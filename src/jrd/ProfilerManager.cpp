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
 *  The Original Code was created by Adriano dos Santos Fernandes
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2020 Adriano dos Santos Fernandes <adrianosf@gmail.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../jrd/ProfilerManager.h"
#include "../jrd/Record.h"
#include "../jrd/ini.h"
#include "../jrd/tra.h"
#include "../jrd/ids.h"
#include "../jrd/recsrc/Cursor.h"
#include "../jrd/dpm_proto.h"
#include "../jrd/met_proto.h"

using namespace Jrd;
using namespace Firebird;


//--------------------------------------


IExternalResultSet* ProfilerPackage::discardProcedure(ThrowStatusExceptionWrapper* /*status*/,
	IExternalContext* context, const void* in, void* out)
{
	const auto tdbb = JRD_get_thread_data();
	const auto attachment = tdbb->getAttachment();

	const auto profilerManager = attachment->getProfilerManager(tdbb);

	profilerManager->discard();

	return nullptr;
}

IExternalResultSet* ProfilerPackage::flushProcedure(ThrowStatusExceptionWrapper* /*status*/,
	IExternalContext* context, const void* in, void* out)
{
	const auto tdbb = JRD_get_thread_data();
	const auto attachment = tdbb->getAttachment();
	const auto transaction = tdbb->getTransaction();

	const auto profilerManager = attachment->getProfilerManager(tdbb);
	AutoSetRestore<bool> pauseProfiler(&profilerManager->paused, true);

	profilerManager->flush(transaction->getInterface(true));

	return nullptr;
}

IExternalResultSet* ProfilerPackage::cancelSessionProcedure(ThrowStatusExceptionWrapper* /*status*/,
	IExternalContext* context, const void* in, void* out)
{
	const auto tdbb = JRD_get_thread_data();
	const auto attachment = tdbb->getAttachment();
	const auto transaction = tdbb->getTransaction();

	const auto profilerManager = attachment->getProfilerManager(tdbb);

	if (profilerManager->currentSession)
	{
		LogLocalStatus status("Profiler cancelSession");

		profilerManager->currentSession->pluginSession->cancel(&status);
		profilerManager->currentSession = nullptr;
	}

	return nullptr;
}

IExternalResultSet* ProfilerPackage::finishSessionProcedure(ThrowStatusExceptionWrapper* /*status*/,
	IExternalContext* context, const FinishSessionInput::Type* in, void* out)
{
	const auto tdbb = JRD_get_thread_data();
	const auto attachment = tdbb->getAttachment();
	const auto transaction = tdbb->getTransaction();

	const auto profilerManager = attachment->getProfilerManager(tdbb);
	AutoSetRestore<bool> pauseProfiler(&profilerManager->paused, true);

	if (profilerManager->currentSession)
	{
		const auto timestamp = TimeZoneUtil::getCurrentTimeStamp(attachment->att_current_timezone);
		LogLocalStatus status("Profiler finish");

		profilerManager->currentSession->pluginSession->finish(&status, timestamp);
		profilerManager->currentSession = nullptr;
	}

	if (in->flush)
		profilerManager->flush(transaction->getInterface(true));

	return nullptr;
}

IExternalResultSet* ProfilerPackage::pauseSessionProcedure(ThrowStatusExceptionWrapper* /*status*/,
	IExternalContext* context, const PauseSessionInput::Type* in, void* out)
{
	const auto tdbb = JRD_get_thread_data();
	const auto attachment = tdbb->getAttachment();
	const auto transaction = tdbb->getTransaction();

	const auto profilerManager = attachment->getProfilerManager(tdbb);

	if (!profilerManager->currentSession)
		return nullptr;

	profilerManager->paused = true;

	if (in->flush)
		profilerManager->flush(transaction->getInterface(true));

	return nullptr;
}

IExternalResultSet* ProfilerPackage::resumeSessionProcedure(ThrowStatusExceptionWrapper* /*status*/,
	IExternalContext* context, const void* in, void* out)
{
	const auto tdbb = JRD_get_thread_data();
	const auto attachment = tdbb->getAttachment();

	const auto profilerManager = attachment->getProfilerManager(tdbb);

	if (profilerManager->currentSession && profilerManager->paused)
		profilerManager->paused = false;

	return nullptr;
}

void ProfilerPackage::startSessionFunction(ThrowStatusExceptionWrapper* /*status*/,
	IExternalContext* context, const StartSessionInput::Type* in, StartSessionOutput::Type* out)
{
	const auto tdbb = JRD_get_thread_data();
	const auto attachment = tdbb->getAttachment();
	const auto transaction = tdbb->getTransaction();

	const string description(in->description.str, in->descriptionNull ? 0 : in->description.length);
	const PathName pluginName(in->pluginName.str, in->pluginNameNull ? 0 : in->pluginName.length);
	const string pluginOptions(in->pluginOptions.str, in->pluginOptionsNull ? 0 : in->pluginOptions.length);

	const auto profilerManager = attachment->getProfilerManager(tdbb);
	AutoSetRestore<bool> pauseProfiler(&profilerManager->paused, true);

	out->sessionIdNull = FB_FALSE;
	out->sessionId = profilerManager->startSession(tdbb, pluginName, description, pluginOptions);
}


//--------------------------------------


ProfilerManager::ProfilerManager(thread_db* tdbb)
	: activePlugins(*tdbb->getAttachment()->att_pool)
{
}

ProfilerManager* ProfilerManager::create(thread_db* tdbb)
{
	return FB_NEW_POOL(*tdbb->getAttachment()->att_pool) ProfilerManager(tdbb);
}

SINT64 ProfilerManager::startSession(thread_db* tdbb, const PathName& pluginName, const string& description,
	const string& options)
{
	const auto attachment = tdbb->getAttachment();
	const auto transaction = tdbb->getTransaction();
	ThrowLocalStatus status;

	const auto timestamp = TimeZoneUtil::getCurrentTimeStamp(attachment->att_current_timezone);

	if (currentSession)
	{
		currentSession->pluginSession->finish(&status, timestamp);
		currentSession = nullptr;
	}

	auto pluginPtr = activePlugins.get(pluginName);

	AutoPlugin<IProfilerPlugin> plugin;

	if (pluginPtr)
	{
		(*pluginPtr)->addRef();
		plugin.reset(pluginPtr->get());
	}
	else
	{
		GetPlugins<IProfilerPlugin> plugins(IPluginManager::TYPE_PROFILER, pluginName.nullStr());

		if (!plugins.hasData())
		{
			string msg;
			msg.printf("Profiler plugin %s is not found", pluginName.c_str());
			(Arg::Gds(isc_random) << msg).raise();
		}

		plugin.reset(plugins.plugin());
		plugin->addRef();

		plugin->init(&status, attachment->getInterface(), transaction->getInterface(true));

		plugin->addRef();
		activePlugins.put(pluginName)->reset(plugin.get());
	}

	AutoDispose<IProfilerSession> pluginSession = plugin->startSession(&status,
		transaction->getInterface(true),
		description.c_str(),
		options.c_str(),
		timestamp);

	auto& pool = *tdbb->getAttachment()->att_pool;

	currentSession.reset(FB_NEW_POOL(pool) ProfilerManager::Session(pool));
	currentSession->pluginSession = std::move(pluginSession);
	currentSession->plugin = std::move(plugin);
	currentSession->flags = currentSession->pluginSession->getFlags();

	paused = false;

	return currentSession->pluginSession->getId();
}

void ProfilerManager::prepareRecSource(thread_db* tdbb, jrd_req* request, const RecordSource* rsb)
{
	auto profileStatement = getStatement(request);

	if (!profileStatement)
		return;

	if (profileStatement->recSourceSequence.exist(rsb->getRecSourceProfileId()))
		return;

	Array<NonPooledPair<const RecordSource*, const RecordSource*>> tree;
	tree.add({rsb, nullptr});

	for (unsigned pos = 0; pos < tree.getCount(); ++pos)
	{
		const auto thisRsb = tree[pos].first;

		Array<const RecordSource*> children;
		thisRsb->getChildren(children);

		unsigned childPos = pos;

		for (const auto child : children)
			tree.insert(++childPos, {child, thisRsb});
	}

	NonPooledMap<ULONG, ULONG> idSequenceMap;
	auto sequencePtr = profileStatement->cursorNextSequence.getOrPut(rsb->getCursorProfileId());

	for (const auto& pair : tree)
	{
		const auto cursorId = pair.first->getCursorProfileId();
		const auto recSourceId = pair.first->getRecSourceProfileId();
		idSequenceMap.put(recSourceId, ++*sequencePtr);

		string accessPath;
		pair.first->print(tdbb, accessPath, true, 0, false);

		constexpr auto INDENT_MARKER = "\n    ";

		if (accessPath.find(INDENT_MARKER) == 0)
		{
			unsigned pos = 0;

			do {
				accessPath.erase(pos + 1, 4);
			} while ((pos = accessPath.find(INDENT_MARKER, pos + 1)) != string::npos);
		}

		ULONG parentSequence = 0;

		if (pair.second)
			parentSequence = *idSequenceMap.get(pair.second->getRecSourceProfileId());

		currentSession->pluginSession->defineRecordSource(profileStatement->id, cursorId,
			*sequencePtr, accessPath.c_str(), parentSequence);

		profileStatement->recSourceSequence.put(recSourceId, *sequencePtr);
	}
}

void ProfilerManager::onRequestFinish(jrd_req* request)
{
	if (const auto profileRequestId = getRequest(request, 0))
	{
		const auto timestamp = TimeZoneUtil::getCurrentTimeStamp(request->req_attachment->att_current_timezone);

		LogLocalStatus status("Profiler onRequestFinish");
		currentSession->pluginSession->onRequestFinish(&status, profileRequestId, timestamp);

		currentSession->requests.findAndRemove(profileRequestId);
	}
}

void ProfilerManager::beforePsqlLineColumn(jrd_req* request, ULONG line, ULONG column)
{
	if (const auto profileRequestId = getRequest(request, IProfilerSession::FLAG_BEFORE_EVENTS))
		currentSession->pluginSession->beforePsqlLineColumn(profileRequestId, line, column);
}

void ProfilerManager::afterPsqlLineColumn(jrd_req* request, ULONG line, ULONG column, FB_UINT64 runTime)
{
	if (const auto profileRequestId = getRequest(request, IProfilerSession::FLAG_AFTER_EVENTS))
		currentSession->pluginSession->afterPsqlLineColumn(profileRequestId, line, column, runTime);
}

void ProfilerManager::beforeRecordSourceOpen(jrd_req* request, const RecordSource* rsb)
{
	if (const auto profileRequestId = getRequest(request, IProfilerSession::FLAG_BEFORE_EVENTS))
	{
		const auto profileStatement = getStatement(request);
		const auto sequencePtr = profileStatement->recSourceSequence.get(rsb->getRecSourceProfileId());
		fb_assert(sequencePtr);

		currentSession->pluginSession->beforeRecordSourceOpen(
			profileRequestId, rsb->getCursorProfileId(), *sequencePtr);
	}
}

void ProfilerManager::afterRecordSourceOpen(jrd_req* request, const RecordSource* rsb, FB_UINT64 runTime)
{
	if (const auto profileRequestId = getRequest(request, IProfilerSession::FLAG_AFTER_EVENTS))
	{
		const auto profileStatement = getStatement(request);
		const auto sequencePtr = profileStatement->recSourceSequence.get(rsb->getRecSourceProfileId());
		fb_assert(sequencePtr);

		currentSession->pluginSession->afterRecordSourceOpen(
			profileRequestId, rsb->getCursorProfileId(), *sequencePtr, runTime);
	}
}

void ProfilerManager::beforeRecordSourceGetRecord(jrd_req* request, const RecordSource* rsb)
{
	if (const auto profileRequestId = getRequest(request, IProfilerSession::FLAG_BEFORE_EVENTS))
	{
		const auto profileStatement = getStatement(request);
		const auto sequencePtr = profileStatement->recSourceSequence.get(rsb->getRecSourceProfileId());
		fb_assert(sequencePtr);

		currentSession->pluginSession->beforeRecordSourceGetRecord(
			profileRequestId, rsb->getCursorProfileId(), *sequencePtr);
	}
}

void ProfilerManager::afterRecordSourceGetRecord(jrd_req* request, const RecordSource* rsb, FB_UINT64 runTime)
{
	if (const auto profileRequestId = getRequest(request, IProfilerSession::FLAG_AFTER_EVENTS))
	{
		const auto profileStatement = getStatement(request);
		const auto sequencePtr = profileStatement->recSourceSequence.get(rsb->getRecSourceProfileId());
		fb_assert(sequencePtr);

		currentSession->pluginSession->afterRecordSourceGetRecord(
			profileRequestId, rsb->getCursorProfileId(), *sequencePtr, runTime);
	}
}

void ProfilerManager::discard()
{
	currentSession = nullptr;
	activePlugins.clear();
}

void ProfilerManager::flush(ITransaction* transaction)
{
	auto pluginAccessor = activePlugins.accessor();

	for (bool hasNext = pluginAccessor.getFirst(); hasNext;)
	{
		auto& pluginName = pluginAccessor.current()->first;
		auto& plugin = pluginAccessor.current()->second;

		LogLocalStatus status("Profiler flush");
		plugin->flush(&status, transaction);

		hasNext = pluginAccessor.getNext();

		if (!currentSession || plugin.get() != currentSession->plugin.get())
			activePlugins.remove(pluginName);
	}
}

ProfilerManager::Statement* ProfilerManager::getStatement(jrd_req* request)
{
	if (!isActive())
		return nullptr;

	auto mainProfileStatement = currentSession->statements.get(request->getStatement()->getStatementId());

	if (mainProfileStatement)
		return mainProfileStatement;

	for (const auto* statement = request->getStatement();
		 statement && !currentSession->statements.exist(statement->getStatementId());
		 statement = statement->parentStatement)
	{
		MetaName packageName;
		MetaName routineName;
		const char* type;

		if (const auto routine = statement->getRoutine())
		{
			if (statement->procedure)
				type = "PROCEDURE";
			else if (statement->function)
				type = "FUNCTION";

			packageName = routine->getName().package;
			routineName = routine->getName().identifier;
		}
		else if (statement->triggerName.hasData())
		{
			type = "TRIGGER";
			routineName = statement->triggerName;
		}
		else
			type = "BLOCK";

		const StmtNumber parentStatementId = statement->parentStatement ?
			statement->parentStatement->getStatementId() : 0;

		LogLocalStatus status("Profiler defineStatement");
		currentSession->pluginSession->defineStatement(&status,
			(SINT64) statement->getStatementId(), (SINT64) parentStatementId,
			type, packageName.nullStr(), routineName.nullStr(),
			(statement->sqlText.hasData() ? statement->sqlText->c_str() : ""));

		auto profileStatement = currentSession->statements.put(statement->getStatementId());
		profileStatement->id = statement->getStatementId();

		if (!mainProfileStatement)
			mainProfileStatement = profileStatement;
	}

	return mainProfileStatement;
}

SINT64 ProfilerManager::getRequest(jrd_req* request, unsigned flags)
{
	if (!isActive() || (flags && !(currentSession->flags & flags)))
		return 0;

	const auto mainRequestId = request->getRequestId();

	if (!currentSession->requests.exist(mainRequestId))
	{
		const auto timestamp = TimeZoneUtil::getCurrentTimeStamp(request->req_attachment->att_current_timezone);

		do
		{
			getStatement(request);  // define the statement and ignore the result

			const StmtNumber callerRequestId = request->req_caller ? request->req_caller->getRequestId() : 0;

			LogLocalStatus status("Profiler onRequestStart");
			currentSession->pluginSession->onRequestStart(&status,
				(SINT64) request->getRequestId(), (SINT64) request->getStatement()->getStatementId(),
				(SINT64) callerRequestId, timestamp);

			currentSession->requests.add(request->getRequestId());

			request = request->req_caller;
		} while (request && !currentSession->requests.exist(request->getRequestId()));
	}

	return mainRequestId;
}


//--------------------------------------


ProfilerPackage::ProfilerPackage(MemoryPool& pool)
	: SystemPackage(
		pool,
		"RDB$PROFILER",
		ODS_13_1,
		// procedures
		{
			SystemProcedure(
				pool,
				"CANCEL_SESSION",
				SystemProcedureFactory<VoidMessage, VoidMessage, cancelSessionProcedure>(),
				prc_executable,
				// input parameters
				{
				},
				// output parameters
				{
				}
			),
			SystemProcedure(
				pool,
				"DISCARD",
				SystemProcedureFactory<VoidMessage, VoidMessage, discardProcedure>(),
				prc_executable,
				// input parameters
				{
				},
				// output parameters
				{
				}
			),
			SystemProcedure(
				pool,
				"FINISH_SESSION",
				SystemProcedureFactory<FinishSessionInput, VoidMessage, finishSessionProcedure>(),
				prc_executable,
				// input parameters
				{
					{"FLUSH", fld_bool, false, "true", {blr_literal, blr_bool, 1}}
				},
				// output parameters
				{
				}
			),
			SystemProcedure(
				pool,
				"FLUSH",
				SystemProcedureFactory<VoidMessage, VoidMessage, flushProcedure>(),
				prc_executable,
				// input parameters
				{
				},
				// output parameters
				{
				}
			),
			SystemProcedure(
				pool,
				"PAUSE_SESSION",
				SystemProcedureFactory<PauseSessionInput, VoidMessage, pauseSessionProcedure>(),
				prc_executable,
				// input parameters
				{
					{"FLUSH", fld_bool, false, "false", {blr_literal, blr_bool, 0}}
				},
				// output parameters
				{
				}
			),
			SystemProcedure(
				pool,
				"RESUME_SESSION",
				SystemProcedureFactory<VoidMessage, VoidMessage, resumeSessionProcedure>(),
				prc_executable,
				// input parameters
				{
				},
				// output parameters
				{
				}
			)
		},
		// functions
		{
			SystemFunction(
				pool,
				"START_SESSION",
				SystemFunctionFactory<StartSessionInput, StartSessionOutput, startSessionFunction>(),
				// parameters
				{
					{"DESCRIPTION", fld_short_description, true, "null", {blr_null}},
					{"PLUGIN_NAME", fld_file_name2, true, "null", {blr_null}},
					{"PLUGIN_OPTIONS", fld_short_description, true, "null", {blr_null}},
				},
				{fld_prof_ses_id, false}
			)
		}
	)
{
}
