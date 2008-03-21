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
 *  The Original Code was created by Mark O'Donohue
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2002 Mark O'Donohue <skywalker@users.sourceforge.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *  25 May 2003 - Nickolay Samofatov: Fix several bugs that prevented
 *    compatibility with Kylix
 * 
 */

#include "firebird.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "fb_types.h"
#include "../common/classes/fb_string.h"

#include "../jrd/os/config_root.h"
#include "../jrd/os/path_utils.h"

typedef Firebird::PathName string;

/******************************************************************************
 *
 *	Platform-specific root locator
 */

#if defined SUPERSERVER || defined EMBEDDED
#ifdef HAVE__PROC_SELF_EXE
static string getExePathViaProcEntry() 
{
    char buffer[MAXPATHLEN];
    const int len = readlink("/proc/self/exe", buffer, sizeof(buffer));
	if (len >= 0) {
		buffer[len] = 0;
		return buffer;
	}
	return "";
}
#endif
#endif

/******************************************************************************
 *
 *	
 */

#if defined SUPERSERVER || defined EMBEDDED
static string getRootPathFromExePath()
{
#ifdef HAVE__PROC_SELF_EXE
	// get the pathname of the running executable
	string bin_dir = getExePathViaProcEntry();
	if (bin_dir.length() == 0) 
		return "";
	
	// get rid of the filename
	int index = bin_dir.rfind(PathUtils::dir_sep);
	bin_dir = bin_dir.substr(0, index);

	// how should we decide to use bin_dir instead of root_dir? any ideas?
	// ???
#ifdef EMBEDDED
	// Placed here in case we introduce embedded POSIX build
	root_dir = bin_dir + PathUtils::dir_sep;
	return;
#endif

	// go to the parent directory
	index = bin_dir.rfind(PathUtils::dir_sep, bin_dir.length());
	string root_dir = (index ? bin_dir.substr(0, index) : bin_dir) + PathUtils::dir_sep;
    return root_dir;
#else
	return "";
#endif
}
#endif

void ConfigRoot::osConfigRoot()
{
#if defined SUPERSERVER || defined EMBEDDED
	// Try getting the root path from the executable
	root_dir = getRootPathFromExePath();
    if (root_dir.length() != 0) {
        return;
    }
#endif

    // As a last resort get it from the default install directory
    root_dir = string(FB_PREFIX) + PathUtils::dir_sep;    
}

