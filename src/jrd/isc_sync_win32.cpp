/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		isc_sync.c
 *	DESCRIPTION:	General purpose but non-user routines. Win32 specific.
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

#ifdef SHLIB_DEFS
#define LOCAL_SHLIB_DEFS
#endif

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <signal.h>
#include <windows.h>

#include "../jrd/time.h"
#include "../jrd/common.h"
#include "gen/codes.h"
#include "../jrd/isc.h"
#include "../jrd/err_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_i_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/file_params.h"
#include "../jrd/jrd.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thd_proto.h"


extern "C" {


static int		process_id;

static void	error(STATUS *, TEXT *, STATUS);
static void	make_object_name(	char*		buffer,
								const char*	object_name,
								const char*	object_type);

#ifdef SHLIB_DEFS

#define sprintf		(*_libgds_sprintf)
#define strlen		(*_libgds_strlen)
#define strcpy		(*_libgds_strcpy)
#define errno		(*_libgds_errno)
#define open		(*_libgds_open)
#define close		(*_libgds_close)

#pragma FB_COMPILER_MACRO("Warning! Possibly unsafe new declaration of errno.")

// e.g. MS C runtime library defines errno as a macro (to get the
// real errno in a thread-safe fashion).
extern int		errno;

#endif	// SHLIB_DEFS


#ifdef SIG_RESTART
//
// Return a flag that indicats whether or not to restart an interrupted
// system call.
//
BOOLEAN ISC_check_restart (void)
{
	return TRUE;
}
#endif	/* SIG_RESTART */


//
// If a wait would block, return TRUE.
//
int ISC_event_blocked(USHORT count, EVENT* events, SLONG* values)
{
	for (; count > 0; --count, ++events, ++values) {
		EVENT pEvent = *events;
		if (pEvent->event_shared) {
			pEvent = pEvent->event_shared;
		}
		if (pEvent->event_count >= *values) {
			return FALSE;
		}
	}
	return TRUE;
}

//
// Clear an event preparatory to waiting on it.  The order of
// battle for event synchronization is:
//
//    1.  Clear event.
//    2.  Test data structure for event already completed
//    3.  Wait on event.
//
SLONG DLL_EXPORT ISC_event_clear(EVENT event)
{
	ResetEvent ((HANDLE) event->event_handle);

	EVENT pEvent = event;
	if (pEvent->event_shared) {
		pEvent = pEvent->event_shared;
	}
	return pEvent->event_count + 1;
}


//
// Discard an event object.
//
void ISC_event_fini(EVENT event)
{
	CloseHandle ((HANDLE) event->event_handle);
}


//
// Prepare an event object for use.
//
int DLL_EXPORT ISC_event_init(EVENT event, int type, int semnum)
{
	event->event_pid = process_id = getpid();
	event->event_count = 0;
	event->event_type = type;
	event->event_shared = NULL;

	event->event_handle = ISC_make_signal(TRUE, TRUE, process_id, type);

	return (event->event_handle) ? TRUE : FALSE;
}


//
// Prepare an event object for use.
//
int ISC_event_init_shared (
    EVENT	lcl_event,
    int		type,
    TEXT	*name,
    EVENT	shr_event,
    USHORT	init_flag)
{
	TEXT	event_name [MAXPATHLEN], type_name [16];

	lcl_event->event_pid = process_id = getpid();
	lcl_event->event_count = 0;
	lcl_event->event_type = type;
	lcl_event->event_shared = shr_event;

	sprintf (type_name, "_event%d", type);
	make_object_name (event_name, name, type_name);
	if (!(lcl_event->event_handle = CreateEvent (ISC_get_security_desc(), TRUE, FALSE, event_name)))
		return FALSE;

	if (init_flag)
	{
		shr_event->event_pid = 0;
		shr_event->event_count = 0;
		shr_event->event_type = type;
		shr_event->event_handle = NULL;
		shr_event->event_shared = NULL;
	}

	return TRUE;
}


//
// Post an event to wake somebody else up.
//
int DLL_EXPORT ISC_event_post(EVENT	event)
{
	if (!event->event_shared)
		++event->event_count;
	else
		++event->event_shared->event_count;

	if (event->event_pid != process_id) {
		ISC_kill (event->event_pid, event->event_type, event->event_handle);
	} else {
		SetEvent ((HANDLE) event->event_handle);
	}

	return 0;
}


//
// Wait on an event.
//
int DLL_EXPORT ISC_event_wait (
    SSHORT	count,
    EVENT	*events,
    SLONG	*values,
    SLONG	micro_seconds,
    void	(*timeout_handler)(),
    void	*handler_arg)
{
	EVENT*	ptr;
	EVENT*	end;
	HANDLE	handles[16];
	HANDLE*	handle_ptr;

	/* If we're not blocked, the rest is a gross waste of time */

	if (!ISC_event_blocked (count, events, values)) {
		return 0;
	}

	for (ptr = events, end = events + count, handle_ptr = handles; ptr < end;) {
		*handle_ptr++ = (*ptr++)->event_handle;
	}

	/* Go into wait loop */

	DWORD timeout = (micro_seconds > 0) ? micro_seconds / 1000 : INFINITE;

	for (;;)
	{
		if (!ISC_event_blocked (count, events, values)) {
			return 0;
		}

		const DWORD status =
			WaitForMultipleObjects((DWORD) count, handles, TRUE, timeout);

		if (!((status >= WAIT_OBJECT_0) && (status < WAIT_OBJECT_0 + count))) {
			return status;
		}
	}
}


#ifdef SUPERSERVER

/**************************************
 *
 *     When we got a sync exception, fomulate the error code	
 *     write it to the log file, and abort. Note: We can not
 *     actually call "abort" since in windows this will cause
 *     a dialog to appear stating the obvious!  Since on NT we
 *     would not get a core file, there is actually no difference
 *     between abort() and exit(3). 
 *
 **************************************/
ULONG ISC_exception_post(ULONG except_code, TEXT* err_msg)
{
	ULONG result;
	bool is_critical = true;
	
	if (!SCH_thread_enter_check ())
	{
		THREAD_ENTER;
	}

	TDBB tdbb = GET_THREAD_DATA;

	if (!err_msg)
	{
		err_msg = "";
	}

	TEXT *log_msg = (TEXT *) gds__alloc(strlen(err_msg) + 256);

switch (except_code) 
    {
    case EXCEPTION_ACCESS_VIOLATION:
		sprintf (log_msg, "%s Access violation.\n"
				"\t\tThe code attempted to access a virtual\n"
				"\t\taddress without privilege to do so.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
    case EXCEPTION_DATATYPE_MISALIGNMENT:
		sprintf (log_msg, "%s Datatype misalignment.\n"
				"\t\tThe attempted to read or write a value\n"
				"\t\tthat was not stored on a memory boundary.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		sprintf (log_msg, "%s Array bounds exceeded.\n"
				"\t\tThe code attempted to access an array\n"
				"\t\telement that is out of bounds.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
    case EXCEPTION_FLT_DENORMAL_OPERAND:
		sprintf (log_msg, "%s Float denormal operand.\n"
				"\t\tOne of the floating-point operands is too\n"
				"\t\tsmall to represent as a standard floating-point\n"
				"\t\tvalue.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		sprintf (log_msg, "%s Floating-point divide by zero.\n"
				"\t\tThe code attempted to divide a floating-point\n"
				"\t\tvalue by a floating-point divisor of zero.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
    case EXCEPTION_FLT_INEXACT_RESULT:
		sprintf (log_msg, "%s Floating-point inexact result.\n"
				"\t\tThe result of a floating-point operation cannot\n"
				"\t\tbe represented exactly as a decimal fraction.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
    case EXCEPTION_FLT_INVALID_OPERATION:
		sprintf (log_msg, "%s Floating-point invalid operand.\n"
				"\t\tAn indeterminant error occurred during a\n"
				"\t\tfloating-point operation.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
    case EXCEPTION_FLT_OVERFLOW:
		sprintf (log_msg, "%s Floating-point overflow.\n"
				"\t\tThe exponent of a floating-point operation\n"
				"\t\tis greater than the magnitude allowed.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
    case EXCEPTION_FLT_STACK_CHECK:
		sprintf (log_msg, "%s Floating-point stack check.\n"
				"\t\tThe stack overflowed or underflowed as the\n"
				"\t\tresult of a floating-point operation.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
    case EXCEPTION_FLT_UNDERFLOW:
		sprintf (log_msg, "%s Floating-point underflow.\n"
				"\t\tThe exponent of a floating-point operation\n"
				"\t\tis less than the magnitude allowed.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
		sprintf (log_msg, "%s Integer divide by zero.\n"
				"\t\tThe code attempted to divide an integer value\n"
				"\t\tby an integer divisor of zero.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
    case EXCEPTION_INT_OVERFLOW:
		sprintf (log_msg, "%s Interger overflow.\n"
				"\t\tThe result of an integer operation caused the\n"
				"\t\tmost significant bit of the result to carry.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
    case EXCEPTION_STACK_OVERFLOW:
		ERR_post(isc_exception_stack_overflow, 0);
		/* This will never be called, but to be safe it's here */
		result = EXCEPTION_CONTINUE_EXECUTION;
		is_critical = false;
		break;

    case EXCEPTION_BREAKPOINT:
    case EXCEPTION_SINGLE_STEP:
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
    case EXCEPTION_INVALID_DISPOSITION:
    case EXCEPTION_PRIV_INSTRUCTION:
    case EXCEPTION_IN_PAGE_ERROR:
    case EXCEPTION_ILLEGAL_INSTRUCTION:
    case EXCEPTION_GUARD_PAGE:
		/* Pass these exception on to someone else,
           probably the OS or the debugger, since there
           isn't a dam thing we can do with them */
		result = EXCEPTION_CONTINUE_SEARCH;
		is_critical = false;
		break;
    default:
		/* If we've catched our own software exception,
		   continue rewinding the stack to properly handle it
		   and deliver an error information to the client side */
		if (tdbb->tdbb_status_vector[0] == 1 && tdbb->tdbb_status_vector[1] > 0)
		{
			result = EXCEPTION_CONTINUE_SEARCH;
			is_critical = false;
		}
		else
		{
			sprintf (log_msg, "%s An exception occurred that does\n"
					"\t\tnot have a description.  Exception number %X.\n"
					"\tThis exception will cause the Firebird server\n"
					"\tto terminate abnormally.", err_msg, except_code);
		}
		break; 
	}

	gds__log(log_msg);
	gds__free(log_msg);

	if (is_critical)
	{
		exit(3);
	}
	else
	{
		return result;
	}
}

#endif /* SUPERSERVER */


//
// Create or open a Windows/NT event.
// Use the signal number and process id
// in naming the object.
//
void* ISC_make_signal(
    BOOLEAN	create_flag,
    BOOLEAN	manual_reset,
    int		process_id,
    int		signal_number)
{
	if (!signal_number) {
		return CreateEvent (NULL, manual_reset, FALSE, NULL);
	}

	TEXT event_name [64];
	sprintf(event_name, "_interbase_process%u_signal%d", process_id, signal_number);

	HANDLE hEvent;
	if (create_flag) {
		hEvent = CreateEvent(ISC_get_security_desc(), manual_reset, FALSE, event_name);
	} else {
		hEvent = OpenEvent(EVENT_ALL_ACCESS, TRUE, event_name);
	}
	return hEvent;
}


//
// Try to map a given file.  If we are the first (i.e. only)
// process to map the file, call a given initialization
// routine (if given) or punt (leaving the file unmapped).
//
UCHAR * DLL_EXPORT ISC_map_file (
	STATUS*	status_vector,
	TEXT*	filename,
#if 1
	// TMN: Parameter is in errors!
	void	(*init_routine)(void *, struct sh_mem *, int),
#else
	// MUST of course match header.
	FPTR_VOID	init_routine,
#endif
	void*	init_arg,
	SLONG	length,
	SH_MEM	shmem_data)
{
	TEXT	expanded_filename [MAXPATHLEN], hostname [64], *p;
	TEXT	map_file [MAXPATHLEN];
	HANDLE	file_handle, event_handle;
	int		init_flag;
	DWORD	ret_event, fdw_create;
	int	retry_count = 0;

	/* To maintain compatibility with the FAT file system we will truncate
	   the host name to 8 characters.  Heaven help us if this creates
	   an ambiguity. */


	/* retry to attach to mmapped file if the process initializing
	 * dies during initialization.
	 */

retry:
	retry_count++;

	ISC_get_host (hostname, sizeof (hostname));
	hostname [8] = 0;
	sprintf (map_file, filename, hostname);

	if (length < 0)
		length = -length;

	file_handle =
		CreateFile(	map_file,
					GENERIC_READ | GENERIC_WRITE,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL,
					OPEN_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,
					NULL);
	if (file_handle == INVALID_HANDLE_VALUE)
	{
		error(status_vector, "CreateFile", GetLastError());
		return NULL;
	}

	/* Check if file already exists */

	const bool file_exists = (GetLastError() == ERROR_ALREADY_EXISTS);

	// Create an event that can be used to determine if someone has already
	// initialized shared memory.

	make_object_name (expanded_filename, filename, "_event");

#ifndef CHICAGO_FIXED
	if (!ISC_is_WinNT())
		event_handle = CreateMutex (ISC_get_security_desc(), TRUE, expanded_filename);
	else
#endif
	event_handle = CreateEvent (ISC_get_security_desc(), TRUE, FALSE, expanded_filename);

	if (!event_handle)
	{
#ifndef CHICAGO_FIXED
		if (!ISC_is_WinNT())
			error (status_vector, "CreateMutex", GetLastError());
		else
#endif
		error (status_vector, "CreateEvent", GetLastError());
		CloseHandle(file_handle);
		return NULL;
	}

	init_flag = (GetLastError() == ERROR_ALREADY_EXISTS) ? FALSE : TRUE;

	if (init_flag && !init_routine)
	{
		CloseHandle (event_handle);
		CloseHandle (file_handle);
		*status_vector++ = gds_arg_gds;
		*status_vector++ = gds_unavailable;
		*status_vector++ = gds_arg_end;
		return NULL;
	}

	if (length == 0)
	{
		/* Get and use the existing length of the shared segment */
		
		if ((length = GetFileSize (file_handle, NULL)) == -1)
		{
			error (status_vector, "GetFileSize", GetLastError());
			CloseHandle (event_handle);
			CloseHandle (file_handle);
			return NULL;
		}
	}

	/* All but the initializer will wait until the event is set.  That
	 * is done after initialization is complete.
	 * Close the file and wait for the event to be set or time out.
	 * The file may be truncated.
	 */

	CloseHandle (file_handle);

	if (!init_flag)
	{
		/* Wait for 10 seconds.  Then retry */

#ifndef CHICAGO_FIXED
		if (!ISC_is_WinNT())
		{
			ret_event = WaitForSingleObject (event_handle, 10000);
			ReleaseMutex (event_handle);
		}
		else
#endif
			ret_event = WaitForSingleObject (event_handle, 10000);

		// If we timed out, just retry.  It is possible that the
		// process doing the initialization died before setting the
		// event.

		if (ret_event == WAIT_TIMEOUT)
		{
			CloseHandle (event_handle);
			if (retry_count > 10)
			{
				error (status_vector, "WaitForSingleObject", GetLastError());
				return NULL;
			}
			goto retry;
		}
	}

	if (init_flag && file_exists)
		fdw_create = TRUNCATE_EXISTING | OPEN_ALWAYS;
	else
		fdw_create = OPEN_ALWAYS;

	file_handle = CreateFile(	map_file,
								GENERIC_READ | GENERIC_WRITE,
								FILE_SHARE_READ | FILE_SHARE_WRITE,
								NULL,
								fdw_create,
								FILE_ATTRIBUTE_NORMAL,
								NULL);
	if (file_handle == INVALID_HANDLE_VALUE)
	{
		CloseHandle (event_handle);
		error (status_vector, "CreateFile", GetLastError());
		return NULL;
	}

	// Create a file mapping object that will be used to make remapping
	// possible. The current length of real mapped file and its name
	// are saved in it.
	make_object_name (expanded_filename, filename, "_mapping");

	HANDLE header_obj =
		CreateFileMapping ((HANDLE) -1,
							ISC_get_security_desc(),
							PAGE_READWRITE,
							0,
							2 * sizeof (SLONG),
							expanded_filename);
	if (header_obj == NULL)
	{
		error (status_vector, "CreateFileMapping", GetLastError());
		CloseHandle (event_handle);
		CloseHandle (file_handle);
		return NULL;
	}

	SLONG* header_address =
		(SLONG*)MapViewOfFile (header_obj, FILE_MAP_WRITE, 0, 0, 0);

	if (header_address == NULL)
	{
		error (status_vector, "CreateFileMapping", GetLastError());
		CloseHandle (header_obj);
		CloseHandle (event_handle);
		CloseHandle (file_handle);
		return NULL;
	}

	// Set or get the true length of the file depending on whether or not
	// we are the first user.

	if (init_flag)
	{
		header_address [0] = length;
		header_address [1] = 0;
	}
	else
		length = header_address [0];

	/* Create the real file mapping object. */

	for (p = expanded_filename; *p; p++)
		;
	sprintf (p, "%d", header_address [1]);

	HANDLE file_obj =
		CreateFileMapping(	file_handle,
							ISC_get_security_desc(),
							PAGE_READWRITE,
							0,
							length,
							expanded_filename);
	if (file_obj == NULL)
	{
		error (status_vector, "CreateFileMapping", GetLastError());
		UnmapViewOfFile (header_address);
		CloseHandle (header_obj);
		CloseHandle (event_handle);
		CloseHandle (file_handle);
		return NULL;
	}

	UCHAR* address =
		(UCHAR*)MapViewOfFile (file_obj, FILE_MAP_WRITE, 0, 0, 0);

	if (address == NULL)
	{
		error (status_vector, "CreateFileMapping", GetLastError());
		CloseHandle (file_obj);
		UnmapViewOfFile (header_address);
		CloseHandle (header_obj);
		CloseHandle (event_handle);
		CloseHandle (file_handle);
		return NULL;
	}

	*p = 0;

	shmem_data->sh_mem_address = address;
	shmem_data->sh_mem_length_mapped = length;
	shmem_data->sh_mem_handle = file_handle;
	shmem_data->sh_mem_object = file_obj;
	shmem_data->sh_mem_interest = event_handle;
	shmem_data->sh_mem_hdr_object = header_obj;
	shmem_data->sh_mem_hdr_address = header_address;
	strcpy (shmem_data->sh_mem_name, expanded_filename);

	if (init_routine) {
		// Lie a bit to make it compile...
		reinterpret_cast<void(*)(void*, sh_mem*,int)>(*init_routine) (init_arg, shmem_data, init_flag);
	}

	if (init_flag)
	{
		FlushViewOfFile (address, 0);
#ifndef CHICAGO_FIXED
		if (!ISC_is_WinNT())
			ReleaseMutex (event_handle);
		else
#endif
		SetEvent (event_handle);
		if (SetFilePointer (shmem_data->sh_mem_handle, length, NULL, FILE_BEGIN) == 0xFFFFFFFF ||
			!SetEndOfFile (shmem_data->sh_mem_handle) ||
			!FlushViewOfFile (shmem_data->sh_mem_address, 0))
		{
			error (status_vector, "SetFilePointer", GetLastError());
			return NULL;
		}
	}

	return address;
}


//
// Initialize a mutex.
//
int DLL_EXPORT ISC_mutex_init(MTX mutex, TEXT* mutex_name)
{
	char name_buffer[MAXPATHLEN];

	make_object_name(name_buffer, mutex_name, "_mutex");
	mutex->mtx_handle = CreateMutex(ISC_get_security_desc(), FALSE, name_buffer);

	return mutex->mtx_handle ? 0 : 1;
}


//
// Sieze a mutex.
//
int DLL_EXPORT ISC_mutex_lock(MTX mutex)
{
	const DWORD status = WaitForSingleObject(mutex->mtx_handle, INFINITE);
	return (!status || status == WAIT_ABANDONED) ? 0 : 1;
}


//
// Conditionally sieze a mutex.
//
int DLL_EXPORT ISC_mutex_lock_cond(MTX mutex)
{
	const DWORD status = WaitForSingleObject (mutex->mtx_handle, 0L);
	return (!status || status == WAIT_ABANDONED) ? 0 : 1;
}


//
// Release a mutex.
//
int DLL_EXPORT ISC_mutex_unlock(MTX mutex)
{
	return !ReleaseMutex(mutex->mtx_handle);
}


//
// Try to re-map a given file.
//
UCHAR * DLL_EXPORT ISC_remap_file (
    STATUS*	status_vector,
    SH_MEM	shmem_data,
    SLONG	new_length,
    USHORT	flag)
{
	if (flag) {
		if (SetFilePointer (shmem_data->sh_mem_handle, new_length, NULL, FILE_BEGIN) == 0xFFFFFFFF ||
			!SetEndOfFile (shmem_data->sh_mem_handle) ||
			!FlushViewOfFile (shmem_data->sh_mem_address, 0))
		{
			error (status_vector, "SetFilePointer", GetLastError());
			return NULL;
		}
	}

	/* If the remap file exists, remap does not occur correctly.
	 * The file number is local to the process and when it is 
	 * incremented and a new filename is created, that file may
	 * already exist.  In that case, the file is not expanded.
	 * This will happen when the file is expanded more than once
	 * by concurrently running processes.
	 * 
	 * The problem will be fixed by making sure that a new file name
	 * is generated with the mapped file is created.
	 */

	HANDLE file_obj;

	while (1)
	{
		TEXT expanded_filename[MAXPATHLEN];
		sprintf(expanded_filename,
				"%s%d",
				shmem_data->sh_mem_name,
				shmem_data->sh_mem_hdr_address[1] + 1);

		file_obj = CreateFileMapping(	shmem_data->sh_mem_handle,
										ISC_get_security_desc(),
										PAGE_READWRITE,
										0,
										new_length,
										expanded_filename);

		if (!((GetLastError () == ERROR_ALREADY_EXISTS) && flag))
			break;

		CloseHandle (file_obj);
		shmem_data->sh_mem_hdr_address [1] ++;
	}

	if (file_obj == NULL)
	{
		error (status_vector, "CreateFileMapping", GetLastError());
		return NULL;
	}

	LPVOID address = MapViewOfFile(file_obj, FILE_MAP_WRITE, 0, 0, 0);

	if (address == NULL)
	{
		error (status_vector, "CreateFileMapping", GetLastError());
		CloseHandle (file_obj);
		return NULL;
	}

	if (flag)
	{
		shmem_data->sh_mem_hdr_address [0] = new_length;
		shmem_data->sh_mem_hdr_address [1]++;
	}

	UnmapViewOfFile(shmem_data->sh_mem_address);
	CloseHandle(shmem_data->sh_mem_object);

	shmem_data->sh_mem_address = reinterpret_cast<UCHAR*>(address);
	shmem_data->sh_mem_length_mapped = new_length;
	shmem_data->sh_mem_object = file_obj;

	return reinterpret_cast<UCHAR*>(address);
}


//
// Detach from the shared memory.  Depending upon the flag,
// get rid of the semaphore and/or get rid of shared memory.
//
void DLL_EXPORT ISC_unmap_file(
    STATUS	*status_vector,
    SH_MEM	shmem_data,
    USHORT	flag)
{
	CloseHandle (shmem_data->sh_mem_interest);
	CloseHandle ((HANDLE) shmem_data->sh_mem_mutex_arg);
	UnmapViewOfFile (shmem_data->sh_mem_address);
	CloseHandle (shmem_data->sh_mem_object);
	if (flag & ISC_MEM_REMOVE) {
		if (SetFilePointer (shmem_data->sh_mem_handle, 0, NULL, FILE_BEGIN) != 0xFFFFFFFF) {
			SetEndOfFile (shmem_data->sh_mem_handle);
		}
	}
	CloseHandle (shmem_data->sh_mem_handle);
	UnmapViewOfFile(shmem_data->sh_mem_hdr_address);
	CloseHandle (shmem_data->sh_mem_hdr_object);
}


//
// We've encountered an error, report it.
//
static void error(STATUS* status_vector, TEXT* string, STATUS status)
{
	*status_vector++ = gds_arg_gds;
	*status_vector++ = gds_sys_request;
	*status_vector++ = gds_arg_string;
	*status_vector++ = reinterpret_cast<STATUS>(string);
	*status_vector++ = SYS_ARG;
	*status_vector++ = status;
	*status_vector++ = gds_arg_end;
}


//
// Create an object name from a name and type.
// Also replace the file separator with "_".
//
static void make_object_name(	char*		buffer,
								const char*	object_name,
								const char*	object_type)
{
	char	hostname[64];
	char*	p;
	char	c;

	sprintf(buffer, object_name, ISC_get_host(hostname, sizeof(hostname)));
	for (p = buffer; c = *p; p++) {
		if (c == '/' || c == '\\' || c == ':') {
			*p = '_';
		}
	}
	strcpy(p, object_type);
}


}	// extern "C"
