# Microsoft Developer Studio Project File - Name="fbserver" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=fbserver - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fbserver.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fbserver.mak" CFG="fbserver - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fbserver - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "fbserver - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug\fbserver"
# PROP Intermediate_Dir "debug\fbserver"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /Gi /GX /Zi /Od /I "../../../src/include" /D "_DEBUG" /D "_WINDOWS" /D "SUPERSERVER" /D "NOMSG" /D "WIN32_LEAN_AND_MEAN" /D "WIN32" /D "_MBCS" /D "_X86_" /D "DEV_BUILD" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib comctl32.lib wsock32.lib mpr.lib shell32.lib /nologo /stack:0x200000 /subsystem:windows /incremental:no /map /debug /machine:I386 /out:"debug\firebird\bin/fbserver.exe" /pdbtype:sept
# SUBTRACT LINK32 /verbose /profile /pdb:none

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "release\fbserver"
# PROP Intermediate_Dir "release\fbserver"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /w /W0 /Gi /GX /Ot /Oi /Op /Oy /Ob2 /I "../../../src/include" /D "NDEBUG" /D "_WINDOWS" /D "SUPERSERVER" /D "NOMSG" /D "WIN32_LEAN_AND_MEAN" /D "WIN32" /D "_MBCS" /D "_X86_" /FR /FD /GZ /c
# ADD CPP /nologo /G4 /MD /Gi /GX /Ot /Oi /Op /Oy /Ob2 /I "../../../src/include" /D "NDEBUG" /D "_WINDOWS" /D "SUPERSERVER" /D "NOMSG" /D "WIN32_LEAN_AND_MEAN" /D "WIN32" /D "_MBCS" /D "_X86_" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib comctl32.lib wsock32.lib mpr.lib shell32.lib /nologo /subsystem:windows /incremental:no /machine:I386 /pdbtype:sept
# SUBTRACT BASE LINK32 /verbose /profile /pdb:none /map /debug
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib comctl32.lib wsock32.lib mpr.lib shell32.lib /nologo /stack:0x200000 /subsystem:windows /incremental:no /machine:I386 /out:"release\firebird\bin/fbserver.exe" /pdbtype:sept
# SUBTRACT LINK32 /verbose /profile /pdb:none /map /debug

!ENDIF 

# Begin Target

# Name "fbserver - Win32 Debug"
# Name "fbserver - Win32 Release"
# Begin Group "remote Source Files"

# PROP Default_Filter "c;cpp"
# Begin Source File

SOURCE=..\..\..\src\remote\allr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\chop.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\cntl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\ibconfig.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\inet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\interface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\merge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\parser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\property.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\protocol.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\remote.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\server.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\spxnet32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\srvr_w32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\window.cpp
# ADD CPP /D "SERVER_SHUTDOWN" /D "EXACT_NUMERICS"
# SUBTRACT CPP /D "NOMSG"
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\wnet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\xdr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\xnet.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "remote Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\remote\allr_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\ambx_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\chop_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\cntl_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\decne_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\dnet_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\faux.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\ibconfig.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\ibsvrhlp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\in.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\inet_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\inetiodef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\inter_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\ipc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\ipc_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\lanentry.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\lanman.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\merge_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\mslan_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\neterr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\ntoh_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\parse_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\propty_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\proto_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\protocol.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\remot_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\remote.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\remote_def.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\serve_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\spxne_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\spxnet32_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\spxwi_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\spxwin.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\tcptypes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\wfwnp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\wfwnp_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\window.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\window_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\winerr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\wnet_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\xdr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\xdr_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\xnet.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\xnet_proto.h
# End Source File
# End Group
# Begin Group "jrd Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\jrd\acl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\aif_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\ail_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\align.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\all.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\all_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\ansi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\arg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\atom.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\blb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\blb_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\blf_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\blk.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\blob_filter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\blp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\blr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\include\gen\blrtable.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\bookmark.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\btr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\btr_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\build_no.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\cch.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\cch_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\cdefs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\cmp_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\codes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\codetext.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\common.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\constants.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\cvt2_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\cvt_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dbg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dbg_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dflt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dfw_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\divorce.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dls_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dmp_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\doserr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dpm_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\drq.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dsc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dsc_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dyn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dyn_df_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dyn_dl_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dyn_md_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dyn_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dyn_ut_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\enc_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\entry.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\err_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\event.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\event_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\everything.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\evl_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\exe.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\exe_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\ext.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\ext_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\fields.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\fil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\file_params.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\filte_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\flags.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\flu.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\flu_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\fun_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\gds.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\gds.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\gds_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\gdsassert.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\gdsold.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\grant_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\ib_stdio.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\ibase.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\iberr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\iberr_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\iberror.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\ibsetjmp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\ids.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\idx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\idx_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\inf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\inf_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\ini.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\ini_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\intl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\intl_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\intlnames.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\intlobj.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\inuse_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\irq.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_f_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_i_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_s_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_signal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\jrd.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\jrd_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\jrn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\jrn_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\kanji.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\kanji_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\lck.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\lck_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\license.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\llio.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\llio_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\lls.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\lnmdef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\log.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\log_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\loiter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\map_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\math.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\mblr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\mblr_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\met.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\met_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\misc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\misc_func_ids.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\misc_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\mov_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\mpexl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\msg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\msg_encode.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\include\gen\msg_facs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\include\gen\msgs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\names.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\nav_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\nlm_thd.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\nod.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\obj.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\ods.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\old.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\old_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\opt_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\pag.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\pag_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\par_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\pcmet_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\perf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\perf_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\pio.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\pio_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\pragma.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\prv_m_bypass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\pwd.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\pwd_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\quad.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\quad_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\que.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\rdb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\rdb_codes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\rdbcodes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\rec_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\relations.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\req.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\rlck_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\rng.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\rng_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\rse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\rse_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sbm.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sbm_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sch_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\scl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\scl_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\scroll_cursors.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sdl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sdl_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sdw.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sdw_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\seg_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\shdef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\shut_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sort.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sort_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sql_code.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sqz.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sqz_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\svc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\svc_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\svc_undoc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\syidef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sym_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\termtype.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\thd.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\thd_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\time.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\tpc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\tpc_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\tra.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\tra_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\trig.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\utl_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\val.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\val_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\vio_debug.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\vio_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\vms.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\vmslo_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\why_proto.h
# End Source File
# End Group
# Begin Group "lock Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\lock\fparamv3.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\lock\lock.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\lock\lock_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\lock\lockv3.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\lock\lockv3s4.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\lock\plserver.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\lock\prtv3_proto.h
# End Source File
# End Group
# Begin Group "utilities Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\utilities\cmd_util_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\dbaswi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\drpv3_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\ibmgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\ibmgrswi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\install_nt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\ppg_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\rebui_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\rebuild.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\regis_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\rmet_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\rstor_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\secur_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\servi_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\srvrmgr_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\util_proto.h
# End Source File
# End Group
# Begin Group "wal Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\wal\wal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\wal\wal_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\wal\walc_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\wal\walf_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\wal\walr_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\wal\walw_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\wal\wstat_proto.h
# End Source File
# End Group
# Begin Group "intl Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\intl\ancyrr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\anczech.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\angreek1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\anhundc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\anpolish.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\ansii850.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\ansiintl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\ansinor4.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\ansislov.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\ansispan.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\ansiswfn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\anturk.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\ascii.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\ascii2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\bllt1ca0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\bllt1da0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\bllt1de0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\bllt1es0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\bllt1fi0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\bllt1fr0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\bllt1is0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\bllt1it0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\bllt1nl0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\bllt1no0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\bllt1pt0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\bllt1sv0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\bllt1uk0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\bllt1us0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\blnxtde0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\blnxtes0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\blnxtfr0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\blnxtit0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\blnxtus0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\charsets.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\countries.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\country_codes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_437.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_850.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_852.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_857.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_860.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_861.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_863.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_865.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_big5.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_cyrl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_gb2312.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_jis_0208_1990.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_ksc5601.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_latin1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_next.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_sjis.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_w1250.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_w1251.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_w1252.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_w1253.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cs_w1254.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\csjis2_p.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\cyrr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\czech.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db437de0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db437es1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db437fi0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db437fr0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db437it0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db437nl0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db437sv0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db437uk0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db437us0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db850cf0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db850de0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db850es0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db850fr0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db850it1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db850nl0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db850pt0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db850sv1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db850uk0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db850us0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db852cz0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db852hdc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db852po0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db852sl0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db857tr0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db860pt0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db863cf1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db865da0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db865no0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\db866ru0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\hun852dc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\iceland.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\intl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\langdrv.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\ld.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\ld_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\ldcommon.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\mac_437.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\mac_865.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\mac_lat1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\nordan40.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\polish.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\slovene.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\swedfin.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\turk.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\tx437_865.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\tx437_lat1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\tx865_lat1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intl\undef.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\include\fb_blk.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\..\src\remote\caution.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\server.ico
# End Source File
# End Group
# Begin Group "jrd Source Files"

# PROP Default_Filter ""
# Begin Group "don't compile"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\jrd\aif.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\align.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dfloat_stub.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\envelope.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\evl_like.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\extvms.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\ib_stdio.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_cray.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\jio.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\jlx.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\kanji.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\lck_dummy.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\loiter.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\map.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\math.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\mem.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\memtest.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\mpexl.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\netware.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\print.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\quad.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\rdb.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\rdbgpas.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\rdbint.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\seg.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\shrinit.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\stats.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sun_ftn.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\unix.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\vms.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\vms_command.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\vmsevent.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\vmslock.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\vmsthread.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\windows.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "jrd_from_epp Source Files"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=..\..\..\generated\jrd\blob_filter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\jrd\dfw.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\jrd\dpm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\jrd\dyn.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\jrd\dyn_def.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\jrd\dyn_del.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\jrd\dyn_mod.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\jrd\dyn_util.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\jrd\fun.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\jrd\grant.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\jrd\ini.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\jrd\met.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\jrd\pcmet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\jrd\scl.cpp
# End Source File
# End Group
# Begin Group "epp Src Files"

# PROP Default_Filter "epp"
# Begin Source File

SOURCE=..\..\..\src\jrd\blob_filter.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\codes.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dfw.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dpm.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dyn.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dyn_def.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dyn_del.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dyn_mod.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dyn_util.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\envelope.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\fun.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\grant.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\ini.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\met.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\pcmet.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\scl.epp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\stats.epp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\jrd\ail.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\all.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\alt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\blb.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\bookmark.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\btr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\builtin.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\cch.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\cmp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\cvt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\cvt2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\db_alias.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dbg.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dbt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\divorce.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dllshell.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dls.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dmp.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\dsc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\enc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\err.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\event.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\evl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\exe.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\ext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\filters.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\flu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\functions.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\gds.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\grammar.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\iberr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\idx.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\inf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\intl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\intl_builtin.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\inuse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_file.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_ipc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_sync.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_sync_win32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\isc_win32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\jrd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\jrn.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\lck.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\llio.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\log.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\mblr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\misc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\os\win32\mod_loader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\mov.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\nav.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\nodebug.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\old.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\opt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\pag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\par.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\os\win32\path_utils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\pcsleep.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\plugin_manager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\pwd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\qatest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\rec.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\rlck.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\rng.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\rse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sbm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sch.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sdl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sdw.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\shrfinit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\shut.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sort.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sort_mem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sqz.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\status.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\stubs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\svc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\sym.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\thd.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\thd_win32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\tpc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\tra.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\utl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\val.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\vio.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\why.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\winnt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jrd\winnt_dll.cpp

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "FromOtherDirs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\ipserver\alli.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\cmd_util.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\generated\utilities\dba.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\fbutil\FirebirdConfig.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\fbutil\FirebirdConfigFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\gsec.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\gsec.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\gsecswi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\ipserver\ipserver.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilities\ppg.cpp
# End Source File
# End Group
# Begin Group "Lock Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\lock\lock.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\remote\ibconfig.rc

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\property.rc

!IF  "$(CFG)" == "fbserver - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "fbserver - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\remote\window.rc
# End Source File
# End Target
# End Project
