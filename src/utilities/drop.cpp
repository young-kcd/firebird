/*
 *	PROGRAM:	UNIX resource removal program
 *	MODULE:		drop.c
 *	DESCRIPTION:	Drop shared memory and semaphores
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
 */
/*
$Id: drop.cpp,v 1.7 2002-08-26 12:10:19 eku Exp $
*/

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <errno.h>
#include <sys/param.h>
#if ( defined DELTA || defined IMP)
#include <sys/types.h>
#endif
#include <sys/stat.h>

#include "../jrd/common.h"
#include "../jrd/isc.h"
#include "../lock/lock.h"
#include "../lock/lock_proto.h"
#include "../jrd/license.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../utilities/drpv3_proto.h"

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#define FTOK_KEY		15

static void dummy_init(void);
static SLONG get_key(TEXT *);
static void remove_resource(TEXT *, SLONG, SLONG, TEXT *);
static int sem_exclusive(SLONG, SLONG);
static int shm_exclusive(SLONG, SLONG);
#ifdef MANAGER_PROCESS
static void shut_manager(TEXT *);
#endif

static SLONG LOCK_shm_size, LOCK_sem_count;
static SLONG EVENT_default_size;
static int orig_argc;
static SCHAR **orig_argv;

static struct ipccfg config_table[] = {
#ifndef HAVE_MMAP
	{"V4_LOCK_MEM_SIZE", -1, &LOCK_shm_size, 0, 0},
	{"ANY_LOCK_MEM_SIZE", -1, &LOCK_shm_size, -1, 0},
	{"V4_EVENT_MEM_SIZE", -1, &EVENT_default_size, 0, 0},
	{"ANY_EVENT_MEM_SIZE", -1, &EVENT_default_size, -1, 0},
#endif
#ifndef NO_SEMAPHORES
	{"V4_LOCK_SEM_COUNT", -1, &LOCK_sem_count, 0, 0},
	{"ANY_LOCK_SEM_COUNT", -1, &LOCK_sem_count, -1, 0},
#endif
     {NULL, -1, NULL, 0, 0}
};


int CLIB_ROUTINE main( int argc, char *argv[])
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Drop Lock Table and associated semaphores.	
 *
 **************************************/
	SCHAR **end, *p;
	BOOLEAN sw_csv, sw_lockmngr, sw_events, sw_version, sw_nobridge,
		sw_shutmngr;

	orig_argc = argc;
	orig_argv = argv;

	sw_csv = sw_lockmngr = sw_events = sw_version = sw_nobridge =
		sw_shutmngr = FALSE;
	end = argv + argc;
	while (++argv < end)
		if (**argv == '-')
			for (p = *argv + 1; *p; p++)
				switch (UPPER(*p)) {
#ifdef CSV
				case 'C':
					sw_csv = TRUE;
					break;
#endif

				case 'E':
					sw_events = TRUE;
					break;

				case 'L':
					sw_lockmngr = TRUE;
					break;

				case 'A':
					sw_csv = sw_events = sw_lockmngr = TRUE;
					break;

				case 'S':
					sw_shutmngr = sw_nobridge = TRUE;
					break;

				case 'N':
					sw_nobridge = TRUE;
					break;

				case 'Z':
					sw_version = TRUE;
					break;

				default:
					ib_printf("***Ignoring unknown switch %c.\n", *p);
					break;
				}

	if (sw_version)
		ib_printf("gds_drop version %s\n", GDS_VERSION);

	LOCK_shm_size = DEFAULT_SIZE;
	LOCK_sem_count = SEMAPHORES;
	EVENT_default_size = EVENT_DEFAULT_SIZE;
	ISC_get_config(LOCK_HEADER, config_table);

	if (sw_events)
		remove_resource(EVENT_FILE, EVENT_default_size, EVENT_SEMAPHORES,
						"events");

	if (sw_lockmngr)
		remove_resource(LOCK_FILE, LOCK_shm_size, LOCK_sem_count,
						"lock manager");

#ifdef CSV
	if (sw_csv)
		remove_resource(CSI_FILE, CSI_DEFAULT_SIZE, MAX_PROCESSES,
						"central server");
#endif

#ifdef MANAGER_PROCESS
	if (sw_shutmngr)
		shut_manager("lock manager");
#endif

#if !(defined WIN_NT || defined NETWARE_386 || defined linux || defined FREEBSD || defined NETBSD || defined AIX_PPC || defined DARWIN)
	if (!sw_nobridge) {
		ib_printf("\nBRIDGE RESOURCES\n\n");
		V3_drop(orig_argc, orig_argv);
	}
#endif

	exit(FINI_OK);
}


static void dummy_init(void)
{
/**************************************
 *
 *	d u m m y _ i n i t
 *
 **************************************
 *
 * Functional description
 *	A dummy callback routine for ISC_map_file.
 *
 **************************************/
}


static SLONG get_key( TEXT * filename)
{
/*************************************
 *
 *	g e t _ k e y
 *
 *************************************
 *
 * Functional description
 *	Find the semaphore/shared memory key for a file.
 *
 ************************************/
	TEXT expanded_filename[128], hostname[64];

#ifdef NOHOSTNAME
	strcpy(expanded_filename, filename);
#else
	sprintf(expanded_filename, filename,
			ISC_get_host(hostname, sizeof(hostname)));
#endif

/* Produce shared memory key for file */

	return ftok(expanded_filename, FTOK_KEY);
}


#ifndef HAVE_MMAP
static void remove_resource(
							TEXT * filename,
							SLONG shm_length, SLONG sem_count, TEXT * label)
{
/**************************************
 *
 *	r e m o v e _ r e s o u r c e		( n o n - m m a p )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	STATUS status_vector[20];
	SH_MEM_T shmem_data;
	SLONG key, shmid, semid;
	TEXT expanded_filename[MAXPATHLEN];
	pid_t pid;

#ifdef MANAGER_PROCESS
/* Shutdown lock manager process so that shared memory
   and semaphores can be attached exclusively. */

	if (!(strcmp(label, "lock manager"))) {
		if (!(pid = vfork())) {
			execl(orig_argv[0], orig_argv[0], "-s", 0);
			_exit(FINI_ERROR);
		}
        // wait(pid)  // this is wrong wait takes a *int anyway.
		waitpid(pid, 0, 0);
	}
#endif

	gds__prefix_lock(expanded_filename, filename);

	shmem_data.sh_mem_semaphores = sem_count;
	if (!ISC_map_file
		(status_vector, expanded_filename, dummy_init, 0, shm_length,
		 &shmem_data)) {
		ib_printf("\n***Unable to access %s resources:\n", label);
		gds__print_status(status_vector);
		return;
	}

	if ((key = get_key(expanded_filename)) == -1) {
		ib_printf("\n***Unable to get the key value of the %s file.\n",
				  label);
		return;
	}

	if ((shmid = shm_exclusive(key, shmem_data.sh_mem_length_mapped)) == -1 ||
		(semid = sem_exclusive(key, sem_count)) == -1) {
		ib_printf("\n***File or semaphores for %s are currently in use.\n",
				  label);
		return;
	}

	if (shmctl(shmid, IPC_RMID, 0) == -1)
		ib_printf("\n***Error trying to drop %s file.  ERRNO = %d.\n", label,
				  errno);
	else
		ib_printf("Successfully removed %s file.\n", label);

	if (semctl(semid, sem_count, IPC_RMID, 0) == -1)
		ib_printf("\n***Error trying to drop %s semaphores.  ERRNO = %d.\n",
				  label, errno);
	else
		ib_printf("Successfully removed %s semaphores.\n", label);
}
#endif


#ifdef HAVE_MMAP
static void remove_resource(
							TEXT * filename,
							SLONG shm_length, SLONG sem_count, TEXT * label)
{
/**************************************
 *
 *	r e m o v e _ r e s o u r c e		( m m a p )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	STATUS status_vector[20];
	SLONG length, key, semid;
	TEXT expanded_filename[MAXPATHLEN];
	int pid;
	#ifdef DARWIN
	union  semun semctlArg;
	#endif

#ifdef MANAGER_PROCESS
/* Shutdown lock manager process so that shared memory
   and semaphores can be attached exclusively. */

	if (!(strcmp(label, "lock manager"))) {
		if (!(pid = vfork())) {
			execl(orig_argv[0], orig_argv[0], "-s", 0);
			_exit(FINI_ERROR);
		}
#ifdef SCO_EV					/* 5.5 SCO Port: SCO needs waitpid() to function properly */
		waitpid(pid, 0, 0);
#else
		waitpid(pid,NULL,0);
#endif
	}
#endif

	gds__prefix_lock(expanded_filename, filename);

	if ((key = get_key(expanded_filename)) == -1) {
		ib_printf("\n***Unable to get the key value of the %s file.\n",
				  label);
		return;
	}

	if ((semid = sem_exclusive(key, sem_count)) == -1) {
		ib_printf("\n***Semaphores for %s are currently in use.\n", label);
		return;
	}

	#ifdef DARWIN
	semctlArg.val = 0;
	if (semctl(semid, sem_count, IPC_RMID, semctlArg) == -1)
	#else
	if (semctl(semid, sem_count, IPC_RMID, 0) == -1)
	#endif
		ib_printf("\n***Error trying to drop %s semaphores.  ERRNO = %d.\n",
				  label, errno);
	else
		ib_printf("Successfully removed %s semaphores.\n", label);
}
#endif


static int sem_exclusive( SLONG key, SLONG count)
{
/**************************************
 *
 *	s e m  _ e x c l u s i v e
 *
 **************************************
 *
 * Functional description
 *	Check to see if we are the only ones accessing
 *	semaphores.  Return a semaphore id if so,
 *	-1 otherwise.
 *
 **************************************/
	int semid;

#if !(defined SUNOS4 || defined linux || defined FREEBSD || defined NETBSD || defined SINIXZ)
	return semget(key, (int) count, IPC_EXCL);
#else
	if ((semid = semget(key, (int) count, IPC_EXCL)) != -1)
		return semid;
	else
		return semget(key, (int) count, IPC_EXCL | IPC_CREAT);
#endif
}


#ifndef HAVE_MMAP
static int shm_exclusive( SLONG key, SLONG length)
{
/**************************************
 *
 *	s h m _ e x c l u s i v e
 *
 **************************************
 *
 * Functional description
 *	Check to see if we are the only ones accessing
 *	shared memory.  Return a shared memory id
 *	if so, -1 otherwise.
 *
 **************************************/
	int id;
	struct shmid_ds buf;

	if ((id = shmget(key, (int) length, IPC_ALLOC)) == -1 ||
		shmctl(id, IPC_STAT, &buf) == -1 || buf.shm_nattch != 1)
		return -1;

	return id;
}
#endif


#ifdef MANAGER_PROCESS
static void shut_manager( TEXT * label)
{
/**************************************
 *
 *	s h u t _ m a n a g e r
 *
 **************************************
 *
 * Functional description
 *	Shut down the lock manager process preparatory
 *	to releasing shared resources.
 *
 **************************************/
	SLONG status_vector[20], owner_handle;

	if (!(strcmp(label, "lock manager"))) {
		owner_handle = 0;
		LOCK_init(status_vector, FALSE, getpid(), 0, &owner_handle);

		/* In case if lock manager is not running, LOCK_init starts it.
		   It takes time for the manager to be started, hence immediately
		   following LOCK_shut_manager can not shut it down, because there is
		   no manager running yet. To avoid this situation sleep for
		   a second before calling LOCK_shut_manager */
		sleep(1);

		LOCK_shut_manager();
	}
}
#endif
