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

#include <atomic>

namespace Jrd {

	class thread_db;
	class Attachment;

	class HazardObject
	{
	public:
		virtual ~HazardObject();
		virtual int release(thread_db* tdbb);
	};

	class RefHazardObject : public HazardObject
	{
	public:
		RefHazardObject()
			: counter(1)		// non-std reference counted implementation
		{ }

		~RefHazardObject() override;
		int release(thread_db* tdbb) override;
		virtual void addRef(thread_db* tdbb);

	private:
		std::atomic<int> counter;
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
	class HazardPtr : private HazardBase
	{
	private:
		HazardPtr();

	public:
		explicit HazardPtr(thread_db* tdbb)
			: HazardBase(tdbb),
			  hazardPointer(nullptr)
		{ }

		HazardPtr(thread_db* tdbb, std::atomic<T*>& from)
			: HazardBase(tdbb),
			  hazardPointer(nullptr)
		{
			set(from);
		}

		HazardPtr(const HazardPtr& copy)
			: HazardBase(copy),
			  hazardPointer(nullptr)
		{
			reset(copy.hazardPointer);
		}

		HazardPtr(HazardPtr&& move)
			: HazardBase(move),
			  hazardPointer(nullptr)
		{
			hazardPointer = move.hazardPointer;
		}

		~HazardPtr()
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

		void clear()
		{
			reset(nullptr);
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

		bool operator==(const T* v) const
		{
			return hazardPointer == v;
		}

		operator bool() const
		{
			return hazardPointer != nullptr;
		}

		HazardPtr& operator=(const HazardPtr& copyAssign)
		{
			reset(copyAssign.hazardPointer);
			return *this;
		}

		HazardPtr& operator=(HazardPtr&& moveAssign)
		{
			if (hazardPointer)
				 remove(hazardPointer);
			hazardPointer = moveAssign.hazardPointer;
			return *this;
		}

		template <class T2>
		HazardPtr& operator=(const HazardPtr<T2>& copyAssign)
		{
			reset(copyAssign.get());
			return *this;
		}

		template <class T2>
		HazardPtr& operator=(HazardPtr<T2>&& moveAssign)
		{
			if (hazardPointer)
				 remove(hazardPointer);
			hazardPointer = moveAssign.get();
			return *this;
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

	template <class Object>
	class HazardArray : public Firebird::PermanentStorage//, private ObjectBase
	{
	public:
		typedef typename Object::Key Key;

	private:
		static const unsigned SUBARRAY_SHIFT = 8;
		static const unsigned SUBARRAY_SIZE = 1 << SUBARRAY_SHIFT;
		static const unsigned SUBARRAY_MASK = SUBARRAY_SIZE - 1;

		typedef std::atomic<Object*> SubArrayElement;
		typedef std::atomic<SubArrayElement*> ArrayElement;

	public:
		explicit HazardArray(MemoryPool& pool)
			: Firebird::PermanentStorage(pool)
		{}

		SLONG lookup(thread_db* tdbb, const Key& key, HazardPtr<Object>* object = nullptr) const
		{
			for (FB_SIZE_T i = 0; i < m_objects.getCount(); ++i)
			{
				SubArrayElement* const sub = m_objects[i].load(std::memory_order_acquire);
				if (!sub)
					continue;

				for (SubArrayElement* end = &sub[SUBARRAY_SIZE]; sub < end--;)
				{
					HazardPtr<Object> val(tdbb, *end);
					if (val.hasData() && val->getKey() == key)
					{
						if (object)
							*object = val;
						return (SLONG)((i << SUBARRAY_SHIFT) + (end - sub));
					}
				}
			}

			return -1;
		}

		~HazardArray()
		{
			for (FB_SIZE_T i = 0; i < m_objects.getCount(); ++i)
			{
				SubArrayElement* const sub = m_objects[i].load(std::memory_order_relaxed);
				if (!sub)
					continue;

				for (SubArrayElement* end = &sub[SUBARRAY_SIZE]; sub < end--;)
					delete *end;		// no need using release here in HazardArray's dtor

				delete[] sub;
			}
		}

		FB_SIZE_T getCount() const
		{
			return m_objects.getCount() << SUBARRAY_SHIFT;
		}

		void grow(const FB_SIZE_T reqSize)
		{
			Firebird::MutexLockGuard g(objectsGrowMutex, FB_FUNCTION);
			m_objects.grow(reqSize >> SUBARRAY_SHIFT);
		}

		HazardPtr<Object> store(thread_db* tdbb, FB_SIZE_T id, Object* const val)
		{
			fb_assert(id >= 0);

			if (id >= getCount())
				grow(id + 1);

			SubArrayElement* sub = m_objects[id >> SUBARRAY_SHIFT].load(std::memory_order_acquire);
			if (!sub)
			{
				SubArrayElement* newSub = FB_NEW_POOL(getPool()) SubArrayElement[SUBARRAY_SIZE];
				memset(newSub, 0, sizeof(SubArrayElement) * SUBARRAY_SIZE);
				if (!m_objects[id >> SUBARRAY_SHIFT].compare_exchange_strong(sub, newSub,
					std::memory_order_release, std::memory_order_acquire))
				{
					// someone else already installed this subarray
					// ok for us - just free unneeded memory
					delete[] newSub;
				}
				else
					sub = newSub;
			}

			sub = &sub[id & SUBARRAY_MASK];
			Object* oldVal = sub->load(std::memory_order_acquire);
			while (!sub->compare_exchange_weak(oldVal, val,
                    std::memory_order_release, std::memory_order_acquire));	// empty body

			if (oldVal)
				oldVal->release(tdbb);

			return HazardPtr<Object>(tdbb, *sub);
		}

		bool load(SLONG id, HazardPtr<Object>& val) const
		{
			if (id < (int) getCount())
			{
				SubArrayElement* sub = m_objects[id >> SUBARRAY_SHIFT].load(std::memory_order_acquire);
				if (sub)
				{
					val.set(sub[id & SUBARRAY_MASK]);
					if (val->hasData())
						return true;
				}
			}

			return false;
		}

	private:
		Firebird::HalfStaticArray<ArrayElement, 4> m_objects;
		Firebird::Mutex objectsGrowMutex;
	};

} // namespace Jrd

#endif // JRD_HAZARDPTR_H
