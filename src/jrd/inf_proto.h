/*
 *	PROGRAM:	JRD Access method
 *	MODULE:		inf_proto.h
 *	DESCRIPTION:	Prototype header file for inf.c
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

#ifndef JRD_INF_PROTO_H
#define JRD_INF_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

int		INF_blob_info(struct blb *, SCHAR *, SSHORT, SCHAR *, SSHORT);
USHORT	INF_convert(SLONG, SCHAR *);
int		INF_database_info(SCHAR *, SSHORT, SCHAR *, SSHORT);
SCHAR*	INF_put_item(SCHAR, USHORT, SCHAR *, SCHAR *, SCHAR *);
int		INF_request_info(struct jrd_req *, SCHAR *, SSHORT, SCHAR *, SSHORT);
int		INF_transaction_info(struct jrd_tra *, SCHAR *, SSHORT, SCHAR *, SSHORT);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // JRD_INF_PROTO_H
