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
#include "../common/classes/ClumpletReader.h"
#include "../common/classes/ClumpletWriter.h"

namespace Jrd {

class DatabaseSnapshot {
	struct RelationData {
		int rel_id;
		RecordBuffer* data;
	};

	class SharedMemory {
		static const size_t VERSION;
		static const size_t DEFAULT_SIZE;
		static const size_t SEMAPHORES;

		struct Header {
			size_t version;
			size_t length;
	#ifndef WIN_NT
			MTX_T mutex;
	#endif
		};

	public:
		SharedMemory();
		~SharedMemory();

		void acquire();
		void release();

		Firebird::ClumpletReader* readData(thread_db*);
		void writeData(thread_db*, Firebird::ClumpletWriter&);

	private:
		// copying is prohibited
		SharedMemory(const SharedMemory&);
		SharedMemory& operator =(const SharedMemory&);

		void garbageCollect(thread_db*, bool);
		void extend(size_t);

		static void checkMutex(const TEXT*, int);
		static void init(void*, SH_MEM_T*, bool);

		SH_MEM_T handle;
	#ifdef WIN_NT
		MTX_T mutex;
	#endif
		Header* base;
	};

public:
	~DatabaseSnapshot();

	RecordBuffer* getData(const jrd_rel*) const;

	static DatabaseSnapshot* create(thread_db*);
	static int blockingAst(void*);

protected:
	DatabaseSnapshot(thread_db*, MemoryPool&, jrd_tra*);

private:
	RecordBuffer* allocBuffer(thread_db*, MemoryPool&, int);
	void clearRecord(Record*);
	void putField(Record*, int, const void*, size_t);

	static Firebird::ClumpletReader* dumpData(thread_db*, bool);
	static const char* checkNull(int, int, const char*, size_t);

	static void putDatabase(Database*, Firebird::ClumpletWriter&);
	static void putAttachment(Attachment*, Firebird::ClumpletWriter&);
	static void putTransaction(jrd_tra*, Firebird::ClumpletWriter&);
	static void putRequest(jrd_req*, Firebird::ClumpletWriter&);

	static Firebird::Mutex initMutex;
	static SharedMemory* dump;

	jrd_tra* transaction;
	Firebird::Array<RelationData> snapshot;
};

} // namespace

#endif // JRD_DATABASE_SNAPSHOT_H
