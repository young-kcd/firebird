/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		scl_proto.h
 *	DESCRIPTION:	Prototype header file for scl.epp
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

#ifndef JRD_SCL_PROTO_H
#define JRD_SCL_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

void SCL_check_access(const struct scl*, SLONG, const TEXT*,
					  const TEXT*, USHORT, const TEXT*, const TEXT*);
void SCL_check_procedure(struct dsc*, USHORT);
void SCL_check_relation(struct dsc*, USHORT);
struct scl* SCL_get_class(/* INOUT */ TEXT*);
int SCL_get_mask(const TEXT*, const TEXT*);
void SCL_init(bool, const TEXT*, TEXT*, TEXT*, TEXT*, TEXT*, TDBB, bool);
void SCL_move_priv(UCHAR**, USHORT, STR*, ULONG*);
struct scl* SCL_recompute_class(TDBB, TEXT*);
void SCL_release(struct scl*);
void SCL_check_index(TDBB, TEXT*, UCHAR, USHORT);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JRD_SCL_PROTO_H */

