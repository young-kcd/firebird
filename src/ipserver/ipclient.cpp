/*
 *      PROGRAM:        Interprocess Client
 *      MODULE:         ipclient.c
 *      DESCRIPTION:    Interprocess interface
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
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <stdlib.h>
#include "../jrd/common.h"
#include <stdarg.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <windows.h>

#include "../ipserver/ipc.h"
#include "gen/codes.h"
#include "../jrd/license.h"
#include "../jrd/inf.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/db_alias.h"
#include "../ipserver/alli_proto.h"
#include "../ipserver/ipcli_proto.h"
#include "../ipserver/ipevt_proto.h"
#include "../ipserver/ips.h"
#include "../jrd/thd_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_f_proto.h"
#include "../jrd/sch_proto.h"
#include "../remote/os/win32/window.h"
#include "../common/config/config.h"
#define NO_PORT
#include "../remote/xnet_proto.h"
#undef NO_PORT

#define statistics      stat

extern "C" {

typedef void (*PTR) ();

static ISC_STATUS check_response(ICC, ISC_STATUS *);
static void clear_statement_cache(IPSERVER_ISR);
static void clear_transaction_statements(ITR);
static void event_packer(ICC);
static void event_thread(void);
static void extract_status(ICC, ISC_STATUS *);
static ISC_STATUS handle_error(ISC_STATUS *, ISC_STATUS);
static SSHORT init(ISC_STATUS *, ICC *);
static ITR make_transaction(ISC_STATUS *, IDB, FRBRD *);
static SSHORT name_length(TEXT *);
static BOOL pack_strings(ICC);
static void release_blob(IBL);
static void release_database(IDB);
static void release_event(IVNT);
static void release_request(IRQ);
static void release_sql_request(IPSERVER_ISR);
static void release_transaction(ITR);
static BOOL send_and_wait(ICC);
static void server_shutdown(void);
static void server_watcher(void);
static BOOL unpack_strings(ICC);


#define RETURN_ERROR(e)         { THD_mutex_unlock( &mapsect);\
				  if ( icc != NULL) {\
                                    if ( icc->icc_flags & ICCF_UNMAP_CLIENT)\
                                    {\
                                        server_shutdown();\
                                        icc->icc_flags &= ~ICCF_UNMAP_CLIENT;\
                                    }\
				  }\
                                  return e; }

#define RETURN_SUCCESS          RETURN_ERROR( FB_SUCCESS)

#define CHECK_HANDLE(blk, type, error)\
		if ( !blk || ((BLK)blk)->blk_type != (SCHAR) type) \
		    return handle_error( user_status, error)

#define CHECK_HANDLE_REL(blk, type, error)\
		if ( !blk || ((BLK)blk)->blk_type != (SCHAR) type) \
                {\
                    RETURN_ERROR( handle_error( user_status, error));\
                }

#define NULL_CHECK(ptr, code)	if (*ptr)\
				    return handle_error( user_status, code)

#define NOT_NULL(ptr)	if (!ptr)\
                        {\
                            RETURN_ERROR( handle_error( user_status, isc_virmemexh));\
                        }

#define GET_OBJECT(obj) icc = (ICC)obj;\
                        if ( icc->icc_thread_handle != GetCurrentThreadId() &&\
                             event_icc)\
                            icc = event_icc;\
                        if ( icc->icc_flags & ICCF_SERVER_SHUTDOWN)\
                        {\
                            user_status[0] = isc_arg_gds;\
                            user_status[1] = isc_lost_db_connection;\
                            user_status[2] = isc_arg_end;\
                            return user_status[1];\
                        }\
                        THD_mutex_lock( &mapsect);\
                        comm = (ips_comm_area*)icc->icc_mapped_addr;\
                        for ( commi = 0; commi < MAX_IPS_STRINGS; commi++)\
                            comm->ips_buffers[commi].ips_flags = 0;

/* some macros to set up the strings */

#define IPS_CLIENT(c,p,n,a,s,f) { c->ips_buffers[n].ips_cl_size = s;\
				  c->ips_buffers[n].ips_cl_addr = (UCHAR*)a;\
				  c->ips_buffers[n].ips_cl_copied = 0;\
				  c->ips_buffers[n].ips_flags = f;\
				  p = &c->ips_buffers[n];}

#define IPS_C_IN(c,p,n,a,s)     IPS_CLIENT(c,p,n,a,s,IPS_INPUT_BUFFER)
#define IPS_C_OUT(c,p,n,a,s)    IPS_CLIENT(c,p,n,a,s,IPS_OUTPUT_BUFFER)
#define IPS_C_IN_OUT(c,p,n,a,s) IPS_CLIENT(c,p,n,a,s,IPS_INPUT_BUFFER+IPS_OUTPUT_BUFFER)

static TEXT error_text[1024];
static ICC client_threads = NULL;
static IPM client_maps = NULL;
static MUTX_T clisect;
static MUTX_T evtsect;
static MUTX_T mapsect;
static LONG interlock = -1;
static BOOLEAN initialized = FALSE;
static SLONG eventID = 0;
static HANDLE event_semaphore = 0;
static HWND event_window = 0;
static ICC event_icc = NULL;
static HANDLE event_packer_handle = 0;
static HANDLE event_thread_handle = 0;
static EVENTQ event_head = NULL, event_tail = NULL;
static USHORT exit_flag = 0;
static USHORT pages_per_user = IPS_DEF_PAGES_PER_CLI;
static USHORT users_per_map = IPS_DEF_NUM_CLI;
static HANDLE server_watcher_handle = 0;
static HANDLE server_process_handle = 0;


#define GDS_ATTACH_DATABASE     IPI_attach_database
#define GDS_BLOB_INFO           IPI_blob_info
#define GDS_CANCEL_BLOB         IPI_cancel_blob
#define GDS_CANCEL_EVENTS       IPI_cancel_events
#define GDS_CLOSE_BLOB          IPI_close_blob
#define GDS_COMMIT              IPI_commit_transaction
#define GDS_COMMIT_RETAINING    IPI_commit_retaining
#define GDS_COMPILE             IPI_compile_request
#define GDS_CREATE_BLOB         IPI_create_blob
#define GDS_CREATE_BLOB2        IPI_create_blob2
#define GDS_CREATE_DATABASE     IPI_create_database
#define GDS_DATABASE_INFO       IPI_database_info
#define GDS_DDL                 IPI_ddl
#define GDS_DETACH              IPI_detach_database
#define GDS_DROP_DATABASE       IPI_drop_database
#define GDS_GET_SEGMENT         IPI_get_segment
#define GDS_GET_SLICE           IPI_get_slice
#define GDS_OPEN_BLOB           IPI_open_blob
#define GDS_OPEN_BLOB2          IPI_open_blob2
#define GDS_PREPARE             IPI_prepare_transaction
#define GDS_PUT_SEGMENT         IPI_put_segment
#define GDS_PUT_SLICE           IPI_put_slice
#define GDS_QUE_EVENTS          IPI_que_events
#define GDS_RECEIVE             IPI_receive
#define GDS_RECONNECT           IPI_reconnect_transaction
#define GDS_RELEASE_REQUEST     IPI_release_request
#define GDS_REQUEST_INFO        IPI_request_info
#define GDS_ROLLBACK            IPI_rollback_transaction
#define GDS_ROLLBACK_RETAINING  IPI_rollback_retaining
#define GDS_SEEK_BLOB           IPI_seek_blob
#define GDS_SEND                IPI_send
#define GDS_SERVICE_ATTACH      IPI_service_attach
#define GDS_SERVICE_DETACH      IPI_service_detach
#define GDS_SERVICE_QUERY       IPI_service_query
#define GDS_SERVICE_START       IPI_service_start
#define GDS_START               IPI_start_request
#define GDS_START_AND_SEND      IPI_start_and_send
#define GDS_START_MULTIPLE      IPI_start_multiple
#define GDS_START_TRANSACTION   IPI_start_transaction
#define GDS_TRANSACTION_INFO    IPI_transaction_info
#define GDS_TRANSACT_REQUEST    IPI_transact_request
#define GDS_UNWIND              IPI_unwind_request

#define GDS_DSQL_ALLOCATE       IPI_allocate_statement
#define GDS_DSQL_EXECUTE        IPI_execute
#define GDS_DSQL_EXECUTE2       IPI_execute2
#define GDS_DSQL_EXECUTE_IMMED  IPI_execute_immediate
#define GDS_DSQL_EXECUTE_IMMED2 IPI_execute_immediate2
#define GDS_DSQL_FETCH          IPI_fetch
#define GDS_DSQL_FREE           IPI_free_statement
#define GDS_DSQL_INSERT         IPI_insert
#define GDS_DSQL_PREPARE        IPI_prepare
#define GDS_DSQL_SET_CURSOR     IPI_set_cursor_name
#define GDS_DSQL_SQL_INFO       IPI_sql_info


ISC_STATUS GDS_ATTACH_DATABASE(
						   ISC_STATUS * user_status,
						   USHORT GDS_VAL(file_length),
						   SCHAR * file_name,
IDB * handle,
USHORT GDS_VAL(dpb_length), SCHAR * dpb, SCHAR * expanded_filename)
{
/**************************************
 *
 *      g d s _ a t t a c h _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *      Attach to an existing database via the interprocess
 *      interface to the local server.
 *
 **************************************/
	IDB idb;
	ICC icc;
	SSHORT l;
	ips_object *ips;
	ips_string *ips_name;
	ips_string *ips_dpb;
	ips_string *ips_expanded;
	ips_comm_area *comm;
	USHORT commi;


	/* make sure handle is valid (should be null coming in) */

	NULL_CHECK(handle, isc_bad_db_handle);
	if (!(l = GDS_VAL(file_length)))
		l = strlen(file_name);

	/* remote names should not go through here */

	if (ISC_check_if_remote(expanded_filename, TRUE)) {
		user_status[0] = isc_arg_gds;
		user_status[1] = isc_unavailable;
		user_status[2] = isc_arg_end;
		return user_status[1];
	}

	/* call init to initiate connection */

	if (!init(user_status, &icc))
		return user_status[1];

	/* set up communications area */

	THD_mutex_lock(&mapsect);
	comm = (ips_comm_area *) icc->icc_mapped_addr;
	for (commi = 0; commi < MAX_IPS_STRINGS; commi++)
		comm->ips_buffers[commi].ips_flags = 0;
	comm->ips_operation = op_attach;
	ips = &comm->ips_operations.ips_op_object;
	char temp[MAXPATHLEN];
	if (ResolveDatabaseAlias(file_name, temp)) {
		IPS_C_IN(comm, ips_name, IPS_ATTACH_NAME, file_name, l);
	} else {
		IPS_C_IN(comm, ips_name, IPS_ATTACH_NAME, 
				 expanded_filename, strlen(expanded_filename));
	}
	IPS_C_IN(comm, ips_dpb, IPS_ATTACH_DPB, dpb, dpb_length);
	IPS_C_IN(comm, ips_expanded, IPS_ATTACH_EXPANDED,
			 expanded_filename, strlen(expanded_filename));

	/* send request and wait for response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);

	/* allocate a database structure and link it to thread */

	idb = (IDB) ALLOC(type_idb);
	*handle = idb;
	NOT_NULL(idb);
	idb->idb_flags = IDBF_DATABASE_ATTACHMENT;
	idb->idb_next = icc->icc_databases;
	idb->idb_thread = icc;
	icc->icc_databases = idb;
	idb->idb_handle = (FRBRD *) (ips->ips_handle);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_BLOB_INFO(ISC_STATUS * user_status,
					 IBL * blob,
					 USHORT GDS_VAL(item_length),
					 UCHAR * items,
					 USHORT GDS_VAL(buffer_length), UCHAR * buffer)
{
/**************************************
 *
 *      g d s _ b l o b _ i n f o
 *
 **************************************
 *
 * Functional description
 *      Provide information on blob object.
 *
 **************************************/
	IDB idb;
	ips_object *ips;
	ips_string *ips_items;
	ips_string *ips_data;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	CHECK_HANDLE((*blob), type_ibl, isc_bad_segstr_handle);
	idb = (*blob)->ibl_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* set up communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_info_blob;
	ips = &comm->ips_operations.ips_op_object;
	ips->ips_handle = (UCHAR *) ((*blob)->ibl_handle);
	IPS_C_IN(comm, ips_items, IPS_INFO_ITEMS, items, item_length);
	IPS_C_OUT(comm, ips_data, IPS_INFO_DATA, buffer, buffer_length);

	/* send request and wait for response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_CANCEL_BLOB(ISC_STATUS * user_status, IBL * blob_handle)
{
/**************************************
 *
 *      g d s _ c a n c e l _ b l o b
 *
 **************************************
 *
 * Functional description
 *      Abort a partially completed blob.
 *
 **************************************/
	IDB idb;
	IBL blob;
	ips_object *ips;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	blob = *blob_handle;
	CHECK_HANDLE(blob, type_ibl, isc_bad_segstr_handle);
	idb = blob->ibl_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* if blob handle is null, we're done */

	blob = *blob_handle;
	if (!blob) {
		*user_status++ = isc_arg_gds;
		*user_status++ = FB_SUCCESS;
		*user_status = isc_arg_end;
		return FB_SUCCESS;
	}

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_cancel_blob;
	ips = &comm->ips_operations.ips_op_object;
	ips->ips_handle = (UCHAR *) (blob->ibl_handle);

	/* free object */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	release_blob(blob);
	*blob_handle = NULL;
	RETURN_SUCCESS;
}


ISC_STATUS GDS_CANCEL_EVENTS(ISC_STATUS * user_status, IDB * handle, SLONG * id)
{
/**************************************
 *
 *      g d s _ c a n c e l _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *      Try to cancel an event.
 *
 **************************************/
	IDB idb;
	IVNT event;
	ISC_STATUS result;
	ips_object *ips;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handle */

	idb = *handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_cancel_events;
	ips = &comm->ips_operations.ips_op_object;
	ips->ips_handle = (UCHAR *) (idb->idb_handle);
	ips->ips_parameter = (ULONG) (*id);

	/* send request and get response */

	result = check_response(icc, user_status);

	/* find and neutralize event */

	for (event = idb->idb_events; event; event = event->ivnt_next)
		if (event->ivnt_handle == *id)
			break;
	if (event)
		event->ivnt_id = 0;

	/* send response */

	if (result)
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_CLOSE_BLOB(ISC_STATUS * user_status, IBL * blob_handle)
{
/**************************************
 *
 *      g d s _ c l o s e _ b l o b
 *
 **************************************
 *
 * Functional description
 *      Abort a partially completed blob.
 *
 **************************************/
	IDB idb;
	IBL blob;
	ips_object *ips;
	ips_string *ips_seg;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	blob = *blob_handle;
	CHECK_HANDLE(blob, type_ibl, isc_bad_segstr_handle);
	idb = blob->ibl_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_close_blob;
	ips = &comm->ips_operations.ips_op_object;
	ips->ips_handle = (UCHAR *) (blob->ibl_handle);

	/* send any unsent portion of the blob */

	if ((blob->ibl_flags & IBL_create) && blob->ibl_ptr != blob->ibl_buffer) {
		IPS_C_IN(comm, ips_seg, IPS_CLOSE_BLOB_REM, blob->ibl_buffer,
				 (blob->ibl_ptr - blob->ibl_buffer));
	}

	/* send request, release blob object and resources */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	release_blob(blob);
	*blob_handle = NULL;
	RETURN_SUCCESS;
}


ISC_STATUS GDS_COMMIT(ISC_STATUS * user_status, ITR * itr_handle)
{
/**************************************
 *
 *      g d s _ c o m m i t
 *
 **************************************
 *
 * Functional description
 *      Commit a transaction.
 *
 **************************************/
	IDB idb;
	ITR transaction;
	ips_object *ips;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	transaction = *itr_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
	idb = (*itr_handle)->itr_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_commit;
	ips = &comm->ips_operations.ips_op_object;
	ips->ips_handle = (UCHAR *) (transaction->itr_handle);

	/* send request, release object and resources */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	clear_transaction_statements(transaction);
	release_transaction(transaction);
	*itr_handle = NULL;
	RETURN_SUCCESS;
}


ISC_STATUS GDS_COMMIT_RETAINING(ISC_STATUS * user_status, ITR * itr_handle)
{
/**************************************
 *
 *      g d s _ c o m m i t _ r e t a i n i n g
 *
 **************************************
 *
 * Functional description
 *      Prepare a transaction for commit.  First phase of a two
 *      phase commit.
 *
 **************************************/
	IDB idb;
	ITR transaction;
	ips_object *ips;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	transaction = *itr_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
	idb = (*itr_handle)->itr_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_commit_retaining;
	ips = &comm->ips_operations.ips_op_object;
	ips->ips_handle = (UCHAR *) (transaction->itr_handle);

	/* send request */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_COMPILE(ISC_STATUS * user_status,
				   IDB * db_handle,
				   IRQ * req_handle, USHORT GDS_VAL(blr_length), UCHAR * blr)
{
/**************************************
 *
 *      g d s _ c o m p i l e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	IDB idb;
	IRQ request;
	ips_compile_req *ips;
	ips_string *ips_blr;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	NULL_CHECK(req_handle, isc_bad_req_handle);
	idb = *db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_compile;
	ips = &comm->ips_operations.ips_op_compile;
	ips->ips_db_handle = (UCHAR *) (idb->idb_handle);
	IPS_C_IN(comm, ips_blr, IPS_COMPILE_REQ, blr, blr_length);

	/* send request and wait for response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);

	/* allocate request block */

	request = (IRQ) ALLOC(type_irq);
	*req_handle = request;
	NOT_NULL(request);
	request->irq_handle = (FRBRD *) ips->ips_rq_handle;
	request->irq_idb = idb;
	request->irq_next = idb->idb_requests;
	idb->idb_requests = request;
	RETURN_SUCCESS;
}


ISC_STATUS GDS_CREATE_BLOB(ISC_STATUS * user_status,
					   IDB * db_handle,
					   ITR * itr_handle, IBL * blob_handle, BID blob_id)
{
/**************************************
 *
 *      g d s _ c r e a t e _ b l o b
 *
 **************************************
 *
 * Functional description
 *      Create a brand new blob.
 *
 **************************************/
	IDB idb;
	ITR transaction;
	IBL blob;
	ips_blob *ips;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	NULL_CHECK(blob_handle, isc_bad_segstr_handle);
	idb = *db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	transaction = *itr_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_create_blob;
	ips = &comm->ips_operations.ips_op_blob;
	ips->ips_db_handle = (UCHAR *) (idb->idb_handle);
	ips->ips_tr_handle = (UCHAR *) (transaction->itr_handle);

	/* send request and wait for response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);

	/* allocate and fill blob object */

	blob = (IBL) ALLOCV(type_ibl, BLOB_LENGTH);
	*blob_handle = blob;
	NOT_NULL(blob);
	blob->ibl_handle = (FRBRD *) ips->ips_bl_handle;
	blob->ibl_buffer_length = BLOB_LENGTH;
	blob->ibl_idb = idb;
	blob->ibl_itr = transaction;
	blob->ibl_ptr = blob->ibl_buffer;
	blob->ibl_flags |= IBL_create;
	blob->ibl_next = transaction->itr_blobs;
	transaction->itr_blobs = blob;
	blob_id->bid_relation_id = ips->ips_rel_id;
	blob_id->bid_number = ips->ips_bid_number;
	RETURN_SUCCESS;
}


ISC_STATUS GDS_CREATE_BLOB2(ISC_STATUS * user_status,
						IDB * db_handle,
						ITR * itr_handle,
						IBL * blob_handle,
						BID blob_id, USHORT GDS_VAL(bpb_length), UCHAR * bpb)
{
/**************************************
 *
 *      g d s _ c r e a t e _ b l o b 2
 *
 **************************************
 *
 * Functional description
 *      Create a brand new blob.
 *
 **************************************/
	IDB idb;
	ITR transaction;
	IBL blob;
	ips_blob *ips;
	ips_string *ips_bpb;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	NULL_CHECK(blob_handle, isc_bad_segstr_handle);
	idb = *db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	transaction = *itr_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_create_blob2;
	ips = &comm->ips_operations.ips_op_blob;
	ips->ips_db_handle = (UCHAR *) (idb->idb_handle);
	ips->ips_tr_handle = (UCHAR *) (transaction->itr_handle);
	IPS_C_IN(comm, ips_bpb, IPS_BLOB_BPB, bpb, bpb_length);

	/* send request and wait for response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);

	/* allocate and fill blob object */

	blob = (IBL) ALLOCV(type_ibl, BLOB_LENGTH);
	*blob_handle = blob;
	NOT_NULL(blob);
	blob->ibl_handle = (FRBRD *) ips->ips_bl_handle;
	blob->ibl_buffer_length = BLOB_LENGTH;
	blob->ibl_idb = idb;
	blob->ibl_itr = transaction;
	blob->ibl_ptr = blob->ibl_buffer;
	blob->ibl_flags |= IBL_create;
	blob->ibl_next = transaction->itr_blobs;
	transaction->itr_blobs = blob;
	blob_id->bid_relation_id = ips->ips_rel_id;
	blob_id->bid_number = ips->ips_bid_number;
	RETURN_SUCCESS;
}


ISC_STATUS GDS_CREATE_DATABASE(ISC_STATUS * user_status,
						   USHORT GDS_VAL(file_length),
						   SCHAR * file_name,
						   IDB * handle,
						   USHORT GDS_VAL(dpb_length),
						   SCHAR * dpb,
						   SSHORT GDS_VAL(db_type), SCHAR * expanded_filename)
{
/**************************************
 *
 *      g d s _ c r e a t e _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *      Create a database.
 *
 **************************************/
	IDB idb;
	ICC icc;
	USHORT l;
	ips_object *ips;
	ips_string *ips_name;
	ips_string *ips_dpb;
	ips_string *ips_expanded;
	ips_comm_area *comm;
	USHORT commi;


	/* handle must be null coming in */

	NULL_CHECK(handle, isc_bad_db_handle);
	if (!(l = GDS_VAL(file_length)))
		l = strlen(file_name);

	/* remote names should not go through here */

	if (ISC_check_if_remote(expanded_filename, TRUE)) {
		user_status[0] = isc_arg_gds;
		user_status[1] = isc_unavailable;
		user_status[2] = isc_arg_end;
		return user_status[1];
	}

	/* initiate a connection */

	if (!init(user_status, &icc))
		return user_status[1];

	/* point to communications area */

	GET_OBJECT(icc);
	comm->ips_operation = op_create;
	ips = &comm->ips_operations.ips_op_object;
	char temp[MAXPATHLEN];
	if (ResolveDatabaseAlias(file_name, temp)) {
		IPS_C_IN(comm, ips_name, IPS_CREATE_NAME, file_name, l);
	} else {
		IPS_C_IN(comm, ips_name, IPS_CREATE_NAME, 
				 expanded_filename, strlen(expanded_filename));
	}
	IPS_C_IN(comm, ips_dpb, IPS_CREATE_DPB, dpb, GDS_VAL(dpb_length));
	IPS_C_IN(comm, ips_expanded, IPS_CREATE_EXPANDED,
			 expanded_filename, strlen(expanded_filename));

	/* send request and wait for response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);

	/* allocate a database structure and link it to thread */

	idb = (IDB) ALLOC(type_idb);
	*handle = idb;
	NOT_NULL(idb);
	idb->idb_flags = IDBF_DATABASE_ATTACHMENT;
	idb->idb_next = icc->icc_databases;
	idb->idb_thread = icc;
	icc->icc_databases = idb;
	idb->idb_handle = (FRBRD *) (ips->ips_handle);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_DATABASE_INFO(ISC_STATUS * user_status,
						 IDB * handle,
						 USHORT GDS_VAL(item_length),
						 UCHAR * items,
						 USHORT GDS_VAL(buffer_length), UCHAR * buffer)
{
/**************************************
 *
 *      g d s _ d a t a b a s e _ i n f o
 *
 **************************************
 *
 * Functional description
 *      Provide information on database object.
 *
 **************************************/
	IDB idb;
	ips_object *ips;
	ips_string *ips_items;
	ips_string *ips_data;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handle */

	idb = *handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_info_database;
	ips = &comm->ips_operations.ips_op_object;
	ips->ips_handle = (UCHAR *) (idb->idb_handle);
	IPS_C_IN(comm, ips_items, IPS_INFO_ITEMS, items, item_length);
	IPS_C_OUT(comm, ips_data, IPS_INFO_DATA, buffer, buffer_length);

	/* send request and wait for response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_DDL(ISC_STATUS * user_status,
			   IDB * db_handle,
			   ITR * itr_handle, USHORT GDS_VAL(length), UCHAR * msg)
{
/**************************************
 *
 *      g d s _ d d l
 *
 **************************************
 *
 * Functional description
 *      Pump thru a DDL call.
 *
 **************************************/
	ITR transaction;
	IDB idb;
	ips_ddl *ips;
	ips_string *ips_ddl_string;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	idb = *db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	transaction = *itr_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_ddl;
	ips = &comm->ips_operations.ips_op_ddl;
	ips->ips_db_handle = (UCHAR *) (idb->idb_handle);
	ips->ips_tr_handle = (UCHAR *) (transaction->itr_handle);
	IPS_C_IN(comm, ips_ddl_string, IPS_DDL_DDL, msg, length);

	/* send request and get response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_DETACH(ISC_STATUS * user_status, IDB * handle)
{
/**************************************
 *
 *      g d s _ d e t a c h  
 *
 **************************************
 *
 * Functional description
 *      Close down a database.
 *
 **************************************/
	ICC icc;
	IDB idb;
	IRQ irq;
	ips_object *ips;
	ips_comm_area *comm;
	USHORT commi;


	/* verify handle */

	idb = *handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	icc = idb->idb_thread;
	THD_mutex_lock(&mapsect);
	if (icc->icc_flags & ICCF_SERVER_SHUTDOWN) {

		/* if server was shut down, set up vector */

		user_status[0] = isc_arg_gds;
		user_status[1] = FB_SUCCESS;
		user_status[2] = isc_arg_end;
	}
	else {
		comm = (ips_comm_area *) icc->icc_mapped_addr;
		for (commi = 0; commi < MAX_IPS_STRINGS; commi++)
			comm->ips_buffers[commi].ips_flags = 0;
		comm->ips_operation = op_detach;
		ips = &comm->ips_operations.ips_op_object;
		ips->ips_handle = (UCHAR *) (idb->idb_handle);

		/* send request and release object */

		if (check_response(icc, user_status))
			if (user_status[1] != isc_lost_db_connection)
				RETURN_ERROR(user_status[1]);
	}

	/* release any event, request, sql request or transaction objects for this
	   database, and then free the data structure (events are protected by
	   a critical region to make sure there's no event trigger in progress) */

	THD_mutex_lock(&clisect);
	while (idb->idb_events)
		release_event(idb->idb_events);
	THD_mutex_unlock(&clisect);
	while (irq = idb->idb_requests) {
		if (irq->irq_user_handle)
			*(irq->irq_user_handle) = NULL;
		release_request(irq);
	}
	while (idb->idb_sql_requests)
		release_sql_request(idb->idb_sql_requests);
	while (idb->idb_transactions)
		release_transaction(idb->idb_transactions);
	release_database(idb);
	*handle = NULL;
	/* 
	   icc is already freed as a part of idb and the
	   necessary cleanup thro server_shutdown has occured.
	 */
	icc = NULL;
	if (user_status[1])
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_DROP_DATABASE(ISC_STATUS * user_status, IDB * handle)
{
/**************************************
 *
 *      i s c _ d r o p _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *      Close down and purge a database.
 *
 **************************************/
	IDB idb;
	IRQ irq;
	ISC_STATUS code;
	ips_object *ips;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handle */

	idb = *handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_drop_database;
	ips = &comm->ips_operations.ips_op_object;
	ips->ips_handle = (UCHAR *) (idb->idb_handle);

	/* send request and release handle */

	code = check_response(icc, user_status);
	if (code && (code != isc_drdb_completed_with_errs))
		RETURN_ERROR(user_status[1]);

	/* release any event, request, sql request or transaction objects for this
	   database, and then free the data structure (events are protected by
	   a critical region to make sure there's no event trigger in progress) */

	THD_mutex_lock(&clisect);
	while (idb->idb_events)
		release_event(idb->idb_events);
	THD_mutex_unlock(&clisect);
	while (irq = idb->idb_requests) {
		if (irq->irq_user_handle)
			*(irq->irq_user_handle) = NULL;
		release_request(irq);
	}
	while (idb->idb_sql_requests)
		release_sql_request(idb->idb_sql_requests);
	while (idb->idb_transactions)
		release_transaction(idb->idb_transactions);
	release_database(idb);
	*handle = NULL;
	if (code)
		RETURN_ERROR(code);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_DSQL_ALLOCATE(ISC_STATUS * user_status,
						 IDB * db_handle, IPSERVER_ISR * stmt_handle)
{
/**************************************
 *
 *      d s q l _ a l l o c a t e _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *      Allocate a statement handle.
 *
 **************************************/
	IDB idb;
	IPSERVER_ISR statement;
	ips_dsql *ips;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	NULL_CHECK(stmt_handle, isc_bad_req_handle);
	idb = *db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_allocate_stmt;
	ips = &comm->ips_operations.ips_op_dsql;
	ips->ips_db_handle = (UCHAR *) (idb->idb_handle);

	/* send request and wait for response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);

	/* allocate and fill SQL request block */

	statement = (IPSERVER_ISR) ALLOC(type_ipserver_isr);
	*stmt_handle = statement;
	NOT_NULL(statement);
	statement->isr_handle = (FRBRD *) ips->ips_st_handle;
	statement->isr_idb = idb;
	statement->isr_next = idb->idb_sql_requests;
	statement->isr_batch_flag = 0;
	statement->isr_rec_count = 0;
	statement->isr_max_recs = 0;
	statement->isr_eof_flag = FALSE;
	statement->isr_packed = NULL;
	idb->idb_sql_requests = statement;
	RETURN_SUCCESS;
}


ISC_STATUS GDS_DSQL_EXECUTE(ISC_STATUS * user_status,
						ITR * itr_handle,
						IPSERVER_ISR * stmt_handle,
						USHORT blr_length,
						UCHAR * blr,
						USHORT msg_type, USHORT msg_length, UCHAR * msg)
{
/**************************************
 *
 *      d s q l _ e x e c u t e
 *
 **************************************
 *
 * Functional description
 *      Execute a dynamic SQL statement.
 *
 **************************************/
	IDB idb;
	ITR transaction;
	IPSERVER_ISR statement;
	FRBRD * handle;
	ips_dsql *ips;
	ips_string *ips_blr;
	ips_string *ips_msg;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	if (transaction = *itr_handle)
		CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
	statement = *stmt_handle;
	CHECK_HANDLE(statement, type_ipserver_isr, isc_bad_req_handle);
	idb = statement->isr_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_execute;
	ips = &comm->ips_operations.ips_op_dsql;
	if (transaction)
		ips->ips_tr_handle = (UCHAR *) (transaction->itr_handle);
	else
		ips->ips_tr_handle = NULL;
	ips->ips_st_handle = (UCHAR *) (statement->isr_handle);
	ips->ips_msg_type = msg_type;
	IPS_C_IN(comm, ips_blr, IPS_DSQL_EXEC_BLR, blr, blr_length);
	IPS_C_IN(comm, ips_msg, IPS_DSQL_EXEC_MSG, msg, msg_length);

	/* send request and wait for response */

	(void) check_response(icc, user_status);

	/* take care of transactions */

	handle = (FRBRD *) ips->ips_tr_handle;
	if (transaction && !handle) {
		release_transaction(transaction);
		*itr_handle = NULL;
	}
	else if (!transaction && handle)
		*itr_handle = make_transaction(user_status, idb, handle);
	if (user_status[1])
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_DSQL_EXECUTE2(ISC_STATUS * user_status,
						 ITR * itr_handle,
						 IPSERVER_ISR * stmt_handle,
						 USHORT in_blr_length,
						 UCHAR * in_blr,
						 USHORT in_msg_type,
						 USHORT in_msg_length,
						 UCHAR * in_msg,
						 USHORT out_blr_length,
						 UCHAR * out_blr,
						 USHORT out_msg_type,
						 USHORT out_msg_length, UCHAR * out_msg)
{
/**************************************
 *
 *      d s q l _ e x e c u t e 2
 *
 **************************************
 *
 * Functional description
 *      Execute a dynamic SQL statement.
 *
 **************************************/
	IDB idb;
	ITR transaction;
	IPSERVER_ISR statement;
	FRBRD * handle;
	ips_dsql *ips;
	ips_string *ips_blr_in;
	ips_string *ips_blr_out;
	ips_string *ips_msg_in;
	ips_string *ips_msg_out;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	if (transaction = *itr_handle)
		CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
	statement = *stmt_handle;
	CHECK_HANDLE(statement, type_ipserver_isr, isc_bad_req_handle);
	idb = statement->isr_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_execute2;
	ips = &comm->ips_operations.ips_op_dsql;
	if (transaction)
		ips->ips_tr_handle = (UCHAR *) (transaction->itr_handle);
	else
		ips->ips_tr_handle = NULL;
	ips->ips_st_handle = (UCHAR *) (statement->isr_handle);
	ips->ips_msg_type = in_msg_type;
	ips->ips_msg_out = out_msg_type;
	IPS_C_IN(comm, ips_blr_in, IPS_DSQL_EXEC2_BLR_IN, in_blr, in_blr_length);
	IPS_C_IN(comm, ips_blr_out, IPS_DSQL_EXEC2_BLR_OUT, out_blr,
			 out_blr_length);
	IPS_C_IN(comm, ips_msg_in, IPS_DSQL_EXEC2_MSG_IN, in_msg, in_msg_length);
	IPS_C_OUT(comm, ips_msg_out, IPS_DSQL_EXEC2_MSG_OUT, out_msg,
			  out_msg_length);

	/* send request and wait for response */

	(void) check_response(icc, user_status);

	/* take care of transactions */

	handle = (FRBRD *) ips->ips_tr_handle;
	if (transaction && !handle) {
		release_transaction(transaction);
		*itr_handle = NULL;
	}
	else if (!transaction && handle)
		*itr_handle = make_transaction(user_status, idb, handle);
	statement->isr_itr = *itr_handle;
	if (user_status[1])
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_DSQL_EXECUTE_IMMED(ISC_STATUS * user_status,
							  IDB * db_handle,
							  ITR * itr_handle,
							  USHORT length,
							  UCHAR * string,
							  USHORT dialect,
							  USHORT blr_length,
							  UCHAR * blr,
							  USHORT msg_type, USHORT msg_length, UCHAR * msg)
{
/**************************************
 *
 *      d s q l _ e x e c u t e _ i m m e d i a t e 
 *
 **************************************
 *
 * Functional description
 *      Prepare and execute a statement.
 *
 **************************************/
	IDB idb;
	ITR transaction;
	FRBRD * handle;
	ips_dsql *ips;
	ips_string *ips_blr;
	ips_string *ips_msg;
	ips_string *ips_sql;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	idb = *db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	if (transaction = *itr_handle)
		CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
	if (!length)
		length = strlen(reinterpret_cast < char *>(string));

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_exec_immediate;
	ips = &comm->ips_operations.ips_op_dsql;
	ips->ips_db_handle = (UCHAR *) (idb->idb_handle);
	if (transaction)
		ips->ips_tr_handle = (UCHAR *) (transaction->itr_handle);
	else
		ips->ips_tr_handle = NULL;
	ips->ips_msg_type = msg_type;
	ips->ips_parameter = dialect;
	IPS_C_IN(comm, ips_blr, IPS_DSQL_EXEC_IMMED_BLR, blr, blr_length);
	IPS_C_IN(comm, ips_msg, IPS_DSQL_EXEC_IMMED_MSG, msg, msg_length);
	IPS_C_IN(comm, ips_sql, IPS_DSQL_EXEC_IMMED_SQL, string, length);

	/* send request and get response */

	(void) check_response(icc, user_status);

	/* take care of transacion handles */

	handle = (FRBRD *) ips->ips_tr_handle;
	if (transaction && !handle) {
		release_transaction(transaction);
		*itr_handle = NULL;
	}
	else if (!transaction && handle)
		*itr_handle = make_transaction(user_status, idb, handle);
	if (user_status[1])
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_DSQL_EXECUTE_IMMED2(ISC_STATUS * user_status,
							   IDB * db_handle,
							   ITR * itr_handle,
							   USHORT length,
							   UCHAR * string,
							   USHORT dialect,
							   USHORT in_blr_length,
							   UCHAR * in_blr,
							   USHORT in_msg_type,
							   USHORT in_msg_length,
							   UCHAR * in_msg,
							   USHORT out_blr_length,
							   UCHAR * out_blr,
							   USHORT out_msg_type,
							   USHORT out_msg_length, UCHAR * out_msg)
{
/**************************************
 *
 *      d s q l _ e x e c u t e _ i m m e d i a t e 2
 *
 **************************************
 *
 * Functional description
 *      Prepare and execute a statement.
 *
 **************************************/
	IDB idb;
	ITR transaction;
	FRBRD * handle;
	ips_dsql *ips;
	ips_string *ips_blr_in;
	ips_string *ips_msg_in;
	ips_string *ips_blr_out;
	ips_string *ips_msg_out;
	ips_string *ips_sql;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	idb = *db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	if (transaction = *itr_handle)
		CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
	if (!length)
		length = strlen(reinterpret_cast < char *>(string));

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_exec_immediate2;
	ips = &comm->ips_operations.ips_op_dsql;
	ips->ips_db_handle = (UCHAR *) (idb->idb_handle);
	if (transaction)
		ips->ips_tr_handle = (UCHAR *) (transaction->itr_handle);
	else
		ips->ips_tr_handle = NULL;
	ips->ips_msg_type = in_msg_type;
	ips->ips_msg_out = out_msg_type;
	ips->ips_parameter = dialect;
	IPS_C_IN(comm, ips_sql, IPS_DSQL_EXEC_IMMED2_SQL, string, length);
	IPS_C_IN(comm, ips_blr_in, IPS_DSQL_EXEC_IMMED2_BLR_IN, in_blr,
			 in_blr_length);
	IPS_C_IN(comm, ips_msg_in, IPS_DSQL_EXEC_IMMED2_MSG_IN, in_msg,
			 in_msg_length);
	IPS_C_IN(comm, ips_blr_out, IPS_DSQL_EXEC_IMMED2_BLR_OUT, out_blr,
			 out_blr_length);
	IPS_C_OUT(comm, ips_msg_out, IPS_DSQL_EXEC_IMMED2_MSG_OUT, out_msg,
			  out_msg_length);

	/* send request and get answer */

	(void) check_response(icc, user_status);

	/* handle transactions */

	handle = (FRBRD *) ips->ips_tr_handle;
	if (transaction && !handle) {
		release_transaction(transaction);
		*itr_handle = NULL;
	}
	else if (!transaction && handle)
		*itr_handle = make_transaction(user_status, idb, handle);
	if (user_status[1])
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_DSQL_FETCH(ISC_STATUS * user_status,
					  IPSERVER_ISR * stmt_handle,
					  USHORT blr_length,
					  UCHAR * blr,
					  USHORT msg_type, USHORT msg_length, UCHAR * msg)
{
/**************************************
 *                  
 *      d s q l _ f e t c h
 *
 **************************************
 *
 * Functional description
 *      Fetch next record from a dynamic SQL cursor.
 *
 **************************************/
	IDB idb;
	IPSERVER_ISR statement;
	USHORT packed_length;
	UCHAR *packed_buffer;
	USHORT i;
	ips_dsql *ips;
	ips_string *ips_blr;
	ips_string *ips_msg;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	statement = *stmt_handle;
	CHECK_HANDLE(statement, type_ipserver_isr, isc_bad_req_handle);
	idb = statement->isr_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	icc = idb->idb_thread;

	/* check for connection lost */

	if (icc->icc_flags & ICCF_SERVER_SHUTDOWN) {
		user_status[0] = isc_arg_gds;
		user_status[1] = isc_lost_db_connection;
		user_status[2] = isc_arg_end;
		return user_status[1];
	}

	/* calculate how many records to pack */

	THD_mutex_lock(&mapsect);
	if (!statement->isr_max_recs) {
		statement->isr_max_recs =
			(USHORT) (IPS_USEFUL_SPACE(pages_per_user) / msg_length);
		if (!statement->isr_max_recs || !statement->isr_batch_flag)
			statement->isr_max_recs = 1;
		else {
			statement->isr_packed =
				(UCHAR *) ALLI_alloc(msg_length * statement->isr_max_recs);
			NOT_NULL(statement->isr_packed);
			statement->isr_rec_count = 0;
			statement->isr_cursor = statement->isr_packed;
		}
	}

	/* if there are any packed records, return the next one */

	user_status[0] = isc_arg_gds;
	user_status[1] = FB_SUCCESS;
	user_status[2] = isc_arg_end;
	if (statement->isr_max_recs > 1) {
		if (statement->isr_rec_count) {
			memcpy(msg, statement->isr_cursor, msg_length);
			statement->isr_rec_count--;
			statement->isr_cursor += msg_length;
			RETURN_SUCCESS;
		}
		else if (statement->isr_eof_flag) {
			statement->isr_eof_flag = FALSE;
			RETURN_ERROR(100);
		}
		else if (statement->isr_status[1]) {
			for (i = 0; i < ISC_STATUS_LENGTH && statement->isr_status[i];
				 i++) {
				user_status[i] = statement->isr_status[i];
				statement->isr_status[i] = 0;
			}

			RETURN_ERROR(user_status[1]);
		}
	}
	THD_mutex_unlock(&mapsect);

	/* either there's no packing, or there's no more packed records */

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_fetch;
	ips = &comm->ips_operations.ips_op_dsql;
	ips->ips_st_handle = (UCHAR *) (statement->isr_handle);
	ips->ips_msg_type = msg_type;
	packed_length = msg_length * statement->isr_max_recs;
	if (statement->isr_max_recs == 1)
		packed_buffer = msg;
	else
		packed_buffer = statement->isr_packed;
	IPS_C_IN(comm, ips_blr, IPS_DSQL_FETCH_BLR, blr, blr_length);
	IPS_C_OUT(comm, ips_msg, IPS_DSQL_FETCH_MSG, packed_buffer,
			  packed_length);
	ips->ips_rec_count = statement->isr_max_recs;

	/* send request and get response */

	(void) check_response(icc, user_status);

	/* for packed records, extract the first */

	statement->isr_rec_count = ips->ips_rec_count;
	if (statement->isr_max_recs > 1 && statement->isr_rec_count) {

		/* get first record and save status vector */

		statement->isr_cursor = statement->isr_packed;
		memcpy(msg, statement->isr_cursor, msg_length);
		statement->isr_cursor += msg_length;
		for (i = 0; i < ISC_STATUS_LENGTH; i++)
			statement->isr_status[i] = user_status[i];
		if (ips->ips_parameter == 100)
			statement->isr_eof_flag = TRUE;
		else
			statement->isr_eof_flag = FALSE;
		ips->ips_parameter = 0;

		/* if there are more records, fake good return */

		if (statement->isr_rec_count) {
			user_status[0] = isc_arg_gds;
			user_status[1] = FB_SUCCESS;
			user_status[2] = isc_arg_end;
		}
		statement->isr_rec_count--;
	}
	if (ips->ips_parameter == 100)
		RETURN_ERROR(100);
	if (user_status[1])
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_DSQL_FREE(ISC_STATUS * user_status,
					 IPSERVER_ISR * stmt_handle, USHORT option)
{
/**************************************
 *
 *      d s q l _ f r e e _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *      Release request for a Dynamic SQL statement
 *
 **************************************/
	IDB idb;
	IPSERVER_ISR statement;
	ips_object *ips;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	statement = *stmt_handle;
	CHECK_HANDLE(statement, type_ipserver_isr, isc_bad_req_handle);
	idb = statement->isr_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_free_stmt;
	ips = &comm->ips_operations.ips_op_object;
	ips->ips_handle = (UCHAR *) (statement->isr_handle);
	ips->ips_parameter = (ULONG) option;

	/* send request and get response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);

	/* free statement resources */

	statement->isr_handle = (FRBRD *) ips->ips_handle;
	if (!statement->isr_handle) {
		release_sql_request(statement);
		*stmt_handle = NULL;
	}
	else {
		statement->isr_itr = NULL;
		clear_statement_cache(statement);
	}
	RETURN_SUCCESS;
}


ISC_STATUS GDS_DSQL_INSERT(ISC_STATUS * user_status,
					   IPSERVER_ISR * stmt_handle,
					   USHORT blr_length,
					   UCHAR * blr,
					   USHORT msg_type, USHORT msg_length, UCHAR * msg)
{
/**************************************
 *                  
 *      d s q l _ i n s e r t
 *
 **************************************
 *
 * Functional description
 *      Insert next record into a dynamic SQL cursor.
 *
 **************************************/
	IDB idb;
	IPSERVER_ISR statement;
	ips_dsql *ips;
	ips_string *ips_blr;
	ips_string *ips_msg;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	statement = *stmt_handle;
	CHECK_HANDLE(statement, type_ipserver_isr, isc_bad_req_handle);
	idb = statement->isr_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_insert;
	ips = &comm->ips_operations.ips_op_dsql;
	ips->ips_st_handle = (UCHAR *) (statement->isr_handle);
	ips->ips_msg_type = msg_type;
	IPS_C_IN(comm, ips_blr, IPS_DSQL_INSERT_BLR, blr, blr_length);
	IPS_C_IN(comm, ips_msg, IPS_DSQL_INSERT_MSG, msg, msg_length);

	/* send request and get response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_DSQL_PREPARE(ISC_STATUS * user_status,
						ITR * itr_handle,
						IPSERVER_ISR * stmt_handle,
						USHORT length,
						UCHAR * string,
						USHORT dialect,
						USHORT item_length,
						UCHAR * items, USHORT buffer_length, UCHAR * buffer)
{
/**************************************
 *
 *      d s q l _ p r e p a r e
 *
 **************************************
 *
 * Functional description
 *      Prepare a dynamic SQL statement for execution.
 *
 **************************************/
	IDB idb;
	ITR transaction;
	IPSERVER_ISR statement;
	ips_dsql *ips;
	ips_string *ips_prep_string;
	ips_string *ips_prep_items;
	ips_string *ips_prep_buffer;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	statement = *stmt_handle;
	CHECK_HANDLE(statement, type_ipserver_isr, isc_bad_req_handle);
	idb = statement->isr_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	if (transaction = *itr_handle)
		CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
	if (!length)
		length = strlen(reinterpret_cast < char *>(string));
	if (statement->isr_packed)
		ALLI_free((UCHAR *) statement->isr_packed);
	statement->isr_max_recs = 0;
	statement->isr_eof_flag = FALSE;
	statement->isr_packed = NULL;

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_prepare_stmt;
	ips = &comm->ips_operations.ips_op_dsql;
	if (transaction)
		ips->ips_tr_handle = (UCHAR *) (transaction->itr_handle);
	else
		ips->ips_tr_handle = NULL;
	ips->ips_st_handle = (UCHAR *) (statement->isr_handle);
	ips->ips_parameter = dialect;
	IPS_C_IN(comm, ips_prep_string, IPS_DSQL_PREP_STRING, string, length);
	IPS_C_IN(comm, ips_prep_items, IPS_DSQL_PREP_ITEMS, items, item_length);
	IPS_C_OUT(comm, ips_prep_buffer, IPS_DSQL_PREP_BUFFER, buffer,
			  buffer_length);

	/* send request and get response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	statement->isr_batch_flag = (USHORT) ips->ips_parameter;
	RETURN_SUCCESS;
}


ISC_STATUS GDS_DSQL_SET_CURSOR(ISC_STATUS * user_status,
						   IPSERVER_ISR * stmt_handle,
						   UCHAR * cursor, USHORT type)
{
/*****************************************
 *
 *      d s q l _ s e t _ c u r s o r
 *
 *****************************************
 *
 * Functional Description
 *      Set a cursor name for a dynamic request.
 *
 *****************************************/
	IDB idb;
	IPSERVER_ISR statement;
	USHORT cursor_length;
	ips_dsql *ips;
	ips_string *ips_name;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	statement = *stmt_handle;
	CHECK_HANDLE(statement, type_ipserver_isr, isc_bad_req_handle);
	idb = statement->isr_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	cursor_length = name_length(reinterpret_cast < char *>(cursor)) + 1;

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_set_cursor;
	ips = &comm->ips_operations.ips_op_dsql;
	ips->ips_st_handle = (UCHAR *) (statement->isr_handle);
	ips->ips_parameter = type;
	IPS_C_IN(comm, ips_name, IPS_DSQL_SET_CURSOR, cursor, cursor_length);

	/* send request and wait for response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_DSQL_SQL_INFO(ISC_STATUS * user_status,
						 IPSERVER_ISR * stmt_handle,
						 USHORT item_length,
						 UCHAR * items, USHORT buffer_length, UCHAR * buffer)
{
/**************************************
 *
 *      d s q l _ s q l _ i n f o
 *
 **************************************
 *
 * Functional description
 *      Provide information on sql object.
 *
 **************************************/
	IDB idb;
	IPSERVER_ISR statement;
	ips_object *ips;
	ips_string *ips_items;
	ips_string *ips_data;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	statement = *stmt_handle;
	CHECK_HANDLE(statement, type_ipserver_isr, isc_bad_req_handle);
	idb = statement->isr_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_info_sql;
	ips = &comm->ips_operations.ips_op_object;
	ips->ips_handle = (UCHAR *) (statement->isr_handle);
	IPS_C_IN(comm, ips_items, IPS_INFO_ITEMS, items, item_length);
	IPS_C_OUT(comm, ips_data, IPS_INFO_DATA, buffer, buffer_length);

	/* send request and get response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_GET_SEGMENT(ISC_STATUS * user_status,
					   IBL * blob_handle,
					   USHORT * length,
					   USHORT GDS_VAL(buffer_length), UCHAR * buffer)
{
/**************************************
 *
 *      g d s _ g e t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *      Get a segment from an open blob.
 *
 **************************************/
	IDB idb;
	IBL blob;
	ips_segment *ips;
	ips_string *ips_seg;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	blob = *blob_handle;
	CHECK_HANDLE(blob, type_ibl, isc_bad_segstr_handle);
	idb = blob->ibl_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_get_segment;
	ips = &comm->ips_operations.ips_op_segment;
	ips->ips_bl_handle = (UCHAR *) (blob->ibl_handle);
	IPS_C_OUT(comm, ips_seg, IPS_BLOB_SEGMENT, buffer, buffer_length);

	/* send request and get response */

	*length = 0;
	if (check_response(icc, user_status))
		if (user_status[1] != isc_segment)
			RETURN_ERROR(user_status[1]);
	*length = ips->ips_length;
	RETURN_ERROR(user_status[1]);
}


ISC_STATUS GDS_GET_SLICE(ISC_STATUS * user_status,
					 IDB * db_handle,
					 ITR * tra_handle,
					 BID array_id,
					 USHORT GDS_VAL(sdl_length),
					 UCHAR * sdl,
					 USHORT GDS_VAL(param_length),
					 UCHAR * param,
					 SLONG GDS_VAL(slice_length),
					 UCHAR * slice, SLONG * return_length)
{
/**************************************
 *
 *      g d s _ g e t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *      Snatch a slice of an array.
 *
 **************************************/
	IDB idb;
	ITR transaction;
	ULONG length;
	ips_slice *ips;
	ips_string *ips_sdl;
	ips_string *ips_parms;
	ips_string *ips_data;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	idb = *db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	transaction = *tra_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_get_slice;
	ips = &comm->ips_operations.ips_op_slice;
	ips->ips_db_handle = (UCHAR *) (idb->idb_handle);
	ips->ips_tr_handle = (UCHAR *) (transaction->itr_handle);
	ips->ips_rel_id = array_id->bid_relation_id;
	ips->ips_number = array_id->bid_number;
	IPS_C_IN(comm, ips_sdl, IPS_SLICE_SDL, sdl, sdl_length);
	IPS_C_IN(comm, ips_parms, IPS_SLICE_PARAM, param, param_length);
	IPS_C_OUT(comm, ips_data, IPS_SLICE_BUFFER, slice, slice_length);

	/* send request and wait for response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);

	/* return data */

	length = ips->ips_length;
	if (return_length)
		*return_length = length;
	RETURN_SUCCESS;
}


ISC_STATUS GDS_OPEN_BLOB(ISC_STATUS * user_status,
					 IDB * db_handle,
					 ITR * itr_handle, IBL * blob_handle, BID blob_id)
{
/**************************************
 *
 *      g d s _ o p e n _ b l o b
 *
 **************************************
 *
 * Functional description
 *      Open an existing blob.
 *
 **************************************/
	IDB idb;
	ITR transaction;
	IBL blob;
	ips_blob *ips;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	NULL_CHECK(blob_handle, isc_bad_segstr_handle);
	idb = *db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	transaction = *itr_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_open_blob;
	ips = &comm->ips_operations.ips_op_blob;
	ips->ips_db_handle = (UCHAR *) (idb->idb_handle);
	ips->ips_tr_handle = (UCHAR *) (transaction->itr_handle);
	ips->ips_rel_id = blob_id->bid_relation_id;
	ips->ips_bid_number = blob_id->bid_number;

	/* send request and get response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);

	/* create and fill a blob structure with results */

	blob = (IBL) ALLOCV(type_ibl, BLOB_LENGTH);
	*blob_handle = blob;
	NOT_NULL(blob);
	blob->ibl_handle = (FRBRD *) ips->ips_bl_handle;
	blob->ibl_buffer_length = BLOB_LENGTH;
	blob->ibl_idb = idb;
	blob->ibl_itr = transaction;
	blob->ibl_ptr = blob->ibl_buffer;
	blob->ibl_next = transaction->itr_blobs;
	transaction->itr_blobs = blob;
	RETURN_SUCCESS;
}


ISC_STATUS GDS_OPEN_BLOB2(ISC_STATUS * user_status,
					  IDB * db_handle,
					  ITR * itr_handle,
					  IBL * blob_handle,
					  BID blob_id, USHORT GDS_VAL(bpb_length), UCHAR * bpb)
{
/**************************************
 *
 *      g d s _ o p e n _ b l o b 2
 *
 **************************************
 *
 * Functional description
 *      Open an existing blob.
 *
 **************************************/
	IDB idb;
	ITR transaction;
	IBL blob;
	ips_blob *ips;
	ips_string *ips_bpb;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	NULL_CHECK(blob_handle, isc_bad_segstr_handle);
	idb = *db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	transaction = *itr_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_open_blob2;
	ips = &comm->ips_operations.ips_op_blob;
	ips->ips_db_handle = (UCHAR *) (idb->idb_handle);
	ips->ips_tr_handle = (UCHAR *) (transaction->itr_handle);
	ips->ips_rel_id = blob_id->bid_relation_id;
	ips->ips_bid_number = blob_id->bid_number;
	IPS_C_IN(comm, ips_bpb, IPS_BLOB_BPB, bpb, bpb_length);

	/* send request and wait for response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);

	/* create and fill blob structure */

	blob = (IBL) ALLOCV(type_ibl, BLOB_LENGTH);
	*blob_handle = blob;
	NOT_NULL(blob);
	blob->ibl_handle = (FRBRD *) ips->ips_bl_handle;
	blob->ibl_buffer_length = BLOB_LENGTH;
	blob->ibl_idb = idb;
	blob->ibl_itr = transaction;
	blob->ibl_ptr = blob->ibl_buffer;
	blob->ibl_next = transaction->itr_blobs;
	transaction->itr_blobs = blob;
	RETURN_SUCCESS;
}


ISC_STATUS GDS_PREPARE(ISC_STATUS * user_status,
				   ITR * itr_handle,
				   USHORT GDS_VAL(buffer_length), UCHAR * buffer)
{
/**************************************
 *
 *      g d s _ p r e p a r e
 *
 **************************************
 *
 * Functional description
 *      Prepare a transaction for commit.  First phase of a two
 *      phase commit.
 *
 **************************************/
	IDB idb;
	ITR transaction;
	ips_object *ips;
	ips_string *ips_prep_str;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	transaction = *itr_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
	idb = (*itr_handle)->itr_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_prepare2;
	ips = &comm->ips_operations.ips_op_object;
	ips->ips_handle = (UCHAR *) (transaction->itr_handle);
	IPS_C_IN(comm, ips_prep_str, IPS_PREPARE_TRANS, buffer, buffer_length);

	/* send request and wait for and return response */

	RETURN_ERROR(check_response(icc, user_status));
}


ISC_STATUS GDS_PUT_SEGMENT(ISC_STATUS * user_status,
					   IBL * blob_handle,
					   USHORT GDS_VAL(buffer_length), UCHAR * buffer)
{
/**************************************
 *
 *      g d s _ p u t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *      Send a segment of data to an open blob.
 *
 **************************************/
	IDB idb;
	IBL blob;
	ips_segment *ips;
	ips_string *ips_seg;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/*  verify handles */

	blob = *blob_handle;
	CHECK_HANDLE(blob, type_ibl, isc_bad_segstr_handle);
	idb = blob->ibl_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_put_segment;
	ips = &comm->ips_operations.ips_op_segment;
	ips->ips_bl_handle = (UCHAR *) (blob->ibl_handle);
	IPS_C_IN(comm, ips_seg, IPS_BLOB_SEGMENT, buffer, buffer_length);

	/* send request and get response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_PUT_SLICE(ISC_STATUS * user_status,
					 IDB * db_handle,
					 ITR * tra_handle,
					 BID array_id,
					 USHORT GDS_VAL(sdl_length),
					 UCHAR * sdl,
					 USHORT GDS_VAL(param_length),
					 UCHAR * param,
					 SLONG GDS_VAL(slice_length), UCHAR * slice)
{
/**************************************
 *
 *      g d s _ p u t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *      Insert a slice of an array.
 *
 **************************************/
	IDB idb;
	ITR transaction;
	ips_slice *ips;
	ips_string *ips_sdl;
	ips_string *ips_parms;
	ips_string *ips_data;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	idb = *db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	transaction = *tra_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_put_slice;
	ips = &comm->ips_operations.ips_op_slice;
	ips->ips_db_handle = (UCHAR *) (idb->idb_handle);
	ips->ips_tr_handle = (UCHAR *) (transaction->itr_handle);
	ips->ips_rel_id = array_id->bid_relation_id;
	ips->ips_number = array_id->bid_number;
	IPS_C_IN(comm, ips_sdl, IPS_SLICE_SDL, sdl, sdl_length);
	IPS_C_IN(comm, ips_parms, IPS_SLICE_PARAM, param, param_length);
	IPS_C_IN(comm, ips_data, IPS_SLICE_BUFFER, slice, slice_length);

	/* send request and wait for response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);

	/* return resulting ids */

	array_id->bid_relation_id = ips->ips_rel_id;
	array_id->bid_number = ips->ips_number;
	RETURN_SUCCESS;
}


ISC_STATUS GDS_QUE_EVENTS(ISC_STATUS * user_status,
					  IDB * handle,
					  SLONG * id,
					  USHORT GDS_VAL(length),
					  UCHAR * events, void (GDS_VAL(*ast)) (), void *arg)
{
/**************************************
 *
 *      g d s _ q u e _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *      Que request for event notification.
 *
 **************************************/
	IDB idb;
	USHORT evsem;
	TEXT name_buffer[128];
	IVNT event;
	ips_que_events *ips;
	ips_string *ips_event;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	idb = *handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_que_events;
	ips = &comm->ips_operations.ips_op_que_evnt;
	ips->ips_db_handle = (UCHAR *) (idb->idb_handle);
	ips->ips_ast = (UCHAR *) ast;
	ips->ips_arg = (UCHAR *) arg;
	IPS_C_IN(comm, ips_event, IPS_QUEUE_EVENT, events, length);

	/* allocate an event block */

	THD_mutex_lock(&clisect);
	if (!event_semaphore) {

		/* start the event threads, if necessary, by creating the
		   event semaphore, starting the event packer, waiting until
		   it's done on the event semaphore, and then starting the
		   event thread itself */

		/* create an event semaphore, but make sure it's a new one */

		for (evsem = 0; evsem < 1000; evsem++) {
			sprintf(name_buffer, IPI_EVENT_THREAD, Config::getIpcName(), evsem);
			event_semaphore =
				CreateSemaphore(ISC_get_security_desc(), 0L, 10000L,
								name_buffer);
			if (event_semaphore && GetLastError() != ERROR_ALREADY_EXISTS)
				break;
			event_semaphore = 0;
		}
		if (event_semaphore) {
			gds__thread_start(reinterpret_cast < FPTR_INT_VOID_PTR >
							  (event_packer), idb->idb_thread, THREAD_high, 0,
							  &event_packer_handle);
			if (!event_packer_handle)
				gds__log("unable to start event packer thread %ld",
						 (long) GetLastError());
			else {
				WaitForSingleObject(event_semaphore, INFINITE);
				gds__thread_start(reinterpret_cast < FPTR_INT_VOID_PTR >
								  (event_thread), NULL, THREAD_high, 0,
								  &event_thread_handle);
				if (!event_thread_handle)
					gds__log("unable to start event processing thread %ld",
							 (long) GetLastError());
			}
		}
		else
			gds__log("Unable to create event semaphore");
	}
	for (event = idb->idb_events; event; event = event->ivnt_next)
		if (!event->ivnt_id)
			break;
	if (!event) {
		event = (IVNT) ALLOC(type_ivnt);
		if (!event) {
			THD_mutex_unlock(&clisect);
			NOT_NULL(event);
		}
		event->ivnt_handle = 0;
		event->ivnt_idb = idb;
		event->ivnt_next = idb->idb_events;
		idb->idb_events = event;
	}
	eventID++;
	event->ivnt_id = eventID;
	event->ivnt_ast = ast;
	event->ivnt_arg = arg;
	THD_mutex_unlock(&clisect);

	/* send request and get response */

	ips->ips_event_hwnd = event_window;
	ips->ips_event_id = eventID;
	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	*id = ips->ips_event_id;
	event->ivnt_handle = ips->ips_event_id;
	RETURN_SUCCESS;
}


ISC_STATUS GDS_RECEIVE(ISC_STATUS * user_status,
				   IRQ * req_handle,
				   SSHORT GDS_VAL(msg_type),
				   USHORT GDS_VAL(msg_length),
				   UCHAR * msg, SSHORT GDS_VAL(level)
#ifdef SCROLLABLE_CURSORS
				   , USHORT direction, ULONG offset
#endif
	)
{
/**************************************
 *
 *      g d s _ r e c e i v e 2
 *
 **************************************
 *
 * Functional description
 *      Get a record from the host program.
 *
 **************************************/
	IRQ request;
	IDB idb;
	ips_request *ips;
	ips_string *ips_message;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	request = *req_handle;
	CHECK_HANDLE(request, type_irq, isc_bad_req_handle);
	idb = request->irq_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_receive;
	ips = &comm->ips_operations.ips_op_request;
	ips->ips_rq_handle = (UCHAR *) (request->irq_handle);
	ips->ips_msg_type = msg_type;
	ips->ips_req_level = level;
#ifdef SCROLLABLE_CURSORS
	ips->ips_direction = direction;
	ips->ips_offset = offset;
#endif
	IPS_C_OUT(comm, ips_message, IPS_RECEIVE_MESSAGE, msg, msg_length);

	/* send request and get answer */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_RECONNECT(ISC_STATUS * user_status,
					 IDB * db_handle,
					 ITR * itr_handle, USHORT GDS_VAL(length), UCHAR * id)
{
/**************************************
 *
 *      g d s _ r e c o n n e c t
 *
 **************************************
 *
 * Functional description
 *      Connect to a transaction in limbo.
 *
 **************************************/
	IDB idb;
	ISC_STATUS l;
	ips_reconnect *ips;
	ips_string *ips_id;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	NULL_CHECK(itr_handle, isc_bad_trans_handle);
	idb = *db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	l = GDS_VAL(length);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_reconnect;
	ips = &comm->ips_operations.ips_op_recon_trans;
	ips->ips_db_handle = (UCHAR *) (idb->idb_handle);
	IPS_C_IN(comm, ips_id, IPS_RECONNECT_ID, id, l);

	/* send message and wait for response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	*itr_handle =
		make_transaction(user_status, idb, (FRBRD *) ips->ips_tr_handle);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_RELEASE_REQUEST(ISC_STATUS * user_status, IRQ * req_handle)
{
/**************************************
 *
 *      g d s _ r e l e a s e _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *      Release a request.
 *
 **************************************/
	IDB idb;
	IRQ request;
	ips_object *ips;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	request = *req_handle;
	CHECK_HANDLE(request, type_irq, isc_bad_req_handle);
	idb = request->irq_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_release;
	ips = &comm->ips_operations.ips_op_object;
	ips->ips_handle = (UCHAR *) (request->irq_handle);

	/* make request and free handle */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);

	/* release request resources */

	release_request(request);
	*req_handle = NULL;
	RETURN_SUCCESS;
}


ISC_STATUS GDS_REQUEST_INFO(ISC_STATUS * user_status,
						IRQ * request,
						USHORT GDS_VAL(level),
						USHORT GDS_VAL(item_length),
						UCHAR * items,
						USHORT GDS_VAL(buffer_length), UCHAR * buffer)
{
/**************************************
 *
 *      g d s _ r e q u e s t _ i n f o
 *
 **************************************
 *
 * Functional description
 *      Provide information on request object.
 *
 **************************************/
	IDB idb;
	ips_object *ips;
	ips_string *ips_items;
	ips_string *ips_data;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	CHECK_HANDLE((*request), type_irq, isc_bad_req_handle);
	idb = (*request)->irq_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_info_request;
	ips = &comm->ips_operations.ips_op_object;
	ips->ips_handle = (UCHAR *) ((*request)->irq_handle);
	ips->ips_parameter = level;
	IPS_C_IN(comm, ips_items, IPS_INFO_ITEMS, items, item_length);
	IPS_C_OUT(comm, ips_data, IPS_INFO_DATA, buffer, buffer_length);

	/* send request and get response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_ROLLBACK_RETAINING(ISC_STATUS * user_status, ITR * itr_handle)
{
/**************************************
 *
 *      i s c _ r o l l b a c k _ r e t a i n i n g
 *
 **************************************
 *
 * Functional description
 *
 *    Abort the transaction but keep the environment valid
 *
 **************************************/
	IDB idb;
	ITR transaction;
	ips_object *ips;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	transaction = *itr_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
	idb = (*itr_handle)->itr_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_rollback_retaining;
	ips = &comm->ips_operations.ips_op_object;
	ips->ips_handle = (UCHAR *) (transaction->itr_handle);

	/* send request */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_ROLLBACK(ISC_STATUS * user_status, ITR * itr_handle)
{
/**************************************
 *
 *      g d s _ r o l l b a c k
 *
 **************************************
 *
 * Functional description
 *      Abort a transaction.
 *
 **************************************/
	IDB idb;
	ITR transaction;
	ips_object *ips;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	transaction = *itr_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
	idb = (*itr_handle)->itr_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_rollback;
	ips = &comm->ips_operations.ips_op_object;
	ips->ips_handle = (UCHAR *) (transaction->itr_handle);

	/* send request and free handle */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);

	/* release transaction resources */

	clear_transaction_statements(transaction);
	release_transaction(transaction);
	*itr_handle = NULL;
	RETURN_SUCCESS;
}


ISC_STATUS GDS_SEEK_BLOB(ISC_STATUS * user_status,
					 IBL * blob_handle,
					 SSHORT GDS_VAL(mode),
					 SLONG GDS_VAL(offset), SLONG * result)
{
/**************************************
 *
 *      g d s _ s e e k _ b l o b
 *
 **************************************
 *
 * Functional description
 *      Try to seek a blob.
 *
 **************************************/
	IDB idb;
	IBL blob;
	ips_seek_blob *ips;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	blob = *blob_handle;
	CHECK_HANDLE(blob, type_ibl, isc_bad_segstr_handle);
	idb = blob->ibl_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_seek_blob;
	ips = &comm->ips_operations.ips_op_sk_blob;
	ips->ips_bl_handle = (UCHAR *) (blob->ibl_handle);
	ips->ips_offset = offset;
	ips->ips_mode = mode;

	/* send request and get response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	*result = ips->ips_result;
	RETURN_SUCCESS;
}


ISC_STATUS GDS_SEND(ISC_STATUS * user_status,
				IRQ * req_handle,
				SSHORT GDS_VAL(msg_type),
				USHORT GDS_VAL(msg_length),
				UCHAR * msg, SSHORT GDS_VAL(level))
{
/**************************************
 *
 *      g d s _ s e n d
 *
 **************************************
 *
 * Functional description
 *      Get a record from the host program.
 *
 **************************************/
	IRQ request;
	IDB idb;
	ips_request *ips;
	ips_string *ips_message;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	request = *req_handle;
	CHECK_HANDLE(request, type_irq, isc_bad_req_handle);
	idb = request->irq_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_send;
	ips = &comm->ips_operations.ips_op_request;
	ips->ips_rq_handle = (UCHAR *) (request->irq_handle);
	ips->ips_msg_type = msg_type;
	ips->ips_req_level = level;
	IPS_C_IN(comm, ips_message, IPS_SEND_MESSAGE, msg, msg_length);

	/* send request and get response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}

ISC_STATUS GDS_SERVICE_ATTACH(ISC_STATUS * user_status,
						  USHORT service_length,
						  TEXT * service_name,
						  IDB * handle, USHORT spb_length, SCHAR * spb)
{
/**************************************
 *
 *      g d s _ s e r v i c e _ a t t a c h
 *
 **************************************
 *
 * Functional description
 *      Connect to an Interbase service via the
 *      interprocess interface to a local server.
 *
 **************************************/
	IDB idb;
	ICC icc;
	SSHORT l;
	ips_object *ips;
	ips_string *ips_name;
	ips_string *ips_spb;
	ips_comm_area *comm;
	USHORT commi;


	/* handle should be null coming in */

	NULL_CHECK(handle, isc_bad_svc_handle);
	if (!(l = GDS_VAL(service_length)))
		l = strlen(service_name);

	/* initiate connection */

	if (!init(user_status, &icc))
		return user_status[1];

	/* set up communications area */

	THD_mutex_lock(&mapsect);
	comm = (ips_comm_area *) icc->icc_mapped_addr;
	for (commi = 0; commi < MAX_IPS_STRINGS; commi++)
		comm->ips_buffers[commi].ips_flags = 0;
	comm->ips_operation = op_service_attach;
	ips = &comm->ips_operations.ips_op_object;
	IPS_C_IN(comm, ips_name, IPS_ATTACH_SVC_NAME, service_name, l);
	IPS_C_IN(comm, ips_spb, IPS_ATTACH_SVC_SPB, spb, spb_length);

	/* send request and wait for response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);

	/* allocate a database structure and link it to thread */

	idb = (IDB) ALLOC(type_idb);
	*handle = idb;
	NOT_NULL(idb);
	idb->idb_flags = IDBF_SERVICE_ATTACHMENT;
	idb->idb_next = icc->icc_databases;
	idb->idb_thread = icc;
	icc->icc_databases = idb;
	idb->idb_handle = (FRBRD *) (ips->ips_handle);
	RETURN_SUCCESS;
}

ISC_STATUS GDS_SERVICE_DETACH(ISC_STATUS * user_status, IDB * handle)
{
/**************************************
 *
 *      g d s _ s e r v i c e _ d e t a c h
 *
 **************************************
 *
 * Functional description
 *      Close down a service.
 *
 **************************************/
	ICC icc;
	IDB idb;
	ips_object *ips;
	ips_comm_area *comm;
	USHORT commi;


	/* verify handle */

	idb = *handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_svc_handle);

	/* point to communications area */

	icc = idb->idb_thread;
	THD_mutex_lock(&mapsect);
	if (icc->icc_flags & ICCF_SERVER_SHUTDOWN) {

		/* if server was shut down, set up vector */

		user_status[0] = isc_arg_gds;
		user_status[1] = FB_SUCCESS;
		user_status[2] = isc_arg_end;
	}
	else {
		comm = (ips_comm_area *) icc->icc_mapped_addr;
		for (commi = 0; commi < MAX_IPS_STRINGS; commi++)
			comm->ips_buffers[commi].ips_flags = 0;
		comm->ips_operation = op_service_detach;
		ips = &comm->ips_operations.ips_op_object;
		ips->ips_handle = (UCHAR *) (idb->idb_handle);

		/* send request and release handle */

		if (check_response(icc, user_status))
			if (user_status[1] != isc_lost_db_connection)
				return user_status[1];
	}

	/* free up resources */

	release_database(idb);
	*handle = NULL;
	if (user_status[1])
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}

ISC_STATUS GDS_SERVICE_QUERY(ISC_STATUS * user_status,
						 IDB * service,
						 ULONG * reserved,
						 USHORT send_item_length,
						 SCHAR * send_items,
						 USHORT recv_item_length,
						 SCHAR * recv_items,
						 USHORT buffer_length, SCHAR * buffer)
{
/**************************************
 *
 *      g d s _ s e r v i c e _ q u e r y
 *
 **************************************
 *
 * Functional description
 *      Provide information on service object.
 *
 **************************************/
	IDB idb;
	ips_object *ips;
	ips_string *ips_snd_items;
	ips_string *ips_rcv_items;
	ips_string *ips_data;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handle */

	idb = *service;
	CHECK_HANDLE(idb, type_idb, isc_bad_svc_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_service_info;
	ips = &comm->ips_operations.ips_op_object;
	ips->ips_handle = (UCHAR *) (idb->idb_handle);
	IPS_C_IN(comm, ips_snd_items, IPS_INFO_ITEMS, send_items,
			 send_item_length);
	IPS_C_IN(comm, ips_rcv_items, IPS_QUERY_RECV_ITEMS, recv_items,
			 recv_item_length);
	IPS_C_OUT(comm, ips_data, IPS_INFO_DATA, buffer, buffer_length);

	/* send request and get answer */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}

ISC_STATUS GDS_SERVICE_START(ISC_STATUS * user_status,
						 IDB * service,
						 ULONG * reserved, USHORT spb_length, SCHAR * spb)
{
 /**************************************
 *
 *      g d s _ s e r v i c e _ s t a r t
 *
 **************************************
 *
 * Functional description
 *      Start an InterBase service
 *
 **************************************/
	IDB idb;
	ips_object *ips;
	ips_string *ips_spb;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handle */
	idb = *service;
	CHECK_HANDLE(idb, type_idb, isc_bad_svc_handle);

	/* point to communications area */
	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_service_start;
	ips = &comm->ips_operations.ips_op_object;
	ips->ips_handle = (UCHAR *) (idb->idb_handle);
	IPS_C_IN(comm, ips_spb, IPS_START_SVC_SPB, spb, spb_length);

	/* send request and get answer */
	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_START(ISC_STATUS * user_status,
				 IRQ * req_handle,
				 ITR * itr_handle, SSHORT GDS_VAL(level))
{
/**************************************
 *
 *      g d s _ s t a r t
 *
 **************************************
 *
 * Functional description
 *      Get a record from the host program.
 *
 **************************************/
	IRQ request;
	ITR transaction;
	IDB idb;
	ips_request *ips;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	request = *req_handle;
	CHECK_HANDLE(request, type_irq, isc_bad_req_handle);
	transaction = *itr_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
	idb = request->irq_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_start;
	ips = &comm->ips_operations.ips_op_request;
	ips->ips_rq_handle = (UCHAR *) (request->irq_handle);
	ips->ips_tr_handle = (UCHAR *) (transaction->itr_handle);
	ips->ips_req_level = level;

	/* send request and get response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_START_AND_SEND(ISC_STATUS * user_status,
						  IRQ * req_handle,
						  ITR * itr_handle,
						  SSHORT GDS_VAL(msg_type),
						  USHORT GDS_VAL(msg_length),
						  UCHAR * msg, SSHORT GDS_VAL(level))
{
/**************************************
 *
 *      g d s _ s t a r t _ a n d _ s e n d
 *
 **************************************
 *
 * Functional description
 *      Get a record from the host program.
 *
 **************************************/
	IRQ request;
	ITR transaction;
	IDB idb;
	ips_request *ips;
	ips_string *ips_message;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	request = *req_handle;
	CHECK_HANDLE(request, type_irq, isc_bad_req_handle);
	transaction = *itr_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);
	idb = request->irq_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_start_and_send;
	ips = &comm->ips_operations.ips_op_request;
	ips->ips_rq_handle = (UCHAR *) (request->irq_handle);
	ips->ips_tr_handle = (UCHAR *) (transaction->itr_handle);
	ips->ips_msg_type = msg_type;
	ips->ips_req_level = level;
	IPS_C_IN(comm, ips_message, IPS_SEND_MESSAGE, msg, msg_length);

	/* send request and get response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_START_MULTIPLE(ISC_STATUS * user_status,
						  ITR * itr_handle,
						  SSHORT GDS_VAL(count), int **vector)
{
/**************************************
 *
 *      g d s _ s t a r t _ m u l t i p l e
 *
 **************************************
 *
 * Functional description
 *      Start a transaction.
 *
 **************************************/
	int **args;
	IDB idb;
	USHORT i, j;
	TEXT *p;
	ULONG l, *lp;
	ips_start_trans *ips;
	ips_comm_area *comm;
	TEXT *comm_ptr;
	USHORT commi;
	ICC icc;


	/* verify handle */

	NULL_CHECK(itr_handle, isc_bad_trans_handle);

	/* point to communications area */

	args = vector;
	idb = (IDB) * (*args++);
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	GET_OBJECT(idb->idb_thread);
	comm_ptr = (TEXT *) comm->ips_data;
	comm->ips_operation = op_transaction;
	ips = &comm->ips_operations.ips_op_strt_trans;
	ips->ips_db_count = count;

	/* fill in handles, verifying along the way (note that this is the
	   ONLY case where normal buffering is not done, so we'd better
	   never have enough databases to exceed 8k worth of space */

	for (i = 0; i < GDS_VAL(count); i++) {

		/* first idb pointer already acquired above */

		if (i) {
			idb = (IDB) * (*args++);
			CHECK_HANDLE_REL(idb, type_idb, isc_bad_db_handle);
		}
		lp = (ULONG *) comm_ptr;
		*lp++ = (ULONG) (idb->idb_handle);
		l = (ULONG) * args++;
		*lp++ = l;
		comm_ptr = (TEXT *) lp;
		p = (TEXT *) * args++;
		for (j = 0; j < l; j++)
			*comm_ptr++ = *p++;

		/* align on ULONG */

		l = l % sizeof(ULONG);
		if (l)
			comm_ptr += sizeof(ULONG) - l;
	}

	/* send request and get response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	*itr_handle =
		make_transaction(user_status, idb, (FRBRD *) ips->ips_tr_handle);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_START_TRANSACTION(ISC_STATUS * user_status,
							 ITR * itr_handle, SSHORT GDS_VAL(count), ...)
{
/**************************************
 *
 *      g d s _ s t a r t _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *      Start a transaction.
 *
 **************************************/
	va_list args;
	IDB idb;
	USHORT i, j;
	UCHAR *p;
	ULONG l, *lp;
	ips_start_trans *ips;
	ips_comm_area *comm;
	TEXT *comm_ptr;
	USHORT commi;
	ICC icc;


	/* verify handle */

	NULL_CHECK(itr_handle, isc_bad_trans_handle);

	/* point to communications area */

	VA_START(args, count);
	idb = *(va_arg(args, IDB *));
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	GET_OBJECT(idb->idb_thread);
	comm_ptr = (TEXT *) comm->ips_data;
	comm->ips_operation = op_transaction;
	ips = &comm->ips_operations.ips_op_strt_trans;
	ips->ips_db_count = count;

	/* fill in handles, verifying along the way (note that this is the
	   ONLY case where normal buffering is not done, so we'd better
	   never have enough databases to exceed 8k worth of space */

	for (i = 0; i < GDS_VAL(count); i++) {

		/* the first pointer has already been gotten above */

		if (i) {
			idb = *(va_arg(args, IDB *));
			CHECK_HANDLE_REL(idb, type_idb, isc_bad_db_handle);
		}
		lp = (ULONG *) comm_ptr;
		*lp++ = (ULONG) (idb->idb_handle);
		l = (ULONG) va_arg(args, int);
		*lp++ = l;
		comm_ptr = (TEXT *) lp;
		p = va_arg(args, UCHAR *);
		for (j = 0; j < l; j++)
			*comm_ptr++ = *p++;

		/* align on ULONG */

		l = l % sizeof(ULONG);
		if (l)
			comm_ptr += sizeof(ULONG) - l;
	}

	/* send message and get response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	*itr_handle =
		make_transaction(user_status, idb, (FRBRD *) ips->ips_tr_handle);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_TRANSACTION_INFO(ISC_STATUS * user_status,
							ITR * transaction,
							USHORT GDS_VAL(item_length),
							UCHAR * items,
							USHORT GDS_VAL(buffer_length), UCHAR * buffer)
{
/**************************************
 *
 *      g d s _ t r a n s a c t i o n _ i n f o
 *
 **************************************
 *
 * Functional description
 *      Provide information on transaction object.
 *
 **************************************/
	IDB idb;
	ips_object *ips;
	ips_string *ips_items;
	ips_string *ips_data;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	CHECK_HANDLE((*transaction), type_itr, isc_bad_trans_handle);
	idb = (*transaction)->itr_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_info_transaction;
	ips = &comm->ips_operations.ips_op_object;
	ips->ips_handle = (UCHAR *) ((*transaction)->itr_handle);
	IPS_C_IN(comm, ips_items, IPS_INFO_ITEMS, items, item_length);
	IPS_C_OUT(comm, ips_data, IPS_INFO_DATA, buffer, buffer_length);

	/* send request and get response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_TRANSACT_REQUEST(ISC_STATUS * user_status,
							IDB * db_handle,
							ITR * tra_handle,
							USHORT blr_length,
							UCHAR * blr,
							USHORT in_msg_length,
							UCHAR * in_msg,
							USHORT out_msg_length, UCHAR * out_msg)
{
/**************************************
 *
 *      i s c _ t r a n s a c t _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	IDB idb;
	ITR transaction;
	ips_transact_request *ips;
	ips_string *ips_blr;
	ips_string *ips_in_msg;
	ips_string *ips_out_msg;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	idb = *db_handle;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);
	transaction = *tra_handle;
	CHECK_HANDLE(transaction, type_itr, isc_bad_trans_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_transact_request;
	ips = &comm->ips_operations.ips_op_trans_req;
	ips->ips_db_handle = (UCHAR *) (idb->idb_handle);
	ips->ips_tr_handle = (UCHAR *) (transaction->itr_handle);
	IPS_C_IN(comm, ips_blr, IPS_TRANS_REQ_BLR, blr, blr_length);
	IPS_C_IN(comm, ips_in_msg, IPS_TRANS_REQ_IN_MSG, in_msg, in_msg_length);
	IPS_C_OUT(comm, ips_out_msg, IPS_TRANS_REQ_OUT_MSG, out_msg,
			  out_msg_length);

	/* send message and get response */

	if (check_response(icc, user_status))
		RETURN_ERROR(user_status[1]);
	RETURN_SUCCESS;
}


ISC_STATUS GDS_UNWIND(ISC_STATUS * user_status,
				  IRQ * req_handle, SSHORT GDS_VAL(level))
{
/**************************************
 *
 *      g d s _ u n w i n d
 *
 **************************************
 *
 * Functional description
 *      Unwind a running request.
 *
 **************************************/
	IDB idb;
	IRQ request;
	ips_object *ips;
	ips_comm_area *comm;
	USHORT commi;
	ICC icc;


	/* verify handles */

	request = *req_handle;
	CHECK_HANDLE(request, type_irq, isc_bad_req_handle);
	idb = request->irq_idb;
	CHECK_HANDLE(idb, type_idb, isc_bad_db_handle);

	/* point to communications area */

	GET_OBJECT(idb->idb_thread);
	comm->ips_operation = op_unwind;
	ips = &comm->ips_operations.ips_op_object;
	ips->ips_handle = (UCHAR *) (request->irq_handle);
	ips->ips_parameter = (ULONG) level;

	/* send request and return response */

	RETURN_ERROR(check_response(icc, user_status));
}


static ISC_STATUS check_response( ICC icc, ISC_STATUS * user_status)
{
/**************************************
 *
 *      c h e c k _ r e s p o n s e
 *
 **************************************
 *
 * Functional description
 *      Check response to a remote call.
 *
 *      Pack any and all input buffers and send the
 *      request to the server.   This gets done as
 *      often as needed to get all the client's
 *      input buffers across.
 *
 *      After all the client buffers get across, this
 *      function waits for the server to complete the
 *      operation.   This entails getting any and all
 *      client output buffers from the server.   Again,
 *      the unpacking gets done as often as necessary.
 *
 *      Data movement is done in order of buffer usage
 *      for any of the buffers that need copying.   As
 *      much as possible is crammed into the remaining
 *      buffer space in the comminucations area for each
 *      copy stage.
 *
 **************************************/
#ifdef	DEBUG_IP_TRACE
	ips_comm_area *comm;


	comm = (ips_comm_area *) icc->icc_mapped_addr;
	gds__log("ipclient sends %d at %8lX %8lX", comm->ips_operation, icc,
			 comm);
#endif

	if (!(icc->icc_flags & ICCF_SERVER_SHUTDOWN)) {
		if (pack_strings(icc)) {
			if (send_and_wait(icc)) {
				extract_status(icc, user_status);
				if (unpack_strings(icc))
					return user_status[1];
			}
		}
	}

	/* communications failure - database is now unavailable */

	icc->icc_flags |= ICCF_SERVER_SHUTDOWN;
#ifdef	DEBUG_IP_TRACE
	gds__log("ipclient comm failure %lX", (long) icc);
#endif
	user_status[0] = isc_arg_gds;
	user_status[1] = isc_lost_db_connection;
	user_status[2] = isc_arg_end;
	return user_status[1];
}


static void clear_statement_cache( IPSERVER_ISR statement)
{
/**************************************
 *
 *      c l e a r _ s t a t e m e n t _ c a c h e
 *
 **************************************
 *
 * Functional description
 *      Clean out any unfetched cached records for a statement.
 *
 **************************************/
	USHORT i;


	if (statement->isr_packed)
		ALLI_free((UCHAR *) statement->isr_packed);
	statement->isr_rec_count = 0;
	statement->isr_max_recs = 0;
	statement->isr_eof_flag = FALSE;
	statement->isr_packed = NULL;
	for (i = 0; i < ISC_STATUS_LENGTH; i++)
		statement->isr_status[i] = 0;
}


static void clear_transaction_statements( ITR transaction)
{
/**************************************
 *
 *      c l e a r _ t r a n s a c t i o n _ s t a t e m e n t s
 *
 **************************************
 *
 * Functional description
 *      Clean out cached records for any statements
 *      for this transaction.
 *
 **************************************/
	IPSERVER_ISR statement;
	IDB database;


	database = transaction->itr_idb;
	for (statement = database->idb_sql_requests; statement;
		 statement =
		 statement->isr_next) if (statement->isr_itr ==
								  transaction)
				clear_statement_cache(statement);
}


static void event_packer( ICC icc)
{
/**************************************
 *
 *      e v e n t _ p a c k e r
 *
 **************************************
 *
 * Functional description
 *      This thread just packs events on the
 *	client's queue for event_thread, so
 *	the server isn't kept waiting
 *
 **************************************/
	TEXT name_buffer[128], class_buffer[32];


	/* init the window, which never really returns */

	sprintf(name_buffer, IPI_EVENT_NAME, Config::getIpcName(), icc->icc_file, icc->icc_slot);
	sprintf(class_buffer, IPI_EVENT_CLASS, Config::getIpcName());
	IPC_window_init((char *) class_buffer, (char *) name_buffer,
					&event_window, event_semaphore);
	CloseHandle(event_packer_handle);
	event_packer_handle = 0;
}


static void event_thread(void)
{
/**************************************
 *
 *      e v e n t _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *      This thread gets events of an event queue and
 *      posts them.   Events are put on the queue by
 *      the event handler hidden windows.
 *
 **************************************/
	ICC icc;
	IDB idb;
	IVNT ivnt;
	IVNT event;
	EVENTQ queued;
	ISC_STATUS_ARRAY status;


	/* loop to wait for events */

	for (;;) {
		WaitForSingleObject(event_semaphore, INFINITE);
		if (exit_flag)
			break;

		/* init our icc */

		if (!event_icc)
			if (!init(status, &event_icc))
				return;

		/* find next queued event */

		THD_mutex_lock(&evtsect);
		if (event_head) {
			queued = event_head;
			event_head = queued->evq_next;

			/* if queue is empty, clear tail */

			if (!event_head)
				event_tail = NULL;
			THD_mutex_unlock(&evtsect);

			/* find event in icc/idb/events chain */

			event = NULL;
			THD_mutex_lock(&clisect);
			for (icc = client_threads; icc && !event; icc = icc->icc_next)
				for (idb = icc->icc_databases; idb && !event;
					 idb = idb->idb_next) for (ivnt = idb->idb_events; ivnt;
											   ivnt = ivnt->ivnt_next)
						if (ivnt->ivnt_id == queued->evq_id) {
							event = ivnt;
							break;
						}
			if (event) {
				if (event->ivnt_ast) {
					reinterpret_cast < void (*) (...) >
						(*event->ivnt_ast) (event->ivnt_arg,
											queued->evq_length,
											queued->evq_string);
					event->ivnt_id = 0;
				}
			}
			THD_mutex_unlock(&clisect);
			ALLI_free((UCHAR *) queued);
		}
		else
			THD_mutex_unlock(&evtsect);
	}
	CloseHandle(event_thread_handle);
	event_thread_handle = 0;
}


static void extract_status( ICC icc, ISC_STATUS * user_status)
{
/**************************************
 *
 *      e x t r a c t _ s t a t u s
 *
 **************************************
 *
 * Functional description
 *      Extract and convert the status vector
 *      and pack it into the comm area.
 *
 **************************************/
	USHORT i, length;
	TEXT *text;
	ISC_STATUS *v, code, *server_status;
	ips_comm_area *comm;
	ULONG *comm_ptr;
	TEXT *p;


	comm = (ips_comm_area *) icc->icc_mapped_addr;

	/* extract the status information */

	text = error_text;
	server_status = reinterpret_cast < ISC_STATUS * >(comm->ips_status);
	comm_ptr = comm->ips_data;
	v = user_status;

	/* if there's no error or warning, skip the rest */

	if (!server_status[1] && !server_status[2]) {
		*v++ = *server_status++;
		*v++ = *server_status++;
	}
	else {
		while (code = *server_status++) {
			switch (*v++ = code) {
			case isc_arg_interpreted:
			case isc_arg_string:

				/* get a string from the comm area, aligning the
				   pointer accordingly */

				*v++ = (ISC_STATUS) text;
				length = (USHORT) * comm_ptr++;
				p = (TEXT *) comm_ptr;
				comm_ptr = comm_ptr +
					(length + (sizeof(ULONG) - 1)) / sizeof(ULONG);
				for (i = 0; i < length; i++)
					*text++ = *p++;
				*text++ = (TEXT) 0;
				continue;

			case isc_arg_gds:
			case isc_arg_warning:
			case isc_arg_number:
			case isc_arg_vms:
			case isc_arg_unix:
			case isc_arg_win32:
				*v++ = *server_status++;
				continue;

			case isc_arg_end:
			case isc_arg_cstring:
			default:
				return;
			}
		}
	}
	*v = isc_arg_end;
}


static ISC_STATUS handle_error( ISC_STATUS * user_status, ISC_STATUS code)
{
/**************************************
 *
 *      h a n d l e _ e r r o r
 *
 **************************************
 *
 * Functional description
 *      An invalid handle has been passed in.  If there is a user status
 *      vector, make it reflect the error.  If not, emulate the routine
 *      "error" and abort.
 *
 **************************************/

	*user_status++ = isc_arg_gds;
	*user_status++ = code;
	*user_status = isc_arg_end;
	return code;
}


static SSHORT init( ISC_STATUS * user_status, ICC * picc)
{
/**************************************
 *
 *      i n i t
 *
 **************************************
 *
 * Functional description
 *      Initialize for database access.  First call from both CREATE and
 *      OPEN.
 *
 **************************************/
	ICC icc;
	IPM ipm;
	ULONG number=0;
	USHORT mapped_area, mapped_position;
	TEXT name_buffer[128];
	DWORD current_thread_id;
	DWORD last_error;
	HANDLE file_handle;
	LPVOID mapped_address;
	DWORD client_pid;
	ips_comm_area *comm;

	/* first, make sure that the critical region is initialized */

	while (!initialized)
	{
		if (!InterlockedIncrement(&interlock))
		{

			/* increment "succeeded", initialize */

			THD_mutex_init(&clisect);
			THD_mutex_init(&evtsect);
			THD_mutex_init(&mapsect);
			initialized = TRUE;
			gds__register_cleanup(reinterpret_cast<void(*)(void*)>(IPC_release_all),
									NULL);
			gds__register_cleanup(reinterpret_cast<void(*)(void*)>(XNET_release_all),
									NULL);
		}
		else
		{
			/* some other thread is initializing now */

			InterlockedDecrement(&interlock);
		}
	}

	/* set up for unavailable server */

	user_status[0] = isc_arg_gds;
	user_status[1] = isc_unavailable;
	user_status[2] = isc_arg_end;

	/* see if this thread has been connected yet, and if not, do so */

	THD_mutex_lock(&clisect);
	current_thread_id = GetCurrentThreadId();
	for (icc = client_threads; icc; icc = icc->icc_next)
	{
		if (icc->icc_thread_handle == current_thread_id &&
			!(icc->icc_flags & ICCF_SERVER_SHUTDOWN))
		{
			break;
		}
	}

	if (!icc)
	{

		/* send a message to the server to get the next available
		   attachment, a 32 bit value with a mapped memory file
		   ID in the high order and an allocated area in that
		   mapped file in the low order */

		client_pid = GetCurrentProcessId();
		HWND hWndServer = FindWindow(szClassName, APP_NAME);
		if (hWndServer)
		{
			number = (ULONG) SendMessage(hWndServer,
										 IPI_CONNECT_MESSAGE, 0,
										 (LPARAM) client_pid);
		}
		if (!number || number == (ULONG) - 1)
		{
			THD_mutex_unlock(&clisect);
			return 0;
		}
		pages_per_user = (USHORT) IPS_UNPACK_PAGES(number);
		users_per_map = (USHORT) IPS_UNPACK_MAX_USERS(number);
		mapped_area = (USHORT) IPS_UNPACK_MAPNUM(number);
		mapped_position = (USHORT) IPS_UNPACK_USERNUM(number);

		/* see if area is already mapped for this client */

		for (ipm = client_maps; ipm; ipm = ipm->ipm_next)
		{
			if (ipm->ipm_number == mapped_area &&
				!(ipm->ipm_flags & IPMF_SERVER_SHUTDOWN))
			{
				break;
			}
		}

		if (!ipm)
		{
			/* add new mapping */

			sprintf(name_buffer, IPI_MAPPED_FILE_NAME, Config::getIpcName(), mapped_area);
			file_handle = OpenFileMapping(FILE_MAP_WRITE, FALSE, name_buffer);
			if (!file_handle) {
				THD_mutex_unlock(&clisect);
				return 0;
			}
			mapped_address =
				MapViewOfFile(file_handle, FILE_MAP_WRITE, 0L, 0L,
							  IPS_MAPPED_SIZE(users_per_map, pages_per_user));
			if (!mapped_address)
			{
				last_error = GetLastError();
				CloseHandle(file_handle);
				THD_mutex_unlock(&clisect);
				return 0;
			}
			ipm = (IPM) ALLI_alloc(sizeof(struct ipm));
			if (!ipm)
			{
				UnmapViewOfFile(mapped_address);
				CloseHandle(file_handle);
				THD_mutex_unlock(&clisect);
				return 0;
			}
			ipm->ipm_next = client_maps;
			client_maps = ipm;
			ipm->ipm_count = 0;
			ipm->ipm_number = mapped_area;
			ipm->ipm_handle = file_handle;
			ipm->ipm_address = mapped_address;
			ipm->ipm_flags = 0;
		}

		/* there's no thread structure, so make one  */

		icc = (ICC) ALLOC(type_icc);
		if (!icc)
		{
			UnmapViewOfFile(mapped_address);
			CloseHandle(file_handle);
			THD_mutex_unlock(&clisect);
			return 0;
		}
		icc->icc_thread_handle = current_thread_id;
		icc->icc_file_handle = ipm->ipm_handle;
		icc->icc_mapped_addr = (UCHAR *) ipm->ipm_address +
			IPS_MAPPED_FOR_CLI(pages_per_user, mapped_position);
		icc->icc_file = mapped_area;
		icc->icc_slot = mapped_position;
		icc->icc_ipm = ipm;
		icc->icc_flags = 0;
#ifdef	DEBUG_IP_TRACE
		gds__log("ipclient icc %8lX, comm %8lX, number %lX",
				 icc, comm, number);
#endif
		comm = (ips_comm_area *) icc->icc_mapped_addr;

		/* only speak if server has correct protocol */

		if (comm->ips_server_protocol != 1L || comm->ips_server_proc == 0)
		{
			UnmapViewOfFile(mapped_address);
			CloseHandle(file_handle);
			THD_mutex_unlock(&clisect);
			return 0;
		}
		comm->ips_client_protocol = 1L;
		icc->icc_server_id = comm->ips_server_id;
		/*
		 * comm-ips_server_proc is set by ipserver.c when the client first
		 * initiates the conversation with the server.
		 *
		 */
		icc->icc_server_proc = comm->ips_server_proc;
		ipm->ipm_count++;

		/* get handles of semaphores */

		sprintf(name_buffer, IPI_CLIENT_SEM_NAME, Config::getIpcName(),
				mapped_area, mapped_position);
		icc->icc_client_sem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE,
											name_buffer);
		sprintf(name_buffer, IPI_SERVER_SEM_NAME, Config::getIpcName(),
				mapped_area, mapped_position);
		icc->icc_server_sem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE,
											name_buffer);

		/* initialize wait for multiple objects array */

		icc->icc_waits[0] = icc->icc_client_sem;
		icc->icc_waits[1] = icc->icc_server_proc;

		/* if we're not watching the server yet, do so now */

		if (!server_watcher_handle)
		{
			server_process_handle = icc->icc_server_proc;
			gds__thread_start(reinterpret_cast<FPTR_INT_VOID_PTR>(server_watcher),
								NULL,
								THREAD_high,
								0,
								&server_watcher_handle);
		}

		/* link to icc chain */

		icc->icc_next = client_threads;
		client_threads = icc;
	}
	THD_mutex_unlock(&clisect);
	if (picc) {
		*picc = icc;
	}
	return 1;
}


static ITR make_transaction( ISC_STATUS * user_status, IDB idb, FRBRD * handle)
{
/**************************************
 *
 *      m a k e _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *      Create a local transaction handle.
 *
 **************************************/
	ITR transaction;

	transaction = (ITR) ALLOC(type_itr);
	if (!transaction) {
		user_status[0] = isc_arg_gds;
		user_status[1] = isc_virmemexh;
		user_status[2] = isc_arg_end;
		return NULL;
	}
	transaction->itr_idb = idb;
	transaction->itr_handle = handle;
	transaction->itr_next = idb->idb_transactions;
	idb->idb_transactions = transaction;
	return transaction;
}


static SSHORT name_length( TEXT * name)
{
/*****************************************
 *
 *      n a m e _ l e n g t h
 *
 *****************************************
 *
 * Functional Description
 *      Compute length of user-supplied( blank-padded) or
 *      NULL-terminated buffer
 *
 *****************************************/
	TEXT *p;

	for (p = name; *p && *p != ' '; p++);
	return (p - name);
}


static BOOL pack_strings( ICC icc)
{
/**************************************
 *
 *      p a c k _ s t r i n g s
 *
 **************************************
 *
 * Functional description
 *      Pack the input buffers into the comm area.
 *      Note that this may require multiple send/receive
 *      operations to get all the buffers across.
 *
 **************************************/
	USHORT i;
	ips_comm_area *comm;
	ips_string *string;
	TEXT *acursor, *ccursor, *abase;
	ULONG to_copy, size_left;


	comm = (ips_comm_area *) icc->icc_mapped_addr;

	/* for each buffer, pack it into the comm area and send it,
	   waiting until the server is done to pack the next buffer
	   full */

	size_left = (ULONG) (IPS_USEFUL_SPACE(pages_per_user));
	acursor = (TEXT *) comm->ips_data;
	abase = acursor;
	for (i = 0; i < MAX_IPS_STRINGS; i++) {
		string = &comm->ips_buffers[i];

		/* see if this buffer needs sending */

		if (string->ips_flags & IPS_INPUT_BUFFER && string->ips_cl_size) {

			string->ips_cl_copied = 0;
			ccursor = reinterpret_cast < char *>(string->ips_cl_addr);

			/* keep packing it in */

			while (string->ips_cl_copied < string->ips_cl_size) {

				/* put in the whole thing, or whatever will fit */

				to_copy = string->ips_cl_size - string->ips_cl_copied;
				if (size_left <= to_copy)
					to_copy = size_left;
				string->ips_com_offset = acursor - abase;
				string->ips_com_size = to_copy;
				memcpy(acursor, ccursor, (USHORT) to_copy);
				acursor += to_copy;
				ccursor += to_copy;
				size_left -= to_copy;
				string->ips_cl_copied += to_copy;

				/* if all the space has been used up, send it and wait */

				if (!size_left && string->ips_cl_copied < string->ips_cl_size) {
					if (!send_and_wait(icc))
						return FALSE;
					size_left = (ULONG) (IPS_USEFUL_SPACE(pages_per_user));
					acursor = (TEXT *) comm->ips_data;
				}
			}
		}
	}
	return TRUE;
}


static void release_blob( IBL blob)
{
/**************************************
 *
 *      r e l e a s e _ b l o b
 *
 **************************************
 *
 * Functional description
 *      Release a blob block and friends.
 *
 **************************************/
	ITR transaction;
	IBL *p;


	transaction = blob->ibl_itr;
	for (p = &transaction->itr_blobs; *p; p = &(*p)->ibl_next)
		if (*p == blob) {
			*p = blob->ibl_next;
			break;
		}
	ALLI_release((BLK) blob);
}


static void release_database( IDB idb)
{
/**************************************
 *
 *      r e l e a s e _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *      Take a database structure out of
 *      the thread's chain and close down
 *      the thread's comm if it's the last
 *      structure on the chain.
 *
 **************************************/
	ICC icc;
	ICC picc;
	IPM ipm;
	IPM pipm;
	IDB pidb;
	ips_comm_area *comm;


	/* find the structure in the thread's chain and remove it */

	THD_mutex_lock(&clisect);
	icc = idb->idb_thread;
	if (icc->icc_databases == idb)
		icc->icc_databases = idb->idb_next;
	else {
		for (pidb = icc->icc_databases; pidb->idb_next; pidb = pidb->idb_next)
			if (pidb->idb_next == idb) {
				pidb->idb_next = idb->idb_next;
				break;
			}
	}
	ALLI_release((BLK) idb);

	/* close down if chain is now empty */

	if (!icc->icc_databases) {

		/* point to communications area */

		ipm = icc->icc_ipm;
		if (!(icc->icc_flags & ICCF_SERVER_SHUTDOWN)) {
			comm = (ips_comm_area *) icc->icc_mapped_addr;
			comm->ips_operation = op_disconnect;
			ReleaseSemaphore(icc->icc_server_sem, 1L, NULL);
		}

		/* free up semaphores */

		if (icc->icc_client_sem)
			CloseHandle(icc->icc_client_sem);
		if (icc->icc_server_sem)
			CloseHandle(icc->icc_server_sem);
		if (icc->icc_server_proc)
			CloseHandle(icc->icc_server_proc);

		/* find icc in chain and release */

		if (client_threads == icc)
			client_threads = icc->icc_next;
		else {
			for (picc = client_threads; picc->icc_next; picc = picc->icc_next)
				if (picc->icc_next == icc) {
					picc->icc_next = icc->icc_next;
					break;
				}
		}
		ALLI_release((BLK) icc);

		/* if this was the last area for this map, unmap it */

		ipm->ipm_count--;
		if (!ipm->ipm_count) {
			if (!(ipm->ipm_flags & IPMF_SERVER_SHUTDOWN)) {
				UnmapViewOfFile(ipm->ipm_address);
				CloseHandle(ipm->ipm_handle);
			}

			/* find in chain and release */

			if (client_maps == ipm)
				client_maps = ipm->ipm_next;
			else {
				for (pipm = client_maps; pipm->ipm_next;
					 pipm = pipm->ipm_next) if (pipm->ipm_next == ipm) {
						pipm->ipm_next = ipm->ipm_next;
						break;
					}
			}
			ALLI_free((UCHAR *) ipm);
		}
	}
	THD_mutex_unlock(&clisect);
}


static void release_event( IVNT event)
{
/**************************************
 *
 *      r e l e a s e _ e v e n t
 *
 **************************************
 *
 * Functional description
 *      Release a event block and friends.
 *
 **************************************/
	IDB idb;
	IVNT *p;


	idb = event->ivnt_idb;
	for (p = &idb->idb_events; *p; p = &(*p)->ivnt_next)
		if (*p == event) {
			*p = event->ivnt_next;
			break;
		}
	ALLI_release((BLK) event);
}


static void release_request( IRQ request)
{
/**************************************
 *
 *      r e l e a s e _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *      Release a request block and friends.
 *
 **************************************/
	IDB idb;
	IRQ *p;


	idb = request->irq_idb;
	for (p = &idb->idb_requests; *p; p = &(*p)->irq_next)
		if (*p == request) {
			*p = request->irq_next;
			break;
		}
	ALLI_release((BLK) request);
}


static void release_transaction( ITR transaction)
{
/**************************************
 *
 *      r e l e a s e _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *      Release a transaction block and friends.
 *
 **************************************/
	IDB idb;
	ITR *p;


	idb = transaction->itr_idb;
	for (p = &idb->idb_transactions; *p; p = &(*p)->itr_next)
		if (*p == transaction) {
			*p = transaction->itr_next;
			break;
		}
	ALLI_release((BLK) transaction);
}


static void release_sql_request( IPSERVER_ISR stmt)
{
/**************************************
 *
 *      r e l e a s e _ s q l _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *      Release an SQL request block.
 *
 **************************************/
	IDB idb;
	IPSERVER_ISR *p;


	idb = stmt->isr_idb;
	for (p = &idb->idb_sql_requests; *p; p = &(*p)->isr_next)
		if (*p == stmt) {
			*p = stmt->isr_next;
			break;
		}
	if (stmt->isr_packed)
		ALLI_free((UCHAR *) stmt->isr_packed);
	ALLI_release((BLK) stmt);
}


static BOOL send_and_wait( ICC icc)
{
/**************************************
 *
 *      s e n d _ a n d _ w a i t
 *
 **************************************
 *
 * Functional description
 *      This function starts the server by releasing
 *      its semaphore, and then waits for the server's
 *      response by waiting on the client's semaphore.
 *
 **************************************/
	DWORD result;

/* dead server means no point in sending */

	if (icc->icc_flags & ICCF_SERVER_SHUTDOWN)
		return FALSE;

/* first, signal the server that the communications
   area in mapped memory is filed and ready */

	if (!ReleaseSemaphore(icc->icc_server_sem, 1L, NULL))
		return FALSE;

/* next, wait for the server to signal us back or die */

	THREAD_EXIT;
	result =
		WaitForMultipleObjects((DWORD) 2, icc->icc_waits, FALSE, INFINITE);
	THREAD_ENTER;

	if (result != WAIT_OBJECT_0) {
		icc->icc_flags |= ICCF_UNMAP_CLIENT;
		return FALSE;
	}

	return TRUE;
}


static void server_shutdown(void)
{
/**************************************
 *
 *      s e r v e r _ s h u t d o w n
 *
 **************************************
 *
 * Functional description
 *      Server shutdown detected, so mark
 *      everything and release handles.
 *
 **************************************/
	ICC icc;
	IPM ipm;
	IDB idb;


	/* for each icc structure, shut it and its database */

	for (icc = client_threads; icc; icc = icc->icc_next) {
		icc->icc_flags |= ICCF_SERVER_SHUTDOWN;

		/* free up semaphores */

		if (icc->icc_client_sem)
			CloseHandle(icc->icc_client_sem);
		if (icc->icc_server_sem)
			CloseHandle(icc->icc_server_sem);
		if (icc->icc_server_proc)
			CloseHandle(icc->icc_server_proc);
		icc->icc_client_sem = 0;
		icc->icc_server_sem = 0;
		icc->icc_server_proc = 0;

		/* mark databases */

		for (idb = icc->icc_databases; idb; idb = idb->idb_next)
			idb->idb_flags |= IDBF_SERVER_SHUTDOWN;
	}

	/* unmap all mapped files */

	for (ipm = client_maps; ipm; ipm = ipm->ipm_next) {
		ipm->ipm_flags |= IPMF_SERVER_SHUTDOWN;
		UnmapViewOfFile(ipm->ipm_address);
		CloseHandle(ipm->ipm_handle);
		ipm->ipm_handle = 0;
	}
}


static void server_watcher(void)
{
/**************************************
 *
 *      s e r v e r _ w a t c h e r
 *
 **************************************
 *
 * Functional description
 *      This thread just waits for the server
 *      process.   If that process goes away,
 *      this gets triggered and unmaps any
 *      currently mapped maps.
 *
 **************************************/
	DWORD result;


	for (;;) {
		if (exit_flag)
			break;
		result = WaitForSingleObject(server_process_handle, INFINITE);
		if (result == WAIT_OBJECT_0) {

			/* unmap and close all maps */

			THD_mutex_lock(&mapsect);
			server_shutdown();
			THD_mutex_unlock(&mapsect);
			server_process_handle = 0;
			CloseHandle(server_watcher_handle);
			server_watcher_handle = 0;
			break;
		}
	}
}


static BOOL unpack_strings( ICC icc)
{
/**************************************
 *
 *      u n p a c k _ s t r i n g s
 *
 **************************************
 *
 * Functional description
 *      Unpack all buffers from comm area to user's
 *      address space.   Note that this may require
 *      multiple send/receive operations to get all
 *      the data across.
 *
 **************************************/
	USHORT i;
	ips_comm_area *comm;
	ips_string *string;
	TEXT *acursor, *ccursor;
	ULONG to_copy;


	comm = (ips_comm_area *) icc->icc_mapped_addr;
	for (i = 0; i < MAX_IPS_STRINGS; i++) {
		string = &comm->ips_buffers[i];

		/* see if this buffer needs extraction */

		if (string->ips_flags & IPS_OUTPUT_BUFFER && string->ips_cl_size) {
			string->ips_cl_copied = 0;
			ccursor = reinterpret_cast < char *>(string->ips_cl_addr);

			/* start extraction */

			while (string->ips_cl_copied < string->ips_cl_size) {

				/* extract all or whatever's available */

				to_copy = string->ips_cl_size - string->ips_cl_copied;
				if (string->ips_com_size <= to_copy)
					to_copy = string->ips_com_size;
				acursor = (TEXT *) comm->ips_data + string->ips_com_offset;
				memcpy(ccursor, acursor, (USHORT) to_copy);
				ccursor += to_copy;
				string->ips_cl_copied += to_copy;

				/* if there is more to copy, wait for it */

				if (string->ips_cl_copied < string->ips_cl_size)
					if (!send_and_wait(icc))
						return FALSE;
			}
		}
	}
	return TRUE;
}


void IPC_process_event(
					   DWORD eventID, DWORD event_length, LPVOID event_string)
{
/**************************************
 *
 *      I P C _ p r o c e s s _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *      Process an event.   This looks the event up by
 *      its id and executes the ast with the argument
 *      and the event string.
 *
 **************************************/
	EVENTQ queued;


	/* allocate a queued event structure, fill it and link it */

	THD_mutex_lock(&evtsect);
	queued = (EVENTQ) ALLI_alloc(sizeof(struct eventq) + event_length);
	if (queued) {
		memcpy(queued->evq_string, event_string, event_length);
		queued->evq_length = (USHORT) event_length;
		queued->evq_id = eventID;
		queued->evq_next = NULL;
		if (!event_head) {
			event_head = queued;
			event_tail = queued;
		}
		else {
			event_tail->evq_next = queued;
			event_tail = queued;
		}
	}
	THD_mutex_unlock(&evtsect);
	ReleaseSemaphore(event_semaphore, 1L, NULL);
}


#ifndef XNET
void IPC_release_all(void)
{
/**************************************
 *
 *      I P C _ r e l e a s e _ a l l
 *
 **************************************
 *
 * Functional description
 *      Release all connections and dependant stuff.
 *
 **************************************/
	ICC icc, nicc;
	IPM ipm, nipm;
	IDB idb, nidb;
	ITR itr, nitr;
	IBL ibl, nibl;
	IRQ irq, nirq;
	IPSERVER_ISR isr, nisr;
	IVNT ivnt, nivnt;
	EVENTQ event, nevent;
	ips_comm_area *comm;


	/* get stuff to release and clear list heads */

	exit_flag++;
	if (exit_flag > 1)
		return;
	icc = client_threads;
	nicc = icc;
	client_threads = NULL;
	ipm = client_maps;
	client_maps = NULL;

	/* cause event threads to shut down */

	if (event_window) {
		PostMessage(event_window, WM_CLOSE, 0, 0);
		event_window = 0;
	}
	if (event_semaphore) {
		ReleaseSemaphore(event_semaphore, 1L, NULL);
		CloseHandle(event_semaphore);
		event_semaphore = 0;
	}
	if (event_packer_handle) {
		TerminateThread(event_packer_handle, 0);
		CloseHandle(event_packer_handle);
		event_packer_handle = 0;
	}
	if (event_thread_handle) {
		TerminateThread(event_thread_handle, 0);
		CloseHandle(event_thread_handle);
		event_thread_handle = 0;
	}
	if (server_watcher_handle) {
		TerminateThread(server_watcher_handle, 0);
		CloseHandle(server_watcher_handle);
		server_watcher_handle = 0;
	}

	/* release queued event structures */

	event = event_head;
	event_head = NULL;
	event_tail = NULL;
	for (; event; event = nevent) {
		nevent = event->evq_next;
		ALLI_free((UCHAR *) event);
	}

	/* disconnect each thread */

	for (; icc; icc = icc->icc_next) {
		if (!(icc->icc_flags & ICCF_SERVER_SHUTDOWN)) {
			comm = (ips_comm_area *) icc->icc_mapped_addr;
			comm->ips_operation = op_disconnect;
			ReleaseSemaphore(icc->icc_server_sem, 1L, NULL);
		}
	}
	icc = nicc;

	/* for each icc structure, release its database and close it down */

	for (; icc; icc = nicc) {
		nicc = icc->icc_next;

		/* free up semaphores */

		if (icc->icc_client_sem)
			CloseHandle(icc->icc_client_sem);
		if (icc->icc_server_sem)
			CloseHandle(icc->icc_server_sem);
		if (icc->icc_server_proc)
			CloseHandle(icc->icc_server_proc);

		/* free up dependant data structures */

		for (idb = icc->icc_databases; idb; idb = nidb) {
			nidb = idb->idb_next;
			for (ivnt = idb->idb_events; ivnt; ivnt = nivnt) {
				nivnt = ivnt->ivnt_next;
				ALLI_release((BLK) ivnt);
			}
			for (irq = idb->idb_requests; irq; irq = nirq) {
				nirq = irq->irq_next;
				ALLI_release((BLK) irq);
			}
			for (isr = idb->idb_sql_requests; isr; isr = nisr) {
				nisr = isr->isr_next;
				if (isr->isr_packed)
					ALLI_free((UCHAR *) isr->isr_packed);
				ALLI_release((BLK) isr);
			}
			for (itr = idb->idb_transactions; itr; itr = nitr) {
				nitr = itr->itr_next;
				for (ibl = itr->itr_blobs; ibl; ibl = nibl) {
					nibl = ibl->ibl_next;
					ALLI_release((BLK) ibl);
				}
				ALLI_release((BLK) itr);
			}
			ALLI_release((BLK) idb);
		}
		ALLI_release((BLK) icc);
	}

	/* unmap all mapped files */

	for (; ipm; ipm = nipm) {
		nipm = ipm->ipm_next;
		if (!(ipm->ipm_flags & IPMF_SERVER_SHUTDOWN)) {
			UnmapViewOfFile(ipm->ipm_address);
			CloseHandle(ipm->ipm_handle);
		}
		ALLI_free((UCHAR *) ipm);
	}
	if (initialized) {
		THD_mutex_destroy(&clisect);
		THD_mutex_destroy(&evtsect);
		THD_mutex_destroy(&mapsect);
		initialized = FALSE;
	}
}
#endif


} // extern "C"
