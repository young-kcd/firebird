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
	void cleanError()
	{
#ifdef DEV_BUILD
		// we do not have big choice in error reporting when running destructors
		abort();
#endif
	}

	// This helps initialize globals, needed before regular ctors run
	bool initDone = false;

	void allClean()
	{
		Firebird::InstanceControl::destructors();
		try
		{
			Firebird::StaticMutex::release();
		}
		catch(...)
		{
			cleanError();
		}
		try
		{
			Firebird::MemoryPool::cleanup();
		}
		catch(...)
		{
			cleanError();
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

	InstanceControl::InstanceControl()
	{
		init();
		next = instanceList;
		instanceList = this;
	}

	void InstanceControl::destructors()
	{
		// Call gds__cleanup() if present
		if (gdsCleanup)
		{
			try
			{
				gdsCleanup();
			}
			catch(...)
			{
				cleanError();
			}
		}

		// Destroy global objects
		for (InstanceControl* i = instanceList; i; i = i->next)
		{
			try
			{
				i->dtor();
			}
			catch(...)
			{
				cleanError();
			}
		}
	}

	void InstanceControl::registerGdsCleanup(FPTR_VOID cleanup)
	{
		fb_assert((!gdsCleanup) || (!cleanup));
		gdsCleanup = cleanup;
	}

	RecursiveMutex* StaticMutex::mutex = 0;

	void StaticMutex::create()
	{
		mutex = FB_NEW(*getDefaultMemoryPool()) RecursiveMutex;
	}

	void StaticMutex::release()
	{
		delete mutex;
	}
}
