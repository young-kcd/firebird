/*
 *	PROGRAM:		Firebird authentication
 *	MODULE:			LegacyClient.cpp
 *	DESCRIPTION:	Performs legacy actions on password at client side.
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
#include "../auth/SecurityDatabase/LegacyHash.h"
#include "../common/enc_proto.h"
#include "../common/classes/ImplementHelper.h"
#include "../common/classes/init.h"

namespace Auth {

Result SecurityDatabaseClient::authenticate(Firebird::IStatus*, IClientBlock* cb)
{
	if (!(cb->getLogin() && cb->getPassword()))
	{
		return AUTH_CONTINUE;
	}

	TEXT pwt[Auth::MAX_LEGACY_PASSWORD_LENGTH + 2];
	ENC_crypt(pwt, sizeof pwt, cb->getPassword(), Auth::LEGACY_PASSWORD_SALT);
	cb->putData(strlen(&pwt[2]), &pwt[2]);

	return AUTH_SUCCESS;
}

Result SecurityDatabaseClient::getSessionKey(Firebird::IStatus*,
								 const unsigned char** key, unsigned int* keyLen)
{
	*key = NULL;
	*keyLen = 0;
	return AUTH_CONTINUE;
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
