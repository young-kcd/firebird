/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		isc_s_proto.h
 *	DESCRIPTION:	Prototype header file for isc_sync.cpp
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
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#ifndef JRD_ISC_S_PROTO_H
#define JRD_ISC_S_PROTO_H

#include "../common/classes/alloc.h"

// Firebird platform-specific synchronization data structures

#if defined(DARWIN) || defined(FREEBSD)
#define USE_SYS5SEMAPHORE	// TODO: remap system objects to separate pages in System V case
#endif

#ifdef LINUX
// This hack fixes CORE-2896 - embedded connections fail on linux.
// Looks like a lot of linux kernels are buggy when working with PRIO_INHERIT mutexes.
#undef HAVE_PTHREAD_MUTEXATTR_SETPROTOCOL
#endif

#if defined HAVE_MMAP || defined WIN_NT
#define HAVE_OBJECT_MAP
#endif


#ifdef UNIX

#if defined(USE_POSIX_THREADS)

#if defined(HAVE_PTHREAD_MUTEXATTR_SETROBUST_NP) && defined(HAVE_PTHREAD_MUTEX_CONSISTENT_NP)

#define USE_ROBUST_MUTEX

#endif // ROBUST mutex

#include "fb_pthread.h"

#ifdef USE_SYS5SEMAPHORE

struct Sys5Semaphore
{
	int semSet;				// index in shared memory table
	unsigned short semNum;	// number in semset
	int getId();
};

struct mtx : public Sys5Semaphore
{
};

struct event_t : public Sys5Semaphore
{
	SLONG event_count;
};

#else

struct mtx
{
	pthread_mutex_t mtx_mutex[1];
};

struct event_t
{
	SLONG event_count;
	int pid;
	pthread_mutex_t event_mutex[1];
	pthread_cond_t event_cond[1];
};

#endif // USE_SYS5SEMAPHORE

#else // USE_POSIX_THREADS
#error: Do not know how to declare shared mutex and event on this system.
#endif // USE_POSIX_THREADS

#define SH_MEM_STRUCTURE_DEFINED

#endif // UNIX


#ifdef WIN_NT
#include <windows.h>

struct FAST_MUTEX_SHARED_SECTION
{
	SLONG fInitialized;
	SLONG lSpinLock;
	SLONG lThreadsWaiting;
	SLONG lAvailable;
	SLONG lOwnerPID;
#ifdef _DEBUG
	DWORD dwThreadId;
#endif
};

struct FAST_MUTEX
{
	HANDLE hEvent;
	HANDLE hFileMap;
	SLONG lSpinCount;
	volatile FAST_MUTEX_SHARED_SECTION* lpSharedInfo;
};

struct mtx
{
	FAST_MUTEX mtx_fast;
};

struct event_t
{
	SLONG event_pid;
	SLONG event_id;
	SLONG event_count;
	void* event_handle;
};

#endif // WIN_NT

SLONG	ISC_event_clear(event_t*);
void	ISC_event_fini(event_t*);
int		ISC_event_init(event_t*);

int		ISC_event_post(event_t*);
int		ISC_event_wait(event_t*, const SLONG, const SLONG);

#ifdef UNIX
int		ISC_mutex_init(struct mtx*);
#endif

#ifdef WIN_NT
int		ISC_mutex_init(struct mtx*, const TEXT*);
void	ISC_mutex_fini(struct mtx*);
#endif

int		ISC_mutex_lock(struct mtx*);
int		ISC_mutex_lock_cond(struct mtx*);
int		ISC_mutex_unlock(struct mtx*);

ULONG	ISC_exception_post(ULONG, const TEXT*);

namespace Jrd {

class MemoryHeader
{
public:
	USHORT mhb_type;
	USHORT mhb_version;
#ifndef WIN_NT
	struct mtx mhb_mutex;
#endif
};

class SharedMemoryBase
{
public:
	SharedMemoryBase();
	virtual ~SharedMemoryBase();

	virtual bool initialize(bool) = 0;

	virtual void setHeader(MemoryHeader*) = 0;
	virtual MemoryHeader* getHeader() const = 0;

	bool mapFile(Firebird::Arg::StatusVector& status, const TEXT* fileName, ULONG size);
#ifdef HAVE_OBJECT_MAP
	UCHAR* mapObject(Firebird::Arg::StatusVector& status, ULONG offset, ULONG size);
	void unmapObject(Firebird::Arg::StatusVector& status, UCHAR** object, ULONG size);
#endif
	bool remapFile(Firebird::Arg::StatusVector& status, ULONG newSize, bool truncateFlag);
	void unmapFile(Firebird::Arg::StatusVector& status);
	void removeMapFile();

	virtual void mutexBug(int osErrorCode, const char* text) = 0;
	void mutexLock();
	bool mutexLockCond();
	void mutexUnlock();

public:
#ifdef WIN_NT
	struct mtx sh_mem_winMutex;
#endif
	struct mtx* sh_mem_mutex;

	ULONG	sh_mem_length_mapped;
#ifdef WIN_NT
	void*	sh_mem_handle;
	void*	sh_mem_object;
	void*	sh_mem_interest;
	void*	sh_mem_hdr_object;
	ULONG*	sh_mem_hdr_address;
#else
	int		sh_mem_handle;
#endif
	TEXT	sh_mem_name[MAXPATHLEN];

public:
	enum MemoryTypes
	{
		SRAM_LOCK_MANAGER = 0xFF,		// To avoid mixing with old files no matter of endianness
		SRAM_DATABASE_SNAPSHOT = 0xFE,	// use downcount for shared memory types
		SRAM_EVENT_MANAGER = 0xFD,
		SRAM_TRACE_CONFIG = 0xFC,
		SRAM_TRACE_LOG = 0xFB
	};

protected:
	void logError(const char* text, const Firebird::Arg::StatusVector& status);
};

template <class Header>		// Header must be "public MemoryHeader"
class SharedMemory : public SharedMemoryBase
{
public:
	SharedMemory()
		: SharedMemoryBase(), sh_mem_header(NULL)
	{ }

	~SharedMemory()
	{
		if (sh_mem_header)
		{
			Firebird::Arg::StatusVector status;
			unmapFile(status);
			if (status.hasData())
			{
				logError("Failed unmapFile() in SharedMemory's dtor", status);
			}
		}
	}

#ifdef HAVE_OBJECT_MAP
	template <class Object> Object* mapObject(Firebird::Arg::StatusVector& status, ULONG offset)
	{
		return (Object*) SharedMemoryBase::mapObject(status, offset, sizeof(Object));
	}

	template <class Object> void unmapObject(Firebird::Arg::StatusVector& status, Object** object)
	{
		SharedMemoryBase::unmapObject(status, (UCHAR**) object, sizeof(Object));
	}
#endif

private:
	void setHeader(MemoryHeader* hdr)
	{
		sh_mem_header = (Header*) hdr;
	}

	MemoryHeader* getHeader() const
	{
		return sh_mem_header;
	}

public:
	Header* volatile sh_mem_header;
};

} // namespace Jrd

#endif // JRD_ISC_S_PROTO_H
