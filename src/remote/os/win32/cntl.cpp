/*
 *	PROGRAM:	JRD Remote Server
 *	MODULE:		cntl.cpp
 *	DESCRIPTION:	Windows NT service control panel interface
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
#include <stdio.h>
#include "../jrd/common.h"
#include "../remote/remote.h"
#include "../jrd/thd.h"
#include "../utilities/install/install_nt.h"
#include "../remote/os/win32/cntl_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thread_proto.h"
#include "../jrd/jrd_proto.h"

#ifdef WIN_NT
#include <windows.h>
#endif

const int ERROR_BUFFER_LENGTH	= 1024;

struct cntl_thread
{
	cntl_thread* thread_next;
	HANDLE thread_handle;
};

static void WINAPI control_thread(DWORD);
static THREAD_ENTRY_DECLARE cleanup_thread(THREAD_ENTRY_PARAM);

static USHORT report_status(DWORD, DWORD, DWORD, DWORD);

static ThreadEntryPoint* main_handler;
static SERVICE_STATUS_HANDLE service_handle;
static Firebird::string* service_name = NULL;
static Firebird::string* mutex_name = NULL;
static HANDLE stop_event_handle;
static Firebird::Mutex thread_mutex;
static cntl_thread* threads = NULL;
static HANDLE hMutex = NULL;
static bool bGuarded = false;


void CNTL_init(ThreadEntryPoint* handler, const TEXT* name)
{
/**************************************
 *
 *	C N T L _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	main_handler = handler;
	MemoryPool& pool = *getDefaultMemoryPool();
	service_name = FB_NEW(pool) Firebird::string(pool);
	service_name->printf(REMOTE_SERVICE, name);
	mutex_name = FB_NEW(pool) Firebird::string(pool);
	mutex_name->printf(GUARDIAN_MUTEX, name);
}


void* CNTL_insert_thread()
{
/**************************************
 *
 *	C N T L _ i n s e r t _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	THREAD_ENTER();
	cntl_thread* new_thread = (cntl_thread*) ALLR_alloc((SLONG) sizeof(cntl_thread));
/* NOMEM: ALLR_alloc() handled */
/* FREE:  in CTRL_remove_thread() */

	THREAD_EXIT();
	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(),
					GetCurrentProcess(), &new_thread->thread_handle, 0, FALSE,
					DUPLICATE_SAME_ACCESS);

	thread_mutex.enter();
	new_thread->thread_next = threads;
	threads = new_thread;
    thread_mutex.leave();

	return new_thread;
}


void WINAPI CNTL_main_thread( DWORD argc, char* argv[])
{
/**************************************
 *
 *	C N T L _ m a i n _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	service_handle =
		RegisterServiceCtrlHandler(service_name->c_str(), control_thread);
	if (!service_handle)
		return;

	int status = 1;
	DWORD temp = 0;

	if (report_status(SERVICE_START_PENDING, NO_ERROR, 1, 3000) &&
		(stop_event_handle = CreateEvent(NULL, TRUE, FALSE, NULL)) != NULL &&
		report_status(SERVICE_START_PENDING, NO_ERROR, 2, 3000) &&
		!gds__thread_start(main_handler, NULL, 0, 0, 0)
		&& report_status(SERVICE_RUNNING, NO_ERROR, 0, 0))
	{
		status = 0;
		temp = WaitForSingleObject(stop_event_handle, INFINITE);
	}

	DWORD last_error = 0;
	if (temp == WAIT_FAILED || status)
		last_error = GetLastError();

	if (stop_event_handle)
		CloseHandle(stop_event_handle);
/* set the status with the timer, start the cleanup thread and wait for the
 * cleanup thread to exit or the timer to expire, once we reach the max number
 * of loops or the thread exits set the state to shutdown and exit */
	HANDLE cleanup_thread_handle;
	if (gds__thread_start(cleanup_thread, NULL, THREAD_medium, 0, 
		&cleanup_thread_handle) != 0) 
	{
		last_error = GetLastError();
		report_status(SERVICE_STOPPED, last_error, 0, 0);
		return;
	}

	DWORD count = 1;
	DWORD return_from_wait;

	do {
		count++;
		report_status(SERVICE_STOP_PENDING, NO_ERROR, count, 60000);
		return_from_wait = WaitForSingleObject(cleanup_thread_handle, 50000);
	} while (count < 10 && return_from_wait == WAIT_TIMEOUT);
/* loop for 10 times about 7 minutes should be enough time for the server to
 * cleanup */

/* TMN 29 Jul 2000 - close the thread handle */
	CloseHandle(cleanup_thread_handle);

	report_status(SERVICE_STOPPED, last_error, 0, 0);
}


void CNTL_remove_thread(void* athread)
{
/**************************************
 *
 *	C N T L _ r e m o v e _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	thread_mutex.enter();
	cntl_thread* const rem_thread = (cntl_thread*) athread;

	for (cntl_thread** thread_ptr = &threads;
		 *thread_ptr; thread_ptr = &(*thread_ptr)->thread_next)
	{
		if (*thread_ptr == rem_thread) {
			*thread_ptr = rem_thread->thread_next;
			break;
		}
	}
	thread_mutex.leave();

	CloseHandle(rem_thread->thread_handle);

	THREAD_ENTER();
	ALLR_free(rem_thread);
	THREAD_EXIT();
}


void CNTL_shutdown_service( const TEXT* message)
{
/**************************************
 *
 *	C N T L _ s h u t d o w n _ s e r v i c e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	const char* strings[2];

	char buffer[BUFFER_LARGE];
	sprintf(buffer, "%s error: %lu", service_name->c_str(), GetLastError());

	HANDLE event_source = RegisterEventSource(NULL, service_name->c_str());
	if (event_source) {
		strings[0] = buffer;
		strings[1] = message;
		ReportEvent(event_source,
					EVENTLOG_ERROR_TYPE,
					0,
					0,
					NULL,
					2,
					0,
					const_cast<const char**>(strings), NULL);
		DeregisterEventSource(event_source);
	}

	if (stop_event_handle)
		SetEvent(stop_event_handle);
}


static void WINAPI control_thread( DWORD action)
{
/**************************************
 *
 *	c o n t r o l _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *	Process a service control request.
 *
 **************************************/
	const DWORD state = SERVICE_RUNNING;

	switch (action) {
	case SERVICE_CONTROL_STOP:
		report_status(SERVICE_STOP_PENDING, NO_ERROR, 1, 3000);
		if (bGuarded == true)
			ReleaseMutex(hMutex);
		SetEvent(stop_event_handle);
		return;

	case SERVICE_CONTROL_INTERROGATE:
		break;

#if (defined SUPERCLIENT || defined SUPERSERVER)
	case SERVICE_CREATE_GUARDIAN_MUTEX:
		hMutex = OpenMutex(SYNCHRONIZE, FALSE, mutex_name->c_str());
		if (hMutex) {
			UINT error_mode = SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX |
				SEM_NOOPENFILEERRORBOX | SEM_NOALIGNMENTFAULTEXCEPT;
			SetErrorMode(error_mode);
			bGuarded = true;
			WaitForSingleObject(hMutex, INFINITE);
		}
		break;
#endif

	default:
		break;
	}

	report_status(state, NO_ERROR, 0, 0);
}

static THREAD_ENTRY_DECLARE cleanup_thread(THREAD_ENTRY_PARAM)
{
/**************************************
 *
 *	c l e a n u p _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *	This thread is responsible for the cleanup.
 *
 **************************************/
	ULONG attach_count, database_count;
	TEXT return_buffer[ERROR_BUFFER_LENGTH], *buff_ptr = return_buffer;

// find out if we have any attachments 
// if we do then log a message to the log file
	JRD_num_attachments(return_buffer, ERROR_BUFFER_LENGTH, JRD_info_dbnames,
						&attach_count, &database_count);

// if we have active attachments then log messages
	if (attach_count > 0) {
		TEXT print_buffer[ERROR_BUFFER_LENGTH], *print_ptr;
		sprintf(print_buffer,
				"Shutting down the Firebird service with %d active connection(s) to %d database(s)",
				attach_count, database_count);
		gds__log(print_buffer);

		TEXT out_message[ERROR_BUFFER_LENGTH + 100];
		// just get the ushort value and increment it by a ushort length
		const USHORT num_databases = * (USHORT *) buff_ptr;
		buff_ptr += sizeof(USHORT);
		for (USHORT i = 0; i < num_databases; i++) {
			const USHORT database_name_length = * (USHORT *) buff_ptr;
			buff_ptr += sizeof(USHORT);
			print_ptr = print_buffer;
			for (USHORT j = 0; j < database_name_length; j++)
				*print_ptr++ = *buff_ptr++;
			*print_ptr = '\0';
			sprintf(out_message,
					"The database %s was being accessed when the server was shutdown",
					print_buffer);
			gds__log(out_message);
		}
	}

	JRD_shutdown_all(false);
	return 0;
}


static USHORT report_status(
							DWORD state,
							DWORD exit_code, DWORD checkpoint, DWORD hint)
{
/**************************************
 *
 *	r e p o r t _ s t a t u s
 *
 **************************************
 *
 * Functional description
 *	Report our status to the control manager.
 *
 **************************************/
	SERVICE_STATUS status;
	status.dwServiceType =
		(SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS);
	status.dwServiceSpecificExitCode = 0;

	if (state == SERVICE_START_PENDING)
		status.dwControlsAccepted = 0;
	else
		status.dwControlsAccepted = SERVICE_ACCEPT_STOP;

	status.dwCurrentState = state;
	status.dwWin32ExitCode = exit_code;
	status.dwCheckPoint = checkpoint;
	status.dwWaitHint = hint;

	const USHORT ret = SetServiceStatus(service_handle, &status);
	if (!ret)
		CNTL_shutdown_service("SetServiceStatus");

	return ret;
}

