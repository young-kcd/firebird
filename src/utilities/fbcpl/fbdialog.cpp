/*
 *	PROGRAM:	Firebird 1.5 control panel applet
 *	MODULE:		fbdialog.cpp
 *	DESCRIPTION:	Main file to provide GUI based server control functions
 *					for Firebird 1.5 Super Server
 *
 * The contents of this file are subject to the Independant Developer's 
 * Public License Version 1.0 (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy
 * of the License at http://www.ibphoenix.com/idpl.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Paul Reeves
 * Copyright (C) 2003 Paul Reeves
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 *
 *
 *	History:
 *		This current version is derived from the Fb 1.0 control panel applet
 *		It was adapted to support management of a dual Super Server and
 *		Classic Server install, allowing easy switching between the two server 
 *		types. Unfortunately it became obvious too late in the development 
 *		cycle that such support was not feasible without delaying release 
 *		further. Consequently the applet has been converted back to manage 
 *		Super Server only. The relevant code for Classic has been ifdef'ed out.
 *
 *
 */


#include "stdafx.h"
#include "FBDialog.h"

#include "../../common/config/config.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFBDialog 


CFBDialog::CFBDialog(CWnd* pParent /*=NULL*/)
	:	CDialog(CFBDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFBDialog)
	m_FB_Version = _T("");
	m_Firebird_Status = _T("");
	//}}AFX_DATA_INIT

	m_uiTimer = 0;
    hScManager = 0;
	initialised = false;

	m_Guardian_Name		= ISCGUARD_EXECUTABLE;
	m_SS_Server_Name	= REMOTE_SS_EXECUTABLE; 
#ifdef MANAGE_CLASSIC
	m_CS_Server_Name	= REMOTE_CS_EXECUTABLE; 
#endif

	fb_status.AutoStart = 0;
	fb_status.ServicesAvailable = 0;
	fb_status.ServerStatus = 0;
	fb_status.UseGuardian = 0;
	fb_status.UseService = 0;
	fb_status.WasRunning  = 0;
#ifdef MANAGE_CLASSIC
	fb_status.UseClassic = 0;
#endif
	fb_status.SystemLogin = 1;
	fb_status.ServerName = "";

	new_settings = fb_status;
}


void CFBDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFBDialog)
	DDX_Control(pDX, IDC_SERVER_ARCH, m_Server_Arch);
	DDX_Control(pDX, IDAPPLY, m_Apply);
	DDX_Control(pDX, IDC_USE_GUARDIAN, m_Use_Guardian);
	DDX_Control(pDX, IDC_MANUAL_START, m_Manual_Start);
	DDX_Control(pDX, IDC_APPLICATION, m_Run_As_Application);
	DDX_Control(pDX, IDC_SERVICE, m_Run_As_Service);
	DDX_Control(pDX, IDC_AUTO_START, m_Auto_Start);
	DDX_Control(pDX, IDC_RUN_TYPE, m_Run_Type);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_Button_Stop);
	DDX_Control(pDX, IDC_STATUS_ICON, m_Icon);
	DDX_Text(pDX, IDC_FB_VERSION, m_FB_Version);
	DDV_MaxChars(pDX, m_FB_Version, 128);
	DDX_Text(pDX, IDC_FIREBIRD_STATUS, m_Firebird_Status);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFBDialog, CDialog)
	//{{AFX_MSG_MAP(CFBDialog)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SERVICE, OnService)
	ON_BN_CLICKED(IDC_MANUAL_START, OnManualStart)
	ON_BN_CLICKED(IDC_APPLICATION, OnApplication)
	ON_BN_CLICKED(IDC_AUTO_START, OnAutoStart)
	ON_BN_CLICKED(IDC_USE_GUARDIAN, OnUseGuardian)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	ON_BN_CLICKED(IDC_CLASSIC_SERVER, OnClassicServer)
	ON_BN_CLICKED(IDC_SUPER_SERVER, OnSuperServer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFBDialog message handlers


BOOL CFBDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Reset_Display_To_Existing_Values = TRUE;

	fb_status.ServicesAvailable = ServiceSupportAvailable();
	if ( fb_status.ServicesAvailable )
	{
		m_Run_Type.EnableWindow(TRUE);
		m_Run_As_Service.EnableWindow(TRUE);
		m_Run_As_Application.EnableWindow(TRUE);
	}
	else
	{
		m_Run_As_Service.EnableWindow(FALSE);
	}

	m_uiTimer = SetTimer(1, 500, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CFBDialog::UpdateServerStatus() 
// This is one of the key functions. It is called
// from the timer and sets the display.
// It is in three sections
//	a) Evaluate current status of the Firebird install
//	b) Update internal variables
//	c) Update display
{
	fb_status.WasRunning = FirebirdRunning();

	// Lets see if the guardian or a classic server are 
	// currently running. We need to know about the guardian
	// before we call GetServerStatus()
	if (fb_status.WasRunning)
	{ 
#ifdef MANAGE_CLASSIC
		fb_status.UseClassic = (bool) GetClassicServerHandle();
#endif
		fb_status.UseGuardian = (bool) GetGuardianHandle();
	}
	else
	{
#ifdef MANAGE_CLASSIC
		fb_status.UseClassic = (bool) GetPreferredArchitecture();
#endif
		fb_status.UseGuardian = (bool) GetGuardianUseSpecified();
	}

	fb_status.ServerStatus = GetServerStatus();


	ViewRegistryEntries();


	// ViewRegistryEntries will update some status variables with 
	// values from the registry. However, this may not reflect 
	// reality, so lets check what is actually running again.
	if (fb_status.WasRunning)
	{ 
#ifdef MANAGE_CLASSIC
		fb_status.UseClassic = (bool) GetClassicServerHandle();
#endif
		fb_status.UseGuardian = (bool) GetGuardianHandle();
	}


//========Update other internal variables ==============

	m_Firebird_Status.Format(fb_status.ServerStatus);

#ifdef MANAGE_CLASSIC
	if (fb_status.UseClassic) 
	{
		fb_status.ServiceExecutable = m_CS_Server_Name; 
	}
	else
#endif
	{
		fb_status.ServiceExecutable = m_SS_Server_Name; 
	}

//========Start of code that updates GUI================
	if ( fb_status.WasRunning )
	{
		m_Icon.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON1)); 
		m_Button_Stop.SetWindowText("&Stop");
	}
	else
	{
		m_Icon.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON4));
		m_Button_Stop.SetWindowText("&Start");

	}

	//Reset check boxes 
	if (m_Reset_Display_To_Existing_Values)
	{
		// This is always called on startup. This method 
		// is also called if an attempt to install
		// the service fails.
		ResetCheckBoxes( fb_status );
		if ( m_Apply.IsWindowEnabled() )
			ApplyChanges();
	}

	ResetWarningBox( fb_status );

	m_Reset_Display_To_Existing_Values = false;
	
	UpdateData(false);		

	// This will be false the first time round.
	// It is needed because the Config class doesn't 
	// refresh after the conf file has been updated.
	initialised = true;

	// The only time new_settings differs from fb_status is when 
	// the timer is turned off by ApplyChanges().
	new_settings = fb_status;
}


int CFBDialog::GetServerStatus()
// This is called by UpdateServerStatus, 
// which is called on the timer, so our status 
// should always be 'up-to-date'.
// Requires:
//		fb_status.UseGuardian
// Returns:
//		fb_status.ServerStatus

{
	int result = IDS_APPLICATION_STOPPED;
	
	CString service;
	if (fb_status.UseGuardian)
	{
		service = ISCGUARD_SERVICE;
	}
	else
	{
		service = REMOTE_SERVICE;
	}
	
	SC_HANDLE hService = NULL;
	if (fb_status.ServicesAvailable) {
		OpenServiceManager();
		hService = OpenService (hScManager, service, GENERIC_READ );
		if (hService == NULL)
		{
			if (GetLastError() != ERROR_SERVICE_DOES_NOT_EXIST)
			{
				ShowError(0, "GetServiceStatus");
			}
		}
	}

	CloseServiceManager();

	fb_status.UseService = hService;

	if ( fb_status.UseService )
	{

		QueryServiceStatus( hService, &service_status ); 
		CloseServiceHandle ( hService );
		switch ( service_status.dwCurrentState )
		{
			case SERVICE_STOPPED :
			{
				if ( FirebirdRunning() )
					result = IDS_APPLICATION_RUNNING;
				else
					result = IDS_SERVICE_STOPPED;
				break;
			}
			case SERVICE_START_PENDING :
			{
				result = IDS_SERVICE_START_PENDING;
				break;
			}
			case SERVICE_STOP_PENDING : 
			{
				result = IDS_SERVICE_STOP_PENDING;
				break;
			}
			case SERVICE_RUNNING : 
			{
				result = IDS_SERVICE_RUNNING;
				break;
			}
			case SERVICE_CONTINUE_PENDING :
			{
				result = IDS_SERVICE_CONTINUE_PENDING;
				break;
			}
			case SERVICE_PAUSE_PENDING : 
			{
				result = IDS_SERVICE_PAUSE_PENDING;
				break;
			}
			case SERVICE_PAUSED :
			{
				result = IDS_SERVICE_PAUSED;
				break;
			}
			default :
			{
				result = IDS_APPLICATION_STOPPED;
				break;
			}
		}
	}
	else
	{
		//Firebird might still be running so...
		if ( FirebirdRunning() ) 
			result = IDS_APPLICATION_RUNNING;
	}
	return result;
}


void CFBDialog::ViewRegistryEntries()
// Find out what we have in the non-Firebird section of the 
// registry ie, in Services or Application Run
// The following variables must have been set:
//	fb_status.UseService - set on startup
//	fb_status.UseGuardian - set by calling method

// The following variables will be set on return:
//	fb_status.AutoStart
//	fb_status.SystemLogin
//	fb_status.UseClassic
//	fb_status.ServerName
{
	
	fb_status.AutoStart = 0;

	if ( fb_status.UseService )
	{
	    LPQUERY_SERVICE_CONFIG status_info;
		SC_HANDLE hService = 0;
		DWORD dwBytesNeeded; 
		OpenServiceManager();

		char * service = "";
		char * display_name = "";
		if (fb_status.UseGuardian)
		{
			service = ISCGUARD_SERVICE;
			display_name = ISCGUARD_DISPLAY_NAME;
		}
		else
		{
			service = REMOTE_SERVICE;
			display_name = REMOTE_DISPLAY_NAME;
		}

		hService = OpenService (hScManager, service, SERVICE_QUERY_CONFIG);
		if (hService != NULL) // then we are running as a Service
		{
			status_info = (LPQUERY_SERVICE_CONFIG) LocalAlloc(LPTR, 4096); 
			if (!QueryServiceConfig(hService,status_info,4096,&dwBytesNeeded))
			{
				//dwBytesNeeded=GetLastError();
				ShowError(0, "ViewRegistryEntries - Cannot query Guardian service.");
			}
			else
			{
				if (status_info->dwStartType == SERVICE_AUTO_START )
				{
					fb_status.AutoStart = 1;
				}
			}
			
			CloseServiceHandle (hService);
		}

		//Now do the same again, but this time only look at the server itself
		service = REMOTE_SERVICE;
		display_name = REMOTE_DISPLAY_NAME;
		hService = OpenService (hScManager, service, SERVICE_QUERY_CONFIG);
		CloseServiceManager();
		if (hService != NULL) // then we are running as a Service
		{
			status_info = (LPQUERY_SERVICE_CONFIG) LocalAlloc(LPTR, 4096); 
			if (!QueryServiceConfig(hService,status_info,4096,&dwBytesNeeded))
			{
				//dwBytesNeeded=GetLastError();
				ShowError(0, "ViewRegistryEntries - Cannot query server service.");
			}
			else
			{
				fb_status.ServerName = status_info->lpBinaryPathName;
			}

			CString LoginAccount = status_info->lpServiceStartName;
			if (  LoginAccount == "LocalSystem" )
			{
				fb_status.SystemLogin = true;
			}
			else
			{
				fb_status.SystemLogin = false;
			}

			CloseServiceHandle (hService);
		}
	}
	else //Installed as Application, so look for an entry in registry
	{
	    HKEY hkey;
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
			"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, 
			KEY_QUERY_VALUE, &hkey) == ERROR_SUCCESS)
		{
			DWORD dwType;
			DWORD dwSize = MAX_PATH;
			fb_status.AutoStart = !(RegQueryValueEx(hkey, "Firebird", NULL, 
				&dwType, (LPBYTE) fb_status.ServerName.GetBuffer(dwSize/sizeof(TCHAR)), 
				&dwSize));
		    RegCloseKey (hkey);
		}
	}

#ifdef MANAGE_CLASSIC
	//Our look in the registry has probably given us 
	//the filename of the installed server
	if ( fb_status.ServerName.Find("fb_inet_server") == ERROR_SUCCESS )
	{
		fb_status.UseClassic = true;
	}
	if ( fb_status.ServerName.Find("fbserver") == ERROR_SUCCESS )
	{
		fb_status.UseClassic = false;
	}
	if ( fb_status.ServerName == "" ) 
	{
		// Nothing is stored in the registry so we must look to see if 
		// Firebird.conf has a preference
		fb_status.UseClassic = GetPreferredArchitecture();
	}
#endif

}


void CFBDialog::ApplyChanges()
/*
 *
 *	This is the guts of the applet. It all happens here.
 *
 *	With the addition of support for classic we now need to 
 *  evaluate 16 possible states.
 *
 */

{
	// Stop the update timer before doing anything else
	if (m_uiTimer) KillTimer(m_uiTimer);

	//find out what has changed and implement the changes
	try
	{	
	//Stage 1
		// Stop the Server 
		// We don't restart unless we were running
		// and we successfully stop the server.
		new_settings.WasRunning = false;	
		if ( fb_status.WasRunning )
			if ( ServerStop() )
				new_settings.WasRunning = true;

#if defined(_DEBUG)
		if (m_Reset_Display_To_Existing_Values = false)
			UpdateServerStatus();
#endif
		ProcessMessages();

	//Stage 2 - Gather details of changes to make

		//Manage change to startup - from/to manual or auto
		bool ChangeStartType = false;
		ChangeStartType = (	( (bool) fb_status.AutoStart &&  m_Manual_Start.GetCheck() ) || 
			((bool)  !fb_status.AutoStart &&  m_Auto_Start.GetCheck() ) );
		
		if ( ChangeStartType )
			new_settings.AutoStart = !fb_status.AutoStart;
		else 
			new_settings.AutoStart = fb_status.AutoStart;
		
		
/*		//Do we use Super Server or Classic
		bool ChangeServerArchitecture = false;
		ChangeServerArchitecture = ( ( (bool) !fb_status.UseClassic && m_Classic_Server.GetCheck() ) || 
			( (bool) fb_status.UseClassic && m_Super_Server.GetCheck() ) );
		if ( ChangeServerArchitecture )
			new_settings.UseClassic = !fb_status.UseClassic;
		else
			new_settings.UseClassic = fb_status.UseClassic;

		if (new_settings.UseClassic) 
		{
			new_settings.ServiceExecutable = m_CS_Server_Name;
		}
		else
		{
			new_settings.ServiceExecutable = m_SS_Server_Name;
		}

*/
		//Do we change Guardian Usage?
		bool ChangeGuardianUse = false;
		ChangeGuardianUse = ( ( (bool) !fb_status.UseGuardian && m_Use_Guardian.GetCheck() ) || 
			( (bool) fb_status.UseGuardian && !m_Use_Guardian.GetCheck() ) );
		if ( ChangeGuardianUse )
			new_settings.UseGuardian = !fb_status.UseGuardian;
		else
			new_settings.UseGuardian = fb_status.UseGuardian;

		//Finally, test for change between service and application usage.
		bool ChangeRunStyle = false;
		ChangeRunStyle = ( ( (bool) fb_status.UseService && m_Run_As_Application.GetCheck() )  ||
			( !(bool) fb_status.UseService && m_Run_As_Service.GetCheck() ) );
		if (ChangeRunStyle)
			new_settings.UseService = !fb_status.UseService;
		else
			new_settings.UseService = fb_status.UseService;

		
	//Stage 3 - implement changes

#if !defined(_DEBUG)
		BeginWaitCursor();
#endif

		//Three things to do
		// a) First pull down what is already there
		if ( ChangeRunStyle || ChangeGuardianUse /* || ChangeServerArchitecture */)
		{
			if ( fb_status.UseService )
			{
				ServiceRemove();
			}
			else
				AppRemove();
		}
		
		// b) update firebird.conf
		if ( ChangeGuardianUse )
		{
			SetGuardianUseInConf( new_settings.UseGuardian );
		}

/*		if ( ChangeServerArchitecture )
		{
			SetPreferredArchitectureInConf( new_settings.UseClassic );
		}
*/

		// c) install the new configuration
		if ( ChangeRunStyle || ChangeGuardianUse /* || ChangeServerArchitecture */)
		{
			
			if ( new_settings.UseService )
			{
				ServiceInstall( new_settings );
			}
			else
			{
				ConfigureRegistryForApp( new_settings.AutoStart );

			}
		}
		else
		{
			// We are not changing the run style and not changing guardian usage
			// and we are not changing the server architecture so
			// we only have the autostart setting left to change
			SetAutoStart( new_settings );
		}

		
		if ( new_settings.WasRunning && ( !FirebirdRunning() ) )
		{
			ProcessMessages();
			ServerStart( new_settings );
		}

		// If we haven't had a failure then we disable the apply button
		if ( m_Reset_Display_To_Existing_Values == false )
			DisableApplyButton();

	}
	catch ( ... )
	{
		// Oops, something bad happened. Which 
		// means the apply button is still enabled.
		// We should probably do a reset here.

		m_Reset_Display_To_Existing_Values = true;
	}

#if !defined(_DEBUG)
    EndWaitCursor();
#endif

	//Whatever the outcome of ApplyChanges we need to refresh the dialog
	m_uiTimer = SetTimer( 1, 500, NULL );

}


void CFBDialog::ResetCheckBoxes(CFBDialog::STATUS status)
{
	if (( status.ServerStatus == IDS_APPLICATION_RUNNING ) ||
		( status.ServerStatus == IDS_APPLICATION_STOPPED ))
	{
		m_Run_As_Application.SetCheck(1);
		m_Run_As_Service.SetCheck(0);
	}
	else
	{
		m_Run_As_Application.SetCheck(0);
		m_Run_As_Service.SetCheck(1);
	}
	
	//Now are we starting automatically or not?
	if (status.AutoStart)
	{
		m_Auto_Start.SetCheck(1);
		m_Manual_Start.SetCheck(0);
	}
	else
	{
		m_Auto_Start.SetCheck(0);
		m_Manual_Start.SetCheck(1);
	}
	
	m_Use_Guardian.SetCheck(status.UseGuardian);

/*	if ((status.UseClassic) )
	{
		m_Classic_Server.SetCheck(1);
		m_Super_Server.SetCheck(0);
	}
	else
	{
		m_Classic_Server.SetCheck(0);
		m_Super_Server.SetCheck(1);
	}
*/

	// The server can now be controlled by a specific 
	// username/password. If it is set then for now we 
	// will disable all config options and add 
	// support at a later date.
/*	m_Classic_Server.EnableWindow( status.SystemLogin );
	m_Super_Server.EnableWindow( status.SystemLogin );
*/	m_Auto_Start.EnableWindow( status.SystemLogin );
	m_Manual_Start.EnableWindow( status.SystemLogin );
	m_Run_As_Application.EnableWindow( status.SystemLogin );
	m_Run_As_Service.EnableWindow( status.SystemLogin );
	m_Use_Guardian.EnableWindow( status.SystemLogin );

}


void CFBDialog::ResetWarningBox( CFBDialog::STATUS status )
{
	if (status.SystemLogin)
	{
//		m_Config_Disabled_Notice.SetWindowText(CONFIG_OUT_OF_SYNC);

/*		bool showwarning = false;

		// Show warning if config is out of sync with 
		// reality of a running server.
		if (status.WasRunning) 
		{
#ifdef _DEBUG
			bool test1 = ( (bool) GetGuardianHandle() != (bool) GetGuardianUseSpecified()  );
			bool test2 = ( (bool) GetClassicServerHandle() != (bool) GetPreferredArchitecture() ); 
			if ((test1) || (test2))
			{
				showwarning = true;
			}
#endif
			showwarning = (
				( (bool) GetGuardianHandle() != (bool) GetGuardianUseSpecified()  ) ||
				( (bool) GetClassicServerHandle() != (bool) GetPreferredArchitecture() )
				);
		}

		m_Config_Disabled_Notice.ShowWindow( showwarning );

*/	}
	else
	{
//		m_Config_Disabled_Notice.SetWindowText(USER_ACCOUNT_IN_USE);
//		m_Config_Disabled_Notice.ShowWindow( !status.SystemLogin );
	}

}


int CFBDialog::DatabasesConnected()
// Return number of open databases
//**
//** Note: We really need a way of getting number of attachments
//** without having to enter a username / password.
//** This could be done by reading the LOCK_Header
//**
{
	int nDatabases = 0;

	return nDatabases;
}


bool CFBDialog::ServerStart( CFBDialog::STATUS status )
{
	bool result = false;

#if !defined(_DEBUG)
	BeginWaitCursor();
#endif

	if ( status.UseService ) 
	{

		char * service = "";
		char * display_name = "";
		if (status.UseGuardian)
		{
			service = ISCGUARD_SERVICE;
			display_name = ISCGUARD_DISPLAY_NAME;
		}
		else
		{
			service = REMOTE_SERVICE;
			display_name = REMOTE_DISPLAY_NAME;
		}


		OpenServiceManager();
		try
		{
			if (SERVICES_start (hScManager, service, display_name, 0, svc_error)
				==FB_SUCCESS)
			result = true;
		}
		catch( ... ) 
		{
		}
		CloseServiceManager();

	}
	else
	{
		try
		{
			STARTUPINFO si;
			SECURITY_ATTRIBUTES sa;
			PROCESS_INFORMATION pi;
			ZeroMemory (&si, sizeof(si));
			si.cb = sizeof (si);
			sa.nLength = sizeof (sa);
			sa.lpSecurityDescriptor = NULL;
			sa.bInheritHandle = TRUE;
			
			char full_name[MAX_PATH] = "";
			GetFullAppPath( status, full_name );
			
			if (!CreateProcess (NULL, full_name, &sa, NULL, FALSE, 0, NULL, NULL, &si, &pi))
				ShowError(0,"Application Start");
			else
				result = true;
		}
		catch( ... )
		{
		}
	}

#if !defined(_DEBUG)
    EndWaitCursor();
#endif

return result;
}


bool CFBDialog::ServerStop()
{
	bool result = false;

	if (!DatabasesConnected())
	{
		if ( fb_status.UseService ) 
		{
			try
			{
#if !defined(_DEBUG)
				BeginWaitCursor();
#endif
				OpenServiceManager();
				SERVICES_stop(hScManager, REMOTE_SERVICE, REMOTE_DISPLAY_NAME, svc_error);

				// If things are out of sync there is a slight possibility 
				// that the guardian may be running, so let's try and stop that too.
				SERVICES_stop(hScManager, ISCGUARD_SERVICE, ISCGUARD_DISPLAY_NAME, svc_error);

				result = true;
			}
			catch (...) 
			{
			MessageBeep(-1);
			}

				CloseServiceManager();
#if !defined(_DEBUG)
			    EndWaitCursor();
#endif
		}
		else
		{
			try
			{
#if !defined(_DEBUG)
				BeginWaitCursor();
#endif
				KillApp();
				result = true;
			}
			catch (...) 
			{
			}
#if !defined(_DEBUG)
		    EndWaitCursor();
#endif
		}
	}
	return result;
}


void CFBDialog::KillApp()
{
	HWND hTmpWnd = GetFirebirdHandle();
	if (hTmpWnd != NULL)
	{
		::SendMessage(hTmpWnd, WM_CLOSE, 0, 0);
	}
}


bool CFBDialog::ServiceInstall( CFBDialog::STATUS status )
							   
{
	USHORT	result;
	char * ServerPath = const_cast<char *> ((LPCTSTR)m_Root_Path);

	OpenServiceManager();

	if (new_settings.UseGuardian) 
	{
		result = FB_SUCCESS;
		result = SERVICES_install (hScManager, ISCGUARD_SERVICE, ISCGUARD_DISPLAY_NAME,
				ISCGUARD_EXECUTABLE, ServerPath, NULL, status.AutoStart,
				NULL, NULL, svc_error);
		if (result != FB_SUCCESS)
		{
			CloseServiceManager();
			return false;
		}

	/* Set AutoStart to manual in preparation for installing the ib_server service */
		status.AutoStart = false;

	}
	/* do the install of server */
	result = SERVICES_install (hScManager, REMOTE_SERVICE, REMOTE_DISPLAY_NAME,
				const_cast<char *> ((LPCTSTR) status.ServiceExecutable), 
				ServerPath, NULL, status.AutoStart, 
				NULL, NULL, svc_error);
	if (result != FB_SUCCESS)
	{
		CloseServiceManager();
		try
		{
			ServiceRemove();
			m_Reset_Display_To_Existing_Values = true;
			EnableApplyButton();
		}
		catch ( ... )
		{
		}

		return false;
	}

	CloseServiceManager();
	return true;
}


bool CFBDialog::ServiceRemove()
{
	USHORT	status;

	OpenServiceManager();
	status = SERVICES_remove (hScManager, ISCGUARD_SERVICE, 
								ISCGUARD_DISPLAY_NAME, svc_error);
	if (status == IB_SERVICE_RUNNING)
	{
		CloseServiceManager();
		return false;
	}

	status = SERVICES_remove (hScManager, REMOTE_SERVICE, 
								REMOTE_DISPLAY_NAME, svc_error);
	if (status == IB_SERVICE_RUNNING)
	{
		CloseServiceManager();
		return false;
	}

	CloseServiceManager();
	return true;

}


bool CFBDialog::ConfigureRegistryForApp(bool install)
{
	// The calling procedure will have already removed the 
	// service. All we need to do now
	// is configure the registry if AutoStart has been set.
	if (install)
	{
		//Add line to registry 
		HKEY hkey;
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
			"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 
			0, KEY_WRITE, &hkey) == ERROR_SUCCESS)
		{
			
			char full_name[MAX_PATH] = "";
			GetFullAppPath( new_settings, full_name);
			if (!RegSetValueEx (hkey, "Firebird", 0,REG_SZ, (unsigned char *) full_name, sizeof(full_name) )  == ERROR_SUCCESS)
			{
				ShowError(0, "AppInstall");
				return false;
			}
		}
		else
		{
			ShowError(0, "AppInstall");
			return false;
		}
	}
	else
	{
		//Remove registry entry if set to start automatically on boot.
		HKEY hkey;
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
			"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 
			0, KEY_QUERY_VALUE | KEY_WRITE, &hkey) == ERROR_SUCCESS)
			
		{
			if (RegQueryValueEx(hkey, "Firebird", NULL, NULL, NULL, NULL) 
				== ERROR_SUCCESS)
			{
				if (RegDeleteValue(hkey, "Firebird") == ERROR_SUCCESS)
					return true;
				else
				{
					ShowError(0, "Removing registry entry to stop autorun failed.");
					return false;
				}
			}
			else
			{
				// If an error is thrown it must be because there is no
				// entry in the registry so we shouldn't need to show an error.
				return true;
			}
			
		}
		else
		{
			//Things are really bad - perhaps user has screwed up their registry?
			ShowError(0, "Could not find HKLM\\Software\\Microsoft\\Windows\\CurrentVersion\\Run in the registry.");
			return false;
		}
	}

	return true;
}


bool CFBDialog::AppRemove()
{
	return ConfigureRegistryForApp(false);
}


static USHORT svc_error (SLONG	status, TEXT *string, SC_HANDLE	service)
{
	bool RaiseError = true;

	// process the kinds of errors we may be need to deal with quietly
	switch ( status )
	{
		case ERROR_SERVICE_CANNOT_ACCEPT_CTRL:
			RaiseError = false;

		case ERROR_SERVICE_ALREADY_RUNNING:
			RaiseError = false;

		case ERROR_SERVICE_DOES_NOT_EXIST:
			RaiseError = false;

	}

	if (RaiseError)
		CFBDialog::ShowError(status, string);
	
	if (service != NULL)
		CloseServiceHandle (service);

	return (USHORT) status;
}

void CFBDialog::ProcessMessages()
{
	MSG Msg;
	
	while (::PeekMessage(&Msg, NULL, 0, 0, PM_NOREMOVE))
	{
		if (!AfxGetApp()->PumpMessage())
		{
			::PostQuitMessage(0); 
			return;
		}
	}

	LONG lIdle = 0;
	while (AfxGetApp()->OnIdle(lIdle++));
	return;
}

void CFBDialog::ShowError(SLONG	status, TEXT *string )
{
	LPTSTR lpMsgBuf;
	DWORD error_code = GetLastError();
	DWORD Size;
	CString error_title = "";

	Size = FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error_code,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,	0, NULL );

	error_title.Format("Error Code %d raised in %s",error_code, (LPCTSTR) string );

	::MessageBox( NULL, lpMsgBuf, (LPCTSTR) error_title, MB_OK | MB_ICONINFORMATION );
	LocalFree( lpMsgBuf );
}


void CFBDialog::OnButtonStop() 
{
	if ( FirebirdRunning() )
		ServerStop();
	else	
		ServerStart( fb_status );
}


void CFBDialog::OnOK() 
{
	// Extra validation can be added here

	//if IDAPPLY is enabled then click IDAPPLY to apply changes before we close
	if (m_Apply.IsWindowEnabled())
	{
		OnApply();
	}
	
	CDialog::OnOK();
}


void CFBDialog::EnableApplyButton()
{
	m_Apply.EnableWindow(TRUE);
	m_Button_Stop.EnableWindow(FALSE);
}

void CFBDialog::DisableApplyButton()
{
	m_Apply.EnableWindow(FALSE);
	m_Button_Stop.EnableWindow(TRUE);
}
		

void CFBDialog::OnTimer(UINT nIDEvent) 
{
	UpdateServerStatus();
}


void CFBDialog::OnDestroy() 
{
	// Kill the update timer
	if (m_uiTimer) KillTimer(m_uiTimer);

	CDialog::OnDestroy();
}


void CFBDialog::OnService() 
{
	EnableApplyButton();
}


void CFBDialog::OnManualStart() 
{
	EnableApplyButton();
}


void CFBDialog::OnApplication() 
{
	EnableApplyButton();
}


void CFBDialog::OnAutoStart() 
{
	EnableApplyButton();
}


void CFBDialog::OnUseGuardian() 
{
	EnableApplyButton();
}


void CFBDialog::OnSuperServer() 
{
	EnableApplyButton();
}


void CFBDialog::OnClassicServer() 
{
	EnableApplyButton();
}


void CFBDialog::OnApply() 
{
	ApplyChanges();
}


void CFBDialog::SetGuardianUseInConf( bool UseGuardian )
{

	CString newvalue = "";
	if (UseGuardian)
		newvalue = "1";
	else
		newvalue = "0";


// One day the Config class will have set methods...
/*
	if (Config::setGuardianOption( UseGuardian ))
*/
	if (UpdateFirebirdConf("GuardianOption", newvalue ) )
	{
		// Do we assign here? or wait for the 
		// update status routine to pick this up?
		fb_status.UseGuardian = UseGuardian;
	}
	else
	{
		ShowError(0,"SetGuardianUseInConf");
		return;
	}
}


#ifdef MANAGE_CLASSIC
void CFBDialog::SetPreferredArchitectureInConf( bool UseClassic )
{
	CString newvalue = "";
	if ( UseClassic )
		newvalue = "1";
	else
		newvalue = "0";


// One day the Config class will have set methods...
/*
	if (Config::setPreferClassicServer( UseClassic ))
*/
	if ( UpdateFirebirdConf("PreferClassicServer", newvalue ) )
	{
		// Do we assign here? or wait for the 
		// update status routine to pick this up?
		fb_status.UseClassic = UseClassic;
	}
	else
	{
		ShowError(0,"SetPreferredArchitectureInConf");
		return;
	}
}
#endif

bool CFBDialog::UpdateFirebirdConf(CString option, CString value)
{
	bool result = false;
	
	CStdioFile FbConfFile, FbConfFileNew;
	CString FirebirdConfFilename = m_Root_Path + "firebird.conf";
	CString FirebirdConfNewname = FirebirdConfFilename + ".new";
	CString FirebirdConfOldname = FirebirdConfFilename + ".old";
	CString FirebirdConfLine = "";

	bool res;

	res = FbConfFile.Open(FirebirdConfFilename, CFile::modeReadWrite); 
	res = FbConfFileNew.Open(FirebirdConfNewname, CFile::modeCreate | 
							CFile::shareExclusive | CFile::modeWrite); 
	try
	{

		bool Found = false;
		int i = 0;

		while (FbConfFile.ReadString(FirebirdConfLine) != NULL)
		{

			if (FirebirdConfLine.Find( option ) > -1 )
			{
				FirebirdConfLine = option + " = " + value;
			}
	
			FbConfFileNew.WriteString( FirebirdConfLine + '\n');
		
		}
	}
	 catch ( CFileException e)
	{
    #ifdef _DEBUG
        afxDump << "Problem updating " << e.m_strFileName << ".\n \
					cause = " << e.m_cause << "\n";
    #endif
	}
	

	FbConfFile.Close();
	FbConfFileNew.Close();

	CFile::Rename(FirebirdConfFilename, FirebirdConfFilename + ".old");
	
	try
	{
		CFile::Rename(FirebirdConfNewname, FirebirdConfFilename );
		
		//If we get this far then all is well and we can return good news
		result = true;
	}
	catch ( CFileException e)
	{
		CFile::Rename(FirebirdConfOldname, FirebirdConfFilename);
	}

	//always try to delete the temporary old conf file.
	CFile::Remove(FirebirdConfOldname);

	return result;
}


void CFBDialog::SetAutoStart( CFBDialog::STATUS status )
{
	if (status.UseService)
	{
		SC_LOCK sclLock; 
		DWORD dwStartType; 
		SC_HANDLE hService;
		
		OpenServiceManager();
		// Need to acquire database lock before reconfiguring. 
		sclLock = LockServiceDatabase(hScManager); 
		
		// If the database cannot be locked, report the details. 
		if (sclLock == NULL) 
		{
			ShowError(NULL,"Could not lock service database"); 
			return;
		}
		
		// The database is locked, so it is safe to make changes. 
		
		char * service = "";
		char * display_name = "";

		if ( status.UseGuardian )
		{
			service = ISCGUARD_SERVICE;
			display_name = ISCGUARD_DISPLAY_NAME;
		}
		else
		{
			service = REMOTE_SERVICE;
			display_name = REMOTE_DISPLAY_NAME;
		}
		
		
		// Open a handle to the service. 
		hService = OpenService( 
			hScManager,				// SCManager database 
			service,				// name of service 
			SERVICE_CHANGE_CONFIG);	// need CHANGE access 
		if (hService == NULL) 
			ShowError(NULL,"OpenService in SetAutoStart"); 
		
		dwStartType = ( status.AutoStart ) ? SERVICE_AUTO_START : SERVICE_DEMAND_START; 
		
		if (! ChangeServiceConfig( 
			hService,			// handle of service 
			SERVICE_NO_CHANGE,	// service type: no change 
			dwStartType,		// change service start type 
			SERVICE_NO_CHANGE,	// error control: no change 
			NULL,				// binary path: no change 
			NULL,				// load order group: no change 
			NULL,				// tag ID: no change 
			NULL,				// dependencies: no change 
			NULL,				// account name: no change 
			NULL,				// password: no change 
			display_name ) )
		{
			ShowError(NULL,"ChangeServiceConfig in SetAutoStart"); 
		}
		
		
		// Release the database lock. 
		UnlockServiceDatabase(sclLock); 
		
		// Close the handle to the service. 
		CloseServiceHandle(hService); 

		CloseServiceManager();
	
	}
	else
	{
		ConfigureRegistryForApp( status.AutoStart );
	}
}


bool CFBDialog::FirebirdRunning()
{

	bool result = false;

	result = (bool) GetFirebirdHandle();

	// Server may be running under a user account, 
	// so application handles will disappear. Let's check 
	// for the guardian.
	if ( !result )
	{
		result = (bool) GetGuardianHandle();
	}

	return result;

}

HWND CFBDialog::GetFirebirdHandle()
{
	HWND result = NULL;
	result = GetSuperServerHandle();
#ifdef MANAGE_CLASSIC
	if ( !result )
	{
		result = GetClassicServerHandle();
	}
#endif
	return result;
}


HWND CFBDialog::GetSuperServerHandle()
{
	return ::FindWindow(szClassName, szWindowName);
}


#ifdef MANAGE_CLASSIC
HWND CFBDialog::GetClassicServerHandle()
{
	// oops - hard-coded string that is liable to change
	// Plus, the original definition is hidden locally
	// within a function in remote/windows (or whatever).
	return ::FindWindow( "FB_Disabled", szWindowName);
}
#endif

HWND CFBDialog::GetGuardianHandle()
{
	return ::FindWindow(GUARDIAN_CLASS_NAME, GUARDIAN_APP_LABEL);
}


bool CFBDialog::ServiceSupportAvailable()
{
	OSVERSIONINFO   OsVersionInfo;

	/* need to set the sizeof this structure for NT to work */
	OsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx ((LPOSVERSIONINFO) &OsVersionInfo);

	/* true for NT family, false for 95 family */
	return (OsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT);
}


void CFBDialog::OpenServiceManager()
{
	if (!fb_status.ServicesAvailable)
		return;
	if (hScManager == NULL)
		hScManager = OpenSCManager (NULL, SERVICES_ACTIVE_DATABASE, GENERIC_READ | GENERIC_EXECUTE | GENERIC_WRITE );

	if (!hScManager)
		ShowError(0, "OpenServiceManager");
}


void CFBDialog::CloseServiceManager()
{
	if (hScManager == NULL)
		return;

	try
	{
		CloseServiceHandle (hScManager);
	}
	catch (...)
	{
		// Err... what do we do now?
	}

	hScManager = NULL;
}


void CFBDialog::GetFullAppPath( CFBDialog::STATUS status, char * app)
// This returns the fully qualified path and name of the application 
// to start along with parameters -a and -c .
{
	CString AppName = m_Root_Path;

	if ( status.UseGuardian )
	{
		AppName += m_Guardian_Name;

#ifdef MANAGE_CLASSIC
		if (status.UseClassic) 
		{
			AppName += ".exe -c -a";
		}
		else
#endif
		{
			AppName += ".exe -a";
		}
	}
	else
	{
		GetServerName( status, AppName );
	}

	::strcat(app,AppName);

}

void CFBDialog::GetServerName( CFBDialog::STATUS status, CString& AppName)
{
#ifdef MANAGE_CLASSIC
	if ( status.UseClassic )
	{
		AppName += m_CS_Server_Name;
	}
	else
#endif
	{
		AppName += m_SS_Server_Name;
	}

	AppName += ".exe -a";
}


bool CFBDialog::GetGuardianUseSpecified()
{
	// Currently (RC6 timeframe) the pointer to the config
	// is not refreshed if the config file is changed on disk
	// and there doesn't seem to be a public mechanism to destroy it.
	// So we have to do this clunky thing where we check for the
	// latest possible setting.
	int option;
	if (!initialised)
		option = Config::getGuardianOption();
	else
		if ((new_settings.UseGuardian) != (fb_status.UseGuardian))
			option = new_settings.UseGuardian;
		else
			option = fb_status.UseGuardian;

	return (bool) option;
}


#ifdef MANAGE_CLASSIC
bool CFBDialog::GetPreferredArchitecture()
{
	//See note for GetGuardianUseSpecified()
	int option;
	if (!initialised)
		option = Config::getPreferredArchitecture();
	else
		if ((new_settings.UseClassic) != (fb_status.UseClassic))
			option = new_settings.UseClassic;
		else
			option = fb_status.UseClassic;

	return (bool) option;
}
#endif

