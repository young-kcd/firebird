/*
 *	PROGRAM:	Preprocessor
 *	MODULE:		msc_proto.h
 *	DESCRIPTION:	Prototype header file for msc.cpp
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

#ifndef GPRE_MSC_PROTO_H
#define GPRE_MSC_PROTO_H

#include "../gpre/parse.h"

ACT			MSC_action(GPRE_REQ, enum act_t);
UCHAR*		MSC_alloc(int);
UCHAR*		MSC_alloc_permanent(int);
GPRE_NOD	MSC_binary(NOD_T, GPRE_NOD, GPRE_NOD);
GPRE_CTX	MSC_context(GPRE_REQ);
void		MSC_copy(const char*, int, char*);
void		MSC_copy_cat(const char*, int, const char*, int, char*);
SYM			MSC_find_symbol(SYM, enum sym_t);
void		MSC_free(UCHAR*);
void		MSC_free_request(GPRE_REQ);
void		MSC_init(void);
bool		MSC_match(KWWORDS);
GPRE_NOD	MSC_node(enum nod_t, SSHORT);
GPRE_NOD	MSC_pop(LLS*);
PRV			MSC_privilege_block(void);
void		MSC_push(GPRE_NOD, LLS*);
REF			MSC_reference(REF*);
GPRE_REQ	MSC_request(enum req_t);
SCHAR*		MSC_string(const TEXT*);
SYM			MSC_symbol(enum sym_t, const TEXT*, USHORT, GPRE_CTX);
GPRE_NOD	MSC_unary(NOD_T, GPRE_NOD);
GPRE_USN	MSC_username(SCHAR*, USHORT);

#endif // GPRE_MSC_PROTO_H

