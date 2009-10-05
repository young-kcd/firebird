/*
 *	PROGRAM:	Command line utilities
 *	MODULE:		cmd_util_proto.h
 *	DESCRIPTION:	Prototype header file for cmd_util.c
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

#ifndef _UTILITIES_CMD_UTIL_PROTO_H_
#define _UTILITIES_CMD_UTIL_PROTO_H_


#ifdef __cplusplus
extern "C" {
#endif

void CMD_UTIL_put_svc_status(ISC_STATUS* svc_status,
							 USHORT  facility,
							 USHORT  errcode,
							 USHORT arg1_t, const void* arg1,
							 USHORT arg2_t, const void* arg2,
							 USHORT arg3_t, const void* arg3,
							 USHORT arg4_t, const void* arg4,
							 USHORT arg5_t, const void* arg5);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif /* _UTILITIES_CMD_UTIL_PROTO_H_ */

