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
extern STATUS API_ROUTINE isc_dsql_allocate_statement(STATUS *,
													  FRBRD **,
													  FRBRD **);
extern STATUS API_ROUTINE isc_dsql_alloc_statement2(STATUS *, FRBRD **,
													FRBRD **);
extern STATUS API_ROUTINE isc_dsql_describe(STATUS *, FRBRD **, USHORT,
											XSQLDA *);
extern STATUS API_ROUTINE isc_dsql_describe_bind(STATUS *, FRBRD **,
												 USHORT, XSQLDA *);
extern STATUS API_ROUTINE isc_dsql_execute2(STATUS *, FRBRD **,
											FRBRD **, USHORT, XSQLDA *,
											XSQLDA *);
extern STATUS API_ROUTINE isc_dsql_execute2_m(STATUS *, FRBRD **,
											  FRBRD **, USHORT, SCHAR *,
											  USHORT, USHORT, SCHAR *, USHORT,
											  SCHAR *, USHORT, USHORT,
											  SCHAR *);
extern STATUS API_ROUTINE isc_dsql_execute_immediate(STATUS *, FRBRD **,
													 FRBRD **, USHORT,
													 SCHAR *, USHORT,
													 XSQLDA *);
extern STATUS API_ROUTINE isc_dsql_exec_immed2(STATUS *, FRBRD **,
											   FRBRD **, USHORT,
											   SCHAR *, USHORT, XSQLDA *,
											   XSQLDA *);
extern STATUS API_ROUTINE isc_dsql_execute_immediate_m(STATUS *,
													   FRBRD **,
													   FRBRD **, USHORT,
													   SCHAR *, USHORT,
													   USHORT, SCHAR *,
													   USHORT, USHORT,
													   SCHAR *);
extern STATUS API_ROUTINE isc_dsql_exec_immed2_m(STATUS *, FRBRD **,
												 FRBRD **, USHORT,
												 SCHAR *, USHORT, USHORT,
												 SCHAR *, USHORT, USHORT,
												 SCHAR *, USHORT, SCHAR *,
												 USHORT, USHORT, SCHAR *);
extern STATUS API_ROUTINE isc_dsql_exec_immed3_m(STATUS *, FRBRD **,
												 FRBRD **, USHORT,
												 SCHAR *, USHORT, USHORT,
												 SCHAR *, USHORT, USHORT,
												 SCHAR *, USHORT, SCHAR *,
												 USHORT, USHORT, SCHAR *);
extern STATUS API_ROUTINE isc_dsql_fetch(STATUS *, FRBRD **, USHORT,
										 XSQLDA *);
#ifdef SCROLLABLE_CURSORS
extern STATUS API_ROUTINE isc_dsql_fetch2(STATUS *, FRBRD **, USHORT,
										  XSQLDA *, USHORT, SLONG);
#endif
extern STATUS API_ROUTINE isc_dsql_fetch_m(STATUS *, FRBRD **, USHORT,
										   SCHAR *, USHORT, USHORT, SCHAR *);
#ifdef SCROLLABLE_CURSORS
extern STATUS API_ROUTINE isc_dsql_fetch2_m(STATUS *, FRBRD **, USHORT,
											SCHAR *, USHORT, USHORT, SCHAR *,
											USHORT, SLONG);
#endif
extern STATUS API_ROUTINE isc_dsql_free_statement(STATUS *, FRBRD **,
												  USHORT);
extern STATUS API_ROUTINE isc_dsql_insert(STATUS *, FRBRD **, USHORT,
										  XSQLDA *);
extern STATUS API_ROUTINE isc_dsql_insert_m(STATUS *, FRBRD **, USHORT,
											SCHAR *, USHORT, USHORT, SCHAR *);
extern STATUS API_ROUTINE isc_dsql_prepare(STATUS *, FRBRD **,
										   FRBRD **, USHORT, SCHAR *,
										   USHORT, XSQLDA *);
extern STATUS API_ROUTINE isc_dsql_prepare_m(STATUS *, FRBRD **,
											 FRBRD **, USHORT, SCHAR *,
											 USHORT, USHORT, CONST SCHAR *, USHORT,
											 SCHAR *);
extern STATUS API_ROUTINE isc_dsql_set_cursor_name(STATUS *, FRBRD **,
												   SCHAR *, USHORT);
extern STATUS API_ROUTINE isc_dsql_sql_info(STATUS *, FRBRD **, SSHORT,
											CONST SCHAR *, SSHORT, SCHAR *);
extern STATUS API_ROUTINE isc_prepare_transaction2(STATUS *, FRBRD **, USHORT,
												   UCHAR *);
typedef void event_ast_routine(UCHAR*, USHORT, UCHAR*);
extern STATUS API_ROUTINE isc_que_events(STATUS *, FRBRD **, SLONG *,
										 USHORT, UCHAR *, event_ast_routine *, void *);
#ifdef SCROLLABLE_CURSORS
extern STATUS API_ROUTINE isc_receive2(STATUS *, FRBRD **, USHORT,
									   USHORT, SCHAR *, SSHORT, USHORT,
									   ULONG);
#endif
extern STATUS API_ROUTINE isc_rollback_transaction(STATUS *, FRBRD **);
extern STATUS API_ROUTINE_VARARG isc_start_transaction(STATUS *,
													   FRBRD **, SSHORT,
													   ...);
#endif

#ifdef CANCEL_OPERATION
extern STATUS API_ROUTINE gds__cancel_operation(STATUS *, FRBRD **,
												USHORT);
#endif
typedef void DatabaseCleanupRoutine(FRBRD **, SLONG);
extern STATUS API_ROUTINE gds__database_cleanup(STATUS *, FRBRD **,
												DatabaseCleanupRoutine *, SLONG);
extern int API_ROUTINE gds__disable_subsystem(TEXT *);
extern int API_ROUTINE gds__enable_subsystem(TEXT *);
extern STATUS gds__handle_cleanup(STATUS *, FRBRD **);
typedef void TransactionCleanupRoutine(FRBRD *, SLONG);
extern STATUS API_ROUTINE gds__transaction_cleanup(STATUS *, FRBRD **,
												   TransactionCleanupRoutine *, SLONG);

#ifdef SERVER_SHUTDOWN
extern BOOLEAN WHY_set_shutdown(BOOLEAN);
extern BOOLEAN WHY_get_shutdown();
#endif /* SERVER_SHUTDOWN */


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JRD_WHY_PROTO_H */
