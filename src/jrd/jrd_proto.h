/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		jrd_proto.h
 *	DESCRIPTION:	Prototype header file for jrd.c
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


#ifndef JRD_JRD_PROTO_H
#define JRD_JRD_PROTO_H

#ifndef __cplusplus

/* included for the bid structure */
#ifndef WIN_NT
#include "../jrd/blb.h"
#endif

/* included for the svc structure */
#ifndef WIN_NT
#include "../jrd/svc.h"
#endif

#endif	/* __cplusplus */

#ifdef __cplusplus
extern "C" {
#endif


extern STATUS DLL_EXPORT jrd8_attach_database(STATUS *, SSHORT, TEXT *,
											  struct att **, SSHORT, UCHAR *,
											  TEXT *);
extern STATUS DLL_EXPORT jrd8_blob_info(STATUS *, struct blb **, SSHORT,
										SCHAR *, SSHORT, SCHAR *);
extern STATUS DLL_EXPORT jrd8_cancel_blob(STATUS *, struct blb **);
extern STATUS DLL_EXPORT jrd8_cancel_events(STATUS *, struct att **, SLONG *);
#ifdef CANCEL_OPERATION
extern STATUS DLL_EXPORT jrd8_cancel_operation(STATUS *, struct att **,
											   USHORT);
#endif
extern STATUS DLL_EXPORT jrd8_close_blob(STATUS *, struct blb **);
extern STATUS DLL_EXPORT jrd8_commit_transaction(STATUS *, struct jrd_tra **);
extern STATUS DLL_EXPORT jrd8_commit_retaining(STATUS *, struct jrd_tra **);
extern STATUS DLL_EXPORT jrd8_compile_request(STATUS *, struct att **,
											  struct jrd_req **, SSHORT, SCHAR *);
extern STATUS DLL_EXPORT jrd8_create_blob2(STATUS *, struct att **,
										   struct jrd_tra **, struct blb **,
										   struct bid *, USHORT, UCHAR *);
extern STATUS DLL_EXPORT jrd8_create_database(STATUS *, USHORT, TEXT *,
											  struct att **, USHORT, UCHAR *,
											  USHORT, TEXT *);
extern STATUS DLL_EXPORT jrd8_database_info(STATUS *, struct att **, SSHORT,
											SCHAR *, SSHORT, SCHAR *);
extern STATUS DLL_EXPORT jrd8_ddl(STATUS *, struct att **, struct jrd_tra **,
								  USHORT, SCHAR *);
extern STATUS DLL_EXPORT jrd8_detach_database(STATUS *, struct att **);
extern STATUS DLL_EXPORT jrd8_drop_database(STATUS *, struct att **);
extern STATUS DLL_EXPORT jrd8_get_segment(STATUS *, struct blb **, USHORT *,
										  USHORT, UCHAR *);
extern STATUS DLL_EXPORT jrd8_get_slice(STATUS *, struct att **,
										struct jrd_tra **, SLONG *, USHORT,
										UCHAR *, USHORT, UCHAR *, SLONG,
										UCHAR *, SLONG *);
extern STATUS DLL_EXPORT jrd8_open_blob2(STATUS *, struct att **,
										 struct jrd_tra **, struct blb **,
										 struct bid *, USHORT, UCHAR *);
extern STATUS DLL_EXPORT jrd8_prepare_transaction(STATUS *, struct jrd_tra **,
												  USHORT, UCHAR *);
extern STATUS DLL_EXPORT jrd8_put_segment(STATUS *, struct blb **, USHORT,
										  UCHAR *);
extern STATUS DLL_EXPORT jrd8_put_slice(STATUS *, struct att **,
										struct jrd_tra **, SLONG *, USHORT,
										UCHAR *, USHORT, UCHAR *, SLONG,
										UCHAR *);
extern STATUS DLL_EXPORT jrd8_que_events(STATUS *, struct att **, SLONG *,
										 SSHORT, UCHAR *, FPTR_VOID, void *);
extern STATUS DLL_EXPORT jrd8_receive(STATUS *, struct jrd_req **, USHORT, USHORT,
									  SCHAR *, SSHORT);
extern STATUS DLL_EXPORT jrd8_reconnect_transaction(STATUS *, struct att **,
													struct jrd_tra **, SSHORT,
													UCHAR *);
extern STATUS DLL_EXPORT jrd8_release_request(STATUS *, struct jrd_req **);
extern STATUS DLL_EXPORT jrd8_request_info(STATUS *, struct jrd_req **, SSHORT,
										   SSHORT, SCHAR *, SSHORT, SCHAR *);
extern STATUS DLL_EXPORT jrd8_rollback_transaction(STATUS *, struct jrd_tra **);
extern STATUS DLL_EXPORT jrd8_rollback_retaining(STATUS *, struct jrd_tra **);
extern STATUS DLL_EXPORT jrd8_seek_blob(STATUS *, struct blb **, SSHORT,
										SLONG, SLONG *);
extern STATUS DLL_EXPORT jrd8_send(STATUS *, struct jrd_req **, USHORT, USHORT,
								   SCHAR *, SSHORT);
extern STATUS DLL_EXPORT jrd8_service_attach(STATUS *, USHORT, SCHAR *,
											 struct svc **, USHORT, SCHAR *);
extern STATUS DLL_EXPORT jrd8_service_detach(STATUS *, struct svc **);
extern STATUS DLL_EXPORT jrd8_service_query(STATUS *, struct svc **, ULONG *,
											USHORT, SCHAR *, USHORT, SCHAR *,
											USHORT, SCHAR *);
extern STATUS DLL_EXPORT jrd8_service_start(STATUS *, struct svc **, ULONG *,
											USHORT, SCHAR *);
extern STATUS DLL_EXPORT jrd8_start_and_send(STATUS *, struct jrd_req **,
											 struct jrd_tra **, USHORT, USHORT,
											 SCHAR *, SSHORT);
extern STATUS DLL_EXPORT jrd8_start_request(STATUS *, struct jrd_req **,
											struct jrd_tra **, SSHORT);
extern STATUS DLL_EXPORT jrd8_start_multiple(STATUS *, struct jrd_tra **, USHORT,
											 struct teb *);
extern STATUS DLL_EXPORT jrd8_start_transaction(STATUS *, struct jrd_tra **,
												SSHORT, ...);
extern STATUS DLL_EXPORT jrd8_transaction_info(STATUS *, struct jrd_tra **,
											   SSHORT, SCHAR *, SSHORT,
											   SCHAR *);
extern STATUS DLL_EXPORT jrd8_transact_request(STATUS *, struct att **,
											   struct jrd_tra **, USHORT, SCHAR *,
											   USHORT, SCHAR *, USHORT,
											   SCHAR *);
extern STATUS DLL_EXPORT jrd8_unwind_request(STATUS *, struct jrd_req **, SSHORT);
extern void jrd_vtof(const char*, char*, SSHORT);

#ifdef SERVER_SHUTDOWN
/* Defines for paramater 3 of JRD_num_attachments */
#define JRD_info_drivemask	1
#define JRD_info_dbnames	2

extern TEXT *JRD_num_attachments(TEXT *, USHORT, USHORT, USHORT *, USHORT *);
extern ULONG JRD_shutdown_all();
#endif /* SERVER_SHUTDOWN */

extern void JRD_set_cache_default(ULONG *);
extern void JRD_blocked(struct att *, struct btb **);
extern void JRD_mutex_lock(struct mutx_t *);
extern void JRD_mutex_unlock(struct mutx_t *);
extern BOOLEAN JRD_reschedule(struct tdbb *, SLONG, BOOLEAN);
extern void JRD_restore_context(void);
extern void JRD_set_context(struct tdbb *);
extern void JRD_unblock(struct btb **);
extern void JRD_wlck_lock(struct mutx_t *);
extern void JRD_wlck_unlock(struct mutx_t *);

#ifdef SUPERSERVER
extern void JRD_print_all_counters(TEXT *);
extern USHORT JRD_getdir(TEXT *, USHORT);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JRD_JRD_PROTO_H */
