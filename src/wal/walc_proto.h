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

#ifndef _WAL_WALC_PROTO_H_
#define _WAL_WALC_PROTO_H_

extern void	WALC_acquire (struct wal *, struct wals **);
extern void	WALC_alarm_handler (struct event *);
extern SSHORT	WALC_bug (ISC_STATUS *, TEXT *, TEXT *);
extern void	WALC_build_dbg_filename (TEXT *, TEXT *);
extern void	WALC_build_logname (TEXT *, TEXT *, SLONG);
extern SSHORT	WALC_check_writer (struct wal *);
extern void	WALC_fini (ISC_STATUS *, struct wal **);
extern SSHORT	WALC_init (ISC_STATUS *, struct wal **, TEXT *, USHORT, TEXT *, SLONG, SSHORT, SLONG, SSHORT, UCHAR *, USHORT);
extern void	WALC_release (struct wal *);
extern void	WALC_save_status_strings (ISC_STATUS *);
extern void	WALC_setup_buffer_block (struct wals *, struct walblk *, SSHORT);

#endif	/* _WAL_WALC_PROTO_H_ */
