/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		utl_proto.h
 *	DESCRIPTION:	Prototype header file for utl.cpp
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

#ifndef JRD_UTL_PROTO_H
#define JRD_UTL_PROTO_H

#ifndef INCLUDE_FB_TYPES_H
#include "../include/fb_types.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FRBRD
#define FRBRD void
#endif

int		API_ROUTINE gds__blob_size(FRBRD **, SLONG *, SLONG *, SLONG *);
void	API_ROUTINE_VARARG isc_expand_dpb(SCHAR**, SSHORT*, ...);
int		API_ROUTINE isc_modify_dpb(SCHAR**, SSHORT*, USHORT, const SCHAR*, SSHORT);
int		API_ROUTINE gds__edit(TEXT *, USHORT);
SLONG	API_ROUTINE gds__event_block(SCHAR **, SCHAR **, USHORT, ...);
USHORT	API_ROUTINE gds__event_block_a(SCHAR **, SCHAR **, SSHORT,
											 SCHAR **);
void	API_ROUTINE gds__event_block_s(SCHAR **, SCHAR **, SSHORT,
										   SCHAR **, SSHORT *);
void	API_ROUTINE gds__event_counts(ULONG *, SSHORT, SCHAR *, SCHAR *);
void	API_ROUTINE gds__get_client_version(SCHAR *);
int		API_ROUTINE gds__get_client_major_version();
int		API_ROUTINE gds__get_client_minor_version();
void	API_ROUTINE gds__map_blobs(int*, int*);
void	API_ROUTINE gds__set_debug(int);
void	API_ROUTINE isc_set_login(const UCHAR**, SSHORT*);
BOOLEAN	API_ROUTINE isc_set_path(TEXT*, USHORT, TEXT*);
void	API_ROUTINE isc_set_single_user(const UCHAR**, SSHORT*, const TEXT*);
int		API_ROUTINE gds__version(FRBRD **, FPTR_VOID, void *);
void	API_ROUTINE isc_format_implementation(USHORT, USHORT, TEXT *,
												  USHORT, USHORT, TEXT *);
U_IPTR	API_ROUTINE isc_baddress(SCHAR *);
void	API_ROUTINE isc_baddress_s(SCHAR *, U_IPTR *);
int		API_ROUTINE BLOB_close(struct bstream *);
int		API_ROUTINE blob__display(SLONG *, FRBRD **, FRBRD **, TEXT *,
									 SSHORT *);
int		API_ROUTINE BLOB_display(GDS_QUAD *, FRBRD*, FRBRD*, TEXT*);
int		API_ROUTINE blob__dump(SLONG *, FRBRD **, FRBRD **, TEXT *,
								  SSHORT *);
int		API_ROUTINE BLOB_dump(GDS_QUAD *, FRBRD*, FRBRD*, SCHAR*);
int		API_ROUTINE blob__edit(SLONG *, FRBRD **, FRBRD **, TEXT *,
								  SSHORT *);
int		API_ROUTINE BLOB_edit(GDS_QUAD *, FRBRD*, FRBRD*, SCHAR *);
int		API_ROUTINE BLOB_get(struct bstream *);
int		API_ROUTINE blob__load(SLONG *, FRBRD **, FRBRD **, TEXT *,
								  SSHORT *);
int		API_ROUTINE BLOB_load(GDS_QUAD *, FRBRD*, FRBRD*, TEXT *);
int		API_ROUTINE BLOB_text_dump(GDS_QUAD *, FRBRD*, FRBRD*, SCHAR *);
int		API_ROUTINE BLOB_text_load(GDS_QUAD *, FRBRD*, FRBRD*, TEXT *);
struct	bstream* API_ROUTINE Bopen(GDS_QUAD*, FRBRD*, FRBRD*, const SCHAR*);
struct  bstream* API_ROUTINE BLOB_open(FRBRD*, SCHAR*, int);
int		API_ROUTINE BLOB_put(SCHAR, struct bstream*);

#ifdef VMS
ISC_STATUS API_ROUTINE gds__attach_database_d(ISC_STATUS*,
												 struct dsc$descriptor_s*,
												 int, SSHORT, const SCHAR*,
												 SSHORT);
void	API_ROUTINE gds__wake_init(void);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // JRD_UTL_PROTO_H

