/*
 *      PROGRAM:        JRD access method
 *      MODULE:         Attachment.cpp
 *      DESCRIPTION:    JRD Attachment class
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
 *
 */

#include "firebird.h"
#include "../jrd/Attachment.h"
#include "../jrd/Database.h"
#include "../jrd/nbak.h"
#include "../jrd/trace/TraceManager.h"
#include "../jrd/PreparedStatement.h"
#include "../jrd/tra.h"
#include "../jrd/intl.h"

#include "../jrd/blb_proto.h"
#include "../jrd/exe_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/met_proto.h"

#include "../jrd/extds/ExtDS.h"

#include "../common/classes/fb_string.h"
#include "../common/classes/MetaName.h"
#include "../common/StatusArg.h"


using namespace Jrd;
using namespace Firebird;


// static method
Jrd::Attachment* Jrd::Attachment::create(Database* dbb, FB_API_HANDLE publicHandle)
{
	MemoryPool* const pool = dbb->createPool();

	try
	{
		Attachment* const attachment = FB_NEW(*pool) Attachment(pool, dbb, publicHandle);
		pool->setStatsGroup(attachment->att_memory_stats);
		attachment->addRef();
		return attachment;
	}
	catch (const Firebird::Exception&)
	{
		dbb->deletePool(pool);
		throw;
	}
}


// static method
void Jrd::Attachment::destroy(Attachment* const attachment)
{
	if (!attachment)
		return;

	thread_db* tdbb = JRD_get_thread_data();

	jrd_tra* sysTransaction = attachment->getSysTransaction();
	if (sysTransaction)
	{
		// unwind any active system requests
		while (sysTransaction->tra_requests)
			EXE_unwind(tdbb, sysTransaction->tra_requests);

		jrd_tra::destroy(NULL, sysTransaction);
	}

	Database* const dbb = attachment->att_database;
	MemoryPool* const pool = attachment->att_pool;
	Firebird::MemoryStats temp_stats;
	pool->setStatsGroup(temp_stats);

	--(attachment->refCounter);
	delete attachment;

	dbb->deletePool(pool);
}


bool Jrd::Attachment::backupStateWriteLock(thread_db* tdbb, SSHORT wait)
{
	if (att_backup_state_counter++)
		return true;

	if (att_database->dbb_backup_manager->lockStateWrite(tdbb, wait))
		return true;

	att_backup_state_counter--;
	return false;
}


void Jrd::Attachment::backupStateWriteUnLock(thread_db* tdbb)
{
	if (--att_backup_state_counter == 0)
		att_database->dbb_backup_manager->unlockStateWrite(tdbb);
}


bool Jrd::Attachment::backupStateReadLock(thread_db* tdbb, SSHORT wait)
{
	if (att_backup_state_counter++)
		return true;

	if (att_database->dbb_backup_manager->lockStateRead(tdbb, wait))
		return true;

	att_backup_state_counter--;
	return false;
}


void Jrd::Attachment::backupStateReadUnLock(thread_db* tdbb)
{
	if (--att_backup_state_counter == 0)
		att_database->dbb_backup_manager->unlockStateRead(tdbb);
}


Jrd::Attachment::Attachment(MemoryPool* pool, Database* dbb, FB_API_HANDLE publicHandle)
	: att_pool(pool),
	  att_memory_stats(&dbb->dbb_memory_stats),
	  att_database(dbb),
	  att_public_handle(publicHandle),
	  att_requests(*pool),
	  att_lock_owner_id(Database::getLockOwnerId()),
	  att_backup_state_counter(0),
	  att_stats(*pool),
	  att_working_directory(*pool),
	  att_filename(*pool),
	  att_timestamp(Firebird::TimeStamp::getCurrentTimeStamp()),
	  att_context_vars(*pool),
	  ddlTriggersContext(*pool),
	  att_network_protocol(*pool),
	  att_remote_address(*pool),
	  att_remote_process(*pool),
	  att_dsql_cache(*pool),
	  att_udf_pointers(*pool),
	  att_ext_connection(NULL),
	  att_ext_call_depth(0),
	  att_trace_manager(FB_NEW(*att_pool) TraceManager(this))
{
	att_mutex.enter();
}


Jrd::Attachment::~Attachment()
{
	delete att_trace_manager;

	// For normal attachments that happens in release_attachment(),
	// but for special ones like GC should be done also in dtor -
	// they do not (and should not) call release_attachment().
	// It's no danger calling detachLocksFromAttachment()
	// once more here because it nulls att_long_locks.
	//		AP 2007
	detachLocksFromAttachment();
	att_mutex.leave();
}


Jrd::PreparedStatement* Jrd::Attachment::prepareStatement(thread_db* tdbb, jrd_tra* transaction,
	const string& text, Firebird::MemoryPool* pool)
{
	pool = pool ? pool : tdbb->getDefaultPool();
	return FB_NEW(*pool) PreparedStatement(tdbb, *pool, this, transaction, text, true);
}


Jrd::PreparedStatement* Jrd::Attachment::prepareStatement(thread_db* tdbb, jrd_tra* transaction,
	const PreparedStatement::Builder& builder, Firebird::MemoryPool* pool)
{
	pool = pool ? pool : tdbb->getDefaultPool();
	return FB_NEW(*pool) PreparedStatement(tdbb, *pool, this, transaction, builder, true);
}


PreparedStatement* Jrd::Attachment::prepareUserStatement(thread_db* tdbb, jrd_tra* transaction,
	const string& text, Firebird::MemoryPool* pool)
{
	pool = pool ? pool : tdbb->getDefaultPool();
	return FB_NEW(*pool) PreparedStatement(tdbb, *pool, this, transaction, text, false);
}


MetaName Jrd::Attachment::nameToMetaCharSet(thread_db* tdbb, const MetaName& name)
{
	if (att_charset == CS_METADATA || att_charset == CS_NONE)
		return name;

	UCHAR buffer[MAX_SQL_IDENTIFIER_SIZE];
	ULONG len = INTL_convert_bytes(tdbb, CS_METADATA, buffer, MAX_SQL_IDENTIFIER_LEN,
		att_charset, (const BYTE*) name.c_str(), name.length(), ERR_post);
	buffer[len] = '\0';

	return MetaName((const char*) buffer);
}


MetaName Jrd::Attachment::nameToUserCharSet(thread_db* tdbb, const MetaName& name)
{
	if (att_charset == CS_METADATA || att_charset == CS_NONE)
		return name;

	UCHAR buffer[MAX_SQL_IDENTIFIER_SIZE];
	ULONG len = INTL_convert_bytes(tdbb, att_charset, buffer, MAX_SQL_IDENTIFIER_LEN,
		CS_METADATA, (const BYTE*) name.c_str(), name.length(), ERR_post);
	buffer[len] = '\0';

	return MetaName((const char*) buffer);
}


string Jrd::Attachment::stringToMetaCharSet(thread_db* tdbb, const string& str,
	const char* charSet)
{
	USHORT charSetId = att_charset;

	if (charSet)
	{
		if (!MET_get_char_coll_subtype(tdbb, &charSetId, (const UCHAR*) charSet, strlen(charSet)))
			(Arg::Gds(isc_charset_not_found) << Arg::Str(charSet)).raise();
	}

	if (charSetId == CS_METADATA || charSetId == CS_NONE)
		return str;

	HalfStaticArray<UCHAR, BUFFER_MEDIUM> buffer(str.length() * sizeof(ULONG));
	ULONG len = INTL_convert_bytes(tdbb, CS_METADATA, buffer.begin(), buffer.getCapacity(),
		charSetId, (const BYTE*) str.c_str(), str.length(), ERR_post);

	return string((char*) buffer.begin(), len);
}


string Jrd::Attachment::stringToUserCharSet(thread_db* tdbb, const string& str)
{
	if (att_charset == CS_METADATA || att_charset == CS_NONE)
		return str;

	HalfStaticArray<UCHAR, BUFFER_MEDIUM> buffer(str.length() * sizeof(ULONG));
	ULONG len = INTL_convert_bytes(tdbb, att_charset, buffer.begin(), buffer.getCapacity(),
		CS_METADATA, (const BYTE*) str.c_str(), str.length(), ERR_post);

	return string((char*) buffer.begin(), len);
}


// We store in CS_METADATA.
void Jrd::Attachment::storeMetaDataBlob(thread_db* tdbb, jrd_tra* transaction,
	bid* blobId, const string& text, USHORT fromCharSet)
{
	UCharBuffer bpb;
	BLB_gen_bpb(isc_blob_text, isc_blob_text, fromCharSet, CS_METADATA, bpb);

	blb* blob = BLB_create2(tdbb, transaction, blobId, bpb.getCount(), bpb.begin());
	try
	{
		BLB_put_data(tdbb, blob, (const UCHAR*) text.c_str(), text.length());
	}
	catch (const Exception&)
	{
		BLB_close(tdbb, blob);
		throw;
	}

	BLB_close(tdbb, blob);
}


// We store raw stuff; don't attempt to translate.
void Jrd::Attachment::storeBinaryBlob(thread_db* tdbb, jrd_tra* transaction,
	bid* blobId, const ByteChunk& chunk)
{
	blb* blob = BLB_create2(tdbb, transaction, blobId, 0, NULL);
	try
	{
		BLB_put_data(tdbb, blob, chunk.data, chunk.length);
	}
	catch (const Exception&)
	{
		BLB_close(tdbb, blob);
		throw;
	}

	BLB_close(tdbb, blob);
}


void Jrd::Attachment::cancelExternalConnection(thread_db* tdbb)
{
	if (att_ext_connection) {
		att_ext_connection->cancelExecution(tdbb);
	}
}


void Jrd::Attachment::detachLocksFromAttachment()
{
/**************************************
 *
 *	d e t a c h L o c k s F r o m A t t a c h m e n t
 *
 **************************************
 *
 * Functional description
 * Bug #7781, need to null out the attachment pointer of all locks which
 * were hung off this attachment block, to ensure that the attachment
 * block doesn't get dereferenced after it is released
 *
 **************************************/
	Lock* long_lock = att_long_locks;
	while (long_lock)
	{
		Lock* next = long_lock->lck_next;
		long_lock->lck_attachment = NULL;
		long_lock->lck_next = NULL;
		long_lock->lck_prior = NULL;
		long_lock = next;
	}
	att_long_locks = NULL;
}
