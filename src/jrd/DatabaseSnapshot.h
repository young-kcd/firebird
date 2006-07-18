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

namespace Jrd {

class DatabaseSnapshot  {
	struct RelationData {
		int rel_id;
		RecordBuffer* data;
	};

public:
	~DatabaseSnapshot();

	RecordBuffer* getData(const jrd_rel*) const;

	static DatabaseSnapshot* create(thread_db*);

protected:
	DatabaseSnapshot(thread_db*, MemoryPool&, jrd_tra*);

private:
	jrd_tra* transaction;
	Firebird::Array<RelationData> snapshot;

	RecordBuffer* allocBuffer(thread_db*, MemoryPool&, int);

	void clearRecord(Record*);

	void putField(Record*, int, const void*);

	void putDatabase(Database*, RecordBuffer*);
	void putAttachment(Attachment*, RecordBuffer*);
	void putTransaction(jrd_tra*, RecordBuffer*);
	void putRequest(jrd_req*, RecordBuffer*);
};

} // namespace

#endif // JRD_DATABASE_SNAPSHOT_H
