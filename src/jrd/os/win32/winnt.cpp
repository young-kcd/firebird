/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		winnt.cpp
 *	DESCRIPTION:	Windows NT specific physical IO
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
 * 2001.07.06 Sean Leyne - Code Cleanup, removed "#ifdef READONLY_DATABASE"
 *                         conditionals, as the engine now fully supports
 *                         readonly databases.
 *
 *
 * 17-Oct-2001 Mike Nordell: Non-shared file access
 *
 * 02-Nov-2001 Mike Nordell: Synch with FB1 changes.
 *
 * 20-Nov-2001 Ann Harrison: Make page count work on db with forced write
 * 
 * 21-Nov-2001 Ann Harrison: Allow read sharing so gstat works 
 */

#include "firebird.h"
#include <string.h>
#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../jrd/os/pio.h"
#include "../jrd/ods.h"
#include "../jrd/lck.h"
#include "../jrd/cch.h"
#include "gen/iberror.h"
#include "../jrd/all_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_f_proto.h"

#include "../jrd/lck_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/os/pio_proto.h"
#include "../jrd/thd.h"

#include <windows.h>


using namespace Jrd;

#ifdef TEXT
#undef TEXT
#endif
#define TEXT		SCHAR

const USHORT OS_WINDOWS_NT	= 1;
const USHORT OS_CHICAGO		= 2;

#ifdef SUPERSERVER_V2
static void release_io_event(jrd_file*, OVERLAPPED*);
#endif
static ULONG get_number_of_pages(const jrd_file*, const USHORT);
static bool	MaybeCloseFile(SLONG*);
static jrd_file* seek_file(jrd_file*, BufferDesc*, ISC_STATUS*, OVERLAPPED*, OVERLAPPED**);
static jrd_file* setup_file(Database*, const TEXT*, USHORT, HANDLE);
static bool nt_error(TEXT*, const jrd_file*, ISC_STATUS, ISC_STATUS*);

static USHORT ostype;

#ifdef SUPERSERVER_V2
static const DWORD g_dwShareFlags = FILE_SHARE_READ;	// no write sharing
static const DWORD g_dwExtraFlags = FILE_FLAG_OVERLAPPED |
									FILE_FLAG_NO_BUFFERING |
									FILE_FLAG_RANDOM_ACCESS;
#else
#ifdef SUPERSERVER
static const DWORD g_dwShareFlags = FILE_SHARE_READ;	// no write sharing
static const DWORD g_dwExtraFlags = FILE_FLAG_RANDOM_ACCESS;
#else
static const DWORD g_dwShareFlags = FILE_SHARE_READ | FILE_SHARE_WRITE;
static const DWORD g_dwExtraFlags = FILE_FLAG_RANDOM_ACCESS;
#endif
#endif



int PIO_add_file(Database* dbb, jrd_file* main_file, const TEXT* file_name, SLONG start)
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
 *
 **************************************/
	jrd_file* new_file = PIO_create(dbb, file_name, strlen(file_name), false);
	if (!new_file) {
		return 0;
	}

	new_file->fil_min_page = start;
	USHORT sequence = 1;

	jrd_file* file;
	for (file = main_file; file->fil_next; file = file->fil_next) {
		++sequence;
	}

	file->fil_max_page = start - 1;
	file->fil_next = new_file;

	return sequence;
}


void PIO_close(jrd_file* main_file)
{
/**************************************
 *
 *	P I O _ c l o s e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	for (jrd_file* file = main_file; file; file = file->fil_next)
	{
		if (MaybeCloseFile(&file->fil_desc) ||
			MaybeCloseFile(&file->fil_force_write_desc))
		{
#ifdef SUPERSERVER_V2
			for (int i = 0; i < MAX_FILE_IO; i++)
			{
				if (file->fil_io_events[i])
				{
					CloseHandle((HANDLE) file->fil_io_events[i]);
					file->fil_io_events[i] = 0;
				}
			}
#endif
		}
	}
}


int PIO_connection(const TEXT* file_name, USHORT* file_length)
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

	return 0;
}



jrd_file* PIO_create(Database* dbb, const TEXT* string, SSHORT length, bool overwrite)
{
/**************************************
 *
 *	P I O _ c r e a t e
 *
 **************************************
 *
 * Functional description
 *	Create a new database file.
 *
 **************************************/
	TEXT workspace[MAXPATHLEN];

	const TEXT* file_name = string;

	if (length)
	{
		MOVE_FAST(file_name, workspace, length);
		workspace[length] = 0;
		file_name = workspace;
	}

	const HANDLE desc = CreateFile(file_name,
					  GENERIC_READ | GENERIC_WRITE,
					  g_dwShareFlags,
					  NULL,
					  (overwrite ? CREATE_ALWAYS : CREATE_NEW),
					  FILE_ATTRIBUTE_NORMAL |
					  g_dwExtraFlags,
					  0);

	if (desc == INVALID_HANDLE_VALUE)
	{
		ERR_post(isc_io_error,
				 isc_arg_string, "CreateFile (create)",
				 isc_arg_cstring,
				 length,
				 ERR_string(string, length),
				 isc_arg_gds,
				 isc_io_create_err, isc_arg_win32, GetLastError(), 0);
	}

/* File open succeeded.  Now expand the file name. */
/* workspace is the exapnded name here */

	length = PIO_expand(string, length, workspace, sizeof(workspace));
	jrd_file *file;
	try {
		file = setup_file(dbb, workspace, length, desc);
	} catch(const std::exception&) {
		CloseHandle(desc);
		throw;
	}

	return file;
}


int PIO_expand(const TEXT* file_name, USHORT file_length, TEXT* expanded_name,
	size_t len_expanded)
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

	return ISC_expand_filename(file_name, file_length, 
				expanded_name, len_expanded, false);
}


void PIO_flush(jrd_file* main_file)
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
	for (jrd_file* file = main_file; file; file = file->fil_next)
	{
		if (ostype == OS_CHICAGO)
		{
			THD_MUTEX_LOCK(file->fil_mutex);
		}
		FlushFileBuffers((HANDLE) file->fil_desc);
		if (ostype == OS_CHICAGO)
		{
			THD_MUTEX_UNLOCK(file->fil_mutex);
		}
	}
}


void PIO_force_write(jrd_file* file, bool flag)
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

	const bool bOldForce = (file->fil_flags & FIL_force_write_init) != 0;

	if ((flag && !bOldForce) || (!flag && bOldForce)) {
		SLONG& hOld = flag ? file->fil_desc : file->fil_force_write_desc;
		HANDLE& hNew = reinterpret_cast<HANDLE&>(flag ? file->fil_force_write_desc : file->fil_desc);
        const int force = flag ? FILE_FLAG_WRITE_THROUGH : 0;

		MaybeCloseFile(&hOld);
		hNew = CreateFile(file->fil_string,
						  GENERIC_READ | GENERIC_WRITE,
						  g_dwShareFlags,
						  NULL,
						  OPEN_EXISTING,
						  FILE_ATTRIBUTE_NORMAL | force | g_dwExtraFlags,
						  0);

		if (hNew == INVALID_HANDLE_VALUE)
		{
			ERR_post(isc_io_error,
					 isc_arg_string,
					 "CreateFile (force write)",
					 isc_arg_cstring,
					 file->fil_length,
					 ERR_string(file->fil_string, file->fil_length),
					 isc_arg_gds, isc_io_access_err,
					 isc_arg_win32,
					 GetLastError(),
					 0);
		}
		
		if (flag) {
			file->fil_flags |= (FIL_force_write | FIL_force_write_init);
		}
		else {
			file->fil_flags &= ~FIL_force_write;
		}
	}
}


void PIO_header(Database* dbb, SCHAR * address, int length)
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
 *  The detail of Win32 implementation is that it doesn't assume 
 *  this fact as seeks to first byte of file initially, but external
 *  callers should not rely on this behavior
 *
 **************************************/
	jrd_file* file = dbb->dbb_file;
	HANDLE desc = (HANDLE) ((file->fil_flags & FIL_force_write) ?
					 file->fil_force_write_desc : file->fil_desc);

	OVERLAPPED overlapped, *overlapped_ptr;
	if (ostype == OS_CHICAGO)
	{
		THD_MUTEX_LOCK(file->fil_mutex);
		if (SetFilePointer(desc, 0, NULL, FILE_BEGIN) == (DWORD) -1)
		{
			THD_MUTEX_UNLOCK(file->fil_mutex);
			nt_error("SetFilePointer", file, isc_io_read_err, 0);
		}
		overlapped_ptr = NULL;
	}
	else
	{
		memset(&overlapped, 0, sizeof(OVERLAPPED));
		overlapped_ptr = &overlapped;
#ifdef SUPERSERVER_V2
		if (!(overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
			nt_error("Overlapped event", file, isc_io_read_err, 0);
		ResetEvent(overlapped.hEvent);
#endif
	}

    DWORD actual_length;
	if (dbb->dbb_encrypt_key.hasData())
	{
		SLONG spare_buffer[MAX_PAGE_SIZE / sizeof(SLONG)];

		if (!ReadFile(desc, spare_buffer, length, &actual_length, overlapped_ptr)
				|| actual_length != (DWORD)length)
		{
			if (ostype == OS_CHICAGO)
			{
				THD_MUTEX_UNLOCK(file->fil_mutex);
			}
			nt_error("ReadFile", file, isc_io_read_err, 0);
		}

		(*dbb->dbb_decrypt) (dbb->dbb_encrypt_key.c_str(),
							 spare_buffer, length, address);
	}
	else
	{
		if (!ReadFile(desc, address, length, &actual_length, overlapped_ptr)
				|| actual_length != (DWORD)length)
		{
#ifdef SUPERSERVER_V2
			if (!GetOverlappedResult(desc, overlapped_ptr, &actual_length, TRUE)
					|| actual_length != length)
			{
					CloseHandle(overlapped.hEvent);
					nt_error("GetOverlappedResult", file, isc_io_read_err, 0);
			}
#else
			if (ostype == OS_CHICAGO)
			{
				THD_MUTEX_UNLOCK(file->fil_mutex);
			}
			nt_error("ReadFile", file, isc_io_read_err, 0);
#endif
		}
	}

#ifdef SUPERSERVER_V2
	CloseHandle(overlapped.hEvent);
#endif

	if (ostype == OS_CHICAGO) {
		THD_MUTEX_UNLOCK(file->fil_mutex);
	}
}


SLONG PIO_max_alloc(Database* dbb)
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
	jrd_file* file = dbb->dbb_file;

	while (file->fil_next) {
		file = file->fil_next;
	}

	const ULONG nPages = get_number_of_pages(file, dbb->dbb_page_size);

	return file->fil_min_page - file->fil_fudge + nPages;
}


SLONG PIO_act_alloc(Database* dbb)
{
/**************************************
 *
 *	P I O _ a c t _ a l l o c
 *
 **************************************
 *
 * Functional description
 *  Compute actual number of physically allocated pages of database.
 *
 **************************************/
	SLONG tot_pages = 0;

/**
 **  Traverse the linked list of files and add up the number of pages
 **  in each file
 **/
	for (const jrd_file* file = dbb->dbb_file; file != NULL; file = file->fil_next) {
		tot_pages += get_number_of_pages(file, dbb->dbb_page_size);
	}

	return tot_pages;
}


jrd_file* PIO_open(Database* dbb,
			 const TEXT* string,
			 SSHORT length,
			 bool trace_flag,
			 blk* connection, const TEXT* file_name, USHORT file_length)
{
/**************************************
 *
 *	P I O _ o p e n
 *
 **************************************
 *
 * Functional description
 *	Open a database file. If a "connection"
 *	block is provided, use the connection
 *	to communicate with a page/lock server.
 *
 **************************************/
	TEXT temp[MAXPATHLEN];

	const TEXT* ptr;
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

	HANDLE desc = CreateFile(ptr,
					  GENERIC_READ | GENERIC_WRITE,
					  g_dwShareFlags,
					  NULL,
					  OPEN_EXISTING,
					  FILE_ATTRIBUTE_NORMAL |
					  g_dwExtraFlags,
					  0);

	if (desc == INVALID_HANDLE_VALUE) {
		/* Try opening the database file in ReadOnly mode.
		 * The database file could be on a RO medium (CD-ROM etc.).
		 * If this fileopen fails, return error.
		 */
		desc = CreateFile(ptr,
						  GENERIC_READ,
						  FILE_SHARE_READ,
						  NULL,
						  OPEN_EXISTING,
						  FILE_ATTRIBUTE_NORMAL |
						  g_dwExtraFlags, 0);

		if (desc == INVALID_HANDLE_VALUE) {
			ERR_post(isc_io_error,
					 isc_arg_string,
					 "CreateFile (open)",
					 isc_arg_cstring,
					 file_length,
					 ERR_string(file_name, file_length),
					 isc_arg_gds,
					 isc_io_open_err, isc_arg_win32, GetLastError(), 0);
		}
		else {
			/* If this is the primary file, set Database flag to indicate that it is
			 * being opened ReadOnly. This flag will be used later to compare with
			 * the Header Page flag setting to make sure that the database is set
			 * ReadOnly.
			 */
			if (!dbb->dbb_file)
				dbb->dbb_flags |= DBB_being_opened_read_only;
		}
	}

	jrd_file *file;
	try {
		file = setup_file(dbb, string, length, desc);
	} catch(const std::exception&) {
		CloseHandle(desc);
		throw;
	}
	return file;
}


bool PIO_read(jrd_file* file, BufferDesc* bdb, Ods::pag* page, ISC_STATUS* status_vector)
{
/**************************************
 *
 *	P I O _ r e a d
 *
 **************************************
 *
 * Functional description
 *	Read a data page.
 *
 **************************************/
	Database* dbb = bdb->bdb_dbb;
	const DWORD size = dbb->dbb_page_size;

	OVERLAPPED overlapped, *overlapped_ptr;
	if (!(file = seek_file(file, bdb, status_vector, &overlapped, &overlapped_ptr)))
		return false;

	HANDLE desc = (HANDLE) ((file->fil_flags & FIL_force_write) ?
					 file->fil_force_write_desc : file->fil_desc);

	if (dbb->dbb_encrypt_key.hasData())
	{
		SLONG spare_buffer[MAX_PAGE_SIZE / sizeof(SLONG)];
        DWORD actual_length;

		if (!ReadFile(desc, spare_buffer, size, &actual_length, overlapped_ptr)
			|| actual_length != size)
		{
			if (ostype == OS_CHICAGO) {
				THD_MUTEX_UNLOCK(file->fil_mutex);
			}
			return nt_error("ReadFile", file, isc_io_read_err, status_vector);
		}

		(*dbb->dbb_decrypt) (dbb->dbb_encrypt_key.c_str(),
							 spare_buffer, size, page);
	}
	else
	{
		DWORD actual_length;
		if (!ReadFile(desc, page, size, &actual_length, overlapped_ptr) ||
			actual_length != size)
		{
#ifdef SUPERSERVER_V2
			if (!GetOverlappedResult(desc, overlapped_ptr, &actual_length, TRUE)
				|| actual_length != size)
			{
				release_io_event(file, overlapped_ptr);
				return nt_error("GetOverlappedResult", file, isc_io_read_err,
								status_vector);
			}
#else
			if (ostype == OS_CHICAGO) {
				THD_MUTEX_UNLOCK(file->fil_mutex);
			}
			return nt_error("ReadFile", file, isc_io_read_err, status_vector);
#endif
		}
	}

#ifdef SUPERSERVER_V2
	release_io_event(file, overlapped_ptr);
#endif

	if (ostype == OS_CHICAGO) {
		THD_MUTEX_UNLOCK(file->fil_mutex);
	}

	return true;
}


#ifdef SUPERSERVER_V2
bool PIO_read_ahead(Database*		dbb,
				   SLONG	start_page,
				   SCHAR*	buffer,
				   SLONG	pages,
				   phys_io_blk*		piob,
				   ISC_STATUS*	status_vector)
{
/**************************************
 *
 *	P I O _ r e a d _ a h e a d
 *
 **************************************
 *
 * Functional description
 *	Read a contiguous set of pages. The only
 *	tricky part is to segment the I/O when crossing
 *	file boundaries.
 *
 **************************************/
	OVERLAPPED overlapped, *overlapped_ptr;

/* If an I/O status block was passed the caller wants to
   queue an asynchronous I/O. */

	if (!piob) {
		overlapped_ptr = &overlapped;
	}
	else {
		overlapped_ptr = (OVERLAPPED *) & piob->piob_io_event;
		piob->piob_flags = 0;
	}

	BufferDesc bdb;
	while (pages) {
		/* Setup up a dummy buffer descriptor block for seeking file. */

		bdb.bdb_dbb = dbb;
		bdb.bdb_page = start_page;

		jrd_file* file = seek_file(dbb->dbb_file,
						&bdb, status_vector,
						overlapped_ptr,
						&overlapped_ptr);
		if (!file) {
			return false;
		}

		/* Check that every page within the set resides in the same database
		   file. If not read what you can and loop back for the rest. */

		DWORD segmented_length = 0;
		while (pages && start_page >= file->fil_min_page
			   && start_page <= file->fil_max_page)
		{
			segmented_length += dbb->dbb_page_size;
			++start_page;
			--pages;
		}

		HANDLE desc = (HANDLE) ((file->fil_flags & FIL_force_write) ?
						 file->fil_force_write_desc : file->fil_desc);

		DWORD actual_length;
		if (ReadFile(	desc,
						buffer,
						segmented_length,
						&actual_length,
						overlapped_ptr) &&
			actual_length == segmented_length)
		{
			if (piob && !pages) {
				piob->piob_flags = PIOB_success;
			}
		}
		else if (piob && !pages) {
			piob->piob_flags = PIOB_pending;
			piob->piob_desc = reinterpret_cast<SLONG>(desc);
			piob->piob_file = file;
			piob->piob_io_length = segmented_length;
		}
		else if (!GetOverlappedResult(	desc,
										overlapped_ptr,
										&actual_length,
										TRUE) ||
				actual_length != segmented_length)
		{
			if (piob) {
					piob->piob_flags = PIOB_error;
			}
			release_io_event(file, overlapped_ptr);
			return nt_error("GetOverlappedResult", file, isc_io_read_err,
							status_vector);
		}

		if (!piob || (piob->piob_flags & (PIOB_success | PIOB_error)))
			release_io_event(file, overlapped_ptr);

		buffer += segmented_length;
	}

	return true;
}
#endif


#ifdef SUPERSERVER_V2
bool PIO_status(phys_io_blk* piob, ISC_STATUS* status_vector)
{
/**************************************
 *
 *	P I O _ s t a t u s
 *
 **************************************
 *
 * Functional description
 *	Check the status of an asynchronous I/O.
 *
 **************************************/

	if (!(piob->piob_flags & PIOB_success)) {
		if (piob->piob_flags & PIOB_error) {
			return false;
		}
		DWORD actual_length;
		if (!GetOverlappedResult((HANDLE) piob->piob_desc,
								 (OVERLAPPED *) & piob->piob_io_event,
								 &actual_length,
								 piob->piob_wait) ||
			actual_length != piob->piob_io_length)
		{
			release_io_event(piob->piob_file,
							 (OVERLAPPED *) & piob->piob_io_event);
			return nt_error("GetOverlappedResult", piob->piob_file,
							isc_io_error, status_vector);
		}
	}

	release_io_event(piob->piob_file, (OVERLAPPED *) & piob->piob_io_event);
	return true;
}
#endif


bool PIO_write(jrd_file* file, BufferDesc* bdb, Ods::pag* page, ISC_STATUS* status_vector)
{
/**************************************
 *
 *	P I O _ w r i t e
 *
 **************************************
 *
 * Functional description
 *	Write a data page.
 *
 **************************************/
	OVERLAPPED overlapped, *overlapped_ptr;

	Database*   dbb  = bdb->bdb_dbb;
	const DWORD size = dbb->dbb_page_size;

	file = seek_file(file, bdb, status_vector, &overlapped,
				   &overlapped_ptr);
	if (!file) {
		return false;
	}

	HANDLE desc = (HANDLE) ((file->fil_flags & FIL_force_write) ?
					 file->fil_force_write_desc : file->fil_desc);

	if (dbb->dbb_encrypt_key.hasData()) {
		SLONG spare_buffer[MAX_PAGE_SIZE / sizeof(SLONG)];

		(*dbb->dbb_encrypt) (dbb->dbb_encrypt_key.c_str(),
							 page, size, spare_buffer);

		DWORD actual_length;
		if (!WriteFile(desc, spare_buffer, size, &actual_length, overlapped_ptr)
			|| actual_length != size)
		{
			if (ostype == OS_CHICAGO) {
				THD_MUTEX_UNLOCK(file->fil_mutex);
			}
			return nt_error("WriteFile", file, isc_io_write_err, status_vector);
		}
	}
	else
	{
		DWORD actual_length;
		if (!WriteFile(desc, page, size, &actual_length, overlapped_ptr) &&
			actual_length == size)
		{
#ifdef SUPERSERVER_V2
			if (!GetOverlappedResult(desc, overlapped_ptr, &actual_length, TRUE)
				|| actual_length != size)
			{
				release_io_event(file, overlapped_ptr);
				return nt_error("GetOverlappedResult", file, isc_io_write_err,
								status_vector);
			}
#else
			if (ostype == OS_CHICAGO) {
				THD_MUTEX_UNLOCK(file->fil_mutex);
			}
			return nt_error("WriteFile", file, isc_io_write_err, status_vector);
#endif
		}
	}

#ifdef SUPERSERVER_V2
	release_io_event(file, overlapped_ptr);
#endif

	if (ostype == OS_CHICAGO) {
		THD_MUTEX_UNLOCK(file->fil_mutex);
	}

	return true;
}



static ULONG get_number_of_pages(const jrd_file* file, const USHORT pagesize)
{
/**************************************
 *
 *	g e t _ n u m b e r _ o f _ p a g e s
 *
 **************************************
 *
 * Functional description
 *	Compute number of pages in file, based only on file size.
 *
 **************************************/
	HANDLE hFile = (HANDLE) ((file->fil_flags & FIL_force_write) ?
		file->fil_force_write_desc : file->fil_desc);

	DWORD dwFileSizeHigh;
	const DWORD dwFileSizeLow = GetFileSize(hFile, &dwFileSizeHigh);

	if (dwFileSizeLow == (DWORD) -1) {
		nt_error("GetFileSize", file, isc_io_access_err, 0);
	}

    const ULONGLONG ullFileSize = (((ULONGLONG) dwFileSizeHigh) << 32)
		+ dwFileSizeLow;
	return (ULONG) ((ullFileSize + pagesize - 1) / pagesize);
}


#ifdef SUPERSERVER_V2
static void release_io_event(jrd_file* file, OVERLAPPED* overlapped)
{
/**************************************
 *
 *	r e l e a s e _ i o _ e v e n t
 *
 **************************************
 *
 * Functional description
 *	Release an overlapped I/O event
 *	back to the file block.
 *
 **************************************/
	if (!overlapped || !overlapped->hEvent)
		return;

	THD_MUTEX_LOCK(file->fil_mutex);
	for (int i = 0; i < MAX_FILE_IO; i++)
		if (!file->fil_io_events[i]) {
			file->fil_io_events[i] = overlapped->hEvent;
			overlapped->hEvent = NULL;
			break;
		}
	THD_MUTEX_UNLOCK(file->fil_mutex);

	if (overlapped->hEvent)
		CloseHandle(overlapped->hEvent);
}
#endif


static jrd_file* seek_file(jrd_file*			file,
					 BufferDesc*			bdb,
					 ISC_STATUS*		status_vector,
					 OVERLAPPED*	overlapped,
					 OVERLAPPED**	overlapped_ptr)
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
	Database* dbb = bdb->bdb_dbb;
	ULONG page = bdb->bdb_page;

	for (;; file = file->fil_next) {
		if (!file) {
			CORRUPT(158);		/* msg 158 database file not available */
		}
		else if (page >= file->fil_min_page && page <= file->fil_max_page) {
			break;
		}
	}

	page -= file->fil_min_page - file->fil_fudge;

    LARGE_INTEGER liOffset;
	liOffset.QuadPart =
		UInt32x32To64((DWORD) page, (DWORD) dbb->dbb_page_size);

	if (ostype == OS_CHICAGO) {
		THD_MUTEX_LOCK(file->fil_mutex);
		HANDLE desc = (HANDLE) ((file->fil_flags & FIL_force_write) ?
						 file->fil_force_write_desc : file->fil_desc);

		if (SetFilePointer(desc,
						   (LONG) liOffset.LowPart,
						   &liOffset.HighPart, FILE_BEGIN) == 0xffffffff)
		{
			THD_MUTEX_UNLOCK(file->fil_mutex);
			nt_error("SetFilePointer", file, isc_io_access_err, status_vector);
			return 0;
		}
		*overlapped_ptr = NULL;
	}
	else {
		overlapped->Offset = liOffset.LowPart;
		overlapped->OffsetHigh = liOffset.HighPart;
		overlapped->Internal = 0;
		overlapped->InternalHigh = 0;
		overlapped->hEvent = (HANDLE) 0;

		*overlapped_ptr = overlapped;

#ifdef SUPERSERVER_V2
		THD_MUTEX_LOCK(file->fil_mutex);
		for (USHORT i = 0; i < MAX_FILE_IO; i++) {
			if (overlapped->hEvent = (HANDLE) file->fil_io_events[i]) {
				file->fil_io_events[i] = 0;
				break;
			}
		}
		THD_MUTEX_UNLOCK(file->fil_mutex);
		if (!overlapped->hEvent &&
			!(overlapped->hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
		{
			nt_error("CreateEvent", file, isc_io_access_err, status_vector);
			return 0;
		}
		ResetEvent(overlapped->hEvent);
#endif
	}

	return file;
}


static jrd_file* setup_file(Database*		dbb,
					  const TEXT*		file_name,
					  USHORT	file_length,
					  HANDLE	desc)
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

/* Allocate file block and copy file name string */

	jrd_file* file = FB_NEW_RPT(*dbb->dbb_permanent, file_length + 1) jrd_file;
	file->fil_desc = reinterpret_cast<SLONG>(desc);
	file->fil_force_write_desc =
		reinterpret_cast<SLONG>(INVALID_HANDLE_VALUE);
	file->fil_length = file_length;
	file->fil_max_page = (ULONG) -1;
#ifdef SUPERSERVER_V2
	memset(file->fil_io_events, 0, MAX_FILE_IO * sizeof(SLONG));
#endif
	MOVE_FAST(file_name, file->fil_string, file_length);
	file->fil_string[file_length] = 0;

/* If this isn't the primary file, we're done */

	if (dbb->dbb_file)
		return file;

/* Set a local variable that indicates whether we're running
   under Windows/NT or Chicago */
// CVC: local variable to all this unit, it means.

	ostype = ISC_is_WinNT() ? OS_WINDOWS_NT : OS_CHICAGO;

/* Build unique lock string for file and construct lock block */

	BY_HANDLE_FILE_INFORMATION file_info;
	GetFileInformationByHandle((HANDLE) desc, &file_info);
	UCHAR lock_string[32];
	UCHAR* p = lock_string;

	const UCHAR* q = (UCHAR *) &file_info.dwVolumeSerialNumber;
	size_t l = sizeof(file_info.dwVolumeSerialNumber);
	do {
		*p++ = *q++;
	} while (--l);

	q = (UCHAR *) &file_info.nFileIndexHigh;
	l = sizeof(file_info.nFileIndexHigh);
	do {
		*p++ = *q++;
	} while (--l);

	q = (UCHAR *) &file_info.nFileIndexLow;
	l = sizeof(file_info.nFileIndexLow);
	do {
		*p++ = *q++;
	} while (--l);

	// We know p only was incremented, so can use safely size_t instead of ptrdiff_t
	l = p - lock_string;

	Lock* lock = FB_NEW_RPT(*dbb->dbb_permanent, l) Lock;
	dbb->dbb_lock = lock;
	lock->lck_type = LCK_database;
	lock->lck_owner_handle = LCK_get_owner_handle(NULL, lock->lck_type);
	lock->lck_object = reinterpret_cast<blk*>(dbb);
	lock->lck_length = l;
	lock->lck_dbb = dbb;
	lock->lck_ast = CCH_down_grade_dbb;
	MOVE_FAST(lock_string, lock->lck_key.lck_string, l);

/* Try to get an exclusive lock on database.  If this fails, insist
   on at least a shared lock */

	dbb->dbb_flags |= DBB_exclusive;
	if (!LCK_lock(NULL, lock, LCK_EX, LCK_NO_WAIT)) {
		dbb->dbb_flags &= ~DBB_exclusive;
		thread_db* tdbb = JRD_get_thread_data();
		
		while (!LCK_lock(tdbb, lock, LCK_SW, -1)) {
			tdbb->tdbb_status_vector[0] = 0; // Clean status vector from lock manager error code
			// If we are in a single-threaded maintenance mode then clean up and stop waiting
			SCHAR spare_memory[MIN_PAGE_SIZE*2];
			SCHAR *header_page_buffer = (SCHAR*) FB_ALIGN((IPTR)spare_memory, MIN_PAGE_SIZE);
		
			try {
				dbb->dbb_file = file;
				PIO_header(dbb, header_page_buffer, MIN_PAGE_SIZE);
				if ((reinterpret_cast<Ods::header_page*>(header_page_buffer)->hdr_flags & Ods::hdr_shutdown_mask) == Ods::hdr_shutdown_single)
					ERR_post(isc_shutdown, isc_arg_string, ERR_string(file_name, file_length), 0);
				dbb->dbb_file = NULL; // Will be set again later by the caller				
			} catch(const std::exception&) {
				delete dbb->dbb_lock;
				dbb->dbb_lock = NULL;
				delete file;
				dbb->dbb_file = NULL; // Will be set again later by the caller
				throw;
			}
		}
	}

	return file;
}

static bool MaybeCloseFile(SLONG* pFile)
{
/**************************************
 *
 *	M a y b e C l o s e F i l e
 *
 **************************************
 *
 * Functional description
 *	If the file is open, close it.
 *
 **************************************/

	if (pFile && (HANDLE)*pFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle((HANDLE)*pFile);
		*pFile = (SLONG) INVALID_HANDLE_VALUE;
		return true;
	}
	return false;
}

static bool nt_error(TEXT*	string,
						const jrd_file*		file,
						ISC_STATUS	operation,
						ISC_STATUS*	status_vector)
{
/**************************************
 *
 *	n t _ e r r o r
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
		*status_vector++ = isc_arg_string;
		*status_vector++ = (ISC_STATUS) string;
		*status_vector++ = isc_arg_string;
		*status_vector++ =
			(ISC_STATUS)(U_IPTR) ERR_string(file->fil_string, file->fil_length);
		*status_vector++ = isc_arg_gds;
		*status_vector++ = operation;
		*status_vector++ = isc_arg_win32;
		*status_vector++ = GetLastError();
		*status_vector++ = isc_arg_end;
		return false;
	}

	ERR_post(isc_io_error,
			 isc_arg_string, string,
			 isc_arg_string, ERR_string(file->fil_string, file->fil_length),
			 isc_arg_gds, operation, isc_arg_win32, GetLastError(), 0);

	return true;
}

