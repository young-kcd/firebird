/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		map_proto.h
 *	DESCRIPTION:	Prototype header file for map.c
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

#ifndef JRD_MAP_PROTO_H
#define JRD_MAP_PROTO_H

void	MAP_date_to_gds(SLONG *, SLONG *);
void	MAP_date_to_rdb(SLONG *, SLONG *);
int		MAP_gds_to_rdb(USHORT, struct map_msg *, UCHAR *, UCHAR *);
struct map*	MAP_parse_blr(UCHAR *, USHORT, UCHAR *, USHORT *, SLONG *);
int		MAP_rdb_length(USHORT, struct map_msg *);
int		MAP_rdb_to_gds(USHORT, struct map_msg *, UCHAR *, UCHAR *);
void	MAP_release(struct map_msg *);
int		MAP_status_to_gds(ISC_STATUS *, ISC_STATUS *);
int		MAP_status_to_rdb(ISC_STATUS *, ISC_STATUS *);

#endif // JRD_MAP_PROTO_H
