/*
 * PROGRAM: JRD Access Method
 * MODULE: ipapi_proto.h
 * DESCRIPTION: Prototype header file for ipclient.cpp api calls
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

ISC_STATUS IPI_attach_database(ISC_STATUS*, SSHORT, const SCHAR*,
											 struct idb**, SSHORT, const SCHAR*);
ISC_STATUS IPI_blob_info(ISC_STATUS*, struct ibl**, SSHORT,
									   const UCHAR*, SSHORT, UCHAR*);
ISC_STATUS IPI_cancel_blob(ISC_STATUS *, struct ibl **);
ISC_STATUS IPI_cancel_events(ISC_STATUS *, struct idb **, SLONG *);
ISC_STATUS IPI_close_blob(ISC_STATUS *, struct ibl **);
ISC_STATUS IPI_commit_transaction(ISC_STATUS *, struct itr **);
ISC_STATUS IPI_commit_retaining(ISC_STATUS *, struct itr **);
ISC_STATUS IPI_compile_request(ISC_STATUS *, struct idb **,
											 struct irq **, SSHORT, UCHAR *);
ISC_STATUS IPI_create_blob(ISC_STATUS *, struct idb **,
										 struct itr **, struct ibl **,
										 struct bid *);
ISC_STATUS IPI_create_blob2(ISC_STATUS*, struct idb**,
										  struct itr**, struct ibl**,
										  struct bid*, SSHORT, const UCHAR*);
ISC_STATUS IPI_create_database(ISC_STATUS*, SSHORT, const SCHAR*,
											 struct idb**, SSHORT, SCHAR*,
											 SSHORT);
ISC_STATUS IPI_database_info(ISC_STATUS*, struct idb**, SSHORT,
										   const UCHAR*, SSHORT, UCHAR*);
ISC_STATUS IPI_ddl(ISC_STATUS *, struct idb **, struct itr **,
								 SSHORT, UCHAR *);
ISC_STATUS IPI_detach_database(ISC_STATUS *, struct idb **);
ISC_STATUS IPI_drop_database(ISC_STATUS *, struct idb **);
ISC_STATUS IPI_get_segment(ISC_STATUS *, struct ibl **, USHORT *,
										 USHORT, UCHAR *);
ISC_STATUS IPI_get_slice(ISC_STATUS *, struct idb **, struct itr **,
									   struct bid *, USHORT, UCHAR *, USHORT,
									   UCHAR *, SLONG, UCHAR *, SLONG *);
ISC_STATUS IPI_open_blob(ISC_STATUS*, struct idb**, struct itr**,
									   struct ibl**, struct bid*);
ISC_STATUS IPI_open_blob2(ISC_STATUS*, struct idb**,
										struct itr**, struct ibl**,
										struct bid*, SSHORT, const UCHAR*);
ISC_STATUS IPI_prepare_transaction(ISC_STATUS *, struct itr **,
												 USHORT, UCHAR *);
ISC_STATUS IPI_put_segment(ISC_STATUS*, struct ibl**, USHORT,
										 const UCHAR*);
ISC_STATUS IPI_put_slice(ISC_STATUS *, struct idb **, struct itr **,
									   struct bid *, USHORT, UCHAR *, USHORT,
									   UCHAR *, SLONG, UCHAR *);
ISC_STATUS IPI_que_events(ISC_STATUS *, struct idb **, SLONG *,
										USHORT, UCHAR *, FPTR_VOID, void *);
ISC_STATUS IPI_receive(ISC_STATUS *, struct irq **, SSHORT, SSHORT,
									 UCHAR *, SSHORT
#ifdef SCROLLABLE_CURSORS
									 , USHORT, ULONG
#endif
	);
ISC_STATUS IPI_reconnect_transaction(ISC_STATUS*, struct idb**,
												   struct itr**, SSHORT,
												   const UCHAR*);
ISC_STATUS IPI_release_request(ISC_STATUS *, struct irq **);
ISC_STATUS IPI_request_info(ISC_STATUS*, struct irq**, USHORT,
										  SSHORT, const UCHAR*, SSHORT, UCHAR*);
ISC_STATUS IPI_rollback_transaction(ISC_STATUS *, struct itr **);
ISC_STATUS IPI_rollback_retaining(ISC_STATUS *, struct itr **);
ISC_STATUS IPI_seek_blob(ISC_STATUS *, struct ibl **, SSHORT, SLONG,
									   SLONG *);
ISC_STATUS IPI_send(ISC_STATUS *, struct irq **, SSHORT, SSHORT,
								  UCHAR *, SSHORT);
ISC_STATUS IPI_service_attach(ISC_STATUS *, USHORT, TEXT *,
											struct idb **, USHORT, SCHAR *);
ISC_STATUS IPI_service_detach(ISC_STATUS *, struct idb **);
ISC_STATUS IPI_service_query(ISC_STATUS*, struct idb**, ULONG*,
										   USHORT, const SCHAR*,
										   USHORT, const SCHAR*,
										   USHORT, SCHAR*);
ISC_STATUS IPI_service_start(ISC_STATUS *, struct idb **, ULONG *,
										   USHORT, SCHAR *);
ISC_STATUS IPI_start_request(ISC_STATUS *, struct irq **,
										   struct itr **, SSHORT);
ISC_STATUS IPI_start_and_send(ISC_STATUS *, struct irq **,
											struct itr **, SSHORT, SSHORT,
											UCHAR *, SSHORT);
ISC_STATUS IPI_start_multiple(ISC_STATUS *, struct itr **, SSHORT,
											int **);
ISC_STATUS IPI_start_transaction(ISC_STATUS *, struct itr **,
											   SSHORT, ...);
ISC_STATUS IPI_transaction_info(ISC_STATUS*, struct itr**, SSHORT,
											  const UCHAR*, SSHORT, UCHAR*);
ISC_STATUS IPI_transact_request(ISC_STATUS *, struct idb **,
											  struct itr **, USHORT, UCHAR *,
											  USHORT, UCHAR *, USHORT,
											  UCHAR *);
ISC_STATUS IPI_unwind_request(ISC_STATUS *, struct irq **, SSHORT);

/* DSQL entrypoints */

ISC_STATUS IPI_allocate_statement(ISC_STATUS *, struct idb **,
												struct ipserver_isr **);
ISC_STATUS IPI_execute(ISC_STATUS *, struct itr **,
									 struct ipserver_isr **, USHORT, UCHAR *,
									 USHORT, USHORT, UCHAR *);
ISC_STATUS IPI_execute2(ISC_STATUS *, struct itr **,
									  struct ipserver_isr **, USHORT, UCHAR *,
									  USHORT, USHORT, UCHAR *, USHORT,
									  UCHAR *, USHORT, USHORT, UCHAR *);
ISC_STATUS IPI_execute_immediate(ISC_STATUS *, struct idb **,
											   struct itr **, USHORT, UCHAR *,
											   USHORT, USHORT, UCHAR *,
											   USHORT, USHORT, UCHAR *);
ISC_STATUS IPI_execute_immediate2(ISC_STATUS *, struct idb **,
												struct itr **, USHORT,
												UCHAR *, USHORT, USHORT,
												UCHAR *, USHORT, USHORT,
												UCHAR *, USHORT, UCHAR *,
												USHORT, USHORT, UCHAR *);
ISC_STATUS IPI_fetch(ISC_STATUS *, struct ipserver_isr **, USHORT,
								   UCHAR *, USHORT, USHORT, UCHAR *);
ISC_STATUS IPI_free_statement(ISC_STATUS *, struct ipserver_isr **,
											USHORT);
ISC_STATUS IPI_insert(ISC_STATUS *, struct ipserver_isr **, USHORT,
									UCHAR *, USHORT, USHORT, UCHAR *);
ISC_STATUS IPI_prepare(ISC_STATUS *, struct itr **,
									 struct ipserver_isr **, USHORT, UCHAR *,
									 USHORT, USHORT, UCHAR *, USHORT,
									 UCHAR *);
ISC_STATUS IPI_set_cursor_name(ISC_STATUS *, struct ipserver_isr **,
											 UCHAR *, USHORT);
ISC_STATUS IPI_sql_info(ISC_STATUS*, struct ipserver_isr**,
									  SSHORT, const UCHAR*, SSHORT, UCHAR*);


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif // IPAPI_PROTO_H

