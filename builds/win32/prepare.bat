
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
	@set FIREBIRD= %FIREBIRD% %1
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
@"%FIREBIRD%\bin\gbak" -r %ROOT_PATH%\src\misc\metadata.gbak localhost:%DB_PATH%\gen\dbs\metadata.fdb
@"%FIREBIRD%\bin\gbak" -r %ROOT_PATH%\src\misc\security.gbak localhost:%DB_PATH%\gen\dbs\security.fdb
@"%FIREBIRD%\bin\gbak" -r %ROOT_PATH%\src\msgs\msg.gbak localhost:%DB_PATH%\gen\dbs\msg.fdb
@"%FIREBIRD%\bin\gbak" -r %ROOT_PATH%\src\misc\help.gbak localhost:%DB_PATH%\gen\dbs\help.fdb
@copy %ROOT_PATH%\gen\dbs\metadata.fdb %ROOT_PATH%\gen\dbs\yachts.lnk

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
