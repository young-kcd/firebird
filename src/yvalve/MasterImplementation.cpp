/*
 *	PROGRAM:		Firebird interface.
 *	MODULE:			MasterImplementation.cpp
 *	DESCRIPTION:	Main firebird interface, used to get other interfaces.
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

#include "firebird.h"
#include "Interface.h"
#include "../common/classes/ImplementHelper.h"
#include "../common/classes/init.h"
#include "../common/StatusHolder.h"
#include "../yvalve/PluginManager.h"
#include "../common/classes/GenericMap.h"
#include "../common/classes/fb_pair.h"

namespace Firebird {

class MasterImplementation : public StackIface<IMaster, IMaster::VERSION>
{
public:
	Status* FB_CARG getStatusInstance();
	IPlugin* FB_CARG getPluginInterface();
	void FB_CARG upgradeInterface(Interface* toUpgrade, int desiredVersion, void* missingFunctionClass);
};

class UserStatus : public Firebird::StdIface<Firebird::BaseStatus, FB_STATUS_VERSION>
{
private:
	int FB_CARG release()
	{
		if (--refCounter == 0)
		{
			delete this;
			return 0;
		}
		return 1;
	}
};

Firebird::Status* FB_CARG Firebird::MasterImplementation::getStatusInstance()
{
	return new UserStatus;
}


IPlugin* FB_CARG MasterImplementation::getPluginInterface()
{
	static Static<PluginManager> manager;

	manager->addRef();
	return &manager;
}

namespace {
	typedef void function();
	typedef function* FunctionPtr;

	struct CVirtualClass
	{
		FunctionPtr* vTab;
	};

	typedef Firebird::Pair<Firebird::NonPooled<U_IPTR, FunctionPtr*> > FunctionPair;
	GlobalPtr<GenericMap<FunctionPair> > functionMap;
}

void FB_CARG MasterImplementation::upgradeInterface(Interface* toUpgrade,
													int desiredVersion,
													void* missingFunctionClass)
{
	if (toUpgrade->version() >= desiredVersion)
		return;

	CVirtualClass* target = (CVirtualClass*)toUpgrade;
	FunctionPtr* newTab = NULL;

	if (!functionMap->get((U_IPTR)(target->vTab), newTab))
	{
		CVirtualClass* miss = (CVirtualClass*)missingFunctionClass;
		newTab = FB_NEW(*getDefaultMemoryPool()) FunctionPtr[desiredVersion];

		for (int i = 0; i < toUpgrade->version(); ++i)
			newTab[i] = target->vTab[i];
		for (int j = toUpgrade->version(); j < desiredVersion; ++j)
			newTab[j] = miss->vTab[0];

		functionMap->put((U_IPTR)(target->vTab), newTab);
	}

	target->vTab = newTab;
}

} // namespace Firebird

Firebird::IMaster* ISC_EXPORT fb_get_master_interface()
{
	static Firebird::Static<Firebird::MasterImplementation> master;

	master->addRef();
	return &master;
}
