/*
 *	PROGRAM:		Client/Server Common Code
 *	MODULE:			fb_atomic.h
 *	DESCRIPTION:	Atomic counters
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
 */

#ifndef CLASSES_FB_ATOMIC_H
#define CLASSES_FB_ATOMIC_H

// NS 2014-08-13. I implemented this module using libatomic_ops for the sole reason that
// we need to support older compilers (e.g. MSVC10). C++11 sequentially consistent atomics 
// are the proper way to go forward, and we shall migrate code to use them as the (only)
// implementation as soon we require C++11 compiler for Firebird build.

extern "C" {
#define AO_ASSUME_WINDOWS98
#define AO_USE_INTERLOCKED_INTRINSICS
#define AO_REQUIRE_CAS
#include <atomic_ops.h>
}

namespace Firebird {

// [1] Load-acquire, store-release semantics

template <typename T_PTR>
inline T_PTR atomic_ptr_load_acquire(const volatile T_PTR* location) {
	static_assert(sizeof(T_PTR) == sizeof(AO_t), "Incorrect argument size");
	return reinterpret_cast<T_PTR>(AO_load_acquire(reinterpret_cast<const volatile AO_t*>(location)));
}

template <typename T_PTR>
inline void atomic_ptr_store_release(volatile T_PTR* location, T_PTR value) {
	static_assert(sizeof(T_PTR) == sizeof(AO_t), "Incorrect argument size");
	AO_store_release(reinterpret_cast<volatile AO_t*>(location), reinterpret_cast<AO_t>(value));
}

template <typename INT32>
inline INT32 atomic_int_load_acquire(const volatile INT32* location) {
	static_assert(sizeof(INT32) == sizeof(unsigned), "Incorrect argument size");
	return static_cast<INT32>(AO_int_load_acquire(reinterpret_cast<const volatile unsigned*>(location)));
}

template <typename INT32>
inline void atomic_int_store_release(volatile INT32* location, INT32 value) {
	static_assert(sizeof(INT32) == sizeof(unsigned), "Incorrect argument size");
	AO_int_store_release(reinterpret_cast<volatile unsigned*>(location), static_cast<unsigned>(value));
}

// [2] Atomic increment with and without barrier (use when you know what you are doing)

template <typename INT32>
inline INT32 atomic_int_fetch_and_add1(volatile INT32* location) {
	static_assert(sizeof(INT32) == sizeof(unsigned), "Incorrect argument size");
	return static_cast<INT32>(AO_int_fetch_and_add1(reinterpret_cast<volatile unsigned*>(location)));
}

template <typename INT32>
inline INT32 atomic_int_fetch_and_add1_full(volatile INT32* location) {
	static_assert(sizeof(INT32) == sizeof(unsigned), "Incorrect argument size");
	return static_cast<INT32>(AO_int_fetch_and_add1_full(reinterpret_cast<volatile unsigned*>(location)));
}


// [3] Atomic operations with no barrier (use when you know what you are doing)

template <typename INT32>
inline INT32 atomic_int_load(const volatile INT32* location) {
	static_assert(sizeof(INT32) == sizeof(unsigned), "Incorrect argument size");
	return static_cast<INT32>(AO_int_load(reinterpret_cast<const volatile unsigned*>(location)));
}

template <typename INT32>
inline void atomic_int_store(volatile INT32* location, INT32 value) {
	static_assert(sizeof(INT32) == sizeof(unsigned), "Incorrect argument size");
	AO_int_store(reinterpret_cast<volatile unsigned*>(location), static_cast<unsigned>(value));
}

// [4] Compiler barrier to protect against code re-ordering (protects logic in the presence of signals)

inline void compiler_barrier() {
	AO_compiler_barrier();
}

class AtomicCounter
{
public:
	typedef IPTR counter_type;

	explicit AtomicCounter(counter_type value = 0)
		: counter(value)
	{
		static_assert(sizeof(counter_type) == sizeof(counter), "Internal and external counter sizes need to match");
	}

	~AtomicCounter()
	{
	}

	counter_type value() const { return AO_load_acquire(&counter); }

	counter_type exchangeAdd(counter_type value)
	{
		return AO_fetch_and_add_full(&counter, value);
	}

	void setValue(counter_type val) {
		AO_store_release(&counter, val);
	}

	bool compareExchange(counter_type oldVal, counter_type newVal)
	{
		return AO_compare_and_swap_full(&counter, oldVal, newVal);
	}

	// returns old value
	counter_type exchangeBitAnd(counter_type val)
	{
		while (true)
		{
			counter_type oldVal = AO_load(&counter);
			if (AO_compare_and_swap_full(&counter, oldVal, oldVal & val))
				return oldVal;
		}
	}

	// returns old value
	counter_type exchangeBitOr(counter_type val)
	{
		while (true)
		{
			counter_type oldVal = AO_load(&counter);
			if (AO_compare_and_swap_full(&counter, oldVal, oldVal | val))
				return oldVal;
		}
	}

	// returns old value
	counter_type exchangeGreater(counter_type val)
	{
		while (true)
		{
			counter_type oldVal = AO_load_acquire(&counter);

			if (oldVal >= val)
				return oldVal;

			if (AO_compare_and_swap_full(&counter, oldVal, val))
				return oldVal;
		}
	}

	// returns old value
	counter_type exchangeLower(counter_type val)
	{
		while (true)
		{
			counter_type oldVal = AO_load_acquire(&counter);

			if (oldVal <= val)
				return oldVal;

			if (AO_compare_and_swap_full(&counter, oldVal, val))
				return oldVal;
		}
	}

	void operator &=(counter_type val)
	{
		AO_and_full(&counter, val);
	}

	void operator |=(counter_type val)
	{
		AO_or_full(&counter, val);
	}

	// returns new value !
	counter_type operator ++()
	{
		return AO_fetch_and_add1_full(&counter) + 1;
	}

	// returns new value !
	counter_type operator --()
	{
		return AO_fetch_and_sub1_full(&counter) - 1;
	}

	inline operator counter_type () const
	{
		return value();
	}

	inline void operator =(counter_type val)
	{
		setValue(val);
	}

	inline counter_type operator +=(counter_type val)
	{
		return exchangeAdd(val) + val;
	}

	inline counter_type operator -=(counter_type val)
	{
		return exchangeAdd(-val) - val;
	}

private:
	volatile AO_t counter;
};

class PlatformAtomicPointer
{
public:
	explicit PlatformAtomicPointer(void* val = NULL)
		: pointer(reinterpret_cast<AO_t>(val))
	{
		static_assert(sizeof(void*) == sizeof(AO_t), "AO_t size needs to match pointer size");
	}

	~PlatformAtomicPointer()
	{
	}

	void* platformValue() const
	{
		return reinterpret_cast<void*>(AO_load_acquire(&pointer));
	}

	void platformSetValue(void* val) {
		AO_store_release(&pointer, reinterpret_cast<AO_t>(val));
	}

	bool platformCompareExchange(void* oldVal, void* newVal)
	{
		return AO_compare_and_swap_full(&pointer, reinterpret_cast<AO_t>(oldVal), reinterpret_cast<AO_t>(newVal));
	}

private:
	volatile AO_t pointer;
};


// NS 2014-08-01 FIXME: This interface is ugly and cannot be used correctly.
// The users end up treating this class as struct, without calling class destructor.
// Implementation is also sub-optimal - full barriers are used everywhere 
// while more relaxed semantics is possible. Atomic pointers shall be handled 
// via inline template functions atomic_ptr_<op>_<barrier>
template <typename T>
class AtomicPointer : public PlatformAtomicPointer
{
public:
	explicit AtomicPointer(T* val = NULL) : PlatformAtomicPointer(val) {}
	~AtomicPointer() {}

	T* value() const
	{
		return (T*)platformValue();
	}

	void setValue(T* val)
	{
		platformSetValue(val);
	}

	bool compareExchange(T* oldVal, T* newVal)
	{
		return platformCompareExchange(oldVal, newVal);
	}

	operator T* () const
	{
		return value();
	}

	T* operator ->() const
	{
		return value();
	}

	void operator =(T* val)
	{
		setValue(val);
	}
};

// NS 2014-08-01: FIXME. Atomic counters use barriers on all platforms, so 
// these operations are no-ops and will always be no-ops. They were used 
// to work around (incorrectly) bugs in Sparc and PPC atomics code. 
inline void FlushCache() { }
inline void WaitForFlushCache() { }

} // namespace Firebird

#endif // CLASSES_FB_ATOMIC_H
