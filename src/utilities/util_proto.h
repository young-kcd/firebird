/*
 *      PROGRAM:        UTILITIES shared functions prototypes.
 *      MODULE:         util_proto.h
 *      DESCRIPTION:    Prototype header file for util.c
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

#ifndef _UTIL_PROTO_H_
#define _UTIL_PROTO_H_

extern pid_t UTIL_start_process(char *process, char **argv);
extern int 	UTIL_wait_for_child(pid_t child_pid); 
extern int 	UTIL_ex_lock(char *file); 
extern void UTIL_ex_unlock(int fd_file); 

#endif /* _UTIL_PROTO_H_ */

