/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		exe_proto.h
 *	DESCRIPTION:	Prototype header file for exe.c
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

#ifndef QLI_EXE_PROTO_H
#define QLI_EXE_PROTO_H

extern void		EXEC_abort (void);
extern void		EXEC_execute(qli_nod*);
extern FRBRD*	EXEC_open_blob(qli_nod*);
extern file*	EXEC_open_output(qli_nod*);
extern void		EXEC_poll_abort (void);
extern dsc*		EXEC_receive(qli_msg*, par*);
extern void		EXEC_send(qli_msg*);
extern void		EXEC_start_request(qli_req*, qli_msg*);
extern void		EXEC_top(qli_nod*);

#endif /* QLI_EXE_PROTO_H */
