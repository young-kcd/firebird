/*
 *	PROGRAM:	JRD Access method
 *	MODULE:		dyn_md_proto.h
 *	DESCRIPTION:	Prototype Header file for dyn_modify.e
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

#ifndef JRD_DYN_MD_PROTO_H
#define JRD_DYN_MD_PROTO_H

void DYN_modify_database(GBL, UCHAR **);
void DYN_modify_exception(GBL, UCHAR **);
void DYN_modify_global_field(GBL, UCHAR **, TEXT *, TEXT *);
void DYN_modify_index(GBL, UCHAR **);
void DYN_modify_local_field(GBL, UCHAR **, TEXT *, TEXT *);
void DYN_modify_procedure(GBL, UCHAR **);
void DYN_modify_relation(GBL, UCHAR **);
void DYN_modify_trigger(GBL, UCHAR **);
void DYN_modify_trigger_msg(GBL, UCHAR **, TEXT *);
void DYN_modify_sql_field(GBL, UCHAR**, TEXT*, TEXT*);
void DYN_modify_view(GBL, UCHAR **);


#endif // JRD_DYN_MD_PROTO_H
