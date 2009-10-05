/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		lex_proto.h
 *	DESCRIPTION:	Prototype header file for lex.c
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

#ifndef _QLI_LEX_PROTO_H_
#define _QLI_LEX_PROTO_H_

extern int		LEX_active_procedure (void);
extern void		LEX_edit (SLONG, SLONG);
extern struct tok	*LEX_edit_string (void);
extern struct tok	*LEX_filename (void);
extern void		LEX_fini (void);
extern void		LEX_flush (void);
extern int		LEX_get_line (TEXT *, TEXT *, int);
extern void		LEX_init (void);
extern void		LEX_mark_statement (void);
extern void		LEX_pop_line (void);
extern void		LEX_procedure (struct dbb *, FRBRD *);
extern int		LEX_push_file (TEXT *, int);
extern int		LEX_push_string (TEXT *);
extern void		LEX_put_procedure (FRBRD *, SLONG, SLONG);
extern void		LEX_real (void);
extern struct lls	*LEX_statement_list (void);
extern struct tok	*LEX_token (void);

#endif /* _QLI_LEX_PROTO_H_ */
