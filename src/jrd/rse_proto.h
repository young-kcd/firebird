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

#ifndef _JRD_RSE_PROTO_H_
#define _JRD_RSE_PROTO_H_

#include "../jrd/jrd.h"
#include "../jrd/rse.h"

extern void RSE_close(TDBB, register struct rsb *);
#ifdef PC_ENGINE
extern BOOLEAN RSE_find_dbkey(TDBB, struct rsb *, struct nod *, struct nod *);
extern BOOLEAN RSE_find_record(TDBB, struct rsb *, USHORT, USHORT,
							   struct nod *);
#endif
extern BOOLEAN RSE_get_record(TDBB, struct rsb *, enum rse_get_mode);
#ifdef PC_ENGINE
extern struct bkm *RSE_get_bookmark(TDBB, struct rsb *);
extern void RSE_mark_crack(TDBB, struct rsb *, USHORT);
#endif
extern void RSE_open(TDBB, register struct rsb *);
#ifdef PC_ENGINE
extern BOOLEAN RSE_reset_position(TDBB, struct rsb *, struct rpb *);
extern BOOLEAN RSE_set_bookmark(TDBB, struct rsb *, struct rpb *,
								struct bkm *);
#endif

#ifdef PC_ENGINE
#define RSE_MARK_CRACK(t, var1, var2)	RSE_mark_crack (t, var1, var2)
#else
#define RSE_MARK_CRACK(t, var1, var2)
#endif

#endif /* _JRD_RSE_PROTO_H_ */
