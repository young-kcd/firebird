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
 *
 * 2002-07-20 Arno Brinkman: Added MAKE_desc_from_list
 */

#ifndef _DSQL_MAKE_PROTO_H_
#define _DSQL_MAKE_PROTO_H_

#include "../dsql/sym.h"

struct dsql_nod* MAKE_constant(class str* , int);
struct dsql_nod* MAKE_str_constant(class str* , SSHORT);
class str* MAKE_cstring(const char*);
void MAKE_desc(struct dsc* , struct dsql_nod*);
void MAKE_desc_from_field(struct dsc* , class dsql_fld*);
void MAKE_desc_from_list(struct dsc* , struct dsql_nod*, const TEXT*);
struct dsql_nod* MAKE_field(class dsql_ctx* , class dsql_fld* , struct dsql_nod*);
struct dsql_nod* MAKE_list(class dsql_lls*);
struct dsql_nod* MAKE_node(enum nod_t, int);
class par* MAKE_parameter(class dsql_msg* , USHORT, USHORT, USHORT);
class str* MAKE_string(const char* , int);
struct sym* MAKE_symbol(class dbb* , const TEXT* , USHORT,
						   enum sym_type, class dsql_req*);
class str* MAKE_tagged_string(const char* str, size_t length, const char* charset);
struct dsql_nod* MAKE_trigger_type(struct dsql_nod*, struct dsql_nod*);
struct dsql_nod* MAKE_variable(class dsql_fld* , const TEXT* , USHORT, USHORT,
								 USHORT, USHORT);


#endif /* _DSQL_MAKE_PROTO_H_ */
