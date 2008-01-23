/*
 *	PROGRAM:	JRD Access method
 *	MODULE:		sch_proto.h
 *	DESCRIPTION:	Prototype Header file for sch.cpp
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

#ifndef JRD_SCH_PROTO_H
#define JRD_SCH_PROTO_H

extern "C" {

/* AST actions taken by SCH_ast() */

enum ast_t
{
	AST_alloc,
	AST_init,
	AST_fini,
	AST_check,
	AST_disable,
	AST_enable,
	AST_enter,
	AST_exit
};

int		API_ROUTINE gds__thread_enable(int);
void	API_ROUTINE gds__thread_enter();
void	API_ROUTINE gds__thread_exit();

} // extern "C"

// Thread quanta

const int QUANTUM			= 100;	// Default quantum
const int SWEEP_QUANTUM		= 10;	// Make sweeps less disruptive

struct thread;

void	SCH_ast(enum ast_t);
thread*	SCH_current_thread();
void	SCH_enter();
void	SCH_exit();
void	SCH_hiber();
bool	SCH_schedule();
bool	SCH_thread_enter_check();
void	SCH_wake(thread*);


#endif // JRD_SCH_PROTO_H

