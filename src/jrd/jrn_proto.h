/*
 *	PROGRAM:	JRD Access method
 *	MODULE:		jrn_proto.h		
 *	DESCRIPTION:	Prototype Header file for jrn.c
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

#ifndef JRD_JRN_PROTO_H
#define JRD_JRN_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

int JRN_archive_begin(ISC_STATUS *, struct jrn **, SLONG, SLONG, TEXT *,
							 USHORT);
int JRN_archive_end(ISC_STATUS *, struct jrn **, SLONG, SLONG);
int JRN_archive_error(ISC_STATUS *, struct jrn **, SLONG, SLONG, SLONG);
int JRN_disable(ISC_STATUS *, struct jrn *, struct jrnh *, UCHAR *,
					   USHORT);
void JRN_dump_page(void);
int JRN_enable(ISC_STATUS *, struct jrn **, TEXT *, USHORT, UCHAR *,
					  USHORT, struct ltjc *);
int JRN_fini(ISC_STATUS *, struct jrn **);
int JRN_init(ISC_STATUS *, struct jrn **, USHORT, UCHAR *, USHORT, UCHAR *,
					USHORT);
void JRN_make_init_data(UCHAR *, SSHORT *, UCHAR *, USHORT, UCHAR *,
							   USHORT);
int JRN_put_wal_name(ISC_STATUS *, struct jrn *, TEXT *, USHORT, SLONG,
							SLONG, SLONG, USHORT);
int JRN_put_wal_info(ISC_STATUS *, struct jrn *, TEXT *, USHORT, SLONG,
							SLONG, SLONG, USHORT, USHORT, USHORT *, USHORT);
int JRN_put_old_start(ISC_STATUS *, struct jrn *, SLONG, SLONG, SLONG,
							 USHORT *);
int JRN_put_old_end(ISC_STATUS *, struct jrn *, SLONG, SLONG, SLONG,
						   USHORT);
int JRN_put_old_file(ISC_STATUS *, struct jrn *, SCHAR *, USHORT, SLONG,
							USHORT, USHORT);
void JRN_sync(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // JRD_JRN_PROTO_H
