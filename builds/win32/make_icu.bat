@echo off

set CLEAN=/build
if "%1"=="CLEAN" (set CLEAN=/rebuild)

if "%VS_VER%"=="msvc6" (
	@msdev %ROOT_PATH%\extern\icu\source\allinone\allinone.dsw /MAKE "all - Win32 Release" %CLEAN% /OUT icu.log
) else (
	@devenv %ROOT_PATH%\extern\icu\source\allinone\allinone.sln %CLEAN% release /OUT icu.log
)
