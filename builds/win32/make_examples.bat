@echo off

:: Set env vars
@call setenvvar.bat
@if errorlevel 1 (goto :END)

:: verify that prepare was run before
@if not exist %ROOT_PATH%\gen\dbs\metadata.fdb (goto :HELP_PREP & goto :END)

:: verify that boot was run before
@if not exist %ROOT_PATH%\gen\gpre_boot.exe (goto :HELP_BOOT & goto :END)

::===========
:: Read input values
@set DBG=
@set DBG_DIR=release
@set CLEAN=/build
@if "%1"=="DEBUG" ((set DBG=TRUE) && (set DBG_DIR=debug))
@if "%2"=="DEBUG" ((set DBG=TRUE) && (set DBG_DIR=debug))
@if "%1"=="CLEAN" (set CLEAN=/REBUILD)
@if "%2"=="CLEAN" (set CLEAN=/REBUILD)

::===========
:MAIN
@call :BUILD_EMPBUILD
@if "%DBG%"=="" (call :RELEASE) else (call :DEBUG)
@call :MOVE
@call :BUILD_EMPLOYEE
@call :MOVE2
@goto :END

::===========
:BUILD_EMPBUILD
@echo.
@echo Building empbuild.fdb
@copy %ROOT_PATH%\output\bin\ %ROOT_PATH%\gen\examples\ > nul
@copy /y %ROOT_PATH%\examples\empbuild\*.sql   %ROOT_PATH%\gen\examples\ > nul
@copy /y %ROOT_PATH%\examples\empbuild\*.inp   %ROOT_PATH%\gen\examples\ > nul

@echo.
@echo Creating empbuild.fdb
:: Here we must use cd because isql does not have an option to set a base directory
@cd %ROOT_PATH%\gen\examples
@del empbuild.fdb 2> nul
@del intlbuild.fdb 2> nul
:: CVC: I need this line to ensure isql is the correct one.
@copy %ROOT_PATH%\output\bin\isql.exe %ROOT_PATH%\gen\examples\ /y > nul
@%ROOT_PATH%\gen\examples\isql -i empbld.sql
@%ROOT_PATH%\gen\examples\isql -i intlbld.sql
@cd %ROOT_PATH%\builds\win32

@echo.
@echo preprocessing empbuild.e and intlbld.e
@echo path = %DB_PATH%/gen/examples
@%ROOT_PATH%\gen\gpre_static -r -m -n -z %ROOT_PATH%\examples\empbuild\empbuild.e %ROOT_PATH%\gen\examples\empbuild.c -b %SERVER_NAME%:%DB_PATH%/gen/examples/
@%ROOT_PATH%\gen\gpre_static -r -m -n -z %ROOT_PATH%\examples\empbuild\intlbld.e %ROOT_PATH%\gen\examples\intlbld.c -b %SERVER_NAME%:%DB_PATH%/gen/examples/
@goto :EOF

::===========
:RELEASE
@echo.
@echo Building release
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2.dsw /MAKE "empbuild - Win32 Release" "intlbld - Win32 Release" %CLEAN% /OUT examples.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2.sln /project empbuild %CLEAN% /OUT empbuild.log
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2.sln /project intlbld %CLEAN% /OUT intlbld.log
)
@goto :EOF

::===========
:DEBUG
@echo.
@echo Building debug
if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2.dsw /MAKE "empbuild - Win32 Debug" "intlbld - Win32 Debug" %CLEAN% /OUT examples.log
) else (
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2.sln /project empbuild debug %CLEAN% /OUT empbuild.log
	@devenv %ROOT_PATH%\builds\win32\%VS_VER%\Firebird2.sln /project intlbld debug %CLEAN% /OUT intlbld.log
)
@goto :EOF


::===========
:MOVE
@echo.
@rmdir /q /s %ROOT_PATH%\output\examples 2>nul
@mkdir %ROOT_PATH%\output\examples
@mkdir %ROOT_PATH%\output\examples\api
@mkdir %ROOT_PATH%\output\examples\build_unix
@mkdir %ROOT_PATH%\output\examples\build_win32
@mkdir %ROOT_PATH%\output\examples\dyn
@mkdir %ROOT_PATH%\output\examples\empbuild
@mkdir %ROOT_PATH%\output\examples\include
@mkdir %ROOT_PATH%\output\examples\stat
@mkdir %ROOT_PATH%\output\examples\udf
@echo Moving files to output directory
@copy %ROOT_PATH%\examples\* %ROOT_PATH%\output\examples > nul
@copy %ROOT_PATH%\examples\api\* %ROOT_PATH%\output\examples\api > nul
@copy %ROOT_PATH%\examples\build_unix\* %ROOT_PATH%\output\examples\build_unix > nul
@copy %ROOT_PATH%\examples\build_win32\* %ROOT_PATH%\output\examples\build_win32 > nul
@copy %ROOT_PATH%\examples\dyn\* %ROOT_PATH%\output\examples\dyn > nul
:: @copy %ROOT_PATH%\examples\empbuild\* %ROOT_PATH%\output\examples\empbuild > nul
@copy %ROOT_PATH%\examples\empbuild\employe2.sql %ROOT_PATH%\output\examples\empbuild > nul
@copy %ROOT_PATH%\examples\include\* %ROOT_PATH%\output\examples\include > nul
@copy %ROOT_PATH%\examples\stat\* %ROOT_PATH%\output\examples\stat > nul
@copy %ROOT_PATH%\examples\udf\* %ROOT_PATH%\output\examples\udf > nul
@copy %ROOT_PATH%\src\extlib\ib_udf* %ROOT_PATH%\output\examples\udf > nul
@copy %ROOT_PATH%\src\extlib\fbudf\* %ROOT_PATH%\output\examples\udf > nul

:: @copy %ROOT_PATH%\gen\examples\empbuild.c %ROOT_PATH%\output\examples\empbuild\ > nul
@copy %ROOT_PATH%\temp\%DBG_DIR%\examples\empbuild.exe %ROOT_PATH%\gen\examples\empbuild.exe > nul
@copy %ROOT_PATH%\temp\%DBG_DIR%\examples\intlbld.exe %ROOT_PATH%\gen\examples\intlbld.exe > nul
@goto :EOF

::===========
:: only to test if it works
:BUILD_EMPLOYEE
@echo.
@echo Building employee.fdb
:: Here we must use cd because isql does not have an option to set a base directory
:: and empbuild.exe uses isql
@cd %ROOT_PATH%\gen\examples
@del %ROOT_PATH%\gen\examples\employee.fdb 2>nul
@del %ROOT_PATH%\gen\examples\intlemp.fdb 2>nul
@%ROOT_PATH%\gen\examples\empbuild.exe %DB_PATH%/gen/examples/employee.fdb
:: The script intldml.sql contains a reference to intlemp.fdb that must be changed
@del isql.tmp 2>nul
@echo s;intlemp.fdb;%SERVER_NAME%:%ROOT_PATH%\gen\examples\intlemp.fdb;g > isql.tmp
@%ROOT_PATH%\gen\examples\intlbld.exe %DB_PATH%/gen/examples/intlemp.fdb
@cd %ROOT_PATH%\builds\win32

@goto :EOF

::==============
:MOVE2
@copy %ROOT_PATH%\gen\examples\employee.fdb %ROOT_PATH%\output\examples\empbuild\ > nul
@copy %ROOT_PATH%\gen\examples\intlemp.fdb %ROOT_PATH%\output\examples\empbuild\ > nul

@goto :EOF

::==============
:HELP_PREP
@echo.
@echo    You must run prepare.bat before running this script
@echo.
@goto :EOF

::==============
:HELP_BOOT
@echo.
@echo    You must run make_boot.bat before running this script
@echo.
@goto :EOF

:END