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

const UCHAR TAG_RECORD = MAX_UCHAR;


// SharedMemory class

DatabaseSnapshot::SharedData::SharedData(const Database* dbb)
	: process_id(getpid()), local_id(dbb->dbb_monitoring_id)
{
	string name;
	name.printf(MONITOR_FILE, dbb->getUniqueFileId().c_str());

	ISC_STATUS_ARRAY statusVector;
	base = (Header*) ISC_map_file(statusVector, name.c_str(), init, this, DEFAULT_SIZE, &handle);
	if (!base)
	{
		iscLogStatus("Cannot initialize the shared memory region", statusVector);
		status_exception::raise(statusVector);
	}

	fb_assert(base->version == MONITOR_VERSION);
}


DatabaseSnapshot::SharedData::~SharedData()
{
	cleanup();

#ifdef WIN_NT
	ISC_mutex_fini(&mutex);
#else
	ISC_mutex_fini(&base->mutex);
#endif

	ISC_STATUS_ARRAY statusVector;
	ISC_unmap_file(statusVector, &handle);
}


void DatabaseSnapshot::SharedData::acquire()
{
#ifdef WIN_NT
	checkMutex("lock", ISC_mutex_lock(&mutex));
#else
	checkMutex("lock", ISC_mutex_lock(&base->mutex));
#endif
	if (base->allocated > handle.sh_mem_length_mapped)
	{
#if (defined HAVE_MMAP || defined WIN_NT)
		ISC_STATUS_ARRAY statusVector;
		base = (Header*) ISC_remap_file(statusVector, &handle, base->allocated, false);
		if (!base)
		{
			status_exception::raise(statusVector);
		}
#else
		status_exception::raise(Arg::Gds(isc_montabexh));
#endif
	}
}


void DatabaseSnapshot::SharedData::release()
{
#ifdef WIN_NT
	checkMutex("unlock", ISC_mutex_unlock(&mutex));
#else
	checkMutex("unlock", ISC_mutex_unlock(&base->mutex));
#endif
}


UCHAR* DatabaseSnapshot::SharedData::read(MemoryPool& pool, ULONG& resultSize)
{
	DumpGuard guard(this);

	ULONG self_dbb_offset = 0;

	// Garbage collect elements belonging to dead processes.
	// This is done in two passes. First, we compact the data
	// and calculate the total size of the resulting data.
	// Second, we create a resulting buffer of the necessary size
	// and copy the data there, starting with our own dbb.

	// First pass
	for (ULONG offset = sizeof(Header); offset < base->used;)
	{
		UCHAR* const ptr = (UCHAR*) base + offset;
		const Element* const element = (Element*) ptr;
		const ULONG length = sizeof(Element) + element->length;

		if (element->processId == process_id && element->localId == local_id)
		{
			self_dbb_offset = offset;
		}

		if (ISC_check_process_existence(element->processId))
		{
			resultSize += element->length;
			offset += length;
		}
		else
		{
			fb_assert(base->used >= offset + length);
			memmove(ptr, ptr + length, base->used - offset - length);
			base->used -= length;
		}
	}

	// Second pass
	UCHAR* const buffer = FB_NEW(pool) UCHAR[resultSize];
	UCHAR* bufferPtr(buffer);

	fb_assert(self_dbb_offset);

	UCHAR* const ptr = (UCHAR*) base + self_dbb_offset;
	const Element* const element = (Element*) ptr;
	memcpy(bufferPtr, ptr + sizeof(Element), element->length);
	bufferPtr += element->length;

	for (ULONG offset = sizeof(Header); offset < base->used;)
	{
		UCHAR* const ptr = (UCHAR*) base + offset;
		const Element* const element = (Element*) ptr;
		const ULONG length = sizeof(Element) + element->length;

		if (offset != self_dbb_offset)
		{
			memcpy(bufferPtr, ptr + sizeof(Element), element->length);
			bufferPtr += element->length;
		}

		offset += length;
	}

	fb_assert(buffer + resultSize == bufferPtr);
	return buffer;
}


void DatabaseSnapshot::SharedData::write(ULONG length, const UCHAR* buffer)
{
	DumpGuard guard(this);

	// Remove old copies of our element, if any
	cleanup();

	// Do we need to extend the allocated memory?
	while (base->used + sizeof(Element) + length > base->allocated)
	{
		extend();
	}

	// Put an up-to-date element at the tail
	UCHAR* const ptr = (UCHAR*) base + base->used;
	Element* const element = (Element*) ptr;
	element->processId = process_id;
	element->localId = local_id;
	element->length = length;
	memcpy(ptr + sizeof(Element), buffer, length);
	base->used += sizeof(Element) + length;
}


void DatabaseSnapshot::SharedData::cleanup()
{
	// Remove information about our dbb
	for (ULONG offset = sizeof(Header); offset < base->used;)
	{
		UCHAR* const ptr = (UCHAR*) base + offset;
		const Element* const element = (Element*) ptr;
		const ULONG length = sizeof(Element) + element->length;

		if (element->processId == process_id && element->localId == local_id)
		{
			fb_assert(base->used >= offset + length);
			memmove(ptr, ptr + length, base->used - offset - length);
			base->used -= length;
		}
		else
		{
			offset += length;
		}
	}
}


void DatabaseSnapshot::SharedData::extend()
{
	const ULONG newSize = handle.sh_mem_length_mapped + DEFAULT_SIZE;

#if (defined HAVE_MMAP || defined WIN_NT)
	ISC_STATUS_ARRAY statusVector;
	base = (Header*) ISC_remap_file(statusVector, &handle, newSize, true);
	if (!base)
	{
		status_exception::raise(statusVector);
	}
	base->allocated = handle.sh_mem_length_mapped;
#else
	status_exception::raise(Arg::Gds(isc_montabexh));
#endif
}


void DatabaseSnapshot::SharedData::checkMutex(const TEXT* string, int state)
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


void DatabaseSnapshot::SharedData::init(void* arg, SH_MEM_T* shmemData, bool initialize)
{
	SharedData* const shmem = (SharedData*) arg;
	fb_assert(shmem);

#ifdef WIN_NT
	checkMutex("init", ISC_mutex_init(&shmem->mutex, shmemData->sh_mem_name));
#endif

	if (!initialize)
		return;

	// Initialize the shared data header
	Header* const header = (Header*) shmemData->sh_mem_address;
	header->version = MONITOR_VERSION;
	header->used = sizeof(Header);
	header->allocated = shmemData->sh_mem_length_mapped;

#ifndef WIN_NT
	checkMutex("init", ISC_mutex_init(&header->mutex));
#endif
}


// DatabaseSnapshot class


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
		transaction->tra_db_snapshot = FB_NEW(pool) DatabaseSnapshot(tdbb, pool);
	}

	return transaction->tra_db_snapshot;
}


int DatabaseSnapshot::blockingAst(void* ast_object)
{
	Database* dbb = static_cast<Database*>(ast_object);

	try
	{
		Lock* const lock = dbb->dbb_monitor_lock;

		Database::SyncGuard dsGuard(dbb, true);

		ThreadContextHolder tdbb;
		tdbb->setDatabase(lock->lck_dbb);
		tdbb->setAttachment(lock->lck_attachment);

		ContextPoolHolder context(tdbb, dbb->dbb_permanent);

		if (!(dbb->dbb_ast_flags & DBB_monitor_off))
		{
			try {
				// Write the data to the shared memory
				dumpData(tdbb);

				// Release the lock and mark dbb as requesting a new one
				LCK_release(tdbb, lock);
				dbb->dbb_ast_flags |= DBB_monitor_off;
			}
			catch (const Exception&) {
				gds__log("Unexpected exception at the AST level");
			}
		}
	}
	catch (const Exception&)
	{} // no-op

	return 0;
}


DatabaseSnapshot::DatabaseSnapshot(thread_db* tdbb, MemoryPool& pool)
	: snapshot(pool), idMap(pool), idCounter(0)
{
	SET_TDBB(tdbb);

	PAG_header(tdbb, true);

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

	// Release our own lock
	LCK_release(tdbb, dbb->dbb_monitor_lock);

	// Dump our own data
	dumpData(tdbb);

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

	// Mark dbb as requesting a new lock
	dbb->dbb_ast_flags |= DBB_monitor_off;

	// Read the shared memory
	fb_assert(dbb->dbb_monitoring_data);
	ULONG dataSize = 0;
	AutoPtr<UCHAR, ArrayDelete<UCHAR> > data(dbb->dbb_monitoring_data->read(pool, dataSize));
	fb_assert(dataSize);

	ClumpletReader reader(ClumpletReader::WideUnTagged, data, dataSize);

	const Attachment* const attachment = tdbb->getAttachment();
	fb_assert(attachment);
	const PathName& databaseName = dbb->dbb_database_name;
	const string& userName = attachment->att_user->usr_user_name;
	const bool locksmith = attachment->locksmith();

	// Parse the dump
	RecordBuffer* buffer = NULL;
	Record* record = NULL;

	int rid = 0;
	bool dbb_processed = false, fields_processed = false;
	bool dbb_allowed = false, att_allowed = false;

	for (reader.rewind(); !reader.isEof(); reader.moveNext())
	{
		if (reader.getClumpTag() == TAG_RECORD)
		{
			rid = reader.getInt();

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
			const int fid = reader.getClumpTag();
			const size_t length = reader.getClumpLength();

			const char* source = checkNull(rid, fid, (char*) reader.getBytes(), length);

			// special case for MON$DATABASE
			if (rid == rel_mon_database)
			{
				if (fid == f_mon_db_name)
				{
					dbb_allowed = !databaseName.compare(source, length);
				}

				if (record && dbb_allowed && !dbb_processed)
				{
					putField(record, fid, reader, source == NULL);
					fields_processed = true;
				}

				att_allowed = (dbb_allowed && !dbb_processed);
			}
			// special case for MON$ATTACHMENTS
			else if (rid == rel_mon_attachments)
			{
				if (fid == f_mon_att_user)
				{
					att_allowed = locksmith || !userName.compare(source, length);
				}

				if (record && dbb_allowed && att_allowed)
				{
					putField(record, fid, reader, source == NULL);
					fields_processed = true;
					dbb_processed = true;
				}
			}
			// generic logic that covers all other relations
			else if (record && dbb_allowed && att_allowed)
			{
				putField(record, fid, reader, source == NULL);
				fields_processed = true;
				dbb_processed = true;
			}
		}
	}

	if (fields_processed)
	{
		buffer->store(record);
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


RecordBuffer* DatabaseSnapshot::allocBuffer(thread_db* tdbb, MemoryPool& pool, int rel_id)
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


void DatabaseSnapshot::putField(Record* record, int id, const ClumpletReader& reader, bool makeNull)
{
	fb_assert(record);

	const Format* const format = record->rec_format;
	fb_assert(format && id < format->fmt_count);

	if (makeNull)
	{
		SET_NULL(record, id);
		return;
	}

	const dsc desc = format->fmt_desc[id];
	UCHAR* const address = record->rec_data + (IPTR) desc.dsc_address;

	if (reader.getClumpLength() == sizeof(SINT64) && desc.dsc_dtype == dtype_long)
	{
		// special case: translate 64-bit global ID into 32-bit local ID
		const SINT64 global_id = reader.getBigInt();
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

	switch (desc.dsc_dtype)
	{
	case dtype_text:
		{
			const char* const string = (char*) reader.getBytes();
			const size_t max_length = desc.dsc_length;
			const size_t length = MIN(reader.getClumpLength(), max_length);
			memcpy(address, string, length);
			memset(address + length, ' ', max_length - length);
		}
		break;
	case dtype_varying:
		{
			const char* const string = (char*) reader.getBytes();
			const size_t max_length = desc.dsc_length - sizeof(USHORT);
			const size_t length = MIN(reader.getClumpLength(), max_length);
			vary* varying = (vary*) address;
			varying->vary_length = length;
			memcpy(varying->vary_string, string, length);
		}
		break;

	case dtype_short:
		*(SSHORT*) address = reader.getBigInt();
		break;
	case dtype_long:
		*(SLONG*) address = reader.getBigInt();
		break;
	case dtype_int64:
		*(SINT64*) address = reader.getBigInt();
		break;

	case dtype_real:
		*(float*) address = reader.getDouble();
		break;
	case dtype_double:
		*(double*) address = reader.getDouble();
		break;

	case dtype_sql_date:
		*(ISC_DATE*) address = reader.getDate();
		break;
	case dtype_sql_time:
		*(ISC_TIME*) address = reader.getTime();
		break;
	case dtype_timestamp:
		*(ISC_TIMESTAMP*) address = reader.getTimeStamp();
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
			put_vax_short(p, isc_blob_text);
			p += 2;
			*p++ = isc_bpb_source_interp;
			*p++ = 1;
			*p++ = tdbb->getAttachment()->att_charset;

			*p++ = isc_bpb_target_type;
			*p++ = 2;
			put_vax_short(p, isc_blob_text);
			p += 2;
			*p++ = isc_bpb_target_interp;
			*p++ = 1;
			*p++ = CS_METADATA;

			bpb.shrink(p - bpb.begin());

			bid blob_id;
			blb* blob = BLB_create2(tdbb, tdbb->getTransaction(), &blob_id,
									bpb.getCount(), bpb.begin());

			const size_t length = MIN(reader.getClumpLength(), MAX_USHORT);

			BLB_put_segment(tdbb, blob, reader.getBytes(), length);
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
			return (*(SINT64*) source) ? source : NULL;
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


void DatabaseSnapshot::dumpData(thread_db* tdbb)
{
	fb_assert(tdbb);

	Database* const dbb = tdbb->getDatabase();
	fb_assert(dbb);

	ClumpletWriter writer(ClumpletReader::WideUnTagged, MAX_ULONG);

	// Database information

	putDatabase(dbb, writer, fb_utils::genUniqueId());

	// Attachment information

	for (Attachment* attachment = dbb->dbb_attachments; attachment; attachment = attachment->att_next)
	{
		putAttachment(attachment, writer, fb_utils::genUniqueId());
		putContextVars(attachment->att_context_vars, writer, attachment->att_attachment_id, true);

		jrd_tra* transaction = NULL;
		jrd_req* request = NULL;

		// Transaction information

		for (transaction = attachment->att_transactions;
			transaction; transaction = transaction->tra_next)
		{
			putTransaction(transaction, writer, fb_utils::genUniqueId());
			putContextVars(transaction->tra_context_vars, writer, transaction->tra_number, false);
		}

		// Call stack information

		for (transaction = attachment->att_transactions;
			transaction; transaction = transaction->tra_next)
		{
			for (request = transaction->tra_requests; request; request = request->req_caller)
			{
				request->adjustCallerStats();

				if (!(request->req_flags & (req_internal | req_sys_trigger)) && request->req_caller)
				{
					putCall(request, writer, fb_utils::genUniqueId());
				}
			}
		}

		// Request information

		for (request = attachment->att_requests; request; request = request->req_request)
		{
			if (!(request->req_flags & (req_internal | req_sys_trigger)))
			{
				putRequest(request, writer, fb_utils::genUniqueId());
			}
		}
	}

	if (!dbb->dbb_monitoring_data)
	{
		dbb->dbb_monitoring_data = FB_NEW(*dbb->dbb_permanent) SharedData(dbb);
	}

	dbb->dbb_monitoring_data->write(writer.getBufferLength(), writer.getBuffer());
}


SINT64 DatabaseSnapshot::getGlobalId(int value)
{
	return ((SINT64) getpid() << BITS_PER_LONG) + value;
}


void DatabaseSnapshot::putDatabase(const Database* database, ClumpletWriter& writer, int stat_id)
{
	fb_assert(database);

	writer.insertByte(TAG_RECORD, rel_mon_database);

	// Reload header
	// CVC: I don't see the need for this call. If it produces a side effect that's necessary here,
	// then calling the function without assigning its result value may be clearer.
	//const PageSpace* const pageSpace = database->dbb_page_manager.findPageSpace(DB_PAGE_SPACE);

	// database name or alias (MUST BE ALWAYS THE FIRST ITEM PASSED!)
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
	writer.insertTimeStamp(f_mon_db_created, database->dbb_creation_date.value());
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


void DatabaseSnapshot::putAttachment(const Attachment* attachment, ClumpletWriter& writer, int stat_id)
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

	// user (MUST BE ALWAYS THE FIRST ITEM PASSED!)
	writer.insertString(f_mon_att_user, attachment->att_user->usr_user_name);
	// attachment id
	writer.insertInt(f_mon_att_id, attachment->att_attachment_id);
	// process id
	writer.insertInt(f_mon_att_server_pid, getpid());
	// state
	writer.insertInt(f_mon_att_state, temp);
	// attachment name
	writer.insertPath(f_mon_att_name, attachment->att_filename);
	// role
	writer.insertString(f_mon_att_role, attachment->att_user->usr_sql_role_name);
	// remote protocol
	writer.insertString(f_mon_att_remote_proto, attachment->att_network_protocol);
	// remote address
	writer.insertString(f_mon_att_remote_addr, attachment->att_remote_address);
	// remote process id
	writer.insertInt(f_mon_att_remote_pid, attachment->att_remote_pid);
	// remote process name
	writer.insertPath(f_mon_att_remote_process, attachment->att_remote_process);
	// charset
	writer.insertInt(f_mon_att_charset_id, attachment->att_charset);
	// timestamp
	writer.insertTimeStamp(f_mon_att_timestamp, attachment->att_timestamp.value());
	// garbage collection flag
	temp = (attachment->att_flags & ATT_no_cleanup) ? 0 : 1;
	writer.insertInt(f_mon_att_gc, temp);
	// statistics
	writer.insertBigInt(f_mon_att_stat_id, getGlobalId(stat_id));
	putStatistics(attachment->att_stats, writer, stat_id, stat_attachment);
	putMemoryUsage(attachment->att_memory_stats, writer, stat_id, stat_attachment);
}


void DatabaseSnapshot::putTransaction(const jrd_tra* transaction, ClumpletWriter& writer, int stat_id)
{
	fb_assert(transaction);

	writer.insertByte(TAG_RECORD, rel_mon_transactions);

	int temp;

	// transaction id
	writer.insertInt(f_mon_tra_id, transaction->tra_number);
	// attachment id
	writer.insertInt(f_mon_tra_att_id, transaction->tra_attachment->att_attachment_id);
	// state
	temp = transaction->tra_requests ? mon_state_active : mon_state_idle;
	writer.insertInt(f_mon_tra_state, temp);
	// timestamp
	writer.insertTimeStamp(f_mon_tra_timestamp, transaction->tra_timestamp.value());
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


void DatabaseSnapshot::putRequest(const jrd_req* request, ClumpletWriter& writer, int stat_id)
{
	fb_assert(request);

	writer.insertByte(TAG_RECORD, rel_mon_statements);

	// request id
	writer.insertBigInt(f_mon_stmt_id, getGlobalId(request->req_id));
	// attachment id
	if (request->req_attachment) {
		writer.insertInt(f_mon_stmt_att_id, request->req_attachment->att_attachment_id);
	}
	else {
		writer.insertInt(f_mon_stmt_att_id, 0);
	}
	// state, transaction ID, timestamp
	if (request->req_flags & req_active) {
		const bool is_stalled = (request->req_flags & req_stall);
		writer.insertInt(f_mon_stmt_state, is_stalled ? mon_state_stalled : mon_state_active);
		const int tra_id = request->req_transaction ? request->req_transaction->tra_number : 0;
		writer.insertInt(f_mon_stmt_tra_id, tra_id);
		writer.insertTimeStamp(f_mon_stmt_timestamp, request->req_timestamp.value());
	}
	else {
		writer.insertInt(f_mon_stmt_state, mon_state_idle);
		writer.insertInt(f_mon_stmt_tra_id, 0);
		ISC_TIMESTAMP empty = {0, 0};
		writer.insertTimeStamp(f_mon_stmt_timestamp, empty);
	}
	// sql text
	writer.insertString(f_mon_stmt_sql_text, request->req_sql_text ? *request->req_sql_text : string());
	// statistics
	writer.insertBigInt(f_mon_stmt_stat_id, getGlobalId(stat_id));
	putStatistics(request->req_stats, writer, stat_id, stat_statement);
	putMemoryUsage(request->req_memory_stats, writer, stat_id, stat_statement);
}


void DatabaseSnapshot::putCall(const jrd_req* request, ClumpletWriter& writer, int stat_id)
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
		writer.insertBigInt(f_mon_call_caller_id, 0);
	}
	else {
		writer.insertBigInt(f_mon_call_caller_id, getGlobalId(request->req_caller->req_id));
	}
	// object name/type
	if (request->req_procedure) {
		writer.insertString(f_mon_call_name, request->req_procedure->prc_name.c_str());
		writer.insertInt(f_mon_call_type, obj_procedure);
	}
	else if (!request->req_trg_name.isEmpty()) {
		writer.insertString(f_mon_call_name, request->req_trg_name.c_str());
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
	writer.insertTimeStamp(f_mon_call_timestamp, request->req_timestamp.value());
	// source line/column
	writer.insertInt(f_mon_call_src_line, request->req_src_line);
	writer.insertInt(f_mon_call_src_column, request->req_src_column);
	// statistics
	writer.insertBigInt(f_mon_call_stat_id, getGlobalId(stat_id));
	putStatistics(request->req_stats, writer, stat_id, stat_call);
	putMemoryUsage(request->req_memory_stats, writer, stat_id, stat_call);
}

void DatabaseSnapshot::putStatistics(const RuntimeStatistics& statistics,
									 ClumpletWriter& writer,
									 int stat_id,
									 int stat_group)
{
	// statistics id
	const SINT64 id = getGlobalId(stat_id);

	// physical I/O statistics
	writer.insertByte(TAG_RECORD, rel_mon_io_stats);
	writer.insertBigInt(f_mon_io_stat_id, id);
	writer.insertInt(f_mon_io_stat_group, stat_group);
	writer.insertBigInt(f_mon_io_page_reads, statistics.getValue(RuntimeStatistics::PAGE_READS));
	writer.insertBigInt(f_mon_io_page_writes, statistics.getValue(RuntimeStatistics::PAGE_WRITES));
	writer.insertBigInt(f_mon_io_page_fetches, statistics.getValue(RuntimeStatistics::PAGE_FETCHES));
	writer.insertBigInt(f_mon_io_page_marks, statistics.getValue(RuntimeStatistics::PAGE_MARKS));

	// logical I/O statistics
	writer.insertByte(TAG_RECORD, rel_mon_rec_stats);
	writer.insertBigInt(f_mon_rec_stat_id, id);
	writer.insertInt(f_mon_rec_stat_group, stat_group);
	writer.insertBigInt(f_mon_rec_seq_reads, statistics.getValue(RuntimeStatistics::RECORD_SEQ_READS));
	writer.insertBigInt(f_mon_rec_idx_reads, statistics.getValue(RuntimeStatistics::RECORD_IDX_READS));
	writer.insertBigInt(f_mon_rec_inserts, statistics.getValue(RuntimeStatistics::RECORD_INSERTS));
	writer.insertBigInt(f_mon_rec_updates, statistics.getValue(RuntimeStatistics::RECORD_UPDATES));
	writer.insertBigInt(f_mon_rec_deletes, statistics.getValue(RuntimeStatistics::RECORD_DELETES));
	writer.insertBigInt(f_mon_rec_backouts, statistics.getValue(RuntimeStatistics::RECORD_BACKOUTS));
	writer.insertBigInt(f_mon_rec_purges, statistics.getValue(RuntimeStatistics::RECORD_PURGES));
	writer.insertBigInt(f_mon_rec_expunges, statistics.getValue(RuntimeStatistics::RECORD_EXPUNGES));
}

void DatabaseSnapshot::putContextVars(StringMap& variables,
									  ClumpletWriter& writer,
									  int object_id, bool is_attachment)
{
	StringMap::Accessor accessor(&variables);

	for (bool found = accessor.getFirst(); found; found = accessor.getNext())
	{
		writer.insertByte(TAG_RECORD, rel_mon_ctx_vars);

		if (is_attachment)
			writer.insertInt(f_mon_ctx_var_att_id, object_id);
		else
			writer.insertInt(f_mon_ctx_var_tra_id, object_id);

		writer.insertString(f_mon_ctx_var_name, accessor.current()->first);
		writer.insertString(f_mon_ctx_var_value, accessor.current()->second);
	}
}

void DatabaseSnapshot::putMemoryUsage(const MemoryStats& stats,
									  ClumpletWriter& writer,
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
