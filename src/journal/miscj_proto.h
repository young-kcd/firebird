/*
 *	PROGRAM:	JRD Journalling Subsystem
 *	MODULE:		misc_proto.h
 *	DESCRIPTION:	Prototype header file for misc.c
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

#ifndef JOURNAL_MISC_PROTO_H
#define JOURNAL_MISC_PROTO_H

UCHAR*	MISC_alloc_jrnl(SLONG);
void	MISC_down_case(UCHAR *, UCHAR *);
void	MISC_free_jrnl(int *);
bool	MISC_get_line(TEXT *, TEXT *, SSHORT);
void	MISC_get_new_value(SCHAR *, SCHAR *, int);
void	MISC_get_time(struct timeval*);
void	MISC_get_wal_info(struct ltjc*, SCHAR*, SCHAR*);
void	MISC_print_journal_syntax(void);
int		MISC_time_convert(TEXT *, USHORT, SLONG[2]);

#endif // JOURNAL_MISC_PROTO_H
