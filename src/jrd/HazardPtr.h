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
#include "../common/gdsassert.h"
#include "fb_blk.h"

#include <atomic>

namespace Jrd {

	class thread_db;
	class Attachment;
	class HazardDelayedDelete;

	class HazardObject
	{
		friend HazardDelayedDelete;
	protected:
		virtual ~HazardObject();
	public:
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
	public:
		typedef const void* Ptr;

	private:
		static const unsigned int INITIAL_SIZE = 4;
		static const unsigned int DELETED_LIST_SIZE = 32;

		typedef Firebird::SortedArray<Ptr, Firebird::InlineStorage<Ptr, 128>> LocalHP;

		class HazardPointers : public HazardObject, public pool_alloc_rpt<Ptr>
		{
		private:
			HazardPointers(unsigned size)
				: hpCount(0), hpSize(size)
			{ }

		public:
			unsigned int hpCount;
			unsigned int hpSize;
			Ptr hp[1];

			static HazardPointers* create(MemoryPool& p, unsigned size);
		};

	public:
		enum class GarbageCollectMethod {GC_NORMAL, GC_ALL, GC_FORCE};

		HazardDelayedDelete(MemoryPool& dbbPool, MemoryPool& attPool)
			: Firebird::PermanentStorage(dbbPool),
			  toDelete(attPool),
			  hazardPointers(HazardPointers::create(getPool(), INITIAL_SIZE))
		{ }

		void add(Ptr ptr);
		void remove(Ptr ptr);

		void delayedDelete(HazardObject* mem, bool gc = true);
		void garbageCollect(GarbageCollectMethod gcMethod);

		// required in order to correctly pass that memory to DBB when destroying attachment
		HazardPointers* getHazardPointers();

	private:
		static void copyHazardPointers(LocalHP& local, Ptr* from, unsigned count);
		static void copyHazardPointers(thread_db* tdbb, LocalHP& local, Attachment* from);

		Firebird::HalfStaticArray<HazardObject*, DELETED_LIST_SIZE> toDelete;
		std::atomic<HazardPointers*> hazardPointers;
	};

	class HazardBase
	{
	protected:
		explicit HazardBase(thread_db* tdbb)
			: hazardDelayed(getHazardDelayed(tdbb))
		{ }

		explicit HazardBase(Attachment* att)
			: hazardDelayed(getHazardDelayed(att))
		{ }

		explicit HazardBase(HazardDelayedDelete* hd)
			: hazardDelayed(hd)
		{ }

		HazardBase()
			: hazardDelayed(nullptr)
		{ }

		void add(const void* hazardPointer)
		{
			if (!hazardDelayed)
				hazardDelayed = getHazardDelayed();
			hazardDelayed->add(hazardPointer);
		}

		void remove(const void* hazardPointer)
		{
			if (!hazardDelayed)
				hazardDelayed = getHazardDelayed();
			hazardDelayed->remove(hazardPointer);
		}

	private:
		HazardDelayedDelete* hazardDelayed;

	public:
		static HazardDelayedDelete* getHazardDelayed(thread_db* tdbb = nullptr);
		static HazardDelayedDelete* getHazardDelayed(Attachment* att);
	};

	template <typename T>
	class HazardPtr : public HazardBase
	{
	public:
		HazardPtr()
			: hazardPointer(nullptr)
		{ }

		explicit HazardPtr(thread_db* tdbb)
			: HazardBase(tdbb),
			  hazardPointer(nullptr)
		{ }

		explicit HazardPtr(Attachment* att)
			: HazardBase(att),
			  hazardPointer(nullptr)
		{ }

		explicit HazardPtr(HazardDelayedDelete* hd)
			: HazardBase(hd),
			  hazardPointer(nullptr)
		{ }

		HazardPtr(thread_db* tdbb, std::atomic<T*>& from)
			: HazardBase(tdbb),
			  hazardPointer(nullptr)
		{
			set(from);
		}

		HazardPtr(HazardPtr& copy)
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

		template <class T2>
		HazardPtr(HazardPtr<T2>& copy)
			: HazardBase(copy),
			  hazardPointer(nullptr)
		{
			reset(copy.unsafePointer());
		}

		template <class T2>
		HazardPtr(HazardPtr<T2>&& move)
			: HazardBase(move),
			  hazardPointer(nullptr)
		{
			hazardPointer = move.unsafePointer();
		}

		~HazardPtr()
		{
			reset(nullptr);
		}

		T* unsafePointer() const
		{
			return get();
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

		// atomically replaces 'where' with 'newVal', using *this as old value for comparison
		// always sets *this to actual data from 'where'
		bool replace(std::atomic<T*>* where, T* newVal)
		{
			T* val = get();
			bool rc = where->compare_exchange_strong(val, newVal,
				std::memory_order_release, std::memory_order_acquire);
			reset(rc ? newVal : val);
			return rc;
		}

		void clear()
		{
			reset(nullptr);
		}

		T* operator->()
		{
			return hazardPointer;
		}

		const T* operator->() const
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
			reset(copyAssign.hazardPointer, &copyAssign);
			return *this;
		}

		HazardPtr& operator=(HazardPtr&& moveAssign)
		{
			if (hazardPointer)
				 remove(hazardPointer);
			HazardBase::operator=(moveAssign);
			hazardPointer = moveAssign.hazardPointer;
			return *this;
		}

		template <class T2>
		HazardPtr& operator=(const HazardPtr<T2>& copyAssign)
		{
			reset(copyAssign.unsafePointer(), &copyAssign);
			return *this;
		}

		template <class T2>
		HazardPtr& operator=(HazardPtr<T2>&& moveAssign)
		{
			if (hazardPointer)
				 remove(hazardPointer);
			HazardBase::operator=(moveAssign);
			hazardPointer = moveAssign.unsafePointer();
			return *this;
		}

	private:
		void reset(T* newPtr, const HazardBase* newBase = nullptr)
		{
			if (newPtr != hazardPointer)
			{
				if (hazardPointer)
					remove(hazardPointer);
				if (newBase)
					HazardBase::operator=(*newBase);
				if (newPtr)
					add(newPtr);
				hazardPointer = newPtr;
			}
		}

		T* get() const
		{
			return hazardPointer;
		}

		T* hazardPointer;
	};

	template <typename T>
	bool operator==(const T* v1, const HazardPtr<T> v2)
	{
		return v2 == v1;
	}

	template <typename T, typename T2>
	bool operator==(const T* v1, const HazardPtr<T2> v2)
	{
		return v1 == v2.unsafePointer();
	}


	template <class Object>
	class HazardArray : public Firebird::PermanentStorage
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
				oldVal->release(tdbb);		// delayedDelete

			return HazardPtr<Object>(tdbb, *sub);
		}

		bool replace(thread_db* tdbb, FB_SIZE_T id, HazardPtr<Object>& oldVal, Object* const newVal)
		{
			if (id >= getCount())
				grow(id + 1);

			SubArrayElement* sub = m_objects[id >> SUBARRAY_SHIFT].load(std::memory_order_acquire);
			fb_assert(sub);
			sub = &sub[id & SUBARRAY_MASK];

			return oldVal.replace(sub, newVal);
		}

		void store(thread_db* tdbb, FB_SIZE_T id, const HazardPtr<Object>& val)
		{
			store(tdbb, id, val.unsafePointer());
		}

		bool load(FB_SIZE_T id, HazardPtr<Object>& val) const
		{
			if (id < getCount())
			{
				SubArrayElement* sub = m_objects[id >> SUBARRAY_SHIFT].load(std::memory_order_acquire);
				if (sub)
				{
					val.set(sub[id & SUBARRAY_MASK]);
					if (val && val->hasData())
						return true;
				}
			}

			return false;
		}

		class iterator
		{
		public:
			HazardPtr<Object> operator*()
			{
				return get();
			}

			HazardPtr<Object> operator->()
			{
				return get();
			}

			iterator& operator++()
			{
				++index;
				return *this;
			}

			iterator& operator--()
			{
				--index;
				return *this;
			}

			bool operator==(const iterator& itr) const
			{
				fb_assert(array == itr.array);
				return index == itr.index;
			}

			bool operator!=(const iterator& itr) const
			{
				fb_assert(array == itr.array);
				return index != itr.index;
			}

		private:
			void* operator new(size_t);
			void* operator new[](size_t);

		public:
			enum class Location {Begin, End};
			iterator(const HazardArray* a, Location loc = Location::Begin)
				: array(a),
				  hd(HazardPtr<Object>::getHazardDelayed()),
				  index(loc == Location::Begin ? 0 : array->getCount())
			{ }

			HazardPtr<Object> get()
			{
				HazardPtr<Object> rc(hd);
				array->load(index, rc);
				return rc;
			}

		private:
			const HazardArray* array;
			HazardDelayedDelete* hd;
			FB_SIZE_T index;
		};

		iterator begin() const
		{
			return iterator(this);
		}

		iterator end() const
		{
			return iterator(this, iterator::Location::End);
		}

	private:
		Firebird::HalfStaticArray<ArrayElement, 4> m_objects;			//!!!!!!!
		Firebird::Mutex objectsGrowMutex;
	};

} // namespace Jrd

#endif // JRD_HAZARDPTR_H
