/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		jrn.h
 *	DESCRIPTION:	Journal interface definitions
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

#ifndef _JRD_JRN_H_
#define _JRD_JRN_H_

#include "../jrd/jrd_blks.h"
#include "../include/fb_blk.h"

#define MAX_RECORD		9000
#define JOURNAL_VERSION3	3
#define JOURNAL_VERSION4	4

#define JOURNAL_VERSION 	JOURNAL_VERSION4

#define CONSOLE_ADDR	"console.addr"	/* socket address for console */
#define JOURNAL_ADDR	"journal.addr"	/* socket address for journal */

#ifndef JOURNAL_FILE
#define JOURNAL_FILE	"journal.isc"
#define CONSOLE_FILE	"console.isc"
#endif

/* Journal block */

class jrn : public pool_alloc_rpt<SCHAR, type_jrn>
{
    public:
	ISC_STATUS *jrn_status_vector;	/* Status vector for errors */
	int *jrn_channel;			/* Communication channel */
	int jrn_iri;				/* RMS internal RAB id */
	SLONG jrn_lock_id;			/* Controlling lock id */
	SLONG jrn_handle;			/* Server handle */
	SLONG jrn_series;			/* Journal file series */
	SLONG jrn_dump;				/* page to be dumped (-1 -> dump database) */
	USHORT jrn_fid[3];			/* VMS file id */
	USHORT jrn_did[3];			/* VMS directory id */
	TEXT jrn_server[1];			/* Server name */
};
typedef jrn *JRN;

/* Journal record types */

#define JRN_ENABLE		1		/* Enable journalling */
#define JRN_DISABLE		2		/* Disable journalling */
#define JRN_SIGN_ON		3		/* Database attachment coming on line */
#define JRN_SIGN_OFF		4	/* Database being detached */
#define JRN_CNTRL_PT            5	/* Control point        */
#define JRN_WAL_NAME            6	/* WAL FILE name */
#define JRN_NEW_FILE            7	/* New secondary FILE name */
#define JRN_ACK			8		/* Anybody home? */
#define JRN_RESPONSE		9	/* Response to control message */
#define JRN_SET_TRANSACTION	10	/* Set transaction state */
#define JRN_DUMMY		11		/* Dummy record -- space not used */
#define JRN_RESET		12		/* Journal file has been reset */
#define JRN_COMMIT		13		/* Commit control message */
#define JRN_CONSOLE		14		/* Console message */
#define JRN_SYNC		15		/* Synchronize journal */
#define JRN_START_ONLINE_DMP	16
#define JRN_END_ONLINE_DMP	17
#define JRN_ONLINE_DMP_FILE	18
#define JRN_ARCHIVE_BEGIN	19
#define JRN_ARCHIVE_END		20
#define JRN_ARCHIVE_ERROR	21
#define JRN_max			21

#define JRN_PAGE		100		/* A full page WAL record */

/* Journal data clumplets.  Note: types above 100 are clumps of data */

#define JRNP_DATA_SEGMENT	101	/* Add segment to data page */
#define JRNP_POINTER_SLOT	102	/* Add pointer page to relation */
#define JRNP_TRANSACTION	103	/* Journal transaction state */
#define JRNP_NULL		104		/* Odd byte */
#define JRNP_PIP		105		/* Page allocation/deallocation */

#define JRNP_BTREE_NODE         106	/* Add/delete a node to BTREE */
#define JRNP_BTREE_SEGMENT      107	/* B Tree split - valid part */
#define JRNP_BTREE_DELETE       108	/* B Tree node delete - logical */
#define JRNP_INDEX_ROOT         109	/* Add/drop an index    */
#define JRNP_DB_HEADER          110	/* Modify db header page */
#define JRNP_GENERATOR          111	/* generator            */
#define JRNP_ROOT_PAGE		112	/* Index root page  */
#define JRNP_DB_ATTACHMENT	113	/* next attachment */
#define JRNP_DB_HDR_PAGES	114	/* Header pages     */
#define JRNP_DB_HDR_FLAGS	115	/* Header flags */
#define JRNP_DB_HDR_SDW_COUNT	116	/* Header shadow count */
#define JRNP_LOG_PAGE		117	/* log page information */
#define JRNP_NEXT_TIP		118	/* Next tip page is logged */
#define JRNP_MAX		118


/* Journal header block */

typedef struct jrnh {
	UCHAR jrnh_type;
	UCHAR jrnh_version;
	USHORT jrnh_length;			/* Total length of journal record */
	SLONG jrnh_handle;			/* Journal supplied handle for db */
	SLONG jrnh_series;			/* Journal series */
	SLONG jrnh_prev_seqno;		/* previous seqno  of record */
	SLONG jrnh_prev_offset;		/* previous offset of record */
} JRNH;

/* Long Term Journal control message format */

typedef struct ltjc {
	JRNH ltjc_header;
	USHORT ltjc_page_size;		/* Database page size */
	SLONG ltjc_seqno;			/* Seqno of WAL */
	ULONG ltjc_offset;			/* offset in WAL */
	USHORT ltjc_length;			/* Length of data */
	TEXT ltjc_data[1];			/* Expanded filename */
} LTJC;

#define LTJC_SIZE 	OFFSETA (LTJC*, ltjc_data)

/* Long Term Journal archive message format */

typedef struct ltja {
	JRNH ltja_header;
	SLONG ltja_db_id;			/* Database ID */
	SLONG ltja_file_id;			/* WAL file ID */
	SLONG ltja_error_code;		/* Error number */
	USHORT ltja_length;			/* Length of data */
	TEXT ltja_data[1];			/* Expanded filename */
} LTJA;

#define LTJA_SIZE 	OFFSETA (LTJA*, ltja_data)

enum jrnr_t {
	jrnr_accepted = 1,			/* Connection accepted */
	jrnr_rejected,				/* Connection rejected */
	jrnr_dump_database,			/* Dump the database before proceding */
	jrnr_dump_page,				/* Dump a specific page */
	jrnr_enabled,				/* Database enabled */
	jrnr_start_dump,			/* Start online dump */
	jrnr_end_dump,				/* End Online dump */
	jrnr_shutdown,				/* Journal server is shutting down */
	jrnr_msg,					/* Message line from server */
	jrnr_end_msg,				/* End of message lines */
	jrnr_exit,					/* Console should exit */
	jrnr_error,					/* Console request not granted */
	jrnr_archive_error,			/* invalid archive directory */
	jrnr_ack					/* Acknowlege journal message */
};

/* Journal server response message */

typedef struct jrnr {
	JRNH jrnr_header;
	enum jrnr_t jrnr_response;
	SLONG jrnr_page;
} JRNR;

/* Journal data message format */

typedef struct jrnd {
	JRNH jrnd_header;
	USHORT jrnd_length;			/* Length of data portion */
	SLONG jrnd_page;			/* Page number */
	UCHAR jrnd_data[1];
} JRND;

#define JRND_SIZE	OFFSETA (JRND*, jrnd_data)

/* Journal page clumplet formats */

typedef struct jrnp {
	UCHAR jrnp_type;			/* Clumplet type */
	USHORT jrnp_index;			/* Segment or slot index */
	USHORT jrnp_length;			/* Length of data */
	UCHAR jrnp_data[1];			/* Variable data */
} JRNP;

#define JRNP_SIZE	OFFSETA (JRNP*, jrnp_data)

/* B-tree node addition clumplet formats */

typedef struct jrnb {
	UCHAR jrnb_type;			/* Clumplet type */
	ULONG jrnb_prefix_total;	/* new prefix total */
	USHORT jrnb_offset;			/* offset into page */
	USHORT jrnb_delta;			/* increase in btr_length */
	USHORT jrnb_length;			/* Length of data */
	UCHAR jrnb_data[1];			/* Variable data */
} JRNB;

#define JRNB_SIZE	OFFSETA (JRNB*, jrnb_data)

/* Clumplet for index changes */
/* used for JRNP_TRANSACTION and JRNP_NEXT_TIP */

typedef struct jrni {
	UCHAR jrni_type;			/* Clumplet type */
	UCHAR jrni_states;			/* New state */
	USHORT jrni_position;		/* Position on transaction page */
	SLONG jrni_transaction;		/* Transaction number */
} JRNI;

# define JRNI_SIZE	sizeof (struct jrni)

/* Clumplet for page allocation/deallocation */

typedef struct jrna {
	UCHAR jrna_type;			/* Clumplet type */
	UCHAR jrna_allocate;		/* 1 = allocate, 2 = free */
	ULONG jrna_slot;			/* Slot on page */
} JRNA;

/* Journal buffer file definition */

#define BLOCK_SIZE	512			/* Size of disk block in bytes */
#define SEGMENT_SIZE	128		/* Size of file segment in blocks */
#define SEGMENT_SLOTS	500		/* Number of segment slots */
#define ID_SPACE	4096		/* Bytes reserved for database filename/id space */
#define ID_OFFSET	((sizeof (struct jfh) + BLOCK_SIZE - 1) / BLOCK_SIZE)
#define SEGMENT_OFFSET	ID_OFFSET + ((ID_SPACE + BLOCK_SIZE - 1) / BLOCK_SIZE)
#define HANDLE_WORDS	8

/* Journal File header data */

typedef struct jfh {
	USHORT jfh_version;			/* File format version number */
	USHORT jfh_allocation;		/* Number of segments allocated */
	USHORT jfh_data;			/* Slot number of first data segment */
	USHORT jfh_free;			/* Slot number of first free segment */
	USHORT jfh_flags;			/* Misc flags */
	USHORT jfh_copy_block;		/* Current (last recorded) block copied */
	USHORT jfh_write_block;		/* Current (last recorded) block written */
	USHORT jfh_id_space;		/* Database id space used */
	ULONG jfh_next_id;			/* Next available database id */
	ULONG jfh_series;			/* Current journal update series */
	ULONG jfh_sequence;			/* Next sequence number for journal server */
	USHORT jfh_segments[SEGMENT_SLOTS];
} *JFH;

#define JFH_VERSION	1			/* current file format version number */

#define JFH_suspended	1		/* Journalling has been suspended */

/* Database identification record */

typedef struct dir {
	ULONG dir_id;				/* Database journaling id */
	ULONG dir_last_page;		/* Last page journal (if questionable) */
	UCHAR dir_length;			/* Length of name */
	TEXT dir_name[1];			/* Database file name */
} *DIR;

/*
 * For start transaction log -
 */

/* database header - logical log record */

typedef struct jrndh {
	UCHAR jrndh_type;			/* = JRNP_DB_HEADER */
	SLONG jrndh_nti;			/* next transaction id */
	SLONG jrndh_oit;			/* oldest interesting transaction */
	SLONG jrndh_oat;			/* oldest active transaction */
} JRNDH;

#define JRNDH_SIZE 	sizeof (struct jrndh)

/* log header - logical log record */

typedef struct jrnl {
	UCHAR jrnl_type;			/* = JRNP_LOG_PAGE */
	SLONG jrnl_flags;			/* log page flags  */
	SLONG jrnl_tip;				/* updating transactions tip */
	SLONG jrnl_tid;				/* updating transaction id */
} JRNL;

#define JRNL_SIZE 	sizeof (struct jrnl)

/* 
 * Generator page logging will be done by logging the offset of the
 * generator and the new value.
 */

typedef struct jrng {
	UCHAR jrng_type;			/* = JRNP_GENERATOR     */
	SLONG jrng_offset;			/* offset into page     */
	SLONG jrng_genval;			/* generator value      */
} JRNG;

#define JRNG_SIZE 	sizeof (struct jrng)

/* 
 * Index root page logging will be done by logging the index id and
 * the new b-tree root page number.
 */

typedef struct jrnrp {
	UCHAR jrnrp_type;			/* = JRNP_ROOT_PAGE */
	SLONG jrnrp_id;				/* index number     */
	SLONG jrnrp_page;			/* new page number  */
} JRNRP;

#define JRNRP_SIZE 	sizeof (struct jrnrp)

/* 
 * WAL file message format.
 * This is a generic message that will be used by a 
 * variety of users.
 * - Creation of WAL file.
 * - Control point record.
 * - Online dump record.
 * In some of these usages, the record is first written to WAL to get
 * a seqno & offset pair, and then a message is written to the
 * journal server.
 */

typedef struct ltjw {
	JRNH ltjw_header;
	USHORT ltjw_mode;			/* any mode of operation */
	ULONG ltjw_seqno;			/* log file seqno */
	ULONG ltjw_offset;			/* offset of record */
	ULONG ltjw_p_offset;		/* partition offset */
	USHORT ltjw_count;			/* Count used for online dump */
	SLONG ltjw_date[2];			/* Date/time of event */
	USHORT ltjw_length;			/* Length of WAL filename */
	TEXT ltjw_data[1];			/* Expanded filename */
} LTJW;

#define LTJW_SIZE 	OFFSETA (LTJW*, ltjw_data)

/* values of mode */

#define JRNW_OPEN	0
#define JRNW_CLOSE	1

/* WAL message types */

/* Encoded as <type> <length> <data> in ltjw_data */

#define JRNW_END		0
#define JRNW_DELETE_RULE	1
#define JRNW_DB_NAME		2
#define JRNW_BACKUP_DIR		3
#define JRNW_DEVICE_TYPE	4

/* device type */

#define JRNW_DISK		0
#define JRNW_TAPE		1

/* Secondary file name message */

typedef struct jrnf {
	JRNH jrnf_header;
	ULONG jrnf_start;			/* start block number */
	USHORT jrnf_length;			/* Length of filename */
	USHORT jrnf_sequence;		/* File sequence # */
	TEXT jrnf_filename[1];		/* Expanded filename */
} JRNF;

#define JRNF_SIZE 	OFFSETA (JRNF*, jrnf_filename)

typedef struct jrnda {
	UCHAR jrnda_type;
	ULONG jrnda_data;
} JRNDA;

#define JRNDA_SIZE 	sizeof (struct jrnda)

#define MAX_OLD_FILES   16
#define MAX_LOG_FILES   16

class logfiles : public pool_alloc_rpt<SCHAR, type_ail>
{
    public:
	SLONG lg_size;
	SSHORT lg_sequence;
	SSHORT lg_partitions;
	SSHORT lg_flags;
	TEXT lg_name[2];
};
typedef logfiles LGFILE;

#define LGFILE_SIZE 	OFFSETA (LGFILE*, lg_name)

#endif /* _JRD_JRN_H_ */
