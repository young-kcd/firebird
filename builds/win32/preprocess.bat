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
@if "%1"=="BOOT" (set BOOTBUILD=1) else (set BOOTBUILD=0)
@echo.
@call :CHECKTOOLS
@if "%BOOTBUILD%"=="1" (call :BOOT_PROCESS) else (call :MASTER_PROCESS)
@set BOOTBUILD=
@set GPRE=
@goto :END

::===========
:PREPROCESS
@echo Processing %1/%2.epp
@del ..\..\generated\%1\%2.cpp
@sed -f expand.sed ..\..\src\%1\%2.epp > sed.tmp
@copy sed.tmp ..\..\generated\%1\%2.epp
@del sed.tmp
@echo Calling GPRE for %1/%2.epp
@if "%3"=="" (call :GPRE_M %1 %2) else (call :GPRE_GDS %1 %2)
@del ..\..\generated\%1\%2.epp
@echo.
@goto :EOF

::===========
:GPRE_M
@%GPRE% -n -m -raw ..\..\generated\%1\%2.epp ..\..\generated\%1\%2.cpp
@goto :EOF

::===========
:GPRE_GDS
@%GPRE% -n -gds -raw -ids ..\..\generated\%1\%2.epp ..\..\generated\%1\%2.cpp
goto :EOF

::=====================================

:CHECKTOOLS
:: verify our unix tool set is available

@sed --help > nul
@if errorlevel 1 ((goto :HELP_TOOLS) & (goto :EOF))
echo sed located on path

goto :EOF

::===========
:BOOT_PROCESS
@echo.
@set GPRE=gpre_boot -lang_internal
@for %%i in (array, blob, metd) do @call :PREPROCESS dsql %%i
@for %%i in (gpre_meta) do @call :PREPROCESS gpre %%i
@goto :EOF

::===========
:MASTER_PROCESS
@echo.
@set GPRE=gpre
@for %%i in (alice_meta) do @call :PREPROCESS alice %%i
@for %%i in (backup, restore) do @call :PREPROCESS burp %%i
@for %%i in (array, blob, metd) do @call :PREPROCESS dsql %%i
@for %%i in (exe, extract) do @call :PREPROCESS dudley %%i
@for %%i in (gpre_meta) do @call :PREPROCESS gpre %%i
@for %%i in (extract, isql, show) do @call :PREPROCESS isql %%i
@for %%i in (dfw, dpm, dyn, dyn_def, dyn_del, dyn_mod, dyn_util, envelope, fun, grant, ini, met, pcmet, scl, stats) do @call :PREPROCESS jrd %%i GDS
@for %%i in (codes) do @call :PREPROCESS jrd %%i
@for %%i in (build_file) do @call :PREPROCESS msgs %%i
@for %%i in (help, meta, proc, show) do @call :PREPROCESS qli %%i
@for %%i in (dba, security) do @call :PREPROCESS utilities %%i

@sed -f ..\..\src\jrd\dyn_def.sed ..\..\generated\jrd\dyn_def.cpp > sed.tmp
@del ..\..\generated\jrd\dyn_def.cpp
@copy sed.tmp ..\..\generated\jrd\dyn_def.cpp
@del sed.tmp

@goto :EOF

::=================
:HELP_TOOLS
@echo:
@echo   Please check that these utilities programs are
@echo   all on your path:
@echo:
@echo     sed.exe
@echo:
@echo   If you do not have these utilities they may be downloaded
@echo   via ftp from the Firebird Project on Sourceforge:
@echo:
@echo     http://prdownloads.sourceforge.net/firebird/
@echo:
@echo   filename:  Firebird_Unix_Tools_for_Win32.zip
@echo:
@goto :END

:END
