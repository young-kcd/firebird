/*
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
 * Dmitry Yemanov
 * Sean Leyne
 * Alex Peshkoff
 * Adriano dos Santos Fernandes
 *
 */

#ifndef YVALVE_Y_OBJECTS_H
#define YVALVE_Y_OBJECTS_H

#include "firebird.h"
#include "firebird/Provider.h"
#include "gen/iberror.h"
#include "../common/common.h"
#include "../common/StatusHolder.h"
#include "../common/classes/alloc.h"
#include "../common/classes/array.h"

namespace Why
{


class YAttachment;
class YBlob;
class YRequest;
class YService;
class YStatement;
class YTransaction;

class YObject
{
public:
	YObject()
		: handle(0)
	{
	}

public:
	FB_API_HANDLE handle;
};

class CleanupCallback
{
public:
	virtual void FB_CARG cleanupCallbackFunction() = 0;
};

template <typename T>
class HandleArray
{
public:
	explicit HandleArray(Firebird::MemoryPool& pool)
		: array(pool)
	{
	}

	void add(T* obj)
	{
		Firebird::MutexLockGuard guard(mtx);

		array.add(obj);
	}

	void remove(T* obj)
	{
		Firebird::MutexLockGuard guard(mtx);
		size_t pos;

		if (array.find(obj, pos))
			array.remove(pos);
	}

	void destroy()
	{
		Firebird::MutexLockGuard guard(mtx);
		size_t i;

		while ((i = array.getCount()) > 0)
			array[i - 1]->destroy();
	}

	void assign(HandleArray& from)
	{
		clear();
		array.assign(from.array);
	}

	void clear()
	{
		array.clear();
	}

private:
	Firebird::Mutex mtx;
	Firebird::SortedArray<T*> array;
};

template <typename Impl, typename Intf, int Vers>
class YHelper : public Firebird::StdPlugin<Intf, Vers>, public YObject
{
public:
	YHelper(Intf* aNext);

	int FB_CARG release()
	{
		if (--this->refCounter == 0)
		{
			Impl* impl = static_cast<Impl*>(this);

			if (next)
			{
				++this->refCounter; // to be decremented in destroy()
				++this->refCounter; // to avoid recursion
				impl->destroy(); // destroy() must call release()
				--this->refCounter;
			}

			delete impl; // call correct destructor !
			return 0;
		}

		return 1;
	}

	Firebird::RefPtr<Intf> next;
};

class YEvents : public YHelper<YEvents, Firebird::IEvents, FB_EVENTS_VERSION>
{
public:
	static const ISC_STATUS ERROR_CODE = isc_bad_events_handle;

	YEvents(YAttachment* aAttachment, IEvents* aNext, Firebird::IEventCallback* aCallback);

	~YEvents()
	{
		if (deleteCallback)
			delete callback;
	}

	void destroy();

	// IEvents implementation
	virtual void FB_CARG cancel(Firebird::IStatus* status);

public:
	YAttachment* attachment;
	Firebird::IEventCallback* callback;
	bool deleteCallback;
};

class YRequest : public YHelper<YRequest, Firebird::IRequest, FB_REQUEST_VERSION>
{
public:
	static const ISC_STATUS ERROR_CODE = isc_bad_req_handle;

	YRequest(YAttachment* aAttachment, Firebird::IRequest* aNext);

	void destroy();

	// IRequest implementation
	virtual void FB_CARG receive(Firebird::IStatus* status, int level, unsigned int msgType,
		unsigned int length, unsigned char* message);
	virtual void FB_CARG send(Firebird::IStatus* status, int level, unsigned int msgType,
		unsigned int length, const unsigned char* message);
	virtual void FB_CARG getInfo(Firebird::IStatus* status, int level, unsigned int itemsLength,
		const unsigned char* items, unsigned int bufferLength, unsigned char* buffer);
	virtual void FB_CARG start(Firebird::IStatus* status, Firebird::ITransaction* transaction, int level);
	virtual void FB_CARG startAndSend(Firebird::IStatus* status, Firebird::ITransaction* transaction, int level,
		unsigned int msgType, unsigned int length, const unsigned char* message);
	virtual void FB_CARG unwind(Firebird::IStatus* status, int level);
	virtual void FB_CARG free(Firebird::IStatus* status);

public:
	YAttachment* attachment;
	FB_API_HANDLE* userHandle;
};

class YTransaction : public YHelper<YTransaction, Firebird::ITransaction, FB_TRANSACTION_VERSION>
{
public:
	static const ISC_STATUS ERROR_CODE = isc_bad_trans_handle;

	YTransaction(YAttachment* aAttachment, Firebird::ITransaction* aNext);

	void destroy();

	// ITransaction implementation
	virtual void FB_CARG getInfo(Firebird::IStatus* status, unsigned int itemsLength,
		const unsigned char* items, unsigned int bufferLength, unsigned char* buffer);
	virtual void FB_CARG prepare(Firebird::IStatus* status, unsigned int msgLength,
		const unsigned char* message);
	virtual void FB_CARG commit(Firebird::IStatus* status);
	virtual void FB_CARG commitRetaining(Firebird::IStatus* status);
	virtual void FB_CARG rollback(Firebird::IStatus* status);
	virtual void FB_CARG rollbackRetaining(Firebird::IStatus* status);
	virtual void FB_CARG disconnect(Firebird::IStatus* status);
	virtual ITransaction* FB_CARG join(Firebird::IStatus* status, Firebird::ITransaction* transaction);
	virtual ITransaction* FB_CARG validate(Firebird::IStatus* status, Firebird::IAttachment* testAtt);
	virtual YTransaction* FB_CARG enterDtc(Firebird::IStatus* status);

	void addCleanupHandler(Firebird::IStatus* status, CleanupCallback* callback);
	void selfCheck();

public:
	YAttachment* attachment;
	HandleArray<YBlob> childBlobs;
	Firebird::Array<CleanupCallback*> cleanupHandlers;

private:
	YTransaction(YTransaction* from)
		: YHelper<YTransaction, Firebird::ITransaction, FB_TRANSACTION_VERSION>(from->next),
		  attachment(from->attachment),
		  childBlobs(getPool()),
		  cleanupHandlers(getPool())
	{
		childBlobs.assign(from->childBlobs);
		from->childBlobs.clear();
		cleanupHandlers.assign(from->cleanupHandlers);
		from->cleanupHandlers.clear();
	}
};

class YBlob : public YHelper<YBlob, Firebird::IBlob, FB_BLOB_VERSION>
{
public:
	static const ISC_STATUS ERROR_CODE = isc_bad_segstr_handle;

	YBlob(YAttachment* aAttachment, YTransaction* aTransaction, Firebird::IBlob* aNext);

	void destroy();

	// IBlob implementation
	virtual void FB_CARG getInfo(Firebird::IStatus* status, unsigned int itemsLength,
		const unsigned char* items, unsigned int bufferLength, unsigned char* buffer);
	virtual unsigned int FB_CARG getSegment(Firebird::IStatus* status, unsigned int length, void* buffer);
	virtual void FB_CARG putSegment(Firebird::IStatus* status, unsigned int length, const void* buffer);
	virtual void FB_CARG cancel(Firebird::IStatus* status);
	virtual void FB_CARG close(Firebird::IStatus* status);
	virtual int FB_CARG seek(Firebird::IStatus* status, int mode, int offset);

public:
	YAttachment* attachment;
	YTransaction* transaction;
};

class YStatement : public YHelper<YStatement, Firebird::IStatement, FB_STATEMENT_VERSION>
{
public:
	static const ISC_STATUS ERROR_CODE = isc_bad_stmt_handle;

	YStatement(YAttachment* aAttachment, Firebird::IStatement* aNext);

	void destroy();

	// IStatement implementation

	void checkPrepared() const
	{
		if (!prepared)
			Firebird::status_exception::raise(Firebird::Arg::Gds(isc_unprepared_stmt));
	}

	virtual void FB_CARG prepare(Firebird::IStatus* status, Firebird::ITransaction* transaction,
		unsigned int stmtLength, const char* sqlStmt, unsigned int dialect, unsigned int flags);
	virtual void FB_CARG getInfo(Firebird::IStatus* status, unsigned int itemsLength,
		const unsigned char* items, unsigned int bufferLength, unsigned char* buffer);
	virtual unsigned FB_CARG getType(Firebird::IStatus* status);
	virtual const char* FB_CARG getPlan(Firebird::IStatus* status, bool detailed);
	virtual const Firebird::IParametersMetadata* FB_CARG getInputParameters(Firebird::IStatus* status);
	virtual const Firebird::IParametersMetadata* FB_CARG getOutputParameters(Firebird::IStatus* status);
	virtual ISC_UINT64 FB_CARG getAffectedRecords(Firebird::IStatus* status);
	virtual void FB_CARG setCursorName(Firebird::IStatus* status, const char* name);
	virtual YTransaction* FB_CARG execute(Firebird::IStatus* status, Firebird::ITransaction* transaction,
		unsigned int inMsgType, const Firebird::FbMessage* inMsgBuffer,
		const Firebird::FbMessage* outMsgBuffer);
	virtual int FB_CARG fetch(Firebird::IStatus* status, const Firebird::FbMessage* msgBuffer);
	virtual void FB_CARG insert(Firebird::IStatus* status, const Firebird::FbMessage* msgBuffer);
	virtual void FB_CARG free(Firebird::IStatus* status, unsigned int option);

public:
	YAttachment* attachment;
	FB_API_HANDLE* userHandle;
	bool prepared;
};

class EnterCount
{
public:
	EnterCount()
		: enterCount(0)
	{}

	~EnterCount()
	{
		fb_assert(enterCount == 0);
	}

	int enterCount;
	Firebird::Mutex enterMutex;
};

class YAttachment : public YHelper<YAttachment, Firebird::IAttachment, FB_ATTACHMENT_VERSION>, public EnterCount
{
public:
	static const ISC_STATUS ERROR_CODE = isc_bad_db_handle;

	explicit YAttachment(Firebird::IProvider* aProvider, Firebird::IAttachment* aNext,
		const Firebird::PathName& aDbPath);
	~YAttachment();

	void destroy();

	// IAttachment implementation
	virtual void FB_CARG getInfo(Firebird::IStatus* status, unsigned int itemsLength,
		const unsigned char* items, unsigned int bufferLength, unsigned char* buffer);
	virtual YTransaction* FB_CARG startTransaction(Firebird::IStatus* status, unsigned int tpbLength,
		const unsigned char* tpb);
	virtual YTransaction* FB_CARG reconnectTransaction(Firebird::IStatus* status, unsigned int length,
		const unsigned char* id);
	virtual YStatement* FB_CARG allocateStatement(Firebird::IStatus* status);
	virtual YRequest* FB_CARG compileRequest(Firebird::IStatus* status, unsigned int blrLength,
		const unsigned char* blr);
	virtual void FB_CARG transactRequest(Firebird::IStatus* status, Firebird::ITransaction* transaction,
		unsigned int blrLength, const unsigned char* blr, unsigned int inMsgLength,
		const unsigned char* inMsg, unsigned int outMsgLength, unsigned char* outMsg);
	virtual YBlob* FB_CARG createBlob(Firebird::IStatus* status, Firebird::ITransaction* transaction, ISC_QUAD* id,
		unsigned int bpbLength, const unsigned char* bpb);
	virtual YBlob* FB_CARG openBlob(Firebird::IStatus* status, Firebird::ITransaction* transaction, ISC_QUAD* id,
		unsigned int bpbLength, const unsigned char* bpb);
	virtual int FB_CARG getSlice(Firebird::IStatus* status, Firebird::ITransaction* transaction, ISC_QUAD* id,
		unsigned int sdlLength, const unsigned char* sdl, unsigned int paramLength,
		const unsigned char* param, int sliceLength, unsigned char* slice);
	virtual void FB_CARG putSlice(Firebird::IStatus* status, Firebird::ITransaction* transaction, ISC_QUAD* id,
		unsigned int sdlLength, const unsigned char* sdl, unsigned int paramLength,
		const unsigned char* param, int sliceLength, unsigned char* slice);
	virtual void FB_CARG ddl(Firebird::IStatus* status, Firebird::ITransaction* transaction, unsigned int length,
		const unsigned char* dyn);
	virtual YTransaction* FB_CARG execute(Firebird::IStatus* status, Firebird::ITransaction* transaction,
		unsigned int length, const char* string, unsigned int dialect, unsigned int inMsgType,
		const Firebird::FbMessage* inMsgBuffer, const Firebird::FbMessage* outMsgBuffer);
	virtual YEvents* FB_CARG queEvents(Firebird::IStatus* status, Firebird::IEventCallback* callback,
		unsigned int length, const unsigned char* eventsData);
	virtual void FB_CARG cancelOperation(Firebird::IStatus* status, int option);
	virtual void FB_CARG ping(Firebird::IStatus* status);
	virtual void FB_CARG detach(Firebird::IStatus* status);
	virtual void FB_CARG drop(Firebird::IStatus* status);

	void addCleanupHandler(Firebird::IStatus* status, CleanupCallback* callback);
	Firebird::ITransaction* getNextTransaction(Firebird::IStatus* status, Firebird::ITransaction* tra);

public:
	Firebird::IProvider* provider;
	Firebird::PathName dbPath;
	HandleArray<YBlob> childBlobs;
	HandleArray<YEvents> childEvents;
	HandleArray<YRequest> childRequests;
	HandleArray<YStatement> childStatements;
	HandleArray<YTransaction> childTransactions;
	Firebird::Array<CleanupCallback*> cleanupHandlers;
	Firebird::StatusHolder savedStatus;	// Do not use raise() method of this class in yValve.
};

class YService : public YHelper<YService, Firebird::IService, FB_SERVICE_VERSION>, public EnterCount
{
public:
	static const ISC_STATUS ERROR_CODE = isc_bad_svc_handle;

	explicit YService(Firebird::IProvider* aProvider, Firebird::IService* aNext);
	~YService();

	void destroy();

	// IService implementation
	virtual void FB_CARG detach(Firebird::IStatus* status);
	virtual void FB_CARG query(Firebird::IStatus* status,
		unsigned int sendLength, const unsigned char* sendItems,
		unsigned int receiveLength, const unsigned char* receiveItems,
		unsigned int bufferLength, unsigned char* buffer);
	virtual void FB_CARG start(Firebird::IStatus* status,
		unsigned int spbLength, const unsigned char* spb);

public:
	Firebird::IProvider* provider;
};

class Dispatcher : public Firebird::StdPlugin<Firebird::IProvider, FB_PROVIDER_VERSION>
{
public:
	void* operator new(size_t, void* memory) throw()
	{
		return memory;
	}

	// IProvider implementation
	virtual YAttachment* FB_CARG attachDatabase(Firebird::IStatus* status, const char* filename,
		unsigned int dpbLength, const unsigned char* dpb);
	virtual YAttachment* FB_CARG createDatabase(Firebird::IStatus* status, const char* filename,
		unsigned int dpbLength, const unsigned char* dpb);
	virtual YService* FB_CARG attachServiceManager(Firebird::IStatus* status, const char* serviceName,
		unsigned int spbLength, const unsigned char* spb);
	virtual void FB_CARG shutdown(Firebird::IStatus* status, unsigned int timeout, const int reason);

	virtual int FB_CARG release()
	{
		if (--refCounter == 0)
		{
			//delete this;
			return 0;
		}

		return 1;
	}
};


}	// namespace Why

#endif	// YVALVE_Y_OBJECTS_H
