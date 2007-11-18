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

#include "firebird.h"
#include "fb_types.h"
#include "../jrd/common.h"
#include "../jrd/jrd_blks.h"
#include "../include/fb_blk.h"
#include "fb_exception.h"
#include "iberror.h"

#include "ibase.h"
#include "../common/classes/init.h"

#include "../jrd/dsc.h"
#include "../jrd/exe.h"
#include "../jrd/ExtDS.h"

#include "../dsql/chars.h"


using namespace Firebird;

namespace Jrd {

const char *NATIVE_PROVIDER_NAME = "NATIVE";

class NativeProvider : public ExtProvider
{
public:
	NativeProvider(MemoryPool &pool, const string &prvName) :
		ExtProvider(pool, prvName)
	{
		m_flags = (prvMultyStmts | prvMultyTrans);
	}

protected:
	virtual void loadAPI();
	virtual ExtConnection* doCreateConnection();
};

class NativeConnection : public ExtConnection
{
public:
	NativeConnection(ExtProvider &prov) : 
	  ExtConnection(prov) 
	 { m_internal = false; }

	virtual ~NativeConnection() {}

	virtual void attach(thread_db* tdbb, const Firebird::string &dbName, const Firebird::string &user, 
		const Firebird::string &pwd);
	virtual void detach(thread_db* tdbb);

	virtual bool isAvailable(thread_db* tdbb, TraScope traScope) const;

	bool isInternal() const
	{ return m_internal; }

protected:
	virtual ExtTransaction*	doCreateTransaction();
	virtual ExtStatement*	doCreateStatement();

private:
	virtual void getInternal(thread_db* tdbb);

	bool m_internal;
};


class NativeTransaction : public ExtTransaction
{
protected:
	friend class NativeConnection;

	NativeTransaction(NativeConnection &conn) :
	  ExtTransaction(conn) {}

	virtual ~NativeTransaction() {}

protected:
	virtual void doStart(ISC_STATUS* status, thread_db* tdbb, TraScope traScope, 
		TraModes traMode, bool readOnly, bool wait, int lockTimeout);
	virtual void doPrepare(ISC_STATUS* status, thread_db* tdbb, char* info);
	virtual void doCommit(ISC_STATUS* status, thread_db* tdbb, bool retain);
	virtual void doRollback(ISC_STATUS* status, thread_db* tdbb, bool retain);

private:
	void getInternal(ISC_STATUS* status, thread_db* tdbb);
};


class NativeStatement : public ExtStatement
{
public:
	NativeStatement(ExtConnection &conn) :
	  ExtStatement(conn),
	  m_map(m_pool),
	  m_userParamsCount(0),
	  m_userSql(m_pool)
	  {}

	virtual void prepare(thread_db* tdbb, ExtTransaction &tran, string &sql);

protected:
	virtual ~NativeStatement() {}

	void preprocess(string &sql, string &ret);
	virtual void setInParams(thread_db* tdbb, int count, jrd_nod **params);

	// sql (?) param num -> user named param num
	GenericMap<Pair<NonPooled<int, int>>> m_map;
	int m_userParamsCount;
	string m_userSql;
};

/////////////////////////////////////////////////////////////////////

static func_register_builtin_provider *save_register_builtin_provider = NULL;

static void registerNative(ExtProvManager& manager)
{
	MemoryPool  &pool = manager.getPool();
	string name(pool, NATIVE_PROVIDER_NAME, strlen(NATIVE_PROVIDER_NAME));
	NativeProvider* provider = FB_NEW(pool) NativeProvider(pool, name);
	manager.addProvider(provider);

	if (save_register_builtin_provider)
		(*save_register_builtin_provider) (manager);
}

class SetRegisterFunction
{
public:
	SetRegisterFunction()
	{
		save_register_builtin_provider = register_builtin_provider;
		register_builtin_provider = &registerNative;
	}
};

SetRegisterFunction setRegisterFunction;

// 	
ExtConnection* NativeProvider::doCreateConnection()
{
	return FB_NEW(m_pool) NativeConnection(*this);
}


// NativeConnection

void NativeConnection::attach(thread_db* tdbb, const Firebird::string &dbName, const Firebird::string &user, 
	const Firebird::string &pwd)
{
	fb_assert(!m_handle);

	m_internal = (dbName.isEmpty() && (user.isEmpty() || 
		user == tdbb->tdbb_attachment->att_user->usr_user_name));

	if (m_internal)
	{
		getInternal(tdbb);

		m_dbName = dbName;
		m_userName = user;
		m_password = pwd;
	}
	else {
		ExtConnection::attach(tdbb, dbName, user, pwd);
	}
}

void NativeConnection::detach(thread_db* tdbb)
{
	fb_assert(m_handle);

	if (m_internal)
		m_handle = NULL;
	else
		ExtConnection::detach(tdbb);
}

bool NativeConnection::isAvailable(thread_db* tdbb, TraScope traScope) const
{
	// internal (current) connection may be used only to serve calls from itself
	if (m_internal && tdbb->tdbb_attachment->att_public_handle != m_handle)
		return false;

	return ExtConnection::isAvailable(tdbb, traScope);
}

ExtTransaction*	NativeConnection::doCreateTransaction()
{
	return FB_NEW(m_pool) NativeTransaction(*this);
}

ExtStatement* NativeConnection::doCreateStatement()
{
	return FB_NEW(m_pool) NativeStatement(*this);
}

void NativeConnection::getInternal(thread_db* tdbb)
{
	char dpb[2] = {isc_dpb_version1, isc_dpb_current_attachment};

	ISC_STATUS_ARRAY status = {0};
	{
		EngineCallbackGuard guard(tdbb);
		m_provider.isc_attach_database(status, 0, NULL, &m_handle, sizeof(dpb), dpb);
	}

	if (status[1]) {
		status_exception::raise(status);
	}
}


// NativeTransaction

void NativeTransaction::doStart(ISC_STATUS* status, thread_db *tdbb, TraScope traScope, 
	TraModes traMode, bool readOnly, bool wait, int lockTimeout)
{
	fb_assert(!m_handle);

	if (traScope == traCommon && ((NativeConnection&)m_connection).isInternal() ) {
		getInternal(status, tdbb);
	}
	else {
		ExtTransaction::doStart(status, tdbb, traScope, traMode, readOnly, wait, lockTimeout);
	}
}

void NativeTransaction::doPrepare(ISC_STATUS* status, thread_db *tdbb, char* info)
{
	fb_assert(m_handle);

	const bool is_internal = ((NativeConnection&)m_connection).isInternal();
	if (m_scope != traCommon || !is_internal) 
		ExtTransaction::doPrepare(status, tdbb, info);
}

void NativeTransaction::doCommit(ISC_STATUS* status, thread_db *tdbb, bool retain)
{
	fb_assert(m_handle);

	const bool is_internal = ((NativeConnection&)m_connection).isInternal();
	if (m_scope != traCommon || !is_internal) 
		ExtTransaction::doCommit(status, tdbb, retain);
	else
		m_handle = 0;
}

void NativeTransaction::doRollback(ISC_STATUS* status, thread_db *tdbb, bool retain)
{
	fb_assert(m_handle);

	const bool is_internal = ((NativeConnection&)m_connection).isInternal();
	if (m_scope != traCommon || !is_internal) 
		ExtTransaction::doRollback(status, tdbb, retain);
	else
		m_handle = 0;
}

void NativeTransaction::getInternal(ISC_STATUS* status, thread_db *tdbb)
{
	char tpb[2] = {isc_tpb_version3, isc_tpb_current_transaction};

	FB_API_HANDLE db_handle = m_connection.getAPIHandle();
	EngineCallbackGuard guard(tdbb);

	m_provider.isc_start_transaction(status, &m_handle, 1, &db_handle,
		sizeof(tpb), tpb);
}



// NativeStatement

void NativeStatement::prepare(thread_db *tdbb, ExtTransaction &tran, Firebird::string &sql)
{
	if (m_handle && m_userSql == sql)
		return;

	string sql2(m_pool);
	preprocess(sql, sql2);

	//if (m_handle && m_sql == sql2)
	//	return;

	ExtStatement::prepare(tdbb, tran, sql2);
	m_userSql = sql;
}


typedef enum {ttNone, ttWhite, ttComment, ttString, ttParamMark, ttIdent, ttOther} tokenType;

tokenType getToken(const char **begin, const char *end)
{
	tokenType ret = ttNone;
	const char *p = *begin;

	char c = *p++;
	switch (c)
	{
	case ':': 
	case '?':
		ret = ttParamMark;
	break;

	case '\'':
	case '"':
	{
		const char q = c;
		while (p < end)
			if (*p++ == q) {
				ret = ttString;
				break;
			}
	}
	break;
	
	case '/':
		if (p < end && *p == '*') {
			while (p < end) {
				if (*p++ == '*' && p < end && *p == '/') {
					p++;
					ret = ttComment;
					break;
				}
			}
		}
		else {
			ret = ttOther;
		}
	break;

	case '-':
		if (p < end && *p == '-') {
			while (p < end) {
				if (*p++ == '\n') {
					p--;
					ret = ttComment;
					break;
				}
			}
		}
		else {
			ret = ttOther;
		}
	break;

	default:
		if (classes(c) & CHR_DIGIT)
		{
			while (p < end && classes(*p) & CHR_DIGIT)
				p++;
			ret = ttOther;
		}
		else if (classes(c) & CHR_IDENT)
		{
			while (p < end && classes(*p) & CHR_IDENT)
				p++;
			ret = ttIdent;
		}
		else if (classes(c) & CHR_WHITE)
		{
			while (p < end && (classes(*p) & CHR_WHITE))
				*p++;
			ret = ttWhite;
		}
		else 
		{
			c = *p;
			while (p < end && !( classes(c) & (CHR_DIGIT | CHR_IDENT | CHR_WHITE) ) && 
				c != '/' && c != '-' && c != ':' && c != '?' && c != '\'' && c != '"')
			{
				c = *p++;
			}
			ret = ttOther;
		}
	}

	*begin = p;
	return ret;
}

void NativeStatement::preprocess(string &sql, string &ret)
{
	m_userParamsCount = 0;
	m_map.clear();

	Firebird::ObjectsArray<string> names(m_pool);	// hvlad: TODO: sorted array

	bool passAsIs = true, execBlock = false;
	const char *p = sql.begin(), *end = sql.end();
	const char *start = p;
	tokenType tok = getToken(&p, end);

	const char *i = start;
	while (p < end && (tok == ttComment || tok == ttWhite)) 
	{
		i = p;
		tok = getToken(&p, end);
	}

	if (p >= end || tok != ttIdent) {
		ERR_post(isc_random, isc_arg_string, "Statement expected", isc_arg_end);
	}

	start = i; // skip leading comments ??
	string ident(i, p - i);
	ident.upper();

	if (ident == "EXECUTE")
	{
		const char *i2 = p;
		tok = getToken(&p, end);
		while (p < end && (tok == ttComment || tok == ttWhite)) 
		{
			i2 = p;
			tok = getToken(&p, end);
		}
		if (p >= end || tok != ttIdent) {
			ERR_post(isc_random, isc_arg_string, "Statement expected", isc_arg_end);
		}
		string ident2(i2, p - i2);
		ident2.upper();

		execBlock = (ident2 == "BLOCK");
		passAsIs = false;
	}
	else {
		passAsIs = !(ident == "INSERT" || ident == "UPDATE" ||  ident == "DELETE" 
			|| ident == "MERGE" || ident == "SELECT" || ident == "WITH");
	}

	if (passAsIs)
	{
		ret = sql;
		return;
	}

	ret += string(start, p - start);

	int sqlParams = 0;
	while (p < end)
	{
		start = p;
		tok = getToken(&p, end);
		switch (tok)
		{
		case ttParamMark:
			tok = getToken(&p, end);
			if (tok == ttIdent /*|| tok == ttString*/)
			{
				// hvlad: TODO: check quoted param names
				ident.assign(start + 1, p - start - 1);
				if (tok == ttIdent)
					ident.upper();

				int n = 0;
				for (; n < names.getCount(); n++)
					if (names[n] == ident)
						break;

				if (n >= names.getCount())
				{
					n = names.getCount();
					names.add(ident);
					m_userParamsCount++;
				}

				m_map.put(sqlParams++, n);
			}
			else {
				ERR_post(isc_random, isc_arg_string, "Parameter name expected", 0);
			}
			ret += '?';
		break;
		
		case ttIdent:
			if (execBlock)
			{
				ident.assign(start, p - start);
				ident.upper();
				if (ident == "AS")
				{
					ret += string(start, end - start);
					return;
				}
			}
			// fall thru
		
		case ttWhite:
		case ttComment:
		case ttString:
		case ttOther:
			ret += string(start, p - start);
		break;

		case ttNone: 
			ERR_post(isc_random, isc_arg_string, "parse error", 0);
		break;
		}
	}
	return;
}

void NativeStatement::setInParams(thread_db* tdbb, int count, jrd_nod **userParams)
{
	if (count != m_userParamsCount) {
		status_exception::raise(isc_random, isc_arg_string, "input parameters mismatch", 0);
	}

	const int sqlCount = m_map.count();
	Array<jrd_nod*> sqlParamsArray(m_pool, 16);
	jrd_nod **sqlParams = sqlParamsArray.getBuffer(sqlCount);
	if (count && m_map.getFirst())  {
		do 
		{
			const int sqlNum = m_map.current()->first;
			const int userNum = m_map.current()->second;
			
			fb_assert(userNum >= 0 && userNum < count);

			sqlParams[sqlNum] = userParams[userNum];
		} while(m_map.getNext());
	}

	ExtStatement::setInParams(tdbb, sqlCount, sqlParams);
}

// NativeProvider

#define PROTO(X) (prototype_##X*)&X

static FirebirdApiPointer isc_callbacks = 
{
	PROTO(isc_attach_database),
	PROTO(isc_array_gen_sdl),
	PROTO(isc_array_get_slice),
	PROTO(isc_array_lookup_bounds),
	PROTO(isc_array_lookup_desc),
	PROTO(isc_array_set_desc),
	PROTO(isc_array_put_slice),
	PROTO(isc_blob_default_desc),
	PROTO(isc_blob_gen_bpb),
	PROTO(isc_blob_info),
	PROTO(isc_blob_lookup_desc),
	PROTO(isc_blob_set_desc),
	PROTO(isc_cancel_blob),
	PROTO(isc_cancel_events),
	PROTO(isc_close_blob),
	PROTO(isc_commit_retaining),
	PROTO(isc_commit_transaction),
	PROTO(isc_create_blob),
	PROTO(isc_create_blob2),
	PROTO(isc_create_database),
	PROTO(isc_database_info),
	PROTO(isc_decode_date),
	PROTO(isc_decode_sql_date),
	PROTO(isc_decode_sql_time),
	PROTO(isc_decode_timestamp),
	PROTO(isc_detach_database),
	PROTO(isc_drop_database),
	PROTO(isc_dsql_allocate_statement),
	PROTO(isc_dsql_alloc_statement2),
	PROTO(isc_dsql_describe),
	PROTO(isc_dsql_describe_bind),
	PROTO(isc_dsql_exec_immed2),
	PROTO(isc_dsql_execute),
	PROTO(isc_dsql_execute2),
	PROTO(isc_dsql_execute_immediate),
	PROTO(isc_dsql_fetch),
	PROTO(isc_dsql_finish),
	PROTO(isc_dsql_free_statement),
	PROTO(isc_dsql_insert),
	PROTO(isc_dsql_prepare),
	PROTO(isc_dsql_set_cursor_name),
	PROTO(isc_dsql_sql_info),
	PROTO(isc_encode_date),
	PROTO(isc_encode_sql_date),
	PROTO(isc_encode_sql_time),
	PROTO(isc_encode_timestamp),
	PROTO(isc_event_block),
	PROTO(isc_event_counts),
	PROTO(isc_expand_dpb),
	PROTO(isc_modify_dpb),
	PROTO(isc_free),
	PROTO(isc_get_segment),
	PROTO(isc_get_slice),
	PROTO(isc_interprete),
	PROTO(isc_open_blob),
	PROTO(isc_open_blob2),
	PROTO(isc_prepare_transaction2),
	PROTO(isc_print_sqlerror),
	PROTO(isc_print_status),
	PROTO(isc_put_segment),
	PROTO(isc_put_slice),
	PROTO(isc_que_events),
	PROTO(isc_rollback_retaining),
	PROTO(isc_rollback_transaction),
	PROTO(isc_start_multiple),
	PROTO(isc_start_transaction),
	PROTO(isc_reconnect_transaction),
	PROTO(isc_sqlcode),
	PROTO(isc_sql_interprete),
	PROTO(isc_transaction_info),
	PROTO(isc_transact_request),
	PROTO(isc_vax_integer),
	PROTO(isc_seek_blob),
	PROTO(isc_service_attach),
	PROTO(isc_service_detach),
	PROTO(isc_service_query),
	PROTO(isc_service_start),
};


void NativeProvider::loadAPI()
{
	m_api = isc_callbacks;
	m_api_loaded = true;
}

}; // namespace Jrd