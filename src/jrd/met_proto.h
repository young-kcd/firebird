/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		met_proto.h
 *	DESCRIPTION:	Prototype header file for met.cpp
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

#ifndef JRD_MET_PROTO_H
#define JRD_MET_PROTO_H

#include "../jrd/exe.h"
#include "../jrd/blob_filter.h"

class jrd_tra;
class jrd_req;
class jrd_prc;
class fmt;
class jrd_rel;
class Csb;
class jrd_nod;

void		MET_activate_shadow(thread_db*);
ULONG		MET_align(const struct dsc*, USHORT);
void		MET_change_fields(thread_db*, jrd_tra *, struct dsc *);
fmt*		MET_current(thread_db*, jrd_rel*);
void		MET_delete_dependencies(thread_db*, const TEXT*, USHORT);
void		MET_delete_shadow(thread_db*, USHORT);
void		MET_error(const TEXT*, ...);
fmt*		MET_format(thread_db*, jrd_rel*, USHORT);
bool		MET_get_char_subtype(thread_db*, SSHORT*, const UCHAR*, USHORT);
jrd_nod*	MET_get_dependencies(thread_db*, jrd_rel*, const TEXT*,
								Csb*, struct bid*, jrd_req**,
								Csb**, const TEXT*, USHORT);
class jrd_fld*	MET_get_field(jrd_rel*, USHORT);
void		MET_get_shadow_files(thread_db*, bool);
void		MET_load_trigger(thread_db*, jrd_rel*, const TEXT*, trig_vec**);
void		MET_lookup_cnstrt_for_index(thread_db*, TEXT* constraint, const TEXT* index_name);
void		MET_lookup_cnstrt_for_trigger(thread_db*, TEXT*, TEXT*, const TEXT*);
void		MET_lookup_exception(thread_db*, SLONG, /* INOUT */ TEXT*, /* INOUT */ TEXT*);
SLONG		MET_lookup_exception_number(thread_db*, const TEXT*);
int			MET_lookup_field(thread_db*, jrd_rel*, const TEXT*, const TEXT*);
BLF			MET_lookup_filter(thread_db*, SSHORT, SSHORT);
SLONG		MET_lookup_generator(thread_db*, const TEXT*);
void		MET_lookup_generator_id(thread_db*, SLONG, TEXT *);
void		MET_lookup_index(thread_db*, TEXT*, const TEXT*, USHORT);
SLONG		MET_lookup_index_name(thread_db*, const TEXT*, SLONG*, SSHORT*);
bool		MET_lookup_partner(thread_db*, jrd_rel*, struct idx*, const TEXT*);
jrd_prc*	MET_lookup_procedure(thread_db*, SCHAR *, bool);
jrd_prc*	MET_lookup_procedure_id(thread_db*, SSHORT, bool, bool, USHORT);
jrd_rel*	MET_lookup_relation(thread_db*, const char*);
jrd_rel*	MET_lookup_relation_id(thread_db*, SLONG, bool);
jrd_nod*	MET_parse_blob(thread_db*, jrd_rel*, struct bid*, Csb**,
								  jrd_req**, const bool, const bool);
void		MET_parse_sys_trigger(thread_db*, jrd_rel*);
int			MET_post_existence(thread_db*, jrd_rel*);
void		MET_prepare(thread_db*, jrd_tra*, USHORT, const UCHAR*);
jrd_prc*	MET_procedure(thread_db*, int, bool, USHORT);
jrd_rel*	MET_relation(thread_db*, USHORT);
bool		MET_relation_owns_trigger (thread_db*, const TEXT*, const TEXT*);
bool		MET_relation_default_class (thread_db*, const TEXT*, const TEXT*);
void		MET_release_existence(jrd_rel*);
void		MET_release_triggers(thread_db*, trig_vec**);
#ifdef DEV_BUILD
void		MET_verify_cache(thread_db*);
#endif
bool		MET_clear_cache(thread_db*, jrd_prc*);
bool		MET_procedure_in_use(thread_db*, jrd_prc*);
void		MET_remove_procedure(thread_db*, int, jrd_prc*);
void		MET_revoke(thread_db*, jrd_tra*, const TEXT*, const TEXT*, const TEXT*);
TEXT*		MET_save_name(thread_db*, const TEXT*);
void		MET_scan_relation(thread_db*, jrd_rel*);
const TEXT* MET_trigger_msg(thread_db*, const TEXT*, USHORT);
void		MET_update_shadow(thread_db*, class Shadow*, USHORT);
void		MET_update_transaction(thread_db*, jrd_tra*, const bool);
void		MET_update_partners(thread_db*);

#endif // JRD_MET_PROTO_H

