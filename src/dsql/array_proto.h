/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		array_proto.h
 *	DESCRIPTION:	Prototype Header file for array.epp
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

#ifndef DSQL_ARRAY_PROTO_H
#define DSQL_ARRAY_PROTO_H

extern ISC_STATUS API_ROUTINE isc_array_get_slice(ISC_STATUS *, void **, void **,
											  GDS_QUAD *, ISC_ARRAY_DESC *,
											  void *, SLONG *);
extern ISC_STATUS API_ROUTINE isc_array_lookup_bounds(ISC_STATUS *, void **, void **,
												  SCHAR *, SCHAR *,
												  ISC_ARRAY_DESC *);
extern ISC_STATUS API_ROUTINE isc_array_lookup_desc(ISC_STATUS *, void **, void **,
												SCHAR *, SCHAR *,
												ISC_ARRAY_DESC *);
extern ISC_STATUS API_ROUTINE isc_array_put_slice(ISC_STATUS *, void **, void **,
											  GDS_QUAD *, ISC_ARRAY_DESC *,
											  void *, SLONG *);
extern ISC_STATUS API_ROUTINE isc_array_set_desc(ISC_STATUS *, SCHAR *, SCHAR *,
											 SSHORT *, SSHORT *, SSHORT *,
											 ISC_ARRAY_DESC *);

#endif // DSQL_ARRAY_PROTO_H

