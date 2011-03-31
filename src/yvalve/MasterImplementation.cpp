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
#include "Timer.h"

#include "../yvalve/MasterImplementation.h"
#include "../common/classes/ImplementHelper.h"
#include "../common/classes/init.h"
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

namespace Firebird {

class MasterImplementation : public StackIface<IMaster>
{
public:
	Status* FB_CARG getStatusInstance();
	IPlugin* FB_CARG getPluginInterface();
	int FB_CARG upgradeInterface(Interface* toUpgrade, int desiredVersion, void* missingFunctionClass);
	const char* FB_CARG circularAlloc(const char* s, size_t len, intptr_t thr);
	ITimerControl* FB_CARG getTimerControl();
};

//
// getStatusInstance()
//

class UserStatus : public Firebird::DisposeIface<Firebird::BaseStatus>
{
private:
	void FB_CARG dispose()
	{
		delete this;
	}
};

Firebird::Status* FB_CARG Firebird::MasterImplementation::getStatusInstance()
{
	return new UserStatus;
}

//
// getPluginInterface()
//

IPlugin* FB_CARG MasterImplementation::getPluginInterface()
{
	static Static<PluginManager> manager;

	return &manager;
}

//
// upgradeInterface()
//

namespace
{
	typedef void function();
	typedef function* FunctionPtr;

	struct CVirtualClass
	{
		FunctionPtr* vTab;
	};

	typedef Firebird::Pair<Firebird::NonPooled<U_IPTR, FunctionPtr*> > FunctionPair;
	GlobalPtr<GenericMap<FunctionPair> > functionMap;
	GlobalPtr<RWLock> mapLock;
}

int FB_CARG MasterImplementation::upgradeInterface(Interface* toUpgrade,
												   int desiredVersion,
												   void* missingFunctionClass)
{
	if (toUpgrade->version() >= desiredVersion)
		return 0;

	FunctionPtr* newTab = NULL;
	try
	{
		CVirtualClass* target = (CVirtualClass*) toUpgrade;

		{ // sync scope
			ReadLockGuard sync(mapLock);
			if (functionMap->get((U_IPTR) target->vTab, newTab))
			{
				target->vTab = newTab;
				return 0;
			}
		}

		WriteLockGuard sync(mapLock);

		if (!functionMap->get((U_IPTR) target->vTab, newTab))
		{
			CVirtualClass* miss = (CVirtualClass*) missingFunctionClass;
			newTab = FB_NEW(*getDefaultMemoryPool()) FunctionPtr[desiredVersion];

			for (int i = 0; i < toUpgrade->version(); ++i)
				newTab[i] = target->vTab[i];
			for (int j = toUpgrade->version(); j < desiredVersion; ++j)
				newTab[j] = miss->vTab[0];

			functionMap->put((U_IPTR) target->vTab, newTab);
		}

		target->vTab = newTab;
	}
	catch (const Exception& ex)
	{
		ISC_STATUS_ARRAY s;
		ex.stuff_exception(s);
		iscLogStatus("upgradeInterface", s);
		if (newTab)
		{
			delete[] newTab;
		}
		return -1;
	}

	return 0;
}

} // namespace Firebird

//
// circularAlloc()
//

#ifdef WIN_NT
#include <windows.h>
#else
#ifndef USE_THREAD_DESTRUCTOR
#include <pthread.h>
#include <signal.h>
#endif
#endif

namespace {

class StringsBuffer
{
private:
	class ThreadBuffer : public Firebird::GlobalStorage
	{
	private:
		const static size_t BUFFER_SIZE = 4096;
		char buffer[BUFFER_SIZE];
		char* buffer_ptr;
		FB_THREAD_ID thread;

	public:
		explicit ThreadBuffer(FB_THREAD_ID thr) : buffer_ptr(buffer), thread(thr) { }

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

		bool thisThread(FB_THREAD_ID currTID)
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
#else
#ifndef USE_THREAD_DESTRUCTOR
			if (thread != currTID)
			{
				if (pthread_kill(thread, 0) == ESRCH)
				{
					// Thread does not exist any more
					thread = currTID;
				}
			}
#endif // USE_THREAD_DESTRUCTOR
#endif // WIN_NT

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
		ThreadCleanup::remove(cleanupAllStrings, this);
	}

private:
	size_t position(FB_THREAD_ID thr)
	{
		// mutex should be locked when this function is called

		for (size_t i = 0; i < processBuffer.getCount(); ++i)
		{
			if (processBuffer[i]->thisThread(thr))
			{
				return i;
			}
		}

		return processBuffer.getCount();
	}

	ThreadBuffer* getThreadBuffer(FB_THREAD_ID thr)
	{
		Firebird::MutexLockGuard guard(mutex);

		size_t p = position(thr);
		if (p < processBuffer.getCount())
		{
			return processBuffer[p];
		}

		ThreadBuffer* b = new ThreadBuffer(thr);
		processBuffer.add(b);
		return b;
	}

	void cleanup()
	{
		Firebird::MutexLockGuard guard(mutex);

		size_t p = position(getThreadId());
		if (p >= processBuffer.getCount())
		{
			return;
		}

		delete processBuffer[p];
		processBuffer.remove(p);
	}

	static void cleanupAllStrings(void* toClean)
	{
		static_cast<StringsBuffer*>(toClean)->cleanup();
	}

public:
	const char* alloc(const char* s, size_t len, FB_THREAD_ID thr = getThreadId())
	{
		ThreadCleanup::add(cleanupAllStrings, this);
		return getThreadBuffer(thr)->alloc(s, len);
	}
};

Firebird::GlobalPtr<StringsBuffer> allStrings;

} // anonymous namespace

namespace Firebird {

const char* FB_CARG MasterImplementation::circularAlloc(const char* s, size_t len, intptr_t thr)
{
	return allStrings->alloc(s, len, thr);
}

} // namespace Firebird


//
// timer
//

namespace Firebird {

namespace {

GlobalPtr<Mutex> timerAccess;
GlobalPtr<Semaphore> timerWakeup;

bool stopThread = false;
Thread::Handle timerThreadHandle = 0;

struct TimerEntry
{
	TimerDelay fireTime;
	ITimer* timer;

	static const TimerDelay generate(const void* /*sender*/, const TimerEntry& item) { return item.fireTime; }
	static THREAD_ENTRY_DECLARE timeThread(THREAD_ENTRY_PARAM);

	static void init()
	{
		Thread::start(timeThread, 0, 0, &timerThreadHandle);
	}

	static void cleanup();
};

typedef SortedArray<TimerEntry, InlineStorage<TimerEntry, 64>, TimerDelay, TimerEntry> TimerQueue;
GlobalPtr<TimerQueue> timerQueue;

InitMutex<TimerEntry> timerHolder;

void TimerEntry::cleanup()
{
	MutexLockGuard guard(timerAccess);

	stopThread = true;
	timerWakeup->release();
	Thread::waitForCompletion(timerThreadHandle);

	while (timerQueue->hasData())
	{
		TimerEntry& e(timerQueue->operator[](timerQueue->getCount() - 1));
		e.timer->release();
		timerQueue->remove(&e);
	}
}

TimerDelay curTime()
{
	return fb_utils::query_performance_counter() / fb_utils::query_performance_frequency();
}

TimerEntry* getTimer(ITimer* timer)
{
	timerAccess->assertLocked();

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
	while (!stopThread)
	{
		TimerDelay microSeconds = 0;

		{
			MutexLockGuard guard(timerAccess);

			const TimerDelay cur = curTime();

			while (timerQueue->getCount() > 0)
			{
				TimerEntry e(timerQueue->operator[](0));

				if (e.fireTime <= cur)
				{
					timerQueue->remove((size_t) 0);
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

class TimerImplementation : public StackIface<ITimerControl>
{
public:
	void FB_CARG start(ITimer* timer, TimerDelay microSeconds)
	{
		MutexLockGuard guard(timerAccess);

		timerHolder.init();

		if (stopThread)
		{
			// ignore an attempt to start timer - anyway thread to make it fire is down
			return;
		}

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

	void FB_CARG stop(ITimer* timer)
	{
		MutexLockGuard guard(timerAccess);

		TimerEntry* curTimer = getTimer(timer);
		if (curTimer)
		{
			curTimer->timer->release();
			timerQueue->remove(curTimer);
		}
	}
};

ITimerControl* FB_CARG MasterImplementation::getTimerControl()
{
	static Static<TimerImplementation> timer;

	return &timer;
}

void shutdownTimers()
{
	timerHolder.cleanup();
}

} // namespace Firebird


//
// get master
//

Firebird::IMaster* ISC_EXPORT fb_get_master_interface()
{
	static Firebird::Static<Firebird::MasterImplementation> master;

	return &master;
}
