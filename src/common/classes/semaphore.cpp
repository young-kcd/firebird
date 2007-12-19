/*
 *	PROGRAM:		Client/Server Common Code
 *	MODULE:			locks.cpp
 *	DESCRIPTION:	Darwin specific semaphore support
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
 *  Copyright (c) 2007 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../../common/classes/semaphore.h"
#include "../../common/classes/alloc.h"

#ifdef MIXED_SEMAPHORE_AND_FILE_HANDLE

/*
There is a problem with values returned by sem_open() in MacOS 10.5 and earlier.
They are not pointers. They are handles, in format of file handle (0,1,2,3,...),
and worse - they share same space with actual file handles. And when we close 
unneeded file handles in divorce_terminal(), we kill all global semaphores.
MIXED_SEMAPHORE_AND_FILE_HANDLE exists to avoid it.
*/

#include <sys/param.h>

namespace Firebird {

SignalSafeSemaphore* SignalSafeSemaphore::initialList = 0;
bool SignalSafeSemaphore::divorceDone = false;

void SignalSafeSemaphore::linkToInitialList()
{
	if (divorceDone)
	{
		return;
	}

	unsigned int n = (U_IPTR) sem;
	if (n < NOFILE)
	{
		next = initialList;
		initialList = this;
	}
	else
	{
		next = 0;
	}
}

bool SignalSafeSemaphore::checkHandle(int n)
{
	divorceDone = true;

	for (SignalSafeSemaphore* cur = initialList; cur; cur = cur->next)
	{
		if (((IPTR) (cur->sem)) == n)
		{
			return true;
		}
	}

	return false;
}

}		// namespace Firebird

#endif  // MIXED_SEMAPHORE_AND_FILE_HANDLE
