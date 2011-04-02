/*
 *	PROGRAM:		Firebird basic API
 *	MODULE:			YValveInterface.h
 *	DESCRIPTION:	Interfaces, used by yValve
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
 *  Copyright (c) 2010 Alex Peshkov <peshkoff at mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#ifndef FB_PROVIDER_INTERFACE
#define FB_PROVIDER_INTERFACE

#include "firebird.h"
#include "FirebirdPluginApi.h"

namespace Firebird {

class IAttachment;	// Forward

class Message
{
public:
	Message(unsigned aBlrLength, const unsigned char* aBlr, unsigned aBufferLength)
		: blrLength(aBlrLength),
		  blr(aBlr),
		  bufferLength(aBufferLength)
	{
	}

public:
	unsigned blrLength;
	const unsigned char* blr;
	unsigned bufferLength;
};

class MessageBuffer
{
public:
	MessageBuffer(const Message* aMessage, unsigned char* aBuffer)
		: message(aMessage),
		  buffer(aBuffer)
	{
	}

public:
	const Message* const message;
	unsigned char* const buffer;
};

class CleanupCallback
{
public:
	virtual void FB_CARG cleanupCallbackFunction() = 0;
};

class EventCallback
{
public:
	virtual void FB_CARG eventCallbackFunction(unsigned int length, const UCHAR* events) = 0;
};

/*
class ShutdownCallback
{
public:
	virtual void FB_CARG shutdownCallbackFunction(int reason, int mask) = 0;
};

class MultipleTransaction
{
public:
	virtual unsigned int FB_CARG count() = 0;
	virtual Attachment* FB_CARG attachment(unsigned int n) = 0;
	virtual unsigned char* FB_CARG tpb(unsigned int n) = 0;
	virtual unsigned int FB_CARG tpbLength(unsigned int n) = 0;
};
*/

class IBlob : public Interface
{
public:
	virtual void FB_CARG getInfo(Status* status,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer) = 0;
	virtual unsigned int FB_CARG getSegment(Status* status, unsigned int length,
											unsigned char* buffer) = 0;	// returns real length
	virtual void FB_CARG putSegment(Status* status, unsigned int length,
									const unsigned char* buffer) = 0;
	virtual void FB_CARG cancel(Status* status) = 0;
	virtual void FB_CARG close(Status* status) = 0;
	virtual int FB_CARG seek(Status* status, int mode, int offset) = 0;			// returns position
};
#define FB_I_BLOB_VERSION (FB_INTERFACE_VERSION + 6)

class ITransaction : public Interface
{
public:
	virtual void FB_CARG getInfo(Status* status,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer) = 0;
	virtual void FB_CARG prepare(Status* status,
						 unsigned int msgLength = 0, const unsigned char* message = 0) = 0;
	virtual void FB_CARG commit(Status* status) = 0;
	virtual void FB_CARG commitRetaining(Status* status) = 0;
	virtual void FB_CARG rollback(Status* status) = 0;
	virtual void FB_CARG rollbackRetaining(Status* status) = 0;
	virtual void FB_CARG disconnect(Status* status) = 0;
};
#define FB_I_TRANSACTION_VERSION (FB_INTERFACE_VERSION + 12)

class IStatement : public Interface
{
public:
	// FixMe - prepare must return void, not new statement handle
	virtual IStatement* FB_CARG prepare(Status* status, ITransaction* tra,
							   unsigned int stmtLength, const char* sqlStmt, unsigned int dialect,
							   unsigned int itemLength, const unsigned char* items,
							   unsigned int bufferLength, unsigned char* buffer) = 0;

	virtual void FB_CARG getInfo(Status* status,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer) = 0;
	virtual void FB_CARG setCursor(Status* status, const char* name, unsigned int type) = 0;
	virtual ITransaction* FB_CARG execute(Status* status, ITransaction* tra,
										unsigned int inMsgType, const MessageBuffer* inMsgBuffer,
										const MessageBuffer* outMsgBuffer) = 0;
	virtual int FB_CARG fetch(Status* status, const MessageBuffer* msgBuffer) = 0;	// returns 100 if EOF, 101 if fragmented
	virtual void FB_CARG insert(Status* status, const MessageBuffer* msgBuffer) = 0;
	virtual void FB_CARG free(Status* status, unsigned int option) = 0;
};
#define FB_I_STATEMENT_VERSION (FB_INTERFACE_VERSION + 7)

class IRequest : public Interface
{
public:
	virtual void FB_CARG receive(Status* status, int level, unsigned int msgType,
						 unsigned int length, unsigned char* message) = 0;
	virtual void FB_CARG send(Status* status, int level, unsigned int msgType,
					  unsigned int length, const unsigned char* message) = 0;
	virtual void FB_CARG getInfo(Status* status, int level,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer) = 0;
	virtual void FB_CARG start(Status* status, ITransaction* tra, int level) = 0;
	virtual void FB_CARG startAndSend(Status* status, ITransaction* tra, int level, unsigned int msgType,
							  unsigned int length, const unsigned char* message) = 0;
	virtual void FB_CARG unwind(Status* status, int level) = 0;
	virtual void FB_CARG free(Status* status) = 0;
};
#define FB_I_REQUEST_VERSION (FB_INTERFACE_VERSION + 7)

class IEvents : public Interface
{
public:
	virtual void FB_CARG cancel(Status* status) = 0;
};
#define FB_I_EVENTS_VERSION (FB_INTERFACE_VERSION + 1)

class IAttachment : public Interface
{
public:
	virtual void FB_CARG getInfo(Status* status,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer) = 0;
//	virtual ITransaction* FB_CARG startTransaction(Status* status, unsigned int tpbLength, const unsigned char* tpb) = 0;
// second form is tmp - not to rewrite external engines right now
	virtual ITransaction* FB_CARG startTransaction(Status* status, unsigned int tpbLength, const unsigned char* tpb,
										  FB_API_HANDLE api) = 0;
	virtual ITransaction* FB_CARG reconnectTransaction(Status* status, unsigned int length, const unsigned char* id) = 0;
	virtual IStatement* FB_CARG allocateStatement(Status* status) = 0;
	virtual IRequest* FB_CARG compileRequest(Status* status, unsigned int blrLength, const unsigned char* blr) = 0;
	virtual void FB_CARG transactRequest(Status* status, ITransaction* transaction,
								 unsigned int blrLength, const unsigned char* blr,
								 unsigned int inMsgLength, const unsigned char* inMsg,
								 unsigned int outMsgLength, unsigned char* outMsg) = 0;
	virtual IBlob* FB_CARG createBlob(Status* status, ITransaction* transaction, ISC_QUAD* id,
							 unsigned int bpbLength = 0, const unsigned char* bpb = 0) = 0;
	virtual IBlob* FB_CARG openBlob(Status* status, ITransaction* transaction, ISC_QUAD* id,
						   unsigned int bpbLength = 0, const unsigned char* bpb = 0) = 0;
	virtual int FB_CARG getSlice(Status* status, ITransaction* transaction, ISC_QUAD* id,
						 unsigned int sdlLength, const unsigned char* sdl,
						 unsigned int paramLength, const unsigned char* param,
						 int sliceLength, unsigned char* slice) = 0;
	virtual void FB_CARG putSlice(Status* status, ITransaction* transaction, ISC_QUAD* id,
						  unsigned int sdlLength, const unsigned char* sdl,
						  unsigned int paramLength, const unsigned char* param,
						  int sliceLength, unsigned char* slice) = 0;
	virtual void FB_CARG ddl(Status* status, ITransaction* transaction, unsigned int length,
		const unsigned char* dyn) = 0;
	virtual ITransaction* FB_CARG execute(Status* status, ITransaction* transaction,
								 unsigned int length, const char* string, unsigned int dialect,
								 unsigned int inMsgType, const MessageBuffer* inMsgBuffer,
								 const MessageBuffer* outMsgBuffer) = 0;
	virtual IEvents* FB_CARG queEvents(Status* status, EventCallback* callback,
						   unsigned int length, const unsigned char* events) = 0;
	virtual void FB_CARG cancelOperation(Status* status, int option) = 0;
	virtual void FB_CARG ping(Status* status) = 0;
	virtual void FB_CARG detach(Status* status) = 0;
	virtual void FB_CARG drop(Status* status) = 0;
};
#define FB_I_ATTACHMENT_VERSION (FB_INTERFACE_VERSION + 11)

class IService : public Interface
{
public:
	virtual void FB_CARG detach(Status* status) = 0;
	virtual void FB_CARG query(Status* status,
					   unsigned int sendLength, const unsigned char* sendItems,
					   unsigned int receiveLength, const unsigned char* receiveItems,
					   unsigned int bufferLength, unsigned char* buffer) = 0;
	virtual void FB_CARG start(Status* status,
					   unsigned int spbLength, const unsigned char* spb) = 0;
};
#define FB_I_SERVICE_VERSION (FB_INTERFACE_VERSION + 3)

class PProvider : public Plugin
{
public:
	virtual void FB_CARG attachDatabase(Status* status, IAttachment** ptr, FB_API_HANDLE api, const char* fileName,
								unsigned int dpbLength, const unsigned char* dpb) = 0;
	virtual void FB_CARG createDatabase(Status* status, IAttachment** ptr, FB_API_HANDLE api, const char* fileName,
								unsigned int dpbLength, const unsigned char* dpb) = 0;
	virtual IService* FB_CARG attachServiceManager(Status* status, const char* service,
										  unsigned int spbLength, const unsigned char* spb) = 0;
	//virtual ITransaction* FB_CARG startTransaction(Status* status, unsigned int count, ...) = 0;
	//virtual ITransaction* FB_CARG startMultiple(Status* status, MultipleTransaction* multi) = 0;
	virtual void FB_CARG shutdown(Status* status, unsigned int timeout, const int reason) = 0;
	//virtual void FB_CARG fb_shutdown_callback(Status* status, const int mask, ShutdownCallback* callback) = 0;
};
#define FB_P_PROVIDER_VERSION (FB_PLUGIN_VERSION + 4)

} // namespace Firebird


#endif // FB_PROVIDER_INTERFACE
