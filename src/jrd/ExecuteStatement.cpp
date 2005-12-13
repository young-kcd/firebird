/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		ExecuteStatement.cpp
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
#include "gen/codes.h"

#include "../jrd/common.h"
#include <string.h>
#include <math.h>
#include "../jrd/jrd.h"
#include "../jrd/tra.h"
#include "../jrd/dsc.h"
#include "../jrd/y_handle.h"
#include "../jrd/thd_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/sch_proto.h"
#define	WHY_NO_API 
#include "../jrd/why_proto.h"

#include "../jrd/ExecuteStatement.h"

extern "C" WHY_DBB GetWhyAttachment(ISC_STATUS* status,
						  class att* jrd_attachment_handle);

static struct {
	SSHORT SqlType;
	SSHORT DataLength;
} DscType2SqlType[] = {
/* dtype_null		*/ {-1, 0},
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
/* dtype_blob		*/ {SQL_BLOB, 
#ifdef NATIVE_QUAD
		sizeof(SQUAD)},
#else
		sizeof(SLONG) * 2},
#endif
/* dtype_array		*/ {SQL_ARRAY, -1}, // Not supported for a while
/* dtype_int64		*/ {SQL_INT64, sizeof(SINT64)},
};

void ExecuteStatement::Open(TDBB tdbb, JRD_NOD sql, SSHORT nVars, bool SingleTon) {
	SET_TDBB(tdbb);
	Sqlda = 0;
	Transaction = 0;
	Buffer = 0;
	StartOfSqlOperator = 0;
	SingleMode = SingleTon;

	assert(tdbb->tdbb_transaction->tra_pool);
	AutoPtr<str> p(getString(tdbb->tdbb_transaction->tra_pool, 
		EVL_expr(tdbb, sql), tdbb->tdbb_request));
	if (tdbb->tdbb_transaction->tra_callback_count >= MAX_CALLBACKS) {
		tdbb->tdbb_status_vector[0] = gds_arg_gds;
		tdbb->tdbb_status_vector[1] = gds_exec_sql_max_call_exceeded;
		tdbb->tdbb_status_vector[2] = gds_arg_end;
		ERR_punt();
	}

	Attachment = GetWhyAttachment(tdbb->tdbb_status_vector,
								  tdbb->tdbb_attachment);
	if (! Attachment)
		ERR_punt();

	Transaction = FB_NEW(*tdbb->tdbb_transaction->tra_pool) why_hndl;
	memset (Transaction, 0, sizeof(why_hndl));
	Transaction->implementation = Attachment->implementation;
	Transaction->handle.h_tra = tdbb->tdbb_transaction;
	Transaction->type = HANDLE_transaction;
	Transaction->parent = Attachment;

	Statement = 0;
	Sqlda = MakeSqlda(tdbb, nVars ? nVars : 1);
    Sqlda->sqln = nVars;
    Sqlda->version = 1;

	tdbb->tdbb_transaction->tra_callback_count++;
	THREAD_EXIT;

	// For normal diagnostic
	const int max_diag_len = 50;
	int diag_len = p->str_length;
	if (diag_len > max_diag_len)
		diag_len = max_diag_len;
	StartOfSqlOperator = FB_NEW(*tdbb->tdbb_transaction->tra_pool) TEXT[diag_len + 1];
	StartOfSqlOperator[0] = 0;
	strncat(StartOfSqlOperator, (TEXT*)(p->str_data), diag_len);

	// this check uses local error handler for local status vector
	ISC_STATUS_ARRAY local;
	ISC_STATUS *status = local;
	memset(local, 0, sizeof(local));
#	define Chk(x) if ((x) != 0) goto err_handler

	Chk(isc_dsql_allocate_statement(status, &Attachment, &Statement));

	Chk(isc_dsql_prepare(status, &Transaction, &Statement,
			(USHORT)(p->str_length), (TEXT*)(p->str_data), 
			SQL_DIALECT_CURRENT, Sqlda));
    if (! Sqlda->sqld) {  // Non-select statement - reject for a while
		/*Chk(isc_dsql_execute(status, &Transaction,
               &Statement, SQLDA_VERSION1, 0)); */
		Chk(isc_dsql_free_statement(status, &Statement, DSQL_drop));
		Statement = 0;
		status[0] = gds_arg_gds;
		status[1] = gds_exec_sql_invalid_req;
		status[2] = gds_arg_string;
		status[3] = reinterpret_cast<ISC_STATUS>(ERR_cstring(StartOfSqlOperator));
		status[4] = gds_arg_end;
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
	THREAD_ENTER;
	tdbb->tdbb_transaction->tra_callback_count--;
	if (status[0] == 1 && status[1]) {
		memcpy(tdbb->tdbb_status_vector, status, sizeof(local));
		Firebird::status_exception::raise(status[1]);
	}
}

bool ExecuteStatement::Fetch(TDBB tdbb, JRD_NOD * JrdVar) {
	// If already bugged - we should never get here
	assert(! (tdbb->tdbb_status_vector[0] == 1 && 
			  tdbb->tdbb_status_vector[1] != 0));
	if (! Statement)
		return false;

	ISC_STATUS_ARRAY local;
	ISC_STATUS *status = local;
	memset(local, 0, sizeof(local));
	status = local;
	tdbb->tdbb_transaction->tra_callback_count++;
	THREAD_EXIT;
    if (isc_dsql_fetch(status, &Statement,
                SQLDA_VERSION1, Sqlda) == 100) {
		isc_dsql_free_statement(status, &Statement, DSQL_drop);
		THREAD_ENTER;
		tdbb->tdbb_transaction->tra_callback_count--;
		Statement = 0;
		return false;
    }
	THREAD_ENTER;
	tdbb->tdbb_transaction->tra_callback_count--;
	if (status[0] == 1 && status[1]) {
		memcpy(tdbb->tdbb_status_vector, status, sizeof(local));
		Firebird::status_exception::raise(status[1]);
	}

	XSQLVAR *var=Sqlda->sqlvar;
    for (int i=0; i < Sqlda->sqld; i++, var++, JrdVar++) {
        DSC * d = EVL_assign_to(tdbb, *JrdVar);
		if (d->dsc_dtype >= 
			  sizeof(DscType2SqlType) / sizeof(DscType2SqlType[0])) {
rec_err:
			tdbb->tdbb_status_vector[0] = gds_arg_gds;
			tdbb->tdbb_status_vector[1] = gds_exec_sql_invalid_var;
			tdbb->tdbb_status_vector[2] = gds_arg_number;
			tdbb->tdbb_status_vector[3] = i;
			tdbb->tdbb_status_vector[4] = gds_arg_string;
			tdbb->tdbb_status_vector[5] = 
				reinterpret_cast<ISC_STATUS>(ERR_cstring(StartOfSqlOperator));
			tdbb->tdbb_status_vector[6] = gds_arg_end;
			Firebird::status_exception::raise(status[1]);
		}
		if (DscType2SqlType[d->dsc_dtype].SqlType < 0)
			goto rec_err;

		// Added to quickly fix problems with returning BLOB's from the statement
		if (!((d->dsc_dtype == dtype_quad || d->dsc_dtype == dtype_blob) && 
			  ((var->sqltype & ~1) == SQL_QUAD || (var->sqltype & ~1) == SQL_BLOB)))
			if ((var->sqltype & ~1) != DscType2SqlType[d->dsc_dtype].SqlType)
				goto rec_err;
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
#			define ReScaleLike(t) *((t *)d->dsc_address) *= DeltaPow
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
		THREAD_EXIT;
		if (isc_dsql_fetch(status, &Statement,
                SQLDA_VERSION1, Sqlda) == 100) {
			isc_dsql_free_statement(status, &Statement, DSQL_drop);
			THREAD_ENTER;
			tdbb->tdbb_transaction->tra_callback_count--;
			Statement = 0;
			return false;
		}
		THREAD_ENTER;
		tdbb->tdbb_transaction->tra_callback_count--;
		if (! (status[0] == 1 && status[1])) {
			status[0] = gds_arg_gds;
			status[1] = gds_sing_select_err;
			status[2] = gds_arg_end;
		}
		memcpy(tdbb->tdbb_status_vector, status, sizeof(local));
		Firebird::status_exception::raise(status[1]);
	}
    return true;
}

void ExecuteStatement::Close(TDBB tdbb) {
	if (Statement) {
		tdbb->tdbb_transaction->tra_callback_count++;
		THREAD_EXIT;
		// for a while don't check for errors while freeing statement
		isc_dsql_free_statement(0, &Statement, DSQL_drop);
		THREAD_ENTER;
		tdbb->tdbb_transaction->tra_callback_count--;
		Statement = 0;
	}
	delete Sqlda;
	Sqlda = 0;
	if (Transaction)
	{
		THREAD_EXIT;
		WHY_cleanup_transaction(Transaction);
		THREAD_ENTER;
		delete Transaction;
		Transaction = 0;
	}
	delete Buffer;
	Buffer = 0;
	delete StartOfSqlOperator;
	StartOfSqlOperator = 0;
}

XSQLDA * ExecuteStatement::MakeSqlda(TDBB tdbb, short n) {
	return (XSQLDA *)
		(FB_NEW(*tdbb->tdbb_transaction->tra_pool) char[XSQLDA_LENGTH(n)]);
}

ISC_STATUS ExecuteStatement::ReMakeSqlda(ISC_STATUS *vector, TDBB tdbb)
{
	if (Sqlda->sqln != Sqlda->sqld) {
		vector[0] = gds_arg_gds;
		vector[1] = gds_wronumarg;
		vector[2] = gds_arg_end;
	}
	return vector[1];
}

ULONG ExecuteStatement::ParseSqlda(void) {
    ULONG offset = 0;
    int i=0;
    for (XSQLVAR *var=Sqlda->sqlvar;
                        i < Sqlda->sqld; var++, i++) {
        USHORT length = var->sqllen;
        int type = var->sqltype & (~1);
        if (type == SQL_VARYING)
            length += sizeof (SSHORT);
        var->sqldata = &Buffer[offset];
        offset += length;
        var->sqlind = (short*) (&Buffer[offset]);
        offset += sizeof (short);
    }
    return offset;
}

class str* ExecuteStatement::getString(MemoryPool* pool, dsc* d, const jrd_req* r) {
	char buffer[BUFFER_LARGE + sizeof(vary)];
	vary* v = reinterpret_cast<vary*>(buffer);
	v->vary_length = BUFFER_LARGE;

	UCHAR* p = 0;
	const SSHORT l = (d && !(r->req_flags & req_null)) ?
		MOV_get_string(d, &p, v, BUFFER_LARGE) : 0; // !!! How call Msgs ?
	if (! p) {
		ERR_post(isc_exec_sql_invalid_arg, 0);
	}

	class str* s = FB_NEW_RPT(*pool, l + 1) str();
	s->str_length = l;
	memcpy(s->str_data, p, l);
	return s;
}
