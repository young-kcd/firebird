# Microsoft Developer Studio Project File - Name="wal_classic" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=wal_classic - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wal_classic.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wal_classic.mak" CFG="wal_classic - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wal_classic - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wal_classic - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wal_classic - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\debug\wal_cs"
# PROP Intermediate_Dir "..\..\..\temp\debug\wal_cs"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../src/include" /I "../../../src/include/gen" /D "_DEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /D "_X86_" /D "DEV_BUILD" /FD /GZ /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\temp\debug\wal_cs\wal.lib"

!ELSEIF  "$(CFG)" == "wal_classic - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\release\wal_cs"
# PROP Intermediate_Dir "..\..\..\temp\release\wal_cs"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Ot /Oi /Op /Oy /Ob2 /I "../../../src/include" /I "../../../src/include/gen" /D "NDEBUG" /D "_LIB" /D "SUPERSERVER" /D "WIN32" /D "_MBCS" /D "_X86_" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /Ot /Og /Oi /Op /Oy /Ob1 /I "../../../src/include" /I "../../../src/include/gen" /D "NDEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /D "_X86_" /FD /EHc- /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\temp\release\wal_cs\wal.lib"

!ENDIF 

# Begin Target

# Name "wal_classic - Win32 Debug"
# Name "wal_classic - Win32 Release"
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
# End Group
# End Target
# End Project
