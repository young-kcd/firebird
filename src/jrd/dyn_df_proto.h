/*
 *	PROGRAM:	JRD Access method
 *	MODULE:		dyn_df_proto.h
 *	DESCRIPTION:	Prototype Header file for dyn_def.epp
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

#ifndef JRD_DYN_DF_PROTO_H
#define JRD_DYN_DF_PROTO_H

void DYN_define_cache(GBL, const UCHAR**);
void DYN_define_constraint(GBL, const UCHAR**, const TEXT*, TEXT*);
void DYN_define_dimension(GBL, const UCHAR**, const TEXT*, TEXT*);
void DYN_define_exception(GBL, const UCHAR**);
void DYN_define_file(GBL, const UCHAR**, SLONG, SLONG*, USHORT);
void DYN_define_filter(GBL, const UCHAR**);
void DYN_define_function(GBL, const UCHAR**);
void DYN_define_function_arg(GBL, const UCHAR**, TEXT*);
void DYN_define_generator(GBL, const UCHAR**);
void DYN_define_global_field(GBL, const UCHAR**, const TEXT*, TEXT*);
void DYN_define_index(GBL, const UCHAR**, const TEXT*, UCHAR, TEXT*, TEXT*,
							 TEXT*, UCHAR *);
void DYN_define_local_field(GBL, const UCHAR**, const TEXT*, TEXT*);
void DYN_define_log_file(GBL, const UCHAR**, bool, bool);
void DYN_define_parameter(GBL, const UCHAR**, TEXT*);
void DYN_define_procedure(GBL, const UCHAR**);
void DYN_define_relation(GBL, const UCHAR**);
void DYN_define_role(GBL, const UCHAR**);
void DYN_define_security_class(GBL, const UCHAR**);
void DYN_define_shadow(GBL, const UCHAR**);
void DYN_define_sql_field(GBL, const UCHAR**, const TEXT*, TEXT*);
void DYN_define_trigger(GBL, const UCHAR**, const TEXT*, TEXT*, const bool);
void DYN_define_trigger_msg(GBL, const UCHAR**, const TEXT*);
void DYN_define_view_relation(GBL, const UCHAR**, const TEXT*);
void DYN_define_difference(GBL, const UCHAR**);

#endif // JRD_DYN_DF_PROTO_H

