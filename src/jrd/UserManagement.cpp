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
 *  The Original Code was created by Alex Peshkov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2008 Alex Peshkov <alexpeshkoff@users.sf.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../common/classes/ClumpletWriter.h"
#include "../jrd/UserManagement.h"
#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../jrd/jrd_pwd.h"
#include "../utilities/gsec/gsec.h"
#include "../utilities/gsec/secur_proto.h"

using namespace Jrd;
using namespace Firebird;


UserManagement::UserManagement(thread_db* tdbb)
	: database(0), transaction(0)
{
	char securityDatabaseName[MAXPATHLEN];
	SecurityDatabase::getPath(securityDatabaseName);
	ISC_STATUS_ARRAY status;

	ClumpletWriter dpb(ClumpletReader::Tagged, MAX_DPB_SIZE, isc_dpb_version1);
	dpb.insertByte(isc_dpb_gsec_attach, TRUE);
	dpb.insertString(isc_dpb_trusted_auth, tdbb->getAttachment()->att_user->usr_user_name);

	if (isc_attach_database(status, 0, securityDatabaseName, &database,
							dpb.getBufferLength(), reinterpret_cast<const char*>(dpb.getBuffer())))
	{
		status_exception::raise(status);
	}

	if (isc_start_transaction(status, &transaction, 1, &database, 0, NULL))
	{
		status_exception::raise(status);
	}
}

void UserManagement::commit()
{
	ISC_STATUS_ARRAY status;
	if (transaction)
	{
		// Commit transaction in security database
		if (isc_commit_transaction(status, &transaction))
		{
			status_exception::raise(status);
		}
		transaction = 0;
	}
}

UserManagement::~UserManagement()
{
	ISC_STATUS_ARRAY status;
	if (transaction)
	{
		// Rollback transaction in security database ...
		if (isc_rollback_transaction(status, &transaction))
		{
			status_exception::raise(status);
		}
	}

	if (database)
	{
		if (isc_detach_database(status, &database))
		{
			status_exception::raise(status);
		}
	}
}

int UserManagement::execute(ISC_STATUS* status, internal_user_data* u)
{
#if (defined BOOT_BUILD || defined EMBEDDED)
	status_exception::raise(Arg::Gds(isc_wish_list));
	return 0; // make the compiler happy
#else
	return (!u->user_name_entered) ? GsecMsg18 : SECURITY_exec_line(status, database, transaction, u, NULL, NULL);
#endif
}
