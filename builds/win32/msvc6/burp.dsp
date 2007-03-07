# Microsoft Developer Studio Project File - Name="burp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=burp - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "burp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "burp.mak" CFG="burp - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "burp - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "burp - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "burp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\debug\burp_ss"
# PROP Intermediate_Dir "..\..\..\temp\debug\burp_ss"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../src/include" /I "../../../src/include/gen" /D "_DEBUG" /D "_LIB" /D "SUPERSERVER" /D "WIN32" /D "_MBCS" /D "_X86_" /D "DEV_BUILD" /FD /GZ /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "burp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\temp\release\burp_ss"
# PROP Intermediate_Dir "..\..\..\temp\release\burp_ss"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Ot /Oi /Op /Oy /Ob2 /I "../../../src/include" /I "../../../src/include/gen" /D "NDEBUG" /D "_LIB" /D "SUPERSERVER" /D "NOMSG" /D "WIN32" /D "_MBCS" /D "_X86_" /FD /GZ /c
# ADD CPP /nologo /MD /W3 /GX /Ot /Og /Oi /Op /Oy /Ob1 /I "../../../src/include" /I "../../../src/include/gen" /D "NDEBUG" /D "_LIB" /D "SUPERSERVER" /D "WIN32" /D "_MBCS" /D "_X86_" /FD /EHc- /c
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

# Name "burp - Win32 Debug"
# Name "burp - Win32 Release"
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

SOURCE=..\..\..\gen\burp\backup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\gen\burp\restore.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\burp\burp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\burp\canonical.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\burp\misc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\burp\mvol.cpp
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
# End Target
# End Project
