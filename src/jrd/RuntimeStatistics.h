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

#ifndef JRD_RUNTIME_STATISTICS_H
#define JRD_RUNTIME_STATISTICS_H

#include "../common/classes/alloc.h"
#include "../common/classes/array.h"

namespace Jrd {

class thread_db;

// Multi-level runtime statistics class

class RuntimeStatistics {
public:
	enum {
		PAGE_FETCHES = 0,
		PAGE_READS,
		PAGE_MARKS,
		PAGE_WRITES,
		FLUSHES,
		RECORD_SEQ_READS,
		RECORD_IDX_READS,
		RECORD_INSERTS,
		RECORD_UPDATES,
		RECORD_DELETES,
		RECORD_BACKOUTS,
		RECORD_PURGES,
		RECORD_EXPUNGES,
		SORTS,
		SORT_GETS,
		SORT_PUTS,
		STMT_PREPARES,
		STMT_EXECUTES,
		TOTAL_ITEMS
	};

	explicit RuntimeStatistics(Firebird::MemoryPool&, RuntimeStatistics* = NULL);
	~RuntimeStatistics() {}

	void setParent(RuntimeStatistics*);
	void bumpValue(size_t);
	SINT64 getValue(size_t) const;

	static void bumpValue(thread_db*, size_t);

private:
	// copying is prohibited
	RuntimeStatistics(const RuntimeStatistics&);
	RuntimeStatistics& operator= (const RuntimeStatistics&);

	RuntimeStatistics* parent;
	Firebird::Array<SINT64> values;
};

} // namespace

#endif // JRD_RUNTIME_STATISTICS_H
