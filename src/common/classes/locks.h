/*
 *	PROGRAM:		Client/Server Common Code
 *	MODULE:			locks.h
 *	DESCRIPTION:	Single-state locks
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
 *  $Id: locks.h,v 1.14 2004-05-20 23:04:30 skidder Exp $
 *
 */

#ifndef CLASSES_LOCKS_H
#define CLASSES_LOCKS_H

#include "firebird.h"

#ifdef MULTI_THREAD
#ifdef WIN_NT
// It is relatively easy to avoid using this header. Maybe do the same stuff like
// in thd.h ? This is Windows platform maintainers choice
#include <windows.h>
#else
#ifndef SOLARIS
#include <pthread.h>
#else
#include <sys/mutex.h>
#include <thread.h>
#endif
#endif
#endif /* MULTI_THREAD */

namespace Firebird {

#ifdef MULTI_THREAD
#ifdef WIN_NT

/* Process-local spinlock. Used to manage memory heaps in threaded environment. */
// Windows version of the class

typedef WINBASEAPI DWORD WINAPI tSetCriticalSectionSpinCount (
	LPCRITICAL_SECTION lpCriticalSection,
	DWORD dwSpinCount
);

class Mutex {
private:
	CRITICAL_SECTION spinlock;
	static tSetCriticalSectionSpinCount* SetCriticalSectionSpinCount;
public:
	Mutex();
	~Mutex() {
		DeleteCriticalSection(&spinlock);
	}
	void enter() {
		EnterCriticalSection(&spinlock);
	}
	void leave() {
		LeaveCriticalSection(&spinlock);
	}
};

#else

/* Process-local spinlock. Used to manage memory heaps in threaded environment. */
// Pthreads version of the class
#if !defined(SOLARIS) && !defined(DARWIN) && !defined(FREEBSD) && !defined(NETBSD)
class Mutex {
private:
	pthread_spinlock_t spinlock;
public:
	Mutex() {
		if (pthread_spin_init(&spinlock, false))
			system_call_failed::raise("pthread_spin_init");
	}
	~Mutex() {
		if (pthread_spin_destroy(&spinlock))
			system_call_failed::raise("pthread_spin_destroy");
	}
	void enter() {
		if (pthread_spin_lock(&spinlock))
			system_call_failed::raise("pthread_spin_lock");
	}
	void leave() {
		if (pthread_spin_unlock(&spinlock))
			system_call_failed::raise("pthread_spin_unlock");
	}
};
#else
#ifdef SOLARIS
// Who knows why Solaris 2.6 have not THIS funny spins?
//The next code is not comlpeted but let me compile //Konstantin
class Mutex {
private:
	mutex_t spinlock;
public:
	Mutex() {
		if (mutex_init(&spinlock, MUTEX_SPIN, NULL))
			system_call_failed::raise("mutex_init");
	}
	~Mutex() {
		if (mutex_destroy(&spinlock))
			system_call_failed::raise("mutex_destroy");
	}
	void enter() {
		if (mutex_lock(&spinlock))
			system_call_failed::raise("mutex_lock");
	}
	void leave() {
		if (mutex_unlock(&spinlock))
			system_call_failed::raise("mutex_unlock");
	}
};
#else  // DARWIN and FREEBSD and NETBSD
class Mutex {
private:
	pthread_mutex_t mlock;
public:
	Mutex() {
		if (pthread_mutex_init(&mlock, 0))
			system_call_failed::raise("pthread_mutex_init");
	}
	~Mutex() {
		if (pthread_mutex_destroy(&mlock))
			system_call_failed::raise("pthread_mutex_destroy");
	}
	void enter() {
		if (pthread_mutex_lock(&mlock))
			system_call_failed::raise("pthread_mutex_lock");
	}
	void leave() {
		if (pthread_mutex_unlock(&mlock))
			system_call_failed::raise("pthread_mutex_unlock");
	}
};
#endif

#endif
#endif
#else
// Non-MT version
class Mutex {
public:
	Mutex() {
	}
	~Mutex() {
	}
	void enter() {
	}
	void leave() {
	}
};

#endif /* MULTI_THREAD */

// RAII holder of mutex lock
class MutexLockGuard {
public:
	MutexLockGuard(Mutex &alock) : lock(&alock) { lock->enter(); }
	~MutexLockGuard() { lock->leave(); };
private:
	// Forbid copy constructor
	MutexLockGuard(const MutexLockGuard& source);
	Mutex *lock;
};

} //namespace Firebird

#endif // CLASSES_LOCKS_H
