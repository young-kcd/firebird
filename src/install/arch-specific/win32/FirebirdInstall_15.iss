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
;   nor tested. My InnoSetup Extensions is available from
;     http://www.wintax.nl/isx/
;
;   Known bugs and problems etc etc.
;
;   o The uninstaller can only uninstall what it installed.
;     For instance, if Firebird is installed as a service, but is actually
;     running as an application when the uninstaller is run then the
;     application will not be stopped and the uninstall will not complete
;     cleanly.
;
;     InnoSetup does not support script execution as part of uninstallation.
;     And this feature is not likely to be added in the near future. Here is
;     the definitive explanation from Jordan Russell, the developer of InnoSetup:
;
;       'The uninstaller currently relies on a 100% forward compatible uninstall
;       log format. When an existing uninstall log is appended to, and the
;       uninstaller EXE is replaced with a newer version, the previous log contents
;       are guaranteed to be processed properly. There are no such guarantees of
;       forward compatibility with the Pascal Scripting feature; some support
;       functions might be removed at some point, their declarations might change,
;       etc. For the uninstaller to support Pascal Scripting, it would likely have
;       to abandon the uninstall-log-appending concept, and instead create multiple
;       uninstaller EXEs (chained together somehow) and multiple uninstall logs.'
;
;     To work around this we will probably need to extend the instsvc/instreg
;     utilities to support uninstallation of applications.
;
;
;   o The uninstaller does not know how to stop multiple instances of a classic
;     server. They must be stopped manually.
;
;
;To Do
;  Check for other references to classic/super
;  Code to check registry for actual version we are installing
;Make sure we update firebird.conf if not using guardian

#define msvc_version 6
#define FirebirdURL "http://www.firebirdsql.org"
#define BaseVer "1_5"
#define release

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
DefaultDirName={code:ChooseInstallDir|{pf}\Firebird\Firebird_{#BaseVer}}
DefaultGroupName=Firebird_{#BaseVer}
AllowNoIcons=true
SourceDir=..\..\..\..\..\firebird2
LicenseFile=src\install\misc\IPLicense.txt
InfoBeforeFile=output\doc\installation_readme.txt
InfoAfterFile=src\install\arch-specific\win32\readme.txt
AlwaysShowComponentsList=true
WizardImageFile=src\install\arch-specific\win32\firebird_install_logo1.bmp
PrivilegesRequired=admin
UninstallDisplayIcon={code:ChooseUninstallIcon|{app}\bin\fbserver.exe}
OutputDir=builds\win32\install_image
OutputBaseFilename=Firebird-1.5.0-Win32
Compression=bzip
ShowTasksTreeLines=false

[Types]
Name: SuperServerInstall; Description: "Full installation of Super Server and development tools."
Name: ClassicServerInstall; Description: "Full installation of Classic Server and development tools."
Name: DeveloperInstall; Description: "Installation of Client tools for Developers and database administrators."
Name: ClientInstall; Description: "Minimum client install - no server, no tools."
Name: CustomInstall; Description: "Custom installation"; Flags: iscustom

[Components]
Name: SuperServerComponent; Description: Super Server binary; Types: SuperServerInstall; Flags: exclusive;
Name: ClassicServerComponent; Description: Classic Server binary; Types: ClassicServerInstall; Flags: exclusive;
Name: ServerComponent; Description: Server components; Types: SuperServerInstall ClassicServerInstall;
Name: DevAdminComponent; Description: Developer and admin tools components; Types: SuperServerInstall ClassicServerInstall DeveloperInstall;
Name: ClientComponent; Description: Client components; Types: SuperServerInstall ClassicServerInstall DeveloperInstall ClientInstall CustomInstall; Flags: fixed disablenouninstallwarning;

[Tasks]
;Server tasks
Name: UseGuardianTask; Description: Use the &Guardian to control the server?; Components: ServerComponent; MinVersion: 4.0,4.0
Name: UseApplicationTask; Description: Run as an &Application?; GroupDescription: Run Firebird server as:; Components: ServerComponent; MinVersion: 4,4; Flags: exclusive
Name: UseServiceTask; Description: Run as a &Service?; GroupDescription: Run Firebird server as:; Components: ServerComponent; MinVersion: 0,4; Flags: exclusive
;;Name: UseClassicServerTask; Description: "Classic Server?"; GroupDescription: "Which Server Architecture do you want to run?"; Components: ServerComponent; MinVersion: 4,4; Flags: exclusive;
;;Name: UseSuperServerTask; Description: "Super Server"; GroupDescription: "Which Server Architecture do you want to run?"; Components: ServerComponent; MinVersion: 4,4; Flags: exclusive;
Name: AutoStartTask; Description: Start &Firebird automatically everytime you boot up?; Components: ServerComponent; MinVersion: 4,4
;Developer Tasks
Name: MenuGroupTask; Description: Create a Menu &Group; Components: DevAdminComponent; MinVersion: 4,4
;One for Ron
;Name: MenuGroupTask\desktopicon; Description: Create a &desktop icon; Components: ServerComponent; MinVersion: 4.0,4.0;

[Run]
;Always register Firebird
Filename: {app}\bin\instreg.exe; Parameters: "install "; StatusMsg: Updating the registry; MinVersion: 4.0,4.0; Components: ClientComponent; Flags: runminimized

;If on NT/Win2k etc and 'Install and start service' requested
Filename: {app}\bin\instsvc.exe; Parameters: "install {code:ServiceStartFlags|""""} "; StatusMsg: Setting up the service; MinVersion: 0,4.0; Components: ServerComponent; Flags: runminimized; Tasks: UseServiceTask
Filename: {app}\bin\instsvc.exe; Description: Start Firebird Service now?; Parameters: start; StatusMsg: Starting the server; MinVersion: 0,4.0; Components: ServerComponent; Flags: runminimized postinstall; Tasks: UseServiceTask; Check: StartEngine

;If 'start as application' requested
Filename: {code:StartApp|{app}\bin\fbserver.exe}; Description: Start Firebird now?; Parameters: -a; StatusMsg: Starting the server; MinVersion: 0,4.0; Components: ServerComponent; Flags: nowait postinstall; Tasks: UseApplicationTask; Check: StartEngine

[Registry]
;If user has chosen to start as App they may well want to start automatically. That is handled by a function below.
;Unless we set a marker here the uninstall will leave some annoying debris.
Root: HKLM; Subkey: SOFTWARE\Microsoft\Windows\CurrentVersion\Run; ValueType: string; ValueName: Firebird; ValueData: ; Flags: uninsdeletevalue; Tasks: UseApplicationTask

;This doesn't seem to get cleared automatically by instreg on uninstall, so lets make sure of it
Root: HKLM; Subkey: SOFTWARE\FirebirdSQL; Flags: uninsdeletekeyifempty; Components: ClientComponent DevAdminComponent ServerComponent

[Icons]
Name: {group}\Firebird Server; Filename: {app}\bin\fb_inet_server.exe; Parameters: -a; Flags: runminimized; MinVersion: 4.0,4.0; Tasks: MenuGroupTask; Check: InstallServerIcon; IconIndex: 0; Components: ClassicServerComponent; Comment: Run Firebird classic server (without guardian)
Name: {group}\Firebird Server; Filename: {app}\bin\fbserver.exe; Parameters: -a; Flags: runminimized; MinVersion: 4.0,4.0; Tasks: MenuGroupTask; Check: InstallServerIcon; IconIndex: 0; Components: SuperServerComponent; Comment: Run Firebird Superserver (without guardian)
Name: {group}\Firebird Guardian; Filename: {app}\bin\fbguard.exe; Parameters: -a; Flags: runminimized; MinVersion: 4.0,4.0; Tasks: MenuGroupTask; Check: InstallGuardianIcon; IconIndex: 1; Components: SuperServerComponent; Comment: Run Firebird Super Server (with guardian)
Name: {group}\Firebird Guardian; Filename: {app}\bin\fbguard.exe; Parameters: -c; Flags: runminimized; MinVersion: 4.0,4.0; Tasks: MenuGroupTask; Check: InstallGuardianIcon; IconIndex: 1; Components: ClassicServerComponent; Comment: Run Firebird Classic Server (with guardian)
Name: {group}\Firebird 1.5 Release Notes; Filename: {app}\doc\Firebird_v15_ReleaseNotes.draft1.pdf; MinVersion: 4.0,4.0; Tasks: MenuGroupTask; IconIndex: 1; Comment: #AppName release notes. (Requires Acrobat Reader.)
Name: {group}\Firebird 1.5 Readme; Filename: {app}\readme.txt; MinVersion: 4.0,4.0; Tasks: MenuGroupTask
Name: {group}\Uninstall Firebird; Filename: {uninstallexe}; Comment: Uninstall Firebird

[Files]
Source: src\install\misc\IPLicense.txt; DestDir: {app}; Components: ClientComponent; Flags: sharedfile ignoreversion
Source: src\install\misc\IDPLicense.txt; DestDir: {app}; Components: ClientComponent; Flags: sharedfile ignoreversion
Source: src\install\arch-specific\win32\readme.txt; DestDir: {app}; Components: DevAdminComponent; Flags: ignoreversion
Source: output\firebird.conf; DestDir: {app}; DestName: firebird.conf; Components: ServerComponent; Flags: uninsneveruninstall; check: SaveFirebirdConf
Source: output\aliases.conf; DestDir: {app}; Components: ServerComponent; Flags: uninsneveruninstall onlyifdoesntexist
Source: output\security.fdb; DestDir: {app}; Components: ServerComponent; Flags: uninsneveruninstall onlyifdoesntexist
Source: output\security.fbk; DestDir: {app}; Components: ServerComponent; Flags: ignoreversion
Source: output\firebird.msg; DestDir: {app}; Components: ClientComponent; Flags: sharedfile ignoreversion
Source: output\firebird.log; DestDir: {app}; Components: ServerComponent; Flags: uninsneveruninstall skipifsourcedoesntexist external dontcopy

Source: output\bin\gbak.exe; DestDir: {app}\bin; Components: DevAdminComponent; Flags: sharedfile ignoreversion
Source: output\bin\gdef.exe; DestDir: {app}\bin; Components: DevAdminComponent; Flags: ignoreversion
Source: output\bin\gfix.exe; DestDir: {app}\bin; Components: DevAdminComponent; Flags: sharedfile ignoreversion
Source: output\bin\gpre.exe; DestDir: {app}\bin; Components: DevAdminComponent; Flags: ignoreversion
Source: output\bin\gsec.exe; DestDir: {app}\bin; Components: DevAdminComponent; Flags: sharedfile ignoreversion
Source: output\bin\gsplit.exe; DestDir: {app}\bin; Components: DevAdminComponent; Flags: sharedfile ignoreversion
Source: output\bin\gstat.exe; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion
Source: output\bin\fbguard.exe; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion
Source: output\bin\fb_lock_print.exe; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion
Source: output\bin\fb_inet_server.exe; DestDir: {app}\bin; Components: ClassicServerComponent; Flags: sharedfile ignoreversion
Source: output\bin\fbserver.exe; DestDir: {app}\bin; Components: SuperServerComponent; Flags: sharedfile ignoreversion
Source: output\bin\ib_util.dll; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion
Source: output\bin\instreg.exe; DestDir: {app}\bin; Components: ClientComponent; Flags: sharedfile ignoreversion
Source: output\bin\instsvc.exe; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion
Source: output\bin\isql.exe; DestDir: {app}\bin; Components: DevAdminComponent; Flags: ignoreversion
Source: output\bin\qli.exe; DestDir: {app}\bin; Components: DevAdminComponent; Flags: ignoreversion

Source: output\bin\gds32.dll; DestDir: {app}\bin; Components: ClientComponent; Flags: overwritereadonly sharedfile promptifolder
Source: output\bin\gds32.local; DestDir: {app}\bin; Components: ClientComponent; Flags: overwritereadonly sharedfile promptifolder; MinVersion: 0,5.0
Source: output\bin\fbclient.dll; DestDir: {app}\bin; Components: ClientComponent; Flags: overwritereadonly sharedfile promptifolder
Source: output\bin\fbclient.local; DestDir: {app}\bin; Components: ClientComponent; Flags: overwritereadonly sharedfile promptifolder; MinVersion: 0,5.0

Source: output\bin\msvcrt.dll; DestDir: {app}\bin; Components: ClientComponent; 
Source: output\bin\msvcrt.local; DestDir: {app}\bin; Components: ClientComponent; MinVersion: 0,5.0;
Source: output\bin\msvcp{#msvc_version}0.dll; DestDir: {app}\bin; Components: ClientComponent; 
Source: output\bin\msvcp{#msvc_version}0.local; DestDir: {app}\bin; Components: ClientComponent; MinVersion: 0,5.0;

Source: output\doc\*.*; DestDir: {app}\doc; Components: DevAdminComponent; Flags: skipifsourcedoesntexist  ignoreversion
Source: output\doc\sql.extensions\*.*; DestDir: {app}\doc\sql.extensions; Components: DevAdminComponent; Flags: skipifsourcedoesntexist ignoreversion
Source: output\help\*.*; DestDir: {app}\help; Components: DevAdminComponent; Flags: ignoreversion
Source: output\include\*.*; DestDir: {app}\include; Components: DevAdminComponent; Flags: ignoreversion
Source: output\intl\fbintl.dll; DestDir: {app}\intl; Components: ServerComponent; Flags: sharedfile ignoreversion
Source: output\lib\*.*; DestDir: {app}\lib; Components: DevAdminComponent; Flags: ignoreversion
Source: output\UDF\ib_udf.dll; DestDir: {app}\UDF; Components: ServerComponent; Flags: sharedfile ignoreversion
Source: output\UDF\fbudf.dll; DestDir: {app}\UDF; Components: ServerComponent; Flags: sharedfile ignoreversion
Source: output\UDF\*.sql; DestDir: {app}\UDF; Components: ServerComponent; Flags: ignoreversion
;Source: output\examples\*.*; DestDir: {app}\examples; Components: DevAdminComponent; Flags: ignoreversion
Source: output\doc\Firebird_v15_ReleaseNotes.draft1.pdf; DestDir: {app}\doc; Components: DevAdminComponent; Flags: ignoreversion
;Source: firebird\install\doc_all_platforms\Firebird_v1_5_*.html; DestDir: {app}\doc; Components: DevAdminComponent;  Flags: ignoreversion;
;Note - Win9x requires 8.3 filenames for the uninsrestartdelete option to work
Source: output\system32\Firebird2Control.cpl; DestDir: {sys}; Components: SuperServerComponent; MinVersion: 0,4.0; Flags: sharedfile ignoreversion promptifolder restartreplace uninsrestartdelete
Source: output\system32\FIREBI~1.CPL; DestDir: {sys}; Components: SuperServerComponent; MinVersion: 4.0,0; Flags: sharedfile ignoreversion promptifolder restartreplace uninsrestartdelete

;TO DO note
;We need to add in the debug files here, if #define debug has been set above.
;Until then we don't really support the debug option

[UninstallRun]
Filename: {app}\bin\instsvc.exe; Parameters: " stop"; StatusMsg: Stopping the service; MinVersion: 0,4.0; Components: ServerComponent; Flags: runminimized; Tasks: UseServiceTask; RunOnceId: StopService
Filename: {app}\bin\instsvc.exe; Parameters: " remove"; StatusMsg: Removing the service; MinVersion: 0,4.0; Components: ServerComponent; Flags: runminimized; Tasks: UseServiceTask; RunOnceId: RemoveService
Filename: {app}\bin\instreg.exe; Parameters: " remove"; StatusMsg: Updating the registry; MinVersion: 4.0,4.0; Flags: runminimized; RunOnceId: RemoveRegistryEntry

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
//  ProductVersion = '1.5.0';

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
  // Note - sed will process the commas to periods in the above line

Type
  TSharedFileArrayRecord = record
    Filename: String;
    Count: Integer;
  end;

Var
  SharedFileArray: Array of TSharedFileArrayRecord;
  InstallRootDir: String;

Var
  FirebirdConfSaved: boolean;

(*
InnoSetup Help Extract on Windows version strings:
4.0.950       Windows 95
4.0.1111      Windows 95 OSR 2 & OSR 2.1
4.0.1212      Windows 95 OSR 2.5
4.1.1998      Windows 98
4.1.2222      Windows 98 Second Edition
4.9.3000      Windows Me
Windows NT versions:
4.0.1381      Windows NT 4.0
5.0.2195      Windows 2000
5.01.2600     Windows XP
5.2.3790      Windows 2003 Standard
*)

function UsingWin2k: boolean;
//return true if using Win2k OR later
begin
  Result := (InstallOnThisVersion('0,5.0', '0,0') = irInstall);
end;

function UsingWinXP: boolean;
// return true if using WinXP OR later.
// Currently not used in this script.
begin
  Result := (InstallOnThisVersion('0,5.01', '0,0') = irInstall);
end;

function ClassicInstallChosen: Boolean;
var
  SelectedComponents: String;
begin
  result := false;

  SelectedComponents := WizardSelectedComponents(false);
  if pos(lowercase('ClassicServerComponent'),SelectedComponents) >0 then
    result := true;
end;

procedure SetupSharedFilesArray;
begin
if UsingWin2k then
  SetArrayLength(SharedFileArray,23)
else
  SetArrayLength(SharedFileArray,19);

SharedFileArray[0].Filename := ExpandConstant('{app}')+'\IPLicense.txt';
SharedFileArray[1].Filename := ExpandConstant('{app}')+'\firebird.msg';

SharedFileArray[2].Filename := ExpandConstant('{app}')+'\bin\gds32.dll';
SharedFileArray[3].Filename := ExpandConstant('{app}')+'\bin\fbclient.dll';

SharedFileArray[4].Filename := ExpandConstant('{app}')+'\bin\instreg.exe';
SharedFileArray[5].Filename := ExpandConstant('{app}')+'\bin\gbak.exe';
SharedFileArray[6].Filename := ExpandConstant('{app}')+'\bin\gfix.exe';
SharedFileArray[7].Filename := ExpandConstant('{app}')+'\bin\gsec.exe';
SharedFileArray[8].Filename := ExpandConstant('{app}')+'\bin\gsplit.exe';
SharedFileArray[9].Filename := ExpandConstant('{app}')+'\bin\gstat.exe';
SharedFileArray[10].Filename := ExpandConstant('{app}')+'\bin\fbguard.exe';
SharedFileArray[11].Filename := ExpandConstant('{app}')+'\bin\fb_lock_print.exe';
SharedFileArray[12].Filename := ExpandConstant('{app}')+'\bin\ib_util.dll';
SharedFileArray[13].Filename := ExpandConstant('{app}')+'\bin\instsvc.exe';
SharedFileArray[14].Filename := ExpandConstant('{app}')+'\bin\fbintl.dll';

if ClassicInstallChosen then
  SharedFileArray[15].Filename := ExpandConstant('{app}')+'\bin\fb_inet_server.exe'
else
  SharedFileArray[15].Filename := ExpandConstant('{app}')+'\bin\fbserver.exe';

if UsingWinNT then
  SharedFileArray[16].Filename := ExpandConstant('{sys}')+'\Firebird2Control.cpl'
else
  SharedFileArray[16].Filename := ExpandConstant('{sys}')+'\FIREBI~1.CPL';

SharedFileArray[17].Filename := ExpandConstant('{app}')+'\bin\msvcrt.dll';
SharedFileArray[18].Filename := ExpandConstant('{app}')+'\bin\msvcp{#msvc_version}0.dll';

if UsingWin2k then begin
  SharedFileArray[19].Filename := ExpandConstant('{app}')+'\bin\gds32.local';
  SharedFileArray[20].Filename := ExpandConstant('{app}')+'\bin\fbclient.local';
  SharedFileArray[21].Filename := ExpandConstant('{app}')+'\bin\msvcrt.local';
  SharedFileArray[22].Filename := ExpandConstant('{app}')+'\bin\msvcp{#msvc_version}0.local';
end;


end;


procedure GetSharedLibCountBeforeCopy;
var
  dw: Cardinal;
  i:  Integer;
begin
  for i:= 0 to GetArrayLength(SharedFileArray)-1 do begin
    if RegQueryDWordValue(HKEY_LOCAL_MACHINE,
          'SOFTWARE\Microsoft\Windows\CurrentVersion\SharedDLLs',SharedFileArray[i].filename, dw) then
      SharedFileArray[i].Count := dw
    else
      SharedFileArray[i].Count := 0;
  end;
end;


procedure CheckSharedLibCountAtEnd;
// If a shared file exists on disk (from a manual install perhaps?) then
// the Installer will set the SharedFile count to 2 even if no registry
// entry exists. Is it a bug, an anomaly or a WAD?
// Is it InnoSetup or the O/S?
// Anyway, let's work around it, otherwise the files will appear 'sticky'
// after an uninstall.

var
  dw: cardinal;
  i: Integer;

begin
  for i:= 0 to GetArrayLength(SharedFileArray)-1 do begin
    if RegQueryDWordValue(HKEY_LOCAL_MACHINE,
      'SOFTWARE\Microsoft\Windows\CurrentVersion\SharedDLLs',SharedFileArray[i].Filename, dw) then begin
        if (( dw - SharedFileArray[i].Count ) > 1 ) then begin
        dw := SharedFileArray[i].Count + 1 ;
        RegWriteDWordValue(HKEY_LOCAL_MACHINE,
        'SOFTWARE\Microsoft\Windows\CurrentVersion\SharedDLLs',SharedFileArray[i].Filename, dw);
      end;
    end;
  end;
end;


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
  if ( i=0 ) then
    i:=FindWindowByClassName('FB_Guard');

  if ( i<>0 ) then begin
    result := false;
    MsgBox('An existing Firebird Server is running. You must close the '+
           'application or stop the service before continuing.', mbError, MB_OK);
  end;

  InstallRootDir := '';
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

procedure DecodeVersion( VerStr: String; var VerInt: array of Integer );
var
  i,p: Integer; s: string;
begin
  VerInt := [0,0,0,0];
  i := 0;
  while ( (Length(VerStr) > 0) and (i < 4) ) do
  begin
  	p := pos('.', VerStr);
  	if p > 0 then begin
      if p = 1 then s:= '0' else s:= Copy( VerStr, 1, p - 1 );
  	  VerInt[i] := StrToInt(s);
  	  i := i + 1;
  	  VerStr := Copy( VerStr, p+1, Length(VerStr));
			end
  	else begin
  	  VerInt[i] := StrToInt( VerStr );
  	  VerStr := '';
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
  FirebirdDir := '';
	FirebirdVer := [0,0,0,0];
  RegQueryStringValue(HKEY_LOCAL_MACHINE,
    'SOFTWARE\FirebirdSQL\Firebird\Instances','DefaultInstance', FirebirdDir);
  //If nothing returned then check for the registry entry used during beta/RC phase
  if (FirebirdDir='') then
    RegQueryStringValue(HKEY_LOCAL_MACHINE,
      'SOFTWARE\FirebirdSQL\Firebird\CurrentVersion','RootDirectory', FirebirdDir);
  if (FirebirdDir<>'') then
    FirebirdVer:=GetInstalledVersion(FirebirdDir);

  Result := FirebirdDir;
end;

function GetInterBaseDir: string;
//Check if InterBase installed, get version info to global var and return root dir
var
  InterBaseDir: String;
begin
  InterBaseDir := '';
	InterBaseVer   := [0,0,0,0];
  RegQueryStringValue(HKEY_LOCAL_MACHINE,
    'SOFTWARE\Borland\InterBase\CurrentVersion','RootDirectory', InterBaseDir);
  if ( InterBaseDir <> '' ) then
    InterBaseVer:=GetInstalledVersion(InterBaseDir);

  Result := InterBaseDir;
end;

//This function tries to find an existing install of Firebird 1.5
//If it succeeds it suggests that directory for the install
//Otherwise it suggests the default for Fb 1.5
function ChooseInstallDir(Default: String): String;
var
	InterBaseRootDir,
	FirebirdRootDir: String;
begin
  //The installer likes to call this FOUR times, which makes debugging a pain,
  //so let's test for a previous call.
  if InstallRootDir = '' then begin

    // Try to find the value of "RootDirectory" in the Firebird
    // registry settings. This is either where Fb 1.0 exists or Fb 1.5
    InterBaseRootDir := GetInterBaseDir;
    FirebirdRootDir := GetFirebirdDir;

    if (FirebirdRootDir <> '') and ( FirebirdRootDir = InterBaseRootDir ) then  //Fb 1.0 must be installed so don't overwrite it.
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

    //if no existing locations found make sure we default to the default.
    if (InstallRootDir = '') then
      InstallRootDir := Default;

  end // if InstallRootDir = '' then begin

    Result := ExpandConstant(InstallRootDir);

end;

(*
function UseGuardian(Default: String): String;
begin
if ShouldProcessEntry('ServerComponent', 'UseGuardianTask')= srYes then
  Result := '1'
else
  Result := '0';
end;
*)


(*
function RunClassic: Boolean;
begin
  result := false;

  if ShouldProcessEntry('ServerComponent', 'UseClassicServerTask')= srYes then
    result := true;

end;
*)

function ServiceStartFlags(Default: String): String;
var
  ServerType: String;
  SvcParams: String;
begin
  servertype := '';
  SvcParams := '';
  if ClassicInstallChosen then
    ServerType := ' -classic '
  else
    ServerType := ' -superserver ';


  if ShouldProcessEntry('ServerComponent', 'AutoStartTask')= srYes then
    SvcParams := ' -auto '
  else
    SvcParams := ' -demand ';

  if ShouldProcessEntry('ServerComponent', 'UseGuardianTask')= srYes then
    SvcParams := SvcParams + ServerType + ' -guardian'
  else
    SvcParams := SvcParams + ServerType;

  Result := SvcParams;
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
  AppPath := ExpandConstant('{app}');
  AppPath := '"' + AppPath +'"';
  if ShouldProcessEntry('ServerComponent', 'UseGuardianTask')= srYes then begin
    Result := AppPath+'\bin\fbguard.exe';
    if ClassicInstallChosen then
      Result := Result + ' -c';
    end
  else
  if ClassicInstallChosen then
      Result := AppPath+'\bin\fb_inet_server.exe'
    else
      Result := AppPath+'\bin\fbserver.exe';
end;

procedure UpdateFirebirdConf;
// Update firebird conf. If user has deselected the guardian we should update
// firebird.conf accordingly. Otherwise we leave the file unchanged.
var
  fbconf: TStringList;
  AppPath: String;
  i, position: Integer;
begin
  fbconf := TStringList.create;
  AppPath := ExpandConstant('{app}');
  fbconf.loadfromfile(AppPath+'\firebird.conf');
  for i:=0 to fbconf.count-1 do begin
    position := pos('GuardianOption',fbconf.strings[i]);
    if (position > 0) then
      if ShouldProcessEntry('ServerComponent', 'UseGuardianTask') = srNo  then
        fbconf.strings[i] := 'GuardianOption = 0';
  end;
  fbconf.savetofile(AppPath+'\firebird.conf');
end;


procedure CurPageChanged(CurPage: Integer);
begin
  case CurPage of
    wpInfoBefore:   WizardForm.INFOBEFOREMEMO.font.name:='Courier New';
    wpSelectTasks:  WizardForm.TASKSLIST.height := WizardForm.TASKSLIST.height+20;
  end;
end;


procedure CurStepChanged(CurStep: Integer);
var
  AppStr: String;
begin
  case CurStep of
    csCopy: begin
              SetupSharedFilesArray;
              GetSharedLibCountBeforeCopy;
      end;

    csFinished: begin
      //If user has chosen to install an app and run it automatically set up the registry accordingly
      //so that the server or guardian starts evertime they login.
      if (ShouldProcessEntry('ServerComponent', 'AutoStartTask')= srYes) and
              ( ShouldProcessEntry('ServerComponent', 'UseApplicationTask')= srYes ) then begin
        AppStr := StartApp('')+' -a';
        RegWriteStringValue (HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Run', 'Firebird', AppStr);
      end;

      UpdateFirebirdConf;
      CheckSharedLibCountAtEnd;

      end;
   end;

end;

function FirebirdOneRunning: boolean;
var
  i: Integer;
begin
  result := false;

  //Look for a running copy of InterBase or Firebird 1.0.
  i:=0;
  i:=FindWindowByClassName('IB_Server') ;
  if ( i<>0 ) then
    result := true;

end;

function StartEngine: boolean;
begin
  result := not FirebirdOneRunning;
end;

(*
//Not currently in use
function RemoveThisVersion: boolean;
//check if we are still the current version before removing
var
  VersionStr: string;
begin
  result := false;
  if RegQueryStringValue(HKEY_LOCAL_MACHINE,
    'SOFTWARE\FirebirdSQL\Firebird\CurrentVersion','Version', VersionStr ) then
    if (pos(ProductVersion,VersionStr)>0) then
      result := true;
end;
*)

function ChooseUninstallIcon(Default: String): String;
var
  AppPath: String;
begin
  AppPath:=ExpandConstant('{app}');
  if ClassicInstallChosen then
    result := AppPath+'\bin\fb_inet_server.exe'
  else
    result := AppPath+'\bin\fbserver.exe';

end;

function SaveFirebirdConf: boolean;
// If we are installing over an existing installation we save the original
// config file. This is so that we don't have to worry about a conflict between
// the users configuration and the configuration expected by the installer. We
// always use the standard configuration and let the user sort out any problems
// later. We could do things the other way round, but then we would have to read
// the existing config file and dynamically set install options (like use of
// guardian). This will quickly start to get way too complicated.
var
  AppPath: String;
  NewFileName: String;
  i:  Integer;
begin

//for some reason the 3.0.6 installer wants to call this twice
//so we need to check.
if not FirebirdConfSaved then begin
  AppPath:=ExpandConstant('{app}');
  if fileexists(AppPath+'\firebird.conf') then begin
    i:=0;
	// 999 previous installs ought to be enough :-)
    while i < 999  do begin
      NewFileName := AppPath+'\firebird.conf.saved.'+IntToStr(i);
      if fileexists(NewFileName) then
        i := i + 1
      else
        break;
    end;

    filecopy( AppPath+'\firebird.conf', NewFileName, false );
    FirebirdConfSaved := true;
  end
end

//Always return true
result := true;

end;

begin
end.
