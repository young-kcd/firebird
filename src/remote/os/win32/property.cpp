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
 *
 * 2002.10.29 Sean Leyne - Removed support for obsolete IPX/SPX Protocol
 *
*/


/*
 *      PROGRAM:        IB Server
 *      MODULE:         property.cpp
 *      DESCRIPTION:    Property sheet implementation for WIN32 server
 *
 */

#include "firebird.h"
#include <windows.h>
#include <shellapi.h>
#include <prsht.h>
#include <dbt.h>

/* Since it's a Win32-only file, we might as well assert it*/
#if !defined(WIN_NT)
#error This is a Win32 only file.
#endif

#include "../jrd/common.h"
#include "../jrd/license.h"
#include "../jrd/isc.h"
#include "../jrd/file_params.h"
#include "../remote/remote_def.h"
#include "../remote/os/win32/window.rh"
#include "../remote/os/win32/property.rh"

#include "../jrd/svc_proto.h"
#include "../remote/os/win32/window_proto.h"
#include "../remote/os/win32/propty_proto.h"
#include "../remote/os/win32/ibconfig.h"

#include "../jrd/ibase.h"

#include "../remote/os/win32/ibsvrhlp.h"
#include "../remote/os/win32/chop_proto.h"

#include "../jrd/thd.h"			/* get jrd_proto.h to declare the function */
#include "../jrd/jrd_proto.h"	/* JRD_num_attachments() */
#include <stdio.h>				/* sprintf() */

const int WIN_TEXTLEN			= 128;
const int MSG_STRINGLEN			= 64;
static const char* APP_LABEL			= "Firebird Server";

static HINSTANCE hInstance = NULL;	// Handle to the current app. instance
static HWND hPSDlg = NULL;		// Handle to the parent prop. sheet window
HBRUSH hGrayBrush = NULL;		// Handle to a Gray Brush
static USHORT usServerFlags;	// Server Flag Mask

// Define an array of dword pairs,
// where the first of each pair is the control ID,
// and the second is the context ID for a help topic,
// which is used in the help file.
static const DWORD aMenuHelpIDs[] = {
	IDC_IBSVR_ICON, ibs_server_icon,
	IDC_PROTOCOLS, ibs_capabilities,
	IDC_CAPABILITIES_TEXT, ibs_capabilities,
	IDC_PRODNAME, ibs_prodname,
	IDC_PATH, ibs_image_path,
	IDC_LOCATION_TEXT, ibs_image_path,
	IDC_STAT1, ibs_version,
	IDC_VERSION_TEXT, ibs_version,
	IDC_STAT2, ibs_num_dbs_attached,
	IDC_NUM_ATTACH_TEXT, ibs_num_dbs_attached,
	IDC_STAT3, ibs_num_dbs,
	IDC_NUM_DB_TEXT, ibs_num_dbs,
	IDC_REFRESH, ibs_refresh,
	0, 0
};

// Window procedures
LRESULT APIENTRY GeneralPage(HWND, UINT, WPARAM, LPARAM);

// Static functions to be called from this file only.
static char *MakeVersionString(char *, int, USHORT);
static void RefreshUserCount(HWND);

HWND DisplayProperties(HWND hParentWnd,
					   HINSTANCE hInst, USHORT usServerFlagMask)
{
/******************************************************************************
 *
 *  D i s p l a y P r o p e r t i e s
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
	hInstance = hInst;
	usServerFlags = usServerFlagMask;

	PROPSHEETPAGE PSPages[1];
	PSPages[0].dwSize = sizeof(PROPSHEETPAGE);
	PSPages[0].dwFlags = PSP_USETITLE | PSP_HASHELP;
	PSPages[0].hInstance = hInstance;
	PSPages[0].pszTemplate = MAKEINTRESOURCE(GENERAL_DLG);
	PSPages[0].pszTitle = "General";
	PSPages[0].pfnDlgProc = (DLGPROC) GeneralPage;
	PSPages[0].pfnCallback = NULL;

	PROPSHEETHEADER PSHdr;
	PSHdr.dwSize = sizeof(PROPSHEETHEADER);
	PSHdr.dwFlags = PSH_PROPTITLE | PSH_PROPSHEETPAGE |
		PSH_USEICONID | PSH_MODELESS;
	PSHdr.hwndParent = hParentWnd;
	PSHdr.hInstance = hInstance;
	PSHdr.pszIcon = MAKEINTRESOURCE(IDI_IBSVR);
	PSHdr.pszCaption = (LPSTR) APP_LABEL;
	PSHdr.nPages = FB_NELEM(PSPages);
	PSHdr.nStartPage = 0;
	PSHdr.ppsp = (LPCPROPSHEETPAGE) & PSPages;
	PSHdr.pfnCallback = NULL;

// Initialize the gray brush to paint the background
// for all prop. sheet pages and their controls
	hGrayBrush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));

	hPSDlg = (HWND) PropertySheet(&PSHdr);

	if (hPSDlg <= 0)
		hPSDlg = NULL;
	else
		// Add the Configuration pages
		AddConfigPages(hPSDlg, hInstance);

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

	switch (unMsg) {
	case WM_INITDIALOG:
		{
			char szText[MSG_STRINGLEN];
			lstrcpy(szText, GDS_VERSION);
			SetDlgItemText(hDlg, IDC_STAT1, szText);

			LoadString(hInstance, IDS_UNLIMITED_USERS, szText, MSG_STRINGLEN);

			SetDlgItemText(hDlg, IDC_LICENSE, szText);

			if (usServerFlags & (SRVR_inet | SRVR_wnet))
				LoadString(hInstance,
						   IDS_SERVERPROD_NAME, szText, MSG_STRINGLEN);
			else
				LoadString(hInstance,
						   IDS_LOCALPROD_NAME, szText, MSG_STRINGLEN);

			SetDlgItemText(hDlg, IDC_PRODNAME, szText);

			char szWindowText[WIN_TEXTLEN];
			MakeVersionString(szWindowText, WIN_TEXTLEN, usServerFlags);
			SetDlgItemText(hDlg, IDC_PROTOCOLS, szWindowText);

			GetModuleFileName(hInstance, szWindowText, WIN_TEXTLEN);
			char* pszPtr = strrchr(szWindowText, '\\');
			*(pszPtr + 1) = 0x00;

			ChopFileName(szWindowText, szWindowText, 38);
			SetDlgItemText(hDlg, IDC_PATH, szWindowText);

			RefreshUserCount(hDlg);
		}
		break;
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLORSCROLLBAR:
	case WM_CTLCOLORBTN:
		{
			OSVERSIONINFO OsVersionInfo;

			OsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			if (GetVersionEx((LPOSVERSIONINFO) & OsVersionInfo) &&
				OsVersionInfo.dwMajorVersion < 4) {
				SetBkMode((HDC) wParam, TRANSPARENT);
				return (LRESULT) hGrayBrush;
			}
		}
		break;
	case WM_HELP:
		{
			LPHELPINFO lphi = (LPHELPINFO) lParam;
			if (lphi->iContextType == HELPINFO_WINDOW)	// must be for a control
			{
				WinHelp(static_cast<HWND>(lphi->hItemHandle),
						"IBSERVER.HLP",
						HELP_WM_HELP, (DWORD) (LPVOID) aMenuHelpIDs);
			}
		}
		return TRUE;
	case WM_CONTEXTMENU:
		{
			WinHelp((HWND) wParam,
					"IBSERVER.HLP",
					HELP_CONTEXTMENU, (DWORD) (LPVOID) aMenuHelpIDs);
		}
		return TRUE;
	case WM_COMMAND:
		switch (wParam) {
		case IDC_REFRESH:
			RefreshUserCount(hDlg);
			break;
		}
		break;
	case WM_NOTIFY:
		switch (((LPNMHDR) lParam)->code) {
		case PSN_KILLACTIVE:
			SetWindowLong(hDlg, DWL_MSGRESULT, FALSE);
			break;
		case PSN_HELP:
			HelpCmd(hDlg, hInstance, ibsp_Server_Information_Properties);
			break;
		}
		break;
	}
	return FALSE;
}

static char *MakeVersionString(char *pchBuf, int nLen,
							   USHORT usServerFlagMask)
{
/******************************************************************************
 *
 *  M a k e V e r s i o n S t r i n g
 *
 ******************************************************************************
 *
 *  Input:  pchBuf - Buffer to be filled into
 *          nLen - Length of the buffer
 *          usServerFlagMask - Bit flag mask encoding the server flags
 *
 *  Return: Buffer containing the version string
 *
 *  Description: This method is called to get the Version String. This string
 *               is based on the flag set in usServerFlagMask.
 *****************************************************************************/
	char* p = pchBuf;
	const char* const end = p + nLen;

	if (usServerFlagMask & SRVR_inet) {
		p += LoadString(hInstance, IDS_TCP, p, end - p);
		if (p < end)
			*p++ = '\r';
		if (p < end)
			*p++ = '\n';
	}
	if (usServerFlagMask & SRVR_wnet) {
		p += LoadString(hInstance, IDS_NP, p, end - p);
		if (p < end)
			*p++ = '\r';
		if (p < end)
			*p++ = '\n';
	}
	if (usServerFlagMask & (SRVR_xnet | SRVR_ipc)) {
		p += LoadString(hInstance, IDS_IPC, p, end - p);
	}
	*p = '\0';
	return pchBuf;
}

static void RefreshUserCount(HWND hDlg)
{
/******************************************************************************
 *
 *  R e f r e s h U s e r C o u n t
 *
 ******************************************************************************
 *
 *  Input:  hDlg - Handle to the General page dialog
 *
 *  Return: void
 *
 *  Description: This method calls the JRD_num_attachments() function to get
 *               the number of active attachments to the server.
 *****************************************************************************/
	USHORT num_att = 0;
	USHORT num_dbs = 0;
	HCURSOR hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

	JRD_num_attachments(NULL, 0, 0, &num_att, &num_dbs);

	char szText[MSG_STRINGLEN];
	sprintf(szText, "%d", num_att);
	SetDlgItemText(hDlg, IDC_STAT2, szText);
	sprintf(szText, "%d", num_dbs);
	SetDlgItemText(hDlg, IDC_STAT3, szText);

	SetCursor(hOldCursor);
}

