# Microsoft Developer Studio Project File - Name="intlcpp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=intlcpp - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "intlcpp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "intlcpp.mak" CFG="intlcpp - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "intlcpp - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "intlcpp - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "intlcpp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug\intlcpp"
# PROP Intermediate_Dir "debug\intlcpp"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "INTL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../src/include" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "WINDOWS_ONLY" /D "SUPERCLIENT" /D "WIN32" /D "_MBCS" /D "_X86_" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib shell32.lib comctl32.lib advapi32.lib ws2_32.lib mpr.lib version.lib /nologo /dll /incremental:no /debug /machine:I386 /out:"debug/firebird/intlcpp/fbintlcpp.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "intlcpp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "release\intlcpp"
# PROP Intermediate_Dir "release\intlcpp"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../src/include" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "INTL_EXPORTS" /D "WINDOWS_ONLY" /D "SUPERCLIENT" /D "WIN32" /D "_MBCS" /D "_X86_" /FR /FD /GZ /c
# ADD CPP /nologo /MD /W3 /GX /Ot /Og /Oi /Op /Oy /Ob1 /I "../../../src/include" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "INTL_EXPORTS" /D "WINDOWS_ONLY" /D "SUPERCLIENT" /D "WIN32" /D "_MBCS" /D "_X86_" /FR /FD /EHc- /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib shell32.lib comctl32.lib advapi32.lib ws2_32.lib mpr.lib version.lib /nologo /dll /incremental:no /machine:I386 /out:"release/firebird/intlcpp/fbintlcpp.dll" /pdbtype:sept
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "intlcpp - Win32 Debug"
# Name "intlcpp - Win32 Release"
# Begin Group "INTL files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\intlcpp\cs_big5.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\cs_gb2312.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\cs_jis.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\cs_ksc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\cs_narrow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\cs_unicode_fss.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\cs_unicode_ucs2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\cv_big5.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\cv_gb2312.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\cv_jis.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\cv_ksc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\cv_narrow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\cv_unicode_fss.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\lc_ascii.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\lc_big5.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\lc_dos.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\lc_gb2312.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\lc_iso8859_1.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\lc_iso8859_2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\lc_jis.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\lc_ksc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\lc_narrow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\lc_unicode_ucs2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\ld.cpp
# End Source File
# End Group
# Begin Group "Header files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\ascii.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\ascii2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\bl88591ca0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\bl88591da0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\bl88591de0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\bl88591es0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\bl88591fi0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\bl88591fr0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\bl88591is0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\bl88591it0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\bl88591nl0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\bl88591no0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\bl88591pt0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\bl88591sv0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\bl88591uk0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\bl88591us0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\blNEXTde0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\blNEXTes0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\blNEXTfr0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\blNEXTit0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\blNEXTus0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\countries.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\country_codes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_437.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_850.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_852.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_857.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_860.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_861.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_863.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_865.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_big5.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_cyrl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_gb2312.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_iso8859_1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_iso8859_2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_jis_0208_1990.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_ksc5601.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_next.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_sjis.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_w1250.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_w1251.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_w1252.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_w1253.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\charsets\cs_w1254.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db437de0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db437es1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db437fi0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db437fr0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db437it0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db437nl0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db437sv0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db437uk0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db437us0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db850cf0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db850de0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db850es0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db850fr0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db850it1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db850nl0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db850pt0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db850sv1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db850uk0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db850us0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db852cz0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db852hdc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db852po0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db852sl0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db857tr0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db860pt0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db863cf1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db865da0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db865no0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\db866ru0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\kanji.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\kanji_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\langdrv.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\ld.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\ld_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\ldcommon.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pd437intl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pd437swedfin.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pd852hundc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pd852polish.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pd852slovene.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pd857turk.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pd861iceland.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pd865nordan40.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pd866cyrr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\ps852czech.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pw1250czech.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pw1250hun.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pw1250hundc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pw1250polish.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pw1250slov.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pw1251cyrr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pw1252i850.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pw1252intl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pw1252nor4.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pw1252span.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pw1252swfn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pw1253greek1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\pw1253turk.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\conversions\tx437_865.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\conversions\tx437_lat1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\conversions\tx865_lat1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\undef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\win1251_ua.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intlcpp\collations\xx88592czech.h
# End Source File
# End Group
# Begin Group "Resource files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\jrd\version.rc
# End Source File
# End Group
# Begin Source File

SOURCE=..\defs\intlcpp.def

!IF  "$(CFG)" == "intlcpp - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "intlcpp - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Target
# End Project
