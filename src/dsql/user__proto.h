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

STATUS API_ROUTINE gds__close(STATUS*, SCHAR*);
STATUS API_ROUTINE gds__declare(STATUS*, SCHAR*, SCHAR*);
STATUS API_ROUTINE gds__describe(STATUS*, SCHAR*, SQLDA*);
STATUS API_ROUTINE gds__describe_bind(STATUS*, SCHAR*, SQLDA*);
STATUS API_ROUTINE gds__dsql_finish(FRBRD**);
STATUS API_ROUTINE gds__execute(STATUS*, FRBRD**, SCHAR*, SQLDA*);
STATUS API_ROUTINE gds__execute_immediate(STATUS*, FRBRD**, FRBRD**,
											 SSHORT*, SCHAR*);
STATUS API_ROUTINE gds__fetch(STATUS*, SCHAR*, SQLDA*);
STATUS API_ROUTINE gds__fetch_a(STATUS*, int*, SCHAR*, SQLDA*);
STATUS API_ROUTINE gds__open(STATUS*, FRBRD**, SCHAR*, SQLDA*);
STATUS API_ROUTINE gds__prepare(STATUS*, FRBRD**, FRBRD**, SCHAR*,
								   SSHORT*, SCHAR*, SQLDA*);
STATUS API_ROUTINE gds__to_sqlda(SQLDA*, int, SCHAR*, int, SCHAR*);
STATUS API_ROUTINE isc_close(STATUS*, SCHAR*);
STATUS API_ROUTINE isc_declare(STATUS*, SCHAR*, SCHAR*);
STATUS API_ROUTINE isc_describe(STATUS*, SCHAR*, SQLDA*);
STATUS API_ROUTINE isc_describe_bind(STATUS*, SCHAR*, SQLDA*);
STATUS API_ROUTINE isc_dsql_finish(FRBRD**);
STATUS API_ROUTINE isc_dsql_release(STATUS*, SCHAR*);
STATUS API_ROUTINE isc_dsql_fetch_a(STATUS*, int*, int*, USHORT, int*);
#ifdef SCROLLABLE_CURSORS
STATUS API_ROUTINE isc_dsql_fetch2_a(STATUS*, int*, int*, USHORT, int*, USHORT, SLONG);
#endif

STATUS API_ROUTINE isc_embed_dsql_close(STATUS*, SCHAR*);
STATUS API_ROUTINE isc_embed_dsql_declare(STATUS*, SCHAR*, SCHAR*);
STATUS API_ROUTINE isc_embed_dsql_descr_bind(STATUS*, SCHAR*, USHORT, XSQLDA*);
STATUS API_ROUTINE isc_embed_dsql_describe(STATUS*, SCHAR*, USHORT, XSQLDA*);
STATUS API_ROUTINE isc_embed_dsql_describe_bind(STATUS*, SCHAR*, USHORT, XSQLDA*);
STATUS API_ROUTINE isc_embed_dsql_exec_immed(STATUS*, FRBRD**, FRBRD**, USHORT, SCHAR*, USHORT, XSQLDA*);
STATUS API_ROUTINE isc_embed_dsql_exec_immed2(STATUS*, FRBRD**, FRBRD**, USHORT, SCHAR*, USHORT, XSQLDA*, XSQLDA*);
STATUS API_ROUTINE isc_embed_dsql_execute(STATUS*, FRBRD**, SCHAR*, USHORT, XSQLDA*);
STATUS API_ROUTINE isc_embed_dsql_execute2(STATUS*, FRBRD**, SCHAR*, USHORT, XSQLDA*, XSQLDA*);
STATUS API_ROUTINE isc_embed_dsql_execute_immed(STATUS*, FRBRD**, FRBRD**, USHORT, SCHAR*, USHORT, XSQLDA*);
STATUS API_ROUTINE isc_embed_dsql_fetch(STATUS*, SCHAR*, USHORT, XSQLDA*);

#ifdef SCROLLABLE_CURSORS
STATUS API_ROUTINE isc_embed_dsql_fetch2(STATUS*, SCHAR*, USHORT, XSQLDA*, USHORT, SLONG);
#endif

STATUS API_ROUTINE isc_embed_dsql_fetch_a(STATUS*, int*, SCHAR*, USHORT, XSQLDA*);

#ifdef SCROLLABLE_CURSORS
STATUS API_ROUTINE isc_embed_dsql_fetch2_a(STATUS*, int*, SCHAR*, USHORT, XSQLDA*, USHORT, SLONG);
#endif

STATUS API_ROUTINE isc_embed_dsql_insert(STATUS*, SCHAR*, USHORT, XSQLDA*);
void   API_ROUTINE isc_embed_dsql_length(UCHAR*, USHORT*);
STATUS API_ROUTINE isc_embed_dsql_open(STATUS*, FRBRD**, SCHAR*, USHORT, XSQLDA*);
STATUS API_ROUTINE isc_embed_dsql_open2(STATUS*, FRBRD**, SCHAR*, USHORT, XSQLDA*, XSQLDA*);
STATUS API_ROUTINE isc_embed_dsql_prepare(STATUS*, FRBRD**, FRBRD**, SCHAR*, USHORT, SCHAR*, USHORT, XSQLDA*);
STATUS API_ROUTINE isc_embed_dsql_release(STATUS*, SCHAR*);
STATUS API_ROUTINE isc_execute(STATUS*, FRBRD**, SCHAR*, SQLDA*);
STATUS API_ROUTINE isc_execute_immediate(STATUS*, FRBRD**, FRBRD**, SSHORT*, SCHAR*);
STATUS API_ROUTINE isc_fetch(STATUS*, SCHAR*, SQLDA*);
STATUS API_ROUTINE isc_fetch_a(STATUS*, int*, SCHAR*, SQLDA*);
STATUS API_ROUTINE isc_open(STATUS*, FRBRD**, SCHAR*, SQLDA*);
STATUS API_ROUTINE isc_prepare(STATUS*, FRBRD**, FRBRD**, SCHAR*, SSHORT*, SCHAR*, SQLDA*);
int    API_ROUTINE isc_to_sqlda(SQLDA*, int, SCHAR*, int, SCHAR*);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif /*  DSQL_USER__PROTO_H */
