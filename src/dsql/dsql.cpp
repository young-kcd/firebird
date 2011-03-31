/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		dsql.cpp
 *	DESCRIPTION:	Local processing for External entry points.
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 * 2001.07.06 Sean Leyne - Code Cleanup, removed "#ifdef READONLY_DATABASE"
 *                         conditionals, as the engine now fully supports
 *                         readonly databases.
 * December 2001 Mike Nordell: Major overhaul to (try to) make it C++
 * 2001.6.3 Claudio Valderrama: fixed a bad behaved loop in get_plan_info()
 * and get_rsb_item() that caused a crash when plan info was requested.
 * 2001.6.9 Claudio Valderrama: Added nod_del_view, nod_current_role and nod_breakleave.
 * 2002.10.29 Nickolay Samofatov: Added support for savepoints
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 * 2004.01.16 Vlad Horsun: added support for EXECUTE BLOCK statement
 * Adriano dos Santos Fernandes
 */

#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../dsql/dsql.h"
#include "../dsql/node.h"
#include "../jrd/ibase.h"
#include "../jrd/align.h"
#include "../jrd/intl.h"
#include "../common/intlobj_new.h"
#include "../jrd/jrd.h"
#include "../common/CharSet.h"
#include "../dsql/Parser.h"
#include "../dsql/ddl_proto.h"
#include "../dsql/dsql_proto.h"
#include "../dsql/errd_proto.h"
#include "../dsql/gen_proto.h"
#include "../dsql/make_proto.h"
#include "../dsql/movd_proto.h"
#include "../dsql/pass1_proto.h"
#include "../jrd/blb_proto.h"
#include "../jrd/cmp_proto.h"
#include "../yvalve/gds_proto.h"
#include "../jrd/inf_proto.h"
#include "../jrd/ini_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/jrd_proto.h"
#include "../jrd/opt_proto.h"
#include "../jrd/tra_proto.h"
#include "../jrd/recsrc/RecordSource.h"
#include "../jrd/trace/TraceManager.h"
#include "../jrd/trace/TraceDSQLHelpers.h"
#include "../common/classes/init.h"
#include "../common/utils_proto.h"
#include "../common/StatusArg.h"

#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif

using namespace Jrd;
using namespace Dsql;
using namespace Firebird;


static void		close_cursor(thread_db*, dsql_req*);
static void		execute_blob(thread_db*, dsql_req*, ULONG, const UCHAR*, ULONG, const UCHAR*,
						 ULONG, const UCHAR*, ULONG, UCHAR*);
static void		execute_immediate(thread_db*, Jrd::Attachment*, jrd_tra**,
							  ULONG, const TEXT*, USHORT,
							  ULONG, const UCHAR*, ULONG, const UCHAR*,
							  ULONG, const UCHAR*, ULONG, UCHAR*, bool);
static void		execute_request(thread_db*, dsql_req*, jrd_tra**, ULONG, const UCHAR*,
	ULONG, const UCHAR*, ULONG, const UCHAR*, ULONG, UCHAR*, bool);
static SSHORT	filter_sub_type(dsql_req*, const dsql_nod*);
static ULONG	get_request_info(thread_db*, dsql_req*, ULONG, UCHAR*);
static dsql_dbb*	init(Jrd::thread_db*, Jrd::Attachment*);
static void		map_in_out(dsql_req*, bool, const dsql_msg*, ULONG, const UCHAR*, ULONG, UCHAR*,
	const UCHAR* = 0);
static USHORT	parse_blr(dsql_req*, ULONG, const UCHAR*, const ULONG, const Array<dsql_par*>&);
static dsql_req* prepareRequest(thread_db*, dsql_dbb*, jrd_tra*, ULONG, const TEXT*, USHORT, USHORT, bool);
static dsql_req* prepareStatement(thread_db*, dsql_dbb*, jrd_tra*, ULONG, const TEXT*, USHORT, USHORT, bool);
static UCHAR*	put_item(UCHAR, const USHORT, const UCHAR*, UCHAR*, const UCHAR* const);
static void		release_statement(DsqlCompiledStatement* statement);
static void		sql_info(thread_db*, dsql_req*, ULONG, const UCHAR*, ULONG, UCHAR*);
static UCHAR*	var_info(const dsql_msg*, const UCHAR*, const UCHAR* const, UCHAR*,
	const UCHAR* const, USHORT, bool);

static inline bool reqTypeWithCursor(DsqlCompiledStatement::Type type)
{
	switch (type)
	{
	case DsqlCompiledStatement::TYPE_SELECT:
	case DsqlCompiledStatement::TYPE_SELECT_BLOCK:
	case DsqlCompiledStatement::TYPE_SELECT_UPD:
	case DsqlCompiledStatement::TYPE_GET_SEGMENT:
	case DsqlCompiledStatement::TYPE_PUT_SEGMENT:
		return true;
	}

	return false;
}

#ifdef DSQL_DEBUG
unsigned DSQL_debug = 0;
#endif

namespace
{
	const UCHAR record_info[] =
	{
		isc_info_req_update_count, isc_info_req_delete_count,
		isc_info_req_select_count, isc_info_req_insert_count
	};
}	// namespace


#ifdef DSQL_DEBUG
IMPLEMENT_TRACE_ROUTINE(dsql_trace, "DSQL")
#endif

dsql_dbb::~dsql_dbb()
{
}


/**

 	DSQL_allocate_statement

    @brief	Allocate a statement handle.


    @param tdbb
    @param attachment

 **/
dsql_req* DSQL_allocate_statement(thread_db* tdbb, Jrd::Attachment* attachment)
{
	SET_TDBB(tdbb);

	dsql_dbb* const database = init(tdbb, attachment);
	Jrd::ContextPoolHolder context(tdbb, database->createPool());

	// allocate the request block

	MemoryPool& pool = *tdbb->getDefaultPool();
	DsqlCompiledStatement* statement = FB_NEW(pool) DsqlCompiledStatement(pool);
	dsql_req* const request = FB_NEW(pool) dsql_req(statement);
	request->addRef();
	request->req_dbb = database;

	return request;
}


/**

 	DSQL_execute

    @brief	Execute a non-SELECT dynamic SQL statement.


    @param tdbb
    @param tra_handle
    @param request
    @param in_blr_length
    @param in_blr
    @param in_msg_type
    @param in_msg_length
    @param in_msg
    @param out_blr_length
    @param out_blr
    @param out_msg_length
    @param out_msg

 **/
void DSQL_execute(thread_db* tdbb,
				  jrd_tra** tra_handle,
				  dsql_req* request,
				  ULONG in_blr_length, const UCHAR* in_blr,
				  USHORT in_msg_type, ULONG in_msg_length, const UCHAR* in_msg,
				  ULONG out_blr_length, const UCHAR* const out_blr,
				  ULONG out_msg_length, UCHAR* out_msg)
{
	SET_TDBB(tdbb);

	Jrd::ContextPoolHolder context(tdbb, &request->getPool());

	const DsqlCompiledStatement* statement = request->getStatement();

	if (statement->getFlags() & DsqlCompiledStatement::FLAG_ORPHAN)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-901) <<
		          Arg::Gds(isc_bad_req_handle));
	}

	if ((SSHORT) in_msg_type == -1)
		request->req_flags |= dsql_req::FLAG_EMBEDDED;

	// Only allow NULL trans_handle if we're starting a transaction

	if (!*tra_handle && statement->getType() != DsqlCompiledStatement::TYPE_START_TRANS)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-901) <<
				  Arg::Gds(isc_bad_trans_handle));
	}

	// If the request is a SELECT or blob statement then this is an open.
	// Make sure the cursor is not already open.

	if (reqTypeWithCursor(statement->getType()))
	{
		if (request->req_flags & dsql_req::FLAG_OPENED_CURSOR)
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-502) <<
					  Arg::Gds(isc_dsql_cursor_open_err));
		}
	}

	// A select with a non zero output length is a singleton select
	const bool singleton =
		(statement->getType() == DsqlCompiledStatement::TYPE_SELECT && out_msg_length != 0);

	if (!(request->req_flags & dsql_req::FLAG_EMBEDDED))
	{
		execute_request(tdbb, request, tra_handle,
						in_blr_length, in_blr, in_msg_length, in_msg,
						out_blr_length, out_blr, out_msg_length, out_msg,
						singleton);
	}
	else
		request->req_transaction = *tra_handle;

	// If the output message length is zero on a TYPE_SELECT then we must
	// be doing an OPEN cursor operation.
	// If we do have an output message length, then we're doing
	// a singleton SELECT.  In that event, we don't add the cursor
	// to the list of open cursors (it's not really open).

	if (reqTypeWithCursor(statement->getType()) && !singleton)
	{
		request->req_flags |= dsql_req::FLAG_OPENED_CURSOR;
		TRA_link_cursor(request->req_transaction, request);
	}
}


/**

 	DSQL_execute_immediate

    @brief	Execute a non-SELECT dynamic SQL statement.


    @param tdbb
    @param attachment
    @param tra_handle
    @param length
    @param string
    @param dialect
    @param in_blr_length
    @param in_blr
    @param in_msg_length
    @param in_msg
    @param out_blr_length
    @param out_blr
    @param out_msg_length
    @param out_msg

 **/
void DSQL_execute_immediate(thread_db* tdbb,
							Jrd::Attachment* attachment,
							jrd_tra** tra_handle,
							ULONG length, const TEXT* string, USHORT dialect,
							ULONG in_blr_length, const UCHAR* in_blr,
							ULONG in_msg_length, const UCHAR* in_msg,
							ULONG out_blr_length, const UCHAR* out_blr,
							ULONG out_msg_length, UCHAR* out_msg,
							bool isInternalRequest)
{
	execute_immediate(tdbb, attachment, tra_handle, length,
		string, dialect,
		in_blr_length, in_blr, in_msg_length, in_msg,
		out_blr_length, out_blr, out_msg_length, out_msg,
		isInternalRequest);
}


/**

 	DSQL_fetch

    @brief	Fetch next record from a dynamic SQL cursor


    @param user_status
    @param req_handle
    @param blr_length
    @param blr
    @param msg_length
    @param dsql_msg
    @param direction
    @param offset

 **/
ISC_STATUS DSQL_fetch(thread_db* tdbb,
					  dsql_req* request,
					  ULONG blr_length, const UCHAR* blr,
					  ULONG msg_length, UCHAR* dsql_msg_buf)
{
	SET_TDBB(tdbb);

	Jrd::ContextPoolHolder context(tdbb, &request->getPool());

	const DsqlCompiledStatement* statement = request->getStatement();

	// if the cursor isn't open, we've got a problem
	if (reqTypeWithCursor(statement->getType()))
	{
		if (!(request->req_flags & dsql_req::FLAG_OPENED_CURSOR))
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-504) <<
					  Arg::Gds(isc_dsql_cursor_err) <<
					  Arg::Gds(isc_dsql_cursor_not_open));
		}
	}

	dsql_msg* message = (dsql_msg*) statement->getReceiveMsg();

	// Set up things for tracing this call
	Jrd::Attachment* att = request->req_dbb->dbb_attachment;
	TraceDSQLFetch trace(att, request);

	// Insure that the blr for the message is parsed, regardless of
	// whether anything is found by the call to receive.

	if (blr_length) {
		parse_blr(request, blr_length, blr, msg_length, message->msg_parameters);
	}

	if (statement->getType() == DsqlCompiledStatement::TYPE_GET_SEGMENT)
	{
		// For get segment, use the user buffer and indicator directly.

		const dsql_par* parameter = statement->getBlob()->blb_segment;
		const dsql_par* null = parameter->par_null;

		dsc userDesc;
		if (!request->req_user_descs.get(parameter, userDesc))
			userDesc.clear();

		dsc userNullDesc;
		if (!request->req_user_descs.get(null, userNullDesc))
			userNullDesc.clear();

		USHORT* ret_length = (USHORT*) (dsql_msg_buf + (IPTR) userNullDesc.dsc_address);
		UCHAR* buffer = dsql_msg_buf + (IPTR) userDesc.dsc_address;

		*ret_length = BLB_get_segment(tdbb, request->req_blb, buffer, userDesc.dsc_length);

		if (request->req_blb->blb_flags & BLB_eof)
			return 100;

		if (request->req_blb->blb_fragment_size)
			return 101;

		return 0;
	}

	UCHAR* msgBuffer = request->req_msg_buffers[message->msg_buffer_number];

	JRD_receive(tdbb, request->req_request, message->msg_number, message->msg_length,
		msgBuffer);

	const dsql_par* const eof = statement->getEof();

	dsc eofDesc = eof->par_desc;
	eofDesc.dsc_address = msgBuffer + (IPTR) eofDesc.dsc_address;
	const bool eof_reached = eof && !*((USHORT*) eofDesc.dsc_address);

	if (eof_reached)
	{
		trace.fetch(true, res_successful);
		return 100;
	}

	map_in_out(request, true, message, 0, blr, msg_length, dsql_msg_buf);

	trace.fetch(false, res_successful);
	return FB_SUCCESS;
}


/**

 	DSQL_free_statement

    @brief	Release request for a dsql statement


    @param user_status
    @param req_handle
    @param option

 **/
void DSQL_free_statement(thread_db* tdbb, dsql_req* request, USHORT option)
{
	SET_TDBB(tdbb);

	Jrd::ContextPoolHolder context(tdbb, &request->getPool());

	const DsqlCompiledStatement* statement = request->getStatement();

	if (option & DSQL_drop)
	{
		// Release everything associated with the request
		dsql_req::destroy(tdbb, request, true);
	}
	else if (option & DSQL_unprepare)
	{
		// Release everything but the request itself
		dsql_req::destroy(tdbb, request, false);
	}
	else if (option & DSQL_close)
	{
		// Just close the cursor associated with the request
		if (reqTypeWithCursor(statement->getType()))
		{
			if (!(request->req_flags & dsql_req::FLAG_OPENED_CURSOR))
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-501) <<
						  Arg::Gds(isc_dsql_cursor_close_err));
			}

			close_cursor(tdbb, request);
		}
	}
}


/**

 	DSQL_insert

    @brief	Insert next record into a dynamic SQL cursor


    @param user_status
    @param req_handle
    @param blr_length
    @param blr
    @param msg_length
    @param dsql_msg

 **/
void DSQL_insert(thread_db* tdbb,
				 dsql_req* request,
				 ULONG blr_length, const UCHAR* blr,
				 ULONG msg_length, const UCHAR* dsql_msg_buf)
{
	SET_TDBB(tdbb);

	Jrd::ContextPoolHolder context(tdbb, &request->getPool());

	const DsqlCompiledStatement* statement = request->getStatement();

	if (statement->getFlags() & DsqlCompiledStatement::FLAG_ORPHAN)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-901) <<
		          Arg::Gds(isc_bad_req_handle));
	}

	// if the cursor isn't open, we've got a problem

	if (statement->getType() == DsqlCompiledStatement::TYPE_PUT_SEGMENT)
	{
		if (!(request->req_flags & dsql_req::FLAG_OPENED_CURSOR))
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-504) <<
					  Arg::Gds(isc_dsql_cursor_err) <<
					  Arg::Gds(isc_dsql_cursor_not_open));
		}
	}

	dsql_msg* message = (dsql_msg*) statement->getReceiveMsg();

	// Insure that the blr for the message is parsed, regardless of
	// whether anything is found by the call to receive.

	if (blr_length)
		parse_blr(request, blr_length, blr, msg_length, message->msg_parameters);

	if (statement->getType() == DsqlCompiledStatement::TYPE_PUT_SEGMENT)
	{
		// For put segment, use the user buffer and indicator directly.

		const dsql_par* const parameter = statement->getBlob()->blb_segment;

		dsc userDesc;
		if (!request->req_user_descs.get(parameter, userDesc))
			userDesc.clear();

		const UCHAR* const buffer = dsql_msg_buf + (IPTR) userDesc.dsc_address;
		BLB_put_segment(tdbb, request->req_blb, buffer, userDesc.dsc_length);
	}
}


/**

 	DSQL_prepare

    @brief	Prepare a statement for execution.


    @param user_status
    @param trans_handle
    @param req_handle
    @param length
    @param string
    @param dialect
    @param item_length
    @param items
    @param buffer_length
    @param buffer

 **/
void DSQL_prepare(thread_db* tdbb,
				  jrd_tra* transaction,
				  dsql_req** req_handle,
				  ULONG length, const TEXT* string, USHORT dialect,
				  ULONG item_length, const UCHAR* items,
				  ULONG buffer_length, UCHAR* buffer,
				  bool isInternalRequest)
{
	SET_TDBB(tdbb);

	dsql_req* const old_request = *req_handle;

	if (!old_request)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-901) <<
		          Arg::Gds(isc_bad_req_handle));
	}

	dsql_dbb* database = old_request->req_dbb;
	if (!database)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-901) <<
                  Arg::Gds(isc_bad_req_handle));
	}

	// check to see if old request has an open cursor

	if (old_request && (old_request->req_flags & dsql_req::FLAG_OPENED_CURSOR))
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-519) <<
				  Arg::Gds(isc_dsql_open_cursor_request));
	}

	dsql_req* request = NULL;

	if (string && !length)
	{
		length = strlen(string);
	}

	if (!string || !length)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  // Unexpected end of command
				  // CVC: Nothing will be line 1, column 1 for the user.
				  Arg::Gds(isc_command_end_err2) << Arg::Num(1) << Arg::Num(1));
	}

	try
	{
		// Figure out which parser version to use
		// Since the API to dsql8_prepare is public and can not be changed, there needs to
		// be a way to send the parser version to DSQL so that the parser can compare the keyword
		// version to the parser version.  To accomplish this, the parser version is combined with
		// the client dialect and sent across that way.  In dsql8_prepare_statement, the parser version
		// and client dialect are separated and passed on to their final destinations.  The information
		// is combined as follows:
		//     Dialect * 10 + parser_version
		//
		// and is extracted in dsql8_prepare_statement as follows:
		//      parser_version = ((dialect *10)+parser_version)%10
		//      client_dialect = ((dialect *10)+parser_version)/10
		//
		// For example, parser_version = 1 and client dialect = 1
		//
		//  combined = (1 * 10) + 1 == 11
		//
		//  parser = (combined) %10 == 1
		//  dialect = (combined) / 19 == 1
		//
		// If the parser version is not part of the dialect, then assume that the
		// connection being made is a local classic connection.

		USHORT parser_version;
		if ((dialect / 10) == 0)
			parser_version = 2;
		else
		{
			parser_version = dialect % 10;
			dialect /= 10;
		}

		// Allocate a new request block and then prepare the request.  We want to
		// keep the old request around, as is, until we know that we are able
		// to prepare the new one.
		// It would be really *nice* to know *why* we want to
		// keep the old request around -- 1994-October-27 David Schnepper
		// Because that's the client's allocated statement handle and we
		// don't want to trash the context in it -- 2001-Oct-27 Ann Harrison

		request = prepareRequest(tdbb, database, transaction, length, string, dialect, parser_version,
			isInternalRequest);

		// Can not prepare a CREATE DATABASE/SCHEMA statement

		const DsqlCompiledStatement* statement = request->getStatement();

		if (statement->getType() == DsqlCompiledStatement::TYPE_CREATE_DB)
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-530) <<
					  Arg::Gds(isc_dsql_crdb_prepare_err));
		}

		// Now that we know that the new request exists, zap the old one.

		{
			Jrd::ContextPoolHolder context(tdbb, &old_request->getPool());
			dsql_req::destroy(tdbb, old_request, true);
		}

		*req_handle = request;

		Jrd::ContextPoolHolder context(tdbb, &request->getPool());
		sql_info(tdbb, request, item_length, items, buffer_length, buffer);
	}
	catch (const Firebird::Exception&)
	{
		if (request)
		{
			Jrd::ContextPoolHolder context(tdbb, &request->getPool());
			dsql_req::destroy(tdbb, request, true);
		}
		throw;
	}
}


/**

	DSQL_set_cursor

    @brief	Set a cursor name for a dynamic request


    @param tdbb
    @param req_handle
    @param input_cursor

 **/
void DSQL_set_cursor(thread_db* tdbb, dsql_req* request, const TEXT* input_cursor)
{
	SET_TDBB(tdbb);

	Jrd::ContextPoolHolder context(tdbb, &request->getPool());

	const size_t MAX_CURSOR_LENGTH = 132 - 1;
	string cursor = input_cursor;

	if (cursor[0] == '\"')
	{
		// Quoted cursor names eh? Strip'em.
		// Note that "" will be replaced with ".
		// The code is very strange, because it doesn't check for "" really
		// and thus deletes one isolated " in the middle of the cursor.
		for (string::iterator i = cursor.begin(); i < cursor.end(); ++i)
		{
			if (*i == '\"')
				cursor.erase(i);
		}
	}
	else	// not quoted name
	{
		const string::size_type i = cursor.find(' ');
		if (i != string::npos)
			cursor.resize(i);

		cursor.upper();
	}

	USHORT length = (USHORT) fb_utils::name_length(cursor.c_str());

	if (length == 0)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-502) <<
				  Arg::Gds(isc_dsql_decl_err) <<
				  Arg::Gds(isc_dsql_cursor_invalid));
	}

	if (length > MAX_CURSOR_LENGTH)
		length = MAX_CURSOR_LENGTH;

	cursor.resize(length);

	// If there already is a different cursor by the same name, bitch

	dsql_req* const* symbol = request->req_dbb->dbb_cursors.get(cursor);
	if (symbol)
	{
		if (request == *symbol)
			return;

		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-502) <<
				  Arg::Gds(isc_dsql_decl_err) <<
				  Arg::Gds(isc_dsql_cursor_redefined) << cursor);
	}

	// If there already is a cursor and its name isn't the same, ditto.
	// We already know there is no cursor by this name in the hash table

	if (request->req_cursor.isEmpty())
	{
		request->req_cursor = cursor;
		request->req_dbb->dbb_cursors.put(cursor, request);
	}
	else
	{
		fb_assert(request != *symbol);
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-502) <<
				  Arg::Gds(isc_dsql_decl_err) <<
				  Arg::Gds(isc_dsql_cursor_redefined) << request->req_cursor);
	}
}


/**

 	DSQL_sql_info

    @brief	Provide information on dsql statement


    @param user_status
    @param req_handle
    @param item_length
    @param items
    @param info_length
    @param info

 **/
void DSQL_sql_info(thread_db* tdbb,
				   dsql_req* request,
				   ULONG item_length, const UCHAR* items,
				   ULONG info_length, UCHAR* info)
{
	SET_TDBB(tdbb);

	Jrd::ContextPoolHolder context(tdbb, &request->getPool());

	sql_info(tdbb, request, item_length, items, info_length, info);
}


/**

 	close_cursor

    @brief	Close an open cursor.


    @param request
    @param tdbb

 **/
static void close_cursor(thread_db* tdbb, dsql_req* request)
{
	SET_TDBB(tdbb);

	Jrd::Attachment* attachment = request->req_dbb->dbb_attachment;
	const DsqlCompiledStatement* statement = request->getStatement();

	if (request->req_request)
	{
		ThreadStatusGuard status_vector(tdbb);
		try
		{
			if (statement->getType() == DsqlCompiledStatement::TYPE_GET_SEGMENT ||
				statement->getType() == DsqlCompiledStatement::TYPE_PUT_SEGMENT)
			{
				BLB_close(tdbb, request->req_blb);
				request->req_blb = NULL;
			}
			else
			{
				// Report some remaining fetches if any
				if (request->req_fetch_baseline)
				{
					TraceDSQLFetch trace(attachment, request);
					trace.fetch(true, res_successful);
				}

				if (request->req_traced && TraceManager::need_dsql_free(attachment))
				{
					TraceSQLStatementImpl stmt(request, NULL);

					TraceManager::event_dsql_free(attachment, &stmt, DSQL_close);
				}

				JRD_unwind_request(tdbb, request->req_request);
			}
		}
		catch (Firebird::Exception&)
		{
		}
	}

	request->req_flags &= ~dsql_req::FLAG_OPENED_CURSOR;
	TRA_unlink_cursor(request->req_transaction, request);
}


/**

 	execute_blob

    @brief	Open or create a blob.


	@param tdbb
    @param request
    @param in_blr_length
    @param in_blr
    @param in_msg_length
    @param in_msg
    @param out_blr_length
    @param out_blr
    @param out_msg_length
    @param out_msg

 **/
static void execute_blob(thread_db* tdbb,
						 dsql_req* request,
						 ULONG in_blr_length,
						 const UCHAR* in_blr,
						 ULONG in_msg_length,
						 const UCHAR* in_msg,
						 ULONG out_blr_length,
						 const UCHAR* out_blr,
						 ULONG out_msg_length,
						 UCHAR* out_msg)
{
	UCHAR bpb[24];

	const DsqlCompiledStatement* statement = request->getStatement();

	const dsql_blb* blob = statement->getBlob();
	map_in_out(request, false, blob->blb_open_in_msg, in_blr_length, in_blr, in_msg_length,
		NULL, in_msg);

	UCHAR* p = bpb;
	*p++ = isc_bpb_version1;

	SSHORT filter = filter_sub_type(request, blob->blb_to);

	if (filter)
	{
		*p++ = isc_bpb_target_type;
		*p++ = 2;
		*p++ = static_cast<UCHAR>(filter);
		*p++ = filter >> 8;
	}

	filter = filter_sub_type(request, blob->blb_from);

	if (filter)
	{
		*p++ = isc_bpb_source_type;
		*p++ = 2;
		*p++ = static_cast<UCHAR>(filter);
		*p++ = filter >> 8;
	}

	USHORT bpb_length = p - bpb;

	if (bpb_length == 1)
		bpb_length = 0;

	dsql_par* parameter = blob->blb_blob_id;
	const dsql_par* null = parameter->par_null;

	UCHAR* msgBuffer = request->req_msg_buffers[parameter->par_message->msg_buffer_number];

	dsc desc = parameter->par_desc;
	desc.dsc_address = msgBuffer + (IPTR) desc.dsc_address;

	bid* blob_id = (bid*) desc.dsc_address;

	if (statement->getType() == DsqlCompiledStatement::TYPE_GET_SEGMENT)
	{
		if (null)
		{
			desc = null->par_desc;
			desc.dsc_address = msgBuffer + (IPTR) desc.dsc_address;

			if (*((SSHORT*) desc.dsc_address) < 0)
				memset(blob_id, 0, sizeof(bid));
		}

		request->req_blb = BLB_open2(tdbb, request->req_transaction, blob_id, bpb_length, bpb, true);
	}
	else
	{
		request->req_request = NULL;
		memset(blob_id, 0, sizeof(bid));

		request->req_blb = BLB_create2(tdbb, request->req_transaction, blob_id, bpb_length, bpb);

		map_in_out(request, true, blob->blb_open_out_msg, out_blr_length, out_blr,
			out_msg_length, out_msg);
	}
}


/**

 	execute_immediate

    @brief	Common part of prepare and execute a statement.


    @param tdbb
    @param attachment
    @param tra_handle
	@param length
	@param string
	@param dialect
    @param in_blr_length
    @param in_blr
    @param in_msg_length
    @param in_msg
    @param out_blr_length
    @param out_blr
    @param out_msg_length
    @param out_msg

 **/
static void execute_immediate(thread_db* tdbb,
							  Jrd::Attachment* attachment,
							  jrd_tra** tra_handle,
							  ULONG length, const TEXT* string, USHORT dialect,
							  ULONG in_blr_length, const UCHAR* in_blr,
							  ULONG in_msg_length, const UCHAR* in_msg,
							  ULONG out_blr_length, const UCHAR* out_blr,
							  ULONG out_msg_length, UCHAR* out_msg,
							  bool isInternalRequest)
{
	SET_TDBB(tdbb);

	if (!string)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  // Unexpected end of command
				  // CVC: Nothing will be line 1, column 1 for the user.
				  Arg::Gds(isc_command_end_err2) << Arg::Num(1) << Arg::Num(1));
	}

	if (length == 0)
		length = strlen(string);

	dsql_dbb* const database = init(tdbb, attachment);
	dsql_req* request = NULL;

	try
	{
		// Figure out which parser version to use
		// Since the API to dsql8_execute_immediate is public and can not be changed, there needs to
		// be a way to send the parser version to DSQL so that the parser can compare the keyword
		// version to the parser version.  To accomplish this, the parser version is combined with
		// the client dialect and sent across that way.  In dsql8_execute_immediate, the parser version
		// and client dialect are separated and passed on to their final destinations.  The information
		// is combined as follows:
		//     Dialect * 10 + parser_version
		//
		// and is extracted in dsql8_execute_immediate as follows:
		//      parser_version = ((dialect *10)+parser_version)%10
		//      client_dialect = ((dialect *10)+parser_version)/10
		//
		// For example, parser_version = 1 and client dialect = 1
		//
		//  combined = (1 * 10) + 1 == 11
		//
		//  parser = (combined) % 10 == 1
		//  dialect = (combined) / 19 == 1
		//
		// If the parser version is not part of the dialect, then assume that the
		// connection being made is a local classic connection.

		USHORT parser_version;
		if ((dialect / 10) == 0)
			parser_version = 2;
		else
		{
			parser_version = dialect % 10;
			dialect /= 10;
		}

		request = prepareRequest(tdbb, database, *tra_handle, length, string, dialect, parser_version,
			isInternalRequest);

		const DsqlCompiledStatement* statement = request->getStatement();

		// Only allow NULL trans_handle if we're starting a transaction

		if (!*tra_handle && statement->getType() != DsqlCompiledStatement::TYPE_START_TRANS)
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-901) <<
					  Arg::Gds(isc_bad_trans_handle));
		}

		Jrd::ContextPoolHolder context(tdbb, &request->getPool());

		// A select with a non zero output length is a singleton select
		const bool singleton =
			(statement->getType() == DsqlCompiledStatement::TYPE_SELECT && out_msg_length != 0);

		execute_request(tdbb, request, tra_handle,
						in_blr_length, in_blr, in_msg_length, in_msg,
						out_blr_length, out_blr, out_msg_length, out_msg,
						singleton);

		dsql_req::destroy(tdbb, request, true);
	}
	catch (const Firebird::Exception&)
	{
		if (request)
		{
			Jrd::ContextPoolHolder context(tdbb, &request->getPool());
			dsql_req::destroy(tdbb, request, true);
		}
		throw;
	}
}


/**

 	execute_request

    @brief	Execute a dynamic SQL statement.


	@param tdbb
    @param request
    @param trans_handle
    @param in_blr_length
    @param in_blr
    @param in_msg_length
    @param in_msg
    @param out_blr_length
    @param out_blr
    @param out_msg_length
    @param out_msg
	@param singleton

 **/
static void execute_request(thread_db* tdbb,
							dsql_req* request,
							jrd_tra** tra_handle,
							ULONG in_blr_length, const UCHAR* in_blr,
							ULONG in_msg_length, const UCHAR* in_msg,
							ULONG out_blr_length, const UCHAR* out_blr,
							ULONG out_msg_length, UCHAR* out_msg,
							bool singleton)
{
	request->req_transaction = *tra_handle;

	const DsqlCompiledStatement* statement = request->getStatement();

	switch (statement->getType())
	{
	case DsqlCompiledStatement::TYPE_START_TRANS:
		JRD_start_transaction(tdbb, &request->req_transaction, 1, request->req_dbb->dbb_attachment,
			statement->getDdlData().getCount(), statement->getDdlData().begin());
		*tra_handle = request->req_transaction;
		return;

	case DsqlCompiledStatement::TYPE_COMMIT:
		JRD_commit_transaction(tdbb, request->req_transaction);
		*tra_handle = NULL;
		return;

	case DsqlCompiledStatement::TYPE_COMMIT_RETAIN:
		JRD_commit_retaining(tdbb, request->req_transaction);
		return;

	case DsqlCompiledStatement::TYPE_ROLLBACK:
		JRD_rollback_transaction(tdbb, request->req_transaction);
		*tra_handle = NULL;
		return;

	case DsqlCompiledStatement::TYPE_ROLLBACK_RETAIN:
		JRD_rollback_retaining(tdbb, request->req_transaction);
		return;

	case DsqlCompiledStatement::TYPE_CREATE_DB:
	case DsqlCompiledStatement::TYPE_DDL:
		{
			TraceDSQLExecute trace(request->req_dbb->dbb_attachment, request);
			DDL_execute(request);
			trace.finish(false, res_successful);
			return;
		}

	case DsqlCompiledStatement::TYPE_GET_SEGMENT:
		execute_blob(tdbb, request,
					 in_blr_length, in_blr, in_msg_length, in_msg,
					 out_blr_length, out_blr, out_msg_length, out_msg);
		return;

	case DsqlCompiledStatement::TYPE_PUT_SEGMENT:
		execute_blob(tdbb, request,
					 in_blr_length, in_blr, in_msg_length, in_msg,
					 out_blr_length, out_blr, out_msg_length, out_msg);
		return;

	case DsqlCompiledStatement::TYPE_SELECT:
	case DsqlCompiledStatement::TYPE_SELECT_UPD:
	case DsqlCompiledStatement::TYPE_INSERT:
	case DsqlCompiledStatement::TYPE_UPDATE:
	case DsqlCompiledStatement::TYPE_UPDATE_CURSOR:
	case DsqlCompiledStatement::TYPE_DELETE:
	case DsqlCompiledStatement::TYPE_DELETE_CURSOR:
	case DsqlCompiledStatement::TYPE_EXEC_PROCEDURE:
	case DsqlCompiledStatement::TYPE_SET_GENERATOR:
	case DsqlCompiledStatement::TYPE_SAVEPOINT:
	case DsqlCompiledStatement::TYPE_EXEC_BLOCK:
	case DsqlCompiledStatement::TYPE_SELECT_BLOCK:
		break;

	default:
		// Catch invalid request types
		fb_assert(false);
	}

	// If there is no data required, just start the request

	const dsql_msg* message = statement->getSendMsg();
	if (message)
		map_in_out(request, false, message, in_blr_length, in_blr, in_msg_length, NULL, in_msg);

	// we need to map_in_out before tracing of execution start to let trace
	// manager know statement parameters values
	TraceDSQLExecute trace(request->req_dbb->dbb_attachment, request);

	if (!message)
		JRD_start(tdbb, request->req_request, request->req_transaction);
	else
	{
		UCHAR* msgBuffer = request->req_msg_buffers[message->msg_buffer_number];
		JRD_start_and_send(tdbb, request->req_request, request->req_transaction, message->msg_number,
			message->msg_length, msgBuffer);
	}

	// Selectable execute block should get the "proc fetch" flag assigned,
	// which ensures that the savepoint stack is preserved while suspending
	if (statement->getType() == DsqlCompiledStatement::TYPE_SELECT_BLOCK)
	{
		fb_assert(request->req_request);
		request->req_request->req_flags |= req_proc_fetch;
	}

	// TYPE_EXEC_BLOCK has no outputs so there are no out_msg
	// supplied from client side, but TYPE_EXEC_BLOCK requires
	// 2-byte message for EOS synchronization
	const bool isBlock = (statement->getType() == DsqlCompiledStatement::TYPE_EXEC_BLOCK);

	message = statement->getReceiveMsg();

	if ((out_msg_length && message) || isBlock)
	{
		UCHAR temp_buffer[FB_DOUBLE_ALIGN * 2];
		dsql_msg temp_msg(*getDefaultMemoryPool());

		// Insure that the blr for the message is parsed, regardless of
		// whether anything is found by the call to receive.

		UCHAR* msgBuffer = request->req_msg_buffers[message->msg_buffer_number];

		if (out_msg_length && out_blr_length)
			parse_blr(request, out_blr_length, out_blr, out_msg_length, message->msg_parameters);
		else if (!out_msg_length && isBlock)
		{
			message = &temp_msg;
			temp_msg.msg_number = 1;
			temp_msg.msg_length = 2;
			msgBuffer = (UCHAR*) FB_ALIGN((U_IPTR) temp_buffer, FB_DOUBLE_ALIGN);
		}

		JRD_receive(tdbb, request->req_request, message->msg_number, message->msg_length,
			msgBuffer);

		if (out_msg_length)
			map_in_out(request, true, message, 0, out_blr, out_msg_length, out_msg);

		// if this is a singleton select, make sure there's in fact one record

		if (singleton)
		{
			USHORT counter;

			// Create a temp message buffer and try two more receives.
			// If both succeed then the first is the next record and the
			// second is either another record or the end of record message.
			// In either case, there's more than one record.

			UCHAR* message_buffer = (UCHAR*) gds__alloc(message->msg_length);

			ISC_STATUS status = FB_SUCCESS;
			ISC_STATUS_ARRAY localStatus;

			for (counter = 0; counter < 2 && !status; counter++)
			{
				AutoSetRestore<ISC_STATUS*> autoStatus(&tdbb->tdbb_status_vector, localStatus);
				fb_utils::init_status(localStatus);

				try
				{
					JRD_receive(tdbb, request->req_request, message->msg_number,
								message->msg_length, message_buffer);
					status = FB_SUCCESS;
				}
				catch (Firebird::Exception&)
				{
					status = tdbb->tdbb_status_vector[1];
				}
			}

			gds__free(message_buffer);

			// two successful receives means more than one record
			// a req_sync error on the first pass above means no records
			// a non-req_sync error on any of the passes above is an error

			if (!status)
				status_exception::raise(Arg::Gds(isc_sing_select_err));
			else if (status == isc_req_sync && counter == 1)
				status_exception::raise(Arg::Gds(isc_stream_eof));
			else if (status != isc_req_sync)
				status_exception::raise(localStatus);
		}
	}

	if (statement->getType() == DsqlCompiledStatement::TYPE_UPDATE_CURSOR)
	{
		if (!request->req_request->req_records_updated)
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-913) <<
					  Arg::Gds(isc_deadlock) <<
					  Arg::Gds(isc_update_conflict));
		}
	}
	else if (statement->getType() == DsqlCompiledStatement::TYPE_DELETE_CURSOR)
	{
		if (!request->req_request->req_records_deleted)
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-913) <<
					  Arg::Gds(isc_deadlock) <<
					  Arg::Gds(isc_update_conflict));
		}
	}

	const bool have_cursor = reqTypeWithCursor(statement->getType()) && !singleton;
	trace.finish(have_cursor, res_successful);
}

/**

 	filter_sub_type

    @brief	Determine the sub_type to use in filtering
 	a blob.


    @param node

 **/
static SSHORT filter_sub_type(dsql_req* request, const dsql_nod* node)
{
	const LiteralNode* literal = ExprNode::as<LiteralNode>(node);
	if (literal)
		return (SSHORT) literal->getSlong();

	const dsql_par* parameter = ExprNode::as<ParameterNode>(node)->dsqlParameter;
	const dsql_par* null = parameter->par_null;
	dsc desc;

	UCHAR* msgBuffer = request->req_msg_buffers[parameter->par_message->msg_buffer_number];

	if (null)
	{
		desc = null->par_desc;
		desc.dsc_address = msgBuffer + (IPTR) null->par_desc.dsc_address;

		if (*((SSHORT*) desc.dsc_address))
			return 0;
	}

	desc = parameter->par_desc;
	desc.dsc_address = msgBuffer + (IPTR) parameter->par_desc.dsc_address;

	return *((SSHORT*) desc.dsc_address);
}


/**

 	get_request_info

    @brief	Get the records updated/deleted for record


    @param request
    @param buffer_length
    @param buffer

 **/
static ULONG get_request_info(thread_db* tdbb, dsql_req* request, ULONG buffer_length, UCHAR* buffer)
{
	if (!request->req_request)	// DDL
		return 0;

	// get the info for the request from the engine

	try
	{
		return INF_request_info(request->req_request, sizeof(record_info), record_info,
			buffer_length, buffer);
	}
	catch (Firebird::Exception&)
	{
		return 0;
	}
}


/**

 	init

    @brief	Initialize dynamic SQL.  This is called only once.


    @param db_handle

 **/
static dsql_dbb* init(thread_db* tdbb, Jrd::Attachment* attachment)
{
	SET_TDBB(tdbb);
	Database* const dbb = tdbb->getDatabase();

	if (attachment->att_dsql_instance)
		return attachment->att_dsql_instance;

	MemoryPool& pool = *attachment->att_database->createPool();
	dsql_dbb* const database = FB_NEW(pool) dsql_dbb(pool);
	database->dbb_attachment = attachment;
	database->dbb_database = attachment->att_database;
	attachment->att_dsql_instance = database;

	INI_init_dsql(tdbb, database);

	database->dbb_db_SQL_dialect =
		(dbb->dbb_flags & DBB_DB_SQL_dialect_3) ? SQL_DIALECT_V6 : SQL_DIALECT_V5;

	database->dbb_ods_version = dbb->dbb_ods_version;
	database->dbb_minor_version = dbb->dbb_minor_version;

	database->dbb_read_only = (dbb->dbb_flags & DBB_read_only) ? 1 : 0;

#ifdef DSQL_DEBUG
	DSQL_debug = Config::getTraceDSQL();
#endif

	return attachment->att_dsql_instance;
}


/**

 	map_in_out

    @brief	Map data from external world into message or
 	from message to external world.


    @param request
    @param message
    @param blr_length
    @param blr
    @param msg_length
    @param dsql_msg_buf
    @param in_dsql_msg_buf

 **/
static void map_in_out(dsql_req* request, bool toExternal, const dsql_msg* message,
	ULONG blr_length, const UCHAR* blr, ULONG msg_length, UCHAR* dsql_msg_buf,
	const UCHAR* in_dsql_msg_buf)
{
	USHORT count = parse_blr(request, blr_length, blr, msg_length, message->msg_parameters);

	bool err = false;

	for (size_t i = 0; i < message->msg_parameters.getCount(); ++i)
	{
		dsql_par* parameter = message->msg_parameters[i];

		if (parameter->par_index)
		{
			 // Make sure the message given to us is long enough

			dsc desc;
			if (!request->req_user_descs.get(parameter, desc))
				desc.clear();

			ULONG length = (IPTR) desc.dsc_address + desc.dsc_length;

			if (length > msg_length || !desc.dsc_dtype)
			{
				err = true;
				break;
			}

			UCHAR* msgBuffer = request->req_msg_buffers[parameter->par_message->msg_buffer_number];

			SSHORT* flag = NULL;
			dsql_par* const null_ind = parameter->par_null;
			if (null_ind != NULL)
			{
				dsc userNullDesc;
				if (!request->req_user_descs.get(null_ind, userNullDesc))
					userNullDesc.clear();

				const ULONG null_offset = (IPTR) userNullDesc.dsc_address;
				length = null_offset + sizeof(SSHORT);
				if (length > msg_length)
				{
					err = true;
					break;
				}

				dsc nullDesc = null_ind->par_desc;
				nullDesc.dsc_address = msgBuffer + (IPTR) nullDesc.dsc_address;

				if (toExternal)
				{
					flag = reinterpret_cast<SSHORT*>(dsql_msg_buf + null_offset);
					*flag = *reinterpret_cast<const SSHORT*>(nullDesc.dsc_address);
				}
				else
				{
					flag = reinterpret_cast<SSHORT*>(nullDesc.dsc_address);
					*flag = *reinterpret_cast<const SSHORT*>(in_dsql_msg_buf + null_offset);
				}
			}

			dsc parDesc = parameter->par_desc;
			parDesc.dsc_address = msgBuffer + (IPTR) parDesc.dsc_address;

			if (toExternal)
			{
				desc.dsc_address = dsql_msg_buf + (IPTR) desc.dsc_address;

				if (!flag || *flag >= 0)
					MOVD_move(&parDesc, &desc);
				else
					memset(desc.dsc_address, 0, desc.dsc_length);
			}
			else if (!flag || *flag >= 0)
			{
				if (!(parDesc.dsc_flags & DSC_null))
				{
					// Safe cast because desc is used as source only.
					desc.dsc_address = const_cast<UCHAR*>(in_dsql_msg_buf) + (IPTR) desc.dsc_address;
					MOVD_move(&desc, &parDesc);
				}
			}
			else
				memset(parDesc.dsc_address, 0, parDesc.dsc_length);

			count--;
		}
	}

	// If we got here because the loop was exited early or if part of the
	// message given to us hasn't been used, complain.

	if (err || count)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-804) <<
				  Arg::Gds(isc_dsql_sqlda_err));
	}

	const DsqlCompiledStatement* statement = request->getStatement();
	const dsql_par* parameter;

	const dsql_par* dbkey;
	if (!toExternal && (dbkey = statement->getParentDbKey()) &&
		(parameter = statement->getDbKey()))
	{
		UCHAR* parentMsgBuffer = statement->getParentRequest() ?
			statement->getParentRequest()->req_msg_buffers[dbkey->par_message->msg_buffer_number] : NULL;
		UCHAR* msgBuffer = request->req_msg_buffers[parameter->par_message->msg_buffer_number];

		fb_assert(parentMsgBuffer);

		dsc parentDesc = dbkey->par_desc;
		parentDesc.dsc_address = parentMsgBuffer + (IPTR) parentDesc.dsc_address;

		dsc desc = parameter->par_desc;
		desc.dsc_address = msgBuffer + (IPTR) desc.dsc_address;

		MOVD_move(&parentDesc, &desc);

		dsql_par* null_ind = parameter->par_null;
		if (null_ind != NULL)
		{
			desc = null_ind->par_desc;
			desc.dsc_address = msgBuffer + (IPTR) desc.dsc_address;

			SSHORT* flag = (SSHORT*) desc.dsc_address;
			*flag = 0;
		}
	}

	const dsql_par* rec_version;
	if (!toExternal && (rec_version = statement->getParentRecVersion()) &&
		(parameter = statement->getRecVersion()))
	{
		UCHAR* parentMsgBuffer = statement->getParentRequest() ?
			statement->getParentRequest()->req_msg_buffers[rec_version->par_message->msg_buffer_number] :
			NULL;
		UCHAR* msgBuffer = request->req_msg_buffers[parameter->par_message->msg_buffer_number];

		fb_assert(parentMsgBuffer);

		dsc parentDesc = rec_version->par_desc;
		parentDesc.dsc_address = parentMsgBuffer + (IPTR) parentDesc.dsc_address;

		dsc desc = parameter->par_desc;
		desc.dsc_address = msgBuffer + (IPTR) desc.dsc_address;

		MOVD_move(&parentDesc, &desc);

		dsql_par* null_ind = parameter->par_null;
		if (null_ind != NULL)
		{
			desc = null_ind->par_desc;
			desc.dsc_address = msgBuffer + (IPTR) desc.dsc_address;

			SSHORT* flag = (SSHORT*) desc.dsc_address;
			*flag = 0;
		}
	}
}


/**

 	parse_blr

    @brief	Parse the message of a blr request.


    @param blr_length
    @param blr
    @param msg_length
    @param parameters

 **/
static USHORT parse_blr(dsql_req* request, ULONG blr_length, const UCHAR* blr,
	const ULONG msg_length, const Array<dsql_par*>& parameters_list)
{
	HalfStaticArray<const dsql_par*, 16> parameters;

	for (size_t i = 0; i < parameters_list.getCount(); ++i)
	{
		dsql_par* param = parameters_list[i];

		if (param->par_index)
		{
			if (param->par_index > parameters.getCount())
				parameters.grow(param->par_index);
			fb_assert(!parameters[param->par_index - 1]);
			parameters[param->par_index - 1] = param;
		}
	}

	// If there's no blr length, then the format of the current message buffer
	// is identical to the format of the previous one.

	if (blr_length == 0)
		return parameters.getCount();

	if (*blr != blr_version4 && *blr != blr_version5)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-804) <<
				  Arg::Gds(isc_dsql_sqlda_err));
	}

	blr++;						// skip the blr_version

	if (*blr++ != blr_begin || *blr++ != blr_message)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-804) <<
				  Arg::Gds(isc_dsql_sqlda_err));
	}

	++blr;						// skip the message number
	USHORT count = *blr++;
	count += (*blr++) << 8;
	count /= 2;

	ULONG offset = 0;

	for (USHORT index = 1; index <= count; index++)
	{
		dsc desc;
		desc.dsc_scale = 0;
		desc.dsc_sub_type = 0;
		desc.dsc_flags = 0;

		switch (*blr++)
		{
		case blr_text:
			desc.dsc_dtype = dtype_text;
			desc.dsc_sub_type = ttype_dynamic;
			desc.dsc_length = *blr++;
			desc.dsc_length += (*blr++) << 8;
			break;

		case blr_varying:
			desc.dsc_dtype = dtype_varying;
			desc.dsc_sub_type = ttype_dynamic;
			desc.dsc_length = *blr++ + sizeof(USHORT);
			desc.dsc_length += (*blr++) << 8;
			break;

		case blr_text2:
			desc.dsc_dtype = dtype_text;
			desc.dsc_sub_type = *blr++;
			desc.dsc_sub_type += (*blr++) << 8;
			desc.dsc_length = *blr++;
			desc.dsc_length += (*blr++) << 8;
			break;

		case blr_varying2:
			desc.dsc_dtype = dtype_varying;
			desc.dsc_sub_type = *blr++;
			desc.dsc_sub_type += (*blr++) << 8;
			desc.dsc_length = *blr++ + sizeof(USHORT);
			desc.dsc_length += (*blr++) << 8;
			break;

		case blr_short:
			desc.dsc_dtype = dtype_short;
			desc.dsc_length = sizeof(SSHORT);
			desc.dsc_scale = *blr++;
			break;

		case blr_long:
			desc.dsc_dtype = dtype_long;
			desc.dsc_length = sizeof(SLONG);
			desc.dsc_scale = *blr++;
			break;

		case blr_int64:
			desc.dsc_dtype = dtype_int64;
			desc.dsc_length = sizeof(SINT64);
			desc.dsc_scale = *blr++;
			break;

		case blr_quad:
			desc.dsc_dtype = dtype_quad;
			desc.dsc_length = sizeof(SLONG) * 2;
			desc.dsc_scale = *blr++;
			break;

		case blr_float:
			desc.dsc_dtype = dtype_real;
			desc.dsc_length = sizeof(float);
			break;

		case blr_double:
		case blr_d_float:
			desc.dsc_dtype = dtype_double;
			desc.dsc_length = sizeof(double);
			break;

		case blr_timestamp:
			desc.dsc_dtype = dtype_timestamp;
			desc.dsc_length = sizeof(SLONG) * 2;
			break;

		case blr_sql_date:
			desc.dsc_dtype = dtype_sql_date;
			desc.dsc_length = sizeof(SLONG);
			break;

		case blr_sql_time:
			desc.dsc_dtype = dtype_sql_time;
			desc.dsc_length = sizeof(SLONG);
			break;

		case blr_blob2:
			{
				desc.dsc_dtype = dtype_blob;
				desc.dsc_length = sizeof(ISC_QUAD);
				desc.dsc_sub_type = *blr++;
				desc.dsc_sub_type += (*blr++) << 8;

				USHORT textType = *blr++;
				textType += (*blr++) << 8;
				desc.setTextType(textType);
			}
			break;

		case blr_bool:
			desc.dsc_dtype = dtype_boolean;
			desc.dsc_length = sizeof(UCHAR);
			break;

		default:
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-804) <<
					  Arg::Gds(isc_dsql_sqlda_err));
		}

		USHORT align = type_alignments[desc.dsc_dtype];
		if (align)
			offset = FB_ALIGN(offset, align);

		desc.dsc_address = (UCHAR*)(IPTR) offset;
		offset += desc.dsc_length;

		if (*blr++ != blr_short || *blr++ != 0)
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-804) <<
					  Arg::Gds(isc_dsql_sqlda_err));
		}

		align = type_alignments[dtype_short];
		if (align)
			offset = FB_ALIGN(offset, align);

		const ULONG null_offset = offset;
		offset += sizeof(SSHORT);

		const dsql_par* const parameter = parameters[index - 1];
		fb_assert(parameter);

		// ASF: Older than 2.5 engine hasn't validating strings in DSQL. After this has been
		// implemented in 2.5, selecting a NONE column with UTF-8 attachment charset started
		// failing. The real problem is that the client encodes SQL_TEXT/SQL_VARYING using
		// blr_text/blr_varying (i.e. with the connection charset). I'm reseting the charset
		// here at the server as a way to make older (and not yet changed) client work
		// correctly.
		if (desc.isText())
			desc.setTextType(ttype_none);

		request->req_user_descs.put(parameter, desc);

		dsql_par* null = parameter->par_null;
		if (null)
		{
			desc.clear();
			desc.dsc_dtype = dtype_short;
			desc.dsc_scale = 0;
			desc.dsc_length = sizeof(SSHORT);
			desc.dsc_address = (UCHAR*)(IPTR) null_offset;

			request->req_user_descs.put(null, desc);
		}
	}

	if (*blr++ != (UCHAR) blr_end || offset != msg_length)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-804) <<
				  Arg::Gds(isc_dsql_sqlda_err));
	}

	return count;
}


// Prepare a request for execution. Return SQL status code.
// Note: caller is responsible for pool handling.
static dsql_req* prepareRequest(thread_db* tdbb, dsql_dbb* database, jrd_tra* transaction,
	ULONG stringLength, const TEXT* string, USHORT clientDialect, USHORT parserVersion,
	bool isInternalRequest)
{
	return prepareStatement(tdbb, database, transaction, stringLength, string, clientDialect,
		parserVersion, isInternalRequest);
}


// Prepare a statement for execution. Return SQL status code.
// Note: caller is responsible for pool handling.
static dsql_req* prepareStatement(thread_db* tdbb, dsql_dbb* database, jrd_tra* transaction,
	ULONG string_length, const TEXT* string, USHORT client_dialect, USHORT parser_version,
	bool isInternalRequest)
{
	ISC_STATUS_ARRAY local_status;
	MOVE_CLEAR(local_status, sizeof(local_status));

	TraceDSQLPrepare trace(database->dbb_attachment, string_length, string);

	if (client_dialect > SQL_DIALECT_CURRENT)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-901) <<
				  Arg::Gds(isc_wish_list));
	}

	if (!string)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  // Unexpected end of command
				  // CVC: Nothing will be line 1, column 1 for the user.
				  Arg::Gds(isc_command_end_err2) << Arg::Num(1) << Arg::Num(1));
	}

	if (string_length == 0)
		string_length = strlen(string);

	// Get rid of the trailing ";" if there is one.

	for (const TEXT* p = string + string_length; p-- > string;)
	{
		if (*p != ' ')
		{
			if (*p == ';')
				string_length = p - string;
			break;
		}
	}

	// allocate the statement block, then prepare the statement

	Jrd::ContextPoolHolder context(tdbb, database->createPool());
	MemoryPool& pool = *tdbb->getDefaultPool();

	DsqlCompiledStatement* statement = FB_NEW(pool) DsqlCompiledStatement(pool);
	DsqlCompilerScratch* scratch = FB_NEW(pool) DsqlCompilerScratch(pool, database,
		transaction, statement);
	scratch->clientDialect = client_dialect;

	dsql_req* request = FB_NEW(statement->getPool()) dsql_req(statement);
	request->addRef();
	request->req_dbb = database;
	request->req_transaction = transaction;

	request->req_traced = true;
	trace.setStatement(request);

	try {

	// Parse the SQL statement.  If it croaks, return

	Parser parser(*tdbb->getDefaultPool(), client_dialect,
		scratch->getAttachment()->dbb_db_SQL_dialect, parser_version, string, string_length,
		tdbb->getAttachment()->att_charset);

	dsql_nod* node = parser.parse();
	Firebird::string transformedText = parser.getTransformedString();
	SSHORT charSetId = database->dbb_attachment->att_charset;

	// If the attachment charset is NONE, replace non-ASCII characters by question marks, so that
	// engine internals doesn't receive non-mappeable data to UTF8. If an attachment charset is
	// used, validate the string.
	if (charSetId == CS_NONE)
	{
		for (char* p = transformedText.begin(), *end = transformedText.end(); p < end; ++p)
		{
			if (UCHAR(*p) > 0x7F)
				*p = '?';
		}
	}
	else
	{
		CharSet* charSet = INTL_charset_lookup(tdbb, charSetId);

		if (!charSet->wellFormed(transformedText.length(), (const UCHAR*) transformedText.begin(), NULL))
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					  Arg::Gds(isc_malformed_string));
		}

		UCharBuffer temp;

		CsConvert conversor(charSet->getStruct(),
			INTL_charset_lookup(tdbb, CS_METADATA)->getStruct());
		conversor.convert(transformedText.length(), (const UCHAR*) transformedText.c_str(), temp);

		transformedText.assign(temp.begin(), temp.getCount());
	}

	statement->setSqlText(FB_NEW(pool) RefString(pool, transformedText));

	if (!node)
	{
		// CVC: Apparently, Parser::parse won't return if the command is incomplete,
		// because yyerror() will call ERRD_post().
		// This may be a special case, but we don't know about positions here.
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  // Unexpected end of command
				  Arg::Gds(isc_command_end_err));
	}

	// allocate the send and receive messages

	statement->setSendMsg(FB_NEW(pool) dsql_msg(pool));
	dsql_msg* message = FB_NEW(pool) dsql_msg(pool);
	statement->setReceiveMsg(message);
	message->msg_number = 1;

	statement->setType(DsqlCompiledStatement::TYPE_SELECT);

	// No work is done during pass1 for set transaction - like
	// checking for valid table names.  This is because that will
	// require a valid transaction handle.
	// Error will be caught at execute time.

	node = PASS1_statement(scratch, node);
	if (!node)
		return request;

	switch (statement->getType())
	{
	case DsqlCompiledStatement::TYPE_COMMIT:
	case DsqlCompiledStatement::TYPE_COMMIT_RETAIN:
	case DsqlCompiledStatement::TYPE_ROLLBACK:
	case DsqlCompiledStatement::TYPE_ROLLBACK_RETAIN:
	case DsqlCompiledStatement::TYPE_GET_SEGMENT:
	case DsqlCompiledStatement::TYPE_PUT_SEGMENT:
	case DsqlCompiledStatement::TYPE_START_TRANS:
		request->req_traced = false;
		break;
	default:
		request->req_traced = true;
	}

	// stop here for statements not requiring code generation

	if (statement->getType() == DsqlCompiledStatement::TYPE_DDL && parser.isStmtAmbiguous() &&
		scratch->getAttachment()->dbb_db_SQL_dialect != client_dialect)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-817) <<
				  Arg::Gds(isc_ddl_not_allowed_by_db_sql_dial) <<
					  Arg::Num(scratch->getAttachment()->dbb_db_SQL_dialect));
	}

	switch (statement->getType())
	{
		// Work on blob segment statements
		case DsqlCompiledStatement::TYPE_GET_SEGMENT:
		case DsqlCompiledStatement::TYPE_PUT_SEGMENT:
			GEN_port(scratch, statement->getBlob()->blb_open_in_msg);
			GEN_port(scratch, statement->getBlob()->blb_open_out_msg);
			GEN_port(scratch, statement->getBlob()->blb_segment_msg);
			break;

		case DsqlCompiledStatement::TYPE_DDL:
			statement->setDdlScratch(scratch);
			break;
	}

	switch (statement->getType())
	{
		case DsqlCompiledStatement::TYPE_COMMIT:
		case DsqlCompiledStatement::TYPE_COMMIT_RETAIN:
		case DsqlCompiledStatement::TYPE_ROLLBACK:
		case DsqlCompiledStatement::TYPE_ROLLBACK_RETAIN:
			return request;

		case DsqlCompiledStatement::TYPE_START_TRANS:
			GEN_start_transaction(scratch, node);
			return request;

		default:
			if (client_dialect > SQL_DIALECT_V5)
				statement->addFlags(DsqlCompiledStatement::FLAG_BLR_VERSION5);
			else
				statement->addFlags(DsqlCompiledStatement::FLAG_BLR_VERSION4);

			GEN_request(scratch, node);
			// fall into

		case DsqlCompiledStatement::TYPE_GET_SEGMENT:
		case DsqlCompiledStatement::TYPE_PUT_SEGMENT:
			{
				// Create the messages buffers
				for (size_t i = 0; i < scratch->ports.getCount(); ++i)
				{
					dsql_msg* message = scratch->ports[i];

					// Allocate buffer for message
					const ULONG newLen = message->msg_length + FB_DOUBLE_ALIGN - 1;

					message->msg_buffer_number = request->req_msg_buffers.getCount();
					request->req_msg_buffers.grow(message->msg_buffer_number + 1);

					UCHAR*& msgBuffer = request->req_msg_buffers[message->msg_buffer_number];
					fb_assert(!msgBuffer);

					msgBuffer = FB_NEW(*tdbb->getDefaultPool()) UCHAR[newLen];
					msgBuffer = (UCHAR*) FB_ALIGN((U_IPTR) msgBuffer, FB_DOUBLE_ALIGN);
				}
				break;
			}
	}

	switch (statement->getType())
	{
		case DsqlCompiledStatement::TYPE_GET_SEGMENT:
		case DsqlCompiledStatement::TYPE_PUT_SEGMENT:
			return request;
	}

	const ULONG length = (ULONG) scratch->getBlrData().getCount();

	// stop here for ddl statements

	if (statement->getType() == DsqlCompiledStatement::TYPE_CREATE_DB ||
		statement->getType() == DsqlCompiledStatement::TYPE_DDL)
	{
		// Notify Trace API manager about new DDL request cooked.
		trace.prepare(res_successful);
		return request;
	}

	// have the access method compile the statement

#ifdef DSQL_DEBUG
	if (DSQL_debug & 64)
	{
		dsql_trace("Resulting BLR code for DSQL:");
		gds__trace_raw("Statement:\n");
		gds__trace_raw(string, string_length);
		gds__trace_raw("\nBLR:\n");
		fb_print_blr(scratch->getBlrData().begin(),
			(ULONG) scratch->getBlrData().getCount(),
			gds__trace_printer, 0, 0);
	}
#endif

	// check for warnings
	if (tdbb->tdbb_status_vector[2] == isc_arg_warning)
	{
		// save a status vector
		memcpy(local_status, tdbb->tdbb_status_vector, sizeof(ISC_STATUS_ARRAY));
	}

	ISC_STATUS status = FB_SUCCESS;

	try
	{
		JRD_compile(tdbb,
					scratch->getAttachment()->dbb_attachment,
					&request->req_request,
					length,
					scratch->getBlrData().begin(),
					statement->getSqlText(),
					scratch->getDebugData().getCount(),
					scratch->getDebugData().begin(),
					isInternalRequest);
	}
	catch (const Firebird::Exception&)
	{
		status = tdbb->tdbb_status_vector[1];
		trace.prepare(status == isc_no_priv ? res_unauthorized : res_failed);
	}

	// restore warnings (if there are any)
	if (local_status[2] == isc_arg_warning)
	{
		size_t indx, len, warning;

		// find end of a status vector
		PARSE_STATUS(tdbb->tdbb_status_vector, indx, warning);
		if (indx)
			--indx;

		// calculate length of saved warnings
		PARSE_STATUS(local_status, len, warning);
		len -= 2;

		if ((len + indx - 1) < ISC_STATUS_LENGTH)
			memcpy(&tdbb->tdbb_status_vector[indx], &local_status[2], sizeof(ISC_STATUS) * len);
	}

	// free blr memory
	scratch->getBlrData().free();

	if (status)
		Firebird::status_exception::raise(tdbb->tdbb_status_vector);

	// Notify Trace API manager about new request cooked.
	trace.prepare(res_successful);

	return request;

	}
	catch (const Firebird::Exception&)
	{
		trace.prepare(res_failed);
		request->req_traced = false;
		dsql_req::destroy(tdbb, request, true);
		throw;
	}
}


/**

 	put_item

    @brief	Put information item in output buffer if there is room, and
 	return an updated pointer.  If there isn't room for the item,
 	indicate truncation and return NULL.


    @param item
    @param length
    @param string
    @param ptr
    @param end

 **/
static UCHAR* put_item(	UCHAR	item,
						const USHORT	length,
						const UCHAR* string,
						UCHAR*	ptr,
						const UCHAR* const end)
{
	if (ptr + length + 3 >= end)
	{
		*ptr = isc_info_truncated;
		return NULL;
	}

	*ptr++ = item;

	*ptr++ = (UCHAR) length;
	*ptr++ = length >> 8;

	if (length)
		memcpy(ptr, string, length);

	return ptr + length;
}


// Release a compiled statement.
static void release_statement(DsqlCompiledStatement* statement)
{
	if (statement->getParentRequest())
	{
		dsql_req* parent = statement->getParentRequest();

		size_t pos;
		if (parent->cursors.find(statement, pos))
			parent->cursors.remove(pos);

		statement->setParentRequest(NULL);
	}

	statement->setSqlText(NULL);
	statement->getDdlData().free();	// free blr memory
}


/**

 	dsql_req::destroy

    @brief	Release a dynamic request.


    @param request
    @param top_level

 **/
void dsql_req::destroy(thread_db* tdbb, dsql_req* request, bool drop)
{
	SET_TDBB(tdbb);

	// If request is parent, orphan the children and release a portion of their requests

	for (size_t i = 0; i < request->cursors.getCount(); ++i)
	{
		DsqlCompiledStatement* child = request->cursors[i];
		child->addFlags(DsqlCompiledStatement::FLAG_ORPHAN);
		child->setParentRequest(NULL);

		Jrd::ContextPoolHolder context(tdbb, &child->getPool());
		release_statement(child);
	}

	// For requests that are linked to a parent, unlink it

	const DsqlCompiledStatement* statement = request->getStatement();

	release_statement(const_cast<DsqlCompiledStatement*>(statement));

	// If the request had an open cursor, close it

	if (request->req_flags & dsql_req::FLAG_OPENED_CURSOR) {
		close_cursor(tdbb, request);
	}

	Jrd::Attachment* att = request->req_dbb->dbb_attachment;
	const bool need_trace_free = request->req_traced && TraceManager::need_dsql_free(att);
	if (need_trace_free)
	{
		TraceSQLStatementImpl stmt(request, NULL);
		TraceManager::event_dsql_free(att, &stmt, DSQL_drop);
	}
	request->req_traced = false;

	if (request->req_cursor.hasData())
	{
		request->req_dbb->dbb_cursors.remove(request->req_cursor);
		request->req_cursor = "";
	}

	// If a request has been compiled, release it now

	if (request->req_request)
	{
		ThreadStatusGuard status_vector(tdbb);

		try
		{
			CMP_release(tdbb, request->req_request);
			request->req_request = NULL;
		}
		catch (Firebird::Exception&)
		{
		}
	}

	// Release the entire request if explicitly asked for

	if (drop)
	{
		--(request->refCounter);
		request->req_dbb->deletePool(&request->getPool());
	}
}


/**

	sql_info

	@brief	Return DSQL information buffer.

	@param request
	@param item_length
	@param items
	@param info_length
	@param info

 **/

static void sql_info(thread_db* tdbb,
					 dsql_req* request,
					 ULONG item_length,
					 const UCHAR* items,
					 ULONG info_length,
					 UCHAR* info)
{
	if (!item_length || !items || !info_length || !info)
		return;

	UCHAR buffer[BUFFER_SMALL];
	memset(buffer, 0, sizeof(buffer));

	// Pre-initialize buffer. This is necessary because we don't want to transfer rubbish over the wire
	memset(info, 0, info_length);

	const UCHAR* const end_items = items + item_length;
	const UCHAR* const end_info = info + info_length;
	UCHAR *start_info;

	if (*items == isc_info_length)
	{
		start_info = info;
		items++;
	}
	else
		start_info = NULL;

	// CVC: Is it the idea that this pointer remains with its previous value
	// in the loop or should it be made NULL in each iteration?
	const dsql_msg* message = NULL;
	bool messageFound = false;
	USHORT first_index = 0;

	const DsqlCompiledStatement* statement = request->getStatement();

	while (items < end_items && *items != isc_info_end)
	{
		ULONG length;
		USHORT number;
		const UCHAR item = *items++;

		switch (item)
		{
		case isc_info_sql_select:
		case isc_info_sql_bind:
			message = (item == isc_info_sql_select) ?
				statement->getReceiveMsg() : statement->getSendMsg();
			messageFound = true;
			if (info + 1 >= end_info)
			{
				*info = isc_info_truncated;
				return;
			}
			*info++ = item;
			break;

		case isc_info_sql_stmt_type:
			switch (statement->getType())
			{
			case DsqlCompiledStatement::TYPE_SELECT:
				number = isc_info_sql_stmt_select;
				break;
			case DsqlCompiledStatement::TYPE_SELECT_UPD:
				number = isc_info_sql_stmt_select_for_upd;
				break;
			case DsqlCompiledStatement::TYPE_CREATE_DB:
			case DsqlCompiledStatement::TYPE_DDL:
				number = isc_info_sql_stmt_ddl;
				break;
			case DsqlCompiledStatement::TYPE_GET_SEGMENT:
				number = isc_info_sql_stmt_get_segment;
				break;
			case DsqlCompiledStatement::TYPE_PUT_SEGMENT:
				number = isc_info_sql_stmt_put_segment;
				break;
			case DsqlCompiledStatement::TYPE_COMMIT:
			case DsqlCompiledStatement::TYPE_COMMIT_RETAIN:
				number = isc_info_sql_stmt_commit;
				break;
			case DsqlCompiledStatement::TYPE_ROLLBACK:
			case DsqlCompiledStatement::TYPE_ROLLBACK_RETAIN:
				number = isc_info_sql_stmt_rollback;
				break;
			case DsqlCompiledStatement::TYPE_START_TRANS:
				number = isc_info_sql_stmt_start_trans;
				break;
			case DsqlCompiledStatement::TYPE_INSERT:
				number = isc_info_sql_stmt_insert;
				break;
			case DsqlCompiledStatement::TYPE_UPDATE:
			case DsqlCompiledStatement::TYPE_UPDATE_CURSOR:
				number = isc_info_sql_stmt_update;
				break;
			case DsqlCompiledStatement::TYPE_DELETE:
			case DsqlCompiledStatement::TYPE_DELETE_CURSOR:
				number = isc_info_sql_stmt_delete;
				break;
			case DsqlCompiledStatement::TYPE_EXEC_PROCEDURE:
				number = isc_info_sql_stmt_exec_procedure;
				break;
			case DsqlCompiledStatement::TYPE_SET_GENERATOR:
				number = isc_info_sql_stmt_set_generator;
				break;
			case DsqlCompiledStatement::TYPE_SAVEPOINT:
				number = isc_info_sql_stmt_savepoint;
				break;
			case DsqlCompiledStatement::TYPE_EXEC_BLOCK:
				number = isc_info_sql_stmt_exec_procedure;
				break;
			case DsqlCompiledStatement::TYPE_SELECT_BLOCK:
				number = isc_info_sql_stmt_select;
				break;
			default:
				number = 0;
				break;
			}
			length = put_vax_long(buffer, (SLONG) number);
			info = put_item(item, length, buffer, info, end_info);
			if (!info)
				return;
			break;

		case isc_info_sql_sqlda_start:
			length = *items++;
			first_index = static_cast<USHORT>(gds__vax_integer(items, length));
			items += length;
			break;

		case isc_info_sql_batch_fetch:
			if (statement->getFlags() & DsqlCompiledStatement::FLAG_NO_BATCH)
				number = 0;
			else
				number = 1;
			length = put_vax_long(buffer, (SLONG) number);
			if (!(info = put_item(item, length, buffer, info, end_info)))
				return;
			break;

		case isc_info_sql_records:
			length = get_request_info(tdbb, request, sizeof(buffer), buffer);
			if (length && !(info = put_item(item, length, buffer, info, end_info)))
				return;
			break;

		case isc_info_sql_get_plan:
		case isc_info_sql_explain_plan:
			{
				const bool detailed = (item == isc_info_sql_explain_plan);
				string plan = OPT_get_plan(tdbb, request->req_request, detailed);

				if (plan.hasData())
				{
					const ULONG buffer_length = end_info - info - 3; // 1-byte item + 2-byte length
					const ULONG max_length = MIN(buffer_length, MAX_USHORT);

					if (plan.length() > max_length)
					{
						// If the plan doesn't fit the supplied buffer or exceeds the API limits,
						// truncate it to the rightmost space and add ellipsis to the end
						plan.resize(max_length);

						while (plan.length() > max_length - 4)
						{
							const size_t pos = plan.find_last_of(' ');
							if (pos == string::npos)
								break;
							plan.resize(pos);
						}

						plan += " ...";
					}

					if (plan.length() > max_length)
					{
						// If it's still too long, give up
						fb_assert(info < end_info);
						*info = isc_info_truncated;
						info = NULL;
					}
					else
					{
						info = put_item(item, plan.length(), reinterpret_cast<const UCHAR*>(plan.c_str()),
										info, end_info);
					}
				}

				if (!info)
					return;
			}
			break;

		case isc_info_sql_num_variables:
		case isc_info_sql_describe_vars:
			if (messageFound)
			{
				number = message ? message->msg_index : 0;
				length = put_vax_long(buffer, (SLONG) number);
				if (!(info = put_item(item, length, buffer, info, end_info)))
					return;
				if (item == isc_info_sql_num_variables)
					continue;

				const UCHAR* end_describe = items;
				while (end_describe < end_items &&
					*end_describe != isc_info_end && *end_describe != isc_info_sql_describe_end)
				{
					end_describe++;
				}

				info = var_info(message, items, end_describe, info, end_info, first_index,
					message == statement->getSendMsg());
				if (!info)
					return;

				items = end_describe;
				if (*items == isc_info_sql_describe_end)
					items++;
				break;
			}
			// else fall into

		default:
			buffer[0] = item;
			length = 1 + put_vax_long(buffer + 1, (SLONG) isc_infunk);
			if (!(info = put_item(isc_info_error, length, buffer, info, end_info)))
				return;
		}
	}

	*info++ = isc_info_end;

	if (start_info && (end_info - info >= 7))
	{
		const SLONG number = info - start_info;
		fb_assert(number > 0);
		memmove(start_info + 7, start_info, number);
		USHORT length = put_vax_long(buffer, number);
		fb_assert(length == 4); // We only accept SLONG
		put_item(isc_info_length, length, buffer, start_info, end_info);
	}
}


/**

 	var_info

    @brief	Provide information on an internal message.


    @param message
    @param items
    @param end_describe
    @param info
    @param end
    @param first_index

 **/
static UCHAR* var_info(const dsql_msg* message,
					   const UCHAR* items,
					   const UCHAR* const end_describe,
					   UCHAR* info,
					   const UCHAR* const end,
					   USHORT first_index,
					   bool input_message)
{
	if (!message || !message->msg_index)
		return info;

	thread_db* tdbb = JRD_get_thread_data();
	Jrd::Attachment* attachment = tdbb->getAttachment();

	HalfStaticArray<const dsql_par*, 16> parameters;

	for (size_t i = 0; i < message->msg_parameters.getCount(); ++i)
	{
		const dsql_par* param = message->msg_parameters[i];

		if (param->par_index)
		{
			if (param->par_index > parameters.getCount())
				parameters.grow(param->par_index);
			fb_assert(!parameters[param->par_index - 1]);
			parameters[param->par_index - 1] = param;
		}
	}

	UCHAR buf[128];

	for (size_t i = 0; i < parameters.getCount(); i++)
	{
		const dsql_par* param = parameters[i];
		fb_assert(param);

		if (param->par_index >= first_index)
		{
			SLONG sql_len = param->par_desc.dsc_length;
			SLONG sql_sub_type = 0;
			SLONG sql_scale = 0;
			SLONG sql_type = 0;

			switch (param->par_desc.dsc_dtype)
			{
			case dtype_real:
				sql_type = SQL_FLOAT;
				break;
			case dtype_array:
				sql_type = SQL_ARRAY;
				break;

			case dtype_timestamp:
				sql_type = SQL_TIMESTAMP;
				break;
			case dtype_sql_date:
				sql_type = SQL_TYPE_DATE;
				break;
			case dtype_sql_time:
				sql_type = SQL_TYPE_TIME;
				break;

			case dtype_double:
				sql_type = SQL_DOUBLE;
				sql_scale = param->par_desc.dsc_scale;
				break;

			case dtype_text:
				if (input_message && (param->par_desc.dsc_flags & DSC_null))
				{
					sql_type = SQL_NULL;
					sql_len = 0;
				}
				else
				{
					sql_type = SQL_TEXT;
					sql_sub_type = param->par_desc.dsc_sub_type;
				}
				break;

			case dtype_blob:
				sql_type = SQL_BLOB;
				sql_sub_type = param->par_desc.dsc_sub_type;
				sql_scale = param->par_desc.dsc_scale;
				break;

			case dtype_varying:
				sql_type = SQL_VARYING;
				sql_len -= sizeof(USHORT);
				sql_sub_type = param->par_desc.dsc_sub_type;
				break;

			case dtype_short:
			case dtype_long:
			case dtype_int64:
				switch (param->par_desc.dsc_dtype)
				{
				case dtype_short:
					sql_type = SQL_SHORT;
					break;
				case dtype_long:
					sql_type = SQL_LONG;
					break;
				default:
					sql_type = SQL_INT64;
				}
				sql_scale = param->par_desc.dsc_scale;
				if (param->par_desc.dsc_sub_type)
					sql_sub_type = param->par_desc.dsc_sub_type;
				break;

			case dtype_quad:
				sql_type = SQL_QUAD;
				sql_scale = param->par_desc.dsc_scale;
				break;

			case dtype_boolean:
				sql_type = SQL_BOOLEAN;
				break;

			default:
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-804) <<
						  Arg::Gds(isc_dsql_datatype_err));
			}

			if (sql_type && (param->par_desc.dsc_flags & DSC_nullable))
				sql_type++;

			for (const UCHAR* describe = items; describe < end_describe;)
			{
				USHORT length;
				MetaName name;
				const UCHAR* buffer = buf;
				UCHAR item = *describe++;

				switch (item)
				{
				case isc_info_sql_sqlda_seq:
					length = put_vax_long(buf, (SLONG) param->par_index);
					break;

				case isc_info_sql_message_seq:
					length = 0;
					break;

				case isc_info_sql_type:
					length = put_vax_long(buf, (SLONG) sql_type);
					break;

				case isc_info_sql_sub_type:
					length = put_vax_long(buf, (SLONG) sql_sub_type);
					break;

				case isc_info_sql_scale:
					length = put_vax_long(buf, (SLONG) sql_scale);
					break;

				case isc_info_sql_length:
					length = put_vax_long(buf, (SLONG) sql_len);
					break;

				case isc_info_sql_null_ind:
					length = put_vax_long(buf, (SLONG) (sql_type & 1));
					break;

				case isc_info_sql_field:
					if (param->par_name.hasData())
					{
						name = attachment->nameToUserCharSet(tdbb, param->par_name);
						length = name.length();
						buffer = reinterpret_cast<const UCHAR*>(name.c_str());
					}
					else
						length = 0;
					break;

				case isc_info_sql_relation:
					if (param->par_rel_name.hasData())
					{
						name = attachment->nameToUserCharSet(tdbb, param->par_rel_name);
						length = name.length();
						buffer = reinterpret_cast<const UCHAR*>(name.c_str());
					}
					else
						length = 0;
					break;

				case isc_info_sql_owner:
					if (param->par_owner_name.hasData())
					{
						name = attachment->nameToUserCharSet(tdbb, param->par_owner_name);
						length = name.length();
						buffer = reinterpret_cast<const UCHAR*>(name.c_str());
					}
					else
						length = 0;
					break;

				case isc_info_sql_relation_alias:
					if (param->par_rel_alias.hasData())
					{
						name = attachment->nameToUserCharSet(tdbb, param->par_rel_alias);
						length = name.length();
						buffer = reinterpret_cast<const UCHAR*>(name.c_str());
					}
					else
						length = 0;
					break;

				case isc_info_sql_alias:
					if (param->par_alias.hasData())
					{
						name = attachment->nameToUserCharSet(tdbb, param->par_alias);
						length = name.length();
						buffer = reinterpret_cast<const UCHAR*>(name.c_str());
					}
					else
						length = 0;
					break;

				default:
					buf[0] = item;
					item = isc_info_error;
					length = 1 + put_vax_long(buf + 1, (SLONG) isc_infunk);
					break;
				}

				if (!(info = put_item(item, length, buffer, info, end)))
					return info;
			}

			if (info + 1 >= end)
			{
				*info = isc_info_truncated;
				return NULL;
			}
			*info++ = isc_info_sql_describe_end;
		} // if()
	} // for()

	return info;
}
