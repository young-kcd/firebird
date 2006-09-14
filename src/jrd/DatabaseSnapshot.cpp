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
#include "../jrd/gdsassert.h"
#include "../jrd/jrd.h"
#include "../jrd/cch.h"
#include "../jrd/ini.h"
#include "../jrd/os/pio.h"
#include "../jrd/req.h"
#include "../jrd/tra.h"
#include "../jrd/blb_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/pag_proto.h"
#include "../jrd/os/pio_proto.h"

#include "../jrd/Relation.h"
#include "../jrd/RecordBuffer.h"
#include "../jrd/DatabaseSnapshot.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef WIN_NT
#include <process.h>
#endif

using namespace Jrd;


DatabaseSnapshot* DatabaseSnapshot::create(thread_db* tdbb)
{
	SET_TDBB(tdbb);

	jrd_tra* transaction = tdbb->tdbb_transaction;
	fb_assert(transaction);

	if (!transaction->tra_db_snapshot)
	{	
		MemoryPool& pool = *transaction->tra_pool;
		transaction->tra_db_snapshot =
			FB_NEW(pool) DatabaseSnapshot(tdbb, pool, transaction);
	}

	return transaction->tra_db_snapshot;
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


DatabaseSnapshot::DatabaseSnapshot(thread_db* tdbb, MemoryPool& pool, jrd_tra* tran)
	: transaction(tran), snapshot(pool)
{
	Database* dbb = tdbb->tdbb_database;

	// Database information

	putDatabase(dbb, allocBuffer(tdbb, pool, rel_mon_database));

	// Attachment information

	RecordBuffer* att_buffer =
		allocBuffer(tdbb, pool, rel_mon_attachments);
	RecordBuffer* tra_buffer =
		allocBuffer(tdbb, pool, rel_mon_transactions);
	RecordBuffer* req_buffer =
		allocBuffer(tdbb, pool, rel_mon_statements);

	for (Attachment* attachment = dbb->dbb_attachments;
		attachment; attachment = attachment->att_next)
	{
		putAttachment(attachment, att_buffer);

		// Transaction information

		for (jrd_tra* transaction_itr = attachment->att_transactions;
			transaction_itr; transaction_itr = transaction_itr->tra_next)
		{
			putTransaction(transaction_itr, tra_buffer);
		}

		// Request information

		for (jrd_req* request = attachment->att_requests;
			request; request = request->req_request)
		{
			// Ignore GDML requests
			if (request->req_sql_text.hasData() || request->req_caller)
			{
				putRequest(request, req_buffer);
			}
		}
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


void DatabaseSnapshot::clearRecord(Record* record)
{
	// Initialize all fields to NULLs
	memset(record->rec_data, 0, record->rec_length);
	const size_t null_bytes = (record->rec_format->fmt_count + 7) >> 3;
	memset(record->rec_data, 0xFF, null_bytes);
}


void DatabaseSnapshot::putField(Record* record, int id, const void* source)
{
	fb_assert(record);

	const Format* const format = record->rec_format;
	fb_assert(format && id < format->fmt_count);

	if (!source) {
		SET_NULL(record, id);
		return;
	}

	const dsc desc = format->fmt_desc[id];
	UCHAR* const address = record->rec_data + (IPTR) desc.dsc_address;

	switch (desc.dsc_dtype) {
	case dtype_text:
		{
			const char* const string = (char*) source;
			const size_t max_length = desc.dsc_length;
			const size_t length = MIN(strlen(string), max_length);
			memcpy(address, string, length);
			memcpy(address + length, " ", max_length - length);
		}
		break;
	case dtype_varying:
		{
			const char* const string = (char*) source;
			const size_t max_length = desc.dsc_length - sizeof(USHORT);
			const size_t length = MIN(strlen(string), max_length);
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

			const char* const string = (char*) source;
			const size_t length = MIN(strlen(string), MAX_USHORT);

			BLB_put_segment(tdbb, blob, (UCHAR*) string, length);
			BLB_close(tdbb, blob);

			*(bid*) address = blob_id;
		}
		break;

	default:
		fb_assert(false);
	}

	CLEAR_NULL(record, id);
}


void DatabaseSnapshot::putDatabase(Database* dbb, RecordBuffer* buffer)
{
	fb_assert(dbb && buffer);

	// Reload header
	const PageSpace* const pageSpace =
		dbb->dbb_page_manager.findPageSpace(DB_PAGE_SPACE);
	const jrd_file* const file = pageSpace->file;
	PAG_header(file->fil_string, file->fil_length, true);

	Record* record = buffer->getTempRecord();

	// Initialize all fields to NULLs
	clearRecord(record);

	SSHORT temp_short;
	// SLONG temp_long;
	SINT64 temp_int64;

	// database name or alias
	putField(record, f_mon_db_name, dbb->dbb_database_name.c_str());
	// page size
	putField(record, f_mon_db_page_size, &dbb->dbb_page_size);
	// major ODS version
	putField(record, f_mon_db_ods_major, &dbb->dbb_ods_version);
	// minor ODS version
	putField(record, f_mon_db_ods_minor, &dbb->dbb_minor_version);
	// oldest interesting transaction
	putField(record, f_mon_db_oit, &dbb->dbb_oldest_transaction);
	// oldest active transaction
	putField(record, f_mon_db_oat, &dbb->dbb_oldest_active);
	// oldest snapshot transaction
	putField(record, f_mon_db_ost, &dbb->dbb_oldest_snapshot);
	// next transaction
	putField(record, f_mon_db_nt, &dbb->dbb_next_transaction);
	// number of page buffers
	putField(record, f_mon_db_page_bufs, &dbb->dbb_bcb->bcb_count);
	// SQL dialect
	temp_short = (dbb->dbb_flags & DBB_DB_SQL_dialect_3) ? 3 : 1;
	putField(record, f_mon_db_dialect, &temp_short);
	// shutdown mode
	if (dbb->dbb_ast_flags & DBB_shutdown_full)
		temp_short = shut_mode_full;
	else if (dbb->dbb_ast_flags & DBB_shutdown_single)
		temp_short = shut_mode_single;
	else if (dbb->dbb_ast_flags & DBB_shutdown)
		temp_short = shut_mode_multi;
	else
		temp_short = shut_mode_online;
	putField(record, f_mon_db_shut_mode, &temp_short);
	// sweep interval
	putField(record, f_mon_db_sweep_int, &dbb->dbb_sweep_interval);
	// read only flag
	temp_short = (dbb->dbb_flags & DBB_read_only) ? 1 : 0;
	putField(record, f_mon_db_read_only, &temp_short);
	// forced writes flag
	temp_short = (dbb->dbb_flags & DBB_force_write) ? 1 : 0;
	putField(record, f_mon_db_forced_writes, &temp_short);
	// reserve space flag
	temp_short = (dbb->dbb_flags & DBB_no_reserve) ? 0 : 1;
	putField(record, f_mon_db_res_space, &temp_short);
	// creation date
	putField(record, f_mon_db_created, &dbb->dbb_creation_date.value());
	// database size
	temp_int64 = PIO_act_alloc(dbb);
	putField(record, f_mon_db_pages, &temp_int64);
	// current memory usage
	temp_int64 = dbb->dbb_memory_stats.get_current_usage();
	putField(record, f_mon_db_cur_mem, &temp_int64);
	// maximum memory usage
	temp_int64 = dbb->dbb_memory_stats.get_maximum_usage();
	putField(record, f_mon_db_max_mem, &temp_int64);
	// page reads
	temp_int64 = dbb->dbb_reads;
	putField(record, f_mon_db_page_reads, &temp_int64);
	// page writes
	temp_int64 = dbb->dbb_writes;
	putField(record, f_mon_db_page_writes, &temp_int64);
	// page fetches
	temp_int64 = dbb->dbb_fetches;
	putField(record, f_mon_db_page_fetches, &temp_int64);
	// page marks
	temp_int64 = dbb->dbb_marks;
	putField(record, f_mon_db_page_marks, &temp_int64);

	buffer->store(record);
}


void DatabaseSnapshot::putAttachment(Attachment* attachment, RecordBuffer* buffer)
{
	fb_assert(attachment && buffer);

	Record* record = buffer->getTempRecord();

	// Initialize all fields to NULLs
	clearRecord(record);

	SSHORT temp_short;
	SLONG temp_long;
	// SINT64 temp_int64;

	SSHORT state = att_s_idle;

	for (jrd_tra* transaction_itr = attachment->att_transactions;
		 transaction_itr; transaction_itr = transaction_itr->tra_next)
	{
		if (transaction_itr->tra_requests)
		{
			state = att_s_active;
			break;
		}
	}

	// attachment id
	putField(record, f_mon_att_id, &attachment->att_attachment_id);
	// process id
	temp_long = getpid();
	putField(record, f_mon_att_server_pid, &temp_long);
	// state
	putField(record, f_mon_att_state, &state);
	// attachment name
	putField(record, f_mon_att_name, attachment->att_filename.c_str());
	// user
	putField(record, f_mon_att_user, attachment->att_user->usr_user_name);
	// role
	putField(record, f_mon_att_role, attachment->att_user->usr_sql_role_name);
	// remote protocol
	putField(record, f_mon_att_remote_proto, attachment->att_network_protocol.c_str());
	// remote address
	if (attachment->att_remote_address.hasData()) {
		putField(record, f_mon_att_remote_addr, attachment->att_remote_address.c_str());
	}
	// remote process id
	if (attachment->att_remote_pid) {
		temp_long = attachment->att_remote_pid;
		putField(record, f_mon_att_remote_pid, &temp_long);
	}
	// charset
	putField(record, f_mon_att_charset_id, &attachment->att_charset);
	// timestamp
	putField(record, f_mon_att_timestamp, &attachment->att_timestamp.value());
	// garbage collection flag
	temp_short = (attachment->att_flags & ATT_no_cleanup) ? 0 : 1;
	putField(record, f_mon_att_gc, &temp_short);

	buffer->store(record);
}


void DatabaseSnapshot::putTransaction(jrd_tra* new_transaction, RecordBuffer* buffer)
{
	fb_assert(new_transaction && buffer);

	Record* record = buffer->getTempRecord();

	// Initialize all fields to NULLs
	clearRecord(record);

	SSHORT temp_short;
	// SLONG temp_long;
	// SINT64 temp_int64;

	// transaction id
	putField(record, f_mon_tra_id, &new_transaction->tra_number);
	// attachment id
	putField(record, f_mon_tra_att_id, &new_transaction->tra_attachment->att_attachment_id);
	// state
	temp_short = new_transaction->tra_requests ? tra_s_active : tra_s_idle;
	putField(record, f_mon_tra_state, &temp_short);
	// timestamp
	putField(record, f_mon_tra_timestamp, &new_transaction->tra_timestamp.value());
	// top transaction
	putField(record, f_mon_tra_top, &new_transaction->tra_top);
	// oldest transaction
	putField(record, f_mon_tra_oit, &new_transaction->tra_oldest);
	// oldest active transaction
	putField(record, f_mon_tra_oat, &new_transaction->tra_oldest_active);
	// isolation mode
	if (new_transaction->tra_flags & TRA_degree3)
		temp_short = iso_mode_consistency;
	else if (new_transaction->tra_flags & TRA_read_committed)
		temp_short = (new_transaction->tra_flags &  TRA_rec_version) ?
			iso_mode_rc_version : iso_mode_rc_no_version;
	else
		temp_short = iso_mode_concurrency;
	putField(record, f_mon_tra_iso_mode, &temp_short);
	// lock timeout
	putField(record, f_mon_tra_lock_timeout, &new_transaction->tra_lock_timeout);
	// read only flag
	temp_short = (new_transaction->tra_flags & TRA_readonly) ? 1 : 0;
	putField(record, f_mon_tra_read_only, &temp_short);
	// autocommit flag
	temp_short = (new_transaction->tra_flags & TRA_autocommit) ? 1 : 0;
	putField(record, f_mon_tra_auto_commit, &temp_short);
	// auto undo flag
	temp_short = (new_transaction->tra_flags & TRA_no_auto_undo) ? 0 : 1;
	putField(record, f_mon_tra_auto_undo, &temp_short);

	buffer->store(record);
}


void DatabaseSnapshot::putRequest(jrd_req* request, RecordBuffer* buffer)
{
	fb_assert(request && buffer);

	Record* record = buffer->getTempRecord();

	// Initialize all fields to NULLs
	clearRecord(record);

	SSHORT temp_short;
	// SLONG temp_long;
	// SINT64 temp_int64;

	// request id
	putField(record, f_mon_stmt_id, &request->req_id);
	// attachment id
	if (request->req_attachment) {
		putField(record, f_mon_stmt_att_id,
				 &request->req_attachment->att_attachment_id);
	}
	// transaction id
	if (request->req_transaction) {
		putField(record, f_mon_stmt_tra_id,
				 &request->req_transaction->tra_number);
	}
	// timestamp
	if (!request->req_timestamp.isEmpty()) {
		putField(record, f_mon_stmt_timestamp, &request->req_timestamp.value());
	}
	// state
	if (request->req_flags & req_active)
		temp_short = stmt_s_active;
	else if (request->req_flags & req_stall)
		temp_short = stmt_s_stalled;
	else
		temp_short = stmt_s_idle;
	putField(record, f_mon_stmt_state, &temp_short);
	// caller
	if (request->req_caller) {
		putField(record, f_mon_stmt_caller_id, &request->req_caller->req_id);
	}
	// sql text
	if (request->req_sql_text.hasData()) {
		putField(record, f_mon_stmt_sql_text, request->req_sql_text.c_str());
	}
	// selects
	putField(record, f_mon_stmt_rec_selects, &request->req_records_selected);
	// inserts
	putField(record, f_mon_stmt_rec_inserts, &request->req_records_inserted);
	// updates
	putField(record, f_mon_stmt_rec_updates, &request->req_records_updated);
	// deletes
	putField(record, f_mon_stmt_rec_deletes, &request->req_records_deleted);

	buffer->store(record);
}
