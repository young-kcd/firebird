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
#include "../common/security.h"
#include "../common/StatusArg.h"

using namespace Firebird;

namespace {

void raise()
{
	(Arg::Gds(isc_random) << "Missing user management plugin").raise();
}

} // anonymous namespace

namespace Auth {

Get::Get(Config* firebirdConf)
	: GetPlugins<Auth::Management>(PluginType::AuthUserManagement, FB_AUTH_MANAGE_VERSION, firebirdConf)
{
	if (!hasData())
	{
		raise();
	}
}

void FB_CARG UserData::clear()
{
	op = trustedRole = trustedAuth = 0;
	user.clear();
	pass.clear();
	first.clear();
	last.clear();
	middle.clear();
	group.clear();
	u.clear();
	g.clear();
	adm.clear();

	database.clear();
	dba.clear();
	dbaPassword.clear();
	role.clear();
	trustedUser.clear();
}

} // namespace Auth
