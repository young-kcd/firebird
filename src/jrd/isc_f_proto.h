/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		isc_f_proto.h
 *	DESCRIPTION:	Prototype header file for isc_file.cpp
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
 * 2002.10.29 Sean Leyne - Removed support for obsolete IPX/SPX Protocol
 *
 */

#ifndef JRD_ISC_FILE_PROTO_H
#define JRD_ISC_FILE_PROTO_H

#include "fb_string.h"

#ifndef NO_NFS
bool		ISC_analyze_nfs(TEXT*, TEXT*);
#endif
int			ISC_analyze_pclan(TEXT*, TEXT*);
int			ISC_analyze_tcp(TEXT*, TEXT*);
int			ISC_analyze_xnet(TEXT*, TEXT*);
bool		ISC_check_if_remote(const Firebird::PathName&, bool);
int			ISC_expand_filename(const Firebird::PathName&, Firebird::PathName&);
// Temporary hack to make old files happy
inline int	ISC_expand_filename(const TEXT* unexp, USHORT len, TEXT* exp, USHORT bufsize)
{
	Firebird::PathName pn(unexp, len ? len : strlen(unexp));
	Firebird::PathName result;
	const int rc = ISC_expand_filename(pn, result);
	result.copy_to(exp, bufsize);
	return rc;
}
int			ISC_expand_logical(const TEXT*, USHORT, TEXT*, USHORT);
int			ISC_expand_share(const TEXT*, TEXT*, USHORT);
int			ISC_file_lock(SSHORT);
int			ISC_file_unlock(SSHORT);
bool		ISC_verify_database_access(const Firebird::PathName&);

#endif // JRD_ISC_FILE_PROTO_H

