/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		fun_proto.h
 *	DESCRIPTION:	Prototype header file for fun.c
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

#ifndef _JRD_FUN_PROTO_H_
#define _JRD_FUN_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

extern void DLL_EXPORT FUN_evaluate(struct fun *, struct jrd_nod *, struct vlu *);
extern void DLL_EXPORT FUN_fini(struct tdbb *);
extern void DLL_EXPORT FUN_init(void);
extern struct fun *DLL_EXPORT FUN_lookup_function(TEXT *, bool ShowAccessError);
extern struct fun *DLL_EXPORT FUN_resolve(class Csb *, struct fun *,
										  struct jrd_nod *);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _JRD_FUN_PROTO_H_ */
