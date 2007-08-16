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

#ifndef CLASSES_OBJECTS_ARRAY_H
#define CLASSES_OBJECTS_ARRAY_H
 
#include "../common/classes/alloc.h"
#include "../common/classes/array.h"

namespace Firebird
{
	template <typename T, typename A = Array<T*, InlineStorage<T*, 8> > >
	class ObjectsArray : protected A
	{
	private:
		typedef A inherited;
	public:
		class const_iterator; // fwd decl.
		
		class iterator
		{
			friend class ObjectsArray<T, A>;
			friend class const_iterator;
		private:
			ObjectsArray *lst;
			size_t pos;
			iterator(ObjectsArray *l, size_t p) : lst(l), pos(p) { }
		public:
			iterator() : lst(0), pos(0) { }
/*
			iterator& operator=(ObjectsArray& a)
			{
				lst = &a;
				pos = 0;
			}
 */
			iterator& operator++() {
				++pos;
				return (*this);
			}
			iterator operator++(int) {
				iterator tmp = *this;
				++pos;
				 return tmp;
			}
			iterator& operator--() {
				fb_assert(pos > 0);
				--pos;
				return (*this);
			}
			iterator operator--(int) {
				fb_assert(pos > 0);
				iterator tmp = *this;
				--pos;
				 return tmp;
			}
			T* operator->() {
				fb_assert(lst);
				T* pointer = lst->getPointer(pos);
				return pointer;
			}
			T& operator*() {
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
			friend class ObjectsArray<T, A>;
		private:
			const ObjectsArray *lst;
			size_t pos;
			const_iterator(const ObjectsArray *l, size_t p) : lst(l), pos(p) { }
		public:
			const_iterator() : lst(0), pos(0) { }
			explicit const_iterator(const iterator& it) : lst(it.lst), pos(it.pos) {}
/*
			const_iterator& operator=(const ObjectsArray& a)
			{
				lst = &a;
				pos = 0;
			}
 */
			const_iterator& operator++() {
				++pos;
				return (*this);
			}
			const_iterator operator++(int) {
				const_iterator tmp = *this;
				++pos;
				 return tmp;
			}
			const_iterator& operator--() {
				fb_assert(pos > 0);
				--pos;
				return (*this);
			}
			const_iterator operator--(int) {
				fb_assert(pos > 0);
				const_iterator tmp = *this;
				--pos;
				 return tmp;
			}
			const T* operator->() {
				fb_assert(lst);
				const T* pointer = lst->getPointer(pos);
				return pointer;
			}
			const T& operator*() {
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
		void insert(size_t index, const T& item) {
			T* dataL = FB_NEW(this->getPool()) T(this->getPool(), item);
			inherited::insert(index, dataL);
		}
		size_t add(const T& item) {
			T* dataL = FB_NEW(this->getPool()) T(this->getPool(), item);
			return inherited::add(dataL);
		}
		T& add() {
			T* dataL = FB_NEW(this->getPool()) T(this->getPool());
			inherited::add(dataL);
			return *dataL;
		}
		void push(const T& item) {
			add(item);
		}
		T pop() {
			T* pntr = inherited::pop();
			T rc = *pntr;
			delete pntr;
			return rc;
		}
		void remove(size_t index) {
			fb_assert(index < getCount());
			delete getPointer(index);
			inherited::remove(index);
		}
		void remove(iterator itr) {
  			fb_assert(itr.lst == this);
			remove(itr.pos);
		}
		void shrink(size_t newCount) {
			for (size_t i = newCount; i < getCount(); i++) {
				delete getPointer(i);
			}
			inherited::shrink(newCount);
		}
		iterator begin() {
			return iterator(this, 0);
		}
		iterator end() {
			return iterator(this, getCount());
		}
		iterator back() {
  			fb_assert(getCount() > 0);
			return iterator(this, getCount() - 1);
		}
		const_iterator begin() const {
			return const_iterator(this, 0);
		}
		const_iterator end() const {
			return const_iterator(this, getCount());
		}
		const T& operator[](size_t index) const {
  			return *getPointer(index);
		}
		const T* getPointer(size_t index) const {
  			return inherited::getElement(index);
		}
		T& operator[](size_t index) {
  			return *getPointer(index);
		}
		T* getPointer(size_t index) {
  			return inherited::getElement(index);
		}
		explicit ObjectsArray(MemoryPool& p) : A(p) { }
		ObjectsArray() : A() { }
		~ObjectsArray() {
			for (size_t i = 0; i < getCount(); i++) {
				delete getPointer(i);
			}
		}
		size_t getCount() const {return inherited::getCount();}
		size_t getCapacity() const {return inherited::getCapacity();}
		void clear() { 
			for (size_t i = 0; i < getCount(); i++) {
				delete getPointer(i);
			}
			inherited::clear(); 
		}
	};

	// Template to convert object value to index directly
	template <typename T>
	class ObjectKeyValue {
	public:
		static const T& generate(const void* sender, const T* Item) { return Item; }
	};

	// Template for default value comparsion
	template <typename T>
	class ObjectComparator {
	public:
		static bool greaterThan(const T i1, const T i2) {
			return *i1 > *i2;
		}
	};

	// Dynamic sorted array of simple objects
	template <typename ObjectValue,
		typename ObjectStorage = InlineStorage<ObjectValue*, 32>, 
		typename ObjectKey = ObjectValue, 
		typename ObjectKeyOfValue = DefaultKeyValue<ObjectValue*>, 
		typename ObjectCmp = ObjectComparator<const ObjectKey*> >
	class SortedObjectsArray : public ObjectsArray<ObjectValue, 
			SortedArray <ObjectValue*, ObjectStorage, const ObjectKey*, 
			ObjectKeyOfValue, ObjectCmp> >
	{
	private:
		typedef ObjectsArray <ObjectValue, SortedArray<ObjectValue*, 
				ObjectStorage, const ObjectKey*, ObjectKeyOfValue, 
				ObjectCmp> > inherited;
	public:
		explicit SortedObjectsArray(MemoryPool& p) : 
			ObjectsArray <ObjectValue, SortedArray<ObjectValue*, 
				ObjectStorage, const ObjectKey*, ObjectKeyOfValue, 
				ObjectCmp> >(p) { }
		bool find(const ObjectKey& item, size_t& pos) const {
			const ObjectKey* const pItem = &item;
			return static_cast<const SortedArray<ObjectValue*, 
				ObjectStorage, const ObjectKey*, ObjectKeyOfValue, 
				ObjectCmp>*>(this)->find(pItem, pos);
		}
	};

} // namespace Firebird

#endif	// CLASSES_OBJECTS_ARRAY_H
