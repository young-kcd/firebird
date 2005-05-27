/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		make_proto.h
 *	DESCRIPTION:	Prototype Header file for make.cpp
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
 *
 * 2002-07-20 Arno Brinkman: Added MAKE_desc_from_list
 */

#ifndef DSQL_MAKE_PROTO_H
#define DSQL_MAKE_PROTO_H

#include "../dsql/sym.h"

dsql_nod* MAKE_constant(class dsql_str*, dsql_constant_type);
dsql_nod* MAKE_str_constant(class dsql_str*, SSHORT);
class dsql_str* MAKE_cstring(const char*);
void MAKE_desc(dsql_req*, dsc*, dsql_nod*, dsql_nod*);
void MAKE_desc_from_field(dsc*, const class dsql_fld*);
void MAKE_desc_from_list(dsql_req*, dsc*, dsql_nod*, dsql_nod*, const TEXT*);
dsql_nod* MAKE_field(class dsql_ctx*, class dsql_fld*, dsql_nod*);
dsql_nod* MAKE_list(DsqlNodStack&);
dsql_nod* MAKE_node(enum nod_t, int);
class dsql_par* MAKE_parameter(class dsql_msg* , bool, bool, USHORT);
class dsql_str* MAKE_string(const char* , int);
dsql_sym* MAKE_symbol(dsql_dbb*, const TEXT*, USHORT,
						   enum sym_type, class dsql_req*);
class dsql_str* MAKE_tagged_string(const char* str, size_t length, const char* charset);
dsql_nod* MAKE_trigger_type(dsql_nod*, dsql_nod*);
dsql_nod* MAKE_variable(class dsql_fld*, const TEXT*, USHORT, USHORT,
								 USHORT, USHORT);


#endif // DSQL_MAKE_PROTO_H

