/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		config.cpp
 *	DESCRIPTION:	Configuration manager (generic code)
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

#include "firebird.h"

#ifdef _MSC_VER
#pragma warning (disable: 4786)	// debug identifier truncated
#endif

#include "../../common/config/config.h"
#include "../../common/config/config_impl.h"
#include "../../common/config/config_file.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include "../jrd/gdsassert.h"

typedef Firebird::string string;
typedef Firebird::vector<string> string_vector;

/******************************************************************************
 *
 *	Configuration keys (physical names)
 */

const string keys[] = {
	"RootDirectory",						// 0
	"SortMemBlockSize",						// 1
	"SortMemUpperLimit",					// 2
	"RemoteFileOpenAbility",				// 3
	"TempDirectories"						// 4
};

/******************************************************************************
 *
 *	Static instance of the system configuration file
 */

ConfigImpl& ConfigImpl::instance()
{
	static ConfigImpl sysConfig;
	return sysConfig;
}

/******************************************************************************
 *
 *	Implementation interface
 */

ConfigImpl::ConfigImpl()
{
	file.setConfigFile(getConfigFile());
	val_sep = ",";
}

void ConfigImpl::validateKey(ConfigKey key)
{
	assert(key >= 0 && key < (sizeof(keys) / sizeof(string)));
}

void ConfigImpl::getValue(ConfigKey key, string &result)
{
	validateKey(key);
	
	if (file.doesKeyExist(keys[key]))
	{
		 result = file.getString(keys[key]);
	}
}

void ConfigImpl::getValue(ConfigKey key, int &result)
{
	validateKey(key);
	
	if (file.doesKeyExist(keys[key]))
	{
		result = atoi(file.getString(keys[key]).data());
	}
}

void ConfigImpl::getValue(ConfigKey key, bool &result)
{
	validateKey(key);

	if (file.doesKeyExist(keys[key]))
	{
		result = (atoi(file.getString(keys[key]).data()) != 0);
	}
}

void ConfigImpl::getValueList(ConfigKey key, Firebird::vector<string> &result)
{
	validateKey(key);

	string value = result[0];
	if (file.doesKeyExist(keys[key]))
	{
		value = file.getString(keys[key]);
	}

	result.empty();

	int index = value.find(val_sep);
	while (index != string::npos)
	{
		string temp = value.substr(0, index);
		ConfigFile::stripLeadingWhiteSpace(temp);
		ConfigFile::stripTrailingWhiteSpace(temp);
		if (temp.length())
		{
			result.push_back(temp);
		}
		value = value.substr(index + 1, value.length());
		index = value.find(val_sep);
	}
	ConfigFile::stripLeadingWhiteSpace(value);
	ConfigFile::stripTrailingWhiteSpace(value);
	if (value.length())
	{
		result.push_back(value);
	}
}

/******************************************************************************
 *
 *	Public interface
 */

string Config::getRootDirectory()
{
	string result = ConfigImpl::instance().getRootDirectory();
	ConfigImpl::instance().getValue(ROOT_DIRECTORY, result);
	return result;
}

int Config::getSortMemBlockSize(int default_value)
{
	int result = default_value;
	ConfigImpl::instance().getValue(SORT_MEM_BLOCK_SIZE, result);
	return result;
}

int Config::getSortMemUpperLimit(int default_value)
{
	int result = default_value;
	ConfigImpl::instance().getValue(SORT_MEM_UPPER_LIMIT, result);
	return result;
}

bool Config::getRemoteFileOpenAbility(bool default_value)
{
	bool result = default_value;
	ConfigImpl::instance().getValue(REMOTE_FILE_OPEN_ABILITY, result);
	return result;
}

string_vector Config::getTempDirectories(string default_value)
{
	string_vector result(0);
	result.push_back(default_value);
	ConfigImpl::instance().getValueList(TEMP_DIRECTORIES, result);
	return result;
}
