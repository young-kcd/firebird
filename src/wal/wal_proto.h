/*
 *	PROGRAM:	JRD Write Ahead Log APIs
 *	MODULE:		wal_proto.h
 *	DESCRIPTION:	Prototype header file for wal.c
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
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#ifndef _WAL_WAL_PROTO_H_
#define _WAL_WAL_PROTO_H_

extern SSHORT	WAL_attach (ISC_STATUS *, struct wal **, SCHAR *);
extern SSHORT	WAL_checkpoint_finish (ISC_STATUS *, struct wal *, SLONG *, SCHAR *, SLONG *, SLONG *);
extern SSHORT	WAL_checkpoint_force (ISC_STATUS *, struct wal *, SLONG *, SCHAR *, SLONG *, SLONG *);
extern SSHORT	WAL_checkpoint_start (ISC_STATUS *, struct wal *, SSHORT *);
extern SSHORT	WAL_checkpoint_recorded (ISC_STATUS *, struct wal *);
extern SSHORT	WAL_commit (ISC_STATUS *, struct wal *, UCHAR *, USHORT, SLONG *, SLONG *);
extern void		WAL_fini (ISC_STATUS *, struct wal **);
extern SSHORT	WAL_flush (ISC_STATUS *, struct wal *, SLONG *, SLONG *, BOOLEAN);
extern SSHORT	WAL_init (ISC_STATUS *, struct wal **, SCHAR *, USHORT, SCHAR *, SLONG, SSHORT, SLONG, SSHORT, SCHAR *);
extern SSHORT	WAL_journal_disable (ISC_STATUS *, struct wal *);
extern SSHORT	WAL_journal_enable (ISC_STATUS *, struct wal *, SCHAR *, USHORT, SCHAR *);
extern SSHORT	WAL_put (ISC_STATUS *, struct wal *, UCHAR *, USHORT, UCHAR *, USHORT, SLONG *, SLONG *);
extern bool		WAL_rollover_happened (ISC_STATUS *, struct wal *, SLONG *, TEXT *, SLONG *);
extern void		WAL_rollover_recorded (struct wal *);
extern SSHORT	WAL_set_checkpoint_length (ISC_STATUS *, struct wal *, SLONG);
extern void		WAL_set_cleanup_flag (struct wal *);
extern SSHORT	WAL_set_grpc_wait_time (ISC_STATUS *, struct wal *, SLONG);
extern SSHORT	WAL_set_rollover_log (ISC_STATUS *, struct wal *, struct logfiles *);
extern SSHORT	WAL_shutdown (ISC_STATUS *, struct wal *, SLONG *, SCHAR *, SLONG *, SLONG *, SSHORT);
extern SSHORT	WAL_shutdown_old_writer (ISC_STATUS *, SCHAR *);
extern SSHORT	WAL_status (ISC_STATUS *, struct wal *, SLONG *, SCHAR *, SLONG *, SLONG *, SLONG *, SCHAR *, SLONG *, SLONG *);

#endif /* _WAL_WAL_PROTO_H_ */
