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
 *	Configuration keys (logical names)
 */

enum ConfigKey
{
	ROOT_DIRECTORY,							// 0
	SORT_MEM_BLOCK_SIZE,					// 1
	SORT_MEM_UPPER_LIMIT,					// 2
	REMOTE_FILE_OPEN_ABILITY,				// 3
	TEMP_DIRECTORIES						// 4
};

/******************************************************************************
 *
 *	Main implementation class
 */

class ConfigImpl : public ConfigRoot
{
	typedef Firebird::string string;
	typedef Firebird::vector<string> string_vector;

public:
	ConfigImpl();

	static void validateKey(ConfigKey);

	void getValue(ConfigKey, string&);
	void getValue(ConfigKey, int&);
	void getValue(ConfigKey, bool&);

	void getValueList(ConfigKey key, string_vector&);

	static ConfigImpl& instance();

private:
	ConfigFile file;
	string val_sep;

    ConfigImpl(const ConfigImpl&);
    void operator=(const ConfigImpl&);
};

#endif // CONFIG_IMPL_H
