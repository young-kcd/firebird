/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		lex_proto.h
 *	DESCRIPTION:	Prototype header file for lex.cpp
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

#ifndef QLI_LEX_PROTO_H
#define QLI_LEX_PROTO_H

extern bool		LEX_active_procedure (void);
extern void		LEX_edit (SLONG, SLONG);
extern tok*		LEX_edit_string(void);
extern tok*		LEX_filename(void);
extern void		LEX_fini (void);
extern void		LEX_flush (void);
extern bool		LEX_get_line (TEXT *, TEXT *, int);
extern void		LEX_init (void);
extern void		LEX_mark_statement (void);
extern void		LEX_pop_line (void);
extern void		LEX_procedure(dbb*, FRBRD*);
extern bool		LEX_push_file (const TEXT*, const bool);
extern bool		LEX_push_string (const TEXT* const);
extern void		LEX_put_procedure (FRBRD *, SLONG, SLONG);
extern void		LEX_real (void);
extern lls*		LEX_statement_list(void);
extern tok*		LEX_token(void);

#endif /* QLI_LEX_PROTO_H */

