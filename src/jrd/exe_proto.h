/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		exe_proto.h
 *	DESCRIPTION:	Prototype header file for exe.c
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

#ifndef JRD_EXE_PROTO_H
#define JRD_EXE_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

void		EXE_assignment(TDBB, struct jrd_nod*);
struct jrd_req*	EXE_find_request(TDBB, struct jrd_req *, BOOLEAN);
void		EXE_receive(TDBB, struct jrd_req*, USHORT, USHORT, UCHAR*);
void		EXE_send(TDBB, struct jrd_req *, USHORT, USHORT, UCHAR *);
void		EXE_start(TDBB, struct jrd_req *, struct jrd_tra *);
void		EXE_unwind(TDBB, struct jrd_req *);
#ifdef SCROLLABLE_CURSORS
void		EXE_seek(TDBB, struct jrd_req *, USHORT, ULONG);
#endif

#ifdef PC_ENGINE
BOOLEAN EXE_crack(TDBB, struct rsb *, USHORT);
void		EXE_mark_crack(TDBB, struct rsb *, USHORT);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JRD_EXE_PROTO_H */
