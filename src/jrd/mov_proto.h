/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		mov_proto.h
 *	DESCRIPTION:	Prototype header file for mov.cpp
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

#ifndef JRD_MOV_PROTO_H
#define JRD_MOV_PROTO_H

#include "../jrd/dsc.h"
#include "../jrd/jrd.h"
#include "../jrd/val.h"

#ifdef __cplusplus
extern "C" {
#endif

int		MOV_compare(const struct dsc*, const struct dsc*);
double	MOV_date_to_double(const struct dsc*);
void	MOV_double_to_date(double, SLONG[2]);
void	MOV_fast(const SCHAR*, SCHAR*, ULONG);
void	MOV_faster(const SLONG*, SLONG*, ULONG);
void	MOV_fill(SLONG*, ULONG);
double	MOV_get_double(const struct dsc*);
SLONG	MOV_get_long(const struct dsc*, SSHORT);
void	MOV_get_metadata_str(const struct dsc*, TEXT*, USHORT);
void	MOV_get_name(const struct dsc*, TEXT*);
SQUAD	MOV_get_quad(const struct dsc*, SSHORT);
SINT64	MOV_get_int64(const struct dsc*, SSHORT);
int		MOV_get_string_ptr(const struct dsc*, USHORT*, UCHAR**, struct vary*,
							  USHORT);
int		MOV_get_string(const struct dsc*, UCHAR**, struct vary*, USHORT);
GDS_DATE	MOV_get_sql_date(const struct dsc*);
GDS_TIME	MOV_get_sql_time(const struct dsc*);
GDS_TIMESTAMP	MOV_get_timestamp(const struct dsc*);
int		MOV_make_string(const struct dsc*, USHORT, const char**, struct vary*,
						   USHORT);
int		MOV_make_string2(const struct dsc*, USHORT, UCHAR**, struct vary*,
							USHORT, struct str**);
void	MOV_move(const struct dsc*, struct dsc*);
void	MOV_time_stamp(GDS_TIMESTAMP*);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JRD_MOV_PROTO_H */

