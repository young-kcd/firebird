/*
 *	PROGRAM:	JRD Access method
 *	MODULE:		dyn_ut_proto.h
 *	DESCRIPTION:	Prototype Header file for dyn_util.epp
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


#ifndef JRD_DYN_UT_PROTO_H
#define JRD_DYN_UT_PROTO_H

void	DYN_UTIL_store_check_constraints(thread_db*, GBL, const TEXT*, const TEXT*);
bool	DYN_UTIL_get_prot(thread_db*, GBL, const SCHAR*, const SCHAR*, USHORT*);
void	DYN_UTIL_generate_trigger_name(thread_db*, GBL, TEXT*);
void	DYN_UTIL_generate_index_name(thread_db*, GBL, TEXT*, UCHAR);
void	DYN_UTIL_generate_field_position(thread_db*, GBL, const TEXT*, SLONG*);
void	DYN_UTIL_generate_field_name(thread_db*, GBL, TEXT*);
void	DYN_UTIL_generate_constraint_name(thread_db*, GBL, TEXT*);
SINT64	DYN_UTIL_gen_unique_id(thread_db*, GBL, SSHORT, const SCHAR*, BLK*);

#endif // JRD_DYN_UT_PROTO_H

