# Microsoft Developer Studio Project File - Name="fbclient_static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=fbclient_static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fbclient_static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fbclient_static.mak" CFG="fbclient_static - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fbclient_static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "fbclient_static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fbclient_static - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\..\temp\release\fbclient_static"
# PROP BASE Intermediate_Dir "..\..\..\temp\release\fbclient_static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\temp\release\fbclient_static"
# PROP Intermediate_Dir "..\..\..\temp\release\fbclient_static"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../../../src/include" /I "../../../src/include/gen" /D "_WINDOWS" /D "_USRDLL" /D "CLIENT" /D "SUPERCLIENT" /D "IPSERV" /D "GOVERNOR" /D "I386" /D _X86_=1 /D "WIN32" /D "_MBCS" /D "_X86_" /D "_LIB" /D "NDEBUG" /D "BOOT_BUILD" /YX /FD /c
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "fbclient_static - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\..\temp\debug\fbclient_static"
# PROP BASE Intermediate_Dir "..\..\..\temp\debug\fbclient_static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\debug\fbclient_static"
# PROP Intermediate_Dir "..\..\..\temp\debug\fbclient_static"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "../../../src/include" /I "../../../src/include/gen" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "CLIENT" /D "SUPERCLIENT" /D "DEV_BUILD" /D "IPSERV" /D "GOVERNOR" /D "I386" /D _X86_=1 /D "WIN32" /D "_MBCS" /D "_X86_" /D "_LIB" /D "BOOT_BUILD" /YX /FD /GZ /c
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "fbclient_static - Win32 Release"
# Name "fbclient_static - Win32 Debug"
# Begin Group "Resource files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\jrd\version.rc
# End Source File
# End Group
# Begin Group "Header files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\jrd\cvt_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\db_alias.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dls_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dsc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dsc_pub.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\enc_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\err_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\flu.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\flu_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\gds_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\perf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\perf_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sch_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sdl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sdl_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\thd.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\utl_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\why_proto.h
# End Source File
# End Group
# Begin Group "REMOTE files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\remote\allr.cpp
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

SOURCE=..\..\..\src\remote\protocol.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\remote.cpp
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
# Begin Group "DSQL files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\gen\dsql\array.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\gen\dsql\blob.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\keywords.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\preparse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\user_dsql.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\utld.cpp
# End Source File
# End Group
# Begin Group "JRD files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\jrd\alt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\cvt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\db_alias.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dls.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dsc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\enc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\gds.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\os\win32\ibinitdll.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_file.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\os\win32\isc_ipc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_sync.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\perf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sch.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sdl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\status.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\thd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\ThreadData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\utl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\why.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\defs\fbclient.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\defs\fbclient.def
# End Source File
# End Target
# End Project
