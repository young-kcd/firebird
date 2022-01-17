/*
 *	PROGRAM:	Engine Code
 *	MODULE:		HazardPtr.cpp
 *	DESCRIPTION:	Use of hazard pointers in metadata cache
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
 *  The Original Code was created by Alexander Peshkov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2021 Alexander Peshkov <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#include "firebird.h"

#include "../jrd/HazardPtr.h"
#include "../jrd/jrd.h"

using namespace Jrd;
using namespace Firebird;

HazardObject::~HazardObject()
{ }

int HazardObject::release(thread_db* tdbb)
{
	HazardDelayedDelete& dd = tdbb->getAttachment()->att_delayed_delete;
	dd.delayedDelete(this);
	return 0;
}

RefHazardObject::~RefHazardObject()
{
	fb_assert(counter == 0);
}

int RefHazardObject::release(thread_db* tdbb)
{
	fb_assert(counter > 0);
	if (--counter == 0)
	{
		HazardObject::release(tdbb);
		return 0;
	}

	return 1;
}

void RefHazardObject::addRef(thread_db*)
{
	fb_assert(counter >= 0);
	if (counter < 1)
		fatal_exception::raise("Attempt to reuse released object failed");		// need special error handling? !!!!!!!!!!!
	++counter;
}

HazardBase::HazardBase(thread_db* tdbb)
	: hazardDelayed(tdbb->getAttachment()->att_delayed_delete)
{ }


HazardDelayedDelete::HazardPointers* HazardDelayedDelete::HazardPointers::create(MemoryPool& p, unsigned size)
{
	return FB_NEW_RPT(p, size) HazardPointers(size);
}

void HazardDelayedDelete::add(void* ptr)
{
	// as long as we access our own hazard pointers single relaxed load is OK
	HazardPointers *hp = hazardPointers.load(std::memory_order_relaxed);

	// 1. Search for holes
	for (unsigned n = 0; n < hp->hpCount; ++n)
	{
		if (!hp->hp[n])
		{
			hp->hp[n] = ptr;
			return;
		}
	}

	// 2. Grow if needed
	if (hp->hpCount >= hp->hpSize)
	{
		HazardPointers* newHp = HazardPointers::create(getPool(), hp->hpSize * 2);
		memcpy(newHp->hp, hp->hp, hp->hpCount * sizeof(hp->hp[0]));
		newHp->hpCount = hp->hpCount;

		HazardPointers* oldHp = hp;
		hazardPointers.store((hp = newHp));
		delayedDelete(oldHp);	// delay delete for a case when someone else is accessing it now
	}

	// 3. Append
	hp->hp[hp->hpCount] = ptr;
	hp->hpCount++;
}

void HazardDelayedDelete::remove(void* ptr)
{
	// as long as we access our own hazard pointers single relaxed load is OK
	HazardPointers *hp = hazardPointers.load(std::memory_order_relaxed);

	for (unsigned n = 0; n < hp->hpCount; ++n)
	{
		if (hp->hp[n] == ptr)
		{
			hp->hp[n] = nullptr;

			while (hp->hpCount && !hp->hp[hp->hpCount - 1])
				hp->hpCount--;
			return;
		}
	}

	fb_assert(!"Required ptr not found in HazardDelayedDelete::remove");
}

void HazardDelayedDelete::delayedDelete(HazardObject* mem, bool gc)
{
	if (mem)
		toDelete.push(mem);

	if (gc)
		garbageCollect(GarbageCollectMethod::GC_NORMAL);
}

void HazardDelayedDelete::copyHazardPointers(LocalHP& local, void** from, unsigned count)
{
	for (unsigned n = 0; n < count; ++n)
	{
		if (from[n])
			local.push(from[n]);
	}
}

void HazardDelayedDelete::copyHazardPointers(thread_db* tdbb, LocalHP& local, Attachment* from)
{
	for (Attachment* attachment = from; attachment; attachment = attachment->att_next)
	{
		HazardPtr<HazardPointers> hp(tdbb, attachment->att_delayed_delete.hazardPointers);
		copyHazardPointers(local, hp->hp, hp->hpCount);
	}
}


void HazardDelayedDelete::garbageCollect(GarbageCollectMethod gcMethod)
{
	HazardPointers *myHp = hazardPointers.load(std::memory_order_relaxed);
	if (gcMethod == GarbageCollectMethod::GC_NORMAL && myHp->hpCount < DELETED_LIST_SIZE)
		return;

	thread_db* tdbb = JRD_get_thread_data();
	Database* database = tdbb->getDatabase();

	// collect hazard pointers from all atachments
	LocalHP localCopy;
	localCopy.setSortMode(FB_ARRAY_SORT_MANUAL);
	{
		Sync dbbSync(&database->dbb_sync, FB_FUNCTION);
		if (!database->dbb_sync.ourExclusiveLock())
			dbbSync.lock(SYNC_SHARED);

		copyHazardPointers(tdbb, localCopy, database->dbb_attachments);
		copyHazardPointers(tdbb, localCopy, database->dbb_sys_attachments);

		HazardPtr<HazardPointers> hp(tdbb, database->dbb_delayed_delete.hazardPointers);
		copyHazardPointers(localCopy, hp->hp, hp->hpCount);
	}
	localCopy.sort();

	// delete what can be deleted
	unsigned keep = 0;
	for (unsigned i = 0; i < toDelete.getCount(); ++i)
	{
		if (localCopy.exist(toDelete[i]))
			toDelete[keep++] = toDelete[i];
		else
			delete toDelete[i];

		if (i != keep)
			toDelete[i] = nullptr;
	}
	toDelete.shrink(keep);

	if (gcMethod != GarbageCollectMethod::GC_FORCE || keep == 0)
		return;

	// Pass remaining to Database
	MutexLockGuard g(database->dbb_dd_mutex, FB_FUNCTION);

	database->dbb_delayed_delete.garbageCollect(GarbageCollectMethod::GC_NORMAL);
	for (unsigned i = 0; i < toDelete.getCount(); ++i)
	{
		database->dbb_delayed_delete.add(toDelete[i]);
		toDelete[i] = nullptr;
	}
	toDelete.shrink(0);
}

HazardDelayedDelete::HazardPointers* HazardDelayedDelete::getHazardPointers()
{
	// as long as we access our own hazard pointers single relaxed load is OK
	return hazardPointers.load(std::memory_order_relaxed);
}
