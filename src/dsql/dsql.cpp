/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		dsql.c
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
 */
/**************************************************************
V4 Multi-threading changes.

-- direct calls to gds__ () & isc_ () entrypoints

	THREAD_EXIT;
	    gds__ () or isc_ () call.
	THREAD_ENTER;

-- calls through embedded GDML.

the following protocol will be used.  Care should be taken if
nested FOR loops are added.

    THREAD_EXIT;                // last statment before FOR loop

    FOR ...............

	THREAD_ENTER;           // First statment in FOR loop
	.....some C code....
	.....some C code....
	THREAD_EXIT;            // last statment in FOR loop

    END_FOR;

    THREAD_ENTER;               // First statment after FOR loop
***************************************************************/

#include "firebird.h"
#include "fb_exception.h"
#include "../jrd/ib_stdio.h"
#include <stdlib.h>
#include <string.h>
#include "../dsql/dsql.h"
#include "../dsql/node.h"
#include "../dsql/sym.h"
#include "../jrd/gds.h"
#include "../jrd/thd.h"
#include "../jrd/align.h"
#include "../jrd/intl.h"
#include "../jrd/iberr.h"
#include "../dsql/sqlda.h"
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
#include "../jrd/thd_proto.h"
#include "../jrd/why_proto.h"
#include "../jrd/gds.h"
#include "../jrd/y_handle.h"
#include "../common/config/config.h"

#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif

#if defined(WIN_NT)
#define vsnprintf _vsnprintf
#endif

ASSERT_FILENAME
#ifdef VMS
#include <descrip.h>
#endif

static void		cleanup(void*);
static void		cleanup_database(FRBRD**, SLONG);
static void		cleanup_transaction(FRBRD*, SLONG);
static void		close_cursor(DSQL_REQ);
static USHORT	convert(SLONG, UCHAR*);
static ISC_STATUS	error();
static void		execute_blob(DSQL_REQ, USHORT, UCHAR*, USHORT, UCHAR*,
						 USHORT, UCHAR*, USHORT, UCHAR*);
static ISC_STATUS	execute_request(DSQL_REQ, FRBRD**, USHORT, UCHAR*, USHORT, UCHAR*,
							  USHORT, UCHAR*, USHORT, UCHAR*, bool);
static SSHORT	filter_sub_type(DSQL_REQ, DSQL_NOD);
static bool		get_indices(SSHORT*, SCHAR**, SSHORT*, SCHAR**);
static USHORT	get_plan_info(DSQL_REQ, SSHORT, SCHAR**);
static USHORT	get_request_info(DSQL_REQ, SSHORT, SCHAR*);
static bool		get_rsb_item(SSHORT*, SCHAR**, SSHORT*, SCHAR**, USHORT*,
							USHORT*);
static DBB		init(FRBRD**);
static void		map_in_out(DSQL_REQ, DSQL_MSG, USHORT, UCHAR*, USHORT, UCHAR*);
static USHORT	name_length(TEXT*);
static USHORT	parse_blr(USHORT, UCHAR*, USHORT, PAR);
static DSQL_REQ		prepare(DSQL_REQ, USHORT, TEXT*, USHORT, USHORT);
static void		punt(void);
static UCHAR*	put_item(UCHAR, USHORT, UCHAR*, UCHAR*, UCHAR*);
static void		release_request(DSQL_REQ, bool);
static ISC_STATUS	return_success(void);
static UCHAR*	var_info(DSQL_MSG, const UCHAR*, const UCHAR*, UCHAR*, UCHAR*, USHORT);

extern DSQL_NOD DSQL_parse;

#ifdef DSQL_DEBUG
unsigned DSQL_debug;
#endif

static bool init_flag = false;
static DBB databases;
static OPN open_cursors;
static const SCHAR db_hdr_info_items[] = {
	isc_info_db_sql_dialect,
	gds_info_ods_version,
	gds_info_base_level,
	isc_info_db_read_only,
	frb_info_att_charset,
	gds_info_end
};
static const SCHAR explain_info[] = {
	gds_info_access_path
};
static const SCHAR record_info[] = {
	gds_info_req_update_count, gds_info_req_delete_count,
	gds_info_req_select_count, gds_info_req_insert_count
};
static const UCHAR sql_records_info[] = {
	gds_info_sql_records
};

#ifdef	ANY_THREADING
static MUTX_T databases_mutex;
static MUTX_T cursors_mutex;
static bool mutex_inited = false;
#endif


#ifdef DSQL_DEBUG
IMPLEMENT_TRACE_ROUTINE(dsql_trace, "DSQL");
#endif

//////////////////////////////////////////////////////////////////
// declarations of the C++ implementations of the C API functions
// with the matching name.
//////////////////////////////////////////////////////////////////

static
ISC_STATUS GDS_DSQL_ALLOCATE_CPP(	ISC_STATUS*	user_status,
								FRBRD**	db_handle,
								dsql_req**	req_handle);

static
ISC_STATUS GDS_DSQL_EXECUTE_CPP(   ISC_STATUS*			user_status,
							   FRBRD**			trans_handle,
							   dsql_req**		req_handle,
							   USHORT			in_blr_length,
							   UCHAR*			in_blr,
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
							 UCHAR*		blr,
							 USHORT		msg_type,
							 USHORT		msg_length,
							 UCHAR*		dsql_msg
#ifdef SCROLLABLE_CURSORS
							 , USHORT direction, SLONG offset)
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
							UCHAR*	blr,
							USHORT	msg_type,
							USHORT	msg_length,
							UCHAR*	dsql_msg);

static
ISC_STATUS GDS_DSQL_PREPARE_CPP(ISC_STATUS*			user_status,
							WHY_TRA*		trans_handle,
							dsql_req**		req_handle,
							USHORT			length,
							TEXT*			string,
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
								TEXT*		input_cursor,
								USHORT		type);


//////////////////////////////////////////////////////////////////
// Begin : C API -> C++ wrapper functions
// Note that we don't wrap dsql8_execute_immediate since it
// contains no req in its parameter list.
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////

ISC_STATUS DLL_EXPORT
dsql8_allocate_statement(	ISC_STATUS*    user_status,
					FRBRD**    db_handle,
					dsql_req** req_handle)
{
	return GDS_DSQL_ALLOCATE_CPP(user_status, db_handle, req_handle);
}

//////////////////////////////////////////////////////////////////

ISC_STATUS DLL_EXPORT
dsql8_execute(	ISC_STATUS*    user_status,
					WHY_TRA*   trans_handle,
					dsql_req** req_handle,
					USHORT     in_blr_length,
					SCHAR*     in_blr,
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
								reinterpret_cast<UCHAR*>(in_blr),
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

ISC_STATUS DLL_EXPORT
dsql8_fetch(	ISC_STATUS*    user_status,
				dsql_req** req_handle,
				USHORT     blr_length,
				SCHAR*     blr,
				USHORT     msg_type,
				USHORT     msg_length,
				SCHAR*     dsql_msg
#ifdef SCROLLABLE_CURSORS
				 , USHORT direction, SLONG offset
#endif
				 )
{
	return GDS_DSQL_FETCH_CPP(	user_status,
								req_handle,
								blr_length,
								reinterpret_cast<UCHAR*>(blr),
								msg_type,
								msg_length,
								reinterpret_cast<UCHAR*>(dsql_msg)
#ifdef SCROLLABLE_CURSORS
								 , direction, offset
#endif
								);
}

//////////////////////////////////////////////////////////////////

ISC_STATUS DLL_EXPORT
dsql8_free_statement(	ISC_STATUS*    user_status,
				dsql_req** req_handle,
				USHORT     option)
{
	return GDS_DSQL_FREE_CPP(user_status, req_handle, option);
}

//////////////////////////////////////////////////////////////////

ISC_STATUS DLL_EXPORT
dsql8_insert(ISC_STATUS*				user_status,
				struct dsql_req**	req_handle,
				USHORT				blr_length,
				SCHAR*				blr,
				USHORT				msg_type,
				USHORT				msg_length,
				SCHAR*				dsql_msg)
{
	return GDS_DSQL_INSERT_CPP(	user_status,
								req_handle,
								blr_length,
								reinterpret_cast<UCHAR*>(blr),
								msg_type,
								msg_length,
								reinterpret_cast<UCHAR*>(dsql_msg));
}

//////////////////////////////////////////////////////////////////

ISC_STATUS DLL_EXPORT
dsql8_prepare(	ISC_STATUS*				user_status,
					WHY_TRA*			trans_handle,
					struct dsql_req**	req_handle,
					USHORT				length,
					TEXT*				string,
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

ISC_STATUS DLL_EXPORT
dsql8_sql_info(	ISC_STATUS*				user_status,
					struct dsql_req**	req_handle,
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

ISC_STATUS DLL_EXPORT
dsql8_set_cursor(ISC_STATUS*		user_status,
					dsql_req**	req_handle,
					TEXT*		input_cursor,
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
						FRBRD**    db_handle,
						dsql_req** req_handle)
{
	DBB database;
	DSQL_REQ request;
	struct tsql thd_context, *tdsql;

	SET_THREAD_DATA;

    try
    {
		tdsql->tsql_status = user_status;
		tdsql->tsql_default = NULL;

		init(0);

/* If we haven't been initialized yet, do it now */

		database = init(db_handle);

		tdsql->tsql_default = DsqlMemoryPool::createPool();

/* allocate the request block */

		request = FB_NEW(*tdsql->tsql_default) dsql_req;
		request->req_dbb = database;
		request->req_pool = tdsql->tsql_default;

		*req_handle = request;
	}
	catch(const std::exception&)
	{
		RESTORE_THREAD_DATA;
		return tdsql->tsql_status [1];
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
ISC_STATUS DLL_EXPORT GDS_DSQL_EXECUTE_CPP(ISC_STATUS*		user_status,
									   FRBRD**		trans_handle,
									   dsql_req**	req_handle,
									   USHORT		in_blr_length,
									   UCHAR*		in_blr,
									   USHORT		in_msg_type,
									   USHORT		in_msg_length,
									   UCHAR*		in_msg,
									   USHORT		out_blr_length,
									   UCHAR*		out_blr,
									   USHORT		out_msg_type,
									   USHORT		out_msg_length,
									   UCHAR*		out_msg)
{
	DSQL_REQ request;
	OPN open_cursor;
	ISC_STATUS_ARRAY local_status;
	struct tsql thd_context, *tdsql;
	bool singleton;
	ISC_STATUS sing_status;

	SET_THREAD_DATA;

    try
    {
		tdsql->tsql_status = user_status;
		tdsql->tsql_default = NULL;

		init(0);
		sing_status = 0;

		request = *req_handle;
		tdsql->tsql_default = request->req_pool;

		if ((SSHORT) in_msg_type == -1) {
			request->req_type = REQ_EMBED_SELECT;
		}

/* Only allow NULL trans_handle if we're starting a transaction */

		if (*trans_handle == NULL && request->req_type != REQ_START_TRANS)
		{
			ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 901,
				  	gds_arg_gds, gds_bad_trans_handle, 0);
		}

/* If the request is a SELECT or blob statement then this is an open.
   Make sure the cursor is not already open. */

		if (request->req_type == REQ_SELECT ||
			request->req_type == REQ_SELECT_UPD ||
			request->req_type == REQ_EMBED_SELECT ||
			request->req_type == REQ_GET_SEGMENT ||
			request->req_type == REQ_PUT_SEGMENT)
		{
			if (request->req_flags & REQ_cursor_open)
			{
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 502,
					  	gds_arg_gds, gds_dsql_cursor_open_err, 0);
			}
		}

/* A select with a non zero output length is a singleton select */

		if (request->req_type == REQ_SELECT && out_msg_length != 0) {
			singleton = true;
		} else {
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
			request->req_type == REQ_SELECT_UPD ||
			request->req_type == REQ_EMBED_SELECT ||
			request->req_type == REQ_GET_SEGMENT ||
			request->req_type == REQ_PUT_SEGMENT)
		{
			request->req_flags |= REQ_cursor_open |
				((request->
			  	req_type == REQ_EMBED_SELECT) ? REQ_embedded_sql_cursor : 0);

			request->req_open_cursor = open_cursor = FB_NEW(*DSQL_permanent_pool) opn;
			open_cursor->opn_request = request;
			open_cursor->opn_transaction = *trans_handle;
			THD_MUTEX_LOCK(&cursors_mutex);
			open_cursor->opn_next = open_cursors;
			open_cursors = open_cursor;
			THD_MUTEX_UNLOCK(&cursors_mutex);
			THREAD_EXIT;
			gds__transaction_cleanup(local_status,
								 trans_handle,
								 cleanup_transaction, 0);
			THREAD_ENTER;
		}

		if (!sing_status) {
			return return_success();
		}
	}
	catch(const std::exception&)
	{
		RESTORE_THREAD_DATA;
		return tdsql->tsql_status [1];
	}

	RESTORE_THREAD_DATA;
	return sing_status;
}


static ISC_STATUS dsql8_execute_immediate_common(ISC_STATUS*	user_status,
											 WHY_DBB*	db_handle,
											 WHY_TRA*	trans_handle,
											 USHORT		length,
											 TEXT*		string,
											 USHORT		dialect,
											 USHORT		in_blr_length,
											 UCHAR*		in_blr,
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
	DSQL_REQ request;
	DBB database;
	USHORT parser_version;
	ISC_STATUS status;
	struct tsql thd_context, *tdsql;

	SET_THREAD_DATA;

    try
    {
		tdsql->tsql_status = user_status;
		tdsql->tsql_default = NULL;

		database = init(db_handle);

		tdsql->tsql_default = DsqlMemoryPool::createPool();

	/* allocate the request block, then prepare the request */

		request = FB_NEW(*tdsql->tsql_default) dsql_req;
		request->req_dbb = database;
		request->req_pool = tdsql->tsql_default;
		request->req_trans = *trans_handle;

		try {

			if (!length) {
				length = strlen(string);
			}

/* Figure out which parser version to use */
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
				if (length > max_diag_len)
					string[max_diag_len] = 0;
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) -902,
					gds_arg_gds, gds_exec_sql_invalid_req, 
					gds_arg_string, string, gds_arg_end);
			}

			execute_request(request, trans_handle, in_blr_length, in_blr,
							in_msg_length, in_msg, out_blr_length, out_blr,
							out_msg_length,	out_msg, false);

			release_request(request, true);
		}	// try
		catch (const std::exception&) {
			status = error();
			release_request(request, true);
			RESTORE_THREAD_DATA;
			return status;
		}
	}
	catch(const std::exception&)
	{
		RESTORE_THREAD_DATA;
		return tdsql->tsql_status [1];
	}

	return return_success();
}

ISC_STATUS DLL_EXPORT dsql8_execute_immediate(ISC_STATUS*	user_status,
					 WHY_DBB*	db_handle,
					 WHY_TRA*	trans_handle,
					 USHORT		length,
					 TEXT*		string,
					 USHORT		dialect,
					 USHORT		in_blr_length,
					 SCHAR*		in_blr,
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
		reinterpret_cast<UCHAR*>(in_blr), 
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
  
    @brief	When executing dynamic sql_operator in context of existing connrction and
 	existing transaction, it is not clear - what should do "Create Database"
 	operator with existing db_handle and tra_handle. Therefore we don't exec it.
 

    @param sql
    @param l
    @param crdb

 **/
static int check_for_create_database( TEXT* sql,
									  int l,
									  TEXT* crdb)
{
	for (; l--; sql++) {
    	switch (*sql) {
        	case '\t':
        	case '\n':
        	case '\r':
        	case ' ':
            	continue;
        }
	    if (tolower(*sql) != *crdb++)
    	    break;
	    if (! *crdb)
    	    return 1;
    }

	return 0;
}

/**
  
 	callback_execute_immediate
  
    @brief	Execute sql_operator in context of jrd_transaction_handle
 

    @param status
    @param jrd_attachment_handle
    @param jrd_transaction_handle
    @param sql_operator
    @param len

 **/
ISC_STATUS callback_execute_immediate( ISC_STATUS* status,
								   class att* jrd_attachment_handle,
								   class jrd_tra* jrd_transaction_handle,
    							   TEXT* sql_operator,
    							   int len)
{

	DBB	database;

	/* Other requests appear to be incorrect in this context */
	long requests = (1 << REQ_INSERT) | (1 << REQ_DELETE) | (1 << REQ_UPDATE)
			      | (1 << REQ_DDL) | (1 << REQ_SET_GENERATOR) | (1 << REQ_EXEC_PROCEDURE);

	if (check_for_create_database(sql_operator, len, "createdatabase") ||
    	check_for_create_database(sql_operator, len, "createschema"))
    	requests = 0;

	/* 1. Locate why_db_handle, corresponding to jrd_database_handle */
	THREAD_EXIT;
	THD_MUTEX_LOCK (&databases_mutex);
	for (database = databases; database; database = database->dbb_next)
	{
	    if (database->dbb_database_handle->handle.h_dbb == jrd_attachment_handle)
		{
			break;
		}
	}
	if (! database) {
    	status[0] = gds_arg_gds;
    	status[1] = gds_bad_db_handle;
    	status[2] = gds_arg_end;
    	THD_MUTEX_UNLOCK(&databases_mutex);
        THREAD_ENTER;
    	return status[1];
    }
	WHY_DBB	why_db_handle = (WHY_DBB) (database->dbb_database_handle);

	/* 2. Create why_trans_handle - it's new, but points to the same jrd
    	  transaction as original before callback. */
	why_hndl w;
	WHY_TRA	why_trans_handle = &w;
	memset (why_trans_handle, 0, sizeof(why_hndl));
	why_trans_handle->implementation = why_db_handle->implementation;
	why_trans_handle->handle.h_tra = jrd_transaction_handle;
	why_trans_handle->type = HANDLE_transaction;
	why_trans_handle->parent = why_db_handle;
	THD_MUTEX_UNLOCK (&databases_mutex);
    THREAD_ENTER;

	/* 3. Call execute... function */
	return dsql8_execute_immediate_common(status,
										  &why_db_handle, &why_trans_handle,
										  len, sql_operator, database->dbb_db_SQL_dialect,
										  0, NULL, 0, 0, NULL, 0, NULL, 0, 0, NULL, requests);
}


WHY_DBB	GetWhyAttachment (ISC_STATUS* status,
						  class att* jrd_attachment_handle)
{
	DBB	database;

	THREAD_EXIT;
	THD_MUTEX_LOCK (&databases_mutex);
	for (database = databases; database; database = database->dbb_next)
	{
	    if (database->dbb_database_handle->handle.h_dbb == jrd_attachment_handle)
		{
			break;
		}
	}
	if (! database) {
    	status[0] = gds_arg_gds;
    	status[1] = gds_bad_db_handle;
    	status[2] = gds_arg_end;
	}
	THD_MUTEX_UNLOCK (&databases_mutex);
    THREAD_ENTER;
	return database ? database->dbb_database_handle : 0;
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
							UCHAR*	blr,
							USHORT	msg_type,
							USHORT	msg_length,
							UCHAR*	dsql_msg
#ifdef SCROLLABLE_CURSORS
							, USHORT direction, SLONG offset
#endif
							)
{

	DSQL_MSG message;
	PAR parameter;
	ISC_STATUS s;
	struct tsql thd_context, *tdsql;

	SET_THREAD_DATA;

    try
    {
		tdsql->tsql_status = user_status;
		tdsql->tsql_default = NULL;

		init(0);

		DSQL_REQ request = *req_handle;
		tdsql->tsql_default = request->req_pool;

/* if the cursor isn't open, we've got a problem */

		if (request->req_type == REQ_SELECT ||
			request->req_type == REQ_SELECT_UPD ||
			request->req_type == REQ_EMBED_SELECT ||
			request->req_type == REQ_GET_SEGMENT)
				if (!(request->req_flags & REQ_cursor_open))
					ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 504,
						  gds_arg_gds, gds_dsql_cursor_err, 0);

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
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 804,
					  gds_arg_gds, gds_dsql_sqlda_err, 0);
			}

			if (offset)
			{
				PAR offset_parameter;
				DSC desc;

				message = (DSQL_MSG) request->req_async;

				desc.dsc_dtype = dtype_short;
				desc.dsc_scale = 0;
				desc.dsc_length = sizeof(USHORT);
				desc.dsc_flags = 0;
				desc.dsc_address = (UCHAR *) & direction;

				offset_parameter = message->msg_parameters;
				parameter = offset_parameter->par_next;
				MOVD_move(&desc, &parameter->par_desc);

				desc.dsc_dtype = dtype_long;
				desc.dsc_scale = 0;
				desc.dsc_length = sizeof(SLONG);
				desc.dsc_flags = 0;
				desc.dsc_address = (UCHAR *) & offset;

				MOVD_move(&desc, &offset_parameter->par_desc);

				THREAD_EXIT;
				s = isc_receive2(tdsql->tsql_status, &request->req_handle,
								 message->msg_number, message->msg_length,
								 message->msg_buffer, 0, direction, offset);
				THREAD_ENTER;

				if (s) {
					punt();
				}
			}
		}
#endif

		message = (DSQL_MSG) request->req_receive;

/* Insure that the blr for the message is parsed, regardless of
   whether anything is found by the call to receive. */

		if (blr_length) {
			parse_blr(blr_length, blr, msg_length, message->msg_parameters);
		}

		if (request->req_type == REQ_GET_SEGMENT)
		{
			// For get segment, use the user buffer and indicator directly.
	
			parameter = request->req_blob->blb_segment;
			PAR null = parameter->par_null;
			USHORT* ret_length =
				(USHORT *) (dsql_msg + (SLONG) null->par_user_desc.dsc_address);
			UCHAR* buffer = dsql_msg + (SLONG) parameter->par_user_desc.dsc_address;
			THREAD_EXIT;
			s = isc_get_segment(tdsql->tsql_status, &request->req_handle,
							ret_length, parameter->par_user_desc.dsc_length,
							reinterpret_cast<char*>(buffer));
			THREAD_ENTER;
			if (!s) {
				RESTORE_THREAD_DATA;
				return 0;
			}
			else if (s == gds_segment) {
				RESTORE_THREAD_DATA;
				return 101;
			}
			else if (s == gds_segstr_eof) {
				RESTORE_THREAD_DATA;
				return 100;
			}
			else
				punt();
		}

		THREAD_EXIT;
		s = isc_receive(tdsql->tsql_status, &request->req_handle,
						message->msg_number, message->msg_length, 
						message->msg_buffer, 0);
		THREAD_ENTER;

		if (s) {
			punt();
		}

		const PAR eof = request->req_eof;
		if (eof)
		{
			if (!*((USHORT *) eof->par_desc.dsc_address)) {
				RESTORE_THREAD_DATA;
				return 100;
			}
		}

		map_in_out(NULL, message, 0, blr, msg_length, dsql_msg);
	}  // try
	catch(const std::exception&)
	{
		RESTORE_THREAD_DATA;
		return tdsql->tsql_status [1];
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
	DSQL_REQ request;
	struct tsql thd_context, *tdsql;

	SET_THREAD_DATA;

    try
    {
		tdsql->tsql_status = user_status;
		tdsql->tsql_default = NULL;

		init(0);

		request = *req_handle;
		tdsql->tsql_default = request->req_pool;

		if (option & DSQL_drop) {
		/* Release everything associate with the request. */

			release_request(request, true);
			*req_handle = NULL;
		}
		else if (option & DSQL_close) {
		/* Just close the cursor associated with the request. */

			if (!(request->req_flags & REQ_cursor_open))
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 501,
					  gds_arg_gds, gds_dsql_cursor_close_err, 0);

			close_cursor(request);
		}
	}
	catch(const std::exception&)
	{
		RESTORE_THREAD_DATA;
		return tdsql->tsql_status [1];
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
							UCHAR*	blr,
							USHORT	msg_type,
							USHORT	msg_length,
							UCHAR*	dsql_msg)
{
	DSQL_REQ request;
	DSQL_MSG message;
	PAR parameter;
	SCHAR *buffer;
	ISC_STATUS s;
	struct tsql thd_context, *tdsql;

	SET_THREAD_DATA;

    try
    {
		tdsql->tsql_status = user_status;
		tdsql->tsql_default = NULL;

		init(0);

		request = *req_handle;
		tdsql->tsql_default = request->req_pool;

/* if the cursor isn't open, we've got a problem */

		if (request->req_type == REQ_PUT_SEGMENT)
			if (!(request->req_flags & REQ_cursor_open))
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 504,
					  gds_arg_gds, gds_dsql_cursor_err, 0);

		message = (DSQL_MSG) request->req_receive;

/* Insure that the blr for the message is parsed, regardless of
   whether anything is found by the call to receive. */

		if (blr_length)
			parse_blr(blr_length, blr, msg_length, message->msg_parameters);

		if (request->req_type == REQ_PUT_SEGMENT) {
		/* For put segment, use the user buffer and indicator directly. */

			parameter = request->req_blob->blb_segment;
			buffer =
				reinterpret_cast<SCHAR*>(
					dsql_msg + (SLONG) parameter->par_user_desc.dsc_address);
			THREAD_EXIT;
			s = isc_put_segment(tdsql->tsql_status, &request->req_handle,
							parameter->par_user_desc.dsc_length, buffer);
			THREAD_ENTER;
			if (s)
				punt();
		}
	}
	catch(const std::exception&)
	{
		RESTORE_THREAD_DATA;
		return tdsql->tsql_status [1];
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
							WHY_TRA*		trans_handle,
							dsql_req**		req_handle,
							USHORT			length,
							TEXT*			string,
							USHORT			dialect,
							USHORT			item_length,
							const UCHAR*	items,
							USHORT			buffer_length,
							UCHAR*			buffer)
{
	DSQL_REQ old_request, request;
	USHORT parser_version;
	DBB database;
	ISC_STATUS status;
	struct tsql thd_context, *tdsql;

	SET_THREAD_DATA;

	try
	{
		tdsql->tsql_status = user_status;
		tdsql->tsql_default = NULL;

		init(0);

    old_request = *req_handle;
    if (!old_request) {
        ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -901,
                   gds_arg_gds, gds_bad_req_handle,
                   0);
    }

    if (old_request) {
        database = old_request->req_dbb;
        if (!database) {
            ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -901,
                       gds_arg_gds, gds_bad_req_handle,
                       0);
        }
    }

/* check to see if old request has an open cursor */

		if (old_request && (old_request->req_flags & REQ_cursor_open)) {
			ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 519,
				  gds_arg_gds, gds_dsql_open_cursor_request, 0);
		}

/* Allocate a new request block and then prepare the request.  We want to
   keep the old request around, as is, until we know that we are able
   to prepare the new one. */
/* It would be really *nice* to know *why* we want to
   keep the old request around -- 1994-October-27 David Schnepper */
/* Because that's the client's allocated statement handle and we
   don't want to trash the context in it -- 2001-Oct-27 Ann Harrison */

		tdsql->tsql_default = DsqlMemoryPool::createPool();
		request = FB_NEW(*tdsql->tsql_default) dsql_req;
		request->req_dbb = database;
		request->req_pool = tdsql->tsql_default;
		request->req_trans = *trans_handle;

		try {

			if (!length)
				length = strlen(string);

/* Figure out which parser version to use */
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

			if ((dialect / 10) == 0)
				parser_version = 2;
			else {
				parser_version = dialect % 10;
				dialect /= 10;
			}

			request->req_client_dialect = dialect;

			request = prepare(request, length, string, dialect, parser_version);

/* Can not prepare a CREATE DATABASE/SCHEMA statement */

			if ((request->req_type == REQ_DDL) &&
				(request->req_ddl_node->nod_type == nod_def_database))
					ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 530,
					  	gds_arg_gds, gds_dsql_crdb_prepare_err, 0);

			request->req_flags |= REQ_prepared;

/* Now that we know that the new request exists, zap the old one. */

			tdsql->tsql_default = old_request->req_pool;
			release_request(old_request, true);
			tdsql->tsql_default = NULL;

/* The request was sucessfully prepared, and the old request was
 * successfully zapped, so set the client's handle to the new request */

			*req_handle = request;

			RESTORE_THREAD_DATA;

			return GDS_DSQL_SQL_INFO_CPP(user_status,
									req_handle,
									item_length,
									items,
									buffer_length,
									buffer);

		}	// try
		catch(const std::exception&) {
			status = error();
			release_request(request, true);
			RESTORE_THREAD_DATA;
			return status;
		}
	}
	catch(const std::exception&)
	{
		RESTORE_THREAD_DATA;
		return tdsql->tsql_status [1];
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
								TEXT*	input_cursor,
								USHORT	type)
{
	DSQL_REQ request;
	SYM symbol;
	USHORT length;
	struct tsql thd_context, *tdsql;
	TEXT cursor[132];


	SET_THREAD_DATA;

    try
    {
		tdsql->tsql_status = user_status;
		tdsql->tsql_default = NULL;

		init(0);

		request = *req_handle;
		tdsql->tsql_default = request->req_pool;
		if (input_cursor[0] == '\"')
		{
			// Quoted cursor names eh? Strip'em.
			// Note that "" will be replaced with ".
			//
			int index;
			for (index = 0; *input_cursor; input_cursor++) {
				if (*input_cursor == '\"') {
					input_cursor++;
				}
				cursor[index++] = *input_cursor;
			}
			cursor[index] = 0;
		}
		else	// not quoted name
		{
			USHORT i;
			for (i = 0; i < sizeof(cursor) - 1	/* PJPG 20001013 */
			 	&& input_cursor[i]	/* PJPG 20001013 */
			 	&& input_cursor[i] != ' '; ++i)	/* PJPG 20001013 */
			{
				cursor[i] = UPPER7(input_cursor[i]);
			}
			cursor[i] = '\0';
		}
		length = name_length(cursor);
	
		if (length == 0) {
			ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 502,
				  gds_arg_gds, gds_dsql_decl_err, 0);
		}

/* If there already is a different cursor by the same name, bitch */

		if (symbol = HSHD_lookup(request->req_dbb, cursor, length, SYM_cursor, 0)) {
			if (request->req_cursor == symbol) {
				return return_success();
			}

			ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 502,
				  gds_arg_gds, gds_dsql_decl_err, 0);
		}

/* If there already is a cursor and its name isn't the same, ditto.
   We already know there is no cursor by this name in the hash table */

		if (!request->req_cursor) {
			request->req_cursor = MAKE_symbol(request->req_dbb, cursor,
										  length, SYM_cursor, request);
		} else {
			assert(request->req_cursor != symbol);
			ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 502,
				  gds_arg_gds, gds_dsql_decl_err, 0);
		}
	}
	catch(const std::exception&)
	{
		RESTORE_THREAD_DATA;
		return tdsql->tsql_status [1];
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
	DSQL_REQ request;
	DSQL_MSG *message;
	UCHAR item, *end_info, buffer[256], *buffer_ptr;
	const UCHAR *end_items, *end_describe;
	USHORT length, number, first_index;
	struct tsql thd_context, *tdsql;

	SET_THREAD_DATA;

    try
    {
		tdsql->tsql_status = user_status;
		tdsql->tsql_default = NULL;

		init(0);
		memset(buffer, 0, sizeof(buffer));

		request = *req_handle;
	
		end_items = items + item_length;
		end_info = info + info_length;

		message = NULL;
		first_index = 0;

		while (items < end_items && *items != gds_info_end)
		{
			item = *items++;
			if (item == gds_info_sql_select || item == gds_info_sql_bind)
			{
				message = (item == gds_info_sql_select) ?
					&request->req_receive : &request->req_send;
				if (info + 1 >= end_info) {
					*info = gds_info_truncated;
					return return_success();
				}
				*info++ = item;
			}
			else if (item == gds_info_sql_stmt_type)
			{
				switch (request->req_type) {
				case REQ_SELECT:
				case REQ_EMBED_SELECT:
					number = gds_info_sql_stmt_select;
					break;
				case REQ_SELECT_UPD:
					number = gds_info_sql_stmt_select_for_upd;
					break;
				case REQ_DDL:
					number = gds_info_sql_stmt_ddl;
					break;
				case REQ_GET_SEGMENT:
					number = gds_info_sql_stmt_get_segment;
					break;
				case REQ_PUT_SEGMENT:
					number = gds_info_sql_stmt_put_segment;
					break;
				case REQ_COMMIT:
				case REQ_COMMIT_RETAIN:
					number = gds_info_sql_stmt_commit;
					break;
				case REQ_ROLLBACK:
					number = gds_info_sql_stmt_rollback;
					break;
				case REQ_START_TRANS:
					number = gds_info_sql_stmt_start_trans;
					break;
				case REQ_INSERT:
					number = gds_info_sql_stmt_insert;
					break;
				case REQ_UPDATE:
				case REQ_UPDATE_CURSOR:
					number = gds_info_sql_stmt_update;
					break;
				case REQ_DELETE:
				case REQ_DELETE_CURSOR:
					number = gds_info_sql_stmt_delete;
					break;
				case REQ_EXEC_PROCEDURE:
					number = gds_info_sql_stmt_exec_procedure;
					break;
				case REQ_SET_GENERATOR:
					number = isc_info_sql_stmt_set_generator;
					break;
				case REQ_SAVEPOINT:
					number = isc_info_sql_stmt_savepoint;
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
			else if (item == gds_info_sql_sqlda_start) {
				length = *items++;
				first_index =
					static_cast<USHORT>
					(gds__vax_integer
				 	(items, length));
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
			else if (item == gds_info_sql_records) {
				length = get_request_info(request, (SSHORT) sizeof(buffer),
										  reinterpret_cast<SCHAR*>(buffer));
				if (length && !(info = put_item(item, length, buffer, info, 
												end_info))) 
				{
					return return_success();
				}
			}
			else if (item == gds_info_sql_get_plan) {
			/* be careful, get_plan_info() will reallocate the buffer to a
			   larger size if it is not big enough */

				buffer_ptr = buffer;
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
				 	(item != gds_info_sql_num_variables
				  	&& item != gds_info_sql_describe_vars))
			{
				buffer[0] = item;
				item = gds_info_error;
				length = 1 + convert((SLONG) gds_infunk, buffer + 1);
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
				if (item == gds_info_sql_num_variables) {
					continue;
				}

				end_describe = items;
				while (end_describe < end_items &&
				   	*end_describe != gds_info_end &&
				   	*end_describe != gds_info_sql_describe_end) end_describe++;

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
				if (*items == gds_info_sql_describe_end) {
					items++;
				}
			}
		}

		*info++ = gds_info_end;
	}
	catch(const std::exception&)
	{
		RESTORE_THREAD_DATA;
		return tdsql->tsql_status [1];
	}

	return return_success();
}


#ifdef DSQL_DEBUG

static void trace_line(const char* message, ...) {
	char buffer[1024];
	char *ptr = buffer;
	va_list params;
	va_start(params, message);
#ifdef HAVE_VSNPRINTF
	vsnprintf(ptr, sizeof(buffer), message, params);
#else
	vsprintf(ptr, message, params);
#endif
	va_end(params);
	gds__trace_raw(buffer);
}

/**
  
 	DSQL_pretty
  
    @brief	Pretty print a node tree.


    @param node
    @param column

 **/
void DSQL_pretty(DSQL_NOD node, int column)
{
	DSQL_MAP map;
	DSQL_REL relation;
	DSQL_PRC procedure;
	DSQL_CTX context;
	DSQL_FLD field;
	STR string;
	VAR variable;

	TEXT buffer[1024];

	TEXT* verb;
	TEXT s[64];

	TEXT* p = buffer;
	p += sprintf(p, "%.7X ", (unsigned int) node);

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
		trace_line("%sSTRING: \"%s\"\n", buffer, ((STR) node)->str_data);
		return;

	case (TEXT) dsql_type_fld:
		trace_line("%sFIELD: %s\n", buffer, ((DSQL_FLD) node)->fld_name);
		return;

	case (TEXT) dsql_type_sym:
		trace_line("%sSYMBOL: %s\n", buffer, ((SYM) node)->sym_string);
		return;

	case (TEXT) dsql_type_nod:
		break;

	default:
		trace_line("%sUNKNOWN BLOCK TYPE\n", buffer);
		return;
	}

	DSQL_NOD* ptr = node->nod_arg;
	DSQL_NOD* end = ptr + node->nod_count;

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
   /* CVC: New node redef_relation. */
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
    /* CVC: New node del_view. */
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
    /* CVC: New node redef_procedure. */
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
	case nod_erase:
		verb = "erase";
		break;
	case nod_execute:
		verb = "execute";
		break;
	case nod_exec_procedure:
		verb = "execute procedure";
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
        /* CVC: New node current_role. */
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
	/* IOL:	missing	node types */
	case nod_on_error:
		verb="on error";  
		break;
	case nod_block:
		verb="block";  
		break;
	case nod_default:
		verb="default";	 
		break;
	case nod_plan_expr:
		verb="plan";  
		break;
	case nod_index:
		verb="index";  
		break;
	case nod_index_order:
		verb="order";  
		break;
	case nod_plan_item:
		verb="item";  
		break;
	case nod_natural:
		verb="natural";	 
		break;
	case nod_join_inner:
		verb="join_inner";	
		break;
	/* SKIDDER: some more missing node types */
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
	case nod_def_trigger_msg:
		verb = "def_trigger_msg";
		break;
	case nod_mod_trigger_msg:
		verb = "mod_trigger_msg";
		break;
	case nod_del_trigger_msg:
		verb = "del_trigger_msg";
		break;
	case nod_def_exception:
		verb = "def_exception";
		break;
	case nod_mod_exception:
		verb = "mod_exception";
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
		DSQL_pretty(node->nod_arg[e_label_name], column+1);
		trace_line("%s   number %d\n", buffer,
			(int)(IPTR)node->nod_arg[e_label_number]);
		return;

	case nod_derived_field:
		verb = "derived_field";
		trace_line("%s%s\n", buffer, verb);
		DSQL_pretty(node->nod_arg[e_derived_field_value], column+1);
		DSQL_pretty(node->nod_arg[e_derived_field_name], column+1);
		trace_line("%s   scope %d\n", buffer,
			(USHORT)(U_IPTR)node->nod_arg[e_derived_field_scope]);
		return;

	case nod_aggregate:
		verb = "aggregate";
		trace_line("%s%s\n", buffer, verb);
		context = (DSQL_CTX) node->nod_arg[e_agg_context];
		trace_line("%s   context %d\n", buffer, context->ctx_context);
		if ((map = context->ctx_map) != NULL)
			trace_line("%s   map\n", buffer);
		while (map) {
			trace_line("%s      position %d\n", buffer, map->map_position);
			DSQL_pretty(map->map_node, column + 2);
			map = map->map_next;
		}
		DSQL_pretty(node->nod_arg[e_agg_group], column + 1);
		DSQL_pretty(node->nod_arg[e_agg_rse], column + 1);
		return;

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
		context = (DSQL_CTX) node->nod_arg[e_fld_context];
		relation = context->ctx_relation;
 		procedure = context->ctx_procedure;
		field = (DSQL_FLD) node->nod_arg[e_fld_field];
		trace_line("%sfield %s.%s, context %d\n", buffer,
 			(relation != NULL ? 
 				relation->rel_name : 
 				(procedure != NULL ? 
 					procedure->prc_name : 
 					"unknown_db_object")), 
 			field->fld_name, context->ctx_context);
		return;
	
	case nod_field_name:
		trace_line("%sfield name: \"", buffer);
		string = (STR) node->nod_arg[e_fln_context];
		if (string)
			trace_line("%s.", string->str_data);
		string = (STR) node->nod_arg[e_fln_name];
        if (string != 0) {
            trace_line("%s\"\n", string->str_data);
        }
        else {
            trace_line("%s\"\n", "*");
        }
		return;

	case nod_map:
		verb = "map";
		trace_line("%s%s\n", buffer, verb);
		context = (DSQL_CTX) node->nod_arg[e_map_context];
		trace_line("%s   context %d\n", buffer, context->ctx_context);
		for (map = (DSQL_MAP) node->nod_arg[e_map_map]; map; map = map->map_next) {
			trace_line("%s   position %d\n", buffer, map->map_position);
			DSQL_pretty(map->map_node, column + 1);
		}
		return;

	case nod_relation:
		context = (DSQL_CTX) node->nod_arg[e_rel_context];
		relation = context->ctx_relation;
		procedure = context->ctx_procedure;
 		if( relation != NULL ){ 
 			trace_line("%srelation %s, context %d\n",
 				buffer, relation->rel_name, context->ctx_context);
 		} else if ( procedure != NULL ) { 
 			trace_line("%sprocedure %s, context %d\n",
 				buffer, procedure->prc_name, context->ctx_context);
 		} else {
 			trace_line("%sUNKNOWN DB OBJECT, context %d\n",
 			buffer, context->ctx_context);
 		}
		return;

	case nod_variable:
		variable = (VAR) node->nod_arg[e_var_variable];
        // Adding variable->var_variable_number to display, obviously something
        // is missing from the printf, and Im assuming this was it.
        // (anyway can't be worse than it was MOD 05-July-2002.
		trace_line("%svariable %s %d\n", buffer, variable->var_name, variable->var_variable_number);
		return;

	case nod_var_name:
		trace_line("%svariable name: \"", buffer);
		string = (STR) node->nod_arg[e_vrn_name];
		trace_line("%s\"\n", string->str_data);
		return;

	case nod_parameter:
		if (node->nod_column){
			trace_line("%sparameter: %d\n",	buffer,(USHORT)(ULONG)node->nod_arg[e_par_parameter]);
		}else{
			PAR	par=(PAR)node->nod_arg[e_par_parameter];
			trace_line("%sparameter: %d\n",	buffer,par->par_index);
		}
		return;


    case nod_udf:
        trace_line ("%sfunction: \"", buffer);  
    /* nmcc: how are we supposed to tell which type of nod_udf this is ?? */
    /* CVC: The answer is that nod_arg[0] can be either the udf name or the
    pointer to udf struct returned by METD_get_function, so we should resort
    to the block type. The replacement happens in pass1_udf(). */
        //        switch (node->nod_arg [e_udf_name]->nod_header.blk_type) {
        switch (MemoryPool::blk_type(node->nod_arg [e_udf_name])) {
        case dsql_type_udf:
            trace_line ("%s\"\n", ((UDF) node->nod_arg [e_udf_name])->udf_name);
            break;
        case dsql_type_str:  
            string = (STR) node->nod_arg [e_udf_name];
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
	} else {
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
 
  N.B., the cleanup handlers (registered with gds__database_cleanup)
  are called outside of the ISC thread mechanism...
 
  These do not make use of the context at this time.
 

    @param db_handle
    @param flag

 **/
static void cleanup_database(FRBRD ** db_handle, SLONG flag)
{
	DBB *dbb_ptr, dbb;

	if (flag)
		Firebird::fatal_exception::raise("Illegal call to cleanup_database");

	if (!db_handle || !databases)
		return;

/*	if (flag)
		THREAD_EXIT;*/

	THD_MUTEX_LOCK(&databases_mutex);

	for (dbb_ptr = &databases; dbb = *dbb_ptr; dbb_ptr = &dbb->dbb_next)
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
			THREAD_ENTER;
		}
		HSHD_finish(dbb);
		delete dbb->dbb_pool;
	}
	else if (flag)
		THREAD_ENTER;
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
static void cleanup_transaction (FRBRD * tra_handle, SLONG arg)
{
	ISC_STATUS_ARRAY local_status;
	OPN *open_cursor_ptr, open_cursor;

/* find this transaction/request pair in the list of pairs */

	THD_MUTEX_LOCK(&cursors_mutex);
	open_cursor_ptr = &open_cursors;
	while (open_cursor = *open_cursor_ptr)
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
			THREAD_ENTER; //ttt
			GDS_DSQL_FREE_CPP(	local_status,
								&open_cursor->opn_request,
								DSQL_close);
			THREAD_EXIT;
			THD_MUTEX_LOCK(&cursors_mutex);
			open_cursor_ptr = &open_cursors;
		}
		else
			open_cursor_ptr = &open_cursor->opn_next;

	THD_MUTEX_UNLOCK(&cursors_mutex);
}


/**
  
 	close_cursor
  
    @brief	Close an open cursor.
 

    @param request

 **/
static void close_cursor( DSQL_REQ request)
{
	OPN *open_cursor_ptr, open_cursor;
	ISC_STATUS_ARRAY status_vector;

	if (request->req_handle) {
		THREAD_EXIT; //ttt
		if (request->req_type == REQ_GET_SEGMENT ||
			request->req_type == REQ_PUT_SEGMENT)
				isc_close_blob(status_vector, &request->req_handle);
		else
			isc_unwind_request(status_vector, &request->req_handle, 0);
		THREAD_ENTER;
	}

	request->req_flags &= ~(REQ_cursor_open | REQ_embedded_sql_cursor);

/* Remove the open cursor from the list */

	THD_MUTEX_LOCK(&cursors_mutex);
	open_cursor_ptr = &open_cursors;
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
static USHORT convert( SLONG number, UCHAR * buffer)
{
	SLONG n;
	UCHAR *p;

#ifndef WORDS_BIGENDIAN
	n = number;
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

	TSQL tdsql = GET_THREAD_DATA;

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
static void execute_blob(	DSQL_REQ		request,
							USHORT	in_blr_length,
							UCHAR*	in_blr,
							USHORT	in_msg_length,
							UCHAR*	in_msg,
							USHORT	out_blr_length,
							UCHAR*	out_blr,
							USHORT	out_msg_length,
							UCHAR*	out_msg)
{
	BLB blob;
	SSHORT filter;
	USHORT bpb_length;
	GDS__QUAD *blob_id;
	UCHAR bpb[24], *p;
	PAR parameter, null;
	ISC_STATUS s;
	TSQL tdsql;

	tdsql = GET_THREAD_DATA;

	blob = request->req_blob;
	map_in_out(request, blob->blb_open_in_msg, in_blr_length, in_blr,
			   in_msg_length, in_msg);

	p = bpb;
	*p++ = gds_bpb_version1;
	if (filter = filter_sub_type(request, blob->blb_to)) {
		*p++ = gds_bpb_target_type;
		*p++ = 2;
		*p++ = static_cast<UCHAR>(filter);
		*p++ = filter >> 8;
	}
	if (filter = filter_sub_type(request, blob->blb_from)) {
		*p++ = gds_bpb_source_type;
		*p++ = 2;
		*p++ = static_cast<UCHAR>(filter);
		*p++ = filter >> 8;
	}
	bpb_length = p - bpb;
	if (bpb_length == 1) {
		bpb_length = 0;
	}

	parameter = blob->blb_blob_id;
	null = parameter->par_null;

	if (request->req_type == REQ_GET_SEGMENT)
	{
		blob_id = (GDS__QUAD *) parameter->par_desc.dsc_address;
		if (null && *((SSHORT *) null->par_desc.dsc_address) < 0) {
			memset(blob_id, 0, sizeof(GDS__QUAD));
		}
		THREAD_EXIT;
		s = isc_open_blob2(tdsql->tsql_status,
						   &request->req_dbb->dbb_database_handle,
						   &request->req_trans, &request->req_handle,
						   blob_id, bpb_length,
						   reinterpret_cast<UCHAR*>(bpb));
		THREAD_ENTER;
		if (s) {
			punt();
		}
	}
	else
	{
		request->req_handle = NULL;
		blob_id = (GDS__QUAD *) parameter->par_desc.dsc_address;
		memset(blob_id, 0, sizeof(GDS__QUAD));
		THREAD_EXIT;
		s = isc_create_blob2(tdsql->tsql_status,
							 &request->req_dbb->dbb_database_handle,
							 &request->req_trans, &request->req_handle,
							 blob_id, bpb_length, reinterpret_cast<char*>(bpb));
		THREAD_ENTER;
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
static ISC_STATUS execute_request(DSQL_REQ			request,
								  WHY_TRA*			trans_handle,
								  USHORT			in_blr_length,
								  UCHAR*			in_blr,
								  USHORT			in_msg_length,
								  UCHAR*			in_msg,
								  USHORT			out_blr_length,
								  UCHAR*			out_blr,
								  USHORT			out_msg_length,
								  UCHAR*			out_msg,
								  bool				singleton)
{
	DSQL_MSG message;
	USHORT use_msg_length;
	UCHAR *use_msg;
	UCHAR buffer[20];
	ISC_STATUS s;
	ISC_STATUS_ARRAY local_status;
	TSQL tdsql;
	ISC_STATUS return_status;

	tdsql = GET_THREAD_DATA;

	request->req_trans = *trans_handle;
	return_status = FB_SUCCESS;

	switch (request->req_type) {
	case REQ_START_TRANS:
		THREAD_EXIT;
		s = isc_start_transaction(	tdsql->tsql_status,
									&request->req_trans,
									1,
									&request->req_dbb->dbb_database_handle,
									(int)(reinterpret_cast<char*>(request->req_blr) - request->req_blr_string->str_data),
									request->req_blr_string->str_data);
		THREAD_ENTER;
		if (s)
			punt();
		*trans_handle = request->req_trans;
		return FB_SUCCESS;

	case REQ_COMMIT:
		THREAD_EXIT;
		s = isc_commit_transaction(tdsql->tsql_status,
								   &request->req_trans);
		THREAD_ENTER;
		if (s)
			punt();
		*trans_handle = NULL;
		return FB_SUCCESS;

	case REQ_COMMIT_RETAIN:
		THREAD_EXIT;
		s = isc_commit_retaining(tdsql->tsql_status,
								 &request->req_trans);
		THREAD_ENTER;
		if (s)
			punt();
		return FB_SUCCESS;

	case REQ_ROLLBACK:
		THREAD_EXIT;
		s = isc_rollback_transaction(tdsql->tsql_status,
									 &request->req_trans);
		THREAD_ENTER;
		if (s)
			punt();
		*trans_handle = NULL;
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

	case REQ_EXEC_PROCEDURE:
		if (message = (DSQL_MSG) request->req_send) {
			map_in_out(request, message, in_blr_length, in_blr,
					   in_msg_length, in_msg);
			in_msg_length = message->msg_length;
			in_msg = message->msg_buffer;
		}
		else {
			in_msg_length = 0;
			in_msg = NULL;
		}
		if (out_msg_length && (message = (DSQL_MSG) request->req_receive)) {
			if (out_blr_length) {
				parse_blr(out_blr_length, out_blr, out_msg_length,
						  message->msg_parameters);
			}
			use_msg_length = message->msg_length;
			use_msg = message->msg_buffer;
		}
		else {
			use_msg_length = 0;
			use_msg = NULL;
		}
		THREAD_EXIT;
		s = isc_transact_request(tdsql->tsql_status,
								 &request->req_dbb->dbb_database_handle,
								 &request->req_trans,
								 (USHORT) (reinterpret_cast<char*>(request->req_blr) -
										   request->req_blr_string->str_data),
								 request->req_blr_string->str_data,
								 in_msg_length,
								 reinterpret_cast<char*>(in_msg),
								 use_msg_length,
								 reinterpret_cast<char*>(use_msg));
		THREAD_ENTER;

		if (s)
			punt();
		if (out_msg_length && message)
			map_in_out(NULL, message, 0, out_blr, out_msg_length, out_msg);
		return FB_SUCCESS;

	default:
		/* Catch invalid request types */
		assert(false);
		/* Fall into ... */

	case REQ_SELECT:
	case REQ_SELECT_UPD:
	case REQ_INSERT:
	case REQ_DELETE:
	case REQ_UPDATE:
	case REQ_UPDATE_CURSOR:
	case REQ_DELETE_CURSOR:
	case REQ_EMBED_SELECT:
	case REQ_SET_GENERATOR:
	case REQ_SAVEPOINT:
		break;
	}

/* If there is no data required, just start the request */

	message = (DSQL_MSG) request->req_send;
	if (!message)
	{
		THREAD_EXIT;
		s = isc_start_request(tdsql->tsql_status,
							  &request->req_handle,
							  &request->req_trans,
							  0);
		THREAD_ENTER;
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

		THREAD_EXIT;
		s = isc_start_and_send(tdsql->tsql_status,
							   &request->req_handle,
							   &request->req_trans,
							   message->msg_number,
							   message->msg_length,
							   message->msg_buffer,
							   0);
		THREAD_ENTER;
		if (s)
		{
			punt();
		}
	}

	if (out_msg_length && (message = (DSQL_MSG) request->req_receive)) {
		/* Insure that the blr for the message is parsed, regardless of
		   whether anything is found by the call to receive. */

		if (out_blr_length) {
			parse_blr(out_blr_length, out_blr, out_msg_length,
					  message->msg_parameters);
		}

		THREAD_EXIT;
		s = isc_receive(tdsql->tsql_status,
						&request->req_handle,
						message->msg_number,
						message->msg_length,
						message->msg_buffer,
						0);
		THREAD_ENTER;
		if (s) {
			punt();
		}

		map_in_out(NULL, message, 0, out_blr, out_msg_length, out_msg);

		/* if this is a singleton select, make sure there's in fact one record */

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
			THREAD_EXIT;
			for (counter = 0; counter < 2 && !s; counter++)
			{
				s = isc_receive(local_status,
								&request->req_handle,
								message->msg_number,
								message->msg_length,
								message_buffer,
								0);
			}
			THREAD_ENTER;
			gds__free(message_buffer);

			/* two successful receives means more than one record
			   a req_sync error on the first pass above means no records
			   a non-req_sync error on any of the passes above is an error */

			if (!s)
			{
				tdsql->tsql_status[0] = gds_arg_gds;
				tdsql->tsql_status[1] = gds_sing_select_err;
				tdsql->tsql_status[2] = gds_arg_end;
				return_status = gds_sing_select_err;
			}
			else if (s == gds_req_sync && counter == 1)
			{
				tdsql->tsql_status[0] = gds_arg_gds;
				tdsql->tsql_status[1] = gds_stream_eof;
				tdsql->tsql_status[2] = gds_arg_end;
				return_status = gds_stream_eof;
			}
			else if (s != gds_req_sync)
			{
				punt();
			}
		}
	}

	if (!(request->req_dbb->dbb_flags & DBB_v3))
	{
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
static SSHORT filter_sub_type( DSQL_REQ request, DSQL_NOD node)
{
	PAR parameter, null;

	if (node->nod_type == nod_constant)
		return (SSHORT)(SLONG) node->nod_arg[0];

	parameter = (PAR) node->nod_arg[e_par_parameter];
	if (null = parameter->par_null)
		if (*((SSHORT *) null->par_desc.dsc_address))
			return 0;

	return *((SSHORT *) parameter->par_desc.dsc_address);
}


/**
  
 	get_indices
  
    @brief	Retrieve the indices from the index tree in
 	the request info buffer, and print them out
 	in the plan buffer. Return true on success and false on failure
 

    @param explain_length_ptr
    @param explain_ptr
    @param plan_length_ptr
    @param plan_ptr

 **/
static bool get_indices(
						   SSHORT * explain_length_ptr,
						   SCHAR ** explain_ptr,
						   SSHORT * plan_length_ptr, SCHAR ** plan_ptr)
{
	SCHAR *explain, *plan;
	SSHORT explain_length, plan_length;
	USHORT length;

	explain_length = *explain_length_ptr;
	explain = *explain_ptr;
	plan_length = *plan_length_ptr;
	plan = *plan_ptr;

/* go through the index tree information, just
   extracting the indices used */

	explain_length--;
	switch (*explain++) {
	case gds_info_rsb_and:
	case gds_info_rsb_or:
		if (!get_indices(&explain_length, &explain, &plan_length, &plan))
			return false;
		if (!get_indices(&explain_length, &explain, &plan_length, &plan))
			return false;
		break;

	case gds_info_rsb_dbkey:
		break;

	case gds_info_rsb_index:
		explain_length--;
		length = *explain++;

		/* if this isn't the first index, put out a comma */

		if (plan[-1] != '(' && plan[-1] != ' ') {
			plan_length -= 2;
			if (plan_length < 0)
				return false;
			*plan++ = ',';
			*plan++ = ' ';
		}

		/* now put out the index name */

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
							DSQL_REQ request,
							SSHORT buffer_length, SCHAR ** buffer)
{
	SCHAR explain_buffer[256], *explain, *plan, *explain_ptr, *buffer_ptr;
	SSHORT explain_length, i;
    //	USHORT join_count = 0, level = 0;
	TSQL tdsql;
	ISC_STATUS s;

	tdsql = GET_THREAD_DATA;
	memset(explain_buffer, 0, sizeof(explain_buffer));
	explain_ptr = explain_buffer;
	buffer_ptr = *buffer;

/* get the access path info for the underlying request from the engine */

	THREAD_EXIT;
	s = isc_request_info(tdsql->tsql_status,
						 &request->req_handle,
						 0,
						 sizeof(explain_info),
						 const_cast < char *>(explain_info),
						 sizeof(explain_buffer), explain_buffer);
	THREAD_ENTER;

	if (s)
		return 0;
	explain = explain_buffer;

	if (*explain == gds_info_truncated) {
		explain_ptr = (SCHAR *) gds__alloc(BUFFER_XLARGE);

		THREAD_EXIT;
		s = isc_request_info(tdsql->tsql_status,
							 &request->req_handle, 0,
							 sizeof(explain_info),
							 const_cast < char *>(explain_info),
							 BUFFER_XLARGE, explain_ptr);
		THREAD_ENTER;

		if (s)
			return 0;
	}

	for (i = 0; i < 2; i++) {
		explain = explain_ptr;
		if (*explain++ != gds_info_access_path)
			return 0;

		explain_length = (UCHAR) * explain++;
		explain_length += (UCHAR) (*explain++) << 8;

		plan = buffer_ptr;

        /* CVC: What if we need to do 2nd pass? Those variables were only initialized
           at the begining of the function hence they had trash the second time. */
        USHORT join_count = 0, level = 0;

		/* keep going until we reach the end of the explain info */

		while (explain_length > 0 && buffer_length > 0)
			if (!get_rsb_item(&explain_length, &explain, &buffer_length, &plan,
							  &join_count, &level)) {
				/* assume we have run out of room in the buffer, try again with a larger one */

				buffer_ptr =
					reinterpret_cast<char*>(gds__alloc(BUFFER_XLARGE));
				buffer_length = BUFFER_XLARGE;
				break;
			}

		if (buffer_ptr == *buffer)
			break;
	}


	if (explain_ptr != explain_buffer)
		gds__free(explain_ptr);

	*buffer = buffer_ptr;
	return plan - *buffer;
}


/**
  
 	get_request_info
  
    @brief	Get the records updated/deleted for record
 

    @param request
    @param buffer_length
    @param buffer

 **/
static USHORT get_request_info(
							   DSQL_REQ request,
							   SSHORT buffer_length, SCHAR * buffer)
{
	SCHAR *data;
	UCHAR p;
	TSQL tdsql;
	ISC_STATUS s;
	USHORT data_length;

	tdsql = GET_THREAD_DATA;

/* get the info for the request from the engine */

	THREAD_EXIT;
	s = isc_request_info(tdsql->tsql_status,
						 &request->req_handle,
						 0,
						 sizeof(record_info),
						 const_cast < char *>(record_info),
						 buffer_length, buffer);
	THREAD_ENTER;

	if (s)
		return 0;

	data = buffer;

	request->req_updates = request->req_deletes = 0;
	request->req_selects = request->req_inserts = 0;

	while ((p = *data++) != gds_info_end) {
		data_length =
			static_cast < USHORT >
			(gds__vax_integer(reinterpret_cast<UCHAR*>(data), 2));
		data += 2;

		switch (p) {
		case gds_info_req_update_count:
			request->req_updates =
				gds__vax_integer(reinterpret_cast<UCHAR*>(data),
								 data_length);
			break;

		case gds_info_req_delete_count:
			request->req_deletes =
				gds__vax_integer(reinterpret_cast<UCHAR*>(data),
								 data_length);
			break;

		case gds_info_req_select_count:
			request->req_selects =
				gds__vax_integer(reinterpret_cast<UCHAR*>(data),
								 data_length);
			break;

		case gds_info_req_insert_count:
			request->req_inserts =
				gds__vax_integer(reinterpret_cast<UCHAR*>(data),
								 data_length);
			break;

		default:
			break;
		};

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
							SCHAR **	explain_ptr,
							SSHORT*		plan_length_ptr,
							SCHAR**		plan_ptr,
							USHORT*		parent_join_count,
							USHORT*		level_ptr)
{
	SCHAR *explain, *plan;
	const SCHAR *p;
	SSHORT explain_length, plan_length, rsb_type, length;
	USHORT join_count, union_count, union_level, union_join_count, save_level;

	explain_length = *explain_length_ptr;
	explain = *explain_ptr;
	plan_length = *plan_length_ptr;
	plan = *plan_ptr;

	explain_length--;
	switch (*explain++) {
	case gds_info_rsb_begin:
		if (!*level_ptr) {
			/* put out the PLAN prefix */

			p = "\nPLAN ";
			if ((plan_length -= strlen(p)) < 0)
				return false;
			while (*p)
				*plan++ = *p++;
		}

		(*level_ptr)++;
		break;

	case gds_info_rsb_end:
        if (*level_ptr) {
            (*level_ptr)--;
        }
        /* else --*parent_join_count; ??? */
		break;

	case gds_info_rsb_relation:

		/* for the single relation case, initiate
		   the relation with a parenthesis */

		if (!*parent_join_count) {
			if (--plan_length < 0)
				return false;
			*plan++ = '(';
		}

		/* if this isn't the first relation, put out a comma */

		if (plan[-1] != '(') {
			plan_length -= 2;
			if (plan_length < 0)
				return false;
			*plan++ = ',';
			*plan++ = ' ';
		}

		/* put out the relation name */

		explain_length--;
		explain_length -= (length = (UCHAR) * explain++);
		if ((plan_length -= length) < 0)
			return false;
		while (length--)
			*plan++ = *explain++;
		break;

	case gds_info_rsb_type:
		explain_length--;
		switch (rsb_type = *explain++) {
			/* for stream types which have multiple substreams, print out
			   the stream type and recursively print out the substreams so
			   we will know where to put the parentheses */

		case gds_info_rsb_union:

			/* put out all the substreams of the join */

			explain_length--;
			union_count = (USHORT) * explain++ - 1;

			/* finish the first union member */

			union_level = *level_ptr;
			union_join_count = 0;
			while (true) {
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
				while (true) {
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
			break;

		case gds_info_rsb_cross:
		case gds_info_rsb_left_cross:
		case gds_info_rsb_merge:

			/* if this join is itself part of a join list,
			   but not the first item, then put out a comma */

			if (*parent_join_count && plan[-1] != '(') {
				plan_length -= 2;
				if (plan_length < 0)
					return false;
				*plan++ = ',';
				*plan++ = ' ';
			}

			/* put out the join type */

			if (rsb_type == gds_info_rsb_cross ||
				rsb_type == gds_info_rsb_left_cross) {
                p = "JOIN (";
            }
			else {
				p = "MERGE (";
            }

			if ((plan_length -= strlen(p)) < 0)
				return false;
			while (*p)
				*plan++ = *p++;

			/* put out all the substreams of the join */

			explain_length--;
			join_count = (USHORT) * explain++;
			while (join_count) {
				if (!get_rsb_item(&explain_length, &explain, &plan_length,
								  &plan, &join_count, level_ptr))
				{
					return false;
				}
				/* CVC: Here's the additional stop condition. */
				if (!*level_ptr) {
					break;
				}
			}


			/* put out the final parenthesis for the join */

			if (--plan_length < 0)
				return false;
			else
				*plan++ = ')';

			/* this qualifies as a stream, so decrement the join count */

			if (*parent_join_count)
				-- * parent_join_count;
			break;

		case gds_info_rsb_indexed:
		case gds_info_rsb_navigate:
		case gds_info_rsb_sequential:
		case gds_info_rsb_ext_sequential:
		case gds_info_rsb_ext_indexed:
			if (rsb_type == gds_info_rsb_indexed ||
				rsb_type == gds_info_rsb_ext_indexed) 
			{
				p = " INDEX (";
			}
			else if (rsb_type == gds_info_rsb_navigate)
				p = " ORDER ";
			else
				p = " NATURAL";

			if ((plan_length -= strlen(p)) < 0)
				return false;
			while (*p)
				*plan++ = *p++;

			/* print out additional index information */

			if (rsb_type == gds_info_rsb_indexed ||
				rsb_type == gds_info_rsb_navigate ||
				rsb_type == gds_info_rsb_ext_indexed) {
				if (!get_indices(&explain_length, &explain, &plan_length, &plan))
					return false;
			}

			if (rsb_type == gds_info_rsb_navigate &&
				*explain == gds_info_rsb_indexed)
			{
				if (!get_rsb_item(&explain_length, &explain, &plan_length,
								  &plan, &join_count, level_ptr))
				{
					return false;
				}
			}

			if (rsb_type == gds_info_rsb_indexed ||
				rsb_type == gds_info_rsb_ext_indexed) {
				if (--plan_length < 0)
					return false;
				*plan++ = ')';
			}

			/* detect the end of a single relation and put out a final parenthesis */

			if (!*parent_join_count)
				if (--plan_length < 0)
					return false;
				else
					*plan++ = ')';

			/* this also qualifies as a stream, so decrement the join count */

			if (*parent_join_count)
				-- * parent_join_count;
			break;

		case gds_info_rsb_sort:

			/* if this sort is on behalf of a union, don't bother to
			   print out the sort, because unions handle the sort on all
			   substreams at once, and a plan maps to each substream
			   in the union, so the sort doesn't really apply to a particular plan */

			if (explain_length > 2 &&
				(explain[0] == gds_info_rsb_begin) &&
				(explain[1] == gds_info_rsb_type) &&
				(explain[2] == gds_info_rsb_union))
			{
				break;
			}

			/* if this isn't the first item in the list, put out a comma */

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

			save_level = *level_ptr;
			while (explain_length > 0 && plan_length > 0) {
				if (!get_rsb_item(&explain_length, &explain, &plan_length,
								  &plan, parent_join_count, level_ptr))
					return false;
				if (*level_ptr == save_level)
					break;
			}

			if (--plan_length < 0)
				return false;
			*plan++ = ')';
			break;

		default:
			break;
		}
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
static DBB init(FRBRD** db_handle)
{

#ifdef ANY_THREADING
	if (!mutex_inited) {
		mutex_inited = true;
		THD_MUTEX_INIT(&databases_mutex);
		THD_MUTEX_INIT(&cursors_mutex);
	}
#endif

	THREAD_EXIT;
	THD_MUTEX_LOCK(&databases_mutex);
	THREAD_ENTER;

	if (!init_flag)
	{
		init_flag = true;
		ALLD_init();
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

	DBB database;
	for (database = databases; database; database = database->dbb_next)
	{
		if (database->dbb_database_handle == *db_handle) {
			THD_MUTEX_UNLOCK(&databases_mutex);
			return database;
		}
	}

	DsqlMemoryPool* pool = DsqlMemoryPool::createPool();
	database = FB_NEW(*pool) dbb;
	database->dbb_pool = pool;
	database->dbb_next = databases;
	databases = database;
	database->dbb_database_handle = *db_handle;
	THD_MUTEX_UNLOCK(&databases_mutex);

	ISC_STATUS_ARRAY user_status;

	THREAD_EXIT;
	gds__database_cleanup(user_status, db_handle, cleanup_database, (SLONG) 0);
	THREAD_ENTER;

/* Determine if the database is V3 or V4 */

	SCHAR buffer[128];

	database->dbb_flags |= DBB_v3;
	THREAD_EXIT;
	const ISC_STATUS s =
		isc_database_info(user_status, db_handle,
						  sizeof(db_hdr_info_items),
						  const_cast<char*>(db_hdr_info_items),
						  sizeof(buffer), buffer);
	THREAD_ENTER;

	if (s) {
		return database;
	}

/* assume that server can not report current character set,
   and if not then emulate pre-patch actions. */
	database->dbb_att_charset = 127;

	SCHAR* data = buffer;
	UCHAR p;
	while ((p = *data++) != gds_info_end)
	{
		SSHORT l =
			static_cast<SSHORT>(
				gds__vax_integer(reinterpret_cast<UCHAR*>(data), 2));
		data += 2;

		switch (p)
		{
		case isc_info_db_sql_dialect:
			database->dbb_db_SQL_dialect = (USHORT) data[0];
			break;

		case gds_info_ods_version:
			if (gds__vax_integer(reinterpret_cast<UCHAR*>(data), l) > 7)
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

		case gds_info_base_level:
			database->dbb_base_level = (USHORT) data[1];
			break;

		case isc_info_db_read_only:
			if ((USHORT) data[0])
				database->dbb_flags |= DBB_read_only;
			else
				database->dbb_flags &= ~DBB_read_only;
			break;

		case frb_info_att_charset:
			database->dbb_att_charset =
				static_cast<short>(
					gds__vax_integer(reinterpret_cast<UCHAR*>(data), 2));
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
static void map_in_out(	DSQL_REQ		request,
						DSQL_MSG		message,
						USHORT	blr_length,
						UCHAR*	blr,
						USHORT	msg_length,
						UCHAR*	dsql_msg)
{
	PAR parameter, null;
	SSHORT *flag;

	USHORT count = parse_blr(blr_length, blr, msg_length, message->msg_parameters);

/* When mapping data from the external world, request will be non-NULL.
   When mapping data from an internal message, request will be NULL. */

	for (parameter = message->msg_parameters; parameter;
		 parameter = parameter->par_next)
	{
		if (parameter->par_index)
		{
			 /* Make sure the message given to us is long enough */

			DSC    desc   = parameter->par_user_desc;
			USHORT length = (SLONG) desc.dsc_address + desc.dsc_length;
			if (length > msg_length)
				break;
			if (!desc.dsc_dtype)
				break;

			flag = NULL;
			if ((null = parameter->par_null) != NULL)
			{
				USHORT null_offset = (USHORT)(ULONG) (null->par_user_desc.dsc_address);
				length = null_offset + sizeof(SSHORT);
				if (length > msg_length)
					break;

				if (!request) {
					flag = (SSHORT *) (dsql_msg + null_offset);
					*flag = *((SSHORT *) null->par_desc.dsc_address);
				}
				else {
					flag = (SSHORT *) null->par_desc.dsc_address;
					*flag = *((SSHORT *) (dsql_msg + null_offset));
				}
			}

			desc.dsc_address = dsql_msg + (SLONG) desc.dsc_address;
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
		ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 804,
				  gds_arg_gds, gds_dsql_sqlda_err, 0);
	}

	PAR dbkey;
	if (request &&
		((dbkey = request->req_parent_dbkey) != NULL) &&
		((parameter = request->req_dbkey) != NULL))
	{
		MOVD_move(&dbkey->par_desc, &parameter->par_desc);
		if ((null = parameter->par_null) != NULL)
		{
			flag = (SSHORT *) null->par_desc.dsc_address;
			*flag = 0;
		}
	}

	PAR rec_version;
	if (request &&
		((rec_version = request->req_parent_rec_version) != NULL) &&
		((parameter = request->req_rec_version) != NULL))
	{
		MOVD_move(&rec_version->par_desc, &parameter->par_desc);
		if ((null = parameter->par_null) != NULL)
		{
			flag = (SSHORT *) null->par_desc.dsc_address;
			*flag = 0;
		}
	}
}


/**
  
 	name_length
  
    @brief	Compute length of user supplied name.
 

    @param name

 **/
static USHORT name_length( TEXT * name)
{
	TEXT *p, *q;
	const char BLANK = '\040';

	q = name - 1;
	for (p = name; *p; p++) {
		if (*p != BLANK)
			q = p;
	}

	return (USHORT) ((q + 1) - name);
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
						UCHAR * blr, USHORT msg_length, PAR parameters)
{
	PAR parameter, null;
	USHORT count, index, offset, align, null_offset;
	DSC desc;

/* If there's no blr length, then the format of the current message buffer
   is identical to the format of the previous one. */

	if (!blr_length)
	{
		count = 0;
		for (parameter = parameters; parameter;
			 parameter = parameter->par_next)
		{
			if (parameter->par_index) {
				count++;
			}
		}
		return count;
	}

	if (*blr != blr_version4 && *blr != blr_version5)
	{
		ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 804,
				  gds_arg_gds, gds_dsql_sqlda_err, 0);
	}
	blr++;						/* skip the blr_version */
	if (*blr++ != blr_begin || *blr++ != blr_message)
	{
		ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 804,
				  gds_arg_gds, gds_dsql_sqlda_err, 0);
	}

	blr++;						/* skip the message number */
	count = *blr++;
	count += (*blr++) << 8;
	count /= 2;

	offset = 0;
	for (index = 1; index <= count; index++)
	{
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
			ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 804,
					  gds_arg_gds, gds_dsql_sqlda_err, 0);
		}

		align = type_alignments[desc.dsc_dtype];
		if (align)
			offset = FB_ALIGN(offset, align);
		desc.dsc_address = (UCHAR *) (ULONG) offset;
		offset += desc.dsc_length;

		if (*blr++ != blr_short || *blr++ != 0)
			ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 804,
					  gds_arg_gds, gds_dsql_sqlda_err, 0);

		align = type_alignments[dtype_short];
		if (align)
			offset = FB_ALIGN(offset, align);
		null_offset = offset;
		offset += sizeof(SSHORT);

		for (parameter = parameters; parameter; parameter = parameter->par_next)
		{
			if (parameter->par_index == index) {
				parameter->par_user_desc = desc;
				if (null = parameter->par_null) {
					null->par_user_desc.dsc_dtype = dtype_short;
					null->par_user_desc.dsc_scale = 0;
					null->par_user_desc.dsc_length = sizeof(SSHORT);
					null->par_user_desc.dsc_address = (UCHAR *)(ULONG) null_offset;
				}
			}
		}
	}

	if (*blr++ != (UCHAR) blr_end || offset != msg_length)
	{
		ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 804,
				  gds_arg_gds, gds_dsql_sqlda_err, 0);
	}

	return count;
}


/**
  
 	prepare
  
    @brief	Prepare a statement for execution.  Return SQL status
 	code.  Note: caller is responsible for pool handing.
 

    @param request
    @param string_length
    @param string
    @param client_dialect
    @param parser_version

 **/
static DSQL_REQ prepare(
				   DSQL_REQ request,
				   USHORT string_length,
				   TEXT * string,
				   USHORT client_dialect, USHORT parser_version)
{
	ISC_STATUS status;
	DSQL_NOD node;
	DSQL_MSG message;
	TEXT *p;
	USHORT length;
	TSQL tdsql;
	BOOLEAN stmt_ambiguous = FALSE;
	ISC_STATUS_ARRAY local_status;

	tdsql = GET_THREAD_DATA;

	MOVE_CLEAR(local_status, sizeof(ISC_STATUS) * ISC_STATUS_LENGTH);

	if (client_dialect > SQL_DIALECT_CURRENT)
		ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 901,
				  gds_arg_gds, gds_wish_list, 0);

	if (!string_length)
		string_length = strlen(string);

/* Get rid of the trailing ";" if there is one. */

	for (p = string + string_length; p-- > string;)
	{
		if (*p != ' ') {
			if (*p == ';')
				string_length = p - string;
			break;
		}
	}

/* Allocate a storage pool and get ready to parse */

	LEX_string(string, string_length, request->req_dbb->dbb_att_charset);

/* Parse the SQL statement.  If it croaks, return */

	if (dsql_yyparse(client_dialect,
	                 request->req_dbb->dbb_db_SQL_dialect,
	                 parser_version,
	                 &stmt_ambiguous))
	{
		ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 104, gds_arg_gds, gds_command_end_err,	/* Unexpected end of command */
				  0);
	}

/* allocate the send and receive messages */

	request->req_send = FB_NEW(*tdsql->tsql_default) dsql_msg;
	request->req_receive = message = FB_NEW(*tdsql->tsql_default) dsql_msg;
	message->msg_number = 1;

#ifdef SCROLLABLE_CURSORS
	if (request->req_dbb->dbb_base_level >= 5) {
		/* allocate a message in which to send scrolling information
		   outside of the normal send/receive protocol */

		request->req_async = message = FB_NEW(*tdsql->tsql_default) dsql_msg;
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

	node = PASS1_statement(request, DSQL_parse, false);
	if (!node)
		return request;

/* stop here for requests not requiring code generation */

	if (request->req_type == REQ_DDL && stmt_ambiguous &&
		request->req_dbb->dbb_db_SQL_dialect != client_dialect)
	{
		ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 817,
				  gds_arg_gds, isc_ddl_not_allowed_by_db_sql_dial,
				  gds_arg_number,
				  (SLONG) request->req_dbb->dbb_db_SQL_dialect, 0);
	}

	if (request->req_type == REQ_COMMIT ||
		request->req_type == REQ_COMMIT_RETAIN ||
		request->req_type == REQ_ROLLBACK)
	{
		return request;
	}

/* Work on blob segment requests */

	if (request->req_type == REQ_GET_SEGMENT ||
		request->req_type == REQ_PUT_SEGMENT)
	{
		GEN_port(request, request->req_blob->blb_open_in_msg);
		GEN_port(request, request->req_blob->blb_open_out_msg);
		GEN_port(request, request->req_blob->blb_segment_msg);
		return request;
	}

/* Generate BLR, DDL or TPB for request */

	if (request->req_type == REQ_START_TRANS ||
		request->req_type == REQ_DDL ||
		request->req_type == REQ_EXEC_PROCEDURE)
	{
		/* Allocate persistent blr string from request's pool. */

		request->req_blr_string = FB_NEW_RPT(*tdsql->tsql_default, 980) str;
	}
	else {
		/* Allocate transient blr string from permanent pool so
		   as not to unnecessarily bloat the request's pool. */

		request->req_blr_string = FB_NEW_RPT(*DSQL_permanent_pool, 980) str;
	}
	request->req_blr_string->str_length = 980;
	request->req_blr = reinterpret_cast<BLOB_PTR*>(request->req_blr_string->str_data);
	request->req_blr_yellow =
		request->req_blr + request->req_blr_string->str_length;

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
	length = reinterpret_cast<char*>(request->req_blr) - request->req_blr_string->str_data;

/* stop here for ddl requests */

	if (request->req_type == REQ_DDL)
		return request;

/* have the access method compile the request */

#ifdef DSQL_DEBUG
	if (DSQL_debug & 64) {
		dsql_trace("Resulting BLR code for DSQL:");
		gds__print_blr(reinterpret_cast<UCHAR*>(request->req_blr_string->str_data), 
			gds__trace_printer, 0, 0);
	}
#endif

/* stop here for execute procedure requests */

	if (request->req_type == REQ_EXEC_PROCEDURE) {
		return request;
	}

/* check for warnings */
	if (tdsql->tsql_status[2] == isc_arg_warning) {
		/* save a status vector */
		MOVE_FASTER(tdsql->tsql_status, local_status,
					sizeof(ISC_STATUS) * ISC_STATUS_LENGTH);
	}

	THREAD_EXIT;
	status = isc_compile_request(tdsql->tsql_status,
								 &request->req_dbb->dbb_database_handle,
								 &request->req_handle, length,
								 request->req_blr_string->str_data);
	THREAD_ENTER;

/* restore warnings (if there are any) */
	if (local_status[2] == isc_arg_warning)
	{
		int indx, len, warning;

		/* find end of a status vector */
		PARSE_STATUS(tdsql->tsql_status, indx, warning);
		if (indx)
			--indx;

		/* calculate length of saved warnings */
		PARSE_STATUS(local_status, len, warning);
		len -= 2;

		if ((len + indx - 1) < ISC_STATUS_LENGTH)
			MOVE_FASTER(&local_status[2], &tdsql->tsql_status[indx],
						sizeof(ISC_STATUS) * len);
	}

	delete request->req_blr_string;
	request->req_blr_string = NULL;

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
	struct tsql *tdsql;

	tdsql = GET_THREAD_DATA;

	Firebird::status_exception::raise(tdsql->tsql_status[1]);
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
						UCHAR*	string,
						UCHAR*	ptr,
						UCHAR*	end)
{

	if (ptr + length + 3 >= end) {
		*ptr = gds_info_truncated;
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
static void release_request(DSQL_REQ request, bool top_level)
{

	ISC_STATUS_ARRAY status_vector;

	tsql* tdsql = GET_THREAD_DATA;

	// If request is parent, orphan the children and
	// release a portion of their requests

	for (DSQL_REQ child = request->req_offspring; child; child = child->req_sibling) {
		child->req_flags |= REQ_orphan;
		child->req_parent = NULL;
		DsqlMemoryPool *save_default = tdsql->tsql_default;
		tdsql->tsql_default = child->req_pool;
		release_request(child, false);
		tdsql->tsql_default = save_default;
	}

// For top level requests that are linked to a parent, unlink it

	if (top_level && request->req_parent)
	{
		DSQL_REQ parent = request->req_parent;
		for (DSQL_REQ* ptr = &parent->req_offspring;
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

	if (request->req_blr_string) {
		delete request->req_blr_string;
		request->req_blr_string = NULL;
	}

// If a request has been compiled, release it now

	if (request->req_handle) {
		THREAD_EXIT;
		isc_release_request(status_vector, &request->req_handle);
		THREAD_ENTER;
	}

/* Only release the entire request for top level requests */

	if (top_level)
		DsqlMemoryPool::deletePool(request->req_pool);
}


/**
  
 	return_success
  
    @brief	Set up status vector to reflect successful execution.
 


 **/
static ISC_STATUS return_success(void)
{

	TSQL tdsql = GET_THREAD_DATA;

	ISC_STATUS*p = tdsql->tsql_status;
	*p++ = gds_arg_gds;
	*p++ = FB_SUCCESS;

	// do not overwrite warnings
	if (*p != isc_arg_warning) {
		*p = gds_arg_end;
	}

	RESTORE_THREAD_DATA;

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
static UCHAR *var_info(
					   DSQL_MSG message,
					   const UCHAR * items,
					   const UCHAR * end_describe,
					   UCHAR * info, UCHAR * end, USHORT first_index)
{
	PAR par;
	UCHAR item, *buffer, buf[128];
	const UCHAR *describe;
	USHORT length;
	SLONG sql_type, sql_sub_type, sql_scale, sql_len;

	if (!message || !message->msg_index)
		return info;

	for (par = message->msg_par_ordered; par; par = par->par_ordered)
		if (par->par_index && par->par_index >= first_index) {
			sql_len = par->par_desc.dsc_length;
			sql_sub_type = 0;
			sql_scale = 0;
			switch (par->par_desc.dsc_dtype) {
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
				sql_scale = par->par_desc.dsc_scale;
				break;

			case dtype_text:
				sql_type = SQL_TEXT;
				sql_sub_type = par->par_desc.dsc_sub_type;
				break;

			case dtype_blob:
				sql_type = SQL_BLOB;
				sql_sub_type = par->par_desc.dsc_sub_type;
				break;

			case dtype_varying:
				sql_type = SQL_VARYING;
				sql_len -= sizeof(USHORT);
				sql_sub_type = par->par_desc.dsc_sub_type;
				break;

			case dtype_short:
			case dtype_long:
			case dtype_int64:
				if (par->par_desc.dsc_dtype == dtype_short)
					sql_type = SQL_SHORT;
				else if (par->par_desc.dsc_dtype == dtype_long)
					sql_type = SQL_LONG;
				else
					sql_type = SQL_INT64;
				sql_scale = par->par_desc.dsc_scale;
				if (par->par_desc.dsc_sub_type)
					sql_sub_type = par->par_desc.dsc_sub_type;
				break;

			case dtype_quad:
				sql_type = SQL_QUAD;
				sql_scale = par->par_desc.dsc_scale;
				break;

			default:
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 804,
						  gds_arg_gds, gds_dsql_datatype_err, 0);
			}

			if (sql_type && (par->par_desc.dsc_flags & DSC_nullable))
				sql_type++;

			for (describe = items; describe < end_describe;) {
				buffer = buf;
				switch (item = *describe++) {
				case gds_info_sql_sqlda_seq:
					length = convert((SLONG) par->par_index, buffer);
					break;

				case gds_info_sql_message_seq:
					length = 0;
					break;

				case gds_info_sql_type:
					length = convert((SLONG) sql_type, buffer);
					break;

				case gds_info_sql_sub_type:
					length = convert((SLONG) sql_sub_type, buffer);
					break;

				case gds_info_sql_scale:
					length = convert((SLONG) sql_scale, buffer);
					break;

				case gds_info_sql_length:
					length = convert((SLONG) sql_len, buffer);
					break;

				case gds_info_sql_null_ind:
					length = convert((SLONG) (sql_type & 1), buffer);
					break;

				case gds_info_sql_field:
					if (buffer = reinterpret_cast<UCHAR *>(par->par_name))
						length = strlen(reinterpret_cast<SCHAR *>(buffer));
					else
						length = 0;
					break;

				case gds_info_sql_relation:
					if (buffer = reinterpret_cast<UCHAR *>(par->par_rel_name))
						length = strlen(reinterpret_cast<SCHAR *>(buffer));
					else
						length = 0;
					break;

				case gds_info_sql_owner:
					if (buffer = reinterpret_cast<UCHAR *>(par->par_owner_name))
						length = strlen(reinterpret_cast<SCHAR *>(buffer));
					else
						length = 0;
					break;

				case gds_info_sql_alias:
					if (buffer = reinterpret_cast<UCHAR *>(par->par_alias))
						length = strlen(reinterpret_cast<SCHAR *>(buffer));
					else
						length = 0;
					break;

				default:
					buffer[0] = item;
					item = gds_info_error;
					length = 1 + convert((SLONG) gds_infunk, buffer + 1);
					break;
				}

				if (!(info = put_item(item, length, buffer, info, end)))
					return info;
			}

			if (info + 1 >= end) {
				*info = gds_info_truncated;
				return NULL;
			}
			*info++ = gds_info_sql_describe_end;
		}

	return info;
}

