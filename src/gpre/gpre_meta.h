/*
 *	PROGRAM:	Preprocessor
 *	MODULE:		gpre_meta.h
 *	DESCRIPTION:	Prototype header file for gpre_meta.epp and gpre_meta_boot.cpp
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

#ifndef GPRE_GPRE_META_H
#define GPRE_GPRE_META_H

GPRE_FLD	MET_context_field(GPRE_CTX, char*);
bool		MET_database(DBB, bool);
bool		MET_domain_lookup(GPRE_REQ, GPRE_FLD, char*);
GPRE_FLD	MET_field(GPRE_REL, char*);
GPRE_NOD	MET_fields(GPRE_CTX);
void		MET_fini(DBB);
const SCHAR*		MET_generator(const TEXT*, DBB);
bool		MET_get_column_default(GPRE_REL, TEXT*, TEXT*, USHORT);
bool		MET_get_domain_default(DBB, const TEXT*, TEXT*, USHORT);
USHORT		MET_get_dtype(USHORT, USHORT, USHORT*);
LLS			MET_get_primary_key(DBB, TEXT*);
GPRE_PRC	MET_get_procedure(DBB, TEXT*, TEXT*);
GPRE_REL	MET_get_relation(DBB, TEXT*, TEXT*);
INTLSYM		MET_get_text_subtype(SSHORT);
UDF			MET_get_udf(DBB, TEXT*);
GPRE_REL	MET_get_view_relation(GPRE_REQ, const char*, const char*, USHORT);
IND			MET_index(DBB, TEXT*);
void		MET_load_hash_table(DBB);
GPRE_FLD	MET_make_field(SCHAR*, SSHORT, SSHORT, bool);
IND			MET_make_index(SCHAR*);
GPRE_REL	MET_make_relation(SCHAR*);
bool		MET_type(GPRE_FLD, TEXT*, SSHORT*);
bool		MET_trigger_exists(DBB, TEXT*);

#endif // GPRE_GPRE_META_H

