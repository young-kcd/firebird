/*
 *	PROGRAM:	Central Server
 *	MODULE:		csi_proto.h
 *	DESCRIPTION:	Prototype Header file for csi.c
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

#ifndef _CSV_CSI_PROTO_H_
#define _CSV_CSI_PROTO_H_

extern STATUS CSI_attach_database(STATUS *, SSHORT, SCHAR *, struct rdb **,
								  SSHORT, SCHAR *, TEXT *);
extern STATUS CSI_blob_info(STATUS *, struct rbl **, SSHORT, UCHAR *, SSHORT,
							UCHAR *);
extern STATUS CSI_cancel_blob(STATUS *, struct rbl **);
extern STATUS CSI_cancel_events(STATUS *, struct rdb **, SLONG *);
extern STATUS CSI_close_blob(STATUS *, struct rbl **);
extern STATUS CSI_commit_transaction(STATUS *, struct rtr **);
extern STATUS CSI_commit_retaining(STATUS *, struct rtr **);
extern STATUS CSI_compile_request(STATUS *, struct rdb **, struct rrq **,
								  USHORT, UCHAR *);
extern STATUS CSI_create_blob2(STATUS *, struct rdb **, struct rtr **,
							   struct rbl **, BID, USHORT, UCHAR *);
extern STATUS CSI_create_database(STATUS *, SSHORT, SCHAR *, struct rdb **,
								  SSHORT, SCHAR *, SSHORT, TEXT *);
extern STATUS CSI_database_info(STATUS *, struct rdb **, SSHORT, UCHAR *,
								SSHORT, UCHAR *);
extern STATUS CSI_ddl(STATUS *, struct rdb **, struct rtr **, USHORT,
					  UCHAR *);
extern STATUS CSI_detach_database(STATUS *, struct rdb **);
extern STATUS CSI_drop_database(STATUS *, struct rdb **);
extern STATUS CSI_allocate_statement(STATUS *, struct rdb **, struct rsr **);
extern STATUS CSI_execute(STATUS *, struct rtr **, struct rsr **, USHORT,
						  UCHAR *, USHORT, USHORT, UCHAR *);
extern STATUS CSI_execute2(STATUS *, struct rtr **, struct rsr **, USHORT,
						   UCHAR *, USHORT, USHORT, UCHAR *, USHORT, UCHAR *,
						   USHORT, USHORT, UCHAR *);
extern STATUS CSI_execute_immediate(STATUS *, struct rdb **, struct rtr **,
									USHORT, TEXT *, USHORT, USHORT, UCHAR *,
									USHORT, USHORT, UCHAR *);
extern STATUS CSI_execute_immediate2(STATUS *, struct rdb **, struct rtr **,
									 USHORT, TEXT *, USHORT, USHORT, UCHAR *,
									 USHORT, USHORT, UCHAR *, USHORT, UCHAR *,
									 USHORT, USHORT, UCHAR *);
extern STATUS CSI_fetch(STATUS *, struct rsr **, USHORT, UCHAR *, USHORT,
						USHORT, UCHAR *);
extern STATUS CSI_free_statement(STATUS *, struct rsr **, USHORT);
extern STATUS CSI_insert(STATUS *, struct rsr **, USHORT, UCHAR *, USHORT,
						 USHORT, UCHAR *);
extern STATUS CSI_prepare(STATUS *, struct rtr **, struct rsr **, USHORT,
						  TEXT *, USHORT, USHORT, SCHAR *, USHORT, SCHAR *);
extern STATUS CSI_set_cursor_name(STATUS *, struct rsr **, TEXT *, USHORT);
extern STATUS CSI_sql_info(STATUS *, struct rsr **, SSHORT, SCHAR *, SSHORT,
						   SCHAR *);
extern STATUS CSI_get_segment(STATUS *, struct rbl **, USHORT *, USHORT,
							  UCHAR *);
extern STATUS CSI_get_slice(STATUS *, struct rdb **, struct rtr **, BID,
							USHORT, UCHAR *, USHORT, UCHAR *, SLONG, UCHAR *,
							SLONG *);
extern STATUS CSI_open_blob2(STATUS *, struct rdb **, struct rtr **,
							 struct rbl **, BID, USHORT, UCHAR *);
extern STATUS CSI_prepare_transaction(STATUS *, struct rtr **, USHORT,
									  UCHAR *);
extern STATUS CSI_put_segment(STATUS *, struct rbl **, USHORT, UCHAR *);
extern STATUS CSI_put_slice(STATUS *, struct rdb **, struct rtr **, BID,
							USHORT, UCHAR *, USHORT, UCHAR *, SLONG, UCHAR *);
extern STATUS CSI_que_events(STATUS *, struct rdb **, SLONG *, SSHORT,
							 UCHAR *, FPTR_VOID, void *);
extern STATUS CSI_receive(STATUS *, struct rrq **, USHORT, USHORT, UCHAR *,
						  SSHORT);
extern STATUS CSI_reconnect_transaction(STATUS *, struct rdb **,
										struct rtr **, SSHORT, UCHAR *);
extern STATUS CSI_release_request(STATUS *, struct rrq **);
extern STATUS CSI_request_info(STATUS *, struct rrq **, USHORT, SSHORT,
							   UCHAR *, SSHORT, UCHAR *);
extern STATUS CSI_rollback_transaction(STATUS *, struct rtr **);
extern STATUS CSI_seek_blob(STATUS *, struct rbl **, SSHORT, SLONG, SLONG *);
extern STATUS CSI_send(STATUS *, struct rrq **, USHORT, USHORT, UCHAR *,
					   SSHORT);
extern STATUS CSI_start_and_send(STATUS *, struct rrq **, struct rtr **,
								 USHORT, USHORT, UCHAR *, SSHORT);
extern STATUS CSI_start_request(STATUS *, register struct rrq **,
								register struct rtr **, SSHORT);
extern STATUS CSI_start_transaction(STATUS *, struct rtr **, SSHORT, ...);
extern STATUS CSI_transact_request(STATUS *, struct rdb **, struct rtr **,
								   USHORT, UCHAR *, USHORT, UCHAR *, USHORT,
								   UCHAR *);
extern STATUS CSI_transaction_info(STATUS *, struct rtr **, SSHORT, UCHAR *,
								   SSHORT, UCHAR *);
extern STATUS CSI_unwind_request(STATUS *, struct rrq **, SSHORT);

#endif /* _CSV_CSI_PROTO_H_ */
