/*
 *	PROGRAM:		Client/Server Common Code
 *	MODULE:			fb_tls.h
 *	DESCRIPTION:	Thread-local storage handlers
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
 *  $Id: fb_tls.h,v 1.4 2004-05-02 23:03:47 skidder Exp $
 *
 */
 
#ifndef CLASSES_FB_TLS_H
#define CLASSES_FB_TLS_H

// This unit defines a few handy macros
// TLS_DECLARE is used in place of variable declaration
// TLS_GET gives value of thread-specific variable
// TLS_SET sets value of thread-specific variable
//
// TLS variable type should be smaller than size of pointer to stay portable

#if !defined(MULTI_THREAD)
// Single-threaded case
# define TLS_DECLARE(TYPE, NAME) TYPE NAME
# define TLS_GET(NAME) NAME
# define TLS_SET(NAME,VALUE) NAME=(VALUE)
#elif defined(HAVE___THREAD)
// Recent GCC supports __thread keyword. Sun compiler and HP-UX should have it too
# define TLS_DECLARE(TYPE, NAME) __thread TYPE NAME
# define TLS_GET(NAME) NAME
# define TLS_SET(NAME,VALUE) NAME=(VALUE)
#elif defined(WIN_NT)

#ifdef __GNUC__
// this is gcc, but it does not support __thread, as checked above
namespace Firebird {

template <typename T>
class Win32Tls {
public:
	Win32Tls() {
		if ((key = TlsAlloc()) == 0xFFFFFFFF)
			system_call_failed::raise("TlsAlloc");
	}
	const T get() {
	 	LPVOID value = TlsGetValue(key);
		if ((value == NULL) && (GetLastError() != NO_ERROR))
			system_call_failed::raise("TlsGetValue");
		return reinterpret_cast<T>(value);
	}
	void set(const T value) {
		if (TlsSetValue(key, value) == 0)
			system_call_failed::raise("TlsSetValue");
	}
	~Win32Tls() {
		if (TlsFree(key) == 0)
			system_call_failed::raise("TlsFree");
	}
private:
	DWORD key;	
};
} // namespace Firebird
# define TLS_DECLARE(TYPE, NAME) Firebird::Win32Tls<TYPE> NAME
# define TLS_GET(NAME) NAME.get()
# define TLS_SET(NAME,VALUE) NAME.set(VALUE)

#else // MSVC or Intel
# define TLS_DECLARE(TYPE, NAME) __declspec(thread) TYPE NAME
# define TLS_GET(NAME) NAME
# define TLS_SET(NAME,VALUE) NAME=(VALUE)
#endif
#else

#include <pthread.h>

namespace Firebird {


template <typename T>
class TlsValue {
public:
	TlsValue() {
		if (pthread_key_create(&key, NULL))
			system_call_failed::raise("pthread_key_create");
	}
	const T get() {
		// We use double C-style cast to allow using scalar datatypes
		// with sizes up to size of pointer without warnings
		return (T)(IPTR)pthread_getspecific(key);
	}
	void set(const T value) {
		if (pthread_setspecific(key, (void*)(IPTR)value))
			system_call_failed::raise("pthread_setspecific");
	}
	~TlsValue() {
		if (pthread_key_delete(key))
			system_call_failed::raise("pthread_key_delete");
	}
private:
	pthread_key_t key;	
};

} // namespace Firebird

# define TLS_DECLARE(TYPE, NAME) Firebird::TlsValue<TYPE> NAME
# define TLS_GET(NAME) NAME.get()
# define TLS_SET(NAME,VALUE) NAME.set(VALUE)

#endif

#endif // CLASSES_FB_TLS_H
