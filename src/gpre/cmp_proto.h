/*
 *	PROGRAM:	Preprocessor
 *	MODULE:		cmp_proto.h
 *	DESCRIPTION:	Prototype header file for cmp.c
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

#ifndef GPRE_CMP_PROTO_H
#define GPRE_CMP_PROTO_H

void	CMP_check(GPRE_REQ, SSHORT);
void	CMP_compile_request(GPRE_REQ);
void	CMP_external_field(GPRE_REQ, GPRE_FLD);
void	CMP_init(void);
USHORT	CMP_next_ident(void);
void	CMP_stuff_symbol(GPRE_REQ, SYM);
void	CMP_t_start(GPRE_TRA);

#endif // GPRE_CMP_PROTO_H
