/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		ods.h
 *	DESCRIPTION:	On disk structure definitions
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
 * 2000.11.29 Patrick J. P. Griffin: fixed bug SF #116733
 *	Add typedef struct gpg to properly document the layout of the generator page
 * 2002.08.26 Dmitry Yemanov: minor ODS change (new indices on system tables)
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#ifndef JRD_ODS_H
#define JRD_ODS_H

/**********************************************************************
**
** NOTE:
**
**   ODS 5 was shipped with version 3.3 but no longer supported
**   ODS 6 and ODS 7 never went out the door
**   ODS 8 was shipped with version 4.0
**   ODS 9 is going to be shipped with version 4.5
**
***********************************************************************/

/* ODS major version -- major versions are not compatible */

#define ODS_VERSION6 	6		/* on-disk structure as of v3.0 */
#define ODS_VERSION7	7		/* new on disk structure for fixing index bug */
#define ODS_VERSION8	8		/* new btree structure to support pc semantics */
#define ODS_VERSION9	9		/* btree leaf pages are always propogated up */
#define ODS_VERSION10	10		/* V6.0 features. SQL delimited idetifier,
								   SQLDATE, and 64-bit exact numeric type */
#define ODS_VERSION11   11      /* Firebird 2.0 features */

/* ODS minor version -- minor versions ARE compatible, but may be
   increasingly functional.  Add new minor versions, but leave previous
   names intact */

/* Minor versions for ODS 6 */

#define ODS_GRANT6	1			/* adds fields for field level grant */
#define ODS_INTEGRITY6	2		/* adds fields for referential integrity */
#define ODS_FUNCTIONS6	3		/* modifies type of RDB$MODULE_NAME field */
#define ODS_SQLNAMES6	4		/* permits SQL security on > 27 SCHAR names */
#define ODS_CURRENT6	4

/* Minor versions for ODS 7 */

#define ODS_FUNCTIONS7	1		/* modifies type of RDB$MODULE_NAME field */
#define ODS_SQLNAMES7	2		/* permits SQL security on > 27 SCHAR names */
#define ODS_CURRENT7	2

/* Minor versions for ODS 8 */

#define ODS_CASCADE_RI8	1		/* permits cascading referential integrity */
				/* ODS 8.2 is the same as ODS 8.1 */
#define ODS_CURRENT8	2

/* Minor versions for ODS 9 */

#define ODS_CURRENT_9_0	0		/* SQL roles & Index garbage collection */
#define ODS_SYSINDEX9	1		/* Index on RDB$CHECK_CONSTRAINTS (RDB$TRIGGER_NAME) */
#define ODS_CURRENT9	1

/* Minor versions for ODS 10 */

#define ODS_CURRENT10_0	0		/* V6.0 features. SQL delimited identifier,
								   SQLDATE, and 64-bit exact numeric type */
#define ODS_SYSINDEX10	1		/* New system indices */
#define ODS_CURRENT10	1

/* Minor versions for ODS 11 */

#define ODS_CURRENT11_0	0		/* Firebird 2.0 features. */
#define ODS_CURRENT11	0

/* useful ODS macros. These are currently used to flag the version of the
   system triggers and system indices in ini.e */

#define ENCODE_ODS(major,minor) (((major) << 4) | (minor))
#define ODS_8_0         ENCODE_ODS (ODS_VERSION8, 0)
#define ODS_8_1         ENCODE_ODS (ODS_VERSION8, 1)
#define ODS_9_0         ENCODE_ODS (ODS_VERSION9, 0)
#define ODS_9_1         ENCODE_ODS (ODS_VERSION9, 1)
#define ODS_10_0        ENCODE_ODS (ODS_VERSION10, 0)
#define ODS_10_1        ENCODE_ODS (ODS_VERSION10, 1)
#define ODS_11_0        ENCODE_ODS (ODS_VERSION11, 0)

/* Decode ODS version to Major and Minor parts. The 4 LSB's are minor and 
   the next 4 bits are major version number */
#define DECODE_ODS_MAJOR(ods_version) ((ods_version & 0xFFF0) >> 4)
#define DECODE_ODS_MINOR(ods_version) (ods_version & 0x000F)

/* Set current ODS major and minor version */

#define ODS_VERSION	ODS_VERSION11	/* current ods major version -- always 
									   the highest */
#define ODS_CURRENT	ODS_CURRENT11	/* the highest defined minor version
									   number for this ODS_VERSION!! */
#define ODS_CURRENT_VERSION	ODS_11_0	/* Current ODS version in use which includes 
										   both Major and Minor ODS versions! */


#define USER_REL_INIT_ID_ODS8   31	/* ODS <= 8 */
#define USER_DEF_REL_INIT_ID   128	/* ODS >= 9 */


/* Page types */

#define pag_undefined	0
#define pag_header	1			/* Database header page */
#define pag_pages	2			/* Page inventory page */
#define pag_transactions 3		/* Transaction inventory page */
#define pag_pointer	4			/* Pointer page */
#define pag_data	5			/* Data page */
#define pag_root	6			/* Index root page */
#define pag_index	7			/* Index (B-tree) page */
#define pag_blob	8			/* Blob data page */
#define pag_ids		9			/* Gen-ids */
#define pag_log		10			/* Write ahead log information */
#define pag_max		10			/* Max page type */

#define HEADER_PAGE	0
#define LOG_PAGE        2

#define MIN_PAGE_SIZE	1024

#define MAX_PAGE_SIZE		16384
#define DEFAULT_PAGE_SIZE	4096

/* Basic page header */

typedef struct pag {
	SCHAR pag_type;
	SCHAR pag_flags;
	USHORT pag_checksum;
	ULONG pag_generation;
	ULONG pag_seqno;			/* WAL seqno of last update */
	ULONG pag_offset;			/* WAL offset of last update */
	// We use pag_seqno as SCN number to avoid major ODS version bump
	ULONG& pag_scn() { return pag_seqno; }
} *PAG;


/* Blob page */

typedef struct blp {
	struct pag blp_header;
	SLONG blp_lead_page;		/* First page of blob (for redundancy only) */
	SLONG blp_sequence;			/* Sequence within blob */
	USHORT blp_length;			/* Bytes on page */
	USHORT blp_pad;				/* Unused */
	SLONG blp_page[1];			/* Page number if level 1 */
} *BLP;

#define blp_data	blp_page	/* Formerly defined field */
#define BLP_SIZE	OFFSETA (BLP, blp_page)

#define blp_pointers	1		/* Blob pointer page, not data page */


/* B-tree node */

typedef struct btn
{
	UCHAR btn_prefix;			/* size of compressed prefix */
	UCHAR btn_length;			/* length of data in node */
	UCHAR btn_number[4];		/* page or record number */
	UCHAR btn_data[1];
} *BTN;

#define BTN_SIZE	6

/* B-tree page ("bucket") */

typedef struct btr {
	struct pag btr_header;
	SLONG btr_sibling;			/* right sibling page */
	SLONG btr_left_sibling;		/* left sibling page */
	SLONG btr_prefix_total;		/* sum of all prefixes on page */
	USHORT btr_relation;		/* relation id for consistency */
	USHORT btr_length;			/* length of data in bucket */
	UCHAR btr_id;				/* index id for consistency */
	UCHAR btr_level;			/* index level (0 = leaf) */
	struct btn btr_nodes[1];
} *BTR;

#define btr_dont_gc		1		/* Don't garbage-collect this page */
#define btr_not_propogated	2	/* page is not propogated upward */
#define btr_descending  	8	/* Page/bucket is part of a descending index */

#define MAX_NODES	((dbb->dbb_page_size - OFFSETA (BTR, btr_nodes)) / sizeof (struct btn))


/* Data Page */

typedef struct dpg {
	struct pag dpg_header;
	SLONG dpg_sequence;			/* Sequence number in relation */
	USHORT dpg_relation;		/* Relation id */
	USHORT dpg_count;			/* Number of record segments on page */
	struct dpg_repeat {
		USHORT dpg_offset;		/* Offset of record fragment */
		USHORT dpg_length;		/* Length of record fragment */
	} dpg_rpt[1];
} *DPG;

#ifdef __cplusplus
#define DPG_SIZE	(sizeof (dpg) - sizeof (dpg::dpg_repeat))
#else
#define DPG_SIZE	(sizeof (struct dpg) - sizeof(struct dpg_repeat))
#endif

#define dpg_orphan	1			/* Data page is NOT in pointer page */
#define dpg_full	2			/* Pointer page is marked FULL */
#define dpg_large	4			/* Large object is on page */


/* Index root page */

typedef struct irt {
	struct pag irt_header;
	USHORT irt_relation;		/* relation id (for consistency) */
	USHORT irt_count;			/* Number of indices */
	struct irt_repeat {
		SLONG irt_root;			/* page number of index root */
		union {
			float irt_selectivity;	/* selectivity of index */
			SLONG irt_transaction;	/* transaction in progress */
		} irt_stuff;
		USHORT irt_desc;		/* offset to key descriptions */
		UCHAR irt_keys;			/* number of keys in index */
		UCHAR irt_flags;
	} irt_rpt[1];
} *IRT;

/* key descriptor */

typedef struct irtd {
	USHORT irtd_field;
	USHORT irtd_itype;
} IRTD;

#define irt_unique	1
#define irt_descending	2
#define irt_in_progress	4
#define irt_foreign	8
#define irt_primary	16
#define irt_expression	32

#define STUFF_COUNT	4
#define END_LEVEL	-1
#define END_BUCKET	-2




/* Header page */

typedef struct hdr {
	struct pag hdr_header;
	USHORT hdr_page_size;		/* Page size of database */
	USHORT hdr_ods_version;		/* Version of on-disk structure */
	SLONG hdr_PAGES;			/* Page number of PAGES relation */
	ULONG hdr_next_page;		/* Page number of next hdr page */
	SLONG hdr_oldest_transaction;	/* Oldest interesting transaction */
	SLONG hdr_oldest_active;	/* Oldest transaction thought active */
	SLONG hdr_next_transaction;	/* Next transaction id */
	USHORT hdr_sequence;		/* sequence number of file */
	USHORT hdr_flags;			/* Flag settings, see below */
	SLONG hdr_creation_date[2];	/* Date/time of creation */
	SLONG hdr_attachment_id;	/* Next attachment id */
	SLONG hdr_shadow_count;		/* Event count for shadow synchronization */
	SSHORT hdr_implementation;	/* Implementation number */
	USHORT hdr_ods_minor;		/* Update version of ODS */
	USHORT hdr_ods_minor_original;	/* Update version of ODS at creation */
	USHORT hdr_end;				/* offset of HDR_end in page */
	ULONG hdr_page_buffers;		/* Page buffers for database cache */
	SLONG hdr_bumped_transaction;	/* Bumped transaction id for log optimization */
	SLONG hdr_oldest_snapshot;	/* Oldest snapshot of active transactions */
	SLONG hdr_backup_pages; /* The amount of pages in files locked for backup */
	SLONG hdr_misc[3];			/* Stuff to be named later */
	UCHAR hdr_data[1];			/* Misc data */
} *HDR;

#define HDR_SIZE        OFFSETA (HDR, hdr_data)

/* Header page clumplets */

/* Data items have the format

	<type_byte> <length_byte> <data...>
*/

#define HDR_end			0
#define HDR_root_file_name	1	/* Original name of root file */
#define HDR_journal_server	2	/* Name of journal server */
#define HDR_file		3		/* Secondary file */
#define HDR_last_page		4	/* Last logical page number of file */
#define HDR_unlicensed		5	/* Count of unlicensed activity */
#define HDR_sweep_interval	6	/* Transactions between sweeps */
#define HDR_log_name		7	/* replay log name */
#define HDR_journal_file	8	/* Intermediate journal file */
#define	HDR_password_file_key	9	/* Key to compare to password db */
#define HDR_backup_info         10	/* WAL backup information */
#define HDR_cache_file		11	/* Shared cache file */
#define HDR_difference_file 12  /* Delta file that is used during backup lock */
#define HDR_backup_guid     13  /* UID generated on each switch into backup mode */
#define HDR_max			14		/* Maximum HDR_clump value */

/* Header page flags */

#define hdr_active_shadow	0x1	/* 1    file is an active shadow file */
#define hdr_force_write		0x2	/* 2    database is forced write */
#define hdr_short_journal	0x4	/* 4    short-term journalling */
#define hdr_long_journal	0x8	/* 8    long-term journalling */
#define hdr_no_checksums	0x10	/* 16   don't calculate checksums */
#define hdr_no_reserve		0x20	/* 32   don't reserve space for versions */
#define hdr_disable_cache	0x40	/* 64   disable using shared cache file */
#define hdr_shutdown		0x80	/* 128  database is shutdown */
#define hdr_SQL_dialect_3	0x100	/* 256  database SQL dialect 3 */
#define hdr_read_only		0x200	/* 512  Database in ReadOnly. If not set, DB is RW */
/* backup status mask */
#define hdr_backup_mask     0xC00

typedef struct sfd {
	SLONG sfd_min_page;			/* Minimum page number */
	SLONG sfd_max_page;			/* Maximum page number */
	UCHAR sfd_index;			/* Sequence of secondary file */
	UCHAR sfd_file[1];			/* Given file name */
} SFD;

/* Page Inventory Page */

typedef struct pip {
	struct pag pip_header;
	SLONG pip_min;				/* Lowest (possible) free page */
	UCHAR pip_bits[1];
} *PIP;


/* Pointer Page */

typedef struct ppg {
	struct pag ppg_header;
	SLONG ppg_sequence;			/* Sequence number in relation */
	SLONG ppg_next;				/* Next pointer page in relation */
	USHORT ppg_count;			/* Number of slots active */
	USHORT ppg_relation;		/* Relation id */
	USHORT ppg_min_space;		/* Lowest slot with space available */
	USHORT ppg_max_space;		/* Highest slot with space available */
	SLONG ppg_page[1];			/* Data page vector */
} *PPG;

#define ppg_bits	ppg_page

#define ppg_eof		1			/* Last pointer page in relation */

/* Transaction Inventory Page */

typedef struct tip {
	struct pag tip_header;
	SLONG tip_next;				/* Next transaction inventory page */
	UCHAR tip_transactions[1];
} *TIP;


/* Generator Page */

typedef struct gpg {
	struct pag gpg_header;
	SLONG gpg_sequence;			/* Sequence number */
	SLONG gpg_waste1;			/* overhead carried for backward compatibility */
	USHORT gpg_waste2;			/* overhead carried for backward compatibility */
	USHORT gpg_waste3;			/* overhead carried for backward compatibility */
	USHORT gpg_waste4;			/* overhead carried for backward compatibility */
	USHORT gpg_waste5;			/* overhead carried for backward compatibility */
	SINT64 gpg_values[1];		/* Generator vector */
} *GPG;


/* Record header */

typedef struct rhd {
	SLONG rhd_transaction;		/* transaction id */
	SLONG rhd_b_page;			/* back pointer */
	USHORT rhd_b_line;			/* back line */
	USHORT rhd_flags;			/* flags, etc */
	UCHAR rhd_format;			/* format version */
	UCHAR rhd_data[1];
} *RHD;

#define RHD_SIZE	OFFSETA (RHD, rhd_data)

/* Record header for fragmented record */

typedef struct rhdf {
	SLONG rhdf_transaction;		/* transaction id */
	SLONG rhdf_b_page;			/* back pointer */
	USHORT rhdf_b_line;			/* back line */
	USHORT rhdf_flags;			/* flags, etc */
	UCHAR rhdf_format;			/* format version */
	SLONG rhdf_f_page;			/* next fragment page */
	USHORT rhdf_f_line;			/* next fragment line */
	UCHAR rhdf_data[1];			/* Blob data */
} *RHDF;

#define RHDF_SIZE	OFFSETA (RHDF, rhdf_data)

/* Record header for blob header */

typedef struct blh {
	SLONG blh_lead_page;		/* First data page number */
	SLONG blh_max_sequence;		/* Number of data pages */
	USHORT blh_max_segment;		/* Longest segment */
	USHORT blh_flags;
	UCHAR blh_level;			/* Number of address levels */
	SLONG blh_count;			/* Total number of segments */
	SLONG blh_length;			/* Total length of data */
	USHORT blh_sub_type;		/* Blob sub-type */
	USHORT blh_unused;
	SLONG blh_page[1];			/* Page vector for blob pages */
} *BLH;

#define blh_data	blh_page
#define BLH_SIZE	OFFSETA (BLH, blh_page)

#define rhd_deleted	1			/* record is logically deleted */
#define rhd_chain	2			/* record is an old version */
#define rhd_fragment	4		/* record is a fragment */
#define rhd_incomplete	8		/* record is incomplete */
#define rhd_blob	16			/* isn't a record but a blob */
#define rhd_stream_blob	32		/* blob is a stream mode blob */
#define rhd_delta	32			/* prior version is differences only */
#define rhd_large	64			/* object is large */
#define rhd_damaged	128			/* object is known to be damaged */
#define rhd_gc_active	256		/* garbage collecting dead record version */

/* additions for write ahead log */

#define CTRL_FILE_LEN		255	/* Pre allocated size of file name */
#define CLUMP_ADD		0
#define CLUMP_REPLACE		1
#define CLUMP_REPLACE_ONLY	2

/* Log page */

typedef struct ctrl_pt {
	SLONG cp_seqno;
	SLONG cp_offset;
	SLONG cp_p_offset;
	SSHORT cp_fn_length;
} CP;

typedef struct log_info_page {
	struct pag log_header;
	SLONG log_flags;			/* flags            */
	CP log_cp_1;				/* control point 1      */
	CP log_cp_2;				/* control point 2      */
	CP log_file;				/* current file         */
	SLONG log_next_page;		/* Next log page        */
	SLONG log_mod_tip;			/* tip of modify transaction    */
	SLONG log_mod_tid;			/* transaction id of modify process */
	SLONG log_creation_date[2];	/* Date/time of log creation    */
	SLONG log_free[4];			/* some extra space for later use */
	USHORT log_end;				/* similar to hdr_end   */
	UCHAR log_data[1];
} *LIP;

#define LIP_SIZE	OFFSETA (LIP, log_data);

/* log flags */

#define log_recover		1		/* recovery required */
#define log_no_ail		2		/* dummy! will be used till df work */
#define log_delete		4		/* log files have been deleted */
#define log_add			8		/* log files have been added */
#define log_rec_in_progress	16	/* recovery is in progress */
#define log_partial_rebuild	32	/* partial recovery in progress */
#define log_recovery_done 	64	/* long term recovery just finished */

#define CTRL_FILE_LEN		255	/* Pre allocated size of file name */
#define CLUMP_ADD		0
#define CLUMP_REPLACE		1
#define CLUMP_REPLACE_ONLY	2

/* Log Clumplet types */

#define LOG_end			HDR_end
#define LOG_ctrl_file1		1	/* file name of 2nd last control pt */
#define LOG_ctrl_file2		2	/* file name of last ctrl pt */
#define LOG_logfile		3		/* Primary WAL file name */
#define LOG_backup_info		4	/* Journal backup directory */
#define LOG_chkpt_len		5	/* checkpoint length */
#define LOG_num_bufs		6	/* Number of log buffers */
#define LOG_bufsize		7		/* Buffer size */
#define LOG_grp_cmt_wait	8	/* Group commit wait time */
#define LOG_max			8		/* Maximum LOG_clump value */

#endif /* JRD_ODS_H */
