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
 * When Firebird is initializing, it opens all *.conf files from <fbroot>/plugins.
 * For each plugin_module tag found, it constructs a Plugin object, reads the corresponding
 * plugin_config tag and inserts all config information in the object.
 *
 * When requested, the engine gets the attribute value of plugin_module/filename, load it as a
 * dynamic (shared) library and calls the exported function firebirdPlugin (FB_PLUGIN_ENTRY_POINT
 * definition, InitPluginEntryPoint prototype) passing the Plugin object as parameter.
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
 * Plugins may access Firebird API through the client library read from Plugin::getLibraryName
 * method. This method may return different filenames depending on the server architecture, and may
 * even return NULL. Currently it returns values as the table below, but this is implementation
 * detail and portable plugins should just expect a filename or NULL.
 *
 * Architecture    File
 * --------------- ------------------------------------------------------
 * Embedded        The embedded library
 * Windows SS      fbserver executable
 * Windows CS/SC   fb_inet_server executable
 * POSIX CS/SC     The embedded library
 * POSIX SS        NULL [application should open it through dlopen(NULL)]
 */

#ifndef FIREBIRD_PLUGIN_API_H
#define FIREBIRD_PLUGIN_API_H

#ifdef __GNUC__
#	pragma GCC system_header	// disable warning about non-existent virtual destructor
#endif

#include "FirebirdApi.h"

#define FB_PLUGIN_ENTRY_POINT		firebirdPlugin


namespace Firebird {

class ExternalEngine;


const int PLUGIN_VERSION_1 = 1;


class ExternalEngineFactory
{
public:
	// Strings passed to error->addString during this call are assumed to be UTF-8.
	virtual ExternalEngine* FB_CALL createEngine(Error* error, int version,
		const char* name) = 0;
};


// Passed to plugin library through entry point FB_PLUGIN_ENTRY_POINT in Firebird initialization.
class ExtEngPlugin
{
public:
	virtual int FB_CALL getVersion() = 0;
	virtual const char* FB_CALL getName() = 0;
	virtual const char* FB_CALL getLibraryName() = 0;
	virtual uint FB_CALL getConfigInfoCount() = 0;
	virtual void FB_CALL getConfigInfo(Error* error, uint index,	// first index is 0
		const char** key, const char** value) = 0;

	virtual void FB_CALL setExternalEngineFactory(ExternalEngineFactory* factory) = 0;
};


typedef void (*PluginEntryPoint)(Error* error, ExtEngPlugin* plugin);


}	// namespace Firebird


#endif	// FIREBIRD_PLUGIN_API_H
