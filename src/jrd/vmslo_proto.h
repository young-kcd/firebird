/*
 *	PROGRAM:	JRD Lock Manager
 *	MODULE:		vmslo_proto.h
 *	DESCRIPTION:	Prototype header file for vmslock.c
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

#ifndef _JRD_VMSLO_PROTO_H_
#define _JRD_VMSLO_PROTO_H_

extern int LOCK_convert(SLONG, UCHAR, SSHORT, int (*)(int *), int *,
						ISC_STATUS *);
extern int LOCK_deq(SLONG);
extern UCHAR LOCK_downgrade(SLONG, ISC_STATUS *);
extern SLONG LOCK_enq(SLONG, SLONG, USHORT, UCHAR *, USHORT, UCHAR,
					  int (*)(int *), int *, SLONG, SSHORT, ISC_STATUS *, SLONG);
extern void LOCK_fini(ISC_STATUS *, SLONG *);
extern int LOCK_init(ISC_STATUS *, SSHORT, SLONG, UCHAR, SLONG *);
extern void LOCK_manager(SLONG);
extern SLONG LOCK_read_data(SLONG);
extern SLONG LOCK_read_data2(SLONG, USHORT, UCHAR *, USHORT, SLONG);
extern void LOCK_re_post(int (*)(int *), int *, SLONG);
extern SLONG LOCK_write_data(SLONG, SLONG);

#endif /* _JRD_VMSLO_PROTO_H_ */
