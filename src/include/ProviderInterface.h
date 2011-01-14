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

class EventCallback
{
public:
	virtual void FB_CARG eventCallbackFunction(unsigned int length, const UCHAR* events) = 0;
};

/*
class ShutdownCallback
{
public:
	virtual void shutdownCallbackFunction(int reason, int mask) = 0;
};

class MultipleTransaction
{
public:
	virtual unsigned int count() = 0;
	virtual Attachment* attachment(unsigned int n) = 0;
	virtual unsigned char* tpb(unsigned int n) = 0;
	virtual unsigned int tpbLength(unsigned int n) = 0;
};
*/

//class Sqlda;	// from Vulcan mainly

class IBlob : public Firebird::Interface
{
public:
	virtual void FB_CARG getInfo(Firebird::Status* status,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer) = 0;
	virtual unsigned int FB_CARG getSegment(Firebird::Status* status, unsigned int length, 
											unsigned char* buffer) = 0;	// returns real length
	virtual void FB_CARG putSegment(Firebird::Status* status, unsigned int length, 
									const unsigned char* buffer) = 0;
	virtual void FB_CARG cancel(Firebird::Status* status) = 0;
	virtual void FB_CARG close(Firebird::Status* status) = 0;
	virtual int FB_CARG seek(Firebird::Status* status, int mode, int offset) = 0;			// returns position
};
#define FB_I_BLOB_VERSION (FB_INTERFACE_VERSION + 6)

class ITransaction : public Firebird::Interface
{
public:
	virtual void FB_CARG getInfo(Firebird::Status* status,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer) = 0;
	virtual IBlob* FB_CARG createBlob(Firebird::Status* status, ISC_QUAD* id,
							 unsigned int bpbLength = 0, const unsigned char* bpb = 0,
							 IAttachment* att = 0) = 0;
	virtual IBlob* FB_CARG openBlob(Firebird::Status* status, ISC_QUAD* id,
						   unsigned int bpbLength = 0, const unsigned char* bpb = 0,
						   IAttachment* att = 0) = 0;
	virtual int FB_CARG getSlice(Firebird::Status* status, ISC_QUAD* id,
						 unsigned int sdl_length, const unsigned char* sdl,
						 unsigned int param_length, const unsigned char* param,
						 int sliceLength, unsigned char* slice,
						 IAttachment* att = 0) = 0;
	virtual void FB_CARG putSlice(Firebird::Status* status, ISC_QUAD* id,
						  unsigned int sdl_length, const unsigned char* sdl,
						  unsigned int param_length, const unsigned char* param,
						  int sliceLength, unsigned char* slice,
						  IAttachment* att = 0) = 0;
	virtual void FB_CARG transactRequest(Firebird::Status* status,
								 unsigned int blr_length, const unsigned char* blr,
								 unsigned int in_msg_length, const unsigned char* in_msg,
								 unsigned int out_msg_length, unsigned char* out_msg,
								 IAttachment* att = 0) = 0;
	virtual void FB_CARG prepare(Firebird::Status* status,
						 unsigned int msg_length = 0, const unsigned char* message = 0) = 0;
	virtual void FB_CARG ddl(Firebird::Status* status, unsigned int length, const unsigned char* ddlCommand) = 0;
	virtual void FB_CARG commit(Firebird::Status* status) = 0;
	virtual void FB_CARG commitRetaining(Firebird::Status* status) = 0;
	virtual void FB_CARG rollback(Firebird::Status* status) = 0;
	virtual void FB_CARG rollbackRetaining(Firebird::Status* status) = 0;
	// virtual void FB_CARG disconnect(Firebird::Status* status) = 0;		// Y-valve only object
};
#define FB_I_TRANSACTION_VERSION (FB_INTERFACE_VERSION + 12)

class IStatement : public Firebird::Interface
{
public:
	//virtual Sqlda* FB_CARG describeInput(Firebird::Status* status) = 0;
	//virtual Sqlda* FB_CARG describeOutput(Firebird::Status* status) = 0;

	// FixMe - prepare must return void, not new statement handle
	virtual IStatement* FB_CARG prepare(Firebird::Status* status, ITransaction* tra,
							   unsigned int stmtLength, const char* sqlStmt, unsigned int dialect,
							   unsigned int item_length, const unsigned char* items,
							   unsigned int buffer_length, unsigned char* buffer) = 0;

	virtual void FB_CARG getInfo(Firebird::Status* status,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer) = 0;
	virtual void FB_CARG setCursor(Firebird::Status* status, const char* name, unsigned int type) = 0;
//	virtual ITransaction* execute(Firebird::Status* status, ITransaction* tra, Sqlda* in, Sqlda* out) = 0;
	virtual ITransaction* FB_CARG executeMessage(Firebird::Status* status, ITransaction* tra,
										unsigned int in_blr_length, const unsigned char* in_blr,
										unsigned int in_msg_type,
										unsigned int in_msg_length, const unsigned char* in_message,
										unsigned int out_blr_length, const unsigned char* out_blr,
										unsigned int out_msg_type,
										unsigned int out_msg_length, unsigned char* out_message) = 0;
//	virtual int fetch(Firebird::Status* status, Sqlda* out) = 0;								// returns 100 if EOF, 101 if fragmented
	virtual int FB_CARG fetchMessage(Firebird::Status* status,
							 unsigned int blr_length, const unsigned char* blr,
							 unsigned int msg_type,
							 unsigned int msg_length, unsigned char* message) = 0;	// returns 100 if EOF, 101 if fragmented
//	virtual void insert(Firebird::Status* status, Sqlda* in) = 0;
	virtual void FB_CARG insertMessage(Firebird::Status* status,
							   unsigned int blr_length, const unsigned char* blr,
							   unsigned int msg_type,
							   unsigned int msg_length, const unsigned char* message) = 0;
	virtual void FB_CARG free(Firebird::Status* status, unsigned int option) = 0;
};
#define FB_I_STATEMENT_VERSION (FB_INTERFACE_VERSION + 7)

class IRequest : public Firebird::Interface
{
public:
	virtual void FB_CARG receive(Firebird::Status* status, int level, unsigned int msg_type,
						 unsigned int length, unsigned char* message) = 0;
	virtual void FB_CARG send(Firebird::Status* status, int level, unsigned int msg_type,
					  unsigned int length, const unsigned char* message) = 0;
	virtual void FB_CARG getInfo(Firebird::Status* status, int level,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer) = 0;
	virtual void FB_CARG start(Firebird::Status* status, ITransaction* tra, int level) = 0;
	virtual void FB_CARG startAndSend(Firebird::Status* status, ITransaction* tra, int level, unsigned int msg_type,
							  unsigned int length, const unsigned char* message) = 0;
	virtual void FB_CARG unwind(Firebird::Status* status, int level) = 0;
	virtual void FB_CARG free(Firebird::Status* status) = 0;
};
#define FB_I_REQUEST_VERSION (FB_INTERFACE_VERSION + 7)

class IEvents : public Firebird::Interface
{
public:
	virtual void cancel(Firebird::Status* status) = 0;
};
#define FB_I_EVENTS_VERSION (FB_INTERFACE_VERSION + 1)

class IAttachment : public Firebird::Interface
{
public:
	virtual void FB_CARG getInfo(Firebird::Status* status,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer) = 0;
//	virtual ITransaction* FB_CARG startTransaction(Firebird::Status* status, unsigned int tpbLength, const unsigned char* tpb) = 0;
// second form is tmp - not to rewrite external engines right now
	virtual ITransaction* FB_CARG startTransaction(Firebird::Status* status, unsigned int tpbLength, const unsigned char* tpb,
										  FB_API_HANDLE api) = 0;
	virtual ITransaction* FB_CARG reconnectTransaction(Firebird::Status* status, unsigned int length, const unsigned char* id) = 0;
	virtual IStatement* FB_CARG allocateStatement(Firebird::Status* status) = 0;
	virtual IRequest* FB_CARG compileRequest(Firebird::Status* status, unsigned int blr_length, const unsigned char* blr) = 0;
	virtual ITransaction* FB_CARG execute(Firebird::Status* status, ITransaction* transaction,
								 unsigned int length, const char* string, unsigned int dialect,
								 unsigned int in_blr_length, const unsigned char* in_blr,
								 unsigned int in_msg_type, unsigned int in_msg_length, const unsigned char* in_msg,
								 unsigned int out_blr_length, unsigned char* out_blr,
								 unsigned int out_msg_type, unsigned int out_msg_length, unsigned char* out_msg) = 0;
	virtual IEvents* FB_CARG queEvents(Firebird::Status* status, EventCallback* callback,
						   unsigned int length, const unsigned char* events) = 0;
	virtual void FB_CARG cancelOperation(Firebird::Status* status, int option) = 0;
	virtual void FB_CARG ping(Firebird::Status* status) = 0;
	virtual void FB_CARG detach(Firebird::Status* status) = 0;
	virtual void FB_CARG drop(Firebird::Status* status) = 0;
};
#define FB_I_ATTACHMENT_VERSION (FB_INTERFACE_VERSION + 11)

class IService : public Firebird::Interface
{
public:
	virtual void FB_CARG detach(Firebird::Status* status) = 0;
	virtual void FB_CARG query(Firebird::Status* status,
					   unsigned int sendLength, const unsigned char* sendItems,
					   unsigned int receiveLength, const unsigned char* receiveItems,
					   unsigned int bufferLength, unsigned char* buffer) = 0;
	virtual void FB_CARG start(Firebird::Status* status,
					   unsigned int spbLength, const unsigned char* spb) = 0;
};
#define FB_I_SERVICE_VERSION (FB_INTERFACE_VERSION + 3)

class PProvider : public Firebird::Plugin
{
public:
	virtual void FB_CARG attachDatabase(Firebird::Status* status, IAttachment** ptr, FB_API_HANDLE api, const char* fileName,
								unsigned int dpbLength, const unsigned char* dpb) = 0;
	virtual void FB_CARG createDatabase(Firebird::Status* status, IAttachment** ptr, FB_API_HANDLE api, const char* fileName,
								unsigned int dpbLength, const unsigned char* dpb) = 0;
	virtual IService* FB_CARG attachServiceManager(Firebird::Status* status, const char* service,
										  unsigned int spbLength, const unsigned char* spb) = 0;
	//virtual ITransaction* startTransaction(Firebird::Status* status, unsigned int count, ...) = 0;
	//virtual ITransaction* startMultiple(Firebird::Status* status, MultipleTransaction* multi) = 0;
	virtual void FB_CARG shutdown(Firebird::Status* status, unsigned int timeout, const int reason) = 0;
	//virtual void fb_shutdown_callback(Firebird::Status* status, const int mask, ShutdownCallback* callback) = 0;
};
#define FB_P_PROVIDER_VERSION (FB_PLUGIN_VERSION + 4)

} // namespace Firebird


#endif // FB_PROVIDER_INTERFACE
