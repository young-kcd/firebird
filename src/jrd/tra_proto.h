/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		tra_proto.h
 *	DESCRIPTION:	Prototype header file for tra.c
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

#ifndef _JRD_TRA_PROTO_H_
#define _JRD_TRA_PROTO_H_

#include "../jrd/req.h"

#ifdef __cplusplus
extern "C" {
#endif

extern BOOLEAN TRA_active_transactions(TDBB, struct dbb *);
extern void TRA_cleanup(TDBB);
extern void TRA_commit(TDBB, struct jrd_tra *, USHORT);
extern void TRA_extend_tip(TDBB, ULONG, struct win *);
extern int TRA_fetch_state(TDBB, SLONG);
extern void TRA_get_inventory(TDBB, UCHAR *, ULONG, ULONG);
extern int TRA_get_state(TDBB, SLONG);
#ifdef SUPERSERVER_V2
extern void TRA_header_write(TDBB, struct dbb *, SLONG);
#endif
extern void TRA_init(TDBB);
extern void TRA_invalidate(struct dbb *, ULONG);
extern void TRA_link_transaction(TDBB, struct jrd_tra *);
extern void TRA_post_resources(TDBB, struct jrd_tra *, class Rsc *);
extern BOOLEAN TRA_precommited(TDBB, SLONG, SLONG);
extern void TRA_prepare(TDBB, struct jrd_tra *, USHORT, UCHAR *);
extern struct jrd_tra *TRA_reconnect(TDBB, UCHAR *, USHORT);
extern void TRA_release_transaction(TDBB, struct jrd_tra *);
extern void TRA_rollback(TDBB, struct jrd_tra *, USHORT);
extern void TRA_set_state(TDBB, struct jrd_tra *, SLONG, SSHORT);
extern void TRA_shutdown_attachment(TDBB, struct att *);
extern int TRA_snapshot_state(TDBB, struct jrd_tra *, SLONG);
extern struct jrd_tra *TRA_start(TDBB, int, SCHAR *);
extern int TRA_state(UCHAR *, ULONG, ULONG);
extern int TRA_sweep(TDBB, struct jrd_tra *);
extern struct lck *TRA_transaction_lock(TDBB, struct blk *);
extern int TRA_wait(TDBB, struct jrd_tra *, SLONG, USHORT);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _JRD_TRA_PROTO_H_ */
