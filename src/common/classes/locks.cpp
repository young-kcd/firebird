/*
 *	PROGRAM:		Client/Server Common Code
 *	MODULE:			locks.cpp
 *	DESCRIPTION:	Win32 Mutex support compatible with
 *					old OS versions (like Windows 95)
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
 *  Copyright (c) 2003 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "../../include/firebird.h"
#include "../../common/classes/locks.h"
#include "../../common/thd.h"
#include "../../jrd/common.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

namespace Firebird {

#if defined(WIN_NT)

TryEnterCS::tTryEnterCriticalSection* TryEnterCS::m_funct = &TryEnterCS::notImpl;

static TryEnterCS tryEnterCS;

TryEnterCS::TryEnterCS()
{
	HMODULE kernel32 = GetModuleHandle("kernel32.dll");
	if (kernel32) {
		m_funct = (tTryEnterCriticalSection*)
			GetProcAddress(kernel32, "TryEnterCriticalSection");
	}
}


#define MISS_SPIN_COUNT ((tSetCriticalSectionSpinCount *)(-1))
#define INIT_SPIN_COUNT ((tSetCriticalSectionSpinCount *)(0))

tSetCriticalSectionSpinCount* Spinlock::SetCriticalSectionSpinCount = INIT_SPIN_COUNT;

void Spinlock::init()
{
	if (SetCriticalSectionSpinCount == MISS_SPIN_COUNT)
		return;

	if (SetCriticalSectionSpinCount == INIT_SPIN_COUNT) {
		HMODULE kernel32 = GetModuleHandle("kernel32.dll");
		if (!kernel32) {
			SetCriticalSectionSpinCount = MISS_SPIN_COUNT;
			return;
		}
		SetCriticalSectionSpinCount =
			(tSetCriticalSectionSpinCount *) GetProcAddress(
					kernel32, "SetCriticalSectionSpinCount");
		if (!SetCriticalSectionSpinCount) {
			SetCriticalSectionSpinCount = MISS_SPIN_COUNT;
			return;
		}
	}

	SetCriticalSectionSpinCount(&spinlock, 4000);
}

#elif defined(SOLARIS_MT)

#error Fix me!

#else //posix mutex

pthread_mutexattr_t Mutex::attr;

void Mutex::initMutexes()
{
	// Throw exceptions on errors, but they will not be processed in init
	// (first constructor). Better logging facilities are required here.
	int rc = pthread_mutexattr_init(&attr);
	if (rc < 0)
		system_call_failed::raise("pthread_mutexattr_init", rc);

	rc = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	if (rc < 0)
		system_call_failed::raise("pthread_mutexattr_settype", rc);
}

#endif

} // namespace Firebird
