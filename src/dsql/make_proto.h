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

dsql_nod* MAKE_constant(class str*, int);
dsql_nod* MAKE_str_constant(class str*, SSHORT);
class str* MAKE_cstring(const char*);
void MAKE_desc(dsc*, dsql_nod*);
void MAKE_desc_from_field(dsc*, const class dsql_fld*);
void MAKE_desc_from_list(dsc*, dsql_nod*, const TEXT*);
dsql_nod* MAKE_field(class dsql_ctx*, class dsql_fld*, dsql_nod*);
dsql_nod* MAKE_list(class dsql_lls*);
dsql_nod* MAKE_node(enum nod_t, int);
class par* MAKE_parameter(class dsql_msg* , bool, bool, USHORT);
class str* MAKE_string(const char* , int);
dsql_sym* MAKE_symbol(class dbb*, const TEXT*, USHORT,
						   enum sym_type, class dsql_req*);
class str* MAKE_tagged_string(const char* str, size_t length, const char* charset);
dsql_nod* MAKE_trigger_type(dsql_nod*, dsql_nod*);
dsql_nod* MAKE_variable(class dsql_fld*, const TEXT*, USHORT, USHORT,
								 USHORT, USHORT);


#endif // DSQL_MAKE_PROTO_H

