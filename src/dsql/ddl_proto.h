/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		ddl_proto.h
 *	DESCRIPTION:	Prototype Header file for ddl.cpp
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

#ifndef DSQL_DDL_PROTO_H
#define DSQL_DDL_PROTO_H

// This is a DSQL internal file. Not to be used by anything but
// the DSQL module itself.

class dsql_req;
class dsql_fld;
class dsql_nod;
class str;

void DDL_execute(dsql_req*);
void DDL_generate(dsql_req*, dsql_nod*);
bool DDL_ids(const dsql_req*);
void DDL_put_field_dtype(dsql_req*, const dsql_fld*, bool);
void DDL_resolve_intl_type(dsql_req*, dsql_fld*, const str*);
void DDL_resolve_intl_type2(dsql_req*, dsql_fld*, const str*, bool);

#endif // DSQL_DDL_PROTO_H

