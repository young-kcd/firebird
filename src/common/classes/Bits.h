/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		Bits.h
 *	DESCRIPTION:	Arbitrary size bitmask
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
 *  Copyright (c) 2016, 2022 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 */

#ifndef COMMON_CLASSES_BITS_H
#define COMMON_CLASSES_BITS_H

namespace Firebird {

	// Arbitrary size bitmask
	template <unsigned N>
	class Bits
	{
		static const unsigned shift = 3;
		static const unsigned bitmask = (1 << shift) - 1;

		static const unsigned L = (N >> shift) + (N & bitmask ? 1 : 0);

	public:
		static const unsigned BYTES_COUNT = L;

		Bits()
		{
			clearAll();
		}

		Bits(const Bits& b)
		{
			assign(b);
		}

		Bits& operator=(const Bits& b)
		{
			assign(b);
			return *this;
		}

		Bits& set(unsigned i)
		{
			fb_assert(i < N);
			if (i < N)
				data[index(i)] |= mask(i);
			return *this;
		}

		Bits& setAll()
		{
			memset(data, ~0, sizeof data);
			return *this;
		}

		Bits& clear(unsigned i)
		{
			fb_assert(i < N);
			if (i < N)
				data[index(i)] &= ~mask(i);
			return *this;
		}

		Bits& clearAll()
		{
			memset(data, 0, sizeof data);
			return *this;
		}

		bool test(unsigned int i) const
		{
			fb_assert(i < N);
			if (i >= N)
				return false;
			return data[index(i)] & mask(i);
		}

		void load(const void* from)
		{
			memcpy(data, from, sizeof data);
		}

		void store(void* to) const
		{
			memcpy(to, data, sizeof data);
		}

		Bits& operator|=(const Bits& b)
		{
			for (unsigned n = 0; n < L; ++n)
				data[n] |= b.data[n];
			return *this;
		}

	private:
		UCHAR data[L];

		void assign(const Bits& b)
		{
			memcpy(data, b.data, sizeof data);
		}

		static unsigned index(unsigned i)
		{
			return i >> shift;
		}

		static UCHAR mask(unsigned i)
		{
			return 1U << (i & bitmask);
		}
	};

} // namespace Firebird

#endif // COMMON_CLASSES_BITS_H

