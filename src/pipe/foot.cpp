/*
 *	PROGRAM:	Pipe Server
 *	MODULE:		foot.c
 *	DESCRIPTION:	Pipe server
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
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <signal.h>
#include <errno.h>
#include "../pipe/pipe.h"
#include "../jrd/license.h"
#include "../jrd/gds.h" 
#include "../jrd/gds_proto.h"
#include "../pipe/allp_proto.h"
#include "../remote/merge_proto.h"

static void	allocate_statement (void);
static void	attach_database (P_OP);
static void	service_attach (void);
static void	cancel_events (void);
static void	compile (void);
static void	ddl (void);
static void	drop_database (void);
static void	end_blob (P_OP);
static void	end_database (void);
static void	end_request (void);
static void	service_end (void);
static void	end_statement (void);
static void	end_transaction (P_OP);
static void	event_ast (RVNT, USHORT, UCHAR *);
static void	execute_immediate (P_OP);
static void	execute_statement (P_OP);
static void	fetch (void);
static UCHAR	*get_buffer (USHORT, USHORT);
static int	get_byte (BOOLEAN);
static IPTR	get_handle (void);
static void	get_segment (P_OP);
static void	get_slice (void);
static SLONG	get_word (void);
static void	info (P_OP);
static void	insert (void);
static RTR	make_transaction (RDB, HANDLE);
static void	open_blob (P_OP op);
static void	prepare_statement (void);
static int	put_byte (int);
static int	put_byte_event (int);
static IPTR	put_handle (IPTR);
static IPTR	put_handle_event (IPTR);
static void	put_segment (P_OP);
static void	put_slice (void);
static SLONG	put_word (SLONG);
static SLONG	put_word_event (SLONG);
static void	que_events (void);
static void	receive_msg (void);
static void	reconnect (void);
static void	release_blob (RBL);
static void	release_request (RRQ);
static void	release_sql_request (RSR);
static void	release_transaction (RTR);
static void	seek_blob (void);
static void	send_msg (void);
static STATUS	send_response (STATUS *);
static void	set_cursor (void);
static void	shutdown_attachments (void);
static void	start (void);
static void	start_and_send (void);
static void	start_transaction (void);
static void	transact_request (void);
static void	unwind (void);

static int	parent_pid = 0;
static UCHAR	*buffer [5] = {NULL, NULL, NULL, NULL, NULL};
static USHORT	buffer_length [5] = {0, 0, 0, 0, 0};
static FILE	*read_pipe = NULL, 
		*write_pipe = NULL, 
		*event_pipe = NULL;
static RDB	PSV_databases = NULL;
static RDB	PSV_services = NULL;
static RVNT	PSV_events = NULL;

#ifdef SOLARIS
/* NOTE: This code is cloned in head.c */
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

/* Define KILLER_SIGNALS for platforms where system calls can
 * be interrupted (and return error EINTR).  These routines
 * check and restart the call.
 */

#define GET_BYTE(f)		get_byte (f)
#define GET_WORD		get_word ()
#define GET_HANDLE		get_handle ()
#define PUT_BYTE(byte)		put_byte (byte)
#define PUT_WORD(word)		put_word ((SLONG) (word))
#define PUT_HANDLE(word)	put_handle ((IPTR) (word))
#define PUT_BYTE_EVENT(byte)	put_byte_event (byte)
#define PUT_WORD_EVENT(word)	put_word_event ((SLONG) (word))
#define PUT_HANDLE_EVENT(word)	put_handle_event ((IPTR) (word))

#else

/* Note that these definitions, while faster, DON'T
 * check for any errors and don't check for an interrupted
 * system call.
 */

#define GET_BYTE(f)		GETC_UNLOCKED (read_pipe)
#define GET_WORD		getw (read_pipe)
#define GET_HANDLE		getw (read_pipe)
#define PUT_BYTE(byte)		PUTC_UNLOCKED (byte, write_pipe)
#define PUT_WORD(word)		putw ((SLONG) word, write_pipe)
#define PUT_HANDLE(word)	putw ((SLONG) word, write_pipe)
#define PUT_BYTE_EVENT(byte)	PUTC_UNLOCKED (byte, event_pipe)
#define PUT_WORD_EVENT(word)	putw ((SLONG) word, event_pipe)
#define PUT_HANDLE_EVENT(word)	putw ((SLONG) word, event_pipe)
#endif


/* Macro to check a given handle for validity.  While generally
 * we can depend on the client-side remote library to do this
 * for us, there were bugs due to not checking on our side.
 * Also, since these handles are passed though to
 * the why-value routines and checked there, strictly speaking
 * we only have to check ones that we dereference before
 * passing along.  For safety's side, we check them all.
 */

/* h = handle to check
 * t = type of handle to expect
 * c = error code to return
 */
#define CHECK_HANDLE(h,t,c)	if (!(h) || (((BLK) (h))->blk_type != (t))) \
				    { \
				    status_vector [0] = gds_arg_gds;\
				    status_vector [1] = (c);\
				    status_vector [2] = gds_arg_end;\
				    (void) send_response (status_vector); \
				    return; \
				    } 

/* Macros to get and validate various types of handles.
 * Note that it's possible the memory we refer to has been
 * freed already - but in most cases we should get the error
 * properly (the normal time we get an invalid handle is
 * a reference to a valid handle sometime after it has been
 * released).
 *
 * Note that transaction handles coming via DSQL can be NULL.
 */
#define RDB_GET_HANDLE(v)	{v = (RDB) GET_HANDLE; CHECK_HANDLE(v,type_rdb,gds__bad_db_handle);}
#define RTR_GET_HANDLE(v)	{v = (RTR) GET_HANDLE; CHECK_HANDLE(v,type_rtr,gds__bad_trans_handle);}
#define RTR_GET_HANDLE_NULL(v)	{v = (RTR) GET_HANDLE; if (v) CHECK_HANDLE(v,type_rtr,gds__bad_trans_handle);}
#define RRQ_GET_HANDLE(v)	{v = (RRQ) GET_HANDLE; CHECK_HANDLE(v,type_rrq,gds__bad_req_handle);}
#define RBL_GET_HANDLE(v)	{v = (RBL) GET_HANDLE; CHECK_HANDLE(v,type_rbl,gds__bad_segstr_handle);}
#define RSR_GET_HANDLE(v)	{v = (RSR) GET_HANDLE; CHECK_HANDLE(v,type_rsr,gds__bad_stmt_handle);}

#define GET_STRING(length,ptr)	if (l = (length))\
	{p = (ptr); do *p++ = GET_BYTE (FALSE); while (--l);}

#define PUT_STRING(length,ptr)	if (l = (length))\
	{p = (ptr); do PUT_BYTE (*p++); while (--l);}

#ifdef DEBUG
/* Defined to trace problems with pipe server */
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
	if (!pipe) \
	    return -1; \
	c.element = data; \
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


#define GDS_ATTACH_DATABASE	gds__attach_database
#define GDS_SERVICE_ATTACH	isc_service_attach
#define GDS_BLOB_INFO		gds__blob_info
#define GDS_CANCEL_BLOB		gds__cancel_blob
#define GDS_CANCEL_EVENTS	gds__cancel_events
#define GDS_CLOSE_BLOB		gds__close_blob
#define GDS_COMMIT		gds__commit_transaction
#define GDS_COMMIT_RETAINING	gds__commit_retaining
#define GDS_COMPILE		gds__compile_request
#define GDS_CREATE_BLOB		gds__create_blob
#define GDS_CREATE_BLOB2	gds__create_blob2
#define GDS_CREATE_DATABASE	gds__create_database
#define GDS_DATABASE_INFO	gds__database_info
#define GDS_DDL			gds__ddl
#define GDS_DETACH		gds__detach_database
#define GDS_SERVICE_DETACH	isc_service_detach
#define GDS_DROP_DATABASE	isc_drop_database
#define GDS_GET_SEGMENT		gds__get_segment
#define GDS_GET_SLICE		gds__get_slice
#define GDS_OPEN_BLOB		gds__open_blob
#define GDS_OPEN_BLOB2		gds__open_blob2
#define GDS_PREPARE		gds__prepare_transaction
#define GDS_PREPARE2		gds__prepare_transaction2
#define GDS_PUT_SEGMENT		gds__put_segment
#define GDS_PUT_SLICE		gds__put_slice
#define GDS_QUE_EVENTS		gds__que_events
#define GDS_SERVICE_QUERY	isc_service_query
#define GDS_RECONNECT		gds__reconnect_transaction
#ifdef SCROLLABLE_CURSORS
#define GDS_RECEIVE		isc_receive2
#else 
#define GDS_RECEIVE		gds__receive
#endif
#define GDS_RELEASE_REQUEST	gds__release_request
#define GDS_REQUEST_INFO	gds__request_info
#define GDS_ROLLBACK		gds__rollback_transaction
#define GDS_SEEK_BLOB		gds__seek_blob
#define GDS_SEND		gds__send
#define GDS_START_AND_SEND	gds__start_and_send
#define GDS_START		gds__start_request
#define GDS_START_MULTIPLE	gds__start_multiple
#define GDS_START_TRANSACTION	gds__start_transaction
#define GDS_TRANSACT_REQUEST	isc_transact_request
#define GDS_TRANSACTION_INFO	gds__transaction_info
#define GDS_UNWIND		gds__unwind_request

#define GDS_DSQL_ALLOCATE	isc_dsql_allocate_statement
#define GDS_DSQL_EXECUTE	isc_dsql_execute2_m
#define GDS_DSQL_EXECUTE_IMMED	isc_dsql_exec_immed3_m
#define GDS_DSQL_FETCH		isc_dsql_fetch_m
#define GDS_DSQL_FREE		isc_dsql_free_statement
#define GDS_DSQL_INSERT		isc_dsql_insert_m
#define GDS_DSQL_PREPARE	isc_dsql_prepare_m
#define GDS_DSQL_SET_CURSOR	isc_dsql_set_cursor_name
#define GDS_DSQL_SQL_INFO	isc_dsql_sql_info


int CLIB_ROUTINE main (
    USHORT	argc,
    TEXT	*argv[])
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Main entrypoint of server.
 *
 **************************************/
int	desc_in, desc_out, desc_event, fd;
P_OP	op, prev_op;
TEXT	buffer [256];

/*
fprintf (stderr, "gds_pipe here argc = %d argv[1] = '%s' getpid = %d\n", 
	argc, argv[1], getpid());
*/

#ifndef DEBUG
#ifdef HAVE_SETPGRP
#ifdef SETPGRP_VOID
(void)setpgrp();
#else 
(void)setpgrp(0, 0);
#endif /* SETPGRP_VOID */
#else
#ifdef HAVE_SETPGID
(void)setpgid(0, 0);
#endif /* HAVE_SETPGID */
#endif /* HAVE_SETPGRP */
#endif /*!DEBUG */

signal (SIGINT, SIG_IGN);
signal (SIGQUIT, SIG_IGN);
signal (SIGHUP, SIG_IGN);

/* Parent left the file descriptor numbers in the first argument.
   Get them out and then get file handles for the pipes. */

if (argc < 2)
    {
    desc_in = 0;
    desc_out = 1;
    }
else
    sscanf (argv [1], "%d %d %d %d", &desc_in, &desc_out, &desc_event, &parent_pid);

if (!(read_pipe = fdopen (desc_in, "r")) ||
    !(write_pipe = fdopen (desc_out, "w")) ||
    !(event_pipe = fdopen (desc_event, "w")))
    {
    fprintf (stderr, "Fdopen failed in pipe server.  Errno = %d\n", errno);
    exit (FINI_ERROR);
    }

#ifdef SOLARIS
/* In Solaris 2.5, _filbuf has a bug causing it to return bogus values
   (not in the range -1 through MAX_UCHAR).  Making the read_pipe FILE
   unbuffered avoids triggering the bug. */
 
setbuf(read_pipe, (char *)0);
#endif
 
op = (P_OP) 0;

while (TRUE)
    {
    prev_op = op;
    op = (P_OP) GET_BYTE (TRUE);
/*
    fprintf (stderr,  "Operation %d\n", op);
*/
    switch (op)
	{
	case op_attach:
	case op_create:
	    attach_database (op);
	    break;

	case op_cancel_events:
	    cancel_events();
	    break;

	case op_compile:
	    compile();
	    break;

	case op_ddl:
	    ddl();
	    break;

	case op_receive:
	    receive_msg();
	    break;

	case op_seek_blob:
	    seek_blob();
	    break;

	case op_send:
	    send_msg();
	    break;

	case op_start:
	    start();
	    break;

	case op_start_and_send:
	    start_and_send();
	    break;

	case op_transact_request:
	    transact_request();
	    break;

	case op_transaction:
	    start_transaction();
	    break;

	case op_prepare:
	case op_rollback:
	case op_commit:
	case op_commit_retaining:
	case op_prepare2:
	    end_transaction (op);
	    break;

	case op_detach:
	    end_database();
	    break;

	case op_drop_database:
	    drop_database();
	    break;

	case op_create_blob:
	case op_open_blob:
	case op_create_blob2:
	case op_open_blob2:
	    open_blob (op);
	    break;
	
	case op_put_segment:
	case op_put_segment_buf:
	    put_segment (op);
	    break;

	case op_get_segment:
	case op_get_segment_buf:
	    get_segment (op);
	    break;

	case op_cancel_blob:
	case op_close_blob:
	    end_blob (op);
	    break;

	case op_release:
	    end_request();
	    break;

	case op_que_events:
	    que_events();
	    break;

	case op_unwind:
	    unwind();
	    break;

	case op_reconnect:
	    reconnect();
	    break;

	case op_info_blob:
	case op_info_request:
	case op_info_database:
	case op_info_transaction:
	case op_info_sql:
	    info (op);
	    break;

	case op_get_slice:
	    get_slice();
	    break;
	
	case op_put_slice:
	    put_slice();
	    break;

	case op_allocate_stmt:
	    allocate_statement();
	    break;

	case op_execute:
	case op_execute2:
	    execute_statement (op);
	    break;

	case op_exec_immediate:
	case op_exec_immediate2:
	    execute_immediate (op);
	    break;

	case op_fetch:
	    fetch();
	    break;

	case op_free_stmt:
	    end_statement();
	    break;

	case op_insert:
	    insert();
	    break;

	case op_prepare_stmt:
	    prepare_statement();
	    break;

	case op_set_cursor:
	    set_cursor();
	    break;

	case op_attach_service:
	    service_attach();
	    break;

	case op_detach_service:
	    service_end();
	    break;

	case op_info_service:
	    info (op);
	    break;

	default:
	    if ((int) op != -1)
                gds__log ("Pipe Server op code %d unknown, previous %d\n",
                          (int) op, (int) prev_op);
    
	    shutdown_attachments();
	    exit (FINI_ERROR);
	}

    if (fflush (write_pipe) == EOF)
	{
	gds__log ("Pipe Server connection lost\n", 0);
	fprintf (stderr, "gds pipe server connection lost");
	shutdown_attachments();
	exit (FINI_ERROR);
	}
    }
}

static void allocate_statement (void)
{
/**************************************
 *
 *	a l l o c a t e _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *	Allocate a statement handle.
 *
 **************************************/
RDB		rdb;
RSR		statement;
HANDLE		handle;
STATUS		status_vector[ISC_STATUS_LENGTH];

RDB_GET_HANDLE (rdb);
handle = NULL;

GDS_DSQL_ALLOCATE (status_vector, 
	&rdb->rdb_handle,
	&handle);

if (send_response (status_vector))
    return;

statement = (RSR) ALLOC (type_rsr);
statement->rsr_handle = handle;
statement->rsr_rdb = rdb;
statement->rsr_next = rdb->rdb_sql_requests;
rdb->rdb_sql_requests = statement;
PUT_HANDLE (statement);
}

static void attach_database (
    P_OP	operation)
{
/**************************************
 *
 *	a t t a c h _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Process an attach or create packet.
 *
 **************************************/
USHORT	l, file_length, dpb_length, object;
UCHAR	*p, *file_name, file_buf [256], *dpb, dpb_buf [256];
HANDLE	handle;
STATUS	status_vector[ISC_STATUS_LENGTH];
RDB	rdb;

file_length = GET_WORD;
file_name = (file_length <= sizeof (file_buf)) ? file_buf : get_buffer (file_length, 1);
GET_STRING (file_length, file_name);
dpb_length = GET_WORD;
dpb = (dpb_length <= sizeof (dpb_buf)) ? dpb_buf : get_buffer (dpb_length, 0);
GET_STRING (dpb_length, dpb);
handle = NULL;

if (operation == op_attach)
    GDS_ATTACH_DATABASE (status_vector, 
	file_length, file_name, 
	GDS_REF (handle), 
	dpb_length, dpb);
else
    GDS_CREATE_DATABASE (status_vector, 
	file_length, file_name, 
	GDS_REF (handle), 
	dpb_length, dpb, 0);

if (!send_response (status_vector))
    {
    rdb = (RDB) ALLOC (type_rdb);
    rdb->rdb_handle = handle;
    rdb->rdb_next = PSV_databases;
    PSV_databases = rdb;
    PUT_HANDLE (rdb);
    }
}


static void service_attach (void)
{
/**************************************
 *
 *	s e r v i c e _ a t t a c h
 *
 **************************************
 *
 * Functional description
 *	Connect to an Interbase service.
 *
 **************************************/
USHORT	l, service_length, spb_length, object;
UCHAR	*p, service_name [256], *spb, spb_buf [256];
HANDLE	handle;
STATUS	status_vector[ISC_STATUS_LENGTH];
RDB	rdb;

service_length = GET_WORD;
GET_STRING (service_length, service_name);
spb_length = GET_WORD;
spb = (spb_length <= sizeof (spb_buf)) ? spb_buf : get_buffer (spb_length, 0);
GET_STRING (spb_length, spb);
handle = NULL;

GDS_SERVICE_ATTACH (status_vector, 
	service_length, service_name, 
	&handle, 
	spb_length, spb);

if (!send_response (status_vector))
    {
    rdb = (RDB) ALLOC (type_rdb);
    rdb->rdb_handle = handle;
    rdb->rdb_next = PSV_services;
    PSV_services = rdb;
    PUT_HANDLE (rdb);
    }
}


static void cancel_events (void)
{
/**************************************
 *
 *	c a n c e l _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *	Cancel an outstanding event call.
 *
 **************************************/
STATUS	status_vector[ISC_STATUS_LENGTH];
RDB	rdb;
RVNT	event;
SLONG	id;

RDB_GET_HANDLE (rdb);
id = GET_WORD;

GDS_CANCEL_EVENTS (status_vector,
    GDS_REF (rdb->rdb_handle), 
    GDS_REF (id));

(void) send_response (status_vector);

for (event = PSV_events; event; event = event->rvnt_next)
    if (event->rvnt_ast && event->rvnt_handle == id)
	{
	event->rvnt_ast = NULL;
	event->rvnt_handle = 0;
	break;
	}
}

static void compile (void)
{
/**************************************
 *
 *	c o m p i l e
 *
 **************************************
 *
 * Functional description
 *	Compile a request.
 *
 **************************************/
RDB	rdb;
RRQ	request;
USHORT	l, blr_length, object;
UCHAR	*p, *blr;
HANDLE	handle;
STATUS	status_vector[ISC_STATUS_LENGTH];

RDB_GET_HANDLE (rdb);
blr_length = GET_WORD;
blr = get_buffer (blr_length, 0);
GET_STRING (blr_length, blr);
handle = NULL;

GDS_COMPILE (status_vector, 
	GDS_REF (rdb->rdb_handle), 
	GDS_REF (handle), 
	blr_length, 
	GDS_VAL (blr));

if (!send_response (status_vector))
    {
    request = (RRQ) ALLOC (type_rrq);
    request->rrq_handle = handle;
    request->rrq_rdb = rdb;
    request->rrq_next = rdb->rdb_requests;
    rdb->rdb_requests = request;
    PUT_HANDLE (request);
    }
}

static void ddl (void)
{
/**************************************
 *
 *	d d l
 *
 **************************************
 *
 * Functional description
 *	Pass thru a DDL call.
 *
 **************************************/
STATUS	status_vector[ISC_STATUS_LENGTH];
RTR	transaction;
RDB	rdb;
UCHAR	*p, *buffer;
USHORT	l, length, number;

RDB_GET_HANDLE (rdb);
RTR_GET_HANDLE (transaction);
length = GET_WORD;
buffer = get_buffer (length, 0);
GET_STRING (length, buffer);

GDS_DDL (status_vector, 
	GDS_REF (rdb->rdb_handle), 
	GDS_REF (transaction->rtr_handle),
	length, 
	GDS_VAL (buffer));

(void) send_response (status_vector);
}


static void drop_database (void)
{
/**************************************
 *
 *	d r o p _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	End a request..
 *
 **************************************/
RDB	rdb, *ptr;
STATUS	status_vector[ISC_STATUS_LENGTH];
STATUS	code;

RDB_GET_HANDLE (rdb);
GDS_DROP_DATABASE (status_vector, 
	&rdb->rdb_handle);

code = send_response (status_vector);
if (!code || (code == gds__drdb_completed_with_errs))
    {
    while (rdb->rdb_requests)
	release_request (rdb->rdb_requests);
    while (rdb->rdb_sql_requests)
	release_sql_request (rdb->rdb_sql_requests);
    while (rdb->rdb_transactions)
	release_transaction (rdb->rdb_transactions);
    for (ptr = &PSV_databases; *ptr; ptr = &(*ptr)->rdb_next)
	if (*ptr == rdb)
	    {
	    *ptr = rdb->rdb_next;
	    break;
	    }
    ALLP_release (rdb);
    }
}


static void end_blob (
    P_OP	operation)
{
/**************************************
 *
 *	e n d _ b l o b
 *
 **************************************
 *
 * Functional description
 *	End a blob..
 *
 **************************************/
RBL	blob;
STATUS	status_vector[ISC_STATUS_LENGTH];

RBL_GET_HANDLE (blob);

if (operation == op_close_blob)
    GDS_CLOSE_BLOB (status_vector, 
	GDS_REF (blob->rbl_handle));
else
    GDS_CANCEL_BLOB (status_vector,
	GDS_REF (blob->rbl_handle));

if (!send_response (status_vector))
    release_blob (blob);
}

static void end_database (void)
{
/**************************************
 *
 *	e n d _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	End a request..
 *
 **************************************/
RDB	rdb, *ptr;
STATUS	status_vector[ISC_STATUS_LENGTH];

RDB_GET_HANDLE (rdb);
GDS_DETACH (status_vector, 
	GDS_REF (rdb->rdb_handle));

if (!send_response (status_vector))
    {
    while (rdb->rdb_requests)
	release_request (rdb->rdb_requests);
    while (rdb->rdb_sql_requests)
	release_sql_request (rdb->rdb_sql_requests);
    while (rdb->rdb_transactions)
	release_transaction (rdb->rdb_transactions);
    for (ptr = &PSV_databases; *ptr; ptr = &(*ptr)->rdb_next)
	if (*ptr == rdb)
	    {
	    *ptr = rdb->rdb_next;
	    break;
	    }
    ALLP_release (rdb);
    }
}

static void end_request (void)
{
/**************************************
 *
 *	e n d _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	End a request..
 *
 **************************************/
RDB	rdb;
RRQ	request;
STATUS	status_vector[ISC_STATUS_LENGTH];

RRQ_GET_HANDLE (request);

GDS_RELEASE_REQUEST (status_vector, 
	GDS_REF (request->rrq_handle));

if (!send_response (status_vector))
    release_request (request);
}


static void service_end (void)
{
/**************************************
 *
 *	s e r v i c e _ e n d
 *
 **************************************
 *
 * Functional description
 *	Close down a service.
 *
 **************************************/
RDB	rdb, *ptr;
STATUS	status_vector[ISC_STATUS_LENGTH];

RDB_GET_HANDLE (rdb);
GDS_SERVICE_DETACH (status_vector, 
	&rdb->rdb_handle);

if (!send_response (status_vector))
    {
    for (ptr = &PSV_services; *ptr; ptr = &(*ptr)->rdb_next)
	if (*ptr == rdb)
	    {
	    *ptr = rdb->rdb_next;
	    break;
	    }
    ALLP_release (rdb);
    }
}


static void end_statement (void)
{
/**************************************
 *
 *	e n d _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *	Free a dynamic statement.
 *
 **************************************/
RSR	statement;
USHORT	option;
STATUS	status_vector[ISC_STATUS_LENGTH];

RSR_GET_HANDLE (statement);
option = GET_WORD;

GDS_DSQL_FREE (status_vector, 
	&statement->rsr_handle,
	option);

if (send_response (status_vector))
    return;

if (!statement->rsr_handle)
    {
    release_sql_request (statement);
    statement = NULL;
    }

PUT_HANDLE (statement);
}

static void end_transaction (
    P_OP	operation)
{
/**************************************
 *
 *	e n d _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	End a transaction.
 *
 **************************************/
RDB	rdb;
RTR	transaction;
UCHAR	*p, *msg;
USHORT	l, msg_length;
STATUS	status_vector[ISC_STATUS_LENGTH];

RTR_GET_HANDLE (transaction);

switch (operation)
    {
    case op_commit:
	if (!GDS_COMMIT (status_vector, 
		GDS_REF (transaction->rtr_handle)))
	    release_transaction (transaction);
	break;

    case op_rollback:
	if (!GDS_ROLLBACK (status_vector, 
		GDS_REF (transaction->rtr_handle)))
	    release_transaction (transaction);
	break;

    case op_commit_retaining:
	if (!GDS_COMMIT_RETAINING (status_vector, 
		GDS_REF (transaction->rtr_handle)))
	    transaction->rtr_flags |= RTR_limbo;
	break;

    case op_prepare:
	if (!GDS_PREPARE (status_vector, 
		GDS_REF (transaction->rtr_handle)))
	    transaction->rtr_flags |= RTR_limbo;
	break;

    case op_prepare2:
	msg_length = GET_WORD;
	msg = get_buffer (msg_length, 0);
	GET_STRING (msg_length, msg);
	if (!GDS_PREPARE2 (status_vector, 
		GDS_REF (transaction->rtr_handle),
		msg_length,
		GDS_VAL (msg)))
	    transaction->rtr_flags |= RTR_limbo;
	break;
    }

(void) send_response (status_vector);
}

static void event_ast (
    RVNT	event,
    USHORT	length,
    UCHAR	*data)
{
/**************************************
 *
 *	e v e n t _ a s t
 *
 **************************************
 *
 * Functional description
 *	Send off event stuff.
 *
 **************************************/

#ifdef DEBUG
fprintf (stderr, "event ast entered\n");
#endif

if (!event->rvnt_ast)
    return;

PUT_HANDLE_EVENT (event->rvnt_ast);
PUT_HANDLE_EVENT (event->rvnt_arg);
PUT_WORD_EVENT (length);

if (length)
    do PUT_BYTE_EVENT (*data++); while (--length);

fflush (event_pipe);
kill (parent_pid, SIGUSR1);
event->rvnt_ast = NULL;
}

static void execute_immediate (
    P_OP	operation)
{
/**************************************
 *
 *	e x e c u t e _ i m m e d i a t e 
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
USHORT	l, length, dialect, in_blr_length, in_msg_type, in_msg_length,
	out_blr_length, out_msg_type, out_msg_length;
UCHAR	*p, *string, *in_blr, *in_msg, *out_blr, *out_msg;
STATUS	status_vector[ISC_STATUS_LENGTH];

RDB_GET_HANDLE (rdb);
RTR_GET_HANDLE_NULL (transaction);
length = GET_WORD;
string = get_buffer (length, 2);
GET_STRING (length, string);
dialect = GET_WORD;
in_blr_length = GET_WORD;
in_blr = get_buffer (in_blr_length, 0);
GET_STRING (in_blr_length, in_blr);
in_msg_type = GET_WORD;
in_msg_length = GET_WORD;
in_msg = get_buffer (in_msg_length, 1);
GET_STRING (in_msg_length, in_msg);
if (operation == op_exec_immediate2)
    {
    out_blr_length = GET_WORD;
    out_blr = get_buffer (out_blr_length, 3);
    GET_STRING (out_blr_length, out_blr);
    out_msg_type = GET_WORD;
    out_msg_length = GET_WORD;
    out_msg = get_buffer (out_msg_length, 4);
    }
else
    {
    out_blr_length = out_msg_type = out_msg_length = 0;
    out_blr = out_msg = NULL;
    }

handle = (transaction) ? transaction->rtr_handle : NULL;

GDS_DSQL_EXECUTE_IMMED (status_vector,
	&rdb->rdb_handle,
	&handle,
	length,
	string,
	dialect,
	in_blr_length,
	in_blr,
	in_msg_type,
	in_msg_length,
	in_msg,
	out_blr_length,
	out_blr,
	out_msg_type,
	out_msg_length,
	out_msg);

(void) send_response (status_vector);

if (transaction && !handle)
    {
    release_transaction (transaction);
    transaction = NULL;
    }
else if (!transaction && handle)
    transaction = make_transaction (rdb, handle);

PUT_HANDLE (transaction);
if (operation == op_exec_immediate2 && !status_vector [1])
    {
    PUT_WORD (out_msg_length);
    PUT_STRING (out_msg_length, out_msg);
    }
}

static void execute_statement (
    P_OP	operation)
{
/**************************************
 *
 *	e x e c u t e _ s t a t e m e n t 
 *
 **************************************
 *
 * Functional description
 *	Execute a non-SELECT dynamic SQL statement.
 *
 **************************************/
RTR	transaction;
RSR	statement;
HANDLE	handle;
USHORT	l, in_blr_length, in_msg_type, in_msg_length,
	out_blr_length, out_msg_type, out_msg_length;
UCHAR	*p, *in_blr, *in_msg, *out_blr, *out_msg;
STATUS	status_vector[ISC_STATUS_LENGTH];

RTR_GET_HANDLE_NULL (transaction);
RSR_GET_HANDLE (statement);
in_blr_length = GET_WORD;
in_blr = get_buffer (in_blr_length, 0);
GET_STRING (in_blr_length, in_blr);
in_msg_type = GET_WORD;
in_msg_length = GET_WORD;
in_msg = get_buffer (in_msg_length, 1);
GET_STRING (in_msg_length, in_msg);
if (operation == op_execute2)
    {
    out_blr_length = GET_WORD;
    out_blr = get_buffer (out_blr_length, 2);
    GET_STRING (out_blr_length, out_blr);
    out_msg_type = GET_WORD;
    out_msg_length = GET_WORD;
    out_msg = get_buffer (out_msg_length, 3);
    }
else
    {
    out_blr_length = out_msg_type = out_msg_length = 0;
    out_blr = out_msg = NULL;
    }

handle = (transaction) ? transaction->rtr_handle : NULL;

GDS_DSQL_EXECUTE (status_vector,
	&handle,
	&statement->rsr_handle,
	in_blr_length,
	in_blr,
	in_msg_type,
	in_msg_length,
	in_msg,
	out_blr_length,
	out_blr,
	out_msg_type,
	out_msg_length,
	out_msg);

(void) send_response (status_vector);

if (transaction && !handle)
    {
    release_transaction (transaction);
    transaction = NULL;
    }
else if (!transaction && handle)
    transaction = make_transaction (statement->rsr_rdb, handle);

PUT_HANDLE (transaction);
if (operation == op_execute2 && !status_vector [1])
    {
    PUT_WORD (out_msg_length);
    PUT_STRING (out_msg_length, out_msg);
    }
}

static void fetch (void)
{
/**************************************
 *
 *	f e t c h
 *
 **************************************
 *
 * Functional description
 *	Fetch next record from a dynamic SQL cursor.
 *
 **************************************/
RSR	statement;
USHORT	l, blr_length, msg_type, msg_length;
UCHAR	*p, *blr, *msg;
STATUS	s, status_vector[ISC_STATUS_LENGTH];

RSR_GET_HANDLE (statement);
blr_length = GET_WORD;
blr = get_buffer (blr_length, 0);
GET_STRING (blr_length, blr);
msg_type = GET_WORD;
msg_length = GET_WORD;
msg = get_buffer (msg_length, 1);

s = GDS_DSQL_FETCH (status_vector,
	&statement->rsr_handle,
	blr_length,
	blr,
	msg_type,
	msg_length,
	msg);

(void) send_response (status_vector);

PUT_WORD (s);

/* Note: code 101 should be allowed here for a gds__segment on dynamic blob
 * cursors
 */
if (s != 100 && !status_vector [1])
    {
    PUT_WORD (msg_length);
    PUT_STRING (msg_length, msg);
    }
}

static UCHAR *get_buffer (
    USHORT	length,
    USHORT	n)
{
/**************************************
 *
 *	g e t _ b u f f e r
 *
 **************************************
 *
 * Functional description
 *	Get a buffer "big enough".
 *
 **************************************/

if (buffer_length [n] >= length)
    return buffer [n];

if (buffer [n])
    ALLP_free (buffer [n]);
buffer_length [n] = length;

return buffer [n] = ALLP_alloc ((SLONG) length);
}

#ifdef KILLER_SIGNALS
static int get_byte (
    BOOLEAN	eof_flag)
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
int	c;

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
	    if (!eof_flag)
		{
		DEBUGMSG ("get_byte: EOF on read_pipe");
		};
            if (!eof_flag ||
                 (errno && (errno != ENOENT) && (errno != EINVAL)))
		 gds__log ("Pipe Server read pipe error %d\n", (int*) errno);
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

GET_DATA_CODE (h, read_pipe, "Pipe Server read pipe error %d\n");
}
#endif 

static void get_segment (
    P_OP	operation)
{
/**************************************
 *
 *	g e t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Get a single blob segment.
 *
 **************************************/
RDB	rdb;
RBL	blob;
USHORT	l, buffer_length, length;
UCHAR	*p, *buffer;
STATUS	status_vector[ISC_STATUS_LENGTH], state;

RBL_GET_HANDLE (blob);
buffer_length = GET_WORD;
buffer = get_buffer (buffer_length, 0);

if (operation == op_get_segment)
    {
    /* Get the segment data - return it regardless of result
     * as that is what the client side expects.  Note that
     * gds__segment can be returned in the status vector,
     * but that the data is still valid.
     */

    length = 0;

    GDS_GET_SEGMENT (status_vector,
	GDS_REF (blob->rbl_handle),
	GDS_REF (length),
	buffer_length,
	GDS_VAL (buffer));

    (void) send_response (status_vector);
    PUT_WORD (length);
    PUT_STRING (length, buffer);
    }
else /* operation == op_get_segment_buf */
    {
    /* Gobble up a buffer's worth of segments */

    p = buffer;
    state = 0;

    while (buffer_length > 2)
	{
	buffer_length -= 2;
	p += 2;
	GDS_GET_SEGMENT (status_vector,
	    GDS_REF (blob->rbl_handle),
	    GDS_REF (length),
	    buffer_length,
	    GDS_VAL (p));
	if (status_vector [1] == gds__segstr_eof)
	    {
	    state = 2;
	    status_vector [1] = SUCCESS;
	    p -= 2;
	    break;
	    }
	if (status_vector [1] && (status_vector [1] != gds__segment))
	    {
	    p -= 2;
	    break;
	    }
	p [-2] = length;
	p [-1] = length >> 8;
	p += length;
	buffer_length -= length;
	if (status_vector [1] == gds__segment)
	    {
	    state = 1;
	    status_vector [1] = SUCCESS;
	    break;
	    }
	}

    length = p - buffer;
    if (!send_response (status_vector))
	{
	PUT_WORD (length);
	PUT_STRING (length, buffer);
	PUT_WORD (state);
	}
    }
}

static void get_slice (void)
{
/**************************************
 *
 *	g e t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *	Get slice of array;
 *
 **************************************/
RDB		rdb;
RTR		transaction;
STATUS		status_vector[ISC_STATUS_LENGTH];
USHORT		sdl_length, param_length;
ULONG		l, slice_length, return_length, parameters [32];
UCHAR		*p, *sdl, sdl_buf [256], *slice;
struct bid	array_id;

RDB_GET_HANDLE (rdb);
RTR_GET_HANDLE (transaction);
array_id.bid_relation_id = GET_WORD;
array_id.bid_number = GET_WORD;
sdl_length = GET_WORD;
sdl = (sdl_length <= sizeof (sdl_buf)) ? sdl_buf : get_buffer (sdl_length, 0);
GET_STRING (sdl_length, sdl);
param_length = GET_WORD;
GET_STRING (param_length, (UCHAR*) parameters);
slice_length = GET_WORD;

slice = (slice_length) ? ALLP_alloc (slice_length) : NULL;
if (slice)
    memset (slice, 0, slice_length);

GDS_GET_SLICE (status_vector,
	GDS_REF (rdb->rdb_handle), 
	GDS_REF (transaction->rtr_handle), 
	GDS_REF (array_id),
	sdl_length,
	sdl,
	param_length,
	parameters,
	slice_length,
	slice,
	GDS_REF (return_length));

if (!send_response (status_vector))
    {
    PUT_WORD (return_length);
    PUT_STRING (slice_length, slice);
    }

if (slice)
    ALLP_free (slice);
}

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

GET_DATA_CODE (l, read_pipe, "Pipe Server read pipe error %d\n");
}
#endif 

static void info (
    P_OP	operation)
{
/**************************************
 *
 *	i n f o
 *
 **************************************
 *
 * Functional description
 *	Issue information.
 *
 **************************************/
HANDLE	handle;
STATUS	status_vector[ISC_STATUS_LENGTH];
USHORT	item_length, recv_item_length, buffer_length, l, incarnation;
UCHAR	*p, *buffer, *items, *recv_items, *temp;
UCHAR	items_buf [128], recv_items_buf [128], temp_buf [512];
RDB	rdb;
RTR	transaction;
RBL	blob;
RRQ	request;
RSR	statement;

handle = (HANDLE) GET_HANDLE;
if (operation == op_info_request)
    incarnation = GET_WORD;
item_length = GET_WORD;
items = (item_length <= sizeof (items_buf)) ? items_buf : get_buffer (item_length, 1);
GET_STRING (item_length, items);
if (operation == op_info_service)
    {
    recv_item_length = GET_WORD;
    recv_items = (recv_item_length <= sizeof (recv_items_buf)) ?
	recv_items_buf : get_buffer (recv_item_length, 3);
    GET_STRING (recv_item_length, recv_items);
    }
buffer_length = GET_WORD;
buffer = get_buffer (buffer_length, 0);

switch (operation)
    {
    case op_info_database:
	temp = (buffer_length <= sizeof (temp_buf)) ?
	    temp_buf : get_buffer (buffer_length, 2);
	rdb = (RDB) handle;
	CHECK_HANDLE (rdb, type_rdb, gds__bad_db_handle);
	if (!GDS_DATABASE_INFO (status_vector,
		GDS_REF (rdb->rdb_handle),
		item_length,
		items,
		buffer_length,
		temp))
	    MERGE_database_info (temp, buffer, buffer_length,
		IMPLEMENTATION, 8, 1, GDS_VERSION, "", 0);
	break;

    case op_info_blob:
	blob = (RBL) handle;
	CHECK_HANDLE (blob, type_rbl, gds__bad_segstr_handle);
	GDS_BLOB_INFO (status_vector,
		GDS_REF (blob->rbl_handle),
		item_length,
		items,
		buffer_length,
		GDS_VAL (buffer));
	break;

    case op_info_request:
	request = (RRQ) handle;
	CHECK_HANDLE (request, type_rrq, gds__bad_req_handle);
	GDS_REQUEST_INFO (status_vector,
		GDS_REF (request->rrq_handle),
		incarnation,
		item_length,
		items,
		buffer_length,
		GDS_VAL (buffer));
	break;

    case op_info_transaction:
	transaction = (RTR) handle;
	CHECK_HANDLE (transaction, type_rtr, gds__bad_trans_handle);
	GDS_TRANSACTION_INFO (status_vector,
		GDS_REF (transaction->rtr_handle),
		item_length,
		items,
		buffer_length,
		GDS_VAL (buffer));
	break;

    case op_info_service:
	rdb = (RDB) handle;
	CHECK_HANDLE (rdb, type_rdb, gds__bad_db_handle);
	GDS_SERVICE_QUERY (status_vector,
		&rdb->rdb_handle,
		NULL, 
		item_length,
		items,
		recv_item_length,
		recv_items,
		buffer_length,
		buffer);
	break;

    case op_info_sql:
	statement = (RSR) handle;
	CHECK_HANDLE (statement, type_rsr, gds__bad_stmt_handle);
	GDS_DSQL_SQL_INFO (status_vector,
		&statement->rsr_handle,
		item_length,
		items,
		buffer_length,
		buffer);
	break;
    }

if (send_response (status_vector))
    return;

PUT_WORD (buffer_length);
PUT_STRING (buffer_length, buffer);
}

static void insert (void)
{
/**************************************
 *
 *	i n s e r t
 *
 **************************************
 *
 * Functional description
 *	Insert next record into a dynamic SQL cursor.
 *
 **************************************/
RSR	statement;
USHORT	l, blr_length, msg_type, msg_length;
UCHAR	*p, *blr, *msg;
STATUS	status_vector[ISC_STATUS_LENGTH];

RSR_GET_HANDLE (statement);
blr_length = GET_WORD;
blr = get_buffer (blr_length, 0);
GET_STRING (blr_length, blr);
msg_type = GET_WORD;
msg_length = GET_WORD;
msg = get_buffer (msg_length, 1);
GET_STRING (msg_length, msg);

GDS_DSQL_INSERT (status_vector,
	&statement->rsr_handle,
	blr_length,
	blr,
	msg_type,
	msg_length,
	msg);

(void) send_response (status_vector);
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

static void open_blob (
    P_OP	op)
{
/**************************************
 *
 *	o p e n _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Open or create a new blob.
 *
 **************************************/
RDB		rdb;
RBL		blob;
RTR		transaction;
HANDLE		handle;
STATUS		status_vector[ISC_STATUS_LENGTH];
USHORT		l, bpb_length;
UCHAR		*p, *bpb, bpb_buf [128];
struct bid	blob_id;

RDB_GET_HANDLE (rdb);
RTR_GET_HANDLE (transaction);
handle = NULL;
bpb_length = 0;

switch (op)
    {
    case op_open_blob:
	blob_id.bid_relation_id = GET_WORD;
	blob_id.bid_number = GET_WORD;
	GDS_OPEN_BLOB (status_vector, 
	    GDS_REF (rdb->rdb_handle), 
	    GDS_REF (transaction->rtr_handle), 
	    GDS_REF (handle), 
	    GDS_REF (blob_id));
	break;
    
    case op_create_blob:
	GDS_CREATE_BLOB (status_vector,
	    GDS_REF (rdb->rdb_handle), 
	    GDS_REF (transaction->rtr_handle), 
	    GDS_REF (handle), 
	    GDS_REF (blob_id));
	break;

    case op_open_blob2:
	blob_id.bid_relation_id = GET_WORD;
	blob_id.bid_number = GET_WORD;
	bpb_length = GET_WORD;
	bpb = (bpb_length <= sizeof (bpb_buf)) ? bpb_buf : get_buffer (bpb_length, 0);
	GET_STRING (bpb_length, bpb);
	GDS_OPEN_BLOB2 (status_vector, 
	    GDS_REF (rdb->rdb_handle), 
	    GDS_REF (transaction->rtr_handle), 
	    GDS_REF (handle), 
	    GDS_REF (blob_id),
	    bpb_length,
	    bpb);
	break;
    
    case op_create_blob2:
	bpb_length = GET_WORD;
	bpb = (bpb_length <= sizeof (bpb_buf)) ? bpb_buf : get_buffer (bpb_length, 0);
	GET_STRING (bpb_length, bpb);
	GDS_CREATE_BLOB2 (status_vector,
	    GDS_REF (rdb->rdb_handle), 
	    GDS_REF (transaction->rtr_handle), 
	    GDS_REF (handle), 
	    GDS_REF (blob_id),
	    bpb_length,
	    bpb);
	break;
    }

if (send_response (status_vector))
    return;

blob = (RBL) ALLOC (type_rbl);
blob->rbl_handle = handle;
blob->rbl_rdb = rdb;
blob->rbl_rtr = transaction;
blob->rbl_next = transaction->rtr_blobs;
transaction->rtr_blobs = blob;
PUT_HANDLE (blob);

if (op == op_create_blob || op == op_create_blob2)
    {
    PUT_WORD (blob_id.bid_relation_id);
    PUT_WORD (blob_id.bid_number);
    }
}

static void prepare_statement (void)
{
/**************************************
 *
 *	p r e p a r e _ s t a t e m e n t 
 *
 **************************************
 *
 * Functional description
 *	Prepare a dynamic SQL statement for execution.
 *
 **************************************/
RTR	transaction;
RSR	statement;
USHORT	l, length, item_length, buffer_length, dialect;
UCHAR	*p, *string, *items, items_buf [128], *buffer;
STATUS	status_vector[ISC_STATUS_LENGTH];
HANDLE	handle;

RTR_GET_HANDLE_NULL (transaction);
RSR_GET_HANDLE (statement);
length = GET_WORD;
string = get_buffer (length, 0);
GET_STRING (length, string);
dialect = GET_WORD;
item_length = GET_WORD;
items = (item_length <= sizeof (items_buf)) ? items_buf : get_buffer (item_length, 2);
GET_STRING (item_length, items);
buffer_length = GET_WORD;
buffer = get_buffer (buffer_length, 1);

handle = (transaction) ? transaction->rtr_handle : NULL;

GDS_DSQL_PREPARE (status_vector,
	&handle,
	&statement->rsr_handle,
	length,
	string,
	dialect,
	item_length,
	items,
	buffer_length,
	buffer);

if (!send_response (status_vector))
    {
    PUT_WORD (buffer_length);
    PUT_STRING (buffer_length, buffer);
    }
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

PUT_DATA_CODE (b, c, write_pipe, "Pipe Server write pipe error %d\n");
}
#endif

#ifdef KILLER_SIGNALS
static int put_byte_event (
    int b)
{
/**************************************
 *
 *	p u t _ b y t e _ e v e n t
 *
 **************************************
 *
 * Functional description
 *	Put a byte into event pipe but watch for EINTR
 *
 **************************************/

PUT_DATA_CODE (b, c, event_pipe, "Pipe Server write event pipe error %d\n");
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

PUT_DATA_CODE (b, h, write_pipe, "Pipe Server write pipe error %d\n");
}
#endif

#ifdef KILLER_SIGNALS
static IPTR put_handle_event (
    IPTR	b)
{
/**************************************
 *
 *	p u t _ h a n d l e _ e v e n t
 *
 **************************************
 *
 * Functional description
 *	Put a handle into event pipe but watch for EINTR
 *
 **************************************/

PUT_DATA_CODE (b, h, event_pipe, "Pipe Server write event pipe error %d\n");
}
#endif

static void put_segment (
    P_OP	operation)
{
/**************************************
 *
 *	p u t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Write a single blob segment.
 *
 **************************************/
RDB	rdb;
RBL	blob;
USHORT	l, buffer_length, length;
UCHAR	*p, *buffer, *end;
STATUS	status_vector[ISC_STATUS_LENGTH];

RBL_GET_HANDLE (blob);
buffer_length = GET_WORD;
buffer = get_buffer (buffer_length, 0);
GET_STRING (buffer_length, buffer);

if (operation == op_put_segment)
    {
    /* Do the single segment version.  If it failed, just pass on the
       bad news. */

    GDS_PUT_SEGMENT (status_vector, 
	GDS_REF (blob->rbl_handle), 
	buffer_length, 
	GDS_VAL (buffer));
    }
else /* operation == op_put_segment_buf */
    {
    /* We've got a batch of segments.  This is only slightly more complicated */

    end = buffer + buffer_length;

    while (buffer < end)
	{
	length = *buffer++;
	length += *buffer++ << 8;
	GDS_PUT_SEGMENT (status_vector,
	    GDS_REF (blob->rbl_handle),
	    length,
	    GDS_VAL (buffer));
	if (status_vector [1])
	    break;
	buffer += length;
	}
    }

(void) send_response (status_vector);
}

static void put_slice (void)
{
/**************************************
 *
 *	p u t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *	Put slice of array;
 *
 **************************************/
RDB		rdb;
RTR		transaction;
STATUS		status_vector[ISC_STATUS_LENGTH];
USHORT		sdl_length, param_length;
ULONG		l, slice_length, return_length, parameters [32];
UCHAR		*p, *sdl, sdl_buf [256], *slice;
struct bid	array_id;

RDB_GET_HANDLE (rdb);
RTR_GET_HANDLE (transaction);
array_id.bid_relation_id = GET_WORD;
array_id.bid_number = GET_WORD;
sdl_length = GET_WORD;
sdl = (sdl_length <= sizeof (sdl_buf)) ? sdl_buf : get_buffer (sdl_length, 0);
GET_STRING (sdl_length, sdl);
param_length = GET_WORD;
GET_STRING (param_length, (UCHAR*) parameters);
slice = NULL;

if (slice_length = GET_WORD)
    {
    slice = ALLP_alloc (slice_length);
    GET_STRING (slice_length, slice);
    }

GDS_PUT_SLICE (status_vector,
	GDS_REF (rdb->rdb_handle), 
	GDS_REF (transaction->rtr_handle), 
	GDS_REF (array_id),
	sdl_length,
	sdl,
	param_length,
	parameters,
	slice_length,
	GDS_VAL (slice));

if (slice)
    ALLP_free (slice);

if (send_response (status_vector))
    return;

PUT_WORD (array_id.bid_relation_id);
PUT_WORD (array_id.bid_number);
}

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

PUT_DATA_CODE (b, l, write_pipe, "Pipe Server write pipe error %d\n");
}
#endif

#ifdef KILLER_SIGNALS
static SLONG put_word_event (
    SLONG	b)
{
/**************************************
 *
 *	p u t _ w o r d _ e v e n t
 *
 **************************************
 *
 * Functional description
 *	Put a word into event pipe but watch for EINTR
 *
 **************************************/

PUT_DATA_CODE (b, l, event_pipe, "Pipe Server write event pipe error %d\n");
}
#endif

static void que_events (void)
{
/**************************************
 *
 *	q u e _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *	Que events for asynchronous notification.
 *
 **************************************/
RDB	rdb;
USHORT	l, length, object;
UCHAR	*p, *events;
STATUS	status_vector[ISC_STATUS_LENGTH];
RVNT	event;

#ifdef DEBUG
fprintf (stderr, "que_events entered\n");
#endif

/* Find or allocate pipe block */

for (event = PSV_events; event; event = event->rvnt_next)
    if (!event->rvnt_ast)
	break;

if (!event)
    {
    event = (RVNT) ALLOC (type_rvnt);
    event->rvnt_next = PSV_events;
    PSV_events = event;
    }

/* Get data, etc */

RDB_GET_HANDLE (rdb);
event->rvnt_ast = (void (*)()) GET_HANDLE;
event->rvnt_arg = (void *) GET_HANDLE;
length = GET_WORD;
events = get_buffer (length, 0);
GET_STRING (length, events);

#ifdef DEBUG
fprintf (stderr, "que_events: making call \n");
#endif

GDS_QUE_EVENTS (status_vector, 
	GDS_REF (rdb->rdb_handle), 
	GDS_REF (event->rvnt_handle), 
	length, 
	GDS_VAL (events),
	event_ast,
	GDS_VAL (event));

#ifdef DEBUG
fprintf (stderr, "que_events: call status: %d\n", status_vector [1]);
#endif

if (send_response (status_vector))
    event->rvnt_ast = 0;
else
    PUT_WORD (event->rvnt_handle);
}

static void receive_msg (void)
{
/**************************************
 *
 *	r e c e i v e _ m s g
 *
 **************************************
 *
 * Functional description
 *	Receive a message.
 *
 **************************************/
STATUS	status_vector[ISC_STATUS_LENGTH];
RRQ	request;
RDB	rdb;
USHORT	l, length, number, level;
#ifdef SCROLLABLE_CURSORS
USHORT	direction;
ULONG	offset;
#endif
UCHAR	*p, *buffer;

RRQ_GET_HANDLE (request);
number = GET_WORD;
length = GET_WORD;
buffer = get_buffer (length, 0);
level = GET_WORD;
#ifdef SCROLLABLE_CURSORS
direction = GET_WORD; 
offset = GET_WORD;
#endif

GDS_RECEIVE (status_vector, 
	GDS_REF (request->rrq_handle), 
	number, 
	length, 
	GDS_VAL (buffer),
	level
#ifdef SCROLLABLE_CURSORS
	, 
	direction, 
	offset
#endif
	);

if (send_response (status_vector))
    return;

PUT_STRING (length, buffer);
}

static void reconnect (void)
{
/**************************************
 *
 *	r e c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Reconnect to a transaction in limbo.
 *
 **************************************/
RDB	rdb;
RTR	transaction;
USHORT	l, length, count, object;
UCHAR	*p, *buffer;
HANDLE	handle;
STATUS	status_vector[ISC_STATUS_LENGTH];

RDB_GET_HANDLE (rdb);
length = GET_WORD;
buffer = get_buffer (length, 0);
GET_STRING (length, buffer);
handle = NULL;

GDS_RECONNECT (status_vector, 
	GDS_REF (rdb->rdb_handle),
	GDS_REF (handle), 
	length, 
	GDS_VAL (buffer));

if (!send_response (status_vector))
    {
    transaction = make_transaction (rdb, handle);
    transaction->rtr_flags |= RTR_limbo;
    PUT_HANDLE (transaction);
    }
}

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
RDB	rdb;
RTR	transaction;
RBL	*p;

rdb = blob->rbl_rdb;
transaction = blob->rbl_rtr;

for (p = &transaction->rtr_blobs; *p; p = &(*p)->rbl_next)
    if (*p == blob)
	{
	*p = blob->rbl_next;
	break;
	}

ALLP_release (blob);
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

rdb = request->rrq_rdb;

for (p = &rdb->rdb_requests; *p; p = &(*p)->rrq_next)
    if (*p == request)
	{
	*p = request->rrq_next;
	break;
	}

ALLP_release (request);
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

ALLP_release (stmt);
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

while (transaction->rtr_blobs)
    release_blob (transaction->rtr_blobs);

for (p = &rdb->rdb_transactions; *p; p = &(*p)->rtr_next)
    if (*p == transaction)
	{
	*p = transaction->rtr_next;
	break;
	}

ALLP_release (transaction);
}

static void seek_blob (void)
{
/**************************************
 *
 *	s e e k _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Perform a blob seek.
 *
 **************************************/
RDB	rdb;
RBL	blob;
SSHORT	mode;
SLONG	offset, result;
STATUS	status_vector[ISC_STATUS_LENGTH];

RBL_GET_HANDLE (blob);
mode = GET_WORD;
offset = GET_WORD;

/* Do the call.  We basically don't care whether it succeeds or not */

GDS_SEEK_BLOB (status_vector, 
	GDS_REF (blob->rbl_handle), 
	mode, 
	offset,
	GDS_REF (result));

if (send_response (status_vector))
    return;

PUT_WORD (result);
}

static void send_msg (void)
{
/**************************************
 *
 *	s e n d _ m s g
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
STATUS	status_vector[ISC_STATUS_LENGTH];
RRQ	request;
RTR	transaction;
RDB	rdb;
UCHAR	*p, *buffer;
USHORT	l, length, number, level;

RRQ_GET_HANDLE (request);
number = GET_WORD;
length = GET_WORD;
buffer = get_buffer (length, 0);
GET_STRING (length, buffer);
level = GET_WORD;

GDS_SEND (status_vector, 
	GDS_REF (request->rrq_handle),
	number, 
	length, 
	GDS_VAL (buffer),
	level);

(void) send_response (status_vector);
}

static STATUS send_response (
    STATUS	*status_vector)
{
/**************************************
 *
 *	s e n d _ r e s p o n s e
 *
 **************************************
 *
 * Functional description
 *	Send a response packet.
 *
 **************************************/
STATUS	code;
USHORT	l, length;
TEXT	*p, buffer [1024];

if (!(code = status_vector [1]))
    {
    PUT_WORD ((SLONG) *status_vector++);
    PUT_WORD ((SLONG) *status_vector++);
    PUT_WORD (gds_arg_end);
    return SUCCESS;
    }

/* Start by translating the status vector into "generic" form */

while (*status_vector)
    switch (*status_vector)
	{
	case gds_arg_interpreted:
	case gds_arg_string:
	    PUT_WORD ((SLONG) *status_vector++);
	    p = (TEXT*) *status_vector++;
	    length = ((p) ? strlen (p) : 0);
	    PUT_WORD (length);
	    PUT_STRING (length, p);
	    break;

	case gds_arg_cstring:
	    PUT_WORD (gds_arg_string);
	    status_vector++;
	    length = *status_vector++;
	    PUT_WORD (length);
	    p = (TEXT*) *status_vector++;
	    PUT_STRING (length, p);
	    break;
	
	case gds_arg_number:
	case gds_arg_gds:
	    PUT_WORD ((SLONG) *status_vector++);
	    PUT_WORD ((SLONG) *status_vector++);
	    break;

	default:
	    if (!(length = gds__interprete (buffer, &status_vector)))
		{
		*status_vector = 0;
		break;
		}
	    PUT_WORD (gds_arg_interpreted);
	    PUT_WORD (length);
	    PUT_STRING (length, buffer);
	    break;
	}

PUT_WORD (gds_arg_end);

return code;
}

static void set_cursor (void)
{
/**************************************
 *
 *	s e t _ c u r s o r
 *
 **************************************
 *
 * Functional description
 *	Set a cursor name for a dynamic request.
 *
 **************************************/
RSR	statement;
USHORT	l, length, type;
UCHAR	*p, *cursor;
STATUS	status_vector[ISC_STATUS_LENGTH];

RSR_GET_HANDLE (statement);
length = GET_WORD;
cursor = get_buffer (length, 0);
GET_STRING (length, cursor);
type = GET_WORD;

GDS_DSQL_SET_CURSOR (status_vector, 
	&statement->rsr_handle,
	cursor,
	type);

(void) send_response (status_vector);
}

static void shutdown_attachments (void)
{
/**************************************
 *
 *	s h u t d o w n _ a t t a c h m e n t s
 *
 **************************************
 *
 * Functional description
 *	About to exit, detach any remaining database
 *	and service attachments.
 *
 **************************************/
RDB	rdb;
RTR	transaction;
STATUS	status_vector[ISC_STATUS_LENGTH];

for (rdb = PSV_databases; rdb; rdb = rdb->rdb_next)
    {

    GDS_DETACH (status_vector, GDS_REF (rdb->rdb_handle));
    }

for (rdb = PSV_services; rdb; rdb = rdb->rdb_next)
    GDS_SERVICE_DETACH (status_vector, &rdb->rdb_handle);
}

static void start (void)
{
/**************************************
 *
 *	s t a r t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
STATUS	status_vector[ISC_STATUS_LENGTH];
RRQ	request;
RTR	transaction;
RDB	rdb;
USHORT	level;

RRQ_GET_HANDLE (request);
RTR_GET_HANDLE (transaction);
level = GET_WORD;

GDS_START (status_vector, 
	GDS_REF (request->rrq_handle), 
	GDS_REF (transaction->rtr_handle),
	level);

(void) send_response (status_vector);
}

static void start_and_send (void)
{
/**************************************
 *
 *	s t a r t _ a n d _ s e n d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
STATUS	status_vector[ISC_STATUS_LENGTH];
RRQ	request;
RTR	transaction;
RDB	rdb;
UCHAR	*p, *buffer;
USHORT	l, length, number, level;

RRQ_GET_HANDLE (request);
RTR_GET_HANDLE (transaction);
number = GET_WORD;
length = GET_WORD;
buffer = get_buffer (length, 0);
GET_STRING (length, buffer);
level = GET_WORD;

GDS_START_AND_SEND (status_vector, 
	GDS_REF (request->rrq_handle),
	GDS_REF (transaction->rtr_handle),
	number, 
	length, 
	GDS_VAL (buffer),
	level);

(void) send_response (status_vector);
}

static void start_transaction (void)
{
/**************************************
 *
 *	s t a r t _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Start a transaction.
 *
 **************************************/
RDB	rdb;
RTR	transaction;
USHORT	l, length, count, c, object;
UCHAR	*p, *buffer;
HANDLE	handle;
int	**v, *vector [3*16];
STATUS	status_vector[ISC_STATUS_LENGTH];

v = vector;
count = GET_WORD;
buffer = get_buffer (1024, 0);

for (c = 0; c < count; c++)
    {
    RDB_GET_HANDLE (rdb);
    *v++ = (int*) &rdb->rdb_handle;
    length = GET_WORD;
    *v++ = (int*) length;
    *v++ = (int*) buffer;
    GET_STRING (length, buffer);
    buffer += length;
    }

handle = NULL;

GDS_START_MULTIPLE (status_vector, 
	GDS_REF (handle), 
	count, 
	vector);

if (!send_response (status_vector))
    {
    transaction = make_transaction (rdb, handle);
    PUT_HANDLE (transaction);
    }
}

static void transact_request (void)
{
/**************************************
 *
 *	t r a n s a c t _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
RDB	rdb;
RTR	transaction;
USHORT	i, l, blr_length, in_msg_length, out_msg_length;
UCHAR	*p, *blr, *in_buffer, *out_buffer;
STATUS	status_vector[ISC_STATUS_LENGTH];

RDB_GET_HANDLE (rdb);
RTR_GET_HANDLE (transaction);
blr_length = GET_WORD;
blr = get_buffer (blr_length, 0);
GET_STRING (blr_length, blr);
in_msg_length = GET_WORD;
in_buffer = get_buffer (in_msg_length, 1);
GET_STRING (in_msg_length, in_buffer);
out_msg_length = GET_WORD;
out_buffer = get_buffer (out_msg_length, 2);

GDS_TRANSACT_REQUEST (status_vector, 
	&rdb->rdb_handle, 
	&transaction->rtr_handle,
	blr_length, blr,
	in_msg_length, in_buffer,
	out_msg_length, out_buffer);

if (send_response (status_vector))
    return;

PUT_STRING (out_msg_length, out_buffer);
}

static void unwind (void)
{
/**************************************
 *
 *	u n w i n d
 *
 **************************************
 *
 * Functional description
 *	Unwind a request.
 *
 **************************************/
RDB	rdb;
RRQ	request;
USHORT	level;
STATUS	status_vector[ISC_STATUS_LENGTH];

RRQ_GET_HANDLE (request);
level = GET_WORD;

GDS_UNWIND (status_vector, 
	GDS_REF (request->rrq_handle),
	level);

(void) send_response (status_vector);
}
