::  Initial Developer's Public License.
::  The contents of this file are subject to the  Initial Developer's Public
::  License Version 1.0 (the "License"). You may not use this file except
::  in compliance with the License. You may obtain a copy of the License at
::    http://www.ibphoenix.com?a=ibphoenix&page=ibp_idpl
::  Software distributed under the License is distributed on an "AS IS" basis,
::  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
::  for the specific language governing rights and limitations under the
::  License.
::
::  The Original Code is copyright 2003-2004 Paul Reeves.
::
::  The Initial Developer of the Original Code is Paul Reeves
::
::  All Rights Reserved.
::
::=============================================================================
@echo off

if DEFINED VS71COMNTOOLS (
@devenv /? >nul 2>nul
@if errorlevel 9009 (call "%VS71COMNTOOLS%vsvars32.bat") else ( echo "%VS71COMNTOOLS%vsvars32.bat has already been executed.")
) else (
@msdev /? >nul 2>nul
@if errorlevel 9009 (call "C:\Program Files\Microsoft Visual Studio\VC98\Bin\vcvars32.bat") else (echo "MSVC6 vcvars.bat has already been executed")
)



@goto :MAIN
@goto :EOF
::============================================================================

:SET_PARAMS
@echo off
::Assume we are preparing a production build
set FBBUILD_BUILDTYPE=release
::Don't ship pdb files by default
set FBBUILD_SHIP_PDB=no_pdb

:: Reset "make" vars to null
set FBBUILD_ZIP_PACK=0
set FBBUILD_ISX_PACK=0
set FBBUILD_EMB_PACK=0

:: Hard code our package number - it only needs to be changed if the
:: kit is repackaged but the engine is not rebuilt
set FBBUILD_PACKAGE_NUMBER=0

:: See what we have on the command line
for %%v in ( %* )  do (
  ( if /I "%%v"=="DEBUG" (set FBBUILD_BUILDTYPE=debug) )
  ( if /I "%%v"=="PDB" (set FBBUILD_SHIP_PDB=ship_pdb) )
  ( if /I "%%v"=="ZIP" (set FBBUILD_ZIP_PACK=1) )
  ( if /I "%%v"=="ISX" (set FBBUILD_ISX_PACK=1) )
  ( if /I "%%v"=="EMB" (set FBBUILD_EMB_PACK=1) )
  ( if /I "%%v"=="ALL" ( (set FBBUILD_ZIP_PACK=1) & (set FBBUILD_ISX_PACK=1) & (set FBBUILD_EMB_PACK=1) ) )
)
:: Now check whether we are debugging the InnoSetup script

@if %FB2_ISS_DEBUG% equ 0 (@set ISS_BUILD_TYPE=iss_release) else (@set ISS_BUILD_TYPE=iss_debug)
@if %FB2_ISS_DEBUG% equ 0 (@set ISS_COMPRESS=compression) else (@set ISS_COMPRESS=nocompression)
:: And check whether we have built examples
@if %FB2_EXAMPLES% equ 0 (@set ISS_EXAMPLES=noexamples) else (@set ISS_EXAMPLES=examples)



:: Set up our final destination
set FBBUILD_INSTALL_IMAGES=%ROOT_PATH%\builds\install_images
if not exist %FBBUILD_INSTALL_IMAGES% (mkdir %FBBUILD_INSTALL_IMAGES%)
set FBBUILD_OUTPUT=%ROOT_PATH%\output
set FBBUILD_INSTALL_ROOT=%ROOT_PATH%\src\install


::End of SET_PARAMS
::-----------------
@goto :EOF


:CHECK_ENVIRONMENT
::================
:: Make sure we have everything we need. If something is missing then
:: let's bail out now.

if not DEFINED FB_EXTERNAL_DOCS (
 @echo.
 @echo The FB_EXTERNAL_DOCS environment var is not defined
 @echo It should point to the directory containing the relevant release notes
 @echo in adobe pdf format.
 @echo.
 @echo Subsequent script execution will be cancelled.
 @echo.
 cancel_script > nul 2>&1
 goto :EOF
)

sed -V | findstr version > nul
@if %ERRORLEVEL% GEQ 1 (
    call :ERROR Could not locate sed
    goto :EOF
) else (@echo     o sed found.)

if %FBBUILD_ZIP_PACK% EQU 1 (
  if not defined PKZIP (
    call :ERROR PKZIP environment variable is not defined.
    @goto :EOF
  ) else (@echo     o Compression utility found.)
)


if %FBBUILD_ISX_PACK% EQU 1 (
  if NOT DEFINED INNO_SETUP_PATH (
    call :ERROR INNO_SETUP_PATH variable not defined
    @goto :EOF
  ) else (@echo     o Inno Setup found at %INNO_SETUP_PATH%.)
)

::End of CHECK_ENVIRONMENT
::------------------------
@goto :EOF


:SED_MAGIC
:: Do some sed magic to make sure that the final product
:: includes the version string in the filename.
:: If the Firebird Unix tools for Win32 aren't on
:: the path this will fail! Use of the cygwin tools has not
:: been tested and may produce unexpected results.
::========================================================
find "#define PRODUCT_VER_STRING" %ROOT_PATH%\src\jrd\build_no.h > %temp%.\b$1.txt
sed -n -e s/\"//g -e s/"#define PRODUCT_VER_STRING "//w%temp%.\b$2.txt %temp%.\b$1.txt
for /f "tokens=*" %%a in ('type %temp%.\b$2.txt') do set PRODUCT_VER_STRING=%%a
@echo s/1.5.0/%PRODUCT_VER_STRING%/ > %temp%.\b$3.txt
@echo s/define release/define %FBBUILD_BUILDTYPE%/ >> %temp%.\b$3.txt
@echo s/define msvc_version 6/define msvc_version %MSVC_VERSION%/ >> %temp%.\b$3.txt
@echo s/define no_pdb/define %FBBUILD_SHIP_PDB%/ >> %temp%.\b$3.txt
@echo s/define package_number=\"0\"/define package_number=\"%FBBUILD_PACKAGE_NUMBER%\"/ >> %temp%.\b$3.txt
@echo s/define iss_release/define %ISS_BUILD_TYPE%/ >> %temp%.\b$3.txt
@echo s/define examples/define %ISS_EXAMPLES%/ >> %temp%.\b$3.txt
@echo s/define compression/define %ISS_COMPRESS%/ >> %temp%.\b$3.txt
@echo s/PRODUCT_VER_STRING/%PRODUCT_VER_STRING%/ >> %temp%.\b$3.txt

sed -f  %temp%.\b$3.txt FirebirdInstall_15.iss > FirebirdInstall_%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%.iss

del %temp%.\b$?.txt
::End of SED_MAGIC
::----------------
@goto :EOF


:COPY_XTRA
:: system dll's we need
:: (You may need to download and extract the
:: vcredist stuff to your MSDevDir for this to work.
::=====================
@if DEFINED MSDevDir (
@if %msvc_version% EQU 6 (
 (@copy "%MSDevDir%\vcredist\msvcrt.dll" %ROOT_PATH%\output\system32\ 2>nul)
 (@copy "%MSDevDir%\vcredist\msvcp%msvc_version%0.dll" %ROOT_PATH%\output\system32\ 2>nul)
)
)

:: grab some missing bits'n'pieces from different parts of the source tree
::=========================================================================
@echo Copying firebird.conf
@copy %FBBUILD_INSTALL_ROOT%\misc\firebird.conf %ROOT_PATH%\output\ > nul
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY of firebird.conf failed with errorlevel %ERRORLEVEL% ) & (goto :EOF))

@echo   Copying ib_util etc
for %%v in ( ib_util.h ib_util.pas ) do (
  ((copy %ROOT_PATH%\src\extlib\%%v %FBBUILD_OUTPUT%\include\%%v > nul) || 	(@echo Copying %%v failed.))
)


@echo   Copying fbclient lib etc
for %%v in (fbclient ib_util) do @(
@copy /Y %ROOT_PATH%\temp\%FBBUILD_BUILDTYPE%\%%v\%%v.lib %FBBUILD_OUTPUT%\lib\%%v_ms.lib || @if %ERRORLEVEL% GEQ 1 (call :ERROR Copying %%v to %%v_ms.lib failed with error %ERRORLEVEL%) & (goto :EOF))
)

::Allow fbserver to access upto 3GB RAM, if it is available.
dumpbin /headers %ROOT_PATH%\output\bin\fbserver.exe | findstr /C:"Application can handle large (>2GB) addresses" || editbin /largeaddressaware %ROOT_PATH%\output\bin\fbserver.exe


::Generate .lib compatible with Borland compiler.
@implib.exe | findstr "Borland" > nul
@if errorlevel 0 (
  @echo   Generating fbclient_bor.lib
  @implib %FBBUILD_OUTPUT%\lib\fbclient_bor.lib %FBBUILD_OUTPUT%\bin\fbclient.dll > nul
)

@if "%FBBUILD_SHIP_PDB%"=="ship_pdb" (
  @echo   Copying pdb files...
  for %%v in ( fbembed fb_inet_server fbserver fbclient ) do (
    @copy %ROOT_PATH%\temp\%FBBUILD_BUILDTYPE%\%%v\%%v.pdb %FBBUILD_OUTPUT%\bin > nul
    @if %ERRORLEVEL% GEQ 1 (
      call :ERROR Copying %%v.pdb files failed
      goto :EOF
    )
  )
)
::  @copy %ROOT_PATH%\temp\%FBBUILD_BUILDTYPE%\firebird\bin\*.pdb %FBBUILD_OUTPUT%\bin > nul

@echo Started copying docs...
@rmdir /S /Q %ROOT_PATH%\output\doc 2>nul
@mkdir %ROOT_PATH%\output\doc
@copy %ROOT_PATH%\ChangeLog %ROOT_PATH%\output\doc\ChangeLog.txt >nul
@copy %ROOT_PATH%\doc\*.* %FBBUILD_OUTPUT%\doc\ > nul
@if %ERRORLEVEL% GEQ 1 (
  call :ERROR COPY of main documentation tree failed with error %ERRORLEVEL%
  goto :EOF
)

@echo Copying udf library scripts...
@copy %ROOT_PATH%\src\extlib\ib_udf.sql  %FBBUILD_OUTPUT%\UDF\ib_udf.sql > nul
@if %ERRORLEVEL% GEQ 1 (
  call :ERROR Copying %ROOT_PATH%\src\extlib\%%v failed.
  goto :EOF
)

for %%v in ( fbudf.sql fbudf.txt ) do (
  copy %ROOT_PATH%\src\extlib\fbudf\%%v  %FBBUILD_OUTPUT%\UDF\%%v > nul
  @if %ERRORLEVEL% GEQ 1 (
    call :ERROR Copying %ROOT_PATH%\src\extlib\%%v failed with error %ERRORLEVEL%
    goto :EOF
  )
)


@echo   Copying other documentation...
@copy %FBBUILD_INSTALL_ROOT%\arch-specific\win32\installation_readme.txt %ROOT_PATH%\output\doc\installation_readme.txt > nul

:: This stuff doesn't make much sense to Windows users, although the troubleshooting doc
:: could be made more platform agnostic.
@for %%v in (  README.makefiles README.user README.user.embedded README.user.troubleshooting README.build.mingw.html README.build.msvc.html fb2-todo.txt install_win32.txt README.install.Solaris_on_Intel *.*~) do (
  (@del %ROOT_PATH%\output\doc\%%v 2>nul)
)

@copy %FBBUILD_OUTPUT%\doc\WhatsNew %FBBUILD_OUTPUT%\doc\WhatsNew.txt > nul
@del %FBBUILD_OUTPUT%\doc\WhatsNew

@mkdir %ROOT_PATH%\output\doc\sql.extensions 2>nul
@if %ERRORLEVEL% GEQ 2 ( (call :ERROR MKDIR for doc\sql.extensions dir failed ) & (goto :EOF))

@mkdir %FBBUILD_OUTPUT%\doc\sql.extensions 2>nul
@if %ERRORLEVEL% GEQ 2 ( (call :ERROR MKDIR for doc\sql.extensions dir failed) & (@goto :EOF))
@copy %ROOT_PATH%\doc\sql.extensions\*.* %FBBUILD_OUTPUT%\doc\sql.extensions\ > nul
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR Copying doc\sql.extensions failed  ) & (goto :EOF))

@echo   Copying pdf docs...
@for %%v in (Firebird-1.5-QuickStart.pdf Firebird_v1.5.5.ReleaseNotes.pdf ) do (
  @echo     ... %%v
  (@copy /Y %FB_EXTERNAL_DOCS%\%%v %FBBUILD_OUTPUT%\doc\%%v > nul) || (call :WARNING Copying %FB_EXTERNAL_DOCS%\%%v failed.)
)

:: Clean out text notes that are either not relevant to Windows or
:: are only of use to engine developers.
@for %%v in (  README.makefiles README.user README.user.embedded README.user.troubleshooting README.build.mingw.html README.build.msvc.html fb2-todo.txt cleaning-todo.txt install_win32.txt README.coding.style emacros-cross_ref.html *.*~) do (
  @del %FBBUILD_OUTPUT%\doc\%%v 2>nul
)

:: Add license
for %%v in (IPLicense.txt IDPLicense.txt ) do (
	@copy %FBBUILD_INSTALL_ROOT%\misc\%%v %FBBUILD_OUTPUT%\%%v > nul
)

:: And readme
@copy %FBBUILD_INSTALL_ROOT%\arch-specific\win32\readme.txt %FBBUILD_OUTPUT%\ > nul


:: Throw away any errorlevel left hanging around
@set | findstr win > nul

::rename the examples directory
:: /Y is not necessary in a script, but is added to document
:: that no prompting will take place
if EXIST %ROOT_PATH%\output\v5_examples (
@move /Y %ROOT_PATH%\output\v5_examples %ROOT_PATH%\output\examples > nul 2>&1
)

@echo   Completed copying docs.
::End of COPY_XTRA
::----------------
@goto :EOF

:IBASE_H
:: Concatenate header files into ibase.h
::======================================
:: o This section of code takes two header files, strips comments and
::   inserts them into ibase.h for distribution. The only drawback is that
::   it strips all the comments.
:: o No error checking is done.
:: o Take note that different versions of sed use different
::   string delimiters. The firebird_tools version uses double quotes - ".
::   The cygwin one probably uses single quotes.
:: o The script 'strip_comments.sed' is taken from
::      http://sed.sourceforge.net/grabbag/scripts/testo.htm

setlocal
set OUTPATH=%FBBUILD_OUTPUT%\include
for %%v in ( %ROOT_PATH%\src\include\fb_types.h %ROOT_PATH%\src\jrd\blr.h ) do (
  del %OUTPATH%\%%~nxv 2> nul
  copy %%v %OUTPATH%\%%~nxv > nul
  sed -n -f strip_comments.sed %OUTPATH%\%%~nxv > %OUTPATH%\%%~nv.more
  more /s %OUTPATH%\%%~nv.more > %OUTPATH%\%%~nv.sed
)
move /y %OUTPATH%\ibase.h %OUTPATH%\ibase.sed
sed -e "/#include \"fb_types\.h\"/r %OUTPATH%\fb_types.h" -e "/#include \"fb_types\.h\"/d" -e "/#include \"blr\.h\"/r %OUTPATH%\blr.h" -e "/#include \"blr\.h\"/d" %OUTPATH%\ibase.sed > %OUTPATH%\ibase.h
del %OUTPATH%\ibase.sed %OUTPATH%\fb_types.* %OUTPATH%\blr.*
endlocal

::End of IBASE_H
::--------------
@goto :EOF


:ALIAS_CONF
:: Generate a sample aliases file
::===============================
@echo Creating sample aliases.conf
@echo # > %FBBUILD_OUTPUT%\aliases.conf
@echo # List of known database aliases >> %FBBUILD_OUTPUT%\aliases.conf
@echo # ------------------------------ >> %FBBUILD_OUTPUT%\aliases.conf
@echo # >> %FBBUILD_OUTPUT%\aliases.conf
@echo # Examples: >> %FBBUILD_OUTPUT%\aliases.conf
@echo # >> %FBBUILD_OUTPUT%\aliases.conf
@echo #   dummy = c:\data\dummy.fdb >> %FBBUILD_OUTPUT%\aliases.conf
@echo #  >> %FBBUILD_OUTPUT%\aliases.conf

::End of ALIAS_CONF
::-----------------
@goto :EOF


:GBAK_SEC_DB
@echo   Let's make sure that we have a backup of the security database handy.
::======================================================================
@copy %ROOT_PATH%\src\misc\security.gbak %ROOT_PATH%\output\security.fbk > nul
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR copy security.fbk failed ) & (goto :EOF))


:: Make sure that qli's help.fdb is available
::===============================================
@if not exist %FBBUILD_OUTPUT%\help\help.fdb (
	(@echo   Copying help.fdb for qli support)
	(@copy %ROOT_PATH%\gen\dbs\help.fdb %FBBUILD_OUTPUT%\help\help.fdb > nul)
	(@if %ERRORLEVEL% GEQ 1 ( (call :ERROR Could not copy qli help database ) & (goto :EOF)))
)

::End of GBAK_SEC_DB
::------------------
@goto :EOF


:FB_MSG
::=================================================================
:: firebird.msg is generated as part of the build process
:: in builds\win32 by build_msg.bat. Copying from there to output dir
::=================================================================
@if not exist %FBBUILD_OUTPUT%\firebird.msg (
	(@copy %ROOT_PATH%\gen\firebird.msg %FBBUILD_OUTPUT%\firebird.msg > nul)
	(@if %ERRORLEVEL% GEQ 1 ( (call :ERROR Could not copy firebird.msg ) & (goto :EOF)))
)

::End of FB_MSG
::-------------
@goto :EOF

:TOUCH_LOCAL
::==========
:: Note 1: This doesn't seem to make any difference to whether local libraries are used.
:: Note 2: MS documentation was incorrectly interpreted. .local files should not be created
::         for libraries, they should be created for executables.
:: Create libname.local files for each locally installed library
::for %%v in ( fbclient msvcrt msvcp%MSVC_VERSION%0 )  do touch %ROOT_PATH%\output\bin\%%v.local
@goto :EOF


:GEN_ZIP
::======
if %FBBUILD_ZIP_PACK% EQU 0 goto :EOF
:: Generate the directory tree to be zipped
set FBBUILD_ZIP_PACK_ROOT=%ROOT_PATH%\builds\zip_pack
if not exist %FBBUILD_ZIP_PACK_ROOT% @mkdir %FBBUILD_ZIP_PACK_ROOT% 2>nul
@del /s /q %FBBUILD_ZIP_PACK_ROOT%\ > nul
@copy /Y %FBBUILD_OUTPUT% %FBBUILD_ZIP_PACK_ROOT% > nul
for %%v in (bin doc doc\sql.extensions help include intl lib udf examples ) do (
	@mkdir %FBBUILD_ZIP_PACK_ROOT%\%%v 2>nul
	@copy /Y %FBBUILD_OUTPUT%\%%v\*.* %FBBUILD_ZIP_PACK_ROOT%\%%v\ > nul
)

:: Now remove stuff that is not needed.
for %%v in (bin\gpre_boot.exe bin\gpre_static.exe doc\installation_readme.txt bin\fbembed.dll bin\fbembed.pdb ) do (
  @del %FBBUILD_ZIP_PACK_ROOT%\%%v
)


if not "%FBBUILD_SHIP_PDB%"=="ship_pdb" (
  @del /q %FBBUILD_ZIP_PACK_ROOT%\bin\*.pdb > nul 2>&1
)

::grab install notes for zip pack
@copy %ROOT_PATH%\doc\install_win32.txt %FBBUILD_ZIP_PACK_ROOT%\doc\README_installation.txt > nul

::End of GEN_ZIP
::--------------
goto :EOF


:ZIP_PACK
::=======
if "%FBBUILD_ZIP_PACK%" NEQ "1" goto :EOF
  if "%FBBUILD_SHIP_PDB%" == "ship_pdb" (
    del %FBBUILD_INSTALL_IMAGES%\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_win32_pdb.zip
    @%PKZIP%\pkzip25.exe -level=9 -add=update -rec -path=relative %FBBUILD_INSTALL_IMAGES%\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_win32_pdb.zip %FBBUILD_ZIP_PACK_ROOT%\*.*
) else (
    del %FBBUILD_INSTALL_IMAGES%\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_win32.zip
    @%PKZIP%\pkzip25.exe -level=9 -add=update -rec -path=relative -exclude=*.pdb %FBBUILD_INSTALL_IMAGES%\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_win32.zip %FBBUILD_ZIP_PACK_ROOT%\*.*
)

::End of ZIP_PACK
::----------------
@goto :EOF


:GEN_EMBEDDED
::===========
:: Generate the directory tree for the embedded zip pack
if %FBBUILD_EMB_PACK% EQU 0 goto :EOF
set FBBUILD_EMB_PACK_ROOT=%ROOT_PATH%\builds\emb_pack
@mkdir %FBBUILD_EMB_PACK_ROOT% 2>nul
@del /s /q %FBBUILD_EMB_PACK_ROOT%\ > nul

for %%v in (aliases.conf firebird.conf firebird.msg) do (	@copy /Y %FBBUILD_OUTPUT%\%%v %FBBUILD_EMB_PACK_ROOT%\%%v > nul)

for %%v in ( doc intl udf ) do (@mkdir %FBBUILD_EMB_PACK_ROOT%\%%v 2>nul)

@copy /Y %ROOT_PATH%\temp\release\firebird\bin\fbembed.* %FBBUILD_EMB_PACK_ROOT% > nul
@copy /Y %ROOT_PATH%\temp\release\firebird\bin\ib_util.dll %FBBUILD_EMB_PACK_ROOT% > nul
@copy /Y %FBBUILD_OUTPUT%\doc\Firebird*.pdf %FBBUILD_EMB_PACK_ROOT%\doc\ > nul
@copy /Y %FBBUILD_OUTPUT%\intl\*.* %FBBUILD_EMB_PACK_ROOT%\intl\ > nul
@copy /Y %FBBUILD_OUTPUT%\udf\*.* %FBBUILD_EMB_PACK_ROOT%\udf\ > nul

if "%FBBUILD_SHIP_PDB%"=="ship_pdb" (
  @copy /Y %ROOT_PATH%\temp\release\fbembed\fbembed.pdb %FBBUILD_EMB_PACK_ROOT% > nul
)


:: grab install notes for embedded zip pack
@copy %ROOT_PATH%\doc\README.user.embedded %FBBUILD_EMB_PACK_ROOT%\doc\README_embedded.txt > nul
@copy %ROOT_PATH%\doc\WhatsNew %FBBUILD_EMB_PACK_ROOT%\doc\WhatsNew.txt > nul

:: Add license
for %%v in (IPLicense.txt IDPLicense.txt ) do (
  @copy %FBBUILD_INSTALL_ROOT%\misc\%%v %FBBUILD_EMB_PACK_ROOT%\%%v > nul
)

::And readme
@copy %FBBUILD_INSTALL_ROOT%\arch-specific\win32\readme.txt %FBBUILD_EMB_PACK_ROOT%\ > nul


::End of GEN_EMBEDDED
::-------------------
@goto :EOF


:EMB_PACK
::=======
if "%FBBUILD_EMB_PACK%" NEQ "1" goto :EOF
@echo Now building embedded package
::Now we can zip it up and copy the package to the install images directory.
  if "%FBBUILD_SHIP_PDB%" == "ship_pdb" (
    del %FBBUILD_INSTALL_IMAGES%\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_embed_win32_pdb.zip
    %PKZIP%\pkzip25.exe -level=9 -add=update -rec -path=relative %FBBUILD_INSTALL_IMAGES%\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_embed_win32_pdb.zip %FBBUILD_EMB_PACK_ROOT%\*.*
  ) else (
    del %FBBUILD_INSTALL_IMAGES%\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_embed_win32.zip
    %PKZIP%\pkzip25.exe -level=9 -add=update -rec -path=relative -exclude=*.pdb %FBBUILD_INSTALL_IMAGES%\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_embed_win32.zip %FBBUILD_EMB_PACK_ROOT%\*.*
	)

::End of EMB_PACK
::---------------
goto :EOF


:TOUCH_ALL
::========
::Set file timestamp to something meaningful.
::While building and testing this feature might be annoying, so we don't do it.
::==========================================================
setlocal
set TIMESTRING=0%PRODUCT_VER_STRING:~0,1%:0%PRODUCT_VER_STRING:~2,1%:0%PRODUCT_VER_STRING:~4,1%
@if /I "%FBBUILD_BUILDTYPE%"=="release" (
	(@echo Touching release build files with %TIMESTRING% timestamp) & (touch -s -D -t%TIMESTRING% %FBBUILD_OUTPUT%\*.*)
	(if %FBBUILD_EMB_PACK% EQU 1 (@echo Touching release build files with %TIMESTRING% timestamp) & (touch -s -D -t%TIMESTRING% %FBBUILD_EMB_PACK_ROOT%\*.*) )
	(if %FBBUILD_ZIP_PACK% EQU 1 (@echo Touching release build files with %TIMESTRING% timestamp) & (touch -s -D -t%TIMESTRING% %FBBUILD_ZIP_PACK_ROOT%\*.*) )
)
endlocal
::End of TOUCH_ALL
::----------------
@goto :EOF


:ISX_PACK
::=======
:: Now let's go and build the installable .exe
:: Obviously this will fail if InnoSetup
:: Extensions is not installed.
::=================================================
if "%FBBUILD_ISX_PACK%" NEQ "1" goto :EOF
@Echo Now let's compile the InnoSetup scripts
@Echo.
"%INNO_SETUP_PATH%\iscc" %FBBUILD_INSTALL_ROOT%\arch-specific\win32\FirebirdInstall_%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%.iss
@echo.

::End of ISX_PACK
::---------------
@goto :EOF


:HELP
::===
@echo.
@echo.
@echo   Parameters can be passed in any order.
@echo   Currently the recognised params are:
@echo.
@echo       DEBUG  Use binaries from 'debug' dir, not 'release' dir.
@echo              (Requires a debug build. NOTE: A debug build is
@echo               not required to create packages with debug info.)
@echo.
@echo       PDB    Include pdb files.
@echo              (These files roughly double the size of the package.)
@echo.
@echo       ISX    Create installable binary from InnoSetup Extensions compiler.
@echo              (You need to set the INNO_SETUP_PATH environment variable.)
@echo.
@echo       ZIP    Create Zip package.
@echo              (PKZIP is currently used and the PKZIP env var must be set.)
@echo.
@echo       EMB    Create Embedded package.
@echo              (PKZIP is currently used and the PKZIP env var must be set.)
@echo.
@echo       ALL    Build InnoSetup, Zip and Embedded packages.
@echo.
@echo       HELP   This help screen.
@echo.
@echo   In addition, the following environment variables are checked:
@echo.
@echo     FB2_ISS_DEBUG=1 - Prepare an InnoSetup script that is
@echo                       easier to debug
@echo.
@echo     FB2_EXAMPLES=0  - Don't include examples in the install kit.
@echo.
::End of HELP
::-----------
@goto :EOF


:ERROR
::====
@echo.
@echo   Error in BuildExecutableInstall
@echo     %*
@echo.
popd
:: Attempt to execute a phony command. This will ensure
:: that ERRORLEVEL is set on exit.
cancel_script > nul 2>&1
::End of ERROR
::------------
@goto :EOF


:WARNING
::======
@echo.
@echo   **** WARNING - Execution of a non-critical component failed.
@echo   %*
@echo.
if "%FBBUILD_PROD_STATUS%"=="PROD" (
@echo.
@echo   Production status is Final or Release Candidate
@echo   Error must be fixed before continuing
@echo.
cancel_script > nul 2>&1
) else (
@set | findstr win > nul 2>&1
)
@goto :EOF


:MAIN
::====

::Check if on-line help is required
for %%v in ( %1 %2 %3 %4 %5 %6 %7 %8 %9 )  do (
  ( @if /I "%%v"=="-h" (goto :HELP & goto :EOF) )
  ( @if /I "%%v"=="/h" (goto :HELP & goto :EOF) )
  ( @if /I "%%v"=="HELP" (goto :HELP & goto :EOF) )
)

@call setenvvar.bat
@if errorlevel 1 (goto :END)

@if not defined FB2_ISS_DEBUG (set FB2_ISS_DEBUG=0)
@if %FB2_ISS_DEBUG% equ 0 (SETLOCAL)

@if not defined FB2_EXAMPLES (set FB2_EXAMPLES=1)
@Echo.
@Echo   Reading command-line parameters...
@(@call :SET_PARAMS %* )
@if "%ERRLEV%"=="1" (goto :ERROR %errorlevel% calling SET_PARAMS )

@Echo.
@Echo   Checking that all required components are available...
@(@call :CHECK_ENVIRONMENT ) || (@echo Error calling CHECK_ENVIRONMENT & @goto :EOF)
@Echo.

@Echo   Setting version number...
@(@call :SED_MAGIC ) || (@echo Error calling SED_MAGIC & @goto :EOF)
@Echo.

@Echo   Copying additonal files needed for installation, documentation etc.
@(@call :COPY_XTRA ) || (@echo Error calling COPY_XTRA & @goto :EOF)
@Echo.

@Echo   Concatenating header files for ibase.h
@(@call :IBASE_H ) || (@echo Error calling IBASE_H & @goto :EOF)
@Echo.

@Echo   Writing alias conf
@(@call :ALIAS_CONF ) || (@echo Error calling ALIAS_CONF & @goto :EOF)
@Echo.
@Echo   Copying backup of security database
@(@call :GBAK_SEC_DB ) || (@echo Error calling GBAK_SEC_DB & @goto :EOF)
@Echo.
@Echo   Copying firebird.msg
@(@call :FB_MSG ) || (@echo Error calling FB_MSG & @goto :EOF)
@Echo.

if %FBBUILD_EMB_PACK% EQU 1 (
@Echo   Generating image of embedded install
@(@call :GEN_EMBEDDED ) || (@echo Error calling GEN_EMBEDDED & @goto :EOF)
@Echo.
)

if %FBBUILD_ZIP_PACK% EQU 1 (
@echo   Generating image of zipped install
@(@call :GEN_ZIP ) || (@echo Error calling GEN_ZIP & @goto :EOF)
@echo.
)

::@Echo Creating .local files for libraries
::@(@call :TOUCH_LOCAL ) || (@echo Error calling TOUCH_LOCAL & @goto :EOF)
::@Echo.

@(@call :TOUCH_ALL ) || (@echo Error calling TOUCH_ALL & @goto :EOF)
@echo.

if %FBBUILD_ZIP_PACK% EQU 1 (
@echo   Zipping files for zip pack
@(@call :ZIP_PACK ) || (@echo Error calling ZIP_PACK & @goto :EOF)
@echo.
)

if %FBBUILD_EMB_PACK% EQU 1 (
@echo   Zipping files for embedded pack
@(@call :EMB_PACK ) || (@echo Error calling EMB_PACK & @goto :EOF)
@echo.
)

if %FBBUILD_ISX_PACK% EQU 1 (
@(@call :ISX_PACK ) || (@echo Error calling ISX_PACK & @goto :EOF)
@echo.
)

@echo.
@echo Completed building installation kit(s)
@echo.

::@if %FB2_ISS_DEBUG% equ 0 (ENDLOCAL)
::End of MAIN
::-----------
@goto :EOF


:END



