/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		ext_proto.h
 *	DESCRIPTION:	Prototype header file for ext.c & extvms.c
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

#ifndef _JRD_EXT_PROTO_H_
#define _JRD_EXT_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

extern void EXT_close(struct rsb *);
extern void EXT_erase(struct rpb *, int *);
extern struct ext *EXT_file(struct rel *, TEXT *, SLONG *);
extern void EXT_fini(struct rel *);
extern int EXT_get(struct rsb *);
extern void EXT_modify(struct rpb *, struct rpb *, int *);
#ifdef VMS
extern int EXT_open(struct rsb *);
#else
extern void EXT_open(struct rsb *);
#endif
extern struct rsb *EXT_optimize(register struct opt *, SSHORT, struct nod **);
extern void EXT_ready(struct rel *);
extern void EXT_store(struct rpb *, int *);
extern void EXT_trans_commit(struct tra *);
extern void EXT_trans_prepare(struct tra *);
extern void EXT_trans_rollback(struct tra *);
extern void EXT_trans_start(struct tra *);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _JRD_EXT_PROTO_H_ */
