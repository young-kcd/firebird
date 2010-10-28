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
#include "../common/classes/Interface.h"

namespace FbApi {	// Do not mix it with namespace Why

class Attachment;	// Forward

class EventCallback
{
public:
	virtual void eventCallbackFunction(unsigned int length, const UCHAR* events) = 0;
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

class Blob : public Firebird::Interface
{
public:
	virtual void getInfo(Firebird::Status* status,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer) = 0;
	virtual unsigned int getSegment(Firebird::Status* status, unsigned int length, unsigned char* buffer) = 0;	// returns real length
	virtual void putSegment(Firebird::Status* status, unsigned int length, const unsigned char* buffer) = 0;
	virtual void cancel(Firebird::Status* status) = 0;
	virtual void close(Firebird::Status* status) = 0;
	virtual int seek(Firebird::Status* status, int mode, int offset) = 0;			// returns position
};

class Transaction : public Firebird::Interface
{
public:
	virtual void getInfo(Firebird::Status* status,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer) = 0;
	virtual Blob* createBlob(Firebird::Status* status, ISC_QUAD* id,
							 unsigned int bpbLength = 0, const unsigned char* bpb = 0,
							 Attachment* att = 0) = 0;
	virtual Blob* openBlob(Firebird::Status* status, ISC_QUAD* id,
						   unsigned int bpbLength = 0, const unsigned char* bpb = 0,
						   Attachment* att = 0) = 0;
	virtual int getSlice(Firebird::Status* status, ISC_QUAD* id,
						 unsigned int sdl_length, const unsigned char* sdl,
						 unsigned int param_length, const unsigned char* param,
						 int sliceLength, unsigned char* slice,
						 Attachment* att = 0) = 0;
	virtual void putSlice(Firebird::Status* status, ISC_QUAD* id,
						  unsigned int sdl_length, const unsigned char* sdl,
						  unsigned int param_length, const unsigned char* param,
						  int sliceLength, unsigned char* slice,
						  Attachment* att = 0) = 0;
	virtual void transactRequest(Firebird::Status* status,
								 unsigned int blr_length, const unsigned char* blr,
								 unsigned int in_msg_length, const unsigned char* in_msg,
								 unsigned int out_msg_length, unsigned char* out_msg,
								 Attachment* att = 0) = 0;
	virtual void prepare(Firebird::Status* status,
						 unsigned int msg_length = 0, const unsigned char* message = 0) = 0;
	virtual void ddl(Firebird::Status* status, unsigned int length, const unsigned char* ddlCommand) = 0;
	virtual void commit(Firebird::Status* status) = 0;
	virtual void commitRetaining(Firebird::Status* status) = 0;
	virtual void rollback(Firebird::Status* status) = 0;
	virtual void rollbackRetaining(Firebird::Status* status) = 0;
	// virtual void disconnect(Firebird::Status* status) = 0;		// Y-valve only object
};

class Statement : public Firebird::Interface
{
public:
	//virtual Sqlda* describeInput(Firebird::Status* status) = 0;
	//virtual Sqlda* describeOutput(Firebird::Status* status) = 0;

	// FixMe - prepare must return void, not new statement handle
	virtual Statement* prepare(Firebird::Status* status, Transaction* tra,
							   unsigned int stmtLength, const char* sqlStmt, unsigned int dialect,
							   unsigned int item_length, const unsigned char* items,
							   unsigned int buffer_length, unsigned char* buffer) = 0;

	virtual void getInfo(Firebird::Status* status,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer) = 0;
	virtual void setCursor(Firebird::Status* status, const char* name, unsigned int type) = 0;
//	virtual Transaction* execute(Firebird::Status* status, Transaction* tra, Sqlda* in, Sqlda* out) = 0;
	virtual Transaction* executeMessage(Firebird::Status* status, Transaction* tra,
										unsigned int in_blr_length, const unsigned char* in_blr,
										unsigned int in_msg_type,
										unsigned int in_msg_length, const unsigned char* in_message,
										unsigned int out_blr_length, const unsigned char* out_blr,
										unsigned int out_msg_type,
										unsigned int out_msg_length, unsigned char* out_message) = 0;
//	virtual int fetch(Firebird::Status* status, Sqlda* out) = 0;								// returns 100 if EOF, 101 if fragmented
	virtual int fetchMessage(Firebird::Status* status,
							 unsigned int blr_length, const unsigned char* blr,
							 unsigned int msg_type,
							 unsigned int msg_length, unsigned char* message) = 0;	// returns 100 if EOF, 101 if fragmented
//	virtual void insert(Firebird::Status* status, Sqlda* in) = 0;
	virtual void insertMessage(Firebird::Status* status,
							   unsigned int blr_length, const unsigned char* blr,
							   unsigned int msg_type,
							   unsigned int msg_length, const unsigned char* message) = 0;
	virtual void free(Firebird::Status* status, unsigned int option) = 0;
};

class Request : public Firebird::Interface
{
public:
	virtual void receive(Firebird::Status* status, int level, unsigned int msg_type,
						 unsigned int length, unsigned char* message) = 0;
	virtual void send(Firebird::Status* status, int level, unsigned int msg_type,
					  unsigned int length, const unsigned char* message) = 0;
	virtual void getInfo(Firebird::Status* status, int level,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer) = 0;
	virtual void start(Firebird::Status* status, Transaction* tra, int level) = 0;
	virtual void startAndSend(Firebird::Status* status, Transaction* tra, int level, unsigned int msg_type,
							  unsigned int length, const unsigned char* message) = 0;
	virtual void unwind(Firebird::Status* status, int level) = 0;
	virtual void detach(Firebird::Status* status) = 0;
};

class Events : public Firebird::Interface
{
public:
	virtual void cancel(Firebird::Status* status) = 0;
};

class Attachment : public Firebird::Interface
{
public:
	virtual void getInfo(Firebird::Status* status,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer) = 0;
//	virtual Transaction* startTransaction(Firebird::Status* status, unsigned int tpbLength, const unsigned char* tpb) = 0;
// second form is tmp - not to rewrite external engines right now
	virtual Transaction* startTransaction(Firebird::Status* status, unsigned int tpbLength, const unsigned char* tpb,
										  FB_API_HANDLE api) = 0;
	virtual Transaction* reconnectTransaction(Firebird::Status* status, unsigned int length, const unsigned char* id) = 0;
	virtual Statement* allocateStatement(Firebird::Status* status) = 0;
	virtual Request* compileRequest(Firebird::Status* status, unsigned int blr_length, const unsigned char* blr) = 0;
	virtual Transaction* execute(Firebird::Status* status, Transaction* transaction,
								 unsigned int length, const char* string, unsigned int dialect,
								 unsigned int in_blr_length, const unsigned char* in_blr,
								 unsigned int in_msg_type, unsigned int in_msg_length, const unsigned char* in_msg,
								 unsigned int out_blr_length, unsigned char* out_blr,
								 unsigned int out_msg_type, unsigned int out_msg_length, unsigned char* out_msg) = 0;
	virtual Events* queEvents(Firebird::Status* status, EventCallback* callback,
						   unsigned int length, const unsigned char* events) = 0;
	virtual void cancelOperation(Firebird::Status* status, int option) = 0;
	virtual void ping(Firebird::Status* status) = 0;
	virtual void detach(Firebird::Status* status) = 0;
	virtual void drop(Firebird::Status* status) = 0;
};

class Service : public Firebird::Interface
{
public:
	virtual void detach(Firebird::Status* status) = 0;
	virtual void query(Firebird::Status* status,
					   unsigned int sendLength, const unsigned char* sendItems,
					   unsigned int receiveLength, const unsigned char* receiveItems,
					   unsigned int bufferLength, unsigned char* buffer) = 0;
	virtual void start(Firebird::Status* status,
					   unsigned int spbLength, const unsigned char* spb) = 0;
};

class Provider : public Firebird::Plugin
{
public:
	virtual void attachDatabase(Attachment** ptr, Firebird::Status* status, FB_API_HANDLE api, const char* fileName,
								unsigned int dpbLength, const unsigned char* dpb) = 0;
	virtual void createDatabase(Attachment** ptr, Firebird::Status* status, FB_API_HANDLE api, const char* fileName,
								unsigned int dpbLength, const unsigned char* dpb) = 0;
	virtual Service* attachServiceManager(Firebird::Status* status, const char* service,
										  unsigned int spbLength, const unsigned char* spb) = 0;
	//virtual Transaction* startTransaction(Firebird::Status* status, unsigned int count, ...) = 0;
	//virtual Transaction* startMultiple(Firebird::Status* status, MultipleTransaction* multi) = 0;
	virtual int shutdown(unsigned int timeout, const int reason) = 0;
	//virtual void fb_shutdown_callback(Firebird::Status* status, const int mask, ShutdownCallback* callback) = 0;
};

extern "C" Firebird::Status* ISC_EXPORT fb_get_status_instance();

} // namespace FbApi


#endif // FB_PROVIDER_INTERFACE
