/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		cvt_proto.h
 *	DESCRIPTION:	Prototype header file for cvt.c
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

#ifndef JRD_CVT_PROTO_H
#define JRD_CVT_PROTO_H

double		CVT_date_to_double(struct dsc *, FPTR_VOID);
void		CVT_double_to_date(double, SLONG[2], FPTR_VOID);
double		CVT_get_double(struct dsc *, FPTR_VOID);
SLONG		CVT_get_long(struct dsc *, SSHORT, FPTR_VOID);
SINT64		CVT_get_int64(struct dsc *, SSHORT, FPTR_VOID);
UCHAR		CVT_get_numeric(UCHAR *, USHORT, SSHORT *, double *, FPTR_VOID);
SQUAD		CVT_get_quad(struct dsc *, SSHORT, FPTR_VOID);
USHORT		CVT_get_string_ptr(struct dsc *, USHORT *, UCHAR **,
								 struct vary *, USHORT, FPTR_VOID);
GDS_DATE	CVT_get_sql_date(struct dsc *, FPTR_VOID);
GDS_TIME	CVT_get_sql_time(struct dsc *, FPTR_VOID);
GDS_TIMESTAMP CVT_get_timestamp(struct dsc *, FPTR_VOID);
USHORT		CVT_make_string(struct dsc*, USHORT, const char**, struct vary *,
							  USHORT, FPTR_VOID);
extern "C" void CVT_move(struct dsc *, struct dsc *, FPTR_VOID);

#endif // JRD_CVT_PROTO_H
