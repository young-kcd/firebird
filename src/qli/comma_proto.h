/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		comma_proto.h
 *	DESCRIPTION:	Prototype header file for command.c
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

#ifndef QLI_COMMA_PROTO_H
#define QLI_COMMA_PROTO_H

int		CMD_check_ready(void);
void	CMD_copy_procedure(syn*);
void	CMD_define_procedure(syn*);
void	CMD_delete_proc(syn*);
void	CMD_edit_proc(syn*);
void	CMD_extract(syn*);
void	CMD_finish(syn*);
void	CMD_rename_proc(syn*);
void	CMD_set(syn*);
void	CMD_shell(syn*);
void	CMD_transaction(syn*);

#endif /* QLI_COMMA_PROTO_H */
