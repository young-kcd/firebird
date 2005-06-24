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

typedef struct thread {
	struct thread *thread_next;
	HANDLE thread_handle;
} *THREAD;

static void WINAPI control_thread(DWORD);
static THREAD_ENTRY_DECLARE cleanup_thread(THREAD_ENTRY_PARAM);

static void parse_switch(const TEXT*, int*);
static USHORT report_status(DWORD, DWORD, DWORD, DWORD);

static DWORD current_state;
static ThreadEntryPoint* main_handler;
static SERVICE_STATUS_HANDLE global_service_handle;
static const TEXT* global_service_name;
static HANDLE stop_event_handle;
static MUTX_T thread_mutex[1];
static THREAD threads;
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
	global_service_name = name;
}


void *CNTL_insert_thread(void)
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
	THREAD new_thread = (THREAD) ALLR_alloc((SLONG) sizeof(struct thread));
/* NOMEM: ALLR_alloc() handled */
/* FREE:  in CTRL_remove_thread() */

	THREAD_EXIT();
	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(),
					GetCurrentProcess(), &new_thread->thread_handle, 0, FALSE,
					DUPLICATE_SAME_ACCESS);

	THD_mutex_lock(thread_mutex);
	new_thread->thread_next = threads;
	threads = new_thread;
	THD_mutex_unlock(thread_mutex);

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
	global_service_handle =
		RegisterServiceCtrlHandler(global_service_name, control_thread);
	if (!global_service_handle)
		return;

#if (defined SUPERCLIENT || defined SUPERSERVER)
	int flag = SRVR_multi_client;
#else
	int flag = 0;
#endif

/* Parse the command line looking for any additional arguments. */

	argv++;

	while (--argc) {
		const TEXT* p = *argv++;
		if (*p++ == '-') // replaced assignment by comparison
			parse_switch(p, &flag);
	}

	int status = 1;
	DWORD temp = 0;

	if (report_status(SERVICE_START_PENDING, NO_ERROR, 1, 3000) &&
		(stop_event_handle = CreateEvent(NULL, TRUE, FALSE, NULL)) != NULL &&
		report_status(SERVICE_START_PENDING, NO_ERROR, 2, 3000) &&
		!gds__thread_start(main_handler, (void *) flag, 0, 0, 0)
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


void CNTL_remove_thread( void *thread)
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
	THD_mutex_lock(thread_mutex);
	for (THREAD* thread_ptr = &threads;
		 *thread_ptr; thread_ptr = &(*thread_ptr)->thread_next)
	{
		if (*thread_ptr == (THREAD) thread) {
			*thread_ptr = ((THREAD) thread)->thread_next;
			break;
		}
	}
	THD_mutex_unlock(thread_mutex);

	THREAD this_thread = (THREAD) thread;
	CloseHandle(this_thread->thread_handle);

	THREAD_ENTER();
	ALLR_free(thread);
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

	char buffer[256];
	sprintf(buffer, "%s error: %lu", "Firebird Server", GetLastError());

	HANDLE event_source = RegisterEventSource(NULL, "Firebird Server");
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
		hMutex = OpenMutex(SYNCHRONIZE, FALSE, GUARDIAN_MUTEX);
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
	USHORT attach_count, database_count;
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


static void parse_switch( const TEXT* switches, int* flag)
{
/**************************************
 *
 *	p a r s e _ s w i t c h
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	TEXT c;

	while (c = *switches++)
		switch (UPPER(c)) {
		case 'B':
			*flag |= SRVR_high_priority;
			break;

		case 'I':
			*flag |= SRVR_inet;
			break;

		case 'R':
			*flag &= ~SRVR_high_priority;
			break;

		case 'W':
			*flag |= SRVR_wnet;
			break;

		case 'X':
			*flag |= SRVR_xnet;
			break;
		}

#if (defined SUPERCLIENT || defined SUPERSERVER)
	*flag |= SRVR_multi_client;
#else
	*flag &= ~SRVR_multi_client;
#endif
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

	status.dwCurrentState = current_state = state;
	status.dwWin32ExitCode = exit_code;
	status.dwCheckPoint = checkpoint;
	status.dwWaitHint = hint;

	const USHORT ret = SetServiceStatus(global_service_handle, &status);
	if (!ret)
		CNTL_shutdown_service("SetServiceStatus");

	return ret;
}

