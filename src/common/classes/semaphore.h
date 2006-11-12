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

#ifdef WIN_NT

#include <windows.h>
#include <limits.h>

namespace Firebird {

class Semaphore {
private:
	HANDLE hSemaphore;
public:
	Semaphore() { 
		hSemaphore = CreateSemaphore(NULL, 0 /*initial count*/, 
			INT_MAX, NULL); 
		if (hSemaphore == NULL)
			system_call_failed::raise("CreateSemaphore");
	}
	~Semaphore() {
		if (hSemaphore && !CloseHandle(hSemaphore))
			system_call_failed::raise("CloseHandle");
	}	

	bool tryEnter(int seconds = 0) {
		DWORD result = WaitForSingleObject(
			hSemaphore, seconds >= 0 ? seconds * 1000 : INFINITE);
		if (result == WAIT_FAILED)
			system_call_failed::raise("WaitForSingleObject");
		return result != WAIT_TIMEOUT;
	}

	void release(SLONG count = 1) {
		if (!ReleaseSemaphore(hSemaphore, count, NULL))
			system_call_failed::raise("ReleaseSemaphore");
	}
};


} // namespace Firebird

#else

#ifdef MULTI_THREAD

#ifdef SOLARIS_MT
/* This is  implementation of FB::Semaphore
 on Solaris using conditional variable protected by mutex.
 Reasons is:
	1) Old (~1985) troubles reported to Borland about semaphores on Solaris
	2) No gds_lock_manager
	3) No sem_timedwait on Solaris
	4) thread functions do not guarantee setting errno 12 apr 2006
 Readings is:
	1) http://www.freebsd.org/cgi/cvsweb.cgi/src/lib/libpthread/thread/thr_sem.c?rev=1.15
	2) http://docs.sun.com/app/docs/doc/802-5747-03/6i9g1bhqp?a=view
	3) http://docs.sun.com/app/docs/doc/802-5938?a=expand
Status:
		12 apr 2006 add looping if EINTR caused
		Still under testings 
 Konstantin Kuznetsov kkuznetsov at the users of the sf net
 
 
 thank's to Claudio ...
 */ 
#include <thread.h>
#include <synch.h>

#include <semaphore.h>
#include <errno.h>
#include <sys/stat.h> //for timestruct_t
namespace Firebird {

class Semaphore {
private:
	mutex_t	mu;
	cond_t	cv;
	bool	init;
public:
	Semaphore() : init(false) {
		/* USINC_PROCESS got  ability to syncronise Classic
		*/
		int err = mutex_init(&mu, USYNC_PROCESS, NULL);
		if (err != 0) {
			//gds__log("Error on semaphore.h: constructor");
			system_call_failed::raise("mutex_init", err);
		}
		err = cond_init(&cv, USYNC_PROCESS, NULL);
		if (err != 0) {
			//gds__log("Error on semaphore.h: constructor");
			system_call_failed::raise("cond_init", err);
		}
		init = true;
	}
	
	~Semaphore() {
		fb_assert(init == true);
		int err = mutex_destroy(&mu);
		if (err != 0) {
			//gds__log("Error on semaphore.h: destructor");
			system_call_failed::raise("mutex_destroy", err);
		}
		err = cond_destroy(&cv);
		if (err != 0) {
			//gds__log("Error on semaphore.h: destructor");
			system_call_failed::raise("cond_destroy", err);
		}
		
		init = false;

	}
	
	bool tryEnter(int seconds = 0) {
		bool rt = false;
		int err2 = 0;
		int err = 0;
		// Return true in case of success
		fb_assert(init == true);
		if (seconds == 0) {
			// Instant try
			
			err2 = mutex_trylock(&mu);
			if (err2 != 0) {
				do {
					err = cond_wait(&cv, &mu);
					if (err != 0) {
						rt = false;
					}
					else
						rt = true;
				} while (err == EINTR);	
			    if (err == ETIMEDOUT)
					rt = false;

				mutex_unlock(&mu);
				return rt;
			}
			else if (err2 == EBUSY) {
				rt = false;
				return rt;
			}
			
			system_call_failed::raise("mutex_trylock", err2);
		}
		if (seconds < 0) {
			// Unlimited wait, like enter()
			err2 = mutex_lock(&mu);
			if (err2 != 0) {
				do {
					err = cond_wait(&cv, &mu);
					if (err != 0) {
						rt = false;
					}
					else 
						rt = true;
				} while (err == EINTR);
				if (err == ETIMEDOUT)
					rt = false;

				mutex_unlock(&mu);
				return rt;
			}
			else if (err2 == EBUSY) {
				rt = false;
				return rt;
			}
			else 
				system_call_failed::raise("mutex_lock", err2);
			
		} //seconds < 0 
		// Wait with timeout
		timestruc_t timeout;
		timeout.tv_sec = time(NULL) + seconds;
		timeout.tv_nsec = 0;
		err2 = mutex_lock(&mu);
		if (err2 != 0) {
			do {
				err = cond_timedwait(&cv, &mu, &timeout);
				if (err != 0) {
					rt = false;
				}
				else
					rt = true;
			} while (err == EINTR);		
			if (err == ETIMEDOUT)
				rt = false;

			mutex_unlock(&mu);
			return rt;
		}
		else if (err2 == EBUSY) {
			rt = false;
			return rt;
  		}
		else
			system_call_failed::raise("mutex_lock", err2);
	}
	
	void enter() {
		fb_assert(init == true);
		int err = 0;
		int	err2 = mutex_lock(&mu);
		if (err2 != 0) {
			do {
				err = cond_wait(&cv, &mu);
				if (err == 0) {
				   break;
				}
			} while (err == EINTR);
			
			mutex_unlock(&mu);
		}
		else 
			system_call_failed::raise("mutex_lock", err2);
	}
	
	void release(SLONG count = 1) {
		int err = 0;
		fb_assert(init == true);
		for (int i = 0; i < count; i++) 
		{
			err = mutex_lock(&mu) ;
			if (err != 0) {
				err = cond_broadcast(&cv);
				if (err != 0) {
					system_call_failed::raise("cond_signal", err);
				}

				mutex_unlock(&mu);
			} 
			else {
				//gds__log("Error on semaphore.h: release");
				system_call_failed::raise("mutex_lock", err);
			}
		}	
	}
};

} // namespace Firebird


#else
#include <semaphore.h>
#include <errno.h>

namespace Firebird {

class Semaphore {
private:
	sem_t sem;
	bool  init;
public:
	Semaphore() : init(false) {
		if (sem_init(&sem, 0, 0) == -1) {
			//gds__log("Error on semaphore.h: constructor");
			system_call_failed::raise("sem_init");
		}
		init = true;
	}
	
	~Semaphore() {
		fb_assert(init == true);
		if (sem_destroy(&sem) == -1) {
			//gds__log("Error on semaphore.h: destructor");
			system_call_failed::raise("sem_destroy");
		}
		init = false;

	}
	
	bool tryEnter(int seconds = 0) {
		// Return true in case of success
		fb_assert(init == true);
		if (seconds == 0) {
			// Instant try
			do {
				if (sem_trywait(&sem) != -1) 
					return true;
			} while (errno == EINTR);
			if (errno == EAGAIN) 
				return false;
			system_call_failed::raise("sem_trywait");
		}
		if (seconds < 0) {
			// Unlimited wait, like enter()
			do {
				if (sem_wait(&sem) != -1)
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
			int rc = sem_timedwait(&sem, &timeout);
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
	
	void enter() {
		fb_assert(init == true);
		do {
			if (sem_wait(&sem) != -1)
				return;
		} while (errno == EINTR);
		//gds__log("Error on semaphore.h: enter");
		system_call_failed::raise("sem_wait");
	}
	
	void release(SLONG count = 1) {
		fb_assert(init == true);
		for (int i = 0; i < count; i++)
			if (sem_post(&sem) == -1) {
				//gds__log("Error on semaphore.h: release");
				system_call_failed::raise("sem_post");
			}
	}
};

} // namespace Firebird

#endif /* SOLARIS_MT */
#endif /*MULTI_THREAD*/

#endif /*!WIN_NT*/

#endif // CLASSES_SEMAPHORE_H

