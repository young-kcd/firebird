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
#include "../common/os/path_utils.h"
#include "../jrd/err_proto.h"
#include "../common/isc_proto.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/init.h"
#include "../common/config/config.h"
#include "../common/config/config_file.h"
#include "../common/utils_proto.h"
#include "../common/ScanDir.h"

using namespace Firebird;
using Firebird::uint;

namespace Jrd {


namespace
{
	class PluginsMap : public GenericMap<Pair<Left<ConfigFile::String, PluginImpl*> > >
	{
	public:
		explicit PluginsMap(MemoryPool& p)
			: GenericMap<Pair<Left<ConfigFile::String, PluginImpl*> > >(p)
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
#else // WIN_NT
	// This will become
	// libraryName->assign(fb_utils::getPrefix(fb_utils::FB_DIR_LIB, "libfirebird.so"));
	// when we start building libfirebird.so.
#ifndef SUPERSERVER
	libraryName->assign(fb_utils::getPrefix(fb_utils::FB_DIR_LIB, "libfbembed.so"));
#endif
#endif // WIN_NT

	PathName pluginsPath = getPluginsDirectory();
	ScanDir dir(pluginsPath.c_str(), "*.conf");

	try
	{
		SortedObjectsArray<ConfigFile::String> conflicts(*getDefaultMemoryPool());

		while (dir.next())
		{
			ConfigFile configFile(dir.getFilePath(),
				ConfigFile::HAS_SUB_CONF | ConfigFile::EXCEPTION_ON_ERROR);

			const ConfigFile::Parameters& params = configFile.getParameters();
			for (size_t n = 0; n < params.getCount(); ++n)
			{
				const ConfigFile::Parameter* pm = &params[n];
				if (pm->name != "plugin_module")
				{
					continue;
				}

				AutoPtr<PluginImpl> plugin(new PluginImpl);
				plugin->name = pm->value;
				if (plugins->exist(plugin->name) || conflicts.exist(plugin->name))
				{
					gds__log("Plugin %s defined more than once.", plugin->name.c_str());
					conflicts.add(plugin->name);
					continue;
				}

				if (!pm->sub)
				{
					fatal_exception::raiseFmt("Missing required parameters for plugin %s",
						plugin->name.c_str());
				}

				const ConfigFile::Parameter* par = pm->sub->findParameter("filename");

				if (!par)
				{
					fatal_exception::raiseFmt("Missing required parameter 'filename' for plugin %s",
						plugin->name.c_str());
				}

				plugin->filename = par->value.ToPathName();

				par = pm->sub->findParameter("plugin_config");
				if (par)
				{
					par = configFile.findParameter("plugin_config", par->value);
					if (!par)
					{
						fatal_exception::raiseFmt("Missing required config for plugin %s",
							plugin->name.c_str());
					}

					if (par->sub)
					{
						const ConfigFile::Parameters& all = par->sub->getParameters();
						for (size_t n = 0; n < all.getCount(); ++n)
						{
							plugin->configInfo.add(ConfigEntry(*getDefaultMemoryPool(),
								all[n].name, all[n].value));
						}
					}
				}

				PluginImpl* p = plugin.release();
				plugins->put(p->name, p);
			}
		}
	}
	catch (const Exception& ex)
	{
		string s;
		s.printf("Error in plugin config file '%s'", dir.getFilePath());
		iscLogException(s.c_str(), ex);
	}
}


PathName PluginManager::getPluginsDirectory()
{
	PathName pluginsPath;
	PathUtils::concatPath(pluginsPath, Config::getRootDirectory(), "plugins");
	return pluginsPath;
}


PluginImpl* PluginManager::getPlugin(const ConfigFile::String& name)
{
	PluginImpl* plugin;

	if (!plugins->get(name, plugin))
	{
		//// TODO: localize
		gds__log("Plugin %s not found.", name.c_str());
		status_exception::raise(Arg::Gds(isc_pman_plugin_notfound) << name);
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
		ModuleLoader::doctorModuleExtension(plugin->filename);
		module = ModuleLoader::loadModule(plugin->filename);
	}

	if (!module)
	{
		//// TODO: localize
		gds__log("Can't load plugin module '%s'.", plugin->filename.c_str());
		status_exception::raise(Arg::Gds(isc_pman_cannot_load_plugin) << plugin->filename);
	}

	PluginEntryPoint entryPoint;
	if (module->findSymbol(STRINGIZE(FB_PLUGIN_ENTRY_POINT), entryPoint))
		entryPoint(RaiseError(), plugin);
	else
	{
		//// TODO: localize
		gds__log("Entrypoint of Plugin '%s' does not exist.", plugin->filename.c_str());
		status_exception::raise(Arg::Gds(isc_pman_entrypoint_notfound) << plugin->filename);
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
	if (index >= configInfo.getCount())
	{
		Arg::Gds vector(isc_pman_bad_conf_index);
		vector << Arg::Num(index);
		ErrorImpl::statusVectorToError(vector.value(), error);
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
			Arg::Gds(isc_pman_unknown_instance) << name << "ExternalEngine");
	}

	return externalEngineFactory;
}


}	// namespace Jrd
