/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		llio.cpp
 *	DESCRIPTION:	Low-level I/O routines
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
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "EPSON" define
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "DELTA" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "IMP" port
 *
 * 2002.10.27 Sean Leyne - Completed removal of "DELTA" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <errno.h>
#include <string.h>
#include "../jrd/common.h"
#include "../jrd/llio.h"
#include "gen/iberror.h"
#include "../jrd/iberr_proto.h"
#include "../jrd/llio_proto.h"

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#  ifndef WIN_NT
#    ifndef VMS
#      include <fcntl.h>
#      include <sys/file.h>
#    else
#      include <file.h>
#    endif
#  endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef WIN_NT
#include <windows.h>
#ifdef TEXT
#undef TEXT
#endif
#define TEXT		SCHAR
#endif

#ifndef O_SYNC
#define O_SYNC		0
#endif

#ifndef EINTR
#define EINTR		0
#endif

#define IO_RETRY	20
#define BUFSIZE		32768

static void io_error(ISC_STATUS*, const TEXT*, const TEXT*, ISC_STATUS);


int LLIO_allocate_file_space(
							 ISC_STATUS * status_vector,
							 TEXT * filename,
							 SLONG size, UCHAR fill_char, USHORT overwrite)
{
/**************************************
 *
 *        L L I O _ a l l o c a t e _ f i l e _ s p a c e
 *
 **************************************
 *
 * Functional description
 *        Open (create if necessary) the given file and write 'size'
 *        number of bytes to it.  Each byte is initialized to
 *        the passed fill_char.  If 'overwrite' is FALSE and the file
 *        already exists, return FB_FAILURE.
 *
 *        This routine may be used to make sure that the file already
 *        has enough space allocated to it.
 *
 *        Other side benefit of using this routine could be that once the
 *        space is allocated, file system would not update the inode every
 *        time something is written to the file unless the file size grows
 *        beyond the given 'size'.  So 'write' performance should improve.
 *
 *        If there is any error, return FB_FAILURE else return FB_SUCCESS.
 *        In case of an error, status_vector would be updated.
 *
 **************************************/
	SLONG fd;
	SCHAR buffer[BUFSIZE];
	SLONG times;
	SLONG last_size, length;
	USHORT open_mode;

	if (overwrite)
		open_mode = LLIO_OPEN_RW;
	else
		open_mode = LLIO_OPEN_NEW_RW;

	if (LLIO_open(status_vector, filename, open_mode, TRUE, &fd))
		return FB_FAILURE;

	memset(buffer, (int) fill_char, BUFSIZE);
	times = size / BUFSIZE + 1;	/* For the while loop below */
	last_size = size % BUFSIZE;
	while (times--) {
		length = (times != 0) ? BUFSIZE : last_size;
		if (LLIO_write(status_vector, fd, filename, 0L, LLIO_SEEK_NONE,
					   reinterpret_cast < UCHAR * >(buffer), length, 0)) {
			LLIO_close(0, fd);
			return FB_FAILURE;
		}
	}

	LLIO_close(0, fd);

	return FB_SUCCESS;
}


#ifdef WIN_NT
#define IO_DEFINED
int LLIO_close(ISC_STATUS * status_vector, SLONG file_desc)
{
/**************************************
 *
 *	L L I O _ c l o s e		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Close a file.
 *
 **************************************/

	return (CloseHandle((HANDLE) file_desc) ? FB_SUCCESS : FB_FAILURE);
}


int LLIO_open(
			  ISC_STATUS* status_vector,
			  const TEXT* filename,
			  USHORT open_mode, USHORT share_flag, SLONG* file_desc)
{
/**************************************
 *
 *	L L I O _ o p e n		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Open a file.
 *
 **************************************/
	DWORD access, create, attributes;

	access = GENERIC_READ | GENERIC_WRITE;
	attributes = 0;
	switch (open_mode) {
	case LLIO_OPEN_R:
		access = GENERIC_READ;
		create = OPEN_EXISTING;
		break;
	case LLIO_OPEN_RW:
		create = OPEN_ALWAYS;
		break;
	case LLIO_OPEN_WITH_SYNC_RW:
		create = OPEN_ALWAYS;
		attributes = FILE_FLAG_WRITE_THROUGH;
		break;
	case LLIO_OPEN_WITH_TRUNC_RW:
		create = TRUNCATE_EXISTING;
		break;
	case LLIO_OPEN_EXISTING_RW:
		create = OPEN_EXISTING;
		break;
	case LLIO_OPEN_WITH_SYNC_W:
		access = GENERIC_WRITE;
		create = OPEN_ALWAYS;
		attributes = FILE_FLAG_WRITE_THROUGH;
		break;
	case LLIO_OPEN_NEW_RW:
		create = CREATE_NEW;
		break;
	}

	*file_desc = reinterpret_cast < SLONG > (CreateFile(filename,
														access,
														FILE_SHARE_READ |
														FILE_SHARE_WRITE,
														NULL, create,
														FILE_ATTRIBUTE_NORMAL
														|
														FILE_FLAG_RANDOM_ACCESS
														| attributes, 0));
	if ((HANDLE) *file_desc == INVALID_HANDLE_VALUE
		&& open_mode == LLIO_OPEN_WITH_TRUNC_RW)
	{
		*file_desc =
			reinterpret_cast < SLONG >
			(CreateFile
			 (filename, access, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			  CREATE_NEW,
			  FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS | attributes,
			  0));
	}
	if ((HANDLE) *file_desc == INVALID_HANDLE_VALUE) {
		if (status_vector)
			io_error(status_vector, "CreateFile", filename, isc_io_open_err);
		return FB_FAILURE;
	}

	return FB_SUCCESS;
}


int LLIO_read(
			  ISC_STATUS * status_vector,
			  SLONG file_desc,
			  TEXT * filename,
			  SLONG offset,
			  USHORT whence,
UCHAR * buffer, SLONG length, SLONG * length_read)
{
/**************************************
 *
 *	L L I O _ r e a d		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Read a record from a file.
 *
 **************************************/
	DWORD len;

	if ((whence != LLIO_SEEK_NONE) &&
		(LLIO_seek(status_vector, file_desc, filename, offset, whence) ==
		 FB_FAILURE)) return FB_FAILURE;

	if (buffer)
		if (!ReadFile((HANDLE) file_desc, buffer, length, &len, NULL)) {
			if (status_vector)
				io_error(status_vector, "ReadFile", filename,
						 isc_io_read_err);
			return FB_FAILURE;
		}

	if (length_read)
		*length_read = len;

	return FB_SUCCESS;
}


int LLIO_seek(
			  ISC_STATUS * status_vector,
			  SLONG file_desc, TEXT * filename, SLONG offset, USHORT whence)
{
/**************************************
 *
 *	L L I O _ s e e k		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Seek to the given offset.
 *
 **************************************/
	DWORD method;

	if (whence != LLIO_SEEK_NONE) {
		switch (whence) {
		case LLIO_SEEK_BEGIN:
			method = FILE_BEGIN;
			break;
		case LLIO_SEEK_CURRENT:
			method = FILE_CURRENT;
			break;
		case LLIO_SEEK_END:
			method = FILE_END;
			break;
		}
		if (SetFilePointer((HANDLE) file_desc, offset, NULL, method) == (DWORD) -1) {
			if (status_vector)
				io_error(status_vector, "SetFilePointer", filename,
						 isc_io_access_err);
			return FB_FAILURE;
		}
	}

	return FB_SUCCESS;
}


int LLIO_sync(ISC_STATUS * status_vector, SLONG file_desc)
{
/**************************************
 *
 *	L L I O _ s y n c		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Flush the buffers in a file.
 *
 **************************************/

	return (FlushFileBuffers((HANDLE) file_desc) ? FB_SUCCESS : FB_FAILURE);
}


int LLIO_write(
			   ISC_STATUS * status_vector,
			   SLONG file_desc,
			   TEXT * filename,
			   SLONG offset,
USHORT whence, UCHAR * buffer, SLONG length, SLONG * length_written)
{
/**************************************
 *
 *	L L I O _ w r i t e		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Write a record to a file.
 *
 **************************************/
	DWORD len;

	if ((whence != LLIO_SEEK_NONE) &&
		(LLIO_seek(status_vector, file_desc, filename, offset, whence) ==
		 FB_FAILURE)) return FB_FAILURE;

	if (buffer)
		if (!WriteFile((HANDLE) file_desc, buffer, length, &len, NULL)) {
			if (status_vector)
				io_error(status_vector, "WriteFile", filename,
						 isc_io_write_err);
			return FB_FAILURE;
		}

	if (length_written)
		*length_written = len;

	return FB_SUCCESS;
}


static void io_error(ISC_STATUS* status_vector,
					 const TEXT* op, const TEXT* filename, ISC_STATUS operation)
{
/**************************************
 *
 *	i o _ e r r o r		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Fill up the status_vector with error info.
 *
 **************************************/

	IBERR_build_status(status_vector, isc_io_error, isc_arg_string, op,
					   isc_arg_string, filename, isc_arg_gds, operation,
					   isc_arg_win32, GetLastError(), 0);
}
#endif


#ifndef IO_DEFINED
int LLIO_close(ISC_STATUS * status_vector, SLONG file_desc)
{
/**************************************
 *
 *	L L I O _ c l o s e		( g e n e r i c )
 *
 **************************************
 *
 * Functional description
 *	Close a file.
 *
 **************************************/

	return ((close((int) file_desc) != -1) ? FB_SUCCESS : FB_FAILURE);
}


int LLIO_open(
			  ISC_STATUS* status_vector,
			  const TEXT* filename,
			  USHORT open_mode, USHORT share_flag, SLONG * file_desc)
{
/**************************************
 *
 *	L L I O _ o p e n		( g e n e r i c )
 *
 **************************************
 *
 * Functional description
 *	Open a file.
 *
 **************************************/
	int oldmask;
	int mode;

	switch (open_mode) {
	case LLIO_OPEN_R:
		mode = O_RDONLY;
		break;
	case LLIO_OPEN_RW:
		mode = O_RDWR | O_CREAT;
		break;
	case LLIO_OPEN_WITH_SYNC_RW:
		mode = O_RDWR | O_CREAT | O_SYNC;
		break;
	case LLIO_OPEN_WITH_TRUNC_RW:
		mode = O_RDWR | O_CREAT | O_TRUNC;
		break;
	case LLIO_OPEN_EXISTING_RW:
		mode = O_RDWR;
		break;
	case LLIO_OPEN_WITH_SYNC_W:
		mode = O_WRONLY | O_CREAT | O_SYNC;
		break;
	case LLIO_OPEN_NEW_RW:
		mode = O_CREAT | O_EXCL | O_RDWR;
		break;
	}
	if (share_flag)
		oldmask = umask(0);
	*file_desc = open(filename, mode, 0666);
	if (share_flag)
		umask(oldmask);
	if (*file_desc == -1) {
		if (status_vector)
			io_error(status_vector, "open", filename, isc_io_open_err);
		return FB_FAILURE;
	}

	return FB_SUCCESS;
}

int LLIO_read(
			  ISC_STATUS * status_vector,
			  SLONG file_desc,
			  TEXT * filename,
			  SLONG offset,
			  USHORT whence,
UCHAR * buffer, SLONG length, SLONG * length_read)
{
/**************************************
 *
 *	L L I O _ r e a d		( g e n e r i c )
 *
 **************************************
 *
 * Functional description
 *	Read a record from a file.
 *
 **************************************/
	int len, i;
	UCHAR *p;

	if ((whence != LLIO_SEEK_NONE) &&
		(LLIO_seek(status_vector, file_desc, filename, offset, whence) ==
		 FB_FAILURE)) return FB_FAILURE;

	if (p = buffer)
		for (i = 0; length && i++ < IO_RETRY;) {
			if ((len = read((int) file_desc, p, (int) length)) == -1) {
				if (SYSCALL_INTERRUPTED(errno) && i < IO_RETRY)
					continue;
				if (status_vector)
					io_error(status_vector, "read", filename,
							 isc_io_read_err);
				return FB_FAILURE;
			}
			if (!len)
				break;
			length -= len;
			p += len;
		}

	if (length_read)
		*length_read = p - buffer;

	return FB_SUCCESS;
}


int LLIO_seek(
			  ISC_STATUS * status_vector,
			  SLONG file_desc, TEXT * filename, SLONG offset, USHORT whence)
{
/**************************************
 *
 *	L L I O _ s e e k		( g e n e r i c )
 *
 **************************************
 *
 * Functional description
 *	Seek to the given offset.
 *
 **************************************/

	if (whence != LLIO_SEEK_NONE) {
		switch (whence) {
		case LLIO_SEEK_BEGIN:
			whence = SEEK_SET;
			break;
		case LLIO_SEEK_CURRENT:
			whence = SEEK_CUR;
			break;
		case LLIO_SEEK_END:
			whence = SEEK_END;
			break;
		}

		/* Darwin - Add the offset cast because the types are different
 		 * between the implicit declaration and the actual declaration,
		 * causing problems with the stack frame, etc.  Bad.
 		 */
		if (lseek((int) file_desc, LSEEK_OFFSET_CAST offset, (int) whence) == -1) {
			if (status_vector)
				io_error(status_vector, "lseek", filename, isc_io_access_err);
			return FB_FAILURE;
		}
	}
	return FB_SUCCESS;
}


int LLIO_sync(ISC_STATUS * status_vector, SLONG file_desc)
{
/**************************************
 *
 *	L L I O _ s y n c		( g e n e r i c )
 *
 **************************************
 *
 * Functional description
 *	Flush the buffers in a file.
 *
 **************************************/

	return (fsync((int) file_desc) != -1) ? FB_SUCCESS : FB_FAILURE;
}


int LLIO_write(
			   ISC_STATUS * status_vector,
			   SLONG file_desc,
			   TEXT * filename,
			   SLONG offset,
USHORT whence, UCHAR * buffer, SLONG length, SLONG * length_written)
{
/**************************************
 *
 *	L L I O _ w r i t e		( g e n e r i c )
 *
 **************************************
 *
 * Functional description
 *	Write a record to a file.
 *
 **************************************/
	int len, i;
	UCHAR *p;

	if ((whence != LLIO_SEEK_NONE) &&
		(LLIO_seek(status_vector, file_desc, filename, offset, whence) ==
		 FB_FAILURE)) return FB_FAILURE;

	if (p = buffer)
		for (i = 0; length && i++ < IO_RETRY;) {
			if ((len = write((int) file_desc, p, (int) length)) == -1) {
				if (SYSCALL_INTERRUPTED(errno) && i < IO_RETRY)
					continue;
				if (status_vector)
					io_error(status_vector, "write", filename,
							 isc_io_write_err);
				return FB_FAILURE;
			}
			if (!len)
				break;
			length -= len;
			p += len;
		}

	if (length_written)
		*length_written = p - buffer;

	return FB_SUCCESS;
}


static void io_error(
					 ISC_STATUS* status_vector,
					 const TEXT* op, const TEXT* filename, ISC_STATUS operation)
{
/**************************************
 *
 *	i o _ e r r o r		( g e n e r i c )
 *
 **************************************
 *
 * Functional description
 *	Fill up the status_vector with error info.
 *
 **************************************/

	IBERR_build_status(status_vector, isc_io_error, isc_arg_string, op,
					   isc_arg_string, filename, isc_arg_gds, operation,
					   isc_arg_unix, errno, 0);
}
#endif

