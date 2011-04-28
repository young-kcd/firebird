/*
*  
*     The contents of this file are subject to the Initial 
*     Developer's Public License Version 1.0 (the "License"); 
*     you may not use this file except in compliance with the 
*     License. You may obtain a copy of the License at 
*     http://www.ibphoenix.com/idpl.html. 
*
*     Software distributed under the License is distributed on 
*     an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either 
*     express or implied.  See the License for the specific 
*     language governing rights and limitations under the License.
*
*     The contents of this file or any work derived from this file
*     may not be distributed under any other license whatsoever 
*     without the express prior written permission of the original 
*     author.
*
*
*  The Original Code was created by James A. Starkey for IBPhoenix.
*
*  Copyright (c) 1997 - 2000, 2001, 2003 James A. Starkey
*  Copyright (c) 1997 - 2000, 2001, 2003 Netfrastructure, Inc.
*  All Rights Reserved.
*
*  The Ñode was ported into Firebird Open Source RDBMS project by
*  Vladyslav Khorsun at 2010
*
*  Contributor(s):
*/

#include "../../common/common.h"
#include "fb_tls.h"
#include "../thd.h"

#include "SyncObject.h"
#include "Synchronize.h"

#define NANO		1000000000
#define MICRO		1000000

namespace Firebird {

Synchronize::Synchronize()
{
	shutdownInProgress = false;
	sleeping = false;
	wakeup = false;

#ifdef _WIN32
	evnt = CreateEvent(NULL, false, false, NULL);
#endif

#ifdef _PTHREADS
	int ret = pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&condition, NULL);
#endif

#ifdef SOLARIS_MT
	int ret = mutex_init(&mutex, USYNC_THREAD, NULL);
	cond_init(&condition, USYNC_THREAD, NULL);
#endif
}

Synchronize::~Synchronize()
{
#ifdef _WIN32
	CloseHandle(evnt);
#endif

#ifdef _PTHREADS
	int ret = pthread_mutex_destroy(&mutex);
	ret = pthread_cond_destroy(&condition);
#endif

#ifdef SOLARIS_MT
	int ret = mutex_destroy(&mutex);
#endif
}


void Synchronize::sleep()
{
	sleeping = true;
#ifdef _WIN32
#ifdef _DEBUG
	for (;;)
	{
		const int n = WaitForSingleObject(evnt, 10000);
		if (n != WAIT_TIMEOUT)
			break;
	}
#else
	sleep (INFINITE);
#endif
#endif

#ifdef _PTHREADS
	int ret = pthread_mutex_lock(&mutex);
	CHECK_RET("pthread_mutex_lock failed, errno %d", errno);

	while (!wakeup)
		pthread_cond_wait(&condition, &mutex);

	wakeup = false;
	ret = pthread_mutex_unlock(&mutex);
	CHECK_RET("pthread_mutex_unlock failed, errno %d", errno);
#endif

#ifdef SOLARIS_MT
	int ret = mutex_lock(&mutex);
	CHECK_RET("mutex_lock failed, errno %d", errno);

	while (!wakeup)
		cond_wait(&condition, &mutex);

	wakeup = false;
	ret = mutex_unlock(&mutex);
	CHECK_RET("mutex_unlock failed, errno %d", errno);
#endif

	sleeping = false;
}

bool Synchronize::sleep(int milliseconds)
{
	sleeping = true;

#ifdef _WIN32
	const int n = WaitForSingleObject(evnt, milliseconds);
	sleeping = false;

	return n != WAIT_TIMEOUT;
#endif

#ifdef _PTHREADS
	struct timeval microTime;
	int ret = gettimeofday(&microTime, NULL);
	SINT64 nanos = (SINT64) microTime.tv_sec * NANO + microTime.tv_usec * 1000 +
		(SINT64) milliseconds * 1000000;
	struct timespec nanoTime;
	nanoTime.tv_sec = nanos / NANO;
	nanoTime.tv_nsec = nanos % NANO;
	ret = pthread_mutex_lock (&mutex);
	CHECK_RET("pthread_mutex_lock failed, errno %d", errno);
	int seconds = nanoTime.tv_sec - microTime.tv_sec;

	while (!wakeup)
	{
#ifdef MVS
		ret = pthread_cond_timedwait(&condition, &mutex, &nanoTime);
		if (ret == -1 && errno == EAGAIN)
			ret = ETIMEDOUT;
		break;
#else
		ret = pthread_cond_timedwait(&condition, &mutex, &nanoTime);
		if (ret == ETIMEDOUT)
			break;
#endif
		/***
		if (!wakeup)
		Log::debug ("Synchronize::sleep(milliseconds): unexpected wakeup, ret %d\n", ret);
		***/
	}

	sleeping = false;
	wakeup = false;
	pthread_mutex_unlock(&mutex);
	return ret != ETIMEDOUT;
#endif

#ifdef SOLARIS_MT
	struct timeval microTime;
	int ret = gettimeofday(&microTime, NULL);
	SINT64 nanos = (SINT64) microTime.tv_sec * NANO + microTime.tv_usec * 1000 +
		(SINT64) milliseconds * 1000000;
	struct timespec nanoTime;
	nanoTime.tv_sec = nanos / NANO;
	nanoTime.tv_nsec = nanos % NANO;
	ret = mutex_lock (&mutex);
	CHECK_RET("mutex_lock failed, errno %d", errno);
	int seconds = nanoTime.tv_sec - microTime.tv_sec;

	while (!wakeup)
	{
		ret = cond_timedwait(&condition, &mutex, &nanoTime);
		if (ret == ETIMEDOUT)
			break;
		/***
		if (!wakeup)
		Log::debug ("Synchronize::sleep(milliseconds): unexpected wakeup, ret %d\n", ret);
		***/
	}

	sleeping = false;
	wakeup = false;
	mutex_unlock(&mutex);

	return ret != ETIMEDOUT;
#endif
}

void Synchronize::wake()
{
#ifdef _WIN32
	SetEvent (evnt);
#endif

#ifdef _PTHREADS
	int ret = pthread_mutex_lock(&mutex);
	CHECK_RET("pthread_mutex_lock failed, errno %d", errno);
	wakeup = true;
	pthread_cond_broadcast(&condition);
	ret = pthread_mutex_unlock(&mutex);
	CHECK_RET("pthread_mutex_unlock failed, errno %d", errno);
#endif

#ifdef SOLARIS_MT
	int ret = mutex_lock(&mutex);
	CHECK_RET("mutex_lock failed, errno %d", errno);
	wakeup = true;
	cond_broadcast(&condition);
	ret = mutex_unlock(&mutex);
	CHECK_RET("mutex_unlock failed, errno %d", errno);
#endif
}

void Synchronize::shutdown()
{
	shutdownInProgress = true;
	wake();
}



/// ThreadSync

TLS_DECLARE (ThreadSync*, threadIndex);

ThreadSync::ThreadSync(const char *desc)
{
	init(desc);
	setThread(this);
}

void ThreadSync::init(const char *desc)
{
	description = desc;
	threadId = getCurrentThreadId();
	prevWaiting = nextWaiting = NULL;
	lockType = None;
	lockGranted = false;
	lockPending = NULL;
	locks = NULL;
}

ThreadSync::~ThreadSync()
{
	setThread (NULL);
}


ThreadSync* ThreadSync::findThread()
{
	return TLS_GET(threadIndex);
}


ThreadSync* ThreadSync::getThread(const char *desc)
{
	ThreadSync *thread = findThread();

	if (!thread)
	{
		thread = new ThreadSync (desc);
		setThread(thread);
	}

	return thread;
}

void ThreadSync::setThread(ThreadSync *thread)
{
	TLS_SET(threadIndex, thread);
}

FB_THREAD_ID ThreadSync::getCurrentThreadId(void)
{
	return getThreadId();
}


const char* ThreadSync::getWhere()
{
	if (lockPending && lockPending->where)
		return lockPending->where;

	return "";
}


void ThreadSync::validateLocks()
{
	ThreadSync *thread = getThread("ThreadSync::validateLocks");

	// hvlad: not worked
	if (thread->locks)
	{
		LOG_DEBUG ("thread %d has active locks:\n", thread->threadId);
		for (Sync *sync = thread->locks; sync; sync = sync->prior)
			LOG_DEBUG ("   %s\n", sync->where);
	}
}


void ThreadSync::grantLock(SyncObject *lock)
{
	ASSERT(!lockGranted);
	ASSERT(!lockPending || lockPending->syncObject == lock);

	lockGranted = true;
	lockPending = NULL;

	wake();
}

} // namespace Firebird
