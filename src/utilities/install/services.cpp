/*
 *	PROGRAM:	Windows NT service control panel installation program
 *	MODULE:		services.c
 *	DESCRIPTION:	Functions which update the Windows service manager for IB
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
 *
 * 01-Feb-2002 Paul Reeves: Removed hard-coded registry path
 *
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <windows.h>
#include "../jrd/common.h"
#include "../jrd/license.h"
#include "../utilities/install/install_nt.h"
#include "../utilities/install/servi_proto.h"
#include "../utilities/install/registry.h"

/* Defines */
#define RUNAS_SERVICE " -s"


USHORT SERVICES_config(SC_HANDLE manager,
					   TEXT * service_name,
					   TEXT * display_name,
					   USHORT sw_mode,
					   USHORT(*err_handler)(SLONG, TEXT *, SC_HANDLE))
{
/**************************************
 *
 *	S E R V I C E S _ c o n f i g
 *
 **************************************
 *
 * Functional description
 *	Configure an installed service.
 *
 **************************************/
	HKEY hkey;
	SLONG status;
	TEXT *mode;

	if ((status = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
							   REG_KEY_ROOT_CUR_VER,
							   0, KEY_WRITE, &hkey)) != ERROR_SUCCESS) {
		return (*err_handler) (status, "RegOpenKeyEx", NULL);
	}

	switch (sw_mode) {
	case DEFAULT_CLIENT:
		mode = "";
		break;
	case NORMAL_PRIORITY:
		mode = "-r";
		break;
	case HIGH_PRIORITY:
		mode = "-b";
		break;
	}

	if ((status = RegSetValueEx(hkey, "DefaultClientMode", 0,
								REG_SZ, reinterpret_cast<UCHAR*>(mode),
								(DWORD) (strlen(mode) + 1))) !=
		ERROR_SUCCESS) {
		RegCloseKey(hkey);
		return (*err_handler) (status, "RegSetValueEx", NULL);
	}

	RegCloseKey(hkey);

	return FB_SUCCESS;
}


USHORT SERVICES_delete(SC_HANDLE manager,
					   TEXT * service_name,
					   TEXT * display_name,
					   USHORT(*err_handler)(SLONG, TEXT *, SC_HANDLE))
{
/**************************************
 *
 *	S E R V I C E S _ d e l e t e
 *
 **************************************
 *
 * Functional description
 *	delete installed service param from registry.
 *
 **************************************/
	HKEY hkey;
	SLONG status;
	TEXT *mode;

	mode = "";

	if ((status = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
							   REG_KEY_ROOT_CUR_VER,
							   0, KEY_WRITE, &hkey)) != ERROR_SUCCESS) {
		return (*err_handler) (status, "RegOpenKeyEx", NULL);
	}

	if ((status = RegSetValueEx(hkey, "DefaultClientMode", 0,
								REG_SZ, reinterpret_cast<UCHAR*>(mode),
								(DWORD) (strlen(mode) + 1))) !=
		ERROR_SUCCESS) {
		RegCloseKey(hkey);
		return (*err_handler) (status, "RegSetValueEx", NULL);
	}

	RegCloseKey(hkey);

	return FB_SUCCESS;
}


USHORT SERVICES_install(SC_HANDLE manager,
						TEXT * service_name,
						TEXT * display_name,
						TEXT * executable,
						TEXT * directory,
						TEXT * dependencies,
						USHORT sw_startup,
						TEXT * nt_user_name,
						TEXT * nt_user_password,
						USHORT(*err_handler)(SLONG, TEXT *, SC_HANDLE))
{
/**************************************
 *
 *	S E R V I C E S _ i n s t a l l
 *
 **************************************
 *
 * Functional description
 *	Install a service in the service control panel.
 *
 **************************************/
	SC_HANDLE service;
	TEXT path_name[260];
	USHORT len;
	DWORD errnum;
	DWORD dwServiceType;

	strcpy(path_name, directory);
	len = strlen(path_name);
	if (len && path_name[len - 1] != '/' && path_name[len - 1] != '\\') {
		path_name[len++] = '\\';
		path_name[len] = 0;
	}
	strcpy(path_name + len, executable);
	strcat(path_name, RUNAS_SERVICE);

	dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	if (nt_user_name) {
		if (! nt_user_password)
			nt_user_password = "";
	}
	else {
		dwServiceType |= SERVICE_INTERACTIVE_PROCESS;
	}

	service = CreateService(manager,
							service_name,
							display_name,
							SERVICE_ALL_ACCESS,
							dwServiceType,
							(sw_startup ==
							 STARTUP_DEMAND) ? SERVICE_DEMAND_START :
							SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
							path_name, NULL, NULL, dependencies, 
							nt_user_name, nt_user_password);

	if (service == NULL) {
		errnum = GetLastError();
		if (errnum == ERROR_DUP_NAME || errnum == ERROR_SERVICE_EXISTS)
			return IB_SERVICE_ALREADY_DEFINED;
		else
			return (*err_handler) (errnum, "CreateService", NULL);
	}

	CloseServiceHandle(service);

	return FB_SUCCESS;
}


USHORT SERVICES_remove(SC_HANDLE manager,
					   TEXT * service_name,
					   TEXT * display_name,
					   USHORT(*err_handler)(SLONG, TEXT *, SC_HANDLE))
{
/**************************************
 *
 *	S E R V I C E S _ r e m o v e
 *
 **************************************
 *
 * Functional description
 *	Remove a service from the service control panel.
 *
 **************************************/
	SC_HANDLE service;
	SERVICE_STATUS service_status;

	service = OpenService(manager, service_name, SERVICE_ALL_ACCESS);
	if (service == NULL)
		return (*err_handler) (GetLastError(), "OpenService", NULL);

	if (!QueryServiceStatus(service, &service_status)) {
		CloseServiceHandle(service);
		return (*err_handler) (GetLastError(), "QueryServiceStatus", service);
	}

	if (service_status.dwCurrentState != SERVICE_STOPPED) {
		CloseServiceHandle(service);
		return IB_SERVICE_RUNNING;
	}

	if (!DeleteService(service)) {
		CloseServiceHandle(service);
		return (*err_handler) (GetLastError(), "DeleteService", service);
	}

	CloseServiceHandle(service);

	return FB_SUCCESS;
}


USHORT SERVICES_start(SC_HANDLE manager,
					  TEXT * service_name,
					  TEXT * display_name,
					  USHORT sw_mode,
					  USHORT(*err_handler)(SLONG, TEXT *, SC_HANDLE))
{
/**************************************
 *
 *	S E R V I C E S _ s t a r t
 *
 **************************************
 *
 * Functional description
 *	Start an installed service.
 *
 **************************************/
	SC_HANDLE service;
	const TEXT *mode;

	service = OpenService(manager, service_name, SERVICE_ALL_ACCESS);
	if (service == NULL)
		return (*err_handler) (GetLastError(), "OpenService", NULL);

	switch (sw_mode) {
	case DEFAULT_CLIENT:
		mode = NULL;
		break;
	case NORMAL_PRIORITY:
		mode = "-r";
		break;
	case HIGH_PRIORITY:
		mode = "-b";
		break;
	}
	if (!StartService(service, (mode) ? 1 : 0, &mode)) {
		CloseServiceHandle(service);
		return (*err_handler) (GetLastError(), "StartService", service);
	}

	CloseServiceHandle(service);

	return FB_SUCCESS;
}


USHORT SERVICES_stop(SC_HANDLE manager,
					 TEXT * service_name,
					 TEXT * display_name,
					 USHORT(*err_handler)(SLONG, TEXT *, SC_HANDLE))
{
/**************************************
 *
 *	S E R V I C E S _ s t o p
 *
 **************************************
 *
 * Functional description
 *	Start a running service.
 *
 **************************************/
	SC_HANDLE service;
	SERVICE_STATUS service_status;
	DWORD errnum;

	service = OpenService(manager, service_name, SERVICE_ALL_ACCESS);
	if (service == NULL)
		return (*err_handler) (GetLastError(), "OpenService", NULL);

	if (!ControlService(service, SERVICE_CONTROL_STOP, &service_status)) {
		CloseServiceHandle(service);
		errnum = GetLastError();
		if (errnum == ERROR_SERVICE_NOT_ACTIVE)
			return FB_SUCCESS;
		else
			return (*err_handler) (errnum, "ControlService", service);
	}

/* Wait for the service to actually stop before returning. */

	do {
		if (!QueryServiceStatus(service, &service_status)) {
			CloseServiceHandle(service);
			return (*err_handler) (GetLastError(), "QueryServiceStatus",
								   service);
		}
	}
	while (service_status.dwCurrentState == SERVICE_STOP_PENDING);

	CloseServiceHandle(service);

	return FB_SUCCESS;
}
