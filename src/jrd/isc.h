/*
 *	PROGRAM: JRD access method
 *	MODULE:  isc.h
 *	DESCRIPTION: Common descriptions for general-purpose but non-user routines
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
 * 26-Sept-2001 Paul Beach - External File Directory Config. Parameter
 * 17-Oct-2001  Mike Nordell - CPU affinity
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#ifndef JRD_ISC_H
#define JRD_ISC_H

// Firebird platform-specific synchronization data structures

#if defined(DARWIN)
#define USE_SYS5SEMAPHORE
#endif
//#define USE_SYS5SEMAPHORE

#ifdef UNIX

#ifdef SOLARIS_MT
#include <thread.h>

struct mtx {
	mutex_t mtx_mutex[1];
};

struct event_t
{
	SLONG event_count;
	mutex_t event_mutex[1];
	cond_t event_cond[1];
};

#endif

#ifdef USE_POSIX_THREADS
#include <pthread.h>

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

#endif // USE_POSIX_THREADS

#define SH_MEM_STRUCTURE_DEFINED
struct sh_mem
{
	UCHAR *sh_mem_address;
	ULONG sh_mem_length_mapped;
	SLONG sh_mem_handle;
};
typedef sh_mem SH_MEM_T;
typedef sh_mem *SH_MEM;
#endif // UNIX


#ifdef WIN_NT
#include <windows.h>

typedef struct _FAST_MUTEX_SHARED_SECTION
{
	SLONG   fInitialized;
	SLONG   lSpinLock;
	SLONG   lThreadsWaiting;
	SLONG   lAvailable;
#ifdef _DEBUG
	DWORD  dwThreadId;
#endif
} FAST_MUTEX_SHARED_SECTION;

typedef struct _FAST_MUTEX
{
	HANDLE hEvent;
	HANDLE hFileMap;
	SLONG  lSpinCount;
	volatile FAST_MUTEX_SHARED_SECTION* lpSharedInfo;
} FAST_MUTEX;

struct mtx
{
	FAST_MUTEX mtx_fast;
};

struct event_t
{
	SLONG		event_pid;
	SLONG		event_id;
	SLONG		event_count;
	void*		event_handle;
};

#define SH_MEM_STRUCTURE_DEFINED
struct sh_mem
{
	UCHAR*	sh_mem_address;
	ULONG	sh_mem_length_mapped;
	void*	sh_mem_handle;
	void*	sh_mem_object;
	void*	sh_mem_interest;
	void*	sh_mem_hdr_object;
	SLONG*	sh_mem_hdr_address;
	TEXT	sh_mem_name[MAXPATHLEN];
};
typedef sh_mem SH_MEM_T;
typedef sh_mem *SH_MEM;
#define THREAD_HANDLE_DEFINED
typedef void *THD_T;
#endif // WIN_NT

#ifndef SH_MEM_STRUCTURE_DEFINED
#define SH_MEM_STRUCTURE_DEFINED
struct sh_mem
{
	UCHAR*	sh_mem_address;
	ULONG	sh_mem_length_mapped;
	SLONG	sh_mem_handle;
};
typedef sh_mem SH_MEM_T;
typedef sh_mem *SH_MEM;
#endif
#undef SH_MEM_STRUCTURE_DEFINED

#ifndef THREAD_HANDLE_DEFINED
#define THREAD_HANDLE_DEFINED
typedef ULONG THD_T;
#endif
#undef THREAD_HANDLE_DEFINED


// Interprocess communication configuration structure
// This was used to read to and write from the Config dialog when the server
// or the guardian is showing an icon in the tray.

/*
struct ipccfg
{
	const char*	ipccfg_keyword;
	SCHAR		ipccfg_key;
	SLONG*		ipccfg_variable;
	SSHORT		ipccfg_parent_offset;	// Relative offset of parent keyword
	USHORT		ipccfg_found;		// TRUE when keyword has been set
};

typedef ipccfg *IPCCFG;
*/

#endif // JRD_ISC_H
