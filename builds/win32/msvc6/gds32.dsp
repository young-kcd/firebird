# Microsoft Developer Studio Project File - Name="gds32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=gds32 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gds32.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gds32.mak" CFG="gds32 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gds32 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "gds32 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gds32 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug\gds32"
# PROP Intermediate_Dir "debug\gds32"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GDS32_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../src/include" /I "../../../src" /D "_DEBUG" /D "DEV_BUILD" /D "_WINDOWS" /D "_USRDLL" /D "GDS32_EXPORTS" /D "CLIENT" /D "SUPERCLIENT" /D "NOMSG" /D "WIN32_LEAN_AND_MEAN" /D "SERVER_SHUTDOWN" /D "IPSERV" /D "GOVERNOR" /D "EXACT_NUMERICS" /D "I386" /D "WIN32" /D "_MBCS" /D "_X86_" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib mpr.lib wsock32.lib /nologo /dll /incremental:no /debug /machine:I386 /out:"debug\firebird/gds32.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "gds32 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "release\gds32"
# PROP Intermediate_Dir "release\gds32"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../src/include" /I "../../../src" /D "_DEBUG" /D "DEV_BUILD" /D "_WINDOWS" /D "_USRDLL" /D "GDS32_EXPORTS" /D "CLIENT" /D "SUPERCLIENT" /D "NOMSG" /D "WIN32_LEAN_AND_MEAN" /D "SERVER_SHUTDOWN" /D "IPSERV" /D "GOVERNOR" /D "EXACT_NUMERICS" /D "I386" /D _X86_=1 /D "WIN32" /D "_MBCS" /D "_X86_" /FR /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /Ot /Oi /Op /Oy /Ob2 /I "../../../src/include" /I "../../../src" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "GDS32_EXPORTS" /D "CLIENT" /D "SUPERCLIENT" /D "NOMSG" /D "WIN32_LEAN_AND_MEAN" /D "SERVER_SHUTDOWN" /D "IPSERV" /D "GOVERNOR" /D "EXACT_NUMERICS" /D "I386" /D _X86_=1 /D "WIN32" /D "_MBCS" /D "_X86_" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib mpr.lib wsock32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib mpr.lib wsock32.lib /nologo /dll /incremental:no /machine:I386 /out:"release\firebird/gds32.dll" /pdbtype:sept
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "gds32 - Win32 Debug"
# Name "gds32 - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\jrd\alt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\cvt.cpp
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

SOURCE=..\..\..\src\jrd\err.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\flu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\functions.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\gds.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\ibinitdll.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_file.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_ipc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_sync.cpp

!IF  "$(CFG)" == "gds32 - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "gds32 - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_sync_win32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_win32.cpp
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

SOURCE=..\..\..\src\misc\status.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\thd.cpp

!IF  "$(CFG)" == "gds32 - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "gds32 - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\thd_win32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\utl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\why.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\jrd\cvt_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dls_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dsc.h
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

SOURCE=..\..\..\src\jrd\gds.h
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

SOURCE=..\..\..\src\jrd\thd_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\utl_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\why_proto.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "OS2 specific files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\jrd\lan.cpp

!IF  "$(CFG)" == "gds32 - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "gds32 - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\os2.cpp

!IF  "$(CFG)" == "gds32 - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "gds32 - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Utilities files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\utilities\gsec.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\utilities\security.cpp
# End Source File
# End Group
# Begin Group "Remote Source Files"

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

SOURCE=..\..\..\src\remote\spxnet32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\wnet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\xdr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\xnet.cpp
# End Source File
# End Group
# Begin Group "dsql Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\dsql\alld.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\dsql\array.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\dsql\blob.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\ddl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\dsql.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\errd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\gen.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\hsh.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\make.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\dsql\metd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\movd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\parse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\pass1.cpp
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
# Begin Group "ipserver Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\ipserver\alli.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\ipserver\ipcevent.cpp
# SUBTRACT CPP /D "NOMSG"
# End Source File
# Begin Source File

SOURCE=..\..\..\src\ipserver\ipclient.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\gds32.def

!IF  "$(CFG)" == "gds32 - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "gds32 - Win32 Release"

# PROP BASE Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\gds32_debug.def

!IF  "$(CFG)" == "gds32 - Win32 Debug"

!ELSEIF  "$(CFG)" == "gds32 - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\jrd_ver.rc

!IF  "$(CFG)" == "gds32 - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "gds32 - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\version.rc

!IF  "$(CFG)" == "gds32 - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "gds32 - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\version_95.rc

!IF  "$(CFG)" == "gds32 - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "gds32 - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Target
# End Project
