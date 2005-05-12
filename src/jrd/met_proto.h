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
#include "../common/classes/MetaName.h"

struct dsc;

namespace Jrd {
	class jrd_tra;
	class jrd_req;
	class jrd_prc;
	class Format;
	class jrd_rel;
	class CompilerScratch;
	class jrd_nod;
	class Database;
	struct bid;
	struct index_desc;
}

void		MET_activate_shadow(Jrd::thread_db*);
ULONG		MET_align(Jrd::Database* dbb, const dsc*, ULONG);
void		MET_change_fields(Jrd::thread_db*, Jrd::jrd_tra*, const dsc*);
Jrd::Format*	MET_current(Jrd::thread_db*, Jrd::jrd_rel*);
void		MET_delete_dependencies(Jrd::thread_db*, const Firebird::MetaName&, USHORT);
void		MET_delete_shadow(Jrd::thread_db*, USHORT);
void		MET_error(const TEXT*, ...);
Jrd::Format*	MET_format(Jrd::thread_db*, Jrd::jrd_rel*, USHORT);
bool		MET_get_char_subtype(Jrd::thread_db*, SSHORT*, const UCHAR*, USHORT);
Jrd::jrd_nod*	MET_get_dependencies(Jrd::thread_db*, Jrd::jrd_rel*, const TEXT*,
								Jrd::CompilerScratch*, Jrd::bid*, Jrd::jrd_req**,
								Jrd::CompilerScratch**, Firebird::MetaName&, USHORT, USHORT);
class Jrd::jrd_fld*	MET_get_field(Jrd::jrd_rel*, USHORT);
void		MET_get_shadow_files(Jrd::thread_db*, bool);
void		MET_load_trigger(Jrd::thread_db*, Jrd::jrd_rel*, const Firebird::MetaName&, Jrd::trig_vec**);
void		MET_lookup_cnstrt_for_index(Jrd::thread_db*, Firebird::MetaName& constraint, const Firebird::MetaName& index_name);
void		MET_lookup_cnstrt_for_trigger(Jrd::thread_db*, Firebird::MetaName&, Firebird::MetaName&, const Firebird::MetaName&);
void		MET_lookup_exception(Jrd::thread_db*, SLONG, /* OUT */ Firebird::MetaName&, /* OUT */ TEXT*, size_t);
SLONG		MET_lookup_exception_number(Jrd::thread_db*, const Firebird::MetaName&);
int			MET_lookup_field(Jrd::thread_db*, Jrd::jrd_rel*, const Firebird::MetaName&, const Firebird::MetaName*);
Jrd::BlobFilter*	MET_lookup_filter(Jrd::thread_db*, SSHORT, SSHORT);
SLONG		MET_lookup_generator(Jrd::thread_db*, const TEXT*);
void		MET_lookup_generator_id(Jrd::thread_db*, SLONG, Firebird::MetaName&);
void		MET_lookup_index(Jrd::thread_db*, Firebird::MetaName&, const Firebird::MetaName&, USHORT);
SLONG		MET_lookup_index_name(Jrd::thread_db*, const Firebird::MetaName&, SLONG*, SSHORT*);
bool		MET_lookup_partner(Jrd::thread_db*, Jrd::jrd_rel*, struct Jrd::index_desc*, const TEXT*);
Jrd::jrd_prc*	MET_lookup_procedure(Jrd::thread_db*, const Firebird::MetaName&, bool);
Jrd::jrd_prc*	MET_lookup_procedure_id(Jrd::thread_db*, SSHORT, bool, bool, USHORT);
Jrd::jrd_rel*	MET_lookup_relation(Jrd::thread_db*, const Firebird::MetaName&);
Jrd::jrd_rel*	MET_lookup_relation_id(Jrd::thread_db*, SLONG, bool);
Jrd::jrd_nod*	MET_parse_blob(Jrd::thread_db*, Jrd::jrd_rel*, Jrd::bid*, Jrd::CompilerScratch**,
								  Jrd::jrd_req**, bool);
void		MET_parse_sys_trigger(Jrd::thread_db*, Jrd::jrd_rel*);
bool		MET_post_existence(Jrd::thread_db*, Jrd::jrd_rel*);
void		MET_prepare(Jrd::thread_db*, Jrd::jrd_tra*, USHORT, const UCHAR*);
Jrd::jrd_prc*	MET_procedure(Jrd::thread_db*, int, bool, USHORT);
Jrd::jrd_rel*	MET_relation(Jrd::thread_db*, USHORT);
bool		MET_relation_default_class (Jrd::thread_db*, const Firebird::MetaName&, const Firebird::MetaName&);
void		MET_release_existence(Jrd::jrd_rel*);
void		MET_release_triggers(Jrd::thread_db*, Jrd::trig_vec**);
#ifdef DEV_BUILD
void		MET_verify_cache(Jrd::thread_db*);
#endif
void		MET_clear_cache(Jrd::thread_db*);
bool		MET_procedure_in_use(Jrd::thread_db*, Jrd::jrd_prc*);
void		MET_remove_procedure(Jrd::thread_db*, int, Jrd::jrd_prc*);
void		MET_revoke(Jrd::thread_db*, Jrd::jrd_tra*, const TEXT*, const TEXT*, const TEXT*);
void		MET_scan_relation(Jrd::thread_db*, Jrd::jrd_rel*);
const TEXT* MET_trigger_msg(Jrd::thread_db*, const Firebird::MetaName&, USHORT);
void		MET_update_shadow(Jrd::thread_db*, class Jrd::Shadow*, USHORT);
void		MET_update_transaction(Jrd::thread_db*, Jrd::jrd_tra*, const bool);
void		MET_update_partners(Jrd::thread_db*);

#endif // JRD_MET_PROTO_H

