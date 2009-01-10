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

#ifndef JRD_DATABASE_SNAPSHOT_H
#define JRD_DATABASE_SNAPSHOT_H

#include "../common/classes/array.h"
#include "../common/classes/init.h"
#include "../common/classes/ClumpletReader.h"
#include "../common/classes/ClumpletWriter.h"

namespace Jrd {

class DatabaseSnapshot
{
	struct RelationData
	{
		int rel_id;
		RecordBuffer* data;
	};

	class SharedMemory
	{
		static const ULONG VERSION;
		static const ULONG DEFAULT_SIZE;

		struct Header
		{
			ULONG version;
			ULONG used;
			ULONG allocated;
#ifndef WIN_NT
			struct mtx mutex;
#endif
		};

		class DumpGuard
		{
		public:
			explicit DumpGuard(SharedMemory* ptr)
				: dump(ptr)
			{
				dump->acquire();
			}

			~DumpGuard()
			{
				dump->release();
			}

		private:
			DumpGuard(const DumpGuard&);
			DumpGuard& operator=(const DumpGuard&);

			SharedMemory* dump;
		};

	public:
		SharedMemory();
		~SharedMemory();

		void acquire();
		void release();

		UCHAR* readData(thread_db*, MemoryPool&, ULONG&);
		void writeData(thread_db*, ULONG, const UCHAR*);

		void cleanup(thread_db*);

	private:
		// copying is prohibited
		SharedMemory(const SharedMemory&);
		SharedMemory& operator =(const SharedMemory&);

		void doCleanup(const Database* const dbb);
		void extend();

		static void checkMutex(const TEXT*, int);
		static void init(void*, SH_MEM_T*, bool);

		SH_MEM_T handle;
#ifdef WIN_NT
		struct mtx mutex;
#endif
		Header* base;
	};

	struct Element
	{
		ULONG processId;
		SLONG localId;
		ULONG length;
	};

public:
	~DatabaseSnapshot();

	RecordBuffer* getData(const jrd_rel*) const;

	static DatabaseSnapshot* create(thread_db*);
	static void cleanup(thread_db*);
	static int blockingAst(void*);

	static void init()
	{
		dump = FB_NEW(*getDefaultMemoryPool()) SharedMemory;
	}

protected:
	DatabaseSnapshot(thread_db*, MemoryPool&);

private:
	RecordBuffer* allocBuffer(thread_db*, MemoryPool&, int);
	void clearRecord(Record*);
	void putField(Record*, int, const Firebird::ClumpletReader&, bool);

	static void dumpData(thread_db*);
	static const char* checkNull(int, int, const char*, size_t);

	static SINT64 getGlobalId(int);

	static void putDatabase(const Database*, Firebird::ClumpletWriter&, int);
	static void putAttachment(const Attachment*, Firebird::ClumpletWriter&, int);
	static void putTransaction(const jrd_tra*, Firebird::ClumpletWriter&, int);
	static void putRequest(const jrd_req*, Firebird::ClumpletWriter&, int);
	static void putCall(const jrd_req*, Firebird::ClumpletWriter&, int);
	static void putStatistics(const RuntimeStatistics&, Firebird::ClumpletWriter&, int, int);
	static void putContextVars(Firebird::StringMap&, Firebird::ClumpletWriter&, int, bool);
	static void putMemoryUsage(const Firebird::MemoryStats&, Firebird::ClumpletWriter&, int, int);

	static SharedMemory* dump;
	static Firebird::InitMutex<DatabaseSnapshot> startup;

	Firebird::Array<RelationData> snapshot;
	Firebird::GenericMap<Firebird::Pair<Firebird::NonPooled<SINT64, SLONG> > > idMap;
	int idCounter;
};

} // namespace

#endif // JRD_DATABASE_SNAPSHOT_H
