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
template <typename T, int Capacity>
class InlineStorage : public AutoStorage {
public:
	explicit InlineStorage(MemoryPool& p) : AutoStorage(p) { }
	InlineStorage() : AutoStorage() { }
protected:
	T* getStorage() {
		return buffer;
	}
	int getStorageSize() const {
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
	int getStorageSize() const { return 0; }
};

// Dynamic array of simple types
template <typename T, typename Storage = EmptyStorage<T> >
class Array : protected Storage {
public:
	explicit Array(MemoryPool& p) : 
		Storage(p), count(0), capacity(getStorageSize()), data(getStorage()) { }
	Array(MemoryPool& p, int InitialCapacity) : 
		Storage(p), count(0), capacity(getStorageSize()), data(getStorage())
	{
		ensureCapacity(InitialCapacity);
	}
	Array() : count(0), 
		capacity(getStorageSize()), data(getStorage()) { }
	explicit Array(int InitialCapacity) : count(0), 
		capacity(getStorageSize()), data(getStorage())
	{
		ensureCapacity(InitialCapacity);
	}
	~Array()
	{
		freeData();
	}
	void clear() { count = 0; };
protected:
	const T& getElement(int index) const {
  		fb_assert(index >= 0 && index < count);
  		return data[index];
	}
	T& getElement(int index) {
  		fb_assert(index >= 0 && index < count);
  		return data[index];
	}
	void freeData()
	{
		if (data != getStorage())
			getPool().deallocate(data);
	}
public:
	Array<T, Storage>& operator =(const Array<T, Storage>& L) 
	{
		ensureCapacity(L.count);
		memcpy(data, L.data, sizeof(T) * L.count);
		count = L.count;
		return *this;
	}
	const T& operator[](int index) const {
  		return getElement(index);
	}
	T& operator[](int index) {
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
	void insert(int index, const T& item) {
		fb_assert(index >= 0 && index <= count);
		ensureCapacity(count + 1);
		memmove(data + index + 1, data + index, sizeof(T) * (count++ - index));
		data[index] = item;
	}
	void insert(int index, const Array<T, Storage>& L) {
		fb_assert(index >= 0 && index <= count);
		ensureCapacity(count + L.count);
		memmove(data + index + L.count, data + index, sizeof(T) * (count - index));
		memcpy(data + index, L.data, L.count);
		count += L.count;
	}
	int add(const T& item) {
		ensureCapacity(count + 1);
		data[count++] = item;
  		return count;
	};
	void remove(int index) {
  		fb_assert(index >= 0 && index < count);
  		memmove(data + index, data + index + 1, sizeof(T) * (--count - index));
	}
	void remove(T* itr) {
		int index = itr - begin();
  		fb_assert(index >= 0 && index < count);
  		memmove(data + index, data + index + 1, sizeof(T) * (--count - index));
	}
	void shrink(int newCount) {
		fb_assert(newCount <= count);
		count = newCount;
	};
	// Grow size of our array and zero-initialize new items
	void grow(int newCount) {
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
	int getCount() const { return count; }
	int getCapacity() const { return capacity; }
	void push(const T& item) {
		add(item);
	}
	T pop() {
		fb_assert(count > 0);
		count--;
		return data[count];
	}
	// prepare array to be used as a buffer of capacity items
	T* getBuffer(int capacity) {
		ensureCapacity(capacity);
		count = capacity;
		return data;
	}
	// clear array and release dinamically allocated memory
	void free() 
	{
		clear();
		freeData();
		capacity = getStorageSize();
		data = getStorage();
	}

protected:
	int count, capacity;
	T* data;
	void ensureCapacity(int newcapacity) {
		if (newcapacity > capacity) {
			if (newcapacity < capacity * 2) {
				newcapacity = capacity * 2;
			}
			T* newdata = reinterpret_cast<T*>
				(getPool().allocate(sizeof(T) * newcapacity
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
	SortedArray(MemoryPool& p, int s) : Array<Value, Storage>(p, s) {}
	explicit SortedArray(MemoryPool& p) : Array<Value, Storage>(p) {}
	explicit SortedArray(int s) : Array<Value, Storage>(s) {}
	SortedArray() : Array<Value, Storage>() {}
	bool find(const Key& item, int& pos) const {
		int highBound = count, lowBound = 0;
		while (highBound > lowBound) {
			int temp = (highBound + lowBound) >> 1;
			if (Cmp::greaterThan(item, KeyOfValue::generate(this, data[temp])))
				lowBound = temp + 1;
			else
				highBound = temp;
		}
		pos = lowBound;
		return highBound != count &&
			!Cmp::greaterThan(KeyOfValue::generate(this, data[lowBound]), item);
	}
	int add(const Value& item) {
	    int pos;
  	    find(KeyOfValue::generate(this, item), pos);
		insert(pos, item);
		return pos;
	}
};

// Nice shorthand for arrays with static part
template <typename T, int InlineCapacity>
class HalfStaticArray : public Array<T, InlineStorage<T, InlineCapacity> > {
public:
	explicit HalfStaticArray(MemoryPool& p) : Array<T,InlineStorage<T, InlineCapacity> > (p) {}
	HalfStaticArray(MemoryPool& p, int InitialCapacity) : 
		Array<T, InlineStorage<T, InlineCapacity> > (p, InitialCapacity) {}
	HalfStaticArray() : Array<T,InlineStorage<T, InlineCapacity> > () {}
	explicit HalfStaticArray(int InitialCapacity) : 
		Array<T, InlineStorage<T, InlineCapacity> > (InitialCapacity) {}
};

}	// Firebird

#endif // CLASSES_ARRAY_H

