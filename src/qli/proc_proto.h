/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		proc_proto.h
 *	DESCRIPTION:	Prototype header file for proc.c
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

#ifndef _QLI_PROC_PROTO_H_
#define _QLI_PROC_PROTO_H_

extern void	PRO_close (struct dbb *, FRBRD *);
extern void	PRO_commit (struct dbb *);
extern void	PRO_copy_procedure (struct dbb *, TEXT *, struct dbb *, TEXT *);
extern void	PRO_create (struct dbb *, TEXT *);
extern int	PRO_delete_procedure (struct dbb *, TEXT *);
extern void	PRO_edit_procedure (struct dbb *, TEXT *);
extern FRBRD	*PRO_fetch_procedure (struct dbb *, TEXT *);
extern bool	PRO_get_line (FRBRD *, TEXT *, USHORT);
extern void	PRO_invoke (struct dbb *, TEXT *);
extern FRBRD	*PRO_open_blob (struct dbb *, SLONG *);
extern int	PRO_rename_procedure (struct dbb *, TEXT *, TEXT *);
extern void	PRO_rollback (struct dbb *);
extern void	PRO_scan (struct dbb *, void (*)(), void *);
extern void	PRO_setup (struct dbb *);
extern FRBRD	*PRO_transaction (struct dbb *, int);

#endif /* _QLI_PROC_PROTO_H_ */
