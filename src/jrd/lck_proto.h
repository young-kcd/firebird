/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		lck_proto.h
 *	DESCRIPTION:	Prototype header file for lck.c
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

#ifndef JRD_LCK_PROTO_H
#define JRD_LCK_PROTO_H

#include "../jrd/lck.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void LCK_assert(TDBB, struct lck *);
extern int LCK_convert(TDBB, struct lck *, USHORT, SSHORT);
extern int LCK_convert_non_blocking(TDBB, struct lck *, USHORT, SSHORT);
extern int LCK_convert_opt(TDBB, struct lck *, USHORT);
extern int LCK_downgrade(TDBB, struct lck *);
extern void LCK_fini(TDBB, enum lck_owner_t);
extern SLONG LCK_get_owner_handle(TDBB, enum lck_t);
extern void LCK_init(TDBB, enum lck_owner_t);
extern int LCK_lock(TDBB, struct lck *, USHORT, SSHORT);
extern int LCK_lock_non_blocking(TDBB, struct lck *, USHORT, SSHORT);
extern int LCK_lock_opt(TDBB, struct lck *, USHORT, SSHORT);
extern SLONG LCK_query_data(struct lck *, enum lck_t, USHORT);
extern SLONG LCK_read_data(struct lck *);
extern void LCK_release(TDBB, struct lck *);
extern void LCK_re_post(struct lck *);
extern void LCK_write_data(struct lck *, SLONG);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JRD_LCK_PROTO_H */
