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
 *  The Original Code was created by Adriano dos Santos Fernandes
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2008 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef JRD_PLUGIN_MANAGER_H
#define JRD_PLUGIN_MANAGER_H

#include "FirebirdPluginApi.h"
#include "../common/common.h"
#include "../common/os/mod_loader.h"
#include "../common/classes/array.h"
#include "../common/classes/auto.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/GenericMap.h"
#include "../common/classes/MetaName.h"
#include "../common/classes/objects_array.h"
#include "../common/classes/rwlock.h"
#include "../common/config/config_file.h"

namespace Jrd {


typedef Firebird::Pair<Firebird::Full<ConfigFile::KeyType, ConfigFile::String> > ConfigEntry;

class PluginImpl : public Firebird::ExtEngPlugin, public Firebird::GlobalStorage
{
friend class PluginManager;

public:
	PluginImpl()
		: name(getPool()),
		  filename(getPool()),
		  configInfo(getPool()),
		  module(NULL),
		  externalEngineFactory(NULL)
	{
	}

	virtual int FB_CALL getVersion();
	virtual const char* FB_CALL getName();
	virtual const char* FB_CALL getLibraryName();
	virtual Firebird::uint FB_CALL getConfigInfoCount();
	virtual void FB_CALL getConfigInfo(Firebird::Error* error, Firebird::uint index,
		const char** key, const char** value);

	virtual void FB_CALL setExternalEngineFactory(Firebird::ExternalEngineFactory* factory);

public:
	Firebird::ExternalEngineFactory* getExternalEngineFactory();

private:
	ConfigFile::String name;
	Firebird::PathName filename;
	Firebird::SortedObjectsArray<
		ConfigEntry,
		Firebird::EmptyStorage<ConfigEntry*>,
		ConfigFile::KeyType, Firebird::FirstPointerKey<ConfigEntry>
	> configInfo;
	Firebird::AutoPtr<ModuleLoader::Module> module;
	Firebird::ExternalEngineFactory* externalEngineFactory;
};


class PluginManager
{
public:
	static void initialize();
	static Firebird::PathName getPluginsDirectory();
	static PluginImpl* getPlugin(const ConfigFile::String& name);
};


}	// namespace Jrd

#endif	// JRD_PLUGIN_MANAGER_H
