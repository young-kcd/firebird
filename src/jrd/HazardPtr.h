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
		int delayedDelete(thread_db* tdbb);
	};

	class HazardDelayedDelete;
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

		inline void add(const void* hazardPointer);
		inline void remove(const void* hazardPointer);

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

		template <class DDS>
		explicit HazardPtr(DDS* par)
			: HazardBase(par),
			  hazardPointer(nullptr)
		{ }

		template <class DDS>
		HazardPtr(DDS* par, const std::atomic<T*>& from)
			: HazardBase(par),
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
			reset(copy.getPointer());
		}

		template <class T2>
		HazardPtr(HazardPtr<T2>&& move)
			: HazardBase(move),
			  hazardPointer(nullptr)
		{
			hazardPointer = move.getPointer();
		}

		~HazardPtr()
		{
			reset(nullptr);
		}

		T* unsafePointer() const		// to be removed
		{
			return getPointer();
		}

		T* getPointer() const
		{
			return hazardPointer;
		}

		void set(const std::atomic<T*>& from)
		{
			T* v = from.load(std::memory_order_relaxed);
			do
			{
				reset(v);
				v = from.load(std::memory_order_acquire);
			} while (hazardPointer != v);
		}

		// atomically replaces 'where' with 'newVal', using *this as old value for comparison
		// always sets *this to actual data from 'where'
		bool replace(std::atomic<T*>* where, T* newVal)
		{
			T* val = hazardPointer;
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
/*
		template <typename R>
		R& operator->*(R T::*mem)
		{
			return (this->hazardPointer)->*mem;
		}
 */
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

		bool operator!=(const T* v) const
		{
			return hazardPointer != v;
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
			reset(copyAssign.getPointer(), &copyAssign);
			return *this;
		}

		template <class T2>
		HazardPtr& operator=(HazardPtr<T2>&& moveAssign)
		{
			if (hazardPointer)
				 remove(hazardPointer);
			HazardBase::operator=(moveAssign);
			hazardPointer = moveAssign.getPointer();
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
		return v1 == v2.getPointer();
	}

	template <typename T>
	bool operator!=(const T* v1, const HazardPtr<T> v2)
	{
		return v2 != v1;
	}


	// Shared read here means that any thread can read from vector using HP.
	// It can be modified only in single thread, and it's caller's responsibility that modifying thread is single.

	template <typename T, FB_SIZE_T CAP, bool GC_ENABLED = true>
	class SharedReadVector : public Firebird::PermanentStorage
	{
	public:
		class Generation : public HazardObject, public pool_alloc_rpt<T>
		{
		private:
			Generation(FB_SIZE_T size)
				: count(0), capacity(size)
			{ }

			FB_SIZE_T count, capacity;
			T data[1];

		public:
			static Generation* create(MemoryPool& p, FB_SIZE_T cap)
			{
				return FB_NEW_RPT(p, CAP) Generation(CAP);
			}

			FB_SIZE_T getCount() const
			{
				return count;
			}

			FB_SIZE_T getCapacity() const
			{
				return capacity;
			}

			const T& value(FB_SIZE_T i) const
			{
				fb_assert(i < count);
				return data[i];
			}

			T& value(FB_SIZE_T i)
			{
				fb_assert(i < count);
				return data[i];
			}

			bool hasSpace(FB_SIZE_T needs = 1) const
			{
				return count + needs <= capacity;
			}

			bool add(const Generation* from)
			{
				if (!hasSpace(from->count))
					return false;
				memcpy(&data[count], from->data, from->count * sizeof(T));
				count += from->count;
				return true;
			}

			T* add()
			{
				if (!hasSpace())
					return nullptr;
				return &data[count++];
			}

			void truncate(const T& notValue)
			{
				while (count && data[count - 1] == notValue)
					count--;
			}
		};

		SharedReadVector(MemoryPool& p)
			: Firebird::PermanentStorage(p),
			  v(Generation::create(getPool(), CAP))
		{ }

		~SharedReadVector()
		{
			delete writeAccessor();
		}

		Generation* writeAccessor()
		{
			return v.load(std::memory_order_acquire);
		}

		template <class DDS>
		HazardPtr<Generation> readAccessor(DDS* par) const
		{
			return HazardPtr<Generation>(par, v);
		}

		inline void grow(HazardDelayedDelete* dd, FB_SIZE_T newSize = 0);

	private:
		std::atomic<Generation*> v;
	};


	class HazardDelayedDelete : public Firebird::PermanentStorage
	{
	private:
		static const unsigned int INITIAL_SIZE = 4;
		static const unsigned int DELETED_LIST_SIZE = 32;

		typedef Firebird::SortedArray<const void*, Firebird::InlineStorage<const void*, 128>> LocalHP;

	public:
		enum class GarbageCollectMethod {GC_NORMAL, GC_ALL, GC_FORCE};
		// In this and only this case disable GC in delayedDelete()
		typedef SharedReadVector<const void*, INITIAL_SIZE, true> HazardPointersStorage;
		typedef HazardPointersStorage::Generation HazardPointers;

		HazardDelayedDelete(MemoryPool& dbbPool, MemoryPool& attPool)
			: Firebird::PermanentStorage(dbbPool),
			  toDelete(attPool),
			  hazardPointers(getPool())
		{ }

		void add(const void* ptr);
		void remove(const void* ptr);

		void delayedDelete(HazardObject* mem, bool gc = true);
		void garbageCollect(GarbageCollectMethod gcMethod);

		// required in order to correctly pass that memory to DBB when destroying attachment
		HazardPointers* getHazardPointers();

	private:
		static void copyHazardPointers(LocalHP& local, HazardPtr<HazardPointers>& from);
		static void copyHazardPointers(thread_db* tdbb, LocalHP& local, Attachment* from);

		Firebird::HalfStaticArray<HazardObject*, DELETED_LIST_SIZE> toDelete;
		HazardPointersStorage hazardPointers;
	};


	inline void HazardBase::add(const void* hazardPointer)
	{
		if (!hazardDelayed)
			hazardDelayed = getHazardDelayed();
		hazardDelayed->add(hazardPointer);
	}

	inline void HazardBase::remove(const void* hazardPointer)
	{
		if (!hazardDelayed)
			hazardDelayed = getHazardDelayed();
		hazardDelayed->remove(hazardPointer);
	}

	template <typename T, FB_SIZE_T CAP, bool GC_ENABLED>
	inline void SharedReadVector<T, CAP, GC_ENABLED>::grow(HazardDelayedDelete* dd, FB_SIZE_T newSize)
	{
		Generation* oldGeneration = writeAccessor();
		if (newSize && (oldGeneration->getCapacity() >= newSize))
			return;

		FB_SIZE_T doubleSize = oldGeneration->getCapacity() * 2;
		if (newSize < doubleSize)
			newSize = doubleSize;

		Generation* newGeneration = Generation::create(getPool(), newSize);
		newGeneration->add(oldGeneration);
		v.store(newGeneration, std::memory_order_release);

		// delay delete - someone else may access it
		dd->delayedDelete(oldGeneration, GC_ENABLED);
	}


	template <class Object, unsigned SUBARRAY_SHIFT = 8>
	class HazardArray : public Firebird::PermanentStorage
	{
	private:
		static const unsigned SUBARRAY_SIZE = 1 << SUBARRAY_SHIFT;
		static const unsigned SUBARRAY_MASK = SUBARRAY_SIZE - 1;

		typedef std::atomic<Object*> SubArrayElement;
		typedef std::atomic<SubArrayElement*> ArrayElement;

	public:
		explicit HazardArray(MemoryPool& pool)
			: Firebird::PermanentStorage(pool),
			  m_objects(getPool())
		{}

		SLONG lookup(thread_db* tdbb, const typename Object::Key& key, HazardPtr<Object>* object = nullptr) const
		{
			auto a = m_objects.readAccessor(tdbb);
			for (FB_SIZE_T i = 0; i < a->getCount(); ++i)
			{
				SubArrayElement* const sub = a->value(i).load(std::memory_order_relaxed);
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
			auto a = m_objects.writeAccessor();
			for (FB_SIZE_T i = 0; i < a->getCount(); ++i)
			{
				SubArrayElement* const sub = a->value(i).load(std::memory_order_relaxed);
				if (!sub)
					continue;

				for (SubArrayElement* end = &sub[SUBARRAY_SIZE]; sub < end--;)
					delete *end;		// no need using release here in HazardArray's dtor

				delete[] sub;
			}
		}

		template <class DDS>
		FB_SIZE_T getCount(DDS* par) const
		{
			return m_objects.readAccessor(par)->getCount() << SUBARRAY_SHIFT;
		}

		void grow(thread_db* tdbb, FB_SIZE_T reqSize)
		{
			fb_assert(reqSize > 0);
			reqSize = ((reqSize - 1) >> SUBARRAY_SHIFT) + 1;

			Firebird::MutexLockGuard g(objectsGrowMutex, FB_FUNCTION);

			m_objects.grow(HazardBase::getHazardDelayed(tdbb), reqSize);
			auto a = m_objects.writeAccessor();
			fb_assert(a->getCapacity() >= reqSize);
			while (a->getCount() < reqSize)
			{
				SubArrayElement* sub = FB_NEW_POOL(getPool()) SubArrayElement[SUBARRAY_SIZE];
				memset(sub, 0, sizeof(SubArrayElement) * SUBARRAY_SIZE);
				a->add()->store(sub, std::memory_order_release);
			}
		}

		HazardPtr<Object> store(thread_db* tdbb, FB_SIZE_T id, Object* const val)
		{
			if (id >= getCount(tdbb))
				grow(tdbb, id + 1);

			auto a = m_objects.readAccessor(tdbb);
			SubArrayElement* sub = a->value(id >> SUBARRAY_SHIFT).load(std::memory_order_relaxed);
			fb_assert(sub);
			sub = &sub[id & SUBARRAY_MASK];

			Object* oldVal = sub->load(std::memory_order_acquire);
			while (!sub->compare_exchange_weak(oldVal, val,
				std::memory_order_release, std::memory_order_acquire));	// empty body
			if (oldVal)
				oldVal->delayedDelete(tdbb);

			return HazardPtr<Object>(tdbb, *sub);
		}

		bool replace(thread_db* tdbb, FB_SIZE_T id, HazardPtr<Object>& oldVal, Object* const newVal)
		{
			if (id >= getCount(tdbb))
				grow(tdbb, id + 1);

			auto a = m_objects.readAccessor(tdbb);
			SubArrayElement* sub = a->value(id >> SUBARRAY_SHIFT).load(std::memory_order_acquire);
			fb_assert(sub);
			sub = &sub[id & SUBARRAY_MASK];

			return oldVal.replace(sub, newVal);
		}

		void store(thread_db* tdbb, FB_SIZE_T id, const HazardPtr<Object>& val)
		{
			store(tdbb, id, val.getPointer());
		}

		template <class DDS>
		bool load(DDS* par, FB_SIZE_T id, HazardPtr<Object>& val) const
		{
			if (id < getCount(par))
			{
				auto a = m_objects.readAccessor(par);
				SubArrayElement* sub = a->value(id >> SUBARRAY_SHIFT).load(std::memory_order_acquire);
				if (sub)
				{
					val.set(sub[id & SUBARRAY_MASK]);
					if (val && val->hasData())
						return true;
				}
			}

			return false;
		}

		template <class DDS>
		HazardPtr<Object> load(DDS* par, FB_SIZE_T id) const
		{
			HazardPtr<Object> val;
			load(par, id, val);
			return val;
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
				  index(loc == Location::Begin ? 0 : array->getCount(hd))
			{ }

			HazardPtr<Object> get()
			{
				HazardPtr<Object> rc(hd);
				array->load(hd, index, rc);
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
		SharedReadVector<ArrayElement, 4> m_objects;
		Firebird::Mutex objectsGrowMutex;
	};

	class CacheObject : public HazardObject
	{
	public:
		virtual bool checkObject(thread_db* tdbb, Firebird::Arg::StatusVector&);
		virtual void afterUnlock(thread_db* tdbb);
	};

} // namespace Jrd

#endif // JRD_HAZARDPTR_H
