/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		svc_proto.h
 *	DESCRIPTION:	Prototype header file for svc.c
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

#ifndef JRD_SVC_PROTO_H
#define JRD_SVC_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

extern class svc *SVC_attach(USHORT, TEXT *, USHORT, SCHAR *);
extern void SVC_cleanup(class svc *);
extern void SVC_detach(class svc *);
extern void SVC_fprintf(class svc *, const SCHAR *, ...);
extern void SVC_putc(class svc*, UCHAR);
extern void SVC_query(class svc*, USHORT, SCHAR*, USHORT, SCHAR*, USHORT, SCHAR*);
extern STATUS SVC_query2(class svc*, struct tdbb*, USHORT, SCHAR *, USHORT, SCHAR*, USHORT, SCHAR*);
extern void *SVC_start(class svc*, USHORT, SCHAR*);
extern void SVC_finish(class svc*, USHORT);
extern void SVC_read_ib_log(class svc*);
extern TEXT *SVC_err_string(TEXT*, USHORT);

#ifdef SERVER_SHUTDOWN
#ifdef WIN_NT
#include <windows.h>
typedef void (__stdcall * STDCALL_FPTR_VOID) (UINT);
void SVC_shutdown_init(STDCALL_FPTR_VOID, ULONG);
#else
void SVC_shutdown_init(void (*)(), ULONG);
#endif
#endif /* SERVER_SHUTDOWN */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JRD_SVC_PROTO_H */
