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
 */

#ifndef _WAL_WAL_PROTO_H_
#define _WAL_WAL_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

extern SSHORT	WAL_attach (STATUS *, struct wal **, SCHAR *);
extern SSHORT	WAL_checkpoint_finish (STATUS *, struct wal *, SLONG *, SCHAR *, SLONG *, SLONG *);
extern SSHORT	WAL_checkpoint_force (STATUS *, struct wal *, SLONG *, SCHAR *, SLONG *, SLONG *);
extern SSHORT	WAL_checkpoint_start (STATUS *, struct wal *, SSHORT *);
extern SSHORT	WAL_checkpoint_recorded (STATUS *, struct wal *);
extern SSHORT	WAL_commit (STATUS *, struct wal *, UCHAR *, USHORT, SLONG *, SLONG *);
extern void	WAL_fini (STATUS *, struct wal **);
extern SSHORT	WAL_flush (STATUS *, struct wal *, SLONG *, SLONG *, BOOLEAN);
extern SSHORT	WAL_init (STATUS *, struct wal **, SCHAR *, USHORT, SCHAR *, SLONG, SSHORT, SLONG, SSHORT, SCHAR *);
extern SSHORT	WAL_journal_disable (STATUS *, struct wal *);
extern SSHORT	WAL_journal_enable (STATUS *, struct wal *, SCHAR *, USHORT, SCHAR *);
extern SSHORT	WAL_put (STATUS *, struct wal *, UCHAR *, USHORT, UCHAR *, USHORT, SLONG *, SLONG *);
extern BOOLEAN	WAL_rollover_happened (STATUS *, struct wal *, SLONG *, TEXT *, SLONG *);
extern void	WAL_rollover_recorded (struct wal *);
extern SSHORT	WAL_set_checkpoint_length (STATUS *, struct wal *, SLONG);
extern void	WAL_set_cleanup_flag (struct wal *);
extern SSHORT	WAL_set_grpc_wait_time (STATUS *, struct wal *, SLONG);
extern SSHORT	WAL_set_rollover_log (STATUS *, struct wal *, struct logfiles *);
extern SSHORT	WAL_shutdown (STATUS *, struct wal *, SLONG *, SCHAR *, SLONG *, SLONG *, SSHORT);
extern SSHORT	WAL_shutdown_old_writer (STATUS *, SCHAR *);
extern SSHORT	WAL_status (STATUS *, struct wal *, SLONG *, SCHAR *, SLONG *, SLONG *, SLONG *, SCHAR *, SLONG *, SLONG *);

#if (defined PC_PLATFORM && !defined NETWARE_386)
#define WAL_attach(a,b,c)			FAILURE
#define WAL_checkpoint_force(a,b,c,d,e,f)	FAILURE
#define WAL_checkpoint_recorded(a,b)		FAILURE
#define WAL_flush(a,b,c,d,e)			FAILURE
#endif

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif /* _WAL_WAL_PROTO_H_ */
