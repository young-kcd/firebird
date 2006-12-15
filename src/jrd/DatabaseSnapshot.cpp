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
const size_t DatabaseSnapshot::SharedMemory::SEMAPHORES = 1;


DatabaseSnapshot::SharedMemory::SharedMemory()
{
#ifdef UNIX
	handle.sh_mem_semaphores = SEMAPHORES;
#endif

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
	const size_t length = base->length;

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
	if (base->length + length > handle.sh_mem_length_mapped)
	{
		extend(base->length + length);
	}

	// Get the remapped pointer
	UCHAR* buffer = (UCHAR*) base + sizeof(Header);
	// Copy the data at the insertion point
	memcpy(buffer + base->length, writer.getBuffer(), length);
	// Adjust the length
	base->length += length;
}


void DatabaseSnapshot::SharedMemory::garbageCollect(thread_db* tdbb, bool self)
{
	fb_assert(tdbb);
	Database* dbb = tdbb->tdbb_database;
	fb_assert(dbb);

	// Get the buffer length
	UCHAR* buffer = (UCHAR*) base + sizeof(Header);
	const size_t length = base->length;

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
	if (newLength < base->length)
	{
		memcpy(buffer, writer.getBuffer(), newLength);
		base->length = newLength;
	}
}


void DatabaseSnapshot::SharedMemory::extend(size_t newLength)
{
	fb_assert(newLength > base->length);
	newLength = FB_ALIGN(newLength, DEFAULT_SIZE);

	// Temporary code. To be replaced with an attempt to remap the memory
	// plus a new GDS error code if the remap was unsuccessful.
	release();
	gds__log("Monitoring table space exhausted");
	exit(FINI_ERROR);
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
	checkMutex("init", ISC_mutex_init(&shmem->mutex, MONITOR_FILE));
#endif

	if (!initialize)
		return;

	// Initialize the shared data header
	Header* header = (Header*) shmemData->sh_mem_address;
	header->version = VERSION;
	header->length = 0;

#ifndef WIN_NT
	checkMutex("init", ISC_mutex_init(&header->mutex, shmemData->sh_mem_mutex_arg));
#endif
}


// DatabaseSnapshot class

Mutex DatabaseSnapshot::initMutex;
DatabaseSnapshot::SharedMemory* DatabaseSnapshot::dump = NULL;


DatabaseSnapshot* DatabaseSnapshot::create(thread_db* tdbb)
{
	SET_TDBB(tdbb);

	jrd_tra* transaction = tdbb->tdbb_transaction;
	fb_assert(transaction);

	if (!transaction->tra_db_snapshot)
	{
		// Create a database snapshot and store it
		// in the transaction block
		MemoryPool& pool = *transaction->tra_pool;
		transaction->tra_db_snapshot =
			FB_NEW(pool) DatabaseSnapshot(tdbb, pool, transaction);
	}

	return transaction->tra_db_snapshot;
}


int DatabaseSnapshot::blockingAst(void* ast_object)
{
	Database* dbb = static_cast<Database*>(ast_object);
	fb_assert(dbb);

	thread_db thd_context, *tdbb;
	JRD_set_thread_data(tdbb, thd_context);

	Lock* lock = dbb->dbb_monitor_lock;
	fb_assert(lock);

	tdbb->tdbb_database = lock->lck_dbb;
	tdbb->tdbb_attachment = lock->lck_attachment;
	tdbb->tdbb_quantum = QUANTUM;
	tdbb->tdbb_request = NULL;
	tdbb->tdbb_transaction = NULL;

	Jrd::ContextPoolHolder context(tdbb, dbb->dbb_permanent);

	// Write the data to the shared memory
	dumpData(tdbb, true);

	// Release the lock and mark dbb as requesting a new one
	LCK_release(tdbb, lock);
	dbb->dbb_ast_flags |= DBB_monitor_off;

	JRD_restore_thread_data();
	return 0;
}


DatabaseSnapshot::DatabaseSnapshot(thread_db* tdbb, MemoryPool& pool, jrd_tra* tran)
	: transaction(tran), snapshot(pool)
{
	// Initialize record buffers
	RecordBuffer* dbb_buffer =
		allocBuffer(tdbb, pool, rel_mon_database);
	RecordBuffer* att_buffer =
		allocBuffer(tdbb, pool, rel_mon_attachments);
	RecordBuffer* tra_buffer =
		allocBuffer(tdbb, pool, rel_mon_transactions);
	RecordBuffer* stmt_buffer =
		allocBuffer(tdbb, pool, rel_mon_statements);

	Database* dbb = tdbb->tdbb_database;
	fb_assert(dbb);

	// Dump data for our own dbb, as we won't be
	// signalled by the LCK_convert call below
	AutoPtr<ClumpletReader> reader(dumpData(tdbb, false));

	// This variable defines whether we're interested in the global data
	// (i.e. all attachments) and hence should signal other processes
	// or it's enough to return the local data (our own attachment) only.
	const bool broadcast = (reader == NULL);

	if (broadcast)
	{
		// Signal owners of other dbbs to dump their data as well
		LCK_convert(tdbb, dbb->dbb_monitor_lock, LCK_EX, LCK_WAIT);
		dump->acquire();
		reader = dump->readData(tdbb);
	}

	fb_assert(reader);
	reader->rewind();

	try {
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
				default:
					fb_assert(false);
				}

				record = buffer->getTempRecord();
				clearRecord(record);
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
					if (allowed && !database_processed)
					{
						putField(record, fid, source, length);
						fields_processed = true;
					}
				}
				// The generic logic that covers all relations
				// except MON$DATABASE
				else if (allowed)
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
	catch (const Exception& ex) {
		gds__log(ex.what());
	}

	// Release locks
	if (broadcast)
	{
		dump->release();
		LCK_convert(tdbb, dbb->dbb_monitor_lock, LCK_SR, LCK_WAIT);
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

	switch (desc.dsc_dtype) {
	case dtype_text:
		{
			const char* const string = (char*) source;
			const size_t max_length = desc.dsc_length;
			length = MIN(length, max_length);
			memcpy(address, string, length);
			memcpy(address + length, " ", max_length - length);
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

			bid blob_id;
			blb* blob = BLB_create2(tdbb, transaction, &blob_id, 0, NULL);

			length = MIN(length, MAX_USHORT);

			BLB_put_segment(tdbb, blob, (UCHAR*) source, length);
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

	switch (rid) {
	case rel_mon_attachments:
		switch (fid) {
		case f_mon_att_remote_proto:
		case f_mon_att_remote_addr:
			return length ? source : NULL;
		case f_mon_att_remote_pid:
			return (*(SLONG*) source) ? source : NULL;
		default:
			break;
		}
		break;
	case rel_mon_statements:
		switch (fid) {
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
	default:
		break;
	}

	return source;
}


ClumpletReader* DatabaseSnapshot::dumpData(thread_db* tdbb, bool locksmith)
{
	fb_assert(tdbb);
	Database* dbb = tdbb->tdbb_database;
	fb_assert(dbb);

	const Attachment* self_attachment = tdbb->tdbb_attachment;
	fb_assert(self_attachment);

	// Define whether we work in the global or local mode.
	// The local one implies that we should dump our own
	// attachment only and that the dump will be processed
	// inside the caller process.
	if (!locksmith)
	{
		locksmith = self_attachment->att_user->locksmith();
	}

	ClumpletWriter* writer = NULL;

	try {

	MemoryPool& pool = *tdbb->getDefaultPool();
	writer = FB_NEW(pool)
		ClumpletWriter(pool, ClumpletReader::WideUnTagged, MAX_ULONG);

	writer->insertBytes(TAG_DBB, (UCHAR*) &dbb->dbb_guid, sizeof(FB_GUID));

	// Database information

	putDatabase(dbb, *writer);

	// Attachment information

	for (Attachment* attachment = dbb->dbb_attachments;
		attachment; attachment = attachment->att_next)
	{
		if (!locksmith && self_attachment != attachment)
			continue;

		putAttachment(attachment, *writer);

		// Transaction information

		for (jrd_tra* transaction = attachment->att_transactions;
			transaction; transaction = transaction->tra_next)
		{
			putTransaction(transaction, *writer);
		}

		// Request information

		for (jrd_req* request = attachment->att_requests;
			request; request = request->req_request)
		{
			if (!(request->req_flags & (req_internal | req_sys_trigger)))
			{
				putRequest(request, *writer);
			}
		}
	}

	if (locksmith)
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

		dump->acquire();
		dump->writeData(tdbb, *writer);
		dump->release();
	}

	}
	catch (const Firebird::Exception& ex) {
		gds__log(ex.what());
	}

	if (locksmith)
	{
		delete writer;
		writer = NULL;
	}

	return writer;
}


void DatabaseSnapshot::putDatabase(Database* database,
								   ClumpletWriter& writer)
{
	fb_assert(database);

	writer.insertByte(TAG_RECORD, rel_mon_database);

	// Reload header
	const PageSpace* const pageSpace =
		database->dbb_page_manager.findPageSpace(DB_PAGE_SPACE);
	const jrd_file* const file = pageSpace->file;
	PAG_header(file->fil_string, file->fil_length, true);

	int temp;

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
	writer.insertBigInt(f_mon_db_pages, PIO_act_alloc(database));
}


void DatabaseSnapshot::putAttachment(Attachment* attachment,
									 ClumpletWriter& writer)
{
	fb_assert(attachment);

	writer.insertByte(TAG_RECORD, rel_mon_attachments);

	int temp = att_s_idle;

	for (jrd_tra* transaction_itr = attachment->att_transactions;
		 transaction_itr; transaction_itr = transaction_itr->tra_next)
	{
		if (transaction_itr->tra_requests)
		{
			temp = att_s_active;
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
	// charset
	writer.insertInt(f_mon_att_charset_id, attachment->att_charset);
	// timestamp
	writer.insertBytes(f_mon_att_timestamp,
					   (UCHAR*) &attachment->att_timestamp.value(),
					   sizeof(ISC_TIMESTAMP));
	// garbage collection flag
	temp = (attachment->att_flags & ATT_no_cleanup) ? 0 : 1;
	writer.insertInt(f_mon_att_gc, temp);
}


void DatabaseSnapshot::putTransaction(jrd_tra* new_transaction,
									  ClumpletWriter& writer)
{
	fb_assert(new_transaction);

	writer.insertByte(TAG_RECORD, rel_mon_transactions);

	int temp;

	// transaction id
	writer.insertInt(f_mon_tra_id, new_transaction->tra_number);
	// attachment id
	writer.insertInt(f_mon_tra_att_id,
					 new_transaction->tra_attachment->att_attachment_id);
	// state
	temp = new_transaction->tra_requests ? tra_s_active : tra_s_idle;
	writer.insertInt(f_mon_tra_state, temp);
	// timestamp
	writer.insertBytes(f_mon_tra_timestamp,
					   (UCHAR*) &new_transaction->tra_timestamp.value(),
					   sizeof(ISC_TIMESTAMP));
	// top transaction
	writer.insertInt(f_mon_tra_top, new_transaction->tra_top);
	// oldest transaction
	writer.insertInt(f_mon_tra_oit, new_transaction->tra_oldest);
	// oldest active transaction
	writer.insertInt(f_mon_tra_oat, new_transaction->tra_oldest_active);
	// isolation mode
	if (new_transaction->tra_flags & TRA_degree3)
		temp = iso_mode_consistency;
	else if (new_transaction->tra_flags & TRA_read_committed)
		temp = (new_transaction->tra_flags &  TRA_rec_version) ?
			iso_mode_rc_version : iso_mode_rc_no_version;
	else
		temp = iso_mode_concurrency;
	writer.insertInt(f_mon_tra_iso_mode, temp);
	// lock timeout
	writer.insertInt(f_mon_tra_lock_timeout,
					 new_transaction->tra_lock_timeout);
	// read only flag
	temp = (new_transaction->tra_flags & TRA_readonly) ? 1 : 0;
	writer.insertInt(f_mon_tra_read_only, temp);
	// autocommit flag
	temp = (new_transaction->tra_flags & TRA_autocommit) ? 1 : 0;
	writer.insertInt(f_mon_tra_auto_commit, temp);
	// auto undo flag
	temp = (new_transaction->tra_flags & TRA_no_auto_undo) ? 0 : 1;
	writer.insertInt(f_mon_tra_auto_undo, temp);
}


void DatabaseSnapshot::putRequest(jrd_req* request,
								  ClumpletWriter& writer)
{
	fb_assert(request);

	writer.insertByte(TAG_RECORD, rel_mon_statements);

	int temp;

	// request id
	writer.insertInt(f_mon_stmt_id, request->req_id);
	// attachment id
	if (request->req_attachment) {
		writer.insertInt(f_mon_stmt_att_id,
						 request->req_attachment->att_attachment_id);
	}
	else {
		writer.insertInt(f_mon_stmt_att_id, 0);
	}
	// transaction id
	if (request->req_transaction) {
		writer.insertInt(f_mon_stmt_tra_id,
						 request->req_transaction->tra_number);
	}
	else {
		writer.insertInt(f_mon_stmt_tra_id, 0);
	}
	// timestamp
	writer.insertBytes(f_mon_stmt_timestamp,
					   (UCHAR*) &request->req_timestamp.value(),
					   sizeof(ISC_TIMESTAMP));
	// state
	if (request->req_flags & req_blocking)
		temp = stmt_s_killed;
	else if (request->req_flags & req_active)
		temp = stmt_s_active;
	else
		temp = stmt_s_idle;
	writer.insertInt(f_mon_stmt_state, temp);
	// sql text
	writer.insertString(f_mon_stmt_sql_text, request->req_sql_text);
}
