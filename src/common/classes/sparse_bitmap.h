/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		sparse_bitmap.h
 *	DESCRIPTION:	generic sparse bitmap of integers
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
 *  The Original Code was created by Nickolay Samofatov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Nickolay Samofatov <nickolay@broadviewsoftware.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 *  $Id: sparse_bitmap.h,v 1.7 2004-10-27 09:27:22 aafemt Exp $
 *
 */

// FIXME, FIXME: migrate SparseBitmap::Accessor to current_bucket!
 
#ifndef SPARSE_BITMAP_H
#define SPARSE_BITMAP_H

#include "../common/classes/alloc.h"

namespace Firebird {

struct BitmapTypes_32 {
	typedef ULONG BUNCH_T;
	enum {
		LOG2_BUNCH_BITS = 7,
		BUNCH_BITS = 32
	};
};

struct BitmapTypes_64 {
	typedef UINT64 BUNCH_T;
	enum {
		LOG2_BUNCH_BITS = 8,
		BUNCH_BITS = 64
	};
};

#define BUNCH_ONE  ((BUNCH_T)1)

template <typename T, typename InternalTypes = BitmapTypes_64>
class SparseBitmap : public AutoStorage {
public:
	// Default constructor, stack placement
	SparseBitmap() : 
		singular(false), singular_value(0), tree(&getPool()), defaultAccessor(this)
	{ }

	// Pooled constructor
	SparseBitmap(MemoryPool& p) : 
		AutoStorage(p), singular(false), singular_value(0), tree(&getPool()), defaultAccessor(this)
	{ }

	// Default accessor methods
	bool locate(const T& key) {
		return defaultAccessor.locate(locEqual, key);
	}
	
	bool locate(LocType lt, const T key) {
		return defaultAccessor.locate(lt, key);
	}

	bool getFirst() {
		return defaultAccessor.getFirst();
	}

	bool getLast() {
		return defaultAccessor.getLast();
	}

	bool getNext() {
		return defaultAccessor.getNext();
	}

	bool getPrev() {
		return defaultAccessor.getPrev();
	}

	T current() const { return defaultAccessor.current(); }

	// Set bit
	void set(T value) {
		if (singular) {
			// If we are trying to set the same bit as already set - do nothing
			if (singular_value == value)
				return;

			// Add singular value to the tree
			fb_assert(tree.isEmpty());

			singular = false;

			Bucket bucket;
			bucket.start_value = singular_value & ~(T) (BUNCH_BITS - 1);
			bucket.bits = BUNCH_ONE << (singular_value - bucket.start_value);
			tree.add(bucket);
		} else {
			if (tree.isEmpty()) {
				singular = true;
				singular_value = value;
				return;
			}
		}

		T val_aligned = value & ~(T) (BUNCH_BITS - 1);
		BUNCH_T bit_mask = BUNCH_ONE << (value - val_aligned);
		if (tree.locate(val_aligned)) {
			tree.current().bits |= bit_mask;
		} else {
			Bucket bucket;
			bucket.start_value = val_aligned;
			bucket.bits = bit_mask;
			tree.add(bucket);
		}
	}

	bool clear(T value) {
		if (singular) {
			fb_assert(tree.isEmpty());

			if (value == singular_value) {
				singular = false;
				return true;
			}
			return false;
		}

		T val_aligned = value & ~(T)(BUNCH_BITS-1);
		BUNCH_T bit_mask = BUNCH_ONE << (value - val_aligned);
		if (tree.locate(val_aligned)) {
			Bucket *current_bucket = &tree.current();
			if (current_bucket->bits & bit_mask) {
				current_bucket->bits &= ~bit_mask;
				if (!current_bucket->bits)
					tree.fastRemove();
				return true;
			}
		}
		return false;
	}

	bool test(T value) {
		if (singular) {
			fb_assert(tree.isEmpty());
			return value == singular_value;
		}

		T val_aligned = value & ~(T) (BUNCH_BITS - 1);
		BUNCH_T bit_mask = BUNCH_ONE << (value - val_aligned);
		BitmapTreeAccessor i(&tree); // Use accessor to be const-correct
		if (i.locate(val_aligned)) {
			return i.current().bits & bit_mask;
		}
		return false;
	}

	static bool test(SparseBitmap* bitmap, T value) {
		if (!bitmap)
			return false;
		return bitmap->test(value);
	}

	// Clear bitmap if it is not NULL
	static void reset(SparseBitmap* bitmap) {
		if (bitmap)
			bitmap->clear();
	}

	size_t approxSize() {
		return sizeof(*this) + tree.approxSize();
	}

	// Make bitmap empty
	void clear() {
		singular = false;
		tree.clear();
	}

 	// Compute the union of two bitmaps. 
	// Note: this method uses one of the bitmaps to return result
	static SparseBitmap* bit_or(SparseBitmap* s1, SparseBitmap* s2);

 	// Compute the intersection of two bitmaps.
	// Note: this method uses one of the bitmaps to return result
	static SparseBitmap* bit_and(SparseBitmap* s1, SparseBitmap* s2);

protected:
	// Internal types and constants
	typedef typename InternalTypes::BUNCH_T BUNCH_T;
	enum {
		LOG2_BUNCH_BITS = InternalTypes::LOG2_BUNCH_BITS,
		BUNCH_BITS = InternalTypes::BUNCH_BITS
	};

	// Bucket with bits
	struct Bucket {
		T start_value; // starting value, BUNCH_BITS-aligned
		BUNCH_T bits;  // bits data
		inline static const T& generate(const void* sender, const Bucket& i) {
			return i.start_value;
		}
	};

	typedef BePlusTree<Bucket, T, MemoryPool, Bucket> BitmapTree;
	typedef typename BitmapTree::Accessor BitmapTreeAccessor;

	// Set if bitmap contains a single value only
	bool singular;
	T singular_value;

	BitmapTree tree;

private:
	SparseBitmap(const SparseBitmap& from); // Copy constructor. Not implemented for now.
	SparseBitmap& operator =(const SparseBitmap& from); // Assignment operator. Not implemented for now.

public:
	class Accessor {
	public:
		Accessor(SparseBitmap* _bitmap) : 
			bitmap(_bitmap), treeAccessor(_bitmap ? &_bitmap->tree : NULL), bit_mask(BUNCH_ONE), current_value(0) {}
	
		bool locate(const T& key) {
			return locate(locEqual, key);
		}
	
		// Position accessor on item having LocType relationship with given key
		// If method returns false position of accessor is not defined.
		bool locate(LocType lt, const T key) {
			// Small convenience related to fact engine likes to use NULL SparseBitmap pointers
			if (!bitmap)
				return false;

			// This routine is not perfectly optimized, but is simple and functional
			// If need arises to use it in performance-critical places separation
			// of handling for different LocType cases may make sense.

			if (bitmap->singular) {
				// Trivial handling for singular bitmap
				current_value = bitmap->singular_value;

				switch (lt) {
				case locEqual: 
					return current_value == key;
				case locGreatEqual:
					return current_value >= key;
				case locLessEqual:
					return current_value <= key;
				case locLess:
					return current_value < key;
				case locGreat:
					return current_value > key;
				}
				return false;
			}

			// Transform locLess and locGreat to locLessEqual and locGreatEqual
			switch (lt) {
				case locLess:
					if (key == 0)
						return false;
					key--;
					lt = locLessEqual;
					break;
				case locGreat:
					if (key == ~(T)0)
						return false;
					key++;
					lt = locGreatEqual;
					break;
			}

			// Look up a bucket for our key
			T key_aligned = key & ~(T) (BUNCH_BITS - 1);
			if (!treeAccessor.locate(lt, key_aligned)) {
				// If we didn't find the desired bucket no way we can find desired value
				return false;
			}

			switch (lt) {
				case locEqual:
					current_value = key;
					bit_mask = BUNCH_ONE << (key - key_aligned);
					return treeAccessor.current().bits & bit_mask;
				case locGreatEqual: {
					// Initialize bit_mask
					if (treeAccessor.current().start_value == key_aligned) {
						current_value = key;
						bit_mask = BUNCH_ONE << (key - key_aligned);
					} else {
						current_value = treeAccessor.current().start_value;
						bit_mask = BUNCH_ONE;
					}

					// Scan bucket forwards looking for a match
					BUNCH_T tree_bits = treeAccessor.current().bits;
					do {
						if (tree_bits & bit_mask)
							return true;
						bit_mask <<= 1;
						current_value++;
					} while (bit_mask);

					// We scanned bucket, but found no match.
					// No problem, scan the next bucket (there should be at least one bit set for a bucket)
					if (!treeAccessor.getNext())
						return false;

					tree_bits = treeAccessor.current().bits;
					bit_mask = BUNCH_ONE;
					current_value = treeAccessor.current().start_value;
					do {
						if (tree_bits & bit_mask)
							return true;
						bit_mask <<= 1;
						current_value++;
					} while (bit_mask);

					// Bucket must contain one bit at least
					fb_assert(false);
				}
				case locLessEqual: {
					// Initialize bit_mask
					if (treeAccessor.current().start_value == key_aligned) {
						current_value = key;
						bit_mask = BUNCH_ONE << (key - key_aligned);
					} else {
						current_value = treeAccessor.current().start_value;
						bit_mask = BUNCH_ONE << (BUNCH_BITS - 1);
					}

					// Scan bucket backwards looking for a match
					BUNCH_T tree_bits = treeAccessor.current().bits;
					do {
						if (tree_bits & bit_mask)
							return true;
						bit_mask >>= 1;
						current_value--;
					} while (bit_mask);

					// We scanned bucket, but found no match.
					// No problem, scan the next bucket (there should be at least one bit set for a bucket)
					if (!treeAccessor.getPrev())
						return false;

					tree_bits = treeAccessor.current().bits;
					bit_mask = BUNCH_ONE << (BUNCH_BITS - 1);
					current_value = treeAccessor.current().start_value + BUNCH_BITS - 1;
					do {
						if (tree_bits & bit_mask)
							return true;
						bit_mask >>= 1;
						current_value--;
					} while (bit_mask);

					// Bucket must contain one bit at least
					fb_assert(false);
				}
			}
			fb_assert(false); // Invalid constant is used ?
		}

		// If method returns false it means list is empty and 
		// position of accessor is not defined.
		bool getFirst() {
			// Small convenience related to fact engine likes to use NULL SparseBitmap pointers
			if (!bitmap)
				return false;

			if (bitmap->singular) {
				current_value = bitmap->singular_value;
				return true;
			}

			if (!treeAccessor.getFirst()) return false;

			BUNCH_T tree_bits = treeAccessor.current().bits;
			bit_mask = BUNCH_ONE;
			current_value = treeAccessor.current().start_value;
			do {
				if (tree_bits & bit_mask)
					return true;
				bit_mask <<= 1;
				current_value++;
			} while (bit_mask);

			// Bucket must contain one bit at least
			fb_assert(false);
			return false;
		}

		// If method returns false it means list is empty and 
		// position of accessor is not defined.
		bool getLast() {
			// Small convenience related to fact engine likes to use NULL SparseBitmap pointers
			if (!bitmap)
				return false;

			if (bitmap->singular) {
				current_value = bitmap->singular_value;
				return true;
			}

			if (!treeAccessor.getLast())
				return false;

			BUNCH_T tree_bits = treeAccessor.current().bits;
			bit_mask = BUNCH_ONE << (BUNCH_BITS - 1);
			current_value = treeAccessor.current().start_value + BUNCH_BITS - 1;
			do {
				if (tree_bits & bit_mask)
					return true;
				bit_mask >>= 1;
				current_value--;
			} while (bit_mask);

			// Bucket must contain one bit at least
			fb_assert(false);
		}

		// Accessor position must be establised via successful call to getFirst(), 
		// getLast() or locate() before you can call this method
		bool getNext() {
			if (bitmap->singular)
				return false;

			// Use temporaries to avoid corrupting position if there is no next item in bitmap
			BUNCH_T try_mask = bit_mask;
			T try_value = current_value;

			// Proceed to next value
			try_mask <<= 1;
			try_value++;

			// Scan bucket forwards looking for a match
			BUNCH_T tree_bits = treeAccessor.current().bits;
			while (try_mask) {
				if (tree_bits & try_mask) {
					bit_mask = try_mask;
					current_value = try_value;
					return true;
				}
				try_mask <<= 1;
				try_value++;
			}

			// We scanned bucket, but found no match.
			// No problem, scan the next bucket (there should be at least one bit set for a bucket)
			if (!treeAccessor.getNext())
				return false;

			tree_bits = treeAccessor.current().bits;
			try_mask = BUNCH_ONE;
			try_value = treeAccessor.current().start_value;
			do {
				if (tree_bits & try_mask) {
					bit_mask = try_mask;
					current_value = try_value;
					return true;
				}
				try_mask <<= 1;
				try_value++;
			} while (try_mask);

			// Bucket must contain one bit at least
			fb_assert(false); 
			return false; // Absence of this statement makes GCC 3.4 generate wrong code here
		}

		// Accessor position must be establised via successful call to getFirst(), 
		// getLast() or locate() before you can call this method
		bool getPrev() {
			if (bitmap->singular)
				return false;

			// Use temporaries to avoid corrupting position if there is no next item in bitmap
			BUNCH_T try_mask = bit_mask;
			T try_value = current_value;

			// Proceed to next value
			try_mask >>= 1;
			try_value--;

			// Scan bucket backwards looking for a match
			BUNCH_T tree_bits = treeAccessor.current().bits;
			while (try_mask) {
				if (tree_bits & try_mask) {
					bit_mask = try_mask;
					current_value = try_value;
					return true;
				}
				try_mask >>= 1;
				try_value--;
			}

			// We scanned bucket, but found no match.
			// No problem, scan the next bucket (there should be at least one bit set for a bucket)
			if (!treeAccessor.getPrev())
				return false;

			tree_bits = treeAccessor.current().bits;
			try_mask = BUNCH_ONE << (BUNCH_BITS - 1);
			try_value = treeAccessor.current().start_value + BUNCH_BITS - 1;
			do {
				if (tree_bits & try_mask) {
					bit_mask = try_mask;
					current_value = try_value;
					return true;
				}
				try_mask >>= 1;
				try_value--;
			} while (bit_mask);

			// Bucket must contain one bit at least
			fb_assert(false);
		}

	    T current() const { return current_value; }
	private:
		SparseBitmap* bitmap;
		BitmapTreeAccessor treeAccessor;
		BUNCH_T bit_mask; // MIGRATE TO current_bucket to stop search early
		T current_value;
	};
private:
	Accessor defaultAccessor;

	friend class Accessor;
};

template <typename T, typename InternalTypes>
SparseBitmap<T, InternalTypes>* 
SparseBitmap<T, InternalTypes>::bit_or(
	SparseBitmap<T, InternalTypes>* dest, 
	SparseBitmap<T, InternalTypes>* source
) 
{
	// Handle the case when one or the other of the bitmaps is NULL
	if (!dest) {
		return source;
	}

	if (!source) {
		return dest;
	}

	// Make sure target differs from destination
	fb_assert(dest != source);

	// First bitmap is singular. Set appropriate bit in second and return it
	if (dest->singular) {
		source->set(dest->singular_value);
		//delete dest;
		return source;		
	}

	// Second bitmap is singular. Set appropriate bit in first and return it
	if (source->singular) {
		dest->set(source->singular_value);
		//delete source;
		return dest;		
	}

	// If second bitmap seems larger then use it as a target
	if (source->tree.seemsBiggerThan(dest->tree)) {
		// Flip bitmaps
		SparseBitmap* temp = dest;
		dest = source;
		source = temp;
	}

	bool sourceFound = source->tree.getFirst();

	// Source bitmap is empty. We have nothing to do thus return.
	if (!sourceFound) {
		//delete source;
		return dest;
	}

	// Both source and destination are empty. Return destination FWIW.
	bool destFound = dest->tree.getFirst();
	if (!destFound) {
		//delete source;
		return dest;
	}
		
	T destValue = dest->tree.current().start_value;
	T sourceValue = source->tree.current().start_value;

	while (sourceFound) {
		if (destFound) {
			// See if we need to skip value in destination tree
			if (destValue < sourceValue) {
				if ((destFound = dest->tree.getNext()))
					destValue = dest->tree.current().start_value;
				continue;
			}

			// Positions of our trees match
			if (destValue == sourceValue) {
				dest->tree.current().bits |= source->tree.current().bits;

				if ((destFound = dest->tree.getNext()))
					destValue = dest->tree.current().start_value;

				if ((sourceFound = source->tree.getNext()))
					sourceValue = source->tree.current().start_value;
				continue;
			}

			// Need to add some buckets to destination tree.
			// Try to add them in a row to avoid accessor resync after addition
			while (true) {
				dest->tree.add(source->tree.current());

				if (!(sourceFound = source->tree.getNext()))
					break;

				sourceValue = source->tree.current().start_value;

				// Resync accessor position if necessary
				if (destValue <= sourceValue) {
					dest->tree.locate(destValue);
					break;
				}
			}
		} else {
			// Add remaining buckets to destination tree and get out
			do {
				dest->tree.add(source->tree.current());
			} while (source->tree.getNext());

			break;
		}
	} 

	//delete source;
	return dest;
}

template <typename T, typename InternalTypes>
SparseBitmap<T, InternalTypes>* 
SparseBitmap<T, InternalTypes>::bit_and(
	SparseBitmap<T, InternalTypes>* dest, 
	SparseBitmap<T, InternalTypes>* source
) 
{
	// Handle the case when one or the other of the bitmaps is NULL
	if (!dest) {
		//delete source;
		return NULL;
	}

	if (!source) {
		//delete dest;
		return NULL;
	}

	// Make sure target differs from destination
	fb_assert(dest != source);

	// First bitmap is singular. Test appropriate bit in second and return first
	if (dest->singular) {
		if (!source->test(dest->singular_value))
			dest->singular = false;
		return dest;
	}

	// Second bitmap is singular. Test appropriate bit in first and return second
	if (source->singular) {
		if (!dest->test(source->singular_value))
			source->singular = false;
		return source;
	}

	// If second bitmap seems smaller then use it as a target
	if (dest->tree.seemsBiggerThan(source->tree)) {
		// Flip bitmaps
		SparseBitmap* temp = dest;
		dest = source;
		source = temp;
	}

	bool destFound = dest->tree.getFirst();

	// Destination bitmap is empty. We have nothing to do thus return.
	if (!destFound) {
		//delete source;
		return dest;
	}
		
	bool sourceFound = source->tree.getFirst();

	// Both source and destination are empty. Return destination FWIW.
	if (!sourceFound) {
		//delete source;
		return dest;
	}

	T destValue = dest->tree.current().start_value;
	T sourceValue = source->tree.current().start_value;

	while (destFound) {
		if (sourceFound) {
			// See if we need to skip value in destination tree
			if (sourceValue < destValue) {
				if ((sourceFound = source->tree.getNext()))
					sourceValue = source->tree.current().start_value;
				continue;
			}

			// Positions of our trees match
			if (sourceValue == destValue) {
				BUNCH_T bits = dest->tree.current().bits &= source->tree.current().bits;

				// Move to the next item of destination tree
				if (bits)
					destFound = dest->tree.getNext();
				else
					destFound = dest->tree.fastRemove();

				if (destFound)
					destValue = dest->tree.current().start_value;

				// Move to the next item of source tree
				if ((sourceFound = source->tree.getNext()))
					sourceValue = source->tree.current().start_value;
				continue;
			}

			// Need to remove some buckets from destination tree.
			// if (sourceValue > destValue)
			if ((destFound = dest->tree.fastRemove()))
				destValue = dest->tree.current().start_value;
		} else {
			// Trim out remaining values from destination list and get out
			while (dest->tree.fastRemove()) 
			{ }
			break;
		}
	}

	//delete source;
	return dest;
}

} // namespace Firebird

#endif
