/*
 *	PROGRAM:	JRD Access method
 *	MODULE:		dyn_md_proto.h
 *	DESCRIPTION:	Prototype Header file for dyn_mod.epp
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

void DYN_modify_database(Jrd::gbl*, const UCHAR**);
void DYN_modify_exception(Jrd::gbl*, const UCHAR**);
void DYN_modify_global_field(Jrd::gbl*, const UCHAR**, const TEXT*, TEXT*);
void DYN_modify_index(Jrd::gbl*, const UCHAR**);
void DYN_modify_local_field(Jrd::gbl*, const UCHAR**, const TEXT*, TEXT*);
void DYN_modify_procedure(Jrd::gbl*, const UCHAR**);
void DYN_modify_relation(Jrd::gbl*, const UCHAR**);
void DYN_modify_trigger(Jrd::gbl*, const UCHAR**);
void DYN_modify_trigger_msg(Jrd::gbl*, const UCHAR**, TEXT*);
void DYN_modify_sql_field(Jrd::gbl*, const UCHAR**, const TEXT*, TEXT*);
void DYN_modify_view(Jrd::gbl*, const UCHAR**);


#endif // JRD_DYN_MD_PROTO_H

