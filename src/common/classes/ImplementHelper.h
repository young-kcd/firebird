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
#include "Timer.h"
#include "../common/classes/alloc.h"
#include "gen/iberror.h"
#include "../yvalve/gds_proto.h"
#include "../common/classes/init.h"
#include "../common/classes/auto.h"
#include "../common/classes/RefCounted.h"
#include "consts_pub.h"

extern "C" int API_ROUTINE fb_shutdown(unsigned int timeout, const int reason);

namespace Firebird {

// If you need interface on stack, use template AutoPtr<YourInterface, AutoInterface>
// as second parameter to store it.
class AutoInterface
{
public:
	static void clear(IDisposable* ptr)
	{
		if (ptr)
		{
			ptr->dispose();
		}
	}
};


// Implement standard interface and plugin functions
template <class C, typename S = GlobalStorage>
class DisposeIface : public C, public S
{
public:
	DisposeIface() { }

private:
	DisposeIface(const DisposeIface&);
	DisposeIface& operator=(const DisposeIface&);
};

template <class C, typename S = GlobalStorage>
class StackIface : public DisposeIface <C, S>
{
public:
	void FB_CARG dispose()
	{ }

	void* operator new(size_t, void* memory) throw()
	{
		return memory;
	}
};

template <class C, int V, typename S = GlobalStorage>
class StdIface : public C, public S
{
public:
	StdIface() : refCounter(0) { }

	int FB_CARG version()
	{
		return V;
	}

#ifdef DEV_BUILD
	~StdIface()
	{
		fb_assert(refCounter.value() == 0);
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
class SimpleFactoryBase : public StackIface<PluginsFactory>
{
public:
	Plugin* FB_CARG createPlugin(IFactoryParameter* factoryParameter)
	{
		P* p = new P(factoryParameter);
		p->addRef();
		return p;
	}
};

template <class P>
class SimpleFactory : public Static<SimpleFactoryBase<P> >
{
};


// Master interface
class MasterInterface : public AutoPtr<IMaster, AutoInterface>
{
public:
	MasterInterface() : AutoPtr<IMaster, AutoInterface>(fb_get_master_interface())
	{ }
};


// Generic plugins interface
class PluginInterface : public AutoPtr<IPlugin, AutoInterface>
{
public:
	PluginInterface() : AutoPtr<IPlugin, AutoInterface>(fb_get_master_interface()->getPluginInterface())
	{ }
	PluginInterface(IMaster* master) : AutoPtr<IPlugin, AutoInterface>(master->getPluginInterface())
	{ }
};


// Control timer interface
class TimerInterface : public AutoPtr<ITimerControl, AutoInterface>
{
public:
	TimerInterface() : AutoPtr<ITimerControl, AutoInterface>(fb_get_master_interface()->getTimerControl())
	{ }
};


// When process exits, dynamically loaded modules (for us plugin modules)
// are unloaded first. As the result all global variables in plugin are already destroyed
// when yvalve is starting fb_shutdown(). This causes almost unavoidable segfault.
// To avoid it this class is added - it detects spontaneous (not by PluginManager)
// module unload and invokes shutdown in order to stop all plugins before they
// are destroyed by OS.
class DummyStorage
{
};

class UnloadDetectorHelper : public DisposeIface<IModuleCleanup, DummyStorage>
{
public:
	UnloadDetectorHelper(MemoryPool&)
		: flagOsUnload(true), cleanup(NULL)
	{ }

	~UnloadDetectorHelper()
	{
		if (flagOsUnload)
		{
			PluginInterface()->resetModuleCleanup(this);

			fb_shutdown(5000, fb_shutrsn_exit_called);
			doClean();
		}
	}

	bool unloadStarted()
	{
		return !flagOsUnload;
	}

	void setCleanup(FPTR_VOID c)
	{
		cleanup = c;
	}

	void FB_CARG dispose()
	{
		// delete this;	-don't do that!
	}

private:
	bool flagOsUnload;
	FPTR_VOID cleanup;

	void FB_CARG doClean()
	{
		flagOsUnload = false;
		if (cleanup)
		{
			cleanup();
			cleanup = NULL;
		}
	}
};

typedef GlobalPtr<UnloadDetectorHelper, InstanceControl::PRIORITY_DETECT_UNLOAD> UnloadDetector;

} // namespace Firebird


#endif // FB_COMMON_CLASSES_IMPLEMENT_HELPER
