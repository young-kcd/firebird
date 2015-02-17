/*
 *	PROGRAM:		Firebird samples.
 *	MODULE:			DbCrypt.cpp
 *	DESCRIPTION:	Sample of how diskcrypt may be written.
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

class DbCrypt : public IDbCryptPluginImpl<DbCrypt, CheckStatusWrapper>
{
public:
	explicit DbCrypt(IPluginConfig* cnf) throw()
		: config(cnf), key(0), owner(NULL)
	{
		config->addRef();
	}

	~DbCrypt()
	{
		config->release();
	}

	// ICryptPlugin implementation
	void encrypt(CheckStatusWrapper* status, unsigned int length, const void* from, void* to);
	void decrypt(CheckStatusWrapper* status, unsigned int length, const void* from, void* to);
	void setKey(CheckStatusWrapper* status, unsigned int length, IKeyHolderPlugin** sources);

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

	void setOwner(IReferenceCounted* o)
	{
		owner = o;
	}

	IReferenceCounted* getOwner()
	{
		return owner;
	}

private:
	IPluginConfig* config;
	UCHAR key;

	AtomicCounter refCounter;
	IReferenceCounted* owner;

	void noKeyError(CheckStatusWrapper* status);
};

void DbCrypt::noKeyError(CheckStatusWrapper* status)
{
	ISC_STATUS_ARRAY vector;
	vector[0] = isc_arg_gds;
	vector[1] = isc_random;
	vector[2] = isc_arg_string;
	vector[3] = (ISC_STATUS)"Key not set";
	vector[4] = isc_arg_end;
	status->setErrors(vector);
}

void DbCrypt::encrypt(CheckStatusWrapper* status, unsigned int length, const void* from, void* to)
{
	status->init();

	if (!key)
	{
		noKeyError(status);
		return;
	}

	const UCHAR* f = static_cast<const UCHAR*>(from);
	UCHAR* t = static_cast<UCHAR*>(to);

	while (length--)
	{
		*t++ = (*f++) ^ key;
	}
}

void DbCrypt::decrypt(CheckStatusWrapper* status, unsigned int length, const void* from, void* to)
{
	status->init();

	if (!key)
	{
		noKeyError(status);
		return;
	}

	const UCHAR* f = static_cast<const UCHAR*>(from);
	UCHAR* t = static_cast<UCHAR*>(to);

	while (length--)
	{
		*t++ = (*f++) ^ key;
	}
}

void DbCrypt::setKey(CheckStatusWrapper* status, unsigned int length, IKeyHolderPlugin** sources)
{
	status->init();

	if (key != 0)
		return;

	IConfig* def = config->getDefaultConfig(status);
	if (status->getStatus() & Firebird::IStatus::FB_HAS_ERRORS)
		return;

	IConfigEntry* confEntry = def->find(status, "Auto");
	if (status->getStatus() & Firebird::IStatus::FB_HAS_ERRORS)
	{
		def->release();
		return;
	}

	if (confEntry)
	{
		char v = *(confEntry->getValue());
		confEntry->release();
		if (v == '1' || v == 'y' || v == 'Y' || v == 't' || v == 'T')
		{
			confEntry = def->find(status, "Value");
			def->release();
			if (confEntry)
			{
				v = confEntry->getIntValue();
				confEntry->release();
				if (v)
				{
					key = v;
					return;
				}
			}
			key = 0x5a;
			return;
		}
		def->release();
	}

	for (unsigned n = 0; n < length; ++n)
	{
		ICryptKeyCallback* callback = sources[n]->keyHandle(status, "sample");
		if (status->getStatus() & Firebird::IStatus::FB_HAS_ERRORS)
			return;

		if (callback && callback->callback(0, NULL, 1, &key) == 1)
			return;
	}

	key = 0;
	noKeyError(status);
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
			DbCrypt* p = new DbCrypt(factoryParameter);
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
	pluginManager->registerPluginFactory(IPluginManager::DbCrypt, "DbCrypt_example", &factory);
}
