
@echo off

:: Reset or clear some variables, as appropriate.
set ERRLEV=0
set FB_NOCLEAN=
set FBBUILD_BUILDTYPE=release
set FBBUILD_INCLUDE_PDB=

:: Read the command line
for %%v in ( %* )  do (
( if /I "%%v"=="NOCLEAN" (set FB_NOCLEAN=1) )
( if /I "%%v"=="DEBUG" (set FBBUILD_BUILDTYPE=debug) )
( if /I "%%v"=="PDB" (set FBBUILD_INCLUDE_PDB=1) )
)

:: Go to work
if not defined FB_NOCLEAN (call clean_all)
:: We do not support debug builds of icu, so we don't pass %FBBUILD_BUILDTYPE%
call make_icu
if "%ERRLEV%"=="1" goto :END
call make_boot %FBBUILD_BUILDTYPE%
if "%ERRLEV%"=="1" goto :END
call make_all %FBBUILD_BUILDTYPE%
if "%ERRLEV%"=="1" goto :END
call make_examples %FBBUILD_BUILDTYPE%
if "%ERRLEV%"=="1" goto :END

:: Package everything up
pushd ..\install\arch-specific\win32
call BuildExecutableInstall ISX ZIP EMB %FBBUILD_BUILDTYPE%
if "%ERRLEV%"=="1" (popd & goto :END)
if defined FBBUILD_INCLUDE_PDB (
set FBBUILD_PACKAGE_NUMBER=
call BuildExecutableInstall ISX ZIP EMB %FBBUILD_BUILDTYPE% PDB
)
popd

goto:END


:END
