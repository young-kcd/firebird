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
 *  The Original Code was created by Vlad Khorsun
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2007 Vlad Khorsun <hvlad@users.sourceforge.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef EXTDS_H
#define EXTDS_H

#include "../common/classes/fb_string.h"
#include "../common/classes/array.h"
#include "../common/classes/init.h"
#include "ibase.h"
#include "fb_api_proto.h"


namespace Jrd {

class jrd_nod;
class jrd_tra;
class thread_db;

class StringPtrComparator
{
public:
	static bool greaterThan(const Firebird::string* s1, const Firebird::string* s2) {
		return stricmp(s1->c_str(), s2->c_str()) > 0;
	}
};

class ExtProvManager;
class ExtProvider;
class ExtObject;
class ExtConnection;
class ExtTransaction;
class ExtStatement;

enum TraModes {traReadCommited, traReadCommitedRecVersions, traConcurrency, traConsistency};
enum TraScope {traAutonomous = 1, traCommon, traTwoPhase};

extern const char *NATIVE_PROVIDER_NAME;

typedef void (func_register_builtin_provider) (ExtProvManager&);

extern func_register_builtin_provider *register_builtin_provider;

// manages provider objects
class ExtProvManager
{
public:
	ExtProvManager(Firebird::MemoryPool &pool);
	~ExtProvManager();

	void addProvider(ExtProvider *provider);
	ExtProvider& getProvider(const Firebird::string &prvName);
	ExtConnection& getConnection(thread_db* tdbb, const Firebird::string &prvName, 
		const Firebird::string &dbName, const Firebird::string &user, 
		const Firebird::string &pwd, TraScope tra_scope);

	Firebird::MemoryPool &getPool() const
	{ return m_pool; }

	static Firebird::InitInstance<ExtProvManager> extProvManager;
private:

	typedef Firebird::SortedArray<
		ExtProvider*, 
		Firebird::EmptyStorage<ExtProvider*>, 
		const Firebird::string*,
		ExtProvider,
		StringPtrComparator > PrvArray;

	PrvArray m_providers;
	Firebird::MemoryPool &m_pool;
};


// manages connections\connection pool
class ExtProvider
{
public:
	ExtProvider(Firebird::MemoryPool &pool, const Firebird::string &prvName);
	virtual ~ExtProvider();

	virtual ExtConnection& getConnection(thread_db* tdbb, const Firebird::string &dbName, 
		const Firebird::string &user, const Firebird::string &pwd, TraScope tra_scope);

	virtual void releaseConnection(thread_db* tdbb, ExtConnection& conn, bool inPool = true);

	void attachmentReleased(thread_db* tdbb, Attachment* att);

	Firebird::MemoryPool &getPool() const
	{ return m_pool; }

	const Firebird::string& getName() const
	{ return m_name; }

	static const Firebird::string* generate(const void*, const ExtProvider* item) 
	{ return &item->m_name; }

	void initialize()
	{ 
		if (!m_api_loaded)
			loadAPI();
	}

	// provider properties
	int getFlags() const
	{ return m_flags; }

protected:
	virtual void loadAPI();
	ISC_STATUS notImplemented(ISC_STATUS*) const;
	virtual void releaseConnections(thread_db* tdbb);
	virtual ExtConnection* doCreateConnection();

	Firebird::MemoryPool &m_pool;
	const Firebird::string m_name;
	FirebirdApiPointer m_api;
	bool m_api_loaded;

	Firebird::Array<ExtConnection*> m_connections;

	int m_flags; 

public:
	virtual ISC_STATUS ISC_EXPORT isc_attach_database(ISC_STATUS *,
										  short,
										  char *,
										  isc_db_handle *,
										  short,
										  char *);

	virtual ISC_STATUS ISC_EXPORT isc_array_gen_sdl(ISC_STATUS *,
										ISC_ARRAY_DESC *,
										short *,
										char *,
										short *);

	virtual ISC_STATUS ISC_EXPORT isc_array_get_slice(ISC_STATUS *,
										  isc_db_handle *,
										  isc_tr_handle *,
										  ISC_QUAD *,
										  ISC_ARRAY_DESC *,
										  void *,
										  ISC_LONG *);

	virtual ISC_STATUS ISC_EXPORT isc_array_lookup_bounds(ISC_STATUS *,
											  isc_db_handle *,
											  isc_tr_handle *,
											  char *,
											  char *,
											  ISC_ARRAY_DESC *);

	virtual ISC_STATUS ISC_EXPORT isc_array_lookup_desc(ISC_STATUS *,
											isc_db_handle *,
											isc_tr_handle *,
											char *,
											char *,
											ISC_ARRAY_DESC *);

	virtual ISC_STATUS ISC_EXPORT isc_array_set_desc(ISC_STATUS *,
										 char *,
										 char *,
										 short *,
										 short *,
										 short *,
										 ISC_ARRAY_DESC *);

	virtual ISC_STATUS ISC_EXPORT isc_array_put_slice(ISC_STATUS *,
										  isc_db_handle *,
										  isc_tr_handle *,
										  ISC_QUAD *,
										  ISC_ARRAY_DESC *,
										  void *,
										  ISC_LONG *);

	virtual void ISC_EXPORT isc_blob_default_desc(ISC_BLOB_DESC *,
									  unsigned char *,
									  unsigned char *);

	virtual ISC_STATUS ISC_EXPORT isc_blob_gen_bpb(ISC_STATUS *,
									   ISC_BLOB_DESC *,
									   ISC_BLOB_DESC *,
									   unsigned short,
									   unsigned char *,
									   unsigned short *);

	virtual ISC_STATUS ISC_EXPORT isc_blob_info(ISC_STATUS *,
									isc_blob_handle *,
									short,
									char *,
									short,
									char *);

	virtual ISC_STATUS ISC_EXPORT isc_blob_lookup_desc(ISC_STATUS *,
										   isc_db_handle *,
										   isc_tr_handle *,
										   unsigned char *,
										   unsigned char *,
										   ISC_BLOB_DESC *,
										   unsigned char *);

	virtual ISC_STATUS ISC_EXPORT isc_blob_set_desc(ISC_STATUS *,
										unsigned char *,
										unsigned char *,
										short,
										short,
										short,
										ISC_BLOB_DESC *);

	virtual ISC_STATUS ISC_EXPORT isc_cancel_blob(ISC_STATUS *,
									  isc_blob_handle *);

	virtual ISC_STATUS ISC_EXPORT isc_cancel_events(ISC_STATUS *,
										isc_db_handle *,
										ISC_LONG *);

	virtual ISC_STATUS ISC_EXPORT isc_close_blob(ISC_STATUS *,
									 isc_blob_handle *);

	virtual ISC_STATUS ISC_EXPORT isc_commit_retaining(ISC_STATUS *,
										   isc_tr_handle *);

	virtual ISC_STATUS ISC_EXPORT isc_commit_transaction(ISC_STATUS *,
											 isc_tr_handle *);

	virtual ISC_STATUS ISC_EXPORT isc_create_blob(ISC_STATUS *,
									  isc_db_handle *,
									  isc_tr_handle *,
									  isc_blob_handle *,
									  ISC_QUAD *);

	virtual ISC_STATUS ISC_EXPORT isc_create_blob2(ISC_STATUS *,
									   isc_db_handle *,
									   isc_tr_handle *,
									   isc_blob_handle *,
									   ISC_QUAD *,
									   short,
									   char *);

	virtual ISC_STATUS ISC_EXPORT isc_create_database(ISC_STATUS *,
										  short,
										  char *,
										  isc_db_handle *,
										  short,
										  char *,
										  short);

	virtual ISC_STATUS ISC_EXPORT isc_database_info(ISC_STATUS *,
										isc_db_handle *,
										short,
										char *,
										short,
										char *);

	virtual void ISC_EXPORT isc_decode_date(ISC_QUAD *,
								void *);

	virtual void ISC_EXPORT isc_decode_sql_date(ISC_DATE *,
									void *);

	virtual void ISC_EXPORT isc_decode_sql_time(ISC_TIME *,
									void *);

	virtual void ISC_EXPORT isc_decode_timestamp(ISC_TIMESTAMP *,
									 void *);

	virtual ISC_STATUS ISC_EXPORT isc_detach_database(ISC_STATUS *,
										  isc_db_handle *);

	virtual ISC_STATUS ISC_EXPORT isc_drop_database(ISC_STATUS *,
										isc_db_handle *);

	virtual ISC_STATUS ISC_EXPORT isc_dsql_allocate_statement(ISC_STATUS *,
												  isc_db_handle *,
												  isc_stmt_handle *);

	virtual ISC_STATUS ISC_EXPORT isc_dsql_alloc_statement2(ISC_STATUS *,
												isc_db_handle *,
												isc_stmt_handle *);

	virtual ISC_STATUS ISC_EXPORT isc_dsql_describe(ISC_STATUS *,
										isc_stmt_handle *,
										unsigned short,
										XSQLDA *);

	virtual ISC_STATUS ISC_EXPORT isc_dsql_describe_bind(ISC_STATUS *,
											 isc_stmt_handle *,
											 unsigned short,
											 XSQLDA *);

	virtual ISC_STATUS ISC_EXPORT isc_dsql_exec_immed2(ISC_STATUS *,
										   isc_db_handle *,
										   isc_tr_handle *,
										   unsigned short,
										   char *,
										   unsigned short,
										   XSQLDA *,
										   XSQLDA *);

	virtual ISC_STATUS ISC_EXPORT isc_dsql_execute(ISC_STATUS *,
									   isc_tr_handle *,
									   isc_stmt_handle *,
									   unsigned short,
									   XSQLDA *);

	virtual ISC_STATUS ISC_EXPORT isc_dsql_execute2(ISC_STATUS *,
										isc_tr_handle *,
										isc_stmt_handle *,
										unsigned short,
										XSQLDA *,
										XSQLDA *);

	virtual ISC_STATUS ISC_EXPORT isc_dsql_execute_immediate(ISC_STATUS *,
												 isc_db_handle *,
												 isc_tr_handle *,
												 unsigned short,
												 char *,
												 unsigned short,
												 XSQLDA *);

	virtual ISC_STATUS ISC_EXPORT isc_dsql_fetch(ISC_STATUS *,
									 isc_stmt_handle *,
									 unsigned short,
									 XSQLDA *);

	virtual ISC_STATUS ISC_EXPORT isc_dsql_finish(isc_db_handle *);

	virtual ISC_STATUS ISC_EXPORT isc_dsql_free_statement(ISC_STATUS *,
											  isc_stmt_handle *,
											  unsigned short);

	virtual ISC_STATUS ISC_EXPORT isc_dsql_insert(ISC_STATUS *,
									  isc_stmt_handle *,
									  unsigned short,
									  XSQLDA *);

	virtual ISC_STATUS ISC_EXPORT isc_dsql_prepare(ISC_STATUS *,
									   isc_tr_handle *,
									   isc_stmt_handle *,
									   unsigned short,
									   char *,
									   unsigned short,
									   XSQLDA *);

	virtual ISC_STATUS ISC_EXPORT isc_dsql_set_cursor_name(ISC_STATUS *,
											   isc_stmt_handle *,
											   char *,
											   unsigned short);

	virtual ISC_STATUS ISC_EXPORT isc_dsql_sql_info(ISC_STATUS *,
										isc_stmt_handle *,
										short,
										const char *,
										short,
										char *);

	virtual void ISC_EXPORT isc_encode_date(void *,
								ISC_QUAD *);

	virtual void ISC_EXPORT isc_encode_sql_date(void *,
									ISC_DATE *);

	virtual void ISC_EXPORT isc_encode_sql_time(void *,
									ISC_TIME *);

	virtual void ISC_EXPORT isc_encode_timestamp(void *,
									 ISC_TIMESTAMP *);

	virtual ISC_LONG ISC_EXPORT_VARARG isc_event_block(char * *,
										   char * *,
										   unsigned short, ...);

	virtual void ISC_EXPORT isc_event_counts(ISC_ULONG *,
								 short,
								 char *,
								 char *);

/* 17 May 2001 - isc_expand_dpb is DEPRECATED */
	virtual void ISC_EXPORT_VARARG isc_expand_dpb(char * *,
									  short *, ...);

	virtual int ISC_EXPORT isc_modify_dpb(char * *,
							  short *,
							  unsigned short,
							  char *,
							  short);

	virtual ISC_LONG ISC_EXPORT isc_free(char *);

	virtual ISC_STATUS ISC_EXPORT isc_get_segment(ISC_STATUS *,
									  isc_blob_handle *,
									  unsigned short *,
									  unsigned short,
									  char *);

	virtual ISC_STATUS ISC_EXPORT isc_get_slice(ISC_STATUS *,
									isc_db_handle *,
									isc_tr_handle *,
									ISC_QUAD *,
									short,
									char *,
									short,
									ISC_LONG *,
									ISC_LONG,
									void *,
									ISC_LONG *);

	virtual ISC_STATUS ISC_EXPORT isc_interprete(char *,
									 ISC_STATUS * *);

	virtual ISC_STATUS ISC_EXPORT isc_open_blob(ISC_STATUS *,
									isc_db_handle *,
									isc_tr_handle *,
									isc_blob_handle *,
									ISC_QUAD *);

	virtual ISC_STATUS ISC_EXPORT isc_open_blob2(ISC_STATUS *,
									 isc_db_handle *,
									 isc_tr_handle *,
									 isc_blob_handle *,
									 ISC_QUAD *,
									 ISC_USHORT,
									 ISC_UCHAR *);

	virtual ISC_STATUS ISC_EXPORT isc_prepare_transaction2(ISC_STATUS *,
											   isc_tr_handle *,
											   ISC_USHORT,
											   ISC_UCHAR *);

	virtual void ISC_EXPORT isc_print_sqlerror(ISC_SHORT,
								   ISC_STATUS *);

	virtual ISC_STATUS ISC_EXPORT isc_print_status(ISC_STATUS *);

	virtual ISC_STATUS ISC_EXPORT isc_put_segment(ISC_STATUS *,
									  isc_blob_handle *,
									  unsigned short,
									  char *);

	virtual ISC_STATUS ISC_EXPORT isc_put_slice(ISC_STATUS *,
									isc_db_handle *,
									isc_tr_handle *,
									ISC_QUAD *,
									short,
									char *,
									short,
									ISC_LONG *,
									ISC_LONG,
									void *);

	virtual ISC_STATUS ISC_EXPORT isc_que_events(ISC_STATUS *,
									 isc_db_handle *,
									 ISC_LONG *,
									 ISC_USHORT,
									 ISC_UCHAR *,
									 isc_callback,
									 void *);

	virtual ISC_STATUS ISC_EXPORT isc_rollback_retaining(ISC_STATUS *,
											 isc_tr_handle *);

	virtual ISC_STATUS ISC_EXPORT isc_rollback_transaction(ISC_STATUS *,
											   isc_tr_handle *);

	virtual ISC_STATUS ISC_EXPORT isc_start_multiple(ISC_STATUS *,
										 isc_tr_handle *,
										 short,
										 void *);

	virtual ISC_STATUS ISC_EXPORT_VARARG isc_start_transaction(ISC_STATUS *,
												   isc_tr_handle *,
												   short, ...);

	virtual ISC_STATUS ISC_EXPORT_VARARG isc_reconnect_transaction(ISC_STATUS *,
                                                   isc_db_handle *,
                                                   isc_tr_handle *,
                                                   short, 
                                                   char*);

	virtual ISC_LONG ISC_EXPORT isc_sqlcode(ISC_STATUS *);

	virtual void ISC_EXPORT isc_sql_interprete(short,
								   char *,
								   short);

	virtual ISC_STATUS ISC_EXPORT isc_transaction_info(ISC_STATUS *,
										   isc_tr_handle *,
										   short,
										   char *,
										   short,
										   char *);

	virtual ISC_STATUS ISC_EXPORT isc_transact_request(ISC_STATUS *,
										   isc_db_handle *,
										   isc_tr_handle *,
										   unsigned short,
										   char *,
										   unsigned short,
										   char *,
										   unsigned short,
										   char *);

	virtual ISC_LONG ISC_EXPORT isc_vax_integer(char *,
									short);

	virtual ISC_INT64 ISC_EXPORT isc_portable_integer(unsigned char *,
										  short);

	virtual ISC_STATUS ISC_EXPORT isc_seek_blob(ISC_STATUS *,
									isc_blob_handle *,
									short,
									ISC_LONG,
									ISC_LONG *);



	virtual ISC_STATUS ISC_EXPORT isc_service_attach(ISC_STATUS *,
										 unsigned short,
										 char *,
										 isc_svc_handle *,
										 unsigned short,
										 char *);

	virtual ISC_STATUS ISC_EXPORT isc_service_detach(ISC_STATUS *,
										 isc_svc_handle *);

	virtual ISC_STATUS ISC_EXPORT isc_service_query(ISC_STATUS *,
										isc_svc_handle *,
										isc_resv_handle *,
										unsigned short,
										char *,
										unsigned short,
										char *,
										unsigned short,
										char *);

	virtual ISC_STATUS ISC_EXPORT isc_service_start(ISC_STATUS *,
										isc_svc_handle *,
										isc_resv_handle *,
										unsigned short,
										char *);
};

// provider flags
const int prvMultyStmts = 0x0001;	// supports many active statements per connection
const int prvMultyTrans = 0x0002;	// supports many active transactions per connection


// base class for external objects
class ExtObject
{
public:
	ExtObject(ExtProvider &prov) :
		m_provider(prov),
		m_pool(prov.getPool()),
		m_handle(0)	{};

	virtual ~ExtObject() {};

	ExtProvider& getProvider() const
	{ return m_provider; }

	FB_API_HANDLE getAPIHandle() const
	{ return m_handle; }

protected:
	ExtProvider	&m_provider;
	Firebird::MemoryPool &m_pool;
	FB_API_HANDLE m_handle;
};


class ExtConnection : public ExtObject
{
protected:
	friend class ExtProvider;
	friend class ExtTransaction;
	friend class ExtStatement;

	ExtConnection(ExtProvider &prov);
	virtual ~ExtConnection();

public:
	virtual void attach(thread_db* tdbb, const Firebird::string &dbName, 
		const Firebird::string &user, const Firebird::string &pwd);
	virtual void detach(thread_db* tdbb);

	ExtTransaction&	createTransaction();
	void deleteTransaction(ExtTransaction *tran);

	ExtStatement& createStatement(const Firebird::string &sql);
	void releaseStatement(ExtStatement *stmt);

	bool isSameDatabase(const Firebird::string &dbName, const Firebird::string &user, 
		const Firebird::string &pwd) const
	{
		return m_dbName == dbName && m_userName == user && m_password == pwd;
	}

	virtual bool isAvailable(thread_db* tdbb, TraScope traScope) const;

	ExtTransaction* findTransaction(thread_db* tdbb, 
		TraScope traScope) const;

	const Firebird::string& getDatasource() const
	{ return m_dbName; }

protected:
	virtual ExtTransaction*	doCreateTransaction();
	virtual ExtStatement*	doCreateStatement();
	void releaseStatements(thread_db* tdbb);

	Firebird::string m_dbName;
	Firebird::string m_userName;
	Firebird::string m_password;

	Firebird::Array<ExtTransaction*> m_transactions;
	Firebird::Array<ExtStatement*> m_statements;
	ExtStatement* m_FreeStatements;

	int	m_used_stmts;
	bool m_deleting;
};


class ExtTransaction : public ExtObject
{
protected:
	friend class ExtConnection;

	// create and delete only via parent ExtConnection
	ExtTransaction(ExtConnection &conn);
	virtual ~ExtTransaction();

public:
	ExtConnection& getConnection() const
	{ return m_connection; }

	TraScope getScope() const
	{ return m_scope; }

	void start(thread_db* tdbb, TraScope traScope, TraModes traMode, 
		bool readOnly, bool wait, int lockTimeout);
	void prepare(thread_db* tdbb, char* info);
	void commit(thread_db* tdbb, bool retain);
	void rollback(thread_db* tdbb, bool retain);

	static ExtTransaction& getTransaction(thread_db* tdbb, 
		ExtConnection &conn, TraScope tra_scope);

	static void endTransaction(thread_db* tdbb, jrd_tra* tran, 
		bool commit, bool retain, bool force);


protected:
	virtual void doStart(ISC_STATUS* status, thread_db* tdbb, TraScope traScope, TraModes traMode, 
		bool readOnly, bool wait, int lockTimeout);
	virtual void doPrepare(ISC_STATUS* status, thread_db* tdbb, char* info);
	virtual void doCommit(ISC_STATUS* status, thread_db* tdbb, bool retain);
	virtual void doRollback(ISC_STATUS* status, thread_db* tdbb, bool retain);

	ExtConnection	&m_connection; 
	TraScope		m_scope;
	ExtTransaction* m_nextTran;		// next common transaction 
};


class ExtStatement : public ExtObject
{
protected:
	friend class ExtConnection;

	// create and delete only via parent ExtConnection
	ExtStatement(ExtConnection &conn);
	virtual ~ExtStatement();

public:
	ExtConnection& getConnection() const
	{ return m_connection; }

	ExtTransaction* getTransaction() const
	{ return m_transaction; }

	virtual void prepare(thread_db* tdbb, ExtTransaction &tran, Firebird::string &sql);
	virtual void execute(thread_db* tdbb, ExtTransaction &tran, int in_count, 
		jrd_nod **in_params, int out_count, jrd_nod **out_params);
	virtual void open(thread_db* tdbb, ExtTransaction &tran, int in_count, 
		jrd_nod **in_params, bool singleton);
	virtual bool fetch(thread_db* tdbb, int out_count, jrd_nod **out_params);
	virtual void close(thread_db* tdbb);

	const Firebird::string& getSql() const
	{ return m_sql; }

	bool getSelectable() const
	{ return m_stmt_selectable; }

	bool getActive() const
	{ return m_active; }

	int getInputs() const
	{ return m_in_xsqlda ? m_in_xsqlda->sqld : 0; }

	int getOutputs() const
	{ return m_out_xsqlda ? m_out_xsqlda->sqld : 0; }

	void raise(ISC_STATUS* status, thread_db* tdbb, const char* sWhere, 
		const Firebird::string* sQuery = NULL);

protected:
	virtual void setInParams(thread_db* tdbb, int count, jrd_nod **params);
	virtual void getOutParams(thread_db* tdbb, int count, jrd_nod **params);

	ExtStatement* m_nextStmt;		// next free statement

	ExtConnection	&m_connection; 
	ExtTransaction	*m_transaction;
	Firebird::string m_sql;
	XSQLDA	*m_in_xsqlda;
	XSQLDA	*m_out_xsqlda;
	Firebird::Array<char> m_in_buffer;
	Firebird::Array<char> m_out_buffer;
	bool	m_stmt_selectable;
	bool	m_singleton;
	bool	m_error;
	bool	m_active;
};

class EngineCallbackGuard
{
public:
	EngineCallbackGuard(thread_db* tdbb);
	~EngineCallbackGuard();

private:
	thread_db* m_tdbb;
};

}; // namespace Jrd

#endif // EXTDS_H