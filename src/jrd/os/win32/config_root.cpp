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
 *  The Original Code was created by Dmitry Yemanov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2002 Dmitry Yemanov <dimitr@users.sf.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"

#include <windows.h>

#include "fb_types.h"
#include "fb_string.h"

#include "../jrd/os/config_root.h"
#include "../jrd/os/path_utils.h"
#include "../utilities/install/registry.h"

typedef Firebird::string string;

static const char *REGISTRY_KEY = "RootDirectory";
static const char *CONFIG_FILE = "firebird.conf";

/******************************************************************************
 *
 *	Platform-specific root locator
 */

void getRootFromRegistry(TEXT *buffer, DWORD buffer_length)
{
	HKEY hkey;
	DWORD type;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_KEY_ROOT_CUR_VER,
		0, KEY_QUERY_VALUE, &hkey) != ERROR_SUCCESS)
	{
		return;
	}

	RegQueryValueEx(hkey, REGISTRY_KEY, NULL, &type,
		reinterpret_cast<UCHAR*>(buffer), &buffer_length);
	RegCloseKey(hkey);
}

ConfigRoot::ConfigRoot()
{
	TEXT buffer[MAXPATHLEN];

	buffer[0] = 0;

	// check the registry first
#if !defined(EMBEDDED)
	getRootFromRegistry(buffer, sizeof(buffer));
#endif
	if (buffer[0])
	{
		root_dir = buffer;
		if (root_dir.rfind(PathUtils::dir_sep) != root_dir.length() - 1)
		{
			root_dir += PathUtils::dir_sep;
		}
		return;
	}

	// get the pathname of the running executable
	GetModuleFileName(NULL, buffer, sizeof(buffer));
	string bin_dir = buffer;
	
	// get rid of the filename
	int index = bin_dir.rfind(PathUtils::dir_sep);
	bin_dir = bin_dir.substr(0, index);

	// how should we decide to use bin_dir instead of root_dir? any ideas?
	// ???
#if defined(EMBEDDED)
	root_dir = bin_dir + PathUtils::dir_sep;
	return;
#endif

	// go to the parent directory
	index = bin_dir.rfind(PathUtils::dir_sep, bin_dir.length());
	root_dir = (index ? bin_dir.substr(0, index) : bin_dir) + PathUtils::dir_sep;
}

const char *ConfigRoot::getRootDirectory() const
{
	return root_dir.c_str();
}

const char *ConfigRoot::getConfigFile() const
{
	static string file = root_dir + string(CONFIG_FILE);
	return file.c_str();
}
