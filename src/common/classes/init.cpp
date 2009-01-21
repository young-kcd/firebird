/*
 *	PROGRAM:	Common Access Method
 *	MODULE:		init.cpp
 *	DESCRIPTION:	InstanceControl - class to control global ctors/dtors
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
 *  The Original Code was created by Alexander Peshkoff
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2008 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "init.h"
#include "alloc.h"

// Setting this define helps (with AV at exit time) detect globals
// with destructors, declared not using InstanceControl.
// The reason for AV is that process memory pool (from where globals should allocate memory)
// is destoyed in atexit(), before destructors are called. Therefore each delete
// operator in destructor will cause AV.
#undef DEBUG_INIT

namespace
{
#ifdef DEV_BUILD
	void cleanError(const Firebird::Exception* e)
	{
		// This is done to be able to look at status in debugger
		ISC_STATUS_ARRAY status;
		if (e)
		{
			Firebird::CircularStringsBuffer<4096> localBuffer;
			e->stuff_exception(status, &localBuffer);
		}
		// we do not have big choice in error reporting when running destructors
		abort();
	}
#else
	void cleanError(const Firebird::Exception*) { }
#endif

	// This helps initialize globals, needed before regular ctors run
	bool initDone = false;

	void allClean()
	{
		Firebird::InstanceControl::destructors();

		try
		{
			Firebird::StaticMutex::release();
		}
		catch (...)
		{
			cleanError(NULL);
		}

		try
		{
			Firebird::MemoryPool::cleanup();
		}
		catch (...)
		{
			cleanError(NULL);
		}
	}

#ifndef DEBUG_INIT
	// This class with it's single instance ensures global cleanup
	class Cleanup
	{
	public:
		~Cleanup()
		{
			allClean();
		}
	};

	Cleanup global;
#endif //DEBUG_INIT

	void init()
	{
		if (initDone)
		{
			return;
		}

		Firebird::Mutex::initMutexes();
		Firebird::MemoryPool::init();
		Firebird::StaticMutex::create();

#ifdef DEBUG_INIT
		atexit(allClean);
#endif //DEBUG_INIT

		initDone = true;
	}
}


namespace Firebird
{

	InstanceControl* InstanceControl::instanceList = 0;
	FPTR_VOID InstanceControl::gdsCleanup = 0;
	FPTR_VOID InstanceControl::gdsShutdown = 0;

	InstanceControl::InstanceControl(DtorPriority p)
		: priority(p)
	{
		// Initialize required subsystems, including static mutex, needed next line
		init();

		// Ready to be used in static variables inside functions
		MutexLockGuard guard(*mutex);
		next = instanceList;
		instanceList = this;
	}

	void InstanceControl::destructors()
	{
		// Call fb_shutdown() if needed
		if (gdsShutdown)
		{
			try
			{
				gdsShutdown();
			}
			catch (const Firebird::Exception& e)
			{
				cleanError(&e);
			}
		}

		// Call gds__cleanup() if present
		if (gdsCleanup)
		{
			try
			{
				gdsCleanup();
			}
			catch (const Firebird::Exception& e)
			{
				cleanError(&e);
			}
		}

		// Destroy global objects
		DtorPriority currentPriority = PRIORITY_REGULAR, nextPriority = currentPriority;

		do
		{
			currentPriority = nextPriority;

			for (InstanceControl* i = instanceList; i; i = i->next)
			{
				if (i->priority == currentPriority)
				{
					try
					{
						i->dtor();
					}
					catch (const Firebird::Exception& e)
					{
						cleanError(&e);
					}
				}
				else if (i->priority > currentPriority)
				{
					if (nextPriority == currentPriority || i->priority < nextPriority)
					{
						nextPriority = i->priority;
					}
				}
			}
		} while (nextPriority != currentPriority);
	}

	void InstanceControl::registerGdsCleanup(FPTR_VOID cleanup)
	{
		fb_assert(!gdsCleanup || !cleanup);
		gdsCleanup = cleanup;
	}

	void InstanceControl::registerShutdown(FPTR_VOID shutdown)
	{
		fb_assert(!gdsShutdown || !shutdown);
		gdsShutdown = shutdown;
	}

	Mutex* StaticMutex::mutex = NULL;

	void StaticMutex::create()
	{
		static char staticMutexPlace[sizeof(Firebird::Mutex)];
		mutex = new(staticMutexPlace) Mutex;
	}

	void StaticMutex::release()
	{
		mutex->~Mutex();
	}
}
