/*
 *	PROGRAM:		Client/Server Common Code
 *	MODULE:			locks.h
 *	DESCRIPTION:	Single-state locks
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
 * Created by: Nickolay Samofatov <skidder@bssys.com>
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#ifndef LOCKS_H
#define LOCKS_H

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

class Spinlock {
private:
	CRITICAL_SECTION spinlock;
	static tSetCriticalSectionSpinCount* SetCriticalSectionSpinCount;
public:
	Spinlock();
	~Spinlock() {
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
#if !defined(SOLARIS) && !defined(DARWIN) && !defined(FREEBSD)
class Spinlock {
private:
	pthread_spinlock_t spinlock;
public:
	Spinlock() {
		if (pthread_spin_init(&spinlock, false))
			system_call_failed::raise();
	}
	~Spinlock() {
		if (pthread_spin_destroy(&spinlock))
			system_call_failed::raise();
	}
	void enter() {
		if (pthread_spin_lock(&spinlock))
			system_call_failed::raise();
	}
	void leave() {
		if (pthread_spin_unlock(&spinlock))
			system_call_failed::raise();
	}
};
#else
#ifdef SOLARIS
// Who knows why Solaris 2.6 have not THIS funny spins?
//The next code is not comlpeted but let me compile //Konstantin
class Spinlock {
private:
	mutex_t spinlock;
public:
	Spinlock() {
		if (mutex_init(&spinlock, MUTEX_SPIN, NULL))
			system_call_failed::raise();
	}
	~Spinlock() {
		if (mutex_destroy(&spinlock))
			system_call_failed::raise();
	}
	void enter() {
		if (mutex_lock(&spinlock))
			system_call_failed::raise();
	}
	void leave() {
		if (mutex_unlock(&spinlock))
			system_call_failed::raise();
	}
};
#else  // DARWIN and FREEBSD
class Spinlock {
private:
	pthread_mutex_t mlock;
public:
	Spinlock() {
		if (pthread_mutex_init(&mlock, 0))
			system_call_failed::raise();
	}
	~Spinlock() {
		if (pthread_mutex_destroy(&mlock))
			system_call_failed::raise();
	}
	void enter() {
		if (pthread_mutex_lock(&mlock))
			system_call_failed::raise();
	}
	void leave() {
		if (pthread_mutex_unlock(&mlock))
			system_call_failed::raise();
	}
};
#endif

#endif
#endif
#endif /* MULTI_THREAD */

// Spinlock in shared memory. Not implemented yet !
class SharedSpinlock {
public:
	SharedSpinlock() {
	}
	~SharedSpinlock() {
	}
	void enter() {
	}
	void leave() {
	}
};

}

#endif
