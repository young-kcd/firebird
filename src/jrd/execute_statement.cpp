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
 *  Adriano dos Santos Fernandes
 */

#include "firebird.h"
#include "gen/iberror.h"

#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../jrd/tra.h"
#include "../jrd/dsc.h"
#include "../jrd/err_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/exe_proto.h"
#include "../jrd/align.h"
#include "../jrd/execute_statement.h"
#include "../dsql/dsql_proto.h"

#include <string.h>
#include <math.h>

using namespace Jrd;

namespace {

const UCHAR sql_output_info[] = {isc_info_sql_select, isc_info_sql_num_variables};

} // anonymous namespace

void ExecuteStatement::Open(thread_db* tdbb, jrd_nod* sql, SSHORT nVars, bool singleton)
{
	SET_TDBB(tdbb);

	Attachment* const attachment = tdbb->getAttachment();
	jrd_tra* transaction = tdbb->getTransaction();

	if (transaction->tra_callback_count >= MAX_CALLBACKS)
	{
		ERR_post(isc_exec_sql_max_call_exceeded, 0);
	}

	blr = NULL;
	message = NULL;
	values = NULL;

	varCount = nVars;
	singleMode = singleton;

	Firebird::string sqlText;
	getString(tdbb, sqlText, EVL_expr(tdbb, sql), tdbb->getRequest());
	memcpy(startOfSqlOperator, sqlText.c_str(), sizeof(startOfSqlOperator) - 1);
	startOfSqlOperator[sizeof(startOfSqlOperator) - 1] = 0;

	transaction->tra_callback_count++;

	try
	{
		statement = DSQL_allocate_statement(tdbb, attachment);

		const Database* const dbb = tdbb->getDatabase();
		const int dialect = dbb->dbb_flags & DBB_DB_SQL_dialect_3 ? SQL_DIALECT_V6 : SQL_DIALECT_V5;

		UCHAR info_buffer[BUFFER_TINY], *info = info_buffer;

		DSQL_prepare(tdbb, transaction, &statement,
					 sqlText.length(), sqlText.c_str(), dialect,
					 sizeof(sql_output_info), sql_output_info,
					 sizeof(info_buffer), info_buffer);

		UCHAR tag = *info++;
		fb_assert(tag == isc_info_sql_select);
		tag = *info++;
		fb_assert(tag == isc_info_sql_num_variables);
		const int length = gds__vax_integer(info, sizeof(SSHORT));
		info += sizeof(SSHORT);
		const int number = gds__vax_integer(info, length);
		info += length;
		tag = *info++;
		fb_assert(tag == isc_info_end);

		if (!number)
		{
			DSQL_free_statement(tdbb, statement, DSQL_drop);
			statement = NULL;

			ERR_post(isc_exec_sql_invalid_req,
					 isc_arg_string, ERR_cstring(startOfSqlOperator),
					 0);
		}

		if (number != varCount)
		{
			DSQL_free_statement(tdbb, statement, DSQL_drop);
			statement = NULL;

			ERR_post(isc_wronumarg, 0);
		}

		DSQL_execute(tdbb, &transaction, statement,
					 0, NULL, 0, 0, NULL,
					 0, NULL, 0, 0, NULL);

		fb_assert(transaction == tdbb->getTransaction());
	}
	catch (const Firebird::Exception&)
	{
		transaction->tra_callback_count--;
		throw;
	}

	transaction->tra_callback_count--;
}

bool ExecuteStatement::Fetch(thread_db* tdbb, jrd_nod** jrdVar)
{
	fb_assert(statement);

	size_t blr_length = 0;

	if (!message && !blr && !values)
	{
		const size_t param_count = varCount * 2;

		MemoryPool& pool = *tdbb->getDefaultPool();

		values = FB_NEW(pool) Firebird::Array<dsc>(pool);
		values->grow(param_count);

		blr = FB_NEW(pool) Firebird::UCharBuffer(pool);
		blr->add(blr_version5);
		blr->add(blr_begin);
		blr->add(blr_message);
		blr->add(0);
		blr->add(param_count);
		blr->add(param_count >> 8);

		size_t msg_length = 0;

		for (int i = 0; i < varCount; i++)
		{
			jrd_nod* const target = jrdVar[i];
			fb_assert(target);

			dsc* const desc = EVL_assign_to(tdbb, target);
			(*values)[i * 2] = *desc;

			(*values)[i * 2 + 1].dsc_dtype = dtype_short;
			(*values)[i * 2 + 1].dsc_length = sizeof(SSHORT);

			// Generate BLR for the value
			generateBlr(desc);
			// Generate BLR for the NULL indicator
			blr->add(blr_short);
			blr->add(desc->dsc_scale);

			// Calculate the value offset
			msg_length = FB_ALIGN(msg_length, type_alignments[desc->dsc_dtype]);
			(*values)[i * 2].dsc_address = (UCHAR*)(IPTR) msg_length;
			msg_length += desc->dsc_length;

			// Calculate the NULL indicator offset
			msg_length = FB_ALIGN(msg_length, type_alignments[dtype_short]);
			(*values)[i * 2 + 1].dsc_address = (UCHAR*)(IPTR) msg_length;
			msg_length += sizeof(SSHORT);
		}

		blr->add(blr_end);
		blr_length = blr->getCount();

		message = FB_NEW(pool) Firebird::UCharBuffer(pool);
		message->grow(msg_length);
	}
	else
	{
		memset(message->begin(), 0, message->getCount());
	}

	ISC_STATUS status = DSQL_fetch(tdbb, statement,
						blr->getCount(), blr->begin(),
						0, message->getCount(), message->begin());

	if (status == 100)
	{
		DSQL_free_statement(tdbb, statement, DSQL_drop);
		statement = NULL;

		return false;
	}

	for (int i = 0; i < varCount; i++)
	{
		dsc desc = (*values)[i * 2];
		desc.dsc_address = message->begin() + (IPTR) desc.dsc_address;
		dsc null_desc = (*values)[i * 2 + 1];
		null_desc.dsc_address = message->begin() + (IPTR) null_desc.dsc_address;
		const bool null_flag = ((*(SSHORT*) null_desc.dsc_address) == 0) ? false : true;
		EXE_assignment(tdbb, jrdVar[i], &desc, null_flag, NULL, NULL);
	}

	if (singleMode)
	{
		status = DSQL_fetch(tdbb, statement,
							blr->getCount(), blr->begin(),
							0, message->getCount(), message->begin());

		if (status == 100)
		{
			DSQL_free_statement(tdbb, statement, DSQL_drop);
			statement = NULL;

			return false;
		}

		ERR_post(isc_sing_select_err, 0);
	}

	return true;
}

void ExecuteStatement::Close(thread_db* tdbb)
{
	if (statement)
	{
		DSQL_free_statement(tdbb, statement, DSQL_drop);
		statement = NULL;
	}

	delete blr;
	blr = NULL;
	delete message;
	message = NULL;
	delete values;
	values = NULL;
}

void ExecuteStatement::getString(thread_db* tdbb,
								 Firebird::string& sql,
								 const dsc* desc,
								 const jrd_req* request)
{
	MoveBuffer buffer;

	UCHAR* ptr = NULL;

	const SSHORT len = (desc && !(request->req_flags & req_null)) ?
		MOV_make_string2(tdbb, desc, desc->getTextType(), &ptr, buffer) : 0; // !!! How call Msgs ?

	if (!ptr)
	{
		ERR_post(isc_exec_sql_invalid_arg, 0);
	}

	sql.assign((const char*)ptr, len);
}

void ExecuteStatement::generateBlr(const dsc* desc)
{
	fb_assert(blr);

	USHORT length = 0;

	switch (desc->dsc_dtype)
	{
	case dtype_text:
		blr->add(blr_text2);
		blr->add(desc->dsc_ttype());
		blr->add(desc->dsc_ttype() >> 8);
		length = desc->dsc_length;
		blr->add(length);
		blr->add(length >> 8);
		break;

	case dtype_varying:
		blr->add(blr_varying2);
		blr->add(desc->dsc_ttype());
		blr->add(desc->dsc_ttype() >> 8);
		length = desc->dsc_length - sizeof(USHORT);
		blr->add(length);
		blr->add(length >> 8);
		break;

	case dtype_short:
		blr->add(blr_short);
		blr->add(desc->dsc_scale);
		break;

	case dtype_long:
		blr->add(blr_long);
		blr->add(desc->dsc_scale);
		break;

	case dtype_quad:
		blr->add(blr_quad);
		blr->add(desc->dsc_scale);
		break;

	case dtype_int64:
		blr->add(blr_int64);
		blr->add(desc->dsc_scale);
		break;

	case dtype_real:
		blr->add(blr_float);
		break;

	case dtype_double:
		blr->add(blr_double);
		break;

	case dtype_sql_date:
		blr->add(blr_sql_date);
		break;

	case dtype_sql_time:
		blr->add(blr_sql_time);
		break;

	case dtype_timestamp:
		blr->add(blr_timestamp);
		break;

	case dtype_array:
		blr->add(blr_quad);
		blr->add(0);
		break;

	case dtype_blob:
		blr->add(blr_blob2);
		blr->add(desc->dsc_sub_type);
		blr->add(desc->dsc_sub_type >> 8);
		blr->add(desc->getTextType());
		blr->add(desc->getTextType() >> 8);
		break;

	default:
		fb_assert(false);
	}
}
