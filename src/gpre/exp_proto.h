/*
 *	PROGRAM:	Preprocessor
 *	MODULE:		exp_proto.h
 *	DESCRIPTION:	Prototype header file for exp.c
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

#ifndef _GPRE_EXP_PROTO_H_
#define _GPRE_EXP_PROTO_H_

extern NOD EXP_array(REQ, FLD, SSHORT, SSHORT);
extern FLD EXP_cast(FLD);
extern CTX EXP_context(REQ, SYM);
extern FLD EXP_field(CTX *);
extern FLD EXP_form_field(CTX *);
extern void EXP_left_paren(TEXT *);
extern NOD EXP_literal(void);
extern void EXP_post_array(REF);
extern REF EXP_post_field(FLD, CTX, USHORT);
extern BOOLEAN EXP_match_paren(void);
extern REL EXP_relation(void);
extern RSE EXP_rse(REQ, SYM);
extern void EXP_rse_cleanup(RSE);
extern NOD EXP_subscript(REQ);
extern SLONG EXP_SLONG_ordinal(USHORT);
extern SINT64 EXP_SINT64_ordinal(USHORT);
extern SSHORT EXP_SSHORT_ordinal(USHORT);
extern ULONG EXP_ULONG_ordinal(USHORT);
extern USHORT EXP_USHORT_ordinal(USHORT);
extern USHORT EXP_pos_USHORT_ordinal(USHORT);

#endif /* _GPRE_EXP_PROTO_H_ */
