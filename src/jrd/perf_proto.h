/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		perf_proto.h
 *	DESCRIPTION:	Prototype header file for perf.c
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

#ifndef JRD_PERF_PROTO_H
#define JRD_PERF_PROTO_H

#include "../jrd/y_ref.h"

#ifdef __cplusplus
extern "C" {
#endif

int		API_ROUTINE perf_format(struct perf *, struct perf *, SCHAR *,
								   SCHAR *, SSHORT *);
void	API_ROUTINE perf_get_info(FRBRD **, struct perf *);
void	API_ROUTINE perf_report(struct perf *, struct perf *, SCHAR *,
									SSHORT *);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // JRD_PERF_PROTO_H
