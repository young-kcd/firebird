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
#include "firebird/Interface.h"

#include <string.h>

#include "../yvalve/MasterImplementation.h"
#include "../common/classes/init.h"
#include "../common/MsgMetadata.h"
#include "../common/StatusHolder.h"
#include "../yvalve/PluginManager.h"
#include "../common/classes/GenericMap.h"
#include "../common/classes/fb_pair.h"
#include "../common/classes/rwlock.h"
#include "../common/classes/semaphore.h"
#include "../common/isc_proto.h"
#include "../common/ThreadStart.h"
#include "../common/utils_proto.h"
#include "../jrd/ibase.h"
#include "../yvalve/utl_proto.h"

using namespace Firebird;

namespace Why {

//
// getStatus()
//

class UserStatus FB_FINAL : public Firebird::DisposeIface<Firebird::BaseStatus<UserStatus> >
{
public:
	// IStatus implementation
	void dispose()
	{
		delete this;
	}
};

IStatus* MasterImplementation::getStatus()
{
	return new UserStatus;
}


//
// getDispatcher()
//

IProvider* MasterImplementation::getDispatcher()
{
	IProvider* dispatcher = new Dispatcher;
	dispatcher->addRef();
	return dispatcher;
}

//
// getDtc()
//

Static<Dtc> MasterImplementation::dtc;

Dtc* MasterImplementation::getDtc()
{
	return &dtc;
}

//
// getPluginManager()
//

IPluginManager* MasterImplementation::getPluginManager()
{
	static Static<PluginManager> manager;

	return &manager;
}

IMetadataBuilder* MasterImplementation::getMetadataBuilder(CheckStatusWrapper* status, unsigned fieldCount)
{
	try
	{
		IMetadataBuilder* bld = new MetadataBuilder(fieldCount);
		bld->addRef();
		return bld;
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
		return NULL;
	}
}

/***
IDebug* MasterImplementation::getDebug()
{
#ifdef DEV_BUILD
	return getImpDebug();
#else
	return NULL;
#endif
}
***/

int MasterImplementation::serverMode(int mode)
{
	static int currentMode = -1;
	if (mode >= 0)
		currentMode = mode;
	return currentMode;
}

} // namespace Why

//
// circularAlloc()
//

#ifdef WIN_NT
#include <windows.h>
#endif

#ifdef USE_POSIX_THREADS
namespace {

class ThreadCleanup
{
public:
	static void add(FPTR_VOID_PTR cleanup, void* arg);
	static void remove(FPTR_VOID_PTR cleanup, void* arg);
	static void destructor(void*);

private:
	FPTR_VOID_PTR function;
	void* argument;
	ThreadCleanup* next;

	ThreadCleanup(FPTR_VOID_PTR cleanup, void* arg, ThreadCleanup* chain)
		: function(cleanup), argument(arg), next(chain) { }

	static ThreadCleanup** findCleanup(FPTR_VOID_PTR cleanup, void* arg);
};

pthread_key_t key;
pthread_once_t keyOnce = PTHREAD_ONCE_INIT;
bool keySet = false;

void makeKey()
{
	int err = pthread_key_create(&key, ThreadCleanup::destructor);
	if (err)
	{
		Firebird::system_call_failed("pthread_key_create", err);
	}
	keySet = true;
}

void initThreadCleanup()
{
	int err = pthread_once(&keyOnce, makeKey);
	if (err)
	{
		Firebird::system_call_failed("pthread_once", err);
	}

	err = pthread_setspecific(key, &key);
	if (err)
	{
		Firebird::system_call_failed("pthread_setspecific", err);
	}
}

ThreadCleanup* chain = NULL;

class FiniThreadCleanup
{
public:
	FiniThreadCleanup(Firebird::MemoryPool&)
	{ }

	~FiniThreadCleanup()
	{
		fb_assert(!chain);
		if (keySet)
		{
			int err = pthread_key_delete(key);
			if (err)
				Firebird::system_call_failed("pthread_key_delete", err);
		}
	}
};

Firebird::GlobalPtr<Firebird::Mutex> cleanupMutex;
Firebird::GlobalPtr<FiniThreadCleanup> thrCleanup;		// needed to call dtor


ThreadCleanup** ThreadCleanup::findCleanup(FPTR_VOID_PTR cleanup, void* arg)
{
	for (ThreadCleanup** ptr = &chain; *ptr; ptr = &((*ptr)->next))
	{
		if ((*ptr)->function == cleanup && (*ptr)->argument == arg)
		{
			return ptr;
		}
	}

	return NULL;
}

void ThreadCleanup::destructor(void*)
{
	Firebird::MutexLockGuard guard(cleanupMutex, FB_FUNCTION);

	for (ThreadCleanup* ptr = chain; ptr; ptr = ptr->next)
	{
		ptr->function(ptr->argument);
	}

	pthread_setspecific(key, NULL);
}

void ThreadCleanup::add(FPTR_VOID_PTR cleanup, void* arg)
{
	Firebird::MutexLockGuard guard(cleanupMutex, FB_FUNCTION);

	initThreadCleanup();

	if (findCleanup(cleanup, arg))
	{
		return;
	}

	chain = FB_NEW(*getDefaultMemoryPool()) ThreadCleanup(cleanup, arg, chain);
}

void ThreadCleanup::remove(FPTR_VOID_PTR cleanup, void* arg)
{
	Firebird::MutexLockGuard guard(cleanupMutex, FB_FUNCTION);

	ThreadCleanup** ptr = findCleanup(cleanup, arg);
	if (!ptr)
	{
		return;
	}

	ThreadCleanup* toDelete = *ptr;
	*ptr = toDelete->next;
	delete toDelete;
}

} // anonymous namespace
#endif // USE_POSIX_THREADS

namespace {

class StringsBuffer
{
private:
	class ThreadBuffer : public Firebird::GlobalStorage
	{
	private:
		const static size_t BUFFER_SIZE = 8192;		// make it match with call stack limit == 2048
		char buffer[BUFFER_SIZE];
		char* buffer_ptr;
		ThreadId thread;

	public:
		explicit ThreadBuffer(ThreadId thr) : buffer_ptr(buffer), thread(thr) { }

		const char* alloc(const char* string, size_t length)
		{
			// if string is already in our buffer - return it
			// it was already saved in our buffer once
			if (string >= buffer && string < &buffer[BUFFER_SIZE])
				return string;

			// if string too long, truncate it
			if (length > BUFFER_SIZE / 4)
				length = BUFFER_SIZE / 4;

			// If there isn't any more room in the buffer, start at the beginning again
			if (buffer_ptr + length + 1 > buffer + BUFFER_SIZE)
				buffer_ptr = buffer;

			char* new_string = buffer_ptr;
			memcpy(new_string, string, length);
			new_string[length] = 0;
			buffer_ptr += length + 1;

			return new_string;
		}

		bool thisThread(ThreadId currTID)
		{
#ifdef WIN_NT
			if (thread != currTID)
			{
				HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION, false, thread);
				// commented exit code check - looks like OS does not return handle
				// for already exited thread
				//DWORD exitCode = STILL_ACTIVE;
				if (hThread)
				{
					//GetExitCodeThread(hThread, &exitCode);
					CloseHandle(hThread);
				}

				//if ((!hThread) || (exitCode != STILL_ACTIVE))
				if (!hThread)
				{
					// Thread does not exist any more
					thread = currTID;
				}
			}
#endif

			return thread == currTID;
		}
	};

	typedef Firebird::Array<ThreadBuffer*> ProcessBuffer;

	ProcessBuffer processBuffer;
	Firebird::Mutex mutex;

public:
	explicit StringsBuffer(Firebird::MemoryPool& p)
		: processBuffer(p)
	{
	}

	~StringsBuffer()
	{
#ifdef USE_POSIX_THREADS
		ThreadCleanup::remove(cleanupAllStrings, this);
#endif
	}

private:
	FB_SIZE_T position(ThreadId thr)
	{
		fb_assert(mutex.locked());

		for (FB_SIZE_T i = 0; i < processBuffer.getCount(); ++i)
		{
			if (processBuffer[i]->thisThread(thr))
			{
				return i;
			}
		}

		return processBuffer.getCount();
	}

	ThreadBuffer* getThreadBuffer(ThreadId thr)
	{
		Firebird::MutexLockGuard guard(mutex, FB_FUNCTION);

		FB_SIZE_T p = position(thr);
		if (p < processBuffer.getCount())
		{
			return processBuffer[p];
		}

#ifdef USE_POSIX_THREADS
		ThreadCleanup::add(cleanupAllStrings, this);
#endif
		ThreadBuffer* b = new ThreadBuffer(thr);
		processBuffer.add(b);
		return b;
	}

	void cleanup()
	{
		Firebird::MutexLockGuard guard(mutex, FB_FUNCTION);

		///fprintf(stderr, "Cleanup is called\n");
		FB_SIZE_T p = position(getThreadId());
		if (p >= processBuffer.getCount())
		{
			return;
		}

		delete processBuffer[p];
		processBuffer.remove(p);
		///fprintf(stderr, "Buffer removed\n");
	}

	static void cleanupAllStrings(void* toClean)
	{
		static_cast<StringsBuffer*>(toClean)->cleanup();
	}

public:
	const char* alloc(const char* s, size_t len, ThreadId thr = getThreadId())
	{
		return getThreadBuffer(thr)->alloc(s, len);
	}
};

Firebird::GlobalPtr<StringsBuffer> allStrings;

} // anonymous namespace

namespace Why {

const char* MasterImplementation::circularAlloc(const char* s, unsigned len, intptr_t thr)
{
	return allStrings->alloc(s, len, (ThreadId) thr);
}

} // namespace Why


//
// timer
//

namespace Why {

namespace {

// Protects timerQueue array
GlobalPtr<Mutex> timerAccess;
// Protects from races during module unload process
// Should be taken before timerAccess
GlobalPtr<Mutex> timerPause;

GlobalPtr<Semaphore> timerWakeup;
// Should use atomic flag for thread stop to provide correct membar
AtomicCounter stopTimerThread(0);
Thread::Handle timerThreadHandle = 0;

struct TimerEntry
{
	ISC_UINT64 fireTime;
	ITimer* timer;

	static const ISC_UINT64 generate(const TimerEntry& item) { return item.fireTime; }
	static THREAD_ENTRY_DECLARE timeThread(THREAD_ENTRY_PARAM);

	static void init()
	{
		Thread::start(timeThread, 0, 0, &timerThreadHandle);
	}

	static void cleanup();
};

typedef SortedArray<TimerEntry, InlineStorage<TimerEntry, 64>, ISC_UINT64, TimerEntry> TimerQueue;
GlobalPtr<TimerQueue> timerQueue;

InitMutex<TimerEntry> timerHolder("TimerHolder");

void TimerEntry::cleanup()
{
	{
		MutexLockGuard guard(timerAccess, FB_FUNCTION);

		stopTimerThread.setValue(1);
		timerWakeup->release();
	}
	Thread::waitForCompletion(timerThreadHandle);

	while (timerQueue->hasData())
	{
		ITimer* timer = NULL;
		{
			MutexLockGuard guard(timerAccess, FB_FUNCTION);

			TimerEntry* e = timerQueue->end();
			timer = (--e)->timer;
			timerQueue->remove(e);
		}
		timer->release();
	}
}

ISC_UINT64 curTime()
{
	ISC_UINT64 rc = fb_utils::query_performance_counter();
	rc *= 1000000;
	rc /= fb_utils::query_performance_frequency();
	return rc;
}

TimerEntry* getTimer(ITimer* timer)
{
	fb_assert(timerAccess->locked());

	for (unsigned int i = 0; i < timerQueue->getCount(); ++i)
	{
		TimerEntry& e(timerQueue->operator[](i));
		if (e.timer == timer)
		{
			return &e;
		}
	}

	return NULL;
}

THREAD_ENTRY_DECLARE TimerEntry::timeThread(THREAD_ENTRY_PARAM)
{
	while (stopTimerThread.value() == 0)
	{
		ISC_UINT64 microSeconds = 0;

		{
			MutexLockGuard pauseGuard(timerPause, FB_FUNCTION);
			MutexLockGuard guard(timerAccess, FB_FUNCTION);

			const ISC_UINT64 cur = curTime();

			while (timerQueue->getCount() > 0)
			{
				TimerEntry e(timerQueue->operator[](0));

				if (e.fireTime <= cur)
				{
					timerQueue->remove((FB_SIZE_T) 0);

					// We must leave timerAccess mutex here to avoid deadlocks
					MutexUnlockGuard ug(timerAccess, FB_FUNCTION);

					e.timer->handler();
					e.timer->release();
				}
				else
				{
					microSeconds = e.fireTime - cur;
					break;
				}
			}
		}

		if (microSeconds)
		{
			timerWakeup->tryEnter(0, microSeconds / 1000);
		}
		else
		{
			timerWakeup->enter();
		}
	}

	return 0;
}

} // namespace

class TimerImplementation :
	public AutoIface<ITimerControlImpl<TimerImplementation, CheckStatusWrapper> >
{
public:
	// ITimerControl implementation
	void start(CheckStatusWrapper* status, ITimer* timer, ISC_UINT64 microSeconds)
	{
		try
		{
			MutexLockGuard guard(timerAccess, FB_FUNCTION);

			if (stopTimerThread.value() != 0)
			{
				// Ignore an attempt to start timer - anyway thread to make it fire is down

				// We must leave timerAccess mutex here to avoid deadlocks
				MutexUnlockGuard ug(timerAccess, FB_FUNCTION);

				timer->addRef();
				timer->release();
				return;
			}

			timerHolder.init();

			TimerEntry* curTimer = getTimer(timer);
			if (!curTimer)
			{
				TimerEntry newTimer;

				newTimer.timer = timer;
				newTimer.fireTime = curTime() + microSeconds;
				timerQueue->add(newTimer);
				timer->addRef();
			}
			else
			{
				curTimer->fireTime = curTime() + microSeconds;
			}

			timerWakeup->release();
		}
		catch (const Firebird::Exception& ex)
		{
			ex.stuffException(status);
		}
	}

	void stop(CheckStatusWrapper* status, ITimer* timer)
	{
		try
		{
			MutexLockGuard guard(timerAccess, FB_FUNCTION);

			TimerEntry* curTimer = getTimer(timer);
			if (curTimer)
			{
				curTimer->timer->release();
				timerQueue->remove(curTimer);
			}
		}
		catch (const Firebird::Exception& ex)
		{
			ex.stuffException(status);
		}
	}
};

ITimerControl* MasterImplementation::getTimerControl()
{
	static Static<TimerImplementation> timer;

	return &timer;
}

void shutdownTimers()
{
	timerHolder.cleanup();
}

Mutex& pauseTimer()
{
	return timerPause;
}

} // namespace Why


//
// Util (misc calls)
//

namespace Why {

	extern UtilInterface utilInterface;		// Implemented in utl.cpp

	IUtil* MasterImplementation::getUtilInterface()
	{
		return &utilInterface;
	}

} // namespace Why


//
// ConfigManager (config info access)
//

namespace Firebird {

	extern IConfigManager* iConfigManager;

} // namespace Firebird

namespace Why {

	IConfigManager* MasterImplementation::getConfigManager()
	{
		return Firebird::iConfigManager;
	}

} // namespace Why


//
// get master
//

namespace Firebird
{
	typedef IMaster* IMasterPtr;

	extern "C" IMasterPtr API_ROUTINE fb_get_master_interface()
	{
		static Static<Why::MasterImplementation> instance;
		return &instance;
	}
}
