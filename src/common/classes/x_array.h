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
 * Adriano dos Santos Fernandes
 */

#ifndef CLASSES_XARRAY_H
#define CLASSES_XARRAY_H

#include "../common/classes/array.h"

namespace Firebird {

// Dynamic array of simple types
template <typename T, typename Storage = EmptyStorage<T> >
class XArray : protected Storage
{
public:
	typedef FB_SIZE_T size_type;
	typedef FB_SSIZE_T difference_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;
	typedef pointer iterator;
	typedef const_pointer const_iterator;

	explicit XArray(MemoryPool& p)
		: Storage(p), count(0), capacity(this->getStorageSize()),
		  dataCopy(this->getStorage()), data(this->getStorage())
	{
		// Ensure we can carry byte copy operations.
		fb_assert(capacity < FB_MAX_SIZEOF / sizeof(T));
	}

	XArray(MemoryPool& p, const size_type InitialCapacity)
		: Storage(p), count(0), capacity(this->getStorageSize()),
		  dataCopy(this->getStorage()), data(this->getStorage())
	{
		ensureCapacity(InitialCapacity);
	}

	XArray(MemoryPool& p, const XArray<T, Storage>& source)
		: Storage(p), count(0), capacity(this->getStorageSize()),
		  dataCopy(this->getStorage()), data(this->getStorage())
	{
		copyFrom(source);
	}

	XArray() : count(0), capacity(this->getStorageSize()),
		dataCopy(this->getStorage()), data(this->getStorage()) { }

	explicit XArray(const size_type InitialCapacity)
		: Storage(), count(0), capacity(this->getStorageSize()),
		  dataCopy(this->getStorage()), data(this->getStorage())
	{
		ensureCapacity(InitialCapacity);
	}

	XArray(const XArray<T, Storage>& source)
		: Storage(), count(0), capacity(this->getStorageSize()),
		  dataCopy(this->getStorage()), data(this->getStorage())
	{
		copyFrom(source);
	}

	~XArray()
	{
		freeData();
	}

	void clear() throw()
	{
		count = 0;
	}

protected:
	const T& getElement(size_type index) const throw()
	{
  		fb_assert(index < count);
  		validDataCopy();
  		return data[index];
	}

	T& getElement(size_type index) throw()
	{
  		fb_assert(index < count);
  		validDataCopy();
  		return data[index];
	}

	void freeData()
	{
  		validDataCopy();
		// CVC: Warning, after this call, "data" is an invalid pointer, be sure to reassign it
		// or make it equal to this->getStorage()
		if (data != this->getStorage())
			Firebird::MemoryPool::globalFree(data);
	}

	void copyFrom(const XArray<T, Storage>& source)
	{
		ensureCapacity(source.count, false);
		memcpy(data, source.data, sizeof(T) * source.count);
		count = source.count;
	}

public:
	XArray<T, Storage>& operator =(const XArray<T, Storage>& source)
	{
		copyFrom(source);
		return *this;
	}

	const T& operator[](size_type index) const throw()
	{
  		return getElement(index);
	}

	T& operator[](size_type index) throw()
	{
  		return getElement(index);
	}

	const T& front() const
	{
  		fb_assert(count > 0);
  		validDataCopy();
		return *data;
	}

	const T& back() const
	{
  		fb_assert(count > 0);
  		validDataCopy();
		return *(data + count - 1);
	}

	const T* begin() const
	{
  		validDataCopy();
		return data;
	}

	const T* end() const
	{
  		validDataCopy();
		return data + count;
	}

	T& front()
	{
  		fb_assert(count > 0);
  		validDataCopy();
		return *data;
	}

	T& back()
	{
  		fb_assert(count > 0);
  		validDataCopy();
		return *(data + count - 1);
	}

	T* begin()
	{
  		validDataCopy();
		return data;
	}

	T* end()
	{
		return data + count;
  		validDataCopy();
	}

	void insert(const size_type index, const T& item)
	{
		fb_assert(index <= count);
		fb_assert(count < FB_MAX_SIZEOF);
		ensureCapacity(count + 1);
		memmove(data + index + 1, data + index, sizeof(T) * (count++ - index));
		data[index] = item;
	}

	void insert(const size_type index, const XArray<T, Storage>& items)
	{
		fb_assert(index <= count);
		fb_assert(count <= FB_MAX_SIZEOF - items.count);
		ensureCapacity(count + items.count);
		memmove(data + index + items.count, data + index, sizeof(T) * (count - index));
		memcpy(data + index, items.data, items.count);
		count += items.count;
	}

	void insert(const size_type index, const T* items, const size_type itemsCount)
	{
		fb_assert(index <= count);
		fb_assert(count <= FB_MAX_SIZEOF - itemsCount);
		ensureCapacity(count + itemsCount);
		memmove(data + index + itemsCount, data + index, sizeof(T) * (count - index));
		memcpy(data + index, items, sizeof(T) * itemsCount);
		count += itemsCount;
	}

	size_type add(const T& item)
	{
		ensureCapacity(count + 1);
		data[count] = item;
  		return count++;
	}

	void add(const T* items, const size_type itemsCount)
	{
		fb_assert(count <= FB_MAX_SIZEOF - itemsCount);
		ensureCapacity(count + itemsCount);
		memcpy(data + count, items, sizeof(T) * itemsCount);
		count += itemsCount;
	}

	T* remove(const size_type index) throw()
	{
  		fb_assert(index < count);
  		validDataCopy();
  		memmove(data + index, data + index + 1, sizeof(T) * (--count - index));
		return &data[index];
	}

	T* removeRange(const size_type from, const size_type to) throw()
	{
  		fb_assert(from <= to);
  		fb_assert(to <= count);
  		validDataCopy();
  		memmove(data + from, data + to, sizeof(T) * (count - to));
		count -= (to - from);
		return &data[from];
	}

	T* removeCount(const size_type index, const size_type n) throw()
	{
  		fb_assert(index + n <= count);
  		validDataCopy();
  		memmove(data + index, data + index + n, sizeof(T) * (count - index - n));
		count -= n;
		return &data[index];
	}

	T* remove(const T* itr) throw()
	{
		const size_type index = itr - begin();
  		fb_assert(index < count);
  		validDataCopy();
  		memmove(data + index, data + index + 1, sizeof(T) * (--count - index));
		return &data[index];
	}

	T* remove(const T* itrFrom, const T* itrTo) throw()
	{
		return removeRange(itrFrom - begin(), itrTo - begin());
	}

	void shrink(size_type newCount) throw()
	{
		fb_assert(newCount <= count);
		count = newCount;
	}

	// Grow size of our array and zero-initialize new items
	void grow(const size_type newCount)
	{
		fb_assert(newCount >= count);
		ensureCapacity(newCount);
		memset(data + count, 0, sizeof(T) * (newCount - count));
		count = newCount;
	}

	// Resize array according to STL's vector::resize() rules
	void resize(const size_type newCount, const T& val)
	{
		if (newCount > count)
		{
			ensureCapacity(newCount);
			while (count < newCount) {
				data[count++] = val;
			}
		}
		else {
			count = newCount;
		}
	}

	// Resize array according to STL's vector::resize() rules
	void resize(const size_type newCount)
	{
		if (newCount > count) {
			grow(newCount);
		}
		else {
			count = newCount;
		}
	}

	void join(const XArray<T, Storage>& L)
	{
		fb_assert(count <= FB_MAX_SIZEOF - L.count);
		ensureCapacity(count + L.count);
		memcpy(data + count, L.data, sizeof(T) * L.count);
		count += L.count;
	}

	void assign(const XArray<T, Storage>& source)
	{
		copyFrom(source);
	}

	void assign(const T* items, const size_type itemsCount)
	{
		resize(itemsCount);
		memcpy(data, items, sizeof(T) * count);
	}

	size_type getCount() const throw() { return count; }

	bool isEmpty() const { return count == 0; }

	bool hasData() const { return count != 0; }

	size_type getCapacity() const { return capacity; }

	void push(const T& item)
	{
		add(item);
	}

	void push(const T* items, const size_type itemsSize)
	{
		fb_assert(count <= FB_MAX_SIZEOF - itemsSize);
		ensureCapacity(count + itemsSize);
		memcpy(data + count, items, sizeof(T) * itemsSize);
		count += itemsSize;
	}

	void append(const T* items, const size_type itemsSize)
	{
		push(items, itemsSize);
	}

	void append(const XArray<T, Storage>& source)
	{
		push(source.begin(), source.getCount());
	}

	T pop()
	{
		fb_assert(count > 0);
		count--;
  		validDataCopy();
		return data[count];
	}

	// prepare array to be used as a buffer of capacity items
	T* getBuffer(const size_type capacityL, bool preserve = true)
	{
		ensureCapacity(capacityL, preserve);
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

	// This method only assigns "pos" if the element is found.
	// Maybe we should modify it to iterate directy with "pos".
	bool find(const T& item, size_type& pos) const
	{
  		validDataCopy();
		for (size_type i = 0; i < count; i++)
		{
			if (data[i] == item)
			{
				pos = i;
				return true;
			}
		}
		return false;
	}

	bool exist(const T& item) const
	{
		size_type pos;	// ignored
		return find(item, pos);
	}

	bool operator==(const XArray& op) const
	{
		if (count != op.count)
			return false;
  		validDataCopy();
		return memcmp(data, op.data, count) == 0;
	}

	// Member function only for some debugging tests. Hope nobody is bothered.
	void swapElems()
	{
  		validDataCopy();
		const size_type limit = count / 2;
		for (size_type i = 0; i < limit; ++i)
		{
			T temp = data[i];
			data[i] = data[count - 1 - i];
			data[count - 1 - i] = temp;
		}
	}

protected:
	size_type count, capacity;
	mutable T* dataCopy;
	mutable T* data;

	static bool validDataCopy(T** d1, T* d2, const char* txt)
	{
		if (((U_IPTR) *d1) & 1)
		{
			*d1 = d2;
			gds__log("data%s validation error at %p", txt, d1);
			return true;
		}
		return false;
	}

	void validDataCopy() const
	{
		if (!count)
			return;

		bool errCopy = validDataCopy(&dataCopy, data, "Copy");
		bool errData = validDataCopy(&data, dataCopy, "");
		if (errCopy && errData)
		{
			gds__log("Fatal validation error for %p", this);
			abort();
		}
	}

	void ensureCapacity(size_type newcapacity, bool preserve = true)
	{
  		validDataCopy();
		if (newcapacity > capacity)
		{
			if (capacity <= FB_MAX_SIZEOF / 2)
			{
				if (newcapacity < capacity * 2)
					newcapacity = capacity * 2;
			}
			else
			{
				newcapacity = FB_MAX_SIZEOF;
			}

			// Ensure we can carry byte copy operations.
			// What to do here, throw in release build?
			fb_assert(newcapacity < FB_MAX_SIZEOF / sizeof(T));

			T* newdata = static_cast<T*>
				(this->getPool().allocate(sizeof(T) * newcapacity ALLOC_ARGS));
			if (preserve)
				memcpy(newdata, data, sizeof(T) * count);
			freeData();
			data = newdata;
			capacity = newcapacity;
		}
	}
};

}	// namespace Firebird

#endif // CLASSES_XARRAY_H
