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

#ifndef FB_LIST_H
#define FB_LIST_H
 
#include "../common/classes/alloc.h"
#include "../common/classes/array.h"

namespace Firebird
{
	template <typename T, int InlineCapacity = 8>
	class ObjectsArray : private HalfStaticArray<T*, InlineCapacity>
	{
	private:
		typedef HalfStaticArray<T*, InlineCapacity> inherited;
	public:
		class iterator {
			friend class ObjectsArray<T, InlineCapacity>;
		private:
			ObjectsArray *lst;
			int pos;
			iterator(ObjectsArray *l, int p) : pos(p), lst(l) {}
		public:
			iterator& operator++() {++pos; return (*this);}
			iterator operator++(int) {iterator tmp = *this; ++pos; 
									  return tmp;}
			iterator& operator--() {--pos; return (*this);}
			iterator operator--(int) {iterator tmp = *this; --pos; 
									  return tmp;}
			T* operator->() {
				T* pointer = lst->getPointer(pos);
				return pointer;
			}
			T& operator*() {
				T* pointer = lst->getPointer(pos);
				return *pointer;
			}
			operator!=(const iterator& v) const {return pos != v.pos;}
		};
	public:
		void insert(int index, const T& item) {
			T* data = FB_NEW(*pool) T(pool, item);
			inherited::insert(index, data);
		}
		int add(const T& item) {
			T* data = FB_NEW(*pool) T(pool, item);
			return inherited::add(data);
		};
		void push(const T& item) {
			add(item);
		}
		T pop() {
			T* pntr = inherited::pop();
			T rc = *pntr;
			delete pntr;
			return rc;
		}
		void remove(int index) {
			delete getPointer(index);
			inherited::remove(index);
		}
		void remove(iterator itr) {
  			fb_assert(itr.lst == this);
			remove(itr.pos);
		}
		void shrink(int newCount) {
			for (int i = newCount; i < getCount(); i++) {
				delete getPointer(i);
			}
			inherited::shrink(newCount);
		};
		iterator begin() {
			return iterator(this, 0);
		}
		iterator end() {
			return iterator(this, getCount());
		}
		const T& operator[](int index) const {
  			return *getPointer(index);
		}
		const T* getPointer(int index) const {
  			return inherited::getElement(index);
		}
		T& operator[](int index) {
  			return *getPointer(index);
		}
		T* getPointer(int index) {
  			return inherited::getElement(index);
		}
		inline ObjectsArray(MemoryPool* p) : 
			HalfStaticArray<T*, InlineCapacity>(p) {}
		~ObjectsArray() {
			for (int i=0; i<getCount(); i++) {
				delete getPointer(i);
			}
		}
		int getCount() const {return inherited::getCount();}
		int getCapacity() const {return inherited::getCapacity();}
	};
}

#endif	// FB_LIST_H
