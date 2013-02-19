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
#include "../common/StatusHolder.h"
#include "../common/classes/alloc.h"
#include "../common/classes/array.h"

namespace Firebird
{
	class ClumpletWriter;
}

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

protected:
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
		Firebird::MutexLockGuard guard(mtx, FB_FUNCTION);

		array.add(obj);
	}

	void remove(T* obj)
	{
		Firebird::MutexLockGuard guard(mtx, FB_FUNCTION);
		size_t pos;

		if (array.find(obj, pos))
			array.remove(pos);
	}

	void destroy()
	{
		Firebird::MutexLockGuard guard(mtx, FB_FUNCTION);
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
	explicit YHelper(Intf* aNext);

	int FB_CARG release()
	{
		if (--this->refCounter == 0)
		{
			Impl* impl = static_cast<Impl*>(this);

			if (next)
			{
				++this->refCounter;		// to be decremented in destroy()
				++this->refCounter;		// to avoid recursion
				impl->destroy();		// destroy() must call release()
				--this->refCounter;
			}

			delete impl; // call correct destructor !
			return 0;
		}

		return 1;
	}

	void destroy2()
	{
		next = NULL;
		release();
	}

	typedef Intf NextInterface;
	typedef YAttachment YRef;
	Firebird::RefPtr<NextInterface> next;
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
	FB_API_HANDLE& getHandle();

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
	FB_API_HANDLE& getHandle();

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
	FB_API_HANDLE& getHandle();

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

typedef Firebird::RefPtr<Firebird::ITransaction> NextTransaction;

class YBlob : public YHelper<YBlob, Firebird::IBlob, FB_BLOB_VERSION>
{
public:
	static const ISC_STATUS ERROR_CODE = isc_bad_segstr_handle;

	YBlob(YAttachment* aAttachment, YTransaction* aTransaction, Firebird::IBlob* aNext);

	void destroy();
	FB_API_HANDLE& getHandle();

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

class YResultSet : public YHelper<YResultSet, Firebird::IResultSet, FB_RESULTSET_VERSION>
{
public:
	static const ISC_STATUS ERROR_CODE = isc_bad_result_set;

	YResultSet(YAttachment* aAttachment, Firebird::IResultSet* aNext);
	YResultSet(YAttachment* aAttachment, YStatement* aStatement, Firebird::IResultSet* aNext);

	void destroy();

	// IResultSet implementation
	virtual FB_BOOLEAN FB_CARG fetch(Firebird::IStatus* status, unsigned char* message);
	virtual FB_BOOLEAN FB_CARG isEof(Firebird::IStatus* status);
	virtual Firebird::IMessageMetadata* FB_CARG getMetadata(Firebird::IStatus* status);
	virtual void FB_CARG close(Firebird::IStatus* status);

public:
	YAttachment* attachment;
	YStatement* statement;
};

class YStatement : public YHelper<YStatement, Firebird::IStatement, FB_STATEMENT_VERSION>
{
public:
	static const ISC_STATUS ERROR_CODE = isc_bad_stmt_handle;

	YStatement(YAttachment* aAttachment, Firebird::IStatement* aNext);

	void destroy();

	// IStatement implementation
	virtual void FB_CARG getInfo(Firebird::IStatus* status,
								 unsigned int itemsLength, const unsigned char* items,
								 unsigned int bufferLength, unsigned char* buffer);
	virtual unsigned FB_CARG getType(Firebird::IStatus* status);
	virtual const char* FB_CARG getPlan(Firebird::IStatus* status, FB_BOOLEAN detailed);
	virtual ISC_UINT64 FB_CARG getAffectedRecords(Firebird::IStatus* status);
	virtual Firebird::IMessageMetadata* FB_CARG getInputMetadata(Firebird::IStatus* status);
	virtual Firebird::IMessageMetadata* FB_CARG getOutputMetadata(Firebird::IStatus* status);
	virtual Firebird::ITransaction* FB_CARG execute(Firebird::IStatus* status, Firebird::ITransaction* transaction,
		Firebird::FbMessage* in, Firebird::FbMessage *out);
	virtual Firebird::IResultSet* FB_CARG openCursor(Firebird::IStatus* status, Firebird::ITransaction* transaction,
		Firebird::FbMessage* in, Firebird::IMessageMetadata* out);
	virtual void FB_CARG setCursorName(Firebird::IStatus* status, const char* name);
	virtual void FB_CARG free(Firebird::IStatus* status);
	virtual unsigned FB_CARG getFlags(Firebird::IStatus* status);

public:
	Firebird::Mutex cursorMutex;
	YAttachment* attachment;
	YResultSet* openedCursor;
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

	YAttachment(Firebird::IProvider* aProvider, Firebird::IAttachment* aNext,
		const Firebird::PathName& aDbPath);
	~YAttachment();

	void destroy();
	void shutdown();
	FB_API_HANDLE& getHandle();

	// IAttachment implementation
	virtual void FB_CARG getInfo(Firebird::IStatus* status, unsigned int itemsLength,
		const unsigned char* items, unsigned int bufferLength, unsigned char* buffer);
	virtual YTransaction* FB_CARG startTransaction(Firebird::IStatus* status, unsigned int tpbLength,
		const unsigned char* tpb);
	virtual YTransaction* FB_CARG reconnectTransaction(Firebird::IStatus* status, unsigned int length,
		const unsigned char* id);
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
	virtual void FB_CARG executeDyn(Firebird::IStatus* status, Firebird::ITransaction* transaction, unsigned int length,
		const unsigned char* dyn);
	virtual YStatement* FB_CARG prepare(Firebird::IStatus* status, Firebird::ITransaction* tra,
		unsigned int stmtLength, const char* sqlStmt, unsigned int dialect, unsigned int flags);
	virtual Firebird::ITransaction* FB_CARG execute(Firebird::IStatus* status, Firebird::ITransaction* transaction,
		unsigned int stmtLength, const char* sqlStmt, unsigned int dialect, Firebird::FbMessage* in, Firebird::FbMessage* out);
	virtual Firebird::IResultSet* FB_CARG openCursor(Firebird::IStatus* status, Firebird::ITransaction* transaction,
		unsigned int stmtLength, const char* sqlStmt, unsigned int dialect, Firebird::FbMessage* in, Firebird::IMessageMetadata* out);
	virtual YEvents* FB_CARG queEvents(Firebird::IStatus* status, Firebird::IEventCallback* callback,
		unsigned int length, const unsigned char* eventsData);
	virtual void FB_CARG cancelOperation(Firebird::IStatus* status, int option);
	virtual void FB_CARG ping(Firebird::IStatus* status);
	virtual void FB_CARG detach(Firebird::IStatus* status);
	virtual void FB_CARG dropDatabase(Firebird::IStatus* status);

	void addCleanupHandler(Firebird::IStatus* status, CleanupCallback* callback);
	YTransaction* getTransaction(Firebird::IStatus* status, Firebird::ITransaction* tra);
	void getNextTransaction(Firebird::IStatus* status, Firebird::ITransaction* tra, NextTransaction& next);

public:
	Firebird::IProvider* provider;
	Firebird::PathName dbPath;
	HandleArray<YBlob> childBlobs;
	HandleArray<YEvents> childEvents;
	HandleArray<YRequest> childRequests;
	HandleArray<YStatement> childStatements;
	HandleArray<YTransaction> childTransactions;
	HandleArray<YResultSet> childCursors;
	Firebird::Array<CleanupCallback*> cleanupHandlers;
	Firebird::StatusHolder savedStatus;	// Do not use raise() method of this class in yValve.
};

class YService : public Firebird::StdPlugin<Firebird::IService, FB_SERVICE_VERSION>, public YObject, public EnterCount
{
public:
	static const ISC_STATUS ERROR_CODE = isc_bad_svc_handle;

	static const int SERV_START = 1;
	static const int SERV_QUERY = 2;
	static const int SERV_DETACH = 3;

	// Regular case
	YService(Firebird::IProvider* aProvider, Firebird::IService* aNext, bool utf8);
	// Used when next handle creation is delayed till service start
	YService(const char* svcName, unsigned int spbLength, const unsigned char* spb,
		Firebird::ICryptKeyCallback* callback, bool utf8);
	~YService();

	void destroy();
	FB_API_HANDLE& getHandle();

	// IService implementation
	virtual void FB_CARG detach(Firebird::IStatus* status);
	virtual void FB_CARG query(Firebird::IStatus* status,
		unsigned int sendLength, const unsigned char* sendItems,
		unsigned int receiveLength, const unsigned char* receiveItems,
		unsigned int bufferLength, unsigned char* buffer);
	virtual void FB_CARG start(Firebird::IStatus* status,
		unsigned int spbLength, const unsigned char* spb);

public:
	class ServiceType
	{
	public:
		Firebird::IProvider* provider;
		Firebird::RefPtr<Firebird::IService> next;

	public:
		ServiceType(Firebird::IService* n, Firebird::IProvider* p)
			: provider(p), next(n)
		{ }

		ServiceType()
			: provider(NULL)
		{ }

		~ServiceType();

		void shutdown();
		void detach(Firebird::IStatus* status);
	};

	ServiceType regular, started, queryCache;

	Firebird::PathName attachName;
	Firebird::AutoPtr<Firebird::ClumpletWriter> attachSpb;

	int FB_CARG release();

	typedef IService NextInterface;
	typedef YService YRef;

	void shutdown()
	{
		regular.shutdown();
		started.shutdown();
		queryCache.shutdown();
	}

	Firebird::IService* getNextService(int mode, Firebird::IStatus* status);

private:
	unsigned int checkSpbLen;
	const unsigned char* checkSpbPresent;
	Firebird::HalfStaticArray<UCHAR, 256> authBlock;
	Firebird::ICryptKeyCallback* cryptCallback;
	bool utf8Connection;		// Client talks to us using UTF8, else - system default charset

	void populateSpb(Firebird::ClumpletWriter& spb, UCHAR tag);
};

class Dispatcher : public Firebird::StdPlugin<Firebird::IProvider, FB_PROVIDER_VERSION>
{
public:
	Dispatcher()
		: cryptCallback(NULL)
	{ }

	// IProvider implementation
	virtual YAttachment* FB_CARG attachDatabase(Firebird::IStatus* status, const char* filename,
		unsigned int dpbLength, const unsigned char* dpb);
	virtual YAttachment* FB_CARG createDatabase(Firebird::IStatus* status, const char* filename,
		unsigned int dpbLength, const unsigned char* dpb);
	virtual YService* FB_CARG attachServiceManager(Firebird::IStatus* status, const char* serviceName,
		unsigned int spbLength, const unsigned char* spb);
	virtual void FB_CARG shutdown(Firebird::IStatus* status, unsigned int timeout, const int reason);
	virtual void FB_CARG setDbCryptCallback(Firebird::IStatus* status,
		Firebird::ICryptKeyCallback* cryptCallback);

	virtual int FB_CARG release()
	{
		if (--refCounter == 0)
		{
			delete this;
			return 0;
		}

		return 1;
	}

private:
	YAttachment* attachOrCreateDatabase(Firebird::IStatus* status, bool createFlag,
		const char* filename, unsigned int dpbLength, const unsigned char* dpb);

	Firebird::ICryptKeyCallback* cryptCallback;
};


}	// namespace Why

#endif	// YVALVE_Y_OBJECTS_H
