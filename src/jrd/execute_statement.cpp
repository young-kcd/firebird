/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		execute_statement.cpp
 *	DESCRIPTION:	Dynamic SQL statements execution
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
 *  The Original Code was created by Alexander Peshkoff
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2003 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "fb_types.h"
#include "gen/iberror.h"

#include "../jrd/common.h"
#include <string.h>
#include <math.h>
#include "../jrd/jrd.h"
#include "../jrd/tra.h"
#include "../jrd/dsc.h"
#include "../jrd/y_handle.h"
#include "../jrd/thd.h"
#include "../jrd/err_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thread_proto.h"
#define	WHY_NO_API 
#include "../jrd/why_proto.h"

#include "../jrd/execute_statement.h"

using namespace Jrd;

WHY_DBB GetWhyAttachment(ISC_STATUS* status,
						  Attachment* jrd_attachment_handle);

static const struct {
	SSHORT SqlType;
	SSHORT DataLength;
} DscType2SqlType[] =
{
/* dtype_unknown		*/ {-1, 0},
/* dtype_text		*/ {SQL_TEXT, 0},
/* dtype_cstring	*/ {-1, 0},
/* dtype_varying	*/ {SQL_VARYING, 0},
/* dtype_none1		*/ {-1, 0},
/* dtype_none2		*/ {-1, 0},
/* dtype_packed		*/ {-1, 0},
/* dtype_byte		*/ {-1, 0},
/* dtype_short		*/ {SQL_SHORT, sizeof(short)},
/* dtype_long		*/ {SQL_LONG, sizeof(long)},
/* dtype_quad		*/ {SQL_QUAD, 
#ifdef NATIVE_QUAD
		sizeof(SQUAD)},
#else
		sizeof(SLONG) * 2},
#endif
/* dtype_real		*/ {SQL_FLOAT, sizeof(float)},
/* dtype_double		*/ {SQL_DOUBLE, sizeof(double)},
/* dtype_d_float	*/ {-1, 0}, // Fix to use in VMS
/* dtype_sql_date	*/ {SQL_TYPE_DATE, sizeof(SLONG)},
/* dtype_sql_time	*/ {SQL_TYPE_TIME, sizeof(SLONG)},
/* dtype_timestamp	*/ {SQL_TIMESTAMP, sizeof(SLONG) * 2},
/* dtype_blob		*/ {SQL_BLOB, 0}, 
/* dtype_array		*/ {SQL_ARRAY, -1}, // Not supported for a while
/* dtype_int64		*/ {SQL_INT64, sizeof(SINT64)},
};

void ExecuteStatement::Open(thread_db* tdbb, jrd_nod* sql, SSHORT nVars, bool SingleTon)
{
	SET_TDBB(tdbb);
	if (tdbb->tdbb_transaction->tra_callback_count >= MAX_CALLBACKS) {
		tdbb->tdbb_status_vector[0] = isc_arg_gds;
		tdbb->tdbb_status_vector[1] = isc_exec_sql_max_call_exceeded;
		tdbb->tdbb_status_vector[2] = isc_arg_end;
		ERR_punt();
	}

	Sqlda = 0;
	Transaction = 0;
	Buffer = 0;
	SingleMode = SingleTon;

	fb_assert(tdbb->tdbb_transaction->tra_pool);
	Firebird::string SqlText;
	getString(SqlText, EVL_expr(tdbb, sql), tdbb->tdbb_request);
	memcpy(StartOfSqlOperator, SqlText.c_str(), sizeof(StartOfSqlOperator) - 1);
	StartOfSqlOperator[sizeof(StartOfSqlOperator) - 1] = 0;

	WHY_DBB temp_dbb = GetWhyAttachment(tdbb->tdbb_status_vector,
								  tdbb->tdbb_attachment);
	if (!temp_dbb)
		ERR_punt();

	Attachment = temp_dbb->public_handle;

	WHY_TRA temp_tra = WHY_alloc_handle(temp_dbb->implementation, HANDLE_transaction);
	if (!temp_tra)
		ERR_post(isc_virmemexh, 0);
	Transaction = temp_tra->public_handle;
	temp_tra->handle.h_tra = tdbb->tdbb_transaction;
	temp_tra->parent = temp_dbb;

	Statement = 0;
	Sqlda = MakeSqlda(tdbb, nVars ? nVars : 1);
    Sqlda->sqln = nVars;
    Sqlda->version = 1;

	tdbb->tdbb_transaction->tra_callback_count++;
	THREAD_EXIT();

	// For normal diagnostic
	const int max_diag_len = 50;

	// this check uses local error handler for local status vector
	ISC_STATUS_ARRAY local;
	ISC_STATUS *status = local;
	memset(local, 0, sizeof(local));
#	define Chk(x) if ((x) != 0) goto err_handler

	Chk(isc_dsql_allocate_statement(status, &Attachment, &Statement));

	Chk(isc_dsql_prepare(status, &Transaction, &Statement,
		SqlText.length(), SqlText.c_str(), SQL_DIALECT_CURRENT, Sqlda));
    if (! Sqlda->sqld) {  // Non-select statement - reject for a while
		/*Chk(isc_dsql_execute(status, &Transaction,
               &Statement, SQLDA_VERSION1, 0)); */
		Chk(isc_dsql_free_statement(status, &Statement, DSQL_drop));
		Statement = 0;
		status[0] = isc_arg_gds;
		status[1] = isc_exec_sql_invalid_req;
		status[2] = isc_arg_string;
		status[3] = (ISC_STATUS)(U_IPTR) ERR_cstring(StartOfSqlOperator);
		status[4] = isc_arg_end;
		Chk(status[1]);
	}
	else {
		Chk(ReMakeSqlda(status, tdbb));
		Chk(isc_dsql_describe(status, &Statement,
                        SQLDA_VERSION1, Sqlda));
		Buffer = 0;		// Buffer is used in ParseSqlda
						// First dummy parse - to define buffer size
		Buffer = FB_NEW(*tdbb->tdbb_transaction->tra_pool) 
			SCHAR[XSQLDA_LENGTH(ParseSqlda())];
		ParseSqlda();
		Chk(isc_dsql_execute(status, &Transaction,
                &Statement, SQLDA_VERSION1, 0));
	}

#	undef Chk
err_handler:
	THREAD_ENTER();
	tdbb->tdbb_transaction->tra_callback_count--;
	if (status[0] == 1 && status[1]) {
		memcpy(tdbb->tdbb_status_vector, status, sizeof(local));
		Firebird::status_exception::raise(tdbb->tdbb_status_vector);
	}
}

bool ExecuteStatement::Fetch(thread_db* tdbb, jrd_nod** JrdVar)
{
	// If already bugged - we should never get here
	fb_assert(! (tdbb->tdbb_status_vector[0] == 1 && 
			  tdbb->tdbb_status_vector[1] != 0));
	if (! Statement)
		return false;

	ISC_STATUS_ARRAY local;
	ISC_STATUS *status = local;
	memset(local, 0, sizeof(local));
	status = local;
	tdbb->tdbb_transaction->tra_callback_count++;
	THREAD_EXIT();
    if (isc_dsql_fetch(status, &Statement,
                SQLDA_VERSION1, Sqlda) == 100)
	{
		isc_dsql_free_statement(status, &Statement, DSQL_drop);
		THREAD_ENTER();
		tdbb->tdbb_transaction->tra_callback_count--;
		Statement = 0;
		return false;
    }
	THREAD_ENTER();
	tdbb->tdbb_transaction->tra_callback_count--;
	if (status[0] == 1 && status[1]) {
		memcpy(tdbb->tdbb_status_vector, status, sizeof(local));
		Firebird::status_exception::raise(tdbb->tdbb_status_vector);
	}

	const XSQLVAR *var = Sqlda->sqlvar;
	for (int i = 0; i < Sqlda->sqld; i++, var++, JrdVar++) {
		dsc* d = EVL_assign_to(tdbb, *JrdVar);
		if (d->dsc_dtype >= 
			  sizeof(DscType2SqlType) / sizeof(DscType2SqlType[0]))
		{
rec_err:
			tdbb->tdbb_status_vector[0] = isc_arg_gds;
			tdbb->tdbb_status_vector[1] = isc_exec_sql_invalid_var;
			tdbb->tdbb_status_vector[2] = isc_arg_number;
			tdbb->tdbb_status_vector[3] = i;
			tdbb->tdbb_status_vector[4] = isc_arg_string;
			tdbb->tdbb_status_vector[5] = 
				(ISC_STATUS)(U_IPTR) ERR_cstring(StartOfSqlOperator);
			tdbb->tdbb_status_vector[6] = isc_arg_end;
			Firebird::status_exception::raise(tdbb->tdbb_status_vector);
		}

		if (DscType2SqlType[d->dsc_dtype].SqlType < 0)
			goto rec_err;
		if (!((d->dsc_dtype == dtype_quad || d->dsc_dtype == dtype_blob) &&
			((var->sqltype & ~1) == SQL_QUAD || (var->sqltype & ~1) == SQL_BLOB)))
		{
			if ((var->sqltype & ~1) != DscType2SqlType[d->dsc_dtype].SqlType) {
				goto rec_err;
			}
		}
		if ((var->sqltype & 1) && (*var->sqlind < 0)) {
			d->dsc_flags |= DSC_null;
			continue;
		}
		d->dsc_flags &= ~DSC_null;
		SSHORT length = DscType2SqlType[d->dsc_dtype].DataLength;
		if (! length)
			length = d->dsc_length;
		memcpy(d->dsc_address, var->sqldata, length);
		if (d->dsc_scale != var->sqlscale) {
			double DeltaPow = pow(10, var->sqlscale - d->dsc_scale);
// To avoid compiler warning don't use the *= operator and cast the right side
#			define ReScaleLike(t) *((t *)d->dsc_address) = static_cast<t>(*((t *)d->dsc_address) * DeltaPow)
			switch (d->dsc_dtype) {
			case dtype_short:
				ReScaleLike(SSHORT);
				break;
			case dtype_long: 
				ReScaleLike(SLONG);
				break;
			case dtype_int64:
				ReScaleLike(ISC_INT64);
				break;
			}
#			undef ReScaleLike
		}
    }

	if (SingleMode) {
		tdbb->tdbb_transaction->tra_callback_count++;
		THREAD_EXIT();
		if (isc_dsql_fetch(status, &Statement,
                SQLDA_VERSION1, Sqlda) == 100)
		{
			isc_dsql_free_statement(status, &Statement, DSQL_drop);
			THREAD_ENTER();
			tdbb->tdbb_transaction->tra_callback_count--;
			Statement = 0;
			return false;
		}
		THREAD_ENTER();
		tdbb->tdbb_transaction->tra_callback_count--;
		if (! (status[0] == 1 && status[1])) {
			status[0] = isc_arg_gds;
			status[1] = isc_sing_select_err;
			status[2] = isc_arg_end;
		}
		memcpy(tdbb->tdbb_status_vector, status, sizeof(local));
		Firebird::status_exception::raise(tdbb->tdbb_status_vector);
	}
    return true;
}

void ExecuteStatement::Close(thread_db* tdbb)
{
	if (Statement) {
		tdbb->tdbb_transaction->tra_callback_count++;
		THREAD_EXIT();
		// for a while don't check for errors while freeing statement
		isc_dsql_free_statement(0, &Statement, DSQL_drop);
		THREAD_ENTER();
		tdbb->tdbb_transaction->tra_callback_count--;
		Statement = 0;
	}
	char* p = reinterpret_cast<char*>(Sqlda);
	delete[] p;
	Sqlda = 0;
	if (Transaction) {
		WHY_cleanup_transaction(WHY_translate_handle(Transaction));
	}
	WHY_free_handle(Transaction);
	Transaction = 0;
	delete[] Buffer;
	Buffer = 0;
}

XSQLDA* ExecuteStatement::MakeSqlda(thread_db* tdbb, short n)
{
	return (XSQLDA *)
		(FB_NEW(*tdbb->tdbb_transaction->tra_pool) char[XSQLDA_LENGTH(n)]);
}

ISC_STATUS ExecuteStatement::ReMakeSqlda(ISC_STATUS *vector, thread_db* tdbb)
{
	if (Sqlda->sqln != Sqlda->sqld) {
		vector[0] = isc_arg_gds;
		vector[1] = isc_wronumarg;
		vector[2] = isc_arg_end;
	}
	return vector[1];
}

ULONG ExecuteStatement::ParseSqlda(void)
{
    ULONG offset = 0;
    int i = 0;
    for (XSQLVAR* var = Sqlda->sqlvar;
                        i < Sqlda->sqld; var++, i++)
	{
        USHORT length = var->sqllen;
        const int type = var->sqltype & (~1);
        if (type == SQL_VARYING)
            length += sizeof (SSHORT);
        var->sqldata = &Buffer[offset];
        offset += length;
        var->sqlind = (short*) (&Buffer[offset]);
        offset += sizeof (short);
    }
    return offset;
}

void ExecuteStatement::getString(Firebird::string& s, const dsc* d, const jrd_req* r)
{
	char buffer[BUFFER_LARGE + sizeof(vary)];
	vary* v = reinterpret_cast<vary*>(buffer);
	v->vary_length = BUFFER_LARGE;

	UCHAR* p = 0;
	const SSHORT l = (d && !(r->req_flags & req_null)) ?
		MOV_get_string(d, &p, v, BUFFER_LARGE) : 0; // !!! How call Msgs ?
	if (! p) {
		ERR_post(isc_exec_sql_invalid_arg, 0);
	}

	s.assign((const char*)p, l);
}
