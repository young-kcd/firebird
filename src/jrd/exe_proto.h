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

extern void EXE_assignment(TDBB, struct nod*);
extern struct req *EXE_find_request(TDBB, struct req *, BOOLEAN);
extern void EXE_receive(TDBB, struct req*, USHORT, USHORT, UCHAR*);
extern void EXE_send(TDBB, struct req *, USHORT, USHORT, UCHAR *);
extern void EXE_start(TDBB, struct req *, struct tra *);
extern void EXE_unwind(TDBB, struct req *);
#ifdef SCROLLABLE_CURSORS
extern void EXE_seek(TDBB, struct req *, USHORT, ULONG);
#endif

#ifdef PC_ENGINE
extern BOOLEAN EXE_crack(TDBB, struct rsb *, USHORT);
extern void EXE_mark_crack(TDBB, struct rsb *, USHORT);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JRD_EXE_PROTO_H */
