/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		array.h
 *	DESCRIPTION:	dynamic array of simple elements
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
 * Created by: Alex Peshkov <peshkoff@mail.ru>
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */
 
#ifndef CLASSES_ARRAY_H
#define CLASSES_ARRAY_H

#include "../jrd/gdsassert.h"
#include <string.h>
#include "../common/classes/alloc.h"

namespace Firebird {

// Static part of the array
template <typename T, size_t Capacity>
class InlineStorage : public AutoStorage {
public:
	explicit InlineStorage(MemoryPool& p) : AutoStorage(p) { }
	InlineStorage() : AutoStorage() { }
protected:
	T* getStorage() {
		return buffer;
	}
	size_t getStorageSize() const {
		return Capacity;
	}
private:
	T buffer[Capacity];
};

// Used when array doesn't have static part
template <typename T>
class EmptyStorage : public AutoStorage {
public:
	explicit EmptyStorage(MemoryPool& p) : AutoStorage(p) { }
	EmptyStorage() : AutoStorage() { }
protected:
	T* getStorage() { return NULL; }
	size_t getStorageSize() const { return 0; }
};

// Dynamic array of simple types
template <typename T, typename Storage = EmptyStorage<T> >
class Array : protected Storage {
public:
	explicit Array(MemoryPool& p) : 
		Storage(p), count(0), capacity(this->getStorageSize()), data(this->getStorage()) { }
	Array(MemoryPool& p, size_t InitialCapacity) : 
		Storage(p), count(0), capacity(this->getStorageSize()), data(this->getStorage())
	{
		ensureCapacity(InitialCapacity);
	}
	Array() : count(0), 
		capacity(this->getStorageSize()), data(this->getStorage()) { }
	explicit Array(size_t InitialCapacity) : count(0), 
		capacity(this->getStorageSize()), data(this->getStorage())
	{
		ensureCapacity(InitialCapacity);
	}
	~Array()
	{
		freeData();
	}
	void clear() { count = 0; }
protected:
	const T& getElement(size_t index) const {
  		fb_assert(index < count);
  		return data[index];
	}
	T& getElement(size_t index) {
  		fb_assert(index < count);
  		return data[index];
	}
	void freeData()
	{
		if (data != this->getStorage())
			this->getPool().deallocate(data);
	}
public:
	Array<T, Storage>& operator =(const Array<T, Storage>& L) 
	{
		ensureCapacity(L.count);
		memcpy(data, L.data, sizeof(T) * L.count);
		count = L.count;
		return *this;
	}
	const T& operator[](size_t index) const {
  		return getElement(index);
	}
	T& operator[](size_t index) {
  		return getElement(index);
	}
	const T& front() const {
  		fb_assert(count > 0);
		return *data;
	}
	const T& back() const {
  		fb_assert(count > 0);
		return *(data + count - 1);
	}
	const T* begin() const { return data; }
	const T* end() const { return data + count; }
	T& front() {
  		fb_assert(count > 0);
		return *data;
	}
	T& back() {
  		fb_assert(count > 0);
		return *(data + count - 1);
	}
	T* begin() { return data; }
	T* end() { return data + count; }
	void insert(size_t index, const T& item) {
		fb_assert(index <= count);
		ensureCapacity(count + 1);
		memmove(data + index + 1, data + index, sizeof(T) * (count++ - index));
		data[index] = item;
	}
	void insert(size_t index, const Array<T, Storage>& L) {
		fb_assert(index <= count);
		ensureCapacity(count + L.count);
		memmove(data + index + L.count, data + index, sizeof(T) * (count - index));
		memcpy(data + index, L.data, L.count);
		count += L.count;
	}
	void insert(size_t index, const T* items, size_t itemsSize) {
		fb_assert(index <= count);
		ensureCapacity(count + itemsSize);
		memmove(data + index + itemsSize, data + index, sizeof(T) * (count - index));
		memcpy(data + index, items, sizeof(T) * itemsSize);
		count += itemsSize;
	}
	size_t add(const T& item) {
		ensureCapacity(count + 1);
		data[count++] = item;
  		return count;
	}
	void remove(size_t index) {
  		fb_assert(index < count);
  		memmove(data + index, data + index + 1, sizeof(T) * (--count - index));
	}
	void remove(size_t from, size_t to) {
  		fb_assert(from <= to);
  		fb_assert(to <= count);
  		memmove(data + from, data + to, sizeof(T) * (to - from));
		count -= (to - from);
	}
	void remove(T* itr) {
		const size_t index = itr - begin();
  		fb_assert(index < count);
  		memmove(data + index, data + index + 1, sizeof(T) * (--count - index));
	}
	void shrink(size_t newCount) {
		fb_assert(newCount <= count);
		count = newCount;
	}
	// Grow size of our array and zero-initialize new items
	void grow(size_t newCount) {
		fb_assert(newCount >= count);
		ensureCapacity(newCount);
		memset(data + count, 0, sizeof(T) * (newCount - count));
		count = newCount;
	}
	void join(const Array<T, Storage>& L) {
		ensureCapacity(count + L.count);
		memcpy(data + count, L.data, sizeof(T) * L.count);
		count += L.count;
	}
	size_t getCount() const { return count; }
	size_t getCapacity() const { return capacity; }
	void push(const T& item) {
		add(item);
	}
	void push(const T* items, size_t itemsSize) {
		ensureCapacity(count + itemsSize);
		memcpy(data + count, items, sizeof(T) * itemsSize);
		count += itemsSize;
	}
	T pop() {
		fb_assert(count > 0);
		count--;
		return data[count];
	}
	// prepare array to be used as a buffer of capacity items
	T* getBuffer(size_t capacityL) {
		ensureCapacity(capacityL);
		count = capacityL;
		return data;
	}
	// clear array and release dinamically allocated memory
	void free() 
	{
		clear();
		freeData();
		capacity = this->getStorageSize();
		data = this->getStorage();
	}

protected:
	size_t count, capacity;
	T* data;
	void ensureCapacity(size_t newcapacity) {
		if (newcapacity > capacity) {
			if (newcapacity < capacity * 2) {
				newcapacity = capacity * 2;
			}
			T* newdata = reinterpret_cast<T*>
				(this->getPool().allocate(sizeof(T) * newcapacity
#ifdef DEBUG_GDS_ALLOC
		, 1, __FILE__, __LINE__
#endif
						));
			memcpy(newdata, data, sizeof(T) * count);
			freeData();
			data = newdata;
			capacity = newcapacity;
		}
	}
};

// Dynamic sorted array of simple objects
template <typename Value,
	typename Storage = EmptyStorage<Value>, 
	typename Key = Value, 
	typename KeyOfValue = DefaultKeyValue<Value>, 
	typename Cmp = DefaultComparator<Key> >
class SortedArray : public Array<Value, Storage> {
public:
	SortedArray(MemoryPool& p, size_t s) : Array<Value, Storage>(p, s) {}
	explicit SortedArray(MemoryPool& p) : Array<Value, Storage>(p) {}
	explicit SortedArray(size_t s) : Array<Value, Storage>(s) {}
	SortedArray() : Array<Value, Storage>() {}
	bool find(const Key& item, size_t& pos) const {
		size_t highBound = this->count, lowBound = 0;
		while (highBound > lowBound) {
			const size_t temp = (highBound + lowBound) >> 1;
			if (Cmp::greaterThan(item, KeyOfValue::generate(this, this->data[temp])))
				lowBound = temp + 1;
			else
				highBound = temp;
		}
		pos = lowBound;
		return highBound != this->count &&
			!Cmp::greaterThan(KeyOfValue::generate(this, this->data[lowBound]), item);
	}
	size_t add(const Value& item) {
	    size_t pos;
  	    find(KeyOfValue::generate(this, item), pos);
		insert(pos, item);
		return pos;
	}
};

// Nice shorthand for arrays with static part
template <typename T, size_t InlineCapacity>
class HalfStaticArray : public Array<T, InlineStorage<T, InlineCapacity> > {
public:
	explicit HalfStaticArray(MemoryPool& p) : Array<T, InlineStorage<T, InlineCapacity> > (p) {}
	HalfStaticArray(MemoryPool& p, size_t InitialCapacity) : 
		Array<T, InlineStorage<T, InlineCapacity> > (p, InitialCapacity) {}
	HalfStaticArray() : Array<T, InlineStorage<T, InlineCapacity> > () {}
	explicit HalfStaticArray(size_t InitialCapacity) : 
		Array<T, InlineStorage<T, InlineCapacity> > (InitialCapacity) {}
};

}	// namespace Firebird

#endif // CLASSES_ARRAY_H

