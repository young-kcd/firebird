/*
 *	PROGRAM:		Firebird interface.
 *	MODULE:			ImplementHelper.h
 *	DESCRIPTION:	Tools to help write plugins.
 *
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
 *  The Original Code was created by Alex Peshkov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2010 Alex Peshkov <peshkoff at mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#ifndef FB_COMMON_CLASSES_GET_PLUGINS
#define FB_COMMON_CLASSES_GET_PLUGINS

#include "../common/classes/ImplementHelper.h"
#include "../common/config/config.h"

namespace Firebird {

// Default replacement for missing virtual functions
class DefaultMissingEntrypoint
{
public:
	virtual void FB_CARG noEntrypoint()
	{
		Arg::Gds(isc_wish_list).raise();
	}
};

// Template to help with loop in the set of plugins
template <typename P, typename M = DefaultMissingEntrypoint>
class GetPlugins
{
public:
	GetPlugins(unsigned int interfaceType, unsigned int desiredVersion, const char* namesList = NULL)
		: masterInterface(fb_get_master_interface()), pluginInterface(masterInterface->getPluginInterface()), missing(),
		  pluginSet(pluginInterface->getPlugins(interfaceType, namesList ? namesList : Config::getPlugins(interfaceType),
		  										desiredVersion, &missing, NULL)),
		  currentPlugin(NULL)
	{
		getPlugin();
	}

	GetPlugins(unsigned int interfaceType, unsigned int desiredVersion,
			   Config* knownConfig, const char* namesList = NULL)
		: masterInterface(fb_get_master_interface()), pluginInterface(masterInterface->getPluginInterface()), missing(),
		  pluginSet(pluginInterface->getPlugins(interfaceType, namesList ? namesList : Config::getPlugins(interfaceType),
		  										desiredVersion, &missing, new FirebirdConf(knownConfig))),
		  currentPlugin(NULL)
	{
		getPlugin();
	}

	bool hasData()
	{
		return currentPlugin;
	}

	const char* name()
	{
		return hasData() ? pluginSet->name() : NULL;
	}

	const char* module()
	{
		return hasData() ? pluginSet->module() : NULL;
	}

	P* plugin()
	{
		return currentPlugin;
	}

	void next()
	{
		if (hasData())
		{
			pluginInterface->releasePlugin(currentPlugin);
			currentPlugin = NULL;
			pluginSet->next();
			getPlugin();
		}
	}

	void set(const char* newName)
	{
		if (hasData())
		{
			pluginInterface->releasePlugin(currentPlugin);
			currentPlugin = NULL;
		}
		pluginSet->set(newName);
		getPlugin();
	}

	~GetPlugins()
	{
		if (hasData())
		{
			pluginInterface->releasePlugin(currentPlugin);
			currentPlugin = NULL;
		}
		if (pluginSet)
			pluginSet->release();
	}

private:
	AutoPtr<IMaster, AutoInterface> masterInterface;
	AutoPtr<IPlugin, AutoInterface> pluginInterface;
	M missing;
//	AutoPtr<IPluginSet, AutoInterface> pluginSet;
	IPluginSet *pluginSet;
	P* currentPlugin;

	void getPlugin()
	{
		currentPlugin = (P*) pluginSet->plugin();
	}
};

} // namespace Firebird


#endif // FB_COMMON_CLASSES_GET_PLUGINS
