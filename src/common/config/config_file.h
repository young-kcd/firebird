/*
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
 */

#ifndef CONFIG_CONFIG_FILE_H
#define CONFIG_CONFIG_FILE_H

#include <functional>
#include <map>

#include "../../common/classes/alloc.h"
#include "../../common/classes/fb_pair.h"
#include "../../common/classes/objects_array.h"
#include "../common/classes/fb_string.h"

/**
	Since the original (isc.cpp) code wasn't able to provide powerful and
	easy-to-use abilities to work with complex configurations, a decision
	has been made to create a completely new one.

	The below class implements generic file abstraction for new configuration
	manager. It allows "value-by-key" retrieval based on plain text files. Both
	keys and values are just strings that may be handled case-sensitively or
	case-insensitively, depending on host OS. The configuration file is loaded
	on demand, its current status can be checked with isLoaded() member function.
	All leading and trailing spaces are ignored. Comments (follow after a
	hash-mark) are ignored as well.

	Now this implementation is used by generic configuration manager
	(common/config/config.cpp) and server-side alias manager (jrd/db_alias.cpp).
**/

class ConfigFile : public Firebird::AutoStorage
{
	// config_file works with OS case-sensitivity
	typedef Firebird::PathName string;

	typedef Firebird::Pair<Firebird::Full<string, string> > Parameter;

    typedef Firebird::SortedObjectsArray <Parameter, 
		Firebird::InlineStorage<Parameter *, 100>,
		string, Firebird::FirstPointerKey<Parameter> > mymap_t;

public:
	ConfigFile(MemoryPool& p, bool ExceptionOnError) 
		: AutoStorage(p), isLoadedFlg(false), 
		  fExceptionOnError(ExceptionOnError), parameters(getPool()) {}
    explicit ConfigFile(bool ExceptionOnError) 
		: AutoStorage(), isLoadedFlg(false), 
		  fExceptionOnError(ExceptionOnError), parameters(getPool()) {}

	// configuration file management
    const string getConfigFile() { return configFile; }
    void setConfigFile(const string& newFile) { configFile = newFile; }

    bool isLoaded() const { return isLoadedFlg; }

    void loadConfig();
    void checkLoadConfig();

	// key and value management
    bool doesKeyExist(const string&);
    string getString(const string&);

	// utilities
	static void stripComments(string&);
	static string parseKeyFrom(const string&, string::size_type&);
	static string parseValueFrom(string, string::size_type);

private:
    string configFile;
    bool isLoadedFlg;
	bool fExceptionOnError;
    mymap_t parameters;
};

#endif	// CONFIG_CONFIG_FILE_H
