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
	class AutoDispose
	{
	public:
		AutoDispose<T>(T* aPtr = NULL)
			: ptr(aPtr)
		{
		}

		~AutoDispose()
		{
			clear();
		}

		AutoDispose<T>& operator =(T* aPtr)
		{
			clear();
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
				clear();
				ptr = aPtr;
			}
		}

	private:
		void clear()
		{
			if (ptr)
				ptr->dispose();
		}

		// not implemented
		AutoDispose<T>(AutoDispose<T>&);
		void operator =(AutoDispose<T>&);

	private:
		T* ptr;
	};
}


static IMaster* master = fb_get_master_interface();


//------------------------------------------------------------------------------


class MessageImpl;

class ParamDescBase
{
public:
	ParamDescBase()
		: pos(0),
		  nullPos(0)
	{
	}

	unsigned pos;
	unsigned nullPos;
};

template <class T>
class ParamDesc : public ParamDescBase
{
};

template <>
class ParamDesc<void*> : public ParamDescBase
{
public:
	ParamDesc(MessageImpl& message, const Firebird::IParametersMetadata* aParams);

	unsigned align(unsigned size, unsigned aIndex)
	{
		index = aIndex;

		AutoDispose<IStatus> status(master->getStatus());

		switch ((type = params->getType(status, index)))
		{
			case SQL_SHORT:
				size = FB_ALIGN(size, sizeof(ISC_SHORT));
				break;

			case SQL_LONG:
				size = FB_ALIGN(size, sizeof(ISC_LONG));
				break;

			case SQL_INT64:
				size = FB_ALIGN(size, sizeof(ISC_INT64));
				break;

			case SQL_FLOAT:
				size = FB_ALIGN(size, sizeof(float));
				break;

			case SQL_DOUBLE:
				size = FB_ALIGN(size, sizeof(double));
				break;

			case SQL_BLOB:
				size = FB_ALIGN(size, sizeof(ISC_QUAD));
				break;

			case SQL_TEXT:
			case SQL_VARYING:
				size = FB_ALIGN(size, sizeof(ISC_USHORT));
				break;

			default:
				assert(false);
				break;
		}

		return size;
	}

	unsigned addBlr(ISC_UCHAR*& blr)
	{
		AutoDispose<IStatus> status(master->getStatus());
		unsigned ret;

		switch (type)
		{
			case SQL_SHORT:
			{
				unsigned scale = params->getScale(status, index);
				*blr++ = blr_short;
				*blr++ = scale;
				ret = sizeof(ISC_SHORT);
				break;
			}

			case SQL_LONG:
			{
				unsigned scale = params->getScale(status, index);
				*blr++ = blr_long;
				*blr++ = scale;
				ret = sizeof(ISC_LONG);
				break;
			}

			case SQL_INT64:
			{
				unsigned scale = params->getScale(status, index);
				*blr++ = blr_int64;
				*blr++ = scale;
				ret = sizeof(ISC_INT64);
				break;
			}

			case SQL_FLOAT:
				*blr++ = blr_float;
				ret = sizeof(float);
				break;

			case SQL_DOUBLE:
				*blr++ = blr_double;
				ret = sizeof(double);
				break;

			case SQL_BLOB:
				*blr++ = blr_blob2;
				*blr++ = 0;
				*blr++ = 0;
				*blr++ = 0;
				*blr++ = 0;
				ret = sizeof(ISC_QUAD);
				break;

			case SQL_TEXT:
			case SQL_VARYING:
			{
				unsigned length = params->getLength(status, index);
				*blr++ = blr_varying;
				*blr++ = length & 0xFF;
				*blr++ = (length >> 8) & 0xFF;
				ret = sizeof(ISC_USHORT) + length;
				break;
			}

			default:
				assert(false);
				ret = 0;
				break;
		}

		return ret;
	}

	unsigned getType() const
	{
		return type;
	}

private:
	const Firebird::IParametersMetadata* params;
	unsigned type;
	unsigned index;
};

class MessageImpl : public Firebird::FbMessage
{
public:
	MessageImpl(unsigned aItemCount, void* aBuffer = NULL)
		: itemCount(aItemCount * 2),
		  freeBuffer(!aBuffer),
		  items(0)
	{
		static const ISC_UCHAR HEADER[] = {
			blr_version5,
			blr_begin,
			blr_message, 0, 0, 0
		};

		blrLength = 0;
		blr = blrPos = new ISC_UCHAR[sizeof(HEADER) + 10 * itemCount + 2];
		bufferLength = 0;
		buffer = static_cast<ISC_UCHAR*>(aBuffer);

		memcpy(blrPos, HEADER, sizeof(HEADER));
		blrPos += sizeof(HEADER);
	}

	~MessageImpl()
	{
		if (freeBuffer && buffer)
			delete [] buffer;

		if (blr)
			delete [] blr;
	}

	template <typename T>
	void add(ParamDesc<T>& paramDesc)
	{
		if (items >= itemCount)
			return;	// return an error, this is already constructed message

		bufferLength = paramDesc.align(bufferLength, items / 2);
		paramDesc.pos = bufferLength;
		bufferLength += paramDesc.addBlr(blrPos);

		bufferLength = FB_ALIGN(bufferLength, sizeof(ISC_SHORT));
		paramDesc.nullPos = bufferLength;
		bufferLength += sizeof(ISC_SHORT);

		*blrPos++ = blr_short;
		*blrPos++ = 0;

		items += 2;

		if (items == itemCount)
		{
			*blrPos++ = blr_end;
			*blrPos++ = blr_eoc;

			blrLength = blrPos - blr;

			ISC_UCHAR* blrStart = blrPos - blrLength;
			blrStart[4] = items & 0xFF;
			blrStart[5] = (items >> 8) & 0xFF;

			if (!buffer)
			{
				buffer = new ISC_UCHAR[bufferLength];
				memset(buffer, 0, bufferLength);
			}
		}
	}

	bool isNull(const ParamDescBase& index)
	{
		return *(ISC_SHORT*) (buffer + index.nullPos);
	}

	void setNull(const ParamDescBase& index, bool null)
	{
		*(ISC_SHORT*) (buffer + index.nullPos) = (ISC_SHORT) null;
	}

	template <typename T> T& operator [](const ParamDesc<T>& index)
	{
		return *(T*) (buffer + index.pos);
	}

	void* operator [](const ParamDesc<void*>& index)
	{
		return buffer + index.pos;
	}

public:
	unsigned itemCount;
	bool freeBuffer;
	unsigned items;
	ISC_UCHAR* blrPos;
};

template <>
class ParamDesc<ISC_SHORT> : public ParamDescBase
{
public:
	ParamDesc(MessageImpl& message, ISC_UCHAR aScale = 0)
		: scale(aScale)
	{
		message.add(*this);
	}

	unsigned align(unsigned size, unsigned /*index*/)
	{
		return FB_ALIGN(size, sizeof(ISC_SHORT));
	}

	unsigned addBlr(ISC_UCHAR*& blr)
	{
		*blr++ = blr_short;
		*blr++ = scale;
		return sizeof(ISC_SHORT);
	}

private:
	ISC_UCHAR scale;
};

template <>
class ParamDesc<ISC_LONG> : public ParamDescBase
{
public:
	ParamDesc(MessageImpl& message, ISC_UCHAR aScale = 0)
		: scale(aScale)
	{
		message.add(*this);
	}

	unsigned align(unsigned size, unsigned /*index*/)
	{
		return FB_ALIGN(size, sizeof(ISC_LONG));
	}

	unsigned addBlr(ISC_UCHAR*& blr)
	{
		*blr++ = blr_long;
		*blr++ = scale;
		return sizeof(ISC_LONG);
	}

private:
	ISC_UCHAR scale;
};

template <>
class ParamDesc<ISC_INT64> : public ParamDescBase
{
public:
	ParamDesc(MessageImpl& message, ISC_UCHAR aScale = 0)
		: scale(aScale)
	{
		message.add(*this);
	}

	unsigned align(unsigned size, unsigned /*index*/)
	{
		return FB_ALIGN(size, sizeof(ISC_INT64));
	}

	unsigned addBlr(ISC_UCHAR*& blr)
	{
		*blr++ = blr_int64;
		*blr++ = scale;
		return sizeof(ISC_INT64);
	}

private:
	ISC_UCHAR scale;
};

template <>
class ParamDesc<float> : public ParamDescBase
{
public:
	ParamDesc(MessageImpl& message)
	{
		message.add(*this);
	}

	unsigned align(unsigned size, unsigned /*index*/)
	{
		return FB_ALIGN(size, sizeof(float));
	}

	unsigned addBlr(ISC_UCHAR*& blr)
	{
		*blr++ = blr_float;
		return sizeof(float);
	}
};

template <>
class ParamDesc<double> : public ParamDescBase
{
public:
	ParamDesc(MessageImpl& message)
	{
		message.add(*this);
	}

	unsigned align(unsigned size, unsigned /*index*/)
	{
		return FB_ALIGN(size, sizeof(double));
	}

	unsigned addBlr(ISC_UCHAR*& blr)
	{
		*blr++ = blr_double;
		return sizeof(double);
	}
};

template <>
class ParamDesc<ISC_QUAD> : public ParamDescBase
{
public:
	ParamDesc(MessageImpl& message)
	{
		message.add(*this);
	}

	unsigned align(unsigned size, unsigned /*index*/)
	{
		return FB_ALIGN(size, sizeof(ISC_QUAD));
	}

	unsigned addBlr(ISC_UCHAR*& blr)
	{
		*blr++ = blr_blob2;
		*blr++ = 0;
		*blr++ = 0;
		*blr++ = 0;
		*blr++ = 0;
		return sizeof(ISC_QUAD);
	}
};

struct FbString
{
	ISC_USHORT length;
	char str[1];
};

template <>
class ParamDesc<FbString> : public ParamDescBase
{
public:
	ParamDesc(MessageImpl& message, ISC_USHORT aLength)
		: length(aLength)
	{
		message.add(*this);
	}

	unsigned align(unsigned size, unsigned /*index*/)
	{
		return FB_ALIGN(size, sizeof(ISC_USHORT));
	}

	unsigned addBlr(ISC_UCHAR*& blr)
	{
		*blr++ = blr_varying;
		*blr++ = length & 0xFF;
		*blr++ = (length >> 8) & 0xFF;
		return sizeof(ISC_USHORT) + length;
	}

private:
	ISC_USHORT length;
};

//// TODO: boolean, date, time, timestamp

//--------------------------------------

inline ParamDesc<void*>::ParamDesc(MessageImpl& message, const Firebird::IParametersMetadata* aParams)
	: params(aParams),
	  type(0)
{
	message.add(*this);
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

		ThrowError::check(isc_wait_for_event(statusVector, &dbHandle, eveLen, eveBuffer, eveResult),
			statusVector);
		isc_event_counts(&counter, eveLen, eveBuffer, eveResult);
		ThrowError::check(isc_wait_for_event(statusVector, &dbHandle, eveLen, eveBuffer, eveResult),
			statusVector);
		isc_event_counts(&counter, eveLen, eveBuffer, eveResult);

		isc_free((char*) eveBuffer);
		isc_free((char*) eveResult);

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
	FB_UDR_EXECUTE_DYNAMIC_FUNCTION
	{
		AutoDispose<IStatus> status(master->getStatus());

		const IParametersMetadata* params = metadata->getInputParameters(status);
		ThrowError::check(status->get());

		unsigned count = params->getCount(status);
		ThrowError::check(status->get());

		MessageImpl inMessage(count, in);

		MessageImpl outMessage(1, out);
		ParamDesc<ISC_LONG> retDesc(outMessage);

		int ret = 0;

		for (unsigned i = 0; i < count; ++i)
		{
			ParamDesc<ISC_LONG> numDesc(inMessage);

			if (inMessage.isNull(numDesc))
			{
				outMessage.setNull(retDesc, true);
				return;
			}
			else
				ret += inMessage[numDesc];
		}

		outMessage[retDesc] = ret;
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
	FB_UDR_EXECUTE_DYNAMIC_PROCEDURE
	{
		MessageImpl inMessage(2, inMsg);
		ParamDesc<ISC_LONG> startDesc(inMessage);
		ParamDesc<ISC_LONG> endDesc(inMessage);

		counter = inMessage[startDesc];
		end = inMessage[endDesc];
	}

	FB_UDR_FETCH_PROCEDURE
	{
		if (counter > end)
			return false;

		MessageImpl outMessage(1, out);
		ParamDesc<ISC_LONG> retDesc(outMessage);

		outMessage[retDesc] = counter++;

		return true;
	}

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


//// TODO: Rework triggers.
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
FB_UDR_BEGIN_TRIGGER(replicate)
	FB_UDR_TRIGGER(replicate)()
		: initialized(false)
	{
	}

	~FB_UDR_TRIGGER(replicate)()
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
		isc_dsql_free_statement(statusVector, &stmtHandle, DSQL_drop);
	}

	FB_UDR_EXECUTE_DYNAMIC_TRIGGER
	{
		AutoDispose<IStatus> status(master->getStatus());

		const IParametersMetadata* fields = metadata->getTriggerFields(status);
		ThrowError::check(status->get());

		unsigned fieldsCount = fields->getCount(status);
		ThrowError::check(status->get());

		MessageImpl message(fieldsCount, newMsg);

		ISC_STATUS_ARRAY statusVector = {0};
		isc_db_handle dbHandle = getIscDbHandle(context);
		isc_tr_handle trHandle = getIscTrHandle(context);

		for (unsigned i = 1; i <= fieldsCount; ++i)
		{
			ParamDesc<void*> field(message, fields);

			XSQLVAR* var = &inSqlDa->sqlvar[i - 1];

			if (message.isNull(field))
				*var->sqlind = -1;
			else
			{
				*var->sqlind = 0;
				memcpy(var->sqldata, message[field], var->sqllen);
			}
		}

		ThrowError::check(isc_dsql_execute(statusVector, &trHandle, &stmtHandle, SQL_DIALECT_CURRENT,
			inSqlDa), statusVector);
	}

	FB_UDR_INITIALIZE
	{
		ISC_STATUS_ARRAY statusVector = {0};
		isc_db_handle dbHandle = getIscDbHandle(context);
		isc_tr_handle trHandle = getIscTrHandle(context);

		stmtHandle = 0;
		ThrowError::check(isc_dsql_allocate_statement(statusVector, &dbHandle, &stmtHandle), statusVector);
		ThrowError::check(isc_dsql_prepare(statusVector, &trHandle, &stmtHandle, 0,
			"select data_source from replicate_config where name = ?",
			SQL_DIALECT_CURRENT, NULL), statusVector);

		AutoDispose<IStatus> status(master->getStatus());

		const char* table = metadata->getTriggerTable(status);
		ThrowError::check(status->get());

		// Skip the first exclamation point, separing the module name and entry point.
		const char* info = strchr(metadata->getEntryPoint(status), '!');
		ThrowError::check(status->get());

		// Skip the second exclamation point, separing the entry point and the misc info (config).
		if (info)
			info = strchr(info + 1, '!');

		if (info)
			++info;
		else
			info = "";

		inSqlDa = reinterpret_cast<XSQLDA*>(new char[(XSQLDA_LENGTH(1))]);
		inSqlDa->version = SQLDA_VERSION1;
		inSqlDa->sqln = 1;
		ThrowError::check(isc_dsql_describe_bind(statusVector, &stmtHandle, SQL_DIALECT_CURRENT, inSqlDa),
			statusVector);
		inSqlDa->sqlvar[0].sqldata = new char[sizeof(short) + inSqlDa->sqlvar[0].sqllen];
		strncpy(inSqlDa->sqlvar[0].sqldata + sizeof(short), info, inSqlDa->sqlvar[0].sqllen);
		*reinterpret_cast<short*>(inSqlDa->sqlvar[0].sqldata) = strlen(info);

		XSQLDA* outSqlDa = reinterpret_cast<XSQLDA*>(new char[(XSQLDA_LENGTH(1))]);
		outSqlDa->version = SQLDA_VERSION1;
		outSqlDa->sqln = 1;
		ThrowError::check(isc_dsql_describe(statusVector, &stmtHandle, SQL_DIALECT_CURRENT, outSqlDa),
			statusVector);
		outSqlDa->sqlvar[0].sqldata = new char[sizeof(short) + outSqlDa->sqlvar[0].sqllen + 1];
		outSqlDa->sqlvar[0].sqldata[sizeof(short) + outSqlDa->sqlvar[0].sqllen] = '\0';

		ThrowError::check(isc_dsql_execute2(statusVector, &trHandle, &stmtHandle, SQL_DIALECT_CURRENT,
			inSqlDa, outSqlDa), statusVector);
		ThrowError::check(isc_dsql_free_statement(statusVector, &stmtHandle, DSQL_unprepare), statusVector);

		delete [] inSqlDa->sqlvar[0].sqldata;
		delete [] reinterpret_cast<char*>(inSqlDa);
		inSqlDa = NULL;

		const IParametersMetadata* fields = metadata->getTriggerFields(status);
		ThrowError::check(status->get());

		unsigned count = fields->getCount(status);
		ThrowError::check(status->get());

		char buffer[65536];
		strcpy(buffer, "execute block (\n");

		for (unsigned i = 0; i < count; ++i)
		{
			if (i > 0)
				strcat(buffer, ",\n");

			const char* name = fields->getField(status, i);
			ThrowError::check(status->get());

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

			const char* name = fields->getField(status, i);
			ThrowError::check(status->get());

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

		ThrowError::check(isc_dsql_prepare(statusVector, &trHandle, &stmtHandle, 0, buffer,
			SQL_DIALECT_CURRENT, NULL), statusVector);

		inSqlDa = reinterpret_cast<XSQLDA*>(new char[(XSQLDA_LENGTH(count))]);
		inSqlDa->version = SQLDA_VERSION1;
		inSqlDa->sqln = count;
		ThrowError::check(isc_dsql_describe_bind(statusVector, &stmtHandle, SQL_DIALECT_CURRENT, inSqlDa),
			statusVector);

		for (unsigned i = 0; i < count; ++i)
		{
			XSQLVAR* var = &inSqlDa->sqlvar[i];

			switch (var->sqltype & ~1)
			{
				case SQL_TEXT:
					var->sqldata = new char[var->sqllen];
					break;

				case SQL_VARYING:
					var->sqldata = new char[var->sqllen];
					var->sqllen += sizeof(short);
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
					var->sqllen = sizeof(double);
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

	bool initialized;
	XSQLDA* inSqlDa;
	isc_stmt_handle stmtHandle;
FB_UDR_END_TRIGGER
