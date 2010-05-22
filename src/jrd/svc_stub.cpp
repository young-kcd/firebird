/*
 *	PROGRAM:		Firebird services stub
 *	MODULE:			svc_stub.cpp
 *	DESCRIPTION:	Replacement of real entries for boot build (static library)
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
 *  The Original Code was created by Alex Peshkov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2010 Alex Peshkov <peshkoff at mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#include "firebird.h"
#include "gen/iberror.h"
#include "../utilities/gsec/call_service.h"
#include "../jrd/svc_tab.h"

using namespace Firebird;

namespace Jrd {

const serv_entry services[] =
{
	{ 0, NULL, NULL, NULL }
};

} //namespace Jrd

SSHORT SECURITY_exec_line(ISC_STATUS* isc_status,
                          FB_API_HANDLE DB,
                          FB_API_HANDLE trans,
                          internal_user_data* io_user_data,
                          FPTR_SECURITY_CALLBACK display_func,
                          void* callback_arg)
{
	(Arg::Gds(isc_random) << "Security is missing in current build").copyTo(isc_status);
	return -1;
}
