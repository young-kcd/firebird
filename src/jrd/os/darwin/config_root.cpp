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
 * 
 *
 *  $Id: config_root.cpp,v 1.1 2003-02-13 22:40:42 bellardo Exp $
 */

#include "firebird.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "fb_types.h"
#include "fb_string.h"

#include "../jrd/os/config_root.h"
#include "../jrd/os/path_utils.h"
#include "../jrd/file_params.h"

#include <CoreFoundation/CFBundle.h>
#include <CoreFoundation/CFURL.h>

typedef Firebird::string string;

static const char *CONFIG_FILE = "firebird.conf";

ConfigRoot::ConfigRoot()
{
	CFBundleRef fbFramework;
	CFURLRef	msgFileUrl;
	CFStringRef	msgFilePath;
	char file_buff[MAXPATHLEN];

	// Check the environment variable
	const char* envPath = getenv("FIREBIRD");
	if (envPath != NULL && strcmp("", envPath))
	{
		root_dir = envPath;
		return;
	}

	// Attempt to locate the Firebird.framework bundle
	if ((fbFramework = CFBundleGetBundleWithIdentifier(
			CFSTR(DARWIN_FRAMEWORK_ID)) ))
	if ((msgFileUrl = CFBundleCopyResourceURL( fbFramework,
			CFSTR(DARWIN_GEN_DIR), NULL, NULL)))
	if ((msgFilePath = CFURLCopyFileSystemPath(msgFileUrl,
			kCFURLPOSIXPathStyle)))
	if ((CFStringGetCString(msgFilePath, file_buff, MAXPATHLEN,
			kCFStringEncodingMacRoman )) )
	{
		root_dir = file_buff;
		return;
	}

	// As a last resort get it from the default install directory
	// this doesn't make much sense because the framework method should
	// never fail, but what the heck.
	root_dir = FB_PREFIX;
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
