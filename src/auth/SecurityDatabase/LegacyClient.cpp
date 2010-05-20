/*
 *	PROGRAM:		Firebird authentication
 *	MODULE:			LegacyClient.cpp
 *	DESCRIPTION:	Performs legacy actions on DPB at client side.
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
#include "../jrd/ibase.h"
#include "../auth/SecurityDatabase/LegacyClient.h"

namespace Auth {

ClientInstance* SecurityDatabaseClient::instance()
{
	return Firebird::interfaceAlloc<SecurityDatabaseClientInstance>();
}

void SecurityDatabaseClient::getName(const char** data, unsigned short* dataSize)
{
	// Do not violate the constness.
	static char name[] = "LEGACY_AUTH";
	*data = name;
	*dataSize = strlen(name);
}

void SecurityDatabaseClient::release()
{
	interfaceFree(this);
}

Result SecurityDatabaseClientInstance::startAuthentication(bool, const char*, DpbInterface* dpb)
{
	return dpb->find(isc_dpb_user_name) &&
		(dpb->find(isc_dpb_password) || dpb->find(isc_dpb_password_enc)) ?
			AUTH_SUCCESS : AUTH_CONTINUE;
}

Result SecurityDatabaseClientInstance::contAuthentication(const unsigned char*, unsigned int)
{
	return AUTH_FAILED;
}

void SecurityDatabaseClientInstance::getData(const unsigned char**, unsigned short* dataSize)
{
	*dataSize = 0;
}

void SecurityDatabaseClientInstance::release()
{
	interfaceFree(this);
}

} // namespace Auth
