/*
 *	PROGRAM:		Firebird authentication
 *	MODULE:			LegacyManagement.h
 *	DESCRIPTION:	Performs legacy actions with security database
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
 */

#ifndef AUTH_LEGACY_MANAGEMENT_H
#define AUTH_LEGACY_MANAGEMENT_H

#include "../common/classes/ImplementHelper.h"
#include "../auth/AuthInterface.h"


namespace Auth {

class SecurityDatabaseManagement : public Firebird::StdPlugin<Management, FB_AUTH_MANAGE_VERSION>
{
public:
	explicit SecurityDatabaseManagement(Firebird::IFactoryParameter*)
	{
	}

	// work in progress - we must avoid both internal_user_data and callback function
	int execLine(ISC_STATUS* isc_status, const char* realUser,
				 FB_API_HANDLE db, FB_API_HANDLE trans,
				 internal_user_data* io_user_data,
				 FPTR_SECURITY_CALLBACK display_func, void* callback_arg);

	int release();
};

} // namespace Auth

#endif // AUTH_LEGACY_MANAGEMENT_H
