/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		make_proto.h
 *	DESCRIPTION:	Prototype Header file for make.c
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

#ifndef _DSQL_MAKE_PROTO_H_
#define _DSQL_MAKE_PROTO_H_

#include "../dsql/sym.h"

struct nod* MAKE_constant(class str* , int);
struct nod* MAKE_str_constant(class str* , SSHORT);
class str* MAKE_cstring(CONST SCHAR*);
void MAKE_desc(struct dsc* , struct nod*);
void MAKE_desc_from_field(struct dsc* , class fld*);
struct nod* MAKE_field(class ctx* , class fld* , struct nod*);
struct nod* MAKE_list(class dsql_lls*);
struct nod* MAKE_node(ENUM nod_t, int);
class par* MAKE_parameter(class msg* , USHORT, USHORT);
class str* MAKE_string(CONST UCHAR* , int);
struct sym* MAKE_symbol(class dbb* , CONST TEXT* , USHORT,
						   ENUM sym_type, class req*);
class str* MAKE_tagged_string(CONST UCHAR* , int, CONST TEXT*);
struct nod* MAKE_variable(class fld* , CONST TEXT* , USHORT, USHORT,
								 USHORT, USHORT);


#endif /* _DSQL_MAKE_PROTO_H_ */
