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

GPRE_NOD	EXP_array(GPRE_REQ, GPRE_FLD, SSHORT, SSHORT);
GPRE_FLD	EXP_cast(GPRE_FLD);
GPRE_CTX	EXP_context(GPRE_REQ, SYM);
GPRE_FLD	EXP_field(GPRE_CTX *);
void		EXP_left_paren(TEXT *);
GPRE_NOD	EXP_literal(void);
void		EXP_post_array(REF);
REF			EXP_post_field(GPRE_FLD, GPRE_CTX, USHORT);
bool		EXP_match_paren(void);
GPRE_REL	EXP_relation(void);
GPRE_RSE	EXP_rse(GPRE_REQ, SYM);
void		EXP_rse_cleanup(GPRE_RSE);
GPRE_NOD	EXP_subscript(GPRE_REQ);
SLONG		EXP_SLONG_ordinal(USHORT);
SINT64		EXP_SINT64_ordinal(USHORT);
SSHORT		EXP_SSHORT_ordinal(USHORT);
ULONG		EXP_ULONG_ordinal(USHORT);
USHORT		EXP_USHORT_ordinal(bool);
USHORT		EXP_pos_USHORT_ordinal(bool);

#endif // GPRE_EXP_PROTO_H

