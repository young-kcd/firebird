# Microsoft Developer Studio Project File - Name="ipserver" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ipserver - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ipserver.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ipserver.mak" CFG="ipserver - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ipserver - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ipserver - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ipserver - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug\ipserver"
# PROP Intermediate_Dir "debug\ipserver"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../src/include" /I "../../../src/include/gen" /D "_DEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /D "_X86_" /D "DEV_BUILD" /D "SUPERSERVER" /FD /GZ /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "ipserver - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "release\ipserver"
# PROP Intermediate_Dir "release\ipserver"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Ot /Oi /Op /Oy /Ob2 /I "../../../src/include" /I "../../../src/include/gen" /D "NDEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /D "_X86_" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /Ot /Og /Oi /Op /Oy /Ob1 /I "../../../src/include" /I "../../../src/include/gen" /D "NDEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /D "_X86_" /D "SUPERSERVER" /FD /EHc- /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "ipserver - Win32 Debug"
# Name "ipserver - Win32 Release"
# Begin Group "IPSERVER files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\ipserver\alli.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\ipserver\ipcevent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\ipserver\ipclient.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\ipserver\ipserver.cpp
# End Source File
# End Group
# Begin Group "Header files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\ipserver\alli_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\ipserver\blk.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\ipserver\ipapi_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\ipserver\ipc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\ipserver\ipcli_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\ipserver\ipevt_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\ipserver\ips.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\ipserver\ipsrv_proto.h
# End Source File
# End Group
# End Target
# End Project
