/*
 * PROGRAM: JRD Access Method
 * MODULE: ipapi_proto.h
 * DESCRIPTION: Prototype header file for ipclient.c api calls
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


#ifndef IPAPI_PROTO_H
#define IPAPI_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

extern STATUS DLL_EXPORT IPI_attach_database(STATUS *, SSHORT, SCHAR *,
											 struct idb **, SSHORT, SCHAR *);
extern STATUS DLL_EXPORT IPI_blob_info(STATUS *, struct ibl **, SSHORT,
									   UCHAR *, SSHORT, UCHAR *);
extern STATUS DLL_EXPORT IPI_cancel_blob(STATUS *, struct ibl **);
extern STATUS DLL_EXPORT IPI_cancel_events(STATUS *, struct idb **, SLONG *);
extern STATUS DLL_EXPORT IPI_close_blob(STATUS *, struct ibl **);
extern STATUS DLL_EXPORT IPI_commit_transaction(STATUS *, struct itr **);
extern STATUS DLL_EXPORT IPI_commit_retaining(STATUS *, struct itr **);
extern STATUS DLL_EXPORT IPI_compile_request(STATUS *, struct idb **,
											 struct irq **, SSHORT, UCHAR *);
extern STATUS DLL_EXPORT IPI_create_blob(STATUS *, struct idb **,
										 struct itr **, struct ibl **,
										 struct bid *);
extern STATUS DLL_EXPORT IPI_create_blob2(STATUS *, struct idb **,
										  struct itr **, struct ibl **,
										  struct bid *, SSHORT, UCHAR *);
extern STATUS DLL_EXPORT IPI_create_database(STATUS *, SSHORT, SCHAR *,
											 struct idb **, SSHORT, SCHAR *,
											 SSHORT);
extern STATUS DLL_EXPORT IPI_database_info(STATUS *, struct idb **, SSHORT,
										   UCHAR *, SSHORT, UCHAR *);
extern STATUS DLL_EXPORT IPI_ddl(STATUS *, struct idb **, struct itr **,
								 SSHORT, UCHAR *);
extern STATUS DLL_EXPORT IPI_detach_database(STATUS *, struct idb **);
extern STATUS DLL_EXPORT IPI_drop_database(STATUS *, struct idb **);
extern STATUS DLL_EXPORT IPI_get_segment(STATUS *, struct ibl **, USHORT *,
										 USHORT, UCHAR *);
extern STATUS DLL_EXPORT IPI_get_slice(STATUS *, struct idb **, struct itr **,
									   struct bid *, USHORT, UCHAR *, USHORT,
									   UCHAR *, SLONG, UCHAR *, SLONG *);
extern STATUS DLL_EXPORT IPI_open_blob(STATUS *, struct idb **, struct itr **,
									   struct ibl **, struct bid *);
extern STATUS DLL_EXPORT IPI_open_blob2(STATUS *, struct idb **,
										struct itr **, struct ibl **,
										struct bid *, SSHORT, UCHAR *);
extern STATUS DLL_EXPORT IPI_prepare_transaction(STATUS *, struct itr **,
												 USHORT, UCHAR *);
extern STATUS DLL_EXPORT IPI_put_segment(STATUS *, struct ibl **, USHORT,
										 UCHAR *);
extern STATUS DLL_EXPORT IPI_put_slice(STATUS *, struct idb **, struct itr **,
									   struct bid *, USHORT, UCHAR *, USHORT,
									   UCHAR *, SLONG, UCHAR *);
extern STATUS DLL_EXPORT IPI_que_events(STATUS *, struct idb **, SLONG *,
										USHORT, UCHAR *, FPTR_VOID, void *);
extern STATUS DLL_EXPORT IPI_receive(STATUS *, struct irq **, SSHORT, SSHORT,
									 UCHAR *, SSHORT
#ifdef SCROLLABLE_CURSORS
									 , USHORT, ULONG
#endif
	);
extern STATUS DLL_EXPORT IPI_reconnect_transaction(STATUS *, struct idb **,
												   struct itr **, SSHORT,
												   UCHAR *);
extern STATUS DLL_EXPORT IPI_release_request(STATUS *, struct irq **);
extern STATUS DLL_EXPORT IPI_request_info(STATUS *, struct irq **, USHORT,
										  SSHORT, UCHAR *, SSHORT, UCHAR *);
extern STATUS DLL_EXPORT IPI_rollback_transaction(STATUS *, struct itr **);
extern STATUS DLL_EXPORT IPI_rollback_retaining(STATUS *, struct itr **);
extern STATUS DLL_EXPORT IPI_seek_blob(STATUS *, struct ibl **, SSHORT, SLONG,
									   SLONG *);
extern STATUS DLL_EXPORT IPI_send(STATUS *, struct irq **, SSHORT, SSHORT,
								  UCHAR *, SSHORT);
extern STATUS DLL_EXPORT IPI_service_attach(STATUS *, USHORT, TEXT *,
											struct idb **, USHORT, SCHAR *);
extern STATUS DLL_EXPORT IPI_service_detach(STATUS *, struct idb **);
extern STATUS DLL_EXPORT IPI_service_query(STATUS *, struct idb **, ULONG *,
										   USHORT, SCHAR *, USHORT, SCHAR *,
										   USHORT, SCHAR *);
extern STATUS DLL_EXPORT IPI_service_start(STATUS *, struct idb **, ULONG *,
										   USHORT, SCHAR *);
extern STATUS DLL_EXPORT IPI_start_request(STATUS *, struct irq **,
										   struct itr **, SSHORT);
extern STATUS DLL_EXPORT IPI_start_and_send(STATUS *, struct irq **,
											struct itr **, SSHORT, SSHORT,
											UCHAR *, SSHORT);
extern STATUS DLL_EXPORT IPI_start_multiple(STATUS *, struct itr **, SSHORT,
											int **);
extern STATUS DLL_EXPORT IPI_start_transaction(STATUS *, struct itr **,
											   SSHORT, ...);
extern STATUS DLL_EXPORT IPI_transaction_info(STATUS *, struct itr **, SSHORT,
											  UCHAR *, SSHORT, UCHAR *);
extern STATUS DLL_EXPORT IPI_transact_request(STATUS *, struct idb **,
											  struct itr **, USHORT, UCHAR *,
											  USHORT, UCHAR *, USHORT,
											  UCHAR *);
extern STATUS DLL_EXPORT IPI_unwind_request(STATUS *, struct irq **, SSHORT);

/* DSQL entrypoints */

extern STATUS DLL_EXPORT IPI_allocate_statement(STATUS *, struct idb **,
												struct ipserver_isr **);
extern STATUS DLL_EXPORT IPI_execute(STATUS *, struct itr **,
									 struct ipserver_isr **, USHORT, UCHAR *,
									 USHORT, USHORT, UCHAR *);
extern STATUS DLL_EXPORT IPI_execute2(STATUS *, struct itr **,
									  struct ipserver_isr **, USHORT, UCHAR *,
									  USHORT, USHORT, UCHAR *, USHORT,
									  UCHAR *, USHORT, USHORT, UCHAR *);
extern STATUS DLL_EXPORT IPI_execute_immediate(STATUS *, struct idb **,
											   struct itr **, USHORT, UCHAR *,
											   USHORT, USHORT, UCHAR *,
											   USHORT, USHORT, UCHAR *);
extern STATUS DLL_EXPORT IPI_execute_immediate2(STATUS *, struct idb **,
												struct itr **, USHORT,
												UCHAR *, USHORT, USHORT,
												UCHAR *, USHORT, USHORT,
												UCHAR *, USHORT, UCHAR *,
												USHORT, USHORT, UCHAR *);
extern STATUS DLL_EXPORT IPI_fetch(STATUS *, struct ipserver_isr **, USHORT,
								   UCHAR *, USHORT, USHORT, UCHAR *);
extern STATUS DLL_EXPORT IPI_free_statement(STATUS *, struct ipserver_isr **,
											USHORT);
extern STATUS DLL_EXPORT IPI_insert(STATUS *, struct ipserver_isr **, USHORT,
									UCHAR *, USHORT, USHORT, UCHAR *);
extern STATUS DLL_EXPORT IPI_prepare(STATUS *, struct itr **,
									 struct ipserver_isr **, USHORT, UCHAR *,
									 USHORT, USHORT, UCHAR *, USHORT,
									 UCHAR *);
extern STATUS DLL_EXPORT IPI_set_cursor_name(STATUS *, struct ipserver_isr **,
											 UCHAR *, USHORT);
extern STATUS DLL_EXPORT IPI_sql_info(STATUS *, struct ipserver_isr **,
									  SSHORT, UCHAR *, SSHORT, UCHAR *);


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* IPAPI_PROTO_H */
