/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		scl_proto.h
 *	DESCRIPTION:	Prototype header file for scl.c
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

#ifndef _JRD_SCL_PROTO_H_
#define _JRD_SCL_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

void		SCL_check_access(struct scl*, struct jrd_rel*, const TEXT*,
							const TEXT*, USHORT,
							const TEXT*, const TEXT*);
void		SCL_check_procedure(struct dsc*, USHORT);
void		SCL_check_relation(struct dsc*, USHORT);
struct scl*	SCL_get_class(/* INOUT */ TEXT*);
int			SCL_get_mask(const TEXT*, const TEXT*);
void		SCL_init(BOOLEAN, TEXT*, TEXT*, TEXT*, TEXT*, TEXT*, TDBB, BOOLEAN);
void		SCL_move_priv(UCHAR**, USHORT, STR*, ULONG*);
struct scl*	SCL_recompute_class(TDBB, TEXT*);
void		SCL_release(struct scl*);
void		SCL_check_index(TDBB, TEXT*, UCHAR, USHORT);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _JRD_SCL_PROTO_H_ */
