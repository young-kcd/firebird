/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		utld_proto.h
 *	DESCRIPTION:	Prototype Header file for utld.c
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

#ifndef _DSQL_UTLD_PROTO_H_
#define _DSQL_UTLD_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

extern STATUS DLL_EXPORT UTLD_parse_sql_info(STATUS *, USHORT, SCHAR *,
											 XSQLDA *, USHORT *);
extern STATUS DLL_EXPORT UTLD_parse_sqlda(STATUS *, struct dasup *, USHORT *,
										  USHORT *, USHORT *, USHORT,
										  XSQLDA *, USHORT);
extern void DLL_EXPORT UTLD_save_status_strings(STATUS *);

#ifdef  WINDOWS_ONLY
extern void UTLD_wep(void);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*  _DSQL_UTLD_PROTO_H_  */
