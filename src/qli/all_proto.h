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

#ifndef _QLI_ALL_PROTO_H_
#define _QLI_ALL_PROTO_H_

extern blk*		ALLQ_alloc(plb*, UCHAR, int);
extern blk*		ALLQ_extend(blk**, int);
extern void		ALLQ_fini(void);
extern void		ALLQ_free(SCHAR*);
extern void		ALLQ_init(void);
extern SCHAR*	ALLQ_malloc(SLONG);
extern plb*		ALLQ_pool(void);
extern void		ALLQ_push(blk*, lls**);
extern blk*		ALLQ_pop(lls**);
extern void		ALLQ_release(frb*);
extern void		ALLQ_rlpool(plb*);

#endif /* _QLI_ALL_PROTO_H_ */
