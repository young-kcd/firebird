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
 */
#include "firebird.h"
#include "../jrd/common.h"
#include <stdio.h>
#include "../jrd/gds_proto.h"
#include <stdlib.h>
#include <windows.h>
#include <shellapi.h>
#include <prsht.h>
#include <commctrl.h>
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#include "../iscguard/iscguard.rh"
#include "../iscguard/iscguard.h"
#include "../iscguard/cntlg_proto.h"
#include "../utilities/install/install_nt.h"
#include "../remote/os/win32/window.h"
#include "../remote/os/win32/chop_proto.h"
#include "../common/config/config.h"

#ifdef WIN_NT
#include <process.h>			/* _beginthread */
#endif

/* Startup Configuration Entry point for regcfg.exe. */
//#define SVC_CONFIG          4
//#define REGCFG_ENTRYPOINT   "LaunchInstReg"
//#define REGCFG_DLL	        "REGCFG.DLL"
typedef void (__cdecl * LPFNREGCFG) (char *, short);

// Define an array of dword pairs,
// where the first of each pair is the control ID,
// and the second is the context ID for a help topic,
// which is used in the help file.
static DWORD aMenuHelpIDs[] = {
	IDC_VERSION, ibs_guard_version,
	IDC_LOG, ibs_guard_log,
	IDC_LOCATION, ibs_server_directory,
	0, 0
};

/* Function prototypes */
static LRESULT CALLBACK WindowFunc(HWND, UINT, WPARAM, LPARAM);
static THREAD_ENTRY_DECLARE WINDOW_main(THREAD_ENTRY_PARAM);
#ifdef NOT_USED_OR_REPLACED
static void StartGuardian(HWND);
#endif
static bool parse_args(LPCSTR, bool*);
#ifdef NOT_USED_OR_REPLACED
static void HelpCmd(HWND, HINSTANCE, WPARAM);
#endif
//char* ChopFileName (char*, char*, long);

THREAD_ENTRY_DECLARE start_and_watch_server(THREAD_ENTRY_PARAM);
THREAD_ENTRY_DECLARE swap_icons(THREAD_ENTRY_PARAM);
void write_log(int, const char*);

HWND DisplayPropSheet(HWND, HINSTANCE);
LRESULT CALLBACK GeneralPage(HWND, UINT, WPARAM, LPARAM);


HINSTANCE hInstance_gbl;
HWND hPSDlg, hWndGbl;
static DWORD ServerPid = 0;
static int nRestarts = 0;		/* the number of times the server was restarted */
bool service_flag = true;
/* unsigned short shutdown_flag = FALSE; */
log_info* log_entry;

/* contains the guardian service */
static const SERVICE_TABLE_ENTRY service_table[] = {
	{const_cast<CHAR*>(ISCGUARD_SERVICE), (LPSERVICE_MAIN_FUNCTION) CNTL_main_thread},
	{NULL, NULL}
};


int WINAPI WinMain(
				   HINSTANCE hInstance,
				   HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
/**************************************
*
*	m a i n
*
**************************************
*
* Functional description
*     The main routine for Windows based server guardian.
*
**************************************/


	OSVERSIONINFO OsVersionInfo;

/* need to set the sizeof this structure for NT to work */
	OsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx((LPOSVERSIONINFO) & OsVersionInfo);

/* service_flag is TRUE for NT false for 95 */
	service_flag = (OsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT);

	if (service_flag == true)
		service_flag = parse_args(lpszCmdLine, &service_flag);

/* set the global HINSTANCE as we need it in WINDOW_main */
	hInstance_gbl = hInstance;

/* allocate space for the event list */
	log_entry =
		reinterpret_cast<log_info*>(malloc(sizeof(log_info)));
	log_entry->next = NULL;

/* since the flag is set we run as a service */
	if (service_flag == true) {
		CNTL_init(WINDOW_main, ISCGUARD_SERVICE);
//
// BRS There is a error in MinGW (3.1.0) headers 
// the parameter of StartServiceCtrlDispatcher is declared const in msvc headers
//
#if defined(MINGW)
		if (!StartServiceCtrlDispatcher(const_cast<SERVICE_TABLE_ENTRY*>(service_table))) {
#else
		if (!StartServiceCtrlDispatcher(service_table)) {
#endif
			if (GetLastError() != ERROR_CALL_NOT_IMPLEMENTED)
				CNTL_shutdown_service("StartServiceCtrlDispatcher failed");
		}
	}
	else {
		return (WINDOW_main(0));
	}

	return (TRUE);

}

static bool parse_args(LPCSTR lpszArgs, bool* pserver_flag)
{
/**************************************
*
*      p a r s e _ a r g s
*
**************************************
*
* Functional description
*      WinMain gives us a stupid command string, not
*      a cool argv.  Parse through the string and
*      set the options.
* Returns
*      A value of TRUE or FALSE depending on if -s is specified.
*      CVC: Service is the default for NT, use -a for application.
*      
*
**************************************/
	char c;
	bool return_value = true;

	for (const char* p = lpszArgs; *p; p++) {
		if (*p++ == '-')
			while (c = *p++) {
				switch (c) {
				case 'a':
				case 'A':
					return_value = false;
					break;

				default:
					return_value = true;
					break;
				}
			}
	}
	return return_value;
}

static THREAD_ENTRY_DECLARE WINDOW_main(THREAD_ENTRY_PARAM)
{
/**************************************
 *
 *	W I N D O W _ m a i n
 *
 **************************************
 *
 * Functional description
 *
 *      This function is where the actual service code starts.
 * Do all the window init stuff, then fork off a thread for starting 
 * the server.
 *
 **************************************/
 
/* Make sure that there is only 1 instance of the guardian running */
	HWND hWnd = FindWindow(GUARDIAN_CLASS_NAME, GUARDIAN_APP_NAME);
	if (hWnd) {
		char szMsgString[256];
		LoadString(hInstance_gbl, IDS_ALREADYSTARTED, szMsgString, 256);
		MessageBox(NULL, szMsgString, GUARDIAN_APP_LABEL, MB_OK | MB_ICONHAND);
		gds__log(szMsgString);
		return 0;
	}

/* initialize main window */
	WNDCLASS wcl;
	wcl.hInstance = hInstance_gbl;
	wcl.lpszClassName = GUARDIAN_CLASS_NAME;
	wcl.lpfnWndProc = WindowFunc;
	wcl.style = 0;
	wcl.hIcon = LoadIcon(hInstance_gbl, MAKEINTRESOURCE(IDI_IBGUARD));
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcl.lpszMenuName = NULL;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

	if (!RegisterClass(&wcl)) {
		char szMsgString[256];
		LoadString(hInstance_gbl, IDS_REGERROR, szMsgString, 256);
		MessageBox(NULL, szMsgString, GUARDIAN_APP_LABEL, MB_OK);
		return 0;
	}

	hWnd = CreateWindowEx(0,
						  GUARDIAN_CLASS_NAME,
						  GUARDIAN_APP_NAME,
						  WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX,
						  CW_USEDEFAULT,
						  CW_USEDEFAULT,
						  CW_USEDEFAULT,
						  CW_USEDEFAULT,
						  HWND_DESKTOP, NULL, hInstance_gbl, NULL);

/* Save the window handle for the thread */
	hWndGbl = hWnd;

/* begin a new thread for calling the start_and_watch_server */
	unsigned long thread_id = 0;
	gds__thread_start(start_and_watch_server, 0, 
			THREAD_medium, 0, &thread_id);

	if (thread_id == (DWORD) -1) {
		/* error starting server thread */
		char szMsgString[256];
		LoadString(hInstance_gbl, IDS_CANT_START_THREAD, szMsgString, 256);
		MessageBox(NULL, szMsgString, GUARDIAN_APP_LABEL, MB_OK);
		gds__log(szMsgString);
		DestroyWindow(hWnd);
		return 0;
	}

	if (service_flag == false) {
		SendMessage(hWnd, WM_COMMAND, IDM_CANCEL, 0);
		UpdateWindow(hWnd);
	}

    MSG message;
	while (GetMessage(&message, NULL, 0, 0)) {
		if (hPSDlg) {			/* If property sheet dialog is open */
			/* Check if the message is property sheet dialog specific */
			BOOL bPSMsg = PropSheet_IsDialogMessage(hPSDlg, &message);

			/* Check if the property sheet dialog is still valid, if not destroy it */
			if (!PropSheet_GetCurrentPageHwnd(hPSDlg)) {
				DestroyWindow(hPSDlg);
				hPSDlg = NULL;
			}
			if (bPSMsg)
				continue;
		}
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	return message.wParam;
}


static LRESULT CALLBACK WindowFunc(
								   HWND hWnd,
								   UINT message, WPARAM wParam, LPARAM lParam)
{
/**************************************
 *
 *	W i n d o w _ F u n c
 *
 **************************************
 *
 * Functional description
 *
 *      This function is where the windowing action takes place.
 * Handle the various messages which come here from GetMessage.
 *
 **************************************/

	static BOOL bInTaskBar = FALSE;
	static bool bStartup = false;
	static HINSTANCE hInstance = NULL;
	static unsigned long thread_id;

	hInstance = (HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE);
	switch (message) {
	case WM_CLOSE:
		{
			/* Clean up memory for log_entry */
			log_info* tmp;
			while (log_entry->next) {
				tmp = log_entry->next;
				free(log_entry);
				log_entry = tmp;
			}
			free(log_entry);
			DestroyWindow(hWnd);
		}
		break;

	case WM_COMMAND:
		switch (wParam) {
		case IDM_CANCEL:
			ShowWindow(hWnd, bInTaskBar ? SW_HIDE : SW_MINIMIZE);
			return TRUE;

		case IDM_OPENPOPUP:
			{
				/* The SetForegroundWindow() has to be called because our window
				   does not become the Foreground one (inspite of clicking on
				   the icon).  This is so because the icon is painted on the task
				   bar and is not the same as a minimized window.
				 */
				SetForegroundWindow(hWnd);

				HMENU hPopup = CreatePopupMenu();
				char szMsgString[256];
				LoadString(hInstance, IDS_SVRPROPERTIES, szMsgString, 256);
				AppendMenu(hPopup, MF_STRING, IDM_SVRPROPERTIES, szMsgString);
				LoadString(hInstance, IDS_SHUTDOWN, szMsgString, 256);
				AppendMenu(hPopup, MF_STRING, IDM_SHUTDOWN, szMsgString);
				LoadString(hInstance, IDS_PROPERTIES, szMsgString, 256);
				AppendMenu(hPopup, MF_STRING, IDM_PROPERTIES, szMsgString);
				SetMenuDefaultItem(hPopup, IDM_PROPERTIES, FALSE);

				POINT curPos;
				GetCursorPos(&curPos);
				TrackPopupMenu(hPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON,
							   curPos.x, curPos.y, 0, hWnd, NULL);
				DestroyMenu(hPopup);
				return TRUE;
			}

		case IDM_SHUTDOWN:
			{
				HWND hTmpWnd = FindWindow(szClassName, szWindowName);
				PostMessage(hTmpWnd, WM_COMMAND, (WPARAM) IDM_SHUTDOWN, 0);
			}
			return TRUE;

		case IDM_PROPERTIES:
			if (!hPSDlg)
				hPSDlg = DisplayPropSheet(hWnd, hInstance);
			else
				SetForegroundWindow(hPSDlg);
			return TRUE;

		case IDM_INTRSVRPROPERTIES:
			return TRUE;

		case IDM_SVRPROPERTIES:
			{
				HWND hTmpWnd = FindWindow(szClassName, szWindowName);
				PostMessage(hTmpWnd, WM_COMMAND, (WPARAM) IDM_PROPERTIES, 0);
			}
			return TRUE;

		case IDM_SET_SERVER_PID:
			ServerPid = (DWORD) lParam;
			return TRUE;
		}
		break;

	case WM_SWITCHICONS:
		nRestarts++;
		gds__thread_start(swap_icons, hWnd, 
				THREAD_medium, 0, &thread_id);
		break;

	case ON_NOTIFYICON:
		if (bStartup) {
			SendMessage(hWnd, WM_COMMAND, 0, 0);
			return TRUE;
		}
		switch (lParam) {
		case WM_LBUTTONDOWN:
			break;

		case WM_LBUTTONDBLCLK:
			PostMessage(hWnd, WM_COMMAND, (WPARAM) IDM_PROPERTIES, 0);
			break;

		case WM_RBUTTONUP:
			PostMessage(hWnd, WM_COMMAND, (WPARAM) IDM_OPENPOPUP, 0);
			break;
		}
		break;

	case WM_CREATE:
		if (service_flag == false) {
			HICON hIcon = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_IBGUARD),
									  IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
									  
			NOTIFYICONDATA nid;
			nid.cbSize = sizeof(NOTIFYICONDATA);
			nid.hWnd = hWnd;
			nid.uID = IDI_IBGUARD;
			nid.uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE;
			nid.uCallbackMessage = ON_NOTIFYICON;
			nid.hIcon = hIcon;
			lstrcpy(nid.szTip, GUARDIAN_APP_LABEL);

			/* This will be true if we are using the explorer interface */
			bInTaskBar = Shell_NotifyIcon(NIM_ADD, &nid);

			if (hIcon)
				DestroyIcon(hIcon);

			/* This will be true if we are using the program manager interface */
			if (!bInTaskBar) {
				char szMsgString[256];
				HMENU hSysMenu = GetSystemMenu(hWnd, FALSE);
				DeleteMenu(hSysMenu, SC_RESTORE, MF_BYCOMMAND);
				AppendMenu(hSysMenu, MF_SEPARATOR, 0, NULL);
				LoadString(hInstance, IDS_SVRPROPERTIES, szMsgString, 256);
				AppendMenu(hSysMenu, MF_STRING, IDM_SVRPROPERTIES,
						   szMsgString);
				LoadString(hInstance, IDS_SHUTDOWN, szMsgString, 256);
				AppendMenu(hSysMenu, MF_STRING, IDM_SHUTDOWN, szMsgString);
				LoadString(hInstance, IDS_PROPERTIES, szMsgString, 256);
				AppendMenu(hSysMenu, MF_STRING, IDM_PROPERTIES, szMsgString);
				DestroyMenu(hSysMenu);
			}
		}
		break;

	case WM_QUERYOPEN:
		if (!bInTaskBar)
			return FALSE;
		return DefWindowProc(hWnd, message, wParam, lParam);

	case WM_SYSCOMMAND:
		if (!bInTaskBar)
			switch (wParam) {
			case SC_RESTORE:
				return TRUE;

			case IDM_SHUTDOWN:
				{
					HWND hTmpWnd = FindWindow(szClassName, szWindowName);
					PostMessage(hTmpWnd, WM_COMMAND, (WPARAM) IDM_SHUTDOWN,
								0);
				}
				return TRUE;

			case IDM_PROPERTIES:
				if (!hPSDlg)
					hPSDlg = DisplayPropSheet(hWnd, hInstance);
				else
					SetFocus(hPSDlg);
				return TRUE;

			case IDM_SVRPROPERTIES:
				{
					HWND hTmpWnd = FindWindow(szClassName, szWindowName);
					PostMessage(hTmpWnd, WM_COMMAND, (WPARAM) IDM_PROPERTIES,
								0);
				}
				return TRUE;
			}
		return DefWindowProc(hWnd, message, wParam, lParam);

	case WM_DESTROY:
		if (bInTaskBar) {
			NOTIFYICONDATA nid;

			nid.cbSize = sizeof(NOTIFYICONDATA);
			nid.hWnd = hWnd;
			nid.uID = IDI_IBGUARD;
			nid.uFlags = 0;
			Shell_NotifyIcon(NIM_DELETE, &nid);
		}
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return FALSE;
}


THREAD_ENTRY_DECLARE start_and_watch_server(THREAD_ENTRY_PARAM)
{
/**************************************
 *
 *	s t a r t _ a n d _ w a t c h _ s e r v e r
 *
 **************************************
 *
 * Functional description
 *
 *      This function is where the server process is created and 
 * the thread waits for this process to exit.
 *
 **************************************/
	Firebird::ContextPoolHolder threadContext(getDefaultMemoryPool());

	const char* server_name = FBSERVER;
	char prog_name[MAXPATHLEN + 128];
	char path[MAXPATHLEN];
	HANDLE procHandle = NULL;
	bool done = true;
	const UINT error_mode = SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX |
		SEM_NOOPENFILEERRORBOX | SEM_NOALIGNMENTFAULTEXCEPT;
	SC_HANDLE hScManager = 0, hService = 0;

/* get the guardian startup information */
	const short option = Config::getGuardianOption();
	gds__prefix(path, "");
	sprintf(prog_name, "\"%s%s%s\" -a -n", path, "bin\\", server_name);
	sprintf(path, "%s%s%s", path, "bin\\", FBSERVER);

/* if the guardian is set to FOREVER then set the error mode */
	UINT old_error_mode = 0;
	if (option == START_FOREVER)
		old_error_mode = SetErrorMode(error_mode);

/* Spawn the new process */
	do {
		SERVICE_STATUS ServiceStatus;
		char out_buf[1024];
		BOOL success;
		int error = 0;

		if (service_flag == true) {
			if (hService) {
				while ((QueryServiceStatus(hService, &ServiceStatus) == TRUE)
					   && (ServiceStatus.dwCurrentState != SERVICE_STOPPED))
				{
					Sleep(500);
				}
			}

			procHandle = CreateMutex(NULL, FALSE, GUARDIAN_MUTEX);

			/* start as a service.  If the service can not be found or
			   fails to start, close the handle to the mutex and set
			   success = FALSE */
			if (!hScManager)
				hScManager = OpenSCManager(NULL, NULL, GENERIC_READ);
			if (!hService)
				hService =
					OpenService(hScManager, REMOTE_SERVICE,
								GENERIC_READ | GENERIC_EXECUTE);
			success = StartService(hService, 0, NULL);
			if (success != TRUE)
				error = GetLastError();
			/* if the server is alrady running, then inform it that it should
			 * open the guardian mutex so that it may be governed. */
			if ((!error) || (error == ERROR_SERVICE_ALREADY_RUNNING)) {
				/* Make sure that it is actually ready to receive commands.
				 * If we were the one who started it, then it will need a few
				 * seconds to get ready. */
				while ((QueryServiceStatus(hService, &ServiceStatus) == TRUE)
					   && (ServiceStatus.dwCurrentState != SERVICE_RUNNING))
					Sleep(500);
				ControlService(hService, SERVICE_CREATE_GUARDIAN_MUTEX,
							   &ServiceStatus);
				success = TRUE;
			}
		}
		else {
			HWND hTmpWnd = FindWindow(szClassName, szWindowName);
			if (hTmpWnd == NULL) {
				STARTUPINFO si;
				SECURITY_ATTRIBUTES sa;
				PROCESS_INFORMATION pi;
				ZeroMemory(&si, sizeof(si));
				si.cb = sizeof(si);
				sa.nLength = sizeof(sa);
				sa.lpSecurityDescriptor = NULL;
				sa.bInheritHandle = TRUE;
				success = CreateProcess(NULL, prog_name, &sa, NULL, FALSE, 0, 
										NULL, NULL, &si, &pi);
				if (success != TRUE)
					error = GetLastError();

				procHandle = pi.hProcess;
				/* TMN: 04 Aug 2000 - closed the handle that previously leaked. */
				CloseHandle(pi.hThread);
			}
			else {
				SendMessage(hTmpWnd, WM_COMMAND, (WPARAM) IDM_GUARDED,
							(LPARAM) hWndGbl);
				if (
					(procHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE,
								 ServerPid)) == NULL) {
					error = GetLastError();
					success = FALSE;
				}
				else
					success = TRUE;
			}
		}

		if (success != TRUE) {
			/* error creating new process */
			char szMsgString[256];
			LoadString(hInstance_gbl, IDS_CANT_START_THREAD, szMsgString, 256);
			sprintf(out_buf, "%s : %s errno : %d", path, szMsgString, error);
			write_log(IDS_CANT_START_THREAD, out_buf);

			if (service_flag == true) {
				SERVICE_STATUS status_info;
				/* wait a second to get the mutex handle (just in case) and
				   then close it
				 */
				WaitForSingleObject(procHandle, 1000);
				CloseHandle(procHandle);
				hService = OpenService(hScManager, REMOTE_SERVICE,
					GENERIC_READ | GENERIC_EXECUTE);
				ControlService(hService, SERVICE_CONTROL_STOP, &status_info);
				CloseServiceHandle(hScManager);
				CloseServiceHandle(hService);
				CNTL_stop_service(ISCGUARD_SERVICE);
			}
			else {
				MessageBox(NULL, out_buf, NULL, MB_OK);
				PostMessage(hWndGbl, WM_CLOSE, 0, 0);
			}
			return 0;
		}
		else {
			char szMsgString[256];
			LoadString(hInstance_gbl, IDS_STARTING_GUARD, szMsgString, 256);
			sprintf(out_buf, "%s: %s\n", szMsgString, path);
			write_log(IDS_LOG_START, out_buf);
		}

		/* wait for process to terminate */
		DWORD exit_status;
		if (service_flag == true) {
			while (WaitForSingleObject(procHandle, 500) == WAIT_OBJECT_0) {
				ReleaseMutex(procHandle);
				Sleep(100);
			}

       		const int ret_val = WaitForSingleObject(procHandle, INFINITE);
			if (ret_val == WAIT_ABANDONED)
				exit_status = CRASHED;
			else if (ret_val == WAIT_OBJECT_0)
				exit_status = NORMAL_EXIT;

			CloseHandle(procHandle);
		}
		else {
			while (WaitForSingleObject(procHandle, INFINITE) == WAIT_FAILED);
			GetExitCodeProcess(procHandle, &exit_status);
			CloseHandle(procHandle);
		}

		if (exit_status != NORMAL_EXIT) {
			/* check for startup error */
			if (exit_status == STARTUP_ERROR) {
				char szMsgString[256];
				LoadString(hInstance_gbl, IDS_STARTUP_ERROR, szMsgString, 256);
				sprintf(out_buf, "%s: %s (%lu)\n", path, szMsgString,
						exit_status);
				write_log(IDS_STARTUP_ERROR, out_buf);
				done = true;

			}
			else {
				char szMsgString[256];
				LoadString(hInstance_gbl, IDS_ABNORMAL_TERM, szMsgString, 256);
				sprintf(out_buf, "%s: %s (%lu)\n", path, szMsgString,
						exit_status);
				write_log(IDS_LOG_TERM, out_buf);

				/* switch the icons if the server restarted */
				if (service_flag == false)
					PostMessage(hWndGbl, WM_SWITCHICONS, 0, 0);
				if (option == START_FOREVER)
					done = false;
			}
		}
		else {
			/* Normal shutdown - ie: via ibmgr - don't restart the server */
			char szMsgString[256];
			LoadString(hInstance_gbl, IDS_NORMAL_TERM, szMsgString, 256);
			sprintf(out_buf, "%s: %s\n", path, szMsgString);
			write_log(IDS_LOG_STOP, out_buf);
			done = true;
		}

		if (option == START_ONCE)
			done = true;
	} while (!done);


/* If on WINNT */
	if (service_flag == true) {
		CloseServiceHandle(hScManager);
		CloseServiceHandle(hService);
		CNTL_stop_service(ISCGUARD_SERVICE);
	}
	else
		PostMessage(hWndGbl, WM_CLOSE, 0, 0);

	return 0;
}


HWND DisplayPropSheet(HWND hParentWnd, HINSTANCE hInst)
{
/******************************************************************************
 *
 *  D i s p l a y P r o p S h e e t
 *
 ******************************************************************************
 *
 *  Input:  hParentWnd - Handle to the main window of this application
 *
 *  Return: Handle to the Property sheet dialog if successful
 *          NULL if error in displaying property sheet
 *
 *  Description: This function initializes the page(s) of the property sheet,
 *               and then calls the PropertySheet() function to display it.
 *****************************************************************************/
	PROPSHEETHEADER PSHdr;
	PROPSHEETPAGE PSPages[1];
	HINSTANCE hInstance = hInst;

	PSPages[0].dwSize = sizeof(PROPSHEETPAGE);
	PSPages[0].dwFlags = PSP_USETITLE;
	PSPages[0].hInstance = hInstance;
	PSPages[0].pszTemplate = MAKEINTRESOURCE(IDD_PROPSHEET);
	PSPages[0].pszTitle = MAKEINTRESOURCE(IDS_PROP_TITLE);
	PSPages[0].pfnDlgProc = (DLGPROC) GeneralPage;
	PSPages[0].pfnCallback = NULL;

	PSHdr.dwSize = sizeof(PROPSHEETHEADER);
	PSHdr.dwFlags = PSH_PROPTITLE | PSH_PROPSHEETPAGE |
		PSH_USEICONID | PSH_MODELESS;
	PSHdr.hwndParent = hParentWnd;
	PSHdr.hInstance = hInstance;
	PSHdr.pszIcon = MAKEINTRESOURCE(IDI_IBGUARD);
	PSHdr.pszCaption = (LPSTR) GUARDIAN_APP_LABEL;
	PSHdr.nPages = FB_NELEM(PSPages);
	PSHdr.nStartPage = 0;
	PSHdr.ppsp = (LPCPROPSHEETPAGE) & PSPages;
	PSHdr.pfnCallback = NULL;

	hPSDlg = (HWND) PropertySheet(&PSHdr);

	if (hPSDlg <= 0)
		hPSDlg = NULL;
	return hPSDlg;
}


LRESULT CALLBACK GeneralPage(HWND hDlg, UINT unMsg, WPARAM wParam,
							 LPARAM lParam)
{
/******************************************************************************
 *
 *  G e n e r a l P a g e
 *
 ******************************************************************************
 *
 *  Input:  hDlg - Handle to the page dialog
 *          unMsg - Message ID
 *          wParam - WPARAM message parameter
 *          lParam - LPARAM message parameter
 *
 *  Return: FALSE if message is not processed
 *          TRUE if message is processed here
 *
 *  Description: This is the window procedure for the "General" page dialog
 *               of the property sheet dialog box. All the Property Sheet
 *               related events are passed as WM_NOTIFY messages and they
 *               are identified within the LPARAM which will be pointer to 
 *               the NMDR structure
 *****************************************************************************/
	HINSTANCE hInstance;
	hInstance = (HINSTANCE) GetWindowLong(hDlg, GWL_HINSTANCE);

	switch (unMsg) {
	case WM_INITDIALOG:
		{
			char szText[256];
			char szWindowText[256];
			char szFullPath[256];
			int index = 0;
			const int NCOLS = 3;

			/* Display the number of times the server has been started by
			   this session of the guardian
			 */
			SetDlgItemInt(hDlg, IDC_RESTARTS, nRestarts, FALSE);

			/* get the path to the exe.  
			   Make sure that it is null terminated */
			GetModuleFileName(hInstance, szWindowText, 256);
			char* pszPtr = strrchr(szWindowText, '\\');
			*(pszPtr + 1) = 0x00;

			ChopFileName(szWindowText, szWindowText, 38);
			SetDlgItemText(hDlg, IDC_LOCATION, szWindowText);

			/* Get version information from the application */
			GetModuleFileName(hInstance, szFullPath, sizeof(szFullPath));
			DWORD dwVerHnd;
			const DWORD dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &dwVerHnd);
			if (dwVerInfoSize) {
				/* If we were able to get the information, process it: */
				UINT cchVer = 25;
				LPSTR lszVer = NULL;

				HANDLE hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
				LPVOID lpvMem = GlobalLock(hMem);
				GetFileVersionInfo(szFullPath, dwVerHnd, dwVerInfoSize,
								   lpvMem);
				if (VerQueryValue
					(lpvMem, "\\StringFileInfo\\040904E4\\FileVersion",
					 reinterpret_cast < void **>(&lszVer), &cchVer))
					  SetDlgItemText(hDlg, IDC_VERSION, lszVer);
				else
					SetDlgItemText(hDlg, IDC_VERSION, "N/A");
				GlobalUnlock(hMem);
				GlobalFree(hMem);
			}

			/* Create the columns Action, Date, Time for the listbox */
			HWND hWndLog = GetDlgItem(hDlg, IDC_LOG);
			LV_COLUMN lvC;
			lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			lvC.fmt = LVCFMT_LEFT;	// left-align column
			lvC.pszText = szText;

			for (index = 0; index < NCOLS; index++) {
				/* NOTE:  IDS_ACTION = 220
				   IDS_DATE   = 230
				   IDS_TIME   = 240
				 */
				lvC.iSubItem = index;
				lvC.cx = 85;
				LoadString(hInstance, IDS_ACTION + (index * 10), szText,
						   sizeof(szText));
				ListView_InsertColumn(hWndLog, index, &lvC);
			}

            log_info* liTemp = log_entry->next;
            LV_ITEM lvI;
			lvI.cchTextMax = sizeof(liTemp->log_action);
			lvI.mask = LVIF_TEXT;
			for (index = 0; liTemp->log_action;
				 index++, liTemp = liTemp->next)
			{
				lvI.iItem = index;
				lvI.iSubItem = 0;
				lvI.pszText = liTemp->log_action;
				ListView_InsertItem(hWndLog, &lvI);
				ListView_SetItemText(hWndLog, index, 0, lvI.pszText);

				lvI.iSubItem = 1;
				lvI.pszText = liTemp->log_date;
				ListView_InsertItem(hWndLog, &lvI);
				ListView_SetItemText(hWndLog, index, 1, lvI.pszText);

				lvI.iSubItem = 2;
				lvI.pszText = liTemp->log_time;
				ListView_InsertItem(hWndLog, &lvI);
				ListView_SetItemText(hWndLog, index, 2, lvI.pszText);
			}
		}
		break;
	case WM_HELP:
		{
			LPHELPINFO lphi = (LPHELPINFO) lParam;
			if (lphi->iContextType == HELPINFO_WINDOW)	// must be for a control
				WinHelp((HWND) lphi->hItemHandle, GUARDIAN_HELP_FILE,
						HELP_WM_HELP, (DWORD) (LPVOID) aMenuHelpIDs);
			return TRUE;
		}

	case WM_CONTEXTMENU:
		WinHelp((HWND) wParam, GUARDIAN_HELP_FILE, HELP_CONTEXTMENU,
				(DWORD) (LPVOID) aMenuHelpIDs);
		return TRUE;
		break;
	case WM_NOTIFY:
		switch (((LPNMHDR) lParam)->code) {
		case PSN_KILLACTIVE:
			SetWindowLong(hDlg, DWL_MSGRESULT, FALSE);
			break;
		case PSN_HELP:
#ifdef NOT_USED_OR_REPLACED
			/*          HelpCmd(hDlg, hInstance, ibsp_Server_Information_Properties); */
#endif
			break;
		}
		break;
	}
	return FALSE;
}


THREAD_ENTRY_DECLARE  swap_icons(THREAD_ENTRY_PARAM param)
{
/******************************************************************************
 *
 *  S w a p I c o n s
 *
 ******************************************************************************
 *
 *  Description:  Animates the icon if the server restarted
 *****************************************************************************/
	Firebird::ContextPoolHolder threadContext(getDefaultMemoryPool());

	HWND hWnd = reinterpret_cast<HWND>(param);
	HINSTANCE hInstance = (HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE);
	HICON hIconNormal = (HICON) LoadImage(hInstance,
									MAKEINTRESOURCE(IDI_IBGUARD),
									IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);

	HICON hIconAlert = (HICON) LoadImage(hInstance,
								   MAKEINTRESOURCE(IDI_IBGUARDALRT),
								   IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);

	NOTIFYICONDATA nidNormal;
	nidNormal.cbSize = sizeof(NOTIFYICONDATA);
	nidNormal.hWnd = hWnd;
	nidNormal.uID = IDI_IBGUARD;
	nidNormal.uFlags = NIF_ICON;
	nidNormal.hIcon = hIconNormal;

	NOTIFYICONDATA nidAlert;
	nidAlert.cbSize = sizeof(NOTIFYICONDATA);
	nidAlert.hWnd = hWnd;
	nidAlert.uID = IDI_IBGUARD;
	nidAlert.uFlags = NIF_ICON;
	nidAlert.hIcon = hIconAlert;

/* Animate until the property sheet is displayed.  Once the property sheet is
displayed, stop animating the icon
*/
	while (!hPSDlg) {
		if (!Shell_NotifyIcon(NIM_MODIFY, &nidAlert))
			SetClassLong(hWnd, GCL_HICON, (long) hIconAlert);
		Sleep(500);

		if (!Shell_NotifyIcon(NIM_MODIFY, &nidNormal))
			SetClassLong(hWnd, GCL_HICON, (long) hIconNormal);
		Sleep(500);
	}
/* Make sure that the icon is normal */
	if (!Shell_NotifyIcon(NIM_MODIFY, &nidNormal))
		SetClassLong(hWnd, GCL_HICON, (long) hIconNormal);

	if (hIconNormal)
		DestroyIcon(hIconNormal);
	if (hIconAlert)
		DestroyIcon(hIconAlert);

	return 0;
}


void write_log(int log_action, const char* buff)
{
/******************************************************************************
 *
 *  W r i t e L o g
 *
 ******************************************************************************
 *
 *  Description:  Writes the guardian information to either the Windows 95
 *                property sheet structure (log_entry) or to the Windows NT
 *                Event Log
 *****************************************************************************/
	char tmp_buff[128];

	time_t ltime;
	time(&ltime);
	const tm* today = localtime(&ltime);

/* Move to the end of the log_entry list */
	log_info* log_temp = log_entry;
	while (log_temp->next)
		log_temp = log_temp->next;

	log_info* tmp =
		reinterpret_cast<log_info*>(malloc(sizeof(log_info)));
	memset(tmp, 0, sizeof(log_info));

#ifdef NOT_USED_OR_REPLACED
	sprintf(tmp->log_time, "%02d:%02d", today->tm_hour, today->tm_min);
	sprintf(tmp->log_date, "%02d/%02d/%02d",
			today->tm_mon + 1, today->tm_mday, today->tm_year % 100);
#else
/* TMN: Fixed this after bug-report. Should it really force */
/* 24hr format in e.g US, where they use AM/PM wharts?      */
	GetTimeFormat(LOCALE_USER_DEFAULT,
				  TIME_NOSECONDS | TIME_FORCE24HOURFORMAT, NULL, NULL,
				  tmp->log_time, sizeof(tmp->log_time));
	GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, NULL, NULL,
				  tmp->log_date, sizeof(tmp->log_date));
#endif

	if (log_action >= IDS_LOG_START && log_action <= IDS_LOG_TERM) {
		/* Only Windows 95 needs this information since it goes in the property sheet */
		LoadString(hInstance_gbl, log_action, tmp_buff,
				   sizeof(tmp->log_action));
		sprintf(tmp->log_action, "%s", tmp_buff);
		tmp->next = NULL;
		log_temp->next = tmp;
	}

	if (service_flag == true) {	/* on NT */
		HANDLE hLog = RegisterEventSource(NULL, ISCGUARD_SERVICE);
		if (!hLog)
			gds__log("Error opening Windows NT Event Log");
		else {
			char* act_buff[1]; // CVC: Where is this deallocated?
			act_buff[0] = (char*) malloc(sizeof(tmp_buff));
			LoadString(hInstance_gbl, log_action + 1, tmp_buff,
					   sizeof(tmp_buff));
			sprintf(act_buff[0], "%s", buff);
			
			LPVOID lpMsgBuf;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
						  FORMAT_MESSAGE_ARGUMENT_ARRAY |
						  FORMAT_MESSAGE_FROM_STRING,
						  tmp_buff, 0, 0, (LPTSTR) &lpMsgBuf, 0,
						  reinterpret_cast<va_list*>(act_buff));
			strncpy(act_buff[0], (LPTSTR) lpMsgBuf,
					strlen(reinterpret_cast<const char*>(lpMsgBuf)) - 1);
			LocalFree(lpMsgBuf);
			WORD wLogType;
			
			switch (log_action) {
			case IDS_LOG_START:
			case IDS_LOG_STOP:
				wLogType = EVENTLOG_INFORMATION_TYPE;
				break;
			default:
				wLogType = EVENTLOG_ERROR_TYPE;
			}
			
			if (!ReportEvent
				(hLog, wLogType, 0, log_action + 1, NULL, 1, 0,
				 const_cast<const char**>(act_buff), NULL))
			{
				FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
							  FORMAT_MESSAGE_FROM_SYSTEM |
							  FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
							  GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	// Default language
							  (LPTSTR) & lpMsgBuf, 0, NULL);
				gds__log("Unable to update NT Event Log.\n\tOS Message: %s",
						 lpMsgBuf);
				LocalFree(lpMsgBuf);
			}
			// CVC: free(act_buff[0]); ???
			DeregisterEventSource(hLog);
		}
	}

/* Write to the Firebird log */
	if (*buff)
		gds__log(buff);
}


#ifdef NOT_USED_OR_REPLACED
void HelpCmd(HWND hWnd, HINSTANCE hInst, WPARAM wId)
{
/****************************************************************
 *                                              
 *  H e l p C m d
 *
 ****************************************************************
 *
 *  Input:     hWnd     - Handle of dialog box from which help was
 *                        invoked.
 *              hInst   - Instance of application.
 *              nId       - The help message Id.
 *
 *  Description:  Invoke the Windows Help facility with context of nId.
 *      
 *****************************************************************/
	HCURSOR hOldCursor;
	char szPathFileName[1024 + 256 + 1];

	GetModuleFileName(hInst, szPathFileName, sizeof(szPathFileName));

/* Show hour glass cursor */
	hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

	strcpy(strrchr(szPathFileName, '\\') + 1, "fbserver.hlp");
	WinHelp(hWnd, szPathFileName, HELP_CONTEXT, wId);

/* Restore old cursor */
	SetCursor(hOldCursor);

	return;
}
#endif

