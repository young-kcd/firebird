
@echo off

set ERRLEV=0

if not defined FB_NOCLEAN (call clean_all)
call make_icu
if "%ERRLEV%"=="1" goto :END
call make_boot
if "%ERRLEV%"=="1" goto :END
call make_all
if "%ERRLEV%"=="1" goto :END
call make_examples
if "%ERRLEV%"=="1" goto :END
pushd ..\install\arch-specific\win32
call BuildExecutableInstall ISX ZIP EMB PDB
popd

goto:END


:END
