/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		old.h
 *	DESCRIPTION:	
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

#ifndef JRD_OLD_H
#define JRD_OLD_H

#include "../jrd/common.h"

#define OLD_VERSION1		1
#define OLD_VERSION		OLD_VERSION1

#define MAX_OLDBUFLEN		64000

/* OLD block buffer header */

typedef struct old_hdr {
	UCHAR oh_type;				/* type of record */
	USHORT oh_hdr_len;			/* Length of the block buffer */
	SLONG oh_seqno;				/* seqno of the block   */
	SLONG oh_hdr_timestamp[2];	/* When this buffer was written */
	SCHAR oh_buf[1];
} *OLD_HDR;

#define	OLD_HEADER	1
#define OLD_DATA	2
#define OLD_ERR		253
#define	OLD_EOF		254
#define OLD_EOD		255

#define OLD_HDROVHD             OFFSETA (OLD_HDR, oh_buf)
#define OLD_TAILOVHD            sizeof(SSHORT)
#define OLD_OVHD                OLD_HDROVHD + OLD_TAILOVHD
#define OLD_HEADER_SIZE		512	/* make it 256 later */


/* OLD buffer block */

typedef struct oldblk {
	SLONG ob_cur_offset;
	SLONG ob_cur_seqno;
	struct old_hdr *ob_hdr;
} *OLDBLK;


/* OLD handle, used in the API calls.*/

typedef struct old {
	SCHAR *old_db;				/* name of database */
	SLONG old_fd;				/* File descriptor for the OLD lock file */
	USHORT old_dump_id;
	ULONG old_log_seqno;		/* log seqno of OLD */
	ULONG old_log_offset;		/* log offset of OLD */
	ULONG old_log_p_offset;		/* log p_offset of OLD */
	ULONG old_db_page_size;
	SSHORT old_num_files;		/* number of files to be used */
	SCHAR **old_files;			/* list of files passed in  */
	SSHORT old_cur_file;		/* current file being written to */
	SSHORT old_file_seqno;		/* file seqno being written to */
	SSHORT old_rec_size;		/* size of each record */
	ULONG old_file_size;		/* max file size */
	SLONG old_cur_size;			/* current file size */
	SLONG old_cur_seqno;		/* used during read processing */
	int old_length;				/* length of the OLD segement */
	OLDBLK old_block;
} *OLD;

/* OnLine Dump header page info */

typedef struct hdr_page {
	USHORT hp_file_seqno;		/* Sequence number of current file */
	ULONG hp_start_block;		/* start block number */
	ULONG hp_file_size;			/* max size of file */
	ULONG hp_log_seqno;			/* old start log seqno */
	ULONG hp_log_offset;		/* old start log offset */
	ULONG hp_log_p_offset;		/* old start log offset */
	USHORT hp_db_page_size;		/* database page size */
	USHORT hp_dump_id;			/* dump id */
	USHORT hp_rec_size;
	USHORT hp_version;			/* OnLine Dump Version */
	USHORT hp_length;			/* length of file name */
	SCHAR hp_db[1];				/* database name */
} *OLD_HDR_PAGE;

#endif /* JRD_OLD_H */
