/*
 *	PROGRAM:	JRD Write Ahead LOG APIs
 *	MODULE:		wstat_proto.h
 *	DESCRIPTION:	Prototype header file for wstatus.c
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
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#ifndef WAL_WSTAT_PROTO_H
#define WAL_WSTAT_PROTO_H

#include "../jrd/ib_stdio.h"
void	WSTATUS_dump_wal_segment (WAL, int, IB_FILE *);

#endif	//  WAL_WSTAT_PROTO_H

