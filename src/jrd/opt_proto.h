/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		opt_proto.h
 *	DESCRIPTION:	Prototype header file for opt.c
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

#ifndef JRD_OPT_PROTO_H
#define JRD_OPT_PROTO_H

#include "../jrd/jrd.h"
#include "../jrd/btr.h"
#include "../jrd/rse.h"
#include "../jrd/lls.h"


BOOLEAN OPT_access_path(struct jrd_req *, SCHAR *, SSHORT, USHORT *);
class Rsb *OPT_compile(TDBB, class Csb *,
							   struct rse *, struct lls *);
struct jrd_nod *OPT_make_dbkey(struct opt *, struct jrd_nod *,
								  USHORT);
struct jrd_nod *OPT_make_index(TDBB, struct opt *, struct jrd_rel *,
								  struct idx *);
int OPT_match_index(struct opt *, USHORT, struct idx *);
void OPT_set_index(TDBB, struct jrd_req *, class Rsb **, struct jrd_rel *,
						  struct idx *);

#endif // JRD_OPT_PROTO_H
