/*
 *	PROGRAM:		Client/Server Common Code
 *	MODULE:			fb_tls.h
 *	DESCRIPTION:	Thread-local storage handlers
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
 *  $Id: fb_tls.h,v 1.10 2004-11-24 09:05:12 robocop Exp $
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
# define TLS_SET(NAME, VALUE) NAME=(VALUE)
#elif defined(HAVE___THREAD)
// Recent GCC supports __thread keyword. Sun compiler and HP-UX should have it too
# define TLS_DECLARE(TYPE, NAME) __thread TYPE NAME
# define TLS_GET(NAME) NAME
# define TLS_SET(NAME, VALUE) NAME=(VALUE)
#elif defined(WIN_NT)

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
//		return reinterpret_cast<T>(value);
		return (T)value;
	}
	void set(const T value) {
		if (TlsSetValue(key, (LPVOID)value) == 0)
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
# define TLS_DECLARE(TYPE, NAME) ::Firebird::Win32Tls<TYPE> NAME
# define TLS_GET(NAME) NAME.get()
# define TLS_SET(NAME, VALUE) NAME.set(VALUE)

// 14-Jul-2004 Nickolay Samofatov.
//
// Unfortunately, compiler-assisted TLS doesn't work with dynamic link libraries
// loaded via LoadLibrary - it intermittently crashes and these crashes are 
// documented by MS. We may still use it for server binaries, but it requires
// some changes in build environment. Let's defer this till later point and 
// think of reliable mean to prevent linking of DLL with code below (if enabled).
//
//# define TLS_DECLARE(TYPE, NAME) __declspec(thread) TYPE NAME
//# define TLS_GET(NAME) NAME
//# define TLS_SET(NAME, VALUE) NAME=(VALUE)
#else

#if not (defined SOLARIS)

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
#else //SOLARIS
#include <thread.h>
#include <string.h>

namespace Firebird {


template <typename T>
class TlsValue {
public:
	static void  TlsV_on_thread_exit (void * pval) {
	/* Usually should delete pval like this
		T * ptempT= (T*) pval ;
		delete ptempT;
	*/

	}

	TlsValue() {
		if (thr_keycreate(&key, TlsV_on_thread_exit) )
			system_call_failed::raise("thr_key_create");
	}
	const T get() {
		// We use double C-style cast to allow using scalar datatypes
		// with sizes up to size of pointer without warnings
		T  * valuep;
		if (thr_getspecific(key, (void **) &valuep) == 0)
			return (T)(IPTR) (valuep) ;
		else
			system_call_failed::raise("thr_getspecific");
			return (T)NULL;
	}
	void set(const T value) {
		if (thr_setspecific(key, (void*)(IPTR)value))
			system_call_failed::raise("thr_setspecific");
	}
	~TlsValue() {
		/* Do nothing if no pthread_key_delete */
	}
private:
	thread_key_t key;	
};


#endif //SOLARIS

} // namespace Firebird

# define TLS_DECLARE(TYPE, NAME) ::Firebird::TlsValue<TYPE> NAME
# define TLS_GET(NAME) NAME.get()
# define TLS_SET(NAME, VALUE) NAME.set(VALUE)

#endif

#endif // CLASSES_FB_TLS_H
