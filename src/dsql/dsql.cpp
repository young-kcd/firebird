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
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2004.01.16 Vlad Horsun: added support for EXECUTE BLOCK statement
 */
/**************************************************************
V4 Multi-threading changes.

-- direct calls to gds__ () & isc_ () entrypoints

	THREAD_EXIT();
	    gds__ () or isc_ () call.
	THREAD_ENTER();

-- calls through embedded GDML.

the following protocol will be used.  Care should be taken if
nested FOR loops are added.

    THREAD_EXIT();                // last statement before FOR loop

    FOR ...............

	THREAD_ENTER();           // First statement in FOR loop
	.....some C code....
	.....some C code....
	THREAD_EXIT();            // last statement in FOR loop

    END_FOR;

    THREAD_ENTER();               // First statement after FOR loop
***************************************************************/

#include "firebird.h"
#include "fb_exception.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../dsql/dsql.h"
#include "../jrd/ibase.h"
#include "../jrd/thd.h"
#include "../jrd/align.h"
#include "../jrd/intl.h"
#include "../jrd/iberr.h"
//#include "../dsql/sqlda.h"
#include "../dsql/alld_proto.h"
#include "../dsql/ddl_proto.h"
#include "../dsql/dsql_proto.h"
#include "../dsql/errd_proto.h"
#include "../dsql/gen_proto.h"
#include "../dsql/hsh_proto.h"
#include "../dsql/make_proto.h"
#include "../dsql/movd_proto.h"
#include "../dsql/parse_proto.h"
#include "../dsql/pass1_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thread_proto.h"
#include "../jrd/why_proto.h"
#include "../jrd/y_handle.h"
#include "../common/config/config.h"
#include "../common/utils_proto.h"

#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif

#ifdef VMS
#include <descrip.h>
#endif

static void		cleanup(void*);
static void		cleanup_database(FB_API_HANDLE*, void*);
static void		cleanup_transaction(FB_API_HANDLE, void*);
static void		close_cursor(dsql_req*);
static USHORT	convert(SLONG, UCHAR*);
static ISC_STATUS	error();
static void		execute_blob(dsql_req*, USHORT, const UCHAR*, USHORT, UCHAR*,
						 USHORT, UCHAR*, USHORT, UCHAR*);
static ISC_STATUS	execute_request(dsql_req*, FB_API_HANDLE*, USHORT, const UCHAR*,
	USHORT, UCHAR*, USHORT, UCHAR*, USHORT, UCHAR*, bool);
static SSHORT	filter_sub_type(dsql_req*, const dsql_nod*);
static bool		get_indices(SSHORT*, const SCHAR**, SSHORT*, SCHAR**);
static USHORT	get_plan_info(dsql_req*, SSHORT, SCHAR**);
static USHORT	get_request_info(dsql_req*, SSHORT, UCHAR*);
static bool		get_rsb_item(SSHORT*, const SCHAR**, SSHORT*, SCHAR**, USHORT*,
							USHORT*);
static dsql_dbb*	init(FB_API_HANDLE*);
static void		map_in_out(dsql_req*, dsql_msg*, USHORT, const UCHAR*, USHORT, UCHAR*);
static USHORT	parse_blr(USHORT, const UCHAR*, const USHORT, dsql_par*);
static dsql_req*		prepare(dsql_req*, USHORT, const TEXT*, USHORT, USHORT);
static void		punt(void);
static UCHAR*	put_item(UCHAR, USHORT, const UCHAR*, UCHAR*, const UCHAR* const);
static void		release_request(dsql_req*, bool);
static ISC_STATUS	return_success(void);
static UCHAR*	var_info(dsql_msg*, const UCHAR*, const UCHAR* const, UCHAR*,
	const UCHAR* const, USHORT);

extern dsql_nod* DSQL_parse;

#ifdef DSQL_DEBUG
	unsigned DSQL_debug;
#endif

static bool init_flag = false;
static dsql_dbb* databases;
static dsql_opn* open_cursors;

static const SCHAR db_hdr_info_items[] = {
	isc_info_db_sql_dialect,
	isc_info_ods_version,
	isc_info_base_level,
	isc_info_db_read_only,
	frb_info_att_charset,
	isc_info_end
};

static const SCHAR explain_info[] = {
	isc_info_access_path
};

static const SCHAR record_info[] = {
	isc_info_req_update_count, isc_info_req_delete_count,
	isc_info_req_select_count, isc_info_req_insert_count
};

static const UCHAR sql_records_info[] = {
	isc_info_sql_records
};

#ifdef	ANY_THREADING
static MUTX_T databases_mutex;
static MUTX_T cursors_mutex;
#endif


#ifdef DSQL_DEBUG
IMPLEMENT_TRACE_ROUTINE(dsql_trace, "DSQL")
#endif

//////////////////////////////////////////////////////////////////
// declarations of the C++ implementations of the C API functions
// with the matching name.
//////////////////////////////////////////////////////////////////

static
ISC_STATUS GDS_DSQL_ALLOCATE_CPP(	ISC_STATUS*	user_status,
								FB_API_HANDLE*	db_handle,
								dsql_req**	req_handle);

static
ISC_STATUS GDS_DSQL_EXECUTE_CPP(   ISC_STATUS*			user_status,
							   FB_API_HANDLE*			trans_handle,
							   dsql_req**		req_handle,
							   USHORT			in_blr_length,
							   const UCHAR*			in_blr,
							   USHORT			in_msg_type,
							   USHORT			in_msg_length,
							   UCHAR*			in_msg,
							   USHORT			out_blr_length,
							   UCHAR*			out_blr,
							   USHORT			out_msg_type,
							   USHORT			out_msg_length,
							   UCHAR*			out_msg);

static
ISC_STATUS GDS_DSQL_FETCH_CPP(	ISC_STATUS*		user_status,
							 dsql_req**		req_handle,
							 USHORT		blr_length,
							 const UCHAR*		blr,
							 USHORT		msg_type,
							 USHORT		msg_length,
							 UCHAR*		dsql_msg_buf
#ifdef SCROLLABLE_CURSORS
							 , USHORT direction, SLONG offset);
#else
							);
#endif

static
ISC_STATUS GDS_DSQL_FREE_CPP(ISC_STATUS*	user_status,
						 dsql_req**		req_handle,
						 USHORT		option);

static
ISC_STATUS GDS_DSQL_INSERT_CPP(	ISC_STATUS*	user_status,
							dsql_req**	req_handle,
							USHORT	blr_length,
							const UCHAR*	blr,
							USHORT	msg_type,
							USHORT	msg_length,
							const UCHAR*	dsql_msg_buf);

static
ISC_STATUS GDS_DSQL_PREPARE_CPP(ISC_STATUS*			user_status,
							FB_API_HANDLE*		trans_handle,
							dsql_req**		req_handle,
							USHORT			length,
							const TEXT*			string,
							USHORT			dialect,
							USHORT			item_length,
							const UCHAR*			items,
							USHORT			buffer_length,
							UCHAR*			buffer);

static
ISC_STATUS GDS_DSQL_SQL_INFO_CPP(	ISC_STATUS*		user_status,
								dsql_req**		req_handle,
								USHORT		item_length,
								const UCHAR*	items,
								USHORT		info_length,
								UCHAR*		info);

ISC_STATUS GDS_DSQL_SET_CURSOR_CPP(	ISC_STATUS*		user_status,
								dsql_req**		req_handle,
								const TEXT*		input_cursor,
								USHORT		type);


//////////////////////////////////////////////////////////////////
// Begin : C API -> C++ wrapper functions
// Note that we don't wrap dsql8_execute_immediate since it
// contains no req in its parameter list.
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////

ISC_STATUS	dsql8_allocate_statement(
				ISC_STATUS*    user_status,
				FB_API_HANDLE*    db_handle,
				dsql_req** req_handle)
{
	return GDS_DSQL_ALLOCATE_CPP(user_status, db_handle, req_handle);
}

//////////////////////////////////////////////////////////////////

ISC_STATUS	dsql8_execute(
				ISC_STATUS*    user_status,
				FB_API_HANDLE*   trans_handle,
				dsql_req** req_handle,
				USHORT     in_blr_length,
				const SCHAR*     in_blr,
				USHORT     in_msg_type,
				USHORT     in_msg_length,
				SCHAR*     in_msg,
				USHORT     out_blr_length,
				SCHAR*     out_blr,
				USHORT     out_msg_type,
				USHORT     out_msg_length,
				SCHAR*     out_msg)
{
	return GDS_DSQL_EXECUTE_CPP(user_status,
								trans_handle,
								req_handle,
								in_blr_length,
								reinterpret_cast<const UCHAR*>(in_blr),
								in_msg_type,
								in_msg_length,
								reinterpret_cast<UCHAR*>(in_msg),
								out_blr_length,
								reinterpret_cast<UCHAR*>(out_blr),
								out_msg_type,
								out_msg_length,
								reinterpret_cast<UCHAR*>(out_msg));
}

//////////////////////////////////////////////////////////////////

ISC_STATUS	dsql8_fetch(
				ISC_STATUS*    user_status,
				dsql_req** req_handle,
				USHORT     blr_length,
				const SCHAR*     blr,
				USHORT     msg_type,
				USHORT     msg_length,
				SCHAR*     dsql_msg_buf
#ifdef SCROLLABLE_CURSORS
				, USHORT direction, SLONG offset
#endif
				)
{
	return GDS_DSQL_FETCH_CPP(	user_status,
								req_handle,
								blr_length,
								reinterpret_cast<const UCHAR*>(blr),
								msg_type,
								msg_length,
								reinterpret_cast<UCHAR*>(dsql_msg_buf)
#ifdef SCROLLABLE_CURSORS
								 , direction, offset
#endif
								);
}

//////////////////////////////////////////////////////////////////

ISC_STATUS	dsql8_free_statement(
				ISC_STATUS*    user_status,
				dsql_req** req_handle,
				USHORT     option)
{
	return GDS_DSQL_FREE_CPP(user_status, req_handle, option);
}

//////////////////////////////////////////////////////////////////

ISC_STATUS	dsql8_insert(
				ISC_STATUS*				user_status,
				dsql_req**			req_handle,
				USHORT				blr_length,
				const SCHAR*				blr,
				USHORT				msg_type,
				USHORT				msg_length,
				const SCHAR*				dsql_msg_buf)
{
	return GDS_DSQL_INSERT_CPP(	user_status,
								req_handle,
								blr_length,
								reinterpret_cast<const UCHAR*>(blr),
								msg_type,
								msg_length,
								reinterpret_cast<const UCHAR*>(dsql_msg_buf));
}

//////////////////////////////////////////////////////////////////

ISC_STATUS	dsql8_prepare(
				ISC_STATUS*				user_status,
				FB_API_HANDLE*			trans_handle,
				dsql_req**	req_handle,
				USHORT				length,
				const TEXT*				string,
				USHORT				dialect,
				USHORT				item_length,
				const SCHAR*		items,
				USHORT				buffer_length,
				SCHAR*				buffer)
{
	return GDS_DSQL_PREPARE_CPP(user_status,
								trans_handle,
								req_handle,
								length,
								string,
								dialect,
								item_length,
								reinterpret_cast<const UCHAR*>(items),
								buffer_length,
								reinterpret_cast<UCHAR*>(buffer));

}

//////////////////////////////////////////////////////////////////

ISC_STATUS	dsql8_sql_info(
				ISC_STATUS*				user_status,
				dsql_req**			req_handle,
				USHORT				item_length,
				const SCHAR*		items,
				USHORT				info_length,
				SCHAR*				info)
{
	return GDS_DSQL_SQL_INFO_CPP(	user_status,
									req_handle,
									item_length,
									reinterpret_cast<const UCHAR*>(items),
									info_length,
									reinterpret_cast<UCHAR*>(info));
}

//////////////////////////////////////////////////////////////////

ISC_STATUS	dsql8_set_cursor(
				ISC_STATUS*		user_status,
				dsql_req**	req_handle,
				const TEXT*		input_cursor,
				USHORT		type)
{
	return GDS_DSQL_SET_CURSOR_CPP(user_status,
									req_handle,
									input_cursor,
									type);
}

//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
// End : C API -> C++ wrapper functions
//////////////////////////////////////////////////////////////////



/**
  
 	dsql_allocate_statement
  
    @brief	Allocate a statement handle.
 

    @param user_status
    @param db_handle
    @param req_handle

 **/
static ISC_STATUS
GDS_DSQL_ALLOCATE_CPP(	ISC_STATUS*    user_status,
						FB_API_HANDLE*    db_handle,
						dsql_req** req_handle)
{
	tsql* tdsql;
	tsql thd_context(user_status, tdsql);

	try
	{
		init(0);

// If we haven't been initialized yet, do it now 

		dsql_dbb* database = init(db_handle);

		DsqlContextPoolHolder context(tdsql, DsqlMemoryPool::createPool());

// allocate the request block 

		dsql_req* request = FB_NEW(*tdsql->getDefaultPool()) dsql_req(*tdsql->getDefaultPool());
		request->req_dbb = database;

		*req_handle = request;
	}
	catch(const std::exception& ex)
	{
		Firebird::stuff_exception(tdsql->tsql_status, ex);
		return tdsql->tsql_status[1];
	}

	return return_success();
}


/**
  
 	dsql_execute
  
    @brief	Execute a non-SELECT dynamic SQL statement.
 

    @param user_status
    @param trans_handle
    @param req_handle
    @param in_blr_length
    @param in_blr
    @param in_msg_type
    @param in_msg_length
    @param in_msg
    @param out_blr_length
    @param out_blr
    @param out_msg_type
    @param out_msg_length
    @param out_msg

 **/
ISC_STATUS	GDS_DSQL_EXECUTE_CPP(
				ISC_STATUS*		user_status,
				FB_API_HANDLE*		trans_handle,
				dsql_req**	req_handle,
				USHORT		in_blr_length,
				const UCHAR*		in_blr,
				USHORT		in_msg_type,
				USHORT		in_msg_length,
				UCHAR*		in_msg,
				USHORT		out_blr_length,
				UCHAR*		out_blr,
				USHORT		out_msg_type,
				USHORT		out_msg_length,
				UCHAR*		out_msg)
{
	tsql* tdsql;
	tsql thd_context(user_status, tdsql);
	ISC_STATUS sing_status;

	try
	{
		init(0);
		sing_status = 0;

		dsql_req* request = *req_handle;
		DsqlContextPoolHolder context(tdsql, &request->req_pool);

		if ((SSHORT) in_msg_type == -1) {
			request->req_type = REQ_EMBED_SELECT;
		}

// Only allow NULL trans_handle if we're starting a transaction 

		if (*trans_handle == 0 && request->req_type != REQ_START_TRANS)
		{
			ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 901,
				  	isc_arg_gds, isc_bad_trans_handle, 0);
		}

/* If the request is a SELECT or blob statement then this is an open.
   Make sure the cursor is not already open. */

		if (request->req_type == REQ_SELECT ||
			request->req_type == REQ_EXEC_BLOCK ||  
			request->req_type == REQ_SELECT_BLOCK ||
			request->req_type == REQ_SELECT_UPD ||
			request->req_type == REQ_EMBED_SELECT ||
			request->req_type == REQ_GET_SEGMENT ||
			request->req_type == REQ_PUT_SEGMENT)
		{
			if (request->req_flags & REQ_cursor_open)
			{
				ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 502,
					  	isc_arg_gds, isc_dsql_cursor_open_err, 0);
			}
		}

// A select with a non zero output length is a singleton select 
		bool singleton;
		if (request->req_type == REQ_SELECT && out_msg_length != 0) {
			singleton = true;
		}
		else {
			singleton = false;
		}

		if (request->req_type != REQ_EMBED_SELECT)
		{
			sing_status = execute_request(request, trans_handle, in_blr_length,
										  in_blr, in_msg_length, in_msg,
										  out_blr_length, out_blr,
										  out_msg_length, out_msg, singleton);
		}

/* If the output message length is zero on a REQ_SELECT then we must
 * be doing an OPEN cursor operation.
 * If we do have an output message length, then we're doing
 * a singleton SELECT.  In that event, we don't add the cursor
 * to the list of open cursors (it's not really open).
 */
		if ((request->req_type == REQ_SELECT && out_msg_length == 0) ||
			(request->req_type == REQ_SELECT_BLOCK) ||  
			request->req_type == REQ_SELECT_UPD ||
			request->req_type == REQ_EMBED_SELECT ||
			request->req_type == REQ_GET_SEGMENT ||
			request->req_type == REQ_PUT_SEGMENT)
		{
			request->req_flags |= REQ_cursor_open |
				((request->
			  	req_type == REQ_EMBED_SELECT) ? REQ_embedded_sql_cursor : 0);

			dsql_opn* open_cursor = FB_NEW(*DSQL_permanent_pool) dsql_opn;
			request->req_open_cursor = open_cursor;
			open_cursor->opn_request = request;
			open_cursor->opn_transaction = *trans_handle;
			THD_MUTEX_LOCK(&cursors_mutex);
			open_cursor->opn_next = open_cursors;
			open_cursors = open_cursor;
			THD_MUTEX_UNLOCK(&cursors_mutex);
			THREAD_EXIT();
			ISC_STATUS_ARRAY local_status;
			gds__transaction_cleanup(local_status,
								 trans_handle,
								 cleanup_transaction, NULL);
			THREAD_ENTER();
		}

		if (!sing_status) {
			return return_success();
		}
	}
	catch (const std::exception& ex)
	{
		Firebird::stuff_exception(tdsql->tsql_status, ex);
		return tdsql->tsql_status[1];
	}

	return sing_status;
}


static ISC_STATUS dsql8_execute_immediate_common(ISC_STATUS*	user_status,
											 FB_API_HANDLE*	db_handle,
											 FB_API_HANDLE*	trans_handle,
											 USHORT		length,
											 const TEXT*		string,
											 USHORT		dialect,
											 USHORT		in_blr_length,
											 const UCHAR*		in_blr,
											 USHORT		in_msg_type,
											 USHORT		in_msg_length,
											 UCHAR*		in_msg,
											 USHORT		out_blr_length,
											 UCHAR*		out_blr,
											 USHORT		out_msg_type,
											 USHORT		out_msg_length,
											 UCHAR*		out_msg,
											 long 		possible_requests)
{
/**************************************
 *
 *	d s q l _ e x e c u t e _ i m m e d i a t e _ c o m m o n
 *
 **************************************
 *
 * Functional description
 *	Common part of prepare and execute a statement.
 *
 **************************************/
	ISC_STATUS status;
	tsql* tdsql;
	tsql thd_context(user_status, tdsql);

	try
	{
		dsql_dbb* database = init(db_handle);

		DsqlContextPoolHolder context(tdsql, DsqlMemoryPool::createPool());

	// allocate the request block, then prepare the request 

		dsql_req* request = FB_NEW(*tdsql->getDefaultPool()) 
			dsql_req(*tdsql->getDefaultPool());
		request->req_dbb = database;
		request->req_trans = *trans_handle;

		try {

			if (!length) {
				length = strlen(string);
			}

// Figure out which parser version to use 
/* Since the API to dsql8_execute_immediate is public and can not be changed, there needs to
 * be a way to send the parser version to DSQL so that the parser can compare the keyword
 * version to the parser version.  To accomplish this, the parser version is combined with
 * the client dialect and sent across that way.  In dsql8_execute_immediate, the parser version
 * and client dialect are separated and passed on to their final desintations.  The information
 * is combined as follows:
 *     Dialect * 10 + parser_version
 *
 * and is extracted in dsql8_execute_immediate as follows:
 *      parser_version = ((dialect *10)+parser_version)%10
 *      client_dialect = ((dialect *10)+parser_version)/10
 *
 * For example, parser_version = 1 and client dialect = 1
 *
 *  combined = (1 * 10) + 1 == 11
 *
 *  parser = (combined) %10 == 1
 *  dialect = (combined) / 19 == 1
 *
 * If the parser version is not part of the dialect, then assume that the
 * connection being made is a local classic connection.
 */

			USHORT parser_version;
			if ((dialect / 10) == 0)
				parser_version = 2;
			else {
				parser_version = dialect % 10;
				dialect /= 10;
			}

			request->req_client_dialect = dialect;

			request = prepare(request, length, string, dialect, parser_version);

			if (!((1 << request->req_type) & possible_requests))
			{
				const int max_diag_len = 50;
				char err_str[max_diag_len + 1];
				strncpy(err_str, string, max_diag_len);
				err_str[max_diag_len] = 0;
				ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) -902,
					isc_arg_gds, isc_exec_sql_invalid_req, 
					isc_arg_string, err_str, isc_arg_end);
			}

			execute_request(request, trans_handle, in_blr_length, in_blr,
							in_msg_length, in_msg, out_blr_length, out_blr,
							out_msg_length,	out_msg, false);

			release_request(request, true);
		}	// try
		catch (const std::exception& ex) {
			Firebird::stuff_exception(tdsql->tsql_status, ex);
			status = error();
			release_request(request, true);
			return status;
		}
	}
	catch(const std::exception& ex)
	{
		Firebird::stuff_exception(tdsql->tsql_status, ex);
		return tdsql->tsql_status[1];
	}

	return return_success();
}

ISC_STATUS	dsql8_execute_immediate(
				ISC_STATUS*	user_status,
				FB_API_HANDLE*	db_handle,
				FB_API_HANDLE*	trans_handle,
				USHORT		length,
				const TEXT*		string,
				USHORT		dialect,
				USHORT		in_blr_length,
				const SCHAR*		in_blr,
				USHORT		in_msg_type,
				USHORT		in_msg_length,
				SCHAR*		in_msg,
				USHORT		out_blr_length,
				SCHAR*		out_blr,
				USHORT		out_msg_type,
				USHORT		out_msg_length,
				SCHAR*		out_msg)
{
/**************************************
 *
 *	d s q l _ e x e c u t e _ i m m e d i a t e
 *
 **************************************
 *
 * Functional description
 *	Prepare and execute a statement.
 *
 **************************************/
	return dsql8_execute_immediate_common(user_status, db_handle, trans_handle, length,
		string, dialect, in_blr_length, 
		reinterpret_cast<const UCHAR*>(in_blr),
		in_msg_type, in_msg_length,
		reinterpret_cast<UCHAR*>(in_msg),
		out_blr_length, 
		reinterpret_cast<UCHAR*>(out_blr), 
		out_msg_type, out_msg_length, 
		reinterpret_cast<UCHAR*>(out_msg),
		~0);
}

/**
  
 	check_for_create_database
  
    @brief	When executing dynamic sql_operator in context of existing connection and
 	existing transaction, it is not clear - what should do "Create Database"
 	operator with existing db_handle and tra_handle. Therefore we don't exec it.
 

    @param sql
    @param crdb

 **/
static bool check_for_create_database(const Firebird::string& sql,
									  const TEXT* crdb)
{
	for (Firebird::string::size_type i = 0; i < sql.length(); i++)
	{
		switch (sql[i]) {
			case '\t':
			case '\n':
			case '\r':
			case ' ':
				continue;
		}
		if (tolower(sql[i]) != *crdb++)
			break;
		if (! *crdb)
			return true;
	}

	return false;
}

/**
  
 	callback_execute_immediate
  
    @brief	Execute sql_operator in context of jrd_transaction_handle
 

    @param status
    @param jrd_attachment_handle
    @param jrd_transaction_handle
    @param sql_operator

 **/
ISC_STATUS callback_execute_immediate( ISC_STATUS* status,
								   Jrd::Attachment* jrd_attachment_handle,
								   Jrd::jrd_tra* jrd_transaction_handle,
								   const Firebird::string& sql_operator)
{
	// Other requests appear to be incorrect in this context 
	long requests = (1 << REQ_INSERT) | (1 << REQ_DELETE) | (1 << REQ_UPDATE)
			      | (1 << REQ_DDL) | (1 << REQ_SET_GENERATOR) | (1 << REQ_EXEC_PROCEDURE);

	if (check_for_create_database(sql_operator, "createdatabase") ||
		check_for_create_database(sql_operator, "createschema"))
	{
		requests = 0;
	}

	// 1. Locate why_db_handle, corresponding to jrd_database_handle 
	THREAD_EXIT();
	THD_MUTEX_LOCK (&databases_mutex);
	dsql_dbb* database;
	for (database = databases; database; database = database->dbb_next)
	{
		if (WHY_translate_handle(database->dbb_database_handle)->handle.h_dbb == jrd_attachment_handle)
		{
			break;
		}
	}
	if (! database) {
		status[0] = isc_arg_gds;
		status[1] = isc_bad_db_handle;
		status[2] = isc_arg_end;
		THD_MUTEX_UNLOCK(&databases_mutex);
		THREAD_ENTER();
		return status[1];
	}
	WHY_DBB	why_db_handle = WHY_translate_handle(database->dbb_database_handle);

	/* 2. Create why_trans_handle - it's new, but points to the same jrd
    	  transaction as original before callback. */
	WHY_TRA why_trans_handle = WHY_alloc_handle(why_db_handle->implementation, HANDLE_transaction);
	if (!why_trans_handle) {
		status[0] = isc_arg_gds;
		status[1] = isc_virmemexh;
		status[2] = isc_arg_end;
		THD_MUTEX_UNLOCK(&databases_mutex);
		THREAD_ENTER();
		return status[1];
	}
	why_trans_handle->handle.h_tra = jrd_transaction_handle;
	why_trans_handle->parent = why_db_handle;
	THD_MUTEX_UNLOCK (&databases_mutex);
    THREAD_ENTER();

	// 3. Call execute... function 
	const ISC_STATUS rc = dsql8_execute_immediate_common(status,
						&database->dbb_database_handle, &why_trans_handle->public_handle,
						sql_operator.length(), sql_operator.c_str(), 
						database->dbb_db_SQL_dialect,
						0, NULL, 0, 0, NULL, 0, NULL, 0, 0, NULL, requests);
	WHY_cleanup_transaction(why_trans_handle);
	WHY_free_handle(why_trans_handle->public_handle);
	return rc;
}


WHY_DBB	GetWhyAttachment (ISC_STATUS* status,
						  Jrd::Attachment* jrd_attachment_handle)
{
	THREAD_EXIT();
	THD_MUTEX_LOCK (&databases_mutex);
	dsql_dbb* database;
	WHY_DBB db_handle = 0;
	for (database = databases; database; database = database->dbb_next)
	{
		db_handle = WHY_translate_handle(database->dbb_database_handle);
		if (db_handle->handle.h_dbb == jrd_attachment_handle)
		{
			break;
		}
	}
	if (! database) {
		status[0] = isc_arg_gds;
		status[1] = isc_bad_db_handle;
		status[2] = isc_arg_end;
	}
	THD_MUTEX_UNLOCK (&databases_mutex);
	THREAD_ENTER();
	return database ? db_handle : 0;
}


/**
  
 	dsql_fetch
  
    @brief	Fetch next record from a dynamic SQL cursor
 

    @param user_status
    @param req_handle
    @param blr_length
    @param blr
    @param msg_type
    @param msg_length
    @param dsql_msg
    @param direction
    @param offset

 **/
ISC_STATUS GDS_DSQL_FETCH_CPP(	ISC_STATUS*	user_status,
							dsql_req**	req_handle,
							USHORT	blr_length,
							const UCHAR*	blr,
							USHORT	msg_type,
							USHORT	msg_length,
							UCHAR*	dsql_msg_buf
#ifdef SCROLLABLE_CURSORS
							, USHORT direction, SLONG offset
#endif
							)
{
	dsql_msg* message;
	dsql_par* parameter;
	ISC_STATUS s;
	tsql* tdsql;
	tsql thd_context(user_status, tdsql);

	try
	{
		init(0);

		dsql_req* request = *req_handle;
		DsqlContextPoolHolder context(tdsql, &request->req_pool);

// if the cursor isn't open, we've got a problem 

		if (request->req_type == REQ_SELECT ||
			request->req_type == REQ_SELECT_UPD ||
			request->req_type == REQ_SELECT_BLOCK ||
			request->req_type == REQ_EMBED_SELECT ||
			request->req_type == REQ_GET_SEGMENT)
		{
			if (!(request->req_flags & REQ_cursor_open))
				ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 504,
					  isc_arg_gds, isc_dsql_cursor_err,
					  isc_arg_gds, isc_dsql_cursor_not_open, 0);
		}

#ifdef SCROLLABLE_CURSORS

/* check whether we need to send an asynchronous scrolling message
   to the engine; the engine will automatically advance one record
   in the same direction as before, so optimize out messages of that
   type */

		if (request->req_type == REQ_SELECT &&
			request->req_dbb->dbb_base_level >= 5)
		{
			switch (direction)
			{
			case isc_fetch_next:
				if (!(request->req_flags & REQ_backwards))
					offset = 0;
				else {
					direction = blr_forward;
					offset = 1;
					request->req_flags &= ~REQ_backwards;
				}
				break;

			case isc_fetch_prior:
				if (request->req_flags & REQ_backwards)
					offset = 0;
				else {
					direction = blr_backward;
					offset = 1;
					request->req_flags |= REQ_backwards;
				}
				break;

			case isc_fetch_first:
				direction = blr_bof_forward;
				offset = 1;
				request->req_flags &= ~REQ_backwards;
				break;

			case isc_fetch_last:
				direction = blr_eof_backward;
				offset = 1;
				request->req_flags |= REQ_backwards;
				break;

			case isc_fetch_absolute:
				direction = blr_bof_forward;
				request->req_flags &= ~REQ_backwards;
				break;

			case isc_fetch_relative:
				if (offset < 0) {
					direction = blr_backward;
					offset = -offset;
					request->req_flags |= REQ_backwards;
				}
				else {
					direction = blr_forward;
					request->req_flags &= ~REQ_backwards;
				}
				break;

			default:
				ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 804,
					  isc_arg_gds, isc_dsql_sqlda_err, 0);
			}

			if (offset)
			{
				DSC desc;

				message = (dsql_msg*) request->req_async;

				desc.dsc_dtype = dtype_short;
				desc.dsc_scale = 0;
				desc.dsc_length = sizeof(USHORT);
				desc.dsc_flags = 0;
				desc.dsc_address = (UCHAR*) & direction;

				dsql_par* offset_parameter = message->msg_parameters;
				parameter = offset_parameter->par_next;
				MOVD_move(&desc, &parameter->par_desc);

				desc.dsc_dtype = dtype_long;
				desc.dsc_scale = 0;
				desc.dsc_length = sizeof(SLONG);
				desc.dsc_flags = 0;
				desc.dsc_address = (UCHAR*) & offset;

				MOVD_move(&desc, &offset_parameter->par_desc);

				THREAD_EXIT();
				s = isc_receive2(tdsql->tsql_status, &request->req_handle,
								 message->msg_number, message->msg_length,
								 message->msg_buffer, 0, direction, offset);
				THREAD_ENTER();

				if (s) {
					punt();
				}
			}
		}
#endif

		message = (dsql_msg*) request->req_receive;

/* Insure that the blr for the message is parsed, regardless of
   whether anything is found by the call to receive. */

		if (blr_length) {
			parse_blr(blr_length, blr, msg_length, message->msg_parameters);
		}

		if (request->req_type == REQ_GET_SEGMENT)
		{
			// For get segment, use the user buffer and indicator directly.
	
			parameter = request->req_blob->blb_segment;
			dsql_par* null = parameter->par_null;
			USHORT* ret_length =
				(USHORT *) (dsql_msg_buf + (IPTR) null->par_user_desc.dsc_address);
			UCHAR* buffer = dsql_msg_buf + (IPTR) parameter->par_user_desc.dsc_address;
			THREAD_EXIT();
			s = isc_get_segment(tdsql->tsql_status, &request->req_handle,
							ret_length, parameter->par_user_desc.dsc_length,
							reinterpret_cast<char*>(buffer));
			THREAD_ENTER();
			if (!s) {
				return 0;
			}
			else if (s == isc_segment) {
				return 101;
			}
			else if (s == isc_segstr_eof) {
				return 100;
			}
			else
				punt();
		}

		THREAD_EXIT();
		s = isc_receive(tdsql->tsql_status, &request->req_handle,
						message->msg_number, message->msg_length, 
						message->msg_buffer, 0);
		THREAD_ENTER();

		if (s) {
			punt();
		}

		const dsql_par* eof = request->req_eof;
		if (eof)
		{
			if (!*((USHORT *) eof->par_desc.dsc_address)) {
				return 100;
			}
		}

		map_in_out(NULL, message, 0, blr, msg_length, dsql_msg_buf);
	}  // try
	catch(const std::exception& ex)
	{
		Firebird::stuff_exception(tdsql->tsql_status, ex);
		return tdsql->tsql_status[1];
	}

	return return_success();
}


/**
  
 	dsql_free_statement
  
    @brief	Release request for a dsql statement
 

    @param user_status
    @param req_handle
    @param option

 **/
ISC_STATUS GDS_DSQL_FREE_CPP(ISC_STATUS*	user_status,
						 dsql_req**		req_handle,
						 USHORT		option)
{
	dsql_req* request;
	tsql* tdsql;
	tsql thd_context(user_status, tdsql);

	try
	{
		init(0);

		request = *req_handle;
		DsqlContextPoolHolder context(tdsql, &request->req_pool);

		if (option & DSQL_drop) {
		// Release everything associate with the request. 

			release_request(request, true);
			*req_handle = NULL;
		}
		else if (option & DSQL_close) {
		// Just close the cursor associated with the request. 

			if (!(request->req_flags & REQ_cursor_open))
				ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 501,
					  isc_arg_gds, isc_dsql_cursor_close_err, 0);

			close_cursor(request);
		}
	}
	catch(const std::exception& ex)
	{
		Firebird::stuff_exception(tdsql->tsql_status, ex);
		return tdsql->tsql_status[1];
	}

	return return_success();
}


/**
  
 	dsql_insert
  
    @brief	Insert next record into a dynamic SQL cursor
 

    @param user_status
    @param req_handle
    @param blr_length
    @param blr
    @param msg_type
    @param msg_length
    @param dsql_msg

 **/
ISC_STATUS GDS_DSQL_INSERT_CPP(	ISC_STATUS*	user_status,
							dsql_req**	req_handle,
							USHORT	blr_length,
							const UCHAR*	blr,
							USHORT	msg_type,
							USHORT	msg_length,
							const UCHAR*	dsql_msg_buf)
{
	tsql* tdsql;
	tsql thd_context(user_status, tdsql);

	try
	{
		init(0);

		dsql_req* request = *req_handle;
		DsqlContextPoolHolder context(tdsql, &request->req_pool);

// if the cursor isn't open, we've got a problem 

		if (request->req_type == REQ_PUT_SEGMENT)
			if (!(request->req_flags & REQ_cursor_open))
				ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 504,
					  isc_arg_gds, isc_dsql_cursor_err,
					  isc_arg_gds, isc_dsql_cursor_not_open, 0);

		dsql_msg* message = (dsql_msg*) request->req_receive;

/* Insure that the blr for the message is parsed, regardless of
   whether anything is found by the call to receive. */

		if (blr_length)
			parse_blr(blr_length, blr, msg_length, message->msg_parameters);

		if (request->req_type == REQ_PUT_SEGMENT) {
		// For put segment, use the user buffer and indicator directly. 

			dsql_par* parameter = request->req_blob->blb_segment;
			const SCHAR* buffer =
				reinterpret_cast<const SCHAR*>(
					dsql_msg_buf + (IPTR) parameter->par_user_desc.dsc_address);
			THREAD_EXIT();
			const ISC_STATUS s =
				isc_put_segment(tdsql->tsql_status, &request->req_handle,
							parameter->par_user_desc.dsc_length, buffer);
			THREAD_ENTER();
			if (s)
				punt();
		}
	}
	catch(const std::exception& ex)
	{
		Firebird::stuff_exception(tdsql->tsql_status, ex);
		return tdsql->tsql_status[1];
	}

	return return_success();
}


/**
  
 	dsql_prepare
  
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
ISC_STATUS GDS_DSQL_PREPARE_CPP(ISC_STATUS*			user_status,
							FB_API_HANDLE*		trans_handle,
							dsql_req**		req_handle,
							USHORT			length,
							const TEXT*			string,
							USHORT			dialect,
							USHORT			item_length,
							const UCHAR*	items,
							USHORT			buffer_length,
							UCHAR*			buffer)
{
	ISC_STATUS status;
	tsql* tdsql;
	tsql thd_context(user_status, tdsql);

	try
	{
		init(0);

		dsql_req* old_request = *req_handle;
		if (!old_request) {
			ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -901,
			           isc_arg_gds, isc_bad_req_handle,
				       0);
		}

		dsql_dbb* database;
		if (old_request) { // this test is irrelevant, see ERRD_post call above
			database = old_request->req_dbb;
			if (!database) {
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -901,
		                   isc_arg_gds, isc_bad_req_handle,
			               0);
			}
		}

// check to see if old request has an open cursor 

		if (old_request && (old_request->req_flags & REQ_cursor_open)) {
			ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 519,
				  isc_arg_gds, isc_dsql_open_cursor_request, 0);
		}

/* Allocate a new request block and then prepare the request.  We want to
   keep the old request around, as is, until we know that we are able
   to prepare the new one. */
/* It would be really *nice* to know *why* we want to
   keep the old request around -- 1994-October-27 David Schnepper */
/* Because that's the client's allocated statement handle and we
   don't want to trash the context in it -- 2001-Oct-27 Ann Harrison */

		DsqlContextPoolHolder context(tdsql, DsqlMemoryPool::createPool());
		dsql_req* request = FB_NEW(*tdsql->getDefaultPool()) 
			dsql_req(*tdsql->getDefaultPool());
		request->req_dbb = database;
		request->req_trans = *trans_handle;

		try {

			if (!string) {
				ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 104, 
					isc_arg_gds, isc_command_end_err, 0);	// Unexpected end of command
			}

			if (!length) {
				length = strlen(string);
			}

// Figure out which parser version to use 
/* Since the API to dsql8_prepare is public and can not be changed, there needs to
 * be a way to send the parser version to DSQL so that the parser can compare the keyword
 * version to the parser version.  To accomplish this, the parser version is combined with
 * the client dialect and sent across that way.  In dsql8_prepare_statement, the parser version
 * and client dialect are separated and passed on to their final desintations.  The information
 * is combined as follows:
 *     Dialect * 10 + parser_version
 *
 * and is extracted in dsql8_prepare_statement as follows:
 *      parser_version = ((dialect *10)+parser_version)%10
 *      client_dialect = ((dialect *10)+parser_version)/10
 *
 * For example, parser_version = 1 and client dialect = 1
 *
 *  combined = (1 * 10) + 1 == 11
 *
 *  parser = (combined) %10 == 1
 *  dialect = (combined) / 19 == 1
 *
 * If the parser version is not part of the dialect, then assume that the
 * connection being made is a local classic connection.
 */

			USHORT parser_version;
			if ((dialect / 10) == 0)
				parser_version = 2;
			else {
				parser_version = dialect % 10;
				dialect /= 10;
			}

			request->req_client_dialect = dialect;

			request = prepare(request, length, string, dialect, parser_version);

// Can not prepare a CREATE DATABASE/SCHEMA statement 

			if ((request->req_type == REQ_DDL) &&
				(request->req_ddl_node->nod_type == nod_def_database))
			{
				ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 530,
					isc_arg_gds, isc_dsql_crdb_prepare_err, 0);
			}

			request->req_flags |= REQ_prepared;

// Now that we know that the new request exists, zap the old one. 

			{
				DsqlContextPoolHolder another_context(tdsql, &old_request->req_pool);
				release_request(old_request, true);
			}

/* The request was sucessfully prepared, and the old request was
 * successfully zapped, so set the client's handle to the new request */

			*req_handle = request;

			return GDS_DSQL_SQL_INFO_CPP(user_status,
									req_handle,
									item_length,
									items,
									buffer_length,
									buffer);

		}	// try
		catch(const std::exception& ex) {
			Firebird::stuff_exception(tdsql->tsql_status, ex);
			status = error();
			release_request(request, true);
			return status;
		}
	}
	catch(const std::exception& ex)
	{
		Firebird::stuff_exception(tdsql->tsql_status, ex);
		return tdsql->tsql_status[1];
	}
}


/**
  
 	dsql_set_cursor_name
  
    @brief	Set a cursor name for a dynamic request
 

    @param user_status
    @param req_handle
    @param input_cursor
    @param type

 **/
ISC_STATUS GDS_DSQL_SET_CURSOR_CPP(	ISC_STATUS*	user_status,
								dsql_req**	req_handle,
								const TEXT*	input_cursor,
								USHORT	type)
{
	tsql* tdsql;
	tsql thd_context(user_status, tdsql);

	try
	{
		init(0);

		dsql_req* request = *req_handle;
		DsqlContextPoolHolder context(tdsql, &request->req_pool);

		const size_t MAX_CURSOR_LENGTH = 132 - 1;
		Firebird::string cursor = input_cursor;

		if (cursor[0] == '\"')
		{
			// Quoted cursor names eh? Strip'em.
			// Note that "" will be replaced with ".
			for (Firebird::string::iterator i = cursor.begin(); 
							i < cursor.end(); ++i) {
				if (*i == '\"') {
					cursor.erase(i);
				}
			}
		}
		else	// not quoted name
		{
			Firebird::string::size_type i = cursor.find(' ');
			if (i != Firebird::string::npos)
			{
				cursor.resize(i);
			}
			cursor.upper();
		}
		USHORT length = (USHORT) fb_utils::name_length(cursor.c_str());
	
		if (length == 0) {
			ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 502,
					  isc_arg_gds, isc_dsql_decl_err,
					  isc_arg_gds, isc_dsql_cursor_invalid, 0);
		}
		if (length > MAX_CURSOR_LENGTH) {
			length = MAX_CURSOR_LENGTH;
		}
		cursor.resize(length);

// If there already is a different cursor by the same name, bitch 

		const dsql_sym* symbol = 
			HSHD_lookup(request->req_dbb, cursor.c_str(), length, SYM_cursor, 0);
		if (symbol) {
			if (request->req_cursor == symbol) {
				return return_success();
			}

			ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 502,
					  isc_arg_gds, isc_dsql_decl_err,
					  isc_arg_gds, isc_dsql_cursor_redefined,
					  isc_arg_string, symbol->sym_string, 0);
		}

/* If there already is a cursor and its name isn't the same, ditto.
   We already know there is no cursor by this name in the hash table */

		if (!request->req_cursor) {
			request->req_cursor = MAKE_symbol(request->req_dbb, cursor.c_str(),
										  length, SYM_cursor, request);
		}
		else {
			fb_assert(request->req_cursor != symbol);
			ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 502,
					  isc_arg_gds, isc_dsql_decl_err,
					  isc_arg_gds, isc_dsql_cursor_redefined,
					  isc_arg_string, request->req_cursor->sym_string, 0);
		}
	}
	catch(const std::exception& ex)
	{
		Firebird::stuff_exception(tdsql->tsql_status, ex);
		return tdsql->tsql_status[1];
	}

	return return_success();
}


/**
  
 	dsql_sql_info
  
    @brief	Provide information on dsql statement
 

    @param user_status
    @param req_handle
    @param item_length
    @param items
    @param info_length
    @param info

 **/
ISC_STATUS GDS_DSQL_SQL_INFO_CPP(	ISC_STATUS*		user_status,
								dsql_req**		req_handle,
								USHORT		item_length,
								const UCHAR*	items,
								USHORT		info_length,
								UCHAR*		info)
{
	UCHAR buffer[256];
	USHORT length, number, first_index;
	tsql* tdsql;
	tsql thd_context(user_status, tdsql);

	try
	{
		init(0);
		memset(buffer, 0, sizeof(buffer));

		// Pre-initialize buffer. This is necessary because we don't want to transfer rubbish over the wire
		memset(info, 0, info_length); 

		dsql_req* request = *req_handle;
	
		const UCHAR* const end_items = items + item_length;
		const UCHAR* const end_info = info + info_length;

		// CVC: Is it the idea that this pointer remains with its previous value
		// in the loop or should it be made NULL in each iteration?
		dsql_msg** message = NULL;
		first_index = 0;

		while (items < end_items && *items != isc_info_end)
		{
			UCHAR item = *items++; // cannot be const
			if (item == isc_info_sql_select || item == isc_info_sql_bind)
			{
				message = (item == isc_info_sql_select) ?
					&request->req_receive : &request->req_send;
				if (info + 1 >= end_info) {
					*info = isc_info_truncated;
					return return_success();
				}
				*info++ = item;
			}
			else if (item == isc_info_sql_stmt_type)
			{
				switch (request->req_type) {
				case REQ_SELECT:
				case REQ_EMBED_SELECT:
					number = isc_info_sql_stmt_select;
					break;
				case REQ_SELECT_UPD:
					number = isc_info_sql_stmt_select_for_upd;
					break;
				case REQ_DDL:
					number = isc_info_sql_stmt_ddl;
					break;
				case REQ_GET_SEGMENT:
					number = isc_info_sql_stmt_get_segment;
					break;
				case REQ_PUT_SEGMENT:
					number = isc_info_sql_stmt_put_segment;
					break;
				case REQ_COMMIT:
				case REQ_COMMIT_RETAIN:
					number = isc_info_sql_stmt_commit;
					break;
				case REQ_ROLLBACK:
					number = isc_info_sql_stmt_rollback;
					break;
				case REQ_START_TRANS:
					number = isc_info_sql_stmt_start_trans;
					break;
				case REQ_INSERT:
					number = isc_info_sql_stmt_insert;
					break;
				case REQ_UPDATE:
				case REQ_UPDATE_CURSOR:
					number = isc_info_sql_stmt_update;
					break;
				case REQ_DELETE:
				case REQ_DELETE_CURSOR:
					number = isc_info_sql_stmt_delete;
					break;
				case REQ_EXEC_PROCEDURE:
					number = isc_info_sql_stmt_exec_procedure;
					break;
				case REQ_SET_GENERATOR:
					number = isc_info_sql_stmt_set_generator;
					break;
				case REQ_SAVEPOINT:
					number = isc_info_sql_stmt_savepoint;
					break;
				case REQ_EXEC_BLOCK: 
					number = isc_info_sql_stmt_exec_procedure;
					break;
				case REQ_SELECT_BLOCK: 
					number = isc_info_sql_stmt_select;
					break;
				default:
					number = 0;
					break;
				}
				length = convert((SLONG) number, buffer);
				info = put_item(item, length, buffer, info, end_info);
				if (!info) {
					return return_success();
				}
			}
			else if (item == isc_info_sql_sqlda_start) {
				length = *items++;
				first_index =
					static_cast<USHORT>(gds__vax_integer(items, length));
				items += length;
			}
			else if (item == isc_info_sql_batch_fetch) {
				if (request->req_flags & REQ_no_batch)
					number = 0;
				else
					number = 1;
				length = convert((SLONG) number, buffer);
				if (!(info = put_item(item, length, buffer, info, end_info))) {
					return return_success();
				}
			}
			else if (item == isc_info_sql_records) {
				length = get_request_info(request, (SSHORT) sizeof(buffer),
										  buffer);
				if (length && !(info = put_item(item, length, buffer, info, 
												end_info))) 
				{
					return return_success();
				}
			}
			else if (item == isc_info_sql_get_plan) {
			/* be careful, get_plan_info() will reallocate the buffer to a
			   larger size if it is not big enough */

				UCHAR* buffer_ptr = buffer;
				length =
					get_plan_info(request, (SSHORT) sizeof(buffer), reinterpret_cast<SCHAR**>(&buffer_ptr));

				if (length) {
					info = put_item(item, length, buffer_ptr, info, end_info);
				}

				if (length > sizeof(buffer)) {
					gds__free(buffer_ptr);
				}

				if (!info) {
					return return_success();
				}
			}
			else if (!message ||
				 	(item != isc_info_sql_num_variables
				  	&& item != isc_info_sql_describe_vars))
			{
				buffer[0] = item;
				item = isc_info_error;
				length = 1 + convert((SLONG) isc_infunk, buffer + 1);
				if (!(info = put_item(item, length, buffer, info, end_info))) {
					return return_success();
				}
			}
			else
			{
				number = (*message) ? (*message)->msg_index : 0;
				length = convert((SLONG) number, buffer);
				if (!(info = put_item(item, length, buffer, info, end_info))) {
					return return_success();
				}
				if (item == isc_info_sql_num_variables) {
					continue;
				}

				const UCHAR* end_describe = items;
				while (end_describe < end_items &&
				   	*end_describe != isc_info_end &&
				   	*end_describe != isc_info_sql_describe_end) 
				{
					end_describe++;
				}

				info = var_info(*message,
							items,
							end_describe,
							info,
							end_info,
							first_index);
				if (!info) {
					return return_success();
				}

				items = end_describe;
				if (*items == isc_info_sql_describe_end) {
					items++;
				}
			}
		}

		*info++ = isc_info_end;
	}
	catch(const std::exception& ex)
	{
		Firebird::stuff_exception(tdsql->tsql_status, ex);
		return tdsql->tsql_status[1];
	}

	return return_success();
}


#ifdef DSQL_DEBUG

static void trace_line(const char* message, ...) {
	char buffer[1024];
	va_list params;
	va_start(params, message);
	VSNPRINTF(buffer, sizeof(buffer), message, params);
	va_end(params);
	buffer[sizeof(buffer) - 1] = 0;
	gds__trace_raw(buffer);
}

/**
  
 	DSQL_pretty
  
    @brief	Pretty print a node tree.


    @param node
    @param column

 **/
void DSQL_pretty(const dsql_nod* node, int column)
{
	const dsql_str* string;

	TEXT buffer[1024];

	TEXT s[64];

	TEXT* p = buffer;
	p += sprintf(p, "%p ", (void*) node);

	if (column) {
		USHORT l = column * 3;
		do {
			*p++ = ' ';
		} while (--l);
	}

	*p = 0;

	if (!node) {
		trace_line("%s *** null ***\n", buffer);
		return;
	}

	switch (MemoryPool::blk_type(node)) {
	case (TEXT) dsql_type_str:
		trace_line("%sSTRING: \"%s\"\n", buffer, ((dsql_str*) node)->str_data);
		return;

	case (TEXT) dsql_type_fld:
		trace_line("%sFIELD: %s\n", buffer, ((dsql_fld*) node)->fld_name);
		return;

	case (TEXT) dsql_type_sym:
		trace_line("%sSYMBOL: %s\n", buffer, ((dsql_sym*) node)->sym_string);
		return;

	case (TEXT) dsql_type_nod:
		break;

	default:
		trace_line("%sUNKNOWN BLOCK TYPE\n", buffer);
		return;
	}

	const TEXT* verb;
	const dsql_nod* const* ptr = node->nod_arg;
	const dsql_nod* const* const end = ptr + node->nod_count;

	switch (node->nod_type) {
	case nod_abort:
		verb = "abort";
		break;
	case nod_agg_average:
		verb = "agg_average";
		break;
	case nod_agg_count:
		verb = "agg_count";
		break;
/* count2
	case nod_agg_distinct: verb = "agg_distinct";	break;
*/
	case nod_agg_max:
		verb = "agg_max";
		break;
	case nod_agg_min:
		verb = "agg_min";
		break;
	case nod_agg_total:
		verb = "agg_total";
		break;
	case nod_add:
		verb = "add";
		break;
	case nod_alias:
		verb = "alias";
		break;
	case nod_ansi_all:
	case nod_all:
		verb = "all";
		break;
	case nod_and:
		verb = "and";
		break;
	case nod_ansi_any:
	case nod_any:
		verb = "any";
		break;
	case nod_array:
		verb = "array element";
		break;
	case nod_assign:
		verb = "assign";
		break;
	case nod_average:
		verb = "average";
		break;
	case nod_between:
		verb = "between";
		break;
	case nod_cast:
		verb = "cast";
		break;
	case nod_collate:
		verb = "collate";
		break;
	case nod_concatenate:
		verb = "concatenate";
		break;
	case nod_containing:
		verb = "containing";
		break;
	case nod_count:
		verb = "count";
		break;
	case nod_current_date:
		verb = "current_date";
		break;
	case nod_current_time:
		verb = "current_time";
		break;
	case nod_current_timestamp:
		verb = "current_timestamp";
		break;
	case nod_cursor:
		verb = "cursor";
		break;
	case nod_dbkey:
		verb = "dbkey";
		break;
	case nod_rec_version:
		verb = "record_version";
		break;
	case nod_def_database:
		verb = "define database";
		break;
	case nod_def_field:
		verb = "define field";
		break;
	case nod_def_generator:
		verb = "define generator";
		break;
	case nod_def_filter:
		verb = "define filter";
		break;
	case nod_def_index:
		verb = "define index";
		break;
	case nod_def_relation:
		verb = "define relation";
		break;
	// CVC: New node redef_relation.
	case nod_redef_relation:
		verb = "redefine relation";
		break;
	case nod_def_view:
		verb = "define view";
		break;
	case nod_redef_view:
		verb = "redefine view";
		break;
	case nod_mod_view:
		verb = "modify view";
		break;
	case nod_replace_view:
		verb = "replace view";
		break;
	case nod_delete:
		verb = "delete";
		break;
	case nod_del_field:
		verb = "delete field";
		break;
	case nod_del_filter:
		verb = "delete filter";
		break;
	case nod_del_generator:
		verb = "delete generator";
		break;
	case nod_del_index:
		verb = "delete index";
		break;
	case nod_del_relation:
		verb = "delete relation";
		break;
	// CVC: New node del_view.
	case nod_del_view:
		verb = "delete view";
		break;
	case nod_def_procedure:
		verb = "define procedure";
		break;
	case nod_mod_procedure:
		verb = "modify procedure";
		break;
	case nod_replace_procedure:
		verb = "replace procedure";
		break;
	// CVC: New node redef_procedure.
	case nod_redef_procedure:
		verb = "redefine procedure";
		break;
	case nod_del_procedure:
		verb = "delete procedure";
		break;
	case nod_def_trigger:
		verb = "define trigger";
		break;
	case nod_mod_trigger:
		verb = "modify trigger";
		break;
	case nod_replace_trigger:
		verb = "replace trigger";
		break;
	case nod_del_trigger:
		verb = "delete trigger";
		break;
	case nod_divide:
		verb = "divide";
		break;
	case nod_eql_all:
	case nod_eql_any:
	case nod_eql:
		verb = "eql";
		break;
	case nod_equiv:
		verb = "equiv";
		break;
	case nod_erase:
		verb = "erase";
		break;
	case nod_execute:
		verb = "execute";
		break;
	case nod_exec_procedure:
		verb = "execute procedure";
		break;
	case nod_exec_block: 
		verb = "execute block";
		break;
	case nod_exists:
		verb = "exists";
		break;
	case nod_extract:
		verb = "extract";
		break;
	case nod_flag:
		verb = "flag";
		break;
	case nod_foreign:
		verb = "foreign key";
		break;
	case nod_gen_id:
		verb = "gen_id";
		break;
	case nod_geq_all:
	case nod_geq_any:
	case nod_geq:
		verb = "geq";
		break;
	case nod_get_segment:
		verb = "get segment";
		break;
	case nod_grant:
		verb = "grant";
		break;
	case nod_gtr_all:
	case nod_gtr_any:
	case nod_gtr:
		verb = "gtr";
		break;
	case nod_insert:
		verb = "insert";
		break;
	case nod_internal_info:
		verb = "internal info";
		break;
	case nod_join:
		verb = "join";
		break;
	case nod_join_full:
		verb = "join_full";
		break;
	case nod_join_left:
		verb = "join_left";
		break;
	case nod_join_right:
		verb = "join_right";
		break;
	case nod_leq_all:
	case nod_leq_any:
	case nod_leq:
		verb = "leq";
		break;
	case nod_like:
		verb = "like";
		break;
	case nod_list:
		verb = "list";
		break;
	case nod_lss_all:
	case nod_lss_any:
	case nod_lss:
		verb = "lss";
		break;
	case nod_max:
		verb = "max";
		break;
	case nod_min:
		verb = "min";
		break;
	case nod_missing:
		verb = "missing";
		break;
	case nod_modify:
		verb = "modify";
		break;
	case nod_mod_database:
		verb = "modify database";
		break;
	case nod_mod_field:
		verb = "modify field";
		break;
	case nod_mod_relation:
		verb = "modify relation";
		break;
	case nod_multiply:
		verb = "multiply";
		break;
	case nod_negate:
		verb = "negate";
		break;
	case nod_neq_all:
	case nod_neq_any:
	case nod_neq:
		verb = "neq";
		break;
	case nod_not:
		verb = "not";
		break;
	case nod_null:
		verb = "null";
		break;
	case nod_or:
		verb = "or";
		break;
	case nod_order:
		verb = "order";
		break;
	case nod_primary:
		verb = "primary key";
		break;
	case nod_procedure_name:
		verb = "procedure name";
		break;
	case nod_put_segment:
		verb = "put segment";
		break;
	case nod_relation_name:
		verb = "relation name";
		break;
	case nod_rel_proc_name:
		verb = "rel/proc name";
		break;
	case nod_return:
		verb = "return";
		break;
	case nod_revoke:
		verb = "revoke";
		break;
	case nod_rse:
		verb = "rse";
		break;
	case nod_select:
		verb = "select";
		break;
	case nod_select_expr:
		verb = "select expr";
		break;
	case nod_starting:
		verb = "starting";
		break;
	case nod_store:
		verb = "store";
		break;
	case nod_substr:
		verb = "substr";
		break;
	case nod_subtract:
		verb = "subtract";
		break;
	case nod_total:
		verb = "total";
		break;
	case nod_update:
		verb = "update";
		break;
	case nod_union:
		verb = "union";
		break;
	case nod_unique:
		verb = "unique";
		break;
	case nod_upcase:
		verb = "upcase";
		break;
	case nod_singular:
		verb = "singular";
		break;
	case nod_user_name:
		verb = "user_name";
		break;
	// CVC: New node current_role.
	case nod_current_role:
		verb = "current_role";
		break;
	case nod_via:
		verb = "via";
		break;

	case nod_coalesce:
		verb = "coalesce";
		break;

	case nod_simple_case:
		verb = "simple_case";
		break;

	case nod_searched_case:
		verb = "searched_case";
		break;

	case nod_add2:
		verb = "add2";
		break;
	case nod_agg_total2:
		verb = "agg_total2";
		break;
	case nod_divide2:
		verb = "divide2";
		break;
	case nod_gen_id2:
		verb = "gen_id2";
		break;
	case nod_multiply2:
		verb = "multiply2";
		break;
	case nod_subtract2:
		verb = "subtract2";
		break;
	case nod_limit:
		verb = "limit";
		break;
	case nod_rows:
		verb = "rows";
		break;
	/* IOL:	missing	node types */
	case nod_on_error:
		verb = "on error";
		break;
	case nod_block:
		verb = "block";
		break;
	case nod_default:
		verb = "default";
		break;
	case nod_plan_expr:
		verb = "plan";
		break;
	case nod_index:
		verb = "index";
		break;
	case nod_index_order:
		verb = "order";
		break;
	case nod_plan_item:
		verb = "item";
		break;
	case nod_natural:
		verb = "natural";
		break;
	case nod_join_inner:
		verb = "join_inner";
		break;
	// SKIDDER: some more missing node types 
	case nod_commit:
		verb = "commit";
		break;
	case nod_rollback:
		verb = "rollback";
		break;
	case nod_trans:
		verb = "trans";
		break;
	case nod_def_default:
		verb = "def_default";
		break;
	case nod_del_default:
		verb = "del_default";
		break;
	case nod_def_domain:
		verb = "def_domain";
		break;
	case nod_mod_domain:
		verb = "mod_domain";
		break;
	case nod_del_domain:
		verb = "del_domain";
		break;
	case nod_def_constraint:
		verb = "def_constraint";
		break;
/*
	case nod_def_trigger_msg:
		verb = "def_trigger_msg";
		break;
	case nod_mod_trigger_msg:
		verb = "mod_trigger_msg";
		break;
	case nod_del_trigger_msg:
		verb = "del_trigger_msg";
		break;
*/
	case nod_def_exception:
		verb = "def_exception";
		break;
	case nod_redef_exception:
		verb = "redef_exception";
		break;
	case nod_mod_exception:
		verb = "mod_exception";
		break;
	case nod_replace_exception:
		verb = "replace_exception";
		break;
	case nod_del_exception:
		verb = "del_exception";
		break;
	case nod_def_shadow:
		verb = "def_shadow";
		break;
	case nod_del_shadow:
		verb = "del_shadow";
		break;
	case nod_def_udf:
		verb = "def_udf";
		break;
	case nod_del_udf:
		verb = "del_udf";
		break;
	case nod_rel_constraint:
		verb = "rel_constraint";
		break;
	case nod_delete_rel_constraint:
		verb = "delete_rel_constraint";
		break;
	case nod_references:
		verb = "references";
		break;
	case nod_proc_obj:
		verb = "proc_obj";
		break;
	case nod_trig_obj:
		verb = "trig_obj";
		break;
	case nod_view_obj:
		verb = "view_obj";
		break;
	case nod_exit:
		verb = "exit";
		break;
	case nod_if:
		verb = "if";
		break;
	case nod_erase_current:
		verb = "erase_current";
		break;
	case nod_modify_current:
		verb = "modify_current";
		break;
	case nod_post:
		verb = "post";
		break;
	case nod_sqlcode:
		verb = "sqlcode";
		break;
	case nod_gdscode:
		verb = "gdscode";
		break;
	case nod_exception:
		verb = "exception";
		break;
	case nod_exception_stmt:
		verb = "exception_stmt";
		break;
	case nod_start_savepoint:
		verb = "start_savepoint";
		break;
	case nod_end_savepoint:
		verb = "end_savepoint";
		break;
	case nod_dom_value:
		verb = "dom_value";
		break;
	case nod_user_group:
		verb = "user_group";
		break;
	case nod_from:
		verb = "from";
		break;
	case nod_agg_average2:
		verb = "agg_average2";
		break;
	case nod_access:
		verb = "access";
		break;
	case nod_wait:
		verb = "wait";
		break;
	case nod_isolation:
		verb = "isolation";
		break;
	case nod_version:
		verb = "version";
		break;
	case nod_table_lock:
		verb = "table_lock";
		break;
	case nod_lock_mode:
		verb = "lock_mode";
		break;
	case nod_reserve:
		verb = "reserve";
		break;
	case nod_commit_retain:
		verb = "commit_retain";
		break;
	case nod_page_size:
		verb = "page_size";
		break;
	case nod_file_length:
		verb = "file_length";
		break;
	case nod_file_desc:
		verb = "file_desc";
		break;
/*
	case nod_log_file_desc:
		verb = "log_file_desc";
		break;
	case nod_cache_file_desc:
		verb = "cache_file_desc";
		break;
	case nod_group_commit_wait:
		verb = "group_commit_wait";
		break;
	case nod_check_point_len:
		verb = "check_point_len";
		break;
	case nod_num_log_buffers:
		verb = "num_log_buffers";
		break;
	case nod_log_buffer_size:
		verb = "log_buffer_size";
		break;
	case nod_drop_log:
		verb = "drop_log";
		break;
	case nod_drop_cache:
		verb = "drop_cache";
		break;
*/
	case nod_dfl_charset:
		verb = "dfl_charset";
		break;
	case nod_password:
		verb = "password";
		break;
	case nod_lc_ctype:
		verb = "lc_ctype";
		break;
	case nod_udf_return_value:
		verb = "udf_return_value";
		break;
	case nod_def_computed:
		verb = "def_computed";
		break;
	case nod_merge:
		verb = "merge";
		break;
	case nod_set_generator:
		verb = "set_generator";
		break;
	case nod_set_generator2:
		verb = "set_generator2";
		break;
	case nod_mod_index:
		verb = "mod_index";
		break;
	case nod_idx_active:
		verb = "idx_active";
		break;
	case nod_idx_inactive:
		verb = "idx_inactive";
		break;
	case nod_restrict:
		verb = "restrict";
		break;
	case nod_cascade:
		verb = "cascade";
		break;
	case nod_set_statistics:
		verb = "set_statistics";
		break;
	case nod_ref_upd_del:
		verb = "ref_upd_del";
		break;
	case nod_ref_trig_action:
		verb = "ref_trig_action";
		break;
	case nod_def_role:
		verb = "def_role";
		break;
	case nod_role_name:
		verb = "role_name";
		break;
	case nod_grant_admin:
		verb = "grant_admin";
		break;
	case nod_del_role:
		verb = "del_role";
		break;
	case nod_mod_domain_type:
		verb = "mod_domain_type";
		break;
	case nod_mod_field_name:
		verb = "mod_field_name";
		break;
	case nod_mod_field_type:
		verb = "mod_field_type";
		break;
	case nod_mod_field_pos:
		verb = "mod_field_pos";
		break;
	case nod_udf_param:
		verb = "udf_param";
		break;
	case nod_exec_sql:
		verb = "exec_sql";
		break;
	case nod_for_update:
		verb = "for_update";
		break;
	case nod_user_savepoint:
		verb = "user_savepoint";
		break;
	case nod_release_savepoint:
		verb = "release_savepoint";
		break;
	case nod_undo_savepoint:
		verb = "undo_savepoint";
		break;
	case nod_difference_file:
		verb = "difference_file";
		break;
	case nod_drop_difference:
		verb = "drop_difference";
		break;
	case nod_begin_backup:
		verb = "begin_backup";
		break;
	case nod_end_backup:
		verb = "end_backup";
		break;
	case nod_derived_table:
		verb = "derived_table";
		break;

	case nod_exec_into:
		verb = "exec_into";
		break;

	case nod_breakleave:
		verb = "breakleave";
		break;

	case nod_for_select:
		verb = "for_select";
		break;

	case nod_while:
		verb = "while";
		break;

	case nod_label:
		verb = "label";
		DSQL_pretty(node->nod_arg[e_label_name], column + 1);
		trace_line("%s   number %d\n", buffer,
			(int)(IPTR)node->nod_arg[e_label_number]);
		return;

	case nod_derived_field:
		verb = "derived_field";
		trace_line("%s%s\n", buffer, verb);
		DSQL_pretty(node->nod_arg[e_derived_field_value], column + 1);
		DSQL_pretty(node->nod_arg[e_derived_field_name], column + 1);
		trace_line("%s   scope %d\n", buffer,
			(USHORT)(U_IPTR)node->nod_arg[e_derived_field_scope]);
		return;

	case nod_aggregate:
		{
		verb = "aggregate";
		trace_line("%s%s\n", buffer, verb);
		const dsql_ctx* context = (dsql_ctx*) node->nod_arg[e_agg_context];
		trace_line("%s   context %d\n", buffer, context->ctx_context);
		dsql_map* map = context->ctx_map;
		if (map != NULL)
			trace_line("%s   map\n", buffer);
		while (map) {
			trace_line("%s      position %d\n", buffer, map->map_position);
			DSQL_pretty(map->map_node, column + 2);
			map = map->map_next;
		}
		DSQL_pretty(node->nod_arg[e_agg_group], column + 1);
		DSQL_pretty(node->nod_arg[e_agg_rse], column + 1);
		return;
		}

	case nod_constant:
		verb = "constant";
		if (node->nod_desc.dsc_address) {
			if (node->nod_desc.dsc_dtype == dtype_text)
				sprintf(s, "constant \"%s\"", node->nod_desc.dsc_address);
			else
				sprintf(s, "constant %ld",
						*(SLONG *) (node->nod_desc.dsc_address));
			verb = s;
		}
		break;

	case nod_field:
		{
		const dsql_ctx* context = (dsql_ctx*) node->nod_arg[e_fld_context];
		const dsql_rel* relation = context->ctx_relation;
		const dsql_prc* procedure = context->ctx_procedure;
		const dsql_fld* field = (dsql_fld*) node->nod_arg[e_fld_field];
		trace_line("%sfield %s.%s, context %d\n", buffer,
			(relation != NULL ?
				relation->rel_name :
				(procedure != NULL ?
					procedure->prc_name :
					"unknown_db_object")),
			field->fld_name, context->ctx_context);
		return;
		}
	
	case nod_field_name:
		trace_line("%sfield name: \"", buffer);
		string = (dsql_str*) node->nod_arg[e_fln_context];
		if (string)
			trace_line("%s.", string->str_data);
		string = (dsql_str*) node->nod_arg[e_fln_name];
		if (string != 0) {
			trace_line("%s\"\n", string->str_data);
		}
		else {
			trace_line("%s\"\n", "*");
		}
		return;

	case nod_map:
		{
		verb = "map";
		trace_line("%s%s\n", buffer, verb);
		const dsql_ctx* context = (dsql_ctx*) node->nod_arg[e_map_context];
		trace_line("%s   context %d\n", buffer, context->ctx_context);
		for (const dsql_map* map = (dsql_map*) node->nod_arg[e_map_map]; map;
			map = map->map_next)
		{
			trace_line("%s   position %d\n", buffer, map->map_position);
			DSQL_pretty(map->map_node, column + 1);
		}
		return;
		}

	case nod_relation:
		{
		const dsql_ctx* context = (dsql_ctx*) node->nod_arg[e_rel_context];
		const dsql_rel* relation = context->ctx_relation;
		const dsql_prc* procedure = context->ctx_procedure;
		if ( relation != NULL ) {
			trace_line("%srelation %s, context %d\n",
				buffer, relation->rel_name, context->ctx_context);
		}
		else if ( procedure != NULL ) {
			trace_line("%sprocedure %s, context %d\n",
				buffer, procedure->prc_name, context->ctx_context);
		}
		else {
			trace_line("%sUNKNOWN DB OBJECT, context %d\n",
				buffer, context->ctx_context);
		}
		return;
		}

	case nod_variable:
		{
		const dsql_var* variable = (dsql_var*) node->nod_arg[e_var_variable];
		// Adding variable->var_variable_number to display, obviously something
		// is missing from the printf, and Im assuming this was it.
		// (anyway can't be worse than it was MOD 05-July-2002.
		trace_line("%svariable %s %d\n", buffer, variable->var_name, variable->var_variable_number);
		return;
		}

	case nod_var_name:
		trace_line("%svariable name: \"", buffer);
		string = (dsql_str*) node->nod_arg[e_vrn_name];
		trace_line("%s\"\n", string->str_data);
		return;

	case nod_parameter:
		if (node->nod_column) {
			trace_line("%sparameter: %d\n",	buffer,
				(USHORT)(IPTR)node->nod_arg[e_par_parameter]);
		}
		else {
			const dsql_par* param = (dsql_par*) node->nod_arg[e_par_parameter];
			trace_line("%sparameter: %d\n",	buffer, param->par_index);
		}
		return;


	case nod_udf:
		trace_line ("%sfunction: \"", buffer);
		/* nmcc: how are we supposed to tell which type of nod_udf this is ?? */
		/* CVC: The answer is that nod_arg[0] can be either the udf name or the
		pointer to udf struct returned by METD_get_function, so we should resort
		to the block type. The replacement happens in pass1_udf(). */
		//switch (node->nod_arg[e_udf_name]->nod_header.blk_type) {
		switch (MemoryPool::blk_type(node->nod_arg[e_udf_name])) {
		case dsql_type_udf:
			trace_line ("%s\"\n", ((dsql_udf*) node->nod_arg[e_udf_name])->udf_name);
			break;
		case dsql_type_str:
			string = (dsql_str*) node->nod_arg[e_udf_name];
			trace_line ("%s\"\n", string->str_data);
			break;
		default:
			trace_line ("%s\"\n", "<ERROR>");
			break;
		}
		ptr++;

		if (node->nod_count == 2) {
			DSQL_pretty (*ptr, column + 1);
		}
		return;

	default:
		sprintf(s, "unknown type %d", node->nod_type);
		verb = s;
	}

	if (node->nod_desc.dsc_dtype) {
		trace_line("%s%s (%d,%d,%p)\n",
				buffer, verb,
				node->nod_desc.dsc_dtype,
				node->nod_desc.dsc_length, node->nod_desc.dsc_address);
	}
	else {
		trace_line("%s%s\n", buffer, verb);
	}
	++column;

	while (ptr < end) {
		DSQL_pretty(*ptr++, column);
	}

	return;
}
#endif


/**
  
 	cleanup
  
    @brief	exit handler for local dsql image
 

    @param arg

 **/
static void cleanup( void *arg)
{

	if (init_flag) {
		init_flag = false;
		databases = 0;
		open_cursors = 0;
		HSHD_fini();
		ALLD_fini();
	}
}


/**
  
 	cleanup_database
  
    @brief	Clean up DSQL globals.
 
  N.B., the cleanup handlers (registered with isc_database_cleanup)
  are called outside of the ISC thread mechanism...
 
  These do not make use of the context at this time.
 

    @param db_handle
    @param flag

 **/
static void cleanup_database(FB_API_HANDLE* db_handle, void* flag)
{
	if (flag)
		Firebird::fatal_exception::raise("Illegal call to cleanup_database");

	if (!db_handle || !databases)
		return;

/*	if (flag)
		THREAD_EXIT();*/

	THD_MUTEX_LOCK(&databases_mutex);

	dsql_dbb* dbb;
	for (dsql_dbb** dbb_ptr = &databases; dbb = *dbb_ptr; dbb_ptr = &dbb->dbb_next)
		if (dbb->dbb_database_handle == *db_handle) {
			*dbb_ptr = dbb->dbb_next;
			dbb->dbb_next = NULL;
			break;
		}
/*
	if (dbb) {
		if (flag) {
			for (int i = 0; i < irq_MAX; i++)
				if (dbb->dbb_requests[i])
					isc_release_request(user_status,
								reinterpret_cast <void**>(&dbb->dbb_requests[i]));
			THREAD_ENTER();
		}
		HSHD_finish(dbb);
		delete dbb->dbb_pool;
	}
	else if (flag)
		THREAD_ENTER();
*/
	if (dbb) {
		HSHD_finish(dbb);
		DsqlMemoryPool::deletePool(dbb->dbb_pool);
	}

	if (!databases) {
		cleanup(0);
		gds__unregister_cleanup(cleanup, 0);
	}
	THD_MUTEX_UNLOCK(&databases_mutex);
}


/**
  
 	cleanup_transaction
  
    @brief	Clean up after a transaction.  This means
 	closing all open cursors.
 

    @param tra_handle
    @param arg

 **/
static void cleanup_transaction (FB_API_HANDLE tra_handle, void* arg)
{
	ISC_STATUS_ARRAY local_status;

// find this transaction/request pair in the list of pairs 

	THD_MUTEX_LOCK(&cursors_mutex);
	dsql_opn** open_cursor_ptr = &open_cursors;
	dsql_opn* open_cursor;
	while (open_cursor = *open_cursor_ptr)
	{
		if (open_cursor->opn_transaction == tra_handle) {
			/* Found it, close the cursor but don't remove it from the list.
			   The close routine will have done that. */

			THD_MUTEX_UNLOCK(&cursors_mutex);
			/*
			 * we are expected to be within the subsystem when we do this
			 * cleanup, for now do a thread_enter/thread_exit here.
			 * Note that the function dsql8_free_statement() calls the local function.
			 * Over the long run, it might be better to move the subsystem_exit()
			 * call in why.c below the cleanup handlers. smistry 9-27-98
			 */
			THREAD_ENTER(); //ttt
			GDS_DSQL_FREE_CPP(	local_status,
								&open_cursor->opn_request,
								DSQL_close);
			THREAD_EXIT();
			THD_MUTEX_LOCK(&cursors_mutex);
			open_cursor_ptr = &open_cursors;
		}
		else
			open_cursor_ptr = &open_cursor->opn_next;
	}

	THD_MUTEX_UNLOCK(&cursors_mutex);
}


/**
  
 	close_cursor
  
    @brief	Close an open cursor.
 

    @param request

 **/
static void close_cursor( dsql_req* request)
{
	ISC_STATUS_ARRAY status_vector;

	if (request->req_handle) {
		THREAD_EXIT(); //ttt
		if (request->req_type == REQ_GET_SEGMENT ||
			request->req_type == REQ_PUT_SEGMENT)
		{
			isc_close_blob(status_vector, &request->req_handle);
		}
		else
			isc_unwind_request(status_vector, &request->req_handle, 0);
		THREAD_ENTER();
	}

	request->req_flags &= ~(REQ_cursor_open | REQ_embedded_sql_cursor);

// Remove the open cursor from the list 

	THD_MUTEX_LOCK(&cursors_mutex);
	dsql_opn** open_cursor_ptr = &open_cursors;
	dsql_opn* open_cursor;
	for (; open_cursor = *open_cursor_ptr;
		 open_cursor_ptr = &open_cursor->opn_next)
	{
		if (open_cursor == request->req_open_cursor) {
			*open_cursor_ptr = open_cursor->opn_next;
			break;
		}
	}

	THD_MUTEX_UNLOCK(&cursors_mutex);

	if (open_cursor) {
		delete open_cursor;
		request->req_open_cursor = NULL;
	}
}


/**
  
 	convert
  
    @brief	Convert a number to VAX form -- least significant bytes first.
 	Return the length.
 

    @param number
    @param buffer

 **/

// CVC: This routine should disappear in favor of a centralized function.
static USHORT convert( SLONG number, UCHAR* buffer)
{
	const UCHAR* p;

#ifndef WORDS_BIGENDIAN
	const SLONG n = number;
	p = (UCHAR *) & n;
	*buffer++ = *p++;
	*buffer++ = *p++;
	*buffer++ = *p++;
	*buffer++ = *p++;

#else

	p = (UCHAR *) (&number + 1);
	*buffer++ = *--p;
	*buffer++ = *--p;
	*buffer++ = *--p;
	*buffer++ = *--p;

#endif

	return 4;
}


/**
  
 	error
  
    @brief	An error returned has been trapped.
 


 **/
static ISC_STATUS error()
{
	tsql* tdsql = DSQL_get_thread_data();

	return tdsql->tsql_status[1];
}


/**
  
 	execute_blob
  
    @brief	Open or create a blob.
 

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
static void execute_blob(	dsql_req*		request,
							USHORT	in_blr_length,
							const UCHAR*	in_blr,
							USHORT	in_msg_length,
							UCHAR*	in_msg,
							USHORT	out_blr_length,
							UCHAR*	out_blr,
							USHORT	out_msg_length,
							UCHAR*	out_msg)
{
	UCHAR bpb[24];
	ISC_STATUS s;

	tsql* tdsql = DSQL_get_thread_data();

	dsql_blb* blob = request->req_blob;
	map_in_out(request, blob->blb_open_in_msg, in_blr_length, in_blr,
			   in_msg_length, in_msg);

	UCHAR* p = bpb;
	*p++ = isc_bpb_version1;
	SSHORT filter = filter_sub_type(request, blob->blb_to);
	if (filter) {
		*p++ = isc_bpb_target_type;
		*p++ = 2;
		*p++ = static_cast<UCHAR>(filter);
		*p++ = filter >> 8;
	}
	filter = filter_sub_type(request, blob->blb_from);
	if (filter) {
		*p++ = isc_bpb_source_type;
		*p++ = 2;
		*p++ = static_cast<UCHAR>(filter);
		*p++ = filter >> 8;
	}
	USHORT bpb_length = p - bpb;
	if (bpb_length == 1) {
		bpb_length = 0;
	}

	dsql_par* parameter = blob->blb_blob_id;
	const dsql_par* null = parameter->par_null;

	if (request->req_type == REQ_GET_SEGMENT)
	{
		ISC_QUAD* blob_id = (ISC_QUAD*) parameter->par_desc.dsc_address;
		if (null && *((SSHORT *) null->par_desc.dsc_address) < 0) {
			memset(blob_id, 0, sizeof(ISC_QUAD));
		}
		THREAD_EXIT();
		s = isc_open_blob2(tdsql->tsql_status,
						   &request->req_dbb->dbb_database_handle,
						   &request->req_trans, &request->req_handle,
						   blob_id, bpb_length,
						   bpb);
		THREAD_ENTER();
		if (s) {
			punt();
		}
	}
	else
	{
		request->req_handle = 0;
		ISC_QUAD* blob_id = (ISC_QUAD*) parameter->par_desc.dsc_address;
		memset(blob_id, 0, sizeof(ISC_QUAD));
		THREAD_EXIT();
		s = isc_create_blob2(tdsql->tsql_status,
							 &request->req_dbb->dbb_database_handle,
							 &request->req_trans, &request->req_handle,
							 blob_id, bpb_length,
							 reinterpret_cast<const char*>(bpb));
		THREAD_ENTER();
		if (s) {
			punt();
		}
		map_in_out(NULL, blob->blb_open_out_msg, out_blr_length, out_blr,
				   out_msg_length, out_msg);
	}
}


/**
  
 	execute_request
  
    @brief	Execute a dynamic SQL statement.
 

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
static ISC_STATUS execute_request(dsql_req*			request,
								  FB_API_HANDLE*	trans_handle,
								  USHORT			in_blr_length,
								  const UCHAR*			in_blr,
								  USHORT			in_msg_length,
								  UCHAR*			in_msg,
								  USHORT			out_blr_length,
								  UCHAR*			out_blr,
								  USHORT			out_msg_length,
								  UCHAR*			out_msg,
								  bool				singleton)
{
	ISC_STATUS s;

	tsql* tdsql = DSQL_get_thread_data();

	request->req_trans = *trans_handle;
	ISC_STATUS return_status = FB_SUCCESS;

	switch (request->req_type) {
	case REQ_START_TRANS:
		THREAD_EXIT();
		s = isc_start_transaction(	tdsql->tsql_status,
									&request->req_trans,
									1,
									&request->req_dbb->dbb_database_handle,
									request->req_blr_data.getCount(),
									request->req_blr_data.begin());
		THREAD_ENTER();
		if (s)
			punt();
		*trans_handle = request->req_trans;
		return FB_SUCCESS;

	case REQ_COMMIT:
		THREAD_EXIT();
		s = isc_commit_transaction(tdsql->tsql_status,
								   &request->req_trans);
		THREAD_ENTER();
		if (s)
			punt();
		*trans_handle = 0;
		return FB_SUCCESS;

	case REQ_COMMIT_RETAIN:
		THREAD_EXIT();
		s = isc_commit_retaining(tdsql->tsql_status,
								 &request->req_trans);
		THREAD_ENTER();
		if (s)
			punt();
		return FB_SUCCESS;

	case REQ_ROLLBACK:
		THREAD_EXIT();
		s = isc_rollback_transaction(tdsql->tsql_status,
									 &request->req_trans);
		THREAD_ENTER();
		if (s)
			punt();
		*trans_handle = 0;
		return FB_SUCCESS;

	case REQ_DDL:
		DDL_execute(request);
		return FB_SUCCESS;

	case REQ_GET_SEGMENT:
		execute_blob(	request,
						in_blr_length,
						in_blr,
						in_msg_length,
						in_msg,
						out_blr_length,
						out_blr,
						out_msg_length,
						out_msg);
		return FB_SUCCESS;

	case REQ_PUT_SEGMENT:
		execute_blob(	request,
						in_blr_length,
						in_blr,
						in_msg_length,
						in_msg,
						out_blr_length,
						out_blr,
						out_msg_length,
						out_msg);
		return FB_SUCCESS;

	case REQ_SELECT:
	case REQ_SELECT_UPD:
	case REQ_EMBED_SELECT:
	case REQ_INSERT:
	case REQ_UPDATE:
	case REQ_UPDATE_CURSOR:
	case REQ_DELETE:
	case REQ_DELETE_CURSOR:
	case REQ_EXEC_PROCEDURE:
	case REQ_SET_GENERATOR:
	case REQ_SAVEPOINT:
	case REQ_EXEC_BLOCK:
	case REQ_SELECT_BLOCK:
		break;

	default:
		// Catch invalid request types 
		fb_assert(false);
	}

// If there is no data required, just start the request 

	dsql_msg* message = (dsql_msg*) request->req_send;
	if (!message)
	{
		THREAD_EXIT();
		s = isc_start_request(tdsql->tsql_status,
							  &request->req_handle,
							  &request->req_trans,
							  0);
		THREAD_ENTER();
		if (s)
			punt();
	}
	else
	{
		map_in_out(	request,
					message,
					in_blr_length,
					in_blr,
					in_msg_length,
					in_msg);

		THREAD_EXIT();
		s = isc_start_and_send(tdsql->tsql_status,
							   &request->req_handle,
							   &request->req_trans,
							   message->msg_number,
							   message->msg_length,
							   message->msg_buffer,
							   0);
		THREAD_ENTER();
		if (s)
		{
			punt();
		}
	}

	ISC_STATUS_ARRAY local_status;
	// REQ_EXEC_BLOCK has no outputs so there are no out_msg 
	// supplied from client side, but REQ_EXEC_BLOCK requires
	// 2-byte message for EOS synchronization
	const bool isBlock = (request->req_type == REQ_EXEC_BLOCK);

	if (out_msg_length && (message = request->req_receive) || isBlock) {
		char		temp_buffer[DOUBLE_ALIGN * 2];
		dsql_msg	temp_msg;

		/* Insure that the blr for the message is parsed, regardless of
		   whether anything is found by the call to receive. */

		if (out_msg_length && out_blr_length) {
			parse_blr(out_blr_length, out_blr, out_msg_length,
					  message->msg_parameters);
		} 
		else if (!out_msg_length && isBlock) {
			message = &temp_msg;

			message->msg_number = 1;
			message->msg_length = 2;
			message->msg_buffer = (UCHAR*)FB_ALIGN((U_IPTR) temp_buffer, DOUBLE_ALIGN);
		}

		THREAD_EXIT();
		s = isc_receive(tdsql->tsql_status,
						&request->req_handle,
						message->msg_number,
						message->msg_length,
						message->msg_buffer,
						0);
		THREAD_ENTER();
		if (s) {
			punt();
		}

		if (out_msg_length)
			map_in_out(NULL, message, 0, out_blr, out_msg_length, out_msg);

		// if this is a singleton select, make sure there's in fact one record 

		if (singleton)
		{
			USHORT counter;

			/* Create a temp message buffer and try two more receives.
			   If both succeed then the first is the next record and the
			   second is either another record or the end of record message.
			   In either case, there's more than one record. */

			UCHAR* message_buffer =
				(UCHAR*)gds__alloc((ULONG) message->msg_length);

			s = 0;
			THREAD_EXIT();
			for (counter = 0; counter < 2 && !s; counter++)
			{
				s = isc_receive(local_status,
								&request->req_handle,
								message->msg_number,
								message->msg_length,
								message_buffer,
								0);
			}
			THREAD_ENTER();
			gds__free(message_buffer);

			/* two successful receives means more than one record
			   a req_sync error on the first pass above means no records
			   a non-req_sync error on any of the passes above is an error */

			if (!s)
			{
				tdsql->tsql_status[0] = isc_arg_gds;
				tdsql->tsql_status[1] = isc_sing_select_err;
				tdsql->tsql_status[2] = isc_arg_end;
				return_status = isc_sing_select_err;
			}
			else if (s == isc_req_sync && counter == 1)
			{
				tdsql->tsql_status[0] = isc_arg_gds;
				tdsql->tsql_status[1] = isc_stream_eof;
				tdsql->tsql_status[2] = isc_arg_end;
				return_status = isc_stream_eof;
			}
			else if (s != isc_req_sync)
			{
				punt();
			}
		}
	}

	if (!(request->req_dbb->dbb_flags & DBB_v3))
	{
		UCHAR buffer[20]; // Not used after retrieved.
		if (request->req_type == REQ_UPDATE_CURSOR)
		{
			GDS_DSQL_SQL_INFO_CPP(	local_status,
									&request,
									sizeof(sql_records_info),
									sql_records_info,
									sizeof(buffer),
									buffer);
			if (!request->req_updates)
			{
				ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 913,
						  isc_arg_gds, isc_deadlock, isc_arg_gds,
						  isc_update_conflict, 0);
			}
		}
		else if (request->req_type == REQ_DELETE_CURSOR)
		{
			GDS_DSQL_SQL_INFO_CPP(	local_status,
									&request,
									sizeof(sql_records_info),
									sql_records_info,
									sizeof(buffer),
									buffer);
			if (!request->req_deletes)
			{
				ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 913,
						  isc_arg_gds, isc_deadlock, isc_arg_gds,
						  isc_update_conflict, 0);
			}
		}
	}
	return return_status;
}


/**
  
 	filter_sub_type
  
    @brief	Determine the sub_type to use in filtering
 	a blob.
 

    @param request
    @param node

 **/
static SSHORT filter_sub_type( dsql_req* request, const dsql_nod* node)
{
	if (node->nod_type == nod_constant)
		return (SSHORT)(IPTR) node->nod_arg[0];

	const dsql_par* parameter = (dsql_par*) node->nod_arg[e_par_parameter];
	const dsql_par* null = parameter->par_null;
	if (null)
		if (*((SSHORT *) null->par_desc.dsc_address))
			return 0;

	return *((SSHORT *) parameter->par_desc.dsc_address);
}


/**
  
 	get_indices
  
    @brief	Retrieve the indices from the index tree in
 	the request info buffer (explain_ptr), and print them out
 	in the plan buffer. Return true on success and false on failure.
 

    @param explain_length_ptr
    @param explain_ptr
    @param plan_length_ptr
    @param plan_ptr

 **/
static bool get_indices(
						   SSHORT* explain_length_ptr,
						   const SCHAR** explain_ptr,
						   SSHORT* plan_length_ptr, SCHAR** plan_ptr)
{
	USHORT length;

	SSHORT explain_length = *explain_length_ptr;
	const SCHAR* explain = *explain_ptr;
	SSHORT plan_length = *plan_length_ptr;
	SCHAR* plan = *plan_ptr;

/* go through the index tree information, just
   extracting the indices used */

	explain_length--;
	switch (*explain++) {
	case isc_info_rsb_and:
	case isc_info_rsb_or:
		if (!get_indices(&explain_length, &explain, &plan_length, &plan))
			return false;
		if (!get_indices(&explain_length, &explain, &plan_length, &plan))
			return false;
		break;

	case isc_info_rsb_dbkey:
		break;

	case isc_info_rsb_index:
		explain_length--;
		length = *explain++;

		// if this isn't the first index, put out a comma 

		if (plan[-1] != '(' && plan[-1] != ' ') {
			plan_length -= 2;
			if (plan_length < 0)
				return false;
			*plan++ = ',';
			*plan++ = ' ';
		}

		// now put out the index name 

		if ((plan_length -= length) < 0)
			return false;
		explain_length -= length;
		while (length--)
			*plan++ = *explain++;
		break;

	default:
		return false;
	}

	*explain_length_ptr = explain_length;
	*explain_ptr = explain;
	*plan_length_ptr = plan_length;
	*plan_ptr = plan;

	return true;
}


/**
  
 	get_plan_info
  
    @brief	Get the access plan for the request and turn
 	it into a textual representation suitable for
 	human reading.
 

    @param request
    @param buffer_length
    @param buffer

 **/
static USHORT get_plan_info(
							dsql_req* request,
							SSHORT buffer_length, SCHAR** out_buffer)
{
	SCHAR explain_buffer[256];

	ISC_STATUS s;

	tsql* tdsql = DSQL_get_thread_data();
	memset(explain_buffer, 0, sizeof(explain_buffer));
	SCHAR* explain_ptr = explain_buffer;
	SCHAR* buffer_ptr = *out_buffer;

// get the access path info for the underlying request from the engine 

	THREAD_EXIT();
	s = isc_request_info(tdsql->tsql_status,
						 &request->req_handle,
						 0,
						 sizeof(explain_info),
						 explain_info,
						 sizeof(explain_buffer), explain_buffer);
	THREAD_ENTER();

	if (s)
		return 0;

	if (*explain_buffer == isc_info_truncated) {
		explain_ptr = (SCHAR *) gds__alloc(BUFFER_XLARGE);
		// CVC: Added test for memory exhaustion here.
		// Should we throw an exception or simply return 0 to the caller?
		if (!explain_ptr) {
			return 0;
		}

		THREAD_EXIT();
		s = isc_request_info(tdsql->tsql_status,
							 &request->req_handle, 0,
							 sizeof(explain_info),
							 explain_info,
							 BUFFER_XLARGE, explain_ptr);
		THREAD_ENTER();

		if (s || *explain_ptr == isc_info_truncated) {
			// CVC: Before returning, deallocate the buffer!
			gds__free(explain_ptr);
			return 0;
		}
	}

	SCHAR* plan;
	for (int i = 0; i < 2; i++) {
		const SCHAR* explain = explain_ptr;
		if (*explain++ != isc_info_access_path)
		{
			// CVC: deallocate memory!
			if (explain_ptr != explain_buffer) {
				gds__free(explain_ptr);
			}
			return 0;
		}

		SSHORT explain_length = (UCHAR) *explain++;
		explain_length += (UCHAR) (*explain++) << 8;

		plan = buffer_ptr;

		/* CVC: What if we need to do 2nd pass? Those variables were only initialized
		at the begining of the function hence they had trash the second time. */
		USHORT join_count = 0, level = 0;

		// keep going until we reach the end of the explain info 

		while (explain_length > 0 && buffer_length > 0) {
			if (!get_rsb_item(&explain_length, &explain, &buffer_length, &plan,
							  &join_count, &level)) 
			{
				// assume we have run out of room in the buffer, try again with a larger one 
				char* temp = reinterpret_cast<char*>(gds__alloc(BUFFER_XLARGE));
				if (!temp) {
					// NOMEM. Do not attempt one more try
					i++;
					continue;
				}
				else {
					buffer_ptr = temp;
					buffer_length = BUFFER_XLARGE;
				}
				break;
			}
		}

		if (buffer_ptr == *out_buffer)
			break;
	}


	if (explain_ptr != explain_buffer) {
		gds__free(explain_ptr);
	}

	*out_buffer = buffer_ptr;
	return plan - *out_buffer;
}


/**
  
 	get_request_info
  
    @brief	Get the records updated/deleted for record
 

    @param request
    @param buffer_length
    @param buffer

 **/
static USHORT get_request_info(
							   dsql_req* request,
							   SSHORT buffer_length, UCHAR* buffer)
{
	tsql* tdsql = DSQL_get_thread_data();

// get the info for the request from the engine 

	THREAD_EXIT();
	const ISC_STATUS s = isc_request_info(tdsql->tsql_status,
						 &request->req_handle,
						 0,
						 sizeof(record_info),
						 record_info,
						 buffer_length, reinterpret_cast<char*>(buffer));
	THREAD_ENTER();

	if (s)
		return 0;

	const UCHAR* data = buffer;

	request->req_updates = request->req_deletes = 0;
	request->req_selects = request->req_inserts = 0;

	UCHAR p;
	while ((p = *data++) != isc_info_end) {
		const USHORT data_length =
			static_cast<USHORT>(gds__vax_integer(data, 2));
		data += 2;

		switch (p) {
		case isc_info_req_update_count:
			request->req_updates = gds__vax_integer(data, data_length);
			break;

		case isc_info_req_delete_count:
			request->req_deletes = gds__vax_integer(data, data_length);
			break;

		case isc_info_req_select_count:
			request->req_selects = gds__vax_integer(data, data_length);
			break;

		case isc_info_req_insert_count:
			request->req_inserts = gds__vax_integer(data, data_length);
			break;

		default:
			break;
		}

		data += data_length;
	}

	return data - buffer;
}


/**
  
 	get_rsb_item
  
    @brief	Use recursion to print out a reverse-polish
 	access plan of joins and join types. Return true on success
 	and false on failure
 

    @param explain_length_ptr
    @param explain_ptr
    @param plan_length_ptr
    @param plan_ptr
    @param parent_join_count
    @param level_ptr

 **/
static bool get_rsb_item(SSHORT*		explain_length_ptr,
							const SCHAR**	explain_ptr,
							SSHORT*		plan_length_ptr,
							SCHAR**		plan_ptr,
							USHORT*		parent_join_count,
							USHORT*		level_ptr)
{
	const SCHAR* p;
	SSHORT rsb_type;

	SSHORT explain_length = *explain_length_ptr;
	const SCHAR* explain = *explain_ptr;
	SSHORT plan_length = *plan_length_ptr;
	SCHAR* plan = *plan_ptr;

	explain_length--;
	switch (*explain++) {
	case isc_info_rsb_begin:
		if (!*level_ptr) {
			// put out the PLAN prefix 

			p = "\nPLAN ";
			if ((plan_length -= strlen(p)) < 0)
				return false;
			while (*p)
				*plan++ = *p++;
		}

		(*level_ptr)++;
		break;

	case isc_info_rsb_end:
		if (*level_ptr) {
			(*level_ptr)--;
		}
		/* else --*parent_join_count; ??? */
		break;

	case isc_info_rsb_relation:

		/* for the single relation case, initiate
		   the relation with a parenthesis */

		if (!*parent_join_count) {
			if (--plan_length < 0)
				return false;
			*plan++ = '(';
		}

		// if this isn't the first relation, put out a comma 

		if (plan[-1] != '(') {
			plan_length -= 2;
			if (plan_length < 0)
				return false;
			*plan++ = ',';
			*plan++ = ' ';
		}

		// put out the relation name 
		{ // scope to keep length local.
		explain_length--;
		SSHORT length = (UCHAR) * explain++;
		explain_length -= length;
		if ((plan_length -= length) < 0)
			return false;
		while (length--)
			*plan++ = *explain++;
		} // scope
		break;

	case isc_info_rsb_type:
		explain_length--;
		switch (rsb_type = *explain++) {
			/* for stream types which have multiple substreams, print out
			   the stream type and recursively print out the substreams so
			   we will know where to put the parentheses */

		case isc_info_rsb_union:

			// put out all the substreams of the join 
			{ // scope to have union_count, union_level and union_join_count local.
			explain_length--;
			USHORT union_count = (USHORT) * explain++ - 1;

			// finish the first union member 

			USHORT union_level = *level_ptr;
			USHORT union_join_count = 0;
			while (explain_length > 0 && plan_length > 0) {
				if (!get_rsb_item(&explain_length, &explain, &plan_length, &plan,
								  &union_join_count, &union_level)) 
				{
					return false;
				}
				if (union_level == *level_ptr)
					break;
			}

			/* for the rest of the members, start the level at 0 so each
			   gets its own "PLAN ... " line */

			while (union_count) {
				union_join_count = 0;
				union_level = 0;
				while (explain_length > 0 && plan_length > 0) {
					if (!get_rsb_item(&explain_length, &explain, &plan_length,
									  &plan, &union_join_count, &union_level)) 
					{
						return false;
					}
					if (!union_level)
						break;
				}
				union_count--;
			}
			} // scope
			break;

		case isc_info_rsb_cross:
		case isc_info_rsb_left_cross:
		case isc_info_rsb_merge:

			/* if this join is itself part of a join list,
			   but not the first item, then put out a comma */

			if (*parent_join_count && plan[-1] != '(') {
				plan_length -= 2;
				if (plan_length < 0)
					return false;
				*plan++ = ',';
				*plan++ = ' ';
			}

			// put out the join type 

			if (rsb_type == isc_info_rsb_cross ||
				rsb_type == isc_info_rsb_left_cross)
			{
				p = "JOIN (";
			}
			else {
				p = "MERGE (";
			}

			if ((plan_length -= strlen(p)) < 0)
				return false;
			while (*p)
				*plan++ = *p++;

			// put out all the substreams of the join 

			explain_length--;
			{ // scope to have join_count local.
			USHORT join_count = (USHORT) * explain++;
			while (join_count && explain_length > 0 && plan_length > 0) {
				if (!get_rsb_item(&explain_length, &explain, &plan_length,
								  &plan, &join_count, level_ptr))
				{
					return false;
				}
				// CVC: Here's the additional stop condition. 
				if (!*level_ptr) {
					break;
				}
			}
			} // scope

			// put out the final parenthesis for the join 

			if (--plan_length < 0)
				return false;
			else
				*plan++ = ')';

			// this qualifies as a stream, so decrement the join count 

			if (*parent_join_count)
				-- * parent_join_count;
			break;

		case isc_info_rsb_indexed:
		case isc_info_rsb_navigate:
		case isc_info_rsb_sequential:
		case isc_info_rsb_ext_sequential:
		case isc_info_rsb_ext_indexed:
			if (rsb_type == isc_info_rsb_indexed ||
				rsb_type == isc_info_rsb_ext_indexed) 
			{
				p = " INDEX (";
			}
			else if (rsb_type == isc_info_rsb_navigate)
				p = " ORDER ";
			else
				p = " NATURAL";

			if ((plan_length -= strlen(p)) < 0)
				return false;
			while (*p)
				*plan++ = *p++;

			// print out additional index information 

			if (rsb_type == isc_info_rsb_indexed ||
				rsb_type == isc_info_rsb_navigate ||
				rsb_type == isc_info_rsb_ext_indexed)
			{
				if (!get_indices(&explain_length, &explain, &plan_length, &plan))
					return false;
			}

			if (rsb_type == isc_info_rsb_navigate &&
				*explain == isc_info_rsb_indexed)
			{
				USHORT idx_count = 1;
				if (!get_rsb_item(&explain_length, &explain, &plan_length,
								  &plan, &idx_count, level_ptr))
				{
					return false;
				}
			}

			if (rsb_type == isc_info_rsb_indexed ||
				rsb_type == isc_info_rsb_ext_indexed)
			{
				if (--plan_length < 0)
					return false;
				*plan++ = ')';
			}

			// detect the end of a single relation and put out a final parenthesis 

			if (!*parent_join_count)
				if (--plan_length < 0)
					return false;
				else
					*plan++ = ')';

			// this also qualifies as a stream, so decrement the join count 

			if (*parent_join_count)
				-- * parent_join_count;
			break;

		case isc_info_rsb_sort:

			/* if this sort is on behalf of a union, don't bother to
			   print out the sort, because unions handle the sort on all
			   substreams at once, and a plan maps to each substream
			   in the union, so the sort doesn't really apply to a particular plan */

			if (explain_length > 2 &&
				(explain[0] == isc_info_rsb_begin) &&
				(explain[1] == isc_info_rsb_type) &&
				(explain[2] == isc_info_rsb_union))
			{
				break;
			}

			// if this isn't the first item in the list, put out a comma 

			if (*parent_join_count && plan[-1] != '(') {
				plan_length -= 2;
				if (plan_length < 0)
					return false;
				*plan++ = ',';
				*plan++ = ' ';
			}

			p = "SORT (";

			if ((plan_length -= strlen(p)) < 0)
				return false;
			while (*p)
				*plan++ = *p++;

			/* the rsb_sort should always be followed by a begin...end block,
			   allowing us to include everything inside the sort in parentheses */

			{ // scope to have save_level local.
			const USHORT save_level = *level_ptr;
			while (explain_length > 0 && plan_length > 0) {
				if (!get_rsb_item(&explain_length, &explain, &plan_length,
								  &plan, parent_join_count, level_ptr))
				{
					return false;
				}
				if (*level_ptr == save_level)
					break;
			}

			if (--plan_length < 0)
				return false;
			*plan++ = ')';
			} // scope
			break;

		default:
			break;
		} // switch (rsb_type = *explain++)
		break;

	default:
		break;
	}

	*explain_length_ptr = explain_length;
	*explain_ptr = explain;
	*plan_length_ptr = plan_length;
	*plan_ptr = plan;

	return true;
}


/**
  
 	init
  
    @brief	Initialize dynamic SQL.  This is called only once.
 

    @param db_handle

 **/
static dsql_dbb* init(FB_API_HANDLE* db_handle)
{
	THREAD_EXIT();
	THD_MUTEX_LOCK(&databases_mutex);
	THREAD_ENTER();

	if (!init_flag)
	{
		init_flag = true;
		ALLD_init();
		
		// may be someone needs context pool later - 
		// lets set it correctly here, not in ALLD_init()
		DsqlContextPoolHolder context(DSQL_get_thread_data(), DSQL_permanent_pool);
		HSHD_init();

#ifdef DSQL_DEBUG
		DSQL_debug = Config::getTraceDSQL();
#endif

		LEX_dsql_init();

		gds__register_cleanup(cleanup, 0);
	}

	if (!db_handle) {
		THD_MUTEX_UNLOCK(&databases_mutex);
		return NULL;
	}

	// Look for database block.  If we don't find one, allocate one.

	dsql_dbb* database;
	for (database = databases; database; database = database->dbb_next)
	{
		if (database->dbb_database_handle == *db_handle) {
			THD_MUTEX_UNLOCK(&databases_mutex);
			return database;
		}
	}

	DsqlMemoryPool* pool = DsqlMemoryPool::createPool();
	database = FB_NEW(*pool) dsql_dbb;
	database->dbb_pool = pool;
	database->dbb_next = databases;
	databases = database;
	database->dbb_database_handle = *db_handle;
	THD_MUTEX_UNLOCK(&databases_mutex);

	ISC_STATUS_ARRAY user_status;

	THREAD_EXIT();
	isc_database_cleanup(user_status, db_handle, cleanup_database, NULL);
	THREAD_ENTER();

// Determine if the database is V3 or V4 

	SCHAR buffer[128];

	database->dbb_flags |= DBB_v3;
	THREAD_EXIT();
	const ISC_STATUS s =
		isc_database_info(user_status, db_handle,
						  sizeof(db_hdr_info_items),
						  db_hdr_info_items,
						  sizeof(buffer), buffer);
	THREAD_ENTER();

	if (s) {
		return database;
	}

/* assume that server can not report current character set,
   and if not then emulate pre-patch actions. */
	database->dbb_att_charset = 127;

	const UCHAR* data = reinterpret_cast<UCHAR*>(buffer);
	UCHAR p;
	while ((p = *data++) != isc_info_end)
	{
		const SSHORT l = static_cast<SSHORT>(gds__vax_integer(data, 2));
		data += 2;

		switch (p)
		{
		case isc_info_db_sql_dialect:
			database->dbb_db_SQL_dialect = (USHORT) data[0];
			break;

		case isc_info_ods_version:
			if (gds__vax_integer(data, l) > 7)
				database->dbb_flags &= ~DBB_v3;
			break;

			/* This flag indicates the version level of the engine
			   itself, so we can tell what capabilities the engine
			   code itself (as opposed to the on-disk structure).
			   Apparently the base level up to now indicated the major
			   version number, but for 4.1 the base level is being
			   incremented, so the base level indicates an engine version
			   as follows:
			   1 == v1.x
			   2 == v2.x
			   3 == v3.x
			   4 == v4.0 only
			   5 == v4.1
			   Note: this info item is so old it apparently uses an
			   archaic format, not a standard vax integer format.
			 */

		case isc_info_base_level:
			database->dbb_base_level = (USHORT) data[1];
			break;

		case isc_info_db_read_only:
			if ((USHORT) data[0])
				database->dbb_flags |= DBB_read_only;
			else
				database->dbb_flags &= ~DBB_read_only;
			break;

		case frb_info_att_charset:
			database->dbb_att_charset = static_cast<SSHORT>(gds__vax_integer(data, 2));
			break;

		default:
			break;
		}

		data += l;
	}

	return database;
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
    @param dsql_msg

 **/
static void map_in_out(	dsql_req*		request,
						dsql_msg*		message,
						USHORT	blr_length,
						const UCHAR*	blr,
						USHORT	msg_length,
						UCHAR*	dsql_msg_buf)
{
	dsql_par* parameter;

	USHORT count = parse_blr(blr_length, blr, msg_length, message->msg_parameters);

/* When mapping data from the external world, request will be non-NULL.
   When mapping data from an internal message, request will be NULL. */

	for (parameter = message->msg_parameters; parameter;
		 parameter = parameter->par_next)
	{
		if (parameter->par_index)
		{
			 // Make sure the message given to us is long enough 

			DSC    desc   = parameter->par_user_desc;
			USHORT length = (IPTR) desc.dsc_address + desc.dsc_length;
			if (length > msg_length)
				break;
			if (!desc.dsc_dtype)
				break;

			SSHORT* flag = NULL;
			dsql_par* null = parameter->par_null;
			if (null != NULL)
			{
				const USHORT null_offset = (IPTR) null->par_user_desc.dsc_address;
				length = null_offset + sizeof(SSHORT);
				if (length > msg_length)
					break;

				if (!request) {
					flag = (SSHORT *) (dsql_msg_buf + null_offset);
					*flag = *((SSHORT *) null->par_desc.dsc_address);
				}
				else {
					flag = (SSHORT *) null->par_desc.dsc_address;
					*flag = *((SSHORT *) (dsql_msg_buf + null_offset));
				}
			}

			desc.dsc_address = dsql_msg_buf + (IPTR) desc.dsc_address;
			if (!request)
				MOVD_move(&parameter->par_desc, &desc);
			else if (!flag || *flag >= 0)
				MOVD_move(&desc, &parameter->par_desc);

			count--;
		}
	}

/* If we got here because the loop was exited early or if part of the
   message given to us hasn't been used, complain. */

	if (parameter || count)
	{
		ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 804,
				  isc_arg_gds, isc_dsql_sqlda_err, 0);
	}

	dsql_par* dbkey;
	if (request &&
		((dbkey = request->req_parent_dbkey) != NULL) &&
		((parameter = request->req_dbkey) != NULL))
	{
		MOVD_move(&dbkey->par_desc, &parameter->par_desc);
		dsql_par* null = parameter->par_null;
		if (null != NULL)
		{
			SSHORT* flag = (SSHORT *) null->par_desc.dsc_address;
			*flag = 0;
		}
	}

	dsql_par* rec_version;
	if (request &&
		((rec_version = request->req_parent_rec_version) != NULL) &&
		((parameter = request->req_rec_version) != NULL))
	{
		MOVD_move(&rec_version->par_desc, &parameter->par_desc);
		dsql_par* null = parameter->par_null;
		if (null != NULL)
		{
			SSHORT* flag = (SSHORT *) null->par_desc.dsc_address;
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
static USHORT parse_blr(
						USHORT blr_length,
						const UCHAR* blr, const USHORT msg_length, dsql_par* parameters)
{
/* If there's no blr length, then the format of the current message buffer
   is identical to the format of the previous one. */

	if (!blr_length)
	{
		USHORT par_count = 0;
		for (const dsql_par* parameter = parameters; parameter;
			 parameter = parameter->par_next)
		{
			if (parameter->par_index) {
				++par_count;
			}
		}
		return par_count;
	}

	if (*blr != blr_version4 && *blr != blr_version5)
	{
		ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 804,
				  isc_arg_gds, isc_dsql_sqlda_err, 0);
	}
	blr++;						// skip the blr_version 
	if (*blr++ != blr_begin || *blr++ != blr_message)
	{
		ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 804,
				  isc_arg_gds, isc_dsql_sqlda_err, 0);
	}

	++blr;						// skip the message number 
	USHORT count = *blr++;
	count += (*blr++) << 8;
	count /= 2;

	USHORT offset = 0;
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
#ifndef VMS
		case blr_d_float:
#endif
			desc.dsc_dtype = dtype_double;
			desc.dsc_length = sizeof(double);
			break;

#ifdef VMS
		case blr_d_float:
			desc.dsc_dtype = dtype_d_float;
			desc.dsc_length = sizeof(double);
			break;
#endif

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

		default:
			ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 804,
					  isc_arg_gds, isc_dsql_sqlda_err, 0);
		}

		USHORT align = type_alignments[desc.dsc_dtype];
		if (align)
			offset = FB_ALIGN(offset, align);
		desc.dsc_address = (UCHAR*)(IPTR) offset;
		offset += desc.dsc_length;

		if (*blr++ != blr_short || *blr++ != 0)
			ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 804,
					  isc_arg_gds, isc_dsql_sqlda_err, 0);

		align = type_alignments[dtype_short];
		if (align)
			offset = FB_ALIGN(offset, align);
		USHORT null_offset = offset;
		offset += sizeof(SSHORT);

		for (dsql_par* parameter = parameters; parameter; parameter = parameter->par_next)
		{
			if (parameter->par_index == index) {
				parameter->par_user_desc = desc;
				dsql_par* null = parameter->par_null;
				if (null) {
					null->par_user_desc.dsc_dtype = dtype_short;
					null->par_user_desc.dsc_scale = 0;
					null->par_user_desc.dsc_length = sizeof(SSHORT);
					null->par_user_desc.dsc_address = (UCHAR*)(IPTR) null_offset;
				}
			}
		}
	}

	if (*blr++ != (UCHAR) blr_end || offset != msg_length)
	{
		ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 804,
				  isc_arg_gds, isc_dsql_sqlda_err, 0);
	}

	return count;
}


/**
  
 	prepare
  
    @brief	Prepare a statement for execution.  Return SQL status
 	code.  Note: caller is responsible for pool handling.
 

    @param request
    @param string_length
    @param string
    @param client_dialect
    @param parser_version

 **/
static dsql_req* prepare(
				   dsql_req* request,
				   USHORT string_length,
				   const TEXT* string,
				   USHORT client_dialect, USHORT parser_version)
{
	ISC_STATUS_ARRAY local_status;

	tsql* tdsql = DSQL_get_thread_data();

	MOVE_CLEAR(local_status, sizeof(ISC_STATUS) * ISC_STATUS_LENGTH);

	if (client_dialect > SQL_DIALECT_CURRENT)
		ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 901,
				  isc_arg_gds, isc_wish_list, 0);

	if (!string_length)
		string_length = strlen(string);

/* Get rid of the trailing ";" if there is one. */

	for (const TEXT* p = string + string_length; p-- > string;)
	{
		if (*p != ' ') {
			if (*p == ';')
				string_length = p - string;
			break;
		}
	}

// Allocate a storage pool and get ready to parse 

	LEX_string(string, string_length, request->req_dbb->dbb_att_charset);

// Parse the SQL statement.  If it croaks, return 

	bool stmt_ambiguous = false;
	if (dsql_yyparse(client_dialect,
	                 request->req_dbb->dbb_db_SQL_dialect,
	                 parser_version,
	                 &stmt_ambiguous))
	{
		ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 104,
			isc_arg_gds, isc_command_end_err,	// Unexpected end of command
				  0);
	}

// allocate the send and receive messages 

	request->req_send = FB_NEW(*tdsql->getDefaultPool()) dsql_msg;
	dsql_msg* message = FB_NEW(*tdsql->getDefaultPool()) dsql_msg;
	request->req_receive = message;
	message->msg_number = 1;

#ifdef SCROLLABLE_CURSORS
	if (request->req_dbb->dbb_base_level >= 5) {
		/* allocate a message in which to send scrolling information
		   outside of the normal send/receive protocol */

		request->req_async = message = FB_NEW(*tdsql->getDefaultPool()) dsql_msg;
		message->msg_number = 2;
	}
#endif

	request->req_type = REQ_SELECT;
	request->req_flags &= ~(REQ_cursor_open | REQ_embedded_sql_cursor);

/*
 * No work is done during pass1 for set transaction - like
 * checking for valid table names.  This is because that will
 * require a valid transaction handle.
 * Error will be caught at execute time.
 */

	dsql_nod* node = PASS1_statement(request, DSQL_parse, false);
	if (!node)
		return request;

// stop here for requests not requiring code generation 

	if (request->req_type == REQ_DDL && stmt_ambiguous &&
		request->req_dbb->dbb_db_SQL_dialect != client_dialect)
	{
		ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 817,
				  isc_arg_gds, isc_ddl_not_allowed_by_db_sql_dial,
				  isc_arg_number,
				  (SLONG) request->req_dbb->dbb_db_SQL_dialect, 0);
	}

	if (request->req_type == REQ_COMMIT ||
		request->req_type == REQ_COMMIT_RETAIN ||
		request->req_type == REQ_ROLLBACK)
	{
		return request;
	}

// Work on blob segment requests 

	if (request->req_type == REQ_GET_SEGMENT ||
		request->req_type == REQ_PUT_SEGMENT)
	{
		GEN_port(request, request->req_blob->blb_open_in_msg);
		GEN_port(request, request->req_blob->blb_open_out_msg);
		GEN_port(request, request->req_blob->blb_segment_msg);
		return request;
	}

// Generate BLR, DDL or TPB for request 

#ifdef NOT_USED_OR_REPLACED

	//What's a reason not to bloat request_pool?
	//It was made to keep data local for that request...

	if (request->req_type == REQ_START_TRANS ||
		request->req_type == REQ_DDL ||
		request->req_type == REQ_EXEC_PROCEDURE)
	{
		// Allocate persistent blr string from request's pool. 

		request->req_blr_string = FB_NEW_RPT(*tdsql->getDefaultPool(), 980) dsql_str;
	}
	else {
		/* Allocate transient blr string from permanent pool so
		   as not to unnecessarily bloat the request's pool. */

		request->req_blr_string = FB_NEW_RPT(*DSQL_permanent_pool, 980) dsql_str;
	}
	request->req_blr_string->str_length = 980;
	request->req_blr = request->req_blr_string->str_data;
	request->req_blr_yellow =
		request->req_blr + request->req_blr_string->str_length;
#endif

/* Start transactions takes parameters via a parameter block.
   The request blr string is used for that. */

	if (request->req_type == REQ_START_TRANS) {
		GEN_start_transaction(request, node);
		return request;
	}

	if (client_dialect > SQL_DIALECT_V5)
		request->req_flags |= REQ_blr_version5;
	else
		request->req_flags |= REQ_blr_version4;
	GEN_request(request, node);
	const USHORT length = request->req_blr_data.getCount();
	
// stop here for ddl requests 

	if (request->req_type == REQ_DDL)
		return request;

// have the access method compile the request 

#ifdef DSQL_DEBUG
	if (DSQL_debug & 64) {
		dsql_trace("Resulting BLR code for DSQL:");
		gds__print_blr(request->req_blr_data.begin(),
			gds__trace_printer, 0, 0);
	}
#endif

// check for warnings 
	if (tdsql->tsql_status[2] == isc_arg_warning) {
		// save a status vector 
		MOVE_FASTER(tdsql->tsql_status, local_status,
					sizeof(ISC_STATUS) * ISC_STATUS_LENGTH);
	}

	THREAD_EXIT();
	const ISC_STATUS status = isc_compile_request(tdsql->tsql_status,
								 &request->req_dbb->dbb_database_handle,
								 &request->req_handle, length,
								 (const char*)(request->req_blr_data.begin()));
	THREAD_ENTER();

// restore warnings (if there are any) 
	if (local_status[2] == isc_arg_warning)
	{
		int indx, len, warning;

		// find end of a status vector 
		PARSE_STATUS(tdsql->tsql_status, indx, warning);
		if (indx)
			--indx;

		// calculate length of saved warnings 
		PARSE_STATUS(local_status, len, warning);
		len -= 2;

		if ((len + indx - 1) < ISC_STATUS_LENGTH)
			MOVE_FASTER(&local_status[2], &tdsql->tsql_status[indx],
						sizeof(ISC_STATUS) * len);
	}

// free blr memory
	request->req_blr_data.free();

	if (status)
		punt();

	return request;
}


/**
  
 	punt
  
    @brief	Report a signification error.
 


 **/
static void punt(void)
{
	tsql* tdsql = DSQL_get_thread_data();

	Firebird::status_exception::raise(tdsql->tsql_status);
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
						USHORT	length,
						const UCHAR* string,
						UCHAR*	ptr,
						const UCHAR* const end)
{

	if (ptr + length + 3 >= end) {
		*ptr = isc_info_truncated;
		return NULL;
	}

	*ptr++ = item;

	*ptr++ = (UCHAR)length;
	*ptr++ = length >> 8;

	if (length) {
		do {
			*ptr++ = *string++;
		} while (--length);
	}

	return ptr;
}


/**
  
 	release_request
  
    @brief	Release a dynamic request.
 

    @param request
    @param top_level

 **/
static void release_request(dsql_req* request, bool top_level)
{

	ISC_STATUS_ARRAY status_vector;

	tsql* tdsql = DSQL_get_thread_data();

	// If request is parent, orphan the children and
	// release a portion of their requests

	for (dsql_req* child = request->req_offspring; child; child = child->req_sibling)
	{
		child->req_flags |= REQ_orphan;
		child->req_parent = NULL;
		DsqlContextPoolHolder context(tdsql, &child->req_pool);
		release_request(child, false);
	}

// For top level requests that are linked to a parent, unlink it

	if (top_level && request->req_parent)
	{
		dsql_req* parent = request->req_parent;
		for (dsql_req** ptr = &parent->req_offspring;
			*ptr;
			ptr = &(*ptr)->req_sibling)
		{
			if (*ptr == request) {
				*ptr = request->req_sibling;
				break;
			}
		}
	}

// If the request had an open cursor, close it

	if (request->req_open_cursor) {
		close_cursor(request);
	}

// If request is named, clear it from the hash table

	if (request->req_name) {
		HSHD_remove(request->req_name);
		request->req_name = NULL;
	}

	if (request->req_cursor) {
		HSHD_remove(request->req_cursor);
		request->req_cursor = NULL;
	}

// If a request has been compiled, release it now

	if (request->req_handle) {
		THREAD_EXIT();
		isc_release_request(status_vector, &request->req_handle);
		THREAD_ENTER();
	}

// free blr memory
	request->req_blr_data.free();

// Only release the entire request for top level requests 

	if (top_level)
		DsqlMemoryPool::deletePool(&request->req_pool);
}


/**
  
 	return_success
  
    @brief	Set up status vector to reflect successful execution.
 


 **/
static ISC_STATUS return_success(void)
{

	tsql* tdsql = DSQL_get_thread_data();

	ISC_STATUS* p = tdsql->tsql_status;
	*p++ = isc_arg_gds;
	*p++ = FB_SUCCESS;

	// do not overwrite warnings
	if (*p != isc_arg_warning) {
		*p = isc_arg_end;
	}

	return FB_SUCCESS;
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
static UCHAR* var_info(
					   dsql_msg* message,
					   const UCHAR* items,
					   const UCHAR* const end_describe,
					   UCHAR* info, 
					   const UCHAR* const end, 
					   USHORT first_index)
{
	UCHAR buf[128];
	SLONG sql_type, sql_sub_type, sql_scale, sql_len;

	if (!message || !message->msg_index)
		return info;

	for (const dsql_par* param = message->msg_par_ordered; param;
		param = param->par_ordered)
	{
		if (param->par_index && param->par_index >= first_index) {
			sql_len = param->par_desc.dsc_length;
			sql_sub_type = 0;
			sql_scale = 0;
			switch (param->par_desc.dsc_dtype) {
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
				sql_type = SQL_TEXT;
				sql_sub_type = param->par_desc.dsc_sub_type;
				break;

			case dtype_blob:
				sql_type = SQL_BLOB;
				sql_sub_type = param->par_desc.dsc_sub_type;
				break;

			case dtype_varying:
				sql_type = SQL_VARYING;
				sql_len -= sizeof(USHORT);
				sql_sub_type = param->par_desc.dsc_sub_type;
				break;

			case dtype_short:
			case dtype_long:
			case dtype_int64:
				if (param->par_desc.dsc_dtype == dtype_short)
					sql_type = SQL_SHORT;
				else if (param->par_desc.dsc_dtype == dtype_long)
					sql_type = SQL_LONG;
				else
					sql_type = SQL_INT64;
				sql_scale = param->par_desc.dsc_scale;
				if (param->par_desc.dsc_sub_type)
					sql_sub_type = param->par_desc.dsc_sub_type;
				break;

			case dtype_quad:
				sql_type = SQL_QUAD;
				sql_scale = param->par_desc.dsc_scale;
				break;

			default:
				ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 804,
						  isc_arg_gds, isc_dsql_datatype_err, 0);
			}

			if (sql_type && (param->par_desc.dsc_flags & DSC_nullable))
				sql_type++;

			for (const UCHAR* describe = items; describe < end_describe;) {
				USHORT length;
				const TEXT* name;
				const UCHAR* buffer = buf;
				UCHAR item = *describe++;
				switch (item) {
				case isc_info_sql_sqlda_seq:
					length = convert((SLONG) param->par_index, buf);
					break;

				case isc_info_sql_message_seq:
					length = 0;
					break;

				case isc_info_sql_type:
					length = convert((SLONG) sql_type, buf);
					break;

				case isc_info_sql_sub_type:
					length = convert((SLONG) sql_sub_type, buf);
					break;

				case isc_info_sql_scale:
					length = convert((SLONG) sql_scale, buf);
					break;

				case isc_info_sql_length:
					length = convert((SLONG) sql_len, buf);
					break;

				case isc_info_sql_null_ind:
					length = convert((SLONG) (sql_type & 1), buf);
					break;

				case isc_info_sql_field:
					if (name = param->par_name) {
						length = strlen(name);
						buffer = reinterpret_cast<const UCHAR*>(name);
					}
					else
						length = 0;
					break;

				case isc_info_sql_relation:
					if (name = param->par_rel_name) {
						length = strlen(name);
						buffer = reinterpret_cast<const UCHAR*>(name);
					}
					else
						length = 0;
					break;

				case isc_info_sql_owner:
					if (name = param->par_owner_name) {
						length = strlen(name);
						buffer = reinterpret_cast<const UCHAR*>(name);
					}
					else
						length = 0;
					break;

				case isc_info_sql_relation_alias:
					if (name = param->par_rel_alias) {
						length = strlen(name);
						buffer = reinterpret_cast<const UCHAR*>(name);
					}
					else
						length = 0;
					break;

				case isc_info_sql_alias:
					if (name = param->par_alias) {
						length = strlen(name);
						buffer = reinterpret_cast<const UCHAR*>(name);
					}
					else
						length = 0;
					break;

				default:
					buf[0] = item;
					item = isc_info_error;
					length = 1 + convert((SLONG) isc_infunk, buf + 1);
					break;
				}

				if (!(info = put_item(item, length, buffer, info, end)))
					return info;
			}

			if (info + 1 >= end) {
				*info = isc_info_truncated;
				return NULL;
			}
			*info++ = isc_info_sql_describe_end;
		} // if()
	} // for()

	return info;
}

