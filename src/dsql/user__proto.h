/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		user__proto.h
 *	DESCRIPTION:	Prototype Header file for user_dsql.c
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

#ifndef DSQL_USER__PROTO_H
#define DSQL_USER__PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

ISC_STATUS API_ROUTINE gds__close(ISC_STATUS*, SCHAR*);
ISC_STATUS API_ROUTINE gds__declare(ISC_STATUS*, SCHAR*, SCHAR*);
ISC_STATUS API_ROUTINE gds__describe(ISC_STATUS*, SCHAR*, SQLDA*);
ISC_STATUS API_ROUTINE gds__describe_bind(ISC_STATUS*, SCHAR*, SQLDA*);
ISC_STATUS API_ROUTINE gds__dsql_finish(FRBRD**);
ISC_STATUS API_ROUTINE gds__execute(ISC_STATUS*, FRBRD**, SCHAR*, SQLDA*);
ISC_STATUS API_ROUTINE gds__execute_immediate(ISC_STATUS*, FRBRD**, FRBRD**,
											 SSHORT*, SCHAR*);
ISC_STATUS API_ROUTINE gds__fetch(ISC_STATUS*, SCHAR*, SQLDA*);
ISC_STATUS API_ROUTINE gds__fetch_a(ISC_STATUS*, int*, SCHAR*, SQLDA*);
ISC_STATUS API_ROUTINE gds__open(ISC_STATUS*, FRBRD**, SCHAR*, SQLDA*);
ISC_STATUS API_ROUTINE gds__prepare(ISC_STATUS*, FRBRD**, FRBRD**, SCHAR*,
								   SSHORT*, SCHAR*, SQLDA*);
ISC_STATUS API_ROUTINE gds__to_sqlda(SQLDA*, int, SCHAR*, int, SCHAR*);
ISC_STATUS API_ROUTINE isc_close(ISC_STATUS*, SCHAR*);
ISC_STATUS API_ROUTINE isc_declare(ISC_STATUS*, SCHAR*, SCHAR*);
ISC_STATUS API_ROUTINE isc_describe(ISC_STATUS*, SCHAR*, SQLDA*);
ISC_STATUS API_ROUTINE isc_describe_bind(ISC_STATUS*, SCHAR*, SQLDA*);
ISC_STATUS API_ROUTINE isc_dsql_finish(FRBRD**);
ISC_STATUS API_ROUTINE isc_dsql_release(ISC_STATUS*, SCHAR*);
ISC_STATUS API_ROUTINE isc_dsql_fetch_a(ISC_STATUS*, int*, int*, USHORT, int*);
#ifdef SCROLLABLE_CURSORS
ISC_STATUS API_ROUTINE isc_dsql_fetch2_a(ISC_STATUS*, int*, int*, USHORT, int*, USHORT, SLONG);
#endif

ISC_STATUS API_ROUTINE isc_embed_dsql_close(ISC_STATUS*, SCHAR*);
ISC_STATUS API_ROUTINE isc_embed_dsql_declare(ISC_STATUS*, SCHAR*, SCHAR*);
ISC_STATUS API_ROUTINE isc_embed_dsql_descr_bind(ISC_STATUS*, SCHAR*, USHORT, XSQLDA*);
ISC_STATUS API_ROUTINE isc_embed_dsql_describe(ISC_STATUS*, SCHAR*, USHORT, XSQLDA*);
ISC_STATUS API_ROUTINE isc_embed_dsql_describe_bind(ISC_STATUS*, SCHAR*, USHORT, XSQLDA*);
ISC_STATUS API_ROUTINE isc_embed_dsql_exec_immed(ISC_STATUS*, FRBRD**, FRBRD**, USHORT, SCHAR*, USHORT, XSQLDA*);
ISC_STATUS API_ROUTINE isc_embed_dsql_exec_immed2(ISC_STATUS*, FRBRD**, FRBRD**, USHORT, SCHAR*, USHORT, XSQLDA*, XSQLDA*);
ISC_STATUS API_ROUTINE isc_embed_dsql_execute(ISC_STATUS*, FRBRD**, SCHAR*, USHORT, XSQLDA*);
ISC_STATUS API_ROUTINE isc_embed_dsql_execute2(ISC_STATUS*, FRBRD**, SCHAR*, USHORT, XSQLDA*, XSQLDA*);
ISC_STATUS API_ROUTINE isc_embed_dsql_execute_immed(ISC_STATUS*, FRBRD**, FRBRD**, USHORT, SCHAR*, USHORT, XSQLDA*);
ISC_STATUS API_ROUTINE isc_embed_dsql_fetch(ISC_STATUS*, SCHAR*, USHORT, XSQLDA*);

#ifdef SCROLLABLE_CURSORS
ISC_STATUS API_ROUTINE isc_embed_dsql_fetch2(ISC_STATUS*, SCHAR*, USHORT, XSQLDA*, USHORT, SLONG);
#endif

ISC_STATUS API_ROUTINE isc_embed_dsql_fetch_a(ISC_STATUS*, int*, SCHAR*, USHORT, XSQLDA*);

#ifdef SCROLLABLE_CURSORS
ISC_STATUS API_ROUTINE isc_embed_dsql_fetch2_a(ISC_STATUS*, int*, SCHAR*, USHORT, XSQLDA*, USHORT, SLONG);
#endif

ISC_STATUS API_ROUTINE isc_embed_dsql_insert(ISC_STATUS*, SCHAR*, USHORT, XSQLDA*);
void   API_ROUTINE isc_embed_dsql_length(UCHAR*, USHORT*);
ISC_STATUS API_ROUTINE isc_embed_dsql_open(ISC_STATUS*, FRBRD**, SCHAR*, USHORT, XSQLDA*);
ISC_STATUS API_ROUTINE isc_embed_dsql_open2(ISC_STATUS*, FRBRD**, SCHAR*, USHORT, XSQLDA*, XSQLDA*);
ISC_STATUS API_ROUTINE isc_embed_dsql_prepare(ISC_STATUS*, FRBRD**, FRBRD**, SCHAR*, USHORT, SCHAR*, USHORT, XSQLDA*);
ISC_STATUS API_ROUTINE isc_embed_dsql_release(ISC_STATUS*, SCHAR*);
ISC_STATUS API_ROUTINE isc_execute(ISC_STATUS*, FRBRD**, SCHAR*, SQLDA*);
ISC_STATUS API_ROUTINE isc_execute_immediate(ISC_STATUS*, FRBRD**, FRBRD**, SSHORT*, SCHAR*);
ISC_STATUS API_ROUTINE isc_fetch(ISC_STATUS*, SCHAR*, SQLDA*);
ISC_STATUS API_ROUTINE isc_fetch_a(ISC_STATUS*, int*, SCHAR*, SQLDA*);
ISC_STATUS API_ROUTINE isc_open(ISC_STATUS*, FRBRD**, SCHAR*, SQLDA*);
ISC_STATUS API_ROUTINE isc_prepare(ISC_STATUS*, FRBRD**, FRBRD**, SCHAR*, SSHORT*, SCHAR*, SQLDA*);
int    API_ROUTINE isc_to_sqlda(SQLDA*, int, SCHAR*, int, SCHAR*);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif /*  DSQL_USER__PROTO_H */
