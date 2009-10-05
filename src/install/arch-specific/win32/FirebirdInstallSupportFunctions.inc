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

 Helper functions for FB installer

 These are / ought to be fairly generic
 It makes more sense if they are independant functions - ie, they don't
 call other functions in the script and they don't need to know about the
 install script itself.

 They can call functions in the pascal script library

Function Prototypes

  function GetWindowsOS: integer;
  function IsWin64: boolean;
  function CheckWinsock2(): Boolean;
  function GetAppPath: String;
  function GetSysPath: String;
  function ReplaceLine(Filename, StringToFind, NewLine,CommentType: string): boolean;
  procedure DecodeVersion( VerStr: String; var VerInt: array of Integer );
  function CompareVersion( ver1, ver2: String ) : Integer;
  function GetInstalledVersion(BinaryFile: String): Array of Integer;
  function ConvertIBVerStrToFbVerStr( VerStr: String) : String;
  function GetRegistryEntry(RegKey, RegEntry: string): String;

 *)

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
  or Windows XP 64-Bit Edition Version 2002 (Itanium)
5.02.3790 Windows Server 2003
  or Windows XP x64 Edition (AMD64/EM64T)
  or Windows XP 64-Bit Edition Version 2003 (Itanium)
6.0.6000 Windows Vista

*)


const
  OS_WIN95 = 1;
  OS_WIN98 = 2;
  OS_WINME = 3;
  OS_WINNT = 4;
  OS_WIN2K = 5;
  OS_WINXP = 6;
  OS_WIN23 = 7;
  OS_VISTA = 8;

function IsWin64: boolean;
begin
if getenv('PROCESSOR_ARCHITECTURE') = 'AMD64' then
  result := true;

//Wow - Itanium!
if getenv('PROCESSOR_ARCHITECTURE') = 'IA64' then
  result := true;

end;

function GetWindowsOS: integer;
var
  Major: Cardinal;          // Major version number
  Minor: Cardinal;          // Minor version number
  Build: Cardinal;          // Build number
  NTPlatform: Boolean;      // True if an NT-based platform
  OS64: Boolean;            // True if O/S is 64-bit
  
begin
  Major := GetWindowsVersion shr 24;
  Minor := (GetWindowsVersion  shr 16) AND $FF;
  Build := GetWindowsVersion AND $FFFF;
  NTPlatform := UsingWinNT;
  OS64 := IsWin64;
  
  result := 0;
  
  if (Major = 4) and (Minor = 0) then
    if NTPlatform then
     result := OS_WINNT
    else
     result := OS_WIN95;

  if (Major = 4) and (Minor = 1) then
     result := OS_WIN98;

  if (Major = 4) and (Minor = 9) then
     result := OS_WINME;

  if (Major = 5) and (Minor = 0) then
     result := OS_WIN2K;

  if (Major = 5) and (Minor = 1) then
     result := OS_WINXP;

  if (Major = 5) and (Minor = 2) then
     result := OS_WIN23;

  if (Major = 6) and (Minor = 0) then
     result := OS_VISTA;

end;




const
  sWinSock2 = 'ws2_32.dll';
  sNoWinsock2 = 'Please Install Winsock 2 Update before continuing';
  sMSWinsock2Update = 'http://www.microsoft.com/windows95/downloads/contents/WUAdminTools/S_WUNetworkingTools/W95Sockets2/Default.asp';
  sWinsock2Web = 'Winsock 2 is not installed.'#13#13'Would you like to Visit the Winsock 2 Update Home Page?';

var
  Winsock2Failure:    Boolean;

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


function GetAppPath: String;
begin
  Result := ExpandConstant('{app}');
//  Result := '"' + Result +'"';
end;


function GetSysPath: String;
begin
  Result := ExpandConstant('{sys}');
//  Result := '"' + Result +'"';
end;


(*Based on InnoSetup KB example at http://www13.brinkster.com/vincenzog/isxart.asp?idart=14)*)
function ReplaceLine(Filename, StringToFind, NewLine,CommentType: string): boolean;
var
  i: Integer;
  Lines: TArrayOfString;
  CommentSize: Integer;
  ArraySize: Integer;
  FileChanged: Boolean;
  TempStr: String;
begin
  // Load textfile into string array
  LoadStringsFromFile(Filename, Lines);

  FileChanged := false;
  CommentSize := Length(CommentType);
  ArraySize := GetArrayLength(Lines)-1;

  // Search through all textlines for given text
  for i := 0 to ArraySize do begin
    // Overwrite textline when text searched for is part of it
    if Pos(StringToFind,Lines[i]) > 0 then begin
      // does this line start with a comment ?
      if CommentSize > 0 then begin
        TempStr := TrimLeft(Lines[i]);
        // only replace if line does not start with a comment
        if CompareText(Copy(TempStr,1,CommentSize),CommentType) <> 0 then begin
          Lines[i] := NewLine;
          FileChanged := true
          end
        end
      else begin
        Lines[i] := NewLine;
        FileChanged := true
      end
    end
  end;

  // Save string array to textfile (overwrite, no append!)
  if FileChanged = true then
    SaveStringsToFile(Filename, Lines, false);

  Result := FileChanged;
end;


procedure DecodeVersion( VerStr: String; var VerInt: array of Integer );
var
  i,p: Integer; s: string;
begin
  VerInt := [0,0,0,0];
  i := 0;
  while ( (Length(VerStr) > 0) and (i < 4) ) do begin
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


function CompareVersion( ver1, ver2: String; places: Integer ) : Integer;
// This function compares version strings to number of places
// ie, if we are only interested in comparing major.minor versions
// then places = 2
// return -1 if ver1 < ver2
// return  0 if ver1 = ver2
// return  1 if ver1 > ver2
//
var
  verint1, verint2: array of Integer;
  i: integer;
begin

  if places > 4 then
    places :=4;

  SetArrayLength( verint1, 4 );
  DecodeVersion( ver1, verint1 );

  SetArrayLength( verint2, 4 );
  DecodeVersion( ver2, verint2 );

  Result := 0; i := 0;
  while ( (Result = 0) and ( i < places ) ) do begin
    if verint1[i] > verint2[i] then
      Result := 1
    else
      if verint1[i] < verint2[i] then
        Result := -1
      else
        Result := 0;

    i := i + 1;
  end;

end;


function GetInstalledVersion(BinaryFile: String; var VerInt: Array of Integer): String;
var
  AString: String;
begin
  if (BinaryFile<>'') then begin
    GetVersionNumbersString( BinaryFile, Astring );
    DecodeVersion( AString, VerInt );
  end;
  result := AString;
end;


function ConvertIBVerStrToFbVerStr( VerStr: String) : String;
var
  VerInt: array of Integer;
  i: Integer;
begin
  DecodeVersion(VerStr, VerInt);
  VerInt[0]:=1;
  if VerInt[1]=2 then
    VerInt[1] := 0
  else
    if VerInt[1]=3 then
      VerInt[1]:=5;

  Result := '';
  for i:=0 to 3 do begin
    Result := Result+IntToStr(VerInt[i]);
    if i<3 then
      Result:=Result+'.'
  end;

end;

function GetRegistryEntry(RegKey, RegEntry: string): String;
begin
  result := '';
  RegQueryStringValue(HKEY_LOCAL_MACHINE, RegKey, RegEntry, Result);
end;


