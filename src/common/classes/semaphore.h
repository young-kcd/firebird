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

#ifdef DARWIN

#include <pthread.h>
#include <errno.h>

namespace Firebird {

class Semaphore {
private:
	pthread_mutex_t	mu;
	pthread_cond_t	cv;
	bool	init;
public:
	Semaphore() : init(false) {
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
	
	~Semaphore() {
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
	
	bool tryEnter(int seconds = 0) {
		bool rt = false;
		int err2 = 0;
		int err = 0;
		// Return true in case of success
		fb_assert(init == true);
		if (seconds == 0) {
			// Instant try
			
			err2 = pthread_mutex_trylock(&mu);
			if (err2 == 0) {
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
			else if (err2 == EBUSY) {
				rt = false;
				return rt;
			}
			
			system_call_failed::raise("pthread_mutex_trylock", err2);
		}
		if (seconds < 0) {
			// Unlimited wait, like enter()
			err2 = pthread_mutex_lock(&mu);
			if (err2 == 0) {
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
			else if (err2 == EBUSY) {
				rt = false;
				return rt;
			}
			else 
				system_call_failed::raise("pthread_mutex_lock", err2);
			
		} //seconds < 0 
		// Wait with timeout
		timespec timeout;
		timeout.tv_sec = time(NULL) + seconds;
		timeout.tv_nsec = 0;
		err2 = pthread_mutex_lock(&mu);
		if (err2 == 0) {
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
		else if (err2 == EBUSY) {
			rt = false;
			return rt;
  		}
		else
			system_call_failed::raise("pthread_mutex_lock", err2);
	}
	
	void enter() {
		fb_assert(init == true);
		int err = 0;
		int	err2 = pthread_mutex_lock(&mu);
		if (err2 == 0) {
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
	
	void release(SLONG count = 1) {
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

#define SEM_DEFINED

#endif //DARWIN


#ifndef SEM_DEFINED
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

#endif /* SEM_DEFINED */
#else  /* MULTI_THREAD*/

namespace Firebird {

class Semaphore {
public:
	Semaphore() {}
	~Semaphore() {}
	bool tryEnter(int seconds = 0) {return true;}
	void enter() {}
	void release(SLONG count = 1) {}
};

} // namespace Firebird

#endif /* MULTI_THREAD*/

#endif /*!WIN_NT*/

#endif // CLASSES_SEMAPHORE_H

