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
#include "../common/classes/fb_string.h"

#include "../jrd/os/config_root.h"
#include "../utilities/install/registry.h"

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

void ConfigRoot::osConfigRoot()
{
	// check the registry first
#if !defined(EMBEDDED)
	if (getRootFromRegistry(root_dir))
	{
		addSlash();
		return;
	}
#endif

	// get the pathname of the running executable
	string bin_dir;
	{
		// Given the current semantics of PathName, when "buffer" goes
		// out of scope, it's already bitwise copied into bin_dir.
		char buffer[MAXPATHLEN];
		GetModuleFileName(NULL, buffer, sizeof(buffer));
		bin_dir = buffer;
	}
	
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

