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
													int**,
													struct req**);
extern STATUS DLL_EXPORT dsql8_execute(STATUS *, void **, struct req**,
									   USHORT, UCHAR *, USHORT, USHORT,
									   UCHAR *, USHORT, UCHAR *, USHORT,
									   USHORT, UCHAR *);
extern STATUS DLL_EXPORT dsql8_execute_immediate(STATUS *, int **, int **,
												 USHORT, TEXT *, USHORT,
												 USHORT, UCHAR *, USHORT,
												 USHORT, UCHAR *, USHORT,
												 UCHAR *, USHORT, USHORT,
												 UCHAR *);
#ifdef SCROLLABLE_CURSORS
extern STATUS DLL_EXPORT dsql8_fetch(STATUS *, struct req **, USHORT, UCHAR *,
									 USHORT, USHORT, UCHAR *, USHORT, SLONG);
#else
extern STATUS DLL_EXPORT dsql8_fetch(STATUS *, struct req **, USHORT, UCHAR *,
									 USHORT, USHORT, UCHAR *);
#endif /* SCROLLABLE_CURSORS */
extern STATUS DLL_EXPORT dsql8_free_statement(STATUS *, struct req **,
											  USHORT);
extern STATUS DLL_EXPORT dsql8_insert(STATUS *, struct req **, USHORT,
									  UCHAR *, USHORT, USHORT, UCHAR *);
extern STATUS DLL_EXPORT dsql8_prepare(STATUS *, void **, struct req **,
									   USHORT, TEXT *, USHORT, USHORT,
									   UCHAR *, USHORT, UCHAR *);
extern STATUS DLL_EXPORT dsql8_set_cursor(STATUS *, struct req **, TEXT *,
										  USHORT);
extern STATUS DLL_EXPORT dsql8_sql_info(STATUS *, struct req **, USHORT,
										SCHAR *, USHORT, SCHAR *);
extern void DSQL_pretty(struct nod *, int);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*  _DSQL_DSQL_PROTO_H_  */
