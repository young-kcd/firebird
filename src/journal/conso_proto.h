/*
 *	PROGRAM:	JRD Journalling Subsystem
 *	MODULE:		conso_proto.h
 *	DESCRIPTION:	Prototype header file for console.c
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

#ifndef JOURNAL_CONSO_PROTO_H
#define JOURNAL_CONSO_PROTO_H

bool	CONSOLE_start_console(int, SCHAR **);
bool	CONSOLE_test_server(SCHAR *);

#endif // JOURNAL_CONSO_PROTO_H
