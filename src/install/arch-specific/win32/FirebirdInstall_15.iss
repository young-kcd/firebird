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
;   This script has been designed to work with Inno Setup v4.0.9
;   with Inno Setup Preprocessor v. 1.1.1.267. It is available
;   as a quick start pack from here:
;     http://www.jrsoftware.org/isdl.php#qsp
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
;

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
Name: CustomInstall; Description: "Custom installation"; Flags: iscustom;

[Components]
Name: SuperServerComponent; Description: Super Server binary; Types: SuperServerInstall; Flags: exclusive;
Name: ClassicServerComponent; Description: Classic Server binary; Types: ClassicServerInstall; Flags: exclusive;
Name: ServerComponent; Description: Server components; Types: SuperServerInstall ClassicServerInstall;
Name: DevAdminComponent; Description: Developer and admin tools components; Types: SuperServerInstall ClassicServerInstall DeveloperInstall;
Name: ClientComponent; Description: Client components; Types: SuperServerInstall ClassicServerInstall DeveloperInstall ClientInstall CustomInstall; Flags: fixed disablenouninstallwarning;

[Tasks]
;Server tasks
Name: UseGuardianTask; Description: Use the &Guardian to control the server?; Components: ServerComponent; MinVersion: 4.0,4.0; Check: ConfigureFirebird;
Name: UseApplicationTask; Description: Run as an &Application?; GroupDescription: Run Firebird server as:; Components: ServerComponent; MinVersion: 4,4; Flags: exclusive; Check: ConfigureFirebird;
Name: UseServiceTask; Description: Run as a &Service?; GroupDescription: Run Firebird server as:; Components: ServerComponent; MinVersion: 0,4; Flags: exclusive; Check: ConfigureFirebird;
;;Name: UseClassicServerTask; Description: "Classic Server?"; GroupDescription: "Which Server Architecture do you want to run?"; Components: ServerComponent; MinVersion: 4,4; Flags: exclusive;
;;Name: UseSuperServerTask; Description: "Super Server"; GroupDescription: "Which Server Architecture do you want to run?"; Components: ServerComponent; MinVersion: 4,4; Flags: exclusive;
Name: AutoStartTask; Description: Start &Firebird automatically everytime you boot up?; Components: ServerComponent; MinVersion: 4,4; Check: ConfigureFirebird;
;Developer Tasks
Name: MenuGroupTask; Description: Create a Menu &Group; Components: DevAdminComponent; MinVersion: 4,4
;One for Ron
;Name: MenuGroupTask\desktopicon; Description: Create a &desktop icon; Components: ServerComponent; MinVersion: 4.0,4.0;

[Run]
;Always register Firebird
Filename: {app}\bin\instreg.exe; Parameters: "install "; StatusMsg: Updating the registry; MinVersion: 4.0,4.0; Components: ClientComponent; Flags: runminimized

;If on NT/Win2k etc and 'Install and start service' requested
Filename: {app}\bin\instsvc.exe; Parameters: "install {code:ServiceStartFlags|""""} "; StatusMsg: Setting up the service; MinVersion: 0,4.0; Components: ServerComponent; Flags: runminimized; Tasks: UseServiceTask; Check: ConfigureFirebird;
Filename: {app}\bin\instsvc.exe; Description: Start Firebird Service now?; Parameters: start; StatusMsg: Starting the server; MinVersion: 0,4.0; Components: ServerComponent; Flags: runminimized postinstall; Tasks: UseServiceTask; Check: StartEngine

;If 'start as application' requested
Filename: {code:StartApp|{app}\bin\fbserver.exe}; Description: Start Firebird now?; Parameters: -a; StatusMsg: Starting the server; MinVersion: 0,4.0; Components: ServerComponent; Flags: nowait postinstall; Tasks: UseApplicationTask; Check: StartEngine

[Registry]
;If user has chosen to start as App they may well want to start automatically. That is handled by a function below.
;Unless we set a marker here the uninstall will leave some annoying debris.
Root: HKLM; Subkey: SOFTWARE\Microsoft\Windows\CurrentVersion\Run; ValueType: string; ValueName: Firebird; ValueData: ; Flags: uninsdeletevalue; Tasks: UseApplicationTask; Check: ConfigureFirebird;

;This doesn't seem to get cleared automatically by instreg on uninstall, so lets make sure of it
Root: HKLM; Subkey: "SOFTWARE\Firebird Project"; Flags: uninsdeletekeyifempty; Components: ClientComponent DevAdminComponent ServerComponent

[Icons]
Name: {group}\Firebird Server; Filename: {app}\bin\fb_inet_server.exe; Parameters: -a; Flags: runminimized; MinVersion: 4.0,4.0; Tasks: MenuGroupTask; Check: InstallServerIcon; IconIndex: 0; Components: ClassicServerComponent; Comment: Run Firebird classic server (without guardian)
Name: {group}\Firebird Server; Filename: {app}\bin\fbserver.exe; Parameters: -a; Flags: runminimized; MinVersion: 4.0,4.0; Tasks: MenuGroupTask; Check: InstallServerIcon; IconIndex: 0; Components: SuperServerComponent; Comment: Run Firebird Superserver (without guardian)
Name: {group}\Firebird Guardian; Filename: {app}\bin\fbguard.exe; Parameters: -a; Flags: runminimized; MinVersion: 4.0,4.0; Tasks: MenuGroupTask; Check: InstallGuardianIcon; IconIndex: 1; Components: SuperServerComponent; Comment: Run Firebird Super Server (with guardian)
Name: {group}\Firebird Guardian; Filename: {app}\bin\fbguard.exe; Parameters: -c; Flags: runminimized; MinVersion: 4.0,4.0; Tasks: MenuGroupTask; Check: InstallGuardianIcon; IconIndex: 1; Components: ClassicServerComponent; Comment: Run Firebird Classic Server (with guardian)
Name: {group}\Firebird 1.5 Release Notes; Filename: {app}\doc\Firebird_v15.ReleaseNotes.pdf; MinVersion: 4.0,4.0; Tasks: MenuGroupTask; Comment: #AppName release notes. (Requires Acrobat Reader.)
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

;This file is a bit 'special'. See the InstallGds32 procedure below for more info.
Source: output\bin\gds32.dll; DestDir: {sys}; Components: ClientComponent; Flags: sharedfile; Check: InstallGds32
Source: output\bin\gds32.dll; DestDir: {app}\bin\gds32.dll.stub; Components: ClientComponent; Check: InstallGds32Stub
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
Source: output\doc\Firebird_v15.104_ReleaseNotes.pdf; DestDir: {app}\doc\Firebird_v15.ReleaseNotes.pdf; Components: DevAdminComponent; Flags: ignoreversion
;Source: firebird\install\doc_all_platforms\Firebird_v1_5_*.html; DestDir: {app}\doc; Components: DevAdminComponent;  Flags: ignoreversion;
;Note - Win9x requires 8.3 filenames for the uninsrestartdelete option to work
Source: output\system32\Firebird2Control.cpl; DestDir: {sys}; Components: SuperServerComponent; MinVersion: 0,4.0; Flags: sharedfile ignoreversion promptifolder restartreplace uninsrestartdelete
Source: output\system32\FIREBI~1.CPL; DestDir: {sys}; Components: SuperServerComponent; MinVersion: 4.0,0; Flags: sharedfile ignoreversion promptifolder restartreplace uninsrestartdelete

;TO DO note
;We could add in the debug files here, if #define debug has been set above.
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

//Var
//  ProductVersion = '1.5.0';

{To Do

--remove old, incorrect registry entries that exist from RC installs

}


Var
  InstallRootDir: String;
  FirebirdConfSaved: boolean;


#include "FirebirdInstallSupportFunctions.inc"
#include "FirebirdInstallEnvironmentChecks.inc"



function SummarizeInstalledProducts: String;
var
  InstallSummaryArray: TArrayofString;
  product: Integer;
  i: Integer;
  StatusDescription: String;
  InstallSummary: String;
  prodstr: String;
begin

//do nothing gracefully if we are called by accident.
if ProductsInstalledCount = 0 then
  exit;

if ProductsInstalledCount = 1 then
  prodstr := 'version has'
else
  prodstr := 'versions have';

SetArrayLength(InstallSummaryArray,ProductsInstalledCount);
for product := 0 to MaxProdInstalled -1 do begin
  if (ProductsInstalledArray[product].InstallType <> NotInstalled) then begin
      InstallSummaryArray[i] := Format1(ProductsInstalledArray[product].Description,
            ProductsInstalledArray[product].ActualVersion);

    if (ProductsInstalledArray[product].ServerVersion <> '') then begin
      if ((ProductsInstalledArray[product].InstallType AND ClassicServerInstall) = ClassicServerInstall) then
        InstallSummaryArray[i] := InstallSummaryArray[i] + ' Classic Server install.'
      else
        InstallSummaryArray[i] := InstallSummaryArray[i] + ' Super Server install.'
      end
    else begin
      if (ProductsInstalledArray[product].GBAKVersion <> '') then
        InstallSummaryArray[i] := InstallSummaryArray[i] + ' Admin and Dev. Tools install.'
      else
        InstallSummaryArray[i] := InstallSummaryArray[i] + ' Minimal client install. '
    end;

    if ((ProductsInstalledArray[product].InstallType AND BrokenInstall) = BrokenInstall) then
      InstallSummaryArray[i] := InstallSummaryArray[i]
      + #13 + '   (Install appears broken due to version string mismatch.)'
    else
      InstallSummaryArray[i] := InstallSummaryArray[i]
      + #13 + '   (Installation appears to be correct.)'
    ;

    i:= i+1;
  end;
end;

for i:=0 to ProductsInstalledCount-1 do
  InstallSummary := InstallSummary + InstallSummaryArray[i] + #13;

//If FB2 is installed
If ((ProductsInstalled AND FB2) = FB2) then
      InstallSummary := InstallSummary
      +#13 + 'This version of Firebird will become the default install.'
      +#13 + 'Also, take note that installing this version of Firebird '
      +#13 + 'when a later version is already installed may lead to '
      +#13 + 'unpredictable results. '
      +#13;

StatusDescription := Format2('Pre-installation analysis indicates that %s existing Firebird or Interbase %s been found.',
        IntToStr(ProductsInstalledCount), prodstr);
Result := StatusDescription
    +#13 + InstallSummary
    +#13 + ' If you continue with this installation Firebird will be installed but not configured.'
    +#13 + ' You will have to complete installation manually.'
    +#13
    +#13 + ' Do you want to CANCEL this installation?';
end;

function AnalysisAssessment: boolean;
var
  MsgText: String;
  MsgResult: Integer;
begin
  result := false;

  //We've got all this information. What do we do with it?

  if ProductsInstalledCount = 0 then begin
    result := true;
    exit;
  end;
  
  //If Fb15 RC is installed then we can install over it.
  //unless we find the server running.
  if (ProductsInstalledCount = 1) AND
    (((ProductsInstalled AND FB15) = FB15) OR
     ((ProductsInstalled AND FB15RC) = FB15RC)) then
    if ( FirebirdOneFiveRunning ) then begin
      result := false;
      MsgBox('An existing Firebird 1.5 Server is running. You must close the '+
             'application or stop the service before continuing.', mbError, MB_OK);
      exit;
      end
    else begin
      result := true;
      exit;
    end
  ;

  if ForceInstall then begin
    result := true;
    exit;
  end;

  //Otherwise, show user the analysis report.
  MsgText := SummarizeInstalledProducts;
  MsgResult := MsgBox(MsgText,mbConfirmation,MB_YESNO);
  if (MsgResult = IDNO ) then
    result := true;
    //but we don't configure
    If ((InstallAndConfigure AND Configure) = Configure) then
      InstallAndConfigure := InstallAndConfigure - Configure;
end;

function InitializeSetup(): Boolean;
var
  i: Integer;
  CommandLine: String;
begin

  result := true;

  if not CheckWinsock2 then begin
    result := False;
    exit;
  end

  CommandLine:=GetCmdTail;
  if pos('FORCE',Uppercase(CommandLine))>0 then
    ForceInstall:=True;
    
  //By default we want to install and confugure,
  //unless subsequent analysis suggest otherwise.
  InstallAndConfigure := Install + Configure;

  InitExistingInstallRecords;
  AnalyzeEnvironment;
  result := AnalysisAssessment;

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
  if (InstallRootDir = '') then begin

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
begin
  if ShouldProcessEntry('ServerComponent', 'UseGuardianTask')= srYes then begin
    Result := GetAppPath+'\bin\fbguard.exe';
    if ClassicInstallChosen then
      Result := Result + ' -c';
    end
  else
  if ClassicInstallChosen then
      Result := GetAppPath+'\bin\fb_inet_server.exe'
    else
      Result := GetAppPath+'\bin\fbserver.exe';
end;

procedure UpdateFirebirdConf;
// Update firebird conf. If user has deselected the guardian we should update
// firebird.conf accordingly. Otherwise we leave the file unchanged.
var
  fbconf: TArrayOfString;
  i, position: Integer;
  ArraySize: Integer;
  FileChanged: boolean;
begin
  //There is no simple, foolproof and futureproof way to check whether
  //we are doing a server install, so the easiest way is to see if a
  //firebird.conf exists. If it doesn't then we don't care.
  if FileExists(GetAppPath+'\firebird.conf') then begin
    if ShouldProcessEntry('ServerComponent', 'UseGuardianTask') = srNo  then
      ReplaceLine(GetAppPath+'\firebird.conf','GuardianOption','GuardianOption = 0','#');
  end;
end;


procedure CurPageChanged(CurPage: Integer);
begin
  case CurPage of
    wpInfoBefore:   WizardForm.INFOBEFOREMEMO.font.name:='Courier New';
    wpInfoBefore:   WizardForm.INFOAFTERMEMO.font.name:='Courier New';
    wpSelectTasks:  WizardForm.TASKSLIST.height := WizardForm.TASKSLIST.height+20;
    wpFinished:     ; // Create some links to Firebird and IBP here?.
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

function StartEngine: boolean;
begin
  if ConfigureFirebird then
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
begin
  if ClassicInstallChosen then
    result := GetAppPath+'\bin\fb_inet_server.exe'
  else
    result := GetAppPath+'\bin\fbserver.exe';

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
  NewFileName: String;
  i:  Integer;
begin
//for some reason the 3.0.6 installer wants to call this twice
//so we need to check.
if not FirebirdConfSaved then begin
  if fileexists(GetAppPath+'\firebird.conf') then begin
    i:=0;
	// 999 previous installs ought to be enough :-)
    while i < 999  do begin
      NewFileName := GetAppPath+'\firebird.conf.saved.'+IntToStr(i);
      if fileexists(NewFileName) then
        i := i + 1
      else
        break;
    end;

    filecopy( GetAppPath+'\firebird.conf', NewFileName, false );
    FirebirdConfSaved := true;
  end
end

//Always return true
result := true;
end;



begin
end.



