/*
 *
 *	PROGRAM:	Security data base manager
 *	MODULE:		security.cpp
 *	DESCRIPTION:	Security routines
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
 * 					Alex Peshkoff
 */

#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../common/common.h"
#include "../jrd/ibase.h"
#include "../auth/SecurityDatabase/jrd_pwd.h"
#include "../common/enc_proto.h"
#include "../yvalve/gds_proto.h"
#include "../common/isc_proto.h"
#include "../common/os/mod_loader.h"
#include "../utilities/gsec/gsec.h"
#include "../utilities/gsec/secur_proto.h"
#include "../common/utils_proto.h"
#include "../common/classes/init.h"
#include "../common/classes/UserBlob.h"
#include "../common/classes/Interface.h"
#include "../common/utils_proto.h"

using namespace Firebird;

SSHORT SECURITY_exec_line(ISC_STATUS* isc_status,
						  const char* realUser,
						  FB_API_HANDLE db,
						  FB_API_HANDLE trans,
						  internal_user_data* io_user_data,
						  FPTR_SECURITY_CALLBACK display_func,
						  void* callback_arg)
{
	static Auth::ManagementPlugin* volatile plugin = NULL;

	if (!plugin)
	{
		static GlobalPtr<Mutex> mutex;
		MutexLockGuard g(mutex);

		if (!plugin)
		{
			// temporary measure before PluginManager integration
			PathName plugFile(fb_utils::getPrefix(fb_utils::FB_DIR_PLUGINS, "user_management"));
			ModuleLoader::doctorModuleExtension(plugFile);
			ModuleLoader::loadModule(plugFile);

			plugin = reinterpret_cast<Auth::ManagementPlugin*>(
				fb_query_plugin(Plugin::UserManagement, NULL));
		}

		if (!plugin)
		{
			(Arg::Gds(isc_random) << "Missing user management plugin").copyTo(isc_status);
			return GsecMsg75;
		}
	}

	return plugin->execLine(isc_status, realUser, db, trans, io_user_data, display_func, callback_arg);
}

SSHORT SECURITY_exec_line(ISC_STATUS* isc_status,
						  const char* realUser,
						  FB_API_HANDLE db,
						  internal_user_data* io_user_data,
						  FPTR_SECURITY_CALLBACK display_func,
						  void* callback_arg)
{
	FB_API_HANDLE tra = 0;

	if (isc_start_transaction(isc_status, &tra, 1, &db, 0, NULL))
	{
		return GsecMsg75;		// gsec error
	}

	SSHORT ret = SECURITY_exec_line(isc_status, realUser, db, tra, io_user_data, display_func, callback_arg);

	// rollback if we have an error using tmp_status to avoid
	// overwriting the error status which the caller wants to see

	ISC_STATUS_ARRAY tmp_status;

	if (ret)
	{
		isc_rollback_transaction(tmp_status, &tra);
	}
	else
	{
		if (isc_commit_transaction(isc_status, &tra))
		{
			// CVC: Shouldn't we call isc_rollback_transaction() here?
			isc_rollback_transaction(tmp_status, &tra);
			return GsecMsg75;	// gsec error
		}
	}

	return ret;
}
