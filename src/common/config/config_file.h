/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		config_file.h
 *	DESCRIPTION:	Configuration manager (file handling)
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
 * Created by: Mark O'Donohue <mark.odonohue@ludwig.edu.au>
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include <functional>
#include <map>

#include "../../common/classes/alloc.h"
#include "fb_string.h"

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

class ConfigFile
{
	typedef Firebird::string string;

	// used to provide proper filename handling in various OS
	class key_compare : public std::binary_function<const string&, const string&, bool>
	{
	public:
		key_compare() : isCaseSensitive(CASE_SENSITIVITY) {}
		bool operator()(const string&, const string&) const;
	private:
		bool isCaseSensitive;
		void uppercase(string&) const;
	};

    typedef std::map <string, string, key_compare,
		Firebird::allocator <std::pair <const string, string> > > mymap_t;

public:
    ConfigFile(bool ExitOnError) 
		: isLoadedFlg(false), fExitOnError(ExitOnError) {}

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
	static void stripLeadingWhiteSpace(string&);
	static void stripTrailingWhiteSpace(string&);
	static void stripComments(string&);
	static string parseKeyFrom(const string&, string::size_type&);
	static string parseValueFrom(string, string::size_type);

private:
    string configFile;
    bool isLoadedFlg;
	bool fExitOnError;
    mymap_t parameters;
};

#endif	// CONFIG_FILE_H
