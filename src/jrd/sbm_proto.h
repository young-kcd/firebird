/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		sbm_proto.h
 *	DESCRIPTION:	Prototype header file for sbm.c
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

#ifndef JRD_SBM_PROTO_H
#define JRD_SBM_PROTO_H

#include "../jrd/sbm.h"
#include "../jrd/rse.h"

#ifdef DEV_BUILD
#include "../jrd/ib_stdio.h"
void SBM_dump(IB_FILE *, SBM);
#endif

struct sbm**	SBM_and(struct sbm **, struct sbm **);
int			SBM_clear(struct sbm *, SLONG);
BOOLEAN		SBM_equal(SBM, SBM);
void		SBM_init(void);
int			SBM_next(struct sbm *, SLONG *, enum rse_get_mode);
struct sbm**	SBM_or(struct sbm **, struct sbm **);
void		SBM_release(struct sbm *);
void		SBM_reset(struct sbm **);
void		SBM_set(TDBB, struct sbm **, SLONG);
int			SBM_test(struct sbm *, SLONG);
SLONG		SBM_size(struct sbm **);


#endif // JRD_SBM_PROTO_H

