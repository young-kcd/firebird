/*
 *	PROGRAM:	UNIX resource removal program
 *	MODULE:		drop.cpp
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
 *
 * $Id: drop.cpp,v 1.30 2004-11-24 09:14:32 robocop Exp $
 *
 * 2002.10.27 Sean Leyne - Completed removal of obsolete "DELTA" port
 * 2002.10.27 Sean Leyne - Completed removal of obsolete "IMP" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
*/

#include "firebird.h"
#include <stdio.h>
#include <errno.h>
#include <sys/param.h>
#include <sys/stat.h>

#include "../jrd/common.h"
#include "../jrd/isc.h"
#include "../lock/lock.h"
#include "../lock/lock_proto.h"
#include "../jrd/license.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../common/config/config.h"

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#if HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

const int FTOK_KEY		= 15;

#ifndef HAVE_MMAP
static void dummy_init(void);
#endif
static SLONG get_key(TEXT *);
static void remove_resource(const TEXT*, SLONG, SLONG, const TEXT*);
static int sem_exclusive(SLONG, SLONG);
#ifndef HAVE_MMAP
static int shm_exclusive(SLONG, SLONG);
#endif
#ifdef MANAGER_PROCESS
static void shut_manager(const TEXT*);
#endif

static int orig_argc;
static SCHAR **orig_argv;


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
	bool sw_lockmngr = false;
	bool sw_events = false;
	bool sw_version = false;
	bool sw_nobridge = false;
	bool sw_shutmngr = false;

	orig_argc = argc;
	orig_argv = argv;

	SCHAR** const end = argv + argc;
	while (++argv < end)
		if (**argv == '-')
			for (const char* p = *argv + 1; *p; p++)
				switch (UPPER(*p)) {

				case 'E':
					sw_events = true;
					break;

				case 'L':
					sw_lockmngr = true;
					break;

				case 'A':
					sw_events = sw_lockmngr = true;
					break;

				case 'S':
					sw_shutmngr = sw_nobridge = true;
					break;

				case 'N':
					sw_nobridge = true;
					break;

				case 'Z':
					sw_version = true;
					break;

				default:
					printf("***Ignoring unknown switch %c.\n", *p);
					break;
				}

	if (sw_version)
		printf("gds_drop version %s\n", GDS_VERSION);

	if (sw_events)
		remove_resource(EVENT_FILE, Config::getEventMemSize(), EVENT_SEMAPHORES,
						"events");

	if (sw_lockmngr)
		remove_resource(LOCK_FILE, Config::getLockMemSize(), Config::getLockSemCount(),
						"lock manager");

#ifdef MANAGER_PROCESS
	if (sw_shutmngr)
		shut_manager("lock manager");
#endif

	exit(FINI_OK);
}


#ifndef HAVE_MMAP
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
#endif

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
							const TEXT* filename,
							SLONG shm_length, SLONG sem_count, const TEXT* label)
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
	ISC_STATUS_ARRAY status_vector;
	SH_MEM_T shmem_data;
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
		 &shmem_data))
	{
		printf("\n***Unable to access %s resources:\n", label);
		gds__print_status(status_vector);
		return;
	}

	const SLONG key = get_key(expanded_filename);
	if (key == -1) {
		printf("\n***Unable to get the key value of the %s file.\n",
				  label);
		return;
	}

	SLONG shmid, semid;
	if ((shmid = shm_exclusive(key, shmem_data.sh_mem_length_mapped)) == -1 ||
		(semid = sem_exclusive(key, sem_count)) == -1) 
	{
		printf("\n***File or semaphores for %s are currently in use.\n",
				  label);
		return;
	}

	if (shmctl(shmid, IPC_RMID, 0) == -1)
		printf("\n***Error trying to drop %s file.  ERRNO = %d.\n", label,
				  errno);
	else
		printf("Successfully removed %s file.\n", label);

	if (semctl(semid, sem_count, IPC_RMID, 0) == -1)
		printf("\n***Error trying to drop %s semaphores.  ERRNO = %d.\n",
				  label, errno);
	else
		printf("Successfully removed %s semaphores.\n", label);
}
#endif


#ifdef HAVE_MMAP
static void remove_resource(
							const TEXT* filename,
							SLONG shm_length, SLONG sem_count, const TEXT* label)
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
	ISC_STATUS_ARRAY status_vector;
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
		waitpid(pid, NULL, 0);
#endif
	}
#endif

	gds__prefix_lock(expanded_filename, filename);

	const SLONG key = get_key(expanded_filename);
	if (key == -1) {
		printf("\n***Unable to get the key value of the %s file.\n",
				  label);
		return;
	}

	const SLONG semid = sem_exclusive(key, sem_count);
	if (semid == -1) {
		printf("\n***Semaphores for %s are currently in use.\n", label);
		return;
	}

#ifdef DARWIN
	semctlArg.val = 0;
	if (semctl(semid, sem_count, IPC_RMID, semctlArg) == -1)
#else
	if (semctl(semid, sem_count, IPC_RMID, 0) == -1)
#endif
		printf("\n***Error trying to drop %s semaphores.  ERRNO = %d.\n",
				  label, errno);
	else
		printf("Successfully removed %s semaphores.\n", label);
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

#if !(defined sun || defined LINUX || defined FREEBSD || defined NETBSD || defined SINIXZ)
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
	struct shmid_ds buf;

	const int id = shmget(key, (int) length, IPC_ALLOC);
	if (id == -1 ||
		shmctl(id, IPC_STAT, &buf) == -1 || buf.shm_nattch != 1)
	{
		return -1;
	}

	return id;
}
#endif


#ifdef MANAGER_PROCESS
static void shut_manager(const TEXT* label)
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
	ISC_STATUS_ARRAY status_vector;
	SLONG owner_handle;

	if (!(strcmp(label, "lock manager"))) {
		owner_handle = 0;
		LOCK_init(status_vector, false, getpid(), 0, &owner_handle);

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

