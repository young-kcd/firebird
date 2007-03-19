/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		RecordNumber.h
 *	DESCRIPTION:	Handler class for table record number
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
 *  $Id: RecordNumber.h,v 1.5 2005-05-27 22:43:14 asfernandes Exp $
 *
 */

#ifndef JRD_RECORDNUMBER_H
#define JRD_RECORDNUMBER_H

const SINT64 EMPTY_NUMBER = QUADCONST(0);
const SINT64 BOF_NUMBER = QUADCONST(-1);

// This class is to be used everywhere you may need to handle record numbers. We 
// deliberately not define implicit conversions to and from integer to allow 
// compiler check errors on our migration path from 32-bit to 64-bit record 
// numbers.
class RecordNumber {
public:
	// Default constructor.
	inline RecordNumber() : value(EMPTY_NUMBER) {}

	// Copy constructor
	inline RecordNumber(const RecordNumber& from) : value(from.value) {}

	// Explicit constructor from 64-bit record number value
	inline explicit RecordNumber(SINT64 number) : value(number) {}

	// Assignment operator
	inline RecordNumber& operator =(const RecordNumber& from) { 
		value = from.value;
		return *this;
	}

	inline bool operator ==(const RecordNumber& other) const { 
		return value == other.value; 
	}

	inline bool operator !=(const RecordNumber& other) const { 
		return value != other.value; 
	}

	inline bool operator > (const RecordNumber& other) const { 
		return value > other.value; 
	}

	inline bool operator < (const RecordNumber& other) const { 
		return value < other.value; 
	}

	inline bool operator >= (const RecordNumber& other) const { 
		return value >= other.value; 
	}

	inline bool operator <= (const RecordNumber& other) const { 
		return value <= other.value; 
	}

	inline void decrement() { value--; }

	inline void increment() { value++; }

	inline SINT64 getValue() const { return value; }

	inline void setValue(SINT64 avalue) { value = avalue; }

	bool isBof() const { return value == BOF_NUMBER; }

	bool isEmpty() const { return value == EMPTY_NUMBER; }

	inline bool checkNumber(
		USHORT records_per_page, // ~400 (8k page)
		USHORT data_pages_per_pointer_page  // ~2000 (8k page)
	) const
	{
		// We limit record number value to 40 bits and make sure decomposed value
		// fits into 3 USHORTs. This all makes practical table size limit (not 
		// counting allocation threshold and overhead) roughtly equal to:
		// 16k page - 20000 GB
		// 8k page -  10000 GB
		// 4k page -   2500 GB
		// 2k page -    600 GB
		// 1k page -    150 GB
		// Large page size values are recommended for large databases because 
		// page allocators are generally linear.
		return value < QUADCONST(0x10000000000) && 
			value < (SINT64) MAX_USHORT * records_per_page * data_pages_per_pointer_page;
	}

	inline void decompose(
		USHORT records_per_page, // ~400 (8k page)
		USHORT data_pages_per_pointer_page,  // ~2000 (8k page)
		SSHORT& line,
		SSHORT& slot,
		USHORT& pp_sequence
	) const
	{
		line = value % records_per_page;
		const ULONG sequence = value / records_per_page;
		slot = sequence % data_pages_per_pointer_page;
		pp_sequence = sequence / data_pages_per_pointer_page;
	}

	inline void compose(
		USHORT records_per_page, // ~400 (8k page)
		USHORT data_pages_per_pointer_page,  // ~2000 (8k page)
		SSHORT line,
		SSHORT slot,
		USHORT pp_sequence
	) 
	{
		value = (((SINT64) pp_sequence) * data_pages_per_pointer_page + slot) * records_per_page + line;
	}

	// Handle encoding of record number for RDB$DB_KEY and BLOB ID structure.
	// bid_number array has special alignment requirement - its second byte must
	// be 4-byte aligned. This is generally not a problem because this routine is
	// called for BID or DB_KEY structure member which is already aligned 
	// appropriately.
	inline void bid_encode(UCHAR* bid_number) const {
		// BLOB ID is stored in database thus we do encode large record numbers 
		// in a manner which preserves backward compatibility with older ODS. 
		// The same applies to bid_decode routine below.

		// Store lower 32 bits of number
		*reinterpret_cast<ULONG*>(bid_number + 1) = value;
		// Store high 8 bits of number
		bid_number[0] = value >> 32;
	}

	inline void bid_decode(const UCHAR* bid_number) {
		value = 
			*reinterpret_cast<const ULONG*>(bid_number + 1) +
			(((UINT64) bid_number[0]) << 32);
	}

private:
	// Use signed value because negative values are widely used as flags in the 
	// engine. Since page number is the signed 32-bit integer and it is also may
	// be stored in this structure we want sign extension to take place.
	SINT64 value;
};

#endif // JRD_RECORDNUMBER_H
