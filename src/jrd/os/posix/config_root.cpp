/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		config_root.cpp
 *	DESCRIPTION:	Configuration manager (platform specific - linux/posix)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * You may obtain a copy of the Licence at
 * http://www.gnu.org/licences/lgpl.html
 * 
 * As a special exception this file can also be included in modules
 * with other source code as long as that source code has been 
 * released under an Open Source Initiative certificed licence.  
 * More information about OSI certification can be found at: 
 * http://www.opensource.org 
 * 
 * This module is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public Licence for more details.
 * 
 * This module was created by members of the firebird development 
 * team.  All individual contributions remain the Copyright (C) of 
 * those individuals and all rights are reserved.  Contributors to 
 * this file are either listed below or can be obtained from a CVS 
 * history command.
 *
 *  Created by:  Mark O'Donohue <skywalker@users.sourceforge.net>
 *
 *  Contributor(s):
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

