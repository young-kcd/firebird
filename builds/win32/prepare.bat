
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

if "%1" NEQ "" (set FIREBIRD=%1)
if "%FIREBIRD%"=="" (goto :HELP & goto :EOF)

::===========
:MAIN
@call setenvvar.bat
@if errorlevel 1 (goto :END)

@md dbs 2>nul
@echo Creating databases

@"%FIREBIRD%\bin\gbak" -r %ROOT_PATH%\src\misc\metadata.gbak localhost:%DB_PATH%\builds\win32\dbs\metadata.fdb
@md %ROOT_PATH%\builds\win32\dbs\jrd 2>nul
@"%FIREBIRD%\bin\gbak" -r %ROOT_PATH%\src\misc\security.gbak localhost:%DB_PATH%\builds\win32\dbs\jrd\security.fdb
@md %ROOT_PATH%\builds\win32\dbs\msgs  2>nul
@"%FIREBIRD%\bin\gbak" -r %ROOT_PATH%\src\msgs\msg.gbak localhost:%DB_PATH%\builds\win32\dbs\msgs\msg.fdb
@md %ROOT_PATH%\builds\win32\dbs\qli 2>nul
@"%FIREBIRD%\bin\gbak" -r %ROOT_PATH%\src\misc\help.gbak localhost:%DB_PATH%\builds\win32\dbs\qli\help.fdb

@echo.
@echo Completed Preparations for build
@echo    You may now run make_boot.bat
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
