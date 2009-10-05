# Microsoft Developer Studio Project File - Name="gpre_boot" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=gpre_boot - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gpre_boot.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gpre_boot.mak" CFG="gpre_boot - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gpre_boot - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "gpre_boot - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gpre_boot - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\debug\gpre_boot"
# PROP Intermediate_Dir "..\..\..\temp\debug\gpre_boot"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../src/include" /I "../../../src/include/gen" /D "_DEBUG" /D "_CONSOLE" /D "SUPERCLIENT" /D "WIN32" /D "_MBCS" /D "_X86_" /D "DEV_BUILD" /D "_WINDOWS" /D "_USRDLL" /D "CLIENT" /FR /FD /GZ /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib shell32.lib comctl32.lib advapi32.lib ws2_32.lib mpr.lib version.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"..\..\..\temp\debug\firebird/bin/gpre_boot.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "gpre_boot - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\release\gpre_boot"
# PROP Intermediate_Dir "..\..\..\temp\release\gpre_boot"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../src/include" /I "../../../src/include/gen" /I "../../../src/jrd" /D "_DEBUG" /D "_CONSOLE" /D "SUPERCLIENT" /D "WIN32" /D "_MBCS" /D "_X86_" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /Ot /Og /Oi /Oy /Ob1 /I "../../../src/include" /I "../../../src/include/gen" /D "NDEBUG" /D "_CONSOLE" /D "SUPERCLIENT" /D "WIN32" /D "_MBCS" /D "_X86_" /FR /FD /EHc- /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib shell32.lib comctl32.lib advapi32.lib ws2_32.lib mpr.lib version.lib /nologo /subsystem:console /incremental:no /machine:I386 /out:"..\..\..\temp\release\firebird/bin/gpre_boot.exe" /pdbtype:sept
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "gpre_boot - Win32 Debug"
# Name "gpre_boot - Win32 Release"
# Begin Group "GPRE files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\gpre\ada.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\c_cxx.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\cmd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\cme.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\cmp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\cob.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\exp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\fb_exception.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\gpre.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\gpre_meta_boot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\hsh.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\init.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\int_cxx.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\jrdmet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\movg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\msc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\noform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\par.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\pas.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\pat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\pretty.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\sqe.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\sql.cpp
# End Source File
# End Group
# Begin Group "JRD files"

# PROP Default_Filter "h;c"
# Begin Source File

SOURCE=..\..\..\src\jrd\dls.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dsc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dsc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\gds.cpp
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

SOURCE=..\..\..\src\jrd\thd.cpp
# End Source File
# End Group
# Begin Group "Header files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\gpre\cdbtable.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\cmd_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\cme_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\cmp_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\dyntable.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\exp_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\form.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\form__proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\form_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\gpre.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\gpre_meta.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\gpre_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\gpreswi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\hsh.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\hsh_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\init.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\jrdme_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\lang_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\movg_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\msc_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\par_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\parse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\pat.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\pat_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\prett_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\sdltable.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\sqe_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\sql_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gpre\words.h
# End Source File
# End Group
# Begin Group "Resource files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\jrd\version.rc
# End Source File
# End Group
# End Target
# End Project
