/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		remot_proto.h
 *	DESCRIPTION:	Prototpe header file for remot.c
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

#ifndef _REMOTE_REMOT_PROTO_H_
#define _REMOTE_REMOT_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif


extern void DLL_EXPORT 	REMOTE_cleanup_transaction (struct rtr *);
extern ULONG		REMOTE_compute_batch_size (struct port *, USHORT, P_OP, FMT);
extern void DLL_EXPORT  REMOTE_get_timeout_params (struct port *, UCHAR *, USHORT);
extern struct rrq	* DLL_EXPORT REMOTE_find_request (struct rrq *, USHORT);
extern void DLL_EXPORT 	REMOTE_free_packet (struct port *, struct packet *);
extern struct str	* DLL_EXPORT REMOTE_make_string (SCHAR *);
extern void DLL_EXPORT	REMOTE_release_messages (struct message *);
extern void DLL_EXPORT 	REMOTE_release_request (struct rrq *);
extern void DLL_EXPORT	REMOTE_reset_request (struct rrq *, struct message *);
extern void DLL_EXPORT	REMOTE_reset_statement (struct rsr *);
extern void		REMOTE_save_status_strings (ISC_STATUS *);
extern OBJCT DLL_EXPORT REMOTE_set_object (struct port *, struct blk *, OBJCT);

#ifdef __cplusplus
}	/* extern "C" */
#endif


#endif /* _REMOTE_REMOT_PROTO_H */ 
