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

bool	LEX_active_procedure (void);
void	LEX_edit (SLONG, SLONG);
qli_tok*	LEX_edit_string(void);
qli_tok*	LEX_filename(void);
void	LEX_fini (void);
void	LEX_flush (void);
bool	LEX_get_line (const TEXT*, TEXT *, int);
void	LEX_init (void);
void	LEX_mark_statement (void);
void	LEX_pop_line (void);
void	LEX_procedure(dbb*, FB_API_HANDLE);
bool	LEX_push_file (const TEXT*, const bool);
bool	LEX_push_string (const TEXT* const);
void	LEX_put_procedure (FB_API_HANDLE, SLONG, SLONG);
void	LEX_real (void);
qli_lls*	LEX_statement_list(void);
qli_tok*	LEX_token(void);

#endif // QLI_LEX_PROTO_H

