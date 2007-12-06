/*
 *	PROGRAM:	JRD Access method
 *	MODULE:		dyn_ut_proto.h
 *	DESCRIPTION:	Prototype Header file for dyn_util.e
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


#ifndef _JRD_DYN_UT_PROTO_H_
#define _JRD_DYN_UT_PROTO_H_

extern void DYN_UTIL_store_check_constraints(TDBB, GBL, TEXT *, TEXT *);
extern BOOLEAN DYN_UTIL_get_prot(TDBB, GBL, SCHAR *, SCHAR *, USHORT *);
extern void DYN_UTIL_generate_trigger_name(TDBB, GBL, TEXT *);
extern void DYN_UTIL_generate_index_name(TDBB, GBL, TEXT *, UCHAR);
extern void DYN_UTIL_generate_field_position(TDBB, GBL, TEXT *, SLONG *);
extern void DYN_UTIL_generate_field_name(TDBB, GBL, TEXT *);
extern void DYN_UTIL_generate_constraint_name(TDBB, GBL, TEXT *);
extern SINT64 DYN_UTIL_gen_unique_id(TDBB, GBL, SSHORT, SCHAR *, BLK *);

#endif /* _JRD_DYN_UT_PROTO_H_ */
