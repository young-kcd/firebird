::
:: Preprocess all .epp files to .cpp
:: ---------------------------------
::
::  To do:
::
::  o Better/Some documentation as to what this does
::  o Add some logging/diagnostics, so we can see what is happening
::    and track errors.
::  o License boiler plate, copyright and authorship acknowledgements
::

@echo off

::===========
:MAIN

@call setenvvar.bat

@if "%1"=="BOOT" (set BOOTBUILD=1) else (set BOOTBUILD=0)
@echo.
@if "%BOOTBUILD%"=="1" (call :BOOT_PROCESS) else (call :MASTER_PROCESS)
@set BOOTBUILD=
@set GPRE=
@goto :END

::===========
:PREPROCESS
@echo Processing %1/%2.epp
@del %ROOT_PATH%\gen\%1\%2.cpp 2>nul
@echo Calling GPRE for %1/%2.epp
@if "%3"=="" (call :GPRE_M %1 %2) else (call :GPRE_GDS %1 %2)
@echo.
@goto :EOF

::===========
:GPRE_M
@%GPRE% -n -m -raw %ROOT_PATH%\src\%1\%2.epp %ROOT_PATH%\gen\%1\%2.cpp -b %SERVER_NAME%:%DB_PATH%/gen/dbs/
@goto :EOF

::===========
:GPRE_GDS
@%GPRE% -n -gds -raw -ids %ROOT_PATH%\src\%1\%2.epp %ROOT_PATH%\gen\%1\%2.cpp -b %SERVER_NAME%:%DB_PATH%/gen/dbs/
goto :EOF

::===========
:BOOT_PROCESS
@echo.
@set GPRE=%ROOT_PATH%\gen\gpre_boot -lang_internal
@for %%i in (array, blob, metd) do @call :PREPROCESS dsql %%i
@for %%i in (gpre_meta) do @call :PREPROCESS gpre %%i
::CVC - Uncomment this line if you can't compile when new fields are added. See below.
::@for %%i in (backup, restore) do @call :PREPROCESS burp %%i
@goto :EOF

::===========
:MASTER_PROCESS
@echo.
@set GPRE=%ROOT_PATH%\gen\gpre_static
@for %%i in (alice_meta) do @call :PREPROCESS alice %%i
::CVC - Comment this line if you can't compile when new fields are added. See above.
@for %%i in (backup, restore) do @call :PREPROCESS burp %%i
@for %%i in (array, blob, metd) do @call :PREPROCESS dsql %%i
@for %%i in (exe, extract) do @call :PREPROCESS dudley %%i
@for %%i in (gpre_meta) do @call :PREPROCESS gpre %%i
@for %%i in (extract, isql, show) do @call :PREPROCESS isql %%i
@for %%i in (dfw, dpm, dyn, dyn_def, dyn_del, dyn_mod, dyn_util, fun, grant, ini, met, pcmet, scl) do @call :PREPROCESS jrd %%i GDS
@for %%i in (codes) do @call :PREPROCESS misc %%i
@for %%i in (build_file) do @call :PREPROCESS msgs %%i
@for %%i in (help, meta, proc, show) do @call :PREPROCESS qli %%i
@for %%i in (dba) do @call :PREPROCESS utilities/gstat %%i
@for %%i in (security) do @call :PREPROCESS utilities/gsec %%i
@for %%i in (stats) do @call :PREPROCESS utilities %%i

@goto :EOF

:END

