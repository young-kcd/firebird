/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		evl_proto.h
 *	DESCRIPTION:	Prototype header file for evl.cpp
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

#ifndef JRD_EVL_PROTO_H
#define JRD_EVL_PROTO_H

#include "../jrd/intl_classes.h"

// Implemented in evl.cpp
dsc*		EVL_assign_to(Jrd::thread_db* tdbb, Jrd::jrd_nod*);
Jrd::RecordBitmap*	EVL_bitmap(Jrd::thread_db* tdbb, Jrd::jrd_nod*);
bool		EVL_boolean(Jrd::thread_db* tdbb, Jrd::jrd_nod*);
dsc*		EVL_expr(Jrd::thread_db* tdbb, Jrd::jrd_nod*);
bool		EVL_field(Jrd::jrd_rel*, Jrd::Record*, USHORT, dsc*);
USHORT		EVL_group(Jrd::thread_db* tdbb, Jrd::RecordSource*, Jrd::jrd_nod* const, USHORT);
void		EVL_make_value(Jrd::thread_db* tdbb, const dsc*, Jrd::impure_value*);

// Obsolete functions to support MATCHES and SLEUTH
bool		EVL_mb_matches(Jrd::thread_db* tdbb, Jrd::TextType, const UCHAR*, SSHORT, const UCHAR*, SSHORT);
bool		EVL_mb_sleuth_check(Jrd::thread_db* tdbb, Jrd::TextType, USHORT, const UCHAR*, USHORT,
								const UCHAR*,USHORT);
USHORT		EVL_mb_sleuth_merge(Jrd::thread_db* tdbb, Jrd::TextType, const UCHAR*, USHORT, const UCHAR*,
								USHORT, UCHAR*, USHORT);
bool		EVL_nc_matches(Jrd::thread_db* tdbb, Jrd::TextType, const UCHAR*, SSHORT, const UCHAR*, SSHORT);
bool		EVL_nc_sleuth_check(Jrd::thread_db* tdbb, Jrd::TextType, USHORT, const UCHAR*, USHORT,
								const UCHAR*, USHORT);
USHORT		EVL_nc_sleuth_merge(Jrd::thread_db* tdbb, Jrd::TextType, const UCHAR*, USHORT, const UCHAR*,
								USHORT, UCHAR*, USHORT);
bool		EVL_wc_matches(Jrd::thread_db* tdbb, Jrd::TextType, const USHORT*, SSHORT, const USHORT*, SSHORT);
bool		EVL_wc_sleuth_check(Jrd::thread_db* tdbb, Jrd::TextType, USHORT, const USHORT*, USHORT,
								const USHORT*, USHORT);
USHORT		EVL_wc_sleuth_merge(Jrd::thread_db* tdbb, Jrd::TextType, const USHORT*, USHORT, const USHORT*,
								USHORT, USHORT*, USHORT);

#endif // JRD_EVL_PROTO_H

