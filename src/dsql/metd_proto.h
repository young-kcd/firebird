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

#ifndef __cplusplus
#error This header file can only be used from C++
#endif

// forward declarations
class dsql_req;
class str;

void METD_drop_function(dsql_req*, str*);
void METD_drop_procedure(dsql_req*, str*);
void METD_drop_relation(dsql_req*, str*);

INTLSYM  METD_get_charset(dsql_req*, USHORT, const char* name /* UTF-8 */);
USHORT   METD_get_charset_bpc (struct dsql_req *, SSHORT);
INTLSYM  METD_get_collation(dsql_req*, str*);
void     METD_get_col_default(DSQL_REQ, const char*, const char*, bool*, TEXT*, USHORT);
STR      METD_get_default_charset(dsql_req*);
USHORT   METD_get_domain(dsql_req*, class dsql_fld*, const char* name /* UTF-8 */);
void     METD_get_domain_default(dsql_req*, TEXT*, bool*, TEXT*, USHORT);
UDF      METD_get_function(dsql_req*, str*);
DSQL_NOD METD_get_primary_key(dsql_req*, str*);
DSQL_PRC METD_get_procedure(dsql_req*, str*);
DSQL_REL METD_get_relation(dsql_req*, str*);
STR      METD_get_trigger_relation(dsql_req*, str*, USHORT*);
USHORT   METD_get_type(dsql_req*, str*, char*, SSHORT*);
DSQL_REL METD_get_view_relation(dsql_req*   request,
								const char* view_name         /* UTF-8 */,
								const char* relation_or_alias /* UTF-8 */,
								USHORT      level);

#endif // DSQL_METD_PROTO_H

