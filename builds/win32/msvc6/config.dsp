# Microsoft Developer Studio Project File - Name="config" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=config - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "config.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "config.mak" CFG="config - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "config - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "config - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "config - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\debug\config"
# PROP Intermediate_Dir "..\..\..\temp\debug\config"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /I "../../../src/include" /I "../../../src/include/gen" /I "../../../src/vulcan" /I "../../../src/jrd" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "DEV_BUILD" /D NAMESPACE=Vulcan /YX /FD /GZ /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "config - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\release\config"
# PROP Intermediate_Dir "..\..\..\temp\release\config"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GR /GX /Ot /Oi /Op /Oy /Ob2 /I "../../../src/include" /I "../../../src/include/gen" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /GR /GX /Ot /Og /Oi /Op /Oy /Ob1 /I "../../../src/include" /I "../../../src/include/gen" /I "../../../src/vulcan" /I "../../../src/jrd" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D NAMESPACE=Vulcan /FD /EHc- /c
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

# Name "config - Win32 Debug"
# Name "config - Win32 Release"
# Begin Group "CONFIG files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\config\AdminException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\Args.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\ArgsException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\ConfigFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\Configuration.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\ConfObj.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\ConfObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\DirectoryList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\Element.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\FileName.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\InputFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\InputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\Lex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\ScanDir.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\Stream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\StreamSegment.cpp
# End Source File
# End Group
# Begin Group "VULCAN files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\vulcan\JString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\vulcan\PathName.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\vulcan\RefObject.cpp
# End Source File
# End Group
# Begin Group "Header files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=..\..\..\src\config\AdminException.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\Args.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\ArgsException.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\ConfigFile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\Configuration.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\ConfObj.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\ConfObject.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\DirectoryList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\Element.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\FileName.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\InputFile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\InputStream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\vulcan\JString.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\Lex.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\Parameters.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\vulcan\PathName.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\vulcan\RefObject.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\ScanDir.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\Stream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\config\StreamSegment.h
# End Source File
# End Group
# End Target
# End Project
