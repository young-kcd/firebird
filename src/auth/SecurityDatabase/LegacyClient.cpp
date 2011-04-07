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
#include "../common/classes/ImplementHelper.h"
#include "../common/classes/init.h"

namespace Auth {

Result SecurityDatabaseClient::startAuthentication(Firebird::IStatus*, bool, const char*, DpbInterface* dpb)
{
	return dpb->find(isc_dpb_user_name) &&
		(dpb->find(isc_dpb_password) || dpb->find(isc_dpb_password_enc)) ?
			AUTH_SUCCESS : AUTH_CONTINUE;
}

Result SecurityDatabaseClient::contAuthentication(Firebird::IStatus*, const unsigned char*, unsigned int)
{
	return AUTH_FAILED;
}

void SecurityDatabaseClient::getData(const unsigned char**, unsigned short* dataSize)
{
	*dataSize = 0;
}

int SecurityDatabaseClient::release()
{
	if (--refCounter == 0)
	{
		delete this;
		return 0;
	}
	return 1;
}

namespace {
	Firebird::SimpleFactory<SecurityDatabaseClient> factory;
}

void registerLegacyClient(Firebird::IPluginManager* iPlugin)
{
	iPlugin->registerPluginFactory(Firebird::PluginType::AuthClient, "Legacy_Auth", &factory);
}

} // namespace Auth
