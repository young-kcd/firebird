/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		remot_proto.h
 *	DESCRIPTION:	Prototpe header file for remote.cpp
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

#ifndef REMOTE_REMOT_PROTO_H
#define REMOTE_REMOT_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif


void		REMOTE_cleanup_transaction (struct rtr *);
ULONG		REMOTE_compute_batch_size (struct port *, USHORT, P_OP, FMT);
void		REMOTE_get_timeout_params (struct port*, const UCHAR*, USHORT);
struct rrq*	REMOTE_find_request (struct rrq *, USHORT);
void		REMOTE_free_packet (struct port *, struct packet *);
struct str*	REMOTE_make_string (const SCHAR*);
void		REMOTE_release_messages (struct message *);
void		REMOTE_release_request (struct rrq *);
void		REMOTE_reset_request (struct rrq *, struct message *);
void		REMOTE_reset_statement (struct rsr *);
void		REMOTE_save_status_strings (ISC_STATUS *);
OBJCT		REMOTE_set_object (struct port *, struct blk *, OBJCT);

#ifdef __cplusplus
}	/* extern "C" */
#endif


#endif // REMOTE_REMOT_PROTO_H
