
@echo off

@echo.
@echo    !!  Warning !!
@echo    This script no longer requires the 
@echo   path to the source. It now works it  
@echo   out automatically.
@echo.
@echo   Paremeter 1 will now set the FIREBIRD variable
@echo   This may not be what you intended 
@echo.



::Check if on-line help is required
@if /I "%1"=="-h" (goto :HELP & goto :EOF)
@if /I "%1"=="/h" (goto :HELP & goto :EOF)
@if /I "%1"=="-?" (goto :HELP & goto :EOF)
@if /I "%1"=="/?" (goto :HELP & goto :EOF)

if "%1" NEQ "" (set FIREBIRD="%1")
if "%FIREBIRD%"=="" (goto :HELP & goto :EOF)
@goto :MAIN

:SET_DB_DIR
@cd ..\..
@for /f "delims=" %%a in ('@cd') do (set ROOT_PATH=%%a)
@echo Setting root path to %ROOT_PATH%
@cd %~dp0
::for /f "delims=" %%a in ('cd') do (@echo %%a)
@goto :EOF

::===========
:MAIN
call :SET_DB_DIR

::Make sure that the db path is set to a style that wont break SED
for /f "tokens=*" %%a in ('@echo %ROOT_PATH:\=/%') do (set DB_PATH=%%a)

@md dbs 2>nul
@cd dbs
@echo Creating databases

@%FIREBIRD%\bin\gbak -r %ROOT_PATH%\src\misc\metadata.gbak localhost:%DB_PATH%\builds\win32\dbs\metadata.fdb
@md jrd 2>nul
@%FIREBIRD%\bin\gbak -r %ROOT_PATH%\src\misc\security.gbak localhost:%DB_PATH%\builds\win32\dbs\jrd\security.fdb
@md msgs  2>nul
@%FIREBIRD%\bin\gbak -r %ROOT_PATH%\src\msgs\msg.gbak localhost:%DB_PATH%\builds\win32\dbs\msgs\msg.fdb
@md qli 2>nul
@%FIREBIRD%\bin\gbak -r %ROOT_PATH%\src\misc\help.gbak localhost:%DB_PATH%\builds\win32\dbs\qli\help.fdb
@cd ..

::provisional, copy all fdbs to the same directory to be used from preprocess
copy dbs\metadata.fdb %ROOT_PATH%\generated\yachts.lnk > nul
copy dbs\jrd\security.fdb %ROOT_PATH%\generated\security.fdb > nul
copy dbs\msgs\msg.fdb %ROOT_PATH%\generated\msg.fdb > nul
copy dbs\qli\help.fdb %ROOT_PATH%\generated\help.fdb > nul

@echo.
@echo Completed Preparations for build
@echo You many now continue by running BUILD_ALL.BAT
@echo.

@goto :END

::===========
:HELP
@echo.
@echo   If gbak is not available on your path specify the  
@echo   root directory of your Firebird installation.
@echo   Example:
@echo     c:\program files\firebird
@echo. 
@goto :END



:END
