/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		why_proto.h
 *	DESCRIPTION:	Prototype header file for why.c
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

#ifndef JRD_WHY_PROTO_H
#define JRD_WHY_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif


#ifndef JRD_IBASE_H
extern ISC_STATUS API_ROUTINE isc_dsql_allocate_statement(ISC_STATUS *,
													  FRBRD **,
													  FRBRD **);
extern ISC_STATUS API_ROUTINE isc_dsql_alloc_statement2(ISC_STATUS *, FRBRD **,
													FRBRD **);
extern ISC_STATUS API_ROUTINE isc_dsql_describe(ISC_STATUS *, FRBRD **, USHORT,
											XSQLDA *);
extern ISC_STATUS API_ROUTINE isc_dsql_describe_bind(ISC_STATUS *, FRBRD **,
												 USHORT, XSQLDA *);
extern ISC_STATUS API_ROUTINE isc_dsql_execute2(ISC_STATUS *, FRBRD **,
											FRBRD **, USHORT, XSQLDA *,
											XSQLDA *);
extern ISC_STATUS API_ROUTINE isc_dsql_execute2_m(ISC_STATUS *, FRBRD **,
											  FRBRD **, USHORT, SCHAR *,
											  USHORT, USHORT, SCHAR *, USHORT,
											  SCHAR *, USHORT, USHORT,
											  SCHAR *);
extern ISC_STATUS API_ROUTINE isc_dsql_execute_immediate(ISC_STATUS *, FRBRD **,
													 FRBRD **, USHORT,
													 SCHAR *, USHORT,
													 XSQLDA *);
extern ISC_STATUS API_ROUTINE isc_dsql_exec_immed2(ISC_STATUS *, FRBRD **,
											   FRBRD **, USHORT,
											   SCHAR *, USHORT, XSQLDA *,
											   XSQLDA *);
extern ISC_STATUS API_ROUTINE isc_dsql_execute_immediate_m(ISC_STATUS *,
													   FRBRD **,
													   FRBRD **, USHORT,
													   SCHAR *, USHORT,
													   USHORT, SCHAR *,
													   USHORT, USHORT,
													   SCHAR *);
extern ISC_STATUS API_ROUTINE isc_dsql_exec_immed2_m(ISC_STATUS *, FRBRD **,
												 FRBRD **, USHORT,
												 SCHAR *, USHORT, USHORT,
												 SCHAR *, USHORT, USHORT,
												 SCHAR *, USHORT, SCHAR *,
												 USHORT, USHORT, SCHAR *);
extern ISC_STATUS API_ROUTINE isc_dsql_exec_immed3_m(ISC_STATUS *, FRBRD **,
												 FRBRD **, USHORT,
												 SCHAR *, USHORT, USHORT,
												 SCHAR *, USHORT, USHORT,
												 SCHAR *, USHORT, SCHAR *,
												 USHORT, USHORT, SCHAR *);
extern ISC_STATUS API_ROUTINE isc_dsql_fetch(ISC_STATUS *, FRBRD **, USHORT,
										 XSQLDA *);
#ifdef SCROLLABLE_CURSORS
extern ISC_STATUS API_ROUTINE isc_dsql_fetch2(ISC_STATUS *, FRBRD **, USHORT,
										  XSQLDA *, USHORT, SLONG);
#endif
extern ISC_STATUS API_ROUTINE isc_dsql_fetch_m(ISC_STATUS *, FRBRD **, USHORT,
										   SCHAR *, USHORT, USHORT, SCHAR *);
#ifdef SCROLLABLE_CURSORS
extern ISC_STATUS API_ROUTINE isc_dsql_fetch2_m(ISC_STATUS *, FRBRD **, USHORT,
											SCHAR *, USHORT, USHORT, SCHAR *,
											USHORT, SLONG);
#endif
extern ISC_STATUS API_ROUTINE isc_dsql_free_statement(ISC_STATUS *, FRBRD **,
												  USHORT);
extern ISC_STATUS API_ROUTINE isc_dsql_insert(ISC_STATUS *, FRBRD **, USHORT,
										  XSQLDA *);
extern ISC_STATUS API_ROUTINE isc_dsql_insert_m(ISC_STATUS *, FRBRD **, USHORT,
											SCHAR *, USHORT, USHORT, SCHAR *);
extern ISC_STATUS API_ROUTINE isc_dsql_prepare(ISC_STATUS *, FRBRD **,
										   FRBRD **, USHORT, SCHAR *,
										   USHORT, XSQLDA *);
extern ISC_STATUS API_ROUTINE isc_dsql_prepare_m(ISC_STATUS *, FRBRD **,
											 FRBRD **, USHORT, SCHAR *,
											 USHORT, USHORT, const SCHAR *, USHORT,
											 SCHAR *);
extern ISC_STATUS API_ROUTINE isc_dsql_set_cursor_name(ISC_STATUS *, FRBRD **,
												   SCHAR *, USHORT);
extern ISC_STATUS API_ROUTINE isc_dsql_sql_info(ISC_STATUS *, FRBRD **, SSHORT,
											const SCHAR *, SSHORT, SCHAR *);
extern ISC_STATUS API_ROUTINE isc_prepare_transaction2(ISC_STATUS *, FRBRD **, USHORT,
												   UCHAR *);
typedef void event_ast_routine(UCHAR*, USHORT, UCHAR*);
extern ISC_STATUS API_ROUTINE isc_que_events(ISC_STATUS *, FRBRD **, SLONG *,
										 USHORT, UCHAR *, event_ast_routine *, void *);
#ifdef SCROLLABLE_CURSORS
extern ISC_STATUS API_ROUTINE isc_receive2(ISC_STATUS *, FRBRD **, USHORT,
									   USHORT, SCHAR *, SSHORT, USHORT,
									   ULONG);
#endif
extern ISC_STATUS API_ROUTINE isc_rollback_transaction(ISC_STATUS *, FRBRD **);
extern ISC_STATUS API_ROUTINE_VARARG isc_start_transaction(ISC_STATUS *,
													   FRBRD **, SSHORT,
													   ...);
#endif

#ifdef CANCEL_OPERATION
extern ISC_STATUS API_ROUTINE gds__cancel_operation(ISC_STATUS *, FRBRD **,
												USHORT);
#endif
typedef void DatabaseCleanupRoutine(FRBRD **, SLONG);
extern ISC_STATUS API_ROUTINE gds__database_cleanup(ISC_STATUS *, FRBRD **,
												DatabaseCleanupRoutine *, SLONG);
extern int API_ROUTINE gds__disable_subsystem(TEXT *);
extern int API_ROUTINE gds__enable_subsystem(TEXT *);
extern ISC_STATUS gds__handle_cleanup(ISC_STATUS *, FRBRD **);
typedef void TransactionCleanupRoutine(FRBRD *, SLONG);
extern ISC_STATUS API_ROUTINE gds__transaction_cleanup(ISC_STATUS *, FRBRD **,
												   TransactionCleanupRoutine *, SLONG);

#ifdef SERVER_SHUTDOWN
extern BOOLEAN WHY_set_shutdown(BOOLEAN);
extern BOOLEAN WHY_get_shutdown();
#endif /* SERVER_SHUTDOWN */


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JRD_WHY_PROTO_H */
