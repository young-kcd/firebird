/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		jrd_proto.h
 *	DESCRIPTION:	Prototype header file for jrd.cpp
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


ISC_STATUS jrd8_attach_database(ISC_STATUS*, SSHORT, const TEXT*,
											  struct att**, SSHORT,
											  const UCHAR*, const TEXT*);
ISC_STATUS jrd8_blob_info(ISC_STATUS*, struct blb**, SSHORT,
										const SCHAR*, SSHORT, SCHAR*);
ISC_STATUS jrd8_cancel_blob(ISC_STATUS *, struct blb **);
ISC_STATUS jrd8_cancel_events(ISC_STATUS *, struct att **, SLONG *);
#ifdef CANCEL_OPERATION
ISC_STATUS jrd8_cancel_operation(ISC_STATUS *, struct att **,
											   USHORT);
#endif
ISC_STATUS jrd8_close_blob(ISC_STATUS *, struct blb **);
ISC_STATUS jrd8_commit_transaction(ISC_STATUS *, struct jrd_tra **);
ISC_STATUS jrd8_commit_retaining(ISC_STATUS *, struct jrd_tra **);
ISC_STATUS jrd8_compile_request(ISC_STATUS*, struct att**,
											  struct jrd_req**,
											  SSHORT, const SCHAR*);
ISC_STATUS jrd8_create_blob2(ISC_STATUS*, struct att**,
										   struct jrd_tra**, struct blb**,
										   struct bid*, USHORT, const UCHAR*);
ISC_STATUS jrd8_create_database(ISC_STATUS*, USHORT, const TEXT*,
											  struct att**, USHORT,
											  const UCHAR*, USHORT,
											  const TEXT*);
ISC_STATUS jrd8_database_info(ISC_STATUS*, struct att**, SSHORT,
											const SCHAR*, SSHORT, SCHAR*);
ISC_STATUS jrd8_ddl(ISC_STATUS*, struct att**, struct jrd_tra**,
								  USHORT, const SCHAR*);
ISC_STATUS jrd8_detach_database(ISC_STATUS *, struct att **);
ISC_STATUS jrd8_drop_database(ISC_STATUS *, struct att **);
ISC_STATUS jrd8_get_segment(ISC_STATUS *, struct blb **, USHORT *,
										  USHORT, UCHAR *);
ISC_STATUS jrd8_get_slice(ISC_STATUS *, struct att **,
										struct jrd_tra **, SLONG *, USHORT,
										UCHAR *, USHORT, UCHAR *, SLONG,
										UCHAR *, SLONG *);
ISC_STATUS jrd8_open_blob2(ISC_STATUS*, struct att**,
										 struct jrd_tra**, struct blb**,
										 struct bid*, USHORT, const UCHAR*);
ISC_STATUS jrd8_prepare_transaction(ISC_STATUS *, struct jrd_tra **,
												  USHORT, UCHAR *);
ISC_STATUS jrd8_put_segment(ISC_STATUS*, struct blb**, USHORT,
										  const UCHAR*);
ISC_STATUS jrd8_put_slice(ISC_STATUS *, struct att **,
										struct jrd_tra **, SLONG *, USHORT,
										UCHAR *, USHORT, UCHAR *, SLONG,
										UCHAR *);
ISC_STATUS jrd8_que_events(ISC_STATUS *, struct att **, SLONG *,
										 SSHORT, UCHAR *, FPTR_VOID, void *);
ISC_STATUS jrd8_receive(ISC_STATUS *, struct jrd_req **, USHORT, USHORT,
									  SCHAR *, SSHORT);
ISC_STATUS jrd8_reconnect_transaction(ISC_STATUS*, struct att**,
													struct jrd_tra**, SSHORT,
													const UCHAR*);
ISC_STATUS jrd8_release_request(ISC_STATUS *, struct jrd_req **);
ISC_STATUS jrd8_request_info(ISC_STATUS*, struct jrd_req**, SSHORT,
										   SSHORT, const SCHAR*, SSHORT, SCHAR*);
ISC_STATUS jrd8_rollback_transaction(ISC_STATUS *, struct jrd_tra **);
ISC_STATUS jrd8_rollback_retaining(ISC_STATUS *, struct jrd_tra **);
ISC_STATUS jrd8_seek_blob(ISC_STATUS *, struct blb **, SSHORT,
										SLONG, SLONG *);
ISC_STATUS jrd8_send(ISC_STATUS *, struct jrd_req **, USHORT, USHORT,
								   SCHAR *, SSHORT);
ISC_STATUS jrd8_service_attach(ISC_STATUS *, USHORT, SCHAR *,
											 struct svc **, USHORT, SCHAR *);
ISC_STATUS jrd8_service_detach(ISC_STATUS *, struct svc **);
ISC_STATUS jrd8_service_query(ISC_STATUS*, struct svc**, ULONG*,
											USHORT, const SCHAR*,
											USHORT, const SCHAR*,
											USHORT, SCHAR*);
ISC_STATUS jrd8_service_start(ISC_STATUS *, struct svc **, ULONG *,
											USHORT, SCHAR *);
ISC_STATUS jrd8_start_and_send(ISC_STATUS *, struct jrd_req **,
											 struct jrd_tra **, USHORT, USHORT,
											 SCHAR *, SSHORT);
ISC_STATUS jrd8_start_request(ISC_STATUS *, struct jrd_req **,
											struct jrd_tra **, SSHORT);
ISC_STATUS jrd8_start_multiple(ISC_STATUS *, struct jrd_tra **, USHORT,
											 struct teb *);
ISC_STATUS jrd8_start_transaction(ISC_STATUS *, struct jrd_tra **,
												SSHORT, ...);
ISC_STATUS jrd8_transaction_info(ISC_STATUS*, struct jrd_tra**,
											   SSHORT, const SCHAR*, SSHORT,
											   SCHAR*);
ISC_STATUS jrd8_transact_request(ISC_STATUS*, struct att**,
											   struct jrd_tra**, USHORT, SCHAR*,
											   USHORT, SCHAR*, USHORT,
											   SCHAR*);
ISC_STATUS jrd8_unwind_request(ISC_STATUS *, struct jrd_req **, SSHORT);
void jrd_vtof(const char*, char*, SSHORT);

#ifdef SERVER_SHUTDOWN
/* Defines for paramater 3 of JRD_num_attachments */
#define JRD_info_drivemask	1
#define JRD_info_dbnames	2

TEXT*	JRD_num_attachments(TEXT* const, USHORT, USHORT, USHORT*, USHORT*);
ULONG	JRD_shutdown_all();
#endif /* SERVER_SHUTDOWN */

void	JRD_set_cache_default(ULONG *);
void	JRD_blocked(struct att *, struct btb **);
void	JRD_mutex_lock(struct mutx_t *);
void	JRD_mutex_unlock(struct mutx_t *);
BOOLEAN	JRD_reschedule(struct tdbb *, SLONG, BOOLEAN);
void	JRD_restore_context(void);
void	JRD_set_context(struct tdbb *);
void	JRD_unblock(struct btb **);
void	JRD_wlck_lock(struct mutx_t *);
void	JRD_wlck_unlock(struct mutx_t *);

#ifdef SUPERSERVER
void	JRD_print_all_counters(const TEXT*);
USHORT	JRD_getdir(TEXT*, USHORT);
#endif

#ifdef DEBUG_PROCS
void	JRD_print_procedure_info(TDBB, const char*);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JRD_JRD_PROTO_H */

