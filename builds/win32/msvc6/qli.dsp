# Microsoft Developer Studio Project File - Name="qli" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=qli - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "qli.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "qli.mak" CFG="qli - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "qli - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "qli - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "qli - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\debug\qli"
# PROP Intermediate_Dir "..\..\..\temp\debug\qli"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../src/include" /I "../../../src/include/gen" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "DEV_BUILD" /D "SUPERCLIENT" /FD /GZ /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib shell32.lib comctl32.lib advapi32.lib ws2_32.lib mpr.lib version.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"..\..\..\temp\debug\firebird/bin/qli.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "qli - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\release\qli"
# PROP Intermediate_Dir "..\..\..\temp\release\qli"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../src/include" /I "../../../src/include/gen" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /Ot /Og /Oi /Op /Oy /Ob1 /I "../../../src/include" /I "../../../src/include/gen" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "SUPERCLIENT" /FD /EHc- /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib shell32.lib comctl32.lib advapi32.lib ws2_32.lib mpr.lib version.lib /nologo /subsystem:console /incremental:no /machine:I386 /out:"..\..\..\temp\release\firebird/bin/qli.exe" /pdbtype:sept
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "qli - Win32 Debug"
# Name "qli - Win32 Release"
# Begin Group "QLI files"

# PROP Default_Filter ""
# Begin Group "epp files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\qli\help.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\meta.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\proc.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\show.epp
# End Source File
# End Group
# Begin Group "Generated files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\gen\qli\help.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\gen\qli\meta.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\gen\qli\proc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\gen\qli\show.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\qli\all.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\command.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\compile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\dtr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\err.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\eval.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\exe.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\expand.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\format.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\gener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\hsh.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\lex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\mov.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\noform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\parse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\picstr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\report.cpp
# End Source File
# End Group
# Begin Group "JRD files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\jrd\dsc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\iberr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_file.cpp
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
# End Group
# Begin Group "GPRE files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\gpre\pretty.cpp
# End Source File
# End Group
# Begin Group "COMMON files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\common\fb_exception.cpp
# End Source File
# End Group
# Begin Group "Header files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\qli\all_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\comma_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\compi_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\compile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\dtr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\err_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\eval_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\everything.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\exe.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\exe_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\expan_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\form.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\form_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\forma_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\format.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\gener_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\help_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\hsh_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\lex_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\meta_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\mov_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\parse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\parse_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\picst_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\proc_ddl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\proc_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\procddl1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\procddl2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\procddl3.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\procddl4.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\repor_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\report.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\reqs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\show_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\symbols.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\qli\words.h
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
