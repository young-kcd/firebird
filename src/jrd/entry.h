/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		entry.h
 *	DESCRIPTION:	Y-value entrypoints
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

/* 
    Order of battle in ENTRY macro:
	generic name
	current local access method entrypoint
	prior local access method (bridge) entrypoint
	remote entrypoint
	OS2 remote name
	Central server interface
	RDB interface entrypoint
	Pipe server interface
	Bridge pipe server interface
	Windows local access method entrypoint
	Win95 interprocess interface
*/

/*****************************************************
 *  IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT
 *
 *  The order in which these entrypoints appear MUST match
 *  the order in which the defines for PROC_XXX appear in
 *  source/jrd/why.cpp.  Failure to do so will result in
 *  much frustration
 ******************************************************/

/*
 * TMN: Equally important: Don't add a compiler directive like
 * extern "C" { } to this file since it's included to be the body
 * of an array (!) from why.c.
 */

#if defined(NOT_USED_OR_REPLACED) && defined(__cplusplus)


ENTRYPOINT( "gds_attach_database",
		    jrd8_attach_database,
		   "jrd5_attach_database",
		     REM_attach_database,
		   "_REM_attach_database",
		     CSI_attach_database,
		     RDB_attach_database,
		     PSI_attach_database,
		    PSI5_attach_database,
		  "_jrd8_attach_database",
		     IPI_attach_database,
		   (ISC_STATUS*, SSHORT, const SCHAR*, struct att**, SSHORT,
				const SCHAR*, SCHAR*))

ENTRYPOINT( "gds_blob_info",
		    jrd8_blob_info,
		   "jrd5_blob_info",
		     REM_blob_info,
		   "_REM_blob_info",
		     CSI_blob_info,
		     RDB_blob_info,
		     PSI_blob_info,
		    PSI5_blob_info,
		  "_jrd8_blob_info",
		     IPI_blob_info,
		   (ISC_STATUS*, struct blb**, SSHORT, const SCHAR*, SSHORT, SCHAR*))

ENTRYPOINT( "gds_cancel_blob",
		    jrd8_cancel_blob,
		   "jrd5_cancel_blob",
		     REM_cancel_blob,
		   "_REM_cancel_blob",
		     CSI_cancel_blob,
		     RDB_cancel_blob,
		     PSI_cancel_blob,
		    PSI5_cancel_blob,
		  "_jrd8_cancel_blob",
		     IPI_cancel_blob,
		   (ISC_STATUS*, struct blb**))

ENTRYPOINT( "gds_close_blob",
		    jrd8_close_blob,
		   "jrd5_close_blob",
		     REM_close_blob,
		   "_REM_close_blob",
		     CSI_close_blob,
		     RDB_close_blob,
		     PSI_close_blob,
		    PSI5_close_blob,
		  "_jrd8_close_blob",
		     IPI_close_blob,
		   (ISC_STATUS*, struct blb**))

ENTRYPOINT( "gds_commit_transaction",
		    jrd8_commit_transaction,
		   "jrd5_commit_transaction",
		     REM_commit_transaction,
		   "_REM_commit_transaction",
		     CSI_commit_transaction,
		     RDB_commit_transaction,
		     PSI_commit_transaction,
		    PSI5_commit_transaction,
		  "_jrd8_commit_transaction",
		     IPI_commit_transaction,
		   (ISC_STATUS*, struct jrd_tra**))

ENTRYPOINT( "gds_compile_request",
		    jrd8_compile_request,
		   "jrd5_compile_request",
		     REM_compile_request,
		   "_REM_compile_request",
		     CSI_compile_request,
		     RDB_compile_request,
		     PSI_compile_request,
		    PSI5_compile_request,
		  "_jrd8_compile_request",
		     IPI_compile_request,
		   (ISC_STATUS*, struct att**, struct jrd_req**, SSHORT, SCHAR*))

ENTRYPOINT( "gds_create_blob",
   /****    jrd8_create_blob    ***/ y_valve_no_entrypoint,
   /****   "jrd5_create_blob"   ***/ NULL,
   /****     REM_create_blob    ***/ y_valve_no_entrypoint,
		   "_REM_create_blob",
   /****     CSI_create_blob    ***/ y_valve_no_entrypoint,
			 RDB_create_blob,
			 PSI_create_blob,
			PSI5_create_blob,
   /****  "_jrd8_create_blob"    ***/ "no_entrypoint",
			 IPI_create_blob,
			(ISC_STATUS*, struct rdb**, struct rtr**, struct rbl**, struct bid*))

ENTRYPOINT( "gds_create_database",
		    jrd8_create_database,
   /****   "jrd5_create_database"   ***/ NULL,
			 REM_create_database,
		   "_REM_create_database",
			 CSI_create_database,
			 RDB_create_database,
			 PSI_create_database,
   /****    PSI5_create_database    ***/ y_valve_no_entrypoint,
		  "_jrd8_create_database",
		     IPI_create_database,
			(ISC_STATUS*, USHORT, const UCHAR*, struct att**, USHORT,
				const UCHAR*, USHORT, UCHAR*))

ENTRYPOINT( "gds_database_info",
		    jrd8_database_info,
		   "jrd5_database_info",
			 REM_database_info,
		   "_REM_database_info",
		     CSI_database_info,
		     RDB_database_info,
			 PSI_database_info,
			PSI5_database_info,
		  "_jrd8_database_info",
		     IPI_database_info,
		   (ISC_STATUS*, struct att**, SSHORT, const SCHAR*, SSHORT, SCHAR*))

ENTRYPOINT( "gds_detach_database",
		    jrd8_detach_database,
		   "jrd5_detach_database",
		     REM_detach_database,
		   "_REM_detach_database",
		     CSI_detach_database,
			 RDB_detach_database,
			 PSI_detach_database,
			PSI5_detach_database,
		  "_jrd8_detach_database",
		     IPI_detach_database,
		   (ISC_STATUS*, struct att**))

ENTRYPOINT( "gds_get_segment",
		    jrd8_get_segment,
		   "jrd5_get_segment",
		     REM_get_segment,
		   "_REM_get_segment",
		     CSI_get_segment,
			 RDB_get_segment,
			 PSI_get_segment,
			PSI5_get_segment,
		  "_jrd8_get_segment",
		     IPI_get_segment,
		   (ISC_STATUS*, struct blb**, USHORT*, USHORT, UCHAR*))

ENTRYPOINT( "gds_open_blob",
   /****    jrd8_open_blob    ***/ y_valve_no_entrypoint,
   /****   "jrd5_open_blob"   ***/ NULL,
   /****     REM_open_blob    ***/ y_valve_no_entrypoint,
		   "_REM_open_blob",
   /****     CSI_open_blob    ***/ y_valve_no_entrypoint,
			 RDB_open_blob,
			 PSI_open_blob,
			PSI5_open_blob,
   /****  "_jrd8_open_blob"   ***/ "no_entrypoint",
			 IPI_open_blob,
			(ISC_STATUS*, struct rdb**, struct rtr**, struct rbl**, struct bid*))

ENTRYPOINT( "gds_prepare_transaction",
		    jrd8_prepare_transaction,
		   "jrd5_prepare_transaction",
			 REM_prepare_transaction,
		   "_REM_prepare_transaction",
			 CSI_prepare_transaction,
			 RDB_prepare_transaction,
			 PSI_prepare_transaction,
			PSI5_prepare_transaction,
		  "_jrd8_prepare_transaction",
			 IPI_prepare_transaction,
			 (ISC_STATUS*, struct jrd_tra**, USHORT, UCHAR*))

ENTRYPOINT( "gds_put_segment",
		    jrd8_put_segment,
		   "jrd5_put_segment",
			 REM_put_segment,
		   "_REM_put_segment",
			 CSI_put_segment,
			 RDB_put_segment,
			 PSI_put_segment,
			PSI5_put_segment,
		  "_jrd8_put_segment",
			 IPI_put_segment,
			 (ISC_STATUS*, struct blb**, USHORT, const UCHAR*))

ENTRYPOINT( "gds_reconnect_transaction",
		    jrd8_reconnect_transaction,
		   "jrd5_reconnect_transaction",
		     REM_reconnect_transaction,
		   "_REM_reconnect_transaction",
			 CSI_reconnect_transaction,
			 RDB_reconnect_transaction,
			 PSI_reconnect_transaction,
			PSI5_reconnect_transaction,
		  "_jrd8_reconnect_transaction",
		     IPI_reconnect_transaction,
			 (ISC_STATUS*, struct att**, struct jrd_tra**, SSHORT, const UCHAR*))

ENTRYPOINT( "gds_receive",
		    jrd8_receive,
		   "jrd5_receive",
		     REM_receive,
		   "_REM_receive",
		     CSI_receive,
			 RDB_receive,
			 PSI_receive,
			PSI5_receive,	/* the bridge pipe server will have two less params, but should be OK */
		  "_jrd8_receive",
		     IPI_receive,
			(ISC_STATUS*, struct jrd_req**, USHORT, USHORT, SCHAR*, SSHORT))

ENTRYPOINT( "gds_release_request",
		    jrd8_release_request,
		   "jrd5_release_request",
		     REM_release_request,
		   "_REM_release_request",
		     CSI_release_request,
			 RDB_release_request,
			 PSI_release_request,
			PSI5_release_request,
		  "_jrd8_release_request",
		     IPI_release_request,
		   (ISC_STATUS*, struct jrd_req**))

ENTRYPOINT( "gds_request_info",
			jrd8_request_info,
			"jrd5_request_info",
			REM_request_info,
			"_REM_request_info",
			CSI_request_info,
			RDB_request_info,
			PSI_request_info,
			PSI5_request_info,
			"_jrd8_request_info",
			IPI_request_info,
			(ISC_STATUS*, struct jrd_req**, SSHORT, SSHORT, const SCHAR*, SSHORT, SCHAR*))

ENTRYPOINT("gds_rollback_transaction",
			jrd8_rollback_transaction,
			"jrd5_rollback_transaction",
			REM_rollback_transaction,
			"_REM_rollback_transaction",
			CSI_rollback_transaction,
			RDB_rollback_transaction,
			PSI_rollback_transaction,
			PSI5_rollback_transaction,
			"_jrd8_rollback_transaction",
			IPI_rollback_transaction,
			(ISC_STATUS *, struct jrd_tra **))

ENTRYPOINT("gds_send",
			jrd8_send,
			"jrd5_send",
			REM_send,
			"_REM_send",
			CSI_send,
			RDB_send,
			PSI_send,
			PSI5_send,
			"_jrd8_send",
			IPI_send,
			(ISC_STATUS *, struct jrd_req **, USHORT, USHORT, SCHAR *, SSHORT))

ENTRYPOINT("gds_start_and_send",
			jrd8_start_and_send,
			"jrd5_start_and_send",
			REM_start_and_send,
			"_REM_start_and_send",
			CSI_start_and_send,
			RDB_start_and_send,
			PSI_start_and_send,
			PSI5_start_and_send,
			"_jrd8_start_and_send",
			IPI_start_and_send,
			(ISC_STATUS*, struct jrd_req**, struct jrd_tra**, USHORT, USHORT,
			    SCHAR*, SSHORT))

ENTRYPOINT("gds_start_request",
			jrd8_start_request,
			"jrd5_start_request",
			REM_start_request,
			"_REM_start_request",
			CSI_start_request,
			RDB_start_request,
			PSI_start_request,
			PSI5_start_request,
			"_jrd8_start_request",
			IPI_start_request,
			(ISC_STATUS*, struct jrd_req**, struct jrd_tra**, SSHORT))

ENTRYPOINT( "gds_start_multiple",
			jrd8_start_multiple,
		   "jrd5_start_multiple",
   /****     REM_start_multiple    ***/ y_valve_no_entrypoint,
		   "_REM_start_multiple",
			 CSI_start_multiple,
			 RDB_start_multiple,
			 PSI_start_multiple,
			PSI5_start_multiple,
		  "_jrd8_start_multiple",
			 IPI_start_multiple,
		   (ISC_STATUS *, struct jrd_tra **, USHORT, struct teb *))

ENTRYPOINT("gds_start_transaction",
		   jrd8_start_transaction,
		   "jrd5_start_transaction",
		   REM_start_transaction,
		   "_REM_start_transaction",
		   CSI_start_transaction,
		   RDB_start_transaction,
		   PSI_start_transaction,
		   PSI5_start_transaction,
		   "_jrd8_start_transaction",
		   IPI_start_transaction,
		   (ISC_STATUS*, struct jrd_tra**, SSHORT, ...))

ENTRYPOINT( "gds_transaction_info",
		    jrd8_transaction_info,
		   "jrd5_transaction_info",
		     REM_transaction_info,
		   "_REM_transaction_info",
		     CSI_transaction_info,
		     RDB_transaction_info,
		     PSI_transaction_info,
		    PSI5_transaction_info,
		  "_jrd8_transaction_info",
		     IPI_transaction_info,
		   (ISC_STATUS*, struct jrd_tra**, SSHORT, const SCHAR*, SSHORT, SCHAR*))

ENTRYPOINT("gds_unwind_request",
		   jrd8_unwind_request,
		   "jrd5_unwind_request",
		   REM_unwind_request,
		   "_REM_unwind_request",
		   CSI_unwind_request,
		   RDB_unwind_request,
		   PSI_unwind_request,
		   PSI5_unwind_request,
		   "_jrd8_unwind_request",
		   IPI_unwind_request,
		   (ISC_STATUS*, struct jrd_req**, SSHORT))

ENTRYPOINT("gds_commit_retaining",
		   jrd8_commit_retaining,
		   "jrd5_commit_retaining",
		   REM_commit_retaining,
		   "_REM_commit_retaining",
		   CSI_commit_retaining,
   /****     RDB_commit_retaining    ***/ y_valve_no_entrypoint,
			PSI_commit_retaining,
			PSI5_commit_retaining,
			"_jrd8_commit_retaining",
			IPI_commit_retaining,
			(ISC_STATUS *, struct jrd_tra **))

ENTRYPOINT("gds_que_events",
		   jrd8_que_events,
		   "jrd5_que_events",
		   REM_que_events,
		   "_REM_que_events",
		   CSI_que_events,
   /****     RDB_que_events    ***/ y_valve_no_entrypoint,
			   PSI_que_events,
			   PSI5_que_events,
			   "_jrd8_que_events",
			   IPI_que_events,
			   (ISC_STATUS*, struct att**, SLONG*, SSHORT, UCHAR*, FPTR_VOID,
				void*))

ENTRYPOINT("gds_cancel_events",
		   jrd8_cancel_events,
		   "jrd5_cancel_events",
		   REM_cancel_events,
		   "_REM_cancel_events",
		   CSI_cancel_events,
   /****     RDB_cancel_events    ***/ y_valve_no_entrypoint,
			   PSI_cancel_events,
			   PSI5_cancel_events,
			   "_jrd8_cancel_events",
			   IPI_cancel_events,
			   (ISC_STATUS*, struct att**, SLONG*))

ENTRYPOINT("gds_ddl",
		   jrd8_ddl,
   /****   "jrd5_ddl"   ***/ NULL,
			   REM_ddl,
			   "_REM_ddl",
			   CSI_ddl,
   /****     RDB_ddl    ***/ y_valve_no_entrypoint,
			   PSI_ddl,
			   PSI5_ddl,
			   "_jrd8_ddl",
			   IPI_ddl,
			   (ISC_STATUS*, struct att**, struct jrd_tra**, USHORT, SCHAR*))

ENTRYPOINT("gds_open_blob2",
		   jrd8_open_blob2,
		   "jrd5_open_blob2",
		   REM_open_blob2,
		   "_REM_open_blob2",
		   CSI_open_blob2,
   /****     RDB_open_blob2    ***/ y_valve_no_entrypoint,
			PSI_open_blob2,
			PSI5_open_blob2,
			"_jrd8_open_blob2",
			IPI_open_blob2,
			(ISC_STATUS*, struct att**, struct jrd_tra**, struct blb**,
				struct bid*, USHORT, const UCHAR*))

ENTRYPOINT("gds_create_blob2",
		   jrd8_create_blob2,
		   "jrd5_create_blob2",
		   REM_create_blob2,
		   "_REM_create_blob2",
		   CSI_create_blob2,
   /****     RDB_create_blob2    ***/ y_valve_no_entrypoint,
			PSI_create_blob2,
			PSI5_create_blob2,
			"_jrd8_create_blob2",
			IPI_create_blob2,
			(ISC_STATUS*, struct att**, struct jrd_tra**, struct blb**,
				struct bid*, USHORT, const UCHAR*))

ENTRYPOINT( "gds_get_slice",
		    jrd8_get_slice,
		   "jrd5_get_slice",
		     REM_get_slice,
		   "_REM_get_slice",
		     CSI_get_slice,
   /****     RDB_get_slice    ***/ y_valve_no_entrypoint,
			 PSI_get_slice,
			PSI5_get_slice,
		  "_jrd8_get_slice",
			 IPI_get_slice,
			   (ISC_STATUS*, struct att **, struct jrd_tra **, SLONG *, USHORT,
				UCHAR *, USHORT, UCHAR *, SLONG, UCHAR *, SLONG *))

ENTRYPOINT( "gds_put_slice",
		    jrd8_put_slice,
		   "jrd5_put_slice",
		     REM_put_slice,
		   "_REM_put_slice",
		     CSI_put_slice,
   /****     RDB_put_slice    ***/ y_valve_no_entrypoint,
			 PSI_put_slice,
			PSI5_put_slice,
		  "_jrd8_put_slice",
		    IPI_put_slice,
			   (ISC_STATUS*, struct att**, struct jrd_tra**, SLONG*, USHORT,
				UCHAR*, USHORT, UCHAR*, SLONG, UCHAR*))

ENTRYPOINT( "gds_seek_blob",
		    jrd8_seek_blob,
		   "jrd5_seek_blob",
		     REM_seek_blob,
		   "_REM_seek_blob",
		     CSI_seek_blob,
   /****     RDB_seek_blob    ***/ y_valve_no_entrypoint,
			 PSI_seek_blob,
			PSI5_seek_blob,
		  "_jrd8_seek_blob",
		     IPI_seek_blob,
			   (ISC_STATUS*, struct blb**, SSHORT, SLONG, SLONG*))

ENTRYPOINT( "isc_transact_request",
		    jrd8_transact_request,
   /****   "jrd5_transact_request"   ***/ NULL,
			 REM_transact_request,
		   "_REM_transact_request",
			 CSI_transact_request,
   /****     RDB_transact_request    ***/ y_valve_no_entrypoint,
			 PSI_transact_request,
   /****    PSI5_transact_request    ***/ y_valve_no_entrypoint,
		  "_jrd8_transact_request",
			 IPI_transact_request,
			   (ISC_STATUS*, struct att**, struct jrd_tra**, USHORT, SCHAR*,
				USHORT, SCHAR*, USHORT, SCHAR*))

ENTRYPOINT( "isc_drop_database",
		    jrd8_drop_database,
   /****   "jrd5_drop_database"   ***/ NULL,
			 REM_drop_database,
		   "_REM_drop_database",
			 CSI_drop_database,
   /****     RDB_drop_database    ***/ y_valve_no_entrypoint,
			 PSI_drop_database,
   /****    PSI5_drop_database    ***/ y_valve_no_entrypoint,
		  "_jrd8_drop_database",
			 IPI_drop_database,
			   (ISC_STATUS*, struct att**))

/* and the DSQL entry points */
ENTRYPOINT("isc_dsql_allocate_statement",
   /****         jrd8_allocate_statement    ***/ y_valve_no_entrypoint,
			   "LDSQL_allocate_statement",
			      REM_allocate_statement,
   /****        "_REM_allocate_statement"   ***/ "no_entrypoint",
			      CSI_allocate_statement,
   /****          RDB_allocate_statement    ***/ y_valve_no_entrypoint,
			      PSI_allocate_statement,
			     PSI5_allocate_statement,
   /****       "_jrd8_allocate_statement"   ***/ "no_entrypoint",
			      IPI_allocate_statement,
			   (ISC_STATUS*, struct rdb**, struct rsr**))

ENTRYPOINT( "isc_dsql_execute",
   /****         jrd8_execute    ***/ y_valve_no_entrypoint,
			   "LDSQL_execute",
			      REM_execute,
   /****        "_REM_execute"   ***/ "no_entrypoint",
			      CSI_execute,
   /****          RDB_execute    ***/ y_valve_no_entrypoint,
			      PSI_execute,
			     PSI5_execute,
   /****       "_jrd8_execute"   ***/ "no_entrypoint",
			      IPI_execute,
			   (ISC_STATUS*, struct rtr**, struct rsr**, USHORT, UCHAR*,
				USHORT, USHORT, UCHAR*))

ENTRYPOINT("isc_dsql_execute2",
   /****         jrd8_execute2    ***/ y_valve_no_entrypoint,
   /****        "jrd5_execute2"   ***/ NULL,
			      REM_execute2,
   /****        "_REM_execute2"   ***/ "no_entrypoint",
			      CSI_execute2,
   /****          RDB_execute2    ***/ y_valve_no_entrypoint,
			      PSI_execute2,
   /****         PSI5_execute2    ***/ y_valve_no_entrypoint,
   /****       "_jrd8_execute2"   ***/ "no_entrypoint",
			      IPI_execute2,
			   (ISC_STATUS*, struct rtr**, struct rsr**, USHORT, UCHAR*,
				USHORT, USHORT, UCHAR*, USHORT, UCHAR*, USHORT, USHORT,
				UCHAR*))

ENTRYPOINT( "isc_dsql_execute_immediate",
   /****         jrd8_execute_immediate    ***/ y_valve_no_entrypoint,
			   "LDSQL_execute_immediate",
			      REM_execute_immediate,
   /****        "_REM_execute_immediate"   ***/ "no_entrypoint",
			      CSI_execute_immediate,
   /****          RDB_execute_immediate    ***/ y_valve_no_entrypoint,
			      PSI_execute_immediate,
			     PSI5_execute_immediate,
   /****       "_jrd8_execute_immediate"   ***/ "no_entrypoint",
			      IPI_execute_immediate,
			   (ISC_STATUS*, struct rdb**, struct rtr**, USHORT, UCHAR*,
				USHORT, USHORT, UCHAR *, USHORT, USHORT, UCHAR*))

ENTRYPOINT( "isc_dsql_execute_immediate2",
   /****         jrd8_execute_immediate2    ***/ y_valve_no_entrypoint,
   /****        "jrd5_execute_immediate2"   ***/ NULL,
			      REM_execute_immediate2,
   /****        "_REM_execute_immediate2"   ***/ "no_entrypoint",
			      CSI_execute_immediate2,
   /****          RDB_execute_immediate2    ***/ y_valve_no_entrypoint,
				  PSI_execute_immediate2,
   /****         PSI5_execute_immediate2    ***/ y_valve_no_entrypoint,
   /****       "_jrd8_execute_immediate2"   ***/ "no_entrypoint",
			      IPI_execute_immediate2,
			   (ISC_STATUS*, struct rdb**, struct rtr**, USHORT, UCHAR*,
				USHORT, USHORT, UCHAR *, USHORT, USHORT, UCHAR *, USHORT,
				UCHAR*, USHORT, USHORT, UCHAR*))

ENTRYPOINT( "isc_dsql_fetch",
   /****         jrd8_fetch    ***/ y_valve_no_entrypoint,
			   "LDSQL_fetch",
			      REM_fetch,
   /****        "_REM_fetch"   ***/ "no_entrypoint",
			      CSI_fetch,
   /****          RDB_fetch    ***/ y_valve_no_entrypoint,
			      PSI_fetch,
				 PSI5_fetch,
   /****       "_jrd8_fetch"   ***/ "no_entrypoint",
			      IPI_fetch,
			   (ISC_STATUS*, struct rsr**, USHORT, UCHAR*, USHORT, USHORT, UCHAR*))

ENTRYPOINT( "isc_dsql_free_statement",
   /****         jrd8_free_statement    ***/ y_valve_no_entrypoint,
			   "LDSQL_free_statement",
			      REM_free_statement,
   /****        "_REM_free_statement"   ***/ "no_entrypoint",
			      CSI_free_statement,
   /****          RDB_free_statement    ***/ y_valve_no_entrypoint,
				  PSI_free_statement,
				 PSI5_free_statement,
   /****       "_jrd8_free_statement"   ***/ "no_entrypoint",
				 IPI_free_statement,
			   (ISC_STATUS *, struct rsr **, USHORT))

ENTRYPOINT( "isc_dsql_insert",
   /****         jrd8_insert    ***/ y_valve_no_entrypoint,
   /****        "jrd5_insert"   ***/ NULL,
			      REM_insert,
   /****        "_REM_insert"   ***/ "no_entrypoint",
			      CSI_insert,
   /****          RDB_insert    ***/ y_valve_no_entrypoint,
			      PSI_insert,
   /****         PSI5_insert    ***/ y_valve_no_entrypoint,
   /****       "_jrd8_insert"   ***/ "no_entrypoint",
			      IPI_insert,
			   (ISC_STATUS*, struct rsr**, USHORT, UCHAR*, USHORT, USHORT, UCHAR*))

ENTRYPOINT( "isc_dsql_prepare",
   /****         jrd8_prepare    ***/ y_valve_no_entrypoint,
			   "LDSQL_prepare",
			      REM_prepare,
   /****        "_REM_prepare"   ***/ "no_entrypoint",
                  CSI_prepare,
   /****          RDB_prepare    ***/ y_valve_no_entrypoint,
			      PSI_prepare,
			      PSI5_prepare,
   /****       "_jrd8_prepare"   ***/ "no_entrypoint",
			      IPI_prepare,
			   (ISC_STATUS *, struct rtr **, struct rsr **, USHORT, UCHAR *,
				USHORT, USHORT, UCHAR *, USHORT, UCHAR *))

ENTRYPOINT( "isc_dsql_set_cursor",
   /****         jrd8_set_cursor    ***/ y_valve_no_entrypoint,
			   "LDSQL_set_cursor",
			      REM_set_cursor_name,
   /****        "_REM_set_cursor"   ***/ "no_entrypoint",
			      CSI_set_cursor_name,
   /****          RDB_set_cursor    ***/ y_valve_no_entrypoint,
			      PSI_set_cursor_name,
			     PSI5_set_cursor_name,
   /****       "_jrd8_set_cursor"   ***/ "no_entrypoint",
			      IPI_set_cursor_name,
			   (ISC_STATUS *, struct rsr **, UCHAR *, USHORT))

ENTRYPOINT( "isc_dsql_sql_info",
   /****         jrd8_sql_info    ***/ y_valve_no_entrypoint,
			"LDSQL_sql_info",
			REM_sql_info,
   /****        "_REM_sql_info"   ***/ "no_entrypoint",
			CSI_sql_info,
   /****          RDB_sql_info    ***/ y_valve_no_entrypoint,
			PSI_sql_info,
			PSI5_sql_info,
   /****       "_jrd8_sql_info"   ***/ "no_entrypoint",
			IPI_sql_info,
			(ISC_STATUS*, struct rsr**, SSHORT, const UCHAR*,
				SSHORT, UCHAR*))

/* and service entry points */
ENTRYPOINT( "isc_service_attach",
		    jrd8_service_attach,
   /****   "jrd5_service_attach"   ***/ NULL,
			 REM_service_attach,
			"REM_service_attach",
   /****     CSI_service_attach    ***/ y_valve_no_entrypoint,
   /****     RDB_service_attach    ***/ y_valve_no_entrypoint,
			 PSI_attach_service,
   /****    PSI5_service_attach    ***/ y_valve_no_entrypoint,
		  "_jrd8_service_attach",
			 IPI_service_attach,
			   (ISC_STATUS *, USHORT, TEXT *, struct rdb **, USHORT, SCHAR *))

ENTRYPOINT("isc_service_detach", jrd8_service_detach,
   /****   "jrd5_service_detach"   ***/ NULL,
			   REM_service_detach, "REM_service_detach",
   /****     CSI_service_detach    ***/ y_valve_no_entrypoint,
   /****     RDB_service_detach    ***/ y_valve_no_entrypoint,
			   PSI_detach_service,
   /****    PSI5_service_detach    ***/ y_valve_no_entrypoint,
			   "_jrd8_service_detach",
			   IPI_service_detach, (ISC_STATUS *, struct rdb **))

ENTRYPOINT("isc_service_query", jrd8_service_query,
   /****   "jrd5_service_query"   ***/ NULL,
			REM_service_query, "REM_service_query",
   /****     CSI_service_query    ***/ y_valve_no_entrypoint,
   /****     RDB_service_query    ***/ y_valve_no_entrypoint,
			PSI_query_service,
   /****    PSI5_service_query    ***/ y_valve_no_entrypoint,
			"_jrd8_service_query",
			IPI_service_query,
			(ISC_STATUS*, struct rdb**, USHORT, const SCHAR*,
				USHORT, const SCHAR*, USHORT, SCHAR*))

ENTRYPOINT("isc_service_start", jrd8_service_start,
   /****   "jrd5_service_start"   ***/ NULL,
			   REM_service_start, "REM_service_start",
   /****     CSI_service_start    ***/ y_valve_no_entrypoint,
   /****     RDB_service_start    ***/ y_valve_no_entrypoint,
   /****     PSI_service_start    ***/ y_valve_no_entrypoint,
   /****    PSI5_service_start    ***/ y_valve_no_entrypoint,
			   "_jrd8_service_start",
			   IPI_service_start,
			   (ISC_STATUS *, struct svc **, ULONG *, USHORT, SCHAR *))

ENTRYPOINT("isc_rollback_retaining", jrd8_rollback_retaining,
   /****    "jrd5_rollback_retaining"   ***/ NULL,
			   REM_rollback_retaining, "_REM_rollback_retaining",
   /****    CSI_rollback_retaining    ***/ y_valve_no_entrypoint,
   /****    RDB_rollback_retaining    ***/ y_valve_no_entrypoint,
   /****    PSI_rollback_retaining    ***/ y_valve_no_entrypoint,
   /****    PSI5_rollback_retaining    ***/ y_valve_no_entrypoint,
			   "_jrd8_rollback_retaining",
			   IPI_rollback_retaining, (ISC_STATUS *, struct jrd_tra **))
#ifdef CANCEL_OPERATION
ENTRYPOINT("gds_cancel_operation", jrd8_cancel_operation,
   /***    "jrd5_cancel_operation"  ***/ NULL,
   /***     REM_cancel_operation    ***/ y_valve_no_entrypoint,
   /***    "REM_cancel_operation"   ***/ NULL,
   /***     CSI_cancel_operation    ***/ y_valve_no_entrypoint,
   /***     RDB_cancel_operation    ***/ y_valve_no_entrypoint,
   /***     PSI_cancel_operation    ***/ y_valve_no_entrypoint,
   /***     PSI5_cancel_operation   ***/ y_valve_no_entrypoint,
   /***    "_jrd8_cancel_operation" ***/ NULL,
   /***     IPI_cancel_operation    ***/ y_valve_no_entrypoint,
			   (ISC_STATUS *, struct att **, USHORT))
#else
ENTRYPOINT("gds_cancel_operation",
   /***	    jrd8_cancel_operation   ***/ y_valve_no_entrypoint,
   /***    "jrd5_cancel_operation"  ***/ NULL,
   /***     REM_cancel_operation    ***/ y_valve_no_entrypoint,
   /***    "REM_cancel_operation"   ***/ NULL,
   /***     CSI_cancel_operation    ***/ y_valve_no_entrypoint,
   /***     RDB_cancel_operation    ***/ y_valve_no_entrypoint,
   /***     PSI_cancel_operation    ***/ y_valve_no_entrypoint,
   /***     PSI5_cancel_operation   ***/ y_valve_no_entrypoint,
   /***    "_jrd8_cancel_operation" ***/ NULL,
   /***     IPI_cancel_operation    ***/ y_valve_no_entrypoint,
			   (ISC_STATUS *, struct att **, USHORT))
#endif
#undef ENTRYPOINT
#else // 0/1

ENTRYPOINT( "gds_attach_database",
            jrd8_attach_database,
           "jrd5_attach_database",
             REM_attach_database,
           "_REM_attach_database",
             CSI_attach_database,
             RDB_attach_database,
             PSI_attach_database,
            PSI5_attach_database,
          "_jrd8_attach_database",
             IPI_attach_database)

ENTRYPOINT( "gds_blob_info",
            jrd8_blob_info,
           "jrd5_blob_info",
             REM_blob_info,
           "_REM_blob_info",
             CSI_blob_info,
             RDB_blob_info,
             PSI_blob_info,
            PSI5_blob_info,
          "_jrd8_blob_info",
             IPI_blob_info)

ENTRYPOINT( "gds_cancel_blob",
            jrd8_cancel_blob,
           "jrd5_cancel_blob",
             REM_cancel_blob,
           "_REM_cancel_blob",
             CSI_cancel_blob,
             RDB_cancel_blob,
             PSI_cancel_blob,
            PSI5_cancel_blob,
          "_jrd8_cancel_blob",
             IPI_cancel_blob)

ENTRYPOINT( "gds_close_blob",
            jrd8_close_blob,
           "jrd5_close_blob",
             REM_close_blob,
           "_REM_close_blob",
             CSI_close_blob,
             RDB_close_blob,
             PSI_close_blob,
            PSI5_close_blob,
          "_jrd8_close_blob",
             IPI_close_blob)

ENTRYPOINT( "gds_commit_transaction",
            jrd8_commit_transaction,
           "jrd5_commit_transaction",
             REM_commit_transaction,
           "_REM_commit_transaction",
             CSI_commit_transaction,
             RDB_commit_transaction,
             PSI_commit_transaction,
            PSI5_commit_transaction,
          "_jrd8_commit_transaction",
             IPI_commit_transaction)

ENTRYPOINT( "gds_compile_request",
            jrd8_compile_request,
           "jrd5_compile_request",
             REM_compile_request,
           "_REM_compile_request",
             CSI_compile_request,
             RDB_compile_request,
             PSI_compile_request,
            PSI5_compile_request,
          "_jrd8_compile_request",
             IPI_compile_request)

ENTRYPOINT( "gds_create_blob",
   /****    jrd8_create_blob    ***/  no_entrypoint,
   /****   "jrd5_create_blob"   ***/  NULL,
   /****     REM_create_blob    ***/  no_entrypoint,
           "_REM_create_blob",
   /****     CSI_create_blob    ***/  no_entrypoint,
             RDB_create_blob,
             PSI_create_blob,
            PSI5_create_blob,
   /****  "_jrd8_create_blob"    ***/  no_entrypoint,
             IPI_create_blob)

ENTRYPOINT( "gds_create_database",
            jrd8_create_database,
   /****   "jrd5_create_database"   ***/  NULL,
             REM_create_database,
           "_REM_create_database",
             CSI_create_database,
             RDB_create_database,
             PSI_create_database,
   /****    PSI5_create_database    ***/  no_entrypoint,
          "_jrd8_create_database",
             IPI_create_database)
 
ENTRYPOINT( "gds_database_info",
            jrd8_database_info,
           "jrd5_database_info",
             REM_database_info,
           "_REM_database_info",
             CSI_database_info,
             RDB_database_info,
             PSI_database_info,
            PSI5_database_info,
          "_jrd8_database_info",
             IPI_database_info)

ENTRYPOINT( "gds_detach_database",
            jrd8_detach_database,
           "jrd5_detach_database",
             REM_detach_database,
           "_REM_detach_database",
             CSI_detach_database,
             RDB_detach_database,
             PSI_detach_database,
            PSI5_detach_database,
          "_jrd8_detach_database",
             IPI_detach_database)

ENTRYPOINT( "gds_get_segment",
            jrd8_get_segment,
           "jrd5_get_segment",
             REM_get_segment,
           "_REM_get_segment",
             CSI_get_segment,
             RDB_get_segment,
             PSI_get_segment,
            PSI5_get_segment,
          "_jrd8_get_segment",
             IPI_get_segment)

ENTRYPOINT( "gds_open_blob",
   /****    jrd8_open_blob    ***/  no_entrypoint,
   /****   "jrd5_open_blob"   ***/  NULL,
   /****     REM_open_blob    ***/  no_entrypoint,
           "_REM_open_blob",
   /****     CSI_open_blob    ***/  no_entrypoint,
             RDB_open_blob,
             PSI_open_blob,
            PSI5_open_blob,
   /****  "_jrd8_open_blob"   ***/  no_entrypoint,
             IPI_open_blob)

ENTRYPOINT( "gds_prepare_transaction",
            jrd8_prepare_transaction,
           "jrd5_prepare_transaction",
             REM_prepare_transaction,
           "_REM_prepare_transaction",
             CSI_prepare_transaction,
             RDB_prepare_transaction,
             PSI_prepare_transaction,
            PSI5_prepare_transaction,
          "_jrd8_prepare_transaction",
             IPI_prepare_transaction)

ENTRYPOINT( "gds_put_segment",
            jrd8_put_segment,
           "jrd5_put_segment",
             REM_put_segment,
           "_REM_put_segment",
             CSI_put_segment,
             RDB_put_segment,
             PSI_put_segment,
            PSI5_put_segment,
          "_jrd8_put_segment",
             IPI_put_segment)

ENTRYPOINT( "gds_reconnect_transaction",
            jrd8_reconnect_transaction,
           "jrd5_reconnect_transaction",
             REM_reconnect_transaction,
           "_REM_reconnect_transaction",
             CSI_reconnect_transaction,
             RDB_reconnect_transaction,
             PSI_reconnect_transaction,
            PSI5_reconnect_transaction,
          "_jrd8_reconnect_transaction",
             IPI_reconnect_transaction)

ENTRYPOINT( "gds_receive",
            jrd8_receive,
           "jrd5_receive",
             REM_receive,
           "_REM_receive",
             CSI_receive,
             RDB_receive,
             PSI_receive,    
            PSI5_receive,    /* the bridge pipe server will have two less params, but should be OK */
          "_jrd8_receive",
             IPI_receive)

ENTRYPOINT( "gds_release_request",
            jrd8_release_request,
           "jrd5_release_request",
             REM_release_request,
           "_REM_release_request",
             CSI_release_request,
             RDB_release_request,
             PSI_release_request,
            PSI5_release_request,
          "_jrd8_release_request",
             IPI_release_request)

ENTRYPOINT( "gds_request_info",
            jrd8_request_info,
           "jrd5_request_info",
             REM_request_info,
           "_REM_request_info",
             CSI_request_info,
             RDB_request_info,
             PSI_request_info,
            PSI5_request_info,
          "_jrd8_request_info",
             IPI_request_info)

ENTRYPOINT( "gds_rollback_transaction",
            jrd8_rollback_transaction,
           "jrd5_rollback_transaction",
             REM_rollback_transaction,
           "_REM_rollback_transaction",
             CSI_rollback_transaction,
             RDB_rollback_transaction,
             PSI_rollback_transaction,
            PSI5_rollback_transaction,
          "_jrd8_rollback_transaction",
             IPI_rollback_transaction)

ENTRYPOINT( "gds_send",
            jrd8_send,
           "jrd5_send",
             REM_send,
           "_REM_send",
             CSI_send,
             RDB_send,
             PSI_send,
            PSI5_send,
          "_jrd8_send",
             IPI_send)

ENTRYPOINT( "gds_start_and_send",
            jrd8_start_and_send,
           "jrd5_start_and_send",
             REM_start_and_send,
           "_REM_start_and_send",
             CSI_start_and_send,
             RDB_start_and_send,
             PSI_start_and_send,
            PSI5_start_and_send,
          "_jrd8_start_and_send",
             IPI_start_and_send)

ENTRYPOINT( "gds_start_request",
            jrd8_start_request,
           "jrd5_start_request",
             REM_start_request,
           "_REM_start_request",
             CSI_start_request,
             RDB_start_request,
             PSI_start_request,
            PSI5_start_request,
          "_jrd8_start_request",
             IPI_start_request)

ENTRYPOINT( "gds_start_multiple",
            jrd8_start_multiple,
           "jrd5_start_multiple",
   /****     REM_start_multiple    ***/  no_entrypoint,
           "_REM_start_multiple",
             CSI_start_multiple,
             RDB_start_multiple,
             PSI_start_multiple,
            PSI5_start_multiple,
          "_jrd8_start_multiple",
             IPI_start_multiple)

ENTRYPOINT( "gds_start_transaction",
            jrd8_start_transaction,
           "jrd5_start_transaction",
             REM_start_transaction,
           "_REM_start_transaction",
             CSI_start_transaction,
             RDB_start_transaction,
             PSI_start_transaction,
            PSI5_start_transaction,
          "_jrd8_start_transaction",
             IPI_start_transaction)

ENTRYPOINT( "gds_transaction_info",
            jrd8_transaction_info,
           "jrd5_transaction_info",
             REM_transaction_info,
           "_REM_transaction_info",
             CSI_transaction_info,
             RDB_transaction_info,
             PSI_transaction_info,
            PSI5_transaction_info,
          "_jrd8_transaction_info",
             IPI_transaction_info)

ENTRYPOINT( "gds_unwind_request",
            jrd8_unwind_request,
           "jrd5_unwind_request",
             REM_unwind_request,
           "_REM_unwind_request",
             CSI_unwind_request,
             RDB_unwind_request,
             PSI_unwind_request,
            PSI5_unwind_request,
          "_jrd8_unwind_request",
             IPI_unwind_request)

ENTRYPOINT( "gds_commit_retaining",
            jrd8_commit_retaining,
           "jrd5_commit_retaining",
             REM_commit_retaining,
           "_REM_commit_retaining",
             CSI_commit_retaining,
   /****     RDB_commit_retaining    ***/  no_entrypoint,
             PSI_commit_retaining,
            PSI5_commit_retaining,
          "_jrd8_commit_retaining",
             IPI_commit_retaining)

ENTRYPOINT( "gds_que_events",
            jrd8_que_events,
           "jrd5_que_events",
             REM_que_events,
           "_REM_que_events",
             CSI_que_events,
   /****     RDB_que_events    ***/  no_entrypoint,
             PSI_que_events,
            PSI5_que_events,
          "_jrd8_que_events",
             IPI_que_events)

ENTRYPOINT( "gds_cancel_events",
            jrd8_cancel_events,
           "jrd5_cancel_events",
             REM_cancel_events,
           "_REM_cancel_events",
             CSI_cancel_events,
   /****     RDB_cancel_events    ***/  no_entrypoint,
             PSI_cancel_events,
            PSI5_cancel_events,
          "_jrd8_cancel_events",
             IPI_cancel_events)

ENTRYPOINT( "gds_ddl",
            jrd8_ddl,
   /****   "jrd5_ddl"   ***/  NULL,
             REM_ddl,
           "_REM_ddl",
             CSI_ddl,
   /****     RDB_ddl    ***/  no_entrypoint,
             PSI_ddl,
            PSI5_ddl,
          "_jrd8_ddl",
             IPI_ddl)

ENTRYPOINT( "gds_open_blob2",
            jrd8_open_blob2,
           "jrd5_open_blob2",
             REM_open_blob2,
           "_REM_open_blob2",
             CSI_open_blob2,
   /****     RDB_open_blob2    ***/  no_entrypoint,
             PSI_open_blob2,
            PSI5_open_blob2,
          "_jrd8_open_blob2",
             IPI_open_blob2)

ENTRYPOINT( "gds_create_blob2",
            jrd8_create_blob2,
           "jrd5_create_blob2",
             REM_create_blob2,
           "_REM_create_blob2",
             CSI_create_blob2,
   /****     RDB_create_blob2    ***/  no_entrypoint,
             PSI_create_blob2,
            PSI5_create_blob2,
          "_jrd8_create_blob2",
             IPI_create_blob2)

ENTRYPOINT( "gds_get_slice",
            jrd8_get_slice,
           "jrd5_get_slice",
             REM_get_slice,
           "_REM_get_slice",
             CSI_get_slice,
   /****     RDB_get_slice    ***/  no_entrypoint,
             PSI_get_slice,
            PSI5_get_slice,
          "_jrd8_get_slice",
             IPI_get_slice)

ENTRYPOINT( "gds_put_slice",
            jrd8_put_slice,
           "jrd5_put_slice",
             REM_put_slice,
           "_REM_put_slice",
             CSI_put_slice,
   /****     RDB_put_slice    ***/  no_entrypoint,
             PSI_put_slice,
            PSI5_put_slice,
          "_jrd8_put_slice",
             IPI_put_slice)

ENTRYPOINT( "gds_seek_blob",
            jrd8_seek_blob,
           "jrd5_seek_blob",
             REM_seek_blob,
           "_REM_seek_blob",
             CSI_seek_blob,
   /****     RDB_seek_blob    ***/  no_entrypoint,
             PSI_seek_blob,
            PSI5_seek_blob,
          "_jrd8_seek_blob",
             IPI_seek_blob)

ENTRYPOINT( "isc_transact_request",
            jrd8_transact_request,
   /****   "jrd5_transact_request"   ***/  NULL,
             REM_transact_request,
           "_REM_transact_request",
             CSI_transact_request,
   /****     RDB_transact_request    ***/  no_entrypoint,
             PSI_transact_request,
   /****    PSI5_transact_request    ***/  no_entrypoint,
          "_jrd8_transact_request",
             IPI_transact_request)

ENTRYPOINT( "isc_drop_database",
            jrd8_drop_database,
   /****   "jrd5_drop_database"   ***/  NULL,
             REM_drop_database,
           "_REM_drop_database",
             CSI_drop_database,
   /****     RDB_drop_database    ***/  no_entrypoint,
             PSI_drop_database,
   /****    PSI5_drop_database    ***/  no_entrypoint,
          "_jrd8_drop_database",
             IPI_drop_database)

/* and the DSQL entry points */

ENTRYPOINT( "isc_dsql_allocate_statement",
   /****         jrd8_allocate_statement    ***/  no_entrypoint,
               "LDSQL_allocate_statement",
                  REM_allocate_statement,
   /****        "_REM_allocate_statement"   ***/  "no_entrypoint",
                  CSI_allocate_statement,
   /****          RDB_allocate_statement    ***/  no_entrypoint,
                  PSI_allocate_statement,
                 PSI5_allocate_statement,
   /****       "_jrd8_allocate_statement"   ***/  no_entrypoint,
                  IPI_allocate_statement)

ENTRYPOINT( "isc_dsql_execute",
   /****         jrd8_execute    ***/  no_entrypoint,
               "LDSQL_execute",
                  REM_execute,
   /****        "_REM_execute"   ***/  "no_entrypoint",
                  CSI_execute,
   /****          RDB_execute    ***/  no_entrypoint,
                  PSI_execute,
                 PSI5_execute,
   /****       "_jrd8_execute"   ***/  no_entrypoint,
                  IPI_execute)

ENTRYPOINT( "isc_dsql_execute2",
   /****         jrd8_execute2    ***/  no_entrypoint,
   /****        "jrd5_execute2"   ***/  NULL,
                  REM_execute2,
   /****        "_REM_execute2"   ***/  "no_entrypoint",
                  CSI_execute2,
   /****          RDB_execute2    ***/  no_entrypoint,
                  PSI_execute2,
   /****         PSI5_execute2    ***/  no_entrypoint,
   /****       "_jrd8_execute2"   ***/  no_entrypoint,
                  IPI_execute2)

ENTRYPOINT( "isc_dsql_execute_immediate",
   /****         jrd8_execute_immediate    ***/  no_entrypoint,
               "LDSQL_execute_immediate",
                  REM_execute_immediate,
   /****        "_REM_execute_immediate"   ***/  "no_entrypoint",
                  CSI_execute_immediate,
   /****          RDB_execute_immediate    ***/  no_entrypoint,
                  PSI_execute_immediate,
                 PSI5_execute_immediate,
   /****       "_jrd8_execute_immediate"   ***/  no_entrypoint,
                  IPI_execute_immediate)

ENTRYPOINT( "isc_dsql_execute_immediate2",
   /****         jrd8_execute_immediate2    ***/  no_entrypoint,
   /****        "jrd5_execute_immediate2"   ***/  NULL,
                  REM_execute_immediate2,
   /****        "_REM_execute_immediate2"   ***/  "no_entrypoint",
                  CSI_execute_immediate2,
   /****          RDB_execute_immediate2    ***/  no_entrypoint,
                  PSI_execute_immediate2,
   /****         PSI5_execute_immediate2    ***/  no_entrypoint,
   /****       "_jrd8_execute_immediate2"   ***/  no_entrypoint,
                  IPI_execute_immediate2)

ENTRYPOINT( "isc_dsql_fetch",
   /****         jrd8_fetch    ***/  no_entrypoint,
               "LDSQL_fetch",
                  REM_fetch,
   /****        "_REM_fetch"   ***/  "no_entrypoint",
                  CSI_fetch,
   /****          RDB_fetch    ***/  no_entrypoint,
                  PSI_fetch,
                 PSI5_fetch,
   /****       "_jrd8_fetch"   ***/  no_entrypoint,
                  IPI_fetch)

ENTRYPOINT( "isc_dsql_free_statement",
   /****         jrd8_free_statement    ***/  no_entrypoint,
               "LDSQL_free_statement",
                  REM_free_statement,
   /****        "_REM_free_statement"   ***/  "no_entrypoint",
                  CSI_free_statement,
   /****          RDB_free_statement    ***/  no_entrypoint,
                  PSI_free_statement,
                 PSI5_free_statement,
   /****       "_jrd8_free_statement"   ***/  no_entrypoint,
                  IPI_free_statement)

ENTRYPOINT( "isc_dsql_insert",
   /****         jrd8_insert    ***/  no_entrypoint,
   /****        "jrd5_insert"   ***/  NULL,
                  REM_insert,
   /****        "_REM_insert"   ***/  "no_entrypoint",
                  CSI_insert,
   /****          RDB_insert    ***/  no_entrypoint,
                  PSI_insert,
   /****         PSI5_insert    ***/  no_entrypoint,
   /****       "_jrd8_insert"   ***/  no_entrypoint,
                  IPI_insert)

ENTRYPOINT( "isc_dsql_prepare",
   /****         jrd8_prepare    ***/  no_entrypoint,
               "LDSQL_prepare",
                  REM_prepare,
   /****        "_REM_prepare"   ***/  "no_entrypoint",
                  CSI_prepare,
   /****          RDB_prepare    ***/  no_entrypoint,
                  PSI_prepare,
                 PSI5_prepare,
   /****       "_jrd8_prepare"   ***/  no_entrypoint,
                  IPI_prepare)

ENTRYPOINT( "isc_dsql_set_cursor",
   /****         jrd8_set_cursor    ***/  no_entrypoint,
               "LDSQL_set_cursor",
                  REM_set_cursor_name,
   /****        "_REM_set_cursor"   ***/  "no_entrypoint",
                  CSI_set_cursor_name,
   /****          RDB_set_cursor    ***/  no_entrypoint,
                  PSI_set_cursor_name,
                 PSI5_set_cursor_name,
   /****       "_jrd8_set_cursor"   ***/  no_entrypoint,
                  IPI_set_cursor_name)

ENTRYPOINT( "isc_dsql_sql_info",
   /****         jrd8_sql_info    ***/  no_entrypoint,
               "LDSQL_sql_info",
                  REM_sql_info,
   /****        "_REM_sql_info"   ***/  "no_entrypoint",
                  CSI_sql_info,
   /****          RDB_sql_info    ***/  no_entrypoint,
                  PSI_sql_info,
                 PSI5_sql_info,
   /****       "_jrd8_sql_info"   ***/  no_entrypoint,
                  IPI_sql_info)

/* and service entry points */

ENTRYPOINT( "isc_service_attach",
            jrd8_service_attach,
   /****   "jrd5_service_attach"   ***/  NULL,
             REM_service_attach,
            "REM_service_attach",
   /****     CSI_service_attach    ***/  no_entrypoint,
   /****     RDB_service_attach    ***/  no_entrypoint,
             PSI_attach_service,
   /****    PSI5_service_attach    ***/  no_entrypoint,
          "_jrd8_service_attach",
             IPI_service_attach)

ENTRYPOINT( "isc_service_detach",
            jrd8_service_detach,
   /****   "jrd5_service_detach"   ***/  NULL,
             REM_service_detach,
            "REM_service_detach",
   /****     CSI_service_detach    ***/  no_entrypoint,
   /****     RDB_service_detach    ***/  no_entrypoint,
             PSI_detach_service,
   /****    PSI5_service_detach    ***/  no_entrypoint,
          "_jrd8_service_detach",
             IPI_service_detach)

ENTRYPOINT( "isc_service_query",
            jrd8_service_query,
   /****   "jrd5_service_query"   ***/  NULL,
             REM_service_query,
            "REM_service_query",
   /****     CSI_service_query    ***/  no_entrypoint,
   /****     RDB_service_query    ***/  no_entrypoint,
             PSI_query_service,
   /****    PSI5_service_query    ***/  no_entrypoint,
          "_jrd8_service_query",
             IPI_service_query)

ENTRYPOINT( "isc_service_start",
            jrd8_service_start,
   /****   "jrd5_service_start"   ***/  NULL,
             REM_service_start,
            "REM_service_start",
   /****     CSI_service_start    ***/  no_entrypoint,
   /****     RDB_service_start    ***/  no_entrypoint,
   /****     PSI_service_start    ***/  no_entrypoint,
   /****    PSI5_service_start    ***/  no_entrypoint,
          "_jrd8_service_start",
             IPI_service_start)

ENTRYPOINT( "isc_rollback_retaining",
            jrd8_rollback_retaining,
   /****    "jrd5_rollback_retaining"   ***/  NULL,
             REM_rollback_retaining,
           "_REM_rollback_retaining",
   /****    CSI_rollback_retaining    ***/  no_entrypoint,
   /****    RDB_rollback_retaining    ***/  no_entrypoint,
   /****    PSI_rollback_retaining    ***/  no_entrypoint,
   /****    PSI5_rollback_retaining    ***/  no_entrypoint,
          "_jrd8_rollback_retaining",
             IPI_rollback_retaining)

#ifdef CANCEL_OPERATION
ENTRYPOINT( "gds_cancel_operation",
            jrd8_cancel_operation,
   /***    "jrd5_cancel_operation"  ***/  NULL,
   /***     REM_cancel_operation    ***/  no_entrypoint,
   /***    "REM_cancel_operation"   ***/  NULL,
   /***     CSI_cancel_operation    ***/  no_entrypoint,
   /***     RDB_cancel_operation    ***/  no_entrypoint,
   /***     PSI_cancel_operation    ***/  no_entrypoint,
   /***     PSI5_cancel_operation   ***/  no_entrypoint,
   /***    "_jrd8_cancel_operation" ***/  NULL,
   /***     IPI_cancel_operation    ***/  no_entrypoint)
#else
ENTRYPOINT( "gds_cancel_operation",
   /***	    jrd8_cancel_operation   ***/  no_entrypoint,
   /***    "jrd5_cancel_operation"  ***/  NULL,
   /***     REM_cancel_operation    ***/  no_entrypoint,
   /***    "REM_cancel_operation"   ***/  NULL,
   /***     CSI_cancel_operation    ***/  no_entrypoint,
   /***     RDB_cancel_operation    ***/  no_entrypoint,
   /***     PSI_cancel_operation    ***/  no_entrypoint,
   /***     PSI5_cancel_operation   ***/  no_entrypoint,
   /***    "_jrd8_cancel_operation" ***/  NULL,
   /***     IPI_cancel_operation    ***/  no_entrypoint)
#endif

#undef ENTRYPOINT
#endif // 0/1

