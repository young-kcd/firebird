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

// This interfaces are implemented by yvalve code and by each of providers.

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

class IEventCallback
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

class IBlob : public IInterface
{
public:
	virtual void FB_CARG getInfo(IStatus* status,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer) = 0;
	virtual unsigned int FB_CARG getSegment(IStatus* status, unsigned int length,
											unsigned char* buffer) = 0;	// returns real length
	virtual void FB_CARG putSegment(IStatus* status, unsigned int length,
									const unsigned char* buffer) = 0;
	virtual void FB_CARG cancel(IStatus* status) = 0;
	virtual void FB_CARG close(IStatus* status) = 0;
	virtual int FB_CARG seek(IStatus* status, int mode, int offset) = 0;			// returns position
};
#define FB_I_BLOB_VERSION (FB_INTERFACE_VERSION + 6)

class ITransaction : public IInterface
{
public:
	virtual void FB_CARG getInfo(IStatus* status,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer) = 0;
	virtual void FB_CARG prepare(IStatus* status,
						 unsigned int msgLength = 0, const unsigned char* message = 0) = 0;
	virtual void FB_CARG commit(IStatus* status) = 0;
	virtual void FB_CARG commitRetaining(IStatus* status) = 0;
	virtual void FB_CARG rollback(IStatus* status) = 0;
	virtual void FB_CARG rollbackRetaining(IStatus* status) = 0;
	virtual void FB_CARG disconnect(IStatus* status) = 0;
};
#define FB_I_TRANSACTION_VERSION (FB_INTERFACE_VERSION + 12)

class IStatement : public IInterface
{
public:
	// FixMe - prepare must return void, not new statement handle
	virtual IStatement* FB_CARG prepare(IStatus* status, ITransaction* tra,
							   unsigned int stmtLength, const char* sqlStmt, unsigned int dialect,
							   unsigned int itemLength, const unsigned char* items,
							   unsigned int bufferLength, unsigned char* buffer) = 0;

	virtual void FB_CARG getInfo(IStatus* status,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer) = 0;
	virtual void FB_CARG setCursorName(IStatus* status, const char* name, unsigned int type) = 0;
	virtual ITransaction* FB_CARG execute(IStatus* status, ITransaction* tra,
										unsigned int inMsgType, const MessageBuffer* inMsgBuffer,
										const MessageBuffer* outMsgBuffer) = 0;
	virtual int FB_CARG fetch(IStatus* status, const MessageBuffer* msgBuffer) = 0;	// returns 100 if EOF, 101 if fragmented
	virtual void FB_CARG insert(IStatus* status, const MessageBuffer* msgBuffer) = 0;
	virtual void FB_CARG free(IStatus* status, unsigned int option) = 0;
};
#define FB_I_STATEMENT_VERSION (FB_INTERFACE_VERSION + 7)

class IRequest : public IInterface
{
public:
	virtual void FB_CARG receive(IStatus* status, int level, unsigned int msgType,
						 unsigned int length, unsigned char* message) = 0;
	virtual void FB_CARG send(IStatus* status, int level, unsigned int msgType,
					  unsigned int length, const unsigned char* message) = 0;
	virtual void FB_CARG getInfo(IStatus* status, int level,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer) = 0;
	virtual void FB_CARG start(IStatus* status, ITransaction* tra, int level) = 0;
	virtual void FB_CARG startAndSend(IStatus* status, ITransaction* tra, int level, unsigned int msgType,
							  unsigned int length, const unsigned char* message) = 0;
	virtual void FB_CARG unwind(IStatus* status, int level) = 0;
	virtual void FB_CARG free(IStatus* status) = 0;
};
#define FB_I_REQUEST_VERSION (FB_INTERFACE_VERSION + 7)

class IEvents : public IInterface
{
public:
	virtual void FB_CARG cancel(IStatus* status) = 0;
};
#define FB_I_EVENTS_VERSION (FB_INTERFACE_VERSION + 1)

class IAttachment : public IInterface
{
public:
	virtual void FB_CARG getInfo(IStatus* status,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer) = 0;
//	virtual ITransaction* FB_CARG startTransaction(IStatus* status, unsigned int tpbLength, const unsigned char* tpb) = 0;
// second form is tmp - not to rewrite external engines right now
	virtual ITransaction* FB_CARG startTransaction(IStatus* status, unsigned int tpbLength, const unsigned char* tpb,
										  FB_API_HANDLE api) = 0;
	virtual ITransaction* FB_CARG reconnectTransaction(IStatus* status, unsigned int length, const unsigned char* id) = 0;
	virtual IStatement* FB_CARG allocateStatement(IStatus* status) = 0;
	virtual IRequest* FB_CARG compileRequest(IStatus* status, unsigned int blrLength, const unsigned char* blr) = 0;
	virtual void FB_CARG transactRequest(IStatus* status, ITransaction* transaction,
								 unsigned int blrLength, const unsigned char* blr,
								 unsigned int inMsgLength, const unsigned char* inMsg,
								 unsigned int outMsgLength, unsigned char* outMsg) = 0;
	virtual IBlob* FB_CARG createBlob(IStatus* status, ITransaction* transaction, ISC_QUAD* id,
							 unsigned int bpbLength = 0, const unsigned char* bpb = 0) = 0;
	virtual IBlob* FB_CARG openBlob(IStatus* status, ITransaction* transaction, ISC_QUAD* id,
						   unsigned int bpbLength = 0, const unsigned char* bpb = 0) = 0;
	virtual int FB_CARG getSlice(IStatus* status, ITransaction* transaction, ISC_QUAD* id,
						 unsigned int sdlLength, const unsigned char* sdl,
						 unsigned int paramLength, const unsigned char* param,
						 int sliceLength, unsigned char* slice) = 0;
	virtual void FB_CARG putSlice(IStatus* status, ITransaction* transaction, ISC_QUAD* id,
						  unsigned int sdlLength, const unsigned char* sdl,
						  unsigned int paramLength, const unsigned char* param,
						  int sliceLength, unsigned char* slice) = 0;
	virtual void FB_CARG ddl(IStatus* status, ITransaction* transaction, unsigned int length,
		const unsigned char* dyn) = 0;
	virtual ITransaction* FB_CARG execute(IStatus* status, ITransaction* transaction,
								 unsigned int length, const char* string, unsigned int dialect,
								 unsigned int inMsgType, const MessageBuffer* inMsgBuffer,
								 const MessageBuffer* outMsgBuffer) = 0;
	virtual IEvents* FB_CARG queEvents(IStatus* status, IEventCallback* callback,
						   unsigned int length, const unsigned char* events) = 0;
	virtual void FB_CARG cancelOperation(IStatus* status, int option) = 0;
	virtual void FB_CARG ping(IStatus* status) = 0;
	virtual void FB_CARG detach(IStatus* status) = 0;
	virtual void FB_CARG drop(IStatus* status) = 0;
};
#define FB_I_ATTACHMENT_VERSION (FB_INTERFACE_VERSION + 11)

class IService : public IInterface
{
public:
	virtual void FB_CARG detach(IStatus* status) = 0;
	virtual void FB_CARG query(IStatus* status,
					   unsigned int sendLength, const unsigned char* sendItems,
					   unsigned int receiveLength, const unsigned char* receiveItems,
					   unsigned int bufferLength, unsigned char* buffer) = 0;
	virtual void FB_CARG start(IStatus* status,
					   unsigned int spbLength, const unsigned char* spb) = 0;
};
#define FB_I_SERVICE_VERSION (FB_INTERFACE_VERSION + 3)

class IProvider : public IPluginBase
{
public:
	virtual void FB_CARG attachDatabase(IStatus* status, IAttachment** ptr, FB_API_HANDLE api, const char* fileName,
								unsigned int dpbLength, const unsigned char* dpb) = 0;
	virtual void FB_CARG createDatabase(IStatus* status, IAttachment** ptr, FB_API_HANDLE api, const char* fileName,
								unsigned int dpbLength, const unsigned char* dpb) = 0;
	virtual IService* FB_CARG attachServiceManager(IStatus* status, const char* service,
										  unsigned int spbLength, const unsigned char* spb) = 0;
	//virtual ITransaction* FB_CARG startTransaction(IStatus* status, unsigned int count, ...) = 0;
	//virtual ITransaction* FB_CARG startMultiple(IStatus* status, MultipleTransaction* multi) = 0;
	virtual void FB_CARG shutdown(IStatus* status, unsigned int timeout, const int reason) = 0;
	//virtual void FB_CARG fb_shutdown_callback(IStatus* status, const int mask, ShutdownCallback* callback) = 0;
};
#define FB_P_PROVIDER_VERSION (FB_PLUGIN_VERSION + 4)

} // namespace Firebird


#endif // FB_PROVIDER_INTERFACE
