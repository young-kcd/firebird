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
#include "ProviderInterface.h"
#include "../common/classes/alloc.h"
#include "gen/iberror.h"
#include "../yvalve/gds_proto.h"
#include "../common/classes/init.h"
#include "../common/classes/auto.h"
#include "../common/classes/RefCounted.h"
#include "consts_pub.h"

namespace Firebird {

// If you need interface on stack, use template AutoPtr<YourInterface, AutoDisposable>
// as second parameter to store it.
class AutoDisposable
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

// Helps to implement disposable interfaces
template <class C, typename S = GlobalStorage>
class DisposeIface : public C, public S
{
public:
	DisposeIface() { }

private:
	DisposeIface(const DisposeIface&);
	DisposeIface& operator=(const DisposeIface&);
};

// Helps to implement disposable interfaces on stack or static
template <class C, typename S = GlobalStorage>
class StackIface : public DisposeIface<C, S>
{
public:
	void FB_CARG dispose()
	{ }

	void* operator new(size_t, void* memory) throw()
	{
		return memory;
	}
};

// Helps to implement standard interfaces
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


// Helps to implement plugins
template <class C, int V, typename S = GlobalStorage>
class StdPlugin : public StdIface<C, V, S>
{
private:
	IInterface* own;

public:
	StdPlugin() : own(NULL)
	{ }

	IInterface* FB_CARG owner(IInterface* iface)
	{
		IInterface* rc = own;
		own = iface;
		return rc;
	}
};


// Trivial factory
template <class P>
class SimpleFactoryBase : public StackIface<IPluginFactory>
{
public:
	IPluginBase* FB_CARG createPlugin(IPluginConfig* factoryParameter)
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


// Master interface access
class MasterInterfacePtr : public AutoPtr<IMaster, AutoDisposable>
{
public:
	MasterInterfacePtr() : AutoPtr<IMaster, AutoDisposable>(fb_get_master_interface())
	{ }
};


// Generic plugins interface access
class PluginManagerInterfacePtr : public AutoPtr<IPluginManager, AutoDisposable>
{
public:
	PluginManagerInterfacePtr() : AutoPtr<IPluginManager, AutoDisposable>(fb_get_master_interface()->getPluginManager())
	{ }
	PluginManagerInterfacePtr(IMaster* master) : AutoPtr<IPluginManager, AutoDisposable>(master->getPluginManager())
	{ }
};


// Control timer interface access
class TimerInterfacePtr : public AutoPtr<ITimerControl, AutoDisposable>
{
public:
	TimerInterfacePtr() : AutoPtr<ITimerControl, AutoDisposable>(fb_get_master_interface()->getTimerControl())
	{ }
};


// When process exits, dynamically loaded modules (for us plugin modules)
// are unloaded first. As the result all global variables in plugin are already destroyed
// when yvalve is starting fb_shutdown(). This causes almost unavoidable segfault.
// To avoid it this class is added - it detects spontaneous (not by PluginManager)
// module unload and notifies PluginManager about this said fact.
class DummyStorage
{
};

class UnloadDetectorHelper : public DisposeIface<IPluginModule, DummyStorage>
{
public:
	explicit UnloadDetectorHelper(MemoryPool&)
		: flagOsUnload(true)
	{ }

	~UnloadDetectorHelper()
	{
		if (flagOsUnload)
		{
			PluginManagerInterfacePtr pi;
			pi->unregisterModule(this);
			pi->moduleUnloaded();

			flagOsUnload = false;
		}
	}

	bool unloadStarted()
	{
		return !flagOsUnload;
	}

	void FB_CARG dispose()
	{
		// delete this;	-don't do that!
	}

private:
	bool flagOsUnload;

	void FB_CARG doClean()
	{
		flagOsUnload = false;
	}
};

typedef GlobalPtr<UnloadDetectorHelper, InstanceControl::PRIORITY_DETECT_UNLOAD> UnloadDetector;


class BlrMessage
{
public:
	BlrMessage(unsigned aBlrLength, const unsigned char* aBlr, unsigned aBufferLength)
		: blrLength(aBlrLength),
		  blr(aBlr),
		  bufferLength(aBufferLength)
	{
	}

public:
	unsigned blrLength;
	const unsigned char* blr;
	unsigned bufferLength;
};

class BlrMessageBuffer : public IBlrMessage
{
public:
	BlrMessageBuffer(const  BlrMessage* aMessage, unsigned char* aBuffer)
		: message(aMessage),
		  buffer(aBuffer)
	{
	}

private:
	const BlrMessage* const message;
	unsigned char* const buffer;

public:
	unsigned int FB_CARG getBlrLength() const
	{
		return message ? message->blrLength : 0;
	}
	const unsigned char* FB_CARG getBlr() const
	{
		return message ? message->blr : NULL;
	}
	unsigned int FB_CARG getBufferLength() const
	{
		return message ? message->bufferLength : 0;
	}
	unsigned char* FB_CARG getBuffer() const
	{
		return buffer;
	}
};


} // namespace Firebird


#endif // FB_COMMON_CLASSES_IMPLEMENT_HELPER
