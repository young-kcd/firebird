# Microsoft Developer Studio Project File - Name="fb2control" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=FB2CONTROL - WIN32 DEBUG
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fb2control.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fb2control.mak" CFG="FB2CONTROL - WIN32 DEBUG"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fb2control - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fb2control - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fb2control - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\..\temp\release\fb2control"
# PROP BASE Intermediate_Dir "..\..\..\temp\release\fb2control"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\temp\release\fb2control"
# PROP Intermediate_Dir "..\..\..\temp\release\fb2control"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /Ox /Ow /Og /Os /I "../../../src/include" /I "../../../src/include/gen" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SUPERSERVER" /D "_WINDLL" /D "_AFXDLL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 shlwapi.lib mfc42.lib mfcs42.lib version.lib /nologo /version:1.0 /subsystem:windows /dll /machine:I386 /out:"..\..\..\temp\release\firebird\system32\Firebird2Control.cpl"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "fb2control - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\..\temp\debug\fb2control"
# PROP BASE Intermediate_Dir "..\..\..\temp\debug\fb2control"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\debug\fb2control"
# PROP Intermediate_Dir "..\..\..\temp\debug\fb2control"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../../src/include" /I "../../../src/include/gen" /D "_DEBUG" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /D "_X86_" /D "DEV_BUILD" /D "_USRDLL" /D "CPL_APPLET" /D "_WINDLL" /U "TRACE" /FR /YX /FD /GZ /c
# SUBTRACT BASE CPP /X
# ADD CPP /nologo /MDd /W3 /GR /GX /Zi /Od /I "../../../src/include" /I "../../../src/include/gen" /D "_DEBUG" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /D "DEV_BUILD" /D "_USRDLL" /D "CPL_APPLET" /D "_WINDLL" /D "_AFXDLL" /U "TRACE" /FR /FD /GZ /c
# SUBTRACT CPP /Gy /X /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 shlwapi.lib /nologo /version:1.0 /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 shlwapi.lib mfc42d.lib mfcs42d.lib th32.lib version.lib /nologo /version:1.0 /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:"..\..\..\temp\debug\firebird\system32\Firebird2Control.cpl" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "fb2control - Win32 Release"
# Name "fb2control - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "MEMORY files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\common\classes\alloc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\classes\locks.cpp
# End Source File
# End Group
# Begin Group "CONFIG files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\common\config\config.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\config\config_file.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\os\win32\config_root.cpp
# End Source File
# End Group
# Begin Group "OTHER files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\common\fb_exception.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\classes\fb_string.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\os\win32\fbsyslog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\os\win32\path_utils.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\utilities\fbcpl\ctrlpan.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\fbcpl\FBControl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\fbcpl\FBControl.def
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\fbcpl\FBDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\fbcpl\FBPanel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\install\services.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\fbcpl\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\common\classes\alloc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\include\gen\autoconfig.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\common.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\config\config.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\fbcpl\ctrlpan.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\fbcpl\FBControl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\fbcpl\FBDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\fbcpl\FBPanel.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\os\fbsyslog.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\registry.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\fbcpl\Resource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\servi_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\fbcpl\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\..\src\utilities\fbcpl\FBControl.rc
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\fbcpl\res\FBControl.rc2
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\fbcpl\res\server.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\fbcpl\res\server_stop.ico
# End Source File
# End Group
# End Target
# End Project
