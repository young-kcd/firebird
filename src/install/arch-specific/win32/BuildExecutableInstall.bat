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



@goto :MAIN
@goto :EOF


:SET_PARAMS
::Uncomment as required
::set PACKAGE_TYPE=classic_server_install
set PACKAGE_TYPE=super_server_install
goto :EOF


:SED_MAGIC
:: Do some sed magic to make sure that the final product 
:: includes the version string in the filename.
:: If the Firebird Unix tools for Win32 aren't on 
:: the path this will fail!
::======================================================
sed /"#define PRODUCT_VER_STRING"/!d ..\..\..\..\src\jrd\build_no.h > %temp%.\b$1.bat
sed -n -e s/\"//g -e s/"#define PRODUCT_VER_STRING "//w%temp%.\b$2.bat %temp%.\b$1.bat
for /f "tokens=*" %%a in ('cat %temp%.\b$2.bat') do set PRODUCT_VER_STRING=%%a
@echo s/1.5.0/%PRODUCT_VER_STRING%/ > %temp%.\b$3.bat
@echo s/define super_server_install/define %PACKAGE_TYPE%/ >> %temp%.\b$3.bat
sed -f  %temp%.\b$3.bat FirebirdInstall_15.iss > FirebirdInstall%PRODUCT_VER_STRING%.iss

::@del %temp%.\b$?.bat
::sed s/1.5.0/%PRODUCT_VER_STRING%/ s/define super_server_install/define %PACKAGE_TYPE%/ FirebirdInstall_15.iss > FirebirdInstall%PRODUCT_VER_STRING%.iss
@goto :EOF




:COPY_XTRA
:: system dll's we need
::=====================
copy %SystemRoot%\System32\msvcrt.dll .
copy %SystemRoot%\System32\msvcp60.dll .

:: grab some missing bits'n'pieces  from different parts of the source tree
::=========================================================================
copy ..\..\..\..\src\install\misc\firebird.conf ..\..\..\..\builds\win32\output\
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY of firebird.conf failed ) & (@goto :EOF))

mkdir ..\..\..\..\builds\win32\output\examples
@if %ERRORLEVEL% GEQ 2 ( (call :ERROR MKDIR or examples dir failed ) & (@goto :EOF))

copy ..\..\..\..\src\v5_examples\*.* ..\..\..\..\builds\win32\output\examples\
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY examples failed  ) & (@goto :EOF))


copy ..\..\..\..\builds\win32\msvc6\debug\fbclient\fbclient.lib ..\..\..\..\builds\win32\output\lib\fbclient_ms.lib
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY *.lib failed ) & (@goto :EOF))

copy ..\..\..\..\builds\win32\msvc6\debug\gds32\gds32.lib ..\..\..\..\builds\win32\output\lib\gds32_ms.lib
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR COPY *.lib failed ) & (@goto :EOF))

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
gbak -b ..\..\..\..\builds\win32\output\security.fdb ..\..\..\..\builds\win32\output\security.fbk
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR GBAK failed ) & (@goto :EOF))

:: Make sure that qli's help.gdb is available
:: For now it has the .gdb. file extension
:: Next time it will have the .fdb file extension
::===============================================
if not exist ..\..\..\..\builds\win32\output\help\help.fdb (copy ..\..\..\..\builds\win32\dbs\qli\help.fdb ..\..\..\..\builds\win32\output\help\help.fdb)
@if %ERRORLEVEL% GEQ 1 ( (call :ERROR Could not copy qli help database ) & (@goto :EOF))
@goto :EOF

:FB_MSG
::=================================================================
:: firebird.msg is not being generated as part of the build process
:: so we need to cheat - let's get it from our existing install of 
:: Firebird 1.0
::=================================================================
@if "%INTERBASE%" == "" (( 
						echo Cannot locate Firebird 1.0 install
						) & ( 
						echo The interbase.msg file is needed to build the Firebird 1.5 kit
						) & (
						echo Is the INTERBASE env var defined?
						)
						) else (
						copy %INTERBASE%\interbase.msg ..\..\..\..\builds\win32\output\firebird.msg
						copy %INTERBASE%\udf\fbudf.dll ..\..\..\..\builds\win32\output\udf
						)
@goto :EOF

:TOUCH_ALL
::========
::Set file timestamp to something meaningful.
::While building and testing this feature might be annoying.
::We ought to test for a debug build - but what?
::==========================================================
touch -s -D -t01:05:00 ..\..\..\..\builds\win32\output\*.* 
@goto :EOF


:RUN_ISX
::=======
:: Now let's go and build the installable .exe
:: Obviously this will fail if InnoSetup 
:: Extensions is not installed.
::=================================================

start FirebirdInstall%PRODUCT_VER_STRING%.iss
@goto :EOF


:HELP_PARAMS
::==========
@echo.
@echo You must specify CS for a Classic Server package
@echo or SS for to build a Super Server package
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
@(@call :SET_PARAMS )|| (@goto :EOF) 
@(@call :SED_MAGIC ) || (@goto :EOF)
@(@call :COPY_XTRA ) || (@goto :EOF)
@(@call :ALIAS_CONF ) || (@goto :EOF)
@(@call :GBAK_SEC_DB ) || (@goto :EOF)
@(@call :FB_MSG ) || (@goto :EOF)
@(@call :TOUCH_ALL ) || (@goto :EOF)
@(@call :RUN_ISX ) || (@goto :EOF)
@goto :EOF

:END