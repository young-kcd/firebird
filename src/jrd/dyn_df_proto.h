/*
 *	PROGRAM:	JRD Access method
 *	MODULE:		dyn_df_proto.h
 *	DESCRIPTION:	Prototype Header file for dyn_define.e
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

#ifndef _JRD_DYN_DF_PROTO_H_
#define _JRD_DYN_DF_PROTO_H_

extern void DYN_define_cache(GBL, UCHAR **);
extern void DYN_define_constraint(GBL, UCHAR **, TEXT *, TEXT *);
extern void DYN_define_dimension(GBL, UCHAR **, TEXT *, TEXT *);
extern void DYN_define_exception(GBL, UCHAR **);
extern void DYN_define_file(GBL, UCHAR **, SLONG, SLONG *, USHORT);
extern void DYN_define_filter(GBL, UCHAR **);
extern void DYN_define_function(GBL, UCHAR **);
extern void DYN_define_function_arg(GBL, UCHAR **, TEXT *);
extern void DYN_define_generator(GBL, UCHAR **);
extern void DYN_define_global_field(GBL, UCHAR **, TEXT *, TEXT *);
extern void DYN_define_index(GBL, UCHAR **, TEXT *, UCHAR, TEXT *, TEXT *,
							 TEXT *, UCHAR *);
extern void DYN_define_local_field(GBL, UCHAR **, TEXT *, TEXT *);
extern void DYN_define_log_file(GBL, UCHAR **, USHORT, USHORT);
extern void DYN_define_parameter(GBL, UCHAR **, TEXT *);
extern void DYN_define_procedure(GBL, UCHAR **);
extern void DYN_define_relation(GBL, UCHAR **);
extern void DYN_define_role(GBL, UCHAR **);
extern void DYN_define_security_class(GBL, UCHAR **);
extern void DYN_define_shadow(GBL, UCHAR **);
extern void DYN_define_sql_field(GBL, UCHAR **, TEXT *, TEXT *);
extern void DYN_define_trigger(GBL, UCHAR **, TEXT *, TEXT *, BOOLEAN);
extern void DYN_define_trigger_msg(GBL, UCHAR **, TEXT *);
extern void DYN_define_view_relation(GBL, UCHAR **, TEXT *);

#endif /* _JRD_DYN_DF_PROTO_H_ */
