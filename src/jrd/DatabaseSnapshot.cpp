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

#include "firebird.h"
#include "ids.h"

#include "../common/classes/auto.h"
#include "../common/classes/locks.h"

#include "../jrd/gdsassert.h"
#include "../jrd/jrd.h"
#include "../jrd/cch.h"
#include "../jrd/ini.h"
#include "../jrd/nbak.h"
#include "../jrd/os/guid.h"
#include "../jrd/os/pio.h"
#include "../jrd/req.h"
#include "../jrd/tra.h"
#include "../jrd/blb_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/lck_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/os/pio_proto.h"
#include "../jrd/pag_proto.h"
#include "../jrd/thread_proto.h"

#include "../jrd/Relation.h"
#include "../jrd/RecordBuffer.h"
#include "../jrd/DatabaseSnapshot.h"

#include "../common/utils_proto.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef UNIX
#include <signal.h>
#endif

#ifdef WIN_NT
#include <process.h>
#include <windows.h>
#endif


using namespace Firebird;
using namespace Jrd;

const UCHAR TAG_DBB		= '\xFF'; //-1U
const UCHAR TAG_RECORD	= '\xFE'; //-2U;


// SharedMemory class

const size_t DatabaseSnapshot::SharedMemory::VERSION = 1;
const size_t DatabaseSnapshot::SharedMemory::DEFAULT_SIZE = 1048576;


DatabaseSnapshot::SharedMemory::SharedMemory()
{
	TEXT filename[MAXPATHLEN];
	gds__prefix_lock(filename, MONITOR_FILE);

	ISC_STATUS_ARRAY statusVector;
	base = (Header*) ISC_map_file(statusVector,
								  filename,
								  init, this,
								  DEFAULT_SIZE,
								  &handle);
	if (!base)
	{
		gds__log("Cannot initialize the shared memory region");
		status_exception::raise(statusVector);
	}

	fb_assert(base->version == VERSION);
}


DatabaseSnapshot::SharedMemory::~SharedMemory()
{
	ISC_STATUS_ARRAY statusVector;
	ISC_unmap_file(statusVector, &handle, 0);
}


void DatabaseSnapshot::SharedMemory::acquire()
{
#ifdef WIN_NT
	checkMutex("lock", ISC_mutex_lock(&mutex));
#else
	checkMutex("lock", ISC_mutex_lock(&base->mutex));
#endif
	if (base->allocated > handle.sh_mem_length_mapped)
	{
#if (defined HAVE_MMAP || defined WIN_NT)
		const size_t newSize = base->allocated;
		ISC_STATUS_ARRAY statusVector;
		base = (Header*) ISC_remap_file(statusVector, &handle, newSize, FALSE);
		if (!base)
		{
			Firebird::status_exception::raise(statusVector);
		}
#else
		Firebird::status_exception::raise(isc_random, isc_arg_string,
										  "Monitoring table space exhausted",
										  0);
#endif
	}
}


void DatabaseSnapshot::SharedMemory::release()
{
#ifdef WIN_NT
	checkMutex("unlock", ISC_mutex_unlock(&mutex));
#else
	checkMutex("unlock", ISC_mutex_unlock(&base->mutex));
#endif
}


ClumpletReader* DatabaseSnapshot::SharedMemory::readData(thread_db* tdbb)
{
	garbageCollect(tdbb, false);

	const UCHAR* const buffer = (UCHAR*) base + sizeof(Header);
	const size_t length = base->used;

	MemoryPool& pool = *tdbb->getDefaultPool();

	ClumpletReader* reader = FB_NEW(pool)
		ClumpletReader(pool, ClumpletReader::WideUnTagged, buffer, length);

	return reader;
}


void DatabaseSnapshot::SharedMemory::writeData(thread_db* tdbb, ClumpletWriter& writer)
{
	garbageCollect(tdbb, true);

	// Get number of bytes to add
	const size_t length = writer.getBufferLength();

	// Do we need to extend the allocated memory?
	while (base->used + length > base->allocated)
	{
		extend();
	}

	// Get the remapped pointer
	UCHAR* buffer = (UCHAR*) base + sizeof(Header);
	// Copy the data at the insertion point
	memcpy(buffer + base->used, writer.getBuffer(), length);
	// Adjust the length
	base->used += length;
}


void DatabaseSnapshot::SharedMemory::garbageCollect(thread_db* tdbb, bool self)
{
	fb_assert(tdbb);
	Database* dbb = tdbb->getDatabase();
	fb_assert(dbb);

	// Get the buffer length
	UCHAR* buffer = (UCHAR*) base + sizeof(Header);
	const size_t length = base->used;

	if (!length)
	{
		// No data means no garbage to collect
		return;
	}

	// Initialize reader and writer helpers
	ClumpletReader reader(ClumpletReader::WideUnTagged, buffer, length);
	ClumpletWriter writer(ClumpletReader::WideUnTagged, MAX_ULONG);

	// Temporary lock used to check existence of the given dbb instance
	AutoPtr<Lock> temp_lock(FB_NEW_RPT(*tdbb->getDefaultPool(), sizeof(FB_GUID)) Lock());
	temp_lock->lck_type = LCK_instance;
	temp_lock->lck_owner_handle =
		LCK_get_owner_handle(tdbb, temp_lock->lck_type);
	temp_lock->lck_length = sizeof(FB_GUID);
	temp_lock->lck_dbb = dbb;

	// Parse the data and remove all garbage clumplets

	bool garbage_collect = false;

	while (!reader.isEof())
	{
		if (reader.getClumpTag() == TAG_DBB)
		{
			FB_GUID guid;
			fb_assert(reader.getClumpLength() == sizeof(FB_GUID));
			memcpy(&guid, reader.getBytes(), sizeof(FB_GUID));

			// Is this our own dbb instance?
			const bool our_dbb =
				!memcmp(&guid, &dbb->dbb_guid, sizeof(FB_GUID));

			if (self)
			{
				// We're asked to garbage collect our own data only
				garbage_collect = our_dbb;
			}
			else if (our_dbb)
			{
				// We're a generic reader and this is our dbb clumplet.
				// Don't garbage collect as we're definitely alive.
				garbage_collect = false;
			}
			else
			{
				ThreadStatusGuard temp_status(tdbb);

				memcpy(temp_lock->lck_key.lck_string, &guid, sizeof(FB_GUID));
				if (LCK_lock(tdbb, temp_lock, LCK_EX, LCK_NO_WAIT))
				{
					// This clumplet identifies the instance already died.
					// Garbage collect this unneccessary crap.
					LCK_release(tdbb, temp_lock);
					garbage_collect = true;
				}
				else
				{
					// This clumplet identifies the instance still alive.
					// Don't remove its data.
					garbage_collect = false;
				}
			}
		}

		// Copy data if we shouldn't garbage collect this clumplet
		if (!garbage_collect)
		{
			writer.insertBytes(reader.getClumpTag(),
							   reader.getBytes(),
							   reader.getClumpLength());
		}

		reader.moveNext();
	}

	// If we have compacted the data, let's adjust the stored length
	const size_t newLength = writer.getBufferLength();
	if (newLength < base->used)
	{
		memcpy(buffer, writer.getBuffer(), newLength);
		base->used = newLength;
	}
}


void DatabaseSnapshot::SharedMemory::extend()
{
	const size_t newSize = handle.sh_mem_length_mapped + DEFAULT_SIZE;

#if (defined HAVE_MMAP || defined WIN_NT)
	ISC_STATUS_ARRAY statusVector;
	base = (Header*) ISC_remap_file(statusVector, &handle, newSize, TRUE);
	if (!base)
	{
		Firebird::status_exception::raise(statusVector);
	}
	base->allocated = handle.sh_mem_length_mapped;
#else
	Firebird::status_exception::raise(isc_random, isc_arg_string,
									  "Monitoring table space exhausted",
									  0);
#endif
}


void DatabaseSnapshot::SharedMemory::checkMutex(const TEXT* string, int state)
{
	if (state)
	{
		TEXT msg[BUFFER_TINY];

		sprintf(msg, "MONITOR: mutex %s error, status = %d", string, state);
		gds__log(msg);

		fprintf(stderr, "%s\n", msg);
		exit(FINI_ERROR);
	}
}


void DatabaseSnapshot::SharedMemory::init(void* arg, SH_MEM_T* shmemData, bool initialize)
{
	SharedMemory* shmem = (SharedMemory*) arg;
	fb_assert(shmem);

#ifdef WIN_NT
	char buffer[MAXPATHLEN];
	gds__prefix_lock(buffer, MONITOR_FILE);
	checkMutex("init", ISC_mutex_init(&shmem->mutex, buffer));
#endif

	if (!initialize)
		return;

	// Initialize the shared data header
	Header* header = (Header*) shmemData->sh_mem_address;
	header->version = VERSION;
	header->used = 0;
	header->allocated = 0;

#ifndef WIN_NT
	checkMutex("init", ISC_mutex_init(&header->mutex));
#endif
}


// DatabaseSnapshot class

GlobalPtr<Mutex> DatabaseSnapshot::initMutex;
DatabaseSnapshot::SharedMemory* DatabaseSnapshot::dump = NULL;
int DatabaseSnapshot::pid = getpid();


DatabaseSnapshot* DatabaseSnapshot::create(thread_db* tdbb)
{
	SET_TDBB(tdbb);

	jrd_tra* transaction = tdbb->getTransaction();
	fb_assert(transaction);

	if (!transaction->tra_db_snapshot)
	{
		// Create a database snapshot and store it
		// in the transaction block
		MemoryPool& pool = *transaction->tra_pool;
		transaction->tra_db_snapshot =
			FB_NEW(pool) DatabaseSnapshot(tdbb, pool);
	}

	return transaction->tra_db_snapshot;
}


int DatabaseSnapshot::blockingAst(void* ast_object)
{
	Database* dbb = static_cast<Database*>(ast_object);

	try
	{
		Lock* lock = dbb->dbb_monitor_lock;

		Database::SyncGuard dsGuard(dbb, true);

		ThreadContextHolder tdbb;
		tdbb->setDatabase(lock->lck_dbb);
		tdbb->setAttachment(lock->lck_attachment);

		Jrd::ContextPoolHolder context(tdbb, dbb->dbb_permanent);

		if (!(dbb->dbb_ast_flags & DBB_monitor_off))
		{
			try {
				// Write the data to the shared memory
				dumpData(tdbb, true);

				// Release the lock and mark dbb as requesting a new one
				LCK_release(tdbb, lock);
				dbb->dbb_ast_flags |= DBB_monitor_off;
			}
			catch (const Exception&) {
				gds__log("Unexpected exception at the AST level");
			}
		}
	}
	catch (const Firebird::Exception&)
	{} // no-op

	return 0;
}


DatabaseSnapshot::DatabaseSnapshot(thread_db* tdbb, MemoryPool& pool)
	: snapshot(pool), idMap(pool), idCounter(0)
{
	Database* const dbb = tdbb->getDatabase();
	fb_assert(dbb);

	const USHORT ods_version = ENCODE_ODS(dbb->dbb_ods_version, dbb->dbb_minor_original);

	// Initialize record buffers
	RecordBuffer* const dbb_buffer =
		ods_version >= ODS_11_1 ? allocBuffer(tdbb, pool, rel_mon_database) : NULL;
	RecordBuffer* const att_buffer =
		ods_version >= ODS_11_1 ? allocBuffer(tdbb, pool, rel_mon_attachments) : NULL;
	RecordBuffer* const tra_buffer =
		ods_version >= ODS_11_1 ? allocBuffer(tdbb, pool, rel_mon_transactions) : NULL;
	RecordBuffer* const stmt_buffer =
		ods_version >= ODS_11_1 ? allocBuffer(tdbb, pool, rel_mon_statements) : NULL;
	RecordBuffer* const call_buffer =
		ods_version >= ODS_11_1 ? allocBuffer(tdbb, pool, rel_mon_calls) : NULL;
	RecordBuffer* const io_stat_buffer =
		ods_version >= ODS_11_1 ? allocBuffer(tdbb, pool, rel_mon_io_stats) : NULL;
	RecordBuffer* const rec_stat_buffer =
		ods_version >= ODS_11_1 ? allocBuffer(tdbb, pool, rel_mon_rec_stats) : NULL;
	RecordBuffer* const ctx_var_buffer =
		ods_version >= ODS_11_2 ? allocBuffer(tdbb, pool, rel_mon_ctx_vars) : NULL;
	RecordBuffer* const mem_usage_buffer =
		ods_version >= ODS_11_2 ? allocBuffer(tdbb, pool, rel_mon_mem_usage) : NULL;

	const Attachment* const attachment = tdbb->getAttachment();
	fb_assert(attachment);

	// This variable defines whether we're interested in the global data
	// (i.e. all attachments) and hence should signal other processes
	// or it's enough to return the local data (our own attachment) only.
	const bool broadcast = attachment->att_user->locksmith();

	AutoPtr<ClumpletReader> reader(NULL);

	if (broadcast)
	{
		dumpData(tdbb, true);

		// Release our own lock and mark dbb as requesting a new one
		LCK_release(tdbb, dbb->dbb_monitor_lock);
		dbb->dbb_ast_flags |= DBB_monitor_off;

		// Signal other processes to dump their data
		Lock temp_lock, *lock = &temp_lock;
		lock->lck_dbb = dbb;
		lock->lck_length = sizeof(SLONG);
		lock->lck_key.lck_long = 0;
		lock->lck_type = LCK_monitor;
		lock->lck_owner_handle = LCK_get_owner_handle(tdbb, lock->lck_type);
		lock->lck_parent = dbb->dbb_lock;

		if (LCK_lock(tdbb, lock, LCK_EX, LCK_WAIT))
			LCK_release(tdbb, lock);

		// Read the shared memory
		DumpGuard guard(dump);
		reader = dump->readData(tdbb);
	}
	else
	{
		reader = dumpData(tdbb, false);
	}

	reader->rewind();

	// Parse the dump
	RecordBuffer* buffer = NULL;
	Record* record = NULL;

	int rid = 0;
	bool database_processed = false, database_seen = false;
	bool fields_processed = false, allowed = false;

	while (!reader->isEof())
	{
		if (reader->getClumpTag() == TAG_DBB)
		{
			reader->moveNext();

			if (fields_processed)
			{
				buffer->store(record);
				fields_processed = false;
			}
		}
		else if (reader->getClumpTag() == TAG_RECORD)
		{
			rid = reader->getInt();
			reader->moveNext();

			if (fields_processed)
			{
				buffer->store(record);
				fields_processed = false;
			}

			switch (rid)
			{
			case rel_mon_database:
				buffer = dbb_buffer;
				break;
			case rel_mon_attachments:
				buffer = att_buffer;
				break;
			case rel_mon_transactions:
				buffer = tra_buffer;
				break;
			case rel_mon_statements:
				buffer = stmt_buffer;
				break;
			case rel_mon_calls:
				buffer = call_buffer;
				break;
			case rel_mon_io_stats:
				buffer = io_stat_buffer;
				break;
			case rel_mon_rec_stats:
				buffer = rec_stat_buffer;
				break;
			case rel_mon_ctx_vars:
				buffer = ctx_var_buffer;
				break;
			case rel_mon_mem_usage:
				buffer = mem_usage_buffer;
				break;
			default:
				fb_assert(false);
			}

			if (buffer)
			{
				record = buffer->getTempRecord();
				clearRecord(record);
			}
			else
			{
				record = NULL;
			}
		}
		else
		{
			const int fid = reader->getClumpTag();
			const size_t length = reader->getClumpLength();

			const char* source =
				checkNull(rid, fid, (char*) reader->getBytes(), length);

			reader->moveNext();

			if (rid == rel_mon_database)
			{
				if (fid == f_mon_db_name)
				{
					database_processed = database_seen;

					// Do we look at our own database record?
					allowed = !dbb->dbb_database_name.compare(source, length);
					if (allowed)
					{
						// Yep, we've seen it
						database_seen = true;
					}
				}

				// We need to return the database record only once
				if (record && allowed && !database_processed)
				{
					putField(record, fid, source, length);
					fields_processed = true;
				}
			}
			// The generic logic that covers all relations
			// except MON$DATABASE
			else if (record && allowed)
			{
				putField(record, fid, source, length);
				fields_processed = true;
			}
		}
	}

	if (fields_processed)
	{
		buffer->store(record);
		fields_processed = false;
	}
}


DatabaseSnapshot::~DatabaseSnapshot()
{
	for (size_t i = 0; i < snapshot.getCount(); i++)
	{
		delete snapshot[i].data;
	}
}


RecordBuffer* DatabaseSnapshot::getData(const jrd_rel* relation) const
{
	fb_assert(relation);

	for (size_t i = 0; i < snapshot.getCount(); i++)
	{
		if (snapshot[i].rel_id == relation->rel_id)
			return snapshot[i].data;
	}

	return NULL;
}


RecordBuffer* DatabaseSnapshot::allocBuffer(thread_db* tdbb,
											MemoryPool& pool,
											int rel_id)
{
	jrd_rel* relation = MET_lookup_relation_id(tdbb, rel_id, false);
	fb_assert(relation);
	MET_scan_relation(tdbb, relation);
	fb_assert(relation->isVirtual());
	Format* format = MET_current(tdbb, relation);
	fb_assert(format);

	RecordBuffer* buffer = FB_NEW(pool) RecordBuffer(pool, format);
	RelationData data = {relation->rel_id, buffer};
	snapshot.add(data);

	return buffer;
}


void DatabaseSnapshot::clearRecord(Record* record)
{
	fb_assert(record);

	// Initialize all fields to NULLs
	memset(record->rec_data, 0, record->rec_length);
	const size_t null_bytes = (record->rec_format->fmt_count + 7) >> 3;
	memset(record->rec_data, 0xFF, null_bytes);
}


void DatabaseSnapshot::putField(Record* record, int id, const void* source, size_t length)
{
	fb_assert(record);

	const Format* const format = record->rec_format;
	fb_assert(format && id < format->fmt_count);

	const dsc desc = format->fmt_desc[id];
	UCHAR* const address = record->rec_data + (IPTR) desc.dsc_address;

	if (!source)
	{
		SET_NULL(record, id);
		return;
	}

	if (length == sizeof(SINT64) && desc.dsc_dtype == dtype_long)
	{
		// special case: translate 64-bit global ID into 32-bit local ID
		const SINT64 global_id = *(SINT64*) source;
		SLONG local_id = 0;
		if (!idMap.get(global_id, local_id))
		{
			local_id = ++idCounter;
			idMap.put(global_id, local_id);
		}
		*(SLONG*) address = local_id;
		CLEAR_NULL(record, id);
		return;
	}

	switch (desc.dsc_dtype) {
	case dtype_text:
		{
			const char* const string = (char*) source;
			const size_t max_length = desc.dsc_length;
			length = MIN(length, max_length);
			memcpy(address, string, length);
			memset(address + length, ' ', max_length - length);
		}
		break;
	case dtype_varying:
		{
			const char* const string = (char*) source;
			const size_t max_length = desc.dsc_length - sizeof(USHORT);
			length = MIN(length, max_length);
			vary* varying = (vary*) address;
			varying->vary_length = length;
			memcpy(varying->vary_string, string, length);
		}
		break;

	case dtype_short:
		*(SSHORT*) address = *(SSHORT*) source;
		break;
	case dtype_long:
		*(SLONG*) address = *(SLONG*) source;
		break;
	case dtype_int64:
		*(SINT64*) address = *(SINT64*) source;
		break;

	case dtype_real:
		*(float*) address = *(float*) source;
		break;
	case dtype_double:
		*(double*) address = *(double*) source;
		break;

	case dtype_sql_date:
		*(ISC_DATE*) address = *(ISC_DATE*) source;
		break;
	case dtype_sql_time:
		*(ISC_TIME*) address = *(ISC_TIME*) source;
		break;
	case dtype_timestamp:
		*(ISC_TIMESTAMP*) address = *(ISC_TIMESTAMP*) source;
		break;

	case dtype_blob:
		{
			thread_db* tdbb = JRD_get_thread_data();

			UCharBuffer bpb;
			bpb.resize(15);

			UCHAR* p = bpb.begin();
			*p++ = isc_bpb_version1;

			*p++ = isc_bpb_source_type;
			*p++ = 2;
			put_short(p, isc_blob_text);
			p += 2;
			*p++ = isc_bpb_source_interp;
			*p++ = 1;
			*p++ = tdbb->getAttachment()->att_charset;

			*p++ = isc_bpb_target_type;
			*p++ = 2;
			put_short(p, isc_blob_text);
			p += 2;
			*p++ = isc_bpb_target_interp;
			*p++ = 1;
			*p++ = CS_METADATA;

			bpb.shrink(p - bpb.begin());

			bid blob_id;
			blb* blob = BLB_create2(tdbb, tdbb->getTransaction(), &blob_id,
									bpb.getCount(), bpb.begin());

			length = MIN(length, MAX_USHORT);

			BLB_put_segment(tdbb, blob, (const UCHAR*) source, length);
			BLB_close(tdbb, blob);

			*(bid*) address = blob_id;
		}
		break;

	default:
		fb_assert(false);
	}

	CLEAR_NULL(record, id);
}


const char* DatabaseSnapshot::checkNull(int rid, int fid, const char* source, size_t length)
{
	// The only goal of this function is to substitute some numeric zeroes
	// and empty strings with NULLs

	switch (rid)
	{
	case rel_mon_attachments:
		switch (fid)
		{
		case f_mon_att_remote_proto:
		case f_mon_att_remote_addr:
		case f_mon_att_remote_process:
			return length ? source : NULL;
		case f_mon_att_remote_pid:
			return (*(SLONG*) source) ? source : NULL;
		default:
			break;
		}
		break;

	case rel_mon_statements:
		switch (fid)
		{
		case f_mon_stmt_att_id:
		case f_mon_stmt_tra_id:
			return (*(SLONG*) source) ? source : NULL;
		case f_mon_stmt_timestamp:
			return (*(SINT64*) source) ? source : NULL;
		case f_mon_stmt_sql_text:
			return length ? source : NULL;
		default:
			break;
		}
		break;

	case rel_mon_calls:
		switch (fid)
		{
		case f_mon_call_caller_id:
		case f_mon_call_type:
		case f_mon_call_src_line:
		case f_mon_call_src_column:
			return (*(SLONG*) source) ? source : NULL;
		case f_mon_call_name:
			return length ? source : NULL;
		default:
			break;
		}
		break;

	case rel_mon_mem_usage:
		switch (fid)
		{
		case f_mon_mem_cur_alloc:
		case f_mon_mem_max_alloc:
			return (*(SLONG*) source) ? source : NULL;
		default:
			break;
		}
		break;

	default:
		break;
	}

	return source;
}


ClumpletReader* DatabaseSnapshot::dumpData(thread_db* tdbb, bool broadcast)
{
	fb_assert(tdbb);
	Database* const dbb = tdbb->getDatabase();
	fb_assert(dbb);

	const Attachment* const self_attachment = tdbb->getAttachment();
	fb_assert(self_attachment);

	MemoryPool& pool = *tdbb->getDefaultPool();
	AutoPtr<ClumpletWriter> writer(FB_NEW(pool)
		ClumpletWriter(pool, ClumpletReader::WideUnTagged, MAX_ULONG));

	writer->insertBytes(TAG_DBB, (UCHAR*) &dbb->dbb_guid, sizeof(FB_GUID));

	// Database information

	putDatabase(dbb, *writer, fb_utils::genUniqueId());

	// Attachment information

	for (Attachment* attachment = dbb->dbb_attachments;
		attachment; attachment = attachment->att_next)
	{
		if (broadcast || attachment == self_attachment)
		{
			putAttachment(attachment, *writer, fb_utils::genUniqueId());
			putContextVars(attachment->att_context_vars, *writer,
						   attachment->att_attachment_id, true);

			jrd_tra* transaction = NULL;
			const jrd_req* request = NULL;

			// Transaction information

			for (transaction = attachment->att_transactions;
				transaction; transaction = transaction->tra_next)
			{
				putTransaction(transaction, *writer, fb_utils::genUniqueId());
				putContextVars(transaction->tra_context_vars, *writer,
							   transaction->tra_number, false);
			}

			// Request information

			for (request = attachment->att_requests;
				request; request = request->req_request)
			{
				if (!(request->req_flags & (req_internal | req_sys_trigger)))
				{
					putRequest(request, *writer, fb_utils::genUniqueId());
				}
			}

			// Call stack information

			for (transaction = attachment->att_transactions;
				transaction; transaction = transaction->tra_next)
			{
				for (request = transaction->tra_requests;
					request; request = request->req_tra_next)
				{
					if (!(request->req_flags & (req_internal | req_sys_trigger)) &&
						request->req_caller)
					{
						putCall(request, *writer, fb_utils::genUniqueId());
					}
				}
			}
		}
	}

	if (broadcast)
	{
		if (!dump)
		{
			// Initialize the shared memory region
			MutexLockGuard guard(initMutex);
			if (!dump)
			{
				dump = FB_NEW(*getDefaultMemoryPool()) SharedMemory;
			}
		}

		DumpGuard guard(dump);
		dump->writeData(tdbb, *writer);

		return NULL;
	}

	return writer.release();
}


void DatabaseSnapshot::putDatabase(const Database* database,
								   ClumpletWriter& writer,
								   int stat_id)
{
	fb_assert(database);

	writer.insertByte(TAG_RECORD, rel_mon_database);

	// Reload header
	const PageSpace* const pageSpace =
		database->dbb_page_manager.findPageSpace(DB_PAGE_SPACE);
	PAG_header(true);

	// database name or alias
	writer.insertPath(f_mon_db_name, database->dbb_database_name);
	// page size
	writer.insertInt(f_mon_db_page_size, database->dbb_page_size);
	// major ODS version
	writer.insertInt(f_mon_db_ods_major, database->dbb_ods_version);
	// minor ODS version
	writer.insertInt(f_mon_db_ods_minor, database->dbb_minor_version);
	// oldest interesting transaction
	writer.insertInt(f_mon_db_oit, database->dbb_oldest_transaction);
	// oldest active transaction
	writer.insertInt(f_mon_db_oat, database->dbb_oldest_active);
	// oldest snapshot transaction
	writer.insertInt(f_mon_db_ost, database->dbb_oldest_snapshot);
	// next transaction
	writer.insertInt(f_mon_db_nt, database->dbb_next_transaction);
	// number of page buffers
	writer.insertInt(f_mon_db_page_bufs, database->dbb_bcb->bcb_count);

	int temp;

	// SQL dialect
	temp = (database->dbb_flags & DBB_DB_SQL_dialect_3) ? 3 : 1;
	writer.insertInt(f_mon_db_dialect, temp);
	
	// shutdown mode
	if (database->dbb_ast_flags & DBB_shutdown_full)
		temp = shut_mode_full;
	else if (database->dbb_ast_flags & DBB_shutdown_single)
		temp = shut_mode_single;
	else if (database->dbb_ast_flags & DBB_shutdown)
		temp = shut_mode_multi;
	else
		temp = shut_mode_online;
	writer.insertInt(f_mon_db_shut_mode, temp);

	// sweep interval
	writer.insertInt(f_mon_db_sweep_int, database->dbb_sweep_interval);
	// read only flag
	temp = (database->dbb_flags & DBB_read_only) ? 1 : 0;
	writer.insertInt(f_mon_db_read_only, temp);
	// forced writes flag
	temp = (database->dbb_flags & DBB_force_write) ? 1 : 0;
	writer.insertInt(f_mon_db_forced_writes, temp);
	// reserve space flag
	temp = (database->dbb_flags & DBB_no_reserve) ? 0 : 1;
	writer.insertInt(f_mon_db_res_space, temp);
	// creation date
	writer.insertBytes(f_mon_db_created,
					   (UCHAR*) &database->dbb_creation_date.value(),
					   sizeof(ISC_TIMESTAMP));
	// database size
	writer.insertBigInt(f_mon_db_pages, PageSpace::actAlloc(database));

	// database state
	thread_db* tdbb = JRD_get_thread_data();

	{	// scope
		BackupManager::SharedDatabaseHolder sdbHolder(tdbb, database->dbb_backup_manager);

		switch (database->dbb_backup_manager->get_state())
		{
			case nbak_state_normal:
				temp = backup_state_normal;
				break;
			case nbak_state_stalled:
				temp = backup_state_stalled;
				break;
			case nbak_state_merge:
				temp = backup_state_merge;
				break;
			default:
				fb_assert(false);
		}
	}
	writer.insertInt(f_mon_db_backup_state, temp);
	// statistics
	writer.insertBigInt(f_mon_db_stat_id, getGlobalId(stat_id));
	putStatistics(database->dbb_stats, writer, stat_id, stat_database);
	putMemoryUsage(database->dbb_memory_stats, writer, stat_id, stat_database);
}


void DatabaseSnapshot::putAttachment(const Attachment* attachment,
									 ClumpletWriter& writer,
									 int stat_id)
{
	fb_assert(attachment);

	writer.insertByte(TAG_RECORD, rel_mon_attachments);

	int temp = mon_state_idle;

	for (const jrd_tra* transaction_itr = attachment->att_transactions;
		 transaction_itr; transaction_itr = transaction_itr->tra_next)
	{
		if (transaction_itr->tra_requests)
		{
			temp = mon_state_active;
			break;
		}
	}

	// attachment id
	writer.insertInt(f_mon_att_id, attachment->att_attachment_id);
	// process id
	writer.insertInt(f_mon_att_server_pid, getpid());
	// state
	writer.insertInt(f_mon_att_state, temp);
	// attachment name
	writer.insertPath(f_mon_att_name, attachment->att_filename);
	// user
	writer.insertString(f_mon_att_user,
						attachment->att_user->usr_user_name);
	// role
	writer.insertString(f_mon_att_role,
						attachment->att_user->usr_sql_role_name);
	// remote protocol
	writer.insertString(f_mon_att_remote_proto,
						attachment->att_network_protocol);
	// remote address
	writer.insertString(f_mon_att_remote_addr,
						attachment->att_remote_address);
	// remote process id
	writer.insertInt(f_mon_att_remote_pid, attachment->att_remote_pid);
	// remote process name
	writer.insertPath(f_mon_att_remote_process, attachment->att_remote_process);
	// charset
	writer.insertInt(f_mon_att_charset_id, attachment->att_charset);
	// timestamp
	writer.insertBytes(f_mon_att_timestamp,
					   (UCHAR*) &attachment->att_timestamp.value(),
					   sizeof(ISC_TIMESTAMP));
	// garbage collection flag
	temp = (attachment->att_flags & ATT_no_cleanup) ? 0 : 1;
	writer.insertInt(f_mon_att_gc, temp);
	// statistics
	writer.insertBigInt(f_mon_att_stat_id, getGlobalId(stat_id));
	putStatistics(attachment->att_stats, writer, stat_id, stat_attachment);
	putMemoryUsage(attachment->att_memory_stats, writer, stat_id, stat_attachment);
}


void DatabaseSnapshot::putTransaction(const jrd_tra* transaction,
									  ClumpletWriter& writer,
									  int stat_id)
{
	fb_assert(transaction);

	writer.insertByte(TAG_RECORD, rel_mon_transactions);

	int temp;

	// transaction id
	writer.insertInt(f_mon_tra_id, transaction->tra_number);
	// attachment id
	writer.insertInt(f_mon_tra_att_id,
					 transaction->tra_attachment->att_attachment_id);
	// state
	temp = transaction->tra_requests ? mon_state_active : mon_state_idle;
	writer.insertInt(f_mon_tra_state, temp);
	// timestamp
	writer.insertBytes(f_mon_tra_timestamp,
					   (UCHAR*) &transaction->tra_timestamp.value(),
					   sizeof(ISC_TIMESTAMP));
	// top transaction
	writer.insertInt(f_mon_tra_top, transaction->tra_top);
	// oldest transaction
	writer.insertInt(f_mon_tra_oit, transaction->tra_oldest);
	// oldest active transaction
	writer.insertInt(f_mon_tra_oat, transaction->tra_oldest_active);
	// isolation mode
	if (transaction->tra_flags & TRA_degree3)
		temp = iso_mode_consistency;
	else if (transaction->tra_flags & TRA_read_committed)
	{
		temp = (transaction->tra_flags &  TRA_rec_version) ?
			iso_mode_rc_version : iso_mode_rc_no_version;
	}
	else
		temp = iso_mode_concurrency;
	writer.insertInt(f_mon_tra_iso_mode, temp);
	// lock timeout
	writer.insertInt(f_mon_tra_lock_timeout,
					 transaction->tra_lock_timeout);
	// read only flag
	temp = (transaction->tra_flags & TRA_readonly) ? 1 : 0;
	writer.insertInt(f_mon_tra_read_only, temp);
	// autocommit flag
	temp = (transaction->tra_flags & TRA_autocommit) ? 1 : 0;
	writer.insertInt(f_mon_tra_auto_commit, temp);
	// auto undo flag
	temp = (transaction->tra_flags & TRA_no_auto_undo) ? 0 : 1;
	writer.insertInt(f_mon_tra_auto_undo, temp);
	// statistics
	writer.insertBigInt(f_mon_tra_stat_id, getGlobalId(stat_id));
	putStatistics(transaction->tra_stats, writer, stat_id, stat_transaction);
	putMemoryUsage(transaction->tra_memory_stats, writer, stat_id, stat_transaction);
}


void DatabaseSnapshot::putRequest(const jrd_req* request,
								  ClumpletWriter& writer,
								  int stat_id)
{
	fb_assert(request);

	writer.insertByte(TAG_RECORD, rel_mon_statements);

	// request id
	writer.insertBigInt(f_mon_stmt_id, getGlobalId(request->req_id));
	// attachment id
	if (request->req_attachment) {
		writer.insertInt(f_mon_stmt_att_id,
						 request->req_attachment->att_attachment_id);
	}
	else {
		writer.insertInt(f_mon_stmt_att_id, 0);
	}
	// state, transaction ID, timestamp
	if (request->req_flags & req_active) {
		writer.insertInt(f_mon_stmt_state, mon_state_active);
		const int tra_id = request->req_transaction ?
			request->req_transaction->tra_number : 0;
		writer.insertInt(f_mon_stmt_tra_id, tra_id);
		writer.insertBytes(f_mon_stmt_timestamp,
						   (UCHAR*) &request->req_timestamp.value(),
						   sizeof(ISC_TIMESTAMP));
	}
	else {
		writer.insertInt(f_mon_stmt_state, mon_state_idle);
		writer.insertInt(f_mon_stmt_tra_id, 0);
		ISC_TIMESTAMP empty = {0, 0};
		writer.insertBytes(f_mon_stmt_timestamp,
						   (UCHAR*) &empty,
						   sizeof(ISC_TIMESTAMP));
	}
	// sql text
	writer.insertString(f_mon_stmt_sql_text, request->req_sql_text);
	// statistics
	writer.insertBigInt(f_mon_stmt_stat_id, getGlobalId(stat_id));
	putStatistics(request->req_stats, writer, stat_id, stat_statement);
	putMemoryUsage(request->req_memory_stats, writer, stat_id, stat_statement);
}


void DatabaseSnapshot::putCall(const jrd_req* request,
							   ClumpletWriter& writer,
							   int stat_id)
{
	fb_assert(request);

	const jrd_req* statement = request->req_caller;
	while (statement->req_caller)
		statement = statement->req_caller;
	fb_assert(statement);

	writer.insertByte(TAG_RECORD, rel_mon_calls);

	// call id
	writer.insertBigInt(f_mon_call_id, getGlobalId(request->req_id));
	// statement id
	writer.insertBigInt(f_mon_call_stmt_id, getGlobalId(statement->req_id));
	// caller id
	if (statement == request->req_caller) {
		writer.insertInt(f_mon_call_caller_id, 0);
	}
	else {
		writer.insertInt(f_mon_call_caller_id,
						 request->req_caller->req_id);
	}
	// object name/type
	if (request->req_procedure) {
		writer.insertString(f_mon_call_name,
							request->req_procedure->prc_name.c_str());
		writer.insertInt(f_mon_call_type, obj_procedure);
	}
	else if (!request->req_trg_name.isEmpty()) {
		writer.insertString(f_mon_call_name,
							request->req_trg_name.c_str());
		writer.insertInt(f_mon_call_type, obj_trigger);
	}
	else {
		// we should never be here...
		fb_assert(false);
		// ... but just in case it happened...
		writer.insertString(f_mon_call_name, "");
		writer.insertInt(f_mon_call_type, 0);
	}
	// timestamp
	writer.insertBytes(f_mon_call_timestamp,
					   (const UCHAR*) &request->req_timestamp.value(),
					   sizeof(ISC_TIMESTAMP));
	// source line/column
	writer.insertInt(f_mon_call_src_line, request->req_src_line);
	writer.insertInt(f_mon_call_src_column, request->req_src_column);
	// statistics
	writer.insertBigInt(f_mon_call_stat_id, getGlobalId(stat_id));
	putStatistics(request->req_stats, writer, stat_id, stat_call);
	putMemoryUsage(request->req_memory_stats, writer, stat_id, stat_call);
}

void DatabaseSnapshot::putStatistics(const RuntimeStatistics& statistics,
									 Firebird::ClumpletWriter& writer,
									 int stat_id,
									 int stat_group)
{
	// statistics id
	const SINT64 id = getGlobalId(stat_id);

	// physical I/O statistics
	writer.insertByte(TAG_RECORD, rel_mon_io_stats);
	writer.insertBigInt(f_mon_io_stat_id, id);
	writer.insertInt(f_mon_io_stat_group, stat_group);
	writer.insertBigInt(f_mon_io_page_reads,
						statistics.getValue(RuntimeStatistics::PAGE_READS));
	writer.insertBigInt(f_mon_io_page_writes,
						statistics.getValue(RuntimeStatistics::PAGE_WRITES));
	writer.insertBigInt(f_mon_io_page_fetches,
						statistics.getValue(RuntimeStatistics::PAGE_FETCHES));
	writer.insertBigInt(f_mon_io_page_marks,
						statistics.getValue(RuntimeStatistics::PAGE_MARKS));

	// logical I/O statistics
	writer.insertByte(TAG_RECORD, rel_mon_rec_stats);
	writer.insertBigInt(f_mon_rec_stat_id, id);
	writer.insertInt(f_mon_rec_stat_group, stat_group);
	writer.insertBigInt(f_mon_rec_seq_reads,
						statistics.getValue(RuntimeStatistics::RECORD_SEQ_READS));
	writer.insertBigInt(f_mon_rec_idx_reads,
						statistics.getValue(RuntimeStatistics::RECORD_IDX_READS));
	writer.insertBigInt(f_mon_rec_inserts,
						statistics.getValue(RuntimeStatistics::RECORD_INSERTS));
	writer.insertBigInt(f_mon_rec_updates,
						statistics.getValue(RuntimeStatistics::RECORD_UPDATES));
	writer.insertBigInt(f_mon_rec_deletes,
						statistics.getValue(RuntimeStatistics::RECORD_DELETES));
	writer.insertBigInt(f_mon_rec_backouts,
						statistics.getValue(RuntimeStatistics::RECORD_BACKOUTS));
	writer.insertBigInt(f_mon_rec_purges,
						statistics.getValue(RuntimeStatistics::RECORD_PURGES));
	writer.insertBigInt(f_mon_rec_expunges,
						statistics.getValue(RuntimeStatistics::RECORD_EXPUNGES));
}

void DatabaseSnapshot::putContextVars(Firebird::StringMap& variables,
									  Firebird::ClumpletWriter& writer,
									  int object_id, bool is_attachment)
{
	for (bool found = variables.getFirst(); found; found = variables.getNext())
	{
		writer.insertByte(TAG_RECORD, rel_mon_ctx_vars);

		if (is_attachment)
			writer.insertInt(f_mon_ctx_var_att_id, object_id);
		else
			writer.insertInt(f_mon_ctx_var_tra_id, object_id);

		writer.insertString(f_mon_ctx_var_name, variables.current()->first);
		writer.insertString(f_mon_ctx_var_value, variables.current()->second);
	}
}

void DatabaseSnapshot::putMemoryUsage(const Firebird::MemoryStats& stats,
									  Firebird::ClumpletWriter& writer,
									  int stat_id,
									  int stat_group)
{
	// statistics id
	const SINT64 id = getGlobalId(stat_id);

	// memory usage
	writer.insertByte(TAG_RECORD, rel_mon_mem_usage);
	writer.insertBigInt(f_mon_mem_stat_id, id);
	writer.insertInt(f_mon_mem_stat_group, stat_group);
	writer.insertBigInt(f_mon_mem_cur_used, stats.getCurrentUsage());
	writer.insertBigInt(f_mon_mem_cur_alloc, stats.getCurrentMapping());
	writer.insertBigInt(f_mon_mem_max_used, stats.getMaximumUsage());
	writer.insertBigInt(f_mon_mem_max_alloc, stats.getMaximumMapping());
}
