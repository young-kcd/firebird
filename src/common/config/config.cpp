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
#pragma warning (disable: 4800)	// forcing value to bool 'true' or 'false'
#endif

#include "../../common/config/config.h"
#include "../../common/config/config_impl.h"
#include "../../common/config/config_file.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include "../jrd/gdsassert.h"

typedef Firebird::string string;

/******************************************************************************
 *
 *	Configuration entries
 */

const ConfigImpl::ConfigEntry ConfigImpl::entries[] =
{
	{TYPE_STRING,		"RootDirectory",			(ConfigValue) 0},
	{TYPE_INTEGER,		"SortMemBlockSize",			(ConfigValue) 1048576},
	{TYPE_INTEGER,		"SortMemUpperLimit",		(ConfigValue) 268435456},
	{TYPE_BOOLEAN,		"RemoteFileOpenAbility",	(ConfigValue) false},
	{TYPE_INTEGER,		"GuardianOption",			(ConfigValue) 1},
	{TYPE_INTEGER,		"CpuAffinityMask",			(ConfigValue) 1},
	{TYPE_BOOLEAN,      "OldParameterOrdering",		(ConfigValue) false}
};

/******************************************************************************
 *
 *	Static instance of the system configuration file
 */

const static ConfigImpl sysConfig;

/******************************************************************************
 *
 *	Implementation interface
 */

ConfigImpl::ConfigImpl()
{
	/* Prepare some stuff */

	ConfigFile file;
	root_dir = getRootDirectory().c_str();
	MemoryPool *pool = getDefaultMemoryPool();
	int size = FB_NELEM(entries);
	values = FB_NEW(*pool) ConfigValue[size];

	string val_sep = ",";
	file.setConfigFile(getConfigFile());

	/* Iterate through the known configuration entries */

	for (int i = 0; i < size; i++)
	{
		ConfigEntry entry = entries[i];
		string value = getValue(file, entries[i].key);

		if (!value.length())
		{
			/* Assign the default value */

			values[i] = entries[i].default_value;
			continue;
		}

		/* Assign the actual value */

		switch (entry.data_type)
		{
		case TYPE_BOOLEAN:
			values[i] = (ConfigValue) asBoolean(value);
			break;
		case TYPE_INTEGER:
			values[i] = (ConfigValue) asInteger(value);
			break;
		case TYPE_STRING:
			{
			const char *src = asString(value);
			char *dst = FB_NEW(*pool) char[strlen(src) + 1];
			strcpy(dst, src);
			values[i] = (ConfigValue) dst;
			}
			break;
		case TYPE_STRING_VECTOR:
			;
		}
	}
}

ConfigImpl::~ConfigImpl()
{
	int size = FB_NELEM(entries);

	/* Free allocated memory */

	for (int i = 0; i < size; i++)
	{
		if (values[i] == entries[i].default_value)
			continue;

		switch (entries[i].data_type)
		{
		case TYPE_STRING:
			delete[] values[i];
			break;
		case TYPE_STRING_VECTOR:
			;
		}
	}

	delete[] values;
}

string ConfigImpl::getValue(ConfigFile file, ConfigKey key)
{
	return file.doesKeyExist(key) ? file.getString(key) : "";
}

int ConfigImpl::asInteger(const string &value)
{
	return atoi(value.data());
}

bool ConfigImpl::asBoolean(const string &value)
{
	return (atoi(value.data()) != 0);
}

const char* ConfigImpl::asString(const string &value)
{
	return value.c_str();
}

/******************************************************************************
 *
 *	Public interface
 */

const char* Config::getRootDirectory()
{
	const char* result = (char*) sysConfig.values[KEY_ROOT_DIRECTORY];
	return result ? result : sysConfig.root_dir;
}

int Config::getSortMemBlockSize()
{
	return (int) sysConfig.values[KEY_SORT_MEM_BLOCK_SIZE];
}

int Config::getSortMemUpperLimit()
{
	return (int) sysConfig.values[KEY_SORT_MEM_UPPER_LIMIT];
}

bool Config::getRemoteFileOpenAbility()
{
	return (bool) sysConfig.values[KEY_REMOTE_FILE_OPEN_ABILITY];
}

int Config::getGuardianOption()
{
	return (int) sysConfig.values[KEY_GUARDIAN_OPTION];
}

int Config::getCpuAffinityMask()
{
	return (int) sysConfig.values[KEY_CPU_AFFINITY_MASK];
}

bool Config::getOldParameterOrdering()
{
	return (bool) sysConfig.values[KEY_OLD_PARAMETER_ORDERING];
}
