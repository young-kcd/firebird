/*
 *	PROGRAM:	Preprocessor
 *	MODULE:		msc_proto.h
 *	DESCRIPTION:	Prototype header file for msc.c
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

extern ACT MSC_action(GPRE_REQ, enum act_t);
extern UCHAR *MSC_alloc(int);
extern UCHAR *MSC_alloc_permanent(int);
extern GPRE_NOD MSC_binary(NOD_T, GPRE_NOD, GPRE_NOD);
extern GPRE_CTX MSC_context(GPRE_REQ);
extern void MSC_copy(const char*, int, char*);
extern void MSC_copy_cat(const char*, int, const char*, int, char*);
extern SYM MSC_find_symbol(SYM, enum sym_t);
extern void MSC_free(UCHAR *);
extern void MSC_free_request(GPRE_REQ);
extern void MSC_init(void);
extern BOOLEAN MSC_match(KWWORDS);
#ifdef PYXIS
extern BOOLEAN MSC_member(GPRE_NOD, LLS);
#endif
extern GPRE_NOD MSC_node(enum nod_t, SSHORT);
extern GPRE_NOD MSC_pop(LLS *);
extern PRV MSC_privilege_block(void);
extern void MSC_push(GPRE_NOD, LLS *);
extern REF MSC_reference(REF *);
extern GPRE_REQ MSC_request(enum req_t);
extern SCHAR *MSC_string(TEXT *);
extern SYM MSC_symbol(enum sym_t, TEXT *, USHORT, GPRE_CTX);
extern GPRE_NOD MSC_unary(NOD_T, GPRE_NOD);
extern USN MSC_username(SCHAR *, USHORT);

#endif /* GPRE_MSC_PROTO_H */
