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
#include "firebird/Interface.h"
#include "gen/iberror.h"
#include "../common/StatusHolder.h"
#include "../common/classes/alloc.h"
#include "../common/classes/array.h"
#include "../common/MsgMetadata.h"

namespace Firebird
{
	class ClumpletWriter;
}

namespace Why
{


class YAttachment;
class YBlob;
class YRequest;
class YResultSet;
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
	virtual void cleanupCallbackFunction() = 0;
	virtual ~CleanupCallback() { }
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
		FB_SIZE_T pos;

		if (array.find(obj, pos))
			array.remove(pos);
	}

	void destroy(unsigned dstrFlags)
	{
		Firebird::MutexLockGuard guard(mtx, FB_FUNCTION);
		FB_SIZE_T i;

		while ((i = array.getCount()) > 0)
			array[i - 1]->destroy(dstrFlags);
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

template <typename Impl, typename Intf>
class YHelper : public Firebird::StdPlugin<Intf>, public YObject
{
public:
	typedef typename Intf::Declaration NextInterface;
	typedef YAttachment YRef;

	static const unsigned DF_RELEASE =		0x1;

	explicit YHelper(NextInterface* aNext);

	int release()
	{
		if (--this->refCounter == 0)
		{
			Impl* impl = static_cast<Impl*>(this);

			if (next)
			{
				impl->destroy(0);
			}

			delete impl; 	// call correct destructor
			return 0;
		}

		return 1;
	}

	void destroy2(unsigned dstrFlags)
	{
		RefDeb(Firebird::DEB_RLS_JATT, "YValve/destroy2");
		next = NULL;

		if (dstrFlags & DF_RELEASE)
		{
			RefDeb(Firebird::DEB_RLS_YATT, "destroy2");
			release();
		}
	}

	Firebird::RefPtr<NextInterface> next;
};

class YEvents FB_FINAL : public YHelper<YEvents, Firebird::Api::IEventsImpl<YEvents> >
{
public:
	static const ISC_STATUS ERROR_CODE = isc_bad_events_handle;

	YEvents(YAttachment* aAttachment, Firebird::IEvents* aNext, Firebird::IEventCallback* aCallback);

	void destroy(unsigned dstrFlags);
	FB_API_HANDLE& getHandle();

	// IEvents implementation
	void cancel(Firebird::IStatus* status);

public:
	YAttachment* attachment;
	Firebird::RefPtr<Firebird::IEventCallback> callback;
};

class YRequest FB_FINAL : public YHelper<YRequest, Firebird::Api::IRequestImpl<YRequest> >
{
public:
	static const ISC_STATUS ERROR_CODE = isc_bad_req_handle;

	YRequest(YAttachment* aAttachment, Firebird::IRequest* aNext);

	void destroy(unsigned dstrFlags);
	FB_API_HANDLE& getHandle();

	// IRequest implementation
	void receive(Firebird::IStatus* status, int level, unsigned int msgType,
		unsigned int length, unsigned char* message);
	void send(Firebird::IStatus* status, int level, unsigned int msgType,
		unsigned int length, const unsigned char* message);
	void getInfo(Firebird::IStatus* status, int level, unsigned int itemsLength,
		const unsigned char* items, unsigned int bufferLength, unsigned char* buffer);
	void start(Firebird::IStatus* status, Firebird::ITransaction* transaction, int level);
	void startAndSend(Firebird::IStatus* status, Firebird::ITransaction* transaction, int level,
		unsigned int msgType, unsigned int length, const unsigned char* message);
	void unwind(Firebird::IStatus* status, int level);
	void free(Firebird::IStatus* status);

public:
	YAttachment* attachment;
	FB_API_HANDLE* userHandle;
};

class YTransaction FB_FINAL : public YHelper<YTransaction, Firebird::Api::ITransactionImpl<YTransaction> >
{
public:
	static const ISC_STATUS ERROR_CODE = isc_bad_trans_handle;

	YTransaction(YAttachment* aAttachment, Firebird::ITransaction* aNext);

	void destroy(unsigned dstrFlags);
	FB_API_HANDLE& getHandle();

	// ITransaction implementation
	void getInfo(Firebird::IStatus* status, unsigned int itemsLength,
		const unsigned char* items, unsigned int bufferLength, unsigned char* buffer);
	void prepare(Firebird::IStatus* status, unsigned int msgLength,
		const unsigned char* message);
	void commit(Firebird::IStatus* status);
	void commitRetaining(Firebird::IStatus* status);
	void rollback(Firebird::IStatus* status);
	void rollbackRetaining(Firebird::IStatus* status);
	void disconnect(Firebird::IStatus* status);
	Firebird::ITransaction* join(Firebird::IStatus* status, Firebird::ITransaction* transaction);
	Firebird::ITransaction* validate(Firebird::IStatus* status, Firebird::IAttachment* testAtt);
	YTransaction* enterDtc(Firebird::IStatus* status);

	void addCleanupHandler(Firebird::IStatus* status, CleanupCallback* callback);
	void selfCheck();

public:
	YAttachment* attachment;
	HandleArray<YBlob> childBlobs;
	HandleArray<YResultSet> childCursors;
	Firebird::Array<CleanupCallback*> cleanupHandlers;

private:
	YTransaction(YTransaction* from)
		: YHelper(from->next),
		  attachment(from->attachment),
		  childBlobs(getPool()),
		  childCursors(getPool()),
		  cleanupHandlers(getPool())
	{
		childBlobs.assign(from->childBlobs);
		from->childBlobs.clear();
		childCursors.assign(from->childCursors);
		from->childCursors.clear();
		cleanupHandlers.assign(from->cleanupHandlers);
		from->cleanupHandlers.clear();
	}
};

typedef Firebird::RefPtr<Firebird::ITransaction> NextTransaction;

class YBlob FB_FINAL : public YHelper<YBlob, Firebird::Api::IBlobImpl<YBlob> >
{
public:
	static const ISC_STATUS ERROR_CODE = isc_bad_segstr_handle;

	YBlob(YAttachment* aAttachment, YTransaction* aTransaction, Firebird::IBlob* aNext);

	void destroy(unsigned dstrFlags);
	FB_API_HANDLE& getHandle();

	// IBlob implementation
	void getInfo(Firebird::IStatus* status, unsigned int itemsLength,
		const unsigned char* items, unsigned int bufferLength, unsigned char* buffer);
	int getSegment(Firebird::IStatus* status, unsigned int length, void* buffer,
								   unsigned int* segmentLength);
	void putSegment(Firebird::IStatus* status, unsigned int length, const void* buffer);
	void cancel(Firebird::IStatus* status);
	void close(Firebird::IStatus* status);
	int seek(Firebird::IStatus* status, int mode, int offset);

public:
	YAttachment* attachment;
	YTransaction* transaction;
};

class YResultSet FB_FINAL : public YHelper<YResultSet, Firebird::Api::IResultSetImpl<YResultSet> >
{
public:
	static const ISC_STATUS ERROR_CODE = isc_bad_result_set;

	YResultSet(YAttachment* anAttachment, YTransaction* aTransaction, Firebird::IResultSet* aNext);
	YResultSet(YAttachment* anAttachment, YTransaction* aTransaction, YStatement* aStatement,
		Firebird::IResultSet* aNext);

	void destroy(unsigned dstrFlags);

	// IResultSet implementation
	int fetchNext(Firebird::IStatus* status, void* message);
	int fetchPrior(Firebird::IStatus* status, void* message);
	int fetchFirst(Firebird::IStatus* status, void* message);
	int fetchLast(Firebird::IStatus* status, void* message);
	int fetchAbsolute(Firebird::IStatus* status, unsigned int position, void* message);
	int fetchRelative(Firebird::IStatus* status, int offset, void* message);
	FB_BOOLEAN isEof(Firebird::IStatus* status);
	FB_BOOLEAN isBof(Firebird::IStatus* status);
	Firebird::IMessageMetadata* getMetadata(Firebird::IStatus* status);
	void close(Firebird::IStatus* status);
	void setDelayedOutputFormat(Firebird::IStatus* status, Firebird::IMessageMetadata* format);

public:
	YAttachment* attachment;
	YTransaction* transaction;
	YStatement* statement;
};

class YMetadata
{
public:
	explicit YMetadata(bool in)
		: flag(false), input(in)
	{ }

	Firebird::IMessageMetadata* get(Firebird::IStatement* next, YStatement* statement);

private:
	Firebird::RefPtr<Firebird::MsgMetadata> metadata;
	volatile bool flag;
	bool input;
};

class YStatement FB_FINAL : public YHelper<YStatement, Firebird::Api::IStatementImpl<YStatement> >
{
public:
	static const ISC_STATUS ERROR_CODE = isc_bad_stmt_handle;

	YStatement(YAttachment* aAttachment, Firebird::IStatement* aNext);

	void destroy(unsigned dstrFlags);

	// IStatement implementation
	void getInfo(Firebird::IStatus* status,
		unsigned int itemsLength, const unsigned char* items,
		unsigned int bufferLength, unsigned char* buffer);
	unsigned getType(Firebird::IStatus* status);
	const char* getPlan(Firebird::IStatus* status, FB_BOOLEAN detailed);
	ISC_UINT64 getAffectedRecords(Firebird::IStatus* status);
	Firebird::IMessageMetadata* getInputMetadata(Firebird::IStatus* status);
	Firebird::IMessageMetadata* getOutputMetadata(Firebird::IStatus* status);
	Firebird::ITransaction* execute(Firebird::IStatus* status, Firebird::ITransaction* transaction,
		Firebird::IMessageMetadata* inMetadata, void* inBuffer,
		Firebird::IMessageMetadata* outMetadata, void* outBuffer);
	Firebird::IResultSet* openCursor(Firebird::IStatus* status, Firebird::ITransaction* transaction,
		Firebird::IMessageMetadata* inMetadata, void* inBuffer, Firebird::IMessageMetadata* outMetadata);
	void setCursorName(Firebird::IStatus* status, const char* name);
	void free(Firebird::IStatus* status);
	unsigned getFlags(Firebird::IStatus* status);

public:
	Firebird::Mutex statementMutex;
	YAttachment* attachment;
	YResultSet* cursor;

	Firebird::IMessageMetadata* getMetadata(bool in, Firebird::IStatement* next);

private:
	YMetadata input, output;
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

class YAttachment FB_FINAL : public YHelper<YAttachment, Firebird::Api::IAttachmentImpl<YAttachment> >,
	public EnterCount
{
public:
	static const ISC_STATUS ERROR_CODE = isc_bad_db_handle;

	YAttachment(Firebird::IProvider* aProvider, Firebird::IAttachment* aNext,
		const Firebird::PathName& aDbPath);
	~YAttachment();

	void destroy(unsigned dstrFlags);
	void shutdown();
	FB_API_HANDLE& getHandle();

	// IAttachment implementation
	void getInfo(Firebird::IStatus* status, unsigned int itemsLength,
		const unsigned char* items, unsigned int bufferLength, unsigned char* buffer);
	YTransaction* startTransaction(Firebird::IStatus* status, unsigned int tpbLength,
		const unsigned char* tpb);
	YTransaction* reconnectTransaction(Firebird::IStatus* status, unsigned int length,
		const unsigned char* id);
	YRequest* compileRequest(Firebird::IStatus* status, unsigned int blrLength,
		const unsigned char* blr);
	void transactRequest(Firebird::IStatus* status, Firebird::ITransaction* transaction,
		unsigned int blrLength, const unsigned char* blr, unsigned int inMsgLength,
		const unsigned char* inMsg, unsigned int outMsgLength, unsigned char* outMsg);
	YBlob* createBlob(Firebird::IStatus* status, Firebird::ITransaction* transaction, ISC_QUAD* id,
		unsigned int bpbLength, const unsigned char* bpb);
	YBlob* openBlob(Firebird::IStatus* status, Firebird::ITransaction* transaction, ISC_QUAD* id,
		unsigned int bpbLength, const unsigned char* bpb);
	int getSlice(Firebird::IStatus* status, Firebird::ITransaction* transaction, ISC_QUAD* id,
		unsigned int sdlLength, const unsigned char* sdl, unsigned int paramLength,
		const unsigned char* param, int sliceLength, unsigned char* slice);
	void putSlice(Firebird::IStatus* status, Firebird::ITransaction* transaction, ISC_QUAD* id,
		unsigned int sdlLength, const unsigned char* sdl, unsigned int paramLength,
		const unsigned char* param, int sliceLength, unsigned char* slice);
	void executeDyn(Firebird::IStatus* status, Firebird::ITransaction* transaction, unsigned int length,
		const unsigned char* dyn);
	YStatement* prepare(Firebird::IStatus* status, Firebird::ITransaction* tra,
		unsigned int stmtLength, const char* sqlStmt, unsigned int dialect, unsigned int flags);
	Firebird::ITransaction* execute(Firebird::IStatus* status, Firebird::ITransaction* transaction,
		unsigned int stmtLength, const char* sqlStmt, unsigned int dialect,
		Firebird::IMessageMetadata* inMetadata, void* inBuffer,
		Firebird::IMessageMetadata* outMetadata, void* outBuffer);
	Firebird::IResultSet* openCursor(Firebird::IStatus* status, Firebird::ITransaction* transaction,
		unsigned int stmtLength, const char* sqlStmt, unsigned int dialect,
		Firebird::IMessageMetadata* inMetadata, void* inBuffer, Firebird::IMessageMetadata* outMetadata,
		const char* cursorName);
	YEvents* queEvents(Firebird::IStatus* status, Firebird::IEventCallback* callback,
		unsigned int length, const unsigned char* eventsData);
	void cancelOperation(Firebird::IStatus* status, int option);
	void ping(Firebird::IStatus* status);
	void detach(Firebird::IStatus* status);
	void dropDatabase(Firebird::IStatus* status);

	void addCleanupHandler(Firebird::IStatus* status, CleanupCallback* callback);
	YTransaction* getTransaction(Firebird::IStatus* status, Firebird::ITransaction* tra);
	void getNextTransaction(Firebird::IStatus* status, Firebird::ITransaction* tra, NextTransaction& next);
	void execute(Firebird::IStatus* status, FB_API_HANDLE* traHandle,
		unsigned int stmtLength, const char* sqlStmt, unsigned int dialect,
		Firebird::IMessageMetadata* inMetadata, void* inBuffer,
		Firebird::IMessageMetadata* outMetadata, void* outBuffer);

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

class YService FB_FINAL : public YHelper<YService, Firebird::Api::IServiceImpl<YService> >,
	public EnterCount
{
public:
	static const ISC_STATUS ERROR_CODE = isc_bad_svc_handle;

	YService(Firebird::IProvider* aProvider, Firebird::IService* aNext, bool utf8);
	~YService();

	void shutdown();
	void destroy(unsigned dstrFlags);
	FB_API_HANDLE& getHandle();

	// IService implementation
	void detach(Firebird::IStatus* status);
	void query(Firebird::IStatus* status,
		unsigned int sendLength, const unsigned char* sendItems,
		unsigned int receiveLength, const unsigned char* receiveItems,
		unsigned int bufferLength, unsigned char* buffer);
	void start(Firebird::IStatus* status,
		unsigned int spbLength, const unsigned char* spb);

public:
	typedef Firebird::IService NextInterface;
	typedef YService YRef;

private:
	Firebird::IProvider* provider;
	bool utf8Connection;		// Client talks to us using UTF8, else - system default charset
};

class Dispatcher FB_FINAL : public Firebird::StdPlugin<Firebird::Api::IProviderImpl<Dispatcher> >
{
public:
	Dispatcher()
		: cryptCallback(NULL)
	{ }

	// IProvider implementation
	YAttachment* attachDatabase(Firebird::IStatus* status, const char* filename,
		unsigned int dpbLength, const unsigned char* dpb);
	YAttachment* createDatabase(Firebird::IStatus* status, const char* filename,
		unsigned int dpbLength, const unsigned char* dpb);
	YService* attachServiceManager(Firebird::IStatus* status, const char* serviceName,
		unsigned int spbLength, const unsigned char* spb);
	void shutdown(Firebird::IStatus* status, unsigned int timeout, const int reason);
	void setDbCryptCallback(Firebird::IStatus* status,
		Firebird::ICryptKeyCallback* cryptCallback);

	int release()
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

class UtlInterface FB_FINAL : public Firebird::AutoIface<Firebird::Api::IUtlImpl<UtlInterface> >
{
	// IUtl implementation
public:
	void getFbVersion(Firebird::IStatus* status, Firebird::IAttachment* att,
		Firebird::IVersionCallback* callback);
	void loadBlob(Firebird::IStatus* status, ISC_QUAD* blobId, Firebird::IAttachment* att,
		Firebird::ITransaction* tra, const char* file, FB_BOOLEAN txt);
	void dumpBlob(Firebird::IStatus* status, ISC_QUAD* blobId, Firebird::IAttachment* att,
		Firebird::ITransaction* tra, const char* file, FB_BOOLEAN txt);
	void getPerfCounters(Firebird::IStatus* status, Firebird::IAttachment* att,
		const char* countersSet, ISC_INT64* counters);			// in perf.cpp
	YAttachment* executeCreateDatabase(Firebird::IStatus* status,
		unsigned stmtLength, const char* creatDBstatement, unsigned dialect,
		FB_BOOLEAN* stmtIsCreateDb = NULL);
};

}	// namespace Why

#endif	// YVALVE_Y_OBJECTS_H
