/*
 *	PROGRAM:	Pipe server Interface
 *	MODULE:		head_proto.h
 *	DESCRIPTION:	Prototype Header file for head.c
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

#ifndef _PIPE_HEAD_PROTO_H_
#define _PIPE_HEAD_PROTO_H_

#if (defined __cplusplus) && (defined SOLX86)
extern "C" {
#endif
/* Dublicated from head.cpp */

#ifndef BRIDGE
  STATUS PSI_attach_database(STATUS *, SSHORT, SCHAR *, struct rdb **,
								  SSHORT, SCHAR *);
  STATUS PSI_attach_service(STATUS *, USHORT, TEXT *, struct rdb **,
								 USHORT, SCHAR *);
  STATUS PSI_blob_info(STATUS *, struct rbl **, SSHORT, UCHAR *, SSHORT,
							UCHAR *);
  STATUS PSI_cancel_blob(STATUS *, struct rbl **);
  STATUS PSI_cancel_events(STATUS *, struct rdb **, SLONG *);
  STATUS PSI_close_blob(STATUS *, struct rbl **);
  STATUS PSI_commit_transaction(STATUS *, struct rtr **);
  STATUS PSI_commit_retaining(STATUS *, struct rtr **);
  STATUS PSI_compile_request(STATUS *, struct rdb **, struct rrq **,
								  SSHORT, UCHAR *);
  STATUS PSI_create_blob(STATUS *, struct rdb **, struct rtr **,
							  struct rbl **, struct bid *);
  STATUS PSI_create_blob2(STATUS *, struct rdb **, struct rtr **,
							   struct rbl **, struct bid *, SSHORT, UCHAR *);
  STATUS PSI_create_database(STATUS *, SSHORT, SCHAR *, struct rdb **,
								  SSHORT, SCHAR *, SSHORT);
  STATUS PSI_database_info(STATUS *, struct rdb **, SSHORT, UCHAR *,
								SSHORT, UCHAR *);
  STATUS PSI_ddl(STATUS *, struct rdb **, struct rtr **, SSHORT,
					  UCHAR *);
  STATUS PSI_detach_database(STATUS *, struct rdb **);
  STATUS PSI_detach_service(STATUS *, struct rdb **);
  STATUS PSI_drop_database(STATUS *, struct rdb **);
  STATUS PSI_allocate_statement(STATUS *, struct rdb **, struct rsr **);
  STATUS PSI_execute(STATUS *, struct rtr **, struct rsr **, USHORT,
						  UCHAR *, USHORT, USHORT, UCHAR *);
  STATUS PSI_execute2(STATUS *, struct rtr **, struct rsr **, USHORT,
						   UCHAR *, USHORT, USHORT, UCHAR *, USHORT, UCHAR *,
						   USHORT, USHORT, UCHAR *);
  STATUS PSI_execute_immediate(STATUS *, struct rdb **, struct rtr **,
									USHORT, UCHAR *, USHORT, USHORT, UCHAR *,
									USHORT, USHORT, UCHAR *);
  STATUS PSI_execute_immediate2(STATUS *, struct rdb **, struct rtr **,
									 USHORT, UCHAR *, USHORT, USHORT, UCHAR *,
									 USHORT, USHORT, UCHAR *, USHORT, UCHAR *,
									 USHORT, USHORT, UCHAR *);
  STATUS PSI_fetch(STATUS *, struct rsr **, USHORT, UCHAR *, USHORT,
						USHORT, UCHAR *);
  STATUS PSI_free_statement(STATUS *, struct rsr **, USHORT);
  STATUS PSI_get_segment(STATUS *, struct rbl **, USHORT *, USHORT,
							  UCHAR *);
  STATUS PSI_get_slice(STATUS *, struct rdb **, struct rtr **,
							struct bid *, USHORT, UCHAR *, USHORT, UCHAR *,
							SLONG, UCHAR *, SLONG *);
  STATUS PSI_insert(STATUS *, struct rsr **, USHORT, UCHAR *, USHORT,
						 USHORT, UCHAR *);
  STATUS PSI_open_blob(STATUS *, struct rdb **, struct rtr **,
							struct rbl **, struct bid *);
  STATUS PSI_open_blob2(STATUS *, struct rdb **, struct rtr **,
							 struct rbl **, struct bid *, SSHORT, UCHAR *);
  STATUS PSI_prepare(STATUS *, struct rtr **, struct rsr **, USHORT,
						  UCHAR *, USHORT, USHORT, UCHAR *, USHORT, UCHAR *);
  STATUS PSI_prepare_transaction(STATUS *, struct rtr **, USHORT,
									  UCHAR *);
  STATUS PSI_put_segment(STATUS *, struct rbl **, USHORT, UCHAR *);
  STATUS PSI_put_slice(STATUS *, struct rdb **, struct rtr **,
							struct bid *, USHORT, UCHAR *, USHORT, UCHAR *,
							SLONG, UCHAR *);
  STATUS PSI_que_events(STATUS *, struct rdb **, SLONG *, USHORT,
							 UCHAR * events, FPTR_VOID, void *);
  STATUS PSI_query_service(STATUS *, struct rdb **, USHORT, SCHAR *,
								USHORT, SCHAR *, USHORT, SCHAR *);
/* GCC compiler does not allow breaking of the statement */
#ifdef SCROLLABLE_CURSORS
  STATUS PSI_receive(STATUS *, struct rrq **, SSHORT, SSHORT, UCHAR *,
						  SSHORT, USHORT, ULONG);
#else
  STATUS PSI_receive(STATUS *, struct rrq **, SSHORT, SSHORT, UCHAR *,
						  SSHORT);
#endif
  STATUS PSI_reconnect_transaction(STATUS *, struct rdb **,
										struct rtr **, SSHORT, UCHAR *);
  STATUS PSI_release_request(STATUS *, struct rrq **);
  STATUS PSI_request_info(STATUS *, struct rrq **, USHORT, SSHORT,
							   UCHAR *, SSHORT, UCHAR *);
  STATUS PSI_rollback_transaction(STATUS *, struct rtr **);
  STATUS PSI_seek_blob(STATUS *, struct rbl **, SSHORT, SLONG, SLONG *);
  STATUS PSI_send(STATUS *, struct rrq **, SSHORT, SSHORT, UCHAR *,
					   SSHORT);
  STATUS PSI_set_cursor_name(STATUS *, struct rsr **, UCHAR *, USHORT);
  STATUS PSI_sql_info(STATUS *, struct rsr **, SSHORT, UCHAR *, SSHORT,
						   UCHAR *);
  STATUS PSI_start_and_send(STATUS *, struct rrq **, struct rtr **,
								 SSHORT, SSHORT, UCHAR *, SSHORT);
  STATUS PSI_start_multiple(STATUS *, struct rtr **, SSHORT, int **);
  STATUS PSI_start_request(STATUS *, register struct rrq **,
								register struct rtr **, SSHORT);
  STATUS PSI_start_transaction(STATUS *, struct rtr **, SSHORT, ...);
  STATUS PSI_transact_request(STATUS *, struct rdb **, struct rtr **,
								   USHORT, UCHAR *, USHORT, UCHAR *, USHORT,
								   UCHAR *);
  STATUS PSI_transaction_info(STATUS *, struct rtr **, SSHORT, UCHAR *,
								   SSHORT buffer_length, UCHAR *);
   STATUS PSI_unwind_request(STATUS *, struct rrq **, SSHORT);
#else
  STATUS PSI5_attach_database(STATUS *, SSHORT, SCHAR *, struct rdb **,
								  SSHORT, SCHAR *);
  STATUS PSI5_attach_service(STATUS *, USHORT, TEXT *, struct rdb **,
								 USHORT, SCHAR *);
  STATUS PSI5_blob_info(STATUS *, struct rbl **, SSHORT, UCHAR *, SSHORT,
							UCHAR *);
  STATUS PSI5_cancel_blob(STATUS *, struct rbl **);
  STATUS PSI5_cancel_events(STATUS *, struct rdb **, SLONG *);
  STATUS PSI5_close_blob(STATUS *, struct rbl **);
  STATUS PSI5_commit_transaction(STATUS *, struct rtr **);
  STATUS PSI5_commit_retaining(STATUS *, struct rtr **);
  STATUS PSI5_compile_request(STATUS *, struct rdb **, struct rrq **,
								  SSHORT, UCHAR *);
  STATUS PSI5_create_blob(STATUS *, struct rdb **, struct rtr **,
							  struct rbl **, struct bid *);
  STATUS PSI5_create_blob2(STATUS *, struct rdb **, struct rtr **,
							   struct rbl **, struct bid *, SSHORT, UCHAR *);
  STATUS PSI5_create_database(STATUS *, SSHORT, SCHAR *, struct rdb **,
								  SSHORT, SCHAR *, SSHORT);
  STATUS PSI5_database_info(STATUS *, struct rdb **, SSHORT, UCHAR *,
								SSHORT, UCHAR *);
  STATUS PSI5_ddl(STATUS *, struct rdb **, struct rtr **, SSHORT,
					  UCHAR *);
  STATUS PSI5_detach_database(STATUS *, struct rdb **);
  STATUS PSI5_detach_service(STATUS *, struct rdb **);
  STATUS PSI5_drop_database(STATUS *, struct rdb **);
  STATUS PSI5_allocate_statement(STATUS *, struct rdb **, struct rsr **);
  STATUS PSI5_execute(STATUS *, struct rtr **, struct rsr **, USHORT,
						  UCHAR *, USHORT, USHORT, UCHAR *);
  STATUS PSI5_execute2(STATUS *, struct rtr **, struct rsr **, USHORT,
						   UCHAR *, USHORT, USHORT, UCHAR *, USHORT, UCHAR *,
						   USHORT, USHORT, UCHAR *);
  STATUS PSI5_execute_immediate(STATUS *, struct rdb **, struct rtr **,
									USHORT, UCHAR *, USHORT, USHORT, UCHAR *,
									USHORT, USHORT, UCHAR *);
  STATUS PSI5_execute_immediate2(STATUS *, struct rdb **, struct rtr **,
									 USHORT, UCHAR *, USHORT, USHORT, UCHAR *,
									 USHORT, USHORT, UCHAR *, USHORT, UCHAR *,
									 USHORT, USHORT, UCHAR *);
  STATUS PSI5_fetch(STATUS *, struct rsr **, USHORT, UCHAR *, USHORT,
						USHORT, UCHAR *);
  STATUS PSI5_free_statement(STATUS *, struct rsr **, USHORT);
  STATUS PSI5_get_segment(STATUS *, struct rbl **, USHORT *, USHORT,
							  UCHAR *);
  STATUS PSI5_get_slice(STATUS *, struct rdb **, struct rtr **,
							struct bid *, USHORT, UCHAR *, USHORT, UCHAR *,
							SLONG, UCHAR *, SLONG *);
  STATUS PSI5_insert(STATUS *, struct rsr **, USHORT, UCHAR *, USHORT,
						 USHORT, UCHAR *);
  STATUS PSI5_open_blob(STATUS *, struct rdb **, struct rtr **,
							struct rbl **, struct bid *);
  STATUS PSI5_open_blob2(STATUS *, struct rdb **, struct rtr **,
							 struct rbl **, struct bid *, SSHORT, UCHAR *);
  STATUS PSI5_prepare(STATUS *, struct rtr **, struct rsr **, USHORT,
						  UCHAR *, USHORT, USHORT, UCHAR *, USHORT, UCHAR *);
  STATUS PSI5_prepare_transaction(STATUS *, struct rtr **, USHORT,
									  UCHAR *);
  STATUS PSI5_put_segment(STATUS *, struct rbl **, USHORT, UCHAR *);
  STATUS PSI5_put_slice(STATUS *, struct rdb **, struct rtr **,
							struct bid *, USHORT, UCHAR *, USHORT, UCHAR *,
							SLONG, UCHAR *);
  STATUS PSI5_que_events(STATUS *, struct rdb **, SLONG *, USHORT,
							 UCHAR * events, FPTR_VOID, void *);
  STATUS PSI5_query_service(STATUS *, struct rdb **, USHORT, SCHAR *,
								USHORT, SCHAR *, USHORT, SCHAR *);
/* GCC compiler does not allow breaking of the statement */
#ifdef SCROLLABLE_CURSORS
  STATUS PSI5_receive(STATUS *, struct rrq **, SSHORT, SSHORT, UCHAR *,
						  SSHORT, USHORT, ULONG);
#else
  STATUS PSI5_receive(STATUS *, struct rrq **, SSHORT, SSHORT, UCHAR *,
						  SSHORT);
#endif
  STATUS PSI5_reconnect_transaction(STATUS *, struct rdb **,
										struct rtr **, SSHORT, UCHAR *);
  STATUS PSI5_release_request(STATUS *, struct rrq **);
  STATUS PSI5_request_info(STATUS *, struct rrq **, USHORT, SSHORT,
							   UCHAR *, SSHORT, UCHAR *);
  STATUS PSI5_rollback_transaction(STATUS *, struct rtr **);
  STATUS PSI5_seek_blob(STATUS *, struct rbl **, SSHORT, SLONG, SLONG *);
  STATUS PSI5_send(STATUS *, struct rrq **, SSHORT, SSHORT, UCHAR *,
					   SSHORT);
  STATUS PSI5_set_cursor_name(STATUS *, struct rsr **, UCHAR *, USHORT);
  STATUS PSI5_sql_info(STATUS *, struct rsr **, SSHORT, UCHAR *, SSHORT,
						   UCHAR *);
  STATUS PSI5_start_and_send(STATUS *, struct rrq **, struct rtr **,
								 SSHORT, SSHORT, UCHAR *, SSHORT);
  STATUS PSI5_start_multiple(STATUS *, struct rtr **, SSHORT, int **);
  STATUS PSI5_start_request(STATUS *, register struct rrq **,
								register struct rtr **, SSHORT);
  STATUS PSI5_start_transaction(STATUS *, struct rtr **, SSHORT, ...);
  STATUS PSI5_transact_request(STATUS *, struct rdb **, struct rtr **,
								   USHORT, UCHAR *, USHORT, UCHAR *, USHORT,
								   UCHAR *);
  STATUS PSI5_transaction_info(STATUS *, struct rtr **, SSHORT, UCHAR *,
								   SSHORT buffer_length, UCHAR *);
   STATUS PSI5_unwind_request(STATUS *, struct rrq **, SSHORT);
#endif


#if (defined __cplusplus) && (defined SOLX86)
}
#endif
#endif /*  _PIPE_HEAD_PROTO_H_      */
