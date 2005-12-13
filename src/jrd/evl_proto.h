/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		evl_proto.h
 *	DESCRIPTION:	Prototype header file for evl.c
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

#ifndef _JRD_EVL_PROTO_H_
#define _JRD_EVL_PROTO_H_

#include "../jrd/intl_classes.h"

extern struct dsc *DLL_EXPORT EVL_assign_to(TDBB tdbb, struct jrd_nod *);

extern struct sbm **DLL_EXPORT EVL_bitmap(TDBB tdbb, struct jrd_nod *);

extern BOOLEAN DLL_EXPORT EVL_boolean(TDBB tdbb, struct jrd_nod *);

extern struct dsc *DLL_EXPORT EVL_expr(TDBB tdbb, struct jrd_nod *);

extern BOOLEAN DLL_EXPORT EVL_field(struct jrd_rel *,
									struct rec *,
									USHORT, DSC *);

extern USHORT DLL_EXPORT EVL_group
	(TDBB tdbb, struct blk *, struct jrd_nod *, USHORT);

extern USHORT DLL_EXPORT EVL_mb_contains
	(TDBB tdbb, TextType*, UCHAR *, USHORT, UCHAR *, USHORT);

extern USHORT DLL_EXPORT EVL_mb_like
	(TDBB tdbb, TextType*, UCHAR *, SSHORT, UCHAR *, SSHORT, USHORT);

extern USHORT DLL_EXPORT EVL_mb_matches
	(TDBB tdbb, TextType*, UCHAR *, SSHORT, UCHAR *, SSHORT);

extern USHORT DLL_EXPORT EVL_mb_sleuth_check
	(TDBB tdbb, TextType*, USHORT, UCHAR *, USHORT, UCHAR *, USHORT);

extern USHORT DLL_EXPORT EVL_mb_sleuth_merge
	(TDBB tdbb, TextType*, UCHAR *, USHORT, UCHAR *, USHORT, UCHAR *, USHORT);

extern void DLL_EXPORT EVL_make_value(TDBB tdbb, struct dsc *, struct vlu *);

extern USHORT DLL_EXPORT EVL_nc_contains
	(TDBB tdbb, TextType*, UCHAR *, USHORT, UCHAR *, USHORT);

extern USHORT EVL_nc_like
	(TDBB tdbb, TextType*, UCHAR *, SSHORT, UCHAR *, SSHORT, USHORT);

extern USHORT EVL_nc_matches
	(TDBB tdbb, TextType*, UCHAR *, SSHORT, UCHAR *, SSHORT);

extern USHORT EVL_nc_sleuth_check
	(TDBB tdbb, TextType*, USHORT, UCHAR *, USHORT, UCHAR *, USHORT);

extern USHORT EVL_nc_sleuth_merge
	(TDBB tdbb, TextType*, UCHAR *, USHORT, UCHAR *, USHORT, UCHAR *, USHORT);

extern USHORT DLL_EXPORT EVL_wc_contains
	(TDBB tdbb, TextType*, USHORT *, USHORT, USHORT *, USHORT);

extern USHORT EVL_wc_like
	(TDBB tdbb, TextType*, USHORT *, SSHORT, USHORT *, SSHORT, USHORT);

extern USHORT EVL_wc_matches
	(TDBB tdbb, TextType*, USHORT *, SSHORT, USHORT *, SSHORT);

extern USHORT EVL_wc_sleuth_check
	(TDBB tdbb, TextType*, USHORT, USHORT *, USHORT, USHORT *, USHORT);

extern USHORT EVL_wc_sleuth_merge
	(TDBB tdbb,
	 TextType*, USHORT *, USHORT, USHORT *, USHORT, USHORT *, USHORT);

#endif /* _JRD_EVL_PROTO_H_ */
