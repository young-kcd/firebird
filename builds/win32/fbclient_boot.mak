
NULL=

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

OUTDIR=.\temp
INTDIR=.\temp

ALL : ".\fbclient.dll" "$(OUTDIR)\fbclient.bsc"

CLEAN :
	-@erase "$(INTDIR)\alli.obj"
	-@erase "$(INTDIR)\alli.sbr"
	-@erase "$(INTDIR)\allr.obj"
	-@erase "$(INTDIR)\allr.sbr"
	-@erase "$(INTDIR)\alt.obj"
	-@erase "$(INTDIR)\alt.sbr"
	-@erase "$(INTDIR)\array.obj"
	-@erase "$(INTDIR)\array.sbr"
	-@erase "$(INTDIR)\blob.obj"
	-@erase "$(INTDIR)\blob.sbr"
	-@erase "$(INTDIR)\config_file.obj"
	-@erase "$(INTDIR)\config_file.sbr"
	-@erase "$(INTDIR)\config_root.obj"
	-@erase "$(INTDIR)\config_root.sbr"
	-@erase "$(INTDIR)\cvt.obj"
	-@erase "$(INTDIR)\cvt.sbr"
	-@erase "$(INTDIR)\dls.obj"
	-@erase "$(INTDIR)\dls.sbr"
	-@erase "$(INTDIR)\dsc.obj"
	-@erase "$(INTDIR)\dsc.sbr"
	-@erase "$(INTDIR)\enc.obj"
	-@erase "$(INTDIR)\enc.sbr"
	-@erase "$(INTDIR)\gds.obj"
	-@erase "$(INTDIR)\gds.sbr"
	-@erase "$(INTDIR)\ibinitdll.obj"
	-@erase "$(INTDIR)\ibinitdll.sbr"
	-@erase "$(INTDIR)\inet.obj"
	-@erase "$(INTDIR)\inet.sbr"
	-@erase "$(INTDIR)\interface.obj"
	-@erase "$(INTDIR)\interface.sbr"
	-@erase "$(INTDIR)\ipcevent.obj"
	-@erase "$(INTDIR)\ipcevent.sbr"
	-@erase "$(INTDIR)\ipclient.obj"
	-@erase "$(INTDIR)\ipclient.sbr"
	-@erase "$(INTDIR)\isc.obj"
	-@erase "$(INTDIR)\isc.sbr"
	-@erase "$(INTDIR)\isc_file.obj"
	-@erase "$(INTDIR)\isc_file.sbr"
	-@erase "$(INTDIR)\isc_ipc.obj"
	-@erase "$(INTDIR)\isc_ipc.sbr"
	-@erase "$(INTDIR)\isc_sync.obj"
	-@erase "$(INTDIR)\isc_sync.sbr"
	-@erase "$(INTDIR)\keywords.obj"
	-@erase "$(INTDIR)\keywords.sbr"
	-@erase "$(INTDIR)\merge.obj"
	-@erase "$(INTDIR)\merge.sbr"
	-@erase "$(INTDIR)\parser.obj"
	-@erase "$(INTDIR)\parser.sbr"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\perf.sbr"
	-@erase "$(INTDIR)\preparse.obj"
	-@erase "$(INTDIR)\preparse.sbr"
	-@erase "$(INTDIR)\protocol.obj"
	-@erase "$(INTDIR)\protocol.sbr"
	-@erase "$(INTDIR)\remote.obj"
	-@erase "$(INTDIR)\remote.sbr"
	-@erase "$(INTDIR)\sch.obj"
	-@erase "$(INTDIR)\sch.sbr"
	-@erase "$(INTDIR)\sdl.obj"
	-@erase "$(INTDIR)\sdl.sbr"
	-@erase "$(INTDIR)\status.obj"
	-@erase "$(INTDIR)\status.sbr"
	-@erase "$(INTDIR)\thd.obj"
	-@erase "$(INTDIR)\thd.sbr"
	-@erase "$(INTDIR)\user_dsql.obj"
	-@erase "$(INTDIR)\user_dsql.sbr"
	-@erase "$(INTDIR)\utl.obj"
	-@erase "$(INTDIR)\utl.sbr"
	-@erase "$(INTDIR)\utld.obj"
	-@erase "$(INTDIR)\utld.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\version.res"
	-@erase "$(INTDIR)\why.obj"
	-@erase "$(INTDIR)\why.sbr"
	-@erase "$(INTDIR)\wnet.obj"
	-@erase "$(INTDIR)\wnet.sbr"
	-@erase "$(INTDIR)\xdr.obj"
	-@erase "$(INTDIR)\xdr.sbr"
	-@erase "$(INTDIR)\xnet.obj"
	-@erase "$(INTDIR)\xnet.sbr"
	-@erase "$(OUTDIR)\fbclient.bsc"
	-@erase "$(OUTDIR)\fbclient.exp"
	-@erase "$(OUTDIR)\fbclient.lib"
	-@erase ".\fbclient.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /Ot /Oi /Op /Oy /Ob2 /I "../../src/include" /I "../../src/include/gen" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "CLIENT" /D "SUPERCLIENT" /D "SERVER_SHUTDOWN" /D "IPSERV" /D "GOVERNOR" /D "EXACT_NUMERICS" /D "I386" /D _X86_=1 /D "WIN32" /D "_MBCS" /D "_X86_" /D "BOOT_BUILD" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /n /o"$(OUTDIR)\fbclient.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\alt.sbr" \
	"$(INTDIR)\cvt.sbr" \
	"$(INTDIR)\dls.sbr" \
	"$(INTDIR)\dsc.sbr" \
	"$(INTDIR)\enc.sbr" \
	"$(INTDIR)\gds.sbr" \
	"$(INTDIR)\ibinitdll.sbr" \
	"$(INTDIR)\isc.sbr" \
	"$(INTDIR)\isc_file.sbr" \
	"$(INTDIR)\isc_ipc.sbr" \
	"$(INTDIR)\isc_sync.sbr" \
	"$(INTDIR)\perf.sbr" \
	"$(INTDIR)\sch.sbr" \
	"$(INTDIR)\sdl.sbr" \
	"$(INTDIR)\status.sbr" \
	"$(INTDIR)\thd.sbr" \
	"$(INTDIR)\utl.sbr" \
	"$(INTDIR)\why.sbr" \
	"$(INTDIR)\array.sbr" \
	"$(INTDIR)\blob.sbr" \
	"$(INTDIR)\keywords.sbr" \
	"$(INTDIR)\preparse.sbr" \
	"$(INTDIR)\user_dsql.sbr" \
	"$(INTDIR)\utld.sbr" \
	"$(INTDIR)\alli.sbr" \
	"$(INTDIR)\ipcevent.sbr" \
	"$(INTDIR)\ipclient.sbr" \
	"$(INTDIR)\allr.sbr" \
	"$(INTDIR)\inet.sbr" \
	"$(INTDIR)\interface.sbr" \
	"$(INTDIR)\merge.sbr" \
	"$(INTDIR)\parser.sbr" \
	"$(INTDIR)\protocol.sbr" \
	"$(INTDIR)\remote.sbr" \
	"$(INTDIR)\wnet.sbr" \
	"$(INTDIR)\xdr.sbr" \
	"$(INTDIR)\xnet.sbr" \
	"$(INTDIR)\config_file.sbr" \
	"$(INTDIR)\config_root.sbr" \

"$(OUTDIR)\fbclient.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib shell32.lib comctl32.lib advapi32.lib ws2_32.lib mpr.lib version.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\fbclient.pdb" /machine:I386 /def:"defs/fbclient.def" /out:"fbclient.dll" /implib:"$(OUTDIR)\fbclient.lib" /pdbtype:sept 
DEF_FILE= \
	"defs/fbclient.def"
LINK32_OBJS= \
	"$(INTDIR)\alt.obj" \
	"$(INTDIR)\cvt.obj" \
	"$(INTDIR)\dls.obj" \
	"$(INTDIR)\dsc.obj" \
	"$(INTDIR)\enc.obj" \
	"$(INTDIR)\gds.obj" \
	"$(INTDIR)\ibinitdll.obj" \
	"$(INTDIR)\isc.obj" \
	"$(INTDIR)\isc_file.obj" \
	"$(INTDIR)\isc_ipc.obj" \
	"$(INTDIR)\isc_sync.obj" \
	"$(INTDIR)\perf.obj" \
	"$(INTDIR)\sch.obj" \
	"$(INTDIR)\sdl.obj" \
	"$(INTDIR)\status.obj" \
	"$(INTDIR)\thd.obj" \
	"$(INTDIR)\utl.obj" \
	"$(INTDIR)\why.obj" \
	"$(INTDIR)\array.obj" \
	"$(INTDIR)\blob.obj" \
	"$(INTDIR)\keywords.obj" \
	"$(INTDIR)\preparse.obj" \
	"$(INTDIR)\user_dsql.obj" \
	"$(INTDIR)\utld.obj" \
	"$(INTDIR)\alli.obj" \
	"$(INTDIR)\ipcevent.obj" \
	"$(INTDIR)\ipclient.obj" \
	"$(INTDIR)\allr.obj" \
	"$(INTDIR)\inet.obj" \
	"$(INTDIR)\interface.obj" \
	"$(INTDIR)\merge.obj" \
	"$(INTDIR)\parser.obj" \
	"$(INTDIR)\protocol.obj" \
	"$(INTDIR)\remote.obj" \
	"$(INTDIR)\wnet.obj" \
	"$(INTDIR)\xdr.obj" \
	"$(INTDIR)\xnet.obj" \
	"$(INTDIR)\config_file.obj" \
	"$(INTDIR)\config_root.obj" \
	"$(INTDIR)\version.res" \
	".\temp\common.lib"

".\fbclient.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x41d /fo"$(INTDIR)\version.res"

SOURCE=..\..\src\jrd\alt.cpp

"$(INTDIR)\alt.obj"	"$(INTDIR)\alt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\cvt.cpp

"$(INTDIR)\cvt.obj"	"$(INTDIR)\cvt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\dls.cpp

"$(INTDIR)\dls.obj"	"$(INTDIR)\dls.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\dsc.cpp

"$(INTDIR)\dsc.obj"	"$(INTDIR)\dsc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\enc.cpp

"$(INTDIR)\enc.obj"	"$(INTDIR)\enc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\gds.cpp

"$(INTDIR)\gds.obj"	"$(INTDIR)\gds.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\ibinitdll.cpp

"$(INTDIR)\ibinitdll.obj"	"$(INTDIR)\ibinitdll.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\isc.cpp

"$(INTDIR)\isc.obj"	"$(INTDIR)\isc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\isc_file.cpp

"$(INTDIR)\isc_file.obj"	"$(INTDIR)\isc_file.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\isc_ipc.cpp

"$(INTDIR)\isc_ipc.obj"	"$(INTDIR)\isc_ipc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\isc_sync.cpp

"$(INTDIR)\isc_sync.obj"	"$(INTDIR)\isc_sync.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\perf.cpp

"$(INTDIR)\perf.obj"	"$(INTDIR)\perf.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\sch.cpp

"$(INTDIR)\sch.obj"	"$(INTDIR)\sch.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\sdl.cpp

"$(INTDIR)\sdl.obj"	"$(INTDIR)\sdl.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\status.cpp

"$(INTDIR)\status.obj"	"$(INTDIR)\status.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\thd.cpp

"$(INTDIR)\thd.obj"	"$(INTDIR)\thd.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\utl.cpp

"$(INTDIR)\utl.obj"	"$(INTDIR)\utl.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\why.cpp

"$(INTDIR)\why.obj"	"$(INTDIR)\why.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\generated\dsql\array.cpp

"$(INTDIR)\array.obj"	"$(INTDIR)\array.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\generated\dsql\blob.cpp

"$(INTDIR)\blob.obj"	"$(INTDIR)\blob.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\dsql\keywords.cpp

"$(INTDIR)\keywords.obj"	"$(INTDIR)\keywords.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\dsql\preparse.cpp

"$(INTDIR)\preparse.obj"	"$(INTDIR)\preparse.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\dsql\user_dsql.cpp

"$(INTDIR)\user_dsql.obj"	"$(INTDIR)\user_dsql.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\dsql\utld.cpp

"$(INTDIR)\utld.obj"	"$(INTDIR)\utld.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\ipserver\alli.cpp

"$(INTDIR)\alli.obj"	"$(INTDIR)\alli.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\ipserver\ipcevent.cpp

"$(INTDIR)\ipcevent.obj"	"$(INTDIR)\ipcevent.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\ipserver\ipclient.cpp

"$(INTDIR)\ipclient.obj"	"$(INTDIR)\ipclient.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\remote\allr.cpp

"$(INTDIR)\allr.obj"	"$(INTDIR)\allr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\remote\inet.cpp

"$(INTDIR)\inet.obj"	"$(INTDIR)\inet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\remote\interface.cpp

"$(INTDIR)\interface.obj"	"$(INTDIR)\interface.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\remote\merge.cpp

"$(INTDIR)\merge.obj"	"$(INTDIR)\merge.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\remote\parser.cpp

"$(INTDIR)\parser.obj"	"$(INTDIR)\parser.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\remote\protocol.cpp

"$(INTDIR)\protocol.obj"	"$(INTDIR)\protocol.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\remote\remote.cpp

"$(INTDIR)\remote.obj"	"$(INTDIR)\remote.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\remote\wnet.cpp

"$(INTDIR)\wnet.obj"	"$(INTDIR)\wnet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\remote\xdr.cpp

"$(INTDIR)\xdr.obj"	"$(INTDIR)\xdr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\remote\xnet.cpp

"$(INTDIR)\xnet.obj"	"$(INTDIR)\xnet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\common\config\config_file.cpp

"$(INTDIR)\config_file.obj"	"$(INTDIR)\config_file.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\os\win32\config_root.cpp

"$(INTDIR)\config_root.obj"	"$(INTDIR)\config_root.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\version.rc

"$(INTDIR)\version.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x41d /fo"$(INTDIR)\version.res" $(SOURCE)


