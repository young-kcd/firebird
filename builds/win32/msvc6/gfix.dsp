# Microsoft Developer Studio Project File - Name="gfix" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=gfix - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gfix.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gfix.mak" CFG="gfix - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gfix - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "gfix - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gfix - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug\gfix"
# PROP Intermediate_Dir "debug\gfix"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../src/include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"debug\firebird\bin/gfix.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "gfix - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "release\gfix"
# PROP Intermediate_Dir "release\gfix"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../src/include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /GX /Ot /Oi /Op /Oy /Ob2 /I "../../../src/include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /machine:I386 /out:"release\firebird\bin/gfix.exe" /pdbtype:sept
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "gfix - Win32 Debug"
# Name "gfix - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\alice\alice.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\alice\alice_meta.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\all.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\exe.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\fb_exception.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\tdr.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\alice\alice.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\alice_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\aliceswi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\all.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\all_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\exe_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\met_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\tdr_proto.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "FromJrd"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\jrd\isc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_win32.cpp
# End Source File
# End Group
# End Target
# End Project
