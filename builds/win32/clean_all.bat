@echo off

@call setenvvar.bat
@if errorlevel 1 (goto :END)

@echo Cleaning temporary directories
@rmdir /S /Q %FB_OUTPUT_DIR% 2>nul
@rmdir /S /Q %FB_ROOT_PATH%\gen 2>nul
@rmdir /S /Q %FB_TEMP_DIR% 2>nul

@echo Cleaning INTL
@rmdir /S /Q %FB_ROOT_PATH%\extern\icu\bin 2>nul
@rmdir /S /Q %FB_ROOT_PATH%\extern\icu\include 2>nul
@rmdir /S /Q %FB_ROOT_PATH%\extern\icu\lib 2>nul
@rmdir /S /Q %FB_ROOT_PATH%\extern\icu\source\data\out 2>nul
@del /Q %FB_ROOT_PATH%\extern\icu\source\extra\uconv\resources\*.res 2>nul
@del %FB_ROOT_PATH%\extern\icu\source\extra\uconv\pkgdatain.txt 2>nul
@del %FB_ROOT_PATH%\extern\icu\source\stubdata\stubdatabuilt.txt 2>nul
@rmdir /S /Q %FB_ROOT_PATH%\extern\icu\source\test\testdata\out 2>nul
@rmdir /S /Q %FB_ROOT_PATH%\extern\icu\source\tools\gencmn\decmn_Win32_Release 2>nul
@rmdir /S /Q %FB_ROOT_PATH%\extern\icu\source\tools\genrb\derb_Win32_Release 2>nul

::This really does clean the icu stuff - but is it too much? Not cleaning speeds 
::up the build process if a previous build has been made for the same target.
::for /R %FB_ROOT_PATH%\extern\icu\source\ %%a in (release) do (rmdir /q /s "%%a" 2>nul)
::for /R %FB_ROOT_PATH%\extern\icu\source\ %%a in (debug) do (rmdir /q /s "%%a" 2>nul)

@echo Cleaning install and build files
@del *%FB_TARGET_PLATFORM%.log 2>nul
@del *.manifest 2>nul
@del %FB_ROOT_PATH%\src\include\gen\autoconfig.h 2>nul
@del %FB_ROOT_PATH%\src\include\gen\blrtable.h 2>nul
@del %FB_ROOT_PATH%\builds\win32\msvc6\*.plg 2>nul
@rmdir /s /q %FB_ROOT_PATH%\builds\win32\install_image 2>nul

@del %FB_ROOT_PATH%\src\dsql\dsql.tab.h 2>nul
@del %FB_ROOT_PATH%\src\dsql\parse.cpp 2>nul

@echo Completed executing %0
@echo.

