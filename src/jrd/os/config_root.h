/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		config_root.h
 *	DESCRIPTION:	Configuration manager (platform specific)
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
 * Created by: Dmitry Yemanov <yemanov@yandex.ru>
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#ifndef CONFIG_ROOT_H
#define CONFIG_ROOT_H

#include "fb_types.h"
#include "../common/classes/fb_string.h"

#include "../jrd/os/path_utils.h"

static const char* CONFIG_FILE	= "firebird.conf";

/**
	Since the original (isc.cpp) code wasn't able to provide powerful and
	easy-to-use abilities to work with complex configurations, a decision
	has been made to create a completely new one.

	This class is used to determine root directory of the installation and
	to get a pathname for the main configuration file. You may ask what's
	treated as root directory. The implementation of this class uses the
	following algorithm (located in getRootDirectory() member function):

		1. Retrieve full path of the running executable.
		2. If we're either client library or embedded server, return this path.
		3. Otherwise, return parent directory of this path.

	Pathname of the configuration file is platform-specific and defined by
	getConfigFile() member function.
**/

class ConfigRoot : public Firebird::PermanentStorage
{
	// config_file works with OS case-sensitivity
	typedef Firebird::PathName string;

private:
	void GetRoot() {
#ifdef DEV_BUILD
		if (getRootFromEnvironment("FIREBIRD_DEV")) {
			return;
		}
#endif
		if (getRootFromEnvironment("FIREBIRD"))	{
			return;
		}
		osConfigRoot();
	}

public:
	ConfigRoot(MemoryPool& p) : PermanentStorage(p),
		root_dir(getPool()), config_file(getPool()) 
	{
		GetRoot();
		config_file = root_dir + string(CONFIG_FILE);
	}


	virtual ~ConfigRoot() {}

	const char *getRootDirectory() const {
		return root_dir.c_str();
	}


protected:
	const char *getConfigFile() const {
		return config_file.c_str();
	}

	
private:
	string root_dir, config_file;
	void addSlash() {
		if (root_dir.rfind(PathUtils::dir_sep) != root_dir.length() - 1)
		{
			root_dir += PathUtils::dir_sep;
		}
	}
	bool getRootFromEnvironment(const char* envName) {
		const char* envValue = getenv(envName);
		if (! envValue) {
			return false;
		}
		root_dir = envValue;
		addSlash();
		return true;
	}
	void osConfigRoot();

	// copy prohibition
	ConfigRoot(const ConfigRoot&);
	void operator=(const ConfigRoot&);
};

#endif // CONFIG_ROOT_H
