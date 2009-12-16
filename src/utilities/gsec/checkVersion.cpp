/*
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
 *  The Original Code was created by Alexander Peshkoff
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2009 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 */


// =====================================
// Common utilities functions

#include "firebird.h"
#include "gen/iberror.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <ctype.h>

#include "../utilities/gsec/checkVersion.h"
#include "../common/classes/ClumpletWriter.h"

using namespace Firebird;

void checkServerUsersVersion(isc_svc_handle svc_handle, char& server_users)
{
	if (server_users)
	{
		return;
	}

	// use safe old value
	server_users = isc_action_svc_display_user;

	// query service
	ClumpletWriter spbItems(ClumpletWriter::SpbStart, 8);
	spbItems.insertTag(isc_info_svc_server_version);
	ISC_STATUS_ARRAY status;
	char results[1024];
	if (isc_service_query(status, &svc_handle, 0, 0, 0,
						  static_cast<USHORT>(spbItems.getBufferLength()),
						  reinterpret_cast<const char*>(spbItems.getBuffer()),
						  sizeof(results), results))
	{
		return;
	}

	// parse results
	const char* p = results;
	string version;
	while (*p != isc_info_end)
	{
		switch (*p++)
		{
		case isc_info_svc_server_version:
			{
				unsigned short length = (unsigned short) isc_vax_integer (p, sizeof(unsigned short));
				p += sizeof (unsigned short);
				version.assign(p, length);
				p += length;
			}
			break;
		default:
			return;
		}
	}

	// parse version
	size_t pos = version.find('-');
	if (pos == string::npos)
	{
		return;
	}
	version.erase(0, pos);
	for (pos = 0; pos < version.length(); ++pos)
	{
		if (isdigit(version[pos]))
		{
			version.erase(0, pos);
			double f = atof(version.c_str());
			if (f > 2.45)	// need 2.5, but take into an account it's float
			{
				server_users = isc_action_svc_display_user_a;
			}
			return;
		}
	}
}
