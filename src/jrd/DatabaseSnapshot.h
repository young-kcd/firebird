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
 *		Alex Peshkoff, 2010 - divided into class DataDump and the remaining part of DatabaseSnapshot
 */

#ifndef JRD_DATABASE_SNAPSHOT_H
#define JRD_DATABASE_SNAPSHOT_H

#include "../common/classes/array.h"
#include "../common/classes/init.h"
#include "../common/isc_s_proto.h"
#include "../common/classes/timestamp.h"
#include "../jrd/val.h"
#include "../jrd/recsrc/RecordSource.h"

namespace Jrd {

// forward declarations
class jrd_rel;
class Record;
class RecordBuffer;
class RuntimeStatistics;

class DataDump
{
public:
	enum ValueType {VALUE_GLOBAL_ID, VALUE_INTEGER, VALUE_TIMESTAMP, VALUE_STRING};

	explicit DataDump(MemoryPool& pool)
		: idMap(pool), idCounter(0) { }

	struct DumpField
	{
		DumpField(USHORT p_id, ValueType p_type, USHORT p_length, const void* p_data)
			: id(p_id), type(p_type), length(p_length), data(p_data) { }
		DumpField()
			: id(0), type(VALUE_GLOBAL_ID), length(0), data(NULL) { }

		USHORT id;
		ValueType type;
		USHORT length;
		const void* data;
	};

	class DumpRecord
	{
	public:
		DumpRecord() : offset(0), sizeLimit(0)
		{}

		explicit DumpRecord(int rel_id)
		{
			reset(rel_id);
		}

		void reset(int rel_id)
		{
			offset = 0;
			sizeLimit = sizeof(buffer);
			fb_assert(rel_id > 0 && rel_id <= int(MAX_UCHAR));
			buffer[offset++] = (UCHAR) rel_id;
		}

		void assign(USHORT length, const UCHAR* ptr)
		{
			offset = 0;
			sizeLimit = length;
			memcpy(buffer, ptr, length);
		}

		ULONG getLength() const
		{
			return offset;
		}

		const UCHAR* getData() const
		{
			return buffer;
		}

		void storeGlobalId(int field_id, SINT64 value)
		{
			storeField(field_id, VALUE_GLOBAL_ID, sizeof(SINT64), &value);
		}

		void storeInteger(int field_id, SINT64 value)
		{
			storeField(field_id, VALUE_INTEGER, sizeof(SINT64), &value);
		}

		void storeTimestamp(int field_id, const Firebird::TimeStamp& value)
		{
			if (!value.isEmpty())
			{
				storeField(field_id, VALUE_TIMESTAMP, sizeof(ISC_TIMESTAMP), &value.value());
			}
		}

		void storeString(int field_id, const Firebird::string& value)
		{
			if (value.length())
			{
				storeField(field_id, VALUE_STRING, value.length(), value.c_str());
			}
		}

		void storeString(int field_id, const Firebird::PathName& value)
		{
			if (value.length())
			{
				storeField(field_id, VALUE_STRING, value.length(), value.c_str());
			}
		}

		void storeString(int field_id, const Firebird::MetaName& value)
		{
			if (value.length())
			{
				storeField(field_id, VALUE_STRING, value.length(), value.c_str());
			}
		}

		int getRelationId()
		{
			fb_assert(!offset);
			return (ULONG) buffer[offset++];
		}

		bool getField(DumpField& field)
		{
			fb_assert(offset);

			if (offset < sizeLimit)
			{
				field.id = (USHORT) buffer[offset++];
				field.type = (ValueType) buffer[offset++];
				fb_assert(field.type >= VALUE_GLOBAL_ID && field.type <= VALUE_STRING);
				memcpy(&field.length, buffer + offset, sizeof(USHORT));
				offset += sizeof(USHORT);
				field.data = buffer + offset;
				offset += field.length;
				return true;
			}

			return false;
		}

	private:
		void storeField(int field_id, ValueType type, size_t length, const void* value)
		{
			const size_t delta = sizeof(UCHAR) + sizeof(UCHAR) + sizeof(USHORT) + length;

			if (offset + delta > sizeLimit)
			{
				fb_assert(false);
				return;
			}

			UCHAR* ptr = buffer + offset;
			fb_assert(field_id <= int(MAX_UCHAR));
			*ptr++ = (UCHAR) field_id;
			*ptr++ = (UCHAR) type;
			const USHORT adjusted_length = (USHORT) length;
			memcpy(ptr, &adjusted_length, sizeof(adjusted_length));
			ptr += sizeof(USHORT);
			memcpy(ptr, value, length);
			offset += (ULONG) delta;
		}

		UCHAR buffer[MAX_RECORD_SIZE];
		ULONG offset;
		ULONG sizeLimit;
	};

	void clearRecord(Record*);
	void putField(thread_db*, Record*, const DumpField&, int&, bool = false);

private:
	Firebird::GenericMap<Firebird::Pair<Firebird::NonPooled<SINT64, SLONG> > > idMap;
	int idCounter;
};


class MonitoringHeader : public MemoryHeader
{
public:
	ULONG used;
	ULONG allocated;
};

class MonitoringData : public SharedMemory<MonitoringHeader>
{
	static const ULONG MONITOR_VERSION = 3;
	static const ULONG DEFAULT_SIZE = 1048576;

	typedef MonitoringHeader Header;

	struct Element
	{
		SLONG processId;
		SLONG localId;
		ULONG length;
	};

	static ULONG alignOffset(ULONG absoluteOffset);

public:
	class Guard
	{
	public:
		explicit Guard(MonitoringData* ptr)
			: data(ptr)
		{
			data->acquire();
		}

		~Guard()
		{
			data->release();
		}

	private:
		Guard(const Guard&);
		Guard& operator=(const Guard&);

		MonitoringData* const data;
	};

	explicit MonitoringData(const Database*);
	~MonitoringData();

	bool initialize(bool);
	void mutexBug(int osErrorCode, const char* text);

	void acquire();
	void release();

	UCHAR* read(MemoryPool&, ULONG&);
	ULONG setup();
	void write(ULONG, ULONG, const void*);

	void cleanup();

private:
	// copying is prohibited
	MonitoringData(const MonitoringData&);
	MonitoringData& operator =(const MonitoringData&);

	void ensureSpace(ULONG);

	const SLONG process_id;
	const SLONG local_id;
};


class MonitoringTableScan: public VirtualTableScan
{
public:
	MonitoringTableScan(CompilerScratch* csb, const Firebird::string& name, UCHAR stream)
		: VirtualTableScan(csb, name, stream)
	{}

	bool retrieveRecord(thread_db* tdbb, jrd_rel* relation, FB_UINT64 position, Record* record) const;
};


class DatabaseSnapshot : public DataDump
{
	struct RelationData
	{
		int rel_id;
		RecordBuffer* data;
	};

private:
	class Writer
	{
	public:
		explicit Writer(MonitoringData* data)
			: dump(data)
		{
			fb_assert(dump);
			offset = dump->setup();
			fb_assert(offset);
		}

		void putRecord(const DumpRecord& record)
		{
			const USHORT length = (USHORT) record.getLength();
			dump->write(offset, sizeof(USHORT), &length);
			dump->write(offset, length, record.getData());
		}

	private:
		MonitoringData* dump;
		ULONG offset;
	};

	class Reader
	{
	public:
		Reader(ULONG length, UCHAR* ptr)
			: sizeLimit(length), buffer(ptr), offset(0)
		{}

		bool getRecord(DumpRecord& record)
		{
			if (offset < sizeLimit)
			{
				USHORT length;
				memcpy(&length, buffer + offset, sizeof(USHORT));
				offset += sizeof(USHORT);
				record.assign(length, buffer + offset);
				offset += length;
				return true;
			}

			return false;
		}

	private:
		ULONG sizeLimit;
		const UCHAR* buffer;
		ULONG offset;
	};

public:
	~DatabaseSnapshot();

	RecordBuffer* getData(const jrd_rel*) const;

	static DatabaseSnapshot* create(thread_db*);
	static int blockingAst(void*);
	static bool getRecord(thread_db* tdbb, jrd_rel* relation, FB_UINT64 position, Record* record);

protected:
	DatabaseSnapshot(thread_db*, MemoryPool&);

private:
	RecordBuffer* allocBuffer(thread_db*, MemoryPool&, int);

	static void dumpData(thread_db*);

	static SINT64 getGlobalId(int);

	static void putDatabase(const Database*, Writer&, int);
	static bool putAttachment(thread_db*, const Attachment*, Writer&, int);
	static void putTransaction(const jrd_tra*, Writer&, int);
	static void putRequest(const jrd_req*, Writer&, int);
	static void putCall(const jrd_req*, Writer&, int);
	static void putStatistics(const RuntimeStatistics&, Writer&, int, int);
	static void putContextVars(const Firebird::StringMap&, Writer&, int, bool);
	static void putMemoryUsage(const Firebird::MemoryStats&, Writer&, int, int);

	Firebird::Array<RelationData> snapshot;
};

} // namespace

#endif // JRD_DATABASE_SNAPSHOT_H
