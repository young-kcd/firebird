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

#include "../common/classes/fb_string.h"

namespace Jrd {
	class Attachment;
	class jrd_tra;
	class blb;
	struct bid;
	class jrd_req;
	class Service;
	class BlockingThread;
	class thread_db;
	struct teb;
}

extern "C" {

ISC_STATUS jrd8_attach_database(ISC_STATUS*, SSHORT, const TEXT*,
											  Jrd::Attachment**, SSHORT,
											  const UCHAR*, const TEXT*);
ISC_STATUS jrd8_blob_info(ISC_STATUS*, Jrd::blb**, SSHORT,
										const SCHAR*, SSHORT, SCHAR*);
ISC_STATUS jrd8_cancel_blob(ISC_STATUS *, Jrd::blb **);
ISC_STATUS jrd8_cancel_events(ISC_STATUS *, Jrd::Attachment**, SLONG *);
#ifdef CANCEL_OPERATION
#define CANCEL_disable	1
#define CANCEL_enable	2
#define CANCEL_raise	3
ISC_STATUS jrd8_cancel_operation(ISC_STATUS *, Jrd::Attachment**,
											   USHORT);
#endif
ISC_STATUS jrd8_close_blob(ISC_STATUS *, Jrd::blb **);
ISC_STATUS jrd8_commit_transaction(ISC_STATUS *, Jrd::jrd_tra **);
ISC_STATUS jrd8_commit_retaining(ISC_STATUS *, Jrd::jrd_tra **);
ISC_STATUS jrd8_compile_request(ISC_STATUS*, Jrd::Attachment**,
											  Jrd::jrd_req**,
											  SSHORT, const SCHAR*);
ISC_STATUS jrd8_create_blob2(ISC_STATUS*, Jrd::Attachment**,
										   Jrd::jrd_tra**, Jrd::blb**,
										   Jrd::bid*, USHORT, const UCHAR*);
ISC_STATUS jrd8_create_database(ISC_STATUS*, USHORT, const TEXT*,
											  Jrd::Attachment**, USHORT,
											  const UCHAR*, USHORT,
											  const TEXT*);
ISC_STATUS jrd8_database_info(ISC_STATUS*, Jrd::Attachment**, SSHORT,
											const SCHAR*, SSHORT, SCHAR*);
ISC_STATUS jrd8_ddl(ISC_STATUS*, Jrd::Attachment**, Jrd::jrd_tra**,
								  USHORT, const SCHAR*);
ISC_STATUS jrd8_detach_database(ISC_STATUS *, Jrd::Attachment**);
ISC_STATUS jrd8_drop_database(ISC_STATUS *, Jrd::Attachment**);
ISC_STATUS jrd8_get_segment(ISC_STATUS *, Jrd::blb**, USHORT *,
										  USHORT, UCHAR *);
ISC_STATUS jrd8_get_slice(ISC_STATUS*, Jrd::Attachment**,
										Jrd::jrd_tra**, ISC_QUAD*, USHORT,
										const UCHAR*, USHORT, const UCHAR*, SLONG,
										UCHAR*, SLONG*);
ISC_STATUS jrd8_open_blob2(ISC_STATUS*, Jrd::Attachment**,
										 Jrd::jrd_tra**, Jrd::blb**,
										 Jrd::bid*, USHORT, const UCHAR*);
ISC_STATUS jrd8_prepare_transaction(ISC_STATUS *, Jrd::jrd_tra **,
												  USHORT, UCHAR *);
ISC_STATUS jrd8_put_segment(ISC_STATUS*, Jrd::blb**, USHORT,
										  const UCHAR*);
ISC_STATUS jrd8_put_slice(ISC_STATUS*, Jrd::Attachment**,
										Jrd::jrd_tra**, ISC_QUAD*, USHORT,
										const UCHAR*, USHORT, const UCHAR*, SLONG,
										UCHAR*);
ISC_STATUS jrd8_que_events(ISC_STATUS*, Jrd::Attachment**, SLONG*,
										 SSHORT, const UCHAR*,
										 FPTR_EVENT_CALLBACK, void*);
ISC_STATUS jrd8_receive(ISC_STATUS *, Jrd::jrd_req**, USHORT, USHORT,
									  SCHAR *, SSHORT);
ISC_STATUS jrd8_reconnect_transaction(ISC_STATUS*, Jrd::Attachment**,
													Jrd::jrd_tra**, SSHORT,
													const UCHAR*);
ISC_STATUS jrd8_release_request(ISC_STATUS *, Jrd::jrd_req**);
ISC_STATUS jrd8_request_info(ISC_STATUS*, Jrd::jrd_req**, SSHORT,
										   SSHORT, const SCHAR*, SSHORT, SCHAR*);
ISC_STATUS jrd8_rollback_transaction(ISC_STATUS *, Jrd::jrd_tra **);
ISC_STATUS jrd8_rollback_retaining(ISC_STATUS *, Jrd::jrd_tra **);
ISC_STATUS jrd8_seek_blob(ISC_STATUS *, Jrd::blb **, SSHORT,
										SLONG, SLONG *);
ISC_STATUS jrd8_send(ISC_STATUS *, Jrd::jrd_req**, USHORT, USHORT,
								   SCHAR *, SSHORT);
ISC_STATUS jrd8_service_attach(ISC_STATUS*, USHORT, const SCHAR*,
											 Jrd::Service**, USHORT, const SCHAR*);
ISC_STATUS jrd8_service_detach(ISC_STATUS *, Jrd::Service**);
ISC_STATUS jrd8_service_query(ISC_STATUS*, Jrd::Service**, ULONG*,
											USHORT, const SCHAR*,
											USHORT, const SCHAR*,
											USHORT, SCHAR*);
ISC_STATUS jrd8_service_start(ISC_STATUS*, Jrd::Service**, ULONG*,
											USHORT, const SCHAR*);
ISC_STATUS jrd8_start_and_send(ISC_STATUS *, Jrd::jrd_req**,
											 Jrd::jrd_tra **, USHORT, USHORT,
											 SCHAR *, SSHORT);
ISC_STATUS jrd8_start_request(ISC_STATUS *, Jrd::jrd_req**,
											Jrd::jrd_tra **, SSHORT);
ISC_STATUS jrd8_start_multiple(ISC_STATUS *, Jrd::jrd_tra **, USHORT,
											Jrd::teb *);
ISC_STATUS jrd8_start_transaction(ISC_STATUS *, Jrd::jrd_tra **,
												SSHORT, ...);
ISC_STATUS jrd8_transaction_info(ISC_STATUS*, Jrd::jrd_tra**,
											   SSHORT, const SCHAR*, SSHORT,
											   SCHAR*);
ISC_STATUS jrd8_transact_request(ISC_STATUS*, Jrd::Attachment**,
											   Jrd::jrd_tra**, USHORT, const SCHAR*,
											   USHORT, SCHAR*, USHORT,
											   SCHAR*);
ISC_STATUS jrd8_unwind_request(ISC_STATUS *, Jrd::jrd_req**, SSHORT);

} // extern "C"

void jrd_vtof(const char*, char*, SSHORT);

#ifdef SERVER_SHUTDOWN
/* Defines for paramater 3 of JRD_num_attachments */
const USHORT JRD_info_drivemask	= 1;
const USHORT JRD_info_dbnames	= 2;

TEXT*	JRD_num_attachments(TEXT* const, USHORT, USHORT, USHORT*, USHORT*);
ULONG	JRD_shutdown_all();
#endif /* SERVER_SHUTDOWN */

void	JRD_set_cache_default(ULONG *);
void	JRD_blocked(Jrd::Attachment*, Jrd::BlockingThread**);
void	JRD_mutex_lock(struct mutx_t *);
void	JRD_mutex_unlock(struct mutx_t *);
bool	JRD_reschedule(Jrd::thread_db*, SLONG, bool);
void	JRD_restore_context(void);
void	JRD_set_context(Jrd::thread_db*);
void	JRD_inuse_clear(Jrd::thread_db* tdbb);
void	JRD_unblock(Jrd::BlockingThread**);
void	JRD_wlck_lock(struct mutx_t *);
void	JRD_wlck_unlock(struct mutx_t *);
void	JRD_thread_security_disable(bool disable);
bool	JRD_get_thread_security_disabled();

// Call this function from the debugger if desired
void	JRD_print_pools(const char* filename);

#ifdef SUPERSERVER
void	JRD_print_all_counters(const TEXT*);
bool	JRD_getdir(Firebird::PathName&);
#endif

#ifdef DEBUG_PROCS
void	JRD_print_procedure_info(Jrd::thread_db*, const char*);
#endif

#ifdef WIN_NT
#include <direct.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

inline bool fb_getcwd(Firebird::PathName& pn)
{
	char buffer[MAXPATHLEN];
#if defined(WIN_NT)
	_getcwd(buffer, MAXPATHLEN);
#elif defined(HAVE_GETCWD)
	getcwd(buffer, MAXPATHLEN);
#else
	getwd(buffer);
#endif
	pn = buffer;
	return bool(buffer);
}


#endif /* JRD_JRD_PROTO_H */

