/*
 *	PROGRAM:	JRD Backup and Restore program  
 *	MODULE:		burp_proto.h
 *	DESCRIPTION:	Prototype header file for burp.c
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

#ifndef _BURP_BURP_PROTO_H_
#define _BURP_BURP_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef int (DLL_EXPORT* OUTPUTPROC) (SLONG, UCHAR *);

extern int  BURP_gbak (int, char **, OUTPUTPROC, SLONG);
extern void	BURP_abort (void);
extern void	BURP_svc_error (USHORT, USHORT, void *, USHORT, void *, USHORT, void *, USHORT, void *, USHORT, void *);
extern void	BURP_error (USHORT, const void*, const void*, const void*, const void*, const void*);
extern void	BURP_print_status (ISC_STATUS *);
extern void	BURP_error_redirect (ISC_STATUS *, USHORT, void*, void*);
extern void	BURP_msg_partial (USHORT, const void*, const void*, const void*, const void*, const void*);
extern void	BURP_msg_put (USHORT, const void*, const void*, const void*, const void*, const void*);
extern void	BURP_msg_get (USHORT, void*, void*, void*, void*, void*, void*);
extern void	BURP_output_version (TEXT *, TEXT *);
extern void	BURP_print (USHORT, const void*, const void*, const void*, const void*, const void*);
extern void	BURP_print_warning (ISC_STATUS*);
extern void	BURP_verbose (USHORT, const void*, const void*, const void*, const void*, const void*);

#ifdef __cplusplus
}
#endif

#endif	/*  _BURP_BURP_PROTO_H_  */

