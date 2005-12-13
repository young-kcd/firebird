/*
 *	PROGRAM:	JRD Access method
 *	MODULE:		dyn_dl_proto.h
 *	DESCRIPTION:	Prototype Header file for dyn_delete.e
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

#ifndef _JRD_DYN_DL_PROTO_H_
#define _JRD_DYN_DL_PROTO_H_

extern void DYN_delete_constraint(GBL, UCHAR **, TEXT *);
extern void DYN_delete_dimensions(GBL, UCHAR **, TEXT *, TEXT *);
extern void DYN_delete_exception(GBL, UCHAR **);
extern void DYN_delete_filter(GBL, UCHAR **);
extern void DYN_delete_function(GBL, UCHAR **);
extern void	DYN_delete_generator(GBL, UCHAR **);
extern void DYN_delete_global_field(GBL, UCHAR **);
extern void DYN_delete_index(GBL, UCHAR **);
extern void DYN_delete_local_field(GBL, UCHAR **, TEXT *, TEXT *);
extern void DYN_delete_parameter(GBL, UCHAR **, TEXT *);
extern void DYN_delete_procedure(GBL, UCHAR **);
extern void DYN_delete_relation(GBL, UCHAR **, TEXT *);
extern void DYN_delete_role(GBL, UCHAR **);
extern void DYN_delete_security_class(GBL, UCHAR **);
extern void DYN_delete_shadow(GBL, UCHAR **);
extern void DYN_delete_trigger(GBL, UCHAR **);
extern void DYN_delete_trigger_msg(GBL, UCHAR **, TEXT *);

#endif /* _JRD_DYN_DL_PROTO_H_ */
