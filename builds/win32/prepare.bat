
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

@echo create database '%SERVER_NAME%:%DB_PATH%\gen\dbs\security.fdb'; | "%FIREBIRD%\bin\isql" -q
@set SEC_ISQL=@"%FIREBIRD%\bin\isql" -q %SERVER_NAME%:%DB_PATH%\gen\dbs\security.fdb -i %ROOT_PATH%\src\dbs\
@%SEC_ISQL%security.sql

@"%FIREBIRD%\bin\gbak" -r %ROOT_PATH%\builds\misc\metadata.gbak %SERVER_NAME%:%DB_PATH%\gen\dbs\metadata.fdb

@echo create database '%SERVER_NAME%:%DB_PATH%\gen\dbs\msg.fdb'; | "%FIREBIRD%\bin\isql" -q
@set MSG_ISQL=@"%FIREBIRD%\bin\isql" -q %SERVER_NAME%:%DB_PATH%\gen\dbs\msg.fdb -i %ROOT_PATH%\src\msgs\
@%MSG_ISQL%msg.sql 
@%MSG_ISQL%facilities.sql
@%MSG_ISQL%locales.sql 
@%MSG_ISQL%history.sql 
@%MSG_ISQL%messages.sql 
@%MSG_ISQL%symbols.sql 
@%MSG_ISQL%system_errors.sql 
@%MSG_ISQL%transmsgs.fr_FR.sql 
@%MSG_ISQL%transmsgs.de_DE.sql 

@"%FIREBIRD%\bin\gbak" -r %ROOT_PATH%\builds\misc\help.gbak %SERVER_NAME%:%DB_PATH%\gen\dbs\help.fdb 
@copy %ROOT_PATH%\gen\dbs\metadata.fdb %ROOT_PATH%\gen\dbs\yachts.lnk > nul

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

