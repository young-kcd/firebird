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

#ifndef _JRD_OPT_PROTO_H_
#define _JRD_OPT_PROTO_H_

#include "../jrd/jrd.h"
#include "../jrd/btr.h"
#include "../jrd/rse.h"
#include "../jrd/lls.h"


extern BOOLEAN OPT_access_path(struct req *, SCHAR *, SSHORT, USHORT *);
extern struct rsb *OPT_compile(TDBB, register struct csb *,
							   register struct rse *, struct lls *);
extern struct nod *OPT_make_dbkey(register struct opt *, struct nod *,
								  USHORT);
extern struct nod *OPT_make_index(TDBB, struct opt *, struct rel *,
								  struct idx *);
extern int OPT_match_index(struct opt *, USHORT, struct idx *);
extern void OPT_set_index(TDBB, struct req *, struct rsb **, struct rel *,
						  struct idx *);

#endif /* _JRD_OPT_PROTO_H_ */
