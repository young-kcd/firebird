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

#ifndef _UTILITIES_SERVI_PROTO_H_
#define _UTILITIES_SERVI_PROTO_H_

extern USHORT	SERVICES_install (SC_HANDLE manager,
				TEXT	*service_name,
				TEXT	*display_name,
				TEXT	*executable,
				TEXT	*directory,
				TEXT	*dependencies,
				USHORT	sw_startup,
				TEXT	*nt_user_name,
				TEXT	*nt_user_password,
				USHORT (*)(SLONG, TEXT *, SC_HANDLE));

extern USHORT	SERVICES_remove (SC_HANDLE manager,
				TEXT	*service_name,
				TEXT	*display_name,
				USHORT (*)(SLONG, TEXT *, SC_HANDLE));

extern USHORT	SERVICES_start (SC_HANDLE manager,
				TEXT	*service_name,
				TEXT	*display_name,
				USHORT	sw_mode,
				USHORT (*)(SLONG, TEXT *, SC_HANDLE));

extern USHORT	SERVICES_stop (SC_HANDLE manager,
				TEXT	*service_name,
				TEXT	*display_name,
				USHORT (*)(SLONG, TEXT *, SC_HANDLE));

extern USHORT	SERVICES_grant_logon_right(TEXT* account,
				USHORT (*)(SLONG, TEXT *, SC_HANDLE));

extern USHORT	SERVICES_grant_access_rights(TEXT* service_name,
				TEXT* account,
				USHORT (*)(SLONG, TEXT *, SC_HANDLE));

#endif /* _UTILITIES_SERVI_PROTO_H_ */
