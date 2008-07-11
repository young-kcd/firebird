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
 *  Adriano dos Santos Fernandes
 */

#include "firebird.h"

#include <windows.h>

#include "fb_types.h"
#include "../common/classes/fb_string.h"

#include "../jrd/os/config_root.h"
#include "../utilities/install/registry.h"
#include "../utilities/install/install_nt.h"

typedef Firebird::PathName string;

/******************************************************************************
 *
 *	Platform-specific root locator
 */
namespace {

bool getRootFromRegistry(string& root)
{
	HKEY hkey;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_KEY_ROOT_INSTANCES,
		0, KEY_QUERY_VALUE, &hkey) != ERROR_SUCCESS)
	{
		return false;
	}

	DWORD bufsize = MAXPATHLEN;
	char buffer[MAXPATHLEN];
	DWORD type;
	const long RegRC = RegQueryValueEx(hkey, FB_DEFAULT_INSTANCE,
		NULL, &type, reinterpret_cast<UCHAR*>(buffer), &bufsize);
	RegCloseKey(hkey);
	if (RegRC == ERROR_SUCCESS) {
		root = buffer;
		return true;
	}
	return false;
}

} // namespace


bool ConfigRoot::initialized = false;
Firebird::InitInstance<string> ConfigRoot::install_dir;


void ConfigRoot::osConfigRoot()
{
	initialized = true;

	// check the registry first
#if defined(SUPERCLIENT)
	if (getRootFromRegistry(root_dir))
	{
		addSlash();
		return;
	}
#endif

	if (install_dir().isEmpty())
	{
		// get the pathname of the running executable
		string bin_dir = fb_utils::get_process_name();
		if (bin_dir.length() != 0)
		{
			// get rid of the filename
			const size_t index = bin_dir.rfind(PathUtils::dir_sep);
			install_dir() = bin_dir.substr(0, index);
		}
	}

	if (install_dir().hasData())
	{
		// how should we decide to use bin_dir instead of root_dir? any ideas?
		// ???
#if defined(EMBEDDED)
		root_dir = install_dir() + PathUtils::dir_sep;
		return;
#endif

		// go to the parent directory
		const size_t index = install_dir().rfind(PathUtils::dir_sep, install_dir().length());
		root_dir = (index ? install_dir().substr(0, index) : install_dir()) + PathUtils::dir_sep;
		return;
	}

	// As a last resort get it from the default install directory
	root_dir = FB_PREFIX;
}
