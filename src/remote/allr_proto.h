/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		allr_proto.h
 *	DESCRIPTION:	Prototype header file for allr.c
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

#ifndef REMOTE_ALLR_PROTO_H
#define REMOTE_ALLR_PROTO_H

#ifdef DEBUG_GDS_ALLOC
#define ALLR_alloc(s)	  ALLR_alloc_debug((s),(TEXT*)__FILE__,(ULONG)__LINE__)
#define ALLR_block(s, sz) ALLR_block_debug((s), (sz), (TEXT*)__FILE__,(ULONG)__LINE__)
UCHAR*		ALLR_alloc_debug(ULONG, TEXT*, ULONG);
struct blk*	ALLR_block_debug(UCHAR, ULONG, TEXT*, ULONG);
#else  //DEBUG_GDS_ALLOC
UCHAR*		ALLR_alloc(ULONG);
struct blk*	ALLR_block(UCHAR, ULONG);
#endif //DEBUG_GDS_ALLOC

struct blk*	ALLR_clone(struct blk *);
void		ALLR_free (void *);
void		ALLR_release (void *);
struct vec*	ALLR_vector (struct vec **, ULONG);

#endif // REMOTE_ALLR_PROTO_H

