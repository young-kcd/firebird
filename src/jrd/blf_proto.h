/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		blf_proto.h
 *	DESCRIPTION:	Prototype header file for blf.c
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

#ifndef _JRD_BLF_PROTO_H_
#define _JRD_BLF_PROTO_H_

extern "C" {

extern ISC_STATUS DLL_EXPORT BLF_close_blob(TDBB, struct ctl **);
extern ISC_STATUS DLL_EXPORT BLF_create_blob(TDBB, JRD_TRA, struct ctl **, SLONG *,
										 USHORT, UCHAR *, ISC_STATUS(*)(), BLF);
extern ISC_STATUS DLL_EXPORT BLF_get_segment(TDBB, struct ctl **, USHORT *,
										 USHORT, UCHAR *);
extern BLF DLL_EXPORT BLF_lookup_internal_filter(TDBB, SSHORT, SSHORT);
extern ISC_STATUS DLL_EXPORT BLF_open_blob(TDBB, JRD_TRA, struct ctl **, SLONG *,
									   USHORT, UCHAR *, ISC_STATUS(*)(), BLF);
extern ISC_STATUS DLL_EXPORT BLF_put_segment(TDBB, struct ctl **, USHORT,
										 UCHAR *);

} /* extern "C" */

#endif /* _JRD_BLF_PROTO_H_ */
