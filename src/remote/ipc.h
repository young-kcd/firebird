/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		ipc.h
 *	DESCRIPTION:	MPE/XL specific NetIPC
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

#ifndef _REMOTE_IPC_H_
#define _REMOTE_IPC_H_

#pragma intrinsic	IPCCREATE, IPCCONTROL, IPCNAME, IPCRECVCN
#pragma intrinsic	IPCGIVE, IPCGET
#pragma intrinsic	IPCSEND, IPCRECV, IPCERRMSG, IPCCHECK
#pragma intrinsic	IPCLOOKUP, IPCCONNECT, IPCDEST, IPCSHUTDOWN
#pragma intrinsic	INITOPT, ADDOPT, OPTOVERHEAD

#define IPC_CALL_SOCKET	3
#define IPC_TCP		4
#define IPC_GIVELEN	8

#define IPC_PREVIEWFLAG	2
#define IPC_LOCALFLAGS	32768

#define IPCCTL_TIMEOUT		3
#define IPCCTL_NOWAIT_RECV	256
#define IPCCTL_ADDRESS		514

#define IPCOPT_PROTOCOL		144
#define IPCOPT_ADDRESS		128
#define IPCOPT_MAX_RECV_SIZE	4
#define IPCOPT_MAX_SEND_SIZE	3

#define IPCSND_URGENTFLAG	16

#define IPCERR_TIMEOUT	59

#endif /* _REMOTE_IPC_H_ */
