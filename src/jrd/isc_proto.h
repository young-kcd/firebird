/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		isc_proto.h
 *	DESCRIPTION:	Prototype header file for isc.c
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

#ifndef _JRD_ISC_PROTO_H_
#define _JRD_ISC_PROTO_H_

#include "../jrd/isc.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void DLL_EXPORT ISC_ast_enter(void);
extern void DLL_EXPORT ISC_ast_exit(void);
extern int DLL_EXPORT ISC_check_process_existence(SLONG, SLONG, USHORT);
extern void DLL_EXPORT ISC_get_config(TEXT *, struct ipccfg *);
extern int DLL_EXPORT ISC_set_config(TEXT *, struct ipccfg *);
extern TEXT *INTERNAL_API_ROUTINE ISC_get_host(TEXT *, USHORT);
extern int INTERNAL_API_ROUTINE ISC_get_user(TEXT *, int *, int *, TEXT *,
											 TEXT *, int *, TEXT *);
extern SLONG ISC_get_user_group_id(TEXT *);
extern void ISC_set_user(TEXT *);
extern SLONG API_ROUTINE ISC_get_prefix(TEXT *);
extern void API_ROUTINE ISC_prefix(TEXT *, const TEXT *);
extern void API_ROUTINE ISC_prefix_lock(TEXT *, const TEXT *);
extern void API_ROUTINE ISC_prefix_msg(TEXT *, const TEXT *);

#ifdef VMS
extern int ISC_expand_logical_once(TEXT *, USHORT, TEXT *);
extern int ISC_make_desc(TEXT *, struct dsc$descriptor *, USHORT);
extern void ISC_wait(SSHORT *, SLONG);
extern void ISC_wake(SLONG);
extern void ISC_wake_init(void);
#endif

#ifdef WIN_NT
extern BOOLEAN ISC_is_WinNT(void);
extern struct _SECURITY_ATTRIBUTES *ISC_get_security_desc(void);
extern TEXT *ISC_prefix_interbase(TEXT *, TEXT *);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _JRD_ISC_PROTO_H_ */
