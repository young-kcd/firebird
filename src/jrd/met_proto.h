/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		met_proto.h
 *	DESCRIPTION:	Prototype header file for met.c
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

#ifndef _JRD_MET_PROTO_H_
#define _JRD_MET_PROTO_H_

#include "../jrd/exe.h"
#include "../jrd/jrn.h"
#include "../jrd/blob_filter.h"

#ifdef __cplusplus
extern "C" {
#endif

void MET_activate_shadow(TDBB);
ULONG MET_align(struct dsc *, USHORT);
void MET_change_fields(TDBB, struct tra *, struct dsc *);
struct fmt *MET_current(TDBB, struct rel *);
void MET_delete_dependencies(TDBB, TEXT *, USHORT);
void MET_delete_shadow(TDBB, USHORT);
void MET_error(TEXT *, ...);
SCHAR *MET_exact_name(TEXT *);
struct fmt *MET_format(TDBB, register struct rel *, USHORT);
BOOLEAN MET_get_char_subtype(TDBB, SSHORT *, UCHAR *, USHORT);
struct nod *MET_get_dependencies(TDBB, struct rel*, TEXT*,
								class Csb*, SLONG[2], struct req**,
								class Csb **, CONST TEXT*, USHORT);
struct fld *MET_get_field(struct rel *, USHORT);
void MET_get_shadow_files(TDBB, USHORT);
int MET_get_walinfo(TDBB, struct logfiles **, ULONG *,
						   struct logfiles **);
void MET_load_trigger(TDBB, struct rel *, TEXT *, VEC *);
void DLL_EXPORT MET_lookup_cnstrt_for_index(TDBB, TEXT *, TEXT *);
void MET_lookup_cnstrt_for_trigger(TDBB, TEXT *, TEXT *, TEXT *);
void MET_lookup_exception(TDBB, SLONG, /* INOUT */ TEXT*, /* INOUT */ TEXT*);
SLONG MET_lookup_exception_number(TDBB, TEXT*);
int MET_lookup_field(TDBB, struct rel*, CONST TEXT*);
BLF MET_lookup_filter(TDBB, SSHORT, SSHORT);
SLONG MET_lookup_generator(TDBB, TEXT *);
void DLL_EXPORT MET_lookup_index(TDBB, TEXT *, TEXT *, USHORT);
SLONG MET_lookup_index_name(TDBB, TEXT *, SLONG *, SSHORT *);
int MET_lookup_partner(TDBB, struct rel *, struct idx *, UCHAR *);
struct prc *MET_lookup_procedure(TDBB, SCHAR *);
struct prc *MET_lookup_procedure_id(TDBB, SSHORT, BOOLEAN, USHORT);
struct rel *MET_lookup_relation(TDBB, CONST SCHAR*);
struct rel *MET_lookup_relation_id(TDBB, SLONG, BOOLEAN);
struct nod *MET_parse_blob(TDBB, struct rel *, SLONG[2], class Csb **,
								  struct req **, BOOLEAN, BOOLEAN);
void MET_parse_sys_trigger(TDBB, struct rel *);
int MET_post_existence(TDBB, struct rel *);
void MET_prepare(TDBB, struct tra *, USHORT, UCHAR *);
struct prc *MET_procedure(TDBB, int, USHORT);
struct rel *MET_relation(TDBB, USHORT);
void MET_release_existence(struct rel *);
void MET_release_triggers(TDBB, VEC *);
void MET_remove_procedure(TDBB, int, PRC);
void MET_revoke(TDBB, struct tra *, TEXT *, TEXT *, TEXT *);
TEXT*MET_save_name(TDBB, CONST TEXT*);
void MET_scan_relation(TDBB, struct rel *);
TEXT *MET_trigger_msg(TDBB, TEXT *, USHORT);
void MET_update_shadow(TDBB, struct sdw *, USHORT);
void MET_update_transaction(TDBB, struct tra *, USHORT);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _JRD_MET_PROTO_H_ */
