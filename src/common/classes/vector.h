/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		vector.h
 *	DESCRIPTION:	static array of simple elements
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * You may obtain a copy of the Licence at
 * http://www.gnu.org/licences/lgpl.html
 * 
 * As a special exception this file can also be included in modules
 * with other source code as long as that source code has been 
 * released under an Open Source Initiative certificed licence.  
 * More information about OSI certification can be found at: 
 * http://www.opensource.org 
 * 
 * This module is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public Licence for more details.
 * 
 * This module was created by members of the firebird development 
 * team.  All individual contributions remain the Copyright (C) of 
 * those individuals and all rights are reserved.  Contributors to 
 * this file are either listed below or can be obtained from a CVS 
 * history command.
 *
 *  Created by: Nickolay Samofatov <skidder@bssys.com>
 *
 *  Contributor(s):
 * 
 *
 *  $Id: vector.h,v 1.3 2003-09-08 20:23:32 skidder Exp $
 *
 */
 
#ifndef VECTOR_H
#define VECTOR_H

#include <assert.h>
#include <string.h>

namespace Firebird {

// Very fast static array of simple types
template <typename T, int Capacity>
class Vector {
public:
	Vector() : count(0) {}
	void clear() { count = 0; };
	T& operator[](int index) {
  		assert(index >= 0 && index < count);
  		return data[index];
	}
	T* begin() { return data; }
	T* end() { return data+count; }
	void insert(int index, const T& item) {
	  assert(index >= 0 && index <= count);
	  assert(count < Capacity);
	  memmove(data+index+1, data+index, sizeof(T)*(count++-index));
	  data[index] = item;
	}

	int add(const T& item) {
		assert(count < Capacity);
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
	void join(Vector<T,Capacity>& L) {
		assert(count + L.count <= Capacity);
		memcpy(data + count, L.data, sizeof(T)*L.count);
		count += L.count;
	}
	int getCount() const { return count; }
	int getCapacity() const { return Capacity; }
protected:
	int count;
	T data[Capacity];
};

// Template for default value comparsion
template <typename T>
class DefaultComparator {
public:
	static bool compare(const T& i1, const T& i2) {
	    return i1 > i2;
	}
};

// Template to convert value to index directly
template <typename T>
class DefaultKeyValue {
public:
	static const T& generate(void *sender, const T& Item) { return Item; }
};

// Fast sorted array of simple objects
// It is used for B+ tree nodes lower, but can still be used by itself
template <typename Value, int Capacity, typename Key = Value, 
	typename KeyOfValue = DefaultKeyValue<Value>, 
	typename Cmp = DefaultComparator<Key> >
class SortedVector : public Vector<Value, Capacity> {
public:
	SortedVector() : Vector<Value, Capacity>() {}
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

};

#endif
