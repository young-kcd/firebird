/*
 *        PROGRAM:        JRD Write Ahead Log
 *        MODULE:         wal.h
 *        DESCRIPTION:    Write Ahead Log (WAL) interface definitions
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
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "DELTA" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "IMP" port
 *
 */

#ifndef WAL_WAL_H
#define WAL_WAL_H

#include <stdio.h>
#ifndef VMS
#ifndef FCNTL_INCLUDED
#include <fcntl.h>
#define FCNTL_INCLUDED
#endif
#endif
#include "../jrd/common.h"
#include "../jrd/misc.h"
#include "../jrd/isc.h"

#ifdef WIN_NT
#define SYS_ERR			isc_arg_win32
#endif

#ifndef SYS_ERR
#define SYS_ERR			isc_arg_unix
#endif

#define MAX_JRN_INIT_DATA	2 * (MAXPATHLEN + 2)

#define WALS_VERSION1		1
#define WALS_VERSION		WALS_VERSION1

/* WAL Semaphores */

#define MAX_WALSEMS			6
#define WAL_SEM				0
#define WAL_WRITER_WORK_SEM		1
#define WAL_WRITER_WORK_DONE_SEM	2
#define WAL_GCOMMIT_WAIT1_SEM		3
#define WAL_GCOMMIT_WAIT2_SEM		4
#define WAL_GCOMMIT_STALL_SEM		5

#if (defined WIN_NT)
#define WAL_EVENTS		WAL_handle->wal_events
#endif

#ifndef WAL_EVENTS
#define WAL_EVENTS		WAL_segment->wals_events
#endif

#define OneK			1024

#define DEFAULT_CKPT_INTRVL	500
#define MIN_CKPT_INTRVL		100
#define MIN_WALBLKS		4
#define MAX_WALBLKS		64

#define MIN_WALBUFLEN		2048
#define MAX_WALBUFLEN		64000

#define DEFAULT_LOG_LENGTH	500
#define MIN_LOG_LENGTH		100

/* WAL record header */
/* the walrec_offset field is useful for easy traversal at
   block level given a record offset. */

typedef struct walrec_header {
    USHORT   walrec_len;     /* length of the logrec */
    USHORT   walrec_offset;  /* of this logrec within its block. */
} WALREC_HDR;

#define REC_HDROVHD             sizeof(WALREC_HDR)

/* WAL block buffer header */

typedef struct walblk_hdr {
    USHORT   walblk_hdr_blklen;       /* Total length of the block including padded bytes */
    USHORT   walblk_hdr_len;          /* Useful length of the block buffer */
    SLONG    walblk_hdr_bsn;          /* Block Sequence Number of this buffer */
    SLONG    walblk_hdr_timestamp[2]; /* When this buffer was written */
} WALBLK_HDR;

#define BLK_HDROVHD             sizeof(WALBLK_HDR)
#define BLK_TAILOVHD            sizeof(USHORT)
#define BLK_OVHD                (BLK_HDROVHD + BLK_TAILOVHD)


/* WAL block, to hold the WAL records. */

typedef struct walblk {
    USHORT   walblk_number;           /* Position among all the buffers */
    USHORT   walblk_flags;            /* Various flags, explained below */
    USHORT   walblk_cur_offset;       /* Current offset within the buffer */
    USHORT   walblk_roundup_offset;   /* After rounding off the cur_offset */
    UCHAR    walblk_buf[1];           /* Actual WAL buffer, starts with WALBLK_HDR */
} WALBLK;

#define WALBLK_to_be_written    1
#define WALBLK_checkpoint       2

/* Group-commit block, to hold group_commit information. */

typedef struct grp_commit {
    SLONG    grp_commit_coordinator;	/* Process-id of the coordinator */
    USHORT   grp_commit_size;		/* Number of processes involved */
    int     grp_commit_event_num;	/* Event to wait on */
} GRP_COMMIT;

#define MAX_GRP_COMMITTERS      2

/* Default waiting times (in seconds and microseconds) and other factors
   for the group-commit protocol */

#define GRPC_WAIT_USECS                 0
#define GRPC_COORD_FACTOR               20
#define GRPC_OTHER_COORD_FACTOR         20

/* Log files info block, to hold pre-allocated log files and overflow
   log information */

typedef struct logf {
    USHORT   logf_name_offset;        /* Offset within WAL segment of the name of the pre-allocated log file */
    USHORT   logf_partitions;         /* Number of partitions */
    SLONG    logf_max_size;           /* For rollover; if -1 use wals_max_log_length */
    USHORT   logf_roundup_size;       /* For each write call, 0 means no roundup */
    USHORT   logf_flags;              /* Various flags, defined below */
    SLONG    logf_base_offset;        /* Starting offset within a raw device for the logical logfile */
    SLONG    logf_fname_seqno;        /* To generate series of new log file names */
} LOGF;

#define LOGF_PARTITIONED 		  1
#define LOGF_RAW 	          	  2

/* WAL segment, one shared segment per database.  The variable length info
   and WAL buffers are tagged after this structure.  */

typedef struct wals {
    USHORT   wals_version;           /* Version number of this structure */
    USHORT   wals_dbpage_size;       /* Size of the database page */
    SLONG    wals_length;            /* Length of the WAL segment */
    SLONG    wals_id;                /* Time when this structure was initialized */
    SLONG    wals_owner_id;          /* id of the owner of the database file */
    SLONG    wals_group_id;          /* id of the group of the database file */
    SLONG    wals_writer_pid;        /* Process id of the WAL writer */
    MTX_T   wals_mutex [1];         /* Mutex controlling access to WAL segment */
    EVENT_T wals_events [MAX_WALSEMS];
    SLONG    wals_last_pid;          /* Process-id of the last process acquiring mutex */
    USHORT   wals_num_attaches;      /* Number of processes who did attachment to WAL segment */
    USHORT   wals_num_detaches;      /* Number of processes who did detachment from WAL segment */
    SLONG    wals_last_err;          /* Last error code for the WAL subsystem */
    SLONG    wals_last_bug;          /* Last bug code for the WAL subsystem */

    LOGF    *wals_curr_log_info;    /* Pointer to the LOGF of the current logfile */
    SLONG    wals_cur_log_base_offset; /* Where does the current log file actually start (on a raw device) */
    SLONG    wals_log_partition_offset;/* Where in the logname file, this log file logically starts. */
    SLONG    wals_max_log_length;    /* Default maximum length for a log file */
    SLONG    wals_rollover_threshold; /* After which we should attempt to rollover from the current log file */
    SLONG    wals_log_seqno;         /* The sequence number of this log file */

    SLONG    wals_max_ckpt_intrvl;   /* Maximum checkpoint interval length in terms of log length */
    SLONG    wals_thrshold_intrvl;   /* Threshold (some fraction of max_ckpt_intrvl) value to initiate checkpoint */
    SLONG    wals_cur_ckpt_intrvl;   /* How many bytes written so far since the last checkpoint */
    SLONG    wals_ckpted_log_seqno;  /* Sequence no of the log file which has the last checkpoint record */
    SLONG    wals_ckpt_log_p_offset; /* Where in the logname, this log file logically starts. */
    SLONG    wals_ckpted_offset;     /* Byte offset (in the log) of the last checkpoint */
    SLONG    wals_saved_ckpted_offset;/* Saving place of byte offset (in the log) of the last checkpoint */

    SLONG    wals_prev_ckpted_log_seqno;/* Sequence no of the log file which has the previous checkpoint record */
    SLONG    wals_prev_ckpt_log_p_offset;/* Where in the logname, this log file logically starts. */
    SLONG    wals_prev_ckpted_offset;/* Byte offset (in the log) of the previous checkpoint */
    int     wals_buf_waiters;       /* Number of processes currently waiting for an empty buffer or a flush */
    int     wals_buf_waiting_count; /* Num of times any process had to wait for an empty buffer after the last expansion of buffer blocks. */
    SLONG    wals_buf_offset;        /* Offset in log file of the last byte put in log buffer */
    SLONG    wals_flushed_log_seqno; /* Sequence number of the log file which was flushed last */
    SLONG    wals_flushed_offset;    /* Offset of the last byte flushed to log file */

    int     wals_grpc_wait_id;      /* Montonically increasing id for the following wait settings */
    SLONG    wals_grpc_wait_usecs;   /* Number of micro-seconds to wait for grouping */
    SSHORT   wals_cur_grpc_blknum;   /* Current group-commit block */
    USHORT   wals_dummy1;
    GRP_COMMIT  wals_grpc_blks[MAX_GRP_COMMITTERS]; /* Group-commit blocks */
    SLONG    wals_grpc_count;        /* Number of group commits in this session */

    SSHORT   wals_max_logfiles;      /* Total number of preallocated log files */
    SSHORT   wals_cur_logfile;       /* Currently used pre-allocated log file, -1 if none used. */
    USHORT   wals_logf_offset;       /* Offset for the info about the first pre-allocated log file. */
    USHORT   wals_logf_size;         /* Roundup size of each logf structure in the array starting at wals_logf_offset. */
    LOGF    wals_log_serial_file_info; /* Info about the serial log files */
    LOGF    wals_log_ovflow_file_info; /* Info about the overflow log files */

    SSHORT   wals_initial_maxbufs;   /* Total buffer blocks in the WAL segment initially */
    SSHORT   wals_maxbufs;           /* Total buffer blocks in the WAL segment currently */
    SSHORT   wals_curbuf;            /* Buffer collecting log records now, -1 => no buffer available */
    SSHORT   wals_last_flushed_buf;  /* Buffer (number) flushed last by the buffer writer */
    USHORT   wals_bufsize;           /* Size of each WAL buffer */ 
    USHORT   wals_roundup_size;      /* To go to the next block boundary */ 
    USHORT   wals_max_recsize;       /* Maximum allowable size of a log record */

    USHORT   wals_jrn_init_data_len;    /* In the following array */
    UCHAR    wals_jrn_init_data[MAX_JRN_INIT_DATA]; /* to store info about initializing the journal server */
    SLONG    wals_total_IO_bytes;    /* Total number of bytes bytes written to log so far */
    SLONG    wals_IO_count;          /* Number of WAL buffer writes so far */

    SLONG    wals_acquire_count;     /* Number of WAL semaphore acquire calls in this session */
    SLONG    wals_put_count;         /* Number of wal_put2 calls in this session */
    SLONG    wals_commit_count;      /* Number of WAL_commit calls in this session */

    USHORT   wals_blksize;           /* Size of each buffer block structure */
    USHORT   wals_dummy2;
    SLONG    wals_blkseqno;          /* Sequence number of the next WAL block to be written */
    SLONG    wals_blks_offset;       /* Offset to the first buffer block. */
    SLONG    wals_flags;             /* Various flags, explained below */    
    SLONG    wals_flags2;            /* Various flags, explained below */    

    /* The following less frequently used fields are kept together at 
       the end to avoid unnecessary page faults while accessing the 
       earlier fields. */
    TEXT     wals_dbname[MAXPATHLEN];/* The database file name */
    TEXT     wals_logname[MAXPATHLEN];/* The log file name */
    TEXT     wals_rollover_logname_base[MAXPATHLEN];/* The log file name base for rolling over */
    TEXT     wals_ckpt_logname[MAXPATHLEN];/* Name of the log file which has the last checkpoint record */
    TEXT     wals_prev_ckpt_logname[MAXPATHLEN];/* Name of the log file which has the previous checkpoint record */
    TEXT     wals_jrn_dirname[MAXPATHLEN];/* The journal server directory name */
} *WALS;

#define WALS_WRITER_INITIALIZED       1
#define WALS_CKPT_START               2
#define WALS_CKPT_RECORDED            4
#define WALS_FIRST_TIME_LOG           8
#define WALS_WRITER_INFORMED         16
#define WALS_SHUTDOWN_WRITER         32
#define WALS_WRITER_DONE             64
#define WALS_GRP_COMMIT_IN_PROGRESS 128
#define WALS_GRP_COMMIT_WAITING     256
#define WALS_ROLLOVER_REQUIRED      512
#define WALS_ROLLOVER_HAPPENED     1024
#define WALS_ENABLE_JOURNAL        2048
#define WALS_DISABLE_JOURNAL       4096
#define WALS_JOURNAL_ENABLED       8192
#define WALS_INFORM_CLOSE_TO_JOURNAL 16384
#define WALS_BUG_HAPPENED          32768
#define WALS_ERROR_HAPPENED        65536
#define WALS_BUG_OR_ERROR_HAPPENED (WALS_BUG_HAPPENED+WALS_ERROR_HAPPENED)

#define WALS2_EXPAND                   1
#define WALS2_CANT_EXPAND              2
#define WALS2_DEBUG_MSGS               4

#define CUR_BUF WAL_segment->wals_curbuf
#define WAL_BLOCK(n) (WALBLK*) ((UCHAR*) WAL_segment + WAL_segment->wals_blks_offset + n * WAL_segment->wals_blksize)

#define CUR_LOG WAL_segment->wals_cur_logfile
#define LOGF_INFO(n) (LOGF*) ((UCHAR*) WAL_segment + WAL_segment->wals_logf_offset + n * WAL_segment->wals_logf_size)
#define LOGF_NAME(lf) (SCHAR*) ((UCHAR*) WAL_segment + lf->logf_name_offset)
#define MIN_WALS_LENGTH	sizeof(struct wals)

/* WAL handle, used in the API calls.  One handle per database per process. */

typedef struct wal {
    int		wal_count;	/* Number of times initialized */
    SH_MEM_T	wal_shmem_data;
#if (defined WIN_NT)
    MTX_T	wal_mutex [1];	/* Mutex controlling access to WAL segment */
    EVENT_T	wal_events [MAX_WALSEMS];
#endif
    SLONG	wal_pid;	/* Process (Thread) id of the current process (thread) */
    SLONG	wal_length;	/* Length of the WAL segement */
    SLONG	wal_id;		/* File descriptor of the mapped file or shmid for WAL_segment */
    SLONG	wal_grpc_wait_id; /* To keep track of a change in grp commit wait time */
    SLONG	wal_grpc_wait_usecs;
    SLONG	wal_grpc_wait_coord_usecs;
    SLONG	wal_grpc_wait_other_coord_usecs;
    SLONG        wal_flushed_seqno;     /* Highest cached log sequence no. */
    SLONG        wal_flushed_offset;    /* Hightest cached log offset */
    USHORT	wal_flags;	/* Various flags, described below */
    UCHAR	*wal_local_info_ptr;	/* Save some process specific info */
    WALS	wal_segment;	/* The WAL segment */
    TEXT	wal_dbname [MAXPATHLEN]; /* The database file name */
} *WAL;

#define WAL_SHUTDOWN_HANDLE  1
#define WAL_CLEANUP_HANDLE   2
    
/* WAL file header. */

typedef struct walfh {
    USHORT   walfh_version; /* Version number of this log file header structure */
    SLONG    walfh_wals_id; /* Id of the last shared WAL segment when this header was modified */
    USHORT   walfh_generation; /* Generation id for this log file configuration */
    TEXT     *walfh_dbname; /* Name of the associated database */
    SLONG    walfh_seqno;   /* Sequence number in the series of WAL files */
    SLONG    walfh_log_partition_offset;/* Offset for the beginning of this logical file */
    SLONG    walfh_length;  /* Of the file when last closed */
    SLONG    walfh_lowbsn;  /* Lowest block sequence number in this file */
    SLONG    walfh_hibsn;   /* Highest block sequence number in this file */
    SLONG    walfh_flags;   /* Various flags, see below */
    SLONG    walfh_offset;  /* Offset to the first WAL block in this file */
    SLONG    walfh_ckpt1;   /* Offset to where the second last checkpoint in this file done */
    SLONG    walfh_ckpt2;   /* Offset to where the last checkpoint in this file done */
    TEXT     *walfh_prev_logname;/* Name of the previous log file in sequence, if any */
    SLONG    walfh_prev_log_partition_offset;/* Where in the logname, this file logically starts. */
    TEXT     *walfh_next_logname;/* Name of the next log file in sequence, if any */
    SLONG    walfh_next_log_partition_offset;/* Where in the logname, this file logically starts. */
    SLONG    walfh_reserved1;
    SLONG    walfh_reserved2;
    USHORT   walfh_data_len;/* In the buffer below */
    UCHAR    walfh_data[2]; /* Various other info about the WAL file, even out the structure length */
} *WALFH;

#define WALFH_VERSION  4

#define WALFH_OPEN                        1
#define WALFH_SERIAL                      2
#define WALFH_PREALLOCATED                4
#define WALFH_PARTITIONED                 8
#define WALFH_RAW                        16
#define WALFH_OVERFLOW                   32
#define WALFH_KEEP_FOR_SHORT_TERM_RECV   64
#define WALFH_KEEP_FOR_LONG_TERM_RECV   128
#define WALFH_KEEP_FOR_RECV  (WALFH_KEEP_FOR_SHORT_TERM_RECV+WALFH_KEEP_FOR_LONG_TERM_RECV)   

#define WALFH_LENGTH   2048

/* Defines for storing info in walfh_data[] buffer */
#define WALFH_end            1
#define WALFH_dbname         2
#define WALFH_prev_logname   3
#define WALFH_next_logname   4


/* Partitioned log file header */

typedef struct p_logfh {
    UCHAR    p_logfh_version1;     /* Major version number of this structure */
    UCHAR    p_logfh_version2;     /* Minor version number of this structure */
    SLONG    p_logfh_length;       /* Total length of the log file */
    SSHORT   p_logfh_maxp;         /* Total number of partitions */ 
    SLONG    p_logfh_firstp_offset;/* Offset from the beginning of the very first partition */
    SLONG    p_logfh_sizep;        /* Size of each partition */
    SSHORT   p_logfh_curp;         /* Most recently (currently) used partition */
    SLONG    p_logfh_flags;        /* Various flags */
    TEXT     p_logfh_dbname[MAXPATHLEN]; /* Name of the database being logged */
    SLONG    p_logfh_reserved[10]; /* For later use */
} *P_LOGFH;

#define P_LOGFH_VERSION1    1
#define P_LOGFH_VERSION     P_LOGFH_VERSION1
#define P_LOGFH_LENGTH      2048

#define PARTITION_OFFSET(p_log, p_num) (P_LOGFH_LENGTH + p_num * p_log->p_logfh_sizep)
#define PARTITION_SIZE(loglen, nump) ((loglen - P_LOGFH_LENGTH)/nump)
#define PARTITIONED_LOG_TOTAL_SIZE(nump, p_size) ((nump * p_size) + P_LOGFH_LENGTH)

/* WAL reading handle used for reading one log files. */

typedef struct walr {
    TEXT     *walr_logname;     /* Name of the current log file */
    SLONG    walr_log_partition_offset; /* Where this log file physically starts */
    SLONG    walr_fd;           /* File descriptor for the current log file */
    SLONG    walr_next_bsn;     /* Next expected block sequence number */
    USHORT   walr_flags;        /* See below */
    USHORT   walr_read_ahead_bytes;/* How many bytes of the next block are read ahead */
    WALFH   walr_log_header;   /* Pointer to the header info of the current log file */
    SLONG    walr_offset;       /* Offset of the current block in the log file */
    SLONG    walr_rec_offset;   /* For the next log record in the buffer below */
    WALBLK_HDR  *walr_next_blkhdr;/* Points to the header of the next block */
    UCHAR    *walr_read_ahead_buffer; /* To read the next block */
    WALBLK_HDR  *walr_blkhdr;  /* Block header, points to the walr_buffer below */
    UCHAR    *walr_buffer;      /* To read blocks from the log file */
} *WALR;

#define WALR_EOF          1

/* WAL reading handle, used for reading a set of log files. */

typedef struct walrs {
    TEXT     *walrs_dbname;      /* Name of the database */
    int     walrs_max_logs;     /* Count of specified log files */ 
    int     walrs_cur_log;      /* Index of the current logfile into the array below */ 
    TEXT     **walrs_lognames;   /* Array of log file names */
    SLONG    *walrs_log_partitions_offsets; /* Array of log partition offsets */
    USHORT   walrs_flags;        /* See below */
    SLONG    walrs_timestamp[2]; /* Filter WAL blocks upto this timestamp */
    WALR    walrs_walr;         /* Handle for reading the current log file */
} *WALRS;

#define WALRS_TIMESTAMP           1
#define WALRS_EOF_SCAN            2
#define WALRS_DONT_SCAN_TAIL_LOGS 4

#define BLKHDR      WALRS_handle->walrs_walr->walr_blkhdr
#define REC_OFFSET  WALRS_handle->walrs_walr->walr_rec_offset
    
/* Defines for WAL parameter block */

#define WAL_PARAM_END                1

#define WAL_PARAM_BUF_COUNT          2
#define WAL_PARAM_BUF_LEN            3

#define WAL_PARAM_CKPT_INTRVL        4

#define WAL_PARAM_SERIAL_LOG         5
#define WAL_PARAM_RR_LOGS_COUNT      6
#define WAL_PARAM_RR_LOGS            7
#define WAL_PARAM_OVFLOW_LOG         8

#define WAL_PARAM_GRPC_WAIT_USECS    9

#define WAL_PARAM_JRN_DIRNAME       11
#define WAL_PARAM_JRN_DATA_LEN      12
#define WAL_PARAM_JRN_DATA          13

/* Defines for creating the full name of a logfile from its base name 
   and the sequence number */

#ifdef SMALL_FILE_NAMES
#define WAL_FNAME_SUBSCRIPT		""
#endif

#if (defined WIN_NT)
#define WAL_FNAME_SUBSCRIPT		""
#endif

#ifndef WAL_FNAME_SUBSCRIPT
#define WAL_FNAME_SUBSCRIPT		".log"
#endif

#define LOG_SECTOR_SIZE			512
#define WAL_TERMINATOR_CHAR		0377
#define WAL_TERMINATOR_BLKLEN		LOG_SECTOR_SIZE
#define WAL_TERMINATOR(variable)	UCHAR variable [WAL_TERMINATOR_BLKLEN]
#define WAL_TERMINATOR_SET(variable) memset (variable, (int) WAL_TERMINATOR_CHAR, sizeof (variable))

#define WAL_MAPNAME_SUBSCRIPT		"walfile"
#define WAL_MAPNAME_TEMPLATE		"%s.%s"
#define WAL_BUILD_MAPFNAME(mapfname, dbname) sprintf(mapfname, WAL_MAPNAME_TEMPLATE, dbname, WAL_MAPNAME_SUBSCRIPT)

#define WAL_DBGNAME_SUBSCRIPT		"ww.dbg"
#define WAL_DBGNAME_TEMPLATE		"%s.%s"
#define WAL_BUILD_DBGFNAME(dbgfname, dbname) sprintf(dbgfname, WAL_DBGNAME_TEMPLATE, dbname, WAL_DBGNAME_SUBSCRIPT)

#define WAL_ERROR(status_vector, code, string) IBERR_build_status \
        (status_vector, code, isc_arg_string, string, 0)

#define WAL_ERROR_APPEND(status_vector, code, string) IBERR_append_status \
        (status_vector, code, isc_arg_string, string, 0)

#define WAL_IO_ERROR(status_vector, string, logname, operation, error) IBERR_build_status \
        (status_vector, isc_io_error, isc_arg_string, string, \
         isc_arg_string, logname, isc_arg_gds, operation, SYS_ERR, error, 0)

#define WAL_FORMAT_ERROR(status_vector, code, logname, offset) IBERR_build_status \
        (status_vector, code, isc_arg_string, logname, \
         isc_arg_number, (SLONG) offset, 0)

#define WAL_CHECK_BUG(WAL_handle, WAL_segment) if (WAL_segment->wals_flags & WALS_BUG_HAPPENED) \
    {                                                                       \
    IBERR_build_status (status_vector, isc_wal_subsys_corrupt,             \
                        isc_arg_gds, WAL_segment->wals_last_bug, 0);        \
    WALC_release(WAL_handle);                                               \
    return FB_FAILURE;                                                         \
    }

#define WAL_CHECK_BUG_ERROR(WAL_handle, WAL_segment) if (WAL_segment->wals_flags & WALS_BUG_OR_ERROR_HAPPENED) \
    {                                                                       \
    if (WAL_segment->wals_flags & WALS_BUG_HAPPENED)                        \
        IBERR_build_status (status_vector, isc_wal_subsys_corrupt,         \
                            isc_arg_gds, WAL_segment->wals_last_bug, 0);    \
    else                                                                    \
        IBERR_build_status (status_vector, isc_wal_subsys_error,           \
                            isc_arg_gds, WAL_segment->wals_last_err, 0);    \
    WALC_release(WAL_handle);                                               \
    return FB_FAILURE;                                                         \
    }

#define STRING_DUP(s1,s2)	{s1 = (SCHAR*) gds__alloc (strlen (s2) + 1); if (s1) strcpy (s1, s2);}

#endif /* WAL_WAL_H */
