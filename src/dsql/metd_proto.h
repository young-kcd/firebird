/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		metd_proto.h
 *	DESCRIPTION:	Prototype Header file for metd.e
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

#ifndef _DSQL_METD_PROTO_H
#define _DSQL_METD_PROTO_H

void METD_drop_function(class dsql_req*, class str*);
void METD_drop_procedure(class dsql_req*, class str*);
void METD_drop_relation(class dsql_req*, class str*);
INTLSYM METD_get_charset(class dsql_req*, USHORT, UCHAR *);
USHORT METD_get_charset_bpc (struct dsql_req *, SSHORT);
INTLSYM METD_get_collation(class dsql_req*, class str*);
void METD_get_col_default(DSQL_REQ, TEXT*, TEXT*, BOOLEAN*, TEXT*, USHORT);
STR METD_get_default_charset(class dsql_req*);
USHORT METD_get_domain(class dsql_req*, class dsql_fld*, UCHAR*);
void METD_get_domain_default(class dsql_req*, TEXT*, BOOLEAN*, TEXT*, USHORT);
UDF METD_get_function(class dsql_req*, class str*);
DSQL_NOD METD_get_primary_key(class dsql_req*, class str*);
DSQL_PRC METD_get_procedure(class dsql_req*, class str*);
DSQL_REL METD_get_relation(class dsql_req*, class str*);
STR METD_get_trigger_relation(class dsql_req*, class str*, USHORT*);
USHORT METD_get_type(class dsql_req*, class str*, UCHAR*, SSHORT*);
DSQL_REL METD_get_view_relation(class dsql_req*, UCHAR*, UCHAR*, USHORT);


#endif /*_DSQL_METD_PROTO_H */
