/*
 *	PROGRAM:	Preprocessor
 *	MODULE:		gpre_proto.h
 *	DESCRIPTION:	Prototype header file for gpre.cpp
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

#ifndef GPRE_GPRE_PROTO_H
#define GPRE_GPRE_PROTO_H

#include "../gpre/parse.h"

void	CPR_abort(void);
#ifdef DEV_BUILD
void	CPR_assert(TEXT*, int);
#endif
void	CPR_bugcheck(TEXT*);
void	CPR_end_text(TXT);
int		CPR_error(const TEXT*);
void	CPR_exit(int);
void	CPR_warn(const TEXT*);
TOK		CPR_eol_token(void);
void	CPR_get_text(TEXT*, TXT);
void	CPR_raw_read(void);
void	CPR_s_error(TEXT*);
TXT		CPR_start_text(void);
TOK		CPR_token(void);

#endif // GPRE_GPRE_PROTO_H

