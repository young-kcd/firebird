# Microsoft Developer Studio Project File - Name="gdef" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=gdef - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gdef.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gdef.mak" CFG="gdef - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gdef - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "gdef - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gdef - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug\gdef"
# PROP Intermediate_Dir "debug\gdef"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../src/include" /I "../../../src/include/gen" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "SUPERCLIENT" /D "DEV_BUILD" /FD /GZ /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib shell32.lib comctl32.lib advapi32.lib ws2_32.lib mpr.lib version.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"debug/firebird/bin/gdef.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "gdef - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "release\gdef"
# PROP Intermediate_Dir "release\gdef"
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib shell32.lib comctl32.lib advapi32.lib ws2_32.lib mpr.lib version.lib /nologo /subsystem:console /incremental:no /machine:I386 /out:"release/firebird/bin/gdef.exe" /pdbtype:sept
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "gdef - Win32 Debug"
# Name "gdef - Win32 Release"
# Begin Group "DUDLEY files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\dudley\ddl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\dudley\exe.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dudley\expand.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dudley\expr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\dudley\extract.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\fb_exception.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dudley\generate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dudley\hsh.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dudley\lex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dudley\parse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dudley\trn.cpp
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
# Begin Group "WAL files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\wal\walf.cpp
# End Source File
# End Group
# Begin Group "Header files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\dudley\ddl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dudley\ddl_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dudley\exe_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dudley\expan_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dudley\expr_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dudley\extra_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dudley\gener_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dudley\hsh_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dudley\lex_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dudley\parse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dudley\parse_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dudley\trn_proto.h
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
