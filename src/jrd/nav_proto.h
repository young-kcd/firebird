/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		nav_proto.h
 *	DESCRIPTION:	Prototype header file for nav.c
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

#ifndef JRD_NAV_PROTO_H
#define JRD_NAV_PROTO_H

#include "../jrd/rse.h"

#ifdef SCROLLABLE_CURSORS
struct exp *NAV_expand_index(struct win *, struct irsb_nav *);
#endif
BOOLEAN NAV_get_record(TDBB tdbb, class Rsb *, struct irsb_nav *, struct rpb *,
							  enum rse_get_mode);

#ifdef PC_ENGINE
BOOLEAN NAV_find_record(class Rsb *, USHORT, USHORT, struct jrd_nod *);
void NAV_get_bookmark(class Rsb *, struct irsb_nav *, struct bkm *);
BOOLEAN NAV_reset_position(class Rsb *, struct rpb *);
BOOLEAN NAV_set_bookmark(class Rsb *, struct irsb_nav *, struct rpb *,
								struct bkm *);
#endif

#endif // JRD_NAV_PROTO_H
