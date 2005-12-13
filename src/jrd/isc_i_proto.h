/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		isc_i_proto.h
 *	DESCRIPTION:	Prototype header file for isc_ipc.c
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

#ifndef _JRD_ISC_I_PROTO_H_
#define _JRD_ISC_I_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

void DLL_EXPORT ISC_enter(void);
void DLL_EXPORT ISC_exit(void);

#ifdef __cplusplus
}
#endif

void DLL_EXPORT ISC_enable(void);
void DLL_EXPORT ISC_inhibit(void);

#ifdef WIN_NT
int API_ROUTINE ISC_kill(SLONG, SLONG, void *);
#else
int ISC_kill(SLONG, SLONG);
#endif

/* Signal routines have FPTR_VOID parameters instead of SIG_FPTR to
  hide OS signal implementation details for this module users.
  SIG_FPTR is very platform dependent. C/C++ ignores redundant function
  parameters anyway
*/

#if (defined __cplusplus) && (defined SOLX86)
/* Who else got mixed c and C++ linkage error - let join me. KLK
*/
extern "C" {
#endif

extern void API_ROUTINE ISC_signal(int, FPTR_VOID, void *);
extern void API_ROUTINE ISC_signal_cancel(int, FPTR_VOID, void *);
extern void DLL_EXPORT ISC_signal_init(void);

#if (defined __cplusplus) && (defined SOLX86)
/* Who else got mixed c and C++ linkage error - let join me. KLK
*/
}
#endif

#endif /* _JRD_ISC_I_PROTO_H_ */
