/*
 *	PROGRAM:		Client/Server Common Code
 *	MODULE:			locks.cpp
 *	DESCRIPTION:	Win32 Spinlock support compatible with 
 *					old OS versions (like Windows 95)
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * Created by: Alex Peshkoff <peshkoff@hotmail.ru>
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#include "../../include/firebird.h"
#include "../../common/classes/locks.h"

namespace Firebird {

#ifdef SUPERSERVER
#ifdef WIN_NT

#define MISS_SPIN_COUNT ((tSetCriticalSectionSpinCount *)(-1))
#define INIT_SPIN_COUNT ((tSetCriticalSectionSpinCount *)(0))

tSetCriticalSectionSpinCount* 
	Spinlock::SetCriticalSectionSpinCount = INIT_SPIN_COUNT;

Spinlock::Spinlock() {
	InitializeCriticalSection(&spinlock);
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
#endif	// SUPERSERVER

}		// namespace Firebird
