/*
 *        PROGRAM:        JRD Write Ahead Log APIs
 *        MODULE:         wal.c
 *        DESCRIPTION:    Simple driver for WAL routines
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
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "M88K" port
 *
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <stdlib.h>
#include <string.h>
#include "../jrd/common.h"

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#ifdef WIN_NT
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#ifdef HAVE_SYS_TIMEB_H
# include <sys/timeb.h>
#endif
#include <io.h>
#endif

#include "../jrd/jrd.h"
#include "../jrd/flags.h"
#include "../jrd/dsc.h"
#include "../wal/wal.h"
#include "../jrd/jrn.h"
#include "../wal/wal_proto.h"
#include "../wal/walc_proto.h"
#include "../wal/walf_proto.h"
#include "../wal/walr_proto.h"
#include "../wal/wstat_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_f_proto.h"
#include "../jrd/misc_proto.h"

static void do_mem_benchmark(void);
static void do_sem_benchmark(WAL);
static SLONG get_tod(void);
static bool get_word(SCHAR *);
static void partitioned_logfile_create(ISC_STATUS *, SCHAR *);
static void partitioned_logfile_info(ISC_STATUS *, SCHAR *);
static void print_help(void);
static SSHORT read_log(ISC_STATUS *, SCHAR *, SCHAR *, SLONG);
static void toggle_debug_msg_flag(ISC_STATUS *, WAL);

static ULONG base_seconds;


int CLIB_ROUTINE main( int argc, char **argv)
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	WAL WAL_handle;
	SCHAR dbname[256];
	SCHAR local_dbname[256];
	SCHAR logname[256];			/* Command line logname */
	SCHAR log_name[256];		/* Run-time logname */
	SLONG log_seqno;
	SLONG new_log_seqno;
	SLONG log_partition_offset;
	SLONG log_offset;
	SCHAR buff[1000];
	SLONG ckpt_seqno;
	SLONG ckpt_offset;
	SCHAR ckpt_logname[100];
	SLONG ckpt_log_partition_offset;
	SCHAR dbnum;
	SCHAR shutdown;
	USHORT page_size;

	SCHAR wpb[1000], *w;
	USHORT wpb_len;
	USHORT wal_buf_count;
	USHORT wal_buflen;
	SLONG wal_ckpt_intrvl;
	bool first_time_log;

	SSHORT logcount;

	LGFILE *logfiles[4], *ovf_logfile, *serial_logfile, *logfile;

	SCHAR jrn_dirname[256];
	USHORT jrn_data_len;
	UCHAR jrn_data[MAX_JRN_INIT_DATA];
	int ret;
	SCHAR set;


#define MAXLOGS   32
	SCHAR lognames_buffer[MAXLOGS * MAXPATHLEN];	/* To hold all the lognames */
	SCHAR *logs_names[MAXLOGS];
	SLONG logs_partition_offsets[MAXLOGS];
	SLONG logs_seqnos[MAXLOGS];
	SLONG logs_lengths[MAXLOGS];
	SLONG logs_flags[MAXLOGS];
	SLONG log_flag;
	int log_count, count;
	SLONG grpc_wait_usecs;
	SSHORT only_attach;
	SSHORT direction;
	SCHAR last_logname[MAXPATHLEN];
	SLONG last_log_partition_offset;
	SLONG last_log_flags;
	bool any_log_to_be_archived;

	if (argc < 2) {
		ib_printf
			("\nUsage: driver <database_name> <page size> <logname> <partition_offset> [<new_log_seqno>]\n");
		ib_printf("         driver <database_name>\n");
		exit(FINI_ERROR);
	}
	only_attach = (argc == 2) ? 1 : 0;
	ISC_expand_filename(argv[1], 0, dbname);

	WAL_handle = NULL;
	wal_buf_count = 5;
	wal_buflen = MIN_WALBUFLEN;
	wal_ckpt_intrvl = 100;		/* 100K bytes */
	if (argc == 6) {
		first_time_log = true;
		sscanf(argv[5], "%ld", &new_log_seqno);
	}
	else {
		first_time_log = false;
		/* Set some default value for the log_seqno.  If using an existing
		   log file, log_seqno will be set accordingly by the WAL writer */
		new_log_seqno = 1L;
	}

	if (only_attach)
		ret = WAL_attach(status_vector, &WAL_handle, dbname);
	else {
		logcount = 3;

		logfiles[0] =
			(LGFILE *) gds__alloc(sizeof(LGFILE) + strlen("log1") + 1);
		logfiles[1] =
			(LGFILE *) gds__alloc(sizeof(LGFILE) + strlen("log2") + 1);
		logfiles[2] =
			(LGFILE *) gds__alloc(sizeof(LGFILE) + strlen("part1") + 1);
		/* NOMEM: handled, FREE: don't bother - R&D test code only */
		if (!logfiles[0] || !logfiles[1] || !logfiles[2]) {
			ib_printf("driver: Out of memory\n");
			/* Development code - don't bother to free */
			exit(FINI_ERROR);
		}

		logfile = logfiles[0];
		logfile->lg_size = 100;	/* 100K bytes */
		logfile->lg_partitions = 8;
		logfile->lg_flags = 0;
		strcpy(logfile->lg_name, "log1");

		logfile = logfiles[1];
		logfile->lg_size = 200;	/* 200K bytes */
		logfile->lg_partitions = 4;
		logfile->lg_flags = LOG_raw;
		strcpy(logfile->lg_name, "part1");

		logfile = logfiles[2];
		logfile->lg_size = 100;	/* 100K bytes */
		logfile->lg_partitions = 1;
		logfile->lg_flags = 0;
		strcpy(logfile->lg_name, "log2");

		ovf_logfile =
			(LGFILE *) gds__alloc(sizeof(LGFILE) + strlen("ovflow") + 1);
		if (!ovf_logfile) {
			ib_printf("driver: Out of memory\n");
			/* Development code - don't bother to free */
			exit(FINI_ERROR);
		}
		ovf_logfile->lg_size = 50;	/* 50K bytes */
		ovf_logfile->lg_partitions = 1;
		ovf_logfile->lg_flags = LOG_overflow;
		strcpy(ovf_logfile->lg_name, "ovflow");

		serial_logfile =
			(LGFILE *) gds__alloc(sizeof(LGFILE) + strlen(argv[3]) + 1);
		if (!serial_logfile) {
			ib_printf("driver: Out of memory\n");
			/* Development code - don't bother to free */
			exit(FINI_ERROR);
		}
		serial_logfile->lg_size = 3;	/* 3K bytes */
		serial_logfile->lg_partitions = 1;
		serial_logfile->lg_flags = LOG_serial;
		strcpy(serial_logfile->lg_name, argv[3]);

		strcpy(jrn_dirname, "jrndir");
		jrn_data_len = 5;
		strcpy(jrn_data, "12345");

		grpc_wait_usecs = 300000;

		w = wpb;
		w += MISC_build_parameters_block(w,
										 PARAM_BYTE(WAL_PARAM_BUF_COUNT),
										 PARAM_SHORT(wal_buf_count),
										 PARAM_BYTE(WAL_PARAM_BUF_LEN),
										 PARAM_SHORT(wal_buflen),
										 PARAM_BYTE(WAL_PARAM_CKPT_INTRVL),
										 PARAM_LONG(wal_ckpt_intrvl),
										 (SCHAR) 0);
		w +=
			MISC_build_parameters_block(w, PARAM_BYTE(WAL_PARAM_SERIAL_LOG),
										PARAM_POINTER(serial_logfile),
										(SCHAR) 0);
		w +=
			MISC_build_parameters_block(w,
										PARAM_BYTE(WAL_PARAM_RR_LOGS_COUNT),
										PARAM_SHORT(logcount),
										PARAM_BYTE(WAL_PARAM_RR_LOGS),
										PARAM_POINTER(logfiles), (SCHAR) 0);
		w +=
			MISC_build_parameters_block(w, PARAM_BYTE(WAL_PARAM_OVFLOW_LOG),
										PARAM_POINTER(ovf_logfile),
										(SCHAR) 0);
		w +=
			MISC_build_parameters_block(w,
										PARAM_BYTE(WAL_PARAM_GRPC_WAIT_USECS),
										PARAM_LONG(grpc_wait_usecs),
										(SCHAR) 0);
		/*
		   w += MISC_build_parameters_block(w,
		   PARAM_BYTE(WAL_PARAM_JRN_DIRNAME), PARAM_STRING(jrn_dirname),
		   PARAM_BYTE(WAL_PARAM_JRN_DATA), PARAM_NBYTES(jrn_data_len, jrn_data),
		   (SCHAR)0);
		 */
		w +=
			MISC_build_parameters_block(w, PARAM_BYTE(WAL_PARAM_END),
										(SCHAR) 0);
		wpb_len = w - wpb;

		strcpy(logname, argv[3]);
		WAL_shutdown_old_writer(status_vector, dbname);
		sscanf(argv[2], "%hu", &page_size);
		sscanf(argv[4], "%ld", &log_partition_offset);
		ret = WAL_init(status_vector, &WAL_handle, dbname, page_size,
					   logname, log_partition_offset, first_time_log,
					   new_log_seqno, wpb_len, wpb);
		if (ret == FB_SUCCESS) {
			ret =
				WAL_checkpoint_force(status_vector, WAL_handle, &log_seqno,
									 buff, &log_partition_offset,
									 &log_offset);
			ib_printf
				("After first forced checkpoint, seqno=%d, logname=%s, p_offset=%d, offset=%d\n",
				 log_seqno, buff, log_partition_offset, log_offset);
			WAL_checkpoint_recorded(status_vector, WAL_handle);
		}
	}

	if (ret != FB_SUCCESS) {
		gds__print_status(status_vector);
		exit(FINI_ERROR);
	}

	print_help();
	while (1) {
		ib_printf("WAL>");
		if (get_word(buff) == false)
			break;
		if (!strlen(buff))
			continue;
		if (strcmp(buff, "Q") == 0)
			break;
		if (strcmp(buff, "F") == 0) {
			WAL_flush(status_vector, WAL_handle, &log_seqno, &log_offset, false);
			ib_printf("flushed_seqno=%ld, flushed_offset=%ld\n", log_seqno,
					  log_offset);
		}
		else if (strcmp(buff, "C") == 0) {
			strcpy(buff, "COMMIT");
			WAL_commit(status_vector, WAL_handle, buff, strlen(buff),
					   &log_seqno, &log_offset);
			ib_printf("log_seqno=%ld, log_offset=%ld\n", log_seqno,
					  log_offset);
		}
		else if (strcmp(buff, "K") == 0)
			WAL_checkpoint_finish(status_vector, WAL_handle, &log_seqno, buff,
								  &log_partition_offset, &log_offset);
		else if (strcmp(buff, "V") == 0) {
			WAL_checkpoint_force(status_vector, WAL_handle, &log_seqno, buff,
								 &log_partition_offset, &log_offset);
		}
		else if (strcmp(buff, "W") == 0)
			WSTATUS_dump_wal_segment(WAL_handle, TRUE, ib_stdout);
		else if (strcmp(buff, "Y") == 0)
			WSTATUS_dump_wal_segment(WAL_handle, FALSE, ib_stdout);
		else if (strcmp(buff, "S") == 0) {
			WAL_status(status_vector, WAL_handle,
					   &log_seqno, log_name, &log_partition_offset,
					   &log_offset, &ckpt_seqno, ckpt_logname,
					   &ckpt_log_partition_offset, &ckpt_offset);
			ib_printf
				("log_seqno=%ld, logname=%s, log_partition_offset=%ld, flushed offset=%ld\n",
				 log_seqno, log_name, log_partition_offset, log_offset);
			ib_printf
				("ckpt_log_seqno=%ld, ckpt_logname=%s, ckpt_log_partition_offset=%ld, ckpt_offset=%ld\n",
				 ckpt_seqno, ckpt_logname, ckpt_log_partition_offset,
				 ckpt_offset);
		}
		else if (strcmp(buff, "P") == 0 || strcmp(buff, "N") == 0) {
			direction = ((strcmp(buff, "N") == 0) ? 1 : -1);
			ib_printf("Enter the starting logname: ");
			get_word(log_name);
			ib_printf("Enter the partition offset: ");
			ib_scanf("%ld", &log_partition_offset);
			ib_scanf("%c", &dbnum);	/* get rid of the trailing '\n' character */
			while (1) {
				WALF_get_all_next_logs_info(status_vector, dbname,
											log_name, log_partition_offset,
											MAXLOGS,
											lognames_buffer, &log_count,
											logs_names,
											logs_partition_offsets,
											logs_seqnos, logs_lengths,
											logs_flags, direction);
				for (count = 0; count < log_count; count++) {
					/* Check if the log file can be deleted, e.g. if journalling was
					   enabled when this log file was created, make sure that the file
					   has been backed up or it is no SLONGer needed because there is
					   a newer online dump. etc... */
					ib_printf
						("log name=%s, log_partition_offset=%d, log_seqno=%d, log_length=%d, log_flags=%d\n",
						 logs_names[count], logs_partition_offsets[count],
						 logs_seqnos[count], logs_lengths[count],
						 logs_flags[count]);
				}
				if (log_count < MAXLOGS)
					break;		/* No more logs */
				else {
					/* setup for the next iteration */

					strcpy(log_name, logs_names[0]);
					log_partition_offset = logs_partition_offsets[0];
				}
			}					/* while */
		}
		else if (strcmp(buff, "L") == 0) {
			ib_printf("Enter the starting logname: ");
			get_word(log_name);
			ib_printf("Enter the partition offset: ");
			ib_scanf("%ld", &log_partition_offset);
			ib_scanf("%c", &dbnum);	/* get rid of the trailing '\n' character */
			any_log_to_be_archived = false;
			WALF_get_linked_logs_info(status_vector, dbname,
									  log_name, log_partition_offset,
									  &log_count,
									  last_logname,
									  &last_log_partition_offset,
									  &last_log_flags,
									  &any_log_to_be_archived);
			ib_printf
				("Total previous logs=%d, last log=%s, partition offset=%ld\n",
				 log_count, last_logname, last_log_partition_offset);
			ib_printf("last log flags=%ld, any_log_to_be_archived=%s\n",
					  last_log_flags, any_log_to_be_archived ? "Yes" : "No");
		}
		else if (strcmp(buff, "G") == 0) {
			ib_printf("Enter the logname: ");
			get_word(log_name);
			ib_printf("Enter the partition offset: ");
			ib_scanf("%ld", &log_partition_offset);
			ib_printf("Enter flag value: ");
			ib_scanf("%ld", &log_flag);
			ib_scanf("%c", &dbnum);	/* get rid of the trailing '\n' character */
			ib_printf("S(et) or R(eset)?: ");
			ib_scanf("%c", &set);
			ib_scanf("%c", &dbnum);	/* get rid of the trailing '\n' character */
			WALF_set_log_header_flag(status_vector, dbname, log_name,
									 log_partition_offset, log_flag,
									 (set == 'S'));
		}
		else if (strcmp(buff, "J") == 0) {
			WAL_journal_enable(status_vector, WAL_handle,
							   "/home/dperiwal/wal/test", 0, NULL);
		}
		else if (strcmp(buff, "D") == 0) {
			WAL_journal_disable(status_vector, WAL_handle);
		}
		else if (strcmp(buff, "R") == 0) {
			ib_printf("Enter the logname: ");
			get_word(log_name);
			ib_printf("Enter the partition offset: ");
			ib_scanf("%ld", &log_partition_offset);
			ib_scanf("%c", &dbnum);	/* get rid of the trailing '\n' character */
			read_log(status_vector, dbname, log_name, log_partition_offset);
		}
		else if (strcmp(buff, "M") == 0) {
			partitioned_logfile_create(status_vector, dbname);
		}
		else if (strcmp(buff, "I") == 0) {
			partitioned_logfile_info(status_vector, dbname);
		}
		else if (strcmp(buff, "B") == 0) {
			do_sem_benchmark(WAL_handle);
		}
		else if (strcmp(buff, "E") == 0) {
			ib_printf("Enter the database name: ");
			get_word(local_dbname);
			WAL_shutdown_old_writer(status_vector, local_dbname);
		}
		else if (strcmp(buff, "T") == 0) {
			ib_printf("Enter group-commit wait time in micro-seconds: ");
			ib_scanf("%ld", &grpc_wait_usecs);
			ib_scanf("%c", &dbnum);	/* get rid of the trailing '\n' character */
			WAL_set_grpc_wait_time(status_vector, WAL_handle,
								   grpc_wait_usecs);
		}
		else if (strcmp(buff, "X") == 0) {
			toggle_debug_msg_flag(status_vector, WAL_handle);
		}
		else if (strcmp(buff, "H") == 0) {
			print_help();
		}
		else {
			WAL_put(status_vector, WAL_handle,
					buff, strlen(buff), 0, 0, &log_seqno, &log_offset);
			ib_printf("log_seqno=%ld, log_offset=%ld\n", log_seqno,
					  log_offset);
		}
	}

	WAL_flush(status_vector, WAL_handle, &log_seqno, &log_offset, false);
	ib_printf("\nDo you want to shutdown the WAL writer(s) ? <y/n> ");
	ib_scanf("%c", &shutdown);

	if (shutdown == 'y' || shutdown == 'Y') {
		WAL_shutdown(status_vector, WAL_handle, &log_seqno, log_name,
					 &log_partition_offset, &log_offset, false);
		ib_printf("At shutdown:\n");
		ib_printf
			("log_seqno=%ld, logname=%s, log_partition_offset=%ld, shutdown offset=%ld\n",
			 log_seqno, log_name, log_partition_offset, log_offset);
	}
	WAL_fini(status_vector, &WAL_handle);

	exit(FINI_OK);
}


static void do_mem_benchmark(void)
{
/**************************************
 *
 *	d o _ m e m _ b e n c h m a r k
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	int count, i, dummy;
	SLONG time1, diffusec_loop, diffusec;

	ib_printf("Enter the count: ");
	ib_scanf("%d", &count);
	dummy = 0;

/* First find the time for the empty loop */

	time1 = get_tod();
	for (i = 0; i < count; i++);
	diffusec_loop = get_tod() - time1;
	ib_printf("time interval for empty loop seconds=%d, microseconds=%d\n",
			  diffusec_loop / 1000000, diffusec_loop % 1000000);

/* Now do the iteration of the operation for the given count */

	time1 = get_tod();
	for (i = 0; i < count; i++) {
		dummy++;
		dummy--;
	}
	diffusec = get_tod() - time1;

/* Now get the actual interval for just the operations */

	diffusec = diffusec - diffusec_loop;

	ib_printf("time interval seconds=%d, microseconds=%d\n",
			  diffusec / 1000000, diffusec % 1000000);
}


static void do_sem_benchmark( WAL WAL_handle)
{
/**************************************
 *
 *	d o _ s e m _ b e n c h m a r k
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	int count, i;
	SLONG time1, diffusec_loop, diffusec;
	WALS WAL_segment;

	ib_printf("Enter the count: ");
	ib_scanf("%d", &count);

/* First find the time for the empty loop */

	time1 = get_tod();
	for (i = 0; i < count; i++);
	diffusec_loop = get_tod() - time1;
	ib_printf("time interval for empty loop seconds=%d, microseconds=%d\n",
			  diffusec_loop / 1000000, diffusec_loop % 1000000);

/* Now do the iteration of the operation for the given count */

	time1 = get_tod();
	for (i = 0; i < count; i++) {
		WALC_acquire(WAL_handle, &WAL_segment);
		WALC_release(WAL_handle);
	}
	diffusec = get_tod() - time1;

/* Now get the actual interval for just the operations */

	diffusec = diffusec - diffusec_loop;

	ib_printf("time interval seconds=%d, microseconds=%d\n",
			  diffusec / 1000000, diffusec % 1000000);
}


static SLONG get_tod(void)
{
/**************************************
 *
 *	 g e t _ t o d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
#ifdef HAVE_GETTIMEOFDAY
#ifdef GETTIMEOFDAY_RETURNS_TIMEZONE
	struct timeval tp1;

	(void)gettimeofday(&tp1, (struct timezone *)0);
#else
	(void)gettimeofday(&tp1);
#endif
	if (!base_seconds)
		base_seconds = tp1.tv_sec;
	return (tp1.tv_sec - base_seconds) * 1000000 + tp1.tv_usec;
#else
	struct timeb tp1;

	ftime(&tp1);
	if (!base_seconds)
		base_seconds = tp1.time;
	return (tp1.time - base_seconds) * 1000000 + (tp1.millitm * 1000);
#endif
}


static bool get_word( SCHAR * buffer)
{
/**************************************
 *
 *	 g e t _ w o r d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	SSHORT c;

	while ((c = ib_getchar()) != '\n') {
		if (c == EOF)
			return false;
		*buffer++ = c;
	}
	*buffer = 0;

	return true;
}


static void partitioned_logfile_create(
									   ISC_STATUS * status_vector, SCHAR * dbname)
{
/**************************************
 *
 *	p a r t i t i o n e d _ l o g f i l e _ c r e a t e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	int nump;
	SLONG logsize;
	SCHAR master_logname[MAXPATHLEN];
	int ret;

	ib_printf("Enter the logname: ");
	ib_scanf("%s", master_logname);
	ib_scanf("%c", &ret);		/* get rid of the trailing '\n' character */
	ib_printf("Enter the size of the log file: ");
	ib_scanf("%ld", &logsize);
	ib_scanf("%c", &ret);		/* get rid of the trailing '\n' character */
	ib_printf("Enter the number of partitions: ");
	ib_scanf("%d", &nump);
	ib_scanf("%c", &ret);		/* get rid of the trailing '\n' character */
	ret =
		WALF_init_p_log(status_vector, dbname, master_logname, logsize, nump);
	if (ret != FB_SUCCESS)
		gds__print_status(status_vector);
}


static void partitioned_logfile_info( ISC_STATUS * status_vector, SCHAR * dbname)
{
/**************************************
 *
 *	p a r t i t i o n e d _ l o g f i l e _ i n f o
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	P_LOGFH p_log_header;
	int p_log_fd;
	SCHAR master_logname[MAXPATHLEN];
	int ret;

	p_log_header = (P_LOGFH) gds__alloc(P_LOGFH_LENGTH);
	if (!p_log_header) {
		ib_printf("driver: Out of memory\n");
		exit(FINI_ERROR);
	}

	ib_printf("Enter the logname: ");
	ib_scanf("%s", master_logname);
	ib_scanf("%c", &ret);		/* get rid of the trailing '\n' character */
	if (WALF_open_partitioned_log_file(status_vector, dbname, master_logname,
									   p_log_header, &p_log_fd) != FB_SUCCESS) {
		gds__free(p_log_header);
		gds__print_status(status_vector);
		return;
	}
	ib_printf("length=%d, total partitions=%d, current_partition=%d\n",
			  p_log_header->p_logfh_length, p_log_header->p_logfh_maxp,
			  p_log_header->p_logfh_curp);
	ib_printf("partition size=%d, first partition offset=%d, flags=%ld\n",
			  p_log_header->p_logfh_sizep,
			  p_log_header->p_logfh_firstp_offset,
			  p_log_header->p_logfh_flags);

	close(p_log_fd);
	gds__free(p_log_header);
}


static SSHORT read_log(
					   ISC_STATUS * status_vector,
					   SCHAR * dbname,
					   SCHAR * logname, SLONG log_partition_offset)
{
/**************************************
 *
 *	r e a d _ l o g
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
#define MAX_LOG_COUNT  5
	WALRS WALRS_handle;
	SLONG log_seqno;
	SLONG log_offset;
	SCHAR buff[10000];
	SSHORT ret, len;
	SCHAR *lognames[MAX_LOG_COUNT];
	SLONG log_p_offsets[MAX_LOG_COUNT];
	int log_count;
	int fixup;

	lognames[0] = logname;
	log_p_offsets[0] = log_partition_offset;
	log_count = 1;
/*
lognames[1] = "log1";
log_p_offsets[1] = 0L;
lognames[2] = "log2";
log_p_offsets[2] = 0L;
log_count = 3;
*/

	WALRS_handle = NULL;
	if ((ret = WALR_open(status_vector, &WALRS_handle, dbname,
						 log_count, lognames, log_p_offsets,
						 0L, NULL, false)) != FB_SUCCESS) {
		if (ret == -1) {
			ib_printf("End of log...\n");
			return FB_SUCCESS;
		}
		ib_printf("Error = %d from WALR_open\n", ret);
		gds__print_status(status_vector);
		return ret;
	}
	ib_printf("Now doing WALR_get() calls.\n");
	while (1) {
		ret =
			WALR_get(status_vector, WALRS_handle, buff, &len, &log_seqno,
					 &log_offset);
		if (ret == -1) {
			ib_printf("End of log...\n");
			break;
		}
		else if (ret != FB_SUCCESS) {
			ib_printf("Log read error %d\n", ret);
			gds__print_status(status_vector);
			break;
		}
		buff[len] = '\0';
		ib_printf("log_seqno=%ld, log_offset=%ld, log_record=%s\n", log_seqno,
				  log_offset, buff);
	}

	ib_printf
		("\nDo you want to fixup the log header of the last log ? <y/n> ");
	ib_scanf("%c", &fixup);

	if (fixup == 'y' || fixup == 'Y')
		WALR_fixup_log_header(status_vector, WALRS_handle);
	ib_scanf("%c", &fixup);		/* get rid of the trailing '\n' character */
	WALR_close(status_vector, &WALRS_handle);

	return FB_SUCCESS;
}


static void print_help(void)
{
/**************************************
 *
 *	p r i n t _ h e l p
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	ib_printf("Enter F to flush, C to commit.\n");
	ib_printf
		("      K to finish checkpoint, V to do virtual checkpointing.\n");
	ib_printf("      J to enable journalling, D to disable journalling.\n");
	ib_printf("      R to read logs.\n");
	ib_printf("      S for simple WAL status.\n");
	ib_printf
		("      W or Y to dump WAL segment info with or without acquire.\n");
	ib_printf
		("      P to get the names of all the previously linked log files.\n");
	ib_printf
		("      N to get the names of all the forward linked log files.\n");
	ib_printf("      L to get the info about the last linked log file.\n");
	ib_printf("      G to set the walfh_flag of a given log file.\n");
	ib_printf("      B to do semaphore benchmark.\n");
	ib_printf("      M to create a log file with multiple partitions.\n");
	ib_printf
		("      I to get the header info of a multi-partition log file.\n");
	ib_printf("      E for ending (shutting down) a WAL writer.\n");
	ib_printf("      T to set group-commit wait time in micro-seconds.\n");
	ib_printf
		("      X to toggle message writing by the WAL writer to the debug file.\n");
	ib_printf("      H for this help message.\n");
	ib_printf("      Q to quit .\n");
	ib_printf("      anything else is simply put in the log.\n");
}


static void toggle_debug_msg_flag( ISC_STATUS * status_vector, WAL WAL_handle)
{
/**************************************
 *
 *	t o g g l e _ d e b u g _ m s g _ f l a g
 *
 **************************************
 *
 * Functional description
 *    Toggle the debug messages flag in WAL segment.
 *
 **************************************/
	WALS WAL_segment;

	WALC_acquire(WAL_handle, &WAL_segment);
	if (WAL_segment->wals_flags2 & WALS2_DEBUG_MSGS)
		WAL_segment->wals_flags2 &= ~WALS2_DEBUG_MSGS;
	else
		WAL_segment->wals_flags2 |= WALS2_DEBUG_MSGS;
	WALC_release(WAL_handle);
}
