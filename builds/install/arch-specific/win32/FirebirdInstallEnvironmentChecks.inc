(*  Initial Developer's Public License.
  The contents of this file are subject to the  Initial Developer's Public
  License Version 1.0 (the "License"). You may not use this file except
  in compliance with the License. You may obtain a copy of the License at
    http://www.ibphoenix.com?a=ibphoenix&page=ibp_idpl
  Software distributed under the License is distributed on an "AS IS" basis,
  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
  for the specific language governing rights and limitations under the
  License.

  The Original Code is copyright 2001-2003 Paul Reeves for IBPhoenix.

  The Initial Developer of the Original Code is Paul Reeves for IBPhoenix.

  All Rights Reserved.

  This file contains a whole bunch of functions that
  check the registry and see what versions of firebird or interbase
  are already installed.

  This stuff _is_ firebird/interbase specific and some of it is entirely dependant
  upon parts of the InnoSetup install script.

  Here is a list of functions available in this script:

    function ClassicInstallChosen: Boolean;

    procedure SetupSharedFilesArray;
    procedure GetSharedLibCountBeforeCopy;
    procedure CheckSharedLibCountAtEnd;

    function GetFirebirdDir: string;
    function GetInterBaseDir: string;
    function TestForWorkingInstalls: boolean;

    function FirebirdOneRunning: boolean;

    function InstallCPLApplet: boolean;
    function ShowInstallCPLAppletTask: boolean;
    function CopyGds32: boolean;
    function ShowCopyGds32Task: boolean;
    function CopyFbClientLib: boolean;
    function ShowCopyFbClientLibTask: boolean;


 *)


//Registry keys for Firebird and InterBase
Const
  //All InterBase and Firebird 1.0.n except IB5.n
  IBRegKey            = 'SOFTWARE\Borland\InterBase\CurrentVersion';
  //IB5.n
  IB5RegKey           = 'SOFTWARE\InterBase Corp\InterBase\CurrentVersion';
  //Fb15 RC
  FB15RCKey           = 'SOFTWARE\FirebirdSQL\Firebird\CurrentVersion';
  FB15RCKeyRoot       = 'SOFTWARE\FirebirdSQL';

  //All IB, Fb 1.0  and Fb 1.5RC's use RootDirectory entry
  LegacyRegPathEntry  = 'RootDirectory';

  //Firebird 1.5 and beyond
  FB2RegKey       = 'SOFTWARE\Firebird Project\Firebird Server\Instances';

  FBRegPathEntry  = 'DefaultInstance';    //Stores path to root

  IB4MessageFile  = 'interbas.msg';
  IBMessageFile   = 'interbase.msg';      //IB5, IB6, IB7 and Fb 1.0
  FBMessageFile   = 'firebird.msg';       //Fb2 codebase

  IBDesc = 'InterBase %s ';
  FBDesc = 'Firebird %s ';



Const
  //Install Types
  NotInstalled          = 0;
  ClientInstall         = 1;
  AdminInstall          = 2;
  SuperServerInstall    = 4;
  ClassicServerInstall  = 8;
  BrokenInstall         = 32;   //version or component mismatch found, so mark broken

  //Possible product installs
  IB4Install    = 0;
  IB5Install    = 1;
  IB6Install    = 2;
  IB65Install   = 3;
  IB7Install    = 4;
  FB1Install    = 5;
  FB15RCInstall = 6;
  FB15Install   = 7;
  FB2Install    = 8;            //All Fb 1.6 and beyond
  MaxProdInstalled = FB2Install;

  //ProductsInstalled
  IB4     = $0001;
  IB5     = $0002;
  IB6     = $0004;
  IB65    = $0008;
  IB7     = $0010;
  FB1     = $0020;
  FB15RC  = $0040;
  FB15    = $0080;
  FB2     = $0100;

Const
  Install   = 1;
  Configure = 2;

Var
  ProductsInstalled: Integer;
  ProductsInstalledCount: Integer;
  InstallAndConfigure: Integer;

Type
  TProduct = record
    ProductID:      Integer;
    Description:    String;
    RegKey:         String;
    RegEntry:       String;
    RegVersion:     String;
    MessageFile:    String;
    Path:           String;
    ClientVersion:  String;
    GBAKVersion:    String;
    ServerVersion:  String;
    InstallType:    Integer;
    ActualVersion:  String;
    FirebirdVersion:String;
  end;


Var
  ProductsInstalledArray: Array of TProduct;

procedure InitExistingInstallRecords;
var
  product: Integer;
begin
  SetArrayLength(ProductsInstalledArray,MaxProdInstalled + 1);
  for product := 0 to MaxProdInstalled do begin

     ProductsInstalledArray[product].ProductID := product;

    case product of

      IB4Install: begin
        ProductsInstalledArray[product].Description := IBDesc;
        ProductsInstalledArray[product].RegKey := IBRegKey;
        ProductsInstalledArray[product].RegEntry := LegacyRegPathEntry;
        ProductsInstalledArray[product].MessageFile := IB4MessageFile;
      end;

      IB5Install: begin
        ProductsInstalledArray[product].Description := IBDesc;
        ProductsInstalledArray[product].RegKey := IB5RegKey;
        ProductsInstalledArray[product].RegEntry := LegacyRegPathEntry;
        ProductsInstalledArray[product].MessageFile := IBMessageFile;
      end;

      IB6Install: begin
        ProductsInstalledArray[product].Description := IBDesc;
        ProductsInstalledArray[product].RegKey := IBRegKey;
        ProductsInstalledArray[product].RegEntry := LegacyRegPathEntry;
        ProductsInstalledArray[product].MessageFile := IBMessageFile;
      end;

      IB65Install: begin
        ProductsInstalledArray[product].Description := IBDesc;
        ProductsInstalledArray[product].RegKey := IBRegKey;
        ProductsInstalledArray[product].RegEntry := LegacyRegPathEntry;
        ProductsInstalledArray[product].MessageFile := IBMessageFile;
      end;

      IB7Install: begin
        ProductsInstalledArray[product].Description := IBDesc;
        ProductsInstalledArray[product].RegKey := IBRegKey;
        ProductsInstalledArray[product].RegEntry := LegacyRegPathEntry;
        ProductsInstalledArray[product].MessageFile := IBMessageFile;
      end;

      FB1Install: begin
        ProductsInstalledArray[product].Description := FBDesc;
        ProductsInstalledArray[product].RegKey := IBRegKey;
        ProductsInstalledArray[product].RegEntry := LegacyRegPathEntry;
        ProductsInstalledArray[product].MessageFile := IBMessageFile;
      end;

      FB15RCInstall: begin
        ProductsInstalledArray[product].Description := FBDesc;
        ProductsInstalledArray[product].RegKey := FB15RCKey;
        ProductsInstalledArray[product].RegEntry := LegacyRegPathEntry;
        ProductsInstalledArray[product].MessageFile := FBMessageFile;
      end;

      FB15Install: begin
        ProductsInstalledArray[product].Description := FBDesc;
        ProductsInstalledArray[product].RegKey := FB2RegKey;
        ProductsInstalledArray[product].RegEntry := FBRegPathEntry;
        ProductsInstalledArray[product].MessageFile := FBMessageFile;
      end;

      FB2Install: begin
        ProductsInstalledArray[product].Description := FBDesc;
        ProductsInstalledArray[product].RegKey := FB2RegKey;
        ProductsInstalledArray[product].RegEntry := FBRegPathEntry;
        ProductsInstalledArray[product].MessageFile := FBMessageFile;
      end;

    end; //case
  end;  //for
end; //function


function GetRegistryEntry(RegKey, RegEntry: string): String;
begin
  result := '';
  RegQueryStringValue(HKEY_LOCAL_MACHINE, RegKey, RegEntry, Result);
end;


Procedure AnalyzeEnvironment;
var
  product: Integer;
  gds32VersionString: String;
  VerInt: Array of Integer;
  BoolOne, BoolTwo, BoolEval: Boolean;
  EvalOne, EvalTwo: Integer;
begin

  ProductsInstalled := 0;
  ProductsInstalledCount := 0;

  //Test for gds32 version in <sys>
  if FileExists(GetSysPath+'\gds32.dll') then begin
    gds32VersionString := GetInstalledVersion(GetSysPath+'\gds32.dll',VerInt);
  end;

  for product := 0 to MaxProdInstalled -1 do begin

    ProductsInstalledArray[product].Path := GetRegistryEntry(
        ProductsInstalledArray[product].RegKey, ProductsInstalledArray[product].RegEntry);

    ProductsInstalledArray[product].RegVersion := GetRegistryEntry(
        ProductsInstalledArray[product].RegKey, 'Version');


    if FileExists(ProductsInstalledArray[product].Path + '\bin\fbclient.dll') then
      ProductsInstalledArray[product].ClientVersion := GetInstalledVersion(
        ProductsInstalledArray[product].Path + '\bin\fbclient.dll',VerInt)
      else
        ProductsInstalledArray[product].ClientVersion := gds32VersionString;

    If (ProductsInstalledArray[product].Path<>'') AND (ProductsInstalledArray[product].ClientVersion <> '') AND
      (FileExists(ProductsInstalledArray[product].Path+'\'+ProductsInstalledArray[product].MessageFile)) then
        ProductsInstalledArray[product].InstallType := ProductsInstalledArray[product].InstallType + ClientInstall
    else
      //The minimum requirements for a working client don't exist, so ignore this product.
      Continue;


    if ((ProductsInstalledArray[product].InstallType AND ClientInstall) = ClientInstall) then begin

      GetVersionNumbersString( ProductsInstalledArray[product].Path+'\bin\gbak.exe',
          ProductsInstalledArray[product].GBAKVersion);
      If ProductsInstalledArray[product].GBAKVersion <> '' then begin
        ProductsInstalledArray[product].ActualVersion:=ProductsInstalledArray[product].GBAKVersion;
        ProductsInstalledArray[product].InstallType := ProductsInstalledArray[product].InstallType + AdminInstall;
      end;

      if FileExists(ProductsInstalledArray[product].Path+'\bin\fb_inet_server.exe') then begin
        GetVersionNumbersString( ProductsInstalledArray[product].Path+'\bin\fb_inet_server.exe',
          ProductsInstalledArray[product].ServerVersion);
        If ProductsInstalledArray[product].ServerVersion <> '' then begin
          ProductsInstalledArray[product].ActualVersion:=ProductsInstalledArray[product].ServerVersion;
          ProductsInstalledArray[product].InstallType := ProductsInstalledArray[product].InstallType + ClassicServerInstall;
        end;
      end;

      if FileExists(ProductsInstalledArray[product].Path+'\bin\fbserver.exe') then begin
        GetVersionNumbersString( ProductsInstalledArray[product].Path+'\bin\fbserver.exe',
          ProductsInstalledArray[product].ServerVersion);
        If ProductsInstalledArray[product].ServerVersion <> '' then begin
          ProductsInstalledArray[product].ActualVersion:=ProductsInstalledArray[product].ServerVersion;
          ProductsInstalledArray[product].InstallType := ProductsInstalledArray[product].InstallType + SuperServerInstall;
        end;
      end;

      if FileExists(ProductsInstalledArray[product].Path+'\bin\ibserver.exe') then begin
        GetVersionNumbersString( ProductsInstalledArray[product].Path+'\bin\ibserver.exe',
          ProductsInstalledArray[product].ServerVersion);
        If ProductsInstalledArray[product].ServerVersion <> '' then begin
          ProductsInstalledArray[product].ActualVersion:=ProductsInstalledArray[product].ServerVersion;
          ProductsInstalledArray[product].InstallType := ProductsInstalledArray[product].InstallType + SuperServerInstall;
        end;
      end;

      //Now, resolve version numbers. If the version number
      //  Likely values for installed versions of Firebird are:
      //  [6,0,n,n]     InterBase 6.0
      //  [6,2,0,nnn]   Firebird 1.0.0
      //  [6,2,2,nnn]   Firebird 1.0.2
      //  [6,2,3,nnn]   Firebird 1.0.3
      //  [6,5,n,n]     InterBase 6.5
      //  [6,3,0,nnnn]  Firebird 1.5.0
      //  [6,3,0,nnnn]  Firebird 2.0.0
      //  [7,n,n,n]     InterBase 7

      case product of
        IB4Install: begin
          //If we get here we must have an install, because the message file is unique to 4.n
          ProductsInstalled := ProductsInstalled + IB4;
          ProductsInstalledCount := ProductsInstalledCount + 1;
          //check to see if the client library matches the server version installed.
          if CompareVersion(ProductsInstalledArray[product].ActualVersion, '4.0.0.0',1) <> 0 then
            ProductsInstalledArray[product].InstallType := ProductsInstalledArray[product].InstallType + BrokenInstall;

        end;

        IB5Install: begin
          //If we get here we must have an install, because the registry key is unique to 5.n
          if ((ProductsInstalledArray[product].InstallType AND ClientInstall)= ClientInstall) then begin
            ProductsInstalled := ProductsInstalled + IB5;
            ProductsInstalledCount := ProductsInstalledCount + 1;
          end;
          //check to see if the client library matches the server version installed.
          if CompareVersion(ProductsInstalledArray[product].ActualVersion, '5.0.0.0',1) <> 0 then
            ProductsInstalledArray[product].InstallType := ProductsInstalledArray[product].InstallType + BrokenInstall;
        end;

        IB6Install: begin
          //If we get here we have ambiguity with other versions of InterBase and Firebird
          if ( pos('InterBase',ProductsInstalledArray[product].RegVersion) > 0 ) then begin
            if CompareVersion(ProductsInstalledArray[product].ClientVersion, '6.0.0.0',2) <> 0 then
              ProductsInstalledArray[product].InstallType := ProductsInstalledArray[product].InstallType + BrokenInstall;
            if ((ProductsInstalledArray[product].InstallType AND ClientInstall)= ClientInstall) then begin
              ProductsInstalled := ProductsInstalled + IB6;
              ProductsInstalledCount := ProductsInstalledCount + 1;
              end;
            end
          else
            ProductsInstalledArray[product].InstallType := NotInstalled;
        end;

        IB65Install: begin
          //If we get here we have ambiguity with other versions of InterBase and Firebird
          if ( pos('InterBase',ProductsInstalledArray[product].RegVersion) > 0 ) then begin
            if CompareVersion(ProductsInstalledArray[product].ClientVersion, '6.5.0.0',2) <> 0 then
              ProductsInstalledArray[product].InstallType := ProductsInstalledArray[product].InstallType + BrokenInstall;
            if ((ProductsInstalledArray[product].InstallType AND ClientInstall)= ClientInstall) then begin
              ProductsInstalled := ProductsInstalled + IB65;
              ProductsInstalledCount := ProductsInstalledCount + 1;
              end;
            end
          else
            ProductsInstalledArray[product].InstallType := NotInstalled;
        end;

        IB7Install: begin
          //If we get here we have ambiguity with other versions of InterBase and Firebird
          if ( pos('InterBase',ProductsInstalledArray[product].RegVersion) > 0 ) then begin
            if CompareVersion(ProductsInstalledArray[product].ClientVersion, '7.0.0.0',1) <> 0 then
            ProductsInstalledArray[product].InstallType := ProductsInstalledArray[product].InstallType + BrokenInstall;
            if ((ProductsInstalledArray[product].InstallType AND ClientInstall)= ClientInstall) then begin
              ProductsInstalled := ProductsInstalled + IB7;
              ProductsInstalledCount := ProductsInstalledCount + 1;
              end;
            end
          else
            ProductsInstalledArray[product].InstallType := NotInstalled;
        end;

        FB1Install: begin
          if ( pos('Firebird',ProductsInstalledArray[product].RegVersion) > 0 ) then begin
            if CompareVersion(ProductsInstalledArray[product].ClientVersion, '6.2.0.0',2) <> 0 then
              ProductsInstalledArray[product].InstallType := ProductsInstalledArray[product].InstallType + BrokenInstall;
            if ((ProductsInstalledArray[product].InstallType AND ClientInstall)= ClientInstall) then begin
              ProductsInstalled := ProductsInstalled + FB1;
              ProductsInstalledCount := ProductsInstalledCount + 1;
              ProductsInstalledArray[product].ActualVersion := ConvertIBVerStrToFbVerStr(ProductsInstalledArray[product].ActualVersion);
              end;
            end
          else
            ProductsInstalledArray[product].InstallType := NotInstalled;
        end;

        FB15RCInstall: begin
          if CompareVersion(ProductsInstalledArray[product].ClientVersion, '1.5.0.0',2) <> 0 then
            ProductsInstalledArray[product].InstallType := ProductsInstalledArray[product].InstallType + BrokenInstall;
          if ((ProductsInstalledArray[product].InstallType AND ClientInstall)= ClientInstall) then begin
            ProductsInstalled := ProductsInstalled + FB15RC;
            ProductsInstalledCount := ProductsInstalledCount + 1;
          end;
        end;

        FB15Install: begin
          if CompareVersion(ProductsInstalledArray[product].ClientVersion, '1.5.0.0',2) <> 0 then
            ProductsInstalledArray[product].InstallType := ProductsInstalledArray[product].InstallType + BrokenInstall;
          if ((ProductsInstalledArray[product].InstallType AND ClientInstall)= ClientInstall) then begin
            ProductsInstalled := ProductsInstalled + FB15;
            ProductsInstalledCount := ProductsInstalledCount + 1;
          end;

        end;

        FB2Install: begin
          //We don't know (yet) what we should be comparing against here, ClientVersion
          //should be equal or greater than 2.0.0.0
          if (CompareVersion(ProductsInstalledArray[product].ClientVersion, '2.0.0.0',2) < 0) then
            ProductsInstalledArray[product].InstallType := ProductsInstalledArray[product].InstallType + BrokenInstall;
          if ((ProductsInstalledArray[product].InstallType AND ClientInstall)= ClientInstall) then begin
            ProductsInstalled := ProductsInstalled + FB2;
            ProductsInstalledCount := ProductsInstalledCount + 1;
          end;
        end;

      end;//case


      if ((ProductsInstalledArray[product].InstallType <> NotInstalled) AND
        //Check that we haven't already flagged the install as broken.
        ((ProductsInstalledArray[product].InstallType AND BrokenInstall)<>BrokenInstall)) then begin

        //Now test that the version strings match!
        if (CompareStr(ProductsInstalledArray[product].ClientVersion, ProductsInstalledArray[product].GBAKVersion)<> 0) then
          ProductsInstalledArray[product].InstallType := ProductsInstalledArray[product].InstallType + BrokenInstall
        else
          if (CompareStr(ProductsInstalledArray[product].ClientVersion, ProductsInstalledArray[product].ServerVersion )<> 0) then
          ProductsInstalledArray[product].InstallType := ProductsInstalledArray[product].InstallType + BrokenInstall;

      end;


    end; //if ((ProductsInstalledArray[product].InstallType AND ClientInstall)= ClientInstall) then begin
  end; //for
end;


Var
  InterBaseVer: Array of Integer;
  FirebirdVer: Array of Integer;


function ClassicInstallChosen: Boolean;
var
  SelectedComponents: String;
begin
  result := false;

  SelectedComponents := WizardSelectedComponents(false);
  if pos(lowercase('ClassicServerComponent'),SelectedComponents) >0 then
    result := true;
end;



/////=======================================



Type
  TSharedFileArrayRecord = record
    Filename: String;
    Count: Integer;
  end;

var
  SharedFileArray: Array of TSharedFileArrayRecord;



procedure SetupSharedFilesArray;
//All shared files go in this list. Use
// find /n "sharedfile" FirebirdInstall_15.iss
//to list them in the order they appear in the setup script
begin
SetArrayLength(SharedFileArray,24);

SharedFileArray[0].Filename := ExpandConstant('{app}')+'\IPLicense.txt';
SharedFileArray[1].Filename := ExpandConstant('{app}')+'\IDPLicense.txt';
SharedFileArray[2].Filename := ExpandConstant('{app}')+'\firebird.msg';
SharedFileArray[3].Filename := ExpandConstant('{app}')+'\bin\gbak.exe';
SharedFileArray[4].Filename := ExpandConstant('{app}')+'\bin\gfix.exe';
SharedFileArray[5].Filename := ExpandConstant('{app}')+'\bin\gsec.exe';
SharedFileArray[6].Filename := ExpandConstant('{app}')+'\bin\gsplit.exe';
SharedFileArray[7].Filename := ExpandConstant('{app}')+'\bin\gstat.exe';
SharedFileArray[8].Filename := ExpandConstant('{app}')+'\bin\fbguard.exe';
SharedFileArray[9].Filename := ExpandConstant('{app}')+'\bin\fb_lock_print.exe';

if ClassicInstallChosen then
  SharedFileArray[10].Filename := ExpandConstant('{app}')+'\bin\fb_inet_server.exe'
else
  SharedFileArray[10].Filename := ExpandConstant('{app}')+'\bin\fbserver.exe';

SharedFileArray[11].Filename := ExpandConstant('{app}')+'\bin\ib_util.dll';
SharedFileArray[12].Filename := ExpandConstant('{app}')+'\bin\instclient.exe';
SharedFileArray[13].Filename := ExpandConstant('{app}')+'\bin\instreg.exe';
SharedFileArray[14].Filename := ExpandConstant('{app}')+'\bin\instsvc.exe';

SharedFileArray[15].Filename := ExpandConstant('{sys}')+'\gds32.dll';
SharedFileArray[16].Filename := ExpandConstant('{sys}')+'\fbclient.dll';

SharedFileArray[17].Filename := ExpandConstant('{app}')+'\bin\fbclient.dll';

SharedFileArray[18].Filename := ExpandConstant('{app}')+'\bin\msvcrt.dll';
SharedFileArray[19].Filename := ExpandConstant('{app}')+'\bin\msvcp{#msvc_version}0.dll';

SharedFileArray[20].Filename := ExpandConstant('{app}')+'\bin\fbintl.dll';

SharedFileArray[21].Filename := ExpandConstant('{app}')+'\UDF\ib_udf.dll';
SharedFileArray[22].Filename := ExpandConstant('{app}')+'\UDF\fbudf.dll';


if UsingWinNT then
  SharedFileArray[23].Filename := ExpandConstant('{sys}')+'\Firebird2Control.cpl'
else
  SharedFileArray[23].Filename := ExpandConstant('{sys}')+'\FIREBI~1.CPL';



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


///===================================================

function GetFirebirdDir: string;
//Check if Firebird installed, get version info to global var and return root dir
var
	FirebirdDir: String;
begin
  FirebirdDir := '';
	FirebirdVer := [0,0,0,0];
  RegQueryStringValue(HKEY_LOCAL_MACHINE,
    'SOFTWARE\Firebird Project\Firebird Server\Instances','DefaultInstance', FirebirdDir);
  //If nothing returned then check for the registry entry used during beta/RC phase
  if (FirebirdDir='') then
    RegQueryStringValue(HKEY_LOCAL_MACHINE,
      'SOFTWARE\FirebirdSQL\Firebird\CurrentVersion','RootDirectory', FirebirdDir);
  if (FirebirdDir<>'') then
    GetInstalledVersion(FirebirdDir+'\bin\gbak.exe', FirebirdVer);

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
    GetInstalledVersion(InterBaseDir+'\bin\gbak.exe',InterBaseVer);

  Result := InterBaseDir;
end;


function ConfigureFirebird: boolean;
begin
  result := (InstallAndConfigure AND Configure) = Configure;
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

//Detect any 1.5 or 2.0 server running with default ClassName or mutex
function FirebirdDefaultServerRunning: boolean;
var
  Handle: Integer;
  mutex_found: boolean;
begin
  result := False;
  //Look for a running version of Firebird 1.5 or later
  Handle:=FindWindowByClassName('FB_Disabled');
  if ( Handle=0 ) then
    Handle:=FindWindowByClassName('FB_Server');
  if ( Handle=0 ) then
    Handle:=FindWindowByClassName('FB_Guard');

  if (Handle>0) then
     result := True
  else begin
    mutex_found := CheckForMutexes('FirebirdGuardianMutex,FirebirdServerMutex');
    if mutex_found then
      result := true;
  end;

end;


function InstallCPLApplet: boolean;
begin
  result := False;
  if ( (ConfigureFirebird) AND (not NoCPL) ) then
     result := (ShouldProcessEntry('SuperServerComponent', 'InstallCPLAppletTask') = srYes) ;
end;


function ShowInstallCPLAppletTask: boolean;
begin
  //If NOCPL is on the command line then don't offer the task in UI mode.
  result := ((not NoCPL) and ConfigureFirebird);
end;



function CopyGds32: boolean;
begin
  //Note that we invert the value of NOLEGACYCLIENT so we provide the
  //correct answer to the question 'Do we copy GDS32 to <sys>' which is
  //the default behaviour.
  result := False;
  if ConfigureFirebird then begin
    //If one of these is false then either the commandline switch was passed
    //or the user unchecked the Copy client as GDS32 box
    result := ( (not NoLegacyClient) AND (ShouldProcessEntry('ClientComponent', 'CopyFbClientAsGds32Task')= srYes) );
  end;
end;


function ShowCopyGds32Task: boolean;
begin
  //If NOGDS32 is on the command line then don't offer the task in UI mode.
  result := ((not NoLegacyClient) and ConfigureFirebird);
end;


function CopyFbClientLib: boolean;
begin
//Note that the default for this is the opposite to CopyGds32.
  result := ( (CopyFbClient) OR (ShouldProcessEntry('ClientComponent', 'CopyFbClientToSysTask')= srYes) );
end;


function ShowCopyFbClientLibTask: boolean;
//See note for ShowCopyGds32Task.
begin
  result := False;
  if ConfigureFirebird then
    result := ((not CopyFbClient) and ConfigureFirebird);
end;

