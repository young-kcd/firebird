/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		tra_proto.h
 *	DESCRIPTION:	Prototype header file for tra.cpp
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

#ifndef JRD_TRA_PROTO_H
#define JRD_TRA_PROTO_H

#include "../jrd/req.h"

#ifdef __cplusplus
extern "C" {
#endif

BOOLEAN	TRA_active_transactions(TDBB, struct dbb *);
void	TRA_cleanup(TDBB);
void	TRA_commit(TDBB, struct jrd_tra*, const bool);
void	TRA_extend_tip(TDBB, ULONG, struct win *);
int		TRA_fetch_state(TDBB, SLONG);
void	TRA_get_inventory(TDBB, UCHAR *, ULONG, ULONG);
int		TRA_get_state(TDBB, SLONG);
#ifdef SUPERSERVER_V2
void	TRA_header_write(TDBB, struct dbb *, SLONG);
#endif
void	TRA_init(TDBB);
void	TRA_invalidate(struct dbb *, ULONG);
void	TRA_link_transaction(TDBB, struct jrd_tra *);
void	TRA_post_resources(TDBB, struct jrd_tra *, class Rsc *);
BOOLEAN TRA_precommited(TDBB, SLONG, SLONG);
void	TRA_prepare(TDBB, struct jrd_tra*, USHORT, const UCHAR*);
struct jrd_tra*	TRA_reconnect(TDBB, const UCHAR*, USHORT);
void	TRA_release_transaction(TDBB, struct jrd_tra *);
void	TRA_rollback(TDBB, struct jrd_tra*, const bool);
void	TRA_set_state(TDBB, struct jrd_tra *, SLONG, SSHORT);
void	TRA_shutdown_attachment(TDBB, struct att *);
int		TRA_snapshot_state(TDBB, struct jrd_tra *, SLONG);
struct jrd_tra*	TRA_start(TDBB, int, const SCHAR*);
int		TRA_state(UCHAR *, ULONG, ULONG);
int		TRA_sweep(TDBB, struct jrd_tra *);
struct lck*	TRA_transaction_lock(TDBB, struct blk *);
int		TRA_wait(TDBB, struct jrd_tra *, SLONG, USHORT);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // JRD_TRA_PROTO_H

