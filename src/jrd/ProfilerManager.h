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

#ifndef JRD_PROFILER_MANAGER_H
#define JRD_PROFILER_MANAGER_H

#include "firebird.h"
#include "firebird/Message.h"
#include "../common/classes/auto.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/Nullable.h"
#include "../jrd/Monitoring.h"
#include "../jrd/SystemPackages.h"

namespace Jrd {

class Attachment;
class jrd_req;
class thread_db;
class Profiler;


class ProfilerManager final
{
	friend class ProfilerPackage;

private:
	class Statement final
	{
	public:
		Statement(MemoryPool& pool)
			: cursorNextSequence(pool),
			  recSourceSequence(pool)
		{
		}

		Statement(const Statement&) = delete;
		void operator=(const Statement&) = delete;

		SINT64 id = 0;
		Firebird::NonPooledMap<ULONG, ULONG> cursorNextSequence;
		Firebird::NonPooledMap<ULONG, ULONG> recSourceSequence;
	};

	class Session final
	{
	public:
		Session(MemoryPool& pool)
			: statements(pool),
			  requests(pool)
		{
		}

		Session(const Session&) = delete;
		void operator=(const Session&) = delete;

		Firebird::AutoPlugin<Firebird::IProfilerPlugin> plugin;
		Firebird::AutoDispose<Firebird::IProfilerSession> pluginSession;
		Firebird::RightPooledMap<StmtNumber, Statement> statements;
		Firebird::SortedArray<StmtNumber> requests;
		unsigned flags = 0;
	};

private:
	ProfilerManager(thread_db* tdbb);

public:
	static ProfilerManager* create(thread_db* tdbb);

	ProfilerManager(const ProfilerManager&) = delete;
	void operator=(const ProfilerManager&) = delete;

public:
	SINT64 startSession(thread_db* tdbb, const Firebird::PathName& pluginName, const Firebird::string& description);
	void prepareRecSource(thread_db* tdbb, jrd_req* request, const RecordSource* rsb);
	void onRequestFinish(jrd_req* request);
	void beforePsqlLineColumn(jrd_req* request, ULONG line, ULONG column);
	void afterPsqlLineColumn(jrd_req* request, ULONG line, ULONG column, FB_UINT64 runTime);
	void beforeRecordSourceOpen(jrd_req* request, const RecordSource* rsb);
	void afterRecordSourceOpen(jrd_req* request, const RecordSource* rsb, FB_UINT64 runTime);
	void beforeRecordSourceGetRecord(jrd_req* request, const RecordSource* rsb);
	void afterRecordSourceGetRecord(jrd_req* request, const RecordSource* rsb, FB_UINT64 runTime);

	bool isActive() const
	{
		return currentSession && !paused;
	}

private:
	void flush(Firebird::ITransaction* transaction);
	Statement* getStatement(jrd_req* request);
	SINT64 getRequest(jrd_req* request, unsigned flags);

private:
	Firebird::LeftPooledMap<Firebird::PathName, Firebird::AutoPlugin<Firebird::IProfilerPlugin>> activePlugins;
	Firebird::AutoPtr<Session> currentSession;
	bool paused = false;
};


class ProfilerPackage : public SystemPackage
{
public:
	ProfilerPackage(Firebird::MemoryPool& pool);

private:
	static Firebird::IExternalResultSet* flushProcedure(Firebird::ThrowStatusExceptionWrapper* status,
		Firebird::IExternalContext* context, const void* in, void* out);

	//----------

	static Firebird::IExternalResultSet* cancelSessionProcedure(Firebird::ThrowStatusExceptionWrapper* status,
		Firebird::IExternalContext* context, const void* in, void* out);

	//----------

	FB_MESSAGE(FinishSessionInput, Firebird::ThrowStatusExceptionWrapper,
		(FB_BOOLEAN, flush)
	);

	static Firebird::IExternalResultSet* finishSessionProcedure(Firebird::ThrowStatusExceptionWrapper* status,
		Firebird::IExternalContext* context, const FinishSessionInput::Type* in, void* out);

	//----------

	FB_MESSAGE(PauseSessionInput, Firebird::ThrowStatusExceptionWrapper,
		(FB_BOOLEAN, flush)
	);

	static Firebird::IExternalResultSet* pauseSessionProcedure(Firebird::ThrowStatusExceptionWrapper* status,
		Firebird::IExternalContext* context, const PauseSessionInput::Type* in, void* out);

	//----------

	static Firebird::IExternalResultSet* resumeSessionProcedure(Firebird::ThrowStatusExceptionWrapper* status,
		Firebird::IExternalContext* context, const void* in, void* out);

	//----------

	FB_MESSAGE(StartSessionInput, Firebird::ThrowStatusExceptionWrapper,
		(FB_INTL_VARCHAR(255, CS_METADATA), pluginName)
		//// TODO: Options: PSQL, SQL.
		(FB_INTL_VARCHAR(255, CS_METADATA), description)
		//// TODO: Plugin options.
	);

	FB_MESSAGE(StartSessionOutput, Firebird::ThrowStatusExceptionWrapper,
		(FB_BIGINT, sessionId)
	);

	static void startSessionFunction(Firebird::ThrowStatusExceptionWrapper* status,
		Firebird::IExternalContext* context, const StartSessionInput::Type* in, StartSessionOutput::Type* out);
};


}	// namespace

#endif	// JRD_PROFILER_MANAGER_H
