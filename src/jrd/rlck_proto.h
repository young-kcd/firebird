/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		rlck_proto.h
 *	DESCRIPTION:	Prototype header file for rlck.cpp
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

#ifndef JRD_RLCK_PROTO_H
#define JRD_RLCK_PROTO_H

namespace Jrd {
	class Lock;
	class jrd_rel;
	class jrd_tra;
	struct record_param;
	class Attachment;
	class thread_db;
}
struct blk;

#ifdef PC_ENGINE

Jrd::Lock* RLCK_lock_record(Jrd::record_param*, USHORT, lock_ast_t, blk*);
									
Jrd::Lock* RLCK_lock_record_implicit(Jrd::jrd_tra*, Jrd::record_param*,
											 USHORT, lock_ast_t, blk*);
Jrd::Lock* RLCK_lock_relation(Jrd::jrd_rel*, USHORT, lock_ast_t, blk*);
Jrd::Lock* RLCK_range_relation(Jrd::jrd_tra*, Jrd::jrd_rel*, lock_ast_t, blk*);
Jrd::Lock* RLCK_record_locking(Jrd::jrd_rel*);
void RLCK_release_lock(Jrd::Lock*);
void RLCK_release_locks(Jrd::Attachment*);
#endif
Jrd::Lock* RLCK_reserve_relation(Jrd::thread_db*, Jrd::jrd_tra*,
										 Jrd::jrd_rel*, bool, bool);

/* TMN: This header did not match the implementation.
 * I moved the #ifdef as noted
 */
/* #ifdef PC_ENGINE */
void RLCK_shutdown_attachment(Jrd::Attachment*);
void RLCK_shutdown_database(Jrd::Database*);
#ifdef PC_ENGINE
void RLCK_signal_refresh(Jrd::jrd_tra*);
#endif

Jrd::Lock* RLCK_transaction_relation_lock(Jrd::jrd_tra*, Jrd::jrd_rel*);

#ifdef PC_ENGINE
void RLCK_unlock_record(Lock*, Jrd::record_param*);
void RLCK_unlock_record_implicit(Lock*, Jrd::record_param*);
void RLCK_unlock_relation(Lock*, Jrd::jrd_rel*);
#endif

#endif // JRD_RLCK_PROTO_H

