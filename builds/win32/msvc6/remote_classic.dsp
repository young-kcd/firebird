# Microsoft Developer Studio Project File - Name="remote_classic" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=remote_classic - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "remote_classic.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "remote_classic.mak" CFG="remote_classic - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "remote_classic - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "remote_classic - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "remote_classic - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\debug\remote_cs"
# PROP Intermediate_Dir "..\..\..\temp\debug\remote_cs"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../src/include" /I "../../../src/include/gen" /D "_DEBUG" /D "_LIB" /D "SERVER_SHUTDOWN" /D "WIN32" /D "_MBCS" /D "_X86_" /D "DEV_BUILD" /FD /GZ /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\temp\debug\remote_cs\remote.lib"

!ELSEIF  "$(CFG)" == "remote_classic - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\release\remote_cs"
# PROP Intermediate_Dir "..\..\..\temp\release\remote_cs"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Ot /Oi /Op /Oy /Ob2 /I "../../../src/include" /I "../../../src/include/gen" /D "_DEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /D "NOMSG" /D "_X86_" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /Ot /Og /Oi /Op /Oy /Ob1 /I "../../../src/include" /I "../../../src/include/gen" /D "NDEBUG" /D "_LIB" /D "SERVER_SHUTDOWN" /D "WIN32" /D "_MBCS" /D "_X86_" /FD /EHc- /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\temp\release\remote_cs\remote.lib"

!ENDIF 

# Begin Target

# Name "remote_classic - Win32 Debug"
# Name "remote_classic - Win32 Release"
# Begin Group "REMOTE files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\remote\allr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\os\win32\chop.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\os\win32\cntl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\os\win32\ibconfig.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\inet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\interface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\merge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\parser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\os\win32\property.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\protocol.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\remote.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\server.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\os\win32\srvr_w32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\os\win32\window.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\os\win32\wnet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\xdr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\xnet.cpp
# End Source File
# End Group
# Begin Group "Header files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\remote\allr_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\inet_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\merge_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\proto_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\protocol.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\remot_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\remote.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\serve_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\spxnet32_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\os\win32\wnet_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\xdr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\xnet.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\xnet_proto.h
# End Source File
# End Group
# End Target
# End Project
