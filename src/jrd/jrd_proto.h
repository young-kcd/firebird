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


extern ISC_STATUS DLL_EXPORT jrd8_attach_database(ISC_STATUS *, SSHORT, TEXT *,
											  struct att **, SSHORT, UCHAR *,
											  TEXT *);
extern ISC_STATUS DLL_EXPORT jrd8_blob_info(ISC_STATUS *, struct blb **, SSHORT,
										SCHAR *, SSHORT, SCHAR *);
extern ISC_STATUS DLL_EXPORT jrd8_cancel_blob(ISC_STATUS *, struct blb **);
extern ISC_STATUS DLL_EXPORT jrd8_cancel_events(ISC_STATUS *, struct att **, SLONG *);
#ifdef CANCEL_OPERATION
extern ISC_STATUS DLL_EXPORT jrd8_cancel_operation(ISC_STATUS *, struct att **,
											   USHORT);
#endif
extern ISC_STATUS DLL_EXPORT jrd8_close_blob(ISC_STATUS *, struct blb **);
extern ISC_STATUS DLL_EXPORT jrd8_commit_transaction(ISC_STATUS *, struct jrd_tra **);
extern ISC_STATUS DLL_EXPORT jrd8_commit_retaining(ISC_STATUS *, struct jrd_tra **);
extern ISC_STATUS DLL_EXPORT jrd8_compile_request(ISC_STATUS *, struct att **,
											  struct jrd_req **, SSHORT, SCHAR *);
extern ISC_STATUS DLL_EXPORT jrd8_create_blob2(ISC_STATUS *, struct att **,
										   struct jrd_tra **, struct blb **,
										   struct bid *, USHORT, UCHAR *);
extern ISC_STATUS DLL_EXPORT jrd8_create_database(ISC_STATUS *, USHORT, TEXT *,
											  struct att **, USHORT, UCHAR *,
											  USHORT, TEXT *);
extern ISC_STATUS DLL_EXPORT jrd8_database_info(ISC_STATUS *, struct att **, SSHORT,
											SCHAR *, SSHORT, SCHAR *);
extern ISC_STATUS DLL_EXPORT jrd8_ddl(ISC_STATUS *, struct att **, struct jrd_tra **,
								  USHORT, SCHAR *);
extern ISC_STATUS DLL_EXPORT jrd8_detach_database(ISC_STATUS *, struct att **);
extern ISC_STATUS DLL_EXPORT jrd8_drop_database(ISC_STATUS *, struct att **);
extern ISC_STATUS DLL_EXPORT jrd8_get_segment(ISC_STATUS *, struct blb **, USHORT *,
										  USHORT, UCHAR *);
extern ISC_STATUS DLL_EXPORT jrd8_get_slice(ISC_STATUS *, struct att **,
										struct jrd_tra **, SLONG *, USHORT,
										UCHAR *, USHORT, UCHAR *, SLONG,
										UCHAR *, SLONG *);
extern ISC_STATUS DLL_EXPORT jrd8_open_blob2(ISC_STATUS *, struct att **,
										 struct jrd_tra **, struct blb **,
										 struct bid *, USHORT, UCHAR *);
extern ISC_STATUS DLL_EXPORT jrd8_prepare_transaction(ISC_STATUS *, struct jrd_tra **,
												  USHORT, UCHAR *);
extern ISC_STATUS DLL_EXPORT jrd8_put_segment(ISC_STATUS *, struct blb **, USHORT,
										  UCHAR *);
extern ISC_STATUS DLL_EXPORT jrd8_put_slice(ISC_STATUS *, struct att **,
										struct jrd_tra **, SLONG *, USHORT,
										UCHAR *, USHORT, UCHAR *, SLONG,
										UCHAR *);
extern ISC_STATUS DLL_EXPORT jrd8_que_events(ISC_STATUS *, struct att **, SLONG *,
										 SSHORT, UCHAR *, FPTR_VOID, void *);
extern ISC_STATUS DLL_EXPORT jrd8_receive(ISC_STATUS *, struct jrd_req **, USHORT, USHORT,
									  SCHAR *, SSHORT);
extern ISC_STATUS DLL_EXPORT jrd8_reconnect_transaction(ISC_STATUS *, struct att **,
													struct jrd_tra **, SSHORT,
													UCHAR *);
extern ISC_STATUS DLL_EXPORT jrd8_release_request(ISC_STATUS *, struct jrd_req **);
extern ISC_STATUS DLL_EXPORT jrd8_request_info(ISC_STATUS *, struct jrd_req **, SSHORT,
										   SSHORT, SCHAR *, SSHORT, SCHAR *);
extern ISC_STATUS DLL_EXPORT jrd8_rollback_transaction(ISC_STATUS *, struct jrd_tra **);
extern ISC_STATUS DLL_EXPORT jrd8_rollback_retaining(ISC_STATUS *, struct jrd_tra **);
extern ISC_STATUS DLL_EXPORT jrd8_seek_blob(ISC_STATUS *, struct blb **, SSHORT,
										SLONG, SLONG *);
extern ISC_STATUS DLL_EXPORT jrd8_send(ISC_STATUS *, struct jrd_req **, USHORT, USHORT,
								   SCHAR *, SSHORT);
extern ISC_STATUS DLL_EXPORT jrd8_service_attach(ISC_STATUS *, USHORT, SCHAR *,
											 struct svc **, USHORT, SCHAR *);
extern ISC_STATUS DLL_EXPORT jrd8_service_detach(ISC_STATUS *, struct svc **);
extern ISC_STATUS DLL_EXPORT jrd8_service_query(ISC_STATUS *, struct svc **, ULONG *,
											USHORT, SCHAR *, USHORT, SCHAR *,
											USHORT, SCHAR *);
extern ISC_STATUS DLL_EXPORT jrd8_service_start(ISC_STATUS *, struct svc **, ULONG *,
											USHORT, SCHAR *);
extern ISC_STATUS DLL_EXPORT jrd8_start_and_send(ISC_STATUS *, struct jrd_req **,
											 struct jrd_tra **, USHORT, USHORT,
											 SCHAR *, SSHORT);
extern ISC_STATUS DLL_EXPORT jrd8_start_request(ISC_STATUS *, struct jrd_req **,
											struct jrd_tra **, SSHORT);
extern ISC_STATUS DLL_EXPORT jrd8_start_multiple(ISC_STATUS *, struct jrd_tra **, USHORT,
											 struct teb *);
extern ISC_STATUS DLL_EXPORT jrd8_start_transaction(ISC_STATUS *, struct jrd_tra **,
												SSHORT, ...);
extern ISC_STATUS DLL_EXPORT jrd8_transaction_info(ISC_STATUS *, struct jrd_tra **,
											   SSHORT, SCHAR *, SSHORT,
											   SCHAR *);
extern ISC_STATUS DLL_EXPORT jrd8_transact_request(ISC_STATUS *, struct att **,
											   struct jrd_tra **, USHORT, SCHAR *,
											   USHORT, SCHAR *, USHORT,
											   SCHAR *);
extern ISC_STATUS DLL_EXPORT jrd8_unwind_request(ISC_STATUS *, struct jrd_req **, SSHORT);
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
