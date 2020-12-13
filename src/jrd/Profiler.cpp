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
#include "../jrd/Profiler.h"
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


IExternalResultSet* ProfilerPackage::refreshSnapshotsProcedure(ThrowStatusExceptionWrapper* status,
	IExternalContext* context, const void* in, void* out)
{
	const auto tdbb = JRD_get_thread_data();
	const auto attachment = tdbb->getAttachment();

	Attachment::SyncGuard guard(attachment, FB_FUNCTION);

	const auto profiler = attachment->getProfiler(tdbb);

	profiler->refreshSnapshots(tdbb);

	return nullptr;
}

IExternalResultSet* ProfilerPackage::finishSessionProcedure(ThrowStatusExceptionWrapper* status,
	IExternalContext* context, const FinishSessionInput::Type* in, void* out)
{
	const auto tdbb = JRD_get_thread_data();
	const auto attachment = tdbb->getAttachment();

	Attachment::SyncGuard guard(attachment, FB_FUNCTION);

	const auto profiler = attachment->getProfiler(tdbb);

	if (profiler->activeSession)
	{
		profiler->activeSession = false;

		const auto profileSession = profiler->sessions.get(profiler->currentSessionId);

		profileSession->finishTimeStamp = TimeZoneUtil::getCurrentSystemTimeStamp();
		profileSession->finishTimeStamp.value.time_zone = attachment->att_current_timezone;
	}

	profiler->currentSessionId = 0;

	if (in->refreshSnapshots)
		profiler->refreshSnapshots(tdbb);

	return nullptr;
}

IExternalResultSet* ProfilerPackage::pauseSessionProcedure(ThrowStatusExceptionWrapper* status,
	IExternalContext* context, const PauseSessionInput::Type* in, void* out)
{
	const auto tdbb = JRD_get_thread_data();
	const auto attachment = tdbb->getAttachment();

	Attachment::SyncGuard guard(attachment, FB_FUNCTION);

	const auto profiler = attachment->getProfiler(tdbb);

	if (!profiler->activeSession)
		return nullptr;

	profiler->paused = true;

	if (in->refreshSnapshots)
		profiler->refreshSnapshots(tdbb);

	return nullptr;
}

IExternalResultSet* ProfilerPackage::resumeSessionProcedure(ThrowStatusExceptionWrapper* status,
	IExternalContext* context, const void* in, void* out)
{
	const auto tdbb = JRD_get_thread_data();
	const auto attachment = tdbb->getAttachment();

	Attachment::SyncGuard guard(attachment, FB_FUNCTION);

	const auto profiler = attachment->getProfiler(tdbb);

	if (profiler->activeSession && profiler->paused)
		profiler->paused = false;

	return nullptr;
}

void ProfilerPackage::startSessionFunction(ThrowStatusExceptionWrapper* status,
	IExternalContext* context, const StartSessionInput::Type* in, StartSessionOutput::Type* out)
{
	const auto tdbb = JRD_get_thread_data();
	const auto attachment = tdbb->getAttachment();

	Attachment::SyncGuard guard(attachment, FB_FUNCTION);

	const auto profiler = attachment->getProfiler(tdbb);

	profiler->activeSession = true;
	profiler->paused = false;

	const auto generator = MET_lookup_generator(tdbb, PROFILE_SESSION_GENERATOR);
	const auto sessionId = profiler->currentSessionId = DPM_gen_id(tdbb, generator, false, 1);

	profiler->sessions.put(sessionId)->init(attachment,
		in->descriptionNull ? "" : string(string(in->description.str, in->description.length)));

	out->sessionIdNull = FB_FALSE;
	out->sessionId = sessionId;
}


//--------------------------------------


void Profiler::Request::init(jrd_req* request)
{
	timeStamp.time_zone = request->req_attachment->att_current_timezone;

	if (request->getStatement()->sqlText.hasData())
		sqlText = *request->getStatement()->sqlText;
}


void Profiler::Session::init(Attachment* attachment, const string& aDescription)
{
	startTimeStamp.time_zone = attachment->att_current_timezone;
	description = aDescription;
}

//--------------------------------------


Profiler::Profiler(thread_db* tdbb)
	: sessions(*tdbb->getAttachment()->att_pool)
{
}

Profiler* Profiler::create(thread_db* tdbb)
{
	return FB_NEW_POOL(*tdbb->getAttachment()->att_pool) Profiler(tdbb);
}

void Profiler::deleteSessionDetails(thread_db* tdbb, SINT64 sessionId)
{
	const auto attachment = tdbb->getAttachment();

	AutoSetRestore<bool> autoInSystemPackage(&attachment->att_in_system_routine, true);

	IAttachment* const attachmentIntf = attachment->getInterface();
	ITransaction* const transactionIntf = tdbb->getTransaction()->getInterface(false);
	ThrowLocalStatus throwStatus;

	const char* deleteSessionSql = R"""(
		execute block (session_id type of rdb$profile_session_id = ?) as
		begin
			delete from rdb$profile_record_source_stats where rdb$profile_session_id = :session_id;
			delete from rdb$profile_stats where rdb$profile_session_id = :session_id;
			delete from rdb$profile_requests where rdb$profile_session_id = :session_id;
		end
	)""";

	FB_MESSAGE(SessionMessage, ThrowWrapper,
		(FB_BIGINT, sessionId)
	) sessionMessage(&throwStatus, fb_get_master_interface());
	sessionMessage.clear();

	sessionMessage->sessionId = sessionId;
	sessionMessage->sessionIdNull = FB_FALSE;

	attachmentIntf->execute(&throwStatus, transactionIntf, 0, deleteSessionSql, SQL_DIALECT_CURRENT,
		sessionMessage.getMetadata(), sessionMessage.getData(), nullptr, nullptr);
}

void Profiler::setupCursor(thread_db* tdbb, jrd_req* request, const Cursor* cursor)
{
	if (!isActive())
		return;

	auto profileRequest = getRequest(request);

	auto profileCursor = profileRequest->cursors.get(cursor->getProfileId());

	if (!profileCursor)
	{
		const auto profileSession = sessions.get(currentSessionId);
		fb_assert(profileSession);

		profileCursor = profileRequest->cursors.put(cursor->getProfileId());

		using PairType = Pair<NonPooled<const RecordSource*, const RecordSource*>>;
		Array<PairType> tree;
		tree.add(PairType(cursor->getAccessPath(), nullptr));

		for (unsigned pos = 0; pos < tree.getCount(); ++pos)
		{
			const auto rsb = tree[pos].first;

			Array<const RecordSource*> children;
			rsb->getChildren(children);

			unsigned childPos = pos;

			for (const auto child : children)
				tree.insert(++childPos, PairType(child, rsb));
		}

		unsigned sequence = 0;

		for (const auto& pair : tree)
		{
			auto profileRecSource = profileCursor->sources.put(pair.first->getRecSourceProfileId());
			profileRecSource->sequence = ++sequence;
			pair.first->print(tdbb, profileRecSource->accessPath, true, 0, false);

			const auto markerPos = profileRecSource->accessPath.find("-> ");
			if (markerPos != string::npos)
				profileRecSource->accessPath = profileRecSource->accessPath.substr(markerPos + 3);

			if (pair.second)
			{
				const auto parentSource = profileCursor->sources.get(pair.second->getRecSourceProfileId());
				fb_assert(parentSource);
				profileRecSource->parentSequence = parentSource->sequence;
			}

			profileRequest->sourcesCursor.put(pair.first->getRecSourceProfileId(), cursor->getProfileId());
		}
	}
}

void Profiler::hitLineColumn(jrd_req* request, ULONG line, ULONG column, SINT64 runTime)
{
	if (!isActive())
		return;

	auto profileRequest = getRequest(request);

	const auto statsKey = encodeLineColumn(line, column);
	auto profileStats = profileRequest->stats.get(statsKey);

	if (!profileStats)
		profileStats = profileRequest->stats.put(statsKey);

	profileStats->hit(runTime);
}

void Profiler::hitRecSourceOpen(jrd_req* request, ULONG recSourceId, SINT64 runTime)
{
	if (!isActive())
		return;

	auto profileRequest = getRequest(request);

	const auto cursorIdPtr = profileRequest->sourcesCursor.get(recSourceId);
	fb_assert(cursorIdPtr);

	auto profileCursor = profileRequest->cursors.get(*cursorIdPtr);
	fb_assert(profileCursor);

	const auto profileRecSource = profileCursor->sources.get(recSourceId);
	fb_assert(profileRecSource);

	profileRecSource->openStats.hit(runTime);
}

void Profiler::hitRecSourceGetRecord(jrd_req* request, ULONG recSourceId, SINT64 runTime)
{
	if (!isActive())
		return;

	auto profileRequest = getRequest(request);

	const auto cursorIdPtr = profileRequest->sourcesCursor.get(recSourceId);
	fb_assert(cursorIdPtr);

	auto profileCursor = profileRequest->cursors.get(*cursorIdPtr);
	fb_assert(profileCursor);

	const auto profileRecSource = profileCursor->sources.get(recSourceId);
	fb_assert(profileRecSource);

	profileRecSource->fetchStats.hit(runTime);
}

void Profiler::refreshSnapshots(thread_db* tdbb)
{
	const static UCHAR TEMP_BPB[] = {isc_bpb_version1, isc_bpb_storage, 1, isc_bpb_storage_temp};

	IAttachment* const attachment = tdbb->getAttachment()->getInterface();
	ITransaction* const transaction = tdbb->getTransaction()->getInterface(false);
	ThrowLocalStatus throwStatus;

	const char* sessionSql = R"""(
		update or insert into rdb$profile_sessions
			(rdb$profile_session_id, rdb$attachment_id, rdb$user, rdb$description, rdb$start_timestamp, rdb$finish_timestamp)
			values (?, ?, ?, ?, ?, ?)
			matching (rdb$profile_session_id);
	)""";

	FB_MESSAGE(SessionMessage, ThrowWrapper,
		(FB_BIGINT, sessionId)
		(FB_BIGINT, attachmentId)
		(FB_INTL_VARCHAR(METADATA_IDENTIFIER_CHAR_LEN * 4, CS_UTF8), user)
		(FB_INTL_VARCHAR(255 * 4, CS_UTF8), description)
		(FB_TIMESTAMP_TZ, startTimestamp)
		(FB_TIMESTAMP_TZ, finishTimestamp)
	) sessionMessage(&throwStatus, fb_get_master_interface());
	sessionMessage.clear();

	const char* requestSql = R"""(
		update or insert into rdb$profile_requests
			(rdb$profile_session_id, rdb$profile_request_id, rdb$timestamp, rdb$request_type, rdb$package_name,
			 rdb$routine_name, rdb$sql_text)
			values (?, ?, ?, ?, ?, ?, ?)
			matching (rdb$profile_session_id, rdb$profile_request_id)
	)""";

	FB_MESSAGE(RequestMessage, ThrowWrapper,
		(FB_BIGINT, sessionId)
		(FB_BIGINT, requestId)
		(FB_TIMESTAMP_TZ, timestamp)
		(FB_INTL_VARCHAR(20 * 4, CS_UTF8), requestType)
		(FB_INTL_VARCHAR(METADATA_IDENTIFIER_CHAR_LEN * 4, CS_UTF8), packageName)
		(FB_INTL_VARCHAR(METADATA_IDENTIFIER_CHAR_LEN * 4, CS_UTF8), routineName)
		(FB_BLOB, sqlText)
	) requestMessage(&throwStatus, fb_get_master_interface());
	requestMessage.clear();

	const char* recSrcStatsSql = R"""(
		update or insert into rdb$profile_record_source_stats
			(rdb$profile_session_id, rdb$profile_request_id, rdb$cursor_id, rdb$record_source_id,
			 rdb$parent_record_source_id, rdb$access_path,
			 rdb$open_counter, rdb$open_min_time, rdb$open_max_time, rdb$open_total_time,
			 rdb$fetch_counter, rdb$fetch_min_time, rdb$fetch_max_time, rdb$fetch_total_time)
			values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
			matching (rdb$profile_session_id, rdb$profile_request_id, rdb$cursor_id, rdb$record_source_id)
	)""";

	FB_MESSAGE(RecSrcStatsMessage, ThrowWrapper,
		(FB_BIGINT, sessionId)
		(FB_BIGINT, requestId)
		(FB_INTEGER, cursorId)
		(FB_INTEGER, recordSourceId)
		(FB_BIGINT, parentRecordSourceId)
		(FB_INTL_VARCHAR(255 * 4, CS_UTF8), accessPath)
		(FB_BIGINT, openCounter)
		(FB_BIGINT, openMinTime)
		(FB_BIGINT, openMaxTime)
		(FB_BIGINT, openTotalTime)
		(FB_BIGINT, fetchCounter)
		(FB_BIGINT, fetchMinTime)
		(FB_BIGINT, fetchMaxTime)
		(FB_BIGINT, fetchTotalTime)
	) recSrcStatsMessage(&throwStatus, fb_get_master_interface());
	recSrcStatsMessage.clear();

	const char* statsSql = R"""(
		update or insert into rdb$profile_stats
			(rdb$profile_session_id, rdb$profile_request_id, rdb$line, rdb$column,
			 rdb$counter, rdb$min_time, rdb$max_time, rdb$total_time)
			values (?, ?, ?, ?, ?, ?, ?, ?)
			matching (rdb$profile_session_id, rdb$profile_request_id, rdb$line, rdb$column)
	)""";

	FB_MESSAGE(StatsMessage, ThrowWrapper,
		(FB_BIGINT, sessionId)
		(FB_BIGINT, requestId)
		(FB_INTEGER, line)
		(FB_INTEGER, column)
		(FB_BIGINT, counter)
		(FB_BIGINT, minTime)
		(FB_BIGINT, maxTime)
		(FB_BIGINT, totalTime)
	) statsMessage(&throwStatus, fb_get_master_interface());
	statsMessage.clear();

	auto sessionStmt = makeNoIncRef(attachment->prepare(
		&throwStatus, transaction, 0, sessionSql, SQL_DIALECT_CURRENT, 0));
	auto requestStmt = makeNoIncRef(attachment->prepare(
		&throwStatus, transaction, 0, requestSql, SQL_DIALECT_CURRENT, 0));
	auto recSrcStatsStmt = makeNoIncRef(attachment->prepare(
		&throwStatus, transaction, 0, recSrcStatsSql, SQL_DIALECT_CURRENT, 0));
	auto statsStmt = makeNoIncRef(attachment->prepare(
		&throwStatus, transaction, 0, statsSql, SQL_DIALECT_CURRENT, 0));

	auto sessionAccessor = sessions.accessor();

	for (bool sessionFound = sessionAccessor.getFirst(); sessionFound;)
	{
		sessionMessage->sessionIdNull = FB_FALSE;
		sessionMessage->sessionId = sessionAccessor.current()->first;

		sessionMessage->attachmentIdNull = FB_FALSE;
		sessionMessage->attachmentId = tdbb->getAttachment()->att_attachment_id;

		sessionMessage->userNull = FB_FALSE;
		sessionMessage->user.set(tdbb->getAttachment()->att_user->getUserName().c_str());

		sessionMessage->descriptionNull = sessionAccessor.current()->second.description.isEmpty();
		sessionMessage->description.set(sessionAccessor.current()->second.description.c_str());

		sessionMessage->startTimestampNull = FB_FALSE;
		sessionMessage->startTimestamp = sessionAccessor.current()->second.startTimeStamp;

		sessionMessage->finishTimestampNull = sessionAccessor.current()->second.finishTimeStamp.isUnknown();
		sessionMessage->finishTimestamp = sessionAccessor.current()->second.finishTimeStamp.value;

		sessionStmt->execute(&throwStatus, transaction, sessionMessage.getMetadata(),
			sessionMessage.getData(), nullptr, nullptr);

		for (const auto& requestIt : sessionAccessor.current()->second.requests)
		{
			const auto& profileRequest = requestIt.second;

			requestMessage->sessionIdNull = FB_FALSE;
			requestMessage->sessionId = sessionMessage->sessionId;

			requestMessage->requestIdNull = FB_FALSE;
			requestMessage->requestId = requestIt.first;

			requestMessage->timestampNull = FB_FALSE;
			requestMessage->timestamp = profileRequest.timeStamp;

			requestMessage->requestTypeNull = FB_FALSE;
			requestMessage->requestType.set(profileRequest.requestType.c_str());

			requestMessage->packageNameNull = profileRequest.packageName.isEmpty();
			requestMessage->packageName.set(profileRequest.packageName.c_str());

			requestMessage->routineNameNull = profileRequest.routineName.isEmpty();
			requestMessage->routineName.set(profileRequest.routineName.c_str());

			requestMessage->sqlTextNull = profileRequest.sqlText.isEmpty();

			if (profileRequest.sqlText.hasData())
			{
				const auto blob = attachment->createBlob(
					&throwStatus, transaction, &requestMessage->sqlText, sizeof(TEMP_BPB), TEMP_BPB);
				blob->putSegment(&throwStatus, profileRequest.sqlText.length(), profileRequest.sqlText.c_str());
				blob->close(&throwStatus);
			}

			requestStmt->execute(&throwStatus, transaction, requestMessage.getMetadata(),
				requestMessage.getData(), nullptr, nullptr);

			for (const auto& cursorIt : profileRequest.cursors)
			{
				const auto& profileCursor = cursorIt.second;

				recSrcStatsMessage->sessionIdNull = FB_FALSE;
				recSrcStatsMessage->sessionId = sessionMessage->sessionId;

				recSrcStatsMessage->requestIdNull = FB_FALSE;
				recSrcStatsMessage->requestId = requestIt.first;

				recSrcStatsMessage->cursorIdNull = FB_FALSE;
				recSrcStatsMessage->cursorId = cursorIt.first;

				for (const auto& sourceIt : profileCursor.sources)
				{
					const auto& recSrc = sourceIt.second;

					// Use sequence instead of the internal ID.
					recSrcStatsMessage->recordSourceIdNull = FB_FALSE;
					recSrcStatsMessage->recordSourceId = recSrc.sequence;

					// Use parent sequence instead of the internal parent ID.
					recSrcStatsMessage->parentRecordSourceIdNull = !recSrc.parentSequence.specified;
					recSrcStatsMessage->parentRecordSourceId = recSrc.parentSequence.value;

					recSrcStatsMessage->accessPathNull = recSrc.accessPath.isEmpty();
					recSrcStatsMessage->accessPath.set(recSrc.accessPath.c_str());

					recSrcStatsMessage->openCounterNull = FB_FALSE;
					recSrcStatsMessage->openCounter = recSrc.openStats.counter;

					recSrcStatsMessage->openMinTimeNull = FB_FALSE;
					recSrcStatsMessage->openMinTime = recSrc.openStats.minTime;

					recSrcStatsMessage->openMaxTimeNull = FB_FALSE;
					recSrcStatsMessage->openMaxTime = recSrc.openStats.maxTime;

					recSrcStatsMessage->openTotalTimeNull = FB_FALSE;
					recSrcStatsMessage->openTotalTime = recSrc.openStats.totalTime;

					recSrcStatsMessage->fetchCounterNull = FB_FALSE;
					recSrcStatsMessage->fetchCounter = recSrc.fetchStats.counter;

					recSrcStatsMessage->fetchMinTimeNull = FB_FALSE;
					recSrcStatsMessage->fetchMinTime = recSrc.fetchStats.minTime;

					recSrcStatsMessage->fetchMaxTimeNull = FB_FALSE;
					recSrcStatsMessage->fetchMaxTime = recSrc.fetchStats.maxTime;

					recSrcStatsMessage->fetchTotalTimeNull = FB_FALSE;
					recSrcStatsMessage->fetchTotalTime = recSrc.fetchStats.totalTime;

					recSrcStatsStmt->execute(&throwStatus, transaction, recSrcStatsMessage.getMetadata(),
						recSrcStatsMessage.getData(), nullptr, nullptr);
				}
			}

			for (const auto& statsIt : profileRequest.stats)
			{
				const auto lineColumn = Profiler::decodeLineColumn(statsIt.first);

				statsMessage->sessionIdNull = FB_FALSE;
				statsMessage->sessionId = sessionMessage->sessionId;

				statsMessage->requestIdNull = FB_FALSE;
				statsMessage->requestId = requestIt.first;

				statsMessage->lineNull = FB_FALSE;
				statsMessage->line = lineColumn.first;

				statsMessage->columnNull = FB_FALSE;
				statsMessage->column = lineColumn.second;

				statsMessage->counterNull = FB_FALSE;
				statsMessage->counter = statsIt.second.counter;

				statsMessage->minTimeNull = FB_FALSE;
				statsMessage->minTime = statsIt.second.minTime;

				statsMessage->maxTimeNull = FB_FALSE;
				statsMessage->maxTime = statsIt.second.maxTime;

				statsMessage->totalTimeNull = FB_FALSE;
				statsMessage->totalTime = statsIt.second.totalTime;

				statsStmt->execute(&throwStatus, transaction, statsMessage.getMetadata(),
					statsMessage.getData(), nullptr, nullptr);
			}
		}

		if (sessionAccessor.current()->first != currentSessionId)
			sessionFound = sessionAccessor.fastRemove();
		else
			sessionFound = sessionAccessor.getNext();
	}
}

Profiler::Request* Profiler::getRequest(jrd_req* request)
{
	const auto profileSession = sessions.get(currentSessionId);
	fb_assert(profileSession);

	auto profileRequest = profileSession->requests.get(request->getRequestId());

	if (!profileRequest)
	{
		profileRequest = profileSession->requests.put(request->getRequestId());
		profileRequest->init(request);

		if (request->getStatement()->getRoutine())
		{
			if (request->getStatement()->procedure)
				profileRequest->requestType = "PROCEDURE";
			else if (request->getStatement()->function)
				profileRequest->requestType = "FUNCTION";

			profileRequest->packageName =
				request->getStatement()->getRoutine()->getName().package;

			profileRequest->routineName =
				request->getStatement()->getRoutine()->getName().identifier;
		}
		else if (request->getStatement()->triggerName.hasData())
		{
			profileRequest->requestType = "TRIGGER";
			profileRequest->routineName = request->getStatement()->triggerName;
		}
		else
			profileRequest->requestType = "BLOCK";
	}

	return profileRequest;
}

//--------------------------------------


ProfilerPackage::ProfilerPackage(MemoryPool& pool)
	: SystemPackage(
		pool,
		"RDB$PROFILER",
		ODS_13_0,	//// TODO: adjust
		// procedures
		{
			SystemProcedure(
				pool,
				"FINISH_SESSION",
				SystemProcedureFactory<FinishSessionInput, VoidMessage, finishSessionProcedure>(),
				prc_executable,
				// input parameters
				{
					{"REFRESH_SNAPSHOTS", fld_bool, false}
				},
				// output parameters
				{
				}
			),
			SystemProcedure(
				pool,
				"REFRESH_SNAPSHOTS",
				SystemProcedureFactory<VoidMessage, VoidMessage, refreshSnapshotsProcedure>(),
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
					{"REFRESH_SNAPSHOTS", fld_bool, false}
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
					{"DESCRIPTION", fld_short_description, true}
				},
				{fld_prof_ses_id, false}
			)
		}
	)
{
}
