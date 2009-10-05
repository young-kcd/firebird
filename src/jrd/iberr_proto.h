/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		iberr_proto.h
 *	DESCRIPTION:	Prototype header file for iberr.c
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

#ifndef _JRD_IBERR_PROTO_H_
#define _JRD_IBERR_PROTO_H_

extern void IBERR_append_status(ISC_STATUS *, ISC_STATUS, ...);
extern void IBERR_build_status(ISC_STATUS *, ISC_STATUS, ...);
extern void IBERR_bugcheck(ISC_STATUS *, SCHAR *, UCHAR *, int, TEXT *);
extern void IBERR_error(ISC_STATUS *, SCHAR *, UCHAR *, int, TEXT *);

#endif /* _JRD_IBERR_PROTO_H_ */
