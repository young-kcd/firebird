/*
 *	PROGRAM:	string class definition
 *	MODULE:		fb_string.cpp
 *	DESCRIPTION:	Provides almost that same functionality,
 *			that STL::basic_string<char> does, 
 *			but behaves MemoryPools friendly.
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

#include "firebird.h"
#include "fb_string.h"

#include <ctype.h>

#ifdef HAVE_STRCASECMP
#define STRNCASECMP strncasecmp
#else
#ifdef HAVE_STRICMP
#define STRNCASECMP strnicmp
#else
namespace {
	int StringIgnoreCaseCompare(const char* s1, const char* s2, unsigned int l) {
		while (l--) {
			const int delta = toupper(*s1++) - toupper(*s2++);
			if (delta) {
				return delta;
			}
		}
		return 0;
	}
}
#define STRNCASECMP StringIgnoreCaseCompare
#endif // HAVE_STRICMP
#endif // HAVE_STRCASECMP

namespace {
	class strBitMask {
	private:
		char m[32];
	public:
		strBitMask(Firebird::AbstractString::const_pointer s, Firebird::AbstractString::size_type l) {
			memset(m, 0, sizeof(m));
			if (l == Firebird::AbstractString::npos) {
				l = strlen(s);
			}
			Firebird::AbstractString::const_pointer end = s + l;
			while (s < end) {
				const unsigned char c = static_cast<unsigned char>(*s++);
				m[c >> 3] |= (1 << (c & 7));
			}
		}
		inline bool Contains(const char c) {
			return m[c >> 3] & (1 << (c & 7));
		}
	};
}

namespace Firebird {
	const AbstractString::size_type AbstractString::npos = ~0;

	AbstractString::AbstractString(const AbstractString& v) {
		memcpy(createStorage(v.length()), v.c_str(), v.length());
	}

	AbstractString::AbstractString(size_type size, const_pointer data) {
		memcpy(createStorage(size), data, size);
	}

	AbstractString::AbstractString(const_pointer p1, size_type n1, 
				 const_pointer p2, size_type n2)
	{
		char *s = createStorage(n1 + n2);
		memcpy(s, p1, n1);
		memcpy(&s[n1], p2, n2);
	}

	AbstractString::AbstractString(size_type size, char_type c) {
		memset(createStorage(size), c, size);
	}

	void AbstractString::AdjustRange(size_type length, size_type& pos, size_type& n) {
		if (pos == npos) {
			pos = length > n ? length - n : 0;
		}
		if (pos >= length) {
			pos = length;
			n = 0;
			return;
		}
		else if (pos + n > length || n == npos) {
			n = length - pos;
		}
	}

	AbstractString::pointer AbstractString::baseAssign(size_type n) {
		StoragePair x;
		openStorage(x, n
#ifdef DEV_BUILD
				, __FILE__, __LINE__
#endif
			);
		x.newStorage[n] = 0;
		return x.newStorage;
	}

	AbstractString::pointer AbstractString::baseAppend(size_type n) {
		StoragePair x;
		openStorage(x, length() + n
#ifdef DEV_BUILD
				, __FILE__, __LINE__
#endif
			);
		if (x.oldStorage) {
			memcpy(x.newStorage, x.oldStorage, x.oldSize);
		}
		x.newStorage[length()] = 0;
		return &x.newStorage[x.oldSize];
	}

	AbstractString::pointer AbstractString::baseInsert(size_type p0, size_type n) {
		if (p0 >= length()) {
			return baseAppend(n);
		}
		StoragePair x;
		openStorage(x, length() + n
#ifdef DEV_BUILD
				, __FILE__, __LINE__
#endif
			);
		if (x.oldStorage) {
			memcpy(x.newStorage, x.oldStorage, p0);
			memcpy(&x.newStorage[p0 + n], 
					&x.oldStorage[p0], x.oldSize - p0);
		}
		else {
			memmove(&x.newStorage[p0 + n], &x.newStorage[p0], 
				x.oldSize - p0);
		}
		x.newStorage[length()] = 0;
		return &x.newStorage[p0];
	}

	void AbstractString::baseErase(size_type p0, size_type n) {
		AdjustRange(length(), p0, n);
		StoragePair x;
		openStorage(x, length() - n
#ifdef DEV_BUILD
				, __FILE__, __LINE__
#endif
			);
		if (x.oldStorage) {
			memcpy(x.newStorage, x.oldStorage, p0);
			memcpy(&x.newStorage[p0], &x.oldStorage[p0 + n], 
				x.oldSize - (p0 + n));
		}
		else {
			memmove(&x.newStorage[p0], 
				&x.newStorage[p0 + n], x.oldSize - (p0 + n));
		}
		x.newStorage[length()] = 0;
	}

/*	void AbstractString::reserve(size_type n) {
		if (n <= actualSize) {
			return;
		}
		const unsigned short l = userSize;
		StoragePair x;
		openStorage(x, n
#ifdef DEV_BUILD
				, __FILE__, __LINE__
#endif
		);
		userSize = l;
		if (actualSize > smallStorageSize) {
			forced = n;
		}
		if (x.oldStorage) {
			memcpy(x.newStorage, x.oldStorage, l);
		}
		x.newStorage[l] = 0;
	}*/

	void AbstractString::resize(size_type n, char_type c) {
		if (n == length()) {
			return;
		}
		StoragePair x;
		openStorage(x, n
#ifdef DEV_BUILD
				, __FILE__, __LINE__
#endif
			);
		if (x.oldStorage) {
			memcpy(x.newStorage, x.oldStorage, n < x.oldSize ? n : x.oldSize);
		}
		if (n > x.oldSize) {
			memset(&x.newStorage[x.oldSize], c, n - x.oldSize);
		}
		x.newStorage[n] = 0;
	}

	AbstractString::size_type AbstractString::rfind(const_pointer s, size_type pos) const {
		const size_type l = strlen(s);
		int lastpos = length() - l;
		if (lastpos < 0) {
			return npos;
		}
		if (pos < static_cast<size_type>(lastpos)) {
			lastpos = pos;
		}
		const_pointer start = c_str();
		const_pointer end = &start[lastpos];
		while (end >= start) {
			if (memcmp(end, s, l) == 0) {
				return end - start;
			}
			--end;
		}
		return npos;
	}

	AbstractString::size_type AbstractString::rfind(char_type c, size_type pos) const {
		int lastpos = length() - 1;
		if (lastpos < 0) {
			return npos;
		}
		if (pos < static_cast<size_type>(lastpos)) {
			lastpos = pos;
		}
		const_pointer start = c_str();
		const_pointer end = &start[lastpos];
		while (end >= start) {
			if (*end == c) {
				return end - start;
			}
			--end;
		}
		return npos;
	}

	AbstractString::size_type AbstractString::find_first_of(const_pointer s, size_type pos, size_type n) const {
		strBitMask sm(s, n);
		const_pointer p = &c_str()[pos];
		while (pos < length()) {
			if (sm.Contains(*p++)) {
				return pos;
			}
			++pos;
		}
		return npos;
	}

	AbstractString::size_type AbstractString::find_last_of(const_pointer s, size_type pos, size_type n) const {
		strBitMask sm(s, n);
		int lpos = length() - 1;
		if (static_cast<int>(pos) < lpos) {
			lpos = pos;
		}
		const_pointer p = &c_str()[lpos];
		while (lpos >= 0) {
			if (sm.Contains(*p--)) {
				return lpos;
			}
			--lpos;
		}
		return npos;
	}

	AbstractString::size_type AbstractString::find_first_not_of(const_pointer s, size_type pos, size_type n) const {
		strBitMask sm(s, n);
		const_pointer p = &c_str()[pos];
		while (pos < length()) {
			if (! sm.Contains(*p++)) {
				return pos;
			}
			++pos;
		}
		return npos;
	}

	AbstractString::size_type AbstractString::find_last_not_of(const_pointer s, size_type pos, size_type n) const {
		strBitMask sm(s, n);
		int lpos = length() - 1;
		if (static_cast<int>(pos) < lpos) {
			lpos = pos;
		}
		const_pointer p = &c_str()[lpos];
		while (lpos >= 0) {
			if (! sm.Contains(*p--)) {
				return lpos;
			}
			--lpos;
		}
		return npos;
	}

	bool AbstractString::LoadFromFile(FILE *file) {
		baseErase(0, length());
		if (! file)
			return false;
		bool rc = false;
		int c;
		while ((c = getc(file)) != EOF) {
			rc = true;
			if (c == '\n') {
				break;
			}
			*baseAppend(1) = c;
		}
		return rc;
	}

	void AbstractString::upper() {
		for(pointer p = Modify(); *p; p++) {
			*p = toupper(*p);
		}
	}

	void AbstractString::lower() {
		for(pointer p = Modify(); *p; p++) {
			*p = tolower(*p);
		}
	}

	void AbstractString::baseTrim(TrimType WhereTrim, const_pointer ToTrim) {
		strBitMask sm(ToTrim, strlen(ToTrim));
		const_pointer b = c_str();
		const_pointer e = &c_str()[length() - 1];
		if (WhereTrim != TrimRight) {
			while (b <= e) {
				if (! sm.Contains(*b)) {
					break;
				}
				++b;
			}
		}
		if (WhereTrim != TrimLeft) {
			while (b <= e) {
				if (! sm.Contains(*e)) {
					break;
				}
				--e;
			}
		}
		size_type NewLength = e - b + 1;

		if (NewLength == length())
			return;
		StoragePair x;
		openStorage(x, NewLength
#ifdef DEV_BUILD
				, __FILE__, __LINE__
#endif
			);
		if (x.oldStorage) {
			memcpy(x.newStorage, b, NewLength);
		}
		else if (b != x.newStorage) {
			memmove(x.newStorage, b, NewLength);
		}
		x.newStorage[NewLength] = 0;
	}

	void AbstractString::printf(const char* format,...) {
		enum {tempsize = 256};
		char temp[tempsize];
		va_list params;
		va_start(params, format);
		int l = VSNPRINTF(temp, tempsize, format, params);
		if (l < 0) {
			int n = sizeof(temp);
			do {
				n *= 2;
				l = VSNPRINTF(baseAssign(n), n + 1, format, params);
				if (l > 16 * 1024) {
					char *errLine = "String size overflow in .printf()";
					memcpy(baseAssign(strlen(errLine)), errLine, strlen(errLine));
					return;
				}
			} while (l < 0);
			resize(l);
			return;	
		}
		temp[tempsize - 1] = 0;
		if (l < tempsize) {
			memcpy(baseAssign(l), temp, l);
		}
		else {
			resize(l);
			VSNPRINTF(begin(), l + 1, format, params);
		}
		va_end(params);
	}

	int PathNameComparator::compare(AbstractString::const_pointer s1, AbstractString::const_pointer s2, AbstractString::size_type n) {
		if (CASE_SENSITIVITY)
			return memcmp(s1, s2, n);
		return STRNCASECMP(s1, s2, n);
	} 
}	// namespace Firebird
