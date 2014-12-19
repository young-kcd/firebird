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
 *  The Original Code was created by Alex Peshkov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2011 Alex Peshkov <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef JRD_ENGINE_INTERFACE_H
#define JRD_ENGINE_INTERFACE_H

#include "firebird/Interface.h"
#include "../common/classes/ImplementHelper.h"
#include "../common/StatementMetadata.h"
#include "../common/classes/RefCounted.h"

namespace Jrd {

// Engine objects used by interface objects
class blb;
class jrd_tra;
class dsql_req;
class JrdStatement;
class StableAttachmentPart;
class Attachment;
class Service;

// forward declarations
class JStatement;
class JAttachment;
class JProvider;

class JBlob FB_FINAL : public Firebird::RefCntIface<Firebird::Api::IBlobImpl<JBlob> >
{
public:
	// IBlob implementation
	int release();
	void getInfo(Firebird::IStatus* status,
		unsigned int itemsLength, const unsigned char* items,
		unsigned int bufferLength, unsigned char* buffer);
	int getSegment(Firebird::IStatus* status, unsigned int length, void* buffer,
		unsigned int* segmentLength);
	void putSegment(Firebird::IStatus* status, unsigned int length, const void* buffer);
	void cancel(Firebird::IStatus* status);
	void close(Firebird::IStatus* status);
	int seek(Firebird::IStatus* status, int mode, int offset);			// returns position

public:
	JBlob(blb* handle, StableAttachmentPart* sa);

	StableAttachmentPart* getAttachment()
	{
		return sAtt;
	}

	blb* getHandle() throw()
	{
		return blob;
	}

	void clearHandle()
	{
		blob = NULL;
	}

private:
	blb* blob;
	Firebird::RefPtr<StableAttachmentPart> sAtt;

	void freeEngineData(Firebird::IStatus* status);
};

class JTransaction FB_FINAL : public Firebird::RefCntIface<Firebird::Api::ITransactionImpl<JTransaction> >
{
public:
	// ITransaction implementation
	int release();
	void getInfo(Firebird::IStatus* status,
		unsigned int itemsLength, const unsigned char* items,
		unsigned int bufferLength, unsigned char* buffer);
	void prepare(Firebird::IStatus* status,
		unsigned int msg_length = 0, const unsigned char* message = 0);
	void commit(Firebird::IStatus* status);
	void commitRetaining(Firebird::IStatus* status);
	void rollback(Firebird::IStatus* status);
	void rollbackRetaining(Firebird::IStatus* status);
	void disconnect(Firebird::IStatus* status);
	Firebird::ITransaction* join(Firebird::IStatus* status, Firebird::ITransaction* transaction);
	JTransaction* validate(Firebird::IStatus* status, Firebird::IAttachment* testAtt);
	JTransaction* enterDtc(Firebird::IStatus* status);

public:
	JTransaction(jrd_tra* handle, StableAttachmentPart* sa);

	jrd_tra* getHandle() throw()
	{
		return transaction;
	}

	void setHandle(jrd_tra* handle)
	{
		transaction = handle;
	}

	StableAttachmentPart* getAttachment()
	{
		return sAtt;
	}

	void clear()
	{
		transaction = NULL;
		release();
	}

private:
	jrd_tra* transaction;
	Firebird::RefPtr<StableAttachmentPart> sAtt;

	JTransaction(JTransaction* from);

	void freeEngineData(Firebird::IStatus* status);
};

class JResultSet FB_FINAL : public Firebird::RefCntIface<Firebird::Api::IResultSetImpl<JResultSet> >
{
public:
	// IResultSet implementation
	int release();
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
	JResultSet(JStatement* aStatement);

	JStatement* getStatement()
	{
		return statement;
	}

	StableAttachmentPart* getAttachment();

	// Change after adding separate handle for cursor in dsql
	dsql_req* getHandle() throw();

private:
	Firebird::RefPtr<JStatement> statement;
	bool eof;

	void freeEngineData(Firebird::IStatus* status);
};

class JStatement FB_FINAL : public Firebird::RefCntIface<Firebird::Api::IStatementImpl<JStatement> >
{
public:
	// IStatement implementation
	int release();
	void getInfo(Firebird::IStatus* status,
		unsigned int itemsLength, const unsigned char* items,
		unsigned int bufferLength, unsigned char* buffer);
	void free(Firebird::IStatus* status);
	ISC_UINT64 getAffectedRecords(Firebird::IStatus* userStatus);
	Firebird::IMessageMetadata* getOutputMetadata(Firebird::IStatus* userStatus);
	Firebird::IMessageMetadata* getInputMetadata(Firebird::IStatus* userStatus);
	unsigned getType(Firebird::IStatus* status);
    const char* getPlan(Firebird::IStatus* status, FB_BOOLEAN detailed);
	Firebird::ITransaction* execute(Firebird::IStatus* status,
		Firebird::ITransaction* transaction, Firebird::IMessageMetadata* inMetadata, void* inBuffer,
		Firebird::IMessageMetadata* outMetadata, void* outBuffer);
	JResultSet* openCursor(Firebird::IStatus* status,
		Firebird::ITransaction* transaction, Firebird::IMessageMetadata* inMetadata, void* inBuffer,
		Firebird::IMessageMetadata* outMetadata);
	void setCursorName(Firebird::IStatus* status, const char* name);
	unsigned getFlags(Firebird::IStatus* status);

public:
	JStatement(dsql_req* handle, StableAttachmentPart* sa, Firebird::Array<UCHAR>& meta);

	StableAttachmentPart* getAttachment()
	{
		return sAtt;
	}

	dsql_req* getHandle() throw()
	{
		return statement;
	}

private:
	dsql_req* statement;
	Firebird::RefPtr<StableAttachmentPart> sAtt;
	Firebird::StatementMetadata metadata;

	void freeEngineData(Firebird::IStatus* status);
};

inline StableAttachmentPart* JResultSet::getAttachment()
{
	return statement->getAttachment();
}

// Change after adding separate handle for cursor in dsql
inline dsql_req* JResultSet::getHandle() throw()
{
	return statement->getHandle();
}

class JRequest FB_FINAL : public Firebird::RefCntIface<Firebird::Api::IRequestImpl<JRequest> >
{
public:
	// IRequest implementation
	int release();
	void receive(Firebird::IStatus* status, int level, unsigned int msg_type,
		unsigned int length, unsigned char* message);
	void send(Firebird::IStatus* status, int level, unsigned int msg_type,
		unsigned int length, const unsigned char* message);
	void getInfo(Firebird::IStatus* status, int level,
		unsigned int itemsLength, const unsigned char* items,
		unsigned int bufferLength, unsigned char* buffer);
	void start(Firebird::IStatus* status, Firebird::ITransaction* tra, int level);
	void startAndSend(Firebird::IStatus* status, Firebird::ITransaction* tra, int level,
		unsigned int msg_type, unsigned int length, const unsigned char* message);
	void unwind(Firebird::IStatus* status, int level);
	void free(Firebird::IStatus* status);

public:
	JRequest(JrdStatement* handle, StableAttachmentPart* sa);

	StableAttachmentPart* getAttachment()
	{
		return sAtt;
	}

	JrdStatement* getHandle() throw()
	{
		return rq;
	}

private:
	JrdStatement* rq;
	Firebird::RefPtr<StableAttachmentPart> sAtt;

	void freeEngineData(Firebird::IStatus* status);
};

class JEvents FB_FINAL : public Firebird::RefCntIface<Firebird::Api::IEventsImpl<JEvents> >
{
public:
	// IEvents implementation
	int release();
	void cancel(Firebird::IStatus* status);

public:
	JEvents(int aId, StableAttachmentPart* sa, Firebird::IEventCallback* aCallback);

	JEvents* getHandle() throw()
	{
		return this;
	}

	StableAttachmentPart* getAttachment()
	{
		return sAtt;
	}

private:
	int id;
	Firebird::RefPtr<StableAttachmentPart> sAtt;
	Firebird::RefPtr<Firebird::IEventCallback> callback;

	void freeEngineData(Firebird::IStatus* status);
};

class JAttachment FB_FINAL : public Firebird::RefCntIface<Firebird::Api::IAttachmentImpl<JAttachment> >
{
public:
	// IAttachment implementation
	int release();
	void addRef();

	void getInfo(Firebird::IStatus* status,
		unsigned int itemsLength, const unsigned char* items,
		unsigned int bufferLength, unsigned char* buffer);
	JTransaction* startTransaction(Firebird::IStatus* status,
		unsigned int tpbLength, const unsigned char* tpb);
	JTransaction* reconnectTransaction(Firebird::IStatus* status, unsigned int length, const unsigned char* id);
	JRequest* compileRequest(Firebird::IStatus* status, unsigned int blr_length, const unsigned char* blr);
	void transactRequest(Firebird::IStatus* status, Firebird::ITransaction* transaction,
		unsigned int blr_length, const unsigned char* blr,
		unsigned int in_msg_length, const unsigned char* in_msg,
		unsigned int out_msg_length, unsigned char* out_msg);
	JBlob* createBlob(Firebird::IStatus* status, Firebird::ITransaction* transaction,
		ISC_QUAD* id, unsigned int bpbLength = 0, const unsigned char* bpb = 0);
	JBlob* openBlob(Firebird::IStatus* status, Firebird::ITransaction* transaction,
		ISC_QUAD* id, unsigned int bpbLength = 0, const unsigned char* bpb = 0);
	int getSlice(Firebird::IStatus* status, Firebird::ITransaction* transaction, ISC_QUAD* id,
		unsigned int sdl_length, const unsigned char* sdl,
		unsigned int param_length, const unsigned char* param,
		int sliceLength, unsigned char* slice);
	void putSlice(Firebird::IStatus* status, Firebird::ITransaction* transaction, ISC_QUAD* id,
		unsigned int sdl_length, const unsigned char* sdl,
		unsigned int param_length, const unsigned char* param,
		int sliceLength, unsigned char* slice);
	void executeDyn(Firebird::IStatus* status, Firebird::ITransaction* transaction, unsigned int length,
		const unsigned char* dyn);
	JStatement* prepare(Firebird::IStatus* status, Firebird::ITransaction* tra,
		unsigned int stmtLength, const char* sqlStmt, unsigned int dialect, unsigned int flags);
	Firebird::ITransaction* execute(Firebird::IStatus* status,
		Firebird::ITransaction* transaction, unsigned int stmtLength, const char* sqlStmt,
		unsigned int dialect, Firebird::IMessageMetadata* inMetadata, void* inBuffer,
		Firebird::IMessageMetadata* outMetadata, void* outBuffer);
	Firebird::IResultSet* openCursor(Firebird::IStatus* status,
		Firebird::ITransaction* transaction, unsigned int stmtLength, const char* sqlStmt,
		unsigned int dialect, Firebird::IMessageMetadata* inMetadata, void* inBuffer,
		Firebird::IMessageMetadata* outMetadata, const char* cursorName);
	JEvents* queEvents(Firebird::IStatus* status, Firebird::IEventCallback* callback,
		unsigned int length, const unsigned char* events);
	void cancelOperation(Firebird::IStatus* status, int option);
	void ping(Firebird::IStatus* status);
	void detach(Firebird::IStatus* status);
	void dropDatabase(Firebird::IStatus* status);

public:
	explicit JAttachment(StableAttachmentPart* js);

	StableAttachmentPart* getStable() throw()
	{
		return att;
	}

	Jrd::Attachment* getHandle() throw();

	StableAttachmentPart* getAttachment() throw()
	{
		return att;
	}

	JTransaction* getTransactionInterface(Firebird::IStatus* status, Firebird::ITransaction* tra);
	jrd_tra* getEngineTransaction(Firebird::IStatus* status, Firebird::ITransaction* tra);

private:
	StableAttachmentPart* att;

	void freeEngineData(Firebird::IStatus* status);
};

class JService FB_FINAL : public Firebird::RefCntIface<Firebird::Api::IServiceImpl<JService> >
{
public:
	// IService implementation
	int release();
	void detach(Firebird::IStatus* status);
	void query(Firebird::IStatus* status,
		unsigned int sendLength, const unsigned char* sendItems,
		unsigned int receiveLength, const unsigned char* receiveItems,
		unsigned int bufferLength, unsigned char* buffer);
	void start(Firebird::IStatus* status,
		unsigned int spbLength, const unsigned char* spb);

public:
	explicit JService(Jrd::Service* handle);
	Firebird::Mutex mutex;
	Jrd::Service* svc;

private:
	void freeEngineData(Firebird::IStatus* status);
};

class JProvider FB_FINAL : public Firebird::StdPlugin<Firebird::Api::IProviderImpl<JProvider> >
{
public:
	explicit JProvider(Firebird::IPluginConfig* pConf)
		: cryptCallback(NULL), pluginConfig(pConf)
	{
	}

	static Firebird::RefPtr<JProvider> getInstance()
	{
		Firebird::RefPtr<JProvider> p(new JProvider(NULL));
		return p;
	}

	// IProvider implementation
	JAttachment* attachDatabase(Firebird::IStatus* status, const char* fileName,
		unsigned int dpbLength, const unsigned char* dpb);
	JAttachment* createDatabase(Firebird::IStatus* status, const char* fileName,
		unsigned int dpbLength, const unsigned char* dpb);
	JService* attachServiceManager(Firebird::IStatus* status, const char* service,
		unsigned int spbLength, const unsigned char* spb);
	void shutdown(Firebird::IStatus* status, unsigned int timeout, const int reason);
	void setDbCryptCallback(Firebird::IStatus* status,
		Firebird::ICryptKeyCallback* cryptCallback);

	int release();

private:
	Firebird::ICryptKeyCallback* cryptCallback;
	Firebird::IPluginConfig* pluginConfig;
};

} // namespace Jrd

#endif // JRD_ENGINE_INTERFACE_H
