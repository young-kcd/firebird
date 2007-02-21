/*
 *      PROGRAM:        Interprocess Client
 *      MODULE:         ipcevent.c
 *      DESCRIPTION:    Interprocess interface event window initializer
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

#include "../ipserver/ipcli_proto.h"
#include "../ipserver/ipevt_proto.h"
#include "../ipserver/ips.h"

/* instance handle */

extern HINSTANCE hIBDLLInstance;

/* callback for event handling */

LRESULT CALLBACK IBEventWindowFunc(HWND, UINT, WPARAM, LPARAM);



int IPC_window_init(
					char *class_name,
					char *task_name, HWND * event_window, HANDLE event_sem)
{
/**************************************
 *
 *	I P C _ w i n d o w _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize the window used to process events.
 *
 **************************************/
	HWND hWnd;
	WNDCLASS wcl;
	DWORD result;
	MSG msg;


	wcl.hInstance = hIBDLLInstance;
	wcl.lpszClassName = class_name;
	wcl.lpfnWndProc = IBEventWindowFunc;
	wcl.style = 0;

	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcl.lpszMenuName = NULL;

	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;

	wcl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

	if (!RegisterClass(&wcl)) {
		result = GetLastError();
		return 0;
	}

	hWnd = CreateWindow(class_name,
						task_name,
						WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
						WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
						CW_USEDEFAULT, CW_USEDEFAULT, HWND_DESKTOP, NULL,
						hIBDLLInstance, NULL);

	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);

	/* return handle and signal with semaphore */

	*event_window = hWnd;
	ReleaseSemaphore(event_sem, 1L, NULL);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}


LRESULT CALLBACK IBEventWindowFunc(HWND hWnd,
								   UINT message, WPARAM wParam, LPARAM lParam)
{
	COPYDATASTRUCT *cpdata;


	switch (message) {
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_COPYDATA:
		cpdata = (COPYDATASTRUCT *) lParam;
		IPC_process_event(cpdata->dwData, cpdata->cbData, cpdata->lpData);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
