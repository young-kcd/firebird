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

#ifndef _JRD_SCH_PROTO_H_
#define _JRD_SCH_PROTO_H_

#include "../jrd/isc.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int API_ROUTINE gds__thread_enable(int);
extern void API_ROUTINE gds__thread_enter(void);
extern void API_ROUTINE gds__thread_exit(void);
extern void SCH_abort(void);
extern void SCH_ast(enum ast_t);
extern struct thread *SCH_current_thread(void);
extern void SCH_enter(void);
extern void SCH_exit(void);
extern void SCH_hiber(void);
extern void SCH_init(void);
extern int SCH_schedule(void);
extern BOOLEAN SCH_thread_enter_check(void);
extern BOOLEAN SCH_validate(void);
extern void SCH_wake(struct thread *);
#ifdef VMS
extern int API_ROUTINE gds__ast_active(void);
extern void API_ROUTINE gds__completion_ast(void);
extern int API_ROUTINE gds__thread_wait(int (*)(), SLONG);
#endif /* VMS  */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _JRD_SCH_PROTO_H_ */
