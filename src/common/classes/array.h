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
 
#ifndef ARRAY_H
#define ARRAY_H

#include <assert.h>
#include <string.h>
#include "../common/classes/vector.h"
#include "../common/classes/alloc.h"

namespace Firebird {

// Dynamic array of simple types
template <typename T>
class Array {
public:
	Array(MemoryPool*p, int InitialSize) : 
	  pool(p), data(0), count(0), capacity(0) {grow(InitialSize);}
	~Array() {pool->deallocate(data);}
	void clear() { count = 0; };
	T& operator[](int index) {
  		assert(index >= 0 && index < count);
  		return data[index];
	}
	T* begin() { return data; }
	T* end() { return data+count; }
	void insert(int index, const T& item) {
		assert(index >= 0 && index <= count);
		grow(1);
		memmove(data+index+1, data+index, sizeof(T)*(count++-index));
		data[index] = item;
	}
	int add(const T& item) {
		grow(1);
		data[count++] = item;
  		return count;
	};
	void remove(int index) {
  		assert(index >= 0 && index < count);
  		memmove(data+index, data+index+1, sizeof(T)*(--count-index));
	}
	void shrink(int newCount) {
		assert(newCount <= count);
		count = newCount;
	};
	void join(Array<T>& L) {
		grow(L.count);
		memcpy(data + count, L.data, sizeof(T)*L.count);
		count += L.count;
	}
	int getCount() const { return count; }
	int getCapacity() const { return capacity; }
protected:
	int count, capacity;
	T* data;
	MemoryPool* pool;
	void grow(int incr) {
		int newcapacity = incr + count;
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
	SortedArray(MemoryPool *p, int s) : Array<Value>(p, s) {}
	bool find(const Key& item, int& pos) {
		int highBound=count, lowBound=0;
		while (highBound > lowBound) {
			int temp = (highBound + lowBound) >> 1;
			if (Cmp::compare(item, KeyOfValue::generate(this,data[temp])))
				lowBound = temp+1;
			else
				highBound = temp;
		}
		pos = lowBound;
		return highBound != count &&
			!Cmp::compare(KeyOfValue::generate(this,data[lowBound]), item);
	}
	int add(const Value& item) {
	    int pos;
  	    find(KeyOfValue::generate(this,item),pos);
		insert(pos,item);
		return pos;
	}
};

}	// Firebird

#endif
