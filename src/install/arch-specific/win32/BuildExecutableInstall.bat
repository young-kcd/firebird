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

::Check if on-line help is required
@if /I "%1"=="-h" (goto :HELP & goto :EOF)
@if /I "%1"=="/h" (goto :HELP & goto :EOF)
@if /I "%1"=="-?" (goto :HELP & goto :EOF)
@if /I "%1"=="/?" (goto :HELP & goto :EOF)

@goto :MAIN
@goto :EOF


:SET_PARAMS
::Assume we are preparing a production build
@set BUILDTYPE=release

::Assume we are creating an SS package unless specified on the command-line
@set PACKAGE_TYPE=super_server_install
@set PACKAGE_DESC=SuperServer

:: See what we have on the command line
for %%v in ( %1 %2 )  do (
  ( if /I "%%v"=="DEBUG" (set BUILDTYPE=debug) )
  ( if /I "%%v"=="CS"  ( (set PACKAGE_TYPE=classic_server_install) & (set PACKAGE_DESC=Classic) ) )
)

@if not defined MSVC_VERSION (set MSVC_VERSION=6)
@goto :EOF


:SED_MAGIC
:: Do some sed magic to make sure that the final product 
:: includes the version string in the filename.
:: If the Firebird Unix tools for Win32 aren't on 
:: the path this will fail! Use of the cygwin tools has not
:: been tested and may produce unexpected results. 
::========================================================
sed /"#define PRODUCT_VER_STRING"/!d ..\..\..\..\src\jrd\build_no.h > %temp%.\b$1.bat
sed -n -e s/\"//g -e s/"#define PRODUCT_VER_STRING "//w%temp%.\b$2.bat %temp%.\b$1.bat
for /f "tokens=*" %%a in ('cat %temp%.\b$2.bat') do set PRODUCT_VER_STRING=%%a
@echo s/1.5.0/%PRODUCT_VER_STRING%/ > %temp%.\b$3.bat
@echo s/define super_server_install/define %PACKAGE_TYPE%/ >> %temp%.\b$3.bat
@echo s/define server_architecture \"SuperServer\"/define server_architecture \"%PACKAGE_DESC%\"/ >> %temp%.\b$3.bat
@echo s/define msvc_version 6/define msvc_version %MSVC_VERSION%/ >> %temp%.\b$3.bat
@echo s/PRODUCT_VER_STRING/%PRODUCT_VER_STRING%/ >> %temp%.\b$3.bat

sed -f  %temp%.\b$3.bat FirebirdInstall_15.iss > FirebirdInstall_%PRODUCT_VER_STRING%_%PACKAGE_DESC%.iss

del %temp%.\b$?.bat
@goto :EOF


:COPY_XTRA
:: system dll's we need
::=====================
copy %SystemRoot%\System32\msvcrt.dll .
copy %SystemRoot%\System32\msvcp%msvc_version%0.dll .

:: grab some missing bits'n'pieces  from different parts of the source tree
::=========================================================================
copy ..\..\..\..\src\install\misc\firebird.conf ..\..\..\..\builds\win32\output\
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY of firebird.conf failed ) & (goto :EOF))

mkdir ..\..\..\..\builds\win32\output\examples 2>nul
@if %ERRORLEVEL% GEQ 2 ( (call :ERROR MKDIR for examples dir failed ) & (goto :EOF))

copy ..\..\..\..\src\v5_examples\*.* ..\..\..\..\builds\win32\output\examples\ 
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY examples failed  ) & (goto :EOF))

copy ..\..\..\..\builds\win32\msvc%msvc_version%\%BUILDTYPE%\fbclient\fbclient.lib ..\..\..\..\builds\win32\output\lib\fbclient_ms.lib
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY *.lib failed ) & (goto :EOF))

copy ..\..\..\..\builds\win32\msvc%msvc_version%\%BUILDTYPE%\gds32\gds32.lib ..\..\..\..\builds\win32\output\lib\gds32_ms.lib
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY *.lib failed ) & (goto :EOF))

copy ..\..\..\..\doc\*.* ..\..\..\..\builds\win32\output\doc\ 
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY doc failed  ) & (goto :EOF))

copy ..\..\..\..\ChangeLog ..\..\..\..\builds\win32\output\doc\ChangeLog.txt

copy ..\..\..\..\builds\win32\output\doc\install_win32.txt ..\..\..\..\builds\win32\output\doc\InstallNotes.txt
del ..\..\..\..\builds\win32\output\doc\install_win32.txt
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR Rename install_win32.txt failed ) & (goto :EOF))

:: This stuff doesn't make much sense to Windows users, although the troubleshooting doc 
:: could be made more platform agnostic.
@for %%v in (  README.makefiles README.user README.user.embedded README.user.troubleshooting fb2-todo.txt ) do (
  (@del ..\..\..\..\builds\win32\output\doc\%%v 2>nul)
)

copy ..\..\..\..\builds\win32\output\doc\WhatsNew ..\..\..\..\builds\win32\output\doc\WhatsNew.txt
del ..\..\..\..\builds\win32\output\doc\WhatsNew

mkdir ..\..\..\..\builds\win32\output\doc\sql.extensions 2>nul
@if %ERRORLEVEL% GEQ 2 ( (call :ERROR MKDIR for doc\sql.extensions dir failed ) & (goto :EOF))

copy ..\..\..\..\doc\sql.extensions\*.* ..\..\..\..\builds\win32\output\doc\sql.extensions\ 
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY doc\sql.extensions failed  ) & (goto :EOF))

@goto :EOF


:ALIAS_CONF
:: Generate a sample aliases file
::===============================
@echo # > ..\..\..\..\builds\win32\output\aliases.conf
@echo # List of known database aliases >> ..\..\..\..\builds\win32\output\aliases.conf
@echo # ------------------------------ >> ..\..\..\..\builds\win32\output\aliases.conf
@echo # >> ..\..\..\..\builds\win32\output\aliases.conf
@echo # Examples: >> ..\..\..\..\builds\win32\output\aliases.conf
@echo # >> ..\..\..\..\builds\win32\output\aliases.conf
@echo #   dummy = c:\data\dummy.fdb >> ..\..\..\..\builds\win32\output\aliases.conf
@echo #  >> ..\..\..\..\builds\win32\output\aliases.conf
@goto :EOF


:GBAK_SEC_DB
:: let's make sure that we have a backup of the security database handy.
::======================================================================
copy ..\..\..\misc\security.gbak ..\..\..\..\builds\win32\output\security.fbk
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR copy security.fbk failed ) & (goto :EOF))

:: Make sure that qli's help.gdb is available
:: For now it has the .gdb. file extension
:: Next time it will have the .fdb file extension
::===============================================
if not exist ..\..\..\..\builds\win32\output\help\help.fdb (copy ..\..\..\..\builds\win32\dbs\qli\help.fdb ..\..\..\..\builds\win32\output\help\help.fdb)
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR Could not copy qli help database ) & (goto :EOF))
@goto :EOF

:FB_MSG
::=================================================================
:: firebird.msg is generated as part of the build process
:: in builds\win32 by build_msg.bat copying from there to output dir
::	To Do !!!
::						copy %INTERBASE%\udf\fbudf.dll ..\..\..\..\builds\win32\output\udf
::=================================================================
@if not exist ..\..\..\..\builds\win32\firebird.msg (( 
						echo Cannot locate firebird.msg file. 
						) & ( 
						echo You need to run the build_msg scriptfile.
						)
						) else (
						copy ..\..\..\..\builds\win32\firebird.msg ..\..\..\..\builds\win32\output\firebird.msg
						)
@goto :EOF

:TOUCH_ALL
::========
::Set file timestamp to something meaningful.
::While building and testing this feature might be annoying.
::We ought to test for a debug build - but what?
::==========================================================
@if /I "%BUILDTYPE%"=="release" ((@echo Touching files) & (touch -s -D -t01:05:00 ..\..\..\..\builds\win32\output\*.*)) 
@goto :EOF


:RUN_ISX
::=======
:: Now let's go and build the installable .exe
:: Obviously this will fail if InnoSetup 
:: Extensions is not installed.
::=================================================

start FirebirdInstall_%PRODUCT_VER_STRING%_%PACKAGE_DESC%.iss
@goto :EOF


:HELP
::===
@echo.
@echo   If you wish to build a Classic Server package
@echo   you can pass CS as a parameter.
@echo.
@echo   If you are testing a debug build you can
@echo   pass DEBUG as a parameter to the script.
@echo.
@echo   Parameter order is not relevant
@echo.
@echo   If no parameters are passed a production build
@echo   of the Super Server will be created.
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
@(@call :SET_PARAMS %1 %2 )|| (@goto :EOF) 
@(@call :SED_MAGIC ) || (@goto :EOF)
@(@call :COPY_XTRA ) || (@goto :EOF)
@(@call :ALIAS_CONF ) || (@goto :EOF)
@(@call :GBAK_SEC_DB ) || (@goto :EOF)
@(@call :FB_MSG ) || (@goto :EOF)
::@(@call :TOUCH_ALL ) || (@goto :EOF)
@(@call :RUN_ISX ) || (@goto :EOF)
@goto :EOF

:END
