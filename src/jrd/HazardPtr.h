/*
 *	PROGRAM:	Engine Code
 *	MODULE:		HazardPtr.h
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

#ifndef JRD_HAZARDPTR_H
#define JRD_HAZARDPTR_H

#include "../common/classes/alloc.h"
#include "../common/classes/array.h"
#include "fb_blk.h"

namespace Jrd {

	class thread_db;
	class Attachment;

	class HazardObject
	{
	public:
		virtual ~HazardObject();
		void delayedDelete(thread_db* tdbb);
	};


	class HazardDelayedDelete : public Firebird::PermanentStorage
	{
		static const unsigned int INITIAL_SIZE = 4;
		static const unsigned int DELETED_LIST_SIZE = 32;

		typedef Firebird::SortedArray<void*, Firebird::InlineStorage<void*, 128>> LocalHP;

		class HazardPointers : public HazardObject, public pool_alloc_rpt<void*>
		{
		private:
			HazardPointers(unsigned size)
				: hpCount(0), hpSize(size)
			{ }

		public:
			unsigned int hpCount;
			unsigned int hpSize;
			void* hp[1];

			static HazardPointers* create(MemoryPool& p, unsigned size);
		};

	public:
		enum class GarbageCollectMethod {GC_NORMAL, GC_ALL, GC_FORCE};

		HazardDelayedDelete(MemoryPool& dbbPool, MemoryPool& attPool)
			: Firebird::PermanentStorage(dbbPool),
			  toDelete(attPool),
			  hazardPointers(HazardPointers::create(getPool(), INITIAL_SIZE))
		{ }

		void add(void* ptr);
		void remove(void* ptr);

		void delayedDelete(HazardObject* mem, bool gc = true);
		void garbageCollect(GarbageCollectMethod gcMethod);

		// required in order to correctly pass that memory to DBB when destroying attachment
		HazardPointers* getHazardPointers();

	private:
		static void copyHazardPointers(LocalHP& local, void** from, unsigned count);
		static void copyHazardPointers(thread_db* tdbb, LocalHP& local, Attachment* from);

		Firebird::HalfStaticArray<HazardObject*, DELETED_LIST_SIZE> toDelete;
		std::atomic<HazardPointers*> hazardPointers;
	};

	class HazardBase
	{
	public:
		HazardBase(thread_db* tdbb);

		void add(void* hazardPointer)
		{
			hazardDelayed.add(hazardPointer);
		}

		void remove(void* hazardPointer)
		{
			hazardDelayed.remove(hazardPointer);
		}

	private:
		HazardDelayedDelete& hazardDelayed;
	};

	template <typename T>
	class Hazard : private HazardBase
	{
	public:
		Hazard(thread_db* tdbb)
			: HazardBase(tdbb),
			  hazardPointer(nullptr)
		{ }

		Hazard(thread_db* tdbb, std::atomic<T*>& from)
			: HazardBase(tdbb),
			  hazardPointer(nullptr)
		{
			set(from);
		}

		~Hazard()
		{
			reset(nullptr);
		}

		T* get()
		{
			return hazardPointer;
		}

		void set(std::atomic<T*>& from)
		{
			T* v = from.load(std::memory_order_relaxed);
			do
			{
				reset(v);
				v = from.load(std::memory_order_acquire);
			} while (get() != v);
		}

		T* operator->()
		{
			return hazardPointer;
		}

		bool operator!() const
		{
			return hazardPointer == nullptr;
		}

		bool hasData() const
		{
			return hazardPointer != nullptr;
		}

	private:
		void reset(T* newPtr)
		{
			if (newPtr != hazardPointer)
			{
				if (hazardPointer)
					remove(hazardPointer);
				if (newPtr)
					add(newPtr);
				hazardPointer = newPtr;
			}
		}

		T* hazardPointer;
	};

} // namespace Jrd

#endif // JRD_HAZARDPTR_H
