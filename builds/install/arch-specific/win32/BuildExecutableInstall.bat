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

@goto :MAIN
@goto :EOF
::============================================================================

:SET_PARAMS
@echo off
:: Assume we are preparing a production build
set FBBUILD_BUILDTYPE=release
:: Don't ship pdb files by default
set FBBUILD_SHIP_PDB=no_pdb
:: Reset "make" vars to null
set FBBUILD_ZIP_PACK=0
set FBBUILD_ISX_PACK=0
set FBBUILD_EMB_PACK=0

:: Hard code our package number - it only needs to be changed if the
:: kit is repackaged but the engine is not rebuilt
set FBBUILD_PACKAGE_NUMBER=0

:: See what we have on the command line
::for %%v in ( %1 %2 %3 %4 %5 )  do (
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

:: Location of most of the build that we are creating the
:: installation kits from. This subdir is created by make_all.bat
set FBBUILD_OUTPUT=%ROOT_PATH%\output

:: Determine Product Status
set FBBUILD_PROD_STATUS=
@type %ROOT_PATH%\src\jrd\build_no.h | findstr /I UNSTABLE  > nul && (
set FBBUILD_PROD_STATUS=DEV) || type %ROOT_PATH%\src\jrd\build_no.h | findstr /I ALPHA > nul && (
set FBBUILD_PROD_STATUS=DEV) || type %ROOT_PATH%\src\jrd\build_no.h | findstr /I BETA > nul && (
set FBBUILD_PROD_STATUS=PROD) || type %ROOT_PATH%\src\jrd\build_no.h | findstr /I "Release Candidate" > nul && (
set FBBUILD_PROD_STATUS=PROD) || type %ROOT_PATH%\src\jrd\build_no.h | findstr /I "RC" > nul && (
set FBBUILD_PROD_STATUS=PROD) || type %ROOT_PATH%\src\jrd\build_no.h | findstr /I "Final" > nul && (
set FBBUILD_PROD_STATUS=PROD)


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
  if not defined 7ZIP (
    call :ERROR 7ZIP environment variable is not defined.
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

@echo s/2.0.0/%PRODUCT_VER_STRING%/ > %temp%.\b$3.txt
@echo s/define release/define %FBBUILD_BUILDTYPE%/ >> %temp%.\b$3.txt
@echo s/define msvc_version 6/define msvc_version %MSVC_VERSION%/ >> %temp%.\b$3.txt
@echo s/define no_pdb/define %FBBUILD_SHIP_PDB%/ >> %temp%.\b$3.txt
@echo s/define package_number=\"0\"/define package_number=\"%FBBUILD_PACKAGE_NUMBER%\"/ >> %temp%.\b$3.txt
@echo s/define iss_release/define %ISS_BUILD_TYPE%/ >> %temp%.\b$3.txt
@echo s/define examples/define %ISS_EXAMPLES%/ >> %temp%.\b$3.txt
@echo s/define compression/define %ISS_COMPRESS%/ >> %temp%.\b$3.txt
@echo s/PRODUCT_VER_STRING/%PRODUCT_VER_STRING%/ >> %temp%.\b$3.txt

sed -f  %temp%.\b$3.txt FirebirdInstall_20.iss > FirebirdInstall_%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%.iss
del %temp%.\b$?.txt

::End of SED_MAGIC
::----------------
@goto :EOF


:COPY_XTRA
:: system dll's we need
:: (You may need to download and extract the
:: vcredist stuff to your MSDevDir for this to work.
::=====================
if not exist %FBBUILD_OUTPUT%\system32 (mkdir %FBBUILD_OUTPUT%\system32)
@if DEFINED MSDevDir (
  @if %MSVC_VERSION% EQU 6 (
    @copy "%MSDevDir%\vcredist\msvcrt.dll" %FBBUILD_OUTPUT%\system32\ > nul
    @copy "%MSDevDir%\vcredist\msvcp%MSVC_VERSION%0.dll" %FBBUILD_OUTPUT%\system32\ > nul
  )
) else (
  @if DEFINED MSVCDir (
	@if %MSVC_VERSION% EQU 7 (
      @copy "%FrameworkSDKDir%\bin\msvcp%MSVC_VERSION%?.dll" %FBBUILD_OUTPUT%\system32\ > nul
      @copy "%FrameworkSDKDir%\bin\msvcr%MSVC_VERSION%?.dll" %FBBUILD_OUTPUT%\system32\ > nul
	)
  )
)
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR Copying MSVC runtime library failed with error %ERRORLEVEL% ) & (goto :EOF))

:: grab some missing bits'n'pieces from different parts of the source tree
::=========================================================================
@echo   Copying firebird.conf
@copy %ROOT_PATH%\builds\install\misc\firebird.conf %FBBUILD_OUTPUT%\ > nul
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY of firebird.conf failed with errorlevel %ERRORLEVEL% ) & (goto :EOF))

@echo   Copying ib_util etc
for %%v in ( ib_util.h ib_util.pas ) do (
	((copy %ROOT_PATH%\src\extlib\%%v %FBBUILD_OUTPUT%\include\%%v > nul) || (call :WARNING Copying %%v failed.))
)

@echo   Copying fbclient lib etc
:: For some unknown reason copy sets the errorlevel even when it succeeds,
:: so the || branch is only executed if copy throws a real error, even though the errorlevel is set.
:: Just another of those fabulous msdos idiosyncracies.
for %%v in (fbclient ib_util) do (
@copy /Y %ROOT_PATH%\temp\%FBBUILD_BUILDTYPE%\%%v\%%v.lib %FBBUILD_OUTPUT%\lib\%%v_ms.lib || @if %ERRORLEVEL% GEQ 1 (call :ERROR Copying %%v to %%v_ms.lib failed with error %ERRORLEVEL%) & (goto :EOF))
)

@implib.exe | findstr "Borland" > nul
@if errorlevel 0 (
  @echo   Generating fbclient_bor.lib
  @implib %FBBUILD_OUTPUT%\lib\fbclient_bor.lib %FBBUILD_OUTPUT%\bin\fbclient.dll > nul
)

@if "%FBBUILD_SHIP_PDB%"=="ship_pdb" (
  @echo   Copying pdb files...
  @copy %ROOT_PATH%\temp\%FBBUILD_BUILDTYPE%\firebird\bin\*.pdb %FBBUILD_OUTPUT%\bin > nul
  @if %ERRORLEVEL% GEQ 1 (
    call :ERROR Copying pdb files failed
	goto :EOF
  )
)

@echo   Started copying docs...
@rmdir /S /Q %ROOT_PATH%\output\doc 2>nul
@mkdir %ROOT_PATH%\output\doc
@copy %ROOT_PATH%\ChangeLog %ROOT_PATH%\output\doc\ChangeLog.txt >nul
@copy %ROOT_PATH%\doc\*.* %FBBUILD_OUTPUT%\doc\ > nul
@if %ERRORLEVEL% GEQ 1 (
  call :ERROR COPY of main documentation tree failed with error %ERRORLEVEL%
  goto :EOF
)

@echo   Copying udf library scripts...
for %%v in ( ib_udf.sql ) do (
  copy %ROOT_PATH%\src\extlib\%%v  %FBBUILD_OUTPUT%\UDF\%%v > nul
  @if %ERRORLEVEL% GEQ 1 (
    call :ERROR Copying %ROOT_PATH%\src\extlib\%%v failed.
    goto :EOF
  )
)

for %%v in ( fbudf.sql fbudf.txt ) do (
  copy %ROOT_PATH%\src\extlib\fbudf\%%v  %FBBUILD_OUTPUT%\UDF\%%v > nul
  @if %ERRORLEVEL% GEQ 1 (
    call :ERROR Copying %ROOT_PATH%\src\extlib\%%v failed with error %ERRORLEVEL%
    goto :EOF
  )
)

@echo   Copying other documentation...
@copy %ROOT_PATH%\builds\install\arch-specific\win32\installation_readme.txt %FBBUILD_OUTPUT%\doc\installation_readme.txt > nul
@copy %FBBUILD_OUTPUT%\doc\WhatsNew %FBBUILD_OUTPUT%\doc\WhatsNew.txt > nul
@del %FBBUILD_OUTPUT%\doc\WhatsNew


:: If we are not doing a final release then include stuff that is
:: likely to be of use to testers, especially as our release notes
:: may be incomplete or non-existent
@if /I "%FBBUILD_PROD_STATUS%"=="DEV" (
  @copy %ROOT_PATH%\ChangeLog %FBBUILD_OUTPUT%\doc\ChangeLog.txt  > nul
)


@mkdir %FBBUILD_OUTPUT%\doc\sql.extensions 2>nul
@if %ERRORLEVEL% GEQ 2 ( (call :ERROR MKDIR for doc\sql.extensions dir failed) & (@goto :EOF))
@copy %ROOT_PATH%\doc\sql.extensions\*.* %FBBUILD_OUTPUT%\doc\sql.extensions\ > nul
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR Copying doc\sql.extensions failed  ) & (goto :EOF))

@echo   Copying pdf docs...
@for %%v in ( Firebird-1.5-QuickStart.pdf Firebird_v1.5.ReleaseNotes.pdf Firebird_v1.5.1.ReleaseNotes.pdf Firebird_v1.5.2.ReleaseNotes.pdf Firebird_v2.0.ReleaseNotes.pdf ) do (
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
	@copy %ROOT_PATH%\builds\install\misc\%%v %FBBUILD_OUTPUT%\%%v > nul
)

:: And readme
@copy %ROOT_PATH%\builds\install\arch-specific\win32\readme.txt %FBBUILD_OUTPUT%\ > nul

::  Walk through all docs and transform any that are not .txt, .pdf or .html to .txt
echo   Setting .txt filetype to ascii docs.
for /R %FBBUILD_OUTPUT%\doc %%v in (.) do (
  pushd %%v
  for /F %%W in ( 'dir /B /A-D' ) do (
	if /I "%%~xW" NEQ ".txt" (
  	  if /I "%%~xW" NEQ ".pdf" (
        if /I "%%~xW" NEQ ".htm" (
          if /I "%%~xW" NEQ ".html" (
            ren %%W %%W.txt
		  )
		)
	  )
	)
  )
  popd
)

:: Throw away any errorlevel left hanging around
@set | findstr win > nul

@echo   Completed copying docs.
::End of COPY_XTRA
::----------------
@goto :EOF


:IBASE_H
:: Concatenate header files into ibase.h
::======================================
:: o This section of code takes several header files, strips license
::   boiler plates and comments and inserts them into ibase.h for
::   distribution. The only drawback is that it strips all the comments.
:: o No error checking is done.
:: o Take note that different versions of sed use different
::   string delimiters. The firebird_tools version uses double quotes - ".
::   The cygwin one probably uses single quotes.
:: o The script 'strip_comments.sed' is taken from
::      http://sed.sourceforge.net/grabbag/scripts/testo.htm

setlocal
set OUTPATH=%FBBUILD_OUTPUT%\include
copy %ROOT_PATH%\src\jrd\ibase.h %OUTPATH%\ibase.h > nul
for %%v in ( %ROOT_PATH%\src\include\fb_types.h %ROOT_PATH%\src\dsql\sqlda_pub.h %ROOT_PATH%\src\jrd\dsc_pub.h %ROOT_PATH%\src\jrd\inf_pub.h %ROOT_PATH%\src\jrd\blr.h ) do (
  del %OUTPATH%\%%~nxv 2> nul
  copy %%v %OUTPATH%\%%~nxv > nul
  sed -n -f strip_comments.sed %OUTPATH%\%%~nxv > %OUTPATH%\%%~nv.more
  more /s %OUTPATH%\%%~nv.more > %OUTPATH%\%%~nv.sed
)
move /y %OUTPATH%\ibase.h %OUTPATH%\ibase.sed
sed -e "/#include \"fb_types\.h\"/r %OUTPATH%\fb_types.sed" -e "/#include \"fb_types\.h\"/d" -e "/#include \"..\/jrd\/dsc_pub\.h\"/r %OUTPATH%\dsc_pub.sed" -e "/#include \"..\/jrd\/dsc_pub\.h\"/d" -e "/#include \"..\/dsql\/sqlda_pub\.h\"/r %OUTPATH%\sqlda_pub.sed" -e "/#include \"..\/dsql\/sqlda_pub\.h\"/d" -e "/#include \"blr\.h\"/r %OUTPATH%\blr.sed" -e "/#include \"blr\.h\"/d" -e "/#include \"..\/jrd\/inf_pub\.h\"/r %OUTPATH%\inf_pub.sed" -e "/#include \"..\/jrd\/inf_pub\.h\"/d" %OUTPATH%\ibase.sed > %OUTPATH%\ibase.h
del %OUTPATH%\ibase.sed %OUTPATH%\fb_types.* %OUTPATH%\sqlda_pub.* %OUTPATH%\dsc_pub.* %OUTPATH%\inf_pub.* %OUTPATH%\blr.*
endlocal

::End of IBASE_H
::--------------
@goto :EOF


:ALIAS_CONF
:: Generate a sample aliases file
::===============================
@echo   Creating sample aliases.conf
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
@copy %ROOT_PATH%\builds\misc\security.gbak %FBBUILD_OUTPUT%\security.fbk > nul
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR copy security.fbk failed ) & (goto :EOF))

@copy %ROOT_PATH%\src\misc\upgrade\v2\security_database.sql %FBBUILD_OUTPUT%\bin\security_database.sql

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


::TOUCH_LOCAL
::==========
:: Note 1: This doesn't seem to make any difference to whether local libraries are used.
:: Note 2: MS documentation was incorrectly interpreted. .local files should not be created
::         for libraries, they should be created for executables.
:: Create libname.local files for each locally installed library
::for %%v in ( fbclient msvcrt msvcp%VS_VER%0 )  do touch %FBBUILD_OUTPUT%\bin\%%v.local
::@goto :EOF


:GEN_ZIP
::======
if "%FBBUILD_ZIP_PACK%"=="" goto :EOF
@Echo   Generating zip package

:: Generate the directory tree to be zipped
set FBBUILD_ZIP_PACK=%ROOT_PATH%\zip_pack
if not exist (@mkdir %FBBUILD_ZIP_PACK% 2>nul ) else ( del /s /q %FBBUILD_ZIP_PACK%)
@copy /Y %FBBUILD_OUTPUT% %FBBUILD_ZIP_PACK% > nul
for %%v in (bin doc doc\sql.extensions help include intl lib udf examples ) do (
	@mkdir %FBBUILD_ZIP_PACK%\%%v 2>nul
	@copy /Y %FBBUILD_OUTPUT%\%%v\*.* %FBBUILD_ZIP_PACK%\%%v\ > nul
)


:: Now remove stuff that is not needed.
::for %%v in (bin\gpre_boot.exe bin\gpre_static.exe doc\installation_readme.txt bin\fbembed.dll bin\fbembed.pdb ) do (
for %%v in ( doc\installation_readme.txt bin\fbembed.dll bin\fbembed.pdb ) do (
  @del %FBBUILD_ZIP_PACK%\%%v > nul
)


:: grab install notes for zip pack
@copy %ROOT_PATH%\doc\install_win32.txt %FBBUILD_ZIP_PACK%\doc\README_installation.txt > nul

::End of GEN_ZIP
::--------------
goto :EOF


:ZIP_PACK
::=======
if "%FBBUILD_ZIP_PACK%"=="" goto :EOF
if "%FBBUILD_SHIP_PDB%" == "ship_pdb" (
	del %FBBUILD_INSTALL_IMAGES%\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_win32_pdb.zip
	%PKZIP%\pkzip25.exe -level=9 -add=update -rec -path=relative %FBBUILD_INSTALL_IMAGES%\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_win32_pdb.zip %FBBUILD_ZIP_PACK%\*.*
) else (
	del %FBBUILD_INSTALL_IMAGES%\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_win32.zip
 	%PKZIP%\pkzip25.exe -level=9 -add=update -rec -path=relative -exclude=*.pdb %FBBUILD_INSTALL_IMAGES%\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_win32.zip %FBBUILD_ZIP_PACK%\*.*
)

::Endo of ZIP_PACK
::----------------
@goto :EOF


:GEN_EMBEDDED
::===========
:: Generate the directory tree for the embedded zip pack
if %FBBUILD_EMB_PACK% NEQ 1 goto :EOF
set EMB_PATH=%ROOT_PATH%\emb_pack
@mkdir %EMB_PATH% 2>nul

for %%v in (aliases.conf firebird.conf firebird.msg) do (	@copy /Y %FBBUILD_OUTPUT%\%%v %EMB_PATH%\%%v > nul)

for %%v in ( doc intl udf ) do (@mkdir %EMB_PATH%\%%v 2>nul)

@copy /Y %ROOT_PATH%\temp\release\firebird\bin\fbembed.* %EMB_PATH% > nul
@copy /Y %ROOT_PATH%\temp\release\firebird\bin\ib_util.dll %EMB_PATH% > nul
@copy /Y %FBBUILD_OUTPUT%\doc\Firebird*.pdf %EMB_PATH%\doc\ > nul
@copy /Y %FBBUILD_OUTPUT%\intl\*.* %EMB_PATH%\intl\ > nul
@copy /Y %FBBUILD_OUTPUT%\udf\*.* %EMB_PATH%\udf\ > nul


:: grab install notes for embedded zip pack
@copy %ROOT_PATH%\doc\README.user.embedded %EMB_PATH%\doc\README_embedded.txt > nul
@copy %ROOT_PATH%\doc\WhatsNew %EMB_PATH%\doc\WhatsNew.txt > nul

:: Add license
for %%v in (IPLicense.txt IDPLicense.txt ) do (
	@copy %ROOT_PATH%\builds\install\misc\%%v %EMB_PATH%\%%v > nul
)

:: And readme
@copy %ROOT_PATH%\builds\install\arch-specific\win32\readme.txt %EMB_PATH%\ > nul


::End of GEN_EMBEDDED
::-------------------
@goto :EOF


:EMB_PACK
::=======
if %FBBUILD_EMB_PACK% NEQ 1 goto :EOF
@echo Now building embedded package
:: Now we can zip it up and copy the package to the install images directory.
if "%FBBUILD_SHIP_PDB%" == "ship_pdb" (
  del %FBBUILD_INSTALL_IMAGES%\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_embed_win32_pdb.zip
  %PKZIP%\pkzip25.exe -level=9 -add=update -rec -path=relative %FBBUILD_INSTALL_IMAGES%\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_embed_win32_pdb.zip %ROOT_PATH%\emb_pack\*.*
) else (
  del %FBBUILD_INSTALL_IMAGES%\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_embed_win32.zip
  %PKZIP%\pkzip25.exe -level=9 -add=update -rec -path=relative -exclude=*.pdb %FBBUILD_INSTALL_IMAGES%\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_embed_win32.zip %ROOT_PATH%\emb_pack\*.*
)


::End of EMB_PACK
::---------------
goto :EOF


:TOUCH_ALL
::========
::Set file timestamp to something meaningful.
::While building and testing this feature might be annoying, so we don't do it.
::==========================================================
@if /I "%BUILDTYPE%"=="release" (
	(@echo Touching release build files with 01:05:10 timestamp) & (touch -s -D -t01:05:10 %FBBUILD_OUTPUT%\*.*)
	(if %FBBUILD_EMB_PACK% EQU 1 (@echo Touching release build files with 01:05:10 timestamp) & (touch -s -D -t01:05:10 %ROOT_PATH%\emb_pack\*.*) )
	(if %FBBUILD_ZIP_PACK% EQU 1 (@echo Touching release build files with 01:05:10 timestamp) & (touch -s -D -t01:05:10 %ROOT_PATH%\zip_pack\*.*) )
)

::End of TOUCH_ALL
::----------------
@goto :EOF


:ISX_PACK
::=======
:: Now let's go and build the installable .exe
::=================================================
if %FBBUILD_ISX_PACK% NEQ 1 goto :EOF
@Echo Now let's compile the InnoSetup scripts
@Echo.
"%INNO_SETUP_PATH%"\iscc %ROOT_PATH%\builds\install\arch-specific\win32\FirebirdInstall_%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%.iss
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

pushd ..\..\..\win32
::This must be called from the directory it resides in.
@call setenvvar.bat
popd
@if errorlevel 1 (goto :END)

@if not defined FB2_ISS_DEBUG (set FB2_ISS_DEBUG=0)
::@if %FB2_ISS_DEBUG% equ 0 (SETLOCAL)

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
@Echo gen_embedded
@(@call :GEN_EMBEDDED ) || (@echo Error calling GEN_EMBEDDED & @goto :EOF)
@Echo.
)

if %FBBUILD_ZIP_PACK% EQU 1 (
@echo gen_zip
@(@call :GEN_ZIP ) || (@echo Error calling GEN_ZIP & @goto :EOF)
@echo.
)

::@Echo Creating .local files for libraries
::@(@call :TOUCH_LOCAL ) || (@echo Error calling TOUCH_LOCAL & @goto :EOF)
::@Echo.

@(@call :TOUCH_ALL ) || (@echo Error calling TOUCH_ALL & @goto :EOF)
@echo.

if %FBBUILD_ZIP_PACK% EQU 1 (
@(@call :ZIP_PACK ) || (@echo Error calling ZIP_PACK & @goto :EOF)
@echo.
)

if %FBBUILD_EMB_PACK% EQU 1 (
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
popd

::@if %FB2_ISS_DEBUG% equ 0 (ENDLOCAL)
::End of MAIN
::-----------
@goto :EOF


:END



