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

class svc* SVC_attach(USHORT, TEXT *, USHORT, SCHAR *);
void   SVC_cleanup(class svc *);
void   SVC_detach(class svc *);
void   SVC_fprintf(class svc *, const SCHAR *, ...);
void   SVC_putc(class svc*, UCHAR);
void   SVC_query(class svc*, USHORT, SCHAR*, USHORT, SCHAR*, USHORT, SCHAR*);
ISC_STATUS SVC_query2(class svc*, struct tdbb*, USHORT, SCHAR *, USHORT, SCHAR*, USHORT, SCHAR*);
void*  SVC_start(class svc*, USHORT, SCHAR*);
void   SVC_finish(class svc*, USHORT);
int   SVC_read_ib_log(class svc*);
const TEXT* SVC_err_string(const TEXT*, USHORT);
int SVC_output(SLONG, UCHAR *);

#ifdef SERVER_SHUTDOWN
typedef void (*shutdown_fct_t) (ULONG);
void SVC_shutdown_init(shutdown_fct_t, ULONG);
#endif /* SERVER_SHUTDOWN */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JRD_SVC_PROTO_H */
