/*
 *	PROGRAM:		JRD Access Method
 *	MODULE:			db_alias.cpp
 *	DESCRIPTION:	Server-side database aliases
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
 *
 * 2002.06.04 Dmitry Yemanov - Server-side database alias management.
 */

#if defined(_MSC_VER) && _MSC_VER < 1300
// Any Microsoft compiler before MSVC7
#pragma warning(disable: 4786)
#endif

#include <algorithm>

#include "../fbutil/FirebirdConfig.h"
#include "../fbutil/FirebirdConfigFile.h"
#include "../jrd/os/path_utils.h"

extern "C" {

bool ResolveDatabaseAlias(const char* alias, char* database)
{
	static FirebirdConfigFile aliasConfig;
	aliasConfig.setConfigFile(FirebirdConfig::getSysString("RootDirectory") + "/aliases.conf");

	const char correct_dir_sep = PathUtils::dir_sep;
	const char incorrect_dir_sep = (correct_dir_sep == '/') ? '\\' : '/';
	Firebird::string corrected_alias = alias;
	std::replace(corrected_alias.begin(), corrected_alias.end(), incorrect_dir_sep, correct_dir_sep);
	
	Firebird::string value = aliasConfig.getString(corrected_alias);

	if (!value.empty())
	{
		std::replace(value.begin(), value.end(), incorrect_dir_sep, correct_dir_sep);
		strcpy(database, value.c_str());
		return true;
	}
	
	return false;
}

}
