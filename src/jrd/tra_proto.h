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

class att;
class jrd_tra;
class Resource;
class Database;

bool	TRA_active_transactions(TDBB, Database*);
void	TRA_cleanup(TDBB);
void	TRA_commit(TDBB, jrd_tra*, const bool);
void	TRA_extend_tip(TDBB, ULONG, struct win *);
int		TRA_fetch_state(TDBB, SLONG);
void	TRA_get_inventory(TDBB, UCHAR *, ULONG, ULONG);
int		TRA_get_state(TDBB, SLONG);

#ifdef SUPERSERVER_V2
void	TRA_header_write(TDBB, Database*, SLONG);
#endif
void	TRA_init(TDBB);
void	TRA_invalidate(Database*, ULONG);
void	TRA_link_transaction(TDBB, jrd_tra *);
void	TRA_post_resources(TDBB, jrd_tra *, Resource*);
bool	TRA_precommited(TDBB, SLONG, SLONG);
void	TRA_prepare(TDBB, jrd_tra*, USHORT, const UCHAR*);
jrd_tra*	TRA_reconnect(TDBB, const UCHAR*, USHORT);
void	TRA_release_transaction(TDBB, jrd_tra *);
void	TRA_rollback(TDBB, jrd_tra*, const bool);
void	TRA_set_state(TDBB, jrd_tra *, SLONG, SSHORT);
void	TRA_shutdown_attachment(TDBB, att *);
int		TRA_snapshot_state(TDBB, jrd_tra *, SLONG);
jrd_tra*	TRA_start(TDBB, int, const SCHAR*);
int		TRA_state(UCHAR *, ULONG, ULONG);
int		TRA_sweep(TDBB, jrd_tra *);
lck*	TRA_transaction_lock(TDBB, struct blk *);
int		TRA_wait(TDBB, jrd_tra *, SLONG, bool);

#endif // JRD_TRA_PROTO_H

