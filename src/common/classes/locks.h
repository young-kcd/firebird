#ifndef LOCKS_H
#define LOCKS_H

#include "firebird.h"

#ifdef SUPERSERVER
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
#endif /* SUPERSERVER */

namespace Firebird {

#ifdef SUPERSERVER
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
#ifndef SOLARIS
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

#endif
#endif
#endif /* SUPERSERVER */

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
