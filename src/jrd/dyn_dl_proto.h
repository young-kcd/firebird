/*
 *	PROGRAM:	JRD Access method
 *	MODULE:		dyn_dl_proto.h
 *	DESCRIPTION:	Prototype Header file for dyn_del.epp
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

#ifndef JRD_DYN_DL_PROTO_H
#define JRD_DYN_DL_PROTO_H

void	DYN_delete_constraint(Jrd::gbl*, const UCHAR**, const TEXT*);
void	DYN_delete_dimensions(Jrd::gbl*, const UCHAR**, const TEXT*, TEXT*);
void	DYN_delete_exception(Jrd::gbl*, const UCHAR**);
void	DYN_delete_filter(Jrd::gbl*, const UCHAR**);
void	DYN_delete_function(Jrd::gbl*, const UCHAR**);
void	DYN_delete_generator(Jrd::gbl*, const UCHAR**);
void	DYN_delete_global_field(Jrd::gbl*, const UCHAR**);
void	DYN_delete_index(Jrd::gbl*, const UCHAR**);
void	DYN_delete_local_field(Jrd::gbl*, const UCHAR**, const TEXT*, TEXT*);
void	DYN_delete_parameter(Jrd::gbl*, const UCHAR**, TEXT*);
void	DYN_delete_procedure(Jrd::gbl*, const UCHAR**);
void	DYN_delete_relation(Jrd::gbl*, const UCHAR**, const TEXT*);
void	DYN_delete_role(Jrd::gbl*, const UCHAR**);
void	DYN_delete_security_class(Jrd::gbl*, const UCHAR**);
void	DYN_delete_shadow(Jrd::gbl*, const UCHAR**);
void	DYN_delete_trigger(Jrd::gbl*, const UCHAR**);
void	DYN_delete_trigger_msg(Jrd::gbl*, const UCHAR**, TEXT*);

#endif // JRD_DYN_DL_PROTO_H

