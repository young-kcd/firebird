/*
 *	PROGRAM:	JRD Lock Manager
 *	MODULE:		lock_proto.h
 *	DESCRIPTION:	Prototype header file for lock.c
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

#ifndef _LOCK_LOCK_PROTO_H_
#define _LOCK_LOCK_PROTO_H_

int	LOCK_convert(SLONG, UCHAR, SSHORT, int (*)(void *), void *,
						ISC_STATUS *);
int		LOCK_deq(SLONG);
UCHAR	LOCK_downgrade(SLONG, ISC_STATUS *);
SLONG	LOCK_enq(SLONG, SLONG, USHORT, UCHAR *, USHORT, UCHAR,
					  int (*)(void *), void *, SLONG, SSHORT, ISC_STATUS *,
					  SLONG);
void	LOCK_fini(ISC_STATUS *, SLONG *);
int		LOCK_init(ISC_STATUS *, SSHORT, SLONG, UCHAR, SLONG *);
void	LOCK_manager(SLONG);
SLONG	LOCK_query_data(SLONG, USHORT, USHORT);
SLONG	LOCK_read_data(SLONG);
SLONG	LOCK_read_data2(SLONG, USHORT, UCHAR *, USHORT, SLONG);
void	LOCK_re_post(int (*)(void *), void *, SLONG);
bool	LOCK_shut_manager(void);
SLONG	LOCK_write_data(SLONG, SLONG);

#endif /* _LOCK_LOCK_PROTO_H_ */
