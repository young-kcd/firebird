/*
 *	PROGRAM:	Data Definition Utility
 *	MODULE:		parse_proto.h
 *	DESCRIPTION:	Prototype header file for parse.c
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

#ifndef _DUDLEY_PARSE_PROTO_H_
#define _DUDLEY_PARSE_PROTO_H_

#include "../dudley/parse.h"

extern void PARSE_actions(void);
extern void PARSE_error(USHORT, TEXT *, TEXT *);
extern FUNC PARSE_function(int);
extern enum kwwords PARSE_keyword(void);
extern DUDLEY_NOD PARSE_make_list(LLS);
extern DUDLEY_NOD PARSE_make_node(enum nod_t, USHORT);
extern int PARSE_match(enum kwwords);
extern int PARSE_number(void);
extern DUDLEY_REL PARSE_relation(void);
extern SYM PARSE_symbol(enum tok_t);

#endif /* _DUDLEY_PARSE_PROTO_H_ */
