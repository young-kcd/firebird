/*
 *	PROGRAM:	UNIX resource removal program
 *	MODULE:		dropv3.c
 *	DESCRIPTION:	Version 3 drop shared memory and semaphores
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

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <errno.h>
#include <sys/param.h>
#ifdef IMP
#include <sys/types.h>
#endif
#include <sys/stat.h>
#include <stdlib.h>

#include "../jrd/common.h"
#include "../jrd/isc.h"
#if ( defined( sun) && defined( sparc) && !defined( SOLARIS))
#include "../lock/lockv3s4.h"
#define SUN_V3_LOCK_MANAGER
#else
#include "../lock/lockv3.h"
#endif
#include "../jrd/license.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../utilities/drpv3_proto.h"

#define FTOK_KEY		15

static long get_key();
static int dummy_init();
static int get_lock_header();
static int sem_exclusive(long , int );
#ifndef MMAP_SUPPORTED   
static int remove_resource(int , TEXT *, int, int , TEXT *);
#else
static int remove_resource(int, TEXT *,int ,int ,TEXT *);
#endif

static int LOCK_shm_size, LOCK_sem_key, LOCK_blk_signal, LOCK_sem_count;

static struct {
	char *tptr;
	int *vptr;
} LOCK_hdrtbl[] = {
#ifndef MMAP_SUPPORTED
	{"SHMSIZE", &LOCK_shm_size},
#else
	{"SEMKEY", &LOCK_sem_key}, 
    {"BLKSIG", &LOCK_blk_signal},
#endif
    {"SEMCOUNT", &LOCK_sem_count}, 
    {NULL, NULL}
};


int V3_drop(int argc, UCHAR **argv)
{
/**************************************
 *
 *	V 3 _ d r o p
 *
 **************************************
 *
 * Functional description
 *	Drop Lock Table and associated semaphores.	
 *
 **************************************/
	UCHAR **end, *p;
	int sw_csv, sw_lockmngr, sw_events, sw_version;

	sw_csv = sw_lockmngr = sw_events = sw_version = FALSE;
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

				case 'Z':
					sw_version = TRUE;
					break;

				default:
					ib_printf("***Ignoring unknown switch %c.\n", *p);
					break;
				}

	if (sw_version)
		ib_printf("gds_drop version %s\n", GDS_VERSION);

	if (sw_events)
		remove_resource(FALSE, EVENT_FILE, EVENT_DEFAULT_SIZE,
						EVENT_SEMAPHORES, "events");

	if (sw_lockmngr) {
		get_lock_header();
		remove_resource(TRUE, LOCK_FILE, LOCK_shm_size, LOCK_sem_count,
						"lock manager");
	}

#ifdef CSV
	if (sw_csv)
		remove_resource(FALSE, CSI_FILE, CSI_DEFAULT_SIZE, MAX_PROCESSES,
						"central server");
#endif

	exit(FINI_OK);
}


static int dummy_init()
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


static long get_key(TEXT *filename)
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


static int get_lock_header()
{
/*************************************
 *
 *	g e t _ l o c k _ h e a d e r
 *
 *************************************
 *
 * Functional description
 *	Read user-defined values for shared memory
 *	and semaphores from LOCK_HEADER.
 *
 ************************************/
	IB_FILE *fd;
	TEXT lock_header[MAXPATHLEN];
	char *p, *q, buf[40];
	int i;

#ifndef MMAP_SUPPORTED
	LOCK_shm_size = DEFAULT_SIZE;
#else
#ifdef SUN_V3_LOCK_MANAGER
	LOCK_sem_key = SEM_KEY;
#endif
	LOCK_blk_signal = BLOCKING_SIGNAL;
#endif
	LOCK_sem_count = SEMAPHORES;

	if (fd = ib_fopen(LOCK_HEADER, "r")) {
		while (p = ib_fgets(buf, sizeof(buf) - 1, fd))
			for (i = 0; q = LOCK_hdrtbl[i].tptr; i++) {
				p = buf;
				while (*p++ == *q++);
				if (!*--q) {
					*LOCK_hdrtbl[i].vptr = atoi(p);
					break;
				}
			}
		ib_fclose(fd);
	}
}


#ifndef MMAP_SUPPORTED
static int remove_resource(int lock_flag, TEXT *filename, int shm_length,
               int sem_count, TEXT *label)
{
/**************************************
 *
 *	r e m o v e _ r e s o u r c e		( G E N E R I C )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	STATUS status_vector[20];
	long length, key, shmid, semid;
	SH_MEM_T shmem;
	TEXT expanded_filename[512], hostname[64];
	struct stat stat_buf;

#ifdef NOHOSTNAME
	strcpy(expanded_filename, filename);
#else
	sprintf(expanded_filename, filename,
			ISC_get_host(hostname, sizeof(hostname)));
#endif
	if (stat(expanded_filename, &stat_buf) == -1) {
		ib_printf("\n***No bridge %s file found.\n", label);
		return;
	}

	shmem.sh_mem_semaphores = sem_count;
	if (!ISC_map_file
		(status_vector, filename, dummy_init, 0, shm_length, &shmem)) {
		ib_printf("\n***Unable to access bridge %s resources:\n", label);
		return;
	}

	if ((key = get_key(filename)) == -1) {
		ib_printf("\n***Unable to get the key value of the bridge %s file.\n",
				  label);
		return;
	}

	if ((shmid = shm_exclusive(key, shmem.sh_mem_length_mapped)) == -1 ||
		(semid = sem_exclusive(key, sem_count)) == -1) {
		ib_printf
			("\n***File or semaphores for bridge %s are currently in use.\n",
			 label);
		return;
	}

	if (shmctl(shmid, IPC_RMID, 0) == -1)
		ib_printf("\n***Error trying to drop bridge %s file.  ERRNO = %d.\n",
				  label, errno);
	else
		ib_printf("Successfully removed %s file.\n", label);

	if (semctl(semid, sem_count, IPC_RMID, 0) == -1)
		ib_printf
			("\n***Error trying to drop bridge %s semaphores.  ERRNO = %d.\n",
			 label, errno);
	else
		ib_printf("Successfully removed bridge %s semaphores.\n", label);
}
#endif


#ifdef MMAP_SUPPORTED
static int remove_resource(int lock_flag, TEXT *filename, int shm_length,
              int sem_count, TEXT *label)
{
/**************************************
 *
 *	r e m o v e _ r e s o u r c e		( S U N O S 4 )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	STATUS status_vector[20];
	long length, key, shmid, semid;
	TEXT expanded_filename[512], hostname[64];
	struct stat stat_buf;
	#ifdef DARWIN
	union semun	semctlArg;
	#endif

#ifdef NOHOSTNAME
	strcpy(expanded_filename, filename);
#else
	sprintf(expanded_filename, filename,
			ISC_get_host(hostname, sizeof(hostname)));
#endif
	if (stat(expanded_filename, &stat_buf) == -1) {
		ib_printf("\n***No bridge %s file found.\n", label);
		return 0;
	}

#ifdef SUN_V3_LOCK_MANAGER
	if (lock_flag)
		key = LOCK_sem_key;
	else
#endif
	if ((key = get_key(filename)) == -1) {
		ib_printf("\n***Unable to get the key value of the bridge %s file.\n",
				  label);
		return 0;
	}

	if ((semid = sem_exclusive(key, sem_count)) == -1) {
		ib_printf("\n***Semaphores for bridge %s are currently in use.\n",
				  label);
		return 0;
	}

	#ifdef DARWIN
	semctlArg.val = 0;
	if (semctl(semid, sem_count, IPC_RMID, semctlArg) == -1)
	#else
	if (semctl(semid, sem_count, IPC_RMID, 0) == -1)
	#endif
		ib_printf
			("\n***Error trying to drop bridge %s semaphores.  ERRNO = %d.\n",
			 label, errno);
	else
		ib_printf("Successfully removed bridge %s semaphores.\n", label);
}
#endif


static int sem_exclusive(long key, int count)
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

#ifndef MMAP_SUPPORTED
	return semget(key, count, IPC_EXCL);
#else
	if ((semid = semget(key, count, IPC_EXCL)) != -1)
		return semid;
	else
		return semget(key, count, IPC_EXCL | IPC_CREAT);
#endif
}


#ifndef MMAP_SUPPORTED
static int shm_exclusive(long key, int length)
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

	if ((id = shmget(key, length, IPC_ALLOC)) == -1 ||
		shmctl(id, IPC_STAT, &buf) == -1 || buf.shm_nattch != 1)
		return -1;

	return id;
}
#endif
