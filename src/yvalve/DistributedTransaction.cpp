/*
 *	PROGRAM:		Firebird interface.
 *	MODULE:			DistributedTransaction.cpp
 *	DESCRIPTION:	DTC and distributed transaction (also known as 2PC).
 *
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
 *  The Original Code was created by Alex Peshkov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2011 Alex Peshkov <peshkoff at mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#include "firebird.h"

#include "../yvalve/MasterImplementation.h"
#include "../yvalve/YObjects.h"
#include "../common/classes/ImplementHelper.h"
#include "../common/classes/rwlock.h"
#include "../common/classes/array.h"
#include "../common/StatusHolder.h"
#include "../jrd/inf_pub.h"
#include "../common/isc_proto.h"
#include "../jrd/acl.h"

using namespace Firebird;
using namespace Why;

namespace {

class DTransaction FB_FINAL : public RefCntIface<ITransactionImpl<DTransaction, CheckStatusWrapper> >
{
public:
	DTransaction()
		: sub(getPool()), limbo(false)
	{ }

	// ITransaction implementation
	int release()
	{
		if (--refCounter == 0)
		{
			delete this;
			return 0;
		}

		return 1;
	}

	void getInfo(CheckStatusWrapper* status, unsigned int itemsLength,
		const unsigned char* items, unsigned int bufferLength, unsigned char* buffer);
	void prepare(CheckStatusWrapper* status, unsigned int msgLength,
		const unsigned char* message);
	void commit(CheckStatusWrapper* status);
	void commitRetaining(CheckStatusWrapper* status);
	void rollback(CheckStatusWrapper* status);
	void rollbackRetaining(CheckStatusWrapper* status);
	void disconnect(CheckStatusWrapper* status);
	DTransaction* join(CheckStatusWrapper* status, ITransaction* transaction);
	ITransaction* validate(CheckStatusWrapper* status, IAttachment* attachment);
	DTransaction* enterDtc(CheckStatusWrapper* status);

private:
	typedef HalfStaticArray<ITransaction*, 8> SubArray;
	typedef HalfStaticArray<UCHAR, 1024> TdrBuffer;
	SubArray sub;
	RWLock rwLock;
	bool limbo;

	explicit DTransaction(const SubArray& aSub)
		: sub(getPool()), limbo(false)
	{
		sub.assign(aSub);
	}

	bool prepareCommit(CheckStatusWrapper* status, TdrBuffer& tdr);
	bool buildPrepareInfo(CheckStatusWrapper* status, TdrBuffer& tdr, ITransaction* from);

	~DTransaction();
};

bool DTransaction::buildPrepareInfo(CheckStatusWrapper* status, TdrBuffer& tdr, ITransaction* from)
{
	// Information items for two phase commit.
	static const UCHAR PREPARE_TR_INFO[] =
	{
		fb_info_tra_dbpath,
		isc_info_tra_id
	};

	Array<UCHAR> bigBuffer;
	// we need something really big here
	// output of chaining distributed transaction can be huge
	// limit MAX_USHORT is chosen cause for old API it was limit of all blocks
	UCHAR* buf = bigBuffer.getBuffer(MAX_USHORT);
	from->getInfo(status, sizeof(PREPARE_TR_INFO), PREPARE_TR_INFO, bigBuffer.getCount(), buf);
	if (status->getStatus() & Firebird::IStatus::FB_HAS_ERRORS)
		return false;

	UCHAR* const end = bigBuffer.end();

	while (buf < end)
	{
		UCHAR item = buf[0];
		++buf;
		const USHORT length = (USHORT) gds__vax_integer(buf, 2);
		// Prevent information out of sync.
		UCHAR lengthByte = length > MAX_UCHAR ? MAX_UCHAR : length;
		buf += 2;

		switch(item)
		{
			case isc_info_tra_id:
				tdr.add(TDR_TRANSACTION_ID);
				tdr.add(lengthByte);
				tdr.add(buf, lengthByte);
				break;

			case fb_info_tra_dbpath:
				tdr.add(TDR_DATABASE_PATH);
				tdr.add(lengthByte);
				tdr.add(buf, lengthByte);
				break;

			case isc_info_end:
				return true;
		}

		buf += length;
	}

	return true;
}

bool DTransaction::prepareCommit(CheckStatusWrapper* status, TdrBuffer& tdr)
{
	TEXT host[64];
	ISC_get_host(host, sizeof(host));
	const UCHAR hostlen = static_cast<UCHAR>(strlen(host));

	// Build a transaction description record containing the host site and database/transaction
	// information for the target databases.
	tdr.clear();
	tdr.add(TDR_VERSION);
	tdr.add(TDR_HOST_SITE);
	tdr.add(hostlen);
	tdr.add(reinterpret_cast<UCHAR*>(host), hostlen);

	// Get database and transaction stuff for each sub-transaction.

	for (unsigned i = 0; i < sub.getCount(); ++i)
	{
		if (sub[i])
		{
			if (! buildPrepareInfo(status, tdr, sub[i]))
				return false;
		}
	}

	return true;
}

void DTransaction::getInfo(CheckStatusWrapper* status,
								   unsigned int itemsLength, const unsigned char* items,
								   unsigned int bufferLength, unsigned char* buffer)
{
	try
	{
		status->init();

		ReadLockGuard guard(rwLock, FB_FUNCTION);

		for (unsigned int i = 0; i < sub.getCount(); ++i)
		{
			if (sub[i])
			{
				sub[i]->getInfo(status, itemsLength, items, bufferLength, buffer);
				if (status->getStatus() & Firebird::IStatus::FB_HAS_ERRORS)
				{
					return;
				}

				unsigned char* const end = buffer + bufferLength;
				while (buffer < end && (buffer[0] == isc_info_tra_id || buffer[0] == fb_info_tra_dbpath))
				{
					buffer += 3 + gds__vax_integer(&buffer[1], 2);
				}

				if (buffer >= end || buffer[0] != isc_info_end)
				{
					return;
				}

				bufferLength = end - buffer;
			}
		}
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}
}

void DTransaction::prepare(CheckStatusWrapper* status,
								   unsigned int msgLength, const unsigned char* message)
{
	try
	{
		status->init();

		WriteLockGuard guard(rwLock, FB_FUNCTION);

		if (limbo)
			return;

		TdrBuffer tdr;
		if (!msgLength)
		{
			if (!prepareCommit(status, tdr))
				return;

			msgLength = tdr.getCount();
			message = tdr.begin();
		}

		for (unsigned int i = 0; i < sub.getCount(); ++i)
		{
			if (sub[i])
			{
				sub[i]->prepare(status, msgLength, message);

				if (status->getStatus() & Firebird::IStatus::FB_HAS_ERRORS)
					return;
			}
		}

		limbo = true;
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}
}

void DTransaction::commit(CheckStatusWrapper* status)
{
	try
	{
		status->init();

		prepare(status, 0, NULL);
		if (status->getStatus() & Firebird::IStatus::FB_HAS_ERRORS)
		{
			return;
		}

		{	// guard scope
			WriteLockGuard guard(rwLock, FB_FUNCTION);

			for (unsigned int i = 0; i < sub.getCount(); ++i)
			{
				if (sub[i])
				{
					sub[i]->commit(status);
					if (status->getStatus() & Firebird::IStatus::FB_HAS_ERRORS)
						return;

					sub[i] = NULL;
				}
			}
		}
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}
}

/*
 *	I see problems with this approach, but keep it 'as was' for a while
 */
void DTransaction::commitRetaining(CheckStatusWrapper* status)
{
	try
	{
		status->init();

		WriteLockGuard guard(rwLock, FB_FUNCTION);

		for (unsigned int i = 0; i < sub.getCount(); ++i)
		{
			if (sub[i])
			{
				sub[i]->commitRetaining(status);
				if (status->getStatus() & Firebird::IStatus::FB_HAS_ERRORS)
					return;
			}
		}

		limbo = true;	// ASF: why do retaining marks limbo?
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}
}

void DTransaction::rollback(CheckStatusWrapper* status)
{
	try
	{
		status->init();

		{	// guard scope
			WriteLockGuard guard(rwLock, FB_FUNCTION);

			for (unsigned int i = 0; i < sub.getCount(); ++i)
			{
				if (sub[i])
				{
					sub[i]->rollback(status);
					if (status->getStatus() & Firebird::IStatus::FB_HAS_ERRORS)
						return;

					sub[i] = NULL;
				}
			}
		}
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}
}

void DTransaction::rollbackRetaining(CheckStatusWrapper* status)
{
	try
	{
		status->init();

		WriteLockGuard guard(rwLock, FB_FUNCTION);

		for (unsigned int i = 0; i < sub.getCount(); ++i)
		{
			if (sub[i])
			{
				sub[i]->rollbackRetaining(status);
				if (status->getStatus() & Firebird::IStatus::FB_HAS_ERRORS)
					return;
			}
		}

		limbo = true;	// ASF: why do retaining marks limbo?
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}
}

void DTransaction::disconnect(CheckStatusWrapper* status)
{
	try
	{
		status->init();

		WriteLockGuard guard(rwLock, FB_FUNCTION);

		if (!limbo)
			status_exception::raise(Arg::Gds(isc_no_recon));

		for (unsigned int i = 0; i < sub.getCount(); ++i)
		{
			if (sub[i])
			{
				sub[i]->disconnect(status);
				if (status->getStatus() & Firebird::IStatus::FB_HAS_ERRORS)
					return;

				sub[i] = NULL;
			}
		}
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}
}

// To do: check the maximum allowed dbs in a two phase commit.
//		  Q: what is the maximum?
DTransaction* DTransaction::join(CheckStatusWrapper* status, ITransaction* transaction)
{
	try
	{
		status->init();

		WriteLockGuard guard(rwLock, FB_FUNCTION);

		// reserve array element to make sure we have a place for copy of transaction
		FB_SIZE_T pos = sub.add(NULL);

		// Nothing throws exceptions after reserving space in sub-array up to the end of block
		ITransaction* traCopy = transaction->enterDtc(status);
		if (traCopy)
		{
			sub[pos] = traCopy;
			return this;
		}

		// enterDtc() failed - remove reserved array element
		sub.remove(pos);
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}

	return NULL;
}

ITransaction* DTransaction::validate(CheckStatusWrapper* status, IAttachment* attachment)
{
	try
	{
		status->init();

		ReadLockGuard guard(rwLock, FB_FUNCTION);

		for (unsigned int i = 0; i < sub.getCount(); ++i)
		{
			ITransaction* rc = sub[i]->validate(status, attachment);

			if (rc)
				return rc;
		}

		Arg::Gds(isc_bad_trans_handle).raise();
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}

	return NULL;
}

DTransaction* DTransaction::enterDtc(CheckStatusWrapper* status)
{
	try
	{
		status->init();

		WriteLockGuard guard(rwLock, FB_FUNCTION);

		RefPtr<DTransaction> traCopy(new DTransaction(sub));
		sub.clear();
		release();

		traCopy->addRef();
		return traCopy;
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}

	return NULL;
}

DTransaction::~DTransaction()
{
	for (unsigned int i = 0; i < sub.getCount(); ++i)
	{
		if (sub[i])
			sub[i]->release();
	}
}

} // anonymous namespace


namespace Why {

YTransaction* Dtc::start(CheckStatusWrapper* status, IDtcStart* components)
{
	try
	{
		status->init();

		RefPtr<DTransaction> dtransaction(new DTransaction);

		unsigned cnt = components->getCount(status);
		if (status->getStatus() & Firebird::IStatus::FB_HAS_ERRORS)
			status_exception::raise(status);

		for (unsigned i = 0; i < cnt; ++i)
		{
			RefPtr<IAttachment> att(REF_NO_INCR, components->getAttachment(status, i));
			if (status->getStatus() & Firebird::IStatus::FB_HAS_ERRORS)
				status_exception::raise(status);

			unsigned tpbLen;
			const unsigned char* tpb = components->getTpb(status, i, &tpbLen);
			if (status->getStatus() & Firebird::IStatus::FB_HAS_ERRORS)
				status_exception::raise(status);

			ITransaction* started = att->startTransaction(status, tpbLen, tpb);
			if (status->getStatus() & Firebird::IStatus::FB_HAS_ERRORS)
				status_exception::raise(status);

			dtransaction->join(status, started);
			if (status->getStatus() & Firebird::IStatus::FB_HAS_ERRORS)
			{
				started->release();
				status_exception::raise(status);
			}
		}

		YTransaction* rc = new YTransaction(NULL, dtransaction);
		dtransaction->addRef();
		components->dispose();
		return rc;
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}

	return NULL;
}

YTransaction* Dtc::join(CheckStatusWrapper* status, ITransaction* one, ITransaction* two)
{
	try
	{
		status->init();

		RefPtr<DTransaction> dtransaction(new DTransaction);

		dtransaction->join(status, one);
		if (status->getStatus() & Firebird::IStatus::FB_HAS_ERRORS)
			return NULL;

		dtransaction->join(status, two);
		/* We must not return NULL - first transaction is available only inside dtransaction
		if (status->getStatus() & Firebird::IStatus::FB_HAS_ERRORS)
			return NULL;
		*/

		dtransaction->addRef();
		return new YTransaction(NULL, dtransaction);
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}

	return NULL;
}

class DtcStart : public DisposeIface<IDtcStartImpl<DtcStart, CheckStatusWrapper> >
{
public:
	DtcStart()
		: components(getPool())
	{ }

	// IDtcStart implementation
	void setComponent(CheckStatusWrapper* status, IAttachment* att)
	{
		this->setWithParam(status, att, 0, NULL);
	}

	void setWithParam(CheckStatusWrapper* status, IAttachment* att, unsigned length, const unsigned char* tpb)
	{
		try
		{
			Component toAdd;
			toAdd.att = att;
			toAdd.tpbLen = length;
			toAdd.tpb = tpb;

			components.add(toAdd);
			att->addRef();
		}
		catch (const Exception& ex)
		{
			ex.stuffException(status);
		}
	}

	unsigned getCount(CheckStatusWrapper*)
	{
		return components.getCount();
	}

	IAttachment* getAttachment(CheckStatusWrapper* status, unsigned pos)
	{
		try
		{
			errorOver(pos);

			components[pos].att->addRef();
			return components[pos].att;
		}
		catch (const Exception& ex)
		{
			ex.stuffException(status);
		}
		return NULL;
	}

	const unsigned char* getTpb(CheckStatusWrapper* status, unsigned pos, unsigned* length)
	{
		try
		{
			errorOver(pos);

			*length = components[pos].tpbLen;
			return components[pos].tpb;
		}
		catch (const Exception& ex)
		{
			ex.stuffException(status);
		}
		return NULL;
	}

	void dispose()
	{
		for (unsigned i = 0; i < components.getCount(); ++i)
			components[i].att->release();

		delete this;
	}

private:
	struct Component
	{
		IAttachment* att;
		unsigned tpbLen;
		const unsigned char* tpb;
	};

	HalfStaticArray<Component, 16> components;

	void errorOver(unsigned n)
	{
		// TODO: add component num & limit to the message
		if (n >= components.getCount())
			(Arg::Gds(isc_random) << "Access to invalid component number in DtcStart").raise();
	}
};

IDtcStart* Dtc::startBuilder(CheckStatusWrapper* status)
{
	try
	{
		status->init();

		return new DtcStart;
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}

	return NULL;
}

} // namespace Why
