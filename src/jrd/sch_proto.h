/*
 *	PROGRAM:	JRD Access method
 *	MODULE:		sch_proto.h
 *	DESCRIPTION:	Prototype Header file for sch.c
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

#include "../jrd/isc.h"

#ifdef __cplusplus
extern "C" {
#endif

int		API_ROUTINE gds__thread_enable(int);
void	API_ROUTINE gds__thread_enter(void);
void	API_ROUTINE gds__thread_exit(void);
void	SCH_abort(void);
void	SCH_ast(enum ast_t);
struct thread*	SCH_current_thread(void);
void	SCH_enter(void);
void	SCH_exit(void);
void	SCH_hiber(void);
void	SCH_init(void);
int		SCH_schedule(void);
BOOLEAN	SCH_thread_enter_check(void);
BOOLEAN	SCH_validate(void);
void	SCH_wake(struct thread *);
#ifdef VMS
int		API_ROUTINE gds__ast_active(void);
void	API_ROUTINE gds__completion_ast(void);
int		API_ROUTINE gds__thread_wait(int (*)(), SLONG);
#endif /* VMS  */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // JRD_SCH_PROTO_H
