/*
 *	PROGRAM:		Client/Server Common Code
 *	MODULE:			semaphore.h
 *	DESCRIPTION:	Semaphore lock
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
 */

#ifndef CLASSES_SEMAPHORE_H
#define CLASSES_SEMAPHORE_H

#include "../jrd/gdsassert.h"
#ifdef WIN_NT
// Note: Windows does not need signal safe version of the class

#include <windows.h>
#include <limits.h>

namespace Firebird
{

class Semaphore
{
private:
	HANDLE hSemaphore;
public:
	Semaphore()
	{ 
		hSemaphore = CreateSemaphore(NULL, 0 /*initial count*/, INT_MAX, NULL);
		if (hSemaphore == NULL)
			system_call_failed::raise("CreateSemaphore");
	}
	~Semaphore()
	{
		if (hSemaphore && !CloseHandle(hSemaphore))
			system_call_failed::raise("CloseHandle");
	}	

	bool tryEnter(int seconds = 0)
	{
		DWORD result = WaitForSingleObject(
			hSemaphore, seconds >= 0 ? seconds * 1000 : INFINITE);
		if (result == WAIT_FAILED)
			system_call_failed::raise("WaitForSingleObject");
		return result != WAIT_TIMEOUT;
	}

	void release(SLONG count = 1)
	{
		if (!ReleaseSemaphore(hSemaphore, count, NULL))
			system_call_failed::raise("ReleaseSemaphore");
	}
};

} // namespace Firebird

#else //WIN_NT

#ifdef MULTI_THREAD

#ifdef HAVE_SEMAPHORE_H

#include <semaphore.h>
#include <errno.h>
#include <time.h>
#ifndef WORKING_SEM_INIT
#include <fcntl.h>
#if defined(DARWIN)
#ifdef SUPERSERVER
#define MIXED_SEMAPHORE_AND_FILE_HANDLE
#endif
#endif
#endif // WORKING_SEM_INIT

namespace Firebird
{
#ifndef WORKING_SEM_INIT
static const char* semName = "/firebird_temp_sem";
#endif

class SignalSafeSemaphore
{
private:
#ifdef WORKING_SEM_INIT
	sem_t sem[1];
#else
	sem_t* sem;
#ifdef MIXED_SEMAPHORE_AND_FILE_HANDLE
	static bool divorceDone;
	static SignalSafeSemaphore* initialList;
	void linkToInitialList();
	SignalSafeSemaphore* next;
#endif
#endif // WORKING_SEM_INIT
	bool  init;
public:
#ifdef MIXED_SEMAPHORE_AND_FILE_HANDLE
	static bool checkHandle(int n);
#endif
	SignalSafeSemaphore()
		: init(false)
	{
#ifdef WORKING_SEM_INIT
		if (sem_init(sem, 0, 0) == -1) {
			system_call_failed::raise("sem_init");
		}
#else
		sem = sem_open(semName, O_CREAT | O_EXCL, 0700, 0);

#ifdef HPUX
#define SEM_FAILED ((sem_t*) (-1))
#endif

#if defined(DARWIN) && !defined(DARWIN64) || defined(HPUX)
		if (sem == (sem_t*) SEM_FAILED) {
#else
		if (sem == SEM_FAILED) {
#endif
			system_call_failed::raise("sem_open");
		}
		sem_unlink(semName);
#ifdef MIXED_SEMAPHORE_AND_FILE_HANDLE
		linkToInitialList();
#endif
#endif
		init = true;
	}
	
	~SignalSafeSemaphore()
	{
		fb_assert(init == true);
#ifdef WORKING_SEM_INIT
		if (sem_destroy(sem) == -1) {
			system_call_failed::raise("sem_destroy");
		}
#else
		if (sem_close(sem) == -1) {
			system_call_failed::raise("sem_close");
		}
#endif
		init = false;

	}
	
	void enter()
	{
		fb_assert(init == true);
		do {
			if (sem_wait(sem) != -1)
				return;
		} while (errno == EINTR);
		system_call_failed::raise("semaphore.h: enter: sem_wait()");
	}
	
	void release(SLONG count = 1)
	{
		fb_assert(init == true);
		for (int i = 0; i < count; i++)
		{
			if (sem_post(sem) == -1)
			{
				system_call_failed::raise("semaphore.h: release: sem_post()");
			}
		}
	}

#ifdef HAVE_SEM_TIMEDWAIT
	// In case when sem_timedwait() is implemented by host OS, 
	// class SignalSafeSemaphore may have this function:
	bool tryEnter(int seconds = 0)
	{
		// Return true in case of success
		fb_assert(init == true);
		if (seconds == 0)
		{
			// Instant try
			do {
				if (sem_trywait(sem) != -1) 
					return true;
			} while (errno == EINTR);
			if (errno == EAGAIN) 
				return false;
			system_call_failed::raise("sem_trywait");
		}
		if (seconds < 0)
		{
			// Unlimited wait, like enter()
			do {
				if (sem_wait(sem) != -1)
					return true;
			} while (errno == EINTR);
			system_call_failed::raise("sem_wait");
		}
		// Wait with timeout
		struct timespec timeout;
		timeout.tv_sec = time(NULL) + seconds;
		timeout.tv_nsec = 0;
		int errcode = 0;
		do {
			int rc = sem_timedwait(sem, &timeout);
			if (rc == 0) 
				return true;
			// fix for CORE-988, also please see 
			// http://carcino.gen.nz/tech/linux/glibc_sem_timedwait_errors.php
			errcode = rc > 0 ? rc : errno;
		} while (errcode == EINTR);
		if (errcode == ETIMEDOUT) {
			return false;
		}
		system_call_failed::raise("sem_timedwait", errcode);
		return false;	// avoid warnings
	}
#endif //HAVE_SEM_TIMEDWAIT
};

#ifdef HAVE_SEM_TIMEDWAIT
// In case when sem_timedwait() is implemented by host OS, 
// SignalSafeSemaphore and Semaphore are just the same
typedef SignalSafeSemaphore Semaphore;
#endif //HAVE_SEM_TIMEDWAIT

} // namespace Firebird

#endif //HAVE_SEMAPHORE_H

#ifndef HAVE_SEM_TIMEDWAIT
// Should implement Semaphore independent from SignalSafeSemaphore.
// In the worst case no SignalSafeSemaphore at all (and no SS for that platform).

#if defined(HAVE_SYS_SEM_H) && defined(HAVE_SEMTIMEDOP)

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

namespace Firebird
{

class Semaphore
{
private:
	int semId;
	union semun
	{
		int					val;
		struct semid_ds*	buf;
		unsigned short*		array;
	};
public:
	Semaphore()
		: semId(semget(IPC_PRIVATE, 1, 0600))
	{
		if (semId < 0)
			system_call_failed::raise("semaphore.h: Semaphore: semget()");
		semun arg;
		arg.val = 0;
		if (semctl(semId, 0, SETVAL, arg) < 0)
			system_call_failed::raise("semaphore.h: Semaphore: semctl()");
	}
	
	~Semaphore()
	{
		semun arg;
		if (semctl(semId, 0, IPC_RMID, arg) < 0)
			system_call_failed::raise("semaphore.h: ~Semaphore: semctl()");
	}
	
	bool tryEnter(int seconds = 0) // Returns true in case of success
	{
		timespec timeout;
		timeout.tv_sec = time(NULL) + seconds;
		timeout.tv_nsec = 0;
		timespec* t = &timeout;

		sembuf sb;
		sb.sem_num = 0;
		sb.sem_op = -1;
		sb.sem_flg = 0;

		if (seconds < 0) {
			// Unlimited wait
			t = 0;
		}
		else if (seconds == 0) {
			// just try
			t = 0;
			sb.sem_flg = IPC_NOWAIT;
		}

		while (semtimedop(semId, &sb, 1, t) < 0)
		{
			switch (errno)
			{
			case EAGAIN:
				return false;
			case EINTR:
				continue;
			}
			system_call_failed::raise("semaphore.h: tryEnter: semop()");
		}

		return true;
	}
	
	void enter()
	{
		tryEnter(-1);
	}
	
	void release(SLONG count = 1)
	{
		sembuf sb;
		sb.sem_num = 0;
		sb.sem_op = 1;
		sb.sem_flg = 0;

		while (semop(semId, &sb, 1) < 0)
		{
			if (errno == EINTR) {
				continue;
			}
			system_call_failed::raise("semaphore.h: release: semop()");
		}
	}
};

} // namespace Firebird

#else //defined(HAVE_SYS_SEM_H) && defined(HAVE_SEMTIMEDOP)

// This implementation will NOT work with FB > 2.1
#ifdef SOLARIS
#error Mutex/Condition based semaphore is NOT OK for Solaris
#endif

#include <pthread.h>
#include <errno.h>

namespace Firebird
{

class Semaphore
{
private:
	pthread_mutex_t	mu;
	pthread_cond_t	cv;
	bool			init;
public:
	Semaphore()
		: init(false)
	{
		int err = pthread_mutex_init(&mu, NULL);
		if (err != 0) {
			//gds__log("Error on semaphore.h: constructor");
			system_call_failed::raise("pthread_mutex_init", err);
		}
		err = pthread_cond_init(&cv, NULL);
		if (err != 0) {
			//gds__log("Error on semaphore.h: constructor");
			system_call_failed::raise("pthread_cond_init", err);
		}
		init = true;
	}
	
	~Semaphore()
	{
		fb_assert(init == true);
		int err = pthread_mutex_destroy(&mu);
		if (err != 0) {
			//gds__log("Error on semaphore.h: destructor");
			//system_call_failed::raise("pthread_mutex_destroy", err);
		}
		err = pthread_cond_destroy(&cv);
		if (err != 0) {
			//gds__log("Error on semaphore.h: destructor");
			//system_call_failed::raise("pthread_cond_destroy", err);
		}
		
		init = false;
	}
	
	bool tryEnter(int seconds = 0)
	{
		bool rt = false;
		int err2 = 0;
		int err = 0;
		// Return true in case of success
		fb_assert(init == true);
		if (seconds == 0)
		{
			// Instant try
			
			err2 = pthread_mutex_trylock(&mu);
			if (err2 == 0)
			{
				do {
					err = pthread_cond_wait(&cv, &mu);
					if (err != 0) {
						rt = false;
					}
					else
						rt = true;
				} while (err == EINTR);	
			    if (err == ETIMEDOUT)
					rt = false;

				pthread_mutex_unlock(&mu);
				return rt;
			}
			if (err2 == EBUSY)
				return false;

			system_call_failed::raise("pthread_mutex_trylock", err2);
		}

		if (seconds < 0)
		{
			// Unlimited wait, like enter()
			err2 = pthread_mutex_lock(&mu);
			if (err2 == 0)
			{
				do {
					err = pthread_cond_wait(&cv, &mu);
					if (err != 0) {
						rt = false;
					}
					else 
						rt = true;
				} while (err == EINTR);
				if (err == ETIMEDOUT)
					rt = false;

				pthread_mutex_unlock(&mu);
				return rt;
			}
			if (err2 == EBUSY)
				return false;

			system_call_failed::raise("pthread_mutex_lock", err2);
		} //seconds < 0 

		// Wait with timeout
		timespec timeout;
		timeout.tv_sec = time(NULL) + seconds;
		timeout.tv_nsec = 0;
		err2 = pthread_mutex_lock(&mu);

		if (err2 == 0)
		{
			do {
				err = pthread_cond_timedwait(&cv, &mu, &timeout);
				if (err != 0) {
					rt = false;
				}
				else
					rt = true;
			} while (err == EINTR);		
			if (err == ETIMEDOUT)
				rt = false;

			pthread_mutex_unlock(&mu);
			return rt;
		}
		if (err2 == EBUSY)
			return false;

		system_call_failed::raise("pthread_mutex_lock", err2);

		return false; //compiler silencer
	}
	
	void enter()
	{
		fb_assert(init == true);
		int err = 0;
		int	err2 = pthread_mutex_lock(&mu);
		if (err2 == 0)
		{
			do {
				err = pthread_cond_wait(&cv, &mu);
				if (err == 0) {
				   break;
				}
			} while (err == EINTR);
			
			pthread_mutex_unlock(&mu);
		}
		else 
			system_call_failed::raise("pthread_mutex_lock", err2);
	}
	
	void release(SLONG count = 1)
	{
		int err = 0;
		fb_assert(init == true);
		for (int i = 0; i < count; i++) 
		{
			err = pthread_mutex_lock(&mu) ;
			if (err == 0) {
				err = pthread_cond_broadcast(&cv);
				if (err != 0) {
					system_call_failed::raise("pthread_cond_broadcast", err);
				}

				pthread_mutex_unlock(&mu);
			} 
			else {
				//gds__log("Error on semaphore.h: release");
				system_call_failed::raise("pthread_mutex_lock", err);
			}
		}	
	}
};

} // namespace Firebird

#endif //defined(HAVE_SYS_SEM_H) && defined(HAVE_SEMTIMEDOP)

#endif //HAVE_SEM_TIMEDWAIT

#else //MULTI_THREAD

namespace Firebird 
{
class SignalSafeSemaphore 
{
public:
	SignalSafeSemaphore() { }
	~SignalSafeSemaphore() { }

	void enter() { }
	void release(SLONG count = 1) { }
	bool tryEnter(int seconds = 0) { return true; }
};
typedef SignalSafeSemaphore Semaphore;
} // namespace Firebird

#endif //MULTI_THREAD

#endif //WIN_NT

#endif // CLASSES_SEMAPHORE_H
