/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		ppg_proto.h
 *	DESCRIPTION:	Prototype header file for ppg.c
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

#ifndef _UTILITIES_PPG_PROTO_H_
#define _UTILITIES_PPG_PROTO_H_

#ifdef SUPERSERVER
extern void	PPG_print_header (HDR, SLONG, SVC);
extern void	PPG_print_log (LIP, SLONG, SVC);
#else
extern void	PPG_print_header (HDR, SLONG, IB_FILE*);
extern void	PPG_print_log (LIP, SLONG, IB_FILE*);
#endif

#endif /* _UTILITIES_PPG_PROTO_H_ */

