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

#include "firebird.h"
#include "consts_pub.h"
#include "iberror.h"
#include "inf_pub.h"
#include "../jrd/PluginManager.h"
#include "../jrd/ErrorImpl.h"
#include "../jrd/os/path_utils.h"
#include "../jrd/err_proto.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/init.h"
#include "../common/config/config.h"
#include "../config/ConfigFile.h"
#include "../config/ConfObj.h"
#include "../config/ConfObject.h"
#include "../config/Element.h"
#include "../config/ScanDir.h"
#include "../config/AdminException.h"

using namespace Firebird;
using Firebird::uint;

namespace Jrd {


namespace 
{
	class PluginsMap : public GenericMap<Pair<Left<string, PluginImpl*> > >
	{
	public:
		PluginsMap(MemoryPool& p)
			: GenericMap<Pair<Left<string, PluginImpl*> > >(p)
		{
		}

		~PluginsMap()
		{
			// unload plugins
			Accessor accessor(this);
			for (bool found = accessor.getFirst(); found; found = accessor.getNext())
			{
				PluginImpl* plugin = accessor.current()->second;
				delete plugin;
			}
		}
	};
}


static GlobalPtr<PluginsMap> plugins;
static GlobalPtr<PathName> libraryName;


void PluginManager::initialize()
{
#ifdef WIN_NT
#ifdef EMBEDDED
	libraryName->assign("fbembed.dll");
#elif defined(SUPERSERVER)
	libraryName->assign("fbserver.exe");
#else
	libraryName->assign("fb_inet_server.exe");
#endif
#elif !defined(SUPERSERVER)
	PathUtils::concatPath(libraryName, Config::getRootDirectory(), "lib/libfbembed.so");
#endif

	PathName pluginsPath = getPluginsDirectory();
	ScanDir dir(pluginsPath.c_str(), "*.conf");

	try
	{
		SortedObjectsArray<string> conflicts(*getDefaultMemoryPool());

		while (dir.next())
		{
			Vulcan::ConfigFile configFile(dir.getFilePath(), Vulcan::ConfigFile::LEX_none);

			for (Element* el = configFile.getObjects()->children; el; el = el->sibling)
			{
				if (el->name == "plugin_module")
				{
					AutoPtr<PluginImpl> plugin(new PluginImpl);
					plugin->name = el->getAttributeName(0);

					if (plugins->exist(plugin->name) || conflicts.exist(plugin->name))
					{
						string s;
						s.printf("Plugin %s defined more than once.", plugin->name.c_str());
						gds__log(s.c_str());

						conflicts.add(plugin->name);
						continue;
					}

					ConfObj objModule(configFile.findObject("plugin_module", plugin->name.c_str()));
					plugin->filename = objModule->getValue("filename", "");

					Element* config = el->findChild("plugin_config");
					if (config)
					{
						string configName(config->getAttributeName(0));
						ConfObj objConfig(configFile.findObject("plugin_config",
							configName.c_str()));

						for (Element* elConfig = objConfig->object->children;
							 elConfig; elConfig = elConfig->sibling)
						{
							string key = elConfig->name;
							string value;
							const char* attribute;

							for (int i = 0; (attribute = elConfig->getAttributeName(i)); ++i)
							{
								if (i != 0)
									value += PathUtils::dir_list_sep;
								value += objConfig->expand(attribute);
							}

							plugin->configInfo.add(ConfigEntry(*getDefaultMemoryPool(), key, value));
						}

						PluginImpl* p = plugin.release();
						plugins->put(p->name, p);
					}
				}
			}
		}
	}
	catch (AdminException& ex)
	{
		string s;
		s.printf("Error in plugin config file '%s': %s'", dir.getFilePath(), ex.getText());
		gds__log(s.c_str());
	}
}


PathName PluginManager::getPluginsDirectory()
{
	PathName pluginsPath;
	PathUtils::concatPath(pluginsPath, Config::getRootDirectory(), "plugins");
	return pluginsPath;
}


PluginImpl* PluginManager::getPlugin(const string& name)
{
	PluginImpl* plugin;

	if (!plugins->get(name, plugin))
	{
		//// TODO: localize
		string s;
		s.printf("Plugin %s not found.", name.c_str());
		gds__log(s.c_str());

		status_exception::raise(Arg::Gds(isc_random) << Arg::Str(s));
	}

	if (plugin->module)
		return plugin;

	static GlobalPtr<Mutex> pluginInitMutex;
	MutexLockGuard pluginInitGuard(pluginInitMutex);

	if (plugin->module)
		return plugin;

	ModuleLoader::Module* module = ModuleLoader::loadModule(plugin->filename);

	if (!module)
	{
		ModuleLoader::doctorModuleExtention(plugin->filename);
		module = ModuleLoader::loadModule(plugin->filename);
	}

	if (!module)
	{
		//// TODO: localize
		string s;
		s.printf("Can't load plugin module '%s'.", plugin->filename.c_str());
		gds__log(s.c_str());

		status_exception::raise(Arg::Gds(isc_random) << Arg::Str(s));
	}

	PluginEntryPoint entryPoint;
	if (module->findSymbol(STRINGIZE(FB_PLUGIN_ENTRY_POINT), entryPoint))
		entryPoint(RaiseError(), plugin);
	else
	{
		//// TODO: localize
		string s;
		s.printf("Entrypoint of Plugin '%s' does not exist.", plugin->filename.c_str());
		gds__log(s.c_str());

		status_exception::raise(Arg::Gds(isc_random) << Arg::Str(s));
	}

	plugin->module = module;

	return plugin;
}


//---------------------


int FB_CALL PluginImpl::getVersion()
{
	return PLUGIN_VERSION_1;
}

const char* FB_CALL PluginImpl::getName()
{
	return name.c_str();
}

const char* FB_CALL PluginImpl::getLibraryName()
{
	return libraryName->nullStr();
}

uint FB_CALL PluginImpl::getConfigInfoCount()
{
	return configInfo.getCount();
}

void FB_CALL PluginImpl::getConfigInfo(Error* error, uint index, const char** key, const char** value)
{
	if (index < 0 || (size_t) index >= configInfo.getCount())
	{
		//// TODO: localize
		ISC_STATUS status[] = {
			isc_arg_gds,
			isc_random, isc_arg_string,
				(ISC_STATUS) "Invalid value for parameter index at Plugin::getConfigInfo: out of bounds",
			isc_arg_end};
		ErrorImpl::statusVectorToError(status, error);
	}
	else
	{
		*key = configInfo[index].first.c_str();
		*value = configInfo[index].second.c_str();
	}
}


void FB_CALL PluginImpl::setExternalEngineFactory(ExternalEngineFactory* factory)
{
	externalEngineFactory = factory;
}


ExternalEngineFactory* PluginImpl::getExternalEngineFactory()
{
	if (!externalEngineFactory)
	{
		status_exception::raise(
			Arg::Gds(isc_random) <<
			Arg::Str("Plugin @1 does not create @2 instances") <<
			Arg::Str(name) <<
			Arg::Str("ExternalEngine"));
	}

	return externalEngineFactory;
}


}	// namespace Jrd
