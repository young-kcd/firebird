/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		sbm_proto.h
 *	DESCRIPTION:	Prototype header file for sbm.cpp
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
#include <stdio.h>
void SBM_dump(FILE*, Jrd::SparseBitmap*);
#endif

Jrd::SparseBitmap**	SBM_and(Jrd::SparseBitmap**, Jrd::SparseBitmap**);
bool		SBM_clear(Jrd::SparseBitmap*, SLONG);
bool		SBM_equal(Jrd::SparseBitmap*, Jrd::SparseBitmap*);
void		SBM_init(void);
bool		SBM_next(Jrd::SparseBitmap*, SLONG*, Jrd::rse_get_mode);
Jrd::SparseBitmap**	SBM_or(Jrd::SparseBitmap**, Jrd::SparseBitmap**);
void		SBM_release(Jrd::SparseBitmap*);
void		SBM_reset(Jrd::SparseBitmap**);
void		SBM_set(Jrd::thread_db*, Jrd::SparseBitmap**, SLONG);
bool		SBM_test(const Jrd::SparseBitmap*, SLONG);
SLONG		SBM_size(const Jrd::SparseBitmap* const* const);

#endif // JRD_SBM_PROTO_H

