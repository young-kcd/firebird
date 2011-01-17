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

/*
 * Firebird plugins are accessed using methods of PluginLoader interface.
 * For each plugin_module tag found, it constructs a Plugin object, reads the corresponding
 * plugin_config tag and inserts all config information in the object.
 *
 * When requested, the engine gets the attribute value of plugin_module/filename, load it as a
 * dynamic (shared) library and calls the exported function firebirdPlugin (FB_PLUGIN_ENTRY_POINT
 * definition, StartLoadedModule prototype) passing the Plugin object as parameter.
 *
 * The plugin library may save the plugin object and call they methods later. The object and all
 * pointers returned by it are valid until the plugin is unloaded (done through OS unload of the
 * dynamic library) when Firebird is shutting down.
 *
 * Inside the plugin entry point (firebirdPlugin), the plugin may register extra functionality that
 * may be obtained by Firebird when required. Currently only External Engines may be registered
 * through Plugin::setExternalEngineFactory.
 *
 * Example plugin configuration file:
 *
 * <external_engine UDR>
 *     plugin_module UDR_engine
 * </external_engine>
 *
 * <plugin_module UDR_engine>
 *     filename $(this)/udr_engine
 *     plugin_config UDR_config
 * </plugin_module>
 *
 * <plugin_config UDR_config>
 *     path $(this)/udr
 * </plugin_config>
 *
 * Note that the external_engine tag is ignored at this stage. Only plugin_module and plugin_config
 * are read. The dynamic library extension may be ommitted, and $(this) expands to the directory of
 * the .conf file.
 *
 * Plugins may access Firebird API through the fbclient library.
 */

#ifndef FIREBIRD_PLUGIN_API_H
#define FIREBIRD_PLUGIN_API_H

#ifdef __GNUC__
#	pragma GCC system_header	// disable warning about non-existent virtual destructor
#endif

#include "Interface.h"

#define FB_PLUGIN_ENTRY_POINT		firebird_plugin


namespace Firebird {

// Plugin interface - base for master plugin interfaces (factories are registered for them)
class Plugin : public Interface
{
public:
	virtual Interface* FB_CARG owner(Interface*) = 0;
};
#define FB_PLUGIN_VERSION (FB_INTERFACE_VERSION + 1)

// IPluginSet - low level tool to access plugins according to parameter from firebird.conf
class IPluginSet : public Interface
{
public:
	virtual const char* FB_CARG name() const = 0;
	virtual const char* FB_CARG module() const = 0;
	virtual Plugin* FB_CARG plugin() = 0;
	virtual void FB_CARG next() = 0;
	virtual void FB_CARG set(const char*) = 0;
};
#define FB_PLUGIN_SET_VERSION (FB_INTERFACE_VERSION + 5)

// Interfaces to work with configuration data
class IConfig;

class IConfigParameter : public Interface
{
public:
	virtual const char* FB_CARG name() = 0;
	virtual const char* FB_CARG value() = 0;
	virtual IConfig* FB_CARG sub() = 0;
};
#define FB_I_CONFIG_PARAMETER_VERSION (FB_INTERFACE_VERSION + 3)

class IConfig : public Interface
{
public:
	virtual IConfigParameter* FB_CARG find(const char* name) = 0;
	virtual IConfigParameter* FB_CARG findValue(const char* name, const char* value) = 0;
	virtual IConfigParameter* FB_CARG findPos(const char* name, unsigned int pos) = 0;
};
#define FB_I_CONFIG_VERSION (FB_INTERFACE_VERSION + 3)

// This interface is passed to plugin's factory as it's single parameter
class IFactoryParameter : public Interface
{
public:
	virtual const char* FB_CARG getConfigFileName() = 0;
	virtual IConfig* FB_CARG getDefaultConfig() = 0;
};
#define FB_FACTORY_PARAMETER_VERSION (FB_INTERFACE_VERSION + 2)

// Required to creat instances of given plugin
class PluginsFactory : public Interface
{
public:
	virtual Plugin* FB_CARG createPlugin(IFactoryParameter* fp) = 0;
};
#define FB_PLUGINS_FACTORY_VERSION (FB_INTERFACE_VERSION + 1)

// Interface to deal with plugins here and there, returned by master interface
class IPlugin : public Interface
{
public:
	virtual void FB_CARG registerPlugin(unsigned int interfaceType, const char* defaultName,
										PluginsFactory* factory) = 0;
	virtual IPluginSet* FB_CARG getPlugins(unsigned int interfaceType, const char* namesList,
										   int desiredVersion, void* missingFunctionClass) = 0;
	virtual IConfig* FB_CARG getConfig(const char* filename) = 0;
	virtual void FB_CARG releasePlugin(Plugin* plugin) = 0;
};
#define FB_I_PLUGIN_VERSION (FB_INTERFACE_VERSION + 4)

typedef void StartLoadedModule(IMaster* masterInterface);

namespace PluginType {
	static const unsigned int YValve = 1;
	static const unsigned int Provider = 2;
	// leave space for may be some more super-std plugins
	static const unsigned int FirstNonLibPlugin = 11;
	static const unsigned int AuthServer = 11;
	static const unsigned int AuthClient = 12;
	static const unsigned int AuthUserManagement = 13;
	static const unsigned int ExternalEngine = 14;
	static const unsigned int Trace = 15;
};

}	// namespace Firebird


#endif	// FIREBIRD_PLUGIN_API_H
