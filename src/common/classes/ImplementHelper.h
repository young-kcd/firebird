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

#ifndef FB_COMMON_CLASSES_IMPLEMENT_HELPER
#define FB_COMMON_CLASSES_IMPLEMENT_HELPER

#include "FirebirdPluginApi.h"
#include "../common/classes/alloc.h"
#include "gen/iberror.h"
#include "../yvalve/gds_proto.h"
#include "../common/classes/init.h"

namespace Firebird {

// If you need interface on stack, use template AutoPtr<YourInterface, AutoInterface>
// as second parameter to store it.
class AutoInterface
{
public:
	static void clear(Interface* ptr)
	{
		if (ptr)
		{
			ptr->release();
		}
	}
};


// Implement standard interface and plugin functions
template <class C, int V, typename S>
class Versioned : public C, public S
{
public:
	int FB_CARG version()
	{
		return V;
	}
};

template <class C, int V, typename S = GlobalStorage>
class StackIface : public Versioned<C, V, S>
{
#ifdef DEV_BUILD
private:
	int counter;

public:
	// StackIface is destroyed in same frame where created,
	// therefore final release() call is not expected,
	// therefore initialize check counter with 0, not 1.
	StackIface() : counter(0) { }

	~StackIface()
	{
		fb_assert(counter == 0);
	}
#endif

public:
	void FB_CARG addRef()
	{
#ifdef DEV_BUILD
		++counter;
#endif
	}

	int FB_CARG release()
	{
#ifdef DEV_BUILD
		--counter;
#endif
		return 1;
	}

	void* operator new(size_t, void* memory) throw()
	{
		return memory;
	}
};

template <class C, int V, typename S = GlobalStorage>
class StdIface : public Versioned<C, V, S>
{
public:
	StdIface() : refCounter(1) { }

#ifdef DEV_BUILD
	~StdIface()
	{
//		fb_assert(refCounter.value() == 0);
	}
#endif

	void FB_CARG addRef()
	{
		++refCounter;
	}

protected:
	AtomicCounter refCounter;

private:
	StdIface(const StdIface&);
	StdIface& operator=(const StdIface&);
};


template <class C, int V, typename S = GlobalStorage>
class StdPlugin : public StdIface<C, V, S>
{
private:
	Interface* own;

public:
	StdPlugin() : own(NULL)
	{ }

	Interface* FB_CARG owner(Interface* iface)
	{
		Interface* rc = own;
		own = iface;
		return rc;
	}
};


// Trivial factory
template <class P>
class SimpleFactoryBase : public StackIface<PluginsFactory, FB_PLUGINS_FACTORY_VERSION>
{
public:
	Plugin* FB_CARG createPlugin(IFactoryParameter* factoryParameter)
	{
		return new P(factoryParameter);
	}
};

template <class P>
class SimpleFactory : public Static<SimpleFactoryBase<P> >
{
};


// Generic plugins interface
class PluginInterface : public AutoPtr<IPlugin, AutoInterface>
{
public:
	PluginInterface() : AutoPtr<IPlugin, AutoInterface>(NULL)
	{
		IMaster* mi = fb_get_master_interface();
		reset(mi->getPluginInterface());
		mi->release();
	}
};

} // namespace Firebird


#endif // FB_COMMON_CLASSES_IMPLEMENT_HELPER
