/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		ppg_proto.h
 *	DESCRIPTION:	Prototype header file for ppg.cpp
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

#ifndef UTILITIES_PPG_PROTO_H
#define UTILITIES_PPG_PROTO_H

#ifdef SUPERSERVER
void	PPG_print_header (const Ods::header_page*, SLONG, bool, Jrd::Service*);
#ifdef NOT_USED_OR_REPLACED
void	PPG_print_log (const Ods::log_info_page*, SLONG, bool, Jrd::Service*);
#endif
#else
void	PPG_print_header (const Ods::header_page*, SLONG, bool, FILE*);
#ifdef NOT_USED_OR_REPLACED
void	PPG_print_log (const Ods::log_info_page*, SLONG, bool, FILE*);
#endif
#endif

#endif // UTILITIES_PPG_PROTO_H

