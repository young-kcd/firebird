/*
 *	PROGRAM:	JRD Write Ahead LOG APIs 
 *	MODULE:		walc_proto.h
 *	DESCRIPTION:	Prototype header file for walc.c
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

#ifndef WAL_WALC_PROTO_H
#define WAL_WALC_PROTO_H

void	WALC_acquire (struct wal *, struct wals **);
void	WALC_alarm_handler (void *);
SSHORT	WALC_bug (ISC_STATUS *, TEXT *, TEXT *);
void	WALC_build_dbg_filename (TEXT *, TEXT *);
void	WALC_build_logname (TEXT *, TEXT *, SLONG);
SSHORT	WALC_check_writer (struct wal *);
void	WALC_fini (ISC_STATUS *, struct wal **);
SSHORT	WALC_init (ISC_STATUS *, struct wal **, TEXT *, USHORT, TEXT *,
						   SLONG, bool, SLONG, SSHORT, UCHAR *, bool);
void	WALC_release (struct wal *);
void	WALC_save_status_strings (ISC_STATUS *);
void	WALC_setup_buffer_block (struct wals *, struct walblk *, bool);

#endif	// WAL_WALC_PROTO_H
