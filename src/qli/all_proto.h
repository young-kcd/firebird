/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		all_proto.h
 *	DESCRIPTION:	Prototype header file for all.c
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

#ifndef QLI_ALL_PROTO_H
#define QLI_ALL_PROTO_H

blk*	ALLQ_alloc(plb*, UCHAR, int);
blk*	ALLQ_extend(blk**, int);
void	ALLQ_fini(void);
void	ALLQ_free(SCHAR*);
void	ALLQ_init(void);
SCHAR*	ALLQ_malloc(SLONG);
plb*	ALLQ_pool(void);
void	ALLQ_push(blk*, lls**);
blk*	ALLQ_pop(lls**);
void	ALLQ_release(frb*);
void	ALLQ_rlpool(plb*);

#endif /* QLI_ALL_PROTO_H */

