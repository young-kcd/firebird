/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		isc_f_proto.h
 *	DESCRIPTION:	Prototype header file for isc_file.c
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

#ifdef __cplusplus
extern "C" {
#endif

int			ISC_analyze_nfs(TEXT *, TEXT *);
int			ISC_analyze_pclan(TEXT *, TEXT *);
int			ISC_analyze_tcp(TEXT *, TEXT *);
int			ISC_analyze_xnet(TEXT *, TEXT *);
BOOLEAN		ISC_check_if_remote(TEXT *, BOOLEAN);
int			ISC_expand_filename(TEXT *, USHORT, TEXT *);
int			ISC_expand_logical(TEXT *, USHORT, TEXT *);
int			ISC_expand_share(TEXT *, TEXT *);
int			ISC_file_lock(SSHORT);
int			ISC_file_unlock(SSHORT);
int			ISC_strip_filename(TEXT *);
bool		ISC_verify_database_access(TEXT *);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // JRD_ISC_FILE_PROTO_H
