::  Initial Developer's Public License.
::  The contents of this file are subject to the  Initial Developer's Public
::  License Version 1.0 (the "License"). You may not use this file except
::  in compliance with the License. You may obtain a copy of the License at
::    http://www.ibphoenix.com/idpl.html
::  Software distributed under the License is distributed on an "AS IS" basis,
::  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
::  for the specific language governing rights and limitations under the
::  License.
::
::  The Original Code is copyright 2003 Paul Reeves.
::
::  The Initial Developer of the Original Code is Paul Reeves
::
::  All Rights Reserved.
::
::=============================================================================
@echo off

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
@set BUILDTYPE=release

:: See what we have on the command line
for %%v in ( %1 %2 )  do (
  ( if /I "%%v"=="DEBUG" (set BUILDTYPE=debug) )
)
@goto :EOF


:SED_MAGIC
:: Do some sed magic to make sure that the final product 
:: includes the version string in the filename.
:: If the Firebird Unix tools for Win32 aren't on 
:: the path this will fail! Use of the cygwin tools has not
:: been tested and may produce unexpected results. 
::========================================================
sed /"#define PRODUCT_VER_STRING"/!d %ROOT_PATH%\src\jrd\build_no.h > %temp%.\b$1.bat
sed -n -e s/\"//g -e s/"#define PRODUCT_VER_STRING "//w%temp%.\b$2.bat %temp%.\b$1.bat
for /f "tokens=*" %%a in ('type %temp%.\b$2.bat') do set PRODUCT_VER_STRING=%%a
@echo s/1.5.0/%PRODUCT_VER_STRING%/ > %temp%.\b$3.bat
@echo s/define super_server_install/define %PACKAGE_TYPE%/ >> %temp%.\b$3.bat
@echo s/define release/define %BUILDTYPE%/ >> %temp%.\b$3.bat
@echo s/define msvc_version 6/define msvc_version %MSVC_VERSION%/ >> %temp%.\b$3.bat
@echo s/PRODUCT_VER_STRING/%PRODUCT_VER_STRING%/ >> %temp%.\b$3.bat

sed -f  %temp%.\b$3.bat FirebirdInstall_15.iss > FirebirdInstall_%PRODUCT_VER_STRING%.iss

del %temp%.\b$?.bat
@goto :EOF


:COPY_XTRA
:: system dll's we need
:: (You may need to download and extract the 
:: vcredist stuff to your MSDevDir for this to work.
::=====================
@if %msvc_version% EQU 6 (
 (@copy %MSDevDir%\vcredist\msvcrt.dll %ROOT_PATH%\output\bin\ > nul)
 (@copy %MSDevDir%\vcredist\msvcp%msvc_version%0.dll %ROOT_PATH%\output\bin\ > nul)
)

:: grab some missing bits'n'pieces from different parts of the source tree
::=========================================================================
@echo Copying firebird.conf
@copy %ROOT_PATH%\src\install\misc\firebird.conf %ROOT_PATH%\output\ > nul
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY of firebird.conf failed with errorlevel %ERRORLEVEL% ) & (goto :EOF))

:: We don't neeed to do this now, as examples have already been copied when make_examples.bat was run.
::@mkdir %ROOT_PATH%\output\examples 2>nul 
::@if %ERRORLEVEL% GEQ 2 ( (call :ERROR MKDIR for examples dir failed with errorlevel %ERRORLEVEL% ) & (goto :EOF))

::@echo Copying examples
::copy %ROOT_PATH%\src\v5_examples\*.* %ROOT_PATH%\output\examples\ > nul
::@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY examples failed with errorlevel %ERRORLEVEL% ) & (goto :EOF))

@echo Copying ib_util etc
for %%v in ( ib_util.h ib_util.pas ) do (
	((copy %ROOT_PATH%\src\extlib\%%v %ROOT_PATH%\output\include\%%v > nul) || 	(@echo Copying %%v failed.)) 
)
	
@echo Copying fbclient lib etc
for %%v in (fbclient gds32 ib_util) do @(
	((copy %ROOT_PATH%\temp\%BUILDTYPE%\%%v\%%v.lib %ROOT_PATH%\output\lib\%%v_ms.lib > nul) || (@echo Copying %%v.lib failed.))
)

@echo Copying docs...
@copy %ROOT_PATH%\doc\*.* %ROOT_PATH%\output\doc\ > nul
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY of main documentation tree failed with errorlevel %ERRORLEVEL% ) & (goto :EOF))
@if EXIST "%ROOT_PATH%\output\doc\Firebird_v15.104_ReleaseNotes.pdf" ren %ROOT_PATH%\output\doc\Firebird_v15.104_ReleaseNotes.pdf Firebird_v15_ReleaseNotes.pdf 

@echo Copying udf library scripts...

(copy %ROOT_PATH%\src\extlib\ib_udf.sql %ROOT_PATH%\output\UDF\ib_udf.sql > nul) || 	(@echo Copying %%v failed.) 
(copy %ROOT_PATH%\src\extlib\fbudf\fbudf.sql %ROOT_PATH%\output\UDF\fbudf.sql > nul) || 	(@echo Copying %%v failed.) 

@copy %ROOT_PATH%\src\extlib\fbudf\fbudf.txt %ROOT_PATH%\output\doc\ > nul
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY fbudf doc failed with errorlevel %ERRORLEVEL% ) & (goto :EOF))

@copy %ROOT_PATH%\ChangeLog %ROOT_PATH%\output\doc\ChangeLog.txt  > nul

@copy %ROOT_PATH%\output\doc\install_win32.txt %ROOT_PATH%\output\doc\InstallNotes.txt > nul
@del %ROOT_PATH%\output\doc\install_win32.txt 
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR Rename install_win32.txt failed with errorlevel %ERRORLEVEL% ) & (goto :EOF))

@copy %ROOT_PATH%\src\install\arch-specific\win32\installation_readme.txt %ROOT_PATH%\output\doc\installation_readme.txt > nul

:: This stuff doesn't make much sense to Windows users, although the troubleshooting doc 
:: could be made more platform agnostic.
@for %%v in (  README.makefiles README.user README.user.embedded README.user.troubleshooting README.build.*.html fb2-todo.txt ) do (
  (@del %ROOT_PATH%\output\doc\%%v )
)

@copy %ROOT_PATH%\output\doc\WhatsNew %ROOT_PATH%\output\doc\WhatsNew.txt > nul
@del %ROOT_PATH%\output\doc\WhatsNew

@mkdir %ROOT_PATH%\output\doc\sql.extensions 2>nul
@if %ERRORLEVEL% GEQ 2 ( (call :ERROR MKDIR for doc\sql.extensions dir failed ) & (goto :EOF))

@copy %ROOT_PATH%\doc\sql.extensions\*.* %ROOT_PATH%\output\doc\sql.extensions\ > nul
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY doc\sql.extensions failed  ) & (goto :EOF))

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
:: Create libname.local files for each locally installed library
for %%v in ( gds32 fbclient msvcrt msvcp%MSVC_VERSION%0 )  do touch %ROOT_PATH%\output\bin\%%v.local
@goto :EOF

:TOUCH_ALL
::========
::Set file timestamp to something meaningful.
::While building and testing this feature might be annoying, so we don't do it.
::==========================================================
@if /I "%BUILDTYPE%"=="release" (
	(@echo Touching release build files with 01:05:00 timestamp) & (touch -s -D -t01:05:00 %ROOT_PATH%\output\*.*)) 
@goto :EOF


:RUN_ISX
::=======
:: Now let's go and build the installable .exe
:: Obviously this will fail if InnoSetup 
:: Extensions is not installed.
::=================================================

start FirebirdInstall_%PRODUCT_VER_STRING%.iss
@goto :EOF


:HELP
::===
@echo.
@echo   If you are testing a debug build you can
@echo   pass DEBUG as a parameter to the script.
@echo.
@echo.
@goto :EOF


:ERROR
::====
@echo.
@echo Error  - %*
@echo.
@goto :EOF


:MAIN
::====

@Echo.
@Echo Reading command-line parameters
@(@call :SET_PARAMS %1 %2 )|| (@goto :EOF) 
@Echo.
@Echo Setting version number
@(@call :SED_MAGIC ) || (@goto :EOF)
@Echo.
@Echo Copying additonal files needed for installation, documentation etc.
@(@call :COPY_XTRA ) || (@goto :EOF)
@Echo.
@Echo Concatenating header files for ibase.h
@(@call :IBASE_H ) || (@goto :EOF)
@Echo.
@Echo alias conf
@(@call :ALIAS_CONF ) || (@goto :EOF)
@Echo.
@Echo gbak_sec_db
@(@call :GBAK_SEC_DB ) || (@goto :EOF)
@Echo.
@Echo fb_msg
@(@call :FB_MSG ) || (@goto :EOF)
@Echo.
@Echo Creating .local files for libraries
@(@call :TOUCH_LOCAL ) || (@goto :EOF)
@Echo.
@(@call :TOUCH_ALL ) || (@goto :EOF)
@Echo Now let's run InnoSetup extensions
@Echo.
@(@call :RUN_ISX ) || (@goto :EOF)
@goto :EOF

:END
