
NULL=

CPP=cl.exe
RSC=rc.exe

OUTDIR=.\temp
INTDIR=.\temp

ALL : ".\codes.exe"
CLEAN :
	-@erase "$(INTDIR)\codes.obj"
	-@erase "$(INTDIR)\fb_exception.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\version.res"
	-@erase ".\codes.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /Ot /Og /Oi /Op /Oy /Ob1 /I "../../src/include" /I "../../src/include/gen" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "SUPERCLIENT" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /EHc- /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\codes.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib shell32.lib comctl32.lib advapi32.lib ws2_32.lib mpr.lib version.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\codes.pdb" /machine:I386 /out:"./codes.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\fb_exception.obj" \
	"$(INTDIR)\version.res" \
	"$(INTDIR)\codes.obj" \
	".\temp\fbclient.lib"

".\codes.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

RSC_PROJ=/l 0x41d /fo"$(INTDIR)\version.res" /d "NDEBUG" 

SOURCE=..\..\gen\jrd\codes.cpp

"$(INTDIR)\codes.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\common\fb_exception.cpp

"$(INTDIR)\fb_exception.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\version.rc

"$(INTDIR)\version.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x41d /fo"$(INTDIR)\version.res" /i "\Firebird\firebird_15\src\jrd" /d "NDEBUG" $(SOURCE)
