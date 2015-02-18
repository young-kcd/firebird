/*
 *	PROGRAM:		Firebird samples.
 *	MODULE:			CryptKeyHolder.cpp
 *	DESCRIPTION:	Sample of how key holder may be written.
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
 *  Copyright (c) 2012 Alex Peshkov <peshkoff at mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "ibase.h"
#include "firebird/Interface.h"

#include "firebird.h"		// Needed for atomic support
#include "../common/classes/fb_atomic.h"


using namespace Firebird;

namespace
{

IMaster* master = NULL;
IPluginManager* pluginManager = NULL;

class PluginModule : public IPluginModuleImpl<PluginModule, CheckStatusWrapper>
{
public:
	PluginModule()
		: flag(false)
	{ }

	void registerMe()
	{
		pluginManager->registerModule(this);
		flag = true;
	}

	~PluginModule()
	{
		if (flag)
		{
			pluginManager->unregisterModule(this);
			doClean();
		}
	}

	IPluginModule* getModule()
	{
		return this;
	}

	void doClean()
	{
		flag = false;
	}

private:
	bool flag;
};

PluginModule module;

class CryptKeyHolder : public IKeyHolderPluginImpl<CryptKeyHolder, CheckStatusWrapper>
{
public:
	explicit CryptKeyHolder(IPluginConfig* cnf) throw()
		: callbackInterface(this), config(cnf), key(0), owner(NULL)
	{
		config->addRef();
	}

	~CryptKeyHolder()
	{
		config->release();
	}

	// IKeyHolderPlugin implementation
	int keyCallback(CheckStatusWrapper* status, ICryptKeyCallback* callback);
	ICryptKeyCallback* keyHandle(CheckStatusWrapper* status, const char* keyName);

	int release()
	{
		if (--refCounter == 0)
		{
			delete this;
			return 0;
		}
		return 1;
	}

	void addRef()
	{
		++refCounter;
	}

	IPluginModule* getModule()
	{
		return &module;
	}

	void setOwner(Firebird::IReferenceCounted* o)
	{
		owner = o;
	}

	IReferenceCounted* getOwner()
	{
		return owner;
	}

	UCHAR getKey()
	{
		return key;
	}

private:
	class CallbackInterface : public ICryptKeyCallbackImpl<CallbackInterface, CheckStatusWrapper>
	{
	public:
		explicit CallbackInterface(CryptKeyHolder* p)
			: parent(p)
		{ }

		unsigned int callback(unsigned int, const void*, unsigned int length, void* buffer)
		{
			UCHAR k = parent->getKey();
			if (!k)
			{
				return 0;
			}

			if (length > 0 && buffer)
			{
				memcpy(buffer, &k, 1);
			}
			return 1;
		}

		IPluginModule* getModule()
		{
			return &module;
		}

	private:
		CryptKeyHolder* parent;
	};

	CallbackInterface callbackInterface;

	IPluginConfig* config;
	UCHAR key;

	AtomicCounter refCounter;
	IReferenceCounted* owner;

	void noKeyError(CheckStatusWrapper* status);
};

void CryptKeyHolder::noKeyError(CheckStatusWrapper* status)
{
	ISC_STATUS_ARRAY vector;
	vector[0] = isc_arg_gds;
	vector[1] = isc_random;
	vector[2] = isc_arg_string;
	vector[3] = (ISC_STATUS) "Key not set";
	vector[4] = isc_arg_end;
	status->setErrors(vector);
}

int CryptKeyHolder::keyCallback(CheckStatusWrapper* status, ICryptKeyCallback* callback)
{
	status->init();

	if (key != 0)
		return 1;

	IConfig* def = config->getDefaultConfig(status);
	if (status->getState() & Firebird::IStatus::STATE_ERRORS)
		return 1;

	IConfigEntry* confEntry = def->find(status, "Auto");
	def->release();
	if (status->getState() & Firebird::IStatus::STATE_ERRORS)
		return 1;

	if (confEntry)
	{
		FB_BOOLEAN b = confEntry->getBoolValue();
		confEntry->release();
		if (b)
		{
			key = 0x5a;
			return 1;
		}
	}

	if (callback && callback->callback(0, NULL, 1, &key) != 1)
	{
		key = 0;
		return 0;
	}

	return 1;
}

ICryptKeyCallback* CryptKeyHolder::keyHandle(CheckStatusWrapper* status, const char* keyName)
{
	if (strcmp(keyName, "sample") != 0)
	{
		return NULL;
	}

	return &callbackInterface;
}

class Factory : public IPluginFactoryImpl<Factory, CheckStatusWrapper>
{
public:
	IPluginModule* getModule()
	{
		return &module;
	}

	IPluginBase* createPlugin(CheckStatusWrapper* status, IPluginConfig* factoryParameter)
	{
		try
		{
			CryptKeyHolder* p = new CryptKeyHolder(factoryParameter);
			p->addRef();
			return p;
		}
		catch (...)
		{
			ISC_STATUS st[3] = {isc_arg_gds, isc_virmemexh, isc_arg_end};
			status->setErrors(st);
		}
		return NULL;
	}
};

Factory factory;

} // anonymous namespace

extern "C" void FB_PLUGIN_ENTRY_POINT(IMaster* m)
{
	master = m;
	pluginManager = master->getPluginManager();

	module.registerMe();
	pluginManager->registerPluginFactory(IPluginManager::TYPE_KEY_HOLDER, "CryptKeyHolder_example",
		&factory);
}
