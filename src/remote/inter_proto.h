/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		inter_proto.h
 *	DESCRIPTION:	Prototype Header file for interface.c
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

#ifndef REMOTE_INTER_PROTO_H
#define REMOTE_INTER_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

extern STATUS	REM_attach_database (STATUS *, SSHORT, SCHAR *, struct rdb **, SSHORT, SCHAR *, UCHAR *);
extern STATUS	REM_attach_service (STATUS *, USHORT, TEXT *, struct rdb **, USHORT, SCHAR *);
extern STATUS	REM_blob_info (STATUS *, struct rbl **, SSHORT, SCHAR *, SSHORT, SCHAR *);
extern STATUS	REM_cancel_blob (STATUS *, struct rbl **);
extern STATUS	REM_cancel_events (STATUS *, struct rdb **, SLONG *);
extern STATUS	REM_close_blob (STATUS *, struct rbl **);
extern STATUS	REM_commit_transaction (STATUS *, struct rtr **);
extern STATUS	REM_commit_retaining (STATUS *, struct rtr **);
extern STATUS	REM_compile_request (STATUS *, struct rdb **, struct rrq **, USHORT, UCHAR *);
extern STATUS	REM_create_blob2 (STATUS *, struct rdb **, struct rtr **, struct rbl **, BID, USHORT, UCHAR *);
extern STATUS	REM_create_database (STATUS *, SSHORT, SCHAR *, struct rdb **, SSHORT, SCHAR *, SSHORT, UCHAR *);
extern STATUS	REM_database_info (STATUS *, struct rdb **, SSHORT, SCHAR *, SSHORT, SCHAR *);
extern STATUS	REM_ddl (STATUS *, struct rdb **, struct rtr **, USHORT, UCHAR *);
extern STATUS	REM_detach_database (STATUS *, struct rdb **);
extern STATUS	REM_detach_service (STATUS *, struct rdb **);
extern STATUS	REM_drop_database (STATUS *, struct rdb **);
extern STATUS	REM_allocate_statement (STATUS *, struct rdb **, struct rsr **);
extern STATUS	REM_execute (STATUS *, struct rtr **, struct rsr **, USHORT, UCHAR *, USHORT, USHORT, UCHAR *);
extern STATUS	REM_execute2 (STATUS *, struct rtr **, struct rsr **, USHORT, UCHAR *, USHORT, USHORT, UCHAR *, USHORT, UCHAR *, USHORT, USHORT, UCHAR *);
extern STATUS	REM_execute_immediate (STATUS *, struct rdb **, struct rtr **, USHORT, TEXT *, USHORT, USHORT, UCHAR *, USHORT, USHORT, UCHAR *);
extern STATUS	REM_execute_immediate2 (STATUS *, struct rdb **, struct rtr **, USHORT, TEXT *, USHORT, USHORT, UCHAR *, USHORT, USHORT, UCHAR *, USHORT, UCHAR *, USHORT, USHORT, UCHAR *);
extern STATUS	REM_fetch (STATUS *, struct rsr **, USHORT, UCHAR *, USHORT, USHORT, UCHAR *);
extern STATUS	REM_free_statement (STATUS *, struct rsr **, USHORT);
extern STATUS	REM_insert (STATUS *, struct rsr **, USHORT, UCHAR *, USHORT, USHORT, UCHAR *);
extern STATUS	REM_prepare (STATUS *, struct rtr **, struct rsr **, USHORT, TEXT *, USHORT, USHORT, SCHAR *, USHORT, SCHAR *);
extern STATUS	REM_set_cursor_name (STATUS *, struct rsr **, TEXT *, USHORT);
extern STATUS	REM_sql_info (STATUS *, struct rsr **, SSHORT, SCHAR *, SSHORT, SCHAR *);
extern STATUS	REM_get_segment (STATUS *, struct rbl **, USHORT *, USHORT, UCHAR *);
extern STATUS	REM_get_slice (STATUS *, struct rdb **, struct rtr **, BID, USHORT, UCHAR *, USHORT, UCHAR *, SLONG, UCHAR *, SLONG *);
extern STATUS	REM_open_blob2 (STATUS *, struct rdb **, struct rtr **, struct rbl **, BID, USHORT, UCHAR *);
extern STATUS	REM_prepare_transaction (STATUS *, struct rtr **, USHORT, UCHAR *);
extern STATUS	REM_put_segment (STATUS *, struct rbl **, USHORT, UCHAR *);
extern STATUS	REM_put_slice (STATUS *, struct rdb **, struct rtr **, BID, USHORT, UCHAR *, USHORT, UCHAR *, SLONG, UCHAR *);
extern STATUS	REM_que_events (STATUS *, struct rdb **, SLONG *, SSHORT, UCHAR *, void (*)(void*, USHORT, UCHAR*), void *);
extern STATUS	REM_query_service (STATUS *, struct rdb **, USHORT, SCHAR *, USHORT, SCHAR *, USHORT, SCHAR *);
extern STATUS	REM_receive (STATUS *, struct rrq **, USHORT, USHORT, UCHAR *, SSHORT);
extern STATUS	REM_reconnect_transaction (STATUS *, struct rdb **, struct rtr **, USHORT, UCHAR *);
extern STATUS	REM_release_request (STATUS *, struct rrq **);
extern STATUS	REM_request_info (STATUS *, struct rrq **, SSHORT, SSHORT, UCHAR *, SSHORT, UCHAR *);
extern STATUS	REM_rollback_transaction (STATUS *, struct rtr **);
extern STATUS	REM_seek_blob (STATUS *, struct rbl **, SSHORT, SLONG, SLONG *);
extern STATUS	REM_send (STATUS *, struct rrq **, USHORT, USHORT, UCHAR *, SSHORT);
extern STATUS	REM_start_and_send (STATUS *, struct rrq **, struct rtr **, USHORT, USHORT, UCHAR *, SSHORT);
extern STATUS	REM_start_request (STATUS *, register struct rrq **, register struct rtr **, USHORT);
extern STATUS	REM_start_transaction (STATUS *, struct rtr **, SSHORT, struct rdb **, SSHORT, UCHAR *);
extern STATUS	REM_transact_request (STATUS *, struct rdb **, struct rtr **, USHORT, UCHAR *, USHORT, UCHAR *, USHORT, UCHAR *);
extern STATUS	REM_transaction_info (STATUS *, struct rtr **, SSHORT, UCHAR *, SSHORT, UCHAR *);
extern STATUS	REM_unwind_request (STATUS *, struct rrq **, USHORT);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* REMOTE_INTER_PROTO_H */
