/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		blf_proto.h
 *	DESCRIPTION:	Prototype header file for blob_filter.cpp
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

#ifndef JRD_BLF_PROTO_H
#define JRD_BLF_PROTO_H

extern "C" {

ISC_STATUS	BLF_close_blob(TDBB, ctl**);
ISC_STATUS	BLF_create_blob(TDBB, JRD_TRA, ctl**, SLONG*,
										 USHORT, const UCHAR*,
										 ISC_STATUS(*)(), BLF);
ISC_STATUS	BLF_get_segment(TDBB, ctl**, USHORT*, USHORT, UCHAR *);
BLF			BLF_lookup_internal_filter(TDBB, SSHORT, SSHORT);
ISC_STATUS	BLF_open_blob(TDBB, JRD_TRA, ctl**, SLONG*,
									   USHORT, const UCHAR*,
									   ISC_STATUS(*)(), BLF);
ISC_STATUS	BLF_put_segment(TDBB, ctl**, USHORT, const UCHAR*);

} /* extern "C" */

#endif /* JRD_BLF_PROTO_H */

