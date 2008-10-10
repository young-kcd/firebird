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
 * Sean Leyne
 * Claudio Valderrama C.
 */

#include "firebird.h"
#include "../jrd/ibase.h"
#include "../jrd/common.h"
#include "../jrd/ods.h"
#include "../jrd/Database.h"
#include "../jrd/tra.h"
#include "../jrd/os/pio_proto.h"

// Thread data block
#include "../jrd/ThreadData.h"

// recursive mutexes
#include "../common/thd.h"

// Definition of block types for data allocation in JRD
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

		dbb_flags |= DBB_destroying;
		Checkout dcoHolder(this);
		dbb_lock_mgr->release();
	}

	void Database::deletePool(MemoryPool* pool)
	{
		if (pool)
		{
			size_t pos;
			if (dbb_pools.find(pool, pos))
			{
				dbb_pools.remove(pos);
			}

			MemoryPool::deletePool(pool);
		}
	}

} // namespace
