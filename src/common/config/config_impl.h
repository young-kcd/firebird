/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		config_impl.h
 *	DESCRIPTION:	Configuration manager (private interface)
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

#ifndef CONFIG_IMPL_H
#define CONFIG_IMPL_H

#include "fb_types.h"
#include "fb_string.h"

#include "../../common/config/config.h"
#include "../../common/config/config_file.h"
#include "../jrd/os/config_root.h"

/******************************************************************************
 *
 *	Main implementation class
 */

class ConfigImpl : public ConfigRoot
{
	friend class Config;

	typedef Firebird::string string;

	enum ConfigType
	{
		TYPE_BOOLEAN,
		TYPE_INTEGER,
		TYPE_STRING,
		TYPE_STRING_VECTOR
	};

	typedef char* ConfigKey;
	typedef void* ConfigValue;

	struct ConfigEntry
	{
		ConfigType data_type;
		ConfigKey key;
		ConfigValue default_value;
	};

public:
    ~ConfigImpl();

	static string getValue(ConfigFile&, ConfigKey);

	static int asInteger(const string&);
	static bool asBoolean(const string&);
	static const char* asString(const string&);

	inline static const ConfigImpl& instance();

private:
	ConfigImpl();

	static const ConfigEntry entries[];
	const char *root_dir;
	ConfigValue *values;

    ConfigImpl(const ConfigImpl&);
    void operator=(const ConfigImpl&);
};

#endif // CONFIG_IMPL_H
