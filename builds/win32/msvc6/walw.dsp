# Microsoft Developer Studio Project File - Name="walw" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=walw - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "walw.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "walw.mak" CFG="walw - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "walw - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "walw - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "walw - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\debug\walw"
# PROP Intermediate_Dir "..\..\..\temp\debug\walw"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../src/include" /I "../../../src/include/gen" /D "_DEBUG" /D "DEV_BUILD" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_X86_" /FR /FD /GZ /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib shell32.lib comctl32.lib advapi32.lib ws2_32.lib mpr.lib version.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"..\..\..\temp\debug\firebird/bin/walw.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "walw - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\release\walw"
# PROP Intermediate_Dir "..\..\..\temp\release\walw"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Ot /Oi /Op /Oy /Ob2 /I "../../../src/include" /I "../../../src/include/gen" /D "NDEBUG" /D "SUPERSERVER" /D "WIN32" /D "_MBCS" /D "_X86_" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /Ot /Og /Oi /Op /Oy /Ob1 /I "../../../src/include" /I "../../../src/include/gen" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_X86_" /FR /FD /EHc- /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib advapi32.lib shell32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib shell32.lib comctl32.lib advapi32.lib ws2_32.lib mpr.lib version.lib /nologo /subsystem:console /incremental:no /machine:I386 /out:"..\..\..\temp\release\firebird/bin/walw.exe" /pdbtype:sept
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "walw - Win32 Debug"
# Name "walw - Win32 Release"
# Begin Group "WAL files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\wal\wal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\wal\walc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\wal\walf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\wal\walr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\wal\walw.cpp
# End Source File
# End Group
# Begin Group "Header files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\wal\wal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\wal\wal_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\wal\walc_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\wal\walf_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\wal\walr_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\wal\walw_proto.h
# End Source File
# End Group
# Begin Group "JRD files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\jrd\iberr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_ipc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_sync.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\jrn.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\llio.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\misc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\status.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\thd.cpp
# End Source File
# End Group
# End Target
# End Project
