/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		rlck_proto.h
 *	DESCRIPTION:	Prototype header file for rlck.c
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

#ifdef PC_ENGINE
extern struct lck *RLCK_lock_record(struct rpb *, USHORT, int (*)(),
									struct blk *);
extern struct lck *RLCK_lock_record_implicit(struct jrd_tra *, struct rpb *,
											 USHORT, int (*)(), struct blk *);
extern struct lck *RLCK_lock_relation(struct jrd_rel *, USHORT, int (*)(),
									  struct blk *);
extern struct lck *RLCK_range_relation(struct jrd_tra *, struct jrd_rel *, int (*)(),
									   struct blk *);
extern struct lck *RLCK_record_locking(struct jrd_rel *);
extern void RLCK_release_lock(struct lck *);
extern void RLCK_release_locks(struct att *);
#endif
extern struct lck *RLCK_reserve_relation(struct tdbb *, struct jrd_tra *,
										 struct jrd_rel *, USHORT, USHORT);

/* TMN: This header did not match the implementation.
 * I moved the #ifdef as noted
 */
/* #ifdef PC_ENGINE */
extern void RLCK_shutdown_attachment(struct att *);
extern void RLCK_shutdown_database(struct dbb *);
#ifdef PC_ENGINE
extern void RLCK_signal_refresh(struct jrd_tra *);
#endif

extern struct lck *RLCK_transaction_relation_lock(struct jrd_tra *, struct jrd_rel *);

#ifdef PC_ENGINE
extern void RLCK_unlock_record(struct lck *, struct rpb *);
extern void RLCK_unlock_record_implicit(struct lck *, struct rpb *);
extern void RLCK_unlock_relation(struct lck *, struct jrd_rel *);
#endif

#endif /* JRD_RLCK_PROTO_H */
