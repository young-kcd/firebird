# Microsoft Developer Studio Project File - Name="intl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=intl - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "intl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "intl.mak" CFG="intl - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "intl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "intl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "intl - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug\intl"
# PROP Intermediate_Dir "debug\intl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "INTL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../src/include" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "INTL_EXPORTS" /D "WINDOWS_ONLY" /D "SUPERCLIENT" /D "WIN32" /D "_MBCS" /D "_X86_" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /debug /machine:I386 /out:"debug\firebird\intl/fbintl.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "intl - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "release\intl"
# PROP Intermediate_Dir "release\intl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../src/include" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "INTL_EXPORTS" /D "WINDOWS_ONLY" /D "SUPERCLIENT" /D "WIN32" /D "_MBCS" /D "_X86_" /FR /FD /GZ /c
# ADD CPP /nologo /MD /W3 /GX /Ot /Oi /Op /Oy /Ob2 /I "../../../src/include" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "INTL_EXPORTS" /D "WINDOWS_ONLY" /D "SUPERCLIENT" /D "WIN32" /D "_MBCS" /D "_X86_" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /machine:I386 /out:"release\firebird\intl/fbintl.dll" /pdbtype:sept
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "intl - Win32 Debug"
# Name "intl - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\intl\cs_big5.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_gb2312.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_jis.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_jis_20208_1990.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_ksc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_narrow.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_unicode.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_utffss.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cv_big5.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cv_gb2312.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cv_jis.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cv_ksc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cv_narrow.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\lc_ascii.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\lc_big5.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\lc_dos.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\lc_gb2312.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\lc_jis.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\lc_ksc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\lc_latin1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\lc_latin2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\lc_narrow.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\lc_unicode.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\ld.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\ld2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\version.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\..\..\src\intl\intl.def

!IF  "$(CFG)" == "intl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "intl - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Target
# End Project
