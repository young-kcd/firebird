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
 *  Copyright (c) 2009 Dmitry Yemanov <dimitr@firebirdsql.org>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef JRD_RECORD_SOURCE_H
#define JRD_RECORD_SOURCE_H

#include "../common/classes/array.h"
#include "../common/classes/objects_array.h"
#include "../common/classes/NestConst.h"
#include "../jrd/RecordSourceNodes.h"
#include "../jrd/req.h"
#include "../jrd/rse.h"
#include "../jrd/inf_pub.h"

namespace Jrd
{
	class thread_db;
	class jrd_req;
	class jrd_nod;
	class jrd_prc;
	class AggNode;
	class Sort;
	class CompilerScratch;
	class RecordBuffer;
	class BtrPageGCLock;
	struct index_desc;
	struct record_param;
	struct temporary_key;
	struct win;
	class BaseBufferedStream;
	class BufferedStream;

	// Abstract base class

	class RecordSource
	{
	public:
		virtual void open(thread_db* tdbb) const = 0;
		virtual void close(thread_db* tdbb) const = 0;

		virtual bool getRecord(thread_db* tdbb) const = 0;
		virtual bool refetchRecord(thread_db* tdbb) const = 0;
		virtual bool lockRecord(thread_db* tdbb) const = 0;

		virtual void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const = 0;

		virtual void markRecursive() = 0;
		virtual void invalidateRecords(jrd_req* request) const = 0;

		virtual void findUsedStreams(StreamsArray& streams) const = 0;
		virtual void nullRecords(thread_db* tdbb) const = 0;
		virtual void saveRecords(thread_db* tdbb) const = 0;
		virtual void restoreRecords(thread_db* tdbb) const = 0;

		virtual ~RecordSource();

		static bool rejectDuplicate(const UCHAR* /*data1*/, const UCHAR* /*data2*/, void* /*userArg*/)
		{
			return true;
		}

	protected:
		// Generic impure block
		struct Impure
		{
			ULONG irsb_flags;
		};

		static const ULONG irsb_open = 1;
		static const ULONG irsb_first = 2;
		static const ULONG irsb_joined = 4;
		static const ULONG irsb_mustread = 8;
		static const ULONG irsb_singular_processed = 16;

		RecordSource()
			: m_impure(0), m_recursive(false)
		{}

		static void dumpName(thread_db* tdbb, const Firebird::string& name,
			Firebird::UCharBuffer& buffer);
		static void dumpInversion(thread_db* tdbb, const InversionNode* inversion,
			Firebird::UCharBuffer& buffer);

		static void saveRecord(thread_db* tdbb, record_param* rpb);
		static void restoreRecord(thread_db* tdbb, record_param* rpb);

		ULONG m_impure;
		bool m_recursive;
	};


	// Helper class implementing some common methods

	class RecordStream : public RecordSource
	{
	public:
		RecordStream(CompilerScratch* csb, UCHAR stream, const Format* format = NULL);

		virtual bool refetchRecord(thread_db* tdbb) const;
		virtual bool lockRecord(thread_db* tdbb) const;

		virtual void markRecursive();
		virtual void invalidateRecords(jrd_req* request) const;

		virtual void findUsedStreams(StreamsArray& streams) const;
		virtual void nullRecords(thread_db* tdbb) const;
		virtual void saveRecords(thread_db* tdbb) const;
		virtual void restoreRecords(thread_db* tdbb) const;

	protected:
		const UCHAR m_stream;
		const Format* const m_format;
	};


	// Primary (table scan) access methods

	class FullTableScan : public RecordStream
	{
	public:
		FullTableScan(CompilerScratch* csb, const Firebird::string& name, UCHAR stream);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

	private:
		const Firebird::string m_name;
	};

	class BitmapTableScan : public RecordStream
	{
		struct Impure : public RecordSource::Impure
		{
			RecordBitmap** irsb_bitmap;
		};

	public:
		BitmapTableScan(CompilerScratch* csb, const Firebird::string& name, UCHAR stream,
			InversionNode* inversion);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

	private:
		const Firebird::string m_name;
		NestConst<InversionNode> const m_inversion;
	};

	class IndexTableScan : public RecordStream
	{
		struct Impure : public RecordSource::Impure
		{
			RecordNumber irsb_nav_number;				// last record number
			SLONG irsb_nav_page;						// index page number
			SLONG irsb_nav_incarnation;					// buffer/page incarnation counter
			RecordBitmap** irsb_nav_bitmap;				// bitmap for inversion tree
			RecordBitmap* irsb_nav_records_visited;		// bitmap of records already retrieved
			BtrPageGCLock* irsb_nav_btr_gc_lock;		// lock to prevent removal of currently walked index page
			USHORT irsb_nav_offset;						// page offset of current index node
			USHORT irsb_nav_lower_length;				// length of lower key value
			USHORT irsb_nav_upper_length;				// length of upper key value
			USHORT irsb_nav_length;						// length of expanded key
			UCHAR irsb_nav_data[1];						// expanded key, upper bound, and index desc
		};

	public:
		IndexTableScan(CompilerScratch* csb, const Firebird::string& name, UCHAR stream,
			InversionNode* index, USHORT keyLength);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

		void setInversion(InversionNode* inversion)
		{
			fb_assert(!m_inversion);
			m_inversion = inversion;
		}

	private:
		int compareKeys(const index_desc*, const UCHAR*, USHORT, const temporary_key*, USHORT) const;
		bool findSavedNode(thread_db* tdbb, Impure* impure, win* window, UCHAR**) const;
		UCHAR* getPosition(thread_db* tdbb, Impure* impure, win* window) const;
		UCHAR* openStream(thread_db* tdbb, Impure* impure, win* window) const;
		void setPage(thread_db* tdbb, Impure* impure, win* window) const;
		void setPosition(thread_db* tdbb, Impure* impure, record_param*,
						 win* window, const UCHAR*, const temporary_key&) const;
		bool setupBitmaps(thread_db* tdbb, Impure* impure) const;

		const Firebird::string m_name;
		NestConst<InversionNode> const m_index;
		NestConst<InversionNode> m_inversion;
		const size_t m_length;
		size_t m_offset;
	};

	class ExternalTableScan : public RecordStream
	{
		struct Impure : public RecordSource::Impure
		{
			FB_UINT64 irsb_position;
		};

	public:
		ExternalTableScan(CompilerScratch* csb, const Firebird::string& name, UCHAR stream);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;
		bool refetchRecord(thread_db* tdbb) const;
		bool lockRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

	private:
		const Firebird::string m_name;
	};

	class VirtualTableScan : public RecordStream
	{
	public:
		VirtualTableScan(CompilerScratch* csb, const Firebird::string& name, UCHAR stream);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;
		bool refetchRecord(thread_db* tdbb) const;
		bool lockRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

	protected:
		virtual bool retrieveRecord(thread_db* tdbb, jrd_rel* relation,
									FB_UINT64 position, Record* record) const = 0;

	private:
		const Firebird::string m_name;
	};

	class ProcedureScan : public RecordStream
	{
		struct Impure : public RecordSource::Impure
		{
			jrd_req* irsb_req_handle;
			VaryingString* irsb_message;
		};

	public:
		ProcedureScan(CompilerScratch* csb, const Firebird::string& name, UCHAR stream,
					  const jrd_prc* procedure, jrd_nod* inputs, jrd_nod* message);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;
		bool refetchRecord(thread_db* tdbb) const;
		bool lockRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

	private:
		void assignParams(thread_db* tdbb, const dsc* from_desc, const dsc* flag_desc,
						  const UCHAR* msg, const dsc* to_desc, SSHORT to_id, Record* record) const;

		const Firebird::string m_name;
		const jrd_prc* const m_procedure;
		NestConst<jrd_nod> const m_inputs;
		NestConst<jrd_nod> const m_message;
	};


	// Filtering (one -> one) access methods

	class SingularStream : public RecordSource
	{
	public:
		SingularStream(CompilerScratch* csb, RecordSource* next);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;
		bool refetchRecord(thread_db* tdbb) const;
		bool lockRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

		void markRecursive();
		void invalidateRecords(jrd_req* request) const;

		void findUsedStreams(StreamsArray& streams) const;
		void nullRecords(thread_db* tdbb) const;
		void saveRecords(thread_db* tdbb) const;
		void restoreRecords(thread_db* tdbb) const;

	private:
		NestConst<RecordSource> m_next;
	};

	class LockedStream : public RecordSource
	{
	public:
		LockedStream(CompilerScratch* csb, RecordSource* next);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;
		bool refetchRecord(thread_db* tdbb) const;
		bool lockRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

		void markRecursive();
		void invalidateRecords(jrd_req* request) const;

		void findUsedStreams(StreamsArray& streams) const;
		void nullRecords(thread_db* tdbb) const;
		void saveRecords(thread_db* tdbb) const;
		void restoreRecords(thread_db* tdbb) const;

	private:
		NestConst<RecordSource> m_next;
	};

	class FirstRowsStream : public RecordSource
	{
		struct Impure : public RecordSource::Impure
		{
			SINT64 irsb_count;
		};

	public:
		FirstRowsStream(CompilerScratch* csb, RecordSource* next, jrd_nod* value);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;
		bool refetchRecord(thread_db* tdbb) const;
		bool lockRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

		void markRecursive();
		void invalidateRecords(jrd_req* request) const;

		void findUsedStreams(StreamsArray& streams) const;
		void nullRecords(thread_db* tdbb) const;
		void saveRecords(thread_db* tdbb) const;
		void restoreRecords(thread_db* tdbb) const;

	private:
		NestConst<RecordSource> m_next;
		NestConst<jrd_nod> const m_value;
	};

	class SkipRowsStream : public RecordSource
	{
		struct Impure : public RecordSource::Impure
		{
			SINT64 irsb_count;
		};

	public:
		SkipRowsStream(CompilerScratch* csb, RecordSource* next, jrd_nod* value);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;
		bool refetchRecord(thread_db* tdbb) const;
		bool lockRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

		void markRecursive();
		void invalidateRecords(jrd_req* request) const;

		void findUsedStreams(StreamsArray& streams) const;
		void nullRecords(thread_db* tdbb) const;
		void saveRecords(thread_db* tdbb) const;
		void restoreRecords(thread_db* tdbb) const;

	private:
		NestConst<RecordSource> m_next;
		NestConst<jrd_nod> const m_value;
	};

	class FilteredStream : public RecordSource
	{
	public:
		FilteredStream(CompilerScratch* csb, RecordSource* next, jrd_nod* boolean);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;
		bool refetchRecord(thread_db* tdbb) const;
		bool lockRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

		void markRecursive();
		void invalidateRecords(jrd_req* request) const;

		void findUsedStreams(StreamsArray& streams) const;
		void nullRecords(thread_db* tdbb) const;
		void saveRecords(thread_db* tdbb) const;
		void restoreRecords(thread_db* tdbb) const;

		void setAnyBoolean(jrd_nod* anyBoolean, bool ansiAny, bool ansiNot)
		{
			fb_assert(!m_anyBoolean);
			m_anyBoolean = anyBoolean;

			m_ansiAny = ansiAny;
			m_ansiAll = !ansiAny;
			m_ansiNot = ansiNot;
		}

	private:
		bool evaluateBoolean(thread_db* tdbb) const;

		NestConst<RecordSource> m_next;
		NestConst<jrd_nod> const m_boolean;
		NestConst<jrd_nod> m_anyBoolean;
		bool m_ansiAny;
		bool m_ansiAll;
		bool m_ansiNot;
	};

	class SortedStream : public RecordSource
	{
		struct Impure : public RecordSource::Impure
		{
			Sort* irsb_sort;
		};

	public:
		static const USHORT FLAG_PROJECT = 0x1;	// sort is really a project
		static const USHORT FLAG_UNIQUE  = 0x2;	// sorts using unique key - for distinct and group by

		// Special values for SortMap::Item::fieldId.
		static const SSHORT ID_DBKEY		= -1;	// dbkey value
		static const SSHORT ID_DBKEY_VALID	= -2;	// dbkey valid flag
		static const SSHORT ID_TRANS 		= -3;	// transaction id of record

		// Sort map block
		class SortMap : public Firebird::PermanentStorage
		{
		public:
			struct Item
			{
				void clear()
				{
					desc.clear();
					flagOffset = stream = fieldId = 0;
					node = NULL;
				}

				dsc desc;					// relative descriptor
				USHORT flagOffset;			// offset of missing flag
				USHORT stream;				// stream for field id
				SSHORT fieldId;				// id for field (or ID constants)
				NestConst<jrd_nod> node;	// expression node
			};

			explicit SortMap(MemoryPool& p)
				: PermanentStorage(p),
				  length(0),
				  keyLength(0),
				  flags(0),
				  keyItems(p),
				  items(p)
			{
			}

			USHORT length;			// sort record length
			USHORT keyLength;		// key length in longwords
			USHORT flags;			// misc sort flags
			Firebird::Array<sort_key_def> keyItems;	// address of key descriptors
			Firebird::Array<Item> items;
		};

		SortedStream(CompilerScratch* csb, RecordSource* next, SortMap* map);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;
		bool refetchRecord(thread_db* tdbb) const;
		bool lockRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

		void markRecursive();
		void invalidateRecords(jrd_req* request) const;

		void findUsedStreams(StreamsArray& streams) const;
		void nullRecords(thread_db* tdbb) const;
		void saveRecords(thread_db* tdbb) const;
		void restoreRecords(thread_db* tdbb) const;

		USHORT getLength() const
		{
			return m_map->length;
		}

		USHORT getKeyLength() const
		{
			return m_map->keyLength;
		}

		UCHAR* getData(thread_db* tdbb) const;
		void mapData(thread_db* tdbb, jrd_req* request, UCHAR* data) const;

	private:
		Sort* init(thread_db* tdbb) const;

		NestConst<RecordSource> m_next;
		const SortMap* const m_map;
	};

	// Make moves in a window without going out of partition boundaries.
	class SlidingWindow
	{
	public:
		SlidingWindow(thread_db* aTdbb, const BaseBufferedStream* aStream,
			const LegacyNodeArray* aGroup, jrd_req* aRequest);
		~SlidingWindow();

		bool move(SINT64 delta);

	private:
		thread_db* tdbb;
		const BaseBufferedStream* const stream;
		const LegacyNodeArray* group;
		jrd_req* request;
		Firebird::Array<impure_value> partitionKeys;
		bool moved;
		FB_UINT64 savedPosition;
	};

	class AggregatedStream : public RecordStream
	{
		enum State
		{
			STATE_PROCESS_EOF = 0,	// We processed everything now process (EOF)
			STATE_PENDING,			// Values are pending from a prior fetch
			STATE_EOF_FOUND,		// We encountered EOF from the last attempted fetch
			STATE_GROUPING			// Entering EVL group before fetching the first record
		};

		struct Impure : public RecordSource::Impure
		{
			State state;
			FB_UINT64 pending;
		};

	public:
		AggregatedStream(CompilerScratch* csb, UCHAR stream, const LegacyNodeArray* group,
			const MapNode* map, BaseBufferedStream* next, const LegacyNodeArray* order);

		AggregatedStream(CompilerScratch* csb, UCHAR stream, const LegacyNodeArray* group,
			const MapNode* map, RecordSource* next);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;
		bool refetchRecord(thread_db* tdbb) const;
		bool lockRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

		void markRecursive();
		void invalidateRecords(jrd_req* request) const;

		void findUsedStreams(StreamsArray& streams) const;

	private:
		void init(CompilerScratch* csb);

		State evaluateGroup(thread_db* tdbb, State state) const;
		void finiDistinct(thread_db* tdbb, jrd_req* request) const;

		NestConst<BaseBufferedStream> m_bufferedStream;
		NestConst<RecordSource> m_next;
		const LegacyNodeArray* const m_group;
		const MapNode* const m_map;
		const LegacyNodeArray* const m_order;
		Firebird::Array<const jrd_nod*> m_winPassMap;
	};

	class WindowedStream : public RecordSource
	{
	public:
		WindowedStream(CompilerScratch* csb,
			Firebird::ObjectsArray<WindowSourceNode::Partition>& partitions, RecordSource* next);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;
		bool refetchRecord(thread_db* tdbb) const;
		bool lockRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

		void markRecursive();
		void invalidateRecords(jrd_req* request) const;

		void findUsedStreams(StreamsArray& streams) const;
		void nullRecords(thread_db* tdbb) const;
		void saveRecords(thread_db* tdbb) const;
		void restoreRecords(thread_db* tdbb) const;

	private:
		NestConst<BufferedStream> m_next;
		NestConst<RecordSource> m_joinedStream;
	};

	// Abstract class for different implementations of buffered streams.
	class BaseBufferedStream : public RecordSource
	{
	public:
		virtual void locate(thread_db* tdbb, FB_UINT64 position) const = 0;
		virtual FB_UINT64 getCount(jrd_req* request) const = 0;
		virtual FB_UINT64 getPosition(jrd_req* request) const = 0;
	};

	class BufferedStream : public BaseBufferedStream
	{
		struct FieldMap
		{
			static const UCHAR REGULAR_FIELD = 1;
			static const UCHAR TRANSACTION_ID = 2;
			static const UCHAR DBKEY_NUMBER = 3;
			static const UCHAR DBKEY_VALID = 4;

			FieldMap() : map_type(0), map_stream(0), map_id(0)
			{}

			FieldMap(UCHAR type, UCHAR stream, ULONG id)
				: map_type(type), map_stream(stream), map_id(id)
			{}

			UCHAR map_type;
			UCHAR map_stream;
			USHORT map_id;
		};

		struct Impure : public RecordSource::Impure
		{
			RecordBuffer* irsb_buffer;
			FB_UINT64 irsb_position;
		};

	public:
		BufferedStream(CompilerScratch* csb, RecordSource* next);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;
		bool refetchRecord(thread_db* tdbb) const;
		bool lockRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

		void markRecursive();
		void invalidateRecords(jrd_req* request) const;

		void findUsedStreams(StreamsArray& streams) const;
		void nullRecords(thread_db* tdbb) const;
		void saveRecords(thread_db* tdbb) const;
		void restoreRecords(thread_db* tdbb) const;

		void locate(thread_db* tdbb, FB_UINT64 position) const;
		FB_UINT64 getCount(jrd_req* request) const;

		FB_UINT64 getPosition(jrd_req* request) const
		{
			Impure* const impure = request->getImpure<Impure>(m_impure);
			return impure->irsb_position;
		}

	private:
		NestConst<RecordSource> m_next;
		Firebird::HalfStaticArray<FieldMap, OPT_STATIC_ITEMS> m_map;
		const Format* m_format;
	};


	// Multiplexing (many -> one) access methods

	class NestedLoopJoin : public RecordSource
	{
	public:
		NestedLoopJoin(CompilerScratch* csb, size_t count, RecordSource* const* args);
		NestedLoopJoin(CompilerScratch* csb, RecordSource* outer, RecordSource* inner,
					   jrd_nod* boolean, bool semiJoin, bool antiJoin);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;
		bool refetchRecord(thread_db* tdbb) const;
		bool lockRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

		void markRecursive();
		void invalidateRecords(jrd_req* request) const;

		void findUsedStreams(StreamsArray& streams) const;
		void nullRecords(thread_db* tdbb) const;
		void saveRecords(thread_db* tdbb) const;
		void restoreRecords(thread_db* tdbb) const;

	private:
		bool fetchRecord(thread_db*, size_t) const;

		const bool m_outerJoin;
		const bool m_semiJoin;
		const bool m_antiJoin;
		Firebird::Array<NestConst<RecordSource> > m_args;
		NestConst<jrd_nod> const m_boolean;
	};

	class FullOuterJoin : public RecordSource
	{
	public:
		FullOuterJoin(CompilerScratch* csb, RecordSource* arg1, RecordSource* arg2);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;
		bool refetchRecord(thread_db* tdbb) const;
		bool lockRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

		void markRecursive();
		void invalidateRecords(jrd_req* request) const;

		void findUsedStreams(StreamsArray& streams) const;
		void nullRecords(thread_db* tdbb) const;
		void saveRecords(thread_db* tdbb) const;
		void restoreRecords(thread_db* tdbb) const;

	private:
		NestConst<RecordSource> m_arg1;
		NestConst<RecordSource> m_arg2;
	};

	class HashJoin : public RecordSource
	{
		class HashTable;

		struct Impure : public RecordSource::Impure
		{
			HashTable* irsb_hash_table;
		};

	public:
		HashJoin(CompilerScratch* csb, size_t count,
				 RecordSource* const* args, const LegacyNodeArray* const* keys);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;
		bool refetchRecord(thread_db* tdbb) const;
		bool lockRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

		void markRecursive();
		void invalidateRecords(jrd_req* request) const;

		void findUsedStreams(StreamsArray& streams) const;
		void nullRecords(thread_db* tdbb) const;
		void saveRecords(thread_db* tdbb) const;
		void restoreRecords(thread_db* tdbb) const;

	private:
		size_t hashKeys(thread_db* tdbb, jrd_req* request, HashTable* table,
			const LegacyNodeArray* keys) const;
		bool compareKeys(thread_db* tdbb, jrd_req* request) const;
		bool fetchRecord(thread_db* tdbb, HashTable* table, size_t stream) const;

		NestConst<RecordSource> m_leader;
		const LegacyNodeArray* m_leaderKeys;
		Firebird::Array<NestConst<BufferedStream> > m_args;
		Firebird::Array<const LegacyNodeArray*> m_keys;
		const bool m_outerJoin;
		const bool m_semiJoin;
		const bool m_antiJoin;
	};

	class MergeJoin : public RecordSource
	{
		struct MergeFile
		{
			TempSpace* mfb_space;				// merge file uses SORT I/O routines
			ULONG mfb_equal_records;			// equality group cardinality
			ULONG mfb_record_size;				// matches sort map length
			ULONG mfb_current_block;			// current merge block in buffer
			ULONG mfb_block_size;				// merge block I/O size
			ULONG mfb_blocking_factor;			// merge equality records per block
			UCHAR* mfb_block_data;				// merge block I/O buffer
		};

		struct Impure : public RecordSource::Impure
		{
			USHORT irsb_mrg_count;				// next stream in group
			struct irsb_mrg_repeat
			{
				SLONG irsb_mrg_equal;			// queue of equal records
				SLONG irsb_mrg_equal_end;		// end of the equal queue
				SLONG irsb_mrg_equal_current;	// last fetched record from equal queue
				SLONG irsb_mrg_last_fetched;	// first sort merge record of next group
				SSHORT irsb_mrg_order;			// logical merge order by substream
				MergeFile irsb_mrg_file;		// merge equivalence file
			} irsb_mrg_rpt[1];
		};

		static const size_t MERGE_BLOCK_SIZE = 65536;

	public:
		MergeJoin(CompilerScratch* csb, size_t count,
				  SortedStream* const* args,
				  const LegacyNodeArray* const* keys);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;
		bool refetchRecord(thread_db* tdbb) const;
		bool lockRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

		void markRecursive();
		void invalidateRecords(jrd_req* request) const;

		void findUsedStreams(StreamsArray& streams) const;
		void nullRecords(thread_db* tdbb) const;
		void saveRecords(thread_db* tdbb) const;
		void restoreRecords(thread_db* tdbb) const;

	private:
		int compare(thread_db* tdbb, const LegacyNodeArray* node1,
			const LegacyNodeArray* node2) const;
		UCHAR* getData(thread_db* tdbb, MergeFile* mfb, SLONG record) const;
		SLONG getRecord(thread_db* tdbb, size_t index) const;
		bool fetchRecord(thread_db* tdbb, size_t index) const;

		Firebird::Array<NestConst<SortedStream> > m_args;
		Firebird::Array<const LegacyNodeArray*> m_keys;
	};

	class Union : public RecordStream
	{
		struct Impure : public RecordSource::Impure
		{
			USHORT irsb_count;
		};

	public:
		Union(CompilerScratch* csb, UCHAR stream,
			  size_t argCount, RecordSource* const* args, NestConst<MapNode>* maps,
			  size_t streamCount, const UCHAR* streams);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;
		bool refetchRecord(thread_db* tdbb) const;
		bool lockRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

		void markRecursive();
		void invalidateRecords(jrd_req* request) const;

	private:
		Firebird::Array<NestConst<RecordSource> > m_args;
		Firebird::Array<NestConst<MapNode> > m_maps;
		StreamsArray m_streams;
	};

	class RecursiveStream : public RecordStream
	{
		static const size_t MAX_RECURSE_LEVEL = 1024;

		enum Mode { ROOT, RECURSE };

		struct Impure: public RecordSource::Impure
		{
			USHORT irsb_level;
			Mode irsb_mode;
			UCHAR* irsb_stack;
			UCHAR* irsb_data;
		};

	public:
		RecursiveStream(CompilerScratch* csb, UCHAR stream, UCHAR mapStream,
					    RecordSource* root, RecordSource* inner,
					    const MapNode* rootMap, const MapNode* innerMap,
					    size_t streamCount, const UCHAR* innerStreams,
					    size_t saveOffset);

		void open(thread_db* tdbb) const;
		void close(thread_db* tdbb) const;

		bool getRecord(thread_db* tdbb) const;
		bool refetchRecord(thread_db* tdbb) const;
		bool lockRecord(thread_db* tdbb) const;

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) const;

		void markRecursive();
		void invalidateRecords(jrd_req* request) const;

	private:
		const UCHAR m_mapStream;
		NestConst<RecordSource> m_root;
		NestConst<RecordSource> m_inner;
		const MapNode* const m_rootMap;
		const MapNode* const m_innerMap;
		StreamsArray m_innerStreams;
		const size_t m_saveOffset;
		size_t m_saveSize;
	};

} // namespace

#endif // JRD_RECORD_SOURCE_H
