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

namespace Firebird {

#ifdef WIN_NT

#define MISS_SPIN_COUNT ((tSetCriticalSectionSpinCount *)(-1))
#define INIT_SPIN_COUNT ((tSetCriticalSectionSpinCount *)(0))

tSetCriticalSectionSpinCount* 
	Spinlock::SetCriticalSectionSpinCount = INIT_SPIN_COUNT;

Spinlock::init() {
	if (SetCriticalSectionSpinCount == MISS_SPIN_COUNT)
		return;
	if (SetCriticalSectionSpinCount == INIT_SPIN_COUNT) {
		HMODULE kernel32 = LoadLibrary("kernel32.dll");
		if (! kernel32) {
			SetCriticalSectionSpinCount = MISS_SPIN_COUNT;
			return;
		}
		SetCriticalSectionSpinCount = 
			(tSetCriticalSectionSpinCount *) GetProcAddress(
					kernel32, "SetCriticalSectionSpinCount");
		if (! SetCriticalSectionSpinCount) {
			SetCriticalSectionSpinCount = MISS_SPIN_COUNT;
			return;
		}
	}
	SetCriticalSectionSpinCount(&spinlock, 4000);
}

#endif  // WIN_NT


// in some cases recursive mutex's are not protected by try/catch
// therefore keep old (return value) logic 

int RecursiveMutex::enter()
{
	if (threadId == getThreadId())
	{
		++count;
	}
	else 
	{
	    try 
		{
			mutex.enter();
		}
		catch (const Firebird::system_call_failed& e)
		{
			return e.getErrorCode();
		}
		threadId = getThreadId();
		count = 1;
	}
	return 0;
}


int RecursiveMutex::leave()
{
	if (threadId != getThreadId())
	{
		return FB_FAILURE;
	}

	if (--count == 0)
	{
		threadId = 0;
	    try 
		{
			mutex.leave();
		}
		catch (const Firebird::system_call_failed& e)
		{
			return e.getErrorCode();
		}
	}
	return 0;
}


}		// namespace Firebird
