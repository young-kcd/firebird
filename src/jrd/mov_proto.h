/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		mov_proto.h
 *	DESCRIPTION:	Prototype header file for mov.c
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

#ifndef _JRD_MOV_PROTO_H_
#define _JRD_MOV_PROTO_H_

#include "../jrd/dsc.h"
#include "../jrd/jrd.h"
#include "../jrd/val.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int MOV_compare(struct dsc *, struct dsc *);
extern double MOV_date_to_double(struct dsc *);
extern void MOV_double_to_date(double, SLONG[2]);
extern void MOV_fast(SCHAR *, SCHAR *, ULONG);
extern void MOV_faster(SLONG *, SLONG *, ULONG);
extern void MOV_fill(SLONG *, ULONG);
extern double MOV_get_double(struct dsc *);
extern SLONG MOV_get_long(struct dsc *, SSHORT);
extern void MOV_get_metadata_str(struct dsc *, TEXT *, USHORT);
extern void MOV_get_name(struct dsc *, TEXT *);
extern SQUAD MOV_get_quad(struct dsc *, SSHORT);
extern SINT64 MOV_get_int64(struct dsc *, SSHORT);
extern int MOV_get_string_ptr(struct dsc *, USHORT *, UCHAR **, struct vary *,
							  USHORT);
extern int MOV_get_string(struct dsc *, UCHAR **, struct vary *, USHORT);
extern GDS_DATE MOV_get_sql_date(struct dsc *);
extern GDS_TIME MOV_get_sql_time(struct dsc *);
extern GDS_TIMESTAMP MOV_get_timestamp(struct dsc *);
extern int MOV_make_string(struct dsc *, USHORT, const char**, struct vary *,
						   USHORT);
extern int MOV_make_string2(struct dsc *, USHORT, UCHAR **, struct vary *,
							USHORT, struct str **);
extern void MOV_move(struct dsc *, struct dsc *);
extern void MOV_time_stamp(GDS_TIMESTAMP *);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _JRD_MOV_PROTO_H_ */
