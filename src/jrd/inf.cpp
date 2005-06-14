/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		inf.cpp
 *	DESCRIPTION:	Information handler
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 * 2001.07.06 Sean Leyne - Code Cleanup, removed "#ifdef READONLY_DATABASE"
 *                         conditionals, as the engine now fully supports
 *                         readonly databases.
 * 2001.08.09 Claudio Valderrama - Added new isc_info_* tokens to INF_database_info():
 *	oldest_transaction, oldest_active, oldest_snapshot and next_transaction.
 *      Make INF_put_item() to reserve 4 bytes: item + length as short + info_end;
 *	otherwise to signal output buffer truncation.
 *
 * 2001.11.28 Ann Harrison - the dbb has to be refreshed before reporting
 *      oldest_transaction, oldest_active, oldest_snapshot and next_transaction.
 *
 * 2001.11.29 Paul Reeves - Added refresh of dbb to ensure forced_writes 
 *      reports correctly when called immediately after a create database 
 *      operation.
 */

#include "firebird.h"
#include <string.h>
#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../jrd/ibase.h"
#include "../jrd/tra.h"
#include "../jrd/blb.h"
#include "../jrd/req.h"
#include "../jrd/val.h"
#include "../jrd/exe.h"
#include "../jrd/os/pio.h"
#include "../jrd/ods.h"
#include "../jrd/scl.h"
#include "../jrd/lck.h"
#include "../jrd/cch.h"
#include "../jrd/license.h"
#include "../jrd/cch_proto.h"
#include "../jrd/inf_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/opt_proto.h"
#include "../jrd/pag_proto.h"
#include "../jrd/os/pio_proto.h"
#include "../jrd/thd.h"
#include "../jrd/tra_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/intl_proto.h"

using namespace Jrd;


/*
 * The variable DBSERVER_BASE_LEVEL was originally IB_MAJOR_VER but with
 * the change to Firebird this number could no longer be used.
 * The DBSERVER_BASE_LEVEL for firebird starts at 6 which is the base level
 * of InterBase(r) from which Firebird was derived.
 * It is expected that this value will increase as changes are added to
 * Firebird
 */

#define DBSERVER_BASE_LEVEL 6


#define STUFF_WORD(p, value)	{*p++ = value; *p++ = value >> 8;}
#define STUFF(p, value)		*p++ = value

static USHORT get_counts(USHORT, SCHAR*, USHORT);


#pragma FB_COMPILER_MESSAGE("Our caller doesn't check our boolean result, only traps an exception!")
int INF_blob_info(const blb* blob,
				  const SCHAR* items,
				  const SSHORT item_length,
				  SCHAR* info,
				  const SSHORT output_length)
{
/**************************************
 *
 *	I N F _ b l o b _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Process requests for blob info.
 *
 **************************************/
	SCHAR buffer[128];
	SSHORT length;

	const SCHAR* const end_items = items + item_length;
	const SCHAR* const end = info + output_length;

	while (items < end_items && *items != isc_info_end) {
		SCHAR item = *items++;
		switch (item) {
		case isc_info_end:
			break;

		case isc_info_blob_num_segments:
			length = INF_convert(blob->blb_count, buffer);
			break;

		case isc_info_blob_max_segment:
			length = INF_convert(blob->blb_max_segment, buffer);
			break;

		case isc_info_blob_total_length:
			length = INF_convert(blob->blb_length, buffer);
			break;

		case isc_info_blob_type:
			buffer[0] = (blob->blb_flags & BLB_stream) ? 1 : 0;
			length = 1;
			break;

		default:
			buffer[0] = item;
			item = isc_info_error;
			length = 1 + INF_convert(isc_infunk, buffer + 1);
			break;
		}
		if (!(info = INF_put_item(item, length, buffer, info, end)))
			return FALSE;
	}

	*info++ = isc_info_end;

	return TRUE;
}


USHORT INF_convert(SLONG number, SCHAR* buffer)
{
/**************************************
 *
 *	I N F _ c o n v e r t
 *
 **************************************
 *
 * Functional description
 *	Convert a number to VAX form -- least significant bytes first.
 *	Return the length.
 *
 **************************************/
	const SCHAR* p;

#ifndef WORDS_BIGENDIAN
	// CVC: What's the need for an intermediate "n" here?
	const SLONG n = number;
	p = reinterpret_cast<const SCHAR*>(&n);
	*buffer++ = *p++;
	*buffer++ = *p++;
	*buffer++ = *p++;
	*buffer = *p;

#else

	p = reinterpret_cast<const SCHAR*>(&number);
	p += 3;
	*buffer++ = *p--;
	*buffer++ = *p--;
	*buffer++ = *p--;
	*buffer = *p;

#endif

	return 4;
}


int INF_database_info(const SCHAR* items,
					  const SSHORT item_length,
					  SCHAR* info, const SSHORT output_length)
{
/**************************************
 *
 *	I N F _ d a t a b a s e _ i n f o	( J R D )
 *
 **************************************
 *
 * Functional description
 *	Process requests for database info.
 *
 **************************************/
	SCHAR buffer[256];
	SSHORT length;
	SLONG id;
	SLONG err_val;
	bool header_refreshed = false;

	thread_db* tdbb = JRD_get_thread_data();
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	jrd_tra* transaction = NULL;
	const SCHAR* const end_items = items + item_length;
	const SCHAR* const end = info + output_length;

	Attachment* err_att = 0;
	Attachment* att = 0;
	const SCHAR* q;

	while (items < end_items && *items != isc_info_end) {
		SCHAR* p = buffer;
		SCHAR item = *items++;
		switch (item) {
		case isc_info_end:
			break;

		case isc_info_reads:
			length = INF_convert(dbb->dbb_reads, buffer);
			break;

		case isc_info_writes:
			length = INF_convert(dbb->dbb_writes, buffer);
			break;

		case isc_info_fetches:
			length = INF_convert(dbb->dbb_fetches, buffer);
			break;

		case isc_info_marks:
			length = INF_convert(dbb->dbb_marks, buffer);
			break;

		case isc_info_page_size:
			length = INF_convert(dbb->dbb_page_size, buffer);
			break;

		case isc_info_num_buffers:
			length = INF_convert(dbb->dbb_bcb->bcb_count, buffer);
			break;

		case isc_info_set_page_buffers:
			length = INF_convert(dbb->dbb_page_buffers, buffer);
			break;

		case isc_info_logfile:
			length = INF_convert(FALSE, buffer);
			break;

		case isc_info_cur_logfile_name:
			*p++ = 0;
			length = p - buffer;
			break;

		case isc_info_cur_log_part_offset:
			length = INF_convert(0, buffer);
			break;

		case isc_info_num_wal_buffers:
			length = 0;
			break;

		case isc_info_wal_buffer_size:
			length = 0;
			break;

		case isc_info_wal_ckpt_length:
			length = 0;
			break;

		case isc_info_wal_cur_ckpt_interval:
			length = 0;
			break;

		case isc_info_wal_prv_ckpt_fname:
			*p++ = 0;
			length = p - buffer;
			break;

		case isc_info_wal_prv_ckpt_poffset:
			length = INF_convert(0, buffer);
			break;

		case isc_info_wal_recv_ckpt_fname:
			// WAL obsolete
			length = 0;
			break;

		case isc_info_wal_recv_ckpt_poffset:
			// WAL obsolete
			length = 0;
			break;

		case isc_info_wal_grpc_wait_usecs:
			length = 0;
			break;

		case isc_info_wal_num_io:
			length = 0;
			break;

		case isc_info_wal_avg_io_size:
			length = 0;
			break;

		case isc_info_wal_num_commits:
			length = 0;
			break;

		case isc_info_wal_avg_grpc_size:
			length = 0;
			break;

#ifdef SUPERSERVER
		case isc_info_current_memory:
			length = INF_convert(dbb->dbb_memory_stats.get_current_usage(), buffer);
			break;

		case isc_info_max_memory:
			length = INF_convert(dbb->dbb_memory_stats.get_maximum_usage(), buffer);
			break;
#else
		case isc_info_current_memory:
			length = INF_convert(MemoryPool::default_stats_group.get_current_usage(), buffer);
			break;

		case isc_info_max_memory:
			length = INF_convert(MemoryPool::default_stats_group.get_maximum_usage(), buffer);
			break;
#endif

		case isc_info_attachment_id:
			length = INF_convert(PAG_attachment_id(), buffer);
			break;

		case isc_info_ods_version:
			length = INF_convert(dbb->dbb_ods_version, buffer);
			break;

		case isc_info_ods_minor_version:
			length = INF_convert(dbb->dbb_minor_version, buffer);
			break;

		case isc_info_allocation:
			CCH_flush(tdbb, FLUSH_ALL, 0L);
			length = INF_convert(PIO_max_alloc(dbb), buffer);
			break;

		case isc_info_sweep_interval:
			length = INF_convert(dbb->dbb_sweep_interval, buffer);
			break;

		case isc_info_read_seq_count:
			length =
				get_counts(DBB_read_seq_count,
						   buffer,
						   sizeof(buffer));
			break;

		case isc_info_read_idx_count:
			length =
				get_counts(DBB_read_idx_count,
						   buffer,
						   sizeof(buffer));
			break;

		case isc_info_update_count:
			length =
				get_counts(DBB_update_count,
						   buffer,
						   sizeof(buffer));
			break;

		case isc_info_insert_count:
			length =
				get_counts(DBB_insert_count,
						   buffer,
						   sizeof(buffer));
			break;

		case isc_info_delete_count:
			length =
				get_counts(DBB_delete_count,
						   buffer,
						   sizeof(buffer));
			break;

		case isc_info_backout_count:
			length =
				get_counts(DBB_backout_count,
						   buffer,
						   sizeof(buffer));
			break;

		case isc_info_purge_count:
			length =
				get_counts(DBB_purge_count,
						   buffer,
						   sizeof(buffer));
			break;

		case isc_info_expunge_count:
			length =
				get_counts(DBB_expunge_count,
						   buffer,
						   sizeof(buffer));
			break;

		case isc_info_implementation:
			STUFF(p, 1);		/* Count */
			STUFF(p, IMPLEMENTATION);
			STUFF(p, 1);		/* Class */
			length = p - buffer;
			break;

		case isc_info_base_level:
			/* info_base_level is used by the client to represent
			 * what the server is capable of.  It is equivalent to the
			 * ods version of a database.  For example,
			 * ods_version represents what the database 'knows'
			 * base_level represents what the server 'knows'
			 */
			STUFF(p, 1);		/* Count */
#ifdef SCROLLABLE_CURSORS
			UPDATE WITH VERSION OF SERVER SUPPORTING
				SCROLLABLE CURSORS STUFF(p, 5);	/* base level of scrollable cursors */
#else
			/* IB_MAJOR_VER is defined as a character string */
			STUFF(p, DBSERVER_BASE_LEVEL);	/* base level of current version */
#endif
			length = p - buffer;
			break;

		case isc_info_isc_version:
			STUFF(p, 1);
			STUFF(p, sizeof(ISC_VERSION) - 1);
			for (q = ISC_VERSION; *q;)
				STUFF(p, *q++);
			length = p - buffer;
			break;

		case isc_info_firebird_version:
		    STUFF(p, 1);
			STUFF(p, sizeof(FB_VERSION) - 1);
			for (q = FB_VERSION; *q;)
				STUFF(p, *q++);
			length = p - buffer;
			break;

		case isc_info_db_id:
			{
				const Firebird::PathName& str_fn = dbb->dbb_database_name;
				STUFF(p, 2);
				SSHORT l = str_fn.length();
				*p++ = l;
				for (q = str_fn.c_str(); *q;)
					*p++ = *q++;
				SCHAR site[256];
				ISC_get_host(site, sizeof(site));
				*p++ = l = strlen(site);
				for (q = site; *q;)
					*p++ = *q++;
				length = p - buffer;
				break;
			}

		case isc_info_no_reserve:
			*p++ = (dbb->dbb_flags & DBB_no_reserve) ? 1 : 0;
			length = p - buffer;
			break;

		case isc_info_forced_writes:
			if (!header_refreshed)
			{
				const jrd_file* file = dbb->dbb_file;
				PAG_header(file->fil_string, file->fil_length);
				header_refreshed = true;
			}
			*p++ = (dbb->dbb_flags & DBB_force_write) ? 1 : 0;
			length = p - buffer;
			break;

		case isc_info_limbo:
			if (!transaction)
				transaction = TRA_start(tdbb, 0, NULL);
			for (id = transaction->tra_oldest;
				 id < transaction->tra_number; id++)
			{
				if (TRA_snapshot_state(tdbb, transaction, id) == tra_limbo &&
					TRA_wait(tdbb, transaction, id, jrd_tra::tra_wait) == tra_limbo)
				{
					length = INF_convert(id, buffer);
					if (!
						(info =
						 INF_put_item(item, length, buffer, info, end)))
					{
						if (transaction)
							TRA_commit(tdbb, transaction, false);
						return FALSE;
					}
				}
			}
			continue;

		case isc_info_active_transactions:
			if (!transaction)
				transaction = TRA_start(tdbb, 0, NULL);
			for (id = transaction->tra_oldest_active;
				 id < transaction->tra_number; id++)
			{
				if (TRA_snapshot_state(tdbb, transaction, id) == tra_active) {
					length = INF_convert(id, buffer);
					if (!
						(info =
						 INF_put_item(item, length, buffer, info, end)))
					{
						if (transaction)
							TRA_commit(tdbb, transaction, false);
						return FALSE;
					}
				}
			}
			continue;

		case isc_info_user_names:
			for (att = dbb->dbb_attachments; att; att = att->att_next) {
				if (att->att_flags & ATT_shutdown)
					continue;
                
                const UserId* user = att->att_user;
				if (user) {
					const char* user_name = user->usr_user_name ?
						user->usr_user_name : "(SQL Server)";
					p = buffer;
					SSHORT l = strlen (user_name);
					*p++ = l;
					for (q = user_name; l; l--)
						*p++ = *q++;
					length = p - buffer;
                    info = INF_put_item(item, length, buffer, info, end);
					if (!info) {
						if (transaction)
							TRA_commit(tdbb, transaction, false);
						return FALSE;
					}
				}
			}
			continue;

		case isc_info_page_errors:
			err_att = tdbb->tdbb_attachment;
			if (err_att->att_val_errors) {
				err_val =
					(*err_att->att_val_errors)[VAL_PAG_WRONG_TYPE]
					+ (*err_att->att_val_errors)[VAL_PAG_CHECKSUM_ERR]
					+ (*err_att->att_val_errors)[VAL_PAG_DOUBLE_ALLOC]
					+ (*err_att->att_val_errors)[VAL_PAG_IN_USE]
					+ (*err_att->att_val_errors)[VAL_PAG_ORPHAN];
			}
			else
				err_val = 0;

			length = INF_convert(err_val, buffer);
			break;

		case isc_info_bpage_errors:
			err_att = tdbb->tdbb_attachment;
			if (err_att->att_val_errors) {
				err_val =
					(*err_att->att_val_errors)[VAL_BLOB_INCONSISTENT]
					+ (*err_att->att_val_errors)[VAL_BLOB_CORRUPT]
					+ (*err_att->att_val_errors)[VAL_BLOB_TRUNCATED];
			}
			else
				err_val = 0;

			length = INF_convert(err_val, buffer);
			break;

		case isc_info_record_errors:
			err_att = tdbb->tdbb_attachment;
			if (err_att->att_val_errors) {
				err_val =
					(*err_att->att_val_errors)[VAL_REC_CHAIN_BROKEN]
					+ (*err_att->att_val_errors)[VAL_REC_DAMAGED]
					+ (*err_att->att_val_errors)[VAL_REC_BAD_TID]
					+
					(*err_att->att_val_errors)[VAL_REC_FRAGMENT_CORRUPT] +
					(*err_att->att_val_errors)[VAL_REC_WRONG_LENGTH] +
					(*err_att->att_val_errors)[VAL_REL_CHAIN_ORPHANS];
			}
			else
				err_val = 0;

			length = INF_convert(err_val, buffer);
			break;

		case isc_info_dpage_errors:
			err_att = tdbb->tdbb_attachment;
			if (err_att->att_val_errors) {
				err_val =
					(*err_att->att_val_errors)[VAL_DATA_PAGE_CONFUSED]
					+
					(*err_att->att_val_errors)[VAL_DATA_PAGE_LINE_ERR];
			}
			else
				err_val = 0;

			length = INF_convert(err_val, buffer);
			break;

		case isc_info_ipage_errors:
			err_att = tdbb->tdbb_attachment;
			if (err_att->att_val_errors) {
				err_val =
					(*err_att->att_val_errors)[VAL_INDEX_PAGE_CORRUPT]
					+
					(*err_att->att_val_errors)[VAL_INDEX_ROOT_MISSING] +
					(*err_att->att_val_errors)[VAL_INDEX_MISSING_ROWS] +
					(*err_att->att_val_errors)[VAL_INDEX_ORPHAN_CHILD];
			}
			else
				err_val = 0;

			length = INF_convert(err_val, buffer);
			break;

		case isc_info_ppage_errors:
			err_att = tdbb->tdbb_attachment;
			if (err_att->att_val_errors) {
				err_val = (*err_att->att_val_errors)[VAL_P_PAGE_LOST]
					+
					(*err_att->att_val_errors)[VAL_P_PAGE_INCONSISTENT];
			}
			else
				err_val = 0;

			length = INF_convert(err_val, buffer);
			break;

		case isc_info_tpage_errors:
			err_att = tdbb->tdbb_attachment;
			if (err_att->att_val_errors) {
				err_val = (*err_att->att_val_errors)[VAL_TIP_LOST]
					+ (*err_att->att_val_errors)[VAL_TIP_LOST_SEQUENCE]
					+ (*err_att->att_val_errors)[VAL_TIP_CONFUSED];
			}
			else
				err_val = 0;

			length = INF_convert(err_val, buffer);
			break;

		case isc_info_db_sql_dialect:
			/*
			   **
			   ** there are 3 types of databases:
			   **
			   **   1. a DB that is created before V6.0. This DB only speak SQL
			   **        dialect 1 and 2.
			   **
			   **   2. a non ODS 10 DB is backed up/restored in IB V6.0. Since
			   **        this DB contained some old SQL dialect, therefore it
			   **        speaks SQL dialect 1, 2, and 3
			   **
			   **   3. a DB that is created in V6.0. This DB speak SQL
			   **        dialect 1, 2 or 3 depending the DB was created
			   **        under which SQL dialect.
			   **
			 */
			if (ENCODE_ODS(dbb->dbb_ods_version, dbb->dbb_minor_original)
				>= ODS_10_0)
			{
				if (dbb->dbb_flags & DBB_DB_SQL_dialect_3) {
					/*
					   ** DB created in IB V6.0 by client SQL dialect 3
					 */
					*p++ = SQL_DIALECT_V6;
				}
				else {
					/*
					   ** old DB was gbaked in IB V6.0
					 */
					*p++ = SQL_DIALECT_V5;
				}
			}
			else
				*p++ = SQL_DIALECT_V5;	/* pre ODS 10 DB */

			length = p - buffer;
			break;

		case isc_info_db_read_only:
			*p++ = (dbb->dbb_flags & DBB_read_only) ? 1 : 0;
			length = p - buffer;

			break;

		case isc_info_db_size_in_pages:
			CCH_flush(tdbb, FLUSH_ALL, 0L);
			length = INF_convert(PIO_act_alloc(dbb), buffer);
			break;

		case isc_info_oldest_transaction:
			if (!header_refreshed)
			{
				const jrd_file* file = dbb->dbb_file;
				PAG_header(file->fil_string, file->fil_length);
				header_refreshed = true;
			}
			length = INF_convert(dbb->dbb_oldest_transaction, buffer);
			break;

		case isc_info_oldest_active:
			if (!header_refreshed)
			{
				const jrd_file* file = dbb->dbb_file;
				PAG_header(file->fil_string, file->fil_length);
				header_refreshed = true;
			}
		    length = INF_convert(dbb->dbb_oldest_active, buffer);
		    break;

		case isc_info_oldest_snapshot:
			if (!header_refreshed)
			{
				const jrd_file* file = dbb->dbb_file;
				PAG_header(file->fil_string, file->fil_length);
				header_refreshed = true;
			}
			length = INF_convert(dbb->dbb_oldest_snapshot, buffer);
			break;

		case isc_info_next_transaction:
			if (!header_refreshed)
			{
				const jrd_file* file = dbb->dbb_file;
				PAG_header(file->fil_string, file->fil_length);
				header_refreshed = true;
			}
			length = INF_convert(dbb->dbb_next_transaction, buffer);
			break;

		case isc_info_db_provider:
		    length = INF_convert(isc_info_db_code_firebird, buffer);
			break;

		case isc_info_db_class:
		    length = INF_convert(FB_ARCHITECTURE, buffer);
			break;

		case frb_info_att_charset:
			length = INF_convert(tdbb->tdbb_attachment->att_charset, buffer);
			break;

		case isc_info_internal:
			length = 0;

			if (items < end_items)
			{
				UCHAR privCode = *items++;

				switch (privCode)
				{
					case INF_internal_db_info_intl_char_length:
					case INF_internal_db_info_intl_octet_length:
					case INF_internal_db_info_intl_is_legacy_charset:
					{
						CharSet* charSet = items < end_items ? INTL_charset_lookup(tdbb, *items++) : NULL;

						if (privCode == INF_internal_db_info_intl_char_length ||
							privCode == INF_internal_db_info_intl_octet_length)
						{
							USHORT strLen = *reinterpret_cast<const USHORT*>(items);
							items += sizeof(USHORT);
							if (items + strLen <= end_items)
							{
								if (privCode == INF_internal_db_info_intl_char_length)
								{
									ULONG offendingPos;
									if (!charSet->wellFormed(strLen, (const UCHAR*)items, &offendingPos))
									{
										ERR_post(isc_sqlerr,
												 isc_arg_number, (SLONG) - 104,
												 isc_arg_gds, isc_malformed_string, 0);
									}
									else
									{
										length = INF_convert(charSet->length(tdbb, strLen,
											(const UCHAR*)items, true), buffer);
									}
								}
								else if (privCode == INF_internal_db_info_intl_octet_length)
								{
									Firebird::HalfStaticArray<UCHAR, 256> dummy;
									length = INF_convert(charSet->substring(tdbb, strLen, (const UCHAR*)items,
										strLen, dummy.getBuffer(strLen), 0, 
										strLen / charSet->maxBytesPerChar()), buffer);
								}
							}
							else
							{
								buffer[0] = item;
								item = isc_info_error;
								length = 1 + INF_convert(isc_infunk, buffer + 1);
								break;
							}

							items += strLen;
						}
						else if (privCode == INF_internal_db_info_intl_is_legacy_charset)
						{
							*p++ = charSet->getFlags() & CHARSET_LEGACY_SEMANTICS ? 1 : 0;
							length = 1;
						}

						break;
					}

					default:
						buffer[0] = item;
						item = isc_info_error;
						length = 1 + INF_convert(isc_infunk, buffer + 1);
						break;
				}
			}
			break;

		default:
			buffer[0] = item;
			item = isc_info_error;
			length = 1 + INF_convert(isc_infunk, buffer + 1);
			break;
		}
		if (!(info = INF_put_item(item, length, buffer, info, end))) {
			if (transaction)
				TRA_commit(tdbb, transaction, false);
			return FALSE;
		}
	}

	if (transaction)
		TRA_commit(tdbb, transaction, false);

	*info++ = isc_info_end;

	return TRUE;
}


SCHAR* INF_put_item(SCHAR item,
					USHORT length, const SCHAR* string, SCHAR* ptr,
					const SCHAR* end)
{
/**************************************
 *
 *	I N F _ p u t _ i t e m
 *
 **************************************
 *
 * Functional description
 *	Put information item in output buffer if there is room, and
 *	return an updated pointer.  If there isn't room for the item,
 *	indicate truncation and return NULL.
 *
 **************************************/

	if (ptr + length + 4 >= end) {
		*ptr = isc_info_truncated;
		return NULL;
	}

	// Typically, in other places, STUFF_WORD is applied to UCHAR*
	*ptr++ = item;
	STUFF_WORD(ptr, length);

	if (length) {
		MEMMOVE(string, ptr, length);
		ptr += length;
	}

	return ptr;
}


int INF_request_info(const jrd_req* request,
					 const SCHAR* items,
					 const SSHORT item_length,
					 SCHAR* info, const SSHORT output_length)
{
/**************************************
 *
 *	I N F _ r e q u e s t _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Return information about requests.
 *
 **************************************/
	jrd_nod* node;
	SSHORT state;
	USHORT length = 0;

	const SCHAR* const end_items = items + item_length;
	const SCHAR* const end = info + output_length;
	SCHAR buffer[256];
	memset(buffer, 0, sizeof(buffer));
	SCHAR* buffer_ptr = buffer;

	while (items < end_items && *items != isc_info_end) {
		SCHAR item = *items++;
		switch (item) {
		case isc_info_end:
			break;

		case isc_info_number_messages:
			length = INF_convert(request->req_nmsgs, buffer_ptr);
			break;

		case isc_info_max_message:
			length = INF_convert(request->req_mmsg, buffer_ptr);
			break;

		case isc_info_max_send:
			length = INF_convert(request->req_msend, buffer_ptr);
			break;

		case isc_info_max_receive:
			length = INF_convert(request->req_mreceive, buffer_ptr);
			break;

		case isc_info_req_select_count:
			length = INF_convert(request->req_records_selected, buffer_ptr);
			break;

		case isc_info_req_insert_count:
			length = INF_convert(request->req_records_inserted, buffer_ptr);
			break;

		case isc_info_req_update_count:
			length = INF_convert(request->req_records_updated, buffer_ptr);
			break;

		case isc_info_req_delete_count:
			length = INF_convert(request->req_records_deleted, buffer_ptr);
			break;

		case isc_info_access_path:

			/* the access path has the potential to be large, so if the default
			   buffer is not big enough, allocate a really large one--don't
			   continue to allocate larger and larger, because of the potential
			   for a bug which would bring the server to its knees */

			if (!OPT_access_path(request, buffer_ptr, sizeof(buffer), &length))
			{
				buffer_ptr = FB_NEW(*getDefaultMemoryPool()) char[BUFFER_XLARGE];
				OPT_access_path(request, buffer_ptr, BUFFER_XLARGE, &length);
			}
			break;

		case isc_info_state:
			state = isc_info_req_active;
			if (request->req_operation == jrd_req::req_send)
				state = isc_info_req_send;
			else if (request->req_operation == jrd_req::req_receive) {
				node = request->req_next;
				if (node->nod_type == nod_select)
					state = isc_info_req_select;
				else
					state = isc_info_req_receive;
			}
			else if ((request->req_operation == jrd_req::req_return) &&
					 (request->req_flags & req_stall))
			{
				state = isc_info_req_sql_stall;
			}
			if (!(request->req_flags & req_active))
				state = isc_info_req_inactive;
			length = INF_convert(state, buffer_ptr);
			break;

		case isc_info_message_number:
		case isc_info_message_size:
			if (!(request->req_flags & req_active) ||
				(request->req_operation != jrd_req::req_receive &&
				request->req_operation != jrd_req::req_send))
			{
				buffer_ptr[0] = item;
				item = isc_info_error;
				length = 1 + INF_convert(isc_infinap, buffer_ptr + 1);
				break;
			}
			node = request->req_message;
			if (item == isc_info_message_number)
				length =
					INF_convert((IPTR) node->nod_arg[e_msg_number],
								buffer_ptr);
			else {
				const Format* format = (Format*) node->nod_arg[e_msg_format];
				length = INF_convert(format->fmt_length, buffer_ptr);
			}
			break;

		case isc_info_request_cost:
		default:
			buffer_ptr[0] = item;
			item = isc_info_error;
			length = 1 + INF_convert(isc_infunk, buffer_ptr + 1);
			break;
		}

		info = INF_put_item(item, length, buffer_ptr, info, end);

		if (buffer_ptr != buffer) {
			delete[] buffer_ptr;
			buffer_ptr = buffer;
		}

		if (!info)
			return FALSE;
	}

	*info++ = isc_info_end;

	return TRUE;
}


int INF_transaction_info(const jrd_tra* transaction,
						 const SCHAR* items,
						 const SSHORT item_length,
						 SCHAR* info, const SSHORT output_length)
{
/**************************************
 *
 *	I N F _ t r a n s a c t i o n _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Process requests for blob info.
 *
 **************************************/
	SCHAR buffer[128];
	SSHORT length;

	const SCHAR* const end_items = items + item_length;
	const SCHAR* const end = info + output_length;

	while (items < end_items && *items != isc_info_end) {
		SCHAR item = *items++;
		switch (item) {
		case isc_info_end:
			break;

		case isc_info_tra_id:
			length = INF_convert(transaction->tra_number, buffer);
			break;

		default:
			buffer[0] = item;
			item = isc_info_error;
			length = 1 + INF_convert(isc_infunk, buffer + 1);
			break;
		}
		if (!(info = INF_put_item(item, length, buffer, info, end)))
			return FALSE;
	}

	*info++ = isc_info_end;

	return TRUE;
}


static USHORT get_counts(USHORT count_id, SCHAR* buffer, USHORT length)
{
/**************************************
 *
 *	g e t _ c o u n t s
 *
 **************************************
 *
 * Functional description
 *	Return operation counts for relation.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	const vcl* vector = tdbb->tdbb_attachment->att_counts[count_id];
	if (!vector)
		return 0;

	// CVC: This function was receiving UCHAR* but to avoid all the casts
	// when calling it, I changed it to SCHAR* and I'm doing here the cast
	// to avoid signed/unsigned surprises.
	UCHAR* p = reinterpret_cast<UCHAR*>(buffer);
	const UCHAR* const end = p + length - 6;

	USHORT relation_id = 0;
	for (vcl::const_iterator ptr = vector->begin();
		 relation_id < vector->count() && p < end; ++relation_id)
	{
		const SLONG n = *ptr++;
		if (n) {
			STUFF_WORD(p, relation_id);
			p += INF_convert(n, reinterpret_cast<char*>(p));
		}
	}

	return p - reinterpret_cast<UCHAR*>(buffer);
}


