/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		all_proto.h
 *	DESCRIPTION:	Prototype header file for all.c
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

#ifndef JRD_ALL_PROTO_H
#define JRD_ALL_PROTO_H

#include "../jrd/jrd.h"
#include "../jrd/all.h"

#ifdef __cplusplus
extern "C" {
#endif

struct blk *ALL_alloc(struct plb *, UCHAR, ULONG, enum err_t);
#ifdef DEV_BUILD
void ALL_check_memory(void);
#endif
TEXT *ALL_cstring(TEXT *);
struct blk *ALL_extend(struct blk **, ULONG);
void ALL_fini(void);
void ALL_free(SCHAR *);
ULONG ALL_get_free_object(struct plb *, struct vec **, USHORT);
void ALL_init(void);
SCHAR *ALL_malloc(ULONG, enum err_t);
struct plb *ALL_pool(void);
void ALL_push(struct blk *, register struct lls **);
struct blk *ALL_pop(register struct lls **);
void ALL_release(register struct frb *);
void ALL_rlpool(struct plb *);
SCHAR *ALL_sys_alloc(ULONG, enum err_t);
void ALL_sys_free(SCHAR *);
ULONG ALL_tail(UCHAR);
struct vec *ALL_vector(struct plb *, struct vec **, USHORT);

#define ALL_RELEASE(block)	ALL_release ((struct frb *) block)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JRD_ALL_PROTO_H */
