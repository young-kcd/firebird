/*
 *	PROGRAM:	JRD Remote Server
 *	MODULE:		winmain.c
 *	DESCRIPTION:	Windows Main server routine for router testing
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
#include <windows.h>
#include <string.h>
#include "../remote/remote.h"
#include "../remote/serve_proto.h"
#include "../jrd/common.h"
#include <winsock2.h>

/* Windows control ids */
#define IDM_START       1
#define IDM_STOP        2
#define IDM_TEXT        3
#define IDM_EXIT        4

/* Default Window Size */
#define APP_HSIZE       200
#define APP_VSIZE       120

#define zap_packet(a) memset ((a), 0, sizeof (struct packet));

/* Function Prototypes */
long FAR PASCAL _export WndProc(HWND, UINT, UINT, LONG);
static int InitApplication(int, HINSTANCE, HINSTANCE);
static BOOL BlockingHook(void);

/* Global Variables */
static char szAppName[] = "IB_Server";
static int isStarted = 0;
static HWND hStcText;

void SRVR_WinMain(
				  PORT main_port,
				  USHORT flags,
				  HINSTANCE hInstance, HINSTANCE hPrevInstance, int nCmdShow)
{
/**************************************
 *
 *      S R V R _ W i n M a i n
 *
 **************************************
 *
 * Functional description
 *      Main entrypoint of server.
 *
 **************************************/
	PACKET send, receive;
	PORT port;

	MSG msg;
	HWND hWnd;
	int txt_pos = 0;
	int step = 5;

	hWnd = InitApplication(nCmdShow, hInstance, hPrevInstance);

	if (!hWnd) {
		MessageBox(NULL,
				   "Unable to initialize main window",
				   szAppName, MB_OK | MB_ICONHAND | MB_APPLMODAL);
		return;
	}

	zap_packet(&receive);
	zap_packet(&send);
	THREAD_ENTER;
	set_server(main_port, flags);

	if (WSASetBlockingHook((FARPROC) BlockingHook)) {
		isStarted = 1;
		SetWindowText(hStcText, "Running");
	}
	else {
		MessageBox(hWnd,
				   "Could NOT setup networking.\r\nTry rebooting to cleanup.",
				   szAppName, MB_OK | MB_ICONHAND | MB_APPLMODAL);
		return;
	}
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				break;
		}
		else if (isStarted) {
			MoveWindow(hStcText, 5, txt_pos, 65, 25, TRUE);
			txt_pos += step;
			if (txt_pos < 0 || txt_pos > 50)
				step = -step;

			/* Note: The following is a clone of the same function
			 * done in server.c:SRVR_main()
			 */

			if (!(port = main_port->receive(&receive)))
				break;
			if (!process_packet(port, &send, &receive, 0))
				break;
		}
	}

	THREAD_EXIT;
}


static int InitApplication(
						   int nCmdShow,
						   HINSTANCE hInstance, HINSTANCE hPrevInstance)
{
/**************************************
 *
 *	I n i t A p p l i c a t i o n
 *
 **************************************
 *
 * Functional description
 *	Do all the setup stuff for a Windows app
 *	and return the handle to the main window.
 *
 **************************************/
	WNDCLASS wndclass;
	HWND hWnd;

	if (!hPrevInstance) {
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc = WndProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInstance;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = szAppName;
		wndclass.lpszClassName = szAppName;

		if (!RegisterClass(&wndclass))
			return FALSE;
	}

	if (!(hWnd = CreateWindow(szAppName, "InterBase Server",
							  WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
							  WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
							  APP_HSIZE, APP_VSIZE, NULL, NULL, hInstance,
							  NULL))) return FALSE;


	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}


static BOOL BlockingHook(void)
{
/**************************************
 *
 *	B l o c k i n g H o o k
 *
 **************************************
 *
 * Functional description
 *	The function to call while cought in a
 *	blocking network call.  This allows us
 *	to yield while waiting for a network call.
 *
 **************************************/
	MSG msg;
	BOOL ret;

	ret = (BOOL) PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	if (ret) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return ret;
}


long FAR PASCAL _export WndProc(
								HWND hWnd,
								UINT message, UINT wParam, LONG lParam)
{
/**************************************
 *
 *	W n d P r o c
 *
 **************************************
 *
 * Functional description
 *	The main event handler.
 *
 **************************************/
	static HWND hBtnStop;
	static HWND hBtnStart;

	switch (message) {
	case WM_CREATE:
		hBtnStart = CreateWindow("button",
								 "&Start",
								 WS_CHILD | WS_VISIBLE | WS_BORDER,
								 APP_HSIZE - 75,
								 10,
								 65,
								 25,
								 hWnd,
								 (HMENU) IDM_START,
								 ((LPCREATESTRUCT) lParam)->hInstance, NULL);
		hBtnStop = CreateWindow("button",
								"S&top",
								WS_CHILD | WS_VISIBLE | WS_BORDER,
								APP_HSIZE - 75,
								45,
								65,
								25,
								hWnd,
								(HMENU) IDM_STOP,
								((LPCREATESTRUCT) lParam)->hInstance, NULL);
		hStcText = CreateWindow("static",
								"Starting",
								WS_CHILD | WS_VISIBLE,
								5,
								5,
								75,
								25,
								hWnd,
								(HMENU) IDM_TEXT,
								((LPCREATESTRUCT) lParam)->hInstance, NULL);
		break;
	case WM_COMMAND:
		switch (wParam) {
		case IDM_EXIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0L);
			return 0;
		case IDM_START:
			SetWindowText(hStcText, "Running");
			isStarted = 1;
			break;
		case IDM_STOP:
			isStarted = 0;
			SetWindowText(hStcText, "Stopped");
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		if (isStarted) {
			MessageBox(hWnd,
					   "Please STOP the service\r\nbefore quitting.",
					   szAppName, MB_OK | MB_ICONHAND | MB_APPLMODAL);
			return 0;
		}
		if (WSAIsBlocking())
			WSACancelBlockingCall();
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
