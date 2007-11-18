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
 *  The Original Code was created by Vladyslav Khorsun for the 
 *  Firebird Open Source RDBMS project and based on execute_statement 
 *	module by Alexander Peshkoff.
 *
 *  Copyright (c) 2007 Vladyslav Khorsun <hvlad@users.sourceforge.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "fb_types.h"
#include "../jrd/common.h"
#include "../jrd/jrd_blks.h"
#include "../include/fb_blk.h"
#include "fb_exception.h"
#include "iberror.h"
#include "ibase.h"
#include "../common/classes/ClumpletWriter.h"
#include "../common/classes/GenericMap.h"

#include "../jrd/dsc.h"
#include "../jrd/exe.h"
#include "../jrd/ExtDS.h"
#include "../jrd/jrd.h"
#include "../jrd/tra.h"

#include "../jrd/err_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/exe_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/thread_proto.h"

using namespace Jrd;
using namespace Firebird;

static void getRemoteError(ISC_STATUS* status, string &err);
static size_t parseSQLDA(XSQLDA *xsqlda, char *buff);


const SSHORT sqlType[] =
{
/* dtype_unknown	*/ -1,
/* dtype_text		*/ SQL_TEXT,
/* dtype_cstring	*/ -1,
/* dtype_varying	*/ SQL_VARYING,
/* dtype_none1		*/ -1,
/* dtype_none2		*/ -1,
/* dtype_packed		*/ -1,
/* dtype_byte		*/ -1,
/* dtype_short		*/ SQL_SHORT,
/* dtype_long		*/ SQL_LONG,
/* dtype_quad		*/ SQL_QUAD,
/* dtype_real		*/ SQL_FLOAT,
/* dtype_double		*/ SQL_DOUBLE,
/* dtype_d_float	*/ -1,				// Fix to use in VMS
/* dtype_sql_date	*/ SQL_TYPE_DATE,
/* dtype_sql_time	*/ SQL_TYPE_TIME,
/* dtype_timestamp	*/ SQL_TIMESTAMP,
/* dtype_blob		*/ SQL_BLOB,
/* dtype_array		*/ SQL_ARRAY,		// Not supported for a while
/* dtype_int64		*/ SQL_INT64
};

static InitInstance<GenericMap<Pair<NonPooled<SSHORT, UCHAR> > > > sqlTypeToDscType;


// ExtProvManager
func_register_builtin_provider *Jrd::register_builtin_provider = NULL;

InitInstance<ExtProvManager> ExtProvManager::extProvManager;

ExtProvManager::ExtProvManager(Firebird::MemoryPool &pool) :
	m_pool(pool),
	m_providers(pool)
{
	if (register_builtin_provider)
		(*register_builtin_provider) (*this);
}

ExtProvManager::~ExtProvManager()
{
	for (ExtProvider** prv = m_providers.begin(); 
		 prv < m_providers.end(); prv++)
	{
		delete *prv;
		prv = NULL;
	}
	m_providers.clear();
}

void ExtProvManager::addProvider(ExtProvider* provider)
{
	size_t pos;
	if (m_providers.find(ExtProvider::generate(0, provider), pos))
		return;

	provider->initialize();
	m_providers.add(provider);
}

ExtProvider& ExtProvManager::getProvider(const string &prvName)
{
	size_t pos;
	if (!m_providers.find(&prvName, pos))
	{
		ExtProvider* prv = FB_NEW (m_pool) ExtProvider(m_pool, prvName);
		prv->initialize();
		pos = m_providers.add(prv);
	}
	return *m_providers[pos];
}

ExtConnection& ExtProvManager::getConnection(thread_db* tdbb, const string &prvName, 
	const string &dbName, const string &user, const string &pwd, TraScope tra_scope)
{
	ExtProvider& prv = getProvider(prvName);
	return prv.getConnection(tdbb, dbName, user, pwd, tra_scope);
}



// ExtProvider

ExtProvider::ExtProvider(Firebird::MemoryPool &pool, const Firebird::string &prvName) :
	m_pool(pool), 
	m_name(pool, prvName),
	m_connections(pool),
	m_flags(0)
{
	memset(&m_api, 0, sizeof(m_api) );
	m_api_loaded = false;
}

ExtProvider::~ExtProvider()
{
	thread_db *tdbb = JRD_get_thread_data();
	releaseConnections(tdbb);
}

ExtConnection& ExtProvider::getConnection(thread_db* tdbb, const string &dbName, 
	const string &user, const string &pwd, TraScope tra_scope)
{
	ExtConnection **conn_ptr = m_connections.begin();
	ExtConnection **end = m_connections.end();
	for (; conn_ptr < end; conn_ptr++)
	{
		ExtConnection *conn = *conn_ptr;
		if (conn->isSameDatabase(dbName, user, pwd) &&
			conn->isAvailable(tdbb, tra_scope))
		{
			return *conn;
		}
	}

	ExtConnection *conn = doCreateConnection();
	m_connections.add(conn);
	conn->attach(tdbb, dbName, user, pwd);
	return *conn;
}

ExtConnection* ExtProvider::doCreateConnection()
{
	return FB_NEW(m_pool) ExtConnection(*this);
}


void ExtProvider::releaseConnection(thread_db* tdbb, ExtConnection& conn, bool inPool)
{
	fb_assert(conn.m_used_stmts == 0);
	fb_assert(conn.m_transactions.getCount() == 0);

	conn.detach(tdbb);
	ExtConnection **ptr = m_connections.begin();
	ExtConnection **end = m_connections.end();
	for (; ptr < end; ptr++)
		if (*ptr == &conn)
		{
			m_connections.remove(ptr);
			delete &conn;
			return;
		}
}

void ExtProvider::releaseConnections(thread_db* tdbb)
{
	ExtConnection **ptr = m_connections.begin();
	ExtConnection **end = m_connections.end();
	for (; ptr < end; ptr++)
	{
		fb_assert((*ptr)->m_used_stmts == 0);
		fb_assert((*ptr)->m_transactions.getCount() == 0);

		delete *ptr;
		*ptr = NULL;
	}
	m_connections.clear();
}


void ExtProvider::attachmentReleased(thread_db* tdbb, Attachment* att)
{
	if (m_connections.getCount() == 0)
		return;

	ExtConnection **ptr = m_connections.end();
	ExtConnection **begin = m_connections.begin();
	for (ptr--; ptr >= begin; ptr--)
	{
		ExtConnection *conn = *ptr;
		if (conn->getAPIHandle() == att->att_public_handle)
			releaseConnection(tdbb, *conn, false);
	}
}

// provider properties


// ExtConnection 
ExtConnection::ExtConnection(ExtProvider &prov) :
	ExtObject(prov),
	m_transactions(m_pool),
	m_statements(m_pool),
	m_FreeStatements(NULL),
	m_dbName(m_pool),
	m_userName(m_pool),
	m_password(m_pool),
	m_used_stmts(0),
	m_deleting(false)
{
}

ExtConnection::~ExtConnection()
{
	if (m_handle)
	{
		m_deleting = true;
		thread_db* tdbb = JRD_get_thread_data();
		detach(tdbb);
	}
}

void ExtConnection::attach(thread_db* tdbb, const string &dbName, const string &user, 
	const string &pwd)
{
	ClumpletWriter dpb(ClumpletReader::Tagged, MAX_DPB_SIZE, isc_dpb_version1);
	dpb.insertString(isc_dpb_user_name, user);
	dpb.insertString(isc_dpb_password, pwd);

	ISC_STATUS_ARRAY status = {0};
	{
		EngineCallbackGuard guard(tdbb);
		m_provider.isc_attach_database(status, dbName.length(), (char*) dbName.c_str(), 
			&m_handle, dpb.getBufferLength(), (char*) dpb.getBuffer());
	}

	if (status[1]) {
		status_exception::raise(status);
	}

	m_dbName = dbName;
	m_userName = user;
	m_password = pwd;
}

void ExtConnection::detach(thread_db* tdbb)
{
	releaseStatements(tdbb);

	ISC_STATUS_ARRAY status = {0};
	if (m_handle)
	{
		EngineCallbackGuard guard(tdbb);

		FB_API_HANDLE h = m_handle;
		m_handle = 0;
		m_provider.isc_detach_database(status, &h);
		m_handle = h;
	}

	if (status[1]) {
		status_exception::raise(status);
	}

	m_dbName = "";
	m_userName = "";
	m_password = "";
}

ExtTransaction& ExtConnection::createTransaction()
{
	ExtTransaction* tran = doCreateTransaction();
	m_transactions.add(tran);
	return *tran;
}

void ExtConnection::deleteTransaction(ExtTransaction *tran)
{
	ExtTransaction **tran_ptr = m_transactions.begin();
	ExtTransaction **end = m_transactions.end();
	for (; tran_ptr < end; tran_ptr++)
	{
		if (tran == *tran_ptr)
		{
			m_transactions.remove(tran_ptr);
			delete tran;
			break;
		}
	}

	if (!m_used_stmts && !m_transactions.getCount() && !m_deleting)
		m_provider.releaseConnection(JRD_get_thread_data(), *this);
}

ExtStatement& ExtConnection::createStatement(const Firebird::string &sql)
{
	m_used_stmts++;

	ExtStatement **stmt_ptr = &m_FreeStatements;
	for (; *stmt_ptr; stmt_ptr = &(*stmt_ptr)->m_nextStmt)
	{
		ExtStatement *stmt = *stmt_ptr;
		if (stmt->getSql() == sql)
		{
			*stmt_ptr = stmt->m_nextStmt;
			stmt->m_nextStmt = NULL;
			return *stmt;
		}
	}
	
	if (m_FreeStatements)
	{
		ExtStatement *stmt = m_FreeStatements;
		m_FreeStatements = stmt->m_nextStmt;
		stmt->m_nextStmt = NULL;
		return *stmt;
	}

	ExtStatement *stmt = doCreateStatement();
	m_statements.add(stmt);
	return *stmt;
}

void ExtConnection::releaseStatement(ExtStatement *stmt)
{
	fb_assert(stmt && !stmt->getActive());
	stmt->m_nextStmt = m_FreeStatements;
	m_FreeStatements = stmt;
	m_used_stmts--;

	if (!m_used_stmts && !m_transactions.getCount() && !m_deleting)
		m_provider.releaseConnection(JRD_get_thread_data(), *this);
}

void ExtConnection::releaseStatements(thread_db* tdbb)
{
	ExtStatement **stmt_ptr = m_statements.begin();
	ExtStatement **end = m_statements.end();
	for (; stmt_ptr < end; stmt_ptr++)
	{
		ExtStatement *stmt = *stmt_ptr;
		if (stmt->getActive())
			stmt->close(tdbb);
		delete stmt;
	}
	m_statements.clear();
	
	fb_assert(!m_used_stmts);
	m_FreeStatements = NULL;
}

ExtTransaction*	ExtConnection::doCreateTransaction()
{
	return FB_NEW(m_pool) ExtTransaction(*this);
}

ExtStatement*	ExtConnection::doCreateStatement()
{
	return FB_NEW(m_pool) ExtStatement(*this);
}

// connection is available for the current execution context if it
// a) have no active statements or support many active statements 
//    and 
// b) have no active transactions or have active transaction of given 
//	  TraScope bound to current jrd transaction or support many active 
//    transactions
bool ExtConnection::isAvailable(thread_db* tdbb, TraScope traScope) const 
{
	const int flags = m_provider.getFlags();
	if (m_used_stmts && !(flags & prvMultyStmts))
		return false;

	if (m_transactions.getCount() && !(flags & prvMultyTrans) && 
		!findTransaction(tdbb, traScope))
		return false;

	return true;
}

ExtTransaction* ExtConnection::findTransaction(thread_db* tdbb, 
	TraScope traScope) const
{
	jrd_tra *tran = tdbb->tdbb_transaction;
	ExtTransaction *ext_tran = NULL;
	switch (traScope)
	{
	case traCommon :
		ext_tran = tran->tra_ext_common;
		while (ext_tran) 
		{
			if (&ext_tran->getConnection() == this)
				break;
			ext_tran = ext_tran->m_nextTran;
		}
		break;

	case traTwoPhase :
		ERR_post(isc_random, isc_arg_string, "2PC transactions not implemented", isc_arg_end);

		//ext_tran = tran->tra_ext_two_phase;
		// join transaction if not already joined
		break;
	}

	return ext_tran;
}

// ExtTransaction 

ExtTransaction::ExtTransaction(ExtConnection &conn) :
	ExtObject(conn.getProvider()),
	m_connection(conn),
	m_scope(traCommon),
	m_nextTran(0)
{
}

ExtTransaction::~ExtTransaction()
{
}

void ExtTransaction::start(thread_db* tdbb, TraScope traScope, TraModes traMode, 
	bool readOnly, bool wait, int lockTimeout)
{
	ISC_STATUS_ARRAY status = {0};
	doStart(status, tdbb, traScope, traMode, readOnly, wait, lockTimeout);

	if (status[1]) {
		status_exception::raise(status);
	}

	m_scope = traScope;

	jrd_tra *tran = tdbb->tdbb_transaction;
	switch (m_scope)
	{
	case traCommon :
		this->m_nextTran = tran->tra_ext_common;
		tran->tra_ext_common = this;
		break;

	case traTwoPhase :
		// join transaction 
		// tran->tra_ext_two_phase = ext_tran;
		break;
	}
}

void ExtTransaction::doStart(ISC_STATUS* status, thread_db* tdbb, TraScope traScope, TraModes traMode, 
		bool readOnly, bool wait, int lockTimeout)
{
	fb_assert(!m_handle);

	ClumpletWriter tpb(ClumpletReader::Tpb, 64, isc_tpb_version3);
	switch (traMode)
	{
	case traReadCommited: 
		tpb.insertTag(isc_tpb_read_committed);
	break;
	
	case traReadCommitedRecVersions: 
		tpb.insertTag(isc_tpb_read_committed);
		tpb.insertTag(isc_tpb_rec_version);
	break;
	
	case traConcurrency: 
		tpb.insertTag(isc_tpb_concurrency);
	break;
	
	case traConsistency: 
		tpb.insertTag(isc_tpb_consistency);
	break;
	}

	tpb.insertTag(readOnly ? isc_tpb_read : isc_tpb_write);
	tpb.insertTag(wait ? isc_tpb_wait : isc_tpb_nowait);
	if (wait && lockTimeout)
		tpb.insertInt(isc_tpb_lock_timeout, lockTimeout);

	FB_API_HANDLE db_handle = m_connection.getAPIHandle();

	EngineCallbackGuard guard(tdbb);
	m_provider.isc_start_transaction(status, &m_handle, 1, &db_handle, 
		tpb.getBufferLength(), tpb.getBuffer());
}

void ExtTransaction::prepare(thread_db* tdbb, char* info)
{
	ISC_STATUS_ARRAY status = {0};
	doPrepare(status, tdbb, info);
}

void ExtTransaction::doPrepare(ISC_STATUS* status, thread_db* tdbb, char* info)
{
}

void ExtTransaction::commit(thread_db* tdbb, bool retain)
{
	ISC_STATUS_ARRAY status = {0};
	doCommit(status, tdbb, retain);

	if (status[1]) {
		status_exception::raise(status);
	}

	fb_assert(retain && m_handle || !retain && !m_handle);
	if (!retain) {
		m_connection.deleteTransaction(this);
	}
}

void ExtTransaction::doCommit(ISC_STATUS* status, thread_db* tdbb, bool retain)
{
	EngineCallbackGuard guard(tdbb);
	if (retain)
		m_provider.isc_commit_retaining(status, &m_handle);
	else
		m_provider.isc_commit_transaction(status, &m_handle);
}

void ExtTransaction::rollback(thread_db* tdbb, bool retain)
{
	ISC_STATUS_ARRAY status = {0};
	doRollback(status, tdbb, retain);

	if (!retain) {
		m_connection.deleteTransaction(this);
	}

	if (status[1]) {
		status_exception::raise(status);
	}
}

void ExtTransaction::doRollback(ISC_STATUS* status, thread_db* tdbb, bool retain)
{
	EngineCallbackGuard guard(tdbb);
	if (retain)
		m_provider.isc_rollback_retaining(status, &m_handle);
	else
		m_provider.isc_rollback_transaction(status, &m_handle);
}


ExtTransaction& ExtTransaction::getTransaction(thread_db* tdbb, 
		ExtConnection &conn, TraScope tra_scope)
{
	jrd_tra *tran = tdbb->tdbb_transaction;
	ExtTransaction *ext_tran = conn.findTransaction(tdbb, tra_scope);

	if (!ext_tran)
	{
		ext_tran = &conn.createTransaction();

		TraModes traMode = traConcurrency;
		if (tran->tra_flags & TRA_read_committed) {
			if (tran->tra_flags & TRA_rec_version)
				traMode = traReadCommitedRecVersions;
			else
				traMode = traReadCommited;
		}
		else if (tran->tra_flags & TRA_degree3) {
			traMode = traConsistency;
		}

		try {
			ext_tran->start(tdbb, 
				tra_scope, 
				traMode, 
				tran->tra_flags & TRA_readonly, 
				tran->getLockWait() == DEFAULT_LOCK_TIMEOUT,
				tran->getLockWait() 
			);
		}
		catch (const Firebird::Exception&)
		{
			conn.deleteTransaction(ext_tran);
			throw;
		}
	}

	return *ext_tran;
}

void ExtTransaction::endTransaction(thread_db* tdbb, jrd_tra* transaction,
		bool commit, bool retain, bool force)
{
	ExtTransaction** ext_tran = &transaction->tra_ext_common;
	ExtTransaction* tran = *ext_tran;
	while (tran)
	{
		ExtTransaction* next = tran->m_nextTran;
		try 
		{
			if (commit)
				tran->commit(tdbb, retain);
			else
				tran->rollback(tdbb, retain);
		}
		catch(const Firebird::Exception&)
		{
			if (!force || commit)
				throw;

			// ignore rollback error
			tdbb->tdbb_status_vector[0] = isc_arg_gds;
			tdbb->tdbb_status_vector[0] = 0;
			tdbb->tdbb_status_vector[0] = isc_arg_end;
		}

		tran = next;
		if (!retain) {
			*ext_tran = tran;
		}
	}
}

// ExtStatement

ExtStatement::ExtStatement(ExtConnection &conn) :
	ExtObject(conn.getProvider()),
	m_connection(conn),
	m_transaction(NULL),
	m_sql(m_pool),
	m_in_xsqlda(NULL),
	m_out_xsqlda(NULL),
	m_in_buffer(m_pool),
	m_out_buffer(m_pool),
	m_stmt_selectable(false),
	m_singleton(false),
	m_error(false),
	m_active(false)
{
	// initialize sqlTypeToDscType in the first call to this function
	if (sqlTypeToDscType().count() == 0)
	{
		for (int i = 0; i < FB_NELEM(sqlType); ++i)
			sqlTypeToDscType().put(sqlType[i], static_cast<UCHAR>(i));
	}
}

ExtStatement::~ExtStatement()
{
	if (m_handle)
	{
		thread_db* tdbb = JRD_get_thread_data();
		EngineCallbackGuard guard(tdbb);
		ISC_STATUS_ARRAY status = {0};
		m_provider.isc_dsql_free_statement(status, &m_handle, DSQL_drop);
	}

	delete m_in_xsqlda;
	delete m_out_xsqlda;
}

void ExtStatement::prepare(thread_db* tdbb, ExtTransaction &tran, Firebird::string &sql)
{
	fb_assert(!m_active);

	// already prepared the same non-empty statement
	if (m_handle && (m_sql == sql) && (m_sql != ""))
		return;

	m_error = false;
	m_transaction = &tran;
	m_sql = "";

	FB_API_HANDLE h_conn = m_connection.getAPIHandle();
	FB_API_HANDLE h_tran = tran.getAPIHandle();

	ISC_STATUS_ARRAY status;

	// prepare and get output parameters
	if (!m_out_xsqlda)
	{
		m_out_xsqlda = (XSQLDA*) FB_NEW (m_pool) char [XSQLDA_LENGTH(1)];
		m_out_xsqlda->sqln = 1;
		m_out_xsqlda->version = 1;
	}

	const char *sWhereError = NULL;
	{
		EngineCallbackGuard guard(tdbb);

		if (!m_handle) {
			if (m_provider.isc_dsql_allocate_statement(status, &h_conn, &m_handle)) 
			{
				//m_error = true;
				//status_exception::raise(status);
				sWhereError = "isc_dsql_allocate_statement";
			}
		}

		if (!sWhereError) {
			if (m_provider.isc_dsql_prepare(status, &h_tran, &m_handle, sql.length(), 
				(char*) sql.c_str(), SQL_DIALECT_CURRENT, m_out_xsqlda)) 
			{
				sWhereError = "isc_dsql_prepare";
			}
		}
	}
	if (sWhereError) {
		raise(status, tdbb, sWhereError, &sql);
	}

	// adjust output parameters
	if (m_out_xsqlda->sqld > m_out_xsqlda->sqln)
	{
		const int n = m_out_xsqlda->sqld;
		delete (char*) m_out_xsqlda;

		m_out_xsqlda = (XSQLDA*) FB_NEW (m_pool) char [XSQLDA_LENGTH(n)];
		m_out_xsqlda->sqln = n;
		m_out_xsqlda->version = 1;

		EngineCallbackGuard guard(tdbb);
		if (m_provider.isc_dsql_describe(status, &m_handle, 1, m_out_xsqlda))
		{
			sWhereError = "isc_dsql_describe";
		}
	}
	if (sWhereError) {
		raise(status, tdbb, sWhereError, &sql);
	}

	size_t buf_len = parseSQLDA(m_out_xsqlda, NULL);
	parseSQLDA(m_out_xsqlda, m_out_buffer.getBuffer(buf_len));

	// get input parameters
	if (!m_in_xsqlda)
	{
		m_in_xsqlda = (XSQLDA*) FB_NEW (m_pool) char [XSQLDA_LENGTH(1)];
		m_in_xsqlda->sqln = 1;
		m_in_xsqlda->version = 1;
	}

	{
		EngineCallbackGuard guard(tdbb);
		if (m_provider.isc_dsql_describe_bind(status, &m_handle, 1, m_in_xsqlda))
		{
			sWhereError = "isc_dsql_describe_bind";
		}
	}
	if (sWhereError) {
		raise(status, tdbb, sWhereError, &sql);
	}

	// adjust input parameters
	if (m_in_xsqlda->sqld > m_in_xsqlda->sqln)
	{
		const int n = m_in_xsqlda->sqld;
		delete (char*) m_in_xsqlda;

		m_in_xsqlda = (XSQLDA*) FB_NEW (m_pool) char [XSQLDA_LENGTH(n)];
		m_in_xsqlda->sqln = n;
		m_in_xsqlda->version = 1;

		EngineCallbackGuard guard(tdbb);
		if (m_provider.isc_dsql_describe_bind(status, &m_handle, 1, m_in_xsqlda))
		{
			sWhereError = "isc_dsql_describe_bind";
		}
	}
	if (sWhereError) {
		raise(status, tdbb, sWhereError, &sql);
	}

	buf_len = parseSQLDA(m_in_xsqlda, NULL);
	parseSQLDA(m_in_xsqlda, m_in_buffer.getBuffer(buf_len));

	// get statement type
	const char stmt_info[] = {isc_info_sql_stmt_selectable};
	char info_buff[16];
	{
		EngineCallbackGuard guard(tdbb);
		if (m_provider.isc_dsql_sql_info(status, &m_handle, sizeof(stmt_info), stmt_info,
			sizeof(info_buff), info_buff))
		{
			sWhereError = "isc_dsql_sql_info";
		}
	}
	if (sWhereError) {
		raise(status, tdbb, sWhereError, &sql);
	}
	
	if (info_buff[0] != isc_info_sql_stmt_selectable) 
	{
		status[0] = isc_arg_gds;
		status[1] = isc_random;
		status[2] = isc_arg_string;
		status[3] = (ISC_STATUS) (IPTR) "Unknown statement type";
		status[4] = isc_arg_end;

		raise(status, tdbb, sWhereError, &sql);
	}

	{
		EngineCallbackGuard guard(tdbb);
		const int len = m_provider.isc_vax_integer(&info_buff[1], 2);
		m_stmt_selectable = (m_provider.isc_vax_integer(&info_buff[3], len) != 0);
	}

	m_transaction = NULL;
	m_sql = sql;
	m_sql.trim();
}

void ExtStatement::execute(thread_db* tdbb, ExtTransaction &tran, int in_count, 
	jrd_nod **in_params, int out_count, jrd_nod **out_params)
{
	fb_assert(m_handle && !m_stmt_selectable);
	fb_assert(!m_error);
	fb_assert(!m_active);
	
	m_transaction = &tran;
	FB_API_HANDLE h_tran = tran.getAPIHandle();
	
	setInParams(tdbb, in_count, in_params);

	ISC_STATUS_ARRAY status = {0};
	{
		EngineCallbackGuard guard(tdbb);
		m_provider.isc_dsql_execute2(status, &h_tran, &m_handle, 1, m_in_xsqlda, m_out_xsqlda);
	}
	if (status[1]) {
		raise(status, tdbb, "isc_dsql_execute2");
	}

	getOutParams(tdbb, out_count, out_params);
}

void ExtStatement::open(thread_db* tdbb, ExtTransaction &tran, int in_count, 
	jrd_nod **in_params, bool singleton)
{
	fb_assert(m_handle && m_stmt_selectable);
	fb_assert(!m_error);
	fb_assert(!m_active);
	
	m_singleton = singleton;
	m_transaction = &tran;
	FB_API_HANDLE h_tran = tran.getAPIHandle();
	
	setInParams(tdbb, in_count, in_params);

	m_active = true;
	ISC_STATUS_ARRAY status = {0};
	{
		EngineCallbackGuard guard(tdbb);
		m_provider.isc_dsql_execute(status, &h_tran, &m_handle, 1, m_in_xsqlda);
	}
	if (status[1]) {
		raise(status, tdbb, "isc_dsql_execute");
	}
}

bool ExtStatement::fetch(thread_db* tdbb, int out_count, jrd_nod **out_params)
{
	fb_assert(m_handle && m_stmt_selectable);
	fb_assert(!m_error);
	fb_assert(m_active);
	
	ISC_STATUS_ARRAY status = {0};
	{
		EngineCallbackGuard guard(tdbb);
		const ISC_STATUS res = m_provider.isc_dsql_fetch(status, &m_handle, 1, m_out_xsqlda);
		if (res == 100) {
			return false;
		}
	}
	if (status[1]) {
		raise(status, tdbb, "isc_dsql_fetch");
	}

	getOutParams(tdbb, out_count, out_params);

	if (m_singleton)
	{
		EngineCallbackGuard guard(tdbb);
		const ISC_STATUS res = m_provider.isc_dsql_fetch(status, &m_handle, 1, m_out_xsqlda);
		if (!res && !status[1]) 
		{
			status[0] = isc_arg_gds;
			status[1] = isc_sing_select_err;
			status[2] = isc_arg_end;
		}
		if (!status[1]) {
			return false;
		}
	}

	if (status[1]) {
		raise(status, tdbb, "isc_dsql_fetch");
	}
	return true;
}

void ExtStatement::close(thread_db* tdbb)
{
	if (m_handle && m_active)
	{
		fb_assert(m_stmt_selectable);

		ISC_STATUS_ARRAY status = {0};
		{
			EngineCallbackGuard guard(tdbb);
			m_provider.isc_dsql_free_statement(status, &m_handle, DSQL_close);
		}
		if (status[1]) {
			raise(status, tdbb, "isc_dsql_free_statement(DSQL_close)");
		}
	}
	m_active = false;

	bool commitFailed = false;
	if (m_transaction && m_transaction->getScope() == traAutonomous) 
	{
		if (!m_error) {
			try {
				m_transaction->commit(tdbb, false);
			}
			catch (const Exception& ex) {
				commitFailed = true;
				stuff_exception(tdbb->tdbb_status_vector, ex);
			}
		}

		if (m_error || commitFailed)
			m_transaction->rollback(tdbb, false);

		m_transaction = NULL;
	}
	m_connection.releaseStatement(this);
	m_error = false;

	if (commitFailed)
		ERR_punt();
}

// jrd_nod -> XSQLDA
void ExtStatement::setInParams(thread_db* tdbb, int count, jrd_nod **params)
{
	m_error = (count && (!m_in_xsqlda || m_in_xsqlda->sqld != count)) ||
			 (!count && m_in_xsqlda && m_in_xsqlda->sqld != count);

	if (m_error) {
		status_exception::raise(isc_random, isc_arg_string, "input parameters mismatch", 0);
	}

	if (!count)
		return;

	fb_assert(count && m_in_xsqlda && m_in_xsqlda->sqld == count);

	ISC_STATUS_ARRAY status = {0};
	const XSQLVAR *xVar = m_in_xsqlda->sqlvar;
	jrd_nod **jrdVar = params;
	GenericMap<Pair<NonPooled<jrd_nod*, dsc*>>> paramDescs(m_pool);
	for (int i = 0; i < count; i++, xVar++, jrdVar++)
	{
		dsc *src = NULL;
		if (!paramDescs.get(*jrdVar, src))
		{
			src = EVL_expr(tdbb, *jrdVar);
			paramDescs.put(*jrdVar, src);
		}

		// build the dst descriptor
		dsc dst;
		dst.clear();
		sqlTypeToDscType().get((xVar->sqltype & ~1), dst.dsc_dtype);
		dst.dsc_length = xVar->sqllen;
		dst.dsc_scale = xVar->sqlscale;
		dst.dsc_sub_type = xVar->sqlsubtype;
		dst.dsc_address = (UCHAR*) xVar->sqldata;

		if ((xVar->sqltype & ~1) == SQL_VARYING)
			dst.dsc_length += sizeof(SSHORT);

		// and assign to the target
		const bool srcNull = !src || src->isNull();
		if (xVar->sqltype & 1) 
			*xVar->sqlind = (srcNull ? -1 : 0);
		
		if (srcNull) 
			memset(xVar->sqldata, 0, xVar->sqllen);
		else 
			MOV_move(tdbb, src, &dst);
	}
}

// XSQLDA -> jrd_nod 
void ExtStatement::getOutParams(thread_db* tdbb, int count, jrd_nod **params)
{
	if (count && (!m_out_xsqlda || m_out_xsqlda->sqld != count))
	{
		m_error = true;
		status_exception::raise(isc_prc_out_param_mismatch, isc_arg_end);
	}

	if (!count && m_out_xsqlda && m_out_xsqlda->sqld != count)
	{
		m_error = true;
		status_exception::raise(isc_prc_out_param_mismatch, isc_arg_end);
	}

	if (!count)
		return;

	fb_assert(count && m_out_xsqlda && m_out_xsqlda->sqld == count);

	ISC_STATUS_ARRAY status = {0};

	const XSQLVAR *xVar = m_out_xsqlda->sqlvar;
	jrd_nod **jrdVar = params;
	for (int i = 0; i < count; i++, xVar++, jrdVar++)
	{
		dsc* d = EVL_assign_to(tdbb, *jrdVar);
		if (d->dsc_dtype >= FB_NELEM(sqlType) || sqlType[d->dsc_dtype] < 0) 
		{
			m_error = true;
			Firebird::status_exception::raise(
				isc_exec_sql_invalid_var, isc_arg_number, i + 1,
				isc_arg_string, (ISC_STATUS)(U_IPTR) ERR_cstring(m_sql.substr(0, 31)),
				isc_arg_end);
		}

		// build the src descriptor
		dsc src;
		src.clear();
		sqlTypeToDscType().get((xVar->sqltype & ~1), src.dsc_dtype);
		src.dsc_length = xVar->sqllen;
		src.dsc_scale = xVar->sqlscale;
		src.dsc_sub_type = xVar->sqlsubtype;
		src.dsc_address = (UCHAR*) xVar->sqldata;

		if ((xVar->sqltype & ~1) == SQL_VARYING)
			src.dsc_length += sizeof(SSHORT);

		// and assign to the target
		EXE_assignment(tdbb, *jrdVar, &src, (xVar->sqltype & 1) && (*xVar->sqlind < 0), NULL, NULL);
	}
}

void ExtStatement::raise(ISC_STATUS* status, thread_db* tdbb, const char* sWhere, 
		const Firebird::string* sQuery)
{
	m_error = true;

	// hvlad: TODO: error message like 
	// "Execute statement error at %s :\n%sStatement : %s\nData source : %s" or
	// "Execute statement error at @1 :\n@2Statement : @3\nData source : @4" 
	string err;
	err.printf("Execute statement error at %s :\n", sWhere);

	string rem_err;
	getRemoteError(status, rem_err);
	err += rem_err;

	sQuery = sQuery ? sQuery : &m_sql;
	err += "Statement : " + sQuery->substr(0, 255) + "\n";
	err += "Data source : " + m_connection.getDatasource();

	ERR_post(isc_random, isc_arg_string, ERR_cstring(err), 0);
}



//  EngineCallbackGuard
EngineCallbackGuard::EngineCallbackGuard(thread_db* tdbb)
{
	m_tdbb = tdbb;
	if (m_tdbb->tdbb_transaction) {
		m_tdbb->tdbb_transaction->tra_callback_count++;
	}
	THREAD_EXIT();
}

EngineCallbackGuard::~EngineCallbackGuard()
{
	THREAD_ENTER();
	if (m_tdbb->tdbb_transaction) {
		m_tdbb->tdbb_transaction->tra_callback_count--;
	}
}


// ExtProvider
// isc api 

ISC_STATUS ExtProvider::notImplemented(ISC_STATUS* status) const
{
	status[0] = isc_arg_gds;
	status[1] = isc_unavailable;
	status[2] = isc_arg_end;

	return status[1];
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_attach_database(ISC_STATUS *user_status, 
	short file_length, char *file_name, isc_db_handle *public_handle, 
	short dpb_length, char *dpb)
{
	if (!m_api.isc_attach_database)
		return notImplemented(user_status);
	else
		return (*m_api.isc_attach_database) (user_status, file_length, file_name, 
			public_handle, dpb_length, dpb);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_array_gen_sdl(ISC_STATUS *user_status,
	ISC_ARRAY_DESC *, short *, char *, short *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_array_get_slice(ISC_STATUS *user_status,
									  isc_db_handle *,
									  isc_tr_handle *,
									  ISC_QUAD *,
									  ISC_ARRAY_DESC *,
									  void *,
									  ISC_LONG *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_array_lookup_bounds(ISC_STATUS *user_status,
										  isc_db_handle *,
										  isc_tr_handle *,
										  char *,
										  char *,
										  ISC_ARRAY_DESC *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_array_lookup_desc(ISC_STATUS *user_status,
										isc_db_handle *,
										isc_tr_handle *,
										char *,
										char *,
										ISC_ARRAY_DESC *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_array_set_desc(ISC_STATUS *user_status,
									 char *,
									 char *,
									 short *,
									 short *,
									 short *,
									 ISC_ARRAY_DESC *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_array_put_slice(ISC_STATUS *user_status,
									  isc_db_handle *,
									  isc_tr_handle *,
									  ISC_QUAD *,
									  ISC_ARRAY_DESC *,
									  void *,
									  ISC_LONG *)
{
	return notImplemented(user_status);
}

void ISC_EXPORT ExtProvider::isc_blob_default_desc(ISC_BLOB_DESC *,
								  unsigned char *,
								  unsigned char *)
{
	return;
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_blob_gen_bpb(ISC_STATUS *user_status,
								   ISC_BLOB_DESC *,
								   ISC_BLOB_DESC *,
								   unsigned short,
								   unsigned char *,
								   unsigned short *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_blob_info(ISC_STATUS *user_status,
								isc_blob_handle *,
								short,
								char *,
								short,
								char *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_blob_lookup_desc(ISC_STATUS *user_status,
									   isc_db_handle *,
									   isc_tr_handle *,
									   unsigned char *,
									   unsigned char *,
									   ISC_BLOB_DESC *,
									   unsigned char *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_blob_set_desc(ISC_STATUS *user_status,
									unsigned char *,
									unsigned char *,
									short,
									short,
									short,
									ISC_BLOB_DESC *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_cancel_blob(ISC_STATUS *user_status,
								  isc_blob_handle *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_cancel_events(ISC_STATUS *user_status,
									isc_db_handle *,
									ISC_LONG *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_close_blob(ISC_STATUS *user_status,
								 isc_blob_handle *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_commit_retaining(ISC_STATUS *user_status,
	isc_tr_handle *tra_handle)
{
	if (!m_api.isc_commit_retaining)
		return notImplemented(user_status);
	else
		return (*m_api.isc_commit_retaining) (user_status, tra_handle);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_commit_transaction(ISC_STATUS *user_status,
	isc_tr_handle *tra_handle)
{
	if (!m_api.isc_commit_transaction)
		return notImplemented(user_status);
	else
		return (*m_api.isc_commit_transaction) (user_status, tra_handle);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_create_blob(ISC_STATUS *user_status,
								  isc_db_handle *,
								  isc_tr_handle *,
								  isc_blob_handle *,
								  ISC_QUAD *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_create_blob2(ISC_STATUS *user_status,
								   isc_db_handle *,
								   isc_tr_handle *,
								   isc_blob_handle *,
								   ISC_QUAD *,
								   short,
								   char *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_create_database(ISC_STATUS *user_status,
									  short,
									  char *,
									  isc_db_handle *,
									  short,
									  char *,
									  short)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_database_info(ISC_STATUS *user_status,
									isc_db_handle *,
									short,
									char *,
									short,
									char *)
{
	return notImplemented(user_status);
}

void ISC_EXPORT ExtProvider::isc_decode_date(ISC_QUAD *,
							void *)
{
	return ;
}

void ISC_EXPORT ExtProvider::isc_decode_sql_date(ISC_DATE *,
								void *)
{
	return ;
}

void ISC_EXPORT ExtProvider::isc_decode_sql_time(ISC_TIME *,
								void *)
{
	return ;
}

void ISC_EXPORT ExtProvider::isc_decode_timestamp(ISC_TIMESTAMP *,
								 void *)
{
	return ;
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_detach_database(ISC_STATUS *user_status,
									  isc_db_handle *public_handle)
{
	if (!m_api.isc_detach_database)
		return notImplemented(user_status);
	else
		return (*m_api.isc_detach_database) (user_status, public_handle);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_drop_database(ISC_STATUS *user_status,
									isc_db_handle *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_dsql_allocate_statement(ISC_STATUS *user_status,
	isc_db_handle *db_handle, isc_stmt_handle *stmt_handle)
{
	if (!m_api.isc_dsql_allocate_statement)
		return notImplemented(user_status);
	else
		return (*m_api.isc_dsql_allocate_statement) (user_status, db_handle, stmt_handle);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_dsql_alloc_statement2(ISC_STATUS *user_status,
	isc_db_handle *db_handle, isc_stmt_handle *stmt_handle)
{
	if (!m_api.isc_dsql_alloc_statement2)
		return notImplemented(user_status);
	else
		return (*m_api.isc_dsql_alloc_statement2) (user_status, db_handle, stmt_handle);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_dsql_describe(ISC_STATUS *user_status,
	isc_stmt_handle *stmt_handle, unsigned short dialect, XSQLDA *sqlda)
{
	if (!m_api.isc_dsql_describe)
		return notImplemented(user_status);
	else
		return (*m_api.isc_dsql_describe) (user_status, stmt_handle, dialect, sqlda);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_dsql_describe_bind(ISC_STATUS *user_status,
	isc_stmt_handle *stmt_handle, unsigned short dialect, XSQLDA *sqlda)
{
	if (!m_api.isc_dsql_describe_bind)
		return notImplemented(user_status);
	else
		return (*m_api.isc_dsql_describe_bind) (user_status, stmt_handle, dialect, sqlda);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_dsql_exec_immed2(ISC_STATUS *user_status,
									   isc_db_handle *,
									   isc_tr_handle *,
									   unsigned short,
									   char *,
									   unsigned short,
									   XSQLDA *,
									   XSQLDA *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_dsql_execute(ISC_STATUS *user_status,
	isc_tr_handle *tra_handle, isc_stmt_handle *stmt_handle, unsigned short dialect, 
	XSQLDA *sqlda)
{
	if (!m_api.isc_dsql_execute)
		return notImplemented(user_status);
	else
		return (*m_api.isc_dsql_execute) (user_status, tra_handle, stmt_handle, dialect, sqlda);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_dsql_execute2(ISC_STATUS *user_status,
	isc_tr_handle *tra_handle, isc_stmt_handle *stmt_handle, unsigned short dialect, 
	XSQLDA *in_sqlda, XSQLDA *out_sqlda)
{
	if (!m_api.isc_dsql_execute2)
		return notImplemented(user_status);
	else
		return (*m_api.isc_dsql_execute2) (user_status, tra_handle, stmt_handle, dialect, 
			in_sqlda, out_sqlda);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_dsql_execute_immediate(ISC_STATUS *user_status,
											 isc_db_handle *,
											 isc_tr_handle *,
											 unsigned short,
											 char *,
											 unsigned short,
											 XSQLDA *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_dsql_fetch(ISC_STATUS *user_status,
	isc_stmt_handle *stmt_handle, unsigned short da_version, XSQLDA *sqlda)
{
	if (!m_api.isc_dsql_fetch)
		return notImplemented(user_status);
	else
		return (*m_api.isc_dsql_fetch) (user_status, stmt_handle, da_version, sqlda);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_dsql_finish(isc_db_handle *)
{
	return isc_unavailable;
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_dsql_free_statement(ISC_STATUS *user_status,
	isc_stmt_handle *stmt_handle, unsigned short option)
{
	if (!m_api.isc_dsql_free_statement)
		return notImplemented(user_status);
	else 
		return (*m_api.isc_dsql_free_statement) (user_status, stmt_handle, option);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_dsql_insert(ISC_STATUS *user_status,
								  isc_stmt_handle *,
								  unsigned short,
								  XSQLDA *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_dsql_prepare(ISC_STATUS *user_status,
	isc_tr_handle *tra_handle, isc_stmt_handle *stmt_handle, unsigned short length,
	char *str, unsigned short dialect, XSQLDA *sqlda)
{
	if (!m_api.isc_dsql_prepare)
		return notImplemented(user_status);
	else
		return (*m_api.isc_dsql_prepare) (user_status, tra_handle, stmt_handle, 
				length, str, dialect, sqlda);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_dsql_set_cursor_name(ISC_STATUS *user_status,
										   isc_stmt_handle *,
										   char *,
										   unsigned short)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_dsql_sql_info(ISC_STATUS *user_status,
	isc_stmt_handle *stmt_handle, short items_len, const char *items, 
	short buffer_len, char *buffer)
{
	if (!m_api.isc_dsql_sql_info)
		return notImplemented(user_status);
	else 
		return (*m_api.isc_dsql_sql_info) (user_status, stmt_handle, items_len, items, 
				buffer_len, buffer);
}

void ISC_EXPORT ExtProvider::isc_encode_date(void *,
							ISC_QUAD *)
{
	return ;
}

void ISC_EXPORT ExtProvider::isc_encode_sql_date(void *,
								ISC_DATE *)
{
	return ;
}

void ISC_EXPORT ExtProvider::isc_encode_sql_time(void *,
								ISC_TIME *)
{
	return ;
}

void ISC_EXPORT ExtProvider::isc_encode_timestamp(void *,
								 ISC_TIMESTAMP *)
{
	return ;
}

ISC_LONG ISC_EXPORT_VARARG ExtProvider::isc_event_block(char * *,
									   char * *,
									   unsigned short, ...)
{
	return 0;
}

void ISC_EXPORT ExtProvider::isc_event_counts(ISC_ULONG *,
							 short,
							 char *,
							 char *)
{
	return ;
}

/* 17 May 2001 - ExtProvider::isc_expand_dpb is DEPRECATED */
void ISC_EXPORT_VARARG ExtProvider::isc_expand_dpb(char * *,
								  short *, ...)
{
	return ;
}

int ISC_EXPORT ExtProvider::isc_modify_dpb(char * *,
						  short *,
						  unsigned short,
						  char *,
						  short)
{
	return 0;
}

ISC_LONG ISC_EXPORT ExtProvider::isc_free(char *)
{
	return 0;
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_get_segment(ISC_STATUS *user_status,
								  isc_blob_handle *,
								  unsigned short *,
								  unsigned short,
								  char *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_get_slice(ISC_STATUS *user_status,
								isc_db_handle *,
								isc_tr_handle *,
								ISC_QUAD *,
								short,
								char *,
								short,
								ISC_LONG *,
								ISC_LONG,
								void *,
								ISC_LONG *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_interprete(char *,
								 ISC_STATUS * *)
{
	return isc_unavailable;
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_open_blob(ISC_STATUS *user_status,
								isc_db_handle *,
								isc_tr_handle *,
								isc_blob_handle *,
								ISC_QUAD *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_open_blob2(ISC_STATUS *user_status,
								 isc_db_handle *,
								 isc_tr_handle *,
								 isc_blob_handle *,
								 ISC_QUAD *,
								 ISC_USHORT,
								 ISC_UCHAR *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_prepare_transaction2(ISC_STATUS *user_status,
										   isc_tr_handle *,
										   ISC_USHORT,
										   ISC_UCHAR *)
{
	return notImplemented(user_status);
}

void ISC_EXPORT ExtProvider::isc_print_sqlerror(ISC_SHORT,
							   ISC_STATUS *)
{
	return ;
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_print_status(ISC_STATUS *)
{
	return isc_unavailable;
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_put_segment(ISC_STATUS *user_status,
								  isc_blob_handle *,
								  unsigned short,
								  char *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_put_slice(ISC_STATUS *user_status,
								isc_db_handle *,
								isc_tr_handle *,
								ISC_QUAD *,
								short,
								char *,
								short,
								ISC_LONG *,
								ISC_LONG,
								void *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_que_events(ISC_STATUS *user_status,
								 isc_db_handle *,
								 ISC_LONG *,
								 ISC_USHORT,
								 ISC_UCHAR *,
								 isc_callback,
								 void *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_rollback_retaining(ISC_STATUS *user_status,
	isc_tr_handle *tra_handle)
{
	if (!m_api.isc_rollback_retaining)
		return notImplemented(user_status);
	else
		return (*m_api.isc_rollback_retaining) (user_status, tra_handle);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_rollback_transaction(ISC_STATUS *user_status,
	isc_tr_handle *tra_handle)
{
	if (!m_api.isc_rollback_transaction)
		return notImplemented(user_status);
	else
		return (*m_api.isc_rollback_transaction) (user_status, tra_handle);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_start_multiple(ISC_STATUS *user_status,
	isc_tr_handle *tra_handle, short count, void *vec)
{
	if (!m_api.isc_start_multiple)
		return notImplemented(user_status);
	else
		return (*m_api.isc_start_multiple) (user_status, tra_handle, count, vec);
}

struct why_teb
{
	FB_API_HANDLE *teb_database;
	int teb_tpb_length;
	UCHAR *teb_tpb;
};

typedef why_teb WHY_TEB;

ISC_STATUS ISC_EXPORT_VARARG ExtProvider::isc_start_transaction(ISC_STATUS *user_status,
											   isc_tr_handle *tra_handle,
											   short count, ...)
{
	if (!m_api.isc_start_multiple)				// !!!
		return notImplemented(user_status);

	Firebird::HalfStaticArray<WHY_TEB, 16> tebs;
	WHY_TEB* teb = tebs.getBuffer(count);

	const WHY_TEB* const end = teb + count;
	va_list ptr;
	va_start(ptr, count);

	for (WHY_TEB* teb_iter = teb; teb_iter < end; teb_iter++) 
	{
		teb_iter->teb_database = va_arg(ptr, FB_API_HANDLE*);
		teb_iter->teb_tpb_length = va_arg(ptr, int);
		teb_iter->teb_tpb = va_arg(ptr, UCHAR *);
	}
	va_end(ptr);

	return (*m_api.isc_start_multiple) (user_status, tra_handle, count, teb);
}

ISC_STATUS ISC_EXPORT_VARARG ExtProvider::isc_reconnect_transaction(ISC_STATUS *user_status,
                                               isc_db_handle *,
                                               isc_tr_handle *,
                                               short, 
                                               char*)
{
	return notImplemented(user_status);
}

ISC_LONG ISC_EXPORT ExtProvider::isc_sqlcode(ISC_STATUS *)
{
	return isc_unavailable;
}

void ISC_EXPORT ExtProvider::isc_sql_interprete(short,
							   char *,
							   short)
{
	return ;
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_transaction_info(ISC_STATUS *user_status,
									   isc_tr_handle *,
									   short,
									   char *,
									   short,
									   char *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_transact_request(ISC_STATUS *user_status,
									   isc_db_handle *,
									   isc_tr_handle *,
									   unsigned short,
									   char *,
									   unsigned short,
									   char *,
									   unsigned short,
									   char *)
{
	return notImplemented(user_status);
}

ISC_LONG ISC_EXPORT ExtProvider::isc_vax_integer(char *p, short len)
{
	return ::isc_vax_integer(p, len);
}

ISC_INT64 ISC_EXPORT ExtProvider::isc_portable_integer(unsigned char *p, short len)
{
	return ::isc_portable_integer(p, len);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_seek_blob(ISC_STATUS *user_status,
								isc_blob_handle *,
								short,
								ISC_LONG,
								ISC_LONG *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_service_attach(ISC_STATUS *user_status,
									 unsigned short,
									 char *,
									 isc_svc_handle *,
									 unsigned short,
									 char *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_service_detach(ISC_STATUS *user_status,
									 isc_svc_handle *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_service_query(ISC_STATUS *user_status,
									isc_svc_handle *,
									isc_resv_handle *,
									unsigned short,
									char *,
									unsigned short,
									char *,
									unsigned short,
									char *)
{
	return notImplemented(user_status);
}

ISC_STATUS ISC_EXPORT ExtProvider::isc_service_start(ISC_STATUS *user_status,
									isc_svc_handle *,
									isc_resv_handle *,
									unsigned short,
									char *)
{
	return notImplemented(user_status);
}

void ExtProvider::loadAPI()
{
	ISC_STATUS_ARRAY status;
	notImplemented(status);
	status_exception::raise(status);
}


static void getRemoteError(ISC_STATUS* status, string &err)
{
	err = "";

	char buff[512];
	const ISC_STATUS* p = status, *end = status + ISC_STATUS_LENGTH;
	while (p < end)
	{
		const ISC_STATUS code = *p ? p[1] : 0;
		if (!fb_interpret(buff, sizeof(buff), &p))
			break;

		string rem_err;
		rem_err.printf("%lu : %s\n", code, buff);
		err += rem_err;
	}
}


static size_t parseSQLDA(XSQLDA *xsqlda, char *buff)
{
    size_t offset = 0;
    int i = 0;
	XSQLVAR* var = xsqlda->sqlvar;
    for (; i < xsqlda->sqld; var++, i++)
	{
        USHORT length = var->sqllen;
        const int type = var->sqltype & (~1);
		if (type == SQL_VARYING) {
            length += sizeof(SSHORT);
		}
		if (buff) {
			var->sqldata = &buff[offset];
		}
        offset += length;
    }

	offset = ROUNDUP(offset, sizeof(short));

	if (buff) {
		for (i = 0, var = xsqlda->sqlvar; i < xsqlda->sqld; var++, i++)
		{
			var->sqlind = (short*) (&buff[offset]);
			offset += sizeof(short);
		}
	}
	else {
		offset += sizeof(short) * xsqlda->sqld;
	}

    return offset;
}
