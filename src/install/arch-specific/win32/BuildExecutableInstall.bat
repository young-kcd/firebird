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



::Check if on-line help is required
@if /I "%1"=="-h" (goto :HELP & goto :EOF)
@if /I "%1"=="/h" (goto :HELP & goto :EOF)
@if /I "%1"=="-?" (goto :HELP & goto :EOF)
@if /I "%1"=="/?" (goto :HELP & goto :EOF)
@if /I "%1"=="HELP" (goto :HELP & goto :EOF)

@call setenvvar.bat
@if errorlevel 1 (goto :END)

@goto :MAIN
@goto :EOF

:SET_PARAMS
::Assume we are preparing a production build
set BUILDTYPE=release
::Don't ship pdb files by default
set SHIP_PDB=no_pdb

:: Reset "make" vars to null
set FBBUILD_ZIP_PACK=0
set FBBUILD_ISX_PACK=0
set FBBUILD_EMB_PACK=0

:: Hard code our package number - it only needs to be changed if the
:: kit is repackaged but the engine is not rebuilt
set FBBUILD_PACKAGE_NUMBER=0_RC5

:: See what we have on the command line
for %%v in ( %1 %2 %3 %4 %5 )  do (
  ( if /I "%%v"=="DEBUG" (set BUILDTYPE=debug) )
  ( if /I "%%v"=="PDB" (set SHIP_PDB=ship_pdb) )
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
@echo s/define release/define %BUILDTYPE%/ >> %temp%.\b$3.txt
@echo s/define msvc_version 6/define msvc_version %MSVC_VERSION%/ >> %temp%.\b$3.txt
@echo s/define no_pdb/define %SHIP_PDB%/ >> %temp%.\b$3.txt
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
@copy %ROOT_PATH%\src\install\misc\firebird.conf %ROOT_PATH%\output\ > nul
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY of firebird.conf failed with errorlevel %ERRORLEVEL% ) & (goto :EOF))

@echo Copying ib_util etc
for %%v in ( ib_util.h ib_util.pas ) do (
	((copy %ROOT_PATH%\src\extlib\%%v %ROOT_PATH%\output\include\%%v > nul) || 	(@echo Copying %%v failed.))
)


@echo Copying fbclient lib etc
for %%v in (fbclient ib_util) do @(
	((copy %ROOT_PATH%\temp\%BUILDTYPE%\%%v\%%v.lib %ROOT_PATH%\output\lib\%%v_ms.lib > nul) || (@echo Copying %%v.lib failed.))
)

::Allow fbserver to access upto 3GB RAM, if it is available.
dumpbin /headers %ROOT_PATH%\output\bin\fbserver.exe | findstr /C:"Application can handle large (>2GB) addresses" || editbin /largeaddressaware %ROOT_PATH%\output\bin\fbserver.exe


::Generate .lib compatible with Borland compiler.
@implib.exe | findstr "Borland" > nul
@if errorlevel 0 (@echo generating fbclient_bor.lib && @implib %ROOT_PATH%\output\lib\fbclient_bor.lib %ROOT_PATH%\output\bin\fbclient.dll)


@if "%SHIP_PDB%"=="ship_pdb" (
(@echo Copying pdb files...)
((@copy %ROOT_PATH%\temp\%BUILDTYPE%\firebird\bin\*.pdb %ROOT_PATH%\output\bin > nul) || (@echo Copying pdb files failed))
)

@echo Started copying docs...
@copy %ROOT_PATH%\doc\*.* %ROOT_PATH%\output\doc\ > nul
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY of main documentation tree failed with errorlevel %ERRORLEVEL% ) & (goto :EOF))
@if EXIST "%ROOT_PATH%\output\doc\Firebird_v15.104_ReleaseNotes.pdf" ren %ROOT_PATH%\output\doc\Firebird_v15.104_ReleaseNotes.pdf Firebird_v15_ReleaseNotes.pdf

@echo Copying udf library scripts...

(copy %ROOT_PATH%\src\extlib\ib_udf.sql %ROOT_PATH%\output\UDF\ib_udf.sql > nul) || 	(@echo Copying %%v failed.)
(copy %ROOT_PATH%\src\extlib\fbudf\fbudf.sql %ROOT_PATH%\output\UDF\fbudf.sql > nul) || 	(@echo Copying %%v failed.)

@copy %ROOT_PATH%\src\extlib\fbudf\fbudf.txt %ROOT_PATH%\output\doc\ > nul
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY fbudf doc failed with errorlevel %ERRORLEVEL% ) & (goto :EOF))

@echo Copying other documentation...
@copy %ROOT_PATH%\ChangeLog %ROOT_PATH%\output\doc\ChangeLog.txt  > nul

::@copy %ROOT_PATH%\output\doc\install_win32.txt %ROOT_PATH%\output\doc\InstallNotes.txt > nul
::@del %ROOT_PATH%\output\doc\install_win32.txt
::@if %ERRORLEVEL% GEQ 1 ( (call :ERROR Rename install_win32.txt failed with errorlevel %ERRORLEVEL% ) & (goto :EOF))

@copy %ROOT_PATH%\src\install\arch-specific\win32\installation_readme.txt %ROOT_PATH%\output\doc\installation_readme.txt > nul

:: This stuff doesn't make much sense to Windows users, although the troubleshooting doc
:: could be made more platform agnostic.
@for %%v in (  README.makefiles README.user README.user.embedded README.user.troubleshooting README.build.mingw.html README.build.msvc.html fb2-todo.txt install_win32.txt README.install.Solaris_on_Intel *.*~) do (
  (@del %ROOT_PATH%\output\doc\%%v 2>nul)
)

@copy %ROOT_PATH%\output\doc\WhatsNew %ROOT_PATH%\output\doc\WhatsNew.txt > nul
@del %ROOT_PATH%\output\doc\WhatsNew

@mkdir %ROOT_PATH%\output\doc\sql.extensions 2>nul
@if %ERRORLEVEL% GEQ 2 ( (call :ERROR MKDIR for doc\sql.extensions dir failed ) & (goto :EOF))

@copy %ROOT_PATH%\doc\sql.extensions\*.* %ROOT_PATH%\output\doc\sql.extensions\ > nul
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY doc\sql.extensions failed  ) & (goto :EOF))

if DEFINED FB_EXTERNAL_DOCS (
	@echo copying pdf docs.
	@for %%v in (Firebird-1.5-QuickStart.pdf Firebird_v1.5.ReleaseNotes.pdf Firebird_v1.5.1.ReleaseNotes.pdf Firebird_v1.5.2.ReleaseNotes.pdf ) do @copy %FB_EXTERNAL_DOCS%\%%v %ROOT_PATH%\output\doc\%%v > nul
) else (
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
@echo Completed copying docs.

::rename the examples directory
:: /Y is not necessary in a script, but is added to document
:: that no prompting will take place
if EXIST %ROOT_PATH%\output\v5_examples (
@move /Y %ROOT_PATH%\output\v5_examples %ROOT_PATH%\output\examples > nul 2>&1
)

@echo Completed copying docs.
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
set OUTPATH=%ROOT_PATH%\output\include
for %%v in ( %ROOT_PATH%\src\include\fb_types.h %ROOT_PATH%\src\jrd\blr.h ) do (
  del %OUTPATH%\%%~nxv 2> nul
  copy %%v %OUTPATH%\%%~nxv > nul
  sed -n -f strip_comments.sed %OUTPATH%\%%~nxv > %OUTPATH%\%%~nv.more
  more /s %OUTPATH%\%%~nv.more > %OUTPATH%\%%~nv.sed
)
ren %OUTPATH%\ibase.h ibase.sed
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
@echo # > %ROOT_PATH%\output\aliases.conf
@echo # List of known database aliases >> %ROOT_PATH%\output\aliases.conf
@echo # ------------------------------ >> %ROOT_PATH%\output\aliases.conf
@echo # >> %ROOT_PATH%\output\aliases.conf
@echo # Examples: >> %ROOT_PATH%\output\aliases.conf
@echo # >> %ROOT_PATH%\output\aliases.conf
@echo #   dummy = c:\data\dummy.fdb >> %ROOT_PATH%\output\aliases.conf
@echo #  >> %ROOT_PATH%\output\aliases.conf
::End of ALIAS_CONF
::-----------------
@goto :EOF


:GBAK_SEC_DB
:: let's make sure that we have a backup of the security database handy.
::======================================================================
@echo Copying backup of security database
@copy %ROOT_PATH%\src\misc\security.gbak %ROOT_PATH%\output\security.fbk > nul
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR copy security.fbk failed ) & (goto :EOF))

:: Make sure that qli's help.fdb is available
::===============================================
@if not exist %ROOT_PATH%\output\help\help.fdb (
	(@echo Copying help.fdb for qli support)
	(@copy %ROOT_PATH%\gen\dbs\help.fdb %ROOT_PATH%\output\help\help.fdb > nul)
	(@if %ERRORLEVEL% GEQ 1 ( (call :ERROR Could not copy qli help database ) & (goto :EOF)))
)

::End of GBAK_SEC_DB
::------------------
@goto :EOF

:FB_MSG
::=================================================================
:: firebird.msg is generated as part of the build process
:: in builds\win32 by build_msg.bat copying from there to output dir
::=================================================================
@if not exist %ROOT_PATH%\output\firebird.msg (
	(@copy %ROOT_PATH%\gen\firebird.msg %ROOT_PATH%\output\firebird.msg > nul)
	(@if %ERRORLEVEL% GEQ 1 ( (call :ERROR Could not copy firebird.msg ) & (goto :EOF)))
)
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

@Echo Generating zip package
:: Generate the directory tree to be zipped
@mkdir %ROOT_PATH%\zip_pack 2>nul
@copy /Y %ROOT_PATH%\output %ROOT_PATH%\zip_pack > nul
for %%v in (bin doc doc\sql.extensions help include intl lib udf examples ) do (
	@mkdir %ROOT_PATH%\zip_pack\%%v 2>nul
	@copy /Y %ROOT_PATH%\output\%%v\*.* %ROOT_PATH%\zip_pack\%%v\ > nul
)

:: Now remove stuff that is not needed.
for %%v in (bin\gpre_boot.exe bin\gpre_static.exe doc\installation_readme.txt bin\fbembed.dll bin\fbembed.pdb ) do (
	@del %ROOT_PATH%\zip_pack\%%v
)

::grab install notes for zip pack
@copy %ROOT_PATH%\doc\install_win32.txt %ROOT_PATH%\zip_pack\doc\README_installation.txt > nul

:: Add license
for %%v in (IPLicense.txt IDPLicense.txt ) do (
	@copy %ROOT_PATH%\src\install\misc\%%v %ROOT_PATH%\zip_pack\%%v > nul
)

::And readme
@copy %ROOT_PATH%\src\install\arch-specific\win32\readme.txt %ROOT_PATH%\zip_pack\ > nul

::End of GEN_ZIP
::--------------
goto :EOF


:ZIP_PACK
::=======
if "%FBBUILD_ZIP_PACK%" NEQ "1" goto :EOF
mkdir %ROOT_PATH%\builds\win32\install_image 2>nul
if defined PKZIP (
	if "%SHIP_PDB%" == "ship_pdb" (
		del %ROOT_PATH%\builds\win32\install_image\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_win32_pdb.zip
		%PKZIP%\pkzip25.exe -level=9 -add=update -rec -path=relative %ROOT_PATH%\builds\win32\install_image\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_win32_pdb.zip %ROOT_PATH%\zip_pack\*.*
	) else (
		del %ROOT_PATH%\builds\win32\install_image\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_win32.zip
	 	%PKZIP%\pkzip25.exe -level=9 -add=update -rec -path=relative -exclude=*.pdb %ROOT_PATH%\builds\win32\install_image\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_win32.zip %ROOT_PATH%\zip_pack\*.*
	)
) else (
@Echo.
@Echo Couldn't find pkzip.
@Echo.
)

::Endo of ZIP_PACK
::----------------
@goto :EOF


:GEN_EMBEDDED
::===========
:: Generate the directory tree for the embedded zip pack
@mkdir %ROOT_PATH%\emb_pack 2>nul

for %%v in (aliases.conf firebird.conf firebird.msg) do (	@copy /Y %ROOT_PATH%\output\%%v %ROOT_PATH%\emb_pack\%%v > nul)

for %%v in ( doc intl udf ) do (@mkdir %ROOT_PATH%\emb_pack\%%v 2>nul)

@copy /Y %ROOT_PATH%\temp\release\firebird\bin\fbembed.* %ROOT_PATH%\emb_pack > nul
@copy /Y %ROOT_PATH%\temp\release\firebird\bin\ib_util.dll %ROOT_PATH%\emb_pack > nul
@copy /Y %ROOT_PATH%\output\doc\Firebird*.pdf %ROOT_PATH%\emb_pack\doc\ > nul
@copy /Y %ROOT_PATH%\output\intl\*.* %ROOT_PATH%\emb_pack\intl\ > nul
@copy /Y %ROOT_PATH%\output\udf\*.* %ROOT_PATH%\emb_pack\udf\ > nul


::grab install notes for embedded zip pack
@copy %ROOT_PATH%\doc\README.user.embedded %ROOT_PATH%\emb_pack\doc\README_embedded.txt > nul
@copy %ROOT_PATH%\doc\WhatsNew %ROOT_PATH%\emb_pack\doc\WhatsNew.txt > nul

:: Add license
for %%v in (IPLicense.txt IDPLicense.txt ) do (
	@copy %ROOT_PATH%\src\install\misc\%%v %ROOT_PATH%\emb_pack\%%v > nul
)

::And readme
@copy %ROOT_PATH%\src\install\arch-specific\win32\readme.txt %ROOT_PATH%\emb_pack\ > nul


::End of GEN_EMBEDDED
::-------------------
@goto :EOF


:EMB_PACK
::=======
if "%FBBUILD_EMB_PACK%" NEQ "1" goto :EOF
@echo Now building embedded package
mkdir %ROOT_PATH%\builds\win32\install_image 2>nul
::Now we can zip it up and copy the package to the install images directory.
if defined PKZIP (
	if "%SHIP_PDB%" == "ship_pdb" (
		del %ROOT_PATH%\builds\win32\install_image\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_embed_win32_pdb.zip
		%PKZIP%\pkzip25.exe -level=9 -add=update -rec -path=relative %ROOT_PATH%\builds\win32\install_image\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_embed_win32_pdb.zip %ROOT_PATH%\emb_pack\*.*
	) else (
	 	del %ROOT_PATH%\builds\win32\install_image\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_embed_win32.zip
	 	%PKZIP%\pkzip25.exe -level=9 -add=update -rec -path=relative -exclude=*.pdb %ROOT_PATH%\builds\win32\install_image\Firebird-%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%_embed_win32.zip %ROOT_PATH%\emb_pack\*.*
	)
) else (
@Echo.
@Echo Couldn't find pkzip.
@Echo.
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
	(@echo Touching release build files with 01:05:20 timestamp) & (touch -s -D -t01:05:20 %ROOT_PATH%\output\*.*)
	(@echo Touching release build files with 01:05:20 timestamp) & (touch -s -D -t01:05:20 %ROOT_PATH%\emb_pack\*.*)
	(@echo Touching release build files with 01:05:20 timestamp) & (touch -s -D -t01:05:20 %ROOT_PATH%\zip_pack\*.*)
)
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
if NOT DEFINED INNO_SETUP_PATH (@echo INNO_SETUP_PATH variable not defined & goto :EOF)
@Echo Now let's compile the InnoSetup scripts
@Echo.
"%INNO_SETUP_PATH%\iscc" %ROOT_PATH%\src\install\arch-specific\win32\FirebirdInstall_%PRODUCT_VER_STRING%-%FBBUILD_PACKAGE_NUMBER%.iss
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
@echo   Error  - %*
@echo.
cancel_script > nul 2>&1
::End of ERROR
::------------
@goto :EOF


:MAIN
::====
@if not defined FB2_ISS_DEBUG (set FB2_ISS_DEBUG=0)
@if %FB2_ISS_DEBUG% equ 0 (SETLOCAL)

@if not defined FB2_EXAMPLES (set FB2_EXAMPLES=1)
@Echo.
@Echo Reading command-line parameters...
@(@call :SET_PARAMS %1 %2 %3 %4 %5)|| (@echo Error calling SET_PARAMS & @goto :EOF)
@Echo.
@Echo Setting version number
@(@call :SED_MAGIC ) || (@echo Error calling SED_MAGIC & @goto :EOF)
@Echo.
@Echo Copying additonal files needed for installation, documentation etc.
@(@call :COPY_XTRA ) || (@echo Error calling COPY_XTRA & @goto :EOF)
@Echo.
@Echo Concatenating header files for ibase.h
@(@call :IBASE_H ) || (@echo Error calling IBASE_H & @goto :EOF)
@Echo.
@Echo alias conf
@(@call :ALIAS_CONF ) || (@echo Error calling ALIAS_CONF & @goto :EOF)
@Echo.
@Echo gbak_sec_db
@(@call :GBAK_SEC_DB ) || (@echo Error calling GBAK_SEC_DB & @goto :EOF)
@Echo.
@Echo fb_msg
@(@call :FB_MSG ) || (@echo Error calling FB_MSG & @goto :EOF)
@Echo.
if %FBBUILD_EMB_PACK% EQU 1 (
@Echo gen_embedded
@(@call :GEN_EMBEDDED ) || (@echo Error calling GEN_EMBEDDED & @goto :EOF)
@Echo.
)
if %FBBUILD_ZIP_PACK% EQU 1 (
@Echo gen_zip
@(@call :GEN_ZIP ) || (@echo Error calling GEN_ZIP & @goto :EOF)
@echo.
)
::@Echo Creating .local files for libraries
::@(@call :TOUCH_LOCAL ) || (@echo Error calling TOUCH_LOCAL & @goto :EOF)
::@Echo.
@(@call :TOUCH_ALL ) || (@echo Error calling TOUCH_ALL & @goto :EOF)
@Echo.
if %FBBUILD_ZIP_PACK% EQU 1 (
@(@call :ZIP_PACK ) || (@echo Error calling ZIP_PACK & @goto :EOF)
@Echo.
)
if %FBBUILD_EMB_PACK% EQU 1 (
@(@call :EMB_PACK ) || (@echo Error calling EMB_PACK & @goto :EOF)
@Echo.
)
if %FBBUILD_ISX_PACK% EQU 1 (
@(@call :ISX_PACK ) || (@echo Error calling ISX_PACK & @goto :EOF)
@goto :EOF
)

@echo.
@echo Completed building installation kit(s)
@echo.

@if %FB2_ISS_DEBUG% equ 0 (ENDLOCAL)
::End of MAIN
::-----------
@goto :EOF

:END

