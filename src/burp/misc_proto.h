/*
 *	PROGRAM:	JRD Backup and Restore Program
 *	MODULE:		misc_proto.h
 *	DESCRIPTION:	Prototype Header file for misc.c
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

#ifndef _BURP_MISC_PROTO_H_
#define _BURP_MISC_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

extern UCHAR	*MISC_alloc_burp (ULONG);
extern void	MISC_free_burp (void *);
extern void	MISC_terminate (UCHAR *, UCHAR *, ULONG, ULONG);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* _BURP_MISC_PROTO_H_ */
