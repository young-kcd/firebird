/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		ld_proto.h
 *	DESCRIPTION:	Prototype header file for ld.c & ld2.c
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

#ifndef _INTL_LD_PROTO_H_
#define _INTL_LD_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEV_BUILD
void DLL_EXPORT LD_assert(const SCHAR*, int);
#endif

USHORT DLL_EXPORT LD_lookup(USHORT, FPTR_SHORT*, SSHORT, SSHORT);
USHORT DLL_EXPORT LD2_lookup(USHORT, FPTR_SHORT*, SSHORT, SSHORT);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _INTL_LD_PROTO_H_ */
