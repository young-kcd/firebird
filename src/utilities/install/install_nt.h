/*
 *	PROGRAM:	Windows NT installation utilities
 *	MODULE:		install.h
 *	DESCRIPTION:	Defines for Windows NT installation utilities
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

#ifndef _UTILITIES_INSTALL_NT_H_
#define _UTILITIES_INSTALL_NT_H_

#define REMOTE_SERVICE		"FirebirdServer"
#define REMOTE_DISPLAY_NAME	"Firebird Server"
#define REMOTE_SS_EXECUTABLE	"bin\\fbserver"
#define REMOTE_CS_EXECUTABLE	"bin\\fb_inet_server"
#define ISCGUARD_SERVICE	"FirebirdGuardian"
#define ISCGUARD_DISPLAY_NAME "Firebird Guardian"
#define ISCGUARD_EXECUTABLE	"bin\\fbguard"
#define GUARDIAN_MUTEX      "FirebirdGuardianMutex"
/* Starting with 128 the service prams are user defined */
#define SERVICE_CREATE_GUARDIAN_MUTEX 128
#define REMOTE_DEPENDENCIES	"Tcpip\0\0"

#define COMMAND_NONE		0
#define COMMAND_INSTALL		1
#define COMMAND_REMOVE		2
#define COMMAND_START		3
#define COMMAND_STOP		4
#define COMMAND_CONFIG		5

#define STARTUP_DEMAND		0
#define STARTUP_AUTO		1

#define NO_GUARDIAN			0
#define USE_GUARDIAN		1

#define DEFAULT_CLIENT		0
#define NORMAL_PRIORITY		1
#define HIGH_PRIORITY		2

#define ARCH_SS				0
#define ARCH_CS				1

#define IB_SERVICE_ALREADY_DEFINED	100
#define IB_SERVICE_RUNNING		101

#define IB_GUARDIAN_ALREADY_DEFINED  102
#define IB_GUARDIAN_RUNNING		103

#endif /* _UTILITIES_INSTALL_NT_H_ */
