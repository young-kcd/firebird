
@echo off

::===========
:MAIN
@if "%1"=="" (@goto :ERROR_PARAM_EXPECTED)


::Make sure that the db path is set to a style that wont break SED
set DB_PATH=%1
for /f "tokens=*" %%a in ('@echo %DB_PATH:\=/%') do (set DB_PATH=%%a)

if "%2" NEQ "" (set FIREBIRD="%2")
@md dbs
@cd dbs


@%FIREBIRD%\bin\gbak -r %1\src\misc\metadata.gbak localhost:%DB_PATH\builds\win32\dbs\metadata.fdb
@md jrd
@%FIREBIRD%\bin\gbak -r %1\src\misc\security.gbak localhost:%DB_PATH\builds\win32\dbs\jrd\security.fdb
@md msgs
@%FIREBIRD%\bin\gbak -r %1\src\msgs\msg.gbak localhost:%DB_PATH\builds\win32\dbs\msgs\msg.fdb
@md qli
@%FIREBIRD%\bin\gbak -r %1\src\misc\help.gbak localhost:%DB_PATH\builds\win32\dbs\qli\help.fdb
@cd ..
@del expand.sed
@call :SED yachts.lnk %1 metadata.fdb
@call :SED ODS.RDB %1 jrd/security.fdb
@call :SED msg.fdb %1 msgs/msg.fdb
@call :SED master_msg_db %1 msgs/msg.fdb
@call :SED yachts.link %1 jrd/security.fdb
@call :SED master_help_db %1 qli/help.fdb
@call :SED help.fdb %1 qli/help.fdb
@call :SED yachts.gdb %1 jrd/security.fdb
@call :SED isc.gdb %1 jrd/security.fdb
@call :SED isc4.gdb %1 jrd/security.fdb
@call :SED security.fdb %1 jrd/security.fdb
@call :SED empbuild.gdb %1 example5/employee.fdb
@call :SED source/msgs/msg.fdb %1 msgs/msg.fdb
@goto :END

::===========
:SED
@echo s?\"%1\"?\"localhost:%2/builds/win32/dbs/%3\"?g >> expand.sed
@goto :EOF

::===========
:ERROR_PARAM_EXPECTED
@echo You must specify the root directory of the Firebird source tree.
@echo Example:
@echo     prepare.bat d:\work\firebird
@echo.
@echo If gbak is not available on your path specify the  
@echo root directory of your Firebird installation.
@echo Example:
@echo c:\program files\firebird
@echo. 
@goto :END

:END
