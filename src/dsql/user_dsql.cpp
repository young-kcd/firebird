/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		user_dsql.cpp
 *	DESCRIPTION:	Above the Y-valve entrypoints for DSQL
 *
 *
 * This module contains DSQL related routines that sit on top
 * of the Y-valve.  This includes the original (pre-version 4)
 * DSQL routines as well as the alternate VMS and Ada entrypoints
 * of the new DSQL routines.  The pre-version 4 entrypoints
 * retain their "gds__" prefix while the new routines are prefixed
 * with "isc_dsql_".
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
 *
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "MPEXL" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2003.01.12 Alex Peshkoff - Code cleanup, bad level of indirection
 *
 */

#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include "../jrd/common.h"
#include <stdarg.h>
#include "../dsql/dsql.h"
#include "../dsql/chars.h"
#include "../dsql/sqlda.h"
#include "../jrd/blr.h"
#include "gen/iberror.h"
#include "../jrd/align.h"
#include "../jrd/gds_proto.h"
#include "../jrd/why_proto.h"
#include "../dsql/user__proto.h"

#ifdef VMS
#include <descrip.h>
#endif

enum name_type {
	NAME_statement = 1,
	NAME_cursor = 2
};

typedef FB_API_HANDLE HNDL;

/* declare a structure which enables us to associate a cursor with a 
   statement and vice versa */

struct dsql_name;	// fwd. decl.

struct stmt
{
	stmt*	stmt_next;			// next in chain 
	dsql_name*	stmt_stmt;			// symbol table entry for statement name
	dsql_name*	stmt_cursor;		// symbol table entry for cursor name
	HNDL	stmt_handle;		// stmt handle returned by dsql_xxx 
	HNDL	stmt_db_handle;		// database handle for this statement 
};

// declare a structure to hold the cursor and statement names 

struct dsql_name
{
	dsql_name*	name_next;
	dsql_name*	name_prev;
	stmt*	name_stmt;
	USHORT	name_length;
	SCHAR	name_symbol[1];
};


static void		cleanup(void*);
static void		cleanup_database(FB_API_HANDLE*, void*);
static ISC_STATUS	error(const std::exception& ex);
static ISC_STATUS	error();
static void		error_post(ISC_STATUS, ...);
static dsql_name*		lookup_name(const SCHAR*, dsql_name*);
static stmt*		lookup_stmt(const SCHAR*, dsql_name*, name_type);
static void		init(FB_API_HANDLE*);
static dsql_name*		insert_name(const SCHAR*, dsql_name**, stmt*);
static USHORT	name_length(const SCHAR*);
static void		remove_name(dsql_name*, dsql_name**);
static bool		scompare(const SCHAR*, USHORT, const SCHAR*, USHORT);

// declare the private data 

#pragma FB_COMPILER_MESSAGE("Dragons ahead. Static data. Not thread safe!")

static bool		init_flag		= false;	// whether we've been initialized 
static dsql_err_stblock*	UDSQL_error		= NULL;
static stmt*		statements		= NULL;
static dsql_name*		statement_names	= NULL;
static dsql_name*		cursor_names	= NULL;
static dsql_dbb*		databases		= NULL;

static inline void set_global_private_status(ISC_STATUS* user_status, ISC_STATUS* local_status)
{
	UDSQL_error->dsql_user_status = user_status;
	UDSQL_error->dsql_status = (user_status) ? user_status : local_status;
}

static inline void INIT_DSQL(ISC_STATUS* user_status, ISC_STATUS* local_status)
{
	init(0);
	set_global_private_status(user_status, local_status);
}


//____________________________________________________________
//
//	Close a dynamic SQL cursor.
//
ISC_STATUS API_ROUTINE isc_embed_dsql_close(ISC_STATUS* user_status, const SCHAR* name)
{
	ISC_STATUS_ARRAY local_status;

	INIT_DSQL(user_status, local_status);
	try
	{
		// get the symbol table entry

		stmt* statement = lookup_stmt(name, cursor_names, NAME_cursor);

		return isc_dsql_free_statement(	user_status,
									&statement->stmt_handle,
									DSQL_close);
	}
	catch(const std::exception& ex)
	{
		return error(ex);
	}

}


//____________________________________________________________
//
//	Declare a cursor for a dynamic request.
//
ISC_STATUS API_ROUTINE isc_embed_dsql_declare(	ISC_STATUS*	user_status,
											const SCHAR*	stmt_name,
											const SCHAR*	cursor)
{
	ISC_STATUS_ARRAY local_status;

	INIT_DSQL(user_status, local_status);
	try
	{
		// get the symbol table entry

		stmt* statement = lookup_stmt(stmt_name, statement_names, NAME_statement);

		const ISC_STATUS s =
			isc_dsql_set_cursor_name(user_status,
								 &statement->stmt_handle,
								 cursor,
								 0);
		if (s) {
			return s;
		}

		statement->stmt_cursor = insert_name(cursor, &cursor_names, statement);

		return s;
	}
	catch(const std::exception& ex)
	{
		return error(ex);
	}
}


//____________________________________________________________
//
//	Describe output parameters for a prepared statement.
//
ISC_STATUS API_ROUTINE isc_embed_dsql_describe(ISC_STATUS* user_status,
										   const SCHAR* stmt_name,
										   USHORT dialect, XSQLDA* sqlda)
{
	ISC_STATUS_ARRAY local_status;

	INIT_DSQL(user_status, local_status);

	try
	{
		// get the symbol table entry

		stmt* statement = lookup_stmt(stmt_name, statement_names, NAME_statement);

		return isc_dsql_describe(user_status,
							 &statement->stmt_handle,
							 dialect,
							 sqlda);
	}
	catch(const std::exception& ex)
	{
		return error(ex);
	}
}


//____________________________________________________________
//
//	isc_embed_dsql_descr_bind
//
ISC_STATUS API_ROUTINE isc_embed_dsql_descr_bind(	ISC_STATUS*	user_status,
												const SCHAR*	stmt_name,
												USHORT	dialect,
												XSQLDA*	sqlda)
{
	return isc_embed_dsql_describe_bind(user_status,
										stmt_name,
										dialect,
										sqlda);
}


//____________________________________________________________
//
//	Describe input parameters for a prepared statement.
//
ISC_STATUS API_ROUTINE isc_embed_dsql_describe_bind(ISC_STATUS*	user_status,
												const SCHAR*	stmt_name,
												USHORT	dialect,
												XSQLDA*	sqlda)
{
	ISC_STATUS_ARRAY local_status;

	INIT_DSQL(user_status, local_status);
	try
	{
		// get the symbol table entry

		stmt* statement = lookup_stmt(stmt_name, statement_names, NAME_statement);

		return isc_dsql_describe_bind(user_status,
								  &statement->stmt_handle,
								  dialect,
								  sqlda);
	}
	catch(const std::exception& ex)
	{
		return error(ex);
	}
}


//____________________________________________________________
//
//	Execute a non-SELECT dynamic SQL statement.
//
ISC_STATUS API_ROUTINE isc_embed_dsql_execute(ISC_STATUS*	user_status,
										  FB_API_HANDLE*	trans_handle,
										  const SCHAR*	stmt_name,
										  USHORT	dialect,
										  XSQLDA*	sqlda)
{
	return isc_embed_dsql_execute2(	user_status,
									trans_handle,
									stmt_name,
									dialect,
									sqlda,
									NULL);
}


//____________________________________________________________
//
//	Execute a non-SELECT dynamic SQL statement.
//
ISC_STATUS API_ROUTINE isc_embed_dsql_execute2(ISC_STATUS*	user_status,
										   FB_API_HANDLE*	trans_handle,
										   const SCHAR*	stmt_name,
										   USHORT	dialect,
										   XSQLDA*	in_sqlda,
										   XSQLDA*	out_sqlda)
{
	ISC_STATUS_ARRAY local_status;

	INIT_DSQL(user_status, local_status);
	try
	{
		// get the symbol table entry

		stmt* statement = lookup_stmt(stmt_name, statement_names, NAME_statement);

		return isc_dsql_execute2(	user_status,
								trans_handle,
								&statement->stmt_handle,
								dialect,
								in_sqlda,
								out_sqlda);
	}
	catch(const std::exception& ex)
	{
		return error(ex);
	}
}


//____________________________________________________________
//
//	isc_embed_dsql_exec_immed
//
ISC_STATUS API_ROUTINE isc_embed_dsql_exec_immed(ISC_STATUS*	user_status,
											 FB_API_HANDLE*	db_handle,
											 FB_API_HANDLE*	trans_handle,
											 USHORT		length,
											 const SCHAR*		string,
											 USHORT		dialect,
											 XSQLDA*	sqlda)
{
	return isc_embed_dsql_execute_immed(user_status,
										db_handle,
										trans_handle,
										length,
										string,
										dialect,
										sqlda);
}


//____________________________________________________________
//
//	Prepare a statement for execution.
//
ISC_STATUS API_ROUTINE isc_embed_dsql_execute_immed(ISC_STATUS*	user_status,
												FB_API_HANDLE*	db_handle,
												FB_API_HANDLE*	trans_handle,
												USHORT	length,
												const SCHAR*	string,
												USHORT	dialect,
												XSQLDA*	sqlda)
{
	return isc_embed_dsql_exec_immed2(	user_status,
										db_handle,
										trans_handle,
										length,
										string,
										dialect,
										sqlda,
										NULL);
}


//____________________________________________________________
//
//	Prepare a statement for execution.
//
ISC_STATUS API_ROUTINE isc_embed_dsql_exec_immed2(	ISC_STATUS*	user_status,
												FB_API_HANDLE*	db_handle,
												FB_API_HANDLE*	trans_handle,
												USHORT	length,
												const SCHAR*	string,
												USHORT	dialect,
												XSQLDA*	in_sqlda,
												XSQLDA*	out_sqlda)
{
	return isc_dsql_exec_immed2(user_status,
								db_handle,
								trans_handle,
								length,
								string,
								dialect,
								in_sqlda,
								out_sqlda);
}


#ifdef VMS
//____________________________________________________________
//
//	An execute immediate for COBOL to call
//
ISC_STATUS API_ROUTINE isc_embed_dsql_execute_immed_d(ISC_STATUS* user_status,
												  FB_API_HANDLE* db_handle,
												  FB_API_HANDLE* trans_handle,
												  struct dsc$descriptor_s *
												  string, USHORT dialect,
												  XSQLDA* sqlda)
{
	return isc_embed_dsql_exec_immed2_d(user_status,
										db_handle, trans_handle, string,
										dialect, sqlda, NULL);
}
#endif


#ifdef VMS
//____________________________________________________________
//
//	An execute immediate for COBOL to call
//
ISC_STATUS API_ROUTINE isc_embed_dsql_exec_immed2_d(ISC_STATUS* user_status,
												FB_API_HANDLE* db_handle,
												FB_API_HANDLE* trans_handle,
												struct dsc$descriptor_s *
												string, USHORT dialect,
												XSQLDA* in_sqlda,
												XSQLDA* out_sqlda)
{
	USHORT len = string->dsc$w_length;

	return isc_embed_dsql_exec_immed2(user_status, db_handle,
									  trans_handle, len,
									  string->dsc$a_pointer, dialect,
									  in_sqlda, out_sqlda);
}
#endif


//____________________________________________________________
//
//	Fetch next record from a dynamic SQL cursor
//
ISC_STATUS API_ROUTINE isc_embed_dsql_fetch(ISC_STATUS* user_status,
										const SCHAR* cursor_name,
										USHORT dialect, XSQLDA* sqlda)
{
	ISC_STATUS_ARRAY local_status;

	INIT_DSQL(user_status, local_status);
	try
	{
		stmt* statement = lookup_stmt(cursor_name, cursor_names, NAME_cursor);

		return isc_dsql_fetch(user_status,
						  &statement->stmt_handle,
						  dialect,
						  sqlda);
	}
	catch(const std::exception& ex)
	{
		return error(ex);
	}
}


#ifdef SCROLLABLE_CURSORS
//____________________________________________________________
//
//	Fetch next record from a dynamic SQL cursor
//
ISC_STATUS API_ROUTINE isc_embed_dsql_fetch2(	ISC_STATUS*	user_status,
											const SCHAR*	cursor_name,
											USHORT	dialect,
											XSQLDA*	sqlda,
											USHORT	direction,
											SLONG	offset)
{
	ISC_STATUS_ARRAY local_status;

	INIT_DSQL(user_status, local_status);
	try
	{
		// get the symbol table entry

		stmt* statement = lookup_stmt(cursor_name, cursor_names, NAME_cursor);

		return isc_dsql_fetch2(	user_status,
							&statement->stmt_handle,
							dialect,
							sqlda,
							direction,
							offset);
	}
	catch(const std::exception& ex)
	{
		return error(ex);
	}
}
#endif


//____________________________________________________________
//
//	Fetch next record from a dynamic SQL cursor (ADA version)
//
ISC_STATUS API_ROUTINE isc_embed_dsql_fetch_a(	ISC_STATUS*	user_status,
											int*	sqlcode,
											const SCHAR*	cursor_name,
											USHORT	dialect,
											XSQLDA*	sqlda)
{
	*sqlcode = 0;

	ISC_STATUS s = isc_embed_dsql_fetch(user_status, cursor_name, dialect, sqlda);
	if (s == 100) {
		*sqlcode = 100;
	}

	return FB_SUCCESS;
}


#ifdef SCROLLABLE_CURSORS
ISC_STATUS API_ROUTINE isc_embed_dsql_fetch2_a(ISC_STATUS* user_status,
										   int *sqlcode,
										   const SCHAR* cursor_name,
										   USHORT dialect,
										   XSQLDA* sqlda,
										   USHORT direction, SLONG offset)
{
/**************************************
 *
 *	i s c _ e m b e d _ d s q l _ f e t c h 2 _ a
 *
 **************************************
 *
 * Functional description
 *	Fetch next record from a dynamic SQL cursor (ADA version)
 *
 **************************************/
	ISC_STATUS s;

	*sqlcode = 0;

	s =
		isc_embed_dsql_fetch2(user_status, cursor_name, dialect, sqlda,
							  direction, offset);
	if (s == 100)
		*sqlcode = 100;

	return FB_SUCCESS;
}
#endif


ISC_STATUS API_ROUTINE isc_embed_dsql_insert(ISC_STATUS* user_status,
										 const SCHAR* cursor_name,
										 USHORT dialect, XSQLDA* sqlda)
{
/**************************************
 *
 *	i s c _ e m b e d _ d s q l _ i n s e r t
 *
 **************************************
 *
 * Functional description
 *	Insert next record into a dynamic SQL cursor
 *
 **************************************/
	ISC_STATUS_ARRAY local_status;
	stmt* statement;

	INIT_DSQL(user_status, local_status);
	try
	{
		// get the symbol table entry 

		statement = lookup_stmt(cursor_name, cursor_names, NAME_cursor);

		return isc_dsql_insert(user_status,
						   &statement->stmt_handle,
						   dialect,
						   sqlda);
	}
	catch(const std::exception& ex)
	{
		return error(ex);
	}
}


void API_ROUTINE isc_embed_dsql_length(const UCHAR* string, USHORT* length)
{
/**************************************
 *
 *	i s c _ e m b e d _ d s q l _ l e n g t h
 *
 **************************************
 *
 * Functional description
 *	Determine length of a ';' or null terminated string
 *
 **************************************/
	const UCHAR* p;
	for (p = string; *p && *p != ';'; p++)
		if (classes[*p] & CHR_QUOTE) {
			for (UCHAR prev = 0, quote = *p++; *p == quote || prev == quote;)
				prev = *p++;
			p--;
		}

	*length = p - string + 1;
}


ISC_STATUS API_ROUTINE isc_embed_dsql_open(ISC_STATUS* user_status,
									   FB_API_HANDLE* trans_handle,
									   const SCHAR* cursor_name,
									   USHORT dialect, XSQLDA* sqlda)
{
/**************************************
 *
 *	i s c _ e m b e d _ d s q l _ o p e n
 *
 **************************************
 *
 * Functional description
 *	Open a dynamic SQL cursor.
 *
 **************************************/

	return isc_embed_dsql_open2(user_status, trans_handle, cursor_name,
								dialect, sqlda, NULL);
}


ISC_STATUS API_ROUTINE isc_embed_dsql_open2(ISC_STATUS* user_status,
										FB_API_HANDLE* trans_handle,
										const SCHAR* cursor_name,
										USHORT dialect,
										XSQLDA* in_sqlda, XSQLDA* out_sqlda)
{
/**************************************
 *
 *	i s c _ e m b e d _ d s q l _ o p e n 2
 *
 **************************************
 *
 * Functional description
 *	Open a dynamic SQL cursor.
 *
 **************************************/
	ISC_STATUS_ARRAY local_status;
	stmt* stmt;

	INIT_DSQL(user_status, local_status);
	try
	{
		// get the symbol table entry 

		stmt = lookup_stmt(cursor_name, cursor_names, NAME_cursor);

		return isc_dsql_execute2(user_status,
							 trans_handle,
							 &stmt->stmt_handle,
							 dialect, in_sqlda, out_sqlda);
	}
	catch(const std::exception& ex)
	{
		return error(ex);
	}
}


ISC_STATUS API_ROUTINE isc_embed_dsql_prepare(ISC_STATUS*	user_status,
										  FB_API_HANDLE*	db_handle,
										  FB_API_HANDLE*	trans_handle,
										  const SCHAR*	stmt_name,
										  USHORT	length,
										  const SCHAR*	string,
										  USHORT	dialect,
										  XSQLDA*	sqlda)
 {
/**************************************
 *
 *	i s c _ e m b e d _ d s q l _ p r e p a r e
 *
 **************************************
 *
 * Functional description
 *	Prepare a statement for execution.
 *
 **************************************/
	ISC_STATUS s;
	ISC_STATUS_ARRAY local_status, local_status2;
	stmt* statement;
	HNDL stmt_handle;

	init(db_handle);
	set_global_private_status(user_status, local_status);

	try {

	dsql_name* name = lookup_name(stmt_name, statement_names);

	if (name && name->name_stmt->stmt_db_handle == *db_handle)
	{
		/* The statement name already exists for this database.
		   Re-use its statement handle. */

		statement = name->name_stmt;
		stmt_handle = statement->stmt_handle;
	}
	else
	{
		/* This is a new statement name for this database.
		   Allocate a statement handle for it. */

		if (name) {
			isc_embed_dsql_release(user_status, stmt_name);
		}
		statement = NULL;
		stmt_handle = 0;
		s = isc_dsql_allocate_statement(user_status,
										db_handle,
										&stmt_handle);
		if (s)
		{
			return s;
		}
	}

	s = isc_dsql_prepare(user_status,
						 trans_handle,
						 &stmt_handle,
						 length, string, dialect, sqlda);

	if (s) {
		// An error occurred.  Free any newly allocated statement handle. 

		if (!statement) {
			isc_dsql_free_statement(local_status2,
									&stmt_handle,
									DSQL_drop);
		}
		return error();
	}

/* If a new statement was allocated, add it to the symbol table and insert it
   into the list of statements */

	if (!statement)
	{
		statement = (stmt*) gds__alloc((SLONG) sizeof(stmt));
		// FREE: by user calling isc_embed_dsql_release() 
		if (!statement)			// NOMEM: 
			error_post(isc_virmemexh, 0);

#ifdef DEBUG_GDS_ALLOC
		gds_alloc_flag_unfreed((void *) statement);
#endif	// DEBUG_GDS_ALLOC 

		statement->stmt_next = statements;
		statements = statement;

		statement->stmt_db_handle = (HNDL) * db_handle;
		statement->stmt_stmt =
			insert_name(stmt_name, &statement_names, statement);
	}
	else if (statement->stmt_cursor)
		remove_name(statement->stmt_cursor, &cursor_names);

	statement->stmt_handle = stmt_handle;
	statement->stmt_cursor = NULL;

	return s;

	}	// try
	catch (const std::exception& ex)
	{
		return error(ex);
	}

}


#ifdef VMS
ISC_STATUS API_ROUTINE isc_embed_dsql_prepare_d(ISC_STATUS* user_status,
											FB_API_HANDLE* db_handle,
											FB_API_HANDLE* trans_handle,
											SCHAR* stmt_name,
											struct dsc$descriptor_s * string,
											USHORT dialect, XSQLDA* sqlda)
{
/**************************************
 *
 *	i s c _ e m b e d _ d s q l _ p r e p a r e _ d
 *
 **************************************
 *
 * Functional description
 *	A prepare for COBOL to call
 *
 **************************************/
	const USHORT len = string->dsc$w_length;

	return isc_embed_dsql_prepare(user_status, db_handle,
								  trans_handle, stmt_name, len,
								  string->dsc$a_pointer, dialect, sqlda);
}
#endif


ISC_STATUS API_ROUTINE isc_embed_dsql_release(ISC_STATUS* user_status,
										  const SCHAR* stmt_name)
{
/**************************************
 *
 *	i s c _ e m b e d _ d s q l _ r e l e a s e
 *
 **************************************
 *
 * Functional description
 *	Release request for a dsql statement
 *
 **************************************/
	ISC_STATUS_ARRAY	local_status;
	stmt**	stmt_ptr;
	stmt* p;

	INIT_DSQL(user_status, local_status);
	try
	{
		// If a request already exists under that name, purge it out 

		stmt* statement = lookup_stmt(stmt_name, statement_names, NAME_statement);

		ISC_STATUS s =
			isc_dsql_free_statement(user_status,
								&statement->stmt_handle,
								DSQL_drop);
		if (s) {
			return s;
		}

		// remove the statement from the symbol tables

		if (statement->stmt_stmt)
			remove_name(statement->stmt_stmt, &statement_names);
		if (statement->stmt_cursor)
			remove_name(statement->stmt_cursor, &cursor_names);

		// and remove this statement from the local list

		for (stmt_ptr = &statements; p = *stmt_ptr; stmt_ptr = &p->stmt_next) {
			if (p == statement) {
				*stmt_ptr = statement->stmt_next;
				gds__free(statement);
				break;
			}
		}

		return s;
	}
	catch(const std::exception& ex)
	{
		return error(ex);
	}
}


#ifdef VMS
int API_ROUTINE isc_dsql_execute_immediate_d(
											 ISC_STATUS* user_status,
											 int db_handle,
											 int trans_handle,
											 struct dsc$descriptor_s* string,
											 USHORT dialect, XSQLDA* sqlda)
{
/**************************************
 *
 *	i s c _ d s q l _ e x e c u t e _ i m m e d i a t e _ d
 *
 **************************************
 *
 * Functional description
 *	An execute immediate for COBOL to call
 *
 **************************************/
	const USHORT len = string->dsc$w_length;

	return isc_dsql_execute_immediate(user_status, db_handle, trans_handle,
									  len, string->dsc$a_pointer, dialect,
									  sqlda);
}
#endif


ISC_STATUS API_ROUTINE isc_dsql_fetch_a(ISC_STATUS* user_status,
									int* sqlcode,
									int* stmt_handle,
									USHORT dialect, int* sqlda)
{
/**************************************
 *
 *	i s c _ d s q l _ f e t c h _ a
 *
 **************************************
 *
 * Functional description
 *	Fetch next record from a dynamic SQL cursor (ADA version)
 *
 **************************************/
	*sqlcode = 0;

	const ISC_STATUS s = isc_dsql_fetch(	user_status,
						reinterpret_cast<FB_API_HANDLE*>(stmt_handle),
						dialect,
						reinterpret_cast<XSQLDA*>(sqlda));
	if (s == 100)
		*sqlcode = 100;

	return FB_SUCCESS;
}


#ifdef SCROLLABLE_CURSORS
ISC_STATUS API_ROUTINE isc_dsql_fetch2_a(ISC_STATUS* user_status,
									 int *sqlcode,
									 int *stmt_handle,
									 USHORT dialect,
									 int *sqlda,
									 USHORT direction, SLONG offset)
{
/**************************************
 *
 *	i s c _ d s q l _ f e t c h 2 _ a
 *
 **************************************
 *
 * Functional description
 *	Fetch next record from a dynamic SQL cursor (ADA version)
 *
 **************************************/
	*sqlcode = 0;

	const ISC_STATUS s =
		isc_dsql_fetch2(user_status, stmt_handle, dialect, sqlda, direction,
						offset);
	if (s == 100)
		*sqlcode = 100;

	return FB_SUCCESS;
}
#endif


#ifdef VMS
int API_ROUTINE isc_dsql_prepare_d(	ISC_STATUS*						user_status,
									int							trans_handle,
									int							stmt_handle,
									struct dsc$descriptor_s*	string,
									USHORT						dialect,
									XSQLDA*						sqlda)
{
/**************************************
 *
 *	i s c _ d s q l _ p r e p a r e _ d
 *
 **************************************
 *
 * Functional description
 *	A prepare for COBOL to call
 *
 **************************************/
	const USHORT len = string->dsc$w_length;

	return isc_dsql_prepare(user_status, trans_handle, stmt_handle,
							len, string->dsc$a_pointer, dialect, sqlda);
}
#endif


/**************************************
 *
 *	i s c _ . . .
 *
 **************************************
 *
 * Functional description
 *	The following routines define the
 *	old isc_ entrypoints.
 *
 **************************************/
ISC_STATUS API_ROUTINE isc_close(ISC_STATUS* status_vector, const SCHAR* statement_name)
{
	return isc_embed_dsql_close(status_vector, statement_name);
}

ISC_STATUS API_ROUTINE isc_declare(	ISC_STATUS*	status_vector,
								const SCHAR*	statement_name,
								const SCHAR*	cursor_name)
{
	return isc_embed_dsql_declare(status_vector, statement_name, cursor_name);
}

ISC_STATUS API_ROUTINE isc_describe(ISC_STATUS* status_vector,
								const SCHAR* statement_name, SQLDA* sqlda)
{
	return isc_embed_dsql_describe(status_vector,
								   statement_name,
								   DIALECT_sqlda,
								   reinterpret_cast<XSQLDA*>(sqlda));
}

ISC_STATUS API_ROUTINE isc_describe_bind(ISC_STATUS* status_vector,
									 const SCHAR* statement_name,
									 SQLDA* sqlda)
{
	return isc_embed_dsql_describe_bind(status_vector,
										statement_name,
										DIALECT_sqlda,
										reinterpret_cast<XSQLDA*>(sqlda));
}

ISC_STATUS API_ROUTINE isc_dsql_finish(HNDL* db_handle)
{
	return 0;
}

ISC_STATUS API_ROUTINE isc_execute(ISC_STATUS* status_vector,
							   FB_API_HANDLE* tra_handle,
							   const SCHAR* statement_name, SQLDA* sqlda)
{
	return isc_embed_dsql_execute(status_vector,
								  tra_handle,
								  statement_name,
								  DIALECT_sqlda,
								  reinterpret_cast<XSQLDA*>(sqlda));
}

ISC_STATUS API_ROUTINE isc_execute_immediate(ISC_STATUS* status_vector,
										 FB_API_HANDLE* db_handle,
										 FB_API_HANDLE* tra_handle,
										 SSHORT* sql_length, const SCHAR* sql)
{
	return isc_embed_dsql_execute_immed(status_vector,
										db_handle, tra_handle,
										(USHORT) ((sql_length) ? *sql_length :
												  0), sql,
										DIALECT_sqlda, NULL);
}

#ifdef VMS
ISC_STATUS API_ROUTINE isc_execute_immediate_d(ISC_STATUS* status_vector,
										   SLONG* db_handle,
										   SLONG* tra_handle,
										   struct dsc$descriptor_s * string)
{
	USHORT len;

	len = string->dsc$w_length;

	return isc_execute_immediate(status_vector,
								 db_handle,
								 tra_handle, &len, string->dsc$a_pointer);
}
#endif

ISC_STATUS API_ROUTINE isc_fetch(ISC_STATUS* status_vector,
							 const SCHAR* cursor_name, SQLDA* sqlda)
{
	return isc_embed_dsql_fetch(status_vector,
								cursor_name,
								DIALECT_sqlda,
								reinterpret_cast<XSQLDA*>(sqlda));
}

ISC_STATUS API_ROUTINE isc_fetch_a(ISC_STATUS* status_vector,
							   int *sqlcode,
							   const SCHAR* cursor_name, SQLDA* sqlda)
{
	return isc_embed_dsql_fetch_a(status_vector,
								  sqlcode,
								  cursor_name,
								  DIALECT_sqlda,
								  reinterpret_cast<XSQLDA*>(sqlda));
}

ISC_STATUS API_ROUTINE isc_open(ISC_STATUS* status_vector,
							FB_API_HANDLE* tra_handle,
							const SCHAR* cursor_name, SQLDA* sqlda)
{
	return isc_embed_dsql_open(status_vector,
							   tra_handle,
							   cursor_name,
							   DIALECT_sqlda,
							   reinterpret_cast<XSQLDA*>(sqlda));
}

ISC_STATUS API_ROUTINE isc_prepare(	ISC_STATUS*	status_vector,
								FB_API_HANDLE*	db_handle,
								FB_API_HANDLE*	tra_handle,
								const SCHAR*	statement_name,
								SSHORT*	sql_length,
								const SCHAR*	sql,
								SQLDA*	sqlda)
{
	return isc_embed_dsql_prepare(	status_vector,
									db_handle,
									tra_handle,
									statement_name,
									(USHORT) ((sql_length) ? *sql_length : 0),
									sql,
									DIALECT_sqlda,
									reinterpret_cast<XSQLDA*>(sqlda));
}

#ifdef VMS
ISC_STATUS API_ROUTINE isc_prepare_d(ISC_STATUS* status_vector,
								 SLONG* db_handle,
								 SLONG* tra_handle,
								 SCHAR* statement_name,
								 struct dsc$descriptor_s * string,
								 SQLDA* sqlda)
{
	USHORT len;

	len = string->dsc$w_length;

	return isc_prepare(status_vector,
					   db_handle,
					   tra_handle,
					   statement_name, &len, string->dsc$a_pointer, sqlda);
}
#endif

ISC_STATUS API_ROUTINE isc_dsql_release(ISC_STATUS*	status_vector,
									const SCHAR*	statement_name)
{
	return isc_embed_dsql_release(status_vector, statement_name);
}

int API_ROUTINE isc_to_sqlda(	SQLDA*	sqlda,
								int		number,
								SCHAR*	host_var,
								int		host_var_size,
								SCHAR*	name)
{
// no longer supported 
	return 0;
}


//____________________________________________________________
//
//	gds_...
//
//	The following routines define the gds__ style names for isc_ entrypoints.
//
//	Please note that these names - strictly speaking - are illegal C++
//	identifiers, and as such should probably be moved into a C file.
//

ISC_STATUS API_ROUTINE gds__close(ISC_STATUS* status_vector, const SCHAR* statement_name)
{
	return isc_close(status_vector, statement_name);
}

ISC_STATUS API_ROUTINE gds__declare(ISC_STATUS*	status_vector,
								const SCHAR*	statement_name,
								const SCHAR*	cursor_name)
{
	return isc_declare(status_vector, statement_name, cursor_name);
}

ISC_STATUS API_ROUTINE gds__describe(	ISC_STATUS*	status_vector,
									const SCHAR*	statement_name,
									SQLDA*	sqlda)
{
	return isc_describe(status_vector, statement_name, sqlda);
}

ISC_STATUS API_ROUTINE gds__describe_bind(ISC_STATUS*	status_vector,
									  const SCHAR*	statement_name,
									  SQLDA*	sqlda)
{
	return isc_describe_bind(status_vector, statement_name, sqlda);
}

ISC_STATUS API_ROUTINE gds__dsql_finish(HNDL* db_handle)
{
	return isc_dsql_finish(db_handle);
}

ISC_STATUS API_ROUTINE gds__execute(ISC_STATUS*	status_vector,
								FB_API_HANDLE*	tra_handle,
								const SCHAR*	statement_name,
								SQLDA*	sqlda)
{
	return isc_execute(status_vector, tra_handle, statement_name, sqlda);
}

ISC_STATUS API_ROUTINE gds__execute_immediate(ISC_STATUS*	status_vector,
										  FB_API_HANDLE*	db_handle,
										  FB_API_HANDLE*	tra_handle,
										  SSHORT*	sql_length,
										  const SCHAR*	sql)
{
	return isc_execute_immediate(	status_vector,
									db_handle,
									tra_handle,
									sql_length,
									sql);
}

#ifdef VMS
ISC_STATUS API_ROUTINE gds__execute_immediate_d(ISC_STATUS*	status_vector,
											SLONG*	db_handle,
											SLONG*	tra_handle,
											const SCHAR*	sql_string)
{
	return isc_execute_immediate_d(status_vector,
								   db_handle, tra_handle, sql_string);
}
#endif

ISC_STATUS API_ROUTINE gds__fetch(ISC_STATUS*	status_vector,
							  const SCHAR*	statement_name,
							  SQLDA*	sqlda)
{
	return isc_fetch(status_vector, statement_name, sqlda);
}

ISC_STATUS API_ROUTINE gds__fetch_a(ISC_STATUS*	status_vector,
								int*	sqlcode,
								const SCHAR*	statement_name,
								SQLDA*	sqlda)
{
	return isc_fetch_a(status_vector, sqlcode, statement_name, sqlda);
}

ISC_STATUS API_ROUTINE gds__open(ISC_STATUS*	status_vector,
							 FB_API_HANDLE*	tra_handle,
							 const SCHAR*		cursor_name,
							 SQLDA*		sqlda)
{
	return isc_open(status_vector, tra_handle, cursor_name, sqlda);
}

ISC_STATUS API_ROUTINE gds__prepare(ISC_STATUS*	status_vector,
								FB_API_HANDLE*	db_handle,
								FB_API_HANDLE*	tra_handle,
								const SCHAR*	statement_name,
								SSHORT*	sql_length,
								const SCHAR*	sql,
								SQLDA*	sqlda)
{
	return isc_prepare(	status_vector,
						db_handle,
						tra_handle,
						statement_name,
						sql_length,
						sql,
						sqlda);
}

#ifdef VMS
ISC_STATUS API_ROUTINE gds__prepare_d(ISC_STATUS* status_vector,
								  SLONG* db_handle,
								  SLONG* tra_handle,
								  SCHAR* statement_name,
								  SLONG* sql_desc, SQLDA* sqlda)
{
	return isc_prepare_d(status_vector,
						 db_handle,
						 tra_handle, statement_name, sql_desc, sqlda);
}
#endif

ISC_STATUS API_ROUTINE gds__to_sqlda(SQLDA* sqlda,
								 int number,
								 SCHAR* host_variable,
								 int host_variable_size,
								 SCHAR* host_variable_name)
{
	return isc_to_sqlda(sqlda,
						number,
						host_variable,
						host_variable_size, host_variable_name);
}


//____________________________________________________________
//
//	Helper functions for cleanup().
//
static void free_all_databases(dsql_dbb*& databasesL)
{
	while (databasesL) {
		dsql_dbb* database = databasesL;
		databasesL = database->dbb_next;
		gds__free(database);
	}
}

static void free_all_statements(stmt*& statementsL)
{
	while (statementsL) {
		stmt* statement = statementsL;
		statementsL = statement->stmt_next;
		gds__free(statement);
	}
}

static void free_all_names(dsql_name*& names)
{
	while (names) {
		dsql_name* name = names;
		names = name->name_next;
		gds__free(name);
	}
}


//____________________________________________________________
//
//		Cleanup handler to free all dynamically allocated memory.
//
static void cleanup(void* arg)
{
	if (!init_flag) {
		return;
	}

	init_flag = false;

	gds__free(UDSQL_error);
	UDSQL_error = NULL;

	free_all_databases(databases);
	free_all_statements(statements);
	free_all_names(statement_names);
	free_all_names(cursor_names);

	gds__unregister_cleanup(cleanup, 0);
}


//____________________________________________________________
//
//		the cleanup handler called when a database is closed
//
static void cleanup_database(FB_API_HANDLE* db_handle, void* dummy)
{
	if (!db_handle || !databases) {
		return;
	}

	// for each of the statements in this database, remove it
	// from the local list and from the hash table

	stmt** stmt_ptr = &statements;
	stmt* p;

	while (p = *stmt_ptr)
	{
		if (p->stmt_db_handle == *db_handle)
		{
			*stmt_ptr = p->stmt_next;
			if (p->stmt_stmt) {
				remove_name(p->stmt_stmt, &statement_names);
			}
			if (p->stmt_cursor) {
				remove_name(p->stmt_cursor, &cursor_names);
			}
			gds__free(p);
		}
		else
		{
			stmt_ptr = &p->stmt_next;
		}
	}

	dsql_dbb* dbb;

	for (dsql_dbb** dbb_ptr = &databases; dbb = *dbb_ptr; dbb_ptr = &dbb->dbb_next)
	{
		if (dbb->dbb_database_handle == *db_handle)
		{
			*dbb_ptr = dbb->dbb_next;
			gds__free(dbb);
			break;
		}
	}
}

//____________________________________________________________
//
//	An error returned has been trapped.  If the user specified
//	a status vector, return a status code.  Otherwise print the
//	error code(s) and abort.
//
static ISC_STATUS error(const std::exception& ex)
{
	if (UDSQL_error->dsql_user_status) {
		Firebird::stuff_exception(UDSQL_error->dsql_user_status, ex);
		return UDSQL_error->dsql_user_status[1];
	}

	Firebird::stuff_exception(UDSQL_error->dsql_status, ex);
	gds__print_status(UDSQL_error->dsql_status);

	exit(UDSQL_error->dsql_status[1]);
}


static ISC_STATUS error()
{
	if (UDSQL_error->dsql_user_status) {
		return UDSQL_error->dsql_user_status[1];
	}

	gds__print_status(UDSQL_error->dsql_status);

	exit(UDSQL_error->dsql_status[1]);
}

//____________________________________________________________
//
//	Post an error sequence to the status vector.  Since an error
//	sequence can, in theory, be arbitrarily lock, pull a cheap
//	trick to get the address of the argument vector.
//
//	this is a copy of the routine found in err.c with the
//	exception that it uses a different error block - one which
//	is local to the V3 DSQL routines...
//
static void error_post(ISC_STATUS status, ...)
{
	va_list	args;
	ISC_STATUS*	p;
	int		type;

/* Get the addresses of the argument vector and the status vector, and do
   word-wise copy. */

	va_start(args, status);
	p = UDSQL_error->dsql_status;

// Copy first argument 

	*p++ = isc_arg_gds;
	*p++ = status;

// Pick up remaining args 

	while (type = va_arg(args, int))
	{
		switch (*p++ = type) {
		case isc_arg_gds:
			*p++ = (ISC_STATUS) va_arg(args, ISC_STATUS);
			break;

		case isc_arg_number:
			*p++ = (ISC_STATUS) va_arg(args, SLONG);
			break;

		case isc_arg_vms:
		case isc_arg_unix:
		case isc_arg_win32:
			*p++ = va_arg(args, int);
			break;

		case isc_arg_string:
		case isc_arg_interpreted:
			*p++ = (ISC_STATUS)(IPTR) va_arg(args, TEXT *);
			break;

		case isc_arg_cstring:
			*p++ = (ISC_STATUS) va_arg(args, int);
			*p++ = (ISC_STATUS)(IPTR) va_arg(args, TEXT *);
			break;
		}
	}

	va_end(args);
	*p = isc_arg_end;

	// Give up whatever we were doing and return to the user.

	Firebird::status_exception::raise(UDSQL_error->dsql_status);
}


//____________________________________________________________
//
//	Initialize dynamic SQL.  This is called only once.
//
static void init(FB_API_HANDLE* db_handle)
{
	// If we haven't been initialized yet, do it now
	if (!init_flag)
	{
		UDSQL_error = (dsql_err_stblock*) gds__alloc((SLONG) sizeof(dsql_err_stblock));
		// FREE: by exit cleanup()
		if (!UDSQL_error) {		// NOMEM:
			return;				// Don't set the init_flag
		}
		init_flag = true;
		gds__register_cleanup(cleanup, 0);
	}

	if (!db_handle) {
		return;
	}

	dsql_dbb* dbb;
	for (dbb = databases; dbb; dbb = dbb->dbb_next) {
		if (dbb->dbb_database_handle == *db_handle) {
			return;
		}
	}

	dbb = (dsql_dbb*) gds__alloc((SLONG) sizeof(dsql_dbb));

	// FREE: by database exit handler cleanup_database()
	if (!dbb) {					// NOMEM
		return;					// Not a great error handler
	}

	dbb->dbb_next = databases;
	databases = dbb;
	dbb->dbb_database_handle = *db_handle;

	ISC_STATUS_ARRAY local_status;
	isc_database_cleanup(local_status, db_handle, cleanup_database, NULL);
}


static dsql_name* insert_name(const TEXT* symbol_name, dsql_name** list_ptr, stmt* stmt)
{
/**************************************
 *
 *	i n s e r t _ n a m e
 *
 **************************************
 *
 * Functional description
 * 	Add the name to the designated list.
 *
 **************************************/
	USHORT l = name_length(symbol_name);
	dsql_name* name = (dsql_name*) gds__alloc((SLONG) sizeof(dsql_name) + l);
// FREE: by exit handler cleanup() or database_cleanup() 
	if (!name)					// NOMEM: 
		error_post(isc_virmemexh, 0);
	name->name_stmt = stmt;
	name->name_length = l;
	TEXT* p = name->name_symbol;
	while (l--)
		*p++ = *symbol_name++;

	if (name->name_next = *list_ptr)
		name->name_next->name_prev = name;
	*list_ptr = name;
	name->name_prev = NULL;

	return name;
}


static dsql_name* lookup_name(const TEXT* name, dsql_name* list)
{
/**************************************
 *
 *	l o o k u p _ n a m e
 *
 **************************************
 *
 * Functional description
 * 	Look up the given name in the designated list.
 *
 **************************************/

	const USHORT l = name_length(name);
	for (; list; list = list->name_next) {
		if (scompare(name, l, list->name_symbol, list->name_length)) {
			break;
		}
	}

	return list;
}


static stmt* lookup_stmt(const TEXT* name, dsql_name* list, name_type type)
{
/**************************************
 *
 *	l o o k u p _ s t m t
 *
 **************************************
 *
 * Functional description
 * 	Look up the given statement in the designated list.
 *
 **************************************/
	dsql_name* found = lookup_name(name, list);

	if (found)
		return found->name_stmt;

	if (type == NAME_statement) {
		error_post(isc_dsql_error,
				   isc_arg_gds, isc_sqlerr, isc_arg_number, (SLONG) - 518,
				   isc_arg_gds, isc_dsql_request_err, 0);
	}
	else {
		error_post(isc_dsql_error,
				   isc_arg_gds, isc_sqlerr, isc_arg_number, (SLONG) - 504,
				   isc_arg_gds, isc_dsql_cursor_err,
				   isc_arg_gds, isc_dsql_cursor_not_found,
				   isc_arg_string, name, 0);
	}
	return NULL;
}


static USHORT name_length(const TEXT* name)
{
/**************************************
 *
 *	n a m e _ l e n g t h
 *
 **************************************
 *
 * Functional description
 *	Compute length of user supplied name.
 *
 **************************************/

	// CVC: Beware, another function that stops at the first blank!!!
	const TEXT* p;
	for (p = name; *p && *p != ' '; p++);

	return (USHORT) (p - name);
}


static void remove_name(dsql_name* name, dsql_name** list_ptr)
{
/**************************************
 *
 *	r e m o v e _ n a m e
 *
 **************************************
 *
 * Functional description
 * 	Remove a name from the designated list.
 *
 **************************************/

	if (name->name_next) {
		name->name_next->name_prev = name->name_prev;
	}

	if (name->name_prev) {
		name->name_prev->name_next = name->name_next;
	}
	else {
		*list_ptr = name->name_next;
	}

	gds__free(name);
}


// CVC: It seems I've read at least three versions of this routine.
static bool scompare(const SCHAR* string1,
					 USHORT length1,
					 const SCHAR* string2,
					 USHORT length2)
{
/**************************************
 *
 *	s c o m p a r e
 *
 **************************************
 *
 * Functional description
 *	Compare two strings case insensitive according to the C locale rules.
 *
 **************************************/

	if (length1 != length2) {
		return false;
	}

	while (length1--)
	{
		const SCHAR c1 = *string1++;
		const SCHAR c2 = *string2++;
		if (c1 != c2 && UPPER7(c1) != UPPER7(c2))
		{
			return false;
		}
	}

	return true;
}

