/*
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
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 * Adriano dos Santos Fernandes
 */

#include <stdlib.h>
#include "ib_udf.h"
#include "firebird.h"
#include "../jrd/common.h"
#include "../common/classes/array.h"
#include "../common/classes/init.h"
#include "../common/classes/locks.h"

using namespace Firebird;

namespace
{
	GlobalPtr<Mutex> mutex;
	InitInstance<SortedArray<void*> > pointers;
}


extern "C" void* EXPORT ib_util_malloc(long size)
{
	void* const ptr = malloc(size);

	Firebird::MutexLockGuard guard(mutex);
	pointers().add(ptr);

	return ptr;
}


extern "C" bool EXPORT ib_util_free(void* ptr)
{
	Firebird::MutexLockGuard guard(mutex);
	size_t pos;

	if (pointers().find(ptr, pos))
	{
		pointers().remove(pos);
		free(ptr);
		return true;
	}

	return false;
}
