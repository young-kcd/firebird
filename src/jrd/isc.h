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
 * Added TCP_NO_DELAY option for superserver on Linux
 * FSG 16.03.2001 
 * 26-Sept-2001 Paul Beach - External File Directory Config. Parameter
 * 17-Oct-2001  Mike Nordell - CPU affinity
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#ifndef JRD_ISC_H
#define JRD_ISC_H

// For AST_THREAD
#include "../jrd/common.h"
#include "../jrd/sch_proto.h"

/* Defines for semaphore and shared memory removal */

const USHORT ISC_SEM_REMOVE		= 1;
const USHORT ISC_MEM_REMOVE		= 2;

/* InterBase platform-specific synchronization data structures */

#ifdef VMS
struct itm {
	SSHORT itm_length;
	SSHORT itm_code;
	SCHAR *itm_buffer;
	SSHORT *itm_return_length;
};

typedef itm ITM;

struct event_t {
	SLONG event_pid;
	SLONG event_count;
};

struct wait {
	USHORT wait_count;
	event_t* wait_events;
	SLONG *wait_values;
};

typedef wait WAIT;

/* Lock status block */

struct lock_status {
	SSHORT lksb_status;
	SSHORT lksb_reserved;
	SLONG lksb_lock_id;
	SLONG lksb_value[4];
};

/* Poke block (for asynchronous poking) */

struct poke {
	struct poke *poke_next;
	lock_status poke_lksb;
	SLONG poke_parent_id;
	USHORT poke_value;
	USHORT poke_use_count;
};

typedef poke* POKE;

#define SH_MEM_STRUCTURE_DEFINED
struct sh_mem {
	int sh_mem_system_flag;
	UCHAR *sh_mem_address;
	SLONG sh_mem_length_mapped;
	SLONG sh_mem_mutex_arg;
	SLONG sh_mem_handle;
	SLONG sh_mem_retadr[2];
	SLONG sh_mem_channel;
	TEXT sh_mem_filename[128];
};
typedef struct sh_mem SH_MEM_T;
typedef struct sh_mem* SH_MEM;
#endif


#ifdef UNIX
#define MTX_STRUCTURE_DEFINED

// This is the only place where COND_STRUCT and MUTEX_STRUCT are used.
// Therefore it's definitions were moved here from thd.h in order 
// to avoid circular dependencies in h-files.
// Alex Peshkov. Tue 08 Jun 2004 02:22:08 PM MSD.

#if  defined(SOLARIS_MT)
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

#ifdef ANY_THREADING
struct mtx {
	MUTEX_STRUCT mtx_mutex[1];
};
typedef mtx MTX_T;
typedef mtx* MTX;
#else
struct mtx {
	SLONG mtx_semid;
	SSHORT mtx_semnum;
	SCHAR mtx_use_count;
	SCHAR mtx_wait;
};
typedef mtx MTX_T;
typedef mtx* MTX;
#endif /* ANY_THREADING */


#ifdef ANY_THREADING

struct event_t
{
	SLONG event_semid;
	SLONG event_count;
	MUTEX_STRUCT event_mutex[1];
	COND_STRUCT event_semnum[1];
};
#else
struct event_t
{
	SLONG event_semid;
	SLONG event_count;
	SSHORT event_semnum;
};
#endif /* ANY_THREADING */


#define SH_MEM_STRUCTURE_DEFINED
struct sh_mem
{
	int sh_mem_semaphores;
	UCHAR *sh_mem_address;
	SLONG sh_mem_length_mapped;
	SLONG sh_mem_mutex_arg;
	SLONG sh_mem_handle;
};
typedef sh_mem SH_MEM_T;
typedef sh_mem *SH_MEM;
#endif /* UNIX */


#ifdef WIN_NT
#define MTX_STRUCTURE_DEFINED
struct mtx
{
	void*	mtx_handle;
};

typedef mtx MTX_T;
typedef mtx *MTX;

struct event_t
{
	SLONG		event_pid;
	SLONG		event_count;
	SLONG		event_type;
	void*		event_handle;
	event_t*	event_shared;
};

#define SH_MEM_STRUCTURE_DEFINED
struct sh_mem
{
	UCHAR*	sh_mem_address;
	SLONG	sh_mem_length_mapped;
	SLONG	sh_mem_mutex_arg;
	void*	sh_mem_handle;
	void*	sh_mem_object;
	void*	sh_mem_interest;
	void*	sh_mem_hdr_object;
	SLONG*	sh_mem_hdr_address;
	TEXT	sh_mem_name[256];
};
typedef sh_mem SH_MEM_T;
typedef sh_mem *SH_MEM;
#define THREAD_HANDLE_DEFINED
typedef void *THD_T;
#endif


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
	SLONG	sh_mem_mutex_arg;
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


/* Interprocess communication configuration structure */

struct ipccfg
{
	const char*	ipccfg_keyword;
	SCHAR		ipccfg_key;
	SLONG*		ipccfg_variable;
	SSHORT		ipccfg_parent_offset;	/* Relative offset of parent keyword */
	USHORT		ipccfg_found;		/* TRUE when keyword has been set */
};

typedef ipccfg *IPCCFG;

/* AST thread scheduling macros */

#ifdef AST_THREAD
inline void AST_ALLOC() {
	SCH_ast(AST_alloc);
}
inline void AST_INIT() {
	SCH_ast(AST_init);
}
inline void AST_FINI() {
	SCH_ast(AST_fini);
}
inline void AST_CHECK() {
	SCH_ast(AST_check);
}
inline void AST_DISABLE() {
	SCH_ast(AST_disable);
}
inline void AST_ENABLE() {
	SCH_ast(AST_enable);
}
inline void AST_ENTER() {
	SCH_ast(AST_enter);
}
inline void AST_EXIT() {
	SCH_ast(AST_exit);
}
#else
inline void AST_ALLOC() {
}
inline void AST_INIT() {
}
inline void AST_FINI() {
}
inline void AST_CHECK() {
}
inline void AST_DISABLE() {
}
inline void AST_ENABLE() {
}
inline void AST_ENTER() {
}
inline void AST_EXIT() {
}
#endif


#endif /* JRD_ISC_H */

