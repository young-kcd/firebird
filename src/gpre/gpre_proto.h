/*
 *	PROGRAM:	Preprocessor
 *	MODULE:		gpre_proto.h
 *	DESCRIPTION:	Prototype header file for gpre.c
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

#ifndef _GPRE_GPRE_PROTO_H_
#define _GPRE_GPRE_PROTO_H_

#include "../gpre/parse.h"

extern void CPR_abort(void);
#ifdef DEV_BUILD
extern void CPR_assert(TEXT *, int);
#endif
extern void CPR_bugcheck(TEXT *);
extern void CPR_end_text(TXT);
extern int CPR_error(TEXT *);
extern void CPR_exit(int);
extern void CPR_warn(TEXT *);
extern TOK CPR_eol_token(void);
extern void CPR_get_text(TEXT *, TXT);
extern void CPR_raw_read(void);
extern void CPR_s_error(TEXT *);
extern TXT CPR_start_text(void);
extern TOK CPR_token(void);

#endif /* _GPRE_GPRE_PROTO_H_ */
