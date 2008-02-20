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

#ifdef UNIX
#define MTX_STRUCTURE_DEFINED

// This is the only place where COND_STRUCT and MUTEX_STRUCT are used.
// Therefore it's definitions were moved here from thd.h in order 
// to avoid circular dependencies in h-files.
// Alex Peshkov. Tue 08 Jun 2004 02:22:08 PM MSD.

#ifdef SOLARIS_MT
#include <thread.h>
#define COND_STRUCT cond_t
#define MUTEX_STRUCT mutex_t
#endif

#ifdef USE_POSIX_THREADS
#include <pthread.h>
#define COND_STRUCT pthread_cond_t
#define MUTEX_STRUCT pthread_mutex_t
#endif

#ifndef COND_STRUCT
#define COND_STRUCT SCHAR
#endif
#ifndef MUTEX_STRUCT
#define MUTEX_STRUCT SCHAR
#endif

struct mtx {
	MUTEX_STRUCT mtx_mutex[1];
};
typedef mtx MTX_T;
typedef mtx* MTX;

struct event_t
{
	SLONG event_semid;
	SLONG event_count;
	MUTEX_STRUCT event_mutex[1];
	COND_STRUCT event_semnum[1];
};

#define SH_MEM_STRUCTURE_DEFINED
struct sh_mem
{
	UCHAR *sh_mem_address;
	SLONG sh_mem_length_mapped;
	SLONG sh_mem_handle;
};
typedef sh_mem SH_MEM_T;
typedef sh_mem *SH_MEM;
#endif // UNIX


#ifdef WIN_NT
#define MTX_STRUCTURE_DEFINED

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

typedef mtx MTX_T;
typedef mtx *MTX;

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
	SLONG	sh_mem_length_mapped;
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


#ifndef MTX_STRUCTURE_DEFINED
#define MTX_STRUCTURE_DEFINED
struct mtx
{
	SSHORT	mtx_event_count[3];
	SCHAR	mtx_use_count;
	SCHAR	mtx_wait;
};
typedef mtx MTX_T;
typedef mtx *MTX;
#endif
#undef MTX_STRUCTURE_DEFINED

#ifndef SH_MEM_STRUCTURE_DEFINED
#define SH_MEM_STRUCTURE_DEFINED
struct sh_mem
{
	UCHAR*	sh_mem_address;
	SLONG	sh_mem_length_mapped;
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
