/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		dfw_proto.h
 *	DESCRIPTION:	Prototype header file for dfw.cpp
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

#ifndef JRD_DFW_PROTO_H
#define JRD_DFW_PROTO_H

USHORT DFW_assign_index_type(Jrd::DeferredWork*, SSHORT, SSHORT);
void DFW_delete_deferred(Jrd::jrd_tra*, SLONG);
void DFW_merge_work(Jrd::jrd_tra*, SLONG, SLONG);
void DFW_perform_system_work(void);
void DFW_perform_work(Jrd::jrd_tra*);
void DFW_perform_post_commit_work(Jrd::jrd_tra*);
Jrd::DeferredWork* DFW_post_work(Jrd::jrd_tra*, enum Jrd::dfw_t, const dsc*, USHORT);
Jrd::DeferredWork* DFW_post_work_arg(Jrd::jrd_tra*, Jrd::DeferredWork*, const dsc*, USHORT);
void DFW_update_index(const TEXT*, USHORT, const Jrd::SelectivityList&);

#endif // JRD_DFW_PROTO_H

