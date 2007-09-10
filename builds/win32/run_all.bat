
@echo off

set ERRLEV=0

if not defined FB_NOCLEAN (call clean_all)
call make_icu %1
if "%ERRLEV%"=="1" goto :END
call make_boot %1
if "%ERRLEV%"=="1" goto :END
call make_all %1
if "%ERRLEV%"=="1" goto :END
call make_examples %1
if "%ERRLEV%"=="1" goto :END
pushd ..\install\arch-specific\win32
call BuildExecutableInstall ISX ZIP EMB
popd

goto:END


:END
