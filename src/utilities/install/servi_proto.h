/*
 *	PROGRAM:	Windows NT installation utilities
 *	MODULE:		servi_proto.h
 *	DESCRIPTION:	Prototypes for NT service functions
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

#ifndef UTILITIES_SERVI_PROTO_H
#define UTILITIES_SERVI_PROTO_H

USHORT	SERVICES_install(SC_HANDLE, TEXT*, TEXT*, TEXT*, TEXT*, TEXT*, USHORT,
						TEXT*, TEXT*, USHORT (*)(SLONG, TEXT *, SC_HANDLE));

USHORT	SERVICES_remove(SC_HANDLE, TEXT*, TEXT*, USHORT (*)(SLONG, TEXT *, SC_HANDLE));

USHORT	SERVICES_start(SC_HANDLE, TEXT*, TEXT*, USHORT, USHORT (*)(SLONG, TEXT *, SC_HANDLE));

USHORT	SERVICES_stop(SC_HANDLE, TEXT*, TEXT*, USHORT (*)(SLONG, TEXT *, SC_HANDLE));

USHORT	SERVICES_grant_logon_right(TEXT*, USHORT (*)(SLONG, TEXT *, SC_HANDLE));

USHORT	SERVICES_grant_access_rights(TEXT*, TEXT*, USHORT (*)(SLONG, TEXT *, SC_HANDLE));

#endif // UTILITIES_SERVI_PROTO_H
