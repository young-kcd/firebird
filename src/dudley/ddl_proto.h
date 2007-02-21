/*
 *	PROGRAM:	Data Definition Utility
 *	MODULE:		ddl_proto.h
 *	DESCRIPTION:	Prototype header file for ddl.c
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

#ifndef _DUDLEY_DDL_PROTO_H_
#define _DUDLEY_DDL_PROTO_H_

extern UCHAR *DDL_alloc(int);
extern int DDL_db_error(ISC_STATUS *, USHORT, TEXT *, TEXT *, TEXT *, TEXT *,
						TEXT *);
extern int DDL_err(USHORT, TEXT *, TEXT *, TEXT *, TEXT *, TEXT *);
extern void DDL_error_abort(ISC_STATUS *, USHORT, TEXT *, TEXT *, TEXT *, TEXT *,
							TEXT *);
extern void DDL_exit(int);
extern void DDL_msg_partial(USHORT, TEXT *, TEXT *, TEXT *, TEXT *, TEXT *);
extern void DDL_msg_put(USHORT, TEXT *, TEXT *, TEXT *, TEXT *, TEXT *);
extern DUDLEY_NOD DDL_pop(LLS *);
extern void DDL_push(DUDLEY_NOD, LLS *);
extern int DDL_yes_no(USHORT);

#endif /* _DUDLEY_DDL_PROTO_H_ */
