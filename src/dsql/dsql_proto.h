/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		dsql_proto.h
 *	DESCRIPTION:	Prototype Header file for dsql.c
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

#ifndef _DSQL_DSQL_PROTO_H_
#define _DSQL_DSQL_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

extern STATUS DLL_EXPORT dsql8_allocate_statement(	STATUS*,
													struct why_hndl **,
													struct dsql_req **);
extern STATUS DLL_EXPORT dsql8_execute(STATUS *, why_hndl **, struct dsql_req**,
									   USHORT, SCHAR *, USHORT, USHORT,
									   SCHAR *, USHORT, SCHAR *, USHORT,
									   USHORT, SCHAR *);
extern STATUS DLL_EXPORT dsql8_execute_immediate(STATUS *, 
												 struct why_hndl **, 
												 struct why_hndl **,
												 USHORT, TEXT *, USHORT,
												 USHORT, SCHAR *, USHORT,
												 USHORT, SCHAR *, USHORT,
												 SCHAR *, USHORT, USHORT,
												 SCHAR *);
#ifdef SCROLLABLE_CURSORS
extern STATUS DLL_EXPORT dsql8_fetch(STATUS *, struct dsql_req**, USHORT, SCHAR *,
									 USHORT, USHORT, SCHAR *, USHORT, SLONG);
#else
extern STATUS DLL_EXPORT dsql8_fetch(STATUS *, struct dsql_req**, USHORT, SCHAR *,
									 USHORT, USHORT, SCHAR *);
#endif /* SCROLLABLE_CURSORS */
extern STATUS DLL_EXPORT dsql8_free_statement(STATUS *, struct dsql_req**,
											  USHORT);
extern STATUS DLL_EXPORT dsql8_insert(STATUS *, struct dsql_req**, USHORT,
									  SCHAR *, USHORT, USHORT, SCHAR *);
extern STATUS DLL_EXPORT dsql8_prepare(STATUS *, struct why_hndl **, struct dsql_req**,
									   USHORT, TEXT *, USHORT, USHORT,
									   CONST SCHAR *, USHORT, SCHAR *);
extern STATUS DLL_EXPORT dsql8_set_cursor(STATUS *, struct dsql_req**, TEXT *,
										  USHORT);
extern STATUS DLL_EXPORT dsql8_sql_info(STATUS *, struct dsql_req**, USHORT,
										CONST SCHAR*, USHORT, SCHAR *);
extern void DSQL_pretty(struct dsql_nod *, int);

extern STATUS callback_execute_immediate(
		STATUS *status,
		class att* jrd_attachment_handle,
		class jrd_tra* jrd_transaction_handle,
		TEXT *sql_operator,
		int len);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*  _DSQL_DSQL_PROTO_H_  */
