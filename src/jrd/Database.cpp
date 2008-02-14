/*
 *      PROGRAM:        JRD access method
 *      MODULE:         Database.cpp
 *      DESCRIPTION:    Common descriptions
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
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 *
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "DecOSF" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 * Claudio Valderrama C.
 *
 */


#include "firebird.h"
#include "../jrd/ibase.h"
#include "../jrd/common.h"
#include "../jrd/ods.h"
#include "../jrd/Database.h"
#include "../jrd/isc.h"
#include "../jrd/tra.h"

// Thread data block / IPC related data blocks
#include "../jrd/ThreadData.h"
#include "../jrd/isc.h"

// recursive mutexes
#include "../common/thd.h"

// Definition of block types for data allocation in JRD
#include "../jrd/jrd_blks.h"
#include "../include/fb_blk.h"


namespace Jrd
{
	bool Database::onRawDevice() const
	{
#ifdef SUPPORT_RAW_DEVICES
		return PIO_on_raw_device(dbb_filename);
#else
		return false;
#endif
	}

	Database::~Database()
	{
		delete dbb_sys_trans;

		destroyIntlObjects();

		fb_assert(dbb_pools[0] == dbb_permanent);
		for (size_t i = 1; i < dbb_pools.getCount(); ++i)
		{
			MemoryPool::deletePool(dbb_pools[i]);
		}
	}

	void Database::deletePool(MemoryPool* pool)
	{
		fb_assert(pool);

		for (size_t i = 0; i < dbb_pools.getCount(); ++i)
		{
			if (dbb_pools[i] == pool)
			{
				dbb_pools.remove(i);
				break;
			}
		}
		MemoryPool::deletePool(pool);
	}

}
