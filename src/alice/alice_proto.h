
/*
 *	PROGRAM:	Alice (All Else) Utility
 *	MODULE:		alice_proto.h
 *	DESCRIPTION:	Prototype header file for alice.cpp
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

#ifndef ALICE_ALICE_PROTO_H
#define ALICE_ALICE_PROTO_H

#ifdef SUPERSERVER
int ALICE_main(SVC service);
#endif

void	ALICE_down_case(const TEXT*, TEXT*, const size_t);
void	ALICE_print(USHORT, const TEXT*, const TEXT*, const TEXT*,
	const TEXT*, const TEXT*);
void	ALICE_error(USHORT, const TEXT*, const TEXT*, const TEXT*,
	const TEXT*, const TEXT*);
void	ALICE_print_status(const ISC_STATUS*);

#endif // ALICE_ALICE_PROTO_H

