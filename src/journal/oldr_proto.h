/*
 *	PROGRAM:	JRD Journalling Subsystem
 *	MODULE:		oldr_proto.h
 *	DESCRIPTION:	Prototype header file for oldr.c
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

#ifndef _JOURNAL_OLDR_PROTO_H_
#define _JOURNAL_OLDR_PROTO_H_

extern int OLDR_close(OLD *);
extern int OLDR_get(OLD, SCHAR *, SSHORT *);
extern void OLDR_get_info(OLD, SSHORT *, SSHORT *, SLONG *, SLONG *, SLONG *);
extern int OLDR_open(OLD *, SCHAR *, SSHORT, SCHAR **);

#endif /* _JOURNAL_OLDR_PROTO_H_ */
