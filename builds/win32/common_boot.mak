
NULL=

CPP=cl.exe
RSC=rc.exe

OUTDIR=.\temp
INTDIR=.\temp

ALL : "$(OUTDIR)\common.lib"

CLEAN :
	-@erase "$(INTDIR)\alloc.obj"
	-@erase "$(INTDIR)\fbsyslog.obj"
	-@erase "$(INTDIR)\config.obj"
	-@erase "$(INTDIR)\config_file.obj"
	-@erase "$(INTDIR)\config_root.obj"
	-@erase "$(INTDIR)\fb_exception.obj"
	-@erase "$(INTDIR)\locks.obj"
	-@erase "$(INTDIR)\mod_loader.obj"
	-@erase "$(INTDIR)\path_utils.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\common.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /Ot /Oi /Op /Oy /Ob2 /I "../../src/include" /I "../../src/include/gen" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "SUPERSERVER" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /n /o"$(OUTDIR)\common.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\common.lib" 
LIB32_OBJS= \
	"$(INTDIR)\alloc.obj" \
	"$(INTDIR)\locks.obj" \
	"$(INTDIR)\fb_exception.obj" \
	"$(INTDIR)\config.obj" \
	"$(INTDIR)\fbsyslog.obj" \
	"$(INTDIR)\config_file.obj" \
	"$(INTDIR)\config_root.obj" \
	"$(INTDIR)\mod_loader.obj" \
	"$(INTDIR)\path_utils.obj"

"$(OUTDIR)\common.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
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


SOURCE=..\..\src\common\classes\alloc.cpp

"$(INTDIR)\alloc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\common\classes\locks.cpp

"$(INTDIR)\locks.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\common\fb_exception.cpp

"$(INTDIR)\fb_exception.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\common\config\config.cpp

"$(INTDIR)\config.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\common\config\config_file.cpp

"$(INTDIR)\config_file.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\os\win32\config_root.cpp

"$(INTDIR)\config_root.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\os\win32\mod_loader.cpp

"$(INTDIR)\mod_loader.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\os\win32\fbsyslog.cpp

"$(INTDIR)\fbsyslog.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\jrd\os\win32\path_utils.cpp

"$(INTDIR)\path_utils.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)

