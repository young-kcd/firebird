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

void DYN_define_cache(Jrd::gbl*, const UCHAR**);
void DYN_define_constraint(Jrd::gbl*, const UCHAR**, const TEXT*, TEXT*);
void DYN_define_dimension(Jrd::gbl*, const UCHAR**, const TEXT*, TEXT*);
void DYN_define_exception(Jrd::gbl*, const UCHAR**);
void DYN_define_file(Jrd::gbl*, const UCHAR**, SLONG, SLONG*, USHORT);
void DYN_define_filter(Jrd::gbl*, const UCHAR**);
void DYN_define_function(Jrd::gbl*, const UCHAR**);
void DYN_define_function_arg(Jrd::gbl*, const UCHAR**, TEXT*);
void DYN_define_generator(Jrd::gbl*, const UCHAR**);
void DYN_define_global_field(Jrd::gbl*, const UCHAR**, const TEXT*, TEXT*);
void DYN_define_index(Jrd::gbl*, const UCHAR**, const TEXT*, UCHAR, TEXT*, TEXT*,
							 TEXT*, UCHAR *);
void DYN_define_local_field(Jrd::gbl*, const UCHAR**, const TEXT*, TEXT*);
void DYN_define_log_file(Jrd::gbl*, const UCHAR**, bool, bool);
void DYN_define_parameter(Jrd::gbl*, const UCHAR**, TEXT*);
void DYN_define_procedure(Jrd::gbl*, const UCHAR**);
void DYN_define_relation(Jrd::gbl*, const UCHAR**);
void DYN_define_role(Jrd::gbl*, const UCHAR**);
void DYN_define_security_class(Jrd::gbl*, const UCHAR**);
void DYN_define_shadow(Jrd::gbl*, const UCHAR**);
void DYN_define_sql_field(Jrd::gbl*, const UCHAR**, const TEXT*, TEXT*);
void DYN_define_trigger(Jrd::gbl*, const UCHAR**, const TEXT*, TEXT*, const bool);
void DYN_define_trigger_msg(Jrd::gbl*, const UCHAR**, const TEXT*);
void DYN_define_view_relation(Jrd::gbl*, const UCHAR**, const TEXT*);
void DYN_define_difference(Jrd::gbl*, const UCHAR**);

#endif // JRD_DYN_DF_PROTO_H

