/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		dfw_proto.h
 *	DESCRIPTION:	Prototype header file for dfw.c
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

#ifndef _JRD_DFW_PROTO_H_
#define _JRD_DFW_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

USHORT DFW_assign_index_type(struct dfw *, SSHORT, SSHORT);
void DFW_delete_deferred(struct jrd_tra *, SLONG);
void DFW_merge_work(struct jrd_tra *, SLONG, SLONG);
void DFW_perform_system_work(void);
void DFW_perform_work(struct jrd_tra *);
void DFW_perform_post_commit_work(struct jrd_tra *);
class dfw * DFW_post_work(struct jrd_tra *, ENUM dfw_t, struct dsc *, USHORT);
void DFW_post_work_arg(struct jrd_tra *, class dfw *, struct dsc *, USHORT);
void DFW_update_index(struct dfw *, USHORT, float);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _JRD_DFW_PROTO_H_ */
