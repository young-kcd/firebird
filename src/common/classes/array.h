/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		array.h
 *	DESCRIPTION:	dynamic array of simple elements
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
 *  Copyright (c) 2003 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */
 
#ifndef ARRAY_H
#define ARRAY_H

#include <assert.h>
#include <string.h>
#include "../common/classes/alloc.h"

namespace Firebird {

// Static part of the array
template <typename T, int Capacity>
class InlineStorage {
public:
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
class EmptyStorage {
public:
	T* getStorage() { return NULL; }
	int getStorageSize() const { return 0; }
};

// Dynamic array of simple types
template <typename T, typename Storage = EmptyStorage<T> >
class Array : private Storage {
public:
	Array(MemoryPool* p) : 
	  count(0), capacity(getStorageSize()), data(getStorage()), pool(p)  {}
	Array(MemoryPool* p, int InitialCapacity) : count(0), 
		capacity(getStorageSize()), data(getStorage()), pool(p)  
	{
		ensureCapacity(InitialCapacity);
	}
	~Array()
	{
		if (data != getStorage())
			pool->deallocate(data);
	}
	void clear() { count = 0; };
protected:
	const T& getElement(int index) const {
  		assert(index >= 0 && index < count);
  		return data[index];
	}
	T& getElement(int index) {
  		assert(index >= 0 && index < count);
  		return data[index];
	}
public:
	const T& operator[](int index) const {
  		return getElement(index);
	}
	T& operator[](int index) {
  		return getElement(index);
	}
	const T& front() const {
  		assert(count > 0);
		return *data;
	}
	const T& back() const {
  		assert(count > 0);
		return *(data + count - 1);
	}
	const T* begin() const { return data; }
	const T* end() const { return data + count; }
	T& front() {
  		assert(count > 0);
		return *data;
	}
	T& back() {
  		assert(count > 0);
		return *(data + count - 1);
	}
	T* begin() { return data; }
	T* end() { return data + count; }
	void insert(int index, const T& item) {
		assert(index >= 0 && index <= count);
		ensureCapacity(count + 1);
		memmove(data + index + 1, data + index, sizeof(T) * (count++ - index));
		data[index] = item;
	}
	int add(const T& item) {
		ensureCapacity(count + 1);
		data[count++] = item;
  		return count;
	};
	void remove(int index) {
  		assert(index >= 0 && index < count);
  		memmove(data + index, data + index + 1, sizeof(T) * (--count - index));
	}
	void remove(T* itr) {
		int index = itr - begin();
  		assert(index >= 0 && index < count);
  		memmove(data + index, data + index + 1, sizeof(T) * (--count - index));
	}
	void shrink(int newCount) {
		assert(newCount <= count);
		count = newCount;
	};
	// Grow size of our array and zero-initialize new items
	void grow(int newCount) {
		assert(newCount >= count);
		ensureCapacity(newCount);
		memset(data + count, 0, sizeof(T) * (newCount - count));
		count = newCount;
	}
	void join(Array<T>& L) {
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
		assert(count > 0);
		count--;
		return data[count];
	}
protected:
	int count, capacity;
	T* data;
	MemoryPool* pool;
	void ensureCapacity(int newcapacity) {
		if (newcapacity > capacity) {
			if (newcapacity < capacity * 2) {
				newcapacity = capacity * 2;
			}
			T* newdata = reinterpret_cast<T*>
				(pool->allocate(sizeof(T) * newcapacity
#ifdef DEBUG_GDS_ALLOC
		, 1, __FILE__, __LINE__
#endif
						));
			memcpy(newdata, data, sizeof(T) * count);
			if (data != getStorage())
				pool->deallocate(data);
			data = newdata;
			capacity = newcapacity;
		}
	}
};

// Dynamic sorted array of simple objects
template <typename Value, 
	typename Key = Value, 
	typename KeyOfValue = DefaultKeyValue<Value>, 
	typename Cmp = DefaultComparator<Key> >
class SortedArray : public Array<Value> {
public:
	SortedArray(MemoryPool* p, int s) : Array<Value>(p, s) {}
	SortedArray(MemoryPool* p) : Array<Value>(p) {}
	bool find(const Key& item, int& pos) {
		int highBound = count, lowBound = 0;
		while (highBound > lowBound) {
			int temp = (highBound + lowBound) >> 1;
			if (Cmp::compare(item, KeyOfValue::generate(this, data[temp])))
				lowBound = temp + 1;
			else
				highBound = temp;
		}
		pos = lowBound;
		return highBound != count &&
			!Cmp::compare(KeyOfValue::generate(this, data[lowBound]), item);
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
	HalfStaticArray(MemoryPool* p) : Array<T,InlineStorage<T, InlineCapacity> > (p) {}
	HalfStaticArray(MemoryPool* p, int InitialCapacity) : 
		Array<T, InlineStorage<T, InlineCapacity> > (p, InitialCapacity) {}
};

}	// Firebird

#endif
