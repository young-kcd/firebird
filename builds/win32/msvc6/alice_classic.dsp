# Microsoft Developer Studio Project File - Name="alice_classic" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=alice_classic - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "alice_classic.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "alice_classic.mak" CFG="alice_classic - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "alice_classic - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "alice_classic - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "alice_classic - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug\alice_cs"
# PROP Intermediate_Dir "debug\alice_cs"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../src/include" /I "../../../src/include/gen" /D "_DEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /D "_X86_" /D "DEV_BUILD" /FD /GZ /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"debug\alice_cs\alice.lib"

!ELSEIF  "$(CFG)" == "alice_classic - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "release\alice_cs"
# PROP Intermediate_Dir "release\alice_cs"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Ot /Oi /Op /Oy /Ob2 /I "../../../src/include" /I "../../../src/include/gen" /D "NDEBUG" /D "_LIB" /D "SUPERSERVER" /D "WIN32" /D "_MBCS" /D "_X86_" /FD /GZ /c
# SUBTRACT BASE CPP /Ox /Og /YX
# ADD CPP /nologo /MD /W3 /GX /Ot /Og /Oi /Op /Oy /Ob1 /I "../../../src/include" /I "../../../src/include/gen" /D "NDEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /D "_X86_" /FD /EHc- /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"release\alice_cs\alice.lib"

!ENDIF 

# Begin Target

# Name "alice_classic - Win32 Debug"
# Name "alice_classic - Win32 Release"
# Begin Group "ALICE files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "epp files"

# PROP Default_Filter "epp"
# Begin Source File

SOURCE=..\..\..\src\alice\alice_meta.epp
# End Source File
# End Group
# Begin Group "Generated files"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=..\..\..\gen\alice\alice_meta.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\alice\alice.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\all.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\exe.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\tdr.cpp
# End Source File
# End Group
# Begin Group "Header files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\alice\alice.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\alice_meta.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\alice_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\aliceswi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\all.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\all_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\alloc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\blk.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\exe_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\info.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\lls.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\alice\tdr_proto.h
# End Source File
# End Group
# End Target
# End Project
