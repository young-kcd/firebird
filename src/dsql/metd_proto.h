/*
 *	PROGRAM:	 Dynamic SQL runtime support
 *	MODULE:		 metd_proto.h
 *	DESCRIPTION: Prototype Header file for metd.epp
 *               This is a DSQL private header file. It is not included
 *               by anything but DSQL itself.
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

#ifndef DSQL_METD_PROTO_H
#define DSQL_METD_PROTO_H

// forward declarations
class dsql_req;
class dsql_str;

void METD_drop_function(dsql_req*, const dsql_str*);
void METD_drop_procedure(dsql_req*, const dsql_str*);
void METD_drop_relation(dsql_req*, const dsql_str*);

dsql_intlsym*  METD_get_charset(dsql_req*, USHORT, const char* name); // UTF-8
USHORT   METD_get_charset_bpc(dsql_req*, SSHORT);
dsql_intlsym* METD_get_collation(dsql_req*, const dsql_str*);
USHORT   METD_get_col_default(dsql_req*, const char*, const char*, bool*, TEXT*, USHORT);
dsql_str*      METD_get_default_charset(dsql_req*);
USHORT   METD_get_domain(dsql_req*, class dsql_fld*, const char* name); // UTF-8
USHORT   METD_get_domain_default(dsql_req*, const TEXT*, bool*, TEXT*, USHORT);
bool METD_get_exception(dsql_req*, const dsql_str*);
dsql_udf*      METD_get_function(dsql_req*, const dsql_str*);
dsql_nod* METD_get_primary_key(dsql_req*, const dsql_str*);
dsql_prc* METD_get_procedure(dsql_req*, const dsql_str*);
dsql_rel* METD_get_relation(dsql_req*, const dsql_str*);
dsql_str*      METD_get_trigger_relation(dsql_req*, const dsql_str*, USHORT*);
bool   METD_get_type(dsql_req*, const dsql_str*, char*, SSHORT*);
dsql_rel* METD_get_view_relation(dsql_req*   request,
								const char* view_name,         // UTF-8
								const char* relation_or_alias, // UTF-8
								USHORT      level);

#endif // DSQL_METD_PROTO_H

