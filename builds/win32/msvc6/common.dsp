# Microsoft Developer Studio Project File - Name="common" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=common - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "common.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "common.mak" CFG="common - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "common - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "common - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "common - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\debug\common_ss"
# PROP Intermediate_Dir "..\..\..\temp\debug\common_ss"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /I "../../../src/include" /I "../../../src/include/gen" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "DEV_BUILD" /D "SUPERSERVER" /YX /FD /GZ /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "common - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\release\common_ss"
# PROP Intermediate_Dir "..\..\..\temp\release\common_ss"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GR /GX /Ot /Oi /Op /Oy /Ob2 /I "../../../src/include" /I "../../../src/include/gen" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /GR /GX /Ot /Og /Oi /Op /Oy /Ob1 /I "../../../src/include" /I "../../../src/include/gen" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "SUPERSERVER" /FD /EHc- /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "common - Win32 Debug"
# Name "common - Win32 Release"
# Begin Group "MEMORY files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\common\classes\alloc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\classes\locks.cpp
# End Source File
# End Group
# Begin Group "EXCEPTION files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\common\fb_exception.cpp
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
# Begin Source File

SOURCE=..\..\..\src\common\config\dir_list.cpp
# End Source File
# End Group
# Begin Group "OTHER files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\jrd\db_alias.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\classes\fb_string.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\os\win32\fbsyslog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\os\win32\mod_loader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\os\win32\path_utils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\classes\StringMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\classes\timestamp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\utils.cpp
# End Source File
# End Group
# Begin Group "Header files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=..\..\..\src\common\classes\alloc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\classes\array.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\classes\auto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\config\config.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\config\config_file.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\config\config_impl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\os\config_root.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\config\dir_list.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\classes\fb_pair.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\os\fbsyslog.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\classes\init.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\classes\objects_array.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\classes\StringMap.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\classes\timestamp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\utils_proto.h
# End Source File
# End Group
# Begin Group "CLUMPLET files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\common\classes\ClumpletReader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\common\classes\ClumpletWriter.cpp
# End Source File
# End Group
# End Target
# End Project
