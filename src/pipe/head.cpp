/*
 *	PROGRAM:	Pipe Server
 *	MODULE:		head.c
 *	DESCRIPTION:	Pipe Server interface
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
 * Solaris x86 changes - Konstantin Kuznetsov, Neil McCalden
 *
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "EPSON" define
 *
 *
 * 2002-02-23 Sean Leyne - Code Cleanup, removed old M88K and NCR3000 port
 *
 */

 /* $Id: head.cpp,v 1.8 2002-07-05 15:00:21 skywalker Exp $ */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <stdlib.h>
#include "../jrd/common.h"

#include <stdarg.h>
#include <signal.h>
#include <errno.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "../jrd/ibase.h"
#include "../pipe/pipe.h"
#include "gen/codes.h"
#include "../jrd/license.h"
#include "../jrd/inf.h"
#include "../pipe/allp_proto.h"
#include "../pipe/head_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_i_proto.h"
#include "../remote/merge_proto.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef sparc
#ifdef SOLARIS
#include <fcntl.h>
#else
#include <vfork.h>
#endif
#endif

#ifdef SOLX86
#include <fcntl.h>
#endif

#if (defined DELTA || defined M88K || defined SCO_UNIX || defined SINIXZ)
#include <fcntl.h>
#endif

#ifdef UNIXWARE
#include <sys/fcntl.h>
#endif

#ifdef GDS_PIPE
#define BRIDGE
#endif

#include <sys/param.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>

#ifndef GDS_PIPE
#define GDS_PIPE	"bin/gds_pipe"
#define GDS_SERVER	"GDS_SERVER"
#else
#define GDS_SERVER	"GDS_BRIDGE"
#endif

#ifndef MAXPATHLEN
#define MAXPATHLEN	256
#endif

#ifndef GATEWAY
#define ESTABLISH_PIPES { \
	    if (!read_pipe || !write_pipe) \
	        { \
		user_status [0] = gds_arg_gds; \
		user_status [1] = gds_conn_lost;	/* Msg 328 Connection lost to pipe server */ \
		user_status [2] = gds_arg_end; \
		return user_status [1]; \
		}; \
	    }
#else
#define ESTABLISH_PIPES		read_pipe = (FILE*) rdb->rdb_pipes.cnct_read_pipe; write_pipe = (FILE*) rdb->rdb_pipes.cnct_write_pipe; connection = rdb;
#endif

#define statistics	stat

typedef void	(*PTR)();

static STATUS	check_response (STATUS *);
static void	event_handler (void);
static int	get_byte (void);
static int	get_byte_event (void);
static IPTR	get_handle (void);
static IPTR	get_handle_event (void);
static SLONG	get_word (void);
static SLONG	get_word_event (void);
static STATUS	handle_error (STATUS *, STATUS);
static RDB	init (STATUS *, P_OP, UCHAR *, USHORT, UCHAR *, USHORT);
static RTR	make_transaction (RDB, HANDLE);
static SSHORT	name_length (TEXT *);
static void	pipe_io_error (STATUS *, TEXT *, TEXT *, int);
static int	put_byte (int);
static IPTR	put_handle (IPTR);
static SLONG	put_word (SLONG);
static void	release_blob (RBL);
static STATUS	release_object (P_OP, HANDLE, STATUS *);
static void	release_request (RRQ);
static void	release_sql_request (RSR);
static void	release_transaction (RTR);
static STATUS	send_blob (STATUS *, RBL, USHORT, UCHAR *);

static FILE	*read_pipe = NULL,
		*write_pipe = NULL,
		*event_pipe = NULL;
static int	event_fd = -1;
static TEXT	error_text [1024];
static RDB	PSI_databases = NULL;

#ifdef GATEWAY
static RDB	connection = NULL;
#endif

#ifdef SOLARIS
/* NOTE: This code is cloned in foot.c */
/* On Solaris, getc&putc are thread-safe calls, which mean they do
 * a file lock/unlock for each character IO.  This is a performance
 * drain; however, the main reason for making the change is that
 * getc&putc don't work on (at least) Solaris 2.4 with the -mt CC option.
 * Our testing on Solaris revealed that the standard definitions
 * of putc/getc would obtain bogus EOF indicators - which would
 * cause one end of the pipe connection to terminate after a message.
 *
 * Note also that getc_unlocked & putc_unlocked are macros.
 *
 * The man page for putc_unlocked/getc_unlocked state that we must
 * do our own file locking to ensure that the usage is MT safe -
 * in our case, the pipe server connection is not threaded - and
 * as there is a unique pipe created for each client-server connection
 * there is no MT issue.
 *  15-September-1995 David Schnepper & Morgan Schweers
 */
#define	GETC_UNLOCKED		getc_unlocked
#define	PUTC_UNLOCKED		putc_unlocked
#endif

#ifndef	GETC_UNLOCKED
#define	GETC_UNLOCKED		getc
#define	PUTC_UNLOCKED		putc
#endif

#ifdef KILLER_SIGNALS
#define GET_BYTE		get_byte ()
#define GET_WORD		get_word ()
#define GET_HANDLE		get_handle ()
#define GET_BYTE_EVENT		get_byte_event ()
#define GET_WORD_EVENT		get_word_event ()
#define GET_HANDLE_EVENT	get_handle_event ()
#define PUT_BYTE(byte)		put_byte (byte)
#define PUT_WORD(word)		put_word ((SLONG) (word))
#define PUT_HANDLE(word)	put_handle ((IPTR) (word))

#else
#define GET_BYTE		GETC_UNLOCKED (read_pipe)
#define GET_WORD		getw (read_pipe)
#define GET_HANDLE		getw (read_pipe)
#define GET_BYTE_EVENT		GETC_UNLOCKED (event_pipe)
#define GET_WORD_EVENT		getw (event_pipe)
#define GET_HANDLE_EVENT	getw (event_pipe)
#define PUT_BYTE(byte)		PUTC_UNLOCKED (byte, write_pipe)
#define PUT_WORD(word)		putw (word, write_pipe)
#define PUT_HANDLE(word)	putw ((int)(word), write_pipe)
#endif

#define GET_STRING(length,ptr)	if (l = (length))\
	{p = (ptr); do *p++ = GET_BYTE; while (--l);}

#define PUT_STRING(length,ptr)	if (l = (length))\
	{p = (ptr); do PUT_BYTE (*p++); while (--l);}

#ifdef DEBUG
/* Define this to try and trace down problems with pipe server */
#define DEBUGMSG(x)	fprintf (stderr, "%s %d %s\n", __FILE__, __LINE__, x)
#endif

#ifndef DEBUGMSG
#define DEBUGMSG(x)	/* nothing */
#endif

/* Macros that generates code for all of the KILLER_SIGNALS get and
   put routines. */

#define GET_DATA_CODE(element,pipe,msg)	\
	union	{SCHAR c; SLONG l; IPTR h; SCHAR p [sizeof (IPTR)];} c; \
	int	i, n; \
	if (!pipe) \
	    return -1; \
	for (i = 0; i < sizeof (c.element);) \
	    { \
	    errno = 0; \
	    if ((n = GETC_UNLOCKED (pipe)) == EOF) \
		{ \
		if (SYSCALL_INTERRUPTED(errno)) \
		    { \
		    DEBUGMSG ("get_data: clearing EINTR"); \
		    clearerr (pipe); \
		    rewind (pipe); \
		    continue; \
		    } \
		else \
		    { \
		    DEBUGMSG ("get_data: closing due to error"); \
		    fclose (pipe); \
		    pipe = NULL; \
		    gds__log (msg, (int*) errno); \
		    return -1; \
		    } \
		} \
	    else \
		c.p [i++] = n; \
	    } \
	return c.element

#define PUT_DATA_CODE(data,element,pipe,msg)	\
	union	{SCHAR c; SLONG l; IPTR h; SCHAR p [sizeof (IPTR)];} c; \
	int	i, n; \
	c.element = data; \
	if (!pipe) \
	    return -1; \
	for (i = 0; i < sizeof (c.element);) \
	    { \
	    errno = 0; \
	    if ((n = PUTC_UNLOCKED (c.p [i], pipe)) == EOF) \
		{ \
		if (SYSCALL_INTERRUPTED(errno)) \
		    { \
		    DEBUGMSG ("put_data: clearing EINTR"); \
		    clearerr (pipe); \
		    rewind (pipe); \
		    continue; \
		    } \
		else \
		    { \
		    DEBUGMSG ("put_data: closing due to error"); \
		    fclose (pipe); \
		    pipe = NULL; \
		    gds__log (msg, (int*) errno); \
		    return -1; \
		    } \
		} \
	    else \
		i++; \
	    } \
	return c.element

#define RETURN_SUCCESS return SUCCESS;

#define CHECK_HANDLE(blk, type, error) if (!blk || ((BLK) blk)->blk_type != (SCHAR) type) \
	return handle_error (user_status, error)

#define NULL_CHECK(ptr, code) if (*ptr) return handle_error (user_status, code)

#ifdef SHLIB_DEFS
#define fcntl		(*_libgds_fcntl)
#define fdopen		(*_libgds_fdopen)
#define pipe		(*_libgds_pipe)
#define statistics	(*_libgds_stat)
#define dup		(*_libgds_dup)
#define execle		(*_libgds_execle)
#define execl		(*_libgds_execl)
#define putenv		(*_libgds_putenv)

extern int		fcntl();
extern FILE		*fdopen();
extern int		pipe();
extern int		statistics();
extern int		dup();
extern int		execle();
extern int		execl();
extern int		putenv();
#endif

#ifndef BRIDGE
#define GDS_ATTACH_DATABASE	PSI_attach_database
#define GDS_ATTACH_SERVICE	PSI_attach_service
#define GDS_BLOB_INFO		PSI_blob_info
#define GDS_CANCEL_BLOB		PSI_cancel_blob
#define GDS_CANCEL_EVENTS	PSI_cancel_events
#define GDS_CLOSE_BLOB		PSI_close_blob
#define GDS_COMMIT		PSI_commit_transaction
#define GDS_COMMIT_RETAINING	PSI_commit_retaining
#define GDS_COMPILE		PSI_compile_request
#define GDS_CREATE_BLOB		PSI_create_blob
#define GDS_CREATE_BLOB2	PSI_create_blob2
#define GDS_CREATE_DATABASE	PSI_create_database
#define GDS_DATABASE_INFO	PSI_database_info
#define GDS_DDL			PSI_ddl
#define GDS_DETACH		PSI_detach_database
#define GDS_DETACH_SERVICE	PSI_detach_service
#define GDS_DROP_DATABASE	PSI_drop_database
#define GDS_GET_SEGMENT		PSI_get_segment
#define GDS_GET_SLICE		PSI_get_slice
#define GDS_OPEN_BLOB		PSI_open_blob
#define GDS_OPEN_BLOB2		PSI_open_blob2
#define GDS_PREPARE		PSI_prepare_transaction
#define GDS_PUT_SEGMENT		PSI_put_segment
#define GDS_PUT_SLICE		PSI_put_slice
#define GDS_QUE_EVENTS		PSI_que_events
#define GDS_QUERY_SERVICE	PSI_query_service
#define GDS_RECEIVE		PSI_receive
#define GDS_RECONNECT		PSI_reconnect_transaction
#define GDS_RELEASE_REQUEST	PSI_release_request
#define GDS_REQUEST_INFO	PSI_request_info
#define GDS_ROLLBACK		PSI_rollback_transaction
#define GDS_SEEK_BLOB		PSI_seek_blob
#define GDS_SEND		PSI_send
#define GDS_START_AND_SEND	PSI_start_and_send
#define GDS_START		PSI_start_request
#define GDS_START_MULTIPLE	PSI_start_multiple
#define GDS_START_TRANSACTION	PSI_start_transaction
#define GDS_TRANSACT_REQUEST	PSI_transact_request
#define GDS_TRANSACTION_INFO	PSI_transaction_info
#define GDS_UNWIND		PSI_unwind_request

#define GDS_DSQL_ALLOCATE	PSI_allocate_statement
#define GDS_DSQL_EXECUTE	PSI_execute
#define GDS_DSQL_EXECUTE2	PSI_execute2
#define GDS_DSQL_EXECUTE_IMMED	PSI_execute_immediate
#define GDS_DSQL_EXECUTE_IMMED2	PSI_execute_immediate2
#define GDS_DSQL_FETCH		PSI_fetch
#define GDS_DSQL_FREE		PSI_free_statement
#define GDS_DSQL_INSERT		PSI_insert
#define GDS_DSQL_PREPARE	PSI_prepare
#define GDS_DSQL_SET_CURSOR	PSI_set_cursor_name
#define GDS_DSQL_SQL_INFO	PSI_sql_info
#else
#define GDS_ATTACH_DATABASE	PSI5_attach_database
#define GDS_BLOB_INFO		PSI5_blob_info
#define GDS_CANCEL_BLOB		PSI5_cancel_blob
#define GDS_CLOSE_BLOB		PSI5_close_blob
#define GDS_COMMIT		PSI5_commit_transaction
#define GDS_COMMIT_RETAINING	PSI5_commit_retaining
#define GDS_COMPILE		PSI5_compile_request
#define GDS_CREATE_BLOB		PSI5_create_blob
#define GDS_CREATE_BLOB2	PSI5_create_blob2
#define GDS_CREATE_DATABASE	PSI5_create_database
#define GDS_DATABASE_INFO	PSI5_database_info
#define GDS_DETACH		PSI5_detach_database
#define GDS_GET_SEGMENT		PSI5_get_segment
#define GDS_OPEN_BLOB		PSI5_open_blob
#define GDS_OPEN_BLOB2		PSI5_open_blob2
#define GDS_PREPARE		PSI5_prepare_transaction
#define GDS_PUT_SEGMENT		PSI5_put_segment
#define GDS_RECEIVE		PSI5_receive
#define GDS_RECONNECT		PSI5_reconnect_transaction
#define GDS_RELEASE_REQUEST	PSI5_release_request
#define GDS_REQUEST_INFO	PSI5_request_info
#define GDS_ROLLBACK		PSI5_rollback_transaction
#define GDS_SEEK_BLOB		PSI5_seek_blob
#define GDS_SEND		PSI5_send
#define GDS_START_AND_SEND	PSI5_start_and_send
#define GDS_START		PSI5_start_request
#define GDS_START_MULTIPLE	PSI5_start_multiple
#define GDS_START_TRANSACTION	PSI5_start_transaction
#define GDS_TRANSACTION_INFO	PSI5_transaction_info
#define GDS_UNWIND		PSI5_unwind_request
#define GDS_DDL			PSI5_ddl
#define GDS_QUE_EVENTS		PSI5_que_events
#define GDS_CANCEL_EVENTS	PSI5_cancel_events
#define GDS_GET_SLICE		PSI5_get_slice
#define GDS_PUT_SLICE		PSI5_put_slice

#define GDS_DSQL_ALLOCATE	PSI5_allocate_statement
#define GDS_DSQL_EXECUTE	PSI5_execute
#define GDS_DSQL_EXECUTE_IMMED	PSI5_execute_immediate
#define GDS_DSQL_FETCH		PSI5_fetch
#define GDS_DSQL_FREE		PSI5_free_statement
#define GDS_DSQL_PREPARE	PSI5_prepare
#define GDS_DSQL_SET_CURSOR	PSI5_set_cursor_name
#define GDS_DSQL_SQL_INFO	PSI5_sql_info
#endif

STATUS GDS_ATTACH_DATABASE (
    STATUS	*user_status,
    SSHORT	GDS_VAL (file_length),
    SCHAR	*file_name,
    RDB		*handle,
    SSHORT	GDS_VAL (dpb_length),
    SCHAR	*dpb)
{
/**************************************
 *
 *	g d s _ a t t a c h _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Attach to a grungy old database, corrupted by user data.
 *
 **************************************/
RDB	rdb;

NULL_CHECK (handle, gds_bad_db_handle);

if (!(rdb = init (user_status, op_attach, 
	(UCHAR*)file_name, GDS_VAL (file_length), (UCHAR*)dpb, GDS_VAL (dpb_length))))
    return user_status [1];

*handle = rdb;

RETURN_SUCCESS;
}

#ifndef BRIDGE
#ifndef GATEWAY
STATUS GDS_ATTACH_SERVICE (
    STATUS	*user_status,
    USHORT	service_length,
    TEXT	*service_name,
    RDB		*handle,
    USHORT	spb_length,
    SCHAR	*spb)
{
/**************************************
 *
 *	g d s _ a t t a c h _ s e r v i c e
 *
 **************************************
 *
 * Functional description
 *	Connect to an Interbase service.
 *
 **************************************/
RDB	rdb;

NULL_CHECK (handle, gds_bad_svc_handle);

if (!(rdb = init (user_status, op_attach_service, 
	(UCHAR*)service_name, service_length, (UCHAR*)spb, spb_length)))
    return user_status [1];

*handle = rdb;

RETURN_SUCCESS;
}
#endif
#endif

STATUS GDS_BLOB_INFO (
    STATUS	*user_status,
    RBL		*blob,
    SSHORT	GDS_VAL (item_length),
    UCHAR	*items,
    SSHORT	GDS_VAL (buffer_length),
    UCHAR	*buffer)
{
/**************************************
 *
 *	g d s _ b l o b _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Provide information on blob object.
 *
 **************************************/
RDB	rdb;
USHORT	l;
UCHAR	*p;

CHECK_HANDLE ((*blob), type_rbl, gds_bad_segstr_handle);
rdb = (*blob)->rbl_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_info_blob);
PUT_HANDLE ((*blob)->rbl_handle);
PUT_WORD (GDS_VAL (item_length));
PUT_STRING (GDS_VAL (item_length), items);
PUT_WORD (GDS_VAL (buffer_length));
if (check_response (user_status))
    return user_status [1];

GET_STRING (GET_WORD, buffer);
RETURN_SUCCESS;
}

STATUS GDS_CANCEL_BLOB (
    STATUS	*user_status,
    RBL		*blob_handle)
{
/**************************************
 *
 *	g d s _ c a n c e l _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Abort a partially completed blob.
 *
 **************************************/
RDB	rdb;
RBL	blob;
USHORT	l;
UCHAR	*p;

if (!(blob = *blob_handle))
    {
    *user_status++ = gds_arg_gds;
    *user_status = SUCCESS;
    return SUCCESS;
    }

CHECK_HANDLE (blob, type_rbl, gds_bad_segstr_handle);
rdb = blob->rbl_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

if (release_object (op_cancel_blob, blob->rbl_handle, user_status))
    return user_status [1];

release_blob (blob);
*blob_handle = NULL;

RETURN_SUCCESS;
}

STATUS GDS_CANCEL_EVENTS (
    STATUS	*user_status,
    RDB		*handle,
    SLONG	*id)
{
/**************************************
 *
 *	g d s _ $ c a n c e l _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *	Try to cancel an event.
 *
 **************************************/
RDB	rdb;
RBL	blob;
USHORT	l;
UCHAR	*p;

#ifdef V2_BRIDGE
return handle_error (user_status, gds_unavailable);

#else

rdb = *handle;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_cancel_events);
PUT_HANDLE (rdb->rdb_handle);
PUT_WORD (*id);

if (check_response (user_status))
    return user_status [1];

RETURN_SUCCESS;
#endif
}

STATUS GDS_CLOSE_BLOB (
    STATUS	*user_status,
    RBL		*blob_handle)
{
/**************************************
 *
 *	g d s _ c l o s e _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Abort a partially completed blob.
 *
 **************************************/
RDB	rdb;
RBL	blob;
USHORT	l;
UCHAR	*p;

blob = *blob_handle;
CHECK_HANDLE (blob, type_rbl, gds_bad_segstr_handle);
rdb = blob->rbl_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

#ifndef V2_BRIDGE
/* If a portion of the blob hasn't been sent to the pipe yet,
   do so now. */

if ((blob->rbl_flags & RBL_create) &&
    blob->rbl_ptr != blob->rbl_buffer)
    if (send_blob (user_status, blob, 0, NULL))
	return user_status [1];
#endif

if (release_object (op_close_blob, blob->rbl_handle, user_status))
    return user_status [1];

release_blob (blob);
*blob_handle = NULL;

RETURN_SUCCESS;
}

STATUS GDS_COMMIT (
    STATUS	*user_status,
    RTR		*rtr_handle)
{
/**************************************
 *
 *	g d s _ c o m m i t
 *
 **************************************
 *
 * Functional description
 *	Commit a transaction.
 *
 **************************************/
RDB	rdb;
RTR	transaction;
USHORT	l;
UCHAR	*p;

transaction = *rtr_handle;
CHECK_HANDLE (transaction, type_rtr, gds_bad_trans_handle);
rdb = (*rtr_handle)->rtr_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

if (release_object (op_commit, transaction->rtr_handle, user_status))
    return user_status [1];

release_transaction (transaction);
*rtr_handle = NULL;

RETURN_SUCCESS;
}

STATUS GDS_COMMIT_RETAINING (
    STATUS	*user_status,
    RTR		*rtr_handle)
{
/**************************************
 *
 *	g d s _ c o m m i t _ r e t a i n i n g
 *
 **************************************
 *
 * Functional description
 *	Prepare a transaction for commit.  First phase of a two
 *	phase commit.
 *
 **************************************/
RDB	rdb;
RTR	transaction;
USHORT	l;
UCHAR	*p;

#ifdef V2_BRIDGE
return handle_error (user_status, gds_unavailable);

#else
transaction = *rtr_handle;
CHECK_HANDLE (transaction, type_rtr, gds_bad_trans_handle);
rdb = (*rtr_handle)->rtr_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

if (release_object (op_commit_retaining, transaction->rtr_handle, user_status))
    return user_status [1];

RETURN_SUCCESS;
#endif
}

STATUS GDS_COMPILE (
    STATUS	*user_status,
    RDB		*db_handle,
    RRQ		*req_handle,
    SSHORT	GDS_VAL (blr_length),
    UCHAR	*blr)
{
/**************************************
 *
 *	g d s _ c o m p i l e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
RDB	rdb;
RRQ	request;
USHORT	i, count, max_msg;
USHORT	l;
UCHAR	*p;

/* Check and validate handles, etc. */

NULL_CHECK (req_handle, gds_bad_req_handle);
rdb = *db_handle;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_compile);
PUT_HANDLE (rdb->rdb_handle);
PUT_WORD (GDS_VAL (blr_length));
PUT_STRING (GDS_VAL (blr_length), blr);

if (check_response (user_status))
    return user_status [1];

/* Allocate request block */

*req_handle = request = (RRQ) ALLOC (type_rrq);
request->rrq_handle = (int*) GET_HANDLE;
request->rrq_rdb = rdb;
request->rrq_next = rdb->rdb_requests;
rdb->rdb_requests = request;

RETURN_SUCCESS;
}

#ifndef GATEWAY
STATUS GDS_CREATE_BLOB (
    STATUS	*user_status,
    RDB		*db_handle,
    RTR		*rtr_handle,
    RBL		*blob_handle,
    BID		blob_id)
{
/**************************************
 *
 *	g d s _ c r e a t e _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Create a brand new blob.
 *
 **************************************/
RDB	rdb;
RTR	transaction;
RBL	blob;
USHORT	l;
UCHAR	*p;

NULL_CHECK (blob_handle, gds_bad_segstr_handle);
rdb = *db_handle;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);
transaction = *rtr_handle;
CHECK_HANDLE (transaction, type_rtr, gds_bad_trans_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_create_blob);
PUT_HANDLE (rdb->rdb_handle);
PUT_HANDLE (transaction->rtr_handle);

if (check_response (user_status))
    return user_status [1];

*blob_handle = blob = (RBL) ALLOCV (type_rbl, BLOB_LENGTH);
blob->rbl_handle = (int*) GET_HANDLE;
blob->rbl_buffer_length = BLOB_LENGTH;
blob->rbl_rdb = rdb;
blob->rbl_rtr = transaction;
blob->rbl_ptr = blob->rbl_buffer;
blob->rbl_flags |= RBL_create;
blob->rbl_next = transaction->rtr_blobs;
transaction->rtr_blobs = blob;

blob_id->bid_relation_id = GET_WORD;
blob_id->bid_number = GET_WORD;

RETURN_SUCCESS;
}
#endif

STATUS GDS_CREATE_BLOB2 (
    STATUS	*user_status,
    RDB		*db_handle,
    RTR		*rtr_handle,
    RBL		*blob_handle,
    BID		blob_id,
    SSHORT	GDS_VAL (bpb_length),
    UCHAR	*bpb)
{
/**************************************
 *
 *	g d s _ c r e a t e _ b l o b 2
 *
 **************************************
 *
 * Functional description
 *	Create a brand new blob.
 *
 **************************************/
RDB	rdb;
RTR	transaction;
RBL	blob;
USHORT	l;
UCHAR	*p;

#ifdef V2_BRIDGE
return handle_error (user_status, gds_unavailable);

#else
NULL_CHECK (blob_handle, gds_bad_segstr_handle);
rdb = *db_handle;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);
transaction = *rtr_handle;
CHECK_HANDLE (transaction, type_rtr, gds_bad_trans_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_create_blob2);
PUT_HANDLE (rdb->rdb_handle);
PUT_HANDLE (transaction->rtr_handle);
PUT_WORD (GDS_VAL (bpb_length));
PUT_STRING (GDS_VAL (bpb_length), bpb);

if (check_response (user_status))
    return user_status [1];

*blob_handle = blob = (RBL) ALLOCV (type_rbl, BLOB_LENGTH);
blob->rbl_handle = (int*) GET_HANDLE;
blob->rbl_buffer_length = BLOB_LENGTH;
blob->rbl_rdb = rdb;
blob->rbl_rtr = transaction;
blob->rbl_ptr = blob->rbl_buffer;
blob->rbl_flags |= RBL_create;
blob->rbl_next = transaction->rtr_blobs;
transaction->rtr_blobs = blob;

blob_id->bid_relation_id = GET_WORD;
blob_id->bid_number = GET_WORD;

RETURN_SUCCESS;
#endif
}

#ifndef GATEWAY
STATUS GDS_CREATE_DATABASE (
    STATUS	*user_status,
    SSHORT	GDS_VAL (file_length),
    SCHAR	*file_name,
    RDB		*handle,
    SSHORT	GDS_VAL (dpb_length),
    SCHAR	*dpb,
    SSHORT	GDS_VAL (db_type))
{
/**************************************
 *
 *	g d s _ c r e a t e _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Create a nice, squeeky clean database, uncorrupted by user data.
 *
 **************************************/
RDB	rdb;
UCHAR	p;
USHORT	l;

NULL_CHECK (handle, gds_bad_db_handle);

if (!(l = GDS_VAL (file_length)))
    l = strlen (file_name);

if (!(rdb = init (user_status, op_create, (UCHAR*)file_name, l, 
	(UCHAR*)dpb, GDS_VAL (dpb_length))))
    return user_status [1];

*handle = rdb;

RETURN_SUCCESS;
}
#endif

STATUS GDS_DATABASE_INFO (
    STATUS	*user_status,
    RDB		*handle,
    SSHORT	GDS_VAL (item_length),
    UCHAR	*items,
    SSHORT	GDS_VAL (buffer_length),
    UCHAR	*buffer)
{
/**************************************
 *
 *	g d s _ d a t a b a s e _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Provide information on database object.
 *
 **************************************/
RDB	rdb;
USHORT	l;
UCHAR	*p, temp [1024], *temp_buffer;

rdb = *handle;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_info_database);
PUT_HANDLE (rdb->rdb_handle);
PUT_WORD (GDS_VAL (item_length));
PUT_STRING (GDS_VAL (item_length), items);
PUT_WORD (GDS_VAL (buffer_length));

if (check_response (user_status))
    return user_status [1];

if (buffer_length > sizeof (temp))
    temp_buffer = ALLP_alloc ((SLONG) buffer_length);
else
    temp_buffer = temp;

GET_STRING (GET_WORD, temp_buffer);
MERGE_database_info (temp_buffer, buffer, GDS_VAL (buffer_length),
	IMPLEMENTATION, 7, 1, (UCHAR*) GDS_VERSION, (UCHAR*) "", 0);

if (buffer_length > sizeof (temp))
    ALLP_free (temp_buffer);

RETURN_SUCCESS;
}

STATUS GDS_DDL (
    STATUS	*user_status,
    RDB		*db_handle,
    RTR		*rtr_handle,
    SSHORT	GDS_VAL (length),
    UCHAR	*msg)
{
/**************************************
 *
 *	g d s _ d d l
 *
 **************************************
 *
 * Functional description
 *	Pump thru a DDL call.
 *
 **************************************/
RTR	transaction;
RDB	rdb;
USHORT	l;
UCHAR	*p;

#ifdef V2_BRIDGE
return handle_error (user_status, gds_unavailable);

#else
rdb = *db_handle;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);
transaction = *rtr_handle;
CHECK_HANDLE (transaction, type_rtr, gds_bad_trans_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_ddl);
PUT_HANDLE (rdb->rdb_handle);
PUT_HANDLE (transaction->rtr_handle);
PUT_WORD (GDS_VAL (length));
PUT_STRING (GDS_VAL (length), msg);

if (check_response (user_status))
    return user_status [1];

RETURN_SUCCESS;
#endif
}

STATUS GDS_DETACH (
    STATUS	*user_status,
    RDB		*handle)
{
/**************************************
 *
 *	g d s _ d e t a c h
 *
 **************************************
 *
 * Functional description
 *	Close down a database.
 *
 **************************************/
RDB	rdb, *ptr;
RRQ	rrq;
USHORT	l;
UCHAR	*p;

rdb = *handle;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

if (release_object (op_detach, rdb->rdb_handle, user_status))
    return user_status [1];

while (rrq = rdb->rdb_requests)
    {
    if (rrq->rrq_user_handle)
	*(rrq->rrq_user_handle) = NULL;
    release_request (rrq);
    }

while (rdb->rdb_sql_requests)
    release_sql_request (rdb->rdb_sql_requests);

while (rdb->rdb_transactions)
    release_transaction (rdb->rdb_transactions);

#ifdef GATEWAY
/* Unhook the rdb block */

for (ptr = &PSI_databases; *ptr; ptr = &(*ptr)->rdb_next)
    if (*ptr == rdb)
	{
	*ptr = rdb->rdb_next;
	break;
	}

/* Close the pipes relating to this fork */

fclose (rdb->rdb_pipes.cnct_read_pipe);
fclose (rdb->rdb_pipes.cnct_write_pipe);

ALLP_release (rdb);

#else
ALLP_release ((BLK) rdb);
#endif

*handle = NULL;

RETURN_SUCCESS;
}

#ifndef BRIDGE
#ifndef GATEWAY
STATUS GDS_DETACH_SERVICE (
    STATUS	*user_status,
    RDB		*handle)
{
/**************************************
 *
 *	g d s _ d e t a c h _ s e r v i c e
 *
 **************************************
 *
 * Functional description
 *	Close down a service.
 *
 **************************************/
RDB	rdb;
USHORT	l;
UCHAR	*p;

rdb = *handle;
CHECK_HANDLE (rdb, type_rdb, gds_bad_svc_handle);

ESTABLISH_PIPES;

if (release_object (op_detach_service, rdb->rdb_handle, user_status))
    return user_status [1];

ALLP_release ((BLK) rdb);

*handle = NULL;

RETURN_SUCCESS;
}
#endif
#endif

#ifndef BRIDGE
#ifndef GATEWAY
STATUS GDS_DROP_DATABASE (
    STATUS	*user_status,
    RDB		*handle)
{
/**************************************
 *
 *	i s c _ d r o p _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Close down and purge a database.
 *
 **************************************/
RDB	rdb, *ptr;
RRQ	rrq;
USHORT	l;
UCHAR	*p;
STATUS  code;

rdb = *handle;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

code = release_object (op_drop_database, rdb->rdb_handle, user_status);
if (code && (code != gds_drdb_completed_with_errs))
    return user_status [1];

while (rrq = rdb->rdb_requests)
    {
    if (rrq->rrq_user_handle)
	*(rrq->rrq_user_handle) = NULL;
    release_request (rrq);
    }

while (rdb->rdb_sql_requests)
    release_sql_request (rdb->rdb_sql_requests);

while (rdb->rdb_transactions)
    release_transaction (rdb->rdb_transactions);

ALLP_release ((BLK) rdb);

*handle = NULL;

if (code)
    return code;

RETURN_SUCCESS;
}
#endif
#endif

STATUS GDS_DSQL_ALLOCATE (
    STATUS	*user_status,
    RDB		*db_handle,
    RSR		*stmt_handle)
{
/**************************************
 *
 *	d s q l _ a l l o c a t e _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *	Allocate a statement handle.
 *
 **************************************/
RDB	rdb;
RSR	statement;
USHORT	l;
UCHAR	*p;

/* Check and validate handles, etc. */

NULL_CHECK (stmt_handle, gds_bad_req_handle);
rdb = *db_handle;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_allocate_stmt);
PUT_HANDLE (rdb->rdb_handle);
if (check_response (user_status))
    return user_status [1];

/* Allocate SQL request block */

*stmt_handle = statement = (RSR) ALLOC (type_rsr);
statement->rsr_handle = (int*) GET_HANDLE;
statement->rsr_rdb = rdb;
statement->rsr_next = rdb->rdb_sql_requests;
rdb->rdb_sql_requests = statement;

RETURN_SUCCESS;
}

STATUS GDS_DSQL_EXECUTE (
    STATUS	*user_status,
    RTR		*rtr_handle,
    RSR		*stmt_handle,
    USHORT	blr_length,
    UCHAR	*blr,
    USHORT	msg_type,
    USHORT	msg_length,
    UCHAR	*msg)
{
/**************************************
 *
 *	d s q l _ e x e c u t e
 *
 **************************************
 *
 * Functional description
 *	Execute a dynamic SQL statement.
 *
 **************************************/
RDB	rdb;
RTR	transaction;
RSR	statement;
HANDLE	handle;
USHORT	l;
UCHAR	*p;

/* Check and validate handles, etc. */

if (transaction = *rtr_handle)
    CHECK_HANDLE (transaction, type_rtr, gds_bad_trans_handle);
statement = *stmt_handle;
CHECK_HANDLE (statement, type_rsr, gds_bad_req_handle);
rdb = statement->rsr_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_execute);
PUT_HANDLE ((transaction) ? transaction->rtr_handle : (HANDLE) NULL);
PUT_HANDLE (statement->rsr_handle);
PUT_WORD (blr_length);
PUT_STRING (blr_length, blr);
PUT_WORD (msg_type);
PUT_WORD (msg_length);
PUT_STRING (msg_length, msg);
(void) check_response (user_status);

handle = (int*) GET_HANDLE;
if (transaction && !handle)
    {
    release_transaction (transaction);
    *rtr_handle = NULL;
    }
else if (!transaction && handle)
    *rtr_handle = make_transaction (rdb, handle);

if (user_status [1])
    return user_status [1];

RETURN_SUCCESS;
}

#ifndef BRIDGE
STATUS GDS_DSQL_EXECUTE2 (
    STATUS	*user_status,
    RTR		*rtr_handle,
    RSR		*stmt_handle,
    USHORT	in_blr_length,
    UCHAR	*in_blr,
    USHORT	in_msg_type,
    USHORT	in_msg_length,
    UCHAR	*in_msg,
    USHORT	out_blr_length,
    UCHAR	*out_blr,
    USHORT	out_msg_type,
    USHORT	out_msg_length,
    UCHAR	*out_msg)
{
/**************************************
 *
 *	d s q l _ e x e c u t e 2
 *
 **************************************
 *
 * Functional description
 *	Execute a dynamic SQL statement.
 *
 **************************************/
RDB	rdb;
RTR	transaction;
RSR	statement;
HANDLE	handle;
USHORT	l;
UCHAR	*p;

/* Check and validate handles, etc. */

if (transaction = *rtr_handle)
    CHECK_HANDLE (transaction, type_rtr, gds_bad_trans_handle);
statement = *stmt_handle;
CHECK_HANDLE (statement, type_rsr, gds_bad_req_handle);
rdb = statement->rsr_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_execute2);
PUT_HANDLE ((transaction) ? transaction->rtr_handle : (HANDLE) NULL);
PUT_HANDLE (statement->rsr_handle);
PUT_WORD (in_blr_length);
PUT_STRING (in_blr_length, in_blr);
PUT_WORD (in_msg_type);
PUT_WORD (in_msg_length);
PUT_STRING (in_msg_length, in_msg);
PUT_WORD (out_blr_length);
PUT_STRING (out_blr_length, out_blr);
PUT_WORD (out_msg_type);
PUT_WORD (out_msg_length);
(void) check_response (user_status);

handle = (int*) GET_HANDLE;
if (transaction && !handle)
    {
    release_transaction (transaction);
    *rtr_handle = NULL;
    }
else if (!transaction && handle)
    *rtr_handle = make_transaction (rdb, handle);

if (user_status [1])
    return user_status [1];

GET_STRING (GET_WORD, out_msg);

RETURN_SUCCESS;
}
#endif

STATUS GDS_DSQL_EXECUTE_IMMED (
    STATUS	*user_status,
    RDB		*db_handle,
    RTR		*rtr_handle,
    USHORT	length,
    UCHAR	*string,
    USHORT	dialect,
    USHORT	blr_length,
    UCHAR	*blr,
    USHORT	msg_type,
    USHORT	msg_length,
    UCHAR	*msg)
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
RDB	rdb;
RTR	transaction;
HANDLE	handle;
USHORT	l;
UCHAR	*p;

/* Check and validate handles, etc. */

rdb = *db_handle;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);
if (transaction = *rtr_handle)
    CHECK_HANDLE (transaction, type_rtr, gds_bad_trans_handle);

ESTABLISH_PIPES;

if (!length)
    length = strlen ((char*)string);

PUT_BYTE (op_exec_immediate);
PUT_HANDLE (rdb->rdb_handle);
PUT_HANDLE ((transaction) ? transaction->rtr_handle : (HANDLE) NULL);
PUT_WORD (length);
PUT_STRING (length, string);
PUT_WORD (dialect);
PUT_WORD (blr_length);
PUT_STRING (blr_length, blr);
PUT_WORD (msg_type);
PUT_WORD (msg_length);
PUT_STRING (msg_length, msg);
(void) check_response (user_status);

handle = (int*) GET_HANDLE;
if (transaction && !handle)
    {
    release_transaction (transaction);
    *rtr_handle = NULL;
    }
else if (!transaction && handle)
    *rtr_handle = make_transaction (rdb, handle);

if (user_status [1])
    return user_status [1];

RETURN_SUCCESS;
}

#ifndef BRIDGE
STATUS GDS_DSQL_EXECUTE_IMMED2 (
    STATUS	*user_status,
    RDB		*db_handle,
    RTR		*rtr_handle,
    USHORT	length,
    UCHAR	*string,
    USHORT	dialect,
    USHORT	in_blr_length,
    UCHAR	*in_blr,
    USHORT	in_msg_type,
    USHORT	in_msg_length,
    UCHAR	*in_msg,
    USHORT	out_blr_length,
    UCHAR	*out_blr,
    USHORT	out_msg_type,
    USHORT	out_msg_length,
    UCHAR	*out_msg)
{
/**************************************
 *
 *	d s q l _ e x e c u t e _ i m m e d i a t e 2
 *
 **************************************
 *
 * Functional description
 *	Prepare and execute a statement.
 *
 **************************************/
RDB	rdb;
RTR	transaction;
HANDLE	handle;
USHORT	l;
UCHAR	*p;

/* Check and validate handles, etc. */

rdb = *db_handle;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);
if (transaction = *rtr_handle)
    CHECK_HANDLE (transaction, type_rtr, gds_bad_trans_handle);

ESTABLISH_PIPES;

if (!length)
    length = strlen ((char*) string);

PUT_BYTE (op_exec_immediate2);
PUT_HANDLE (rdb->rdb_handle);
PUT_HANDLE ((transaction) ? transaction->rtr_handle : (HANDLE) NULL);
PUT_WORD (length);
PUT_STRING (length, string);
PUT_WORD (dialect);
PUT_WORD (in_blr_length);
PUT_STRING (in_blr_length, in_blr);
PUT_WORD (in_msg_type);
PUT_WORD (in_msg_length);
PUT_STRING (in_msg_length, in_msg);
PUT_WORD (out_blr_length);
PUT_STRING (out_blr_length, out_blr);
PUT_WORD (out_msg_type);
PUT_WORD (out_msg_length);

(void) check_response (user_status);

handle = (int*) GET_HANDLE;
if (transaction && !handle)
    {
    release_transaction (transaction);
    *rtr_handle = NULL;
    }
else if (!transaction && handle)
    *rtr_handle = make_transaction (rdb, handle);

if (user_status [1])
    return user_status [1];

GET_STRING (GET_WORD, out_msg);

RETURN_SUCCESS;
}
#endif

STATUS GDS_DSQL_FETCH (
    STATUS	*user_status,
    RSR		*stmt_handle,
    USHORT	blr_length,
    UCHAR	*blr,
    USHORT	msg_type,
    USHORT	msg_length,
    UCHAR	*msg)
{
/**************************************
 *                  
 *	d s q l _ f e t c h
 *
 **************************************
 *
 * Functional description
 *	Fetch next record from a dynamic SQL cursor.
 *
 **************************************/
RDB	rdb;
RSR	statement;
USHORT	l;
UCHAR	*p;

/* Check and validate handles, etc. */

statement = *stmt_handle;
CHECK_HANDLE (statement, type_rsr, gds_bad_req_handle);
rdb = statement->rsr_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_fetch);
PUT_HANDLE (statement->rsr_handle);
PUT_WORD (blr_length);
PUT_STRING (blr_length, blr);
PUT_WORD (msg_type);
PUT_WORD (msg_length);
(void) check_response (user_status);

/* NOTE: for the dynamic blob cursor, likely 101 should also be
 * allowed here  - 101 is used for "more data available", eg: gds__segment
 */
if (GET_WORD == 100)
    return 100;
else if (user_status [1])
    return user_status [1];

GET_STRING (GET_WORD, msg);

RETURN_SUCCESS;
}

STATUS GDS_DSQL_FREE (
    STATUS	*user_status,
    RSR		*stmt_handle,
    USHORT	option)
{
/**************************************
 *
 *	d s q l _ f r e e _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *	Release request for a Dynamic SQL statement
 *
 **************************************/
RDB	rdb;
RSR	statement;

/* Check and validate handles, etc. */

statement = *stmt_handle;
CHECK_HANDLE (statement, type_rsr, gds_bad_req_handle);
rdb = statement->rsr_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_free_stmt);
PUT_HANDLE (statement->rsr_handle);
PUT_WORD (option);

if (check_response (user_status))
    return user_status [1];

statement->rsr_handle = (int*) GET_HANDLE;
if (!statement->rsr_handle)
    {
    release_sql_request (statement);
    *stmt_handle = NULL;
    }

RETURN_SUCCESS;
}

#ifndef BRIDGE
STATUS GDS_DSQL_INSERT (
    STATUS	*user_status,
    RSR		*stmt_handle,
    USHORT	blr_length,
    UCHAR	*blr,
    USHORT	msg_type,
    USHORT	msg_length,
    UCHAR	*msg)
{
/**************************************
 *                  
 *	d s q l _ i n s e r t
 *
 **************************************
 *
 * Functional description
 *	Insert next record into a dynamic SQL cursor.
 *
 **************************************/
RDB	rdb;
RSR	statement;
USHORT	l;
UCHAR	*p;

/* Check and validate handles, etc. */

statement = *stmt_handle;
CHECK_HANDLE (statement, type_rsr, gds_bad_req_handle);
rdb = statement->rsr_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_insert);
PUT_HANDLE (statement->rsr_handle);
PUT_WORD (blr_length);
PUT_STRING (blr_length, blr);
PUT_WORD (msg_type);
PUT_WORD (msg_length);
PUT_STRING (msg_length, msg);
if (check_response (user_status))
    return user_status [1];

RETURN_SUCCESS;
}
#endif

STATUS GDS_DSQL_PREPARE (
    STATUS	*user_status,
    RTR		*rtr_handle,
    RSR		*stmt_handle,	/* a remote statement block */
    USHORT	length,
    UCHAR	*string,
    USHORT	dialect,
    USHORT	item_length,
    UCHAR	*items,
    USHORT	buffer_length,
    UCHAR	*buffer)
{
/**************************************
 *
 *	d s q l _ p r e p a r e
 *
 **************************************
 *
 * Functional description
 *	Prepare a dynamic SQL statement for execution.
 *
 **************************************/
RDB	rdb;
RTR	transaction;
RSR	statement;
USHORT	l;
UCHAR	*p;

/* Check and validate handles, etc. */

statement = *stmt_handle;
CHECK_HANDLE (statement, type_rsr, gds_bad_req_handle);
rdb = statement->rsr_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);
if (transaction = *rtr_handle)
    CHECK_HANDLE (transaction, type_rtr, gds_bad_trans_handle);

ESTABLISH_PIPES;

if (!length)
    length = strlen ((char*) string);

PUT_BYTE (op_prepare_stmt);
PUT_HANDLE ((transaction) ? transaction->rtr_handle : (HANDLE) NULL);
PUT_HANDLE (statement->rsr_handle);
PUT_WORD (length);
PUT_STRING (length, string);
PUT_WORD (dialect);
PUT_WORD (item_length);
PUT_STRING (item_length, items);
PUT_WORD (buffer_length);

if (check_response (user_status))
    return user_status [1];

GET_STRING (GET_WORD, buffer);

RETURN_SUCCESS;
}

STATUS GDS_DSQL_SET_CURSOR (
    STATUS	*user_status,
    RSR		*stmt_handle,
    UCHAR	*cursor,
    USHORT	type)
{
/*****************************************
 *
 *	d s q l _ s e t _ c u r s o r
 *
 *****************************************
 *
 * Functional Description
 *	Set a cursor name for a dynamic request.
 *
 *****************************************/
RDB	rdb;
RSR	statement;
USHORT	cursor_length;
USHORT	l;
UCHAR	*p;

/* Check and validate handles, etc. */

statement = *stmt_handle;
CHECK_HANDLE (statement, type_rsr, gds_bad_req_handle);
rdb = statement->rsr_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

cursor_length = name_length ((TEXT*) cursor) + 1;

PUT_BYTE (op_set_cursor);
PUT_HANDLE (statement->rsr_handle);
PUT_WORD (cursor_length);
PUT_STRING (cursor_length, cursor);
PUT_WORD (type);
if (check_response (user_status))
    return user_status [1];

RETURN_SUCCESS;
}

STATUS GDS_DSQL_SQL_INFO (
    STATUS	*user_status,
    RSR		*stmt_handle,
    SSHORT	item_length,
    UCHAR	*items,
    SSHORT	buffer_length,
    UCHAR	*buffer)
{
/**************************************
 *
 *	d s q l _ s q l _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Provide information on sql object.
 *
 **************************************/
RDB	rdb;
RSR	statement;
USHORT	l;
UCHAR	*p;

/* Check and validate handles, etc. */

statement = *stmt_handle;
CHECK_HANDLE (statement, type_rsr, gds_bad_req_handle);
rdb = statement->rsr_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_info_sql);
PUT_HANDLE (statement->rsr_handle);
PUT_WORD (GDS_VAL (item_length));
PUT_STRING (GDS_VAL (item_length), items);
PUT_WORD (GDS_VAL (buffer_length));
if (check_response (user_status))
    return user_status [1];

GET_STRING (GET_WORD, buffer);

RETURN_SUCCESS;
}

STATUS GDS_GET_SEGMENT (
    STATUS	*user_status,
    RBL		*blob_handle,
    USHORT	*length,
    USHORT	GDS_VAL (buffer_length),
    UCHAR	*buffer)
{
/**************************************
 *
 *	g d s _ g e t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Get a segment from an open blob.
 *
 **************************************/
RDB	rdb;
RBL	blob;
USHORT	l;
UCHAR	*p;
STATUS	*v;
USHORT	state;

/* Sniff out handles, etc, and find the various blocks. */

blob = *blob_handle;
CHECK_HANDLE (blob, type_rbl, gds_bad_segstr_handle);
rdb = blob->rbl_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

#ifndef V2_BRIDGE
if (blob->rbl_flags & RBL_create)
#endif
    {
    PUT_BYTE (op_get_segment);
    PUT_HANDLE (blob->rbl_handle);
    PUT_WORD (GDS_VAL (buffer_length));

    /* pipe protocol will return a segment buffer regardless of
     * the result of the operation.
     * This is to handle gds__segment properly.
     */
    (void) check_response (user_status);

    *length = GET_WORD;
    GET_STRING (*length, buffer);

    return user_status [1];
    }

#ifndef V2_BRIDGE
/* Ask for a 1K chunk of blob and fill segment requests from it until its time
   to get the next section.  In other words, get a bunch, pass it out piece by
   piece, then when there isn't enough left, ask for more. */

/* set up the status vector for the calls we're going to fake */

v = user_status;
*v++ = gds_arg_gds;
v [0] = 0;
v [1] = gds_arg_end;
*length = 0;

/* if we're already done, stop now */

if (blob->rbl_flags & RBL_eof)
    {
    *v = gds_segstr_eof;
    return user_status [1];
    }

/* Here's the loop, passing out data from our basket & refilling it.
   Our buffer (described by the structure blob) is counted strings 
   <count word> <string> <count word> <string>... */

while (TRUE)
    {
    /* If there's data to be given away, give some away (p points to the local data) */

    if (blob->rbl_length)
	{
	p = blob->rbl_ptr;              

	/* If there was a fragment left over last time use it */

	if (l = blob->rbl_fragment_length)
 	    blob->rbl_fragment_length = 0;   
	
	/* otherwise pick up the count word as the length, & decrement the local length */

	else
	    {                             
	    l = *p++;
	    l += *p++ << 8;
	    blob->rbl_length -= 2;        
	    }

	/* Now check that what we've got fits. 
	   If not, set up the fragment pointer and set the status vector */

	if (l > buffer_length)           
	    {
	    blob->rbl_fragment_length = l - buffer_length;  
	    l = buffer_length;
	    *v = gds_segment;                               
	    }

	/* and, just for yucks, see if we're exactly using up the fragment
	   part of a previous incomplete read - if so mark this as an incomplete read */

	if (l == buffer_length && 
	    l == blob->rbl_length && 
	    (blob->rbl_flags & RBL_segment))
	    *v = gds_segment;
	
    	/* finally set up the return length, decrement the current length,
	   copy the data, and indicate where to start next time. */ 

	*length += l; 
	blob->rbl_length -= l;  
	buffer_length -= l;

	if (l)
	    do *buffer++ = *p++; while (--l);    

	blob->rbl_ptr = p;             

	/* return if we've filled up the caller's buffer, or completed a segment */

	if (!buffer_length ||
	    blob->rbl_length ||
	    !(blob->rbl_flags & RBL_segment))
	    break;
	}

    /* We're done with buffer.  If this was the last, we're done */

    if (blob->rbl_flags & RBL_eof_pending)
	{
	blob->rbl_flags |= RBL_eof;
	*v = gds_segstr_eof;
	break;
	}

    /* We need more data.  Ask for it politely */

    PUT_BYTE (op_get_segment_buf);
    PUT_HANDLE (blob->rbl_handle);
    PUT_WORD (blob->rbl_buffer_length);

    /* Note that if gds__segment needs to be returned it will be
     * sent via the STATE value - and the status vector will reflect SUCCESS
     */
    if (check_response (user_status))
	return user_status [1];

    blob->rbl_length = GET_WORD;
    GET_STRING (blob->rbl_length, blob->rbl_buffer);
    state = GET_WORD;
    blob->rbl_ptr = blob->rbl_buffer;
    blob->rbl_flags &= ~RBL_segment;
    if (state == 1)
	blob->rbl_flags |= RBL_segment;
    else if (state == 2)
	blob->rbl_flags |= RBL_eof_pending;
    }

return user_status [1];
#endif
}

STATUS GDS_GET_SLICE (
    STATUS	*user_status,
    RDB		*db_handle,
    RTR		*tra_handle,
    BID		array_id,
    USHORT	GDS_VAL (sdl_length),
    UCHAR	*sdl,
    USHORT	GDS_VAL (param_length),
    UCHAR	*param,
    SLONG	GDS_VAL (slice_length),
    UCHAR	*slice,
    SLONG	*return_length)
{
/**************************************
 *
 *	g d s _ $ g e t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *	Snatch a slice of an array.
 *
 **************************************/
RDB	rdb;
RTR	transaction;
ULONG	l, length;
UCHAR	*p;

#ifdef V2_BRIDGE
return handle_error (user_status, gds_unavailable);

#else
rdb = *db_handle;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);
transaction = *tra_handle;
CHECK_HANDLE (transaction, type_rtr, gds_bad_trans_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_get_slice);
PUT_HANDLE (rdb->rdb_handle);
PUT_HANDLE (transaction->rtr_handle);
PUT_WORD (array_id->bid_relation_id);
PUT_WORD (array_id->bid_number);
PUT_WORD (GDS_VAL (sdl_length));
PUT_STRING (GDS_VAL (sdl_length), sdl);
PUT_WORD (GDS_VAL (param_length));
PUT_STRING (GDS_VAL (param_length), param);
PUT_WORD (GDS_VAL (slice_length));

if (check_response (user_status))
    return user_status [1];

length = GET_WORD;
GET_STRING (GDS_VAL (slice_length), slice);

if (return_length)
    *return_length = length;

RETURN_SUCCESS;
#endif
}

#ifndef GATEWAY
STATUS GDS_OPEN_BLOB (
    STATUS	*user_status,
    RDB		*db_handle,
    RTR		*rtr_handle,
    RBL		*blob_handle,
    BID		blob_id)
{
/**************************************
 *
 *	g d s _ o p e n _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Open an existing blob.
 *
 **************************************/
RDB	rdb;
RTR	transaction;
RBL	blob;
USHORT	l;
UCHAR	*p;

NULL_CHECK (blob_handle, gds_bad_segstr_handle);
rdb = *db_handle;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);
transaction = *rtr_handle;
CHECK_HANDLE (transaction, type_rtr, gds_bad_trans_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_open_blob);
PUT_HANDLE (rdb->rdb_handle);
PUT_HANDLE (transaction->rtr_handle);
PUT_WORD (blob_id->bid_relation_id);
PUT_WORD (blob_id->bid_number);

if (check_response (user_status))
    return user_status [1];

*blob_handle = blob = (RBL) ALLOCV (type_rbl, BLOB_LENGTH);
blob->rbl_handle = (int*) GET_HANDLE;
blob->rbl_buffer_length = BLOB_LENGTH;
blob->rbl_rdb = rdb;
blob->rbl_rtr = transaction;
blob->rbl_ptr = blob->rbl_buffer;
blob->rbl_next = transaction->rtr_blobs;
transaction->rtr_blobs = blob;

RETURN_SUCCESS;
}
#endif

STATUS GDS_OPEN_BLOB2 (
    STATUS	*user_status,
    RDB		*db_handle,
    RTR		*rtr_handle,
    RBL		*blob_handle,
    BID		blob_id,
    SSHORT	GDS_VAL (bpb_length),
    UCHAR	*bpb)
{
/**************************************
 *
 *	g d s _ o p e n _ b l o b 2
 *
 **************************************
 *
 * Functional description
 *	Open an existing blob.
 *
 **************************************/
RDB	rdb;
RTR	transaction;
RBL	blob;
USHORT	l;
UCHAR	*p;

#ifdef V2_BRIDGE
return handle_error (user_status, gds_unavailable);

#else
NULL_CHECK (blob_handle, gds_bad_segstr_handle);
rdb = *db_handle;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);
transaction = *rtr_handle;
CHECK_HANDLE (transaction, type_rtr, gds_bad_trans_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_open_blob2);
PUT_HANDLE (rdb->rdb_handle);
PUT_HANDLE (transaction->rtr_handle);
PUT_WORD (blob_id->bid_relation_id);
PUT_WORD (blob_id->bid_number);
PUT_WORD (GDS_VAL (bpb_length));
PUT_STRING (GDS_VAL (bpb_length), bpb);

if (check_response (user_status))
    return user_status [1];

*blob_handle = blob = (RBL) ALLOCV (type_rbl, BLOB_LENGTH);
blob->rbl_handle = (int*) GET_HANDLE;
blob->rbl_buffer_length = BLOB_LENGTH;
blob->rbl_rdb = rdb;
blob->rbl_rtr = transaction;
blob->rbl_ptr = blob->rbl_buffer;
blob->rbl_next = transaction->rtr_blobs;
transaction->rtr_blobs = blob;

RETURN_SUCCESS;
#endif
}

STATUS GDS_PREPARE (
    STATUS	*user_status,
    RTR		*rtr_handle,
    USHORT	GDS_VAL (buffer_length),
    UCHAR	*buffer)
{
/**************************************
 *
 *	g d s _ p r e p a r e
 *
 **************************************
 *
 * Functional description
 *	Prepare a transaction for commit.  First phase of a two
 *	phase commit.
 *
 **************************************/
RDB	rdb;
RTR	transaction;
USHORT	l;
UCHAR	*p;

transaction = *rtr_handle;
CHECK_HANDLE (transaction, type_rtr, gds_bad_trans_handle);
rdb = (*rtr_handle)->rtr_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

#ifdef V2_BRIDGE
PUT_BYTE (op_prepare);
PUT_HANDLE (transaction->rtr_handle);
#else
PUT_BYTE (op_prepare2);
PUT_HANDLE (transaction->rtr_handle);
PUT_WORD (GDS_VAL (buffer_length));
PUT_STRING (GDS_VAL (buffer_length), buffer);
#endif

return check_response (user_status);
}

STATUS GDS_PUT_SEGMENT (
    STATUS	*user_status,
    RBL		*blob_handle,
    USHORT	GDS_VAL (buffer_length),
    UCHAR	*buffer)
{
/**************************************
 *
 *	g d s _ p u t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Send a segment of data to an open blob.
 *
 **************************************/
RDB	rdb;
RBL	blob;
USHORT	l;
UCHAR	*p;

/* Sniff out handles, etc, and find the various blocks. */

blob = *blob_handle;
CHECK_HANDLE (blob, type_rbl, gds_bad_segstr_handle);
rdb = blob->rbl_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

#ifndef V2_BRIDGE
if (!(blob->rbl_flags & RBL_create))
#endif
    {
    PUT_BYTE (op_put_segment);
    PUT_HANDLE (blob->rbl_handle);
    PUT_WORD (GDS_VAL (buffer_length));
    PUT_STRING (GDS_VAL (buffer_length), buffer);

    if (check_response (user_status))
	return user_status [1];

    RETURN_SUCCESS;
    }

#ifndef V2_BRIDGE
/* set up the status vector for the calls we're going to fake */

user_status [0] = gds_arg_gds;
user_status [1] = 0;
user_status [2] = gds_arg_end;

/* If the buffer is too full to even hold the length of the incoming
   segment, flush out the old buffer.  If the segment is too large
   to fit into the blob buffer, just send it as a single segment. */

p = blob->rbl_ptr;
l = blob->rbl_buffer_length - (p - blob->rbl_buffer);

if ((ULONG) buffer_length + 2 > l)
    {
    if (blob->rbl_ptr > blob->rbl_buffer)
	if (send_blob (user_status, blob, 0, NULL))
	    return user_status [1];
    if ((ULONG) buffer_length + 2 > blob->rbl_buffer_length)
	return send_blob (user_status, blob, buffer_length, buffer);
    p = blob->rbl_buffer;
    }

/* Move segment length and data into blob buffer */

*p++ = buffer_length;
*p++ = buffer_length >> 8;

if (buffer_length)
    do *p++ = *buffer++; while (--buffer_length);

blob->rbl_ptr = p;

RETURN_SUCCESS;
#endif
}

STATUS GDS_PUT_SLICE (
    STATUS	*user_status,
    RDB		*db_handle,
    RTR		*tra_handle,
    BID		array_id,
    USHORT	GDS_VAL (sdl_length),
    UCHAR	*sdl,
    USHORT	GDS_VAL (param_length),
    UCHAR	*param,
    SLONG	GDS_VAL (slice_length),
    UCHAR	*slice)
{
/**************************************
 *
 *	g d s _ $ p u t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *	Insert a slice of an array.
 *
 **************************************/
RDB	rdb;
RTR	transaction;
ULONG	l, length;
UCHAR	*p;

#ifdef V2_BRIDGE
return handle_error (user_status, gds_unavailable);

#else
rdb = *db_handle;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);
transaction = *tra_handle;
CHECK_HANDLE (transaction, type_rtr, gds_bad_trans_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_put_slice);
PUT_HANDLE (rdb->rdb_handle);
PUT_HANDLE (transaction->rtr_handle);
PUT_WORD (array_id->bid_relation_id);
PUT_WORD (array_id->bid_number);
PUT_WORD (GDS_VAL (sdl_length));
PUT_STRING (GDS_VAL (sdl_length), sdl);
PUT_WORD (GDS_VAL (param_length));
PUT_STRING (GDS_VAL (param_length), param);
PUT_WORD (GDS_VAL (slice_length));
PUT_STRING (GDS_VAL (slice_length), slice);

if (check_response (user_status))
    return user_status [1];

array_id->bid_relation_id = GET_WORD;
array_id->bid_number = GET_WORD;

RETURN_SUCCESS;
#endif
}

#ifndef GATEWAY
STATUS GDS_QUE_EVENTS (
    STATUS	*user_status,
    RDB		*handle,
    SLONG	*id,
    USHORT	GDS_VAL (length),
    UCHAR	*events,
    void	(GDS_VAL (*ast))(),
    void	*arg)
{
/**************************************
 *
 *	g d s _ $ q u e _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *	Que request for event notification.
 *
 **************************************/
RDB	rdb;
USHORT	l;
UCHAR	*p;

#ifdef V2_BRIDGE
return handle_error (user_status, gds_unavailable);

#else
/* If the event pipe is not fully active, finish initialization */

#ifdef FNDELAY
if (!event_pipe)
    {
    if (fcntl (event_fd, F_SETFL, FNDELAY) == -1)
	{
	pipe_io_error (user_status, "fcntl", "event pipe", errno);
	return user_status [1];
	}
    if (!(event_pipe = fdopen (event_fd, "r")))
	{
	pipe_io_error (user_status, "fdopen", "event pipe", errno);
	return user_status [1];
	}
    ISC_signal (SIGUSR1, event_handler, event_pipe);
    }
#endif

/* Check and validate handles, etc. */

rdb = *handle;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;
PUT_BYTE (op_que_events);
PUT_HANDLE (rdb->rdb_handle);
PUT_HANDLE (ast);
PUT_HANDLE (arg);
PUT_WORD (GDS_VAL (length));
PUT_STRING (GDS_VAL (length), events);

if (check_response (user_status))
    return user_status [1];

/* Allocate request block */

*id = GET_WORD;

return SUCCESS;
#endif
}
#endif

#ifndef BRIDGE
#ifndef GATEWAY
STATUS GDS_QUERY_SERVICE (
    STATUS	*user_status,
    RDB		*service,
    USHORT	send_item_length,
    SCHAR	*send_items,
    USHORT	recv_item_length,
    SCHAR	*recv_items,
    USHORT	buffer_length,
    SCHAR	*buffer)
{
/**************************************
 *
 *	g d s _ q u e r y _ s e r v i c e
 *
 **************************************
 *
 * Functional description
 *	Provide information on service object.
 *
 **************************************/
RDB	rdb;
USHORT	l;
UCHAR	*p;

rdb = *service;
CHECK_HANDLE (rdb, type_rdb, gds_bad_svc_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_info_service);
PUT_HANDLE (rdb->rdb_handle);
PUT_WORD (send_item_length);
PUT_STRING (send_item_length, (UCHAR*)send_items);
PUT_WORD (recv_item_length);
PUT_STRING (recv_item_length, (UCHAR*)recv_items);
PUT_WORD (buffer_length);
if (check_response (user_status))
    return user_status [1];

GET_STRING (GET_WORD, (UCHAR*)buffer);

RETURN_SUCCESS;
}
#endif
#endif

STATUS GDS_RECEIVE (
    STATUS	*user_status,
    RRQ		*req_handle,
    SSHORT	GDS_VAL (msg_type),
    SSHORT	GDS_VAL (msg_length),
    UCHAR	*msg,
    SSHORT	GDS_VAL (level)
#ifdef SCROLLABLE_CURSORS
    , 
    USHORT	GDS_VAL (direction), 
    ULONG	GDS_VAL (offset)
#endif
    )
{
/**************************************
 *
 *	g d s _ r e c e i v e
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
RRQ	request;
RTR	transaction;
RDB	rdb;
UCHAR	*p;
USHORT	l;

request = *req_handle;
CHECK_HANDLE (request, type_rrq, gds_bad_req_handle);
rdb = request->rrq_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_receive);
PUT_HANDLE (request->rrq_handle);
PUT_WORD (GDS_VAL (msg_type));
PUT_WORD (GDS_VAL (msg_length));
PUT_WORD (GDS_VAL (level));
#ifdef SCROLLABLE_CURSORS
PUT_WORD (GDS_VAL (direction));
PUT_WORD (GDS_VAL (offset));
#endif

if (check_response (user_status))
    return user_status [1];

GET_STRING (GDS_VAL (msg_length), msg);

RETURN_SUCCESS;
}

STATUS GDS_RECONNECT (
    STATUS	*user_status,
    RDB		*db_handle,
    RTR		*rtr_handle,
    SSHORT	GDS_VAL (length),
    UCHAR	*id)
{
/**************************************
 *
 *	g d s _ r e c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Connect to a transaction in limbo.
 *
 **************************************/
RDB	rdb;
RTR	transaction;
STATUS	*temp, l;
UCHAR	*p;

NULL_CHECK (rtr_handle, gds_bad_trans_handle);
rdb = *db_handle;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_reconnect);
PUT_HANDLE (rdb->rdb_handle);

PUT_WORD (l = GDS_VAL (length));
PUT_STRING (l, id);

if (check_response (user_status))
    return user_status [1];

*rtr_handle = make_transaction (rdb, (int*) GET_HANDLE);

RETURN_SUCCESS;
}

STATUS GDS_RELEASE_REQUEST (
    STATUS	*user_status,
    RRQ		*req_handle)
{
/**************************************
 *
 *	g d s _ r e l e a s e _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Release a request.
 *
 **************************************/
RDB	rdb;
RRQ	request;
USHORT	l;
UCHAR	*p;

request = *req_handle;
CHECK_HANDLE (request, type_rrq, gds_bad_req_handle);
rdb = request->rrq_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

if (release_object (op_release, request->rrq_handle, user_status))
    return user_status [1];

release_request (request);
*req_handle = NULL;

RETURN_SUCCESS;
}

STATUS GDS_REQUEST_INFO (
    STATUS	*user_status,
    RRQ		*request,
    USHORT	GDS_VAL (level),
    SSHORT	GDS_VAL (item_length),
    UCHAR	*items,
    SSHORT	GDS_VAL (buffer_length),
    UCHAR	*buffer)
{
/**************************************
 *
 *	g d s _ r e q u e s t _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Provide information on request object.
 *
 **************************************/
RDB	rdb;
USHORT	l;
UCHAR	*p;

CHECK_HANDLE ((*request), type_rrq, gds_bad_req_handle);
rdb = (*request)->rrq_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_info_request);
PUT_HANDLE ((*request)->rrq_handle);
PUT_WORD (GDS_VAL (level));			/* Incarnation */
PUT_WORD (GDS_VAL (item_length));
PUT_STRING (GDS_VAL (item_length), items);
PUT_WORD (GDS_VAL (buffer_length));

if (check_response (user_status))
    return user_status [1];

GET_STRING (GET_WORD, buffer);
RETURN_SUCCESS;
}

STATUS GDS_ROLLBACK (
    STATUS	*user_status,
    RTR		*rtr_handle)
{
/**************************************
 *
 *	g d s _ r o l l b a c k
 *
 **************************************
 *
 * Functional description
 *	Abort a transaction.
 *
 **************************************/
RDB	rdb;
RTR	transaction;
USHORT	l;
UCHAR	*p;

transaction = *rtr_handle;
CHECK_HANDLE (transaction, type_rtr, gds_bad_trans_handle);
rdb = (*rtr_handle)->rtr_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

if (release_object (op_rollback, transaction->rtr_handle, user_status))
    return user_status [1];

release_transaction (transaction);
*rtr_handle = NULL;

RETURN_SUCCESS;
}

STATUS GDS_SEEK_BLOB (
    STATUS	*user_status,
    RBL		*blob_handle,
    SSHORT	GDS_VAL (mode),
    SLONG	GDS_VAL (offset),
    SLONG	*result)
{
/**************************************
 *
 *	g d s _ $ s e e k _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Try to seek a blob.
 *
 **************************************/
RDB	rdb;
RBL	blob;
USHORT	l;
UCHAR	*p;

blob = *blob_handle;
CHECK_HANDLE (blob, type_rbl, gds_bad_segstr_handle);
rdb = blob->rbl_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_seek_blob);
PUT_HANDLE (blob->rbl_handle);
PUT_WORD (GDS_VAL (mode));
PUT_WORD (GDS_VAL (offset));

if (check_response (user_status))
    return user_status [1];

*result = GET_WORD;

RETURN_SUCCESS;
}

STATUS GDS_SEND (
    STATUS	*user_status,
    RRQ		*req_handle,
    SSHORT	GDS_VAL (msg_type),
    SSHORT	GDS_VAL (msg_length),
    UCHAR	*msg,
    SSHORT	GDS_VAL (level))
{
/**************************************
 *
 *	g d s _ s e n d
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
RRQ	request;
RTR	transaction;
RDB	rdb;
USHORT	l;
UCHAR	*p;

request = *req_handle;
CHECK_HANDLE (request, type_rrq, gds_bad_req_handle);
rdb = request->rrq_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_send);
PUT_HANDLE (request->rrq_handle);
PUT_WORD (GDS_VAL (msg_type));
PUT_WORD (GDS_VAL (msg_length));
PUT_STRING (GDS_VAL (msg_length), msg);
PUT_WORD (GDS_VAL (level));

if (check_response (user_status))
    return user_status [1];

RETURN_SUCCESS;
}

STATUS GDS_START_AND_SEND (
    STATUS	*user_status,
    RRQ		*req_handle,
    RTR		*rtr_handle,
    SSHORT	GDS_VAL (msg_type),
    SSHORT	GDS_VAL (msg_length),
    UCHAR	*msg,
    SSHORT	GDS_VAL (level))
{
/**************************************
 *
 *	g d s _ s t a r t _ a n d _ s e n d
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
RRQ	request;
RTR	transaction;
RDB	rdb;
USHORT	l;
UCHAR	*p;

request = *req_handle;
CHECK_HANDLE (request, type_rrq, gds_bad_req_handle);
transaction = *rtr_handle;
CHECK_HANDLE (transaction, type_rtr, gds_bad_trans_handle);
rdb = request->rrq_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_start_and_send);
PUT_HANDLE (request->rrq_handle);
PUT_HANDLE (transaction->rtr_handle);
PUT_WORD (GDS_VAL (msg_type));
PUT_WORD (GDS_VAL (msg_length));
PUT_STRING (GDS_VAL (msg_length), msg);
PUT_WORD (GDS_VAL (level));

if (check_response (user_status))
    return user_status [1];

RETURN_SUCCESS;
}

STATUS GDS_START (
    STATUS		*user_status,
    register RRQ	*req_handle,
    register RTR	*rtr_handle,
    SSHORT		GDS_VAL (level))
{
/**************************************
 *
 *	g d s _ s t a r t
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
RRQ	request;
RTR	transaction;
RDB	rdb;
USHORT	l;
UCHAR	*p;

request = *req_handle;
CHECK_HANDLE (request, type_rrq, gds_bad_req_handle);
transaction = *rtr_handle;
CHECK_HANDLE (transaction, type_rtr, gds_bad_trans_handle);
rdb = request->rrq_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_start);
PUT_HANDLE (request->rrq_handle);
PUT_HANDLE (transaction->rtr_handle);
PUT_WORD (GDS_VAL (level));

if (check_response (user_status))
    return user_status [1];

RETURN_SUCCESS;
}

STATUS GDS_START_MULTIPLE (
    STATUS	*user_status,
    RTR		*rtr_handle,
    SSHORT	GDS_VAL (count),
    int		**vector)
{
/**************************************
 *
 *	g d s _ s t a r t _ m u l t i p l e
 *
 **************************************
 *
 * Functional description
 *	Start a transaction.
 *
 **************************************/
RTR	transaction;
int	**args;
RDB	rdb;
USHORT	l, i;
UCHAR	*p;

NULL_CHECK (rtr_handle, gds_bad_trans_handle);

#ifdef GATEWAY
/* We must peek at the rdb so that we know who to talk to.
   This is safe since the gateway is guaranteed to have a
   single database transaction. */

rdb = (RDB) **vector;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);
#endif

ESTABLISH_PIPES;

PUT_BYTE (op_transaction);
PUT_WORD (GDS_VAL (count));
args = vector;

for (i = 0; i < GDS_VAL (count); i++)
    {
    rdb = (RDB) *(*args++);
    CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);
    PUT_HANDLE (rdb->rdb_handle);
    l = (SSHORT) *args++;
    PUT_WORD (l);
    p = (UCHAR*) *args++;
    PUT_STRING (l, p);
    }

if (check_response (user_status))
    return user_status [1];

*rtr_handle = make_transaction (rdb, (int*) GET_HANDLE);

RETURN_SUCCESS;
}

STATUS GDS_START_TRANSACTION (
    STATUS	*user_status,
    RTR		*rtr_handle,
    SSHORT	GDS_VAL (count),
    ...)
{
/**************************************
 *
 *	g d s _ s t a r t _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Start a transaction.
 *
 **************************************/
RTR	transaction;
va_list	args;
RDB	rdb;
USHORT	l, i;
UCHAR	*p;

NULL_CHECK (rtr_handle, gds_bad_trans_handle);

#ifdef GATEWAY
/* We must peek at the rdb so that we know who to talk to.
   This is safe since the gateway is guaranteed to have a
   single database transaction. */

VA_START (args, count);
rdb = *(va_arg (args, RDB*));
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);
#endif

ESTABLISH_PIPES;

PUT_BYTE (op_transaction);
PUT_WORD (GDS_VAL (count));
VA_START (args, count);

for (i = 0; i < GDS_VAL (count); i++)
    {
    rdb = *(va_arg (args, RDB*));
    CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);
    PUT_HANDLE (rdb->rdb_handle);
    l = va_arg (args, int);
    PUT_WORD (l);
    p = va_arg (args, UCHAR*);
    PUT_STRING (l, p);
    }

if (check_response (user_status))
    return user_status [1];

*rtr_handle = make_transaction (rdb, (int*) GET_HANDLE);

RETURN_SUCCESS;
}

#ifndef BRIDGE
STATUS GDS_TRANSACT_REQUEST (
    STATUS      *user_status,
    RDB         *db_handle,
    RTR         *tra_handle,
    USHORT	blr_length,
    UCHAR	*blr,
    USHORT	in_msg_length,
    UCHAR	*in_msg,
    USHORT	out_msg_length,
    UCHAR	*out_msg)
{
/**************************************
 *
 *	i s c _ t r a n s a c t _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
RDB	rdb;
RTR	transaction;
USHORT	l, count;
UCHAR	*p;

/* Check and validate handles, etc. */

rdb = *db_handle;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);
transaction = *tra_handle;
CHECK_HANDLE (transaction, type_rtr, gds_bad_trans_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_transact_request);
PUT_HANDLE (rdb->rdb_handle);
PUT_HANDLE (transaction->rtr_handle);
PUT_WORD (blr_length);
PUT_STRING (blr_length, blr);
PUT_WORD (in_msg_length);
PUT_STRING (in_msg_length, in_msg);
PUT_WORD (out_msg_length);

if (check_response (user_status))
    return user_status [1];

GET_STRING (out_msg_length, out_msg);

RETURN_SUCCESS;
}
#endif

STATUS GDS_TRANSACTION_INFO (
    STATUS	*user_status,
    RTR		*transaction,
    SSHORT	GDS_VAL (item_length),
    UCHAR	*items,
    SSHORT	GDS_VAL (buffer_length),
    UCHAR	*buffer)
{
/**************************************
 *
 *	g d s _ t r a n s a c t i o n _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Provide information on transaction object.
 *
 **************************************/
RDB	rdb;
USHORT	l;
UCHAR	*p;

CHECK_HANDLE ((*transaction), type_rtr, gds_bad_trans_handle);
rdb = (*transaction)->rtr_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

PUT_BYTE (op_info_transaction);
PUT_HANDLE ((*transaction)->rtr_handle);
PUT_WORD (GDS_VAL (item_length));
PUT_STRING (GDS_VAL (item_length), items);
PUT_WORD (GDS_VAL (buffer_length));
if (check_response (user_status))
    return user_status [1];

GET_STRING (GET_WORD, buffer);
RETURN_SUCCESS;
}

STATUS GDS_UNWIND (
    STATUS	*user_status,
    RRQ		*req_handle,
    SSHORT	GDS_VAL (level))
{
/**************************************
 *
 *	g d s _ u n w i n d
 *
 **************************************
 *
 * Functional description
 *	Unwind a running request.
 *
 **************************************/
RDB	rdb;
RRQ	request;
USHORT	l;
UCHAR	*p;

request = *req_handle;
CHECK_HANDLE (request, type_rrq, gds_bad_req_handle);
rdb = request->rrq_rdb;
CHECK_HANDLE (rdb, type_rdb, gds_bad_db_handle);

ESTABLISH_PIPES;

#ifdef GATEWAY

/* Let's start by trying to flush the input buffer */

if (fseek (read_pipe, (SLONG) 0, 2))
    {
    clearerr (read_pipe);
    fseek (read_pipe, (SLONG) 0, 2);
    }

PUT_BYTE (op_unwind);
PUT_HANDLE (request->rrq_handle);
PUT_WORD (level);

return check_response (user_status);
#else

PUT_BYTE (op_unwind);
PUT_HANDLE (request->rrq_handle);
PUT_WORD (level);

return check_response (user_status);
#endif
}

static STATUS check_response (
    STATUS	*user_status)
{
/**************************************
 *
 *	c h e c k _ r e s p o n s e
 *
 **************************************
 *
 * Functional description
 *	Check response to a remote call.
 *
 **************************************/
USHORT	l, length;
TEXT	*p, *text;
STATUS	*v, code;

text = error_text;

if (!read_pipe || !write_pipe)
    {
    pipe_io_error (user_status, NULL, NULL, 0);
    }
else if (fflush (write_pipe) == EOF)
    {
    /* ERRCHECK: what about EINTR in the fflush call? */
    pipe_io_error (user_status, "fflush", "write pipe", errno);
    (void) fclose (write_pipe);
    write_pipe = NULL;
    }
else
    {
    v = user_status;
    while (code = GET_WORD)
	{
	switch (*v++ = code)
	    {
	    case gds_arg_interpreted:
	    case gds_arg_string:
		length = GET_WORD;
		GET_STRING (length, text);
		*v++ = (STATUS) text;
		text += length;
		*text++ = 0;
		continue;

	    case gds_arg_end:
	    case gds_arg_cstring:
	    default:
		/* Unexpected gds_arg_* value received.
		 *  The remote protocol filters out gds_arg_cstring,
		 *  the loop should terminate with gds_arg_end
		 *  and anything else is out of range.
		 */
#ifdef DEV_BUILD
		pipe_io_error (user_status, "pipe protocol problem", 
				"invalid arg", code);
		return user_status [1];
#endif
	    case EOF:
		/* If the connection breaks while we're still receiving
		 * the status vector we're hosed.  So we'll
		 * throw away the partially received status vector
		 * and just return a connection lost message.
		 */
		pipe_io_error (user_status, "getw", "pipe server", errno);
#ifndef GATEWAY
		(void) fclose (read_pipe);
		read_pipe = NULL;
		/* ERRORCHECK - perhaps write_pipe should also be closed here */
#else
		connection->rdb_pipes.cnct_read_pipe = 0;
#endif
		return user_status [1];

	    case gds_arg_gds:
	    case gds_arg_number:
	    case gds_arg_vms:
	    case gds_arg_unix:
	    case gds_arg_domain:
	    case gds_arg_dos:
	    case gds_arg_mpexl:
	    case gds_arg_mpexl_ipc:
	    case gds_arg_next_mach:
	    case gds_arg_netware:
	    case gds_arg_win32:
		*v++ = GET_WORD;
		continue;
	    }
	}
    *v = gds_arg_end;
    }

return user_status [1];
}

#ifndef GATEWAY
static void event_handler (void)
{
/**************************************
 *
 *	e v e n t _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Respond signal.
 *
 **************************************/
void	*arg;
PTR	ast;
SSHORT	l, length;
UCHAR	*p, events [1024];

ast = (PTR) GET_HANDLE_EVENT;

if ((long) ast == -1)
    return;

arg = (void*) GET_HANDLE_EVENT;
length = GET_WORD_EVENT;

if (l = length)
    {
    p = events; 
    do *p++ = GET_BYTE_EVENT; while (--l);
    }

(*((void (*)(void*, SSHORT, UCHAR*))ast)) (arg, length, events);
}
#endif

#ifdef KILLER_SIGNALS
static int get_byte (void)
{
/**************************************
 *
 *	g e t _ b y t e 
 *
 **************************************
 *
 * Functional description
 *	Get a byte from stdin but watch for EINTR
 *
 **************************************/
int c;

if (!read_pipe)
    return -1;

for (;;)
    {
    errno = 0; 
    if ((c = GETC_UNLOCKED (read_pipe)) == EOF)
        {
	if (SYSCALL_INTERRUPTED(errno))
	    {
	    DEBUGMSG ("get_byte: clearing EINTR");
	    clearerr (read_pipe);
	    rewind (read_pipe);
	    continue;
	    }
        else
            {
	    DEBUGMSG ("get_byte: closing due to error");
	    fclose (read_pipe);
	    read_pipe = NULL;
            gds__log ("Pipe Interface read pipe error %d\n", (int*) errno);
            return -1;
            }
        }
    return c;
    }
}

static int get_byte_event (void)
{
/**************************************
 *
 *	g e t _ b y t e _ e v e n t
 *
 **************************************
 *
 * Functional description
 *	Get a byte from event pipe but watch for EINTR
 *
 **************************************/
int c;

if (!event_pipe)
    return -1;

for (;;)
    {
    errno = 0; 
    if ((c = GETC_UNLOCKED (event_pipe)) == EOF)
        {
	if (SYSCALL_INTERRUPTED(errno))
	    {
	    DEBUGMSG ("get_byte_event: clearing EINTR");
	    clearerr (event_pipe);
	    rewind (event_pipe);
	    continue;
	    }
        else
            {
	    DEBUGMSG ("get_byte_event: closing due to error");
	    fclose (event_pipe);
	    event_pipe = NULL;
            gds__log ("Pipe Interface read event pipe error %d\n", (int*) errno);
            return -1;
            }
        }
    return c;
    }
}
#endif

#ifdef KILLER_SIGNALS
static IPTR get_handle (void)
{
/**************************************
 *
 *	g e t _ h a n d l e 
 *
 **************************************
 *
 * Functional description
 *	Get a handle from stdin but watch for EINTR
 *
 **************************************/

GET_DATA_CODE (h, read_pipe, "Pipe Interface read pipe error %d\n");
}
#endif

#ifdef KILLER_SIGNALS
static IPTR get_handle_event (void)
{
/**************************************
 *
 *	g e t _ h a n d l e _ e v e n t
 *
 **************************************
 *
 * Functional description
 *	Get a handle from event pipe but watch for EINTR
 *
 **************************************/

GET_DATA_CODE (h, event_pipe, "Pipe Interface read event pipe error %d\n");
}
#endif 

#ifdef KILLER_SIGNALS
static SLONG get_word (void)
{
/**************************************
 *
 *	g e t _ w o r d 
 *
 **************************************
 *
 * Functional description
 *	Get a word from stdin but watch for EINTR
 *
 **************************************/

GET_DATA_CODE (l, read_pipe, "Pipe Interface read pipe error %d\n");
}
#endif

#ifdef KILLER_SIGNALS
static SLONG get_word_event (void)
{
/**************************************
 *
 *	g e t _ w o r d _ e v e n t
 *
 **************************************
 *
 * Functional description
 *	Get a word from event pipe but watch for EINTR
 *
 **************************************/

GET_DATA_CODE (l, event_pipe, "Pipe Interface read event pipe error %d\n");
}
#endif 

static STATUS handle_error (
    STATUS	*user_status,
    STATUS	code)
{
/**************************************
 *
 *	h a n d l e _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	An invalid handle has been passed in.  If there is a user status
 *	vector, make it reflect the error.  If not, emulate the routine
 *	"error" and abort.
 *
 **************************************/

*user_status++ = gds_arg_gds;
*user_status++ = code;
*user_status = gds_arg_end;

return code;
}

static RDB init (
    STATUS	*user_status,
    P_OP	op,
    UCHAR	*file_name,
    USHORT	file_length,
    UCHAR	*dpb,
    USHORT	dpb_length)
{
/**************************************
 *
 *	i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize for database access.  First call from both CREATE and
 *	OPEN.
 *
 **************************************/
USHORT	l;
TEXT	*partner, pipe_numbers [32], gds_pipe [MAXPATHLEN], *new_env, new_env_buf [128];
UCHAR	*p;
int	pair1 [2], pair2 [2], pair3 [2], pid, parent_pid, fd;
RDB	rdb;
FILE	*save_pipe;
STATUS	status;
struct stat	stat_buf;

#ifndef GATEWAY
/* Check to see if pipes are already open.  If not, fork. */

if (!read_pipe && !write_pipe)
    {
    if (pipe (pair1) < 0 || pipe (pair2) < 0 || pipe (pair3))
#else
    {
    pair3 [0] = pair3 [1] = -1;
    if (pipe (pair1) < 0 || pipe (pair2) < 0)
#endif
	{
	pipe_io_error (user_status, "pipe", "pipe server", errno);
	return NULL;
	}

#ifdef GATEWAY
    /* We must inform all of our children that the expected
       arrival may try to kill them (by sending them an EOF). */

    for (rdb = PSI_databases; rdb; rdb = rdb->rdb_next)
	{
	write_pipe = (FILE*) rdb->rdb_pipes.cnct_write_pipe;
	PUT_BYTE (op_gateway_sync);
	fflush (write_pipe);
	}
#endif

    parent_pid = getpid();

    if ((partner = getenv (GDS_SERVER)) && *partner)
	new_env = new_env_buf;
    else
	{
	new_env = NULL;
	gds__prefix (gds_pipe, GDS_PIPE);
	partner = gds_pipe;
	}

    /* Probe pipe server to see it if plausibly exists */

    if (statistics (partner, &stat_buf) == -1)
	{
	pipe_io_error (user_status, "stat", partner, errno);
	return NULL;
	}

    if ((pid = vfork()) < 0)
	{
	pipe_io_error (user_status, "vfork", partner, errno);
	return NULL;
	}

    if (pid == 0)
	{
	close (pair1 [0]);
	close (pair2 [1]);
	if (pair2 [0] != 0)
	    {
	    close (0);
	    dup (pair2 [0]);
	    close (pair2 [0]);
	    }
	if (pair1 [1] != 1)
	    {
	    close (1);
	    dup (pair1 [1]);
	    close (pair1 [1]);
	    }
	sprintf (pipe_numbers, "%d %d %d %d", 
		0, 1, pair3 [1], parent_pid);
	if (new_env)
	    {
	    sprintf (new_env, "%s=", GDS_SERVER);
	    putenv (new_env);
	    }
	execl (partner, partner, pipe_numbers, 0);
	abort();
	}

    if (kill (pid, 0) == -1)
	{
	pipe_io_error (user_status, "excl", partner, 0);
	return NULL;
	}

    close (pair1 [1]);
    close (pair2 [0]);
    event_fd = pair3 [0];

    if (!(read_pipe = fdopen (pair1 [0], "r")) ||
        !(write_pipe = fdopen (pair2 [1], "w")))
	{
	pipe_io_error (user_status, "fdopen", "pipe server", errno);
	if (read_pipe)
	    {
	    fclose (read_pipe);
	    read_pipe = NULL;
	    }
	return NULL;
	}
    }

#ifdef SOLARIS
    /* In Solaris 2.5, _filbuf has a bug causing it to return bogus values
       (not in the range -1 through MAX_UCHAR).  Making the read_pipe FILE
       unbuffered avoids triggering the bug. */
 
    setbuf(read_pipe, (char *)0);
#endif
 
/* Check that we have both a read_pipe and a write_pipe.
 * If we don't, then either the above init code didn't complete correctly,
 * or we had an IO error during communication with the server.  That
 * IO error could close either the read_pipe or write_pipe, leaving
 * us in an inconsistant state until the user's process restarted.
 *
 * Note that we cannot silently restart the pipe server, as this client
 * may have multiple attachments going through the same server.  Unless
 * we can invalidate all those database handles, we cannot just leave
 * them around.
 * Possible solutions:
 *   Use the rdb_next field to track all the attachments.
 *   Put the read_pipe & write_pipe handles into the rdb structure
 *
 * Due to time constaints, and that this impacts Unix only, this
 * problem is not being solved at this time. Instead, this simple
 * workaround is put in to prevent core dumps that occur later
 * due to one pipe being closed, but not the other.
 *
 * 1994-August-31 David Schnepper 
 */
if (!read_pipe || !write_pipe)
    {
    pipe_io_error (user_status, "init", "<previous pipe error>", 0);
    return NULL;
    }

/* Send the operation over */

if (file_length == 0)
    file_length = strlen ((char*)file_name);

PUT_BYTE ((SCHAR) op);
PUT_WORD (file_length);
PUT_STRING (file_length, file_name);
PUT_WORD (dpb_length);
PUT_STRING (dpb_length, dpb);

/* Check the return status.  If it failed, return error */

#ifndef GATEWAY
if (check_response (user_status))
    return NULL;
#else
connection = NULL;
status = check_response (user_status);

/* The new child is devious and may try to trick us into
   thinking that the other siblings are dead.  Out smart
   it by reading the EOF (and pray there is one!).  This
   should not be attempted by the faint of heart. */

save_pipe = read_pipe;

for (rdb = PSI_databases; rdb; rdb = rdb->rdb_next)
    {
    read_pipe = (FILE*) rdb->rdb_pipes.cnct_read_pipe;
    GET_BYTE;
    }

read_pipe = save_pipe;

if (status)
    {
    /* Can't connect to database.  Close fork down. */

    fclose (read_pipe);
    fclose (write_pipe);
    return NULL;
    }
#endif

rdb = (RDB) ALLOC (type_rdb);
rdb->rdb_handle = (int*) GET_HANDLE;

#ifdef GATEWAY
rdb->rdb_next = PSI_databases;
PSI_databases = rdb;

/* Since we can fork many processes, save the pipe info */

rdb->rdb_pipes.cnct_read_pipe = (int*) read_pipe;
rdb->rdb_pipes.cnct_write_pipe = (int*) write_pipe;
#endif

return rdb;
}

static RTR make_transaction (
    RDB		rdb,
    HANDLE	handle)
{
/**************************************
 *
 *	m a k e _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Create a local transaction handle.
 *
 **************************************/
RTR	transaction;

transaction = (RTR) ALLOC (type_rtr);
transaction->rtr_rdb = rdb;
transaction->rtr_handle = handle;
transaction->rtr_next = rdb->rdb_transactions;
rdb->rdb_transactions = transaction;

return transaction;
}

static SSHORT name_length (
    TEXT	*name)
{
/*****************************************
 *
 *	n a m e _ l e n g t h
 *
 *****************************************
 *
 * Functional Description
 *	Compute length of user-supplied (blank-padded) or
 * 	NULL-terminated buffer
 *
 *****************************************/
TEXT	*p;

for (p = name; *p && *p != ' '; p++)
    ;

return (p - name);
}

static void pipe_io_error (
    STATUS	*user_status,
    TEXT	*operation,
    TEXT	*file,
    int		unix_errno)
{
/**************************************
 *
 *	p i p e _ i o _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Something happened in system call.
 *
 **************************************/

*user_status++ = gds_arg_gds;
*user_status++ = gds_conn_lost;	/* Connection lost to pipe server */

if (operation)
    {
    *user_status++ = gds_arg_gds;
    *user_status++ = gds_sys_request;
    *user_status++ = gds_arg_string;
    *user_status++ = (STATUS) operation;
    *user_status++ = gds_arg_string;
    *user_status++ = (STATUS) file;
    }

if (unix_errno)
    {
    *user_status++ = gds_arg_unix;
    *user_status++ = unix_errno;
    }

*user_status = gds_arg_end;
}

#ifdef KILLER_SIGNALS
static int put_byte (
    int b)
{
/**************************************
 *
 *	p u t _ b y t e 
 *
 **************************************
 *
 * Functional description
 *	Put a byte into stdout but watch for EINTR
 *
 **************************************/

PUT_DATA_CODE (b, c, write_pipe, "Pipe Interface write pipe error %d\n");
}
#endif

#ifdef KILLER_SIGNALS
static IPTR put_handle (
    IPTR	b)
{
/**************************************
 *
 *	p u t _ h a n d l e 
 *
 **************************************
 *
 * Functional description
 *	Put a handle into stdout but watch for EINTR
 *
 **************************************/

PUT_DATA_CODE (b, h, write_pipe, "Pipe Interface write pipe error %d\n");
}
#endif

#ifdef KILLER_SIGNALS
static SLONG put_word (
    SLONG	b)
{
/**************************************
 *
 *	p u t _ w o r d 
 *
 **************************************
 *
 * Functional description
 *	Put a word into stdout but watch for EINTR
 *
 **************************************/

PUT_DATA_CODE (b, l, write_pipe, "Pipe Interface write pipe error %d\n");
}
#endif

static void release_blob (
    RBL		blob)
{
/**************************************
 *
 *	r e l e a s e _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Release a blob block and friends.
 *
 **************************************/
RTR	transaction;
RBL	*p;

transaction = blob->rbl_rtr;

for (p = &transaction->rtr_blobs; *p; p = &(*p)->rbl_next)
    if (*p == blob)
	{
	*p = blob->rbl_next;
	break;
	}

ALLP_release ((BLK) blob);
}

static STATUS release_object (
    P_OP	op,
    HANDLE	handle,
    STATUS	*status_vector)
{
/**************************************
 *
 *	r e l e a s e _ o b j e c t
 *
 **************************************
 *
 * Functional description
 *	Tell the server to zap an object.  This doesn't not necessary
 *	release the object, but usually does.
 *
 **************************************/
USHORT	l;
UCHAR	*p;

PUT_BYTE ((SCHAR) op);
PUT_HANDLE (handle);

return check_response (status_vector);
}

static void release_request (
    RRQ		request)
{
/**************************************
 *
 *	r e l e a s e _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Release a request block and friends.
 *
 **************************************/
RDB	rdb;
RRQ	*p;
USHORT	i;

rdb = request->rrq_rdb;

for (p = &rdb->rdb_requests; *p; p = &(*p)->rrq_next)
    if (*p == request)
	{
	*p = request->rrq_next;
	break;
	}

ALLP_release ((BLK) request);
}

static void release_transaction (
    RTR		transaction)
{
/**************************************
 *
 *	r e l e a s e _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Release a transaction block and friends.
 *
 **************************************/
RDB	rdb;
RTR	*p;

rdb = transaction->rtr_rdb;

for (p = &rdb->rdb_transactions; *p; p = &(*p)->rtr_next)
    if (*p == transaction)
	{
	*p = transaction->rtr_next;
	break;
	}

ALLP_release ((BLK) transaction);
}

static void release_sql_request (
    RSR		stmt)
{
/**************************************
 *
 *	r e l e a s e _ s q l _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Release an SQL request block.
 *
 **************************************/
RDB	rdb;
RSR	*p;

rdb = stmt->rsr_rdb;

for (p = &rdb->rdb_sql_requests; *p; p = &(*p)->rsr_next)
    if (*p == stmt)
	{
	*p = stmt->rsr_next;
	break;
	}

ALLP_release ((BLK) stmt);
}

static STATUS send_blob (
    STATUS	*user_status,
    RBL		blob,
    USHORT	buffer_length,
    UCHAR	*buffer)
{
/**************************************
 *
 *	s e n d _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Send a partially completed blob.
 *
 **************************************/
USHORT	l;
UCHAR	*p;
P_OP	op;

/* If there is no buffer address, this is a batch send.  Pick up the
   address and length from the blob buffer and blast away */

if (!buffer)
    {
    buffer = blob->rbl_buffer;
    buffer_length = blob->rbl_ptr - buffer;
    blob->rbl_ptr = buffer;
    op = op_put_segment_buf;
    }
else
    op = op_put_segment;

PUT_BYTE (op);
PUT_HANDLE (blob->rbl_handle);
PUT_WORD (buffer_length);
PUT_STRING (buffer_length, buffer);

if (check_response (user_status))
    return user_status [1];

RETURN_SUCCESS;
}
