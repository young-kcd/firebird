/*
 *	PROGRAM:		Windows NT service control panel installation program
 *	MODULE:			services.c
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
#include <ntsecapi.h>
#include <aclapi.h>
#include "../jrd/common.h"
#include "../jrd/license.h"
#include "../utilities/install_nt.h"
#include "../utilities/servi_proto.h"
#include "../utilities/registry.h"

/* Defines */
#define RUNAS_SERVICE " -s"

/*
USHORT SERVICES_config(SC_HANDLE manager,
						TEXT * service_name,
						TEXT * display_name,
						USHORT sw_mode,
						USHORT(*err_handler)(SLONG, TEXT *, SC_HANDLE))
{
*/
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
/*
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
*/

/*
USHORT SERVICES_delete(SC_HANDLE manager,
					   TEXT * service_name,
					   TEXT * display_name,
					   USHORT(*err_handler)(SLONG, TEXT *, SC_HANDLE))
{
*/
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
/*
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
*/

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
	TEXT path_name[MAXPATHLEN];
	USHORT len;
	DWORD errnum;
	DWORD dwServiceType;

	strcpy(path_name, directory);
	len = strlen(path_name);
	if (len && path_name[len - 1] != '/' && path_name[len - 1] != '\\')
	{
		path_name[len++] = '\\';
		path_name[len] = 0;
	}

	strcpy(path_name + len, executable);
	strcat(path_name, ".exe");
	strcat(path_name, RUNAS_SERVICE);

	dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	if (nt_user_name != 0)
	{
		if (nt_user_password == 0)
			nt_user_password = "";
	}
	else
		dwServiceType |= SERVICE_INTERACTIVE_PROCESS;

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

	if (service == NULL)
	{
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

	if (!QueryServiceStatus(service, &service_status))
		return (*err_handler) (GetLastError(), "QueryServiceStatus", service);

	if (service_status.dwCurrentState != SERVICE_STOPPED)
	{
		CloseServiceHandle(service);
		return IB_SERVICE_RUNNING;
	}

	if (!DeleteService(service))
		return (*err_handler) (GetLastError(), "DeleteService", service);

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
	SERVICE_STATUS service_status;
	const TEXT *mode;
	DWORD errnum;

	service = OpenService(manager, service_name, SERVICE_ALL_ACCESS);
	if (service == NULL)
		return (*err_handler) (GetLastError(), "OpenService", NULL);

	switch (sw_mode)
	{
		case DEFAULT_PRIORITY:
			mode = NULL;
			break;
		case NORMAL_PRIORITY:
			mode = "-r";
			break;
		case HIGH_PRIORITY:
			mode = "-b";
			break;
	}

	if (!StartService(service, (mode) ? 1 : 0, &mode))
	{
		errnum = GetLastError();
		CloseServiceHandle(service);
		if (errnum == ERROR_SERVICE_ALREADY_RUNNING)
			return FB_SUCCESS;
		else
			return (*err_handler) (errnum, "StartService", NULL);
	}

	/* Wait for the service to actually start before returning. */
	do
	{
		if (!QueryServiceStatus(service, &service_status))
			return (*err_handler) (GetLastError(), "QueryServiceStatus", service);
		Sleep(100);	// Don't loop too quickly (would be useless)
	}
	while (service_status.dwCurrentState == SERVICE_START_PENDING);

	if (service_status.dwCurrentState != SERVICE_RUNNING)
		return (*err_handler) (0, "Service failed to complete its startup sequence.", service);

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

	if (!ControlService(service, SERVICE_CONTROL_STOP, &service_status))
	{
		errnum = GetLastError();
		CloseServiceHandle(service);
		if (errnum == ERROR_SERVICE_NOT_ACTIVE)
			return FB_SUCCESS;
		else
			return (*err_handler) (errnum, "ControlService", NULL);
	}

	/* Wait for the service to actually stop before returning. */
	do
	{
		if (!QueryServiceStatus(service, &service_status))
			return (*err_handler) (GetLastError(), "QueryServiceStatus", service);
		Sleep(100);	// Don't loop too quickly (would be useless)
	}
	while (service_status.dwCurrentState == SERVICE_STOP_PENDING);

	if (service_status.dwCurrentState != SERVICE_STOPPED)
		return (*err_handler) (0, "Service failed to complete its stop sequence", service);

	CloseServiceHandle(service);

	return FB_SUCCESS;
}

USHORT SERVICES_grant_logon_right(TEXT* account,
							USHORT(*err_handler)(SLONG, TEXT *, SC_HANDLE))
{
/***************************************************
 *
 * S E R V I C E _ g r a n t _ l o g o n _ r i g h t
 *
 ***************************************************
 *
 * Functional description
 *  Grants the "Log on as a service" right to account.
 *  This is a Windows NT, 2000, XP, 2003 security thing.
 *  To run a service under an account other than LocalSystem, the account
 *  must have this right. To succeed granting the right, the current user
 *  must be an Administrator.
 *  Returns FB_SUCCESS when actually granted the right.
 *  Returns FB_LOGON_SRVC_RIGHT_ALREADY_DEFINED if right was already granted
 *  to the user.
 *  Returns FB_FAILURE on any error.
 *
 *  OM - AUG 2003 - Initial implementation
 *  OM - SEP 2003 - Control flow revision, no functional change
 *
 ***************************************************/

	LSA_OBJECT_ATTRIBUTES ObjectAttributes;
	LSA_HANDLE PolicyHandle;
	PSID pSid;
	DWORD cbSid;
	TEXT *pDomain;
	DWORD cchDomain;
	SID_NAME_USE peUse;
	LSA_UNICODE_STRING PrivilegeString;
	PLSA_UNICODE_STRING UserRights;
	ULONG CountOfRights = 0;
	ULONG i;

	NTSTATUS lsaErr;
	
	// Open the policy on the local machine.
	ZeroMemory(&ObjectAttributes, sizeof(ObjectAttributes));
	if ((lsaErr = LsaOpenPolicy(NULL, &ObjectAttributes,
		POLICY_CREATE_ACCOUNT | POLICY_LOOKUP_NAMES, &PolicyHandle))
			!= (NTSTATUS)0)
	{
		return (*err_handler)(LsaNtStatusToWinError(lsaErr), "LsaOpenPolicy", NULL);
	}

	// Obtain the SID of the user/group.
	// First, dummy call to LookupAccountName to get the required buffer sizes.
	cbSid = cchDomain = 0;
	LookupAccountName(NULL, account, NULL, &cbSid, NULL, &cchDomain, &peUse);
	pSid = (PSID)LocalAlloc(LMEM_ZEROINIT, cbSid);
	if (pSid == 0)
	{	
		DWORD err = GetLastError();
		LsaClose(PolicyHandle);
		return (*err_handler)(err, "LocalAlloc(Sid)", NULL);
	}
	pDomain = (LPTSTR)LocalAlloc(LMEM_ZEROINIT, cchDomain);
	if (pDomain == 0)
	{
		DWORD err = GetLastError();
		LsaClose(PolicyHandle);
		LocalFree(pSid);
		return (*err_handler)(err, "LocalAlloc(Domain)", NULL);
	}
	// Now, really obtain the SID of the user/group.
	if (LookupAccountName(NULL, account, pSid, &cbSid,
			pDomain, &cchDomain, &peUse) == 0)
	{
		DWORD err = GetLastError();
		LsaClose(PolicyHandle);
		LocalFree(pSid);
		LocalFree(pDomain);
		return (*err_handler)(err, "LookupAccountName", NULL);
	}

		LsaEnumerateAccountRights(PolicyHandle, pSid, &UserRights, &CountOfRights);
		// Check if the seServiceLogonRight is already granted
		for (i = 0; i < CountOfRights; i++)
		{
			if (wcscmp(UserRights[i].Buffer, L"SeServiceLogonRight") == 0)
				break;
		}
		LsaFreeMemory(UserRights); // Don't leak
		if (CountOfRights == 0 || i == CountOfRights)
		{
			// Grant the SeServiceLogonRight to users represented by pSid.
			PrivilegeString.Buffer = L"SeServiceLogonRight";
			PrivilegeString.Length = (USHORT) 19 * sizeof(WCHAR); // 19 : char len of Buffer
			PrivilegeString.MaximumLength=(USHORT)(19 + 1) * sizeof(WCHAR);
			if ((lsaErr = LsaAddAccountRights(PolicyHandle, pSid, &PrivilegeString, 1))
				!= (NTSTATUS)0)
			{
				LsaClose(PolicyHandle);
				LocalFree(pSid);
				LocalFree(pDomain);
				return (*err_handler)(LsaNtStatusToWinError(lsaErr), "LsaAddAccountRights", NULL);
			}
		}
		else
		{
			LsaClose(PolicyHandle);
			LocalFree(pSid);
			LocalFree(pDomain);
			return FB_LOGON_SRVC_RIGHT_ALREADY_DEFINED;
	}
	
	LsaClose(PolicyHandle);
	LocalFree(pSid);
	LocalFree(pDomain);

	return FB_SUCCESS;
}


USHORT SERVICES_grant_access_rights(TEXT* service_name, TEXT* account,
	USHORT(*err_handler)(SLONG, TEXT *, SC_HANDLE))
{
/*********************************************************
 *
 * S E R V I C E S _ g r a n t _ a c c e s s _ r i g h t s
 *
 *********************************************************
 *
 * Functional description
 *
 * Grant access rights to service 'service_name' so that user 'account'
 * can control it (start, stop, query).
 * Intended to be called after SERVICES_install().
 * By doing so to the Firebird server service object, we can set the Guardian
 * to run as the same specific user, yet still be able to start the Firebird
 * server from the Guardian.
 * 'account' is of format : DOMAIN\User or SERVER\User.
 * Returns FB_SUCCESS or FB_FAILURE.
 *
 * OM - SEP 2003 - Initial implementation
 *
 *********************************************************/

	PACL pOldDACL = NULL;
	PACL pNewDACL = NULL;
	PSECURITY_DESCRIPTOR pSD = NULL;
	EXPLICIT_ACCESS ea;

	// Get Security Information on the service. Will of course fail if we're
	// not allowed to do this. Administrators should be allowed, by default.
	if (GetNamedSecurityInfo(service_name, SE_SERVICE,
		DACL_SECURITY_INFORMATION,
		NULL /*Owner Sid*/, NULL /*Group Sid*/,
		&pOldDACL, NULL /*Sacl*/, &pSD) != ERROR_SUCCESS)
	{
		return (*err_handler)(GetLastError(), "GetNamedSecurityInfo", NULL);
	}

	// Initialize an EXPLICIT_ACCESS structure.
	ZeroMemory(&ea, sizeof(ea));
	ea.grfAccessPermissions = GENERIC_READ|GENERIC_EXECUTE;
	ea.grfAccessMode = SET_ACCESS;
	ea.grfInheritance = NO_INHERITANCE;
	ea.Trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
	ea.Trustee.TrusteeForm = TRUSTEE_IS_NAME;
	ea.Trustee.TrusteeType = TRUSTEE_IS_USER;
	ea.Trustee.ptstrName = account;

	// Create a new DACL, adding this right to whatever exists.
	if (SetEntriesInAcl(1, &ea, pOldDACL, &pNewDACL) != ERROR_SUCCESS)
	{
		DWORD err = GetLastError();
		LocalFree(pSD);
		return (*err_handler)(err, "SetEntriesInAcl", NULL);
	}

	// Updates the new rights in the object
	if (SetNamedSecurityInfo(service_name, SE_SERVICE,
		DACL_SECURITY_INFORMATION,
		NULL /*Owner Sid*/, NULL /*Group Sid*/,
		pNewDACL, NULL /*Sacl*/) != ERROR_SUCCESS)
	{
		DWORD err = GetLastError();
		LocalFree(pSD);
		LocalFree(pNewDACL);
		return (*err_handler)(err, "SetNamedSecurityInfo", NULL);
	}

	return FB_SUCCESS;
}

//
// EOF
//
