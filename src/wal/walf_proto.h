/*
 *	PROGRAM:	JRD Write Ahead LOG APIs 
 *	MODULE:		walf_proto.h
 *	DESCRIPTION:	Prototype header file for walf.c
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

#ifndef _WAL_WALF_PROTO_H_
#define _WAL_WALF_PROTO_H_

extern SSHORT	WALF_delink_log (ISC_STATUS *, SCHAR *, SCHAR *, SLONG);
extern SSHORT	WALF_delink_prev_log (ISC_STATUS *, SCHAR *, SCHAR *, SLONG);
extern void	WALF_dispose_log_header (struct walfh *);
extern SSHORT	WALF_get_linked_logs_info (ISC_STATUS *, SCHAR *, SCHAR *, SLONG, int *, SCHAR *, SLONG *, SLONG *, SSHORT *);
extern SSHORT	WALF_get_log_info (ISC_STATUS *, SCHAR *, SCHAR *, SLONG, SLONG *, SLONG *, SLONG *);
extern SSHORT	WALF_get_next_log_info (ISC_STATUS *, SCHAR *, SCHAR *, SLONG, SCHAR *, SLONG *, SLONG *, SLONG *, SLONG *, SSHORT);
extern SSHORT	WALF_get_all_next_logs_info (ISC_STATUS *, SCHAR *, SCHAR *, SLONG, int, SCHAR *, int *, SCHAR * *, SLONG *, SLONG *, SLONG *, SLONG *, SSHORT);
extern SSHORT	WALF_init_p_log (ISC_STATUS *, SCHAR *, SCHAR *, SLONG, SSHORT);
extern SSHORT	WALF_open_partitioned_log_file (ISC_STATUS *, SCHAR *, SCHAR *, struct p_logfh *, SLONG *);
extern SSHORT	WALF_open_log_file (ISC_STATUS *, SCHAR *, SCHAR *, SLONG, struct walfh *, SLONG *);
extern SSHORT	WALF_set_log_header_flag (ISC_STATUS *, SCHAR *, SCHAR *, SLONG, SLONG, int);
extern SSHORT	WALF_update_log_header (ISC_STATUS *, SCHAR *, SLONG, struct walfh *, SLONG);
extern void	WALF_upd_log_hdr_frm_walfh_data (struct walfh *, UCHAR *);
extern SSHORT	WALF_update_partitioned_log_hdr (ISC_STATUS *, SCHAR *, struct p_logfh *, SLONG);

#endif /* _WAL_WALF_PROTO_H_ */ 
