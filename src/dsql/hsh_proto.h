/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		hsh_proto.h
 *	DESCRIPTION:	Prototype Header file for hsh.cpp
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

#ifndef DSQL_HSH_PROTO_H
#define DSQL_HSH_PROTO_H

void HSHD_fini(void);
void HSHD_finish(const void*);
void HSHD_init(void);
void HSHD_insert(dsql_sym*);
DSQL_SYM HSHD_lookup(const void*, const TEXT*, SSHORT, SYM_TYPE, USHORT);
void HSHD_remove(dsql_sym*);
void HSHD_set_flag(const void *, const TEXT*, SSHORT, SYM_TYPE, SSHORT);

#endif /*DSQL_HSH_PROTO_H*/

