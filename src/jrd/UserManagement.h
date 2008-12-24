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

#ifndef JRD_USER_MANAGEMENT_H
#define JRD_USER_MANAGEMENT_H

#include "firebird.h"
#include "../jrd/ibase.h"

struct internal_user_data;

namespace Jrd {

class thread_db;

// User management argument for deferred work
class UserManagement
{
public:
	explicit UserManagement(thread_db* tdbb);
	~UserManagement();

private:
	FB_API_HANDLE database, transaction;

public:
	int execute(ISC_STATUS* status, internal_user_data* u);
	void commit();
};

}	// namespace

#endif // JRD_USER_MANAGEMENT_H
