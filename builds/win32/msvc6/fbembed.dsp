# Microsoft Developer Studio Project File - Name="fbembed" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=fbembed - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fbembed.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fbembed.mak" CFG="fbembed - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fbembed - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fbembed - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fbembed - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\temp\release\fbembed"
# PROP Intermediate_Dir "..\..\..\temp\release\fbembed"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "fbembed_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Ot /Og /Oi /Op /Oy /Ob1 /I "../../../src/include" /I "../../../src/include/gen" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_X86_" /D "SUPERSERVER" /D "EMBEDDED" /YX /FD /EHc- /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib advapi32.lib mpr.lib ws2_32.lib version.lib ole32.lib icuuc.lib icuin.lib /nologo /dll /machine:I386 /out:"..\..\..\temp\release\firebird/bin/fbembed.dll" /libpath:../../../extern/icu/lib
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "fbembed - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\debug\fbembed"
# PROP Intermediate_Dir "..\..\..\temp\debug\fbembed"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "fbembed_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "../../../src/include" /I "../../../src/include/gen" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_X86_" /D "SUPERSERVER" /D "DEV_BUILD" /D "EMBEDDED" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib shell32.lib comctl32.lib advapi32.lib ws2_32.lib mpr.lib version.lib ole32.lib icuuc.lib icuin.lib /nologo /dll /incremental:no /debug /machine:I386 /out:"..\..\..\temp\debug\firebird/bin/fbembed.dll" /pdbtype:sept /libpath:../../../extern/icu/lib
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "fbembed - Win32 Release"
# Name "fbembed - Win32 Debug"
# Begin Group "COMMON"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\common\config\config_file.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\os\win32\config_root.cpp
# End Source File
# End Group
# Begin Group "JRD files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\jrd\os\win32\ibinitdll.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\perf.cpp
# End Source File
# End Group
# Begin Group "REMOTE files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\remote\allr.cpp

!IF  "$(CFG)" == "fbembed - Win32 Release"

# ADD CPP /D "SUPERCLIENT"
# SUBTRACT CPP /D "SUPERSERVER"

!ELSEIF  "$(CFG)" == "fbembed - Win32 Debug"

# ADD CPP /D "SUPERCLIENT"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\inet.cpp

!IF  "$(CFG)" == "fbembed - Win32 Release"

# ADD CPP /D "SUPERCLIENT"
# SUBTRACT CPP /D "SUPERSERVER"

!ELSEIF  "$(CFG)" == "fbembed - Win32 Debug"

# ADD CPP /D "SUPERCLIENT"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\interface.cpp

!IF  "$(CFG)" == "fbembed - Win32 Release"

# ADD CPP /D "SUPERCLIENT"
# SUBTRACT CPP /D "SUPERSERVER"

!ELSEIF  "$(CFG)" == "fbembed - Win32 Debug"

# ADD CPP /D "SUPERCLIENT"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\merge.cpp

!IF  "$(CFG)" == "fbembed - Win32 Release"

# ADD CPP /D "SUPERCLIENT"
# SUBTRACT CPP /D "SUPERSERVER"

!ELSEIF  "$(CFG)" == "fbembed - Win32 Debug"

# ADD CPP /D "SUPERCLIENT"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\parser.cpp

!IF  "$(CFG)" == "fbembed - Win32 Release"

# ADD CPP /D "SUPERCLIENT"
# SUBTRACT CPP /D "SUPERSERVER"

!ELSEIF  "$(CFG)" == "fbembed - Win32 Debug"

# ADD CPP /D "SUPERCLIENT"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\protocol.cpp

!IF  "$(CFG)" == "fbembed - Win32 Release"

# ADD CPP /D "SUPERCLIENT"
# SUBTRACT CPP /D "SUPERSERVER"

!ELSEIF  "$(CFG)" == "fbembed - Win32 Debug"

# ADD CPP /D "SUPERCLIENT"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\remote.cpp

!IF  "$(CFG)" == "fbembed - Win32 Release"

# ADD CPP /D "SUPERCLIENT"
# SUBTRACT CPP /D "SUPERSERVER"

!ELSEIF  "$(CFG)" == "fbembed - Win32 Debug"

# ADD CPP /D "SUPERCLIENT"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\os\win32\wnet.cpp

!IF  "$(CFG)" == "fbembed - Win32 Release"

# ADD CPP /D "SUPERCLIENT"
# SUBTRACT CPP /D "SUPERSERVER"

!ELSEIF  "$(CFG)" == "fbembed - Win32 Debug"

# ADD CPP /D "SUPERCLIENT"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\xdr.cpp

!IF  "$(CFG)" == "fbembed - Win32 Release"

# ADD CPP /D "SUPERCLIENT"
# SUBTRACT CPP /D "SUPERSERVER"

!ELSEIF  "$(CFG)" == "fbembed - Win32 Debug"

# ADD CPP /D "SUPERCLIENT"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\xnet.cpp

!IF  "$(CFG)" == "fbembed - Win32 Release"

# ADD CPP /D "SUPERCLIENT"
# SUBTRACT CPP /D "SUPERSERVER"

!ELSEIF  "$(CFG)" == "fbembed - Win32 Debug"

# ADD CPP /D "SUPERCLIENT"

!ENDIF 

# End Source File
# End Group
# Begin Group "UTILITIES files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\gen\utilities\gstat\dba.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\gstat\ppg.cpp
# End Source File
# End Group
# Begin Group "Resource files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\jrd\version.rc
# End Source File
# End Group
# Begin Source File

SOURCE=..\defs\fbclient.def
# End Source File
# End Target
# End Project
