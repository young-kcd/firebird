/*
 *	PROGRAM:		Client/Server Common Code
 *	MODULE:			semaphore.h
 *	DESCRIPTION:	Semaphore lock
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
 *  $Id: semaphore.h,v 1.3 2003-10-30 10:59:32 brodsom Exp $
 *
 */

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

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
	}
	~Semaphore() { CloseHandle(hSemaphore); }	

	bool tryEnter(int seconds = 0) {
		DWORD result = WaitForSingleObject(
			hSemaphore, seconds >= 0 ? seconds*1000 : INFINITE);
		if (result == WAIT_FAILED)
			system_call_failed::raise();
		return result != WAIT_TIMEOUT;
	}

	void release(SLONG count = 1) {
		if (!ReleaseSemaphore(hSemaphore, count, NULL))
			system_call_failed::raise();
	}
};


}

#else

#ifdef MULTI_THREAD

#include <semaphore.h>
#include <errno.h>

namespace Firebird {

class Semaphore {
private:
	sem_t sem;
	bool  init;
public:
	Semaphore() {
		if (sem_init(&sem, 0, 0) == -1){
			gds__log("Error on semaphore.h constructor error");
			system_call_failed::raise();
		}
		init = true;
	}
	~Semaphore() {
		assert(init==true);
		if (sem_destroy(&sem) == -1) {
			gds__log("Error on semaphore.h destructor error");
			system_call_failed::raise();
		}
		init = false;

	}
	bool tryEnter(int seconds = 0) {
		assert(init==true);
		if (seconds == 0) {
			if (sem_trywait(&sem) != -1) 
				return true;
			if (errno == EAGAIN) 
				return false;
		} else if (seconds < 0) {
			if (sem_wait(&sem) != -1)
				return false;
		} else {
			struct timespec timeout;
			timeout.tv_sec = time(NULL) + seconds;
			timeout.tv_nsec = 0;
			if (sem_timedwait(&sem, &timeout) == 0) 
				return true;
			if (errno == ETIMEDOUT) 
				return false;
		}
		char message[128];
		sprintf(message,"Error on semaphore.h tryEnter errno=%d\n",errno);
		gds__log(message);
		system_call_failed::raise();
	}
	void enter() {
		assert(init==true);
		if (sem_wait(&sem) == -1){
			gds__log("Error on semaphore.h enter error");
			system_call_failed::raise();
		}
	}
	void release(SLONG count = 1) {
		assert(init==true);
		for (int i = 0; i < count; i++)
			if (sem_post(&sem) == -1){
				gds__log("Error on semaphore.h release error");
				system_call_failed::raise();
			}
	}
};

}

#endif /*MULTI_THREAD*/

#endif /*!WIN_NT*/

#endif
