# Microsoft Developer Studio Project File - Name="gbak" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=gbak - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gbak.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gbak.mak" CFG="gbak - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gbak - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "gbak - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gbak - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug\gbak"
# PROP Intermediate_Dir "debug\gbak"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../src/include" /D "_DEBUG" /D "DEV_BUILD" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_X86_" /D "SUPERCLIENT" /FR /FD /GZ /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib shell32.lib comctl32.lib advapi32.lib ws2_32.lib mpr.lib version.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"debug/firebird/bin/gbak.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "gbak - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "release\gbak"
# PROP Intermediate_Dir "release\gbak"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../src/include" /D "_DEBUG" /D "DEV_BUILD" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_X86_" /D "NOMSG" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /Ot /Og /Oi /Op /Oy /Ob1 /I "../../../src/include" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_X86_" /D "SUPERCLIENT" /FR /FD /EHc- /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib advapi32.lib shell32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib shell32.lib comctl32.lib advapi32.lib ws2_32.lib mpr.lib version.lib /nologo /subsystem:console /incremental:no /machine:I386 /out:"release/firebird/bin/gbak.exe" /pdbtype:sept
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "gbak - Win32 Debug"
# Name "gbak - Win32 Release"
# Begin Group "BURP files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "epp files"

# PROP Default_Filter "epp"
# Begin Source File

SOURCE=..\..\..\src\burp\backup.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\burp\restore.epp
# End Source File
# End Group
# Begin Group "Generated files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\generated\burp\backup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\burp\restore.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\burp\burp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\burp\canonical.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\fb_exception.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\burp\misc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\burp\mvol.cpp
# End Source File
# End Group
# Begin Group "JRD files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\jrd\sdl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sdl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sdl_proto.h
# End Source File
# End Group
# Begin Group "REMOTE files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\remote\xdr.cpp
# End Source File
# End Group
# Begin Group "Header files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\burp\backu_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\burp\burp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\burp\burp_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\burp\burpswi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\burp\canon_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\burp\misc_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\burp\mvol_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\burp\resto_proto.h
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
