/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		old.c
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

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <string.h>
#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../jrd/os/pio.h"
#include "../jrd/ods.h"
#include "../jrd/pag.h"
#include "../jrd/jrn.h"
#include "../jrd/old.h"
#include "../jrd/llio.h"
#include "gen/codes.h"
#include "../wal/wal.h"
#include "../jrd/ail.h"
#include "../jrd/all_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_f_proto.h"
#include "../jrd/jrn_proto.h"
#include "../jrd/llio_proto.h"
#include "../jrd/misc_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/old_proto.h"
#include "../jrd/pag_proto.h"
#include "../jrd/thd_proto.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

static int close_cur_file(OLD, USHORT);
static int create_file(OLD, SLONG *);
static void get_wal_offset(USHORT, ULONG *, ULONG *, USHORT);
static SLONG old_dump_all_pages(OLD, PIP, SLONG, ULONG);
static int old_dump_page(OLD, ULONG);
static void old_fini(OLD *, USHORT);
static int old_init(OLD *, SCHAR *, USHORT, SSHORT, SCHAR **, ULONG, ULONG,
					SSHORT, SSHORT, ULONG, ULONG, ULONG);
static int old_put(OLD, SCHAR *, USHORT);
static void old_put_db_filename(OLD);
static int old_put_rec(OLD, JRNH *, USHORT, UCHAR *, USHORT);
static int open_next_file(OLD);


int OLD_dump(
			 TEXT * dbname,
			 USHORT db_len,
			 USHORT dump_id,
			 ULONG file_size,
			 ULONG start_page,
ULONG start_seqno, USHORT start_file, USHORT num_files, SCHAR ** files)
{
/**************************************
 *
 *	O L D _ d u m p
 *
 **************************************
 *
 * Functional description
 *	Do an online dump of the database.
 *	RETURNS:
 *		Success.
 *		error conditions
 *		Full disk - page number on which error occured.
 *			    Other relevent info to pick up where it leaves off.
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;
	WIN window;
	PGC pgc;
	UCHAR byte;
	SLONG sequence;
	ULONG seqno;
	ULONG offset;
	ULONG p_offset;
	OLD OLD_handle;
	PIP page;
	SLONG last_page;
	PIP temp_page;
	TEXT db[MAXPATHLEN];
	SSHORT file_seqno;
	SLONG block_seqno;
	SCHAR journal_dir[MAXPATHLEN], data[MAXPATHLEN];
	SSHORT jd_len, d_len;
	SLONG ret_val;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;

	PAG_get_clump(HEADER_PAGE, HDR_backup_info,
				  reinterpret_cast < USHORT * >(&d_len),
				  reinterpret_cast < UCHAR * >(data));
	PAG_get_clump(HEADER_PAGE, HDR_journal_server,
				  reinterpret_cast < USHORT * >(&jd_len),
				  reinterpret_cast < UCHAR * >(journal_dir));
	journal_dir[jd_len] = 0;

	if (!jd_len)
		ERR_post(gds_jrn_enable, 0);

	if ((ret_val = JRN_init(tdbb->tdbb_status_vector,
							&dbb->dbb_journal,
							dbb->dbb_page_size,
							reinterpret_cast < UCHAR * >(journal_dir),
							jd_len,
							reinterpret_cast < UCHAR * >(data),
							d_len)) != FB_SUCCESS)
			AIL_process_jrn_error(ret_val);

	if (db_len) {
		MOVE_FAST(dbname, db, db_len);
		db[db_len] = 0;
	}
	else
		strcpy(db, dbname);

	seqno = offset = p_offset = 0;

	if (!start_page) {
		p_offset = 0;
		dump_id = 0;

		/* First time around, send message to journal server */

		get_wal_offset(dump_id, &seqno, &offset, JRN_START_ONLINE_DMP);

		/* 
		 * Journal the Start Online Dump info to journal server.
		 * This will return the dump id of this online dump.
		 */

		if ((ret_val = JRN_put_old_start(tdbb->tdbb_status_vector,
										 dbb->dbb_journal, seqno, offset,
										 p_offset, &dump_id)) != FB_SUCCESS)
			AIL_process_jrn_error(ret_val);
	}

/* If file size is not specified, go for the maximum possible */

	if (!file_size)
		file_size = -1;

	OLD_handle = NULL;

	if (old_init(&OLD_handle,
				 db, dump_id,
				 num_files, files, file_size,
				 start_seqno, start_file,
				 JRND_SIZE + dbb->dbb_page_size, seqno, offset, p_offset)) {
		old_fini(&OLD_handle, 0);
		ERR_post(gds_old_failure, gds_arg_string,
				 "Online Dump Initialize failed", 0);
	}

/* put all secondary database file names to OLD file */

	if (!start_page)
		old_put_db_filename(OLD_handle);

/* dump all 'in use' pages in database.  */

	temp_page = (PIP) gds__alloc(dbb->dbb_page_size);

	pgc = dbb->dbb_pcontrol;

	for (sequence = 0;; sequence++) {
		window.win_page = (sequence) ? (SLONG) (sequence * pgc->pgc_ppp - 1) :
			(SLONG) pgc->pgc_pip;
		window.win_flags = 0;
		page = (PIP) CCH_FETCH(NULL_TDBB, &window, LCK_read, pag_pages);
		MOVE_FAST((UCHAR *) page, (UCHAR *) temp_page, dbb->dbb_page_size);
		CCH_RELEASE(NULL_TDBB, &window);

		if (start_page < ((sequence + 1) * pgc->pgc_ppp)) {
			last_page = old_dump_all_pages(OLD_handle, temp_page,
										   sequence, start_page);
			/* Check for error */
			if (last_page)
				break;
		}

		/* check if we are at end of database files */

		byte = temp_page->pip_bits[pgc->pgc_bytes - 1];

		/* 
		 * When pages are allocated, the last page available on a PIP
		 * is formatted as the next PIP.  A bit 1 represents a free page
		 * and a bit 0 represents an allocated page.  Since PIP pages 
		 * are never allocated, a free last bit on a PIP indicates that 
		 * it is the last PIP for this database. 
		 */

		if (byte & 0x80)
			break;
	}

	gds__free(temp_page);

	if (last_page) {
		file_seqno = OLD_handle->old_file_seqno;
		block_seqno = OLD_handle->old_block->ob_cur_seqno;
		old_fini(&OLD_handle, OLD_EOF);
		ERR_post(gds_old_no_space,
				 gds_arg_number, (SLONG) dump_id,
				 gds_arg_number, (SLONG) last_page,
				 gds_arg_number, (SLONG) block_seqno,
				 gds_arg_number, (SLONG) file_seqno, 0);
	}

/* Add record to JOURNAL SERVER */

	get_wal_offset(dump_id, &seqno, &offset, JRN_END_ONLINE_DMP);

	if ((ret_val = JRN_put_old_end(tdbb->tdbb_status_vector, dbb->dbb_journal,
								   seqno, offset, p_offset,
								   dump_id)) !=
		FB_SUCCESS) AIL_process_jrn_error(ret_val);

	old_fini(&OLD_handle, OLD_EOD);

	return FB_SUCCESS;
}


static int close_cur_file(OLD old, USHORT code)
{
/**************************************
 *
 *	c l o s e _ c u r _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Closes the current file with a code record at end
 *	returns FB_SUCCESS - if things work
 *		FB_FAILURE - close fails
 *
 **************************************/
	TDBB tdbb;
	OLD_HDR hdr;
	OLDBLK ob;
	SLONG len;

	tdbb = GET_THREAD_DATA;
	ob = old->old_block;
	hdr = ob->ob_hdr;

	hdr->oh_type = code;
	hdr->oh_hdr_len = OLD_HDROVHD;
	hdr->oh_seqno = 0;

	if (LLIO_write(0, old->old_fd, 0, 0L, LLIO_SEEK_NONE,
				   (UCHAR *) hdr, old->old_rec_size, &len) == FB_FAILURE ||
		len != old->old_rec_size)
		return FB_FAILURE;

/* flush writes to disk and close file */

	LLIO_sync(0, old->old_fd);

	if (LLIO_close(tdbb->tdbb_status_vector, old->old_fd) == FB_FAILURE)
		ERR_punt();

	old->old_fd = 0;
	old->old_cur_size = 0;

	return FB_SUCCESS;
}


static int create_file(OLD old, SLONG * ret_fd)
{
/**************************************
 *
 *	c r e a t e _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Open next file and write header & journal new name
 *	returns FB_SUCCESS - if things work
 *		FB_FAILURE - open fails 
 *
 **************************************/
	TDBB tdbb;
	SLONG fd;
	SLONG len;
	OLD_HDR hdr;
	OLDBLK ob;
	OLD_HDR_PAGE hp;
	DBB dbb;

	dbb = GET_DBB;
	tdbb = GET_THREAD_DATA;

	*ret_fd = -1;

	ob = old->old_block;
	hdr = ob->ob_hdr;

	if (LLIO_open(tdbb->tdbb_status_vector, old->old_files[old->old_cur_file],
				  LLIO_OPEN_NEW_RW, TRUE, &fd) == FB_FAILURE)
		ERR_punt();

	MOVE_CLEAR((SCHAR *) hdr, OLD_HEADER_SIZE);

	hdr->oh_type = OLD_HEADER;
	hdr->oh_hdr_len = OLD_HEADER_SIZE;
	hdr->oh_seqno = 0;

	hp = (OLD_HDR_PAGE) hdr->oh_buf;
	hp->hp_rec_size = old->old_rec_size;
	hp->hp_dump_id = old->old_dump_id;
	hp->hp_file_seqno = old->old_file_seqno;
	hp->hp_start_block = ob->ob_cur_seqno;
	hp->hp_file_size = old->old_file_size;
	hp->hp_version = OLD_VERSION;
	hp->hp_log_seqno = old->old_log_seqno;
	hp->hp_log_offset = old->old_log_offset;
	hp->hp_log_p_offset = old->old_log_p_offset;
	hp->hp_db_page_size = dbb->dbb_page_size;
	hp->hp_length = strlen(old->old_db);

	MOVE_FAST(old->old_db, hp->hp_db, hp->hp_length);

	if (LLIO_write(0, fd, 0, 0L, LLIO_SEEK_NONE,
				   (UCHAR *) hdr, OLD_HEADER_SIZE, &len) == FB_FAILURE ||
		len != OLD_HEADER_SIZE) {
		LLIO_close(0, fd);
		unlink(old->old_files[old->old_cur_file]);
		return FB_FAILURE;
	}

	old->old_cur_size = OLD_HEADER_SIZE;

	*ret_fd = fd;

	return FB_SUCCESS;
}


static void get_wal_offset(
						   USHORT dump_id,
						   ULONG * seqno, ULONG * offset, USHORT type)
{
/**************************************
 *
 *	g e t _ w a l _ o f f s e t
 *
 **************************************
 *
 * Functional description
 *	Write a record to WAL to get seqno offset pair
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;
	LTJW record;

	tdbb = GET_THREAD_DATA;
	dbb = GET_DBB;

	record.ltjw_header.jrnh_type = type;
	record.ltjw_mode = 0;
	record.ltjw_seqno = dump_id;
	record.ltjw_offset = 0;
	record.ltjw_count = 0;
	record.ltjw_length = 0;

	tdbb->tdbb_status_vector[1] = 0;

	AIL_put(dbb, tdbb->tdbb_status_vector,
			reinterpret_cast < jrnh * >(&record), LTJW_SIZE, 0, 0, 0, 0,
			seqno, offset);

	if (tdbb->tdbb_status_vector[1])
		ERR_punt();
}


static SLONG old_dump_all_pages(
								OLD OLD_handle,
								PIP pages, SLONG sequence, ULONG start_page)
{
/**************************************
 *
 *	o l d _ d u m p _ a l l _ p a g e s
 *
 **************************************
 *
 * Functional description
 *	Dump all inuse pages in this PIP to WAL
 *	Returns 	0	Everything is ok
 *			next page to be dumped
 *
 **************************************/
	DBB dbb;
	PGC pgc;
	UCHAR *end, *bytes, bit;
	SSHORT i;
	SLONG relative_bit;
	ULONG page_num;

	dbb = GET_DBB;

	pgc = dbb->dbb_pcontrol;

	end = (UCHAR *) pages + dbb->dbb_page_size;
	for (bytes = pages->pip_bits; bytes < end; bytes++) {
		for (i = 0, bit = 1; i < 8; i++, bit <<= 1)
			if (!(bit & *bytes)) {
				/* page is inuse, dump it */

				relative_bit = ((bytes - pages->pip_bits) << 3) + i;
				page_num = relative_bit + sequence * pgc->pgc_ppp;

				/* skip forward till the start page */
				if (page_num < start_page)
					continue;
				if (old_dump_page(OLD_handle, page_num))
					return page_num;
			}

	}
	return (0L);
}


static int old_dump_page(OLD OLD_handle, ULONG page_number)
{
/**************************************
 *
 *	o l d _ d u m p _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Dump a page to the WAL.  If necessary, fetch it.
 *
 **************************************/
	DBB dbb;
	WIN window;
	JRND record;
	SSHORT ret_val;

	dbb = GET_DBB;

	record.jrnd_header.jrnh_type = JRN_PAGE;
	record.jrnd_page = page_number;
	record.jrnd_length = dbb->dbb_page_size;

	window.win_page = page_number;
	window.win_flags = 0;
	CCH_FETCH_NO_CHECKSUM(NULL_TDBB, &window, LCK_read, pag_undefined);
	ret_val = old_put_rec(OLD_handle,
						  reinterpret_cast < jrnh * >(&record),
						  JRND_SIZE,
						  reinterpret_cast < UCHAR * >(window.win_buffer),
						  dbb->dbb_page_size);

	CCH_RELEASE(NULL_TDBB, &window);

	return ret_val;;
}


static void old_fini(OLD * OLD_handle, USHORT code)
{
/**************************************
 *
 *	o l d  _ f i n i
 *
 **************************************
 *
 * Functional description
 *	Finish an online dump.
 *
 **************************************/
	OLD old;

/* write a OLD_EOD record */

	if ( (old = *OLD_handle) ) {
		if (old->old_fd > 0) {
			if (close_cur_file(old, code) == FB_FAILURE)
				return;
		}

		gds__free(old->old_block->ob_hdr);
		gds__free(old->old_block);
		gds__free(old);
	}

	*OLD_handle = NULL;
}


static int old_init(
					OLD * OLD_handle,
					SCHAR * dbname,
					USHORT dump_id,
					SSHORT num_files,
					SCHAR ** files,
ULONG file_size,
ULONG start_seqno,
SSHORT start_file,
SSHORT rec_size, ULONG log_seqno, ULONG log_offset, ULONG log_p_offset)
{
/**************************************
 *
 *	o l d _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize online dump for this process.
 *	Initialize and returns the OLD_handle.
 *
 **************************************/
	OLD old;

	if (*OLD_handle != NULL)
		return FB_FAILURE;

	*OLD_handle = old = (OLD) gds__alloc(sizeof(struct old));
	MOVE_CLEAR(old, sizeof(struct old));

	old->old_block = (OLDBLK) gds__alloc(sizeof(struct oldblk));
	MOVE_CLEAR(old->old_block, sizeof(struct oldblk));

	old->old_block->ob_hdr = (OLD_HDR) gds__alloc(MAX_OLDBUFLEN);
	MOVE_CLEAR(old->old_block->ob_hdr, MAX_OLDBUFLEN);

	old->old_dump_id = dump_id;
	old->old_rec_size = rec_size + OLD_OVHD;
	old->old_log_seqno = log_seqno;
	old->old_log_offset = log_offset;
	old->old_log_p_offset = log_p_offset;
	old->old_num_files = num_files;
	old->old_files = files;
	old->old_file_size = file_size;
	old->old_db = dbname;
	old->old_file_seqno = start_file;
	old->old_block->ob_cur_seqno = start_seqno;

/* Start the online dump processing */

	return open_next_file(old);
}


static int old_put(OLD OLD_handle, SCHAR * logrec, USHORT len)
{
/**************************************
 *
 *	o l d _ p u t  
 *
 **************************************
 *
 * Functional description
 *	Put a given record in the OnLine Dump.
 *	If file size is specified, open new file if required.
 *	If we are out of space, return an error.
 *
 **************************************/
	UCHAR *p, *q;
	OLD_HDR hdr;
	OLDBLK ob;
	SLONG l;
	USHORT ret;

	ob = OLD_handle->old_block;
	hdr = ob->ob_hdr;

	if (len > MAX_OLDBUFLEN - OLD_OVHD)
		len = MAX_OLDBUFLEN - OLD_OVHD;

/* Leave space for trailer page. */

	if (OLD_handle->old_cur_size + 2 * OLD_handle->old_rec_size >
		OLD_handle->old_file_size) {
		/* Rollover to next file */

		if ((open_next_file(OLD_handle)) == FB_FAILURE)
			return FB_FAILURE;

		return old_put(OLD_handle, logrec, len);
	}

	hdr->oh_type = OLD_DATA;
	hdr->oh_hdr_len = len;
	hdr->oh_seqno = ob->ob_cur_seqno;
	p = (UCHAR *) hdr->oh_buf;
	MOVE_FAST(logrec, p, len);

	p += len;
	q = (UCHAR *) & len;
	*p++ = *q++;
	*p++ = *q++;

	ret = LLIO_write(0, OLD_handle->old_fd, 0, 0L, LLIO_SEEK_NONE,
					 (UCHAR *) hdr, OLD_handle->old_rec_size, &l);
	if (ret == FB_FAILURE || l != OLD_handle->old_rec_size) {
		/* Not enough space */
		/* backup to beginning of block and go on to next file */

		if (ret == FB_SUCCESS)
			LLIO_seek(0, OLD_handle->old_fd, 0, -l, LLIO_SEEK_CURRENT);

		if ((open_next_file(OLD_handle)) == FB_FAILURE)
			return FB_FAILURE;

		return old_put(OLD_handle, logrec, len);
	}

	ob->ob_cur_seqno++;
	OLD_handle->old_cur_size += OLD_handle->old_rec_size;

	return FB_SUCCESS;
}


static void old_put_db_filename(OLD OLD_handle)
{
/**************************************
 *
 *	o l d _ p u t _ d b _ f i l e n a m e
 *
 **************************************
 *
 * Functional description
 *     Write the file name of all secondary files to OLD
 *
 **************************************/
	DBB dbb;
	JRNF journal;
	FIL fil;

	dbb = GET_DBB;

	for (fil = dbb->dbb_file; fil; fil = fil->fil_next) {
		/* Skip primary db file */
		if (!fil->fil_min_page)
			continue;

		journal.jrnf_header.jrnh_type = JRN_NEW_FILE;
		journal.jrnf_start = fil->fil_min_page;
		journal.jrnf_sequence = fil->fil_sequence;
		journal.jrnf_length = fil->fil_length;
		old_put_rec(OLD_handle,
					reinterpret_cast < jrnh * >(&journal),
					JRNF_SIZE,
					reinterpret_cast < UCHAR * >(fil->fil_string),
					fil->fil_length);
	}
}


static int old_put_rec(
					   OLD OLD_handle,
					   JRNH * header,
					   USHORT h_length, UCHAR * data, USHORT d_length)
{
/**************************************
 *
 *	o l d _ p u t _ r e c
 *
 **************************************
 *
 * Functional description
 *	Write a journal record to WAL.
 *
 **************************************/
	USHORT length;
	UCHAR buffer[MAX_RECORD], *p, *q;

/* Prepare OLD record for writing */

	length = h_length + d_length;

	header->jrnh_handle = 0;
	header->jrnh_length = length;
	header->jrnh_series = 0;
	header->jrnh_version = JOURNAL_VERSION;

	header->jrnh_prev_seqno = 0;
	header->jrnh_prev_offset = 0;

#ifdef DEV_BUILD
/* Checksum the log record after zeroing out the field */

	header->jrnh_series =
		MISC_checksum_log_rec(reinterpret_cast < UCHAR * >(header), h_length,
							  data, d_length);
#endif /* DEV_BUILD */

/* Concatenate header and data. */

	p = buffer;
	q = (UCHAR *) header;
	do
		*p++ = *q++;
	while (--h_length);

	if (d_length)
		do
			*p++ = *data++;
		while (--d_length);

	return old_put(OLD_handle, reinterpret_cast < char *>(buffer), length);
}


static int open_next_file(OLD old)
{
/**************************************
 *
 *	o p e n _ n e x t _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Opens the next OnLine Dump file and writes out the header.
 *	returns FB_SUCCESS - if things work
 *		FB_FAILURE - open fails, no more file etc.
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;
	SLONG fd;
	SLONG ret_val;
	SCHAR name[MAXPATHLEN];

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;

	if (old->old_fd > 0) {
		if (close_cur_file(old, OLD_EOF) == FB_FAILURE)
			return FB_FAILURE;
	}

	if (old->old_cur_file >= old->old_num_files)
		return FB_FAILURE;

	if (create_file(old, &fd) == FB_FAILURE)
		return FB_FAILURE;

	ISC_expand_filename(old->old_files[old->old_cur_file], 0, name);

	if (
		(ret_val =
		 JRN_put_old_file(tdbb->tdbb_status_vector, dbb->dbb_journal, name,
						  strlen(name), old->old_file_size,
						  old->old_file_seqno, old->old_dump_id)) != FB_SUCCESS) {
		LLIO_close(0, fd);
		AIL_process_jrn_error(ret_val);
	}

	old->old_cur_file++;
	old->old_file_seqno++;

	old->old_fd = fd;

	return FB_SUCCESS;
}
