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
/*
$Id: isc.h,v 1.6 2003-03-23 16:50:52 alexpeshkoff Exp $
*/
#ifndef _JRD_ISC_H_
#define _JRD_ISC_H_

#ifdef __cplusplus
extern "C" {
#endif


/* Defines for semaphore and shared memory removal */

#define ISC_SEM_REMOVE    1
#define ISC_MEM_REMOVE    2

/* Defines for tokens in config file */

#define	ISCCFG_LOCKMEM		"V4_LOCK_MEM_SIZE"
#define	ISCCFG_LOCKMEM_DEF	98304

#define ISCCFG_LOCKSEM		"V4_LOCK_SEM_COUNT"
#ifdef SINIXZ
#define ISCCFG_LOCKSEM_DEF	25
#else
#define ISCCFG_LOCKSEM_DEF	32
#endif

#define ISCCFG_LOCKSIG		"V4_LOCK_SIGNAL"
#define ISCCFG_LOCKSIG_DEF	16

#define ISCCFG_EVNTMEM		"V4_EVENT_MEM_SIZE"
#define ISCCFG_EVNTMEM_DEF	32768

#define ISCCFG_DBCACHE		"DATABASE_CACHE_PAGES"
#ifdef SUPERSERVER
#define ISCCFG_DBCACHE_DEF	2048
#endif
#ifndef ISCCFG_DBCACHE_DEF
#define ISCCFG_DBCACHE_DEF	75
#endif

#define ISCCFG_PRIORITY		"SERVER_PRIORITY_CLASS"
#define ISCCFG_PRIORITY_DEF	1

#define ISCCFG_IPCMAP		"SERVER_CLIENT_MAPPING"
#define ISCCFG_IPCMAP_DEF	4096

#define ISCCFG_MEMMIN		"SERVER_WORKING_SIZE_MIN"
#define ISCCFG_MEMMIN_DEF	0

#define ISCCFG_MEMMAX		"SERVER_WORKING_SIZE_MAX"
#define ISCCFG_MEMMAX_DEF	0

#define	ISCCFG_LOCKORDER	"V4_LOCK_GRANT_ORDER"
#define	ISCCFG_LOCKORDER_DEF	1

#define	ISCCFG_ANYLOCKMEM	"ANY_LOCK_MEM_SIZE"
#define	ISCCFG_ANYLOCKMEM_DEF	98304

#define ISCCFG_ANYLOCKSEM	"ANY_LOCK_SEM_COUNT"
#define ISCCFG_ANYLOCKSEM_DEF	32

#define ISCCFG_ANYLOCKSIG	"ANY_LOCK_SIGNAL"
#define ISCCFG_ANYLOCKSIG_DEF	16

#define ISCCFG_ANYEVNTMEM	"ANY_EVENT_MEM_SIZE"
#define ISCCFG_ANYEVNTMEM_DEF	32768

#define ISCCFG_LOCKHASH		"LOCK_HASH_SLOTS"
#define ISCCFG_LOCKHASH_DEF	101

#define ISCCFG_DEADLOCK		"DEADLOCK_TIMEOUT"
#define ISCCFG_DEADLOCK_DEF	10

#define ISCCFG_LOCKSPIN		"LOCK_ACQUIRE_SPINS"
#define ISCCFG_LOCKSPIN_DEF	0

#define ISCCFG_CONN_TIMEOUT	"CONNECTION_TIMEOUT"
#define ISCCFG_CONN_TIMEOUT_DEF	180	/* seconds */

#define ISCCFG_DUMMY_INTRVL	"DUMMY_PACKET_INTERVAL"
#define ISCCFG_DUMMY_INTRVL_DEF	60	/* seconds */

#define ISCCFG_TMPDIR		"TMP_DIRECTORY"

#define ISCCFG_EXT_FUNC_DIR	"EXTERNAL_FUNCTION_DIRECTORY"

#define ISCCFG_TRACE_POOLS	"TRACE_MEMORY_POOLS"	/* Internal Use only */
#define ISCCFG_TRACE_POOLS_DEF	0	/* Off -  Internal Use only */

#define ISCCFG_REMOTE_BUFFER	"TCP_REMOTE_BUFFER"
#define ISCCFG_REMOTE_BUFFER_DEF	8192	/* xdr buffer size */

#ifdef SET_TCP_NO_DELAY
#define ISCCFG_NO_NAGLE	"TCP_NO_NAGLE"
#define ISCCFG_NO_NAGLE_DEF	0	/*Disable Nagle Algorithm */
#endif

#ifdef WIN_NT
#define ISCCFG_CPU_AFFINITY_TAG		"CPU_AFFINITY"
#define ISCCFG_CPU_AFFINITY_DEF		1	/* bit 0 set, only first CPU */
#endif


typedef struct cfgtbl {
	TEXT *cfgtbl_keyword;
	UCHAR cfgtbl_key;
	ULONG cfgtbl_value;
	ULONG cfgtbl_def_value;
} CFGTBL;

/* InterBase platform-specific synchronization data structures */

#ifdef VMS
typedef struct itm {
	SSHORT itm_length;
	SSHORT itm_code;
	SCHAR *itm_buffer;
	SSHORT *itm_return_length;
} ITM;

typedef struct event {
	SLONG event_pid;
	SLONG event_count;
} EVENT_T, *EVENT;

typedef struct wait {
	USHORT wait_count;
	EVENT wait_events;
	SLONG *wait_values;
} WAIT;

/* Lock status block */

typedef struct lksb {
	SSHORT lksb_status;
	SSHORT lksb_reserved;
	SLONG lksb_lock_id;
	SLONG lksb_value[4];
} LKSB;

/* Poke block (for asynchronous poking) */

typedef struct poke {
	struct poke *poke_next;
	LKSB poke_lksb;
	SLONG poke_parent_id;
	USHORT poke_value;
	USHORT poke_use_count;
} *POKE;

#define SH_MEM_STRUCTURE_DEFINED
typedef struct sh_mem {
	int sh_mem_system_flag;
	UCHAR *sh_mem_address;
	SLONG sh_mem_length_mapped;
	SLONG sh_mem_mutex_arg;
	SLONG sh_mem_handle;
	SLONG sh_mem_retadr[2];
	SLONG sh_mem_channel;
	TEXT sh_mem_filename[128];
} SH_MEM_T, *SH_MEM;
#endif


#ifdef UNIX
#define MTX_STRUCTURE_DEFINED

#ifdef __cplusplus
} /* extern "C" */
#endif

#include "../jrd/thd.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef ANY_THREADING
typedef struct mtx {
	THD_MUTEX_STRUCT mtx_mutex[1];
} MTX_T, *MTX;
#else
typedef struct mtx {
	SLONG mtx_semid;
	SSHORT mtx_semnum;
	SCHAR mtx_use_count;
	SCHAR mtx_wait;
} MTX_T, *MTX;
#endif /* ANY_THREADING */


#ifdef ANY_THREADING
typedef struct event
{
	SLONG event_semid;
	SLONG event_count;
	THD_MUTEX_STRUCT event_mutex[1];
	THD_COND_STRUCT event_semnum[1];
} EVENT_T, *EVENT;
#else
typedef struct event
{
	SLONG event_semid;
	SLONG event_count;
	SSHORT event_semnum;
} EVENT_T, *EVENT;
#endif /* ANY_THREADING */


#define SH_MEM_STRUCTURE_DEFINED
typedef struct sh_mem
{
	int sh_mem_semaphores;
	UCHAR *sh_mem_address;
	SLONG sh_mem_length_mapped;
	SLONG sh_mem_mutex_arg;
	SLONG sh_mem_handle;
} SH_MEM_T, *SH_MEM;
#endif /* UNIX */


#ifdef WIN_NT
#define MTX_STRUCTURE_DEFINED
typedef struct mtx
{
	void*	mtx_handle;
} MTX_T, *MTX;

typedef struct event
{
	SLONG			event_pid;
	SLONG			event_count;
	SLONG			event_type;
	void*			event_handle;
	struct event*	event_shared;
} EVENT_T, *EVENT;

#define SH_MEM_STRUCTURE_DEFINED
typedef struct sh_mem
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
} SH_MEM_T, *SH_MEM;

#define THREAD_HANDLE_DEFINED
typedef void *THD_T;
#endif


#ifndef MTX_STRUCTURE_DEFINED
#define MTX_STRUCTURE_DEFINED
typedef struct mtx
{
	SSHORT	mtx_event_count[3];
	SCHAR	mtx_use_count;
	SCHAR	mtx_wait;
} MTX_T, *MTX;
#endif
#undef MTX_STRUCTURE_DEFINED

#ifndef SH_MEM_STRUCTURE_DEFINED
#define SH_MEM_STRUCTURE_DEFINED
typedef struct sh_mem
{
	UCHAR*	sh_mem_address;
	SLONG	sh_mem_length_mapped;
	SLONG	sh_mem_mutex_arg;
	SLONG	sh_mem_handle;
} SH_MEM_T, *SH_MEM;
#endif
#undef SH_MEM_STRUCTURE_DEFINED

#ifndef THREAD_HANDLE_DEFINED
#define THREAD_HANDLE_DEFINED
typedef ULONG THD_T;
#endif
#undef THREAD_HANDLE_DEFINED


/* Interprocess communication configuration structure */

typedef struct ipccfg
{
	const char*	ipccfg_keyword;
	SCHAR		ipccfg_key;
	SLONG*		ipccfg_variable;
	SSHORT		ipccfg_parent_offset;	/* Relative offset of parent keyword */
	USHORT		ipccfg_found;		/* TRUE when keyword has been set */
} *IPCCFG;

/* AST actions taken by SCH_ast() */

enum ast_t
{
	AST_alloc,
	AST_init,
	AST_fini,
	AST_check,
	AST_disable,
	AST_enable,
	AST_enter,
	AST_exit
};

/* AST thread scheduling macros */

#ifdef AST_THREAD
#define AST_ALLOC	SCH_ast (AST_alloc)
#define AST_INIT	SCH_ast (AST_init)
#define AST_FINI	SCH_ast (AST_fini)
#define AST_CHECK	SCH_ast (AST_check)
#define AST_DISABLE	SCH_ast (AST_disable)
#define AST_ENABLE	SCH_ast (AST_enable)
#define AST_ENTER	SCH_ast (AST_enter)
#define AST_EXIT	SCH_ast (AST_exit)
#else
#define AST_ALLOC
#define AST_INIT
#define AST_FINI
#define AST_CHECK
#define AST_DISABLE
#define AST_ENABLE
#define AST_ENTER
#define AST_EXIT
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _JRD_ISC_H_ */
