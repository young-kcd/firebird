#error This is an obsolete port.
/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		os2.c
 *	DESCRIPTION:	OS2 specific physical IO
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
 * 2002.02.15 Sean Leyne - obsolete "OS/2" port
 *
 */

#define
#include "firebird.h"INCL_DOSFILEMGR
#include <os2.h>
#include <string.h>
#include "../jrd/jrd.h"
#include "../jrd/pio.h"
#include "../jrd/cch.h"
#include "../jrd/ods.h"
#include "../jrd/lck.h"
#include "gen/codes.h"
#include "../jrd/all_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/isc_f_proto.h"

#include "../jrd/lck_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/pio_proto.h"
#include "../jrd/thd_proto.h"


extern "C" {


#define MAXPATHLEN		1024
#define DEFAULTBUFFERSIZE	1024

static BOOLEAN check_network(TEXT *);
static FIL seek_file(FIL, BDB, STATUS *);
static FIL setup_file(DBB, TEXT *, USHORT, ULONG);
static BOOLEAN dos_error(TEXT *, FIL, STATUS, ULONG, STATUS *);


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
	USHORT i;

	for (file = main_file; file; file = file->fil_next)
		if (file->fil_desc) {
			DosClose((HFILE) file->fil_desc);
			V4_MUTEX_DESTROY(file->fil_mutex);
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
	ULONG desc, status, action, open_mode;
	FIL file;
	TEXT expanded_name[MAXPATHLEN], temp_name[MAXPATHLEN], *file_name;

	length = PIO_expand(string, length, expanded_name);

	if (check_network(expanded_name))
		ERR_post(gds__unavailable, 0);

	open_mode = OPEN_ACCESS_READWRITE |	/* Access mode -- read/write */
		OPEN_SHARE_DENYNONE |	/* Share mode -- permit read/write */
		OPEN_FLAGS_NOINHERIT |	/* Inheritance mode -- don't */
		OPEN_FLAGS_FAIL_ON_ERROR |	/* Fail errors -- return code */
		OPEN_FLAGS_WRITE_THROUGH |	/* Write thru -- absolutely */
		(0 << 15);				/* DASD open -- no */

/* take care of non null-terminated case */

	if (length) {
		strncpy(temp_name, string, length);
		temp_name[length] = '\0';
		string = temp_name;
	}

	status = DosOpen(string,	/* File pathname */
					 &desc,		/* File handle */
					 &action,	/* Action taken */
					 100,		/* Initial size */
					 FILE_NORMAL,	/* File attributes */
					 OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_REPLACE_IF_EXISTS,	/* Open Flag */
					 open_mode,	/* Open mode (see above) */
					 0);		/* Reserved */

	if (status)
		ERR_post(isc_io_error,
				 gds_arg_string, "DosOpen (create)",
				 gds_arg_cstring, length, ERR_string(string, length),
				 isc_arg_gds, isc_io_create_err, gds_arg_dos, status, 0);

/* File open succeeded.  Now expand the file name. */

	file = setup_file(dbb, expanded_name, length, desc);

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


void PIO_flush(FIL file)
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


void PIO_force_write(FIL file, USHORT flag)
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
	ULONG status, actual_length;
	ULONG new_offset;
	FIL file;

	file = dbb->dbb_file;

	V4_MUTEX_LOCK(file->fil_mutex);

	if (status = DosSetFilePtr(file->fil_desc, 0, FILE_BEGIN, &new_offset)) {
		V4_MUTEX_UNLOCK(file->fil_mutex);
		dos_error("DosSetFilePtr", file, isc_io_read_err, status, 0);
	}

#ifdef ISC_DATABASE_ENCRYPTION
	if (dbb->dbb_encrypt_key) {
		SLONG spare_buffer[MAX_PAGE_SIZE / sizeof(SLONG)];

		if (
			(status =
			 DosRead(file->fil_desc, spare_buffer, length, &actual_length))
			|| actual_length != length) {
			V4_MUTEX_UNLOCK(file->fil_mutex);
			dos_error("DosRead (read header)", file, isc_io_read_err, status,
					  0);
		}

		(*dbb->dbb_decrypt) (dbb->dbb_encrypt_key->str_data,
							 spare_buffer, length, address);
	}
	else
#endif
	{
		if (
			(status =
			 DosRead(file->fil_desc, address, length, &actual_length))
			|| actual_length != length) {
			V4_MUTEX_UNLOCK(file->fil_mutex);
			dos_error("DosRead (read header)", file, isc_io_read_err, status,
					  0);
		}
	}

	V4_MUTEX_UNLOCK(file->fil_mutex);
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
	ULONG status;
	FILESTATUS3 buffer;
	ULONG length;
	FIL file;

	for (file = dbb->dbb_file; file->fil_next; file = file->fil_next);

	if (status =
		DosQueryFileInfo(file->fil_desc, 1, (PVOID) & buffer,
						 sizeof(buffer))) dos_error("DosQueryFileInfo", file,
													isc_io_access_err, status,
													0);

	length = buffer.cbFile;

	return file->fil_min_page - file->fil_fudge +
		(length + dbb->dbb_page_size - 1) / dbb->dbb_page_size;
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
	PAG page;
	ULONG desc, action, status;
	ULONG open_mode;
	TEXT temp_name[MAXPATHLEN];

	if (check_network(string))
		ERR_post(gds__unavailable, 0);

	open_mode = OPEN_ACCESS_READWRITE |	/* Access mode -- read/write */
		OPEN_SHARE_DENYNONE |	/* Share mode -- permit read/write */
		OPEN_FLAGS_NOINHERIT |	/* Inheritance mode -- don't */
		OPEN_FLAGS_FAIL_ON_ERROR |	/* Fail errors -- return code */
		OPEN_FLAGS_WRITE_THROUGH |	/* Write thru -- absolutely */
		(0 << 15);				/* DASD open -- no */

/* take care of non null-terminated case */

	if (length) {
		strncpy(temp_name, string, length);
		temp_name[length] = '\0';
		string = temp_name;
	}

	status = DosOpen(string,	/* File pathname */
					 &desc,		/* File handle */
					 &action,	/* Action taken */
					 100,		/* Initial size */
					 FILE_NORMAL,	/* File attributes */
					 OPEN_ACTION_OPEN_IF_EXISTS,	/* Open Flag (truncate if file exists) */
					 open_mode,	/* Open mode (see above) */
					 0L);		/* Reserved */

	if (status)
		ERR_post(isc_io_error,
				 gds_arg_string, "DosOpen (open)",
				 gds_arg_cstring, file_length, ERR_string(file_name,
														  file_length),
				 isc_arg_gds, isc_io_open_err, gds_arg_dos, status, 0);

	return setup_file(dbb, string, length, desc);
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
	ULONG status, actual_length;
	DBB dbb;

	dbb = bdb->bdb_dbb;
	if (!(file = seek_file(file, bdb, status_vector)))
		return FALSE;

#ifdef ISC_DATABASE_ENCRYPTION
	if (dbb->dbb_encrypt_key) {
		SLONG spare_buffer[MAX_PAGE_SIZE / sizeof(SLONG)];

		if (status =
			DosRead(file->fil_desc, spare_buffer, dbb->dbb_page_size,
					&actual_length)) {
			V4_MUTEX_UNLOCK(file->fil_mutex);
			return dos_error("DosRead", file, isc_io_read_err, status,
							 status_vector);
		}

		(*dbb->dbb_decrypt) (dbb->dbb_encrypt_key->str_data,
							 spare_buffer, dbb->dbb_page_size, page);
	}
	else
#endif
	{
		if (status =
			DosRead(file->fil_desc, page, dbb->dbb_page_size,
					&actual_length)) {
			V4_MUTEX_UNLOCK(file->fil_mutex);
			return dos_error("DosRead", file, isc_io_read_err, status,
							 status_vector);
		}
	}

	V4_MUTEX_UNLOCK(file->fil_mutex);

	if (actual_length != dbb->dbb_page_size)
		return dos_error("DosRead truncated", file, isc_io_read_err, status,
						 status_vector);

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
 *	Read a data page.  Oh wow.
 *
 **************************************/
	ULONG status, actual_length;
	DBB dbb;

	dbb = bdb->bdb_dbb;
	if (!(file = seek_file(file, bdb, status_vector)))
		return FALSE;

#ifdef ISC_DATABASE_ENCRYPTION
	if (dbb->dbb_encrypt_key) {
		SLONG spare_buffer[MAX_PAGE_SIZE / sizeof(SLONG)];

		(*dbb->dbb_encrypt) (dbb->dbb_encrypt_key->str_data,
							 page, dbb->dbb_page_size, spare_buffer);
		if (status =
			DosWrite(file->fil_desc, spare_buffer, dbb->dbb_page_size,
					 &actual_length)) {
			V4_MUTEX_UNLOCK(file->fil_mutex);
			return dos_error("DosWrite", file, isc_io_write_err, status,
							 status_vector);
		}
	}
	else
#endif
	{
		if (status =
			DosWrite(file->fil_desc, page, dbb->dbb_page_size,
					 &actual_length)) {
			V4_MUTEX_UNLOCK(file->fil_mutex);
			return dos_error("DosWrite", file, isc_io_write_err, status,
							 status_vector);
		}
	}

	V4_MUTEX_UNLOCK(file->fil_mutex);

	if (actual_length != dbb->dbb_page_size)
		return dos_error("DosWrite truncated", file, isc_io_write_err, status,
						 status_vector);

	return TRUE;
}


static BOOLEAN check_network(TEXT * file_name)
{
/**************************************
 *
 *	c h e c k _ n e t w o r k
 *
 **************************************
 *
 * Functional description
 *    Try to determine whether path name given refers to a remote resource.
 *    This is necessary under OS/2 since remote path names (prefixed by two
 *    slashes, or higher than physical drive letters) may be used as ordinary
 *    file names.  If the remote server has failed for any reason to make
 *    the connection, it should not be allowed here either.
 *    This routine checks for both the slash slash network prefix,
 *    and the drive letter redirection to network resources.
 *    Returns FALSE if local, TRUE if network.
 *
 **************************************/

	if (file_name[0] == '\\' && file_name[1] == '\\')
		return TRUE;

	return FALSE;
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
	ULONG status;
	ULONG page, offset, new_offset;
	DBB dbb;

	dbb = bdb->bdb_dbb;
	page = bdb->bdb_page;

	for (;; file = file->fil_next)
		if (!file)
			CORRUPT(158);		/* msg 158 cannot sort on a field that does not exist */
		else if (page >= file->fil_min_page && page <= file->fil_max_page)
			break;

	page -= file->fil_min_page - file->fil_fudge;
	offset = page * dbb->dbb_page_size;

	V4_MUTEX_LOCK(file->fil_mutex);

	if (status =
		DosSetFilePtr(file->fil_desc, offset, FILE_BEGIN, &new_offset)) {
		V4_MUTEX_UNLOCK(file->fil_mutex);
		return (FIL) dos_error("DosSetFilePtr", file, isc_io_access_err,
							   status, status_vector);
	}

	return file;
}


static FIL setup_file(
					  DBB dbb,
					  TEXT * file_name, USHORT file_length, ULONG desc)
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
	TEXT *p, *q;

/* Allocate file block and copy file name string */

	file = new(*dbb->dbb_permanent, file_length + 1) fil();
	file->fil_desc = desc;
	file->fil_length = file_length;
	file->fil_max_page = -1;
	MOVE_FAST(file_name, file->fil_string, file_length);
	file->fil_string[file_length] = 0;
	V4_MUTEX_INIT(file->fil_mutex);

/* If this isn't the primary file, we're done */

	if (dbb->dbb_file)
		return file;

/* Build unique lock string for file and construct lock block */

	dbb->dbb_lock = lock = new(*dbb->dbb_permanent, file_length) lck();
	lock->lck_type = LCK_database;
	lock->lck_owner_handle = LCK_get_owner_handle(NULL_TDBB, lock->lck_type);
	lock->lck_object = reinterpret_cast<blk*>(dbb);
	lock->lck_length = file_length;
	lock->lck_ast = CCH_down_grade_dbb;
	lock->lck_dbb = dbb;
	MOVE_FAST(file_name, lock->lck_key.lck_string, file_length);

/* upcase the string to ensure equality on OS/2 */

	for (p = lock->lck_key.lck_string; file_length--; p++)
		*p = UPPER(*p);

/* Try to get an exclusive lock on database.  If this fails, insist
   on at least a shared lock */

	dbb->dbb_flags |= DBB_exclusive;
	if (!LCK_lock(NULL_TDBB, lock, LCK_EX, LCK_NO_WAIT)) {
		dbb->dbb_flags &= ~DBB_exclusive;
		LCK_lock(NULL_TDBB, lock,
				 (dbb->dbb_flags & DBB_cache_manager) ? LCK_SR : LCK_SW,
				 LCK_WAIT);
	}

	return file;
}


static BOOLEAN dos_error(
						 TEXT * string,
						 FIL file,
						 STATUS operation,
						 ULONG status, STATUS * status_vector)
{
/**************************************
 *
 *	d o s _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Somebody has noticed a file system error and expects error
 *	to do something about it.  Harumph!
 *
 **************************************/

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
		*status_vector++ = gds_arg_dos;
		*status_vector++ = status;
		*status_vector++ = gds_arg_end;
		return FALSE;
	}
	else
		ERR_post(isc_io_error,
				 gds_arg_string, string,
				 gds_arg_string, ERR_string(file->fil_string,
											file->fil_length), isc_arg_gds,
				 operation, gds_arg_dos, status, 0);
}


} // extern "C"
