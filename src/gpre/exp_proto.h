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

#ifndef GPRE_EXP_PROTO_H
#define GPRE_EXP_PROTO_H

extern GPRE_NOD EXP_array(GPRE_REQ, GPRE_FLD, SSHORT, SSHORT);
extern GPRE_FLD EXP_cast(GPRE_FLD);
extern GPRE_CTX EXP_context(GPRE_REQ, SYM);
extern GPRE_FLD EXP_field(GPRE_CTX *);
extern void EXP_left_paren(TEXT *);
extern GPRE_NOD EXP_literal(void);
extern void EXP_post_array(REF);
extern REF EXP_post_field(GPRE_FLD, GPRE_CTX, USHORT);
extern bool EXP_match_paren(void);
extern GPRE_REL EXP_relation(void);
extern RSE EXP_rse(GPRE_REQ, SYM);
extern void EXP_rse_cleanup(RSE);
extern GPRE_NOD EXP_subscript(GPRE_REQ);
extern SLONG EXP_SLONG_ordinal(USHORT);
extern SINT64 EXP_SINT64_ordinal(USHORT);
extern SSHORT EXP_SSHORT_ordinal(USHORT);
extern ULONG EXP_ULONG_ordinal(USHORT);
extern USHORT EXP_USHORT_ordinal(bool);
extern USHORT EXP_pos_USHORT_ordinal(bool);

#endif /* GPRE_EXP_PROTO_H */

