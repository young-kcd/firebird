
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

@if "%1" NEQ "" (set FIREBIRD=%1)
:: BRS 
:: Get all the file name when there are spaces
:: this can be also achieved with %* but I don't know which versions of 
:: windows allows it
:LOOP
	@shift
	@if "%1"==""  (goto DONE)
        @set FIREBIRD=%FIREBIRD% %1
	@goto loop
:DONE

@echo      FIREBIRD=%FIREBIRD%
@if "%FIREBIRD%"=="" (goto :HELP & goto :EOF)

::===========
:MAIN
@call setenvvar.bat
@if errorlevel 1 (goto :END)

@echo Creating databases

@rmdir /s /q %ROOT_PATH%\gen 2>nul
@mkdir %ROOT_PATH%\gen\dbs 2>nul
@cd %ROOT_PATH%\gen\dbs
@"%FIREBIRD%\bin\gbak" -r %ROOT_PATH%\builds\misc\metadata.gbak %SERVER_NAME%:%DB_PATH%\gen\dbs\metadata.fdb
@"%FIREBIRD%\bin\gbak" -r %ROOT_PATH%\builds\misc\security.gbak %SERVER_NAME%:%DB_PATH%\gen\dbs\security.fdb
@"%FIREBIRD%\bin\isql" -q -i %ROOT_PATH%\src\msgs\msg.sql
@"%FIREBIRD%\bin\isql" -q -i %ROOT_PATH%\src\msgs\facilities.sql
@"%FIREBIRD%\bin\isql" -q -i %ROOT_PATH%\src\msgs\locales.sql
@"%FIREBIRD%\bin\isql" -q -i %ROOT_PATH%\src\msgs\history.sql
@"%FIREBIRD%\bin\isql" -q -i %ROOT_PATH%\src\msgs\messages.sql
@"%FIREBIRD%\bin\isql" -q -i %ROOT_PATH%\src\msgs\symbols.sql
@"%FIREBIRD%\bin\isql" -q -i %ROOT_PATH%\src\msgs\system_errors.sql
@"%FIREBIRD%\bin\isql" -q -i %ROOT_PATH%\src\msgs\transmsgs.fr_FR.sql
@"%FIREBIRD%\bin\isql" -q -i %ROOT_PATH%\src\msgs\transmsgs.de_DE.sql
@"%FIREBIRD%\bin\gbak" -r %ROOT_PATH%\builds\misc\help.gbak %SERVER_NAME%:%DB_PATH%\gen\dbs\help.fdb
@copy metadata.fdb yachts.lnk > nul
@cd %ROOT_PATH%\builds\win32

@echo.
@echo Completed Preparations for build
@echo    You may now run make_boot.bat [DEBUG] [CLEAN]
@echo.

@goto :END

::===========
:HELP
@echo.
@echo   Build process need the FIREBIRD environment variable set to work.
@echo   FIREBIRD value should be the root directory of your Firebird installation.
@echo   Example:
@echo     c:\program files\firebird
@echo. 
@goto :END



:END

