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
#include "../jrd/rse.h"
#include "../jrd/inf_pub.h"

struct btree_exp;
struct exp_index_buf;

namespace Ods
{
	struct btree_page;
}

namespace Jrd
{
	class thread_db;
	class jrd_req;
	class jrd_nod;
	class jrd_prc;
	class CompilerScratch;
	class RecordBuffer;
	class BtrPageGCLock;
	struct index_desc;
	struct record_param;
	struct sort_context;
	struct temporary_key;
	struct win;
	class BufferedStream;

	typedef Firebird::HalfStaticArray<UCHAR, OPT_STATIC_ITEMS> StreamsArray;

	// Abstract base class

	class RecordSource
	{
	public:
		virtual void open(thread_db* tdbb) = 0;
		virtual void close(thread_db* tdbb) = 0;

		virtual bool getRecord(thread_db* tdbb) = 0;
		virtual bool refetchRecord(thread_db* tdbb) = 0;
		virtual bool lockRecord(thread_db* tdbb) = 0;

		virtual void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer) = 0;

		virtual void markRecursive() = 0;
		virtual void invalidateRecords(jrd_req* request) = 0;

		virtual void findUsedStreams(StreamsArray& streams) = 0;
		virtual void nullRecords(thread_db* tdbb) = 0;
		virtual void saveRecords(thread_db* tdbb) = 0;
		virtual void restoreRecords(thread_db* tdbb) = 0;

		virtual ~RecordSource();

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
		static void dumpInversion(thread_db* tdbb, const jrd_nod* inversion,
			Firebird::UCharBuffer& buffer);

		static void saveRecord(thread_db* tdbb, record_param* rpb);
		static void restoreRecord(thread_db* tdbb, record_param* rpb);

		static bool rejectDuplicate(const UCHAR* /*data1*/, const UCHAR* /*data2*/, void* /*userArg*/)
		{
			return true;
		}

		ULONG m_impure;
		bool m_recursive;
	};


	// Helper class implementing some common methods

	class RecordStream : public RecordSource
	{
	public:
		RecordStream(CompilerScratch* csb, UCHAR stream, Format* format = NULL);

		virtual bool refetchRecord(thread_db* tdbb);
		virtual bool lockRecord(thread_db* tdbb);

		virtual void markRecursive();
		virtual void invalidateRecords(jrd_req* request);

		virtual void findUsedStreams(StreamsArray& streams);
		virtual void nullRecords(thread_db* tdbb);
		virtual void saveRecords(thread_db* tdbb);
		virtual void restoreRecords(thread_db* tdbb);

	protected:
		const UCHAR m_stream;
		Format* const m_format;
	};


	// Primary (table scan) access methods

	class FullTableScan : public RecordStream
	{
	public:
		FullTableScan(CompilerScratch* csb, const Firebird::string& name, UCHAR stream);

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

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
			jrd_nod* inversion);

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

	private:
		const Firebird::string m_name;
		jrd_nod* const m_inversion;
	};

	class IndexTableScan : public RecordStream
	{
		struct Impure : public RecordSource::Impure
		{
			SLONG irsb_nav_expanded_offset;				// page offset of current index node on expanded index page
			RecordNumber irsb_nav_number;				// last record number
			SLONG irsb_nav_page;						// index page number
			SLONG irsb_nav_incarnation;					// buffer/page incarnation counter
			RecordBitmap**	irsb_nav_bitmap;			// bitmap for inversion tree
			RecordBitmap*	irsb_nav_records_visited;	// bitmap of records already retrieved
			BtrPageGCLock*	irsb_nav_btr_gc_lock;		// lock to prevent removal of currently walked index page
			USHORT irsb_nav_offset;						// page offset of current index node
			USHORT irsb_nav_lower_length;				// length of lower key value
			USHORT irsb_nav_upper_length;				// length of upper key value
			USHORT irsb_nav_length;						// length of expanded key
			UCHAR irsb_nav_data[1];						// expanded key, upper bound, and index desc
		};

	public:
		IndexTableScan(CompilerScratch* csb, const Firebird::string& name, UCHAR stream,
			jrd_nod* index, USHORT keyLength);

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

		void setInversion(jrd_nod* inversion)
		{
			fb_assert(!m_inversion);
			m_inversion = inversion;
		}

	private:
		int compareKeys(const index_desc*, const UCHAR*, USHORT, const temporary_key*, USHORT);
		btree_exp* findCurrent(exp_index_buf*, Ods::btree_page*, const UCHAR*);
		bool findSavedNode(thread_db* tdbb, win* window, UCHAR**);
		UCHAR* getPosition(thread_db* tdbb, win* window, btree_exp**);
		UCHAR* openStream(thread_db* tdbb, win* window);
		void setPage(thread_db* tdbb, win* window);
		void setPosition(thread_db* tdbb, record_param*, win* window, const UCHAR*, btree_exp*,
			const UCHAR*, USHORT);
		bool setupBitmaps(thread_db* tdbb);

		const Firebird::string m_name;
		jrd_nod* const m_index;
		jrd_nod* m_inversion;
		const USHORT m_length;
		USHORT m_offset;
	};

	class ExternalTableScan : public RecordStream
	{
		struct Impure : public RecordSource::Impure
		{
			FB_UINT64 irsb_position;
		};

	public:
		ExternalTableScan(CompilerScratch* csb, const Firebird::string& name, UCHAR stream);

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);
		bool refetchRecord(thread_db* tdbb);
		bool lockRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

	private:
		const Firebird::string m_name;
	};

	class VirtualTableScan : public RecordStream
	{
		struct Impure : public RecordSource::Impure
		{
			RecordBuffer* irsb_record_buffer;
		};

	public:
		VirtualTableScan(CompilerScratch* csb, const Firebird::string& name, UCHAR stream);

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);
		bool refetchRecord(thread_db* tdbb);
		bool lockRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

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
					  jrd_prc* procedure, jrd_nod* inputs, jrd_nod* message);

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);
		bool refetchRecord(thread_db* tdbb);
		bool lockRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

	private:
		void assignParams(thread_db* tdbb, const dsc* from_desc, const dsc* flag_desc,
						  const UCHAR* msg, const dsc* to_desc, SSHORT to_id, Record* record);

		const Firebird::string m_name;
		jrd_prc* const m_procedure;
		jrd_nod* const m_inputs;
		jrd_nod* const m_message;
	};


	// Filtering (one -> one) access methods

	class SingularStream : public RecordSource
	{
	public:
		SingularStream(CompilerScratch* csb, RecordSource* next);

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);
		bool refetchRecord(thread_db* tdbb);
		bool lockRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

		void markRecursive();
		void invalidateRecords(jrd_req* request);

		void findUsedStreams(StreamsArray& streams);
		void nullRecords(thread_db* tdbb);
		void saveRecords(thread_db* tdbb);
		void restoreRecords(thread_db* tdbb);

	private:
		RecordSource* const m_next;
	};

	class LockedStream : public RecordSource
	{
	public:
		LockedStream(CompilerScratch* csb, RecordSource* next);

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);
		bool refetchRecord(thread_db* tdbb);
		bool lockRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

		void markRecursive();
		void invalidateRecords(jrd_req* request);

		void findUsedStreams(StreamsArray& streams);
		void nullRecords(thread_db* tdbb);
		void saveRecords(thread_db* tdbb);
		void restoreRecords(thread_db* tdbb);

	private:
		RecordSource* const m_next;
	};

	class FirstRowsStream : public RecordSource
	{
		struct Impure : public RecordSource::Impure
		{
			SINT64 irsb_count;
		};

	public:
		FirstRowsStream(CompilerScratch* csb, RecordSource* next, jrd_nod* value);

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);
		bool refetchRecord(thread_db* tdbb);
		bool lockRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

		void markRecursive();
		void invalidateRecords(jrd_req* request);

		void findUsedStreams(StreamsArray& streams);
		void nullRecords(thread_db* tdbb);
		void saveRecords(thread_db* tdbb);
		void restoreRecords(thread_db* tdbb);

	private:
		RecordSource* const m_next;
		jrd_nod* const m_value;
	};

	class SkipRowsStream : public RecordSource
	{
		struct Impure : public RecordSource::Impure
		{
			SINT64 irsb_count;
		};

	public:
		SkipRowsStream(CompilerScratch* csb, RecordSource* next, jrd_nod* value);

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);
		bool refetchRecord(thread_db* tdbb);
		bool lockRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

		void markRecursive();
		void invalidateRecords(jrd_req* request);

		void findUsedStreams(StreamsArray& streams);
		void nullRecords(thread_db* tdbb);
		void saveRecords(thread_db* tdbb);
		void restoreRecords(thread_db* tdbb);

	private:
		RecordSource* const m_next;
		jrd_nod* const m_value;
	};

	class FilteredStream : public RecordSource
	{
	public:
		FilteredStream(CompilerScratch* csb, RecordSource* next, jrd_nod* boolean);

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);
		bool refetchRecord(thread_db* tdbb);
		bool lockRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

		void markRecursive();
		void invalidateRecords(jrd_req* request);

		void findUsedStreams(StreamsArray& streams);
		void nullRecords(thread_db* tdbb);
		void saveRecords(thread_db* tdbb);
		void restoreRecords(thread_db* tdbb);

		void setAnyBoolean(jrd_nod* anyBoolean, bool ansiAny, bool ansiNot)
		{
			fb_assert(!m_anyBoolean);
			m_anyBoolean = anyBoolean;

			m_ansiAny = ansiAny;
			m_ansiAll = !ansiAny;
			m_ansiNot = ansiNot;
		}

	private:
		bool evaluateBoolean(thread_db* tdbb);

		RecordSource* const m_next;
		jrd_nod* const m_boolean;
		jrd_nod* m_anyBoolean;
		bool m_ansiAny;
		bool m_ansiAll;
		bool m_ansiNot;
	};

	class SortedStream : public RecordSource
	{
		struct Impure : public RecordSource::Impure
		{
			sort_context* irsb_sort_handle;
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

				dsc desc;			// relative descriptor
				USHORT flagOffset;	// offset of missing flag
				USHORT stream;		// stream for field id
				SSHORT fieldId;		// id for field (or ID constants)
				jrd_nod* node;		// expression node
			};

			explicit SortMap(MemoryPool& p)
				: PermanentStorage(p),
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

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);
		bool refetchRecord(thread_db* tdbb);
		bool lockRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

		void markRecursive();
		void invalidateRecords(jrd_req* request);

		void findUsedStreams(StreamsArray& streams);
		void nullRecords(thread_db* tdbb);
		void saveRecords(thread_db* tdbb);
		void restoreRecords(thread_db* tdbb);

		USHORT getLength() const
		{
			return m_map->length;
		}

		USHORT getKeyLength() const
		{
			return m_map->keyLength;
		}

		UCHAR* getData(thread_db* tdbb);
		void mapData(thread_db* tdbb, jrd_req* request, UCHAR* data);

	private:
		sort_context* init(thread_db* tdbb);

		RecordSource* const m_next;
		SortMap* const m_map;
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
		};

	public:
		AggregatedStream(CompilerScratch* csb, UCHAR stream, jrd_nod* const group,
			jrd_nod* const map, RecordSource* next);

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);
		bool refetchRecord(thread_db* tdbb);
		bool lockRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

		void markRecursive();
		void invalidateRecords(jrd_req* request);

	private:
		State evaluateGroup(thread_db* tdbb, State state);
		void initDistinct(jrd_req* request, const jrd_nod* node);
		void computeDistinct(thread_db* tdbb, jrd_nod* node);
		void finiDistinct(jrd_req* request);

		RecordSource* const m_next;
		jrd_nod* const m_group;
		jrd_nod* const m_map;
	};

	class WindowedStream : public RecordSource
	{
	public:
		WindowedStream(CompilerScratch* csb, const jrd_nod* nodWindows, RecordSource* next);

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);
		bool refetchRecord(thread_db* tdbb);
		bool lockRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

		void markRecursive();
		void invalidateRecords(jrd_req* request);

		void findUsedStreams(StreamsArray& streams);
		void nullRecords(thread_db* tdbb);
		void saveRecords(thread_db* tdbb);
		void restoreRecords(thread_db* tdbb);

	private:
		jrd_nod* m_mainMap;
		BufferedStream* m_next;
		RecordSource* m_joinedStream;
	};

	class BufferedStream : public RecordSource
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

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);
		bool refetchRecord(thread_db* tdbb);
		bool lockRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

		void markRecursive();
		void invalidateRecords(jrd_req* request);

		void findUsedStreams(StreamsArray& streams);
		void nullRecords(thread_db* tdbb);
		void saveRecords(thread_db* tdbb);
		void restoreRecords(thread_db* tdbb);

		void locate(thread_db* tdbb, FB_UINT64 position);
		FB_UINT64 getCount(jrd_req* request) const;

	private:
		RecordSource* m_next;
		Firebird::HalfStaticArray<FieldMap, OPT_STATIC_ITEMS> m_map;
		Format* m_format;
	};


	// Multiplexing (many -> one) access methods

	class NestedLoopJoin : public RecordSource
	{
	public:
		NestedLoopJoin(CompilerScratch* csb, size_t count, RecordSource* const* args);
		NestedLoopJoin(CompilerScratch* csb, RecordSource* outer, RecordSource* inner,
					   jrd_nod* boolean, bool semiJoin, bool antiJoin);

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);
		bool refetchRecord(thread_db* tdbb);
		bool lockRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

		void markRecursive();
		void invalidateRecords(jrd_req* request);

		void findUsedStreams(StreamsArray& streams);
		void nullRecords(thread_db* tdbb);
		void saveRecords(thread_db* tdbb);
		void restoreRecords(thread_db* tdbb);

	private:
		bool fetchRecord(thread_db*, size_t);

		const bool m_outerJoin;
		const bool m_semiJoin;
		const bool m_antiJoin;
		Firebird::Array<RecordSource*> m_args;
		jrd_nod* const m_boolean;
	};

	class FullOuterJoin : public RecordSource
	{
	public:
		FullOuterJoin(CompilerScratch* csb, RecordSource* arg1, RecordSource* arg2);

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);
		bool refetchRecord(thread_db* tdbb);
		bool lockRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

		void markRecursive();
		void invalidateRecords(jrd_req* request);

		void findUsedStreams(StreamsArray& streams);
		void nullRecords(thread_db* tdbb);
		void saveRecords(thread_db* tdbb);
		void restoreRecords(thread_db* tdbb);

	private:
		RecordSource* const m_arg1;
		RecordSource* const m_arg2;
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
				 RecordSource* const* args, jrd_nod* const* keys);

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);
		bool refetchRecord(thread_db* tdbb);
		bool lockRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

		void markRecursive();
		void invalidateRecords(jrd_req* request);

		void findUsedStreams(StreamsArray& streams);
		void nullRecords(thread_db* tdbb);
		void saveRecords(thread_db* tdbb);
		void restoreRecords(thread_db* tdbb);

	private:
		size_t hashKeys(thread_db* tdbb, jrd_req* request, HashTable* table, jrd_nod* keys);
		bool compareKeys(thread_db* tdbb, jrd_req* request);
		bool fetchRecord(thread_db* tdbb, HashTable* table, size_t stream);

		RecordSource* m_leader;
		jrd_nod* m_leaderKeys;
		Firebird::Array<BufferedStream*> m_args;
		Firebird::Array<jrd_nod*> m_keys;
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
				  SortedStream* const* args, jrd_nod* const* keys);

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);
		bool refetchRecord(thread_db* tdbb);
		bool lockRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

		void markRecursive();
		void invalidateRecords(jrd_req* request);

		void findUsedStreams(StreamsArray& streams);
		void nullRecords(thread_db* tdbb);
		void saveRecords(thread_db* tdbb);
		void restoreRecords(thread_db* tdbb);

	private:
		int compare(thread_db* tdbb, jrd_nod* node1, jrd_nod* node2);
		UCHAR* getData(thread_db* tdbb, MergeFile* mfb, SLONG record);
		SLONG getRecord(thread_db* tdbb, size_t index);
		bool fetchRecord(thread_db* tdbb, size_t index);

		Firebird::Array<SortedStream*> m_args;
		Firebird::Array<jrd_nod*> m_keys;
	};

	class Union : public RecordStream
	{
		struct Impure : public RecordSource::Impure
		{
			USHORT irsb_count;
		};

	public:
		Union(CompilerScratch* csb, UCHAR stream,
			  size_t argCount, RecordSource* const* args, jrd_nod* const* maps,
			  size_t streamCount, const UCHAR* streams);

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);
		bool refetchRecord(thread_db* tdbb);
		bool lockRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

		void markRecursive();
		void invalidateRecords(jrd_req* request);

	private:
		Firebird::Array<RecordSource*> m_args;
		Firebird::Array<jrd_nod*> m_maps;
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
					    jrd_nod* rootMap, jrd_nod* innerMap,
					    size_t streamCount, const UCHAR* innerStreams,
					    size_t baseImpure);

		void open(thread_db* tdbb);
		void close(thread_db* tdbb);

		bool getRecord(thread_db* tdbb);
		bool refetchRecord(thread_db* tdbb);
		bool lockRecord(thread_db* tdbb);

		void dump(thread_db* tdbb, Firebird::UCharBuffer& buffer);

		void markRecursive();
		void invalidateRecords(jrd_req* request);

	private:
		const UCHAR m_mapStream;
		RecordSource* const m_root;
		RecordSource* const m_inner;
		jrd_nod* const m_rootMap;
		jrd_nod* const m_innerMap;
		StreamsArray m_innerStreams;
		size_t m_impureSize;
	};

} // namespace

#endif // JRD_RECORD_SOURCE_H
