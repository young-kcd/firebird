/*
 *	PROGRAM:	Preprocessor
 *	MODULE:		lang_proto.h
 *	DESCRIPTION:	Prototype header file for all of the code generators
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

#ifndef GPRE_LANG_PROTO_H
#define GPRE_LANG_PROTO_H

extern void ADA_action(ACT, int);
extern void ADA_print_buffer(TEXT *, int);
extern int BAS_action(ACT, int);
extern void C_CXX_action(ACT, int);
extern void COB_action(ACT, int);
extern void COB_name_init(BOOLEAN);
extern void COB_print_buffer(TEXT *, BOOLEAN);
extern void FTN_action(ACT, int);
extern void FTN_fini(void);
extern void FTN_print_buffer(TEXT *);
extern void INT_action(ACT, int);
extern void INT_CXX_action(ACT, int);
extern void PAS_action(ACT, int);
extern int PLI_action(ACT, int);

#endif /* GPRE_LANG_PROTO_H */
