/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		unix.c
 *	DESCRIPTION:	UNIX (BSD) specific physical IO
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
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <io.h>

#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../jrd/pio.h"
#include "../jrd/ods.h"
#include "../jrd/lck.h"
#include "../jrd/cch.h"
#include "gen/codes.h"
#include "../jrd/all_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_f_proto.h"
#include "../jrd/isc_i_proto.h"

#include "../jrd/lck_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/pio_proto.h"
#include "../jrd/thd_proto.h"
#include "../jrd/doserr.h"

#include "../open/fileserv.h"


extern "C" {


#ifdef V4_THREADING
#undef V4_MUTEX_LOCK
#define V4_MUTEX_LOCK(mutx)	THD_MUTEX_LOCK (mutx)
#endif

#define MAXPATHLEN	1024
#define IO_RETRY	20

#ifdef O_SYNC
#define SYNC		O_SYNC
#endif

#ifdef O_FSYNC
#define SYNC		O_FSYNC
#endif

#ifndef SYNC
#define SYNC		0
#endif

#ifndef O_BINARY
#define O_BINARY	0
#endif

#ifndef EINTR
#define EINTR		0
#endif

#define MASK		(S_IWRITE | S_IREAD)

static void close_marker_file(TEXT *);
static FIL seek_file(FIL, BDB, STATUS *);
static FIL setup_file(DBB, TEXT *, USHORT, int);
static void windows_error(TEXT *, FIL, STATUS, STATUS *);



int PIO_add_file(DBB dbb, FIL main_file, TEXT * file_name, SLONG start)
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


void PIO_close(FIL main_file)
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

	for (file = main_file; file; file = file->fil_next) {
		if (file->fil_desc == HFILE_ERROR)
			continue;			/* This really should be an error */
		if (file->fil_desc) {
			FILE_close(file->fil_desc);
			file->fil_desc = HFILE_ERROR;
			V4_MUTEX_DESTROY(file->fil_mutex);
		}
	}
}


int PIO_connection(TEXT * file_name, USHORT * file_length)
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


FIL PIO_create(DBB dbb, TEXT * string, SSHORT length, BOOLEAN overwrite)
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
	HFILE desc;
	FIL file;
	TEXT expanded_name[256], temp[256], *file_name;
	OFSTRUCT OfStruct;

	file_name = string;

	if (length) {
		MOVE_FAST(file_name, temp, length);
		temp[length] = 0;
		file_name = temp;
	}

	memset(&OfStruct, 0, sizeof OfStruct);
	desc = FILE_OpenFile(file_name, &OfStruct, OF_READWRITE | OF_CREATE);
	if (desc == HFILE_ERROR)
		ERR_post(isc_io_error,
				 gds_arg_string,
				 "OpenFile OF_CREATE",
				 gds_arg_cstring,
				 length,
				 ERR_string(string, length),
				 isc_arg_gds,
				 isc_io_create_err, gds_arg_dos, FILE_GetLastError(), 0);
	FILE_close(desc);
	desc = FILE_OpenFile(NULL, &OfStruct, OF_REOPEN | OF_READWRITE |
						 OF_SHARE_EXCLUSIVE);
	if (desc == HFILE_ERROR)
		ERR_post(isc_io_error,
				 gds_arg_string,
				 "OpenFile OF_READWRITE",
				 gds_arg_cstring,
				 length,
				 ERR_string(string, length),
				 isc_arg_gds,
				 isc_io_open_err, gds_arg_dos, FILE_GetLastError(), 0);

/* File open succeeded.  Now expand the file name. */

	OemToAnsi(OfStruct.szPathName, expanded_name);
	file = setup_file(dbb, expanded_name, strlen(expanded_name), desc);
	return file;
}


int PIO_expand(TEXT * file_name, USHORT file_length, TEXT * expanded_name)
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


void PIO_flush(FIL main_file)
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
	FIL file;

	for (file = main_file; file; file = file->fil_next) {
		FILE_flush(file->fil_desc);
	}
}


void PIO_force_write(FIL file, USHORT flag)
{
/**************************************
 *
 *	P I O _ f o r c e _ w r i t e	( G E N E R I C )
 *
 **************************************
 *
 * Functional description
 *	Set (or clear) force write, if possible, for the database.
 *
 **************************************/
}


void PIO_header(DBB dbb, SCHAR * address, int length)
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

	if (file->fil_desc == HFILE_ERROR)
		windows_error("PIO_header (invalid file descriptor)", file,
					  isc_io_read_err, 0);

	ISC_inhibit();
	V4_MUTEX_LOCK(file->fil_mutex);

	if ((FILE_lseek(file->fil_desc, 0, 0)) == HFILE_ERROR) {
		V4_MUTEX_UNLOCK(file->fil_mutex);
		windows_error("lseek", file, isc_io_read_err, 0);
	}

#ifdef ISC_DATABASE_ENCRYPTION
	if (dbb->dbb_encrypt_key) {
		SLONG spare_buffer[MAX_PAGE_SIZE / sizeof(SLONG)];

		if ((FILE_read(file->fil_desc, spare_buffer, length)) != length) {
			V4_MUTEX_UNLOCK(file->fil_mutex);
			windows_error("read", file, isc_io_read_err, 0);
		}

		(*dbb->dbb_decrypt) (dbb->dbb_encrypt_key->str_data,
							 spare_buffer, length, address);
	}
	else
#endif
	if ((FILE_read(file->fil_desc, address, length)) != length) {
		V4_MUTEX_UNLOCK(file->fil_mutex);
		windows_error("read", file, isc_io_read_err, 0);
	}

	V4_MUTEX_UNLOCK(file->fil_mutex);
	ISC_enable();
}


SLONG PIO_max_alloc(DBB dbb)
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
	LONG length, Offset;
	FIL file;

	for (file = dbb->dbb_file; file->fil_next; file = file->fil_next);

	if (file->fil_desc == HFILE_ERROR) {
		windows_error("PIO_max_alloc (invalid file descriptor)", file,
					  isc_io_access_err, 0);
		return FALSE;
	}

/* save the current position */
	Offset = FILE_lseek(file->fil_desc, 0, 1);

/* get the file length (end of file) */
	length = FILE_lseek(file->fil_desc, 0, 2);

/* restore saved position (from beginning of file) */
	FILE_lseek(file->fil_desc, Offset, 0);

	if (length == HFILE_ERROR) {
		ISC_inhibit();
		windows_error("lseek (end of file)", file, isc_io_access_err, 0);
	}

	return (file->fil_min_page - file->fil_fudge +
			(length + dbb->dbb_page_size - 1) / dbb->dbb_page_size);
}


FIL PIO_open(DBB dbb,
			 TEXT * string,
			 SSHORT length,
			 SSHORT trace_flag,
			 BLK connection, TEXT * file_name, USHORT file_length)
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
	HFILE desc;
	OFSTRUCT OfStruct;
	int i;

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

	for (i = 0; i < IO_RETRY; i++) {
		memset(&OfStruct, 0, sizeof OfStruct);
		if ((desc = FILE_OpenFile(ptr, &OfStruct, OF_READWRITE |
								  OF_SHARE_EXCLUSIVE)) != HFILE_ERROR)
			break;
	}

	if (desc == HFILE_ERROR)
		ERR_post(isc_io_error,
				 gds_arg_string,
				 "OpenFile OF_READWRITE",
				 gds_arg_cstring,
				 file_length,
				 ERR_string(file_name, file_length),
				 isc_arg_gds,
				 isc_io_open_err, gds_arg_dos, FILE_GetLastError(), 0);

/* use the expanded filename */
	OemToAnsi(OfStruct.szPathName, temp);

	return setup_file(dbb, temp, strlen(temp), desc);
}


int PIO_read(FIL file, BDB bdb, PAG page, STATUS * status_vector)
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

	if (file->fil_desc == HFILE_ERROR) {
		windows_error("PIO_read (invalid file descriptor)", file,
					  isc_io_read_err, status_vector);
		return FALSE;
	}

	ISC_inhibit();
	dbb = bdb->bdb_dbb;
	size = dbb->dbb_page_size;

#ifdef ISC_DATABASE_ENCRYPTION
	if (dbb->dbb_encrypt_key) {
		SLONG spare_buffer[MAX_PAGE_SIZE / sizeof(SLONG)];

		for (i = 0; i < IO_RETRY; i++) {
			if (!(file = seek_file(file, bdb, status_vector)))
				return FALSE;
			if ((bytes = FILE_read(file->fil_desc, spare_buffer, size)) ==
				size) {
				(*dbb->dbb_decrypt) (dbb->dbb_encrypt_key->str_data,
									 spare_buffer, size, page);
				break;
			}
			V4_MUTEX_UNLOCK(file->fil_mutex);
			if (bytes == HFILE_ERROR) {
				windows_error("read", file, isc_io_read_err, status_vector);
				return FALSE;
			}
		}
	}
	else
#endif
	{
		for (i = 0; i < IO_RETRY; i++) {
			if (!(file = seek_file(file, bdb, status_vector)))
				return FALSE;
			if ((bytes = FILE_read(file->fil_desc, page, size)) == size)
				break;
			V4_MUTEX_UNLOCK(file->fil_mutex);
			if (bytes == HFILE_ERROR) {
				windows_error("read", file, isc_io_read_err, status_vector);
				return FALSE;
			}
		}
	}

	V4_MUTEX_UNLOCK(file->fil_mutex);

	if (i == IO_RETRY) {
		windows_error("read", file, isc_io_read_err, status_vector);
		return FALSE;
	}

	ISC_enable();
	return TRUE;
}


int PIO_write(FIL file, BDB bdb, PAG page, STATUS * status_vector)
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

	if (file->fil_desc == HFILE_ERROR) {
		windows_error("PIO_write (invalid file descriptor)", file,
					  isc_io_write_err, status_vector);
		return FALSE;
	}

	ISC_inhibit();
	dbb = bdb->bdb_dbb;
	size = dbb->dbb_page_size;

#ifdef ISC_DATABASE_ENCRYPTION
	if (dbb->dbb_encrypt_key) {
		SLONG spare_buffer[MAX_PAGE_SIZE / sizeof(SLONG)];

		(*dbb->dbb_encrypt) (dbb->dbb_encrypt_key->str_data,
							 page, size, spare_buffer);

		for (i = 0; i < IO_RETRY; i++) {
			if (!(file = seek_file(file, bdb, status_vector)))
				return FALSE;
			if ((bytes = FILE_write(file->fil_desc, spare_buffer, size)) ==
				size) break;
			V4_MUTEX_UNLOCK(file->fil_mutex);
			if (bytes == HFILE_ERROR) {
				windows_error("write", file, isc_io_write_err, status_vector);
				return FALSE;
			}
		}
	}
	else
#endif
	{
		for (i = 0; i < IO_RETRY; i++) {
			if (!(file = seek_file(file, bdb, status_vector)))
				return FALSE;
			if ((bytes = FILE_write(file->fil_desc, page, size)) == size)
				break;
			V4_MUTEX_UNLOCK(file->fil_mutex);
			if (bytes == HFILE_ERROR) {
				windows_error("write", file, isc_io_write_err, status_vector);
				return FALSE;
			}
		}
	}

	V4_MUTEX_UNLOCK(file->fil_mutex);

	if (i == IO_RETRY) {
		windows_error("write", file, isc_io_write_err, status_vector);
		return FALSE;
	}

	ISC_enable();
	return TRUE;
}


static FIL seek_file(FIL file, BDB bdb, STATUS * status_vector)
{
/**************************************
 *
 *	s e e k _ f i l e
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

	if (file->fil_desc == HFILE_ERROR) {
		windows_error("seek_file (invalid file descriptor)", file,
					  isc_io_access_err, status_vector);
		return (FIL) FALSE;
	}

	page -= file->fil_min_page - file->fil_fudge;

	V4_MUTEX_LOCK(file->fil_mutex);

	while ((FILE_lseek(file->fil_desc, (LONG) (page * dbb->dbb_page_size), 0))
		   == HFILE_ERROR) {
		V4_MUTEX_UNLOCK(file->fil_mutex);
		windows_error("lseek", file, isc_io_access_err, status_vector);
		return (FIL) FALSE;
	}

	return file;
}


static FIL setup_file(
					  DBB dbb,
					  TEXT * file_name, USHORT file_length, HFILE desc)
{
/**************************************
 *
 *	s e t u p _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Set up file and lock blocks for a file.
 *
 **************************************/
	FIL file;
	LCK lock;
	UCHAR *lock_string;
	USHORT l;

/* Allocate file block and copy file name string */

	file = new(dbb->dbb_permanent, file_length + 1) fil();
	file->fil_desc = desc;
	file->fil_length = file_length;
	file->fil_max_page = -1;
	MOVE_FAST(file_name, file->fil_string, file_length);
	file->fil_string[file_length] = '\0';
	V4_MUTEX_INIT(file->fil_mutex);

/* If this isn't the primary file, we're done */

	if (dbb->dbb_file)
		return file;

/* 
* Build unique lock string for file and construct lock block 
* on the PC/WINDOWS platfrom the only unique identifier is the
* expanded filename (cross references are bound to screwup).
*/

	l = file_length;
	lock_string = file_name;

	dbb->dbb_lock = lock = new(dbb->dbb_permanent, l) lck();
	lock->lck_type = LCK_database;
	lock->lck_owner_handle = LCK_get_owner_handle(NULL_TDBB, lock->lck_type);
	lock->lck_object = (BLK) dbb;
	lock->lck_length = l;
	lock->lck_dbb = dbb;
	lock->lck_ast = CCH_down_grade_dbb;
	MOVE_FAST(lock_string, lock->lck_key.lck_string, l);

/* 
* Try to get an exclusive lock on database.  If this fails, insist
* on at least a shared lock 
*/

	dbb->dbb_flags |= DBB_exclusive;
	if (!LCK_lock(NULL_TDBB, lock, LCK_EX, LCK_NO_WAIT)) {
		dbb->dbb_flags &= ~DBB_exclusive;
		LCK_lock(NULL_TDBB, lock,
				 (dbb->dbb_flags & DBB_cache_manager) ? LCK_SR : LCK_SW,
				 LCK_WAIT);
	}

	return file;
}


static void windows_error(
						  TEXT * string,
						  FIL file, STATUS operation, STATUS * status_vector)
{
/**************************************
 *
 *	w i n d o w s _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Somebody has noticed a file system error and expects error
 *	to do something about it.  Harumph!
 *
 **************************************/
	STATUS *status;
	LONG errno;

/* Adjust error code for messages contained in the message file. */
	errno = FILE_GetLastError();
	if (MAP_LOC_ERROR(errno) != -1)
		errno += LOC_ERROR_BEG;

	ISC_enable();

	if (status = status_vector) {
		*status++ = isc_arg_gds;
		*status++ = isc_io_error;
		*status++ = gds_arg_string;
		*status++ = (STATUS) string;
		*status++ = gds_arg_string;
		*status++ = (STATUS) ERR_string(file->fil_string, file->fil_length);
		*status++ = isc_arg_gds;
		*status++ = operation;
		*status++ = gds_arg_dos;
		*status++ = errno;
		*status++ = gds_arg_end;
		gds__log_status(0, status_vector);
	}
	else
		ERR_post(isc_io_error,
				 gds_arg_string,
				 string,
				 gds_arg_string,
				 ERR_string(file->fil_string, file->fil_length),
				 isc_arg_gds, operation, gds_arg_dos, errno, 0);
}


} // extern "C"
