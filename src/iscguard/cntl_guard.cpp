/*
 *	PROGRAM:	Guardian CNTL function.
 *	MODULE:		cntl_guard.cpp
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
#include "../iscguard/cntlg_proto.h"
#include "../remote/remote.h"
#include "../utilities/install/install_nt.h"
#include "../jrd/thd.h"
#include "../jrd/isc_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/gds_proto.h"

#ifdef WIN_NT
#include <windows.h>
#endif

struct thread {
	thread* thread_next;
	HANDLE thread_handle;
};

static void WINAPI control_thread(DWORD);
static void parse_switch(const TEXT*, int*);
static USHORT report_status(DWORD, DWORD, DWORD, DWORD);

static DWORD current_state;
static ThreadEntryPoint* main_handler;
static SERVICE_STATUS_HANDLE service_handle;
static const TEXT* service_name;
static HANDLE stop_event_handle;
#ifdef NOT_USED_OR_REPLACED
static MUTX_T thread_mutex[1];
static thread* threads;
#endif


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
	service_name = name;
}


void CNTL_main_thread( SLONG argc, SCHAR* argv[])
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
	DWORD last_error = 0;

	service_handle = RegisterServiceCtrlHandler(service_name, control_thread);
	if (!service_handle)
		return;

// THD_mutex_init (thread_mutex);

#if (defined SUPERCLIENT || defined SUPERSERVER)
	int flag = SRVR_multi_client;
#else
	int flag = 0;
#endif

/* Parse the command line looking for any additional arguments. */

	argv++;

	while (--argc) {
		const TEXT* p = *argv++;
		if (*p++ == '-')
			parse_switch(p, &flag);
	}

/* start everything, and wait here for ever, or at
 * least until we get the stop event indicating that
 * the service is stoping. */

	bool failure = true;
	DWORD temp = 0;
	if (report_status(SERVICE_START_PENDING, NO_ERROR, 1, 3000) &&
		(stop_event_handle = CreateEvent(NULL, TRUE, FALSE, NULL)) != NULL &&
		report_status(SERVICE_START_PENDING, NO_ERROR, 2, 3000) &&
		!gds__thread_start(main_handler, (void *) flag, 0, 0, 0)
		&& report_status(SERVICE_RUNNING, NO_ERROR, 0, 0))
	{
		failure = false;
		temp = WaitForSingleObject(stop_event_handle, INFINITE);
	}

	if (failure || temp == WAIT_FAILED)
		last_error = GetLastError();

	if (stop_event_handle)
		CloseHandle(stop_event_handle);

/* ONce we are stopped, we will tell the server to
 * do the same.  We could not do this in the control_thread
 * since the Services Control Manager is single threaded,
 * and thus can only process one request at the time. */
	{
		SERVICE_STATUS status_info;
		SC_HANDLE hScManager = 0, hService = 0;
		hScManager =
			OpenSCManager(NULL, NULL, GENERIC_READ);
		hService = OpenService(hScManager, REMOTE_SERVICE,
			GENERIC_READ | GENERIC_EXECUTE);
		ControlService(hService, SERVICE_CONTROL_STOP, &status_info);
		CloseServiceHandle(hScManager);
		CloseServiceHandle(hService);
	}

	report_status(SERVICE_STOPPED, last_error, 0, 0);

// THD_mutex_destroy (thread_mutex);
}

void CNTL_shutdown_service(const TEXT* message)
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

	sprintf(buffer, "%s error: %lu", service_name, GetLastError());

	HANDLE event_source = RegisterEventSource(NULL, service_name);
	if (event_source) {
		strings[0] = buffer;
		strings[1] = message;
		ReportEvent(event_source,
					EVENTLOG_ERROR_TYPE,
					0,
					0,
					NULL, 2, 0, strings, NULL);
		DeregisterEventSource(event_source);
	}

	if (stop_event_handle)
		SetEvent(stop_event_handle);
}


void CNTL_stop_service(const TEXT* service) // unused param
{
/**************************************
 *
 *	C N T L _ s t o p _ s e r v i c e
 *
 **************************************
 *
 * Functional description
 *   This function is called to stop the service.
 *
 *
 **************************************/

	SC_HANDLE servicemgr_handle = OpenSCManager(NULL, NULL, GENERIC_READ);
	if (servicemgr_handle == NULL) {
		// return error
		int error = GetLastError();
		gds__log("SC manager error %d", error);
		return;
	}

	SC_HANDLE service_handleL =
		OpenService(servicemgr_handle, service_name,
			GENERIC_READ | GENERIC_EXECUTE);

	if (service_handleL == NULL) {
		// return error
		int error = GetLastError();
		gds__log("open services error %d", error);
		return;
	}
	else {
		SERVICE_STATUS status_info;
		if (!ControlService
			(service_handleL, SERVICE_CONTROL_STOP, &status_info))
		{
			// return error
			const int error = GetLastError();
			gds__log("Control services error %d", error);
			return;
		}
	}
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
		SetEvent(stop_event_handle);
		return;

	case SERVICE_CONTROL_INTERROGATE:
		break;

	default:
		break;
	}

	report_status(state, NO_ERROR, 0, 0);
}


static void parse_switch(const TEXT* switches, int* flag)
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

    const USHORT ret = SetServiceStatus(service_handle, &status);
	if (!ret)
		CNTL_shutdown_service("SetServiceStatus");

	return ret;
}

