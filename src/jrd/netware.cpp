/*
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
 * 2001.07.06 Sean Leyne - Code Cleanup, removed "#ifdef READONLY_DATABASE"
 *                         conditionals, as the engine now fully supports
 *                         readonly databases.
 */
#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dfs.h>
#include <share.h>
#include <fileengd.h>

#include "../jrd/jrd.h"
#include "../jrd/pio.h"
#include "../jrd/ods.h"
#include "../jrd/lck.h"
#include "../jrd/cch.h"
#include "gen/codes.h"


extern "C" {


#define MAXPATHLEN	1024
#define IO_RETRY	20
#define BYTES_PER_SECTOR (4096/8)

extern int errno;
extern void CCH_down_grade_dbb(DBB);

PIO_add_file(DBB dbb, FIL main_file, TEXT * file_name, SLONG start);
PIO_close(FIL main_file);
FIL PIO_create(DBB dbb, TEXT * string, SSHORT length, BOOLEAN overwrite);
PIO_connection(TEXT * file_name, USHORT * file_length);
PIO_expand(TEXT * file_name, USHORT file_length, TEXT * expanded_name);
PIO_flush(FIL main_file);
PIO_force_write(FIL file, USHORT flag);
PIO_header(DBB dbb, SCHAR * address, int length);
PIO_max_alloc(DBB dbb);
PIO_read(FIL file, BDB bdb, PAG page, STATUS * status_vector);
PIO_write(FIL file, BDB bdb, PAG page, STATUS * status_vector);
FIL PIO_open(DBB dbb, TEXT * string, SSHORT length, SSHORT trace_flag,
			 BLK connection, TEXT * file_name, USHORT file_length);

int NWDFS_open(SCHAR * file_name, int rights);
int NWDFS_close(FIL fil);
int NWDFS_write(FIL fil, int length, SCHAR * buffer);
int NWDFS_read(FIL fil, int length, SCHAR * buffer);
int NWDFS_lseek(FIL fil, int pos, int sense);
int NWDFS_get_file_size(FIL fil);

int dfs_expand_file(FIL fil, int size);
int dfs_expand(FIL fil, int length, int start);
int dfs_write(FIL fil, int start, int number, SCHAR * buffer);
FIL dfs_setup_file(DBB dbb, UCHAR * file_name, USHORT file_length, int desc);
FIL dfs_seek_file(FIL file, BDB bdb, STATUS * status_vector);
int expand_it(FIL fil, int blocks_to_expand, int starting_block);

PIO_add_file(dbb, main_file, file_name, start)
	 DBB dbb;
	 FIL main_file;
	 TEXT *file_name;
	 SLONG start;
{
/**************************************
 *
 *	P I O _ a d d _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Add a file to an existing database.  Return the sequence
 *	number of the new file.  If anything goes wrong, return a
 *	sequence of 0.
 *	NOTE:  This routine does not lock any mutexes on
 *	its own behalf.  It is assumed that mutexes will
 *	have been locked before entry.
 *
 **************************************/
	USHORT sequence;
	FIL file, new_file;

	if (!(new_file = PIO_create(dbb, file_name, strlen(file_name), FALSE)))
		return 0;

	new_file->fil_min_page = start;
	sequence = 1;

	for (file = main_file; file->fil_next; file = file->fil_next)
		++sequence;

	file->fil_max_page = start - 1;
	file->fil_next = new_file;

	return sequence;
}


PIO_close(main_file)
	 FIL main_file;
{
/**************************************
 *
 *	P I O _ c l o s e
 *
 **************************************
 *
 * Functional description
 *	NOTE:  This routine does not lock any mutexes on
 *	its own behalf.  It is assumed that mutexes will
 *	have been locked before entry.
 *
 **************************************/
	FIL file;
	USHORT i;

	for (file = main_file; file; file = file->fil_next)
		if (file->fil_desc)
			NWDFS_close(file);
}


FIL PIO_create(dbb, string, length, overwrite)
	 DBB dbb;
	 TEXT *string;
	 SSHORT length;
	 BOOLEAN overwrite;
{
/**************************************
 *
 *	P I O _ c r e a t e
 *
 **************************************
 *
 * Functional description
 *	Create a new database file.
 *	NOTE:  This routine does not lock any mutexes on
 *	its own behalf.  It is assumed that mutexes will
 *	have been locked before entry.
 *
 **************************************/
	int desc;
	FIL file;
	TEXT *address, expanded_name[256], temp[256], *file_name;

	file_name = string;

	if (length) {
		MOVE_FAST(file_name, temp, length);
		temp[length] = 0;
		file_name = temp;
	}

	if (
		(desc =
		 NWDFS_open(file_name,
					O_CREAT | (overwrite ? O_TRUNC : O_EXCL) | O_BINARY |
					O_RDWR)) == -1)
		ERR_post(isc_io_error, gds_arg_string, "open O_CREAT",
				 gds_arg_cstring, length, ERR_string(string, length),
				 isc_arg_gds, isc_io_create_err, gds_arg_netware, errno, 0);

	/* File create succeeded.  Now expand the file name. */
	length = PIO_expand(string, length, expanded_name);
	file = dfs_setup_file(dbb, expanded_name, length, desc);

	return file;
}


PIO_connection(file_name, file_length)
	 TEXT *file_name;
	 USHORT *file_length;
{
/**************************************
 *
 *	P I O _ c o n n e c t i o n
 *
 **************************************
 *
 * Functional description
 *	Analyze a file specification and determine whether a page/lock
 *	server is required and available.  If so, return a "connection"
 *	block.  If not, return NULL.
 *
 *	Note: The file name must have been expanded prior to this call.
 *
 **************************************/
	return NULL;
}


PIO_expand(file_name, file_length, expanded_name)
	 TEXT *file_name;
	 USHORT file_length;
	 TEXT *expanded_name;
{
/**************************************
 *
 *	P I O _ e x p a n d
 *
 **************************************
 *
 * Functional description
 *	Fully expand a file name.  If the file doesn't exist, do something
 *	intelligent.
 *
 **************************************/

	return ISC_expand_filename(file_name, file_length, expanded_name);
}


PIO_flush(main_file)
	 FIL main_file;
{
/**************************************
 *
 *	P I O _ f l u s h
 *
 **************************************
 *
 * Functional description
 *	Flush the operating system cache back to good, solid oxide.
 *
 **************************************/
}


PIO_force_write(file, flag)
	 FIL file;
	 USHORT flag;
{
/**************************************
 *
 *	P I O _ f o r c e _ w r i t e
 *
 **************************************
 *
 * Functional description
 *	Set (or clear) force write, if possible, for the database.
 *
 **************************************/
}


PIO_header(dbb, address, length)
	 DBB dbb;
	 SCHAR *address;
	 int length;
{
/**************************************
 *
 *	P I O _ h e a d e r
 *
 **************************************
 *
 * Functional description
 *	Read the page header.  This assumes that the file has not been
 *	repositioned since the file was originally mapped.
 *
 **************************************/
	FIL file;

	file = dbb->dbb_file;
	ISC_inhibit();
	if ((NWDFS_lseek(file, 0, 0)) == -1)
		netware_error("lseek", file, isc_io_read_err, 0);

#ifdef ISC_DATABASE_ENCRYPTION
	if (dbb->dbb_encrypt_key) {
		SCHAR spare_buffer[MAX_PAGE_SIZE];

		if ((NWDFS_read(file, length, spare_buffer)) == 0)
			netware_error("read", file, isc_io_read_err, 0);

		(*dbb->dbb_decrypt) (dbb->dbb_encrypt_key->str_data, spare_buffer,
							 length, address);
	}
	else
#endif
	if ((NWDFS_read(file, length, address)) == 0)
		netware_error("read", file, isc_io_read_err, 0);

	ISC_enable();
}


PIO_max_alloc(dbb)
	 DBB dbb;
{
/**************************************
 *
 *	P I O _ m a x _ a l l o c
 *
 **************************************
 *
 * Functional description
 *	Compute last physically allocated page of database.
 *
 **************************************/
	struct stat statistics;
	int length;
	FIL file;

	for (file = dbb->dbb_file; file->fil_next; file = file->fil_next);

	length = NWDFS_get_file_size(file);

	return file->fil_min_page - file->fil_fudge +
		(length + dbb->dbb_page_size - 1) / dbb->dbb_page_size;
}


PIO_act_alloc(dbb)
	 DBB dbb;
{
/**************************************
 *
 *	P I O _ a c t _ a l l o c
 *
 **************************************
 *
 * Functional description
 *	Compute actual number of physically allocated pages of database.
 *
 **************************************/
	int length;
	FIL file;
	SLONG tot_pages = 0;

	for (file = dbb->dbb_file; file != NULL; file = file->fil_next) {
		length = NWDFS_get_file_size(file);
		tot_pages += (length + dbb->dbb_page_size - 1) / dbb->dbb_page_size;
	}
	return tot_pages;
}


FIL PIO_open(dbb, string, length, trace_flag, connection, file_name,
			 file_length)
	 DBB dbb;
	 TEXT *string;
	 SSHORT length, trace_flag;
	 BLK connection;
	 TEXT *file_name;
	 USHORT file_length;
{
/**************************************
 *
 *	P I O _ o p e n
 *
 **************************************
 *
 * Functional description
 *	Open a database file.  If a "connection" block is provided, use
 *	the connection to communication with a page/lock server.
 *
 **************************************/
	TEXT *ptr, temp[256];
	int desc, i;

	if (string) {
		ptr = string;
		if (length) {
			MOVE_FAST(string, temp, length);
			temp[length] = 0;
			ptr = temp;
		}
	}
	else {
		ptr = file_name;
		if (file_length) {
			MOVE_FAST(file_name, temp, file_length);
			temp[file_length] = 0;
			ptr = temp;
		}
	}
	for (i = 0; i < IO_RETRY; i++)
		if ((desc = NWDFS_open(ptr, O_RDWR)) != -1)
			break;

	if (desc == -1) {
		/* Try opening the database file in ReadOnly mode. The database file could
		 * be on a RO medium (CD-ROM etc.). If this fileopen fails, return error.
		 */
		if ((desc = NWDFS_open(ptr, O_RDONLY)) == -1) {
			ERR_post(isc_io_error,
					 gds_arg_string, "open",
					 gds_arg_cstring, file_length, ERR_string(file_name,
															  file_length),
					 isc_arg_gds, isc_io_open_err, gds_arg_netware, errno, 0);
		}
		else {
			/* If this is the primary file, set DBB flag to indicate that it is
			 * being opened ReadOnly. This flag will be used later to compare with
			 * the Header Page flag setting to make sure that the database is set
			 * ReadOnly.
			 */
			if (!dbb->dbb_file)
				dbb->dbb_flags |= DBB_being_opened_read_only;
		}
	}

	return dfs_setup_file(dbb, string, length, desc);
}


PIO_read(file, bdb, page, status_vector)
	 FIL file;
	 BDB bdb;
	 PAG page;
	 STATUS *status_vector;
{
/**************************************
 *
 *	P I O _ r e a d
 *
 **************************************
 *
 * Functional description
 *	Read a data page.  Oh wow.
 *
 **************************************/
	DBB dbb;
	SSHORT i;
	SLONG bytes, size;

	ISC_inhibit();
	dbb = bdb->bdb_dbb;
	size = dbb->dbb_page_size;

#ifdef ISC_DATABASE_ENCRYPTION
	if (dbb->dbb_encrypt_key) {
		SCHAR spare_buffer[MAX_PAGE_SIZE];

		for (i = 0; i < IO_RETRY; i++) {
			if (!(file = dfs_seek_file(file, bdb, status_vector)))
				return FALSE;

			if ((bytes = NWDFS_read(file, size, spare_buffer)) == size) {
				(*dbb->dbb_decrypt) (dbb->dbb_encrypt_key->str_data,
									 spare_buffer, size, page);
				break;
			}
			else {
				ConsolePrintf("read error on PIO_read\n");
				Breakpoint(1);
			}
		}
	}
	else
#endif
	{
		for (i = 0; i < IO_RETRY; i++) {
			if (!(file = dfs_seek_file(file, bdb, status_vector)))
				return FALSE;

			if ((bytes = NWDFS_read(file, size, (SCHAR *) page)) == size)
				break;
			else {
				ConsolePrintf("read error on PIOread\n");
				Breakpoint(1);
			}
		}
	}

	if (i == IO_RETRY)
		return netware_error("read", file, isc_io_read_err, status_vector);

	ISC_enable();
	return TRUE;
}


PIO_write(file, bdb, page, status_vector)
	 FIL file;
	 BDB bdb;
	 PAG page;
	 STATUS *status_vector;
{
/**************************************
 *
 *	P I O _ w r i t e
 *
 **************************************
 *
 * Functional description
 *	Write a data page.  Oh wow.
 *
 **************************************/
	DBB dbb;
	SSHORT i;
	SLONG bytes, size;
	int page_no;

	ISC_inhibit();
	dbb = bdb->bdb_dbb;
	size = dbb->dbb_page_size;

#ifdef ISC_DATABASE_ENCRYPTION
	if (dbb->dbb_encrypt_key) {
		SCHAR spare_buffer[MAX_PAGE_SIZE];

		(*dbb->dbb_encrypt) (dbb->dbb_encrypt_key->str_data,
							 page, size, spare_buffer);

		for (i = 0; i < IO_RETRY; i++) {
			if (!(file = dfs_seek_file(file, bdb, status_vector)))
				return FALSE;
			if ((bytes = NWDFS_write(file, size, spare_buffer)) == size)
				break;
			else {
				ConsolePrintf("write error on PIO_write\n");
				Breakpoint(1);
			}
		}
	}
	else
#endif
	{
		for (i = 0; i < IO_RETRY; i++) {
			if (!(file = dfs_seek_file(file, bdb, status_vector)))
				return FALSE;
			page_no = bdb->bdb_page;

			if ((bytes = NWDFS_write(file, size, (SCHAR *) page)) == size)
				break;
			else {
				ConsolePrintf("write error on PIO_write\n");
				Breakpoint(1);
			}
		}
	}

	if (i == IO_RETRY)
		return netware_error("write", file, isc_io_write_err, status_vector);

	ISC_enable();

	return TRUE;
}


static FIL dfs_seek_file(file, bdb, status_vector)
	 FIL file;
	 BDB bdb;
	 STATUS *status_vector;
{
/**************************************
 *
 *	d f s _ s e e k _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Given a buffer descriptor block, find the appropropriate
 *	file block and seek to the proper page in that file.
 *
 **************************************/
	ULONG page;
	DBB dbb;

	dbb = bdb->bdb_dbb;
	page = bdb->bdb_page;

	for (;; file = file->fil_next)
		if (!file) {
			ISC_enable();
			CORRUPT(158);		/* msg 158 cannot sort on a field that does not exist */
		}
		else if (page >= file->fil_min_page && page <= file->fil_max_page)
			break;

	page -= file->fil_min_page - file->fil_fudge;

	if ((NWDFS_lseek(file, (SLONG) (page * dbb->dbb_page_size), 0)) == -1)
		return (FIL) netware_error("lseek", file, isc_io_access_err,
								   status_vector);

	return file;
}


static FIL dfs_setup_file(dbb, file_name, file_length, desc)
	 DBB dbb;
	 UCHAR *file_name;
	 USHORT file_length;
	 int desc;
{
/**************************************
 *
 *	d f s _ s e t u p _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Set up file and lock blocks for a file.
 *
 **************************************/
	FIL file;
	LCK lock;
	UCHAR *p, *q, lock_string[32];
	USHORT l;
	struct stat statistics;
	struct VolumeInformationStructure vol_info;
	ULONG len;

	/* Allocate file block and copy file name string */
	file = FB_NEW_RPT(*dbb->dbb_permanent, file_length + 1) fil();
	file->fil_desc = desc;
	file->fil_length = file_length;
	file->fil_max_page = -1;
	MOVE_FAST(file_name, file->fil_string, file_length);
	file->fil_string[file_length] = '\0';

	stat(file_name, &statistics);

	/* fill in dfs info */
	file->dfs_volume = statistics.st_dev;
	DFSReturnVolumeMappingInformation(file->dfs_volume, &vol_info);
	file->dfs_block_size = vol_info.VolumeAllocationUnitSizeInBytes;
	file->dfs_position = 0;
	file->dfs_size = filelength(desc);
	len = filelength(desc);
	file->dfs_last_block = file->dfs_size / file->dfs_block_size;
	if ((file->dfs_size % file->dfs_block_size) == 0)
		file->dfs_last_block--;
	file->dfs_buffer = malloc(dbb->dbb_page_size);

	/* If this isn't the primary file, we're done */
	if (dbb->dbb_file)
		return file;

	/* Build unique lock string for file and construct lock block */
	p = lock_string;
	q = (UCHAR *) & statistics.st_dev;
	l = sizeof(statistics.st_dev);
	do
		*p++ = *q++;
	while (--l);

	q = (UCHAR *) & statistics.st_ino;
	l = sizeof(statistics.st_ino);
	do
		*p++ = *q++;
	while (--l);

	l = p - lock_string;

	dbb->dbb_lock = lock = FB_NEW_RPT(*dbb->dbb_permanent, l) lck();
	lock->lck_type = LCK_database;
	lock->lck_owner_handle = LCK_get_owner_handle(NULL_TDBB, lock->lck_type);
	lock->lck_object = reinterpret_cast<blk*>(dbb);
	lock->lck_length = l;
	lock->lck_dbb = dbb;
	lock->lck_ast = CCH_down_grade_dbb;
	MOVE_FAST(lock_string, lock->lck_key.lck_string, l);

	/* Try to get an exclusive lock on database.  If this fails, insist
	   on at least a shared lock */
	dbb->dbb_flags |= DBB_exclusive;
	if (!LCK_lock(NULL_TDBB, lock, LCK_EX, FALSE)) {
		dbb->dbb_flags &= ~DBB_exclusive;
		LCK_lock(NULL_TDBB, lock, LCK_SR, TRUE);
	}

	return file;
}


static netware_error(string, file, operation, status_vector)
	 SCHAR *string;
	 FIL file;
	 STATUS operation;
	 STATUS *status_vector;
{
/**************************************
 *
 *	n e t w a r e _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Somebody has noticed a file system error and expects error
 *	to do something about it.  Harumph!
 *
 **************************************/

// Breakpoint(1);

	ISC_enable();
	if (status_vector) {
		*status_vector++ = isc_arg_gds;
		*status_vector++ = isc_io_error;
		*status_vector++ = gds_arg_string;
		*status_vector++ = (STATUS) string;
		*status_vector++ = gds_arg_string;
		*status_vector++ =
			(STATUS) ERR_string(file->fil_string, file->fil_length);
		*status_vector++ = isc_arg_gds;
		*status_vector++ = operation;
		*status_vector++ = gds_arg_netware;
		*status_vector++ = errno;
		*status_vector++ = gds_arg_end;
		return FALSE;
	}
	else
		ERR_post(isc_io_error,
				 gds_arg_string, string,
				 gds_arg_string, ERR_string(file->fil_string,
											file->fil_length), isc_arg_gds,
				 operation, gds_arg_netware, errno, 0);
}



/* DFS STUFF !!!!!*/

int NWDFS_open(SCHAR * file_name, int access)
/**************************************
 *
 * N W D F S _ o p e n
 *
 **************************************
 *
 * Functional description
 * open a DFS file
 *
 **************************************/
{
	int desc;

	if ((desc = DFSsopen(file_name, access, SH_COMPAT, S_IREAD | S_IWRITE,
						 NO_RIGHTS_CHECK_ON_OPEN_BIT, DOSNameSpace)) == -1) {
		nerr(1);
		return -1;
	}
	return (desc);
}

int NWDFS_lseek(FIL fil, int pos, int whence)
/**************************************
 *
 * N W D F S _ l s e e k
 *
 **************************************
 *
 * Functional description
 * seek a DFS file
 *
 **************************************/
{


	switch (whence) {
	case 0:
		fil->dfs_position = pos;
		break;

	case 1:
		fil->dfs_position += pos;
		break;

	case 2:
		fil->dfs_position = fil->dfs_size - pos;
		break;

	default:
		break;
	}
	return (fil->dfs_position);
}

int NWDFS_write(FIL fil, int length, SCHAR * buffer)
/**************************************
 *
 * N W D F S _ w r i t e
 *
 **************************************
 *
 * Functional description
 * write to a DFS file
 *
 **************************************/
{
	int starting_sector;
	int new_position;
	int ccode;
	int bytes_left_in_sector;
	int number_of_sectors;
	int bytes_to_copy;
	int bytes_written;
	int offset_in_sector;
	int total_written;

	total_written = length;

	new_position = fil->dfs_position + length;

	if (new_position > fil->dfs_size)
		if (dfs_expand_file(fil, new_position) != 0) {
			nerr(2);
			return (0);
		}
	starting_sector = fil->dfs_position / BYTES_PER_SECTOR;
	offset_in_sector = fil->dfs_position % BYTES_PER_SECTOR;

	/* if we are not at sector boundary :
	   1) read entire sector into temporary buffer
	   2) copy as many bytes as we can from original buffer to temporary buf
	   3) re-write sector */

	if (offset_in_sector != 0) {
		if (ccode =
			DFSRead(fil->fil_desc, starting_sector, 1, fil->dfs_buffer)) {
			nerr(3);
			return (0);
		}

		bytes_left_in_sector = BYTES_PER_SECTOR - offset_in_sector;

		if (bytes_left_in_sector > length)
			bytes_to_copy = length;
		else
			bytes_to_copy = bytes_left_in_sector;

		memcpy(fil->dfs_buffer + offset_in_sector, buffer, bytes_to_copy);

		ccode = dfs_write(fil, starting_sector, 1, fil->dfs_buffer);
		if (ccode == -1)
			return (0);

		if (ccode) {
			nerr(4);
			return (0);
		}

		length -= bytes_to_copy;
		buffer += bytes_to_copy;
		starting_sector++;
	}

	/* we are now at start of new sector:
	   1) if it will take of more that on sector
	   write out rest of sectors */

	number_of_sectors = length / BYTES_PER_SECTOR;

	if (number_of_sectors > 0) {
		ccode = dfs_write(fil, starting_sector, number_of_sectors, buffer);

		if (ccode == -1)
			return (0);

		if (ccode) {
			nerr(6);
			return (0);
		}

		starting_sector += number_of_sectors;
		bytes_written = BYTES_PER_SECTOR * number_of_sectors;
		buffer += bytes_written;
		length -= bytes_written;
	}

	/* if last bit spills out of last sector
	   1) read sector into temp buffer
	   2) copy last bit to temporary buffer
	   2) re-write sector */

	if (length) {
		if (ccode =
			DFSRead(fil->fil_desc, starting_sector, 1, fil->dfs_buffer)) {
			nerr(7);
			return (0);
		}
		memcpy(fil->dfs_buffer, buffer, length);

		ccode = dfs_write(fil, starting_sector, 1, fil->dfs_buffer);
		if (ccode == -1)
			return (0);

		if (ccode) {
			nerr(8);
			return (0);
		}
	}

	fil->dfs_position = new_position;
	return (total_written);
}

int NWDFS_read(FIL fil, int length, SCHAR * buffer)
/**************************************
 *
 * N W D F S _ r e a d
 *
 **************************************
 *
 * Functional description
 * read a DFS file
 *
 **************************************/
{
	int starting_sector;
	int new_position;
	int ccode;
	int bytes_left_in_sector;
	int number_of_sectors;
	int bytes_to_copy;
	int bytes_read;
	int offset_in_sector;
	int total_bytes = 0;

	new_position = fil->dfs_position + length;
	if (new_position > fil->dfs_size)
		if (dfs_expand_file(fil, new_position) == -1) {
			nerr(2);
			return (0);
		}

	starting_sector = fil->dfs_position / BYTES_PER_SECTOR;
	offset_in_sector = fil->dfs_position % BYTES_PER_SECTOR;

	/* if we are not at sector boundary :
	   1) read entire sector into temporary buffer
	   2) copy bytes from current position to end of sector into buffer */

	if (offset_in_sector != 0) {
		if (ccode =
			DFSRead(fil->fil_desc, starting_sector, 1, fil->dfs_buffer)) {
			nerr(10);
			return (0);
		}
		bytes_left_in_sector = BYTES_PER_SECTOR - offset_in_sector;

		if (bytes_left_in_sector > length)
			bytes_to_copy = length;
		else
			bytes_to_copy = bytes_left_in_sector;

		memcpy(buffer, fil->dfs_buffer + offset_in_sector, bytes_to_copy);

		total_bytes += bytes_to_copy;
		length -= bytes_to_copy;
		buffer += bytes_to_copy;
		starting_sector++;
	}

	/* if more than one sector left to read, read all sectors up to last
	   sector */

	number_of_sectors = length / BYTES_PER_SECTOR;

	if (number_of_sectors > 0) {
		ccode =
			DFSRead(fil->fil_desc, starting_sector, number_of_sectors,
					buffer);
		if (ccode) {
			nerr(11);
			return (0);
		}
		starting_sector += number_of_sectors;

		bytes_read = BYTES_PER_SECTOR * number_of_sectors;
		total_bytes += bytes_read;
		buffer += bytes_read;
		length -= bytes_read;
	}

	/* if still some bytes spilling into last sector read them */
	if (length > 0) {
		if (ccode =
			DFSRead(fil->fil_desc, starting_sector, 1, fil->dfs_buffer)) {
			nerr(12);
			return (0);
		}
		memcpy(buffer, fil->dfs_buffer, length);
		total_bytes += length;
	}

	fil->dfs_position = new_position;
	return total_bytes;
}

int NWDFS_close(FIL fil)
/**************************************
 *
 * N W D F S _ c l o s e
 *
 **************************************
 *
 * Functional description
 * close a DFS file
 *
 **************************************/
{
	free(fil->dfs_buffer);
	return (DFSclose(fil->fil_desc));
}

int NWDFS_get_file_size(FIL fil)
/**************************************
 *
 * N W D F S _ g e t _ f i l e _ s i z e
 *
 **************************************
 *
 * Functional description
 * get size of a DFS file
 *
 **************************************/
{
	return (fil->dfs_size);
}

int dfs_write(FIL fil, int starting_sector, int number_of_sectors,
			  SCHAR * buffer)
/**************************************
 *
 * d f s _ w r i t e
 *
 **************************************
 *
 * Functional description
 * low level dfs write
 *
 **************************************/
{
	int ccode;
	int starting_block;
	int byte_offset_in_file;
	int bytes_to_expand;

	/* try to write all sectors sequentially */
	ccode =
		DFSWrite(fil->fil_desc, starting_sector, number_of_sectors, buffer);

	/* success */
	if (ccode == 0)
		return (0);

	/* could not write all sectors sequentially because there is a hole */
	if (ccode == DFSHoleInFileError) {
		byte_offset_in_file = starting_sector * BYTES_PER_SECTOR;
		starting_block = byte_offset_in_file / fil->dfs_block_size;

		/* if it is on a block boundary and it is not block zero
		   start at previous block */
		if ((byte_offset_in_file % fil->dfs_block_size) == 0)
			if (byte_offset_in_file > 0)
				starting_block--;

		bytes_to_expand = number_of_sectors * BYTES_PER_SECTOR;
		ccode = dfs_expand(fil, bytes_to_expand, starting_block);
		if (ccode) {
			nerr(16);
			return (ccode);
		}
		return (-1);
	}
	else {
		nerr(17);
		return (ccode);
	}
	return (0);
}

int dfs_expand_file(FIL fil, int length)
/**************************************
 *
 * d f s _ e x p a n d _ f i l e
 *
 **************************************
 *
 * Functional description
 * dfs expand
 *
 **************************************/
{
	int bytes_to_expand;
	int starting_block;
	int ccode;
	int number_of_blocks_expanded;

	/* set new EOF */
	ccode = DFSSetEndOfFile(fil->fil_desc, length, 0);
	if (ccode) {
		nerr(18);
		return (ccode);
	}
	bytes_to_expand = length - fil->dfs_size;

	starting_block = fil->dfs_last_block + 1;

	ccode = dfs_expand(fil, bytes_to_expand, starting_block);
	if (ccode) {
		nerr(19);
		return (ccode);
	}
	fil->dfs_size = filelength(fil->fil_desc);

	number_of_blocks_expanded = (bytes_to_expand + fil->dfs_block_size - 1) /
		fil->dfs_block_size;
	fil->dfs_last_block += number_of_blocks_expanded;

	return (0);
}

void nerr(int err)
{
	ConsolePrintf("error %d\n", err);
	Breakpoint(1);
}

void read_back_and_check(PAG page, BDB bdb, FIL file)
{
	USHORT read_checksum;
	USHORT written_checksum;
	PAG page_read;
	DBB dbb;
	int saved_pos;
	int page_no;

	Breakpoint(1);
	dbb = bdb->bdb_dbb;
	page_read = malloc(dbb->dbb_page_size);
	page_no = bdb->bdb_page;
	page_no -= file->fil_min_page - file->fil_fudge;

	saved_pos = file->dfs_position;
	file->dfs_position = page_no * dbb->dbb_page_size;
	NWDFS_read(file, dbb->dbb_page_size, (SCHAR *) page_read);
	file->dfs_position = saved_pos;

	read_checksum = page_read->pag_checksum;
	written_checksum = page->pag_checksum;

	if (read_checksum != written_checksum) {
// ConsolePrintf("read = %d write = %d page = %d\n",
//                read_checksum, written_checksum, page_no);
		Breakpoint(1);
	}
}

int dfs_expand(FIL fil, int bytes_to_expand, int starting_block)
/**************************************
 *
 * d f s _ e x p a n d
 *
 **************************************
 *
 * Functional description
 * dfs expand
 *
 **************************************/
{
	int blocks_to_try;
	int total_blocks_expanded = 0;
	int done = FALSE;
	int blocks_to_expand;
	int ccode;

	blocks_to_try = (bytes_to_expand + fil->dfs_block_size - 1) /
		fil->dfs_block_size;

	blocks_to_expand = blocks_to_try;

	while (!done) {
		ccode = expand_it(fil, blocks_to_try, starting_block);

		switch (ccode) {
		case 0:
			total_blocks_expanded += blocks_to_try;
			if (total_blocks_expanded == blocks_to_expand)
				done = TRUE;
			else {
				blocks_to_try = blocks_to_expand - total_blocks_expanded;
				starting_block += blocks_to_try;
			}
			break;

		case DFSOverlapError:
			if (blocks_to_try == 1) {
				ConsolePrintf("blocks to try = 1 and overlap error\n");
				return (-1);
			}
			blocks_to_try = 1;
			break;

		case DFSBoundryError:
			if (blocks_to_try == 1) {
				ConsolePrintf("blocks to try = 1 and boundry error\n");
				return (-1);
			}
			blocks_to_try = blocks_to_try / 2;
			break;

		default:
			ConsolePrintf("default ccode = %d\n", ccode);
			return (-1);
		}
	}
	return (0);
}

int expand_it(FIL fil, int blocks_to_expand, int starting_block)
/**************************************
 *
 * e x p a n d _ i t
 *
 **************************************
 *
 * Functional description
 * dfs expand
 *
 **************************************/
{
	struct VolumeInformationStructure volinfo;
	int blocks_available;
	int ccode;

	DFSReturnVolumeMappingInformation(fil->dfs_volume, &volinfo);

	blocks_available = volinfo.AllocationUnitsFreelyAvailable +
		volinfo.AllocationUnitsInAvailableDeletedFiles;

	if (blocks_to_expand > blocks_available)
		return (-1);

	DFSFreeLimboVolumeSpace(fil->dfs_volume, blocks_to_expand);

	ccode = DFSExpandFile(fil->fil_desc, starting_block,
						  blocks_to_expand, -1, -1);

	return (ccode);
}


} // extern "C"
