/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		ext_proto.h
 *	DESCRIPTION:	Prototype header file for ext.cpp & extvms.cpp
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

#ifndef JRD_EXT_PROTO_H
#define JRD_EXT_PROTO_H

struct bid;
class external_file;

void	EXT_close(Rsb*);
void	EXT_erase(rpb*, int*);
external_file*	EXT_file(jrd_rel*, const TEXT*, bid*);
void	EXT_fini(jrd_rel*);
int	EXT_get(Rsb*);
void	EXT_modify(rpb*, rpb*, int*);

#ifdef VMS
int	EXT_open(Rsb*);
#else
void	EXT_open(Rsb*);
#endif
Rsb*	EXT_optimize(Opt*, SSHORT, jrd_nod**);
void	EXT_ready(jrd_rel*);
void	EXT_store(rpb*, int*);
void	EXT_trans_commit(jrd_tra*);
void	EXT_trans_prepare(jrd_tra*);
void	EXT_trans_rollback(jrd_tra*);
void	EXT_trans_start(jrd_tra*);

#endif // JRD_EXT_PROTO_H

