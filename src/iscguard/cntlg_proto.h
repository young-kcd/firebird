/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		cntl_proto.h
 *	DESCRIPTION:	Prototpe header file for cntl.c
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

#ifndef REMOTE_CNTL_PROTO_H
#define REMOTE_CNTL_PROTO_H

void	CNTL_init(FPTR_VOID, TEXT *);
void*	CNTL_insert_thread(void);
void	CNTL_main_thread(SLONG, SCHAR *[]);
void	CNTL_remove_thread(void *);
void	CNTL_shutdown_service(TEXT *);
void	CNTL_stop_service(TEXT *);

#endif // REMOTE_CNTL_PROTO_H
