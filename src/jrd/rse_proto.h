/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		rse_proto.h
 *	DESCRIPTION:	Prototype header file for rse.c
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

#ifndef JRD_RSE_PROTO_H
#define JRD_RSE_PROTO_H

#include "../jrd/jrd.h"
#include "../jrd/rse.h"

void RSE_close(TDBB, class Rsb *);
#ifdef PC_ENGINE
BOOLEAN RSE_find_dbkey(TDBB, class Rsb *, struct jrd_nod *, struct jrd_nod *);
BOOLEAN RSE_find_record(TDBB, class Rsb *, USHORT, USHORT,
							   struct jrd_nod *);
#endif
BOOLEAN RSE_get_record(TDBB, class Rsb *, enum rse_get_mode);
#ifdef PC_ENGINE
struct bkm *RSE_get_bookmark(TDBB, class Rsb *);
void RSE_mark_crack(TDBB, class Rsb *, USHORT);
#endif
void RSE_open(TDBB, class Rsb *);
#ifdef PC_ENGINE
BOOLEAN RSE_reset_position(TDBB, class Rsb *, struct rpb *);
BOOLEAN RSE_set_bookmark(TDBB, class Rsb *, struct rpb *,
								struct bkm *);
#endif

#ifdef PC_ENGINE
#define RSE_MARK_CRACK(t, var1, var2)	RSE_mark_crack (t, var1, var2)
#else
#define RSE_MARK_CRACK(t, var1, var2)
#endif

#endif // JRD_RSE_PROTO_H
