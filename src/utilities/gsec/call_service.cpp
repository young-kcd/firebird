/*
 *	PROGRAM:	Security data base manager
 *	MODULE:		call_service.cpp
 *	DESCRIPTION:	Invokes remote service manager to work with security DB.
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Alexander Peshkoff (peshkoff@mail.ru)
 *  for the Firebird Open Source RDBMS project.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../jrd/common.h"
#include "../utilities/gsec/call_service.h"
#include <string.h>
#include <stdio.h>

const size_t SERVICE_SIZE = 256;
const size_t SERVER_PART = 200;

// Replaces the original ugly macro. Now the function that calls isValidServer
// is responsible for returning NULL to its invoker in turn.
// It simply makes sure there's something in the string containing the
// server's name; otherwise it fills the status vector with an error.
bool isValidServer(ISC_STATUS* status, const TEXT* server)
{
	if (!server || !*server)
	{
	    status[0] = isc_arg_gds;
	    status[1] = isc_bad_protocol;
	    status[2] = isc_arg_end;
	    return false;
	}
	return true;
}

static int typeBuffer(ISC_STATUS*, char*, int, internal_user_data&,
					   FPTR_SECURITY_CALLBACK, void*);

inline void stuffSpbByte(char*& spb, char data)
{
	*spb++ = data;
}

inline void stuffSpbShort(char*& spb, short data)
{
	ADD_SPB_LENGTH(spb, data);
}

inline void stuffSpbLong(char*& spb, long data)
{
	ADD_SPB_NUMERIC(spb, data);
}

static void stuffSpb(char*& spb, char param, const TEXT* value)
{
	stuffSpbByte(spb, param);
	int l = strlen(value);
	fb_assert(l < 256);
	stuffSpbByte(spb, char(l));
	memcpy(spb, value, l);
	spb += l;
}

static void stuffSpb2(char*& spb, char param, const TEXT* value)
{
	stuffSpbByte(spb, param);
	int l = strlen(value);
	stuffSpbShort(spb, short(l));
	memcpy(spb, value, l);
	spb += l;
}


isc_svc_handle attachRemoteServiceManager(ISC_STATUS* status,
							  const TEXT* username,
							  const TEXT* password, 
							  int protocol, 
							  const TEXT* server)
{
/**************************************
 *
 *	a t t a c h R e m o t e S e r v i c e M a n a g e r
 *
 **************************************
 *
 * Functional description
 *	Opens connection with service manager 
 *	on server using protocol, login username/password
 *
 **************************************/
	char service[SERVICE_SIZE];

	switch (protocol) {
	case sec_protocol_tcpip:
		if (! isValidServer(status, server))
		{
			return 0;
		}
		strncpy(service, server, SERVER_PART);
		strcat(service, ":");
		break;

	case sec_protocol_netbeui:
		if (! isValidServer(status, server))
		{
			return 0;
		}
		strcpy(service, "\\\\");
		strncat(service, server, SERVER_PART);
		strcat(service, "\\");
		break;

	case sec_protocol_local:
		break;

	default:
	    isValidServer(status, 0); // let it to set the error status
		return 0;
	}

	return attachRemoteServiceManager(status, username, password, service);
}


isc_svc_handle attachRemoteServiceManager(ISC_STATUS* status,
							  const TEXT* username,
							  const TEXT* password, 
							  const TEXT* server)
{
/**************************************
 *
 *	a t t a c h R e m o t e S e r v i c e M a n a g e r
 *
 **************************************
 *
 * Functional description
 *	Opens connection with service manager 
 *	on server using protocol, login username/password
 *
 **************************************/
	char service[SERVICE_SIZE];
	strncpy(service, server, SERVER_PART);
	strcat(service, "service_mgr");

	char spb_buffer[1024];
	char* spb = spb_buffer;
	stuffSpbByte(spb, isc_spb_version);
	stuffSpbByte(spb, isc_spb_current_version);
	if (username && password)
	{
		stuffSpb(spb, isc_spb_user_name, username);
		stuffSpb(spb, isc_spb_password, password);
	}

	fb_assert(spb - spb_buffer <= sizeof(spb_buffer));
	isc_svc_handle svc_handle = 0;
	isc_service_attach(status, 
		static_cast<USHORT>(strlen(service)),
		service, &svc_handle, 
		static_cast<USHORT>(spb - spb_buffer), 
		spb_buffer);
	return status[1] ? 0 : svc_handle;
}


static void userInfoToSpb(char*& spb, 
						  const internal_user_data& userInfo)
{
	stuffSpb2(spb, isc_spb_sec_username, userInfo.user_name);
	if (userInfo.uid_entered) {
		stuffSpbByte(spb, isc_spb_sec_userid);
		stuffSpbLong(spb, userInfo.uid);
	}
	if (userInfo.gid_entered) {
		stuffSpbByte(spb, isc_spb_sec_groupid);
		stuffSpbLong(spb, userInfo.gid);
	}
	if (userInfo.group_name_entered) {
		stuffSpb2(spb, isc_spb_sec_groupname, userInfo.group_name);
	}
	if (userInfo.password_entered) {
		stuffSpb2(spb, isc_spb_sec_password, userInfo.password);
	}
	if (userInfo.first_name_entered) {
		stuffSpb2(spb, isc_spb_sec_firstname, userInfo.first_name);
	}
	if (userInfo.middle_name_entered) {
		stuffSpb2(spb, isc_spb_sec_middlename, userInfo.middle_name);
	}
	if (userInfo.last_name_entered) {
		stuffSpb2(spb, isc_spb_sec_lastname, userInfo.last_name);
	}
}


void callRemoteServiceManager(ISC_STATUS* status, 
							  isc_svc_handle handle, 
							  const internal_user_data& userInfo,
							  FPTR_SECURITY_CALLBACK outputFunction,
							  void* functionArg)
{
/**************************************
 *
 *	c a l l R e m o t e S e r v i c e M a n a g e r
 *
 **************************************
 *
 * Functional description
 *	Calls service manager to execute command,
 *	specified in userInfo
 *
 **************************************/
	char spb_buffer[1024];
	char* spb = spb_buffer;
	if (userInfo.operation != DIS_OPER && !userInfo.user_name_entered)
	{
	    status[0] = isc_arg_gds;
	    status[1] = isc_gsec_switches_error;
	    status[2] = isc_arg_end;
		return;
	}

	switch (userInfo.operation) 
	{
	case ADD_OPER:
		stuffSpbByte(spb, isc_action_svc_add_user);
		userInfoToSpb(spb, userInfo);
		break;

	case MOD_OPER:
		stuffSpbByte(spb, isc_action_svc_modify_user);
		userInfoToSpb(spb, userInfo);
		break;

	case DEL_OPER:
		stuffSpbByte(spb, isc_action_svc_delete_user);
		stuffSpb2(spb, isc_spb_sec_username, userInfo.user_name);
		break;

	case DIS_OPER:
		stuffSpbByte(spb, isc_action_svc_display_user);
		if (userInfo.user_name_entered)
		{
			stuffSpb2(spb, isc_spb_sec_username, userInfo.user_name);
		}
		break;

	default:
	    status[0] = isc_arg_gds;
	    status[1] = isc_gsec_switches_error;
	    status[2] = isc_arg_end;
		return;
	}

	fb_assert(spb - spb_buffer <= sizeof(spb_buffer));
	isc_service_start(status, &handle, 0, 
		static_cast<USHORT>(spb - spb_buffer), spb_buffer);
	if (status[1] || userInfo.operation != DIS_OPER || !outputFunction)
	{
		return;
	}

	spb = spb_buffer;
	stuffSpbByte(spb, isc_info_svc_timeout);
	stuffSpbLong(spb, 10);
	char request = isc_info_svc_get_users;
	const int resultBufSize = 512;
	char resultBuffer[resultBufSize];
	int startQuery = 0;
	internal_user_data uData;
	memset(&uData, 0, sizeof uData);
	uData.user_name_entered = false;

	for (;;)
	{
		long reserved = 0;
		isc_service_query(status, &handle, &reserved, spb - spb_buffer, 
			spb, 1, &request, resultBufSize - startQuery, 
			&resultBuffer[startQuery]);
		if (status[1])
		{
			return;
		}
		startQuery = typeBuffer(status, resultBuffer, startQuery, uData, 
			outputFunction, functionArg);
		if (startQuery < 0)
		{
			break;
		}
	}

	if (uData.user_name[0])
	{
		outputFunction(functionArg, &uData, !uData.user_name_entered);
	}
}


void detachRemoteServiceManager(ISC_STATUS* status, 
							    isc_svc_handle handle)
{
/**************************************
 *
 *	d e t a c h R e m o t e S e r v i c e M a n a g e r
 *
 **************************************
 *
 * Functional description
 *	Close service manager.
 *
 **************************************/
	isc_service_detach(status, &handle);
}


void getSecurityDatabasePath(const TEXT* server, TEXT* buffer, size_t bufSize)
{
/**************************************
 *
 *	g e t S e c u r i t y D a t a b a s e P a t h
 *
 **************************************
 *
 * Functional description
 *	Gets the path to the security database
 *	from server	using protocol given in
 *	server name (like server: for TCP/IP).
 *
 **************************************/

/* Whatever is defined for a given platform as a name for
   the security database is used. */

	TEXT filenameBuffer[MAXPATHLEN];
	SecurityDatabase::getPath(filenameBuffer);
	strncpy(buffer, filenameBuffer, bufSize);
	buffer[bufSize - 1] = 0;
}


static void parseString2(const char*& p, char* buffer, size_t bufSize, int& loop)
{
	const size_t len = static_cast<size_t>(isc_vax_integer(p, sizeof(USHORT)));

	size_t len2 = len + sizeof(ISC_USHORT) + 1;
	if (len2 > loop)
	{
		throw loop;
	}
	loop -= len2;

	bufSize--;
	len2 = len < bufSize ? len : bufSize;
	p += sizeof(USHORT);
	memcpy (buffer, p, len2);
	buffer[len2] = 0;
	p += len;
}


static void parseLong(const char*& p, int& ul, int& loop)
{
	ul = isc_vax_integer(p, sizeof(ULONG));
	
	const size_t len2 = sizeof(ULONG) + 1;
	if (len2 > loop)
	{
		throw loop;
	}
	loop -= len2;

	p += sizeof(ULONG);
}


static int typeBuffer(ISC_STATUS* status, char* buf, int offset,
					   internal_user_data& uData,
					   FPTR_SECURITY_CALLBACK outputFunction,
					   void* functionArg)
{
/**************************************
 *
 *	t y p e B u f f e r
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	const char* p = &buf[offset];

	// Sanity checks
	if (*p++ != isc_info_svc_get_users)
	{
	    status[0] = isc_arg_gds;
	    status[1] = isc_gsec_params_not_allowed;
		status[2] = isc_arg_end;
		return -1;
	}
	int loop = isc_vax_integer (p, sizeof (USHORT));
	p += sizeof (USHORT);
    if (p[loop] != isc_info_end)
	{
	    status[0] = isc_arg_gds;
	    status[1] = isc_gsec_params_not_allowed;
		status[2] = isc_arg_end;
		return -1;
	}

	// No data - stop processing
	if (! loop)
	{
		return -1;
	}

	// Old data left - use them
	if (offset)
	{
		memmove(&buf[offset], p, loop);
		p = buf;
		offset = 0;
	}

	while (*p != isc_info_end)
	{
	  try {
		switch (*p++)
		{
		case isc_spb_sec_username:
			if (uData.user_name[0]) 
			{
				outputFunction(functionArg, &uData, !uData.user_name_entered);
				memset(&uData, 0, sizeof uData);
				uData.user_name_entered = true;
			}
			parseString2(p, uData.user_name, sizeof(uData.user_name), loop);
			break;
		case isc_spb_sec_firstname:
			parseString2(p, uData.first_name, sizeof(uData.first_name), loop);
			break;
		case isc_spb_sec_middlename:
			parseString2(p, uData.middle_name, sizeof(uData.middle_name), loop);
			break;
		case isc_spb_sec_lastname:
			parseString2(p, uData.last_name, sizeof(uData.last_name), loop);
			break;
		case isc_spb_sec_groupid:
			parseLong(p, uData.gid, loop);
			break;
		case isc_spb_sec_userid:
			parseLong(p, uData.uid, loop);
			break;
		default:
		    status[0] = isc_arg_gds;
		    status[1] = isc_gsec_params_not_allowed;
			status[2] = isc_arg_end;
			return true;
		}
	  }
	  catch(int newOffset)
	  {
		memmove(buf, --p, newOffset);
		return newOffset;
	  }
	}
	fb_assert(loop == 0);
	return 0;
}
