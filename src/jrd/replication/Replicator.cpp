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
 *  Copyright (c) 2013 Dmitry Yemanov <dimitr@firebirdsql.org>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../jrd/jrd.h"
#include "../../common/classes/BlobWrapper.h"

#include "Config.h"
#include "Replicator.h"
#include "Utils.h"

using namespace Firebird;
using namespace Jrd;
using namespace Replication;


Replicator::Replicator(MemoryPool& pool,
					   Manager* manager,
					   const Guid& guid,
					   const MetaString& user)
	: m_manager(manager),
	  m_config(manager->getConfig()),
	  m_guid(guid),
	  m_user(user),
	  m_transactions(pool),
	  m_generators(pool)
{
}

void Replicator::flush(BatchBlock& block, FlushReason reason, ULONG flags)
{
	block.header.protocol = PROTOCOL_CURRENT_VERSION;
	block.header.length = (ULONG) block.buffer->getCount() - BLOCK_HEADER_SIZE;
	block.header.flags |= flags;
	block.header.timestamp = TimeZoneUtil::getCurrentGmtTimeStamp().utc_timestamp;

	// Re-write the updated header

	auto ptr = block.buffer->begin();

	// Transaction number
	put_vax_int64(ptr, block.header.traNumber);
	ptr += sizeof(SINT64);
	// Protocol version
	put_vax_long(ptr, block.header.protocol);
	ptr += sizeof(SLONG);
	// Block length
	put_vax_long(ptr, block.header.length);
	ptr += sizeof(SLONG);
	// Flags
	put_vax_long(ptr, block.header.flags);
	ptr += sizeof(SLONG);
	// Timestamp (date part)
	put_vax_long(ptr, block.header.timestamp.timestamp_date);
	ptr += sizeof(SLONG);
	// Timestamp (time part)
	put_vax_long(ptr, block.header.timestamp.timestamp_time);
	ptr += sizeof(SLONG);

	fb_assert(ptr == block.buffer->begin() + BLOCK_HEADER_SIZE);

	// Pass the buffer to the replication manager and setup the new one

	const auto traNumber = block.header.traNumber;
	const auto sync = (reason == FLUSH_SYNC);
	m_manager->flush(block.buffer, sync);

	memset(&block.header, 0, sizeof(Block));
	block.header.traNumber = traNumber;

	block.atoms.clear();
	block.lastAtom = MAX_ULONG;
	block.buffer = m_manager->getBuffer();
	block.flushes++;
}

void Replicator::storeBlob(Transaction* transaction, ISC_QUAD blobId)
{
	FbLocalStatus localStatus;

	BlobWrapper blob(&localStatus);
	if (!blob.open(m_attachment, transaction->getInterface(), blobId))
		localStatus.raise();

	UCharBuffer buffer;
	const auto bufferLength = MAX_USHORT;
	auto data = buffer.getBuffer(bufferLength);

	auto& txnData = transaction->getData();
	bool newOp = true;

	FB_SIZE_T segmentLength;
	while (blob.getSegment(bufferLength, data, segmentLength))
	{
		if (!segmentLength)
			continue; // Zero-length segments are unusual but OK

		fb_assert(segmentLength <= MAX_USHORT);

		if (newOp)
		{
			txnData.putByte(opBlobData);
			txnData.putInt32(blobId.gds_quad_high);
			txnData.putInt32(blobId.gds_quad_low);
			newOp = false;
		}

		txnData.putInt16(segmentLength);
		txnData.putBinary(segmentLength, data);

		if (txnData.getSize() > m_config->bufferSize)
		{
			flush(txnData, FLUSH_OVERFLOW);
			newOp = true;
		}
	}

	localStatus.check();

	blob.close();

	if (newOp)
	{
		txnData.putByte(opBlobData);
		txnData.putInt32(blobId.gds_quad_high);
		txnData.putInt32(blobId.gds_quad_low);
	}

	txnData.putInt16(0); // end-of-blob marker

	if (txnData.getSize() > m_config->bufferSize)
		flush(txnData, FLUSH_OVERFLOW);
}

void Replicator::releaseTransaction(Transaction* transaction)
{
	try
	{
		MutexLockGuard guard(m_mutex, FB_FUNCTION);

		auto& txnData = transaction->getData();
		m_manager->releaseBuffer(txnData.buffer);

		FB_SIZE_T pos;
		if (m_transactions.find(transaction, pos))
			m_transactions.remove(pos);
	}
	catch (const Exception&)
	{} // no-op
}

// IReplicatedSession implementation

IReplicatedTransaction* Replicator::startTransaction(CheckStatusWrapper* status, ITransaction* trans, SINT64 number)
{
	AutoPtr<Transaction> transaction;

	try
	{
		MutexLockGuard guard(m_mutex, FB_FUNCTION);

		MemoryPool& pool = getPool();
		transaction = FB_NEW_POOL(pool) Transaction(this, trans);
		m_transactions.add(transaction);

		auto& txnData = transaction->getData();

		fb_assert(!txnData.header.traNumber);
		txnData.header.traNumber = number;
		txnData.header.flags = BLOCK_BEGIN_TRANS;

		txnData.buffer = m_manager->getBuffer();
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}

	return transaction.release();
}

void Replicator::prepareTransaction(CheckStatusWrapper* status, Transaction* transaction)
{
	try
	{
		MutexLockGuard guard(m_mutex, FB_FUNCTION);

		auto& txnData = transaction->getData();

		txnData.putByte(opPrepareTransaction);

		flush(txnData, FLUSH_PREPARE);
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}
}

void Replicator::commitTransaction(CheckStatusWrapper* status, Transaction* transaction)
{
	try
	{
		MutexLockGuard guard(m_mutex, FB_FUNCTION);

		auto& txnData = transaction->getData();

		// Assert this transaction being de-facto dirty
		fb_assert(txnData.flushes || txnData.buffer->getCount() > BLOCK_HEADER_SIZE);

		for (const auto& generator : m_generators)
		{
			fb_assert(generator.name.hasData());

			const auto atom = txnData.defineAtom(generator.name);

			txnData.putByte(opSetSequence);
			txnData.putInt32(atom);
			txnData.putInt64(generator.value);
		}

		m_generators.clear();

		txnData.putByte(opCommitTransaction);
		flush(txnData, FLUSH_SYNC, BLOCK_END_TRANS);
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}
}

void Replicator::rollbackTransaction(CheckStatusWrapper* status, Transaction* transaction)
{
	try
	{
		MutexLockGuard guard(m_mutex, FB_FUNCTION);

		auto& txnData = transaction->getData();

		if (txnData.flushes)
		{
			txnData.putByte(opRollbackTransaction);
			flush(txnData, FLUSH_SYNC, BLOCK_END_TRANS);
		}
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}
}

void Replicator::startSavepoint(CheckStatusWrapper* status, Transaction* transaction)
{
	try
	{
		MutexLockGuard guard(m_mutex, FB_FUNCTION);

		auto& txnData = transaction->getData();

		txnData.putByte(opStartSavepoint);

		if (txnData.getSize() > m_config->bufferSize)
			flush(txnData, FLUSH_OVERFLOW);
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}
}

void Replicator::releaseSavepoint(CheckStatusWrapper* status, Transaction* transaction)
{
	try
	{
		MutexLockGuard guard(m_mutex, FB_FUNCTION);

		auto& txnData = transaction->getData();

		txnData.putByte(opReleaseSavepoint);

		if (txnData.getSize() > m_config->bufferSize)
			flush(txnData, FLUSH_OVERFLOW);
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}
}

void Replicator::rollbackSavepoint(CheckStatusWrapper* status, Transaction* transaction)
{
	try
	{
		MutexLockGuard guard(m_mutex, FB_FUNCTION);

		auto& txnData = transaction->getData();

		txnData.putByte(opRollbackSavepoint);

		flush(txnData, FLUSH_SYNC);
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}
}

void Replicator::insertRecord(CheckStatusWrapper* status,
							  Transaction* transaction,
							  const char* relName,
							  IReplicatedRecord* record)
{
	try
	{
		for (unsigned id = 0; id < record->getCount(); id++)
		{
			IReplicatedField* field = record->getField(id);
			if (field != nullptr)
			{
				auto type = field->getType();
				if (type == SQL_ARRAY || type == SQL_BLOB)
				{
					const auto blobId = (ISC_QUAD*) field->getData();

					if (blobId)
						storeBlob(transaction, *blobId);
				}
			}
		}

		MutexLockGuard guard(m_mutex, FB_FUNCTION);

		const auto length = record->getRawLength();
		const auto data = record->getRawData();

		auto& txnData = transaction->getData();

		const auto atom = txnData.defineAtom(relName);

		txnData.putByte(opInsertRecord);
		txnData.putInt32(atom);
		txnData.putInt32(length);
		txnData.putBinary(length, data);

		if (txnData.getSize() > m_config->bufferSize)
			flush(txnData, FLUSH_OVERFLOW);
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}
}

void Replicator::updateRecord(CheckStatusWrapper* status,
							  Transaction* transaction,
							  const char* relName,
							  IReplicatedRecord* orgRecord,
							  IReplicatedRecord* newRecord)
{
	try
	{
		for (unsigned id = 0; id < newRecord->getCount(); id++)
		{
			IReplicatedField* field = newRecord->getField(id);
			if (field != nullptr)
			{
				auto type = field->getType();
				if (type == SQL_ARRAY || type == SQL_BLOB)
				{
					const auto blobId = (ISC_QUAD*) field->getData();

					if (blobId)
						storeBlob(transaction, *blobId);
				}
			}
		}

		MutexLockGuard guard(m_mutex, FB_FUNCTION);

		const auto orgLength = orgRecord->getRawLength();
		const auto orgData = orgRecord->getRawData();

		const auto newLength = newRecord->getRawLength();
		const auto newData = newRecord->getRawData();

		auto& txnData = transaction->getData();

		const auto atom = txnData.defineAtom(relName);

		txnData.putByte(opUpdateRecord);
		txnData.putInt32(atom);
		txnData.putInt32(orgLength);
		txnData.putBinary(orgLength, orgData);
		txnData.putInt32(newLength);
		txnData.putBinary(newLength, newData);

		if (txnData.getSize() > m_config->bufferSize)
			flush(txnData, FLUSH_OVERFLOW);
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}
}

void Replicator::deleteRecord(CheckStatusWrapper* status,
							  Transaction* transaction,
							  const char* relName,
							  IReplicatedRecord* record)
{
	try
	{
		MutexLockGuard guard(m_mutex, FB_FUNCTION);

		const auto length = record->getRawLength();
		const auto data = record->getRawData();

		auto& txnData = transaction->getData();

		const auto atom = txnData.defineAtom(relName);

		txnData.putByte(opDeleteRecord);
		txnData.putInt32(atom);
		txnData.putInt32(length);
		txnData.putBinary(length, data);

		if (txnData.getSize() > m_config->bufferSize)
			flush(txnData, FLUSH_OVERFLOW);
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}
}

void Replicator::executeSqlIntl(CheckStatusWrapper* status,
								Transaction* transaction,
								unsigned charset,
								const char* sql)
{
	try
	{
		MutexLockGuard guard(m_mutex, FB_FUNCTION);

		auto& txnData = transaction->getData();

		const auto atom = txnData.defineAtom(m_user);

		txnData.putByte(opExecuteSqlIntl);
		txnData.putInt32(atom);
		txnData.putByte(charset);
		txnData.putString(sql);

		if (txnData.getSize() > m_config->bufferSize)
			flush(txnData, FLUSH_OVERFLOW);
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}
}

void Replicator::cleanupTransaction(CheckStatusWrapper* status,
									SINT64 number)
{
	try
	{
		MutexLockGuard guard(m_mutex, FB_FUNCTION);

		BatchBlock block(getPool());
		block.header.traNumber = number;
		block.buffer = m_manager->getBuffer();
		block.putByte(opCleanupTransaction);

		flush(block, FLUSH_SYNC, BLOCK_END_TRANS);
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}
}

void Replicator::setSequence(CheckStatusWrapper* status,
							 const char* genName,
							 SINT64 value)
{
	try
	{
		MutexLockGuard guard(m_mutex, FB_FUNCTION);

		for (auto& generator : m_generators)
		{
			if (generator.name == genName)
			{
				generator.value = value;
				return;
			}
		}

		GeneratorValue generator;
		generator.name = genName;
		generator.value = value;

		m_generators.add(generator);
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}
}
