/*
 *	PROGRAM:	Common class definition
 *	MODULE:		object_array.h
 *	DESCRIPTION:	half-static array of any objects,
 *			having MemoryPool'ed constructor.
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
 *  Copyright (c) 2004 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef CLASSES_XOBJECTS_ARRAY_H
#define CLASSES_XOBJECTS_ARRAY_H

#include "../common/classes/alloc.h"
#include "../common/classes/x_array.h"

namespace Firebird
{
	template <typename T, typename A = XArray<T*, InlineStorage<T*, 8> > >
	class XObjectsArray : protected A
	{
	private:
		typedef A inherited;
	public:
		class const_iterator; // fwd decl.
		typedef FB_SIZE_T size_type;

		class iterator
		{
			friend class XObjectsArray<T, A>;
			friend class const_iterator;
		private:
			XObjectsArray *lst;
			size_type pos;
			iterator(XObjectsArray *l, size_type p) : lst(l), pos(p) { }
		public:
			iterator() : lst(0), pos(0) { }
			iterator(const iterator& it) : lst(it.lst), pos(it.pos) { }

			iterator& operator++()
			{
				++pos;
				return (*this);
			}
			iterator operator++(int)
			{
				iterator tmp = *this;
				++pos;
				 return tmp;
			}
			iterator& operator--()
			{
				fb_assert(pos > 0);
				--pos;
				return (*this);
			}
			iterator operator--(int)
			{
				fb_assert(pos > 0);
				iterator tmp = *this;
				--pos;
				 return tmp;
			}
			T* operator->()
			{
				fb_assert(lst);
				T* pointer = lst->getPointer(pos);
				return pointer;
			}
			T& operator*()
			{
				fb_assert(lst);
				T* pointer = lst->getPointer(pos);
				return *pointer;
			}
			bool operator!=(const iterator& v) const
			{
				fb_assert(lst == v.lst);
				return lst ? pos != v.pos : true;
			}
			bool operator==(const iterator& v) const
			{
				fb_assert(lst == v.lst);
				return lst ? pos == v.pos : false;
			}
		};

		class const_iterator
		{
			friend class XObjectsArray<T, A>;
		private:
			const XObjectsArray *lst;
			size_type pos;
			const_iterator(const XObjectsArray *l, size_type p) : lst(l), pos(p) { }
		public:
			const_iterator() : lst(0), pos(0) { }
			const_iterator(const iterator& it) : lst(it.lst), pos(it.pos) { }
			const_iterator(const const_iterator& it) : lst(it.lst), pos(it.pos) { }

			const_iterator& operator++()
			{
				++pos;
				return (*this);
			}
			const_iterator operator++(int)
			{
				const_iterator tmp = *this;
				++pos;
				 return tmp;
			}
			const_iterator& operator--()
			{
				fb_assert(pos > 0);
				--pos;
				return (*this);
			}
			const_iterator operator--(int)
			{
				fb_assert(pos > 0);
				const_iterator tmp = *this;
				--pos;
				 return tmp;
			}
			const T* operator->()
			{
				fb_assert(lst);
				const T* pointer = lst->getPointer(pos);
				return pointer;
			}
			const T& operator*()
			{
				fb_assert(lst);
				const T* pointer = lst->getPointer(pos);
				return *pointer;
			}
			bool operator!=(const const_iterator& v) const
			{
				fb_assert(lst == v.lst);
				return lst ? pos != v.pos : true;
			}
			bool operator==(const const_iterator& v) const
			{
				fb_assert(lst == v.lst);
				return lst ? pos == v.pos : false;
			}
			// Against iterator
			bool operator!=(const iterator& v) const
			{
				fb_assert(lst == v.lst);
				return lst ? pos != v.pos : true;
			}
			bool operator==(const iterator& v) const
			{
				fb_assert(lst == v.lst);
				return lst ? pos == v.pos : false;
			}

		};

	public:
		MemoryPool& getPool() const
		{
			return inherited::getPool();
		}

		void insert(size_type index, const T& item)
		{
			T* dataL = FB_NEW_POOL(this->getPool()) T(this->getPool(), item);
			inherited::insert(index, dataL);
		}

		T& insert(size_type index)
		{
			T* dataL = FB_NEW_POOL(this->getPool()) T(this->getPool());
			inherited::insert(index, dataL);
			return *dataL;
		}

		size_type add(const T& item)
		{
			T* dataL = FB_NEW_POOL(this->getPool()) T(this->getPool(), item);
			return inherited::add(dataL);
		}

		T& add()
		{
			T* dataL = FB_NEW_POOL(this->getPool()) T(this->getPool());
			inherited::add(dataL);
			return *dataL;
		}

		void push(const T& item)
		{
			add(item);
		}

		T pop()
		{
			T* pntr = inherited::pop();
			T rc = *pntr;
			delete pntr;
			return rc;
		}

		void remove(size_type index)
		{
			fb_assert(index < getCount());
			delete getPointer(index);
			inherited::remove(index);
		}

		void remove(iterator itr)
		{
  			fb_assert(itr.lst == this);
			remove(itr.pos);
		}

		void shrink(size_type newCount)
		{
			for (size_type i = newCount; i < getCount(); i++) {
				delete getPointer(i);
			}
			inherited::shrink(newCount);
		}

		void grow(size_type newCount)
		{
			size_type oldCount = getCount();
			inherited::grow(newCount);
			for (size_type i = oldCount; i < newCount; i++) {
				inherited::getElement(i) = FB_NEW_POOL(this->getPool()) T(this->getPool());
			}
		}

		void resize(const size_type newCount, const T& val)
		{
			if (newCount > getCount())
			{
				size_type oldCount = getCount();
				inherited::grow(newCount);
				for (size_type i = oldCount; i < newCount; i++) {
					inherited::getElement(i) = FB_NEW_POOL(this->getPool()) T(this->getPool(), val);
				}
			}
			else {
				shrink(newCount);
			}
		}

		void resize(const size_type newCount)
		{
			if (newCount > getCount())
			{
				grow(newCount);
			}
			else {
				shrink(newCount);
			}
		}

		iterator begin()
		{
			return iterator(this, 0);
		}

		iterator end()
		{
			return iterator(this, getCount());
		}

		iterator back()
		{
  			fb_assert(getCount() > 0);
			return iterator(this, getCount() - 1);
		}

		const_iterator begin() const
		{
			return const_iterator(this, 0);
		}

		const_iterator end() const
		{
			return const_iterator(this, getCount());
		}

		const T& operator[](size_type index) const
		{
  			return *getPointer(index);
		}

		const T* getPointer(size_type index) const
		{
  			return inherited::getElement(index);
		}

		T& operator[](size_type index)
		{
  			return *getPointer(index);
		}

		T* getPointer(size_type index)
		{
  			return inherited::getElement(index);
		}

		explicit XObjectsArray(MemoryPool& p, const XObjectsArray<T, A>& o)
			: A(p)
		{
			add(o);
		}

		explicit XObjectsArray(MemoryPool& p)
			: A(p)
		{
		}

		XObjectsArray() :
			A()
		{
		}

		~XObjectsArray()
		{
			for (size_type i = 0; i < getCount(); i++)
				delete getPointer(i);
		}

		size_type getCount() const throw()
		{
			return inherited::getCount();
		}

		size_type getCapacity() const
		{
			return inherited::getCapacity();
		}

		bool hasData() const
		{
			return getCount() != 0;
		}

		bool isEmpty() const
		{
			return getCount() == 0;
		}

		void clear()
		{
			for (size_type i = 0; i < getCount(); i++)
				delete getPointer(i);

			inherited::clear();
		}

		XObjectsArray<T, A>& operator =(const XObjectsArray<T, A>& o)
		{
			while (this->count > o.count)
				delete inherited::pop();

			add(o);

			return *this;
		}

		bool find(const T& item, FB_SIZE_T& pos) const
		{
			for (size_type i = 0; i < this->count; i++)
			{
				if (*getPointer(i) == item)
				{
					pos = i;
					return true;
				}
			}
			return false;
		}

	private:
		void add(const XObjectsArray<T, A>& o)
		{
			for (size_type i = 0; i < o.count; i++)
			{
				if (i < this->count)
					(*this)[i] = o[i];
				else
					add(o[i]);
			}
		}
	};

} // namespace Firebird

#endif	// CLASSES_XOBJECTS_ARRAY_H
