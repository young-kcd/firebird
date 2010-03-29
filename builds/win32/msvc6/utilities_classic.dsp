# Microsoft Developer Studio Project File - Name="utilities_classic" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=utilities_classic - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "utilities_classic.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "utilities_classic.mak" CFG="utilities_classic - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "utilities_classic - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "utilities_classic - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "utilities_classic - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\debug\utilities_cs"
# PROP Intermediate_Dir "..\..\..\temp\debug\utilities_cs"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "../../../src/include" /I "../../../src/include/gen" /D "_DEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /D "_X86_" /D "DEV_BUILD" /FD /GZ /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\temp\debug\utilities_cs\utilities.lib"

!ELSEIF  "$(CFG)" == "utilities_classic - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\release\utilities_cs"
# PROP Intermediate_Dir "..\..\..\temp\release\utilities_cs"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GR /GX /Ot /Oi /Op /Oy /Ob2 /I "../../../src/include" /I "../../../src/include/gen" /D "NDEBUG" /D "_LIB" /D "SUPERSERVER" /D "WIN32" /D "_MBCS" /D "_X86_" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GR /GX /Ot /Og /Oi /Op /Oy /Ob1 /I "../../../src/include" /I "../../../src/include/gen" /D "NDEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /D "_X86_" /FD /EHc- /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\temp\release\utilities_cs\utilities.lib"

!ENDIF 

# Begin Target

# Name "utilities_classic - Win32 Debug"
# Name "utilities_classic - Win32 Release"
# Begin Group "UTILITIES files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "epp files"

# PROP Default_Filter "epp"
# Begin Source File

SOURCE=..\..\..\src\utilities\gstat\dba.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\rebuild\rmet.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\rebuild\rstore.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\gsec\security.epp
# End Source File
# End Group
# Begin Group "Generated files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\gen\utilities\gsec\security.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\utilities\gsec\call_service.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\common\cmd_util.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\gsec\gsec.cpp
# End Source File
# End Group
# Begin Group "Header files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\utilities\common\cmd_util_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\gsec\gsec.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\install\registry.h
# End Source File
# End Group
# End Target
# End Project
