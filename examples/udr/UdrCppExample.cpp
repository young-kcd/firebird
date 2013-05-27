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
#include "firebird/UdrCppEngine.h"
#include <assert.h>
#include <stdio.h>


using namespace Firebird;
using namespace Firebird::Udr;


namespace
{
	template <typename T>
	class Auto
	{
	public:
		Auto<T>(T* aPtr = NULL)
			: ptr(aPtr)
		{
		}

		~Auto()
		{
			clear(ptr);
		}

		Auto<T>& operator =(T* aPtr)
		{
			clear(ptr);
			ptr = aPtr;
			return *this;
		}

		operator T*()
		{
			return ptr;
		}

		operator const T*() const
		{
			return ptr;
		}

		bool operator !() const
		{
			return !ptr;
		}

		bool hasData() const
		{
			return ptr != NULL;
		}

		T* operator ->()
		{
			return ptr;
		}

		T* release()
		{
			T* tmp = ptr;
			ptr = NULL;
			return tmp;
		}

		void reset(T* aPtr = NULL)
		{
			if (aPtr != ptr)
			{
				clear(ptr);
				ptr = aPtr;
			}
		}

	private:
		static void clear(IDisposable* ptr)
		{
			if (ptr)
				ptr->dispose();
		}

		static void clear(IRefCounted* ptr)
		{
			if (ptr)
				ptr->release();
		}

		// not implemented
		Auto<T>(Auto<T>&);
		void operator =(Auto<T>&);

	private:
		T* ptr;
	};
}


//------------------------------------------------------------------------------


/***
create function wait_event (
    event_name varchar(31) character set utf8 not null
) returns integer not null
    external name 'udrcpp_example!wait_event'
    engine udr;
***/
FB_UDR_BEGIN_FUNCTION(wait_event)
	FB_UDR_EXECUTE_MESSAGE_FUNCTION(
		(FB_VARCHAR(31 * 4), name)
	,
		(FB_INTEGER, result))
	{
		char* s = new char[in->name.length + 1];
		memcpy(s, in->name.str, in->name.length);
		s[in->name.length] = '\0';

		unsigned char* eveBuffer;
		unsigned char* eveResult;
		int eveLen = isc_event_block(&eveBuffer, &eveResult, 1, s);

		delete [] s;

		ISC_STATUS_ARRAY statusVector = {0};
		isc_db_handle dbHandle = getIscDbHandle(context);
		ISC_ULONG counter = 0;

		StatusException::check(isc_wait_for_event(statusVector, &dbHandle, eveLen, eveBuffer, eveResult),
			statusVector);
		isc_event_counts(&counter, eveLen, eveBuffer, eveResult);
		StatusException::check(isc_wait_for_event(statusVector, &dbHandle, eveLen, eveBuffer, eveResult),
			statusVector);
		isc_event_counts(&counter, eveLen, eveBuffer, eveResult);

		isc_free((char*) eveBuffer);
		isc_free((char*) eveResult);

		out->resultNull = FB_FALSE;
		out->result = counter;
	}
FB_UDR_END_FUNCTION


/***
create function sum_args (
    n1 integer,
    n2 integer,
    n3 integer
) returns integer
    external name 'udrcpp_example!sum_args'
    engine udr;
***/
FB_UDR_BEGIN_FUNCTION(sum_args)
	// This function requires the INTEGER parameters and return value, otherwise it will crash.
	// Metadata is inspected dynamically (in execute). This is not the fastest method.
	FB_UDR_EXECUTE_DYNAMIC_FUNCTION
	{
		// Get input and output metadata.

		Auto<IMessageMetadata> inMetadata(metadata->getInputMetadata(status));
		StatusException::check(status->get());

		Auto<IMessageMetadata> outMetadata(metadata->getOutputMetadata(status));
		StatusException::check(status->get());

		// Get count of input parameters.
		unsigned inCount = inMetadata->getCount(status);
		StatusException::check(status->get());

		// Get null offset of the return value.

		unsigned outNullOffset = outMetadata->getNullOffset(status, 0);
		StatusException::check(status->get());

		*(ISC_SHORT*) (out + outNullOffset) = FB_FALSE;

		// Get offset of the return value.
		unsigned outOffset = outMetadata->getOffset(status, 0);
		StatusException::check(status->get());

		// Get a reference to the return value.
		ISC_LONG& ret = *(ISC_LONG*) (out + outOffset);

		// By default, the return value is 0.
		///ret = 0;

		for (unsigned i = 0; i < inCount; ++i)
		{
			// Get null offset of the i-th input parameter.
			unsigned nullOffset = inMetadata->getNullOffset(status, i);
			StatusException::check(status->get());

			// If the i-th input parameter is NULL, set the output to NULL and finish.
			if (*(ISC_SHORT*) (in + nullOffset))
			{
				*(ISC_SHORT*) (out + outNullOffset) = FB_TRUE;
				// Important: we should not return without release the metadata objects.
				break;
			}

			// Get the offset of the i-th input parameter.
			unsigned offset = inMetadata->getOffset(status, i);
			StatusException::check(status->get());

			// Read the i-th input parameter value and sum it in the referenced return value.
			ret += *(ISC_LONG*) (in + offset);
		}
	}
FB_UDR_END_FUNCTION


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
FB_UDR_BEGIN_PROCEDURE(gen_rows)
	// Procedure variables.
	unsigned inOffsetStart, inOffsetEnd, outNullOffset, outOffset;

	/*** Procedure destructor.
	~FB_UDR_PROCEDURE(gen_rows)()
	{
	}
	***/

	// Get offsets once per procedure.
	FB_UDR_INITIALIZE
	{
		Auto<IMessageMetadata> inMetadata(metadata->getInputMetadata(status));
		StatusException::check(status->get());

		inOffsetStart = inMetadata->getOffset(status, 0);
		StatusException::check(status->get());

		inOffsetEnd = inMetadata->getOffset(status, 1);
		StatusException::check(status->get());

		Auto<IMessageMetadata> outMetadata(metadata->getOutputMetadata(status));
		StatusException::check(status->get());

		outNullOffset = outMetadata->getNullOffset(status, 0);
		StatusException::check(status->get());

		outOffset = outMetadata->getOffset(status, 0);
		StatusException::check(status->get());
	}

	FB_UDR_EXECUTE_DYNAMIC_PROCEDURE
	{
		counter = *(ISC_LONG*) (in + procedure->inOffsetStart);
		end = *(ISC_LONG*) (in + procedure->inOffsetEnd);

		*(ISC_SHORT*) (out + procedure->outNullOffset) = FB_FALSE;
	}

	FB_UDR_FETCH_PROCEDURE
	{
		if (counter > end)
			return false;

		*(ISC_LONG*) (out + procedure->outOffset) = counter++;

		return true;
	}

	/*** ResultSet destructor.
	~ResultSet()
	{
	}
	***/

	// ResultSet variables.
	ISC_LONG counter;
	ISC_LONG end;
FB_UDR_END_PROCEDURE


/***
create procedure gen_rows2 (
    start_n integer not null,
    end_n integer not null
) returns (
    n integer not null
)
    external name 'udrcpp_example!gen_rows2'
    engine udr;
***/
FB_UDR_BEGIN_PROCEDURE(gen_rows2)
	FB_UDR_EXECUTE_MESSAGE_PROCEDURE(
		(FB_INTEGER, start)
		(FB_INTEGER, end)
	,
		(FB_INTEGER, result))
	{
		out->resultNull = FB_FALSE;
		out->result = in->start - 1;
	}

	FB_UDR_FETCH_PROCEDURE
	{
		return out->result++ < in->end;
	}
FB_UDR_END_PROCEDURE


/***
create procedure inc (
    count_n integer not null
) returns (
    n0 integer not null,
    n1 integer not null,
    n2 integer not null,
    n3 integer not null,
    n4 integer not null
)
    external name 'udrcpp_example!inc'
    engine udr;
***/
// This is a sample procedure demonstrating how the scopes of variables works.
// n1 and n2 are on the Procedure scope, i.e., they're shared for each execution of the same cached
// metadata object.
// n3 and n4 are on the ResultSet scope, i.e., each procedure execution have they own instances.
FB_UDR_BEGIN_PROCEDURE(inc)
	ISC_LONG n1;

	// This is how a procedure (class) initializer is written.
	// ResultSet variables are not accessible here.
	// If there is nothing to initialize, it can be completelly suppressed.
	FB_UDR_PROCEDURE(inc)()
		: n1(0),
		  n2(0)
	{
	}

	ISC_LONG n2;

	// FB_UDR_EXECUTE_MESSAGE_PROCEDURE or FB_UDR_EXECUTE_DYNAMIC_PROCEDURE starts the ResultSet scope.
	FB_UDR_EXECUTE_MESSAGE_PROCEDURE(
		(FB_INTEGER, count)
	,
		(FB_INTEGER, n0)
		(FB_INTEGER, n1)
		(FB_INTEGER, n2)
		(FB_INTEGER, n3)
		(FB_INTEGER, n4))
	// This is the ResultSet (class) initializer. If there is nothing to initialize, the comma
	// should be suppressed.
	,
		n3(procedure->n1),	// n3 will start with the next value for n1 of the last execution
		n4(0)
	{
		out->n0Null = out->n1Null = out->n2Null = out->n3Null = out->n4Null = FB_FALSE;

		out->n0 = 0;

		// In the execute method, the procedure scope must be accessed using the 'procedure' pointer.
		procedure->n1 = 0;

		// We don't touch n2 here, so it incremented counter will be kept after each execution.

		// The ResultSet scope must be accessed directly, i.e., they're member variables of the
		// 'this' pointer.
		++n4;
	}

	ISC_LONG n3;

	// FB_UDR_FETCH must be always after FB_UDR_EXECUTE_MESSAGE_PROCEDURE or
	// FB_UDR_EXECUTE_DYNAMIC_PROCEDURE.
	FB_UDR_FETCH_PROCEDURE
	{
		if (out->n0++ <= in->count)
		{
			out->n1 = ++procedure->n1;
			out->n2 = ++procedure->n2;
			out->n3 = ++n3;
			out->n4 = ++n4;
			return true;
		}

		return false;
	}

	ISC_LONG n4;
FB_UDR_END_PROCEDURE


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

create trigger persons_replicate2
    after insert on persons
    external name 'udrcpp_example!replicate_persons!ds1'
    engine udr;
***/
FB_UDR_BEGIN_TRIGGER(replicate)
	/***
	FB_UDR_TRIGGER(replicate)()
		: initialized(false)
	{
	}

	~FB_UDR_TRIGGER(replicate)()
	{
		if (!initialized)
			return;
	}
	***/

	FB_UDR_EXECUTE_DYNAMIC_TRIGGER
	{
		ITransaction* transaction = context->getTransaction(status);
		StatusException::check(status->get());

		// This will not work if the table has computed fields.
		stmt->execute(status, transaction, triggerMetadata, newFields, NULL, NULL);
		StatusException::check(status->get());
	}

	FB_UDR_INITIALIZE
	{
		ISC_STATUS_ARRAY statusVector = {0};
		isc_db_handle dbHandle = getIscDbHandle(context);
		isc_tr_handle trHandle = getIscTrHandle(context);

		isc_stmt_handle stmtHandle = 0;
		StatusException::check(isc_dsql_allocate_statement(statusVector, &dbHandle, &stmtHandle),
			statusVector);
		StatusException::check(isc_dsql_prepare(statusVector, &trHandle, &stmtHandle, 0,
			"select data_source from replicate_config where name = ?",
			SQL_DIALECT_CURRENT, NULL), statusVector);

		///Auto<IStatus> status(master->getStatus());

		const char* table = metadata->getTriggerTable(status);
		StatusException::check(status->get());

		// Skip the first exclamation point, separating the module name and entry point.
		const char* info = strchr(metadata->getEntryPoint(status), '!');
		StatusException::check(status->get());

		// Skip the second exclamation point, separating the entry point and the misc info (config).
		if (info)
			info = strchr(info + 1, '!');

		if (info)
			++info;
		else
			info = "";

		XSQLDA* inSqlDa = reinterpret_cast<XSQLDA*>(new char[(XSQLDA_LENGTH(1))]);
		inSqlDa->version = SQLDA_VERSION1;
		inSqlDa->sqln = 1;
		StatusException::check(isc_dsql_describe_bind(statusVector, &stmtHandle,
			SQL_DIALECT_CURRENT, inSqlDa), statusVector);
		inSqlDa->sqlvar[0].sqldata = new char[sizeof(short) + inSqlDa->sqlvar[0].sqllen];
		strncpy(inSqlDa->sqlvar[0].sqldata + sizeof(short), info, inSqlDa->sqlvar[0].sqllen);
		*reinterpret_cast<short*>(inSqlDa->sqlvar[0].sqldata) = strlen(info);

		XSQLDA* outSqlDa = reinterpret_cast<XSQLDA*>(new char[(XSQLDA_LENGTH(1))]);
		outSqlDa->version = SQLDA_VERSION1;
		outSqlDa->sqln = 1;
		StatusException::check(isc_dsql_describe(statusVector, &stmtHandle,
			SQL_DIALECT_CURRENT, outSqlDa), statusVector);
		outSqlDa->sqlvar[0].sqldata = new char[sizeof(short) + outSqlDa->sqlvar[0].sqllen + 1];
		outSqlDa->sqlvar[0].sqldata[sizeof(short) + outSqlDa->sqlvar[0].sqllen] = '\0';

		StatusException::check(isc_dsql_execute2(statusVector, &trHandle, &stmtHandle,
			SQL_DIALECT_CURRENT, inSqlDa, outSqlDa), statusVector);
		StatusException::check(isc_dsql_free_statement(statusVector, &stmtHandle, DSQL_unprepare),
			statusVector);

		delete [] inSqlDa->sqlvar[0].sqldata;
		delete [] reinterpret_cast<char*>(inSqlDa);

		triggerMetadata.reset(metadata->getTriggerMetadata(status));
		StatusException::check(status->get());

		unsigned count = triggerMetadata->getCount(status);
		StatusException::check(status->get());

		char buffer[65536];
		strcpy(buffer, "execute block (\n");

		for (unsigned i = 0; i < count; ++i)
		{
			if (i > 0)
				strcat(buffer, ",\n");

			const char* name = triggerMetadata->getField(status, i);
			StatusException::check(status->get());

			strcat(buffer, "    p");
			sprintf(buffer + strlen(buffer), "%d type of column \"%s\".\"%s\" = ?", i, table, name);
		}

		strcat(buffer,
			")\n"
			"as\n"
			"begin\n"
			"    execute statement ('insert into \"");

		strcat(buffer, table);
		strcat(buffer, "\" (");

		for (unsigned i = 0; i < count; ++i)
		{
			if (i > 0)
				strcat(buffer, ", ");

			const char* name = triggerMetadata->getField(status, i);
			StatusException::check(status->get());

			strcat(buffer, "\"");
			strcat(buffer, name);
			strcat(buffer, "\"");
		}

		strcat(buffer, ") values (");

		for (unsigned i = 0; i < count; ++i)
		{
			if (i > 0)
				strcat(buffer, ", ");
			strcat(buffer, "?");
		}

		strcat(buffer, ")') (");

		for (unsigned i = 0; i < count; ++i)
		{
			if (i > 0)
				strcat(buffer, ", ");
			strcat(buffer, ":p");
			sprintf(buffer + strlen(buffer), "%d", i);
		}

		strcat(buffer, ")\n        on external data source '");
		strcat(buffer, outSqlDa->sqlvar[0].sqldata + sizeof(short));
		strcat(buffer, "';\nend");

		IAttachment* attachment = context->getAttachment(status);
		StatusException::check(status->get());

		ITransaction* transaction = context->getTransaction(status);
		StatusException::check(status->get());

		stmt.reset(attachment->prepare(status, transaction, 0, buffer, SQL_DIALECT_CURRENT, 0));

		delete [] outSqlDa->sqlvar[0].sqldata;
		delete [] reinterpret_cast<char*>(outSqlDa);

		///initialized = true;
	}

	///bool initialized;
	Auto<IMessageMetadata> triggerMetadata;
	Auto<IStatement> stmt;
FB_UDR_END_TRIGGER


FB_UDR_BEGIN_TRIGGER(replicate_persons)
	/***
	FB_UDR_TRIGGER(replicate_persons)()
		: initialized(false)
	{
	}

	~FB_UDR_TRIGGER(replicate_persons)()
	{
		if (!initialized)
			return;
	}
	***/

	// Order of fields does not need to match the fields order in the table, but it should match
	// the order of fields in the SQL command constructed in the initialization.
	FB_UDR_EXECUTE_MESSAGE_TRIGGER(
		(FB_INTEGER, id, "ID")
		(FB_BLOB, info, "INFO")
		(FB_VARCHAR(60 * 4), address, "ADDRESS")
		(FB_VARCHAR(60 * 4), name, "NAME")
	)
	{
		ITransaction* transaction = context->getTransaction(status);
		StatusException::check(status->get());

		stmt->execute(status, transaction, triggerMetadata, newFields, NULL, NULL);
		StatusException::check(status->get());
	}

	FB_UDR_INITIALIZE
	{
		ISC_STATUS_ARRAY statusVector = {0};
		isc_db_handle dbHandle = getIscDbHandle(context);
		isc_tr_handle trHandle = getIscTrHandle(context);

		isc_stmt_handle stmtHandle = 0;
		StatusException::check(isc_dsql_allocate_statement(statusVector, &dbHandle, &stmtHandle), statusVector);
		StatusException::check(isc_dsql_prepare(statusVector, &trHandle, &stmtHandle, 0,
			"select data_source from replicate_config where name = ?",
			SQL_DIALECT_CURRENT, NULL), statusVector);

		///Auto<IStatus> status(master->getStatus());

		const char* table = metadata->getTriggerTable(status);
		StatusException::check(status->get());

		// Skip the first exclamation point, separating the module name and entry point.
		const char* info = strchr(metadata->getEntryPoint(status), '!');
		StatusException::check(status->get());

		// Skip the second exclamation point, separating the entry point and the misc info (config).
		if (info)
			info = strchr(info + 1, '!');

		if (info)
			++info;
		else
			info = "";

		XSQLDA* inSqlDa = reinterpret_cast<XSQLDA*>(new char[(XSQLDA_LENGTH(1))]);
		inSqlDa->version = SQLDA_VERSION1;
		inSqlDa->sqln = 1;
		StatusException::check(isc_dsql_describe_bind(statusVector, &stmtHandle, SQL_DIALECT_CURRENT, inSqlDa),
			statusVector);
		inSqlDa->sqlvar[0].sqldata = new char[sizeof(short) + inSqlDa->sqlvar[0].sqllen];
		strncpy(inSqlDa->sqlvar[0].sqldata + sizeof(short), info, inSqlDa->sqlvar[0].sqllen);
		*reinterpret_cast<short*>(inSqlDa->sqlvar[0].sqldata) = strlen(info);

		XSQLDA* outSqlDa = reinterpret_cast<XSQLDA*>(new char[(XSQLDA_LENGTH(1))]);
		outSqlDa->version = SQLDA_VERSION1;
		outSqlDa->sqln = 1;
		StatusException::check(isc_dsql_describe(statusVector, &stmtHandle, SQL_DIALECT_CURRENT, outSqlDa),
			statusVector);
		outSqlDa->sqlvar[0].sqldata = new char[sizeof(short) + outSqlDa->sqlvar[0].sqllen + 1];
		outSqlDa->sqlvar[0].sqldata[sizeof(short) + outSqlDa->sqlvar[0].sqllen] = '\0';

		StatusException::check(isc_dsql_execute2(statusVector, &trHandle, &stmtHandle, SQL_DIALECT_CURRENT,
			inSqlDa, outSqlDa), statusVector);
		StatusException::check(isc_dsql_free_statement(statusVector, &stmtHandle, DSQL_unprepare), statusVector);

		delete [] inSqlDa->sqlvar[0].sqldata;
		delete [] reinterpret_cast<char*>(inSqlDa);

		triggerMetadata.reset(metadata->getTriggerMetadata(status));
		StatusException::check(status->get());

		char buffer[65536];
		strcpy(buffer,
			"execute block (\n"
			"    id type of column PERSONS.ID = ?,\n"
			"    info type of column PERSONS.INFO = ?,\n"
			"    address type of column PERSONS.ADDRESS = ?,\n"
			"    name type of column PERSONS.NAME = ?\n"
			")"
			"as\n"
			"begin\n"
			"    execute statement ('insert into persons (id, name, address, info)\n"
			"        values (?, ?, ?, ?)') (:id, :name, :address, :info)\n"
			"        on external data source '");
		strcat(buffer, outSqlDa->sqlvar[0].sqldata + sizeof(short));
		strcat(buffer, "';\nend");

		IAttachment* attachment = context->getAttachment(status);
		StatusException::check(status->get());

		ITransaction* transaction = context->getTransaction(status);
		StatusException::check(status->get());

		stmt.reset(attachment->prepare(status, transaction, 0, buffer, SQL_DIALECT_CURRENT, 0));

		delete [] outSqlDa->sqlvar[0].sqldata;
		delete [] reinterpret_cast<char*>(outSqlDa);

		///initialized = true;
	}

	///bool initialized;
	Auto<IMessageMetadata> triggerMetadata;
	Auto<IStatement> stmt;
FB_UDR_END_TRIGGER
