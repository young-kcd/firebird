# Microsoft Developer Studio Project File - Name="dsql_server_classic" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=dsql_server_classic - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "dsql_server_classic.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dsql_server_classic.mak" CFG="dsql_server_classic - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dsql_server_classic - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "dsql_server_classic - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "dsql_server_classic - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug\dsql_server_cs"
# PROP Intermediate_Dir "debug\dsql_server_cs"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../src/include" /I "../../../src/include/gen" /D "DEBUG" /D "_DEBUG" /D "_LIB" /D "_WINDOWS" /D "WIN32" /D "DEV_BUILD" /D "_MBCS" /D "_X86_" /FD /GZ /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"debug\dsql_server_cs\dsql_server.lib"

!ELSEIF  "$(CFG)" == "dsql_server_classic - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "release\dsql_server_cs"
# PROP Intermediate_Dir "release\dsql_server_cs"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Ot /Oi /Op /Oy /Ob2 /I "../../../src/include" /I "../../../src/include/gen" /D "DEBUG" /D "NDEBUG" /D "_LIB" /D "_WINDOWS" /D "SUPERSERVER" /D "NOMSG" /D "WIN32" /D "_MBCS" /D "_X86_" /FD /GZ /c
# ADD CPP /nologo /MD /W3 /GX /Ot /Og /Oi /Op /Oy /Ob1 /I "../../../src/include" /I "../../../src/include/gen" /D "NDEBUG" /D "_LIB" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /D "_X86_" /FD /EHc- /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"release\dsql_server_cs\dsql_server.lib"

!ENDIF 

# Begin Target

# Name "dsql_server_classic - Win32 Debug"
# Name "dsql_server_classic - Win32 Release"
# Begin Group "DSQL files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "epp files"

# PROP Default_Filter "epp"
# Begin Source File

SOURCE=..\..\..\src\dsql\array.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\blob.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\metd.epp
# End Source File
# End Group
# Begin Group "Generated files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\gen\dsql\array.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\gen\dsql\blob.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\gen\dsql\metd.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\dsql\alld.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\ddl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\dsql.cpp

!IF  "$(CFG)" == "dsql_server_classic - Win32 Debug"

!ELSEIF  "$(CFG)" == "dsql_server_classic - Win32 Release"

# ADD CPP /Og

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\errd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\gen.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\hsh.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\keywords.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\make.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\misc_func.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\movd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\parse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\pass1.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\preparse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\user_dsql.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\utld.cpp
# End Source File
# End Group
# Begin Group "GPRE files"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=..\..\..\src\gpre\pretty.cpp
# End Source File
# End Group
# Begin Group "Header files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\dsql\all.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\alld_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\array_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\blk.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\blob_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\chars.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\ddl_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\dsql.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\dsql_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\errd_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\gen_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\hsh_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\keywords.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\make_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\metd_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\misc_func.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\movd_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\node.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\parse_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\pass1_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\prepa_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\sqlda.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\sym.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\user__proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\dsql\utld_proto.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\dsql\parse.y
# End Source File
# End Target
# End Project
