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
 *  The Original Code was created by Adriano dos Santos Fernandes
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2008 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "ibase.h"
#include "FirebirdUdrCpp.h"
#include <assert.h>
#include <stdio.h>


using namespace Firebird;
using namespace Firebird::Udr;


typedef ISC_LONG (ISC_EXPORT_VARARG *FuncEventBlock)(ISC_UCHAR**, ISC_UCHAR**, ISC_USHORT, ...);
typedef ISC_STATUS (ISC_EXPORT *FuncWaitForEvent)(ISC_STATUS*, isc_db_handle*,
	short, const ISC_UCHAR*, ISC_UCHAR*);
typedef void (ISC_EXPORT *FuncEventCounts)(ISC_ULONG*, short, ISC_UCHAR*, const ISC_UCHAR*);

typedef ISC_STATUS (ISC_EXPORT *FuncDsqlAllocateStatement)(ISC_STATUS*, isc_db_handle*,
	isc_stmt_handle*);
typedef ISC_STATUS (ISC_EXPORT *FuncDsqlDescribe)(ISC_STATUS*, isc_stmt_handle*, unsigned short,
	XSQLDA*);
typedef ISC_STATUS (ISC_EXPORT *FuncDsqlDescribeBind)(ISC_STATUS*, isc_stmt_handle*, unsigned short,
	XSQLDA*);
typedef ISC_STATUS (ISC_EXPORT *FuncDsqlExecute)(ISC_STATUS*, isc_tr_handle*, isc_stmt_handle*,
	unsigned short, XSQLDA*);
typedef ISC_STATUS (ISC_EXPORT *FuncDsqlExecute2)(ISC_STATUS*, isc_tr_handle*, isc_stmt_handle*,
	unsigned short, XSQLDA*, XSQLDA*);
typedef ISC_STATUS (ISC_EXPORT *FuncDsqlFreeStatement)(ISC_STATUS*, isc_stmt_handle*, unsigned short);
typedef ISC_STATUS (ISC_EXPORT *FuncDsqlPrepare)(ISC_STATUS*, isc_tr_handle*, isc_stmt_handle*,
	unsigned short, const ISC_SCHAR*, unsigned short, XSQLDA*);


/***
create function wait_event (
    event_name varchar(31) character set ascii
) returns integer
    external name 'udrcpp_example!wait_event'
    engine udr;
***/
FB_UDR_DECLARE_FUNCTION(wait_event)


/***
create function sum_args (
    n1 integer,
    n2 integer,
    n3 integer
) returns integer
    external name 'udrcpp_example!sum_args'
    engine udr;
***/
FB_UDR_DECLARE_FUNCTION(sum_args)


/***
create procedure gen_rows (
    start_n integer not null,
    end_n integer not null
) returns (
    n integer not null
)
    external name 'udrcpp_example!gen_rows'
    engine udr;
***/
FB_UDR_DECLARE_PROCEDURE(gen_rows)
FB_UDR_BEGIN_DECLARE_FETCH_PROCEDURE(gen_rows)
	int counter;
	int end;
FB_UDR_END_DECLARE_FETCH_PROCEDURE(gen_rows)


/***
Sample usage:

create database 'c:\temp\slave.fdb';
create table persons (
    id integer not null,
    name varchar(60) not null,
    address varchar(60),
    info blob sub_type text
);
commit;

create database 'c:\temp\master.fdb';
create table persons (
    id integer not null,
    name varchar(60) not null,
    address varchar(60),
    info blob sub_type text
);

create table replicate_config (
    name varchar(31) not null,
    data_source varchar(255) not null
);

insert into replicate_config (name, data_source)
   values ('ds1', 'c:\temp\slave.fdb');

create trigger persons_replicate
    after insert on persons
    external name 'udrcpp_example!replicate!ds1'
    engine udr;
***/
FB_UDR_BEGIN_DECLARE_TRIGGER(replicate)
public:
	FB_UDR_TRIGGER(replicate)();
	~FB_UDR_TRIGGER(replicate)();

private:
	void initialize(ExternalContext* context, Values* values);

	bool initialized;
	XSQLDA* inSqlDa;
	isc_stmt_handle stmtHandle;

	// ISC entry points
	FuncDsqlAllocateStatement funcDsqlAllocateStatement;
	FuncDsqlDescribe funcDsqlDescribe;
	FuncDsqlDescribeBind funcDsqlDescribeBind;
	FuncDsqlExecute funcDsqlExecute;
	FuncDsqlExecute2 funcDsqlExecute2;
	FuncDsqlFreeStatement funcDsqlFreeStatement;
	FuncDsqlPrepare funcDsqlPrepare;
FB_UDR_END_DECLARE_TRIGGER(replicate)


FB_UDR_BEGIN_FUNCTION(wait_event)
{
	// ISC entry points
	FuncEventBlock funcEventBlock = (FuncEventBlock) getEntryPoint(context, "isc_event_block");
	FuncWaitForEvent funcWaitForEvent = (FuncWaitForEvent) getEntryPoint(context, "isc_wait_for_event");
	FuncEventCounts funcEventCounts = (FuncEventCounts) getEntryPoint(context, "isc_event_counts");

	Value* val = params->getValue(ThrowError(), 1);

	const char* s = val->getString(ThrowError());

	unsigned char* eveBuffer;
	unsigned char* eveResult;
	int eveLen = funcEventBlock(&eveBuffer, &eveResult, 1, s);

	ISC_STATUS_ARRAY statusVector = {0};
	isc_db_handle dbHandle = getIscDbHandle(context);
	ISC_ULONG counter = 0;

	ThrowError::check(funcWaitForEvent(statusVector, &dbHandle, eveLen, eveBuffer, eveResult),
		statusVector);
	funcEventCounts(&counter, eveLen, eveBuffer, eveResult);
	ThrowError::check(funcWaitForEvent(statusVector, &dbHandle, eveLen, eveBuffer, eveResult),
		statusVector);
	funcEventCounts(&counter, eveLen, eveBuffer, eveResult);

	isc_free((char*) eveBuffer);
	isc_free((char*) eveResult);

	// returns the counter
	result->setInt(ThrowError(), counter);
}
FB_UDR_END_FUNCTION(wait_event)


FB_UDR_BEGIN_FUNCTION(sum_args)
{
	unsigned count = params->getCount();
	int ret = 0;

	for (unsigned i = 0; i < count; ++i)
	{
		Value* val = params->getValue(ThrowError(), i + 1);
		ret += val->getInt(ThrowError());
	}

	result->setInt(ThrowError(), ret);
}
FB_UDR_END_FUNCTION(sum_args)


FB_UDR_BEGIN_PROCEDURE(gen_rows)
{
	Value* valStart = params->getValue(ThrowError(), 1);
	Value* valEnd = params->getValue(ThrowError(), 2);

	counter = valStart->getInt(ThrowError());
	end = valEnd->getInt(ThrowError());
}
FB_UDR_FETCH_PROCEDURE(gen_rows)
{
	if (counter > end)
		return false;

	Value* ret = results->getValue(ThrowError(), 1);
	ret->setInt(ThrowError(), counter++);

	return true;
}
FB_UDR_END_PROCEDURE(gen_rows)


FB_UDR_TRIGGER(replicate)::FB_UDR_TRIGGER(replicate)()
	: initialized(false)
{
}

FB_UDR_TRIGGER(replicate)::~FB_UDR_TRIGGER(replicate)()
{
	if (!initialized)
		return;

	for (int i = 0; i < inSqlDa->sqln; ++i)
	{
		XSQLVAR* var = &inSqlDa->sqlvar[i];
		delete [] var->sqldata;
		delete var->sqlind;
	}

	delete [] reinterpret_cast<char*>(inSqlDa);

	ISC_STATUS_ARRAY statusVector = {0};
	funcDsqlFreeStatement(statusVector, &stmtHandle, DSQL_drop);
}

void FB_UDR_TRIGGER(replicate)::initialize(ExternalContext* context, Values* values)
{
	if (initialized)
		return;

	// ISC entry points
	funcDsqlAllocateStatement = (FuncDsqlAllocateStatement)
		getEntryPoint(context, "isc_dsql_allocate_statement");
	funcDsqlDescribe = (FuncDsqlDescribe) getEntryPoint(context, "isc_dsql_describe");
	funcDsqlDescribeBind = (FuncDsqlDescribeBind) getEntryPoint(context, "isc_dsql_describe_bind");
	funcDsqlExecute = (FuncDsqlExecute) getEntryPoint(context, "isc_dsql_execute");
	funcDsqlExecute2 = (FuncDsqlExecute2) getEntryPoint(context, "isc_dsql_execute2");
	funcDsqlFreeStatement = (FuncDsqlFreeStatement) getEntryPoint(context, "isc_dsql_free_statement");
	funcDsqlPrepare = (FuncDsqlPrepare) getEntryPoint(context, "isc_dsql_prepare");

	ISC_STATUS_ARRAY statusVector = {0};
	isc_db_handle dbHandle = getIscDbHandle(context);
	isc_tr_handle trHandle = getIscTrHandle(context);

	stmtHandle = 0;
	ThrowError::check(funcDsqlAllocateStatement(statusVector, &dbHandle, &stmtHandle), statusVector);
	ThrowError::check(funcDsqlPrepare(statusVector, &trHandle, &stmtHandle, 0,
		"select data_source from replicate_config where name = ?",
		SQL_DIALECT_CURRENT, NULL), statusVector);

	inSqlDa = reinterpret_cast<XSQLDA*>(new char[(XSQLDA_LENGTH(1))]);
	inSqlDa->version = SQLDA_VERSION1;
	inSqlDa->sqln = 1;
	ThrowError::check(funcDsqlDescribeBind(statusVector, &stmtHandle, SQL_DIALECT_CURRENT, inSqlDa),
		statusVector);
	inSqlDa->sqlvar[0].sqldata = new char[sizeof(short) + inSqlDa->sqlvar[0].sqllen];
	strncpy(inSqlDa->sqlvar[0].sqldata + sizeof(short), metaInfo->info, inSqlDa->sqlvar[0].sqllen);
	*reinterpret_cast<short*>(inSqlDa->sqlvar[0].sqldata) = strlen(metaInfo->info);

	XSQLDA* outSqlDa = reinterpret_cast<XSQLDA*>(new char[(XSQLDA_LENGTH(1))]);
	outSqlDa->version = SQLDA_VERSION1;
	outSqlDa->sqln = 1;
	ThrowError::check(funcDsqlDescribe(statusVector, &stmtHandle, SQL_DIALECT_CURRENT, outSqlDa),
		statusVector);
	outSqlDa->sqlvar[0].sqldata = new char[sizeof(short) + outSqlDa->sqlvar[0].sqllen + 1];
	outSqlDa->sqlvar[0].sqldata[sizeof(short) + outSqlDa->sqlvar[0].sqllen] = '\0';

	ThrowError::check(funcDsqlExecute2(statusVector, &trHandle, &stmtHandle, SQL_DIALECT_CURRENT,
		inSqlDa, outSqlDa), statusVector);
	ThrowError::check(funcDsqlFreeStatement(statusVector, &stmtHandle, DSQL_unprepare), statusVector);

	delete [] inSqlDa->sqlvar[0].sqldata;
	delete [] reinterpret_cast<char*>(inSqlDa);
	inSqlDa = NULL;

	int count = values->getCount();

	char buffer[65536];
	strcpy(buffer, "execute block (\n");

	for (int i = 1; i <= count; ++i)
	{
		if (i > 1)
			strcat(buffer, ",\n");

		Value* val = values->getValue(ThrowError(), i);
		const char* name = val->getName(ThrowError());

		strcat(buffer, "    p");
		sprintf(buffer + strlen(buffer), "%d type of column \"%s\".\"%s\" = ?", i, metaInfo->table, name);
	}

	strcat(buffer,
		")\n"
		"as\n"
		"begin\n"
		"    execute statement ('insert into \"");

	strcat(buffer, metaInfo->table);
	strcat(buffer, "\" (");

	for (int i = 1; i <= count; ++i)
	{
		if (i > 1)
			strcat(buffer, ", ");

		Value* val = values->getValue(ThrowError(), i);
		const char* name = val->getName(ThrowError());

		strcat(buffer, "\"");
		strcat(buffer, name);
		strcat(buffer, "\"");
	}

	strcat(buffer, ") values (");

	for (int i = 1; i <= count; ++i)
	{
		if (i > 1)
			strcat(buffer, ", ");
		strcat(buffer, "?");
	}

	strcat(buffer, ")') (");

	for (int i = 1; i <= count; ++i)
	{
		if (i > 1)
			strcat(buffer, ", ");
		strcat(buffer, ":p");
		sprintf(buffer + strlen(buffer), "%d", i);
	}

	strcat(buffer, ")\n        on external data source '");
	strcat(buffer, outSqlDa->sqlvar[0].sqldata + sizeof(short));
	strcat(buffer, "';\nend");

	ThrowError::check(funcDsqlPrepare(statusVector, &trHandle, &stmtHandle, 0, buffer,
		SQL_DIALECT_CURRENT, NULL), statusVector);

	inSqlDa = reinterpret_cast<XSQLDA*>(new char[(XSQLDA_LENGTH(count))]);
	inSqlDa->version = SQLDA_VERSION1;
	inSqlDa->sqln = count;
	ThrowError::check(funcDsqlDescribeBind(statusVector, &stmtHandle, SQL_DIALECT_CURRENT, inSqlDa),
		statusVector);

	for (int i = 0; i < count; ++i)
	{
		XSQLVAR* var = &inSqlDa->sqlvar[i];

		switch (var->sqltype & ~1)
		{
			case SQL_TEXT:
				var->sqltype = SQL_VARYING | (var->sqltype & 1);
				// fall into

			case SQL_VARYING:
				var->sqldata = new char[sizeof(short) + var->sqllen];
				break;

			case SQL_SHORT:
				var->sqldata = new char[sizeof(short)];
				break;

			case SQL_LONG:
				var->sqldata = new char[sizeof(int32)];
				break;

			case SQL_INT64:
				var->sqldata = new char[sizeof(int64)];
				break;

			case SQL_FLOAT:
				var->sqltype = SQL_DOUBLE | (var->sqltype & 1);
				// fall into

			case SQL_DOUBLE:
				var->sqldata = new char[sizeof(double)];
				break;

			case SQL_TYPE_DATE:
				var->sqldata = new char[sizeof(ISC_DATE)];
				break;

			//// TODO: SQL_TIMESTAMP, SQL_TYPE_TIME

			case SQL_BLOB:
				var->sqldata = new char[sizeof(ISC_QUAD)];
				break;

			default:
				assert(false);
		}

		var->sqltype |= 1;
		var->sqlind = new short;
		*reinterpret_cast<short*>(var->sqlind) = -1;
	}

	delete [] outSqlDa->sqlvar[0].sqldata;
	delete [] reinterpret_cast<char*>(outSqlDa);

	initialized = true;
}

FB_UDR_BEGIN_TRIGGER(replicate)
{
	Values* values = newValues;

	initialize(context, values);

	int count = values->getCount();

	ISC_STATUS_ARRAY statusVector = {0};
	isc_db_handle dbHandle = getIscDbHandle(context);
	isc_tr_handle trHandle = getIscTrHandle(context);

	for (int i = 1; i <= count; ++i)
	{
		XSQLVAR* var = &inSqlDa->sqlvar[i - 1];
		Value* val = values->getValue(ThrowError(), i);

		if (val->isNull())
		{
			*var->sqlind = -1;
			continue;
		}
		else
			*var->sqlind = 0;

		switch (var->sqltype & ~1)
		{
			case SQL_VARYING:
			{
				uint len;
				const char* s = val->getString(ThrowError(), &len);
				*reinterpret_cast<unsigned short*>(var->sqldata) = len;
				memcpy(var->sqldata + sizeof(unsigned short), s, len);
				break;
			}

			case SQL_SHORT:
				*reinterpret_cast<short*>(var->sqldata) = (short) val->getInt(
					ThrowError(), var->sqlscale);
				break;

			case SQL_LONG:
				*reinterpret_cast<int32*>(var->sqldata) = val->getInt(ThrowError(), var->sqlscale);
				break;

			case SQL_INT64:
				*reinterpret_cast<int64*>(var->sqldata) = val->getBigInt(ThrowError(), var->sqlscale);
				break;

			case SQL_DOUBLE:
				*reinterpret_cast<double*>(var->sqldata) = val->getDouble(ThrowError());
				break;

			case SQL_BLOB:
			{
				int64 blobId = val->getBlobId(ThrowError());
				ISC_QUAD quad;
				quad.gds_quad_low = ISC_ULONG(blobId);
				quad.gds_quad_high = ISC_ULONG(blobId >> 32);
				*reinterpret_cast<ISC_QUAD*>(var->sqldata) = quad;
				break;
			}

			//// TODO: SQL_TYPE_DATE, SQL_TIMESTAMP, SQL_TYPE_TIME

			default:
				assert(false);
		}
	}

	ThrowError::check(funcDsqlExecute(statusVector, &trHandle, &stmtHandle, SQL_DIALECT_CURRENT,
		inSqlDa), statusVector);
}
FB_UDR_END_TRIGGER(replicate)
