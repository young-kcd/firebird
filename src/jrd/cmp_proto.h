/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		cmp_proto.h
 *	DESCRIPTION:	Prototype header file for cmp.c
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

#ifndef _JRD_CMP_PROTO_H_
#define _JRD_CMP_PROTO_H_

#include "../jrd/req.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int DLL_EXPORT CMP_clone_active(struct req *);
extern struct nod *DLL_EXPORT CMP_clone_node(TDBB, struct csb *,
											 struct nod *);
extern struct req *DLL_EXPORT CMP_clone_request(TDBB, struct req *, USHORT,
												BOOLEAN);
extern struct req *DLL_EXPORT CMP_compile(USHORT, UCHAR *, USHORT);
extern struct req *DLL_EXPORT CMP_compile2(TDBB, UCHAR *, USHORT);
extern struct csb_repeat *DLL_EXPORT CMP_csb_element(struct csb **, USHORT);
extern void DLL_EXPORT CMP_expunge_transaction(struct tra *);
extern void DLL_EXPORT CMP_decrement_prc_use_count(TDBB, PRC);
extern struct req *DLL_EXPORT CMP_find_request(TDBB, USHORT, USHORT);
extern void DLL_EXPORT CMP_fini(TDBB);
extern struct fmt *DLL_EXPORT CMP_format(TDBB, struct csb *, USHORT);
extern void DLL_EXPORT CMP_get_desc(TDBB, register struct csb *,
									register struct nod *, struct dsc *);
extern struct idl *DLL_EXPORT CMP_get_index_lock(TDBB, struct rel *, USHORT);
extern SLONG DLL_EXPORT CMP_impure(struct csb *, USHORT);
extern struct req *DLL_EXPORT CMP_make_request(TDBB, struct csb **);
extern int DLL_EXPORT CMP_post_access(TDBB,
									  struct csb*,
									  /* INOUT */ TEXT*,
									  struct rel*,
									  CONST TEXT*,
									  CONST TEXT*,
									  USHORT,
									  CONST TEXT*,
									  CONST TEXT*);
extern void DLL_EXPORT CMP_post_resource(TDBB, struct rsc **, struct blk *,
										 enum rsc_s, USHORT);
extern void DLL_EXPORT CMP_release_resource(struct rsc **, enum rsc_s,
											USHORT);
extern void DLL_EXPORT CMP_release(TDBB, struct req *);
extern void DLL_EXPORT CMP_shutdown_database(TDBB);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _JRD_CMP_PROTO_H_ */
