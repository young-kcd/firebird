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
#include "../jrd/cch_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_f_proto.h"

#include "../jrd/lck_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/os/pio_proto.h"
#include "../common/classes/init.h"

#include <windows.h>

namespace Jrd {

class FileExtendLockGuard
{
public:
	FileExtendLockGuard(Firebird::RWLock* lock, bool exclusive) :
	  m_lock(lock), m_exclusive(exclusive) 
	{
		if (m_exclusive) {
			fb_assert(m_lock);
		}
		if (m_lock) {
			if (m_exclusive)
				m_lock->beginWrite();
			else
				m_lock->beginRead();
		}
	}

	~FileExtendLockGuard()
	{
		if (m_lock) {
			if (m_exclusive)
				m_lock->endWrite();
			else
				m_lock->endRead();
		}
	}

private:
	Firebird::RWLock*	m_lock;
	bool				m_exclusive;
};


} // namespace Jrd

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
static bool	maybe_close_file(HANDLE&);
static jrd_file* seek_file(jrd_file*, BufferDesc*, ISC_STATUS*, OVERLAPPED*, OVERLAPPED**);
static jrd_file* setup_file(Database*, const Firebird::PathName&, HANDLE);
static bool nt_error(TEXT*, const jrd_file*, ISC_STATUS, ISC_STATUS*);

static USHORT ostype;

#ifdef SUPERSERVER_V2
static const DWORD g_dwShareFlags = FILE_SHARE_READ;	// no write sharing
static const DWORD g_dwExtraFlags = FILE_FLAG_OVERLAPPED |
									FILE_FLAG_NO_BUFFERING;
#else
#ifdef SUPERSERVER
static const DWORD g_dwShareFlags = FILE_SHARE_READ;	// no write sharing
static const DWORD g_dwExtraFlags = 0;
#else
static const DWORD g_dwShareFlags = FILE_SHARE_READ | FILE_SHARE_WRITE;
static const DWORD g_dwExtraFlags = 0;
#endif
#endif

static const DWORD g_dwShareTempFlags = FILE_SHARE_READ;
static const DWORD g_dwExtraTempFlags = FILE_ATTRIBUTE_TEMPORARY |
										FILE_FLAG_DELETE_ON_CLOSE;


int PIO_add_file(Database* dbb, jrd_file* main_file, const Firebird::PathName& file_name, SLONG start)
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
	jrd_file* new_file = PIO_create(dbb, file_name, false, false, false);
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
		if (maybe_close_file(file->fil_desc))
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


jrd_file* PIO_create(Database* dbb, const Firebird::PathName& string,
					 bool overwrite, bool temporary, bool share_delete)
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
	const TEXT* file_name = string.c_str();

	if (!ISC_is_WinNT())
		share_delete = false;

	DWORD dwShareMode = (temporary ? g_dwShareTempFlags : g_dwShareFlags);
	if (share_delete)
		dwShareMode |= FILE_SHARE_DELETE;

	DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL | g_dwExtraFlags;
	if (temporary)
		dwFlagsAndAttributes |= g_dwExtraTempFlags;

	const HANDLE desc = CreateFile(file_name,
					  GENERIC_READ | GENERIC_WRITE,
					  dwShareMode,
					  NULL,
					  (overwrite ? CREATE_ALWAYS : CREATE_NEW),
					  dwFlagsAndAttributes,
					  0);

	if (desc == INVALID_HANDLE_VALUE)
	{
		ERR_post(isc_io_error,
				 isc_arg_string, "CreateFile (create)",
				 isc_arg_cstring, string.length(), ERR_cstring(string),
				 isc_arg_gds, isc_io_create_err, isc_arg_win32, GetLastError(),
				 0);
	}

/* File open succeeded.  Now expand the file name. */
/* workspace is the expanded name here */

	Firebird::PathName workspace(string);
	ISC_expand_filename(workspace, false);
	jrd_file *file;
	try {
		file = setup_file(dbb, workspace, desc);
	}
	catch (const Firebird::Exception&) {
		CloseHandle(desc);
		throw;
	}

	return file;
}


bool PIO_expand(const TEXT* file_name, USHORT file_length, TEXT* expanded_name,
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


void PIO_extend(jrd_file* main_file, const ULONG extPages, const USHORT pageSize)
{
/**************************************
 *
 *	P I O _ e x t e n d
 *
 **************************************
 *
 * Functional description
 *	Extend file by extPages pages of pageSize size. 
 *
 **************************************/
 
	// hvlad: prevent other reading\writing threads from changing file pointer.
	// As we open file without FILE_FLAG_OVERLAPPED, ReadFile\WriteFile calls
	// will change file pointer we set here and file truncation instead of file
	// extension will occurs.
	// It solved issue CORE-1468 (database file corruption when file extension 
	// and read\write activity performed simultaneously)

	// if file have no extend lock it is better to not extend file than corrupt it
	if (!main_file->fil_ext_lock)
		return;

	ThreadExit teHolder;
	FileExtendLockGuard extLock(main_file->fil_ext_lock, true);

	ULONG leftPages = extPages;
	for (jrd_file* file = main_file; file && leftPages; file = file->fil_next)
	{
		const ULONG filePages = PIO_get_number_of_pages(file, pageSize);
		const ULONG fileMaxPages = (file->fil_max_page == MAX_ULONG) ? MAX_ULONG : 
									file->fil_max_page - file->fil_min_page + 1;
		if (filePages < fileMaxPages)
		{
			const ULONG extendBy = MIN(fileMaxPages - filePages + file->fil_fudge, leftPages);

			HANDLE hFile = file->fil_desc;

			LARGE_INTEGER newSize; 
			newSize.QuadPart = (ULONGLONG) (filePages + extendBy) * pageSize;

			if (ostype == OS_CHICAGO) {	// WIN95
				file->fil_mutex.enter();
			}

			const DWORD ret = SetFilePointer(hFile, newSize.LowPart, &newSize.HighPart, FILE_BEGIN);
			if (ret == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR) {
				if (ostype == OS_CHICAGO) {
					file->fil_mutex.leave();
				}
				nt_error("SetFilePointer", file, isc_io_write_err, 0);
			}
			if (!SetEndOfFile(hFile)) {
				if (ostype == OS_CHICAGO) {
					file->fil_mutex.leave();
				}
				nt_error("SetEndOfFile", file, isc_io_write_err, 0);
			}

			if (ostype == OS_CHICAGO) {
				file->fil_mutex.leave();
			}

			leftPages -= extendBy;
		}
	}
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
			file->fil_mutex.enter();
		}
		FlushFileBuffers(file->fil_desc);
		if (ostype == OS_CHICAGO)
		{
			file->fil_mutex.leave();
		}
	}
}


void PIO_force_write(jrd_file* file, bool forceWrite, bool notUseFSCache)
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

	const bool oldForce = (file->fil_flags & FIL_force_write) != 0;
	const bool oldNotUseCache = (file->fil_flags & FIL_no_fs_cache) != 0;

	if (forceWrite != oldForce || notUseFSCache != oldNotUseCache)
	{
		const int force = forceWrite ? FILE_FLAG_WRITE_THROUGH : 0;
		const int fsCache = notUseFSCache ? FILE_FLAG_NO_BUFFERING : 0;
		const int writeMode = (file->fil_flags & FIL_readonly) ? 0 : GENERIC_WRITE;

        HANDLE& hFile = file->fil_desc;
		maybe_close_file(hFile);
		hFile = CreateFile(file->fil_string,
						  GENERIC_READ | writeMode,
						  g_dwShareFlags,
						  NULL,
						  OPEN_EXISTING,
						  FILE_ATTRIBUTE_NORMAL | force | fsCache | g_dwExtraFlags,
						  0);

		if (hFile == INVALID_HANDLE_VALUE)
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
		
		if (forceWrite) {
			file->fil_flags |= FIL_force_write;
		}
		else {
			file->fil_flags &= ~FIL_force_write;
		}
		if (notUseFSCache) {
			file->fil_flags |= FIL_no_fs_cache;
		}
		else {
			file->fil_flags &= ~FIL_no_fs_cache;
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
	PageSpace* pageSpace = dbb->dbb_page_manager.findPageSpace(DB_PAGE_SPACE);
	jrd_file* file = pageSpace->file;
	HANDLE desc = file->fil_desc;

	OVERLAPPED overlapped;
	OVERLAPPED* overlapped_ptr;

	if (ostype == OS_CHICAGO)
	{
		file->fil_mutex.enter();
		if (SetFilePointer(desc, 0, NULL, FILE_BEGIN) == (DWORD) -1)
		{
			file->fil_mutex.leave();
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
				file->fil_mutex.leave();
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
				file->fil_mutex.leave();
			}
			nt_error("ReadFile", file, isc_io_read_err, 0);
#endif
		}
	}

#ifdef SUPERSERVER_V2
	CloseHandle(overlapped.hEvent);
#endif

	if (ostype == OS_CHICAGO) {
		file->fil_mutex.leave();
	}
}

static Firebird::InitInstance<Firebird::ZeroBuffer> zeros;


USHORT PIO_init_data(Database* dbb, jrd_file* main_file, ISC_STATUS* status_vector, 
					 ULONG startPage, USHORT initPages)
{
/**************************************
 *
 *	P I O _ i n i t _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Initialize tail of file with zeros
 *
 **************************************/

	const char* zero_buff = zeros().getBuffer();
	const size_t zero_buff_size = zeros().getSize();

	ThreadExit teHolder;
	FileExtendLockGuard extLock(main_file->fil_ext_lock, false);

	// Fake buffer, used in seek_file. Page space ID have no matter there
	// as we already know file to work with
	BufferDesc bdb;
	bdb.bdb_dbb = dbb;
	bdb.bdb_page = PageNumber(0, startPage);

	OVERLAPPED overlapped;
	OVERLAPPED* overlapped_ptr;
	jrd_file* file = 
		seek_file(main_file, &bdb, status_vector, &overlapped, &overlapped_ptr);

	if (!file)
		return 0;

	if (ostype == OS_CHICAGO)
		file->fil_mutex.leave();

	if (file->fil_min_page + 8 > startPage)
		return 0;

	USHORT leftPages = initPages;
	const ULONG initBy = MIN(file->fil_max_page - startPage, leftPages);
	if (initBy < leftPages)
		leftPages = initBy;

	for (ULONG i = startPage; i < startPage + initBy; )
	{
		bdb.bdb_page = PageNumber(0, i);
		USHORT write_pages = zero_buff_size / dbb->dbb_page_size;
		if (write_pages > leftPages)
			write_pages = leftPages;

		seek_file(main_file, &bdb, status_vector, &overlapped, &overlapped_ptr);

		DWORD to_write = (DWORD) write_pages * dbb->dbb_page_size;
		DWORD written;

		if (!WriteFile(file->fil_desc, zero_buff, to_write, &written, overlapped_ptr) ||
			to_write != written)
		{
			if (ostype == OS_CHICAGO)
				file->fil_mutex.leave();

			nt_error("WriteFile", file, isc_io_write_err, status_vector);
			break;
		}

		if (ostype == OS_CHICAGO)
			file->fil_mutex.leave();

		leftPages -= write_pages;
		i += write_pages;
	}

	return (initPages - leftPages);
}


jrd_file* PIO_open(Database* dbb,
				   const Firebird::PathName& string,
				   bool trace_flag,
				   const Firebird::PathName& file_name,
				   bool share_delete)
{
/**************************************
 *
 *	P I O _ o p e n
 *
 **************************************
 *
 * Functional description
 *	Open a database file.
 *
 **************************************/
	const TEXT* ptr = (string.hasData() ? string : file_name).c_str();
	bool readOnly = false;

	if (!ISC_is_WinNT())
		share_delete = false;

	HANDLE desc = CreateFile(ptr,
					  GENERIC_READ | GENERIC_WRITE,
					  g_dwShareFlags | (share_delete ? FILE_SHARE_DELETE : 0),
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
					 file_name.length(),
					 ERR_cstring(file_name),
					 isc_arg_gds,
					 isc_io_open_err, isc_arg_win32, GetLastError(), 0);
		}
		else {
			/* If this is the primary file, set Database flag to indicate that it is
			 * being opened ReadOnly. This flag will be used later to compare with
			 * the Header Page flag setting to make sure that the database is set
			 * ReadOnly.
			 */
			readOnly = true;
			PageSpace* pageSpace = dbb->dbb_page_manager.findPageSpace(DB_PAGE_SPACE);
			if (!pageSpace->file)
				dbb->dbb_flags |= DBB_being_opened_read_only;
		}
	}

	jrd_file *file;
	try {
		file = setup_file(dbb, string, desc);

		if (readOnly)
			file->fil_flags |= FIL_readonly;
	}
	catch (const Firebird::Exception&) {
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

	ThreadExit teHolder;
	FileExtendLockGuard extLock(file->fil_ext_lock, false);

	OVERLAPPED overlapped, *overlapped_ptr;
	if (!(file = seek_file(file, bdb, status_vector, &overlapped, &overlapped_ptr)))
		return false;

	HANDLE desc = file->fil_desc;

	if (dbb->dbb_encrypt_key.hasData())
	{
		SLONG spare_buffer[MAX_PAGE_SIZE / sizeof(SLONG)];
        DWORD actual_length;

		if (!ReadFile(desc, spare_buffer, size, &actual_length, overlapped_ptr)
			|| actual_length != size)
		{
			if (ostype == OS_CHICAGO) {
				file->fil_mutex.leave();
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
				file->fil_mutex.leave();
			}
			return nt_error("ReadFile", file, isc_io_read_err, status_vector);
#endif
		}
	}

#ifdef SUPERSERVER_V2
	release_io_event(file, overlapped_ptr);
#endif

	if (ostype == OS_CHICAGO) {
		file->fil_mutex.leave();
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

	ThreadExit teHolder;

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

		HANDLE desc = file->fil_desc;

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

	ThreadExit teHolder;

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

	ThreadExit teHolder;
	FileExtendLockGuard extLock(file->fil_ext_lock, false);

	file = seek_file(file, bdb, status_vector, &overlapped,
				   &overlapped_ptr);
	if (!file) {
		return false;
	}

	HANDLE desc = file->fil_desc;

	if (dbb->dbb_encrypt_key.hasData()) {
		SLONG spare_buffer[MAX_PAGE_SIZE / sizeof(SLONG)];

		(*dbb->dbb_encrypt) (dbb->dbb_encrypt_key.c_str(),
							 page, size, spare_buffer);

		DWORD actual_length;
		if (!WriteFile(desc, spare_buffer, size, &actual_length, overlapped_ptr)
			|| actual_length != size)
		{
			if (ostype == OS_CHICAGO) {
				file->fil_mutex.leave();
			}
			return nt_error("WriteFile", file, isc_io_write_err, status_vector);
		}
	}
	else
	{
		DWORD actual_length;
		if (!WriteFile(desc, page, size, &actual_length, overlapped_ptr) 
			|| actual_length != size )
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
				file->fil_mutex.leave();
			}
			return nt_error("WriteFile", file, isc_io_write_err, status_vector);
#endif
		}
	}

#ifdef SUPERSERVER_V2
	release_io_event(file, overlapped_ptr);
#endif

	if (ostype == OS_CHICAGO) {
		file->fil_mutex.leave();
	}

	return true;
}


ULONG PIO_get_number_of_pages(const jrd_file* file, const USHORT pagesize)
{
/**************************************
 *
 *	P I O _ g e t _ n u m b e r _ o f _ p a g e s
 *
 **************************************
 *
 * Functional description
 *	Compute number of pages in file, based only on file size.
 *
 **************************************/
	HANDLE hFile = file->fil_desc;

	DWORD dwFileSizeHigh;
	const DWORD dwFileSizeLow = GetFileSize(hFile, &dwFileSizeHigh);

	if ((dwFileSizeLow == INVALID_FILE_SIZE) && (GetLastError() != NO_ERROR)) {
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

	file->fil_mutex.enter();
	for (int i = 0; i < MAX_FILE_IO; i++)
		if (!file->fil_io_events[i]) {
			file->fil_io_events[i] = overlapped->hEvent;
			overlapped->hEvent = NULL;
			break;
		}
	file->fil_mutex.leave();

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
	ULONG page = bdb->bdb_page.getPageNum();

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
		file->fil_mutex.enter();
		HANDLE desc = file->fil_desc;

		if (SetFilePointer(desc,
						   (LONG) liOffset.LowPart,
						   &liOffset.HighPart, FILE_BEGIN) == 0xffffffff)
		{
			file->fil_mutex.leave();
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
		file->fil_mutex.enter();
		for (USHORT i = 0; i < MAX_FILE_IO; i++) {
			if (overlapped->hEvent = (HANDLE) file->fil_io_events[i]) {
				file->fil_io_events[i] = 0;
				break;
			}
		}
		file->fil_mutex.leave();
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


static jrd_file* setup_file(Database*					dbb,
							const Firebird::PathName&	file_name,
							HANDLE						desc)
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

	jrd_file* file = FB_NEW_RPT(*dbb->dbb_permanent, file_name.length() + 1) jrd_file;
	file->fil_desc = desc;
	file->fil_length = file_name.length();
	file->fil_max_page = (ULONG) -1;
	file->fil_ext_lock = NULL;
#ifdef SUPERSERVER_V2
	memset(file->fil_io_events, 0, MAX_FILE_IO * sizeof(void*));
#endif
	MOVE_FAST(file_name.c_str(), file->fil_string, file_name.length());
	file->fil_string[file_name.length()] = 0;

/* If this isn't the primary file, we're done */

	PageSpace* pageSpace = dbb->dbb_page_manager.findPageSpace(DB_PAGE_SPACE);
	if (pageSpace && pageSpace->file)
		return file;

/* Set a local variable that indicates whether we're running
   under Windows/NT or Chicago */
// CVC: local variable to all this unit, it means.

	ostype = ISC_is_WinNT() ? OS_WINDOWS_NT : OS_CHICAGO;

/* Build unique lock string for file and construct lock block */

	BY_HANDLE_FILE_INFORMATION file_info;
	GetFileInformationByHandle(desc, &file_info);
	UCHAR lock_string[32];
	UCHAR* p = lock_string;

	// The identifier is [nFileIndexHigh, nFileIndexLow]
	// MSDN says: After a process opens a file, the identifier is constant until
	// the file is closed. An application can use this identifier and the
	// volume serial number to determine whether two handles refer to the same file.
	size_t l = sizeof(file_info.dwVolumeSerialNumber);
	memcpy(p, &file_info.dwVolumeSerialNumber, l);
	p += l;

	l = sizeof(file_info.nFileIndexHigh);
	memcpy(p, &file_info.nFileIndexHigh, l);
	p += l;

	l = sizeof(file_info.nFileIndexLow);
	memcpy(p, &file_info.nFileIndexLow, l);
	p += l;

	// We know p only was incremented, so can use safely size_t instead of ptrdiff_t
	l = p - lock_string;
	fb_assert(l <= sizeof(lock_string)); // In case we continue adding information.

	file->fil_ext_lock = FB_NEW(*dbb->dbb_permanent) Firebird::RWLock();

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
			SCHAR spare_memory[MIN_PAGE_SIZE * 2];
			SCHAR *header_page_buffer = (SCHAR*) FB_ALIGN((IPTR)spare_memory, MIN_PAGE_SIZE);
		
			pageSpace = dbb->dbb_page_manager.findPageSpace(DB_PAGE_SPACE);
			try {
				pageSpace->file = file;
				PIO_header(dbb, header_page_buffer, MIN_PAGE_SIZE);
				if ((reinterpret_cast<Ods::header_page*>(header_page_buffer)->hdr_flags & Ods::hdr_shutdown_mask) == Ods::hdr_shutdown_single)
					ERR_post(isc_shutdown, isc_arg_cstring, file_name.length(), ERR_cstring(file_name), 0);
				pageSpace->file = NULL; // Will be set again later by the caller				
			}
			catch (const Firebird::Exception&) {
				delete dbb->dbb_lock;
				dbb->dbb_lock = NULL;
				delete file;
				pageSpace->file = NULL; // Will be set again later by the caller
				throw;
			}
		}
	}

	return file;
}

static bool maybe_close_file(HANDLE& hFile)
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

	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
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
