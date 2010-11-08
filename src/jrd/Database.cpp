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
// Definition of block types for data allocation in JRD
#include "../include/fb_blk.h"

#include "../jrd/ibase.h"
#include "../jrd/common.h"
#include "../jrd/ods.h"
#include "../jrd/lck.h"
#include "../jrd/Database.h"
#include "../jrd/nbak.h"
#include "../jrd/tra.h"
#include "../jrd/lck_proto.h"
#include "../jrd/os/pio_proto.h"

// Thread data block
#include "../jrd/ThreadData.h"

// recursive mutexes
#include "../common/thd.h"

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

	Firebird::string Database::getUniqueFileId() const
	{
		const PageSpace* const pageSpace = dbb_page_manager.findPageSpace(DB_PAGE_SPACE);

		Firebird::UCharBuffer buffer;
		PIO_get_unique_file_id(pageSpace->file, buffer);

		Firebird::string file_id;
		char* s = file_id.getBuffer(2 * buffer.getCount());
		for (size_t i = 0; i < buffer.getCount(); i++)
		{
			sprintf(s, "%02x", (int) buffer[i]);
			s += 2;
		}

		return file_id;
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

		delete dbb_monitoring_data;
		delete dbb_backup_manager;

		dbb_flags |= DBB_destroying;

		Checkout dcoHolder(this);
		// This line decrements the usage counter and may cause the destructor to be called.
		// It should happen with the dbb_sync unlocked.
		LockManager::destroy(dbb_lock_mgr);
		EventManager::destroy(dbb_event_mgr);
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

	SLONG Database::genSharedUniqueNumber(thread_db* tdbb)
	{
		const int SHARED_COUNTER_CACHE_SIZE = 16;

		if (!dbb_sh_counter_lock)
		{
			Lock* lock = FB_NEW_RPT(*dbb_permanent, sizeof(SLONG)) Lock();
			dbb_sh_counter_lock = lock;
			lock->lck_type = LCK_shared_counter;
			lock->lck_owner_handle = LCK_get_owner_handle(tdbb, lock->lck_type);
			lock->lck_parent = dbb_lock;
			lock->lck_length = sizeof(SLONG);
			lock->lck_key.lck_long = 0;
			lock->lck_dbb = this;
			lock->lck_ast = blockingAstSharedCounter;
			lock->lck_object = this;
			LCK_lock(tdbb, lock, LCK_PW, LCK_WAIT);

			dbb_sh_counter_curr = 1;
			dbb_sh_counter_max = 0;
		}

		if (dbb_sh_counter_curr > dbb_sh_counter_max)
		{
			LCK_convert(tdbb, dbb_sh_counter_lock, LCK_PW, LCK_WAIT);

			dbb_sh_counter_curr = LCK_read_data(tdbb, dbb_sh_counter_lock);
			dbb_sh_counter_max = dbb_sh_counter_curr + SHARED_COUNTER_CACHE_SIZE - 1;
			LCK_write_data(tdbb, dbb_sh_counter_lock, dbb_sh_counter_max + 1);
		}

		return dbb_sh_counter_curr++;
	}

	int Database::blockingAstSharedCounter(void* ast_object)
	{
		Database* dbb = static_cast<Database*>(ast_object);

		try
		{
			Database::SyncGuard dsGuard(dbb, true);

			ThreadContextHolder tdbb;
			tdbb->setDatabase(dbb);
			// tdbb->setAttachment(dbb->dbb_sh_counter_lock->lck_attachment);

			Jrd::ContextPoolHolder context(tdbb, dbb->dbb_permanent);

			LCK_downgrade(tdbb, dbb->dbb_sh_counter_lock);
		}
		catch (const Firebird::Exception&)
		{} // no-op

		return 0;
	}
} // namespace
