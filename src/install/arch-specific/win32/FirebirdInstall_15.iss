;  Initial Developer's Public License.
;  The contents of this file are subject to the  Initial Developer's Public
;  License Version 1.0 (the "License"). You may not use this file except
;  in compliance with the License. You may obtain a copy of the License at
;    http://www.ibphoenix.com/idpl.html
;  Software distributed under the License is distributed on an "AS IS" basis,
;  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
;  for the specific language governing rights and limitations under the
;  License.
;
;  The Original Code is copyright 2001-2003 Paul Reeves for IBPhoenix.
;
;  The Initial Developer of the Original Code is Paul Reeves for IBPhoenix.
;
;  All Rights Reserved.
;
;   Contributor(s):
;     Tilo Muetze, Theo ? and Michael Rimov for improved detection
;     of an existing install directory.
;     Simon Carter for the WinSock2 detection.

;   Usage Notes:
;
;   This script has been designed to work with My InnoSetup Extensions 3.0.6.2
;   or later. It may work with earlier versions but this is neither guaranteed
;   nor tested.
;


This script is not finished. Please don't try to use it yet.

;Either classic_server_install or super_server_install
;Default to SuperServer if not defined
#define super_server_install
#define msvc_version "7"
;#define classic_server_install
#define FirebirdURL "http://www.firebirdsql.org"
#define BaseVer "1_5"

[Setup]
AppName=Firebird Database Server 1.5
;The following is important - all ISS install packages should
;duplicate this for v1.5. See the InnoSetup help for details.
AppID=FBDBServer_{#BaseVer}
AppVerName=Firebird 1.5.0
AppPublisher=Firebird Project
AppPublisherURL={#FirebirdURL}
AppSupportURL={#FirebirdURL}
AppUpdatesURL={#FirebirdURL}
DefaultDirName={code:InstallDir|{pf}\FirebirdSQL\Firebird\{#BaseVer}}
DefaultGroupName=Firebird
AllowNoIcons=true
SourceDir=..\..\..\..\..\firebird2
LicenseFile=src\install\misc\IPLicense.txt
InfoBeforeFile=src\install\arch-specific\win32\installation_readme.txt
InfoAfterFile=src\install\arch-specific\win32\readme.txt
AlwaysShowComponentsList=true
WizardImageFile=src\install\arch-specific\win32\firebird_install_logo1.bmp
PrivilegesRequired=admin
UninstallDisplayIcon={app}\bin\fbserver.exe
OutputDir=builds\win32\install_image
#ifdef classic_server_install
OutputBaseFilename=Firebird-1.5.0-Win32-Classic
#else
OutputBaseFilename=Firebird-1.5.0-Win32-SuperServer
#endif
Compression=bzip

[Types]
Name: ServerInstall; Description: Full installation of server and development tools.
Name: DeveloperInstall; Description: Installation of Client tools for Developers and database administrators.
Name: ClientInstall; Description: Minimum client install - no server, no tools.

[Components]
Name: ServerComponent; Description: Server component; Types: ServerInstall
Name: DevAdminComponent; Description: Tools component; Types: ServerInstall DeveloperInstall
Name: ClientComponent; Description: Client component; Types: ServerInstall DeveloperInstall ClientInstall; Flags: fixed disablenouninstallwarning

[Tasks]
;Server tasks
Name: UseGuardianTask; Description: "Use the &Guardian to control the server?"; Components: ServerComponent; MinVersion: 4.0,4.0
Name: UseApplicationTask; Description: An &Application?; GroupDescription: "Run Firebird server as:"; Components: ServerComponent; MinVersion: 4,4; Flags: exclusive
Name: UseServiceTask; Description: A &Service?; GroupDescription: "Run Firebird server as:"; Components: ServerComponent; MinVersion: 0,4; Flags: exclusive
Name: AutoStartTask; Description: "Start &Firebird automatically everytime you boot up?"; Components: ServerComponent; MinVersion: 4,4;
;Developer Tasks
Name: MenuGroupTask; Description: Create a Menu &Group; Components: ServerComponent; MinVersion: 4,4;
;One for Ron
;Name: MenuGroupTask\desktopicon; Description: Create a &desktop icon; Components: ServerComponent; MinVersion: 4.0,4.0;

[Run]
;Always register Firebird
Filename: "{app}\bin\instreg.exe"; Parameters: "install ""{app}"" "; StatusMsg: Updating the registry; MinVersion: 4.0,4.0; Components: ClientComponent; Flags: runminimized

;If on NT/Win2k etc and 'Install and start service' requested
Filename: "{app}\bin\instsvc.exe"; Parameters: "install ""{app}"" {code:ServiceStartFlags|""""} "; StatusMsg: "Setting up the service"; MinVersion: 0,4.0; Components: ServerComponent; Flags: runminimized; Tasks: UseServiceTask;
Filename: "{app}\bin\instsvc.exe"; Description: "Start Firebird Service now?"; Parameters: start; StatusMsg: Starting the server; MinVersion: 0,4.0; Components: ServerComponent; Flags: runminimized postinstall; Tasks: UseServiceTask

;If 'start as application' requested
#ifdef classic_server_install
Filename: "{code:StartApp|{app}\bin\fb_inet_server.exe}"; Description: "Start Firebird now?"; Parameters: "-a"; StatusMsg: Starting the server; MinVersion: 0,4.0; Components: ServerComponent; Flags: nowait postinstall; Tasks: UseApplicationTask
#else
Filename: "{code:StartApp|{app}\bin\fbserver.exe}"; Description: "Start Firebird now?"; Parameters: "-a"; StatusMsg: Starting the server; MinVersion: 0,4.0; Components: ServerComponent; Flags: nowait postinstall; Tasks: UseApplicationTask
#endif

[Registry]
;If user has chosen to start as App they may well want to start automatically. That is handled by a function below.
;Unless we set a marker here the uninstall will leave some annoying debris.
Root: HKLM; Subkey: SOFTWARE\Microsoft\Windows\CurrentVersion\Run; ValueType: string; ValueName: Firebird; ValueData: ""; Flags: uninsdeletevalue; Tasks: UseApplicationTask;

;This doesn't seem to get cleared automatically by instreg on uninstall, so lets make sure of it
Root: HKLM; Subkey: SOFTWARE\FirebirdSQL; Flags: uninsdeletekeyifempty; Components: ClientComponent DevAdminComponent ServerComponent;

[Icons]
#ifdef classic_server_install
Name: "{group}\Firebird Server"; Filename: {app}\bin\fb_inet_server.exe; Parameters: "-a"; Flags: runminimized; MinVersion: 4.0,4.0; Tasks: MenuGroupTask; Check: InstallServerIcon; IconIndex: 0; Comment: "Run Firebird classic server (without guardian)";
#else
Name: "{group}\Firebird Server"; Filename: {app}\bin\fbserver.exe; Parameters: "-a"; Flags: runminimized; MinVersion: 4.0,4.0; Tasks: MenuGroupTask; Check: InstallServerIcon; IconIndex: 0; Comment: "Run Firebird Superserver (without guardian)";
#endif
Name: "{group}\Firebird Guardian"; Filename: {app}\bin\fbguard.exe; Parameters: "-a"; Flags: runminimized; MinVersion: 4.0,4.0; Tasks: MenuGroupTask; Check: InstallGuardianIcon; IconIndex: 1; Comment: "Run Firebird server (with guardian)";
Name: "{group}\Firebird 1.5 Release Notes"; Filename: {app}\doc\Firebird_v1_ReleaseNotes.pdf; MinVersion: 4.0,4.0; Tasks: MenuGroupTask; IconIndex: 1; Comment: "Firebird 1.0 release notes. (Requires Acrobat Reader.)";
Name: "{group}\Firebird 1.5 Readme"; Filename: {app}\readme.txt; MinVersion: 4.0,4.0; Tasks: MenuGroupTask;
Name: "{group}\Uninstall Firebird"; Filename: {uninstallexe}; Comment: "Uninstall Firebird"

[Files]
Source: src\install\misc\IPLicense.txt; DestDir: {app}; Components: ClientComponent; Flags: sharedfile ignoreversion;
Source: src\install\arch-specific\win32\readme.txt; DestDir: {app}; Components: DevAdminComponent; Flags: ignoreversion;
Source: builds\win32\output\firebird.conf; DestDir: {app}; Components: ServerComponent; Flags: uninsneveruninstall onlyifdoesntexist;
Source: builds\win32\output\aliases.conf; DestDir: {app}; Components: ServerComponent; Flags: uninsneveruninstall onlyifdoesntexist;
Source: builds\win32\output\security.fdb; DestDir: {app}; Components: ServerComponent; Flags: uninsneveruninstall onlyifdoesntexist;
Source: builds\win32\output\security.fbk; DestDir: {app}; Components: ServerComponent;  Flags: ignoreversion;
Source: builds\win32\output\firebird.log; DestDir: {app}; Components: ServerComponent; Flags: uninsneveruninstall skipifsourcedoesntexist external dontcopy;
Source: builds\win32\output\firebird.msg; DestDir: {app}; Components: ClientComponent; Flags: sharedfile ignoreversion;
Source: builds\win32\output\bin\gbak.exe; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion;
Source: builds\win32\output\bin\gbak.exe; DestDir: {app}\bin; Components: DevAdminComponent;  Flags: ignoreversion;
Source: builds\win32\output\bin\gdef.exe; DestDir: {app}\bin; Components: DevAdminComponent;  Flags: ignoreversion;
Source: builds\win32\output\bin\gfix.exe; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion;
Source: builds\win32\output\bin\gfix.exe; DestDir: {app}\bin; Components: DevAdminComponent;  Flags: ignoreversion;
Source: builds\win32\output\bin\gpre.exe; DestDir: {app}\bin; Components: DevAdminComponent;  Flags: ignoreversion;
Source: builds\win32\output\bin\gsec.exe; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion;
Source: builds\win32\output\bin\gsec.exe; DestDir: {app}\bin; Components: DevAdminComponent; Flags: sharedfile ignoreversion;
Source: builds\win32\output\bin\gstat.exe; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion;
Source: builds\win32\output\bin\fbguard.exe; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion;
Source: builds\win32\output\bin\fb_lock_print.exe; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion;
Source: builds\win32\output\bin\fb_inet_server.exe; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion;
Source: builds\win32\output\bin\fbserver.exe; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion;
Source: builds\win32\output\bin\ib_util.dll; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion;
Source: builds\win32\output\bin\instreg.exe; DestDir: {app}\bin; Components: ClientComponent; Flags: sharedfile ignoreversion;
Source: builds\win32\output\bin\instsvc.exe; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion;
Source: builds\win32\output\bin\isql.exe; DestDir: {app}\bin; Components: DevAdminComponent;  Flags: ignoreversion;
Source: builds\win32\output\bin\qli.exe; DestDir: {app}\bin; Components: DevAdminComponent;  Flags: ignoreversion;
Source: builds\win32\output\doc\*.*; DestDir: {app}\doc; Components: DevAdminComponent; Flags: skipifsourcedoesntexist external ignoreversion;
Source: builds\win32\output\help\*.*; DestDir: {app}\help; Components: DevAdminComponent;  Flags: ignoreversion;
Source: builds\win32\output\include\*.*; DestDir: {app}\include; Components: DevAdminComponent; Flags: ignoreversion;
Source: builds\win32\output\intl\fbintl.dll; DestDir: {app}\intl; Components: ServerComponent; Flags: sharedfile ignoreversion;
Source: builds\win32\output\lib\*.*; DestDir: {app}\lib; Components: DevAdminComponent;  Flags: ignoreversion;
Source: builds\win32\output\UDF\*.*; DestDir: {app}\UDF; Components: ServerComponent; Flags: sharedfile ignoreversion;
Source: builds\win32\output\examples\*.*; DestDir: {app}\examples; Components: DevAdminComponent;  Flags: ignoreversion;
;Source: builds\win32\output\bin\gds32.dll; DestDir: {app}\bin; Components: ClientComponent; Flags: overwritereadonly sharedfile promptifolder;
;Source: builds\win32\output\bin\fbclient.dll; DestDir: {app}\bin; Components: ClientComponent; Flags: overwritereadonly sharedfile promptifolder;
Source: builds\win32\output\bin\gds32.dll; DestDir: {sys}\; Components: ClientComponent; Flags: overwritereadonly sharedfile promptifolder;
Source: builds\win32\output\bin\fbclient.dll; DestDir: {sys}\; Components: ClientComponent; Flags: overwritereadonly sharedfile promptifolder;
Source: src\install\arch-specific\win32\msvcrt.dll; DestDir: {sys}\; Components: ClientComponent; Flags: uninsneveruninstall sharedfile onlyifdoesntexist;
Source: src\install\arch-specific\win32\msvcp{#msvc_version}0.dll; DestDir: {sys}\; Components: ClientComponent; Flags: uninsneveruninstall sharedfile onlyifdoesntexist;
Source: src\extlib\fbudf\fbudf.sql; DestDir: {app}\examples; Components: ServerComponent;  Flags: ignoreversion;
Source: src\extlib\fbudf\fbudf.txt; DestDir: {app}\doc; Components: ServerComponent;  Flags: ignoreversion;
Source: src\extlib\ib_util.pas; DestDir: {app}\include; Components: DevAdminComponent;  Flags: ignoreversion;
;Source: firebird\install\doc_all_platforms\Firebird_v1_ReleaseNotes.pdf; DestDir: {app}\doc; Components: DevAdminComponent;  Flags: ignoreversion;
;Source: firebird\install\doc_all_platforms\Firebird_v1_*.html; DestDir: {app}\doc; Components: DevAdminComponent;  Flags: ignoreversion;

[UninstallRun]
Filename: {app}\bin\instsvc.exe; Parameters: stop; StatusMsg: "Stopping the service"; MinVersion: 0,4.0; Components: ServerComponent; Flags: runminimized; Tasks: UseServiceTask;
Filename: {app}\bin\instsvc.exe; Parameters: remove -g; StatusMsg: "Removing the service"; MinVersion: 0,4.0; Components: ServerComponent; Flags: runminimized; Tasks: UseServiceTask;
Filename: {app}\bin\instreg.exe; Parameters: remove; StatusMsg: "Updating the registry"; MinVersion: 4.0,4.0; Flags: runminimized;

[UninstallDelete]
Type: files; Name: {app}\*.lck
Type: files; Name: {app}\*.evn

[_ISTool]
EnableISX=true

[Code]
program Setup;

const
  sWinSock2 = 'ws2_32.dll';
  sNoWinsock2 = 'Please Install Winsock 2 Update before continuing';
  sMSWinsock2Update = 'http://www.microsoft.com/windows95/downloads/contents/WUAdminTools/S_WUNetworkingTools/W95Sockets2/Default.asp';
  sWinsock2Web = 'Winsock 2 is not installed.'#13#13'Would you like to Visit the Winsock 2 Update Home Page?';

var
  Winsock2Failure:    Boolean;
	InterBaseVer: Array of Integer;
  //  Likely values for installed versions of InterBase are:
  //  [6,2,0,nnn]   Firebird 1.0.0
  //  [6,2,2,nnn]   Firebird 1.0.2
  //  [6,0,n,n]     InterBase 6.0
  //  [6,5,n,n]     InterBase 6.5
  //  [7,0,n,n]     InterBase 7.0

	FirebirdVer: Array of Integer;
  //  Likely values for installed versions of Firebird are:
  //  [6,2,0,nnn]   Firebird 1.0.0
  //  [6,2,2,nnn]   Firebird 1.0.2
  //  [6,2,3,nnn]   Firebird 1.0.3
  //  [1,5,0,nnnn]  Firebird 1.5.0

function CheckWinsock2(): Boolean;
begin
  Result := True;
  //Check if Winsock 2 is installed (win 95 only)
  if (not UsingWinNt) and (not FileExists(AddBackslash(GetSystemDir) + sWinSock2)) then begin
    Winsock2Failure := True;
    Result := False;
    end
  else
  	Winsock2Failure := False;
end;

procedure DeInitializeSetup();
var
  ErrCode: Integer;
begin
  // Did the install fail because winsock 2 was not installed?
  if Winsock2Failure then
    // Ask user if they want to visit the Winsock2 update web page.
  	if MsgBox(sWinsock2Web, mbInformation, MB_YESNO) = idYes then
  	  // User wants to visit the web page
      InstShellExec(sMSWinsock2Update, '', '', SW_SHOWNORMAL, ErrCode);
end;

function InitializeSetup(): Boolean;
var
  i: Integer;
begin
  result := true;

  if not CheckWinsock2 then
    exit;

  //Look for a running version of Firebird
  i:=FindWindowByClassName('FB_Disabled');
  if ( i=0 ) then
    i:=FindWindowByClassName('FB_Server');
    
  if i<>0 then begin
    result := false;
    MsgBox('An existing Firebird Server is running. You must close the '+
           'application or stop the service before continuing.', mbError, MB_OK);
  end;
end;

procedure DecodeVersion( verstr: String; var verint: array of Integer );
var
  i,p: Integer; s: string;
begin
  verint := [0,0,0,0];
  i := 0;
  while ( (Length(verstr) > 0) and (i < 4) ) do
  begin
  	p := pos('.', verstr);
  	if p > 0 then
  	begin
      if p = 1 then s:= '0' else s:= Copy( verstr, 1, p - 1 );
  	  verint[i] := StrToInt(s);
  	  i := i + 1;
  	  verstr := Copy( verstr, p+1, Length(verstr));
  	end
  	else
  	begin
  	  verint[i] := StrToInt( verstr );
  	  verstr := '';
  	end;
  end;
end;

function GetInstalledVersion(ADir: String): Array of Integer;
var
	AString: String;
	VerInt:  Array of Integer;
begin
  if (ADir<>'') then begin
    GetVersionNumbersString( ADir+'\bin\gbak.exe', Astring);
    DecodeVersion(AString, VerInt);
  end;
  result := VerInt;
end;

function GetFirebirdDir: string;
//Check if Firebird installed, get version info to global var and return root dir
var
	FirebirdDir: String;
begin
	FirebirdVer    := [0,0,0,0];
  RegQueryStringValue(HKEY_LOCAL_MACHINE,
    'SOFTWARE\FirebirdSQL\Firebird\CurrentVersion','RootDirectory', FirebirdDir);
  if (FirebirdDir<>'') then
    FirebirdVer:=GetInstalledVersion(FirebirdDir);
end;

function GetInterBaseDir: string;
//Check if InterBase installed, get version info to global var and return root dir
var
  InterBaseDir: String;
begin
	InterBaseVer   := [0,0,0,0];
  RegQueryStringValue(HKEY_LOCAL_MACHINE,
    'SOFTWARE\Borland\InterBase\CurrentVersion','RootDirectory', InterBaseDir);
  if (InterBaseDir<>'') then
    InterBaseVer:=GetInstalledVersion(InterBaseDir);
end;

//This function tries to find an existing install of Firebird 1.5
//If it succeeds it suggests that directory for the install
//Otherwise it suggests the default for Fb 1.5
function InstallDir(Default: String): String;
var
	InstallRootDir,
	InterBaseRootDir,
	FirebirdRootDir: String;
begin
	InstallRootDir := '';

  // Try to find the value of "RootDirectory" in the Firebird
  // registry settings. This is either where Fb 1.0 exists or Fb 1.5
  InterBaseRootDir:=GetInterBaseDir;
  FirebirdRootDir:=GetFirebirdDir;
  
  if ( FirebirdRootDir = InterBaseRootDir ) then  //Fb 1.0 must be installed so don't overwrite it.
    InstallRootDir := Default;
    
  if (( InstallRootDir = '' ) and
      ( FirebirdRootDir = Default )) then // Fb 1.5 is already installed,
    InstallRootDir := Default;             // so we offer to install over it

  if (( InstallRootDir = '') and
      ( FirebirdVer[0] = 1 ) and ( FirebirdVer[1] = 5 ) ) then   // Firebird 1.5 is installed
    InstallRootDir := FirebirdRootDir;                            // but the user has changed the default

  // if we haven't found anything then try the FIREBIRD env var
  // User may have preferred location for Firebird, but has possibly
  // uninstalled previous version
  if (InstallRootDir = '') then
    InstallRootDir:=getenv('FIREBIRD');
    
  //if no existing locations found so make sure we default to the default.
  if (InstallRootDir = '') then
    InstallRootDir := Default;

  Result := ExpandConstant(InstallRootDir);

end;

function UseGuardian(Default: String): String;
begin
if ShouldProcessEntry('ServerComponent', 'UseGuardianTask')= srYes then
  Result := '1'
else
  Result := '0';
end;

function ServiceStartFlags(Default: String): String;
var
  classic: String;
begin
  classic := '';
#ifdef classic_server_install
  classic := ' -classic';
#endif
  Result := '';
  if ShouldProcessEntry('ServerComponent', 'UseGuardianTask')= srYes then begin
    if ShouldProcessEntry('ServerComponent', 'AutoStartTask')= srYes then
      Result := ' -auto ' + classic + ' -g '
    else
      Result := classic + ' -g ';
    end
  else
    if ShouldProcessEntry('ServerComponent', 'AutoStartTask')= srYes then
      Result := ' -auto ';
end;

function InstallGuardianIcon(): Boolean;
begin
  result := false;
  if ShouldProcessEntry('ServerComponent', 'UseApplicationTask')= srYes then
    if ShouldProcessEntry('ServerComponent', 'UseGuardianTask')= srYes then
      result := true;
end;

function InstallServerIcon(): Boolean;
begin
  result := false;
  if ShouldProcessEntry('ServerComponent', 'UseApplicationTask')= srYes then
    if ShouldProcessEntry('ServerComponent', 'UseGuardianTask')= srNo then
      result := true;
end;

function StartApp(Default: String): String;
var
  AppPath: String;
begin
  AppPath:=ExpandConstant('{app}');
  //Now start the app as
  if ShouldProcessEntry('ServerComponent', 'UseGuardianTask')= srYes then
    Result := AppPath+'\bin\fbguard.exe'
  else
#ifdef classic_server_install
    Result := AppPath+'\bin\fb_inet_server.exe';
#else
    Result := AppPath+'\bin\fbserver.exe';
#endif
end;

procedure CurStepChanged(CurStep: Integer);
var
  AppStr: String;
begin
if CurStep=csFinished then begin
  //If user has chosen to install an app and run it automatically set up the registry accordingly
  //so that the server or guardian starts evertime they login.
  if (ShouldProcessEntry('ServerComponent', 'AutoStartTask')= srYes) and
      ( ShouldProcessEntry('ServerComponent', 'UseApplicationTask')= srYes ) then begin
    AppStr := StartApp('')+' -a';

    RegWriteStringValue (HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Run', 'Firebird', AppStr);

  end;
end;

end;

begin
end.
